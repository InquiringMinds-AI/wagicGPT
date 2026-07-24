# General-suggestions (core-prompt proposals, layer-routed) — deck109 wave-22

## Proposal 1 (CORE-PROMPT): guard the pathological repetition spiral at the reasoning tail

**Evidence:** vs62 s21 (attackers). After correctly emitting the answer-first line
`ATTACK: A1, A2, A3, A4`, the model entered a non-terminating loop — it wrote "I will declare no
attackers … but the strategy guide says attack … wait, maybe I can kill the Birds? No … Okay, I will
declare no attackers" and repeated near-verbatim ~15 times until it hit max_tokens (reply truncated
mid-sentence at 12457 chars). The board was genuinely confusing (a Briar Patch giving its attackers
-1/-0 against an unkillable 5/8 flier), but the failure is the LOOP, not the direction. This burned
the entire token budget and (via a downstream salvage arm — see deck109 notes Ledger A2) corrupted
the recorded attacker set.

This is model-behavior, general to every deck, so it routes to the core prompt, not the deck guide.
The answer-first protocol already puts the coded line FIRST (which worked here and is the reason the
truncation was survivable). The gap is that nothing tells the model to STOP after the coded line.

**Proposed core-prompt addition (adaptable, procedure-for-thinking):** after the reply-protocol's
"emit the coded line first" instruction, add a brief commit-and-stop rung, e.g. — "Once you have
written your coded CHOICE/ATTACK/BLOCKS line, keep your reasoning SHORT and FORWARD-MOVING. Do not
re-derive the same combat or mana math a second time, and never re-open a decision you have already
coded on the first line — if you notice yourself repeating a point, stop and end with your PLAN
line." This is a light nudge, not a hard cap; it targets the observed repetition shape without
constraining genuine multi-step reasoning.

**Caveat / seam note for synthesis:** verify this composes with the last-well-formed-CHOICE-wins
rule. Those two must not fight: last-CHOICE-wins deliberately LETS the model change its mind mid-reply
(vs62 s16 used it correctly to self-correct Cast-nothing -> Cast Pillar). The commit-and-stop nudge
should discourage RE-DERIVING and LOOPING, not forbid a single clean correction. Frame it as
"don't repeat / don't loop," not "never revise." The two are compatible: one clean revision is fine;
15 waffling repetitions to token-exhaustion is the target.

## Proposal 2 (PASS on everything else)

Reveal-framing, wither annotations, bounce-on-stack header, painland CAUTION, transform annotation,
N9 retraction detector, echo prefix-strip — none exercised at this mono-red seat (no reveal, no
wither, no bounce, no painland, no transform, no counters). No core-prompt signal from those surfaces
here.
