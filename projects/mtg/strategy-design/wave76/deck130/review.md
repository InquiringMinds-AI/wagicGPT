# Wave 76 — deck 130 (mono-red land destruction) — per-deck review

Corpus `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260910-141526-final` (consolidated:
2 first-run games + 19 reruns, single binary `archives/wagic-d372a1153-w76step1`, `--thinking on`).
All six of this seat's games are RERUNS (start epochs 1789074078–1789074169), all finished
naturally. Live guide = the wave-75 edit (19,995 B).

**Seat census.** 264 model decisions over 6 games (wave 75: 385 — shorter games), **100% with
native reasoning** (median `reasoning_chars` 6,486, max 21,177), **0 fallbacks**, 0
`reply_truncated`, 0 `action_before_plan`, 264/264 `protocol_deviation: compliant` at the transport
layer and 264/264 `curl=0,http=200,empty=0`. Latency p50 121.6 s. Counters (sums over the six
gameends): `ask_replays_reserved` 12, `identical_ask_answers_reserved` 8, `hold_windows_skipped` 86
(cast 49 / priority 37), `hold_released_turn` 9, `main_phase_windows_skipped` 2 (126 only),
`own_turn_windows_skipped` 94, `plan_line_missing` 13, `off_protocol_bytes>0` 13,
`protocol_deviation_replies` 13, `plan_names_uncastable_zone_card` **4** (wave 75: 14),
`plan_names_stranded_card` 0, `async_drops` 12, `phase2_answer_recovered` 2 / missing 0,
`forced_close_unrecorded` **1** (126), `crossphase_identical_reputs` **0**,
`force_close_arms_refused/deferred/defer_bound_hits` 0/0/0, `repeat_annotated_takes` 4,
`blocker_forecast_multi` 10 / rows 32 / gang 0 / collapsed 0.
Prompt bytes: max **17,199**, median 9,529, **0 over 20 KB**. Hold rows: 155 renders, **median and
max both 596 B** (wave 75: median 597, max 728) — CI's `< 650 B at every window` now holds at the
tail too. Loop family dead: no (turn, phase) in the seat reaches 10 asks.

---

## (1) Game-by-game, and the deciding decisions

| Opponent | Result | Turn | Final life | Decisions |
|---|---|---|---|---|
| 123 | **WIN** | 17 | 20 / -1 | 32 |
| 125 | **WIN** | 31 | 19 / -4 | 70 |
| 126 | **WIN** | 23 | 14 / -1 | 60 |
| 162 | **WIN** | 19 | 20 / -3 | 58 |
| 146 | LOSS | 12 | -2 / 16 | 17 |
| 152 | LOSS | 14 | 0 / 17 | 27 |

**4/6 (wave 75: 2/6).** Hands are real; I priced decisions, not the record. What the wave-75 edits
did and did not decide is in §3.

### vs 146 — LOSS on turn 12. **The paid decision: a land row taken over a kill on the same menu**
`1789074133-...-vs-ai_baka_deck146.jsonl` **seq 14** (t10, Main 1, my life 16, opp 18, 3 mana).
Their line: Nadaar 4/4 vigilance, Barrowin 3/3, Goblin 1/1; header
`CRACK-BACK NEXT TURN: 3 of their creatures ... for up to 8 - you would be at 8 - you control NO
creature`. The menu's row 1 is
`Cast Hammer of Bogardan {1}{r}{r} ... {kills: Barrowin of Clan Undurr, Goblin - and 3 to the
opponent at life 18 leaves them at 15}`. The seat cast **Molten Rain** (row 4) at a tapped
Grimclimb Pathway instead, taking them 4 lands -> 3 and 18 -> 16. At t12 the same three creatures
(now 4/4, 4/4, 2/2) read `for up to 10 ... that would KILL you`, and by **seq 17** nothing on the
menu survived it: Hammer kills the Goblin (-2), Blastminer covers 4, and 3 mana buys only one.
Dead on t13.

