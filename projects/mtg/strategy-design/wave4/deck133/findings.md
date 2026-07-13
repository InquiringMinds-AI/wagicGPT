# Wave 4 review — deck133 (Phyrexian Asphodel, mono-black midrange)

Evidence base: 12 games across runs `matchups-20260712-192832` (1/6, 2 timeouts) and
`matchups-20260712-230636` (2/6, 1 timeout) — 3/12 combined. Deck133 was **deck1** in
every pairing. Game→file→outcome map (epoch = seat jsonl):

| run | jsonl epoch | opp | result | end life (me/opp) | last turn |
|-----|-------------|-----|--------|-------------------|-----------|
| 192832 | 1783904732 | 140 | TIMEOUT | 20 / **2**   | 15 |
| 192832 | 1783906271 | 131 | **WON** | 15 / -7      | 15 |
| 192832 | 1783907133 | 135 | LOST    | -4 / **3**   | 12 |
| 192832 | 1783907590 | 44  | LOST    | -4 / **20**  | 12 |
| 192832 | 1783907829 | 109 | LOST    | -5 / **16**  | 10 |
| 192832 | 1783908830 | 110 | TIMEOUT | 22 / **5**   | 13 |
| 230636 | 1783915604 | 110 | LOST    | -3 / 18      | 10 |
| 230636 | 1783917616 | 109 | **WON** | 5 / -4       | 13 |
| 230636 | 1783918306 | 135 | LOST    | -9 / 12      | 12 |
| 230636 | 1783918460 | 44  | LOST    | -3 / **20**  | 16 |
| 230636 | 1783921425 | 140 | TIMEOUT | 28 / **-10** | 15 |
| 230636 | 1783922461 | 131 | **WON** | 27 / -6      | 29 |

Corpus stats (12 games): 98 non-"nothing" casts; cast-nothing 62/170 ≈ **36%** of casting
decisions (wave3 was 76/101 ≈ 75%). Bloodghast 19 casts (wave3: 0). Gray Merchant 3 casts.
Discard (Thoughtseize 10 + Inquisition 14) = 24 casts, 10 of them on turn ≥4.

---

## The 5 most consequential findings (read these first)

### 1. [FIXED — headline] The wave3 P0 fixes landed and the pilot now actually plays the deck.
The catastrophic wave3 pathology (cast-nothing 75%, Bloodghast 0/77, all games lost/timed
out to a mana-pool misread) is **substantially resolved**:
- The board line now reads `Mana available: N untapped sources (tapped automatically when
  you cast)` / `Mana available: (no untapped sources)` — the wave3 trap line `Mana in your
  pool: (none)` is gone. Cast-nothing fell 75%→36%; total casts rose ~4x (~25→98 across the
  corpus); Bloodghast went 0→19 casts.
- A per-player graveyard line is now in the snapshot (wave3 finding #2, FIXED), and the
  "Bloodghast returns via landfall" hallucination said of a Bloodghast **in hand** is gone
  (0 occurrences corpus-wide; wave3 saw it repeatedly).
- Wins went 0→3, and — see finding 2 — the 3/12 record **understates** the improvement: the
  three "timeouts" are all games deck133 was winning or had already won on board.
The vs109 rematch is the clean before/after: same red-aggro opponent it *lost* to in wave3
and again in run1, it **won** in run2 (1783917616) by deploying Phyrexian Obliterator as a
5/5 wall (seq19), Fatal-Pushing Ash Zealot and a Goblin (seq13/16), **blocking** Ash Zealot
with Obliterator (seq21), then racing. The engine for beating aggro is in the deck; the
question is now *decision quality*, not *whether it acts*.

### 2. [ATTRIBUTION REGRESSED — do not read timeouts as guide failures] All 3 timeouts are "winning but slow," not passivity.
In wave3 every timeout traced to non-casting. That mechanism is gone; the wave4 timeouts are
the **opposite** — deck133 ahead and unable to bank the win before the cap:
- vs140 run1 (1783904732): opp at **2**, deck133 at 20, declaring all-three attackers for
  lethal at seq40 (T15) when the cap hit.
- vs110 run1 (1783908830): opp at **5**, deck133 at 22.
- vs140 run2 (1783921425): opp at **-10**. The pilot's own seq59 plan reads *"The game is
  already won. Gray Merchant of Asphodel resolved on Turn 16, dropping the opponent to -10"*
  — yet results.tsv scores it `timeout`.
Root causes are **engine/harness + matchup, not the guide**: (a) grindy lifegain opponents
(deck140 and deck131 both run **Elixir of Immortality**) stretch games to the turn/wall-clock
cap; (b) slow inference — deck133's own decisions ran median 8-11 s, max 59-68 s (consistent
with `-j` concurrency saturating Spark), so a 40-60-decision game burns the wall clock even
though deck133's cumulative inference was only ~8-12 min; (c) a likely **engine/harness bug**:
in vs140 run2 the opponent reached **-10 life** without the game ending (finding 6). Reviewer
action: treat the three timeouts as *non-losses* deck133 mostly earned, and do **not** add
anti-passivity pressure on their account.

