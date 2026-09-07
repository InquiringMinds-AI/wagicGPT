# Wave 70 — deck 125 (Revelation Fracture, UW draw-go control) per-deck review

Seat files: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-224849/*ai_baka_deck125-*`.
673 model decisions across 6 games; reasoning on for all 673 (`reasoning_chars` median 6.0k, max
23.8k, no record at 0, no `reasoning_degenerate` above 0.004). 3 fallbacks (0.45%), 0 truncations,
0 `action_before_plan`, max `deadline_pct` 35.8 (WAGIC_GPT_TIMEOUT=900). Latency p50 60.3 s /
p95 145.7 s per decision — I did **not** measure wave 69's thinking-off numbers, so I state no ratio.

## 1. Game-by-game

| opp | result | turns | end life | the decision that decided it |
|---|---|---|---|---|
| 130 | WIN | 59 | 37–0 | none contested; Staff clock, opponent never landed a threat past Path/counters |
| 123 | WIN | 41 | 40–0 | none contested; same |
| 146 | WIN | 34 | 16–0 | clean on-guide game; two Staffs by t22, Revelation X=9 at seq 64 stabilised |
| 152 | **LOSS** | 25 | −2–19 | **seq 38** (t24, 12 life): cast Staff of Nin over a sweeper; dead on t25 |
| 162 | WIN | 40 | 43–0 | seq 122 Emrakul with the opponent already at 1 — harmless, but see item G-3 |
| 126 | WIN | 54 | 175–31 | **seq 116** (t39, opp at 10): Emrakul cast → two edicts → opponent 10 → 40; the game ran 15 further turns and was won only by decking them |

### The loss (deck152), traced
- t21 and t23 the opponent attacked with **Lair of the Hydra**, an animating land, for 8 then 6.
- **seq 35** (t22, 18 life) the header read `Opponent battlefield (10 permanents listed, of which 0
  are creatures, 0 of them without a restriction against attacking, and 10 are lands): ... Lair of
  the Hydra [tapped]` — a board that had just dealt 8 rendered as *zero creatures*. Every sweeper row
  read `{right now: destroys 0 of their creatures}`. The model correctly held.
- **seq 38** (t24, 12 life, 7 mana, hand = Final Judgment + Staff of Nin) is the loss. Both sweeper
  rows read `{right now: destroys/exiles 2 of their creatures ... THEIRS: Wolf (2/2), Luminarch
  Aspirant (1/1)}`. The model cast **Staff of Nin**. Its `reasoning` is unambiguous and cites the
  guide verbatim: *"I have no Staff on the battlefield. Shape (d) applies. The gate is open… But the
  guide emphasizes Staff of Nin as the win condition and shape (d) says cast it first."* It then
  self-checked once more and re-affirmed shape (d) over the K≥2 sweeper rule. This is **STRATEGY**,
  and specifically a guide-ordering defect: `ORDER OF OPERATIONS` step 3 lists "a first Staff" before
  "a Path/sweeper", and gate shape (d) is written with no survival exception, so two guide rules both
  fired and the guide gave no tiebreak.
  Counterfactual is concrete: sweeping kills Luminarch Aspirant, so the Hydra animates to 9/9 and
  gains no +1/+1 counter on t25 → 10 power vs 12 life; the model lives at 2 with a sweeper still to
  come. It instead took 9 + 2 + 1 + the Aspirant counter = 14 and died.
- **seq 39** (same turn, main 2) compounds it: with Luminarch Aspirant (1/1, "At the beginning of
  combat … put a +1/+1 counter") on the board, the model pinged the face. Its reasoning shows the
  guide's own ambiguity doing the damage: *"Luminarch Aspirant is a 1 toughness target, so it
  qualifies… However, pinging the face is the default."* The PING section's "otherwise at the player"
  outranked a body the PATH section's engine-text list would have removed on sight.

## 2. Engine / interface / card items

**HIGH — E-1. One creature, two edicts, thirty life: `Tribute to Hunger` double-pays.**
`1788752968-ai_baka_deck125-…-vs-ai_baka_deck126.jsonl`, seq 120 prompt, turn 39 log:
```
- Opponent cast Tribute to Hunger
- Opponent's Tribute to Hunger resolved and went to the opponent's graveyard
- Opponent used: Gain life equal to its toughness with Tribute to Hunger targeting Emrakul, the Aeons Torn
- Opponent cast Tribute to Hunger
- Opponent's Tribute to Hunger resolved and went to the opponent's graveyard
- Opponent used: Gain life equal to its toughness with Tribute to Hunger targeting Emrakul, the Aeons Torn
- Your Emrakul, the Aeons Torn died
- Opponent gained 15 life (now 25)
...
- Opponent gained 15 life (now 40)
```
Deck 125 controlled exactly one creature and it died exactly once, yet both copies paid 15 life.
Primitive (`bin/Res/sets/primitives/mtg.txt:124073`): `auto=ability$!name(sacrifice)
notaTarget(creature|mybattlefield) dynamicability<!mytgt toughnesslifegain targetopponent!>
sacrifice!$ targetedplayer` — the lifegain is computed from the chosen creature and both instances
resolve against the same body; the second has no creature left to sacrifice and should gain 0 (or
fizzle). Scryfall text agrees with the primitive's `text=` line, so this is an engine/resolution bug,
not a card-text bug. Cost: +15 undeserved life and ~15 extra turns of game.

**HIGH — E-2. Animating lands are invisible to every count on the decision screen.**
`…deck125-…-vs-ai_baka_deck152.jsonl` seq 35 (quoted above) and seq 38. The opponent header, the
sweeper rows' `{right now: destroys K …}` and Lightmine Field's `{right now: they control N creatures
able to attack}` all count *current* creatures, so `Lair of the Hydra` — which had already dealt 14
damage across two turns — renders as a land and contributes 0 to every number a control deck reads.
The only mitigation is buried in the CRACK-BACK paragraph's fourth clause. A UW deck whose entire
guide is written around K and N is structurally blind here. Repro is any board with a creature-land.

**MED — E-3. The CRACK-BACK headline understates by design and the model anchors on it.**
seq 38 rendered: `CRACK-BACK NEXT TURN: 2 of their creatures will be able to attack …, for up to 3
from combat as their board stands - you would be at 9 or lower - and that number is a FLOOR, not a
ceiling: noncreature permanents of theirs that can animate and attack are not in that count - Lair of
the Hydra (best rung their mana pays for: 9 power once animated …)`. Actual incoming was 14. The
model's reasoning read the two halves and produced *"Total potential damage: 2 + 1 + 6 = 9. I would
be at 3 life"* — wrong by 5, because the animated body's own +1/+1 counters and the pump trigger
land after the number. A life total the reader is told is a floor is still the number they subtract.
Suggested render: publish the *ceiling* ("as low as N") alongside, computed with the animate rung and
listed pump triggers, or omit the projected life total when an animate clause is present.

**MED — E-4. `Cast nothing right now` and `Hold priority` are near-synonyms and the decline row wins.**
75 windows printed `[you declined this exact list N times already this turn]`; the model took the
hold row in 42 and answered `Cast nothing right now` in 15 more (e.g. `…vs-ai_baka_deck130` seq 9,
12, 23, 26, 57, 90, 97; `…vs-ai_baka_deck123` seq 30, 33, 39, 44). The guide states the rule in
capitals and it is still missed 20% of the time — the two rows differ only in whether the identical
ask repeats, and that difference is described in a 90-word annotation on the *other* row. Cheapest
fix is at the render: when the declined-count clause prints, drop the plain decline row, or tag it
`(this same question will be asked again this turn)`.

**MED — E-5. Two parser refusals where the intent was unambiguous.**
`…vs-ai_baka_deck162` seq 110, reply `HOLD PRIORITY: 2 (Hold priority)` → `<refused:
unparsed_reply>`, recovered by a re-ask at seq 111/112. `…vs-ai_baka_deck123` seq 93, reply
`PLAN: Counter Vision Skeins with Dream Fracture.\nCHOICE: Cast Dream Fracture` → same refusal,
re-asked at seq 95 and answered identically. In both the row number or the exact short name was
present and unique. Accepting `<LABEL>: <n> (<name>)` for any label, and accepting a bare unique row
name, would have cost 0 re-asks. These are 2 of the deck's 3 fallbacks.

**MED — E-6. The echoed `YOUR PLAN` header is itself a protocol violation generator.**
67 of 673 replies (10%) carried `off_protocol_bytes > 0`, and the classes are all render-induced,
not prose: 53 = a bare plan sentence with the `PLAN:` label missing; 7 = the reply opened with
`YOUR PLAN:` — the literal string the prompt prints above the menu (`…vs-ai_baka_deck146` seq 47,
49; `…vs-ai_baka_deck123` seq 76, 99; `…vs-ai_baka_deck162` seq 63, 119); 6 = a sentence *fragment*
that is the tail of the carried plan (`…vs-ai_baka_deck146` seq 41 `"then activate Staff of Nin to
deal 1 damage to opponent."`; `…vs-ai_baka_deck123` seq 52, 61, 62). Only one reply exceeded two
lines (`…vs-ai_baka_deck162` seq 105, two `CHOICE:` lines; the parser took the first). Under
reasoning-on the PLAN line carries none of the thinking, and the model largely re-emits whatever the
header shows it. In `…vs-ai_baka_deck123` the identical sentence *"then ping the opponent with Staff
of Nin each turn until they reach 0."* was emitted as the plan for 13 consecutive windows, seq 50–76,
across land drops, counterspells, an Elixir activation and cleanup discards — a frozen ledger line
with no relation to the choice made. Recommend the engine seat consider whether the plan carry earns
its keep at all now.

**LOW — E-7. Guide text is being emitted as the plan line.** `…vs-ai_baka_deck162` seq 78,
`off_protocol_bytes` 289: the reply began with the guide's own canned plan sentence copied verbatim
(`"then Staff of Nin from a row whose leaves N covers that counter's cost, then an Elixir row …"`).
That paragraph is removed in the revised guide below.

## 3. Guide verdict: **EDIT** (`wave70/deck125/strategy.txt`, 19,941 bytes — 29 bytes *smaller* than the live guide, so no rule needs to buy bytes)

| # | before → after | paid by |
|---|---|---|
| 1 | The canned plan-line paragraph (`Your plan line is this sequence of actions and nothing else: "Hold the counter in my hand payable, then Staff of Nin …"` + the "never append a per-card verdict" sentence, 515 B) → `Your PLAN line is the actions you intend in THIS window and the next, written fresh every window - never the sentence the prompt echoes back to you under "YOUR PLAN".` | deck162 seq 78 (guide sentence emitted verbatim as the reply); deck123 seq 50–76 (same frozen plan line 13 windows running); the 7 `YOUR PLAN:` openings listed in E-6 |
| 2 | Gate shape (d) `… A SECOND Staff is NOT this shape.` → adds `and shape (d) is OFF while a "CRACK-BACK NEXT TURN" line is printed and your life is 15 or less: answer that line first with the sweeper, Path or Revelation row on the menu, and cast the Staff a turn later.` | deck152 seq 38 — the reasoning cites shape (d) by name as the reason it declined a K=2 sweeper at 12 life, and lost on the next turn |
| 3 | SWEEPERS section gains a lead paragraph: `A LAND OF THEIRS THAT ANIMATES IS IN NO COUNT ON THIS SCREEN … that power is damage no row on this menu prevents, so subtract it from your life yourself before you spend a turn on anything else.` | deck152 seq 35 and 38 — K and N both read 0/2 while an 11/11 land was the only thing killing it |
| 4 | PING: `aim at a creature or planeswalker only when 1 damage finishes it (toughness 1, or 1 loyalty counter); otherwise at the player.` → the ping leaves the face only for a 1-damage-lethal body whose own text reads `{T}:`, `Whenever … attacks` or `At the beginning of`; every other such body and all tougher ones stay at the player | deck152 seq 39 — reasoning explicitly weighed Luminarch Aspirant, found it qualified, and went to the face on the "default" wording; that Aspirant supplied the counter that made the crack-back lethal |
| 5 | EMRAKUL: `When the row IS printed, take it` → `When the row IS printed and NO Staff of Nin is on your battlefield line, take it`, plus a new rule `WITH ANY STAFF OF YOURS ON THE BOARD, EMRAKUL IS NOT YOUR ANSWER … a cheap instant that makes you sacrifice a creature pays its controller life equal to its toughness - 15, off one card whose own text never says "life", with no printed line on your screen warning you first.` | deck126 seq 116 — two Staffs out, opponent at 10 (five upkeeps from dead), Emrakul cast tapping 15 of 18 sources; opponent to 40 and the game ran to turn 54. The existing `LIFE-TO-DAMAGE CONVERTER` guard could not fire: that string printed **0 times** in all 673 decisions |
| 6 | The `LIFE-TO-DAMAGE CONVERTER` Emrakul paragraph compressed 5 lines → 3 (same rule, same trigger strings) | bytes; its trigger string printed 0 of 673 windows in this corpus, so it earns fewer lines than rule 5 |
| 7 | `=== SITUATIONS ===` (3 worked examples) deleted; Emrakul's attack/block bullet shortened one line | bytes; each deleted example restated a rule printed verbatim above it (Elixir-at-library-16, Revelation stop (3), the hold row), and the hold-row example did not prevent the 15 misses in E-4 |

KEEP, explicitly: the tap-out gate arithmetic (no misread of a `{leaves N …}` clause in 673
decisions), the counterspell type/power table (every counter this corpus spent was on-list), the
Revelation size floor and X-menu ceiling (no deck-out in six games, including two that ran past turn
50), CLEANUP DISCARD (54 discard windows, no keep this reader would fault), and MULLIGAN.

## 4. Not checked
The opponent seats were read only where deck 125's own log was ambiguous (deck152 turn 25, deck126
turn 39). I did not audit the `reveal`, `attackers`/`blocks` seams (deck 125 has no attackers in five
of six games), did not verify any card besides Tribute to Hunger against Scryfall, did not measure
wave 69's latency baseline, and made no general-guide or skill proposals — nothing in this deck's
evidence generalises past this guide.
