# deck27 (Zombies UB tribal) — wave-25 findings

**Seat:** guided round 6. Binary /tmp/wagic-62d8783a5 (wave-25 step-1 batch on the rules-fixed engine
f0500ad66). Corpus matchups-20260725-035605 (PRIMARY, 21 games, -T 3000, REPPENALTY=1.05). 6 games
round-robin. **Record 2/6** (down from wave-24's 3/6; pool/variance moved — see below). **141 decisions
across 6 games, 0 fallbacks corpus-wide at this seat, retry fired 0x (correct).**

## Record — 2/6 (W vs133-adj, vs131; L vs109, vs137-adj, vs59, vs102)

| vs  | result | life (me/opp) | turn | shape |
|-----|--------|---------------|------|-------|
| 133 | **WIN (adj)** | 14 / 10 | 12 | Cap-adjudication edge (was a clean 20/0 in w24; closer this time). |
| 131 | **WIN**       | 14 / -1 | 14 | Textbook evasion clock — Inkfathom+Vodalian every even turn t4-14. |
| 137 | **LOSS (adj)**| 10 / 16 | 12 | Cap-adj; was a DRAW (12/12) in w24 — same GW Adventures latency sink, edged behind this time. |
| 109 | **LOSS**      | -7 / 14 | 9  | Mono-R Rakdos aggro BLOWOUT (Battalion first-strike+trample go-wide). Draw/matchup. |
| 59  | **LOSS**      | 0 / 4   | 23 | WB Persistence grind to the cap; close (opp to 4) — matchup. |
| 102 | **LOSS**      | -1 / 5  | 13 | Tourach un-interactable grind — close race, matchup-flagged since wave-21. |

**Read (decisions-based):** the 2 wins are earned on the win-path (evasion + lords). 3/6 -> 2/6 is
POOL/VARIANCE, not a guide regression (guide byte-frozen this corpus): vs137 flipped w24-DRAW ->
w25-adj-LOSS on the same latency-sink matchup (behind 10-16 at the cap vs even 12-12 last wave), and
vs133 stayed a WIN but tightened (20/0 -> 14/10 adj). The three losses (109/59/102) are all matchup/
draw-attributable, not misplay-driven. Attribute per game; do NOT read the record drop as anything the
guide did. **No harmful misplay decided any game this corpus** — the opposite of wave-24, whose two
harmful classes (bottom commit-lock, first-strike lord-chump) both structurally DISAPPEARED here.

---

## FOCUS 1 (b2) — NATURAL-STOP at bottom asks: the colorstuck commit-lock class is STRUCTURALLY GONE

Four bottom records this corpus (vs133 bottom-2, vs137/vs59/vs102 bottom-1). **Every one is
single-direction: the coded first line == the reply's conclusion, NO internal reversal.** There was no
wrong-first-line-then-self-correction to rescue — which is the DESIRED state (the wave-24 bottom-phase
guide rule "decide the list ONCE, put it on your first line" is being followed, so natural-stop has
nothing to override at the bottom seam).

| vs  | mull | hand | PUT (line 1 == conclusion) | colorstuck? | verdict |
|-----|------|------|----------------------------|-------------|---------|
| 133 | 2 | 2x LotU, 2x GEL, Island, Metathran, Swamp | `1,2` (one LotU + one GEL) | NO (keeps I+S) | clean — ships redundant expensive dupes, keeps both colors + a 2-drop |
| 137 | 1 | 7 creatures, ZERO lands | `7` (Lord of Atlantis, {U}{U}) | n/a (mull-keep issue) | bottom-pick defensible (ships the double-blue card from an all-creature hand); the 0-land KEEP is the real question, not the bottom |
| 59  | 1 | 2x Island, 2x Swamp, Inkfathom, Walking Dead, LotU | `7` (Lord of the Undead) | NO (2I+2S) | mild flood-lean (kept 4 lands + 2 creatures over 3 lands + LotU); defensible |
| 102 | 1 | Wasp Lancer, U.Sea, LoA, Zombie Master, GEL, Island, LotU | `1` (Wasp Lancer) | NO (keeps U.Sea+Island) | **NEW MISPLAY — see below** |

**The wave-24 colorstuck class (vs137 s5: bottomed the only blue source, kept 3 Swamps, commit-locked)
did NOT recur.** No bottom created a dead-color hand; no bottom shipped a sole color source. **b2
acceptance at the bottom seam: PASS structurally** — but note it was NOT stress-tested by a reversal
here (no bottom reply had a wrong first line to override), because the guide rule prevented the reversal
upstream. The b2 mechanism DID get a live stress-test — at the BLOCKERS seam (FOCUS 2), where it FIRED.

### NEW bottom-phase misplay (vs102 seq3): rule #1 does NOT reach the pregame phase

vs102 bottomed **Wasp Lancer** ({U/B}{U/B}{U/B} 3/2 flyer — its flagship evasion) reasoning:
*"Wasp Lancer costs {U/B}{U/B}{U/B}, which is difficult to cast on turn 2 or 3 without significant mana
fixing."* This is **rule #1 exactly backwards**: a triple-{U/B} hybrid is the EASIEST 3-drop in the
deck to cast (pays with any mix of U and B), far easier than the {U}{U} Lord of Atlantis or {1}{B}{B}
Lord of the Undead it KEPT. The model treated the flexible hybrid as color-demanding because rule #1
is framed around CAST OPTIONS ("if a Cast option is listed...") and that framing does not reach the
BOTTOM phase, which reasons over raw costs with no cast list. Harm was low (kept a functional 2-land
hand, lost to the Tourach matchup anyway) but it is a clean, guide-addressable instance of the mana
fact failing to transfer to a new phase. **Routed to GUIDE:** added a bottom-phase clause restating
rule #1 (hybrids are easiest, ship a double-pip card before a flexible hybrid). Same shape as wave-24
headline-4 (deck59's green-veto costing mulligan quality) — a mana-simplification fact must be carried
into EVERY cost-reasoning phase, not just the cast seam. Skill.md #3.

---

## FOCUS 2 (b2 live-positive) — NATURAL-STOP RESCUED A LETHAL MISREAD at the BLOCKERS seam (vs109 seq11)

**This is the corpus's headline positive.** vs109 seq11 (t7, at 6 life, facing A1 2/2 + A2 3/2 + A3 1/1,
ALL first-strike + trample = 6 damage = LETHAL; only blocker Oona 2/1):

- **Coded first line: `BLOCKS: none`** — with prose initially concluding "I accept the lethal damage."
  At 6 life vs 6 first-strike/trample damage this is DEATH.
- **The reply then reasons to completion and REVERSES:** it recomputes that blocking the biggest
  attacker (A2, the 3/2) leaves it alive ("I must block A2 to survive"), ending on **`BLOCKS: B1:A2`**
  and a matching PLAN. Natural termination.
- **The engine applied the FINAL line: choice=1 (B1:A2).** deck27 SURVIVED turn 7 (it died turn 9, two
  turns later, in the blowout) — proof the block was applied; had `BLOCKS: none` been frozen answer-first
  (the wave-24 ATTACK/bottom failure shape), deck27 dies at turn 7.

This is the **first live-observed positive acceptance of b2 natural-stop precedence** — the exact
structural fix for wave-24's commit-lock / first-line-vs-conclusion family, now caught WORKING. (One
caveat: the blocker parser's own "none is not an assignment, last real BLOCKS line wins" behavior could
co-produce this; I cannot fully isolate b2 from the parser here. Either way the OUTCOME is a correct
rescue, and it is the behavior b2 mandates.) Skill.md #1. The guide now ALSO teaches the lethal-block
line directly (block the biggest to survive; end on your survival-correct line) so the FIRST line is
more likely right without relying on the backstop.

