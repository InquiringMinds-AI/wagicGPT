# Deck-110 (Etched Affinity) — Wave-9 game review (FINAL wave before pause)

**Corpus.** PRIMARY `matchups-20260714-094942/`, 6 deck110 seat translogs, 200 scored
decisions (ask 113, priority 65, attackers 18, blockers 4; +6 gamestart/gameend). Binary
`/tmp/wagic-a40671057`, **wave-8 guides deployed** (the live `deck110_strategy.txt` = the
wave-8 strategy.txt, artifact-count READ edit included), post-A/B core (hand->stack anchor
gone). Card facts verified vs `bin/Res/sets/primitives/mtg.txt`; representation facts vs the
live prompts. Citations = `<epoch-tag>` seat + seq (+turn).

## Record: 4 WIN / 1 LOSS / 1 TIMEOUT(behind)

| seat (tag) | opp | result | end T | end life | one-line cause |
|---|---|---|---|---|---|
| 5643102c | Faerie Archmage (44) | **WIN**  | 13 | my13 / op-12 | flooded bodies + Blasts, closed (was a LOSS in wave-8) |
| 55c8b4ee | Modern Snow (135)    | **WIN**  | 7  | my20 / op0   | went wide, fast close |
| 55b90213 | Phyrexian Asphodel (133) | **WIN** | 12 | my10 / op-4 | Overseer+Champion beatdown |
| 557deb94 | Mind Control (131)   | **WIN**  | 11 | my9 / op-2   | Signal Pest + equipped Champion |
| 5637160a | Hellrider (109)      | **LOSS** | 11 | my-3 / op5   | got opp to 5 early, too few bodies to close, raced down |
| 555beb04 | Wipe Them Out! (140) | **TIMEOUT** | 29 | my~1 / op18 | deck140 Staff-of-Nin grind; deck110 BEHIND |

Win/loss is variance + deck design, cited as context only (USER DOCTRINE). This is
OBEYED-BUT-LOSING / turnaround-preservation mode for the FOURTH consecutive wave: same guide,
one variable flipped per game (vs44 flipped LOSS->WIN, vs109 flipped WIN->LOSS, both pure draw
variance with no matchup-specific change — the wave-6/7 variance-entry prediction). **Freeze
the obeyed lines.**

## HARNESS SWEEP (required)
- **0 desyncs** across 200 records (head-int == choice where it applies).
- **0 defer records** (wave-8 had 1) — corpus-wide zero, matches the brief.
- **0 unparsed_reply** (wave-8 had 1 degenerate vs131 attackers) — clean this wave.
Every scored PLAY below is a genuine decision.

---

## VALIDATION TARGET 1 — RE-ATTACH MARKER: STILL FIRED, FULLY OBEYED (CLOSED, 3rd wave running).
- Marker string `(ALREADY attached to it - this would change NOTHING)` renders on live equip
  options; the two-power cue (`Signal Pest (4/1) (printed 0/1)`) also renders.
- **47 marked-offer decisions; marker TAKEN 0 times.** (wave-8: 16 offered / 0 taken;
  wave-7: 28/1; wave-6: string appeared 0x.) Fully obeyed, larger sample than wave-8.
- **14 equips chosen, all onto valid unattached-or-better carriers; 0 marked no-ops.**
  The compressed Rule #1 (wave-7 demotion) is holding. **FREEZE Rule #1 verbatim.** Loop closed
  end-to-end (shipped -> fired -> obeyed, three waves). No further work.

## WATCH — MULTI-COPY EQUIP CHURN: CLEAN.
Per-game equip target sequences: vs44 1 equip, vs135 1, vs133 2 (Signal Pest->Champion, a
carrier upgrade), vs131 2 (Signal Pest->Champion, upgrade), vs109 1, vs140 7 across a 29-turn
grind (Vault Skirge->Master->Champion->Overseer->Signal Pest->Signal Pest->Ornithopter — carriers
dying/re-entering in a long race, plus two Platings in the deck). No aimless A<->B shuffle; the
two consecutive Signal-Pest equips (vs140 s42/s48) are separated by 6 turns of board turnover, not
a churn loop. The per-instance marker still structurally cannot mark a copy-shuffle (wave-7/8
engine note stands) but it did NOT recur as a costly loop. Low-priority watch; no action.

---

## THE WAVE-9 HEADLINE — the artifact-count READ edit (wave-8 lever) PARTIALLY landed; the
## Glimmervoid/basic-land FOLD-IN false belief SURVIVED the instruction.

