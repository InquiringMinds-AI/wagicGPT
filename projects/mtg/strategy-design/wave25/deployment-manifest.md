# Wave-25 deployment manifest (for the integrator — do NOT deploy from the synthesis seat)

Corpus `matchups-20260725-035605`, binary `/tmp/wagic-62d8783a5` (-T 3000, REPPENALTY=1.05).
Guides live at `projects/mtg/bin/Res/ai/baka/deckN_strategy.txt` (`bin/Res` is gitignored ->
new guides need `git add -f`; existing tracked guides need `-f` on commit too). Core prompt
`bin/Res/ai/gpt/system_prompt.txt`: **NO CHANGE** (20th consecutive PASS, 7-of-7 seats). The
wave artifact `strategy-design/wave25/strategy-writing-skill.md` is the reviewer instrument,
not a Res asset -- it is NOT deployed.

NO process deviation this wave: every live guide's mtime predates the wave-25 review window
(no seat self-deployed to live Res). All five revised seat files DIFFER from their live guides
(i.e. the revisions have NOT yet been applied) -- clean provenance for step-4 deployment.

## cmp-verification of the seat strategy.txt files vs the LIVE deployed guides

Verified with `cmp -s bin/Res/ai/baka/deckN_strategy.txt strategy-design/wave25/deckN/strategy.txt`
(exit 0 = byte-identical), plus byte sizes, a U+FFFD scan (`/usr/bin/grep -c $'\357\277\275'`,
must be 0), and live-file mtimes.

| deck | reviewer claim | cmp result | live bytes | seat bytes | FFFD | live mtime | disposition |
|------|----------------|-----------|-----------|-----------|------|-----------|-------------|
| 27  | REVISED (4 surgical edits) | **DIFFERS** | 9178 | 10948 | 0 | 07-24 17:20 | **DEPLOY** (+bottom-phase rule-#1 clause + edits, +1770 b) |
| 102 | REVISED (1 edit: "early = by turn 2") | **DIFFERS** | 9192 | 9545 | 0 | 07-24 05:50 | **DEPLOY** (mulligan fuzzy-term resolved, +353 b) |
| 131 | REVISED (2 rules-correctness edits) | **DIFFERS** | 10910 | 12161 | 0 | 07-24 17:20 | **DEPLOY** (bottom-step recalibration + never-block lethal/only-blocker exception, +1251 b) |
| 133 | REVISED (1 edit: line 126 Arena) | **DIFFERS** | 9689 | 10190 | 0 | 07-24 05:50 | **DEPLOY** (Phyrexian Arena skip-rule sharpen, +501 b) |
| 137 | REVISED (Lovestruck adventure-body fix + ROLE CHECK) | **DIFFERS** | 6528 | 8038 | 0 | 07-24 17:05 | **DEPLOY** (+1510 b) |
| 59  | BYTE-COPY (validated as-is; ROTATE OUT) | **IDENTICAL** | 4399 | 4399 | 0 | 07-24 17:20 | **NO ACTION** (md5 5bd3675eb91efe2e6605dc7daf6649c0) |
| 109 | BYTE-COPY (frozen; ROTATE OUT) | **IDENTICAL** | 8729 | 8729 | 0 | 07-24 05:50 | **NO ACTION** (md5 a8f163f7f2bf1ecce5c9f4f13997ccf1) |

Notes on the verification:
- **All 5 revised seat files have FFFD = 0 and plausible sizes** (pure additive edits; no seat
  shrank, none ballooned). No decode garbage, no mojibake.
- **deck59 / deck109 are byte-identical** to their live guides (`cmp` exit 0, md5 confirmed).
  Both are ROTATING OUT this wave (converged); the byte-copy status is the terminal record --
  no deploy, and the files stay as the canary reference for those seats.
- deck109's live md5 `a8f163f7f2bf1ecce5c9f4f13997ccf1` matches the reviewer's cited hash.
- deck137's live guide (6528 b, mtime 07-24 17:05) is the wave-24 self-deployed guide; the
  wave-25 seat (8038 b) is a genuine revision (Lovestruck Beast adventure-body misread fix +
  strengthened ROLE CHECK) and DIFFERS from live -> a normal step-4 deploy (NOT self-deployed
  this wave).

## Deploy actions (integrator, step 4 -- apply to live Res; guides are runtime data, no rebuild)

1. `cp strategy-design/wave25/deck27/strategy.txt bin/Res/ai/baka/deck27_strategy.txt`
   -- 4 surgical edits incl. a bottom-phase rule-#1 clause (a mana-simplification fact must
   reach the BOTTOM phase: hybrids are EASIEST, ship a double-pip card before a flexible hybrid).