### 3. [NEW — primary remaining leak] Against aggro/evasion the pilot floods on low-impact discard and feeds its can't-block Bloodghasts into blockers.
The real losses share one shape, and it is *not* passivity — it is casting the wrong things:
- **vs44 Faeries, BOTH games (1783907590, 1783918460): the opponent never lost a single life
  point — ended at 20/20.** Deck133's ground Bloodghasts (2/1, `[cantblock]`) were blocked/
  traded by the opponent's flyers (Oona's Gatewarden 2/1 wither defender, Faerie Bladecrafter
  growing 3/3→5/5), while the Faerie flyers raced deck133 to death. Deck133 has **no flyers
  and no reach except Gray Merchant**, and Bloodghast literally cannot block — so vs an
  evasive board it cannot defend *and* cannot connect on the ground.
- Meanwhile it kept casting **discard**: vs44 run1 cast Inquisition/Thoughtseize at seq20/24/
  28/32 on turns 5-9, when the opponent's hand was down to 2 then 0 cards — stripping a near-
  empty hand does nothing about the three flyers killing it. Corpus-wide, 10 of 24 discard
  casts were on turn >=4.
- This **costs deck133 life it can't afford**: Thoughtseize is 2 life each and fetchlands are
  1 each. vs44 run1 bled 20→13 by turn 7 largely from its own fetches + Thoughtseize, then the
  flyers finished it (seq30-37). vs135 run1 (1783907133) it even cast **Phyrexian Arena** (-1
  life/turn) into a race it then lost at -4 with the opponent at 3.
The guide is partly complicit: "CAST BLOODGHAST" every turn + "attack recklessly" + "cast a
spell every turn" pushes exactly this behavior when the correct line vs evasion is *stop
discarding, deploy Obliterator/removal, preserve life, and go to the Gray Merchant drain.*
See handoff for a proposed matchup brake.

### 4. [PERSISTING, reduced] Gray Merchant — the deck's namesake win condition — is still barely deployed (3 casts / 12 games).
Wave3 was 0/25. Wave4 is 3 casts, but it was offered as *castable* only 6 times, so the
cause is now mostly **under-draw / not reaching 5 mana** rather than the wave3 refuse-to-cast
(the 22/25 decline is gone). Consequence: deck133 wins by creature beats, and its
close/failure-to-close games (the timeouts, the vs135 run1 loss at opp-3) are games where the
drain-to-close never fired. The GRAY MERCHANT KILL CHECK in the guide is sound but rarely gets
a turn to run because the card isn't in play. Lower-priority than 3, but the finisher plan is
still not happening.

