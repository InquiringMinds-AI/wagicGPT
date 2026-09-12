# Wave 78 — deck 130 (mono-red land destruction) — per-deck review

Corpus `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260911-125420-final`, single binary
`archives/wagic-47ef378c8-w78step1`, `--thinking on`, one run, 21 concurrent, all finished
naturally. Live guide under review = the wave-77 edit (19,841 B), installed 2026-09-11.
**Result: 0 wins / 6.** Wave 76 was 4/6, wave 77 1/6.

**Seat census.** 321 model decisions over 6 games (wave 77: 431), **100% with native reasoning**
(median `reasoning_chars` ~6,000, max 16,741; `thinking: on` on every record), **0 fallbacks**
(0 `unparsed_reply`, 0 `answer_label_absent`, 0 `recovery`, 0 `defer`) — the corpus's 7 unparsed
replies are all in deck125 and deck162 seats, and the `125v130` seq 303 one is in the *opponent's*
log, not mine, so I have nothing to quote. 0 `reply_truncated`, 0 `action_before_plan`.
`protocol_deviation`: compliant 306 / `unlabelled_plan` 14 / `plan_absent` 1. Kinds: ask 226,
priority 75, attackers 16, forced_close 6, blockers 3, bottom 1. Latency p50 90 s, max 332 s.
Counter sums over the six gameends: `hold_windows_skipped` 126, `async_drops` 7,
`stack_drain_windows_asked` 7, `crossphase_identical_reputs` 5, `forced_close_events` 6 /
`_unrecorded` 3, `plan_names_uncastable_zone_card` 5, `repeat_annotated_takes` 4,
`reserve_decline_windows_noted` 1, and **0** for each of `own_loop_windows_asked`,
`stop_reached_windows_skipped`, `stop_reached_reputs_collapsed`, `mana_only_windows_skipped`,
`crossphase_board_unchanged`, `menu_pass_no_progress`, `engine_reveal_floor_picks`,
`put_gloss_stripped`, `declined_face_latches`, `answer_label_absent_heuristic_played`,
`phase2_answer_recovered`/`_missing`, `identical_option_asks_resolved`, `plan_names_stranded_card`.
Prompt bytes: max **29,641**, median 12,492, 77 of 321 over 20 KB, **57 over 24 KB** — every one of
the 57 in the 68-turn `125` game (§2 MED-1).

---

## (1) Game-by-game, and the deciding decisions

| Opponent | Result | Turn | Final life | Decisions | Priced to |
|---|---|---|---|---|---|
| 146 | **LOSS** | 22 | -1 / 15 | 51 | **a decision** (seqs 53/54) |
| 152 | **LOSS** | 14 | -3 / 24 | 22 | matchup, one rule break |
| 162 | **LOSS** | 13 | 0 / 12 | 43 | matchup, one rule break (seq 27/28) |
| 126 | **LOSS** | 18 | 0 / 31 | 37 | matchup |
| 123 | **LOSS** | 14 | -76 / 16 | 24 | matchup |
| 125 | **LOSS** | 68 | 0 / 40 | 144 | matchup |

Only ONE of the six is priced to a decision of this seat's, and it is priced tightly: the seat had
a survivable block and sacrificed the blockers away two windows earlier. Two more carry a rule
break against a rule already on the live surface, in games that were probably lost anyway. The
remaining three are boards this deck holds no card for. Against last wave: the Hammer-return
loop and the GANG-BLOCK misassignment that wave 77 paid for did **not** recur.

### vs 146 — LOSS on turn 22 at **-1 life**. The paid decision: two Goblins sacrificed at the face out of a block that survived
`1789149298-...-vs-ai_baka_deck146.jsonl`. At **seq 52** (t22, main 1, life **1**, opponent 19) the
seat cast Siege-Gang Commander into `CRACK-BACK NEXT TURN: 3 of their creatures ... for up to 7 -
you would be at -6; that would KILL you`, giving it four bodies — `[summoning sick - cannot attack
this turn, but CAN block]` on all four.

At **seq 53** the priority menu printed, among others:

```
1. Deal 2 damage ... targeting Goblin #1 [opponent's battlefield] {right now: takes 2 damage - DIES}
   {removes 2 from the CRACK-BACK total above: 7 -> 5 - you would be at -4; that still KILLS you}
5. ... targeting Goblin #2 [opponent's battlefield] {... - DIES} {removes 2 ... that still KILLS you}
8. Deal 2 damage ... targeting the opponent {right now: takes 2 damage - they would be at 17}
```

It took **row 8**. At **seq 54** it took the face row again. At **seq 55** the blockers window read

```
INCOMING THIS COMBAT: 3 attackers, 7 unblocked damage ... (your creatures can legally block at most
2 of those 3 attackers at once ...) - best case with every blocker assigned: you would be at -1;
no block saves you
A1. Nadaar, Selfless Paladin (3/3) deals 3 [vigilance] [GANG BLOCK: any 2 of yours together deal 3,
    enough to kill this attacker]
A2. Goblin #1 (2/2) deals 2     A3. Goblin #2 (2/2) deals 2
```

