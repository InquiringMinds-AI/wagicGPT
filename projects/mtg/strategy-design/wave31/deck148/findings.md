# deck148 (Kor Army - mono-W equipment) findings - wave 31 CONFIRMING corpus / EXIT

Corpus: `matchups-20260727-025026`, binary `cf7cb2e53`, -T 3000, REPPENALTY=1.05.
Guide: **byte-frozen** (cmp-verified: deployed `bin/Res/ai/baka/deck148_strategy.txt` ==
wave30/deck148/strategy.txt == this wave's strategy.txt).
Record: **4/6** - the seat's best, up from 2/6 (wave 30) and 1/6 (wave 29, Step-0).

| game | result | end | note |
|---|---|---|---|
| vs139 | **WIN** | t9, 14 to -4 | fastest kill of the seat's campaign |
| vs158 | **WIN** | t10, 10 to -4 | **158's ONLY loss in the corpus (158 went 5/6)** |
| vs146 | **WIN** | t13, 12 to 0 | gear-the-lord-first converted end-to-end |
| vs122 | **WIN** | t14, 20 to -8 | zero damage taken; beat wave-30's 6/6 perfecta deck |
| vs152 | LOSS | t19, -2 to 2 | opponent left at **2 life**; died to the crack-back |
| vs137 | LOSS | t18, -4 to 1 | opponent left at **1 life**; land-flood + equipment destroyed |

**Zero adjudications at this seat** (all six games are real kills). **Zero fallbacks across
187 decisions** (ask 91 / attackers 34 / priority 37 / blockers 11 / bottom 2 / gamestart+gameend
12) - 2nd consecutive fully clean corpus. **Zero crash signatures in all six game stderr files**
(h1 / N-139h corpus gate contribution: PASSED at this seat).

All card facts below verified against `Res/sets/primitives/*.txt` `text=`/`auto=`; all numbers
read off the translog `prompt` render.

---

## 1. GUIDE VALIDATION - all four teaches fired again, one with a verbatim citation

### Teach #1 - ATTACK FLOOR: VALIDATED, universal
The deck attacked on **every one of its turns on which a legal, non-sick attacker existed, in all
six games**. First-attack turns (translog `turn` field): vs139 **t3** (Duelist cast t1) then
t5/t7/t9; vs158 **t4** (Pilgrim cast t2) then t6/t8/t10; vs146 **t5** then t7/t9/t11/t13; vs152
**t4** then t6/t8/t10/t14/t16/t18; vs137 **t3** then t5/t7/t9/t11/t13/t15/t17 (eight consecutive
attack steps); vs122 **t4** then t6/t8/t10/t12/t14. The only turns with no attackers record are
turns on which every body was summoning-sick or the board was creatureless (e.g. vs152 t12, board
wiped by two Fateful Absence + a Vanishing Verse the turn before). The wave-29 durdle-to-t11 leak
remains dead across three corpora.

### Teach #2 - GEAR ARMAMENT MASTER FIRST: VALIDATED, and now CITED VERBATIM
AM reached the battlefield in two games. **vs146 seq19 t9** is the strongest teach evidence the
seat has produced - the model's own reasoning quotes the guide:

> "The strategy guide explicitly states: 'GEAR ARMAMENT MASTER ITSELF FIRST; IT IS YOUR BEST
> ENGINE.' ... since Armament Master is the engine that pumps *other* Kor creatures based on
> equipment attached to it, this maximizes our board presence."

It equipped Captain's Claws to AM (not to the two Kor allies, not to the Pilgrim) and attacked the
same turn. The engine paid it off exactly as the primitive specifies
(`this(gear = 1) lord(other creature[kor]|myBattlefield) 2/2`): vs146 narration turn 10 shows
**three Kor allies dealing 3 each** (1/1 + 2/2 lord) **plus AM for 3** = 12 damage in one swing,
opponent 19 -> 7. That swing is the game.
The second exercise (vs152) never happened - see the one decision leak in section 3.

### Teach #3 - EQUIP KOR DUELIST FOR DOUBLE STRIKE: VALIDATED, best execution yet
`Kor Duelist` primitive: `auto=this(gear > 0) double strike`, base 1/1, `mana={W}` - guide
faithful. Exercised in three games:
- vs139 seq10 t3 Bone Saw -> Duelist; seq14 t5 Cranial Plating -> Duelist; seq22/23 t9 two
  Captain's Claws -> Duelist. Narration turn 8: **"Damage: 6 dealt by Kor Duelist"** to a blocker.
- vs137 seq9 t3 Bone Saw -> Duelist (the deck's only equipment all game).
- vs122 seq17/18 t8 Captain's Claws x2 -> Duelist; seq24 t12 Cranial Plating -> Duelist. Final
  render at seq30 t14: **`Kor Duelist {w} (10/1) (printed 1/1) [double strike]`** with three
  equipment attached and six Kor Ally tokens beside it, opponent at 2, deck148 still on 20 life.
  Narration turn 13: "Damage: 10 dealt by Kor Duelist to Serra Ascendant."

The guide's "any equipment turns it into a real threat" line is doing exactly what it was written
to do, and the model has learned to STACK onto the Duelist rather than spread.

### Teach #4 - DEPLOY OVER EQUIP WHEN CREATURE-LIGHT: HELD (no violation)
No game shows mana spent on equipment while creature-light. vs137 is the test case: from t7 to t17
the model's only castable card most turns was a Plains, and it never dumped its (empty) equipment
hand into nothing; vs152 t13 it cast Oath of Gideon (bodies) over holding for equipment.

### Teach #5 (MULLIGAN): fired twice, both times on a creature-light hand, once WRONGLY
vs137 seq1 t0 correctly shipped a 4-Plains / Claws / Plating / Claws / AM hand ("critically **zero
creatures**") and the keep at seq2 was a textbook guide keep (3 Plains + Kor Duelist). vs146 seq1
is a **false mulligan**: the hand rendered as
`Bone Saw [artifact]; Kor Duelist {w} (1/1); Bladed Pinions {2} [artifact]; Plains (land: taps for
{W}); Emeria, the Sky Ruin (land: taps for {W}); Plains (land: taps for {W}); Bone Saw [artifact]`
and the model wrote **"This hand has zero creatures."** Kor Duelist is right there - a turn-1
castable creature with two Plains, i.e. an explicit KEEP under the guide's own rule. This is a
STATE misread, not a teach failure; the surface cause is diagnosed in notes.md (N-148d) and routes
to the representation layer per the perception-vs-strategy doctrine. Impact was absorbed (it
mulliganed to 6 and won anyway).

---

## 2. THE 4/6 READ vs THE CEILING ASSESSMENT: **CEILING UNCHANGED**

Short answer: **the construction-ceilinged assessment does not change.** The record moved because
the POOL moved, and this deck's record is maximally sensitive to pool composition. Three
independent lines of evidence, all from this corpus:

**(a) Four of six matchups FLIPPED against wave 30 - the record is not a trend, it is noise around
a coin flip.** wave-30 -> wave-31 per matchup: 146 L->W, 122 L->W, 139 L->W, 152 W->L, 158 new W,
137 L->L. Only 137 is stable. Combined guided record is **6/12**, i.e. ~50% with n=12; 2/6 and 4/6
are both ordinary draws from that. Nothing about the deck changed between the corpora (guide
byte-identical, same 60 cards).

**(b) The pool's ADJUDICATION RATE collapsed, and that alone moves this deck.** Wave 30 ran 12
cap-adjudications of 21 games; this corpus ran 3. deck148 lost TWO wave-30 games by adjudication
while alive (vs122 16-20 t8, vs137 behind 11-21 t12) - a deck with a clock and no stabilizer is
the archetype most punished by a cap-tiebreak and most rewarded by its absence. This is wave-30
method headline 8 running in reverse: this wave's 4/6 has the same structural component that
wave-30's synthesis correctly discounted in deck122's perfecta, just with the sign flipped.

**(c) The LOSSES prove the ceiling is exactly where it was.** In BOTH losses deck148 got the
opponent to **2 life** (vs152) and **1 life** (vs137) and then had nothing. That is the ceiling
signature stated precisely: the shell reliably builds a clock that arrives at ~lethal and owns no
card that converts the last two points - no removal, no reach, no card advantage, no way to
survive one crack-back. vs152: at t19 deck148 attacked for 9 into an opponent on 11, then died -2
to Tovolar's Huntmaster (6/6) + Briarbridge Tracker (5/4) + two Wolves, all deployed on one turn.
vs137: it attacked eight consecutive turns, drew SIX consecutive Plains (t7-t17), had its single
Bone Saw destroyed at turn 11 (Flaxen Intruder sac trigger) which cost Kor Duelist its double
strike, and finished the opponent's life at 1.

**Did the new aggro matchup 158 reward the deck's speed?** Partly, and it is worth naming honestly.
deck158 (RB amass) went 5/6 and **its only loss in the entire corpus was to deck148.** The race was
won on Captain's Claws token accumulation (`@combat(attacking) source(mytgt):token(Kor Ally,...)`,
verified in the primitive) out-pacing amass growth: Stone Haven Pilgrim + Claws hit for 4 at
narration t5, 5 at t7, then after 158's two Feed the Swarm killed the Pilgrim and one ally the
Claws had already migrated to a token and the leftover allies finished at t10. So yes - **against a
creature-light racing deck with no early blockers, deck148's clock is genuinely favored.** But that
is a MATCHUP-SHAPE fact about a one-game sample, not a change in the deck's power: the same six
games contain two razor losses to decks that simply had bigger cards. Replacing a grindy midrange
(136) with an aggro deck (158) shifts this deck's expected record without touching its ceiling.

**Conclusion:** wave-29 called it ceilinged, wave-30 confirmed it and defined the exit, wave-31
confirms it again under a materially different pool. The deck's guide-side potential is realized;
its variance is wide; the lever is construction. The construction hand-off is in notes.md.

---

## 3. THE ONE DECISION RESIDUAL: trading the lord (single witness, vs152 seq10 t7)

Honest counterweight to "it is all construction". One decision this corpus was clearly wrong and it
plausibly cost the vs152 game (lost by 2 life):

**vs152 seq10 (t7, blockers).** Board: deck148 at **20 life**, opponent at 14, one attacker - a
2/2 Wolf token. Sole legal blocker: the **Armament Master cast the previous turn, still ungeared**.
The engine's own line offered it as `B1. Armament Master (2/2) - may block A1 (both die)` and the
prompt header said *"NOT lethal: block only where the trade favors you; taking damage while ahead
is often correct."* The model blocked and traded its payoff engine for a Wolf token - one turn
after writing, in its own PLAN line at seq9, *"Next turn... equip Captain's Claws or Cranial
Plating to Armament Master to activate its team-wide pump ability."* Its stated reason was the
generic even-trade prior: *"trading my 2/2 for their 2/2 is the correct tempo move."*

That is not a construction loss - AM was alive, on 20 life, with the equipment in hand. It is the
absent-teach shape: the guide tells the pilot AM is the best engine and to gear it FIRST, but never
tells it AM is not a legal trade, so at a seam the guide does not cover the model fell back on
generic Magic value logic that a lord deck must not use. Note the second AM in the same game
(seq15 t11) blocked a 1/1 Luminarch Aspirant and survived (fine) and then died to removal (not a
decision).

**Routing:** this does NOT reopen the guide (see below) - it is promoted to the SKILL as a
general teach category, because it is not a deck148 fact, it is a fact about every deck whose plan
hangs on one fragile enabler. See skill.md.

---

## 4. GUIDE DISPOSITION: UNCHANGED / FROZEN

`strategy.txt` in this directory is a cmp-verified byte copy of the deployed guide. No edit.
Reasoning against the two candidate edits:
- **The AM-trade line (section 3).** The rail is a proven MISLEADING teach; this is an ABSENT teach
  with one witness at a guide already at terminus. Adding per-deck prose to a guide that is being
  frozen and rotated out is exactly the wave-25 headline-5(c) shape (a general behavior routed into
  a per-deck floor). Routed to skill.md instead. The exact candidate sentence is preserved in
  notes.md in case the roster owner rebuilds the deck and a new guide is authored.
- **Stone Haven Pilgrim's lifelink (N-148c).** The guide says the Pilgrim "hits for 3 and gains you
  3", which is faithful to BOTH the Oracle text and the engine's own `text=`/`auto=`
  (`...:1/1 ueot && lifelink ueot`). The engine does not deliver the life (two arithmetic witnesses
  in notes.md). Rewriting the guide to describe the bug would be a stopgap that the engine fix must
  later invert - wave-30 method headline 3 says do not do that. Engine ledger, guide untouched.

---

## 5. h-batch and open-ledger steady state at this seat
- **h1 (ManaCost OOB fix)**: 0 crash/assert/abort signatures in all six stderr files. PASS.
- **h2 convoke / h3 reveal-chooser / h4 MDFC / h6 walker magnitudes**: N/A - mono-white equipment
  casts no convoke spell, runs no tutor/reveal, no MDFC, no planeswalker. The seat's only
  remove-from-hand asks are the two `bottom` decisions (vs146 seq3, vs137 seq3), both rendered as
  clean card lists with correct type tags, both answered sensibly (bottom the dead equipment).
- **h5 (display-toggle-only priority auto-pass)**: priority decisions 49 -> 37 wave-30 -> wave-31
  (8.2 -> 6.2 per game); directionally consistent with the fix but **confounded** by game length
  and board composition (attackers rose 20 -> 34 over the same games). Not isolable at this seat -
  report as directional only, do not credit.
- **g4 tapped-tag** (`[tapped - cannot attack or block this turn]`): renders on both boards, no
  misread anywhere in 11 blocker decisions. Third clean corpus.
- **L-18-1 planning tax**: the >10k-char spiral shape RECURS (6 replies over 10k, max **14,120 ch**
  at vs139 seq21 t9; max latency 210.5 s) but produced **zero** truncations, zero abandonments and
  zero wrong commits. Every one of them sits on a ubiquitous low-stakes seam (a `Cast Captain's
  Claws` ask, a priority pass) - never on attackers or blockers. This is a clean confirming data
  point for the wave-25 headline-5 SPIRAL-RELOCATION CEILING: the high-stakes seams stay short and
  correct, the residual migrates to seams no per-deck guide can reach, and it stays a LATENCY
  concern. No action.

---

## 6. EXIT VERDICT: **ROTATE OUT. Guide FROZEN. Construction handed off.**

The wave-30 gate was "hold ONE confirming corpus, then rotate out with a construction hand-off."
That corpus is delivered and it confirms on every axis it was meant to test:
- all four (five, counting mulligan) teaches fired again under a materially different pool, one of
  them with a verbatim guide citation - the guide is internalized, not merely present;
- zero fallbacks, zero crashes, zero parser contradictions, zero adjudications;
- the record moved to 4/6 for pool-composition and matchup reasons that decompose cleanly, with
  BOTH losses ending with the opponent on 1-2 life - the ceiling is exactly where wave 29 put it;
- there is **no unexercised guide surface left** and no guide edit is warranted.

What would have held the seat, and does not: (a) the AM-trade leak is a general teach category, not
deck148 guide work -> skill.md; (b) the four representation/engine items (N-148a..d) are
engine-lane and are better closed by a fix agent plus a targeted probe than by a rotation slot -
exactly the reasoning that rotated 136 out last wave; (c) Zamriel, Argentum Armor and Kabira
Crossroads were never cast in 12 guided games, but under wave-30 method headline 5 they are
draw-variance residuals on already-proven decision surfaces (an equip option, a creature cast), not
distinct unvalidated shapes.

What genuinely reopens a seat here: **the roster owner acting on the construction hand-off.** A
deck148 that has traded its equipment glut for bodies and interaction is a materially different
deck and deserves a fresh Step-0 -> first-guided arc under a new seat, not a continuation of this
one. Until then, deck148 belongs on the frozen/converged canary list.
