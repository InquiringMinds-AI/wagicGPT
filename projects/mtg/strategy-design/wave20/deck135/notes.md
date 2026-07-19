# Deck-135 wave-20 — development notes (engine / harness / model) + ROTATION VERDICT

Run: PRIMARY `matchups-20260719-151849`. Binary `/tmp/wagic-95e89c204` (wave-20 batch: **ENGINE-R1 reveal
partition fix** = driveInteractiveReveal finalizes optionone picks same-tick; attacker-sparing mana payment;
perception bundle = reveal ELIGIBILITY surfacing + combat options_text + #N ordinals + salvage extension;
files[1024] registry-overflow fix). deck135 owns the REVEAL PARTITION CHECK; its 5 reveal records are the live
validation. Doctrine: win column context-only; layer-routing enforced (engine/harness items live HERE with
seq repros, never in guide text). Game->file map in findings.md.

## ENGINE LEDGER

- **ENGINE-R1 (wave-19 deck135-OWNED HIGH-VALUE bug) — FIXED / VALIDATED at the decision surface. CLOSE (with a
  thin-sample validation TODO).**
  - Wave-19: Glacial Revelation's interactive reveal dropped the entire "to-hand" partition — 3/3 parsed
    reveals milled the model's chosen snow permanents to graveyard (`all(*|reveal)` optiontwo overwrote the
    optionone `<anyamount> moveto(hand)` picks). Game-affecting (worsened vs49 + vs110 adj losses).
  - Wave-20 REPRO of the fix: **vs27 s18** (turn 8, 11/19), the corpus's ONLY Glacial Revelation cast.
    chosen `PUT: 1,2,4,5` = Ohran Viper, Ohran Viper, Abominable Treefolk, Ice-Fang Coatl. Events (seq 19):
    `You revealed 6 and put Ohran Viper, Ohran Viper, Abominable Treefolk, Ice-Fang Coatl to get snow` then all
    four `goes to hand` and the two unchosen non-snow fetches (Misty Rainforest, Flooded Strand) `goes to
    graveyard`. **Exactly the Oracle partition.** The wave-19 all-to-graveyard behavior is GONE.
  - STATUS: the same-tick finalize fix WORKS. n=1 (one cast all corpus — draw variance), decisive + clean +
    code-backed. VALIDATION TODO: confirm on a second corpus with a Glacial Revelation (regression detector:
    a Glacial Revelation reveal whose "get snow" picks appear in the subsequent `goes to graveyard` list).

- **ENGINE-R3/R4 (wave-19 deck135-OWNED REPRESENTATION) — eligibility-marking half SHIPPED + FIRED. CLOSE the
  marking; two low residuals.**
  - The perception bundle now writes per-card eligibility onto reveal prompts: `[eligible for "..."]` /
    `[does NOT qualify - goes to "..."]` plus an `ELIGIBILITY:` header. Verified firing on BOTH shapes:
    Glacial Revelation (vs27 s18 — marks snow permanents eligible, non-snow fetches not) and Into the North
    (vs14 s12 / vs102 s14 / vs49 s11 — marks snow lands eligible incl. Scrying Sheets, all non-snow-lands and
    non-lands not). Effect: 3/3 Into the North reveals parsed short + correct; ZERO reveal fallbacks (wave-19
    had 2); ZERO reveal spirals (wave-19 had ~4 12k-char spirals). The wave-19 R3/R4 eligibility gap is CLOSED.
  - RESIDUAL (a), LOW: Into the North is still LABELLED a whole-library "choose card / put back" menu (verbs
    describe a hand-partition, not a search-to-battlefield). Cosmetic now that eligibility is marked; the engine
    still correctly takes the first legal snow land to battlefield (4/4 outcome-correct across both waves).
    Optional polish: relabel verbs + offer only eligible snow-land targets.
  - RESIDUAL (b), UNTESTED: Azcanta ACTIVATION path (`borderline.txt:7118`, to-hand slot
    `target(<upto:1>*[-land;-creature])`) had ZERO windows this corpus (no Search for Azcanta transformed).
    The wave-19 residual (model picked ineligible creatures -> 0 to hand) is UNVERIFIED post-fix, and whether
    the R1-style all-sweep also lurks in the activation optiontwo (`target(<4>*|reveal) bottomoflibrary`) is
    still UNTESTED. Not vindicated by absence. VALIDATION TODO: an Azcanta activation where the model picks an
    eligible noncreature-nonland — confirm it reaches hand and the rest reach bottom.