with **two** blockers left. Kept, the four bodies cover all three attackers — Commander on A2
(`both die`), one Goblin each on A1 and A3 — and **zero** damage gets through. The seat dies at
**-1**, the exact deficit of the two Goblins it spent.

The guide forbade both takes in print. `"{1}{R}, Sacrifice a Goblin: 2 damage" IS REMOVAL, NEVER A
CLOCK, WHILE THEIR LINE COUNTS A CREATURE "without a restriction against attacking"` — their line
read `3 are creatures, 3 of them without a restriction against attacking` — `take it there only
when the row reads "- DIES" or its "they would be at N" is 0 or less. On that board the FACE ROW IS
CLOSED`. And the seat's own reasoning at seq 53 reads the rule, resolves it correctly, and then
overrides it on the ground that the row exists:

> *"THE 6-LIFE FLOOR RELEASES on two printed numbers ... Opponent has 3 creatures and 8 lands. Not
> released." / "Opponent life: 19 -> face row is not on menu unless life <= 6. **But option 8 is
> available. I'll go with 8.**"*

and at seq 54 it prices the removal row off the engine's own clause:

> *"Should I target Goblin? It dies, reduces crackback by 2, but I still die. Dealing damage to
> face is best."*

Two separate causes, both fixable: the guide states the prohibition as a **false claim about the
menu** ("that row is not on your menu"), which the model refuted from the screen and then discarded
the rule with it (**Edit A**); and the removal row's own annotation prices only the damage it
removes, never the blocker the sacrifice spends, and closes with `that still KILLS you` — the
engine tells it the removal is futile and says nothing about the body it keeps by declining
(**§2 HIGH-1**, and **Edit B** on the guide side).

### vs 152 — LOSS on turn 14. Matchup; one X-menu rule break on the way down
`1789149294-...-vs-ai_baka_deck152.jsonl`. Sigarda 4/4 flier + Elite Spellbinder 5/3 flier behind
Ranger Class and Luminarch Aspirant; at **seq 21** the seat is at **1 life** under
`CRACK-BACK ... for up to 13 ... of that, 9 from 2 attackers nothing you control can legally block`,
against a deck it cannot race at 24. Dead on the board. The block at **seq 14** (t11, two Goblins on
Intrepid Adversary, taking 5 from Sigarda) is priced correctly by the guide and is not the loss.
The break is at **seqs 23/25**: the Starstorm row carried

```
[<- the only X that kills anything of THEIRS costs you more of YOURS than it takes - THEIRS 3 / YOURS 5]
3. X = 3 {X pricing: kills THEIRS: Elite Spellbinder, Wolf, Luminarch Aspirant; YOURS: Goblin x4,
   Siege-Gang Commander}
```

