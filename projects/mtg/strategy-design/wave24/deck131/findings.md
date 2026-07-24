# deck131 — wave 24 review (UR Guttersnipe spellslinger). Guttersnipe guard round 2 + pregame mulligan phase.

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260724-125739/` (binary /tmp/wagic-d7ff8b6c4:
step-1 batch + PRE-GAME MULLIGAN PHASE + N9 hotfix). deck131 = 6 games. Live guide = the wave-23
REVISED guide (10262 bytes; deployed == wave23/deck131/strategy.txt, byte-verified — the Guttersnipe
attack guard with per-creature [tapped]-read is LIVE this corpus). Pilot: repetition_penalty=1.05.

**Record: 0 W / 6 L (0/6).** Seat->game map (results.tsv; 131 is deck0 vs 59/137, deck1 vs the rest):

| result | vs (deck) | end life me/opp | turn | one-line cause |
|---|---|---|---|---|
| LOSS (death) | 59 (WB Persistence, GUIDELESS) | -5 / 16 | 17 | both payoffs landed but dealt only 4 dmg; **s23 tapped-MISREAD lost Guttersnipe** |
| LOSS (death) | 133 (Phyr. Asphodel) | -4 / 7 | 12 | curved out BOTH payoffs, raced opp to 7, out-raced by a bigger black deck |
| LOSS (adj) | 137 (Selesnya Adventures) | 11 / 15 | 12 | single YP, no Guttersnipe; clean go-wide race, lost adj by 4 |
| LOSS (death) | 109 (Mono-R Hellrider) | -3 / 20 | 12 | aggro out-race; **dealt ZERO damage** (opp stayed 20); no early defense |
| LOSS (adj) | 102 (Tergrid) | 13 / 32 | 14 | single YP, no reach; opp lifegained to 32; no-reach-vs-lifegain terminus |
| LOSS (death) | 27 (Undead Infiltrator) | -1 / 12 | 18 | guard HELD 4x; lost both Guttersnipes to model-adherence slips (s30/s31) |

**Bottom line:** the 0/6 is **CONSTRUCTION-TERMINAL, not guide-fixable.** Four losses (109, 102, 137, 133)
are clean construction/variance with correct play; two (59, 27) carry play errors but on a construction
backdrop, and those errors are **model-adherence / answer-first extraction, NOT guide-prose gaps** (in
vs27 the model QUOTED the guide correctly and still misfired). No guide edit flips any of the six to a
win. The guide is obeyed on every axis it covers: **mulligan 6/6 correct by the STEP-1 gate — including
the headline: vs27 the model MULLIGANED a no-red hand (the exact scenario the guide targets), then vs133
got a mulligan ask this time (last wave's I-2 harness gap is RESOLVED by the pregame phase)**; Mountain-first
clean; Guttersnipe guard held 4/4 in its origin matchup (vs27) including the exact Lord-of-the-Undead spot
that broke last wave. The ONE guide move this wave is additive: a BOTTOM-priority line for the brand-new
London-mulligan bottom phase (vs27 bottomed its useful Counterspell and KEPT its worst card, Prism Ring).

---

## MANDATE (1) — THE 0/6, DECOMPOSED HONESTLY BY DECISIONS

Zero fallbacks at this seat this corpus (no empty_reply, no retracted_choice, no stale_echo — the
cleanest deck131 corpus on record). So every loss is a real, fully-piloted game.

### Construction-terminal (4/6) — correct play, lost to the deck's known limits:
- **vs109 (Mono-R Hellrider) — L -3/20 T12. PURE construction.** Single YP landed; **131 dealt ZERO
  damage the entire game (opp 20 -> 20)** and was killed by T12. No attackers/blockers records logged —
  its lone payoff never survived to swing. This is the standing *no early defense vs aggro / low threat
  density* terminus, textbook. Nothing guide-addressable.
- **vs102 (Tergrid) — adjL 13/32 T14. PURE construction.** Single YP, no Guttersnipe drawn; opponent
  lifegained/valued up to **32** while 131 could not push reach. The *no-reach-vs-lifegain* terminus
  (same shape as the wave-23 vs140 outage game). 19 priority decisions, all reactive; the deck simply
  cannot close a lifegain deck without Guttersnipe. NB: last wave 131 BEAT Tergrid on a YP-swarm race —
  this wave it drew a single payoff. That delta is DRAW VARIANCE, not a play regression.
- **vs137 (Selesnya Adventures) — adjL 11/15 T12. Construction + variance, NARROW.** Single YP, no
  Guttersnipe (threat-density variance); clean go-wide race, lost adjudication by 4 life. No lifegain-in-race
  slip this time (contrast wave-23 vs137, which had the Prism Ring/Elixir-in-race slip). Clean play, close game.
- **vs133 (Phyrexian Asphodel) — L -4/7 T12. Construction, WELL-PLAYED.** Best offense of the corpus:
  mulligan-kept a 2-Mountain+YP hand, **curved out BOTH payoffs (YP T5, Guttersnipe T6), raced opp to 7.**
  Lost the race to a bigger/recursive black deck (Obliterator 5/5 trample, Geralf's undying). Guttersnipe
  discipline correct throughout (swings s14/s25 into all-tapped boards; held s19 into an untapped
  Obliterator). This is exactly how the deck is supposed to play — it just lacks the defense to survive the
  race it loses. NOT guide-fixable.

### Play errors on a construction backdrop (2/6):
- **vs59 (WB Persistence) — L -5/16 T17. Construction-dominant + ONE tapped-misread (see notes I-1).**
  Both payoffs landed (YP T4, Guttersnipe T10) yet 131 dealt only 4 total damage (opp 20 -> 16) before
  dying -5 — it cannot race a persist/go-wide deck. **s23: the model swung Guttersnipe claiming "both
  [opponent creatures] tapped" when Restless Apparition had NO [tapped] tag; it was untapped, blocked, and
  traded away Guttersnipe.** This is the wave-23 I-1 tapped-misread class RECURRING in a new matchup — the
  guard did NOT prevent it here. Likely non-decisive (131 was already being raced down 25-to-4), but a real
  play error and the key data point on the guard's residual frailty (MANDATE 2).
- **vs27 (Undead Infiltrator) — L -1/12 T18. Construction backdrop + TWO model-adherence slips.**
  Two Guttersnipes landed (T7, T15); no YP-swarm. **The guard WORKED on decision-input: held Guttersnipe
  home 4/4 on untapped boards (s12/16/21/25), correctly reading Lord of the Undead as UNTAPPED at s12 —
  the exact spot that broke last wave.** No-block discipline also clean 3/4 (s14/18/26 all correct "no
  blockers"). The two losses of Guttersnipe were NOT guide gaps:
  - **s30 (attackers, T15): CHOICE-line-vs-reasoning CONTRADICTION.** The reasoning is impeccable and
    quotes the guide: *"three untapped creatures that can block, swinging a Guttersnipe risks trading it...
    you must leave Guttersnipe home... the correct combat declaration is to attack with none."* Then a
    *"Correction: Wait, looking at the prompt again..."* spiral flips it, and the reply's FIRST line is
    `ATTACK: A1`. The harness locked the first line -> Guttersnipe attacked into a 4/3 Oona's Gatewarden
    (wither), took 4 -1/-1 counters, died for nothing. **The board read was CORRECT; the guide was
    followed in prose; the answer-first first-line overrode both.** Harness/model layer (notes I-2).
  - **s31 (blockers, T16): non-lethal Guttersnipe BLOCK.** By the model's OWN survival math taking all
    damage drops it to 1 (not 0/lethal), yet it blocked Lord of the Undead with its last Guttersnipe.
    Violates "NEVER block with Guttersnipe unless incoming puts you at 0." Model-adherence slip under
    lethal-adjacent pressure (the standing protect-discipline-relaxes-under-pressure WATCH).

**Verdict on the 0/6 for the roster owner:** this is the **standing 8+-wave construction terminus**,
unchanged and now cleanly isolated (zero fallbacks, guide fully obeyed where it applies). Threat density
6/60, no reach vs lifegain (102), no early defense vs aggro (109), cannot out-race bigger/recursive decks
(133, 59). **The deck brought opponents to 16/7/15/20/32/12 — only two games (133->7, 27->12) and one
narrow adj (137, 11/15) were close, and NONE was lost to a guide-fixable decision.** No guide rewrite
changes this record; it is a construction ceiling. Recorded for the owner, not relitigated (notes).

---

## MANDATE (2) — GUTTERSNIPE TAPPED-READ GUARD, ROUND 2: PARTIALLY VALIDATED

The guard (per-creature [tapped]-read + never-attack-into-a-possible-trade) shipped wave-23 against the
vs27 misread. Round-2 evidence across every Guttersnipe attack window this corpus:

| game | seq | board (relevant) | model read | decision | verdict |
|---|---|---|---|---|---|
| vs27 | s12 | Gatewarden + **Lord of Undead (untapped)** | "Neither tapped, both can block" | HOLD | **CORRECT — the wave-23 break spot, now read right** |
| vs27 | s16 | 2x untapped Gatewarden | "two untapped Gatewardens" | HOLD | CORRECT |
| vs27 | s21 | 2x untapped Gatewarden | correct | HOLD | CORRECT |
| vs27 | s25 | 2x untapped Gatewarden (defender) | "can legally block" | HOLD | CORRECT (defender still blocks) |
| vs133 | s14 | Bloodghast + Geralf's, **both [tapped]** | "both tapped" | SWING | CORRECT |
| vs133 | s19 | + **Obliterator (untapped)** | (hold, right conclusion) | HOLD | CORRECT |
| vs133 | s25 | all three [tapped] | "all tapped" | SWING | CORRECT |
| vs59 | **s23** | Kitchen Finks [tapped] + **Restless Apparition (untapped)** | **"both tapped"** | **SWING** | **FAIL — misread, lost Guttersnipe** |

**Round-2 result: the guard demonstrably WORKS when the model reads carefully (7/8, including the exact
Lord-of-the-Undead spot it was written for), but it does NOT eliminate the misread class — a fresh
tapped-misread appeared vs59 s23.** Root unchanged from wave-23 I-1: the representation is CORRECT and
unambiguous ([tapped] present on one creature, absent on the other); the model conflates "one is tapped"
into "all tapped." The guard prose already says exactly the right thing per-creature; vs59 shows the model
sometimes ignores it. **Shouting louder at an already-explicit, already-validated line is the sanctioned-
against move — I did NOT touch the guard.** The residual is a stochastic model-comprehension frailty ->
notes.md WATCH (now 2 matchups at this seat: vs27 wave-23, vs59 wave-24). The s30 contradiction adds a
SECOND failure mode: even a CORRECT read + CORRECT guide-quote can be overridden by an answer-first first
line. Neither is guide-addressable.

---

## MANDATE (3) — PREGAME MULLIGAN PHASE VALIDATION (first fleet outing) — BEST MULLIGAN PROBE IN CORPUS

**(1) Mulligan ask rendered + keep/mull decisions sensible: 6/6 CORRECT by the guide's STEP-1 gate.**
All six games opened with a proper turn-1 London-mulligan ask. Decisions vs the STEP-1 red-source gate:
- vs59: `2 Mountain + YP + ...` -> KEEP. Correct (red + payoff).
- vs133: `2 Mountain + YP + ...` -> KEEP. Correct. **(Last wave this seat had NO mulligan record — the
  wave-23 I-2 harness concern; the pregame phase RESOLVED it: a mulligan ask now renders here.)**
- vs137: `2 Mountain + YP + ...` -> KEEP. Correct.
- vs109: `2 Mountain + YP + ...` -> KEEP. Correct.
- vs102: `1 Mountain + YP + ...` -> KEEP. Correct.
- **vs27: `Counterspell; 3 Island; Downsize; Elixir; YP` — NO Mountain, no dig spell -> the model chose
  MULLIGAN.** This is the guide's most-cited STEP-1 line FIRING and the model OBEYING it — the exact
  no-red hand that wave-13 kept and wave-23 never had to face. **Headline positive.** Post-mull hand
  `Counterspell; Mountain; Prism Ring; 3 Island; Essence Scatter` -> KEEP (marginal: has red but no
  payoff/dig; a mull-to-5 with this deck is grim, so keeping a red-source + interaction hand at 6 is
  defensible even though STEP-2's "also has a payoff or draw spell" isn't strictly met).

**(2) The one "bottom" record — did it bottom its N worst? NO (the single miss this corpus).** vs27 s3
bottomed 1 (kept 6). Options `[Counterspell, Mountain, Prism Ring, Island, Island, Island, Essence Scatter]`;
**the model bottomed Counterspell and KEPT Prism Ring.** By the guide's own valuation Prism Ring is the
lowest-impact card in the deck ("lifegain only... does NOT advance your clock"), so the correct bottom was
Prism Ring, not a useful interaction spell. Non-decisive (the hand was threat-less regardless), but a real
bottom-quality miss on a NEW surface the guide gave no guidance for -> the ONE justified guide edit this
wave (BOTTOM STEP line, faithful to the guide's existing Prism Ring/Elixir ranking; see strategy.txt).

**(3) Game-start integrity: clean.** All six opened at turn 1 with the mulligan; the one mulligan bottomed
the correct N (1); no double-asks, no wrong-N. **(4) Leyline window: N/A** (deck131 runs no leyline card).
**(5) Phase anomalies: none observed** (no narration confusion flagged; the redraw events rendered cleanly).

---

## MANDATE (4) — REPPENALTY=1.05 round 3: SAME (no degradation)

- Reply length p50 1125 / **p95 5556** / max 14993 chars (169 replies). Wave-23 seat p95 was 6078 —
  this corpus is **shorter at the tail** (no spiral lengthening). Max 14993 is within corpus norms
  (wave-23 corpus max 15901).
- 43/169 replies contain a "Wait,"/"Correction" self-correction pass — the normal answer-first HARNESS-N9
  family, coherent, NOT token-level repetition loops or decode collapse. Every reply examined opens with a
  clean in-range CHOICE/ATTACK first line.
- **Answer-locked RETRY fired 0x — CORRECT for this seat: there were no garbage/decode-collapse replies to
  catch.** The one problematic reply (vs27 s30) is a COHERENT reply whose leading token contradicts its own
  body (answer-first extraction), a different class than decode-collapse; the retry is right not to fire on it.
- Latency p50 17.4s, one 190s outlier (no timeout-draw at this seat).

**Verdict line:** rep_penalty=1.05 at deck131 — **SAME**; tail shorter than wave-23 (p95 5556 vs 6078),
prose coherent, self-correction is the normal N9 family, zero decode-collapse, retry correctly 0.

---

## strategy.txt change (justified) — ONE surgical edit

Added a BOTTOM STEP bullet to the MULLIGAN section (everything else byte-identical to the deployed
wave-23 guide; diff = pure insertion, verified). Justification: the London-mulligan BOTTOM phase is a
BRAND-NEW engine surface the guide never covered, and the corpus's single bottom decision was suboptimal
(kept Prism Ring — the deck's lowest-value card by the guide's own ranking — over a Counterspell). The
edit reuses the guide's existing card valuation (Prism Ring/Elixir lowest, Mountain/payoffs/dig highest)
to give an explicit bottom order. This is NOT "shout louder at a covered line" (the phase had zero
coverage) and NOT touching the validated-but-imperfect tapped guard (sanctioned-against). Low-risk (bottom
decisions occur only on mulligans, ~1 game in 6). It will not change the construction-terminal record; it
closes the one observed gap on the new surface this seat is designated to probe.
