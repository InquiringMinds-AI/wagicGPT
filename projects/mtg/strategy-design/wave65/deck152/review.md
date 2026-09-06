# Wave 65 - deck 152 (Bant Midrange) per-deck review

Seat files: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-033121/*deck152-*-vs-*.jsonl`
Record 5-1. 0 fallbacks, 0 re-asks, 0 transport errors across all six seats.
Guide: `projects/mtg/bin/Res/ai/baka/deck152_strategy.txt` (68040 B, in band).

## 1. Game by game

| opp | result | turn | life | deciding decisions |
|---|---|---|---|---|
| 146 | WIN | 18 | 20 / 0 | seq 17 counter onto Briarbridge Tracker (5/4, unsick, connecting); seq 27 `Briarbridge Tracker -> Lolth, Spider Queen` - walker attacked by identity, cleanly; seq 43 alpha with Cathar+Sigarda+Wolf for the kill. Clean. |
| 125 | WIN | 17 | 20 / -2 | seq 14 Tovolar's Huntmaster on curve; seq 22 four-body swing 25->8; seq 30/35/41 counters onto the FLYING Sigarda each combat - exactly the guide's "prefer one they cannot block at all". seq 37 cast a 2nd Sigarda under a rendered legend-rule warning (LOW, below). |
| 126 | WIN | 15 | 34 / -6 | model stacked every Luminarch counter onto the flying Elite Spellbinder (seq 29/34/36/49/51/59/61: 4/2 -> 12/10) and rode it from 20 to -6. Textbook execution of the guide's evasion clause. |
| 130 | WIN | 17 | 25 / -2 | 26 decisions total. Brutal Cathar exiled the blocker chain (seq 13/16/22), Moonrage Brute alpha seq 25. Clean. |
| 123 | WIN | 15 | 20 / -12 | Tovolar wolves seq 17->21->30. One misplay: seq 29 put the Luminarch counter on the Aspirant ITSELF with a 7/7 Packleader and a flier on the list (MED-1). Won anyway. |
| **162** | **LOSS** | 15 | 0 / 4 | see below - lost by one turn to a draw-punisher clock, and the decision that cost the turn is HIGH-1. |

### The loss (152 vs 162, `1788683506-ai_baka_deck152-0x55865d9b7e40-vs-ai_baka_deck162.jsonl`)

Opponent landed Ob Nixilis T10, Underworld Dreams T12, Fate Unraveler + 2nd Underworld
Dreams T14, on top of Howling Mine + 2x Dictate of Kruphix. 152 holds no enchantment
removal, and never drew Fateful Absence in this game - so the punishers were unanswerable
by construction, not by pilot error. 152 still took the opponent 20 -> 4.

**The turn that decided it - turn 13, seq 35/36/37.** Three Luminarch Aspirant "put a
+1/+1 counter" triggers, all three targeted **Briarbridge Tracker, which was cast that same
turn and could not attack**. The model's own PLAN at seq 34/36/37 reads "Attack with
Intrepid Adversary #1, Intrepid Adversary #2, and Briarbridge Tracker. Deal 11 damage to
win immediately" - it believed the Tracker was an attacker. At seq 38 the attackers menu
offered no Tracker row and it swung for 7 (blocked down to 4).

Counterfactual, using the same board: the two Intrepid Adversaries have **lifelink**. Three
counters onto Adversary #2 makes it 6/4; the swing is 3+6+1 with Shield Sphere eating one
body. That is ~3 more damage AND ~3 more life. 152 entered turn 15 at 8 and the draw step
dealt exactly 8 (seq 41-49, 8 -> 0). At 11 it survives at 3, with the opponent at 1 and a
7/6 Tracker plus two Adversaries untapping into an empty board. The counter placement, not
the punisher package, is what lost the race.

## 2. Engine / interface items

### HIGH-1 - target-choice rows omit summoning sickness, though they carry every OTHER attack restriction
`projects/mtg/src/AIPlayerGPT.cpp:3553` `summoningSickTag()` has exactly **one** caller,
`:7641`, the battlefield serializer. The target-list builder never calls it.

Repro A - loss game seq 35, rendered target list (verbatim):
```
6. Briarbridge Tracker (4/3) [vigilance] [your battlefield] - "Vigilance -- When Briarbridge Tracker enters, investigate. ..."
```
while the CURRENT SITUATION block, ~20 lines above in the same prompt, reads:
```
Briarbridge Tracker {2}{g} (4/3) (printed 2/3) [vigilance] [summoning sick - cannot attack this turn, but CAN block]
```
The model answered off the row it was told to answer from and threw away the game (above).
Repeated at seq 36 and 37 with the tag still absent.

Repro B - 123 game (`...deck152-0x55b6ea8faec0-vs-...deck123`) seq 29 shows the machinery
IS wired for restrictions on target rows, just not this one - every row carries
`[doesn't untap during its controller's untap step]`:
```
1. Tovolar's Packleader (7/7) [doesn't untap during its controller's untap step, nightbound] [your battlefield] - ...
7. Luminarch Aspirant (1/1) [doesn't untap during its controller's untap step] [your battlefield] - ...
```
So the fix is a one-tag addition at the target-row builder, matching `:7641`. Per the
perception-vs-strategy doctrine this is a RENDER fix, not a guide teach: the pilot read a
true-but-incomplete surface at the point of decision.

### HIGH-2 - DRAW FORECAST is frozen inside its own draw step and double-counts against the stack line
`drawStepForecastText()` (`AIPlayerGPT.cpp:10931-10967`) computes `k` from the static draw
SOURCES (`base` + Howling Mine + each Dictate). When `stepIsNow` flips the wording to
"resolving NOW", `k` is not reduced by the draws that have already resolved.

Loss game, seq 41 through 49 - nine consecutive windows, the line identical in all nine:
```
DRAW FORECAST: your draw step, resolving NOW, draws 4 cards (1 + Howling Mine 1 + Dictate of Kruphix #1 1 + Dictate of Kruphix #2 1) = 4 x 4 = 16 life LOST BY YOU to their punishers above ...
```
while the same prompts' narration already showed `- You drew Hengegate Pathway` /
`- You drew Elite Spellbinder` and the stack held only two Draw-1 abilities. The prompt
therefore states, in two adjacent lines, `ON THE STACK: 7 damage to you` **and** a further
16 - a total of 23 against a true remaining ~15.

Cost is measurable: seq 42 `latency_ms 190040`, `post_answer_overrun 2927`, and its reply
is the model trying and failing to reconcile the render -
`"Wait, the forecast says 16 life lost. Let's re-read carefully. ... Total damage incoming = 7 (stack) + 16 (draw) = 23."`
Both of deck152's two `later_answer_ignored` records (seq 42, seq 43) are this same
re-derivation overrunning the plan block. The first-wins rule (AO) handled them correctly -
the discarded tails would not have changed either answer - but the tokens were spent
because the forecast lied.

### HIGH-3 - the hold key includes volatile annotation payload, so a hold cannot hold (the G7 re-offer churn, measured)
Loss game turn 15 Draw: **9 consecutive no-op priority windows, seq 41-49, 333,919 ms of
inference**, on a menu whose only non-pass row was suicide (`Draw 1 with Clue ... this
KILLS you`). The model took the Hold row four times (seq 41, 43, 44, 47, 48) and was
re-asked every time.

Diff of seq 41 -> seq 42 shows why: the row TEXT is identical apart from the DRAW PRICE
projection embedded in it -
`... so taking it costs you 4 life right now - you would be at 4` becomes `... - you would be at 3`.
The prompt's own annotation admits it:
`[hold check: 1 row above is new since the last window at this seam - a row moving is what re-opens a hold ...]`
Nothing actionable moved; only a life-derived number inside the row's bracket did. The
hold key should be row IDENTITY (option + cost + source), with volatile projections
excluded from it.

Deck-152 corpus total: **16 no-op priority windows in 4 runs, ~630 s of inference**
(126 game T11/T13/T15 Blockers seqs 38-40 / 53-54 / 64-65; loss game T15 Draw seqs 41-49).

### MED-1 - Luminarch counter onto the Aspirant itself (STRATEGY, guide already covers it)
123 game seq 29: chose `7. Luminarch Aspirant (1/1)` from a list containing a 7/7
Packleader, four 2/2 Wolves, a 4/3 Tracker and a 3/1 flier. Reply was a bare `CHOICE: 7`
with no reasoning. The live guide already states "Never put the counter on the Aspirant
itself while another creature of yours can attack" (deck152_strategy.txt:377). Prose exists
and was ignored; this is the same seam as HIGH-1 and is better served by the row tag than
by more prose.

### LOW-1 - second legendary cast under a correct warning
125 game seq 37: cast a 2nd Sigarda off a row reading
`[legendary: you already control Sigarda, Champion of Light - legend rule: casting this sends one copy to your graveyard (you choose which)]`.
Render was correct and complete. It was Main 2, the alternative was "Cast nothing", and
binning the tapped copy for an untapped 4/4 flier has marginal blocker value, so this is
noted, not charged.

### Not checked
Mulligan/bottoming quality beyond "Keep this hand" at seq 2 in all six games (no mulligan
window arose). The opponent seats were read only for blocks and cast timing. No claims made
about the 162v123 `-1089` life loop - that game is not in this deck's set.

## 3. Guide verdict: **KEEP**

`deck152_strategy.txt` stays as shipped (68040 B, in the 41-71 KB band). Reasons:

- 5-1 with zero fallbacks and zero re-asks; the five wins are the guide's own lines being
  executed, not variance. The 126 and 125 games are the guide's Luminarch clause
  ("preferring one they cannot block at all") working verbatim - counters onto the flier,
  20 -> -6 and 22 -> -2.
- The one loss is a matchup 152 cannot answer on cards (no enchantment removal in the
  75 against 2x Underworld Dreams + Dictates), and the pilot still got 162 to 4.
- The single decision that could have flipped it is a PERCEPTION failure (HIGH-1): the
  model believed a summoning-sick body would attack because the row it answered from did
  not say otherwise. The doctrine's own rule is that guide prose against a surface that
  stays silent at the decision point loses; the fix belongs at the target-row builder.
  Adding a sentence would grow a near-ceiling guide to teach around a one-tag render gap.
- MED-1 is a violation of a rule the guide ALREADY states in the exact terms needed
  (line 377). Restating it is noise.

No `strategy.txt` written. No general-guide or skill proposals: nothing in this deck's six
games is evidence for a change to the shared surfaces that HIGH-1 through HIGH-3 do not
already cover as engine work.
