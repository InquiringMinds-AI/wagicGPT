# Wave-29 synthesis notes (FIFTEENTH per-deck-rotation cycle)

Corpus `matchups-20260726-115018`, binary `50eeed8be`, -T 3000, REPPENALTY=1.05 (7th corpus). 21 games,
0 timeouts / 0 draws, 7 cap-adjudications (28 gameend logs = 42-14, integrity clean). 1,316 decisions,
11 fallbacks (0.8% -- the cleanest fallback profile of the campaign). Retry 0.

## Verdict table

| Seat | Archetype | Rec | Guide action | New work | Rotation |
|------|-----------|-----|--------------|----------|----------|
| 93 | Relentless Rats mono-B aggro | 5/6 | BYTE-FROZEN (cmp 5124 b) | S-93a ESTABLISHED; N-93c tapped-tag wording (LOW, cross-deck) | **OUT (THIRD complete pipeline arc; f3 exit gate PASSED)** |
| 137 | GW convoke + Adventures | 3/6 | REVISED (X>=2-3 affordability threshold) | N-137-MARCH PARTIALLY-CLOSED (HIGH); unified w/ Loxodon noop | STAY (sole convoke post) |
| 136 | Yarok-of-Pranks BW walker-midrange | 5/6 | REVISED (Mausoleum Secrets gate) | N-136a illegal-tutor seam (MED-HIGH, 3/6 games) | STAY (Rats matchup unwon pending N-136a) |
| 146 | Orzhov Dungeons WB | 3/6 | REVISED (fact-drift corrections) | N-146f dungeons-completed line (LOW); Kaya/Lolth unexercised | STAY (walkers unproven 2 corpora) |
| 139 | Mutants Temur/Sultai mutate (Step-0) | 2/6 | NEW initial guide | N-139a/b/c/d mutate render batch; N-139g under-card statics probe | STAY (Step-0 debut) |
| 148 | Kor Army mono-W equipment (Step-0) | 1/6 | NEW initial guide | CONSTRUCTION-CEILINGED; N-18e first live fire validated | STAY (Step-0 debut) |
| 122 | Eternal Life WB lifegain/alt-win (Step-0) | 2/6 | NEW initial guide | N-122a Felidar alt-win probe (engine-confirmed, live-unvalidated) | STAY (Step-0 debut) |

Core (general) prompt: **PASS x24** (7/7 seats). wave-26=21st, wave-27=22nd, wave-28=23rd, wave-29=24th.

## The corpus story

**A five-fix f-batch swept for validation; the pipeline recorded its THIRD clean EXIT; the loop's own
instruments earned their keep repeatedly.**

**The f-batch split-decides -- f2/f3/f4/f6 validated decisively, f1 partial (unified root), f5 steady.**
- **f2 dual-land oracle -> DECISIVE.** The no-cast-window class is GONE: deck146 removal on curve 5/6, vs93
  flipped a wave-28 clean-kill LOSS to a WIN, 0 mana-collapse lines across 6 games, instant windows restored
  (35 in vs139, 17 in vs122; 0 in the wave-28 losses).
- **f3 summoning-sick tag -> DECISIVE.** deck93 phantom-attacker over-list 11/15 -> 0/21 (sick class), 18/21
  replies cite the tag; the 1 residual is a DIFFERENT class (tapped-tag wording, N-93c). Guide byte-frozen
  across the transition = the annotation is the isolated cause (completed controlled experiment).
- **f4 dungeon renders -> DECISIVE.** Lost Mine 7/7, option-1 bias broken, value-aware room picks, and
  **DUNGEON COMPLETION reached live** -- the wave-28 N-146e 309.5b probe question is ANSWERED (anthem fires,
  no crash) -> probe RETIRED.
- **f6 N-18e truncation safety -> FIRST LIVE FIRE, VALIDATED.** deck148 vs139 seq31: correctly abandoned a
  self-contradicted truncated block commitment, took the safe no-blocks default at 4 life.
- **f1 March X-convoke -> PARTIAL.** FIRST-EVER March resolution in campaign history (deck137 vs139: X=1
  announced, paid, 1 Soldier, end-to-end trace vs 0/33 prior) but only 1/7 episodes; all 8 residual defers
  are convoke-UNAFFORDABLE, UNIFIED with the Loxodon silent-noop under one root: **the engine offers convoke
  casts whose payment cannot complete.**
