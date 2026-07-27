# deck137 -- Selesnya (GW) Adventures -- wave 30 (guided ROUND 8) -- THE g1 FINAL EXIT GATE

Binary 6e168aa05, corpus matchups-20260726-181642, -T 3000, REPPENALTY=1.05.
My 6 pilot logs: 1785*-ai_baka_deck137-0x*-vs-{146,136,122,152,148,139}.jsonl.

Outcomes: **3/6, 0 kills, 6/6 ADJUDICATED at cap, 0 timeouts, 0 draws.**
- WIN (adj) vs152  9/4  t16 -- werewolf/Class; the ONE game a convoke March RESOLVED (t12 X=3).
- WIN (adj) vs148  21/11 t12 -- Kor equipment; Loxodon convoke arrived, Trostani soldiers, board ahead.
- WIN (adj) vs139  16/10 t10 -- Mutants; won on board DESPITE a March convoke silent no-op at t6.
- LOSS(adj) vs146  12/21 t12 -- Orzhov Dungeons; Loxodon+Trostani but ground out.
- LOSS(adj) vs136  10/12 t12 -- mono-B Rankle/drain; VERY close adj loss, Loxodon convoke arrived.
- LOSS(adj) vs122  13/29 t10 -- Eternal Life lifegain; March plain cast cut off by game-end at cap.

Corpus health at my seat: **0 unparsed, 0 empty, 0 retracted_choice, 0 stale_echo, 0 priority
fallback.** Logged fallbacks = **2 defers** (both vs152 t10, March convoke), the corpus's entire
deck137 defer budget. Down from wave-29's 8. BUT SEE section 3: logged defers UNDERCOUNT the true
convoke-resolution-failure rate -- there are ALSO 3 SILENT NO-OPS (invisible to the fallback
metric) on OFFERED convokes.

================================================================================
## (0) HEADLINE -- g1 VERDICT: **GATE NOT MET. The fix is REAL and substantially narrowed the
## residual (8 defers -> 2; first-ever multi-token March at X=3) but OFFERED CONVOKES STILL FAIL
## TO RESOLVE. deck137 DOES NOT ROTATE.**
================================================================================
The gate: "all OFFERED convokes resolve + defers are only the known out-of-scope seam (or zero)."
Neither clause is satisfied:
  - NOT all offered convokes resolved: 3 OFFERED convokes silent-no-op'd (vs122 Loxodon t6;
    vs139 March t6 x2) and 2 deferred (vs152 t10) -- 5 offered-but-unresolved convoke picks.
  - The 2 defers are engine-lane (opts=0, prompt="", empty reply, deferred_to_heuristic) but are
    NOT the exact named cast-mode-menu cascade shape ("plain cast -> mode menu re-offers convoke
    -> flip -> defer"). At vs152 t10 the model picked the CONVOKE VARIANT DIRECTLY (only convoke +
    "cast nothing" were offered; no plain option, no mode menu) and the engine handed the
    downstream X-announce/payment sub-step to the heuristic. Same engine-lane FAMILY, distinct
    manifestation (section 3).
The wave-29 NAMED EXIT PATH (defers -> 0 AND affordable convoke March consistently makes tokens
AND Loxodon silent-noops -> 0) has NONE of its three conditions met. The convoke arc is NOT closed.

================================================================================
## (1) THE COMPLETE CONVOKE RESOLUTION TABLE (every Loxodon + March pick)
================================================================================