Minor arithmetic slip in the same reply: it computed "block A2 -> take 3, life 3," forgetting A2's
2 trample-over (the annotation correctly printed "(2 tramples to your face)"); true result ~1 life.
Conclusion (block to survive) and outcome (survives) both correct; engine handled trample. No action.

---

## FOCUS 3 — deployed wave-24 rules VALIDATED: first-strike guide + bottom-phase guide both held

**(a) First-strike block rule — VALIDATED, wave-24 harmful chump did NOT recur.** The wave-24 misfire
(vs109 seq11 w24: chumped Lord of Atlantis into an Ash Zealot first striker calling it "a trade") has
no analog this corpus. At vs109 seq9 (t5, three FS+trample attackers, only Oona) the model correctly
read every block as a chump ("blocker dies immediately due to First Strike without dealing damage
back") and took no block — trusting the "(your blocker dies, attacker lives)" annotations rather than
re-deriving "both die." The deployed rule ("first strike changes the math — trust the annotation")
held. No lord was chumped into a first striker anywhere this corpus.

**(b) Bottom-phase guide rules — VALIDATED.** "Decide the list ONCE on line 1" held at all 4 bottoms
(zero reversals). "Never bottom your only source of a color" held (no colorstuck keep). The residual is
the hybrid-flexibility gap (FOCUS 1), which the existing bottom section did not yet cover.

## FOCUS 2b (b5) — WITHER-vs-FIRST-STRIKE annotation: CORRECT in BOTH directions (round 4)

The wave-24 defect was a false "(wither shrinks it to 0/0)" clause rendered even when a first striker
kills Oona before it deals its wither. **b5 fix is LIVE and correct at both poles:**

- **First-strike case (vs109 seq9, seq11):** Oona blocking FS attackers renders
  `(your blocker dies, attacker lives, N tramples to your face)` with **NO shrink clause** — correct
  (Oona dies before biting, so no shrink). The wave-24 false clause is GONE.
- **Non-first-strike case (vs137 seq12):** Oona (2/1 wither) may block a 5/5 Lovestruck Beast (no FS):
  renders `(your blocker dies, attacker lives (wither shrinks it to 3/3))` — **correct** (Oona survives
  to deal 2 wither = -2/-2, 5/5 -> 3/3, then dies to the 5). The shrink clause appears exactly when the
  wall lives long enough to bite.

**b5 acceptance: PASS.** The annotation is now first-strike-aware in both directions. This is a
crutch-lifecycle CLOSED-BY-FACT terminus (the representation fix eliminated the misrender; no durable
model belief to police). The guide's Oona clause was refined to describe the actual annotation strings
now shipping and to note that chump-withering a big non-first-striker (permanent 5/5->3/3) can beat
trading Oona for a small flyer (the vs137 seq12 judgment call — model took the clean "both die" on the
1/1; defensible, not scored a miss). Notes.md #2 (informational — the fix works).

---

## FOCUS 4 — rules-shift watch (batch A): no behavior shift at this seat

- **(a1) turn-1 upkeep:** deck27 has NO upkeep-trigger cards (no echo, cumulative upkeep, or "at the
  beginning of your upkeep" text across its list). a1 is **not exercised** at this seat — nothing to
  fire a turn earlier. Turn-1 narration renders cleanly (mulligan phase then "Main phase 1"); no
  narration anomaly observed.
- **(a3) combat roles clear at end of combat:** no stale-attacker confusion this corpus. One near-miss
  (vs109 seq6) where the model wondered whether a present-but-NOT-attacking Gore-House Chainwalker was
  an attacker — but that is board-reading of a non-attacking creature, not a stale-combat-role artifact.
  The guide's "only A1, A2... exist right now" line covered it (chose the correct single-attacker block
  outcome). Added a small clause not to distrust a correct "up to N" damage hint (the seq6 verbosity was
  driven by disbelieving the accurate "deal up to 2" hint).
- **(a4) planeswalker damage-redirect menu gone:** deck27 has no burn/damage spells — **N/A**, no
  redirect-menu record exists at this seat (confirmed).
- **(a2/a5/a6):** no priority-at-untap/cleanup, no creature-Battles, no day/night at this seat.

## FOCUS 5 — durdle late-stall off-case: STILL UNEXERCISED (6th corpus)

vs59 (t23 grind LOSS) and vs131 (t14 WIN) were the long windows. deck27 attacked whenever it had an
attacker: vs131 swung Inkfathom+Vodalian EVERY even turn t4-14 (never held back — WIN); vs59 swung
t4/6/8/12/14/22 (a t16-20 gap in a stalled grind, but consistent with limited untapped attackers, not
deliberate holding — it swung its evasive Inkfathom every early turn and its lords when it had them).
The off-case TRIGGER (walled ground stall + HOLDING eligible attackers) again did not occur — deck27
follows the swing-every-turn prescription. **Off-case now 6 corpora unexercised; hold and re-flag per
the skill's low-frequency-off-case note, do NOT cut.** No guide edit on this axis.

## FOCUS 6 (2/6 decisions-read) — verbosity persists, harmless

Reply lengths: p50 ~950-1160, max 13672 (vs131 seq20). The longest reply is COHERENT verbose reasoning
(174 lines, 164 unique = 94%, max single-line repeat = 3) — NOT the wave-23 decode-collapse (which was
1354x one line). Retry firing 0x is CORRECT — the collapse class did not recur. Verbosity costs latency
(and contributed to the vs137/vs59 cap games) but no correctness. The vs109 seq6/seq11 replies are the
verbose extremes (60+ lines re-deriving damage math); seq11's over-reasoning actually PRODUCED the
correct reversal that natural-stop then captured. No harm from verbosity this corpus.
