# Wave-20 synthesis notes — SIXTH per-deck-rotation cycle

Unified from seven independent deck-agent reviews mining the wave-20 PRIMARY corpus
`matchups-20260719-151849` (21 games round-robin over 135/62/49/35/14/27/102, binary `/tmp/wagic-95e89c204`).
deck110 rotated OUT last cycle (third converged exemplar; guide stays deployed as a canary). **deck102 (Tergrid)**
entered GUIDELESS → initial guide; **deck27 (Zombies)** played its FIRST GUIDED corpus → surgical revision; **deck14**
round 2 and **deck62** round 4 of their revised guides; **deck49's rotation HOLD discharged** (off-case exercised,
passed) → now a revised seat; **decks 135/35 FROZE**. Nobody rotates OUT this wave.

Corpus health: fallbacks **3.0% (32/1067)** — 29 unparsed, 3 stale_echo, 0 defer, 0 empty; 7th consecutive clean
corpus, 0 segfaults, 0 timeouts, 0 draws, 5 life-adjudicated at cap. Win table 135:4/6, 49:4/6, 35:3/6, 27:3/6,
14:3/6, 62:3/6, 102:1/6. Per-seat fallbacks: 135:7, 62:7, 102:6, 14:6, 27:3, 49:3, **35:0 (the zero seat)**.

**Core (general prompt): PASS, 15th consecutive.** The one 2-seat method convergence (co-firing-imperative fork,
deck49+deck62) is a SKILL method rung, not a general-prompt line; the spirals it touches stay at the GUIDE off-case +
decode-sampler layers (do not double-layer). Skill method sections gained rungs (Step 5 co-firing bullet; Step 0-bis
wave-20 additions) — guide-authoring method, which does not reset the core PASS count.

## Per-seat verdict table

| deck | archetype / status | record (Δ) | fallbacks | guide disposition | rotation |
|------|--------------------|-----------|-----------|-------------------|----------|
| 135 | Modern Snow · frozen veteran | 4/6 (↑ from 2/6) | 7 (all decode spirals in the 2 losses) | **FROZEN** (byte-identical, `cmp`-verified) | KEEP one more (R1 n=1 + only reveal seat) |
| 62 | Enchantresses · revised round 4 | 3/6 (↑ from 2/6) | 7 | **REVISE** — edit D, +5 lines (scope-sharpen edit B) | STAY (guide-mod + N8) |
| 49 | Dragons · hold discharged | 4/6 (↓ from 5/6) | 3 (streak broke) | **REVISE** — two hunks (key-blocker exception + develop-seam tiebreak) | STAY (hold ended, revised seat) |
| 35 | Mighty Slivers · revised round 3 | 3/6 (flat) | **0** | **FROZEN** (byte-identical) | KEEP (attackers-non-issuance 3-wave signal) |
| 14 | Deep Blue · revised round 2 | 3/6 (↑ from 2/6) | 6 | **REVISE** — one bullet hoisted (+8 lines, bounce-on-stack trap) | STAY (turnaround success + E1/E2) |
| 27 | Zombies · FIRST GUIDED | 3/6 (↑ from 2/6 guideless) | 3 | **REVISE** — one insertion (+5-6 lines, late-stall durdle) | STAY (guide-mod + wither engine item) |
| 102 | Tergrid · GUIDELESS newcomer | 1/6 | 6 | **NEW GUIDE** (initial) | STAY (newcomer, never a first-pass candidate) |

## Validation-focus outcomes (the wave-20 engine batch — brief items)

