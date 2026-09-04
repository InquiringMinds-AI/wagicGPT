# wave-59 per-deck review — deck125 "Revelation Fracture" (Modern UW Control)

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-130044/`, binary master 5039427da.
Six games, all natural. Record **4-2**. Seat files are
`1788544*-ai_baka_deck125-*-vs-ai_baka_<opp>.jsonl`; seqs below are that seat's unless noted.
948 decisions, 3 fallbacks (2 `unparsed_reply` vs deck126, 1 vs deck130), 0 timeouts,
0 `wall_miss`, 804 `hold_windows_skipped`, 82 `identical_ask_answers_reserved`,
`identical_option_asks_resolved` = 0 in all six.

## 1. Game by game

| opp | result | turns | final life | deciding decisions |
|---|---|---|---|---|
| 152 | **LOSS** | 27 | -4 / 18 | seq18 T14 + seq19 T16: Supreme Verdict cast twice, each row reading `destroys 1 of their creature`; seq21-25: held priority through T19-T23 while a Ranger-Class-pumped Brutal Cathar/Moonrage Brute went 3→5→6 damage a swing. Staff of Nin, the deck's only clock, did not land until seq27 T24. |
| 162 | **LOSS** | 24 | -31 / 12 | seq110/111 T23: Sphinx's Revelation X=9 taken off a row that reads **"NET 0 life for this cast"**. Hand became 18; seq112 forced an 11-card cleanup discard into two Liliana's Caress; 11 × 2 × 2 = 44 life, 13 → -31 exactly. See HIGH-1. |
| 123 | WIN | 64 | 66 / -9 | seq26 T11 + seq28 T13 again spent Verdicts on `destroys 1`; recovered because Elixir of Immortality (seq39 T17) recycled the graveyard — including the Emrakul discarded at seq125 T59 — and Emrakul was re-drawn and cast at seq138 T63. |
| 146 | WIN | 47 | 29 / -14 | Sweepers used on 2-creature boards (seq20 T8, seq23 T12, seq35 T18, seq69 T28) — guide-compliant; Emrakul seq112 T46 closed it. |
| 126 | WIN | 87 | 95 / 3 | Grind. seq73 T21 Verdict for 7. Two parser fallbacks (seq246, seq354, HIGH-3). |
| 130 | WIN | 70 | 62 / 3 | Grind; seq113 model emitted degenerate tokens on a 3-row land ask → `unparsed_reply` + a `recovery` record at seq114 (model-side, not engine). |

Both losses are the fast games; the deck wins every game it is allowed to reach turn 45.
Hands were real; no loss is attributable to a mulligan.

## 2. Engine / interface / card items

### HIGH-1 — "NET 0 life for this cast" is a false verdict: no DISCARD-punisher lens
File `1788544862-ai_baka_deck125-...-vs-ai_baka_deck162.jsonl`, seq111, T23, my_life 13.
Rendered:
```
1. X = 9 {X pricing: X=9 - you gain 9 life and draw 9 cards; the opponent's Underworld Dreams
   punishes every draw, so those draws cost you 9 life - NET 0 life for this cast}
   [<- largest affordable X - X=9 gains 9 life and draws 9 cards; no listed X does more]