and the seat took X=3, killing its own Commander and four Goblins. The guide's DO NOT CAST clause
(a) names exactly this (`the YOURS list at that X names Rorix or Siege-Gang`), and the SURVIVAL
OVERRIDE's own escape (`unless what it leaves alive still totals your life or more`) also holds
(Sigarda's 4 against 1 life). The marked-row rule as currently written says a marked row **"is the
answer"** — and the marker on this menu is a **refusal**. Wave 77 trimmed the sentence that said so
(see §3). Not the loss, but the rule as it stands actively points at the wrong row. → **Edit C**.

### vs 162 — LOSS on turn 13. Matchup (two Underworld Dreams); one face-row break that cost the only answer to the walker
`1789149296-...-vs-ai_baka_deck162.jsonl`. Two Underworld Dreams from t9 (`Every card YOU draw
costs you 2 life to theirs ... Your DRAW STEP is COMPULSORY`), then Ob Nixilis, the Hate-Twisted,
then Howling Mine + Dictate of Kruphix forcing extra draws. The seat has no card that answers an
enchantment and took 16 damage in four turns from its own draws.
The break is **seq 27/28** (t10, opponent at **18**, their line `0 are creatures`): the seat cast
Hammer of Bogardan and targeted `1. The opponent (player, life 18) {right now: takes 3 damage - they
would be at 15}`. The guide gives the face row only at `"Opponent life:" is 6 or less` and says an
empty opposing board `is a reason to HOLD`. One turn later Ob Nixilis entered and went to **3
loyalty** — exactly Hammer's 3 — and proceeded to destroy both Blastminers and ping 1/turn. The
guide's TARGETING step 1 would have aimed the held Hammer there. I do not claim it saves the game
(the Dreams clock was the killer), but it is the seat's only removal that ever lined up with a
printed loyalty number and it had been spent at a face the guide closed. Same rule as Edit A.

### vs 126 — LOSS on turn 18. Matchup: Sanguine Bond + Exquisite Blood
`1789149289-...-vs-ai_baka_deck126.jsonl`. The seat was at 20 life with Rorix on board and the
opponent at 12 when the loop closed. The window at **seq 39** states the trap honestly and both
answers lose: `both halves of their life LOOP are in play ... Blocking to cut the damage does not
bound it ... Declining every block does not escape it either: the 1 unblocked damage is life you
lose, which is itself an entry into the chain.` No decision of the seat's is the loss. Worth
recording as the shape that keeps recurring: the deck reached its first 6-mana threat on **turn
16**, having spent turns 6/8/10/12 on four land kills against a deck that then cast Chromatic
Lantern. I did not turn that into an edit — see §3 Rejected.

### vs 123 — LOSS on turn 14 at **-76**. Matchup: Intruder Alarm + Bloodline Keeper
`1789149269-...-vs-ai_baka_deck123.jsonl`. Turn 13: `Opponent cast Intruder Alarm ... Opponent
used: Create vampire with Bloodline Keeper (x22)`, then the Keeper flipped to Lord of Lineage and
the Vampires became 4/4s. At **seq 25** the board line reads `24 are creatures` and
`CRACK-BACK NEXT TURN: 24 of their creatures ... for up to 97 - you would be at -76`. The only
window where the Keeper was answerable is **seq 21** (t12): the seat had **2** untapped sources,
Starstorm read `[cannot pay now: needs {r}{r}{x}, your 2 untapped sources cannot pay it]`, and
nothing on the menu touched a 3/3. It never held 5 mana in the game (three Mountains and a
Talisman by t12). No misplay I can price.

### vs 125 — LOSS on turn 68. Matchup: Supreme Verdict, two Staff of Nin, counters, lifegain
`1789149302-...-vs-ai_baka_deck125.jsonl`. Supreme Verdict took Rorix at t19 and the seat never
held a creature again until t54. From t46 the seat ran the Hammer return-and-recast at **seqs 83,
90, 110, 111, 117, 118, 124, 129, 130, 141, 153** — the shape that was wave 77's Edit A. **This
time it is not a misplay**: at every one of those windows `Your hand (0 cards): (none)` and
`Mana available:` 16 → 25, with `0 are creatures` on its own battlefield. There was nothing else to
spend the mana on, and the guide's own FLOOR sends it there. The single best turn of the run is the
one time it had a Commander: **seqs 106-109** (t55), four sacrifices for 8 to the face, 47 → 39 —
more than two Hammer turns for less mana, which is what `IT BECOMES YOUR CLOCK` already says.
Where the wave-77 gate WAS violated — seq 141 (`unchanged at 35`, i.e. zero, so CLOSED) and seq 153
(`+5 since turn 64`, so CLOSED) — the violation cost nothing, because the alternative was an empty
hand. See §3 for why I am not re-editing that rule.

---

## (2) Engine / interface / card items

### HIGH-1 — a sacrifice-for-damage row prices the damage it REMOVES and never the BLOCKER it SPENDS, then closes with "that still KILLS you"
Repro `1789149298-...-vs-ai_baka_deck146.jsonl` **seq 53**, priority menu, life 1, four untapped
bodies, three attackers inbound:

```
1. Deal 2 damage with Siege-Gang Commander targeting Goblin #1 [opponent's battlefield]
   {right now: takes 2 damage - DIES}
   {removes 2 from the CRACK-BACK total above: 7 -> 5 - you would be at -4; that still KILLS you}
```

Two windows later (**seq 55**) the blockers header prices what those bodies were worth:
`best case with every blocker assigned: you would be at -1`. With the two Goblins unspent the
best case is **0 damage through** and the seat lives. Every row on seq 53 that removes an attacker
is stamped *futile*, and the option that keeps a blocker — declining — carries no clause at all.
The model's reasoning at seq 54 uses the clause verbatim to justify the face: *"It dies, reduces
crackback by 2, but I still die."* Fix shape: on any row whose cost line reads
`[cost: ..., Sacrifice]` while a CRACK-BACK line is printed, state the blocker arithmetic as well —
`this sacrifice also removes one of your N blockers; with it kept, the best-case incoming is M`.
The two halves already exist in the engine (the blockers header computes exactly that figure); they
are never shown on the same screen. **This is the single item in this seat that decided a game.**

### MED-1 — prompt bytes: max 29,641, and 20,647 of it is NARRATION (S13, deck130's half, recurs)
`...vs-ai_baka_deck125.jsonl` **seq 144** (t64): 29,641 B total, of which the GAME LOG is **20,647
B** and the situation-plus-menu is 8,994. The log IS trimmed (`(...earlier events trimmed -
graveyards at trim: ...)`) but the retained window still runs from turn ~18 to turn 64 — 46 turns.
57 of this seat's 321 windows are over the 24,000 B trigger and **all 57 are in this one game**.
The row-repetition half of S13 is **CLOSED** in this seat: I searched all 321 prompts for an
identical `{card text: ...}` block appearing twice in the same prompt and found **zero** (wave 77:
every row of a shared-source target menu carried its own copy). What is left is length of history,
not duplication per row. Fix shape: the trim's retained window should be turn-bounded, not
event-bounded — a 68-turn game does not need 46 turns of log.

### MED-2 — the `ON THE STACK: N damage to you` total does not count the punisher damage a queued DRAW trigger on the same stack will cause
`...vs-ai_baka_deck162.jsonl` **seq 49** (t14, life 1). The stack, top-down, is
`Howling Mine's Draw 1`, `Dictate of Kruphix's Draw 1`, `Ob Nixilis ... deals 1 damage to you`,
`Underworld Dreams's deal 1 damage`; the summary line reads
`ON THE STACK: 3 damage to you - you would be at -2; that would KILL you`. The same screen prints
`DRAW PUNISHERS on the battlefield: theirs - Underworld Dreams #1, Underworld Dreams #2. Every card
YOU draw costs you 2 life`, so the two Draw triggers above it are worth **4 more damage** that the
total excludes. The seat was dead either way here, so no decision turns on it, but it is a printed
total that understates a number the engine already knows. The rest of the stack arithmetic in this
seat is **true**: I checked all 6 `ON THE STACK: N damage` lines against the life line at that seq
(seqs 22/25/33/34/35/49) and every subtraction is correct.

### MED-3 — `plan_line_missing` is the dropped `PLAN:` LABEL, 14 of 15, eighth wave unchanged; the 15th is the seat's first genuine `plan_absent`
14 of the 15 `protocol_deviation` replies carry a first line that IS a plan, without the label:
`126` seq 36 `"\n\nCast Dwarven Blastminer to add a body to the battlefield for next turn's
attack.\nCHOICE: 2 (Cast Dwarven Blastminer)"`; `146` seq 35 `"\n\nRemove Triumphant Adventurer
with Starstorm X=1.\nCHOICE: 1 (Cast Card Normally)"`; `125` seq 116 `"\n\nDeal 3 damage to the
opponent to reduce their life total and maintain pressure.\nCHOICE: 1 (The opponent ...)"`.
The exception is `125` **seq 99**, classed `plan_absent`, whose whole reply is `"\n\nCHOICE: 1
(Cast Spark Spray)"` — the first genuine absence I have seen in this seat in three waves. Accepting
an unlabelled first line as the plan still zeroes 14 of 15.

### MED-4 — the reasoning body degenerates into output-formatting chatter on long windows
`146` seq 53 carries `reasoning_ngram_repeat 0.114` over 8,272 chars, and the tail is ~40 lines of
`"Output matches. Proceeds. Done. [Final Check] ... Ready. Output matches. Proceeds."` after the
decision is already made. `146` seq 54 is the same shape. These are the two windows that lost the
game, and in both the model spent its last two thousand reasoning characters re-reading the reply
format rather than the board. Not a correctness defect in the render; recorded because
`reasoning_ngram_repeat` is already instrumented and these two records are its two highest values
in the seat.

### LOW-1 — `hold_check_ref_seq` is now a RECORD seq and it joins (S7 CLOSED in this seat)
239 `[hold check:` brackets, 239 records carrying the ref, **221 resolve to a record in the same
log and 18 carry `-1`** — and those 18 are exactly the 18 `first window I have asked you at this
seam` sentences. Wave 77's HIGH-2 (the ref was a 0-based window index, 246 of 294 landing at the
wrong seam) is **closed**. 199 brackets carry a `gone` clause.
On the counts themselves I audited all 221 comparable pairs with a naive row key (strip `{...}`
and `[...]`, compare the residue): **189 agree, 32 disagree, and in every one of the 32 my key
counts MORE new rows than the bracket does** — the opposite direction from wave 77's over-count.
That is consistent with the engine's identity being coarser than my key (which is what the
bracket's own `a row that changes only in its annotations is not new` sentence says), so I record
this as *my instrument being the weaker one*, not as 32 false brackets. **Wave 77's S9(a) renumber
shape (`Goblin` → `Goblin #1` counted as new) does not appear in this seat this wave**: I searched
all 221 pairs for a bare-name row replaced by a `#n` row and found one hit, `152` seq 17 ← 13,
which is a genuine target-roster change (Sigarda 5/5 → 4/4, Intrepid Adversary gone, two Goblins
added), not a renumber.