MARCH OF THE MULTITUDES ({G}{W}{W}{X} convoke; X = extra soldiers):
  game  | seq(t)      | route picked            | X-afford annot?  | X announced | resolved | outcome
  ------|-------------|-------------------------|------------------|-------------|----------|-------------------
  vs152 | 24 (t10)    | convoke direct          | ABSENT           | no          | NO       | DEFER x2 (s25,s26)
  vs152 | 35-36 (t12) | convoke direct          | PRESENT (up to 3)| **X=3**     | **YES**  | 3 Soldiers -> bf
  vs139 | 20 (t6)     | convoke direct          | ABSENT           | no          | NO       | SILENT NO-OP
  vs139 | 21 (t6)     | convoke direct (re-pick)| ABSENT           | no          | NO       | SILENT NO-OP
  vs122 | 33-34 (t10) | PLAIN {g}{w}{w}{x}      | n/a (plain)      | (mode menu) | CUT OFF  | game ended at cap
  ------|-------------|-------------------------|------------------|-------------|----------|-------------------
  MARCH TOTAL: 5 picks -> 1 RESOLVED (X=3, 3 tokens), 2 defers, 2 silent no-ops, 1 cut off.

VENERATED LOXODON ({4}{W} convoke, fixed cost; +1/+1 counter on each convoker):
  game  | seq(t)  | route            | resolved | outcome
  ------|---------|------------------|----------|-----------------------------------------------
  vs146 | 17 (t6) | convoke direct   | YES      | arrived (later died blocking)
  vs136 | 32 (t10)| convoke direct   | YES      | arrived + "Counter added to Trostani" (convoke pump)
  vs148 | 16 (t6) | convoke direct   | YES      | arrived (blocked Stone Haven, traded)
  vs152 | 17 (t8) | convoke direct   | YES      | arrived (blocked Brutal Cathar, later attacked)
  vs122 | 20 (t6) | convoke direct   | NO       | SILENT NO-OP (option consumed, no arrival)
  vs122 | 27 (t8) | PLAIN {4}{w}     | YES      | arrived via "Cast Card Normally" mode
  ------|---------|------------------|----------|-----------------------------------------------
  LOXODON TOTAL: 6 picks -> 4 convoke arrivals + 1 plain arrival, 1 SILENT NO-OP.

GRAND TALLY: 11 convoke/X-cast picks. Resolved 6 (4 Lox convoke + 1 Lox plain + 1 March X=3).
Unresolved 5 (2 March defers + 2 March silent no-ops + 1 Lox silent no-op). 1 March plain cut off
by cap. So OFFERED-convoke resolution rate ~55%.

================================================================================
## (2) THE END-TO-END RESOLVED CAST (brief-required) -- vs152 seq35-37 t12, LIFE ~15
================================================================================
The X-announce fix works when the affordability annotation fires:
- seq35 t12: `CHOICE: 1 (Cast March of the Multitudes with its convoke cost {x})` -- convoke chosen.
- narration: `Announce the value of X for March of the Multitudes. You can afford X up to 3 with
  your current mana - higher values are NOT offered (they are unaffordable).`  <-- THE ANNOTATION.
- seq36 t12: `CHOICE: 1 (X = 3)` -- **X-announce fires at X=3** (wave-29 only ever reached X=1).
- narration seq37: `March: hand -> stack` ... `Soldier (1/1): created -> battlefield` x3 ...
  `March: stack -> graveyard`. THREE Soldiers.
This is the FIRST multi-token March in four corpora (w27 0/17, w28 0/16, w29 1 token @X=1, w30
3 tokens @X=3). The soldiers then attacked (seq44: `Damage: 1 dealt by Soldier` x3) and vs152 is a
WIN. When the annotation fires, the fix is end-to-end correct and contributes to the win.

================================================================================
## (3) DEFER + SILENT-NO-OP DECOMPOSITION -- the fallback metric UNDERCOUNTS (key finding)
================================================================================
The corpus reports 2 deck137 defers. That is TRUE for LOGGED fallbacks but UNDERSTATES the
convoke-resolution failures, because a failed convoke can end in EITHER of two ways and only one
is logged:

