# wave-81 review seat — deck50 (Erosion), GUIDELESS first seating

Seat record 1/6. Deck50 played all six games with **no** `deck50_strategy.txt` loaded;
this file records the evidence and `wave81/deck50/strategy.txt` is the first guide.
Seat windows: 211 (ask 203, bottom 3, discard 5) across 6 games — 18 / 16 / 17 / 22 / 14 / 124.
0 async drops, 0 `reask_reason`, 1 heuristic fallback, 9 `plan_line_missing` (4.3%, all
`unlabelled_plan`), 12 hold events, 32 `cast_decision_reopened_new_stack`. Window counts
are not comparable to any earlier seat file: this deck has no prior seating.

Files are named by opponent below; full names are
`1789690304-ai_baka_deck50-0x5645ae3e83b0-vs-ai_baka_deck152.jsonl` (vs152),
`…310-…-vs-ai_baka_deck123` (vs123), `…320-…-vs-ai_baka_deck130` (vs130),
`…322-…-vs-ai_baka_deck162` (vs162), `…329-…-vs-ai_baka_deck146` (vs146),
`…337-…-vs-ai_baka_deck125` (vs125).

## 1. Game by game, and the decisions that decided them

**vs146 — LOSS, turn 10, life -3.** Mulligan at seq2 correct (coverage line read "would
not cover any spell in it"). **seq4 bottomed an Island** out of a 3-land hand holding
Forced Fruition {4}{u}{u} (mv 6), Howling Mine, Evacuation and Damnation — the coverage
line named only Howling Mine, so the six-drop was the card to send. **seq10 (T6) answered
`Play no land right now`** on a standalone land window with four lands in hand at 19 life
(recovered at seq11 in main 2 — the drop was not lost, the reasoning was). **seq12 (T8)
played an Island with a Swamp in hand and one black source down**; that choice is the
loss: at seq13 (T10, 7 life, 4 attackers, `CRACK-BACK … that would KILL you`) Damnation
read `[cannot pay now: needs {2}{b}{b}, your 4 untapped sources cannot pay it]` and the
only castable row was Glimpse the Unthinkable, which it took. A Swamp at seq12 makes
Damnation live at seq13.

**vs152 — LOSS, turn 14, life -7.** seq2 mulligan correct. **seq4 bottomed Glimpse the
Unthinkable out of a 5-land / 2-spell hand** — the one cheap proactive spell went, five
lands stayed. seq13 (T10, 15 life, 3 creatures) cast Evacuation tapping out; it bought a
turn that produced nothing. By seq18 (T14, 11 life, `CRACK-BACK … 5 … would KILL you`) the
menu held only Memory Erosion: no answer existed. No sweeper was ever castable in this
game — W/U only, never a second colour pair for {2}{B}{B}.

**vs130 — LOSS, turn 17, life -3.** Kept a 5-land hand (correct). Moat and Wrath of God sat
in hand from seq12 (T11) to the end on one white source; seq14 (T13) correctly took the
Plains. seq15 (T15, 9 life) Evacuation, seq17/seq18 (T17, 3 life, Rorix Bladewing, lethal
printed) nothing castable — and at seq17 the reply called **Forced Fruition "detrimental"**
and declined it at 3 life. Whether or not it was the right row there, the reading is
backwards: an opponent drawing seven per spell is an opponent being milled seven.

**vs162 — LOSS, turn 14, life 0.** The clearest single misplay of the six games.
seq15 (T13, draw step) the seat held priority with the written plan *"cast Moat in main
phase, play land"*. seq18 (T13, main 1, 10 life, 6 sources, `Cast Moat` row present)
played a Swamp — correct sequencing. **seq19 (T13, main 1, 7 sources, `Cast Moat` row
still on the menu, Damnation now `[castable now]`) answered `Hold priority` with the
reply "there is no viable play this turn".** It abandoned its own one-window-old plan with
the named row printed in front of it. Moat would not have stopped Ob Nixilis's pings, but
the decision class — "nothing to do" with a castable permanent answer on screen — is the
one the guide has to close.

**vs123 — LOSS, turn 15, life -24.** seq12 (T11) cast Wrath of God on a `destroys 4 of
their creatures` row: correct, and the best decision the seat made all corpus. seq13 (T13)
declined Moat for Glimpse against a shrouded, hasted Bloodline Keeper under Intruder
Alarm. seq15/seq16 (T15) the prompt printed `CRACK-BACK NEXT TURN: 20 of their creatures …
for up to 41 - you would be at -24; that would KILL you` and the seat took `Play Island`
then `Cast Traumatize`. **Verdict: no window.** All 19 Vampire tokens and the Keeper carry
`[flying]`, so the Moat rows on that menu stop nothing; Damnation was uncastable at {B} 1
and no Wrath was in hand. This game is the deck's ceiling against a hasted, shrouded
token engine, not a guide failure.

**vs125 — WIN, turn 43, life 7, opponent decked at `Opponent library: 0`.** Mulligan
(seq2, coverage line negative) and bottom (seq4, Evacuation, the uncastable five-drop)
both right. Dream Fracture 6 offers / 6 takes, countering Sphinx's Revelation (seq25),
Cancel (seq32, seq155) and a Dream Fracture (seq100) — no misfires. Two live errors
survived only because the opponent was creature-light: **seq88 and seq89 (T36) cast
Damnation twice on rows reading `destroys 0 of their creatures (0 without a restriction
against attacking), 0 of yours`**, burning two sweepers for nothing; seq89's reply reads
*"Cast Damnation to mill 2"*, a false card belief — Memory Erosion triggers on the
OPPONENT casting a spell, never on the seat's own casts. And **seq91 (T36, `Your library:
14` vs `Opponent library: 18`) and seq104 (T38, `Your library: 10` vs `Opponent library:
31`) cast a second and third Howling Mine while behind in the library race.** The game was
won at `Your library: 1` against `Opponent library: 0` (seq170) — a one-card margin that
those two rows very nearly gave away. Cleanup discards (seq44, 47, 53, 75, 164) were
correct throughout: spare lands first, a redundant sweeper second.

**Offers / takes per card, seat-wide** (field: `options_text` rows beginning `Cast <name> `,
unit: row-instance per window; distinct (game, turn) beside it):

| card | offers | takes | distinct (game,turn) offered / taken |
|---|---|---|---|
| Moat | 20 | 1 | 8 / 1 |
| Wrath of God | 41 | 2 | 14 / 2 |
| Damnation | 36 | 2 | 13 / 1 |
| Evacuation | 78 | 4 | 17 / 4 |
| Dream Fracture | 6 | 6 | 6 / 6 |
| Glimpse the Unthinkable | 7 | 6 | 6 / 6 |
| Tome Scour | 14 | 10 | 10 / 10 |
| Traumatize | 7 | 4 | 5 / 4 |
| Memory Erosion | 17 | 8 | 12 / 8 |
| Howling Mine | 25 | 8 | 14 / 8 |
| Forced Fruition | 3 | 1 | 2 / 1 |

Moat at 1 take in 8 distinct offered turns is the headline: the deck's only permanent
answer is the row the guideless seat declined most.

## 2. Engine / interface / card items

**HIGH — a reply whose answer carries the `PLAN:` label instead of the action label is
refused, and the fallback fires with no re-ask.** The corpus's one and only
`unparsed_reply`. File `1789690337-ai_baka_deck50-0x55f86ff817d0-vs-ai_baka_deck125.jsonl`,
seq 41 (window_seq 39, T28, Main phase 1, 4 options, `reasoning_chars` 7505,
`protocol_deviation` compliant, `reask_reason` null, one attempt).
The reply, verbatim and complete:

```
\n\nPLAN: 4 (Cast nothing right now)
```

Judgment, against the standing rule that the parser reads the answer wherever it
unambiguously is and rejects only an answer that PRECEDES the plan: **the robust parser
should have read this.** There is no plan for the answer to precede — the reply contains
one line and one number; `4` and the parenthesised short name match row 4 of this menu
exactly and no other row; there is no competing integer in the reply. The recovery record
(seq 42) shows `answer_label_absent: false`, `executed_by: heuristic`,
`executed_text: "cast nothing"` — the label-less-answer path did not engage because a
label WAS present, just the wrong one, so the reply fell past every robustness rung into
Baka. Here the heuristic happened to land on the same row the model asked for; on a live
menu it would not. Fix direction: when no action-label line exists and the sole `PLAN:`
line's content is exactly `<row number> (<that row's short name>)` matching one row, read
it as the answer; failing that, re-ask rather than fall back — the seat had 891 s of
deadline left (`deadline_pct` 8.9).

**MED — P5: the collapsed repeated-damage line drops the running life total and the
per-line amounts.** Same directory, `…322-…-vs-ai_baka_deck162.jsonl`, seq 18 and 19
(T13), rendered in 8 prompts. The log line:

```
Ob Nixilis, the Hate-Twisted -> 1 damage to you (now 17) [x8 - 8 lines of this shape in this batch; only the numbers in them differ]
```

The seat's life after the batch is 10, and the collapse prints only the first line's
`(now 17)`. Worse, "only the numbers in them differ" makes the *damage* amounts unknown
too, so the batch cannot be reconstructed: the reader cannot tell 8×1 from any other
split summing to 7. The old register printed eight lines each carrying its own new total,
ending at `(now 10)`. The `Your life: 10` header carries the truth, so this is a
reconstruction defect rather than a false surface — but the fix is one clause: end the
collapse with the batch's final total and state the per-line value when it is constant
(`8 x 1 damage, you 17 -> 10`). This is the only P5 defect the seat found.

**MED — `check-reply-instructions.py` gives a staged guide ZERO coverage.** `GUIDE_SUFFIX`
is `("_strategy.txt",)`, so `python3 tools/check-reply-instructions.py src
strategy-design/wave81/deck<N>` reports `0 guide(s)` for a seat file named `strategy.txt`
and exits 0 whatever the guide contains — the briefed verification passes vacuously. Repro:
run it against this seat's directory; then run it against a copy renamed
`deck50_strategy.txt` and watch the guide count go 0 -> 1. Fix: match `strategy.txt` as
well as `*_strategy.txt`, or have the brief name the staged file `deck<N>_strategy.txt`.
(This seat re-ran the check against a renamed copy: also clean.)

**LOW — P9 folding is partial, and a guide rule keyed to "the land row on the casting
menu" would miss a third of the drops.** Seat-wide: 67 windows carried a `Play <land>`
row; 35 of them were casting menus with the land folded in, **32 were standalone land-only
menus** (e.g. vs146 seq10/seq11, vs130 seq14/seq16, vs152 seq19, vs125 seq26). Both shapes
are live in the same game. Not a defect — a fact the next guide-writing seat needs.

**Clean:** no `WAGIC addToGame REFUSED a garbaged element` in any `.stderr` in the corpus
directory (bug-list #11/#12 guard silent). No `order` or `reveal` window reached this seat,
so P10's whole-order answer is UNTESTED-AT-THIS-SEAT. No blockers or attackers window
reached this seat: the deck has 0 creatures, which is unreachable-in-deck, not a gap.

## 3. P9 and P5 counts

**P9 (land drop).** Windows carrying a land row: 67. Land row taken: 51. **Turns on which a
land drop was available and no land was played: 0 of 47** (per game: vs152 0/7, vs123 0/5,
vs130 0/8, vs162 0/7, vs146 0/5, vs125 0/19) — the 16 windows where a spell was taken over
the land row were all re-put in the same turn and the drop was made. **Turns on which the
land row was taken ahead of a spell that should have gone first: 0** — the land row spends
no mana and the casting list is re-put with the new source counted, so no take of it cost a
cast. One window declined the drop outright with lands in hand (vs146 seq10, `Play no land
right now`, T6, 19 life) and recovered it at seq11 in main 2. Against the owner's revert
criterion, this seat shows no behaviour degraded by P9 and one seat-level benefit: on the
folded menus the drop and the cast were priced against each other in one window
(vs162 seq18 played the Swamp that made Damnation `[castable now]` at seq19).

**P5 (compacted GAME LOG).** Decisions where the compacted log omitted or mis-stated a fact
the seat needed: **1** (the collapsed damage batch above, rendered in 8 prompts of one
game). Everything else the seat needed was carried: mill events are enumerated per card
(`opp milled <name>` per card, ten lines for a Glimpse), counter chains name both spells
and both draws, graveyard-to-library recursion is narrated (`opp shuffled their graveyard
(28 cards) into their library with Elixir of Immortality`, vs125 T25), and the two clocks
this deck lives on print in the header on 199 of 211 windows (the 12 without are the
pregame mulligan and bottom asks, which do not need them).

## 4. Guide verdict: AUTHORED (first guide)

`wave81/deck50/strategy.txt`, 14,671 bytes (ceiling 20,000).
Verifications: `stat -c %s` = 14671; `check-reply-instructions.py src
strategy-design/wave81/deck50` = OK, exit 0 (and OK, exit 0, `1 guide(s)`, against a copy
renamed `deck50_strategy.txt` — see the MED item above, the briefed form is vacuous);
A332 grep + `you (declined|took|cast|passed) [0-9]` = 0; frequency-adjective grep = 0;
A334 concessive grep (`even when|even if|regardless of|no matter`) = 0; no reply label
(`CHOICE:|PUT:|ATTACK:|BLOCKS:|ORDER:|PLAN:`) appears anywhere in the guide.
Heading audit (A333): every heading is a noun phrase naming an ask or a menu; none uses
SEND, BLOCK, ATTACK, KEEP, BOTTOM, CAST or PUT as its verb.

Every rule and the decision that paid for it:

| rule | paying evidence |
|---|---|
| Thesis: the opponent's library is the only win route; their life total is not a target | the one win (vs125) came at `Opponent library: 0` with the opponent on 59 life |
| Read `Opponent library:` and `Your library:` as a race | vs125 seq91 / seq104 cast Howling Mine at 14-vs-18 and 10-vs-31; the win landed at 1-vs-0 (seq170) |
| Mulligan: answer the `Playing every land in this hand would …` coverage line, keep on "would cover the cost of:", mulligan on "would not cover any spell in it" | 6/6 of the seat's own mulligan/keep answers already match it (vs152 seq2/3, vs146 seq2/3, vs125 seq2/3, vs123 seq2, vs130 seq2, vs162 seq2) — codified as the rule so it survives a bad hand |
| Bottom the highest-mv spell the coverage line does not name, before any land | vs146 seq4 (bottomed a land, kept a mv-6 enchantment, then flooded out and died T10); vs152 seq4 (bottomed its only cheap spell, kept 5 lands); vs125 seq4 is the correct shape |
| Land row taken every window it appears; `Play no land right now` only with no Play row wanted | vs146 seq10 |
| Which land: the colour that turns a `[cannot pay now: needs {2}{b}{b}` sweeper castable, above raw count | vs146 seq12 -> seq13 (Island over Swamp cost the Damnation at 7 life); vs162 seq18 -> seq19 is the correct shape |
| Cast list entry 1: a sweeper row reading `destroys N of their creatures` with N>=2 outranks every mill row | vs123 seq12 (taken, the seat's best decision); vs152 seq18 and vs146 seq13 are the windows where it was already too late |
| Cast list entry 2: take the `Cast Moat` row the first window it appears, empty board included; the flying exception stated flat | vs162 seq18/seq19 (declined with the row printed and the seat's own plan naming it); Moat 1 take in 20 offers; vs123 seq15/seq16 supplies the flying exception, not a counter-example |
| Cast list refusal: never a sweeper row reading `destroys 0 of their creatures (0 without a restriction against attacking), 0 of yours` | vs125 seq88 and seq89 — two Damnations burned in one turn |
| Howling Mine gated on `Your library:` > `Opponent library:` | vs125 seq91, seq104 |
| Forced Fruition is a mill accelerant | vs130 seq17 ("Forced Fruition is detrimental") |
| Memory Erosion triggers only on the OPPONENT's casts | vs125 seq89 ("Cast Damnation to mill 2") |
| Traumatize sized against the printed library number, taken while it is large | vs123 seq16 (11 cards for a whole turn at a lethal board); vs125 seq133 (half of 54) |
| Target ask: always the opponent, never yourself | 10/10 mill target asks already correct — the rule exists because the self-target row is on every one of those menus |
| Hold rows only when every row is refused for a stated reason; a plan naming a printed row is taken, not re-deliberated | vs162 seq15 -> seq19 |
| Cleanup discard order: spare land, duplicate sweeper, highest unused mv | vs125 seq44, 47, 53, 75, 164 — all correct, codified so it holds |
| Combat asks: no creatures, decline; combat-phase windows are still casting windows | vs125 seq119-seq122 |

Predictions for the next seating, each with its refuting number:
- Moat: offers stay in the 10-25 band, takes go from 1/20 to at least half of the distinct
  turns offered where any non-flying creature is on their battlefield line. A hit is a
  break only if a `Cast Moat` row was declined on a board whose opponent battlefield line
  carried a creature without `[flying]`.
- `destroys 0` sweeper rows: taken 0 times. A hit is a break at 1.
- Howling Mine rows taken while `Your library:` <= `Opponent library:`: 0. A hit is a break
  at 1.
- Land played on every turn a Play row appears: 47/47 held this corpus; a break is any turn
  with a Play row and no land played.
- Bottom ask: the card named is a spell absent from the coverage line whenever one exists.
  Three samples cannot discriminate the order — report offers/ships and withhold the
  verdict, UNTESTED-AT-THIS-SEAT until six.

Removal conditions: the Memory Erosion trigger sentence and the Forced Fruition sentence
are information-supplying — delete them the corpus after the row itself prints the
"triggers on the opponent's casts" / "they draw, you mill" fact. The `destroys 0` refusal
and the Howling Mine library gate are belief-correcting — they stay until BOTH the render
carries the corrective clause AND two corpora pass with zero breaks under the triggering
shape. The Moat entry has no removal condition.

## 5. Proposals

None. The general guide is not loaded at runtime and nothing this seat saw is a
general-pool fact; the strategy-writing skill produced a guide that passed every gate it
names, so a skill amendment here would restate A332/A333/A334. The one tooling gap
(`GUIDE_SUFFIX`) is filed as a MED engine/interface item above, where the synthesis seat's
merged bug list can pick it up.

## What this seat did NOT check

The opponents' own translogs beyond the joins quoted here; the wave-81 lane files DI/DJ/DK/
DL/DM against their counters (this seat's evidence touched none of them beyond the hold
events it reports); the 12 hold events individually (counts only); P10's order seam and the
reveal seam (no such window reached this seat); attackers/blockers rendering (unreachable
for a 0-creature deck); `x_cast_row_refusal_markers` / `x_sweep_roster_markers` (0 at this
seat, no X spell in the deck); latency and token-budget behaviour; and whether the deck50
decklist's 24-land, three-colour manabase is itself the loss cause — five of six losses
trace to a missing second source of a sweeper's colour, which is a DECK question this seat
has no mandate to answer.
