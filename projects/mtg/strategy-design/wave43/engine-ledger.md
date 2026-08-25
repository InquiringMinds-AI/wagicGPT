# Wave-43 engine ledger — THE WAVE-44 DOCKET (assembled 2026-08-24)

## Open items (ranked)
1. **#W44-1 (HIGH) Rigged opening hands + aggressivity pin** (Rules.cpp OptimizedHand
   for MODE_AI x2): ship WAGIC_SELFPLAY_FAIRHAND=1 opt-in env routing to a->resolve();
   leave default unchanged pending owner ruling on corpus comparability. Also surface
   the agressivity+=100 pin as a second gate or document it. Until shipped, mulligan
   teaches are untestable (skill amendment 21).
2. **#W44-2 (HIGH) {X pricing:} headline scope**: "smallest X that kills anything"
   computed over BOTH boards — lead with smallest X killing an OPPONENT creature
   (seat-130 N1; falsifiable: Starstorm cast rate at kills>=2 windows 33% -> >=60%).
3. **#W44-3 (HIGH) Removal target clauses lack {target text:}** — the counter lane's
   exact fix, same shape (both deck125 HIGH losses lived in unseen text).
4. **#W44-4 (MED) Day/Night invisible in CURRENT SITUATION** (0/2190; 33 records render
   daybound creatures) — add game-wide state line.
5. **#W44-5 (MED) teferieffect stack-label path** (one-liner at stackAbilityName) +
   dungeon rows zone tag (sideboard home; or suppress tag on dungeon rows).
6. **#W44-6 (MED) Opponent-hand reveal FALSE FRAME** (library count line + hand size 0
   mid-reveal — E-2 trust breach).
7. **#W44-7 (MED) Plan-name-leak core-prompt item** — now 4/4 of all fallbacks; also
   audit the plan-mismatch note predicate (missed both firing sites).
8. **#W44-8 (MED) Menace DEFENDER-side probe deck** (corpus-unreachable; discharge the
   lane-A block-side fix with a stacked probe: menace attacker vs 2+ untapped walls).
9. **#W44-9 (MED) Battlement-tap economics**: mana-ability-only windows = 21.6% of a
   seat's inference — auto-pass windows whose only options are mana activations with
   no pending cost.
10. **(LOW batch)**: loyalty lost-lines (now N); narration targeting-clause ownership
    ruling (parity vs tags); counter preview single-object false affordance (1/57);
    (both die) omits blocker lifelink (E-3); EDIT C maker-just-landed clause (n=2).
11. **OWNER-CALL items**: DS+trample toughness->life one-token (two pinned asserts
    flip); {room effect:} history keep; deck123 construction case; mana-tap asymmetry
    (#W42-D2r, still parked).
12. **Carried**: CS-024a Boggart Trawler backside; manarestriction blast-radius sweep
    (85 cards, needs alsoabilities flavour); Docent/Ludevic follow-ups CLOSED (lane C);
    counter-clause keyword set; trample parenthesis 0-live-instances (unreachable —
    probe if ever needed); WSrcUnlockedCards bounds guard; Baka X=0.

## Discharged this wave
All owner narration-fix predictions (PASS across the board); wave-43 lane predictions
(menace attack side, reveal, transforms, DFC, silent cast, X pricing, counter text,
tags, parity, collapse); rung/edit adjudications per seat files; damage-order fix live
(the ORDER window governed decisions this corpus); assignment automation VERIFIED
correct incl. deathtouch+trample (5 pins, 0 code changes).

## OWNER RULING on #W44-1 (2026-08-24, verbatim): "I want legitimate hands. legit
## mulligans. none of this fixing hands in advance."
Fair hands are the DEFAULT — OptimizedHand and the aggressivity/forceBestAbilityUse
pins removed for AI-vs-AI, not gated. Comparability break accepted: wave-44+ corpora
are not hand-comparable with waves <=43 (mulligan seams live for the first time; win
rates and game-length distributions may shift for reasons unrelated to guides — the
first fair-hand corpus is a NEW BASELINE, and its review must not attribute
hand-variance effects to the wave's edits).
