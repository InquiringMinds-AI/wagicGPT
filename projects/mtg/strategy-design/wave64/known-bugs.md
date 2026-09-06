# Wave-64 step-one input: KNOWN BUGS from the wave-63 evaluation (orchestrator list; details in the cited files)

Corpus: `matchups-20260905-191148` (19 natural + 2 HUNG 152v162/152v130 = daybound marker loop, fixed 0026a9d87,
`wave63/corpus-hang.md`) + reruns `matchups-20260905-214946` (152v162) and `matchups-20260905-215345` (152v130).
Sources: `wave63/deck{146,152,125,126,162,123,130}/review.md`, `wave63/engine-seat.md`, `wave63/synthesis-notes.md`.
Adjudication (engine seat): 12 PASS, 5 FAIL (E6b, E14a, E4b-by-letter, E10-by-letter, E13 string), 11 UNTESTED, 2 mixed.
Guides: 125/152/130/123/162 EDIT installed; 146/126 KEEP. Numbers below are the seats' own; cite their files.

## HIGH — decisions lost to a false or missing claim
- **F1 "Best X" verdict ignores the library (deck125 HIGH-1).** 125v126 seq 527/528: `[<- best X for this cast: X=20 ...
  no listed X does more]` on a 12-card library; the seat took X=20 from 70 life, 47 ahead, and decked itself. The
  verdict is computed from mana alone. Clamp to library (minus one per untapped Staff-style refill) and print the ceiling.
- **F2 `kNoPassRowFact` printed on windows that DO offer a pass (deck125 HIGH-2).** 680 of 843 windows offering
  `Cast nothing`/`Hold priority` carry "this ask has no pass row" (`AIPlayerGPT.cpp:307`, selected `:28679`); wave-62's
  `declineRowOffered` is passed by the menu family only — the cast seam (`:30706`) and priority seam never set it.
  deck125 HIGH-3 hangs off it: 390 tagged windows, 18 holds (4.6%); 53% of decisions were no-ops.
- **F3 Playing a pathway land's FRONT face consumes the rest of main phase 1 (deck152 HIGH-1, engine).** Corpus-wide:
  front face -> 0 of 15 same-turn main-1 windows; back face -> 5 of 6. A/B 130-rerun seq 6 vs 126 seq 6. Cost 9 damage at
  152v125 seq 74 (no attackers window that turn), pushed Intrepid Adversary to main 2 at 152v162 seq 16 (lost, opp at 3).
- **F4 No prompt ever offers attacking a planeswalker (deck152 HIGH-2).** 0/N attackers blocks mention one though the
  engine supports it. 152v162 seq 17: Ob Nixilis at 3 loyalty, opponent creatureless, seat had 3 unblockable attackers;
  Ob Nixilis dealt 6 of the damage that killed the seat.
- **F5 Bounce guidance on a destroy spell with no decline row (deck152 HIGH-3).** `AIPlayerGPT.cpp:33893-33898`
  unconditionally says "pick a permanent worth bouncing, or decline"; 152v125 seq 45 had 2 rows, no decline, and the
  spell destroys — the seat killed its own Sigarda ("I must accept this loss").
- **F6 Modal `choice` rows carry only the bare `name(...)` label (deck146 HIGH-1).** 146v152 seq 48 at 1 life: "return
  creature and you draw" is `draw:1 controller && life:-1 controller`; the seat died at 0 with lethal on board, reply
  reading "This attack is lethal." Same class as R293/D1 on an uncovered surface: price and lethality on modal rows.
- **F7 GANG BLOCK verdict suppressed for first-strike/deathtouch attackers (deck146 HIGH-2).** `AIPlayerGPT.cpp:36702-36721`
  computes `need` from raw power vs toughness (3/3 vs a 1/1 deathtoucher -> need=1) and `gangBlockPriceTag` returns ""
  at its `need<2` guard while the 1-on-1 results say the attacker LIVES. 146v152 seq 20/26; 7 boilerplate strings, 0 live.
- **F8 Blockers-header suggestion: life-only, oversold "material", obeyed verbatim (deck123 HIGH-1; deck162 HIGH-1;
  deck152 MED; deck126 MED).** Renders on NOT-lethal headers, copied 3/3 (123 vs146 s11/s15 spent the only token-maker
  under Intruder Alarm; s15 named `your blocker dies, attacker lives` over `you kill it, your blocker lives`). 162 rerun
  seq 18: the named block kills a punisher and hands 5 lifelink life, contradicting the same prompt's `KILLS them` draw
  forecast. 146v152 seq 18: pure chump labelled "chosen for your blockers' material" (`AIPlayerGPT.cpp:19488`). 126
  vs152 seq 13: best-case projection excludes the block-trigger gain the next line advertises.
