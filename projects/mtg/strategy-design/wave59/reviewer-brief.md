# Wave-59 step-3 brief (lean; the owner's loop definition is wave58/loop-as-envisioned.md; wave 58 adopted 14 of 14 proposals — the owner flagged that rate as the busywork pattern, so REJECT is the expected verdict for anything not paid for by a decision the corpus shows going wrong)

Repo /home/magi/Projects/wagicGPT, game dir projects/mtg. Use python for translogs; use
/usr/bin/grep (bare grep drops matches). DO NOT edit anything under bin/Res, src/, or run git or
wagic; write only under projects/mtg/strategy-design/wave59/. Never write mechanisms as the
owner's positions. Report what you did NOT check.

CORPUS (wave 59, binary = master 5039427da = wave-58 close + wave-59 step 1: lanes I (K4/K5/K7 render
verdicts), J (K8 own-turn crack-back, K9 PUT gloss, K10 identical-ask latch), H (K1/F3 connect timeout +
transport retry + `transport` field), and the wave-58 residual lanes G (F2 reveal-driver hang exit,
`reveal_stall_forced`), F (F1 menu-row identity), K (K6 granted-ability extra costs); read
wave59/known-bugs.md, wave59/lane-{I,J,H,K}.md and wave58/lane-{F,G}.md for what changed and each lane's
falsifiable predictions): /home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-130044/ (21 games, 21 natural, 0 crashed, 0 hung).
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
wave58/general-strategy.md + the reply protocol in the prompt), find issues in the game, the
interface, and the guide, and surface anything else worth the core loop's attention.
Method: for each of your deck's 6 games (your seat's translog; the opponent's for context),
trace the decisions that decided the game. Classify each misplay PERCEPTION (the model misread a
true surface, or the surface was false/missing -> ENGINE/RENDER item, cite file+seq) vs STRATEGY
(read correctly, decided against the guide or against good play -> guide item). Note fallbacks,
re-asks, and any decision where the prompt's information was insufficient or wasteful (what a
better interface would have shown). Hands are real (mulligans, bottoming): do not read variance
as guide effect.
Output: ONE file, wave59/deck<N>/review.md: (1) game-by-game outcome + the deciding decisions with
seq citations; (2) engine/interface/card items ranked HIGH/MED/LOW with a concrete repro (file,
seq, the rendered line) — this is the primary deliverable; (3) guide verdict: KEEP as is (a
success verdict, state why) or EDIT — if EDIT, also write wave59/deck<N>/strategy.txt (the full
revised guide, start from the live one) and list each edit before->after with the seq that
paid for it; keep the guide within the pool band (41-71 KB); (4) OPTIONAL proposals, only when
your evidence supports them: general-guide changes (wave59/deck<N>/general-proposals.md) and
strategy-writing-skill changes (wave59/deck<N>/skill-proposals.md; current skill edition =
wave58/strategy-writing-skill.md, amendments 1-331 — read it before proposing; a proposal that
restates an existing amendment is noise). No prediction tables, no rotation evidence, no
docket bookkeeping. Final message: <= 10 lines.

## Engine-narration seat (one agent)
You read the whole corpus for the engine's sake, not the guides'. Deliverable wave59/engine-seat.md:
(1) corpus census: games natural/hung/crashed, decisions, fallbacks by kind, stale drops (`async_drops`), transport failures (new `transport` field, lane H) and
`transport_error` retries, timeouts, wall misses, HOLD renders/takes
(prefix-match), SEGV/abort/hang (the wave-58 findings F1-F3 shipped fixes this wave — report whether their signatures recur: `empty_reply` at ~2.5 s, a translog gap past the watchdog, a buildMenuChoice abort); (2) adjudicate EVERY prediction in wave59/lane-{I,J,H,K}.md and
wave58/lane-{F,G}.md as PASS / FAIL / UNTESTED (UNTESTED is a real verdict when no window arose; never
infer) (D22 is closed; skip it) — cite counts N of M with file+seq;
(3) engine/render/narration defects ranked HIGH/MED/LOW with repro (file, seq, the rendered
line) — things a per-deck reader would not see because they cut across decks: false verdicts,
contradictory headers, missing facts at decision time, narration that lies, parser refusals;
(4) the owner's lategame specimen (standing rule): pick ONE late-turn decision (turn >= 25,
preferably a decision that mattered) and write its FULL `prompt` verbatim to
wave59/lategame-specimen.txt with a 3-line header (file, seq, turn, why this one). Final
message: <= 15 lines, the HIGH items first.

## Synthesis (runs only if any deck agent wrote general-proposals.md or skill-proposals.md)
Read all proposals + engine-seat.md. Decide each proposal: ADOPT (edit wave59/general-strategy.md,
a copy of wave58's, or append a numbered amendment to wave59/strategy-writing-skill.md, a
byte-verbatim copy of wave58's with amendments appended — verify the prefix with cmp) or
REJECT with one line of reasons. Write wave59/synthesis-notes.md. If a deck agent's guide edit
contradicts an adopted general change, note it; do not edit deck guides yourself.
