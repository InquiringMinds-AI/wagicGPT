# Wave-25 synthesis notes -- ELEVENTH per-deck-rotation cycle

Unified from seven independent deck-agent reviews mining the wave-25 PRIMARY corpus
`matchups-20260725-035605` (binary `/tmp/wagic-62d8783a5` = wave-25 step-1 batch `62d8783a5` on
top of the RULES-FIX batch `f0500ad66`, -T 3000, REPPENALTY=1.05 standing). Pool = 7:
27 UB Wither / 102 BW edicts+Tergrid / 109 mono-R Aggro / 133 mono-B Devotion / 131 UR
Guttersnipe burn / 137 GW convoke+Adventures / 59 WB Persistence (its FIRST GUIDED corpus).
Cross-wave record comparison is context-only -- judge decisions, not the win column.

## THE CORPUS-LEVEL STORY: the parser stack CONVERGED, both batches VALIDATED

**Zero true model-fallbacks across 1,299 decisions** -- 0 unparsed, 0 empty, 0 retracted_choice,
0 stale_echo. The only 15 fallbacks are deck137's by-design convoke tap-payment defers
(harmless). This is the cleanest corpus in the campaign's history and the one on which the
parser/reply-protocol stack demonstrably converged. Reply lengths p50 1088 / p95 7997 / max 15217
(long deliberations persist but nothing breaks); the answer-locked retry fired 0x (correct -- the
decode-collapse class did not recur).

**Both live validation batches passed with NO regression at any of the 7 seats:**
- **Batch A (rules-fix `f0500ad66`)** -- the first corpus on the rules-fixed engine. a1 turn-1
  untap/real-upkeep, a2 no untap/cleanup priority, a3 combat-role clearing, a4 pw-redirect menu
  REMOVED, a5 creature-Battle combat exclusion, a6 designation-marker suite exclusion. Verified
  clean everywhere: a4 confirmed 0 pw-redirect records corpus-wide (deck102 + deck109 -- burn/
  drain now always hit the player when aimed there, no phantom redirect menu); no turn-1 narration
  anomaly, no expectation of the old redirect menu observed at any seat.
- **Batch B (wave-25 step-1 `62d8783a5`)** -- b1 protocol-example de-fang, b2 NATURAL-STOP
  precedence, b3 echo-qualifier containment trust, b4 shockland ETB annotations, b5 wither-FS
  ordering, b6 persist narration + trade-return clauses. Every item landed (details in the
  batch-item ledger below). b2 was LIVE-VALIDATED and game-saving (deck27 vs109 s11); b1 zeroed
  both wave-24 residual parser FPs; b6 was decision-effective (deck59, 11/12 blocks cite the
  return).

REPPENALTY=1.05 (3rd corpus): KEEP -- same at every seat, no prose degradation.

## Corpus health

Integrity: 21 games, 1 timeout-DRAW + 4 cap-adjudications (10 no-gameend logs = exactly those 5
games). The timeout-DRAW traced to deck133's decision density (51 calls / 2,146s), NOT deck137
(31 calls / 700s) -- the -T 3000 bump (from 2400s) VALIDATED, buying ~+4 turns; deck137 no longer
saturates. Win table (context-only): 59:6/6 (FIRST GUIDED, from 3/6 Step-0), 109:5/6, 102:4/6,
27:2/6, 131:1/6, 133:1/6, 137:1/6.

## Per-seat verdict table (win column is NOT a verdict)

