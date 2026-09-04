# wave-59 deck146 review (Orzhov dungeon midrange)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-130044/`, 6 games, all natural.
Record **4-2**. Zero fallbacks, zero `unparsed_reply`, zero `transport_error`, zero
`wall_miss_events`, zero `async_drops` across all six seat logs. Files below are named by
opponent; every one is `...ai_baka_deck146-<ptr>-vs-ai_baka_deck<N>.jsonl`.

## 1. Game by game, and the decisions that decided them

| vs | result | turn | final life | shape |
|---|---|---|---|---|
| 126 | WIN | 19 | 21 / -2 | curve-out; Kaya -3 twice at the combo (Exquisite Blood s16, Sanguine Bond s24) |
| 123 | WIN | 11 | 22 / -1 | fastest game; five-body alpha at s17 took them 14 -> 2 |
| 152 | WIN | 17 | 21 / -4 | grind; Verse on Elite Spellbinder s11, Barrowin recast from exile s24 |
| 130 | WIN | 25 | 2 / -2 | race won by 2 life; see the s55 misplay below |
| 162 | LOSS | 17 | 0 / 20 | draw-punisher lock (Underworld Dreams + Fate Unraveler x2 + Ob Nixilis + 3x Howling Mine) |
| 125 | LOSS | 47 | -14 / 29 | Staff of Nin pinged 16 times; Emrakul closed |

**162 (loss) - where it was lost.** Death was locked in before the seat could see it. At
`146v162` **s22** (T13) Kaya's -3 exiled Fate Unraveler and left Underworld Dreams; Kaya then sat
at 2 loyalty (below -3) for the rest of the game. From T13 the DRAW FORECAST line was accurate and
lethal (`s19`: *"DRAW FORECAST: your next draw step draws 3 cards ... = 3 x 2 = 6 life"*, life 13),
and the seat had no second answer in hand. Both -3 targets punished draws for 1; Fate Unraveler was
also a 3/4 clock, so the pick is defensible - the loss is the matchup plus never pressuring
(opponent life "unchanged at 20 since turn 9" from s19 on). At **s35** (T17, life 1) the seat drew
Vanishing Verse off the punished draw and exiled Underworld Dreams, but a UD damage trigger was
already on the stack; it died at 0. Nothing on that menu could have saved it. STRATEGY, not
perception - and largely variance.

**125 (loss).** Opponent resolved Staff of Nin (colorless artifact) on T18; it pinged the seat 16
times. deck146's only answer to a colorless permanent is Kaya's -3, and **Kaya never appeared in
the seat's hand in this game** (checked every `Your hand (...)` render). The guide already names
this exact card and this exact answer (deck146_strategy.txt L612-617). Variance, not guide.

**130 (win, but one real misplay).** `146v130` **s55**, T23 Main 2, life **5 vs 2**, right after
completing Lost Mine: the venture menu re-offered all three dungeons and the seat answered
**Dungeon of the Mad Mage** - the one dungeon the guide explicitly forbids (L516-517, "the longest
dungeon at nine rooms ... unless it is a slow grind"). Its room 1 is "Yawning Portal - You gain 1
life". Tomb of Annihilation's room 1 is *"Trapped Entry - Each player loses 1 life"*, which with the
opponent at 2 was on-board damage and exactly the guide's own carve-out (L512: pick Tomb when
"RACING and clearly ahead on life"). The reply's PLAN never mentions the dungeon at all. STRATEGY
(guide content present, not applied). The seat won two turns later anyway.

## 2. Engine / interface / card items

**HIGH-1 - pending damage already ON THE STACK is never priced, at the exact windows where it is
lethal.** The `ON THE STACK, waiting to resolve` block lists trigger rows but no arithmetic; the
prompt's only damage forecast (`DRAW FORECAST`) prices the *next* draw step, not the stack in front
of the seat. Repro: `...deck146-...-vs-ai_baka_deck162.jsonl` **seq 26**, T15 Draw, `Your life: 5`:

