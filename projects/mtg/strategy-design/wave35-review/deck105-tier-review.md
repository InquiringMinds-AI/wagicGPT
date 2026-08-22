# deck105 (Infected Wounds — BG Infect) — Wave-35 Tier Review

**Seat**: deck105, poison-kill deck. **Rubric**: owner's verbatim ruling — win rate irrelevant; score guide adherence and, on divergence, whether departing was correct AT THAT MOMENT.

- **Arm A** (qwen36-35b-a3b): `matchups-20260820-192210/*ai_baka_deck105*` — 6 games, 132 scoreable decisions
- **Arm B** (qwen35 122B-A10B): `matchups-20260821-033000/*ai_baka_deck105*` — 6 games, 118 scoreable decisions

Guide core lines extracted for scoring: (1) poison-not-life win condition, read the printed poison line, never reconcile vs life; (2) block rule — LETHAL → block; growing attacker → subtract its power again; LIFE AFTER ≤13 → BLOCK NOW with smallest bodies; ≥14 → none; Skithiryx/Putrefax never block; (3) standing loop — land, biggest infect cast(s), attack with the full eligible list; (4) card rules — Ichorclaw only pumps when BLOCKED (nothing as a blocker), Putrefax cast+attack same turn, Corpse Cur always return biggest, Hand of the Praetors trigger at opponent, Necropede counter at theirs, Relic Putrescence never on own artifact; (5) mulligan — keep on 2–4 lands + creature + a "would cover" line naming a spell.

---

## Arm A (35B) per-game findings

**vs deck139 (WON t13, poison 11).** Curve-out and full attacks every turn (seq7, 11, 13, 16, 18); Putrefax cast and attacked the same turn (seq15–16, t11); alpha at 7-of-10 poison per the guide's closing rule (seq18). One divergence at **seq12 t8**: LIFE AFTER 12 (≤13, guide says BLOCK NOW), chose "no blockers" vs Brokkos 6/6 trample with only a 1/1 available. Trace quotes the rule, then overrides on correct board math: the option line itself says "5 tramples to your face", so chumping saves 1 life and loses an attacker. **JUSTIFIED AUDIBLE.**

**vs deck152 (LOST t12).** Divergence at **seq9 t6**: LIFE AFTER 18 (≥14, guide says "BLOCKS: none"), chose "Ichorclaw Myr blocks Wolf". Trace: *"If I block with Ichorclaw Myr: Myr becomes blocked, gets +2/+2 → becomes 3/3 … Wolf dies. This is a favorable trade."* — Ichorclaw pumps only when IT becomes blocked; the guide says verbatim "It gets nothing for blocking — as a blocker it is a 1/1", and the prompt's own option line printed "(your blocker dies, attacker lives)". Threw away a body at zero life risk. **COMPREHENSION FAILURE** (card-trigger misread, doubled with the ≥14 rule violation). Later block at LIFE AFTER 7 (seq14) was adherent.

**vs deck158 (LOST t12).** Adherent blocks/attacks throughout (seq8 & seq11 no-block at 18/14 with growth check; seq17 double-chump at LIFE AFTER 7; seq19–20 Putrefax same-turn all-in). Divergence at **seq14 t9**: engine listed "Cast Ichorclaw Myr {2}" and "Cast Necropede {2}"; model chose "Cast nothing". Trace: *"My available mana is {G}{B} … Neither can be cast!"* — colored mana pays generic costs; and the guide says verbatim "If a Cast option is listed the engine has already checked you can pay it — take it, do not recount your lands." **COMPREHENSION FAILURE** (mana-rules misread overriding an explicit engine guarantee). Cost a full development turn in a race it lost.

**vs deck36 (WON t12, poison out).** Adherent curve/attacks; Ichor Rats cast despite self-poison (seq11, per guide); two casts in one turn when mana allowed (seq15–16); blocks at LIFE AFTER 6 and 7 taken (seq18, 22). Divergence at **seq13 t7**: LIFE AFTER 10 (≤13 → BLOCK NOW), chose none vs two non-growing Cathodions. Trace quotes step 3, then leans on the composed prompt's generic hint (*"NOT lethal: block only where the trade favors you"*) and on race logic: only untapped body was Ichor Rats, its 2-power clock mattered more than 1 life vs static attackers, and it won by poison on t12. **JUSTIFIED AUDIBLE** — but note it explicitly used the generic hint line as license to override the deck guide (prompt-seam, see docket).

**vs deck146 (LOST t20).** Long defensive grind, fully adherent: no-blocks at 17 and 14 (seq5, 9 — boundary case handled exactly per "14 or more"); blocks at 8/5/6 (seq17, 20, 26 — seq20 only one blocker existed); Blight Mamba regenerated during the combat it was needed in (seq24–25, per card note); kept casting and attacking to the end. No divergences; lost on draw quality, not play.

**vs deck116 (WON t12, poison out).** Adherent throughout. The five DECLINED-CAST flags (seq13, 15, 18, 20, 23) are all Relic Putrescence with "the only legal targets are YOUR OWN right now" — the guide says decline; all five declines correct. Full attacks each turn, alpha at 8-of-10 (seq24). No divergences.

## Arm B (122B) per-game findings

**vs deck139 (WON t13, poison 10+).** Fully adherent: curve, full attacks (seq7, 9, 13, 17, 19 — 5-creature alpha at 7-of-10), Corpse Cur return taken (seq16), Necropede death-counter pointed at opponent's Gemrazer (seq10–11). No divergences.

