# Wave-73 step-3 brief (lean; REJECT is the expected verdict for anything not paid for by a decision the corpus shows going wrong. Amendment 332 stands: guides are decision-time instruction only, hard ceiling 20,000 bytes, no citations/history in a guide — evidence goes in YOUR review.md.)

Fourth reasoning-on corpus (invariant 000). Wave 73 step one (wave72/known-bugs.md §N → wave73/lane-{BY,BZ,CA,CB}.md + wave73/codex-review.md): a HOLD now EXPIRES at the holder's next untap (`hold_released_turn`, eager release at the phase-change event); the priority window ALWAYS prints an explicit `0. Pass priority` row; every hold spelling leads with its cost; the sibling-window key normalises the answered-tag; ONE loop-scope sentence shared by the LOOP COMPLETE header and the LOOP SCOPE clause; the running-chain collapse generalised to any chain where the seat's OWN mana policy finds no legal action (`chain_windows_collapsed`); the repeat row's bracket teaches `stop=N; M=N` and the PLAN-line reader accepts the labelled forms as COUNTS only (never `stop at 3 life`, never their stop); rung menus sort numerically; reveal/search rows carry `[cost: …]`; worked examples cut balanced; loyalty clauses untruncated in the board frame; the noop re-ask quotes the match; replay counters one semantics; the reserve-decline latch re-armed (it had been dead since wave 72); four render items (animated-land death price, ghostform gloss only while live, MDFC back-face link, cannot-block subject); the carried-plan heading lost its caps phrase; `main_phase_windows_skipped` once per phase. Records carry `thinking`, `reasoning` (read it when a decision looks wrong), `reasoning_chars`, `off_protocol_bytes`, `plan_line_missing`, `plan_steps_done`; gameend carries the counters above. Wave-73 guide edits for all seven decks are the LIVE guides.

Repo /home/magi/Projects/wagicGPT, game dir projects/mtg. Use python for translogs; use /usr/bin/grep (bare grep drops matches). DO NOT edit anything under bin/Res, src/, or run git or wagic; write only under projects/mtg/strategy-design/wave73/. Never write mechanisms as the owner's positions. Report what you did NOT check.

CORPUS (wave 73, binary = source 622b10827 = archives/wagic-4b90ed7d7-w73step1, `--thinking on`, WAGIC_GPT_TIMEOUT=900, no operator token ceiling): __CORPUS_DIR__ (__CORPUS_STATS__). One JSONL per SEAT (filename ...deckA-<ptr>-vs-...deckB = the deckA seat); records {seq, kind, prompt, reply, reasoning, choice, chosen_text, options, turn, phase, my_life, opp_life, latency_ms, fallback?, ...}; `prompt` is the full per-decision message the model saw; a closing `gameend` record carries the outcome and the census counters; results.tsv has winners. COUNT FROM THE RENDERED `prompt`, never from the `options` array; a HOLD take census must PREFIX-match the row. Verify every card fact against the primitive (`/usr/bin/grep -n "name=Card Name" -A8 projects/mtg/bin/Res/sets/primitives/*.txt`); if Scryfall (https://api.scryfall.com/cards/named?exact=NAME) disagrees, report both.

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
Output: ONE file, wave73/deck<N>/review.md: (1) game-by-game outcome + the deciding decisions with
seq citations; (2) engine/interface/card items ranked HIGH/MED/LOW with a concrete repro (file,
seq, the rendered line) — this is the primary deliverable; (3) guide verdict: KEEP as is (a
success verdict, state why) or EDIT — if EDIT, also write wave73/deck<N>/strategy.txt (the full
revised guide, start from the live one) and list each edit before->after with the seq that
paid for it IN review.md — the guide itself carries NO citation, count, or history (Amendment 332); hard ceiling 20,000 bytes, and an EDIT that grows the guide must say which rule earned its bytes; (4) OPTIONAL proposals, only when
your evidence supports them: general-guide changes (wave73/deck<N>/general-proposals.md) and
strategy-writing-skill changes (wave73/deck<N>/skill-proposals.md; current skill edition =
wave68/strategy-writing-skill-v2.md (the ~40 KB distillation, owner-installed 2026-09-06 after six Astra adversarial rounds; read it in full — it is short; the 1.3 MB wave73/strategy-writing-skill.md is lineage only) — read it before proposing; a proposal that
restates an existing amendment is noise). No prediction tables, no rotation evidence, no
docket bookkeeping. Final message: <= 10 lines.

## Engine-narration seat (one agent)
You read the whole corpus for the engine's sake, not the guides'. Deliverable wave73/engine-seat.md: (1) corpus census as last wave (games, decisions, decisions/turn vs wave 72's ~2.1, fallbacks by kind with genuineness, `async_drops`, `transport`, `deadline_pct`, reply shape, `reasoning_chars`, `reply_truncated`, `askreplay/`, `plan_steps_done`); (2) REGRESSION WATCH on the wave-73 HOLD + collapse changes — the section that matters most: EVERY `hold_released_turn` (did any hold outlive the holder's untap? any hold taken on the opponent's turn: trace it to its release), hold-as-pass takes now that the pass row always prints (count holds whose PLAN says "pass" — wave 72: 12), `sibling_window_asks_skipped` with both option sets (any unseen acting row = HIGH), `stop_reached_windows_skipped` — for every collapse quote the PLAN line whose stop bound it and confirm it is the seat's OWN COUNT (a life/mana/their stop binding = HIGH), `chain_windows_collapsed` — for every collapse confirm the menu the model would have seen had no acting row (a hidden instant/activation = HIGH), the repeat-row stop clause render count (wave 72: 0) and how many PLAN lines now parse a stop; (3) adjudicate EVERY prediction in wave73/lane-{BY,BZ,CA,CB}.md as PASS / FAIL / UNTESTED with counts and file+seq (UNTESTED is a real verdict), plus the carried docket ({X}-announcement payment loss — count `dropped after its X was announced` in the stderr files; BS F1/F2/F3/F5/F6; the land latch; N16's `main_phase_windows_skipped`); (4) engine/render/narration defects ranked HIGH/MED/LOW with repro — especially any new render clause stating a false fact (the loop-scope sentence, `[cost:]` on reveal rows, the ghostform gloss, the cannot-block subject); (5) the owner's lategame specimen (standing rule, alert-only): ONE late-turn decision (turn >= 25), full `prompt` verbatim to wave73/lategame-specimen.txt with a 3-line header. Final message: <= 15 lines, HIGH items first.

## Synthesis (runs only if any deck agent wrote general-proposals.md or skill-proposals.md)
Read all proposals + engine-seat.md. Decide each proposal: ADOPT (edit wave73/general-strategy.md,
a copy of wave62's (the R331 edition; waves 63-67 wrote no edition; wave 68 installed wave68/strategy-writing-skill-v2.md as the current edition), or append a numbered amendment to wave73/strategy-writing-skill.md, a
byte-verbatim copy of wave59's with amendments appended — verify the prefix with cmp) or
REJECT with one line of reasons. Write wave73/synthesis-notes.md. If a deck agent's guide edit
contradicts an adopted general change, note it; do not edit deck guides yourself.

METHOD NOTE (from the wave-59 engine seat): the translog `phase` field is not the rendered phase — count phase-gated facts from the prompt text.
