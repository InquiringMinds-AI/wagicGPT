# Wave-53 lane O — pricing annotations (D3, D5, D6, D13)

Branch `w53-lane-O`, worktree `worktrees/lanes/w53-O`, base master 3dfd77d65.

## Baseline re-confirmed on this worktree (seeded binary, before any edit)
PARSETEST **2121 passed / 0 failed**; suite (single-threaded, `WAGIC_FASTCLOCK=0.1`)
**1210 tests / 0 failed, 44 AI / 0 failed**, `==Test Failed !==` 0, `==Test timed out` 0.
(The brief expected the 2 known concurrency-only failures; single-threaded is ground truth and
showed none, so no third failure to chase.)

## Gate after the change
PARSETEST **2159 / 0** (+38 cases). Suite **1211 / 0 failed, 44 AI / 0 failed**
(+1 fixture), `==Test Failed !==` 0, `==Test timed out` 0. `git diff | grep -c U+FFFD` = 0.

## Files
- `projects/mtg/src/AIPlayerGPT.cpp` (all four items + the 38 PARSETEST cases)
- `projects/mtg/bin/Res/test/w53o_tragic_slip_kill_verdict.txt` (new, `git add -f`)
- `projects/mtg/bin/Res/test/_tests.txt` (registry line)

## D3 — the DRAW PRICE lethal tail (HIGH)
`castDrawPriceRowTag` (src/AIPlayerGPT.cpp, verified at ~6797 pre-edit) took a fifth
argument `life` (default -1 = not supplied -> prints nothing, so the wave-49 four-argument
shape is byte-identical). When the tag states a damage total it now continues with lane K's
tail: `- you would be at K`, and `; this KILLS you` at K <= 0. The one call site
(the cast-row emitter, ~17282 pre-edit) passes the seat's `life`.
- Repro grounding: `1788320083-…deck125…vs…deck162`, 41 rows / 22 windows at life <= 7,
  seq 91 declined, seq 97 took row 1 and the seat died at seq 99.
- PARSETEST: seq-97 shape at 3 life (`at -4; this KILLS you`); exactly-lethal at 7;
  the must-NOT-match negative at 20 life (`- you would be at 13`, no `KILLS`); a
  no-punisher negative (no total, therefore no tail); the 4-arg regression; the echo
  (a reply parroting the whole tagged row binds, and the tag leaves no narration residue).
- **Falsifiable prediction (wave 54):** `[DRAW PRICE:` rows whose stated damage >= the
  seat's life carrying `this KILLS you`: **N/N** (was 0/41); casts taken from such a row:
  **0** (was 1).

## D5 — the cast row's kill summary (HIGH)
- The kill test each target verdict prints is now a predicate (`damageKillsTarget`,
  `ptDropKillsTarget`); `damageTargetVerdict` / `ptDropTargetVerdict` call it, so the two
  surfaces cannot answer differently.
- The SPELL halves of the target ask's magnitude derivation were lifted into
  `spellSingleDamageAmount` / `spellPTDropAmount` (same scoping, verbatim: one `damage:`
  rider, no inline `target(...)`, x/rand never evaluated, no non-creature power/toughness,
  live-morbid branch through `parseCastRestrictions`). The target ask now CALLS them — one
  reader of the script, two surfaces.
- The cast row collects the enumerated legal targets (`tgtCards`, parallel to the existing
  name/handle/tail vectors) and emits `castKillSummaryTag`:
  `{kills 0 of the 3 legal targets at -1/-1}` when nothing dies, `{kills: Elite Spellbinder}`
  when something does. `{kills…}` was added to `stripNarrationDecoration` (decision-time
  pricing, never history).
- Repro grounding: `123v146` seq 18/19 (6 life, Tragic Slip taken over three survivors);
  85 verdict-free cast rows vs 9 target rows with one.
- PARSETEST: the zero-kill positive, the singular form, the damage-magnitude form, a named
  single victim, several victims, both must-NOT-match negatives (0 creature targets;
  unknowable magnitude -> empty), the predicate table, the ECHO shape, and an
  agreement pin sweeping (magnitude x remaining toughness x indestructible x deathtouch)
  asserting the cast summary and the printed verdict never disagree.
- Suite pin (engine half): `w53o_tragic_slip_kill_verdict.txt` — morbid off, `-1/-1` leaves
  Gray Ogre (2/2) at 1/1 on the battlefield and puts Goblin Piker (2/1) in the graveyard.
  It is GREEN on base by construction: the change is render-only, so no fixture can be RED
  on base; the RED-on-base pins for this lane are the 38 PARSETEST cases.
