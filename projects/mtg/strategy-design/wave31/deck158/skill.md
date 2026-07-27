# deck158 — proposals for the strategy-writing skill (wave 31)

Seat context: Step-0 guideless debut, 5/6 with five clean kills — the strongest Step-0 showing
of the campaign. That result is itself the source of the proposals: the existing Step-0-bis
procedure is written for guideless seats that played BADLY, and it has no explicit handling for
a guideless seat that played WELL.

---

## S-158a (PROPOSED NEW RUNG, Step 0-bis) — the HIGH-PERFORMING Step-0 seat: write a PRESERVE LEDGER before you write a single rule, and size the guide to the leak, not to the deck.

**Evidence.** deck158 went 5/6 guideless with zero adjudications. Reading its corpus, the
behaviours that produced that record were: 30/30 full attacker declarations (zero
`ATTACK: none`, zero mixed-form drops), the amass-then-attack sequencing discovered unaided in
a won game (vs137 seq19), 11/11 defensible blocks, and correct impact-ranked removal targeting
in every instance. Meanwhile the ONLY loss traced to a single false belief at a single decision
(vs148 seq19). A conventional first-pass guide — Step 0-bis rule 5 says "write the ATTACK /
DEPLOY floor on the FIRST pass for any evasion or aggro deck" — would have opened with a loud
attack floor aimed at a seam with a **perfect** record, which is exactly the deck135
pursuit-overshoot shape being installed on purpose.

