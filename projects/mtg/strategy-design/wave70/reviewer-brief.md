# Wave-70 step-3 brief (lean; the owner's loop definition is wave58/loop-as-envisioned.md; REJECT is the expected verdict for anything not paid for by a decision the corpus shows going wrong. ⚠ Amendment 332 stands: guides are decision-time instruction only, hard ceiling 20,000 bytes, no corpus/wave/seq/game citations or history in a guide — evidence goes in YOUR review.md.)

⚠ THIS CORPUS IS THE FIRST WITH REASONING ON SINCE WAVE 43. Owner ruling (skill invariant 000): reasoning happens ONLY in the model's native reasoning channel; the reply is a PLAN: line (a SEQUENCE of intended actions) and then the action line, nothing else. Every corpus from wave 44 to wave 69 ran with `enable_thinking:false` because the harness default was never overridden; waves 66-69's reply-shape work (protocol reorders, heading-word skips, correction zones, reversal predicates, overrun meters, per-seam caps, truncation re-asks) was compensation for a model with nowhere else to think. Wave 70 step one deleted the prose readers and rewrote every instruction to the two-line shape (wave70/instruction-parser-audit.md = the surface list; wave70/lane-{BK,BL,BM,BN}.md = what changed; wave70/codex-review.md = the Astra review BN fixed). Records now carry `thinking`, `reasoning_chars`, `reasoning_content` (the native reasoning, for you), `off_protocol_bytes`, `action_before_plan`, `max_tokens_reasoning`. Read the model's REASONING when a decision looks wrong: the reasoning says what it saw and why — that is the insight the channel exists to give you.

Repo /home/magi/Projects/wagicGPT, game dir projects/mtg. Use python for translogs; use /usr/bin/grep (bare grep drops matches). DO NOT edit anything under bin/Res, src/, or run git or wagic; write only under projects/mtg/strategy-design/wave70/. Never write mechanisms as the owner's positions. Report what you did NOT check.

