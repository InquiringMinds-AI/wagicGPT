# Wave 77 — deck 130 (mono-red land destruction) — per-deck review

Corpus `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260911-010157-final` (consolidated:
1 first-run game + 20 reruns, single binary `archives/wagic-42c41fc66-w77step1`, `--thinking on`).
All six of this seat's games are RERUNS (start epochs 1789114814–1789114910), all finished
naturally. Live guide = the wave-76 edit (19,929 B).

**Seat census.** 431 model decisions over 6 games (wave 76: 264), **100% with native reasoning**
(median `reasoning_chars` 5,983, max 20,001), **0 fallbacks** (0 `unparsed_reply`, 0
`noop_row_zero_reask`, 0 `repeat_count_reask`, 0 `defer`, 0 `recovery` — the corpus-wide 6 are not
in this seat; I have nothing to quote), 0 `reply_truncated`, 0 `action_before_plan`, 431/431
`protocol_deviation: compliant` at transport, 431/431 `curl=0,http=200`. Kinds: ask 296, priority
123, attackers 8, discard 3, blockers 1. Latency p50 88.2 s. Counter sums over the six gameends:
`ask_replays_reserved` 32, `identical_ask_answers_reserved` 24, `hold_windows_skipped` 108
(cast 52 / priority 56), `hold_released_turn` 14, `own_turn_windows_skipped` 7,
`main_phase_windows_skipped` 0, `mana_only_windows_skipped` 0, `plan_line_missing` 37,
`protocol_deviation_replies` 37, `plan_names_uncastable_zone_card` 5, `plan_names_stranded_card` 0,
`async_drops` 10, `phase2_answer_recovered` 1 / missing 0, `forced_close_unrecorded` 3,
`crossphase_identical_reputs` 0, `own_loop_windows_asked` 0, `force_close_arms_*` 0/0/0,
`repeat_annotated_takes` 2, `identical_option_asks_resolved` 1, `blocker_forecast_*` 0,
`chain_acting_rows` 83 / `chain_selfharm_rows` 22 / `chain_windows_only_selfharm` 8.
Prompt bytes: max **30,125**, median 12,512, **118 of 431 over 20 KB** (wave 76: max 17,199,
median 9,529, 0 over 20 KB) — see §2 MED-2.

---

## (1) Game-by-game, and the deciding decisions

| Opponent | Result | Turn | Final life | Decisions |
|---|---|---|---|---|
| 123 | **WIN** | 24 | 20 / 0 | 74 |
| 125 | LOSS (decked) | 71 | 6 / 43 | 194 |
| 126 | LOSS | 16 | -1 / 17 | 35 |
| 146 | LOSS | 23 | -6 / 5 | 55 |
| 152 | LOSS | 16 | -7 / 16 | 46 |
| 162 | LOSS | 11 | 0 / 18 | 27 |

**1/6 (wave 76: 4/6).** Hands are real; I priced decisions, not the record. Two of the five losses
(126, 152) I can price to the opponent's board and not to a decision of this seat's; one (162) is
a matchup the deck cannot beat and carries one clear misplay that did not decide it; two (146, 125)
turn on a decision each, and BOTH of those decisions are guide-compliance failures against rules
already on the live surface. §3 says what the wave-76 edits did.

### vs 146 — LOSS on turn 23. **The paid decision: two 1/1s into a printed four-body gang requirement**
`1789114864-...-vs-ai_baka_deck146.jsonl` **seq 46** (t17, Blockers, my life 16, 4 available
blockers: Siege-Gang Commander 2/2 and three Goblins). The attacker line read

```
A2. Triumphant Adventurer (2/2) deals 2 [first strike, deathtouch] [GANG BLOCK: any 4 of yours
    together deal 2 past everything its own first strike can kill, enough to kill this attacker;
    each B-line result below is a LONE blocker only]
B1..B4 - may block A2 (your blocker dies, attacker lives)   (all four rows)
```
plus the header `Unblocked, these attackers deal up to 3 - you would be at 13 - NOT lethal`. The
seat answered `BLOCKS: B2:A2, B3:A2` — **two** of the four the clause names. Both Goblins died in
the first-strike step, the Adventurer lived, and the reasoning states the dismissal in so many
words: *"One minor point: The prompt says 'GANG BLOCK: any 4 of yours together deal 2 past
everything its own first strike can kill'. This is just engine text. I'll ignore it as my math is
solid."* Its "math" also assumed the Commander was blocking (*"second blocker deals 1, SGC deals
2. TA dies"*), which its own answer did not assign. The guide already forbade the block twice — the
BLOCKING bullet `N above 5 and every row on that attacker reads "(your blocker dies, attacker
lives)" -> "BLOCKS: none"`, and the two-on-one clause buried under the *other* life branch — and
the prompt forbade it a third time (`Use that number; do not re-derive it`). The seat went from
four bodies to two; by **seq 58** (t22) it controlled no creature, the crack-back read `total to
subtract from your life is 10 and you would be at -1; that would KILL you`, and it died on t23.
**Guide item -> Edit C** (STRATEGY; every surface was true).

