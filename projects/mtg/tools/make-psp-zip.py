#!/usr/bin/env python3
"""Build the PSP end-user zip from a pspdev cross-build in the psp worktree.

Layout inside the zip ("extract to the memory stick root"):
    PSP/GAME/WAGIC/
        EBOOT.PBP           clean build (caller must verify 0 probe strings)
        exception.prx       crash handler, built by the same docker run
        Res/                core pack via make-respack.py (card-art refusal
                            inherited) + the four text-offload sidecar files
                            (cardtext/cardauto .idx/.dat) generated against
                            THIS pack's primitives - sidecar and primitives
                            are a matched pair, never mix builds.

Usage: tools/make-psp-zip.py -o /tmp/wagicGPT-psp.zip \
           [--worktree ~/Projects/wagicGPT-psp]
"""

import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile
import zipfile

def repo_paths():
    tools = os.path.dirname(os.path.abspath(__file__))
    mtg = os.path.dirname(tools)
    return mtg, os.path.join(mtg, "include", "Wagic_Version.h")


def read_version(header):
    with open(header) as f:
        m = re.search(r'WAGICGPT_VERSION_STRING\s+"([^"]+)"', f.read())
    return m.group(1) if m else "unknown"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("-o", "--output", required=True)
    ap.add_argument("--worktree",
                    default=os.path.expanduser("~/Projects/wagicGPT-psp"))
    args = ap.parse_args()

    mtg, header = repo_paths()
    version = read_version(header)
    wt = os.path.join(args.worktree, "projects", "mtg")
    eboot = os.path.join(wt, "EBOOT.PBP")
    prx = os.path.join(args.worktree, "JGE", "exceptionHandler", "prx", "exception.prx")
    gen = os.path.expanduser("~/Projects/wagicGPT/psp-work/gen-cardtext-sidecar.py")
    for p in (eboot, prx, gen):
        if not os.path.exists(p):
            sys.exit("missing: " + p)

    with tempfile.TemporaryDirectory() as tmp:
        game = os.path.join(tmp, "PSP", "GAME", "WAGIC")
        res = os.path.join(game, "Res")
        os.makedirs(res)
        shutil.copy(eboot, os.path.join(game, "EBOOT.PBP"))
        shutil.copy(prx, os.path.join(game, "exception.prx"))

        core = os.path.join(tmp, "core.zip")
        # the pack must come from the WORKTREE's Res (psp-port branch) so the
        # sidecars, primitives and EBOOT are one matched build
        subprocess.run([sys.executable,
                        os.path.join(args.worktree, "projects", "mtg", "tools", "make-respack.py"),
                        "-o", core], check=True)
        with zipfile.ZipFile(core) as z:
            z.extractall(res)

        subprocess.run([sys.executable, gen,
                        os.path.join(res, "sets", "primitives"), res], check=True)
        for f in ("cardtext.idx", "cardtext.dat", "cardauto.idx", "cardauto.dat"):
            if not os.path.exists(os.path.join(res, f)):
                sys.exit("sidecar missing: " + f)

        out = os.path.abspath(args.output)
        if os.path.exists(out):
            os.remove(out)
        with zipfile.ZipFile(out, "w", zipfile.ZIP_DEFLATED) as z:
            for root, _, files in os.walk(os.path.join(tmp, "PSP")):
                for f in sorted(files):
                    p = os.path.join(root, f)
                    z.write(p, os.path.relpath(p, tmp))
        print("wrote %s (version %s)" % (out, version))


if __name__ == "__main__":
    main()
