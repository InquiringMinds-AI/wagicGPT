# Wave-73 deck126 review — SANGUINE BLOOD (WGB wall prison, Exquisite Blood + Sanguine Bond)

Corpus `matchups-20260907-225637-final`, six seat files `...ai_baka_deck126-<ptr>-vs-...` (the
125v126 and 126v123 games are reruns; treated as ordinary corpus games).
Record **2/6** (wave 72: 5/6; wave 71: 3/6). **301 model decisions** at this seat: 0 fallbacks,
0 `plan_line_missing`, 0 `action_before_plan`, 0 `reply_truncated`, 0 `ask_replays_refused`,
0 `menu_pass_no_progress`, 0 `wall_miss_events`, 0 `declined_face_latches`. Every reply was PLAN +
one action line. Per-game census (162 / 152 / 130 / 146 / 125 / 123):
decisions 25/27/44/32/148/25; `hold_windows_skipped` 0/1/0/0/640/0 (all cast-seam except one);
`own_turn_windows_skipped` 0/0/76/48/0/0; `main_phase_windows_skipped` 6/5/11/0/58/0;
`ask_replays_reserved` 5/0/0/0/81/2, refused 0; `hold_released_turn` 0/0/0/0/25/0;
`stop_reached_windows_skipped`, `sibling_window_asks_skipped`, `chain_windows_collapsed`,
`repeat_annotated_takes`, `async_drops` = **0 in all six**; `phase2_answer_recovered` 1 (vs 125);
`reserve_decline_windows_noted` 2/0/0/0/0/4, skipped 0.

## 1. Game by game

