#!/usr/bin/env python3
"""Generate cardtext.idx/.dat sidecar from primitives files (PSP text= offload).

idx: u32 count, then count * {u32 fnv1a(lower name), u32 offset, u32 len}, hash-sorted, LE.
dat: entry at offset = lowercased name + b'\n' + text bytes; len covers whole entry.
Process files in the engine's FAT load order, later files override earlier names.
Byte-level throughout (primitives contain non-UTF8 accent bytes; ASCII-only lower()).
"""
import re, os, struct, sys

src = sys.argv[1] if len(sys.argv) > 1 else os.path.expanduser(
    "~/Projects/wagicGPT/psp-work/snapshots/WTH-fork/Res/sets/primitives")
out = sys.argv[2] if len(sys.argv) > 2 else os.path.expanduser(
    "~/Projects/wagicGPT/psp-work/sidecar")
os.makedirs(out, exist_ok=True)

ORDER = ["mtg.txt", "borderline.txt", "planeswalkers.txt", "unsupported.txt"]

def fnv1a(b: bytes) -> int:
    h = 2166136261
    for c in b:
        h = ((h ^ c) * 16777619) & 0xFFFFFFFF
    return h

texts = {}  # lower name bytes -> text bytes (cardtext pair)
autos = {}  # lower name bytes -> raw auto*/anyzone lines (cardauto pair)
blockpat = re.compile(rb"\[card\](.*?)\[/card\]", re.S)
autopat = re.compile(rb"^((?:auto[^=\n]*|anyzone)=.*)$", re.M)
for fn in ORDER:
    p = os.path.join(src, fn)
    if not os.path.exists(p):
        print(f"skip missing {fn}"); continue
    data = open(p, "rb").read()
    nt = na = 0
    for m in blockpat.finditer(data):
        block = m.group(1)
        nm = re.search(rb"^name=(.*)$", block, re.M)
        if not nm: continue
        key = nm.group(1).strip().lower()
        tlines = re.findall(rb"^text=(.*)$", block, re.M)
        if tlines:
            texts[key] = b"\n".join(t.strip() for t in tlines); nt += 1
        alines = [a.strip() for a in autopat.findall(block)]
        if alines:
            autos[key] = b"\n".join(alines); na += 1
    print(f"{fn}: {nt} cards with text, {na} with auto lines")

def emit(mapping, base):
    entries = []
    datpath, idxpath = os.path.join(out, base + ".dat"), os.path.join(out, base + ".idx")
    with open(datpath, "wb") as dat:
        for name, payload_body in mapping.items():
            off = dat.tell()
            payload = name + b"\n" + payload_body
            dat.write(payload)
            entries.append((fnv1a(name), off, len(payload)))
    entries.sort(key=lambda e: e[0])
    with open(idxpath, "wb") as idx:
        idx.write(struct.pack("<I", len(entries)))
        for e in entries:
            idx.write(struct.pack("<III", *e))
    dup = len(entries) - len({e[0] for e in entries})
    print(f"{base}: {len(entries)} entries, dat={os.path.getsize(datpath)}, "
          f"idx={os.path.getsize(idxpath)}, hash collisions: {dup}")

emit(texts, "cardtext")
emit(autos, "cardauto")