Hammer at Barrowin at seq 14 takes the crack-back from 8 to 5 and leaves the seat at 11 with a
Blastminer to come. The reasoning shows the miss exactly: it enumerated Hammer
(`Can kill the Goblin (1/1) or Barrowin (3/3). If I kill Barrowin, I still have 0 mana for their
turn`), then discarded it — `Is there any reason to cast Hammer? No.` — and decided the window
entirely out of **THE LAND PLAN**, whose death test it ran correctly and out loud
(`N=4, L=16, P=8. None hold. So land destruction is alive.`) and then read *alive* as *do it*.
ORDER OF OPERATIONS entry 2 outranks entry 3 and would have answered it, but the model never walked
that list at this window. **Guide item -> Edit A** (STRATEGY: the surfaces were true and complete
except for §2 HIGH-1).

### vs 152 — LOSS on turn 14. Matchup; no decision I can price as the loss
`1789074159-...-vs-ai_baka_deck152.jsonl`. Mulliganed to 6, then three lands and two Talismans for
the whole game against Ranger Class (a Wolf that grew 2/2 -> 6/6), Augur of Autumn and Elite
Spellbinder. Land destruction hit three lands (seqs 11–14, 6/19 -> 17/17) and never mattered;
Spark Spray correctly killed Luminarch Aspirant (**seq 15–17**) before it could pump again.
Siege-Gang landed at t12 into a stated 7 crack-back with the full `{crack-back cover:}` paragraph
(**seq 20**) and was answered by Fateful Absence.
Two decisions I checked and cleared: **seq 27** (t14, 3 life) took `Draw 1 with Pyrite Spellbomb`
over row 2 `Deal 2 damage ... targeting Elite Spellbinder {right now: takes 2 damage - DIES}
{removes 3 from the CRACK-BACK total above: 11 -> 8 - you would be at -5; that still KILLS you}` —
the render's own arithmetic says the kill does not save the seat, so the draw is defensible; and
**seq 28** answered `ATTACK: none` with two Goblins under a stated lethal, which is the row wave-75
Edit C was written for (see §3).
The one rule violation: both **seq 27** and **seq 28** write plan lines that say the game is lost
(`survival is mathematically impossible`, `Survival is impossible`) — the PLAN LINES section
forbids exactly that sentence. It changed no choice here (both choices were right), so it is
**not paid** and I did not edit for it.

### vs 162 — WIN on turn 19, but one row taken that the guide already closes
`1789074169-...-vs-ai_baka_deck162.jsonl`. Land destruction plus Blastminer took four of their
lands; Siege-Gang's Goblins and Rorix closed it. The misplay is **seq 60** (t20, X-announce menu,
opp at 3). One window earlier (**seq 58**) the seat wrote a correct plan —
`Cast Starstorm at X=4 to destroy the blocker, then attack with Rorix and Siege-Gang Commander for
lethal` — over a row that priced X=4 as `kills THEIRS: Shield Sphere; YOURS: Dwarven Blastminer,
Siege-Gang Commander` with Rorix surviving. At the X menu it took **row 1**,
`X = 1 {X pricing: kills THEIRS: none; YOURS: Dwarven Blastminer}`, reasoning
`X=1 is best. It deals 1 damage to Shield Sphere (making it 0/3) and doesn't kill my creatures.`
The row says its own kill list names a creature of the seat's. This is **PERCEPTION** against a
true, short, printed clause — `Never a row killing none` and the marked row `[<- the only X that
kills anything of THEIRS ... THEIRS 1 / YOURS 2]` were both on screen and both ignored. Two render
contributors are in §2 (HIGH-2, MED-1). It cost a Blastminer and nothing else: Shield Sphere could
block only one of Rorix and Siege-Gang and either unblocked body is lethal at 3.

