# deck126 (Sanguine Blood) — wave-44 edits, before -> after

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck126_strategy.txt` as of the
matchups-20260826-111937 corpus. Card facts read off the primitives and confirmed against
Scryfall for the four load-bearing ones (Overgrown Battlement, Sanguine Bond, Exquisite
Blood, Savannah — primitive and Scryfall agree in all four cases).

deck126 played close to clean this corpus: 15/15 block windows with 100% of offered
blockers assigned and zero "BLOCKS: none"; 3/3 Vampire attacks taken; 9/9 mana-only
upkeep/main windows passed; 3/3 Idyllic Tutor picks correct. Three of the four edits
below are corrections to statements in the guide that the corpus proved false, not
behaviour fixes.

---

## F1 (HIGH) — "Neither does anything useful alone" is false for Sanguine Bond

**Finding.** deck126 won 4 of 6 games. **Two of those wins had no Exquisite Blood on the
battlefield at any point.**

- `...deck126-...-vs-ai_baka_deck123`, won turn 15 at 15 life. Battlefield at seq 21-22
  carried **Sanguine Bond #1 and Sanguine Bond #2** and no Exquisite Blood. Blocking with
  three walls under a Perimeter Captain produced, verbatim from the narration:
  `You gained 2 life (now 22) / Opponent lost 2 life (now 18) / Opponent lost 2 life (now
  16) / ... (now 24) / ... (now 14) / ... (now 12) / ... (now 26) / ... (now 10) / ...
  (now 8)` — 12 life off the opponent in one combat. Two turns later it finished them.
- `...deck126-...-vs-ai_baka_deck146`, won turn 26 at 28 life. Battlefield at seq 57-58:
  Sanguine Bond, Sorin (11 loyalty), Staff of Nin, Perimeter Captain, Pride Guardian, a
  lifelink Vampire — **no Exquisite Blood**. Opponent 13 -> 7 -> 0 on block triggers and
  Staff pings.

The converse also showed up: `...vs-ai_baka_deck125` had **Exquisite Blood alone** on the
battlefield from turn 9 through turn 38 against a creatureless UW control deck and it did
literally nothing — the opponent never lost life, so the enchantment never triggered. Life
20 -> 0 to Staff of Nin pings over 23 turns.

Primitives (both confirmed on Scryfall):
`Sanguine Bond ... auto=@lifeof(player) ... :life:-thatmuch opponent` / *"Whenever you
gain life, target opponent loses that much life."*
`Exquisite Blood ... auto=@lifelostfoeof(player):life:thatmuch controller` / *"Whenever an
opponent loses life, you gain that much life."*
Bond converts a resource this deck generates on its own (life, 16 walls' worth) into
damage. Blood converts a resource only the OPPONENT can generate. They are not symmetric,
and the guide said they were.

**Before**:
```
=== RULE #3 - ASSEMBLING THE PAIR (your only win condition) ===
Exquisite Blood {4}{B}: whenever an opponent loses life, you gain that much life.
Sanguine Bond {3}{B}{B}: whenever you gain life, the opponent loses that much life.
Neither does anything useful alone. Both together end the game.
```
**After**:
```
=== RULE #3 - ASSEMBLING THE PAIR (your fastest win condition) ===
Exquisite Blood {4}{B}: whenever an opponent loses life, you gain that much life.
Sanguine Bond {3}{B}{B}: whenever you gain life, the opponent loses that much life.
Both together end the game the moment any life changes. But the two halves are NOT the same card:
 - SANGUINE BOND ALONE IS ALREADY A CLOCK. Every point of life you gain takes a point off the
   opponent, and this deck gains life constantly: 2 per defender that blocks with a Perimeter
   Captain out, 3 whenever Pride Guardian blocks, the toughness of whatever Tribute to Hunger eats,
   1 for every point a lifelink Vampire connects with. With a Bond down and three walls blocking
   under a Captain you take 6 off them in one combat, and a SECOND Bond doubles it. Games of yours
   have been won from 20 with a Bond and walls and no Exquisite Blood anywhere. So once Sanguine
   Bond has resolved, keep blocking with everything and keep pinging - you are already killing
   them, and finding Exquisite Blood only makes it instant.
 - EXQUISITE BLOOD ALONE DOES NOTHING you can start yourself: it only pays you when the opponent
   is ALREADY losing life. Against an opponent with no creatures and no burn it is a blank.
