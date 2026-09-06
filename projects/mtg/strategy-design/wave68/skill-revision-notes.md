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
  24 (after Revision 2 restored 84 and 259): 117, 132, 167, 198, 208, 217, 222, 223, 230, 231, 240,
  260, 262, 263, 276, 295, 296, 297, 298, 299, 300, 312, 327, 329.
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
- Byte count: v2 = 39,982 bytes after Revision 2 (ceiling 40,000; the 15-30 KB expectation was not met —
  ~307 rules at one or two sentences each did not compress further without dropping tags).

## Revision 2 — findings adjudicated (Astra review, skill-v2-codex-review.md)

Each finding was checked against the cited v2 lines and the original amendment text before acting.
- #1 CONFIRMED (v2 literal audit + checklist 4 ordered deletion of any zero-hit literal). Fixed: delete
  only where the window arose and the string did not print; window never arose / deck cannot produce
  it = UNTESTED, rule STAYS, anecdote may compress. [A200, A283 added]
- #2 CONFIRMED (A84 was in the dropped list). Restored under the constant-floor bullet: count crossings
  ABOVE and compliance failures BELOW; an inert threshold is REPLACED by an evidenced condition; the
  WHY is re-measured each wave and a confirmed WHY that no longer implies its rule is a deletion.
- #3 CONFIRMED (A259 dropped; audit tested presence only). Restored as the audit's fourth check: a
  computed number in a literal is verified against the record's game-state field, full population
  where the field exists or a stated sample where not.
- #4 CONFIRMED (blanket "never state what the render does not print" contradicted the A139 bullet).
  Replaced with A238's qualified permission (re-verification grep + explaining primitive + fix-detecting
  prediction, recorded in review.md); the A139 bullet now points at those conditions.
- #5 CONFIRMED (A105's template rule was applied to all worked situations). Section 3 now separates
  explanatory SITUATIONS (concrete board + right answer, required by A332/brief) from REPLY TEMPLATES
  the guide asks the pilot to emit (shape with blanks).
- #6 CONFIRMED (blanks "filled BEFORE the CHOICE line" vs "PLAN written AFTER"). Resolved: blanks are
  computed first, CHOICE written first, PLAN second carrying the numbers (A282 governs). A330 restored
  as ONE ritual per guide; an unavoidable second names its owning window class and what to write
  elsewhere, and the rarer, higher-stakes ritual displaces the frequent one.
- #7 CONFIRMED (checklist 8 dedup vs 4.3 cross-phase restatement). Checklist 8 and the one-rule-one-
  place bullet now carry the exception: a deciding FACT is restated at each phase where it decides.
Other changes: sections 1, 2.2, 3, 4.x, 5, 6 tightened for bytes (no rules removed; 4.5's history,
cross-guide and hard-cap bullets now point at section 5 / checklist 10 / the 2.2 doctrine line).
A84, A200, A259, A283 no longer dropped; dropped-as-non-rule count is 24 (23 after Revision 3). v2 = 39,982 bytes (ceiling 40,000).

## Revision 3 — findings adjudicated (Astra round 2, skill-v2-codex-review-r2.md)

Verified each against the cited v2 lines and A200/A243/A283, A79/A282, A153, A198, A287, the reviewer
brief (staged wave68/deck<N>/strategy.txt) and the rewrite brief (bin/Res forbidden) before acting.
- #1 CONFIRMED regression. Retention now keyed to REACHABILITY (A200): window never arose but reachable
  = UNTESTED, keep; deck structurally cannot produce it = cut (consistent with A243, now tagged there).
- #6 CONFIRMED not resolved. The A79 "first line of your answer is <number>" instruction is removed and
  marked retired; the number is written in the PLAN line; order is compute -> CHOICE -> PLAN (A282).
- #8 CONFIRMED. A153(b),(c) restored: 2 lands / no coverage at keeping 7 and 6 added to the hand set;
  where a second shippable property survives, the look-N PLAN is tested against the look-N+1 hand.
