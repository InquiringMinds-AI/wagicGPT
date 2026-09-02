#!/usr/bin/env python3
"""Repair transform-DFC BACK-FACE card art (wave-53 lane V).

Why this exists
---------------
Wagic ids are Gatherer multiverse ids, and a transform DFC has ONE id per FACE:
in Res/sets/MOM/_cards.dat, "Heliod, the Radiant Dawn" is 607029 and "Heliod,
the Warped Eclipse" is 607030, the very next entry, carrying rarity=T.

download_card_images.py's --bulk path used to map EVERY multiverse id of a card
onto card_faces[0]["image_uris"] -- the FRONT face -- so every back face it
touched was written as a byte-identical copy of its own front art.  On the
owner's Vita a transformed Heliod therefore rendered an untransformed Heliod
(and, because the engine also collapsed the two MOM printings onto one, the
OTHER printing's untransformed art).  The engine half is fixed in
MTGAllCards::getOtherFaceCard; this repairs the pictures.

Scryfall's per-multiverse-id image endpoint DOES return the right face for a
back-face id, so re-fetching each back face by its own id is the whole fix.
This script only touches ids that are wrong (a byte-copy of the preceding
entry's art) or missing, and leaves every already-correct file alone.

Usage:
  python3 fix-dfc-backface-art.py --dry-run
  python3 fix-dfc-backface-art.py            # download + write
  python3 fix-dfc-backface-art.py --sets MOM
"""
import argparse, hashlib, os, re, sys, time, urllib.error, urllib.request

REPO = "/home/magi/Projects/wagicGPT"
DAT = os.path.join(REPO, "projects/mtg/bin/Res/sets")
UA = "WagicVitaPort/1.0 (card image downloader; github.com/WagicProject/wagic)"
URL = "https://api.scryfall.com/cards/multiverse/{}?format=image&version={}"
RATE = 0.25  # 4 req/s, well inside Scryfall's 10 req/s ceiling


def back_faces(sets_dir, only=None):
    """(set, back_id, name, front_id) for every positive-id rarity=T entry.

    Positive id + rarity=T is the DFC back-face marker: real tokens carry a
    NEGATIVE id in _cards.dat (their art file is "<id>t.jpg")."""
    out = []
    for s in sorted(os.listdir(sets_dir)):
        if only and s not in only:
            continue
        p = os.path.join(sets_dir, s, "_cards.dat")
        if not os.path.isfile(p):
            continue
        txt = open(p, encoding="utf-8", errors="replace").read()
        prev = None
        for blk in txt.split("[card]")[1:]:
            m = re.search(r"^id=(-?\d+)", blk, re.M)
            if not m:
                continue
            cid = int(m.group(1))
            r = re.search(r"^rarity=(\w)", blk, re.M)
            n = re.search(r"^primitive=(.*)$", blk, re.M)
            # Kamigawa flip cards (doubleFaced=kamiflip) also carry a positive
            # rarity=T entry, but with a SYNTHETIC id (front*100+11, e.g. BOK
            # 74536 -> 7453611).  They are one physical card whose "back" is the
            # same picture rotated, so a front-duplicate image is CORRECT there
            # and the id would 404 on Scryfall.  Real Gatherer multiverse ids
            # are well under a million; that is the cut.
            if r and r.group(1) == "T" and 0 < cid < 1000000:
                out.append((s, cid, n.group(1).strip() if n else "", prev))
            prev = cid
    return out


def md5(path):
    return hashlib.md5(open(path, "rb").read()).hexdigest()


def fetch(mid, version, retries=4):
    url = URL.format(mid, version)
    for attempt in range(retries):
        time.sleep(RATE)
        try:
            req = urllib.request.Request(url, headers={"User-Agent": UA,
                                                       "Accept": "image/jpeg,image/*"})
            with urllib.request.urlopen(req, timeout=30) as resp:
                data = resp.read()
            return data if len(data) > 1000 else None
        except urllib.error.HTTPError as e:
            if e.code == 404:
                return None
            time.sleep(2 ** attempt)
        except Exception:
            time.sleep(2 ** attempt)
    return None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--src", default=os.path.join(REPO, "projects/mtg/bin/User/sets/sets.hidden"),
                    help="desktop full-resolution art tree (<SET>/<id>.jpg + thumbnails/)")
    ap.add_argument("--size", default="large", choices=["small", "normal", "large"])
    ap.add_argument("--thumb-geom", default="114x166", help="desktop thumbnail geometry (exact)")
    ap.add_argument("--sets", default=None, help="comma-separated set codes")
    ap.add_argument("--dry-run", action="store_true")
    a = ap.parse_args()

    only = set(x.strip().upper() for x in a.sets.split(",")) if a.sets else None
    faces = back_faces(DAT, only)
    print("back faces in _cards.dat: %d" % len(faces))

    todo, ok = [], 0
    for s, cid, name, front in faces:
        f = os.path.join(a.src, s, "%d.jpg" % cid)
        if not os.path.exists(f):
            todo.append((s, cid, name, "missing"))
            continue
        pf = os.path.join(a.src, s, "%d.jpg" % front) if front else None
        if pf and os.path.exists(pf) and md5(f) == md5(pf):
            todo.append((s, cid, name, "front-duplicate"))
        else:
            ok += 1
    print("already correct: %d | to repair: %d" % (ok, len(todo)))
    for s, cid, name, why in todo[:10]:
        print("  %-6s %-8d %-38s %s" % (s, cid, name[:38], why))
    if a.dry_run or not todo:
        return 0

    fixed = failed = 0
    for i, (s, cid, name, why) in enumerate(todo, 1):
        data = fetch(cid, a.size)
        if not data:
            failed += 1
            print("  MISS %s/%d %s (%s)" % (s, cid, name, why))
            continue
        d = os.path.join(a.src, s)
        os.makedirs(os.path.join(d, "thumbnails"), exist_ok=True)
        full = os.path.join(d, "%d.jpg" % cid)
        open(full, "wb").write(data)
        os.system('magick "%s" -strip -resize "%s!" -quality 85 "%s" 2>/dev/null'
                  % (full, a.thumb_geom, os.path.join(d, "thumbnails", "%d.jpg" % cid)))
        fixed += 1
        if i % 25 == 0:
            print("  ... %d/%d" % (i, len(todo)))
    print("repaired: %d  failed: %d" % (fixed, failed))
    return 0 if not failed else 1


if __name__ == "__main__":
    sys.exit(main())
