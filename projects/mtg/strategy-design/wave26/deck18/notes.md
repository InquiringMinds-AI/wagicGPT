# deck18 notes (engine/harness/card-script items) - wave-26 Step-0

Layer-routing: these are engine/harness/model items with seq repros, NOT guide prose.
Rotation verdict is at the bottom.

## N-18a (MODEL, correctness-neutral tax) - lord/anthem P/T re-derivation despite the
   rendered "(printed X/Y)".

The board already renders current P/T. The pilot recomputes anyway, 7k-14k chars/spike
(vs102 s14; vs131 s18/s22; vs133 s12; vs27 s11/s13/s19/s23). Not a NEW engine item - the
"(printed X/Y)" representation is already shipped and correct; this is the known
model-property spiral that the guide trust-line + repetition_penalty pilot address. Logged
so latency-budgeting sees deck18 among the highest-tax seats (its cap-adjudicated vs137
loss is partly a p90-latency loss - decision-dense stat-stacking + block spirals).

## N-18b (MODEL, combat comprehension) - flying not applied when blocking; illegal
   ground-blocks-flyer attempt.

vs137 s10 (blockers): reply concluded and coded `BLOCKS: B1:A1` = Ballynock Cohort (no
flying/reach) blocks Faerie Guidemother (1/1 FLYING). Illegal; engine correctly dropped
to no-blockers (took 6, L19->13). The opponent line DID annotate "[flying, adventure]" -
the fact is rendered, the model did not apply it. Repro-verbatim of the final coded line
is in findings.md Leak 2 (with the c4-gate decomposition confirming this is a CORRECT
rules-based drop, not a parser-hardening misfire). ENGINE/REPRESENTATION angle (optional,
low priority): the blockers prompt could annotate which of the pilot's blockers can
legally block each attacker (mirror of the cast 601.2c filter / target-legality surfacing)
so an illegal flyer-block is never emitted - but this is a broad change; the guide race
line is the interim, and flying-block illegality is arguably a core mechanics-fact gap
(general-suggestions candidate A). Do NOT open engine work on one seat.

## N-18c (INTERFACE/REPRESENTATION) - Mobilization activated-ability discoverability.

Mobilization `auto={2}{W}:_SOLDIERTOKEN_` surfaces only as a priority-window option
("Create soldier with Mobilization [cost: {2}{w}]"), not in the main-phase cast list. The
pilot burned 3.5k chars (vs137 s13) concluding "the interface doesn't offer an Activate
Mobilization button" before eventually finding it at upkeep priority (vs137 s16/s20, vs27
s23/s25/s28/s29). Same family as the deck49 target-sub-menu interface seam. No game lost;
tax only. Interim = the guide interface note (strategy.txt Mobilization line). Durable
angle if it recurs across enchantment/artifact activated-ability decks: surface an
activatable permanent's ability in the SAME decision window as casts when payable, or note
in the cast-list narration that "activated abilities appear at priority." Tag single-seat.

## N-18d (CARD-SCRIPT / ORACLE-VERIFY, per wagicgpt-verify-oracle-text) - engine vs real-MTG
   divergences on deck18 cards. The GUIDE uses engine behavior; these are flagged for a
   later oracle pass, NOT guide changes.

Verified from bin/Res/sets/primitives/mtg.txt:
  - **Zealous Guardian**: engine = {W/U} 1/1 Kithkin Soldier, `abilities=flash`, NO
    defender. Real card = {W} 2/2 Defender with flash. Engine version CAN attack and is a
    Soldier (gets Field Marshal +1/+1 + first strike). The guide (correctly) treats it as
    an attacker. DIVERGENCE: cost {W/U} vs {W}, stats 1/1 vs 2/2, and defender DROPPED.
    Worth an oracle-verify - the missing defender changes the card's role substantially.
  - **Thistledown Liege**: engine = {1}{W/U}{W/U}{W/U} 1/3 Kithkin Knight, lord (other
    white +1/+1, other blue +1/+1). Real card = 4/4. Engine body is 1/3 - a buff, not a
    beater. Guide reflects the engine 1/3. DIVERGENCE: base P/T 1/3 vs 4/4. Verify.
  - The other eight cards match real Oracle in the load-bearing respects (Goldmeadow Dodger
    cantbeblockedby power>=4; Field Marshal lord other-Soldier +1/+1 & first strike;
    Wizened Cenn lord other-Kithkin; Cenn's Heir attack-trigger per other attacking Kithkin;
    Glorious Anthem +1/+1 all; Mobilization vigilance + {2}{W} token; Armored Ascension
    flying + per-Plains; Ballynock Cohort first strike + white-creature buff). No guide risk.

## ROTATION verdict

Step-0 seats do NOT rotate (per brief). deck18 debuts at 4/6 and produced a full initial
guide (strategy.txt) with a clear new-work signal: three teachable leaks (Dodger evasion
unused at combat, illegal flyer-block/race off-case, and the pervasive rendered-number
tax) plus an interface note and a strength to reinforce (the first-strike gang-block
engine). The initial guide TARGETS: (1) TRUST THE RENDERED [X/Y] as rule #1 to collapse
the 7k-14k spirals; (2) Goldmeadow Dodger as an explicit swing-every-turn evasion order
(the fact the model forgot in its loss); (3) the no-reach / can't-block-flyers RACE
off-case (the illegal-block loss); (4) the Field-Marshal first-strike gang-block as the
named defensive answer to big attackers; (5) Mobilization's activated-ability interface;
(6) a lean mono-white mana/mulligan line. STAY (Step-0 -> validate the initial guide next
corpus, per the deck59 pipeline: 3/6 Step-0 -> 6/6 first-guided).
