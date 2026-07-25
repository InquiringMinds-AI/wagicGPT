# Wave-26 deployment manifest (for the integrator -- do NOT deploy from the synthesis seat)

Corpus `matchups-20260725-115216`, binary `77e91ef4e` (-T 3000, REPPENALTY=1.05). Guides live at
`projects/mtg/bin/Res/ai/baka/deckN_strategy.txt` (`bin/Res` is gitignored -> NEW guides need
`git add -f`; existing tracked guides need `-f` on commit too). Core prompt
`bin/Res/ai/gpt/system_prompt.txt`: **NO CHANGE** (21st consecutive PASS, 7-of-7 seats). The wave
artifact `strategy-design/wave26/strategy-writing-skill.md` is the reviewer instrument, not a Res
asset -- it is NOT deployed.

PROVENANCE: the live guides for 27/102/131/133/137 currently carry the WAVE-25 revisions (all live
mtimes 2026-07-25 08:28:41, predating the 11:52 wave-26 corpus -- clean; no seat self-deployed).
This wave: 27/102/133 are byte-frozen (their wave-26 seat file == the deployed wave-25 guide),
131/137 are revised again, 18/22 are net-new.

## cmp-verification of the seat strategy.txt files vs the LIVE deployed guides

Verified with `cmp -s`, byte sizes, a U+FFFD scan (`/usr/bin/grep -c $'\357\277\275'`, must be 0),
and md5.

| deck | disposition claim | cmp result | live bytes | seat bytes | FFFD | seat md5 | action |
|------|-------------------|-----------|-----------|-----------|------|----------|--------|
| 18  | NEW initial guide (Step-0 debut) | **NO LIVE FILE** | -- | 5940 | 0 | 8efec498... | **DEPLOY (new)** |
| 22  | NEW initial guide (Step-0 debut) | **NO LIVE FILE** | -- | 5385 | 0 | 5315a045... | **DEPLOY (new)** |
| 131 | REVISED (favorable-block + lethal reconciliation) | **DIFFERS** | 12161 | 13264 | 0 | 8791d3ba... | **DEPLOY (+1103 b)** |
| 137 | REVISED (B-list-authoritative blockers clause) | **DIFFERS** | 8038 | 8491 | 0 | 957717d5... | **DEPLOY (+453 b)** |
| 27  | FROZEN (byte-identical, converged) | **IDENTICAL** | 10948 | 10948 | 0 | ff16d630... | **NO ACTION** (ROTATE OUT) |
| 102 | FROZEN (byte-identical, converged) | **IDENTICAL** | 9545 | 9545 | 0 | e9921eb7... | **NO ACTION** (ROTATE OUT) |
| 133 | FROZEN (byte-identical, converged) | **IDENTICAL** | 10190 | 10190 | 0 | fae6240f... | **NO ACTION** (ROTATE OUT) |

Notes on the verification:
- **The two NEW guides (18, 22) are FFFD=0 and plausibly sized** (5940 b / 5385 b -- mid-band for an
  initial guide, comparable to the deck59 first-guide band). No decode garbage, no mojibake. deck18
  carries the trust-the-rendered-number rule #1, the Dodger evasion-as-attack-order line, the
  ground-only can't-block-flyers RACE off-case, the Field-Marshal first-strike gang-block answer, the
  Mobilization interface note, and a lean mono-white mana/mulligan line. deck22 carries the
  Automaton-IS-a-Giant rule #1 (cross-phase to mulligan+bottom), the per-item attack floor over the
  Giant list, and the Mogg Sentry / Stinkdrinker exclusions.
- **131 / 137 DIFFER as expected** (pure additive revisions; neither shrank, neither ballooned;
  FFFD=0). 131 = +1103 b (favorable-block exception on the never-block absolute + a one-line
  lethal-attack reconciliation clause). 137 = +453 b (the blockers-side inverse of the wave-25
  attack-list clause: the offered B-list is authoritative, do not re-derive tapped state from the log).
- **27 / 102 / 133 are byte-identical** to their live (wave-25) guides (`cmp` exit 0, md5-confirmed).
  All three ROTATE OUT (converged); the byte-frozen status is the terminal record -- no deploy, and
  the files stay as the canary reference for those seats.

## CARD-NAME / STALE-LINE FLAG (deck18 <-> ledger seed 3)

The deck18 guide teaches ENGINE behavior (correct for the current binary), NOT real-MTG Oracle
behavior, because the Zealous Guardian / Thistledown Liege card-script fixes have NOT shipped:
- **Zealous Guardian** -- guide treats it as a `{W/U}` Soldier ATTACKER cast off Plains alone (lines
  15-16, 31, 33): correct for the ENGINE ({W/U} 1/1 Soldier, no defender). Real card = `{W}` 2/2
  Defender.
