# Skill proposals — deck109 wave-22

## Proposal 1 (METHOD REFINEMENT — small, real): pure-index attacker forms can ALSO silently under-record; score by intended-vs-recorded regardless of form

Wave-13's Ledger A method note scored attacker declarations by parsing the reply's `ATTACK:` line for
NAMED tokens and comparing intended-vs-recorded, on the premise that the FULL-INDEX form (`A1, A2,
A3`) "parses correctly" and only the mixed/name forms drop. **This wave falsifies that premise:** a
clean pure-index `ATTACK: A1, A2, A3, A4` (vs62 s21) under-recorded to 2 attackers when the reply
spiraled to max_tokens and a prose-salvage arm overrode the answer-first line.

Refinement for the skill's attacker-scoring method: **compare the coded answer-first ATTACK line's
declared set against the RECORDED set (chosen_text / next-record combat log) for EVERY window, not
only mixed/name forms.** The full-index form is NOT a safe-by-construction case — the salvage/parse
layer can still under-record it under truncation. A window-count or a chosen_text glance both miss
this (chosen_text shows the recorded subset, which looks internally consistent). The tell is
answer-first-line-set != recorded-set.

## Proposal 2 (WORKED EXAMPLE for the crutch-lifecycle rung — confirm, not new doctrine)

deck109 this wave is a clean second instance of the wave-21 crutch-lifecycle CLOSED-criteria firing
(after deck102's Thoughtseize retirement). Two representation crutches retired because the engine now
carries the fact AND the failure went to 0:
- **Hybrid mana lecture** (Rakdos Cackler "it is a RED card, NOT black … never decline for lacking
  black mana"): CLOSED — engine renders `[hybrid: each {b/r} pays with B or R - total 1 mana]`;
  hybrid-confusion markers = 0; casts + Unleash 100%.
- **Pillar cost correction** ("{R}, not {1}{R}"): CLOSED — engine renders `Cast Pillar of Flame {r}`;
  0 mis-costings.
No new rung needed — the existing rung (audit each interim crutch at revision; retire/demote when the
ledger shows the defect CLOSED) handled it exactly. Logging as confirmation that the rung generalizes
beyond reveal/discard picks to cost/mana-representation crutches. If the synthesis agent wants a
one-line generalization: "the CLOSED-criteria apply to ANY representation crutch (cost, mana, hybrid,
reveal, trade) once (i) the failure -> 0 and (ii) the durable annotation renders on the option line."

## Proposal 3 (PASS on new deck-guide doctrine)

No new guide-authoring doctrine surfaced at this seat. The EVASIVE/UNTOUCHABLE sharpen (ground
creatures can't block fliers -> attacking is free; commit fast) is deck-appropriate content, not a
transferable skill rung — it stays in the guide. The spiral/anti-repetition item is core-prompt
(general-suggestions), not skill.
