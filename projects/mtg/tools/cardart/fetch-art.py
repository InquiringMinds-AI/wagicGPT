#!/usr/bin/env python3
"""Fetch card art from Scryfall into the master card-art pool.

Wagic card ids (the `id=` lines in Res/sets/<SET>/_cards.dat) ARE Gatherer
multiverse ids, and Scryfall indexes every card by them — Scryfall is the
community-licensed source (WotC Fan Content Policy) and the one this project
already used for the 2026-08 English-refetch campaign.

Pipeline position: this fills/updates the POOL (full-size art only, one jpg per
card). The pack-*.sh scripts derive every platform's resizes and thumbnails
FROM the pool — thumbnails are never downloaded.

Resolution ladder per card id:
  1. bulk map  (multiverse id -> image_uris.large URL, English cards only)
  2. api.scryfall.com/cards/multiverse/<id>   (foreign-only ids land here)
  3. /cards/named?exact=<name from _cards.dat> — borrow the English printing
     of the same card by name (owner ruling 2026-08-02). Reached both when
     step 2 returns a non-English print AND when Scryfall doesn't index the
     multiverse id at all (the Salvat sets 404 there). A card with no English
     printing is SKIPPED -> engine text fallback.
Misses are appended to misses.txt next to this script.

The bulk map is the reduced form of Scryfall's default_cards bulk file
(~2 GB download, stream-parsed, deleted after reduction). The 2026-08
campaign's map at User/_bulk_map_large.json is picked up automatically;
--rebuild-map fetches a fresh bulk file (new sets need this).

Network manners: custom User-Agent, >=120 ms between requests (Scryfall asks
for 50-100 ms minimum). Bulk-file downloads are exempt from that limit but
there is only one of them.

WOTC-derived scans: local + his devices only. Never committed, never in a
release artifact.  `image_uris.large` (672x936) — NOT `normal` — per the
recorded 2026-08-03 regression.
"""
import argparse, json, os, re, sys, time, urllib.parse, urllib.request

HERE = os.path.dirname(os.path.abspath(__file__))
def _find_repo(start):
    d = start
    for _ in range(8):
        if os.path.isdir(os.path.join(d, "projects", "mtg")):
            return d
        d = os.path.dirname(d)
    return os.path.abspath(os.path.join(start, "..", ".."))
REPO = _find_repo(HERE)
DEF_RES_SETS = os.path.join(REPO, "projects/mtg/bin/Res/sets")
DEF_POOL_BASE = os.path.join(REPO, "projects/mtg/bin/User/sets")
DEF_MAP = os.path.join(REPO, "projects/mtg/bin/User/_bulk_map_large.json")
UA = "wagicGPT-cardart/0.1 (+https://github.com/InquiringMinds-AI/wagicGPT)"
THROTTLE = 0.12  # seconds between requests; Scryfall asks >=0.05-0.1

_last_req = [0.0]

def http_get(url, accept="*/*", timeout=60):
    wait = THROTTLE - (time.monotonic() - _last_req[0])
    if wait > 0:
        time.sleep(wait)
    req = urllib.request.Request(url, headers={"User-Agent": UA, "Accept": accept})
    try:
        with urllib.request.urlopen(req, timeout=timeout) as r:
            data = r.read()
    finally:
        _last_req[0] = time.monotonic()
    return data

def img_url(card, mid=None, name=None):
    """Best image URL for a Scryfall card object, for the FACE that `mid`/`name`
    names.

    A transform DFC carries one image_uris per face and one multiverse id per
    face, in the same order, and a single-image card carries image_uris at the
    top level.  Always taking faces[0] gave every BACK face the FRONT's art
    (the 2026-09-02 Heliod report: 207 of 740 back faces in Res/sets were
    front-art duplicates).  Resolution: the face at `mid`'s index in
    multiverse_ids; else the face whose name equals `name` (the name-borrow
    ladder asks by the back face's own name); else the front face."""
    faces = card.get("card_faces") or []
    uris = card.get("image_uris") or {}
    if not uris and faces:
        face = None
        mids = card.get("multiverse_ids") or []
        if mid is not None and len(faces) == len(mids):
            try:
                face = faces[mids.index(int(mid))]
            except (ValueError, TypeError):
                face = None
        if face is None and name:
            for f in faces:
                if (f.get("name") or "").lower() == name.lower():
                    face = f
                    break
        if face is None:
            face = faces[0]
        uris = face.get("image_uris") or {}
    for k in ("large", "normal", "png"):
        if uris.get(k):
            return uris[k]
    return None

def detect_pool(base):
    """The dev pool hides itself as sets/sets.hidden so the dev build boots artless."""
    hidden = os.path.join(base, "sets.hidden")
    return hidden if os.path.isdir(hidden) else base

def parse_set_ids(res_sets, set_name):
    """(id, name) pairs from a set's _cards.dat (bytes: CRLF + non-UTF8 accents).

    Positive ids only; the name (primitive= or name=, whichever the block has)
    feeds the English-borrow fallback for ids Scryfall doesn't index."""
    dat = os.path.join(res_sets, set_name, "_cards.dat")
    if not os.path.isfile(dat):
        return []
    cards, cid, cname = [], None, None
    with open(dat, "rb") as f:
        for line in f:
            line = line.strip()
            if line == b"[/card]":
                if cid:
                    cards.append((cid, cname))
                cid = cname = None
                continue
            m = re.match(rb"id=(\d+)$", line)
            if m:
                cid = m.group(1).decode()
                continue
            m = re.match(rb"(?:primitive|name)=(.+)$", line)
            if m and not cname:
                cname = m.group(1).decode("utf-8", "replace")
    return cards

