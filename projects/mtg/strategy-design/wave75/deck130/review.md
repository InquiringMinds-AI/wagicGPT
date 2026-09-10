# Wave 75 — deck 130 (mono-red land destruction) — per-deck review

Corpus `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260910-003556-final`, single binary
`archives/wagic-7a04cd6da-w75step1`, `--thinking on`. Live guide = the wave-74 edit (19,975 B).

**Seat census.** 385 model decisions over 6 games (wave 74: 324), **100% with native reasoning**
(median `reasoning_chars` 5,995, max 19,038), **0 fallbacks of any kind**, 0 `reply_truncated`,
0 `action_before_plan`, 385/385 `transport: curl=0,http=200,empty=0` (**no HTTP 400 anywhere —
P2 PASS in this seat**), `deadline_pct` max 43.2, latency p50 93.5 s / p95 204.9 s. The two
`parse_note` records are `repeat_annotated_take(n=1)` (126 seq 48, 146 seq 28), not failures.
Counters: `ask_replays_reserved` 63, `identical_ask_answers_reserved` 47, `hold_windows_skipped`
91 (cast 31 / priority 60), `hold_released_turn` 7, `main_phase_windows_skipped` **0 in all six**,
`own_turn_windows_skipped` 34, `plan_line_missing` 16, `off_protocol_bytes>0` 15,
`protocol_deviation_replies` 16, `plan_names_uncastable_zone_card` 14, `async_drops` 12,
`phase2_answer_recovered` 0 / `missing` 0, **`forced_close_unrecorded` 4** (125: 2, 162: 2),
`plan_names_stranded_card` 0, `blocker_forecast_gang` 0 / multi 14 / collapsed 10 / rows 18,
`repeat_annotated_takes` 2, `reasoning_ngram_repeat` median 0.134 / max 0.498.
Loop family dead: worst (turn, phase) in the seat is **9 asks** (125 t26 Main 1, a genuine chain —
two cyclings resolved between windows and put new rows); no phase anywhere reaches 10.

---

## (1) Game-by-game, and the deciding decisions

| Opponent | Result | Turn | Final life | Decisions |
|---|---|---|---|---|
| 126 | **WIN** | 20 | 20 / -2 | 52 |
| 123 | **WIN** | 26 | 26 / -2 | 74 |
| 125 | LOSS | 53 | 0 / **40** | 167 |
| 146 | LOSS | 13 | 0 / 11 | 27 |
| 152 | LOSS | 13 | -2 / 13 | 28 |
| 162 | LOSS | 12 | 0 / 14 | 37 |

**2/6 (wave 74: 1/6).** Both wins were closed by the guide's own lategame line — Siege-Gang
sacrifices and Hammer returns pointed at a face on a board with no creature pressure — and both
were clean. Three of the four losses turn on decisions the transcripts show going wrong, and
**all three are the same defect: the seat spent creatures it needed as blockers, or declined a
sweep, because the guide's clock rules are written without a board gate.** vs125 is matchup.

### vs 152 — LOSS. **The paid decision: a two-for-one sweep declined by a guide rule that fired on the wrong board**
`1789018590-...-vs-ai_baka_deck152.jsonl` **seq 23** (t10, Main 1, my life 10, opp 19, 5 mana,
hand = Talisman / Starstorm / Siege-Gang / Hammer). Their line: Elite Spellbinder (5/3 flier,
2 counters) and Luminarch Aspirant (1/1, pumps every combat). Row 2 verbatim tail:

> `[<- best X for this cast: X=3 - best trade: the most of THEIRS at the smallest cost to YOURS - THEIRS 2 / YOURS 1] ... [<- board sweep: THEIRS 2 / YOURS 1 - the only row on this menu that prices a board sweep]`

X=3 for 5 mana **empties their board** and costs one Blastminer. The seat cast Siege-Gang instead
(row 3), blocked with it at seq 25, was at 4 life by t12 and dead at t13.

Two causes, one guide and one render, and both are quotable.

*Guide (STRATEGY).* `STARSTORM'S X` clause **(c)** reads `paying it taps you out while a creature
sits in your hand - count the land drop, cast the body instead, and sweep on a later turn: one
blocker beats a sweep that kills one creature.` X=3 taps out and Siege-Gang was in hand, so (c)
fired — on a sweep that killed **two** and cleared their line, which is not the case (c)'s own
justification describes. **Edit A.**

