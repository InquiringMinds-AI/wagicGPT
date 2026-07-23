# deck102 skill proposals - wave 21

## PROPOSAL 1 (NEW rung): RETIRE THE CRUTCH when the representation defect it worked around is fixed.

Context: when a prior wave finds a REPRESENTATION defect (a mislabeled/misleading prompt) and writes an
INTERIM guide line to teach the model around it, and a later engine batch FIXES the representation, the
next guide revision MUST retire the stale workaround - it now describes a prompt that no longer exists and
can misdirect the executor toward a mislabel that isn't there anymore.

Live instance (deck102, this wave): wave-20 wrote a Thoughtseize paragraph explaining that the reveal menu
is "mislabeled 'top N cards of your library' - it is really the OPPONENT'S revealed hand" and that "only
the FIRST card you name is discarded ... do NOT list several." Wave-21 item (e) fixed the prompt to render
"The opponent revealed their hand ... Choose the ONE card ... PUT: followed by the ONE card number." The
old guide paragraph now (a) describes a mislabel the model will not see, and (b) fights an over-selection
behavior that no longer happens (0/2 over-picks this corpus). The wave-21 revision REPLACES it with lean
STRATEGIC content (which card to name; Tergrid-prefers-a-permanent) and drops the prompt-shape description.

The rung: at each revision, audit every guide line that was written as an interim crutch for a
representation defect; if the ledger shows that defect CLOSED, retire or re-point the crutch. A crutch that
outlives its defect is not neutral - it re-teaches a stale prompt shape and burns guide budget. This is the
inverse of the existing "teach the fact interim, fix representation durably" pattern: this is the "and
remove the interim once durable ships" second half. Distinct from the "present-and-disobeyed -> do not
restate louder" doctrine (that is about an OBEYED-or-not rule; this is about a rule describing a prompt
reality that changed).

Confirmation-read for a discard-pick reveal fix (companion to the deck135 reveal-partition lifecycle rung):
score it CLOSED only when (i) over-selection -> 0 across the seat's records, (ii) the named card leaves the
opponent's hand exactly once in `events`, AND (iii) the framing names whose hand and pick-EXACTLY-ONE.
"0 fallbacks this wave" is NOT the confirmation - the read is on the zone outcome and the pick cardinality.

## PROPOSAL 2 (application note, not a new rung): co-firing gate resolved by NUANCE, not a new hard rule.

The symmetric-trap gate had a latent co-firing tension (clause (c) "board near-empty so you lose little"
PERMITS while the hard "NEVER while behind/raced" FORBIDS; deck102 vs27 s14 satisfied both). Rather than
legislating a universal winner (brake-absolutism trap) or adding a fourth hard rule, the wave-21 fix ADDED
the missing Magic fact that resolves the tension in the model's favor of judgment: a symmetric sac is the
OPPONENT'S choice, so it removes their WORST creature and will not stop a specific evasive clock. This is a
clean instance of the existing co-firing doctrine (Method headline 1): when two clauses co-fire and BOTH
are sometimes right, supply the deciding FACT that lets the executor tell the cases apart, instead of
picking a universal winner. Recorded as convergence, not a new method.

## Everything else: PASS.
Answer-first (core-prompt) validated here but that is a shipped item, not a skill-method change. No new
authoring-method proposals beyond Proposal 1.
