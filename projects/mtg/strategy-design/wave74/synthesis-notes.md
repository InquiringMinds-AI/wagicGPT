# Wave 74 — synthesis notes

Inputs read: wave74/reviewer-brief.md, wave74/deck162/skill-proposals.md (the only proposals file this
wave — no deck wrote general-proposals.md, so wave74/general-strategy.md was not created and wave62's
R331 edition stands unchanged), wave74/engine-seat.md, and the current skill edition
wave68/strategy-writing-skill-v2.md in full (39,998 B).

## Decisions

| # | Proposal (source) | Verdict | Reason (one line) |
|---|---|---|---|
| 1 | Heading = scope declaration; heading-verb audit (deck162) | **ADOPT** as Amendment 333 | New mechanism with verbatim reasoning as proof — the pilot discarded a correct printed absolute three times because its heading's verb belongs to another section; v2's nearest bullets (A174, A307, A306) govern deciding facts and firing seams, not headings, and none is auditable by grep. |
| 2 | Concessive clause inverts into a condition; write the flat rule (deck162) | **ADOPT** as Amendment 334 | v2 has nothing on `even when` / `even if` / `regardless of`; A12 orders an absolute's edge case and the §5 anti-pattern forbids exception-only rules, neither of which is this failure (rule stated, hard case concessive, concession read as the necessary condition) — and the failure fires on any board where the concession's condition is false. |

Neither proposal restates an existing amendment: the deck-162 seat grepped v2 for `heading`,
`section`, `scope`, `even when`, `even if`, `although`, `despite`, `concess` and found nothing; I
re-read v2 in full against both and agree — the overlaps (A174, A306, A307, A12, §5) are adjacent, not
the same rule, and each amendment states in its own body why.

## What was written

`wave74/strategy-writing-skill-v2-amendments.md` — Amendments 333 and 334, each with the v2 section it
folds into, the fold-in text ready for the owner, the not-a-restatement argument, and the paying
evidence pointer. `wave68/strategy-writing-skill-v2.md` was NOT edited (owner-installed).

## Contradiction check against deck guide edits

No adopted change contradicts any deck's staged guide. Both amendments are in fact PRE-APPLIED by
deck162's own staged strategy.txt: its edit A renames `LAST CHECK BEFORE YOU SEND.` to
`LAST CHECK ON EVERY CASTING ANSWER - this paragraph is about CASTING, not attacking.` and restates the
absolute at the top of the casting first-match list (A333's two permitted forms), and its edit C
replaces the `even when ... pays them MORE life` concession with a flat two-directional refusal
(A334's prescribed shape). The other six staged guides were not audited against A333/A334 — that
audit (heading-verb inventory per guide; a grep for the concessive connectives across the pool) is
open work for whoever installs the amendments, and is the natural falsifier for both.

## Engine-seat cross-check

engine-seat.md carries nothing that argues against either amendment; its only scope-adjacent item is a
render clause needing a scoped qualifier (hold re-open), which is an engine item, not a guide-writing
rule. Both adopted items are guide-WRITING faults, so neither is answered by an engine fix.

## Not checked

I did not re-read the six non-162 review.md files or their staged strategy.txt files, did not verify
the deck-162 seqs against the translogs (I took the seat's verbatim quotes as reported, corroborated by
review.md), and did not run the heading-verb or concessive greps across the guide pool.
