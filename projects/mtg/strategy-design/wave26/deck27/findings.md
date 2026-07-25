# deck27 (UB Wither/Oona zombie-merfolk tribal) - wave-26 findings

**Seat:** guided round 7. Binary 77e91ef4e (wave-26 step-1 batch: c1 target-preview,
c2 ETB pay/tap, c3 PT-pump delta, c4 parser hardening, c5 Tergrid may-ask). Corpus
matchups-20260725-115216 (PRIMARY, 21 games, -T 3000, REPPENALTY=1.05). 6 round-robin
games at this seat. **Record 4/6** (up from wave-25's 2/6). **187 decisions across 6
games, 0 fallbacks corpus-wide at this seat, retry fired 0x, 0 parser misfires.**

## Record - 4/6 (W vs137, vs22, vs102, vs131; L vs133, vs18)

| vs  | result | life (me/opp) | turn | shape |
|-----|--------|---------------|------|-------|
| 137 | **WIN**  | 19 / -8 | 10 | Clean evasion clock (Inkfathom + Lord of Atlantis every turn). Was a wave-25 adj-LOSS; flipped hard. |
| 22  | **WIN**  | 2 / -4  | 12 | Close race vs Giants; survived a lethal crackback at 10->2 (seq25 block), then attacked for lethal. |
| 102 | **WIN**  | 9 / 0   | 13 | 102 collapsed to 0/6 this corpus; deck27 out-tempo'd the Tourach grind on evasion + pro-green bodies. |
| 131 | **WIN**  | 14 / -6 | 16 | Textbook Inkfathom + lords evasion clock (repeat of the wave-25 vs131 win). |
| 133 | **LOSS** | -7 / 11 | 17 | mono-B Devotion (rose to 6/6). Lord-heavy clunky keep, ground out by Obliterator + Bloodghast. Matchup/draw. |
| 18  | **LOSS** | -4 / 5  | 17 | Kithkin (Step-0, 4/6) go-wide BLOWOUT: Glorious Anthem + Mobilization built 6/6 + 2x4/4 + soldiers = 18 power vs a deck with no removal/sweeper. Matchup. |

**Read (decisions-based):** 2/6 -> 4/6 is almost entirely POOL SHIFT + variance, NOT a
guide change. The two brutal wave-25 losses (109 Rakdos blowout, 59 WB persistence) both
ROTATED OUT of the pool; 102 collapsed to 0/6 and deck27 beat it; vs137 flipped adj-LOSS ->
clean 19/-8 WIN. The new pool brought 22 (deck27 won) and 18 (a go-wide anthem aggro that
beat deck27, inheriting the wave-25 vs109 "blowout aggro" role). vs133 rose to 6/6 and
took a game deck27 had won in w25. **Both losses (133, 18) are matchup/draw-attributable,
dead-anyway at the decision point - no harmful misplay decided either game.** Attribute per
game; do NOT read 2/6 -> 4/6 as anything the guide did. See FOCUS 5.

---

## FOCUS 1 (JOB 1) - bottom-phase rule-#1 clause: UNEXERCISED THIS CORPUS (zero mulligans)

**All 6 games kept the opening 7 (seq1 = "Keep this hand" every game); ZERO mulligans were
taken, therefore ZERO bottom/PUT windows corpus-wide at this seat.** The wave-25 headline
edit - the bottom-phase clause restating rule #1 (a {U/B} hybrid is the EASIEST card to
cast, ship a double-pip card before a flexible hybrid; strategy.txt lines 120-128) - got
**NO first-corpus exercise.** It neither validated nor failed; it carries forward as
UNVALIDATED / unexercised.

Confirmation: grep for `PUT:` and for `chosen_text == "Mulligan"` across all 6 files = zero
hits. All seq1 mulligan decisions (vs133/vs22/vs137/vs102/vs18/vs131) -> "Keep this hand".

The keeps themselves were all defensible (each had 2-3 lands with both colors available and
an early creature). The one clunky keep was vs133 (Inkfathom + Swamp + 2 Island + 2x Lord of
the Undead + Lord of Atlantis = 3 lands, one 2-drop, three lords) - lord-heavy but legal per
the mulligan rule (early creature + both colors + 3 lands); it lost to a better 133 draw, not
to the keep. No keep created a colorstuck/dead-color opener. Because there was no bottom
phase, the vs102-w25 hybrid-bottom misplay class had no opportunity to recur or be confirmed
fixed. **The bottom clause's first corpus is UNEXERCISED** - carry forward, do not read
absence as confirmation.

---