| # | seat file (epoch) | opp | result | deciding decisions |
|---|---|---|---|---|
| 1 | 1788839804 | 162 | **LOST** 0–10, t16 | Bond t9 (seq 13), pair completed t13 (seq 20). **The win button had already been spent: seq 16 and seq 18 (t11, life 23) cast BOTH Tributes on "they choose which one" rows** (§3-C). At t13/t15 the pair was out, they controlled 2–3 creatures, and no Tribute was left; no Staff, no Sorin, no Vampire. Death was the seat's own compulsory draw — 2 Fate Unravelers + Underworld Dreams + Howling Mine = 6 life per draw step (the DRAW FORECAST printed it). The opponent seat's last reply (their seq 28) reads *"Attack with Master of the Feast to deal 5 damage safely, avoiding blocking triggers that would trigger their life loop"* — a flier-only attack that denied the Perimeter Captain block deliberately. |
| 2 | 1788839810 | 152 | **LOST** −10 to 20, t15 | Aggro (Katilda / Briarbridge Tracker / Luminarch Aspirant). Sorin t6, two Vampires, correct blocks at seq 15. **seq 22 (t12, life 10) is the misplay** (§3-A): attacked its lone untapped Vampire into a lone blocker whose verdict read *"your attacker dies, their blocker lives"*, with `CRACK-BACK NEXT TURN ... 13 ... that would KILL you` printed, and the row's own CRACK-BACK COST OF ATTACKING clause spelling out that declaring taps it. At seq 23 (t13) only Wall of Omens was left to block; 14 came in, life 11→5. At seq 26 (t14) both halves were finally on one menu with 7 sources — only one castable — and the seat took Sanguine Bond, which is the guide's letter and right. Neither half saves at 5 life; the game was lost at t12-13. |
| 3 | 1788839814 | 130 | **LOST** −1 to **1**, t25 | Bond t13, Blood never found (one Tutor, t9, reveal → Bond, correct by rung 4). Sorin +1 every turn, −2 emblem t19, Staff t21, pings t21/t23/t25 (17→13→5). **seq 42 (t25, you 6, them 5) LOST A WON GAME** (§3-B): rows were `1. +1: create a 1/1 vampire` and `2. -2: emblem "creatures get +1/+0"`, Sorin at 5 loyalty. The seat took the +1 — a **summoning-sick** token that cannot attack — then attacked its 2/1 for 2 + 2 lifelink, which Sanguine Bond doubled to 4: opponent survived at **1**. Taking −2 stacks a second emblem (`planeswalkers.txt:3058`, `emblem transforms(...lord(creature|mybattlefield) 1/0) forever dontremove`; the first emblem is visibly live — Perimeter Captain printed `(1/4) (printed 0/4)`), making the attacker a 3/1: 3 unblocked + 3 lifelink converted = **6 off a total of 5, lethal that turn**. Rorix Bladewing killed the seat the next turn. |
| 4 | 1788839837 | 146 | **LOST** −11 to 21, t18 | Tutor→Bond t6, Tutor→Blood t8, Bond t10 (seq 17), Blood t12 (seq 20) — all by the book, four blocks taken with Perimeter Captain/Pride Guardian. The pair lived for **one opponent turn**: at t13 Kaya the Inexorable's −3 exiled Exquisite Blood before the blockers step, so the three block triggers that turn gained life with no converter of theirs to loop. Kaya then exiled Perimeter Captain at t15. seq 30 (t18, life 4) paid the Silencer's 3 life for a Tribute on a "they choose" row — the row's own crack-back cover clause said *"THIS IS NOT A SURVIVAL VERDICT"*; the seat was dead on board either way. Removal, not a decision game. |
| 5 | 1788902003 | 125 | **WON** 152 to −14, t63 | 148 decisions, 62 turns. Sorin (t28) and Staff of Nin (t56) were both **countered by Dream Fracture**, so the seat had no starter of its own for the whole game. Blood t10 (seq 12), Bond only t50 (seq 139, off the third Tutor); **LOOP COMPLETE from seq 140 (t50) to the end**. Through those 13 turns the opponent controlled **0 creatures** and every Tribute row printed `at 0 this does nothing` — the guide's ZERO STOP, correctly obeyed at 27 straight windows while the seat's life fell 8→2 under the opponent's own Staff. At seq 206 (t63) Emrakul appeared, the row turned into `they control 1 creature - Emrakul ... is sacrificed, you gain 15`, and the seat cast it in that first window. Model-correct and guide-correct throughout. |
| 6 | 1788916641 | 123 | **WON** 28–0, t12 | Textbook. Tutor t5 → reveal Exquisite Blood at seq 9 with Sanguine Bond in hand (rung 4's "pick the OTHER", correct). Bond t9 (seq 18), Blood t11 (seq 25). Four-blocker gangs at seq 14 and seq 19 under Perimeter Captain (+8 life each, drained through the Bond). At seq 26 (t13 upkeep) the first window with LOOP COMPLETE and a live Tribute (`they control 36 creatures`) was taken: `PLAN: Cast Tribute to Hunger to trigger the life loop and win the game.` |

**Is 5/6 → 2/6 variance, opponent effect, or regression?** Mostly the first two, with **one real
regression-shaped loss**. Of the four losses: 146 is removal (Kaya exiled a half one turn after it
landed, then the Captain — nothing in the seat's play is at fault); 152 is a hand that produced no
half until t14 against the fastest deck in the pool, with one guide-violating attack (seq 22) that
cost a blocker but not the game; 162 is a genuine strategy loss the guide licensed (§3-C, both
Tributes spent two turns before the pair completed) **plus** an opponent counterplay that is new and
deliberate (162's flier-only attack to deny the block trigger — its own reasoning names the loop);
130 is the one clean, decision-decided loss and it was **lost at a single window, seq 42**, one
point short. Play quality is not down — the reveal picks were 5/5 correct, land drop 39/39, the ZERO
STOP held at 125 straight windows, LOOP COMPLETE was converted in the first live window in both
wins. Three of four losses turn on cards the seat never drew or never kept.

## 2. Engine / interface / card items

**HIGH-1 — the ATTACK TOTAL header states a false lethality verdict whenever the seat controls
Sanguine Bond and a lifelink attacker.** Repro `1788839814-...deck126-...-vs-ai_baka_deck130.jsonl`
**seq 43** (t25, you 6 / them 5). The rendered line reads:
`ATTACK TOTAL: 1 attacker listed, 2 total combat damage to a player - declaring all of them with none blocked puts them at 3. That is NOT lethal: they survive at 3 even with none of them blocked.`
The same prompt prints `LIFE-TO-DAMAGE CONVERTER on the battlefield: yours - Sanguine Bond ... when
you gain N, your total goes UP by N and theirs goes DOWN by N in the same event`, and the attacker is
`Vampire #1 (2/1) [lifelink]`. The true figure is 4 (2 combat + 2 lifelink converted), and the log
confirms it: the opponent finished the turn at **1**, not 3. The header computes combat damage only
and then issues a categorical verdict ("That is NOT lethal") on that number. With the sibling
decision at seq 42 taken correctly (a second −2 emblem) the true figure would have been 6 against a
total of 5 and the header would still have printed "NOT lethal". Fix: when the acting seat controls
a Sanguine-Bond-shaped converter, add the lifelink attackers' damage a second time before computing
the "puts them at N" figure and the lethality verdict — or drop the verdict and print the combat
subtotal only. A render clause stating a false fact about the win is the worst class of defect for
this deck: the whole guide is built on "any one point ends the game".

**MED-2 — the Sorin loyalty menu prints no consequence annotation, while every neighbouring seam
does.** Same file, **seq 42**. The two rows are
`1. +1: create a 1/1 vampire with Sorin, Lord of Innistrad [cost: Counters] {card text: "+1: Put a 1/1 black Vampire ..."}`
and `2. -2: emblem: "creatures get +1/+0" ... [cost: Counters] {card text: ...}` — identical braces,
both just the planeswalker's full oracle text. Nothing on the menu says the +1 token is **summoning
sick** (the attackers seam one window later does say it: `NOT offered above and NOT able to attack
this turn (summoning sick) - Vampire #2`), nothing says the emblem is cumulative with the one
already applied, and nothing prices either row against the board the way the cast seam's
`{leaves N of your M untapped}` / `{right now: they would be at N}` clauses do. The Staff ping rows
in the same game (seq 40) DO carry `{right now: takes 1 damage - they would be at 5}`. Giving the
loyalty rows the same treatment — "+1: the token cannot attack until your next turn", "−2: your
Vampire #1 becomes 3/1 this turn" — turns seq 42 from a read-the-rules-text inference into a
comparison of two printed numbers.

**MED-3 — `main_phase_windows_skipped` fires on phases that were not skipped by any choice of the
seat's, and the stderr wording invites the wrong reading.** In the 125 game the counter reached 58,
with lines like `own main phase (turn 56, phase 4) ENDED with NO casting window - the instant-speed
response arm answered it; no sorcery-speed casting window was offered (51 this game)`. In that game
the seat held 640 cast-seam windows of its own, so "no casting window was offered" is a consequence
of the seat's own standing hold, not an independent skip — the two meters double-count the same
event. Corpus-wide 375 skipped main phases is **plausible in magnitude** (the two long games account
for 69 of this seat's 80), but as an engine signal it is not measuring what its name says: at this
seat it is high exactly where `hold_windows_skipped` is high (58 with 640 holds; 0 with 0 holds in
two games) and it is 6/5/11 in three games where the seat never held at all. Recommend the counter
exclude phases whose window was suppressed by an active hold latch, or that the two be reported as
one number.

**LOW-4 — the hold row is still taken on menus that print a live activated row, and the priority
seam is where it now happens.** Repro `...vs-ai_baka_deck152.jsonl` **seq 24** (t14 upkeep, life 5).
Rows: `1. Add 1 green mana with Overgrown Battlement`, `2. Draw 1 with Clue [cost: {2}, Sacrifice]`,
`3. Hold priority ... the rows above include ACTIVATED abilities that are usable RIGHT NOW`,
`0. Pass priority`. The reply: `PLAN: Proceed to main phase to cast Sanguine Bond and Exquisite
Blood. / CHOICE: 3 (Hold priority)`. This is the same shape wave 72 paid a guide edit for (its item
C) — the plan defers to a later window this turn and answers with the hold — but it has migrated
from the CAST menu to the PRIORITY menu, where the guide's rule is worded around "the last row of
nearly every **casting** menu". The Clue was never cracked. §3-D adds one sentence rather than a new
section. Rate across this seat: 2 holds taken on menus printing a live activated row, out of 18
priority windows.

**LOW-5 — `[second copy: ...]` fired 55 times and the "no legend rule" gloss reads as encouragement.**
Repro `...vs-ai_baka_deck146.jsonl` seq 30 row 1: `Cast Staff of Nin {6} [second copy: you already
control Staff of Nin; both stay on the battlefield - no legend rule, and this copy is one more of
the same effect - each line it repeats happens again]`. The seat declined every one of the 55, so
nothing is broken; recording only because the clause's first half is permission and its second half
is an argument in favour, with no cost side.

**Card checks.** `Sorin, Lord of Innistrad` (`planeswalkers.txt:3055-3060`): `auto={C(0/0,-2,Loyalty)}:name(-2: Emblem: "Creatures get +1/+0") emblem transforms((,newability[lord(creature|mybattlefield) 1/0])) forever dontremove` and `text=... -2: You get an emblem with "Creatures you control get +1/+0."` — faithful to Oracle. Emblems are separate objects and each grants its own `lord ... 1/0`, so a second −2 stacks; the corpus shows one emblem applying (`Perimeter Captain (1/4) (printed 0/4)`, `Vampire (2/1) (printed 1/1)`), and I did **not** observe two emblems live in any game — the stacking is read off the script, not measured. `Sanguine Bond` (`mtg.txt:99004`): `auto=@lifeof(player) from(*[-lifefaker]|*):life:-thatmuch opponent`, faithful. `Exquisite Blood` (`mtg.txt:37966`): `auto=@lifelostfoeof(player):life:thatmuch controller`, faithful. No other render clause in this seat stated a false fact.

**Corpus zeros the brief asks about, from this seat.** `stop_reached_windows_skipped` = 0 in all six:
no PLAN line at this seat ever wrote a countable stop, so the grammar had nothing to parse here — the
long-run shape that a stop collapse would cover (the 125 game's 27 consecutive `at 0 this does
nothing` windows, seq 135–205) was instead absorbed by the hold latch and by
`ask_replays_reserved` 81 / `identical_ask_answers_reserved` 56, at zero model calls.
`sibling_window_asks_skipped` = 0 and `chain_windows_collapsed` = 0: I did not read the engine code
and cannot say whether either can fire; what I can report is that this seat had **18** priority
windows in the whole corpus and **zero** in its two longest games, so the paths those counters guard
were barely exercised here. `hold_released_turn` = 25, all in the 125 game; each stderr release line
names its own turn and the next (`taken on turn 55 (own turn), now turn 57`, `taken on turn 60
(their turn), now turn 61`) — I traced the four releases printed around turns 55–61 and **no hold
outlived the holder's untap**.

## 3. Guide verdict: **EDIT** (`strategy.txt`, 19,995 B — same as the live guide, ceiling 20,000)

Four changes; five sections cut or compressed to pay for them, and the guide does not grow.

**A. ATTACKING is written in a render vocabulary the engine no longer prints. Paid by
`...vs-ai_baka_deck152.jsonl` seq 22 (t12) and `...vs-ai_baka_deck130.jsonl` seq 29 (t19).**
The live section is built on `THE COLLAPSED TAG reads "they have N untapped creatures that could
block this one, biggest X"` and on a `GANG BLOCK` clause. Across **289 rendered prompts** at this
seat the string `untapped creatures that could block this one` appears **0 times**; every attackers
window used `[their untapped blockers: <name> (<verdict>); ...]` (4 windows) or `[no creature they
control can block this attacker]` (2), and `GANG BLOCK` appears only on **blockers**-window A-lines
(8). So the section's decision procedure — "a GANG BLOCK clause, or N of 2 or more: ATTACK: none",
"N is 1 with no GANG BLOCK clause: count your own A-lines" — is keyed to text the model never sees,
and it is wrong twice over: at seq 29 the seat attacked into **two** listed blockers both reading
"both die", traded a 2/1 for a Goblin, gained 2 and drained 2 (the guide's letter would have benched
it); at seq 22 the one lone-blocker "your attacker dies" case the section does cover in words was
not recognised in the printed shape, and the attack cost the seat its only untapped blocker in front
of a `would KILL you` crack-back.
before: `THE COLLAPSED TAG reads "they have N untapped creatures that could block this one, biggest X", sometimes with "- GANG BLOCK: ... each result above is a LONE blocker only". ... - A GANG BLOCK clause, or N of 2 or more: ATTACK: none for that Vampire; keep it as a blocker. ... - N is 1 with no GANG BLOCK clause: COUNT YOUR OWN A-LINES. ... - No blockers listed, or "[no creature they control can block this]": send every Vampire.`
after: `EACH A-LINE NAMES THEIR BLOCKERS ONE BY ONE: "[their untapped blockers: <name> (<verdict>); ...]", or "[no creature they control can block this attacker]". READ EVERY VERDICT ON THE LINE.` + four bullets keyed to the printed verdicts ("no creature ... can block" → send; "both die"/"neither dies" → send; every verdict "your attacker dies, their blocker lives" → send anyway unless that Vampire is a body you need against a `would KILL you` crack-back, and a Vampire the line already calls unusable via `held back, THIS creature could not block ANY of ...` is free; more Vampires than blockers named → send them all).

**B. The ATTACK TOTAL's "NOT lethal" must be doubled while Sanguine Bond is out, and Sorin's −2 is
not a once-only. Paid by `...vs-ai_baka_deck130.jsonl` seq 42 → seq 43 (t25, you 6 / them 5).**
The header said "puts them at 3. That is NOT lethal"; the real result was 1 (HIGH-1). One window
earlier the live guide's `TAKE IT ONCE, in the first window that prints it` closed off the row that
wins: a second emblem makes the already-attackable Vampire a 3/1, and 3 + 3 converted = 6 against 5.
The +1 the seat took instead produces a summoning-sick token that attacks nothing that turn.
before (ATTACKING, new tail): —
after: `THE ATTACK TOTAL UNDERSTATES YOU WHILE SANGUINE BOND IS OUT: its "puts them at N" and its "NOT lethal" count COMBAT DAMAGE only. Every point a lifelink Vampire deals is also life YOU gain, which the Bond takes off them again, so the true figure is TWICE that damage. Double it yourself before you believe a "NOT lethal".`
before: `HIS -2 EMBLEM ... TAKE IT ONCE, in the first window that prints it, whenever anything on their line kills a 1/1 on sight. Take the +1 every turn after.`
after: `HIS -2 EMBLEM ... Take it in the first window that prints it whenever anything on their line kills a 1/1 on sight, and the +1 every turn after. THE EMBLEMS STACK AND A SECOND ONE CAN BE THE KILL: a fresh +1 token is summoning sick and attacks nothing this turn, so when a Vampire is already able to attack and the extra +1/+0 makes the doubled total above lethal, take -2 AGAIN instead of the +1.`

**C. The last Tribute is the win button once a half is on your line. Paid by
`...vs-ai_baka_deck162.jsonl` seq 18 (t11, life 23) → seq 20 (t13).** At seq 18 Sanguine Bond was
already on the seat's line, the row read `they control 2 creatures - they choose which one - you
gain at least 4`, life was 23, the crack-back line read `for up to 6 - you would be at 17`, and the
Tribute in hand was the **last** one (the other had been cast at seq 16 the same turn). The seat
cast it. Two turns later Exquisite Blood landed (seq 20) with two creatures on their line — a live
Tribute in that window ends the game on the spot, and there was none. The live guide has no rule
that reserves it; rung 9 licenses the cast. The new rule is life-gated at 12 so it never argues with
rung 0 or with the "your life is 12 or less" survival case in the same section.
before: `ONE TRIBUTE ANSWERS ONE CREATURE, AND A SECOND WINDOW THE SAME TURN IS NOT A SECOND CREATURE ...`
after: `KEEP THE LAST ONE ONCE A HALF IS ON YOUR LINE: with Sanguine Bond or Exquisite Blood already on your battlefield, exactly ONE Tribute in hand and your life above 12, a "they choose which one" row is NOT worth it - that card is the win the turn the other half lands. A NAMED non-defender victim, and rung 0, still outrank this.` + the existing sentence unchanged.

**D. Two small corrections.** (i) DRAW PUNISHERS stated a false mechanic: `hand size is the
multiplier: empty your hand`. Every `DRAW FORECAST` line in this corpus (15 exposures, e.g. the 162
game seq 29) meters **cards drawn × punishers** — `your next draw step draws 2 cards (1 + Howling
Mine 1) = 2 x 3 = 6 life LOST BY YOU` — and hand size never enters it; emptying the hand changes
nothing. (ii) The hold-row section is worded for "the last row of nearly every **casting** menu";
LOW-4's failure was on a priority menu.
before: `With only a "DRAW FORECAST: ... = N life" and no per-row price, the damage is metered by your DRAW STEP and hand size is the multiplier: empty your hand, cheapest permanents first. Once the ROWS carry "[DRAW PRICE: ...]" it is metered by your CASTS instead and emptying your hand is suicide.`
after: `"DRAW FORECAST: ... = N life" meters CARDS DRAWN times punishers - not hand size - and your draw step is COMPULSORY, so nothing you cast or decline slows it. Read it as a countdown: your life over N is how many turns you have. Spend them on the fastest loop start, not on a wall you do not need, and never take an OPTIONAL draw.`
after (hold row, appended): `THIS BINDS ON EVERY MENU WITH A ROW 0, TOO: an activated row there is live.`

**What paid for the bytes.** (1) Both `[DRAW PRICE: ...]` passages **deleted** (−196 B in the cast
list header, and the clause inside DRAW PUNISHERS): the string `DRAW PRICE` appears in **0** of 289
prompts this wave. (2) `=== CLEANUP DISCARD ===` compressed (−161 B): **no discard ask reached this
seat in 301 decisions** (kinds: ask 249, priority 18, blockers 10, attackers 6, reveal 5, bottom 1).
(3) The entry-7 Chromatic Lantern paragraph trimmed (−171 B): the Lantern was cast 4 times, every
one of them as the only live row, and the "once a Lantern is on your line, finished" half is already
implied by the entry's own condition. (4) MULLIGAN's two-land bullets merged (−87 B) — 3 mulligan/
bottom decisions this wave, all by the book. (5) MANA's pip enumeration (−129 B), the BLOCKING
preamble (−99 B), the Tribute cast-list wording (−109 B), the Silencer opening (−43 B), and small
rewordings across the Staff, pair, MENACE and answer-format paragraphs made up the rest. Every rule
survives; only restatement and dead vocabulary were removed.

**What KEPT and earned it.** THE ZERO STOP: **125** windows printed `at 0 this does nothing` and the
row was cast **zero** times — including 27 windows running in the 125 game while the loop was
complete and the seat was at 8 life falling, which is exactly the discipline that let it win at t63.
LAND DROP: 39 of 39 windows took a land (wave 72's single decline does not recur). THE WIN BUTTON's
"first live window" rule: converted in both wins, at 123 seq 26 and 125 seq 206. Rung 4's reveal
rule: 5 of 5 correct (Bond when holding neither, the OTHER when one was in hand). Rung 1: no half
sat in hand behind a wall in any of the six games. BLOCKING's Perimeter Captain gangs: 4 blockers on
one attacker twice in the 123 game, +8 life each and drained through the Bond. The wave-72 Lightmine
paragraph: 42 exposures, no attack ever offered under it — kept, untested. The wave-72 Silencer
Blood/Bond exception: 3 exposures, no both-halves-on-one-menu case recurred — kept, untested.
Wave-72 item A (the Lightmine override) and item B (the Silencer half) did **not** recur; wave-72
item C (hold answered to a deferring plan) **did**, in the new priority-menu shape — §3-D(ii).

No general-guide or skill proposal. A–D are deck-local; HIGH-1, MED-2 and MED-3 belong to the engine
seat, and none of them restates an existing amendment.

## 4. Not checked
I did not audit the `.stderr` files beyond the 125v126 and 126v162 games (so I contribute nothing on
the docketed {X}-announcement payment loss, and nothing on `deadline_pct` or `transport`). I read
the opponent seats only for context — 162's flier-only attack reasoning, 125's Dream Fracture
counters and Staff clock, 146's Kaya lines — and traced none of their own decisions. I verified
Sorin, Sanguine Bond and Exquisite Blood against primitives; Fate Unraveler, Underworld Dreams,
Howling Mine, Teferi's Puzzle Box, Master of the Feast, Kaya the Inexorable, Silverquill Silencer,
Barrowin of Clan Undurr, Briarbridge Tracker, Katilda, Rorix Bladewing, Siege-Gang Commander and
Emrakul were **not** verified (no decision here turned on their printed text beyond what the render
stated). I did not verify the emblem-stacking claim in §3-B empirically — no game in this corpus
took −2 twice; it rests on the primitive's script and on the first emblem being visibly applied. I
did not read the engine code, so on `sibling_window_asks_skipped` / `chain_windows_collapsed` I can
only report the zeros and this seat's 18 total priority windows, not which of the causes explains
them. `own_turn_windows_skipped` was 0/0/76/48/0/0 here and I did not verify what those skips
suppressed.
