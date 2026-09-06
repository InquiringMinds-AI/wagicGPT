# Wave-67 deck 130 review — Budde's Ponza (mono-red land destruction + burn)

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-102638/`, 6 seat files
`*-ai_baka_deck130-*-vs-*.jsonl`. Record: **2/6** (beat 152, 123; lost to 126, 125, 162, 146).
466 decisions, 453 model round trips.

## 0. Protocol census (the lane-flagged questions this seat can answer)

- **`unparsed_reply` = 0** (wave 66 carried 16 of the 83 in this seat). AV's prediction met here.
  Total fallbacks in the seat: **1** (`wall_miss_unrecorded_wall`, below). No `empty_reply`, no
  `bad_reply`, no `transport_error` retries, no `async_drops`, no crash/hang.
- **Reply shape: 448/452 answer-first (99%)**, up from 73% corpus-wide last wave. The four
  reasoning-first replies (a lead sentence, coded line later) all parsed. Leading labels seen:
  `CHOICE` 431, `ATTACK` 15, `PUT` 1, `BLOCKS` 1 — **no new heading was transcribed as a label**;
  no `ANSWER:`/`So`/`CORRECTION:` line appeared at all in this seat, and the `So PUT:` discard
  shape AV did not wire never occurred (1 discard record, plain `PUT: 1, 2`).
- **`post_answer_overrun` 387/452 = 86%** in this seat (corpus 71%). The overrun is not noise: it
  is where the model reverses itself (below).
- First-wins fired 22 times (`later_answer_ignored`), 2 with `decision_reversed_in_prose`.
- **UNTESTED in this seat** (no window arose, do not infer): `{library:}` X-ceiling rows (deck 130
  has no drawing X spell — 0 prompts carry `{library:`), reveals (`reveal_fallback_pick`,
  `reveal_stall_forced`, `reveal_wait_unexplained_secs` all absent), stated-stop repeats
  (`repeat_clamped_to_own_stop`/`repeat_past_stop` absent), `label_missing_salvaged`/`_reask`
  (absent), I9a repeat-pay bands (absent). `{reserve:` appeared in 2 prompts and never latched.
  The graveyard line (AY I8) was present in 447 prompts and **was read**: the whole deck-123 win
  is Hammer-of-Bogardan graveyard recursion (seq 60/66/68/70/72), so it fed real decisions.

## 1. Game by game

**vs 126 — LOSS, 0/34, turn 22.** Opponent assembled Exquisite Blood + Sanguine Bond behind
four defenders. Deciding decisions, all turn 10-12: seq 21 announced Starstorm, seq 22 chose
"Cast Card Normally", **seq 23 declined at the X window — six lands and the Talisman were already
tapped and the mana was lost** (HIGH-1). seq 24 then attacked A1-A4 into two Pride Guardians whose
own tags read "(blocking trigger: they gain 3)" — the reply's PLAN says "I must NOT attack into
the defenders... I will pass combat" and the coded line sent all four anyway (HIGH-3); opponent
18 -> 24. seq 27-30 repeated the whole announce/decline cycle on turn 12, burning that turn's
7 mana too. Two of the deck's eight live turns produced nothing.

**vs 125 — LOSS, 0/23, turn 47.** Land destruction landed (5 lands killed by turn 16) but the
deck never fielded a threat: Siege-Gang traded away, and Rorix, cast turn 18 into four untapped
opponent lands with an Island up, was countered by Cancel (seq 40 narration). From turn 20 it had
no creature for 27 turns and won only the Hammer race it could not win (3/turn against a deck
gaining more). Real variance plus a marginal play-around-Cancel miss; not a guide failure.
seq 31/33 (turn 16) are two more declines on prompts carrying HIGH-2's false hand tag.

**vs 162 — LOSS, 0/6, turn 20, and the closest game of the set.** Opponent at 5-6 life for four
turns. seq 70 (turn 18): "CHOICE: 2 (Cast Spark Spray) / PLAN: Kill Ob Nixilis with Spark Spray.
Attack with Rorix Bladewing for lethal." The single Mountain was tapped, then **seq 71 declined at
the follow-up menu — the {r} was stranded** (HIGH-1) and seq 72/73 read the resulting board as
"no legal plays available" and passed with {r} floating. Underworld Dreams + Ob Nixilis then
killed it from 7 through its own draw step. seq 78-80 at 1-3 life had only cycling rows (each a
draw = more triggers) and Hold — genuinely dead by then, not a misplay.
This seat also holds the corpus MAX latency: **seq 20, `wall_miss`, `latency_ms` 900035,
`deadline_pct` 100.0, `transport=curl=28,http=0,empty=1,connect_ms=20000,phase=wall`** — the
deadline was not "passed", it was *consumed*: the wall-phase request itself ran the full 900 s and
curl-28'd, leaving zero retry budget, exactly as lane AX's arithmetic predicts. Recovery record
seq 21 followed and the game continued.

**vs 146 — LOSS, 0/14, turn 33.** Ponza's plan does not beat a five-land deck with Lolth, a
dungeon and Soul Shatter: the deck spent its entire hand on land destruction (11 cards in
graveyard, 1 card in hand by turn 32, seq 50) while the opponent replaced every removed piece.
Starstorm at seq 47-49 (X=3, annotated "same kills as X=7, for 4 less mana" — the ceiling render
worked) swept four creatures for value. Matchup/deck-construction outcome, not a decision failure.

**vs 152 — WIN, 5/0, turn 22.** Starstorm X=1 at seq 19-21 killed Katilda + two Elite Spellbinders
for three mana; a second Starstorm cast **from exile** at seq 27-28 (X=2) held the board; Rorix
from exile at seq 42 closed it. Every X choice took the priced minimum. Clean execution.

**WIN vs 123, 16/-1, turn 28.** The guide's Hammer plan executed exactly: upkeep recursion at
seq 60/66/68, then 3 to the face every turn (seq 57/59/64/71/73) taking 18 -> -1, with Molten
Rain/Lay Waste holding the opponent off blue mana. This is the deck working as written.

## 2. Engine / interface items

**HIGH-1 — Declining at a post-announcement menu strands the mana already paid; the decline row
does not say so.** The engine taps and pools the full cost at *announcement*, before the
alternative-cost / X menu is asked. Taking that menu's Decline returns the card to hand and
**leaves the paid mana floating until it empties at end of phase, unrefunded**.
Repro A: `1788708405-ai_baka_deck130-0x560d564dc440-vs-ai_baka_deck126.jsonl` seq 21-24, turn 10.
Narration at seq 26 reads verbatim:
`- Paid {r}{r}{x} for Starstorm with Mountain #1; Mountain #2; Talisman of Impulse; Mountain #3; Mountain #4; Mountain #5`
`- You chose Cast Card Normally for Starstorm`
`- Phase: Combat begins`
`- You chose Decline - do not cast this after all (the announcement is cancelled and the card stays in your hand) for Starstorm`
Six sources gone, Starstorm back in hand, Talisman having dealt its 1 damage for nothing.
Repro B: same file seq 27-31, turn 12 — 7 mana burned the same way, and the *second* announcement
at seq 29 was consumed, narrated `- Paid {r}{r}{x} for Starstorm from mana already floating`, and
then vanished: no X window followed, Starstorm was still in hand at seq 30, the pool was still
`{r}{r}{r}{r}{r}{r}{c}`, and the Starstorm row was gone from the seq-30 menu. That narration line
is false — nothing was paid and nothing was cast.
Repro C: `...-0x5644fdfb91d0-vs-ai_baka_deck162.jsonl` seq 70-73, turn 18 — one mana, the deck's
only play, its own stated lethal line, lost the same way.
Seven declines in this seat, four of them on a post-payment menu. Two fixes are separable: roll
the payment back on decline (the row already promises "the announcement is cancelled"), and until
then make the row state the cost — a silent omission here is worse than wrong text, and the model
confabulated a "0 untapped / 7 floating" dead end out of it in all three repros.

**HIGH-2 — The hand's `[cannot pay now: needs N mana, you have 0 untapped sources]` tag ignores the
floating pool, and contradicts the option list in the same prompt.** With mana in the pool, the
CURRENT SITUATION block says both `Already in pool: {r}{r}{r}{r}{r}{r}{c} (7 mana ALREADY produced
and floating right now ... spend the floating mana first)` **and** `Lay Waste {3}{r} [sorcery]
[cannot pay now: needs 4 mana, you have 0 untapped sources]` — while row 2 of the same menu is
`Cast Lay Waste {3}{r}`. The trust doctrine makes the tag win: at seq 28 the model's plan reads
"I cannot cast Lay Waste yet (need 4 mana untapped, I have 0 untapped but 7 floating)", and at
seq 30 "I have no mana to cast spells anyway". It then held priority through its own turn.
Census: 82 prompts in this seat carry a floating pool, **55 of them also carry a `cannot pay now`
tag**; in **6** of them a tagged card is simultaneously an offered `Cast <name>` row (corpus-wide
7 — six are this seat, because a mono-red deck taps out for X). Repros: 126 file seq 29 (Lay
Waste, Starstorm, Spark Spray all tagged and all offered), seq 30; 125 file seq 32, 34; 162 file
seq 67, 72. The tag is true about *sources* and false about *payability*, i.e. true in the wrong
scope. It should count the pool, or say "0 untapped sources (7 floating, which pays this)".

**HIGH-3 — `decision_reversed_in_prose` is not wired at the ATTACKERS seam.** 74 attackers records
corpus-wide, **0** carry a reversal note. `126 file seq 24`: reply is
`ATTACK: A1, A2, A3, A4` followed by a PLAN containing "I must NOT attack into the defenders while
they have Sanguine Bond", "If I can't kill the defenders, I can't attack", and "I will pass
combat". First-wins executed the attack; every attacker's tag read
`(neither dies (blocking trigger: they gain 3))`; opponent went 18 -> 24 and the game ended 0/34.
The deck guide's WALL CHECK teaches this case correctly and in the model's own words — the loss is
not a teaching gap, it is the coded line outrunning the reasoning at the one seam the detector
does not cover. Wiring the existing `planArguesAgainstRow` check to attackers replies (an
"ATTACK: <list>" whose plan says pass / do not attack / hold) is the cheapest available fix.

**MED — the alternative-cost menu re-asks a question the model has already answered.** Every
`Cast Card Normally / cycling / Decline` menu (seq 22, 28, 71, and the 125-file pair) follows a
casting menu on which the model already picked the card and, in the annotations, already saw both
the cast text and the cycling text. It costs a full round trip (161-712 s each here) and is where
three of the four stranded-mana declines happened. Where the casting row already prices the cast
and there is no cost difference to decide, this window is pure overrun.

**MED — the X-decline and the phase advance interleave.** In repro A the decline was consumed at
`Phase: Combat begins`, one phase after the announcement, so the model answered an X window that
had already outlived its main phase. Worth checking whether the X chooser should hold the phase.

**LOW — narration prints a payment for a cast that never happened** (repro B's
`Paid {r}{r}{x} for Starstorm from mana already floating`). Folded into HIGH-1 but separable: any
"Paid ..." line should be emitted on the payment actually committing.

**LOW — 86% post-answer overrun in this seat, 1390 bytes on seq 24 alone.** The overrun is where
the model litigates against its own coded answer. It is not free: at ~20 s median and 100 s p90
these tails are most of the latency, and HIGH-3 shows one of them was right.

## 3. Guide verdict: KEEP as is

`projects/mtg/bin/Res/ai/baka/deck130_strategy.txt`, 71,227 bytes (band 41,000-71,500). No edit.

Reasons, each tied to a seq: (a) the two wins are the guide executing — the Hammer race in the
123 game (seq 57-73) and the priced-minimum X in the 152 game (seq 21, 28) are exactly what the
guide's rules #1/#2 and the Hammer plan instruct; (b) the 126 loss's decisive misplay (seq 24) is
a case the guide **already teaches correctly and by name** — the WALL CHECK's "still 'ATTACK:
none' while Perimeter Captain or Pride Guardian is on their line" — and the model's own prose
quoted the rule before the coded line overrode it, so more prose cannot help; (c) the 126 and 162
losses turn on HIGH-1/HIGH-2, false surfaces, and the trust doctrine forbids teaching a guide
hedge against a broken value; (d) the 125 loss is a Cancel on a turn-18 Rorix and the 146 loss is
a matchup the decklist cannot win, neither a decision the guide governs. Nothing in six games
identified a rule the guide lacks, and at 273 bytes of headroom any addition would have to delete
a rule that is currently being followed.

No `general-proposals.md` and no `skill-proposals.md`: every item above routes to the engine, and
the deck's own teaches were followed or overridden mechanically, not misunderstood.

## 4. Not checked

Only this deck's 6 seats (opponent seats read for context only, not audited). No fixture, suite,
build or replay was run; no engine source was read, so every HIGH is stated from the rendered
prompt and narration, not from the code path. The X-library reserve, reveal, stated-stop,
label-salvage and repeat-pay-band predictions had no window here and are marked UNTESTED, not
passed. Card facts verified against primitives (Pride Guardian mtg.txt:88356 `auto=@combat(blocking)
source(this):life:3 controller`; Starstorm mtg.txt:112835 `auto=damage:X all(creature)`,
`mana={X}{R}{R}`); no Scryfall disagreement checked beyond these two.
