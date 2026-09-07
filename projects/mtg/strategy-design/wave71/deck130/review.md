# Wave 71 — deck 130 (Budde's Ponza, mono-red land destruction) — per-deck review

Corpus `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260907-085638`, binary master `b31f66cf3`,
`--thinking on`. My six seat files are `...ai_baka_deck130-0x<ptr>-vs-...`. Live guide = the wave-70
edit (19,998 bytes).

**Seat census.** 353 model decisions over 6 games (wave 70: 245). **0 fallbacks, 0 truncations, 0
`action_before_plan`, 0 `menu_pass_no_progress`, 0 `ask_replays_refused`.** 12 records carry
`plan_line_missing` (3.4%) and 11 of those also carry `off_protocol_bytes` 13-93; 7 `async_drops`;
21 `ask_replays_reserved`; 3 `repeat_annotated_takes`. Window mix (own turn unless noted):
Main1 ask 210, upkeep priority 59, Main1 priority 31, **upkeep ask 27**, Main2 ask 8, draw ask 5,
combat-damage ask 3, blockers ask 1, opponent-turn windows **4 of 353**.

---

## (1) Game-by-game, and the deciding decisions

| Opponent | Result | Turn | Final life | Decisions |
|---|---|---|---|---|
| 152 | LOSS | 9 | -1 / 20 | 16 |
| 126 | **WIN** | 14 | 19 / -2 | 41 |
| 125 | LOSS | 71 | 0 / 67 | 195 |
| 146 | LOSS | 19 | -4 / 12 | 41 |
| 162 | LOSS | 13 | -4 / 19 | 40 |
| 123 | **WIN** | 22 | 20 / 0 | 55 |

### vs 123 — WIN. The wave-70 EMPTY-HAND rule closed the game (guide success)
Seq 43-55 is a clean Hammer-of-Bogardan recursion clock: return at upkeep (seq 43, 48, 52), recast
in main 1 and point 3 at the face (seq 46/47, 49/50, 54/55), 18 → 13 → 10 → 6 → 3 → 0 with two
Molten Rains earlier for tempo. The wave-70 EMPTY-HAND paragraph named exactly this line and the
model executed it verbatim. **No misplay found in this game.**

### vs 126 — WIN, and the first payoff of the new own-turn instant windows
Seq 31-33 is a **Combat damage, YOUR turn** casting ask (a window class that did not exist in wave
70): the model cast Spark Spray and pointed it at the face at opponent life 6 → 5, then killed with
Rorix. Land-kill on curve (seq 13, Lay Waste at Sunpetal Grove after answering the modal menu
correctly), Molten Rain, Rorix, done on turn 14.

### vs 125 — LOSS. Two Rorixes countered back to back, then a 25-turn race it could not win
- **seq 20 (t10, life 20/20, 6 mana)**: cast Rorix into `Their untapped sources: 5 ({u}{w})`,
  `Opponent hand size: 4`, `Their graveyard (1 card): Essence Scatter {1}{u}`. The guide's THEY
  COUNTER predicate was TRUE and it cast the threat with no bait. Countered by Essence Scatter.
- **seq 23 (t12, 6 mana)**: cast the SECOND Rorix into the same five open `{u}{w}` sources.
  This time `Their graveyard` was **empty** — the opponent's Elixir of Immortality had shuffled the
  graveyard away one turn earlier — so the guide's graveyard-keyed predicate read FALSE while the
  narration two lines above read `Your Rorix Bladewing was COUNTERED by Essence Scatter`. Countered
  by Fall of the Gavel; opponent gained 5. **This is a guide gap, not a read failure** (edit D).
- **seq 55-203 (t22-t70)**: with both Rorixes and the Commander in the graveyard, the model ran the
  Hammer loop — return for {2}{R}{R}{R} at upkeep, recast for {1}{R}{R}, 3 to the face — **25
  consecutive turns**, while `Opponent life trend` printed `+13`, `+15`, `+15` per four turns
  (Sphinx's Revelation ×N). Its own reasoning at seq 138 says *"My clock is Hammer of Bogardan (3
  damage). It's extremely slow against 44 life"* and it kept going, because the wave-70 PLAN FLOOR
  names that exact action as the floor. Opponent 20 → 67; deck130 bled out on Talisman taps and
  Staff of Nin pings.
