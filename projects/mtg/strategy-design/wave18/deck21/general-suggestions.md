# deck21 seat — general suggestions (cross-deck / roster / trim) — wave-18

## 1. DECK-CONSTRUCTION (route to roster owner): three-wave flood pattern, now clearly the seat's ceiling.
Third corpus running, deck21's ONLY non-structural loss vector is a creature-light draw:
w16 vs133, w17 vs35, w18 **vs35 AND vs49** (both losses this wave had deck21 at **0 creatures for
the entire back half** — vs35 no creature until T8, vs49 none from T8 on). With the guide now
CONVERGED (both revisions validated, no decision error left), construction is the last remaining
lever on this seat's record. Suggested: trim 1-2 Mountains and/or add cheap redundancy so a
keep-able hand more reliably produces a T2-T6 body every turn. This is a ROSTER change, not a guide
change — the guide already deploys optimally when a body exists.

## 2. CONVERGED-GUIDE TRIM PROPOSAL (for a future DEDICATED pass — NOT executed this wave).
The brief asked whether this validation round should trigger the flagged trim. Verdict: **not now,
as its own pass later** (rationale in skill.md #4 — a trim must be re-validated and must not be
bundled with the freeze). When a dedicated trim pass is scheduled, the safe, behavior-neutral cuts
are the THREE-WAY restatements of already-validated rules — collapse to one canonical statement +
short pointers:
  - **Race-math / power-floor** is stated in full in the BURN section (lines ~35-53), restated in
    the MATCHUPS "SMALL-FLYER swarm" bullet (~111-116), and restated again in SITUATIONS (~135-139).
    Keep the BURN-section canonical block; reduce the other two to one-line pointers.
  - **Kill-on-sight engine** appears in the BURN EXCEPTION (~28-34) and again in SITUATIONS (~132-134).
  - **Deploy-and-attack / never-Cast-nothing** appears in the #1 RULE (~8-13) and SITUATIONS (~125-126).
  DO NOT touch: the "6/1 is 6 power / 1 toughness" numeric worked example, the power-floor worked
  example (BoP), or the hybrid-{R/G} annotation note — these are the load-bearing worked examples
  the model quotes. Estimated safe reduction ~15-20 lines with zero behavior delta IF the trim is
  validated on a corpus that includes a 62-type (0-power dork) and a 49-type (token-engine) opponent.
  CAUTION: the redundancy plausibly *drove* the clean power-floor firing (model quoted it from more
  than one section) — trim conservatively and re-validate before shipping.

## 3. Kill-on-sight ENGINE clause: consider generalizing the example set (LOW priority, post-trim).
The clause now demonstrably fires on token-makers (Dragonmaster Outcast), lifelink flyers (Vault
Skirge), and attack-anthems (Signal Pest battle-cry). The Signal Pest firing was the one slightly
loose call — the model justified it as a "mana engine" (factually wrong; Signal Pest makes no mana)
yet reached a defensible action (battle-cry snowballs a go-wide unblockable Affinity deck + it
enables Cranial Plating). If a future editor wants the reasoning to match the action, the engine
example list could add "an attack-anthem / battle-cry lord that pumps a go-wide board you cannot
block." Not urgent — the outcome was correct and the seat is freezing.

## 4. Cross-seat engine item (also in notes.md): the giant-reply pocket wants a length/format guard.
All 4 deck21 fallbacks and the deck62 seat's 5 unparsed share the computed-P/T-distrust pocket
(over-narrating an un-killable high-toughness body, missing the CHOICE line). This is an engine/
parser opportunity, not a per-deck guide fix — see notes.md. Flagging here because it now spans ≥2
seats and would lift the corpus fallback floor if addressed once centrally.
