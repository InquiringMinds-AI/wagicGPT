# Wave 25 review — deck133 (Phyrexian Asphodel, mono-black devotion midrange)

Corpus: PRIMARY `matchups-20260725-035605` (21 games, binary `/tmp/wagic-62d8783a5` =
wave-25 step-1 batch on the rules-fixed engine `f0500ad66`). deck133 was the GPT seat in 6
games. Pilot: repetition_penalty=1.05 (standing). deck133 STAYED per the wave-24 rotation
verdict (sole witness of the retracted_choice example-leak class; the de-fang `b1` is now
deployed and this corpus is its acceptance test).

**Real record 1W / 4L / 1D** (reconciled from results.tsv + gameend records):

| opp | deck | result | end my/opp | note |
|-----|------|--------|-----------|------|
| 131 | UR Spellslinger | **WON** | 20 / -2 (t14) | Gray Merchant arrived (drain 4 -> recur x3); the ONLY game GM was drawn |
| 137 | GW Adventures | **DRAW** | 10 / 10 (t14) | timeout at cap, even life; Collective Brutality grind, no GM/Obliterator |
| 27 | UB Zombies | **LOST (adj)** | 10 / 14 (t12) | behind-4 at cap; but DEALT 6 dmg (opp 20->14) — IMPROVED vs prior 0-dmg blowouts |
| 59 | WB Persistence | **LOST** | -4 / 13 (t13) | ground out by persist value; GM/Obliterator never drawn; opp is now 6/6 |
| 102 | mono-B Tergrid | **LOST** | -7 / 23 (t12) | over-discarded vs control; no clock; GM/Obliterator never drawn; Arena-at-7 |
| 109 | mono-R Aggro | **LOST** | 0 / 18 (t8) | fast aggro kill; threat-drought (0 GM/Obl/Geralf); discard-heavy open; Arena-at-5 |

Down from wave-24's 4W/1L/1D. **This is NOT a decision-quality collapse — it is a THREAT-DROUGHT
+ pool-rise story** (finding 6), with ONE genuine, actionable, RECURRING guide-adherence flaw
(finding 1, the ONLY new work this corpus). TRUE model-fallbacks at this seat: **0** (all 15
corpus fallbacks are deck137 convoke defers). The retracted_choice class deck133 uniquely produced
is GONE (finding 2). Guide **REVISED** — one surgical Arena sharpening (finding 1); all other
rules kept.

---

## FINDING 1 — [PHYREXIAN ARENA CAST AT LOW LIFE UNDER LETHAL PRESSURE] — 2-window convergent guide-contradicting rationalization. Route: GUIDE (surgical sharpen of the existing Arena line). The one new-work item.

deck133 cast **Phyrexian Arena while low and under lethal pressure in BOTH aggro/tempo losses**,
each time rationalizing it as stabilization / answer-finding — the exact OPPOSITE of what Arena
does. The existing guide already says "skip casting it when defending" (line 126) and DEFENDER
rule 3 says "skip Phyrexian Arena (1 life/turn)" — the model **quoted the guide and violated it
anyway**, the deck109-family "belief overrides the stated rule" shape. The current rule is too
passive to survive the model's rationalization; it needs to name the specific conflation.

- **vs109 s19 (my5/opp20, turn 7, facing 4 attackers next turn).** Options: 1. Cast Phyrexian
  Arena / 2. Cast Thoughtseize (opp hand empty) / 3. Cast nothing. Chose **Arena**. Reply verbatim:
  *"You are at 5 life facing four attackers next turn, so stabilizing is critical. Casting Phyrexian
  Arena immediately starts the card draw engine to find removal..."* — Arena is the inverse of
  stabilization; it costs 1 life/upkeep and its draw is at your NEXT upkeep (never arrives — died t8).
