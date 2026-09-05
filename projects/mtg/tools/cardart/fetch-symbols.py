#!/usr/bin/env python3
"""Fetch set symbols into the card-art pool as User/sets/<SET>/symbol.png.

Sources, in order (owner ruling 2026-09-05: symbols yes, no Forge pack art):
  1. Scryfall  - one index request (api.scryfall.com/sets) gives every set's
     icon_svg_uri (svgs.scryfall.io/sets/<code>.svg, ~800 B, monochrome).
     Served under the WotC Fan Content Policy, like the card scans.
  2. Keyrune   - Andrew Gioia's hand-redrawn glyphs (GPL-3.0 / SIL OFL), via
     raw.githubusercontent.com/andrewgioia/keyrune/master/svg/<code>.svg.
     The code alias comes from MTGJSON SetList.json's `keyruneCode`.
Both are SVG; the handhelds have no SVG renderer, so this script rasterizes
with rsvg-convert (or ImageMagick) to a 64 px-high PNG. The engine draws
sets/<SET>/symbol.png on the shop's procedural booster pack when present.

Pipeline position: fills the POOL next to the card jpgs; pack-common.sh copies
symbol.png into each platform set zip.

Network manners: custom User-Agent, >=120 ms between requests (Scryfall asks
for 50-100 ms). The MTGJSON list (~12 MB) is fetched once and cached.
Misses are written to symbols-misses.txt next to this script.
"""
import argparse, json, os, shutil, subprocess, sys, time, urllib.request

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
CACHE_DIR = os.path.join(REPO, "projects/mtg/bin/User")
UA = "wagicGPT-cardart/0.1 (+https://github.com/InquiringMinds-AI/wagicGPT)"
THROTTLE = 0.12
SCRYFALL_SETS = "https://api.scryfall.com/sets"
KEYRUNE_SVG = "https://raw.githubusercontent.com/andrewgioia/keyrune/master/svg/%s.svg"
MTGJSON_SETLIST = "https://mtgjson.com/api/v5/SetList.json"

_last = [0.0]
SCRY_BY_NAME = {}
def http_get(url, timeout=60):
    wait = THROTTLE - (time.monotonic() - _last[0])
    if wait > 0:
        time.sleep(wait)
    req = urllib.request.Request(url, headers={"User-Agent": UA, "Accept": "*/*"})
    try:
        with urllib.request.urlopen(req, timeout=timeout) as r:
            data = r.read()
    finally:
        _last[0] = time.monotonic()
    return data

def cached_json(url, path, force):
    if os.path.exists(path) and not force:
        with open(path, "rb") as f:
            return json.load(f)
    data = http_get(url, timeout=300)
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "wb") as f:
        f.write(data)
    return json.loads(data)

def set_meta_name(res_sets, code):
    """The set's display name from Res/sets/<code>/_cards.dat [meta] name=."""
    path = os.path.join(res_sets, code, "_cards.dat")
    try:
        with open(path, "rb") as f:
            in_meta = False
            for raw in f:
                line = raw.decode("latin-1").strip()
                if line.startswith("["):
                    in_meta = (line.lower() == "[meta]")
                elif in_meta and line.lower().startswith("name="):
                    return line[5:].strip()
    except OSError:
        pass
    return ""

def norm_name(n):
    return " ".join(n.lower().replace(".", "").replace(":", "").split())

def scryfall_index(force):
    """code (upper) -> icon_svg_uri, following has_more pagination.
    Also fills SCRY_BY_NAME (normalized set name -> icon) for Wagic's legacy
    codes (RV, MRQ, CFX, the xVy duel decks...) that no index knows."""
    out = {}
    url = SCRYFALL_SETS
    page = 0
    while url:
        path = os.path.join(CACHE_DIR, "_scryfall_sets_%d.json" % page)
        j = cached_json(url, path, force)
        for s in j.get("data", []):
            if s.get("code") and s.get("icon_svg_uri"):
                out[s["code"].upper()] = s["icon_svg_uri"]
                if s.get("name"):
                    SCRY_BY_NAME.setdefault(norm_name(s["name"]), s["icon_svg_uri"])
        url = j.get("next_page") if j.get("has_more") else None
        page += 1
    return out

def keyrune_aliases(force):
    """code (upper) -> keyruneCode (lower)."""
    path = os.path.join(CACHE_DIR, "_mtgjson_setlist.json")
    j = cached_json(MTGJSON_SETLIST, path, force)
    out = {}
    for s in j.get("data", []):
        if s.get("code") and s.get("keyruneCode"):
            out[s["code"].upper()] = s["keyruneCode"].lower()
    return out