- #9 CONFIRMED. A198 restored: the distinct (game, turn) count is reported beside every N of M.
- #10 CONFIRMED. The perception classifier carries A287's route-around exception (arithmetic over two
  true printed surfaces, retirement condition in review.md); the 4.2 bullet now points at it.
- #11 CONFIRMED. Section 1 now says seats never edit bin/Res, distinguishes ordinary review (staged
  strategy.txt beside review.md, orchestrator installs) from the rewrite workflow (guide-rewrite-brief.md
  is the contract there), and drops "the whole contract".
Room: prose tightened (prompt-surface inventory, "Read the games", KEEP, section-5 history bullet, combat/
holds/land/matchups/4.1 wording) and four section-5 anti-pattern bullets that only restated 4.1 rules
were removed (tags live in 4.1; A14 moved to the cap bullet). No rule removed. v2 = 39,990 bytes.

## Revision 4 — findings adjudicated (Astra round 3, skill-v2-codex-review-r3.md)

Verified against A119 (10743-10761), A153, A222, A284, A69, the rewrite brief 21-27 and the cited v2 lines.
- #12 CONFIRMED. The fixed floor / fixed stop-look is gone; mulligan is odds reasoning (deck land + colour-source
  counts, each look priced as a fresh seven shaved by one card), bottoming is a preference ORDER satisfiable at
  every N, no NEVER-bottom absolutes; consistent with the 2.2 doctrine line. A66/A67 stay as lineage tags only.
- #13 CONFIRMED. A284's threshold restored in "What earns bytes" and the marker bullet: a derivation is retired only
  after N/N marked-row takes across two corpora, with the "a miss restores the derivation" prediction.
- #14 CONFIRMED. A222 restored: bottoming orders validated only by offers/ships/bottoms with the reply-implied order;
  two or three samples cannot discriminate — report, withhold verdict, UNTESTED-AT-THIS-SEAT.
- #15 CONFIRMED. 2.2 states thesis -> engine -> rules with the mulligan rule as the FIRST rule under rules (A69's
  placement reconciled with the brief); 4.4 and checklist 5 say the same; "top of the file" removed.
- Dropped set re-read in FULL: A117, A132, A167, A208, A217, A223, A230, A231, A240, A260, A262, A263, A276, A295,
  A297, A298, A299, A300, A312, A327, A329 each carried seat-side method and are restored (new "Counting discipline"
  and "Predictions" paragraphs in section 3; A297 in the perception classifier; A167 under KEEP). A296 has no
  seat-side method — an engine-lane rule (a lane that removes a gate ships the legality test the gate enforced);
  it belongs in lane briefs and is the only amendment now untagged. Tagged: 330 of 331.
Room: prose only — 4.5 merged into 5, checklist compressed to pointers, illustrative parentheticals and
section-3/4.x wording tightened; no rule removed. v2 = 39,995 bytes.

## Revision 5 — findings adjudicated (Astra round 4, skill-v2-codex-review-r4.md)

Verified against A208 (12111-12115), A240 (12649-12652), A327 (13985-13989), A217 (12276-12287), A319 (13842-13849).
- #16 CONFIRMED. "rendered prompt, never options_text" replaced by choose-and-DISCLOSE: rendered prompt for a literal
  inside an option row; options_text / prompt / chosen_text each valid for their own measurement; a byte-identical
  options_text repeat stays a valid pinned predicate.
- #17 CONFIRMED. A217's normalisation restored: windows compared PER SEAT-TURN against the previous corpus to
  separate lost opportunities from shorter games; the (game, turn) count is stated as clustering, not exposure.
- #18 CONFIRMED. A319's rejection restored with precedence: obeying a marker is conditional on its definition
  matching the deck's goal; for a TRADE marker the guide counts THEIRS/YOURS and REJECTS the mark when YOURS is
  longer, outranking "answer the mark" until the render fix; named as the sole exception to no-derived-counts.
Room: prose only (illustrative quotes and section 3 / 4.x wording); no rule removed. v2 = 39,964 bytes.
