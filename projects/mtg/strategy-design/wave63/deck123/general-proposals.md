# deck123 — general-guide proposal (one item)

## G1 — a lethal blockers header whose AT-BEST figure is above 0 is a BLOCK, never a decline

**What the render prints** (`matchups-20260905-191148/1788653513-ai_baka_deck123-0x55ecd5fc5a70-vs-ai_baka_deck152.jsonl`, seq 31, turn 13, seat at 5 life):

    INCOMING THIS COMBAT: 3 attackers, 8 unblocked damage - you would be at -3; this KILLS you
    (of that, 4 from 1 attacker none of your creatures can block) - at least 4 of that lands
    whatever you block (trample/menace counted as unblocked): you would be at 1 AT BEST (no
    assignment of your blockers does better)

with twenty-six legal blockers printed below it (`B1-B26. Human #1-#26 (1/1) ... may block A2, A3`).
The seat answered `BLOCKS: none`, re-deriving the total in its own reply ("We take 8 damage, go
to -3, and lose"), and died at -3 on a board that survives at 1 with an army that wins the next
turn.

**Proposal.** A teach in the general guide, deck-agnostic, on reading a lethal blockers header:
the `this KILLS you` clause prices the UNBLOCKED case; the `you would be at N AT BEST` /
`one legal assignment gets you to N` clause on the same header prices the BLOCKED case, and that
second number is the verdict. N above 0 means block and spend every blocker. Decline only when
that clause is absent or its N is 0 or lower. Never re-derive either number.

**Scope, honestly.** ONE window, in the one game of six where the situation arose (1 of 1). It is
offered not on volume but because it lost a won game outright, because the mistake is a pure
reading error on a true surface (no engine defect is involved — the printed 1 was correct), and
because the two clauses read left-to-right are literally contradictory: the first says the combat
kills you, the second says it does not. R276/R303 in wave62/general-strategy.md already track
whether that second figure is ACCURATE; nothing there tells the seat that the figure being
survivable is itself the instruction to block.

**Relationship to the engine item.** deck123's review HIGH-2 proposes fixing this at the render
instead (state survival affirmatively and before the `this KILLS you` clause, or suppress that
clause when the AT-BEST figure is above 0). If that ships, this proposal becomes a restatement of
a true, well-ordered surface and should be REJECTED rather than adopted — the render fix is the
better lever and this teach only exists because the ordering currently buries the answer.

No skill proposals from this seat: the two HIGH items are instances of the existing
perception-routing and trust doctrine (amendments already in the wave59 skill), and restating
them would be noise.
