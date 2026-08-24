# Wave-41 engine ledger — THE WAVE-42 DOCKET (assembled 2026-08-24)

Sources: wave-41 seat reports (seats/), fix-validation verdict table, probe-results.md,
prior seeds carried from wave40/engine-ledger.md appends. Ranked.

## Open items (ranked)
1. **#W42-D1 (HIGH) Tutor-reveal collapse + hidden-info leak.** Search/reveal narrates
   the whole library card-by-card to BOTH seats: 1,546 event lines (12%), 2.53M chars =
   15.2% of ALL assembled prompt text this corpus; observer names opponent library
   contents (377 lines) that MTG rules never grant (shuffled search reveals only the
   found card). Fix shape: lane-B bulk-collapse applied to the search/reveal path +
   observer masking of unfound cards. Cites: ...891-deck126 seq 9 (162-line delta),
   ...863-deck130 seq 12, ...898-deck162 seq 6. Also fold D9's mulligan shuffle-back
   (7-line spam, bottoming never narrated) into the same collapse.
2. **#W42-3 Attack-side combat-outcome annotation** (perception layer). Blockers windows
   get computed parentheses; attackers windows get bare "A1. Name (P/T)". Observed cost:
   hallucinated outcomes both directions (deck146 vs126 s9 "Silencer dies to Battlement"
   — false; the s51 hallucinated-deathtouch attack). Proposal: "- their untapped
   blockers: <name> (P/T) (<outcome>)" mirroring the ladder. Guide prose cannot fix a
   number the model must derive.
3. **#W42-D2 Observer/actor narration unification.** The 165/0 asymmetry INVERTED
   (236/419); 17 of 20 game pairs disagree on the same activations — actor writes only
   consumed decisions, WEventAbilityActivated fires for everything. Unify so the actor
   also narrates heuristic/auto-resolved/trigger-internal activations. TIES TO the open
   OWNER QUESTION: "Opponent chose ..." vs "used" for choice-bearing triggers.
4. **#W42-D3 Sourceless/anonymous activation lines** (13 no-source + 29 "with this
   effect's X ability") — the new observer event needs lane B's waiting->source ladder.
   **#W42-D4 Engine lexicon leak**: ToughLife(9), Put in Play/Hand/Library(43),
   "1/1 Counter"(57, double-space), level Counter(12), Deal 1 Damage(168); malformed
   reveal grammar ("You revealed 51 and put Exquisite Blood to choose card"). One
   render-name mapping pass covers both.
5. **#W42-D5 "the only legal targets are YOUR OWN right now" names nothing** (59 lines;
   trust doctrine — silent omission invites confabulation). Render owned targets + facts.
6. **#W42-1 (carried) Unanswered multi-candidate mandatory chooser -> loud red, never
   guess.** Suite-driver default still undefined for multi-candidate.
7. **(carried) DFC flip-line sweep**: 59 transform-DFC fronts carry Delver's illegal
   anyzone doubleside line.
8. **(carried) Burnout zoneless-teach** primitive bug.
9. **#W42-4 Token battlefield-render collapse**: byte-identical token entries -> one
   ranged line (measured 8% of a 19.7KB prompt; seats 123+126 independently).
10. LOW batch: **#W42-D6** [repeat:] counter not advancing on land-animation abilities
    (Hive of the Eye Tyrant, both misses); **#W42-D7** ceases-to-exist moves render
    "your zone" destination (84 lines); **#W42-D8** events delta sliced mid-line
    (1/1808, char-offset not line boundary).
11. **(carried) Baka-cannot-X=0 residual** (slack penalty + >0 force-gate on Baka's own
    seat).
12. **INFO/economy**: PLAN-line emission 76% despite plan-on-change rule; adjacent
    identical no-op pairs scale with game length (~10 in one long game); decision
    latency medians 81-135s at -j 21. Prompt-economy review candidates, not defects.