- **F9 Blockers header contradicts itself left-to-right (deck123 HIGH-2).** vs152 s31: `this KILLS you ... you would be
  at 1 AT BEST`; 26 blockers printed; the seat read the headline, answered `BLOCKS: none`, lost a won game.
- **F10 Draw forecast never says the draw step is compulsory (deck130 HIGH-1).** 162 seq 35: seat took Hold at 10
  straight windows under "hold priority to avoid drawing", 16 -> 1 idle. Say the draw is not optional.
- **F11 Castable bodies not tied to the crack-back line (deck130 HIGH-2; MED-3).** 162 seq 43 at 1 life under `would
  KILL you`: `Cast Siege-Gang Commander [castable now]` priced only against mana; seat passed on a confabulated
  punisher trigger (primitive draws nothing). Attackers window never says each attacker is one fewer blocker (rerun
  seq 45, died at -4).
- **F12 Narration inverts the ACTOR on a `targetedplayer`-granted ability (deck126 HIGH).** Your Tribute to Hunger reads
  `Opponent used: Gain life`, theirs `You used:`; 8 occurrences / 5 games; cause at mtg.txt:124075.
- **F13 E6b latch still unauditable (engine HIGH-1/2; E6b FAIL).** 130v126@1788653538 seq29: `latched_line_in_plan:true`
  on a column-0 `ATTACK: A4` after a blank line (a second top-level answer) with no `plan_answer_line_only`;
  `plan_answer_line_ignored` written 0 times; 11/14 `answer_replaced` records carry a `latched_line` absent from the
  trimmed reply (130v123@1788653511 seq30). Record a byte offset; narrow `planParagraphBound`.
- **F14 Reveal wait 561 s / 144,213 ticks with no `reveal_stall_forced` (engine HIGH-3).** 152v146@1788653548 seq31 =
  62% of all reveal wait time in 21 games; the 20 s floor did not release.

## MED
- E9 relief clause now fires (8 records) but leaks `{removes ` into 9 `chosen_text` values (engine).
- E14a: `(Wait,` defeats the plan-cut sentence-start guard (engine FAIL, 1/2,801).
- E13 rename: `; converters` string 30x (lane AC's emitter, declared out of scope); `{feeds:}` row with two clauses both
  ending `in your hand: N` counting different cards (deck162, vs125 seq 9); the live 162 guide quoted the dead literal —
  repaired by the seat's edit. Audit every guide for `converters on your battlefield:` (0/141 prompts).
- "N untapped creatures able to block" unfiltered by evasion, contradicts per-attacker tags (deck162 MED, vs123 seq 16).
- Forced-sacrifice rows don't fold the opponent's Sanguine Bond converter though the attackers prompt does (deck162, vs126 seq 16).
- "choose a card NAME" offers token names (dead rows) and annotates by public-zone visibility (deck146 MED-3, 146v130 seq 14).
- Effect text truncated mid-clause on Teferi's Puzzle Box, hiding the clause the lethal forecast is built from (deck146 MED-4, 146v162 seq 33).
- 8/12 self-targeting Kaya `-3` rows uncollapsed, ~2.6 KB (deck146 MED-5).
- Hold broken by a transient `[this cannot target the spell on the stack]` clause; rows return byte-identical, window
  re-asked, no re-arm (deck130 MED-4, 125 seqs 72-74). Related: 51% of 123's holds were same-turn same-seam re-takes;
  the casting ask fired 19x in one main phase under a token loop (deck123 MED-3). Engine seat: the 13 same-turn re-asks
  all followed legitimate row changes (E10 promise kept).
- 44/44 declined land drops were `[enters TAPPED]` rows; the tag omits next-turn value (deck125 MED-1).
- E4b by the letter: `(1 if all block)` summed price below the tag's largest single price 4 (engine).
- Path to Exile land-choice ask never names the exiled permanent (deck126 LOW). One 900 s wall miss (123 vs130 s11).
- Lightmine Field script triggers `@each blockers`; text says on attack (deck123 LOW-5, card).

## Carried, untested
- Ascendant Acolyte keeps the N-event "double the counters" loop (AF E19 left it deliberately). E1 (block-side loop
  header/rows/decline), E3b, R1/R5+/R7+/R9, E19 had no corpus window.
- `intrepid_adversary_repeated_payment.txt` threaded flake; `mReplayEngineActions` matches stale earlier-turn engine
  actions (`wave63/softlock-blockers-1788650768.md`); Vita: menace keyword visibility at declare-blockers (banner
  349a4d5b4 ships the count; not yet in any VPK).
