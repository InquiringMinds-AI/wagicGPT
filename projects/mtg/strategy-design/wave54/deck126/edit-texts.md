# deck126 (Sanguine Blood) - wave-54 edits, before -> after

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck126_strategy.txt` (53,926 B). Revised:
`wave54/deck126/strategy.txt` (56,650 B). Pool band 39.6-70.2 KB: in band. **Three edits** - every
wave-53 prediction (D92-D98) PASSED, so there is nothing else this corpus paid for.

Corpus root `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-034355/`; seq references are the
deck126 seat log of the named matchup.

## 1. D100 taps-you-out is not a warning

**Finding.** D100 - 10 live first-appearances of a missing-half `Cast` row, **8 taken**. Both declines are the same string: `126v146` seq 14 (t12, 25 life, declined `Cast Sanguine Bond ... casting this taps you out` for a Perimeter Captain at `leaves 4`; the Bond arrived t18, six turns late, and the game was lost by 5 life at t33) and `126v162` seq 11 (t9, same string, took an Idyllic Tutor; Bond came t11, game won anyway). Per #193 the fix keys the rule to the clause the pilot is actually reading and adds the #174-style always-false negative, because the rule already NAMED the clause and was broken on it twice.

BEFORE:
```
 1. Exquisite Blood or Sanguine Bond - whichever NAME is missing from your battlefield line, at any
    "leaves N", including "casting this taps you out". A copy of a name already on that line is
    not this entry; skip it. Nine of nine one corpus - the turn the row appeared, it was cast.
```

AFTER:
```
 1. Exquisite Blood or Sanguine Bond - whichever NAME is missing from your battlefield line, at any
    "leaves N", including "casting this taps you out". A copy of a name already on that line is
    not this entry; skip it. Nine of nine one corpus - the turn the row appeared, it was cast.
    "CASTING THIS TAPS YOU OUT" IS THE STRING YOU BREAK THIS RULE ON, AND IT IS NOT A WARNING ABOUT
    THIS CARD. It is a mana note, and this deck holds nothing it wants to cast after the half
    anyway - you have no counterspell, no instant but Tribute to Hunger, and a board of defenders
    that blocks whether your lands are tapped or not. The thought "I will cast it next turn with a
    land more behind it" is ALWAYS FALSE here: the half you did not cast is the half that gets
    countered, exiled or drawn past. Twice last corpus you read that clause and took a wall or a
    Tutor instead - once at 25 life, where the Sanguine Bond then arrived SIX TURNS LATE and you
    lost that game by five life; once at 9 life, where it cost you two turns of a game you won
    anyway. When the row is entry 1 and the row says "casting this taps you out", CAST IT.
```

## 2. D101 the [named:] tag

**Finding.** D101 - Silverquill Silencer `borderline.txt:102764` (`auto=chooseanameopp transforms((,newability[@movedto(*[chosenname]|opponentstack):life:-3 opponent && draw:1 controller]))`). The render puts the choice on the OPPONENT battlefield line as `Silverquill Silencer {b}{w} (3/2) [named: Exquisite Blood]` - **23 records** in `126v146`. The seat cast the named card **2 of 2** times it cast it at all: seq 42 (t26, 24 -> 21) and seq 61 (t32, **8 -> 5**, opponent on 5, four attackers, its own Staff of Nin already lethal in five turns). It died at -5 against 5. The guide had no rule for the tag and Rule #3 entry 5 ("tutor for the next copy and cast it again - there is no other way to win this game") licensed the second cast. #195: the new rule names the non-members too (a Lantern, a Tutor, a wall) so the class does not generalise outward. Sized to one bullet, not a section, because the same tag sat unread and harmless on 19 Lantern records and 12 deck125 Essence Scatter records.

BEFORE:
```
 - "Cast Idyllic Tutor" carries "[finds only an enchantment card - every enchantment left in your
   library is a copy of one you already control or hold: ...]": the Tutor is dead this window. Skip
   it and take the next Rule #7 entry that appears.
```

AFTER:
```
 - THE OPPONENT BATTLEFIELD LINE PRINTS "[named: <card>]" AFTER ONE OF THEIR CREATURES (Silverquill
   Silencer does this as it enters): every time YOU cast a card with that exact name, YOU LOSE 3
   LIFE AND THEY DRAW A CARD, before your spell does anything. The tag is on their battlefield
   line, not on your row, so your cast row will not price it - subtract the 3 yourself, from the
   life total the prompt prints, before you choose. Then:
     * The named card is Exquisite Blood or Sanguine Bond and casting it COMPLETES the pair on your
       battlefield line this turn: pay the 3 and cast it. The pair ends the game; 3 life is nothing.
     * The named card is a half that does NOT complete the pair (the other half is not on your
       battlefield line and not on your hand line with a Cast row this same turn): DO NOT CAST IT
       while your life is 12 or less. Take the next Rule #7 entry - a Perimeter Captain, a wall, a
       Staff ping - and cast the half on a turn when the other one lands with it. You cast Exquisite
       Blood into "[named: Exquisite Blood]" twice in one game, at 24 life and again at EIGHT life
       with four of their creatures attacking and their total on 5; you paid 6 life for two
       enchantments that were exiled the turn they landed, and you lost that game at -5 against 5.
     * The named card is a Chromatic Lantern, an Idyllic Tutor or a wall: it was never worth 3 life.
       Skip it entirely while the tag is printed.
   A named card is the only thing in this deck that costs life to CAST. Read their battlefield line
   for the tag at every cast ask, the way you read your own for the two enchantment names.
 - "Cast Idyllic Tutor" carries "[finds only an enchantment card - every enchantment left in your
   library is a copy of one you already control or hold: ...]": the Tutor is dead this window. Skip
   it and take the next Rule #7 entry that appears.
```

## 3. D102b reveal-pick ceiling refresh

**Finding.** D102b - Idyllic Tutor reveal picks **10 of 10** correct (wave 53: 7 of 9), including both "neither half" looks taking Sanguine Bond per Rule #3.4. The step now records a ceiling instead of two old misses.

BEFORE:
```
You picked
    a Sanguine Bond with a Sanguine Bond on your battlefield and an Exquisite Blood in your hand,
    and again with both halves sitting in your hand.
```

AFTER:
```
You picked
    a Sanguine Bond with a Sanguine Bond on your battlefield and an Exquisite Blood in your hand,
    and again with both halves sitting in your hand. Then you got ten of ten right in one corpus,
    including both "neither half" looks. This step is at its ceiling: read the two lines, name the
    other card, and do not re-derive it.
```
