# Wave 77 — deck 123 (Intruders of Thraben, WBU token combo) — per-deck review

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260911-010157-final`, SINGLE binary
`archives/wagic-42c41fc66-w77step1` (source 8e394a077), `--thinking on`. All six deck123 games are
RERUNS after the pilot wedge (start epochs 1789114814–1789114905); the one first-run game is
126v162, not mine. Live guide = the wave-76 edition, installed 2026-09-11 at 3858f4e67.

**2 wins (vs126 t13 at 18/−1, vs162 t11 at 11/−3), 4 losses (vs130 t23 at 0/20, vs152 t14 at
−15/21, vs146 t20 at 0/22, vs125 t54 at 0/42)** — 2/6, same as wave 76, but the win/loss set flipped
(146 and 152 were the wave-76 wins; 126 and 162 were losses).

Seat census: **477 decisions** (ask 349, priority 113, discard 6, attackers 5, reveal 2, blockers 2;
by game 125: 219, 162: 88, 146: 64, 126: 48, 152: 33, 130: 25), 100% native reasoning
(`reasoning_chars` median 5,488, max 19,448), `protocol_deviation` **compliant 477/477**, **0
`off_protocol_bytes`, 0 `plan_line_missing`, 0 `reply_truncated`, 0 `action_before_plan`, 0
`unparsed_reply`**, and **one fallback: the corpus's single `repeat_count_reask`** (126 seq 38,
quoted in HIGH-1). Latency p50 83.7 s, max 351.2 s (that same record). Gameend sums for this seat:
`ask_replays_reserved` 449, `identical_ask_answers_reserved` 380, `hold_windows_skipped` 279 (all
cast), `hold_released_turn` 29, `stop_reached_windows_skipped` 7 — **all 7 corpus-wide are mine**,
`crossphase_identical_reputs` 57, `plan_names_uncastable_zone_card` 36 (38 records carry the flag),
`plan_names_stranded_card` 0, `forced_close_unrecorded` 1 (146), `phase2_answer_recovered` 1 /
missing 0, `async_drops` 4, `reserve_decline_windows_noted` 11 / skipped 2,
`blocker_forecast_multi` 118 / rows 118, `own_loop_windows_asked` 0, `main_phase_windows_skipped` 0
(fifth wave), `repeat_annotated_takes` 0, `force_close_arms_refused/deferred/same_arm` 0/0/0.

**Hands are real.** Three of the four losses are decided before any choice of mine: 130 is mono-red
land destruction (two Molten Rain, three Lay Waste, every land gone, both Doomsayers answered by
Pyrite Spellbomb and Siege-Gang), 152 is a turn-14 aggro board of nine creatures against a seat
whose first Keeper was exiled by Brutal Cathar, and 125 counters/exiles every maker for 55 turns
(the seat's battlefield reads `of which 0 are creatures` from t16 to the end). I score none of the
1/6↔2/6 movement as guide effect. What I score is the decisions below where the seat had a choice.

## 1. Game by game — the deciding decisions

**vs deck126 — WIN, turn 13 (18 / −1). The win nearly did not happen, and the reason is seq 25/26.**
At t6, board empty on both sides but for their **Perimeter Captain (0/4) [defender]**, the seat cast
Devour Flesh at the opponent. Row 1 printed the new R3 clause in full:
`{right now: they control 1 creature - Perimeter Captain (0/4) [defender] is sacrificed, they gain 4
- the sacrificing player gains, not you; YOU control 0 creatures - targeting yourself does nothing}`
and the target row at **seq 26** printed `they sacrifice Perimeter Captain and they gain 4 life (its
toughness)`. The reply's PLAN was **`cast Devour Flesh on opponent to remove Perimeter Captain and
gain 4 life`** — the seat still believed *it* gained the 4. They went 20 → 24 → 26; the alpha strike
at **seq 60** was 31 attackers for 32 into three 0/4 walls, `At least 28 damage lands`, and they
died at **−1**. Without those 4 life the same swing kills with 5 to spare; with 4 more it does not
kill at all. The seat's own reasoning at seq 25 names the conflict it resolved wrongly:
*"'A - only PERIMETER CAPTAIN... "ATTACK: none" until removal clears them.' So removal is priority.
Also, gaining 4 life is good."* — the guide's CHECK 1 A licensed an edict that the guide's own
TWO EDICTS entry (`At N = 1 a "[defender]" or power-0 body is a decline`) forbids. **Edit 1 exists
for this seq.** The rest of the game is the deck working: Doomsayer t8, second Doomsayer t10, Alarm
t12, 30 tokens t12, `ATTACK: none` at **seq 40** under CHECK -1 with 13 summoning-sick bodies, lethal
at t14.

**vs deck162 — WIN, turn 11 (11 / −3).** Textbook. Doomsayer t6, Alarm t8, `ATTACK: none` at
**seq 59** and **seq 98** (CHECK -1, both on a lone maker carrying
`(your attacker dies, their blocker lives)` against Fate Unraveler), the build across **seq 78 /
109 / 118**, Greaves on the Doomsayer at **seq 117** (never on a token), Morbid Slip on Master of
the Feast at **seq 120/121**, 25 attackers for 26 at **seq 152**. Two notes: its stop read
`stop=25` where L + C + 3 = 20 + 3 + 3 = **26** (harmless, it overshot the real number by building
to 25 anyway), and at seq 152 it sent the Doomsayer although CHECK 0's tag bullet says a maker
carrying `(your attacker dies, their blocker lives)` is *never* sent — correct play on a lethal
swing, and a gap in the guide I did not pay for (§4).

**vs deck130 — LOSS, turn 23 (0 / 20). Variance, played to the guide throughout.** 25 decisions in
23 turns because the seat had nothing: Molten Rain / Lay Waste removed six lands, Pyrite Spellbomb
and Siege-Gang answered both Doomsayers. Every discretionary call is guide-correct: **seq 22** casts
Damnation on `destroys 4 of their creatures (4 without a restriction), 0 of yours` (M = 0, K = 4);
**seq 23** takes Vision Skeins as P7 with Alarm and Tutor both dead on an empty board; **seq 24/25**
casts Devour Flesh at the opponent at N = 1 on Dwarven Blastminer, the land-destroyer, for 1 life —
the one shape the guide licenses at the opponent.

**vs deck152 — LOSS, turn 14 (−15 / 21). Variance.** Doomsayer t6, Alarm t8, Keeper t10 exiled by
Brutal Cathar t11, second Keeper t14 into a nine-creature board. **seq 57** holds at upkeep with
both rows dead (Vision Skeins under a `STRANDS Intruder Alarm` verdict, a {3}{b} exile-cast Devour
Flesh at N = 3 printing `they choose which one`) — licensed. **seq 63** is the only blockers window:
`INCOMING ... 26 unblocked damage ... no block saves you ... best case with every blocker assigned:
you would be at −10`, and the seat took the header's own `ALTERNATIVE, chosen for MATERIAL` line
(Keeper blocks Brutal Cathar, which would have returned its exiled Keeper) rather than the guide's
biggest-attacker-first rule. Lethal either way; noted, not paid (§4).

**vs deck125 — LOSS, turn 54 (0 / 42).** The marathon. Doomsayer t6 and Keeper t14 both answered;
from t16 the seat holds 8–14 cards it cannot use and the board is 10–14 lands against two Staffs of
Nin ticking 2/turn. Two discretionary classes:
- **seq 366 (t42) and seq 448 (t50) cast Intruder Alarm onto `of which 0 are creatures`** — the
  string the guide names as making the Alarm "dead at any price". Both casts were **free** (13 and
  14 untapped sources, nothing else castable, no maker in hand), which is why I do not pay an edit
  with them; see §3.
- **seq 280 (t32) discards Damnation at cleanup.** The guide says `Neither ask ever takes ... a
  Damnation` and also says a `copy N of N` row `is always the discard`; the row was
  `Damnation (copy 2 of 2 in your hand)` with every other row reading `dead right now`, and it kept
  copy 1. The seat resolved a literal contradiction in the guide the sensible way.

## 2. Engine / interface / card items

**HIGH-1 — THE REPEAT-ROW EXAMPLE AND THE WINDOW'S CLOSING PROTOCOL SENTENCE CONTRADICT EACH OTHER,
AND THE CONTRADICTION COST 342 SECONDS AND A REFUSED WINDOW.** Repro:
`1789114839-ai_baka_deck123-0x562cee9be3a0-vs-ai_baka_deck126.jsonl` **seq 38**
(`fallback: repeat_count_reask`, the only one in the corpus, `latency_ms` 342035,
`attempt_ms [339038, 2997]`, `reasoning_budget_hit true`, `reasoning_forced_close true`,
`reasoning_ngram_repeat` **0.626** — the highest in the seat by a factor of two). Row 2 reads
`... you name N on the CHOICE line, e.g. "CHOICE: 2 (Create human with Thraben Doomsayer x<N>)" ...
copying this row's name alone, with no x<count> after it, names no count and is refused and
re-asked`. The window's last line reads `... then on a line of its own CHOICE: followed by the
number (0 = pass priority) and its SHORT NAME in parentheses (the action and card name only - copy
nothing from the {...} annotations)`. The reply, verbatim:

    PLAN: stop=31; M=13; then Create human with Thraben Doomsayer x18
    CHOICE: 2 (Create human with Thraben Doomsayer, repeated then stop)

and the reasoning is 18,616 characters of that exact loop, quoted verbatim from its tail:
*"But wait, the option 2 says 'you name N on the CHOICE line'. The instruction says 'Write the PLAN
as INTENT...'. So I write the plan with the number. And the CHOICE line is just the number and short
name. ... So 'CHOICE: 2 (Create human with Thraben Doomsayer, repeated then stop)'. And the plan
includes 'x18'. But wait, the option 2 says 'N is a DIGIT you choose'. 18 is two digits."* — repeated
to the token budget. The heuristic's play did **not** change the game (the re-ask at seq 39 took
`x18` and reached the stop; the seat won at t13). **Suggested fix**: the closing sentence carves out
the count — `the action and card name only (plus the x<N> where a row asks for one)` — or the
repeat row's example is restated in the closing sentence's own words. This is the highest-value
render change here: it is a direct instruction conflict inside one prompt, and the model resolved it
by looping to the budget.

**HIGH-2 — A ROW THAT PRINTS `{if you pass here, this option is not offered again until the board
changes}` AND `ALREADY AT OR PAST your own stop` IS RE-OFFERED TWELVE TIMES IN ONE TURN.** Repro:
same file, **seqs 41–52** — twelve consecutive priority windows across the opponent's turn 13
(Upkeep, Draw, Main 1 ×4, Combat begins, Attackers, Combat ends, Main 2, End, Cleanup), each with
exactly two rows: `Create human with Thraben Doomsayer [cost: Tap]` and the hold row. Seq 44's row
verbatim: `{if you pass here, this option is not offered again until the board changes} {right now:
M=31, your stated stop=29, so this window would add to a count ALREADY AT OR PAST your own stop -
past your stop = a wasted window}`. The seat answered `CHOICE: 0 (pass)` all twelve times, each
reply restating the same arithmetic (seq 50: *"Stop is 29, M is 31. M >= Stop, so creating tokens is
wasted."*). `stop_reached_windows_skipped` for that game is **5** — the skip fires, but it covered 5
of 17. At this seat's p50 of 83.7 s that is roughly **16 minutes of decode for zero decisions**, and
deck123 is the only deck in the corpus that can reach this shape (all 7 corpus-wide
`stop_reached_windows_skipped` are mine). **Suggested fix**: when the row's own annotation computes
M ≥ stop, that row is stop-reached — skip the window rather than print the finding and ask anyway;
and the `not offered again until the board changes` promise needs the row's own key (M, stop, cost),
not the whole board, or it is false as printed.

**MED-3 — THE WHOLE-TURN PASSIVE RUN IS THE SEAT'S DOMINANT SHAPE: 22 runs of 3+ identical-seam
windows answered `Cast nothing right now` / `pass` with the hold row present, the longest 15, and a
hold-check streak reaching `(36 windows in a row now)`.** Repro: `...vs-ai_baka_deck146.jsonl`
**seqs 75–84** — ten windows at one seam on turn 17, rows `Cast Tragic Slip {b} ... {kills 0 of the
1 CREATURE target at -1/-1}`, hold, `Cast nothing right now`; the bracket counts up
`(6 windows in a row now)` … `(9 windows in a row now)` and the answer is the same each time. Longest
runs: 146 seqs 33–47 (15), 126 seqs 41–55 (14, = HIGH-2), 125 seqs 145–171 (13), 125 seqs 190–210
(9), 146 seqs 67–82 (9); max streak bracket `(36 windows in a row now)` at 125 seq 312. The hold row
would have closed each of these and (146 seq 84 proves it) still re-opened when the Slip's target
roster changed. Half of this is guide (edit 2 below); the other half is that nothing on the screen
tells the model the run *costs* anything. **Suggested fix**: at a streak of N the bracket says what
the hold saves (`answering this window again re-asks the same N rows; the hold row ends the run and
re-opens the moment a row appears, disappears or names a different card, cost or target`).

**MED-4 — `hold_check_ref_seq` IS AN ORDINAL OVER DECISION RECORDS, NOT A `seq`, AND THE BRIEF TELLS
EVERY SEAT TO ADJUDICATE WITH IT.** Repro: `...vs-ai_baka_deck125.jsonl` **seq 37**
(a Marsh Flats priority window) carries `hold_check_ref_seq: 2`; record `seq` 2 is that game's
**mulligan** window. Read as an index into the game's decision records with the mulligan at 0, it
resolves to seq 4 — the previous Marsh Flats priority window, exactly right. Read as a `seq`, my
first pass produced **181 "false" brackets out of 244**; read correctly, **0**. The field name will
manufacture a fabricated HIGH item in any seat that trusts it. **Suggested fix**: rename to
`hold_check_ref_index`, or emit the referenced record's `seq`.

**MED-5 — THE R5 CREATURE GLOSS CAN CUT AWAY THE DURATION CLAUSE THAT DECIDES THE CARD.** 159 glossed
creature entries in this seat, 10 distinct texts, all faithful to `text=` except one: Brutal Cathar
renders as `{effect: "When this creature enters or transforms into Brutal Cathar, exile target
creature an..."}` (4 prints, e.g. `...vs-ai_baka_deck152.jsonl` **seq 62**). The half the cap removed
is *"...until this leaves the battlefield"* — the return condition, i.e. the entire reason the seat's
exiled Bloodline Keeper was recoverable. The blockers header at **seq 63** does print
`{if this leaves the battlefield: your Bloodline Keeper (3/3) comes back from exile}` on the A4 row,
so the fact is not lost everywhere, but the gloss is truncated at precisely the clause that inverts
the card. **Suggested fix**: the cap prefers a clause boundary, or truncates to the head of the
*last complete* clause. Size is not the constraint here: max prompt in this seat is **23,653 bytes**
(125 seq 487) with **25 of 477** over 20 KB, well under CT's 38,657 bound, and the largest single
option row is 1,992 B (152 seq 57, the exile-cast Devour Flesh).

**MED-6 — `plan_names_uncastable_zone_card` IS ~0 REAL AT THIS SEAT AFTER R9.** 38 flagged records
(gameend 36). I read 12: **at least 10 are next-turn cast intents in a multi-turn plan** —
125 seq 27 `next turn play Underground Sea, cast Thraben Doomsayer, equip Greaves`, 125 seq 52
`Play Underground Sea, cast Thraben Doomsayer, cast Intruder Alarm`, 125 seq 24
`Cast Idyllic Tutor to find Intruder Alarm` (the tutor-target shape R9 meant to exempt, still
counted). R9's cast-intent restriction does not separate *this window* from *the plan's later
steps*, and this deck's whole guide is a multi-step plan. Measure only, as R9 says — but at this
seat the number carries no signal.

**LOW-7 — the edict's opponent branch now prices the life and the model still misreads it.** Not a
defect in the render; recorded because it closes R3's question for this deck. 16 rows printed
`the sacrificing player gains, not you` in this seat, and 126 seq 25's PLAN
(`remove Perimeter Captain and gain 4 life`) was written under it. The clause is true, present and
adjacent; the misread survives it. Whatever comes next for this class is a guide or protocol
question, not more render.

**Render clauses I checked and found TRUE.**
- **R2 (a/b/c) — CLOSED at this seat.** 445 `[hold check: ...]` brackets. 18 print
  `this is the first window I have asked you at this seam - there is no earlier menu here to compare
  it with yet` and all 18 carry `hold_check_ref_seq: -1` (genuinely first, including the land seam).
  Of the 427 compared, **0 false `every row above was also on the menu`** (wave 76: 22 false
  corpus-wide, all cast/land) and 8 of the `N rows above are new` counts disagreed with my key — all
  8 are Tragic Slip rows whose `legal targets right now:` roster changed (146 seq 84 vs 82:
  `{kills 0 of the 1 CREATURE target}` → `{kills: Goblin}` with a Goblin added to the roster), i.e.
  a row "naming a different target", so the claims are true and my key was the coarse one.
- **R7 — PASS.** The aggregate now reads `Their N untapped blockers **that can legally block at
  least one of these attackers**`. Both testable renders are exact: 126 seq 60 (31 attackers, 32
  power, 3 walls → `At least 28 damage lands`, = 32 − 2 − 1 − 1) and 162 seq 152 (25 attackers, 26
  power, 2 blockers → `At least 23`, = 26 − 2 − 1). The wave-76 MED-2 from my own 123v152 seq 104 is
  closed.
- **R8 — PASS, and the inversion is large.** 56 `[this exact list was put to you N windows ago ...]`
  brackets against 57 counted `crossphase_identical_reputs` (wave 76: 2 prints against 83). All 56
  print `the board has moved since then, but this list has not`; 0 print `nothing on the board has
  changed`, consistent with the 2 corpus-wide.
- **R3 opponent branch — PASS as a render.** Both edict branches are priced on every row
  (130 seq 24, 146 seq 5, 152 seq 22, 126 seq 25, 162 seq 114); the OWN-converter case did not
  arise here.
- **R6(a) — PASS.** A targeted-removal target row now carries the cover figure: 146 seq 93,
  `Goblin (1/1) ... {right now: -1/-1 - DIES} {removes 1 from the CRACK-BACK total above: 4 -> 3}`,
  arithmetically true. 21 `{crack-back cover:}` renders in the seat; the 4 I recomputed
  (130 seq 22, 146 seq 93, 152 seq 62, 162 seq 120) are exact. No regenerating or returning body
  appeared, so CU F3's half is untested here.
- **R4 — the blockers header's two shapes both print and both were true.** The survivable shape
  (`you would be at N AT BEST (no assignment of your blockers does better)`, now with
  `N AT BEST if you also take every OPTIONAL gain`) does not occur in this seat's 2 blockers
  windows; the lethal shape does (152 seq 63: `26 unblocked damage - you would be at -17 ... best
  case with every blocker assigned: you would be at -10; no block saves you`) and is exact. Note for
  the guide-writers: the live guide keys on the literal string `AT BEST`, which the lethal shape
  does not print — the rule still lands correctly because "no such clause ... is BLOCKS: none", but
  the two shapes are now different sentences.
- **R11's option-row roster cap — PASS.** 10 `{kills whichever you target:}` rosters, all folded
  to counts and shapes (162 seq 120: `THEIRS - 3 bodies: Shield Sphere, Fate Unraveler, Master of
  the Feast; YOURS - 24 bodies: Human #1-#24 x24`); none needed `and N more`, and the largest option
  row in the seat is 1,992 B against wave 76's 2,374 B roster.
- **The reserve-VERDICT rule holds a second wave: 19 renders, 18 declined.** The one taken (152
  seq 59, Vision Skeins under `VERDICT: taking this row STRANDS Lightning Greaves this turn`) strands
  a Greaves on a board with 0 creatures — nothing of value. `plan_names_stranded_card` 0.

**UNTESTED from this seat (0 renders in 477 decisions):** R1 / `own_loop_windows_asked` (0 — this
deck has no life loop), R10 and R6(b) ANNOUNCE_X (0 X-announcement windows), `LOOP COMPLETE` (0),
`identical_option_asks_resolved` (0), CU F3 regeneration/return guard (0 such bodies), Q6 `off YOUR
life` forced-sacrifice rows (0), Q10 `{removes:}` / ghostform (0), MDFC faces (seventh wave with
none in a deck123 hand), `sibling_window_asks_skipped` / `chain_windows_collapsed` (0 again — every
chain row here costs Tap, which the mana policy always affords).

## 3. Which wave-76 items recur

| wave-76 item | wave-77 status |
|---|---|
| **edit 1**, the edict entry gaining `Devour Flesh pays the life to the player you NAME` | **The string held where it was read, and was bypassed where it was not.** 3 of the 4 edicts cast at the opponent this wave are at N = 1 and guide-legal (130 seq 24 on the land-destroyer, 146 seq 5/8, 152 seq 22). The fourth, **126 seq 25**, never reached the entry: the seat entered through CHECK 1 A instead. Edit 1 below closes that door. |
| **edit 2**, the bait-cast gate above the priority list | **UNTESTED.** Its trigger (a counterspell in their log/graveyard, their untapped sources ≥ 2, mana for TWO rows on this menu) never co-occurred with a maker on the menu: in the 125 rerun the seat's two makers were cast at t6 and t14 before any counter was visible, and after t16 there was no maker in hand for 40 turns. No bait window arose to test. |
| **edit 3**, the CHECK 0 attack-trigger stop (`the stop is 1`) | **UNTESTED.** No attack trigger on any opponent's board in these six games; the stop ran on L + C + 3 throughout. |
| the reserve-VERDICT override | **CLOSED, second wave. 18 of 19 declined**, the one override worthless. |
| the Alarm-on-an-empty-board cast (4× in wave 75, "does not recur" in wave 76) | **RECURS 3×** — 146 seq 66 (t16, 7 sources), 125 seq 366 (t42, 13 sources), 125 seq 448 (t50, 14 sources). All three are on a `of which 0 are creatures` line and all three are **free**: main phase, no maker in hand, nothing else castable, the mana spent could buy nothing. The guide's `dead at any price` is the clause the model overrides, and it overrides it on exactly the boards where the clause is false. I do not pay an edit with a decision that cost nothing (§4). |
| CHECK -1 (the lone maker) | **Held, 3 of 3** — 126 seq 40, 162 seq 59, 162 seq 98; two of the three cite it and 162 seq 98 reasons the untap consequence out loud. |
| the `(your attacker dies, their blocker lives)` tag (wave-76 edit 3, then UNTESTED) | **TESTED, 3 renders.** Honoured twice (162 seq 59/98, both `ATTACK: none`), overridden once on a lethal swing (162 seq 152) where overriding was right. |
| the Tragic Slip `kills 0 of the N` / `{kills: <name>}` pair | **Inverted from wave 76.** Every Slip *cast* named a live row (146 seq 92/93, 162 seq 75/76 and 120/121, all `- DIES` or `so Morbid applies`), and the seat twice **declined** a live `{kills: Goblin}` row (146 seq 84, seq 90) reasoning that a 1/1 token is not worth the card. Wave 76 saw the opposite failure (a Slip spent on an idle 0/5 wall) and declined to add a relevance test; the model has now supplied one of its own, in the right direction. |
| MED-3, Q13's cross-phase bracket almost never printing (2 of 83) | **CLOSED by R8** — 56 of 57 here. |
| MED-2, the ATTACK TOTAL blocker count not legality-filtered (my 123v152 seq 104) | **CLOSED by R7** — the sentence now says "that can legally block at least one of these attackers" and both testable floors are exact. |
| HIGH-1, the unpriced Devour Flesh opponent branch | **CLOSED as a render by R3, NOT closed as a comprehension** — see LOW-7 and 126 seq 25. |
| LOW-4, the repeated-activation completion line on the wrong turn | **Not re-observed** (no repeat activation spanning a turn boundary this wave). |
| — new — | the repeat-row / closing-protocol contradiction (HIGH-1), the stop-reached window re-offered 12× (HIGH-2), the 22 passive runs (MED-3), `hold_check_ref_seq` as an ordinal (MED-4). |

## 4. Guide verdict: **EDIT**

Revised guide at `wave77/deck123/strategy.txt` — **19,995 bytes** (live guide 19,988; ceiling
20,000). A332 grep (`seq`, `wave`, `corpus`, `game N`, turn citations, deck numbers): 0 hits.
`YOUR PLAN`: 0. `even when` / `even if` / `regardless of` / `no matter` / `however` (A334 audit): 0.
Both PLAN templates are one physical line each. Headings unchanged, each declaring its own scope
(A333). Two edits; the guide grows **7 bytes**, and the rule that earned them is edit 1.

| # | before → after | seq that paid for it |
|---|---|---|
| 1 | **CHECK 1 A names the removal that qualifies and excludes the edict.** `A - only PERIMETER CAPTAIN or PRIDE GUARDIAN: "ATTACK: none" until removal clears them.` → `A - only PERIMETER CAPTAIN or PRIDE GUARDIAN: "ATTACK: none" until a Slip or a Damnation clears them; an edict is not that removal - it pays them the wall's toughness.` The decline already existed in `THE TWO EDICTS` and the seat never reached it, because the word `removal` in CHECK 1 A read as a licence and CHECK 1 is the section that names the card. The fix is where the licence was granted, and it is a positive statement of which removal answers a wall, not an exception bolted to the edict rule. It also states the reason the edict is the wrong tool on a wall specifically: toughness is what a wall has. | **126 seq 25/26** — 4 life to a Sanguine Bond / Exquisite Blood deck at t6; the t14 alpha strike (31 attackers, `At least 28 damage lands`) killed them at **−1**. **+~80 bytes.** |
| 2 | **The two HOLDS bullets merge, and the run ends on the hold row instead of on a repeated pass.** `- "0. Pass priority" is the answer whenever the stop is reached or all rows are dead, and the same dead list comes back many times a phase: answer it the same way each time.` → `- "0. Pass priority" is the answer whenever the stop is reached or all rows are dead.`, and the next bullet becomes `- THE HOLD ROW IS A P8 ANSWER AND NOTHING ELSE: take it only when EVERY row on THIS window is dead by the order above, and take it there - it is what ends a dead list that keeps coming back. One live row - ...` (rest unchanged). The deleted clause is the one that actively taught the repeat: it told the model that a dead list returning many times a phase is answered the same way each time, which is exactly what 22 runs did. The hold's guard is unchanged, so the Create-row protection (`A HOLD OVER A CREATE ROW IS NOT A SKIP OF THIS WINDOW`) still bars the hold wherever a Create row is live — which is why HIGH-2's twelve windows are an engine item, not this edit's. | **146 seqs 75–84** (ten windows, one dead Slip row, the bracket counting to `(9 windows in a row now)`, the hold row present every time and licensed every time), and 21 further runs of 3+. **−~13 bytes.** |
| — | **Paying for edit 1 (−~60, no rule removed):** the `THE PRINTED STRINGS THAT ANSWER THEIR OWN ROW` opening sentence `This paragraph is about reading the row you mean to name.` is deleted — it restates the heading it sits under, which A333 makes the heading's job. | — |

**What I deliberately did NOT change.**
- **`"of which 0 are creatures" ... dead at any price`** — three overrides this wave (§3), and all
  three cost nothing. Rewriting it to key on the mana it spends would loosen the rule that stopped
  four wave-75 misplays, on evidence that contains no loss. If a wave produces an Alarm cast that
  actually strands a maker, the rule earns the rewrite then.
- **CHECK 0's tag bullet** (`A maker ... carrying that tag is never sent`), although 162 seq 152
  overrode it correctly on a lethal swing. The crack-back bullet two lines above already carries the
  lethal carve-out, so the two bullets disagree — but the decision it produced was right, and A332's
  posture is that a rule is rewritten by a decision that went wrong.
- **`"{kills: <name>}": cast it`** — second wave in a row that this rule misfires, and this wave it
  misfired in the seat's favour (§3). I will not add a relevance test on a decision that went right.
- **The repeat-row count rule.** 126 seq 37 named `x10` with its own PLAN reading `stop=31; M=1`,
  where `N = (L + C + 3) - M` is 30; it cost one extra window and fed the seq-38 refusal. The
  formula, the word `ONCE` and a worked example are all already in the guide; restating them is the
  noise A332 warns about, and the refusal itself is HIGH-1's contradiction, not a guide gap.
- **The blocking order at lethal** (152 seq 63) — the screen's own `ALTERNATIVE, chosen for MATERIAL`
  line competed with the guide's biggest-first rule on a board where every assignment lost. That is a
  render/guide precedence question for a board where it can matter, not one this seat can settle.
- **The `copy N of N` / `never a Damnation` contradiction in DISCARD AND SACRIFICE ASKS**
  (125 seq 280). The seat resolved it correctly on its own; a rewrite costs bytes I need for edit 1.

## 5. Optional proposals

**None.** Both edits are this deck's own wording. HIGH-1, HIGH-2, MED-3, MED-4 and MED-5 are engine
items; LOW-7 is a finding about a render that is already correct; MED-6 is a measurement note that
R9 already frames as measure-only. A general-guide or skill proposal restating A332/A333/A334 would
be noise.

## 6. What I did NOT check

- **Opponent seat files were opened for board context only** (deck126's Perimeter Captain, Sanguine
  Bond and Exquisite Blood; deck152's Brutal Cathar / Tovolar's Huntmaster board; deck130's Molten
  Rain / Lay Waste line; deck125's Staffs of Nin and Elixirs; deck162's Fate Unraveler / Master of
  the Feast). **I audited no opponent decision.**
- **I adjudicated no lane prediction corpus-wide.** §2 answers R2, R3, R4, R5, R6(a), R7, R8, R9 for
  **this seat's six games only**; R1, R6(b), R10 and the CU findings that depend on them had zero
  renders here and are listed UNTESTED rather than PASS.
- **The single `forced_close_unrecorded` (146) was not reconciled against stderr**, and the 4
  `async_drops` were not traced to re-asks. `phase2_answer_recovered` is 1 / missing 0 and no
  decision in the seat lacks a record.
- **The 57 cross-phase re-puts were not individually paired to their answers** — I established the
  bracket print rate (56) and read the shape; I did not verify that all 57 were answered identically.
- **`askreplay/` was not opened**, the 29 `hold_released_turn` events were not paired to their
  holding windows, and I did not verify the CU F1 balanced-group strip beyond noting that my own
  brace-stripping key reproduces the engine's verdict on 427 of 427 `every row` brackets.
- **I read 12 of the 38 `plan_names_uncastable_zone_card` records**, 10 of 159 `{effect:}` glosses
  against their `text=` in full (the rest by distinct string — there are only 10 distinct), and the
  `reasoning` of every window cited above; I did not read the `reasoning` of the 279 held windows or
  of the 22 passive runs individually beyond the runs quoted.
- **I did not open any card script**, run a build, run wagic, or run git; nothing under `bin/Res`,
  `src/` or outside `wave77/deck123/` was touched.