## FOCUS 2 (JOB 2) - NATURAL-STOP / c4 parser hardening: 3 CLEAN ASK-SEAM RESCUES, 0 MISFIRES

This corpus delivered the **cleaner isolation wave-25 asked for**: three natural-stop
CHOICE reversals at the ASK/CHOICE seam - where answer-first is the ONLY prior floor and
the blocker parser's "none-is-not-an-assignment / last-real-line-wins" behavior CANNOT
co-produce the outcome. In all three the reply led with a WRONG coded line, reasoned to
completion, REVERSED, ended naturally (with a PLAN line), and **the engine applied the FINAL
line.** This isolates b2 natural-stop precedence from the blocker parser - the confirmation
gap the wave-25 reviewer flagged (notes #1) is now CLOSED.

| vs  | seq | first coded line | final coded line (applied) | correct? |
|-----|-----|------------------|----------------------------|----------|
| 22  | 26  | `CHOICE: 1 (Cast Lord of Atlantis)` | `CHOICE: 2 (Cast nothing right now)` ch=2 | YES - casting the lord wastes the turn; casting nothing lets the board attack for lethal (opp at 3, board = 7). deck27 won 2/-4 the same turn. |
| 102 | 14  | `CHOICE: 2 (Play Island)` | `CHOICE: 1 (Play Swamp)` ch=1 | YES - Sanguine Guard needs {1}{B}{B}; playing Swamp secures the 2nd black source, Island would not. |
| 18  | 4   | `CHOICE: 2 (Cast nothing right now)` | `CHOICE: 1 (Cast Metathran Zombie)` ch=1 | YES - "durdling is how you lose"; deploying the 1/1 is the guide-correct developing play. |

Each reply terminated naturally (all three end with a `PLAN:` line). Per the precedence
spec (natural-stop -> last-coded-line) the engine correctly applied the LAST line every
time. **These are the first ASK-SEAM natural-stop positives at this seat and they validate
b2 in isolation from the blocker parser.** (Aside: the model's reasoning in vs22 seq26 and
vs18 seq4 spiraled hard over a stale carried "YOUR PLAN" and phase confusion, but CONCLUDED
correctly and natural-stop captured it - correctness protected, latency the only cost.)

### c4 GATE (misfire check) - CLEAN at this seat

The c4 corpus gate asks: any legitimate reply whose answer was DROPPED or MIS-PICKED
(fallback where the reply is parseable, or a choice contradicting the reply's clear final
answer). **NONE found at deck27's seat.** Every ambiguous case decomposes correctly against
the precedence spec:

- **Natural-stop (3 CHOICE reversals above):** last-coded-line applied - CORRECT.
- **Truncated replies (2, both in the already-lost vs18 game):**
  - vs18 seq35 (blockers, 11,754 chars, ends mid-word "...against A4 (Sold"): first coded
    line `BLOCKS: B1:A1, B2:none` applied (ch=2). Truncated -> first-line = CORRECT. (deck27
    at 10 life vs 18 lethal - dead regardless; block choice immaterial.)
  - vs18 seq37 (attackers, truncated mid-sentence at "...contradicting the plan"): first
    line `ATTACK: A1` applied (ch=1). ATTACK answer-first AND truncated-first AND A1
    (Vodalian Zombie) was the ONLY legal attacker offered - CORRECT on all three counts.
    The later in-body `ATTACK: Vodalian Zombie, Marauding Knight` was an illegal hypothetical
    (Marauding Knight still in hand); the parser correctly did NOT latch it.
- **Spurious "reversal" (vs22 seq25 blockers):** first `BLOCKS: B2(A): A5, B1: A3` vs
  normalized `BLOCKS: B2:A5, B1:A3` - SAME assignment, the parser stripped the "(A)"
  annotation. Not a semantic reversal; ch=2 applied both blocks correctly.

**0 fallbacks / 0 retries / 0 parser misfires across 187 decisions.** The parser hardening
shipped in this binary did not drop or mis-pick a single legitimate reply at this seat.

---

## FOCUS 3 (JOB 3) - Wither-FS steady state (round 5): UNEXERCISED - Oona never blocked

Oona's Gatewarden was CAST once this corpus (vs102 seq3) but was **never a block option**
(vs102 did not attack into it; deck27 was the aggressor and won 9/0). No block seam anywhere
in the 6 games offered Oona as a blocker, and **no block-option annotation string containing
"wither" or "shrinks" appeared** (verified across all `blockers` decisions - the 6 block
seams offered Zombie Outlander, Wasp Lancer, Lord of the Undead, Lord of Atlantis, Metathran
Zombie, Vodalian Zombie, never Oona). The wither-vs-first-strike annotation is therefore
**UNEXERCISED at round 5 - no data, neither confirming nor regressing.** wave-25 already
closed this CLOSED-BY-FACT (a representation fix, no durable model belief); this corpus adds
no new evidence. Plainly: **converged/closed, and no block seam re-tested it.**

For completeness, all 6 block decisions this corpus were CORRECT:
- vs22 seq8: Zombie Outlander (pro-green) takes a free kill ("you kill it, your blocker lives"). Correct.
- vs22 seq12: Wasp Lancer takes a free kill. Correct.
- vs22 seq25: at 10 life vs 13 lethal (8/8 Behemoth trample + 4 small), chumps the BIGGEST
  attacker (Behemoth, B2:A5) + trades Lord into the Automaton (B1:A3, "both die") -> survives
  at 2, wins next turn. **The guide's lethal-block rule (block biggest, decide on line 1)
  working - first line was already survival-correct.** Correct.
- vs137 seq8: Lord of Atlantis takes a free kill on Edgewall Innkeeper. Correct.
- vs18 seq5: Metathran (1/1) trades into Goldmeadow Dodger ("both die"). Defensible early trade.
- vs131 seq8: Vodalian Zombie trades into Young Pyromancer ("both die") - kills UR's
  token-engine. Correct.

## FOCUS 4 (JOB 4) - c1-c5 shifts at this seat

- **c1 (target enumeration):** N/A. deck27 has NO targeted removal or burn, and Oona's
  Gatewarden has no targeted ability - it is a pure wither wall. No single-target cast
  preview / no "(+N more)" / no "choose a target" line appeared in ANY option list this
  corpus (verified). The brief's "removal/Oona seat" framing does not apply here; deck27
  offers c1 no surface to shift.
- **c2 (ETB pay/tap):** N/A. deck27's only nonbasic is Underground Sea (an original dual =
  taps for U or B, no ETB pay/tap choice). No shockland/pathway ETB annotation exercised.
