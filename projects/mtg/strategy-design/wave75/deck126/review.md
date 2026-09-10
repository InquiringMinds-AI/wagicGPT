# Wave-75 deck126 review — SANGUINE BLOOD (WGB wall prison, Exquisite Blood + Sanguine Bond)

Corpus `matchups-20260910-003556-final`, SINGLE binary `wagic-7a04cd6da-w75step1`. Record **2/6**
(wave 74: 2/6). **258 model prompts / 276 records** at this seat, across six games (146/130/125/152/
123/162): prompts 47/15/122/22/27/25. **0 fallbacks of any kind, 0 `reply_truncated`, 0
`action_before_plan`, 0 `async_drops`, 0 `forced_close_unrecorded`, 0 `phase2_answer_missing`**
(recovered 2/1/1/0/1/0), `thinking: on` and a non-empty `reasoning` on every record (invariant 000
satisfied; median `reasoning_chars` 5,512, max 23,854; latency p50 82.4 s / p95 203.5 s).
`protocol_deviation` non-compliant on **2 of 258** (both quoted in §2 LOW-6), `plan_line_missing` 1
(the gameend record, not a reply), `off_protocol_bytes>0` 2.

Counters, per game (146/130/125/152/123/162): `hold_windows_skipped` 12/0/303/0/23/0 (**cast-seam in
every case**, priority 0 everywhere); `hold_released_turn` 0/0/16/0/1/0; `ask_replays_reserved`
11/0/105/6/25/11; `identical_ask_answers_reserved` 11/0/90/0/25/0 (wave 74 at this seat: **0 in all
six** — the counter is live again here and tracks the replay path 1:1 in four of the six games);
`main_phase_windows_skipped` 0/0/0/5/0/2; `own_turn_windows_skipped` 7/32/13/51/0/19;
`mana_only_windows_skipped` 153/0/6/70/0/3; `plan_names_uncastable_zone_card` 6/0/12/1/0/0;
`plan_names_stranded_card` 0/0/0/0/1/1; `reserve_decline_windows_noted` 0/0/0/0/0/2;
`blocker_forecast_rows` 22/0/0/2/0/12 (gang 8, multi 26, collapsed 6); `chain_acting_rows` 6 (146
only); `repeat_annotated_takes`, `declined_face_latches`, `wall_miss_events`,
`menu_pass_no_progress`, `sibling_window_asks_skipped`, `chain_windows_collapsed`,
`stop_reached_windows_skipped` **0 in all six**.

**Max asks in any one turn at this seat: 6** (125 game, turns 14/38/46). No loop signature. The
`[you declined this exact list N times already this turn]` note reached **N=2** once (125 game) and
N=1 elsewhere — down from wave 74's N=4.

## 1. Game by game