def stream_json_array(path):
    """Yield objects from a (possibly multi-GB) JSON array without loading it whole."""
    dec = json.JSONDecoder()
    with open(path, "r", encoding="utf-8") as f:
        buf = f.read(1 << 20)
        buf = buf[buf.find("[") + 1:]
        while True:
            buf = buf.lstrip(" \t\r\n,")
            if buf.startswith("]"):
                return
            try:
                obj, end = dec.raw_decode(buf)
            except json.JSONDecodeError:
                more = f.read(1 << 20)
                if not more:
                    return
                buf += more
                continue
            yield obj
            buf = buf[end:]

def rebuild_map(map_path, keep_bulk=False):
    print("fetching bulk-data index ...")
    index = json.loads(http_get("https://api.scryfall.com/bulk-data", "application/json"))
    uri = next(e["download_uri"] for e in index["data"] if e["type"] == "default_cards")
    bulk = os.path.join(HERE, "default_cards.json")
    print("downloading %s (~2 GB) ..." % uri)
    req = urllib.request.Request(uri, headers={"User-Agent": UA})
    with urllib.request.urlopen(req, timeout=600) as r, open(bulk, "wb") as out:
        while True:
            chunk = r.read(1 << 20)
            if not chunk:
                break
            out.write(chunk)
    print("reducing to multiverse-id -> large-url map ...")
    mapping, n = {}, 0
    for card in stream_json_array(bulk):
        n += 1
        if card.get("lang") != "en":
            continue  # foreign-only ids must fall through to the English-borrow ladder
        for mid in card.get("multiverse_ids") or []:
            url = img_url(card, mid)  # per-face for DFCs, shared image otherwise
            if url:
                mapping[str(mid)] = url
    with open(map_path, "w") as f:
        json.dump(mapping, f)
    if not keep_bulk:
        os.unlink(bulk)
    print("map: %d ids from %d bulk cards -> %s" % (len(mapping), n, map_path))
    return mapping

def api_lookup(mid, name, misses):
    """Fallback: per-id API, borrowing the English printing by name when the id
    is foreign or (Salvat sets) not indexed by Scryfall at all."""
    card = None
    try:
        card = json.loads(http_get("https://api.scryfall.com/cards/multiverse/%s" % mid,
                                   "application/json"))
    except Exception:
        pass  # unindexed id — fall through to the name borrow
    if card is None or card.get("lang") != "en":
        name = (card or {}).get("name") or name
        if not name:
            misses.append((mid, "id not on Scryfall and no name in _cards.dat"))
            return None
        try:
            card = json.loads(http_get(
                "https://api.scryfall.com/cards/named?exact=" + urllib.parse.quote(name),
                "application/json"))
        except Exception as e:
            misses.append((mid, "no English printing of %r (%s)" % (name, e)))
            return None
    url = img_url(card, mid, name)
    if not url:
        misses.append((mid, "card has no image_uris"))
    return url

def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("sets", nargs="*", help="set codes (default: every set under Res/sets)")
    ap.add_argument("--pool", default=None, help="pool dir (default: auto-detect %s[/sets.hidden])" % DEF_POOL_BASE)
    ap.add_argument("--res-sets", default=DEF_RES_SETS)
    ap.add_argument("--map", default=DEF_MAP, help="reduced bulk map json")
    ap.add_argument("--rebuild-map", action="store_true", help="fetch fresh Scryfall bulk data first")
    ap.add_argument("--keep-bulk", action="store_true", help="keep the raw bulk file after reducing")
    ap.add_argument("--limit", type=int, default=0, help="stop after N fetch attempts (smoke tests)")
    ap.add_argument("--dry-run", action="store_true", help="count what would be fetched, no network")
    args = ap.parse_args()

    pool = args.pool or detect_pool(DEF_POOL_BASE)
    sets = args.sets or sorted(
        d for d in os.listdir(args.res_sets)
        if os.path.isfile(os.path.join(args.res_sets, d, "_cards.dat")))

    if args.rebuild_map:
        mapping = rebuild_map(args.map, args.keep_bulk)
    elif os.path.isfile(args.map):
        mapping = json.load(open(args.map))
    else:
        print("no map at %s — run with --rebuild-map" % args.map)
        return 1
    print("map: %d ids | pool: %s | sets: %d" % (len(mapping), pool, len(sets)))

    misses, got, present, todo = [], 0, 0, 0
    try:
        for s in sets:
            for mid, name in parse_set_ids(args.res_sets, s):
                dest = os.path.join(pool, s, mid + ".jpg")
                if os.path.isfile(dest):
                    present += 1
                    continue
                todo += 1
                if args.dry_run:
                    continue
                if args.limit and todo > args.limit:
                    raise KeyboardInterrupt
                url = mapping.get(mid) or api_lookup(mid, name, misses)
                if not url:
                    continue
                try:
                    data = http_get(url, "image/*")
                except Exception as e:
                    misses.append((mid, "download: %s" % e))
                    continue
                os.makedirs(os.path.dirname(dest), exist_ok=True)
                tmp = dest + ".part"
                with open(tmp, "wb") as f:
                    f.write(data)
                os.replace(tmp, dest)
                got += 1
                if got % 200 == 0:
                    print("  %d fetched ..." % got)
    except KeyboardInterrupt:
        print("stopped early")

    if misses:
        with open(os.path.join(HERE, "misses.txt"), "a") as f:
            for mid, why in misses:
                f.write("%s\t%s\n" % (mid, why))
    print("present=%d %s=%d fetched=%d missed=%d"
          % (present, "missing" if args.dry_run else "wanted", todo, got, len(misses)))
    return 0

if __name__ == "__main__":
    sys.exit(main())