### vs 125 — WIN on turn 31 (wave 75: LOSS on turn 53 to this deck)
`1789074093-...-vs-ai_baka_deck125.jsonl`. Blastminer activations and three land kills held the
board to zero permanents by t20; the kill was the Siege-Gang face row on an empty line (**seqs
53–56**, opp 14 -> 4), exactly the clock wave-75 Edit B/B2 authorises and gates. **No Hammer was
ever drawn**, so the EMPTY-HAND RULE and Edit D went untested.
One real inefficiency, **seq 56** (t22 Upkeep, opp at 6, their line 0 permanents): the third
activation of the turn sacrificed **the Commander himself** — the log reads
`Your Siege-Gang Commander died / Your Siege-Gang Commander dealt 2 damage to the opponent (now 4)`.
The four bodies the mana paid for that turn total 8 against 10 life, so the row was **not** part of
a lethal count; spending the Commander converted a body that attacks for 2 every turn into one 2,
and the opponent then sat at 4 from t22 until the seat finally drew Rorix on t32. The guide's clock
clause licenses it (`once their line reads 0 creatures ... keep paying while the mana lasts`) and
names no last body. **Guide item -> Edit B.** Not a loss, so the bytes are small.

### vs 123 and vs 126 — WINS, no misplay worth an edit
123: Starstorm X=3 at t12 swept Bloodline Keeper and a Vampire off a stated 5 crack-back (the new
Q5 clause did the arithmetic on the row — §3), then Rorix from t14 and a five-body swing on t18.
126: a Hammer at Pride Guardian and a Starstorm X=4 cleared the wall board the WALL CHECK paragraph
is written for, then Rorix, Siege-Gang and a Hammer recursion loop (seqs 47–60) took them 21 -> -1
while the seat never dropped below 14.

---

## (2) Engine / interface / card items

### HIGH-1 — Q5's kill-row crack-back clause fires on sweeps and NOT on a targeted-removal row, and this wave that gap paid for a loss
The wave-76 fix landed and is correct where it fires: `123` **seq 20**, on the Starstorm cast row,
`{crack-back cover: the CRACK-BACK NEXT TURN line above is 5 from 2 of their creatures and puts you
at 15. This row REMOVES 2 of those 2 attackers - a dead creature does not attack, so that takes 5
off the total, leaving 0 -> you would be at 20, which you SURVIVE ...}` — arithmetic true, seat took
the row, won. lane-CO's residual note 2 is the exact hole: **the Hammer shape is deliberately not
covered**, and `146` **seq 14** is that hole rendering under a stated crack-back:

```
CRACK-BACK NEXT TURN: 3 of their creatures will be able to attack ..., for up to 8 - you would be
   at 8 - you control NO creature, so every point of that is unblocked
1. Cast Hammer of Bogardan {1}{r}{r} ... {kills: Barrowin of Clan Undurr, Goblin - and 3 to the
   opponent at life 18 leaves them at 15}   <- no crack-back clause
```
Row 4 on that same menu (Molten Rain) has none either, and the seat took row 4. The objection that
a targeted removal's victim is the seat's own next choice does not apply to a row whose printed
`{kills: A, B}` list is **exhaustive for that row's damage**: every creature Hammer can kill here
is named, so the honest clause is conditional and still decisive —
`{crack-back after this row: the line above is 8 from 3 of their creatures; of those it can remove
Barrowin (3) or Goblin (1) - at best the line becomes 5 -> you would be at 11}`. The corpus-wide
census belongs to the engine seat; in this seat the score is 1 sweep row covered, **2 kill rows
uncovered on the menu that lost a game**.

### HIGH-2 — the `[hold check: N rows above are new]` bracket counts something that is not new rows, and prints non-zero when nothing appeared
Three clean repros, each a pair of consecutive records at an unambiguous seam
(`Casting decision (Main phase 1, YOUR turn)` both times, nothing of that seam between them):

