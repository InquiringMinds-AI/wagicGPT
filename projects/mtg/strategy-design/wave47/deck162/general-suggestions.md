# deck162 seat - core-prompt / render proposals (wave 47)

---

## G-8 (HIGH) - a converter PAIR is an unbounded loop and the converter block describes only one
## direction of it.
The current block reads: `LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond.
While it is in play, life ITS CONTROLLER gains also makes the other player lose that much life...`
That is exact for `Sanguine Bond` alone (`auto=@lifeof(player) from(*[-lifefaker]|*):life:-thatmuch
opponent`). deck126 also runs `Exquisite Blood` (`auto=@lifelostfoeof(player):life:thatmuch
controller` - "whenever an opponent loses life, you gain that much"). With BOTH on one battlefield,
any single point of life they gain is a closed loop: they gain 1 -> you lose 1 -> they gain 1 ->
... The block names only the one it found, and a model reading it computes a bounded price for an
unbounded event. `146 vs126` seq 33 reveals both cards in deck126's hand; the seat lost that game.
**Proposal:** when both directions are present on the same battlefield, say so in the same block -
`both halves of a life LOOP are on their battlefield (Sanguine Bond + Exquisite Blood): any life
they gain repeats without limit, so ANY nonzero payment on a tag is fatal, not merely expensive`.
This is the true token, it is restriction-first, and it deletes nothing. (Whether the ENGINE's own
loop needs a stopping affordance is the engine seat's item - see the corpus note's invariant-00
game and seat-file E-4.)

## G-9 (MED) - the `right now:` preview on an X/modal cast is the best decision surface in the
## corpus, and it should be the template.
`162 vs152` seq **20**: `1. Cast Peer into the Abyss {4}{b}{b}{b} {right now: if you choose
"target opponent": life -5, draws 19; if you choose "target controller": ...}`. The seat cast it,
answered the follow-up chooser correctly, and won from 21-9 to **21-0** in that turn. The row did
the whole job: it named both branches and priced both, on the row, before the commit.
Contrast the same seat's `-2: destroy target creature and draw two cards with Ob Nixilis ...
targeting <name>` rows, which print the target but not the consequence, and which this seat
answered inconsistently (`vs146` seq 9 at 7 life took the 1/1 deathtouch body over the 3/2 clock).
**Proposal:** extend the `{right now: ...}` per-branch preview from X/modal casts to activated
abilities whose effect quantity depends on the chosen target - the same emitter, the same shape.
Nothing here is a bug; it is a surface that measurably decided a game and is not yet general.

## G-10 (LOW) - the affirmative no-blocker tag has abolished the untagged A-line, and that is a
## quiet, complete win worth recording.
Wave 46's 162-C was written for A-lines that carried **no** `their untapped blockers` tag at all -
the shape the ATTACKING rungs were all silent about. This corpus: **zero** such A-lines across all
three of my seats. Lane K's `[no creature they control can block this attacker]` covers the case
affirmatively, and it is obeyed - **42 of 42 rows carrying it were attacked**, on 146 (25), 152
(14) and 162 (3), with **zero `ATTACK: none` on such a row**. One reply named the creature instead
of the A-number (`146 vs152` seq 17, `ATTACK: Nadaar, Selfless Paladin`) and was accepted; that is
the only format wobble. No change wanted - filed so the next wave does not re-derive it.