### vs 125 — LOSS on turn 71, decked. **The paid decision: the Hammer return taken ~10 times into a line gaining faster than 3 a turn**
`1789114829-...-vs-ai_baka_deck125.jsonl`. Both Rorix casts were countered (Cancel t22, Essence
Scatter t70 — the seat's hand was one card at each, so the guide's bait rule had nothing to bait
with) and the seat never controlled a creature: **0 attackers records in 194 decisions.** From t44
to t72 it ran the graveyard Hammer loop — return `{2}{R}{R}{R}`, recast `{1}{R}{R}`, 3 to the face —
at **seqs 84, 89, 93, 98/99, 106/107, 114/115, 121, 158, 166/167, 203**, against a deck gaining 5
to 7 a turn. Opponent life over that run: 16 → 21 → 26 → 31 → 28 → 32 → 35 → 40 → 43. The seat's
own library was the clock it lost to.

The guide's OFF switch for exactly this (`THE RETURN IS OFF while "Opponent life trend:" shows them
gaining 3 or more a turn`) was **true on the printed surface and never evaluated**. The trend line
said so plainly and repeatedly — seq 89 `turn 44: 16, turn 46: 21, turn 48: 26, now 26 (+10 since
turn 44)`; seq 128 `(+4 since turn 56)`; seq 146 `(+10 since turn 62)` — and the reasoning at
**seq 106** quotes the rule it did follow and nothing else: *"According to the strategy guide: 'THE
EMPTY-HAND RULE wins games: when Your hand reads (0 cards) ... TAKE THE RETURN EVERY UPKEEP'. I
have 0 cards in hand. Mana is 21. I should return Hammer of Bogardan."* The gate is written as a
trailing sentence of the paragraph whose headline is `THE EMPTY-HAND RULE WINS GAMES`; the model
read the headline and stopped. **Guide item -> Edit A** (STRATEGY, ordering).
One render contributor is real and is §2 MED-1: on the two turns the seat's own damage happened to
out-pace the gain the same line reads `-5 since turn 50` while the opponent is climbing, so the
figure the rule keys on flickers.

### vs 162 — LOSS on turn 11. Matchup, plus one row the guide forbids in print
`1789114910-...-vs-ai_baka_deck162.jsonl`. Howling Mine + Fate Unraveler (`Whenever an opponent
draws a card, Fate Unraveler deals 1 damage to that player`) + Master of the Feast 5/5 flier. The
seat's own compulsory draws cost it 2–3 a turn; 20 → 15 → 13 → 3 by t10. The deck has no answer to
a 5/5 and I can price no decision as the loss.
The misplay is **seq 20** (t9, their upkeep, my life 13). Every creature row on that priority menu
printed its own verdict: `1. ... targeting Fate Unraveler {right now: takes 2 damage - SURVIVES
(toughness 4)}`, `2. ... targeting Master of the Feast {right now: takes 2 damage - SURVIVES
(toughness 5)}`, `3. ... Shield Sphere ... SURVIVES (toughness 6)`, and each added `{paying this
costs you 1 life (Talisman of Impulse: 1 damage) - you would be at 12}`. The seat took **row 2** —
a SURVIVES row, for 1 life, on a 5/5 — while TARGETING step 2 says `"- SURVIVES (toughness N)" the
one you decline` and `NEVER SPEND DAMAGE ON A SURVIVOR`. Its PLAN then compounded it: *"Next turn
... use Hammer to kill Master of the Feast"* — Hammer deals 3 to a 5/5, and damage does not carry
across turns. The rule's stated remedy (`the answer one screen earlier was "Cast nothing right
now"`) names a row that does not exist on a priority menu, which is where every Spellbomb
activation is offered. **Guide item -> Edit B.**

### vs 126 — LOSS on turn 16. Sanguine Bond + Exquisite Blood; one land-plan test misread on the way
`1789114844-...-vs-ai_baka_deck126.jsonl`. The seat was ahead on board the whole game (Rorix from
t12, opponent 19 → 6 by t14) and lost to the combo assembling on t15–16: the stderr shows the
`Sanguine Bond` / `Exquisite Blood` chain resolving link by link from 14 life to -1. Nothing on
the seat's menus answers an enchantment, so I price this to the matchup.
Worth recording because the same shape recurs: at **seq 36** (t16, 7 mana, their line 6 lands) the
reasoning wrote the guide's own three numbers and then wrote the opposite of the test —
*"Their lands: 6 total. My life: 14. Their power: 3 (vampires). Land plan is alive."* THE LAND PLAN
kills the plan at `N is 5 or more`. Stone Rain was cast; it changed nothing. **Guide item ->
Edit D**, small, because the number-then-verdict shape is what failed and not the threshold.

