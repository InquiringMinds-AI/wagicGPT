# Deck-135 wave-11 — development notes (engine / harness / model / construction)

Run: PRIMARY `matchups-20260715-072254`. deck135 jsonl epochs + opponents in findings.md game->file map.
Doctrine: win column context-only; layer-routing enforced (engine items live HERE with repros, never in
prompt/guide text). Each item self-contained with seq repros.

## ENGINE LEDGER

- **ENGINE #1 (HIGH PRIORITY, CARRIED — next cycle's top fix) — Force of Negation's FREE ALTERNATIVE COST
  never counters; the targeted noncreature spell resolves anyway.** UNCHANGED this corpus (fixed only the
  annotation half, ENGINE #1b below). NO NEW resolution evidence at this seat this wave: the pilot cast
  Force exactly ONCE (deck140, on Pyroclasm = a correct board wipe) and the game was ADJUDICATED mid-
  resolution.
  REPRO (deck140, seat 1784118177): s30 T13 opponent casts Pyroclasm (EVENTS s30 `Opponent's Pyroclasm:
  hand -> stack`). s30 pilot chooses `Cast Force of Negation with its exile a blue card from hand cost -
  can target on the stack: Pyroclasm`; s31 the SAME window re-offered, pilot chooses the mana version;
  s32 chooses `exile a blue card from hand` (EVENTS s32 `Choose an option for Force of Negation: -> exile
  a blue card from hand`); s33 empty_reply and the game closes adj0 (WIN 17-16, T13). Cross-checked the
  deck140 OPPONENT seat (1784118177-...deck140-...-vs-...deck135): ends seq14/T13 at `Cast Pyroclasm {1}{r}`
  with no resolution logged. So neither a counter NOR a bleed is confirmed this game — the interaction was
  cut off. The board at s32 still showed Search for Azcanta in hand => no confirmed blue card lost.
  QUANTIFIED BLEED THIS SEAT: 1 Force cast, 0 confirmed counters, ~0-1 confirmed blue exiles — down from
  wave-10's 4 casts / 0 counters / ~7-8 exiles. The collapse is partly trajectory, partly the restored
  discipline from ENGINE #1b.
  FIX DIRECTION (unchanged from wave-10): Force's counter-target resolution must bind the stack noncreature
  spell as its legal target (the `can target on the stack: X` hint already knows it exists). DETECT
  REGRESSION: a Force cast where the targeted stack spell's EVENTS show `stack -> graveyard/battlefield`
  (resolved) instead of a counter. VALIDATION TODO next run: force a NON-adjudicated Force-on-board-wipe
  window and confirm the wipe is COUNTERED and the blue card is spent for real effect.

- **ENGINE #1b (FIXED — VALIDATED) — the "NO legal target right now" annotation is no longer applied to
  counter-type (stack-targeting) options.** The wave-10 second fault is RESOLVED: `grep 'NO legal target
  right now'` = 0 at the deck135 seat (brief: 0 corpus-wide). Force options now render cleanly
  (`Cast Force of Negation {1}{u}{u} - can target on the stack: Pyroclasm`). CONSEQUENCES MEASURED:
  (1) reasoning-tax COLLAPSE — Force replies ~470 words (wave-10 deck140 s58) -> ~100 words this wave
  (deck140 s30/s31/s32 = 96/120/109 words, clean and correct); (2) restored Force TARGET DISCIPLINE —
  deck133 15 Force windows / 0 casts, all correct declines (Fatal Push s20, Collective Brutality s22,
  Inquisition s37 discard); the discard-exclusion clause OBSERVED OBEYED at s37 (Inquisition then discarded
  the Force itself, EVENTS s38 `Your Force of Negation: hand -> graveyard`). This is the annotation-
  suppression success signal (contradiction string -> 0 + tax collapse + adjacent-faculty recovery); see
  skill.md.

- **ENGINE #2 (minor, CARRIED) — a fetch-crack CHOSEN but not yet RESOLVED is re-offered for ONE extra
  priority window (down from wave-10's ~2 extra).** The pre-targeting keying REDUCED the wave-10 deck44
  s5-s7 3-windows-per-crack to a consistent 2-windows-per-crack pattern: `choose -> re-offered once ->
  resolve`. REPRO (deck140 seat 1784118177): s5 chooses `Put in Play with Misty Rainforest targeting
  Snow-Covered Forest` (EVENTS s5 `Misty Rainforest: hand -> battlefield` + ability announced); s6 the SAME
  option re-offered, chosen again; s7 resolves (EVENTS s7 `Misty Rainforest: battlefield -> graveyard |
  Snow-Covered Forest: library -> battlefield | life -1`). Same 2-window shape at deck109 s3/s4 & s7/s8;
  deck133 s12/s13, s25/s26, s30/s31, s41/s43; deck44 s6/s7, s15/s17, s28/s29; deck110 s7/s8. ONE life, ONE
  land, ONE sacrifice per crack — DECISION-COUNT inflation only, zero resource loss. Fetch-crack option
  windows: 61 this seat (~flat vs wave-10's 66). DETECT: consecutive `Put in Play with <fetch>` /
  `search basic land with <fetch>` chosen records with no intervening land `library -> battlefield`. If
  consume-on-choose is meant to withdraw the activation the instant it is CHOSEN (before it resolves off
  the stack), it is still re-offering it for the one window it sits on the stack.

- **ENGINE #3 (cosmetic) — fetch pre-targeting keying renders DUPLICATE-IDENTICAL option lines.** At the
  crack windows above, the two priority windows show TWO identical `Put in Play with <fetch> targeting
  Snow-Covered Forest` entries (deck140 s5/s6 options_text = two identical strings). The keying enumerates
  candidate target lands but collapses to duplicates when the deck holds multiple copies of the same snow
  basic (4x Snow-Covered Forest). Harmless (the pilot picks correctly) but wastes an option slot and is a
  minor rendering fix (dedupe identical targeting entries). DETECT: an options_text list with >1 byte-
  identical entries.

## HARNESS

- **HARNESS #1 (CARRIED) — `tools/intent-collapse-metric.py` reversal-flag count remains an unreliable
  defect count (index-offset false positives).** The RELIABLE signal (numeric-headed replies) is clean:
  0/all across the 6 deck135 seat files (every reply opens `PLAN:`, the answer-last signature). Use the
  reversal flags only as pointers to READ a reply, never as a count. (Tool also only accepts a DIRECTORY
  arg and globs `*.jsonl`.)

- **HARNESS #2 — fallback class shifted unparsed_reply(2048) -> empty_reply(120s HTTP timeout): 19 in this
  seat, all self-healing, none changed a game.** deck44 8 (all T15-17 in an already-lost game; heuristic
  still played lands/Search/fetches — e.g. s36 empty_reply then Search cast, EVENTS s36 `Search for
  Azcanta: hand -> stack -> battlefield`), deck133 4, deck131 4, deck140 2 (incl. s33 that closed the
  adjudicated Force window), deck109 1, deck110 0. These are the KNOWN corpus-wide 4.7% empty_reply
  timeouts (WAGIC_GPT_TIMEOUT default 240s shipped next corpus, commit 95cf9f5f9). No decision attributable
  to the heuristic fallback changed an outcome at this seat.

## MODEL (route to model-experiments; NOT guide text)

- **NEW: Azcanta-transform card-fact hallucination (deck44 s34).** The pilot believes Search for Azcanta
  transforms into "Azcanta, the Sunken Ruin, a 4/4 flying creature" blocker (it is a LAND) and fixates on
  the unoffered "transform" action, choosing Cast nothing over offered develop spells (Into the North /
  Search). REPRO deck44 seat 1784124541 s34 (T15, L5): opts `[Cast Into the North {1}{g}; Cast Search for
  Azcanta {1}{u}; Cast nothing]`, reply plans "transform Azcanta ... 4/4 flying creature that can block"
  then `CHOICE: 3`. Same FAMILY as the uncastable-plan hallucination below (unoffered-action fixation ->
  under-develop) plus a specific card-fact error. Single window, lost game, no outcome impact. WATCH: a
  KEY CARDS micro-note (Search for Azcanta = card advantage, flips to a LAND not a blocker) is the cheapest
  fix IF a 2nd seat/window recurs it (general-suggestions OBSERVATION 3). Not adopted this wave.

- **Uncastable-plan / mana-count HALLUCINATION (CARRIED) — the pilot plans an UNOFFERED play and under-
  develops.** deck131 s47 (T14, L1: declines Coatl/Viper/Golem reasoning "cannot pay life to accelerate
  into a board state I cannot survive" — a lost-position judgment call; a 1/3 Viper wall was arguably the
  better line but the game was already lost). deck131 s35 (declines Boreal Druid to "pass and untap"). The
  option list already surfaces castability and the guide carries the anchors (Rule #1 point 1 "read the
  OPTION LIST"; KEY CARDS Treefolk/Coatl "if NOT listed you can't pay yet"; DECIDING SITUATIONS "plan says
  cast Treefolk ... but NOT in your cast list"). KEEP anchors at full prominence (class still leaking, not
  demotable); no 3rd copy. Durable lever is model-side (or an option-line affordability annotation the
  engine already partly does via offered=payable).

- **colored-pays-generic false belief — did NOT recur at this seat this wave** (findings Q6). CORE candidate
  parked; cross-seat grep is the promotion trigger (general-suggestions OBSERVATION 1). The model self-
  corrected the inference in every window this corpus.

## DECK-CONSTRUCTION FLAGS (route upstream; user owns the decklist — restate, don't relitigate)

- **Manabase strands the deck's own 2-color win creatures — STANDS.** Ice-Fang Coatl {G}{U}, Ohran Viper
  {1}{G}{G}, Abominable Treefolk {2}{G}{U}, Diamond Faerie {2}{G}{W}{U} are gold/double-pip off a base with
  only 2 Snow-Covered Island + 2 Snow-Covered Plains + fetches. This wave: vs110 (LOSS, T7) mana-light
  (Druid T0 + Viper T6 only) — fast aggro loss; vs133 (LOSS) drew only 2 castable creatures over 19 turns
  (drain grind). Pilot-side develop is addressed (Rule #1 point 3 validated); the construction ceiling
  remains. Direction (user's call): raise effective double-pip sources, or shave a pip off the creature
  suite.
- **Slow clock is structural.** Abominable Treefolk is the only real closer and lands T6-T11; small bodies
  can't race lifegain/aggro (vs133 drain grind; vs110 T7 race). Construction, not guide.
- **vs110 marginal keep — reverted W->L this wave** (was adj0 win wave-10, now a T7 aggro loss). Single
  data point, variance-bound; NOT enough to reinforce the wave-9 low-priority mulligan watch. Leave the
  mulligan rule FROZEN.

## FREEZE-CHECK (turnaround / OBEYED-BUT-LOSING mode; guide is a BYTE-IDENTICAL copy of the live guide)

The guide was NOT edited (`cmp`: byte-identical, 13357 bytes). No new guide-fixable class — every residual
routes to engine (Force resolution, fetch re-ask/dup), model (Azcanta / uncastable-plan), core (colored-
pays-generic, no recurrence), or construction. Where each wave-10 line stood this corpus:
- **RULE #1** — point 1 (creature on sight) OBEYED across all games; the deck131 s35/s47 declines are lost-
  position judgment, deck44 s34 is the Azcanta model residual — both covered by the block's own anchors.
  Point 3 develop-branch VALIDATED again (no dead-board class; creatures on curve). Board-read/phantom-
  lethal anchor: 0 test windows = UNTESTED, KEEP at full prominence (pause-aware, do not demote off
  absence). FROZEN verbatim.
- **RULE #2 + WHO IS THE BEATDOWN** — combat healthy (Treefolk pressure vs131/vs44; profitable early blocks
  vs109; Boreal-Druid carve-out held). FROZEN verbatim.
- **MANA** — fetch-crack clauses (Island short-blue / Forest short-green) exercised and obeyed both ways.
  FROZEN.
- **KEY CARDS — Force of Negation** — the section is strategically correct (Pyroclasm=board wipe Forced
  correctly; all 1-for-1s declined; discard-exclusion obeyed) and the annotation is now FIXED, but the CARD
  is still engine-broken at RESOLUTION (ENGINE #1). Deliberately NOT changed (layer-routing). FROZEN verbatim.
- **KEY CARDS — Treefolk/Coatl uncastable anchors** — leaked at deck131 s47 (lost position) -> KEEP at full
  prominence, no demotion, no 3rd copy. FROZEN verbatim.
- **MULLIGAN / LETHAL CHECK / DECIDING SITUATIONS** — no counterexamples; vs110 marginal-keep reverted to a
  loss but single-point/variance. FROZEN verbatim.
No frozen line dropped, moved, or demoted. Guide diff = zero bytes.

## VALIDATION TODO (for the next run)
1. Confirm the ENGINE #1 Force fix (when it ships): a Force cast on a board wipe / mass bounce actually
   COUNTERS (target EVENTS show a counter, not `stack -> graveyard/battlefield` resolution) in a NON-
   adjudicated window, and stops bleeding blue cards.
2. Confirm ENGINE #1b holds (annotation stays off counter options): `grep 'NO legal target right now'` = 0.
3. Confirm the colored-pays-generic decline stays at 0 (deck110 s21 class) — and watch for a cross-seat
   2nd deck to trigger the core promotion.
4. Watch for a 2nd Azcanta-transform (or transforming-DFC-as-creature) hallucination window before adding
   any KEY CARDS card-fact note.
The record is construction-bound — do not read a flat/soft win-rate as a guide regression until the
manabase flag is addressed.
