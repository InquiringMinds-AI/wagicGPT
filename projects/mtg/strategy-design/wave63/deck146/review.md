# Wave-63 per-deck review — deck 146 (Orzhov Dungeons)

Corpus `matchups-20260905-191148`, binary `98d13050f`. Six games, seat record **2-4**.
All seq citations are the deck-146 seat's own translog unless the file is named.
Files (abbreviated by their pointer): `...deck146-0x560d7e397fe0-vs-...deck130`,
`-0x557a4362f0e0-vs-...deck125`, `-0x559331e43120-vs-...deck126`,
`-0x557a8d9da180-vs-...deck162`, `-0x55f4f0f58220-vs-...deck123`,
`-0x55bf32e940e0-vs-...deck152`. 472 model windows, 1 fallback, 0 hangs, 0 crashes.

---

## 1. Game by game — the deciding decisions

### 146 v 130 — LOSS, 0/20, turn 18 (16 asks total)
Land destruction, not a guide effect. Mulliganed to 5 (seq 2-4), bottomed Kaya + Soul Shatter
(seq 5). Opponent Stone Rain'd the Pathway (turn 6), Molten Rain'd a Plains (turn 8) and a Hive
(turn 16); the seat made its 6th land on turn 17. Every creature it resolved was answered by a
Pyrite Spellbomb the same turn (turn 11 Adventurer, turn 14 Silencer). No attackers window ever
arose. The one live decision worth reading is **seq 14** (the Silencer name choice) — see MED-3.

### 146 v 125 — LOSS, 19/94, turn 74 (306 records, the corpus's longest)
Opponent held at **10 life from turn 11 to turn 17** and the seat could not close: it had **zero
creatures on the battlefield** from turn 17 onward (seq 40, 47, 55 all render
`Your battlefield (7 permanents ... of which 0 are creatures`) against Supreme Verdict / Final
Judgment / Path to Exile. Acererak was cast repeatedly and bounced itself every time (the guide's
own §500 teach, correctly applied); the seat's only bodies came from dungeon Goblin tokens. The
deciding stretch is turns 11-25 with **no attackers window at all** — nothing to attack with,
not a declined attack. Elixir of Immortality then made the game unwinnable (seq 253/260/268 plans
say so explicitly). Not a guide failure; a deck-vs-deck matchup the guide cannot fix.
One `unparsed_reply` at **seq 154** (degenerate whitespace/CJK output), recovered at seq 155.

### 146 v 126 — WIN, 19/-8, turn 15
Clean, guide-conformant. Curve out, **seq 17** Vanishing Verse on Sanguine Bond the turn it
resolved, **seq 25** Kaya -3 on Staff of Nin, **seq 26/33** attack with the whole board. Nothing
to report against the guide.

