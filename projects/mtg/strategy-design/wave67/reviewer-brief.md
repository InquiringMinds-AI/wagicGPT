# Wave-67 step-3 brief (lean; the owner's loop definition is wave58/loop-as-envisioned.md; wave 58 adopted 14 of 14 proposals and the owner flagged that as the busywork pattern; waves 59-61 adopted 0, wave 62 adopted 1 (R331), waves 63-66 adopted 0 — REJECT is the expected verdict for anything not paid for by a decision the corpus shows going wrong)

Repo /home/magi/Projects/wagicGPT, game dir projects/mtg. Use python for translogs; use
/usr/bin/grep (bare grep drops matches). DO NOT edit anything under bin/Res, src/, or run git or
wagic; write only under projects/mtg/strategy-design/wave67/. Never write mechanisms as the
owner's positions. Report what you did NOT check.

CORPUS (wave 67, binary = master __HASH__ = wave-66 close 8ddf25c89 + wave-67 step 1: lanes AV (the reply rule, from a corpus
measurement table in lane-AV.md: FIRST-WINS KEPT — the before-PLAN anchor tied 4/9 and only 75% of replies carry a
line-leading PLAN; shipped: the label scanner skips a heading word + colon (`ANSWER:`, `So`, `CORRECTION:`, `The answer is`,
`Therefore,`) at every seam; a complete reply with a PLAN and no coded line is salvaged by an exact single-row name match on
the last action sentence (ask/priority seams only; AZ R1 adds negation polarity) else ONE re-ask `label_missing_reask`;
`kReplyProtocol` rewritten with no section named by a label-able word; `correct plan` added to the correction cues, `wait`
deliberately excluded), AW (I4 the X library ceiling reserve = draw-step extras + `stackPendingDrawsFor`, every drawing X
row carries `{library: this draws N of your L library cards - L-N left; M more pending}` and the ANNOUNCE_X ask repeats it
(AZ R5: may-draws counted optional); I9a repeat-pay rungs collapsed at the RENDER with `shownToFull` mapping (AZ R4: every
legal rung stays answerable as a band); ATTACK TOTAL states lethal/not; sorcery-speed bracket names the failed half; MDFC
land rows carry enters-untapped; edict floor names the count), AX (I5 a refused mandatory reveal takes a legal pick — AZ
R7: the heuristic's pick, not index 0 — recorded `reveal_fallback_pick`; ROOT CAUSE of the 600 s reveal stall =
`MTGRevealingCards::CheckUserInput` ignores BTN_NEXT on a `<1>` targetMin chooser, fixed at the driver floor; the
reveal residual reads latency before it is consumed; I7 every fallback record carries `executed_seam/executed_choice/
executed_text`; a `{reserve:}` decline is honoured while the board key is unchanged (AZ R3: FULL hold key, not
phase+names+sources); transport: measured, no change, the 900 s curl-28 wall has zero retry budget by arithmetic;
`async_drops` 59 all on the casting arm, 0 livelock), AY (I6 the second past-stop answer executes the model's OWN stated
stop `repeat_clamped_to_own_stop` (AZ R2: allowed==1 boundary) and the `{right now: M=.., your stated stop=..}` clause
reads a persisted `mStatedStop` so it survives refusals; I8 `Your graveyard (N cards): ...` beside the battlefield (feeds
boardKey), `Their graveyard` when non-empty; an EXILED loop half reads BROKEN (AZ R6: a graveyard half needs visible
recursion); I9b `planArguesAgainstRow` sees anaphora ("this does nothing") and unlabeled plans; exemplar avoids half-dead
rows; 208-line death batches bucketed at render), and AZ (the Codex review: __AZ_SUMMARY__); read wave67/known-bugs.md,
wave67/lane-{AV,AW,AX,AY,AZ}.md and wave67/codex-review.md for what changed and each lane's falsifiable predictions.
Lane-flagged questions for the engine seat: (1) `unparsed_reply` count (wave 66: 83; AV predicts < 5) and the shapes of
any that remain — especially `So PUT:` at the discard seam which AV did NOT wire; (2) answer-first share and overrun bytes
under the rewritten protocol (wave 66: 73% answer-first, 60% overrun) — and did the model transcribe ANY new heading as a
label?; (3) `repeat_clamped_to_own_stop` / `repeat_past_stop` counts and any take still past a stated stop; (4)
`reveal_fallback_pick` count and whether any reveal stalled (`reveal_stall_forced` must be 0; `reveal_wait_unexplained_secs`
nonzero on how many); (5) the X ceiling — any deck-out, any X row whose `{library:}` line was wrong against the seat's
actual library; (6) the reservation-decline latch — how often it held, and whether it ever held across a changed board;
(7) I9a band answers — any mis-mapped pick (a number in the band executing a different rung); (8) graveyard line — did
any decision read it, and did boardKey re-opens rise; (9) `label_missing_salvaged` vs `label_missing_reask` counts and
whether any salvage executed a negated sentence): __CORPUS_DIR__ (__CORPUS_STATS__).
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
wave62/general-strategy.md (wave-59 edition + R331; NOTE from the wave-63/64/65/66 syntheses: this file is NOT loaded at runtime (only deck<N>_strategy.txt is opened, AIPlayerGPT.cpp:15332) — the deck guide is the live surface) + the reply protocol in the prompt), find issues in the game, the
interface, and the guide, and surface anything else worth the core loop's attention.
Method: for each of your deck's 6 games (your seat's translog; the opponent's for context),
trace the decisions that decided the game. Classify each misplay PERCEPTION (the model misread a
true surface, or the surface was false/missing -> ENGINE/RENDER item, cite file+seq) vs STRATEGY
(read correctly, decided against the guide or against good play -> guide item). Note fallbacks,
re-asks, and any decision where the prompt's information was insufficient or wasteful (what a
better interface would have shown). Hands are real (mulligans, bottoming): do not read variance
as guide effect.
Output: ONE file, wave67/deck<N>/review.md: (1) game-by-game outcome + the deciding decisions with
seq citations; (2) engine/interface/card items ranked HIGH/MED/LOW with a concrete repro (file,
seq, the rendered line) — this is the primary deliverable; (3) guide verdict: KEEP as is (a
success verdict, state why) or EDIT — if EDIT, also write wave67/deck<N>/strategy.txt (the full
revised guide, start from the live one) and list each edit before->after with the seq that
paid for it; keep the guide within the pool band (41-71 KB); (4) OPTIONAL proposals, only when
your evidence supports them: general-guide changes (wave67/deck<N>/general-proposals.md) and
strategy-writing-skill changes (wave67/deck<N>/skill-proposals.md; current skill edition =
wave59/strategy-writing-skill.md, amendments 1-331 — read it before proposing; a proposal that
restates an existing amendment is noise). No prediction tables, no rotation evidence, no
docket bookkeeping. Final message: <= 10 lines.

