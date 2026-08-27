#!/usr/bin/env bash
# Upload the built VPK to a Vita over VitaShell's FTP.
#
# VitaShell's FTP server is NOT always on - press SELECT in VitaShell to start
# it, and read the IP:port it prints. Default port is 1337.
#
# This script ONLY uploads a file. It does not install, and it does not touch
# taiHEN, StorageMgr, or any mount configuration - a past session destroyed this
# user's Vita setup that way and required a full reformat and re-hack.
# Install by opening the VPK in VitaShell and pressing Cross.
#
# Usage:
#   ./deploy-vita.sh                 # default host, PSTV
#   ./deploy-vita.sh 10.0.0.123      # handheld, or a reassigned DHCP address
set -euo pipefail

HOST="${1:-10.0.0.95}"
PORT="${2:-1337}"
VPK="${VPK:-/home/magi/Projects/wagicGPT/worktrees/psp/build_vita/wagic.vpk}"
# ux0:/vpk is where VitaShell users expect to find installable packages; an
# earlier version dropped it in ux0:/data and it looked like the upload had
# failed. The directory is created if missing.
DEST="ux0:/vpk/wagic.vpk"

[ -f "$VPK" ] || { echo "no VPK at $VPK - build it first (see NOTES.md)"; exit 1; }

echo "checking ftp://$HOST:$PORT ..."
if ! curl -s --connect-timeout 8 "ftp://$HOST:$PORT/ux0:/" -o /dev/null; then
    echo "FTP not answering."
    echo "  - is the Vita awake and on wifi?"
    echo "  - is VitaShell open with FTP started (SELECT)?"
    echo "  - does the IP VitaShell shows match $HOST?"
    exit 1
fi

SIZE=$(stat -c%s "$VPK")
echo "uploading $(numfmt --to=iec "$SIZE") to $HOST:$DEST"
# Vita FTP is single-threaded: one transfer at a time, no parallel uploads.
curl --connect-timeout 10 --ftp-create-dirs -T "$VPK" "ftp://$HOST:$PORT/$DEST"

echo
echo "uploaded. On the Vita: VitaShell -> $DEST -> Cross to install."
echo "Then launch from the LiveArea."
