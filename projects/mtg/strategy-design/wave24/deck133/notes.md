# deck133 wave-24 — development notes (engine / harness / model / construction) + ROTATION VERDICT

Corpus PRIMARY `matchups-20260724-125739`, binary `d7ff8b6c4`, pilot repetition_penalty=1.05.
Real games: 131(W), 109(W), 137(W-adj), 59(W-adj), 27(L), 102(DRAW/timeout) — **4W/1L/1D.**
Layer-routing enforced: engine/harness/model/construction live HERE with seq repros; the guide
is BYTE-FROZEN (findings.md); win/result column is context-only.

## ENGINE — [RESOLVED — wave-23 item CLOSED] Yawgmoth "Sacrifice another creature" unpayable offer is GONE.
The wave-23 engine finding (vs137 s26: `-1/-1 Counter [cost: Life, Sacrifice]` offered with
only Yawgmoth controlled, cost UNPAYABLE, 85s spiral + fallback) is FIXED by the step-1
SacrificeCost candidate-existence gate. **Acceptance proof:** across all 6 deck133-seat logs,
**37** sacrifice-cost `-1/-1 Counter with Yawgmoth` offers rendered, **every one at
creatures>=2** (counts {2,3,4}); **ZERO at creatures:1.** The fix is SURGICAL — Yawgmoth's
`proliferate [cost: {b}{b}, Discard]` mode (no sacrifice required) was correctly STILL offered
at creatures:1 (vs27 s21, s25), so the gate keys on the SacrificeCost specifically, not on
"Yawgmoth with 1 creature." **Repro (for the record):** grep any deck133-seat log for
`-1/-1 Counter with Yawgmoth` and cross-check the prompt's `(creatures: N)` line — all N>=2.
No further action; close the item.

## HARNESS/PARSER — [OPEN, precise] N9-hotfix retracted_choice RESIDUAL FALSE POSITIVE: the second-coded-index scan matches the prompt's OWN quoted `e.g.` example.
**vs131 s21, ask, my17/opp16, latency 225,068ms. deck133 owns the corpus's ONLY retracted_choice
= the hotfix's live acceptance test. Verdict: RESIDUAL FP.**
- The reply's FIRST line is a clean coded answer `CHOICE: 1 (Cast Fatal Push)`, re-affirmed
  repeatedly ("So I must say CHOICE: 1"; "I will stick to the format CHOICE: 1 (Cast Fatal
  Push)"). The DECISION never changed — the model spiraled 225s over OUTPUT FORMAT (CHOICE
  number vs target name) and target selection.
- stderr `game-133v131` line 638: `AIPlayerGPT: ask CHOICE 1 retracted with no replacement;
  deferring`. The reply has ~30 `CHOICE:` tokens; **all-but-the-first are inside quotation
  marks (prose).** The only contradicting NUMERIC coded index is the prompt's own worked
  example quoted verbatim x4: `"CHOICE: 2 (Cast Fatal Push)"` (the prompt ends `...e.g. "CHOICE:
  2 (Cast Fatal Push)"`). The scan matched that as a second contradictory index vs first-line
  `CHOICE: 1`; the reply truncated mid-spiral (token cap, no closing CHOICE line) -> "no
  replacement" -> defer. Aggravator: the example's card name ("Cast Fatal Push") equals option
  1's card, so the model quoted it obsessively. `"CHOICE: Guttersnipe"` (also quoted) is
  non-numeric and does not trigger.
- **Non-deciding:** heuristic cast Fatal Push (shouldPlay=90); the model then answered the
  SEPARATE target sub-prompt with Guttersnipe (chose 3 of 3) = exact intent; deck133 won 7/-4.
- **Fix for the parser owner:** (1) count only **line-anchored** coded CHOICE indices for the
  retraction scan; (2) **exclude `CHOICE:` tokens inside quotation marks OR that are verbatim
  echoes of the prompt's `e.g.` example**; (3) a re-affirmed first-line index never contradicted
  by another *line-start* index wins under INDEX-WINS. LATENT RISK unchanged: in a window where
  the heuristic's guess differs from the sustained first-line intent, this FP flips a correct
  decision to the heuristic.

