#!/usr/bin/env python3
"""Build the Linux end-user tarball from a release build (Makefile.sdl-rel).

Layout (one top-level folder, untar-and-run):
    wagicGPT-<version>/
        wagic               stripped release binary (bin-rel/wagic)
        wagic.sh            launcher: LD_LIBRARY_PATH=lib, cd to its own dir
        lib/                bundled shared libs, ldd-resolved. glibc, the
                            dynamic loader, and all GPU/display-stack libs
                            (GL/GLX/EGL/X11/xcb/wayland/drm) stay SYSTEM -
                            bundling those breaks other machines.
        Res/                core resource pack via make-respack.py - its
                            card-art refusal gates this artifact too.

No User/ is shipped: first boot shows the language picker and the game
creates its profile next to the binary.

Usage: tools/make-linux-tarball.py -o /tmp/wagicGPT-linux.tar.gz
"""

import argparse
import os
import re
import shutil
import subprocess
import sys
import tarfile
import tempfile
import zipfile

SYSTEM_LIBS = re.compile(
    r"^(libc\.|libm\.|libpthread|libdl|librt|ld-linux|libresolv|"
    r"libGL\.|libGLdispatch|libGLX|libEGL|libGLU|libX|libxcb|libwayland|"
    r"libdrm|libgbm)")


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
    args = ap.parse_args()

    mtg, header = repo_paths()
    version = read_version(header)
    binary = os.path.join(mtg, "bin-rel", "wagic")
    if not os.path.exists(binary):
        sys.exit("no bin-rel/wagic - build with: make -f Makefile.sdl-rel -j8")

    top = "wagicGPT-" + version
    with tempfile.TemporaryDirectory() as tmp:
        stage = os.path.join(tmp, top)
        os.makedirs(os.path.join(stage, "lib"))

        subprocess.run(["strip", "-o", os.path.join(stage, "wagic"), binary], check=True)
        os.chmod(os.path.join(stage, "wagic"), 0o755)

        out_ldd = subprocess.run(["ldd", binary], capture_output=True, text=True).stdout
        bundled = 0
        for line in out_ldd.splitlines():
            m = re.match(r"\s*(\S+)\s+=>\s+(\S+)", line)
            if not m:
                continue
            name, path = m.group(1), m.group(2)
            base = os.path.basename(name)
            if SYSTEM_LIBS.match(base) or not os.path.exists(path):
                continue
            shutil.copy(path, os.path.join(stage, "lib", base))
            bundled += 1

        launcher = os.path.join(stage, "wagic.sh")
        with open(launcher, "w") as f:
            f.write('#!/bin/sh\ncd "$(dirname "$0")"\n'
                    'LD_LIBRARY_PATH="$PWD/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" exec ./wagic "$@"\n')
        os.chmod(launcher, 0o755)

        core = os.path.join(tmp, "core.zip")
        subprocess.run([sys.executable, os.path.join(mtg, "tools", "make-respack.py"),
                        "-o", core], check=True)
        with zipfile.ZipFile(core) as z:
            z.extractall(os.path.join(stage, "Res"))

        out = os.path.abspath(args.output)
        if os.path.exists(out):
            os.remove(out)
        with tarfile.open(out, "w:gz") as t:
            t.add(stage, arcname=top)
        print("wrote %s (%d libs, version %s)" % (out, bundled, version))


if __name__ == "__main__":
    main()
