# RULINGS — draft for the owner to correct (sessions never edit the final; they propose in a separate file)

Each line: the ruling in the owner's words where the record has them; the SURFACE it governs; the CHECK that enforces it
("ask" = no check possible — the brief must name it and a lane may report on the surface, never change it).

1. REASONING: "reasoning could not be done in the output, and ... a plan had to precede the action." Reasoning lives ONLY
   in the model's native reasoning channel; the reply is `PLAN:` then the action line, nothing else. "the model CANNOT
   be told to writing nonsense OR IT FUCKING WILL WRITE NONSENSE." — SURFACE: reply protocol, every seam tail/re-ask,
   every reply parser. CHECK: golden `reply-protocol.txt` pinned in PARSETEST; gate grep rejects prose-licensing phrases.
2. PLAN = "a scratchpad so it can execute sequenced decisions" ("swing all, use combat trick n after blockers are
   declared, then play creature in second main"); a sequence carried across windows, never reasoning. — SURFACE: plan
   split/carry/caps. CHECK: PARSETEST two-line pins; plan carry bounded by steps, not bytes.
3. CORPORA RUN WITH REASONING ON ("fuckin obviously we need reasoning"); the product regime is thinking OFF; the reply
   shape is identical in both. — SURFACE: harness, launch scripts, config. CHECK: `--thinking on|off` REQUIRED; per-regime
   gate kills a wrong-regime corpus; regime stamped on every record.
4. A corpus completes full games or the test has failed; games run uncapped ("no cap"); stop a run whose numbers show
   games cannot finish. — SURFACE: harness, watch. CHECK: harness -T 0 default; feasibility watchdog.
5. NO VPK / EBOOT / APK unless asked in that moment ("don't make vpk I don't ask for"). — SURFACE: build steps. CHECK: ask.
6. NO AI attribution in commits/PRs on this repo. — SURFACE: git. CHECK: a commit-msg grep in the merge script.
7. WATCH CADENCE: terminal events + ONE early check + ONE late check at the predicted maximum; never periodic pings.
   — SURFACE: monitors. CHECK: ask (the monitor template encodes it).
8. GUIDES hold decision-time instruction ONLY — no corpus/wave/seq citations, no "previous games", no history; the band
   is retired; hard ceiling 20,000 B. — SURFACE: deck<N>_strategy.txt, the strategy-writing skill. CHECK: Amendment-332
   grep + size check before install.
9. STRATEGY-WRITING SKILL = `wave68/strategy-writing-skill-v2.md` (owner-installed); Astra/Fable reviews it once per wave,
   never per revision. — SURFACE: reviewer briefs. CHECK: the brief's skill pointer.
10. MODELS: Codex reviews run on gpt-6-astra; when the Codex pool is exhausted the adversarial fallback is FABLE; Fable
    also does instruction-surface distillation; "use what needs to be used to do the task right" — never shift lane/seat
    volume to Codex for cost. — SURFACE: agent launches. CHECK: ask.
11. PROMPT REVIEW is alert-only ("i havent read a lategame prompt in tens of waves"); specimens are never owner-pending.
    — SURFACE: step-three briefs, close-outs. CHECK: none needed.
12. Card bugs: retrieve the REAL Oracle text before trusting a primitive; fix in script (data) before touching the engine.
    — SURFACE: primitives. CHECK: ask (the /wagic-cardscript skill).
13. Vita play reports are BAKA-seat games ("on the vita, ive been playing only against the heuristic ai").
    — SURFACE: bug triage. CHECK: none.
14. The loop is: fix known bugs -> corpus -> evaluate -> repeat; REJECT is the expected synthesis verdict; nothing else in
    the loop's operation is his ("everything else ... is an invention of claude"). — SURFACE: the wave shape. CHECK: ask.

Rulings I could not find in his words and have NOT included (he may add): the reply-protocol history before wave 21;
anything about the guides' band origin; the D42 stacking ruling is in wave58/owner-decisions.md and stands.