### vs 152 — LOSS on turn 16. Matchup; Starstorm did the most it could
`1789114895-...-vs-ai_baka_deck152.jsonl`. Ranger Class levelled, Brutal Cathar flipped to a 6/6
first-striker on the night turn, Tovolar's Huntmaster made four Wolves and flipped to a 7/7. The
seat's two Starstorms (t15 X=2, **seq 53** t16 X=2) each took the marked row — `[<- best X for this
cast: X=2 - most kills at any affordable X that costs you nothing]`, `kills THEIRS: Wolf x4; YOURS:
none` — and neither could touch the 6/6 or the 7/7 at any affordable X. 21 unblockable crack-back
against 7 life. No decision of this seat's is the loss.

### vs 123 — WIN on turn 24, clean
`1789114814-...-vs-ai_baka_deck123.jsonl`. Land destruction plus Lay Waste held them to three
lands; Siege-Gang landed on an empty line at t18, the face sacrifices fired at **seqs 71/72/74/75**
exactly where wave-75 Edit B/B2 and wave-76 Edit C put them (their line reading `0 are creatures`
or only defenders), and Hammer recursion finished it at **seq 83** on the row printing
`{right now: takes 3 damage - they would be at 0; THIS WINS THE GAME}`. Seat life never moved off
20. I checked **seq 79** (Stone Rain with the opponent at 3) for a missed lethal: the seat's
battlefield read `0 are creatures`, so there was no sacrifice mana to count and Stone Rain was the
only acting row. No misplay in this game.

---

## (2) Engine / interface / card items

### HIGH-1 — the `[hold check: N rows above are new]` count inflates when a second copy renumbers an existing row's handle
The bracket is otherwise in good health in this seat after R2: I decoded `hold_check_ref_seq` and
audited every comparable bracket against the record it names — **323 comparable, 318 TRUE, 5
FALSE**, 18 `first window asked` sentences all with no predecessor at that seam, 0 land/cast
cross-seam leaks. All five falses are one shape. Cleanest repro, `152` **seq 6 -> seq 10** (priority
seam, t4 -> t6), bracket `8 rows above are new since the last window I asked you at this seam`:

```
seq 6  rows: Deal 2 damage with Pyrite Spellbomb targeting {Wolf, the opponent, you}; Draw 1 with
             Pyrite Spellbomb; hold; pass                                     (2 Spellbomb sources)
seq 10 rows: ... targeting {Wolf, the opponent, you} x2 (#1 and #2); Draw 1 x2 (#1, #2); hold; pass
```
Five rows genuinely appeared (the second Spellbomb's four rows plus one target). The other three
are the SAME actions whose printed handle changed from `Pyrite Spellbomb` to `Pyrite Spellbomb #1`
when the second copy entered. Same shape at `126` seq 12 -> 18 (`8 ... are new`, 5 appeared) and at
`123` seq 62 -> 70 / `125` seq 84 -> 88 (`6 ... are new`, 3 appeared — Siege-Gang's sacrifice list
picking up `#` handles). Fix shape: the hold key's action identity should carry the permanent's
id, not its rendered `#n` display handle, so a renumber is not an appearance.
**R2c is otherwise CLOSED in this seat**, and CR's refutation holds where I could test it: I
re-checked `152` **seq 30 -> 31**, which my first (naive) key called a false `1 row above is new` —
the row's target roster really did shrink from `2 CREATURE targets ... Brutal Cathar #1, #2` to
`1 CREATURE target ... Brutal Cathar` after the seat killed one at seq 29. A real target change.

### HIGH-2 — `hold_check_ref_seq` is NOT a record `seq`, and nothing on the record says so
It is a **0-based index into the seat's own ordered record list** (the Nth record this seat wrote,
counting the mulligan record as 0). Reading it as the `seq` field it is named after puts 246 of 294
refs at a different seam and 29 at no record at all; decoded as an index, 294 of 294 land on the
correct same-seam predecessor. That cost this seat a full audit pass and would have produced a
confident wrong "the seam key is broken" finding if I had stopped there. Either rename it
(`hold_check_ref_index`) or emit the referenced record's `seq`.

