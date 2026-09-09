# Wave 73 — deck 130 (mono-red land destruction) — per-deck review

Corpus `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260907-225637-final`, binary
`archives/wagic-4b90ed7d7-w73step1` (source `622b10827`), `--thinking on`. My six seat files are
`...ai_baka_deck130-0x<ptr>-vs-...`; two of my games (vs 125, vs 123) are reruns. Live guide = the
wave-72 edit (19,990 bytes).

**Seat census.** 307 model decisions / 295 protocol replies over 6 games (wave 72: 290). **0
fallbacks, 0 truncations, 0 `action_before_plan`, 0 `menu_pass_no_progress`, 0
`ask_replays_refused`, 0 `phase2_answer_recovered`.** 8 `plan_line_missing` (2.7%, down from 4.8%),
all 8 also `off_protocol_bytes` 10-166; 10 `async_drops`; 23 `ask_replays_reserved`; 17
`identical_ask_answers_reserved`; 106 `hold_windows_skipped` (cast 85 / priority 21); 24
`blocker_forecast_rows` (multi 6, **gang 0**, collapsed 0).
Wave-73 instruments in this seat: `hold_released_turn` **8** (126:2, 152:3, 125:1, 123:2);
`own_turn_windows_skipped` **0 in all six** (wave 72: 45); `main_phase_windows_skipped` **7**
(126:3, 152:2, 125:2); `sibling_window_asks_skipped` **0**, `stop_reached_windows_skipped` **0**,
`chain_windows_collapsed` **0**, `reserve_decline_windows_noted/skipped` **0/0** — all six games.
`repeat_annotated_takes` **4 — every one in the corpus is in this seat** (see §2, they all PASSED).

---

## (1) Game-by-game, and the deciding decisions

| Opponent | Result | Turn | Final life | Replies |
|---|---|---|---|---|
| 126 | **WIN** | 25 | 1 / -1 | 74 |
| 125 | **WIN** | 15 | 18 / -1 | 43 |
| 123 | **WIN** | 23 | 23 / -1 | 53 |
| 152 | LOSS | 22 | -4 / 14 | 48 |
| 162 | LOSS | 13 | 0 / 13 | 48 |
| 146 | LOSS | 20 | -3 / 18 | 29 |

3/6, up from 2/6. All three wins ran the guide's own lines end to end and I found no misplay in
them; the three losses each contain exactly one paid decision, and all three are STRATEGY.

### vs 126 — WIN. The Sanguine Bond deck beaten, at 1 life
Reverses last wave's loss to the same list. The clock was Rorix (**seq 64** cast t20, attacks at
**65 / 71 / 85**) plus two Hammer face casts (**seq 73/74**, **seq 83/84**) and the EMPTY-HAND
return at **seq 81** — `Your hand` was down to the Hammer's own line and `Mana available:` 7, which
is exactly the guide's return gate. The WALL CHECK held the board home correctly at **seq 40** and
**seq 55** (`ATTACK: none` with Pride Guardian 0/3 + Wall of Omens on their line). **No misplay.**

### vs 125 — WIN in 15 turns. Land plan then a full board
Three land kills (**seq 16** Molten Rain, **seq 22** Lay Waste, **seq 27** Stone Rain) while the
LAND PLAN's numbers read alive, then Siege-Gang **seq 32** and Rorix **seq 36**, then the full swing
at **seq 37** (`Siege-Gang Commander, Goblin, Goblin, Goblin, Rorix Bladewing`, 21 -> 10). **No
misplay.**

