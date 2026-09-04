# Wave-58 step-3 brief (lean; the owner's loop definition is wave58/loop-as-envisioned.md)

Repo /home/magi/Projects/wagicGPT, game dir projects/mtg. Use python for translogs; use
/usr/bin/grep (bare grep drops matches). DO NOT edit anything under bin/Res, src/, or run git or
wagic; write only under projects/mtg/strategy-design/wave58/. Never write mechanisms as the
owner's positions. Report what you did NOT check.

CORPUS (wave 58, binary = master a3f0700d3 = wave-57 close + wave-58 step 1 lanes A-E; read
wave58/lane-brief.md and wave58/lane-{A,B,C,D,E}.md for what changed and each lane's
falsifiable predictions): ~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-015059/ (21 games; 20
natural, 1 HUNG = deck152 vs deck126, engine defect F2 in wave58/engine-findings.md — its two
translogs are partial; its rerun is in /home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-042235/). One JSONL per SEAT (filename
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
wave57/general-strategy.md + the reply protocol in the prompt), find issues in the game, the
interface, and the guide, and surface anything else worth the core loop's attention.
Method: for each of your deck's 6 games (your seat's translog; the opponent's for context),
trace the decisions that decided the game. Classify each misplay PERCEPTION (the model misread a
true surface, or the surface was false/missing -> ENGINE/RENDER item, cite file+seq) vs STRATEGY
(read correctly, decided against the guide or against good play -> guide item). Note fallbacks,
re-asks, and any decision where the prompt's information was insufficient or wasteful (what a
better interface would have shown). Hands are real (mulligans, bottoming): do not read variance
as guide effect.
Output: ONE file, wave58/deck<N>/review.md: (1) game-by-game outcome + the deciding decisions with
seq citations; (2) engine/interface/card items ranked HIGH/MED/LOW with a concrete repro (file,
seq, the rendered line) — this is the primary deliverable; (3) guide verdict: KEEP as is (a
success verdict, state why) or EDIT — if EDIT, also write wave58/deck<N>/strategy.txt (the full
revised guide, start from the live one) and list each edit before->after with the seq that
paid for it; keep the guide within the pool band (41-71 KB); (4) OPTIONAL proposals, only when
your evidence supports them: general-guide changes (wave58/deck<N>/general-proposals.md) and
strategy-writing-skill changes (wave58/deck<N>/skill-proposals.md; current skill edition =
wave57/strategy-writing-skill.md, amendments 1-329 — read it before proposing; a proposal that
restates an existing amendment is noise). No prediction tables, no rotation evidence, no
docket bookkeeping. Final message: <= 10 lines.

## Engine-narration seat (one agent)
You read the whole corpus for the engine's sake, not the guides'. Deliverable wave58/engine-seat.md:
(1) corpus census: games natural/hung/crashed, decisions, fallbacks by kind, stale drops (now
stamped `async_drops` with arm/drift/outcome — lane C), timeouts, wall misses, HOLD renders/takes
(prefix-match), SEGV/abort/hang (F1 and F2 in wave58/engine-findings.md are known — add what the
translogs show about F2's last minutes); (2) adjudicate EVERY prediction in wave58/lane-A.md ..
lane-E.md as PASS / FAIL / UNTESTED (UNTESTED is a real verdict when no window arose; never
infer), plus wave57/engine-ledger.md D22 (lane V's four blocker predictions; the corpus ran with
WAGIC_BLOCKLOG=1 — find its output in the game stderr files) — cite counts N of M with file+seq;
(3) engine/render/narration defects ranked HIGH/MED/LOW with repro (file, seq, the rendered
line) — things a per-deck reader would not see because they cut across decks: false verdicts,
contradictory headers, missing facts at decision time, narration that lies, parser refusals;
(4) the owner's lategame specimen (standing rule): pick ONE late-turn decision (turn >= 25,
preferably a decision that mattered) and write its FULL `prompt` verbatim to
wave58/lategame-specimen.txt with a 3-line header (file, seq, turn, why this one). Final
message: <= 15 lines, the HIGH items first.

## Synthesis (runs only if any deck agent wrote general-proposals.md or skill-proposals.md)
Read all proposals + engine-seat.md. Decide each proposal: ADOPT (edit wave58/general-strategy.md,
a copy of wave57's, or append a numbered amendment to wave58/strategy-writing-skill.md, a
byte-verbatim copy of wave57's with amendments appended — verify the prefix with cmp) or
REJECT with one line of reasons. Write wave58/synthesis-notes.md. If a deck agent's guide edit
contradicts an adopted general change, note it; do not edit deck guides yourself.