- **vs102 s28 (my7/opp18, turn 11, two attackers that "will likely kill me next turn").** Chose
  **Arena** again. Reply verbatim: *"it provides immediate card advantage (draw 1, lose 1)... I lose
  the game next turn regardless... so I must take the card draw to find an answer."* Two errors:
  (a) a TIMING/RULES error — the model believes Arena draws "immediately"; the engine script is
  `auto=@each my upkeep:draw:1` (oracle: "At the beginning of your upkeep, you draw a card") — it
  does NOT draw on cast, so under lethal-next-turn the draw never comes; (b) the "regardless"
  framing is half-right (both instances were already lost) but the habit is wrong.

**Deciding?** BOTH instances were NON-DECIDING — deck133 was already lost (5 life vs 4 attackers /
7 life facing lethal, empty answer-hand). But the reasoning is systematic, 2-window convergent, and
would be DECIDING in a marginal spot (at 2-3 life where 1 life = the game, or when Arena competes
with a real play). Per the strategy-writing skill's belief-correcting-crutch discipline, the right
move is to SHARPEN the existing rule to name the observed conflation, not add a new crutch.

**Guide edit (grounded in oracle + `auto=` script):** line 126 rewritten to state the timing fact
(draws at your NEXT upkeep, not on cast), the 1-life-every-upkeep cost, and that Arena is "not a
play, it is a countdown" — at low life with nothing else to cast, cast NOTHING. Only cast Arena when
life is SAFE and grinding for cards. See strategy.txt.

---

## FINDING 2 — [EXAMPLE-LEAK CLOSURE (b1)] — retracted_choice class GONE corpus-wide, but STRONG confirmation is UNAVAILABLE: the quoting shape vanished entirely (WEAK evidence, and that is the CEILING). Route: HARNESS/PARSER (notes.md, keep the hardening as belt-and-suspenders).

deck133 owned the corpus's ONLY retracted_choice in wave-24 (vs131 s21: a clean first-line
`CHOICE: 1` deferred because the parser's second-coded-index scan latched the prompt's quoted
`e.g. "CHOICE: 2 (Cast Fatal Push)"` — whose card name equaled a live option). The b1 de-fang
changed the protocol example to a non-live name (`"CHOICE: 3 (Cast Example Card)"`).

Acceptance this corpus, per the skill's shape-present rung (shape-present-and-passing = STRONG,
shape-absent = WEAK):
- **retracted_choice at deck133: 0. Corpus-wide: 0.** (All 15 fallbacks are deck137 convoke defers.)
- **The QUOTING SHAPE VANISHED:** across all 6 deck133-seat logs (227 non-gameend replies),
  replies quoting the new example "Example Card" = **0**; replies with `"CHOICE:` quoted in prose
  = **0**; format-spiral signatures ("stick to the format", CHOICE-number-vs-target-name
  oscillation) = **0**. In wave-24 the model quoted `CHOICE:` ~30x in the single s21 record.

**Verdict: the de-fang looks EFFECTIVE, but this is WEAK evidence and weak is the CEILING.** The
de-fang works precisely by removing the aggravator (example name = live option) that PRODUCED the
quote-latch — so the very behavior you would use to STRONG-confirm the fix (the model still quoting
the de-fanged example, and it parsing clean) can no longer occur. I cannot distinguish "the de-fang
killed the leak" from "the model happened not to quote CHOICE at all this corpus." The 0-retracted
result is consistent with the fix and there is no counter-evidence, but absence-of-recurrence is all
that is obtainable. RECOMMENDATION: still ship the wave-24 parser hardening (count only LINE-ANCHORED
coded indices; exclude `CHOICE:` inside quotes or that echo the protocol example) as belt-and-
suspenders — a future example text or model shift could re-trigger; the de-fang alone is a
trigger-removal, not a parser guarantee. (notes.md, OPEN-low-priority.)

---

## FINDING 3 — [LILIANA DISCIPLINE ROUND 3] — CLEAN. Zero symmetric-+1-while-behind anti-pattern across 4 activations. Route: guide KEPT.

