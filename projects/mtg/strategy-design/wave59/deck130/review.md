# Wave-59 deck130 review (Budde's Ponza — mono-red land destruction)

Corpus: `matchups-20260904-130044`, deck130 seat files (6 games, 497 decisions, 3 fallbacks,
0 crashes, 0 hangs). Record 3-3. Seat files abbreviated below by their `-0x` prefix.

## 1. Game by game

| game | file | result | deciding decisions |
|---|---|---|---|
| vs 162 | `...deck130-0x5630b76e6980-vs-...deck162` | **LOSS** t12, 0 vs 20 | **s14**: attacked Dwarven Blastminer into a tag reading `[their untapped blockers: Master of the Feast (5/5) (your attacker dies, their blocker lives)]` at 20-20. The engine died, the land plan died with it, and the seat never touched Master of the Feast again (20 -> 15 -> 8 -> 1 -> 0 on five uncontested swings + Underworld Dreams). **s21**: 900 s wall miss (below). |
| vs 146 | `...deck130-0x55dcc18cad60-vs-...deck146` | **LOSS** t26, -2 vs 2 | **s43**: at 1 life vs 2, cast Hammer of Bogardan whose own row read *both* `this KILLS you` and `WINS THE GAME`; **s44** picked the face on a row reading `they would be at -1; THIS WINS THE GAME`. Silverquill Silencer's cast trigger resolved first: seat at -2, opponent alive at 2. (The seat had correctly declined the same row at **s40**.) |
| vs 125 | `...deck130-0x55b88f0cf500-vs-...deck125` | **LOSS** t70, 3 vs 62 | Structural. Opponent held 11 -> 16 lands, 0 creatures, and gained +7/+10 per turn (`Opponent life trend` at s120/s156). The guide's face-burn release (0 creatures **and** <=2 lands) can never fire against that board, so the seat correctly latched off face damage at **s120** and passed 58 Hold rows to a decking loss. No misplay found; the deck has no answer to this matchup. |
| vs 126 | `...deck130-0x56432bf0dc80-vs-...deck126` | **WIN** t16, 19 vs 0 | **s35** `no attackers` under the WALL CHECK (Pride Guardian `(neither dies (blocking trigger: they gain 3))`) — correct and guide-compliant; **s54/55** Rorix deployed once the wall board could not stop a flier; **s67** Goblin sacrifice for exact lethal. |
| vs 123 | `...deck130-0x55e222aae9d0-vs-...deck123` | **WIN** t22, 17 vs 0 | **s26-28 / s36** two land kills resolved; **s42-43** then fired Hammer at the face at 18 life across `0 are creatures ... 3 are lands`. See §3 — this **broke** the guide's `L of 2 or fewer` release and won the game with it. |
| vs 152 | `...deck130-0x5570b56f0570-vs-...deck152` | **WIN** t30, 5 vs 0 | Textbook line: **s24-26** Starstorm at X=5 killing only theirs, **s37/57/62/68** recurring Hammer from the graveyard each upkeep, **s55/56/61/70/72** the burn chain from 14 down. |

## 2. Engine / interface / card items

### HIGH-1 — a "WINS THE GAME" verdict that is not gated on the caster surviving the cast
`...deck130-0x55dcc18cad60-vs-...deck146`, **seq 43** (identical text at seq 40), rendered row:

```
1. Cast Hammer of Bogardan {1}{r}{r} [NAMED BY THEIR Silverquill Silencer: casting this costs you 3
   life and draws them a card - you would be at -2; this KILLS you. ...] ... {kills whichever you
   target: THEIRS - Spirit Cai, Silverquill Silencer; YOURS - Goblin - and 3 to the opponent at
   life 2 WINS THE GAME}
```
and then **seq 44**, the target menu, with no death clause at all:
```
1. The opponent (player, life 2) {right now: takes 3 damage - they would be at -1; THIS WINS THE GAME}
```
The `{kills whichever you target: ...}` fold prices the *resolution* and ignores the cast-time cost
printed in the same row eight words earlier. Silverquill Silencer is faithful to Oracle
(`borderline.txt:102764-102766`, `movedto(*[chosenname]|opponentstack): life:-3 opponent`; Scryfall
agrees) — the trigger goes on the stack above the spell, the seat dies at SBA before Hammer
resolves, and the engine played it exactly that way. So the token is a **false verdict at the one
decision that ends the game**, and it is the token the seat flipped on: it declined the row at s40,
was re-asked in Main 2, and took it. Fix: the win/kill fold must subtract any cast-cost life
already priced on the same row, and print `WOULD win, but the cast cost kills you first` (or
suppress the win token) whenever `your life - cast cost <= 0`. Same rule for the target menu, which
today carries the win verdict with no knowledge of the cast row at all.
Contrast, same corpus, where the machinery gets it right: `...-0x5570b56f0570-vs-...152` seq 72
prints `{paying this costs you 1 life ... you would be at 5}` *and* `THIS WINS THE GAME` — true,
because 5 > 0. Only the cross-check is missing.

