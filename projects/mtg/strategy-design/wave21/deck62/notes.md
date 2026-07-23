# deck62 notes — engine/harness items + rotation verdict (wave 21)

Seat: deck62 (Enchantresses), FIFTH guided corpus (revised-guide round 5, edit D live). 153 decisions (ask 121,
attackers 23, blockers 9) across 6 games. Record 4/6 (W vs35,14,49,102; L vs27,135 — matchup/draw floors).
Fallbacks 4 = 2.6% (3 retracted_choice, 1 stale_echo; ZERO blockers): vs27 s10, vs135 s5, vs102 s3
(retracted_choice, all land drops); vs14 s29 (stale_echo, Web target). Binary 3be7f5a67.

## Carryover checks (the brief's explicit asks)
- **Computed-P/T distrust pocket: DEAD into round 5.** Tight-signature grep = 0 recount-of-printed-P/T. Fifth
  consecutive clean corpus. Retired watch stays retired.
- **A-list-only attacks: ZERO attacker fallbacks, ZERO A-list unparses, ZERO 0-attacker durdles.** All 23
  attacker decisions parsed clean. Every `ATTACK: none` had opts=1 (empty eligible set) except vs135 s13
  (opts=3 = Argothian 0/1, Birds 0/1, Gelid-Shackled Druid 1/3) which is a reasonable hold, not a durdle.
- **Enchant-opponent-gift ban / Argothian-first / engine-protection: no violations observed.**

## Wave-21 engine-batch live validation (this seat's mandate)
- **(c) retracted_choice parser: OVER-FIRES at this seat — the corpus's prime witness. See N9 below.** deck62
  owns 3 of the corpus's 5 retracted_choice, and ALL THREE are false positives.
- **(c) template-placeholder CHOICE drop: UNTESTED here.** Zero `[Number]`/`[Name]` parrots this corpus (grep
  clean); answer-first suppressed the wave-20 N7-template class. Nothing to drop -> validate elsewhere.
- **(b) ANSWER-FIRST + parser hardening: VALIDATED WORKING.** 0 unparsed, 0 empty (wave-20 had 29 unparsed).
  All 6 longest asks (10-12.9k chars) lead with a coded CHOICE line and parse clean. Truncation-unparse class
  eliminated at this seat. Max reply length dropped 15.5k -> 12.9k.
- **(h) baka blocker fallback (0-power skip / no 2nd-blocker pile): UNEXERCISED.** Zero blocker fallbacks; the
  heuristic block path never ran (see N8). Report null.
- **(d) prose-intent salvage: 0 fired at this seat** (consistent with answer-first — nothing needed one).
- **Combat options_text / trade-outcome annotations: still present and accurate** across all 9 block decisions
  (e.g. vs135 s21 "(both die)" on Yavimaya-into-Coatl matched deathtouch math; "(you kill it, your blocker
  lives)" on vs14 s21 Yavimaya-into-Lord matched).

## Engine / harness / representation items (NOT guide prose)

