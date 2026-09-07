# Wave 70 — deck 130 (Budde's Ponza, mono-red land destruction) — per-deck review

Corpus: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-224849`, binary master 14c82df37,
`--thinking on`. My seat's six files are `...ai_baka_deck130-0x<ptr>-vs-...`. Record 1/6.

Seat census: 245 model decisions across 6 games. **0 fallbacks, 0 truncations, 0
`action_before_plan`, 0 records with `off_protocol_bytes>0`, 0 `answer_replaced`, 0
`commit_retracted`.** Reply shape was perfect in every one of 245 decisions — under reasoning-on
this seat never wrote a byte outside `PLAN:` + action line. `reasoning_chars` p50 5,945 / p95 10,671
/ max 19,800. Latency p50 97.1 s, p95 215.8 s, max 476.7 s; `deadline_pct` p50 10.7, max 52.9 (no
wall pressure at TIMEOUT=900).

---

## (1) Game-by-game and the deciding decisions

| Opponent | Result | Turn | Final life | Seat file (suffix) |
|---|---|---|---|---|
| 123 | LOSS | 13 | -17 / 20 | `...deck130-0x55fef6684130-vs-...deck123` |
| 125 | LOSS | 59 | 0 / 37 | `...deck130-0x56465b4f3e00-vs-...deck125` |
| 152 | LOSS | 11 | -4 / 18 | `...deck130-0x5635dea458d0-vs-...deck152` |
| 146 | LOSS | 21 | -1 / 11 | `...deck130-0x56297180b7c0-vs-...deck146` |
| 126 | **WIN** | 18 | 12 / -2 | `...deck130-0x56190ce4b7c0-vs-...deck126` |
| 162 | LOSS | 11 | -2 / 16 | `...deck130-0x55860277d490-vs-...deck162` |

### vs 123 — the sweep that was never on the menu (PERCEPTION, engine-caused)
The opponent assembled Intruder Alarm + Bloodline Keeper + two Thraben Doomsayers and went from 1
creature to **20 creatures** between turn 9 and turn 12. Deck130 held **two Starstorms all game and
cast neither**.

The decision: **seq 17, turn 12 Upkeep, life 11, Mana available 4**. Hand prints
`Starstorm ... [instant] [castable now]` twice. The seven-row menu is four Blastminer land-kills,
`cycling with Lay Waste`, `cycling with Starstorm`, and the hold row — **there is no `Cast
Starstorm` row**. Starstorm at X=2 for exactly those 4 mana kills 8 Humans, 9 Vampires and both
Doomsayers (19 of 20 bodies). The model activated Blastminer at Arcane Sanctum for {2}{R}, leaving
1 mana, so the main-phase cast ask never opened (records go seq 17 → seq 18 `discard`), and it died
on the crack-back.

The `reasoning` is unambiguous that this was the interface and not a read failure — it enumerates
the menu verbatim ("1. Destroy … 5. cycling with Lay Waste 6. cycling with Starstorm 7. Hold
priority"), never mentions casting Starstorm, and applies the land plan mechanically: *"The land
plan says: 'Their lands <N>; my life <L>'. N=4, L=11. This is within the land plan range."* It did
not register that 20 creatures on their side is lethal. Two faults, one PERCEPTION (no cast row) and
one STRATEGY (land plan has no board gate).

### vs 125 — THEY COUNTER soft-locked the win condition for three turns (STRATEGY)
Turn 30, life 19, opponent 16 with **zero creatures**, 14 mana available, hand = **one card, Rorix
Bladewing, `[castable now]`**. Opponent graveyard shows Cancel ×2 / Essence Scatter ×2, 5 untapped
sources, hand 6 → the guide's THEY COUNTER predicate is TRUE, and its clause *"a turn with only one
castable spell holds the body instead"* has no release. The reasoning quotes the clause and
concludes *"I have no other spells to cast as bait. So I hold Rorix."*

Held again at seq 68 (t32). At **seq 70 (t34)** it correctly cast Blastminer as bait — and then at
**seq 71**, with `1. Cast Rorix Bladewing` on the menu and the bait already spent, held anyway.
Rorix finally landed t36 (seq 72) and was Path-to-Exiled t37. Three full turns of a hasty 6/5 flier
(≥18 damage) forgone against an empty board; the opponent used them to go from 16 to 37 life.

Later in the same game the PLAN line degenerates into concession — seq 105 *"no path to victory
exists"*, seq 107 *"No win condition available"* — which the guide's PLAN section explicitly bans.
seq 108's plan says "Cycle to find a creature" while the answer is "Cast nothing right now".

### vs 146 — the carried plan overrode a live guide gate (STRATEGY, plan-carry mechanism)
**seq 30, turn 16 Main 1, life 14, opponent 10**, Rorix dead to Silverquill Command the turn before,
hand = Lay Waste + Talisman, 6 mana, their board **5 lands**. The reasoning reads:

> *"the guide says … 'At N of 5 or more … the spell is dead for the game'. They have 5 lands. The
> land plan is dead for the game according to the guide. **But I already committed to it**, and
> it's just a land kill anyway. I'll cast it."*

The carried `YOUR PLAN (as you last stated it …)` line beat an explicit, correctly-recalled guide
rule. That is the plan-carry mechanism doing harm, not the guide text.

### vs 162 — chip damage on a survivor, and a false DRAW PRICE (one of each)
Underworld Dreams + Fate Unraveler up (N=2). **seq 17, t8**: the row
`1. Cast Pyrite Spellbomb {1} … [DRAW PRICE: this draws 1 card, and the opponent's Underworld
Dreams punishes every draw, so taking it costs you 1 life right now - you would be at 15]`. Casting
an artifact draws nothing; the log confirms no life was lost for the cast (16 → 15 came from Master
of the Feast's own forced draw during their upkeep). **False verdict on the rendered row.**

**seq 18, t10, life 10**: aimed the Spellbomb at Master of the Feast on a row reading
`{right now: takes 2 damage - SURVIVES (toughness 5)}`. Licensed by the live guide's TARGETING step
3 ("a creature this cannot kill but that attacks every turn -> aim there rather than bank the
card"), which directly contradicts step 2's "SURVIVES is the row you decline". The card was spent
for nothing; the model died two turns later at 8 life with Stone Rain uncast.

### vs 126 — the win, and what the guide got right
Clean execution of the intended plan: Molten Rain on curve, Rorix t12 and t14, Hammer at **Sorin,
Lord of Innistrad** (TARGETING step 1, planeswalker-first — seq 34), Molten Rain at Bayou for its 2
incidental damage as reach (seq 39-40, opp 6 → 4), then Rorix for lethal (seq 42). One blemish:
**seq 35 → 36**, the model answered `Cast Starstorm` and then took `cycling` at the modal menu —
see the reversal item below.

### vs 152 — no deciding misplay to report
Lost the die roll to a fast aggressive draw; three land-kills resolved but the model was at 4 life
by turn 10 with Siege-Gang still one land away. Hands are real; I read this as variance, not guide
effect.

---

## (2) Engine / interface / card items

### HIGH — an instant marked `[castable now]` has no cast row at `priority`-kind windows
**23 of 23** windows in this seat where `Your hand` printed `Starstorm … [instant] [castable now]`
and `kind == "priority"` offered **no `Cast Starstorm` row** — only its cycling row. Every
"Casting decision" ask with the same annotation *did* offer one (**25 of 25**, two of them at
Upkeep on the opponent's turn), so the engine can render an instant at instant speed; the priority
seam simply does not.
Repro: `1788752934-ai_baka_deck130-0x55fef6684130-vs-ai_baka_deck123.jsonl`, **seq 17**. Hand line:
`Starstorm (copy 1 of 2 in your hand) {r}{r}{x} [instant] [castable now]`. Menu rows 1-7 contain no
cast row. This cost the game outright (see vs 123 above). Fix either the menu (offer the cast) or
the annotation (say `[no cast row now: this window lists abilities only — your main phase's Casting
decision is where you cast it]`, the wording every sorcery already gets).

### HIGH — the two-step cast reverses itself at "Choose an option for <card>"
**7 confirmed reversals**: the model answered `Cast <X>` at the casting ask, then took the
`cycling` row at the follow-up modal menu, discarding the card it had just committed to cast — and
its own PLAN line one window earlier said cast. Of 21 modal cycling menus in this seat only 4 were
answered `Cast Card Normally`.
Repro (deck126 seq 35 → 36): seq 35 reply `PLAN: Starstorm at X=1 to kill Vampire, then attack with
Rorix for 6. / CHOICE: 1 (Cast Starstorm)`; seq 36 menu is
```
Choose an option for Starstorm:
1. Cast Card Normally [cost: {r}{r}{x}] … {the alternative this replaces: cycling - draws 1 card}
2. cycling … {the cast this replaces: cost {r}{r}{x} - …}
3. Decline - do nothing
```
seq 36 reply: `PLAN: Cycle Starstorm to spend mana and draw … / CHOICE: 2 (cycling)`.
Others: deck125 seq 4→5, 41→42, 47→48; deck146 seq 23→24; deck126 seq 4→5; deck125 seq 37→38.
The modal menu does not tell the model that it is the second half of a cast already answered, and
`{the alternative this replaces: …}` reads as an offer rather than as a warning. Suggested render:
carry the answered choice into the header — `Choose an option for Starstorm (you answered "Cast
Starstorm" at the previous window; row 1 completes that cast, row 2 abandons it)`.

### MED — false `[DRAW PRICE]` on a cast row
`1788752971-...-vs-...deck162.jsonl`, **seq 17**, rendered line:
`1. Cast Pyrite Spellbomb {1} … [DRAW PRICE: this draws 1 card, and the opponent's Underworld
Dreams punishes every draw, so taking it costs you 1 life right now - you would be at 15]`.
Casting the artifact draws no card and cost no life (verified against the game log inside seq 18's
prompt). The annotation appears to key off the card's *text* containing "Draw a card" rather than
off the action being a draw. 1 of 1 occurrence in this seat, but the live guide already carries a
hand-written counter-rule for it ("A row marked `[castable now]` is not a draw: cast it"), which
suggests it recurs elsewhere.

### MED — the carried `YOUR PLAN` line outranks live rules in the model's own reasoning
`...deck146.jsonl` **seq 30** (quoted in full above): the model recalls the governing rule, states
the plan is dead, and executes the stale plan because it "already committed". The plan carry is
supposed to give continuity, not commitment. Worth the engine seat's attention as a plan-carry
verdict: consider rendering the carried plan as *context* rather than as an obligation, e.g.
`YOUR PLAN (last stated turn 16 — a record of what you intended, not a commitment; re-decide from
this board)`.

### LOW — reasoning ceremony inflates latency
**239 of 245** reasoning traces end in a self-verification loop that restates the final two lines
three or four times ("Output matches. / Proceeds. / [Output Generation] / Done."). Example: deck123
seq 5, `reasoning_chars` 9,286, of which roughly the last 1,500 are pure format re-checking. This
is prompt-shaped, not a defect, but it is a large share of the p50 97 s decision cost.

### Card facts — verified, all correct
`Starstorm` (`mana={X}{R}{R}`, `auto=damage:X all(creature)`, `autohand=__CYCLING__({3})`),
`Dwarven Blastminer` (`auto={2}{R}{T}:destroy target(land[-basic])`, 1/1, `{1}{R}`),
`Rorix Bladewing` (Legendary 6/5 flying haste, `{3}{R}{R}{R}`), `Hammer of Bogardan`
(`auto=damage:3`, `autograveyard={2}{R}{R}{R}:moveto(hand) myUpkeepOnly`, `{1}{R}{R}`) — all match
`projects/mtg/bin/Res/sets/primitives/mtg.txt` and the guide's stated costs. **No card item.**

---

## (3) Guide verdict: **EDIT** — revised guide at `wave70/deck130/strategy.txt` (19,998 bytes, under the 20,000 ceiling)

The guide did not grow: it is 19,993 bytes live and 19,998 revised. Every rule below was paid for by
bytes trimmed from prose that the corpus shows carrying no decision (verbose restatements in the
life-loop, X-menu, wall-check, lethal-blocking, bottoming and crack-back paragraphs); no rule was
deleted except TARGETING step 3, which is deleted on evidence.

**Edit 1 — new section "YOUR OWN UPKEEP - SPEND NOTHING THAT YOUR MAIN PHASE NEEDS".**
Before: `In your own upkeep answer CHOICE: 0 (pass) unless the row destroys a land, kills a
creature, or is the Hammer return under EMPTY-HAND`.
After: the same pass default, plus the fact that a priority menu lists abilities and cycling and
NOT casts (so `[castable now]` there is not an offer), plus a second gate — an upkeep payment is
only legal if what remains still covers every ORDER OF OPERATIONS entry the board matches.
Paid by: deck123 **seq 17** (upkeep Blastminer for {2}{R} left 1 mana and consumed the turn's only
window at a 4-mana X=2 sweep against 20 creatures) and deck123 **seq 15** (same shape, turn 10).

**Edit 2 — THE LAND PLAN now takes three numbers, not two.**
Before: `WRITE TWO NUMBERS … "Their lands <N>; my life <L>" … At N of 5 or more, or L of 8 or less,
the spell is dead`.
After: `WRITE THREE NUMBERS … "Their lands <N>; my life <L>; their power <P>"` — dead when N≥5,
**or L≤8, or P≥L**, or their life is inside your reach; with the added line *"a board about to kill
you is answered with removal or a blocker, never a land."*
Paid by: deck123 **seq 17**, where N=4 and L=11 both passed the old gate while 20 creatures (P far
above 11) were about to kill the model, and its reasoning cited the two numbers as clearance.

**Edit 3 — the land plan's dead numbers now beat a carried plan.**
Added: `A PLAN LINE YOU WROTE LAST TURN NEVER REVIVES A DEAD RULE …`
Paid by: deck146 **seq 30** (*"the land plan is dead … But I already committed to it"*).

**Edit 4 — TARGETING step 3 deleted; "NEVER SPEND DAMAGE ON A SURVIVOR" put in its place.**
Before: ` 3. A creature this cannot kill but that attacks every turn -> aim there rather than bank
the card.` (old step 4, the face floor, is renumbered to 3.)
After: `NEVER SPEND DAMAGE ON A SURVIVOR. A creature you cannot kill is not a target because it
attacks every turn — chip damage on a 5-toughness body buys nothing and the card is gone.`
Paid by: deck162 **seq 18** (Spellbomb into `SURVIVES (toughness 5)` on Master of the Feast at 10
life, with Fate Unraveler — the guide's own named punisher — also out of range).

**Edit 5 — THEY COUNTER gets an expiry and a one-turn hold cap.**
Before: `… and a turn with only one castable spell holds the body instead.`
After: `THE BAIT EXPIRES. Once a spell of yours has resolved or been countered this turn, the body
is your NEXT cast, not a hold. And you hold a body for AT MOST ONE of your own turns: at the second
cast ask where that body is the only castable row, CAST IT. A held threat deals zero and their
counter does not expire while your turns do.`
Paid by: deck125 **seq 66, 68, 71** — three turns holding the only win condition against an empty
board at 14 mana, and seq 71 specifically, where the bait had already been cast that turn.

**Edit 6 — new cycling paragraph: spare mana is not a reason, and the modal menu is not a new decision.**
Added: `SPARE MANA IS NOT A REASON. Unspent mana costs you nothing; a cycled Starstorm is a sweep
you no longer have.` and `"CHOOSE AN OPTION FOR <card>" IS NOT A NEW DECISION — it is the second
half of the cast you just answered. Read the PLAN you wrote one window ago: if it said cast, take
"Cast Card Normally", whatever row 1 is.`
Paid by: the 7 reversals listed in item 2, and deck126 **seq 36**'s reasoning, which cites the
guide's 10-mana cycle release, notes the gate is unmet — *"Here I have 4 floating mana, not 10, but
I still need to spend it"* — and cycles anyway.

**Edit 7 — DRAW PUNISHERS now names the false bracket.**
Added: `A "[DRAW PRICE: …]" bracket on a row beginning "Cast " is FALSE — casting a permanent whose
later ability can draw costs you no life and is not an optional draw.`
Paid by: deck162 **seq 17**.

**Edit 8 — PLAN LINES section forbids concession and names a floor action.**
Before: `there is no "the game is lost"`.
After: the same, plus `never write that no path exists, that you lack a win condition, or that you
pass to draw into one. When nothing else matches, the FLOOR is the plan: return Hammer this upkeep
and point 3 at their face, or cycle for a body.`
Paid by: deck125 **seq 105, 106, 107** (three consecutive concession plans, all answered "Cast
nothing right now" at 4 → 2 life).

**Edit 9 — new SITUATIONS bullet.** `Their line names ten or more creatures and your life is under
half: the land plan is dead, the sweep is the only play, and your upkeep spends nothing.` Paid by
deck123 seq 17; it is the recognition trigger for Edits 1 and 2 in one line.

---

## (4) Optional proposals

None. Every finding above is either an engine item (section 2) or is fixed inside the deck guide.
The HIGH items are engine-side and belong to the engine seat's revert/repair list, not to
`general-strategy.md`; the plan-carry framing (item 2, MED) is a candidate general change but I saw
it fire once in this seat and would rather the engine seat count it corpus-wide first.

## What I did NOT check
- The deck123, deck125, deck146, deck152, deck162 **opponent** seat logs beyond the excerpts needed
  to date the board states above; I read those boards from my own seat's rendered prompts.
- Scryfall cross-checks. All four card facts I needed matched the primitive exactly and none looked
  mis-modelled, so I did not fetch Oracle text; the four cards' primitives are quoted in section 2.
- The X-menu render. **No `Starstorm` X-menu record exists in this seat's 245 decisions** — the deck
  cast Starstorm zero times in six games, so the whole STARSTORM'S X section is UNTESTED here and I
  edited none of its X-choosing rules.
- Blocker/attacker forecast collapse tags (`blocker_forecast_*` are 0 in five of six games, 2 rows
  in the deck152 game), and the `hold_windows_skipped` counters (47 in the deck126 game, 36 vs 162,
  25 vs 125) — I did not audit whether the skipped windows would have changed a decision; that is
  the engine seat's HOLD census.
- The replacement run for the hung deck152-vs-deck126 game (not my matchup, and it does not involve
  deck130).