So when you have NEITHER piece, Sanguine Bond is the one to look for first.
```
Dependent edit, Rule #3 step 4 — **before**: `if you have neither, pick either.`
**after**: `if you have NEITHER, pick SANGUINE BOND - it is the half that wins on its own
(Rule #3).` (All 3 Tutor picks this corpus happened to pick Bond or the missing half, so
this is a codification of correct behaviour, not a correction of it.)
New DECIDING SITUATIONS line: `Sanguine Bond on your battlefield and no Exquisite Blood:
you are ALREADY killing them. Block with everything, ping with Staff, attack with every
Vampire...`

---

## F2 (MED) — a survival floor above the cast-priority list

**Finding.** `...deck126-...-vs-ai_baka_deck162` seq 14, turn 11, **life 10**, 5 mana
available, land drop already used. Opponent board: Howling Mine, **Fate Unraveler (3/4)**
as the only creature, and Ob Nixilis, the Hate-Twisted at 3 loyalty. Between them the two
permanents were taking 4 life per turn off the model on its own draws (narration turn 11:
`Ob Nixilis ... dealt 1 damage to you (now 13) / Fate Unraveler ... (now 12) / Ob Nixilis
... (now 11) / Fate Unraveler ... (now 10)`).

Menu offered both `Cast Sanguine Bond {3}{b}{b}` (5 mana) and `Cast Tribute to Hunger
{2}{b} - legal targets right now: the opponent` (3 mana). 5 mana pays for exactly one of
them. Exquisite Blood was not in hand and not on the battlefield.

The model took Sanguine Bond, following Rule #7 entry 1 ("take the highest entry on this
list that appears in your casting menu"). Rule #4 already licenses the Tribute ("The
opponent controls exactly ONE creature - then their choice is your choice" / "You need the
life right now to survive the turn") but Rule #7's list is written as absolute, so the two
rules collided and the priority list won. The game ended on turn 12 at 0 life.

Tribute would have taken the Fate Unraveler (the opponent's only creature, so no choice to
make) and gained 4 life — removing half the incoming clock and buying at least two turns
for the Bond that was still in hand.