### MED-1 — `Opponent life trend:` is a NET-of-your-damage figure, so a lifegain engine can read as a decline
`125` **seq 106** (t54): the opponent has gained 7 and 5 on the two turns sampled and the line
reads `turn 50: 31, turn 52: 25, turn 54: 26, now 26 (-5 since turn 50)`. Three windows later
(**seq 116**) the same board reads `(+3 since turn 52)`, and at **seq 119**, after the seat's own
Hammer resolves, `(0 since turn 52)`. The signed total flips sign with the seat's own damage
inside the same turn. Every guide rule that keys on "are they gaining" — this deck's Hammer gate
and its LIFEGAIN CROSS-CHECK — keys on a number that does not separate their gain from the seat's
damage. Fix shape: print their GAIN separately from the net, e.g.
`gained 12 over these turns, took 9 from you`. The game log already carries every
`Opponent gained N life (now M)` line; nothing new has to be computed.

### MED-2 — prompt size jumped and 27% of this seat's windows are now over 20 KB
Max **30,125 B**, median 12,512, **118 of 431 over 20 KB** (wave 76 in this seat: 17,199 / 9,529 /
0). The growth is the R5 creature `{effect:}` gloss plus the expanded `{crack-back cover:}` and
`{X pricing:}` paragraphs, both of which earn their bytes — but the largest windows are the
Spellbomb/Siege-Gang target menus, where one `{card text: ...}` is repeated verbatim on every one
of five to eight rows that share a source. Fix shape: print a shared source's `{card text:}` once
on its first row and reference it from the rest. Still under CT's 38,657 B bound; reporting the
slope, not a breach.

### MED-3 — `plan_line_missing` is the dropped `PLAN:` LABEL, 37 of 37, seventh wave unchanged
Every one carries `off_protocol_bytes` equal to the label's own sentence and a first line that IS a
plan. Verbatim: `123` seq 36 `"\n\nDestroy Scrubland #2 with Lay Waste to cripple their mana base
while they have few lands.\nCHOICE: 2 (Scrubland #2)"`; `123` seq 42 `"\n\nCast Pyrite Spellbomb
for {1} to utilize mana and set up for future card draw or removal.\nCHOICE: 2 (Cast Pyrite
Spellbomb)"`; `123` seq 53 `"\n\nCycle both Starstorms to find creatures or land destruction.\n
CHOICE: 1 (cycling)"`. Zero genuine absences again. `protocol_deviation_replies` 37 =
`plan_line_missing` 37 exactly in this seat. Accepting an unlabelled first line as the plan zeroes
the whole family.

### MED-4 — the crack-back cover does not fire when the crack-back is 1, and that is the window where it would be decisive
R6a otherwise **PASSES** in this seat and the arithmetic is true: `146` **seq 60** prints a
per-victim conditional on a Hammer row — `This row removes ONE body and the target is YOUR choice:
Barrowin of Clan Undurr takes 3 off that total, leaving 4; Spirit Cai, Goblin each take 1 off that
total, leaving 6. The most any ONE of them takes off is 3, leaving 4` — exactly the shape wave 76
asked for, and `123` seq 11 carries both the sweep form and the targeted form on the same menu.
The gap: `146` **seq 37** has a Hammer row reading `{kills: Triumphant Adventurer ...}` under
`CRACK-BACK NEXT TURN: 1 of their creatures ... for up to 1` and **no cover clause at all**, where
the honest clause is "takes 1 off, leaving 0 -> you would be at 17, which you SURVIVE". Some
threshold suppresses the clause at small totals; the survive verdict is the one case where the
clause is worth the most bytes.

### LOW-1 — one `{effect:}` gloss cap removes the clause that bounds the effect
26 distinct glosses in this seat, 3 ellipsis-capped, and one of the three changes the meaning:
`Brutal Cathar` renders as `{effect: "When this creature enters or transforms into Brutal Cathar,
exile target creature an opponent controls until this..."}` — the cut falls exactly on `until this
leaves the battlefield`, so the gloss reads as permanent exile. The other two caps
(`Barrowin of Clan Undurr`, `Elite Spellbinder`) drop only detail. Otherwise R5 is a clear PASS:
187 of 308 creature entries glossed, and the 121 without are vanilla or keyword-only bodies
(Goblin, Wolf, Vampire tokens, Rorix, Spirit Cai). No gloss in this seat states a false fact.

### LOW-2 — `forced_close_unrecorded` 3, still unattributable from a seat's side
2 in the `123` game, 1 in `125`, against `force_close_arms_refused/deferred/same_arm_deferred/
defer_bound_hits` all 0 — the bound has now not fired for four waves. The stderr is shared with the
opponent seat, so I cannot name the arm. 0 fallbacks and 0 `phase2_answer_missing` in the seat are
consistent with no decision lost; unconfirmed.

### Verifying the wave-77 lanes against this seat
- **R1 (own loop)** — `own_loop_windows_asked` **0** in all six games: **UNTESTED here.** The
  `126` game does contain a Sanguine Bond / Exquisite Blood chain, but it is the OPPONENT's loop,
  which this counter is not about; the seat was asked nothing during it.