*Render (PERCEPTION, §2 HIGH-1).* The model's reasoning rejects the sweep on a false ground:
> `What about Starstorm? X=3 kills Elite Spellbinder and Luminarch Aspirant, but kills Blastminer. Leaves 0 mana. No bodies to block next turn. Bad.`

There is nothing left to block once both their creatures are dead. It held the `CRACK-BACK NEXT
TURN` figure (6) fixed while evaluating the row that removes its source — and the prompt let it:
row 3 (Siege-Gang) carries a full `{crack-back cover: ...}` paragraph, rows 2 and 4 (the sweep and
the Hammer, which kill the very creatures in that total) carry none.

Also **seq 28** (t12 Upkeep, my life 4, opp 18, lethal crack-back of 6): the seat sacrificed one
of its two blockers for 2 to the face, then at seq 33/34 cast Hammer at the face for 3 at opp 16.
Its own reasoning quotes the upkeep whitelist, agrees the whitelist condition is unmet, and takes
the row anyway — `Elite Spellbinder doesn't die to 2 damage. So that condition isn't met. But
hitting face is standard.` Nothing in the guide closes the face row on a board that is killing
you. **Edits B and C.**

### vs 146 — LOSS on turn 13. **Both blockers thrown at two 4/4s under a stated lethal**
`1789018580-...-vs-ai_baka_deck146.jsonl`.
- **seq 23** (t12 Upkeep, my life 8, opp 18, crack-back 8 = lethal): sacrificed a Goblin at the
  face. The reasoning quotes the guide's Siege-Gang paragraph and stops mid-quote —
  `"...IT BECOMES YOUR CLOCK when their line reads 0 creatures..." So sacrificing in upkeep is
  correct.` Their line had **two** creatures. The same paragraph's `at any other N the Goblin is a
  body` was in the quoted text and did not fire: the paragraph names the clock use and the removal
  use side by side, and the model took the permissive half. **Edit B.**
- **seq 29** (t12 Attackers, my life 8, opp 11). Both rows priced
  `[their untapped blockers: Nadaar, Selfless Paladin (4/4) (your attacker dies, their blocker
  lives); Barrowin of Clan Undurr (4/4) (your attacker dies, their blocker lives)]`, the header
  read `CRACK-BACK NEXT TURN: 2 of their creatures ... for up to 8 - you would be at 0; that would
  KILL you`, and the prompt itself said `each blocker you keep back stops at most ONE attacker`.
  Reply: `ATTACK: A1, A2` — **no PLAN line at all** (the seat's only genuinely missing plan). Two
  blockers held home cover two attackers and the seat is alive at 8; it sent both and died. The
  reasoning is explicit that it never did the count: `Is there any way to survive next turn? No.
  ... I am dead. I will attack with both.` ATTACK FLOOR bullet 3 and the CRACK-BACK EXCEPTION both
  already forbid this row, so this is a rule failing — but the failure mode is an arithmetic the
  guide never states. **Edit C.**

### vs 162 — LOSS on turn 12. Matchup, with one guide-forbidden row taken
Howling Mine + Underworld Dreams + Master of the Feast (5/5 flier). Two compulsory draws a turn at
1 life each, 5 in the air a turn, and a deck with no answer to a 5-toughness body and none at all
to an enchantment. Life 20 → 18 → 16 → 10 → 8 → 1. The DRAW PUNISHERS section did its job (no
optional draw was ever taken once the punisher was out). The one violation is **seq 44** (t10
Main 1, opp 17): every creature row read `- SURVIVES (toughness 5)` and it took the face row for 2
— TARGETING step 3 puts the face on the menu only at `Opponent life:` 6 or less, and
`NEVER SPEND DAMAGE ON A SURVIVOR` names holding it. Same family as vs146/vs152, but it changed
nothing here: nothing else on that 1-mana menu was worth taking. **Not separately paid.**
The turn-10 Stone Rain (**seq 41**) was also cast with THE LAND PLAN dead by its own numbers
(`L is 8 or less`), and again the prescribed answer changes nothing — the 3 leftover mana had no
other use. Not paid.

### vs 125 — LOSS on turn 53, opponent at 40. Matchup, plus the Hammer floor read past for a second wave
Three Path to Exile, two Supreme Verdict, two Final Judgment, Sphinx's Revelation x4, Elixir of
Immortality, two Staff of Nin, Emrakul. **Every creature the seat resolved was answered at instant
speed before a combat step existed** — which is why this game has 0 `attackers` and 0 `blockers`
records (t42 Rorix and t44 Siege-Gang were both Path'd; the Goblins were Path'd individually).
That is a real board, not a missing window.
The guide item is the **Hammer loop from t28 to t52**: nine upkeep returns and recasts for 3 each
against an opponent trend of +7 / +10 / +9 / +5 a turn (seqs 109, 119, 130, 136, 142, 150, 155,
161, 167, 181, 188). The guide's PLAN LINES floor contains the exception verbatim
(`when "Opponent life trend" shows them gaining 3 or more a turn that return is not a clock`) and
it never fired — **second wave running**, same matchup. The exception is buried in a floor whose
headline sentence is permissive and lives in a section the model reads at a plan seam, not in the
HAMMER card fact it reads at the upkeep priority menu. **Edit D** moves it. This did not lose the
game (nothing else was castable on most of those turns) but it is a rule that has now failed twice
in the only place it can be tested.
One correct call worth recording: at **t46** a Hammer was COUNTERED by Dream Fracture and the seat
**immediately cast the second Hammer in the same phase** — wave-74 Edit E (THE BAIT EXPIRES) doing
exactly what it was written for, and P8's post-counter re-ask working here.

### vs 126 and vs 123 — WINS, no misplay worth an edit
126: land destruction on Bayou/Savannah/Scrubland through t12, Siege-Gang at t10, then the face
plan from t14 once their line was empty — opponent to -2 on t20 with the seat still at 20. 123:
Rorix at t16 into an empty board, then Hammer returns; the seat never dropped below 20 (25 after a
lifelink block) and won on t26. In both, the face-damage line the losses punish was the **right**
line, because their board had nothing that could punish an unblocked turn. Any edit here has to
gate on the board, not ban the row — which is what Edits B and C do.

---

## (2) Engine / interface / card items

### HIGH-1 — a row that KILLS the attackers gets no crack-back arithmetic, while a row that adds blockers gets a paragraph
Repro `...-vs-ai_baka_deck152.jsonl` **seq 23**, one menu, three rows:
- row 3 (Siege-Gang) carries
  `{crack-back cover: the CRACK-BACK NEXT TURN line above is 6 from 2 of their creatures ... This adds 4 bodies ... Counting only the checked bodies you cover 1 of 6, leaving 5 -> you would be at 5.}`
- row 2 (Starstorm, `[<- board sweep: THEIRS 2 / YOURS 1]`, X=3 kills **both** creatures in that
  6) carries nothing about the crack-back.
- row 4 (Hammer, `{kills whichever you target: THEIRS - Elite Spellbinder, Luminarch Aspirant}`)
  likewise carries nothing.

The asymmetry is decisive and the model's reasoning shows it being decided by it. Fix shape,
mirroring the existing cover clause: any row whose printed kill list names creatures counted in
the `CRACK-BACK NEXT TURN` total prints
`{crack-back after this row: removes N of the M stated - the line becomes K - you would be at L}`
(and, when the kill list covers every creature in the total, says the line goes to 0). This is the
same arithmetic the blockers seam already prints as `best case with every blocker assigned`; the
own-turn seam prints it for bodies added and not for attackers removed.

### HIGH-2 — `forced_close_unrecorded` 4 of the corpus's 9 are in this seat, and CJ predicted 0
`125v130` gameend `forced_close_unrecorded: 2`, `162v130` `2`; both games also read
`phase2_answer_recovered: 0` and `phase2_answer_missing: 0`. stderr carries
`AIPlayerGPT: unclosed <think> (budget/truncation); forcing the answer` **3 times** in
`game-125v130-1789018571.stderr` (lines 1215, 1364, 1766) and **4 times** in
`game-162v130-1789018595.stderr` (711, 859, 1025, 1058) — 7 closes against 4 unrecorded, and the
stderr is shared by both seats in each game, so I cannot attribute an individual close to a seat
from the transcript. I could not identify a decision that was lost (no turn/phase gap I can prove,
and 0 fallbacks), but the counter says four closes produced no record. Engine seat: reconcile per
arm; the arm identity is what CM F5 moved and it is the thing my data cannot supply.

### MED-1 — `plan_line_missing` is a DROPPED `PLAN:` LABEL, 15 of 16, fifth wave unchanged
16 `plan_line_missing`, 15 with `off_protocol_bytes>0`, same records, each equal to the length of
the dropped sentence. Verbatim repros: `146` seq 25
`"\n\nPlay Mountain and cast Hammer of Bogardan in main phase.\nCHOICE: 1 (Play Mountain)"` (56 B);
`123` seq 77 `"\n\nCAST HAMMER OF BOGARDAN AT OPPONENT.\nCHOICE: 2 (Cast Hammer of Bogardan)"` (36 B);
and one that costs a single token to rescue — `125` seq 166
`"\n\nTHE PLAN: Deal 3 damage to opponent with Hammer of Bogardan.\nCHOICE: 1 (The opponent (player, life 17))"`,
which writes the label with one extra word in front of it. Accepting an unlabelled or
`THE PLAN:`-prefixed first line as the plan zeroes 15 of 16 here and, on the corpus ratio, most of
the 83. **The residue is exactly one record and it is real**: `146` seq 29, reply
`"\n\nATTACK: A1, A2"`, `off_protocol_bytes: 0` — a genuine missing plan, and it is also the seat's
worst decision of the wave (§1). Reporting the two as one class is right (CL P23d confirmed:
`protocol_deviation_replies` 16 = `plan_line_missing` 16 in this seat), but the one-record residue
is where the interesting failure is.

### MED-2 — `plan_names_uncastable_zone_card` is 12/14 FALSE for this deck: Hammer of Bogardan
CL P17 called it an upper bound; for deck 130 it is mostly noise, and for a structural reason.
Twelve of my fourteen fires name **Hammer of Bogardan** while it sits in the graveyard and the
plan is to use its own printed ability to bring it back:
`125` seq 109 `"PLAN: Return Hammer of Bogardan to hand to cast for 3 damage in main phase."`,
`126` seq 53 `"PLAN: Return Hammer of Bogardan to hand to cast it for lethal damage in Main Phase 1."`,
and ten more (125 seqs 77/110/130/142/155/161/167/188/192, 126 seq 49). These are correct plans
and the row they name is on that very menu. The predicate needs "and no zone-change ability of
that card is offered on this menu" before it counts. The two plausible fires are `123` seq 40
(Rorix) and `125` seq 152 (Siege-Gang).

### MED-3 — the upkeep priority menu's face row has no refusal, and it is where three losses leaked
Not a defect in any single clause — a gap. `146` seq 23, `152` seq 28 and `162` seq 44 are all
priority menus where the row already names its target, so the target menu (and with it every
`TARGETING DAMAGE` rule the guide has) is never asked. The rows print
`{right now: takes 2 damage - they would be at 16}` with no relation to anything else on the
board. `146` seq 69 last wave printed
`{removes 3 from the CRACK-BACK total above: 3 -> 0 - you would be at 1}` on a creature row — that
clause is exactly the missing counterpart for a FACE row: a face row on a board with a stated
lethal crack-back could print `{this changes the CRACK-BACK NEXT TURN line by 0, and spends a
blocker that removes N of it}`. Guide-side I have closed it with Edits B/C; the render clause
would close it for every deck.

### Verifying wave-74's deck-130 items against §P
- **P7** (`{crack-back cover:}` counting a legend-rule duplicate as a body) — **UNTESTED here, and
  the gloss class is visibly repaired.** `[legendary: you already control ...]` renders **0 times**
  in my seat this wave, so the second-Rorix repro did not recur. The one `{crack-back cover:}` this
  seat saw (`152` seq 23) now prints a CHECKED/uncounted split, names which bodies it could not
  ask (`3 are tokens this row has no instance to ask`), and states
  `THIS IS NOT A SURVIVAL VERDICT` — the wave-74 complaint (a confident false SURVIVE) is
  structurally gone even though the legend-rule arm went unexercised.
- **P8** (post-counter Casting re-ask) — CI's REFUTED verdict is **consistent with my data**:
  `125v130` t46 shows a Hammer countered by Dream Fracture and a second Hammer cast in the same
  phase, and `main_phase_windows_skipped` reads **0 in all six games** (wave 74: 0 with the window
  genuinely missing; this wave the window is there). I searched all six stderr files for a trace
  line naming the cast seam's silent exit and found none — the only cast-seam traces are the hold
  lines (`cast ask NOT issued - the oracle finds no legal cast in this instant-speed window ...`
  is the nearest, and it is an oracle exit, not the P8 exit). Either the new trace did not land in
  these games or it is not stderr-visible; I cannot confirm it fires.
- **P21** (the 64-char truncation) — **PASS**: 0 `reply_truncated` in the seat, 0 in the corpus,
  and 385/385 records `http=200`.
- **P6** (`CRACK-BACK NEXT TURN` phase-gated to main phases) — **PASS**. Own-turn windows now carry
  it at Upkeep (20) and Draw (9); wave 74 read 0 of 173 and 0 of 38. I checked the converse: of the
  292 own-turn windows WITHOUT the line, **zero** have an opposing creature `without a restriction
  against attacking` — the line is absent only where there is no crack-back.
- **P19** (hold row size) — hold row median **597 B** across 257 renders, max 728 B (`162` seq 50);
  `[HOW A HOLD ENDS:` appears exactly **once** in every prompt that renders one (max 1, 0 prompts
  with more). Wave 74's 1,074–1,287 B is gone. CI's "< 650 B at every window" is missed at the
  tail, not at the median.
- **P1b** (hold-check bracket keys on the ACTION identity) — the churn is gone: 37 `took the hold
  row` across six games with ~11 re-opens, against wave 74's 104 takes / 93 re-opens. 57 `hold
  check` brackets claim `N rows above are new`; the two large ones (`23 rows`, `20 rows`) are on
  menus that genuinely grew by that many rows. I found no bracket claiming new rows on an
  unchanged menu, and no window where a genuinely new legal row was covered by a hold.
- **P1a** (the prompt-only decline note) — max per seat is **5** (`125`), 2 (`162`), 0 elsewhere;
  wave 74's post-CG max was 1. It is outside every key now, so this is a note count, not a
  re-open — engine seat's call whether 5 is worth anything.
- **P15** (largest-X-first boilerplate) — **PASS**: the 36 renders carrying it now decode both
  directions (`A row reading "X = 12 down to X = 2" ... largest X first ... A row reading
  "X = 2 up to X = 12" is the same run the other way ... Read which of the two the row says`).
- **P22** — **PASS**: `/usr/bin/grep -c "Parser returned NULL"` returns **0** in all six of my
  stderr files, and 0 `ABILITYFACTORY Error parsing`.
- **P18** — **UNTESTED in this seat**: 0 prompts contain `TWO FACES, ONE CARD` or `OTHER FACE of`
  (mono-red, no MDFC in this deck; the pathways are all on the opponent's side).
- **P13** — `main_phase_windows_skipped` is **0 in all six games**, so no false count is possible
  here.
- **P5 / P10 / P11 / P16** — no instance in this seat: 0 `this row feeds their chain`, 0
  `reaches 0 in`, 0 ambiguous short names. `another copy of` (P11) renders **37 times** and every
  one I read names a genuine second copy in hand (`another copy of Starstorm {r}{r}{x} in your
  hand needs 2`), never the row's own card — **PASS**.
- **{X}-announcement payment loss** — `/usr/bin/grep -c 'dropped after its X was announced'` over
  my six stderr files returns **0**.

### Card facts — verified, no item
Starstorm, Siege-Gang Commander, Rorix Bladewing, Pyrite Spellbomb, Hammer of Bogardan, Stone
Rain, Molten Rain, Lay Waste, Spark Spray, Forgotten Cave, Talisman of Impulse, Dwarven Blastminer
all rendered consistently with their real Oracle text, including Master of the Feast's toughness 5
against the 2-damage rows, Underworld Dreams written from the opponent's seat with the pronoun
gloss, and Elite Spellbinder's `(5/3) (printed 3/1) [counters: 2x +1/+1]`. No Scryfall
disagreement worth reporting.

---

## (3) Guide verdict: **EDIT** — revised guide at `wave75/deck130/strategy.txt`

**19,995 bytes vs the live 19,975 — net +20**, five additions (+740) paid for by seven trims
(−720). **The bytes are earned by Edits A, B and C**, which are one defect stated three times: the
guide's clock and sweep rules are written without the board gate that decides whether the row is a
clock at all. Each edit is arithmetic already printed on the window, not caution.

**Edit A — `STARSTORM'S X` clause (c) gets its precondition.**
Before: `(c) paying it taps you out while a creature sits in your hand - count the land drop, cast
the body instead, and sweep on a later turn: one blocker beats a sweep that kills one creature.`
After: `(c) paying it taps you out while a creature sits in your hand AND the THEIRS list at that X
leaves a creature alive on their line - count the land drop, cast the body instead. A sweep whose
THEIRS list names EVERY creature on their line is a different row: it empties the board those
blockers were for, so cast THAT and let the body wait.`
Paid by `152` **seq 23**. (c)'s own justification is "one blocker beats a sweep that kills one
creature"; it fired on a sweep marked `THEIRS 2 / YOURS 1` that emptied their line, and the seat
was dead three turns later. Stated as a precondition, not a concession (A334).

**Edit B — the Siege-Gang sacrifice is a refusal first and a clock second.**
Before: `"{1}{R}, Sacrifice a Goblin: 2 damage" is REMOVAL while their board has creatures: take it
when the row reads "- DIES" or its "they would be at N" is 0 or less; at any other N the Goblin is
a body. / IT BECOMES YOUR CLOCK when their line reads 0 creatures and "Mana available:" is 8 or
more - each {1}{R} is 2 to the face, ...`
After: `"{1}{R}, Sacrifice a Goblin: 2 damage" IS REMOVAL, NEVER A CLOCK, WHILE THEIR LINE NAMES
ANY CREATURE: take it there only when the row reads "- DIES" or its "they would be at N" is 0 or
less. On that board the FACE ROW IS CLOSED - a Goblin kept home stops one whole attacker off the
CRACK-BACK line, and spending it for 2 buys nothing. / IT BECOMES YOUR CLOCK once their line reads
0 creatures, or the sacrifices your "Mana available:" pays for this turn total "Opponent life:" -
each {1}{R} is 2 to the face, ...`
Paid by `146` **seq 23** (the reasoning quotes the old paragraph and stops at the clock half) and
`152` **seq 28**. The permissive clause is no longer available to quote on a board with creatures,
and the lethal case that the "8 or more mana" gate used to approximate is now stated as the
arithmetic it is.

**Edit B2 — the PLAN LINES floor no longer licenses the same row.**
Before: `the FLOOR is the plan: cast Siege-Gang and sacrifice Goblins at their face while the mana
lasts; with no Commander in reach the floor is returning Hammer and pointing 3 at their face, and
when "Opponent life trend" shows them gaining 3 or more a turn that return is not a clock - spend
the turn deploying instead.`
After: `the FLOOR is the plan: cast Siege-Gang and keep its Goblins as blockers, sacrificing them
at the face only once their line reads 0 creatures; with no Commander in reach the floor is
returning Hammer and pointing 3 at their face.`
Paid by `146` **seq 23** / `152` **seq 28**: the floor's headline sentence was the most permissive
text in the file and said the opposite of the card fact two sections earlier. The lifegain clause
moves to Edit D's home, where it is read.

**Edit C — the CRACK-BACK EXCEPTION gains the blocker count.**
Before: `... keep home whatever blocks the biggest attackers; send only vigilance, or a Rorix whose
6 is lethal THIS turn.`
After: same, plus `COUNT THE BLOCKERS BEFORE YOU CALL THAT LINE FATAL: each untapped creature you
keep home stops ONE whole attacker, so keeping N home takes the N biggest attackers out of that
total. "I die either way, so I may as well swing" is that count left undone.`
Paid by `146` **seq 29**, where the reasoning reaches `Is there any way to survive next turn? No.`
with two blockers on the board and two attackers on theirs. The existing rule names the right
answer; what was missing is the number that makes it obviously right, and the last sentence quotes
the exact false step back at the model.

**Edit D — the Hammer return's lifegain gate moves into the Hammer card fact.**
Before (PLAN LINES, deleted in B2) / After (appended to `THE EMPTY-HAND RULE`): `THE RETURN IS OFF
while "Opponent life trend:" shows them gaining 3 or more a turn: eight mana for 3 against a board
that gains 5 is not a clock - spend the turn deploying.`
Paid by `125` seqs 109–188, second wave running. The rule was never wrong, only unreachable: it
sat at the end of a plan-seam floor, and the decision is taken at the upkeep priority menu where
the model is reading the HAMMER paragraph.

**Bytes trimmed to pay for the above (−720):** the `By land count:` curve line (its five entries
restate ORDER OF OPERATIONS entries 2–5, and the seat's curve play was correct in all six games);
the ATTACK FLOOR restatement `The list is already filtered for legality: never hold back on
summoning sickness, or on their "[tapped ...]"` (0 violations, three waves, and the board line
prints `[summoning sick - ... but CAN block]` itself); `YOUR DRAW STEP IS MANDATORY AND NO ROW
SKIPS IT` (the punisher paragraph now prints `This draw step is COMPULSORY - no row on any menu
declines it` twice per render); the TARGETING preamble's restatement of the face row's own text;
`"Hold mana for their turn" is not a play here` (0 instances of holding for the opponent's turn in
two waves); the CLEANUP copy-notation example; the intro's `"Mana available:" is always correct`;
and the LIFEGAIN CROSS-CHECK's `and neither is a Hammer return`, now stated where Edit D put it.

### Rejected, though I looked at them
- **A rule off `162` seq 44** (Spellbomb at a face at opp 17 with every creature row reading
  SURVIVES). `TARGETING DAMAGE` step 3 and `NEVER SPEND DAMAGE ON A SURVIVOR` already name the
  answer, nothing else on that menu was worth taking, and Edit B now closes the priority-menu
  version of the same row. A fourth statement of it would be noise.
- **A land-plan tightening off `162` seq 41 / `125` seq 134.** Both cast into a plan the guide had
  already declared dead, and in both the prescribed `Cast nothing right now` leaves the mana with
  nothing else to do. Not paid.
- **An anti-race clause off `152` seq 33** (Hammer at the face at 4 life vs opp 16). Edit B's
  gate and the existing RACE CHECK cover it; the incremental rule would fire on the same board.

---

## (4) Optional proposals
**None.** The two items worth generalising are engine work (§2 HIGH-1's crack-back-after-this-row
clause and §2 MED-3's face-row cost clause), not general-guide or strategy-skill text, and both
are stated as engine items above. Nothing this seat saw contradicts an existing amendment or needs
a new one.

## What I did NOT check
- **The opponent seats' JSONL.** Board state and their spells were read out of my own rendered
  prompts and the shared stderr; I opened no opponent translog.
- **Any code.** Every claim about a counter, a skipped window or an unfired instrument is a
  transcript/stderr observation. I cannot say whether `forced_close_unrecorded` 4 lost a decision,
  which seat each `unclosed <think>` line belongs to, or whether `blocker_forecast_gang` 0,
  `chain_windows_collapsed` 0 and `sibling_window_asks_skipped` 0 are unwired counters or honest
  zeroes.
- **The `askreplay/` sidecar**, and the replay path behind `identical_ask_answers_reserved` 47 /
  `ask_replays_reserved` 63 beyond the stderr line that names it (`the same ask again, unchanged -
  re-serving this seat's own answer 3 of 3 (8 this game)`). I did not verify that every re-serve
  was of an identical menu.
- **The 12 `async_drops`** — I did not trace them individually; 0 `phase2_answer_missing` and 0
  fallbacks are consistent with none lost, unconfirmed.
- **P18 / P5 / P10 / P16 / P3 / P4** — no instance exists in this seat; UNTESTED, not PASS.
- **MULLIGAN past the opening window**: one mulligan and one bottom decision in six games
  (`125` seqs 2–4), both fine; the BOTTOMING and CLEANUP bullets remain untested for a third wave.
- **Whether 2/6 is a real strength change.** I priced three decisions and made no
  matchup- or field-level accounting.
