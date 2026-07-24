# Wave-24 review — deck109 (Hellrider, mono-red aggro / RDW) — trimmed-guide round 3

**Corpus:** PRIMARY `matchups-20260724-125739` (the un-quarantined second run). Binary
`/tmp/wagic-d7ff8b6c4` (step-1 batch 5f78e2934 + pregame phase 86df3b482 + N9 hotfix).
deck109 seat = 6 games. Pool: 27/131/59/133/102/137 + deck109. Guide FROZEN from wave-23
(md5 `a8f163f7f2bf1ecce5c9f4f13997ccf1`, deployed == wave23/deck109/strategy.txt, cmp-verified).
This corpus is the FIRST validation corpus for the wave-23 restored Pillar cost line.

**Record: 5/6** — beat 27, 131, 59, 102, 137; lost to 133. Context only; decomposed by decision.

| opp | result | end (my/opp, turn) | class |
|---|---|---|---|
| 27  | WIN  | 20 / 0, T11  | undead/merfolk grind; wide Goblin+Boros swarm, never took a point (20 life at end). |
| 131 | WIN  | 20 / -3, T12 | clean race; killed Young Pyromancer with Pillar on sight (exception fired). |
| 59  | WIN  | 14 / -1, T16 | GUIDELESS-Step-0 WB Persistence mirror-grind; out-raced persist blockers. |
| 133 | LOSS | 0 / 11, T13  | raced opp 20->4 by T13, then a one-turn devotion-drain/Gray-Merchant-class swing (+7 opp, -14 me) with no deck109 decision in the window. Lifegain/drain race-loss ceiling. |
| 102 | WIN  | 13 / -1, T11 | mono-B devotion; out-raced. |
| 137 | WIN  | 18 / -5, T8  | GW Adventures; fastest kill of the set (T8). |

The sole loss = the documented drain/lifegain matchup ceiling; no consequential decision error
(decomposed at bottom). **Fallbacks: 0 at this seat** (brief confirms true model-fallbacks =
3 corpus-wide, none deck109's). Answer-first held **111/111** ask/priority/block + **27/27**
attacker windows.

---

## (1) PILLAR RESTORATION ROUND 2 — {R}-vs-{1}{R} CONFLATION GONE. Restored line VALIDATED (corpus 1 of the 2-corpora clock).

The wave-23 regression (vs137 s12: declined an affordable Pillar believing it costs {1}{R}, the
Searing Spear cost) drove the restoration of a lean cost line (deployed guide lines 37-39). This
corpus is the acceptance test, and the **trigger shape was abundantly present**: **14 windows
where Pillar was a cast option at exactly {R} = 1 red mana available**, several with a Searing
Spear simultaneously in hand — including the SAME vs137 matchup. **In every one the model costed
Pillar correctly as {R} = 1 mana. Zero conflations. Zero affordable-Pillar declines attributable
to mis-costing.**

**The direct acceptance test — vs137 s9 (mana {r}{r}{r}, Searing Spear + Pillar both offered):**
the exact wave-23-regressing matchup and card pair. This time the reply states verbatim:
*"Searing Spear costs {1}{R} (2 mana) and Pillar of Flame costs {R} (1 mana)."* No conflation; it
correctly separated the two costs and cast Searing Spear at the face. **The precise wave-23
failure did not recur in the matchup that produced it.**

Two windows chose "Cast nothing" WITH an affordable Pillar — both are correctly-costed strategic
holds, **not** mis-costings:
- **vs133 s3 (T1, mana {r}):** reply reasons explicitly *"Pillar of Flame: {R}. Cost is 1 Red. I
  have 1 Red. I CAN cast this,"* then chose to hold the turn-1 Pillar rather than fire 2 at an
  empty-board opponent's face. Defensible RDW line (hold reach/removal T1); correctly costed.
- **vs131 s14 (mana {r}, hand Pillar + Boros Reckoner):** *"I could cast Pillar of Flame (needs 1
  mana)"* — correct cost. Held because the prompt's legal-target list showed only own creatures +
  "you (+1 more)" and the model was unsure the opponent was targetable. A target-visibility
  hesitation, not a cost error (routes nowhere — the opponent was likely the truncated "+1 more",
  a marginal missed 2 face damage in a game won 20/-3).

Also-correct positive casts under the trigger: vs27 s14 (cast Pillar), vs131 s3/s10/s16 (cast
Pillar), vs59 s12 (cast Pillar), vs133 s4 (cast Pillar) — all costed and fired correctly at 1 red.
Deploy-a-body-over-burn correctly preferred a 1-drop creature at 1 mana in vs27 s3/s9, vs59 s3,
vs102 s3/s7, vs137 s3 (all correctly costed Pillar as affordable but chose the creature per the
DEPLOY-AND-ATTACK rule — not a decline-from-mis-costing).

