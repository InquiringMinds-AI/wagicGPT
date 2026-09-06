# Wave-68 step-3 brief (lean; the owner's loop definition is wave58/loop-as-envisioned.md; wave 58 adopted 14 of 14 proposals and the owner flagged that as the busywork pattern; waves 59-61 adopted 0, wave 62 adopted 1 (R331), waves 63-67 adopted 0 — REJECT is the expected verdict for anything not paid for by a decision the corpus shows going wrong. ⚠ OWNER RULING 2026-09-06 (wave68/strategy-writing-skill.md Amendment 332): the guides were rewritten to decision-time instruction only (8-16 KB); a guide may NOT contain corpus/wave/seq/game citations, 'previous games', counts of past windows, or any history — that evidence goes in YOUR review.md. The 41-71 KB band is retired; hard ceiling 20,000 bytes.)

Repo /home/magi/Projects/wagicGPT, game dir projects/mtg. Use python for translogs; use
/usr/bin/grep (bare grep drops matches). DO NOT edit anything under bin/Res, src/, or run git or
wagic; write only under projects/mtg/strategy-design/wave68/. Never write mechanisms as the
owner's positions. Report what you did NOT check.

CORPUS (wave 68, binary = master 9be85773b = wave-67 close 949a514d7 + wave-68 step 1: lanes BA (J3 per-seam max_tokens caps sized from a corpus replay — ask 640 / priority 768 / attackers 768 / blockers 896 / discard-reveal-bottom 384 — with `reply_truncated_reask` at every seam (BE R4 added attackers/blockers), a two-sided overrun meter `pre_answer_overrun`/`reply_overrun`, and the protocol stating the budget; J6 one reversal predicate at CHOICE/ATTACK/BLOCKS, a `{does nothing}` row's re-ask exempt from the per-board budget; J1 the own-stop test on the plain activation row — BE R3: clamp applies to COUNTED rows only, an explicit second single-activation answer executes one), BB (J4 one X ceiling for header and row — the row now folds the whole reserve, `{library:}` with pending draws; bare-integer X names never conflict; J5 floating pool folded into the cannot-pay tag, post-announcement decline row says the pool stays floating (BE R6) and the abandoned cast is narrated; J9 `{answers the stack: NO ...}` on hold rows, crack-back verdict word in the hold key recomputed live), BC (J2 a wall miss gets no second deadline — `wall_miss_no_retry`, per-attempt `attempt_ms` and `deadline_pct` per attempt's own budget (BE R8); J7 ward `opponentzones` -> `opponentstack` across 85 script lines via the 5 `_WARD` macros, Oracle verified; Teferi stage 1 NOT shipped; `[loyalty N]` on walker target rows + the legend-rule clause now says the PICK DIES (BE R2, fixtured both directions); `executed_by` on recovery records; `async_drops` 0.95%), BD (J8 one `edictFloorScan` for mode and standalone rows; granted draws priced as a PRICE; crack-back cover on edict rows printed as a BOUND not a verdict (BE R5); `NO LIVE CAST ROW` names what is live; band rows shortened to ~40 B; opponent deck-out countdown at <=3 cards folding CANTMILLLOSE/CANTLOSE/CANTWIN (BE R7); stage-1 gate side census; `Paid ... with` fold), and BE (all 8 Astra findings confirmed+fixed incl. the ctor initializer-list comment that swallowed 30 initializers — now guarded at build time by tools/check-ctor-init.py); read wave68/known-bugs.md,
wave68/lane-{BA,BB,BC,BD,BE}.md and wave68/codex-review.md for what changed and each lane's falsifiable predictions. ALSO
NEW THIS CORPUS: the seven guides were REWRITTEN from ~71 KB to decision-time instruction only (wave68/rewrite/deck<N>/
changes.md lists what each kept and dropped) — every seat judges whether the SHORTER guide lost a rule the corpus shows
was needed (cite the seq), and the engine seat compares per-decision latency and reply length against wave 67 (the
guide is ~18K tokens smaller per prompt). Lane-flagged questions for the engine seat: (1) reply caps — `reply_truncated` count per seam, `reply_truncated_reask` count, and whether any decision was lost to a cap (compare latency/seam against wave 67's 88,567 s total); (2) `pre_answer_overrun` + `reply_overrun` census now that both sides are metered; (3) `wall_miss_no_retry` count and max `deadline_pct` (must be <= 100 per attempt); (4) `repeat_past_stop` / clamp counts on the PLAIN row and any take still past a stated stop; (5) `decision_reversed_in_prose` at ATTACK/BLOCKS — fired how often, and the no-op re-ask exemption; (6) X ceiling: any deck-out, any `{library:}` line wrong; (7) ward asks — 0 opponent library/hand cards offered as rows; (8) the reservation-decline latch under the full key — held how often; (9) SHORT GUIDES: per-decision prompt bytes and latency vs wave 67, `post_answer_overrun` share, and any decision the seats say a dropped rule would have changed): /home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-134120/ (21 games: 21 natural, 0 crashed, 0 hung, 74 min (fastest corpus yet); 2414 records; total reply generation 47,931 s vs wave 67's 88,567 s (-46%) with median latency 10.6 s / p90 46 s / max 315 s (wave 67: p90 75 s, max 1233 s) — BUT post-answer overrun bytes are 642,935 vs 662,814 (unchanged) and pre-answer 5,631 B, so the wall-time cut came from the SHORT SYSTEM PROMPT (prefill), not the caps: engine seat, confirm by seam; `reply_truncated` 58 (5 -> `reply_truncated_reask`, the rest kept their answer); fallbacks: 25 `plan_choice_conflict`, 5 `plan_contradicts_noop_row_reask`, 3 `index_name_conflict`, 2 `repeat_count_reask`, 1 `unparsed_reply`, 1 `repeat_past_stop`, 1 `blocks_reversed_in_prose_reask`, 1 `distinct_index_reask`; 162/152 5/6, 146/126/125 3/6, 130 2/6, 123 0/6 (was 3/6 — its rewritten guide cut seven rules for the byte ceiling, listed in wave68/rewrite/deck123/changes.md (c): deck123 seat, judge each cut against the six losses); lopsided finishes 126v123 51/0, 125v130 60/-10, 125v126 65/6, 125v123 89/-10, 146v125 -1/80).
One JSONL per SEAT (filename
...deckA-<ptr>-vs-...deckB = the deckA seat): records {seq, kind, prompt, reply, choice,
chosen_text, options, turn, phase, my_life, opp_life, latency_ms, fallback?, ...}; `prompt` is
the full per-decision message the model saw; a closing `gameend` record carries the outcome;
results.tsv has winners. COUNT FROM THE RENDERED `prompt`, never from the `options` array; a HOLD
take census must PREFIX-match the row. Verify every card fact against the primitive
(`/usr/bin/grep -n "name=Card Name" -A8 projects/mtg/bin/Res/sets/primitives/*.txt`; text=/auto=
are what the engine and model use); if Scryfall (https://api.scryfall.com/cards/named?exact=NAME)
disagrees, report both.

## Per-deck agent (one per deck: 146, 152, 125, 126, 162, 123, 130)
Purpose (owner's words): read the transcripts of your deck, compare the LLM's actions to the
instruction set (the guide projects/mtg/bin/Res/ai/baka/deck<N>_strategy.txt + the general guide
wave62/general-strategy.md (wave-59 edition + R331; NOTE from the wave-63..67 syntheses: this file is NOT loaded at runtime (only deck<N>_strategy.txt is opened, AIPlayerGPT.cpp:15332) — the deck guide is the live surface) + the reply protocol in the prompt), find issues in the game, the
interface, and the guide, and surface anything else worth the core loop's attention.
Method: for each of your deck's 6 games (your seat's translog; the opponent's for context),
trace the decisions that decided the game. Classify each misplay PERCEPTION (the model misread a
true surface, or the surface was false/missing -> ENGINE/RENDER item, cite file+seq) vs STRATEGY
(read correctly, decided against the guide or against good play -> guide item). Note fallbacks,
re-asks, and any decision where the prompt's information was insufficient or wasteful (what a
better interface would have shown). Hands are real (mulligans, bottoming): do not read variance
as guide effect.
Output: ONE file, wave68/deck<N>/review.md: (1) game-by-game outcome + the deciding decisions with
seq citations; (2) engine/interface/card items ranked HIGH/MED/LOW with a concrete repro (file,
seq, the rendered line) — this is the primary deliverable; (3) guide verdict: KEEP as is (a
success verdict, state why) or EDIT — if EDIT, also write wave68/deck<N>/strategy.txt (the full
revised guide, start from the live one) and list each edit before->after with the seq that
paid for it IN review.md — the guide itself carries NO citation, count, or history (Amendment 332); hard ceiling 20,000 bytes, and an EDIT that grows the guide must say which rule earned its bytes; (4) OPTIONAL proposals, only when
your evidence supports them: general-guide changes (wave68/deck<N>/general-proposals.md) and
strategy-writing-skill changes (wave68/deck<N>/skill-proposals.md; current skill edition =
wave68/strategy-writing-skill-v2.md (the ~40 KB distillation, owner-installed 2026-09-06 after six Astra adversarial rounds; read it in full — it is short; the 1.3 MB wave68/strategy-writing-skill.md is lineage only) — read it before proposing; a proposal that
restates an existing amendment is noise). No prediction tables, no rotation evidence, no
docket bookkeeping. Final message: <= 10 lines.

## Engine-narration seat (one agent)
You read the whole corpus for the engine's sake, not the guides'. Deliverable wave68/engine-seat.md:
(1) corpus census: games natural/hung/crashed, decisions, fallbacks by kind, stale drops (`async_drops`), transport failures (`transport` field) and
`transport_error` retries, timeouts, wall misses, HOLD renders/takes
(prefix-match), SEGV/abort/hang (report whether the wave-58/59 signatures recur: `empty_reply`, `transport_error`, `bad_reply`, `deadline_pct > 100`, a translog gap past the watchdog, `reveal_stall_forced`); (2) adjudicate EVERY prediction in wave68/lane-{BA,BB,BC,BD,BE}.md as PASS / FAIL / UNTESTED (UNTESTED is a real verdict when no window arose; never
infer) — cite counts N of M with file+seq;
(3) engine/render/narration defects ranked HIGH/MED/LOW with repro (file, seq, the rendered
line) — things a per-deck reader would not see because they cut across decks: false verdicts,
contradictory headers, missing facts at decision time, narration that lies, parser refusals;
(4) the owner's lategame specimen (standing rule): pick ONE late-turn decision (turn >= 25,
preferably a decision that mattered) and write its FULL `prompt` verbatim to
wave68/lategame-specimen.txt with a 3-line header (file, seq, turn, why this one). Final
message: <= 15 lines, the HIGH items first.

## Synthesis (runs only if any deck agent wrote general-proposals.md or skill-proposals.md)
Read all proposals + engine-seat.md. Decide each proposal: ADOPT (edit wave68/general-strategy.md,
a copy of wave62's (the R331 edition; waves 63-67 wrote no edition; wave 68 wrote wave68/strategy-writing-skill.md = wave59 + Amendment 332), or append a numbered amendment to wave68/strategy-writing-skill.md, a
byte-verbatim copy of wave59's with amendments appended — verify the prefix with cmp) or
REJECT with one line of reasons. Write wave68/synthesis-notes.md. If a deck agent's guide edit
contradicts an adopted general change, note it; do not edit deck guides yourself.

METHOD NOTE (from the wave-59 engine seat): the translog `phase` field is not the rendered phase — count phase-gated facts from the prompt text.