### vs 123 — WIN. LETHAL FIRST and the repeat tag, executed
**seq 34/44/56/57** are four `{1}{R}` face sacrifices, and **seq 44** and **seq 57** are the two
takes that carried `[repeat: activated this turn N times already]`. The guide's "a count, not a
refusal" clause is the only rule in the corpus exercising that instrument and it fired 4/4.
**seq 37** correctly spent a Goblin on Thraben Doomsayer (ORDER entry 2's token-MAKER clause).
**seq 53** (t22, opp at 5, 10 mana, hand = Molten Rain alone) is not a land-plan violation: it was
the only card in hand and its nonbasic clause took them 5 -> 3. **No misplay.**

### vs 152 — LOSS. **The deciding misplay of the seat**: the only body sent into a lone blocker
**seq 43 (turn 20, Attackers, my life 16, opp 9).** Attack list is one row:
`A1. Siege-Gang Commander (2/2) ... [their untapped blockers: Sigarda, Champion of Light (4/4)
(your attacker dies, their blocker lives)]`, and the header adds `That is NOT lethal: they survive
at 7 even with none of them blocked. Their 1 untapped blocker can cover every attacker you could
send, so none of that damage is guaranteed.` The render is complete and correct. The model sent it
(`ATTACK: A1`), Sigarda blocked, the Commander died, and the seat spent the rest of the game with
an empty board and died on turn 23. PERCEPTION is clean — the reasoning quotes the parenthesis
verbatim. This is STRATEGY, and it is a **guide hole**: the ATTACK FLOOR's third case ("Any other
creature listed -> send EVERY creature listed") catches every board where a creature is listed, so
its fourth case ("`ATTACK: none` only when every listed attacker would die and kill nothing") is
**unreachable**, and BLOCKER TAGS' one-blocker bullet ("the parenthesis is the whole story") states
the story without saying what to do with it. Edits A and the BLOCKER TAGS clause.
The reasoning's whole justification was the carried plan: *"the plan says to attack. I'll stick to
the plan"*, three times, and once more as "Is there any reason to change the plan?" — Edit D.
Not misplays in the same game: **seq 39/40/41** (three upkeep Goblin sacrifices, 15 -> 9) look like
a violation of "the Goblin is a body" but Sigarda has flying and the Goblins could never block her;
**seq 45** (upkeep cycle of Forgotten Cave at 6 mana, hand = 2 Talismans) is legal under the
wave-72 subtraction only by accident — with **no creature in hand at all** the subtraction's test
("cannot cast the cheapest creature in your hand") is undefined. Edit F closes that.

### vs 162 — LOSS. Draw-punisher deck again, and 9 mana stranded on the last turn
**seq 51 -> 52 -> 53 (turn 14, Upkeep, my life 4, opp 13).** The CAST row already printed the whole
refusal: `{X pricing: ... At X=7 - kills THEIRS: Fate Unraveler, Fog Bank; YOURS: Siege-Gang
Commander, Goblin x2}` plus `[<- best X for this cast: X=4 - the only X that kills anything of
THEIRS costs you more of YOURS than it takes - THEIRS 2 / YOURS 3]` and `[<- board sweep: THEIRS 2 /
YOURS 3 ...]`. DO NOT CAST clause (a) matched on that row. The model cast anyway (**seq 51**, plan
"Kill Fate Unraveler to survive the mandatory draw step"), took `Cast Card Normally` (**seq 52**),
and then applied clause (a) **at the X menu** (**seq 53**: `PLAN: Preserve Siege-Gang Commander per
the explicit rule against sweeping when it kills win conditions. CHOICE: 9 (Decline)`). The right
end state by the wrong door: the decline row's own text says `declining strands the 9 mana already
paid`, and this was the turn before it died. The guide's first sentence in that section already
says "DECIDE WHETHER AT THE CAST ROW, NOT AT THE X MENU" and was read past — the fix is placement
and naming the cast row's markers, not new policy. Edit B.
Earlier: **seqs 36-42** are 7 consecutive windows on one resolving draw-punisher chain, life 14 ->
8, every one answered `pass` with the reasoning saying the seat has nothing. ENGINE item, §2 HIGH.
The rest of the game is matchup: Underworld Dreams + Dictate of Kruphix + Howling Mine + Fate
Unraveler, and the model refused every optional draw correctly throughout.

### vs 146 — LOSS. The body that was never cast
**seq 19 (turn 10, Main 1, life 20, opp 19, `Mana available:` 6).** Hand = 2 Mountains, **Siege-Gang
Commander `[castable now]`**, Molten Rain. Its own carried plan read "then cast Siege-Gang
Commander". It cast Molten Rain (`{leaves 3 of your 6 untapped mana sources untapped}` — three
short of the Commander's five), and on turn 11 the opponent's Pelakka Predation discarded the
Commander out of its hand. The seat controlled **zero creatures for the entire game** and died on
turn 21. The guide made this choice: ORDER OF OPERATIONS entry 3 (land kill) outranks entry 4 (the
body), and the "take the ask again - more than one spell a turn is allowed" clause silently assumes
the mana covers both. Edit C. (**seq 25**, t16, Stone Rain at 9 mana with an empty hand otherwise,
and **seq 28-30**, the X=3 Starstorm marked `[<- most kills ...]`, are both guide-correct.)
**seq 17** (t8, main 1, cycling Starstorm at 3 mana) is guide-legal — their line read 0 creatures
and no Forgotten Cave was in hand — though the guide says SECOND main phase and this was the first;
LOW, no edit.

---

## (2) Engine / interface / card items

### HIGH — the hold-check's row-identity key ignores the life-total carve-out its own contract prints
`1788839828-ai_baka_deck130-0x561b653ad930-vs-ai_baka_deck162.jsonl`, **turn 12, seqs 36-42** (also
seqs 26-28 on turn 10). The hold row's contract on those same menus says a row *"that differs only
by a LIFE TOTAL it projects - what you or they would be at - because that number moves with the
board and not with the row"* is the SAME row. I diffed the rendered option lists pairwise:

| pair | rendered rows that differ | what differs | hold check printed |
|---|---|---|---|
| 36→37 | 1 | row 8 `you would be at 12` → `11` | `4 rows above are new` |
| 38→39 | 0 | — | `every row above was also on the menu` (correct) |
| 39→40 | 1 | row 8 `at 10` → `9` | `4 rows above are new` |
| 40→41 | 1 | row 8 `at 9` → `8` | `4 rows above are new` |
| 41→42 | 1 | row 8 `at 8` → `7` | `4 rows above are new` |

Two defects in one bracket: (a) the key does **not** apply the projected-life-total carve-out, so a
row that only reprices `Deal 2 damage ... targeting you {you would be at N}` re-opens the hold every
window; (b) the **count is wrong** — it reports 4 when exactly 1 row moved. Consequence in this
seat: 11 windows across two turns on chains the seat could not touch, ~90 s each, and the hold row
provably cannot close the run. This is last wave's HIGH item (20 windows on one chain, same file)
with the mechanism now located. Fix shape: normalise `{right now: ... you would be at N}` /
`{they would be at N}` out of the identity key the same way the pass row's step-name is normalised.

### HIGH — `chain_windows_collapsed` reads 0 because "a legal action exists" is not the right test
Same 7 windows. The collapse predicate is "the seat's OWN mana policy finds no legal action", and
the seat had eight: three Siege-Gang sacrifices at its **own** permanents (`targeting Dwarven
Blastminer [your battlefield]`, `targeting Siege-Gang Commander [your battlefield]`, `targeting you`),
one at a `- SURVIVES (toughness 4)` Fate Unraveler, one at a face 17 life away, and three cycling
rows each tagged `[DRAW PRICE: ... costs you 2 life right now]` under two punishers. Every one is a
row no policy would take, so the menu is *legally* non-empty and *decision-theoretically* empty.
That is why the counter is 0 in all six of my games — the path is reached, the predicate is too
weak. Suggested shape (engine's call): count a row as unavailable for collapse purposes when it is
tagged `[your battlefield]`/`targeting you`, when its only verdict is `- SURVIVES`, or when it is a
draw row whose `[DRAW PRICE]` exceeds a threshold the seat's guide sets.

### MED — `plan_line_missing` is still 100% a DROPPED LABEL, third wave unchanged
8 of 8. The model writes the plan sentence and omits the `PLAN:` label, so the sentence scores as
`off_protocol_bytes` and the carried plan silently reverts. Repro:
`...vs-ai_baka_deck146.jsonl` **seq 25**, reply verbatim `"\n\nDestroy Plains.\nCHOICE: 1 (Cast
Stone Rain)"` (`off_protocol_bytes: 15`). One of the eight echoes the prompt's own carried-plan
heading — `...vs-ai_baka_deck126.jsonl` **seq 47** (`off_protocol_bytes: 166`) — down from 2 of 14
last wave. All 8 parsed to the right action; none is a genuine missing plan. Accepting the prompt's
own heading spelling as the label remains a one-token fix.