### HIGH-2 — the `wall_miss` record does not describe the decision that missed, and costs 900 s
`...deck130-0x5630b76e6980-vs-...deck162`, **seq 21**:
`kind: wall_miss`, `fallback: wall_miss_unrecorded`, `latency_ms: 900020`, `options: 0`,
`phase: "Main phase 1"`, `log_window_kind: "land_drop"`, `reply: ""`,
`transport: "curl=28,http=0,empty=1"` — while the stored `prompt` is a **Draw**-phase casting
decision with **three** options (`1. Cast Starstorm ... 2. Cast nothing right now ... 3. Hold
priority`). Three fields disagree with the prompt they are attached to, so the record cannot be
replayed. Two asks are worth separating: (a) stamp the wall_miss with the prompt and phase of the
decision that actually missed, and fill `options` from it; (b) 900 s of wall clock spent to reach
`<refused>` is 15 minutes of a 21-game run — lane H's retry ladder should cap total per-decision
wall time well below the 900 s wall and file the miss immediately at exhaustion.
Credit where due: lane H's new `transport` field is what made this diagnosable at all
(`curl=28` = operation timed out). deck130 saw **1** transport failure in 497 decisions, against
F3's wave-58 rate of 89/2270 — the K1/F3 signature does not recur here.

### MED-1 — a forced-sacrifice menu is rendered as its lifegain sub-ability, with no priced rows
`...deck130-0x56432bf0dc80-vs-...deck126`, **seq 28/29**:
```
TARGET CHOICE for Tribute to Hunger - its "gain life equal to its toughness" ability ...
1. Siege-Gang Commander (2/2) [your battlefield] - "When Siege-Gang Commander enters, ..."
2-4. Goblin #1-#3 (1/1) [your battlefield] x3
```
The card is faithful (`mtg.txt:124073-124076`; the script's own ability is named `sacrifice`), but
the header names the *lifegain* half, so the menu reads as "pick who gains life" rather than "you
must sacrifice one of these"; no row says the chosen creature dies, and no row prices the life the
opponent gains (1 per Goblin, 2 for the Commander — the log at seq 29 confirms `Opponent gained 1
life`). Every row is `[your battlefield]` and there is no pass row, which collides head-on with a
rule printed in this deck's own guide ("A ROW THAT NAMES YOUR OWN PERMANENT IS NEVER THE ANSWER ...
the answer is `CHOICE: 0 (pass)`"). Ask: head it `FORCED SACRIFICE - you must choose one` and give
each row a `{right now: this creature dies; they gain N}` clause. The seat guessed right here
(cheapest body), so this is MED, not HIGH.

### MED-2 — the decline counter observes a loop it never ends
`...deck130-0x55b88f0cf500-vs-...deck125`, **turn 67**: the same casting list was put to the seat in
seven consecutive windows (seq 202 Draw, 203 Main 1, 204 Combat begins, 205 Attackers, 206 Combat
ends, 207 Main 2), reaching `[you declined this exact list 9 times already this turn]`, before the
seat finally took the Hold row at the last window. Same shape at turn 65 (seq 191/192) and turn 44
(seq 102). The Hold row was present in all of them and the counter escalated in all of them —
nothing else changed. Cheapest engine-side fix: once the decline count on a byte-identical list
reaches 2 within a turn with no board change, latch it as if Hold had been taken (that is precisely
what the Hold row promises), or at minimum move Hold to row 1 and have the counter line say which
row ends the asks. No byte-identical asks recurred *within one window* in any deck130 seat, so
K10's own signature is clean; this is the phase-to-phase sibling.

### LOW-1 — `no_pass_reask` on a menu whose "do nothing" row is not row 0
`...-0x55b88f0cf500-vs-...125` **seq 120**: 5 options, no pass row, the seat replied
`CHOICE: 0 (pass)` with a correct rationale (`the face row is not a valid target ... I must pass`)
and was refused. `Cast nothing right now` was row 3. The prompt does say "this ask has no pass row",
but the seat's *intent* was expressible and was thrown away. Accepting `0` as an alias for a
`Cast nothing right now` row when one exists would have cost nothing here.

### LOW-2 — one unparsed reply
`...-0x55b88f0cf500-vs-...125` **seq 156**: `reply: " gr gr,"` on a 2-option ask at turn 56 with a
17-source mana line. Model-side garbage, not an interface defect; recorded for the census.

### Latency (observation, not a defect claim)
deck130's six seats spent **5.42 h** of model time on 497 decisions. Eight single decisions ran over
230 s and three over 360 s (seq 21 900 s, seq 60 620 s, seq 32 595 s). Worth the engine seat's
attention only as a run-throughput number.

## 3. Guide verdict: **KEEP AS IS**

`deck130_strategy.txt` (70,927 bytes, in band) is not edited this wave. Reasons:

- **Both strategy misplays this corpus violated rules the guide already states verbatim, twice
  each.** The vs162 Blastminer attack (s14) is covered by the ATTACK FLOOR ("He stays home only
  when a blocker's parenthesis says he dies") *and* by the quick-reference line ("Dwarven Blastminer
  appears in `Your creatures that can attack:` -> leave him out of the ATTACK line"); mana available
  was 0, so no carve-out applied. The turn-67 decline loop is covered by the Hold-row line ("take
  THAT row, at the FIRST such ask of the turn, not after four declines"). Restating either would
  add bytes without adding information — the failure is compliance, not coverage.
- **The vs125 loss shows the guide working.** At s120 the seat quoted rule #2 step 4 and passed
  against an opponent gaining +10 a turn; three points a turn from a recurring Hammer does not race
  that, exactly as the CROSS-CHECK latch says. The game was lost to the matchup, not to the text.
- **The wins were guide-shaped**: the WALL CHECK refusal at vs126 s35, the Starstorm X=5 that killed
  only theirs at vs152 s26, the recurring-Hammer upkeep loop, and the LETHAL-THIS-TURN chains.

### Reported, deliberately NOT edited: the face-burn release threshold
In vs123 **seq 42-43** the seat fired Hammer at the face at 18 life across
`Opponent battlefield (4 permanents listed, of which 0 are creatures, ... and 3 are lands)` after
two of its land kills had resolved — i.e. every condition of the guide's release except `L of 2 or
fewer` (L was 3) — and won the game on that clock, its only creature (Dwarven Blastminer) being
permanently locked by the opponent's Intruder Alarm. That is a real, specific case where the
threshold's one-land margin was wrong. I am not moving it on one game: the obvious confound is that
winning decks burn faces because they are winning, the guide's own history records the early-face
mistake as this deck's recurring loss condition, and the vs125 seat shows the floor holding
correctly at L=11+. If the core loop wants this tested, the falsifiable form is "does a release
condition of `0 creatures AND L <= 3 AND a resolved land kill of yours` fire more often on won
games than lost ones", which needs the whole pool, not deck130.

## 4. Proposals
None. No general-guide change and no strategy-writing-skill amendment is paid for by this deck's
evidence: HIGH-1/HIGH-2 are engine defects, MED-1/MED-2 are interface defects, and the two strategy
errors are noncompliance with text that already exists.

## 5. What I did NOT check
- I did not read the opponents' seat files except through my own seat's rendered `GAME LOG`; the
  opponents' decisions are uninspected.
- I did not adjudicate the lane I/J/H/K predictions (engine seat's job); the lane observations above
  are incidental to deck130's six games only, and I read only the lane headings, not the full lane
  files.
- I verified Silverquill Silencer and Tribute to Hunger against their primitives and Scryfall; I did
  **not** re-verify Master of the Feast, Underworld Dreams, Intruder Alarm, Staff of Nin or any
  deck130 card against Oracle text — nothing in the transcripts suggested they misbehaved.
- I did not build or run the engine, and I checked no source file.
- The 5.42 h latency figure is the sum of `latency_ms` over deck130's seats only; I did not separate
  queueing from inference.