13. **#W42-2 false arm on restricted bodies** — PARTIALLY DISCHARGED guide-side this
    wave (deck152 E1 / deck146 gate exclude [defender]/can't-attack). OPEN residual:
    sweep OTHER guides using power-floor patterns for the same blindness.
14. **Watch items**: may-batch turn-scoped verdict edge (0/0 two corpora); deck125 E5
    sweeper split (0 windows in 543 — cut or probe-force next wave); equip done-signal
    no-op form never offered (0/0); deck162-dependent R4 prediction (162 held in pool).

## Discharged this wave (cites in seats/ + probe-results.md)
#W41-4 mutate render (probe 35/35); rung-3 adjudication (2 offers taken = rule-obedient
misplays -> E2/E7 carve-outs); armed-sole-attacker (3-corpus null -> retired, E1);
deck162 Rule-4 crutch (register condition met -> retired); wave-41 lane predictions per
fix-validation verdict table (narration/may/X/morbid/flip/prevention CONFIRMED).

## WAVE-42 STEP-1 OUTCOME (2026-08-24, merged at 6788e6171, gate 1093/0 + 33/0, PARSETEST 928/0)
All 7 lanes landed: A #W42-D1+D9 (search collapse+mask; probe 58/58 <=3 lines, 0 leaks);
B #W42-3 (attack-side forecasts, one math source re-voiced; 539/539 coverage, 10/10
hand-checked); C #W42-D2+D3 (unified narration, storedSourceCard ladder, verb seam for
the owner question; 0 observer-only/0 sourceless in probe); D #W42-D4+D5 (getMenuText
render boundary, derive-not-table; your-own-targets now named); E #W42-1 (loud-fail
mandatory chooser, 500-tick grace, empirically forced) — AND FALSIFIED #W42-D6:
ActivatedAbility::counters was CORRECT; the real defect was missing instance handles on
same-named permanents' option lines (de-dupe collapsed two Hives) — #N handles shipped;
F DFC sweep (59/59 fixed by deleting the illegal anyzone hand-flip line — every card
already carried its faithful mechanism; 4 'may' restorations Group D) + Burnout (zone
AND missing 'controller' rider) + 5 fixtures; G #W42-4+D7+D8 (token-run collapse 100%
on 5,100+ boards; ceased-to-exist narration; delta re-based on structural pending-lines
— the old byte-offset was broken by the 24K front-trim, worse than the 1-record cite).

NEW ITEMS from step-1 (wave-43 docket seeds):
- CS-023 candidate: Ludevic's Test Subject transforms at ONE hatchling counter, not 5
  (bare this(...)-gate + && andAbility bypass; Grizzled Angler's if-then idiom is the
  fix shape; lane F evidence, empirical). Expect a 13/13 for 2 mana in corpora until fixed.
- Docent of Perfection: transform is scripted as a state trigger, Oracle is cast-then-if
  (lane F, expressible via the Grizzled Angler idiom).
- borderline.txt carries 7 doubleside(backside) lines needing the same per-card Oracle
  sweep (modal-DFC doubleside(<name>) forms are LEGITIMATE — do not sweep those).
- #W42-D2r: mana-tap narration asymmetry (actor-only by design; owner/seat call).
- Tutored library->hand renders as "You drew X" (search is not a draw; D4-style pass).
- Observer never learns a mulligan happened (public info in real MTG; deliberate no-widen).
- Single-candidate mandatory NON-player chooser still has no auto-resolve (lane E scope
  note) — would wedge as timeout, not loud-fail.
- Suite instrument warning (lane F): a truncated-but-exit-0 suite run reports 0 failures
  and LOOKS GREEN — always compare the COUNT, not just failures.
- Method: WAGIC_FASTCLOCK=0.1 on suite runs ~10x faster (SDL_WaitEventTimeout throttle).