### MED — the fresh-7 mulligan window prints no `(keeping N)`, the spelling every deck guide keys on
`...vs-ai_baka_deck152.jsonl` **seq 2** (identical in all six). The rendered rows are
`1. Keep this hand` / `2. Mulligan (a keep after this one would keep 6 cards)`. The live guide's
three MULLIGAN bullets are all keyed on `"(keeping N)"`, which appears nowhere on that window, so
**every one of them was unreachable**. The model fell back to a prior it invented — its reasoning
reads *`According to mulligan rules: "Keep any 7-card hand with 2 to 5 lands and at least one spell
you can cast in the early turns"`*, a sentence that is not in its guide — and happened to be right.
This is a render/guide key mismatch rather than a bug; I re-keyed my guide (Edit E) and flag it
because six other deck guides use the same `(keeping N)` spelling.

### LOW — a hold taken on the upkeep CASTING ask does not cover the upkeep PRIORITY menu
21 of my 36 hold takes are at upkeep and 15 of those are immediately followed by a `priority`
record in the same phase (e.g. `...vs-ai_baka_deck152.jsonl` **seq 18** hold → **seq 19** Blastminer
activation). This is the contract working as written ("a different question is still asked") and
the guide teaches it ("Your own upkeep asks twice"), but the hold row's own headline reads "do not
ask me again", which is false at the very next window. Cheap render fix: on a menu the seat will be
re-asked at a different seam this phase, say so in the hold row's bracket.