### LOW-2 — `forced_close_unrecorded` 3 of 6, still unattributable from a seat's side
6 `forced_close` records in the seat (`152` seqs 15/16, `125` seqs 23/24/36/37), all with an empty
reply, against `force_close_arms_refused` / `_deferred` / `_same_arm_deferred` / `_defer_bound_hits`
all **0** for the fifth wave. The stderr is shared with the opponent seat, so I cannot name the arm
that superseded the 3 unrecorded ones. 0 fallbacks and 0 `phase2_answer_missing` are consistent
with no decision lost; unconfirmed.

### S12 — `Opponent life trend:` gained/lost split: PRESENT and TRUE (CY F7 PASSES here)
313 trend lines in the seat: **86** print the split, **105** print a signed net with no split, 122
read `unchanged at N`. The 86 split lines are the spans in which BOTH a gain event and a loss event
occurred — every split pair I collected has both halves non-zero — so the 105 net-only lines are
the spans where one stream is empty (21 of them PLUS, i.e. pure gain; 84 MINUS, i.e. pure loss)
and the net IS that stream. **No net-only line with both streams live.** I verified three splits
against the narrated events of their own span and all three are exact:
`125` seq 124 `turn 56: 39, turn 58: 36, turn 60: 35, now 35 (-4 since turn 56; ... +8 ... -12 ...)`
against `Opponent gained 3 life` (t57) + `gained 5 life` (t59) = 8 and four Hammer hits of 3 = 12;
`125` seq 90 (+10 / -2 over 26 → 34); `152` seq 21 (+5 / -1 over 20 → 24). No item.
The thing worth the core loop's attention is what the split now *proves*: the signed total is the
gain **already cancelled against the seat's own damage**, so a rule keyed on it (wave 77's Edit A)
opens exactly as the seat throws more damage. The GAINED figure is the number such a rule wants.
I did not re-edit that rule this wave — §3 says why.

