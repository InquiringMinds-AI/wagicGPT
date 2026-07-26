# Wave-27 synthesis notes -- THIRTEENTH per-deck-rotation cycle

Unified from seven independent deck-agent reviews mining the wave-27 PRIMARY corpus
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260725-193608` (binary `ffec79fd8` = the d-batch step-1 under live
validation; -T 3000, REPPENALTY=1.05, 5th corpus). Pool = 7: 131 (UR Guttersnipe burn) / 137 (GW convoke+Adventures)
/ 18 (Kithkin mono-W anthem-aggro, FIRST-GUIDED) / 22 ("Giants!" mono-R tribal ramp, FIRST-GUIDED) + the three
GUIDELESS Step-0 debuts 134 (mono-U/G Urza tron control) / 93 (Relentless Rats) / 136 (Yarok-of-Pranks BW midrange).
Cross-wave record comparison is context-only -- judge decisions, not the win column.

## Verdict table

| deck | archetype | record | seat verdict | guide action | rotation |
|------|-----------|--------|--------------|--------------|----------|
| 18  | Kithkin mono-W anthem-aggro | 5/6 (FIRST-GUIDED) | all 3 wave-26 teaches VALIDATED (Dodger evasion fires every turn; ground-only flyer-block off-case exercised at the exact breaking matchup, 0 illegal attempts; trust-line tail p95 -35% / >10k spikes -63%). d1 corroborated. NEAR-TERMINAL. | byte-frozen | STAY; projected wave-28 rotation |
| 22  | "Giants!" mono-R tribal ramp | 5/6 (FIRST-GUIDED) | d2 changeling annotation VALIDATED + DISCHARGED (renders verbatim, misread dead, model provably reads it). Attack-floor killed passivity (2/6->5/6). 1 fallback = R-ANNOTATION-ECHO-PARSE (LOW, safe). KEYWORD-TYPE-CONSEQUENCE face: REFINE, single-seat. NEAR-TERMINAL. | byte-frozen | STAY; projected wave-28 rotation |
| 93  | Relentless Rats (24 Swamp/36 Rat) | 2/6 (Step-0) | d1 SYSTEMATIC AUDIT CLEAN PASS at the 36-Rat stressor (renders/stability/binding all correct, phantom slots safely dropped, negligible prompt size, 0 fallbacks). NEW class: MULLIGAN LAND-BLINDNESS (3-Swamp hand mulliganed 5/6 as "zero lands"). | NEW initial guide (rule-#1 Swamp-is-a-land + mulligan land-count) | STAY (Step-0) |
| 131 | UR Guttersnipe burn | 3/6 | favorable-block edit VALIDATED (3 correct, 2 declines = model inconsistency WATCH, not a corner); whole-corner discipline's FIRST validation (0 new corners, series terminated); priority pair benign. | frozen | **OUT (final)** -- longest-held seat, exits after 11 guided waves |
| 134 | mono-U/G Urza tron control | 0/6 (Step-0) | half construction ceiling (glacial control in all-aggro pool, no clock), half decisions (Map never cracked ~10 passes; Stonehorn+Flicker lock assembled 0 games; land-blinks; evoke-under-pressure). N-134a REVERSED (see below). | NEW initial guide (Map crack / blink loop / evoke gate / survival) | STAY (Step-0) |
| 136 | Yarok-of-Pranks BW midrange | 4/6 (Step-0) | ask-refusal class = ENGINE BUG R-RESOLVED-SPELL-PHANTOM-CHOOSER (model refusals were CORRECT; largest tax sink in corpus). Loyalty/Rankle surfaces UNEXERCISED (carry). Oracle checks clean. | NEW initial guide (no phantom-chooser crutch; board-state attack floor) | STAY (Step-0) |
| 137 | GW convoke + Adventures | 2/6 | **CONVOKE-NONRESOLUTION (ENGINE, HIGH, the wave's headline)**: 4-corpus "harmless defers" verdict REVERSED by resolution-tracing (Loxodon 2/23, March 0/17). Construction/ENGINE ceiling, not guide lever. d1 CLOSED at the motivating seat. | revision (single convoke-preference hunk) | STAY (fresh HIGH new-work) |

Core (general prompt): **PASS, 22nd consecutive** (7-of-7 seats route core to PASS; none proposes a core-prompt body
edit). Pilot repetition_penalty=1.05 (5th corpus): KEEP.

## The corpus story

**The loop caught two engine bugs that the win column hid, and validated its own d-batch.**

1. **CONVOKE-NONRESOLUTION (the headline).** A verdict that stood for FOUR corpora -- "deck137's convoke defers are
   harmless by-design" -- was REVERSED by asking a different question. Prior audits checked decisions ADJACENT to the
   defer (did deck137 still attack? yes). deck137's wave-27 reviewer asked whether the deferred CAST actually RESOLVED:
   Venerated Loxodon 2/23 resolutions, March of the Multitudes 0/17 (zero Soldier tokens ever created). The convoke
   tap-payment sub-step (deferred to the Baka heuristic) does not tap creatures for the convoke portion, so the cast
   aborts and reverts to hand. The model even chose the convoke variant over an affordable PLAIN cast (steered there
   by the old guide). Two repro paths: vs18 s16 (affordable convoke fails) and vs134 s34/s55 (plain cast offered
   alongside, convoke chosen, only convoke fails). This is the mechanistic root of deck137's 2/6 and its closing-speed
   gap. Layer = ENGINE/HARNESS; the guide edit this wave only routes to the working plain cast where one exists.

2. **R-RESOLVED-SPELL-PHANTOM-CHOOSER (deck136).** Legion's End's bare `donothing notaTarget(*|targetcontrollerhand)`
   reveal-idiom line resolves AFTER the target creature has left the battlefield; `targetcontroller` no longer
   resolves, the zone selector degrades to "any hand," and the engine surfaces it as an interactive target chooser for
   a spell already in the graveyard -- offering all cards in BOTH players' hands, none of which can satisfy the header
   ("a creature an opponent controls MV<=2"). The model's 4 refusals were CORRECT reasoning and were the single
   largest reasoning-tax sink in the corpus (top-3 replies ~15.5k). Deterministic (~seq7/turn3). Fix: zero-decision-
   content selectors (donothing/name(look)) auto-resolve, never model-call -- covers 3 other cards with the same idiom.
   NON-fix: no guide crutch (the legitimate Legion's End cast-target seam works perfectly; a crutch would poison it).

3. **The d-batch validated under live fire.** d1 got its stressor (deck93's 36-Rat systematic audit: CLEAN PASS) AND
   its motivator (deck137 vs131 s27: the exact wave-26 shape now binds correctly). d2 changeling annotation DISCHARGED
   at deck22 (renders verbatim; wave-26 2-game misread dead; model provably reads it -- copied the annotation into a
   CHOICE line, which incidentally surfaced R-ANNOTATION-ECHO-PARSE). d5 steady (clean loads). d3/d4 no exercising
   seat (no Kaldheim DFC in pool; DFC back-face fix carries as steady-state, its live firing UNOBSERVED per the probe
   report -- blocked upstream by the flip-thrash LIVELOCK, itself a carried wave-28 candidate).

4. **Hallucinated-Oracle pattern now 3/3 caught by verify-first.** deck134's N-134a flagged Moment's Peace flashback
   (engine `{2}{G}`) as diverging from a claimed "real Oracle `{5}{G}`" and routed it to verify-first. VERIFIED THIS
   SESSION against Scryfall: mana cost `{1}{G}`, Flashback `{2}{G}` -- the ENGINE IS CORRECT; the reviewer's `{5}{G}`
   was the hallucination (the engine's own `text=` line also reads `{2}{G}`). Item STRUCK. Third such reviewer
   hallucination caught by retrieving real Oracle text before recording a divergence -- the verify-oracle discipline
   is earning its keep.

5. **Step-0 -> guided pipeline is now 3-for-3 (59, 18, 22).** Two independent FIRST-GUIDED validations on different
   archetypes: deck18 (4/6 -> 5/6, all three targeted leaks converted) and deck22 (2/6 -> 5/6, double-lever). The
   pattern -- a Step-0-bis initial guide stating each leak as a phase-keyed DIRECTIVE reliably converts on the very
   next corpus -- is promoted to a validated method (skill Method headline 2).

## RANKED WAVE-28 LEDGER

Engine/harness work queue for the between-wave leg, ranked by (severity x provability x reach):

1. **CONVOKE-NONRESOLUTION** (HIGH -- ENGINE/HARNESS, gameplay-provable, two repro paths). Convoke cast variant aborts
   at the deferred tap-payment step; the card reverts to hand. Loxodon 2/23, March 0/17. Repros: vs18 s16, vs134
   s34/s55. Suspect: the convoke-payment path reached from AIPlayer's deferred-decision handling does not tap
   creatures for the convoke portion (REPRO A taps only a land); the X-spell March is worse (0/17 even when mana alone
   would cover a small X). Do NOT paper over with a guide clamp -- the guide edit only recovers cases where a plain
   cast exists; the engine fix is required for early tempo convoke where it is the only path.
2. **R-RESOLVED-SPELL-PHANTOM-CHOOSER** (HIGH-visibility / LOW-harm -- ENGINE card-script + legality-oracle,
   deterministic repro, fix shape known). A `notaTarget(...)` selector on a donothing/name(look) effect carries zero
   decision content and must NEVER generate a model-facing ask -- auto-resolve it silently (covers 3 other cards with
   the idiom). Violates no-ask-without-legal-options + ask-freshness. Repro: deck136 vs22/vs137/vs18 seq7 turn3.
3. **Flip-thrash LIVELOCK** (HIGH -- carried; blocks live Kaldheim-DFC play). `AIPlayerBaka::getEfficiency` toggles
   `source->isFlipped` via `std::rand` as a scorer side effect + `AATurnSide::resolve`'s AI branch
   (AllAbilities.cpp:5421) clears the flag without reverting the display -> the AI oscillates isFlipped indefinitely,
   never casting. This is the desync SOURCE and it blocked live validation of the d4 DFC back-face fix (probe report).
   AI-scorer / shared-AATurnSide owners.
4. **N-A basic-land annotation at hand/mulligan surfaces** (MODERATE -- REPRESENTATION, first live defect). Annotate
   basic lands `Swamp (land: taps for {B})` / `[land]` at the hand + mulligan render. deck93 mulliganed a 3-Swamp hand
   5/6 as "zero lands." Graduates the standing "mulligan surface unexercised" carry to an active candidate; the guide
   rule-#1 is the first lever, this annotation the representation hedge if the guide alone does not stabilize it.
5. **R-ANNOTATION-ECHO-PARSE** (LOW -- PARSER, safe-direction). The name-anchored answer matcher must be
   annotation-tolerant: match choice NUMBER + card-name PREFIX, or strip bracketed `[...]` tails before comparing an
   echoed name. deck22 vs131 s3 (model echoed `[changeling: counts as Giant]` into its CHOICE parenthetical -> safe
   heuristic fall, 1/146 benign). First observed representation-fix-induces-parser-fallback coupling.

Then (lower / watch): N-134d Mulldrifter double cast-mode ask (possible redundant seam -- verify the top-level "with
its evoke cost" and the following normal/evoke sub-menu are not the same decision surfaced twice); N-134b Moment's
Peace graveyard-cast option displays base `{1}{G}` not the `{2}{G}` flashback it charges (display-accuracy, LOW --
NOTE: this survives the N-134a reversal; the cost the engine charges IS `{2}{G}`, so the DISPLAY under-states it);
N-134c Pulse-of-Murasa rider-magnitude annotation (annotate `{gain 6 life}` alongside the target warning, mirroring
Gray Merchant); N-B summoning-sick board marker; L2 cross-zone same-name naming; carried: ETB header card-name, N8
blocker-gang-chump, Azcanta probe, undying trade-annotation, commit-lock watch, prose-reversal watch,
untapped-tagging (below-bar), Guttersnipe all-tapped WATCH, I-4 wobbles, L-18-1 trivial-decision planning tax,
favorable-block model-inconsistency WATCH (deck131), illegal one-onto-many gang-block reflex under lethal pressure
WATCH (deck137: c4 first-legal-pair parser recovers it; watch for a case where recovery discards a BETTER block).

**CLOSE list** (discharged / resolved / reversed -- do not carry):
- **R-DUPLICATE-NAME-INSTANCE** -- CLOSED. Stressor (deck93 36-Rat audit) AND motivator (deck137 vs131 s27) both
  clean; d1 validated corpus-wide.
- **R-CHANGELING-TYPE-INFERENCE** -- discharged at deck22 (d2 render annotation validated; misread dead).
- **N-18c** (Mobilization discoverability) -- pre-empted per the deck18 seat; no recurrence.
- **N-134a** (Moment's Peace flashback `{5}{G}` claim) -- REVERSED by verify-first (engine `{2}{G}` is correct per
  Scryfall). STRUCK.

## Rotation reconciliation

- **OUT (final): deck131.** The longest-held seat exits after 11 guided waves. The wave-26 whole-corner edit left
  nothing new to find: the favorable-block exception was exercised in 6 blocker windows and generated ZERO new corners
  (the recalibration series terminated as the discipline predicted); the 2 declines are model-inconsistency WATCHES,
  not a 4th corner; the priority pair is benign; the tapped-read guard was clean round 5. Guide frozen. The
  whole-corner discipline made this rotation call honest and on-schedule (skill Method headline 4). deck131's guide
  moves to the canary set (canaries now 16).
- **STAY: 137, 18, 22, 134, 93, 136.** 137 stays on fresh HIGH new-work (CONVOKE-NONRESOLUTION); 134/93/136 are
  Step-0 seats (never rotate on debut); 18/22 are FIRST-GUIDED validations. **PROJECTION: 18 + 22 are projected
  wave-28 rotations** -- both are NEAR-TERMINAL (guides byte-frozen, all teaches validated, only below-bar residuals
  remain: 18's L-18-1 tracking item and its author-downgraded flyer-block candidate; 22's REFINE-only face + the
  benign annotation-echo). If both exit at the wave-28 reconciliation, wave-28 may need up to THREE rotate-ins (18, 22,
  and any Step-0 seat that converges) -- flag for that reconciliation; the pick below fills only the ONE open slot NOW.

- **ROTATE-IN (the one open slot): deck146 "Orzhov Dungeons"** (guideless Step-0, no `deck146_strategy.txt` in
  bin/Res/ai/baka/). RATIONALE -- maximizes NEW-SURFACE coverage vs the wave-28 pool, which contains no planeswalker
  deck and no venture/dungeon deck. deck146 exercises TWO surfaces that are UNEXERCISED across the entire campaign:
  (a) PLANESWALKER LOYALTY asks (Kaya the Inexorable x2 -- the loyalty +/-/ultimate ask surface has never fired
  cleanly; deck136's Liliana/Ugin were cast at most once with no observed loyalty ask), and (b) the VENTURE /
  DUNGEON room-choice mechanic (`#DNG:Dungeon of the Mad Mage`, Triumphant Adventurer x4 -- a modal "choose the next
  room" ask class never seen in any corpus). A Step-0 debut on a two-new-mechanic deck is exactly the roster's job:
  new surfaces feed the ledger whether the deck wins or loses. RUNNER-UP (lower variance if deck146's dungeon engine
  proves fragile or the deck too weak to reliably cast Kaya): **deck152 "Bant Midrange"** (Teferi, Who Slows the
  Sunset x3) -- pure single-surface loyalty coverage, more likely to reliably cast + tick loyalty, but strictly less
  new surface than deck146.

