# deck146 -> development notes (wave 33)

Corpus `matchups-20260819-070739`, binary **bdb30d8e7**. Seat volume 238 records / 227 model calls,
**0 fallbacks**, **0 crash signatures** in all six deck146 stderr files.
Latency: median 16.8s / p90 82.1s / max 228.3s / 24 calls over 60s. Prompt chars p50 9,741 /
p95 18,310 / max 25,293. Reply chars p50 1,077 / p95 11,546 / max 15,471.
Seat inference per game: vs116 **30.4 min**, vs152 29.5, vs36 24.1, vs139 17.9, vs158 15.3,
vs105 9.4.

Two new HIGH items (one parser, one render), two new card-script/engine divergences, plus
dispositions on the carried ledger. **CLOSED this wave: N-146k, N-146l, and N-146j's engine half.**

---

## N-146m (NEW, **HIGH**, PARSER/ENGINE) -- a `CHOICE: N (name)` whose NAME matches no option is committed by INDEX

**Symptom.** The model answers the PREVIOUS question (its own carried PLAN), the name in
parentheses appears nowhere in the current option list, and the reconciler silently commits the
number anyway. No fallback is recorded, so the instrument shows a clean decision.

**Repro (deterministic, from the corpus).** `~/.Wagic/ai/gpt/logs/1787147652-ai_baka_deck146-
0x5569983cdf50-vs-ai_baka_deck116.jsonl`, **seq 34**, kind `ask`, `latency_ms=11646`, `choice=1`,
no `fallback` field. Prompt tail (options), abbreviated:

```
1. Tomb of Annihilation [your zone] [dungeon: 5 rooms; completion reward ("Cradle of the Death
   God") ...]
2. Dungeon of the Mad Mage [your zone] [dungeon: 9 rooms ...]
3. Lost Mine of Phandelver [your zone] [dungeon: 7 rooms ...]
```

Reply, verbatim first line:

```
CHOICE: 1 (Nadaar, Selfless Paladin #1 (5/5) [vigilance] [your battlefield])
```

`chosen_text` = `Tomb of Annihilation [your zone] [dungeon: 5 rooms; ...]`.

