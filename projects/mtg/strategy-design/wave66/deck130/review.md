# Wave 66 — deck 130 (mono-red land destruction) per-deck review

Seat files: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-064038/*deck130-*-vs-*.jsonl`
Guide read: `projects/mtg/bin/Res/ai/baka/deck130_strategy.txt` (71,227 B — at the ceiling of the 41-71 KB band).
Record 3/6. 263 answered decisions, **16 `unparsed_reply` -> Baka** (of the corpus's 83), 1 `wall_miss_unrecorded_wall`,
0 `repeat_past_stop`, 0 `named_row_reask`, 0 `reveal_stall_forced`, 3 `async_drops`, `post_answer_overrun` 172/263 (65%).

---

## 1. Game by game

**WIN 146v130 (t14, 7 to 0)** — clean guide line. LD/tempo skipped (opponent's lands never in range), Blastminer as a
blocker (seq 21 `Dwarven Blastminer blocks Silverquill Silencer`), Rorix cast + attacked the same turn at seq 19/20,
Starstorm X=3 at 7 life to clear the crack-back (seq 23-25, chose X=3 over X=4 on the render's `{same kills as X=4,
for 1 less mana}` — the X-pricing annotation earned its place), second Rorix seq 28, Hammer to the face for the last 3
at seq 31/32. Deciding decision: **seq 19** (Rorix at 14 life into a board that had just taken it from 19 to 14) —
guide's "cast him and declare him as an attacker that same turn" executed exactly.

**WIN 123v130 (t14, 20 to -1)** — the textbook curve: Molten Rain on Arcane Sanctum t6 (seq 9/10), Stone Rain on
Isolated Chapel t10 (seq 16/17), Siege-Gang t12 (seq 24), second Siege-Gang + Rorix t14, then `ATTACK:` all five
bodies (seq 30) for exact lethal. Deciding decision: **seq 24** — deploying the Commander the turn after the second
land kill, cast-order entry 3-then-4 as written.

**WIN 162v130 (t14, 3 to -2)** — won on Rorix (seq 25/26 t8) plus land kills, but the closest game of the six: the seat
bled 19 -> 16 inside a single DRAW STEP on turn 10 across **eight** windows (seq 28-35) against 162's Howling
Mine/Underworld Dreams shell, and finished at 3. H5 discard behaved: seq (kind `discard`) took the `{spare: you control
6 lands already}` row and discarded Forgotten Cave — spare-only, 1 of 1.

**LOSS 152v130 (t15, 0 to 18)** — 152 curved out (Katilda, Ranger Class, a 3/3 Wolf) while 130's hand was all
land-destruction. Deciding decision: **seq 20/21 (t10, life 12)**. The menu offered `1. Cast Hammer of Bogardan ...
{kills: Katilda, Dawnhart Prime, Wolf ...}` against a `CRACK-BACK NEXT TURN: ... for up to 4 ... you would be at 8 or
lower`; the seat took Blastminer (seq 20) and then **Molten Rain on their only land** (seq 21), tapping out at 12 life
with two live attackers facing it. Their land count was 1, so the guide's own "FOUR OR FEWER lands" gate said the LD was
live — but so did the crack-back line say a body was about to be needed. This is the one place a guide line is arguably
under-specified (see §3); it is NOT a case of the guide being read wrong.

**LOSS 126v130 (t11, 0 to 35)** — killed from 20 in a single turn by a Sanguine Bond life loop while holding a hand of
two Mountains and two Starstorms. Nothing in mono-red answers an enchantment; the seat's own reasoning at seq 21 named
Sanguine Bond and correctly saw no out. Variance, not guide.

**LOSS 125v130 (t37, 0 to 67)** — the corpus's lopsided finish. 125 assembled Elixir of Immortality + Sphinx's Revelation
+ two Staff of Nin; 130 drew 16 Mountains and no second threat after Final Judgment exiled the Goblins at t15. By seq 90
(t36, life 1 vs 67) the seat held Molten Rain / Hammer / Spellbomb against 13 lands and 0 creatures — genuinely dead.
**But 10 of the game's decisions were `unparsed_reply`** (see HIGH-1), and at least one flipped a decided play: at
**seq 58** the model concluded "I will cast one Talisman of Impulse" and wrote no coded line; Baka answered **seq 60
`Cast Stone Rain`** into a SIX-land opponent — precisely the dead LD spell the guide forbids at N>=5. The game was
already lost, but the mechanism is exactly what would lose a live one.

---

## 2. Engine / interface items

### HIGH-1 — `ANSWER: CHOICE: 5 (...)` is invisible to the parser. AR H2a's first live cost, and it is the corpus's #1 fallback cause.
Repro: `1788694863-...deck130-...-vs-...deck162.jsonl` **seq 10**, reply line
`ANSWER: CHOICE: 5 (Draw 1 with Pyrite Spellbomb #1)` -> `choice -1`, `<refused: unparsed_reply>`.
`findAnswerLabelLine` (`src/AIPlayerGPT.cpp:17513`) and `scanCodedAnswerLines` (:14232) skip only leading
whitespace/`*`/`#`/`-`/`>`/`` ` `` before matching `CHOICE:`, so a line whose first token is `ANSWER:` never matches.
The protocol itself supplies the prefix: `kReplyProtocol` names the section "then your **ANSWER**" and then calls the
line "THE ANSWER LINE", and this model transcribes all three section names as literal labels
(`REASONING:` / `ANSWER: CHOICE: ...` / `PLAN:`).
Scope: **8 of my 16** fallbacks; corpus-wide **45 of 83** — the single largest cause of the wave's new fallback class.
Other deck130 instances: 162 seq 18, 125 seq 13/15/19/31/42, 123 seq 13.
Fix is one of two one-liners: accept an optional `ANSWER:` / `ANSWER LINE:` prefix in the label scan (the retraction and
first-wins machinery are unaffected — the payload after it is already normalised), **or** stop naming that section
"ANSWER" in `kReplyProtocol` and call it by its label ("write the CHOICE: line next"). The parser side is safer: the
protocol wording is what the model is imitating, and any synonym it invents next wave lands in the same hole.

### HIGH-2 — The reordering did not produce reasoning-first replies; it produced **answer-first**, and the pre-thought answer is the one that runs.
**191 of 263** deck130 replies (73%) put the coded answer on **line 1 of the reply, with no reasoning above it**;
only 54 placed it after >80 characters of working. `post_answer_overrun` is 172/263 (65%) — the reasoning simply moved
*below* the answer line rather than above it.
This is not cosmetic. In every one of the 18 `later_answer_ignored` records on this deck the first coded line is at
line offset 0, and in five of them the line the model reached **after** thinking differed from the one that ran:
- `...deck125.jsonl` **seq 60**: line 1 `CHOICE: 3 (Cast Stone Rain)`; line 14 (its conclusion) `CHOICE: 2 (Cast
  Talisman of Impulse)`, recorded as `ignored_line`. Stone Rain was cast into 6 opponent lands.
- `...deck125.jsonl` **seq 64** (`ignored_line: CHOICE: 4 (Hold priority)`) and **seq 66**
  (`ignored_line: CHOICE: 1 (Cast Talisman of Impulse)`, ran `Cast Starstorm` with zero creatures on the board and the
  row itself reading `[<- no X on this menu kills anything of THEIRS]`).
- `...deck162.jsonl` **seq 34** and **seq 37** (`ignored_line: CHOICE: 3 (Cast Rorix Bladewing)`).
First-wins is behaving as designed; what is wrong is that the answer is now written *before* the deliberation, so
first-wins is latching a pre-deliberation guess. `answer_replaced` fired once in 264 records — the retraction header
machinery is not catching these, because the model does not announce a correction it does not know it is making.
The most direct instrument: make the ANSWER LINE's *position* observable, i.e. record the reply-line index of the
executed coded line alongside `post_answer_overrun`, so the next wave can adjudicate H2a on placement rather than on
overrun bytes.

### HIGH-3 — No coded line at all: the reply ends "Therefore, I cast X." and goes straight to PLAN.
Repro: `...deck125.jsonl` **seq 58**, whole reply ends
`Talisman is good ramp. || I will cast one Talisman of Impulse. || PLAN: Cast Talisman of Impulse to ramp. ...` —
no `CHOICE:` anywhere -> Baka cast Stone Rain instead (seq 60). Also `...deck162.jsonl` seq 22 ("Therefore, I cast
Rorix Bladewing." + PLAN), seq 5; `...deck126.jsonl` seq 21; `...deck125.jsonl` seq 17/25/52/56.
**8 of my 16**; corpus-wide 32 of 83. Note this shape is *created* by the same protocol paragraph as HIGH-1: the
model that treats "REASONING / ANSWER / PLAN" as a three-section template sometimes writes the answer as prose inside
the middle section instead of as a label. A `PLAN:` line arriving with zero coded lines is a cheap, unambiguous
salvage trigger — the sentence immediately above `PLAN:` names the row in 6 of these 8.

### MED-4 — `[no cast row now: sorcery speed - only in your own main phase with an empty stack]` states two conditions and never says which one failed.
Repro: `...deck125.jsonl` **seq 52**. The header reads `Casting decision (Main phase 1, YOUR turn)` and the hand line
reads `Talisman of Impulse {2} [artifact] [no cast row now: sorcery speed - only in your own main phase with an empty
stack]`. Both are true (a `Spark Spray's cycling` ability is on the stack, printed four lines above), but the bracket
reads as self-contradictory at the point of use: the model spent **7,212 characters** arguing with it
("Why is it not on the menu? Maybe because it's an artifact and I already played a land? ... It should be castable")
and produced no coded line -> `unparsed_reply`. Repeats at seq 56. Naming the failing half —
`[no cast row now: sorcery speed - an ability is on the stack]` — removes the contradiction at no cost.

### MED-5 — A hold in the DRAW step re-opened eight times in one turn while the seat bled 3 life.
Repro: `...deck162.jsonl` seq 28-35, all `t 10 / Draw`, life 19 -> 16 across the run, three of them answered
`Hold priority` (seq 28, 29, 34) and re-asked anyway. AU R1 forgives six life-projection clauses, but against a
Howling Mine / Underworld Dreams board the moving figure is a **damage** figure, which by design re-opens. Cost here
was latency and 8 windows, not a wrong play — but this is the no-op window run class the brief asks about, and it is
the exact seam where the hold key is worth the least.

### MED-6 — One 900-second wall miss with an empty reply (wave-58/59 `empty_reply` signature, now surfacing as `wall_miss_unrecorded_wall`).
Repro: `...deck146.jsonl` **seq 11**: `transport: curl=28,http=0,empty=1,connect_ms=20000,phase=wall`,
`latency_ms 900026`, `deadline_pct 100.0`, `reply: ''`. Transport-side, not model-side; the recovery at seq 12 re-asked
and the game was won. Worth naming only because `connect_ms=20000` says the connection itself stalled — a shorter
connect timeout would have turned a 900 s stall into a fast retry.

### LOW-7 — `later_answer_ignored` fires on benign duplicates too (e.g. `...deck162.jsonl` seq 55, the same
`CHOICE: 3 (Cast nothing right now)` written twice). Fine as-is; noting so the count is not read as 18 near-misses.

**Card facts checked** against `bin/Res/sets/primitives/mtg.txt`: Molten Rain (:75759), Hammer of Bogardan (:52046,
`autograveyard={2}{R}{R}{R}:moveto(hand) myUpkeepOnly`), Starstorm (:112835, `damage:X all(creature)`), Talisman of
Impulse (:117529, coloured modes deal 1 to controller). All agree with Oracle text and with what the prompt rendered.
No card-script defect found on this deck.

---

## 3. Guide verdict: **KEEP as is**

This is a success verdict, and the evidence is that the guide's instructions were followed and produced the wins,
while the losses trace to variance, matchup, or the parser — never to a guide line being obeyed and going wrong.

- All three wins are the guide's own script executed in order: early land kills while N<=4 (123 seq 9/16, 162 seq 37),
  then the cast-order jump to a body (146 seq 19, 123 seq 24, 162 seq 25), then attack-and-finish. The Hammer-to-face
  close at 146 seq 31/32 is Rule #2's "lethal first" clause firing correctly.
- The three loss mechanisms are each outside the guide's reach: an enchantment-based life loop with a mono-red hand of
  Mountains and Starstorms (126); a two-drop curve-out against a hand of sorceries (152); a lifegain lock plus a wrath
  plus 16 Mountains over 37 turns (125).
- Rule #0 (draw punishers) and the life floors were *cited by name* in the model's own reasoning at 126 seq 19/20,
  125 seq 17/19, 162 seq 10 — the guide is being read and used, not skimmed.
- The one line I considered editing is the LD gate at t10 of 152v130 (seq 20/21): "FOUR OR FEWER lands" said cast
  Molten Rain, the CRACK-BACK line said a body was needed. But the guide **already** carries the counterweight — the
  crack-back carve-out in COMBAT and the `#0` example that spends a window on bodies when the crack-back would kill —
  and one seq of a lost game does not pay for re-cutting a rule that produced three clean wins. Under this wave's
  posture (REJECT is the expected verdict), one ambiguous window is not a purchase.
- The guide is 71,227 B: **at the top of the 41-71 KB band**. Any future EDIT on this deck has to remove before it adds.

No `wave66/deck130/strategy.txt` written. No general-guide or skill proposals: HIGH-1 through HIGH-3 are all engine/
protocol defects, not strategy-writing defects, and nothing this deck showed adds to amendments 1-331.

---

## What I did NOT check
- I did not read the opponent seat files end-to-end; I used them only via `results.tsv` and the rendered game log in my
  own prompts, so opponent-side misplays that made a win look clean are unaudited.
- I did not adjudicate lane predictions (engine seat's job); the H2a/H5/H7 numbers above are deck130-local only.
- I did not verify that a `ANSWER:`-prefix tolerance in the label scan is free of collisions with existing salvage
  paths — I read `findAnswerLabelLine` and `scanCodedAnswerLines` but did not trace every caller.
- I did not check `async_drops` (3 on this deck) for whether a dropped window changed a play.
- No build, no run, no git.