| # | opp | result | deciding decisions |
|---|---|---|---|
| 1 | **146** | **WON** 32–0, t22 | Bond t10 (seq16), Blood t12 (seq20) — then the opponent **exiled both halves** (Kaya `-3` on the Bond t11; Vanishing Verse on the Blood t13), and the seat re-assembled from the second copies at t20 (seq41 Blood, seq44 Bond). The guide's "A COUNTERED or EXILED copy is not on your line, so cast or tutor the next copy" is what won this game. **seq39 (t18) is the swing**: Sorin `-6` charged **six** loyalty (8→2), destroyed three permanents and reanimated all three — Barrowin, Nadaar and the opponent's **Lolth, Spider Queen** — under this seat's control (wave-74 HIGH-2 fixed, §2). **seq45 (t20) is the miss: a guaranteed win declined** (§2 HIGH-1). Won two turns later at seq59 off the one lifelink Vampire. |
| 2 | **130** | **LOST** 0–20, t20 | Burn, not combat: **zero attacks were ever declared against this seat** (`combat decision suppressed by no-legal-attacker` on every one of the opponent's turns in `game-126v130-1789018562.stderr`), so no blockers window existed to mishandle and the five walls on the board were inert. The seat also lost lands — 2 lands at seq13 (t11), **1** at seq14 (t17) — while holding Bond, Blood, Sorin and a Tutor it could never pay for. Land destruction plus reach; no decision at fault, 15 prompts. |
| 3 | **125** | **LOST** 0–38, t49 | **Pair complete from t12 and inert for 37 turns.** Bond t10 (seq14), Blood t12 (seq17); from seq18 to the last record both names are on the battlefield line and the opponent (Supreme Verdict / Path to Exile / Dream Fracture / Elixir control behind a **Lightmine Field**) held **0 creatures for the whole stretch**, so every Tribute row printed `at 0 this does nothing`. The only starter the seat could have made was a Staff of Nin: **it drew two and both were countered** (seq70 t22 Dream Fracture, seq74 t24 Cancel). THE ZERO STOP held (§3 KEPT). Two faults, neither decisive: **seq50 (t16)** cast a `[second copy:` Exquisite Blood the guide twice forbids, and **25 windows** closed with "Cast nothing right now" on menus that printed the hold row's own trigger (§3-B). |
| 4 | **152** | **LOST** −1 to 28, t15 | Curve-out aggro (Elite Spellbinder, two Brutal Cathars exiling both Perimeter Captains, two Moonrage Brutes) plus Teferi gaining 2/turn. Life 20→17→14→8→1 with the seat never holding more than one untapped blocker. Blocks were correct at both windows (seq23: one legal blocker offered, taken; seq29 at life 2: both blockers assigned, the header already reading `no block saves you`). **One guide violation, seq24 (t14, life 1): "Play no land right now"** with three lands listed and a `CRACK-BACK NEXT TURN ... would KILL you` printed — the engine re-put the land window and the seat played Sunpetal Grove at seq27, so nothing was lost. |
| 5 | **123** | **LOST** −30 to 20, t22 | Blood t11 (seq21), Bond t15 (seq24) — **pair complete on t15 and again inert**: the opponent's battlefield line reads `None` at every window from seq26 on, and the seat's own board had 0 creatures, no Staff, no Sorin, no Vampire. Killed on t21 by Intruder Alarm / Bloodline Keeper — 26 flying tokens, 53 damage. The seat's own `-30` forecast was printed correctly. The seat's Tribute at **seq22 (t13)** took the named Bloodline Keeper (rung 0, correct; +3 life). Its four "Cast nothing" closes at t17/t21 are the guide's `CAST SOMETHING EVERY TURN` violated — and the model was **right**: the render told it the only castable wall `covers 0 of 53` (§3, why no edit). |
| 6 | **162** | **WON** 40–0, t13 | The clean line. Tutor t5 → Bond (seq7/8), Bond t9 (seq13), Sorin t7 with `+1` every turn, Blood t13 (seq23), then **seq37: three lifelink Vampires declared, the ATTACK TOTAL's converter clause read correctly, and opp 14 → 0.** One departure: **seq17 (t11)** cast the last Tribute at `they control 3 creatures - they choose which one` with only the Bond out and life 21, which the guide's KEEP THE LAST ONE rule forbids — it gained 2, the Bond converted it to 2 (opp 18→16), and the other half came off the hand two turns later, so the rule pointed the wrong way and the break cost nothing (§3, why no edit). |

**2/6 read.** Both wins are the loop closing (146 off the second copies after the pair was exiled;
162 off the Vampires). Two of the four losses (125, 123) are the **same shape and it is this deck's
real weakness, not a decision fault**: the pair completes against an opponent with no creatures and
the seat holds no Staff, no Sorin and no Vampire, so a complete loop sits inert for 37 and 7 turns
while a control or combo deck wins at leisure. 130 is land destruction plus burn against a wall deck
that never gets to block; 152 is a curve-out. Play quality is up on wave 74: **50 of 51 land drops
taken**, 4/4 reveal picks, 102 `at 0 this does nothing` exposures and **0 takes**, 0 fallbacks in 258
windows against wave 74's 2 in 288.

## 2. Engine / interface / card items

**HIGH-1 — the ATTACK TOTAL's "their N untapped blockers can cover every attacker you could send"
ignores that a MENACE attacker of yours costs them TWO bodies, and it stated that false fact at the
window that would have ended a game.** Repro
`1789018559-ai_baka_deck126-0x55d75e3c48c0-vs-ai_baka_deck146.jsonl` **seq 45** (t20, `LOOP COMPLETE`
printed, opponent at 20). Four attackers offered, two of them tagged
`[menace - cannot be blocked by fewer than two creatures; they have 4 untapped creatures that could
join such a block, and no one of them can block it alone]`; the opponent has **4** untapped
creatures. The header reads:

`ATTACK TOTAL: 4 attackers listed, 10 total combat damage to a player - declaring all of them with none blocked puts them at 10. That is NOT lethal: they survive at 10 even with none of them blocked. Their 4 untapped blockers can cover every attacker you could send, so none of that damage is guaranteed.`

Covering all four attackers costs them 1 + 1 + 2 + 2 = **6** bodies and they have 4, so **at least
one attacker is unblocked under every legal assignment** — and with both halves of the loop on the
battlefield one point of damage ends the game. The per-attacker tags model menace correctly; only
the aggregate sentence does not. The model read the aggregate, weighed sending all four **twice** in
its reasoning (`"Wait, should I attack with all 4?" ... "attacking with them doesn't add much"`) and
sent two; both were blocked and the game ran two more turns. This is the exact failure class the
brief cares about — a new-ish render clause stating a false fact, at the deciding window. Cheap
falsification: any board with two menace attackers of yours and fewer of their untapped creatures
than 2×(menace attackers) + (other attackers); the fix is to count each menace attacker as 2 in
whatever comparison produces `can cover every attacker you could send`. Same file **seq 59** shows
the clause being TRUE on the same board shape (2 menace + 1 plain = 5 needed, 6 untapped), so the
comparison exists — it just uses the wrong per-attacker weight.

**MED-2 — an `<upto:3>` loyalty row prints exactly ONE `targeting <name>`, and the ability resolved
against three permanents none of which was the named one.** Same file **seq 39** (t18). The row:
`3. -6: destroy up to three and reanimate with Sorin, Lord of Innistrad targeting Silverquill Silencer [opponent's battlefield] [cost: Counters]`
and the log at **seq 40** shows what it did: `- Your Sorin, Lord of Innistrad lost 6 loyalty
counters (now 2) | - Opponent's Barrowin of Clan Undurr died | - Your Barrowin of Clan Undurr entered
the battlefield from your graveyard | - Opponent's Nadaar, Selfless Paladin died | - Your Nadaar,
Selfless Paladin entered the battlefield from your graveyard | - Opponent's Lolth, Spider Queen was
put into the opponent's graveyard from the battlefield | - Your Lolth, Spider Queen entered the
battlefield from your graveyard`. The opponent's creature count goes 3 → 1 between seq39 and seq40
and **Silverquill Silencer is the one that survived**. The guide tells the model "THE NAME IN THE
PARENTHESES IS WHAT THE ENGINE EXECUTES"; here it names a permanent the ability does not touch. It
was harmless this time (the model read the card text, not the target name, and planned "destroy and
reanimate their three main threats"), but the same row would let a model take `-6` believing it
removes the one creature it must remove. The fix shape is a row that either names all chosen targets
or names none and says the effect chooses.

**MED-3 — the closing rows of a casting menu are not in a stable order, and the prompt's own
boilerplate calls whichever is last "the row that declines".** At this seat 127 menus print
`Hold priority` then `Cast nothing right now`, and **32 print them in the opposite order** — while
every menu ends with the same sentence: `CHOICE: followed by the number of your choice (the LAST row
of this menu declines: it is a real answer, not a fallback)`. Repro of the false case:
`1789018559-...-vs-ai_baka_deck146.jsonl` **seq 4**, rows `2. Cast nothing right now` /
`3. Hold priority`, where that sentence describes the **hold** row as the one that declines — which
contradicts the hold row's own text (`pass now, and do not ask me again ... you also give up this
turn's remaining CASTING windows`) and the deck guide, both of which treat hold and decline as
different answers. 25 of this seat's 27 wrong declines (§3-B) came on menus whose last row is
`Cast nothing right now`; that is 25/127 against 2/32 on the other ordering, so I would not claim the
ordering *causes* the declines, but the sentence is wrong on 32 menus either way and should say
"the LAST TWO rows both end this window" or name the row it means.

**MED-4 — `plan_names_uncastable_zone_card` is REAL at this seat, 18 of 19.** Every hit is a PLAN
naming a card the opponent had already answered: `Sanguine Bond` at 146 seq30/37/41/42/43 (exiled by
Kaya on t11), `Staff of Nin` at 146 seq45 and at **125 seq78/94/100/101/106/117/140/142/181/183/200**
(both copies countered into the graveyard on t22 and t24, and the model kept planning around them
for 20 turns). The residue is **152 seq15** (`Chromatic Lantern`) — a **false positive**: the Lantern
was in exile under Elite Spellbinder and the menu offered
`Cast Chromatic Lantern {5} [from exile] {castable from exile - your card, exiled by their Elite Spellbinder}`,
which the seat took and cast. So CL P17's "upper bound" framing holds: at this seat the exile-castable
case is the only class that inflates it.

**MED-5 — the hold row's P19 budget holds with one 726 B outlier, and the overage is load-bearing.**
179 prompts render a hold row; `[HOW A HOLD ENDS:` appears **exactly once** in every one of them
(0 duplicates). Longest hold row: **726 B**, `...vs-ai_baka_deck125.jsonl` **seq 228** (t49) — the
extra bytes are the group
`{answers the stack: NO - 1 damage is ALREADY ON THE STACK, and taking this row lets it resolve: that puts you at 0 and KILLS you}`,
which is true and is the most important thing on the menu. Every other hold row is under 650 B.

**LOW-6 — the two protocol deviations, both quoted, both consumed correctly.**
(a) `...vs-ai_baka_deck146.jsonl` **seq 22**, `prose_outside_two_lines`, 823 `off_protocol_bytes`:
the reply opened with a paragraph of board analysis before the PLAN/CHOICE pair (`The opponent has
Kaya the Inexorable (2 loyalty) and a Goblin (1/1) on the board. I have Exquisite Blood on the
battlefield, but Sanguine Bond was exiled by Kaya's -3 ability...`). Choice 1 was still read.
(b) same file **seq 46**, `unlabelled_plan`, 106 bytes: `THE PLAN: Attack with the lifelink Vampire
to trigger the complete life loop and win the game immediately. / CHOICE: 3 (Cast nothing right
now)` — the label is `THE PLAN:`, not `PLAN:`. Choice 3 was read; the record is nonetheless counted
`plan_line_missing`-adjacent by the deviation class. 2 of 258, no cost.

**LOW-7 — the `-2: emblem` row prints a target it does not have.** Every Sorin menu at this seat
(13 windows across 146/152/162) renders row 2 as
`-2: emblem: "creatures get +1/+0" with Sorin, Lord of Innistrad targeting Sorin, Lord of Innistrad [your battlefield]`.
Sorin's `-2` gives its controller an emblem and targets nothing. Cosmetic here, but it is the same
self-target shape CM F1 removed from the `-6`, still present on the `-2`.

**LOW-8 — `reaches 0 in N more turns` uses a plural template for one source.**
`...vs-ai_baka_deck146.jsonl` **seq 27 / seq 28** (t14):
`{the clock you already control: your Staff of Nin deal 1 damage a turn between them - at that rate alone the opponent reaches 0 in 21 more turns, with no card spent}`
— one Staff, `deal ... between them`. The arithmetic is right (opponent at 21, 1/turn) and no lethal
was stated beside it, so P10 is otherwise clean at this seat: 2 exposures, 2 correct.

**Wave-74 items, re-checked on this corpus.**
- **HIGH-1 (Exquisite Blood falsely tagged `[cannot pay now]` with a Battlement untapped) — FIXED,
  verified, CK's prediction of 0 false tags PASSES at this seat.** I checked every prompt carrying
  `cannot pay now` against the seat's own battlefield line. Only five such prompts have an Overgrown
  Battlement on the line at all (125 seq15/18/24, 162 seq24/36) and in every one the Battlement is
  `[tapped - cannot attack or block this turn]`. The one prompt with an *untapped* Battlement beside
  a `cannot pay now` tag is **162 seq20**, where the Battlement reads `[summoning sick - cannot
  attack this turn, but CAN block]` — a summoning-sick creature cannot use `{T}: Add {G}`, so
  excluding it from `1 untapped source` is correct. Across the other 30 windows with an untapped,
  usable Battlement on the line (146 seq33-59, 125 seq13-17, 152 seq7-29, 162 seq22-23) **no
  `cannot pay now` tag is printed at all**. Exquisite Blood was cast from hand in four of the six
  games this corpus (146 t12 and t20, 125 t12, 123 t11, 162 t13) against **never** in the wave-74
  game that defect decided.
- **HIGH-2 (Sorin `-6` charging 3 loyalty, doing nothing, and not stopping a second activation) —
  FIXED, verified.** 146 seq39/40: **6** counters spent (8→2), three permanents destroyed, three
  reanimated under the seat's control, and no second Sorin activation that turn (the seat's next
  priority take, seq40, is **Lolth's** `-3`, a different walker). **No self-target anywhere**: across
  13 Sorin menus the `-6` names `Silverquill Silencer [opponent's battlefield]` (146 seq33/35/38/39),
  `Kaya the Inexorable [opponent's battlefield]` (146 seq27) and `Wall of Omens [your battlefield]
  {this hits YOUR permanent}` (162 seq36) — Sorin is never among them, and the own-permanent case
  carries its warning. CK's P4 prediction PASSES.
- **MED-3 (a `CHOICE:` sharing the PLAN's physical line rejected) — no exposure.** 0 unparsed replies
  at this seat this corpus and no reply put a `CHOICE:` on the PLAN line, so P9/F3 is UNTESTED here.
- **MED-4 / LOW-5 / LOW-6 (transport fallback; `main_phase_windows_skipped`;
  `identical_ask_answers_reserved` dead at 0) — all three moved.** 0 fallbacks and 0 `async_drops`
  this corpus; `main_phase_windows_skipped` is 5 (152) and 2 (162) and 0 in the other four — I cannot
  cross-reference the counted `(turn, phase)` pairs from the translog alone and leave P13 to the
  engine seat; `identical_ask_answers_reserved` is **non-zero in four of six games** (11/90/25/…)
  and tracks `ask_replays_reserved` closely, so wave 74's "dead increment?" question is answered:
  it fires.
- **LOW-4 (hold taken on a menu printing a live activated row) — DOES NOT RECUR.** Every priority
  menu at this seat that printed a Sorin, Lolth or Staff row was answered with that row or with a
  deliberate `0. Pass priority` into a combat the seat wanted (146 seq58, passing to reach the
  winning attack at seq59). 0 holds over a live loyalty row in 6 games.
- **O16 `(keeping 7)` — 7/7 correct.** `O9` own-clock tag — 2 exposures, 2 correct (LOW-8).
- **UNTESTED here:** ghostform gloss (the tag appeared on the opponent's Silverquill Silencer but the
  seat never targeted it), `ANNOUNCE_X`, MDFC land menus (**0** `MDFC` and **0** `OTHER FACE of` in
  any prompt), the P5 loop-fed self-damage row (`you would be at 0 - this row feeds their chain`
  never printed), P11 `another copy of`, P15 X ladders, P16 short names, P7/F8 crack-back-cover
  vs legend-rule agreement (12 `crack-back cover` exposures in the 146 game, no legend-rule duplicate
  on the same board to disagree with).

## 3. Guide verdict: **EDIT** (`strategy.txt`, **19,937 B**, down 59 B from the live 19,996 — ceiling 20,000)

Two changes; two cuts more than pay for them and the guide **shrinks**. Both changes are paid by a
decision this corpus shows going wrong; three other rule-vs-play conflicts are listed below as
deliberate REJECTs.

**A. ATTACKING has no rule for the attackers this deck now acquires, and no rule that a complete
loop makes one point through the whole game. Paid by `...vs-ai_baka_deck146.jsonl` seq 45 (t20).**
The section opens "All sixteen creatures are Defenders and can never attack", and its win-button
sentence is scoped to Vampires — but Sorin's `-6` now works (§2), and at seq45 it had handed this
seat a 3/3 Barrowin, a 3/3 Nadaar and two 2/1 menace Spiders while `LOOP COMPLETE` was printed. Four
attackers against four untapped blockers, two of them menace, is an unblockable attacker under every
legal assignment and therefore the game; the seat sent two, both were blocked, and it won two turns
later instead. The menace arithmetic is read off the A-lines the model already has, so this is a
rule about what those lines mean, not doubt about a rendered number.
before: `THE WIN BUTTON OUTRANKS EVERY STOP BUT THE LIGHTMINE ONE: with BOTH names on your battlefield send EVERY Vampire whatever the tag says - lifelink pays you for damage dealt to a BLOCKER, and that starts the loop.`
after: `THE WIN BUTTON OUTRANKS EVERY STOP BUT THE LIGHTMINE ONE: with BOTH names on your battlefield ONE POINT THROUGH ENDS THE GAME, so send EVERY creature the A-lines offer, not only the Vampires - Sorin's -6 hands you attackers that are not walls, and lifelink pays you for damage dealt to a BLOCKER as well. COUNT WHAT BLOCKING YOU COSTS THEM: an attacker of yours tagged [menace] takes TWO of their untapped creatures and every other attacker takes one, so once your attackers need more bodies than they have untapped, one of yours is unblocked and that is the game.`

**B. THE HOLD ROW's decline exception is void under the hold's own semantics, and it licensed 27
wasted windows. Paid by 25 windows in `...vs-ai_baka_deck125.jsonl` (seq 16, 26, 41, 57, 66, 71, 73,
94, 101, 113, 125, 131, 132, 136, 151, 157, 161, 163, 170, 176, 193, 202, 204, 218, 226) and 2 in
`...vs-ai_baka_deck123.jsonl` (seq 33, 48).** Every one of those menus printed
`NO LIVE CAST ROW ON THIS MENU` or `[you declined this exact list N times already this turn]` — the
guide's own trigger to take the hold row — and the seat answered "Cast nothing right now" instead.
The 125 game is where it costs: 122 model calls over 49 turns, most of them re-puts of a menu whose
only cast row was a Tribute reading `at 0 this does nothing`. The guide's stated reason to decline
("holding Tribute for a creature yet to attack") is exactly the case where a hold is free: the hold
row's own text says the window re-opens when a row appears, disappears, or names a different card,
cost or target, and a creature arriving on their line turns that dead Tribute into a live row, which
re-opens the menu by itself.
before: `Decline only when you WANT asking again - holding Tribute for a creature yet to attack.`
after: `A HOLD NEVER COSTS YOU THE WINDOW YOU ARE WAITING FOR: it ends the moment a row appears, disappears, or names a different card, cost or target, so a Tribute reading "at 0 this does nothing" re-opens this menu by itself the turn they have a creature.`

**What paid for the bytes.** (1) `THE ATTACK TOTAL UNDERSTATES YOU WHILE SANGUINE BOND IS OUT ...
Double it yourself before you believe a "NOT lethal"` — **321 B**. The render now prints the
converter arithmetic itself and printed it correctly in **4 of 4** windows this corpus that offered a
lifelink attacker with the Bond out (162 seq15 `they are at 18, not 19`, seq21 `at 12, not 14`,
seq37 `at 8, not 11`; 146 seq59 `at 14, not 15`) — plus 2 of 2 in wave 74, and it correctly printed
**no** converter clause at 146 seq45, where the four offered attackers had no lifelink among them.
Six of six across two corpora, zero false; wave 74 kept this clause explicitly as a one-corpus hedge,
and the second clean corpus retires it. (2) `THE EMBLEMS STACK AND A SECOND ONE CAN BE THE KILL: a
fresh +1 token is summoning sick ... take -2 AGAIN instead of the +1` — **253 B**. The `-2` was
offered on 13 Sorin menus at this seat and taken **zero** times, and the board shape this sub-rule
describes — a Vampire already able to attack whose doubled total crosses lethal with one more +1/+0 —
has not occurred in either corpus. The rule it hangs off (`HIS -2 EMBLEM BEATS A 1-DAMAGE BOARD ...
take it in the first window that prints it whenever anything on their line kills a 1/1 on sight`)
survives unchanged and is the half that fires.

**What KEPT and earned it.** THE ZERO STOP: **102 exposures, 0 takes**, including ~40 windows in the
125 game where it was the only cast row on the menu for 37 turns. LAND DROP: **50 of 51**. Rung 1's
"A COUNTERED or EXILED copy is not on your line, so cast or tutor the next copy": the 146 game was
won on it after both halves were exiled. Rung 0 and the CRACK-BACK floor: they are what made the
seat's four "Cast nothing" closes in the 123 game correct against the WIN BUTTON's blanket "CAST
SOMETHING EVERY TURN". THE LOOP NEEDS A STARTER: correct and repeatedly load-bearing — the 125 and
123 games are both lost to exactly the situation it describes. MULLIGAN + BOTTOMING: 7 windows, all
by the letter (123 seq2 shipped a seven, kept the six, bottomed a Perimeter Captain). Rung 4's
reveal rule: 4 of 4. THE SILENCER TAG: exposed (146 t14, `[named: Tribute to Hunger]`) but never with
a half on the menu — kept, untested. The LIGHTMINE paragraph: the card was **live on the opponent's
battlefield for most of the 125 game** and the seat had no attacker to test it with — kept, and now
demonstrably not dead text.

**REJECTED edits.** (1) *KEEP THE LAST ONE ONCE A HALF IS ON YOUR LINE* pointed the wrong way at 162
seq17 (last Tribute, Bond alone out, life 21, cast anyway — the Bond converted the 2 life into 2
damage and the other half came off the hand two turns later). One instance, zero cost, and narrowing
it to "Exquisite Blood alone" would trade a live rule for an untested one. (2) The `[second copy:` cast
at 125 seq50 and the land-drop decline at 152 seq24 are both violations of rules the guide already
states flatly and twice over; restating them is churn, and neither decided anything. (3) HIGH-1 gets
**no** guide text about distrusting the ATTACK TOTAL — edit A teaches the model to count bodies off
the A-lines, which is arithmetic it can do, not doubt about a rendered value.

No general-guide proposal and no skill proposal. A and B are deck-local; HIGH-1, MED-2, MED-3, MED-4,
MED-5 and LOW-6/7/8 belong to the engine seat, and none of them restates an existing amendment.

## 4. Not checked
I did not read engine source, so every mechanism sentence in §2 is a hypothesis stated as a
falsifiable repro. I did not build the fixture that would settle HIGH-1 or MED-2. I read opponent
seat files only where a fact of mine depended on one, and traced none of their decisions, so I
contribute nothing on the other decks. I audited `.stderr` only for `game-126v130-1789018562.stderr`
(and only for the combat-suppression and hold lines), so I contribute nothing on `deadline_pct`,
`transport`, `askreplay/`, the `{X}`-announcement payment loss, the stderr forced-close count, or
`Parser returned NULL` noise. I could not cross-reference `main_phase_windows_skipped` (5 in the 152
game, 2 in 162) against rendered `Casting decision (Main phase N, YOUR turn)` headers, because the
counter is only carried on the gameend record and the skipped windows produce no record — P13 is
UNTESTED from this seat. `own_turn_windows_skipped` reaches 51 (152) and 32 (130) and I did not
verify what those skips suppressed. I verified Sorin, Sanguine Bond and Exquisite Blood against the
primitives in wave 74 and re-used that; **Kaya the Inexorable, Vanishing Verse, Silverquill Silencer,
Lolth, Barrowin of Clan Undurr, Nadaar, Lightmine Field, Dream Fracture, Elixir of Immortality,
Elite Spellbinder, Brutal Cathar, Moonrage Brute, Briarbridge Tracker, Teferi Who Slows the Sunset,
Bloodline Keeper, Intruder Alarm, Spark Spray and Hammer of Bogardan were NOT verified** beyond what
the render stated — in particular the HIGH-1 menace claim rests on the A-line tags
(`[menace - cannot be blocked by fewer than two creatures]`), not on the primitives.