**Why it happened.** seq33 was a target choice between two Nadaars; its PLAN (*"Resolve the
Barrowin trigger by targeting the returning Nadaar, then proceed to combat damage"*) is carried
into the seq34 prompt, and seq34 did NOT carry the staleness note that seq32/33 carried
(*"the actions your plan names are no longer among the options available right now"*). The model
re-emitted the plan's action as its answer. The parser's index-first resolution then made a
consequential choice out of an answer to a different question.

**Cost.** The deck entered **Tomb of Annihilation** against its guide's #1 dungeon teach. Downstream:
the Tomb room chain, the 214s `veils of fear` spiral (seq35), the 228s discard spiral (seq37),
Acererak discarded to a Tomb room, and the game reaching the -T 3000 cap (adjudicated 20-14 at
turn 14; ~47.5 of 50 minutes were inference, 30.4 at this seat).

**Class rate.** 1 of 189 `CHOICE: N (name)` replies at this seat name something absent from the
option list. (27 further hits are `CHOICE: 0 (pass)` on option-less priority asks -- benign; the
detector must exempt them.)

**Suggested direction (not a patch).** When a reply carries BOTH an index and a parenthetical
name, and the name anchors to no option, the index is not evidence -- it is a coincidence. Treat
name/index disagreement as a `stale_echo`-class rejection: re-ask, or fall back to the heuristic,
and log it. It is currently indistinguishable from a correct answer in the translog, which is the
worse half of the defect (an instrument that cannot see its own miss). Secondary: the staleness
note that fired on seq32/33 did not fire on seq34 -- worth checking why the drift detector
considered the plan fresh for a menu it shares no options with.

---

## N-146n (NEW, **HIGH**, RENDER) -- the dungeon room-branch menu never says which room you are ON

**Symptom.** The venture room choice renders two room names plus their `{room effect}` text and
nothing else. The model cannot tell where in the dungeon path it is, whether the two options are a
branch or a sequence, or which room each option advances TO -- and it spends thousands of
characters trying to reconstruct that from the narration's explore counters. **This is the largest
latency driver in the corpus.**

**Repro 1** -- `1787147652-...-vs-ai_baka_deck116.jsonl` **seq 35**, `latency_ms=214088`,
`post_plan_overrun=9091`. Options, verbatim and complete:

```
1. veils of fear {room effect: Each player loses 2 life unless they discard a card.}
2. oubliette {room effect: Discard a card and sacrifice an artifact, a creature, and a land.}
```

Reply, verbatim excerpt:

> It seems the game is asking us to pick the *next* room, but the options are weirdly skipping
> "Sandfall Cell". Or perhaps "Oubliette" is the name of the *branch*? [...] Perhaps "Oubliette"
> is a typo in the prompt's options list and it meant "Sandfall Cell"?

**Repro 2** -- `1787142977-...-vs-ai_baka_deck152.jsonl` **seq 45**, `latency_ms=213849`,
`post_plan_overrun=12155`. Same two-room shape (`storeroom` / `dark pool`); the reply hand-counts
explore counters turn by turn to locate itself:

> Turn 11: 1st explore. Turn 12: 2nd explore. Turn 13: 3rd explore. Turn 15: 4th explore. [...]
> So I am at the 5th room: Dark Pool. The options are: 1. Storeroom (Room 4) - Wait...

**Repro 3** -- `1787144409-...-vs-ai_baka_deck36.jsonl` **seq 22**, `latency_ms=200989`,
`post_plan_overrun=15163`: the largest overrun in the corpus, containing *"Wait, the prompt asks
for a choice on the dungeon?"* inside what is actually a land-drop ask.

**Why it is a surface defect and not a model defect.** The information is not merely absent from
the option lines; nothing in the prompt supplies it. The venture state (which dungeon, which room,
how many explores) is engine-side and knowable. Per the trust doctrine the model owes the surface
its belief and the surface owes the model truth -- an underspecified menu that forces the model to
re-derive engine state from narration is the same failure family as a wrong-scope statement, and
here it costs 200s+ per occurrence.

**Suggested direction.** Two facts on the ask, both already in the engine: the dungeon and the
current position (`Tomb of Annihilation - you are entering room 2 of 5`), and per option which
room it advances to (`1. veils of fear (room 2 of 5) {room effect: ...}`). **Falsifiable
prediction if shipped:** the 200s+ room-choice spiral class at this seat goes to zero, and the
`post_plan_overrun` nonzero rate at this seat (currently 22/227) falls by roughly the three
specimens above (~26k of the 68.5k overrun chars).

**Related, same ask family (LOWER):** the `veils of fear` DISCARD ask (vs116 seq37, 228.3s,
overrun 8,638) renders the card list with no statement of whose discard it is or which effect is
asking. The reply litigates *"This implies *we* ... must also discard a card if we don't lose
life? [...] Regardless of the exact wording nuance, the game is forcing me to discard"* for
~2,000 chars. One line naming the source and the reason would close it.

---

## N-146o (NEW, MEDIUM, CARD SCRIPT) -- Nadaar's anthem buffs Nadaar

**Symptom.** Oracle: *"**Other** creatures you control get +1/+1 as long as you've completed a
dungeon."* The primitive (`borderline.txt:75541`) is:

```
auto=this(variable{pdungeoncompleted}>0) lord(creature|myBattlefield) 1/1
```

`lord(creature|myBattlefield)` includes the source, so Nadaar buffs itself.

**Live witnesses.** vs116 seq35 board render, verbatim: `Nadaar, Selfless Paladin {2}{w} (4/4)
(printed 3/3) [vigilance]` with **one** Nadaar in play and 2 dungeons completed (the +1 can only
be its own). Double-confirmation at vs116 seq33, where **two** Nadaars render `(5/5)` each --
3/3 +1 (the other) +1 (itself); with correct semantics they would be 4/4.

**Cost this corpus.** No decision was lost to it (it inflates the pilot's own attack math in its
favour and the render is self-consistent), but it is a rules-wrong card script in a deck whose
whole payoff is the anthem, and the guide teaches the anthem explicitly.

**Suggested direction.** `lord(other creature|myBattlefield) 1/1` -- the idiom other anthem
primitives use. Verify against a PARSETEST/board-render case with exactly one Nadaar and a
completed dungeon (expected `(3/3)` with no `(printed ...)` tag on Nadaar and `+1/+1` on
everything else).

## N-146p (NEW, LOW, CARD SCRIPT) -- Soul Shatter never takes a planeswalker

Oracle: *"Each opponent sacrifices a creature **or planeswalker** with the greatest mana value
among creatures and planeswalkers they control."* The primitive
(`borderline.txt:106257`) sacrifices
`notaTarget(creature[manacost=convertedcost:highest:*:myBattlefield]|myBattlefield)` -- creatures
only, and the `text=` line states the full Oracle. Not exercised this corpus (no opposing
planeswalker was on the board when Soul Shatter was castable). Recorded so the guide's
engine-scoped teach ("does nothing into an empty creature board") is not mistaken for a guide
error later; the guide stays engine-true until this is fixed.

## OBSERVATION (ledger only) -- the PLAN line is a spiral carrier, not just a length problem

14 replies at this seat emitted a `PLAN:` tail over 800 chars (max **15,164**). Because the PLAN
is the only state carried between decisions, a spiral written after the answer becomes prompt
input for the NEXT decisions. Proven at vs116: the 9,091-char seq35 spiral is re-injected
**verbatim** as `YOUR PLAN (as you last stated it)` into the seq36 and seq37 prompts, and seq37
then re-ran the same anthem/lethal arithmetic that seq35 had already failed to settle. Routed as
EVIDENCE to general-suggestions.md under owner directive item 1 -- no counter-proposal offered.

## OBSERVATION (ledger only) -- poison narration double-line at the receiving seat

vs105 seq22-24 narrate two poison lines for one combat step:
`- Poison: you take 2 poison counters - now 3 of 10` (Blight Mamba) followed by
`- Poison: you now have 3 of 10` (Necropede). Arithmetic is correct (1 + 2 = 3) and the pilot read
it correctly, but the second line states a total with no increment and could read as a no-op.
deck105 is the owner seat for i1; noted here as a second-witness wording observation, not a defect.

---

## CARRIED LEDGER -- dispositions at this seat

- **N-146j** (`[-multicolor]` excluding artifacts and lands): **FIXED AND VALIDATED.**
  `TargetChooser.cpp:1066-1092` now sets only `setisMultiColored(-1)`; Vanishing Verse's primitive
  is `target=*[-multicolor;-colorless]|battlefield`. Falsifier passes (0 colourless targets in 21
  target lists); the positive half (a monocolored ARTIFACT offered as a target) was **not
  exercised** -- no coloured artifact permanent appeared at this seat in six games. The guide
  crutch is demoted this wave regardless, because it is now a false statement.
- **N-146k** (pregame battlefield/mana line): **CLOSED.** 0/8 pregame prompts carry a
  `Mana available:` line or any battlefield fact; the echo class produced 0 replies.
- **N-146l** (poison total never rendered): **CLOSED.** 15/15 records from first infect damage
  carry the poison status line, distinct infect narration, and a poison-priced blocker forecast.
- **N-146h layer 2** (Agadeem's Awakening): **STILL UNEXERCISED** -- 0 Cast option lines, 0 casts,
  0 face menus; the card never reached a castable window. Probe-lane per the wave-32 request
  (`strategy-design/wave33/probe-n146h/` was not produced this wave). The guide's "leave Agadeem's
  Awakening alone" crutch stays; demotion not eligible. NOT re-diagnosed here.
- **N-152a** (MDFC back-face land placement): still live and still engine-true at this binary --
  vs116 seq30 offered only `Cast Card Normally` / `Flip Side [display toggle only - no game
  effect]` for Pelakka Predation. The guide's spell-only teach remains correct.
- **N-158e**: not exercised (this deck's instants were all cast at sorcery speed on its own turns).
- **N-158k / N-152g / N-116a / N-139m**: not exercised at this seat.
- **Kaya +1 render/engine divergence (LOW, new note):** the option's `{card text}` shows the full
  Oracle (*"up to one target nontoken creature"*) while the primitive targets
  `creature[-token]|mybattlefield` (your own only). No cost this corpus -- the one +1 (vs152 s38)
  targeted its own Triumphant Adventurer. Recorded because the guide teaches the ENGINE version.
- **i5 / i6 / i8 / i16**: 0 `validation_reject_reask`, 0 truncations, combat tags render and are
  consumed correctly (no paradox verbalizations found this corpus -- the wave-32 residue did not
  recur), 0 FATAL AIStats lines.