- **Guideless roster count:** 164 `deckN.txt` files, 20 live strategy guides pre-wave-27. After wave-27 deploys the
  three new initial guides (93 / 134 / 136), the guided roster = 23 and the guideless roster drops to ~141 (the
  brief's authoritative campaign figure). deck131 rotates OUT but KEEPS its guide (moves to canary). deck146 becomes
  the wave-28 Step-0 rotate-in and stays guideless until its wave-28 debut produces a guide.

## Core prompt

**PASS -- 22nd consecutive (7/7 seats).** No core-prompt body edit is proposed by any seat. Every finding routed to
the guide lane (per-deck strategy.txt), the engine ledger (notes.md), or the method (skill.md). Notable
scope-honesty confirmations: deck136 (mono-black, generic costs) showed ZERO color-anxiety / mana-veto tax --
confirming the cross-phase mana-fact rung needs a color/hybrid confound and should NOT be inflated into a mono-color
core line; deck93's "take damage while ahead" failure happened DESPITE the correct core line rendering verbatim (a
model-adherence miss, a guide lever, not a core gap); deck136's phantom-chooser refusal is an engine ask no core line
should try to fix. The one universal candidate ("you have no flyers/reach -> can't block flyers, race") was
DOWNGRADED by its own deck18 author -- the guide lane handles it and a core line is not needed while it does.