## OWNER LIVE REPORTS (2026-08-24, fixed same day, merged at ee63779b5, gate 1093/0+33/0, PARSETEST 944/0)
Three narration reports, one lane, all mechanism-level:
1. Rules text in historic log ("Seachrome Coast enters tapped unless..." — his words:
   "doesn't need to be in historic log"): describeTarget's decision-surface card-text
   tail was reused verbatim by the narration seams. Fixed with a decisionSurface render
   gate — option lines/board render keep full text, the log states only the event.
   Wave-41 corpus exposure: 2,036 lines.
2. Damage double-entry ("- Opponent lost 1 life (now 34)" + "- Dwarven Blastminer dealt
   1 damage..." — "may be confusing... unnecessarily verbose"): Damage::resolve raises
   WEventLife + WEventDamage for the same delta. Fixed with WEventLife::fromDamage set
   at that ONE raise site; damage renders as one attributed line with the result
   ("dealt 1 damage to the opponent (now 34)"); non-damage life changes (payments,
   drains, lifelink gains) keep their own lines; hold-and-flush so nothing drops.
   Corpus exposure: 503 adjacent pairs. Toxicity branch raises no life event (pre-
   existing, untouched).
3. 'Mountain - "R"' in target narration ("- \"R\" not needed"): SAME emitter as (1) —
   Mountain's primitive text= is literally "R", so the card-text tail rendered as a
   mana hint. Covered by the same gate; bare names in narration, verified vs the
   wave-41 #23 Swamp specimen shape.
Predictions for the next corpus in the lane report (zero quoted narration tails; zero
adjacent pairs; 100% damage lines carry (now N) except toxic/prevented).

## OWNER LIVE REPORTS round 2 (2026-08-24, Vita play on the fresh VPK; merged at e4f4e0bec,
## gate 1096/0 + 35 AI/0, PARSETEST 944/0)
4. Delver never triggered (t1 Island+Delver, t2 drew Into the Roil): ROOT CAUSE was NOT
   the card (script Oracle-faithful) and NOT auto-skip (hypothesis falsified with a
   traced real-game repro) — the held phase-advance button's JGE auto-repeat (0.5s +
   70ms) is ALSO the reveal display's decline key, and MTGRevealingCards read input
   queued before it existed. CLASS FIX: input flush (ResetInput, clears holds) +
   one-frame arming on MTGRevealingCards AND MTGScryCards (~500 reveal/scry cards).
   New harness commands: holdkey/releasekey (real JGE input) + realgame (drops
   mSuiteGame); fixture delver_of_secrets_held_trigger.txt RED-before/green-after,
   20/20 stable. Residual: desktop OS-keyboard auto-repeat narrows but not fully closed
   (JGE-owned repeat path — the Vita — is fully closed).
5. Beastcaller Savant's mana paid for Captain's Claws (artifact): the engine had NO
   spend-restriction concept — the corpus idiom is only an ACTIVATION gate. NEW
   MECHANISM: manarestriction{<TC spec>} on mana abilities (parsed before the
   restriction{ substring scan — ordering matters), stored on AManaProducer,
   enforced at ManaEngine::spendAllowed inside producerUsable -> ALL of potentialMana
   + planPayment; legality/castability/auto-tap/tap-preview inherit free (shared
   substrate). Human seat had the identical hole. Fixtures 4 (incl. autotap harness
   command — suite seats are all AIPlayer-derived, scripted clicks can't reach the
   human payment path: a control failing exposed that false green).
   BLAST RADIUS (wave-43 docket): 86 cards carry "Spend this mana only"; 85 still
   mis-modeled. 48 activation-guarded are near-mechanical manarestriction adds BUT
   many are "...or activate abilities of X" which the current predicate would
   OVER-restrict (needs an alsoabilities flavour); 38 have no modelling; plus
   activation-only, cost-shape ({X}, cumulative upkeep), and zone/mode (flashback,
   foretell) classes the mechanism does not cover. Apply per-card with Oracle
   verification, never blanket.
Both fixes are ENGINE-level: Vita needs the new VPK (built + uploaded this session).