### 3a. THE 2 LOGGED DEFERS (vs152 t10 seq25-26) -- engine-lane, NOT the named cascade.
Shape: choice=-1, opts=0, prompt="" (NO model prompt built), reply="", latency=-1,
deferred_to_heuristic, chosen_text "March of the Multitudes". At seq24 the model was offered ONLY
`[Cast March...convoke cost {x}, Cast nothing]` (no plain option, no "Cast Card Normally" mode
menu), picked convoke directly, and correctly PLANNED X=3 (6 creatures, base 3, X=3) in its reply.
The engine then handed the downstream X-announce/payment sub-steps to the heuristic with zero
options and no prompt -- TWICE. The heuristic declined; no token; March returned to hand and was
successfully recast at t12 (section 2). So this is NOT the wave-28/29 "plain-cast -> mode-menu ->
convoke -> flip -> defer" cascade (there was no plain pick and no mode menu). It is a DISTINCT
manifestation of the same engine-lane root: a directly-picked convoke cast whose choreography
sub-step routes to the heuristic when the X-affordability annotation is ABSENT.

### 3b. THE 3 SILENT NO-OPS (invisible to the defer counter) -- same root, unlogged.
- vs122 Loxodon t6 (seq20): 3 untapped mana sources + 4 convokable creatures (>= the {4}{W}=5
  needed). Model picks Loxodon convoke; option is consumed; NO arrival, NO defer logged; Loxodon
  stays in hand and is recast PLAIN at t8 (seq27 -> arrives). A silently-declined AFFORDABLE
  convoke -- fixed-cost face of the same defect.
- vs139 March t6 (seq20, seq21): 4 mana sources + 2 convokable creatures. NO X-affordability
  annotation on the option line. Model picks convoke; the 4 lands auto-tap into the pool
  ({g}{w}{w}{w}) but the cast never completes; picks convoke AGAIN with the floated pool; still
  no-op. NO token, NO defer, NO X-announce. March sits in hand for the rest of the game.

### 3c. THE PREDICTOR IS RE-CONFIRMED and now GOVERNS BOTH failure modes.
The `You can afford X up to N` annotation is again a PERFECT resolution predictor: PRESENT ->
March resolves via X-announce (vs152 t12 only); ABSENT -> March fails to resolve (vs152 t10 defer;
vs139 t6 silent no-op). The engine's OFFER of the convoke variant is NOT gated on the same
payability the annotation/resolution require -- the convoke option is offered (opt present) even
when the engine will not let convoke reach X>=1, so the model picks a doomed option. This is the
wave-29 root UNCHANGED; the wave-30 fix added the clean-resolution path (annotation present) and
cut the churn, but did not close the offer-vs-resolution payability gap.

================================================================================
## (4) UNIFIED ENGINE CHARACTERIZATION (defer + silent-no-op = one root, two failure modes)
================================================================================
One defect: **the engine offers a convoke cast whose convoke payment the engine will not actually
complete (annotation-absent case).** It fails in one of two ways, and ONLY the first is logged:
  - X-spell March convoke, annotation absent -> DEFER (logged) OR SILENT NO-OP (unlogged),
    apparently state-dependent (vs152 t10 deferred; vs139 t6 silent no-op'd; same shape otherwise).
  - fixed-cost Loxodon convoke, unresolvable -> SILENT NO-OP (unlogged) (vs122 t6).
The wave-30 payability fix made the ANNOTATED case resolve cleanly and multi-token (real progress),
but the unannotated case still gets offered and still fails. Routed to notes.md
(N-137-MARCH-XCONVOKE and N-137-LOXCONVOKE-SILENT-NOOP updated; new sub-finding: the fallback
metric undercounts -- silent no-ops require ARRIVAL-tracing, not fallback-counting, to detect).

================================================================================
## (5) BLOCKERS FLOOR ROUND 6: HELD. 11 records, all legal, zero fallback, zero gang-pile.
================================================================================
11 blockers records (vs146 x1, vs136 x2, vs122 x2, vs152 x4, vs148 x2, vs139 x0). ALL fb=None,
all parsed. ZERO illegal one-blocker-onto-multiple-attackers piles:
- Multi-block records are all distinct-blocker-per-attacker or legal gang-blocks:
  vs146 s28 `Soldier blocks Goblin; Soldier blocks Nadaar` (2 soldiers, 2 attackers);
  vs152 s30 `Flaxen blocks Intrepid; Giant Killer blocks Wolf; Giant Killer blocks Luminarch`
  (B1:A3,B2:A2,B3:A1 -- 3 distinct blockers, 3 attackers);
  vs148 s21 `Trostani blocks Kor Duelist; Soldier blocks Kor ally; Soldier blocks Kor ally`
  (distinct blockers).
- Loxodon convoke bodies block-and-kill repeatedly (vs122 Kitchen Finks, vs148 Stone Haven,
  vs152 Brutal Cathar, vs136 Midnight Reaper) -- the convoke-body-as-blocker line is clean.
Round-6 floor: HELD, clean.

================================================================================
## (6) CLOSING-SPEED FINAL WORD
================================================================================
deck137 went 3 kills (wave-29) -> **0 kills, 6/6 adjudicated** (wave-30). This is NOT a deck137
regression -- it is the POOL: the whole corpus was the grindiest yet (brief: 12 adj at cap
pool-wide). deck137's own wins were all life-AHEAD adjudications (vs152 9/4, vs148 21/11,
vs139 16/10) and its losses close-to-blowout adjudications (vs136 10/12 razor-close; vs146 12/21;
vs122 13/29). The single RESOLVED convoke March (vs152 X=3) landed in a WIN and its 3 soldiers
dealt the closing damage -- direct evidence that a working March payoff helps. The residual
convoke failures still ROB the go-wide payoff: vs139 won anyway but left a March stranded in hand
all game; vs122's March was cut off by the cap.

