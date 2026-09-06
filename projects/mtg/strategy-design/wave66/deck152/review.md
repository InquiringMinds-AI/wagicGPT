# Wave 66 — deck 152 (Bant Human Midrange) per-deck review

Seat files (all under `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-064038/`,
prefix `<epoch>-ai_baka_deck152-<ptr>-vs-ai_baka_<opp>.jsonl`):
1788694842 (v162), 1788694847 (v146), 1788694849 (v130), 1788694851 (v126),
1788694873 (v125), 1788694879 (v123). 236 model decisions, 161 min of seat wall time.

**Fallback note (asked for explicitly): ZERO.** `/usr/bin/grep -c unparsed_reply` returns 0 on
all six deck152 seat files. Corpus-wide there are 83 across 23 other seat files; not one landed
here, and no `fallback` field is set on any deck152 record. The Baka answer never ran on this
seat, so no game outcome here turned on the fallback. (Two of the 83 are on 152's *opponents'*
seats — 1788694842 deck162 x2, 1788694879 deck123 x2 — i.e. the other side of two 152 wins.)

---

## 1. Game by game

| # | Opponent | Result | Turn | Life |
|---|---|---|---|---|
| 1 | 162 | **WIN** | 11 | 24 / -4 |
| 2 | 146 | **LOSS** | 15 | 0 / 22 |
| 3 | 130 | **WIN** | 15 | 18 / 0 |
| 4 | 126 | **WIN** | 13 | 20 / -5 |
| 5 | 125 | **WIN** | 19 | 25 / -3 |
| 6 | 123 | **WIN** | 21 | 24 / -4 |

5/6. The deck executed its own guide: deploy every turn, grow with lords, attack.

### v162 (WIN, t11) — the mulligan rule paid
seq 2 `Mulligan` then seq 3 `Keep this hand` at M=6, seq 4 bottomed Tovolar's Huntmaster (the
6-drop). Guide RULE -1's M=6 clause is exactly the ask that was answered, and the keep produced
a curve-out. Deciding decision: **seq 35** — attacked with all six (Katilda, Intrepid Adversary,
2x Luminarch Aspirant, Sigarda 8/8, Brutal Cathar) into 19 life for exact lethal after two
Aspirant triggers (seq 32, 34) were spent on Sigarda. Correct and guide-conformant.

### v146 (LOSS, t15) — lost to their board, not to a misplay
The deciding sequence is **seq 25/26 (t12)** — cast Sigarda then Briarbridge Tracker, the Tracker
paid by tapping Katilda — followed by the opponent's **Soul Shatter on Sigarda at t13**, which
the model could not have played around (opponent had 0 untapped sources at seq 26, so no
instant-speed answer was visible or available). From t13 on it was a 1-life corpse.
- **seq 23 (t11 blockers)** is the decision most likely to be read as the loss, and it was
  CORRECT. 17 life, one 1/1 blocker, every offered block rung-3 (`your blocker dies, attacker
  lives`), declining leaves `you would be at 10`. The guide's ladder (lines 310-327: take rung 3
  only when N is 9 or less) says decline, and the reply cites the rule by name and follows it.
  Blocking would have bought 3 life and cost the Katilda that paid for the t12 Tracker; at 13
  life the t13 swing of 9 still leaves 4, and the t15 swing of 13 still kills. **Guide rule paid.**
- **seq 27 (t13 blockers)**: forced block correctly taken (`this KILLS you ONLY if you decline
  every block`), Tracker traded up into Barrowin (4/4). Correct.
- **seq 31 (t14)**: `ATTACK: none` at 1 life with a lone 1/1 against 5 untapped blockers.
  Correct; it was already lost.
- The one real deviation is **seq 11**, below (item MED-1).

### v130 (WIN, t15) — Stone Rain x2 survived on a two-land board
Opponent destroyed Overgrown Farmland (t6) and Barkchannel Pathway (t8) and killed Brutal
Cathar. Deciding decision: **seq 16 (t9 priority)** — levelled Ranger Class to 2 with the last 2
mana, which put a +1/+1 counter on the Wolf every combat thereafter; that Wolf went 2/2 -> 5/5
and dealt the last 12 of the damage alone (seq 17, 20, 23). **seq 15** held priority off a menu
whose only cast row was Fateful Absence pointed at its own creatures — correct decline.

### v126 (WIN, t13) — Brutal Cathar as removal
**seq 18/19 (t9)**: cast Brutal Cathar and exiled the opponent's Vampire token, then **seq 20**
cast a *second* Brutal Cathar with the same turn's mana. Two bodies plus two exiles in one turn
broke the board. **seq 28**: correctly identified that the second Sigarda in hand was a legend-rule
non-cast (the row rendered `[legendary: you already control Sigarda...]`) and cast nothing.

