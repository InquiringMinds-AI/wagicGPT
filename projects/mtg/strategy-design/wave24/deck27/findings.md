# deck27 (Zombies UB tribal) — wave-24 findings

**Seat:** guided round 5. Binary /tmp/wagic-d7ff8b6c4 (step-1 batch + PREGAME MULLIGAN PHASE + N9
hotfix). Corpus matchups-20260724-125739 (the PRIMARY run; the N9-overfire run is quarantined —
not read). 6 games round-robin. **Record 3/6** (down from wave-23's 4/6; same-ish pool). **160
decisions across 6 games, 0 fallbacks corpus-wide at this seat.**

## Record — 3/6 (W vs133/59-adj/131; L vs102/109; DRAW vs137-timeout)

| vs  | result | life (me/opp) | turn | shape |
|-----|--------|---------------|------|-------|
| 133 | **WIN**       | 20 / 0  | 12 | Clean life-untouched aggro win (20 life). Yawgmoth deck; flew/evaded over it |
| 59  | **WIN (adj)** | 12 / 11 | 18 | Grind to the 2400s cap; swung every turn, edged the adjudication by ONE life |
| 131 | **WIN**       | 12 / -1 | 18 | Ground the Pyromancer/Guttersnipe burn deck out with tribal beatdown |
| 137 | **DRAW (timeout)** | 12 / 12 | 10 | Even-life 2400s TIMEOUT. Pure inference-latency starvation (FOCUS 3); crippled by a bottom-phase misplay (FOCUS 1) |
| 102 | **LOSS**      | 0 / 4   | 13 | Tourach un-interactable grind — CLOSE race (both 8/8 at t10; got opp to 4). Matchup, flagged since wave-21 |
| 109 | **LOSS**      | 0 / 20  | 11 | BLOWOUT vs mono-R aggro (opp UNTOUCHED at 20). Draw/matchup + a harmful lord-chump misplay (FOCUS 2) |

**Read (decisions-based):** the three wins are earned on the win-path (evasion + lords; two are grinds
that swung every turn). Record fell 4/6 -> 3/6 but the pool/variance moved: vs109 flipped from a wave-23
dominant WIN (18/-1) to a wave-24 BLOWOUT LOSS (0/20) on a bad mono-red draw plus one harmful block.
vs102 stayed a loss but is now a near-miss race (0/4, opp to 4) not a grind-out (wave-23 0/11). Do NOT
read 4/6 -> 3/6 as any revision regressing (guide byte-frozen this corpus). Attribute per game.

---

## FOCUS 1 — the DECODE-COLLAPSE follow-up + the NEW bottom-phase COMMIT-LOCK

### (a) The wave-23 decode-collapse class did NOT recur. Answer-locked retry 0 fires = CORRECT.

Wave-23 owned the collapse cluster (vs137 s12-14: 1354x verbatim-line repetition, U+FFFD bytes, ZERO
coded lines -> unparsed_reply). **This corpus: 0 fallbacks at this seat across all 6 games, and NO
reply carried the collapse signature.** Reply-length + repetition evidence:

| vs  | replies | len p50 | len max | biggest-reply unique-line ratio |
|-----|---------|---------|---------|---------------------------------|
| 102 | 20 | 978  | 3991  | — |
| 109 | 19 | 1330 | 10264 | 171/194 (88%) |
| 133 | 24 | 1106 | 3532  | — |
| 59  | 34 | 984  | 10581 | 185/196 (94%) |
| 137 | 22 | 976  | 5132  | 71/83 (86%) |
| 131 | 31 | 1112 | 8224  | 95/100 (95%) |

The big replies (up to 10.5k chars) are **verbose-but-COHERENT reasoning** — 86-95% unique lines, max
repeat-count of any single line = 3 (the collapse was 1354). Every one emits a valid coded line and
PARSES. Nothing for a collapse detector to catch. **The answer-locked retry firing 0x is correct at
this seat; the collapse class simply did not recur.** What DID appear is verbosity (reasoning to 5-10k
chars) — a distinct phenomenon, costing LATENCY not correctness (FOCUS 3).

### (b) NEW harmful failure: a COMMIT-LOCK in the pregame BOTTOM phase (vs137 seq5).

The verbosity produced a harmful **commit-lock** (WRONG coded line 1, model reasons to the RIGHT answer
in following prose, answer-first freezes the wrong snap). FIRST commit-lock since wave-22, in the new
bottom phase:

- vs137 seq5, kind=bottom, 89s / 5132 chars. Hand after 3 mulligans (bottom N=3): [1 Swamp, 2 Metathran
  Zombie, 3 Underground Sea, 4 Swamp, 5 Sanguine Guard, 6 Wasp Lancer, 7 Swamp].
- **Coded line 1: `PUT: 3, 5, 6`** -> bottoms Underground Sea (3, the ONLY blue source), Sanguine Guard,
  Wasp Lancer. Keeps 3 Swamps + Metathran Zombie.
- **The reply then reasons CORRECTLY and reverses itself**, ending verbatim: "So I bottom 5, 6, and 7.
  PLAN: Put cards 5, 6, and 7 on the bottom..." — the model's FINAL answer was 5,6,7 (keep Underground
  Sea + 2 Swamps + Metathran). Answer-first took line 1.
