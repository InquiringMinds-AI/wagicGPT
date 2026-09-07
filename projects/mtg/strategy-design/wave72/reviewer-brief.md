# Wave-72 step-3 brief (lean; REJECT is the expected verdict for anything not paid for by a decision the corpus shows going wrong. Amendment 332 stands: guides are decision-time instruction only, hard ceiling 20,000 bytes, no citations/history in a guide — evidence goes in YOUR review.md.)

Third reasoning-on corpus (invariant 000). Wave 72 step one (wave71/known-bugs.md §M → wave72/lane-{BT,BU,BV,BW,BX}.md + wave72/codex-review.md): harness harvests the `askreplay/` sidecar; prospective-zero no-op rescue + a re-ask that names the refused row; stop-reached windows collapsed (`stop_reached_windows_skipped`, dated by THIS reply's PLAN stop); `[castable now]` answered-tag; per-window replay refusal; ONE model call per window (casting ask first, else priority ask; a HOLD covers the sibling seam only when its acting rows are a subset — `sibling_window_asks_skipped`); priority-seam hold row priced beside activations; naming menus drop token-only names (CR 201.4), render copies-not-public, reorder with an inverse permutation; opponent `{effect:}` perspective frame; reserve fold of a mana-making unused land drop + consequence-first STRANDS; legend-rule header; loyalty rows untruncated; reveal rows annotated + destination zone; crack-back on casting seams (conditional when an animator/token maker exists); Puzzle Box hand-replacement line (root cause: StackAbility never set Interruptible::source); life-race clause on repeatable rows; LOOP COMPLETE header gated on the loop being able to operate; decline row states its scope; de-coloned hold head. DOCKETED, NOT FIXED (wave 73): an abandoned {X} announcement destroys the payment and leaves the lands tapped for the turn (`MTGPutInPlayRule: cast of … dropped after its X was announced`). Records carry `thinking`, `reasoning` (read it when a decision looks wrong), `reasoning_chars`, `off_protocol_bytes`, `plan_line_missing`, `plan_steps_done`; gameend carries the new counters above. Wave-72 guide edits for all seven decks are the LIVE guides.

Repo /home/magi/Projects/wagicGPT, game dir projects/mtg. Use python for translogs; use /usr/bin/grep (bare grep drops matches). DO NOT edit anything under bin/Res, src/, or run git or wagic; write only under projects/mtg/strategy-design/wave72/. Never write mechanisms as the owner's positions. Report what you did NOT check.

CORPUS (wave 72, binary = source ceb9b8de0 = archives/wagic-ba6af25f1-w72step1, `--thinking on`, WAGIC_GPT_TIMEOUT=900, no operator token ceiling): __CORPUS_DIR__ (__CORPUS_STATS__). One JSONL per SEAT (filename ...deckA-<ptr>-vs-...deckB = the deckA seat); records {seq, kind, prompt, reply, reasoning, choice, chosen_text, options, turn, phase, my_life, opp_life, latency_ms, fallback?, ...}; `prompt` is the full per-decision message the model saw; a closing `gameend` record carries the outcome and the census counters; results.tsv has winners. COUNT FROM THE RENDERED `prompt`, never from the `options` array; a HOLD take census must PREFIX-match the row. Verify every card fact against the primitive (`/usr/bin/grep -n "name=Card Name" -A8 projects/mtg/bin/Res/sets/primitives/*.txt`); if Scryfall (https://api.scryfall.com/cards/named?exact=NAME) disagrees, report both.

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
Output: ONE file, wave72/deck<N>/review.md: (1) game-by-game outcome + the deciding decisions with
seq citations; (2) engine/interface/card items ranked HIGH/MED/LOW with a concrete repro (file,
seq, the rendered line) — this is the primary deliverable; (3) guide verdict: KEEP as is (a
success verdict, state why) or EDIT — if EDIT, also write wave72/deck<N>/strategy.txt (the full
revised guide, start from the live one) and list each edit before->after with the seq that
paid for it IN review.md — the guide itself carries NO citation, count, or history (Amendment 332); hard ceiling 20,000 bytes, and an EDIT that grows the guide must say which rule earned its bytes; (4) OPTIONAL proposals, only when
your evidence supports them: general-guide changes (wave72/deck<N>/general-proposals.md) and
strategy-writing-skill changes (wave72/deck<N>/skill-proposals.md; current skill edition =
wave68/strategy-writing-skill-v2.md (the ~40 KB distillation, owner-installed 2026-09-06 after six Astra adversarial rounds; read it in full — it is short; the 1.3 MB wave72/strategy-writing-skill.md is lineage only) — read it before proposing; a proposal that
restates an existing amendment is noise). No prediction tables, no rotation evidence, no
docket bookkeeping. Final message: <= 10 lines.

## Engine-narration seat (one agent)
You read the whole corpus for the engine's sake, not the guides'. Deliverable wave72/engine-seat.md: (1) corpus census (games natural/hung/crashed, decisions, decisions/turn vs wave 71's 2.40, fallbacks by kind — are any `noop_row_zero_reask` false fires (quote the rows)?, `async_drops`, `transport`, timeouts, `deadline_pct`, reply shape: exactly PLAN+action vs `plan_line_missing` vs `off_protocol_bytes>0`, `reasoning_chars`, `reasoning_budget_hit` vs phase-2 closes, `reply_truncated` at 288/256 + the cardinality-floor re-ask, `askreplay/` now IN the run dir (count, runs, refusals), `plan_steps_done`); (2) REGRESSION WATCH on the wave-72 window model — the section that matters most: `sibling_window_asks_skipped` count and, for EVERY skip, whether the skipped sibling's rows were a subset of the held menu (any cycling/activation row the model never saw = HIGH); `stop_reached_windows_skipped` — for every collapse, was the stop stated THIS turn by THIS seat and was any non-repeat legal row live (HIGH if so); `own_turn_windows_skipped`; any priority ping-pong; any hang; (3) adjudicate EVERY prediction in wave72/lane-{BT,BU,BV,BW,BX}.md as PASS / FAIL / UNTESTED with counts N of M and file+seq (UNTESTED is a real verdict), plus the carried items (the {X}-announcement payment loss: count `dropped after its X was announced` in the stderr files and the tapped-out turns that follow; the Puzzle Box line on its live shape; Idyllic Tutor lag; the venture-step sentence); (4) engine/render/narration defects ranked HIGH/MED/LOW with repro (file, seq, the rendered line) — especially any of the new render clauses (perspective frame, copies-not-public, life-race, LOOP COMPLETE, conditional crack-back, animatable tense, decline-row scope) stating a false fact; (5) the owner's lategame specimen (standing rule, alert-only): ONE late-turn decision (turn >= 25), full `prompt` verbatim to wave72/lategame-specimen.txt with a 3-line header. Final message: <= 15 lines, HIGH items first.

## Synthesis (runs only if any deck agent wrote general-proposals.md or skill-proposals.md)
Read all proposals + engine-seat.md. Decide each proposal: ADOPT (edit wave72/general-strategy.md,
a copy of wave62's (the R331 edition; waves 63-67 wrote no edition; wave 68 installed wave68/strategy-writing-skill-v2.md as the current edition), or append a numbered amendment to wave72/strategy-writing-skill.md, a
byte-verbatim copy of wave59's with amendments appended — verify the prefix with cmp) or
REJECT with one line of reasons. Write wave72/synthesis-notes.md. If a deck agent's guide edit
contradicts an adopted general change, note it; do not edit deck guides yourself.

METHOD NOTE (from the wave-59 engine seat): the translog `phase` field is not the rendered phase — count phase-gated facts from the prompt text.