### Card facts — verified, no item
Intruder Alarm (untap-all on any creature ETB — the Bloodline Keeper engine at `123` t13 is real),
Bloodline Keeper / Lord of Lineage (3/3 flier, tap for a 2/2 flier, flips at five Vampires, gives
other Vampires +2/+2 — the rendered 4/4s are correct), Devour Flesh, Underworld Dreams (1 per
opponent draw, stacks — the `Every card YOU draw costs you 2 life` summary for two copies is
right), Ob Nixilis the Hate-Twisted (−2 destroy + draw two, and the draws feed the Dreams),
Howling Mine, Dictate of Kruphix, Sanguine Bond + Exquisite Blood (the converter header states the
direction correctly), Sigarda Champion of Light, Elite Spellbinder, Luminarch Aspirant, Ranger
Class, Nadaar Selfless Paladin, Perimeter Captain, Overgrown Battlement, Staff of Nin, Supreme
Verdict, Chromatic Lantern. Own side: Starstorm (X to EACH creature, cycling {3}), Hammer of
Bogardan (3 to any target, upkeep-only {2}{R}{R}{R} return), Siege-Gang Commander (three Goblins,
{1}{R} sac for 2 to any target), Rorix, Dwarven Blastminer (nonbasic only), Molten Rain (2 to the
nonbasic's controller), Stone Rain, Lay Waste, Spark Spray, Pyrite Spellbomb, Talisman of Impulse,
Forgotten Cave. No Scryfall disagreement worth reporting.

---

## (3) Guide verdict: **EDIT** — revised guide at `wave78/deck130/strategy.txt`

**19,959 bytes vs the live 19,841 — net +118**, three edits (+658) paid by nine trims (-540).
Every trim removes text the engine now prints on the row itself or that an edit above absorbs. No
addition is a concession and each is a precondition (A334); no citation, count, or history is in
the guide (A332); headings are byte-identical to the live guide and still scope declarations
(A333); the PLAN template is untouched and nothing quotes "YOUR PLAN".

**The two edits that grow the guide earn their bytes this way:** Edit B is the only rule in the
file whose violation is a directly-priced loss this wave (−1 life, the exact cost of the two
bodies), and Edit A is the rule the model *read, resolved correctly, and then discarded* because
the guide had told it something about the menu that the menu contradicted.

**Edit A — the face row is a PROHIBITION, not a claim about what the menu contains.** (+89)
Before: ` 3. Only then the face, and only when "Opponent life:" is 6 or less; at 7 or more that row
is not on your menu, and an empty opposing board is a reason to HOLD.`
After: ` 3. Only then the face. THE FACE ROW IS PRINTED ON EVERY DAMAGE MENU AND ITS BEING THERE IS
NOT PERMISSION: while "Opponent life:" is 7 or more you do not take it, whatever else the window
offers, and an empty opposing board is a reason to HOLD.`
Paid by `146` **seq 53** — reasoning verbatim: *"Opponent life: 19 -> face row is not on menu
unless life <= 6. But option 8 is available. I'll go with 8."* — and by `162` **seq 27/28** (face
at opponent 18 with `0 are creatures` on their line). The threshold is unchanged; what changes is
that the rule no longer stakes itself on a false statement the screen refutes.

