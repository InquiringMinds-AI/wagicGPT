# Wave 76 — deck 125 (Revelation Fracture, UW draw-go control)

Seat files: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260910-141526-final/*ai_baka_deck125-*`
(all six of this deck's games are RERUN games — none of the two first-run games is mine).
SINGLE binary, so every count below is one population.

**2 W / 4 L** (wave 75: 3/3). 438 seat decisions (ask 311, priority 101, discard 26), reasoning on for 438/438, **0 fallbacks, 0 `reply_truncated`, 0 `answer_replaced`,
0 `action_before_plan`**, 6 `plan_line_missing` and 6 `off_protocol_bytes>0` (1.4% each; wave 75
2.6%), `async_drops` 5, `hold_released_turn` 33, `crossphase_identical_reputs` 8 (NEW),
`forced_close_unrecorded` 0, `phase2_answer_recovered` 3 / missing 0, `main_phase_windows_skipped` 0,
`plan_names_uncastable_zone_card` 0, `plan_names_stranded_card` 0, `repeat_annotated_takes` 0.
reasoning median 5,912 chars / max 22,793; latency p50 72.4 s / max 442.8 s. No hang, crash or timeout.

## 1. Game-by-game

| opp | result | turns | end life | the decision that decided it |
|---|---|---|---|---|
| 123 | **WIN** | 67 | 66–0 | Grind win, no misplay decided it. Two Staffs (`seq 191` t45, `seq 227` t53), Elixir shuffles at `seq 163/223/312`, Emrakul discarded at step 4 (`seq 205`). `seq 217` t51 is an upkeep Revelation (§A-1) that cost nothing in a won game; its X was correctly announced K−1 per Staff (K=12 → 11) |
| 146 | **WIN** | 42 | 36–0 | Textbook. Two Staffs, 5 body pings all on `DIES` + `{removes N from the CRACK-BACK total}` rows, 4 sweepers, 8 counters, 0 self-pings. `seq 187` is the winning ping |
| 126 | LOSS | 26 | 0–46 | **`seq 27`, t15 UPKEEP, 11 life, 4 mana, Supreme Verdict in hand against a 9-creature lifelink board.** Cast Sphinx's Revelation for **X=1** — the row itself printed *"Supreme Verdict {1}{u}{w}{w} in your hand needs 4: no X on this row leaves it payable this turn, not even X=0"*. Two violations in one take: the pre-main rule and the SIZE FLOOR (X=1 above 10 life). Repeated at **`seq 49`**, t25 upkeep. §A-1 |
| 152 | LOSS | 23 | −18–17 | **`seq 51`**, t19, 12 life, opponent's main, hand EMPTY: Revelation from exile for **X=1**, 6 sources spent for 1 life and 1 card. SIZE FLOOR violation (X=1 above 10 life). Life 12 → 2 by t22. The rest of the game was clean (Staff t16, 5 pings, 2 sweepers, 2 Cancels on-list) |
| 162 | LOSS | 20 | 0–21 | Not a counterspell failure — **every** counter window was answered on-list (`seq 23` Underworld Dreams, `seq 37` Fate Unraveler, `seq 98` Master of the Feast) and Revelation stop (1) held at `seq 99/103/115` under 2× Underworld Dreams. The seat drew itself to death behind 2× Howling Mine + Dictate of Kruphix (hand 25 at t20). The one avoidable loss inside it: **`seq 103` + `seq 108`**, t19, 7 life, hand 9 vs max 7, `DISCARD PUNISHERS ... Liliana's Caress` printed and `CLEANUP PRICE OF PASSING ... up to 4 life - at worst you would be at 3` printed on the taken row, Lightmine Field and both Final Judgments castable off 9 sources — cast nothing twice, discarded 2, 7 → 3. §A-2 |
| 130 | LOSS | 32 | −4–19 | **PERCEPTION, not strategy.** Dwarven Blastminer (1/1, `{2}{R},{T}: Destroy target nonbasic land`) ground the seat's battlefield to **0 permanents by t17** and it never recovered. At `seq 7/12/17` the opponent battlefield line reads `Dwarven Blastminer {1}{r} (1/1) [tapped - ...]` and **nothing else** — the ability text is not on that line, and the model's own reasoning models it as a 1-power attacker. The text exists: the Path row at `seq 23` (t19, 11 turns later, first Path drawn) prints it in full inside `{target text:}`. §B-1 |

## 2. Engine / interface / card items

**HIGH — B-1. The battlefield line strips EVERY creature's ability text. 3,892 creature entries
corpus-wide, 0 with a `{effect:}` gloss; 2,734 non-creature entries carry one.**
Repro `…-ai_baka_deck125-…-vs-ai_baka_deck130.jsonl` **seq 7** (t7, Main 1), opponent battlefield line:
`Dwarven Blastminer {1}{r} (1/1) [tapped - cannot attack or block this turn]; Mountain #1-#3 [tapped] x3;
Pyrite Spellbomb {1} [artifact] {effect: "{R}, Sacrifice Pyrite Spellbomb: …"}` — the artifact is
glossed, the creature is not. Same prompt's model reasoning: *"Crack-back next turn: 1 creature attacks
(Dwarven Blastminer untaps), deals 1 damage."* The same engine prints the ability at **seq 23** on the
Path row: `{target text: "{2}{R}, {T}: Destroy target nonbasic land. -- Morph {R} …"}`.
This is not cosmetic: this deck's guide routes four separate decisions off "that body's own text reads
`{T}:` / `Whenever … attacks` / `At the beginning of`" (Path priority 2, the sweeper K=1 carve-out, the
ping's engine-body exception, COUNTER ON SIGHT). At a window where no removal row is printed the model
has no channel for that fact at all. Fix: give a creature battlefield entry the same `{effect:}` gloss
every other permanent type gets (truncated the same way). Pin: seq 7's Blastminer entry carries its
activated ability.

**MED — B-2. 56% of every decode is post-decision format re-verification.** Median share of the
`reasoning` body falling AFTER the model's first `CHOICE:` token = **0.562** (mean 0.567) over 438
records; the reply-protocol string `Write nothing else` is quoted back inside the reasoning a median of
1, mean 1.59, **max 8 times per decision** (`…vs-ai_baka_deck126.jsonl` seq 49 is a clean specimen:
the decision is made, then the protocol block is re-read and re-confirmed nine times before output).
`reasoning_degenerate` fires on **437 of 438** records and `reasoning_ngram_repeat` on 435, which makes
both useless as flags but is not a false positive — the loop is real and it is the protocol block, not
the board, that drives it. At p50 72 s/decision this is the single largest engine-side cost on this
seat. Fix belongs to the prompt's protocol tail (the worked `e.g. "CHOICE: 1 (…)"` example plus
`Write nothing else` is what the model keeps re-auditing), not to the guide.

**MED — B-3. Q13's `[this exact list was put to you N windows ago …]` bracket never prints its
no-change clause: 8 of 8 renders on this seat say `the board HAS moved since then`.** All eight:
`126` seq 6/23, `130` seq 17, `162` seq 14/63/72, `152` seq 16/21. This confirms CQ F4's warning — the
board comparison is strict enough that the intended message ("nothing on the board has changed") is
unreachable, so the bracket as shipped only ever tells the model the opposite of what Q13 wanted it to
say. The seat acted on 3 of the 8 (two Paths), so it is not inert, but the designed half is untested.

**LOW — B-4. Two land drops declined with a land on the list** (`123` seq 182 t43, `146` seq 43 t22),
both a tapped Seachrome Coast at 9 and 13 lands. 75 of 77 land asks played a land. Guide rule is
absolute; the two declines cost nothing and no edit is bought.

**LOW — B-5. A Revelation announced and then declined at the X menu**, `146` seq 178/179 (t40, their
life 2 with two Staffs out = Revelation stop (3)): the stop was answered one window late, stranding 17
mana in a won game. The guide already states this ("A DECLINE HERE IS NOT FREE … answer the stops at
the CAST row"); it was simply not consulted.

**Verified TRUE / FIXED on this seat, so the engine seat does not read silence as absence:**
**Q2 PASS** — this seat owned wave 75's 80,805-byte prompt (`125v123` discard); its largest prompt this
wave is **24,895 B** (`123` seq 236) and the seat max is **30,375 B** (`146` seq 179); 90 of 438 prompts
over 20 KB. All 26 `- THEIRS:` rosters render as count + shapes, none needed `and N more`.
**Q3b PASS** — 134 own-clock tag renders, **0** beside a `that would KILL you` crack-back line and 0
beside a `LOOP COMPLETE` (wave 75: 9 of 191, this seat's B-2).
**Q15 / B-5 (wave 75) FIXED** — the own-clock template is now number-correct: 53 singular renders
(`your Staff of Nin deals 1 damage a turn`) against 81 plural, and every plural one names `#1-#2`.
**Q9 PASS here** — `plan_names_uncastable_zone_card` 0 and `plan_names_stranded_card` 0 on this seat
(wave 75: 5, of which 4 were the cast-from-exile false class; the exemption reaches them — `152` seq 51
is that same exile cast this wave and is uncounted).
**Q10 UNTESTED here** — one ghostform prompt in six games (`146` seq 35, and it is Kaya's `+1` target
text on the stack, not a `{removes:}` row); no removal row on a ghostform body occurred.
Own-turn hold rule: **166 own-turn windows, 26 holds, all 26 carrying the declined-count override that
makes the hold row correct = 0 violations** (wave 75: 1.8%). Declined-count override: 108 windows,
45 hold / 23 acted / **40 (37%) plain "Cast nothing"** — first fall in seven waves (54/45/40/24/26/20).

## 3. Guide verdict: **EDIT** (`wave76/deck125/strategy.txt`, **19,993 bytes** against the live guide's
19,989 — it grows by 4 bytes; the three rules below are paid almost entirely out of the card-text
quotes listed as edit 4, which the rows themselves print as `{card text:}`.)

| # | before → after | paid by |
|---|---|---|
| 1 | **ORDER OF OPERATIONS step 3 names the rows instead of a referent, and names the Revelation.** `AT A PRE-MAIN WINDOW OFFERING NONE OF THOSE THREE, "Cast nothing right now" IS THE ANSWER, and no rule below lifts it.` → `AT AN UPKEEP OR DRAW WINDOW OF YOUR OWN TURN EXACTLY THREE ROWS MAY BE TAKEN: a Staff ping, an Elixir ACTIVATION, and a counterspell aimed at something printed under "ON THE STACK". EVERY OTHER ROW AT THOSE TWO PHASES - A SPHINX'S REVELATION INCLUDED, at any X, any life and any mana - IS "Cast nothing right now". The menu printing a row is not a reason to take it and no rule below lifts this.` | **`126 seq 49`** is the proof that the wave-75 wording was unreachable — the model quoted it verbatim and then discarded it on the referent and on the menu: *"The guide says: 'AT A PRE-MAIN WINDOW OFFERING NONE OF THOSE THREE, "Cast nothing right now" IS THE ANSWER'. But this is a casting decision menu in Upkeep. The menu gives me options. I should pick 1."* "THOSE THREE" pointed at a list of allowed acts in the previous clause while the sentence read as pointing at the four main-phase-only items, and a Revelation is in neither. **`126 seq 27`** is the same window at 11 life and X=1, and **`123 seq 217`** the third. 3 of 10 Revelation casts this wave were at an upkeep (wave 75: 13 of 21 pre-main) |
| 2 | **The SIZE FLOOR becomes printed stop (4) instead of a bullet below the low-life paragraph.** stops list gains `(4) the row's "{X pricing: max affordable X=N ...}" reads 2 or less and your printed life is above 10.`; the old bullet collapses to `THE SIZE FLOOR IS STOP (4), and at 10 life or less it is lifted: cast there at the largest X offered.`; `THE LOW-LIFE FLOOR LIFTS STOPS (2) AND (3)` → `(2), (3) AND (4)` | **`126 seq 27`** (X=1 at 11 life) and **`152 seq 51`** (X=1 at 12 life). Neither reasoning mentions the floor at all: at `126 seq 27` the model walks stops (1), (2), (3) and the low-life floor in order, concludes *"So casting X=1 is correct"*, and stops reading — the floor sat below a nine-line paragraph it never reached. Moving the test into the numbered stop list puts it where the model demonstrably reads. Also at `126 seq 27` the model fabricated a counter-rule to license the cast — *"the guide says 'Your own turn is for ACTING… passing your turn with castable cards and unused mana is a losing habit'"* — a string that appears in NO deck guide, in no prompt and in no system message on this seat |
| 3 | **The hand-over-seven cleanup bill moves out of §CLEANUP DISCARD into ORDER OF OPERATIONS as step 4, with its precedence stated.** `HAND OVER SEVEN WITH A DISCARD PUNISHER PRINTED: cleanup bills 2 life per card above seven - while your hand is above seven on YOUR turn, cast one castable card per card you are over rather than "Cast nothing right now".` (last sentence of the discard section) → a numbered step 4 ending `THIS IS THE ONE RULE THAT REACHES A ROW THIS GUIDE CALLS DEAD - a sweeper at K of 0, a Lightmine Field at N of 0, a Revelation under stop (4) - and it reaches them only while the hand count is still above seven.`, with §SWEEPERS' `NO SURVIVAL CLAUSE IN THIS GUIDE REACHES K OF 0` → `ONE RULE REACHES K OF 0 - the hand-over-seven cleanup bill, step 4 - and nothing else does` and §LIGHTMINE's `N of 0: "Cast nothing right now", always` → `N of 0: "Cast nothing right now", a second copy included; step 4 is the one rule that lifts it.` | **The guide gave one seat both answers in one game and it took each once.** `162 seq 97` (t17, hand 8, Liliana's Caress printed) cast a Lightmine Field whose own row read `they control 0 creatures able to attack - deals 0`, reasoning: *"Opponent has 0 creatures. Does nothing. … Cleanup price: discard 1 card for 2 life"*, PLAN line *"Cast Lightmine Field to reduce hand size and avoid the 2-life cleanup discard"* — the cleanup rule beating the absolute N-of-0 rule. Two turns later at **`162 seq 103` and `seq 108`** (7 life, hand 9, both main phases, 9 sources, Lightmine Field and both Final Judgments castable) the absolutes won instead and the seat paid the 4 life it had just been told about on the row. The rule governs a CASTING window and lived in the DISCARD section; the precedence was nowhere. Also closes the two N-of-0 Lightmine casts corpus-wide on this seat (`123 seq 203`, `162 seq 97` — 2 of 2 Lightmine casts) |
| 4 | Byte payers, no decision rule lost: the verbatim card-text quotes for Staff of Nin, Lightmine Field, Elixir of Immortality, Path to Exile and Sphinx's Revelation (every one of those rows prints `{card text:}` itself — verified on `146` seq 25/122, `162` seq 97, `126` seq 27); stop (1)'s X-times-P arithmetic restatement; the hold row's "costs you your own main phase and their whole turn after it" tail; "Only it stops the next identical ask"; the `Already in pool` gloss's inner parenthetical; three small re-wraps | bytes |

**KEEP, explicitly, with the evidence.** The counterspell table — **26 counters cast across six games,
every one on-list**, and in the game that most punished a miss (`162`, two Underworld Dreams) the seat
countered Underworld Dreams on sight at `seq 23` and Fate Unraveler at `seq 37`. Revelation stop (1) —
held at `162 seq 99/103/108/115/116`, every one under a printed DRAW PUNISHERS paragraph and one of
them with `No listed X leaves you alive` on the badge. The ping rules — **46 offered, 45 taken, 40 at
the face, and all 5 body pings carry `DIES` + `{removes N from the CRACK-BACK total}`**, which is
exactly the printed exception; 0 self-pings. The own-turn hold rule — 0 violations in 166 windows
(above). The never-discard-a-Staff step — **0 Staffs sent in 26 cleanups**, including two ten-card
cleanups (`123` seq 216/222). The second-Staff rule — 2 games reached two Staffs, both won. EMRAKUL —
offered and declined with a Staff out, and discarded at step 4 (`123` seq 205) at 10 lands.

**Disobeyed, not misread, so NOT re-capitalised** (the wave-74 ruling): four sweepers cast at a header
K of 1 (`126 seq 39`, `146 seq 40`, `146 seq 73`, and `126 seq 31` is K=5 and correct) — three of them
in a game already won, none in a losing line, so nothing is bought. The two land-drop declines (B-4).

**Wave-75 items that recur:** §A-1 (the pre-main Revelation) recurs at `126 seq 27/49` and `123 seq 217`
and is edit 1; the SIZE FLOOR violation recurs at `126 seq 27` and `152 seq 51` and is edit 2.
Wave-75 B-1 (ghostform `{removes:}`) is UNTESTED here — the board never recurred. B-2 (own-clock beside
a stated lethal) is FIXED, 0 of 134. B-5 (plural template) is FIXED. B-7 (two `CHOICE:` lines consumed
first-wins) did not recur — 0 multi-CHOICE replies on this seat. B-3 (declined-count override) improved
for the first time in seven waves, 54% → 37%. B-4 (`plan_names_uncastable_zone_card` counting a legal
cast from exile) is FIXED — 0 counts, with the same exile cast present at `152 seq 51`.

## 4. Not checked
No `blockers`, `attackers`, `reveal` or `bottom` seam — this deck fielded no creature in any of the six
games and took none of those asks (26 `discard` asks I did read). Opponent seat logs were opened only
for the results row; I did not reconstruct any opponent's line of play, so "the opponent could not have
won faster" is not a claim I make anywhere above. I verified no card against Scryfall or against
`primitives/*.txt` this wave — Dwarven Blastminer's ability in B-1 is quoted from the engine's own
`{target text:}` render, not from the card script, and I did not check whether the script matches
Oracle. I did not trace the 5 `async_drops` individually, did not open `askreplay/`, did not reconcile
`ask_replays_reserved` 332 / `identical_ask_answers_reserved` 275 on this seat against the code, did not
measure hold-row byte widths (Q15 LOW-2), did not measure `deadline_pct` per record beyond its sum, and
adjudicated no lane-{CN,CO,CP,CQ} prediction beyond the ones my own windows touch (Q2, Q3b, Q9, Q10,
Q13, Q15 above) — the rest is the engine seat's job. No general-guide or skill proposals: all three
edits are to rules that exist only in this deck's guide, and edit 3 is a placement fix, not a new rule.
