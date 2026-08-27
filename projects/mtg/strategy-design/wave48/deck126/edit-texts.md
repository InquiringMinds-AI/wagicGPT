# deck126 (Sanguine Blood) - wave-48 edit texts

Base = live `projects/mtg/bin/Res/ai/baka/deck126_strategy.txt` (wave-47 F1-F4 + boundary pass).
Output = `wave48/deck126/strategy.txt` (31,033 -> 32,684 bytes). Corpus root
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260827-072004/`, seat files
`...-ai_baka_deck126-0x...-vs-ai_baka_deckN.jsonl`. Numbering continues wave 47 (F1-F4).

## F5 - TRIBUTE AT "1 is a creature": "1" means "only", and it means NOW
Finding: D29 FAIL 0/2. Both windows the header read `of which 1 is a creature` with Tribute on the
menu, and both replies deferred with the same sentence: `vs146` seq 18 (t9) cast Sanguine Bond -
"Keep Tribute to Hunger in hand to remove the Silverquill Silencer if it becomes the opponent's
only creature" (it WAS their only creature); `vs123` seq 9 (t6) cast Overgrown Battlement - "Remove
Thraben Doomsayer with Tribute to Hunger if it becomes the only creature" (it was). The second is
the loss: Doomsayer (`{T}: Put a 1/1 white Human creature token`) + Intruder Alarm made 255 Humans
by t11 and the seat died at -226 (seq 15-16). The executor reads the rendered "1" and still treats
"only creature" as a future condition. STRATEGY against a complete render (the header is exact and
the creature's text is on the line).
BEFORE (Rule #4): "...CAST IT IN THAT WINDOW, ahead of Sanguine Bond, ... Five windows this corpus
met that condition and you took none of them: ..."
AFTER: "+ READ '1' AS 'ONLY': 'of which 1 is a creature' means the creature printed on that line IS
their only creature, right now. It does not have to 'become' their only creature ... Twice this
corpus you wrote 'keep Tribute to remove it if it becomes their only creature' about a creature the
header had just counted as their only one ... A creature whose text says '{T}: Put a ... token' or
'{T}: Create' is the sharpest case of this rule - it IS the engine..." (earlier-corpus anecdotes kept,
re-dated).
BEFORE (7b): "...whether or not you could also pay for the enchantment. One creature means THEIR
choice is YOUR choice ... It went 0 for 5 this corpus ... The two shapes below ..."
AFTER: "+ The header reading 1 IS the 'only creature' test: it is met NOW, in this window, not 'if it
becomes' their only creature. ... 0 for 5 one corpus and 0 for 2 the next ... The three shapes
below ..." with a new shape (c): a lone creature whose text says "{T}: Put a ... token" / "{T}:
Create" (Thraben Doomsayer, Bloodline Keeper).
BEFORE (situations): "...One creature means their choice is your choice. This is the line you missed
five times."
AFTER: "...and '1' means it is their only creature NOW - not 'if it becomes' the only one. This is the
line you missed five times in one corpus and twice in the next, once against the Thraben Doomsayer
that made 255 Humans."

## F6 - CHROMATIC LANTERN: drop the {B} clause; it is a mana rock cast when nothing above it is listed
Finding: D33 hold-test "FAIL" 3/3 - every Lantern cast was at `{B} 2` or more (`vs130` seq 9 at
{B} 2, `vs152` seq 11 at {B} 3, `vs162` seq 9 at {B} 2) - but reading the menus overturns the rule,
not the play: at `vs130` seq 9 and `vs162` seq 9 the only other row was Tribute at a 0-creature
header (forbidden), so Lantern was the only real play; at `vs152` seq 11 the alternative was Sorin
from exile at {4}{b}{w} (Elite Spellbinder tax, `leaves 1`). Two of the three games were won (t18,
t15). A card that fixes every colour AND adds a mana is not "a wasted turn" on a turn with nothing
else to cast; the {B}-clause rule was blocking the correct default. Second-Lantern half held: 0
second casts with one on the line (14 windows).
BEFORE: entry 7 with conditions (a) "{B} 1 or no {B}" and (b) "no Lantern on the battlefield line",
plus the Urborg paragraph and "You have cast this card five times and every one of them was at {B}
2 or more...".
AFTER: "7. Chromatic Lantern - when nothing from entries 1-6 is on the menu AND no Chromatic Lantern
is on your battlefield line. It is a mana rock ... three times this corpus it was the only real row
beside 'Cast nothing right now', you cast it, and two of those games you won. Do NOT count black
sources by name to decide this; the entry above it on this list decides it. A SECOND Lantern does
nothing at any time..."
BEFORE (situations): "'Cast Chromatic Lantern' is in your menu: read the '(sources that can make
each: ...)' clause ... Cast it only if that clause reads '{B} 1' or shows no {B} at all..."
AFTER: "'Cast Chromatic Lantern' is in your menu, nothing from entries 1-6 of Rule #7 is beside it,
and no Chromatic Lantern is on your battlefield line: cast it. A wall or a combo piece on the same
menu, or a Lantern already out: take that instead."

## F7 - BOTH ENCHANTMENTS OUT: send every Vampire, blocked or not
Finding: `vs130` seq 32 (t18): both enchantments on the battlefield, one Vampire offered against
`their untapped blockers: Siege-Gang Commander (2/2) (your attacker dies...); Goblin #1 (1/1) (both
die...); ...` - four untapped blockers. The live rule says "send if their side is empty of untapped
blockers" and the N >= 2 stop says ATTACK: none. The seat sent it and won on the spot (gameend seq
33, 37 life to 0). Lifelink gains life on damage dealt to a blocker, so the loop starts either way;
the rule's "empty side" condition was wrong. Verified: Sorin token `token(Vampire,...,lifelink)`;
the win arrived in that combat.
BEFORE: "The exception is the win button: with BOTH enchantments on your battlefield, one point of
lifelink damage connecting ends the game, so send if their side is empty of untapped blockers."
AFTER: "The exception is the win button, and it outranks every stop above and below: with BOTH
enchantments on your battlefield, send EVERY Vampire whatever the tag says. Lifelink gains you the
life for damage dealt to a BLOCKER too, so a blocked Vampire still starts the loop ... You sent one
into four untapped Goblins and a Siege-Gang Commander and won on the spot."
BEFORE (situations, N >= 2 line): "...ATTACK: none for that Vampire - it is worth more as a lifelink
blocker."
AFTER: "+ UNLESS both enchantments are on your battlefield: then send every Vampire, blocked or not -
lifelink damage to a blocker starts the loop."

## F8 - STAFF PING WITH BOND ALONE: it is damage, not a trigger
Finding: `vs146` seq 43 (t19 upkeep, opponent at 7): the reply's first line was `CHOICE: 7 (Deal 1
damage with Staff of Nin targeting the opponent)`, then 4.9 KB of reasoning concluding "Staff ping
does nothing for me unless I have Exquisite Blood ... So Staff ping is useless", ending `CHOICE: 0
(pass)` - which E1 took. Same reasoning at seq 46 (chose Sorin +1 over the ping). One free damage
per window forgone at 7 life; the game was still won at t20. The guide says "always aim it at the
opponent" but explains the ping only through the enchantments, which is what the model reasoned
from. STRATEGY (false world-model belief: a ping "needs" a trigger).
BEFORE: "...With Exquisite Blood down that damage is life for you; with both enchantments down it
wins the game on the spot."
AFTER: "+ With Sanguine Bond ALONE it is still ONE DAMAGE - a point off their total every upkeep,
which is the whole game when they are at 7. It is damage, not a trigger, so 'the ping does nothing
without Exquisite Blood' is a false thought: you reasoned exactly that with the opponent at 7 and
passed the window with the ping on the menu."

## F9 - TWO MENACE ATTACKERS, TWO BLOCKERS
Finding: `vs146` seq 24 (t12): A2 and A3 both `[menace ...]`, B1 and B2 two Walls of Omens; answer
`BLOCKS: B1:A2, B2:A3` -> `all_assignments_illegal` (the seat's only fallback; Baka blocked). The
existing rule says two-or-none per attacker; the executor satisfied it per attacker and split the
pair. STRATEGY.
BEFORE: "...Two 0/4 walls on a menace attacker is the cheapest block in this deck - ... both still
trigger their life gain."
AFTER: "+ TWO menace attackers and TWO blockers: both blockers on ONE of them, never one each - one
each is two illegal blocks, the engine rejects the whole answer and Baka blocks for you. You
answered two menace Spiders with one Wall each and lost the decision."

## Considered and NOT edited (no bytes spent)
- **D30 (Tribute at 0 creatures, `vs130` seq 25)** - NOT a guide failure: the model's first coded
  line was "CHOICE: 2 (Cast nothing right now)" and its re-answer named a card not on the menu
  ("CHOICE: 1 (Cast Sanguine Bond)"); E1 executed index 1 = Tribute. Engine item HIGH #1 in the seat
  file. The zero stop held in the model's own text.
- **Mulligan: a 1-land seven kept at `vs130` seq 1** (`1 land, 6 spells`, "would not cover any
  spell"). The guide's KEEP rules do not cover it and it has no ship rule; the seat won at t18 with
  land drops on t2, t3, t6 (Lantern t6). One window, a win, and the brief's new-baseline rule - not
  edited; recorded.
- **Pride Guardian chump at `vs146` seq 11** (t6, life 20, "your blocker dies", no enchantment) -
  STEP 1 already benches it; 1 of 8 blockers windows; the anecdote in STEP 1 is that exact shape.
  Not re-worded.
- **`vs152` seq 9: second Pride Guardian left out of a `neither dies` block** (3 life forgone) -
  STEP 2 already says leftovers gang up; one window.
- **Unpaid `Cast nothing` (D34 4/21)** - `vs125` seq 39/41 Pride Guardian declined with both
  enchantments out on the turn the game was won; `vs130` seq 18 Sorin declined (tap-out) the turn
  before the combo; `vs152` seq 7 Battlement declined at 2 mana. None cost a game; under the <= 4
  threshold.
- **Wall of Omens `[DRAW PRICE:]` at 11 life (`vs162` seq 13)** - 2 life under Dreams + Unraveler in
  a game lost to the draw lock; a rule would be one line for one window in a lost matchup.
