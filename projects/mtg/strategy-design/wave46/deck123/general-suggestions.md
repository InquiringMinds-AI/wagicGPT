# deck123 - core-prompt / render proposals, wave 46
Layer routing: engine BUGS and render items are in `wave46/seats/seat-123-130.md`'s ENGINE list
with repros; this file holds only proposals about the CORE PROMPT and the shared render frames.

## G46-1 (from ENGINE M2) - the pre-game mulligan ask should not carry the carried-plan block.
The `YOUR PLAN (as you last stated it)` block exists so a decision inherits the pilot's own
intent across windows. The pre-game mulligan chain is the one place in the loop where the SAME
question repeats with no state change except a counter, and where the carried intent is itself an
answer to that question. deck123 wrote "Mulligan to find a hand with 4+ lands ..." at look 2 and
re-emitted that string as its reply at looks 3, 4, 5, 6 and 7 - to zero cards.
PROPOSAL: on `Pre-game mulligan decision` asks specifically, either omit the plan block (no plan
stated about a hand that no longer exists is evidence about the hand on screen) or re-frame it as
what it is - `PLAN YOU STATED ABOUT A HAND YOU HAVE ALREADY SHIPPED (it is not about this hand):
...`. The second is more faithful to the trust doctrine: nothing is deleted, the scope is stated.
NOT A BUG, and not a legality question - a scope question about a true surface.

## G46-2 (from ENGINE M2) - put the price on the row being chosen, not only in the header.
The ask's header says "having already taken 5 mulligans you will bottom 5 cards on a keep
(keeping 2)". The OPTION ROW says `2. Mulligan`. Every other menu in this loop annotates the row:
casts carry `{X pricing:}` and `{card text:}`, targets carry `{target text:}`, blockers carry the
computed trade. The one row whose consequence is unbounded carries nothing.
PROPOSAL: `2. Mulligan (a keep after this one would keep 1 card)`. This is a restatement of a
number already in the header, moved to where the choice is made, and it makes the terminal case
readable: `2. Mulligan (a keep after this one would keep 0 cards - your entire hand goes to the
bottom)`.

## G46-3 - the CHECK-1 class: an attackers window can be poisoned by a permanent that no attacker
## tag can see, and the frame says so for exactly one mechanism.
The `LIFE-TO-DAMAGE CONVERTER` line (wave-45 lane B, now in CURRENT SITUATION) is a model of how
to surface a board fact that the per-row combat arithmetic structurally cannot include, and it
worked: deck123 vs126 seq 39 held 14 attackers correctly with that line on screen. The same
window's collapsed tags read `(both die (lifelink: they gain 1))` - true per row, and the total
that actually decides the window (roughly 12 life gained across six blocks, all of it drained
back off the pilot) is nowhere.
PROPOSAL, and I offer it as a QUESTION rather than a spec because I have one window of evidence:
would an attackers-window line stating the summed worst case be claimable - `IF EVERY ONE OF
THEIR UNTAPPED CREATURES BLOCKS: they gain up to N life from blocking triggers and lifelink, and
the converter above turns that into N off your life`? The per-block gains are already computed
(they are printed on the tags); the sum is only claimable if "every one blocks" is stated as the
bound rather than as a forecast, which the wording above does. If it cannot be claimed exactly,
it should not be claimed at all - the tags are true and the guide can do the arithmetic.

## G46-4 - the reply protocol's plan clause is being satisfied by re-emitting the plan verbatim.
The protocol says "then a PLAN: line only if the reply rules call for one (no plan shown yet, or
part of yours is now done or false)". Across 583 decisions at this seat pair, a large share of
replies re-emit the shown plan character-for-character - including at every step of the mulligan
chain, where the plan was neither done nor false by its own terms and so, by the letter, should
not have been re-emitted at all.
PROPOSAL: this is a measurement request before a wording change. The carried item #W46-11
("stale-plan note repeats on identical plan text, 49%") is looking at the same surface from the
other side. Someone should count, corpus-wide, replies whose PLAN line is byte-identical to the
shown plan, and split them by whether the protocol's own condition was met. If most identical
re-emissions are protocol violations, the clause is not being read; if most are cases where the
plan really did go false and the model restated it unchanged anyway, that is a different fix.
I did not compute this beyond my own seats and am not proposing wording on it.
