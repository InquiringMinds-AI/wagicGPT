# Wave-64 step-3 brief (lean; the owner's loop definition is wave58/loop-as-envisioned.md; wave 58 adopted 14 of 14 proposals and the owner flagged that as the busywork pattern; waves 59-61 adopted 0, wave 62 adopted 1 (R331), wave 63 adopted 0 — REJECT is the expected verdict for anything not paid for by a decision the corpus shows going wrong)

Repo /home/magi/Projects/wagicGPT, game dir projects/mtg. Use python for translogs; use
/usr/bin/grep (bare grep drops matches). DO NOT edit anything under bin/Res, src/, or run git or
wagic; write only under projects/mtg/strategy-design/wave64/. Never write mechanisms as the
owner's positions. Report what you did NOT check.

CORPUS (wave 64, binary = master dab6a6cba = wave-63 close be2e88282 + wave-64 step 1: lanes AG (F6 modal rows priced
like cast rows; F7 gang-block verdict derived with deathtouch/first strike modelled; F8 block suggestion labels its ranking
truthfully, names what it spends, optional on NOT-lethal headers, projection folds block-trigger gain; F9 survival stated
first, bare `this KILLS you` only when no assignment survives), AH (F1 best-X clamped to the library with the ceiling
printed; F2 pass-row fact derived from the rendered rows at every seam; F5 stack-trap verb from the ability, decline
sentence from mayStop; F10 the draw step is stated compulsory; F11 `{crack-back cover:}` on body-adding rows + CRACK-BACK
COST OF ATTACKING), AI (F3 ENGINE: Baka's Act no longer passes priority in the tick a menu was answered in place — the
pathway/MDFC FRONT face used to end main 1; F4 planeswalker/battle attack targets as W# rows + `A#>W#` grammar; F12
granted-ability actor credited to the card's controller; F14 = reporting only, `reveal_wait_driver_*` added), AJ (F13
latch record carries byte offsets, column-0 coded line ends the plan paragraph; six MED renders incl. transient-clause hold
forgiveness and the enters-TAPPED next-turn value), and AK (the Codex review: 8 confirmed + fixed — walker-only attackers
admitted to the candidate set, the F3 pass floor gated on hasAnyLegalAction with `menu_pass_forced` recorded, crack-back
cover folds evasion/CANTBLOCK/enters-tapped, indented top-level corrections recognised, whole `ability$!` block skipped,
gang budget board-sized, upkeep draw reserved only while ahead; #3 transport classification refuted as out of scope);
read wave64/known-bugs.md, wave64/lane-{AG,AH,AI,AJ,AK}.md and wave64/codex-review.md for what changed and each lane's
falsifiable predictions. Two lane-flagged questions for the engine seat: Wagic's `flyersonly` allows planeswalker attacks
where modern CR may not; AK's R5 makes wave-63 E6b's in-plan exclusion unreachable for line-leading coded lines): /home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-001533/ (21 games: 21 natural, 0 crashed, 0 hung; 2295 decisions, 1 fallbacks; 152 6/6, 126 4/6, 162/130/146 3/6, 125 2/6, 123 0/6).
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
wave62/general-strategy.md (wave-59 edition + R331; NOTE from the wave-63 synthesis: this file is not loaded at runtime — the deck guide is the live surface) + the reply protocol in the prompt), find issues in the game, the
interface, and the guide, and surface anything else worth the core loop's attention.
Method: for each of your deck's 6 games (your seat's translog; the opponent's for context),
trace the decisions that decided the game. Classify each misplay PERCEPTION (the model misread a
true surface, or the surface was false/missing -> ENGINE/RENDER item, cite file+seq) vs STRATEGY
(read correctly, decided against the guide or against good play -> guide item). Note fallbacks,
re-asks, and any decision where the prompt's information was insufficient or wasteful (what a
better interface would have shown). Hands are real (mulligans, bottoming): do not read variance
as guide effect.
Output: ONE file, wave64/deck<N>/review.md: (1) game-by-game outcome + the deciding decisions with
seq citations; (2) engine/interface/card items ranked HIGH/MED/LOW with a concrete repro (file,
seq, the rendered line) — this is the primary deliverable; (3) guide verdict: KEEP as is (a
success verdict, state why) or EDIT — if EDIT, also write wave64/deck<N>/strategy.txt (the full
revised guide, start from the live one) and list each edit before->after with the seq that
paid for it; keep the guide within the pool band (41-71 KB); (4) OPTIONAL proposals, only when
your evidence supports them: general-guide changes (wave64/deck<N>/general-proposals.md) and
strategy-writing-skill changes (wave64/deck<N>/skill-proposals.md; current skill edition =
wave59/strategy-writing-skill.md, amendments 1-331 — read it before proposing; a proposal that
restates an existing amendment is noise). No prediction tables, no rotation evidence, no
docket bookkeeping. Final message: <= 10 lines.

## Engine-narration seat (one agent)
You read the whole corpus for the engine's sake, not the guides'. Deliverable wave64/engine-seat.md:
(1) corpus census: games natural/hung/crashed, decisions, fallbacks by kind, stale drops (`async_drops`), transport failures (`transport` field) and
`transport_error` retries, timeouts, wall misses, HOLD renders/takes
(prefix-match), SEGV/abort/hang (report whether the wave-58/59 signatures recur: `empty_reply`, `transport_error`, `bad_reply`, `deadline_pct > 100`, a translog gap past the watchdog, `reveal_stall_forced`); (2) adjudicate EVERY prediction in wave64/lane-{AG,AH,AI,AJ,AK}.md as PASS / FAIL / UNTESTED (UNTESTED is a real verdict when no window arose; never
infer) — cite counts N of M with file+seq;
(3) engine/render/narration defects ranked HIGH/MED/LOW with repro (file, seq, the rendered
line) — things a per-deck reader would not see because they cut across decks: false verdicts,
contradictory headers, missing facts at decision time, narration that lies, parser refusals;
(4) the owner's lategame specimen (standing rule): pick ONE late-turn decision (turn >= 25,
preferably a decision that mattered) and write its FULL `prompt` verbatim to
wave64/lategame-specimen.txt with a 3-line header (file, seq, turn, why this one). Final
message: <= 15 lines, the HIGH items first.

## Synthesis (runs only if any deck agent wrote general-proposals.md or skill-proposals.md)
Read all proposals + engine-seat.md. Decide each proposal: ADOPT (edit wave64/general-strategy.md,
a copy of wave62's (the R331 edition; wave 63 wrote no edition), or append a numbered amendment to wave64/strategy-writing-skill.md, a
byte-verbatim copy of wave59's with amendments appended — verify the prefix with cmp) or
REJECT with one line of reasons. Write wave64/synthesis-notes.md. If a deck agent's guide edit
contradicts an adopted general change, note it; do not edit deck guides yourself.

METHOD NOTE (from the wave-59 engine seat): the translog `phase` field is not the rendered phase — count phase-gated facts from the prompt text.