### v125 (WIN, t19) — Lair of the Hydra closed it
**seq 20/21 (t13)**: cast Teferi and immediately used -2 to dig, twice over the game (seq 21, 31),
both times hitting a land. **seq 40 (t17)**: animated Lair of the Hydra to a 5/5 and swung with
Huntmaster + 2 Wolves for 11. **seq 50 (t19)**: animated Lair to a 7/7 during the blockers step
for the kill. Adversary counters were bought here (seq 12, 14) and not in the loss — see MED-1.

### v123 (WIN, t21) — through Intruder Alarm and a Bloodline Keeper
**seq 26/27 (t15)**: Fateful Absence on Bloodline Keeper, the one card that would have out-scaled
the board. Correct target off the `{kills whichever you target: THEIRS - Bloodline Keeper...}`
tag. **seq 59/65**: Sigarda (flying) carried the last 12 damage while the ground stalled.

---

## 2. Engine / interface / card items

### HIGH-1 — Repeat-pay mode menus render every unreachable rung (21 rows for 1 payable)
File `1788694847-...deck152-...-vs-...deck146.jsonl`, **seq 11**, t6. The Intrepid Adversary ETB
renders rows 1-21 (`don't add any counter` .. `add 20 counters`), ~2,700 characters, while the
seat has 2 spendable mana. Rows 3 through 21 are *behaviourally identical* — each says
`you have 2 spendable now, which pays for 1 of them and stops`. Rendered line, row 21:

```
21. add 20 counters {this mode has a legal object right now} {repeat cost: 20 x {1}{w} = 40 mana
for all 20; you have 2 spendable now, which pays for 1 of them and stops} {paying this taps:
Katilda, Dawnhart Prime - it cannot attack this turn (that is the 1 payment your mana covers, not
all 20)}
```

The rows are not a render invention — `borderline.txt:57863-57883` hard-codes 21 `auto=choice`
lines — but the *prompt* need not print them. Repro seats: 146 seq 11, 162 seq 11, 125 seq 7/12/14
(5 occurrences, ~13.5 KB of menu). Fix shape: print rows 0..(payable+1) and collapse the rest to
one line naming the ceiling, e.g. `rows 3-21 all add the same 1 counter your mana covers`. This
generalises to every `thisforeach(variable{N})` card in the pool, not just this one.

### HIGH-2 — 79% post-answer overrun on this seat; 50 min of wall time is discarded text
236 decisions, **187 (79%) carry post-answer overrun** (corpus figure is 60%), 87,224 chars total.
The 20 decisions with >=700 chars of overrun account for 44,884 chars and **50 of the seat's 161
minutes**. Worst: `...vs-deck126` **seq 27**, 6,813 chars of overrun, **493 s latency**, on a
*land-drop* choice. The answer (`CHOICE: 2 (Boulderloft Pathway)`) was committed at character 31;
the remaining 6.8 KB is the model relitigating the legend rule on a Sigarda it was not casting.
9 of the 16 decisions over 200 s on this seat carry >2,000 chars of overrun. Because first-wins is
in force, none of it can change an outcome — it is pure wall clock. This is the seat's single
largest measurable cost and it is an AR-lane (reply protocol) item, not a guide item.

### HIGH-3 — "Wait," is the model's actual retraction token and H2b does not list it
Of the 131 replies with >=200 chars of overrun, **20 contain a `Wait`-headed reversal after the
coded line** and only **8** contain one of H2b's four enumerated headers (`Correction:`,
`Re-evaluating:`, `Actually,`, `On reflection`). `answer_replaced` and `commit_retracted` are
False on **every** deck152 record — H2b never fired here. Two consequences worth separating:
- **The near-miss that H2b would have gotten wrong.** `...vs-deck130` **seq 22**, t15. First coded
  line `CHOICE: 1 (Hengegate Pathway (menu text: Play Land))`; the next non-blank line is
  `Correction: Hengegate Pathway enters tapped because I do not control two other lands (I only
  have Ranger Class). So playing the land leaves me with 0 untapped mana. I cannot cast Katilda. I
  must pass.` The header is one line after the coded line — inside H2b's 3-line window. It carried
  no new coded line, so nothing was superseded, which is the *lucky* outcome: the correction is
  factually FALSE (see MED-2) and honouring it would have cost the land drop.
- **The exposure.** H2b's window is keyed on a header vocabulary the model uses in 8/131 cases
  while using `Wait,` in 20/131. Widening the vocabulary would make H2b fire ~2.5x more often on
  reversals that, on the evidence of seq 22, are as likely to be wrong as right. Recommendation
  for the AR lane: **do not widen it**; the value of H2b on this seat is zero and its downside is
  real. Cheaper lever: cut the overrun at the source (HIGH-2).