* **`146` seq 11 (t8) -> seq 14 (t10).** Row sets are **identical** — {Molten Rain, Hammer,
  Talisman, Stone Rain, hold, decline} both times; only the ORDER changed (Molten Rain moved from
  row 1 to row 4). seq 14 prints `[hold check: 3 rows above are new since the last window I asked
  you at this seam ...]`. **Nothing is new.** This is the menu that lost the game (§1).
* **`162` seq 53 -> seq 54** (same turn 18, same seam). seq 53 rows {Starstorm, Hammer, Rorix,
  hold, decline}; the seat cast Rorix; seq 54 rows {Starstorm, Hammer, hold, decline}. seq 54
  prints `1 row above is new`. **Zero appeared; one disappeared.**
* **`126` seq 59 -> seq 62** (same turn 24). {Molten Rain, Hammer, ...} -> {Molten Rain, ...} after
  Hammer was cast; seq 62 prints `1 row above is new`.

The shape across all three is that the number tracks rows that CHANGED — vanished, or moved index —
while the sentence says *new*. CN's Q1 fix made the referent honest ("the last window I asked you at
this seam") and the residual is now in the **count and the noun**, not the referent. I could not run
the corpus-wide 0/N census the brief asks for from transcripts alone: the engine's seam key is not
in the record, and my two independent reconstructions of it (phase-labelled header; header plus
normalised row identity) both put the mismatch rate near a third of pairs with errors in BOTH
directions — which is a symptom of my key, not a measurement. The three specimens above need no
seam key. Engine seat: key it properly and count.

### MED-1 — the X-announce window drops the plan echo and draws its worked example from row 1
`162` **seq 60** and every other X menu in this seat (`123` seq 22, `126` seq 28): **3 of 3 carry no
`THE PLAN YOU LAST STATED` line**, against 128/155 `ask`, 58/64 `priority`, 20/20 `attackers` and
18/18 target menus that do. The X menu is the second half of a two-window cast whose first half is
where the seat states *why* it is casting — at `162` seq 58 that plan named X=4 and the reason — and
it is the one window that does not show it back. Second, that window's closing example reads
`e.g. "CHOICE: 1 (X = 1)"`, written out from row 1 as everywhere else; but here the option number IS
the X, so the example demonstrates the smallest X, and row 1 was
`{X pricing: kills THEIRS: none; YOURS: Dwarven Blastminer}`. The seat answered `CHOICE: 1 (X = 1)`.
Fix shape: echo the plan on the X menu, and draw the X menu's worked example from the **marked** row
(`[<- best X for this cast: X=N]`) rather than from row 1.

### MED-2 — `plan_names_uncastable_zone_card` is down 14 -> 4 in this seat, and all four remaining are future-turn clauses
Q9's self-recursion exemption removed the whole Hammer family: **0 of 4** name Hammer (wave 75:
12 of 14). The four that remain (`125` seqs 14, 32, 33, 49) all name a graveyard card inside a
*next-turn* clause, never as this window's action:
`PLAN: Attack with Dwarven Blastminer. Next turn, untap lands, ... destroy another Seachrome Coast
with Stone Rain or Blastminer` (seq 14, Stone Rain in graveyard),
`PLAN: Pass this turn. Next turn, draw and look for Siege-Gang Commander or Rorix Bladewing`
(seq 49). The predicate reads the whole plan; the honest scope is the plan's **next-action clause**.
One of the four is a genuine model error worth keeping visible (seq 49 proposes to *find* a card
whose only printed copy is in its own graveyard listing), so the counter is not pure noise — but 3
of 4 are legal English about a future draw.

