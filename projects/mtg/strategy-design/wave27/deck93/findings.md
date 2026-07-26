# deck93 (Relentless Rats) - wave-27 findings (STEP-0 GUIDELESS DEBUT, 2/6)

Reviewer seat: deck93, mono-B Relentless Rats (24 Swamp / 36 Relentless Rats). Binary
ffec79fd8. Corpus matchups-20260725-193608. This seat is the DELIBERATE d1 stressor
(36 identical Relentless Rats). TWO jobs: (1) d1 R-DUPLICATE-NAME-INSTANCE systematic
audit; (2) Step-0 read + initial guide.

Record decomposition (2/6): WIN vs134, WIN vs136; LOSS vs22, vs131, vs137(adj), vs18.
99 model decisions across 6 games, 0 fallbacks (parser/binding held perfectly).

Card fact (Scryfall + primitive both verified): Relentless Rats, {1}{B}{B}, base 2/2,
"gets +1/+1 for each OTHER creature you control named Relentless Rats." Primitive
mtg.txt:93698-93706: auto=foreach(other relentless rats) 1/1, power/toughness 2.
FAITHFUL. So with N Rats total each Rat is (N+1)/(N+1). Confirmed at every board render:
2 Rats -> 3/3, 3 -> 4/4, 4 -> 5/5. Render arithmetic is correct.

================================================================================
PART 1 - d1 R-DUPLICATE-NAME-INSTANCE SYSTEMATIC AUDIT  ->  VERDICT: CLEAN PASS
================================================================================

## 1a. RENDERS - correct and stable (PASS)

Programmatic scan of every board render across all 6 deck93 games: 25 own/opp
battlefield lines containing Relentless Rats handles, 0 anomalies - every render is a
contiguous #1..#N with no duplicates, no gaps, no collisions. Swamps ALSO collide-render
Swamp #1..#8 cleanly alongside the Rats.

State decoration is correct on the handles:
- Relentless Rats #1 {1}{b}{b} (5/5) (printed 2/2) - live P/T + printed P/T both shown.
- [tapped - untaps and can attack next turn] on tapped instances (seq17/20 vs136).
- Handles are STABLE within a decision: seq20 vs136 board shows #1,#2 (both tapped)
  and #3 (untapped); the blocker line relabels the untapped one B1. Relentless Rats #3
  - the board #3 handle is CARRIED into the B-line. No renumbering mid-decision.
- Handles reindex ACROSS decisions after a death (Rat #3 dies at seq20 -> next turn the
  survivors renumber) - by-design battlefield-order behavior, not instability.

26 A-line / B-line entries carry the #N handle (e.g. A1. Relentless Rats #1 (5/5),
A2. Relentless Rats #2 (5/5)). Attacker/blocker slots are correctly keyed to board #N.

## 1b. MODEL USAGE - the model references #N naturally (PASS)

The pilot consumes the handles in reasoning and plans, e.g. vs136 seq17: "My Relentless
Rats #3 is a 4/4." and "PLAN: Block Knight of the Ebon Legion with Relentless Rats #3 to
prevent 7 damage." It reads the specific instance, not just "a Rat."

## 1c. BINDING CORRECTNESS - robust; NO mis-bind found (PASS)

Two stress cases where the model referenced MORE slots than legally existed; in BOTH the
engine bound only the intended-legal instance and dropped the hallucinated slots. No
mis-bind, no wrong-instance hit.

STRESS CASE A (vs134, seq23, turn 13, attackers): board = 4 Rats all 5/5, but #3/#4 were
cast THIS turn (summoning-sick) so the A-lines correctly offered only A1. Relentless Rats
#1 and A2. Relentless Rats #2. Model replied "ATTACK: A1, A2, A3, A4" (expecting "20
damage, lethal"). Parser kept A1+A2, dropped A3/A4. choice=2. Narration (seq24) confirms
opponent went 14 -> 4 = exactly 10 damage (2 Rats x 5), NOT 20. Binding hit the two
intended-legal Rats; the model's lethal math was wrong, the binding was right.

STRESS CASE B (vs136, seq20, turn 15, blockers) - THE key duplicate-name binding test:
board = Rats #1,#2 (both [tapped], attacked last turn) + Rat #3 (untapped). Sole attacker
= Knight of the Ebon Legion 7/8 deathtouch. Engine offered exactly ONE blocker: B1.
Relentless Rats #3 (4/4) - the tapped Rats #1/#2 were correctly EXCLUDED, and the #3
handle carried into B1. Model replied "BLOCKS: B1:A1, B2:A1, B3:A1" (tried to gang all
three). Parser bound B1:A1, dropped non-existent B2/B3. Narration (seq21): "You declared
blockers: Relentless Rats blocks Knight of the Ebon Legion" and "Your Relentless Rats:
battlefield -> graveyard" - exactly ONE Rat blocked and died, not three. The legality
gate bound the intended-legal instance and SAVED the model from its own catastrophic
over-block. Textbook correct handle+legality behavior.

## 1d. PROMPT-SIZE / LATENCY IMPACT OF 36 IDENTICAL NAMES - negligible (PASS)

The 36-Rat decklist lives in the CACHED static system prompt (head), not the per-decision
user message. Per-decision prompts are compact (first-prompt 1075 chars, 6 name mentions).
deck93 reply lengths: p50 925 / p95 4133 / max 5994 - SHORTER than deck137's wave-26 p50
1107 / p95 9919. No length or latency degradation attributable to the duplicate names.
(Absolute latency p50 15s / p90 40s is the corpus-wide -j concurrency artifact.)