CORPUS (wave 70 probe, binary = master 14c82df37 = archives/wagic-14c82df37-w70step1, `--thinking on`, WAGIC_GPT_TIMEOUT=900, no operator token ceiling): __CORPUS_DIR__ (__CORPUS_STATS__). One JSONL per SEAT (filename ...deckA-<ptr>-vs-...deckB = the deckA seat); records {seq, kind, prompt, reply, reasoning_content, choice, chosen_text, options, turn, phase, my_life, opp_life, latency_ms, fallback?, ...}; `prompt` is the full per-decision message the model saw; a closing `gameend` record carries the outcome and the census counters; results.tsv has winners. COUNT FROM THE RENDERED `prompt`, never from the `options` array; a HOLD take census must PREFIX-match the row. Verify every card fact against the primitive (`/usr/bin/grep -n "name=Card Name" -A8 projects/mtg/bin/Res/sets/primitives/*.txt`); if Scryfall (https://api.scryfall.com/cards/named?exact=NAME) disagrees, report both.

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
Output: ONE file, wave70/deck<N>/review.md: (1) game-by-game outcome + the deciding decisions with
seq citations; (2) engine/interface/card items ranked HIGH/MED/LOW with a concrete repro (file,
seq, the rendered line) — this is the primary deliverable; (3) guide verdict: KEEP as is (a
success verdict, state why) or EDIT — if EDIT, also write wave70/deck<N>/strategy.txt (the full
revised guide, start from the live one) and list each edit before->after with the seq that
paid for it IN review.md — the guide itself carries NO citation, count, or history (Amendment 332); hard ceiling 20,000 bytes, and an EDIT that grows the guide must say which rule earned its bytes; (4) OPTIONAL proposals, only when
your evidence supports them: general-guide changes (wave70/deck<N>/general-proposals.md) and
strategy-writing-skill changes (wave70/deck<N>/skill-proposals.md; current skill edition =
wave68/strategy-writing-skill-v2.md (the ~40 KB distillation, owner-installed 2026-09-06 after six Astra adversarial rounds; read it in full — it is short; the 1.3 MB wave70/strategy-writing-skill.md is lineage only) — read it before proposing; a proposal that
restates an existing amendment is noise). No prediction tables, no rotation evidence, no
docket bookkeeping. Final message: <= 10 lines.

## Engine-narration seat (one agent) — THE CENSUS THAT DECIDES THE REVERT LIST
You read the whole corpus for the engine's sake, not the guides'. Deliverable wave70/engine-seat.md. Your FIRST section is the COMPENSATION CENSUS: for every mechanism waves 66-69 added to cope with in-band reasoning, count under reasoning-on whether it still fires and whether the decision it guarded still goes wrong without it. The list (each with its record field or parse note; read wave66..69/lane-*.md for the rest): reply shape — `off_protocol_bytes` distribution per seam, `action_before_plan`, replies that are NOT exactly PLAN+action (count, shape classes, quote three); truncation — `reply_truncated`, `reply_truncated_reask`, `reply_truncated_answer_changed` per seam, `finish_reason=length` with reasoning vs without (did any seam ANSWER cap of 768 cut a two-line reply?); reasoning — `reasoning_chars` distribution, `max_tokens_reasoning` hits (`reasoning_budget_hit`/`reasoning_forced_close`, phase-2 recoveries), `reasoning_hidden`, any record with `reasoning_chars==0`; re-asks by kind (`label_missing_reask`, `index_name`, `noop_plan`/`noop_row_*`, `repeat_past_stop`, `plan_choice`, `attack_*`/`blocks_*` reask kinds — which still fire, which are now dead); the plan carry — PLAN line length, step counts, `plan_steps_done` advance vs execution, the 400-char cut count, plans restated verbatim vs changed, ledger-shaped PLAN lines (board facts instead of intended actions); latency and `deadline_pct` under reasoning-on (WAGIC_GPT_TIMEOUT=900) vs wave 69's thinking-off numbers; fallbacks by kind. For each mechanism: FIRES N of M (file+seq) / DEAD (0 of M windows where it could) / UNTESTED, and a one-line verdict: KEEP (still earns its keep under reasoning-on), DELETE (dead or only ever compensated for in-band reasoning), or WATCH. That table is the revert list's evidence; do not propose reverts on taste. Then:
(1) corpus census: games natural/hung/crashed, decisions, fallbacks by kind, stale drops (`async_drops`), transport failures (`transport` field) and
`transport_error` retries, timeouts, wall misses, HOLD renders/takes
(prefix-match), SEGV/abort/hang (report whether the wave-58/59 signatures recur: `empty_reply`, `transport_error`, `bad_reply`, `deadline_pct > 100`, a translog gap past the watchdog, `reveal_stall_forced`); (2) adjudicate EVERY prediction in wave70/lane-{BK,BL,BM,BN}.md (and the wave-69 carried items: Idyllic Tutor payload stranding, 146v123 replay divergence) as PASS / FAIL / UNTESTED (UNTESTED is a real verdict when no window arose; never
infer) — cite counts N of M with file+seq;
(3) engine/render/narration defects ranked HIGH/MED/LOW with repro (file, seq, the rendered
line) — things a per-deck reader would not see because they cut across decks: false verdicts,
contradictory headers, missing facts at decision time, narration that lies, parser refusals;
(4) the owner's lategame specimen (standing rule): pick ONE late-turn decision (turn >= 25,
preferably a decision that mattered) and write its FULL `prompt` verbatim to
wave70/lategame-specimen.txt with a 3-line header (file, seq, turn, why this one). Final
message: <= 15 lines, the HIGH items first.

## Synthesis (runs only if any deck agent wrote general-proposals.md or skill-proposals.md)
Read all proposals + engine-seat.md. Decide each proposal: ADOPT (edit wave70/general-strategy.md,
a copy of wave62's (the R331 edition; waves 63-67 wrote no edition; wave 68 installed wave68/strategy-writing-skill-v2.md as the current edition), or append a numbered amendment to wave70/strategy-writing-skill.md, a
byte-verbatim copy of wave59's with amendments appended — verify the prefix with cmp) or
REJECT with one line of reasons. Write wave70/synthesis-notes.md. If a deck agent's guide edit
contradicts an adopted general change, note it; do not edit deck guides yourself.

METHOD NOTE (from the wave-59 engine seat): the translog `phase` field is not the rendered phase — count phase-gated facts from the prompt text.
