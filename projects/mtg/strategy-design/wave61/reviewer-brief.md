# Wave-61 step-3 brief (lean; the owner's loop definition is wave58/loop-as-envisioned.md; wave 58 adopted 14 of 14 proposals and the owner flagged that as the busywork pattern; wave 59 adopted 0 of 1, wave 60 had no proposals — REJECT is the expected verdict for anything not paid for by a decision the corpus shows going wrong)

Repo /home/magi/Projects/wagicGPT, game dir projects/mtg. Use python for translogs; use
/usr/bin/grep (bare grep drops matches). DO NOT edit anything under bin/Res, src/, or run git or
wagic; write only under projects/mtg/strategy-design/wave61/. Never write mechanisms as the
owner's positions. Report what you did NOT check.

CORPUS (wave 61 RERUN — the first run on 5faa9e970 livelocked and was killed, see wave61/corpus-livelock.md; binary = master 8097c12cc = 5faa9e970 + the slot-key fix = wave-60 close + wave-61 step 1: lanes R (C1 ATTACK TOTAL
resulting-life honesty, C2 blockers-ask verdict, C3 CRACK-BACK floor/ceiling + evasion, C4 ON THE STACK
prices anytarget abilities aimed at the pilot), S (C5 loop pair, C6 sequential DRAW PRICE fold, C10 best-X
NET badge, C12 one-shot draw grant converter/punisher price), T (C7 hand castability REASON + land enters
tapped, C8 choose-ONE reveal arity, C9 dungeon ventures priced to completion, C11 `{spare: ...}` conditioned),
U (C10 sweeper CAST row rank + damage-wipe survivors, C13 transport phase + deadline field, C14 HOLD across
the turn boundary + per-menu re-open rule), and V (the eight Codex-review fixes: mandatory one-target
chooser no longer offered `PUT: none`, HOLD re-opens on ANY row change, enters-tapped reads the land's own
ETB line only, late-HTTP-error retry budget, wipe survivors read protection/prevention, crack-back
sources must be activatable, no 4-branch cap on modal draw scans, blocking-lifegain ceiling uses the
pairwise can-block gate); read wave61/known-bugs.md, wave61/lane-{R,S,T,U,V}.md and
wave61/codex-review.md for what changed and each lane's falsifiable predictions):
/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-092408/ (21 games, 21 natural, 0 crashed, 0 hung; 2,035 decisions, 6 fallbacks = 0.3%, 16 stale drops; 1.4 h).
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
wave59/general-strategy.md + the reply protocol in the prompt), find issues in the game, the
interface, and the guide, and surface anything else worth the core loop's attention.
Method: for each of your deck's 6 games (your seat's translog; the opponent's for context),
trace the decisions that decided the game. Classify each misplay PERCEPTION (the model misread a
true surface, or the surface was false/missing -> ENGINE/RENDER item, cite file+seq) vs STRATEGY
(read correctly, decided against the guide or against good play -> guide item). Note fallbacks,
re-asks, and any decision where the prompt's information was insufficient or wasteful (what a
better interface would have shown). Hands are real (mulligans, bottoming): do not read variance
as guide effect.
Output: ONE file, wave61/deck<N>/review.md: (1) game-by-game outcome + the deciding decisions with
seq citations; (2) engine/interface/card items ranked HIGH/MED/LOW with a concrete repro (file,
seq, the rendered line) — this is the primary deliverable; (3) guide verdict: KEEP as is (a
success verdict, state why) or EDIT — if EDIT, also write wave61/deck<N>/strategy.txt (the full
revised guide, start from the live one) and list each edit before->after with the seq that
paid for it; keep the guide within the pool band (41-71 KB); (4) OPTIONAL proposals, only when
your evidence supports them: general-guide changes (wave61/deck<N>/general-proposals.md) and
strategy-writing-skill changes (wave61/deck<N>/skill-proposals.md; current skill edition =
wave59/strategy-writing-skill.md, amendments 1-331 — read it before proposing; a proposal that
restates an existing amendment is noise). No prediction tables, no rotation evidence, no
docket bookkeeping. Final message: <= 10 lines.

## Engine-narration seat (one agent)
You read the whole corpus for the engine's sake, not the guides'. Deliverable wave61/engine-seat.md:
(1) corpus census: games natural/hung/crashed, decisions, fallbacks by kind, stale drops (`async_drops`), transport failures (new `transport` field, lane H) and
`transport_error` retries, timeouts, wall misses, HOLD renders/takes
(prefix-match), SEGV/abort/hang (report whether the wave-58/59 signatures recur: `empty_reply`, `transport_error`, `bad_reply`, `deadline_pct > 100`, a translog gap past the watchdog, `reveal_stall_forced`); (2) adjudicate EVERY prediction in wave61/lane-{R,S,T,U,V}.md as PASS / FAIL / UNTESTED (UNTESTED is a real verdict when no window arose; never
infer) (D22 is closed; skip it) — cite counts N of M with file+seq;
(3) engine/render/narration defects ranked HIGH/MED/LOW with repro (file, seq, the rendered
line) — things a per-deck reader would not see because they cut across decks: false verdicts,
contradictory headers, missing facts at decision time, narration that lies, parser refusals;
(4) the owner's lategame specimen (standing rule): pick ONE late-turn decision (turn >= 25,
preferably a decision that mattered) and write its FULL `prompt` verbatim to
wave61/lategame-specimen.txt with a 3-line header (file, seq, turn, why this one). Final
message: <= 15 lines, the HIGH items first.

## Synthesis (runs only if any deck agent wrote general-proposals.md or skill-proposals.md)
Read all proposals + engine-seat.md. Decide each proposal: ADOPT (edit wave61/general-strategy.md,
a copy of wave59's (wave 60 wrote no edition), or append a numbered amendment to wave61/strategy-writing-skill.md, a
byte-verbatim copy of wave59's with amendments appended — verify the prefix with cmp) or
REJECT with one line of reasons. Write wave61/synthesis-notes.md. If a deck agent's guide edit
contradicts an adopted general change, note it; do not edit deck guides yourself.

METHOD NOTE (from the wave-59 engine seat): the translog `phase` field is not the rendered phase — count phase-gated facts from the prompt text.