### 146 v 123 — WIN, 21/-2, turn 17
Same shape. **seq 32-33** Vanishing Verse on Intruder Alarm (the opponent's engine), **seq 22/25/30**
three Lolth -3 activations for six Spiders, **seq 39** lethal swing. The mode menu at **seq 35-37**
(Silverquill Command, `creature gains 3/3 and return creature`) was read and used correctly.

### 146 v 162 — LOSS, 0/9, turn 15
Killed by its own draws. Opponent assembled 2x Underworld Dreams + 2x Dictate of Kruphix +
Teferi's Puzzle Box; life ran 20-16-14-10-7-6-0. The seat's only answer that landed was
**seq 31-32** (Vanishing Verse on Underworld Dreams #1, turn 15) and Kaya -3 on Fate Unraveler
(**seq 21**, turn 11) — it did not sit on removal, it drew it late. By **seq 33** the prompt
correctly rendered `ON THE STACK: 19 damage to you - you would be at -13; that would KILL you`
with only a dead Soul Shatter row on the menu: the game was already over and the render said so
honestly. The window that could have mattered was turn 14, when Teferi's Puzzle Box entered —
see MED-4 for what the board line did not say about it.

### 146 v 152 — LOSS, 0/10, turn 23. **This one was self-inflicted and is the wave's evidence.**
At **seq 45** (turn 23, main 1, life **2**) the seat activated Lolth `+0: draw card and lose life`
→ life 1. At **seq 47** it cast Silverquill Command, and at **seq 48** chose mode 5,
`return creature and you draw` → **life 0, dead**. Its own PLAN on that same reply reads:

> `Total power is 3+2+2+2+3+3 = 15. Opponent has 10 life. This attack is lethal. Win the game.`

It had lethal on board, in its own hand-written arithmetic, and paid its last two life away in
main 1 before ever reaching the attack step. The Command mode label says only "you draw"; the
engine script is `draw:1 controller && life:-1 controller` (`borderline.txt:102744`; Scryfall
agrees: *"Target player draws a card and loses 1 life"*). **The life cost is not on the row.**
Earlier in the same game the blocks were good (**seq 22**, **seq 33** both survived correctly off
the engine's printed 1-on-1 verdicts), and **seq 20/26** are HIGH-2's repro.

---

## 2. Engine / interface / card items

### HIGH-1 — modal `choice` rows carry no magnitude and no lethality verdict; the seat killed itself on one
`146v152` **seq 48**, life 1, the whole rendered menu:

```
Choose an option for Silverquill Command:
1. creature gains 3/3 and return creature
...
5. return creature and you draw
```

Every other menu the engine prints carries a live verdict — cast rows get `{right now: drains N}`,
ability rows get `they would be at K; THIS WINS THE GAME`, the stack line gets `that would KILL
you`. Mode rows get the engine's bare `name(...)` label, and that label is an *omission*, not a
summary: `name(Return creature and you draw)` drops `life:-1 controller`. Under the trust
doctrine the model owes the surface belief, so it read "you draw" as free, wrote a lethal-attack
plan, and died before combat. The card's full text IS present on the *cast* row one window
earlier (seq 47 `{card text: "Choose two -- ..."}`) — the mode window is where it is lost.
This is R293/D1's class (`pay 3 life` arrival row with no lethality verdict) on a surface that
item did not cover.
**Fix**: run the same evaluator the cast rows use over each mode's `auto=` clause and ride the
result on the row — at minimum every `life:` delta to the controller, with
`- THIS KILLS YOU` whenever the delta reaches the seat's life. Same treatment is owed to
`146v152` **seq 45**, `+0: draw card and lose life with Lolth, Spider Queen` at 2 life: the label
names the loss but not the magnitude and not the floor.
PARSETEST shapes: a mode row with a controller life cost, one with an opponent life cost (must
NOT carry the seat-kill tag), one at exactly lethal, one at zero.

### HIGH-2 — the GANG BLOCK verdict is suppressed exactly on first-strike / deathtouch attackers
`146v152` **seq 20**, verbatim:

```
A1. Triumphant Adventurer (1/1) [first strike, deathtouch] [held back, it CANNOT block: Sigarda,
Champion of Light (flying)] [their untapped blockers: Brutal Cathar (3/3) (you kill it, your
attacker lives); Luminarch Aspirant (2/2) (you kill it, your attacker lives); Intrepid Adversary
(4/2) (you kill it, your attacker lives)]
```

Three candidates, three friendly verdicts, **no `GANG BLOCK:` clause** — any two of them kill the
1/1 after first strike. The reply is the trap verbatim: `ATTACK: A1 / PLAN: Attack with Triumphant
Adventurer to kill Brutal Cathar.` Repeats at **seq 26** (two candidates, both "you kill it, your
attacker lives", no verdict). Corpus-wide for this seat: 7 occurrences of the string `GANG BLOCK:`,
**all 7 are the boilerplate legend paragraph**; 0 live verdicts.

Cause, read from `projects/mtg/src/AIPlayerGPT.cpp:36702-36721`: `need` is accumulated from raw
`gangPowers` against `attackers[j]->toughness`, and the code's own comment states the premise —
*"need<2 means a lone blocker already kills it and the listed 1-on-1 results say so."* Against a
1/1 first-strike deathtoucher, Brutal Cathar's power 3 ≥ toughness 1, so `need = 1` and
`gangBlockPriceTag` returns "" at its `need < 2` guard (`:4813`) — while the printed 1-on-1
results, computed by `combatTradePreviewStats`, correctly say the attacker LIVES. Two
computations disagree about the same board, and the one that goes silent is the one that would
have stopped the attack. Lane AB's prediction 6 is about ORDER and is not falsified; this is a
different hole in the same item, and it is deck146's wave-62 HIGH-2 recurring in a new form.
**Fix**: derive `need` by accumulating candidates until `combatTradePreviewStats` reports the
attacker DEAD, not until raw power reaches toughness. PARSETEST: a first-strike attacker, a
deathtouch attacker, an indestructible attacker, and a vanilla control that must stay silent.

### MED-3 — the "choose a card NAME" menu offers token names and ranks rows by the zones the effect cannot reach
`146v130` **seq 14**, Silverquill Silencer (`borderline.txt:102765`, Scryfall agrees: *"Whenever
an opponent casts a spell with the chosen name"*). The rendered rows:

```
1. Goblin {visible now: 3 on their battlefield}
2. Siege-Gang Commander {visible now: 1 on their battlefield}
3. Pyrite Spellbomb {visible now: 1 on their battlefield, 1 in their graveyard}
```

Two problems. (a) **Row 1 is dead**: "Goblin" is a Siege-Gang token, not in deck130's list
(`Res/ai/baka/deck130.txt`) — a token is never *cast*, so naming it can never trigger. It is
offered first, with the loudest tag. (b) The only annotation offered is public-zone visibility,
and for a *cast*-triggered effect a card on the battlefield or in the graveyard is **negative**
evidence, yet it reads as the confirmed row. The model duly wrote
`PLAN: naming Pyrite Spellbomb to punish their artifact recursion` — the battlefield copy was
sacrificed the very next upkeep to kill the Silencer with no penalty. (The pick happened to be
sound — 2 of 4 Spellbombs were still hidden — but the render did not say that and the model did
not reason it.)
**Fix**: filter token names out of `chooseaname` candidates, and replace `{visible now: N}` with
the number the effect actually keys on — copies still in hidden zones (opponent decklist total
minus visible), which the engine already knows because it renders that decklist in the system
prompt.

### MED-4 — a permanent's effect text is truncated at exactly the clause a lethal forecast is built from
`146v162` **seq 33**, opponent battlefield line:

```
Teferi's Puzzle Box {4} [artifact] {effect: "At the beginning of each player's draw step, that
player puts the cards in their hand on the bottom of their library in any order, then..."}
```

The truncated tail is *"...then draws that many cards"* — the clause that turns a 8-card hand into
8 draws, i.e. 8 damage through Underworld Dreams. The `DRAW FORECAST` block does fold it in
correctly one line later (`Teferi's Puzzle Box: your hand size 8`), so the seat was not misled at
that instant; but the board line on its own is a silent omission of the mechanism, and the window
where it would have mattered (turn 14, when the Box entered and the seat was choosing what to
hold) had no forecast against it yet. Silent omissions are the class the trust doctrine calls
worse than wrong text.
**Fix**: never truncate an effect string mid-clause; truncate on a sentence boundary or widen the
budget for permanents the forecast blocks already read.

### MED-5 — self-targeting loyalty rows dominate a menu with no collapse
`146v152` **seq 45**: 12 rows, of which **8** are
`-3: exile non-land permanent with Kaya the Inexorable targeting <one of the seat's own
permanents> {this hits YOUR permanent}` — the opponent controlled 0 nonland permanents, so every
legal `-3` target was the seat's own. The rows are legal and must stay reachable, but eight
near-identical self-harm rows at ~330 bytes each is ~2.6 KB of the decision window spent on a
line the pilot will never take. Same shape as the E5 manland collapse.
**Fix**: collapse an all-self-targeting target set to one row plus a named list, the way the
blockers tag collapses on a wide board.

### LOW-6 — E9's crack-back relief clause still never fires
43 `CRACK-BACK` renders across this seat's six games, **0** occurrences of `relief` in any form.
Consistent with the wave-62 finding that the gate or the emitter is wrong; no new information,
recorded only as a second independent count.

### LOW-7 — E6's latch is now auditable, and correct here
12 records carry `latched_line` verbatim (`146v125` seq 24/70/79/92/150/187/212/303, `146v162`
seq 31/32/33, `146v152` seq 30), `latched_line_in_plan` is **false in 12 of 12**, and
`reply_trimmed_bytes` no longer hides the deciding line (seq 212 trims 3,502 B and still records
`CHOICE: 6 (Hold priority...)`). Exactly one `answer_replaced: true` — `146v125` **seq 92**,
where the model wrote `CHOICE: 1 (becomes beholder)`, reasoned itself out of it in plain prose
and re-emitted `CHOICE: 2 (Hold priority)`; the latch took the model's own correction. deck146's
wave-62 HIGH-1 is discharged for this seat.

### LOW-8 — the hold latch is doing real work in a long grind
`146v125` closes with `hold_windows_skipped: 463` against 6 taken hold rows over 74 turns;
`identical_ask_answers_reserved: 13`, `identical_option_asks_resolved: 1`. Whatever E10 concludes
about the promise's wording, the mechanism is saving hundreds of round trips in exactly the game
shape that needs it.

---

## 3. Guide verdict — **KEEP as is**

`deck146_strategy.txt` is 70,986 B, at the top of the 41-71 KB pool band. No edit; the guide was
followed, and where it was followed it produced correct play.

- **Mulligan discipline held.** Three untouched sevens kept (seq 2 in the 126/123/152 games), and
  the one hand that shipped (`146v130` seq 2-4) shipped twice to a hand with zero lands — the
  STEP-2 ship list's first entry — then kept at (keeping 5) and bottomed the two uncastables.
  Zero instances of the "no white source" ship the guide was written against.
- **Removal was not sat on.** Vanishing Verse fired the turn it had a target in five of six games
  (`146v126` seq 17 Sanguine Bond, `146v123` seq 32 Intruder Alarm, `146v162` seq 31 Underworld
  Dreams, `146v152` seq 14 Augur of Autumn, `146v130` seq 11 Dwarven Blastminer).
- **The Acererak teach (§500-511) worked.** Every Acererak cast was treated as a repeatable
  venture and never as a body; no plan in the corpus asserted a 5/5 on the battlefield.
- **The dungeon teach (§512-528) was executed verbatim** — Lost Mine of Phandelver named at every
  three-dungeon list, 8 of 8, including after completion.
- **The one game-losing decision the guide could own** is `146v152` seq 45 (Lolth `+0` at 2 life
  with lethal on board). Its sibling, seq 48, is HIGH-1 — a render omission, and the trust
  doctrine forbids teaching the guide to hedge against a surface that is being fixed. The
  remaining half is one seq in one game, and the general corpus already carries the adopted
  principle it belongs to (*"the engine renders a lethality verdict on every other menu it
  prints"*). Buying it guide text at the band ceiling would cost more than it pays.

No `strategy.txt`, no `general-proposals.md`, no `skill-proposals.md`: HIGH-1, HIGH-2, MED-3,
MED-4 and MED-5 are all engine-layer, and a general-guide teach on any of them would restate an
adopted engine rule rather than add one.

---

## 4. What I did NOT check

- I read the **opponent seats only for narration context** in `146v130`; I did not trace the other
  five opponent translogs, so any defect visible only from the far seat is unexamined here.
- **I did not run the game, the suite, or PARSETEST**, and I did not build anything. HIGH-2's
  cause is read from `AIPlayerGPT.cpp:36702-36721` + `:4810-4827` by inspection only — the `need`
  arithmetic is not traced under a debugger and the claim that `combatTradePreviewStats` and the
  gang walk disagree is argued from the two code paths and the rendered contradiction, not
  measured.
- I did not adjudicate the lane predictions (that is the engine seat's deliverable); the string
  counts in this file are **this seat's 472 prompts only**: `best case with every blocker
  assigned` 2, `one legal assignment that reaches it` 2 (both carrying `chosen for your
  blockers`), `no block saves you` 0, `the assignment that lets in the LEAST damage is` 0,
  `(N if all block)` 0, `chains without limit until you are at 0` 0, `both halves of their life
  LOOP` 0. Every one of those zeros is **no window arose in this seat**, not a failure — this deck
  saw only 4 blockers windows and no closed life loop all wave.
- **The hung games and the reruns are not in this seat's set** (152v162, 152v130) — I read
  nothing from `matchups-20260905-214946` / `-215345`, and I found no deck-146 game stuck at
  Blockers with a calm stack, so I add nothing to `softlock-blockers-1788650768.md`.
- Card facts verified: Silverquill Command and Silverquill Silencer against **both** the primitive
  (`borderline.txt:102739-102749`, `:102764-102772`) and Scryfall — they agree. Acererak, Nadaar,
  Lolth and Kaya were read off the engine render only, not re-verified against Scryfall.
