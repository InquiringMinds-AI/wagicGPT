# Core-prompt / render proposals from the deck130 seat (wave 53)

Layer-routed. Engine defects with repros are ranked in `wave53/seats/seat-123-130.md`. The
deck123 file's G1-G5 apply at this seat too (G1's PLAN capture, G2's coded-line-first, G5's HOLD
row: 91 offers, 3 takes).

## G6 (render, HIGH) - the cast row's kill summary cannot say "lethal", and said "kills 0" on a
## row that won the game
**What happened:** 130v162 s62, `Opponent life: 1`, ten untapped sources. Row 1:
`Cast Hammer of Bogardan {1}{r}{r} {leaves 7 of your 10 untapped mana sources untapped}
{kills 0 of the 2 legal targets at 3 damage} - legal targets right now: Shield Sphere (artifact
creature 0/6) [defender] ..., Fate Unraveler (3/4) ...`. The target ask that followed listed
`1. The opponent (player, life 1)`. Lane O's own note says the summary "counts only creature
targets currently on their controller's battlefield; player targets and non-creature permanents
are excluded from the denominator" - so on an `anytarget` spell the one row that matters is
outside the count, and the pilot read "kills 0" as "this does nothing".
**Ask:** when the spell's target set includes a player and the printed damage is at or above that
player's life, continue the summary with the lethal tail lane O already built for DRAW PRICE:
`{kills 0 of the 2 legal targets at 3 damage - but 3 to the opponent at life 1 WINS THE GAME}`.
Failing that, at minimum print the denominator honestly: `kills 0 of the 2 creature targets`.

## G7 (render, MED) - the DRAW PRICE tail shipped without its life clause
**Three-number audit:** `[DRAW PRICE:` rows across both my seats: **58 renders** (deck123 34,
deck130 24), five distinct strings, all of the shape
`[DRAW PRICE: this draws 1 card, and the opponent's Ob Nixilis, the Hate-Twisted, Fate Unraveler
punish every draw, so taking it costs you 2 life right now]`. Renders carrying lane O's new tail
`- you would be at K`: **0/58**. Renders carrying `; this KILLS you`: **0/58** - and no window at
either seat reached damage >= life (lowest life under the row was 11 against a 2-life price), so
the KILLS branch is UNTESTED, but the `- you would be at K` half was due on all 58 by the lane's
own description ("when the tag states a damage total it now continues with lane K's tail") and
did not print. Takes from a DRAW PRICE row: **0/58** (behaviour half PASS).
**Ask:** re-check the call site's `life` argument; the tag states a damage total in all 58.

## G8 (render/cost, MED) - the forced-sacrifice menu is not collapsed
**What happened:** 130v125 s181-s186 and s189-s194 - twelve consecutive annihilator sacrifice
asks whose menus ran 18, 17, 16, 15, 14, 13 and 13, 12, ... rows, of which fourteen were
byte-identical but for the handle: `Mountain #1 [land] [your battlefield] - "R"` ...
`Mountain #14 [land] [your battlefield] - "R"`. The battlefield line above them collapses the
same permanents (`Mountain #1-#19 x19`); this menu does not.
**Ask:** apply lane P's collapse to sacrifice/target menus of the pilot's own permanents -
`1-14. Mountain #1-#14 [land] [your battlefield] - "R" x14` - the same shape already used for
`Deal 2 damage with Siege-Gang Commander targeting Goblin #1-#3 ... x3`.

## G9 (prompt, LOW) - the face row prices itself, the ability row still does not
Target menus print `The opponent (player, life 1)`; ability rows print
`Deal 2 damage with Pyrite Spellbomb targeting the opponent` with no number (130v125 s120, s158;
130v162 s35). Carried from wave 52 L1, unchanged this corpus. With G6 this is the same ask seen
from the other side: every row that can point damage at a player should print that player's life.
