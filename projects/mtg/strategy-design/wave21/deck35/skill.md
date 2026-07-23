# deck35 -- proposals for the strategy-writing skill (wave-21, FROZEN guide, 4th guided corpus)

All items single-seat; tagged; layer-routing respected (engine/harness -> notes.md; decode -> notes/
general-suggestions). This wave's theme: an engine fix I own VALIDATED CLEAN and a prior root-cause
hypothesis was SUPERSEDED -- a method rung about how to close an instrumented symptom honestly.

## S1 [SHARPEN -- closing an owned symptom means re-running the instrument for the OUTCOME COUNT, and being willing to SUPERSEDE your own prior root-cause when the fix's behavior demands it]

Extends wave-20 S1 (a fix validated-as-firing is not a symptom validated-as-gone). Wave-20 I ran the
instrument, saw the symptom persist, and REFUTED the batch's payment attribution -- naming a
"combat-gate non-issuance" bug instead. Wave-21 the batch reframed it (all-or-nothing sparing tapped
the attackers; gate always correct), shipped partial sparing, and the instrument now returns the
CORRECT reading: 0-attacker games gone, attacks appear wherever attackers end untapped, no land-payable
Sliver ever mana-tapped. **Rung:** when the post-fix corpus validates a mechanism you previously
rejected, say so PLAINLY -- "my wave-20 hypothesis is superseded," not a soft "additive" hedge. The
instrument's job is to track the symptom; it correctly caught a real symptom AND correctly flagged a
wrong mechanism, and the honest close is to update the mechanism to whatever the fix's behavior proves.
The credibility of the instrument comes from letting it overturn your own prior call, not just the
batch's.

## S2 [NEW -- the strongest validation of a payment/sparing fix is the MAXIMUM-mana-pressure positive case in a deck that LACKS the confound, not the absence of the symptom]

The clean proof that partial sparing works was not "vs49 no longer 0-attacker" (confoundable by draw:
this game drew Heart/haste). It was **vs14 T8: a {4}{g}=5 mana 5-drop paid from all 5 lands, leaving
THREE non-haste attackers untapped, all three then attacked.** A no-haste deck removes the haste
confound entirely, and the full-mana 5-drop is exactly the state where the OLD all-or-nothing logic
would have tapped an attacker. **Rung:** to validate a sparing/payment fix, hunt the highest-mana-cost
last-cast in the deck that has NO alternative mechanism masking the symptom (here: no haste enabler),
and read the post-cast tapped-state of the pre-existing attackers directly. That single render is worth
more than a symptom-count across confounded games.

## S3 [CONFIRM -- rotation candidacy is triggered by the DISCHARGE of the specific keep-reason, not by guide maturity alone (wave-20 S3, now discharged)]

Wave-20 S3 decoupled "guide converged -> FREEZE" from "seat has no new work -> ROTATE" and KEPT the
seat on a NAMED keep-reason: the open non-issuance engine work this deck's shape uniquely surfaced.
Wave-21 that exact keep-reason discharged -- the fix landed, validated clean, hypothesis settled.
**Rung:** when you keep a seat on a specific, stated work-item, the rotation test on the NEXT corpus is
literal: is THAT item closed? If yes and no new seat-specific item replaced it, rotate -- do not
manufacture a fresh reason to hold a converged seat. deck35's residuals (answer-first stale-line watch;
decode-tax) are both non-seat/cross-deck, so they do not re-earn a keep. State the candidacy plainly
even when your own prior wave argued to keep; the discipline is following the discharge condition you
yourself wrote.

## S4 [CONFIRM -- answer-first turns a fragile-parse instrument reading into a solved one; re-classify the metric it measured (wave-20 S2, resolved)]

Wave-20 S2 flagged a "0 fallbacks but FRAGILE" state: 14k repetition loops one bad decode from
truncating unparsed. Answer-first resolved the fragility (coded line first survives truncation), so this
wave the 13k replies parse clean and the metric to watch shifts from PARSE-RISK to TOKEN/LATENCY-COST.
**Rung:** when a harness change removes a failure MODE an instrument tracked, do not keep scoring the
old mode at zero and calling it a win -- re-point the instrument at the residual (here: reply-length as
a cost metric, not a parse-risk metric) and route it to the layer that owns the residual (decode
sampler), not the guide. A "0 fallbacks" that USED to be fragile and is now robust is a genuine
improvement only once you can name WHY the fragility is gone.