**Setup.** Wave-8's one model-fixable lever taught the pilot to READ `Artifacts in play: you N`
for Cranial Plating power + metalcraft (N>=3) and to STOP hand-counting, because the surfaced
count is 100% accurate and Glimmervoid-clean. Wave-9 is the first corpus testing that edit.

**Result — a genuine but PARTIAL win.** The surfaced count is again 100% accurate (recomputed
across every deck110 record, zero offset; it counts Mox Opal + Darksteel Citadel + artifact
creatures and correctly EXCLUDES Glimmervoid). The pilot now MATCHES the surfaced N on the
majority of decisions where it states a count (e.g. vs131 s15/s18-21, vs133 s12/s13/s15/s20/s22,
vs140 s22-54, vs109 s24-27 all match) — an improvement over wave-8's "ignores it and hand-counts."
**BUT the false belief the READ was meant to kill is still firing:** the pilot repeatedly
re-derives the count from the board and folds in Glimmervoid AND basic lands:
- `5643102c` (vs44) s7 T5: "**Play Glimmervoid to increase my artifact count to 3, activating
  Metalcraft**" while `you 2` is surfaced. Glimmervoid is a LAND; playing it does NOT raise the
  artifact count or turn on metalcraft.
- `5637160a` (vs109) s28 T8: "I have **5 artifacts on board (Mox Opal, 2x Darksteel Citadel,
  Cranial Plating, Glimmervoid)**" while `you 4` is surfaced — folds Glimmervoid.
- `555beb04` (vs140) s59 T27: "5 artifacts (Darksteel Citadel, Mox Opal, Cranial Plating,
  **Mountain, Glimmervoid**, plus 3 Islands...)"; s65 T29: "**8 artifacts: Darksteel Citadel, Mox
  Opal, Cranial Plating, Mountain, Glimmervoid, Island, Island...**" — counting Mountains and
  Islands as artifacts. Surfaced `you 5`/`you 8` sat one line above.
- Steady +1/+2 over-counts also at vs135 s4 (surf1 "3"), vs131 s7 (surf2 "3" x6), vs140 s3
  (surf1 "3"), vs109 s7-9. NOTE: most of THESE are correct FORWARD projections ("cast X to reach
  3") — not misreads — and are fine; the Glimmervoid/land fold-ins above are the true error class.

**Outcome impact this wave: NO proven misplay — again.** Every fold-in landed where it did not
flip the pick:
- vs44 s7: the options were land-drops ONLY (Play Island / Play Glimmervoid / Play no land) —
  playing Glimmervoid is correct regardless; the metalcraft-timing belief was wrong but changed no
  decision. It cast Cranial Plating at s8 anyway.
- Every Galvanic Blast fired (vs44 s17/s19 surf6, vs140 s7 surf3, vs109 s15 surf4) was at a board
  with genuine metalcraft ON (N>=3), so each Blast correctly dealt 4. **No Blast fired at surf<3**
  — the metalcraft boundary was never DECIDED by a fold-in (the deck floods artifacts so fast the
  boundary is rarely close). vs109 s25's "7 lethal" attack was correct Plating math (Memnite 1 +
  6 artifacts), not an over-count; it failed only because it was blocked.
The live risk wave-8 named (fire a Blast for 4 on a 2-artifact board, or believe Champion has
protection when it doesn't) remains UNREALIZED but LIVE.

**Doctrinal reading — this is the representation-beats-instruction WALL, now two waves deep at
this seam.** The number is surfaced, accurate, Glimmervoid-clean, the guide POINTS at it, AND the
guide flatly states the corrective fact ("never re-add Glimmervoid or a basic land") — and the
pilot STILL re-derives and mis-tags. Per the skill's own doctrine (representation beats
instruction; a pilot that manually recomputes a displayed value is a representation defect even in
a win), the fix is NOT more/louder guide wording — escalating here would be the escalate-at-a-wall
anti-pattern. The wave-8 READ edit is as far as the GUIDE can push this. **The residual lever is
REPRESENTATION, and the specific lever is battlefield-TAG COMPLETENESS** (see notes E3): the
per-permanent battlefield line tags SOME artifacts `[artifact]` (Cranial Plating, Elixir) but
NOT Darksteel Citadel (an artifact land) and NOT artifact CREATURES (Etched Champion, Signal Pest,
Memnite, Vault Skirge, Ornithopter). A pilot that re-derives from those incomplete tags gets it
wrong in BOTH directions — under-counting artifact lands/creatures AND over-adding Glimmervoid +
basics. Tag EVERY artifact (and leave Glimmervoid/basics untagged) so re-derivation AGREES with
the summary N. That closes the mis-tag at its source. **Guide: FREEZE the wave-8 wording verbatim.**

