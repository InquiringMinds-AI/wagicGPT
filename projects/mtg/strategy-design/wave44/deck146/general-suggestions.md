# deck146 — core-prompt / render proposals (layer-routed)

Engine BUGS are in the seat file's engine list, not here. These are prompt/render TEXT proposals.

## G-1 (MED, render text) — the attack tag should price block-triggered life gain
The tag already annotates lifelink inline: `Vampire (1/1) (both die (lifelink: they gain 1))`.
It does NOT annotate a blocker's own "whenever this blocks" life trigger, so
`Pride Guardian #1 (0/3) (neither dies)` hides 3 life, and `Perimeter Captain (0/4) (neither dies)`
hides 2 life per defender block. deck146 vs deck126, attackers seq 44 / 51 / 56 (turns 18-22), is
the repro: the opponent's life went 13 -> 42 entirely on blocks whose tags all read "(neither dies)".
PROPOSAL: extend the existing lifelink parenthesis convention to any block-triggered life change,
same shape: `(neither dies (they gain 3))`. The mechanism (a `@combat(blocking)` life trigger on
the blocker) is already visible to the code that writes the lifelink annotation.

## G-2 (LOW, prompt text) — the plan-note gate false-fires on land-drop asks
Measured across my three seats: the note "this decision's list does not contain the actions your
plan names" fired on 86 of 125 land-drop asks (69%). At least 35 of those had a plan whose text
literally says "play a land" / "play lands" - i.e. the list DOES contain the plan's action. The
gate is matching card NAMES; a plan that names a card type rather than a card is scored as a miss.
PROPOSAL: suppress the note on asks whose entire option set is `Play X` / `Hold X` / `Play no land
right now`, or match the plan against the option VERB as well as the name.
(Set-B lane #W44-7's own target is met: 0/66 fires on attackers, 0/14 on blockers.)

## G-3 (LOW, render text) — land rows in a tap/untap target menu carry no mana text
`Tundra [land] [opponent's battlefield]` and `Forest [land] [your library]` are rendered with no
production text, while every nonland target row carries `- "..."` rules text (96 of 103 opponent-
battlefield target rows this corpus DO carry text; the misses are lands and a vanilla token).
When an ability asks "choose your land" / "choose opponent land" the model has no basis to pick.
PROPOSAL: append the mana line to land target rows, in the same shape the land-drop menu already
uses: `Tundra [land] (taps for {W} or {U}) [opponent's battlefield]`.

## G-4 (INFO) — what the render got RIGHT and should not be changed
- Day/Night: 31/31 daybound-on-battlefield frames carried a day/night line (lane #W44-4).
- Target text on removal: 96/103 opponent-battlefield target rows carried rules text; 0 blind
  choices on any card with real text (lane #W44-3).
- The reveal frame in deck152 vs deck125 seq 78 was correctly attributed to the OWN library
  ("Your library: 28 cards (3 of them are the cards listed...)") - no false frame (lane #W44-6).
- The Hive animation state is rendered honestly - `(3/3) [menace]` and the creature count both
  update. The 23 redundant animations are a guide gap, NOT a render gap.
- `[doesn't untap during its controller's untap step]` on deck123's board is CORRECT (they control
  Intruder Alarm), not a leaked tag - checked against the primitive before reporting.