- **seq 172-184 (t62)** is the counter-example the guide should have taught 40 turns earlier: it
  drew a Siege-Gang, cast it, and sacrificed **four** Goblins at the face across two windows
  (seq 174, 176, 181, 182) for 8 damage in one turn at 11 mana — nearly 3× the Hammer loop's output
  for less mana. The live guide calls that ability "REMOVAL, not a clock" and takes it "only when
  the row reads - DIES"; the model overrode the guide to make the right play (edit E/F).

### vs 162 — LOSS. Draw-punisher bleed; no window ever existed to answer it
Underworld Dreams + Howling Mine + Fate Unraveler (3/4) + a second Unraveler: 2 then 3 life per
card, 2 cards per draw step. Life 20 → 19 → 17 → 13 → 10 → 8 → 6 → 4 with almost none of it from
combat. The model correctly refused every optional draw (guide's N≥2 rule), held both Starstorms,
and at seq 31 (t10, life 13, **5 mana**) cast Lay Waste at a basic Island — its reasoning names the
punisher tax and the land-plan numbers correctly. **Starstorm at X=4 is the only card in the deck
that kills a 3/4 and it costs 6 mana; the seat never reached 6 lands.** Hammer (3) and Spellbomb (2)
both print `kills 0 of the 1 CREATURE target`. Final blocks (seq 43, life 4): `BLOCKS: none` is
**arithmetically correct** — Master of the Feast (5/5 flying) is tagged `NONE of your available
blockers can block this attacker` and 5 ≥ 4, so every block line still dies. No guide item; this is
a deck-matchup hole (mono-red has no answer to a 4-toughness enchantment-plus-body punisher engine).

### vs 146 — LOSS, one small guide violation, otherwise behind on board
**seq 35 (t16, life 5, hand = one Molten Rain, their board 5 lands)**: reasoning states *"Land plan:
N=5. The plan is dead when N >= 5 … but I have no better play"* and casts it. The live guide already
says the plan "does not revive because you have nothing else to do with the mana", so this is a
recurrence of wave-70's carried-plan shape — but with a one-card hand it changed nothing. LOW.
The game was decided at seq 31/33 (t14-15): Siege-Gang landed at 8 life and its first sacrifice went
at **Kaya the Inexorable** (planeswalker-first, correct per TARGETING step 1) while a 3/3 kept
attacking.

### vs 152 — no deciding misplay to report
Mulliganed to 6, opponent curved out with Luminarch Aspirant + two Elite Spellbinders; at 9 life on
turn 8 the model cast Pyrite Spellbomb then Hammer at Elite Spellbinder #2 (3 kills a 3/1 — correct)
and died on turn 9. Hands are real. Variance.

---

## (2) Engine / interface / card items

### HIGH — the seat gets priority on the OPPONENT'S turn 4 times in 353 decisions, so its only instant is functionally a sorcery
Starstorm is this deck's sweeper and its **only** instant (`type=Instant`, `mana={X}{R}{R}`,
verified in `bin/Res/sets/primitives/mtg.txt:112835`). Across six games the seat received exactly
**4 opponent-turn windows** (3 `priority`, 1 `ask`), **all four at the opponent's Main phase 1** —
never their end step, never after they declared attackers, never with a spell of theirs on the
stack. **0 of those 4 carried a castable Starstorm in hand.** Consequently the seat's whole
instant-speed upside — sweep after their attackers are declared, sweep at their end step, sweep
their creature before it hits the board — was never once available in 353 decisions.
Repro: every deck130 seat file; the census above is reproducible by grouping every `ask`/`priority`
record on the `Phase: ... | It is the opponent's turn.` header line. Contrast lane BQ's L4, which
opened the seat's OWN-turn instant windows and demonstrably works (item below); the missing half is
the response window on the opponent's turn.

