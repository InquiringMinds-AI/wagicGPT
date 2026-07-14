# deck140 wave-9 — development notes (self-contained; project is PAUSING after this wave)

Every item below carries its full repro + context so it stands alone with no wave-10 follow-up.
Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260714-094942`, deck140 seat files
`*ai_baka_deck140*.jsonl` (6 games). Records are kind ask|priority|gamestart|gameend; fields incl.
`prompt, reply, choice, chosen_text, my_life, opp_life, latency_ms, turn, seq, events`. Opponent creature
count is printed as `Opponent battlefield (creatures: N)` in the prompt; tapped attackers render
`[tapped - untaps and can attack next turn]`.

## FREEZE-CHECK (turnaround mode — constrained diff off the live wave-8 guide)
Every line the corpus showed OBEYED was frozen. The ONLY diffs in strategy.txt vs the live
`bin/Res/ai/baka/deck140_strategy.txt`:
1. SWEEP TRIGGER fire line: "your life is 16 or less" -> "your life is 16 or LOWER - 16 itself counts,
   so at exactly 16 you sweep -"; "they attacked you last turn" -> "they have attacked you on any recent
   turn". (Fixes the vs135 t13 boundary misread + the "last turn" -> "this turn" substitution.)
2. Tapped bullet: appended the invented-anti-pattern clause: "and 'it isn't attacking THIS turn' is NOT
   a reason to hold either: a tapped creature already hit you last turn and attacks again next turn, so
   kill it now on your main phase."
3. BSZ entry: qualified the guardrail so it never reads as "cast nothing" when no better sweeper exists:
   "if X would fall short AND a Wrath or Damnation is offered, cast that instead... If no other sweeper
   is offered, cast the biggest Black Sun's Zenith you can - partial removal still beats casting nothing."
   (Closes the implicit gap the wave-8 guardrail left; matches the vs44 t9 X=2 partial-sweep the pilot
   already played correctly.)
4. Lightmine entry + DECIDING SITUATIONS recap: added "do not deploy a second copy while one is already
   on the battlefield" (matches vs131 where the pilot correctly noted a redundant Lightmine) and mirrored
   the "16 or LOWER (16 counts)" + "not attacking this turn" wording into the recap.
Everything else — deck identity, #1 Rakdos hold, X-menu, Staff face-ping, deploy list, lands, mulligan,
override, artifact/shroud belief-kills — is VERBATIM. No frozen line is missing from the revision.

## INFRA / HARNESS
1. `-j` latency saturation is the ENTIRE timeout story. Per-game cumulative model latency (ask+priority):
   vs135 1259.6 s / 63 dec, vs131 1356.5 s / 103 dec, vs110 1297.0 s / 62 dec — vs the 3 losses at
   239-427 s / 18-37 dec. Corpus median ~10.4 s, p90 ~29 s, max 120,003 ms (vs110 t10). All 3 timeouts
   had deck140 AHEAD or even. REPRO: `python3` over the deck140 files summing `int(r['latency_ms'])` for
   kind in (ask,priority), grouped by game; compare to gameend presence (no gameend == timeout).
2. ONE empty-reply fallback all corpus: vs110 t10 seq14, on a land drop ("Play Scrubland"/"Hold
   Scrubland"), inside the 120 s latency spike; engine fell back to heuristic (`fallback: empty_reply`,
   choice -1). Harness artifact, not a decision failure. (Contrast wave-7 vs135's 19/34 cascade — not
   recurring.)
3. Adjudicate-by-life-at-cap remains the right fix (see general-suggestions #1). results.tsv already
   fills life/turn on timeout rows, which made this triage possible without opening logs blind — keep it.

## ENGINE / REPRESENTATION
4. Representation is CORRECT and being READ. The `(creatures: N)` count and `[tapped - untaps and can
   attack next turn]` tag both fire; the pilot's PLAN quotes them accurately (e.g. vs110 t10 seq15
   *"opponent has 0 creatures, so Pyroclasm/Damnation is wasteful"*). The vs135 t13 hold happened
   DESPITE the tag firing on Ohran Viper and the log showing it attacked t13 — so the residual there is
   wording/executor, not a representation gap. No new representation ask for deck140.
5. `events` field made the resolved-vs-asserted check machine-readable again (counters visible when
   they occur). No spurious-resolution assertions found this corpus.

## MODEL (route to reply-protocol layer / model-experiments, not a guide line)
6. Reply-protocol INTENT-COLLAPSE (answer-before-reasoning), 2nd seat after deck135. REPROS:
   - vs109 (`*deck140*0x55c3ebb0f4a0*`) seq10 t10: head `3`=Cast nothing at my_life 1; PLAN reasons
     *"I am at 1 life... it untaps next turn... I MUST kill it. I will cast Black Sun's Zenith with X=1."*
     Head never revised; engine cast nothing. Self-corrected seq11 (cast BSZ X=1, killed Legion Loyalist).
   - vs131 (`*0x557960b3d5e0*`) seq61 t40: head `2`=Cast Wrath of God into `(creatures: 0)`; PLAN
     *"casting a board wipe is unnecessary and wastes mana... I will not cast Wrath of God as there are
     no creatures to destroy."* Head cast Wrath anyway (one wasted sweeper, deck140 at 20 life).
   - vs135 (`*0x559b07f1b5a0*`) seq53 t19: head=Cast nothing; PLAN *"So I will cast Lightmine Field."*
     Self-corrected seq54.
   Distinct from plan/choice mismatch (there head matches a passive option and the reasoning is for a
   different action; here head and reasoning are for the SAME window but the head was emitted first).
   Route to the reply-protocol A/B; do NOT touch the guide.

## DECK-CONSTRUCTION (user owns; flags only, restated not relitigated)
7. Reach starvation + no closer is the ENTIRE loss column (vs109 t15, vs44 t14, vs133 t15 — all aggro,
   swept faithfully, out-raced) plus the vs131 grind (Blightsteel {12} folds to one Essence Scatter;
   Rakdos X=13 short of opp-29). Options in priority: (a) a cheaper/second recurring clock (threat #2
   after a counter); (b) a 1-2 mana early speed bump/lifegain vs aggro (Lightmine/Venser's arrive too
   late); (c) more untapped RED so Pyroclasm {1}{r} is a reliable turn-2/3 sweeper. 14 sweepers is the
   right density — needs TEETH, not text.

## WATCHES (single seat — do NOT legislate; full repro for a cold future check)
8. SNOWBALL single evasive threat (carried from wave-8, no new instance this corpus). Promote to a tight
   countable line ("a creature whose printed power RISES each turn — remove it on sight") only if a
   second seat reproduces a snowball-past-sweep-range loss.
9. WITHIN-TURN DOUBLE-SWEEP at C=0 (new, single seat): vs110 (`*0x555bea0a15b0*`) t22, seq38 Pyroclasm
   killed the lone Signal Pest (C=1 -> 0), then seq39 SAME turn cast a second Pyroclasm into the now-empty
   board, the stale PLAN still naming the dead Pest. Cost one card, C=0, deck140 AHEAD 18-3. Likely a
   re-offer + stale-plan interaction (the engine re-offered a cast after the board cleared). Promote to a
   guide/engine note only if a re-offer double-sweep recurs; not worth a line on one harmless instance.