**Edit B — every sacrifice spends a blocker, and no row prices that.** (+377)
Before, the paragraph ended: `... On that board the FACE ROW IS CLOSED - a Goblin kept home stops
one whole attacker off the CRACK-BACK line, and spending it for 2 buys nothing. A line of nothing
but "[defender]" bodies counts 0 there.`
After, the same two sentences, then: `EVERY SACRIFICE SPENDS A BLOCKER AND NO ROW PRICES THAT:
count your untapped bodies against their attackers before each one, and while your count is at or
below theirs, sacrifice nothing. A "{removes N from the CRACK-BACK total ... that still KILLS you}"
clause is arithmetic for that row alone; it has not counted the body you keep by declining, which
stops a whole attacker.`
Paid by `146` **seqs 53 and 54** — the loss. The existing sentence asserts the value of a Goblin
kept home; it does not tell the model to COUNT, and it does not name the engine clause that talked
it out of the count. Both failures happened inside the same paragraph's scope, which is why this
is an extension of that rule and not a new heading.

**Edit C — the X-menu marker is read for WHAT IT SAYS, and the largest-X fallback is gated.** (+192)
Before: `ON THE X MENU: THE OPTION NUMBER IS THE X - option 4 announces X=4; option 1 is X=1, the
SMALLEST row there is. A marked row ("[<- most kills ...]" / "[<- best trade: ...]") is the answer -
a marker says WHICH X, never WHETHER. Unmarked: take the largest "kills THEIRS" whose "YOURS:"
reads "none", then walk down any "same kills as X=N, for K less mana" run to its lowest. Never a
row killing none.`
After: `ON THE X MENU, READ THE MARKER BEFORE ANY X. A marker naming a best or a most ("[<- most
kills ...]", "[<- best trade: ...]", "[<- best X for this cast: ...]") NAMES THE ROW YOU TAKE at
whatever X it sits on, and the largest affordable X is not that row unless the marker is on it. A
marker saying the sweep costs you more of YOURS than it takes of THEIRS is a REFUSAL: decline the
cast. Only with NO marker anywhere on the menu: take the largest "kills THEIRS" whose "YOURS:"
reads "none", then walk down any "same kills as X=N, for K less mana" run to its lowest. Never a
row killing none.`
Paid twice. `146` **seq 36**: row 1 read `X = 1 {X pricing: same kills as X=3, for 2 less mana}
... [<- most kills at any affordable X that costs you nothing - at X=1 that is THEIRS: Triumphant
Adventurer; YOURS: none]` and rows 2/3 `same kills`; the seat's own plan one window earlier said
*"Remove Triumphant Adventurer with Starstorm X=1 ... and leave mana for Siege-Gang Commander"* and
it then took **X=3**, spending its whole pool (`{leaves 0 of your 5 floating mana unspent - this X
spends your whole pool}`) and pushing Siege-Gang from turn 12 to turn 14 in a game lost at -1.
`152` **seq 25**: the marker on row 3 was `[<- the only X that kills anything of THEIRS costs you
more of YOURS than it takes - THEIRS 3 / YOURS 5]` and the seat took row 3, killing its own
Commander and four Goblins. The old text's `is the answer` licenses exactly that. The dropped
opening sentence (`THE OPTION NUMBER IS THE X - option 4 announces X=4 ...`) is printed verbatim by
the menu itself at both seqs, so it is trimmed rather than kept.

**Bytes trimmed to pay for the above (-540):**
`WHICH LAND`'s parenthetical `(Molten Rain deals 2 extra to its controller, and nonbasics fix their
colours)` (the Molten Rain row prints its own 2 damage; nonbasic/basic is on every target row —
`125` seq 71, `162` seq 45) (-79); the mulligan-row-size sentence `The mulligan row's own "would
keep N cards" is the NEXT hand's size, one lower: never read N off it.` (the row itself now reads
`Mulligan (a keep after this one would keep 6 cards)`, which says it — `125` seq 2) (-101); ORDER
OF OPERATIONS entry 0's tail `A second body is not worth a lethal you already have.` (no instance
in three waves) (-58); CYCLING's `Never twice in one turn.` (never violated in three waves) (-25);
the ONE OVERRIDE's rationale tail `Their land keeps; a body in hand can be discarded, exiled or
countered out of it.` (-82); CYCLING's cost list `Costs: Spark Spray and Forgotten Cave {R}, Lay
Waste {2}, Starstorm {3}.` (every cycling row prints `[cost: {2}, Cycle]` — `123` seq 11, `125`
seq 63) (-73); DWARVEN BLASTMINER's `and he must be untapped` (the entry prints `[tapped - cannot
attack or block this turn]` and the ability is simply not offered) (-25); YOUR OWN UPKEEP's
restatement `An upkeep row that taps you out is the wrong answer.` (the subtraction it restates is
the sentence immediately above it) (-53); and `"CHOOSE AN OPTION FOR <card>"`'s tail `and "Decline -
do nothing" is the way back.` (the menu prints that row with its own explanation; the seat took
`Cast Card Normally` correctly 11 of 11 this wave) (-44).

