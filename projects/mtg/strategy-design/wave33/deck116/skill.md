# deck116 -> strategy-writing skill: wave-33 proposals

Three. S-116b is the one I would fight for; S-116c and S-116d are cheap.

---

## S-116b (PROPOSED, Step 0-ter — a new rung, single-seat but structurally load-bearing)
### A FIRST-GUIDED VALIDATION MUST SEPARATE "THE TEACH DID NOT LAND" FROM "THE TEACH LANDED AND THE PROTOCOL DISCARDED IT" — AND THE ONLY PLACE THAT DISTINCTION LIVES IS THE POST-ANSWER TEXT.

Step 0-ter's firing table has four verdicts (FIRED / MISSED / FALSIFIED / SUPERSEDED). All four are
scored off the DECISION. Under the answer-first protocol that is no longer sufficient, and this
seat produced the clean instance:

- `vs158 s7`. Committed `CHOICE: 4 (Cast Fabricate {2}{u})` with a Quicksilver Amulet in hand. On
  the decision alone this is RULE #2's corollary MISSED — the exact wave-32 strict-dominance error
  the corollary was written to kill, in a game that was then lost.
- The reply's next 5,700 characters **quote the corollary verbatim**, apply it correctly, work
  through every alternative, and conclude *"So I will cast Time of Need."* It never re-commits.

The verdict on the decision is MISSED. The verdict on the TEACH is FIRED. A reviewer who scores the
decision rewrites a rule that is working — the single most expensive mistake this skill exists to
prevent, and the one HL2 already warns about from the other direction (the revision's own new text
is unaudited).

Two more at the same seat, same shape, one costing the deck's engine card (`vs146 s13`: committed
`BLOCKS: B1:A1, B2:A2`, then *"Piper is our win condition engine. We must save Piper"*, never
re-commits, Piper dies) and one costing tempo (`vs152 s3`: committed a tapped land, then
*"I will choose Island"*).

**THE RUNG.** For every decision scored MISSED or FALSIFIED against a taught class, read the text
AFTER the committed answer line before writing the verdict, and record which of three it is:
1. **BELIEF MISS** — the reasoning agrees with the wrong answer. Guide lane. Revise the teach.
2. **COMMIT MISS** — the reasoning reaches the RIGHT answer and the committed line is the wrong
   one. **Protocol lane. Do NOT revise the teach; record it as FIRED with a commit-miss tag.**
3. **NO REASONING** — nothing to read. Guide lane by default.
Report the counts. Cost: reading the tail of the replies you were already going to read at the
seams you already flagged. It is free at exactly the decisions where getting it wrong is most
expensive, because a taught class that "fails" is where every revision budget goes.

**Why it needs to be a rung and not a note:** the wave-34 protocol change makes this the difference
between a corpus that can attribute its own deltas and one that cannot. If reasoning moves
pre-answer and commit misses vanish, every seat that scored them as BELIEF MISSES this wave will
read the disappearance as its own guide revision working.

---

## S-116c (PROPOSED, Step 0-ter / HL5 — one line, guide-side)
### WHEN A TEACH SUPPRESSES A CARD ENTIRELY, THE RENDER DEFECTS ON THAT CARD'S PATH BECOME UNTESTABLE — SAY SO IN FINDINGS AND NAME THE SEAT THAT MUST NOT BE USED AS THE WITNESS.

deck116's wave-32 guide demoted Farseek to a last-choice spell. Wave-33: **Farseek offered in 36
windows, cast 0 times.** N-116d — the pick-cardinality subset-framing defect — lives on Farseek's
un-tokened `target(plains,island,...)` path and therefore has **0 occurrences this corpus**. A
reviewer reading only the count would report the class dead; it was never sampled, because the
guide closed the only door to it.

This is HL5 rung 4's fixture-rides-the-broken-branch lesson with a GUIDE playing the part of the
fixture. The check is one line and it belongs in the routed-item pass: **for every KNOWN-OPEN item
routed at this seat, name the card or effect that exercises it and check the corpus for a nonzero
usage count before writing any verdict. A zero usage count converts the verdict to UNEXERCISED and
disqualifies this seat as the fix's future witness while the suppressing teach is deployed.**
Otherwise a guide teach silently retires an engine docket item.

---

## S-116d (PROPOSED, HL9 rung 2 — a small addition to the ABSENCE TABLE)
### THE ABSENCE TABLE NEEDS AN "ATTEMPTED BUT DID NOT ARRIVE" COLUMN, BECAUSE A ZERO IN THE DEPLOY COLUMN HAS TWO CAUSES AND THEY ROUTE TO OPPOSITE LAYERS.

Wave-32 promoted the absence table on this seat: a column of zeros in "signature permanents that
reached the battlefield" is the finding, and it outranked two individually-observed misplays. It
worked - the column went 3/6 -> 1/6 this wave.

But this wave the same column produced a zero for a completely different reason. vs105 deployed one
creature; the absence table alone says "deploy floor". The truth is the pilot took the deck's win
condition TWICE and the engine dropped it both times (N-116g). And vs158's zero is a third cause
again: no castable creature was ever drawn.

**THE ADDITION:** the absence table gets three columns, not one — **OFFERED / TAKEN / ARRIVED** —
and the reviewer states which gap produced each zero:
- offered 0 -> draw or construction (or an offer-side defect: run the castable-but-not-offered audit)
- offered > taken -> **guide lane**, this is the deploy floor
- taken > arrived -> **engine lane**, and it outranks every guide finding in the seat
The instrument the skill already has (arrival-tracing) supplies the third column for free; the
absence table simply was not asking it to. Without the split, an engine seam that silently eats the
deck's thesis is indistinguishable from a pilot that durdles — and the reviewer's whole revision
budget goes to the wrong layer.

---

## Wave-32 skill items, status

- **S-116a (`#HINT:` lines are a guide-authoring input) — ADOPTED wave-32 and it held.** deck116.txt
  carries `#HINT:dontattackwith(Elvish Piper)`, `#HINT:dontblockwith(Elvish Piper)` and
  `#HINT:alwaysattackwith(creature[Eldrazi])`. RULE #4 and the attack teach are those hints, and
  they scored 7/7 on attacks and 8/9 on blocks (the ninth being S-116b's commit miss). Re-verified
  ABSENT from the assembled prompt this corpus. No change requested.