**Added** (RULE #7, immediately before the closing "Keep casting while the list keeps
offering" line, and given precedence explicitly):
```
7b. THE SURVIVAL FLOOR, and it outranks entries 1 and 2 of this list. When ALL of these are true -
    your life is 10 or less; the opponent battlefield line shows exactly ONE creature; "Cast Tribute
    to Hunger" is in this menu; and you cannot pay for BOTH Tribute and the combo piece out of the
    "Mana available:" line - then cast TRIBUTE TO HUNGER first, even over Exquisite Blood or
    Sanguine Bond. A combo piece you never get to use because you died two turns later bought you
    nothing; the edict removes the creature that is killing you AND gains you its toughness. Cast
    the enchantment next turn - it will still be in your hand.
```
The four conjuncts are all read straight off the render, and the mana clause keeps the
floor from firing whenever both plays fit. DECIDING SITUATIONS updated to match.

---

## F3 (MED) — card-fact error: Savannah does not make black

**Finding.** The live guide's Chromatic Lantern entry read *"Your Bayous, Scrublands,
Savannahs and Urborg already make black"*. **Savannah is `type=Land / subtype=Forest
Plains`** — {G}/{W} only. Scryfall agrees: `Savannah — Land — Forest Plains — ({T}: Add
{G} or {W}.)`. Sunpetal Grove is likewise {G}/{W} (`auto={T}:Add{G}` / `auto={T}:Add{W}`).

deck126's actual black sources are Bayou x4 (Swamp Forest), Scrubland x4 (Plains Swamp),
Woodland Cemetery x2, Isolated Chapel x2, Swamp x1, and Urborg x1 (which makes every land
a Swamp while it is out) — 14 of 24 lands, not 13 plus four Savannahs.

The error mattered: in `...vs-ai_baka_deck125` the model's turn-7 board was Savannah,
Plains, Forest, Scrubland and it correctly cast Chromatic Lantern; a guide that says
Savannahs make black is arguing against that play.

**Before**: `Your Bayous, Scrublands, Savannahs and Urborg already make black; when {b}
is on that line the Lantern does nothing for 3 mana.`
**After**: `The lands of yours that make black are Bayou, Scrubland, Woodland Cemetery,
Isolated Chapel, the Swamp and Urborg. SAVANNAH AND SUNPETAL GROVE DO NOT - they are
{G}/{W} only - so an opening of Savannahs, Sunpetal Groves and Plains is exactly the board
where this card earns its slot. When {b} is already on the "colours you can make:" line
the Lantern does nothing for 3 mana.`

---

## F4 (MED) — Overgrown Battlement's real text, and what "Mana available:" counts

**Finding.** The guide describes the upkeep offer as "tapping Overgrown Battlement for
green mana" and says the "Mana available:" line "already counts the Battlement in with
your lands". Both are true but incomplete, and the corpus shows the gap.

Primitive: `auto={T}:foreach(creature[defender]|myBattlefield) add{G}` /
`text=Defender -- {T}: Add {G} for each creature with defender you control.` Scryfall:
*"{T}: Add {G} for each creature you control with defender."* The corpus offered both
`Add 1 green mana with Overgrown Battlement` and `Add 2 green mana with Overgrown
Battlement` as the wall count grew.

And the render is explicit that the total under-counts it —
`...deck126-...-vs-ai_baka_deck125` seq 17:
```
Mana available: 8 total (eight untapped sources, ...)
  Those sources, one per untapped card: ... Overgrown Battlement {g} (VARIABLE output: this ONE
  source adds more than one mana - its amount is a count, read its own rules text; the total above
  counts it as one source, not as its mana)
```
So the "Mana available:" number is a floor whenever a Battlement is untapped. The guide
told the model the number was complete.

**Before**: `... and the "Mana available:" line already counts the Battlement in with
your lands.`
**After**: `... and the "Mana available:" line already counts the Battlement in with your
lands - as ONE source. Overgrown Battlement reads "{T}: Add {G} for each creature with
defender you control", and the engine says so on its own line in the sources list
("VARIABLE output: ... the total above counts it as one source, not as its mana"). So the
"Mana available:" number is a FLOOR: your real mana is that number plus one for every
defender past the first. When a spell looks one or two mana out of reach and you have
three or four walls out, it is probably affordable - take it and let the engine tap for
you.`

RULE #1 itself (pass every upkeep Battlement offer) is UNCHANGED — 9/9 mana-only windows
passed correctly this corpus.

---

## Deliberately NOT changed

- **RULE #5 (blocking).** 15/15 block windows assigned every blocker offered, zero
  "BLOCKS: none", including 5 windows where the header's "you would be at N" was 8 or
  less. The D9 survival floor from wave 43 held perfectly; nothing to add.
- **RULE #6 (attacking).** 3/3 Vampire attack windows taken; the one-stop clause never
  triggered (no Vampire ever died before combat damage). Untested, not wrong.
- **The MULLIGAN section.** 6 keeps out of 6 offers, kept-land counts 4/3/4/5/3/4. No
  mulligan, no bottoming, no floor pressure. Nothing to learn from yet.
- **RULE #7a.** Discharged again: `...vs-ai_baka_deck152` seq 17-18 — with both
  enchantments out the model cast Staff of Nin and pinged for the win on turn 13, at
  opponent life 18 -> 0. The rule works as written.