- **Thistledown Liege** -- guide states `{1}{W/U}{W/U}{W/U}` 1/3 LORD (line 36): correct for the
  ENGINE 1/3. Real card = 4/4.

**These guide lines are correct NOW but become STALE if ledger seed 3 (the card-script fixes) lands.**
A `{W}` 2/2 Defender cannot attack (invalidates lines 31/33 treating Zealous Guardian as a Soldier
attacker) and uses no hybrid pip (invalidates the lines-15-16 castability note); a 4/4 Thistledown is
a beater, not a 1/3 buff. **If seed 3 is worked at wave-27, the deck18 guide MUST be revised in the
SAME step** (drop Zealous Guardian from the attacker/Soldier lists + the hybrid-castability line;
correct Thistledown's base P/T). Ship the card fix and the guide revision together, or neither. No
action THIS wave -- the guide is correct against the deployed binary.

## Deploy actions (integrator, step 4 -- apply to live Res; guides are runtime data, no rebuild)

1. `cp strategy-design/wave26/deck18/strategy.txt bin/Res/ai/baka/deck18_strategy.txt`
   -- NEW initial Kithkin guide (`git add -f`).
2. `cp strategy-design/wave26/deck22/strategy.txt bin/Res/ai/baka/deck22_strategy.txt`
   -- NEW initial Giants guide (`git add -f`).
3. `cp strategy-design/wave26/deck131/strategy.txt bin/Res/ai/baka/deck131_strategy.txt`
   -- favorable-block exception (Guttersnipe survives a power<2 attacker = free removal, not a trade)
   + a one-line lethal-attack reconciliation clause (attacking Guttersnipe into a blocker IS right
   when the swing is lethal).
4. `cp strategy-design/wave26/deck137/strategy.txt bin/Res/ai/baka/deck137_strategy.txt`
   -- the offered B-list is authoritative: a B-line creature is untapped and can block this turn even
   if the log says it attacked or a same-named creature is tapped; take the "you kill it, your blocker
   lives" block; do not re-derive tapped state from the log.
5. deck27 / deck102 / deck133: **NO ACTION** -- live guides already byte-identical (`cmp`- and
   md5-confirmed). All three rotate OUT to the converged-canary set this wave.

## Post-deploy verification (integrator)
- Re-`cmp` each deployed guide against its seat strategy.txt (exit 0) for 18 / 22 / 131 / 137.
- Confirm `git status` shows deck18/22 `_strategy.txt` as NEW (untracked -> `git add -f`) and
  deck131/137 `_strategy.txt` modified. `bin/Res` is gitignored -> ALL need `-f` if committed. NO AI
  attribution on any commit (user's own repo).
- No binary rebuild (guides are runtime data). Core prompt untouched -> no rebuild for it either.

## Rotate-in guides (step 4, AFTER the wave-26 corpus is committed)
The three wave-27 rotate-ins (see synthesis-notes.md "Rotate-in picks") enter GUIDELESS as Step-0
newcomers -- they have decklists (`deckN.txt`) but NO `_strategy.txt` and produce their initial
guides on their FIRST reviewed corpus (wave-27), exactly as deck18/22 did this wave. Nothing to
deploy for them this step; just add deck134 (Urza's Control) + deck93 (Relentless Rats) + deck136
(Yarok-of-Pranks black-midrange) to the wave-27 matchup roster in place of deck27, deck102, deck133.

## Between-wave engine/card ledger work (step-4 ledger -- see synthesis-notes.md RANKED LEDGER)
Not part of guide deployment; worked separately before the wave-27 corpus. Ranked highest:
(1) R-DUPLICATE-NAME-INSTANCE (per-instance handle -- the witnessed correctness breach; the wave-27
rotate-in pool is chosen to force its 2nd witness); (2) R-CHANGELING-TYPE-INFERENCE render
annotation; (3) the Zealous Guardian + Thistledown Liege card-script fixes (CO-DEPENDENT with a
deck18 guide revision -- see the flag above); then the doubleside parse-poisoning sweep, the
R-DFC-BACKFACE live-probe-verified fix (now warranted -- the probe harness is proven), the set-hint
silent-drop loud rejection, and the lower cosmetic/interface items. Full ranked list + the six CLOSE
recommendations in synthesis-notes.md.

## DO NOT DEPLOY from this seat.
The synthesis agent produces the manifest; the integrator applies it (development-loop step 4).
Deployment and the between-wave engine-ledger work happen in step 4. No self-deploy occurred this
wave (all live guides predate the corpus). Pool swap this wave: deck27 + deck102 + deck133 rotate OUT
(converged); three guideless Step-0 decks rotate IN (see synthesis-notes.md).
