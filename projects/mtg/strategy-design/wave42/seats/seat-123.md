# Wave-42 seat: deck123 (2/6, was 1/6) — harvested 2026-08-24

334 decisions, 0 fallbacks, seating verified per-game.

Predictions: P1 coverage CONFIRMED (battlefield count 87/87, annotation never lies 0/61
disagreements) but OBEDIENCE FALSIFIED — 9 violations in 32 M>=12 decisions (72%):
mechanism from traces = (a) the take-it imperative appears 3x and OUTVOTES the
once-stated stop ("Each token is a free body" quoted verbatim in an override); (b)
NEW-TURN RESET — [repeat:] per-turn framing restarts the chain when the note resets
(6 of 9 violations are first window of a new turn). Honest counter-note: 2 of 9 were
correct lethal-defence blocking plays -> stop rule needs that exception, not more force.
P2 CONFIRMED (Alarm-before-maker vanished; all 3 Alarm picks correct — maker already
out). P3 CONFIRMED n=4 with a gap ("block with EVERYTHING" executed as 4-of-14,
left a 5/5 unblocked -> EDIT G mechanization). P4 CONFIRMED (0 invalid attacker ids).
P5 CONFIRMED LITERALLY but the failure returned through TWO NEW DOORS: multi-creature
edict (they hand over Fog Bank), STACK-vs-battlefield (edict cast at empty board with
Rorix on the stack — new failure class), and edict-at-1-life (put opponent from 1 to 4,
consequence written in its own PLAN). P6: rate held 2/6 BUT the recorded construction
lever "8 makers/8 Alarms/24 lands/17 removal" IS THE DECK AS-BUILT — retire that
prediction. Supported-by-measurement instead: assembly latency separates games (maker
by t6 = 2 wins + 1 near-win; t15/t20/never = losses); dead slots this corpus = 4x Vision
Skeins (now guide-forbidden in 2 of 6 matchups) + 3x Lightning Greaves. ROUTED TO OWNER
(Magic judgment; seat does not change decklists).

## Engine-lane observations
Instance handles LIVE and correctly reasoned about (#1/#2); [repeat:] counter is
PER-INSTANCE (up to 4 unannotated activations/turn with two makers — EDIT A words it);
token collapse + ceased-to-exist render correctly; SINGULAR render "of which 1 is a
creature" (broke a seat regex; guides quote only plural); Bloodline Keeper transform
never offered in 21 windows = NOT a defect (tapped out; Alarm untaps creatures not
lands — Oracle+primitive agree) -> guide gap EDIT D.

## Edits A-H (verbatim in task transcript)
A: RULE-1 rewrite — count-first procedure, handles, per-instance/per-turn note semantics,
   "does not reset on new turn", lethal-defence exception; DELETE "free body" line.
B: DECIDING SITUATIONS count-first bullets. C: REPLACE wave-41 EDIT 7 — fire the chain
   on the OPPONENT'S turn (evidence: t15 own-main 12-token build -> t16 Starstorm swept
   all, lost without one token swing; the old edit ACTIVELY DIRECTED it — an honest
   edit-misfire catch). D: Keeper transform mana note. E: stack-vs-battlefield + life
   floor for edicts. F: multi-creature edict ban with the Fog Bank example. G: block-
   EVERY-attacker mechanization. H: lone-maker attack hold-back.
Residuals: Skeins 7/8; Damnation 1 real miss; everything else clean (fetch 8/8, Slip
2/2 incl. Morbid -13/-13, mulligans 6/6, engine deployment 151/154).