FINAL WORD: closing speed is ENGINE-BOUND on convoke, now PARTIALLY -- the annotated path
resolves and contributes; the unannotated path (silent no-op / defer) forfeits the deck's biggest
go-wide swing. The all-adjudication result is pool-driven, not a deck137 closing-speed regression.
The re-pick CHURN, however, is markedly TAMED: wave-29's vs148 4-7x March re-pick loop is gone;
worst case this wave is 2 consecutive picks (vs139 t6), then the model moves on -- the guide's
one-attempt-per-card clause plus the fix are working together.

================================================================================
## (7) g3 / g4 / parser
================================================================================
- g3 (verb-labeled loot/discard asks): N/A at this seat. deck137 (GW go-wide) has NO loot/rummage/
  discard effect; zero discard/loot asks fired across all 6 logs. The vs152 Clue ("Draw 1 with
  Clue, Sacrifice") is a draw, not a loot. Nothing to cite.
- g4 (tapped-tag reword): `[tapped - cannot attack or block this turn]` rendered 131x across
  deck137 prompts. Read CORRECTLY every time -- vs152 s24 the model cited "Luminarch Aspirant is
  tapped. Wolf is tapped" and swung into the empty board. `[summoning sick - cannot attack this
  turn]` also rendered clean for own creatures. Zero misread; the wave-29 residual misread class
  is dead at this seat. Steady state.
- Parser contradictions: NONE verbatim. One benign self-correction in vs152 s24 reasoning
  ("triggers Emmara (wait, Emmara is dead)") -- the model caught its own error mid-reply; not a
  parser/render contradiction.

================================================================================
## (8) ROTATION VERDICT: **DOES NOT ROTATE (with finality-conditions re-stated).**
================================================================================
See notes.md for the full rotation reasoning. Summary: the g1 gate is NOT met (offered convokes
still fail to resolve via 2 defers + 3 silent no-ops; the offer-vs-resolution payability gap is
UNCHANGED from wave-29; the fallback metric was shown to UNDERCOUNT the failure). deck137 remains
the SOLE seat exercising convoke and the only observation post for this HIGH engine item.
Rotating now would (a) blind the loop to whether the remaining fix lands, and (b) rotate on a
FALSE premise (the orchestrator brief assumed "2 defers" was the whole residual; it is actually
2 defers + 3 unlogged silent no-ops). KEEP.

repetition_penalty=1.05 -- eighth-corpus verdict: SAME. Coherent across all replies, no prose
harm, no spiral. Unchanged.
