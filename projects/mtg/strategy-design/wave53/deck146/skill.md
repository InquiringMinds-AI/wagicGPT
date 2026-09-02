# deck146 -> strategy-writing skill proposals (wave 53)

Only what this seat's evidence supports. Each is a candidate amendment; synthesis owns the merge.

## S-1. #168's literal audit is PER CARD FAMILY, not per literal - a string that renders 1,586
## times corpus-wide can still render 0 times on the row the guide keyed to it.

deck146's cast-order entry 5 gated Silverquill Command on *"the Command row's own `{right now:`
text names a card it would return"*. Corpus-wide `{right now:` renders **1,586** times, so a
grep for the literal passes and the audit's "old-string renders" number looks healthy. On
`Cast Silverquill Command` rows it renders **0 of 28**. The condition was unsatisfiable, entry 5
collapsed to entry 8b/9, and about twenty windows of a free body-plus-a-card were declined across
turns 29-83 of an 84-turn loss - one reply asserting an empty graveyard while the log above it
named two mana-value-2 creatures in it (`146v125` s240, s528).

**Rule.** #168(b)'s first pass greps each quoted literal **restricted to the option rows of the
card family the guide's sentence is about**, and reports the three numbers per (literal, card)
pair. A literal alive elsewhere is not evidence the sentence can fire. Companion to #154: the
re-key obligation applies not only when a lane CHANGES a string but when a guide sentence was
written from a string the emitter never attached to that card at all.

## S-2. Before scoring a ritual or a floor over "menus that held a live row", SPLIT the live rows
## by whether the engine had already marked them dead.

deck146's show-the-work sentence scored **15 of 165** own-turn `Cast nothing` answers holding
another `Cast ...` row - a 9% rate that reads like decay. Splitting the 165: **139** held only
rows the engine itself annotated dead (`at 0 this does nothing` 396 renders / 0 takes;
`[legendary:` 12 / 0; `the only legal targets are YOUR OWN` 0 takes), and the guide's own text
already excuses those. The remaining 26 are one real lane (S-1) plus six guide-legal declines.
The ritual therefore had **0** decisions to protect, and the honest verdict is DELETE, not
restate - #181's gate, applied with the denominator corrected.

**Rule.** Extends #109 and #184: a fraction whose denominator includes engine-marked-dead rows
is not a rule's fraction. Report both numbers (`15/165 raw, 15/26 live`) and score the second.

## S-3. A RULE WHOSE ONLY HOME IS A DISTANT SECTION HAS NO SURFACE AT THE WINDOW IT GOVERNS -
## #177's diff runs on rules the DECIDING SITUATIONS list never mentions at all, not only on
## rules whose bullet dropped a branch.

deck146's cleanup-discard order (Emeria's Call first, then Agadeem's, then a walker, then a land)
is a correct, complete, 12-line bullet - inside the MULLIGAN section, 500 lines above the ask it
governs. The DECIDING SITUATIONS list had no cleanup bullet. `146v130` discarded at three
consecutive cleanups (s15 `PUT: 8`, s17 `PUT: 5`, s18 `PUT: 3`) with **Emeria's Call on all three
numbered lists** and never sent it: **0 of 3**, and the seat died at 0 with the opponent on 2.

**Rule.** #177's mechanical diff gains a step 0: enumerate every ASK KIND the seat's corpus
produced (`ask`, `priority`, `attackers`, `blockers`, `discard`, `reveal`, `bottom`) and check
that each one the guide has a rule for is named in the recognise->do list by the literal the
prompt prints for it (here `"your hand has N cards and your maximum hand size is 7"`). A rule
filed under the wrong heading is dead text in exactly the way #177 describes.

## Recorded, not proposed as amendments
- **P1's clean pass is worth naming.** deck146 shipped **0** of N mulligans at two or more lands
  this corpus (was 3 of 4). Its STEP 2 carries the scoping sentence #171(a) asks for; deck152's
  does not and shipped one. That is a direct cross-guide control for #171 - see `deck152/skill.md`
  S-2 - and it is why the deck152 fix is a port, not a new invention.
- **The 595-second mulligan** (`146v125` s1, the corpus's longest single decision). The reply is
  1,600 characters of self-argument that reverses once and reaches the CORRECT answer, and the
  parser honoured its last coded line. #116 holds; the cost is an observability item, not a guide
  question.
