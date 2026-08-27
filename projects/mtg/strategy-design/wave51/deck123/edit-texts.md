# deck123 (Intruders of Thraben) - wave-51 edits, before -> after

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck123_strategy.txt` (wave-50 revision, 56,413
chars). Fourteen edits by exact-string replacement on a copy; 56,413 -> 60,924 chars (+4,511 net:
+7,050 added, -2,539 deleted). Corpus: `matchups-20260827-155545`. Seqs cited by short form; full
file names in `wave51/seats/seat-123-130.md`.

## A51-1 - RULE 2 rewritten: the Flats is not mana until cracked; crack it the first window
**Finding.** 123v130 seq 6, 8, 10, 11, 12, 13: `Put a card onto the battlefield with Marsh Flats`
passed SIX times across turns 5-11 with Thraben Doomsayer {1}{w}{w} in hand and two Scrublands out;
the PLAN read "Cast Thraben Doomsayer next turn when I have 3 mana" every window. The prompt counts
2 untapped sources with the Flats on the battlefield (seq 5: `leaves 1 of your 2`). Molten Rain (t6)
and Stone Rain (t11) took both Scrublands; dead turn 24 at -2 without a token. Wave 50 recorded the
same shape unpunished at v152/v162; now it cost the game.
**Before:** `When lines beginning "Put a card ..." appear, read the colour counts ... take the DUAL`
**After:** `A Marsh Flats on your battlefield makes NO mana ... the first window that lists "Put a
card onto the battlefield with Marsh Flats targeting ..." is the window - on either player's turn`
+ the specimen. Plus the DECIDING line ("take it THIS window").

