#!/usr/bin/env python3
"""Build the Windows end-user zip from a mingw cross-build.

Layout inside the zip (one top-level folder, unzip-and-run):
    wagicGPT-<version>/
        wagic.exe           stripped release exe from Makefile.win
        *.dll               runtime set, recursively resolved from the
                            mingw sysroot (objdump -p, DLL Name entries)
        Res/                the core resource pack, extracted - produced by
                            make-respack.py so its card-art refusal gates
                            this artifact too. CARD ART IS NEVER PACKAGED.

No User/ is shipped: first boot shows the language picker and the game
creates its profile next to the exe, which is the intended end-user flow.

Usage, from anywhere:
    tools/make-windows-zip.py -o /tmp/wagicGPT-windows.zip
"""

import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile
import zipfile

SYSROOT_BIN = "/usr/x86_64-w64-mingw32/bin"
OBJDUMP = "x86_64-w64-mingw32-objdump"
STRIP = "x86_64-w64-mingw32-strip"


def repo_paths():
    tools = os.path.dirname(os.path.abspath(__file__))
    mtg = os.path.dirname(tools)
    return mtg, os.path.join(mtg, "include", "Wagic_Version.h")


def read_version(header):
    with open(header) as f:
        m = re.search(r'WAGICGPT_VERSION_STRING\s+"([^"]+)"', f.read())
    return m.group(1) if m else "unknown"


def resolve_dlls(pe, found):
    out = subprocess.run([OBJDUMP, "-p", pe], capture_output=True, text=True).stdout
    for line in out.splitlines():
        if "DLL Name" not in line:
            continue
        name = line.split()[-1]
        if name in found:
            continue
        src = os.path.join(SYSROOT_BIN, name)
        if os.path.exists(src):  # system DLLs (KERNEL32 etc.) resolve on Windows itself
            found[name] = src
            resolve_dlls(src, found)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("-o", "--output", required=True)
    args = ap.parse_args()

    mtg, header = repo_paths()
    version = read_version(header)
    exe = os.path.join(mtg, "bin", "wagic.exe")
    if not os.path.exists(exe):
        sys.exit("no bin/wagic.exe - build with: make -f Makefile.win -j8")

    top = "wagicGPT-" + version
    with tempfile.TemporaryDirectory() as tmp:
        stage = os.path.join(tmp, top)
        os.makedirs(stage)

        subprocess.run([STRIP, "-o", os.path.join(stage, "wagic.exe"), exe], check=True)

        dlls = {}
        resolve_dlls(exe, dlls)
        for name, src in sorted(dlls.items()):
            shutil.copy(src, os.path.join(stage, name))

        core = os.path.join(tmp, "core.zip")
        subprocess.run([sys.executable, os.path.join(mtg, "tools", "make-respack.py"),
                        "-o", core], check=True)
        with zipfile.ZipFile(core) as z:
            z.extractall(os.path.join(stage, "Res"))

        out = os.path.abspath(args.output)
        if os.path.exists(out):
            os.remove(out)
        with zipfile.ZipFile(out, "w", zipfile.ZIP_DEFLATED) as z:
            for root, _, files in os.walk(stage):
                for f in sorted(files):
                    p = os.path.join(root, f)
                    z.write(p, os.path.relpath(p, tmp))
        print("wrote %s (%d DLLs, version %s)" % (out, len(dlls), version))


if __name__ == "__main__":
    main()