### HIGH (guide-visible, engine-caused) — the Hammer-return upkeep window makes an unwinnable loop the cheapest legal action
The deck125 game contains **25 consecutive `Put a card into hand with Hammer of Bogardan` upkeep
rows** (seq 55, 61, 65, 71, 75, 82, 86, 90, 94, 102, 106, 113, 119, 125, 130, 135, 142, 158, 169,
185, 192 …), each followed by a main-phase recast for 3 to the face, against an opponent gaining
5-15 life per four turns. Nothing in the render ever priced the loop against the opponent's own
trend line, even though the frame prints `Opponent life trend: turn 50: 29, turn 52: 36, turn 54:
44, now 44 (+15 since turn 50)` on the same screen. This is not a false surface — it is a *missing
comparison* on a repeatable row the model will take forever. Suggested annotation on any repeatable
damage row: `{repeatable: N damage per turn; their trend is +M per turn — this row does not close}`.
Repro: `1788789427-ai_baka_deck130-0x562579c8ff10-vs-ai_baka_deck125.jsonl` seq 138, rendered line
`Opponent life trend: … now 44 (+15 since turn 50)` beside row `1. Cast Hammer of Bogardan {1}{r}{r}
… {kills 0 of the 0 CREATURE targets at 3 damage - and 3 to the opponent at life 44 leaves them at 41}`.

### MED — `plan_line_missing` is always a DROPPED LABEL, never a missing plan; the plan carry then goes stale
12 of 12 `plan_line_missing` records in this seat are the same shape: the model writes the plan
sentence and omits the `PLAN:` label, so the sentence is scored as `off_protocol_bytes` and the
carried plan silently reverts to an older one.
Repro: `...vs-ai_baka_deck146.jsonl` seq 35, reply verbatim
`"\n\nCast Molten Rain at Plains #1.\nCHOICE: 1 (Cast Molten Rain)"` (`plan_line_missing: true`,
`off_protocol_bytes: 30`). Others: deck125 seq 47, 50, 64, 67, 87, 92, 97, 103, 118, 121; deck152
seq 13. The action parsed correctly in all 12. Two cheap engine options: accept a leading
label-less first line as the plan when a `CHOICE:` line follows it, or echo the fact back
(`your last reply carried no PLAN: label, so the plan below is from turn N`).

### MED — turn 60 of the deck125 game asked NINE upkeep windows
seq 152-160 are all `turn 60, Upkeep`, alternating `ask` and `priority`, answered
Hold / cycling / Cast nothing / Cast nothing / Hammer return / Hold / Hold. At p50 latency this is
~10 minutes of inference on one upkeep. The upkeep window count for the seat is 86 of 353 (24%),
up from wave 70. Post-L4 this is the cost side of the fix; worth an engine-seat census of whether
consecutive upkeep windows differ by anything a hold should have covered.

### LOW — a `[repeat: activated this turn N times already]` tag reads as a stop sign
3 `repeat_annotated_takes` in the seat, all at deck125 t62 (seq 176, 181, 182) where repeating was
correct (8 damage in one turn). The tag is a count, but the guide had turned it into a refusal.
No engine change needed; noted so the tag is not "hardened" into a warning.

### Card facts — verified against the primitive, all correct
`Starstorm` (`mtg.txt:112835`, Instant, `{X}{R}{R}`, `damage:X all(creature)`, cycling {3});
`Siege-Gang Commander` (`mtg.txt:105071`, `{3}{R}{R}` 2/2 Goblin, `_GOBLINTOKEN_*3`,
`{1}{R}{S(goblin|myBattlefield)}: Damage:2 target(anytarget)`) — the Commander is itself a Goblin,
so the ability can eat it; `Rorix Bladewing`, `Hammer of Bogardan`, `Dwarven Blastminer` unchanged
from wave 70. **No card item.**

---

## (2b) Wave-71 lane predictions this seat can adjudicate

- **lane BQ L4 (instant `[castable now]` gets no cast row at own-turn non-main windows) — PASS.**
  36 own-turn non-main casting asks now exist in this seat (27 Upkeep, 5 Draw, 3 Combat damage,
  1 Blockers); **35 of 36 render a `Cast ...` row**, and the one that does not (deck126 seq 33) is a
  target menu. The wave-70 signature (23 of 23 upkeep windows with `Starstorm [castable now]` and no
  cast row) is **gone**: e.g. `...vs-ai_baka_deck123.jsonl` **seq 20**, `Phase: Upkeep | It is your
  turn.`, row `1. Cast Starstorm {r}{r}{x} {X pricing: max affordable X=3 …}`. Cards that cannot be
  cast now say so on the hand line — `[no cast row now: sorcery speed - only in your own main phase
  with an empty stack; right now the phase is Upkeep, not a main phase]`, 77 renders. The wave-70
  HIGH is discharged for this deck.
