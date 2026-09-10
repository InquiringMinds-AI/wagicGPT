# Wave 74 — deck 130 (mono-red land destruction) — per-deck review

Corpus `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260909-104713-final`. **All six of my
games are on the OLDER binary** `archives/wagic-60e9fe2ad-w74step1b` (source `1d1d3744d`, through
CG): every one has `start_epoch` 1788968838–1788968874, well below the 1788998000 rerun cut. None
of the three CH reruns is mine, so everything below about the hold latch is a CG-binary
observation. `--thinking on`. Live guide = the wave-73 edit (19,988 bytes).

**Seat census.** 324 model decisions over 6 games (wave 73: 307), 100% with native reasoning
(median `reasoning_chars` 5,829; min 1,643, max 23,318). **1 fallback** (`unparsed_reply`, vs146
seq 16) and **1 `recovery` record** (vs146 seq 17); the corpus's **only `reply_truncated` is mine**
— vs146 seq 16, reply verbatim `"\n\nPLAN: Hold priority, then activate Talisman for {R}, cast
Siege"`, cut mid-word at 64 chars. 0 `action_before_plan`, 0 `menu_pass_no_progress`, 0
`ask_replays_refused`, 0 `phase2_answer_recovered/missing`, 0 `plan_names_stranded_card`.
14 `plan_line_missing` (4.3%, up from 2.7%) — all 14 also `off_protocol_bytes`>0, and
`off_protocol_bytes`>0 is 14/14 the same records (§2 MED). 37 `async_drops`; 35
`ask_replays_reserved`; **`identical_ask_answers_reserved` 0 in all six**; 23 `hold_windows_skipped`
(cast 12 / priority 11); `hold_released_turn` 6 (125:3, 126:1, 146:1, 152:1);
`main_phase_windows_skipped` **0 in all six**; `own_turn_windows_skipped` 14 (123:8, 126:6);
`chain_acting_rows` 76 / `chain_selfharm_rows` 58 / `chain_windows_only_selfharm` **0**;
`sibling_window_asks_skipped`, `stop_reached_windows_skipped`, `chain_windows_collapsed`,
`reserve_decline_*`, `repeat_annotated_takes`(2 in vs146) as before.

---

## (1) Game-by-game, and the deciding decisions

| Opponent | Result | Turn | Final life | Replies |
|---|---|---|---|---|
| 126 | **WIN** | 20 | 20 / 0 | 60 |
| 162 | LOSS | 15 | -3 / **2** | 44 |
| 125 | LOSS | 49 | 0 / **59** | 96 |
| 146 | LOSS | 29 | 0 / 11 | 64 |
| 152 | LOSS | 21 | 0 / **20** | 42 |
| 123 | LOSS | 9 | -4 / 16 | 17 |

**1/6, down from 3/6. My read: roughly one third variance, one third matchup, one third play — and
the play third is two decisions, both traceable to the same structural hole in the guide.** Two
losses (123, 162) turn on a single decision each where the printed row that wins the game was on
the menu and was passed; those are the paid decisions and they buy Edits A and B. vs152 is
variance outright. vs125 is matchup plus one guide-shaped miss. vs146 is matchup with one wasted
Goblin. Deck 130 won 3 of 6 last wave against a partly different field; a 6-game swing of two wins
on this evidence is not by itself a guide verdict, which is why I priced only the two decisions
the transcripts actually show going wrong.

### vs 123 — LOSS on turn 9. **The single most expensive decision in the seat**
`1788968857-...-vs-ai_baka_deck123.jsonl` **seq 12** (turn 6, Upkeep, priority menu, my life 20,
opp 19, 2 mana, Pyrite Spellbomb on my battlefield since turn 4). Row 1 verbatim:

> `1. Deal 2 damage with Pyrite Spellbomb targeting Thraben Doomsayer [opponent's battlefield] {right now: takes 2 damage - DIES} [cost: {r}, Sacrifice] {spends 1 of your 2 untapped mana sources this turn; Dwarven Blastminer {1}{r} in your hand needs 2}`