## A51-2 - RULE 2 colour: the dual is the one that supplies the pip your hand needs twice
**Finding.** 123v146 seq 5: Tundra + Underground Sea out, hand Doomsayer {1}{w}{w} + Keeper x2
{2}{b}{b} + Alarm; fetched a second Sea ({W} stayed 1). The Doomsayer sat in hand turns 1-14 until
an Arcane Sanctum arrived; dead turn 17 at 0. The old tie order ("Underground Sea, then Tundra, then
Scrubland") picked the one dual that helped neither spell.
**Before:** `Ties go to Underground Sea, then Tundra, then Scrubland. ... Never a basic while a dual
is listed`
**After:** `read "Your hand" for the pips your spells need TWICE ... take the dual that supplies
both of them with what is already on your battlefield ... Scrubland is the fetch - it is the second
W AND the second B.` Plus the deck130 basic exception (Molten Rain / Lay Waste / Blastminer hit
nonbasic only - primitive `land[-basic]` for Blastminer and Lay Waste; Molten Rain's 2 damage is
nonbasic-only; Stone Rain any land), evidence 123v130 (both Scrublands destroyed by t11).

## A51-3 - RULE 0: a `[from exile]` row is a real cast
**Finding.** 123v152 seq 20, 24, 26, 27: `Cast Intruder Alarm {4}{u} [from exile]` on the menu (Elite
Spellbinder's exile; primitive `canplayfromexile` + `changecost(colorless:2)`, Oracle agrees) while the
PLAN wrote "Intruder Alarm is exiled, I cannot cast it" four windows running; seq 27 cast Damnation
`destroys 1 of their creature ... 2 of yours` instead; seq 29 cast the Alarm from exile onto an empty
board. PERCEPTION failure on a true surface; the guide had no word for the tag.
**After:** new paragraph at the end of RULE 0 + a DECIDING line.

## A51-4 - RULE 3: the dictated Damnation sentence (#102 third-failure routing); anecdote cut
**Finding.** Entry 0 windows (M 0, K >= 2): 123v152 seq 20 (`destroys 2 ... 0 of yours`, 14 life,
cast Doomsayer; it died blocking next turn) and 123v130 seq 28 (`destroys 5 ... 0 of yours`, 1 life,
cast Doomsayer; Starstorm killed it) - 0/2 (wave 50: 1/3; cumulative 1/5). And 123v152 seq 27:
Damnation at `1 of theirs, 2 of yours` under "board is clear". Third corpus of entry-0 misses with the
row complete -> #102: shrink + move the check onto the plan sentence.
**Before:** the "At 0 against 4 this deck cast Vision Skeins ... died on the next attack" anecdote
(3 lines).
**After:** `PLAN: Damnation: N <their number>, K <able to attack>, M <mine>; <cast / hold>` with the
two entry-0 misses and the M 2 cast as the specimens; DECIDING line gains "Write ... in your PLAN
first". Net +330.

## A51-5 - RULE 4: toughness is the second number on the target row; the Slip rows print no result
**Finding.** 123v152 seq 10-11: Slip at `Elite Spellbinder (4/2)` under "it is 3/1, -1/-1 kills it";
123v130 seq 20-21: Slip at `Siege-Gang Commander #2 (2/2)`. Both lived and attacked. The rule
existed keyed to "toughness 1"; the target rows for -1/-1 print no `{right now: ... DIES/SURVIVES}`
(they do for damage spells - seat item M2).
**After:** `the SECOND number in the "(P/T)" on the target row ... A "(4/2)" and a "(2/2)" both have
toughness 2 and both live` + the two specimens; DECIDING line re-keyed.

## A51-6 - Idyllic Tutor: two paragraphs collapsed to one, keyed to the `[already owned:` tag (#138)
**Finding.** Dead-Tutor casts with the tag rendered: 123v125 seq 55 and seq 105 (the reply at 105
wrote the dictated "Tutor: dead - Alarm on my battlefield; it finds only an Alarm" AND cast it), plus
seq 48 by engine index-conflict (seat item H2). Fourth corpus; #138 leaves the guide at ONE
restatement now that lane E prints `[already owned: Intruder Alarm on your battlefield - this finds
only an enchantment card]` on the row.
**Before:** two paragraphs (the 3-life anecdote, the "Cast Idyllic Tutor to find Bloodline Keeper"
anecdote, the dictated "Tutor: dead" sentence).
**After:** one paragraph keyed to the tag + the counter-bait exception (A51-7). Net -540.

## A51-7 - KEY CARDS: against a counter deck, the spare goes first and the maker second
**Finding.** 123v125: nine spells countered (Essence Scatter s9, s81; Supreme Verdict is a sweep;
Fall of the Gavel s24, s31, s37, s100; Cancel s49, s85, s92; Dream Fracture s106), every one cast
alone into open mana; the two "dead" Tutors at s55/s105 were the only spells that resolved or drew a
counter usefully. The lever the corpus shows: cast the spare first, the maker second, same phase.
**After:** new paragraph after Lightning Greaves in KEY CARDS; the Tutor paragraph points to it.

## A51-8 - DECIDING: annihilator sacrifice picks - lands first, the Alarm last (#143 list ask)
**Finding.** 123v125 seq 60 and seq 112: Emrakul's "Sacrifice" ask (one row per permanent, six
picks); with 11 lands and one Intruder Alarm on the screen the Alarm went second both times.
**After:** DECIDING line naming the ask and the order.

## A51-9 - CHECK 5: the lone-attacker hold-back covers any only creature
**Finding.** 123v146 seq 31 (4 life): `A1. Human (1/1) [haste, shroud ...] [their untapped blockers:
Silverquill Silencer (3/2) (your attacker dies, their blocker lives)]` -> `ATTACK: A1` under a plan
that its death would "untap all creatures"; dead next turn with no creature.
**After:** one sentence after the maker hold-back.

## A51-10 - BLOCKING: a GANG BLOCK kill on a 4-power attacker at 6 life or less
**Finding.** 123v146 seq 14 (10 life): `A2. Nadaar (4/4) deals 4 [GANG BLOCK: any 2 of yours together
deal 5, enough to kill this attacker ...]`, B1 Keeper #2, B2 Vampire -> `BLOCKS: none`, 10 -> 4; seq
17's Damnation then killed both Keepers and a Vampire (`4 of theirs, 3 of yours`). deck130 carries
the sibling rule (D50-6); deck123 had none.
**After:** new bullet before "Otherwise your token-makers do NOT block".

## A51-11 - RULE 0: the #151 sentence (a hand card with no Cast row)
**Finding.** 123v130 seq 25: `CHOICE: 2 (Cast Thraben Doomsayer)` on a 4-row menu without it ({W}
1 on the mana line) -> `stale_echo_in_range` -> `named_row_reask` (lane C worked); seq 26 recovered.
**After:** three sentences in RULE 0.

## A51-12 - DECIDING: the "Skeins at hand 6 or more: not this window" line deleted (#118)
**Finding.** 123v152 seq 4 (hand 7), 123v130 seq 17 (hand 7): cast; wave 50 3/3 -> 5/5 across two
corpora. The harm is now a cleanup `discard` ask the pilot answers sanely (4/4 spares this corpus:
Damnation 1-of-2, Tutor with 3 Alarms in hand, Alarm 1-of-3, Damnation 1-of-3). The KEY CARDS fact
stays; the DECIDING absolute goes. -130.

## A51-13 - RULE 1: own-upkeep firing narrowed to sweeper decks (#118 violated-unpunished ceiling)
**Finding.** 123v162 seq 13-18 (own upkeep turn 10, x17 to M 20, Transform) - won turn 12; 123v126
seq 25-28 (own main phase, x23 to 26) - won turn 14. Wave 50: 2/2 own-turn chains won. 3 corpora, 4
own-turn chains, 4 wins; the one opponent-turn chain (wave 50 v162) lost to a draw step.
**Before:** `YOUR OWN UPKEEP IS THE WORST WINDOW OF ALL ... the answer there is "CHOICE: 0 (pass)"`
**After:** `... WORST WINDOW AGAINST A SWEEPER ... Against a deck whose log shows Starstorm, Supreme
Verdict or Damnation ... pass ... Against a deck with no sweeper in its log the own-turn chain has
won three games running at the L + C + 3 stop`. Odds, not a floor.

## A51-14 - DECIDING: the cleanup ask quotes the `PUT: n` shape (boundary holdout B1, #143)
**After:** `answer "PUT: n" with the number of the spare ... read the number back against the card
your PLAN names next.`

## Not edited, recorded
- RULE 1 P1: 123v126 seq 47 `CHOICE: 29 (Create vampire with Lord of Lineage x17)` at M 27 against
  a stop of 24 with NO PLAN line (the one repeat-row take at M >= stop this corpus; wave 50: 3). The
  answer-first sentence is in the guide; one window is not a third failure. The seq 48-51 aftermath
  (`stale_livelock` fallback casting Devour Flesh mid-repeat, the pilot then naming itself and the
  Lord) is engine lane G - cited in the seat file, no guide text.
- Single-row CHOICE/PLAN conflicts ("this window: pass" under `CHOICE: 1`): 123v162 seq 29, 123v126
  seq 37, 44, 123v125 seq 15 - one token each, harmless; lane C's conflict re-ask covers the repeat
  row only (seat item M3).
- 123v152 seq 7: Doomsayer over `they control 1 creature - Katilda` (A50-10's window, 0/1 again);
  Katilda died to a Human block two turns later; the Spellbinder that exiled the Alarm was the game.
  Recorded, not restated - the RULE 5 text is one corpus old.
- 123v152 seq 17: Devour Flesh at `they control 2 creatures - they choose` (took a 1/1 Aspirant,
  not the 5/3 Spellbinder). RULE 5 exists; second corpus of N >= 2 casts; recorded.
- 123v126 seq 49-50: `Yourself` then Lord of Lineage named on a Devour Flesh the fallback cast -
  engine-caused (lane G); the RULE 5 "Yourself is for a token" line stands.
- Second Alarm cast with the tag: 123v125 seq 36 (countered). Lane E's tag was on the row; one
  window; the Tutor paragraph's "second Alarm is entry nothing" line was deleted with A51-6's
  collapse - the `[already owned:` sentence now covers both cards.