- **Harm: real.** Metathran Zombie costs {1}{U}; kept hand (3 Swamps + Metathran) is COLORSTUCK — cannot
  cast its only creature (no blue source). deck27 then played Swamp t0/t2/t4 with NO play until t4
  (finally Inkfathom off black hybrids). Slow colorstuck opening that fed the vs137 timeout draw.

**Routing:** two owners, both actionable.
- **GUIDE (mine, shipped in strategy.txt):** guide had NO bottom-phase guidance (phase is new). Added:
  keep color-fixing first, NEVER bottom your only source of a color, three-Swamps-is-colorstuck, and
  "decide the bottom list ONCE, put it on your first line." Makes the snap right.
- **PARSER (notes.md #1):** live harmful instance validating the wave-23 skill.md parser proposal — on a
  NON-truncated reply, prefer the LAST coded line (the model's final answer) over the first. Here the
  last line was correct (5,6,7); answer-first threw it away. Bottom decisions are arithmetic/color heavy
  and the fast snap is unreliable for them.

---

## FOCUS 2 — free-kills-first round 3: ONE clean multi-block, ONE HARMFUL lord-chump misfire

Two true multi-attacker windows arose, both in the vs109 blowout loss:

- **vs109 seq15 (t9, 13/20) — CLEAN.** 5 attackers, 2 blockers. Chose `B1:A2, B2:A4`: Vodalian Zombie
  free-kills a Goblin (lives), Metathran trades a Goblin; took 5, 13->8; no lord chumped. Textbook.

- **vs109 seq11 (t7, 17/20) — HARMFUL MISFIRE.** 4 attackers (A1 Ash Zealot 2/2 FIRST STRIKE, A2/A3
  Goblin 1/1, A4 Stromkirk Noble 1/1), 2 blockers (B1 Lord of Atlantis 2/2 LORD; B2 Oona's Gatewarden
  2/1 wither). Engine annotation was CORRECT and explicit: LoA blocking A1 = **"(your blocker dies,
  attacker lives)"** (Ash's first strike kills the 2/2 lord before it deals damage), and LoA had THREE
  FREE KILLS (A2/A3/A4 all "you kill it, your blocker lives"). **The model chose `B1:A1, B2:A2`** —
  chumped Lord of Atlantis into the first striker — with prose claiming "both die, this is a trade." It
  is NOT a trade: Ash SURVIVES, LoA dies for nothing, three free kills unused. deck27 lost its evasion
  lord (islandwalk enabler) to kill zero extra vs the free-kills line, in a game it lost 0/20.

**Root cause:** model RE-DERIVED first-strike math (wrongly, "both die") and OVERRODE the correct
rendered annotation. Guide already says "TRUST the annotation"; failure is not trusting it on first
strike. **Routed to GUIDE** (mine): added a first-strike caution — a same-power first striker kills your
blocker before it deals damage, so the block is a CHUMP not a trade; trust the "(your blocker dies,
attacker lives)" tag; never chump a lord into a first striker. Free-kills-first's first harmful instance,
guide-fixable at the deck layer (single-seat real-blocker deck).

**Free-kills-first status:** 2 clean multi-block executions across w23-24 (vs109 s13 w23, vs109 seq15
w24) + 1 harmful first-strike misfire (vs109 seq11 w24). Allocation rule fires correctly; residual is
specifically FIRST STRIKE, now addressed.

---

## FOCUS 3 — the vs137 TIMEOUT-DRAW: pure inference-latency starvation, not a stall