```
ON THE STACK, waiting to resolve (top resolves FIRST - you can respond now):
  1 (top): ability: Ob Nixilis, the Hate-Twisted's triggered ability [from their Ob Nixilis, the Hate-Twisted] [triggered/activated ability]
  2: ability: Underworld Dreams's deal 1 damage [from their Underworld Dreams] [triggered/activated ability]
  3: ability: Ob Nixilis, the Hate-Twisted's triggered ability [from their Ob Nixilis, the Hate-Twisted] [triggered/activated ability]
  4: ability: Underworld Dreams's deal 1 damage [from their Underworld Dreams] [triggered/activated ability]
```

Four damage is on that stack and the seat was at 5; nothing on the page says "you would be at 1".
Note also that row 1 and row 3 render as a bare `triggered ability` with no effect text while rows
2/4 spell theirs out, so even reading row-by-row does not give the number. The same gap is what the
seat reasoned its way into at **seq 35** (T17, life 1): its PLAN there asserts *"If Underworld
Dreams is gone, I take 0 damage ... I survive the turn"*, and at **seq 37** it works out unaided
that the already-stacked trigger still resolves and writes *"I am dead."* Census: **13 of 77** stack
renders in this deck's corpus carry damage aimed at the seat, **6** of them at life <= 7 and **3**
at life 1 (162 s25/s26/s35/s36/s37; 125 s63/s64/s114/s140/s141/s149/s194). Ask: a
`PENDING ON THE STACK: N damage to you - you would be at K` line (with `; that would KILL you`, the
same claim shape D9/K8 already use), plus effect text on every damage row.

**HIGH-2 - the venture ask still never names the source, and the source is the whole decision.**
Recurrence of wave-58 deck146 MED-1, now with a cost. **11 of 11** venture asks in this deck's
corpus render only `VENTURE - CHOOSE A DUNGEON to enter ...` with the three dungeons; not one names
the card that ventured. In **2 of the 11** the venturer was Acererak the Archlich
(`...vs-deck162` **seq 17**, `...vs-deck130` **seq 55**), whose own primitive
(`bin/Res/sets/primitives/borderline.txt:441-445`, verbatim Oracle, verified) conditions on ONE
named dungeon: *"if you have not completed the dungeon Tomb of Annihilation, return Acererak the
Archlich to its owner's hand and venture into the dungeon."* At 162 s17 the seat had been shown
that clause one window earlier on the cast row (s16) and had no reminder here; it answered Lost
Mine, so Acererak bounced on every cast for the rest of the game (log: *"Your Acererak the Archlich
resolved and entered the battlefield / Your Acererak the Archlich was returned to your hand"*, T11
and again 130 T23). Ask: name the venturing source in the ask's header, and when the source's own
text names a dungeon, print that clause on the matching row. (Corpus-wide the pick is monotonous:
Lost Mine 10/11, Mad Mage 1/11, Tomb 0/11 - the guide's default is doing the work, so the ask's
"weigh how many rooms to completion" sentence is currently decorative.)

**MED-1 - three-dungeon menu option NUMBERS move between renders.** 162 s17 rendered Tomb 1 / Lost
Mine 2 / Mad Mage 3; 130 s55 rendered Tomb 1 / **Mad Mage 2** / Lost Mine 3. Legal and the guide
warns about it (L505-506), but the format example in the ask hard-codes option 1
(`e.g. "CHOICE: 1 (Tomb of Annihilation - full room path: "Trapped )"`) - and that example is
itself malformed, truncating mid-quote inside the parenthetical the reply protocol says is a SHORT
NAME. A stable order (or a name-only example) removes a whole class of index slips.

**LOW-1 - dungeon rooms are not priced against the life totals on the page.** The room path is
printed verbatim ("Trapped Entry - Each player loses 1 life") but nothing connects it to
`Your life: 5 | Opponent life: 2` two screens up. This is the missing half of HIGH-2 at 130 s55.

**LOW-2 - ask volume.** `146v125` spent 152 `ask` records over 47 turns with
`hold_windows_skipped: 253` and `identical_ask_answers_reserved: 44`; 60+ of those asks were
answered `Cast nothing right now` from an empty or uncastable hand. The HOLD row is working (it was
taken 40+ times) but the seat is still being asked at every phase of a board it cannot affect.

