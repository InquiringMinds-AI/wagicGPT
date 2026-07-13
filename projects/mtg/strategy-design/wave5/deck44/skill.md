# Proposed revisions to the strategy-writing skill (from the deck44 wave-5 pass)

Two additive method refinements; the skill otherwise held up well (the attribution ladder,
the tell audit, and the brake/pursuit overshoot audit each did real work this pass).

## 1. Add a PLAN/CHOICE MISMATCH class to the Step-2 harness/leak sweep

The current sweep names the parser **DESYNC** (head-int of the reply != recorded `choice`). Deck44
surfaced a distinct, adjacent failure the sweep does not name: the reply's **own PLAN names a legal
listed action, but `choice` is a different (passive) option, and head-int == choice** (so it is NOT
a parser mis-grab). The model reasoned correctly, even named the exact card, then output the pass
number.

Proof: `e8528 s26`/`s49` (vs140) - PLAN *"I must counter it immediately... I will cast Arcane Denial
to counter Wrath of God"*, options `1. Cast Arcane Denial / 2. Cast nothing`, choice = `2`
(head-int 2 = choice 2, no parser desync). The board was wiped; it lost the game.

Proposed skill text (Step 2, after the DESYNC bullet):
> - **PLAN/CHOICE MISMATCH (intent collapse).** The reply's PLAN names an action that IS a numbered
>   option, but `choice` is a different, usually passive option, AND head-int == choice (rule out
>   DESYNC first). This is the model betraying its own correct plan at the number, not the parser
>   and not an unaffordable-plan reconcile. It is only weakly guide-mitigable (labeling the option so
>   the passive line reads worse than the action); the durable fix is representation/reply-protocol.
>   Distinguish it in findings from DESYNC (parser) and from plan-diverges-from-legality (the planned
>   card is not in the list).

This matters because attributing it to "passivity" (three-bucket thinking) would ship a louder
act-floor against a model that already stated the correct action.

## 2. A fact present only in the LOG TAIL is functionally hidden - the model FABRICATES over it

The skill's "when the prompt hides a deciding fact, teach the TELL" pattern assumes the fact is
either absent or present. Deck44 shows a third state: the fact is **derivable from the game-log
TAIL** (last `hand -> stack` with no `stack ->` after), the core prompt AND the guide both carry the
exact tell, yet the model **confidently asserts the opposite** - inventing a specific false event
(*"the log shows the spell moved to the battlefield"*, `e1248 s3`). This reads like a comprehension
error but behaves like a representation defect: the model treats the log tail as low-attention and
hallucinates over it, and no amount of restating the tell (already present twice) fixes it.

Proposed addition to that pattern:
> A fact that lives ONLY in the log tail is functionally hidden: a weak executor skims the tail and
> will fabricate a confident false reading over it (assert a resolution that did not happen), which
> is distinct from expressing uncertainty. When the reply asserts a specific false event that the
> tail contradicts, the fix is REPRESENTATION - surface the fact on a first-class CURRENT SITUATION
> line and/or label the option - NOT a third copy of the tell. Re-run the TELL AUDIT each wave: if
> the tail still hides the fact and the reply still fabricates over it, the representation ask has
> not shipped - re-file it with fresh repro rather than re-diagnosing it as wording.

## Not proposed
No change to the tell-audit method itself - it produced a clean 27/27 here and gave the confidence
to state the counter rule as an absolute, exactly as intended. No change to the length budget; the
pressure to compress the (obeyed) untap rule paid for the counter rework, as designed.
