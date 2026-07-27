# Wave-30 synthesis notes (SIXTEENTH per-deck-rotation cycle)

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260726-181642`, binary `6e168aa05`, -T 3000,
REPPENALTY=1.05 (8th corpus). 21 games, 0 timeouts / 0 draws, **12 cap-adjudications** (grindiest
pool of the campaign; 18 gameend logs = 42-24, integrity clean). Fallbacks **4 / 1,474 decisions
= 0.27% (CAMPAIGN RECORD)**. Retry fired 0x. Core (general prompt): **PASS, 25th consecutive**.

## Verdict table

| Deck | Rec | Stage | Verdict | Live work / why |
|------|-----|-------|---------|-----------------|
| 122 | 6/6 | first-guided PERFECTA | **NEAR-TERMINAL, wave-31 exit** | 4th Step-0->guided pipeline arc. All 5 wave-29 teaches live-validated; guide byte-frozen; 0 fallbacks. Only residual = N-122a (Felidar wingame never fired live -- probe-deck task, not a seat hold). |
| 136 | 4/6 | mature | **ROTATE OUT (unsentimental)** | Every guide surface exercised + correct (Ugin cast at last, correct -3; Liliana; MV3). Guide frozen. Only live item = N-136a (engine seam, ledger + probe). |
| 146 | 3/6 | 2nd guided | **KEEP (near-terminal, exit wave-32)** | g5 PASS -> N-146f retired; Lolth loyalty guide-perfect. Guide frozen. Kaya = accepted draw-variance on a validated surface. Confirm freeze holds a 3rd corpus. |
| 137 | 3/6 | mature | **KEEP -- exit gate NOT met** | Arrival-tracing found 2 defers + 3 UNLOGGED silent no-ops (~55% offered-convoke resolution). The convoke arc is NOT closed. Guide byte-frozen; blockers held; re-pick churn tamed. |
| 139 | 1/6 | first-guided | **STAY, gated on N-139h** | g3 VALIDATED (all 4 render gaps closed, before/afters). Guide REVISED (stopgap retirement). Record drop = N-139h crash + pool/variance, NOT a misleading guide. Latency tail relocated to combat math. |
| 148 | 2/6 | first-guided | **ONE confirming corpus then ROTATE** (construction hand-off) | All 4 teaches fired; construction-ceilinged CONFIRMED (+1 win + quality upgrade = exactly what the shell allows). Freeze guide, hand construction to roster owner. |
| 152 | 2/6 | Step-0 | **STAY (Step-0 never rotates)** | Initial guide drafted. Werewolf live-flip = HONEST NULL (no spell-less turn ever). Class level-up clean; 3-color mana accurate. N-152a + N-152b new engine items. |

## The corpus story -- the g-batch, and the wave's honesty headline

**A five-fix g-batch was swept for validation and TWO of the five had their validation CORRECTED
at the seat.** The loop's own instruments caught its own optimistic reads. This is the headline:
a fix-batch prediction is a HYPOTHESIS to falsify at the seat, never a fact to confirm in prose.

- **g3 (mutate render batch) -- VALIDATED decisively.** deck139 confirmed N-139a/b/c/d CLOSED
  with before/after render quotes (loot-as-discard misread dead; merged pile ONE line; role-named
  mutate steps + CR 725; unified "mutate cost" label). Perception latency on the flow: 194s->11s.
- **g4 (tapped-tag reword) -- VALIDATED.** `[tapped - cannot attack or block this turn]` on
  creatures, bare `[tapped]` on lands; read correctly 131x at deck137, no misread pool-wide.
- **g5 (dungeons-completed status line) -- VALIDATED -> N-146f RETIRED.** deck146 reasoning flipped
  from wave-29 second-guessing to confident boosted combat math ("completed a dungeon, so Nadaar
  5/5"). Route was an ENGINE render addition, NOT a guide line (kept the guide silent on it).
- **g1 (convoke payability-before-offer) -- PARTIAL, and the METRIC was wrong.** Defers 8 -> 2,
  first MULTI-token March (X=3), churn tamed. But ARRIVAL-TRACING found 3 UNLOGGED silent no-ops
  the defer counter never saw: true offered-convoke resolution ~55%. The X-affordability
  annotation remained a PERFECT resolution predictor. The gate ("defers -> 0") was FALSE-CLEAN by
  the counter; the seat KEEPS.
- **g2 (PUT-predicate filter) -- FALSIFIED, PATH-SCOPED.** All four Mausoleum Secrets reveals still
  offered 42/38/43/39 options (near-whole-library, incl. illegal); silent no-op fired 2/4; the
  empty-eligible no-ask path did NOT engage at 0-creatures-graveyard. The g2 filter landed on the
  SEARCH reveal path (a "1 of 5" filtered reveal was seen at another seat this corpus) but NOT on
  Mausoleum Secrets' `revealzone(mylibrary)` path. N-136a STILL LIVE. The wave-30 fix agent's live
  game hit an eligible pick by luck, masking the gap.

**What let the two slip, and what the skill now guards:**
- *g1's metric* -- the defer/fallback COUNTER undercounts a failure that can also end in an
  UNLOGGED silent no-op. The skill now MANDATES arrival-tracing for any "residual -> 0" exit gate
  (Step-2 resolution-verification rung, promotion-grade, 2nd counter-hid-defect instance).
- *g2's path scope* -- a fix landing on one code path does not close a class-named gap on a sibling
  path. The skill now requires verifying a tutor/reveal fix by offered option COUNT AND
  card-reaches-hand, qualified by WHICH reveal path (harness-defect sweep + DoD).

**Other threads.** (1) deck122's 4th clean Step-0->first-guided->validated pipeline arc: a
well-worn teach category (alt-win + life-as-engine) PREDICTED the win, corpus confirmed 2/6 ->
6/6. (2) NEW engine crash N-139h: a mutate under Pollywog's discount tripped `ManaCost::remove`
assertion and core-dumped -- the SAME path whose static N-139g just validated positive; a feature
and a latent release-build cost-corruption bug share the code. (3) Fallback record 0.27% -- the 4
are 2 deck137 convoke defers + 2 priority unparsed (deck152 fabrication spiral at a cosmetic-only
option; deck146 over-reasoning spiral inventing an off-menu play). (4) **The 12-adjudication rate
is a POOL-COMPOSITION property, not a signal** -- it inflates metric-aligned decks (lifegain adj
on life) and punishes slow seats (deck139's 200s combat decisions -> adjudication-behind losses).

## RANKED WAVE-31 LEDGER

1. **N-139h -- `ManaCost::remove` assertion `cost[i] >= 0` / CRASH on discounted mutate (HIGH --
   crashes games).** Mutating under Pollywog's {1}-off discount drives a mana component negative;
   debug build core-dumps, release build silently corrupts the cost. Fired 1/21 (vs148 mutated the
   same {1}{g}{b}{w} Nethroi WITHOUT crashing -> config-specific path). Cost deck139 a game
   (recorded as a turn-4 "adjudication"). Repro: pin deck139 selfplay with Pollywog in play, force
   a Nethroi (or other multi-symbol/hybrid) mutate; instrument `ManaCost::remove` near
   src/ManaCost.cpp:973 to log the (cost, delta) going negative.
2. **N-136a -- revealzone-path PUT-predicate filter (MED-HIGH -- the g2 gap).** Extend the g2
   filter to the `revealzone(mylibrary)` reveal seam (Mausoleum Secrets), not only the search path.
   Filter offered cards by the option-level target predicate `[black; manacost<=type:creature:
   mygraveyard]`; if the constrained set is EMPTY, auto-take optiontwo (shuffle), do not ask.
   Repro in-corpus (no new run): vs148 seq22 opts=42 chose Rankle MV4 -> never reached hand; vs152
   seq12 opts=43, 0 GY creatures, chose Ritual MV4 -> never reached hand. Reproduce on the current
   binary first.
3. **Convoke offer-vs-resolution gap (HIGH -- the g1 arrival-traced residual).** ~55%
   offered-convoke resolution at deck137; 3 unlogged silent no-ops (engine consumes the pick, taps
   mana, abandons the cast). One no-op was on an AFFORDABLE Loxodon -- the offer gate and the
   resolution path still DISAGREE. Exit gate for the seat: "offered choices producing no downstream
   arrival -> 0" verified by arrival-tracing, NOT by the fallback counter.
4. **N-152a -- MDFC play-land menu offers only the FRONT face (MED).** The play-land menu lists
   only the front-face color; the back (blue) face is hidden behind the cosmetic Flip-Side toggle
   -> Teferi resolved 0/6, blue near-inaccessible. Fix: list BOTH faces with colors in the
   play-land menu ("Play Barkchannel Pathway (taps {G})" / "Play Tidechannel Pathway (taps {U})").
5. **N-152b -- cosmetic-only-option suppression at a bare priority window (LOW, sanctioned class,
   now with fabrication evidence).** A bare-priority window whose ONLY non-pass "action" is the
   Flip-Side DISPLAY TOGGLE is a FABRICATION ATTRACTOR (deck152 seq27 -> model invented "Cast
   Briarbridge Tracker" + over-derived its mana). Suppress the lone cosmetic toggle at a
   no-real-action priority pass, or render "nothing to do -- pass (0)". This strengthens the
   sanctioned no-op-suppression case that wave-30's annotate-don't-filter decision had deferred.
6. **N-146g -- walker magnitude eval misfire (LOW, render).** Lolth's Cast + loyalty options render
   spurious `{right now: damage 8/4}` (probably evaluating the -8 emblem drain out of context).
   Model was not derailed. `WParsedInt` should likely skip planeswalker loyalty/emblem text the way
   it skips "rand". Shares a likely root with the Rankle mode-bleed magnitude-eval watch.
7. **N-122a -- Felidar Sovereign 40-life wingame probe (carry).** State reached live (51 life
   vs136; exactly 40 vs139) and the pilot PURSUES the line, but Felidar was never drawn during a
   40+ upkeep. Probe-deck live validation (stack Felidar + fast redundant lifegain, pin one
   selfplay, confirm the upkeep `wingame` fires) -- between-wave engine task, NOT a seat hold.
8. **Latency-tail WATCH (relocated, 2 mechanisms).** deck139's 195-209s tail MOVED off the mutate
   render onto combat/big-board math (double-strike/trample/reanimation). On the wall-clock cap this
   causes EARLY adjudication-behind losses (2nd loss mechanism beyond truncation). Guide-side hedge
   shipped ("decide combat fast" with the adjudication clock rationale); core soft-reasoning-bound
   candidate IF it reproduces cross-family.
9. **Cross-deck MODEL watches (no core change yet).** (a) the "invent an off-menu option matching
   the PLAN" over-reasoning spiral (deck146 vs152 + deck152 vs136, BOTH at bare-priority windows);
   (b) the cosmetic-only-option fabrication attractor (N-152b, same carrier). Revisit for a core
   item only if either recurs at more seats with a shared root.

### CLOSE / RETIRE this wave
- **N-146f** (dungeons-completed status line): RESOLVED by g5, retired.
- **N-139a/b/c/d** (mutate render batch): CLOSED by g3, before/after validated, retired.
- **N-139g** (under-card statics function on the pile): ANSWERED POSITIVE, retired (same path as
  N-139h -- correct behavior and the latent underflow share the code).
- **N-139e** (Migratory Greathorn land-search is mutate-ONLY): CLOSED, reconfirmed (verify-oracle
  SAVE), so no future reviewer re-opens it as a "missing ETB" bug.
- **g4** (tapped-tag reword) + **N-93c** (the render it closed): VALIDATED (131 clean renders at
  deck137), retired.
- **Ugin carry / Liliana both modes / Rankle combat-damage may-ask mode-bleed:** all RESOLVED at
  deck136 (Ugin cast + correct -3; Rankle clean 2 games, no priority bleed). Retired.

## Core prompt -- 25th consecutive PASS (documented)
7-of-7 seats routed core to PASS. deck137 general-suggestions.md explicitly logged the 25th. No
reviewer proposed a core change. Every residual is engine-lane, deck-specific, or a single-seat
model-watch -- none generalized into the symmetric core prompt. Boundary held under N-139h, N-136a,
the convoke residual, and the two fabrication/over-reasoning spirals.