vs137 timed out at turn 10, even life 12/12. Decomposition (both seats' per-decision latency):

- **Combined decision latency = 2352s of the 2400s cap (98%).** No engine stall; starved by slow
  inference. Reached only turn 10 because ~all wall time was model round trips.
- **deck27 seat: 517s over 23 decisions.** Pregame (turn 0) = **158s over 6 decisions** (3 mull asks +
  keep + bottom + land), of which the bottom alone was **89s / 5132 chars** — the verbosity tax (and the
  commit-lock, FOCUS 1). Non-trivial but NOT dominant.
- **deck137 seat: 1835s over 55 decisions** — the DOMINANT sink. deck137 (Selesnya Adventures/convoke)
  made 2.4x the decisions AND had extreme latencies (t9=627s, t7=452s [12457-char reply], t3=338s). The
  opponent's convoke-heavy volume + verbosity ate the clock.

**Verdict: slow inference, opponent-dominated, not an engine/phase stall.** This corpus ran at much
higher latency than wave-23 (deck27 per-game latency sums 370s-1002s here vs wave-23 p50 ~12-16s) —
consistent with -j concurrency saturation (brief: median 17.3s, convoke seats spiked 197-627s). The
pregame phase cost deck27 158s of the 517s; a max-tokens fast-cap on verbose decisions would reclaim
the timeout risk AND the commit-lock's over-reasoning (notes.md #3/#4). The vs59 game (adj win 12/11,
t18) also ran to the cap in the same latency regime — but deck27 was ahead so it adjudicated a win.

---

## FOCUS 4 — pregame mulligan phase validation

Verified across all 6 games. The phase renders correctly; the London flow is CR-conformant.

1. **Mulligan asks rendered, keep/mull mostly sensible.** vs102/109/133/131 kept hand 1 (reasonable
   snap keeps). vs59 mulled once then kept + bottom-1. vs137 mulled THREE times (to 4) then kept +
   bottom-3 — aggressive mull-to-4; the keep/mull asks do NOT log hand contents (only Keep/Mulligan), so
   I cannot verify the mulled hands were unkeepable, but the loop structure (one ask per iteration, no
   double-asks) is correct.
2. **Bottom decision — one FAILURE (vs137 seq5).** vs59 seq3 bottomed 1 of a DUPLICATE Lord of the
   Undead from a 2-land hand — defensible (most redundant). vs137 seq5 bottomed its ONLY blue source
   while keeping 3 basic Swamps — a MISPLAY (commit-lock, FOCUS 1). Did it bottom its N worst? vs59 yes;
   vs137 NO.
3. **Game-start integrity: OK.** No double-ask, correct bottom-N per mulligan count (1 mull->bottom 1 at
   vs59; 3 mulls->bottom 3 at vs137). Turn-0 renders "Main phase 1"; turn sequence/alternation looks
   correct. No leyline cards in deck27 -> 103.6 window N/A at this seat (not exercised).
4. **Narration anomaly: NONE** of the "opening hand line reflects pre-mulligan deal" cosmetic observed
   at deck27's seat this corpus.

**Structural note:** the bottom decision is the slowest, most error-prone pregame decision (89s + a
commit-lock). Genuinely arithmetic/color-counting heavy and a poor fit for answer-first snap. Both the
guide fix and the parser fix target it.

---

## FOCUS 5 — durdle late-stall off-case: STILL UNEXERCISED (5th corpus)

vs59 (t18 grind, adj win) and vs131 (t18) were the grind windows. deck27 **attacked every turn it had an
attacker** in both (vs59: t4/6/8/10/14/16/18, ending on a DOUBLE Glen Elendra Liege swing; vs131:
t8/10/14/16/18). It NEVER held buffed attackers back. The off-case TRIGGER (walled ground stall +
holding eligible attackers) again did not occur — deck27 follows the prescription (swing every turn
while grinding), and this corpus that behavior WON the vs59 adjudication by one life (12/11) rather than
losing it. Off-case now 5 corpora unexercised; per the skill's low-frequency-off-case note, hold and
re-flag, do not cut. No guide edit on this axis.

---

## Secondary: wither-vs-first-strike annotation clause (representation)

At vs109 seq11 the Oona's Gatewarden option carried `A1 (your blocker dies, attacker lives (wither
shrinks it to 0/0))`. The top-level verdict is correct (Oona dies to Ash's first strike), but the
parenthetical **"(wither shrinks it to 0/0)" is misleading** — with first strike Oona dies BEFORE
dealing its wither, so the attacker is NOT shrunk. The wither annotation does not account for the
first-striker-kills-the-withering-wall interaction. Narrow reopening of the (otherwise-closed) wither
watch (notes.md #2). Low harm (top verdict right; guide now warns against re-deriving first strike), but
the clause is factually wrong.
