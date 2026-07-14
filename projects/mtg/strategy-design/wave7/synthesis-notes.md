# Wave-7 synthesis notes

Corpus: `matchups-20260713-222831` (one round-robin, 6 games/deck), seven seats reviewed
(44, 109, 110, 131, 133, 135, 140). Mode across the pool: mostly OBEYED-BUT-LOSING / turnaround —
the wave's value was NOT in guide rewrites but in the REPRESENTATION rung catching two defects that
prior waves' framing had walked past (the land-drop menu, the false-belief-bridge on sweepers).

## Win table (context, not the signal — n=6 is noise; per-seam obedience is the signal)
44: 5/6 (top, frozen spine) · 109: 4/6 (fell 5→4, variance) · 110: 4/6 (REBOUND from raw 1/6) ·
133: 4/6 (best ever; beat 44) · 131: 1/6 (OBEYED-BUT-LOSING, 3rd wave) · 135: 1/6 (OBEYED-BUT-LOSING)
· 140: 0/4 + 2 TO (collapse — a real model-fixable overshoot, the wave's headline guide fix).

## The headline: a shipped representation fix is a HYPOTHESIS until you see it FIRE — and wave 7 CLOSED both open loops

- **The re-equip marker: SHIPPED → FIRED → OBEYED → licenses DEMOTION.** Wave-5 designed it, wave-6
  found it shipped-but-INERT (wrong field), wave-7 the field fix landed (`AIPlayerGPT.cpp:1163-1173`,
  equipment host = `moved->target`). Marker validation corpus-wide: **75 offered / 1 taken**; deck110
  seat **29 offered / 28 obeyed, no-op rate 50% → 6%.** The single "taken" (deck110 vs131 s33) is a
  PLAN/CHOICE MISMATCH (pilot reasoned for a legit different equip, typed the wrong index), NOT the
  marker ignored. This is the first end-to-end close of the shipped-but-inert trichotomy → the
  deck110 seat demoted its 19-line perception-procedure+count-brake to 10 lines. **New terminus
  adopted into the skill:** a CONFIRMED-fired-and-obeyed representation fix licenses demoting the
  guide stopgap it replaced (the stopgap lifecycle applied to the representation-vs-guide seam).

- **The false-belief BRIDGE: a widened HOLD over-fires INDIRECTLY.** deck140's wave-6 widened hold
  ("hold when opponent has NO creatures") collapsed the deck 0/4+2TO — but not by direct
  over-application. The executor reached the hold on boards WITH creatures through three pre-existing
  false beliefs, each quoted from its own PLAN and contradicted by the same board: "they're TAPPED"
  → "no (untapped) creatures"; "mostly ARTIFACTS" → "sweeper useless"; "SHROUD" → "can't remove."
  **New method adopted:** after widening any permissive/hold/brake condition, enumerate the false
  beliefs the executor already holds and either kill each bridge inline as a flat falsity-fact OR
  re-anchor the condition to a HARD board-readable COUNT the belief cannot reinterpret. This is
  representation-beats-instruction at the CONDITION level.

## Convergences this wave (what met a bar)

- **[2 waves, multi-card — ENGINE, prediction CLOSED] no-legal-target menu MIGRATES across spells.**
  deck44's wave-6 GFTT stopgap held (0 drops) and the deck-agnostic defect walked to **Liliana of
  the Veil (4×) + Fatal Push (2×)** in opponents' seats — exactly as wave-6 predicted. Per-spell
  stopgaps can NEVER be the general fix; the shared menu path needs a target-existence probe. Ledger #2.
- **[2 waves, deck-agnostic — cross-SEAT method] a per-deck stopgap for a deck-agnostic engine defect
  must be class-checked across SEATS.** deck44 P2: grep ALL stderr (not just the reviewer's deck) for
  the defect string to confirm the class MIGRATED vs resolved — the composition-seam rule applied
  across seats, not just files. Adopted into the stopgap lifecycle.
- **[cross-cutting — harness sweep] resolved-vs-asserted narration check.** deck133 P1: the PLAN
  asserts an effect (Gray Merchant "opp to 3, I gain 8") the narration contradicts (`Gray Merchant:
  stack -> GRAVEYARD` after an opponent `Counterspell: hand -> stack` = COUNTERED). A reviewer
  trusting `chosen_text` + plan prose mis-attributes the WIN mechanism. The resolution-stage twin of
  the cast-happened anchor. Adopted into the Step-2 sweep.
- **[3 guises, 5 seats — still REPRESENTATION, not core] reflexive/mis-framed blocking.** Wave-6's
  one 2-seat convergence gained a third guise (deck131: block with the ONLY reach creature at L14,
  framed as survival; deck44: block the Obliterator punisher at L11). Both witnessing seats + the
  skill agree: the core floor exists and the pilot disobeys at distance → surface life-vs-lethal AND
  the punisher rider at the blocker seam (ledger #5). NOT a core sentence.

## Core status: PASS (see core-prompt-revision.md)

No prompt-body edit. ONE scheduled change: the `hand -> stack` cast-happened anchor A/B is **GO for
wave-8** — its condition (two consecutive clean no-op corpora) is MET (0 cast no-ops across all 7
seats, 2 waves; defect fixed at source 353f1a150). It stays in the shipped prompt until the A/B runs
(an A/B needs a control arm); the exact removal old→new is in core-prompt-revision.md §1.

## RANKED ENGINE LEDGER (rank = leverage × breadth × strength-of-evidence)

1. **LAND-DROP SEAM — enumerate distinct playable lands as options.** deck131 headline, STRONGEST
   evidence: the engine offers lands ONE AT A TIME as "Play <land> / Hold <land>" and NEVER co-offers
   two — verified 0/6 games had both "Play Mountain" and "Play Island" in one option list. So
   "Mountain-first" / "play your key color first" is UNEXECUTABLE for every color-hungry deck: when
   Islands surface first (hand order), the red source sits dead in hand for turns (vs44: Mountain in
   hand T1, first offered T7). Holding is NOT a workaround (held an Island MP1, re-offered an Island
   MP2, never the Mountain). Fix: present all in-hand lands as simultaneous Play options (Play
   Mountain / Play Island / Hold), or at minimum annotate the current land offer with the other
   playable lands in hand. Highest leverage in the pool; helps every 2-color deck. When it ships, DROP
   the interim "hold-to-reach-your-color" stopgap from every 2-color guide (tracked removal condition).

2. **NO-LEGAL-TARGET MENU — target-existence probe on the shared menu path.** Two-wave, multi-card,
   mechanically-guaranteed contract violation (the prompt promises "the game has already checked your
   … targets"). Migrated GFTT → Liliana(4×)/Fatal Push(2×). At option-build time, for a spell/ability
   whose only legal mode needs a target, suppress or mark `[no legal target]` when the legal target set
   is empty, and make selecting it a parser-rejected no-op, not a silent heuristic deferral.

3. **"creatures: N" COUNT on the opponent-battlefield line** (+ tag a tapped attacker
   "(untaps and attacks next turn)"). deck140. Deck-agnostic representation that DISSOLVES the whole
   false-belief-bridge class (tapped/artifact/shroud misreads) — the pilot's entire wave-7 collapse is
   mis-reading how many creatures sit on a cluttered line studded with artifacts and [tapped] flags. A
   surfaced integer is the representation the per-deck false-belief kills are standing in for; when it
   ships those kills become deletable. Also obviates the two mechanics-fact core watches.

4. **SUPPRESS the RE-OFFER of a COMPLETED repeatable activation** (held-fetch crack; already-attached
   equipment across all copies). deck135 + deck110. The held-fetch re-offer TAX is **44–97 windows per
   game** (vs140's 97 fed its 144-decision timeout) — a re-offered repeatable activation has TWO cost
   surfaces: the waste-loop (re-taking it) AND the attention/latency tax (a model call to decline it
   every priority window). deck110 P3 adds the multi-copy blind spot: a per-instance "already attached"
   marker can't catch churn among N copies (shuffle Plating-A/B between carriers) — suppress the
   re-offer once every copy is attached to SOME carrier. This directly reduces control-deck decision
   counts → cuts the timeout/latency exposure in item (harness) below.

5. **BLOCKER-SEAM life-vs-lethal + punisher-rider surfacing.** Carried from wave-6, now the routing
   target for the 3-guise reflexive-block convergence. Annotate the `blockers` prompt with
   `Your life: N (unblocked this turn: D -> you'd be at N-D)` / a `[not lethal]` marker, and stamp a
   punisher blocker with its rider ("blocking this makes YOU sacrifice/take X"). Same shape as the ON
   THE STACK win. 2-seat-plus, but representation not core.

6. **GRAY MERCHANT drain display shows the RESOLVED total** (include GM's own {B}{B}=2 devotion; the
   option is built before GM is on the battlefield so it under-counts by 2 — confirmed 4/4 across
   waves). deck133. Narrow (one card, reached the list in 2/6 games) → ship as an A/B. The point (P2):
   the pilot this wave IGNORED the guide's "+2" workaround and hand-recounted, erring — a surfaced
   value beats a formula, so the workaround becomes deletable when the display shows the final number.
   Scope check: add the caster's own contribution ONLY where the magnitude is `mybattlefield`-scoped
   devotion the source itself joins (verify vs `auto=lifeleech: -type:manab:mybattlefield`), or it
   double-counts on other lifeleech cards.

7. **EVIDENCE-TOOLING (reviewer-facing — removes ~7× duplicated toil per wave).**
   (a) Timeout games write NO `gameend` record — add a timeout-adjudication line (final life + turn +
   who was ahead) so control-mirror timeouts don't need manual reconstruction (open since wave 4).
   (b) `resolved: battlefield|countered|fizzled|graveyard` (or an `effect_events` echo) on the next
   record, so a consumed cast's OUTCOME is machine-readable and win-MECHANISM mis-attribution (deck133
   §2) is prevented. (c) `deferred_to_heuristic: true` when a validation-failed pick is dropped to the
   heuristic (deck44), so reviewers don't cross-reference stderr. (d) a game-id/opponent/result header
   in each seat JSONL, ending the manual epoch-arithmetic result pairing (deck131).

## HARNESS KNOBS (concurrency / latency)

- `-j2` fixed the aggregate latency this run: **2/21 timeouts, corpus median ~7.5s** — healthy.
- **BUT the tail is heavy on HIGH-DECISION-DENSITY control mirrors, and it reconciles with the healthy
  median rather than contradicting it.** deck140's vs131 spiked to **111.9s**, vs110/vs44 ~47s, and
  vs135 produced **19 empty-reply fallbacks / 34 decisions at 32.5s median.** A control deck makes
  3–4× the decisions of an aggro deck, so identical per-decision latency gives it disproportionate
  wall-clock + concurrency exposure; under `-j2` the long-context cluttered-board prompts on these
  mirrors saturate and fall back to empty replies. Both deck140 and deck135 reviewers correctly
  triaged these as latency-starved / infra, NOT play defects (deck135 vs140 read as a slow-clock
  control stalemate; deck140 vs131 as ahead-but-slow). So the corpus median is real AND the deck140
  vs135 game's spikes are real — they live in different archetype/decision-density regimes.
- **Knobs for the wave-8 review run:** use `-j3` (or higher `WAGIC_GPT_TIMEOUT` well above the
  fallback floor) for any run whose decision QUALITY matters — otherwise a control deck's "collapse"
  is partly manufactured by empty replies. Log the **fallback-count and latency-median columns** in the
  digest so latency-starvation is separated from play before scoring. Engine ledger item 4 (suppress
  re-offer) attacks the same problem structurally by cutting control-deck decision counts.

## WAVE-8 INHERITANCE

- **Run the `hand -> stack` anchor A/B removal** (condition met; core-prompt-revision.md §1 has the
  exact old→new + measurement). Not a wave edit — a deliberate A/B with a control arm.
- **Verify the land-drop-seam fix FIRED** if it ships (grep for co-offered lands), THEN drop the
  interim hold-to-reach-your-color stopgap from every 2-color guide.
- **Verify the re-equip demotion held** (deck110): the marker should still fire ~28/29; if the demoted
  guide loses the no-op floor, re-inflate.
- **Convergence WATCHES carried (need a 2nd seat to promote):** phantom-lethal-fixation → decline-a-
  listed-body (deck109, 1 seat); mechanics facts tapped-still-threat + shroud-vs-non-targeted (deck140,
  1 seat) — prefer ledger #3 over core prose; free-reaction-card-cost (deck135, need a 2nd free-alt-cost
  deck); multi-copy equip churn (deck110, need a 2nd multiples deck); plan/choice-mismatch on attractive
  labeled counter options (deck44, low-frequency residual of a GOOD representation fix).
- **Deck-construction flags STAND (user owns decklists):** deck131 manabase 14I/8M + 6/60 threat
  density (cut 4 Prism Ring; consider 11M/11I; maindeck Spellheart Chimera) — but part of the "red
  screw" is the land-menu INTERFACE defect (source in hand, unofferable), so weight the manabase change
  against the interface fix landing; deck140 reach starvation + no early defense vs aggro + Akoum-tapped
  red count; deck131 & deck140 both cannot beat a lifegain opponent without a non-incremental finisher.
- **Untouchable-board / evasion matchup class** (deck109): opponent's whole threat base is unblockable
  AND untargetable by your deck — reads like a play problem, is a deck-construction/meta fact ("race
  harder" is already the plan). Routed to deck construction; the evasion cousin of the Obliterator
  named-matchup note.