- **ENGINE-R1 reveal-partition fix — VALIDATED / CLOSED at the decision surface (cross-seat confirmed).** deck135
  vs27 s18 (the corpus's ONLY Glacial Revelation): all four chosen snow permanents → HAND, the two unchosen non-snow
  fetches → graveyard, exactly the Oracle partition (wave-19 was 3/3 all-to-graveyard). deck102 CROSS-CONFIRMS the
  same same-tick finalize on 3 Thoughtseize records (lowest-index eligible pick leaves the hand exactly once). **CLOSE
  with a residual:** deck135's Glacial-Revelation proof is n=1 (draw variance — 1 cast all corpus); hold open for a
  second-corpus confirmation. The Azcanta ACTIVATION path had 0 windows — UNTESTED, not vindicated.
- **Attacker-sparing mana payment — VALIDATED WORKING, but it REFUTES the brief's durdle attribution (deck35).** The
  fix fires (vs49 seq8: Winged paid with two lands, Gemhide left untapped; corpus-wide no Sliver tapped-for-mana with
  lands available). But it did NOT collapse the 0-attacker games — the would-be attackers were UNTAPPED, so payment was
  never the cause. The 0-attacker root is the attackers-decision NON-ISSUANCE bug (below), 3-wave convergent, UNFIXED.
  Do not close the durdle on the payment fix.
- **Reveal ELIGIBILITY surfacing — SHIPPED + FIRED (deck135).** `[eligible]` / `[does NOT qualify]` marks killed the
  Into the North reveal-spiral tax (wave-19 ~12k-char spirals + 2 fallbacks → wave-20 short clean picks, 0 fallbacks,
  3/3 outcome-correct). Verified by reasoning-tax collapse, not a marker grep.
- **Combat `options_text` — CLOSED (deck14 40/40, deck62 confirmed at all combat records).** The wave-19 deck14
  observability gap is fixed; combat-decision review is now possible at those seats.
- **`salvageLoopedChoice` BLOCKS/ATTACK/PUT extension — validated NEGATIVE-CORRECT (deck62, deck14, deck135, deck27,
  deck49).** No positive fire anywhere: every combat/reveal fallback this corpus truncated BEFORE a coded line
  (deck14+deck62 blocker fallbacks truncated before any `BLOCKS:` — salvage correctly can't fire, a clean negative);
  deck135 0/7 (spirals never emit a clean trailing line). The extension is not invalidated (orthogonal to this failure
  mode); the exposed gap is the truncated-before-line class, which needs answer-first / prose-intent, not salvage.
- **Discard-pick reveals — mechanics SOUND, prompt MISLEADS, quality OK (deck102, owned surface).** 3/3 Thoughtseize
  picks landed mechanically (named card left the hand once); pick quality acceptable-to-optimal (vs27 stripped the
  Glen Elendra Liege anthem-lord, optimal). Defect is representation: the prompt calls the opponent's hand "top N of
  your library" and uses subset framing on a pick-ONE → over-selection (harmless only because the true best was the
  lowest index all 3 times). → engine ledger #2.

## Cross-seat reconciliations

- **`#N` ordinal disambiguation — PARSE-side feature, NOT a render-side one; the three seat reports do NOT conflict
  (settled in `src/AIPlayerGPT.cpp`).** The engine (`nameOrdinal` + `uniqueNameMatch(..., ordinal)`, lines
  ~3020/3046/3254/3484) HONORS a `#N` the MODEL appends to a reply segment to break a same-name tie in label order.
  Every render site emits only `(P/T)` and combat A#/B# position codes (lines 317/441/2319/3311-3603/3839) — the
  engine NEVER renders `#N` onto an option menu. Therefore:
  - **deck27 (vs49 s19) and deck62 (vs14 s36) "worked" on COMBAT surfaces** where the A#/B# position codes
    (deck27's two "Lord of Atlantis" + two "Zombie Outlander" attackers) and bare-name→multi-target name-tolerance
    mapping (deck62's two distinct Yavimayas, "no `#N` ordinal even needed") did the disambiguation. **Neither is a
    positive test of the `#N` path.**
  - **deck14's "no `#N` on any TARGET menu" is CORRECT and identifies the real gap.** Non-combat duplicate-name target
    menus (Control-Magic "Yavimaya Enchantress, Yavimaya Enchantress"; Persuasion "Gemhide Sliver, Gemhide Sliver")
    carry no position code and no rendered `#N` — only per-line `(P/T)`, which separates DIFFERENT-stat duplicates but
    NOT two IDENTICAL same-name/same-stat creatures in different states.
  - **Verdict: two different render surfaces, no contradiction. Combat disambiguation works (position codes +
    name-tolerance). The parse-side `#N` is REAL but UNEXERCISED end-to-end** — no seat this corpus forced a selection
    between two truly-identical same-name targets, and nothing renders `#N` to prompt the model to volunteer it.
    RESIDUAL (engine ledger, LOW): either render a distinguishing marker (`#N` or the differing state) onto non-combat
    duplicate-name target menus, or confirm the model volunteers `#N` there; validate on a real forced
    identical-duplicate target selection (needs a seat that gets one — none this corpus).
- **Truncation-above-salvage class — ONE cross-seat ledger item, not seven (14/62/135/35/49/102).** All six report
  >12-13k-char decode spirals: deck135's 7 fallbacks (5 in the lost vs27, 2 in vs49) + 1 hidden (vs35 s10); deck102's
  6 (all >13k behind/stalled, prison-board amplified); deck62's 5 advancing-spiral/over-deliberation; deck49's 3
  (streak-breaking, one repetition-loop + one progressing + one plan/menu mismatch); deck14's 3 truncated-before-line;
  deck35's 14k near-misses that still parsed (0 fallbacks but FRAGILE). deck14+deck62's blocker fallbacks truncated
  BEFORE any `BLOCKS:` line (salvage correctly can't fire — clean negative). Converging engine ask: a decode-time
  repetition-penalty + max-token guard + an ANSWER-FIRST reply protocol (emit the coded line, then reason). One ledger
  item (#5), not seven per-seat findings.
- **Perception-bundle verdicts:** `options_text` CLOSED (deck14 40/40, deck62 confirmed); eligibility surfacing fired
  at deck135 (killed the reveal spirals); attacker-sparing VALIDATED at deck35 but REFUTES the durdle attribution
  (non-issuance is its own bug, still open).

## Wave-21 engine ledger (ranked; seat notes.md carry the seqs)

1. **Attackers-decision NON-ISSUANCE (deck35, HIGHEST — 3-wave convergent, ready for targeted fix).** The declare-
   attackers decision is not issued to the GPT seat on some own-turns despite eligible untapped/unsick attackers.
   vs49 = a COMPLETE 0-attacker game (jq count = 0; opp life never moved off 20; the result-determining LOSS); vs14
   T3/T5/T7 and vs62 T6/T8 = skipped windows (won anyway, slow matchups). Sharpened correlate (hand to engine, NOT
   proven): reliably ISSUED with a HASTE creature present OR after an explicit "Cast nothing"; SKIPPED when the model
   casts its LAST affordable spell (taps out / auto-ends main-1) with all attackers non-haste. Where to look: the
   main-1→combat phase transition for AIPlayerGPT — whether reaching end-of-castable auto-advances past the
   declare-attackers offer when the last resolved action leaves no attack-legal creature (missing pre-existing unsick
   bodies). The payment fix does NOT touch this.
2. **Thoughtseize / targeted-discard reveal MISREPRESENTATION (deck102 E1, HIGH, cross-deck).** The reveal record
   frames the OPPONENT's revealed HAND as "top N cards of your LIBRARY" (false) and uses choose-a-SUBSET framing on a
   pick-EXACTLY-ONE effect (`auto=... target(<1>*[-land]|reveal)`), driving over-selection (2 of 3 records). Engine
   correctly discards only the first eligible pick (mechanical integrity clean), so currently harmless — LATENT
   quality bug when the true #1 is not the lowest index. Fix: render as a pick-ONE prompt sourced from the opponent's
   hand. Generalizes to Duress/Distress/Inquisition.
3. **Wither trade-annotation "(both die)" WRONG when the wither blocker's power < attacker toughness (deck27, VERIFY→FIX).**
   Oona's Gatewarden (2/1 wither) only KILLS an attacker with toughness ≤ 2 (or via deathtouch); the annotator prints
   "(both die)" regardless (vs62 s6 Birds 3/4 survives as 1/2 — the mis-annotation directly CAUSED the s6 fallback;
   vs135 s13 Treefolk 8/8→6/6; vs102 s18 Liege 1/3→0/1). Fix: "both die" requires `blockerPower >= attackerToughness`
   (post-existing-damage) OR deathtouch; else "your blocker dies, attacker lives (wither shrinks it to X/Y)."
4. **Bounce-on-stack menu misconception (deck14 E1, HIGH — 1 costly misplay + 3 fallbacks).** The model casts
   Unsummon/Boomerang to "answer" a spell on the stack, then self-bounces its own creature at the forced
   battlefield-only target menu (vs62 s18 bounced its own Lord of Atlantis) or thrashes to truncation (vs35 s27 /
   vs27 s11 / vs102 s9). Fix: don't offer the bounce as a "respond now" answer whose only conceivable target is an
   illegal stack card, or annotate the CAST option "[cannot target the spell on the stack]"; prepend a target-menu
   header "battlefield permanents only." Interim guide trap shipped (deck14 revision).
5. **Decode / truncation guard (cross-seat class, HIGH by frequency — the whole truncation-above-salvage class).**
   Decode-time repetition-penalty + max-token guard + ANSWER-FIRST reply protocol (emit the coded line, then reason)
   + a PROSE-INTENT salvage for truncated-before-line replies. Owns the dominant fallback shape at 135/102/62/49/14
   and the fragile 14k near-misses at 35. NOT guide-fixable; the guide can only lower the TRIGGER frequency.
6. **Parser: retracted / duplicate-CHOICE lock-in (deck135 HARNESS-1 + deck62 N7-template, same code area).** Prefer
   the LAST well-formed `CHOICE: N (Name)`; treat a CHOICE preceding a "Wait, I made a mistake"/self-retraction as
   superseded (deck135 vs35 s10 logged the retracted `CHOICE: 4` → wrong action, a HIDDEN loop the fallback count
   misses); drop literal template-placeholder lines (`CHOICE: [Number] ([Name])`) before disambiguation (deck62 vs49
   s18 unparsed with a valid choice present).
7. **Baka blocker-fallback chumps engine pieces (deck62 N8, MEDIUM, cross-seat).** When a block reply unparses →
   heuristic, the baka fallback gang-blocks a small attacker with the 0/1 shroud Argothian engine even when a lone
   real blocker suffices (vs14 s26, vs135 s19 — Argothian lost for nothing both times). The heuristic should prefer
   non-0/1/non-shroud/non-engine blockers and not add a second body to an already-dead attacker. Any deck with a
   valuable 0-power creature hits this. (This is the behavior deck62's edit B forbids, but it lives below the model's
   answer where guide prose cannot reach.)
8. **Repeated cast-offer of an already-cast SINGLETON (deck102 E2, LOW, needs a probe).** vs135 "Cast Witch of the
   Moors" chosen 3× (s20/s21/s24) with one battlefield event; vs62 "Cast Dreadhorde Invasion" 3×. Either the cast is
   not resolving and the option is re-offered, or a stale cast option persists. Needs an engine probe (opponent-seat
   cross-read or a pinned marked-singleton probe deck). Distinct from the known stale-plan class.

CLOSED / near-closed: ENGINE-R1 reveal-finalize (deck135 + deck102 cross-confirmed, n=1 residual open); combat
`options_text` (deck14 + deck62); attacker-sparing PAYMENT fix (deck35, validated working — but does NOT close #1).

## Rotation decisions

Nobody rotates OUT this wave. Per the rotation test (no-guide-mod AND no-new-work-signal = candidate):
- **135 KEEP one more** — guide frozen (no mod), but new-work signal survives: the R1 validation is n=1, the Azcanta
  activation path is UNTESTED, deck135 is the ONLY seat that exercises the reveal seam, and it produced HARNESS-1
  (retracted-first-CHOICE hidden loop). Approaching rotation: once R1 is confirmed on a second corpus AND an Azcanta
  activation resolves correctly, a still-frozen deck135 becomes the textbook converged exemplar.
- **49 hold ENDS, stays as a revised seat** — the hold's sole purpose (exercise the flood off-case) is DISCHARGED
  (deck27 was the pure off-case game; behavior PASSED — all burn to face, loss was construction). But the corpus
  produced a guide-mod (two hunks) + new-work (E-49d/E-49e), so it fails both rotation gates: re-enters for one
  validation cycle.
- **62 / 14 / 27 STAY** — all revised (guide-mod present) with durable new-work signals (62: N8 blocker-sac + edit D
  to validate; 14: E1 bounce-on-stack + E2 truncation-before-line; 27: wither annotation + the late-stall revision to
  test in a grind matchup).
- **35 STAYS** — guide FROZEN, but strong live new-work: attackers-non-issuance reproduced a 3rd wave AND this seat
  produced the refutation of the payment attribution (the corpus's most important engine finding).
- **102 STAYS** — guideless newcomer on its first pass; per doctrine never a first-pass rotation candidate, and it
  produced a full new initial guide (new-work by construction).

**Standing rule restated:** rotation candidates emerge only at no-guide-mod AND no-new-work-signal. With nobody
rotating out, **the wave-21 pool is unchanged: 135, 62, 49, 35, 14, 27, 102** — no new deck enters unless the owner's
roster supplies one. Pool composition next wave is therefore identical to this wave (all seven repeat), which keeps a
clean 7-of-7 repeated-opponent control for attribution.

## Next-wave (wave-21) pool and focus

Pool: **135, 62, 49, 35, 14, 27, 102** (unchanged). Per-seat wave-21 exit questions:
- 135: confirm ENGINE-R1 on a second Glacial Revelation; observe an Azcanta activation resolving correctly (retires
  the reveal-seam charge → rotation candidate).
- 49: did the develop-seam fallbacks (seq-8 class) DIE with the two edits (target: 0 unparsed at the early Main-1
  develop seam)? Did burn-face rate hold without the key-blocker tightening over-generalizing into "never kill any
  blocker"? Did "either is fine, commit" avoid a blind creature-cast when burn was lethal?
- 62: did edit D shorten/prevent the favorable-block litigation? Was N8 fixed (engine stops sacrificing the 0/1)? Is
  the over-deliberation-of-a-settled-decision unparse persistent?
- 14: did the bounce-on-stack trap drop the misfire count? Did E1/E2 get an engine/representation fix?
- 27: re-validate the late-stall durdle revision in a GRIND matchup (the winning corpus never engaged it); did the
  wither annotation get fixed?
- 35: did an actual non-issuance fix land and the vs49-class 0-attacker games disappear? Did the fragile 14k decode
  near-misses convert into real fallbacks?
- 102: re-validate the initial guide (Step 0-ter) — does the symmetric-trap fire-gate stop the Pox/Smallpox self-harm?
  does the deploy/attack floor move opponent life? does the Thoughtseize pick-ONE line stop over-selection? does any
  Tergrid-drawing game exercise the steal engine? E1 is the priority engine item to work between waves.
