# Wave-41 seat report: deck125 "Revelation Fracture" (harvested 2026-08-24)

[Seat-returned report, orchestrator-filed. 543 decisions, 0 fallbacks, 6/6 natural.
Seating verified against BOTH seats' gameend records. Record 5-1 (W 152 t50, 146 t35,
126 t41, 123 t40, 130 t66; L 162 t25 at -8/18).]

## Per-edit verdicts (9 wave-40 edits, from git diff 0f6f7996a f5ade6b24)
- E1 Emrakul attacks: 3/3 followed, CAUSAL (152 s80 took opp 25->10; 126 s83 2->-13).
  Wave-40 baseline 0 attacks with "we don't attack with Emrakul" trace. Blocker half null.
  COLLISION: "Cast Emrakul" escape clause went 2/7 (was 2/2) — headline "EMRAKUL IS NOT
  YOUR PLAN" outshouts it; trace quotes "guide says it's not the plan". Cost nothing
  (both games won) but live contradiction -> R3.
- E2 Staff-with-counter-open: 5/9 casts followed; partial.
- E3 numeric counter rule: 34/36 + 10/10 let-resolve correct; 0 counters on walls/mana/
  0-1 power (wave-39: 9/25 wasted). Three "violations" vs162 (Puzzle Box, Dictate,
  Forced Fruition — no P/T, rule doesn't reach) were CORRECT Magic -> rule gap R4.
- E4 Path floor: 53/54 wall-only windows declined (was 6/13 spent). CONFOUNDED: wave-41
  lane D target-preview facts print the deciding fact on the option line — guide edit and
  render fix are co-causes, not isolable this corpus. Two Blastminer Paths below floor
  were CORRECT (land-destruction threat; verified mtg.txt) — no edit, deviation was right.
  One MISFIRE: 162 s60 life-4 branch spent Path on Shield Sphere 0/6 [defender] -> R1.
- E5 sweeper exactly-1 split: 0 windows in 543 decisions — HONEST NULL. Carry one more
  wave, then cut or probe-force.
- E6 Lightmine gate: 83/84 (was 1/3). One misfire at 0 creatures MP2 13 idle mana -> R2.
- E7 Revelation floor + stop-at-opp<=10-with-Staff: 39/39 declined (wave-40 identical
  predicate: 2/2 TAKEN, one from library 1 = lost a won game). Min library 1 -> 24;
  self-decks 1 -> 0; same matchup vs123 flipped loss(70/7)->win(49/0). THE DECISIVE EDIT.
  X menus 10/10 largest X; N<=5 floor + X<=N-2 cap honest nulls (min library 24).
- E8 re-ask stopgap retirement: 0 occurrences of the shape; 28 adjacent identical pairs
  all legitimate. Neutral, no hole.
- E9 numeric phrasing: folded into E3.

## The loss (vs162): matchup loss + guide gap, not adherence
Counter discipline near-perfect; E2 held. Kill mechanism: third Fate Unraveler +
Underworld Dreams made every draw cost 2 life; deck's own engine (Staff upkeep draw,
uncontrollable Mine/Dictate draws) became the opponent's clock. NOTHING in the guide
addresses being punished for drawing; counter-on-sight list keyed to names not shape -> R4.

## Swing attribution: EDITS, not noise
5 of 6 opponents held over; seat flipped 4 head-to-heads, lost the same one. No holdover
opponent degraded (146 4-2->4-2, 126 3-3->3-3, 152 4-2->5-1, 162 2-4->3-3, 123 1-5->1-5);
vacated wins belong to exited 139 (6-0). Direction supported (null-model max likelihood
0.0017); effect size NOT measurable at n=1/matchup — do not quote. Per-game: vs123 = E7
direct counterfactual; vs152 = E1 strong; vs126 = E1+E6+E4 supported (combo DID assemble
— seat just killed 7 turns faster); vs146/vs130 unattributed. Standing confounder: the
wave-41 engine batch shipped between corpora; games ~2x longer — rates only.

## Engine-ledger INFO items
- No-op churn grows with game length: adjacent identical pass/Cast-nothing pairs n=1
  (wave-40) -> ~10 in vs146 alone.
- E4 confound test (cheap): cut the "find each of those names..." sentence, re-measure
  decline rate — tells whether the paragraph still earns its tokens now that the render
  carries the fact.

## Recommended edits R1-R5 (verbatim, deploy-ready)

R1 — replace "Your life is 12 or less: that floor is off - exile the biggest creature they
have right now." with:
Your life is 12 or less: that floor is off for creatures that can attack - exile the one with
the highest printed power. A creature tagged [defender] still never gets the Path, at any life
total: it cannot attack, so it is not what is killing you.

R2 — extend the Lightmine bullet in SWEEPERS:
- 3 or more creatures, and Lightmine Field is in your list: cast Lightmine Field. At 0, 1 or 2
  creatures on that header it kills nothing at any size, so keep it and spend the mana on a Path
  or a sweeper instead - unless it is your second main phase, nothing else in your list is worth
  casting, and your "Mana available:" line still reads 4 or more: then cast it anyway, so it is
  already there when the crowd arrives.

R3 — append to the Emrakul escape-clause paragraph:
This line outranks the heading above it. "Emrakul is not your plan" describes what you do while
Emrakul is uncastable; the moment "Cast Emrakul, the Aeons Torn" is printed in your numbered list
it IS castable, so you take it - including on a turn when the opponent is nearly dead and your
Staff would finish them anyway, because Emrakul finishes them sooner.

R4 — add one bullet to COUNTER ON SIGHT:
  - anything whose text makes a player DRAW extra cards, whatever its printed type: Howling Mine,
    Teferi's Puzzle Box, Dictate of Kruphix, Forced Fruition. This one outranks the "(artifact)
    with no P/T: let it resolve" line above. Extra cards cost you life whenever they have
    Underworld Dreams or Fate Unraveler out, and cost you library either way.

R5 — DELETE the unexercised mulligan line (0 occurrences in three consecutive corpora):
"If the mulligan question ever contains the words "having already taken", this is your LAST
look: answer 1, Keep this hand. The thought "I will keep whatever I draw next" is how you end
up with a 4-card hand - keep THIS one instead."

## Falsifiable predictions (next corpus)
1. E7 holds: >=95% declines at opp<=10+Staff; min library >=15; falsified by any self-deck.
2. R3: Emrakul offers taken >=6/7; zero "not the plan" traces in offer windows.
3. R1: zero defender-Paths at any life.
4. R2: 0-creature Lightmine casts only MP2 at >=4 leftover mana; >=3 gate holds >=95%.
5. R4 testable only if 162 stays in pool; else HONEST NULL, not a pass.
6. E4 confound-resolution probe available (cut sentence, re-measure).
7. No-op churn scales with game length (engine INFO, not guide).