- **lane BQ L5 (cast-then-cycling reversal) — PASS.** 7 modal cast-mode menus, **6 took "Cast Card
  Normally"** (wave 70: 4 of 21). All 7 carry the new header
  `(you answered "Cast X" at the previous window - this menu completes THAT cast …)`. The single
  reversal, `...vs-ai_baka_deck126.jsonl` **seq 39**, is deliberate and correct: opponent at 6,
  Rorix 6/5 flying ready, and the reasoning reads *"Rorix … attacks for lethal. Spark Spray 1 damage
  is unnecessary. Cycle it."* Exactly the outcome lane BQ predicted (a remaining reversal that is
  separable from a misread because the header fact is in the prompt).
- **lane BQ L3 (`noop_row_zero_reask` false fires) — UNTESTED in this seat.** 0 fallbacks of any
  kind in 353 decisions.
- **lane BQ L12 (reveal answered with `BLOCKS:`) — UNTESTED.** 0 `reveal` records in this seat.
- **askreplay sidecar — UNTESTED here**: 21 `ask_replays_reserved` across the seat but the corpus
  `askreplay/` directory is empty; I did not investigate the path (engine seat's item).

---

## (3) Guide verdict: **EDIT** — revised guide at `wave71/deck130/strategy.txt`

**19,950 bytes, 48 fewer than the live 19,998.** The guide did NOT grow, so no rule has to earn new
bytes; the four added rules are paid for by four trims listed at the end.

**Edit A — the upkeep section's false claim removed (Trust Doctrine).**
Before: *"A priority menu … does NOT list your casts. Starstorm can read '[castable now]' … with no
Cast row on that menu - its Cast row appears later, at the 'Casting decision' ask in your main
phase"*.
After: *"Your own upkeep asks twice: a casting ask that CAN offer 'Cast Starstorm', and a priority
menu of abilities and cycling rows. A card you cannot cast there says so on its own hand line ('[no
cast row now: sorcery speed …]'); believe that line, never hunt for a missing row."* The pass
default and the mana gate survive; the exception list gains *"or a Starstorm that kills a creature
THIS turn"*.
Paid by: lane BQ L4 landing — deck123 **seq 20** (an upkeep `Cast Starstorm` row), 27 upkeep casting
asks in this seat, and the 77 `[no cast row now: …]` renders. The old sentence is now false, and a
false statement on the surface is worse than no statement.

**Edit B — modal-menu paragraph cut to the engine's own header.**
Before: five lines telling the model to reread its own PLAN at `Choose an option for <card>`.
After: two lines pointing at the header the engine now prints.
Paid by: lane BQ L5's header shipping (6 of 7 correct); the guide no longer has to carry what the
render says.

**Edit C — `[DRAW PRICE]` rule restated as a scope fact.**
Before: *"A '[DRAW PRICE: …]' bracket on a row beginning 'Cast ' is FALSE - casting a permanent
whose later ability can draw costs you no life…"*
After: *"prices only rows that DRAW (a cycling row, a draw ability, a draw spell); on a row
beginning 'Cast ' it is false - cast it."*
Paid by: all 11 `[DRAW PRICE]` renders in this seat (deck162 seq 8 and its siblings) sit on genuine
draw rows — the false-on-a-cast-row fire of wave 70 did **not** recur, so the rule shrinks to the
scope statement rather than a warning about a bug.

**Edit D — THEY COUNTER's evidence is the LOG, not the graveyard.**
Before: `THEY COUNTER when "Their graveyard" names Cancel, Essence Scatter, Dream Fracture or Fall
of the Gavel, "Their untapped sources:" reads 2 or more and "Opponent hand size:" is 1 or more.`
After: the same three signals, but the graveyard clause becomes `EITHER "Their graveyard" names … OR
the log has ever read "was COUNTERED by"`, plus `THE LOG IS THE EVIDENCE THAT LASTS: a shuffle
empties their graveyard, and a counter you already ate is still the truth about their deck.`
Paid by: deck125 **seq 20 → seq 23**. At seq 20 the predicate was true and the model cast the threat
anyway; at seq 23, one turn after being countered, Elixir of Immortality had shuffled the graveyard
and the predicate read FALSE, so the second Rorix walked into the same five open `{u}{w}` sources
and was countered by Fall of the Gavel. Both copies of the deck's only real win condition, gone in
two turns, on a signal the frame still carried in its narration.

**Edit E — SIEGE-GANG's sacrifice becomes the clock on an empty board at 8+ mana.**
Before: `"{1}{R}, Sacrifice a Goblin: 2 damage" is REMOVAL, not a clock: take it only when the row
reads "- DIES" … At any other N the Goblin is worth more as a body, and a "[repeat: activated this
turn N times already]" tag means you already answered this - move the turn on.`
After: removal **while their board has creatures**; and `IT BECOMES YOUR CLOCK when their line reads
0 creatures and "Mana available:" is 8 or more - each {1}{R} is 2 to the face, payable three or four
times in one turn, which beats every other damage this deck has. A "[repeat: …]" tag is a count, not
a refusal: keep paying while the mana lasts.`
Paid by: deck125 **seq 174, 176, 181, 182** — four sacrifices, 8 damage in a single turn at 11 mana
against an empty board, taken *against* the live guide's own text; versus the 25-turn Hammer loop
(3 damage for 8 mana a turn) the guide did endorse.

**Edit F — the PLAN FLOOR names the Goblin engine first and gives the Hammer loop a release.**
Before: `the FLOOR is the plan: return Hammer this upkeep and point 3 at their face, or cycle for a
body.`
After: `cast Siege-Gang and sacrifice Goblins at their face while the mana lasts; with no Commander
in reach the floor is returning Hammer and pointing 3 at their face, and when "Opponent life trend"
shows them gaining 3 or more a turn that return is not a clock - spend the turn deploying instead.`
Paid by: deck125 **seq 55-203** (the loop the wave-70 floor prescribed) and **seq 138**'s reasoning
naming it *"extremely slow against 44 life"* while executing it, plus seq 172-184 (the alternative).

**Bytes: what was trimmed to pay for the above.**
- The LIFE LOOP paragraph's frame citations (`"LIFE-TO-DAMAGE CONVERTER"`, `"LOOP HALF PENDING"`) —
  **0 renders of either string** in 353 prompts; the actionable half (treat a `paying this costs you
  N life` bracket as lethal) is kept. −130 bytes.
- SITUATIONS bullet 1 (Spellbomb held at opponent 16) — a verbatim restatement of TARGETING step 3
  and THE 6-LIFE FLOOR two sections above. −194.
- BOTTOMING's trailing clause ("what is last goes under, even a land or the Starstorm") — the order
  already says it. −56.
- Modal + DRAW PRICE + upkeep + Hammer/Spellbomb-never-touch-a-land line rewrites. −258.
Net −48.

---

## (4) Optional proposals

**None.** The two HIGH items are engine/render work (opponent-turn response windows; a
trend-vs-repeatable-row comparison), not general-strategy or skill text, and the `PLAN:`-label item
belongs to the engine seat's protocol census. Nothing here generalizes to a rule the general guide
lacks, and I will not restate an existing amendment as a proposal.

## What I did NOT check
- The opponent seats' logs beyond the deck125 file's narration lines quoted above; all opponent
  boards were read from my own seat's rendered prompts.
- Scryfall cross-checks — the five card facts I needed matched the primitives exactly and none
  looked mis-modelled.
- The `askreplay/` sidecar (empty; 21 `ask_replays_reserved` in this seat unexplained) and the
  `hold_windows_skipped` totals (26/121/15/81/12/24) — whether any skipped window would have changed
  a decision is the engine seat's HOLD census, not mine.
- The STARSTORM'S X section beyond one cast: the deck cast Starstorm **once** in six games
  (deck146 seq 23→25, X=2, `kills THEIRS: Silverquill Silencer; YOURS: none` — correct), so most of
  that section remains UNTESTED and I edited none of its X-choosing rules.
- Mulligan/bottoming: one mulligan (deck152 seq 2-4, keeping 6) — too few to judge.
