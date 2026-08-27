# deck123 (Intruders of Thraben) - wave-49 edits, before -> after

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck123_strategy.txt` (wave-48 revision, 44,201
chars). Nine edits. 44,201 -> 49,842 chars. Corpus: `matchups-20260827-094106`. Every number is read
off a prompt, a reply, a narration line or a primitive. Seat files cited by their short form; the
full names are in `wave49/seats/seat-123-130.md`.

---

## A49-1 - RULE 1: N is a subtraction done on the screen; N <= 0 is a pass; the stop number rides in the PLAN line
**Finding.** `...deck123-0x561c789917c0-vs-...deck125.jsonl` seq 11-22, turn 9 upkeep: L = 20, C = 0,
stop = 23. seq 12 computed and wrote x19 at M = 3 (correct); then seq 16 x1, 17 x1, 18 x2, 19 x25,
20 x50, 21 x120, 22 x100 with M at 24, 25, 26, 28, 53, 103, 223 - every take past the stop. The PLAN
had shrunk to *"Attack with all creatures to win the game"*: a sentence with no number (#115). Also
`...deck123-0x55cfc41f41b0-vs-...deck130.jsonl` seq 23: x1 at M = 25 = stop. 8 of 11 takes past the
stop. The old text's WHY ("N=17 ... carried out as ONE token") is stale: lane S's re-ask fired at
vs125 seq 11 (`repeat_count_reask`) and the count grammar widened (#103, #104).
**Before**
```
    Write it exactly that way: in this corpus "N=17" and the bare row label with no number
    were each carried out as ONE token, three times in one turn ...
    Do NOT write x50 out of habit. Four x50 answers on one turn put 257 creatures on a board
    whose L + C + 3 was 30; every activation past the stop is a window spent on nothing.
```
**After**
```
    Name the number the first time: a take with no number is asked again, once, and that is a
    window spent. ...
    DO THE SUBTRACTION ON THE SCREEN BEFORE YOU WRITE N, and carry both numbers in your PLAN
    line in this shape, blanks filled from the screen:
        PLAN: stop at M = <L + C + 3>; M is <M> now; this window: <x<N> / pass>
    When (L + C + 3) - M is 0 or less the repeat row is not yours: CHOICE: 0 (pass). This deck
    did the subtraction once - x19 at M = 3 against a stop of 23 - and then answered x2, x25,
    x50, x120 and x100 ... with M already at 24, 26, 28, 53, 103 and 223 ...
