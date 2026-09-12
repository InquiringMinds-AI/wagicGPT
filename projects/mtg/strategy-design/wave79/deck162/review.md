# Wave 79 — deck 162 ("Draw and Die!", UB forced-draw punish) per-deck review

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260912-000748-final`, SINGLE binary
`wagic-5af6f7a95-w79step1` (source `b1e1db7be`), `--thinking on`. Six deck162 seat translogs plus the six
opponent seats for outcomes and log.

**149 model decisions on this seat, 0 heuristic fallbacks, 0 `reply_truncated`, 0 `action_before_plan`,
`thinking: on` and native reasoning on 149/149** (median `reasoning_chars` 6,091, max 16,302).
`transport` `curl=0,http=200,empty=0` on all 149; `deadline_pct` max 40.1. `protocol_deviation`:
compliant 138 / `unlabelled_plan` 6 / `plan_absent` 5 / `answer_label_absent` 0 — `plan_line_missing`
11 of 149 = **7.4%**, against the corpus's 4.5%; `off_protocol_bytes` > 0 on 6 windows, max 89 B.
Latency p50 **105.4 s**, p95 231.1, max 361.5 (corpus p50 94.4). Prompt bytes: max **20,199**, median
9,765, **1 of 149 windows over 20 KB** (wave 78: max 19,112, 0 over) — see LOW-1.
Gameend sums over the six games: `hold_windows_skipped` 46 (cast 40 / priority 6), `hold_released_turn` 4,
`own_turn_windows_skipped` 209, `ask_key_continuation_differs` 8, `ask_replays_reserved` 0,
`identical_ask_answers_reserved` 0, `forced_close_events` 6 / `_unrecorded` 3, `async_drops` 1,
`crossphase_identical_reputs` 1, `crossphase_board_unchanged` 0, `blocker_forecast_rows` 6 / `multi` 4 /
`gang` 0 / `collapsed` 0, `chain_windows_only_selfharm` 14 / `chain_acting_rows` 14,
`main_phase_windows_skipped` 1, `menu_pass_no_progress_suppressed` 0, `stop_reached_*` 0,
`stack_drain_windows_asked` 0, `own_loop_windows_asked` 0 / `own_loop_verdict_lines_rendered` 0,
`hold_verdict_safer_ignored` 0 / `hold_reopened_new_threat` 0, `plan_names_stranded_card` 0,
`plan_names_uncastable_zone_card` 0, `repeat_annotated_takes` 0, `phase2_*` 0, `wall_miss_*` 0.

**Record 4–2** (wave 78: 2–4). The two wave-78 losses that were guide-attributable did not recur, and
the deck **beat deck123 — the Thraben Doomsayer token deck that had beaten it in waves 77 and 78 — on
exactly the board wave-78 guide edit A was written for**: 24 creatures, a 49-damage crack-back reading
"that would KILL you", Ob Nixilis cast FIRST and his -2 taken the same main phase (`162v123` seq 16/17).
Both losses this wave are matchup, not decision: a completed Sanguine Bond + Exquisite Blood loop on
deck126's board with a Staff of Nin to trigger it, and a deck152 race the seat was arithmetically dead
in from t11 when its wall was exiled before it could block. **One guide violation in 149 decisions**
(`162v125` seq 41, the legend-rule Ob Nixilis, MED-2) and it did not decide that game.

## 1. Game by game

| Opp | Result | Deciding decisions |
|---|---|---|
| 123 | **WIN** t13, 20–0 | **The wave-78 edit-A board, and the edit paid.** 17 decisions, 1 deviation, 0 fallback. Curve: Mine t3 (seq5), second Mine t5 (seq7), Fate Unraveler t7 (seq9), Shield Sphere t7 (seq10), Underworld Dreams t9 (seq11), second Dreams t11 (seq13), Fog Bank t11 (seq14). At **t13 seq16** their board is 23 Vampire tokens + Bloodline Keeper under Intruder Alarm and the header reads `CRACK-BACK NEXT TURN: 24 of their creatures will be able to attack ... for up to 49 - you would be at -29; that would K[ILL you]`. The seat's line is 6 sources, three Masters and a Fate Unraveler in hand — the wave-78 rung 0 named exactly those bodies and struck Ob Nixilis off by name. The seat instead cast **Ob Nixilis** (seq16, plan: *"add a third draw punisher and activate its -2 ability to destroy a Vampire token, reducing the lethal crack-back threat"*), then **-2 on Bloodline Keeper** (seq17, plan: *"stopping token generation and reducing the lethal threat"*). The -2's two forced draws ran 3 punishers for 6, putting them 9 → 3, and their own t14 draw step (2 Howling Mines, 3 cards × 3 punishers = 9) killed them before the 49 ever attacked. Wave 78's seq15 refused this row three times quoting `no noncreature row` and lost at −2. |
| 130 | **WIN** t27, 9–0 | 21 decisions, 0 deviation, 0 fallback, the cleanest seat file in the corpus. Caress t5 (seq7), Underworld Dreams t11 (seq13), Fog Bank t13 (seq14), Master t15 (seq16), second Dreams t17 (seq18), Shield Sphere t19 (seq19), second Caress t25 (seq21). **seq17 (t16 Blockers)** is a correct read of the new header: `1 attacker, 6 unblocked damage - you would be at 8 ... best case with every blocker assigned: you would be at 14 ... Master of the Feast blocks Rorix Bladewing; 1 of those blockers die`. The seat took it; life stayed at 14 and Rorix (6/5 flier, the deck's whole clock) died. §BLOCKING rung 0 is written for exactly this. **seq10/seq11 (t9) declined Fog Bank twice** — guide-legal: their line read `0 are creatures`, so neither §WALLS nor rung 4 fires. It cost 6 life when Rorix arrived at t12, but the gate as written did not reach that board and the game was won. |
| 146 | **WIN** t16, 13–0 | 22 decisions, 0 deviation, 0 fallback. Fog Bank t4, Master t6, Fate Unraveler t8, **Ob Nixilis t10 (seq12) with the -2 in the same main phase (seq13)** — the same-turn -2 window edit A rests on, confirmed for a third wave. Four attack windows, all on rung: seq14/seq17 send Fate Unraveler on `[no creature they control can block this attacker]` (SEND with no tag); seq19 sends into `Nadaar, Selfless Paladin (3/3) (you kill it, your attacker lives)` — every name friendly, no `(blocking trigger:`, no `(lifelink:`; **seq21** sends BOTH Unravelers at opponent 1 into three blockers with the `DRAW FORECAST` already reading `they would be at -1; that KILLS them` — a free roll the model names (*"if blocked, they die on their draw step anyway"*). `Triumphant Adventurer (1/1) (both die)` in that tag is TRUE, not a defect: the card renders `[deathtouch]` on the same screen. |
| 125 | **WIN** t36, 18–0 | 57 decisions, the longest game in the corpus and the one with all 5 `plan_absent` records (LOW-2). A grind against a blue control deck that countered or exiled **five** of the seat's threats (Path to Exile on Fate Unraveler t8, Dream Fracture on Peer into the Abyss t12, Cancel on Teferi's Puzzle Box t32, Staff of Nin killing Ob Nixilis t33). K went to 0 at t8 and stayed there to t16; the brake held every Dictate row through that stretch and released correctly — first Mine taken at K = 0 (seq21, the named exception), Underworld Dreams t16 (seq31), Ob Nixilis t18 (seq34), then Dictates at K = 2 (seq38/45/47). Peer into the Abyss at seq18 is rung 2, taken the first ask it was payable. The kill was **Forced Fruition + Underworld Dreams + Ob Nixilis**: `Opponent cast Cancel / Opponent drew 7 cards` = 14 damage in one window at t32. **One guide violation, seq41 — MED-2.** |
| 126 | **LOSS** t10, 0–21 | 13 decisions, 0 deviation, 0 fallback, and **every one of them on rung**. Their Idyllic Tutor found Sanguine Bond at t5, Bond landed t7, Exquisite Blood t9 — `LOOP COMPLETE` was printed on the seat's very next window (seq11) and every window after. §SANGUINE BOND governs from there and the seat followed it to the letter: it refused BOTH creature rows at seq15 (each carrying `{their converter: ... NO figure is given ... does not stop until you are at 0}`), took the **noncreature** row (Ob Nixilis, rung 1b), and activated the -2 on Overgrown Battlement (seq18) — which the same screen tells it is safe, because making them DRAW is life THEY lose and starts no chain. The -2's two draws ran 3 punishers for 6. They died to nothing: they drew Staff of Nin on their t11 draw step, cast it with six lands, and pinged themselves' opponent for 1, which the loop turned into 20. **Not guide-addressable**: from t9 on, any one point of damage from any source wins for them, and this deck holds no enchantment removal. The seat's own clock had them at 4 when it died. |
| 152 | **LOSS** t13, −2–20 | 19 decisions, 3 deviations, 0 fallback. Curve Mine t4, Underworld Dreams t6, Fate Unraveler t8, second Dreams + Fog Bank t10 (seq13/14, both rungs matched exactly: 1a does not fire with no Unraveler on the menu, 1b takes Dreams, rung 4 takes Fog Bank). **The game was decided at t11 and not by a decision of the seat's**: `Opponent used: exile a creature with Brutal Cathar #2 targeting Fog Bank` removed the wall before blockers, so no blockers window was ever put to the seat that turn and it took all 8 — 12 → 4. At **seq18/19 (t12, life 4, 6 sources)** the header reads `CRACK-BACK NEXT TURN: 5 of their creatures ... for up to 11 - you would be at -7 or lower; that would KILL you` and Master's own `{crack-back cover:}` reads `cover 5 of 11, leaving 6 -> you would be at -2`. Both are true, and neither is survivable: the seat cast Dictate then Master and blocked the lifelinker at seq20 for the least-bad −2. Its one remaining out was maximum damage on their draw step (they were at 19), and the interface did not price it — MED-1. |

## 2. Engine / interface / card items

**HIGH-1 — the CASTING menu's format example is instantiated FROM THE MENU'S OWN ROW 1, and the model
reads it as the recommendation. The priority menu on the same screen already uses a neutral placeholder.**
Every multi-option CASTING window ends with, verbatim, `e.g. "CHOICE: 1 (Cast Fate Unraveler)" (a worked
example of the format, written out from row 1 of this list - choose the option YOU want)`, where the name
in the parentheses is copied out of live row 1. Every PRIORITY window ends with `e.g. "CHOICE: 3 (Cast
Example Card)" (a placeholder - copy a real number and short name from the list)`. **Census on this
seat**: 139 multi-option windows carry an example; in 125 the example names row 1, and in 124 of those
its parenthetical is verbatim row 1 of the live menu. The seat chose the example's own row in **62 of
139 (44.6%)**.
**The repro that shows it operating, not just correlating**: `1789189678-ai_baka_deck162-0x55cd1501dc50-vs-ai_baka_deck152.jsonl`
**seq 18** (t12, life 4, the KILL board above). The reasoning quotes the example line three separate
times and settles the choice with: *"I have 6 mana. MotF costs 3. Dictate costs 3. I can cast both. The
prompt asks for one choice now. **I will pick Dictate as it's first in the menu.**"* The menu's row 1 was
Dictate and the example read `e.g. "CHOICE: 1 (Cast Dictate of Kruphix)"`. The answer was harmless here
(Master followed at seq19), but the stated tie-break is menu position, anchored on a line the render
means as a format sample. **Fix**: use the priority menu's placeholder form on the casting menu too —
the two forms are already both in this codebase, so the fix is making the casting seam match the seam
beside it. I did NOT establish causation beyond this one trace; the 44.6% figure is a base rate and row 1
is often genuinely correct in this deck (punishers sort high), so treat the count as the reason to look
and seq 18 as the evidence.

**MED-1 — every draw engine is priced in life except the deck's biggest one: TEFERI'S PUZZLE BOX renders
as `{feeds: the opponent draws an amount that is not fixed (read the card)}` on a screen that prints
their exact hand size two lines away.**
`1789189678-...-vs-ai_baka_deck152.jsonl` **seq 18** (t12, life 4, 6 sources, dead next turn on the
render's own numbers, opponent at 19). The seat's only out was maximum damage on their draw step. The
menu priced Dictate exactly — `{feeds: the opponent draws 1 extra card per turn ... draw converters on
your battlefield: 2}` — and the seat took it: their draw step became 3 cards × 2 = 6, `DRAW FORECAST
(theirs)` bearing that out (`they would be at 15` before the cast, 13 after). The same window's other
engine, Puzzle Box, was rendered with no number at all, while the screen says `Opponent hand size: 5`;
the real figure is 5 bottomed and redrawn + 1 + Howling Mine 1 = 7 draws × 2 punishers = **14**, against
Dictate's 6. Same shape at **seq 13** (t10, hand size 6, K = 1: Box = 8, Dreams = +2/turn). **Neither is
lethal on either board — I checked, and the seat dies at t13 in both branches — so this cost no game this
wave**, but it is the one engine the deck's own §LETHAL CHECK has to count by hand (`a Puzzle Box is
their hand size off the "Opponent hand size:" line`) and the render refuses to count it. **Fix**: price
the Box row the way the others are priced, off the rendered hand size, and fold it into `DRAW FORECAST`
when one is on the battlefield. **NOT verified**: no Puzzle Box ever resolved in these six games (the
one cast, `162v125` seq 58 / t32, was countered by Cancel), so I could not check the FORECAST side of
this against a live Box — the forecast defect is predicted from the `{feeds:}` text, not observed.

**MED-2 — the `[legendary: ...]` clause omits the one number that decides whether the row is a play, and
the seat spent a card and a full turn's mana on it.**
`1789189684-ai_baka_deck162-0x55e56e267450-vs-ai_baka_deck125.jsonl` **seq 41** (t20). Row 2 reads
`Cast Ob Nixilis, the Hate-Twisted {3}{b}{b} [legendary: you already control Ob Nixilis, the
Hate-Twisted - legend rule: casting this sends one copy to your graveyard (you choose which)] {leaves 0
of your 5 untapped mana sources untapped - casting this taps you out}`. The battlefield line, ~40 lines
earlier, reads `Ob Nixilis, the Hate-Twisted {3}{b}{b} [planeswalker] [counters: 5x loyalty]`. The seat
took the row — a pure loss of a card and 5 mana, with a Dictate row on the same menu it then could not
pay for — and at seq 42 gave up `Ob Nixilis #1 ... [loyalty 5]` of two identical loyalty-5 copies. This
is the **one guide violation on this seat in 149 decisions**: §THE {reserve:} CLAUSE's last paragraph
makes a `[legendary:` row not-a-play unless the battlefield copy shows loyalty 2 or less, and the model
had to join two facts 40 lines apart to see the exception did not apply. The reply is also one of the 6
`unlabelled_plan` records (`"Cast Ob Nixilis, the Hate-Twisted.\nCHOICE: 2 ..."`), so the window where
the rule was dropped is also the window where the PLAN label was. **Fix**: carry the current loyalty
into the clause, as the `{counter cost:}` group already does (`... has 5 now, leaving 3`) — e.g.
`[legendary: you already control Ob Nixilis, the Hate-Twisted (loyalty 5) - ...]`. Cost here: one card,
one turn; the seat still won at t36.

**MED-3 — rung 0's `no other noncreature row` clause again fired against the best line, and the model
went around it. The Ob Nixilis carve-out fixed the case that killed the seat; the clause's shape did not
change.** Same window, `162v152` seq 18. Rung 0 was live (their `5 of them without a restriction` over
the seat's `0 are creatures`, header reading `that would KILL you`), Ob Nixilis was not in hand, and the
rung then says take every creature row and `no other noncreature row`. The seat had 6 sources, exactly
one creature row (Master, 3) and no second body in hand, so the rung as written idles 3 mana; the seat
cast Dictate with them and Master in the next window. **This is a note, not an edit request** — the rung
fired on a board where every line loses, the deviation cost nothing, and no corpus window shows the
clause turning a live game. Recording it because it is the second wave running in which the same clause
has pointed away from the strongest row on a KILL board, and the fix that paid (edit A) was a named
carve-out rather than a change of shape.

**LOW-1 — prompt bytes grew on this seat while the corpus mean fell.** Max 20,199 B (`162v125` seq 29),
1 window over 20 KB, median 9,765 — against wave 78's max 19,112 / 0 over / median 9,124. The growth is
in the new render clauses, not narration: the over-cap window is a 3-row upkeep menu on turn 15 whose
bulk is the `LOOP SCOPE` / `HOW A HOLD ENDS` / `hold check` block. Nothing was lost to the 16/12 KB trim
that I could find: every window I read carried the seat's own last PLAN (`THE PLAN YOU LAST STATED ...
reads "..."`) and I found no window where a stated stop was missing.

**LOW-2 — latency rose 34% against wave 78 on this seat with prompts the same size.** p50 105.4 s
(wave 78: 126.2 — but that was the outlier seat; the corpus moved 78.3 → 94.4), p95 231.1, max 361.5,
`deadline_pct` max 40.1 — no window near the 900 s ceiling. The 5 `plan_absent` records are all in the
`162v125` game (seqs 45, 47, 49, 51, 54), a contiguous t22–t26 stretch of short, low-latency replies on
2–5 row menus; every one of them still carried a legal `CHOICE:` line and was played as answered.

**LOW-3 — the six `forced_close` records, for the S11 reconciliation.** `162v126` window_seq 11 (seq 13
armed, seq 14 `dropped_decision_moved`) and window_seq 12 (seq 16/17, same pair); `162v125` window_seq 23
(seq 25/26). Gameends: 4 events / 2 unrecorded and 2 / 1. In all three pairs the decision that moved was
answered normally in the next record (`162v126` seq 15 cast Ob Nixilis; `162v125` seq 27 declined an End
step) and nothing was lost. I did not read the stderr `unclosed <think>` counts.

**Passes worth recording.**
- **T3 / the CG shape: 0 re-asks.** Across all 149 windows there is **not one** same-turn, same-phase,
  same-seam re-ask that is byte-identical, and none identical after stripping `{...}` and `[...]`
  groups. Wave 78 had 135 + 98 corpus-wide. `ask_key_continuation_differs` fired 8 times on this seat
  and `identical_ask_answers_reserved` 0, consistent with the windows genuinely differing. (Comparator
  caveat: "no counterexample under a group-stripping diff", not a proof.)
- **T5 / the blockers header: every figure on this seat is TRUE and no header prints two verdicts that
  disagree.** Three blocker windows. `162v152` seq 19/20: `3 attackers, 13 unblocked damage - you would
  be at -9 ... best case with every blocker assigned: you would be at -2; no block saves you ... the
  assignment that lets in the LEAST damage is: Master of the Feast blocks Intrepid Adversary`, then the
  material ALTERNATIVE at 10 damage, then — new this wave and exactly what T5 asked for — `10 KILLS you
  at 4 life: this line LOSES THE GAME - and so does the line above (6 at 4 life). Neither line on this
  screen survives; the one above simply loses by less. Both lines are legal`. The seat took the −2 line
  and finished at −2. `162v130` seq 17 (`at 14`, actual 14) and `162v146` seq 10 (`at 15`, actual 15) are
  both exact. No trample or menace arose on this seat, so the DP's spill and two-damage-step arms are
  **UNTESTED here**; the wave-78 HIGH-2 (`12 AT BEST` vs actual 14 at `162v152` seq 19) has no analogue
  in this corpus and the `AT BEST` wording is gone from the seat entirely.
- **T6 / `{crack-back cover:}` now subtracts from the header's own total and carries no survival
  verdict.** Four cover clauses (`162v152` seq 13, 14, 18, 19; `162v123` seq 16). Each names the header's
  figure explicitly (`the CRACK-BACK NEXT TURN line above is 11 from 5 of their creatures and puts you at
  -7 OR LOWER`), subtracts from that same 11, prints `THIS IS NOT A SURVIVAL VERDICT`, and states the
  total is a FLOOR. At seq 18/19 the result is `-2` at 4 life — a losing figure, honestly rendered, and
  the model did not call it safe. Wave 78's HIGH-1 shape (a header ADD-THOSE-UP total superseding a
  creature-only floor the cover then recomputed from) **did not arise on this seat** — no window here had
  an animating noncreature permanent on the opponent's board, so the specific two-totals case is
  **UNTESTED**, not passed.
- **T15 / the `+0` face: PRINTED.** e.g. `162v126` seq 15: `over those turns life-gain EVENTS put +0 on
  them and life-loss EVENTS took -6 off`. It appears on 37 windows across all 6 games. Every split
  I reconstructed was true (`162v152` seq 15: `+9 ... -8` over t8–t12, and the log shows exactly two
  lifelink gains of 4 and 5 and eight points of punisher damage).
- **T11 / gloss cuts: 2 on this seat, 0 mid-clause.** `162v152` seq 8 Intrepid Adversary (`... you may
  pay {1}{W} any number of times. (...more) -- Creatures you control get +1/+1 for each valor counter
  ...` — cut after a whole sentence, marker present, the dependent clause kept) and `162v125` seq 42 Ob
  Nixilis (`... deals 1 damage to that player. (...more)`). Both end on a clause boundary. The second is
  worth one line for the trim's priority rule: on a legend-rule menu it kept the trigger and dropped the
  `-2:` line, which is the half the choice is about.
- **The `{their converter:}` refusal, twice, under a complete loop.** `162v126` seq 15: both creature
  rows carried `... they gain 4 [or 5], and the converter named above turns that into 4 off YOUR life,
  which re-enters the chain and does not stop until you are at 0. A {0} body is exactly as fatal here as
  a {6} one`. The seat took the planeswalker. §SANGUINE BOND, working.
- **Land drops and mulligans**: every one of the 42 windows that offered a `Play <land>` row was answered with a land (42/42); 2 mulligans, both
  to 6 with a bottom ask answered per §MULLIGAN (`162v126` bottomed Ob Nixilis from a 6-card keep — the
  most expensive spell, and the rule as written; it was drawn again at t8 anyway).
- No card behaved contrary to its rendered text in six games. Re-checked against the prompts and the log:
  Ob Nixilis 5 loyalty and the -2 offered in the same main phase (3 games), the -2's two forced draws
  running every punisher, Fate Unraveler 3/4 `{3}{b}`, Master of the Feast 5/5 flier with the upkeep
  gift, Fog Bank's prevention, Shield Sphere's `-0/-1` per block, Howling Mine's untapped condition,
  Underworld Dreams stacking as two copies, Forced Fruition's seven cards on a cast (`162v125` t32:
  `Opponent cast Cancel / Opponent drew 7 cards` then 7 + 7 punisher lines), Liliana's Caress at cleanup,
  Peer into the Abyss's `{right now: ... life -10, draws 24}` mode clause, Triumphant Adventurer's
  deathtouch making `(both die)` true against a 3/4.

## 3. Guide verdict: **KEEP as is**

`projects/mtg/bin/Res/ai/baka/deck162_strategy.txt`, 19,968 bytes, unchanged. No
`wave79/deck162/strategy.txt` is written. Why this is a success verdict and not a shrug:

- **All four wave-78 edits held, and the one that could pay, paid.** **Edit A** (rung 0's Ob Nixilis
  carve-out) is the difference between this wave and last: the same KILL-board shape recurred at
  `162v123` seq 16 with 24 attackers on the other side, the seat took Ob Nixilis first and the -2 the
  same main phase, and it won a matchup it had lost in waves 77 and 78. The same-turn -2 window it rests
  on is confirmed again at `162v146` seq 12/13. **Edit B** (`take a wall or a punisher row`) is
  **UNTESTED** — no window this wave met its condition (K = 0 with a converter in hand, M > N, life ≤ 15).
  **Edits C and D** (the life-gain STOP and the deleted forwarding sentence) are **UNTESTED in both
  directions**: the literals `(blocking trigger:` and `(lifelink:` appear in **zero** attack tags on this
  seat, and no attack was declined that a friendly tag would have permitted — the four attack windows
  all sent, and all four were correct. Edit C's over-restriction risk that I flagged last wave therefore
  remains open, not cleared.
- **The rungs matched the corpus 148 times in 149.** The single violation (MED-2) is a rule that is
  already stated exactly, in the right section, with the right literal; its cause is two facts rendered
  40 lines apart, which is a render fix (MED-2's) and not a wording fix. Restating it in the guide would
  be a second wording of a rule that already lives there — the shape that beat rung 4b in wave 77.
- **Neither loss is guide-attributable.** `162v126` is a two-card loop completed on their board at t9
  against a deck with no enchantment removal; the seat's 13 decisions are all on rung, §SANGUINE BOND
  governed every one of them correctly, and the seat's own clock had them at 4 when it died. `162v152` is
  a race whose hinge is `Opponent used: exile a creature with Brutal Cathar #2 targeting Fog Bank` at
  t11 — a removal spell on the wall the guide correctly told it to cast, one window before it would have
  blocked. Hands are real: deck162 saw Ob Nixilis in 5 of its 6 games this wave (I did not count last wave's).
- **The guide has 32 bytes of headroom.** Nothing in this corpus is worth spending them on, and MED-3 —
  the one clause I would look at hardest — did not cost a decision and the model routed around it. The
  honest reading is that rung 0's shape is worth a look on a wave where it *does* cost something.

No `general-proposals.md` and no `skill-proposals.md`: HIGH-1 is an engine seam, not a general-guide rule,
and nothing here is a skill-level finding that Amendments 332/333/334 do not already cover.

## 4. Not checked / limits

- Only this seat's six translogs and the six opponent translogs (for outcomes and log narration). **No
  stderr files, no `askreplay/` sidecar, no corpus counters beyond my six gameends, no other deck's
  seats, no engine source, no lane-{CZ,DA,DB,DC,DD} adjudication, no `corpus-stats.py` run of my own.**
  I did not reconcile the six `forced_close` records against the stderr wall-miss / `unclosed <think>`
  figures (LOW-3).
- **HIGH-1's causal claim rests on one trace.** The 44.6% example-row agreement is a base rate with no
  control: I did not compare it against the priority windows (whose example is a placeholder) at a
  sample size that would separate anchoring from the rows genuinely being ordered well. What I show is
  that the two seams render the example differently and that one trace names menu position as its
  tie-break while quoting the example three times.
- **MED-1's forecast half is predicted, not observed.** No Teferi's Puzzle Box resolved in these six
  games, so I could not check whether `DRAW FORECAST (theirs)` counts one on the battlefield. The
  `{feeds:}` defect is quoted verbatim from live rows; the arithmetic I give for what the Box would have
  done (14 vs 6 at `162v152` seq 18) is computed off the rendered hand size, not replayed. I also
  verified that it changes neither loss: the seat dies at t13 in both branches.
- **T5's trample / menace / first-strike arms and the >12×12 drop are UNTESTED on this seat** — no such
  attacker arose in three blocker windows. `GANG BLOCK:` never fired (`blocker_forecast_gang` 0); the
  string appears only in the standing explanatory paragraph.
- **Also UNTESTED on this seat, not passed**: T2 (`stop_reached_*` 0), T4 / DC F10 (`own_loop_*` 0 — no
  `[own loop verdict:` line was rendered at all), T7 / DC F8 (no `UNLESS <source> untaps them` clause),
  T8 / DC F6 (no `HELD:` note, no counterspell in the deck, no X spell), T13 (no stack-drain window),
  T9 / DC F4-F5 (no forced-sacrifice row, and no ATTACK TOTAL punisher fold — the two ATTACK TOTAL lines
  at `162v146` seq 19/21 print a combat-only projection with no punisher clause, which may be correct
  for their shape; I did not establish which), T16 (`ON THE STACK` appears on 10 windows in `162v125` but
  no queued punisher damage arose to check it against), `hold_verdict_safer_ignored` /
  `hold_reopened_new_threat` (0 each), `menu_pass_no_progress_suppressed` (0), the S13 hoist at scale
  (this seat's menus were small; I saw one shared-body hoist, `162v126` seq 18, and it was correct).
- The `162v126` and `162v152` losses are called not-guide-addressable on the engine's own printed totals
  and the game logs, not on replayed games. For `162v126` in particular I did not establish that no
  t8–t10 line beats a completed Sanguine Bond + Exquisite Blood; I establish that the deck holds no
  answer to an enchantment and that the loop needs only one point of damage from any source.
- MED-3 is a structural observation about a clause that did NOT cost a game. I did not construct the
  board on which it would, and I am not asserting it will.
