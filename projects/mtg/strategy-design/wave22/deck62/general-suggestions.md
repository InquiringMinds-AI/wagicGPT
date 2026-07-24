# deck62 wave-22 — general (cross-seat) suggestions

## STATUS UPDATE on wave-21's suggestions from this seat

- **wave-21 #1 (retracted_choice over-fire) — RESOLVED at deck62.** The reference-scoping fix (batch item b) shipped in
  binary 1d8ed8194 and this corpus validates it clean: the FP shape (single first-line CHOICE + downstream
  "Wait,"/"Actually," in PLAN prose) recurred 27x — 10 in the exact land-drop FP context — and ALL parsed clean; 0
  retracted_choice at deck62 (down from 3). The genuine revise-and-replace case is preserved (vs27 s16 emits a SECOND
  coded line; last-wins took it). **The 2 remaining corpus retracted_choice fires are BOTH at deck102's seat — the
  cross-seat closure verdict is deck102's to render** (are they real disavowals or residual FPs?). deck62's witness
  role on this item is done.
- **wave-21 #3 (stale_echo spell-name-prefix on target menus) — no deck62 signal this corpus** (0 reveals, 0
  stale_echo at deck62). Untested here; remains a cross-seat item for seats that hit target menus.

## 1. (cross-seat, WATCH — answer-first residual, REINFORCED with a BLOCKS-ask witness) For a reasons-then-revises model, answer-first's snap first answer is often the WORSE option; "last well-formed wins" preserves quality, and the residual risk is a TRUNCATION between the snap answer and the correction.

Carried from wave-21 #2, now strengthened. This corpus produced a fresh witness on a CONSEQUENTIAL ask (not just a land
drop): **vs27 s16 (blockers, 18/13)** snap `BLOCKS: B1:A2` (chump the 0/1 Birds of Paradise into a 2/2 — the exact
guide-violating chump-the-dork move) -> 6118 chars reasoning that cites the guide and reverses -> `BLOCKS: none`
(correct; 18->12 is not lethal, keep the dork). The parser correctly took the reasoned last line. **Snap answer worse,
reasoned answer better, last-wins rescued it — and here the stakes were real (a block decision), disproving any hope
that the snap-then-revise gap only touches trivial land drops.** No regression this corpus (0 truncations, 0 unparsed),
but the structural exposure stands: a reply that truncates AFTER the snap answer and BEFORE the correction would
execute the worse snap answer with no rescue. WATCH: on any seat, flag a truncated reply whose parsed (snap) answer
diverges from where its cut-off reasoning was heading (the answer-first quality-regression signature, distinct from a
parse regression). Only a decode-time length/repetition guard closes the truncation-between-snap-and-correction gap.
Low urgency (unobserved-as-harm), named for cross-seat vigilance. deck62 is a strong exposing seat for it (its
over-deliberation reliably produces the snap-then-revise shape), but with deck62 rotating, other revise-heavy seats
should carry the watch.

## 2. (observation, not a change) deck62's record remains a matchup-shape floor with pool-luck swing.
4/6 this wave (unchanged from wave-21's 4/6, but the pool swapped so the record delta is weak evidence). The four wins
all fired the taught attack-every-turn/go-wide beatdown (incl. grinding deck140-the-grinder to 2 life at the cap while
sitting at 20); the two losses are matchup floors — deck44 out-tempos (a faster clock the guide cannot draw) and
deck135 out-sizes (Icehide Golems + deathtouch flyers by T10-11). It is a turn-8-13 ground/go-wide deck; the guide
cannot fix a clock deficit — route the record to the roster owner, not the guide. Same standing verdict as waves 18-21;
win column is context-only per Step 0.

## 3. (decode-lane, cross-seat, LOW) Over-deliberation latency, not parse harm — repetition_penalty is shipped-but-OFF.
Answer-first has fully converted deck62's >12k-char over-deliberation spirals from parse failures into parsed-but-slow
replies (vs135 s24: 13.5k chars / 198.5s; p90 74.4s across the seat). The disease (decode-repetition CoT spiral) is
undiminished — it is now a token/LATENCY cost, not a correctness cost. Try the cheap knob (enable repetition_penalty at
the decode sampler) before any further work; a guide line cannot reach a decode loop (the model quotes correct guide
sentences mid-spiral). Cross-seat (every verbose-CoT seat pays this latency). Not urgent — harm is latency, not
misplay.
