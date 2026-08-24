# Wave-42 engine ledger — THE WAVE-43 DOCKET (assembled 2026-08-24)

Sources: wave-42 seat reports (wave42/seats/), engine-validation verdicts, the crash
investigation (in flight), carried items. Ranked.

## Open items (ranked)
1. **#W43-1 (HIGH) MENACE, both sides.** (a) Blocker-side: MTGRules.cpp:2747 silently
   strips <2-blocker assignments off MENACE attackers AFTER declaration — option list,
   trade parenthesis, and narration all claim the block happened (8/8 dropped, ~24-life
   swing). Fix at render/legality: drop menace attackers from B-lines when <2 blockers
   assignable + tag the A-line; THREEBLOCKERS same. (b) Attack-side: lane B's blocker
   filter applies FLYING legality but not MENACE (11 semantically-wrong 1-on-1 tags;
   menace attacker must list "[cannot be blocked by fewer than two creatures]" and drop
   the tag when <2 untapped bodies). PARSETEST cases named in both seat reports.
2. **#W43-2 (HIGH) Vita victory-screen crash** — heap corruption at MTGLibrary teardown
   (core-dump analysis + ASAN lane in flight; whatever it finds lands here).
3. **#W43-3 (HIGH) Hand-reveal attribution inverted** — reveal events bind to the
   EFFECT'S CONTROLLER, not the card owner (24 lines, both seats mirrored wrong).
4. **#W43-4 (HIGH) Transform-DFC hand-flips residual** — Brutal Cathar x5 + Tovolar's
   Huntmaster x2 offered Flip Side from HAND (borderline.txt sweep, lane F's F3; same
   per-card Oracle discipline; modal-DFC doubleside(<name>) forms are LEGITIMATE).
5. **#W43-5 (HIGH) Transforms never narrated** — 0 transform lines corpus-wide while
   boards silently swap name/P/T between records.
6. **#W43-6 (HIGH) Silent cast drop** (seat 130 N1): chosen+confirmed Cast Starstorm
   produced NO announce/cast/resolve event, card back in hand 3 records later
   (deck125 seq293-298). Arrival-tracing defect in the cast pipeline.
7. **#W43-7 (HIGH) X-spells render unpriced** (seat 130 N2): no {right now:} magnitude
   on {X} damage spells — annotate smallest lethal X + kill list (root cause of the
   corpus's costliest misplay class).
8. **#W43-8 (MED) Counter-option lines carry the SPELL's text, never the TARGET's**
   (seat 125 INFO 2) + counter-window coverage 3/12 on listed engines (check interrupt
   OFFERING when seat holds mana + counter — seat 125 INFO 1).
9. **#W43-9 (MED) Ability-target rows lack owner tags** (seat 130 N3 — caused a
   self-inflicted land destruction; guide backstop shipped as interim).
10. **#W43-10 (MED batch) Observer narration parity**: targeting clauses dropped
    (0/302 vs 64/309), #N handles dropped, verb-case divergence (two render paths where
    lane C claims one) — one parity pass.
11. **#W43-11 (MED) Event-log run collapse** (52 runs >=3 identical lines; loyalty
    ticks worst — one line with resulting total) + Day/Night rendered as a battlefield
    permanent (66 board entries; game-wide designation, inflates counts).
12. **#W43-12 (MED) MDFC flip-then-play re-enters a committed ask** with zero-option
    menus (the deferred_to_heuristic root; fix the re-entry, not the defer).
13. **LOW batch**: teferieffect counter label; mana ability self-named lowercase;
    doubled verb "put a +1/+1 put counter"; dungeons render [your zone]; multi-select
    ask protocol (echo_index_conflict); phantom Clue option (seat 130 N4); plan-name
    leak into sole-option answer slot (unparsed fallback); stale_livelock candidate-set
    degeneracy (byte-identical copies — give copies distinguishing facts or auto-take).
14. **Carried**: #W42-D2r mana-tap asymmetry (owner call); CS-023 Ludevic's (null this
    corpus — not in pool); Docent of Perfection state-vs-cast trigger; Baka X=0;
    manarestriction blast-radius sweep (85 cards, per-card Oracle, needs alsoabilities
    flavour first); plan-lock general-strategy proposal (seat 130 EDIT 6); PLAN-emission
    economy + no-op churn (INFO).
15. **Watch**: owner narration-fix predictions (validate on the WAVE-43 corpus — this
    one predates them); E2/E7 trample carve-outs (still zero tramplers at a blocker
    window); may-batch turn edge (three corpora 0/0); fizzlelog Vita define (verify
    file appears; else switch to global define).

## Discharged this wave
All seven wave-42 lane prediction sets (one falsification -> #W43-4); wave-41's 24
guide-edit predictions adjudicated across five seats (P6 construction prediction
RETIRED as mis-specified; E5 resolved KEEP; sole-attacker gate deleted from 152 after
third null); rung-3/hallucinated-keyword classes did not recur; serve-transient
hypothesis for wave-41 fallbacks CONFIRMED (0 empty replies).
