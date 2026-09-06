# deck126 (Sanguine Blood) — wave-68 review

Seat files (corpus `matchups-20260906-134120`, one JSONL per seat, `...deck126-<ptr>-vs-<opp>`):
vs125 (109 rec), vs146 (29), vs123 (24), vs152 (19), vs162 (14), vs130 (28). Record **3-3**.
All counts below are read off the rendered `prompt`'s numbered option rows, not `options`.

## 1. Game by game

| opp | result | end | deciding decisions |
|---|---|---|---|
| 123 | WIN 51/0 | t17 | seq 12 Tribute on a NAMED non-defender victim (Thraben Doomsayer, "is sacrificed, you gain 2"); seq 17 Sanguine Bond t13, seq 18 Exquisite Blood t15 (pair closed the turn the second half was castable); seq 21-22 Staff of Nin + ping = win. Guide followed line for line. |
| 130 | WIN 36/0 | t20 | seq 2 shipped a 0-land 7 (guide rule); seq 3 KEPT a 1-land 6 (guide says SHIP — break, unpunished); seq 15-16 Tutor for Sanguine Bond with neither half held (guide rule 4); seq 23 Blood t17, seq 26 Bond t19 = win. 5 block windows, all correctly assigned. |
| 125 | WIN 6/65 | t41 | Won on the OPPONENT DECKING OUT, not the combo. seq 25 Bond resolved t20; seq 27 Exquisite Blood was COUNTERED (Fall of the Gavel); seq 56 + seq 80 re-Tutors were countered too. Pair never closed; `LOOP HALF PENDING ... in your graveyard` rendered correctly throughout — no engine defect here. |
| 146 | LOSS -5/18 | t17 | **seq 14** cast the Silencer-PRICED half over the free one (below). seq 23 blocker mis-assignment (below). Exquisite Blood then exiled from the battlefield; pair broken. |
| 152 | LOSS -7/20 | t11 | Drew ZERO creatures through t8; both combo halves stripped by two Elite Spellbinders. Two Spellbinders are [flying] and no deck-126 body has flying or reach — every block window (seq 11, seq 17) printed `NONE of your available blockers can block this attacker` for 8 of 11 incoming damage. No Tribute in hand at any window. No open lane. |
| 162 | LOSS -3/20 | t12 | Master of the Feast (5/5 [flying]) + Underworld Dreams + Howling Mine + Dictate of Kruphix. Every wall cast was inert against a lone flier; no Tribute drawn. seq 11 Sanguine Bond t11 was already too late (crack-back "you would be at 2"). No open lane. |

Both flier losses are the same shape and neither offered a window the guide could have answered:
Tribute to Hunger — the only card in the deck that answers an unblockable attacker — was never in hand
in either game. That is the deck's ceiling, not a guide fault.

## 2. Engine / interface / card items

**HIGH — stale guide claim caused by a render that has since improved (guide-side fix shipped, see §3).**
`deck146` seq 14: the row printed its own Silencer price —
`2. Cast Exquisite Blood {4}{b} [NAMED BY THEIR Silverquill Silencer: casting this costs you 3 life and draws them a card - you would be at 20. This price is PER CAST, not one-off...]`
The live guide said "The tag is on THEIR line, so your row will not price it - subtract the 3 yourself."
That sentence describes a screen the engine no longer prints. Not an engine defect — an audit debt on the
guide side; the render is correct and better than the guide assumed.

**MED — `[second copy: ...]` tag under-specifies which second copies are dead.**
Repro `deck125` seq 42 (t28):
`2. Cast Sanguine Bond {3}{b}{b} [second copy: you already control Sanguine Bond; both stay on the battlefield - no legend rule]`
and `deck125` seq 35 (t26) the same shape for Staff of Nin, against `deck152` seq 16
`1. Cast Chromatic Lantern {3} [second copy: ... the effect it gives your OTHER permanents is already on - this copy adds only its own abilities]`.
Only the Lantern row states that the copy is redundant. Per the primitives
(`mtg.txt:99004` Sanguine Bond `auto=@lifeof(player)...`, `mtg.txt:112425` Staff of Nin `auto={T}:damage:1`)
a second Bond doubles the drain per life gained and a second Staff is a second ping per turn — both live.
A one-clause distinction on the tag ("this copy adds a second trigger" vs "adds nothing new") would let
a pilot price the row without leaving the render. 3 takes / 11 offered windows; all three were reasonable.

**LOW — land-drop declines re-ask in the same turn.** `deck146` seq 18 declined the drop with two lands
listed and was re-asked at seq 20, where it played Scrubland. Same shape at `deck125` seq 41 and seq 90.
3 declines / 45 land-drop windows; no cost in any of them because the seam re-opened.

**LOW — no defect found in: HOLD rows, `NO LIVE CAST ROW`, `at 0 this does nothing`, `DRAW FORECAST`,
`LOOP HALF PENDING`, `LIFE-TO-DAMAGE CONVERTER`, block trade parentheses, the Tutor reveal list.**
4 fallbacks across 6 games (3 `plan_choice_conflict`, 1 `plan_contradicts_noop_row_reask`,
1 `index_name_conflict`), all in the 41-turn vs125 game, all recovered on the re-ask, none losing a decision.
Zero crashes, hangs, truncations or transport failures on this seat.

