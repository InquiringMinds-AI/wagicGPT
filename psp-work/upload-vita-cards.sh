#!/usr/bin/env bash
# Upload the Vita card pack to the console over VitaShell's FTP server.
#
# UPLOAD ONLY, like deploy-vita.sh.  This script writes exclusively under
# ux0:data/Wagic/sets/.  It never touches shells, StorageMgr, taiHEN, config
# or anything else on the device -- a previous session destroyed his Vita
# setup that way and it cost a full reformat + rehack.
#
# RESUMABLE BY DESIGN.  VitaShell's FTP server dies when the console sleeps,
# and it fails DISHONESTLY: directory listings keep returning empty with exit
# status 0, so only a real transfer surfaces the failure.  Every set is
# therefore verified by remote SIZE after upload, and a re-run skips any set
# whose remote size already matches local.  A death costs one set, not the run.
#
# Usage:  ./psp-work/upload-vita-cards.sh [HOST] [PORT]
set -uo pipefail

HOST="${1:-10.0.0.227}"
PORT="${2:-1337}"
PACK="${PACK:-/home/magi/Projects/wagicGPT/psp-work/card-packs/vita/sets}"
REMOTE_BASE="${REMOTE_BASE:-ux0:/data/Wagic/sets}"
JOBS="${JOBS:-2}"          # parallel streams; 1 if the server gets unhappy
RETRIES="${RETRIES:-3}"
LOG="${LOG:-/home/magi/Projects/wagicGPT/psp-work/card-packs/upload.log}"
STATE="${STATE:-/home/magi/Projects/wagicGPT/psp-work/card-packs/.uploaded}"

base_url="ftp://${HOST}:${PORT}/${REMOTE_BASE}"
mkdir -p "$(dirname "$LOG")"; touch "$STATE"

# Remote size, or empty when absent/unreachable.
remote_size() {
    curl -s -I --connect-timeout 10 --max-time 30 "$1" 2>/dev/null \
      | awk 'tolower($1) ~ /^content-length:/ {gsub(/\r/,"");print $2; exit}'
}

# Liveness probe that actually tells the truth: fetch a byte of a known file.
# An empty LIST is NOT proof the server is up.
alive() {
    curl -s --connect-timeout 8 --max-time 15 -r 0-0 \
         "ftp://${HOST}:${PORT}/ux0:/data/Wagic/" -o /dev/null 2>/dev/null
    # Directory fetch returns non-zero even when healthy on some builds, so
    # fall back to a bare control-connection check.
    curl -s --connect-timeout 8 --max-time 15 "ftp://${HOST}:${PORT}/" -o /dev/null
}

upload_set() {
    local zip="$1" set_name url sz rsz try
    set_name="$(basename "$(dirname "$zip")")"
    url="${base_url}/${set_name}/${set_name}.zip"
    sz="$(stat -c%s "$zip")"

    rsz="$(remote_size "$url")"
    if [ -n "$rsz" ] && [ "$rsz" = "$sz" ]; then
        echo "SKIP $set_name (already on device, $sz bytes)"
        grep -qx "$set_name" "$STATE" || echo "$set_name" >> "$STATE"
        return 0
    fi

    for try in $(seq 1 "$RETRIES"); do
        if curl -s --connect-timeout 15 --max-time 900 --ftp-create-dirs \
                -T "$zip" "$url" 2>/dev/null; then
            rsz="$(remote_size "$url")"
            if [ "$rsz" = "$sz" ]; then
                echo "OK   $set_name  $((sz/1024/1024)) MB"
                echo "$set_name" >> "$STATE"
                return 0
            fi
            echo "  size mismatch $set_name (local $sz, remote ${rsz:-none}) try $try"
        else
            echo "  transfer failed $set_name try $try"
        fi
        sleep 5
    done
    echo "FAIL $set_name"
    return 1
}
export -f upload_set remote_size
export base_url STATE RETRIES

echo "=== vita card upload -> ${HOST}:${PORT}  $(date -Is) ===" | tee -a "$LOG"
if ! alive; then
    echo "FTP server not answering at ${HOST}:${PORT}." | tee -a "$LOG"
    echo "Start it in VitaShell with SELECT, then re-run." | tee -a "$LOG"
    exit 1
fi

total=$(find "$PACK" -name '*.zip' | wc -l)
echo "sets to consider: $total  (already uploaded: $(wc -l < "$STATE"))" | tee -a "$LOG"

find "$PACK" -name '*.zip' | sort \
  | xargs -P "$JOBS" -I{} bash -c 'upload_set "$@"' _ {} 2>&1 | tee -a "$LOG"

done_n=$(sort -u "$STATE" | wc -l)
echo "=== finished $(date -Is): $done_n / $total sets on device ===" | tee -a "$LOG"
if [ "$done_n" -lt "$total" ]; then
    echo "INCOMPLETE - re-run this script; it resumes where it stopped." | tee -a "$LOG"
    exit 1
fi
touch /home/magi/Projects/wagicGPT/psp-work/card-packs/.upload-done