## Engine-narration seat (one agent)
You read the whole corpus for the engine's sake, not the guides'. Deliverable wave67/engine-seat.md:
(1) corpus census: games natural/hung/crashed, decisions, fallbacks by kind, stale drops (`async_drops`), transport failures (`transport` field) and
`transport_error` retries, timeouts, wall misses, HOLD renders/takes
(prefix-match), SEGV/abort/hang (report whether the wave-58/59 signatures recur: `empty_reply`, `transport_error`, `bad_reply`, `deadline_pct > 100`, a translog gap past the watchdog, `reveal_stall_forced`); (2) adjudicate EVERY prediction in wave67/lane-{AV,AW,AX,AY,AZ}.md as PASS / FAIL / UNTESTED (UNTESTED is a real verdict when no window arose; never
infer) — cite counts N of M with file+seq;
(3) engine/render/narration defects ranked HIGH/MED/LOW with repro (file, seq, the rendered
line) — things a per-deck reader would not see because they cut across decks: false verdicts,
contradictory headers, missing facts at decision time, narration that lies, parser refusals;
(4) the owner's lategame specimen (standing rule): pick ONE late-turn decision (turn >= 25,
preferably a decision that mattered) and write its FULL `prompt` verbatim to
wave67/lategame-specimen.txt with a 3-line header (file, seq, turn, why this one). Final
message: <= 15 lines, the HIGH items first.

## Synthesis (runs only if any deck agent wrote general-proposals.md or skill-proposals.md)
Read all proposals + engine-seat.md. Decide each proposal: ADOPT (edit wave67/general-strategy.md,
a copy of wave62's (the R331 edition; waves 63-66 wrote no edition), or append a numbered amendment to wave67/strategy-writing-skill.md, a
byte-verbatim copy of wave59's with amendments appended — verify the prefix with cmp) or
REJECT with one line of reasons. Write wave67/synthesis-notes.md. If a deck agent's guide edit
contradicts an adopted general change, note it; do not edit deck guides yourself.

METHOD NOTE (from the wave-59 engine seat): the translog `phase` field is not the rendered phase — count phase-gated facts from the prompt text.
