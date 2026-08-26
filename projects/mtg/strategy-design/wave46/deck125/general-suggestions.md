# Core-prompt / render proposals from the deck125 seat (wave 46)

Layer-routed. Engine BUGS are in `../seats/seat-125-126.md`'s engine list, not here. These are
proposals about what the prompt SAYS and how it is shaped.

## G-A - Print a count of the pilot's own hand (the #1 ask of this wave)
The prompt already prints `Opponent hand size: 5 | Opponent library: 46 cards` and `Your library:
39 cards`. The pilot's own hand is the one quantity in that neighbourhood rendered as a list and
not as a number:
```
Your hand: Cancel (copy 1 of 2 in your hand) {1}{u}{u} [instant]; Cancel (copy 2 of 2 in your
hand) {1}{u}{u} [instant]; Final Judgment {4}{w}{w} [sorcery]; ... (eleven items)
```
`pregameHandHeaderText()` already emits `Your hand (7 cards), counted by the engine: 3 lands, 4
spells` before the game starts. Proposed in-game shape, one integer, same style:
```
Your hand (11 cards): Cancel (copy 1 of 2 in your hand) {1}{u}{u} [instant]; ...
```
This is the same defect and the same fix as #W46-1's per-colour source counts, which shipped this
wave and which this seat confirmed at 452/452 - and #W46-1's payoff was immediate and measurable
(deck126's coloured-pip overcommits went from a game-losing 1 to 0). The seven-card discard limit
is a rule of the GAME, so every deck in the pool has a hand-size decision somewhere; deck125 is
just the seat where it is a printed rule and therefore measurable (4/17). Evidence and repro in the
seat file, HIGH item #1.

## G-B - The "counted by the engine" idiom is doing real work; extend it deliberately
Three renders now carry it: the pre-game hand breakdown, the pre-game mana-source counts, and (new)
the in-game per-colour source counts. Every one of them replaced a place where a guide had told a
pilot to count something, and every one of them moved a measured rate. The pattern is worth stating
as a design rule for the core loop rather than rediscovering it per lane:
**anything a strategy guide currently instructs the pilot to COUNT off a rendered list is a
candidate for an engine-side count, and the engine already has the data at the call site.**
Remaining candidates visible from this seat, in order of measured cost:
 1. own hand size (G-A) - 13 of 17 X windows wrong.
 2. opponent creatures that can ATTACK, i.e. the header count minus `[defender]` and printed-power-0
    bodies. The header `Opponent battlefield (6 permanents listed, of which 2 are creatures)` says
    2 on a board of two walls, and deck125 spent a Supreme Verdict on it twice this wave. A second
    clause - `(of which 2 are creatures, 0 of them able to attack)` - would retire a whole guide
    branch in every control deck in the pool. Same shape, same call site.
 3. untapped sources remaining AFTER the spell in the option row is paid for. Every tap-out rule in
    every guide is a subtraction the pilot performs; deck125's has been at 4/7 for three waves.
    An annotation on the cast row - `{leaves 3 untapped}` - is the same information the affordability
    check already computes.
I am proposing the RULE, and (2) and (3) as ranked candidates under it, not as separate lanes.

## G-C - Do NOT add a "your hand is full" warning
Worth saying explicitly, because it is the obvious cheap alternative to G-A and it would be worse.
A threshold warning would fire on a boolean the guide would then have to trust as a proxy for a
number, and this seat's whole X problem is that a proxy replaced a number. The count is cheaper to
render and strictly more useful. If only one lands, land the count.