```
Plus the DECIDING line gains "When (L + C + 3) - M is 0 or less: CHOICE: 0 (pass)". +560 chars.

## A49-2 - RULE 1 timing: your own upkeep is the worst window
**Finding.** Same game: the 323 Vampires were made in the pilot's OWN upkeep (turn 9); seq 27 attackers
listed one creature (`A1. Lord of Lineage`, every token `[summoning sick]`); by seq 29 (turn 11) `Your
battlefield (6 permanents listed, of which 0 are creatures)`. The rule "Fire the chain on the
OPPONENT'S turn" existed; the cost did not. Added after "Fire on your OWN turn only when...":
```
YOUR OWN UPKEEP IS THE WORST WINDOW OF ALL. The 323 Vampires this deck made in its own turn-9
upkeep attacked with ONE creature that turn ... and the opponent's turn 10 cleared all of them
before a single token had attacked ... The line is offered in your upkeep; the answer there is
"CHOICE: 0 (pass)" and the chain fires in THEIR turn.
```
Plus a DECIDING line (own upkeep/main + Alarm out -> pass unless the attackers are already there).
+620 chars.

## A49-3 - RULE 2 Marsh Flats: the dual that raises the colour you count lowest; the row string updated
**Finding.** `...deck123-0x556ee08ba8f0-vs-...deck146.jsonl` seq 5: Underground Sea already on the
battlefield, Idyllic Tutor {2}{w} in hand, fetched Tundra with Underground Sea listed - the printed
order violated, and the better play. 5 fetches this corpus: 3 Sea (correct by either rule), 1 Tundra
(above), 2 Scrubland (vs125 seq 86-87, at 3 life, W/B needed for Doomsayer). The quoted row text was
also stale: the emitter prints `Put a card onto the battlefield with Marsh Flats targeting` (#103).
**Before**: a fixed order Sea > Tundra > Scrubland > basic, quoting "Put in Play with Marsh Flats".
**After**
```
When lines beginning "Put a card onto the battlefield with Marsh Flats targeting" appear, read
the colour counts on the mana line - "sources that can make each: {W} n, {U} n, {B} n" - and
take the DUAL that raises the colour you have the least of, a colour at 0 first: ...
Ties go to Underground Sea, then Tundra, then Scrubland. ... Never a basic while a dual is listed
```
DECIDING line re-keyed the same way. Net +150 chars.

## A49-4 - RULE 3 + cast-list entry 0 + DECIDING: the Damnation row's own `(K able to attack)`
**Finding.** `...deck123-0x55ce89a17d90-vs-...deck126.jsonl` seq 15: `Cast Damnation {right now:
destroys 1 of their creature (0 able to attack), 0 of yours}` cast on a lone 1/1 lifelink Vampire
token with `Cast Tragic Slip` (a 1-toughness target) on the same menu. The deck then held no sweeper
from turn 11 to the end (turn 24, opponent 69 life, 11 creatures). Also seq 11: 0-vs-2 where one of
the two was a 0/4 Wall of Omens - the pilot took Slip + Tribute instead of entry 0, correctly (#109:
a violation whose alternative was better is evidence against the absolute). Lane T's D10 string gives
the rule its key (#49): `{right now: destroys N of their creatures (K able to attack), M of yours}`.
**After** (RULE 3 gains)
```
THE ROW COUNTS FOR YOU. "Cast Damnation" prints "{right now: destroys N of their creatures (K
able to attack), M of yours}". ... at K of 0, or at N of exactly 1, a Tragic Slip or an edict on
the same menu that kills the one that matters is the cast and the Damnation stays in hand. This
deck cast Damnation on a lone 1/1 Vampire token with "Cast Tragic Slip" on the same menu ...
```
Entry 0 now reads "and the Damnation row's "(K able to attack)" reads 2 or more"; the DECIDING
Damnation line is re-keyed to N/K/M. +900 chars.

## A49-5 - RULE 5 re-keyed to the edict row's `{right now: they control N creatures ...}`; N = 1 beats the Slip
**Finding.** Lane T's D10 put the count on the row (`they control 1 creature - Silverquill Silencer
is sacrificed, you gain 2`; `they control 3 creatures - they choose which one`; `at 0 this does
nothing`). Casts this corpus: 5 at N = 1 (correct), 1 at N = 2 (vs126 seq 9, Tribute, "they choose"
- got a 0/3 Pride Guardian). And `...vs-...deck146.jsonl` seq 6-7: `Cast Devour Flesh {right now:
they control 1 creature - Silverquill Silencer is sacrificed}` at 2 mana answered "Cast nothing",
then Tragic Slip at the 3/2 Silencer (toughness 2, no Morbid - shrank it, it lived); it dealt 9
more combat damage in a game lost at -2. #110: the render marks the answer; the rule's first line
is the mark.
**Before**: "(1) find "of which N are creatures" on the OPPONENT'S battlefield line."
**After**
```
(1) read N off the edict row itself - it prints "{right now: they control N creatures - <name>
is sacrificed ...}" at N = 1, "- they choose which one" at N of 2 or more, and "- at 0 this does
nothing" at N = 0. ...
AT N = 1 THE EDICT IS THE CAST, NOT THE SLIP. The row "they control 1 creature - Silverquill
Silencer is sacrificed" was on this deck's menu twice at two mana; it answered "Cast nothing",
then spent a Tragic Slip on that 3/2 ...
```
DECIDING edict line re-keyed to the row's three strings. Note: the row's "you gain N" on DEVOUR
FLESH is false (the targeted player gains - primitive `dynamicability<!mytgt toughnesslifegain
targetcontroller!>`); the guide keeps its true sentence ("You never gain life from Devour Flesh")
and says nothing about the row - the render is the engine's to fix (seat file H1). +700 chars.

## A49-6 - Cast-list entry 3: the Alarm before a second maker
**Finding.** `...deck123-0x563fceb01d00-vs-...deck162.jsonl` seq 15-16, turn 9, 11 life, 5 mana,
Bloodline Keeper on the battlefield line, Intruder Alarm x2 in hand: cast `Thraben Doomsayer
{1}{w}{w}` then `Lightning Greaves {2}`; the Alarm was cast at seq 26 (turn 11, 2 life). Entries 1-2
read "when no token-maker is on your battlefield" and the pilot did not resolve that the condition
was already false (#107: write the predicate the count proves). Two turns of one-token-per-maker
under Fate Unraveler + Underworld Dreams.
**After** (entry 3 gains)
```
    AND THE ALARM GOES BEFORE THE SECOND MAKER. Entries 1 and 2 say "when no token-maker is on
    your battlefield": once one IS there, they are done and this entry is next - before a second
    maker, before Lightning Greaves. With Bloodline Keeper on its line, five mana open and the
    Alarm in hand, this deck cast Thraben Doomsayer and then the Greaves ...