2. `cp strategy-design/wave25/deck102/strategy.txt bin/Res/ai/baka/deck102_strategy.txt`
   -- resolve the mulligan fuzzy term ("early = by turn 2"); the model over-indexed on turn-1
   speed the guide never required.
3. `cp strategy-design/wave25/deck131/strategy.txt bin/Res/ai/baka/deck131_strategy.txt`
   -- 2 rules-correctness edits (CORRECT-DEVIATION-FLAGS-A-GUIDE-DEFECT): bottom-step
   recalibration replacing the miscalibrated "never bottom a draw spell" absolute with an
   impact/redundancy ranking; a never-block lethal-AND-only-blocker exception.
4. `cp strategy-design/wave25/deck133/strategy.txt bin/Res/ai/baka/deck133_strategy.txt`
   -- sharpen the Phyrexian Arena skip-rule (name the timing + "stabilize/find answer"
   rationalization the model used to cast Arena at 5 and 7 life under lethal pressure; note the
   model's false belief that Arena draws on cast -- script is `@each my upkeep:draw:1`).
5. `cp strategy-design/wave25/deck137/strategy.txt bin/Res/ai/baka/deck137_strategy.txt`
   -- fix the declined-lethal-Lovestruck-Beast misplay (a resolved adventure creature
   `(1/1) [adventure]` is a REAL body; trust-the-engine-legality stated BOTH directions) +
   strengthened ROLE CHECK.
6. deck59 / deck109: **NO ACTION** -- live guides already byte-identical to the seat files
   (`cmp`-confirmed, md5-confirmed). Both rotate OUT to the converged-canary set this wave.

## Post-deploy verification (integrator)
- Re-`cmp` each deployed guide against its seat strategy.txt (exit 0) for 27 / 102 / 131 / 133 / 137.
- Confirm `git status` shows deck27/102/131/133/137 `_strategy.txt` modified. `bin/Res` is
  gitignored -> ALL need `-f` if committed. NO AI attribution on any commit (user's own repo).
- No binary rebuild (guides are runtime data). Core prompt untouched -> no rebuild for it either.

## Rotate-in guides (step 4, AFTER the wave-25 corpus is committed)
The two rotate-in decks (see synthesis-notes.md "Rotation reconciliation") enter GUIDELESS as
Step-0 newcomers -- they have decklists (`deckN.txt`) but NO `_strategy.txt` and produce their
initial guides on their FIRST reviewed corpus (wave-26), exactly as deck59 did. Nothing to
deploy for them this step; just add them to the wave-26 matchup roster in place of deck59 and
deck109.

## Between-wave engine/harness work (step-4 ledger -- see synthesis-notes.md WAVE-26 ENGINE LEDGER, ranked)
Not part of guide deployment; worked separately before the wave-26 corpus. Highest: #1
R-TRUNCATED-TARGET-PREVIEW (deck109 -- a perception gap suppressing a legal play by omission;
owner's standing rule = fixable perception flaws go first). Then R-RESTLESS-PUMP-OPTION-AMBIGUOUS
(deck59), the deck199 steal + R-DFC-BACKFACE probe (discharges both, then deck102 rotates), the
wave-24 parser-hardening ship-decision (deck133 belt-and-suspenders), and the lower-priority
representation/watch items. Full ranked list in synthesis-notes.md.

## DO NOT DEPLOY from this seat.
The synthesis agent produces the manifest; the integrator applies it (development-loop step 4).
Deployment and the between-wave engine-ledger work happen in step 4. No self-deploy occurred
this wave. Pool swap this wave: deck59 + deck109 rotate OUT (converged); two guideless Step-0
decks rotate IN (see synthesis-notes.md).
