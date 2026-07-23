# Wave-21 deployment manifest

Instruction sheet for the integrator. Source of truth for each seat's guide: `strategy-design/wave21/deckN/strategy.txt`.
Live target: `projects/mtg/bin/Res/ai/baka/deckN_strategy.txt`. Every claim below was verified against the deployed
file with `cmp` (frozen rows) or `diff` (deploy rows) — verified, not rubber-stamped. Do NOT deploy anything yourself
from the synthesis stage; this manifest is the integrator's checklist.

## Guide deployment table

| deck | action | verification | detail |
|------|--------|--------------|--------|
| 135 | **FROZEN** — no copy | `cmp` clean, byte-identical (13357 bytes) | frozen veteran; losses decompose to ENGINE-F1 fetch fizzle + draw, none guide-fixable |
| 35 | **FROZEN** — no copy | `cmp` clean, byte-identical (8293 bytes) | payment fix validated clean; 3 losses = race/draw-screw, none guide-fixable |
| 27 | **FROZEN** — no copy | `cmp` clean, byte-identical (6959 bytes) | wave-20 revision already live; late-stall off-case still UNEXERCISED (do NOT touch) |
| 62 | **FROZEN** — no copy | `cmp` clean, byte-identical (8893 bytes) | edit D validated (round 5); open items all HARNESS/ENGINE lane |
| 49 | **FROZEN** — no copy | `cmp` clean, byte-identical (6319 bytes, md5 `479f90…`) | wave-20 two-hunk revision validated; guide converged, byte-copy |
| 14 | **DEPLOY** | `diff` = one bullet DEMOTED, 7723 → 7106 bytes (−617) | bounce-on-stack `#1 MISFIRE` 10-line (1)/(2) procedure compressed to 4 lines; keeps corrective fact ("ALREADY ON THE BATTLEFIELD" / "cannot touch a spell being CAST"), the shipped marker string `[this cannot target the spell on the stack]`, and the never-self-bounce anchor. Everything else byte-frozen (freeze-check table in seat findings). Triggered by item (f) confirmed fired + obeyed. |
| 102 | **DEPLOY** | `diff` = exactly three edits, 6878 → 7591 bytes (+713) | (1) symmetric-edict clause SHARPENED — added "the SACRIFICE IS THE OPPONENT'S CHOICE … will NOT stop a specific evasive clock … fire it to grind down an even board, never as your answer to the one creature that is killing you". (2) NEW painland-life crutch "MIND YOUR OWN LIFE FROM YOUR LANDS — Ancient Tomb and City of Traitors deal 2 to YOU each tap …". (3) Thoughtseize crutch RETIRED — the stale "top N of your library / do NOT list several" mislabel description replaced with lean strategic content ("the reveal prompt shows the OPPONENT'S hand … choose the ONE card … prefer a FLEXIBLE card … a card they can use NOW"). |

## Copy commands (DEPLOY rows)

```
cd /home/magi/Projects/wagicGPT/projects/mtg
cp strategy-design/wave21/deck14/strategy.txt   bin/Res/ai/baka/deck14_strategy.txt
cp strategy-design/wave21/deck102/strategy.txt  bin/Res/ai/baka/deck102_strategy.txt
```

decks 135, 35, 27, 62, 49 need no copy (already byte-identical to the deployed files, `cmp`-verified). Note `bin/Res`
is gitignored — if these guides are to be committed, they need `git add -f` (per project policy); guides are
LLM-drafted → the owner's authorship pass before any commit.

## Discrepancy flags

**None.** Each DEPLOY file differs from its deployed counterpart ONLY as its seat report describes (14 = one bullet
demoted, −617 bytes; 102 = three edits, +713 bytes) — verified by `diff`, no unexpected edits, no freeze-check
regressions. Each FROZEN file is `cmp`-clean byte-identical (135/35/27/62/49). All five claimed byte-copies matched the
deployed file exactly.

## Skill / core dispositions

- **Skill:** `strategy-design/wave21/strategy-writing-skill.md` is the full revised skill (copied from wave-20's rev,
  title bumped to wave 21, new wave-21 synthesis-notes header prepended + wave-20 header relabeled to lineage, wave-21
  changelog added). Boundary re-checked: all additions are guide-authoring / reviewer-instrument METHOD; engine/harness
  items are labeled "for reviewer awareness only" and routed to the ledger, never prompt text. Deploy target for the
  skill is the reviewer workflow (not a game asset) — no `bin/Res` copy.
- **Core prompt:** **PASS, 16th consecutive — no change, no `general-strategy.txt` produced.** `Res/ai/gpt/system_prompt.txt`
  is UNCHANGED (do not touch). Nothing this wave is both 2+-seat convergent and properly a core line; the decode-spiral
  item is a decode-sampler concern (repetition_penalty), not core prose.

## Rotation actions (for the harness/pool owner — NOT a guide-file change)

- **ROTATE OUT to canary:** decks 14, 35, 49 — their guides STAY deployed byte-identical (deck14 with this wave's
  demotion applied); they leave the active selfplay pool + per-deck review, guides retained as controls.
- **ROTATE IN (re-validation of older deployed guides):** decks 109 (Mono-R Aggro), 44 (UB Faerie Tempo), 140 (RBW
  Control). All three already have deployed `deckN_strategy.txt` — no guide-file action needed, just add them to the
  wave-22 selfplay pool. (Owner may swap any for a fresh guideless deck if pure coverage-expansion is preferred.)
- **WAVE-22 POOL: 135, 62, 27, 102, 109, 44, 140.** Run per the standard loop: `./tools/selfplay-harness.sh -r 1 -j 3
  -T 2400` (decision-quality corpus → `-j 3`, not `-j 6`).

## Between-wave engine/ledger work (the "review and address development notes" step)

Ranked in `synthesis-notes.md` (WAVE-22 ENGINE LEDGER). Top of queue: **ENGINE-F1 fetchland fizzle** (game-deciding;
reproduce on the current binary first, per project discipline — the item8-diagnosis probe pattern applies; a pinned
fetch-deck + stub endpoint discriminates the acting-player-shift vs stale-pointer mechanisms), then **HARNESS-N9
retracted_choice over-fire** (deck62-ready validator; the discriminating shape is specified). Enable a **modest
repetition_penalty pilot** for the wave-22 corpus. Oracle-verify **Ashenmoor Liege** (real = 4/4, `toughness=4` data
fix) before any primitive edit, per the project's verify-Oracle-first rule.
