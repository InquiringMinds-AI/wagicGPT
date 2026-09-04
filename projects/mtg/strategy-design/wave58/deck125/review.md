# Wave-58 review — deck125 (Revelation Fracture, UW draw-go control)

Corpus `matchups-20260904-015059`. Six games, 3-3. Seat files are named
`<epoch>-ai_baka_deck125-<ptr>-vs-ai_baka_deck<opp>.jsonl`; below they are cited as
`125v<opp> sN`. Every card fact was checked against `bin/Res/sets/primitives/mtg.txt`.

## 1. Game by game, and the decisions that decided them

| game | result | turns | the decision |
|---|---|---|---|
| 125v152 | WIN 21 / -6 | 45 | Emrakul row taken at s111 on a board with no converter line; it attacked on the extra turn. Correct, and the guide's #1 carve-out is why. |
| 125v130 | WIN 11 / 0 | 57 | Textbook: Staff cast at s29, then a ping at every upkeep (s30 through s139), counters held, floods discarded. Nothing to fix. |
| 125v123 | WIN 35 / 0 | 40 | Same shape — Staff at turn 13, ping every upkeep, 19 priority windows all spent on the ping. |
| 125v146 | LOSS -4 / 17 | 31 | **Lightmine Field declined four times off a live row** (s38, s39 at 27 life; s41, s42 at 20 life). Guide item, below. |
| 125v126 | LOSS 0 / 39 | 16 | **A false clause the pilot wrote into its own PLAN line** froze it for five turns (s44, s48). Guide item, below. |
| 125v162 | LOSS 0 / 20 | 15 | Mana screw (2 lands until turn 9 on a guide-legal keep, s1), then at 3 life it answered option 1 off an X row reading `NET -2 life for this cast` (s41). Render + guide item, below. |

**125v146 — the deciding decision.** At s38/s39 (turn 24, 27 life) and again at s41/s42 (turn 26,
20 life) the menu carried

```
1. Cast Lightmine Field {2}{w}{w} {right now: they control 2 creatures able to attack - deals 2 to
   each if all 2 attack} {leaves 6 of your 10 untapped mana sources untapped}
```

and the opponent line read `Spider #1 (2/1) [reach, menace]; Spider #2 (2/1) [reach, menace]`.
Toughness 1 each: 2 damage kills both, for four mana the seat was not using. The pilot answered
`Cast nothing right now` all four times, because the guide's floor read "do not cast while that
count is below 3". Lolth, Spider Queen kept making Spiders; the Field was finally cast on turn 28
at 15 life (s44) and the seat died at -4 three turns later. **STRATEGY / guide.** The primitive
(`auto=@each blockers:foreach(creature[attacking]|Battlefield) damage:1
all(creature[attacking]|Battlefield)`) and Scryfall agree with the row: the count IS the damage.

**125v126 — the deciding decision.** The seat cast nothing for sixteen turns and died in one
event (20 -> 0 while the opponent went 20 -> 39: Staff of Nin ping into Sanguine Bond + Exquisite
Blood). At s44 (turn 15) and s48 (turn 16) the menu offered
`Cast Sphinx's Revelation ... {X pricing: max affordable X=5 (8 mana total)}` and
`Cast Supreme Verdict ... {right now: destroys 2 of their creatures}`, and the pilot's echoed PLAN
read: *"Do not cast Supreme Verdict (destroys 0 attackers) or Sphinx's Revelation (X=2 is too low,
and Sanguine Bond converts any gain to damage)."* All three clauses were false at that window:
the row said 2, the row said X=5, and Sanguine Bond (`@lifeof(player) ... :life:-thatmuch
opponent`) fires only on **its controller's** lifegain. **PERCEPTION** — the prompt's own converter
paragraph states the direction correctly two sentences earlier, so the surface was true and the
pilot misread it; its closing clause ("ANY nonzero payment on a tag above is fatal") is the part
that generalises badly, and that half is a render item (I2). By turn 16 the board was already
lethal, so the honest claim is that Revelation for 5 at s44 was the last real chance, not a
guaranteed save.

**125v162.** Mulligan at s1 was guide-legal (2 lands, cheapest spell MV 3, one land away). The
third land arrived on turn 9. Reading that as a guide effect would be reading variance. The one
real error is s41 (below, I3).

## 2. Engine / interface / card items

