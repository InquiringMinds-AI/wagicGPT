# Wave-24 synthesis notes — TENTH per-deck-rotation cycle

Unified from seven independent deck-agent reviews mining the wave-24 PRIMARY corpus
`matchups-20260724-125739` (binary `/tmp/wagic-d7ff8b6c4` = step-1 batch `5f78e2934` + the FIRST-EVER
**PREGAME MULLIGAN PHASE** `86df3b482` + the **N9 retracted_choice hotfix** `d7ff8b6c4`). Pool = 6 stayers
(27 UB Zombies / 102 mono-B Tergrid / 109 mono-R Aggro / 133 mono-B Devotion / 131 UR Spellslinger / 137 GW
Adventures) + 1 rotated-in (**59 WB Persistence, GUIDELESS Step-0-bis**; 140 rotated out to canary). Cross-wave
record comparison is WEAK (pool changed) — judge decisions, not the win column.

⚠ The FIRST run this wave (`matchups-20260724-073032-N9B-OVERFIRE`) is **QUARANTINED** and was NOT read as
decision evidence at any seat (the N9 branch (b) over-fired 131 retracted_choice on hypothetical rules prose;
the hotfix retired branch (b) — retraction now = second-contradictory-coded-index ONLY).

## Corpus health

Integrity: 21 games, **2 timeout-DRAWS (first timeouts in 9 corpora — 27v137 and 102v133, both ~turn 10 at the
2400s cap, even life)**, 7 cap-adjudications. Latency median **17.3s / p90 73s**. Win table (context-only):
109:5/6, 133:4/6, 27:3/6, 59:3/6, 102:2/6, 137:2/6, 131:0/6.

Fallbacks **22/1293 = 1.7% raw**, but 19 are deck137's by-design convoke tap-payment defers
(`deferred_to_heuristic`, harmless). **TRUE model-fallbacks = 3/1293 (0.2% — the cleanest corpus yet):**
- deck133 vs131 s21 `retracted_choice` — the corpus's ONLY retraction; the N9 hotfix's live acceptance test →
  **RESIDUAL FALSE POSITIVE** (the protocol-example leak, reconciliation a).
- deck102 vs133 s5 `unparsed_reply` — out-of-range `CHOICE: 2` hallucination → benign, heuristic passed
  (the protocol-example leak's sibling, reconciliation a).
- deck137 vs102 s4 `stale_echo` — an in-range `CHOICE: 2 (Tap Temple Garden)` downgraded on a qualifier
  collision → FALSE POSITIVE, negative-harm (heuristic paid 2 life, enabling turn-1; ledger R-STALE-ECHO-QUALIFIER).

**HOTFIX VALIDATED (mechanism):** retracted_choice 131→1, and that 1 is a residual FP whose exact trigger is the
model quoting the prompt's own `e.g.` example (below). The Yawgmoth unpayable-sac class is GONE (deck133: 37
sac-cost offers, all at creatures≥2, 0 unpayable; the proliferate-cost mode correctly STILL offered at creatures:1
— surgical). Answer-locked retry fired **0×** — CORRECT (the decode-collapse class did not recur; verified deck27/
deck109/deck131).

**Pilot repetition_penalty=1.05 (2nd corpus): KEEP.** SAME at every seat — no prose degradation, no degenerate
n-gram repetition, tails lean or shorter than wave-23 (deck133 p95 6206<8276; deck131 p95 5556<6078). It neither
cures nor worsens spirals: deck137's worst spiral RELOCATED from the (now-floored) attackers seam to the blockers
seam (13.6k chars) — a GUIDE/representation commit-rule cures spirals the sampler cannot. The pilot verdict is a
decode/sampler output, not a skill or core line.

**Core (general prompt): PASS, 19th consecutive** (7-of-7 seats route core to PASS — see task-2 note below).

## Per-seat verdict table (win column is NOT a verdict)

