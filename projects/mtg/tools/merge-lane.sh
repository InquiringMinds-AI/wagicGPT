#!/bin/bash
# usage: merge-lane.sh <branch>  — merge, resolve known patterns, build, PARSETEST-gate (' 0 failed'), commit.
set -e; B="$1"; cd /home/magi/Projects/wagicGPT
if git merge --no-ff --no-edit "$B" >/dev/null 2>&1; then echo "$B: clean merge"; else python3 ~/.gatelogs/resolve-merge.py "$B"; fi
cd projects/mtg && rm -f bin/wagic && make -f Makefile.sdl -j8 > ~/.gatelogs/merge-$B-make.log 2>&1 && test -x bin/wagic
cd bin && r=$(systemd-run --user --scope -q -p MemoryMax=4G -p MemorySwapMax=0 -- env -u WAYLAND_DISPLAY -u DISPLAY WAGIC_GPT_PARSETEST=1 ./wagic 2>&1 | /usr/bin/grep 'self-test:'); echo "$B: $r"; echo "$r" | /usr/bin/grep -q ' 0 failed'
cd /home/magi/Projects/wagicGPT; if ! git diff --cached --quiet || [ -f .git/MERGE_HEAD ]; then git commit -q --no-edit; fi
echo "$B: $(git log --oneline -1) fffd=$(git show HEAD --format= | /usr/bin/grep -c $'\357\277\275' || true)"