**HIGH — `empty_reply` is a ~2.5-second deadline, not a refusal.** All **89 of 89** `empty_reply`
fallbacks in the whole corpus have `latency_ms` between **2502 and 2558** — a 56 ms spread over 89
events, which is a timer, not a distribution. Successful replies on the same seats have medians of
2.7 s to 30 s and run to 143 s (`125v126` max 142859 ms). deck125 lost 14 decisions this way,
five of them consecutively on one turn (`125v126` s27, s30, s32, s34, s36, all turn 12, all with a
`Cast Sphinx's Revelation ... max affordable X=2` row on the menu). Repro: any of those seqs — the
record has `reply: ""`, `coded_answers: 0`, `latency_ms: 2505-2558`. The classification is also
misleading: the field says the model refused when the evidence says the client hung up.

**HIGH — the cleanup-discard `PUT:` parser refuses the parenthetical gloss that every `CHOICE:`
ask demands.** `125v130` s83, turn 41. The prompt reads:

```
On the FIRST line write PUT: followed by the 2 card numbers you discard, comma-separated
(e.g. "PUT: 2, 5")
```

The reply was `PUT: 9, 1 (Supreme Verdict, Fall of the Gavel)` — the exact required form plus the
name gloss that every `CHOICE:` line in the same protocol *requires*
(`e.g. "CHOICE: 1 (Cast Supreme Verdict)"`). It was scored `unparsed_reply` with
`coded_answers: 1` and `post_answer_overrun: 171`, and the heuristic picked the discards. 71 `PUT:`
asks in the corpus, 2 unparsed, and this is 1 of the 2 (the other, `125v152` s119, is genuinely
degenerate output). Fix: strip a trailing `(...)` from a `PUT:` line before parsing the numbers.

**MED — the Lightmine Field row prints the damage but not the kill count, while every sweeper row
on the same menu prints its kill count.** `125v146` s38:
`{right now: they control 2 creatures able to attack - deals 2 to each if all 2 attack}` sits two
rows from `Cast Final Judgment ... {right now: exiles 2 of their creatures}`. The evaluator already
walks the able-to-attack set; printing `- kills 2 of those 2 (toughness 1, 1)` would have decided
this window on its own. 43 Lightmine rows in deck125's corpus, 1 cast.

**MED — the `[<- best X for this cast]` / `[<- largest affordable X]` marker is computed from mana
alone and will recommend a row whose own text says it is lethal.** `125v162` s41, 3 life:

```
1. X = 2 {X pricing: X=2 - you gain 2 life and draw 2 cards; the opponent's Underworld Dreams,
   Ob Nixilis, the Hate-Twisted punish every draw, so those draws cost you 4 life - NET -2 life
   for this cast} {leaves 0 of your 5 floating mana unspent} [<- largest affordable X - X=2 gains
   2 life and draws 2 cards; no listed X does more]
```

`no listed X does more` is false as written when the row beside it prints a negative NET, and the
pilot took option 1. 11 X menus in the corpus, 1 carried a NET tail, and option 1 was taken on it.
Ask: suppress the marker, or move it to the largest X whose NET leaves life above 0.