**Verdict: the restored cost line CLOSES the vs137-s12 shape. Corpus 1 of the 2-corpora
validation clock passes cleanly WITH the trigger present in the regressing matchup.** Per the
wave-23 skill refinement (one clean corpus is insufficient evidence of redundancy for a
cost/comprehension crutch), this is the first of two required confirmations — see ROTATION VERDICT.

Incidental: vs133 s6 emitted *"...both of which require {1}{r} (two mana...)"* about **Krenko's
Command AND Gore-House Chainwalker** — both genuinely {1}{R}. Correct costing, not a conflation
(the grep flagged it; verified benign).

---

## (2) ATTACKERS DECLARED-VS-RECORDED ROUND 3 — ATTACK-hijack class STAYS DEAD (27/27).

**27 attacker windows across 6 games. First-parseable coded `ATTACK:` line honored in ALL 27;
recorded set == the first line's in-eligible tokens in ALL 27. ZERO eligible attacker silently
dropped. Answer-first (ATTACK-first) 27/27. 0 fallbacks.** The wave-23 fix (d) — first-parseable
ATTACK line authoritative, salvage is fallback-only — holds.

**3 over-declaration windows** (first line named MORE tokens than eligible) — every extra token
was OUT of the eligible set and correctly dropped (the wave-22 A1 outcome-neutral pattern):

