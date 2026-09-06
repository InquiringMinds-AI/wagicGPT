# Wave-65 step-3 brief (lean; the owner's loop definition is wave58/loop-as-envisioned.md; wave 58 adopted 14 of 14 proposals and the owner flagged that as the busywork pattern; waves 59-61 adopted 0, wave 62 adopted 1 (R331), waves 63-64 adopted 0 — REJECT is the expected verdict for anything not paid for by a decision the corpus shows going wrong)

Repo /home/magi/Projects/wagicGPT, game dir projects/mtg. Use python for translogs; use
/usr/bin/grep (bare grep drops matches). DO NOT edit anything under bin/Res, src/, or run git or
wagic; write only under projects/mtg/strategy-design/wave65/. Never write mechanisms as the
owner's positions. Report what you did NOT check.

CORPUS (wave 65, binary = master 0f5e0c7be = wave-64 close 537070ac2 + wave-65 step 1: lanes AL (G1 ROOT CAUSE: modal
"choose one" rows arrive as CHOOSE_MENU with isMultipleChoice=0, never CHOOSE_MODE, and ctx is NULL there — pricing now
rides the subject's script snapshot via DecisionRequest::contextText and one pure modeRowAnnotations serving both seams;
G4 crack-back survival verdict withheld on a FLOOR and the cover gate no longer keyed on the floor; G5 own draw step no
longer billed as life loss under DRAW PUNISHERS: yours; G9 pass row carries `{CLEANUP PRICE OF PASSING: ...}` own turn
only), AM (G2 ENGINE: applyDeclareAttackers answered the walker menu under `if (al->menuObject)` which is always NULL
that tick, so A#>W# attacked the player and then toggled out — now answered by identity; suite driver `aideclareattack`;
W-row states a planeswalker cannot block; G3 granted-ability actor now walks the storedSourceCard ladder; G7 declineCap=2
replaced by a board-scoped re-opener mPassDeclineBoard — declines stand only while the board key, phase header included,
is unchanged), AN (G6 the life-LOOP clause is composed from the same kill computation as the attack total and reads
`LETHAL COMES FIRST`, printed only where a chain branch is reachable under lifeLoopProvenWin; G10 blockers header
"no better material" scoped to the lone-block search, an already-assigned blocker excluded per CR 509.1a with its block
folded, the named block's lifelink folded; forced-sacrifice tie-break faces whoever is paid), AO (G8 the reply rule
written once in findAnswerLabelLine: FIRST coded answer line wins, a later head supersedes only as an EXPLICIT marked
CORRECTION, plan harvested from the answer's own block, `later_answer_ignored` recorded, number/name conflict ->
`index_name_conflict` re-ask; kReplyProtocol states first-wins + the CORRECTION marker and the 400-char plan carry cap;
`{feeds:}` states the symmetric half), and AP (the Codex review: all 7 confirmed + fixed — decline scope keeps the phase
header, ATTACK/BLOCKS drivers go through the same first-wins selector via pure gptAttackLineFromReply /
gptBlocksLineFromReply, a marked correction adjacent to its original is detected before run folding, LETHAL COMES FIRST
and the no-block-saves-you verdict fold blockers' lifelink with double strike counted twice, curl 28 short of the ≥95%
deadline band is `transport_error` with the bounded remainder retry, plan carry bounded in UTF-8 code points); read
wave65/known-bugs.md, wave65/lane-{AL,AM,AN,AO,AP}.md and wave65/codex-review.md for what changed and each lane's
falsifiable predictions. Lane-flagged questions for the engine seat: G7's re-opener now re-offers on every phase advance
— measure the re-offer/decline churn (AP: cost unmeasured); AO's first-wins rule accepted that 130v146 s47's late UNMARKED
improvement no longer runs — count how often a later unmarked coded line would have been the better answer
(`later_answer_ignored` records); AL's G4 withholds a ceiling NUMBER on floor rows — is a decision visibly starved of it?
Also: 162v123 ended with deck162 at life -1089 (turn 11) — identify the loop and whether the loop verdicts printed before
it fired): /home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-033121/ (21 games: 21 natural, 0 crashed, 0 hung; 2339 decisions, 0 heuristic fallbacks, 8 re-asks (3 named_row_reask, 3 index_name_conflict, 1 stale_echo, 1 no_pass_reask); 152 5/6, 162 4/6, 125 4/6, 123 3/6, 146 2/6, 126 2/6, 130 1/6).
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
wave62/general-strategy.md (wave-59 edition + R331; NOTE from the wave-63/64 syntheses: this file is NOT loaded at runtime (only deck<N>_strategy.txt is opened, AIPlayerGPT.cpp:15332) — the deck guide is the live surface) + the reply protocol in the prompt), find issues in the game, the
interface, and the guide, and surface anything else worth the core loop's attention.
Method: for each of your deck's 6 games (your seat's translog; the opponent's for context),
trace the decisions that decided the game. Classify each misplay PERCEPTION (the model misread a
true surface, or the surface was false/missing -> ENGINE/RENDER item, cite file+seq) vs STRATEGY
(read correctly, decided against the guide or against good play -> guide item). Note fallbacks,
re-asks, and any decision where the prompt's information was insufficient or wasteful (what a
better interface would have shown). Hands are real (mulligans, bottoming): do not read variance
as guide effect.
Output: ONE file, wave65/deck<N>/review.md: (1) game-by-game outcome + the deciding decisions with
seq citations; (2) engine/interface/card items ranked HIGH/MED/LOW with a concrete repro (file,
seq, the rendered line) — this is the primary deliverable; (3) guide verdict: KEEP as is (a
success verdict, state why) or EDIT — if EDIT, also write wave65/deck<N>/strategy.txt (the full
revised guide, start from the live one) and list each edit before->after with the seq that
paid for it; keep the guide within the pool band (41-71 KB); (4) OPTIONAL proposals, only when
your evidence supports them: general-guide changes (wave65/deck<N>/general-proposals.md) and
strategy-writing-skill changes (wave65/deck<N>/skill-proposals.md; current skill edition =
wave59/strategy-writing-skill.md, amendments 1-331 — read it before proposing; a proposal that
restates an existing amendment is noise). No prediction tables, no rotation evidence, no
docket bookkeeping. Final message: <= 10 lines.

