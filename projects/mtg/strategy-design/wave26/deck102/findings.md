# deck102 (Tergrid, discard/sacrifice) -- findings (wave 26, guided round 6, ROTATION SEAT)

Corpus: matchups-20260725-115216 (PRIMARY), binary 77e91ef4e (commits 79e39511f + 77e91ef4e),
-T 3000, REPPENALTY=1.05. 6 games round-robin (vs 22/131/133/137/18/27). Record **0/6** (down from
wave-25's 4/6). 0 fallbacks at seat, 0 timeouts/draws. Latency medians 16.7-22.9s; three big
singletons (135-224s) = the reasoning-reversal spirals in Section 5.

Headline: **the 0/6 is a POOL-RISE + DRAW-VARIANCE collapse, NOT a decision regression and NOT a
guide gap.** All four returning opponents 102 beat last wave (131/133/137/27) improved -- deck133
went 1/6 -> 6/6 (biggest riser; last wave 102 beat it 23/-7, this wave lost -5/14) -- and two fresh
aggro decks (18 Kithkin, 22 Giants) entered. The model's DECISIONS were largely sound: 6/6 correct
mulligan keeps (the wave-25 over-mulligan class did NOT recur), a correct board-wipe (Damnation on a
lethal Inferno Titan), a correct reanimation-steal (Animate Dead on the opponent's Giant Killer),
attacks-with-everything-available in every combat, and CORRECT final answers on three reasoning-
reversal spirals that the c4 parser hardening resolved perfectly. The recurring soft factor is
durdle/slow-clock (the guide's already-addressed #1 leak), but this wave it is mostly downstream of
threat-light, rock-heavy openers -- a 100-card singleton with ONE Tergrid is inherently high-variance.
Tergrid appeared in hand ONCE (vs22, never castable before game end); ZERO steal windows, ZERO
may-ask renders live -- consistent with the probe having already discharged the steal chain.

| vs  | result   | end  | my/opp | deciding factor |
|-----|----------|------|--------|-----------------|
| 22  | LOSS adj | t16  | 8/15   | **CAN'T-CLOSE grind** vs a fresh 20-life ramp deck; stabilized correctly, couldn't push through blockers before the cap. Decisions sound. |
| 131 | LOSS     | t15  | 0/19   | **THREAT-LIGHT DRAW** (rocks+1/1+equipment) raced by Guttersnipe burn 20->0; opp improved 1->2/6. |
| 133 | LOSS     | t13  | -5/14  | **POOL-RISE**: 133 went 1/6 -> 6/6; lost the devotion mirror to a hugely-improved opponent. |
| 137 | LOSS     | t15  | -6/25  | **OUT-VALUED**: GW gained to 25 (1->3/6); marginal t2 Smallpox + slow clock. |
| 18  | LOSS     | t10  | 0/14   | **RACED** by fresh mono-W Kithkin aggro; a mild t8 double-rock durdle at 13 life. |
| 27  | LOSS     | t13  | 0/9    | **RACE loss**: deployed Witch of the Moors + attacked (opp to 9), lost the race off a rock-heavy t5. |

Net: 4 losses dominantly pool-rise / fresh-deck (22, 133, 137, 18); 2 draw-variance-heavy (131, 27).

---

## Section 1 -- THE 0/6 DECOMPOSITION (deciding factors vs noise, game by game)

**vs133 (-5/14 t13) -- POOL-RISE, the cleanest signal.** Last wave 102 beat this exact deck 23/-7
dominant; this wave 133 went 6/6 (the pool's biggest riser, per its own Arena/threat-drought edit).
Not a 102 decision failure -- a mirror-ish matchup where the opponent got much better. 102's line was
reasonable (Tinybones t0, Liliana's Triumph edict t4, Dreadhorde Invasion, Liliana Dreadhorde General
t10, Megrim). The Liliana -4 (each player sacrifices two creatures) at t10 (L9/15) is the one arguable
spot -- a symmetric sweep while behind -- but with a token engine on both sides it is defensible board
management; it did not decide the game. DECIDING FACTOR: opponent strength.

**vs22 (adj 8/15 t16) -- CAN'T-CLOSE grind vs a fresh Giants ramp deck.** 102 controlled: Thoughtseize
t0, Liliana Dreadhorde General t10 (token engine), **Damnation t12 -- CORRECT** (removed a lethal
Inferno Titan 6/6 at L11; losing a 2/2 token to a 6/6 wrath is right), Syr Konrad, Tinybones,
equipment. It attacked with EVERYTHING available (s46 both bodies, s55 all three) -- no withheld-
attacker leak. It could not push a 20-life ramp deck through blockers fast enough, and adjudication
went to the higher-life player. DECIDING FACTOR: closing speed vs a fat life total; decisions sound.
This is the deck's structural weakness (slow token clock vs a life-buffered ramp deck), not a fixable
misplay.

**vs131 (0/19 t15) -- THREAT-LIGHT DRAW raced by burn.** Opener rock-heavy (3 Swamp, Arcane Signet,
Elvish Doomsayer 1/1, Blackblade Reforged, Lightning Greaves); drew into more rocks (Mox Jet, Sol Ring)
+ Animate Dead + Plaguecrafter. Only bodies were a 1/1 Doomsayer and a t12 Plaguecrafter; Blackblade
(equipment needing a creature) sat near-useless. Opp (Guttersnipe izzet) stayed 19-20 while 102 was
burned 20->0. Plaguecrafter t12 (L8) was a correct stabilize-attempt but too late. DECIDING FACTOR:
draw variance (no real threats drawn) + opponent burn.

**vs137 (-6/25 t15) -- OUT-VALUED by GW lifegain + slow clock.** Opp reached 25. 102's line: Smallpox
t2 (MARGINAL, Section 3), Animate Dead reanimating the opp's Giant Killer t4 (poor-man's-Tergrid line
WORKING), Oppression, Witch of the Moors t12, Thoughtseize t14. Deployed threats but the clock was too
slow vs a lifegain deck (1->3/6 riser). DECIDING FACTOR: pool-rise + slow clock.

**vs18 (0/14 t10, fastest) -- RACED by fresh mono-W aggro.** 102 raced (Tourach t2, Sword of Feast and
Famine equipped, attacking) but at t8 (L13) cast TWO mana rocks (Arcane Signet + Mind Stone) vs a
Kithkin anthem-aggro deck instead of pure defense -- a mild durdle in a race it needed to respect.
Out-raced by t10. DECIDING FACTOR: fresh fast aggro; the double-rock at 13 life is the one soft
misstep but the race was likely already lost.

**vs27 (0/9 t13) -- RACE loss off a slow start.** Rock-heavy t5 (Sol Ring + Mind Stone + Leaden Myr in
one turn) delayed the clock; 102 then deployed Witch of the Moors (4/4) t7 and attacked, getting opp
to 9, but lost the race. Reasonable threat-deploy, too slow out of the gate. DECIDING FACTOR: race +
slow start.

**Synthesis:** the collapse is REAL but its cause is the environment (four improved returning decks +
two fresh aggro decks) and the deck's own high draw variance (threat-light rock-heavy openers), NOT a
decision regression. The model played the standing guide lines correctly where the game let it.

---

## Section 2 -- MULLIGAN EDIT (wave-25 "early = by turn 2") FIRST CORPUS: FIRED as prevention, validated CLEAN.

All six games opened **"Keep this hand"** -- ZERO mulligans. Every keep correct by the guide's own
criteria (3-4 lands/rocks + an early play):
- vs22: 4 lands + Thoughtseize + Leaden Myr. KEEP correct.
- vs131: 3 Swamp + Arcane Signet + Blackblade + Elvish Doomsayer + Lightning Greaves. KEEP correct.
- vs133: 4 Swamp-equivalents + Tinybones + Lotus Petal. KEEP correct.
- vs137: Urborg + 2 Swamp + Charcoal Diamond + Smallpox + Animate Dead + Leaden Myr. KEEP correct.
- vs18: 3 Swamp + 2 rocks + Tourach + Dreadhorde Invasion. KEEP correct.
- vs27: 2 Swamp + Barren Moor + Mox Jet + Mind Stone + Leaden Myr + Night's Whisper. KEEP correct
  (a no-land-creature ramp-into-disruption hand -- exactly the "keep a functional disruptive hand,
  don't chase turn-1" spirit the wave-25 edit encodes).

**The wave-25 over-mulligan failure class (vs109 turn-1-chase) did NOT recur.** The edit is validated
non-harmful: no incorrect keep is attributable to it, no NEW mulligan ambiguity surfaced. This is the
single confirmation the wave-25 notes said the edit needed. Mulligan surface: no new work signal.

---

## Section 3 -- STEAL / TERGRID surfaces: ZERO live windows; Tergrid in hand once, never cast; flip-thrash 0/6 (round 4 HELD).

- **Steal windows: 0.** Scanned every deck102 prompt/options/reply for "put onto/on battlefield",
  "you MAY act", may-ask render strings -- none. No @sacrificed/@discarded steal ask fired live.
  Consistent with the probe (chain discharged there) and the wave-25 mechanistic finding (token-
  feeding opponents, no forced nontoken discard with Tergrid live).
- **Tergrid on board / cast: 0.** Tergrid appeared in HAND only in vs22 (a "Flip Side with Tergrid"
  pseudo-action at s57/s58, t16) but the game ended before it was castable. R-DFC-BACKFACE remains
  UN-RE-EXERCISED (4th consecutive wave) -- the Lantern back-face was never cast.
- **Flip-thrash: 0/6 consumed.** At vs22 s57 (Flip Side present) the model chose pass; at s58 it chose
  Equip Shadowspear. DFC display-toggle demotion HELD round 4 (w22:11, w23:1, w24:0, w25:0, w26:0).
- **c5 may-ask render (fixed): UNEXERCISED at this seat** -- no may-trigger surfaced live. Validated in
  the probe report, not here.

---

## Section 4 -- c4 PARSER-HARDENING CORPUS GATE: PASS, with THREE live positive validations.

A crude "reply CHOICE index vs recorded choice" scan flagged three seeming mismatches. Decomposed
verbatim against the precedence spec (natural-stop last-coded-line), ALL THREE are the hardening
WORKING CORRECTLY -- the model opened with one answer, REVERSED mid-reply, and the parser correctly
took the model's FINAL/last-coded answer, not the reversed-away opening line. NONE is a misfire.

1. **deck137 s8 (t4):** reply opens CHOICE: 4 (Cast Sol Ring), spirals, then reverses and ENDS with
   CHOICE: 1 (Cast Animate Dead {1}{b}) + PLAN. Recorded choice=1 (Animate Dead). CORRECT -- parser
   honored the reversal to the true final answer (and Animate Dead reanimating the opponent's Giant
   Killer is the guide-correct threat-deploy). Not truncated-first, not opening-line.

2. **deck18 s5 (blockers, t3):** the reply contains a literal, self-negated
   "CHOICE: 1 (Block with Tourach) -> NO, I decided not to block", and its final answer is
   "BLOCKS: none". Recorded choice=0 ("no blockers"). CORRECT -- the parser did NOT get fooled by the
   crossed-out CHOICE line and took the final BLOCKS: none (keep Tourach, take 1 at L20 -- correct).
   Strongest positive: a reply literally containing a negated CHOICE: N that the hardening ignored.

3. **deck27 s9 (t5):** reply opens CHOICE: 3 (Play Witch's Cottage), reasons through tapped-land math,
   reverses, ENDS with CHOICE: 1 (Play Barren Moor) + PLAN. Recorded choice=1 (Barren Moor). CORRECT --
   final answer honored (Barren Moor's Cycling is right; <3 Swamps means Cottage enters tapped anyway).

**No legitimate reply had its answer dropped or mis-picked. Zero fallbacks at this seat. c4 gate:
clean, three-times positively validated.** The standing N9 caution (only a corpus proves not-firing-
where-it-shouldn't) is satisfied at this seat -- the hardening survived three live multi-reversal
spirals.

---

## Section 5 -- c1-c3 behavior + the reversal-spiral observation.

- **c1 (target preview, opponent-first, no cap):** CONFIRMED. Thoughtseize previews render
  "legal targets right now: the opponent, you" (opponent FIRST) at vs22 s3, vs18 s26, vs137 s24 (the
  latter with the "- the opponent holds on..." extension). No truncation, opponent-first, no prompt-
  length side effect observed.
- **c2 (ETB tapped/untapped):** N/A at this mono-B seat -- 0 shockland/pathway options, 0 annotation
  surfaces, no double-annotation confusion.
- **c3 (PT-pump delta render):** N/A -- no activated-pump seat. Equipment equips (Shadowspear, Sword,
  Blackblade) rendered plainly; no stat-conflation spiral.
- **REVERSAL-SPIRAL (new observation, benign):** deck18 s5, deck27 s9, deck137 s8 are multi-page
  replies (135-224s latency singletons) that reverse the opening CHOICE two-to-three times before
  landing. The reversals reached the BETTER answer each time (Sol Ring -> Animate Dead threat;
  Witch's Cottage -> Barren Moor Cycling; a real block-evaluation ending correctly). This is the model
  reconsidering and IMPROVING, and c4 exists precisely to let it land the final line safely. A
  latency/legibility cost, NOT a decision leak (all correct, 0 fallbacks). Do NOT suppress it; see
  general-suggestions.md.