- **Falsifiable prediction (wave 54):** targeted-spell cast rows for a knowable fixed damage
  or `-N/-N` magnitude carrying a kill count or a named victim: **N/N** (was 0/85); removal
  spells cast where every legal target survives: **0** (was 1).

## D6 — a defender's tap costs the BLOCK (MED)
`paymentTapsClause` now takes a per-source restriction vector (`TAP_RESTRICT_NONE /
_NO_ATTACK / _NO_BLOCK`); the wave-49 row-wide `bool` overload is kept and forwards a uniform
vector, so every existing pin is unchanged. `paymentTapRestrictionOf` decides per source: a
non-creature loses nothing; a `defender` loses the block (`- it cannot block on their turn`),
and never claims an attack it never had; anything else keeps `- it cannot attack this turn`,
still only while the attack is ahead of it. A row whose sources differ names each with its own
consequence, `; `-separated. `blockStillMatters` suppresses the block wording once the
opponent's blockers are already declared. Both call sites (activation rows ~13350, cast rows
~16967 pre-edit) build the vector.
- Card fact verified on disk: `Overgrown Battlement` `abilities=defender` (mtg.txt:83596).
- Repro grounding: `126v130` seq 13 (14 -> 8 next combat), `126v125` seq 86; 101
  `{paying this taps:` rows corpus-wide, 91 naming a defender, 22 with the attack tail.
- PARSETEST: the wall positive; the must-NOT-match negative (`cannot attack this turn` absent);
  the plural uniform form; the mixed three-source form; a no-restriction negative; the empty
  negative; the bool-overload equivalence regression; the echo shape.
- **Falsifiable prediction (wave 54):** `{paying this taps:` rows naming a `defender` source
  that print `cannot attack this turn`: **0/N** (was 22 tails over 91 defender-naming rows);
  rows naming a defender that print `cannot block on their turn`: **N/N**.

## D13 — the converter drain, folded into the row that causes it (MED)
`lifeToDamageConverterTake` reads the amount off the converter's own script line
(`:life:-thatmuch` mirrors the gain, `:life:-N` is that N, anything else 0 = say nothing);
`lifeLoopDrainClause` prints ` - and their <name> takes N off YOU: life 14 -> 10`, with
`; this KILLS you` at 0 or less. `edictClause` gained three optional arguments plus the
victim-facts string, so the wave-49/50 shapes are byte-identical without them. At N=1 the row
now names the determined victim with its `(P/T)` and live keyword tag (`[defender]` etc.,
through `keywordList`). The caller scans the OPPONENT's battlefield for the converter, and only
when the script's gain goes to THEM (`toughnesslifegain targetcontroller`).
- Repro grounding: `123v126` seq 66 (14 life vs 26; 14 -> 10 and 26 -> 30 in one window).
- PARSETEST: the `thatmuch` and fixed-N reads; three must-NOT-match negatives (the mirror
  half, a self-gain payload, an unreadable amount); the clause positive and its lethal form;
  the empty-input negatives; the full edict row with victim facts + drain; the
  no-converter negative; the caster-gains negative; the three legacy-shape regressions.
- **Falsifiable prediction (wave 54):** edict rows printing `they gain N` while a
  life-to-damage converter of theirs is on the battlefield, carrying the drain continuation:
  **N/N** (was 0/1 observed); N=1 edict rows naming the victim without its `(P/T)`: **0/N**.

## What I did NOT verify
- **No live game was run.** All four items are proved by PARSETEST (pure emitters) plus the
  one engine fixture; none of the four clauses has been observed in a real translog. The
  wave-54 corpus is the real gate for every prediction above.
- **D5 fight rows are NOT done.** The ledger's D5 paragraph mentions the
  `152v146` seq 49-51 werewolf fight two-step. A fight is scripted as
  `transforms((,newability[target(creature) dynamicability<!powerstrike eachother!>]))`, and
  the waiting granted ability arrives payload-stripped, so neither the fighter's power nor
  the "eachother" shape is readable at the seam without an engine-side handle on the dynamic
  ability. I did not guess one. This is a carry, not a claim that it is unnecessary.
- **D6's `blockStillMatters` gate is untested live.** The suppression case (tapping a defender
  after the opponent's blockers are declared) has no PARSETEST because it is decided at the
  call site, not in the pure emitter.
- **D13's converter scan takes the FIRST converter found** on their battlefield. Two
  converters of theirs would under-state the drain; I did not sum them, because the second
  trigger's amount is only knowable if both scripts are `thatmuch`.
- The D5 cast-row summary counts only creature targets currently on their controller's
  battlefield; player targets and non-creature permanents are excluded from the denominator.
- Concurrency (`THREADS=16`) was not run; the gate is single-threaded ground truth only.