| deck | archetype / status | record | batch A result | batch B result | rotation | guide changed |
|------|--------------------|--------|----------------|----------------|----------|---------------|
| 59  | WB Persistence -- FIRST GUIDED | 6/6 | no-impact (no pw/battle/day-night; a1 upkeep no-op) | b6 persist narration + trade-return LIVE, cited at 11/12 blocks (CLOSED-BY-FACT) | **ROTATE OUT** (converged; only open item engine-lane) | N (BYTE-COPY, validated as-is) |
| 109 | mono-R Aggro -- revised round 3 | 5/6 | a4 burn always face, 0 redirect records | b2 prose-reversal parses answer-first (correct per spec; convergence watch) | **ROTATE OUT** (Pillar 2-corpora clock CLOSED clean, 4/4 windows 0 conflations) | N (BYTE-IDENTICAL, md5 a8f163f7) |
| 102 | BW edicts+Tergrid -- revised round 4 | 4/6 | a4 pw-redirect verified 0 records corpus-wide | b1 de-fang -> 0 fallbacks; example-seeded sub-shape RETIRED to historical | **PROBE-THEN-ROTATE** (steal + R-DFC-BACKFACE still unproven; run deck199 probe, then rotate) | Y (1 edit: "early = by turn 2") |
| 27  | UB Wither -- guided round 5 | 2/6 | colorstuck-bottom class GONE | b2 natural-stop LIVE-RESCUED vs109 s11; b5 wither-FS closed both directions | **STAY** (guide modified; needs validation corpus) | Y (4 surgical edits) |
| 131 | UR Guttersnipe burn -- guided | 1/6 (up from 0/6) | a1 turn-1 upkeep noted (no earlier-firing anomaly) | b2 s37 converged correct | **STAY** (near-terminal; projected rotation wave-26 if edits validate) | Y (2 rules-correctness edits) |
| 133 | mono-B Devotion -- guided | 1/6 (down from 4/6) | rules-fix clean at seat | b1 example-leak closure GONE but WEAK-by-construction | **STAY** (close to rotating; 1/6 = threat-drought + pool-rise, not decision collapse) | Y (1 edit: line 126 Arena) |
| 137 | GW convoke+Adventures -- guided round 2 | 1/6 (+timeout-draw) | a6 designation markers excluded | b4 shockland annotation VALIDATED (residual cosmetic); b3 holding (0 stale_echo); b2 safety-net | **STAY** (blockers-seam floor landed ~10x collapse) | Y (Lovestruck adventure-body fix + ROLE CHECK) |

Record notes: **deck59's 6/6 is EARNED, not a hidden-tax record** -- both idiosyncrasy levers fire
correctly (see reconciliation A); the residual tax RELOCATED to an engine-lane render ambiguity.
**deck133's 4/6 -> 1/6 is threat-drought + pool-rise, NOT decision collapse** (Gray Merchant +
Obliterator offered 0 times in all three flipped losses; zero fallbacks; Liliana round 3 clean).
**deck131's 0/6 -> 1/6 confirms the construction-terminal flag STANDS** (5 losses = known deck
limits; vs102 lost by ONE life) -- projected rotation at wave-26 IF the 2 rules-correctness edits
validate. **deck27's 2/6** is variance/pool (guide just revised; needs a validation corpus).

## Batch-item close/open

- **a4 pw-redirect removal -- CLOSE (deck102 + deck109).** 0 redirect-menu records corpus-wide;
  player-aimed burn/drain hits the player. No decision shifts, no model expecting the old menu.