### LOW — `{X}` announcement payment loss: **0 fires again, but this seat finally abandoned one**
`/usr/bin/grep -c 'dropped after its X was announced'` over all six of my game stderr files returns
**0**, and 0 across every stderr in the run — same as wave 72. Unlike wave 72 this seat *did*
abandon an announced X (`...vs-ai_baka_deck162.jsonl` seq 52 → 53), and the X menu's decline row
described the outcome correctly (`your sources STAY TAPPED, and the 9 mana stays floating in your
pool ... whatever is still floating when the step ends is lost`). No stderr line fired, so either
the loss is not routed through `MTGRules.cpp:475` or the debug string covers a different path.
Docket item is **one abandonment observed, instrument silent** — engine seat's call.

### Card facts — all verified against the primitive, no item
`Siege-Gang Commander` (mtg.txt:105071), `Starstorm` (112835, Instant, `{X}{R}{R}`, cycling {3}),
`Molten Rain` (75759, `teach(land[-basic]) damage:2 targetcontroller`), `Stone Rain` (113726),
`Rorix Bladewing` (96836, Legendary 6/5 flying haste), `Hammer of Bogardan` (52046,
`autograveyard={2}{R}{R}{R}:moveto(hand) myUpkeepOnly`), `Pyrite Spellbomb` (90168),
`Forgotten Cave` (42914). No Scryfall disagreement worth reporting.

---

## (3) Guide verdict: **EDIT** — revised guide at `wave73/deck130/strategy.txt`

**19,988 bytes vs the live 19,990 — net −2** (≈1,080 added, ≈1,082 trimmed). The rule that earned
the added bytes is **Edit A, the attack refusal**: it is the only rule in the file that would have
changed a decision this corpus shows losing a game outright, and it is stated as a verdict the
render already prints rather than as a caution. Edits B and C are the other two paid decisions;
D, E and F are cheap corrections. Nine trims (listed at the end) pay for all of it.

