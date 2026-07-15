# deck140 wave-11 — development notes (engine/harness/model/deck-construction; layer-routed, with repros)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-072254`, deck140 seat files
`*ai_baka_deck140-*.jsonl` (6 games). Fields: `prompt, reply, choice, chosen_text, options_text,
options, my_life, opp_life, latency_ms, turn, seq, kind, phase, events, model, fallback`. Opponent
creature count prints as `Opponent battlefield (creatures: N)`; the prompt's `Mana available: {..} from N
untapped sources` line is authoritative for affordability. Binary `/tmp/wagic-54a5a9d01`. `choice` is
1-based; `choice -1` = a fallback (heuristic played); `CHOICE: 0` = a distinct pass action (not an index).

## HARNESS / TOOLING
1. **`tools/intent-collapse-metric.py` still uncalibrated for answer-after-plan — carried from wave-10.**
   Reports `non-numeric heads: 215+/... (vacuous by design)` and a reversal heuristic that false-positives
   on the ubiquitous "hold Rakdos" plan-tail mention while MISSING the genuine mismatches (item 4). Do NOT
   compare its counts across the protocol boundary. FIX DIRECTION (unchanged): parse the trailing
   `CHOICE:N` as the answer; derive the plan's concluded option from its NAMED action; report a mismatch
   only when the named action != the recorded choice AND handle "named action absent from options"
   (item 4). REPRO: `python3 tools/intent-collapse-metric.py <dir with the 6 deck140 files>`.
2. **Ramble / truncation guard SHIPPED and VALIDATED — the wave-10 evasion is CLOSED.** Wave-10's vs44
   s34 (a 2048-cap formless truncation recorded `fallback: None` + a stray Rakdos cast the plan rejected)
   does NOT recur. This corpus, machine-checked over all 6 seats: EVERY reply lacking a
   `CHOICE:/ATTACK:/BLOCKS:` label is flagged (19 `empty_reply` + 1 `unparsed_reply`, all choice -1 ->
   heuristic); ZERO formless replies mis-cast; the out-of-range `CHOICE: 5` (vs131 seq22) also -> unparsed.
   The fallback TYPE shifted from `unparsed_reply` (2048 truncation) to `empty_reply` (HTTP-120s timeout on
   the longer 4096 tail), as expected. REPRO: for each seat, `[r for r in records if not
   re.search(r'(CHOICE|ATTACK|BLOCKS)\s*:', r['reply'] or '')]` -> all have `fallback` set and `choice -1`.
3. **20 fallbacks, NONE game-changing, NONE on a critical sweep window.** 19 `empty_reply` + 1
   `unparsed_reply` (choice -1), all `latency_ms` ~= 120003 (HTTP timeout, the known corpus-wide class;
   240s `WAGIC_GPT_TIMEOUT` ships next corpus). Machine-checked: no fallback occurred at C>=1 with a
   payable sweeper offered AND life <=16. The deck135 LOSS (adj 16-17, T13) coincided with 4 early
   HTTP-timeout fallbacks (seq4/5/6/9, T5-9) where the heuristic played land/develop/one C=2-at-life-21
   window — a plausible MINOR contributor to a 1-life adjudicated loss, but the mechanism is the timeout
   harness item, NOT pilot decision quality. REPRO: filter `fallback in {empty_reply,unparsed_reply}` and
   join to `results.tsv`.

## MODEL / REPLY-PROTOCOL (route to the reply-protocol A/B; do NOT touch the guide)
4. **Trailing PLAN/CHOICE mismatch = the UNLISTED/UNAFFORDABLE-CARD FIXATION, now BIDIRECTIONAL.** 3 genuine
   mis-maps + 1 caught-by-fallback / ~229 parseable decisions. The plan's NAMED action is a card NOT in the
   offered options (verified unaffordable against `Mana available`); the trailing index mis-maps:
   - **vs135 (`*0x56358a4ce760*`) seq10 t9** — C=0, plan "should NOT cast Pyroclasm... deploy Staff of Nin"
     (Staff unlisted); `CHOICE: 1` cast Pyroclasm into C=0. Over-cast. (Game lost by 1 life at adj.)
   - **vs131 (`*0x556dcd32b6f0*`) seq27 t20** — C=0, plan "should not waste sweepers... deploy Staff of
     Nin... hold Rakdos" (Staff {6} unaffordable: `Mana available {b}{w}{w}{w}{w}` = 5 sources); `CHOICE: 2`
     cast Pyroclasm into C=0. Over-cast. Won (adj).
   - **vs109 (`*0x557fe715e580*`) seq9 t6** — C=5 aggro, plan "cast Pyroclasm to destroy all 5" but NO red
     (`Mana available {b}{w}{w}{w}`, Pyroclasm needs {1}{R}); options [Cruel Edict, Cast nothing];
     `CHOICE: 2` cast Cast-nothing, ignoring the OFFERED payable Cruel Edict. UNDER-cast, in the loss.
   - **vs131 seq22 t16 (caught)** — plan wanted unlisted Staff; `CHOICE: 5` out-of-range -> `unparsed_reply`
     -> heuristic. The guard caught the unmappable index; the defect is only that IN-RANGE-but-wrong indices
     bypass the same fall-through.
   The model reliably NAMES its choice in prose, so the reconcile-named-action arm is tractable; its
   promotable requirement is handling "named action absent from options" -> best AVAILABLE option / pass,
   never an arbitrary cast (general-suggestions.md #1). deck140 = 3rd witnessing wave; deck135 wave-11 Q5 =
   2nd seat this wave. Distinct from head-first collapse (no head token exists) and from the phantom-board
   misread (item 6, where plan AGREES with a wrong choice). REPRO: read the plan tail + `options_text` +
   the `Mana available` line for each seq above.
5. **Mana-color misread (colored-mana / offered=payable family) at vs109 seq9.** The plan asserted "I have
   exactly the mana required for Pyroclasm ({1}{R})" while the prompt's `Mana available: {b}{w}{w}{w}` shows
   NO red source. Single seat here; the core prompt body already carries the governing generic/hybrid facts
   verbatim, so this stays a REPRESENTATION candidate (an option-line affordability/color breakdown), NOT a
   core edit — consistent with the wave-10 convergence verdict. Cross-seat grep remains the promotion
   trigger. REPRO: vs109 seq9 `prompt` vs `reply`.

## ENGINE / REPRESENTATION
6. **Phantom-board / stale-plan sweep vs affinity (WATCH, non-fatal).** vs110 (`*0x564f69b5c6a0*`) seq68
   (Pyroclasm) and seq82 (Wrath of God) cast into a board whose CURRENT line reads `(creatures: 0)`, while
   the plan believed a creature present ("exactly one creature, Ornithopter 0/2" / "Etched Champion" — both
   shown `battlefield -> graveyard` in `events`). Here the plan AGREES with the choice = a board MISREAD,
   NOT the item-4 trailing mismatch. Driver: affinity's within-turn create-then-die churn + the stale
   `YOUR PLAN (as you last stated it): Cast Pyroclasm to destroy the Ornithopter` echo carrying a dead
   creature forward. 2 windows, both in a WIN (vs110 24-14). Candidate lever (single seat -> WATCH):
   suppress/annotate the `YOUR PLAN` echo when the permanent it references is no longer on the current
   board. Route to representation/harness. REPRO: vs110 seq68/seq82 `prompt` (current board line) vs `reply`.
7. **Representation is CORRECT and being READ.** `(creatures: N)`, `[tapped - untaps...]`, the shroud tag
   (vs44 Sleep-Cursed Faerie), `Mana available`, and `- legal targets right now: the opponent` on Cruel
   Edict all render and the pilot's plans quote them accurately (Findings 1/2). No new representation ask
   for deck140 beyond item 6's WATCH.

## INFRA (confirmed shipped; standing item)
8. **adjudicate-by-life-at-cap confirmed a 2nd corpus.** deck140 reached the cap in 4/6 games, all adj by
   life. Rows: vs135 seat 16/17 T13 = `135 140 adj0 17 16 13`; vs131 26/23 T30 = `140 131 adj0 26 23 30`;
   vs110 24/14 T28 = `140 110 adj0 24 14 28` (all exact). MINOR NIT — vs44 seat last logged decision 22/17
   at T33 vs adjudicated `44 140 adj1 17 25 32` (deck140 25 at T32): the adjudicator snapshotted life a beat
   before the seat's final logged decision and deck140 gained a few life after (Venser's Journal / Elixir).
   Verdict unambiguous (deck140 clearly ahead either way); note only as a life-snapshot timing curiosity,
   not a correctness bug. REPRO: compare each seat file's last non-gamestart record `my_life/opp_life/turn`
   to `results.tsv`.

## DECK-CONSTRUCTION (user owns; flags only, restated not relitigated)
9. **Reach starvation + mana-screw is the ENTIRE loss column.** vs109 (mono-R, T9) had NO red source for
   Pyroclasm at the pivotal C=5 window and out-raced; vs133 (mono-B blitz, opp win T27) out-raced. vs131
   shows the control face: mana-screwed at 5 lands on T20, Staff {6} uncastable for many turns, won only via
   cap adjudication. Options in priority (unchanged from wave 9/10): (a) a cheaper recurring clock that
   survives a counter; (b) a 1-2 mana early lifegain/blocker vs aggro; (c) more untapped RED so Pyroclasm
   {1}{r} is a reliable turn-2/3 sweeper. 14 sweepers is the right density — needs TEETH, not text.
