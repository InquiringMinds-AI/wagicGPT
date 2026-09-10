# Wave 75 — deck 125 (Revelation Fracture, UW draw-go control)

Seat files: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260910-003556-final/*ai_baka_deck125-*`.
SINGLE binary (`wagic-7a04cd6da-w75step1`), so every count below is one population.

**3 W / 3 L** (wave 74: 1/5). 612 seat decisions (127+81+56+23+94+44 asks, 149 priority, 37 discard,
1 bottom), reasoning on for all, **0 fallbacks of any kind, 0 `reply_truncated`, 0 `answer_replaced`,
0 `action_before_plan`**, 16 protocol deviations (2.6%: 14 `unlabelled_plan`, 2 `prose_outside_two_lines`),
`async_drops` 17, `hold_released_turn` 41, `plan_names_stranded_card` 0,
`plan_names_uncastable_zone_card` 5 (4 of them false — §B-4), `forced_close_unrecorded` 0,
`phase2_answer_recovered` 2 / `missing` 0, `main_phase_windows_skipped` 0, `repeat_annotated_takes` 0.
No hang, crash, timeout or rerun.

**The wave-74 edits that mattered both PAID, and both wins are theirs.** Edit 1 (the loop direction +
"the ping target is always theirs") plus the CL P5 render turned last wave's signature loss into a win:
125v126 is the same Sanguine Bond + Exquisite Blood board and the seat took **0 self-ping rows in 22
windows that offered one**, closing 38–0. Edit 2's phase discipline is what the two remaining losses
break, and it breaks because a LATER rule in the same guide tells the model to break it.

## 1. Game-by-game

| opp | result | turns | end life | the decision that decided it |
|---|---|---|---|---|
| 123 | **WIN** | 83 | 97–0 | No misplay decided it. No Staff of Nin was drawn until t67 (hand was counters from t35 to t65); two Staffs cast at `seq 229/230` closed it. The 26 turns at 33 life with an empty clock are a draw, not a decision. Both Emrakul offers (`seq 318`, `seq 329`, two Staffs out) correctly declined |
| 130 | **WIN** | 53 | 40–0 | `seq 138`, t39: the second Staff cast (wave-73 rule, second wave running). Opponent 20→0 on pings alone |
| 126 | **WIN** | 49 | 38–0 | `seq 178` t25 and 21 later windows: `4. Deal 1 damage with Staff of Nin targeting you {right now: takes 1 damage - you would be at 0 - this row feeds their chain}` printed under a live `LOOP COMPLETE`; the seat took the face row every time. Last wave the identical board at 42–7 was thrown away on this row |
| 146 | LOSS | 21 | −9–20 | **`seq 68`, t20 UPKEEP, 3 life, 9 untapped sources, Final Judgment ({4}{W}{W}, would exile all 5 attackers) in hand, `CRACK-BACK NEXT TURN … for up to 13 - you would be at -10; that would KILL you` printed.** The seat cast Sphinx's Revelation for X=6, leaving 1 source; Final Judgment was then unpayable in the main phase it was legal in. §A-1 |
| 152 | LOSS | 19 | 0–20 | **`seq 37`, t18 UPKEEP, 6 life, 6 untapped sources, Staff of Nin ({6}) in hand, crack-back 7 printed.** Revelation for **X=1** (from exile at +{2}) spent all six sources to gain 1 life and draw 1 card; the seat's own PLAN line then read *"In Main Phase 1, cast Staff of Nin"* with 0 mana. `seq 35` is the same window two turns earlier at **11 life** — a straight SIZE FLOOR violation (X=1 above 10 life) that cost the t16 Staff. §A-1 |
| 162 | LOSS | 23 | 0–20 | Not a guide failure. Mulliganed to 6 off a hand holding the deck's only win condition, never drew a Staff, and by t16 faced Ob Nixilis + Dictate of Kruphix + **two** Forced Fruition — every spell drew 14 and cost 14 life. `seq 92` (t19, Supreme Verdict at 18 life, `[DRAW PRICE: … you would be at 4]` printed and read) is a knowing trade in a game already gone. Revelation stop (1) held: the Revelation row was on that same menu and declined |

## 2. Engine / interface / card items

**HIGH — A-1 is a GUIDE item, not an engine one, and it is stated in §3. The engine's surfaces were all
true and complete at both losing windows** (`146 seq 68`, `152 seq 35/37`): the crack-back total, the
"[no cast row now: sorcery speed …]" tag on Staff of Nin and Final Judgment, and the X pricing were all
on the screen. The model quoted the guide back verbatim and obeyed it. Classified STRATEGY.

**MED — B-1. `{removes: A, B, C}` on a Path cast row counts a creature carrying a ghostform counter as
removed, while the same prompt's battlefield line says it is not.** `…vs-ai_baka_deck146.jsonl` **seq 71**,
t20: the row reads `{removes: Goblin #1, Silverquill Silencer, Nadaar, Selfless Paladin, Goblin #2,
Goblin #3}` while the battlefield line reads `Nadaar, Selfless Paladin {2}{w} (3/3) [vigilance] [counters:
1x ghostform (when this creature dies or is exiled its owner returns it to hand and creates a 1/1 white
flying Spirit - killing it is not removal)]`. The seat spent its only removal on Nadaar (the guide's
"venture" rule pointed there and nothing on the row contradicted it); Nadaar returned to their hand, a
Spirit token appeared, and they recast it on t21. **4 false clauses in 13 ghostform windows**, all in
vs146: `seq 65` and `seq 71` on `{removes:}`, `seq 67` and `seq 73` on a sweeper's
`{right now: exiles N of their creatures … THEIRS: …}` list. Fix: drop a ghostform-countered name from
`{removes:}` and from a sweeper's kill/exile list, or mark it `(returns to their hand)`. Filed as an
engine item deliberately: the truth belongs on the row, not in more guide bytes.

**MED — B-2. P10 / CL own-clock gate FAILS: 9 of 191 own-clock tags print beside a stated lethal.**
`…vs-ai_baka_deck123.jsonl` **seq 381** (and 382, 384, 385, 387, 392, 393, 395, 397), t77: the prompt
carries `CRACK-BACK NEXT TURN: 80 of their creatures will be able to attack … for up to 161 - you would
be at -84; that would KILL you` and, six lines down, `4. Cast nothing right now {the clock you already
control: your Staff of Nin #1-#2 deal 2 damage a turn between them - at that rate alone the opponent
reaches 0 in 4 more turns, with no card spent}`. The lane's own predicate was "must never print beside a
stated lethal". No decision cost on this seat (the opponent never attacked), so MED.

**MED — B-3. The declined-count override is now failing 54% of its windows** — 149 windows carried
`[you declined this exact list N times already this turn …]`; **57 took the hold row, 12 acted, 80 (54%)
answered "Cast nothing right now" or "Pass priority"**. Wave 70 ≈20%, 71 26%, 72 24%, 73 40%, 74 45%,
75 54%. Six waves, one direction. **What the CI hold-latch fix DID buy is large and should be recorded:
maximum N per game fell from 7/13/23 (wave 74 CG) to 4/1/3/1/2/4** — the re-put runs are gone, so the
absolute waste is far below wave 74 even though the rate rose. The wave-70..74 recommendation is still
the only untried one: stop printing the plain decline row once the clause fires.

**MED — B-4. `plan_names_uncastable_zone_card` counts a card the seat is legally casting from EXILE.**
`…vs-ai_baka_deck152.jsonl` **seq 35, 36, 37, 38** — 4 of this seat's 5 counts and 4 of the corpus's 68.
The row itself reads `Cast Sphinx's Revelation {2}{u}{u}{w}{x} [from exile] {castable from exile - your
card, exiled by their Elite Spellbinder, which lets you cast it from there; it costs {2} more than
printed, already counted in the cost shown}`, and the plan names that same cast. The instrument's
castable-zone set is missing exile-with-permission; the engine seat should treat the corpus's 68 as an
upper bound with this class subtracted.

**LOW — B-5. The O9 own-clock tag still reads plural for one permanent.** 191 instances; with a single
Staff it prints `your Staff of Nin deal 1 damage a turn between them` (`…vs-ai_baka_deck126.jsonl`
seq 178). Cosmetic, unchanged from wave 74 A-6(a). Arithmetic itself was correct wherever spot-checked,
including the doubling on the second Staff.

**LOW — B-6. The hold row is fixed but 727 B at its widest.** 452 hold rows on this seat, **median 596 B**
(wave 74: 1,074–1,287 B), max 727 B; `[HOW A HOLD ENDS:` appears **exactly once in every prompt that
renders one, 0 prompts with more**. P19 PASS on the once-per-prompt half; the < 650 B target is met at
the median and missed at the tail.

**LOW — B-7. A reply carrying TWO `CHOICE:` lines was consumed silently.**
`…vs-ai_baka_deck126.jsonl` **seq 345**: `CHOICE: 1 (Deal 1 damage with Staff of Nin #1 targeting the
opponent)` followed by `CHOICE: 3 (…#2…)`. Flagged `prose_outside_two_lines`, first line taken, second
dropped with no re-ask. Correct outcome here (the second Staff was offered again next window), but the
class is a silently discarded answer.

**Verified TRUE / PASS on this seat, so the engine seat does not read silence as absence:**
P5 (22 `you would be at 0 - this row feeds their chain` rows, every one under a live `LOOP COMPLETE`
with both halves on THEIR battlefield — the clause is true at every instance, and the seat's self-ping
takes went 1 → **0**); **P6 FIXED** — `CRACK-BACK NEXT TURN` now renders in own-turn Upkeep (37 windows),
Draw (2), End (3) and Cleanup (5) against wave 74's 0 of 173 upkeep / 0 of 38 draw;
**P15 FIXED** — the range paragraph now decodes both directions (`"X = 12 down to X = 2" … largest X
first` **and** `"X = 2 up to X = 12" … smallest X first`, with "Read which of the two the row says"),
so the 10 of 21 X menus still containing the string "largest X first" are the correct descending half of
a both-ways paragraph, not the wave-74 defect; `main_phase_windows_skipped` 0 (P13 untestable here);
P22 — 0 `Parser returned NULL` of any kind in this seat's four stderr files.

## 3. Guide verdict: **EDIT** (`wave75/deck125/strategy.txt`, **19,989 bytes** against the live guide's
19,991 — it SHRINKS by 2 bytes, so no rule has to earn new bytes; the two new clauses are paid out of the
rationale trims listed as edit 3.)

| # | before → after | paid by |
|---|---|---|
| 1 | **THE LOW-LIFE FLOOR gets a phase and a survival test.** `Then the row is your answer at ANY "max affordable X" of 1 or more - ahead of the tap-out gate, the size floor, "Cast nothing right now" and the hold row - in the FIRST window printing it. Same override when the printed power of their creatures able to attack totals at or above your life.` → `Then the row is your answer at ANY "max affordable X" of 1 or more - ahead of the tap-out gate and the size floor - IN A MAIN PHASE OF YOUR OWN TURN, NEVER AN UPKEEP OR DRAW STEP, and only while your life PLUS that X is ABOVE the "for up to N" total on the printed "CRACK-BACK NEXT TURN" or "INCOMING THIS COMBAT" line. At or below that total the cast buys nothing: answer "Cast nothing right now" and spend the mana in your main phase on the sweeper, Path, Lightmine Field or first Staff step 3 names. With neither line printed the floor needs no test. Same override, same test, when the printed power of their creatures able to attack totals at or above your life.` | **`152 seq 37` and `146 seq 68`.** The model's own reasoning at `152 seq 37` is the proof: *"Could I hold and cast Staff in Main Phase? The guide says low-life floor overrides tap-out gate and hold row. So I must cast Revelation. **Even if I die next turn, the guide's instruction is explicit. I will follow it.**"* — 6 mana for 1 life at 6 life against a printed 7. At `146 seq 68` the same clause bought 6 life against a printed 13 and stranded a Final Judgment that answered all of it. "in the FIRST window printing it" was reading as "in this upkeep", and the floor named no test for whether the life it buys clears the lethal it is bought against |
| 2 | **ORDER OF OPERATIONS step 3 gets its missing answer row.** `… and mana spent earlier is mana that phase will not have.` → `… and mana spent earlier is mana that phase will not have. AT A PRE-MAIN WINDOW OFFERING NONE OF THOSE THREE, "Cast nothing right now" IS THE ANSWER, and no rule below lifts it.` | **`152 seq 35`.** Wave 74's edit 2 said what upkeep is FOR but never named the row to take, and the model demoted it on exactly that gap: *"the guide says 'nothing else' for upkeep, but it's a general principle. Casting an instant in upkeep is fine, especially if it's the only play… **I have to pick a row.** Picking 1 is correct."* It then spent six sources at 11 life on X=1, a SIZE FLOOR violation it never mentioned. 13 of this seat's 21 Revelation casts were still taken pre-main (wave 74: 15 of 23), so the rule is being read as advisory |
| 3 | Byte payers, no decision rule lost: Emrakul's "a cheap instant that makes you sacrifice a creature hands its controller 15 life…" tail and its "and the hold row" (which contradicted §YOUR WINDOWS); Emrakul's blank-row gloss; the converter's "you would be at N" restatement (the engine now prints `this row feeds their chain` itself) and "safe from a converter alone" → "safe from it"; the [defender] "it takes no point off you" tail; Idyllic Tutor's gloss; SWEEPERS' "and the sweeper row repeats K"; LIGHTMINE's "and N is also the damage each takes"; LAND DROPS' tapped-land parenthetical; CLEANUP's "untagged means the engine could not price the card" clause and "and you cannot decline it"; THE DECK's "You answer every threat and" | bytes |

**KEEP, explicitly, with the evidence.** The loop/converter section and "THE TARGET IS ALWAYS THEIRS" —
**0 self-pings in 22 offered rows** on the board that lost wave 74's biggest game, and vs126 is a win.
The own-turn hold rule — **385 declinable own-turn windows, 51 holds, 7 without the override note = 1.8%**
(wave 74: 3.6%; 73: 8%; 72: 66%): best rule in the guide, still improving. LAND DROPS — **87 asks, a land
played in 87**, zero declines (wave 74 had one). The Staff ping rule — **57 of 60 offered windows taken,
55 at the face**. The second-Staff rule and the never-discard-a-Staff cleanup step — **0 Staffs sent in 37
cleanups**, and `130 seq 138` is a win. EMRAKUL — 2 offers, 2 declines, both with Staffs out. Revelation
stop (1) — held at `162 seq 92` under three draw punishers, the exact window that lost wave 74's vs162.
The counterspell table — every counter this corpus was on-list.

**Disobeyed, not misread, so NOT re-capitalised** (the wave-74 ruling on the SIZE FLOOR): CLEANUP at
`146 seq 73` sent **both Lightmine Fields** (step 1's condition was false — the header read K=5, and both
rows carried `{right now: … SURVIVES: none - this kills every one of them}`, which the tag-sort rule
sends LAST) while **skipping step 4's Emrakul at 10 lands**. Two steps taken against their own clauses;
the game was already lost on t21 before the kept cards could be cast, so no edit is paid for.
Revelation stop (3) was overridden three times in a won game (`130 seq 170, 173, 185`, opponent at 5/5/3
with Staffs out). `146 seq 74` chose `lose 2 life` over `discard a card` at 8 life on an Annihilation
Rooms modal ask — no guide rule covers a modal punisher menu, and the seat was dead to a printed 15
either way.

## 4. Not checked
Opponent seats were read only for the vs146 t21 log and the vs126 loop board. I audited no `blockers`,
`attackers` or `reveal` seam (this deck fielded no creature in any of the six games; the seat took 1
`bottom` and 37 `discard` asks, which I did read). I did not verify any card against Scryfall or the
primitives this wave — the vs126 chain and the ghostform behaviour in B-1 were checked against the
prompt's own rendered card text only. I did not trace the 17 `async_drops` individually or confirm
whether any dropped answer fell to the heuristic; did not open the `askreplay/` sidecar; did not explain
`ask_replays_reserved` 603 / `identical_ask_answers_reserved` 484 on this seat against the code (the
engine seat's item (l)); did not measure `chain_selfharm_rows` (953) against the acting-window share; did
not check `reasoning_ngram_repeat` or `deadline_pct`; and adjudicated no lane-{CI,CJ,CK,CL,CM} prediction
beyond the ones that touch my own windows (P5, P6, P10, P13, P15, P17, P19, P22 above) — the rest is the
engine seat's job. No general-guide or skill proposals: edit 1 is a rewrite of a rule that exists only in
this deck's guide, and edit 2 closes a gap in a wave-74 edit to the same section.
