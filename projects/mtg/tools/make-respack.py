#!/usr/bin/env python3
"""Build a Wagic core resource pack from the Res tree.

Replaces Res/createResourceZip.py, which is Python 2, depends on an ant build
number file this fork does not use, and renames files inside the working tree
while it runs.

The pack is the data half of the game: sets/primitives/mtg.txt is the auto=
script corpus the engine interprets, so a pack and the engine that reads it are
a matched pair. This fork ships one pack per release, bundled into the release
artifact - there is no pack update channel, so the pack a user runs is always
the one their build shipped with.

CARD ART IS NEVER PACKAGED. Card scans are derived from official MTG products
and must not appear in the public repo or any release artifact. That rule is
enforced here as a build-breaking check rather than left to convention - see
check_no_card_art(). The generic colour frames at sets/*.jpg (back, blue,
morph, ...) are the engine's no-art fallbacks, not scans, and are packaged.

Usage, from anywhere:
    tools/make-respack.py -o /tmp/Wagic-core-0255.zip
    tools/make-respack.py -o out.zip --extra /path/to/cardtext.idx
"""

import argparse
import hashlib
import os
import re
import subprocess
import sys
import zipfile

# Folders that make up a pack, in upstream's order. Deliberately excludes
# test/ (suite fixtures), oldthemes/ and missing_cards_by_sets/ (dev material).
PACK_FOLDERS = [
    "themes", "sound", "settings", "sets", "rules",
    "player", "packs", "lang", "graphics", "campaigns", "ai",
]

# Extensions that constitute card art. Checked only inside set subdirectories,
# where per-card art and <SET>.zip image packs land.
ART_EXTENSIONS = {".jpg", ".jpeg", ".png", ".gif", ".bmp", ".zip"}

# Shipped as options.txt but stored as options.orig.txt so a developer's local
# settings never become the packaged defaults.
ORIG_DEFAULTS = ["settings/options.txt", "player/options.txt"]

# Res-root files that belong in a pack. createResourceZip.py and the PSP
# sidecars are deliberately not here (the latter go in via --extra).
ROOT_FILES = ["LICENSE"]


def repo_paths():
    """Locate the Res tree and the version header relative to this script."""
    tools = os.path.dirname(os.path.abspath(__file__))
    mtg = os.path.dirname(tools)
    return os.path.join(mtg, "bin", "Res"), os.path.join(mtg, "include", "Wagic_Version.h")


def read_version(header):
    """Parse WAGIC_VERSION_{MAJOR,MEDIUM,MINOR} out of Wagic_Version.h."""
    text = open(header, encoding="utf-8", errors="replace").read()
    parts = {}
    for key in ("MAJOR", "MEDIUM", "MINOR"):
        m = re.search(r"#define\s+WAGIC_VERSION_%s\s+(\d+)" % key, text)
        if not m:
            sys.exit("could not parse WAGIC_VERSION_%s from %s" % (key, header))
        parts[key] = m.group(1)
    dotted = "%s.%s.%s" % (parts["MAJOR"], parts["MEDIUM"], parts["MINOR"])
    joined = "%s%s%s" % (parts["MAJOR"], parts["MEDIUM"], parts["MINOR"])
    return dotted, joined


def check_no_card_art(res):
    """Abort if card art or an image pack is staged inside any set directory.

    Art lives at sets/<SET>/, so this walks below sets/ only. The loose frames
    at sets/*.jpg are one level up and are intentionally untouched.
    """
    sets_dir = os.path.join(res, "sets")
    offenders = []
    for root, _dirs, files in os.walk(sets_dir):
        if os.path.normpath(root) == os.path.normpath(sets_dir):
            continue  # loose colour frames live here; they are not card art
        for name in files:
            if os.path.splitext(name)[1].lower() in ART_EXTENSIONS:
                offenders.append(os.path.relpath(os.path.join(root, name), res))
    if offenders:
        sys.stderr.write(
            "REFUSING TO BUILD: card art found under sets/.\n"
            "Card images are derived from official MTG products and must not be\n"
            "packaged or committed. Move them out of the Res tree and rebuild.\n\n")
        for path in sorted(offenders)[:40]:
            sys.stderr.write("    %s\n" % path)
        if len(offenders) > 40:
            sys.stderr.write("    ... and %d more\n" % (len(offenders) - 40))
        sys.exit(2)


def git_commit(res):
    try:
        out = subprocess.run(["git", "-C", res, "rev-parse", "--short", "HEAD"],
                             capture_output=True, text=True, timeout=10)
        return out.stdout.strip() if out.returncode == 0 else "unknown"
    except Exception:
        return "unknown"