**Card facts verified this pass** (all three agree with the primitive AND Scryfall, no defect):
Acererak the Archlich `{2}{B}` 5/5 (borderline.txt:440-451); Ob Nixilis, the Hate-Twisted
`{3}{B}{B}`, MV 5 (planeswalkers.txt:2684-2692; Scryfall `cmc 5.0`) - the Soul Shatter row's
`{right now: they sacrifice Ob Nixilis, the Hate-Twisted (MV 5, their highest)}` at 162 s26 is
correct; Tomb of Annihilation's completion path (borderline.txt:118762-118772).

**Wave-59 lane items observed in this seat** (per-deck sightings only; the engine seat adjudicates):
K7's MDFC fix is visibly live - `146v123` **seq 4** now renders
`Grimclimb Pathway [PLAY THIS AS A LAND: ...]` and `146v162` **seq 4** renders
`Brightclimb Pathway (menu text: Play Land) [PLAY THIS AS A LAND: ... (taps for {W}) ...]`; **no
bare `Play Land` row appears anywhere in this deck's six logs**. K8's crack-back line rendered **49**
times and every one I recomputed against its own `Opponent battlefield` line was arithmetically
right, including defender exclusion and untap-first counting (162 s20: Shield Sphere excluded, Fate
Unraveler tapped but counted, "for up to 3 - you would be at 4" from life 7; 130 s35: Siege-Gang 2 +
five Goblins = "for up to 7"). K4 (`{dead right now:}`), K5 (the D7 floor form) and the F/G/H
signatures got no window in this deck's games.

## 3. Guide verdict: **KEEP as is**

No edit. The guide is not what lost either game and it is not what the one misplay lacked.

* **Both losses are matchup/variance, not doctrine.** vs 125 the guide already names Staff of Nin
  and Kaya's -3 as the only answer (L612-617) and Kaya was never drawn. vs 162 the seat had one
  Verse and one Kaya activation against four draw punishers; the guide's removal-priority and
  do-not-sit-on-removal rules were followed (Verse s11 on Fate Unraveler, Soul Shatter s26 on Ob
  Nixilis, Kaya -3 s22), and the seat still lost the count.
* **The one real misplay (130 s55, Mad Mage) is content the guide already carries**, in two places
  (L512 Tomb-when-racing, L516 never-Mad-Mage). Adding a third statement of a rule the seat skipped
  once in eleven venture asks is the busywork pattern; the fix that would actually reach that
  window is HIGH-2/LOW-1 (put the source and the room's life arithmetic on the ask), not more text
  in a 70 KB file.
* **What the guide demonstrably bought:** the Acererak-bounces section (L492-503) - the seat cast
  Acererak only as a spare-mana venture in all four sightings and never planned around a 5/5;
  the mulligan rules - two mulligans, both stopping at (keeping 6) on a hand the STEP-2 list
  keeps, no colour-ship anywhere; the Silencer naming rule - every name chosen from a card
  actually visible (`Perimeter Captain` 126 s6, `Vision Skeins` 123 s8, `Teferi` 152 s31,
  `Hammer of Bogardan` in their graveyard 130 s58); Kaya -3 at the combo pieces (Exquisite Blood
  126 s16, Sanguine Bond s24), which is what turned that game.

No `general-proposals.md` and no `skill-proposals.md`: HIGH-1/HIGH-2 are engine surfaces, and the
one rules misconception I saw (a trigger surviving its source's removal, 162 s35) the model
corrected unaided one window later on a decision that was already lost - too thin to pay for a
general-guide amendment.

## What I did NOT check

The opponent seats' own translogs (I read them only as reflected in the seat's GAME LOG); the
stderr files; every one of the 77 stack renders individually (I recomputed 13); the arithmetic of
all 49 crack-back lines (I recomputed the 20 distinct board states); wave-58/59 lane predictions
(the engine seat's job); deck146's dungeon-room *branch* asks beyond confirming they print the room
number and effect; and whether `Silverquill Command`'s mode rendering is faithful (only two casts,
both looked right, but I did not diff its primitive).
