# Deck-110 (Etched Affinity) — Wave-8 game review

**Corpus.** PRIMARY control arm `matchups-20260714-044131/`, 6 deck110 seat translogs,
206 logged decisions (ask 106, priority 54, attackers 29, blockers 6, gameend 6, defer 1).
Opponents identified from the new `gamestart` header. Card facts verified against
`bin/Res/sets/primitives/mtg.txt`; representation facts verified against the live prompts.
Citations are `<epoch prefix>` + seq (+ turn).

## Record: 5 WIN / 1 LOSS (beat 135, 133, 131, 109, 140; lost 44) — REBOUND from wave-7 4/2.

| file (epoch) | opp (deck) | result | end T | end life | one-line cause |
|---|---|---|---|---|---|
| 1784022094 | Modern Snow (135)        | **WIN**  | 11 | my17 / op0   | went wide, closed |
| 1784025509 | Phyrexian Asphodel (133) | **WIN**  | 8  | my18 / op-3  | fast close (was a LOSS in wave-7) |
| 1784026638 | Mind Control (131)       | **WIN**  | 11 | my18 / op-8  | two Platings, blowout |
| 1784028007 | Wipe Them Out! (140)     | **WIN**  | 15 | my20 / op-10 | double-Lightmine avoided, closed w/ bodies+Blast |
| 1784029007 | Hellrider (109)          | **WIN**  | 8  | my20 / op-3  | aggro race won |
| 1784027527 | Faerie Archmage (44)     | **LOSS** | 18 | my-11 / op7  | Master/Champion/Plating COUNTERED; raced out by a flier |

Result column of results.tsv = winner index (0=first-listed deck, 1=second). Life/turn columns
adjudicated. This is OBEYED-BUT-LOSING / turnaround-preservation mode AGAIN: same guide, one variable
flipped per game. vs133 and vs140 both flipped LOSS->WIN vs wave-7 with no matchup-specific change —
pure draw variance, exactly as the wave-6/7 variance-entry test predicted. **Freeze the obeyed lines.**

## HARNESS SWEEP (required)

- **0 desyncs** across 206 records (head-int == choice everywhere it applies).
- **1 defer record** (NEW record type): `1784028007` (vs140) seq3 T1, `fallback:deferred_to_heuristic`,
  chosen `Glimmervoid`. Cross-ref `game-140v110-*.stderr`: "model chose land Glimmervoid but it fails
  validation; deferring to heuristic". The model's land pick (seq2 "Play Glimmervoid") failed
  validation and the heuristic played the SAME land — zero strategic harm. This is the new `defer`
  record making a validation-drop machine-readable. Harness/engine item -> notes, not guide.
- **1 unparsed_reply** (model degenerate output): `1784026638` (vs131) seq18 T5 attackers,
  `fallback:unparsed_reply`, choice=-1. Reply is `Answer:\n###\n###...` garbage; only one attacker
  (Signal Pest 1/2) was offered; the engine fell back to the heuristic. Non-fatal (vs131 was a blowout
  win, opp -8). Model-capability degenerate output -> model-experiments residuals, NOT guide.

Every scored PLAY below is a genuine decision.

---

## VALIDATION TARGET 1 — RE-ATTACH MARKER: STILL FIRED, FULLY OBEYED (headline validation, CLOSED).

Wave-7 confirmed the marker fixed and fired (28/29, no-op 50%->6%). Wave-8 re-verifies it held:

- **Marker string renders** (`(ALREADY attached to it - this would change NOTHING)`) — exact string
  confirmed on live equip options; matches the guide's quote and the two-power cue
  (`Master of Etherium (10/5) (printed 0/0)`) also renders.
- **16 marked-offer decisions; marker TAKEN 0 times.** (wave-7: 28 offered / 1 taken; wave-6: the
  string appeared 0 times.) Fully obeyed this wave.
- **No-op re-equip rate: 0 marked no-ops across 9 equip picks (0%)** — DOWN from wave-7's 6% and
  wave-6's 50%. The compressed Rule #1 (wave-7 demotion to load-bearing tokens) is holding.
  **FREEZE Rule #1 verbatim.** No further work; this loop is closed end-to-end (shipped -> fired ->
  obeyed, three waves running).

## WATCH — MULTI-COPY EQUIP CHURN: CLEAN this wave.

