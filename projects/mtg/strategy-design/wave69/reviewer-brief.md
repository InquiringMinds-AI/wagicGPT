# Wave-69 step-3 brief (lean; the owner's loop definition is wave58/loop-as-envisioned.md; wave 58 adopted 14 of 14 proposals and the owner flagged that as the busywork pattern; waves 59-61 adopted 0, wave 62 adopted 1 (R331), waves 63-68 adopted 0 — REJECT is the expected verdict for anything not paid for by a decision the corpus shows going wrong. ⚠ OWNER RULING 2026-09-06 (wave69/strategy-writing-skill.md Amendment 332): the guides are decision-time instruction only (~20 KB, rewritten wave 68; every wave-68 edit kept them there); a guide may NOT contain corpus/wave/seq/game citations, 'previous games', counts of past windows, or any history — that evidence goes in YOUR review.md. The 41-71 KB band is retired; hard ceiling 20,000 bytes.)

Repo /home/magi/Projects/wagicGPT, game dir projects/mtg. Use python for translogs; use
/usr/bin/grep (bare grep drops matches). DO NOT edit anything under bin/Res, src/, or run git or
wagic; write only under projects/mtg/strategy-design/wave69/. Never write mechanisms as the
owner's positions. Report what you did NOT check.

CORPUS (wave 69, binary = master f152d0df4 = wave-68 close d8772a91c + wave-69 step 1: lanes BF (K2 caps re-fitted from the
corpus — ask 640->768, discard/reveal/bottom 384->512, nothing tightened; truncation re-ask at EVERY seam; a recovery that
changes the answer is `reply_truncated_answer_changed`; protocol: a correction goes BEFORE the PLAN line; K3 the reversal
predicate reads only the correction zone between the coded line and PLAN, plus `proseNegatesTakenRow` for a negation
naming the taken row (123v162 s34; BJ F6 tightened it); `reversal_reask_same_answer`), BG (K1 ENGINE ROOT CAUSE: Idyllic
Tutor's reveal MOVES the whole library into the reveal zone and its return payload never resolved, so the next draw
decked the seat at 11 life — `drawFromLibrary` now pulls back cards parked out of THIS library, fixtured RED on base; K5 a
tapped-animation row is priced `{right now: does nothing this turn}` and joins the dead census, cleanup dead marks use the
obeyed `[...]` shape, `{you cannot pay this right now: 0 mana available}`; K8 the second-copy tag reads the script), BH
(K4 the missing reserve term = `scanStackAbilityDraws` matched only Player* targets while Staff of Nin's draw targets the
CARD — the stack term had rendered 0 times in 2,282 prompts; `stackDrawRecipient` fixed; Elixir row verdict;
`noop_row_zero_reask` off the row's own zero; K6 crack-back cover folds existing untapped blockers (BJ F2 scoped it),
`{reserve:}` ends in a VERDICT word, `{answers the stack: YES}` on a provable gain, blockers header names the
material-preserving assignment), BI (K7 `transport`/`attempt_ms` were conditional emitters that never triggered — now on
every round trip; async_drops + hold-suppression counters in gameend; `repeat_annotated_take`; GANG BLOCK proven reachable
(census added); `(neither dies)` head restored; band rows + Adversary rows = one payRepeatCollapse defect; `Paid ... with`
fold; one tap literal `{paying this taps:}`), and BJ (the Fable review's 8 findings all confirmed + fixed — the duplicated tap clause removed (F1), the crack-back cover counts standing bodies only once attacks are settled or the body has vigilance/defender/cantattack (F2), the refill +1 requires `all(this)` (F3), the ALTERNATIVE line states lethal (F4), library surfaces fold parked cards via `libraryCountWithParked` while the Tutor payload STRANDING stays open with a fireOneShot/addObserver hypothesis (F5), the negation test requires a governed verb phrase (F6), floating mana folded into cannot-pay (F7), `noop_row_retaken` (F8); a `_DEBUG` census DebugTraces any row stating a braced clause twice); read wave69/known-bugs.md,
wave69/lane-{BF,BG,BH,BI,BJ}.md and wave69/codex-review.md (FABLE seat this wave — the Codex pool was exhausted) for what
changed and each lane's falsifiable predictions. Lane-flagged questions for the engine seat: (1) `reply_truncated` per
seam under the re-fitted caps, `reply_truncated_reask` at the small seams, `reply_truncated_answer_changed` — did any cap
still lose a decision?; (2) reversal: `decision_reversed_in_prose` count (wave 68: 44, ~all from PLAN prose),
correction-zone non-empty count, `proseNegatesTakenRow` fires, `reversal_reask_same_answer`; (3) K1: any `library 0` line
with parked cards, any game end with both lives positive, any stranded `StackAbility (Source: Idyllic Tutor)` — the
payload stranding is NOT fixed unless BJ says so; (4) K4: every X window with an upkeep-draw permanent — printed ceiling
vs true remainder (wave 68: 2 under-counts); (5) `transport` and `attempt_ms` present on every round trip (absent on all
2,414 wave-68 records); (6) the crack-back cover in main 1 vs main 2 after BJ F2; (7) `noop_row_zero_reask` count and
whether K=0 sweeper takes fell (wave 68: 14); (8) hold-suppression counters and `async_drops` in gameend; (9) SHORT
GUIDES second corpus: prompt bytes, latency, and any wave-68 re-keyed literal that now prints): __CORPUS_DIR__ (__CORPUS_STATS__).
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
Output: ONE file, wave69/deck<N>/review.md: (1) game-by-game outcome + the deciding decisions with
seq citations; (2) engine/interface/card items ranked HIGH/MED/LOW with a concrete repro (file,
seq, the rendered line) — this is the primary deliverable; (3) guide verdict: KEEP as is (a
success verdict, state why) or EDIT — if EDIT, also write wave69/deck<N>/strategy.txt (the full
revised guide, start from the live one) and list each edit before->after with the seq that
paid for it IN review.md — the guide itself carries NO citation, count, or history (Amendment 332); hard ceiling 20,000 bytes, and an EDIT that grows the guide must say which rule earned its bytes; (4) OPTIONAL proposals, only when
your evidence supports them: general-guide changes (wave69/deck<N>/general-proposals.md) and
strategy-writing-skill changes (wave69/deck<N>/skill-proposals.md; current skill edition =
wave68/strategy-writing-skill-v2.md (the ~40 KB distillation, owner-installed 2026-09-06 after six Astra adversarial rounds; read it in full — it is short; the 1.3 MB wave69/strategy-writing-skill.md is lineage only) — read it before proposing; a proposal that
restates an existing amendment is noise). No prediction tables, no rotation evidence, no
docket bookkeeping. Final message: <= 10 lines.

## Engine-narration seat (one agent)
You read the whole corpus for the engine's sake, not the guides'. Deliverable wave69/engine-seat.md:
(1) corpus census: games natural/hung/crashed, decisions, fallbacks by kind, stale drops (`async_drops`), transport failures (`transport` field) and
`transport_error` retries, timeouts, wall misses, HOLD renders/takes
(prefix-match), SEGV/abort/hang (report whether the wave-58/59 signatures recur: `empty_reply`, `transport_error`, `bad_reply`, `deadline_pct > 100`, a translog gap past the watchdog, `reveal_stall_forced`); (2) adjudicate EVERY prediction in wave69/lane-{BF,BG,BH,BI,BJ}.md as PASS / FAIL / UNTESTED (UNTESTED is a real verdict when no window arose; never
infer) — cite counts N of M with file+seq;
(3) engine/render/narration defects ranked HIGH/MED/LOW with repro (file, seq, the rendered
line) — things a per-deck reader would not see because they cut across decks: false verdicts,
contradictory headers, missing facts at decision time, narration that lies, parser refusals;
(4) the owner's lategame specimen (standing rule): pick ONE late-turn decision (turn >= 25,
preferably a decision that mattered) and write its FULL `prompt` verbatim to
wave69/lategame-specimen.txt with a 3-line header (file, seq, turn, why this one). Final
message: <= 15 lines, the HIGH items first.

## Synthesis (runs only if any deck agent wrote general-proposals.md or skill-proposals.md)
Read all proposals + engine-seat.md. Decide each proposal: ADOPT (edit wave69/general-strategy.md,
a copy of wave62's (the R331 edition; waves 63-67 wrote no edition; wave 68 installed wave68/strategy-writing-skill-v2.md as the current edition), or append a numbered amendment to wave69/strategy-writing-skill.md, a
byte-verbatim copy of wave59's with amendments appended — verify the prefix with cmp) or
REJECT with one line of reasons. Write wave69/synthesis-notes.md. If a deck agent's guide edit
contradicts an adopted general change, note it; do not edit deck guides yourself.

METHOD NOTE (from the wave-59 engine seat): the translog `phase` field is not the rendered phase — count phase-gated facts from the prompt text.
