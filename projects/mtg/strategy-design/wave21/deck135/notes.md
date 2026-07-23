# Deck-135 wave-21 — development notes (engine / harness / model) + ROTATION VERDICT

Run: PRIMARY `matchups-20260723-084938`. Binary `/tmp/wagic-3be7f5a67` (wave-21 engine batch, LIVE-
UNVALIDATED). deck135 owns: ENGINE-R1 second confirmation, Azcanta activation, HARNESS-1 retracted-
CHOICE, and THE REGRESSION QUESTION. Layer-routing enforced: engine/harness items live HERE with seq
repros, NEVER in guide text. Game->file map in findings.md.

## ENGINE LEDGER

- **ENGINE-F1 (NEW, deck135-OWNED, HIGH-VALUE — the corpus's top new engine finding) — FETCHLAND
  ACTIVATIONS INTERMITTENTLY FIZZLE (item-8(c) class, on fetchlands). This is the primary driver of
  deck135's 4/6 -> 1/6 regression.**
  - Repro (vs35 s6, turn 2, priority): chose `Put in Play with Windswept Heath targeting Snow-Covered
    Forest` (cost `[Tap, Life, Sacrifice]`). Event log: `Your Windswept Heath: hand -> battlefield` ->
    `You: Put in Play with Windswept Heath ...` -> `Your life -1 (now 19)` and then NOTHING: no
    `Windswept Heath: battlefield -> graveyard` (sacrifice), no `Snow-Covered Forest: ... -> battlefield`
    (land). The ability paid Tap+Life then FIZZLED before the Sacrifice+search. Fetchland stayed tapped
    on the battlefield; the `Put in Play` option RE-ARMED (2 -> 4 options after the 2nd failed fetch at
    s12). Model then PASSED it for the rest of the game (its plan believed the fetch succeeded).
  - Second seat game (vs27 s3/s4/s7/s8/s11): repeated re-picks of the SAME fetch (Prismatic Vista at
    s3,s4,s11; Misty at s7,s8) = the item-8 re-arm loop; 0 fetches resolved all game.
  - Contrast (proves intermittency) — vs62 s14/s15 WORKING fetch: `Put in Play with Flooded Strand
    targeting Snow-Covered Island` -> `Your Flooded Strand: battlefield -> graveyard` -> `Your
    Snow-Covered Island: library -> battlefield`. Full resolution. The model re-attempted s14->s15 until
    it stuck; vs62 was the only win.
  - Fizzle scan: vs35 0/2 resolved, vs49 0/3, vs27 0/5, vs102 ~0-1/3, vs14 2/5, vs62 1/2 (win). The
    three total-fizzle games are all losses; the working-fetch game is the win.
  - CLASS: item-8(c), which the wave21 item8-diagnosis explicitly left OPEN (needed a discriminating
    probe). This is a clean NEW repro on FETCHLANDS: a Tap+Life+Sacrifice activated ability that pays
    Tap+Life then aborts before Sacrifice. Candidate mechanisms from the item8 diagnosis both fit and are
    consistent with the fetch being cracked at a PRIORITY window: (i) `ActivatedAbility::isReactingToClick`
    `source->controller() == game->currentlyActing()` mismatch if the acting player shifted between
    clickstream ticks; (ii) stale `MTGAbility*` producer/ability pointers between plan tick and click
    tick. The Sacrifice ExtraCost sub-click is the specific click being refused (Tap+Life already paid).
  - REGRESSION note (evidence-led): item-8(c) existed in wave-20 (it was diagnosed FROM wave-20 data), so
    the 4->1 swing is largely the SAME persistent intermittent bug landing on different games (variance
    in which games the fizzle strikes) — NOT a new side-effect, NOT play-quality decline. HOWEVER wave-21
    (b) ("verify the click landed; on refusal abort the remaining plan") IS in this code path; the
    "pay-life-then-fizzle" signature suggests (b) may have converted item-8's original "float mana / loop"
    into "pay a partial cost then abort." Cannot discriminate without wave-20 fetch-level data or a code
    probe.
  - FIX: extend item-8 fix (a)/(b) to cover the fetchland Sacrifice-cost sub-click — verify the Sacrifice
    click lands; on refusal, ROLL BACK the already-paid Tap+Life (do not leave a partial cost paid) OR
    complete the sacrifice. And/or re-resolve the ability by source card at click time (mechanism ii
    hardening). The landed-verification from item-8 (b) should convert this from "pay life, fizzle,
    re-arm, model believes success" into a clean state. VALIDATION DETECTOR (next corpus): a `Put in Play
    with <fetchland>` / `search basic land with <fetchland>` CHOICE followed by `Your <fetchland> life -N`
    with NO subsequent `<fetchland>: battlefield -> graveyard` and NO `Snow-Covered X: -> battlefield`.
  - deck135 is the pool's most fetch-dependent deck (its entire mana plan is fetch-a-color-then-cast-a-
    {G}{U}-threat), so this bug bites it hardest. High priority.

- **ENGINE-F1b (companion to F1) — the fizzle is SILENT to the model, so it never re-cracks.** No
  negative event, no "cast failed" line; the model's own PLAN narrates the fetch as successful (vs35 s6:
  "Once the Forest is in play, I will tap it..."), so at every later re-offer it PASSES instead of
  re-cracking. The guide's re-crack instruction ("if you still have no extra land in play, you have NOT
  cracked yet — crack it now") is correct and cannot help because the model can't perceive the failure.
  A one-line engine event on a fizzled/aborted activation ("Your Windswept Heath activation was cancelled
  — no land was fetched") would let the model recognize it and re-crack. Route: engine/perception. NOT
  guide-fixable (would be papering an engine bug into prompt text = layer-routing violation).

- **ENGINE-R1 (wave-19 deck135-OWNED HIGH-VALUE bug) — SECOND CONFIRMATION LANDED. n now = 2. CLOSE.**
  - vs14 s23 (turn 8): Glacial Revelation, `PUT: 1..6` (all six, all snow permanents — Snow-Covered
    Forest, Arcum's Astrolabe, Scrying Sheets, Ohran Viper x3 [Coldsnap Ohran Viper is a Snow creature]).
    Events (s24): all six `goes to hand`, ZERO in a `goes to graveyard` list. Regression detector NEGATIVE.
  - With wave-20 vs27 s18 (mixed: 4 snow -> hand, 2 non-snow fetches -> graveyard), the partition is now
    confirmed on TWO corpora across both the mixed-eligibility and all-eligible shapes. The wave-19
    all-to-graveyard behavior is gone on a second independent sample. CLOSE ENGINE-R1.

- **ENGINE-R6 (NEW, deck135-OWNED — REPRESENTATION) — the Azcanta TRANSFORM option is offered WITHOUT an
  eligibility annotation, so the model manually miscounts the graveyard and DECLINES a beneficial,
  available transform.** vs14 seq 29 (turn 10 upkeep) + seq 40 (turn 12 upkeep): options
  `["Transform:azcanta, the sunken ruin", "Decline - do nothing"]`. The engine only offers Transform when
  the 7-cards-in-graveyard threshold is met, so the transform WAS available. Both times the model
  recounted its graveyard from the event log (arrived at 5-6), concluded "I don't have 7," and Declined.
  Real misplay in a grind it lost (13/18). FIX: annotate the Transform option like reveal eligibility —
  e.g. `Transform:Azcanta [available now — 7+ cards in graveyard]` — so the model doesn't fill the vacuum
  with an error-prone manual count. Same family as the reveal eligibility-surfacing that already shipped.
  Consequence: because the transform was declined both windows, the R4 ACTIVATION path never resolved.

- **ENGINE-R4 (Azcanta ACTIVATION path) — STILL UNTESTED.** No Azcanta activation occurred (transform
  declined both windows, see ENGINE-R6). The to-hand slot eligibility (`-land;-creature`) and the
  "is an R1-style all-sweep in the activation optiontwo?" question remain unverified, not vindicated by
  absence. VALIDATION TODO carried: an Azcanta activation where the model picks an eligible
  noncreature-nonland; confirm it reaches hand and the rest bottom. (Fixing ENGINE-R6 makes this window
  reachable — the model keeps declining the transform that would create it.)

- **ENGINE-R2/R3 (non-contiguous ordering; Into-the-North label) — carried, unchanged.** Into the North
  reveals resolved outcome-correct where they fired; no ordering fault. The cosmetic whole-library
  "choose card / put back" label residual persists but is non-actionable (eligibility marks carry it).

- **Reveal eligibility surfacing — still FIRING.** `ELIGIBILITY:` header + `[eligible for ...]` /
  `does NOT qualify` marks present across reveal prompts corpus-wide (grep-confirmed). The Glacial
  Revelation at vs14 s23 lacked per-card marks in its numbered list but all six were eligible so it was
  immaterial; worth a spot-check next corpus that the marks render on Glacial Revelation option lists too.

- **ENGINE #2 (stale `[attacking]` tag) — carried, benign.** Not re-audited (attention on the fetch bug);
  no wrong decision traced to a board tag.

## HARNESS

- **HARNESS-1 (deck135-OWNED, wave-21 OVER-CORRECTION) — the retracted_choice route FALSE-POSITIVES on a
  retraction-shaped token that does NOT retract the CHOICE.** vs27 s26 (ask, turn 10, 8/20, 13417 chars,
  `fallback: retracted_choice`, `choice: -1`): the reply has EXACTLY ONE `CHOICE:` line —
  `CHOICE: 2 (Cast Ice-Fang Coatl)` on line 1, never retracted; the reply reaffirms casting Coatl ~a
  dozen times. The only retraction-class token is one `Correction:` about ISLANDWALK MECHANICS. The
  parser saw the token + no second CHOICE line and routed to heuristic. WRONG — the choice should have
  been honored. Harmless this time only because the heuristic independently cast Coatl (s27 attacks with
  it). This is the INVERSE of wave-20 HARNESS-1 (which LOCKED IN a truly-retracted choice); the wave-21
  fix over-swung to DISCARDING a not-retracted choice. FIX: only fire retracted_choice when the
  retraction targets the CHOICE/decision (or a later CHOICE supersedes); HONOR a single un-superseded
  CHOICE line even when `Correction:`/`Wait` tokens appear downstream reconsidering combat/mechanics.
  DETECT: a clean single-CHOICE record classified retracted_choice where the retraction token is not
  adjacent to / not referencing the CHOICE line.

- **HARNESS-3 (CARRIED) — decode-time REPEAT-LOOP tax.** The underlying disease of the HARNESS-1 record
  (vs27 s26) and the Azcanta declines (vs14 s29, 15k) is a >12k-char verbatim-repetition spiral. Answer-
  first (item b) fixed the PARSING symptom (0 unparsed_reply this seat vs 7 in wave-20 — the coded line
  is now first, so truncation leaves a parseable line), but the SPIRAL itself persists and still degrades
  decisions (the Azcanta miscount happened inside a 15k spiral). Route to a decode-time repetition-
  penalty / max-token guard (repetition_penalty was OFF this corpus per brief item j). Model/decode item,
  NOT guide-fixable.

## MODEL (route to model-experiments; NOT guide text) — see general-suggestions.
- Decode repeat-loop spirals still dominant (HARNESS-3); now they surface as retracted_choice fallbacks
  and hidden-inside-good-parses (Azcanta miscount) rather than unparsed_reply.
- Manual graveyard-recount-from-event-log is error-prone (ENGINE-R6). The model over-trusts its own count
  over the engine's offer. Representation fix (annotate the offer) is the lever, not more prompt text.
- Cannot perceive a silent fetch fizzle (ENGINE-F1b) — needs an engine event on aborted activations.

## DECK-CONSTRUCTION FLAGS (route upstream; user owns the decklist — restate, don't relitigate)
- Fetch-color dependence COMPOUNDS with ENGINE-F1: a deck that must fetch to function is maximally
  exposed to a fetch-resolution bug. Engine fix > decklist change here.
- Under-powered small-creature clock — STANDS (7th corpus); Treefolk/Diamond Faerie the only closers.
- Thin blue base + Windswept Heath can't fetch Island — STANDS.

## VALIDATION TODO (for the next run)
1. ENGINE-F1: confirm the fetchland-fizzle fix (verify Sacrifice-cost click lands / roll back partial
   cost). Detector: a fetch CHOICE followed by a life payment with NO fetchland `-> graveyard` and NO
   `Snow-Covered X: -> battlefield`.
2. ENGINE-R6: confirm the Azcanta Transform option carries an eligibility annotation and the model stops
   declining an available transform.
3. ENGINE-R4 (Azcanta activation): UNTESTED again — needs a resolved transform first (ENGINE-R6) to
   create the window; then confirm the to-hand slot restricts to noncreature-nonland with no all-sweep.
4. HARNESS-1: confirm the retracted_choice route no longer false-positives on a downstream `Correction:`/
   `Wait` token when a single un-superseded CHOICE line exists.
5. ENGINE-R1: CLOSED (n=2 confirmed) — regression-watch only.
6. ENGINE-1d (Force pitch naming): Force cast once off-whitelist in a dead game (vs49 s28); no board-wipe
   window — re-verify when a Force fires on a real target.

## PER-DECK ROTATION VERDICT: **KEEP deck135 IN — NOT a rotation candidate this cycle (exit conditions NOT met; abundant new engine work).**

Rotation retires a veteran whose guide takes NO modification AND whose seat surfaces NO new work signal.
The guide took no modification (FROZEN, byte-identical, `cmp`-verified) — the first condition holds. But
the second condition FAILS hard this wave:
- My wave-20 EXIT CONDITIONS were: (i) ENGINE-R1 confirmed on a second corpus AND (ii) an Azcanta
  activation observed resolving correctly. (i) is MET (n=2, CLOSE ENGINE-R1). (ii) is NOT met — an Azcanta
  TRANSFORM window occurred but the model declined it twice, so the ACTIVATION never resolved. Exit
  condition unsatisfied.
- This seat surfaced a HIGH-VALUE NEW engine bug (ENGINE-F1 fetchland fizzle — the corpus's top new
  engine finding and the primary driver of the record regression), a NEW representation gap (ENGINE-R6
  Azcanta transform), a NEW harness over-correction (HARNESS-1 retracted_choice false-positive), and it
  is the ONLY seat that exercises the reveal / fetch-heavy / Azcanta seams. Rotating it out orphans the
  fetchland-fizzle repro, the Azcanta-activation validation, and the R6/HARNESS-1 confirmations.
- Therefore deck135 is FAR from a rotation exemplar this wave — the exact opposite: a frozen guide but a
  seat generating the most consequential engine findings in the corpus. KEEP IN. New exit condition
  (unchanged in spirit): once ENGINE-F1 is fixed+validated, ENGINE-R6 is fixed and an Azcanta activation
  finally resolves correctly, and HARNESS-1 stops false-positiving, the reveal/fetch/Azcanta charge
  closes and a still-frozen deck135 becomes the textbook rotation candidate.
