# Wave-62 step-3 brief (lean; the owner's loop definition is wave58/loop-as-envisioned.md; wave 58 adopted 14 of 14 proposals and the owner flagged that as the busywork pattern; waves 59-61 adopted 0 — REJECT is the expected verdict for anything not paid for by a decision the corpus shows going wrong)

Repo /home/magi/Projects/wagicGPT, game dir projects/mtg. Use python for translogs; use
/usr/bin/grep (bare grep drops matches). DO NOT edit anything under bin/Res, src/, or run git or
wagic; write only under projects/mtg/strategy-design/wave62/. Never write mechanisms as the
owner's positions. Report what you did NOT check.

CORPUS (wave 62, binary = master 12be0b79c = wave-61 close + wave-62 step 1: lanes W (D1 enters-tapped
classifier: all three script idioms, resolved from the battlefield where decidable; D15 MDFC land face marker;
D17 one land-drop wording), X (D2 no kill claim under an opposing life loop with block gain; D6 Lolth emblem /
next-turn mana gate split; D8 draw forecast tense + KILLS-you verdict + loop win tail; D19 DIES rows priced
against CRACK-BACK), Y (D3 loop caution bound per forecast line — the engine already fires lifelost on damage,
the deck162 claim was refuted; D4 Lair of the Hydra rungs — RankingContainer key collapse fixed, real engine
bug; D5 X-menu decline row; D7 pay-repeat row cost), Z (D9/D10 answer restatement bound to the plan paragraph,
BLOCKS/ATTACK restatement; D11 re-ask names the later-step row; D12 block assignment printed; D13 hold scope;
D14 X-cast remainder tag; D16 plan cut states bytes, Night/Day markers untargetable; D18 wall-miss phase
suffix), and AA (the eight Codex-review fixes: cast-route X decline, negated combat restatement, single loop
forecast line, colour-fit pay-repeat, manland self-mana excluded, loop win only when their life can change,
HTTP status kept on curl failure, 200-empty = empty_reply); read wave62/known-bugs.md,
wave62/lane-{W,X,Y,Z,AA}.md and wave62/codex-review.md for what changed and each lane's falsifiable
predictions): /home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-133058/ (21 games: 20 natural, 0 crashed, 1 HUNG — 152v125, a wave-62 lane-Z regression, see wave62/corpus-hang.md; fixed on master 9230a94c3 and RERUN as a single game in /home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-161801/ (1 game, natural) — read that rerun for the 152v125 pairing and treat the hung game's partial translogs as evidence of the hang only). Binary for the 20 = 12be0b79c; for the rerun = 9230a94c3 (differs only by the marker fix).
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
Output: ONE file, wave62/deck<N>/review.md: (1) game-by-game outcome + the deciding decisions with
seq citations; (2) engine/interface/card items ranked HIGH/MED/LOW with a concrete repro (file,
seq, the rendered line) — this is the primary deliverable; (3) guide verdict: KEEP as is (a
success verdict, state why) or EDIT — if EDIT, also write wave62/deck<N>/strategy.txt (the full
revised guide, start from the live one) and list each edit before->after with the seq that
paid for it; keep the guide within the pool band (41-71 KB); (4) OPTIONAL proposals, only when
your evidence supports them: general-guide changes (wave62/deck<N>/general-proposals.md) and
strategy-writing-skill changes (wave62/deck<N>/skill-proposals.md; current skill edition =
wave59/strategy-writing-skill.md, amendments 1-331 — read it before proposing; a proposal that
restates an existing amendment is noise). No prediction tables, no rotation evidence, no
docket bookkeeping. Final message: <= 10 lines.

## Engine-narration seat (one agent)
You read the whole corpus for the engine's sake, not the guides'. Deliverable wave62/engine-seat.md:
(1) corpus census: games natural/hung/crashed, decisions, fallbacks by kind, stale drops (`async_drops`), transport failures (new `transport` field, lane H) and
`transport_error` retries, timeouts, wall misses, HOLD renders/takes
(prefix-match), SEGV/abort/hang (report whether the wave-58/59 signatures recur: `empty_reply`, `transport_error`, `bad_reply`, `deadline_pct > 100`, a translog gap past the watchdog, `reveal_stall_forced`); (2) adjudicate EVERY prediction in wave62/lane-{W,X,Y,Z,AA}.md as PASS / FAIL / UNTESTED (UNTESTED is a real verdict when no window arose; never
infer) (D22 is closed; skip it) — cite counts N of M with file+seq;
(3) engine/render/narration defects ranked HIGH/MED/LOW with repro (file, seq, the rendered
line) — things a per-deck reader would not see because they cut across decks: false verdicts,
contradictory headers, missing facts at decision time, narration that lies, parser refusals;
(4) the owner's lategame specimen (standing rule): pick ONE late-turn decision (turn >= 25,
preferably a decision that mattered) and write its FULL `prompt` verbatim to
wave62/lategame-specimen.txt with a 3-line header (file, seq, turn, why this one). Final
message: <= 15 lines, the HIGH items first.

## Synthesis (runs only if any deck agent wrote general-proposals.md or skill-proposals.md)
Read all proposals + engine-seat.md. Decide each proposal: ADOPT (edit wave62/general-strategy.md,
a copy of wave59's (waves 60-61 wrote no edition), or append a numbered amendment to wave62/strategy-writing-skill.md, a
byte-verbatim copy of wave59's with amendments appended — verify the prefix with cmp) or
REJECT with one line of reasons. Write wave62/synthesis-notes.md. If a deck agent's guide edit
contradicts an adopted general change, note it; do not edit deck guides yourself.

METHOD NOTE (from the wave-59 engine seat): the translog `phase` field is not the rendered phase — count phase-gated facts from the prompt text.