### MED-2 — MDFC land rows omit the enters-untapped clause that ordinary land rows carry
Same repro, `...vs-deck130` **seq 22**. The Pathway row renders:

```
1. Hengegate Pathway (menu text: Play Land) [PLAY THIS AS A LAND: puts "Hengegate Pathway" onto
the battlefield as a land (taps for {W}). It costs no mana and uses no stack, and it USES YOUR
LAND DROP for this turn.]
```

An ordinary land row in the same game (seq 8) renders `Play Overgrown Farmland [enters UNTAPPED -
it makes mana this turn (you control 2 other lands): "Overgrown Farmland enters tapped unless you
control two or more other lands."]`. The MDFC row states *no* ETB status at all, and the model
filled the gap by importing Farmland's condition onto the Pathway — "enters tapped because I do
not control two other lands". Ground truth: `borderline.txt:52388-52395` gives Hengegate Pathway
`auto={T}:add{W}` and no tap-on-ETB clause; Scryfall agrees (Pathway MDFCs always enter untapped).
Both sources agree, so the render is wrong by omission, not the card. Fix: give the
`PLAY THIS AS A LAND` bracket the same `enters UNTAPPED - it makes mana this turn` clause.
Cost this wave: none (first-wins kept the correct action). Cost if it lands on a hold-vs-cast
window instead of a land drop: a skipped turn.