- **ENGINE-R2 (wave-19 non-contiguous "ordering" residual) — remains REFUTED as an ordering bug.** It was never
  an off-by-one; it was the R1 partition drop, now fixed. The one seam that honors the chosen SET (Into the
  North) took the first legal target in index order correctly, both waves. Closed.

- **ENGINE #4b (optionone reveal AUTO-DECLINE) — remains CLOSED.** Regression-watch clean: the Search-for-Azcanta
  upkeep keep-branch fired correctly this wave (vs49 s27: `PUT: none` -> `kept them all (put in library)`).
  (Only 1 upkeep look this corpus vs 8 in wave-19 — draw variance; both branches known-good.)

- **ENGINE-1d-followup (Force free-pitch card naming) — NO TEST WINDOW this corpus.** Force of Negation cast 0
  times (no whitelist trigger arose). The wave-18 naming annotation (validated wave-19) had nothing to exercise
  it. Carried unchanged; the "auto-pick eats a finisher when a worse pitch existed" detector remains armed.

- **ENGINE #2 (stale `[attacking]` tag) — carried, benign.** Not specifically re-audited (attention went to the
  reveal-fix validation); no wrong decision traced to a board tag. Low priority. FIX (unchanged): clear
  `[attacking]` on any creature not in the current declared-attackers set.

## HARNESS

- **HARNESS-1 (NEW this wave, deck135-OWNED) — the parser locks in a RETRACTED early CHOICE line, and a
  decode-loop with such a line is HIDDEN from the fallback count.** vs35 s10 (ask, turn 4, 19/18,
  `fallback: null`, 15087 chars): the reply opened `CHOICE: 4 (Cast nothing right now)`, immediately retracted
  it (`Wait, I made a mistake in my reasoning`), then repeat-looped ~8x converging in prose on "cast Icehide
  Golem", and never re-emitted a final CHOICE line. The parser took the retracted `CHOICE: 4` as a clean parse
  -> logged cast-nothing -> deck135 deployed no creature that turn (Rule #1 obeyed in reasoning, leaked at the
  number). Non-fatal (vs35 won 4/-8) but a real dropped creature-deploy AND it means the decode-loop tax is
  undercounted by the fallback metric (true tax >=8, not 7). FIX: prefer the LAST well-formed CHOICE line; treat
  a CHOICE that precedes a "Wait, I made a mistake"/self-retraction as superseded; when no final CHOICE line
  exists after a retraction, route to the heuristic fallback rather than the retracted digit. DETECT: a
  clean-parsed record with a repetition signature + multiple/retracted CHOICE lines. (Sibling of the wave-10
  "obeyed-in-reasoning, leaked-at-number" class, answer-order variant.)

- **HARNESS-2 (CARRIED) — salvageLoopedChoice does NOT cover this seat's fallback shapes.** 0 of 7 fallbacks
  salvaged. The 7 are pure enumerate-and-reject spirals that never emit a clean trailing CHOICE/ATTACK line, so
  there is nothing for the last-well-formed-line salvage to grab. (The wave-19 reveal-`PUT:` and block-`BLOCKS:`
  salvage gaps had no windows this wave — 0 reveal fallbacks, 0 blocker fallbacks.) Pair salvage with a
  decode-time repetition guard; salvage alone cannot rescue a spiral with no clean line.

- **HARNESS-3 (CARRIED) — decode-time REPEAT-LOOP tax, worst-seat.** 7 fallbacks (+1 hidden, HARNESS-1) are
  >12k-char verbatim-repetition spirals on lethal-math / mana-confusion / survival-enumeration at hard-or-
  hopeless spots (5 in the lost vs27 grind, 2 in the lost vs49); none flipped a winnable game. Route to a
  decode-time repetition-penalty / max-token guard. Model/decode item, NOT guide-fixable.

