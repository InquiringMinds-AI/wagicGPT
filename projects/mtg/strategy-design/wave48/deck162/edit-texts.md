# deck162 - wave-48 edits (2), before -> after, each with its finding

Baseline: the LIVE guide `bin/Res/ai/baka/deck162_strategy.txt` (28,615 bytes = wave-47 reviewer
revision + boundary-pass B1/B2). After: `wave48/deck162/strategy.txt`, 29,506 bytes (**+891**;
one paragraph deleted under #102, so the net is smaller than the added text). ASCII only.
Corpus `matchups-20260827-072004`. Card facts re-verified with `/usr/bin/grep`: `Master of the
Feast` = `auto=@each my upkeep:draw:1 opponent`, `type=Enchantment Creature`, 5/5 flying;
`Thraben Doomsayer` = `auto={T}:token(Human,Creature Human,1/1,white)`; `Ob Nixilis, the
Hate-Twisted` = `auto={C(0/0,-2,Loyalty)}:... destroy target(creature) && draw:2 targetcontroller`.

deck162 went **5-1**; the loss (vs146, T10) was scored with RULE 1's new tiebreak obeyed (s6 took
Fate Unraveler at 16 life with 0 creatures against 4) and the body Versed one turn later. One
edit under amendment #102 and one cheap target-priority line are all its record pays for.

---

## 162-A (MED) - RULE 2 at its third consecutive violated-unpunished corpus -> #102 routing
**Finding:** 6 breaks of 15 gated windows, all in games won. `vs125` s12 and s14: `Cast Master
of the Feast` with `Your battlefield` holding no punisher name (the Underworld Dreams cast at s8
had been countered) and `Opponent battlefield (4 permanents listed, of which 0 are creatures)`
- the exception requires "shows a creature"; the s12 reply says *"MotF is the exception: it is a
body that defends against threats. Cast it now"*. s17 Forced Fruition and s21 Teferi's Puzzle Box
with the line still empty of punishers (`Your battlefield (8 permanents ...): Drowned Catacomb;
Island #1; Island #2; Swamp #1-#4 x4; Underground Sea`). `vs152` s6 (second Howling Mine, no
punisher, plan: *"Stack remaining draw engines once Fate Unraveler is on the battlefield"*) and
s10 (third Mine, Fate Unraveler exiled that turn). Wave 46: 3 breaks; wave 47: 3; wave 48: 6.
Held correctly: `vs130` s9, `vs152` s8/s14, `vs125` s8/s10, `vs126` s5. Per #102 (third failure,
fact rendered): delete a restatement, re-key the exception to a printed count, and put the check
on the pilot's own plan sentence. CLASSIFICATION: **STRATEGY.**
- **before:** the `THE EXACT SHAPE THAT LOSES GAMES: a Howling Mine is already on your
  battlefield line ...` paragraph (a restatement of the brake) and the exception `With NO punisher
  out, cast it whenever the opponent's battlefield line shows a creature`.
- **after:** the restatement paragraph is DELETED and replaced by one line (`Count the punisher
  names on YOUR battlefield line - not the ones in hand, not the one that was countered last
  turn`); the exception is keyed to `"of which N are creatures" with N of 1 or more ... At "of
  which 0 are creatures" it is a gift like the others and waits`; new `CHECK YOUR OWN PLAN
  SENTENCE BEFORE YOU ANSWER` naming "stack draw engines", "deploy draw engines", "cast Master of
  the Feast for board presence", "once a punisher is on the battlefield" as sentences that, with
  no punisher name on the line, make the answer wrong. One DECIDING row added.
- **Prediction (plan-text metric first):** engine casts behind the brake whose PLAN contains
  "stack" / "draw engine" / "board presence": 0 (was 6). Outcome half: engine casts behind the
  brake <= 1 of N (was 6 of 15). If the plan-text half fails too, the lane is exhausted and the
  remedy is a row annotation (`[no punisher on your battlefield: this card only feeds them]`).

