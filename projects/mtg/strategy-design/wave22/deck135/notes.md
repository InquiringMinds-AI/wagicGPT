# Deck-135 wave-22 — development notes (engine / harness / model) + ROTATION VERDICT

Run: PRIMARY `matchups-20260723-173843`. Binary `/tmp/wagic-1d8ed8194` (wave-22 step-1 batch —
THIS corpus is its first live validation). deck135 owns: ENGINE-F1 fetch validation, R1 reveal-
partition regression watch, Azcanta transform-annotation + activation window, the 1 stale_echo
fallback, the 1/6->4/6 attribution. Layer-routing enforced: engine/harness items live HERE with
seq repros, NEVER in guide text. Game->file map in findings.md.

## ENGINE LEDGER

- **ENGINE-F1 (deck135-OWNED, was the corpus's top wave-21 engine finding) — FETCHLAND-FIZZLE FIX
  CONFIRMED. CLOSE.**
  - Detector run on EVERY fetch-crack CHOICE across all 6 games: 10 cracks, 10 clean resolutions,
    0 fizzles. Each shows `Your life -1` + `<fetch>: battlefield -> graveyard` + `<land>: library
    -> battlefield` in the next record's events. Broad negative-sweep (crack whose next events lack
    graveyard OR land-entry) returned ZERO hits.
  - THE FIXED CASE (crack after same-turn land drop = the 2016 guard's exact trigger) is directly
    confirmed on TWO cracks whose fetchland was played from hand THAT turn then cracked:
    - vs27 s3: `Misty Rainforest: hand -> battlefield` -> crack -> s4 `life -1` + `Misty Rainforest:
      battlefield -> graveyard` + `Snow-Covered Forest: library -> battlefield`.
    - vs140 s3: SAME shape as the wave-21 vs35 s6 repro (Windswept Heath from hand -> crack same
      turn) — resolved cleanly (`Windswept Heath: battlefield -> graveyard` + `Snow-Covered Forest:
      library -> battlefield`).
  - No re-arm loop possible post-fix (a resolved fetch's fetchland is in the graveyard, so the crack
    option cannot re-list). ENGINE-F1b (silent fizzle the model can't perceive) did not arise —
    no fizzles to perceive. CLOSE ENGINE-F1; regression-watch only next corpus.

- **ENGINE-R1 (reveal partition) — THIRD confirmation. Regression-watch NEGATIVE. Stays CLOSED.**
  - 3 true Glacial Revelation reveals (vs102 s16, vs44 s32, vs62 s23) all partitioned snow->hand,
    non-snow->graveyard correctly (mixed-eligibility on all three). Zero all-to-graveyard.
    n now = 3 across three independent corpora. Regression detector NEGATIVE.

- **ENGINE-R6 (Azcanta transform representation) — FIXED by shipped annotation (d). CLOSE.**
  - Annotation present verbatim: `Transform:azcanta, the sunken ruin [available NOW - this transform
    is only offered because its condition is already met; do not recount, it is legal this instant]`.
  - 3/3 transform windows transformed (vs140 s24 clean accept citing the annotation; vs44 s36 clean
    accept; vs27 s27 intended-accept, fell to a naming echo fallback but heuristic completed it).
    Wave-21's 0/2 decline behavior is GONE. The "manual graveyard recount vacuum" that caused the
    declines is closed — the model now trusts the offer. CLOSE ENGINE-R6.

- **ENGINE-R4 (Azcanta ACTIVATION path) — REACHABLE + OFFERED for the first time in 4 waves, but the
  model-elected top-4 to-hand resolution is STILL UNOBSERVED.** After transform, `look top 4 cards
  with Azcanta, the Sunken Ruin [cost: {2}{u}, Tap]` was offered at 6 priority windows (vs27 s28/s31,
  vs140 s25/s28, vs44 s37/s40). Model PASSED all 6 — passes are largely correct (vs27 going for
  lethal; vs44 at 4 life casting Treefolk first; vs140 mana-gated/marginal). The Azcanta card-selects
  that DID resolve in events were the top-**1** Search-for-Azcanta UPKEEP looks (cosmetically labeled
  with the post-transform name), which sent revealed creatures to graveyard = eligibility direction
  correct. The top-4 {2}{U},{T} to-hand slot (noncreature-nonland restriction; any all-sweep?) was
  NOT exercised through a model pick. VALIDATION TODO carried: an Azcanta activation where the model
  ELECTS the top-4 look and picks an eligible noncreature-nonland; confirm it reaches hand and the
  rest bottom to graveyard. NOTE: this may be structurally hard to observe — an aggro-tempo deck
  rationally deprioritizes a {2}{U} card-draw activation for developing/attacking, so the window may
  rarely be taken. Consider whether R4 is best validated by a targeted probe rather than waiting on
  organic selfplay.

- **R-COSMETIC (LOW) — the post-transform name bleeds onto the Search-for-Azcanta upkeep-trigger
  resolution.** Events show `Choose an option for Azcanta, the Sunken Ruin: -> put in graveyard` on
  a top-1 look that is actually the front-face Search-for-Azcanta upkeep. Non-actionable labeling
  quirk (the permanent has transformed, so its current name attaches); creature partition still
  correct. Note-only.

- **Reveal eligibility surfacing / R2-R3 — carried, non-actionable.** 16 of 20 reveals are library-
  search / Scrying-Sheets "to choose card" reveals; all cards `goes to library` (search-and-shuffle),
  no ordering fault observed. The whole-library "choose card / put back" label residual persists but
  is cosmetic.

## HARNESS

- **HARNESS-ECHO-Transform (NEW, deck135-OWNED) — the transform-option DUAL-NAME echo mismatch trips
  stale_echo on a correct Transform pick.** vs27 s27 (ask, T12, 11/6, `stale_echo`, `choice:-1`):
  option = `Transform:azcanta, the sunken ruin [available NOW...]`; reply line 1 = `CHOICE: 1
  (Transform: Search for Azcanta)`. Correct INDEX (1=Transform), pro-transform reasoning, but the
  echoed LABEL names the PRE-transform source card ("Search for Azcanta") while the option string
  names the POST-transform destination face ("azcanta, the sunken ruin"). The echo classifier saw
  the label mismatch and routed to heuristic. This is a NEW shape the shipped (c) fixes do not cover
  (they target "<spell> targeting <target>" source-prefix and combat-line label-hijack). Harmless
  this instance (right index; heuristic transformed; game won), but a correct pick was discarded on
  a naming technicality — and it is a SIDE-EFFECT of the (d) annotation succeeding (confident accepts
  now expose the transform-face echo gap). FIX: for a "Transform:" option, accept an echo matching
  EITHER the source card name OR the destination face name (or validate on the "Transform:" prefix +
  the numeric index rather than the trailing face name). DETECT next corpus: a Transform CHOICE with
  a correct index classified stale_echo where the echo names the other face.

- **HARNESS-3 (CARRIED) — decode-time repeat-loop tax.** Longest replies remain 12k-15k chars
  (vs140 s24 transform reasoning = 14.5k; vs44/vs109 ~15k). Answer-first keeps them parseable (0
  unparsed this seat), and this corpus they all landed on the right answer (no Azcanta miscount, no
  bad decision inside a spiral — the (d) annotation removed the miscount-spiral that hurt wave-21).
  Underlying spiral persists; decode-time repetition-penalty / max-token guard still the real fix.
  Model/decode item, NOT guide-fixable.

## MODEL (route to model-experiments; NOT guide text)
- The (d) transform annotation eliminated the wave-21 manual-graveyard-recount misplay class — the
  model now trusts the engine's offer instead of filling the vacuum with an error-prone count.
  Confirms the wave-21 thesis (annotate the offer > add prompt text).
- Residual: Scrying-Sheets single-card reveals still draw library-vs-graveyard-vs-hand confusion in
  the reasoning (vs44 s38), though the chosen action was fine. Representation nit; the reveal prompt
  wording could name the destination zones more explicitly.
- Decode repeat-loop spirals still dominant (HARNESS-3) but now benign at this seat (parse-clean +
  correct outcomes).

## DECK-CONSTRUCTION FLAGS (route upstream; user owns the decklist — restate, don't relitigate)
- Fetch-color dependence: the ENGINE-F1-compounding risk is RESOLVED (fetches 100% reliable). Flag
  downgrades to ordinary color-screw variance.
- Under-powered small-creature clock — STANDS but softened (Treefolk closed all 3 wins).
- Thin blue base + Windswept Heath can't fetch Island — STANDS (unchanged construction fact).

## VALIDATION TODO (for the next run)
1. ENGINE-F1: regression-watch only (CLOSED, 10/10 clean incl. same-turn-land-drop repro).
2. ENGINE-R1: regression-watch only (CLOSED, n=3).
3. ENGINE-R6: CLOSED (transform annotation validated, 3/3 transformed).
4. ENGINE-R4 (Azcanta activation to-hand pick): STILL UNOBSERVED via model choice — the activation
   is now reachable/offered but the model rationally passes. Consider a targeted probe rather than
   organic selfplay; if organic, watch a grind where the model has {2}{U} up and no better creature
   play.
5. HARNESS-ECHO-Transform (NEW): confirm the transform echo mismatch is fixed (accept source- OR
   destination-face echo) and no longer stale_echoes a correct Transform index.

## PER-DECK ROTATION VERDICT: **ROTATION CANDIDATE — the multi-wave charge is CLEARED. Recommend rotating deck135 OUT (guide stays deployed as a canary), with the ONE fresh harness item handed off.**

The rotation test: retire a veteran whose guide takes NO modification AND whose owned charge is
resolved. This wave both largely hold:
- **Guide: FROZEN, byte-identical, `cmp`-verified** — zero decision-level leak; the two losses are
  matchup/draw (adjudicated grind vs 140; raced by 44). First condition MET.
- **Owned charge: SUBSTANTIALLY CLEARED.** My standing exit conditions were (i) R1 double-confirmed,
  (ii) fetch-fix confirmed, (iii) Azcanta activation observed. Scorecard:
  - (i) R1: MET and then some — now n=3, regression-clean.
  - (ii) ENGINE-F1 fetch fix: MET decisively — 10/10 clean incl. the exact same-turn-land-drop repro.
  - (iii) Azcanta: the multi-wave TRANSFORM block is BROKEN (annotation (d) flips declines to
    accepts, 3/3 transformed) and the activation is now REACHABLE and OFFERED. The only unmet
    sliver is a model-ELECTED top-4 to-hand pick, which the model rationally deprioritizes — this
    may be structurally hard to capture in aggro-tempo selfplay and is better served by a targeted
    probe than by holding this seat open indefinitely.
- Honest caveat against a PURE no-new-work verdict: this seat DID surface one fresh item —
  HARNESS-ECHO-Transform (the transform dual-name echo mismatch), itself a side-effect of the (d)
  fix landing. But it is a small, well-localized harness fix that does not require THIS seat to
  re-derive; it can be handed to engine/harness devs and confirmed at any seat that transforms.

Net: deck135 has gone from "a frozen guide generating the corpus's most consequential engine
findings" (wave-21, KEEP IN) to "a frozen guide whose entire owned charge is resolved bar one
rationally-skipped activation pick" (wave-22). That is the textbook rotation profile. **RECOMMEND
ROTATE OUT** — deploy the guide as a canary, hand off HARNESS-ECHO-Transform + the ENGINE-R4
targeted-probe TODO, and reclaim the seat. If synthesis prefers one more cycle, the sole reason to
KEEP IN is to try to catch an organic ENGINE-R4 activation pick — a low-probability event I judge
not worth a full seat.
