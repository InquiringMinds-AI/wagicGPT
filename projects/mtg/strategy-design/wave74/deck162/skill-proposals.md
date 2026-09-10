# deck162 → strategy-writing skill proposals (wave 74)

Current edition read in full before writing: `wave68/strategy-writing-skill-v2.md` (39,998 B). Neither
proposal restates an existing amendment — I grepped v2 for `heading`, `section`, `scope`, `even when`,
`even if`, `although`, `despite`, `concess` and found nothing on either mechanism. Both are paid by
verbatim reasoning from this wave's deck-162 seat, and both are guide-*writing* faults, not deck content —
which is why they go here and not to the general guide.

## Proposal 1 — A section or paragraph HEADING is a scope declaration, and the pilot enforces it

**Proposed text.** *A guide's headings are read as scope. A rule under a heading is applied to that
heading's subject and refused elsewhere, so a heading verb that another section owns as ITS verb hands the
rule to that section. Audit every heading against the guide's own verb inventory: no heading may use a verb
another section uses imperatively (`SEND`, `BLOCK`, `ATTACK`, `KEEP`, `BOTTOM`, `CAST`). A rule that must
hold across sections either names its scope in its own first clause ("this paragraph is about CASTING") or
is restated at the top of the first-match list every window is read from — a rule filed once, under a
heading, is a rule with a scope.*

**Evidence.** deck 162's guide had `LAST CHECK BEFORE YOU SEND.` at the end of its brake section, carrying
an absolute (`A row you picked whose VERDICT begins "taking this row STRANDS" -> your answer is WRONG`).
Its §ATTACKING opens `SEND when the A-line prints NO tag at all`. The pilot, in one casting decision
(`1788968846-ai_baka_deck162-...-vs-ai_baka_deck123.jsonl` seq 10), re-derived the scope from the heading
three separate times and discarded the rule each time:
- *"This is under 'LAST CHECK BEFORE YOU SEND.' which is under ATTACKING. It might not apply to casting."*
- *"It's under 'LAST CHECK BEFORE YOU SEND.' which is in the ATTACKING section. It's safe."*
- *"I will follow it just in case, but it's under ATTACKING. I'll stick with 1."*

The absolute was correct, printed, quoted, and lost to its own heading. Two further takes of the same row
in the same corpus (126 seq20, 130 seq27) produced all three of the corpus's `plan_names_stranded_card`.

**Why it generalises.** Every deck guide in the pool is sectioned by `=== HEADING ===`, and every section's
imperatives use a small shared verb set. The failure needs no unusual board — only a heading and a pilot
that reads it. The fix is cheap (rename, or add a scope clause) and mechanically auditable (grep each
heading's verbs against the imperative verbs of every other section).

## Proposal 2 — A concessive clause ("even when X") inverts into a condition; write the flat rule instead

**Proposed text.** *Never carry a rule's hardest case in a concessive clause. "Take A even when B" is read
as "take A when B", so on a board where B is false the pilot takes the other row and cites the guide for
it. State the rule flat and then close BOTH directions of the quantity it is refusing to trade against
("the printed price never decides this row: A goes when it costs more, and B stays when it would cost
less"), and name the losing trade in the pilot's own words as the thing being refused.*

**Evidence.** deck 162's §FORCED SACRIFICE read `... and FATE UNRAVELER only as the sole row - take the
wall even when its toughness pays them MORE life.` At `...-vs-ai_baka_deck126.jsonl` seq 11 the two rows
were `Shield Sphere (0/6) ... [they gain 6 life]` and `Fate Unraveler (3/4) ... [they gain 4 life; THIS IS
NOT JUST A BODY: DRAW PUNISHER]`. Reasoning verbatim:

> *"Wait, let's check the guide: 'ORDER: SHIELD SPHERE first ... then FOG BANK, then MASTER OF THE FEAST,
> and FATE UNRAVELER only as the sole row - take the wall even when its toughness pays them MORE life.'
> But here, sacrificing Fate Unraveler pays LESS life (4 vs 6). The guide says to take the wall even when
> its toughness pays them MORE life, but here the wall pays MORE life anyway. So sacrificing Fate Unraveler
> is definitely better."*

The concession was quoted accurately and then used to justify the exact play it exists to forbid; the seat
sacrificed its only draw punisher on turn 9 of a game it did not close until turn 19.

**Why it generalises.** `even when` / `even if` / `regardless of` clauses are the natural way to write a
rule's hard case and appear across the pool wherever an ordering has to beat a printed price. The
inversion needs no unusual board either: it fires whenever the concession's condition happens to be false,
which is half of all boards. The check is a grep for the concessive connectives in every guide.

## Not proposed

Nothing on the K-brake / body distinction (deck-162 card content, handled in the deck guide) and nothing on
the hold latch (an engine item, in review.md §2). No general-guide proposals from this seat: the general
guide is not loaded at runtime, and neither finding is about play.