### What the wave-77 edits decided
- **Edit A (the Hammer return's gate, moved in front and keyed on the signed total)** — the loop
  recurred in `125` (11 returns, seqs 83-153) but this time **as the only legal use of the mana**:
  empty hand, 16-25 mana, empty board at every one. The gate was violated twice (seq 141 at zero,
  seq 153 at +5) at no cost. **The rule is UNTESTED in anger; its number is nonetheless wrong** —
  the S12 split now proves the signed total is the opponent's gain already netted against the
  seat's own damage, so the gate loosens as the seat throws more Hammers (seq 124: net −4 while
  `life-gain EVENTS put +8 on them`). I did **not** edit it: no decision in this corpus turns on
  it, and re-editing a rule on reasoning rather than a paid seq is the kind of unpaid churn the
  brief rejects. It is the first thing to re-test next wave, and the GAINED figure is the surface
  that would replace the net.
- **Edit B (SURVIVES rule names the declining row of the menu it is on)** — **PASS on its one
  case.** `162` seq 27 offered only SURVIVES-free rows and a face; no SURVIVES row was taken in
  the seat this wave (wave 77's `162` seq 20 shape did not recur).
- **Edit C (the GANG BLOCK count binds)** — **PASS.** One GANG BLOCK clause in the seat, `146` seq
  55 (`any 2 of yours together deal 3`), and the seat did not under-assign: it had only two
  blockers left and answered `BLOCKS: B1:A1, B2:A2`. The wave-77 failure (two bodies into a
  four-body requirement) did not recur. What lost the game was upstream of the block, not in it.
- **Edit D (a verdict beside each LAND PLAN number)** — **PASS.** `126` seq 22 reads
  `PLAN: Their lands 3 (ALIVE); my life 20 (ALIVE); their power 0 (ALIVE).` — verdict per number,
  exactly the shape asked for. The wave-77 `126` seq 36 failure (three numbers then one contrary
  verdict) did not recur.
- **Wave-77 HIGH-1 (hold-check count inflates on a renumber)** — no instance in this seat; see
  §2 LOW-1.
- **Wave-77 HIGH-2 (`hold_check_ref_seq` naming)** — **CLOSED** (§2 LOW-1).
- **Wave-77 MED-1 (`Opponent life trend:` nets your damage against their gain)** — the split is
  now **printed and true** (§2 S12); the guide rule that keys on the net is the residual.
- **Wave-77 MED-2 (repeated `{card text:}` per row on shared-source menus)** — **CLOSED**: zero
  repeated identical `{card text:}` blocks in 321 prompts. The size that remains is narration
  (§2 MED-1).
- **Wave-77 MED-3 (the dropped `PLAN:` label)** — **recurs**, 14 of 15 (§2 MED-3).
- **Wave-77 MED-4 (no crack-back cover when the crack-back is 1)** — **appears CLOSED**: `146`
  seq 36's X rows each carry a full cover clause on a crack-back of **1**
  (`the CRACK-BACK ... is 1 from 1 of their creatures ... leaving 0 -> you would be at 13, which
  you SURVIVE`). One instance, so I record it as closed-on-its-one-case.
- **Wave-77 LOW-1 (a `{effect:}` cap that removes a bounding clause)** — 58 `(...more)` cuts in the
  seat over **11 distinct contexts**; I read all 11 and every one cuts after a complete clause, with
  no cut that reverses a meaning (`Elite Spellbinder`, `Sigarda`, `Lord of Lineage`, `Intrepid
  Adversary`, `Nadaar`, `Siege-Gang Commander`, `Dwarven Blastminer`, `Shineshadow Snarl` x2,
  `Hive of the Eye Tyrant`). No item.
- **Wave-77 LOW-2 (`forced_close_unrecorded` unattributable)** — recurs at 3 (§2 LOW-2).

### Wave-78 lane predictions, as this seat can test them
- **S3 (stop-reached RE-PUT collapse)** — `stop_reached_windows_skipped` **0** and
  `stop_reached_reputs_collapsed` **0** in all six games: **UNTESTED here**, no window in the seat
  carries `ALREADY AT OR PAST your own stop`.
- **S4 (drain annotation counts ACTION_ABILITY triggers only)** — `stack_drain_windows_asked` **7**,
  all in `162` (the Underworld Dreams / Ob Nixilis trigger stacks). I could not locate a rendered
  drain annotation by any of its expected phrasings in those seven prompts, so I can neither quote
  one nor call it absent: **UNTESTED here**, and flagged for the engine seat, which has the
  corpus-wide quote duty. No seat of mine held across a drain and lost an option.
- **S6 (`[own loop verdict:]`)** — `own_loop_windows_asked` **0**, zero `[own loop verdict:` strings
  in 321 prompts: **UNTESTED here.** (The `126` game has a loop, but it is the OPPONENT's.)
- **S7 (`hold_check_ref_seq` = a record seq)** — **PASS**, 221/221 join, 18 `-1` on first windows.
- **S9 (`N gone`, renumber counting)** — **PASS as far as I can test it** (§2 LOW-1); the
  renumber over-count does not appear.
- **S11 (`forced_close` records)** — 6 records present and readable; `forced_close_unrecorded` 3 and
  every arm-bound counter 0 (§2 LOW-2). The bound has not fired for five waves.
- **S12 (gained/lost split)** — **PASS** (§2 S12), three spans verified exact.
- **S13 (prompt bytes / hoisted shared card text)** — row-repetition half **PASS** (0 duplicated
  `{card text:}` blocks); size half **FAILS at the narration** (§2 MED-1), and deck130's max is
  again narration, as wave 77 diagnosed.
- **S1 / S10 / S2 / S5 / S8 / S14 / S15** — no instance in this seat: 0 `answer_label_absent`, 0
  `defer`, 0 `repeat_count_reask`, 0 `already targets this on the stack` clauses, 0 counterspells
  of my own, 0 no-op folded bands I could identify. 13 `[second copy:` clauses appear and read
  correctly (`123` seq 21 on a second Talisman of Impulse, naming the repeated effect); 20
  `[repeat:` tags appear and the seat kept paying through them at `146` seq 54 and `125` seqs
  106-109. **UNTESTED**, not PASS.
- **{X}-announcement payment loss** — 0 `dropped after its X was announced` in all six of my stderr
  files.

### Rejected, though I looked at them
- **An ORDER OF OPERATIONS reordering that puts the body ahead of land destruction.** The pattern
  is real and visible in five of six games — 17 land kills, and the deck's first Rorix or
  Siege-Gang lands on turn 10 (`125`), 12 (`146`), 16 (`126`), never (`123`, `162`) — but I could
  not price a single window where a body was castable and a land kill was taken instead. At `123`
  seq 15 and `162` seq 36 the body read `[cannot pay now: needs 5 mana, you have 4 untapped
  sources]`, which is mana, not instruction, and the ONE OVERRIDE already fires the moment the body
  is castable. Re-ranking entries 3 and 4 on a pattern I cannot pay for would be reading variance
  as guide effect.
- **A "damage does not carry across turns" or "hold removal for a walker" rule off `162`.** Edit A
  already closes the window that spent the Hammer; a second rule about what it could have killed a
  turn later would be an unpaid hypothetical.
- **Re-editing the Hammer-return gate onto the GAINED figure.** See §3 above: correct in argument,
  unpaid by any decision in this corpus.
- **A decking rule off `125`.** The seat lost at 0 life to two Staff of Nin, not to its library, and
  its optional draws were not the loss.

---

## (4) Optional proposals
**None.** The two generalisable findings are engine work and are stated as engine items: §2 HIGH-1
(a sacrifice row must price the blocker it spends, on the same screen as the crack-back total —
this is not deck130-specific; any deck with a sacrifice outlet under a lethal crack-back reads the
same false futility) and §2 MED-2 (the on-stack damage total excluding punisher damage from queued
draw triggers). Nothing this seat saw contradicts an existing amendment or needs a new one.

## What I did NOT check
- **The opponent seats' JSONL.** Board state and their plays were read from my own rendered prompts
  and the shared stderr; I opened no opponent translog. The corpus's `125v130` `answer_label_absent`
  fallback is in the deck125 seat and I did not open it — the engine seat has it.
- **Any code.** Every claim about a counter, a skipped window, a trim trigger or an unfired
  instrument is a transcript observation.
- **The S4 drain annotation's text.** 7 windows carry the counter; I could not find the rendered
  clause and did not conclude it is missing.
- **Whether `own_loop_windows_asked` 0, `stop_reached_*` 0, `mana_only_windows_skipped` 0 and the
  `force_close_arms_*` zeros are unwired counters or honest zeroes.** No instance either way.
- **The `askreplay/` sidecar**, the replay path behind `ask_replays_reserved` 21, and the 7
  `async_drops` individually (0 `phase2_answer_missing` and 0 fallbacks are consistent with none
  lost; unconfirmed).
- **The 32 hold-check brackets my naive row key disagreed with.** I did not reconstruct the
  engine's row identity; I report the disagreement's direction and leave the adjudication open.
- **Whether 0/6 is a real strength change.** Four of the six losses are to boards this deck holds
  no card for (a 24-Vampire Intruder Alarm engine, a Sanguine Bond loop, two Underworld Dreams with
  a compulsory draw step, a Supreme Verdict deck with two Staff of Nin). I priced one loss to a
  decision and made no matchup- or field-level accounting.