- **N9 (HARNESS, MEDIUM, NEW) — retracted_choice detector OVER-FIRES on over-deliberation-of-a-settled-decision,
  discarding a correct uncontested CHOICE.** The wave-21 (item c) retracted_choice route fires when a CHOICE is
  "retracted with no replacement." At deck62 it fired on 3 replies (vs27 s10, vs135 s5, vs102 s3) where the
  model emitted the CORRECT `CHOICE: 1 (Play Forest)` as the answer-first first line, NEVER retracted it, and
  then used self-correction language ("Wait", "Correction:") purely in the DOWNSTREAM PLAN reasoning about a
  future cast (Fists targeting Argothian's shroud / Primal-Rage-vs-Fists sequencing / Argothian mana math).
  Each reply has exactly ONE `^CHOICE:` line (the correct one), no second/contradictory CHOICE, no disavowal.
  The detector is keying on retraction PHRASES anywhere after the CHOICE line, not on retraction OF the CHOICE.
  This is deck62's signature N6 over-deliberation pattern (settle the answer, keep deliberating the next step)
  colliding with the new detector — exactly the over-fire the wave-21 brief flagged as the one place to be
  adversarial. Harm this corpus = ZERO (all 3 were land drops; the heuristic plays the land: vs27 3->4 Forests,
  vs102 events "Forest hand->battlefield"), but LATENT: on a CAST/TARGET ask the same over-fire would discard a
  correct answer for a blind heuristic pick. **Fix direction:** route to retracted_choice ONLY when a
  well-formed CHOICE line is explicitly disavowed AND no valid CHOICE remains, OR a CONTRADICTORY second CHOICE
  is emitted (which "last well-formed wins" already handles). If exactly one well-formed, in-range CHOICE line
  stands and no contradictory second CHOICE follows, TAKE IT — do not discard on prose "Wait/Correction" that
  lives in the PLAN/reasoning body. Crucially, this must NOT break the genuine revise-and-replace case (vs14
  s26, vs135 s21) where the model emits a SECOND well-formed coded line — those are distinguishable by the
  presence of a second coded line. Cross-seat (any deck whose model reaches an answer then self-corrects
  downstream reasoning). deck62 is the ideal validator for the fix (owns 3 of 5 corpus retracted_choice).

- **N7-stale (HARNESS, LOW, sibling of wave-20 N7-template) — a `CHOICE: N (qualifier + option-name)` in a
  TARGET menu falls to stale_echo when the parenthetical is prefixed with the spell name.** vs14 s29: Web target
  menu instructs "answer with the chosen TARGET's name (not 'Web')"; model wrote `CHOICE: 4 (Web targeting
  Yavimaya Enchantress)` — N=4 is the CORRECT in-range target (Yavimaya), but the "Web targeting" prefix made
  the parenthetical fail the option-4 name match -> stale_echo -> heuristic Web'd a 1/1 Saproling instead of the
  intended 4/4 Yavimaya (marginal harm; game won). Fix: when a `CHOICE: N (...)` has an in-range N AND the
  parenthetical CONTAINS the option-N name as a substring, prefer N (drop the leading spell-name qualifier)
  rather than stale_echo. Cross-seat (the "answer with the target name not <spell>" instruction is shared). LOW.

- **N8 (ENGINE, MEDIUM, CARRIED — UNEXERCISED this corpus) — baka blocker-fallback engine-sac.** The wave-21
  item (h) fix (0-power creatures skip kill-blocks and never pile as a 2nd blocker) is CODE-SHIPPED but could
  not be validated at deck62: zero blocker fallbacks this corpus (all 9 block decisions committed a first-line
  BLOCKS and parsed). Answer-first has made block fallbacks rare (wave-20's two were truncation-before-commit).
  Keep OPEN; validate at a seat/corpus where a blocker decision actually falls back and a valuable 0-power body
  is present. If deck62 never produces a block fallback again, the fix's validation must come from another seat.

- **N6 (model/decode watch) — REFRAMED again.** Wave-20's advancing-spiral/over-deliberation truncation-unparses
  are GONE (answer-first converted them from fallbacks into parsed-but-verbose replies — 0 unparsed). BUT the
  over-deliberation BEHAVIOR persists (the model still settles the answer then spirals downstream — it just no
  longer breaks parsing) and now surfaces as the N9 detector over-fire and as the snap-then-revise pattern
  (below). So: over-deliberation is no longer a PARSE problem, it is now (a) a false-fallback trigger (N9) and
  (b) a latent answer-first truncation risk. Route to the model/decode watch; not guide-addressable with
  confidence.

- **ANSWER-FIRST snap-then-revise (cross-seat WATCH, from this seat) — the model's snap first-line answer is
  frequently the WORSE option; only "last well-formed wins" rescues it.** vs14 s26 (snap CHOICE 4 [2nd
  Yavimaya] -> reasoned CHOICE 2 [Fists]) and vs135 s21 (snap BLOCKS B2:A1 [deathtouch Coatl] -> reasoned BLOCKS
  B2:A2 [13/13 Treefolk, saves 8 life]) both had the parser correctly take the reasoned last line. Latent risk:
  a reply truncating AFTER the snap answer but BEFORE the correction executes the worse snap answer. None
  truncated this corpus, so no harm — but it is the structural exposure of answer-first for a revise-heavy
  model. Cross-seat; watch for a truncation-between-snap-and-correction on any seat. See general-suggestions #2.

## Guide edit shipped this wave (round 5): NONE — BYTE-FREEZE
strategy.txt is a byte-identical copy of the deployed `Res/ai/baka/deck62_strategy.txt` (8893 bytes, `cmp`
IDENTICAL). Edit D is validated (its target spiral class dead); every other taught class held; the open items
are all HARNESS/ENGINE lane and unreachable by guide prose. No guide contradiction, #1-band leak, or new
model-fixable pathology surfaced to justify an edit.

## Rotation verdict
**NOT YET a rotation candidate — but the GUIDE is CONVERGED; the only hold is HARNESS-WITNESS value.** Apply
the standing test honestly: guide-mod this wave = NO (byte-freeze, edit D discharged). New-work-signal = present
but it is entirely HARNESS-lane, not guide-refinement: (a) N9 (retracted_choice over-fire) is a live, unfixed
harness defect for which deck62 is the corpus's PRIME witness and ideal fix-validator (owns 3 of 5 corpus
retracted_choice, all over-fires); (b) N8 remains code-shipped-but-unexercised and needs a block-fallback seat;
(c) the answer-first snap-then-revise truncation exposure wants one more corpus. Because the remaining signal is
harness-witness rather than guide work, deck62 sits at the ROTATION BOUNDARY: its GUIDE has earned a freeze and
would rotate on guide grounds, but rotating it out now would remove the best witness for the N9 fix. Recommend:
**KEEP one more cycle explicitly as the N9-over-fire fix validator** (and to see whether a block fallback ever
recurs for N8); once the N9 fix ships and is confirmed at this seat, deck62 becomes a clean rotation candidate
(a converged guide whose only open items are engine/harness owned by the ledger). Wave-22 must check: (1) did
the N9 fix stop the retracted_choice over-fire on deck62's land-drop/settled-decision replies without breaking
the genuine revise-and-replace last-wins case; (2) any block fallback for N8; (3) any answer-first
truncation-between-snap-and-correction.

## Engine items count: N9 (retracted over-fire, HARNESS MEDIUM, NEW); N7-stale (HARNESS LOW, NEW); N8
(blocker-fallback engine-sac, ENGINE MEDIUM, CARRIED/unexercised) + N6 model/decode watch (reframed) +
answer-first snap-then-revise watch (NEW). Retired: N2 (name-tolerance), N3 (P/T distrust), N7-template
(did not recur). Closed at this seat: the wave-20 truncation-unparse driver (answer-first).
