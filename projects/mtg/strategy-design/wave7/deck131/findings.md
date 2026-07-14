# deck131 — wave 7 review (UR Guttersnipe spellslinger; deck name "Mind Control / Best Counter Deck Ever")

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260713-222831/`, one GPT-vs-GPT round-robin,
qwen35 both seats. **6 deck131 games.** Live guide in play = the wave-6 revision
(`bin/Res/ai/baka/deck131_strategy.txt`: two-gear identity, Guttersnipe cast-every-turn #1,
Mountain-first, promoted MULLIGAN block with the countable "count the Mountains; zero = Mulligan"
by-name override, attack/no-block, draw-on-curve).

**Record: 1 W / 4 L / 1 TIMEOUT (1/6).** Third straight bottom-tier wave (wave-6 1/6, wave-5 0/6).

Seat-file -> game map (seat epoch = results `start_epoch` + 2..3; confirmed by opponent card names):

| result | vs | file (epoch) | turns | end life me/opp | one-line cause |
|---|---|---|---|---|---|
| LOSS    | 44  | 1783999714 | 14 | -7 / 15 | first Mountain **not offered until T7** -> YP T9 vs tempo-fliers; raced |
| WIN     | 135 | 1784001601 | 17 | 5 / -3  | mulled a 0-Mtn hand; survived on counters, double-Guttersnipe closed |
| LOSS    | 109 | 1784004822 | 11 | 0 / 20  | Mountain in hand but not offered until T4; mono-red raced it; opp at 20 |
| LOSS    | 110 | 1784006250 | 9  | -4 / 31 | curved out (Mtn T0) but **chump-blocked with Guttersnipe**; affinity crushed |
| TIMEOUT | 140 | 1784006779 | 41+ | 11 / 28 | two-control stall; 2-dmg clock can't beat lifegain; Guttersnipe not down til T33 |
| LOSS    | 133 | 1784008428 | 17 | -1 / 23 | **FALSE mulligan** of a 2-Mountain keeper -> red-screwed 6 (first Mtn T10) |

**Opponent life at death in the four losses: 15, 20, 31, 23 - all high.** The wave-4/5/6 signature
(opp near/above 20 = deck131 never had a clock running) recurs unchanged. This is again an
**OBEYED-BUT-LOSING** corpus: the obeyed lines executed, the one win is the guide working as written,
and the losing record traces to (a) a newly-isolated INTERFACE defect and (b) deck construction -
NOT a guide regression. Freeze the obeyed lines; do not rewrite.

---

## TASK 1 - did the wave-6 mulligan sharpen FIRE? (count openers, precondition-count)

Wave-6 promoted the mulligan line to a labeled block, added a by-name override of the general keep-
default, and phrased the trigger as a countable ("count the Mountains in your opening hand; zero =
Mulligan; intercept the thought 'I'll cast my creature once I draw a Mountain'"). Wave-6 recheck
metric: `(no-Mountain openers mulliganed)/(no-Mountain openers)`, which was 1/3 in wave 6.

**Every opening hand this wave, counted:**

| vs | opener Mountains | correct call | pilot call | verdict |
|---|---|---|---|---|
| 44  | 1 Mtn (2 Isl, YP) | KEEP | Keep | ok |
| 135 | **0 Mtn** (3 Isl, YP, no draw spell) | MULLIGAN | Mulligan | ok (cited the rule verbatim) |
| 109 | 1 Mtn (2 Isl, YP) | KEEP | Keep | ok |
| 110 | 2 Mtn (1 Isl, YP) | KEEP | Keep | ok |
| 140 | 1 Mtn (2 Isl, YP) | KEEP | Keep | ok |
| 133 | **2 Mtn** (1 Isl, YP, Elixir, Prism, EScatter) | KEEP | **Mulligan** | **FALSE MULLIGAN** |

**True no-Mountain openers this wave: exactly ONE (vs135), and it was CORRECTLY mulliganed.** The
by-name override + countable phrasing fired on the one real precondition window. Metric: **1/1**
(wave-6 was 1/3). The sharpen is holding on true positives.

**BUT the sharpen OVERSHOT - INVERTED into a false mulligan (vs133).** The vs133 opener was
`Essence Scatter, Mountain, Mountain, Island, Elixir, Prism Ring, Young Pyromancer` - **two Mountains
plus a payoff, a textbook KEEP.** The pilot mulliganed it, reply verbatim: *"Mulligan this hand
because it contains zero Mountains... The strategy guide explicitly states that a hand with zero
Mountains is a mulligan."* It cited the guide while misreading its own hand as "zero Mountains" when
it plainly listed two. This is a guide-echo-while-misplaying (guide bug, per Step 0). **And it was
costly:** the mull-to-6 drew red-screwed - first Mountain not until T10, Young Pyromancer stranded to
T10 - and the game lost at T17. A keepable 2-land red opener was shipped into red screw.

**Precondition-count / honesty (my own wave-6 doctrine):** true no-Mountain openers n=1, false-mull
n=1 - BOTH under-powered. Do NOT declare the mulligan lever "fixed" on 1/1, and do NOT over-escalate
off one false mulligan. Net verdict: the sharpen is directionally working but has exposed a
FALSE-POSITIVE failure mode. The wave-7 fix is a GUARDRAIL, not a louder rule: re-anchor the trigger
to the literal hand list ("if the WORD 'Mountain' appears anywhere in your hand list, that hand HAS
red - keep it if it also has a payoff or a draw spell; the mulligan fires ONLY when no 'Mountain'
appears in the list at all"). Recheck metric wave 8: add `(keepable Mountain-hands kept)/(keepable
Mountain-hands)` alongside the no-Mountain-mull metric - this wave 5/6 (vs133 the miss).

---

## TASK 3 (feeds the headline) - the vs140 timeout: two-control stall, NOT a guide bug

deck140 held everything this corpus. vs140 ran 41+ turns with NO `gameend` record (timeout). Life
crawl: deck131 20->11, deck140 20->28 (GAINING). deck131 played YP T3, held counters, cast Cyclonic
Rift/Hydrolash as it could, drew Guttersnipe LATE and could not cast it until **T33**; by then opp had
gained past the 2-damage clock. This is the wave-6 vs140 finding reproduced at lower magnitude (wave 6
opp reached 143; here 28 and climbing): **a 2-damage-per-spell clock cannot out-race a lifegain deck
at any pilot quality.** Both decks are slow, neither can force the kill -> timeout. Route to
DECK-construction/tiers (deck131 has no reach once Guttersnipe is unavailable), NOT a guide fix. The
pilot's play here was fine (held counters, developed, no passivity flags). vs140 also shows the
land-menu defect (Mountain in opener, first Mountain played T3 - see below).

---

## THE WAVE-7 HEADLINE - a REPRESENTATION defect: "Mountain-first" is UNEXECUTABLE at the interface

Running the representation rung BEFORE any wording diagnosis (Step 0) on the recurring "red screw"
losses surfaced the real mechanism. **The land-drop menu offers ONE land at a time as
`Play <Land>` / `Hold <Land>`, and it NEVER co-offers Play Mountain and Play Island.** Verified: across
all 6 games and every decision, there is ZERO decision whose option list contains both "Play Mountain"
and "Play Island." So "choose Play Mountain over Play Island" is not a choice the pilot is ever shown.

When Islands are surfaced ahead of the Mountain (hand order), the pilot is offered only
`Play Island / Hold Island`, takes its land drop with the Island (correct-looking - it needs lands),
and **the Mountain in its hand is simply never presented for several turns.** Concretely, vs44 T1
offered `Play Island / Hold Island` in BOTH main phases with a Mountain sitting in hand the whole time;
Play Mountain first appeared at **T7**.

**First turn a Mountain was actually PLAYED, per game (and where it sat in the opener):**

| vs | Mountains in opener | first Mountain PLAYED | consequence |
|---|---|---|---|
| 110 | 2, **hand-first** | **T0** | curved out: YP T2, Guttersnipe T4 (menu offered Mtn first) |
| 140 | 1, after 1 Island | T3 | slow, survived into the stall |
| 109 | 1, after 2 Islands | T4 | YP T4 not T2; mono-red raced it; opp untouched at 20 |
| 44  | 1, after 2 Islands | **T7** | YP T9; tempo-fliers raced it; opp at 15 |
| 135 | mull-6, Islands first | T9 | won anyway on counters + double Guttersnipe |
| 133 | (false-mulled) 6-card | T10 | red screw compounded by the false mulligan |

**The deciding fact - that a Mountain is playable this turn - is invisible in the land menu whenever
the engine surfaces an Island instead.** The pilot cannot pick an option it is never shown. This is a
platform defect wearing a guide costume (Step 0's exact warning), and it PARTIALLY RE-FRAMES the
three-wave "red screw / manabase" narrative: in vs44 and vs109 the red source was IN HAND from turn 1
and the interface withheld it. vs110 is the control: when the menu offered the Mountain first, the
pilot curved out perfectly. This is the wave's highest-leverage, most model/harness-fixable lever -
and it is NOT the decklist.

**The guide stopgap is only best-effort:** holding an Island did not reliably surface the Mountain
same turn (vs44 held Island in MP1, was re-offered an Island in MP2, Mountain still not shown). So the
durable fix is the HARNESS (present all lands in hand as simultaneous Play options); the guide can only
carry an interim "hold non-Mountain lands to try to reach the Mountain," tagged unverified.

---

## FROZEN-LINE RECHECK (turnaround/obeyed-but-losing discipline - verify obedience, do not churn)

- **Guttersnipe cast-every-turn #1 (frozen).** Precondition-count: Guttersnipe reached the board with a
  castable spell window in vs135 (fired: Aetherize, Essence Scatter, Opportunity all cast as fuel,
  double-Guttersnipe assembled -> won) and vs140 (T33, attacked, few windows). In vs110 Guttersnipe was
  cast T4 then **thrown away blocking** T5 (see below) so it had no fuel windows. **Cast-nothing-while-
  Guttersnipe-out: ZERO** where checkable. Rule OBEYED where the precondition existed; still under-
  powered (Guttersnipe survived on board in only 1 of 6 games). FREEZE verbatim; recheck metric
  unchanged: `(spells cast while Guttersnipe on board)/(castable windows while Guttersnipe on board)`.
- **Attacker participation (frozen).** vs135 declared every legal attacker each combat (YP; YP+Gut;
  YP+Gut+Gut vs the `A#.` list); vs44 declared YP+Elemental then Elemental+Elemental; vs110 declared YP.
  100% participation where combats occurred. FREEZE.