**Edit A — the ATTACK FLOOR's dead case becomes a live one, ahead of "send everything".**
Before (cases 3 and 4):
`- Any other creature listed -> send EVERY creature listed except Blastminer. ...` /
`- Otherwise "ATTACK: none" only when every listed attacker would die and kill nothing, or under the
crack-back exception.`
After, inserted BEFORE that case:
`- A listed creature whose "their untapped blockers" tag prices it "(your attacker dies, their
blocker lives)" -> that creature STAYS HOME: it is a body traded for nothing. Read this verdict
FIRST, before the line below. For SIEGE-GANG and RORIX it is absolute at any life total.`
and the old fourth case shortened to `- Otherwise "ATTACK: none" under the crack-back exception.`
Paid by `...vs-ai_baka_deck152.jsonl` **seq 43**. The old fourth case was unreachable by
construction — case 3 fires on every board where a creature is listed — so the fix is ordering, not
new policy.

**Edit A2 — BLOCKER TAGS' one-blocker bullet says what to do.**
Before: `- N is 1, no GANG BLOCK clause -> the parenthesis is the whole story.`
After: `... the whole story, and "(your attacker dies, their blocker lives)" is a refusal.`
Same seq. The bullet named the evidence and stopped.

**Edit B — STARSTORM'S X is decided on the cast row's own printed pricing, and names its markers.**
Before: `DECIDE WHETHER AT THE CAST ROW, NOT AT THE X MENU: it prints the same two lists and still
offers "Cast nothing right now". After "Cast Card Normally" the mana is paid; the X menu's Decline
cancels the cast without refunding it and loses the turn's mana.`
After: `DECIDE WHETHER AT THE CAST ROW, NOT AT THE X MENU. That row prints the whole test: "kills
THEIRS: ...; YOURS: ..." at each priced X, plus a marker like "[<- board sweep: THEIRS n / YOURS
m]". RUN DO NOT CAST ON THAT TEXT, THERE, and when it holds answer "Cast nothing right now" on that
same window. A marker saying the sweep takes more of YOURS than of THEIRS is a REFUSAL, not advice
about which X. Declining one screen later is the worst answer there is: the announcement has
already tapped your sources and every mana it paid is stranded for the turn.`
Paid by `...vs-ai_baka_deck162.jsonl` **seq 51 → 53**. The old text asserted the rule without
telling the model what on the cast row to run it against, and the `[<- board sweep: ...]` /
`[<- best X for this cast: ...]` markers were named nowhere in the guide — the only marker sentence
in the file is scoped `ON THE X MENU:` and says a marker "says WHICH X, never WHETHER", which is
exactly wrong for the cast-row marker.

**Edit C — ORDER OF OPERATIONS gains the mana subtraction, so a land kill cannot eat the body.**
Added after entry 7:
`ONE OVERRIDE: before entry 3, subtract its cost from "Mana available:" - if what is left cannot
cast a creature your hand marks [castable now], cast THAT BODY FIRST and take the land later. Their
land keeps; a body in hand can be discarded, exiled or countered out of it.`
Paid by `...vs-ai_baka_deck146.jsonl` **seq 19** (6 mana, Commander `[castable now]` at 5, Molten
Rain taken, `{leaves 3 ...}`, Commander discarded on turn 11, 0 creatures for the game). Same shape
as the wave-72 upkeep gate, moved to the window where the ORDER is actually applied.

**Edit D — a carried plan never outranks the window's printed verdict.**
Added as the opening of PLAN LINES AND THE HOLD ROW:
`A CARRIED PLAN NEVER OUTRANKS THIS WINDOW'S PRINTED VERDICT. "I said I would, so I will" is not a
reason to act: when the row your plan names is priced as losing a body ("your attacker dies, their
blocker lives"), as killing more of YOURS than of THEIRS, or as a rule here forbids, re-plan on the
numbers in front of you.`
Paid by seq 43's reasoning (the plan was its entire stated justification) and seq 19's (its plan
named the Commander and it cast the land kill). The LAND PLAN's own copy of this rule shrank to one
clause to pay for it — the general statement covers both.