**Proposed rung.** When a guideless seat's record and its per-seam metrics are both STRONG,
Step 0-bis gains a step 0: **enumerate the PRESERVE LEDGER first** — the behaviours the corpus
shows the pilot doing correctly WITHOUT a guide, quantified per seam (attacker declarations vs
eligibility list, block windows, cast-nothing rate, target-selection hit rate) — and treat
every one of them as FROZEN territory the initial guide must not legislate over. Then write the
guide only for the residue. Concretely this inverts two existing defaults:
  (a) **Step 0-bis rule 5's "always write the attack floor on the first pass" gains an
      exception:** if the corpus shows 100% (or near-100%) full declaration against the
      eligibility list, write the floor as a SHORT confirmatory line ("keep declaring every
      creature the list offers you; the list already excludes creatures that cannot attack"),
      never as a loud #1-slot procedure. A guide's first duty at a strong Step-0 seat is
      do-no-harm; the turnaround-preservation logic of Step 0 applies to a guideless seat too,
      with the corpus standing in for the previous guide.
  (b) **The guide's #1 slot goes to the leak that traces to the LOSS, even if it is narrow.**
      deck158's #1 is a single false belief about one prompt line, not an archetype-level
      posture — and that is correct here, because the archetype-level posture was already
      right. Do not manufacture a posture rule to fill the #1 slot.
Tag: single-seat (deck158). Promote if a second guideless seat debuts at 4/6 or better.

---

## S-158b (PROPOSED — refines the reasoning-tax ranking, Step 0-bis rule 2) — rank tax spikes by DECISION KIND, and treat a tax spike on a TRIVIAL decision as the strongest possible saliency signal.

**Evidence.** Ranking deck158's replies by length, the top spikes were 13,319 chars / 208s on a
**land-drop** (vs137 seq18), 12,875 / 164s on a turn-1 **land-drop** (vs148 seq2), and 12,198 /
150s on another land-drop (vs152 seq22). A land drop is a two-option decision with an obvious
answer; the entire 13k was mana arithmetic and amass arithmetic the pilot was doing *ahead* of
the real decision. The existing rule 2 ranks candidate teaching points by tax magnitude, which
would have surfaced these — but a reviewer scanning by DECISION KIND would naturally skip
land-drop records as uninteresting and miss the campaign's clearest single-belief signal.

**Proposed refinement.** State explicitly that tax on a LOW-BRANCHING decision (land drop,
one-real-option cast window, single-eligible-attacker combat) is *more* diagnostic than the same
tax on a genuinely hard decision, because the deliberation cannot be about the choice — it is
the model rehearsing a belief it cannot settle. Sort the tax table and then read the
LOW-STAKES rows first; the belief they expose is usually the one that decides a game elsewhere.
(deck158: the land-drop spirals and the game-losing decline are the same belief, and the
land-drop instances outnumber the decisive one 3:1, so they are the better detector.)
Tag: single-seat; cheap to apply, no cost if wrong.

---

## S-158c (PROPOSED — extends the DEFER-CLASSIFICATION discipline) — classify a defer by its TRIGGER, not by its exit; and always look for the SAME-CARD counterfactual in the same game.

**Evidence.** deck158's single defer (vs146 seq25) has the divergence-C EXIT
(`aiForcedCandidate` validation rejecting an `offerable()`-approved cast -> silent defer ->
silent no-op, card back in hand) but no convoke anywhere in the deck. The discriminator was
found for free: the SAME card, SAME face, SAME seat, two turns later with three more untapped
lands, was offered, announced X, and resolved (seq27/28). One record pair isolated the variable
(X-affordability slack 0 vs 3) with no code reading at all.

**Proposed rung.** When a defer appears at a new seat, (i) name the EXIT (which is usually a
known one and tells you nothing new) separately from the TRIGGER (the board/cost condition that
distinguishes the deferring window from the non-deferring one), and (ii) before theorising,
grep the same game and the same corpus for the SAME action succeeding, and diff the two board
states. A same-card counterfactual within one game is the cheapest possible isolation and it
converts "another divergence-C" into a named, separately-fixable seam. Corollary: a defer whose
outcome was a NULL play (deck158's X=0 Assault resolves to a 0/0 token that dies to SBAs) is
low-severity but is the *cleanest* repro to hand a fix agent — record severity and repro
quality as separate fields, and do not let low severity suppress the ledger item.
Tag: single-seat, but it directly generalises the arrival-tracing rung already in the skill.

---

## S-158d (PROPOSED — a fifth annotation-defect shape for the magnitude-audit rung) — the CONSTANT annotation: a `{right now: N}` that never varies is a stronger defect than an offset, and the audit is "does it ever change?"

**Evidence.** deck158 carries two of these at once. Foray of Orcs printed `{right now: damage 0}`
in **2/2** instances where the true magnitudes were 2 and 9; Feed the Swarm printed
`{right now: life -2}` in **~44/44** instances where the true self-cost ranged 0 to 3 by target.
The existing rung (Step 5, "prefer an engine-SURFACED number... but VERIFY its OFFSET", from
Gray Merchant's constant +2) assumes the displayed value TRACKS the real one with a fixed
offset, and prescribes a stopgap stating the correction. Neither of these has an offset: one is
a constant, and one is target-dependent while the annotation is computed before the target
exists.

**Proposed refinement.** Add a CONSTANT-ANNOTATION check to the magnitude audit: before writing
any guide line that reads a `{right now: ...}` value, tabulate that value across EVERY instance
in the corpus and ask *does it ever change?* A magnitude annotation that is invariant across
materially different boards is not offset — it is inert, and there is no correction to state.
Two authoring consequences: (a) the guide must teach the underlying rule from the `auto=` and
say plainly which VISIBLE field the real number comes from (Foray: the Army's printed power
plus 2; Feed the Swarm: the mana cost printed on the target), accepting that this is the one
sanctioned case where guide prose names a different number than a rendered field — flag it in
the handoff as a stopgap with an explicit removal condition; and (b) the notes item should
prefer SUPPRESSING the clause over printing a wrong number, because a printed `0` actively
argues the pilot out of a good play in a way a missing clause does not (deck158 vs137 seq19 and
vs152 seq18 both spent thousands of characters arguing with the annotation before overriding it
— it cost tax in both, and would cost the cast outright with a less careful pilot).
Tag: 2 cards / 1 seat; converges with the existing Gray-Merchant rung, so propose as a
refinement of that rung rather than a new one.

---

## S-158e (OBSERVATION, not a proposal) — the EXECUTABILITY AUDIT paid for itself again, in the negative direction.

deck158's marquee card (Orcish Bowmasters, x4) has flash, and the natural first-guide instinct
is "hold it for the opponent's turn". The audit killed that rule before it was written: 6
priority windows in 6 games, **one** on the opponent's turn, and the one window where
Bowmasters was in hand with mana up did not offer the cast at all (vs152 seq27). No skill change
requested — the existing Step-2 FIRST step already mandates this and it worked exactly as
specified. Recording it because the audit's saves are usually invisible (they are rules that
never got written) and the rung is worth keeping load-bearing.
