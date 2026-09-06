# Wave-66 step-3 brief (lean; the owner's loop definition is wave58/loop-as-envisioned.md; wave 58 adopted 14 of 14 proposals and the owner flagged that as the busywork pattern; waves 59-61 adopted 0, wave 62 adopted 1 (R331), waves 63-65 adopted 0 — REJECT is the expected verdict for anything not paid for by a decision the corpus shows going wrong)

Repo /home/magi/Projects/wagicGPT, game dir projects/mtg. Use python for translogs; use
/usr/bin/grep (bare grep drops matches). DO NOT edit anything under bin/Res, src/, or run git or
wagic; write only under projects/mtg/strategy-design/wave66/. Never write mechanisms as the
owner's positions. Report what you did NOT check.

CORPUS (wave 66, binary = master 9b797fb81 = wave-65 close 0ea7b971c + wave-66 step 1: lanes AQ (H1 the DRAW FORECAST
charges only the draws still ahead — `receiveEvent` counts resolved draws per step, attributed by the drawing ability's
source (AU R2: only the turn-based rule or an `@each ... draw` permanent retires a forecast draw; unknown provenance ->
the minimum; `drawsUnattributedClause`); H4 one `lifeAfterPendingStack` term feeds both the ON THE STACK line and the X
badge; H9 one modal census (HALF DEAD bucket) tags rows at both menu seams with the CHOOSE_MENU subject recovered from the
arm-time script, punisher rate folded into the mode life figure; H10 `engineKindForScript` names token engines / draw
punishers / loop halves inside the sweep bracket and the forced-sacrifice ask — AU R3: repeatable = cost head or
recurring `@` trigger, `@movedto` ETBs are one-shot), AR (H2a `kReplyProtocol` reordered REASONING -> ANSWER LINE -> PLAN,
first-wins unchanged, both orders parse — FIRST LIVE TEST IS THIS CORPUS; H2b a retraction HEADER ("Correction:",
"Re-evaluating:", "Actually,", "On reflection") within 3 non-blank lines since the last coded line supersedes; H2c
`ignored_line` recorded verbatim; H8 `askExemplar` never exemplifies a `rowSaysNoOp` row (AU R5: all-dead -> the decline
row or a row-free template) and `plan_contradicts_noop_row` re-asks once; MED every number/name conflict stamps
`index_name_conflict`; `planLibraryOnlyNote` flags a carried plan naming a library-only card), AS (H3 the repeat row prints
`{right now: M=.., your stated stop=.., ...}` read from the carried PLAN and a counted take past the pilot's own stop earns
ONE `repeat_past_stop` re-ask; `loop_autopass` passes without asking when an opponent life loop is proven AND the oracle
says the seat has no legal action, narrated once; `[LOOP RUNNING: ...]` prompt-only note; H7 the hold key forgives ONLY six
enumerated life-projection clauses (AU R1 — kill counts, damage, prices, survivor counts re-open the hold) and the HOLD row
is inserted ahead of `Cast nothing right now` when `hasInstantResponse`; MED unique-name divergences resolve as
`index_name_unique_name`; `kCastNoRowZeroFact`), AT (H5 discard rows ordered by disposability composed onto `outOrder`,
`DEFAULT ANSWER: discard the spare land.`, `{NOT spare: ...}` tags; H6 `summoningSickTag` on target rows +
`attackersHeldSickLine` naming omitted sick bodies (AU R4: "can attack next turn" only where canAttack gates would pass);
MED `paymentNoLifeCostClause`, `castKillVerdictNow` on the mode menu's Cast-normally row, `landDropThreatTag`,
`sorceryReserveClause`; reveal stalls measured = 100% round trips, `reveal_wait_unexplained_secs` recorded), and AU (the
Codex review: all 5 confirmed + fixed as noted); read wave66/known-bugs.md, wave66/lane-{AQ,AR,AS,AT,AU}.md and
wave66/codex-review.md for what changed and each lane's falsifiable predictions. Lane-flagged questions for the engine
seat: (1) AR H2a — does the reasoning-first order hold (census `post_answer_overrun` vs wave 65's 72%), and does a coded
line INSIDE the reasoning ever become the executed first line (the hazard AR named)? (2) AS H7b — hold-row takes vs plain
decline (wave 65: 4:182 and 29:213) and the no-op window runs (152's 9-window/334 s run class); (3) AS H3 —
`repeat_past_stop` and `loop_autopass` counts, and whether any counted take still overshoots a stated stop; (4) AQ H1 —
runs of identical DRAW FORECAST re-charges (wave 65: 12 runs >= 3) and `drawsUnattributedClause` frequency; (5) AT H5 —
of the discard asks offering a spare row, how many discarded spares only (wave 65: 6 of 12); (6) AU R2 — any DRAW FORECAST
that under-charges (a pending Mine/punisher draw retired by a cantrip)): __CORPUS_DIR__ (__CORPUS_STATS__).
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
wave62/general-strategy.md (wave-59 edition + R331; NOTE from the wave-63/64/65 syntheses: this file is NOT loaded at runtime (only deck<N>_strategy.txt is opened, AIPlayerGPT.cpp:15332) — the deck guide is the live surface) + the reply protocol in the prompt), find issues in the game, the
interface, and the guide, and surface anything else worth the core loop's attention.
Method: for each of your deck's 6 games (your seat's translog; the opponent's for context),
trace the decisions that decided the game. Classify each misplay PERCEPTION (the model misread a
true surface, or the surface was false/missing -> ENGINE/RENDER item, cite file+seq) vs STRATEGY
(read correctly, decided against the guide or against good play -> guide item). Note fallbacks,
re-asks, and any decision where the prompt's information was insufficient or wasteful (what a
better interface would have shown). Hands are real (mulligans, bottoming): do not read variance
as guide effect.
Output: ONE file, wave66/deck<N>/review.md: (1) game-by-game outcome + the deciding decisions with
seq citations; (2) engine/interface/card items ranked HIGH/MED/LOW with a concrete repro (file,
seq, the rendered line) — this is the primary deliverable; (3) guide verdict: KEEP as is (a
success verdict, state why) or EDIT — if EDIT, also write wave66/deck<N>/strategy.txt (the full
revised guide, start from the live one) and list each edit before->after with the seq that
paid for it; keep the guide within the pool band (41-71 KB); (4) OPTIONAL proposals, only when
your evidence supports them: general-guide changes (wave66/deck<N>/general-proposals.md) and
strategy-writing-skill changes (wave66/deck<N>/skill-proposals.md; current skill edition =
wave59/strategy-writing-skill.md, amendments 1-331 — read it before proposing; a proposal that
restates an existing amendment is noise). No prediction tables, no rotation evidence, no
docket bookkeeping. Final message: <= 10 lines.

