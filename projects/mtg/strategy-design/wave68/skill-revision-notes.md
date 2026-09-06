# Skill revision notes — strategy-writing-skill.md -> strategy-writing-skill-v2.md (2026-09-06)

## Source shape (confirmed)
- `strategy-writing-skill.md` = 1,306,741 bytes / 14,146 lines. Lines 1-9240 are 22 wave synthesis
  logs (wave 40 head, then "Superseded wave-N synthesis notes" 39 -> 5) plus the old prose skill body
  (Pitfalls ~7135, Definition of done ~7187, Output and handoff ~7350). Lines 9241-14120 are the
  numbered amendments (waves 41-58); lines 14121-14146 are Amendment 332.
- Sampled wave 33 (line 493) and wave 20 (line 3079): both are corpus reports (records, fallback %,
  "core PASS 28th consecutive", rotation), not rules. Method headlines inside them are review
  method; where one is a writing rule it already reappears as a numbered amendment later.

## Counts
- Amendments located: 331 of 331 (every number 1-331 found by `^\s{0,4}(## )?N\. ` in 9241-14120).
- Cited in v2 as a rule or method source: 305 distinct amendment numbers, in 101 bracket groups,
  76 of which merge two or more amendments into one statement.
- Kept as writing rules (sections 4-5): ~240. Kept as review-method rules (section 3, 6): ~55.
- Cited only as SUPERSEDED-in-guide by A332 (section 5): 61 (partly), 194, 232, 238, 272, 285(c),
  291, 305, 310, 320 (as guide text), 323 = 11.
- Dropped as non-rules (wave results, rotation, telemetry, engine forensics, ledger bookkeeping),
  26: 84, 117, 132, 167, 198, 208, 217, 222, 223, 230, 231, 240, 259, 260, 262, 263, 276, 295, 296,
  297, 298, 299, 300, 312, 327, 329.
- Could not locate: none. Could not classify: none — but see "not verified".

## Where the old skill contradicts Amendment 332 (guide content only; each still holds for review.md)
- #61 "every numeric threshold ships with a one-line WHY in that wave's edit-texts.md" — fine as
  written (outside the guide); contradicts only where seats copied the WHY into guides as wave text.
- #194 "cite the seat's own biggest decline ceiling as proof a decline is a normal answer" — a
  corpus count inside the guide.
- #232 "the size band is restated from the measured distribution each wave; net-change budget" —
  retired by 332's size ruling.
- #238 "a guide may cite a count as evidence for a rule" — banned; only the render-fact half survives.
- #272 "the sentence names the board class: '78 of 78 on boards with no DRAW PUNISHERS line'" —
  the scoping survives, the N/N does not.
- #285(c) / #320 "`REMOVE THIS RULE WHEN ...` lines in the guide; reviewer greps the guide for them"
  — removal conditions move to review.md.
- #291, #323 "one `WHAT THIS COST` paragraph per rule, newest corpus only, wave-named, seq-cited" —
  banned outright by 332's Supersedes clause.
- #305 "state the new baseline in the guide: 'Since wave 56 the row ...'" — wave citation.
- #310 "`perception status: CLOSED, wave NN — the row now prints <literal>`" — wave citation.
- #180 "or an explicit note that the matchup has not produced a loss" — history; dropped that half.
- #265 "state the engine-side residue as not the pilot's problem" — engine narration; kept only
  the register half (standard, not rebuke).
- The wave-40 head's "Definition of done" mixes review method with guide content ("WHAT THIS COST"
  ledgers implied by stopgap rows in the handoff); v2 keeps the greps and moves the ledger rows to
  review.md.

## What I did NOT verify
- I read each amendment's title and first ~800 chars (the extract), plus the full text of 46, 72,
  102, 182, 322 and the wave-40 head. Amendments whose rule lives in their final paragraph after
  long evidence (notably 119, 132, 151, 167, 186, 208, 232, 266, 300, 312, 330, 331 — the long ones)
  were classified from their titles and the first ~800 chars; a rule stated only in a tail clause
  could be under-represented.
- I did not check the wave 5-39 synthesis logs or the old prose skill body (Pitfalls / Output and
  handoff) line by line for rules that never became a numbered amendment. Three lineage rules were
  taken from them by grep: the frequency-adjective ban (wave-31 HL6 / wave-40 HL9), the cross-seat
  ladder (wave-40 HL3), the opponent-channel check (wave-40 HL2), and the cross-phase deciding-fact
  rule (wave-26 HL1, cited as lineage under A174).
- Tag accuracy: each bracket tag was placed from the amendment's title; I did not re-read every
  tagged amendment against its final placement. A tag may sit one bullet away from its best home.
- The v2 file itself hits the A332 grep 33 times — expected (it quotes the ruling verbatim and
  carries "wave-40 HL3"-style lineage tags); the grep is a GUIDE gate, not a skill gate. `deck125`
  appears once, inside the verbatim ruling.
- No guide was rewritten against v2 yet; v2 is unproven at a seat.
- Byte count: v2 = 39,894 bytes (ceiling 40,000; the 15-30 KB expectation was not met — the harvest
  of 305 rules at one or two sentences each did not compress further without dropping tags).