## 162-B (LOW) - Ob Nixilis's -2 killed a token beside the token maker
**Finding:** `vs123` priority s21 (T12): menu `2. ... targeting Human [opponent's battlefield]`,
`3. ... targeting Thraben Doomsayer`, `4. ... targeting Vampire`; reply `CHOICE: 2 (... Human)`.
Two turns later their line read `31 permanents listed, of which 23 are creatures` (the Doomsayer
+ Intruder Alarm loop). Won at T15 on Puzzle Box regardless. `vs130` s12/s18 chose Siege-Gang
Commander and `vs123` s16 chose Bloodline Keeper - both right. The guide said "biggest creature";
amendment 17: printed-power rules are blind to engines. CLASSIFICATION: **STRATEGY.**
- **before:** `Use it on the opponent's biggest creature.`
- **after:** `Aim it FIRST at a creature whose printed text makes more creatures - Thraben
  Doomsayer, Bloodline Keeper, Siege-Gang Commander, any row whose text says "create" or "put a
  ... token" - and only then at the biggest body. A 1/1 token is never the target while its maker
  is on the same list` + WHAT THIS COST. One DECIDING row.
- **Prediction:** -2 aimed at a token while a maker's row is on the menu: 0 (was 1 of 3).

---

## NOT edited, and why
- **MULLIGAN (`vs125` seq 2):** the guide's "ONE LAND: ship it" and its "already taken 1 mulligan
  -> LAST look: Keep" fire on the same hand and contradict; the pilot shipped, kept a 3-lander
  with two punishers at (keeping 5), and won. Per the wave-48 brief this is an OWNER QUESTION
  (Q2 in the seat file); both sentences are byte-identical to the live guide.
- **RULE 1: 17/19.** The two breaks (`vs130` s17 Forced Fruition over Ob Nixilis; `vs126` s14
  Dictate over a second Fate Unraveler) both had a punisher already on the line and both games
  were won at T11 and T12. Recorded, not edited.
- **RULE 1 tiebreak (162-A wave 47 / B1): 1/1** - `vs146` s6. Holds.
- **GANG BLOCK rung: 1/1** (`vs123` s17), first live window in four corpora. Holds.
- **The send rung: 4/4. Land drops 93/93. No-blocker rows 1/1.**
- **Zero-power blocks, the LETHAL survival gate, the collapsed-tag rung at 162: 0 windows**
  (no `blockers` record on this seat in six games). Untested, not deleted (amendment 21).
- **Bottoming (`vs125` s4):** bottomed Howling Mine + Fog Bank from a 3-land / Mine / Dreams /
  Fog Bank / Fate Unraveler keep, against the guide's "most expensive spell first" (Fate
  Unraveler). Keeping two punishers is the better hand by the deck's own thesis; not re-worded
  on one favourable window.

## 162-C (MED, decided after the owner ruling) - the one-land branch is split by look; the LAST-look absolute and the "never fewer than 5" floor are deleted
**Finding:** `162 vs125` seq 2: at `(keeping 6)` with one land, "ONE LAND: ship it" and "already
taken 1 mulligan -> this is your LAST look: Keep" both applied and contradicted (#97); the seat
shipped, kept a 3-lander with two punishers at (keeping 5), won 16-0. Owner constraint: no
forced keeps.
- **before:** `ONE LAND: ship it ...`; `Never keep fewer than 5 cards.`; `If the prompt says
  "having already taken 1 mulligan", this is your LAST look: answer "Keep this hand"`.
- **after:** three one-land branches with the odds at each look - untouched seven: ship (one
  card for a fresh seven, nothing castable before land three); second look: weigh it (the ship
  costs two cards; keep when the coverage line names a card or a punisher is reachable on land
  three, ship when nothing is castable before turn four - the seq-2 ship quoted as a fair bet);
  (keeping 5) or lower: keep and build at the bottom ask. The LAST-look sentence becomes the
  carried-plan warning ("I will keep whatever I draw next" runs a chain; decide each look on its
  own header). "Never keep fewer than 5 cards" deleted.
- **Prediction:** mulligan answers at (keeping 5) or lower with a land in hand: 0 (0 windows so
  far); a one-land second look answered against the branch's own test: 0.