```
The opponent's board (same prompt) lists **Liliana's Caress #1 and #2**
(`mtg.txt:67351 auto=@discarded(*|opponenthand):life:-2 opponent`, verified) plus
Teferi's Puzzle Box and Howling Mine. The cast's *guaranteed* consequence is a hand of 18
against a maximum hand size of 7, i.e. an 11-card cleanup discard, i.e. 11 × 2 life × 2 copies
= **44 life**. Actual: 13 → -31 at seq113 gameend. The engine has a DRAW PUNISHERS block and a
DRAW FORECAST line and neither the block, the forecast, nor the NET arithmetic knows discards
exist. Under the trust doctrine the model is instructed to believe "NET 0" and did.
Two defects, separable:
 (a) no DISCARD PUNISHERS block — the cleanup ask itself (seq112) prints the correct CR 514.1
     math ("your hand has 18 cards and your maximum hand size is 7, so you must discard exactly
     11") and *still* says nothing about the 44 life it is about to cost;
 (b) the X-pricing NET line scopes "for this cast" over the draw punisher only. Either widen it
     to include the forced cleanup discard, or stop asserting a NET at all when a discard
     punisher is on the battlefield — a true number in the wrong scope is a lie.
The `[<- largest affordable X ... no listed X does more]` steer compounded it: the row actively
recommended the lethal X.

### HIGH-2 — sweeper rows price a COUNT but never name the creatures; Path rows name theirs
180 `{right now: destroys/exiles N of their creature...}` renders across this deck's six games,
**0** carry a `{removes: ...}` list; every Path to Exile row does
(e.g. deck152 seq6: `{removes: Katilda, Dawnhart Prime}`).
Repro, `...vs-ai_baka_deck152.jsonl` seq18, T14:
```
1. Cast Supreme Verdict {1}{u}{w}{w} {right now: destroys 1 of their creature (1 without a
   restriction against attacking), 0 of yours} ...
```
The board line 40 lines above reads `Luminarch Aspirant {1}{w} (2/2) (printed 1/1)`.
deck125's guide (lines 362-379) branches on exactly that: *"exactly 1 creature, and that
creature is tagged [defender], or shows printed power 0 or 1 ... cast NOTHING"*. To apply its own
rule the model must carry a name and a printed P/T across the prompt from the board list to the
option row. It failed to do so 5 times in this corpus (deck152 seq18/seq19, deck123 seq26/seq28,
deck123 seq46 Final Judgment) — twice consecutively in a game it then lost by 4 life. This is a
PERCEPTION item, not a guide item: the deciding fact does not ride the option. Fix: emit the
same `{removes: ...}` / `{exiles: ...}` roster the Path row already builds, with printed P/T and
keywords, on every sweeper row.

### HIGH-3 — `PUT:` parser refuses a list containing a duplicate index; no partial credit
`...vs-ai_baka_deck126.jsonl` seq246, T54, a **16-of-23** cleanup discard:
```
REPLY  PUT: 8, 11, 22, 19, 20, 16, 21, 23, 12, 10, 4, 5, 13, 17, 18, 19   -> fallback=unparsed_reply
```
16 numbers, but `19` appears twice, so only 15 distinct → whole reply dropped, heuristic
discarded for it. Same shape at seq354, T78, on a 2-card ask: `PUT: 4, 4`.
These are the only two parse failures the seat produced; both are duplicates, neither is a
formatting error. Two independent fixes, both cheap:
 (a) de-dupe and re-ask only for the shortfall (or fill from the model's own PLAN order) rather
     than discarding a 15/16-correct answer;
 (b) **invert the ask when the discard count exceeds the keep count** — "name the 7 you KEEP"
     instead of "name the 16 you discard". This deck reached that regime routinely (52 cleanup
     discards across six games, five of them ≥ 11 cards). The K9 name-gloss fix is confirmed
     working: 7 replies of the `PUT: 7 (Island)` / `PUT: 9, 10, 1 (Sphinx's Revelation, ...)`
     shape all parsed. known-bugs #9 discharged.

### MED-1 — "Cast nothing right now" is strictly dominated by the HOLD row and wins 282/282
Every one of the 282 `Cast nothing right now` takes in this corpus had a HOLD row on the same
menu, whose own text is *"pass now, and do not ask me again unless the board changes (any change
re-opens this window; you give up no cast)"*. For a declining player HOLD gives up nothing that
`Cast nothing` keeps, and it collapses the rest of the turn's identical windows. The model never
once preferred it when both were offered. Repro, `...vs-ai_baka_deck162.jsonl` seq4-seq10:
seven consecutive `Cast nothing right now` in turn 4 alone, one per phase, ~20 s each.
Corpus cost: 948 decisions, **467 (49%) produced no action, consuming 108 of 194 minutes** of
inference. Fix candidates: drop the bare decline when a HOLD row is offered, or fold them
(`Cast nothing right now — and do not re-ask this turn unless the board changes`).