def add_folder(zf, res, folder, entries):
    """Add a folder recursively with Res-root-relative entry names.

    EVERY directory gets an explicit entry, including nested ones. This is not
    cosmetic: JGE's zip filesystem enumerates a folder by looking the folder's
    own name up in its file map and then keeping only entries whose remainder
    has no interior slash (JGE/src/zipFS/zfsystem.cpp, filesystem::scanfolder).
    Without a "sets/10E/" entry, "sets/10E/_cards.dat" is invisible to a scan of
    "sets/" and the set does not exist as far as the game is concerned - the
    game boots fine and reports a collection of 0 cards.
    """
    base = os.path.join(res, folder)
    if not os.path.isdir(base):
        sys.exit("missing pack folder: %s" % base)
    zf.writestr(folder + "/", "")
    for root, dirs, files in os.walk(base):
        dirs.sort()
        for name in sorted(dirs):
            arc = os.path.relpath(os.path.join(root, name), res).replace(os.sep, "/")
            zf.writestr(arc + "/", "")
        for name in sorted(files):
            full = os.path.join(root, name)
            arc = os.path.relpath(full, res).replace(os.sep, "/")
            zf.write(full, arc)
            entries.append(arc)


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("-o", "--out", help="output zip path (default: ./Wagic-core-<ver>.zip)")
    ap.add_argument("--extra", action="append", default=[],
                    help="extra file to place at the pack root (repeatable), "
                         "e.g. the PSP card-text sidecars")
    ap.add_argument("--no-sha", action="store_true", help="skip the .sha256 sidecar")
    ap.add_argument("--compress", choices=["deflate", "stored"], default="deflate",
                    help="deflate (default) roughly halves the pack and is proven "
                         "in the engine's zipFS; stored trades size for no inflate "
                         "cost, for memory-constrained targets")
    ap.add_argument("--print-name", action="store_true",
                    help="print the pack filename for this engine version and exit, "
                         "so callers never re-parse Wagic_Version.h themselves")
    args = ap.parse_args()

    res, header = repo_paths()
    if not os.path.isdir(res):
        sys.exit("Res tree not found at %s" % res)
    dotted, joined = read_version(header)
    if args.print_name:
        print("Wagic-core-%s.zip" % joined)
        return
    out = args.out or os.path.join(os.getcwd(), "Wagic-core-%s.zip" % joined)

    # The guard runs before anything is written, so a tree with art in it
    # produces no artifact at all rather than one that must be remembered about.
    check_no_card_art(res)

    entries = []
    mode = zipfile.ZIP_STORED if args.compress == "stored" else zipfile.ZIP_DEFLATED
    with zipfile.ZipFile(out, "w", mode, allowZip64=True) as zf:
        for folder in PACK_FOLDERS:
            add_folder(zf, res, folder, entries)

        for name in ROOT_FILES:
            full = os.path.join(res, name)
            if os.path.isfile(full):
                zf.write(full, name)
                entries.append(name)

        # Ship the .orig defaults under their live names without touching the
        # working tree (upstream renamed them in place mid-build).
        for target in ORIG_DEFAULTS:
            if target in entries:
                continue
            orig = os.path.join(res, target.replace(".txt", ".orig.txt"))
            if os.path.isfile(orig):
                zf.write(orig, target)
                entries.append(target)

        for extra in args.extra:
            if not os.path.isfile(extra):
                sys.exit("--extra file not found: %s" % extra)
            arc = os.path.basename(extra)
            zf.write(extra, arc)
            entries.append(arc)

        zf.writestr("packinfo.txt", "\n".join([
            "pack=Wagic-core-%s" % joined,
            "engine=%s" % dotted,
            "commit=%s" % git_commit(res),
            "files=%d" % len(entries),
            "art=excluded",
            "",
        ]))

    size = os.path.getsize(out)
    digest = ""
    if not args.no_sha:
        h = hashlib.sha256()
        with open(out, "rb") as fh:
            for chunk in iter(lambda: fh.read(1 << 20), b""):
                h.update(chunk)
        digest = h.hexdigest()
        with open(out + ".sha256", "w") as fh:
            fh.write("%s  %s\n" % (digest, os.path.basename(out)))

    print("pack    : %s" % out)
    print("engine  : %s (%s)" % (dotted, git_commit(res)))
    print("files   : %d" % len(entries))
    print("size    : %.1f MB" % (size / 1048576.0))
    if digest:
        print("sha256  : %s" % digest)


if __name__ == "__main__":
    main()