- **b1 protocol-example de-fang -- CLOSE-BY-CONSTRUCTION, WEAK (deck133 + deck102).** 0
  retracted_choice AND 0 out-of-range corpus-wide (both wave-24 residual FPs gone). BUT the
  closure is weak-by-construction: the de-fang removed the trigger that produces the very shape
  needed to strong-confirm it (reconciliation C). deck133 recommends shipping the wave-24 parser
  hardening as belt-and-suspenders (ledger #4).
- **b2 NATURAL-STOP precedence -- CLOSE (LIVE-POSITIVE, deck27 + deck137).** deck27 vs109 s11: a
  natural-stop blockers reply reversed `BLOCKS: none` (lethal) -> `BLOCKS: B1:A2` (survival) and
  the engine applied the FINAL line -> deck27 survived. deck137: 4 more replies converged on a
  better final choice the parser correctly consumed (safety net for the relocated generic-seam
  spirals). The wave-24 commit-lock parser design question is resolved as a shipped fix.
- **b3 echo-qualifier containment -- HOLDING (deck137).** 0 stale_echo; the wave-24 s4
  Tap-collision is absent-by-behavior (noted, not refuted).
- **b4 shockland ETB annotation -- VALIDATED with a RESIDUAL (deck137).** The card-text annotation
  landed; residual = the standalone ETB menu still renders bare `pay 2 life`/`tap` with
  `Choose an option for :` (empty subject) -- downgraded to a cosmetic ledger item (#5).
- **b5 wither-vs-first-strike ordering -- CLOSE-BY-FACT both directions (deck27).** First-strike
  case omits the shrink clause (vs109 s9/s11); non-FS case includes it correctly (vs137 s12).
- **b6 persist narration + trade-return -- CLOSE-BY-FACT, DECISION-EFFECTIVE (deck59).** Return
  narration + trade-return clauses render; 11/12 block decisions cite the return and the citation
  drove the block (reconciliation A).
- **-T 3000 bump -- VALIDATED.** The one timeout-DRAW is deck133 density, not deck137; +4 turns.

## Cross-seat reconciliations

**(A) deck59's 6/6 = both levers validated + the crutch-lifecycle matched pair completed.** Rule #1
(hybrid mana simplification) killed BOTH the cast-tax AND the mulligan-anxiety in one corpus: zero
false vetoes, zero color-anxiety mulligans, ask-giants >4k 33->8 (the wave-24 color-anxiety
mulligan LOSS did not recur). Rule #2 (persist makes your sweeper one-sided) validated BOTH
directions: it swept at myL2 facing lethal (vs27 s14 -- the exact wave-24 loss spot -- reproducing
the recursion WHY nearly verbatim, ground back from 2 life and WON) AND correctly HELD the sweeper
while ahead (vs109, the wave-24 LOSS matchup now a dominant win). b6 shipped the persist FACT and
the model reads it AT THE BLOCK (11/12 blocks cite the return). The 6/6 is earned. Feeds skill
Method headline 3 (mana-fact cross-phase) + the rule-3 sixth-face VALIDATION-CONFIRMED carry.

**(B) b2 NATURAL-STOP is now a proven mechanism, and flooring + b2 are complementary (deck27 +
deck137).** deck27 vs109 s11 is the game-saving live-positive the wave-24 synthesis called for;
deck137 shows flooring reduces spiral LENGTH (latency) while b2 protects spiral CORRECTNESS (the
final answer) -- so the residual generic-seam over-deliberation is a latency concern, not a
correctness one (reconciliation D). Feeds skill Method headlines 1 + 5.

**(C) the b1 leak closure is WEAK-BY-CONSTRUCTION -- a general reviewer-method refinement (deck133 +
deck102).** The de-fang removed the aggravator (example-name = live-option) that CAUSED the model
to quote CHOICE at all, so shape-absence is the CEILING of obtainable evidence, not weak-but-
improvable. Label such trigger-removal fixes "weak-by-construction / ceiling reached," ship a
belt-and-suspenders mechanism-layer guard (the wave-24 line-anchored index scan), and do NOT read
future absence as confirmation OR failure-to-look. Feeds skill Method headline 2 + ledger #4.

**(D) the spiral RELOCATED, it did not vanish (deck137 3-data-point causal chain + deck59).** Per-
seat floors collapse high-stakes-seam spirals ~10x (deck137 blockers max now 1,328 ch, zero
illegal) but the worst spiral MIGRATES to ubiquitous low-stakes seams (land drops, priority
passes, single-option casts, reveals) that no per-deck guide can reach. deck59 independently shows
the same: its mana spiral collapsed and the residual >6k giants moved to Restless Apparition
pump-timing (an engine-lane render ambiguity, ledger #2) and tight-game combat math. Doctrine
(skill Method headline 5): floor the named high-stakes seams, then STOP -- the rest is latency, a
MODEL property harmless to correctness, and a general commit-briefly directive would be a CORE
touch, never a per-deck line.

**(E) the CORRECT-DEVIATION and HAND-EVALUATION routing rungs converge on one principle (deck131 +
deck102).** deck131's new rung: a CORRECT play that VIOLATES a guide absolute flags the GUIDE
(audit the miscalibrated absolute, add the exception) -- the inverse of the wave-24 wrong-play
diffs. deck102's pregame split: a decision that fails while the guide's own keep criteria were
SATISFIED points at the guide's AMBIGUITY (resolve the fuzzy term -- "early = by turn 2"), while a
failure from a MISREAD of the inputs points at the MODEL (route to notes). Both share the tell:
was the play/read game-legal and rules-correct? If the model was RIGHT and the outcome/guide
disagree, suspect the GUIDE; if the model MISREAD the inputs, suspect the MODEL. Feeds skill
Method headline 4 + the FIDELITY-vs-JUDGMENT carry.

**(F) the HARNESS-BLOCKED-COMPLIANCE reviewer check (deck109, single-seat).** deck109 vs131 s10/s11
looked like the model ignoring a guide-mandated on-sight kill (Young Pyromancer), but reading the
reply reversed both verdicts: s10 concluded the kill in prose (swallowed by a parse mismatch), s11
declined only because a TRUNCATED target preview hid Pyromancer. The misplay was harness-caused
(rendering + parsing), not model or guide -> route to notes, never the guide. This is the reviewer-
discipline face of engine ledger #1 (R-TRUNCATED-TARGET-PREVIEW). Feeds skill Method headline 6.

## Core prompt: PASS (20th consecutive) -- 7-of-7 seats

All seven seats routed core to PASS; no general-prompt line added or cut. Two below-bar candidates
were flagged for a convergence check and stay in the LEDGER (not the prompt): affirmative
"[untapped - can block]" tagging (deck131, pending convergence) and prose-only answer reversal
without a re-emitted coded CHOICE line (deck109, a b2-extension WATCH -- correct per the current
spec). Break the 20-PASS streak only on convergence. (Note: reviewers and lineage number this the
20th consecutive PASS -- wave-24 was the 19th; the task brief's "21st" in deliverable 5 is a
miscount, reconciled here to 20th.)

## WAVE-26 ENGINE LEDGER (ranked; seat notes.md carry the seqs)

1. **R-TRUNCATED-TARGET-PREVIEW (deck109, HIGH, representation/perception, corpus-general).**
   HIGHEST priority per the owner's standing rule (fixable perception flaws go first in the loop):
   a truncated burn/removal target preview HIDES on-sight-kill targets behind "(+N more)" AND lists
   the model's OWN creatures first, so a legal on-sight kill is suppressed BY OMISSION (deck109
   explicitly declined killing Young Pyromancer "not in the listed targets"). Fix: prioritize legal
   OPPONENT targets in the visible window and never truncate a guide-named/on-sight-kill target out
   of view (raise the target-list cap and/or order opponent creatures first). A perception gap that
   suppresses a legal play is the highest-value class the loop can close.
2. **R-RESTLESS-PUMP-OPTION-AMBIGUOUS (deck59, MED-HIGH, representation/render).** The pump-
   activation option renders `3/3 with Restless Apparition ... {card text: "...gets +3/+3..."}` --
   the "3/3" prefix contradicts the "+3/+3" text and the model cannot reconcile them, driving 4 of
   the corpus's 5 largest priority spirals (deck133 s16/s19, deck27 s18/s21). Every pump resolved
   CORRECTLY (latency, not misplay) but it is the single largest remaining spiral locus at a
   converged seat. Suggested render: `+3/+3 until EOT (2/2 -> 5/5)`.
3. **deck199 STEAL + R-DFC-BACKFACE probe (deck102, MED-HIGH, validation probe).** Discharges BOTH
   open deck102 items in ONE controlled game -- stack Tergrid + opponent-chooses-sac edicts vs an
   opponent with sacrificeable NON-token permanents on a curve that reaches a Lantern-affordable/
   God-unaffordable turn. Steal windows have been ZERO for 3 waves (mechanistic reason:
   opponent-chooses-sac edicts always feed TOKENS; the steal script is verified correct
   `@sacrificed(*[-token]...)`), and the Lantern back-face (R-DFC-BACKFACE) is un-re-exercised. If
   the probe lands, deck102 rotates OUT next wave. (deck199 likely needs to be CONSTRUCTED.)
4. **wave-24 PARSER HARDENING ship-decision (deck133 belt-and-suspenders, MED, reply-protocol/
   parser).** The b1 de-fang closed the protocol-example leak by CONSTRUCTION but WEAKLY
   (reconciliation C). Ship the wave-24 mechanism-layer guard (retraction/index scan counts only
   LINE-ANCHORED coded indices; excludes `CHOICE:` tokens inside quotes or verbatim `e.g.`-example
   echoes) so the closure is robust to any future re-introduction of a literal example index. No
   live failure remains -- this is hardening a weak-by-construction closure, not fixing an active FP.
5. **b4 residual: standalone ETB menu bare options + `Choose an option for :` empty-name (deck137,
   LOW-MED, representation).** b4 validated the card-text annotation, but the standalone shock/ETB
   menu still renders bare `pay 2 life`/`tap` under `Choose an option for :` (empty subject).
   Sibling of R-SHOCKLAND-ETB-UNLABELED / R-PAINLAND. Label `As <land> enters: pay 2 life
   (untapped) / do not pay (tapped)`; fill the empty menu subject.
6. **prose-only answer reversal convergence (deck109, LOW, parser b2-extension WATCH).** A prose-
   only reversal without a re-emitted coded CHOICE line parses answer-first (CORRECT per the b2
   spec). Convergence candidate for a future b2 extension (take the prose conclusion when it clearly
   supersedes an earlier coded line on a natural-stop reply); WATCH, not a fix.

**Below-bar CORE candidates (in ledger, NOT the prompt -- pending convergence):**
- affirmative "[untapped - can block]" tagging (deck131) -- would surface block-eligibility at the
  blockers seam; below-bar, promote only on a 2nd-seat convergence.
- prose-only answer reversal (deck109, = ledger #6).

**Carries (unexercised / un-re-exercised):**
- **N8 baka 0-power blocker gang-chump heuristic -- UNEXERCISED (6th corpus).** Answer-first
  structurally prevents block fallbacks at answer-first seats; needs a probe forcing a block
  fallback with a valuable 0-power body.
- **ENGINE-R4 Azcanta ACTIVATION probe -- orphaned.** A model-elected top-4-to-hand transform pick
  remains unobserved; needs a control-deck probe.
- **undying trade-annotation extension.** b6 shipped persist return clauses; undying is the sibling
  -- extend the trade-return annotation to undying when an undying deck enters the pool.
- **R-BLOCKERS-CONFLICTING-LINES (deck137 wave-24 carry) -- floored-by-guide, WATCH.** The blockers-
  seam guide floor landed (all 7 blocker replies legal, max 1,328 ch); the illegal-first-BLOCKS-line
  class did not recur. Carry as a watch, not an active item.
- **commit-lock regression WATCH.** Now that b2 shipped and is validated, watch for any answer-first
  re-freeze via the diff-3 `choice`-field check.
- **mulligan surface 2 corpora unexercised (deck109); Guttersnipe "attackers-tapped => all-tapped"
  misread single-seat WATCH (deck131, held 8/9); Yawgmoth + Bloodghast-fodder unexercised (deck133,
  no edict opponent at that seat).**

## Rotation reconciliation

Standing test: candidate for rotation OUT when the guide takes NO substantive modification AND no
seat-specific new-work signal survives. Verdicts are UNSENTIMENTAL -- the campaign's termination
condition is approaching, converged seats rotate.

- **59 -- ROTATE OUT (converged).** 6/6 first-guided corpus; both idiosyncrasy levers VALIDATED
  (mana-simplification + persist-sweeper asymmetry, both directions); guide is a BYTE-COPY (validated
  as-is, no modification); the b6 gaps are CLOSED-BY-FACT. The ONLY open item is engine-lane
  (R-RESTLESS-PUMP-OPTION-AMBIGUOUS). No guide new-work signal -> converged, rotates.
- **109 -- ROTATE OUT (converged).** 5/6; the Pillar 2-corpora belief-correcting-crutch clock
  CLOSED clean this corpus (4/4 windows, 0 conflations -- the exact wave-24 condition met); burn
  11/11 face, attackers 25/25; guide BYTE-IDENTICAL (md5 a8f163f7). The two NEW items are
  cross-cutting LEDGER items (R-TRUNCATED-TARGET-PREVIEW, prose-reversal), NOT seat-holds -> the
  seat itself is converged, rotates.
- **102 -- PROBE-THEN-ROTATE (stays ONE more corpus).** 4/6; guide has effectively converged (one
  fuzzy-term edit only), but the deck's CORE PAYOFF is unproven -- steal windows have been zero for
  3 waves and R-DFC-BACKFACE is un-re-exercised. Run the deck199 probe (ledger #3, discharges both);
  if it lands, deck102 rotates at wave-26. It does NOT rotate now (the keystone validation debt is a
  real new-work signal).
- **27 / 131 / 133 / 137 -- STAY (all guide-revised this wave).** 27: 4 surgical edits (needs a
  validation corpus). 131: 2 rules-correctness edits, near-terminal (projected rotation wave-26 if
  the edits validate). 133: 1 Arena edit, close to rotating (1/6 = threat-drought, not decision
  collapse). 137: Lovestruck adventure-body fix + blockers-seam floor landed (fresh new-work). A
  just-revised guide is not convergence.

**Converged CANARY list grows to 12:** 110, 21, 17, 14, 35, 49, 135, 62, 44, 140, **59, 109** (the
two added this wave).

### Rotate-in picks (2 slots -- fill the pool back to 7)

Remaining pool after 59/109 out = 27 (UB Wither), 102 (BW edicts/Tergrid), 131 (UR Guttersnipe
burn), 133 (mono-B devotion), 137 (GW convoke/adventure) = 5 stayers. Two guideless Step-0 decks
rotate IN (both verified: they have a `deckN.txt` decklist but NO `_strategy.txt` guide). Picked for
archetype coverage ORTHOGONAL to the remaining pool and to each other:

1. **deck18 "Kithkin" -- mono-W tribal weenie ANTHEM AGGRO** (Wizened Cenn, Thistledown Liege,
   Field Marshal, Glorious Anthem, Cenn's Heir; all-Plains). Fills the proactive-aggro gap left by
   deck109's rotation, but via a DIFFERENT color and mechanism (white go-wide anthem, not burn-
   reach). Distinct decision character: anthem-timing, alpha-strike math, lord-stacking -- none of
   which the pool exercises. Cleanest available "aggro" archetype.
2. **deck22 "Giants!" -- mono-R Giants tribal RAMP/MIDRANGE** (Calamity Bearer damage-doubling,
   Stinkdrinker Daredevil cost-reduction, Sunrise Sovereign / Borderland Behemoth lords, Inferno
   Titan top-end; all-Mountain). A big-mana tribal-PAYOFF archetype absent everywhere in the pool
   (131's UR is low-creature spellslinger; 109's departed mono-R was low-curve aggro). Exercises NEW
   decision seams with strong new-work potential: cost-reduction mana math + damage-doubling combat
   math + big-creature ramp curves.

Different from each other (white weenie go-wide vs mono-red big-creature ramp) and from the pool.

### The ENDGAME (termination tracking)

**146 guideless decks** currently sit in `bin/Res/ai/baka/` (a `deckN.txt` with no
`_strategy.txt`). deck18 + deck22 rotate in this wave and will produce their initial guides at
wave-26, dropping the guideless pool to **144**. HONEST reframe for the owner: at ~146, the deck
supply is effectively unbounded relative to the campaign, so DECK DEPLETION is NOT the binding
termination constraint -- NEW-WORK EXHAUSTION is. The guided REVIEW roster (previously-guided decks
awaiting re-validation) IS exhausted; every future rotate-in is a fresh guideless Step-0 newcomer.
Track termination by whether rotate-ins keep yielding engine/skill/core signal: this wave still
produced heavy new-work (6 skill Method headlines promoted/added, a 6-item engine ledger, 5 guide
revisions, 2 rotations), so the loop is NOT terminating yet. The signal to watch is a corpus where
the rotate-ins converge in one pass with NO engine/skill/core new-work -- then the loop is done
regardless of remaining deck count.

**WAVE-26 POOL: 27, 102, 131, 133, 137, 18, 22** (59 + 109 rotated OUT to canary; deck18 Kithkin +
deck22 Giants rotated IN as guideless Step-0). deck102 is on its probe-then-rotate clock.