- **R2a (land seam)** — **PASS.** Every land-drop bracket in this seat refs a land-drop
  predecessor; 0 cast/land cross-seam refs in 294.
- **R2b (first window)** — **PASS.** 18 `this is the first window I have asked you at this seam`
  sentences, all with `ref = -1` and no earlier same-seam record. 0 comparisons at streak 1.
- **R2c (the count)** — **PASS at 318/323**, residual in HIGH-1. CR's refutation independently
  confirmed on `152` seq 30 -> 31.
- **R3 (life-gain-on-sacrifice / converter)** — **PASS on the one instance.** `126` seq 36 names
  the event on a body row under a live Sanguine Bond: `{their converter: this body has toughness 5
  and they control 1 life-to-damage converter (Sanguine Bond) - any effect of theirs that gains
  them life equal to its toughness (a sacrifice-for-toughness edict, a drain of that size) takes 5
  off YOU: life 14 -> 9}`, plus a `LOOP HALF PENDING:` header naming the missing half in hand.
  No reasoning in this seat reads it as a mana cost. 0 edicts, 0 own-converter instances: the
  other branches are UNTESTED here.
- **R4 (blockers AT BEST folds the metered gain)** — **UNTESTED.** One blockers window in the seat
  (`146` seq 46) and it is a NOT-lethal header with no AT BEST figure and no lifegain meter.
- **R5 (creature gloss)** — **PASS**, residual in LOW-1; size in MED-2.
- **R6a (crack-back cover on targeted removal)** — **PASS**, residual in MED-4. Regenerating and
  returning bodies: `146` seq 60's roster includes `Triumphant Adventurer` (a ghostform body) among
  the legal targets and does NOT count it in the "most any ONE takes off" figure — CU F3 behaving.
- **R6b (X sub-menu cover)** — **PASS.** `152` seq 53's X rows carry
  `{crack-back cover: the CRACK-BACK NEXT TURN line above is 21 from 6 of their creatures ... This
  row REMOVES 4 of those 6 attackers ... takes 8 off the total, leaving 13 -> you would be at -6}`,
  with the FLOOR caveat stated; arithmetic true.
- **R6c (attackers stay-home line)** — **UNTESTED.** 8 attackers windows, none under a crack-back
  the seat could have covered by staying home.
- **R7 (legality-filtered blocker counts)** — **PASS on the cases present.** `146` seq 56
  `They have 0 untapped creatures able to block`, with a correct `[held back, THIS creature could
  not block Spirit Cai (flying)]` on A1 and the accompanying paragraph distinguishing the two tag
  directions; `ATTACK TOTAL: 1 attacker listed, 1 total combat damage ... At least 1 damage lands
  whatever they block` — true.
- **R8 (Q13 board clause)** — `crossphase_identical_reputs` **0** in all six games: **UNTESTED.**
- **R9 (`plan_names_uncastable_zone_card`)** — **PASS.** 5 in the seat (wave 76: 4), and the
  restriction to cast-intent plans is holding: `125` seq 89 `PLAN: Return Hammer to hand, cast
  Siege-Gang Commander` (both copies in the graveyard) and `125` seq 205 `PLAN: cast Rorix
  Bladewing in main phase 1` (both copies countered into the graveyard) are TRUE positives, and the
  wave-76 residual shape — a future-turn clause counted as a cast intent — appears once at `125`
  seq 185. `plan_names_stranded_card` 0.
- **R10 (X-announce plan echo + neutral example)** — **PASS, both halves.** `152` seq 53 carries
  `THE PLAN YOU LAST STATED (as you stated it, 2 windows ago on turn 16) reads "Cast Starstorm at
  X=2 ..."` with the `(that plan was written on the CAST row, one screen before this menu's per-X
  kill lists existed ...)` note, and closes `e.g. "CHOICE: <n> (X = <n>)" (a worked example of the
  FORMAT only - <n> is a placeholder, not a recommendation ...)`. The wave-76 failure is closed.
- **R11 (harvest by manifest)** — **PASS from my side:** each of my six games has exactly 2 seat
  logs in the pool dir, and the consolidated corpus holds 42 logs for 21 games. Same-arm defer
  never fired (0/0/0/0).
- **CU F1 (balanced-group strip on the ask key)** — I found **0** windows in this seat re-asked
  with rows differing only inside `{...}`/`[...]`: after the strip, every re-ask compared
  byte-identical or genuinely different. The CG shape is not present here.
- **CU F5 (wall-miss does not consume the seam sequence)** — **UNTESTED**, 0 wall misses.
- **{X}-announcement payment loss** — `/usr/bin/grep -c 'dropped after its X was announced'`
  returns **0** in all six of my stderr files.
- **P14 / BS F3** — `declined_face_latches` 0, `repeat_count_reask` 0 in this seat.

