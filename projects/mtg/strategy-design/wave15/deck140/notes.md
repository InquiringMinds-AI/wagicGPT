# deck140 wave-15 — development notes (engine/harness; layer-routed, with repros) + rotation verdict

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260716-040138`, 6 deck140 seat files
`*ai_baka_deck140-0x*-vs-*.jsonl`. Binary `/tmp/wagic-aad32bc7f`. Fields: `prompt, reply, choice,
chosen_text, options_text, options, my_life, opp_life, latency_ms, turn, seq, kind, phase, events,
fallback`. `choice` 1-based; `-1` = heuristic fallback; `CHOICE: 0` = pass. Opp creature count =
`Opponent battlefield (creatures: N)`; `Mana available: {..} from N untapped sources`.

## ENGINE (C++; route to engine batch, never guide prose)
1. **[HIGH, corpus-wide, now heavily quantified] "Mana available" hides dual-producible second colors;
   contradicts the offered-legal set.** 48 instances across 5 of 6 deck140 games (vs110:11, vs135:2,
   vs17:18, vs44:6, vs21:11; vs133:0). The line renders one defaulted tap-plan (B/R and R/W duals
   shown as B or W), so offered red spells (Pyroclasm {1}{r}, Rakdos {r}{b}{x}) and — rarely — black
   (vs17 s28/29 line `{w}{w}{w}{w}` vs offered Cruel Edict {1}{b}) look uncastable though the engine
   offers them as legal. IMPACT THIS CORPUS: zero — the model treated the offered list as
   authoritative (even cast Pyroclasm at vs135 s20 despite the line hiding red) and always had a
   visible alternative. But latent: the day the hidden color is the sole path to a needed cast it
   becomes a wrong decline (wave-13 vs109 s9 shape). ACTION: generate the affordability annotation
   from the same color-reachability engine as the legal-move set (or per-source color options).
   REPRO: any listed seq — e.g. vs110 s6 (line {b}{w}{w}, offers Pyroclasm {1}{r} + Rakdos {r}{b}{x}).

2. **[OPEN — UNTESTED this corpus] Rakdos's Return X auto-underpay (wave-13 vs109).** deck140 cast
   Rakdos's Return ZERO times this corpus (held every window in vs21; milled/held elsewhere), so the
   auto-tap/X-payment underpay could not be reproduced OR cleared — NOT-EXERCISED, not resolved. The
   adjacent X-payment path WAS exercised via Black Sun's Zenith (3 X-menus: vs135 s15 X=3, vs135 s18
   heuristic X=4, vs17 s12 deliberate X=1) — each surfaced as its own `ask`, listed biggest-X-first,
   and paid the chosen option correctly with no leftover-pool underpay. Partial mechanism-adjacent
   evidence that the X-menu machinery works; the Rakdos-specific item stays OPEN. ACTION: keep in
   ledger; needs a corpus where a lethal Rakdos actually fires. REPRO: none available this corpus.

3. **[WATCH — representation, needs 2nd witness, opponent-side] Dynamic-P/T 0/0 + -1/-1 counters
   render confusingly.** vs135 Abominable Treefolk (printed 0/0, snow-pumped, `[counters: 4x -1/-1]`)
   showed a live P/T that moved 4/4->5/5->9/9 while the counter annotation stayed static; the model
   spent whole reasoning blocks (s20, s21) unsure if it was dead. Not a deck140 card, not a deck140
   misplay driver. Candidate: show effective P/T after counters. REPRO: vs135 s19-s22.

## HARNESS / INFRA (confirmed; account for, do not re-diagnose)
4. **Fallbacks: 4 stale_echo + ~9 unparsed_reply at the deck140 seat; 0 empty_reply.** All 4
   stale_echo are CORRECT catches (echo named a card ABSENT from the offered set -> absent-echo
   staleness route -> heuristic): vs133 s9, vs135 s18 (BSZ X-menu, echoed spell not "X=N"), vs17 s32
   (echoed "Play Akoum Refuge"), vs21 s42. **No wrongful downgrade** (none named an offered option yet
   got routed to heuristic). unparsed_reply are long-latency rambles (150-215s, KV preemption @0.75)
   -> heuristic; none on a mandatory-sweep window with a bad outcome; none cost a game. REPRO: filter
   `fallback` and join to results.tsv.

5. **Adjudication regime.** 5 of 6 deck140 games decided by life-adjudication at cap (vs110/133/17/44/
   21); vs135 was a REAL board death (results.tsv winner=0, life1=-2, T16) — the one loss is NOT a
   latency artifact. Evaluate positions, not the adjudication count (per brief).

## DECK-CONSTRUCTION (user owns; flag only, restated not relitigated)
6. **Threat-mismatch is the whole loss column (unchanged).** vs135: Abominable Treefolk (snow-grown
   8/8 trample, regrows past -1/-1) outran Black Sun's Zenith's affordable X (capped 3-4 by black-
   source count) while Wrath/Damnation stayed undrawn; Lightmine arrived at life 3. Priorities
   unchanged from prior waves: (a) a high-toughness answer not gated on a big X; (b) more reliable
   early RED (also de-fangs engine item #1); (c) a faster counter-resilient closer. 14 sweepers is the
   right density — needs TEETH, not text.

## PER-DECK ROTATION VERDICT (brief-mandated, explicit)
**ROTATION CANDIDATE — recommend ROTATE OUT.** deck140's guide takes NO modification (frozen
byte-identical, 6th consecutive corpus of correct SWEEP-TRIGGER + #1-Rakdos-hold + all-lines
adherence), AND the seat surfaced NO NEW decision-level work signal this cycle:
- Owned Item 1 (Rakdos X-underpay): not exercised -> no new evidence.
- Owned Item 2 (dual-hiding): a KNOWN corpus-wide engine item (brief lists it explicitly); this cycle
  only QUANTIFIED it (48 instances, 0 impact) — confirmation, not new work.
- The single loss is structural deck-construction (user-owned), not a pilot/guide gap.
Per the rotation rule (frozen guide + no new work signal = rotation candidate), deck140 qualifies.
CAVEAT: the guide is demonstrably LOAD-BEARING (the model cites SWEEP TRIGGER / "16 or lower" /
hold-Rakdos / Staff-face-ping verbatim and wins 5/6) — rotation means "stop re-reviewing a proven
seat," NOT "remove the guide." The two engine items above remain in the SHARED engine ledger
regardless of whether this seat is reviewed again next cycle.
