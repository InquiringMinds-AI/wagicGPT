# deck102 (Tergrid, discard/sacrifice) — findings (wave 25, guided round 5)

Corpus: `matchups-20260725-035605` (PRIMARY), binary `62d8783a5` (wave-25 step-1 batch + RULES-FIX
batch f0500ad66), -T 3000, REPPENALTY=1.05. 6 games round-robin (vs 131/59/133/137/27/109).
Record **4/6** (up from wave-24's 2/6): WIN vs131 (1/-1 @ t14), LOSS vs59 (0/15 @ t13), WIN vs133
(23/-7 @ t12), WIN(adj) vs137 (13/9 @ t10), WIN vs27 (5/-1 @ t13), LOSS vs109 (0/16 @ t11).
**Fallbacks: 0/6 at this seat, 0 corpus-wide** — the s5 commit-lock example-leak shape I owned last
wave is STRUCTURALLY DEAD (b1 de-fang confirmed live). Latency medians 12.7-19.6s/seat; a few
200s+ singletons but nothing broke.

Headline: **strong, clean corpus. Tergrid cast a THIRD time and again RULES-CORRECT (God front face
via "Cast Card Normally", won the game vs131); flip-thrash stayed 0/6 (round 3); Smallpox discipline
held (round 4, a well-reasoned t6 fire); the pw-redirect removal (a4) is verified — ZERO redirect
records corpus-wide, damage/discard targeting hits the player directly with no menu. The one real
blemish is a PREGAME misplay: an over-mulligan of a keepable 3-Swamp Hymn hand vs109 (NOT wave-24's
miscount — a JUDGMENT error chasing a turn-1 play), which contributed to a loss and drove the ONE
guide edit this wave.**

| vs  | result     | end   | my/opp | read |
|-----|------------|-------|--------|------|
| 131 | WIN        | t14   | 1/-1   | **Tergrid cast t8 (God face, correct) + Sheoldred; closed with the pair. Clean.** |
| 59  | LOSS       | t13   | 0/15   | Persist deck; **t6 Smallpox was DISCIPLINED/correct**; lost the grind, no clear in-game leak |
| 133 | WIN        | t12   | 23/-7  | mono-B devotion; Gray Merchant/Syr Konrad grind; dominant (contrast wave-24's timeout draw) |
| 137 | WIN(adj)   | t10   | 13/9   | GW; ahead at cap; Thoughtseize/Steel Hellkite line; correct adjudication |
| 27  | WIN        | t13   | 5/-1   | Steel Hellkite + Sheoldred beatdown; clean |
| 109 | LOSS       | t11   | 0/16   | **over-mulliganed a keepable Hymn hand to 6; lost to the aggressive matchup down a card** |

---

## Section 1 — STEAL WINDOWS (brief focus 1): THIRD consecutive Tergrid-on-board wave with ZERO genuine windows — but now with a PRECISE mechanistic reason the random pool cannot produce one.

Tergrid was on the battlefield in exactly ONE game (vs131, cast t8, alive through t14). That is the
only window in which a steal could fire. I traced every opponent permanent that left play during
t8-t14 in vs131:

- **Elemental (1/1) tokens** dying/being sacrificed (t12, t14): TOKENS — Tergrid's `@sacrificed(*[-token]|...)`
  correctly EXCLUDES them. Not stealable, not offered. Correct.
- **Young Pyromancer** (t11): died in COMBAT (seq33 — my Sheoldred blocked it). A combat death is
  neither a sacrifice nor a discard -> Tergrid does not trigger. Correct.
- **Elixir of Immortality** (t12): this LOOKED like a sacrifice (life +5, cards to library) but is
  NOT. Cross-checking the deck131 seat log, the engine's Elixir script activates on `{2},{T}` and the
  card leaves via the EFFECT ("Your Elixir of Immortality: battlefield -> library" — a shuffle-into-
  library), **not a sacrifice cost**. So Tergrid's `@sacrificed` trigger correctly did NOT fire. (Even
  in paper MTG where Elixir sacrifices itself, the card is simultaneously shuffled into the library and
  is no longer in the graveyard, so Tergrid's "put that card onto the battlefield from their graveyard"
  would fizzle — no practical miss either way.)
- **Sheoldred's opponent-upkeep forced sacrifice** (t12, t14): Sheoldred ("each opponent upkeep, that
  player sacrifices a creature") DID force sacrifices — but the opponent had Elemental TOKENS available
  and sacrificed a token each time (keeping its real creatures). Tokens are excluded. No steal.

**The mechanistic finding (new, load-bearing for rotation):** deck102's OWN steal enablers that fire
in random-pool play — Sheoldred's edict, symmetric Smallpox/Pox, single-target edicts — all let the
OPPONENT CHOOSE what to sacrifice, and against a token-making opponent (deck131 = Young Pyromancer +
Guttersnipe; much of the pool makes tokens) the opponent always feeds a TOKEN to the edict and keeps
its nontoken permanents. **The take-the-steal line will essentially NEVER validate in random-pool
selfplay** because the sacrifices the deck forces are, in practice, token sacrifices. It requires a
FORCED nontoken discard (Hymn/Thoughtseize with Tergrid out — which needs the deck to draw Tergrid
AND a discard spell AND cast the discard while Tergrid lives) or a targeted single-permanent edict
against a token-less board. The random pool has not produced that intersection in THREE waves
(wave-23: 1 decline, no real window; wave-24: 0 windows; wave-25: 0 genuine windows despite Tergrid
on board and forced sacrifices firing).

**Engine confirmation the steal IS implemented** (so the debt is validation, not a missing feature):
`Res/sets/primitives/borderline.txt` Tergrid front face carries
`auto=@sacrificed(*[-token]|opponentbattlefield) restriction{isflipped==0}: ... transforms((newability[choice name(Put on battlefield) moveTo(myBattlefield)], newability[choice name(Don't put on battlefield) moveto(ownerGraveyard)]))`
and the parallel `@discarded(*[-instant;-sorcery]|opponenthand)` line. The steal modal ("Put on
battlefield / Don't put on battlefield") exists and the token/instant/sorcery exclusions match Oracle.
The line is correct and un-fired, not broken.

**Conclusion:** the 2-wave (now 3-wave) validation debt is DISCHARGEABLE ONLY BY THE deck199 PROBE,
not by more corpora. See notes.md — this decouples the steal validation from the per-wave review loop.

---

## Section 2 — DFC round 3 (brief focus 2): Tergrid cast RULES-CORRECT (3rd time); Lantern never cast (backface bug un-re-exercised, 3rd wave); flip-thrash 0/6.

- **Tergrid cast: YES, vs131 seq27, turn 8.** `Cast Tergrid, God of Fright {3}{b}{b} (4/5)` -> seq28
  `Cast Card Normally` -> hand->stack->battlefield. It had 5 lands (Urborg + Swamps + Mortuary Mire +
  Thran Dynamo ramp) and cast the **{3}{B}{B} GOD FRONT FACE directly**, the rules-correct 4/5.
  Attacked with Tergrid t10/t12/t14 alongside Sheoldred and won.
- **Lantern back-face casts: 0 corpus-wide.** As in wave-24, the model reached the God's full 5 mana
  and cast the God directly, so the {3}{B} Lantern face was never cast. **R-DFC-BACKFACE-RESOLVES-FRONT
  remains UN-RE-EXERCISED (3rd consecutive wave)** — not resolved, not observed. Keep OPEN. Only the
  deck199 probe (a curated Lantern-affordable/God-unaffordable turn) can exercise it.
- **Flip-thrash: 0 Flip Side consumed across all 6 games** (wave-22: 11, wave-23: 1, wave-24: 0,
  wave-25: 0). DFC display-toggle demotion HELD, round 3.

---

## Section 3 — pw-redirect removal a4 (brief focus 3): VERIFIED — zero redirect records corpus-wide; player-aimed damage/discard hits the player with no menu.

- **Corpus-wide redirect-menu records at this seat: 0.** Scanned all 6 seat logs' chosen_text and the
  numbered choice blocks for "redirect" — zero. The deployed guide contains no "redirect" text either
  (grep count 0), so nothing in the prompt could seed the old menu.
- **Player-aimed spells hit the player directly, one decision, no redirect step.** Clearest cases:
  - vs137 seq8-9: `Cast Thoughtseize {b} - legal targets: you, the opponent` -> seq9 chose `The
    opponent (player, life 20)` in ONE target step. No intervening redirect menu.
  - vs109 seq5-6: `Cast Thoughtseize {b}` -> `The opponent (player, life 20)`. Same clean 2-step.
  - No Corrupt/Gray-Merchant-to-face DAMAGE spell was cast at a player this corpus (Gray Merchant fired
    as an ETB drain vs59/vs133 — not a targeted damage spell), so the exact Corrupt-to-player case is
    unexercised, but a4 is confirmed by the general absence of any redirect menu across every targeting
    decision. **No decision-shift vs prior corpora** other than the (correct) disappearance of the
    redirect step: the model no longer has a chance to mis-route. Net at this seat: strictly a
    simplification, no new confusion.

---

## Section 4 — 4/6 decisions-read; Pox discipline round 4; example-leak gone (brief focus 4).

**Record decomposition (decisions, not win column):**
- WINs vs131 (Tergrid+Sheoldred), vs133 (Gray Merchant/Syr Konrad grind, 23/-7 dominant — a real
  improvement over wave-24's dead-even timeout draw vs the same deck), vs27 (Steel Hellkite +
  Sheoldred), vs137-adj (ahead 13/9 at cap): all clean, no seat leaks. The deck presented threats and
  attacked — the guide's "your disruption is not a clock, deploy a threat and attack" line is landing
  (Steel Hellkite/Sheoldred/Tergrid beatdowns won 3 of the 4).
- LOSS vs59 (0/15, t13): Persist matchup; **t6 Smallpox correct** (below); no clear in-game leak.
- LOSS vs109 (0/16, t11): the pregame over-mulligan (Section 5) is the identifiable contributor,
  compounded by an aggressive matchup where starting on 6 down a card hurts.

**Pox/Smallpox discipline — round 4, HELD.** One symmetric cast: **vs59 seq14, Smallpox {b}{b}, turn
6, developed boards.** Reasoning disciplined and correct: opponent is a Persist deck; Smallpox forces
them to sac a creature ("removing one of their annoying Persist engines permanently") and a land, and
discard, while 102 loses one land + 1 life and sets up "a massive Gray Merchant next turn." Fired at t6
on a developed board with a follow-up plan — NOT while being raced low. Guide case-(b)/(c) spirit. Not
a leak (102 still lost vs59 — the grind, not the Smallpox). No forced-self-discard bomb-pitch surfaced.

**Example-leak / commit-lock (b1): STRUCTURALLY DEAD.** 0 fallbacks corpus-wide (vs the s5
`CHOICE: 2 (Cast Commander's Sphere)` out-of-range I classified last wave). The de-fanged protocol
example means the third commit-lock sub-shape I named in wave-24 (example-seeded out-of-range line-1
index) has no surface to reproduce on. Confirmed dead — the shape I owned is gone.

---

## Section 5 — Pregame mulligan: ONE over-mulligan misplay (vs109), a NEW class distinct from wave-24's miscount, and the ONLY corpus-evidenced guide gap.

- **vs131 (mull -> keep, bottom Pox): DEFENSIBLE.** Hand 1 = {Night's Whisper, Liliana's Triumph, 1
  Swamp, Reliquary Tower, Tinybones, Temple of the False God, Charcoal Diamond}. Genuinely awkward mana:
  1 real black source untapped turn 1, Temple dead until 5 lands, Charcoal Diamond enters tapped. Model
  read 1 Swamp correctly (no miscount) and mulliganed for smoother mana. Won. Fine.
- **vs59 / vs133 / vs137 / vs27: KEEP on 7** — all reasonable one-decision keeps.
- **vs109: MISPLAY — over-mulliganed a KEEPABLE hand.** Hand = {Tinybones, Arcane Signet, Swamp, Swamp,
  Swamp, Hymn to Tourach, Smallpox}. A clear KEEP by the guide's OWN criteria: 3 Swamps (2-4 ok), a ramp
  rock (Arcane Signet), premier early disruption (Hymn to Tourach — discard TWO turn 2), plus a 1/2
  body (Tinybones). The model mulliganed it, stating "no Turn 1 play... likely to be slow," wanting a
  turn-1 play.
  - **NOT wave-24's miscount** — the model read the hand correctly (3 Swamps, named Hymn). It is a
    JUDGMENT error: it treated "early black play" as "TURN-1 play" and shipped a hand that literally
    satisfies the guide's keep criteria (Hymn is explicitly listed there) to chase a turn-1 drop. Mulled
    to 6 (bottomed Palladium Myr), lost to the aggressive matchup down a card.
  - **Second consecutive wave with a mulligan misplay at this seat** (wave-24 miscount; wave-25
    turn-1-chase). Different mechanism, same surface — the pregame mulligan is where this seat's leaks
    now concentrate. Unlike the miscount (un-fixable attention slip), this one is a GUIDE-ADDRESSABLE
    AMBIGUITY: the guide's "early black play" did not define "early," and the model resolved it to
    turn-1. Resolving that ambiguity is legitimate calibration, not a crutch — the ONE guide edit this
    wave (strategy.txt) + skill.md.

REPPENALTY=1.05 (standing default): no degradation — coherent long replies, 0 fallbacks, no spirals.
