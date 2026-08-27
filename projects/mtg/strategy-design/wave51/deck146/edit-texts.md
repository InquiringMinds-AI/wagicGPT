# deck146 - wave-51 edits (corpus `matchups-20260827-155545`, deck146 3-3)

Baseline = live `bin/Res/ai/baka/deck146_strategy.txt` (49,553 bytes, wave-50 reviewer revision +
boundary pass B1/B2). After = `wave51/deck146/strategy.txt` (52,658 bytes, +3,105). Eight edits,
each paid by a cited seq. Card facts read off `bin/Res/sets/primitives/*.txt` with `/usr/bin/grep`
(Kaya the Inexorable `-3: target(*[-land]|battlefield) moveto(exile)`, `+1: Don't target any
creature` row exists; Silverquill Silencer `chooseanameopp ... @movedto(*[chosenname]|opponentstack):
life:-3 opponent && draw:1`; land count from `bin/Res/ai/baka/deck146.txt`: 6 Plains, 4 Swamp, 4
Brightclimb Pathway (W // Grimclimb B), 4 Shineshadow Snarl (W/B), 3 Hive (B) = 21; white sources
14, black 15, either-colour 8).

| # | section | before -> after (abridged; full text in strategy.txt) | finding (file, seq) |
|---|---|---|---|
| 146-M | MULLIGAN STEP 2 | *"one of these four: ZERO lands; an all-land hand; or NO source of one of your colors when the header shows every spell needs it; or ONE land under ..."* -> *"one of these three: ZERO lands; an all-land hand; or ONE land under ..."* (the colour clause is CUT). | `146 vs125` s1-s5: five consecutive mulligans, every reply's PLAN "find a hand with at least one white mana source"; s2's reply quotes the clause back verbatim - *"This is a 'no source of one of your colors' mulligan condition"* - at (keeping 6), then carries it to (keeping 5) [3 lands, covers Pelakka + Agadeem's], (keeping 4) [1 land], (keeping 3) [2 lands]; kept TWO cards at s6, first spell T11 (s18), lost -14/45 at T80. The clause was look-scoped in the text and unscoped in the pilot's reading; the STEP 1 "keep with even one land" sentence lost to it three looks running. deck152's FORBIDDEN THOUGHT already cuts colour entirely - #144 holdout closed. |
| 146-M (b) | MULLIGAN, new bullet before "THE ENGINE COUNTS YOUR HAND" | new: *"COLOUR IS NEVER A REASON TO SHIP, AT ANY LOOK ... Fourteen of your 21 lands make white, fifteen make black and eight make either ... a three-land hand of Swamps that covers a Soul Shatter is a KEEP at every look. THE CARRIED PLAN IS NOT EVIDENCE ABOUT THIS HAND ... when the plan says ship and the header says one or more lands at (keeping 5) or lower, the plan is out of date: say so and keep."* | same chain; the carried-plan paragraph is deck152 RULE -1's, ported per #144 (deck146 had no carried-plan sentence; s3/s4/s5 replies each restate the s1 plan). STEP 1's cost line updated from "three-card hand" to "and a corpus later ... a TWO-card hand" (one clause). |
| 146-N | #1 RULE, Kaya bullet + DECIDING | adds: *"When EVERY row on her -3 list carries '[your battlefield]' ... take the +1 ('+1: don't target any creature' is a legal +1) or pass. Exiling your own Kaya or your own creature is the card and the loyalty set on fire; this seat did it at 1 life ..."* DECIDING: *"Every row on Kaya's -3 list reads '[your battlefield]' -> +1 or pass; never the -3."* | `146 vs125` s284 (T78, 1 life): `CHOICE: 2 (-3: exile non-land permanent with Kaya the Inexorable targeting Kaya the Inexorable [your battlefield])`; reply: *"Exiling one of our own ..."* as "the only action available". The +1 row (`+1: don't target any creature`) was taken correctly at s35/s99 earlier in the same game, so the pass/+1 alternative was known to the seat. Lane W's self-target prediction (0 takes) fails on this window - the row carried `[your battlefield]` and was taken anyway; guide half here, render half in the seat file. |
| 146-O | #1 RULE, new SILVERQUILL SILENCER bullet (before Pelakka) | new naming order: *"(1) a card Pelakka Predation showed you ... or the missing half of a pair whose other half is already on their line; (2) the card their deck is built to cast again - a punisher enchantment, a token maker, a lord they replay, a counterspell you have seen; (3) never a card already sitting on their battlefield line, and never a defender."* | `146 vs126` s13 (T9): named **Pride Guardian** with a Pride Guardian already on their line (a name that fires zero times); s57 (T21) named Sanguine Bond correctly - it was cast again and the seat drew (Bond had been Versed at s26). `146 vs152` s7 named Katilda - the second Katilda was cast at `152 vs146` s13 (T6): 3 life + a card. The live guide had NO naming rule for the card (entry 4 says only "your other real body"). |
| 146-P | MULLIGAN, cleanup-discard order | *"a second planeswalker, then the priciest spell ... then a duplicate ... A land goes ONLY when your battlefield line already prints four or more lands"* -> *"a second planeswalker, then a land in hand once your battlefield line already prints four or more lands (with eight lands out and two Plains in hand this seat sent Lolth AND Kaya under ...), then the priciest spell ... From fewer than four lands out, the PUT line holds spell numbers only"*. | `146 vs162` s45 (T13, 8 lands out, hand of 10 incl. two Plains): `PUT: 6, 9, 10` = Emeria's Call, Lolth, Kaya. The order placed "a land" after "a second planeswalker" without saying the second walker outranks a ninth land; the land was the right third card. Game won anyway (T15). |
| 146-Q | DECIDING mulligan line | *"(nine of 21 make both)"* -> *"whatever colours those lands make and whatever your carried plan says ... (fourteen of 21 make white, fifteen make black, eight make either)"* | The "nine" was wrong by the deck list (8 either-colour lands: 4 Snarl + 4 Pathway; Hive is black only). Same finding as 146-M. |

## Read but NOT edited (recorded, per #45/#102/#109)
- **`[repeat: activated this turn 1 times already]` Hive rows taken 3 of 3 times offered after a
  Main-1 animation** (`146 vs125` s91 T47, s112 T51, s262 T76; 12 mana on the second copies of a
  3/3 that already existed). The guide's sentence ("That tag is an unconditional PASS ... Read it
  before you read anything else on the row") exists; each reply ran CHECK 0-3 and never quoted the
  tag. #138 shape - a rule quoted-around and broken; the lever is the offer (seat file E-2, carried
  D12) and no fourth restatement is written.
- **Lolth's 0 at 3 and 2 life** (`146 vs125` s269, s282). The floor sentence exists; s282's reply
  says *"Lolth's 0 ability is suicidal ... We must pass"* and its CHOICE line is `1 (Lolth 0)` - the
  E-5 / #133 shape (decision reversed in prose, no `this window: pass` literal for lane C to catch).
  Engine item E-4 in the seat file; no restatement.
- **Verse on Perimeter Captain at T5** (`146 vs126` s7) - the target list held no Bond/Blood then;
  146-K held when it mattered (s25/s26 took Sanguine Bond over two walls). The loss (0/60 T22) was a
  second Sanguine Bond (Silencer #2 named it at s57 and it was cast anyway) plus Exquisite Blood
  and a Tribute to Hunger - no window in the log had an answer left.
- **Soul Shatter**: 6 casts, 6 sacrifices, the highest-MV victim every time incl. two
  planeswalkers (Ob Nixilis `vs162` s40, Sorin `vs126` s43) and a protection-from-colored Emrakul
  (`vs125` s170). B2's re-key is confirmed; no edit.
- **`146 vs130`** (-1/7 T38): three lands from T21 on (Pelakka at s7 revealed Stone Rain + Molten
  Rain; the seat's lands were destroyed T9-T21 - no cast window between s9 and s13); the six
  cleanup discards followed the order (Agadeem's, Emeria's, Command, Lolth, Pelakka, Command). No
  guide window.
- **`146 vs125` s250**: at 3 life holding four dead Vanishing Verses, discarded a live Silverquill
  Command over a Verse duplicate. The order says "a duplicate of a spell already in hand"; single
  instance at a lost board, recorded.