### MED-3 — `plan_line_missing` is the dropped `PLAN:` LABEL, 13 of 13, sixth wave unchanged
All 13 records carry `off_protocol_bytes` equal to the dropped label's sentence and a first line
that IS a plan. Verbatim: `125` seq 60 `"\n\nDestroy their only land to maintain tempo and prevent
them from casting spells or playing lands.\nCHOICE: 1 (Cast Card Normally)"`; `126` seq 61
`"\n\nPlay Mountain to enable casting Molten Rain for lethal.\nCHOICE: 1 (Play Mountain)"`;
`162` seq 55 `"\n\nRemove Shield Sphere #1 to clear a blocker for Rorix.\nCHOICE: 2 (Shield Sphere
#1)"`. **This wave the residue is zero**: unlike wave 75 (`146` seq 29, a genuinely absent plan),
every one of the 13 is a labelled-plan-minus-label. Accepting an unlabelled first line as the plan
zeroes 13 of 13 here. `protocol_deviation_replies` 13 = `plan_line_missing` 13 in this seat.

### LOW-1 — one seq gap with no counter naming it
`152` has no record at **seq 24**, with `async_drops` 0, `forced_close_unrecorded` 0 and
`phase2_answer_recovered` 0 in that game. In the other five games the gaps reconcile
(123: 2 gaps = 1 drop + 1 recovered; 125: 4 = 3 + 1; 162: 3 vs 4 drops; 126: 2 vs 4 drops). The seq
counter advanced without a record and without any counter claiming it.

### LOW-2 — `forced_close_unrecorded` 1 in 126, and the stderr cannot be attributed to a seat
`game-126v130-1789074105.stderr` carries exactly **one** `AIPlayerGPT: unclosed <think>
(budget/truncation); forcing the answer` against `forced_close_unrecorded: 1` and
`phase2_answer_recovered: 0` in the deck130 gameend — but that stderr is shared with the deck126
seat, so the arm is not attributable from my side. 0 fallbacks in the seat, and I found no
turn/phase gap I can prove. The other five games have 0 `unclosed <think>` lines.

### Verifying the wave-76 lanes against this seat
- **Q4 (ATTACK TOTAL knapsack / floor)** — **PASS**, 6/6 and 8/8. Six
  `Their N untapped blockers can cover every attacker you could send` claims (`126` seqs 10, 16;
  `162` seqs 25, 30, 41, 49), all on boards where their untapped blocker count is at or above the
  attacker count and no attacker has menace or evasion; eight
  `At least N damage lands whatever they block` floors, all arithmetically true, including the one
  that closed the 162 game (`162` seq 56, floor 7, opponent 10 -> 3).
- **Q1** — see HIGH-2. **FAIL** in this seat on the count, on three specimens.
- **Q5** — **PASS on the sweep arm** (`123` seq 20, arithmetic verified), **the named Hammer
  residual is live and cost a game** (HIGH-1).
- **Q9** — **PASS on the self-recursion exemption** (0 Hammer fires, wave 75 had 12); residual in
  MED-2. `plan_names_stranded_card` 0.
- **Q13** — `crossphase_identical_reputs` **0** in all six games: **UNTESTED here**.
- **Q7** — `main_phase_windows_skipped` 2, both in `126`; the stderr's only skip lines are
  `AIPlayerGPT[ph2]/[ph4]: only mana production and no pending cost; auto-passing without a model
  call` (10 in that game), which is a different, honest exit. 0 false counts I can see.
- **Q8 / F2** — `force_close_arms_refused/deferred/defer_bound_hits` all **0**: **UNTESTED**.
- **Q3** — 0 `LOOP COMPLETE`, 0 `reaches 0 in N more turns`, 0
  `NOT lethal: block only where the trade favors you` in the seat: **UNTESTED**.
- **Q2 (`- THEIRS:` roster, copy-row fold)** — 0 `- THEIRS:` rosters and 0 folded copy rows in the
  seat: **UNTESTED**. Max prompt 17,199 B, 0 over 20 KB.
- **Q6 (forced-sacrifice `off YOUR life`)**, **Q10 ghostform**, **Q11 loyalty rows**, **Q14 3-life
  land** — no instance in this seat: **UNTESTED**.