### MED-3 — a static from an opposing permanent is tagged on the creature without naming its source
`...vs-deck123` **seq 54**, t19. `Sigarda, Champion of Light ... [trample, flying, doesn't untap
during its controller's untap step] ... [tapped - cannot attack or block this turn]`. The cause
(the opponent's Intruder Alarm) *is* on screen — in the opponent-battlefield block with full text,
and the stack line even reads `ability: Intruder Alarm's Untap [from their Intruder Alarm]` — but
three screens away from the tag. The reply spends 4,852 chars asking "Why? She entered on Turn 13
and attacked... Did she tap for mana? No. Did she block? No." and reaches no answer.
Repro also at seq 63 (3,095 chars). Fix: append the source to the tag —
`[doesn't untap during its controller's untap step (from their Intruder Alarm)]`. Low cost, and
it retires 8 KB of flailing in one game.

### LOW-1 — the "Opponent life trend" label is start-of-turn, and reads as end-of-turn
`...vs-deck130` seq 15: `Opponent life trend: turn 5: 20, turn 7: 18, turn 9: 15, now 15`, while
the log four lines above shows turn 7 combat taking them 18 -> 16. Both are true — the samples
are start-of-turn — but `turn 7: 18` next to a log that ends turn 7 at 16 invites a
contradiction read. Suggest `start of turn 7: 18`.

### Checked and clean (reported as such, not as absence of looking)
- **No card bug found.** Intrepid Adversary verified against the primitive
  (`borderline.txt:57861+`, `auto=thisforeach(counter{0/0.1.Valor}>0) lord(creature|myBattlefield)
  1/1`, 3/1 lifelink) and against Scryfall (`Creatures you control get +1/+1 for each valor
  counter on this creature`) — they agree, including that Adversary buffs *itself* (the 3/1 -> 4/2
  the engine rendered at 125 seq 18 is correct). Hengegate Pathway and Overgrown Farmland verified
  as above. Fateful Absence (`borderline.txt:38238`) matches its Oracle text.
- **H2a (reasoning-first order): holds, 8 of 8.** Only 8 of 236 replies used a `REASONING:` header
  at all; in all 8 the coded line follows the reasoning and is trailed only by `PLAN:`. **No coded
  line was ever buried mid-reasoning on this seat** — the hazard AR named did not occur here.
- **H2c `ignored_line`: 6 records, none harmful.** 4 are a duplicate of the executed choice
  (162 s33, 125 s41, 123 s54, and 123 s63 where the ignored later line differs but the first-wins
  answer was right); 126 s27 ignored a later `CHOICE: 1 (Branchloft Pathway)` after executing
  `CHOICE: 2`; 125 s50 executed `CHOICE: 7 (becomes a 7/7 hydra)` and ignored a later
  `CHOICE: 0 (pass)` — first-wins took the *winning* line and the model's own second thought would
  have passed on lethal. First-wins was net-positive on this seat.
- **H7 HOLD: rendered 100, taken 19, plain decline 29** (prefix-matched on
  `Hold priority: pass now, and do not ask me again`). 19:29 against wave 65's corpus 4:182 and
  29:213 — the hold row is now genuinely reachable at this seat.
- **H6 summoning-sick tags: correct where seen.** 146 seq 12 renders `NOT offered above and NOT
  able to attack this turn (summoning sick) - Intrepid Adversary ... can attack from your NEXT
  turn on if nothing else changes`, and Adversary had no canAttack gate — AU R4 conformant.
- **No `reveal_stall_forced`.** 6 reveal decisions on this seat, latencies 3.6 s to 69.9 s; the
  69.9 s one (162 seq 36) carries 914 chars of overrun, i.e. model thinking, not a transport stall.
- **Zero occurrences on this seat, so UNTESTED here, not passing:** `DRAW FORECAST` (H1) — this
  deck has almost no draw; `repeat_past_stop` / `loop_autopass` / `[LOOP RUNNING:]` (H3); discard
  asks and `DEFAULT ANSWER: discard the spare land.` (H5); `HALF DEAD` modal census (H9);
  `index_name_conflict` / `index_name_unique_name`; `plan_contradicts_noop_row`;
  `named_row_reask`; `wall_miss`. `GANG BLOCK:` rendered 13 times, was never the taken line.

### What I did NOT check
The other 17 seat files except where 152 is the opponent; the 83 `unparsed_reply` records
(none are on this seat — the engine seat owns that census); `async_drops`/`transport` fields;
Soul Shatter's own primitive (an opposing card); whether the 21-row menu shape appears outside
this deck's pool.

---

## 3. Guide verdict: **KEEP as is**

`projects/mtg/bin/Res/ai/baka/deck152_strategy.txt` (68,040 bytes, inside the 41-71 KB band with
~3 KB of headroom). No `strategy.txt` written.

This is a success verdict, and the evidence is that the guide's rules were the *reason* for the
deck's decisions, not decoration on them:
- **RULE -1 (mulligan)** governed the only mulligan window in the pool (162 seq 2-4): one ship, a
  keep at M=6, and a bottom of the 6-drop. Won that game on turn 11.
- **The block ladder (lines 310-327)** decided the loss's most consequential block (146 seq 23).
  The reply quotes the rung-3 rule and the N<=9 threshold and applies both correctly. Re-deriving
  the counterfactual: blocking buys 3 life and forfeits the Katilda that pays for the turn-12
  Briarbridge Tracker; the t13 and t15 swings still kill through it. The rule was right at N=10.
- **The Katilda-in-Main-2 rule (#2a, lines 211-216, 762)** was honoured — Katilda's counter ability
  was never taken in Upkeep or Main 1 across six games.
- **The Pathway colour rule (line 459)** held: 12 MDFC land-drop asks, and the face chosen matched
  the colour the next turn's cast needed each time (e.g. 123 seq 37 took Tidechannel `{U}` with
  Teferi in hand).
- **The #1 mana rule** held: 236 decisions, zero "Cast" numbers invented, zero unpayable picks.

The one decision that ran against the guide is MED-1 below, and the guide **already contains** the
rule it broke (line 348: *"Pay as many times as you can afford with a board out"*). Adding prose to
restate a rule the model ignored is the busywork pattern wave 58 was flagged for; it would also
spend the guide's last 3 KB of band headroom on a 1-in-5 deviation in the game the deck lost for
unrelated reasons.

### MED-1 (STRATEGY, no edit) — the Adversary decline the guide already forbids
`...vs-deck146` **seq 11**, t6. Adversary resolves leaving `2 of your 4 untapped mana sources`
untapped; the menu prints `add 1 counter ... you have 2 spendable now, which pays for 1 of them`.
The model took `CHOICE: 1 (don't add any counter)` with the stated reason `Attack with Katilda`
— because the row warned `{paying this taps: Katilda, Dawnhart Prime - it cannot attack this
turn}`. **One window later, at seq 12, it declared `ATTACK: none`**, reasoning `Katilda dies to
Silverquill Silencer (3/2 vs 1/1), so she stays home`. The mana was preserved for an attack the
next screen priced as suicide and then declined. Cost: a permanent +1/+1 on the whole board
(Adversary 3/1 -> 4/2) for 2 mana that was otherwise spent on nothing.
Contrast the same ask in the won game (125 seq 12 and seq 14: `add 1 counter` both times, with
identical `2 spendable / pays for 1` text). Guide line 348 already rules this: it says pay. Three
of five Adversary ETBs on this seat declined, and two of those three (162 seq 11, 125 seq 7) had
genuinely no payable spare mana and were correct — so the deviation is 1 in 5, not systemic.

If the wave wants a lever here it belongs in **HIGH-1**, not the guide: the row that made the
model hesitate is the `{paying this taps: Katilda ...}` clause, and it fired on a turn where the
attack it protected was never going to happen. A `{paying this taps: X - it cannot attack this
turn}` clause is worth printing only when X is actually offered a favourable attack.

---

## 4. Proposals
None. No `general-proposals.md`, no `skill-proposals.md`. Nothing found here is a general-strategy
or strategy-writing-skill matter — HIGH-1/2/3 and MED-2/3 are engine and reply-protocol items for
the engine seat and the AR/AT lanes, and MED-1 is a rule the deck guide already states.