| seq | first coded line | eligible | recorded | dropped tokens eligible? |
|---|---|---|---|---|
| vs131 s15 | `A1, A2, A3` | opts=1 (Gore-House) | Gore-House | NO (A2/A3 don't exist) |
| vs133 s18 | `A1, A2, A3, Stromkirk Noble` | opts=3 (Goblin, Goblin, Legion) | Goblin, Goblin, Legion | NO (Stromkirk not an eligible attacker) |
| vs137 s7  | `A1 (Legion), Goblin, Goblin` | opts=1 (Legion) | Legion | NO (Goblins summoning-sick) |

**Spiral windows** (multiple ATTACK lines, the exact shape fix (d) targets): vs27 s19 (3 ATTACK
lines, lat 79s) and vs131 s22 (2 lines, lat 87s) — both took the FIRST line correctly, recorded
all eligible. **No prose "Attack: Deal N..." combat-math line appeared in any of the 27 windows**
(grep = 0), so the hijack shape had no opportunity to fire — and the deeper defect (a valid coded
line overridden by a differently-counted salvage) is NOT REPRODUCED. Aggression intact: every
window declared EVERY eligible attacker (all-in racing per the guide).

---

## (3) BURN DISCIPLINE ROUND 4 — CLEAN (0 self-face; face-by-default 14/15; exception fired 1x).

15 burn-TARGET selection windows. **0 self-face burn (no "Yourself" ever chosen).** Face 14/15.

- **Face by default (14):** vs27 s15 (opp 15), vs131 s4/s11/s17/s21/s24 (down to opp 4, the race
  kill), vs59 s11/s13/s27 (s27 = the lethal face at opp 2), vs133 s5/s21 (s21 = face at opp 7),
  vs137 s10/s12 — all at "The opponent (player, life N)".
- **Exception fired correctly (1): vs131 s9** — Pillar killed **Young Pyromancer** (2/1
  token-maker) over the face at opp life 19. The guide's exact named exception (a 2/1 dies to
  Pillar's 2). Correct.
- **Correct restraint (not misses):**
  - vs59 s11/s13/s27 and vs133 s21 offered **persist creatures** (Safehold Elite, Heartmender)
    and **Yawgmoth (2/4)**; all correctly faced. Persist creatures aren't the token/lifegain
    engine class and re-return if killed; Yawgmoth (2/4) can't die to a 3-damage Searing Spear
    anyway. Facing to advance the race is right.
  - vs137 s10/s12 faced over **Giant Killer (1/2)**, a small ground blocker — defensible
    aggressive race (guide burns a blocker only when it lets MORE damage through than face value;
    the face-race won at T8).

One BLOCK window declared a blocker — **vs27 s23, correct:** the harness's own lethal-line said
"NOT lethal (you would be at 18)... block only where the trade favors you." Model blocked a 2/2
Vodalian Zombie with **Boros Reckoner (3/3 first strike)** — a free kill (first strike removes the
2/2 before it deals damage; Boros takes 0), while at 20 life racing a foe at 8. Favorable trade,
guide-sanctioned, and Boros untaps to attack next turn. All other block windows declared
"BLOCKS: none" per the NOT-lethal default.

---

## (4) PRE-GAME MULLIGAN PHASE — rendered correctly; all keeps sound; one-land keep UNTESTED (no marginal hand dealt).

**Phase integrity: PASS.** Exactly **1 mulligan ask per game** (options `['Keep this hand',
'Mulligan']`), no double-asks, no wrong-N, no bottom-N events (nobody mulliganed, so no library
bottoming was required — the translog `bottom` kind never appeared). Game-start integrity holds:
games where deck109 was on the play open `=== Turn 1 - YOUR turn ===` with no turn-1 draw
(131/59/133/137); on-the-draw games open `=== Turn 1 - opponent's turn ===` (27/102). No
narration anomaly observed.

**Keep/mull quality: all 6 keeps correct, but the aggressive-one-land question is UNTESTED this
corpus.** All 6 opening hands were textbook 3-land RDW keeps — each with 3 Mountains, a 1-2 drop
and/or burn:

| opp | opening 7 | verdict |
|---|---|---|
| 27  | Krenko's Cmd, 3x Mtn, Pillar, Searing Spear, Ash Zealot | snap-keep (3 land, 2 burn, haste 2-drop) |
| 131 | Gore-House, 3x Mtn, 2x Pillar, Krenko's Cmd | snap-keep (1-drop, token-maker, 2 burn) |
| 59  | 3x Mtn, Stromkirk, Krenko's Cmd, Pillar, Searing Spear | snap-keep |
| 133 | Gore-House, 3x Mtn, Searing Spear, Krenko's Cmd, Pillar | snap-keep |
| 102 | Rakdos Cackler, 3x Mtn, Searing Spear, Krenko's Cmd, Pillar | snap-keep |
| 137 | Legion Loyalist, 3x Mtn, Searing Spear, Pillar, Krenko's Cmd | snap-keep |

Every keep is unambiguously correct — no hand was even close to a mulligan, so the model's
judgment on a MARGINAL aggressive hand (the brief's "did it keep aggressive one-land hands?")
**could not be exercised this corpus**: no 1-land, all-land, or spell-heavy hand was dealt across
6 games. The guide's mulligan line ("keep any hand with 2-4 lands and one/two 1-2 drops or burn;
mulligan only 0-1 land or all-land") was consistent with every decision but only tested on its
easy face. **Note for synthesis: the aggressive-mulligan judgment remains observationally
unconfirmed at this seat — not a failure, an untested surface (draw variance).**

---

## (5) 5/6 TOP SEAT — decisions-read: what's working.

deck109 was the top seat (5/6). The wins decompose to disciplined execution of the trimmed guide,
now with the restored Pillar line load-bearing:
- **Deploy-and-attack held every game:** a body cast nearly every main phase; the model correctly
  preferred a 1-drop creature over an affordable Pillar at 1 mana (vs27/vs59/vs102/vs137 openings)
  — the #1 rule executing exactly.
- **All-in attacking:** 27/27 windows declared every eligible attacker; no creature ever kept
  home. Fastest kill T8 (vs137).
- **Burn as reach + one correct engine-kill:** face-by-default 14/15 with the Young Pyromancer
  exception firing on sight — the guide's burn philosophy operating cleanly.
- **Pillar affordability now correct under pressure:** the restored cost line is doing real work —
  14 correct 1-mana Pillar cost reads including the regressing matchup, several under long spirals.
- **Latency/quality:** 138 latency-bearing decisions, median 15.8s / p90 78.6s / max 217s. Long
  EXPLORATORY spirals persist (longest reply 12.2k chars, vs131 s14 Pillar deliberation) but
  answer-first held 111/111 + 27/27, no decode-collapse, no garbage (retry correctly fired 0x —
  nothing to catch). REPPENALTY=1.05 second corpus: the wave-22 tight identical-line loop did NOT
  recur; spirals are divergent re-reasoning, not verbatim repetition. No degradation.

---

## Loss decomposition — vs133 (T13, 0/11): drain/lifegain race ceiling, no misplay.

Raced correctly (opp 20 -> 4 by the last deck109 decision at s21, Searing Spear at face; my life
14). The translog jumps s21 -> gameend with **no deck109 decision in between and no blockers ask
rendered** — the opponent (Yawgmoth BG persist/devotion) took one lethal turn: gained ~7 (opp
4 -> 11) and dealt 14 (me 14 -> 0), consistent with a Gray-Merchant-class devotion drain plus an
alpha strike I had no untapped creatures to block (I was correctly all-in racing). This is the
documented lifegain/drain matchup ceiling — same class as wave-23's deck133 loss — where RDW's
only plan is to race faster, which the model did (opp to 4). The drain simply had a higher ceiling.
No writable fix; the guide already documents this class. Not attributable to any decision error.
