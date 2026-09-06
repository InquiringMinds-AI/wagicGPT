# Wave-63 step-3 brief (lean; the owner's loop definition is wave58/loop-as-envisioned.md; wave 58 adopted 14 of 14 proposals and the owner flagged that as the busywork pattern; waves 59-61 adopted 0, wave 62 adopted 1 (R331) — REJECT is the expected verdict for anything not paid for by a decision the corpus shows going wrong)

Repo /home/magi/Projects/wagicGPT, game dir projects/mtg. Use python for translogs; use
/usr/bin/grep (bare grep drops matches). DO NOT edit anything under bin/Res, src/, or run git or
wagic; write only under projects/mtg/strategy-design/wave63/. Never write mechanisms as the
owner's positions. Report what you did NOT check.

CORPUS (wave 63, binary = master 98d13050f = wave-62 close 41fbad879 + wave-63 step 1: lanes AB (E1 block-side
life loop fail-closed; E3 survivable-block suggestion ranks blocker material; E4 GANG BLOCK verdict hoisted + summed
trigger prices), AC (E2 CLEANUP PRICE folds stacked draws + cast life delta; E5 manland once per permanent / best rung;
E9 crack-back relief clause wired to describeAction; E13/E15/E16), AD (E6 latched line on record + plan-bound answer
selection + `system` record carrying the protocol hash; E7 X decline reason; E10 per-seam HOLD + phase clause out of
the key; E11 reveal `choice_meaning`; E14 plan scratchpad cut + absent-permanent withdrawal), AE (E8 witness
printed-vs-granted; E12 Nadaar lord(other creature); E17 suite fixed dt + pinned seed; E18 taps clause), AF (the nine
Codex-review findings: X cap of 50 removed, hold row states the two-main-phase span, spare-land ceiling, plan cut
keeps operative "Wait", ownership-specific absent-permanent guard, blocker material lexicographic, relief clause
recomputes statics, DrawAction pending draws, body-phase timeout = timeout; plus E19 = eight "double the counters"
cards rewritten as ONE counter event, incl. Mossborn Hydra 76e4f58b9 — Ascendant Acolyte still has the N-event loop,
deliberately unchanged); read wave63/known-bugs.md, wave63/lane-{AB,AC,AD,AE,AF}.md and wave63/codex-review.md for what
changed and each lane's falsifiable predictions. Also open (owner-reported on the Vita, NOT a corpus item):
wave63/softlock-blockers-1788650768.md — a human defender could assign/cancel blockers but never advance; if a corpus
game shows an AI seat stuck at Blockers with a calm stack, cite it there): /home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-191148/ (21 games: 19 natural, 0 crashed, 2 HUNG — 152v162 and 152v130, the daybound marker loop, see wave63/corpus-hang.md; fixed on master 0026a9d87 and RERUN as single games in /home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-214946/ (152v162, natural, 162 won) and /home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-215345/ (152v130, natural, 152 won) — read those for the 152v162 and 152v130 pairings and treat the hung games' partial translogs as evidence of the hang only). Binary for the 19 = 98d13050f; for the reruns = 0026a9d87 (differs only by the marker-count fix).
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
wave62/general-strategy.md (wave-59 edition + R331) + the reply protocol in the prompt), find issues in the game, the
interface, and the guide, and surface anything else worth the core loop's attention.
Method: for each of your deck's 6 games (your seat's translog; the opponent's for context),
trace the decisions that decided the game. Classify each misplay PERCEPTION (the model misread a
true surface, or the surface was false/missing -> ENGINE/RENDER item, cite file+seq) vs STRATEGY
(read correctly, decided against the guide or against good play -> guide item). Note fallbacks,
re-asks, and any decision where the prompt's information was insufficient or wasteful (what a
better interface would have shown). Hands are real (mulligans, bottoming): do not read variance
as guide effect.
Output: ONE file, wave63/deck<N>/review.md: (1) game-by-game outcome + the deciding decisions with
seq citations; (2) engine/interface/card items ranked HIGH/MED/LOW with a concrete repro (file,
seq, the rendered line) — this is the primary deliverable; (3) guide verdict: KEEP as is (a
success verdict, state why) or EDIT — if EDIT, also write wave63/deck<N>/strategy.txt (the full
revised guide, start from the live one) and list each edit before->after with the seq that
paid for it; keep the guide within the pool band (41-71 KB); (4) OPTIONAL proposals, only when
your evidence supports them: general-guide changes (wave63/deck<N>/general-proposals.md) and
strategy-writing-skill changes (wave63/deck<N>/skill-proposals.md; current skill edition =
wave59/strategy-writing-skill.md, amendments 1-331 — read it before proposing; a proposal that
restates an existing amendment is noise). No prediction tables, no rotation evidence, no
docket bookkeeping. Final message: <= 10 lines.

## Engine-narration seat (one agent)
You read the whole corpus for the engine's sake, not the guides'. Deliverable wave63/engine-seat.md:
(1) corpus census: games natural/hung/crashed, decisions, fallbacks by kind, stale drops (`async_drops`), transport failures (`transport` field) and
`transport_error` retries, timeouts, wall misses, HOLD renders/takes
(prefix-match), SEGV/abort/hang (report whether the wave-58/59 signatures recur: `empty_reply`, `transport_error`, `bad_reply`, `deadline_pct > 100`, a translog gap past the watchdog, `reveal_stall_forced`); (2) adjudicate EVERY prediction in wave63/lane-{AB,AC,AD,AE,AF}.md as PASS / FAIL / UNTESTED (UNTESTED is a real verdict when no window arose; never
infer) — cite counts N of M with file+seq;
(3) engine/render/narration defects ranked HIGH/MED/LOW with repro (file, seq, the rendered
line) — things a per-deck reader would not see because they cut across decks: false verdicts,
contradictory headers, missing facts at decision time, narration that lies, parser refusals;
(4) the owner's lategame specimen (standing rule): pick ONE late-turn decision (turn >= 25,
preferably a decision that mattered) and write its FULL `prompt` verbatim to
wave63/lategame-specimen.txt with a 3-line header (file, seq, turn, why this one). Final
message: <= 15 lines, the HIGH items first.

## Synthesis (runs only if any deck agent wrote general-proposals.md or skill-proposals.md)
Read all proposals + engine-seat.md. Decide each proposal: ADOPT (edit wave63/general-strategy.md,
a copy of wave62's (the R331 edition), or append a numbered amendment to wave63/strategy-writing-skill.md, a
byte-verbatim copy of wave59's with amendments appended — verify the prefix with cmp) or
REJECT with one line of reasons. Write wave63/synthesis-notes.md. If a deck agent's guide edit
contradicts an adopted general change, note it; do not edit deck guides yourself.

METHOD NOTE (from the wave-59 engine seat): the translog `phase` field is not the rendered phase — count phase-gated facts from the prompt text.