**vs deck158 (WON t9).** seq5 t4 is the guide's step-2 showcase: LIFE AFTER 17 with a counters-grown Orc army 3/3 — trace explicitly computes 17−3=14 → "not ≤13" → none, and separately declines to trade its only creature. seq13 t8 LETHAL line (−13) → blocked and survived at 2. seq15–16: Putrefax cast+attack same turn for the win (poison 4 + 9 power). Zero divergences; the two hard block calls were resolved by literally executing the guide's arithmetic.

**vs deck152 (WON t15).** Adherent: no-block at LIFE AFTER 14 boundary (seq12), Necropede counters at opponent's Briarbridge Tracker and Wolf (seq22–25), Hand of the Praetors cast-trigger at the opponent (seq31), Clue draws with spare mana (guide-silent, reasonable), full attacks. No divergences.

**vs deck36 (WON t10).** seq9 t5: LIFE AFTER 8 → BLOCK NOW; only legal assignment (Cystbearer may block only Cathodion) taken, survived. Hand of the Praetors triggers at the opponent three times (seq14, 16, 20), double-casts when mana allowed, winning alpha t10 with the Hand pump. No divergences.

**vs deck146 (LOST t12).** Shortest log (13 decisions). Adherent curve and full attacks; no blockers asks ever offered (bodies tapped from attacking — the guide's own all-in risk profile). Lost the race; no divergent decision to score.

**vs deck116 (WON t8).** Adherent; seq13 no-block at 18; seq15–16 Putrefax cast+attack same turn, alpha 3+8 poison for the win on t8. No divergences.

Both arms: 12/12 mulligan keeps were guide-legal (every kept hand had a "would cover the cost of" line naming a spell, 2–4 lands, creatures). Both arms tracked poison lethality off the printed line correctly every time it mattered (e.g. A-139 seq15 "3+9=12", seq17 "7+4=11"; B alphas at 7 and 8 of 10) and never reconciled poison against life.

---

## Divergence table

| Arm | Game | seq/turn | Guide line broken | Choice | Verdict |
|---|---|---|---|---|---|
| A | vs139 | seq12 t8 | LIFE AFTER 12 ≤13 → BLOCK NOW | no blockers vs 6/6 trample, sole 1/1 up | **JUSTIFIED AUDIBLE** — chump saves 1 life through trample, loses an attacker; math in trace is correct |
| A | vs152 | seq9 t6 | LIFE AFTER 18 ≥14 → none; "Ichorclaw gets nothing for blocking" | Myr blocks Wolf | **COMPREHENSION FAILURE** — claimed blocking Myr "becomes blocked, gets +2/+2"; option line itself said "your blocker dies" |
| A | vs158 | seq14 t9 | "If a Cast option is listed … do not recount your lands" | Cast nothing with {G}{B} up and two {2} casts offered | **COMPREHENSION FAILURE** — decided colored mana can't pay generic {2}; skipped a development turn |
| A | vs36 | seq13 t7 | LIFE AFTER 10 ≤13 → BLOCK NOW | no blockers vs 2 static Cathodions | **JUSTIFIED AUDIBLE** (borderline) — race logic sound, non-growing attackers, won t12; but licensed itself via the generic hint line |
| B | — | — | — | — | **no divergences found** (118 decisions) |

## Prompt-caused defects / wave-36 docket

No decision was caused by a render/annotation error — both failures above are model rules-priors overriding correct prompt text. Two composed-prompt seams worth the docket:

1. **Generic blockers hint contradicts the deck guide's numeric rule.** The B-line's suffix "NOT lethal: block only where the trade favors you; taking damage while ahead is often correct" is a standing counter-instruction to deck105's "≤13 → BLOCK NOW". The 35B cited it verbatim to override the guide (A-36 seq13; leaned on it in A-139 seq12). When the deck guide carries its own block calculus, the generic suffix should yield (or be suppressed for decks that override it).
2. **Mana line invites recounting.** "Mana available: 2 total (… colours you can make: {g}{b})" led the 35B to conclude {G}{B} cannot pay {2} (A-158 seq14) despite the guide's pre-emption. A one-clause hardening on the mana line ("any colour pays generic") or a "payable now" tag on cast options would close this class.
3. **Positive control**: the "the only legal targets are YOUR OWN right now" annotation on Relic Putrescence worked 5/5 (A-116) — the annotate-at-the-option pattern is the model that works.

## Per-arm verdicts and the A/B answer

**Arm B (122B): exemplary.** 118/118 decisions guide-adherent or guide-silent-reasonable; zero divergences, zero misreads. Its two hardest block calls were resolved by explicitly executing the guide's own arithmetic (17−3=14 → none; LETHAL → block and survive), and every named card rule (Putrefax same-turn, Hand trigger at opponent, Necropede at theirs, Corpse Cur return) was executed on sight. This is what the guide looks like when followed.

**Arm A (35B): strong but not clean.** 128/132 adherent (~97%); 4 divergences = 2 justified audibles + 2 comprehension failures. The audibles are genuinely good — the trample-chump override is exactly the "good choice to diverge" the owner's rubric rewards. The failures are the worry: both override explicit, correct text (a card-rules note printed in the guide AND on the option line; the engine's payability guarantee) from faulty MTG priors. That failure class is prompt-resistant — the text it ignored was already there.

**A/B for this seat: the 35B is not yet a safe replacement for the 122B.** Adherence is close (97% vs 100%) and its divergence *quality* when auditing is actually good, but the 122B made zero rules errors while the 35B made two, one of which (mana recounting) directly defies the guide's designed guardrail. If the two docket hardenings land and a re-run shows the failure class gone, the 35B becomes acceptable here; on today's evidence the 122B holds the seat.