| deck | archetype / status | record (context-only) | true fallbacks | pilot | guide disposition | rotation |
|------|--------------------|-----------------------|----------------|-------|-------------------|----------|
| 109 | mono-R Aggro · revised round 3 | 5/6 | 0 | SAME | **BYTE-COPY** (frozen; cmp-verified) | **KEEP ONE MORE** (Pillar 2-corpora clock #1 passed clean; #2 needed + marginal-mull untested) |
| 133 | mono-B Devotion · guided | 4W/1L/1D | 1 (retracted_choice FP) | SAME | **BYTE-FROZEN** (cmp-verified) | STAY (unique retracted_choice witness; Bloodghast add unexercised) |
| 27  | UB Zombies aggro · guided round 5 | 3/6 (+1 draw) | 0 | SAME (verbosity, not collapse) | **REVISED** (+bottom-phase +first-strike) | STAY (just-revised; harmful commit-lock; wither-FS clause; durdle 5th-corpus unexercised) |
| 59  | WB Persistence · GUIDELESS Step-0-bis | 3/6 | 0 (1 benign prose-parse) | SAME | **NEW initial guide** | STAY (newcomer never rotates on pass 1) |
| 102 | mono-B Tergrid control/steal · revised round 4 | 2/6 (+1 draw) | 1 (out-of-range) | SAME | **BYTE-COPY** (cmp-verified) | STAY (keystone validation debt — steal-take + R-DFC-BACKFACE both unproven; run the probe) |
| 137 | GW Adventures go-wide · guided round 2 | 2/6 (+1 draw) | 0 (19 by-design defers; 1 stale_echo FP) | SAME (spiral relocated) | **REVISED** (crutch demote + blocking rule) — ⚠ **SELF-DEPLOYED** | STAY (initial-guide round 2; fresh new-work) |
| 131 | UR Spellslinger burn · guided | 0/6 | 0 (cleanest deck131 corpus) | SAME | **REVISED** (+bottom-priority line) | STAY (construction-TERMINAL — flag to owner; guard partial; I-2 new) |

Record notes: **deck131's 0/6 is a CONSTRUCTION TERMINUS, not a piloting failure** (zero fallbacks, mulligan gate
6/6 correct, Guttersnipe guard working in its origin matchup — threat density 6/60, no reach vs lifegain, no early
defense vs aggro; no guide edit flips any of the six). **deck27's 4/6→3/6 dip is variance/pool** (guide byte-frozen
this cycle; vs109 draw-flip, vs102 tightening) — do NOT read it as a regression. **deck109's 5/6** is disciplined
execution of the trimmed guide with the restored Pillar line load-bearing; its one loss is the documented
drain/lifegain ceiling, no misplay.

## Step-1 batch + pregame-phase item validation — CLOSE / OPEN