**MED (I2) — the LIFE-TO-DAMAGE CONVERTER paragraph's closing clause over-generalises.** 57 windows
in the corpus print it. Its body is correct ("a converter of THEIRS turns every life THEY gain ...
a converter of YOURS does the same to life YOU gain"), but it closes "Any life they gain, or any
life you lose, chains until YOU are at 0 - so ANY nonzero payment on a tag above is fatal". A
reader generalised that to its own lifegain (`125v126` s44, s48). Ask: one clause —
`(life YOU gain is not converted by a converter of theirs; only life you PAY chains)`.

**LOW — a 900-second wall miss with no ask recorded.** `125v130` s19, turn 16, kind `wall_miss`,
`fallback: wall_miss_unrecorded`, `latency_ms: 900020`. One event; noted for the engine seat's
census, not diagnosed here.

**Not a defect, checked:** deck125 gets no priority window on opponent turns when it holds no
instant (e.g. `125v146` turns 21-31 with only Emrakul and a land in hand) — that is correct, not a
missing window. The Lightmine primitive fires at declare-blockers rather than on the attack
trigger; the damage still lands before combat damage, so nothing observable differs.

## 3. Guide verdict: EDIT

`wave58/deck125/strategy.txt`, started from the live
`bin/Res/ai/baka/deck125_strategy.txt`. 70766 -> **70801 bytes** (inside the 41-71 KB band; the
additions are paid for by trims of aged tallies and worked examples, per skill #302/#323). Skill
#320 check run first: the guide carries no `REMOVE THIS RULE WHEN` line, so nothing came due.

**E1. Lightmine Field: the count-of-3 floor becomes a kill test.** Section header
`IT ONLY PUNISHES A CROWD` -> `THE COUNT IS THE DAMAGE`.
- before: *"Do not cast Lightmine Field while that count is below 3 - not at 0, not at 1, not at 2,
  in either main phase, however much spare mana you have."* and the opening *"ONE attacker takes 1
  damage, which kills nothing however large it is."*
- after: a three-branch gate on the row's N against the printed TOUGHNESS of the creatures the row
  counted — N=0 is always a decline; N>=1 with any counted creature of toughness <= N is a cast,
  subject to the existing tap-out gate; N>=1 with everything tougher is a hold. Plus the census
  (43 rows, 1 cast) and the board that paid for it.
- paid for by: `125v146` s38, s39, s41, s42 (and the cast at s44 that came six turns too late).
- The old floor was not merely conservative, it was wrong on the card: 1 damage does kill a X/1.

**E2. The PLAN line is the canned sentence and nothing else.** Added to section #1, after the
sentence that tells the pilot to write the plan word for word: never append a per-card verdict,
because the append is echoed back as YOUR PLAN and outlives the board that made it; and
"when a row and your own PLAN line disagree, THE ROW IS TRUE".
- paid for by: `125v126` s44 and s48 (two false clauses re-written verbatim five turns after they
  stopped being true), plus `125v146`, where "ping their face every turn" rode the plan for
  eighteen turns with no Staff of Nin on the battlefield.

**E3. The converter runs one way, and it is not your lifegain.** Added to section #1 beside the
Sanguine Bond / Exquisite Blood paragraph, with the primitive's direction spelled out and the
prompt's own sentence quoted.
- paid for by: `125v126` s44, s48.

**E4. An X-menu NET floor.** The "two things that can lower X" bullet now names the
`NET -K life for this cast` tail, says the `[<- largest affordable X]` marker is computed from mana
alone, and sets the floor: announce the largest X whose NET leaves printed life above 0, X = 0 if
none does.
- paid for by: `125v162` s41. (The draw-is-damage stop should have kept the seat off that menu at
  s40 — that rule was already in the guide and was broken; the floor is the second line of defence,
  not a replacement.)

**E5. The bottoming branch, walked for the first time and broken.** The mulligan section's
"no bottoming ask ever reached" sentence is replaced by what happened: `125v152` s3, the pilot
bottomed a **Plains** out of a three-land six while two `(copy 2 of 2 in your hand)` duplicates sat
on the list — against the guide's own order, and it cost the untapped land, since Glacial Fortress
"enters tapped unless you control a Plains or an Island". (The seat won that game anyway.)

Consistency edits carried with the above: the cleanup section's cross-reference to "the count that
makes a Lightmine Field and a sweeper dead" now says a Lightmine Field is dead only at a count of
0, and the Staff-gate's list of what the gate must still stop keeps the Lightmine "leaves 0/1"
clause intact.

**What I am NOT changing, and why.** Section #0 (land drop), the Staff clock and its tap-out gate,
the counter-on-sight list, the sweeper dead-row rule, the cleanup order, and the Emrakul carve-out
all held: three wins are the guide executing, the Emrakul row was taken correctly at `125v152`
s111 on its only live board, and every land window this corpus was answered with a land. The
Emrakul carve-out itself is **untested this corpus** — the one game that printed a converter line
(`125v126`, 33 windows) never printed an Emrakul row.

## 4. What I did not check

The opponent seats were read only where they explained deck125's board (`125v126`'s Idyllic Tutor
line, `125v146`'s Lolth). I did not open the hung game (`152v126`) or its rerun, did not read the
`.stderr` / `WAGIC_BLOCKLOG` output, and did not adjudicate the lane A-E predictions — those are the
engine seat's. I did not verify the empty_reply timer against the source; the claim rests on the
89/89 latency band alone. I ran no game and built nothing.
