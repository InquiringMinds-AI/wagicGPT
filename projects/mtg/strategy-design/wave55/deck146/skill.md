# deck146 -> strategy-writing skill proposals (wave 55)

Only what this seat's corpus evidence supports. Amendment numbering continues from 232.

## S-1 (PROPOSED) - an ordered prose rung is not executable on a long menu; write it as a
numbered SCAN with an explicit anti-anchor clause

**Evidence.** deck146's cleanup-discard rung has been stated as an ordered prose sentence
("the FIRST name you send is EMERIA'S CALL ... Then Agadeem's Awakening, then a second
planeswalker, then a land ..., then the priciest remaining spell") for two waves. It broke
**2 of 7** in wave 54 and **2 of 7** again in wave 55 (`146v130` s23, `146v162` s20), and the
breaks share a shape the rung does not address: **4 of the 7 answers begin `PUT: 1`**, and both
wrong answers include row 1. The rung and the pilot did not disagree about priority; the pilot
answered from the top of the list.

**Proposal.** When a rule ranks NAMES against a numbered menu whose order the engine chooses, the
skill should require three things the prose form lacks: (a) the rungs as a numbered list with
"STOP at the first name you find"; (b) an explicit **anti-anchor** clause naming the failure
("do not answer from the top of the list, and never answer PUT: 1 because row 1 is first");
(c) for any tail rung that says "the priciest/biggest/highest", a COMPARISON step
("read the {..} cost off each row and compare the numbers"), because a superlative over a menu is
an operation, not a recognition. Existing amendments cover superlative re-keys; none covers
list-position anchoring.

**Falsifier.** If wave 56's deck146 discards break at the same 2-of-7 rate with the scan form in
place, the anchoring diagnosis is wrong and the rung belongs in the ENGINE lane (see
`general-suggestions.md` R-1).

## S-2 (PROPOSED) - a render clause that ENUMERATES a set must say what the set is FOR

**Evidence.** `146v123` s24/s28. The row clause
`{right now: 24 tied at MV 0 (their highest): Human #1; Human #2; ...}` is a candidate list for an
edict; the seat read it twice as a kill list and wrote *"cast Soul Shatter to sacrifice all 24
Human tokens, wiping their board"* in two consecutive plans. The primitive
(`borderline.txt:106068`) is unambiguous: one card.

**Proposal.** The skill's render-truth doctrine says a printed clause must be believed. That makes
the QUANTIFIER on an enumerating clause load-bearing: any clause that lists N objects should state
in the same clause how many of them the effect touches, and the guide-writing side should be told
never to lean on an enumerating clause for a count without a stated quantifier. This is the same
family as lane C's `edictSelfClause` work; it is the missing half of it.

## S-3 (NOT PROPOSED, recorded) - "the engine never offers X" claims need a re-verification
line, not a re-derivation

deck146's guide asserts "EMERIA'S CALL AND AGADEEM'S AWAKENING ARE NOT LANDS FOR YOU ... the game
NEVER offers you their land face". Wave 55 re-confirmed it (`146v130` turns 21/23/25: three land-
drop-less turns at 2-3 lands with two Emeria's Calls and an Agadeem's Awakening in hand). The claim
is correct and cheap to check, so no skill change is proposed - but the check itself (grep the
corpus for a land-drop ask naming the card) should be recorded as the standing re-verification for
any "the engine never offers" sentence, since such sentences go stale silently the day the engine
is fixed.