### 5. [NEW — residual model defect] Plan/choice mismatch: the pilot "PLANs" a cast it can't afford, then selects "Cast nothing."
vs135 run2 (1783918306) is the one genuinely passive loss (cast-nothing 12/17), and the tell
is precise: at seq30 (T11) and seq31 (T12) the reply reads *"PLAN: Cast Gray Merchant of
Asphodel immediately"* while the **chosen action is "Cast nothing right now."** Gray Merchant
costs {3}{B}{B} = 5, and deck133 was stalled on **4 Swamps** — so it was not in the numbered
list, but the model believed it could cast it and defaulted to nothing rather than picking a
real option (it had a targetless Fatal Push in hand). The model does not verify its intended
card is actually among the offered choices. Compounded here by a genuine 4-land mana stall
(bad draw), but the plan/choice divergence is a real, checkable model failure.

---

## Wave-3 findings: FIXED / PERSISTING / REGRESSED

| Wave3 finding | Status | Note |
|---|---|---|
| #1 `Mana in your pool: (none)` misread (P0) | **FIXED** (representation) | Line replaced with `Mana available: N untapped sources`; cast-nothing 75%→36%. "No-mana" *verbalization* still appears in replies but no longer vetoes castable main-phase spells (in vs131 run1, 8 of 18 "no-mana" replies still cast anyway). |
| #2 Graveyard not in snapshot | **FIXED** | Per-player graveyard line present; Bloodghast-in-hand landfall confabulation now 0. |
| #3 Plan carry-over makes procrastination self-reinforcing | **PERSISTING (milder)** | Still visible (vs131 run1 "attack next turn" repeats), but no longer game-defining; its cousin is the finding-5 plan/choice mismatch. |
| #4 Repeated single-option priority asks burn inference time | **PERSISTING** | vs131 run2 logged **62 priority records** (36 "pass", plus repeated single-option fetch offers). Phase auto-skips helped but grindy games still balloon decision count → this is now the main *timeout* driver (finding 2), not passivity. |
| #5 `choice: -1` failed decisions | **FIXED (this corpus)** | 0 fallbacks across all 12 games. |
| #6 Blockers seam near-absent | **IMPROVING** | Blocker decisions now appear (vs109 run2 Obliterator blocks Ash Zealot) as creatures actually hit the board. |
| Passivity-framed identity / circular prohibitions (skill.md) | **FIXED** | Action-framed identity + numeric GO thresholds are in the current guide; the pilot develops. |

**New in wave4 (not seen/blamed in wave3):** discard-flooding + can't-block-Bloodghast vs
aggro/evasion (finding 3); self-inflicted life loss vs aggro (finding 3); plan/choice mismatch
on unaffordable cards (finding 5); a probable engine "negative-life without game-end" artifact
(finding 6).

---

## Supporting detail

### 6. [ENGINE/HARNESS, flag only — outside guide] Opponent reached lethal negative life without the game ending.
vs140 run2 (1783921425): by seq59 the opponent is at **-10 life** (deck133's plan explicitly
notes Gray Merchant resolved and the game "is already won"), yet the game continued to seq61
and was scored `timeout`. A player at <=0 should lose to a state-based action immediately.
Candidate causes: the opponent's Elixir of Immortality interaction, or the win not being
adjudicated before the wall-clock cap. This is an engine/adjudication observation, not a
guide item — but it means results.tsv `timeout` rows for deck133 can hide **clean board
wins**, so win-rate understates the deck. Worth a separate engine look; I did not touch code.