Wave-7 flagged a 6-decision Plating shuffle (vs131 T9). Re-measured: **9 equips total, ALL justified,
no aimless shuffle.** Breakdown:
- First-equips onto an unattached carrier (vs133 s7/s19, vs131 s26, vs140 s12/s17/s38, vs109 s19).
- ONE genuine 2nd-Plating attach: vs140 s39 T13 — Master already carried one Plating; the pilot
  attached the SECOND (unattached) Plating to Master (10/5 -> ~18/6). Correct per guide ("two equips
  only when you cast two Platings"), not churn.
- ONE purposeful REDISTRIBUTION: vs140 s42 T15 — both Platings on Master (18/6); the pilot moved one
  to newly-cast Etched Champion (-> 9/3 with protection-from-colors) to add an EVASIVE threat and
  spread damage in a race it was winning. A state-changing (unmarked) move with a real reason, inside
  a win (opp -10 T15). Not aimless.

The per-instance marker structurally cannot mark a copy-shuffle (moving Plating-A off a carrier that
holds Plating-B is a genuine state change) — the wave-7 engine-representation note stands — but it did
NOT recur as a costly loop. Watch stays low priority; no guide/engine action this wave.

---

## THE WAVE-8 LEVER — the pilot HAND-COUNTS artifacts while the engine now SURFACES the exact count.

**New this wave (engine ledger #3 family):** the prompt carries a dedicated line
`Artifacts in play: you N | opponent N`, alongside the new `Your battlefield (creatures: N)` header.

- **The surfaced count is 100% accurate: 180/180 decisions, ZERO offset.** Precise recompute (perms
  tagged `[artifact]` + artifact lands Darksteel Citadel + artifact creatures, EXCLUDING Glimmervoid)
  matches `Artifacts in play: you N` in every record. It correctly includes Mox Opal / Darksteel
  Citadel and correctly EXCLUDES Glimmervoid.
- **The pilot ignores it and hand-counts — and repeatedly OVER-counts.** Grepping replies for stated
  artifact counts against the surfaced N: the pilot frequently mis-enumerates by folding in
  non-artifacts:
  - `1784026638` (vs131) s5-s12: pilot's PLAN says "3 artifacts" while `you 2` is on the board — a
    steady +1 over-count.
  - `1784028007` (vs140) s33 T11: pilot's PLAN reads "With 6 artifacts on the battlefield (2 Cranial
    Platings, 2 Glimmervoids, Darksteel Citadel, Island)" — counting **2 Glimmervoids AND a basic
    Island as artifacts** — while the surfaced line says `you 3`. The exact Glimmervoid/land error the
    guide already warns about, committed with the correct number sitting one line above.
  - `1784022094` (vs135) s16-s17: pilot "5" vs surfaced `4`; `1784025509` (vs133) s6/s18: "3" vs `2`,
    "5" vs `4`.

**Outcome impact this wave: no proven loss** — the over-counts landed in wins, and the metalcraft-ON
conclusion (N>=3) held because the deck floods artifacts fast so the boundary was rarely close. But
this is a textbook REPRESENTATION DEFECT surviving EVEN IN WINS (the skill's deck133-P2 rung: "a pilot
that manually recomputes a value the option already displays is a representation defect, even in a
win"), and it is a live risk at the metalcraft boundary: an over-count can fire a Galvanic Blast
expecting 4 damage on a board that is only 2 artifacts (Blast deals 2, not 4), or believe Etched
Champion has protection when it does not. The number is surfaced, accurate, and Glimmervoid-clean;
the pilot just isn't reading it.

**Guide consequence — the ONE model-fixable lever this wave (representation READ upgrade).** Per the
skill's "prefer an engine-SURFACED number over a hand-computed one — and VERIFY its offset" (verified
0/180 here): teach the pilot to READ `Artifacts in play: you N` for Cranial Plating's bonus AND
metalcraft (N>=3), and stop hand-counting. This SIMPLIFIES the guide (the surfaced count already
excludes Glimmervoid, so the pilot no longer has to reason about the exclusion — just read N).
Edited THE ENGINE section, the LETHAL CHECK metalcraft clause, the Mox/Citadel/Glimmervoid bullet, and
added one DECIDING SITUATIONS recap line. Everything else FROZEN.

---

## vs44 LOSS — RE-ATTRIBUTED: counter + evasion race, NOT the wave-7 flood.

Wave-7's vs44 loss was a threat-starvation FLOOD (opp never below 20). **This wave is different: the
pilot built a real board and got the opponent to 7.** Cast sequence: Memnite/Signal Pest (T1), Master
(T3), Cranial Plating + Memnite + 2 Vault Skirge (T5), Ornithopter (T7), Etched Champion (T9), and
onward — opp life 20->18->16->14->11->9->7. Loss cause is a STACK of construction/meta axes, not a
guide-wording failure:

1. **Counter-vulnerability (meta shift, confirmed in stderr).** `game-44v110-*.stderr`: opponent
   Counterspelled **Cranial Plating** and **Etched Champion** on the stack (targets logged), and the
   log shows Master of Etherium answered as well. deck110's wins route through a few discrete
   high-value threats — exactly what the now-working counter representation answers 1-for-1.
2. **Evasion clock deck110 cannot block.** The opponent's whole clock was `Archmage of Echoes (4/4
   ->6/6) [flying]`. deck110 can only block a flier with Vault Skirge or Ornithopter; by the time life
   got critical (my9 T15 -> my3 T17 -> my-11 T18) those flyers were spent/countered/dead, leaving
   only Signal Pest (0/1) and Memnite (1/1) on the ground. The flier ran deck110 over unblocked.
3. **Reach starvation at the finish.** At T15 and T17 the pilot cast Cranial Plating from hand with NO
   creature to equip and no Galvanic Blast drawn — 4 damage short of closing from opp 7.

This is the deck109-class evasion cousin of the standing counter/flood flag: a race deck110 got INTO
and lost to an evasive clock + hard counters. Not guide-fixable at n=1; route to the deck-construction
/ meta flag. (Marginal play thought — holding a Vault Skirge back to chump the Archmage earlier — is
single-seat, low-confidence, and the flyers were largely gone before it mattered; recorded as a watch,
not a guide change, to preserve the frozen race clauses.)

---

## ENGINE-CHANGE VERIFICATION (wave-7 ledger, per brief)

1. **Land-drop enumeration — CONFIRMED FIRED.** 16 multi-land co-offers for deck110 (e.g. "Play
   Mountain" + "Play Island" + "Play no land right now" in one options_text; also Citadel+Mountain,
   Island+Citadel). deck110's guide carries NO color-priority land stopgap (it is a flexible
   artifact deck; land color is not gated), so there is nothing to drop — noted, no action.
2. **No-legal-target / defer visibility — CONFIRMED.** 1 defer record (vs140 seq3, harmless land
   validation-drop, above). No `fails validation` play-quality events for this seat beyond it.
3. **Creature-count header + tapped "untaps and can attack next turn" tag — CONFIRMED and accurate.**
   `Your battlefield (creatures: N)` renders; the tapped-flier tag renders on the Archmage
   (`[tapped - untaps and can attack next turn]`). The creature-count header did NOT corrupt
   metalcraft/affinity reasoning (the pilot counts artifacts, not creatures); the artifact miscount is
   a separate hand-count issue addressed by the lever above.
5. **Blocker-seam arithmetic — CLEAN, no reflexive-block leak.** 6 blocker windows; all correct:
   vs135 s20 Etched Champion (protection from green) blocks Abominable Treefolk 8/8 for FREE (prevents
   8, Champion survives); vs131 s22 Master 3/3 trades up into Young Pyromancer 2/1; vs44/vs109 chumps
   at appropriate life. No racing creature chumped a 1/1 at high life; no missed must-block. The
   wave-6/7 attack-seam survival clause continues to behave.

## Corpus health
- cast-nothing 11/64 cast-decisions (17%), concentrated in vs140 (6/16, the double-Lightmine game
  whose guide ORDER is "cast nothing / don't attack") and vs131 (4/13); vs135/vs133/vs109 ran 0. No
  durdle regression.
- attackers 29, blockers 6 — both seams exercised.

## HANDOFF (OBEYED-BUT-LOSING / frozen-line diff)
1. **ADD the artifact-count READ** (the one model-fixable representation lever): teach reading
   `Artifacts in play: you N` for Plating power + metalcraft; simplify the Glimmervoid caveat.
2. **FREEZE everything else verbatim**, including the wave-7-compressed Rule #1 (marker fully obeyed).
3. Route to notes (not guide): the vs140 defer, the vs131 degenerate reply, the multi-copy-churn
   engine watch, and the vs44 counter/evasion/reach construction flag.

## FREEZE-CHECK (obeyed-but-losing mode)
Every wave-7 line survives verbatim in wave-8 strategy.txt EXCEPT the artifact-count edits:
- Identity / kill-by-5-6 / cast-nothing=lost-turn / beatdown-by-default — VERBATIM.
- Rule #1 (marker, two-power cue, release, mana destination) — VERBATIM (wave-7 compression kept).
- CASTING mana falsity-fact + Memnite/Ornithopter {0} — VERBATIM.
- THE ENGINE — kept structure; the parenthetical "(artifact count is shown on the board - usually +4
  or more)" REPLACED by the explicit `Artifacts in play: you N` read + Glimmervoid-already-excluded
  note (representation-read upgrade). Steps 1-3 + carrier order VERBATIM except Champion's "3+
  artifacts" anchored to the surfaced line.
- THE RACE (go-wider / chump-at-8 / <=8 no-attack-with-needed-blocker) — VERBATIM.
- ATTACK EVERY TURN / Signal Pest / Etched Champion / Lightmine rewrite — VERBATIM.
- LETHAL CHECK — metalcraft clause anchored to `Artifacts in play: you N`; rest VERBATIM.
- PLAYING AROUND COUNTERS — VERBATIM (validated doctrine; counter+evasion, not wording, lost vs44).
- OTHER KEY CARDS — Mox/Citadel/Glimmervoid bullet's metalcraft phrasing anchored to the surfaced
  line + Glimmervoid-exclusion restated as "the count already leaves it out"; Steel Overseer / Master
  / Thoughtcast VERBATIM.
- MULLIGAN — VERBATIM.
- DECIDING SITUATIONS — VERBATIM + ONE new recap line ("read Artifacts in play: you N, use N
  directly") placed LAST.
