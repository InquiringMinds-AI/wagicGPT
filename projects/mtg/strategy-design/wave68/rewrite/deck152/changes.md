# deck152 guide rewrite (Amendment 332)

## (a) Rules KEPT
- Mana: trust "Mana available" N + colour list; never re-derive.
- Generic is not a colour; N >= cost + every coloured pip listed = payable.
- A numbered "Cast ..." row is payable; never answer a number not on the list.
- One land pays one pip (Beach W-or-U, Farmland G-or-W); Teferi/Sigarda pip math; missing card -> take best row, move on.
- Land drop is never a choice: always play the land; take the colour the hand needs.
- Pathways: both faces have a row; either is one land drop; take the face that unlocks a card.
- Lethal count (untapped, non-sick power vs printed life + "0 are creatures") outranks the deploy floor.
- "{paying this taps: ...}" clause: 2+ names = pass in Upkeep/Main 1; 1 name only for a power-2+ body.
- "Those sources, one per untapped card:" loses to the clause on the row being taken.
- Deploy floor + its three exceptions (held Absence, lethal cleared, legend-rule row).
- Quiet board / low life are reasons to deploy, not wait; never hold creatures to dodge removal.
- Attack criteria; "ATTACK: none" test; A-line proves untapped/unsick/legal.
- Tag verdicts: kill-it -> send; all-"(neither dies)" -> only when outnumbering or evasive; Shield Sphere exception.
- "your attacker dies, their blocker lives" = per-creature stop; price the worst name; vigilance/lifelink are not licences; tokens exempt.
- GANG BLOCK clause overrides the parenthesis; flying does not exempt Sigarda; no gang clause -> use the parenthesis.
- Blocking triggers (Perimeter Captain 2, Pride Guardian 3); first strike does not cancel; 2+ trigger blockers -> fliers only.
- Blocking legality: "may block" list, one blocker per attacker, several blockers on one attacker, menace 2n rule.
- Parenthesis is computed truth; never override it.
- "you would be at N" first; "BLOCKS: none" only at all-rung-3 AND N >= 10; survival before the ladder; the 0-3 ladder; trample chump math; non-trampler stops all damage.
- Katilda's B-row and A-row exceptions at N >= 10.
- Card facts: Adversary valor counters, Sigarda two white sources, Katilda mana + Main-2-only ability, Aspirant counter placement + 6/6 test, Ranger Class levels, Cathar exile-returns + Moonrage ward "[opponent's stack]" row, Huntmaster/Packleader attack profit, Fateful Absence last-answer + "YOUR OWN" stop, Spellbinder tax + Blood/Bond/Idyllic Tutor priority, Teferi 3-stage cascade + [tapped] picks + tap-half limits + -2 rule, Lair of the Hydra three pass-forms + Main-1-only animation.
- Mulligan: read M first; M<=5 keep with one land; M=6 branches; M=7 floor; one-land coverage test; count-vs-colour bracket families; Pathway face-colour addition; forbidden thought; tiebreakers.
- Bottoming preference order; cleanup discard PUT order and protected cards.
- Hold-priority row rule and the "identical option lists" caveat.
- Matchups: counterspell U-count branches, grind/aggro deploy posture, lifegain/fliers, infect counter arithmetic + Katilda 7/8 threshold, converter both-names loop vs one-name evasion race (N-header, twice-the-gain rule, hold criterion).

## (b) DROPPED, by category
- Every "WHAT THIS COST" / cost-ledger paragraph and all game, seat, corpus and record citations (~14 KB).
- Narration of past behaviour ("this seat wrote...", counts of declined windows, "nine windows offered...") (~6 KB).
- Repeated anti-rationalization rebuttals restating a rule already given positively (~14 KB).
- Duplicated statements of the same rule across RULE headers, DECIDING SITUATIONS and card entries (~9 KB).
- Rule numbering/cross-references (#1 RULE, #2a RULE, RULE -1) and meta-commentary about the guide (~2 KB).
- Marginal display trivia: "{0}" artifact, printed-size tag, Coven prose, Day/Night mechanics detail, Triumphant Adventurer entry (~1.5 KB).
- Removed to fit the ceiling: the standalone "your plan is a memo" section (the plan rule survives inside ATTACKING and MULLIGAN), and 4 of the 13 DECIDING SITUATIONS lines whose rule is stated verbatim in the body.

## (c) Rules that could NOT be restated as condition -> action
- None. Two were folded rather than dropped: "coven happens by itself" is an instruction not to act, and "trust the engine's hand counts" is a source rule rather than an action; both are kept as stated constraints.

## (d) Verification
- `stat -c %s strategy.txt` = **19974** (ceiling 20000).
- `grep -ciE 'corpus|wave[- ]?[0-9]|\bseq\b|previous game|last wave|this wave|census|paid (for )?by|[0-9]{3}v[0-9]{3}|you (declined|took|cast|passed) [0-9]'` = **0**.