### 7. [MODEL, low] "No-mana" self-talk persists but is now mostly harmless.
The representation fix broke the veto but not the verbalization: replies still say "I have no
mana / my lands are tapped." In this corpus those phrases now attach overwhelmingly to
**correct priority passes** (opponent's turn, no instant to hold) or to genuinely-unaffordable
cards (finding 5), not to declining castable main-phase spells. Not worth more guide text;
noting so the next reviewer doesn't mistake the phrase-count for the wave3 pathology.

### 8. [DECK/GUIDE] The winning template vs aggro, for the reviser to encode.
Contrast within the vs109 matchup: **loss** (run1, opp stays 16) = flooded on Thoughtseize/
Inquisition + Bloodghast, no Obliterator drawn/deployed, raced out. **Win** (run2) = Fatal
Push on their creatures (Ash Zealot, Goblin), Obliterator as a wall, **block** with Obliterator,
then race. The lever separating deck133's aggro wins from its aggro losses is *deploy
Obliterator + point removal at their board + stop casting dead discard + preserve life*, not
"cast Bloodghast and attack recklessly." The guide currently teaches the latter.

### 9. [GUIDE, minor] Mulligan slightly loose toward discard-heavy keeps.
Two mulligans in 12 games; both defensible-ish. vs44 run2 mulliganed then **kept
Thoughtseize + Thoughtseize + Swamp** — a discard-heavy 6 that fed the flooding loss. vs131
run1 shipped a keepable 3-land Bloodghast + Fatal Push + 2 Thoughtseize hand (then won anyway
post-mull). Not a major leak; a one-line "a hand that is all discard + one land is a ship vs
unknown aggro" would help.

### What the pilot did WELL (preserve; do not regress)
- **Yawgmoth piloting** (vs131 run2): 17 `-1/-1 counter` activations + 4 proliferate — the
  sac-engine loop was driven correctly across a 29-turn grind it ultimately won.
- **Impact-ranked discard picks** stayed strong (Searing Spear vs burn, Cranial Plating vs
  affinity, Counterspell/Go-for-the-Throat vs control) — keep the "choose by IMPACT" line.
- **Defensive blocking with Obliterator** (vs109 run2) and correct multi-attacker lethal math
  when creatures existed. The problem is never the combat arithmetic; it is *what got deployed
  before combat.*

---

## Handoff for the guide reviser (priority order)

1. **Add a real aggro/evasion brake (finding 3, biggest remaining lever).** Currently the
   guide's "you are the beatdown in nearly every game / cast Bloodghast every turn / attack
   recklessly" fires even when deck133 is being raced by flyers or fast aggro. Encode the
   *flip* concretely and positively (avoid negative framing — it bleeds): e.g. "When the
   opponent has attacked you below ~14 or has flyers you can't block: your job is REMOVAL +
   WALL. Fire Fatal Push / Liliana -2 at their attacker; land Phyrexian Obliterator and BLOCK
   with it; stop casting Thoughtseize/Inquisition (a stripped hand doesn't stop the board);
   don't pay life you don't have — skip the Thoughtseize and the Phyrexian Arena, and don't
   crack a fetch you don't need this turn. Then win with the Gray Merchant drain." This
   overrides three guide lines that currently mislead in this matchup.
2. **Discard timing (finding 3).** Make explicit that Thoughtseize/Inquisition are **turn-1-3
   spells**; casting them past turn 3 needs a named reason (a known sweeper/combo piece), and
   never when being raced. 10 of 24 discard casts were turn >=4.
3. **Push the Gray Merchant drain harder as the closer (finding 4).** The KILL CHECK is good
   but the card rarely reaches play; reinforce "when you have 5 mana and a black board, Gray
   Merchant is the play — it is your only reach" so it isn't passed for a fourth Bloodghast.
4. **Plan/choice mismatch (finding 5) is a general-prompt/representation candidate**, not a
   deck fix: the pilot should be told to pick its action **only from the numbered list**, and
   if its intended card isn't listed, choose the best card that *is*. Flag for the general
   layer.
5. **Do NOT touch the timeouts as a guide problem (finding 2).** They are grind + latency +
   a probable engine adjudication gap. If anything, they argue for *faster* closes (item 3),
   not more activity.

Engine/harness items for a separate track (not guide): negative-life-without-game-end
(finding 6); repeated single-option priority asks inflating decision count and wall-clock
(wave3 #4, still open); results.tsv `timeout` rows should record final life so board-wins
aren't hidden.

**One-line summary of the deck's plan and the piloting problem this review targets:** deck133
is a mono-black disruption-midrange deck that grinds with recursion (Bloodghast/Geralf's/
Yawgmoth) and closes with the Gray Merchant drain; wave3's "won't act" pathology is fixed, and
the wave4 problem is *quality* — it over-casts dead discard and throws can't-block Bloodghasts
into blockers instead of switching to removal + Obliterator-wall + drain when it is being
raced, especially by evasion it has no answer to.