### MED-2 — `CRACK-BACK NEXT TURN ... for up to N` is not a ceiling
`...vs-ai_baka_deck152.jsonl` seq21, T18:
```
CRACK-BACK NEXT TURN: 1 of their creatures will be able to attack (tapped ones untap first),
for up to 2 - you would be at 18
```
The attacker was Brutal Cathar under a level-2 Ranger Class
(`borderline.txt:89050 auto=@each my blockers restriction{compare(hascntlevel)~morethan~1}: ...
target(creature[attacking]|battlefield) counter(1/1)`, verified) and daybound. Actual damage on
the following swings: 3, then 5 (transformed to a 4/4 Moonrage Brute), then 6. Life reached 11,
not 18. Three of 34 measurable crack-back forecasts in this seat were exceeded (deck152 seq21
and seq28, deck146 seq30, the last by 3). The forecast is a static power sum; the phrase
"up to" asserts a maximum it does not compute. Minimal fix: say "at least N" and append a
qualifier when the opponent controls an attack-trigger permanent or a transforming creature —
do not delete the number.

### MED-3 — the crack-back line is absent from every opponent's-turn window
Same file, seq22-seq25 (T19-T22 upkeep): the model held priority through four consecutive
opponent turns and the last crack-back number it ever read was seq21's stale "you would be at
18", while life went 20 → 11. Whatever the general rule for own-turn vs opponent-turn rendering
(known-bugs #8 / general R319), a seat that HOLDs sees no refreshed damage forecast at all.

### LOW-1 — Lightmine Field rows still print damage without a kill count
5 renders of `{right now: they control 1 creature able to attack - deals 1 to each if all 1
attack}` (e.g. deck123 seq70 region). The damage number is now there — wave-58's MED item is now only
the "how many of those creatures die at that damage" half. Unpaid for by any decision here;
this deck declined every one of those rows correctly.

## 3. Guide verdict: **KEEP as is**

`bin/Res/ai/baka/deck125_strategy.txt`, 70,801 bytes, 829 lines — at the top of the 41-71 KB
band; there is no room to add and nothing here that earns a swap.

Why KEEP is the right verdict:
- 4-2, and the guide's core teach (grind, Elixir recursion, Emrakul as the only clock) executed:
  Emrakul closed two of the four wins (deck146 seq112, deck123 seq138) and Elixir recycled a
  discarded Emrakul back into a win (deck123 seq125 → seq138), which is the exact line the guide
  writes.
- The one repeated *misplay* — five sweepers spent on `destroys 1` boards — is a violation of a
  rule the guide **already** states with precision, at lines 362-379, with the printed-power
  branches, the "cast NOTHING" hard line, and two named past failures. Adding more prose against
  a rule the model already has and did not apply loses to the surface (perception-routing
  doctrine); the missing input is the creature identity on the sweeper row — HIGH-2 — not a
  teach.
- Neither loss is a guide failure. vs 162 the deck has no answer to Underworld Dreams /
  Liliana's Caress / Teferi's Puzzle Box in 60 cards and the render told it the lethal line was
  free (HIGH-1). vs 152 the deck drew its only clock on turn 24 and the damage forecast
  under-reported the race (MED-2).

No `strategy.txt`, no `general-proposals.md`, no `skill-proposals.md`: everything this seat
found routes to the engine/render layer, which is the core loop's own lane.

## 4. What I did NOT check
- The opponents' seat logs, except for board facts visible in deck125's own prompts.
- The other six decks; every count above is deck125's six games only.
- Whether HIGH-1's discard-punisher gap exists for the *opponent's* discards (Liliana's Caress
  is symmetric in text but the primitive's `opponent` target makes it one-sided — I did not
  trace the mirrored case).
- Whether the wave-59 lane fixes (I/J/H/K) passed as a set — that is the engine seat's
  adjudication; I only confirm K9 (PUT gloss) parsed here and that the identical-ask latch
  reserved 82 answers.
- Any `SEGV`/abort/hang signature: none of the six `.stderr` files were opened.