## Engine-narration seat (one agent)
You read the whole corpus for the engine's sake, not the guides'. Deliverable wave65/engine-seat.md:
(1) corpus census: games natural/hung/crashed, decisions, fallbacks by kind, stale drops (`async_drops`), transport failures (`transport` field) and
`transport_error` retries, timeouts, wall misses, HOLD renders/takes
(prefix-match), SEGV/abort/hang (report whether the wave-58/59 signatures recur: `empty_reply`, `transport_error`, `bad_reply`, `deadline_pct > 100`, a translog gap past the watchdog, `reveal_stall_forced`); (2) adjudicate EVERY prediction in wave65/lane-{AL,AM,AN,AO,AP}.md as PASS / FAIL / UNTESTED (UNTESTED is a real verdict when no window arose; never
infer) — cite counts N of M with file+seq;
(3) engine/render/narration defects ranked HIGH/MED/LOW with repro (file, seq, the rendered
line) — things a per-deck reader would not see because they cut across decks: false verdicts,
contradictory headers, missing facts at decision time, narration that lies, parser refusals;
(4) the owner's lategame specimen (standing rule): pick ONE late-turn decision (turn >= 25,
preferably a decision that mattered) and write its FULL `prompt` verbatim to
wave65/lategame-specimen.txt with a 3-line header (file, seq, turn, why this one). Final
message: <= 15 lines, the HIGH items first.

## Synthesis (runs only if any deck agent wrote general-proposals.md or skill-proposals.md)
Read all proposals + engine-seat.md. Decide each proposal: ADOPT (edit wave65/general-strategy.md,
a copy of wave62's (the R331 edition; waves 63-64 wrote no edition), or append a numbered amendment to wave65/strategy-writing-skill.md, a
byte-verbatim copy of wave59's with amendments appended — verify the prefix with cmp) or
REJECT with one line of reasons. Write wave65/synthesis-notes.md. If a deck agent's guide edit
contradicts an adopted general change, note it; do not edit deck guides yourself.

METHOD NOTE (from the wave-59 engine seat): the translog `phase` field is not the rendered phase — count phase-gated facts from the prompt text.
