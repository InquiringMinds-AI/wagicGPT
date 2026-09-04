# deck123 → synthesis: ONE general-guide proposal (wave 59)

## P1 — The general guide should tell the pilot what to do when the answer it must commit on LINE 1 depends on arithmetic it is only allowed to write on LINE 2.

**The seam.** `kReplyProtocol` (`src/AIPlayerGPT.cpp`, ~line 193) says: "Your reply is ONE line,
or TWO when you write a plan. Nothing else. / LINE 1 is your ANSWER ... / LINE 2 is a PLAN:
line". Several deck guides — deck123's Rule 3 among them — mandate a computation that *decides*
LINE 1 and can only be written on LINE 2: "WRITE THE THREE NUMBERS BEFORE YOU WRITE THE CHOICE.
... PLAN: Damnation: N <their number>, K <able to attack>, M <mine>; <cast / hold>". Under the
protocol that instruction cannot be obeyed as written. The pilot commits, then computes.

**What it cost, with seqs** (corpus `matchups-20260904-130044`, deck123 seat files):

- `vs126 s86` (t18): reply is
  `CHOICE: 2 (Cast Damnation)` / `PLAN: Damnation: N 6, K 0, M 31; hold (M > N, K=0 walls)` /
  `PLAN: L 22, C 6, stop 29; M 31 now; this window: pass (M >= stop)`.
  Both plan lines say hold; the answer line cast, and the row it took read
  `{right now: destroys 6 of their creatures ..., 31 of yours}`. The seat destroyed its own
  31-creature board and lost the game it was winning.
- `vs125 s129` (t62): `CHOICE: 1 (Cast Damnation)` / `PLAN: Damnation: N 0, K 0, M 0; hold. No
  creatures to sweep.` Same shape, wasted card.
- `vs126 s69` (t16), the control: the model ignored "Nothing else", wrote 4,049 characters of
  deliberation between an opening `CHOICE: 2 (Cast Damnation)` and a closing
  `CHOICE: 7 (Hold priority for the rest of this turn)`, and the parser's separated-line
  supersession rule took the LAST line. That reply was right, and it cost 431,938 ms.

So the pilot reaches the correct answer when it is allowed to think first, and takes the wrong
row when it is made to commit first. This is not a deck-guide content gap — deck123's Rule 3
states the rule correctly and emphatically, and restating it again is exactly the busywork the
wave-59 brief warns against.

**Proposed general-guide text** (wording is the synthesis agent's; the content is the ask):
add to the general strategy's reply section, in the pilot's own frame —

> Some decisions carry a rule that tells you to work a number out before you answer. Work it out
> before you write LINE 1, not after. If you have already written an answer line and your own
> plan line then contradicts it, write a SECOND answer line with the choice you now mean: the
> engine executes the LAST coded answer line in your reply, so an abandoned first line is a play
> you have made. Never end a reply whose PLAN line says "hold" above an answer line that casts.

**Why the general guide and not each deck guide:** the seam is created by the protocol, which is
shared, and it fires wherever a guide mandates arithmetic (deck123's Damnation ritual and token
stop; the same shape exists in any guide with a threshold rule). Fixing it once, where the reply
rules live, is one edit instead of N.

**The durable fix is engine-side and is routed as an engine item, not here** (`review.md` §2 I4):
either let a scratch/PLAN line precede the answer line, or state the supersession rule to the
model in the protocol itself. The general-guide change above is what can be done this wave
without a build.

**Falsifiable prediction if adopted:** in the next corpus, zero replies in which a coded answer
line is contradicted by its own PLAN line's verdict word (this corpus: 2 in deck123's slice
alone, `vs126 s86` and `vs125 s129`), and any reply that does reason to a different answer ends
with the answer it reasoned to.

**Not proposed** (checked and deliberately dropped): nothing about the Damnation rule's content,
the ATTACK ladder, or the mulligan band — deck123's guide already carries all three correctly
and this corpus gives no evidence against any of them.