## Engine-narration seat (one agent)
You read the whole corpus for the engine's sake, not the guides'. Deliverable wave66/engine-seat.md:
(1) corpus census: games natural/hung/crashed, decisions, fallbacks by kind, stale drops (`async_drops`), transport failures (`transport` field) and
`transport_error` retries, timeouts, wall misses, HOLD renders/takes
(prefix-match), SEGV/abort/hang (report whether the wave-58/59 signatures recur: `empty_reply`, `transport_error`, `bad_reply`, `deadline_pct > 100`, a translog gap past the watchdog, `reveal_stall_forced`); (2) adjudicate EVERY prediction in wave66/lane-{AQ,AR,AS,AT,AU}.md as PASS / FAIL / UNTESTED (UNTESTED is a real verdict when no window arose; never
infer) — cite counts N of M with file+seq;
(3) engine/render/narration defects ranked HIGH/MED/LOW with repro (file, seq, the rendered
line) — things a per-deck reader would not see because they cut across decks: false verdicts,
contradictory headers, missing facts at decision time, narration that lies, parser refusals;
(4) the owner's lategame specimen (standing rule): pick ONE late-turn decision (turn >= 25,
preferably a decision that mattered) and write its FULL `prompt` verbatim to
wave66/lategame-specimen.txt with a 3-line header (file, seq, turn, why this one). Final
message: <= 15 lines, the HIGH items first.

## Synthesis (runs only if any deck agent wrote general-proposals.md or skill-proposals.md)
Read all proposals + engine-seat.md. Decide each proposal: ADOPT (edit wave66/general-strategy.md,
a copy of wave62's (the R331 edition; waves 63-65 wrote no edition), or append a numbered amendment to wave66/strategy-writing-skill.md, a
byte-verbatim copy of wave59's with amendments appended — verify the prefix with cmp) or
REJECT with one line of reasons. Write wave66/synthesis-notes.md. If a deck agent's guide edit
contradicts an adopted general change, note it; do not edit deck guides yourself.

METHOD NOTE (from the wave-59 engine seat): the translog `phase` field is not the rendered phase — count phase-gated facts from the prompt text.
