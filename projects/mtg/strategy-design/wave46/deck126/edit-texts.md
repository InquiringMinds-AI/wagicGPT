# deck126 (Sanguine Blood) - wave-46 edits, before -> after

Corpus `matchups-20260826-155842`, seat files `*ai_baka_deck126-0x*`. 211 decisions / 6 games,
**5W-1L**. Card facts read off `projects/mtg/bin/Res/sets/primitives/mtg.txt` with `/usr/bin/grep`.

Six edits. RULE #0 (the land drop) is UNTOUCHED: 17/17 windows taken, 0 declines, D16 PASS. RULE #1's
upkeep pass and the coloured-pip paragraph are untouched too - see the D17/F7 note below.

---

## F1 - RULE #5 BLOCKING: the survival test becomes STEP 1, above "assign every blocker"
FINDING, and it is the whole of this seat's only loss (`...deck126-...-vs-ai_baka_deck146.jsonl`,
lost turn 10 at -8 from 20 life, nine decisions in the game). Two blocks, both fully rendered,
both against an explicit line in this guide:
 - **seq 5, turn 4, life 20.** `A1. Triumphant Adventurer (1/1) deals 1 [first strike, deathtouch]`
   / `B1. Perimeter Captain (0/4) [defender] [deals 0 - this block kills nothing, but it STOPS all
   1 damage from reaching you] - may block A1 (your blocker dies, attacker lives (blocking trigger:
   you may gain 2))`. Header: `you would be at 19 - NOT lethal`. Neither enchantment out, so the
   guide's carve-out says leave this blocker OUT. It blocked. The Captain - the card that turns
   every future block into 2 more life - died to a 1/1 to stop ONE damage.
 - **seq 8, turn 6, life 21.** `B1. Pride Guardian (0/3) ... - may block A1 (your blocker dies,
   attacker lives (blocking trigger: you gain 3)), A2 (neither dies (blocking trigger: you gain
   3))`. The guide's tiebreak is explicit ("Pick, for that blocker, an attacker whose parenthesis
   says your blocker LIVES ... ahead of one that says it dies"). It took A1. The Guardian died.
By turn 8 the board held no blockers at all and 10 damage came through in one swing (21 -> 11).
PERCEPTION vs STRATEGY: **STRATEGY**; every parenthesis was printed.
DIAGNOSIS: an ordering defect, not a content gap. RULE #5 opened with the blanket imperative
`Assign EVERY blocker the list offers you, in every combat:` and put the survival carve-out
thirty lines below it, under the heading "The one blocker you leave out". A blanket imperative
first and its exception later means the imperative wins. Compare the same seat's CORRECT
application at `...vs-ai_baka_deck162.jsonl` seq 15, where two Vampires reading "your blocker
dies" were left out and only Perimeter Captain ("neither dies") blocked - the carve-out fires when
the blocker is a Vampire and loses to the `STOPS all N damage` tag when the blocker is a wall.

