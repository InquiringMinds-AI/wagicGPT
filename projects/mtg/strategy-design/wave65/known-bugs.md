# Wave-65 step-one input: KNOWN BUGS from the wave-64 evaluation (orchestrator list; details in the cited files)

Corpus: `matchups-20260906-001533` (21/21 natural, 0 crash/hang, binary dab6a6cba). Sources:
`wave64/deck{146,152,125,126,162,123,130}/review.md`, `wave64/engine-seat.md`, `wave64/synthesis-notes.md`.
Adjudication (engine seat): PASS F2, F3, F4-render, F7-first-strike, F8a/F8c, F9-falsifier, F11, F13a, F14, E14a, hold
bracket, E4b, blocker scope, enters-TAPPED, AK 2/6; FAIL F6, F12, F10-behaviour, F13c, E9-by-letter; UNTESTED F1 emitter,
F5, F8b, F9's new branches, F7 deathtouch half, AK R1/R5/R9. Guides: 125/130/123 EDIT installed; 146/152/126/162 KEEP.
Three shipped wave-64 fixes did NOT reach the live seam (G1-G3) — verify the seam, not the helper, before claiming done.

## HIGH — decisions lost to a false or missing claim
- **G1 F6 never fired (engine HIGH-1; deck146 HIGH-3).** 3 real CHOOSE_MODE windows (Silverquill Command), 23 rows, 0
  `{this mode right now:` — and 0 wave-60 liveness/tap tags either: the whole `ctx`-gated family at
  `AIPlayerGPT.cpp:33348-33394` is silent while the header names the card from `req.contextName`. deck146 took `you draw
  and sacrifice creature` (`life:-1 controller`, borderline.txt:102747) at 5 life and again at 3 (146v130 seq 57/74),
  lost at 0. Find why `ctx` is empty on the `Choose an option for <card>` ask and price the rows there.
- **G2 `A#>W#` deals no damage (engine HIGH-2).** 123v126 seq 25: unblockable 5/5 declared at Sorin (4 loyalty); seq
  26->27 Attackers -> Main 2 with no combat-damage event, Sorin still 4 loyalty at seq 32, opponent life unchanged. 2 of 2
  walker attacks delivered 0. Lane AI's apply path (`MTGPlaneswalkerAttackRule` + its menu) is not binding the attacker
  to the walker; fixture the engine path end to end (declare -> damage -> loyalty). Also (deck152 LOW) the W-row
  section never says a planeswalker cannot block.