## HARNESS/PROMPT — [OPEN, the ROOT of the s21 spiral] cast-with-target decisions mix two output contracts.
The s21 225s spiral's root cause: the CAST-step prompt asks for `CHOICE: <number>`, but option 1
was `Cast Fatal Push - legal targets right now: Bloodghast, Geralf's Messenger, Guttersnipe`, and
the model (having seen target-choice steps elsewhere that demand "answer with the chosen TARGET's
name") could not resolve WHICH contract applied to a cast-that-has-targets. It burned 15,383 chars
oscillating between `CHOICE: 1` and `CHOICE: Guttersnipe`. The engine actually handles this as TWO
steps (cast -> then a separate TARGET CHOICE), so the cast step should NOT surface target names in
a way that invites target-name output. **Fix candidate:** on the cast step, either drop the
inline "legal targets right now: ..." target list from the option label (targets are chosen in the
following step), or add one clause to the cast-step instruction: "choose the CARD by number here;
you will pick its target on the next step." LOW/MED; cross-seat (any cast-with-target binary).
Related wave-23 carried class (nameless `Put in Play` labels) is quieter this corpus but same
family — the option label under-specifies and the model over-reasons to recover it.

## MODEL/LATENCY — [note-only] the 102v133 TIMEOUT-DRAW is a SLOW GRIND, LLM-latency-bound — NOT a phase/engine stall. (owned decompose)
**102 vs 133, adjudicated DRAW at the 2400s wall-clock cap, turn 11, even life (~18/18).**
- **Combined LLM decision latency = 1939s (deck133 1233s + deck102 706s) = ~81% of the 2400s
  cap.** deck133 side = 39 decisions/11 turns dominated by 9 long SPIRAL decisions
  (175/101/98/90/79/53/52/52/42s ~= 740s). deck102 side = 34 decisions/706s (2 spirals). The
  wall-clock cap is a TIME cap; slow LLM decisions hit it at a LOW turn count.
- **NOT an engine/phase stall:** 196 normal `NextGamePhase` transitions (turn 0->11), only
  cosmetic cache "errors" (`AttemptNew failed to load` / missing `overload.wav`), no stuck loop,
  no empty-reply storm. Pregame consumed normal time (first decision 16.5s).
- **Board reality:** two attrition decks (deck133 devotion-midrange vs deck102 Tergrid control),
  neither with a fast clock. deck133 never assembled a lethal Gray Merchant; opp had answers;
  life stayed 20/20 -> 18/18. At the cap, EVEN life -> a fair DRAW (if anything deck133 had
  marginally more board with Yawgmoth online, but no reach to convert). Note-only.
- Corpus context: the SISTER timeout (27v137) is not at this seat; both are the first timeouts
  in 9 corpora and both are even-life grinds, not engine failures.

## HARNESS — [note-only] pregame mulligan phase, first outing at this seat: clean, one cosmetic.
One Keep/Mulligan flow per game, no double-asks. vs27 ran the full London path (Mulligan -> new
7 -> Keep -> Bottom-1); bottom-N=1 correct; bottomed the redundant duplicate Phyrexian
Obliterator. Starting player skips turn-1 draw (CR-conformant, verified vs131). No leyline card
at this seat (103.6 untestable here). COSMETIC (matches brief item 5): vs27 s2 the model
re-derived its post-mulligan hand from the log because the "opening hand" narration reflects the
pre-mulligan deal — resolved correctly, cost only tokens.

## CONSTRUCTION (upstream decklist flags — RE-CONFIRMED; do NOT fix with guide length)
- **No reach / no flyer answer except Gray Merchant + Liliana -2.** vs27 LOSS: opp life FLAT 20
  the entire game, deck133 dealt ZERO damage; no castable GM, no wall. Same structural face as
  wave-9..23. Land-light greedy mulligan keep (2 fetches, top-heavy 4-drops) plausibly
  compounded but is not the root.
- **No early blocker; Bloodghast cannot block.** Carried.
- **Self-damage suite (Thoughtseize 2, Arena 1/turn, fetch 1 each).** Did not decide a loss this
  corpus. Carried, quiet.

## GUIDE EDIT LEDGER (this wave — NO CHANGES; BYTE-FROZEN)
- **strategy.txt BYTE-FROZEN** — sha256 `6ed73f00...39ce` identical across deployed / wave-23 /
  wave-24 (cmp-verified). No new leak surfaced.
- **KEPT / validated:** Liliana of the Veil +1 caution — VALIDATED across 10 windows, zero
  anti-pattern (findings.md 3). Bloodghast forced-sacrifice fodder — UNEXERCISED (no edict at
  this seat; deck140 the Cruel-Edict deck rotated out) but correct + cheap; KEEP (findings.md 4).
- **NO new rule added:** self-FP, Gray Merchant (2 stabilize casts, guide-perfect), Yawgmoth,
  mulligan all handled cleanly by existing guide + engine surfaces; the only fallback is a
  harness-parser FP (above), not a deck/guide gap.

## ROTATION VERDICT — STAY (NOT a rotation candidate this cycle).
The standing test: "no-guide-mod AND no-new-work-signal = candidate" (for rotation OUT).
- **Prong 1 (no-guide-mod): TRUE** — guide byte-frozen this cycle.
- **Prong 2 (no-new-work-signal): FALSE** — there IS a live new-work signal at this seat:
  deck133 is the corpus's **UNIQUE producer of the retracted_choice class**, and the N9 hotfix's
  live acceptance test is **still failing here** (the quoted-`e.g.`-example residual FP, a
  precise unresolved harness/parser finding with a proposed discriminator refinement).
  Additionally the Bloodghast-fodder add remains UNEXERCISED (wants a corpus with an edict deck
  to validate round 2).
Because BOTH prongs are required for candidacy and prong 2 fails, deck133 is **NOT a candidate ->
STAY.** Rationale: rotating out the sole witness of an unresolved harness FP class would blind
the loop to it. Re-evaluate once the parser discriminator ships (excludes quoted/example CHOICE
tokens) AND an edict-carrying opponent has re-exercised the Bloodghast-fodder add — at which
point, with the guide still frozen and both items closed, deck133 would become a clean rotation
candidate.