- **Racing blocks (frozen, one violation).** vs135: "no blockers" at 18/16, chump-Elemental correctly at
  low life. **VIOLATION - vs110 seq7 T5: "Guttersnipe blocks Master of Etherium" at 14 life (NON-lethal
  incoming).** The guide says Guttersnipe stays alive in every case - the pilot chump-blocked its own
  kill condition. n=1, in a near-unwinnable affinity matchup (opp Master of Etherium 10/5 on curve, opp
  to 31), so route mostly to matchup - but reinforce "NEVER block with Guttersnipe; it is your only
  reach" as a one-line per-deck floor (reflexive-block convergence-watch instance; still per-deck,
  not core).
- **Mountain-first (frozen wording, now known unexecutable).** Obeyed only when the menu offered the
  Mountain (vs110). The failures are the interface, not disobedience - do NOT shout it louder. Keep the
  line, ADD the interface stopgap beneath it.
- **Threat-before-value / Prism Ring durdle (frozen, holding).** Prism/Elixir cast this corpus
  (vs44 1, vs109 1+Elixir, vs110 1+Elixir, vs133 1) - every one a "nothing better offered" window; the
  pilot never chose Prism/Elixir over a castable threat. Holding, as wave 6.

## Harness-defect sweep (required)
- **DESYNCS: 0** across all 6 games (head-first parser clean; matches waves 5-6). Transport clean.
- **ENGINE NO-OPS: none.** No repeated-pick-never-happened loops.
- **NUMERIC-OFFSET / menu traps: n/a** (no X-spells).
- **LAND-MENU REPRESENTATION DEFECT: present, 3 of 6 games** (vs44, vs109, vs140 - Mountain in hand
  withheld by the one-land-at-a-time menu). NEW this wave. Route to harness/representation owner.
- **TIMEOUT writes no `gameend`** (vs140) - wave-4/5/6 ask, still open.

## Routing summary
1. **[HARNESS / representation - NEW top model-fixable lever]** Land-drop menu: present ALL lands in
   hand as simultaneous `Play` options (or annotate other playable lands), so Mountain-first is
   choosable. Highest leverage; not the decklist.
2. **[DECK construction - 4th wave confirmed]** 14 Island / 8 Mountain over all-red payoffs -> 11M/11I;
   threat density 6/60; cut 4 Prism Ring for red sources / reach (Spellheart Chimera). Still real
   (vs109/133 genuinely low red even setting the interface aside).
3. **[GUIDE - model-fixable, low-ceiling]** (a) mulligan FALSE-positive guardrail (vs133); (b)
   Guttersnipe-never-blocks one-liner (vs110); (c) interim land-sequencing stopgap under Mountain-first,
   tagged unverified.
4. **[DECK/matchup - unwinnable/hard]** vs140 lifegain stall, vs110 affinity, vs44 tempo-fliers.