### Card facts — verified, no item
Fate Unraveler (triggers on the OPPONENT's draws, 3/4), Howling Mine, Master of the Feast (5/5
flying, `each opponent draws a card` on THEIR upkeep — the gloss's THEIR-seat warning is correct),
Shield Sphere, Sanguine Bond and Exquisite Blood (the converter header states the direction
correctly), Sorin Lord of Innistrad's three abilities and lifelink Vampires, Perimeter Captain,
Overgrown Battlement, Wall of Omens, Triumphant Adventurer (deathtouch + own-turn first strike +
the ghostform counter gloss), Barrowin, Kaya the Inexorable, Hive of the Eye Tyrant's creature-land
note, Ranger Class levels, Brutal Cathar / Moonrage Brute daybound flip and Ward, Tovolar's
Huntmaster / Packleader. Own side: Hammer of Bogardan (3 damage, upkeep-only return), Pyrite
Spellbomb's two modes and their two costs, Starstorm, Siege-Gang Commander, Rorix, Dwarven
Blastminer's nonbasic-only restriction, Molten Rain's 2 to a nonbasic's controller, Stone Rain,
Lay Waste, Spark Spray, Forgotten Cave, Talisman of Impulse's 1 damage. No Scryfall disagreement
worth reporting.

---

## (3) Guide verdict: **EDIT** — revised guide at `wave77/deck130/strategy.txt`

**19,841 bytes vs the live 19,929 — net -88**, four edits (+~840) paid by eleven trims (-~930).
Every trim removes a sentence the engine now prints itself or that an edit above absorbs; no
addition is caution, each is a precondition rather than a concession (A334); the guide carries no
citation, count or history (A332); headings unchanged and still scope declarations (A333).

**Edit A — the Hammer return's gate moves IN FRONT of the rule it gates, and keys on the trend's
signed total.**
Before (one paragraph, the gate last): `... and with a body in hand, pass. THE EMPTY-HAND RULE wins
games: when "Your hand" reads "(0 cards): (none)" ... TAKE THE RETURN EVERY UPKEEP and recast in
main phase 1. THE RETURN IS OFF while "Opponent life trend:" shows them gaining 3 or more a turn:
eight mana for 3 against a board that gains 5 is not a clock - spend the turn deploying.`
After: `... and with a body in hand, pass.` / `BEFORE EVERY RETURN READ "Opponent life trend:" AND
WRITE ITS SIGNED TOTAL. That total is what your damage has NETTED against their gains across those
turns, and it is the only figure that says whether this is a clock. While it is zero or a PLUS the
return is CLOSED, and repeating it does not reopen it: take that mana to a land kill, a Blastminer
activation or a body, and leave Hammer in the graveyard.` / `THE EMPTY-HAND RULE, at a MINUS total:
when "Your hand" reads "(0 cards): (none)" ... TAKE THE RETURN EVERY UPKEEP and recast in main
phase 1.`
Paid by `125` **seqs 89, 93, 98/99, 106/107, 114/115, 121, 158, 166/167, 203**. The reasoning at
seq 106 quotes the headline sentence and never reaches the trailing gate; making the gate a
precondition with its own written output ("write its signed total") is the same repair wave-75
Edit D made for the lifegain gate and wave-76 Edit A made for the land row. The old gate's
threshold ("gaining 3 or more a turn") is replaced by the sign of a figure the line actually
prints, which is what the model can read. **This edit earns the guide's largest bytes this wave
because it is the only rule whose violation ran for 14 turns of one game.**

**Edit B — the SURVIVES rule names the declining row of the menu it is on.**
Before: `... "- SURVIVES (toughness N)" the one you decline; if EVERY row says SURVIVES the answer
one screen earlier was "Cast nothing right now".`
After: `... if EVERY row says SURVIVES the answer is the declining row of the menu in front of you -
"Cast nothing right now" on a casting list, the hold row or "0 (pass)" on a priority list - and the
charge stays loaded for their next 2- or 3-toughness creature.`
Paid by `162` **seq 20**: three SURVIVES rows on a PRIORITY menu, where the old remedy named a row
that does not exist there; every Spellbomb, Blastminer and Siege-Gang activation is offered at that
seam. Paired trim: `NEVER SPEND DAMAGE ON A SURVIVOR`'s tail `Hold it for the next 2- or
3-toughness creature, or for a face row under step 0.` — the clause now lives inside the rule.

**Edit C — the GANG BLOCK count is binding, and the two-on-one prohibition leaves the life branch.**
Before (BLOCKING's second bullet only): ` - N at 5 or below -> ONE Goblin on the attacker with the
largest "deals N", a second only on a DIFFERENT attacker; never two on one unless the row says
"(both die)" for the pair.` (and, in ATTACKING, `BLOCKER TAGS: each parenthesis prices ONE blocker;
GANG BLOCK measures your toughness against the SUM.`)
After (new first lines of COMBAT: BLOCKING): `A "[GANG BLOCK: any N of yours ...]" clause is the
ENGINE'S OWN COUNT AND IT BINDS: assign N to that attacker or assign none. Fewer than N kills
nothing and every body you sent dies anyway; your own first-strike or deathtouch arithmetic does
not overturn that number. Field that full group only when your life minus that attacker's "deals N"
is 5 or less, Goblins first.` (the bullet keeps only its assignment rule; ATTACKING's line becomes
`BLOCKER TAGS: each parenthesis prices ONE blocker.`)
Paid by `146` **seq 46**. The prohibition was reachable only under `N at 5 or below` while the
window that lost the bodies read `you would be at 13`; the deleted ATTACKING clause was also wrong
(a gang block adds POWER, not toughness) and duplicated the header's own definition. The
`use it when your life minus ... is 5 or less` gate is preserved verbatim in meaning, folded into
the same paragraph.

**Edit D — THE LAND PLAN's death test is written per number, not after all three.**
Before: `The plan is DEAD FOR THE GAME - and does not revive because the mana has nothing else to
do - the moment ANY holds: N is 5 or more; L is 8 or less; P is at or above L; "Opponent life:" is
at or below the damage you can point at a player this turn.`
After: `WRITE A VERDICT BESIDE EACH NUMBER AS YOU WRITE IT, never one verdict after all three:
N at 5 or more is DEAD; L at 8 or less is DEAD; P at or above L is DEAD; "Opponent life:" at or
below the damage you can point at a player this turn is DEAD. One DEAD kills the plan FOR THE GAME,
and it does not revive because the mana has nothing else to do.`
Paid by `126` **seq 36**, where the model wrote `Their lands: 6 total ... Land plan is alive` — it
produced the numbers the rule asks for and then produced a verdict the rule forbids, because the
rule asks for three numbers first and one verdict afterwards. Smallest of the four edits: the
thresholds are unchanged and only the order of writing moves.

**Bytes trimmed to pay for the above (-~930):** STARSTORM'S X's `Also do not cast on "even at X=N
NOTHING on the board dies".` (the cast row prints `there is no creature on the battlefield for X to
kill` and `smallest X that kills an OPPONENT creature:` — `123` seq 52, `152` seq 51 — and
`Never a row killing none` stands); `Any X your plan named is older than the menu.` (the X menu now
prints exactly this, `152` seq 53); `A marker saying the sweep takes more of YOURS than of THEIRS
is a REFUSAL, not advice about which X.` (the marked-row sentence two lines below says the same and
the marker itself now reads `[<- best X for this cast: X=2 - most kills at any affordable X that
costs you nothing]`); DRAW PUNISHERS' `PUNISHERS PUNISH DRAWS AND NOTHING ELSE: casting, land
drops, combat and activating are free.` (the rendered paragraph ends `Count that cost before
choosing an OPTIONAL draw (a cycling ability, a draw spell, any extra draw), which are the only
draws a choice of yours controls.` — `162` seq 20); TARGETING step 2's `, and after combat damage a
row can flip` (no instance in three waves); the two clauses folded into Edits B and C above;
WALL CHECK's `A "[defender]" creature cannot attack but BLOCKS normally and is named in the "their
untapped blockers" tag.` (the board line prints `of which N are creatures, M of them without a
restriction against attacking` and the rows print `[defender]` — `126` seq 36);
`RORIX BLADEWING is a 6/5 with FLYING and HASTE` -> `RORIX BLADEWING` (the entry prints
`(6/5) [flying, haste]`); `SIEGE-GANG COMMANDER brings FOUR BODIES (a 2/2 plus three 1/1 Goblins) -
board and blockers.` (the R5 gloss now prints `When Siege-Gang Commander enters, put three 1/1 red
Goblin creature tokens onto the battlefield.` — `146` seq 48); and DWARVEN BLASTMINER's quoted
ability text (the battlefield gloss prints `{2}{R}, {T}: Destroy target nonbasic land. -- Morph {R} ...`), the
nonbasic-only and untapped facts kept.

### Which wave-76 items recur, and what the wave-76 edits decided
- **Edit A (a land row never outranks a kill on the same menu)** — **PASS, 3 of 3.** Exactly three
  menus in the six games offered a land-destruction row and a kill-naming row together — `123`
  seq 11 (Hammer over Lay Waste), `146` seq 60 (Hammer over Stone Rain), `152` seq 17 (Starstorm
  over Molten Rain) — and the kill row was taken at all three. The wave-76 loss shape did not
  recur. `126` seq 36 is not a counter-example: no row on that menu killed anything.
- **Edit B (the Commander is the last body on his own list)** — **PASS**, 4 face sacrifices in
  `123` (seqs 71/72/74/75) and the Commander survived all of them to attack at seq 73; no
  self-sacrifice off a non-lethal count anywhere in the seat.
- **Edit C (the Siege-Gang gate counts attackers, not creatures)** — **PASS on its own case.**
  `123` seqs 74/75 took the face row on a line reading `0 of them without a restriction against
  attacking`; 0 face rows taken while a live attacker stood.
- **Wave-76 HIGH-1 (kill rows get no crack-back arithmetic)** — **CLOSED**, residual in MED-4.
- **Wave-76 HIGH-2 (the hold-check count)** — **largely closed**: 318/323 true here against
  three clean falses in 264 decisions last wave; the residual is a different mechanism (HIGH-1).
- **Wave-76 MED-1 (X menu drops the plan echo, example drawn from row 1)** — **CLOSED**, R10 PASS.
- **Wave-76 MED-2 (`plan_names_uncastable_zone_card` false on future-turn clauses)** — **mostly
  closed**: 4 of 5 this wave are true cast-intent positives.
- **Wave-76 MED-3 (the dropped `PLAN:` label)** — **recurs unchanged**, 37 of 37, zero genuine
  residue for the second straight wave.
- **Wave-76 LOW-2 (`forced_close_unrecorded` unattributable)** — recurs at 3.
- **Wave-75 Edit D (Hammer return off under a lifegain trend)** — **was UNTESTED, is now TESTED and
  FAILED**: §1 vs 125, repaired by Edit A.
- **Wave-75 Edit A (Starstorm clause (c))** — **UNTESTED** again: no Starstorm this wave was taken
  or declined on clause (c)'s board.

### Rejected, though I looked at them
- **A "damage does not carry across turns" rule off `162` seq 20's plan.** The row's own
  `SURVIVES (toughness 5)` verdict already answers the window, and TARGETING step 2's
  `THE ROW'S VERDICT OUTRANKS THE VERB IN YOUR OWN PLAN` already answers the plan. Edit B fixes
  the remedy; a fourth sentence about damage wearing off would be unpaid noise.
- **A decking rule off the `125` game.** `Your library: N cards` is printed on every window and the
  seat took optional draws at `125` seqs 51, 150, 157 — but the game was already lost on the clock,
  the draws were not the loss, and the deck has no rule shape that would have changed a turn.
  Unpaid.
- **A "do not cast at your own upkeep" tightening off `152` seqs 51–53.** The Starstorm there was
  a sweep killing four creatures on the turn, which is exactly the carve-out YOUR OWN UPKEEP
  already names. The rule fired correctly.
- **A counter-bait tightening off `125` seq 48.** THE BAIT EXPIRES is already correct; the seat
  held one card and had nothing to bait with.

---

## (4) Optional proposals
**None.** The three generalisable items are engine work and are stated as engine items above:
§2 HIGH-1's handle-stable hold key, §2 HIGH-2's `hold_check_ref_seq` naming, and §2 MED-1's
separation of their gain from the net in `Opponent life trend:` — that last one is the surface
several decks' lifegain rules key on, so it is worth the engine seat's attention beyond this deck.
Nothing this seat saw contradicts an existing amendment or needs a new one.

## What I did NOT check
- **The opponent seats' JSONL.** Board state and their plays were read from my own rendered prompts
  and the shared stderr; I opened no opponent translog. Two of my six games do contain an
  opponent-seat fallback (`125v130` and `152v130` match the corpus grep), which I did not open —
  the engine seat has those.
- **Any code.** Every claim about a counter, a skipped window, an unfired instrument or the
  decoding of `hold_check_ref_seq` is a transcript/stderr observation; the decoding is inferred
  from 294 consistent resolutions, not read from the source.
- **Whether `own_loop_windows_asked` 0, `crossphase_identical_reputs` 0, `blocker_forecast_*` 0
  and `force_close_arms_*` 0 are unwired counters or honest zeroes.** No instance exists in this
  seat either way.
- **The `askreplay/` sidecar** and the replay path behind `ask_replays_reserved` 32 /
  `identical_ask_answers_reserved` 24.
- **The 10 `async_drops` individually.** 0 `phase2_answer_missing` and 0 fallbacks are consistent
  with none lost, unconfirmed.
- **R4, R6c, R8, CU F5, the edict and own-converter branches of R3** — no instance in this seat;
  UNTESTED, not PASS.
- **Whether 1/6 is a real strength change.** Three of the five losses are boards this deck has no
  card for (a 5/5 flier behind a draw punisher, a Sanguine Bond combo, a werewolf board at 21
  power); I priced two losses to decisions and made no matchup- or field-level accounting.
