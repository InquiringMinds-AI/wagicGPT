# deck146 -> development notes (wave 32)

Corpus `matchups-20260727-115847`, binary 7fabd9bd0. Seat volume 164 records / 153 model calls,
**0 fallbacks**, **0 crash signatures** in all six deck146 stderr files (i16 confirmed: no `FATAL:`
AIStats line for any of the three new decks at this seat's games).

Four new items (one HIGH engine with a root cause traced to source, two HIGH render, one probe
request), plus dispositions on the carried ledger.

---

## N-146j (NEW, **HIGH**, ENGINE) -- `[-multicolor]` target predicate excludes ARTIFACTS and LANDS

**Symptom.** Vanishing Verse (`target=*[-multicolor]|battlefield`, `borderline.txt`) reports no
legal opponent-side target against a board of monocolored black artifact creatures, and has never
once offered a land as a target in 21 games.

**Repro (deterministic, from the corpus).** `ai_baka_deck146 vs ai_baka_deck36`, seq 17, 19 and 23,
all turn 10. Option line, verbatim:

```
1. Cast Vanishing Verse {b}{w} - the only legal targets are YOUR OWN right now (warning: this
   would harm your own side - only correct if you are deliberately triggering something)
   {card text: "Exile target monocolored permanent."}
```

Opponent battlefield in the same prompt, verbatim:

```
Opponent battlefield (creatures: 4): Tolarian Academy [tapped]; Mox Sapphire [artifact]; Mox Jet
[artifact]; Ornithopter (0/2) [artifact] [flying]; Glaze Fiend {1}{b} (0/1) [artifact] [flying]
[tapped - cannot attack or block this turn]; Seat of the Synod #1 [artifact]; Salvage Slasher
{1}{b} (1/1) [artifact] [tapped - cannot attack or block this turn]; Great Furnace [artifact];
Yotian Soldier {3} (1/4) [artifact] [vigilance]; Vault of Whispers [artifact]; Seat of the Synod #2
```

`Glaze Fiend` and `Salvage Slasher` are `mana={1}{B}`, `type=Artifact Creature` (`mtg.txt`) --
monocolored black, and legal targets for "Exile target monocolored permanent" under both Oracle and
the primitive's own `[-multicolor]` predicate.

**Root cause (traced, not hypothesised).** `src/TargetChooser.cpp:1066-1074`:

```cpp
else if (attribute.find("multicolor") != string::npos)
{
    if (minus)
    {
        cd->setisMultiColored(-1);
        cd->SetExclusionColor(0);//not multicolored is monocolored not colorless, use iscolorless attribute
        cd->SetExclusionColor(6);//restriction... green, red, blue, black or white colored only
        cd->mode = CardDescriptor::CD_OR;
    }
```

`MTGDefinitions.h:100-107`: `MTG_COLOR_ARTIFACT = 0`, `MTG_COLOR_WASTE = 6`, `MTG_COLOR_LAND = 7`.
`src/CardPrimitive.cpp:530-535`:

```cpp
case Constants::MTG_COLOR_LAND:
    value = kColorBitMask_Land;
    break;
case Constants::MTG_COLOR_WASTE://the true colorless mana shares the kbitmask of land. ...
    value = kColorBitMask_Land;
    break;
```

So `SetExclusionColor(0)` sets `kColorBitMask_Artifact (0x01)` and `SetExclusionColor(6)` sets
`kColorBitMask_Land (0x40)` -- **the same bit lands carry**. `CardDescriptor::match_and`
(CardDescriptor.cpp:293-297) rejects on ANY intersection:

```cpp
if (mColorExclusions)
{
    // if any of forbidden colors intersect with card colors
    if ((mColorExclusions & card->colors) != 0)
        match = NULL;
}
```

Net: `[-multicolor]` rejects every artifact and every land regardless of colour. The comments state
the intent was to exclude COLORLESS; `MTG_COLOR_WASTE` sharing the LAND bitmask defeats it, and
using colour index 0 (ARTIFACT) as a colorless proxy defeats it again.

**Second, independent corroboration.** Corpus-wide, the ONLY artifact ever listed as a Verse target
is a **Clue token** (deck152 seq27 `legal targets right now: Brutal Cathar, Clue`) -- token
creation does not set the artifact colour bit, so tokens slip the same filter that blocks artifact
CARDS. And **no land appears in any Verse target list in any of the 21 games.**

**Blast radius.** Every primitive using `[-multicolor]`, not just Vanishing Verse. Worth a grep of
`Res/sets/primitives/*.txt` before fixing, since the fix changes legality for all of them.

**Suggested direction (not a patch).** The predicate needs a real "is colorless" test rather than
two colour-bit exclusions: keep `setisMultiColored(-1)` and replace the two `SetExclusionColor`
calls with a colorless check that does not consult the artifact or land bits. The comment on line
1072 already names the correct mechanism ("use iscolorless attribute"). Note `CardGui.cpp:1822-1829`
carries the identical block and will need the same treatment or the card-browser filter will
disagree with the game.

**Cost this corpus.** deck146 lost vs36 at turn 11 holding three Vanishing Verses against a board
where every non-land permanent was an artifact. Guide corrected in the meantime (strategy.txt now
tells the pilot Verse cannot hit artifacts or lands and to route through Soul Shatter / Kaya) --
that teach becomes a CRUTCH the moment this is fixed, and should be demoted then.

---

## N-146k (NEW, **HIGH**, RENDER/HARNESS) -- the pre-game mulligan prompt renders a battlefield mana line

**Symptom.** Every pre-game mulligan ask carries `Mana available: 0 total (no untapped sources)`,
describing a battlefield that does not exist yet. It is 0 in every opening hand ever dealt, so it
carries no information, and the pilot reads it as a statement about its HAND.

**Repro.** `deck146 vs deck139`, seq 1 (the opening seven), prompt verbatim:

```
--- CURRENT SITUATION ---
Phase: Untap | It is your turn.
Your life: 20 | Opponent life: 20
Mana available: 0 total (no untapped sources)
Your hand: Hive of the Eye Tyrant (land: taps for {B}); Plains (land: taps for {W}); Swamp (land:
taps for {B}); Triumphant Adventurer {b}{w} (1/1) [creature]; Vanishing Verse {b}{w} [instant];
Vanishing Verse {b}{w} [instant]; Vanishing Verse {b}{w} [instant]
Your battlefield (creatures: 0): (none)
```

Reply verbatim: *"This hand has zero lands that can produce mana **right now** (all three are
lands, but you haven't tapped them yet, and you have no mana in your pool)."* -> Mulligan.

Five occurrences of the same false conclusion in 14 mulligan replies at this seat, and the phrasing
tracks the rendered line: deck158 seq4 (*"zero lands that can produce mana right now"*), deck139
seq1 / seq3 / seq4 / seq6 (*"This hand has zero lands"*, each with 1-3 lands listed).

**Cost.** Contributory to two seven-mulligan chains ending in **0-card opening hands** (vs158,
vs139), both losses. Combined with the missing floor in the guide (fixed this wave in
strategy.txt), this is the seat's largest loss driver of the corpus.

**Suggested direction.** Suppress the `Mana available:` line when there is no battlefield / when
the decision kind is the pre-game mulligan. It is the one seam where the count-first mana line
(i12 / N-158g) is not merely useless but actively misleading. If suppression is awkward at the
assembler, the cheap alternative is to make it seam-honest -- e.g. omit it whenever
`Your battlefield (creatures: 0): (none)` and the phase is the pre-game mulligan.

---

## N-146l (NEW, **HIGH**, RENDER) -- the player's POISON COUNTER total is never rendered

**Symptom.** deck146 lost `vs deck105` **at 21 life** on turn 15 to the infect alternate win, with
no line anywhere in any prompt stating its own poison-counter count.

**Repro.** `deck146 vs deck105`. `gameend` record: `{'won': False, 'draw': False, 'my_life': 21,
'opp_life': 6, 'turn': 15}`. Searching all 30 records of that game, "poison" appears only inside
reminder text riding option lines:

```
A1. Corpse Cur (2/2) deals 2 [infect] {text: Infect (This creature deals damage to creatures in
form of -1/-1 counters and to players in form of poison counters ...
```

The board header, the life line and the narration all omit the total. Infect damage narrates as
plain damage with no life change (turn 14, verbatim):

```
- Phase: Combat damage
- Damage: 1 dealt by Plague Stinger to you
- Damage: 2 dealt by Corpse Cur to you
```

No `Counter added to you: poison`, no running total; `Your life` stays 21 for the whole game.

**Suggested direction.** Two lines, both in the situation block: a poison total beside the life
totals whenever either player has >0 (`Your poison: 7 of 10 - you LOSE at 10`), and a narration
event when poison counters are added. deck105 is the natural owner seat for the fix; this is an
independent second witness from the receiving side, and it decided a game.

---

## N-146h-probe (REQUEST) -- Agadeem's Awakening needs a PROBE, not another corpus

**Status.** i3 was arrival-traced at this seat and is **UNEXERCISED**: 0 Cast option lines, 0 cast
attempts, 0 card-face menus. The single copy appeared only in pre-game hands (deck158 seq2, deck105
seq4, deck139 seq2/seq3) and was shuffled or bottomed each time. The N-146h second layer is
therefore neither confirmed nor refuted, and the guide's "leave Agadeem's Awakening alone" crutch
cannot be demoted.

**Why a probe, not a fifth corpus of waiting.** The card is a 1-of costing `{B}{B}{B}{X}`; across
wave-30 to wave-32 it has reached a castable window exactly once (wave-31 vs158, where it burned
three turns of mana silently). The random pool will not discharge this.

**Probe shape** (per the campaign probe-deck pattern): stack a probe deck in a numbering gap
(verify the contiguous roster ends below first) with 4x Agadeem's Awakening, ~12 cheap black
creatures to fill a graveyard, and a heavy black manabase; copy `deck146_strategy.txt` as the
probe's guide with the "do not cast Agadeem's" line REMOVED so the pilot will actually try it; run
2-3 one-shot GPT games with `timeout 3000`. **Pass criterion (arrival-trace, not counters):** for
every announced X, either a `hand -> stack -> resolution` chain with creatures reaching the
battlefield, or a LOUD failure in the narration. A silent no-op with mana consumed is the failure
this is looking for. Delete the probe decks afterwards and verify the roster's highest deckN is
back to the real value; preserve decklists + translogs + report under
`strategy-design/wave33/probe-n146h/`.

---

## OBSERVATION (ledger only, no action) -- answer-first protocol violation absorbed by the parser

`deck105 seq23 t14`, `kind=ask`, `latency_ms=106660`, no fallback. The reply opens
`CHOICE: 2 (Cast nothing right now)`, deliberates at length, genuinely reverses on the merits, and
closes `CHOICE: 1 (Cast Acererak the Archlich {2}{b} (5/5))`. The record shows `choice=1` -- the
parser resolved to the model's final, reasoned intent, which was the correct answer. Recording the
shape because it is a leading-CHOICE-then-reversal that did NOT become a `retracted_choice`; no
defect and no change proposed.

---

## OPTIONAL render proposal -- annotate the attack tax on the attackers seam

Not filed as a defect; the guide fix ships first and wave-33 will say whether prose suffices.

The attackers block lists `Your creatures that can attack: A1. Silverquill Silencer (3/2)` with no
statement that declaring an attacker taps it. At `deck146 vs deck36 seq21 t10`, at 2 life, the
pilot attacked with all three of its untapped bodies and, **in the same reply**, planned to block
with them on the crack-back:

> PLAN: Attack with all three creatures (Silverquill Silencer and both Spiders) ... In the
> opponent's upcoming turn, I will block their attackers to survive: use the Spiders (with reach)
> to block Glaze Fiend and Salvage Slasher, and use Silverquill Silencer to block Yotian Soldier.

It died to that crack-back. Third campaign seat with this shape (deck122 and deck148, wave-31).
If prose does not close it, the cheapest render is a per-creature tail on the attacker lines --
`(attacking taps it; it cannot block next turn)`, omitted for vigilance -- which keeps the
exemption expressible, unlike a global rule.

---

## CARRIED LEDGER -- dispositions at this seat

- **N-146g** (planeswalker magnitude skip): retired wave-31; 0 `{right now: ...}` annotations on
  any walker line this corpus. Stays retired.
- **N-146h layer 2**: OPEN, unexercised -- see N-146h-probe above.
- **N-152a** (MDFC back-face land placement): still live. Zero MDFC land-drop options offered at
  this seat across two corpora; the guide now covers Emeria's Call, Agadeem's Awakening AND
  Pelakka Predation (the third card in the class, missed by the wave-31 revision).
- **N-158e** (no flash windows outside main phases on your own turn): not exercised at this seat --
  this deck's only instants are Verse and Soul Shatter and both were cast at sorcery speed on its
  own turns or at priority on the opponent's.
- **i1 / N-152c / N-122b** (generic-fill colour assignment): PASS at this seat. Mana lines showed
  `colours you can make: {b}{w}` from dual sources and every 2-pip spell in hand had its Cast line
  offered; zero castable-but-not-offered instances.
- **i5** (loud re-ask): zero `validation_reject_reask` / `_exhausted` markers -- no validation
  reject occurred at this seat, consistent with the corpus-level note.
- **i6** (truncation salvage): zero `truncated_abandoned_heuristic`; no truncated replies.
- **i7** (London bottoming): VALIDATED here -- two 7-mulligan keeps each produced exactly 7 bottoms
  with constant labels and an `EXACTLY 7` demand in the prompt. Mechanically correct.
- **i8** (combat tags): renders correctly (101 / 87 / 33 occurrences of the three tag strings); two
  residual stock-phrase paradox verbalizations, neither costly -- see findings (h) and skill.md P3.
- **i16** (AIStats missing-file no longer FATAL): confirmed clean across all six stderr files.