## MODEL (route to model-experiments; NOT guide text) — see general-suggestions.
- Decode repeat-loop spirals (dominant; HARNESS-3). Snow-supertype / lethal-math confusion feeds them, but the
  eligibility surfacing removed the reveal-comprehension trigger this wave (spirals now concentrate on lethal-
  math + mana-payment at losing spots, not on reveals).
- Creature-in-hand-not-in-options false belief (vs35 s10, vs27 s19: "engine error?"). The GUIDE already covers
  this maximally (Rule #1 point 3 "read the OPTION LIST, not your hand"), and the model eventually reasons past
  it in every case — the residual is the decode loop on the way, not a missing guide fact. No new guide line
  would help. Route to decode mitigation.
- IMPROVEMENT: 3 real kills this wave (vs62, vs35, vs102); Glacial Revelation now rakes creatures to hand as the
  guide describes (vs27 s18). The plan closes when the deck's colored mana cooperates.

## DECK-CONSTRUCTION FLAGS (route upstream; user owns the decklist — restate, don't relitigate)
- Under-powered small-creature clock — softened but STANDS (7th corpus); Treefolk/Diamond Faerie the only real
  closers. R1 fix restores Glacial Revelation as a card-advantage engine (helps, doesn't cure).
- Thin blue base + fetch-color mismatch (Windswept Heath cannot fetch Island) — STANDS; strands gold/double-pip
  creatures in hand (vs27 s19: 4 creatures in hand, uncastable for lack of an untapped G/U source).
- Force DEAD vs all-creature aggro — dormant this wave (0 Force casts; vs35 Slivers won without it).

## VALIDATION TODO (for the next run)
1. **ENGINE-R1**: confirm the Glacial Revelation to-hand partition on a SECOND corpus (this wave's proof is n=1).
   Detector: "get snow" picks appearing in the subsequent `goes to graveyard` list.
2. **ENGINE-R4 (Azcanta activation)**: UNTESTED this corpus — get a window where the model picks an eligible
   noncreature-nonland; confirm it reaches hand (rules out an R1-style drop in the activation optiontwo).
3. **HARNESS-1**: confirm the parser no longer locks in a retracted early CHOICE (prefer last CHOICE line).
4. **ENGINE-1d (Force pitch naming)**: regression-watch — no window this wave; re-verify when a Force fires.
5. **HARNESS-1 wave-19 (block name (P/T)#N tolerance)**: no dup-name block window at this seat this wave —
   re-verify at any seat that gets one.

## PER-DECK ROTATION VERDICT: **KEEP deck135 IN one more cycle — but it is now APPROACHING rotation.**

Rotation retires a veteran whose guide takes NO modification AND whose seat surfaces NO new work signal.
deck135's guide took no modification (FROZEN, byte-identical, `cmp`-verified). The new-work signal is WEAKER
than wave-19 (which kept it in on the strength of an OPEN game-affecting bug) — this wave the seat's headline
charge was DISCHARGED: ENGINE-R1 is fixed + validated, the eligibility surfacing shipped + fired, the ordering
residual stays refuted, the auto-decline stays closed. So the seat is trending toward "no guide mod + no open
work." HOWEVER, three residual signals keep it IN for one more cycle:
- **The R1 validation is n=1** (one Glacial Revelation cast all corpus). A validated-on-a-single-instance fix is
  strong-but-thin per the rare-event-power discipline; one more corpus confirmation retires the item cleanly.
- **The Azcanta ACTIVATION path is UNTESTED** (0 windows) — the R4 activation-eligibility residual and the
  "R1-in-the-activation-optiontwo?" question are unverified, not vindicated by absence.
- **deck135 is the ONLY seat that exercises the reveal seam** (0 reveals elsewhere among the 7 decks). Rotating
  it out orphans the R1 second-confirmation and the Azcanta-activation validation — no other seat can run them.
- Plus one NEW harness item this wave (HARNESS-1, the retracted-first-CHOICE hidden loop) with a seq repro.
EXIT CONDITION (when deck135 becomes a rotation candidate): once ENGINE-R1 is confirmed on a second corpus AND
an Azcanta activation is observed resolving correctly, the reveal-seam charge is fully closed — at that point a
still-frozen deck135 with no open reveal work is the textbook rotation exemplar (a converged veteran, like
deck110/21 before it).
