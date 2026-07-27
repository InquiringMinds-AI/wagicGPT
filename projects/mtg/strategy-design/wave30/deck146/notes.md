# deck146 -> development notes (wave-30)

Corpus: matchups-20260726-181642, binary 6e168aa05. Seat: deck146, 3/6, 1 fallback.

## LEDGER: RETIRE

### N-146f "no dungeons-completed status line" -> RESOLVED, RETIRE
The g5 render fix is LIVE. CURRENT SITUATION now prints, when >=1 dungeon completed, verbatim:
  Dungeons completed (you): N - effects that check "completed a dungeon" (e.g. a completed-dungeon
  anthem) are ACTIVE for you.
Rendered in vs148 (N=1), vs137 (N=1), vs139 (N=1 then N=2); correctly absent when N=0. The model's
reasoning flipped from wave-29's second-guessing ("the anthem applies *after* completion?") to
confident assertion doing correct combat math (vs148 s21 "I have completed a dungeon, so ... Nadaar
5/5, Adventurer 2/2"; vs139 s37 "1/1 base + 1/+1 anthem + 1/+1 counter = 3/3"). Retire the probe.

## LEDGER: CONFIRM (fixes durable a 2nd corpus)

### f2 dual-land oracle fix -> STILL CLEAN
0 mana-collapse lines across all 6 games. Vanishing Verse fires on curve in the 4 games with mana
(vs122 x8, vs148 x3, vs137 x3, vs139 x2); the 2 zero-games are variance (screw vs136, mulligan-to-6
vs152). Instant windows present.

### f4 dungeon renders -> STILL CLEAN
Lost Mine selected 100% (answer-by-name held; it was option 3 this corpus). 14 room-branch picks,
all value (goblin/storeroom/dark pool), zero scry-only. Completion + anthem exercised in 3 games.

## NEW ENGINE / RENDER ITEMS

### N-146g (RENDER, low) -- magnitude annotation misfires on Lolth's planeswalker options
Lolth, Spider Queen's Cast option and her loyalty (-3 / +0) options render "{right now: damage 8}"
(and later "{right now: damage 4}") -- see vs122 s20/s21/s42 and vs152 s34/s35/s41. Lolth's cast
and her -3/+0 abilities deal no immediate damage; the dynamic-magnitude eval (WParsedInt on
damage/life expressions) is attaching an irrelevant/incorrect number to a planeswalker option,
probably evaluating the -8 emblem's drain text or a death-drain expression out of context. The
model was not derailed (it made correct picks), so this is cosmetic-to-mild, but the number is
wrong and could mislead a future decision (e.g. the model treating "Cast Lolth" as an 8-damage
burn). Repro: any deck146 game that casts Lolth (vs122 seq>=20, vs152 seq>=34). Likely shares a
root with the open "Rankle mode-bleed watch" magnitude-eval class -- worth checking whether the
WParsedInt guard should skip planeswalker loyalty/emblem text the way it skips "rand".

### Over-reasoning spiral watch (MODEL, cross-deck) -- the "invent an off-menu option" shape
deck146's 1 fallback (vs152 s17, unparsed_reply, 210s): the model's PLAN said "play a land", the
priority list offered only a Flip-Side DFC display toggle + pass, and rather than accept "lists are
complete" the model emitted an out-of-range "CHOICE: 2 (Play Plains)" and looped ~3,000 tokens
mis-counting its own board from the log narration (instead of the CURRENT SITUATION snapshot) until
truncated. Correctly rejected -> Baka passed (right answer). Not a parser/engine bug and not a
core-prompt text gap (the "lists are complete" rule is present and was quoted). This is a model
failure mode; log it as a cross-deck watch and revisit for a core item only if it recurs at
multiple seats. Repro: vs152 seq 17.

### Latency tail (HARNESS, informational)
272 real-call decisions: median 16.8s, p90 73s, max 216s; 32 records >60s. Consistent with the
harness's -j server-contention curve (median ~17s at saturation, p90 near the HTTP timeout). Not a
seat issue; noted because the tail is where the lone over-reasoning spiral crossed into a fallback.

## SURFACE VALIDATED THIS WAVE

### Planeswalker (Lolth) decision quality -> VALIDATED (was unexercised 2 corpora)
Lolth cast in vs122 (t12) and vs152 (t8). Loyalty play is guide-perfect: -3 (two 2/1 Spiders) FIRST
to build a board, then +0 (draw, lose 1) for value, used every turn she was out, no idle-walker
leak. Menu rendered cleanly with full {card text} + [cost: Counters]. The walker DECISION SURFACE
is now proven. Kaya specifically was drawn+mulliganed in vs136, never cast -- her -3-exile-any-
nonland is the same loyalty surface (validation transfers; see skill.md P2), so her non-appearance
is draw variance, NOT a distinct unvalidated surface and NOT a hold-justifying gap.

## ROTATION PROJECTION -> NEAR-TERMINAL; wave-31 EXIT projected

deck146 has now cleared its named wave-29 exit criteria: (a) all blocking engine gaps closed and
durable a 2nd corpus (no-cast-window, dungeon selection/branch renders, completion, and now the g5
dungeons-completed status line); (b) the guide FROZE byte-identical this wave (cmp-verified,
8482 bytes, no revision needed -- every wave-29 fact-correction validated live); (c) the last
unvalidated surface, planeswalker play, was EXERCISED with acceptable (guide-adherent) decision
quality via Lolth.

The only residual is Kaya's specific -3-exile never having been drawn-and-cast across three
corpora -- a draw-variance gap on a surface Lolth already validated, not a fixable engine/decision
gap. Under the unsentimental new-work-exhaustion lens, holding a seat another full corpus purely to
hope Kaya randomly resolves is not worth it when the guideless roster (136 next after 152's guide)
needs the pool slots.

RECOMMENDATION: KEEP for ONE more corpus (wave-31) as a NEAR-TERMINAL confirm seat -- the guide is
frozen, so wave-31 only needs to (i) re-confirm the frozen guide holds a 3rd corpus, (ii) confirm
N-146g does not mislead a real decision, and (iii) opportunistically catch Kaya if she's drawn.
If wave-31 confirms the freeze and surfaces no new gap, ROTATE OUT at wave-32 (Kaya treated as
accepted residual variance). This is the reference campaign-first seat (dungeon nav + venture +
completion + walker play all validated live); it has earned an exit, not an open-ended hold.
