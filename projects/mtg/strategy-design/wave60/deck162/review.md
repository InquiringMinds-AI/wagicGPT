# wave-60 per-deck review — deck162 "Draw and Die!" (UB forced-draw punish)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-171140/`, seat files
`*-ai_baka_deck162-0x*.jsonl` (6 games, all natural, none hung/crashed).
Record 3-3. Every card fact below was checked against
`bin/Res/sets/primitives/borderline.txt`; where I could not settle a claim I say so.

## 1. Game by game

| opp | result | end | deciding decisions |
|---|---|---|---|
| 126 | WIN 20-0, T14 | clean | Engine on curve (Mine T4, Master T6, Fate Unraveler T8, Underworld Dreams T10, Forced Fruition + Shield Sphere T12), then Peer into the Abyss at seq 17-18 targeting the opponent (7 life, 9 draws x Underworld Dreams) for exact lethal. One `named_row_reask` at seq 6 (below), recovered seq 7-8. |
| 130 | WIN 9-0, T14 | clean | Two punishers + 2 Howling Mines + Dictate by T13; opponent died on their own draw step (`DRAW FORECAST`: 4 draws x 2 = 8). seq 18 `ATTACK: none` was correct — but see MED-1 and HIGH-1: the reasoning that produced it was built on casting Ob Nixilis {3}{b}{b} in MP2 with `Mana available: 3 total`. |
| 125 | WIN 20-0, T15 | clean | Underworld Dreams T6, Dictate T8, Ob Nixilis T10, 2nd Underworld Dreams T12/T14, Howling Mine T14, Master attacks. One 900-second `wall_miss` at seq 16 (MED-2), recovered seq 17. |
| 152 | LOSS -18/11, T13 | race | Not a guide failure: from seq 14 on the cast menus offered exactly ONE castable card per window (seq 14/18 Dictate only, seq 20 Underworld Dreams only). The render told it plainly and repeatedly that it was dead (seq 15/16 `CRACK-BACK ... you would be at -1; that would KILL you`, seq 17 `INCOMING ... this KILLS you`, seq 19/20/21 `... at -18`). It had no blocker and no removal it could pay for. Aggro deck out-raced the engine by ~2 turns. |
| 123 | LOSS -118/3, T14 | opponent combo | Model got 123 from 20 to 3 on Master of the Feast beats plus Fate Unraveler triggers. Opponent tutored Intruder Alarm (T6), landed it T8, Damnation'd T10, then T12 played Thraben Doomsayer + Lightning Greaves and made 45 tokens in one turn (see the seq 18 narration: `Create human with Thraben Doomsayer (x23)` then `(x21)`). At seq 18 the model faced 47 attackers / 140 damage with one untapped 3/4; it blocked A1 and wrote "There is no way to survive this combat." Correct read. The Doomsayer had shroud from Greaves, so Ob Nixilis's -2 could not have answered it even if it had been castable (it was not — 5 mana, 4 sources at T13). Deck has no sweeper. Nothing to fault. |
| 146 | LOSS 0/7, T15 | one real misplay | Model was ground down 20→3 by a wide W/B board while its punishers were picked off (Vanishing Verse on Underworld Dreams; Ob Nixilis gone by T14). At seq 23, at 3 life with `CRACK-BACK NEXT TURN ... you would be at -13; that would KILL you`, it spent all 7 mana on **Peer into the Abyss** with **zero** converters on its battlefield, writing "With Underworld Dreams on the battlefield, each of those 20 draws deals 1 damage, totaling 20 damage." Underworld Dreams was NOT on its battlefield (the same prompt's battlefield line lists Drowned Catacomb, Fog Bank, Howling Mine, 4 Islands, Swamp, Underground Sea). It handed the opponent 20 cards for 7 life loss and died next combat. See HIGH-2 — this is a false *belief*, and the row the belief was formed on is the one row in that menu that does not print the fact. |

## 2. Engine / interface / card items

### HIGH-1 — the hand listing has no affordability marker, and the model plans and answers with cards it cannot pay for
Cast ROWS carry exact mana accounting (`leaves 0 of your 3 untapped mana sources untapped - casting this taps you out`, `row 3 needs more mana sources than the 4 this leaves`). The `Your hand (N cards):` line carries the cost but never says whether it is payable now. Three independent symptoms in six games:

- `1788559908-ai_baka_deck162-0x55a47158b570-vs-ai_baka_deck123.jsonl` seq 6.
  `Mana available: 3 total`, hand contains `Fate Unraveler (copy 1 of 2 in your hand) {3}{b} (3/4) [creature]` (4 mana). Fate Unraveler is correctly absent from the 3-row menu. Reply: `CHOICE: 3 (Cast Fate Unraveler)` — index **3 is the HOLD row**. Caught as `named_row_reask`, recovered at seq 7-8.
- `1788559904-...-vs-ai_baka_deck126.jsonl` seq 6 — identical shape, identical card, identical index-3-is-HOLD collision, same fallback and recovery.
- `1788559919-...-vs-ai_baka_deck130.jsonl` seq 18 (turn 13, opponent at 2). `Mana available: 3 total`; hand has `Ob Nixilis, the Hate-Twisted {3}{b}{b}` = 5 mana. The model wrote a 900-word deliberation whose whole winning line was "In Main Phase 2, you can cast Ob Nixilis... Activate -2... Opponent dies," and its `PLAN:` carried that forward. It won anyway (their draw step killed them), and at seq 19 it in fact chose the HOLD row instead of the cast it had planned.

Both of this deck's `named_row_reask` fallbacks are this one defect, and the near-miss at 130 seq 18 shows it also silently corrupts planning when no re-ask fires. The engine already computes this for the human seat (`castableForDisplay` / `castableNow`, `ManaEngine::planPayment`). Suggested repro-and-fix surface: tag each hand entry, e.g. `Ob Nixilis, the Hate-Twisted {3}{b}{b} [planeswalker] [cannot pay now: 5 needed, 3 sources]`. Restriction-first wording, per the annotation doctrine.

### HIGH-2 — Peer into the Abyss's option row omits the converter/punisher accounting that every other draw-granting row in the same menu prints
`1788559906-ai_baka_deck162-0x555a2809cb20-vs-ai_baka_deck146.jsonl` seq 23, turn 14, 3 life. The rendered menu:

- row 1 `Cast Master of the Feast ... {feeds: the opponent draws 1 extra card per turn; converters on your battlefield: 0 (nothing of yours punishes their draws or discards yet ...)}`
- row 2 `Cast Dictate of Kruphix ... {feeds: ... converters on your battlefield: 0 ...}`
- row 4 `Cast Howling Mine ... {feeds: ... converters on your battlefield: 0 ...}`
- row 3 `Cast Peer into the Abyss {4}{b}{b}{b} {right now: if you choose "the opponent": life -7, draws 20; if you choose "you": life -2, draws 21} {leaves 0 of your 7 ...} {card text: ...} {modes live right now: ...}` — **no converter clause at all.**

Peer is the single biggest mass-draw in the game and its entire value is `draws x punishers`. The magnitude machinery already evaluates the draw count and the life loss for that exact row; it stops one step short of the number that decides the cast. The deck guide's cast-order rule 2 ("Peer into the Abyss, when a punisher is already on your battlefield") gates on precisely the fact the row does not print, so the guide is asking the model to cross-reference a board line it demonstrably misread. Same gap applies to Ob Nixilis's `-2` (two draws) and to any one-shot draw grant: the `DRAW FORECAST (theirs)` header prices only the recurring draw step.
Suggested: append to the Peer row (and any `draw:N opponent` magnitude) `; your draw punishers: 0 - those 20 draws deal 0 damage` / `; your draw punishers: 2 - those 20 draws deal 40 damage`.

### MED-1 — the reply protocol's "Write nothing else" is not holding, at real inference cost
Across the 6 games' 119 model windows, 81 replies overran the protocol (`post_answer_overrun > 100`), 6 by more than 1000 characters, 46,052 wasted characters total. Worst: `...vs-ai_baka_deck130` seq 18, `post_answer_overrun` 6003 — the answer line `ATTACK: none` followed by an entire self-dialogue ("Wait, looking closer at the board state...", "Is attacking with Fate Unraveler necessary?"). Also `...vs-ai_baka_deck146` seq 16 (7524 / `post_plan_overrun` 6405) and seq 12 (2089 / 694). The parser handled all of them, so this is not correctness — it is the owner's "efficient use of its inference" axis, and the overrun counters make it measurable per-window. Note the shape: the overruns cluster on the windows where the model is *uncertain*, i.e. exactly where the answer matters.

### MED-2 — a 900-second round trip surfaced as `wall_miss`
`1788559934-ai_baka_deck162-0x561f35f8a2c0-vs-ai_baka_deck125.jsonl` seq 16: `kind: wall_miss`, `fallback: wall_miss_unrecorded`, `latency_ms: 900027`, `options: 0`, `log_window_kind: empty_stack_pass`, `plan_echo_count: 2`. The `recovery` record at seq 17 shows the seat recovered and seq 18 answered normally. Mulligan windows in this deck also ran 118-150 s (seq 1 of the 126/125 games). Flagging for the engine seat's transport census rather than adjudicating it here.

### LOW-1 — a lethal-combat line still prints a reassuring parenthetical
`...vs-ai_baka_deck123` seq 18:
`INCOMING THIS COMBAT: 47 attackers, 140 unblocked damage - you would be at -120; this KILLS you (your creatures may legally block every attacker in that total)`
with a single untapped blocker (B1 Fate Unraveler) against 47 attackers. `incomingCombatLine` (src/AIPlayerGPT.cpp:16281) means only "no attacker is individually unblockable", which is true, but at 1-blocker-vs-47 the clause carries no decision value and sits directly after "this KILLS you". The model was not misled here. Low priority; the surrounding wording was already hardened in wave 59 (#W59-I), so this is a note, not a re-open.

### Verified NOT a defect (recording it so nobody re-opens it)
Peer into the Abyss's magnitude rounding is correct. `...vs-ai_baka_deck146` seq 23 rendered `life -7` against an opponent at 13; the primitive is
`auto=choice name(Target opponent) draw:halfuptype:*:opponentlibrary opponent && life:-halfupopponentlifetotal opponent` (`borderline.txt:82571`), text "Round up each time", and Scryfall agrees. The opponent's final life of 7 (not 6) is because they gained 1 from the `dark pool` dungeon room after Peer resolved — visible in the deck146 seat's own seq 40/41.

## 3. Guide verdict: KEEP as is

`bin/Res/ai/baka/deck162_strategy.txt` (52,982 bytes, inside the 41-71 KB band) is not what cost this deck its three losses, and it is what earned at least two of the three wins.

- The engine was deployed on curve in all six games and the punisher-before-engine ordering held: Underworld Dreams / Fate Unraveler / Ob Nixilis went down first or as the first affordable body in every game, and every Howling Mine / Dictate / Puzzle Box cast after the first Mine came with a converter already out — the exact discipline of the guide's cast-order list.
- Peer into the Abyss was cast twice and answered `target opponent` both times (`126` seq 18, `146` seq 24), never `target controller`. The guide's rule-5 warning is doing its job.
- The two losses to 152 and 123 were forced lines: single-castable-card menus in a race, and a 45-token Intruder Alarm + Thraben Doomsayer combo behind shroud against a deck with no sweeper. Reading either as a guide effect would be reading variance.
- The one true misplay (146 seq 23, Peer with zero converters at 3 life) is a **violation of a rule the guide already states** — cast-order rule 2, "Peer into the Abyss, when a punisher is already on your battlefield." The model did not read the rule wrong; it read the *board* wrong, on a row that omits the very fact the rule keys on. Per the perception-vs-strategy routing doctrine, guide prose loses to a surface that does not carry the fact — the fix is HIGH-2 at the render, not more prose. Adding a fourth restatement of a rule the guide already carries would be noise, and would spend guide bytes hedging against a surface instead of fixing it.

No `strategy.txt` is written for this wave.

## 4. Proposals

None. No `general-proposals.md`, no `skill-proposals.md`. Both HIGH items are engine/render work for the core loop (they cut across decks: an affordability marker on the hand line and punisher accounting on one-shot draw grants are not deck162-specific), and neither the general guide nor the strategy-writing skill would have changed a single decision in these six games. The skill's existing amendments already cover restriction-first annotation wording and the guide-loses-to-a-false-surface routing that HIGH-1 and HIGH-2 are instances of; restating them would be a proposal that restates an existing amendment.

## 5. What I did NOT check

- The opponents' seat translogs beyond the specific cross-checks named above (146's ending, 123's combo narration as seen from my seat).
- The lane-{L,M,N,O,P,Q} predictions — that is the engine seat's adjudication, not mine. I did not classify `named_row_reask` / `wall_miss_unrecorded` / `recovery` against any lane's predicted signature; I report them as observed.
- Whether the 900 s `wall_miss` was transport or server-side queueing (no `transport` field was present on that record).
- The mulligan/bottoming seams: all six games kept the opening hand at seq 1, so no bottoming window arose in this deck's corpus.
- Any counterfactual replay. I did not run the game, rebuild, or execute anything — this review is read-only over the translogs, the primitives and `src/AIPlayerGPT.cpp`.