def rasterize(svg_path, png_path, height, fill):
    """Glyph rasterized in one flat colour (default white) on transparency, so the
    engine can tint it at render time (JQuad::SetColor multiplies)."""
    if shutil.which("rsvg-convert"):
        css = svg_path + ".css"
        with open(css, "w") as f:
            f.write("svg, path, polygon, circle, rect { fill: %s !important; stroke: none; }\n" % fill)
        cmd = ["rsvg-convert", "--stylesheet", css, "-h", str(height), "-o", png_path, svg_path]
    elif shutil.which("magick"):
        css = None
        cmd = ["magick", "-background", "none", "-density", "300", svg_path, "-resize", "x%d" % height,
               "-fill", fill, "-colorize", "100%", png_path]
    else:
        sys.exit("need rsvg-convert or ImageMagick (magick) to rasterize SVG")
    try:
        subprocess.run(cmd, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    finally:
        if css and os.path.exists(css):
            os.remove(css)

def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--res-sets", default=DEF_RES_SETS, help="Res/sets dir (set codes = subdirs)")
    ap.add_argument("--pool", default=DEF_POOL_BASE, help="User/sets pool dir (output)")
    ap.add_argument("--height", type=int, default=64, help="PNG height in px (default 64)")
    ap.add_argument("--fill", default="#ffffff", help="glyph colour (default white; the engine tints at render time)")
    ap.add_argument("--sets", default="", help="comma-separated whitelist of set codes")
    ap.add_argument("--no-keyrune", action="store_true", help="Scryfall only")
    ap.add_argument("--force", action="store_true", help="re-fetch indexes and overwrite existing symbol.png")
    ap.add_argument("--dry-run", action="store_true", help="resolve only; no downloads or writes")
    a = ap.parse_args()

    codes = sorted(d for d in os.listdir(a.res_sets) if os.path.isdir(os.path.join(a.res_sets, d)) and not d.startswith("."))
    if a.sets:
        want = set(c.strip().upper() for c in a.sets.split(","))
        codes = [c for c in codes if c.upper() in want]
    print("sets: %d" % len(codes))
    scry = scryfall_index(a.force)
    print("scryfall index: %d sets" % len(scry))
    aliases = {} if a.no_keyrune else keyrune_aliases(a.force)
    if aliases:
        print("keyrune aliases: %d sets" % len(aliases))

    got = {"scryfall": 0, "keyrune": 0, "kept": 0}
    misses = []
    for code in codes:
        out_dir = os.path.join(a.pool, code)
        png = os.path.join(out_dir, "symbol.png")
        if os.path.exists(png) and not a.force:
            got["kept"] += 1
            continue
        up = code.upper()
        url = scry.get(up)
        src = "scryfall"
        if not url:
            nm = norm_name(set_meta_name(a.res_sets, code))
            if nm and SCRY_BY_NAME.get(nm):
                url = SCRY_BY_NAME[nm]; src = "scryfall"
        if not url and aliases.get(up):
            url = KEYRUNE_SVG % aliases[up]
            src = "keyrune"
        if not url and not a.no_keyrune:
            url = KEYRUNE_SVG % code.lower()  # last try: the code itself is a keyrune name
            src = "keyrune"
        if not url:
            misses.append("%s\tno source" % code); continue
        if a.dry_run:
            print("%s <- %s (%s)" % (code, url, src)); continue
        try:
            svg = http_get(url)
        except Exception as e:
            #Scryfall lists the set but serves no icon (promo sets): try Keyrune
            #by alias, then by the code itself.
            svg = None
            if src == "scryfall" and not a.no_keyrune:
                for kc in [aliases.get(up), code.lower()]:
                    if not kc: continue
                    try:
                        svg = http_get(KEYRUNE_SVG % kc); src = "keyrune"; break
                    except Exception:
                        pass
            if svg is None:
                misses.append("%s\t%s" % (code, e)); continue
        if b"<svg" not in svg[:2000]:
            misses.append("%s\tnot an svg (%s)" % (code, src)); continue
        os.makedirs(out_dir, exist_ok=True)
        svg_path = os.path.join(out_dir, "symbol.svg")
        with open(svg_path, "wb") as f:
            f.write(svg)
        try:
            rasterize(svg_path, png, a.height, a.fill)
        except subprocess.CalledProcessError:
            misses.append("%s\trasterize failed (%s)" % (code, src)); os.remove(svg_path); continue
        os.remove(svg_path)
        got[src] += 1
        print("%s ok (%s)" % (code, src))
    with open(os.path.join(HERE, "symbols-misses.txt"), "w") as f:
        f.write("\n".join(misses) + ("\n" if misses else ""))
    print("done: scryfall=%d keyrune=%d kept=%d misses=%d" % (got["scryfall"], got["keyrune"], got["kept"], len(misses)))

if __name__ == "__main__":
    main()