- **c3 (PT-pump delta):** N/A. deck27's lords are STATIC anthems, not activated pumps; no
  activated PT-pump seam exists at this seat.
- **c4 (parser hardening):** VALIDATED CLEAN - see FOCUS 2 (3 rescues, 0 misfires).
- **c5 (Tergrid may-ask):** N/A. deck27 has no may-triggers of that render class. Lord of
  the Undead's {1}{B},{T} recursion is an activated ability, not a resolvable may-ask; it was
  never activated this corpus.

## FOCUS 5 (JOB 5) - 2/6 -> 4/6 decomposition + durdle off-case

**The record rise is POOL SHIFT + variance, not edits and not a decision improvement:**
1. The two hardest wave-25 losses (109 Rakdos go-wide, 59 WB persistence grind) ROTATED OUT.
2. deck102 collapsed to 0/6 corpus-wide; deck27 beat it (was a w25 loss, matchup-flagged since w21).
3. vs137 flipped adj-LOSS (10/16) -> clean WIN (19/-8) - same GW matchup, deck27 curved out and 137 did not.
4. vs131 stayed a win (repeat evasion clock).
5. NEW: vs22 (won), vs18 (lost - go-wide anthem, inherits the w25-vs109 blowout role).
6. vs133 rose to 6/6 and beat deck27 (a w25 win) - matchup regression on THEIR side, not deck27's.

The wave-25 edits are essentially **unexercised-or-benign this corpus**: bottom clause
UNEXERCISED (FOCUS 1); wither/Oona clause UNEXERCISED (FOCUS 3); the lethal-block line was
exercised and worked (vs22 seq25 survival + win; vs18 seq35 dead-anyway). No edit moved the
record; no edit backfired.

**Durdle late-stall off-case: STILL UNEXERCISED (7th corpus).** No game showed a walled
ground stall with HELD eligible attackers - deck27 swung every turn it had an attacker
(vs131 t7-24, vs102 t5-13, vs137 t7-10). The off-case trigger did not occur. Hold and
carry, do NOT cut (per the low-frequency-off-case note).

## FOCUS 6 - verbosity / spiral (informational, harmless)

Two very long truncated replies this corpus, BOTH in the lost vs18 game (seq35 11,754 ch;
seq37 truncated) - driven by a stale carried "YOUR PLAN" that named main-phase casts no
longer legal in the attackers phase, plus phase confusion. Neither cost correctness (the
first coded line was applied and was correct/forced in both). This is the known
carried-PLAN-hygiene aggravator (wave-24 ledger); at this seat it produced latency, not a
misplay. Retry fired 0x (correct - the decode-collapse class did not recur). No guide/core
action.