## d1 AUDIT BOTTOM LINE
R-DUPLICATE-NAME-INSTANCE is VALIDATED at the deck93 stress seat: stable contiguous #N
renders (25/25), handles carried into A/B slots (26 lines), model uses them, every binding
hit the intended-legal instance with hallucinated extra slots safely dropped. NO
top-severity finding. The one thing the fix does NOT do - and should not - is stop the
model from REFERENCING illegal slots; that is a model-adherence leak (L2 below), not a
handle/binding defect.

================================================================================
PART 2 - STEP-0 MODEL LEAKS
================================================================================

## L1 (TOP, guide) - MULLIGAN LAND-BLINDNESS: mulls perfect 3-Swamp hands as "zero lands"

The identical opening hand "Swamp; Swamp; Swamp; Relentless Rats x3" was dealt in 6 of 6
games and MULLIGANED in 5 of them, only kept once. The reasoning is uniformly a false
"zero lands" claim while holding three Swamps:
- vs22 seq1:  "My opening hand has zero lands. With 0 lands, I cannot cast any..."
- vs134 seq1: "The hand has zero lands and three copies of Relentless Rats..."
- vs137 seq1: "This hand has zero lands to cast the Relentless Rats..."
- vs131 seq1: "This hand has zero lands... With only Swamps and Rats, I cannot play a
              single spell on turn 1 or 2..."
- vs18 seq1:  "This hand has zero lands that can produce mana (Swamps produce black mana,
              but Relentless Rats costs {1}{B}{B}...)" <- the tell: it KNOWS Swamp taps
              for B, yet still says "zero lands" and ships.
- vs136 seq1 (KEEP): "This hand has 3 Swamps and 3 copies of Relentless Rats. While I
              cannot cast a Rat on turn 1 due to the {1}{B}{B} cost requiring 3 mana..."
              - SAME hand, correctly counted, correctly kept.

So it is a STOCHASTIC misread of the identical hand (kept 1/6, shipped 5/6), not a stable
belief. The trigger appears to be "cannot cast on turn 1/2" -> it mislabels the whole hand
"zero lands." Cost: 5/6 games shipped to 6 cards for free.

WORSE - vs131 compounded it: after mulling the 3-Swamp hand it KEPT the replacement
"Relentless Rats x6; Swamp" (ONE land) reasoning "6 threats and 1 land is superior to
shuffling." It then cast ONE Rat in 14 turns, dealt ZERO damage (opponent flat at 20),
died at -2 turn 14. That game was lost outright at the mulligan: shipped a 3-lander and
kept a 1-lander - strictly backwards. Single largest loss driver at this seat.

## L2 (guide) - references illegal attacker/blocker slots; builds lethal math on them

The model reads the board's N creatures and assumes all N can act, ignoring the A./B.
legal lines and the tapped/summoning-sick tags:
- vs134 seq23: 4 Rats on board, only A1/A2 offered; replied "A1,A2,A3,A4", "deals 20
  damage... lethal" - actual 10, opp not dead (engine dropped A3/A4).
- vs136 seq20: 1 legal blocker offered; replied "B1:A1,B2:A1,B3:A1".
Engine drops the illegal slots safely (0 fallbacks, no mis-bind - see 1c), so this never
corrupts state, but the model's PLAN and combat math are built on phantom attackers. Fix
is a guide line: only creatures on the A./B. lines can act this turn; a Rat you just cast
(summoning sick) and a [tapped] Rat cannot.

## L3 (guide) - over-blocking: trades the go-wide Rat board away on defense

vs136 seq20: opponent at 1 LIFE, attacking with a 7/8 deathtoucher. The prompt literally
rendered "Unblocked, these attackers deal up to 7 - you would be at 12 - NOT lethal:
block only where the trade favors you; taking damage while ahead is often correct." The
model IGNORED it and tried to gang all three Rats into the Knight to kill it - throwing
its entire board to remove an irrelevant blocker while one attack next turn was lethal
(opp at 1). Only the legality gate (2 Rats tapped) prevented losing all three. The model
never once noted "opponent is at 1 life, I am about to win." Two compounding leaks:
(a) doesn't factor opponent life / own near-lethal into combat; (b) doesn't value board
WIDTH - and for Relentless Rats, losing a Rat shrinks EVERY surviving Rat (lord-of-each-
other), so trading Rats is doubly costly.

## L4 (strategic) - never reaches critical mass; feeds Rats into one-for-one trades

vs22: kept a 2-Swamp/5-Rat hand (after mulling the 3-Swamp hand), resolved 4 Rats but 3
died one at a time to the Giants deck; final board 1 lone Rat; died turn 12 at -2.
vs137: kept a good 4-Swamp/4-Rat hand, resolved 6 Rats but 4 died; attacked A1,A2 (only 2
3/3 Rats) every single turn (seq10/13/16/19/23) chipping while the board never widened;
lost adj 2 v 6. The pattern: the pilot attacks a small number of Rats into boards where
they trade, bleeding the team, and never builds the insurmountable wide board the deck is
designed to assemble. Guide: hoard to a safe critical mass, avoid unfavorable Rat trades,
then alpha-strike wide.

## Peak scaling observed
Max Rats the pilot ever had on its own board in any game: 4 (vs134 seq23). The deck's
whole engine is board width, and the pilot rarely gets past 2-4 Rats - partly the
mulligan tax (L1), partly bleeding Rats into trades (L4). The two WINS (vs134, vs136) are
the two games it kept a 3-4 land hand and got to 3-4 Rats.
