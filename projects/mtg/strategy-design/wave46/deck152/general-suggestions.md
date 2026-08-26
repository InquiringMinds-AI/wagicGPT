# deck152 -> core-prompt / render proposals (wave 46)

Engine defects are in the seat file's E-list; this file is core-prompt and render convention only.

---

## G1 (HIGH) — the converter line should name the doubling on the price it doubles, not only in
## its own paragraph

**Evidence.** The wave-45 converter line renders correctly and completely:
```
LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond. While it is in play, life
ITS CONTROLLER gains also makes the other player lose that much life. A converter of THEIRS
turns every life THEY gain - lifelink, blocking triggers, drains, any of it - into that much
life off YOUR total as well ...
```
It names blocking triggers explicitly. The same prompt then prints the per-blocker prices on the
A-lines: `(you kill it, your attacker lives (blocking trigger: they may gain 2))`. Both facts are
true and present; the multiplication between them is left to the reader. deck152 read both and
attacked three times, going 20 -> 11 -> 5 while the opponent gained the same amount, and lost at
-1. deck162 did the same thing in one turn (19 -> 0) against the same opponent.

**Proposal.** When a converter of THEIRS is on the battlefield, fold the doubling into the tag
that carries the number: `(blocking trigger: they gain 2, and their Sanguine Bond takes 2 off
you)`. This is the same fix shape as wave 46's lifelink binding (#W46-9), which worked: bind the
consequence to the clause that states the cause. The standalone paragraph is a definition; the
tag is where the decision is made. Two seats in one corpus lost games with both strings on screen.

## G2 (MED) — the "you would be at N" header could carry the guide-independent comparison the
## ladder needs

**Evidence.** deck152 answered `BLOCKS: none` at a header reading "you would be at **8** - NOT
lethal" while its floor is 9, and the reply restated the 8 without comparing it. The header
already computes the number; the executor's failure is the comparison, not the arithmetic. This
is the second corpus with this exact shape on this seat.

**Proposal, narrowly scoped and restriction-first.** The header should not state a threshold (that
is guide content and would constrain choice), but it could state the DELTA the decision is over:
`you would be at 8 (blocking A1 instead leaves you at 14)`. That is a true fact about the board,
it adds no advice, and it puts both numbers of the comparison on one line. Emitting it only when
a block is actually offered keeps it off screens where it means nothing.

## G3 (MED) — `Clue` tokens are the one textless permanent class left

**Evidence.** Every non-creature permanent on both battlefield lines carries `{effect: "..."}`
(542 annotated appearances on my seats: Staff of Nin, Lightmine Field, Sanguine Bond, Exquisite
Blood, Intruder Alarm, Talisman of Impulse, Chromatic Lantern, Pyrite Spellbomb, Elixir of
Immortality, Teferi, Sorin, Underworld Dreams), and the multi-copy variant `{effect (each copy of
this card does this): "..."}` also renders. `Clue [artifact]` renders bare, 14 times on my seats,
on both sides of the board. deck152 makes Clues off Briarbridge Tracker every game.

**Proposal.** Give tokens the same `{effect:}` treatment as cards. The ability is reachable from
the option row (`Draw 1 with Clue [cost: {2}, Sacrifice]`), so nothing is unplayable — but a
permanent the model can see and cannot read is exactly the surface the trust doctrine is about,
and Clue is a resource whose value is entirely in its text.

## G4 (LOW) — the target list already prints current size; say so once

**Evidence.** deck152's counter placements are made from a target list where every entry prints
the creature's current size (`Sigarda, Champion of Light (9/9) [trample, flying] [your
battlefield]`). The guide can and now does key a rule to that. Nothing in the shared narration
tells the model those sizes are current rather than printed, and the battlefield line has a
separate convention for that (`(6/5) (printed 2/3)`).

**Proposal.** One clause in the target-list preamble: the size shown is the creature's CURRENT
size, counters and anthems included. Cheap, and it makes a whole class of guide rules keyable to a
rendered observable instead of to a tally the executor cannot keep.