- **f5 Flip-Side -> steady** (no DFCs stressed in the main pool this corpus).

**The 309.5b live answer.** Dungeon completion was reached and the completed-dungeon anthem applied to the whole
team (Nadaar 5/5, Adventurer 2/2), game ran to a win, no crash -- resolving the wave-28 probe NEGATIVE for
crash, POSITIVE for the mechanic. The random pool now exercises it; the probe deck is unneeded.

**Three Step-0 reads, verify-oracle rail earning its keep TWICE.** deck139: the rail caught a memory claim that
Migratory Greathorn ramps "on enter OR mutate" (Oracle + primitive say MUTATE-ONLY) -- asserting it would have
falsely flagged the engine AND mistaught the pilot. deck148: the rail pulled Armament Master's engine-DOUBLED
+2/+2-per-equipment buff AND its no-gear=0 bare-lord-is-a-blank condition at draft time (both load-bearing).
deck122: the render was confirmed to already show conditional-static `(printed X/Y)` P/T live, routing the
threshold-race loss to GUIDE prose not an annotation.

**The MV3-fix arc closes but reveals a new seam bug.** deck136's wave-28 Relentless-Rats-MV3 fix is FULLY
validated at the decision layer (17 consecutive correct Legion's-End rejections, Ritual valued as THE answer) --
the verify-oracle fabrication incident (wave-27 fabricated MV -> wave-28 fix -> wave-29 validated) is CLOSED.
Yet the targeted matchup still LOST because a SEPARATE seam (N-136a) silently ate the fetched Ritual: the
record and the fix are on different layers.

## RANKED WAVE-30 LEDGER

Top 5 (work these between waves):
1. **Convoke payability-before-offer in `buildCastSpell` [HIGH, engine].** Closes N-137-MARCH-XCONVOKE (partial)
   AND the Loxodon silent-noop under one root -- the last convoke leg. The engine offers convoke casts whose
   payment cannot complete; compute convoke-payability BEFORE offering (the legality-oracle invariant), and stop
   the plain-route "Cast Card Normally -> convoke" detour from re-entering an unpayable path after mana is spent.
   Exit gate: defers -> 0 + affordable March consistently makes tokens + noops -> 0. deck137 is the SOLE
   observation post. Repros: vs148 seq10/34/39 (unaffordable convoke), vs93 seq48-51 (plain-route cascade),
   vs148 seq27 (Loxodon noop).
2. **N-136a reveal/PUT tutor-seam predicate filter [MED-HIGH, engine].** The reveal seam builds PUT options from
   the whole revealed library and does NOT apply the ability's option-level target predicate, so the model is
   offered illegal picks; it takes its best on-color bomb, the engine's filter rejects it, the tutor silently
   no-ops. Recurs 3/6 games; cost the Rats matchup a 3rd straight time. Fix: filter PUT options by the predicate
   (same `WParsedInt`/target-legality machinery); if empty, don't ask (auto-take shuffle). Repros: vs93 seq10-11
   (opts=43), vs139 seq16, vs148 seq22.
3. **N-139b loot/discard verb-label + N-139a/c/d mutate render batch [MED, representation].** N-139b: label a
   remove-from-hand cost (draw-then-discard/sac/exile) with the VERB, not "target it will affect" (perception
   leak; misread deck93 s20 vs decoded deck122 s18). N-139c: group the mutate pile / tag `[under X]` / render the
   merged keyword+ability union. N-139a: reorder the multi-ask (target before over/under). N-139d: unify the
   mutate-cost label (mutate cost / alternative cost).
4. **N-93c tapped-tag reword + annotation-wording audit [LOW, cross-deck, engine/render].** Reword
   `[tapped - untaps and can attack next turn]` -> `[tapped - cannot attack this turn]` (restriction-first, drop
   the action-affirmative substring). Then grep all existing state tags for action-affirmative substrings and
   reword them the same way (S-93b principle). Repro: deck93 vs137 seq18.
5. **N-146f "dungeons completed: N" status line [LOW, engine/render].** The model can only infer completion from
   anthem'd P/T and audibly second-guesses the anthem (deck122 s37 PLAN). A one-line status removes the ambiguity.

Then: **N-122a Felidar alt-win live probe** (engine-confirmed at mtg.txt:39654 / AAWinGame; never live-validated
-- probe deck stacking Felidar + fast redundant lifegain, confirm the 40+ upkeep wingame fires); **N-139g
under-card static abilities on a merged creature** (engine-correctness probe -- CR 725 grants all abilities;
unobserved this corpus); **modal->priority context bleed** (deck136 Rankle-mode answer leaked into a following
priority window, seq39 -- light WATCH, terminal-marker mitigation if a 2nd seat shows it); **L-18-1 planning-tax
migration** (MODEL/CORE, now 2 cross-archetype witnesses -- promote on cross-FAMILY agreement, not this wave);
mutate/combat latency-tail WATCH (195-199k ms, deck139; soft reasoning-length ceiling candidate if cross-family);
then carries: N-134b flashback cost display, N-134d Mulldrifter double-ask, L2 cross-zone dup-name, ETB header
card-name, N8, Azcanta, undying, commit-lock, prose-reversal, untapped-tagging, I-4.

## CLOSE list (discharged this wave -- do NOT re-open)

- **N-93a summoning-sick tag** -- FIX VALIDATED; 11/15 -> 0/21, 18/21 cite the tag. CLOSED.
- **N-93b mulligan double-render** -- de-dup applied, verified 6 games. CLOSED.
- **S-93a candidate** -- PROMOTED to ESTABLISHED heuristic (completed controlled experiment). No longer a
  candidate.
- **N-146e dungeon-completion 309.5b probe** -- ANSWERED live (anthem fires, no crash); probe RETIRED, probe
  deck no longer needed.
- **verify-oracle-text MV-fabrication incident (deck136)** -- validated at the decision layer; incident arc
  wave-27->28->29 COMPLETE. Retire from active watch; keep as the canonical worked example.
- **LILIANA -4 / RANKLE combat-damage may-ask (deck136)** -- both RESOLVED live this wave. Retire from
  carry-forward (Rankle superseded by the new modal->priority bleed light-watch).
- **R-* (phantom-chooser, annotation-echo-parse, flip-thrash livelock, land-blindness, Loxodon fixed-cost
  convoke half)** -- remain closed from wave-28; not re-opened.

## Cross-seat contradictions / reconciliations called out plainly

- **Core-PASS counting: NO drift this wave.** All 7 seats wrote "24th consecutive." Reconciled cleanly against
  the wave-26=21st / wave-27=22nd / wave-28=23rd lineage. (Contrast wave-28, where three seats under-counted.)
- **The Loxodon "silent no-op" and the March "defer" are the SAME defect, not two.** deck137's notes unify them:
  fixed-cost convoke fails silently, X-spell convoke logs a defer, but both are the engine offering a convoke
  cast whose payment can't complete. The wave-28 ledger listed them separately; wave-29 merges them under one
  root (ledger #1). No contradiction -- a unification.
- **"First-corpus record delta is NOT a guide verdict" holds again (deck136).** deck136 rose 2/6 -> 5/6 on the
  MV3-fixed guide, but the seat itself attributes the rise to pool shift + variance, NOT the fix (the fix's
  targeted matchup was LOST). Symmetric to wave-28's caution about deck136's 4/6->2/6 drop -- read the DECISION
  layer, not the record, in both directions.
- **f1 is PARTIAL, not "fixed" -- and the seat resisted the naive read.** A naive read (March made a token,
  wins held 3/6) would call f1 fixed; tracing every March episode (both convoke AND plain X-cast variants)
  showed 1/7 resolved and 8 defers still convoke-class. The resolution-verification rung + trace-ALL-variants
  refinement earned their keep on a post-fix binary. No contradiction with the "FIRST-EVER resolution" headline
  -- both are true: the fix works when convoke is affordable, and the residual is the affordability gap.
- **Layer-routing held under three temptations** (N-137 March death, N-136a illegal-tutor seam losing the
  flagship matchup, deck148's 1/6 loss column): none became guide/skill/core prose -- all engine/representation/
  construction items. A model offered an illegal tutor option cannot be taught out of it by a guide (only the
  guide's DEMAND-reduction half is legitimate); a convoke payment the engine can't complete is not a prompt fix.