- **(a) SacrificeCost candidate-existence (Yawgmoth unpayable offers) — CLOSE (deck133).** 37 sac-cost offers, all
  creatures≥2, 0 unpayable; the proliferate discard-cost mode correctly still offered at creatures:1 (surgical, not a
  blanket suppression). The wave-23 legality-oracle invariant violation (ledger #1) is CLOSED.
- **(b) mulligan-window fix — SUPERSEDED (historical only).** Replaced hours later by the pregame phase; the wave-23
  deck131 missing-mulligan gap (I-2) is RESOLVED by the phase (deck131 vs133 now renders an ask).
- **(c) R-ADV-FACE adventure-cost effect text — CLOSE-BY-FACT (deck137).** Every adventure-cost option now renders
  `{adventure spell: <effect>}` with no wrong (P/T); the Usher self-bounce misfire recurred 0×; the interim P/T-TRAP
  crutch was DEMOTED. Mark the wave-23 FIFTH idiosyncrasy-face WATCH resolved-at-representation, NOT promoted.
- **(d) N9 discriminator hotfix — mechanism VALIDATED (131→1); the residual is OPEN.** The 1 remaining
  retracted_choice (deck133 s21) is a FALSE POSITIVE from the protocol-example leak (reconciliation a) → OPEN as
  ledger #1.
- **(e) ANNOUNCE_X cap header — UNEXERCISED this corpus.** No X-menu deck was materially exercised (deck131/109 held
  or faced Pillar/Searing, no Rakdos-X/BSZ-X windows). Carry forward; no seat evidence either way.
- **(f) decode-collapse ANSWER-LOCKED RETRY — 0 fires, CORRECT.** The collapse class did not recur (deck27/109/131
  confirm: no reply carried the 1354×-repeat/U+FFFD signature; biggest replies 86-95% unique lines). 0 is the right
  outcome, not a missed detector. **Residual: the retry does NOT catch the commit-lock (a wrong-but-parseable snap) —
  the now-dominant residual — which a last-line parser preference does (reconciliation b).**

- **PREGAME PHASE VERDICT (first fleet outing) — MECHANICS CLOSE at all 7 seats; DECISION-QUALITY OPEN (3 findings).**
  Mechanically sound everywhere: no double-asks, London bottom-N math correct (= mulligans taken), full-7 redraw,
  first-draw integrity (starting player skips turn-1 draw), the wave-23 missing-mulligan gap resolved, leyline 103.6
  window correctly silent (no leyline cards in pool). One known cosmetic (harmless): the "opening hand" narration can
  reflect the pre-mulligan deal; the CURRENT-SITUATION hand line is correct. **OPEN — three decision-quality findings,
  routed by lane:** (1) deck102 vs59 hand-misread mulligan (mulliganed a keepable 2-Swamp hand claiming "zero Swamps")
  → MODEL-attention, reviewer-diff lane (headline 5a); (2) deck59 vs109 color-anxiety mulligan (mulliganed a keepable
  all-Plains 7, LOST the game) → GUIDE lane (the belief-that-VETOES corrective fact fixes the mulligan too); (3) deck27
  vs137 colorstuck BOTTOM (bottomed its only blue source, harmful, fed a timeout) → GUIDE lane (bottom-phase section
  shipped) AND PARSER lane (the commit-lock last-line question). Bottom/mulligan are arithmetic-heavy SNAP decisions
  and answer-first is unreliable for them — guide-lane and parser-lane are complementary (see reconciliation c).

## Cross-seat reconciliations

**(a) The PROTOCOL-EXAMPLE LEAK is 2-SEAT CONVERGENT and owns BOTH residual parser items.** The reply-protocol ends
`...e.g. "CHOICE: 2 (Cast Fatal Push)"`. Two seats independently latched onto it:
- **deck133 vs131 s21 (retracted_choice FP):** first-line `CHOICE: 1 (Cast Fatal Push)` re-affirmed throughout; the
  model agonized 225s over OUTPUT FORMAT (CHOICE-number vs target-name) and quoted the example `"CHOICE: 2 (Cast
  Fatal Push)"` 4×; the hotfix's second-coded-index scan matched the quoted example as a contradictory index, and
  because the reply truncated mid-spiral (token cap, no closing CHOICE line) found "no replacement" → deferred.
  Aggravator: the example's card name equals option 1's ("Cast Fatal Push"), maximizing the quote-latch.
- **deck102 vs133 s5 (out-of-range unparsed):** the ONLY coded line was `CHOICE: 2 (Cast Commander's Sphere)` — out
  of range (only 0/1 legal), pattern-filled from the example's literal index 2, compounded by a stale PLAN naming an
  unaffordable card; the model self-corrected to "choose 0 (pass)" in prose but never re-emitted a coded line.
- **ONE ledger item (ranked #1):** the retraction/index scan must count only **LINE-ANCHORED** coded indices and
  **EXCLUDE `CHOICE:` tokens inside quotation marks or that are verbatim echoes of the protocol's `e.g.` example**;
  and/or make the example index-agnostic (`CHOICE: <the number> (<action name>)`) and/or **ROTATE the example text
  per-ask**. Plus plan-hygiene (do not carry a PLAN whose named action is not in the current legal/affordable set as
  authoritative intent). Both fires non-deciding this corpus. Latent risk: where the heuristic's guess differs from
  the sustained coded intent, the deck133-style FP flips a correct decision to the heuristic.

**(b) The COMMIT-LOCK class is PROMOTED (2 corpora, 2 members, both harmful); the natural-stop last-line parser
proposal now has LIVE HARMFUL evidence — but reconcile it as a DESIGN question, not a settled change.** deck27 vs137
seq5 (pregame BOTTOM, NON-truncated): coded line 1 `PUT: 3, 5, 6` froze the wrong list (bottomed the only blue
source) while the reply reasoned to the correct `5, 6, 7` and STOPPED NATURALLY; answer-first threw away the correct
final line → colorstuck opener, fed the vs137 timeout. This is the OTHER member of the wave-23 decode-collapse/
commit-lock pair, independently re-hit with a harmful instance → the classification promotes to a durable rung
(skill Method headline 1). Supporting: deck102 s5 (the example-seeded out-of-range sub-shape) + deck131 vs27 s30
(I-2 answer-vs-conclusion contradiction — a CORRECT read + guide-quote overridden by an `ATTACK: A1` first line).
**The parser candidate SHARPENS:** the freeze is not only a truncation artifact, so a fix must key on "reply reached
a NATURAL STOP (did not hit max-tokens) → prefer the LAST coded line (the conclusion)," GATED so it never touches
truncated replies (where answer-first line-1 is the only safe floor). **This is the TOP parser DESIGN question for
wave-25** — it must reconcile with INDEX-WINS (a re-affirmed in-range first-line index should still win) and with
reconciliation (a)'s hazard (a quoted/example coded line must not become the spurious "last" index). Ranked #2.

**(c) Pregame decision-quality — synthesize across three lanes.** The three findings (b's colorstuck bottom, the
deck102/deck59 mulligans) split cleanly: GUIDE-LANE (bottom guidance rules — deck27's "keep color-fixing first, never
bottom your only color source, decide once on line 1" and deck131's Prism-Ring-lowest bottom-priority line — and the
belief-that-VETOES corrective that fixes deck59's mulligan) vs PARSER-LANE (b's last-line preference on natural-stop
replies, the structural fix for the arithmetic-snap-unreliable bottom) vs PROTOCOL-LANE (a's example leak, which
seeded deck102's phantom cast). The mulligan HAND-READ diff (deck102: never accept "no lands/no black/all bombs"
without counting the literal hand line) and the new-phase-zero-coverage audit are ADOPTED reviewer-method rungs
(skill headline 5). No general-prompt change is warranted — every finding routes below core.

**(d) Both timeout-DRAWS = INFERENCE-LATENCY SATURATION at -j3, not phase/engine stalls.** 27v137: combined decision
latency **2352s/2400s (98%)** — deck137 seat 1835s over 52 decisions (decision-DENSITY 2.4× the opponent: adventures
double cast options, convoke adds defers, go-wide multiplies choices; one 197s spike), deck27 517s over 23. 102v133:
combined **1939s/2400s (81%)** — a mono-B attrition mirror with many pass-priority windows (18 priority decisions at
deck102's seat vs 6 in its aggressive vs27 win), each a full ~16s round-trip; no single decision consumed the cap,
both even-life → correct DRAW. **Harness rule (updated with numbers):** for decision-dense pools (adventures/convoke/
go-wide, or stalled attrition mirrors) raise WAGIC_GPT_TIMEOUT or drop to -j2, or accept cap-adjudication as expected.
A max-tokens/latency fast-cap per decision would also reclaim the runaway over-reasoning that produced the commit-lock.

**(e) Convoke defers (19) — LEAVE BY-DESIGN.** All 19 (`kind=defer`, empty prompt/reply, latency -1) are the
"which creatures to tap for convoke" sub-choice routed to baka by design; deck137 verified none stranded a wanted
attacker/blocker. Harmless; deck137 is the sole defer source. Do NOT over-engineer a "trust the convoke option" core
line on one convoke deck (the rung stays single-seat pending a second convoke deck).

**(f) New representation items** (all → ledger, none guide-fixable): R-SHOCKLAND-ETB-UNLABELED (deck137, sibling of
R-PAINLAND — shock "pay 2 life / tap" ETB menu has an empty subject); persist narration gaps (deck59 — persist
returns not narrated, block-outcome "(both die)" ignores the persist return); wither-under-first-strike annotation
clause (deck27 — "(wither shrinks it to 0/0)" is false when the first striker kills the withering wall before it
deals damage); R-STALE-ECHO-QUALIFIER (deck137 — INDEX-WINS residual on a qualifier-collision in-range index);
R-BLOCKERS-CONFLICTING-LINES (deck137 — flag-not-diagnosed, illegal first BLOCKS line + corrected later lines applied
partially).

## Core prompt: PASS (19th consecutive) — task-2 distinction documented

All 7 seats routed core to PASS; no general-prompt line added or cut. **Reconciliation (a) may argue for a PROTOCOL
change (rotating/removing the worked example) — that is the REPLY-PROTOCOL layer (code-appended text), NOT the core
prose.** The distinction matters: the core answer-first CHOICE-line contract HELD in all non-empty replies at both
affected seats (a parseable first-line CHOICE every time, including the s21/s5 spirals); the defect is that the
example STRING at the end of the appended reply protocol can be quote-latched or pattern-filled. Route the example
fix (index-agnostic / rotated / quote-excluded scan) to the **engine/reply-protocol ledger (item #1)**, not to core.
Core stays PASS at 19.

## WAVE-25 ENGINE LEDGER (ranked; seat notes.md carry the seqs)

1. **PROTOCOL-EXAMPLE LEAK + line-anchored index scan (deck133 + deck102, HIGH, reply-protocol/parser, cross-seat).**
   Owns BOTH residual parser items (deck133 s21 retracted_choice FP + deck102 s5 out-of-range). This is the N9
   hotfix's live acceptance test STILL failing. Fix: (i) retraction/index scan counts only line-anchored coded
   indices and excludes `CHOICE:` tokens inside quotes or verbatim `e.g.`-example echoes; (ii) make the reply-protocol
   example index-agnostic and/or rotate its text per-ask; (iii) plan-hygiene — do not carry a PLAN naming an
   illegal/unaffordable action as authoritative intent. Both fires non-deciding; latent-risk if the heuristic ever
   mismatches a sustained coded intent.
2. **NATURAL-STOP LAST-LINE parser design (deck27 harmful + deck131 I-2 + deck102 sub-shape, HIGH, parser DESIGN
   QUESTION).** The commit-lock class is promoted (2 corpora, both harmful). On a reply that reached a natural stop
   (did not hit max-tokens), when multiple coded lines of the same kind appear, prefer the LAST (the model's
   conclusion); GATE strictly on non-truncation (truncated replies keep answer-first line-1 as the only floor).
   Must reconcile with INDEX-WINS and with ledger #1's hazard (a quoted/example line must not become the spurious
   "last" index). Bottom/mulligan/attacker windows are the highest-value target (arithmetic-heavy, snap unreliable).
   Guide fixes shipped in parallel (deck27 bottom-phase, deck131 bottom-priority) — complementary, not either/or.
3. **Bottom/mulligan DECISION-QUALITY lane split (deck27 + deck131 guide; deck102 + deck59 model/guide, MED).** The
   pregame surface's decision-quality residual: guide-lane (bottom guidance shipped) + parser-lane (#2) + the belief-
   VETOES mulligan corrective (deck59). Watch recurrence now that every game runs the phase.
4. **R-STALE-ECHO-QUALIFIER — INDEX-WINS residual gap (deck137, MED, parser).** An in-range `CHOICE: N (…)` whose
   parenthetical carries a card-name qualifier that collides with a recent/stale sibling option downgrades to
   stale_echo (deck102 s4 `CHOICE: 2 (Tap Temple Garden)` vs stale "Play Temple Garden"). Prefer the in-range index N
   over the qualifier-collision. FP with negative-harm this time; bites where the fallback diverges.
5. **R-SHOCKLAND-ETB-UNLABELED (deck137, LOW-MED, representation, corpus-general).** The shock "pay 2 life / tap" ETB
   menu renders `Choose an option for :` (empty subject); label it `As <land> enters: pay 2 life (untapped) / do not
   pay (tapped)`. Sibling of R-PAINLAND; every dual/shockland in every deck hits it.
6. **R-BLOCKERS-CONFLICTING-LINES (deck137, MED, parser, flag-not-diagnosed).** deck59 s39: an illegal first
   `BLOCKS: B2:A1,A2,A3,A4` (one blocker onto four) + a corrected 2-block plan later in the reply; recorded outcome
   matched NEITHER (a single chump). The blockers parser's handling of a reply whose first BLOCKS line is illegal and
   whose later lines carry the corrected assignment warrants inspection. Repro: deck59 s39.
7. **wither-under-first-strike annotation clause (deck27, LOW, representation).** Suppress "(wither shrinks it to
   0/0)" when the attacker has first strike and is lethal-to-the-blocker (the wall dies before it deals its wither).
   Top-level verdict was right; the parenthetical is factually wrong.
8. **Persist narration gaps (deck59, LOW, representation).** Persist returns log only as generic
   `battlefield -> graveyard` then `stack -> battlefield` (no persist-trigger line, death→return not linked);
   block-outcome "(both die)" understates a persist blocker's chump/trade value (`(both die - yours persists back)`).
9. **Harness -j/-T guidance for decision-dense pools (deck137 + deck102/deck133, HARNESS).** Both timeouts are
   inference-latency saturation at -j3 (numbers in reconciliation d). For corpora including deck137 (or attrition
   mirrors) raise WAGIC_GPT_TIMEOUT or drop to -j2; a per-decision max-tokens/latency fast-cap also structurally
   closes the commit-lock over-reasoning.

**Carries (unchanged / un-re-exercised):**
- **R-DFC-BACKFACE-RESOLVES-FRONT — still OPEN (deck102).** Un-re-exercised this corpus (the Lantern back-face was
  never cast — the model reached the God's full mana and cast the God front face directly, rules-correct). The bug is
  neither resolved nor observed; keep OPEN.
- **deck102 STEAL-PROBE recommendation.** The take-the-steal line got 0 test (0 steal windows all corpus, Tergrid
  landed at the timeout cap). Run a **deck199 probe** stacking Tergrid + edict/discard vs an opponent with
  sacrificeable permanents on a curve that reliably reaches a Lantern-affordable/God-unaffordable turn — ONE
  controlled game validates BOTH the steal-take line AND R-DFC-BACKFACE at once.
- **N8 baka 0-power blocker gang-chump heuristic — UNEXERCISED (5th corpus).** Answer-first structurally prevents
  block fallbacks at answer-first seats; needs a probe deck that forces a block fallback with a valuable 0-power body.
- **ENGINE-R4 Azcanta ACTIVATION probe — orphaned (deck135 rotated out wave-22).** A model-elected top-4-to-hand
  transform pick remains unobserved; needs a control-deck probe.

## Rotation decisions — NOBODY ROTATES this cycle (each verified honestly)

Standing test: candidate for rotation OUT when the guide takes NO substantive modification AND no seat-specific
new-work signal survives.

- **109 — KEEP ONE MORE (convergence candidate, NOT yet).** Guide byte-copy (frozen, cmp-verified); behaviorally
  converged (5/6); engine/harness side closed. But the wave-23 belief-correcting-crutch doctrine set a **2-corpora
  clock** for the restored Pillar cost line, and this is corpus **#1** (passed cleanly WITH the trigger present in the
  exact regressing vs137 matchup — 14 windows, 0 conflations). Rotating now would repeat wave-22's declare-redundancy-
  on-one-corpus mistake. ALSO the marginal-aggressive-mulligan judgment was UNTESTED (all 6 hands trivial 3-land
  keeps — the surface-unexercised carry). Keep one more NON-canary cycle; if corpus #2 again shows 0 Pillar
  mis-costings with the trigger present AND the mulligan surface produces no anomaly, THEN rotate out as converged.
- **133 — STAY.** Guide byte-frozen, but new-work present: deck133 is the corpus's UNIQUE producer of
  retracted_choice and the N9 hotfix's live acceptance test is STILL failing here (the quoted-`e.g.`-example residual
  FP, ledger #1). The Bloodghast-fodder add is UNEXERCISED (no edict opponent at this seat — deck140 rotated out).
  Rotating out the sole witness of an unresolved parser FP class would blind the loop. Re-evaluate once ledger #1
  ships AND an edict opponent re-exercises the add.
- **102 — STAY (keystone validation DEBT; guide has CONVERGED).** Guide byte-copy; all tested lines held (flip-thrash
  0/6, Tergrid cast rules-correct, Smallpox disciplined, painland priced). But 2 of 3 wave-23 rotation conditions are
  UNVALIDATED (not failed): R-DFC-BACKFACE un-re-exercised, steal-take line got 0 windows. The deck's core payoff is
  unproven across two consecutive Tergrid-on-board waves. **Recommendation: run the deck199 steal-probe** (validates
  both open items at once); if it lands, deck102 is a STRONG wave-26 rotation candidate.
- **137 — STAY (initial-guide round 2) — ⚠ PROCESS DEVIATION FLAGGED (see deployment manifest).** Guide REVISED
  (R-ADV-FACE crutch demoted + blocking-under-lethal rule) + fresh new-work (blockers-seam spiral + 3 ledger items).
  One guided corpus is not convergence and the guide just changed. **The seat DEPLOYED its revised guide to live Res
  itself during review** (live `deck137_strategy.txt` mtime 17:05 today == the seat file, vs the integrator's 05:50
  deploy time on all other guides) — content is legitimate and byte-matches the seat file, but deployment from the
  review seat violates the loop's "integrator applies in step 4" rule. Flagged for the integrator to verify and note.
- **131 — STAY (construction-TERMINAL — flag to roster owner).** Guide REVISED (+bottom-priority line for the new
  phase) + new-work (guard PARTIALLY validated — held 4/4 in its origin vs27 including the exact wave-23 break spot,
  but a fresh tapped-misread appeared vs59 s23; and the NEW I-2 answer-vs-conclusion class). **The 0/6 is a
  CONSTRUCTION TERMINUS, not a piloting failure** — zero fallbacks, mulligan gate 6/6, Mountain-first clean; the deck
  cannot race the field (threat density 6/60, no reach vs lifegain, no early defense vs aggro). No guide edit changes
  the record; the lever is DECK CONSTRUCTION (owner's call — sideboard carries Spellheart Chimera reach, Aetherling
  finisher). Recorded for the owner, not relitigated.
- **27 — STAY.** Guide REVISED this wave (bottom-phase + first-strike caution, both from concrete harmful instances)
  → not stable-proven, needs a validation corpus. New-work: the harmful commit-lock (validates ledger #2), the
  wither-FS clause, and the durdle off-case is 5th-corpus UNEXERCISED. Re-flag: does the first-strike caution hold on
  the next first-striker window; does the bottom guidance make the snap right; parser last-line acceptance if it ships.
- **59 — STAY (newcomer never rotates on pass 1).** GUIDELESS Step-0-bis produced an initial guide (new-work by
  construction). Re-validate next corpus (Step-0-ter): did the hybrid/green-mana veto/tax collapse, and did the pilot
  start sweeping to stabilize when behind (the loss lever)?

**WAVE-25 POOL: 27, 102, 109, 133, 131, 137, 59 (UNCHANGED — no rotate-out, no rotate-in this cycle).** Nobody
discharged a keep-reason: 109 is on its 2-corpora clock; 133/102 hold open engine/keystone debts; 137/59 are early
guided rounds; 131 is construction-terminal with an unvalidated guard; 27 just revised. The pool exercises for
wave-25: ledger #1 (protocol-example leak) at deck133/deck102, ledger #2 (natural-stop last-line) at deck27/deck131,
the Pillar 2-corpora clock #2 at deck109, the deck199 steal-probe for deck102 (R-DFC-BACKFACE + steal-take), and the
Step-0-ter validation of deck59's initial guide. The roster's guided review seats are EXHAUSTED — the pool draws
GUIDELESS Step-0 newcomers only when a seat rotates out (none did).