## 3. Guide verdict: EDIT (two changes, both paid; net +11 bytes, 19,952 -> 19,963)

**Edit 1 — resolve a guide-internal collision. Paid by `deck146` seq 14.**
The menu carried BOTH halves: row 1 `Cast Sanguine Bond {3}{b}{b}` unpriced, row 2 `Cast Exquisite Blood`
with the Silencer price. Cast-list entry 1 says "BOTH NAMES MISSING AND BOTH ROWS PRINTED: TAKE SANGUINE
BOND", but THE SILENCER TAG section says only "A half that does NOT [complete the pair]: skip while your
life is 12 or less" — at 23 life that section reads as permission to pay, and the pilot paid: 3 life, a card
to the opponent, and Sanguine Bond delayed to t12 (seq 19). It lost at -5. The collision, not the wording,
is the fault, so the fix goes where the later imperative lives.
before: `... THEY DRAW A CARD. The tag is on THEIR line, so your row will not price it - subtract the 3 yourself. A half that COMPLETES the pair this turn: pay and cast.`
after:  `... THEY DRAW A CARD. BOTH HALVES ON ONE MENU AND ONLY ONE ROW CARRYING THAT PRICE: cast the UNTAGGED half. A half that COMPLETES the pair this turn: pay and cast.`
The cut sentence is the stale one from §2 (the row DOES price it), so the addition is paid for by the cut.
Prediction: on a window whose menu prints both halves and exactly one `[NAMED BY THEIR Silverquill
Silencer` row, the untagged half is taken. Denominator: such windows. A hit is a break only if the untagged
half was castable on that same menu. Removal condition (belief-correcting): the render carries the
comparison itself AND zero breaks across two corpora with the both-halves-printed shape.

**Edit 2 — a false card fact. Paid by the primitive, corroborated by `deck125` seq 42.**
before: `a second copy of either does nothing.`  after: `a second copy of either does not complete it.`
`mtg.txt:99004` gives Sanguine Bond an unconditional `@lifeof(player)` trigger with no uniqueness clause,
so two copies both fire; the pilot's own reading at seq 42 ("having two Sanguine Bonds means every life
gain triggers twice") was rules-correct and the guide called it dead. The rule's job — a second copy is not
the second HALF — survives intact; only the false generalisation is gone. Zero net rules.
Removal condition: none (a card fact).

**KEEP, with the counts that earned it:**
- THE ZERO STOP: 1 take / 71 windows offering `at 0 this does nothing` (28 distinct game-turns). The one
  take (`deck125` seq 102, t40, 15 untapped sources, game already decided by decking) cost nothing. Holds.
- Land drop: 42 takes / 45 windows; see §2 LOW.
- Blocking STEP 1 + STEP 2: 9 of 11 block windows correct, including all 5 in the vs130 win and the
  correct gang-block at `deck123` seq 10 (both defenders onto the lone attacker, two triggers).
  The 2 breaks are both in vs146 and both single instances of already-correctly-keyed rules, so no
  fourth paraphrase: `deck146` seq 12 blocked a [first strike, deathtouch] 1/1 with Pride Guardian at 20
  life with no enchantment out (STEP 1 says bench it — gained 3, lost the wall), and `deck146` seq 23 put
  Perimeter Captain (`A2-A4 (neither dies)`) on the `deals 1` Goblin instead of a `deals 3` Silencer and
  the Vampire on A1 rather than A2/A3 where the row printed `{lifelink, this block: you gain 1, and your
  converter takes 1 off them}` — 4 life and 1 drain given away. One break in 6 windows that offered the
  choice is not a pattern; the rule stays as written.
- Win button / cast order: both wins closed within two turns of the pair landing, with no wall cast ahead
  of a half in any window.
- Mulligan: 5 keeps + 1 ship, all matching the rule except `deck130` seq 3 (kept a 1-land six the rule
  ships) — violated-unpunished on one sample, which never loosens a rule. No odds line moved.
- The Lantern promotion clause ("above entry 4") did not reach `deck162` seq 9, where the pilot cast a
  THIRD Perimeter Captain with an enchantment half in hand and 4 sources, against a board whose only
  attacker was a 5/5 [flying]. Promoting the Lantern above entry 3 is the obvious candidate, but the
  counterfactual does not hold: at t11 (seq 11) the pilot had 5 sources and cast Sanguine Bond anyway, so
  the Lantern would have changed no cast. Unpaid; not edited. Flagged for the next corpus with a flier deck.

## What I did NOT check
The opponent seat files were read only for board context, never counted. I did not adjudicate any
lane-BA..BE prediction (engine seat's job), did not measure latency, prompt bytes, or the short-guide
effect, and did not review any deck other than 126. I did not verify the deck126 decklist against the
primitives beyond the six cards named above (Sanguine Bond, Exquisite Blood, Staff of Nin, Tribute to
Hunger, Perimeter Captain, Pride Guardian) — Scryfall was not consulted; the primitives are the source.
