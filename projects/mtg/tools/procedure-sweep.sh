#!/bin/bash
# procedure-sweep: when a procedure CHANGES, the procedural memory may not keep the old one anywhere.
# Usage: procedure-sweep.sh '<regex for the OLD procedure>'  -> prints every hit across the whole procedural store.
# The session that changes a procedure runs this in the SAME turn and clears every hit (rewrite or delete) before it ends.
# Store = the wagicgpt skill + its sub-memories + the project anchor + the harness/tools + wave briefs.
pat="$1"; [ -z "$pat" ] && { echo "usage: $0 '<regex>'"; exit 2; }
files=( /home/magi/.claude/skills/wagicgpt/SKILL.md
        /home/magi/.claude/skills/wagic-cardscript/SKILL.md
        /home/magi/.claude/projects/-home-magi/memory/project_wagicgpt.md
        /home/magi/.claude/projects/-home-magi/memory/wagicgpt/*.md
        /home/magi/.claude/projects/-home-magi/memory/feedback_wagicgpt_*.md
        /home/magi/Projects/wagicGPT/projects/mtg/tools/*.sh
        /home/magi/Projects/wagicGPT/projects/mtg/strategy-design/wave58/loop-as-envisioned.md )
n=0
for f in "${files[@]}"; do [ -f "$f" ] || continue; while IFS= read -r line; do echo "$f:$line"; n=$((n+1)); done < <(/usr/bin/grep -n -i -E -- "$pat" "$f" | cut -c1-200); done
echo "== $n hit(s) for /$pat/ — every one must be rewritten or deleted before the turn ends (0 = clean)"
[ "$n" = 0 ]