BEFORE:
```
Assign EVERY blocker the list offers you, in every combat:
 - Give each attacker one blocker first, then put the leftovers on the biggest attacker. ...
```
AFTER: `STEP 1 - READ THE PARENTHESIS ON EVERY LINE YOU ARE ABOUT TO USE, BEFORE YOU ASSIGN
ANYTHING`, which states the precedence outright ("that parenthesis outranks the `[deals 0 - ...
but it STOPS all N damage from reaching you]` tag beside it"), carries the LIVES-before-dies
tiebreak and the leave-it-out carve-out with both incidents written in; `STEP 2 - assign every
blocker that STEP 1 did not rule out`. The later paragraphs are rewritten to defer to STEP 1
rather than restate it (skill #51: one rule, one place). The DECIDING SITUATIONS line
`The opponent attacks: assign every blocker on the list.` is replaced with the STEP-1-first form.

## F2 - RULE #7 ENTRY 7, CHROMATIC LANTERN: gate on the rendered per-colour count
FINDING: **5 Lantern casts this corpus, 5 violations of the wave-45 gate** (`fewer than TWO of the
LANDS on your battlefield can make {b}`), and 1 of 1 miss on the positive half:
| where | black lands on the battlefield line | {B} on the mana line | Lantern already out | chose |
|---|---|---|---|---|
| vs130 seq 6 t4 | 2 (Bayou #1, Bayou #2) | 2 | 0 | CAST |
| vs123 seq 7 t4 | 2 (Bayou, Scrubland) | 2 | 0 | CAST |
| vs125 seq 8 t7 | 3 (Bayou #1, Scrubland, Bayou #2) | 3 | 0 | CAST |
| vs152 seq 11 t5 | **1** (Scrubland) | **1** | 0 | Idyllic Tutor (declined the Lantern) |
| vs152 seq 26 t11 | 2 | 3 | 0 | CAST |
| vs152 seq 33 t15 | 2 | 6 | **1** | CAST A SECOND ONE |
D19 FAILS on both halves. Two separate defects:
 (a) the model does not execute a by-name count off the battlefield line - six windows, six wrong;
 (b) **the name list in the guide is itself wrong on an Urborg board.** Primitive:
     `name=Urborg, Tomb of Yawgmoth / auto=lord(land) transforms((swamp)) / text=Each land is a
     Swamp in addition to its other land types.` So with Urborg out, Savannah and Sunpetal Grove
     DO make {b}, and the guide's `SAVANNAH AND SUNPETAL GROVE DO NOT` sentence is false exactly
     on the board where it was applied twice (vs152 seq 26 and seq 33, both with Urborg in play -
     the render's `{B} 3` and `{B} 6` were right and the guide's "2" was wrong).
The fix is available for the first time this wave: lane F (#W46-1) now ships
`(sources that can make each: {W} 2, {B} 1, {G} 3 - ...)` on every `Mana available:` line, 174/174
on this seat, 1708/1708 corpus-wide. That clause is the engine's own count and it already knows
about Urborg.

BEFORE: `Chromatic Lantern - only when BOTH are true: FEWER THAN TWO of the LANDS on your
battlefield can make {b} ... Count them off the battlefield line by name - the lands of yours that
make black are Bayou, Scrubland, Woodland Cemetery, Isolated Chapel, the Swamp and Urborg.
SAVANNAH AND SUNPETAL GROVE DO NOT ... Do not use the "colours you can make:" line for this test.`
AFTER: `(a) the "Mana available:" line's per-colour clause reads "{B} 1" or has no {B} entry at
all` + `(b) no Chromatic Lantern appears anywhere on your battlefield line`, with the Urborg fact
written in as the reason the name list is retired and the six-window record stated. The
"do not use the colours you can make: line" warning is KEPT in substance - the new rule points at
the per-colour COUNT clause, not at the colour SET, and says so.
NOTE the {B} count is of untapped SOURCES, so it can undercount a board whose black lands are
tapped. That is the right direction of error for this entry: it can only make you cast a Lantern
you did not strictly need, never skip one you did. Recorded so wave 47 does not re-derive it.

## F3 - the second-Lantern absolute, into SITUATIONS
`A SECOND Lantern does nothing at any time` was already in the guide body and a second Lantern was
cast anyway (vs152 seq 33, one already on the battlefield line). Added to the situations list with
the observable ("no Chromatic Lantern already on your battlefield line") - the same hoist that took
deck125's sweeper gate to 0/123.

## F4 - RULE #6 ATTACKING: count your attackers against their N
FINDING: 13 attacker windows, 7 attacks declared, 6 `ATTACK: none`. The collapsed-tag branch is
CLEAN where it applies - `...vs-ai_baka_deck123.jsonl` seq 37 and 43 both read `they have 14
untapped creatures that could block this one` and both correctly answered none (2/2). The failures
are in the N = 1 and N = 0 cases, where the guide says "the parenthesis is the whole story":
 - `...vs-ai_baka_deck123.jsonl` seq 18, turn 10: `A1. Vampire (1/1) [lifelink, doesn't untap
   during its controller's untap step]` with **no `their untapped blockers:` clause at all** -
   nothing could block it - and the model answered ATTACK: none. A free point of lifelink thrown
   away. The guide has no line for an unadorned A-line.
 - `...vs-ai_baka_deck130.jsonl` seq 40, turn 22, opponent at 6: four Vampires, each tagged
   `their untapped blockers: Siege-Gang Commander (2/2) (your attacker dies, ...)`. By the letter
   of "the parenthesis is the whole story" that is ATTACK: none four times. The model sent all
   four; three connected; the game ended at turn 22. The guide would have been WRONG here and it
   is the guide that gets corrected, not the play.
AFTER: the N = 1 branch now says COUNT YOUR OWN A-LINES FIRST - one blocker blocks one attacker,
so a surplus connects whatever the parenthesis says - and a new bullet covers an A-line with no
blocker clause at all. Two matching SITUATIONS lines added.

## F5 - MULLIGAN BOTTOMING: the three-land floor moves to the front
FINDING: one bottoming ask (`...vs-ai_baka_deck152.jsonl` seq 3). Numbered hand: `1. Savannah
(land) 2. Woodland Cemetery (land) 3. Tribute to Hunger {2}{b} 4. Wall of Omens {1}{w}
5. Perimeter Captain {w} 6. Chromatic Lantern {3} 7. Scrubland (land)` - three lands. The model
bottomed **Woodland Cemetery**, leaving two lands, against `NEVER bottom a land when the hand you
keep would hold fewer than THREE`, and past `Bottom your most expensive spell first` (the Lantern
at {3}). Double violation of the same rule block, unpunished (that game was won at turn 18). The
floor was the LAST sentence of the section and the "most expensive spell" imperative was the first.
Same ordering defect as F1.
AFTER: `FIRST, RULE THE LANDS OUT` - count the `(land: taps for` lines and cross every land off if
bottoming one would drop below three - THEN the expensive spell, then a duplicate wall. The
SITUATIONS line reordered to match.

## F6 - RULE #1: the auto-pass now tells you something
FINDING (D17): 16 windows offered a `Add N green mana with Overgrown Battlement` option, **1 taken
in a window where nothing was cast** (`...vs-ai_baka_deck123.jsonl` seq 20, turn 10 - the model
tapped for 3 green, then answered `Cast nothing right now` at seq 21, 23 and 25 and passed the
turn with the pool floating). Wave 45 was 2 of 39. The denominator collapsed because the engine's
new mana-only auto-pass (#W46-7) skipped **54 + 98 + 28 = 180 windows on this seat**, which is
100% of the corpus's 180 skips.
AFTER: one sentence appended to the two-question test, stating the new observable - if you can SEE
the tap option, something else was on the menu too, so question 1 has a real answer or the answer
is pass. No threshold moved.

---

## THRESHOLD WHYS (skill amendment 61) - updated for this wave
- Chromatic Lantern at `{B} 1 or no {B}` because Sanguine Bond is {3}{B}{B} and the render's
  per-colour clause is the only count of black sources the pilot has been able to execute; the
  by-name land count was 0/6.
- Two black sources (unchanged, now expressed as the render's number) because {3}{B}{B} has two
  black pips.
- Three lands on the bottoming floor (unchanged) because the cheapest enchantment is {4}{B}.
- The blocking carve-out at `you would be at 8 or less` (unchanged) - no window this corpus tested
  it above or below, and skill #66 forbids moving a floor on no window.
- RULE 7b's `life is 10 or less` (unchanged, UNTESTED for a second wave - no window matched all of
  its conjuncts).