- **G3 F12 unchanged (engine HIGH-3).** 126v130 seq 16 prints the lane's own falsifier verbatim (`You cast Tribute to
  Hunger` ... `Opponent used:` ... `You gained 1 life`); 10 lines / 5 events / 3 games. `e->source->controller()` on a
  `targetedplayer` dummy is the grantee — the lane fixed a different seam. Fix the narration seam that actually emits
  `used:` for granted abilities; PARSETEST the corpus line.
- **G4 Crack-back SURVIVE built on a FLOOR (engine HIGH-4; deck123 HIGH-3; deck162 HIGH-2).** 123v152 seq 9:
  `{crack-back cover:}` promised "at 2, which you SURVIVE ... nothing uncounted here overturns it" off a number the same
  prompt called a FLOOR with a named growth trigger; seq 10 read 17 damage; seat died at -8. 162 vs146 seq 12: the
  crack-back line said "up to 7 - you would be at 2 or lower" while naming two 3-power Hives in the same sentence (13
  vs 9 life); the cover gate keys on that floor's lethality, so two castable 5/5 bodies got no cover clause. No survival
  verdict on a floor; the cover clause must be gated on the ceiling.
- **G5 Own draw step billed as life loss under `DRAW PUNISHERS: yours` (engine HIGH-5).** 32 of 191 windows tell the seat
  its own draw costs it life (162v125 seq 14); 159 are correct. Wrong chair on the direction split.
- **G6 Loop-warning clause overrides or contradicts the verdict (deck123 HIGH-1; deck146 HIGH-1; deck162 HIGH-1).** 123
  v126 seq 48: with 51 creatures the screen printed `ATTACK TOTAL ... KILLS them whatever they block` AND stamped every
  unblockable row with the Sanguine Bond/Exquisite Blood "fatal to you" clause — `ATTACK: none`, lost 43-0. 146 vs126
  seq 29: the clause on all 3 attacker rows against a CREATURELESS opponent where the same prompt says every attacker is
  unblockable (neither loop branch reachable: mtg.txt:37967/:99004). 162 vs126 seq 13: `{their converter:}` row priced a
  body `life 21 -> 17` six lines under the LOOP SCOPE paragraph "ANY nonzero payment is fatal" — seat cast it, died from
  21. The per-row arithmetic never consults the loop detector; the clause must yield to a lethal verdict and print only
  where a branch is reachable.
- **G7 Two-decline cap retires legal rows (deck123 HIGH-2; doctrine).** `AIPlayerGPT.cpp` ~28900 `declineCap=2`, no
  re-open: 162 seq 66/69 -> 73 retired the free `{T}` token-maker rows for the turn; when Intruder Alarm resolved in main
  1 the menu held only three equips — the combo could not fire on the turn it assembled. A hard cap on legal choices:
  re-open on any board change, or replace with the declined-this-list tag (which already exists) and no cap.
- **G8 Reply handling flips the executed answer (deck146 HIGH-2; deck130 HIGH-1/2; deck123 MED).** 146 vs130 s93/s101,
  vs162 s39: model wrote `CHOICE: 2 (Cast nothing)`, engine executed row 1 — `answer_replaced:true`,
  `latched_line_offset:6544` vs `plan_block_end:735`, `latched_line_in_plan:false`: 3 of 3 replacements in this deck
  flipped the row, two on a final turn. 130 vs146 seq 47: a two-block reply takes its answer from block 2 but its PLAN
  from block 1 — the discarded reasoning becomes YOUR PLAN for the next windows (11/424). 130 vs162 seq 57: `CHOICE: 0`
  on both lines, record `choice: 4` — number and name disagree and the name wins silently. Decide the rule once: the
  first coded line is the answer unless a later column-0 coded line explicitly corrects it; number/name conflict is a
  re-ask; the plan is harvested from the SAME block the answer came from; every override is recorded.
- **G9 The pass row is the only unpriced row under a punisher (deck125 HIGH-1).** 162 seq 72/73, T17, 12 life, hand 10,
  Final Judgment + Path castable -> `Cast nothing` twice; cleanup billed 6 of 12 life; dead at seq 97. 18/18 punisher
  windows unpriced, 18/18 passed. Price the pass row with the cleanup discard/loss the engine already computes.
- **G10 Blockers header claims (deck152 HIGH-1; deck126 HIGH-1; deck146 MED).** 152v146 seq 13/27: "best case ... no
  better material reaches that life figure" while the next line's `GANG BLOCK:` reaches the same life with one blocker
  and kills the attacker — 0 of 10 suggestions ever name two blockers on one attacker. 126v152 seq 9 (after blockers
  declared): the committed Perimeter Captain counted as still free, its damage subtracted twice, the named assignment
  unreachable (`AIPlayerGPT.cpp:21480` uses `canBlock()`, no already-assigned test). 152 s12: best-case life ignores
  the same block's lifelink gift.

## MED
- `buildForcedSacrificeAsk` (`AIPlayerGPT.cpp:25227`) appends "the one that pays the least" to the gain==2 branch where
  the payment is life YOU gain (126 vs123 seq 10) — inverted, and Sanguine Bond turns that gain into damage.
- PLAN carry cap 400 chars, never stated: 62/271 windows carried the truncation notice at deck126; 126 seq 41 dropped
  5,522 B in which the model concluded "I win"; 146 seq 24 truncation inverted a self-correcting sentence; the 4 slowest
  decisions were long plan prose (see wave64/synthesis-notes.md for the ruling on deck126's proposal).
- Valor-counter ask prints 20 `add N counters` rows the prompt itself calls no-ops; 4/5 windows had zero affordable
  (152v146 s7/s12, 152v162 s7/s11). Elite Spellbinder target window is bare (no tax rider, no decline; Oracle "may")
  (152v123 s11). Kaya `-3` still offers Kaya as her own target 4/4. `Choose a card NAME` ranks the token name `Goblin`
  first by visibility (130 s27). `{text:}` dropped from some reveal rows (152 s22).
- `{feeds:}` never says a symmetric engine also draws YOU (41/41); `draw converters in your hand: N` counts uncastable
  cards (8/8, contradicted by `[cannot pay now]` in the same prompt) (deck162). Ob Nixilis `-2` script label omits "its
  controller" (planeswalkers.txt:2687).
- Plain `Cast nothing` row should withdraw once the declined-this-list tag fires (deck125: 58% no-ops); byte-identical
  menus across six phases where the seat took `Cast nothing right now` over the collapse row (130 vs126 seq 53-58;
  127/297 hold-offering windows answered non-latchingly); F10 behaviour: 9 replies still say "avoid drawing".
- `flyersonly` models "can't attack YOU" (Form of the Dragon, Teferi's Moat — correct) and "can't attack" (Moat, Magus —
  wrong) alike: card-script split, unreachable in this pool (engine seat, lane AK question).
- F13(c) weak clause; E9 by the letter (engine seat).

## Carried, untested
- E6b counter is 0 BY CONSTRUCTION after AK R5 (engine seat) — retire or re-derive the falsifier. Codex #3 (curl 28 connect
  timeout classified as a decision timeout, `GptConfig.cpp:871` / `gptDeadlineMissed`) refuted as out of scope in wave
  64 — a real item for a transport lane. Ascendant Acolyte N-event loop; `intrepid_adversary_repeated_payment` flake;
  `mReplayEngineActions` cross-turn matching; Vita menace visibility (banner not in any VPK). Windows never arose for F1's
  emitter, F5, F8b, F9's new branches, F7's deathtouch half, AK R1/R5/R9.
