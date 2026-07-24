# skill.md proposal — deck102 wave 22

## PROPOSAL: crutch-lifecycle DEMOTE — audit for clauses the fix made FALSE, not merely redundant.

The wave-21 skill's Method-headline-3 (crutch lifecycle: RETIRE if the rule is gone, DEMOTE if the rule is
intact and the engine now carries it) is confirmed at a THIRD surface class this wave. Wave-21 demoted a
marker-string crutch (deck14) and an option-line-annotation crutch; deck102 this wave demotes a crutch whose
durable fix is an ENGINE CAUTION LINE + a per-permanent battlefield TAG (R-PAINLAND: "CAUTION - ... Ancient
Tomb: 2 damage" + "[tapping for mana deals 2 damage to its controller]"). Same terminus, verified the same
way: (i) misplay-count -> 0 (the self-death class stopped) AND (ii) the FIRED signal is visible in the
prompt. So the CAUTION-line / battlefield-tag shape joins the marker-string and option-annotation shapes
under the demotion terminus. No new rung needed for that.

The **refinement worth adding** is a sharper sub-case of DEMOTE:

**When you demote a crutch after its durable fix ships, audit the crutch text for clauses the fix has made
factually FALSE — those are not redundant, they are LIES the model will read against the new representation,
and they must be removed, not merely trimmed.** The wave-21 painland crutch said "the 'Mana available' line
does NOT warn you of that cost." After item (e) shipped, the line DOES warn (the CAUTION line sits right
there). Left in place, that clause tells the model to distrust a signal the engine is now actively giving it
— strictly worse than a redundant crutch (an attention tax) because it contradicts ground truth in the same
prompt. The demotion terminus already fires on "confirmed FIRED + OBEYED -> double coverage"; this adds:
"...and if the crutch narrated the ABSENCE of the signal the fix now provides, that narration is now false —
delete it, do not paraphrase it." Cheap two-line addition to the demotion rung; caught a real false clause
here.

## PROPOSAL: name the "unexercised keystone blocked by a NEW representation defect" state.
deck102's steal engine (Tergrid) is 0/18 across three corpora. This wave the reason advanced from pure draw
variance to a compound: draw variance + mana requirement + a newly surfaced representation defect
(R-DFC-FLIP: the "Flip Side" DFC toggle the model cannot convert into a cast). The skill's probe-deck rung
already covers "validate a rare seam directly rather than waiting on the 1-of"; the small addition is that a
probe deck must ALSO clear any representation defect on the path to the seam first — validating the steal
through a DFC-flip that the model thrashes would test nothing. I.e. sequence the probe: fix R-DFC-FLIP, THEN
probe the steal. Optional; fold in only if the probe-deck rung is revisited.

(No change proposed to the reveal pick-ONE confirmation-read rung or the answer-first discipline — both held
clean at this seat this wave.)