Four Liliana loyalty activations rendered; the wave-23 anti-pattern (symmetric +1 self-discard while
defending) did NOT recur:
- **vs137 s18 (my17/opp18):** Veil **-2** (edict). Correct.
- **vs137 s36 (my15/opp12, AHEAD):** Veil **+1** (discard). The one +1 taken — while ahead in a
  closing grind; the caution permits +1 when ahead. Acceptable.
- **vs137 s42 (my11/opp12):** Veil **-2** (edict). Correct.
- **vs59 s35 (my2/opp16):** Liliana, the Last Hope **+1** (-2/-1 shrink) — Last Hope's +1 is pure
  value, NOT symmetric; correctly distinguished the two walkers. (Losing position; the +1 was fine,
  the game was already lost on threat-drought.)

**Verdict:** discipline holds round 3; no edit. The two-walker distinction (Veil-symmetric vs
Last-Hope-value) is being applied correctly.

---

## FINDING 4 — [BLOODGHAST FORCED-SAC FODDER LINE ROUND 2] — UNEXERCISED. Route: guide KEPT (cheap insurance).

No forced-sacrifice / edict / forced-discard window rendered at deck133's seat this corpus
(opponents 27/59/102/131/109/137 presented none; vs59 is a persist deck, not an edict deck). The
Bloodghast-fodder clause could not be re-validated round 2 — remains correct, deck-specific, cheap
— **KEEP**. Weak corroboration only: Yawgmoth's own sac would exercise the same logic, but Yawgmoth
was never cast this corpus (Yawgmoth audit below).

---

## FINDING 5 — [RULES-SHIFT WATCH (batch A) + PREGAME] — no observable behavior shift at this seat.

- **(a1) turn-1 upkeep triggers now fire.** deck133's ONLY upkeep-trigger card is Phyrexian Arena
  (`@each my upkeep`); Bitterblossom is NOT in the deck (verified decklist). Arena was cast late
  both times it appeared (t7, t11) — never turn 1 — so no turn-1 upkeep trigger surfaced at this
  seat. Rules-shift a1: **untestable / no effect here this corpus.**
- **Mulligan / turn-0 integrity:** all 6 games opened with exactly ONE `Keep this hand` at s1
  (phase now `t0 Untap`, consistent with a1's real turn-1 upkeep). No double-asks, no London-path
  needed this corpus (all kept the seven). Starting player skips the turn-1 draw. Clean.
- **(a4) pw damage-redirect removal, (a2/a3/a5/a6):** no deck133 decision touched these surfaces
  (no planeswalker-damage redirect, no Battle, no day/night). No shift observed.
- **Shockland/pathway annotation (b4):** deck133 runs fetches only (no shocklands/pathways); the
  fetch "Put in Play targeting Snow-Covered Swamp [cost: Tap, Life, Sacrifice]" labels rendered
  correctly and were cracked on-curve. Untestable for b4 proper.

---

## FINDING 6 — [THE 1/6 DECOMPOSITION] — dominant driver is THREAT-DROUGHT + pool-rise, NOT decision collapse. Route: CONSTRUCTION (notes.md, re-confirmed) + context.

Decomposed honestly per the brief. In the **three games that flipped from wave-24 W/D to a wave-25
loss**, deck133 drew essentially NONE of its win conditions:

| game | flip | Gray Merchant offered | Obliterator offered | Geralf's offered |
|------|------|----------------------|---------------------|------------------|
| vs109 | W -> L (t8) | **0** | **0** | **0** |
| vs102 | D -> L | **0** | **0** | **0** |
| vs59 | W(adj) -> L | **0** | **0** | 2 (cast 1) |

The deck's only deployed body across long stretches of all three was **Bloodghast (which cannot
block)**. The ONE win (vs131) is exactly the game where **Gray Merchant arrived** (cast s24 drain 4,
recurred to close 20/-2). This is the deck's structural signature: it wins when it draws a payoff
(GM/Obliterator) and loses when it floods on discard + Bloodghast. Contributing factors, ranked:

1. **Threat-drought / draw variance (dominant).** GM+Obliterator offered 0x in all three flipped
   losses. No pilot wins mono-B devotion drawing zero payoffs vs aggro/control.
2. **Pool rose (the brief's hypothesis — CONFIRMED partial).** deck59 went 3/6 -> 6/6 on its first
   guide; deck133's vs59 flip W(adj) -> L is substantially deck59's guide coming online, not
   deck133 regressing. deck133's DECISIONS vs59 were reasonable; it was ground out by a value engine.
3. **vs27 actually IMPROVED** — dealt 6 damage (opp 20->14) and lost only by adjudication (behind 4),
   vs the prior-wave 0-damage blowouts. The structural no-reach flag is quieter this corpus.
4. **The Arena misplay (finding 1)** appeared in the two aggro/tempo losses but was NON-DECIDING
   (both already lost). It is a latent, not a proximate, cause of the record.

**No hard deciding decision error caused any of the four losses.** The record drop is variance +
opponent improvement, carrying one real-but-non-deciding guide-adherence flaw worth fixing now.

---

## MANDATORY AUDITS

- **Example-leak / retracted_choice acceptance (b1):** 0 corpus-wide; STRONG confirmation
  unavailable (quoting shape vanished) — WEAK-but-clean, ceiling reached (finding 2).
- **Yawgmoth SacrificeCost round 2:** **UNEXERCISED** — Thran Physician was never cast this corpus
  (0-1 mentions; the many "Yawgmoth" hits are Urborg, Tomb of Yawgmoth the LAND, 52/68 mentions in
  vs59/vs102). No `-1/-1 Counter with Yawgmoth` offer rendered; the wave-24 fix (37 offers, all
  payable) could not be re-witnessed. No regression signal — simply not drawn into an activatable
  window.
- **Liliana discipline round 3:** CLEAN, 4 activations, 0 anti-pattern (finding 3).
- **Bloodghast forced-sac fodder round 2:** UNEXERCISED (no edict at this seat; finding 4).
- **Self-Fatal-Push watch:** CLEAN. FP casts hit opponent creatures (Tinybones vs102 s25;
  Safehold Elite vs59 s12/s23 — persist, a legitimate double-kill; Walking Dead/Lord of the Undead
  vs27 s35/s37). No self-targets, no fizzles observed.
- **Gray Merchant:** 1 cast (vs131 s24, drain 4, correct win-mode; recurred x3). Offered 0x in the
  3 flipped losses — a DRAW fact, not a wait-rule failure.
- **Fallbacks at this seat: 0.** No empty_reply, no unparsed, no retracted_choice, no stale_echo.
- **Longest-reply decode-garbage spot-check (retry fired 0x):** vs59 s33 (13,320 chars, 215s,
  attackers) top-6gram repeat = 5 (not degenerate), answer-first `ATTACK: A1 Bloodghast`, parsed
  clean; vs137 s34 (13,094 chars, 202s, ask) re-emitted `CHOICE: 2 (Collective Brutality)` at head
  AND tail (consistent), parsed clean. Long deliberations are content-rich, not decode-collapse.

## PILOT VERDICT (repetition_penalty=1.05): SAME — no degradation.
227 non-gameend replies: p50 **1128** (wave-24 1129 — identical), p90 5006, p95 8412, max 13320.
No degenerate n-gram repetition (the one 12-count 6-gram is semantic re-emission of a consistent
answer, not decode-collapse). No prose degradation. Consistent with wave-24.

## FREEZE / EDIT CHECK
Guide **REVISED** (NOT byte-frozen this cycle) — ONE surgical edit: the Phyrexian Arena line
(finding 1), grounded in oracle text + the `@each my upkeep` engine script. Deployed guide sha256
`6ed73f00...39ce`; revised strategy.txt in wave25/deck133/ (the Arena line only; all other bytes
identical). See notes.md for the routed ledger + rotation verdict.