- **Q12 (closing rows in one stable order, decline last)** — every menu I read ends hold-then-
  decline (cast menus) or hold-then-`0. Pass priority` (priority menus); **0 reversed** in 264
  prompts.
- **Q15** — 13 `off_protocol_bytes>0`, all the MED-3 label drop, 0 prefill echoes. Hold rows
  596 B, 0 over 650.
- **P14** — the corpus's single `repeat_count_reask` is not in this seat.
- **`{X}-announcement payment loss`** — `/usr/bin/grep -c 'dropped after its X was announced'`
  returns **0** in all six of my stderr files.

### Card facts — verified, no item
Starstorm, Siege-Gang Commander (including that the Commander is himself a Goblin and is offered as
a sacrifice target for his own ability — correct against Oracle), Rorix Bladewing, Hammer of
Bogardan (damage and the upkeep-only return), Pyrite Spellbomb's two modes and their two different
mana costs (`{r}` bills the Talisman's 1 life, `{1}` does not — the render gets this right at
`152` seq 27), Dwarven Blastminer's nonbasic-only restriction, Stone Rain, Molten Rain's 2 to a
nonbasic's controller, Lay Waste, Spark Spray, Forgotten Cave, Talisman of Impulse. Opponent-side:
Nadaar's vigilance and venture counters, Barrowin, Shield Sphere, Fog Bank, Ranger Class's level
counters and its `(THEIR card, written from THEIR seat)` gloss, Elite Spellbinder (3/1 flying),
Lair of the Hydra's creature-land clause. No Scryfall disagreement worth reporting.

---

## (3) Guide verdict: **EDIT** — revised guide at `wave76/deck130/strategy.txt`

**19,929 bytes vs the live 19,995 — net -66**, three edits (+595) paid by seven trims (-661). Every
addition is arithmetic the window already prints, not caution, and each is a precondition rather
than a concession (A334). No citation, count or history is in the guide (A332).