---

## vs109 LOSS + vs140 TIMEOUT — construction/variance, not guide-fixable.
- **vs109 (Hellrider aggro), LOSS my-3 T11.** deck110 got the opponent to 5 life by T4 (Blast +
  equipped Memnite) then STALLED for lack of bodies while a fast red clock raced it 15->13->...->-3.
  This is the RACE the guide already addresses ("go WIDER"); the pilot's problem was material (few
  creatures drawn), not obedience. One low-confidence thought: s25 declared a single 7/1 Memnite as
  "lethal" into an untapped Stromkirk Noble that chumped it — a phantom-lethal cousin (asserted a
  game-ending attack without pricing the blocker) — but n=1, and with Blast spent and only 2 bodies
  it changed little. WATCH only; freeze the race clauses.
- **vs140 (Staff of Nin grind), TIMEOUT, deck110 BEHIND (~1 vs 18).** The known deck140 grind
  class: Staff-of-Nin/Lightmine attrition ground deck110 down over 29 turns. Triage latency, not a
  deck110 play failure. Route to the standing construction flag; not a guide task.

## ENGINE-CHANGE VERIFICATION (per brief)
1. **Land-drop enumeration — CONFIRMED FIRED.** Multi-land co-offers render ("Play Island" + "Play
   Glimmervoid" + "Play no land right now"; Mountain+Glimmervoid). deck110's guide has NO
   color-priority land stopgap to drop (flexible artifact deck) — nothing to demote.
2. **Defer visibility — CONFIRMED (zero this seat).** 0 defer records; no play-quality validation
   drops.
3. **Creature-count header + tapped tag — CONFIRMED accurate.** `Your battlefield (creatures: N)`
   renders; `[tapped - untaps and can attack next turn]` renders. Did not corrupt affinity reasoning
   (the pilot counts artifacts, not creatures; the artifact miscount is the separate tag issue above).
4. **Blocker-seam arithmetic — CLEAN.** 4 blocker windows, all correct (vs109 s20 Memnite chumps
   the Noble to deny its trigger; s27 Vault Skirge 6/1 eats the Noble for free). No reflexive
   high-life block; no missed must-block. Attack-seam survival clause continues to behave.

## Corpus health
- cast-nothing 7/66 cast-decisions (10%; wave-8 17%), concentrated in the vs140 grind. No durdle
  regression.
- attackers 18, blockers 4 — both seams exercised.

## HANDOFF (OBEYED-BUT-LOSING / frozen-line diff; PAUSE-clean)
1. **FREEZE the entire wave-8 guide verbatim.** The wave-8 artifact-count READ edit is as far as
   the guide can push the fold-in; escalating wording would hit the representation wall. strategy.txt
   is a verbatim copy of the live wave-8 guide.
2. Route to notes (not guide): the artifact-count fold-in -> REPRESENTATION lever = battlefield
   `[artifact]`-tag COMPLETENESS (E3, sharpened); the multi-copy churn engine watch (E2); the vs109
   single-attacker phantom-lethal cousin (watch, n=1); the vs140 grind + vs109 race
   construction/meta flag (DC1).

## FREEZE-CHECK (obeyed-but-losing mode) — EVERY wave-8 line survives verbatim.
- Identity / kill-by-5-6 / cast-nothing=lost-turn / beatdown-by-default — VERBATIM.
- Rule #1 (marker, two-power cue, release, mana destination) — VERBATIM (47/0 obeyed).
- CASTING falsity-fact + Memnite/Ornithopter {0} — VERBATIM.
- THE ENGINE (wave-8 `Artifacts in play: you N` READ + Glimmervoid-already-excluded note) —
  VERBATIM. Held despite the residual fold-in: the wording is correct and the fix now lives at the
  representation layer, so the guide is no longer the only defense to escalate.
- THE RACE / ATTACK EVERY TURN / Signal Pest / Champion / Lightmine — VERBATIM.
- LETHAL CHECK (metalcraft anchored to `Artifacts in play: you N`) — VERBATIM.
- PLAYING AROUND COUNTERS — VERBATIM (validated; vs44 flipped to a WIN this wave going wide).
- OTHER KEY CARDS / MULLIGAN / DECIDING SITUATIONS (incl. the wave-8 recap line) — VERBATIM.