The reply was `CHOICE: 0 (pass)`. The plan line was the carried land-destruction plan, unchanged
across seqs 9/10/12. Turn 7 the opponent cast Intruder Alarm and the Doomsayer made 22 Human
tokens; turn 8 the seat swept them with Starstorm X=1 (correct — the cast row's own
`[<- board sweep: THEIRS 22 / YOURS 0]` marked it) and the Doomsayer immediately made 22 more; by
**seq 19** the CRACK-BACK line read `23 of their creatures ... for up to 24 - you would be at -4;
that would KILL you - you control NO creature`. Dead on turn 9.

This is **STRATEGY, and the guide caused it.** The guide names this exact card: ORDER OF OPERATIONS
entry 2 says a token-MAKER "(Thraben Doomsayer 2/2 ...) is what this entry exists for", and
TARGETING step 2 says `- DIES` is the row you take. But both of those sections are consulted at a
*casting* window, and a Spellbomb already on the battlefield is **never offered on a casting
menu** — its only window is the upkeep priority menu, whose section (`YOUR OWN UPKEEP`) carries a
closed whitelist of exactly three rows ("a Blastminer land-kill, the Hammer return under
EMPTY-HAND, or a Starstorm that kills a creature THIS turn") and orders `CHOICE: 0 (pass)`
otherwise. The guide told it to pass the game-winning row. Edits B and B2. I checked: the seat got
no further window that turn (records go seq 15 → seq 16, turn 8), so seq 12 was the only chance.

### vs 162 — LOSS at opponent 2, holding exact lethal
`...-vs-ai_baka_deck162.jsonl` **turn 14, seq 44 → 45**. My life 3, **opp 12**, 7 untapped
Mountains, board = Rorix Bladewing (6/5 flying haste, and at seq 49 its attack row reads
`[no creature they control can block this attacker]`), Siege-Gang Commander #1, 3 Goblins,
Blastminer. **Rorix 6 unblocked + three Goblin sacrifices at {1}{R} (6 of 7 mana) for 6 = 12 =
exactly lethal.** At **seq 44** the model wrote that line out itself:

> `PLAN: Sacrifice three Goblins to Siege-Gang Commander for 6 damage, then attack with Rorix and Siege-Gang Commander for lethal.` / `CHOICE: 9 (Cast nothing right now)`

— correct, since a sacrifice is a priority action, not a cast. Then the *same phase re-asked the
casting question*, and at **seq 45** it cast a second Siege-Gang Commander for 5 of the 7 mana
(`PLAN: ... generate blockers and additional attackers, survive the crack-back`). That left 2 mana
= one sacrifice. At **seq 46** it had already conceded: `accepting that I cannot reach lethal`.
seq 48 = one sac (opp 10), seq 49 = swing all five, opponent finished the turn at 2 and killed it
next turn. PERCEPTION is clean — every number it needed was printed.

STRATEGY, and again structural: **LETHAL FIRST lives only in `=== TARGETING DAMAGE ===`, which the
model reads at a target menu.** ORDER OF OPERATIONS, the list it runs at a casting ask, has no
lethal gate at all, and its entry 4 ("SIEGE-GANG at five") matched. Two loud render clauses pushed
the other way: `CRACK-BACK NEXT TURN ... that would KILL you`, and the row-1 `{crack-back cover:
... you cover 5 of 5 ... which you SURVIVE}`. Edit A.

### vs 125 — LOSS on turn 49, opponent at 59. **THEY COUNTER is finally TESTED, and it failed**
Three waves of "UNTESTED" end here: this game's log carries **eight** `was COUNTERED by` lines
(Molten Rain/Dream Fracture, Siege-Gang/Essence Scatter, Stone Rain/Dream Fracture,
Rorix/Essence Scatter, Siege-Gang/Essence Scatter, Blastminer/Essence Scatter, Hammer/Cancel x2),
against Elixir of Immortality recursion that took the opponent 20 → 62.
The paid window is **seq 61** (turn 20, Main 1): hand = Mountain + Rorix + Siege-Gang,
`Their untapped sources: 5`, `Opponent hand size: 6`, and the log had **already** printed
`was COUNTERED by` four times — all three of the guide's THEY COUNTER predicates true. It cast
Siege-Gang (5) before Rorix (6), which is the guide's bait order and **correct**; Essence Scatter
took it; and then **the threat never followed**. The turn's log goes straight from the counter to
`Phase: Attackers` (see §2 HIGH #2). Earlier, **seq 58** (turn 18) cast Rorix as the turn's first
spell into 3 open sources with the log already showing two counters, but its hand was three
Mountains and two Rorix — no bait existed, and the guide's one-turn hold cap makes that legal.
Edit E. The rest is matchup: from turn 26 on, the guide's own floor (`when "Opponent life trend"
shows them gaining 3 or more a turn that return is not a clock`) was in force and the seat still
returned and recast Hammer at seqs 92/93, 97/98, 104/107 against +5/+9/+10/+12 swings. That is a
guide rule read past, but the game was unwinnable by then; §2 LOW.

### vs 152 — LOSS. **Variance, and I am confident it is variance**
The seat controlled **zero creatures for all 21 turns** and has **zero attackers records**. Its
hand at turn 16 (seq 36) was `Lay Waste; Lay Waste; Stone Rain; Forgotten Cave`; at turn 18
(seq 40) `Lay Waste; Lay Waste; Molten Rain`; at turn 20 (seq 45) `Molten Rain` alone. It never
drew a body. The one guide violation is **seq 45**: the LAND PLAN's numbers were "their lands 3;
my life 9; their power ≥9" — P at or above L kills the plan for the game, and it cast Molten Rain
anyway. But its hand held exactly that one card, so the guide's prescribed answer ("Cast nothing
right now") changes nothing. **No edit is paid by this game.**

### vs 146 — LOSS. Matchup, plus one Goblin thrown away at 1 life
Kaya the Inexorable, Silverquill Silencer and a dungeon engine; the seat had 0 creatures from turn
18 to turn 28. **seq 69** (turn 28, my life 1, opp 15) is the miss: the same priority menu printed

> `5. Deal 2 damage with Siege-Gang Commander targeting Silverquill Silencer [opponent's battlefield] {right now: takes 2 damage - DIES} {removes 3 from the CRACK-BACK total above: 3 -> 0 - you would be at 1}`

and it took row 6, the face, for 2 of the 15 it needed. It took the correct row one window later
(**seq 70**, where the render repeated the clause), then spent the last Goblin at the face
(**seq 71**). The guide already covers seq 69 (TARGETING step 2, "`- DIES` is the row you take") —
this is the rule failing, not a rule missing, so I priced no edit. **seq 54** (2 Goblins sent at
Kaya rather than the face) is guide-correct: TARGETING step 1 puts a planeswalker ahead of the
face, and 2 damage removed both of Kaya's 2 loyalty.

### vs 126 — WIN, and one real misplay inside it
The Perimeter Captain / Overgrown Battlement wall deck. The **WALL CHECK fired correctly at
seq 62** (`ATTACK: A4`, Rorix alone over two 0/4 defenders, `Siege-Gang and Goblins stay home to
avoid triggering the defenders' life gain`), and Rorix flew over for the win. But at **seq 64**
(turn 18, Upkeep) the Starstorm cast row printed
`[<- board sweep: THEIRS 2 / YOURS 4 - the only row on this menu that prices a board sweep (it
takes more of YOURS than of THEIRS)]` — which the wave-73 Edit B declares a REFUSAL — and it cast
anyway; then at **seq 66** it took `CHOICE: 1 (X = 1)`, a row whose own text reads
`{X pricing: kills THEIRS: none; YOURS: Goblin x2, Dwarven Blastminer}`. It killed three of its
own creatures and none of theirs, for 3 of the 7 mana it had already announced. Two causes, one of
them a guide bug: **the guide's `ON THE X MENU: option 1 is the LARGEST X, never X=1` is now
FALSE** — O19 made the menu ascending and index = X — so the stale sentence actively endorsed the
row it took. Edit C.

---

## (2) Engine / interface / card items

### HIGH — a `{crack-back cover:}` gloss that counts a LEGENDARY duplicate as "+1 body", on a row that prints the legend rule two brackets earlier
`...-vs-ai_baka_deck162.jsonl` **seq 44/45**, row 1, both brackets in the same row:

> `1. Cast Rorix Bladewing {3}{r}{r}{r} (6/5) [legendary: you already control Rorix Bladewing - legend rule: casting this sends one copy to your graveyard (you choose which)] ... {crack-back cover: the CRACK-BACK NEXT TURN line above is 5 from 1 of their creatures and puts you at -2. This adds 1 body ... Counting only the checked bodies you cover 5 of 5, leaving 0 -> you would be at 3, which you SURVIVE - and more blockers can only lower that, so nothing uncounted here overturns it.}`

Casting the second Rorix under the legend rule adds **zero** net bodies — one of the two is put
into the graveyard immediately. The gloss's "adds 1 body", its "you cover 5 of 5", its
"you would be at 3, which you SURVIVE" and its closing "nothing uncounted here overturns it" are
all false on this board, and the row states the reason for their falsity itself. This is the
render-clause-states-a-false-fact class the engine seat is tracking. Fix shape: the crack-back-cover
body count must subtract a legend-rule duplicate (and, generally, any body the row's own text says
leaves the battlefield on resolution) before it computes cover. The seat did not take this row, so
it cost nothing here — but the guide can no more defend against a false SURVIVE than against a
false kill count.

### HIGH — after a spell is COUNTERED, the main phase does not re-ask the Casting decision, and `main_phase_windows_skipped` counts 0
`...-vs-ai_baka_deck125.jsonl` **seq 61 → 62**, turn 20. The seat cast Siege-Gang Commander with 11
sources; the log reads

> `- Paid {3}{r}{r} for Siege-Gang Commander (5 sources)` / `- You cast Siege-Gang Commander` / `- Opponent cast Essence Scatter` / `- Your Siege-Gang Commander was COUNTERED by Essence Scatter and went to your graveyard` / `- Opponent's Essence Scatter resolved and went to the opponent's graveyard` / `- Phase: Attackers`

Rorix Bladewing was still in hand, marked `[castable now]` at seq 61, and 6 sources were still
untapped — no window was offered, and **`main_phase_windows_skipped` reads 0 for this game**, so
the instrument does not see it either. Contrast **seq 64 → 65** on turn 22, where a cast that
*resolved* was followed by a second Casting decision on the same phase, and **seq 24 → 26** on
turn 12, where a Stone Rain counter *was* followed by a re-ask. So the behaviour is not uniform
and I cannot tell from the transcript alone whether turn 20's window was never opened or was
suppressed uncounted; no hold was taken on turn 20 and a hold from turn 19 would have been
released at that untap. This is the window the guide's whole bait-then-threat line depends on.
Engine seat's call; I did not read the code.

### MED — `plan_line_missing` is a DROPPED `PLAN:` LABEL, 14 of 14, fourth wave unchanged
Every one writes a correct plan sentence and omits the label, so the sentence scores as
`off_protocol_bytes` and the carried plan silently reverts to the previous window's. The counts
coincide exactly: 14 `plan_line_missing`, 14 records with `off_protocol_bytes`>0, same records,
and each record's `off_protocol_bytes` equals the length of the dropped sentence. Repro:
`...-vs-ai_baka_deck152.jsonl` **seq 40**, reply verbatim
`"\n\nCast Lay Waste targeting Hengegate Pathway #2.\nCHOICE: 1 (Cast Lay Waste)"`
(`off_protocol_bytes: 46`); and **seq 44**, `"\n\nMain phase 1: Cast Molten Rain targeting
Hengegate Pathway #2. Attack with nothing.\nCHOICE: 1 (Play Mountain)"` (83). All 14 parsed to the
right action; **none is a genuine missing plan.** Accepting an unlabelled first line as the plan
zeroes both counters at once — the same one-token fix reported in wave 73, and my seat's rate went
up (2.7% → 4.3%) rather than down. Corpus-wide that is 93 records.

### MED — the reply that truncated is the corpus's only one, and it truncated at 64 characters
`...-vs-ai_baka_deck146.jsonl` **seq 16**, verbatim:
`"\n\nPLAN: Hold priority, then activate Talisman for {R}, cast Siege"`. It scored `unparsed_reply`
and produced the seat's one `recovery` record (**seq 17**, empty reply), after which **seq 18**
re-asked the same window and answered cleanly. So the retry path worked; the interesting part is
the cut point — 64 characters is not a token budget or a deadline shape, and the record carries
`reasoning_chars` in the normal band. Worth one look from the engine seat at whether a short
transport read can return a partial body as a complete one.

### LOW — the hold row is taken and re-opened almost immediately, 89% of the time, on the CG binary
Across my six games' stderr: **104 `took the hold row`, 93 `hold re-opened ... a printed row
changed or is newly available`, 7 releases at untap.** Per game (125 / 126 / 162 / 146 / 152 /
123): 42/35, 21/19, 14/14, 13/12, 8/7, 6/6. Runs of take-reopen-take-reopen inside one turn are
routine (`game-125v130-1788968835.stderr` lines 410–422, turn 4: five holds and five re-opens in
sequence).
**On O6 specifically: `rows above are new` appears ZERO times in all six of my stderr files** —
the wave-73 bracket I diffed pairwise last wave does not render at all this wave, so the specific
defect I reported (a projected-life reprice counted as "4 rows are new") is not observable here and
I cannot say it recurred. What I *can* say is that O6's normalisation **did not fix the re-open
problem in this seat**: the hold still re-opens on 89% of takes, just now for reasons the bracket
no longer names. The wave-73 HIGH's *consequence* did clear, though: my worst (turn, phase) this
wave is **7 asks** (vs146 turn 16, Main 1), and no phase in the seat exceeds 10 — the 7- and
20-window chains of waves 72–73 did not recur, and neither the WINDOW LOOP nor the PILOT STALL
tripwire fired in any of my six games. Whether that is O6, CG, or a softer field, I cannot separate.

### LOW — `{X}` announcement payment loss: the instrument is ALIVE, and both fires are the opponent's
`/usr/bin/grep -c 'dropped after its X was announced'` over my six stderr files returns **2**, both
in `game-125v130-1788968835.stderr` (lines 1699 and 2729) and both **deck125's** Sphinx's
Revelation, not mine — `7 mana already paid stays floating and is lost when the step ends` and
`14 mana ...`. Wave 72 and wave 73 both read 0 corpus-wide with an abandonment observed, so the
docket item "one abandonment observed, instrument silent" is now **closed: the instrument fires**.
Note the shape of both, for O19: the log line above each reads `... option 5 is X = 0 ... Reply
with the OPTION number: -> chose 6 of 6` and `-> chose 13 of 13` — the deck125 seat took the
ASCENDING menu's last row (Decline) twice, at a cost of 21 mana. Deck 125's seat, not mine.

### LOW — the Hammer floor was applied against a runaway lifegain board the guide already forbids
`...-vs-ai_baka_deck125.jsonl` seqs 92/93, 97/98, 104/107: three consecutive upkeep Hammer returns
plus recasts for 3 damage each, against an `Opponent life trend` moving +5 / +9 / +10 / +12 a turn
(Elixir of Immortality). The guide's PLAN LINES floor says exactly this is not a clock. No edit —
the game was already unrecoverable and no alternative row existed on those menus; I record it as
evidence that a floor phrased as "when nothing else matches" will be taken even where its own
exception applies.

### Card facts — verified, no item
`Siege-Gang Commander`, `Starstorm` ({X}{R}{R}, cycling {3}), `Rorix Bladewing` (legendary 6/5
flying haste), `Pyrite Spellbomb`, `Molten Rain`, `Stone Rain`, `Lay Waste`, `Forgotten Cave`,
`Talisman of Impulse`, `Dwarven Blastminer` all rendered consistently with their real Oracle text
across the seat, including Silverquill Silencer's toughness 2 against the Spellbomb/Siege-Gang
2-damage rows and Shield Sphere's `(printed 0/6)` with a `-0/-1` counter. No Scryfall disagreement
worth reporting.

### Verifying last wave's two flagged items
- **wave-73 HIGH (the hold key ignoring the projected-life carve-out):** see §2 LOW above. Built as
  CD O6, replaced by lane CH on a binary none of my games ran. On the CG binary the specific
  bracket string is gone and the multi-window chains it produced are gone with it, but the
  take-then-reopen loop is not.
- **wave-73 MED (`(keeping N)` missing on the fresh-7 window):** **BUILT AND VERIFIED PASS.** The
  header now reads `You have a fresh 7-card opening hand (keeping 7).` in all six games (repro:
  `...-vs-ai_baka_deck125.jsonl` **seq 2**). This has a side effect on my own guide — see Edit D:
  wave-73's Edit E re-keyed MULLIGAN onto the *mulligan row's* number, which is the NEXT hand's
  size (6 on a fresh 7), one lower than the hand being decided. It did not bite (all six kept at 7,
  none on a marginal count) but it is a live off-by-one and Edit D reverses it.

---

## (3) Guide verdict: **EDIT** — revised guide at `wave74/deck130/strategy.txt`

**19,978 bytes vs the live 19,988 — net −10** (≈1,110 added across five edits, ≈1,120 trimmed
across twelve). **The bytes are earned by Edits A and B**, which are the only two rules in the file
that would have changed a decision this corpus shows losing a game outright, and they are the same
defect twice: *the guide's two "take the winning row" rules both live in sections the model reads
at a CASTING window, and both decisions were offered at a PRIORITY window or lost at a second
casting window.* Both are stated as arithmetic the render already prints, not as caution.

**Edit A — ORDER OF OPERATIONS gains a lethal entry 0, ahead of everything.**
Inserted before entry 1:
> ` 0. LETHAL, counted HERE and not one screen later. Add: the damage of every attacker tagged "no creature they control can block this attacker", plus 2 for each Goblin sacrifice your "Mana available:" pays for at {1}{R}, plus every damage row you can point at a player. If that total reaches "Opponent life:", cast NOTHING that spends the mana those sacrifices need - answer "Cast nothing right now", pay them at the priority menu, and swing. A second body is not worth a lethal you already have.`

Paid by `...-vs-ai_baka_deck162.jsonl` **seq 44 → 45**. LETHAL FIRST existed, in TARGETING DAMAGE,
and the model had already computed the sum in its own plan one window earlier; ORDER OF OPERATIONS
— the list it runs at a casting ask — had no lethal gate, and its entry 4 fired.

**Edit B — the upkeep whitelist admits a `- DIES` damage row, and Edit B2 names why the window
matters.** Before: `Answer both windows CHOICE: 0 (pass) or the hold row, unless BOTH hold: the row
is a Blastminer land-kill, the Hammer return under EMPTY-HAND, or a Starstorm that kills a creature
THIS turn, AND ...`. After, the same sentence with `, or a damage row reading "- DIES" against a
token-MAKER, a punisher, or a creature the CRACK-BACK line says would kill you` added to the list,
prefaced by **B2**:
> `AN ABILITY ALREADY ON YOUR BATTLEFIELD - Spellbomb, Siege-Gang sacrifice, Blastminer - IS OFFERED ONLY AT A PRIORITY MENU, never on a "which card do you cast" list. Passing one is not deferring it: their board moves before your next priority window.`

Paid by `...-vs-ai_baka_deck123.jsonl` **seq 12**. B2 is the load-bearing half: the old whitelist
was not merely short, it was written as though the upkeep priority menu offered nothing the rest of
the guide had already ruled on.

**Edit C — `ON THE X MENU` corrected for O19.** Before: `option 1 is the LARGEST X, never X=1, and
any X your plan named is older than the menu.` After: `THE OPTION NUMBER IS THE X - option 4
announces X=4; option 1 is X=1, the SMALLEST row there is. Any X your plan named is older than the
menu.` Paid by `...-vs-ai_baka_deck126.jsonl` **seq 66**. The old sentence was true of the
descending menu and is now exactly inverted; it endorsed the row that killed three of the seat's
own creatures and none of the opponent's.

**Edit D — MULLIGAN re-keyed to `(keeping N)`, now that O16 prints it.** Before: `KEEP SIZE N is the
number the mulligan row itself names ("a keep after this one would keep N cards"); there may be no
"(keeping N)" text anywhere, so use that row.` After: `KEEP SIZE N is the "(keeping N)" printed on
the header line - the hand in front of you. The mulligan row's own "would keep N cards" is the NEXT
hand's size, one lower: never read N off it.` Paid by **seq 2** in all six games (§2, verification).

**Edit E — THEY COUNTER gains the follow-through.** Before: `THE BAIT EXPIRES - once a spell of
yours has resolved or been countered this turn the body is your NEXT cast, and you hold a body for
AT MOST ONE of your own turns.` After: `... once a spell of yours has resolved OR BEEN COUNTERED
this turn, the body is your NEXT cast: take the casting ask again on that same turn and cast it,
and if no further ask comes, cast it FIRST next turn. You hold a body for AT MOST ONE of your own
turns.` Paid by `...-vs-ai_baka_deck125.jsonl` **seq 61**. The bait order was already right and was
followed; what was missing was what to do after the bait is eaten. The clause is written so it
still instructs when the engine does not re-ask (§2 HIGH #2).

**Bytes trimmed to pay for the above** (≈1,120): the GANG BLOCK bullets compressed
(`blocker_forecast_gang` **0 in all six games, third wave running**); the whole `=== SITUATIONS ===`
section (bullet 1 restated STARSTORM'S X; bullet 2's 10-creature board is covered by THE LAND
PLAN's dead rule and the SURVIVAL OVERRIDE); the intro's colour sentence (mono-red, no colour
refusal has ever occurred); the ATTACK FLOOR's summoning-sickness/tapped restatement (the attack
list is pre-filtered and the board line now prints `[summoning sick - cannot attack this turn, but
CAN block]`; 0 violations in 14 attacker records over two waves); THE LAND PLAN's own
"a plan line you wrote last turn never revives a dead rule" (subsumed by the general rule at the
head of PLAN LINES since wave 73); `HAMMER, SPELLBOMB AND SPARK SPRAY NEVER TOUCH A LAND` (0
violations, two waves); TARGETING step 0's duplicated Goblin arithmetic (now in Edit A); and prose
tightening in SPARE MANA, DRAW PUNISHERS, HAMMER, RACE CHECK, BOTTOMING, THE 6-LIFE FLOOR and
CLEANUP DISCARD.

### Rejected, though I looked hard at them
- **A land-plan tightening off vs152.** The plan was dead by its own numbers at seq 45 and the seat
  cast anyway — but its hand was that one card, so the rule's prescribed answer changes nothing.
  Not paid.
- **A "do not spend your last blockers" clause off vs146 seq 69/71.** The existing TARGETING step 2
  already names the right row and the render printed `{removes 3 from the CRACK-BACK total above:
  3 -> 0}`; the seat took that row one window later. This is a rule failing to fire, not a rule
  missing, and a new rule would not obviously fire either.
- **A hard "never cast into open counter mana" rule off vs125 seq 58.** With two copies of the same
  6-drop and three Mountains in hand there was no bait to lead with, and the guide's one-turn hold
  cap already forbids sitting on it. Variance, not policy.

---

## (4) Optional proposals
**None.** The two items worth generalising this wave are engine work (the legend-rule
crack-back-cover arithmetic; the missing post-counter casting window), not general-guide or
strategy-skill text, and both are already stated as engine items in §2.

## What I did NOT check
- **The opponent seats' JSONL.** I read opponent board state and their counterspells out of my own
  rendered prompts and out of `game-125v130-1788968835.stderr`; I opened no opponent translog.
- **Any code.** Every claim about a counter's value, a skipped window or an unfired instrument is a
  transcript/stderr observation. In particular I cannot say whether `main_phase_windows_skipped` 0,
  `identical_ask_answers_reserved` 0, `chain_windows_collapsed` 0 or
  `sibling_window_asks_skipped` 0 are unwired counters, unmet predicates, or honest zeroes.
- **The CH binary.** None of my six games ran it, so I have no evidence at all on whether the
  action-identity hold key holds across a chain; everything in §2 LOW is CG-only.
- **The `askreplay/` sidecar** (35 `ask_replays_reserved` in this seat) — engine seat's instrument.
- **The 37 `async_drops`** — I did not trace whether any dropped answer went unrecovered; the seat
  has 0 `phase2_answer_missing` and 0 `ask_replays_refused`, which is consistent with none, but I
  did not confirm it drop by drop.
- **MULLIGAN past the fresh-7 window and BOTTOMING**: six keeps at 7, no mulligan taken anywhere in
  the seat, so those bullets remain untested for a third wave.
- **Whether deck 130's 1/6 is a real strength change.** I priced two decisions; I did not attempt
  any matchup- or field-level accounting, and I would not read a 6-game record either way.