```
Plus a DECIDING line. +620 chars.

## A49-7 - Idyllic Tutor with an Alarm owned: third corpus -> the reply shape (#102/#105/#118)
**Finding.** P5 FAIL 3: `...vs-...deck146.jsonl` seq 10 (Alarm in hand; plan "find Intruder Alarm");
`...vs-...deck125.jsonl` seq 46 and seq 69 (TWO Alarms on the battlefield; plans "the best available
play to dig for a creature", "Cast Idyllic Tutor to find Bloodline Keeper"). At seq 51 and 56 the
pilot argued with the rule in-reply (`answer_replaced`, Tutor -> Cast nothing) and then cast it at
seq 69 anyway. Same wrong belief every time: the Tutor digs for a creature (#106).
**After** (INTRUDER ALARM AND IDYLLIC TUTOR gains)
```
"Cast Idyllic Tutor to find Bloodline Keeper" and "the Tutor is the best available play to dig
for a creature" are sentences this deck wrote with TWO Intruder Alarms on its battlefield line
... the reveal list marked Bloodline Keeper "[does NOT qualify]" both times. ... write this in
your reply, blanks filled from the screen, and take another row:
    Tutor: dead - Alarm <on my battlefield / in my hand>; it finds only an Alarm.
```
DECIDING line gains the shape. Belief-text metric for wave 50: replies containing "Tutor" and
"creature"/"Keeper"/"Doomsayer" as the thing it finds. +620 chars.

## A49-8 - Vision Skeins at a full hand: the engine discards for you
**Finding.** `...deck123-0x55afd8ccf160-vs-...deck152.jsonl` seq 4, turn 3, `Your hand (7 cards)`,
2 mana: cast Vision Skeins; narration before seq 5: `You drew Thraben Doomsayer / You drew Thraben
Doomsayer / ... / You discarded Intruder Alarm` - no ask was routed to the seat (engine item H2). The
deck's only Alarm went to the graveyard on turn 3.
**After** (KEY CARDS, Vision Skeins gains)
```
  COUNT "Your hand (N cards)" FIRST. Your hand goes back to seven at the end of your turn, and
  the engine - not you - chooses what you discard: at N of 7 on turn 3 this deck cast Skeins,
  could cast nothing it drew, and the engine discarded Intruder Alarm. Cast the Skeins at N of 5
  or fewer, or on a turn with the mana to cast what it draws.
```
Plus a DECIDING line. Retire when H2 ships. +480 chars.

## A49-9 - Blocking: a token is not a maker; under DRAW PUNISHERS every life point is a draw
**Finding.** `...vs-...deck162.jsonl` seq 22, turn 10, 11 life: `A1. Fate Unraveler (3/4) deals 3`,
`B1. Human (1/1)`, `B2. Vampire (2/2) [flying]` (both "your blocker dies"), a maker on the
battlefield, `DRAW PUNISHERS ... 2 life` on the screen: `BLOCKS: none`. 11 -> 8; the next draw steps
(seq 23-25) took 8 -> 2. The ladder's last rung ("Otherwise your token-makers do NOT block. Take
the damage") had no line for a free token.
**After** (new rung after it)
```
 - A TOKEN IS NOT A MAKER. With a maker on your battlefield line you remake a token next
   window, so a Human or Vampire in front of the largest "deals N" costs you nothing, and when a
   "DRAW PUNISHERS" line is on the screen every point it keeps is a draw you can afford. ...
```
+480 chars.

---

## Not edited, recorded
- **RULE 4 Tragic Slip**: 4 casts, 3 at toughness 1 (vs146 seq 4 Adventurer 1/1; vs126 seq 11, 36
  Vampire 1/1), 1 at toughness 2 (vs146 seq 7) - covered by A49-5's edict-first line, not re-taught.
- **RULE 0 land drops**: 0 declines in 40 land asks. **RULE -1 / MULLIGAN**: 0 mulligans in 6;
  nothing to edit; the section is already odds-form with a preference-order bottoming (#119).
- **Cast-list entry 5 (Greaves at 0 creatures)**: 0 violations (vs162 seq 4, vs146 seq 8/10/12
  declined; vs125 seq 94 cast at 0 creatures via the parser taking a rejected line - seat file S1).
- **Blockers "no blockers" text (P4)**: 0 of 4 windows wrote it; vs152 seq 12 blocked the lifelink
  4/2 with the Doomsayer at 9 life per the life-minus-4 rule (correct).