**Edit A — THE LAND PLAN gains the gate at the place the decision is taken.**
Before (section opens): `Land destruction is TEMPO: worth a card while their battlefield shows FOUR
OR FEWER lands ...`
After (inserted as the section's first sentence, before that line):
`A LAND ROW NEVER OUTRANKS A KILL ON THE SAME MENU: while a damage row on this window names a
creature of theirs that it kills ("kills:", "- DIES", a "kills THEIRS" list), that row is the cast
and the land waits. A destroyed land does not stop an attack; a dead creature does.`
Paid by `146` **seq 14**. ORDER OF OPERATIONS already ranks entry 2 above entry 3; the transcript
shows the model deciding the window out of THE LAND PLAN's death test and never reaching that list,
so the rule is stated where it is read — the same repair wave-75 Edit D made for the Hammer
lifegain gate. Paired trim: entry 2's `A token-MAKER on their line ... is what this entry exists
for and outranks entry 4.` loses `is what this entry exists for and` — that clause narrowed entry 2
to the token-maker case, and the row that was skipped killed a plain attacker.

**Edit B — the Commander is named as the last body on his own sacrifice list.**
Before: `A "[repeat: ... N times already]" tag is a count, not a refusal: keep paying while the mana
lasts.`
After: same, plus `THE COMMANDER IS HIMSELF A GOBLIN and the LAST body on that list: sacrifice him
only on the turn the bodies your mana pays for, counting him, reach "Opponent life:". Any other
turn he stays and attacks for 2 - the same 2 again next turn.`
Paid by `125` **seq 56**: four bodies against 10 life is not a lethal count, and the third
activation ate the 2/2 that was the seat's only clock for the following ten turns. The clock clause
is otherwise unchanged and still fires on an empty line — it is what won that game.

**Edit C — the Siege-Gang gate counts attackers, not creatures.**
Before: `"{1}{R}, Sacrifice a Goblin: 2 damage" IS REMOVAL, NEVER A CLOCK, WHILE THEIR LINE NAMES
ANY CREATURE: ... IT BECOMES YOUR CLOCK once their line reads 0 creatures, ...` (and, in the PLAN
LINES floor, `sacrificing them at the face only once their line reads 0 creatures`).
After: `... WHILE THEIR LINE COUNTS A CREATURE "without a restriction against attacking": ... A line
of nothing but "[defender]" bodies counts 0 there. IT BECOMES YOUR CLOCK once that count is 0, ...`
(floor: `... only once nothing on their line can attack you`).
Paid by `162` **seqs 38 and 42** (t14): their line named Fog Bank and Shield Sphere and the board
line read `2 permanents listed, of which 2 are creatures, 0 of them without a restriction against
attacking`. The seat took the face row twice, correctly — two walls generate no crack-back, so the
Goblin kept home stops nothing — and the rule as written forbade it. The gate now reads off the
count the board line already prints, which is the count the rule's own justification is about.

**Bytes trimmed to pay for the above (-661):** entry 2's narrowing clause (above); the upkeep
section's `A card you cannot cast there says so on its own hand line; believe it.` (the hand line
prints `[cannot pay now: needs N mana ...]` and `[no cast row now: ...]` itself, 0 violations in two
waves); TARGETING step 0's `Kill counts count CREATURES only, so "{kills 0 of the 2 legal targets}"
can still be lethal.` (no instance in two waves); STARSTORM'S X's `Declining one screen later is the
worst answer there is: the announcement has already tapped your sources and every mana it paid is
stranded for the turn.` (the Decline row now prints exactly this — `162` seq 60:
`{declining strands the 9 mana already paid: the card comes back to your hand, your sources STAY
TAPPED ...}`); CYCLING's `SPARE MANA IS NOT A REASON: a cycled Starstorm is a sweep you no longer
have.` (the Starstorm cycling gate two lines above states the condition, and `125` seq 41 cycled it
correctly under the release condition); BLOCKING's `ONE BLOCKER STOPS THE WHOLE OF ONE ATTACKER'S
DAMAGE; a second and third stop nothing more and die with it.` (the prompt prints
`Each blocker stops at most ONE attacker` in the crack-back cover clause and
`each blocker you keep back stops at most ONE attacker` in the attackers seam); and TARGETING's
`A ROW TAGGED [your battlefield], or "Yourself (player, life N)", IS NEVER THE ANSWER; when it is
the only row, "CHOICE: 0 (pass)".` (every such row now prints `{this hits YOUR permanent}` itself —
`152` seq 27 — and there are 0 self-targeting takes in two waves).

### Which wave-75 items recur, and what the wave-75 edits decided
- **Edit A (Starstorm clause (c) precondition)** — **UNTESTED**: no Starstorm cast this wave was
  taken or declined under clause (c)'s board (a body in hand plus a tap-out X). The two Starstorm
  sweeps taken (`123` seq 20, `126` seq 26) both priced `YOURS: none`.
- **Edit B/B2 (the Siege-Gang face row is closed while their line names a creature)** — **PASS on
  the case it was written for, and it won a game**, with one misfire. Ten face sacrifices this wave:
  `125` seqs 53–56 and `126` seqs 38/44/45/48 were all taken on a line reading `0 are creatures`,
  against 3 wave-75 takes on boards with live attackers — the leak the edit was for is closed, and
  the `125` sequence is the win. The misfire is `162` seqs 38/42, where the line named two
  **defenders** and the rule forbade a face row that was right; the model overrode the guide and was
  correct. Two residuals, both edited above: no last-body rule (Edit B) and a gate counting
  creatures instead of attackers (Edit C).
- **Edit C (count the blockers before calling the line fatal)** — **PASS on its own case.**
  `152` seq 28: two Goblins, a stated `for up to 11 ... that would KILL you`, and the seat answered
  `ATTACK: none`. Wave 75's `146` seq 29 sent both and died on the same shape. The plan line still
  writes `survival is impossible` — the sentence PLAN LINES forbids — while the choice is now
  right; the words are not the decision, so I did not spend bytes on it.
- **Edit D (Hammer return off under a lifegain trend)** — **UNTESTED**: no Hammer was drawn in the
  125 game, and no opponent in the seat showed a repeating gain.
- **Wave 75 HIGH-1 (kill rows get no crack-back arithmetic)** — **partly closed**: sweeps now carry
  it (§2 HIGH-1), targeted removal still does not, and that gap is this wave's loss.
- **Wave 75 HIGH-2 (`forced_close_unrecorded`)** — recurs at 1 (was 4), same attribution problem.
- **Wave 75 MED-1 (the dropped `PLAN:` label)** — recurs unchanged, now with a **zero** genuine
  residue.
- **Wave 75 MED-2 (`plan_names_uncastable_zone_card` 12/14 false on Hammer)** — **closed by Q9**;
  the new residual is a different shape (MED-2 above).
- **Wave 75 MED-3 (the upkeep face row has no cost clause)** — no longer leaks in this seat: the
  face rows taken this wave were all on empty lines. The render-side clause remains unbuilt.
- **P19 (hold row size)** — now **PASS at the tail**: max 596 B.

### Rejected, though I looked at them
- **An X-menu rule off `162` seq 60.** `Never a row killing none` and `A marked row ... is the
  answer` are both already in STARSTORM'S X and both were on screen; the failure was a misread of a
  ten-word brace, and a fourth statement would be noise. It is §2 MED-1 as render work instead.
- **A "do not write that the game is lost" restatement off `152` seqs 27/28.** PLAN LINES already
  forbids the sentence, and this wave the sentence accompanied two correct choices. Unpaid.
- **A Talisman second-copy rule off `152` seq 18.** The `[second copy: ... this window's cast spent
  DOUBLING an effect you already have]` gloss argues against a cast that was the only row on the
  menu that advanced anything. Nothing to fix.

---

## (4) Optional proposals
**None.** The two generalisable items are engine work (§2 HIGH-1's conditional crack-back clause for
exhaustive-kill-list removal rows, and §2 MED-1's plan echo plus marked-row example on the X menu),
stated as engine items above. Nothing this seat saw contradicts an existing amendment or needs a new
one.

## What I did NOT check
- **The opponent seats' JSONL.** Board state and their plays were read from my own rendered prompts
  and the shared stderr; I opened no opponent translog.
- **Any code.** Every claim about a counter, a skipped window or an unfired instrument is a
  transcript/stderr observation. I cannot say whether `forced_close_unrecorded` 1 lost a decision,
  which seat the one `unclosed <think>` belongs to, or whether `crossphase_identical_reputs` 0,
  `blocker_forecast_gang` 0 and `force_close_arms_*` 0 are unwired counters or honest zeroes.
- **The corpus-wide Q1 census.** I have three specimens (§2 HIGH-2) and no honest rate: the seam key
  is not in the record and both of my reconstructions of it are unreliable.
- **The `askreplay/` sidecar** and the replay path behind `ask_replays_reserved` 12 /
  `identical_ask_answers_reserved` 8.
- **The 12 `async_drops`** individually; 0 `phase2_answer_missing` and 0 fallbacks are consistent
  with none lost, unconfirmed. The `152` seq-24 gap (§2 LOW-1) is unexplained.
- **Q3, Q6, Q8, Q10–Q14, P14** — no instance exists in this seat; UNTESTED, not PASS.
- **Whether 4/6 is a real strength change.** I priced one loss and three leaks and made no
  matchup- or field-level accounting; the 125 and 162 games that flipped from wave 75 were different
  boards, not the same board played better.