**Edit E — MULLIGAN re-keyed to the words the window prints.** All three bullets moved off
`"(keeping N)"` onto `KEEP SIZE N is the number the mulligan row itself names ("a keep after this
one would keep N cards"); there may be no "(keeping N)" text anywhere, so use that row.` Paid by
**seq 2** in all six games (§2 MED).

**Edit F — the upkeep subtraction is defined when the hand holds no creature.**
`...cannot cast the cheapest creature in your hand, pass` → `...in your hand (with no creature
there, the cheapest spell), pass.` Paid by `...vs-ai_baka_deck152.jsonl` **seq 45** (upkeep cycle,
hand = two Talismans and a land, the test as written had no referent).

**Bytes trimmed to pay for the above** (≈1,082):
- DRAW PUNISHERS' `a "[DRAW PRICE: ...]" bracket on a row beginning "Cast " is false` — **0 false
  fires**: 36 `[DRAW PRICE` rows in the seat, 0 on a `Cast `/`Play ` row, third wave clean. −86.
- THEY COUNTER compressed to its operative core (bait/threat order and the one-turn hold), losing
  the restatements — **UNTESTED a third wave**: 0 `was COUNTERED by` in any of my six logs. −280.
- The GANG BLOCK header-clause rule compressed — `blocker_forecast_gang` **0 in all six games**;
  the clause has never rendered here. −160.
- The upkeep "never hunt for a missing row" restatement, the mandatory-draw row enumeration, the
  LIFEGAIN example number, the `X=0` refusal (**0 X=0 takes**, both X menus), the Rorix-duplicate
  explanation, the CLEANUP DISCARD restatement, the intro's colour paragraph, SITUATIONS bullet 2's
  tail, and prose tightening in SPARE MANA / NEVER SPEND / RACE CHECK / CRACK-BACK / BLASTMINER /
  LAND PLAN. −556.

### Rejected, though I looked hard at it
**The upkeep Siege-Gang sacrifice gate.** `...vs-ai_baka_deck152.jsonl` **seq 39/40/41** spent all
three Goblins at the face on turn 20 with a creature (Sigarda) on their line and only 6 mana — the
guide's "IT BECOMES YOUR CLOCK when their line reads 0 creatures and Mana available: is 8 or more"
matches neither clause, and the seat then had no blockers. I am not proposing a tightening: Sigarda
has flying, so no 1/1 Goblin could ever have blocked her, and the sacrifices took the opponent
15 → 9. No decision the corpus shows going wrong pays for it.

---

## (4) Optional proposals

**None.** The two items worth generalizing are engine work (the hold-check identity key; the
collapse predicate), not general-guide or skill text, and the `(keeping N)` mismatch is a fact
about the render that each deck guide has to key off — I will not restate an existing amendment.

## What I did NOT check
- The opponent seats' JSONL, except reading board state out of my own rendered prompts; I opened no
  opponent file this wave.
- Whether the `sibling_window_asks_skipped` / `stop_reached_windows_skipped` / `reserve_decline_*`
  zeros are unwired counters, unmet predicates, or subset bars that cannot fire — I confirmed only
  that all four read 0 on all six of my gameend records. I did not read the code. Engine seat's call.
- Whether the hold-check's "4 rows" is a fixed constant or a real count of something I cannot see —
  I established only that the rendered option lists differ by exactly 1 row.
- The 106 `hold_windows_skipped` and the 7 `main_phase_windows_skipped`: I did not audit whether any
  skipped window would have changed a decision. On my seat 7 skipped main phases over 6 games (≈1.2
  per game, all in games where the seat had no castable main-phase row) reads plausible, not
  over-counted; I cannot speak to the corpus's 375.
- The `askreplay/` sidecar (23 `ask_replays_reserved` in this seat) — engine seat's instrument.
- MULLIGAN beyond the fresh-7 window: six keeps at 7, no mulligan taken, so no `(keeping N)` window
  and no BOTTOMING window exists anywhere in the seat. That half of the section is untested.
- THEY COUNTER: **UNTESTED**, third wave running — no counter and no `was COUNTERED by` line.
