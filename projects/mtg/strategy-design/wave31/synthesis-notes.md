# Wave-31 synthesis notes

Seventeenth per-deck-rotation cycle. Pool = 137 / 146 / 139 / 148 / 122 / 152 + **158 (Orcs of
Mordor, RB amass) as the guideless Step-0 debut**. PRIMARY corpus
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260727-025026`, binary `cf7cb2e53`, -T 3000,
REPPENALTY=1.05 (9th corpus). 21 games, 0 timeouts / 0 draws, **3 cap-adjudications** (wave-30:
12), 36 gameend logs, integrity clean. **Zero crash signatures in all 21 stderr files — the
N-139h corpus gate PASSED.** Fallbacks 6 / 1,432 (0.42%). Win table: 158:5/6 (Step-0 debut,
strongest of the campaign), 148:4/6, 122:3/6, 152:3/6, 137:3/6, 146:2/6, 139:1/6.

**Core prompt: PASS, 26th consecutive — 7 of 7 seats.** Recorded in full at the end of this file.

---

## 1. Per-proposal adjudications

Every proposal from the seven `skill.md` files, with the disposition and a one-line rationale.
The merged rungs are listed under the merge, not twice.

### ACCEPTED — promotion-grade / hard gate

| # | Proposal | Seat(s) | Disposition | Rationale |
|---|---|---|---|---|
| 1 | RETROACTIVE full-guide card-fact audit gating any FREEZE or EXIT | deck146 P1 | **ADOPTED as a HARD GATE** (wave-31 HL1; Step 0 + Step 0-ter + DoD) | The wave's strongest single lesson. Three false teaches — a manland priced at `{1}{B}` against the primitive's `auto={3}{B}`, "sticky value creature" Acererak whom the primitive bounces to hand on ETB, and an MDFC "extra land" that is structurally unofferable — survived **four corpora and three freeze validations** because the machinery only ever asked whether teaches FIRED. A false-but-unexercised teach fires zero times and looks fine; that is a structural blind spot, so the fix must be a gate, not a reminder. Adopted with deck146's two-line freeze verdict (teaches FIRED vs teaches are TRUE). |
| 2 | Extend the verify-oracle rail from NUMERIC to PERSISTENCE facts | deck146 P2 | **ADOPTED** (wave-31 HL2, folded into the same rail) | Acererak has no wrong number; the defect is an omitted persistence fact, and it converted into a lost game (vs148 seq18 t8: declined an offered Vanishing Verse on the Kor lord to cast him; the Kor board swung 12 the next turn) plus **59 impossible planning passages across 3 games**. The render cannot cure it (0 of 163 `Cast ...` lines carry card text). Adopted with the collapsed-absolute rule for interacting teaches. |

### ACCEPTED — merged rungs (convergence was the merge signal)

| # | Proposals merged | Seats | Disposition | Rationale |
|---|---|---|---|---|
| 3 | Control-role teaches must name the TAP COST at the attack seam **+** PAYOFF-PERMANENT PRESERVATION | deck122 P1 + deck148 P1 | **ADOPTED, MERGED into one rung** (wave-31 HL3; Step 4) | Two seats, two archetypes, one mechanism: a teach that assigns a role or raises a permanent's value is applied where the model REASONS, and evaporates at the combat seam, which re-derives from a generic prior ("Risk: None"; "trading my 2/2 for their 2/2 is the correct tempo move"). Both models demonstrably knew better in the abstract, so this is local re-derivation, not knowledge. Shipping two categories would have hidden that they are one; the two faces are kept explicit inside the rung. deck148's fragile-lord corollary (split underperformance into construction vs voluntary trades) is its closing clause. |
| 4 | Bidirectional adjudication caveat **+** ceilinged-exit confirming-corpus clause | deck122 P2 + deck148 P2 | **ADOPTED, MERGED** (wave-31 HL7; DoD) | Same principle, opposite signs. deck122 fell 6/6 → 3/6 with **kills flat at 2** and pool adjudications 12/21 → 3/21; deck148 rose 1/6 → 2/6 → 4/6 on a byte-identical guide and an unchanged list. Both records are pool properties. The merged rung names the KILL COUNT as the pool-invariant headline statistic and requires the pool's adjudication rate beside every record at such seats. |
| 5 | Same-seam CONTROL **+** TRIGGER CONDITION before the read **+** classify a defer by TRIGGER not EXIT | deck137 P1 + deck139 P1 + deck158 S-158c | **ADOPTED, MERGED into a three-part sharpening of arrival-tracing** (wave-31 HL4; Step 2 + DoD) | Three independent seats hit the same instrument gap in one wave — that is what promoted it past single-seat tagging. Each part is cheap and each paid: the Loxodon control pinned deck137's fault to the alternative(convoke) branch; the Pollywog trigger predicate exposed that a "0 crashes, 4/4 mutates arrived" validation touched the crashing conjunction **zero** times; deck158's same-card counterfactual (X-slack 0 vs 3, two turns apart in one game) isolated a new trigger with no code reading. |

### ACCEPTED — single-seat, adopted on the stated grounds

| # | Proposal | Seat | Disposition | Rationale |
|---|---|---|---|---|
| 6 | Never encode a FREQUENCY/RARITY claim about an unexercised mechanic | deck152 P1 | **ADOPTED as doctrine** (wave-31 HL6; Step 0-bis) | Single-seat but it closes the AUTHORING half of a rule the skill already holds on the TRIM side (Step 0-ter rule 5), so it opens no new doctrine. Falsified hard: the wave-30 guide's "this rarely happens, so treat them as their day-side bodies" met **werewolf flips in 4 of 6 games**. The decisive detail is the self-contradiction — the same reviewer recorded the flip as UNEXERCISED in findings in the same pass, which is why a written rung beats a reminder. |
| 7 | GUIDE/ANNOTATION RECONCILIATION SWEEP when an h-batch rewrites a surface | deck152 P2 | **ADOPTED, ordered FIRST in the Step-0-ter procedure** (wave-31 HL10) | The adversarial twin of the wave-18 apportionment rung: apportionment asks about CREDIT, this asks about CORRECTNESS, and correctness runs first. Worked instance is exact: h4 shipped "only the currently-shown face can actually be played as a land" into the same option line where the deployed guide said "you can also flip a Pathway to its blue face before playing it." One grep. The same sweep retired an obsoleted teach for free (h5 drove toggle-only windows 121 → 0). |
| 8 | CAST-NOTHING RATE split by window composition | deck152 P3 | **ADOPTED as a Step-0-ter instrument** | Measurement-only, biases no guide content — the same standard that admitted the 0-attacker durdle instrument. It changes the verdict: raw 3/39 → 7/49 reads as a regression; split it is 5 legitimate instant-holds and 2 fatal declines (deck146 seq21 at 3 life; seq26 at **1 life** with four casts offered). Without the split the revision aims at removal discipline, which is working. |
| 9 | CASTABLE-BUT-NOT-OFFERED audit in every loss | deck152 P4 (+ deck146 wave-30 probe) | **ADOPTED** (2 seats) | The inverse of the wave-26 harness-blocked-compliance rung, which structurally cannot see a silently absent option (no fallback, no defer, no counter). It paid on contact: Sigarda payable-and-unoffered twice in the game deck152 lost at 1 life, with a free positive control (deck137 seq28, offered off only four untapped sources) that isolated the mechanism in one diff → **N-152c**, a source-located engine defect. |
| 10 | PRESERVE LEDGER at a high-performing Step-0 seat | deck158 S-158a | **ADOPTED as Step 0-bis rule 0** (wave-31 HL8) | Step 0-bis is written for guideless seats that played badly, and its rule 5 would have opened deck158's guide with a loud attack floor aimed at a **30/30** seam — the deck135 pursuit-overshoot installed deliberately. Promotion trigger recorded: a second guideless debut at 4/6 or better. |
| 11 | Rank tax spikes by DECISION KIND; low-branching tax is the strongest saliency signal | deck158 S-158b | **ADOPTED as a Step-0-bis rule-2 refinement** | Cheap, no cost if wrong, and it was the wave's clearest detector: three land-drop replies at 13,319 / 12,875 / 12,198 chars, all the same belief that lost the seat's only game, outnumbering the decisive instance 3:1. |
| 12 | CONSTANT-annotation check in the magnitude audit | deck158 S-158d | **ADOPTED as a refinement of the Gray-Merchant offset rung** (per the reviewer's own request, not a new rung) | The existing rung assumes a fixed OFFSET; neither witness has one — Foray of Orcs printed `{right now: damage 0}` in 2/2 instances against true 2 and 9, Feed the Swarm printed `life -2` in ~44/44 against a target-dependent 0..3. The audit question becomes "does it ever CHANGE?" |
| 13 | "PROMISE THE ENGINE OWES" as a third guide-prose state | deck137 P2 | **ADOPTED single-seat** as a face on the stopgap-retirement lifecycle rung | Real gap: the line is rules-correct (CR 702.51), engine-intended, fixture-asserted, and has delivered **0 Soldiers from 5 March picks over 6 games**. Neither existing rung fires and FREEZE is correct — but unnamed, the line persists across freezes with nobody owning the question. Adopted with the `PROMISE OWED:` phrasing. |
| 14 | WRONG-EXEMPLAR defect + mulligan numeric floor | deck139 P2 | **ADOPTED single-seat** (Step 0 audit + Step 5 pursuit rule + DoD) | The guide's clause named a Forest; the three failures named dual lands and, once, a literal Forest ("no Forests"). The exemplar does no work, so the sentence never matches the moment — making it louder cannot help. The mulligan corollary is filed as a named instance of the existing pursuit/release rule (one game reached **seven mulligans and a 3-card keep**). |

### ADOPTED BY SYNTHESIS (no single reviewer proposed it)

| # | Rung | Disposition | Rationale |
|---|---|---|---|
| 15 | PATH-SCOPED FIXES: enumerate every path that emits the surface, quote the post-fix string from each | **ADOPTED — wave-30 HL2 hardened from a caution into a SWEEP** (wave-31 HL5) | Three independent recurrences in one wave (§2). This is a cross-seat pattern no single seat could see, which is precisely the synthesis seat's job. |

### NOT ADOPTED (recorded so they are not re-proposed)

- **deck122 — relax the Blood Artist / Felidar-bottom absolutes.** No change. Both were relaxed by
  the model with sound local reasoning and zero realized cost; the deck131 never-block precedent
  says the absolute-with-escape-clause form is working as intended.
- **deck146 — off-menu invention licensed by a guide imperative** (vs122 seq18: `CHOICE: 4 (Cast
  Lolth, Spider Queen)` into `{0=pass, 1=becomes beholder}`, quoting the guide). **RECORDED as a
  convergence watch, not adopted** — the reviewer explicitly declined to propose it, the guide's
  imperative is correctly conditioned ("the moment you can **afford** one"), and this is witness 2
  at the same seat. A third witness at a DIFFERENT seat promotes the candidate rail (pair every
  CAST-IT imperative with its own negative clause).
- **deck137 — a Giant Killer tap-teach.** Rejected: 29 priority windows, 28 correct passes, and the
  passes are right (the ability taps Giant Killer itself). Teaching restraint that already exists
  is the anti-pattern the cut-what-works rule forbids. Its X=0 March lapse (vs152 s33) is one
  violation against already-optimal prose = variance.
- **deck148 — mulligan "zero creatures" misread → representation (N-148d), not a skill item;
  >10k-char spirals (6 this corpus, max 14,120) → the wave-25 headline-5 latency ceiling predicts
  exactly this and the rung already forbids new per-seat floors.**
- **deck152 — a blocking-protocol change off deck148 seq17's `B2:A1, B2:A2`.** Single benign
  occurrence, absorbed by first-wins parsing to the same block the corrected reasoning chose.
- **deck158 — a "hold Bowmasters for the opponent's turn" flash rule.** Killed by the executability
  audit BEFORE it was written (6 priority windows in 6 games, ONE on the opponent's turn, and the
  one window with Bowmasters in hand and mana up did not offer the cast at all). Recorded as the
  audit's invisible save.
- **Any core-prompt change** (all seven seats). §3.

---

## 2. Cross-seat pattern analysis

### 2.1 THE DOMINANT PATTERN — three independent PATH-SCOPED-FIX recurrences in one wave

wave-30 HL2 said a fix landing on one code path does not close a class-named gap on a sibling path.
Wave 31 produced three at once, from three seats that could not see each other, and two of them
cost games:

| item | the fix that landed | the sibling path it missed | cost |
|---|---|---|---|
| **N-122c** (render, CROSS-SEAT) | wave-30 g4 tapped-tag reword, validated on non-combat tapped creatures ("read correctly 131x at deck137") | the DECLARED-ATTACKER path, where the same wording contradicts the adjacent tag: `[tapped - cannot attack or block this turn] [attacking]`, **331 occurrences across the 42 corpus logs**, verbalized by models at **four separate seats** | **LOST a game.** deck122 vs158 seq39 t13: the answer-first line was the correct `BLOCKS: B1:A2, B2:A3, B3:A1, B4:A1`; the paradox-driven self-correction dropped both chumps off a 19/19 and the seat died at -16 |
| **N-137b** (engine) | divergence A: convoke creature-credit added to `ManaEngine::maxAnnounceableX` (ManaEngine.cpp:640) | **neither** MTGRules announce path calls it — both compute X from the floated pool (MTGRules.cpp:433 normal; :1057 alternative-cost), and a convoke cast is an ALTERNATIVE-cost shell by construction | the credit is unreachable on the only route that needs it: `Announce the value of X` fires **28x at deck158** (normal X cast) and **0x at deck137** across six games despite 5 March picks |
| **N-152e** (render) | h4's MDFC-land honesty rewrite | TRANSFORM DFCs, which still carry the wave-30 wording including the now-false "the other face appears there as an alternative-cost cast", plus a literal `"backside"` placeholder | cosmetic this corpus (both windows also offered a real action) — but it is the same defect shape surviving in plain sight |

The synthesis reading: this is no longer a caution about optimism, it is a **verification procedure
gap**. A surface-level fix is not shipped until every path that emits the surface has been
enumerated and its post-fix string quoted; for an engine fix, until the production call sites are
named — a fix in a helper is not shipped while production paths do not call the helper. The cheap
mechanical form: grep for the NEW string **and** for the OLD string; an old string surviving
anywhere is the sibling path. Encoded as wave-31 HL5.

### 2.2 THE DIVERGENCE-C FAMILY: three trigger shapes, ONE exit — fix the EXIT

The ledger has been carrying "convoke divergence C" as a convoke item. Wave 31 falsifies the
scoping from two directions in the same corpus:

| trigger | witness | detail |
|---|---|---|
| convoke validation reject | (the original) | `AIPlayerGPT::FindCardToPlay`, `aiForcedCandidate` set, `AIPlayerBaka::FindCardToPlay(pMana, "*")` returns NULL |
| **PLAIN cast reject** | deck137 vs152 **s34 t12** (N-137d) | the rejected pick at s33 was the **plain** `Cast March of the Multitudes {g}{w}{w}{x}` — convoke correctly absent, all three creatures tapped from the s32 attack. So the gap is the general **offerable()-approves / `"*"`-validation-rejects** shape across ALL cast kinds |
| **X-slack == 0** | deck158 vs146 **s25 t13** (N-158i) | deck158 runs **no convoke** and no creature-tapping alternative cost. Three untapped Mountains against `{X}{R}{R}{R}` ⇒ the only affordable X is 0. Same-card counterfactual two turns later (6 sources, slack 3) announces X and resolves |

All three exit identically: `kind=defer`, `choice=-1`, `options=0`, `prompt=""`, `reply=""`,
`fallback=deferred_to_heuristic` — **the model is never asked and its window is burned.**

**Directive for the wave-32 fix agent: fix the EXIT, not the triggers.** Per-trigger patches will
chase this forever; a third trigger already exists that nobody predicted. And deck137's sharpening
settles which half of the named fix direction to take: at vs152 s34 the reject was substantively
**RIGHT** (X could only be 0 and March's own script is `auto=this(X=0) donothing`), so "accept the
cast on `offerable()`'s payability" would let guaranteed-blank casts through. **Take the LOUD
RE-ASK half.** The bad part is the mechanism — silent, no re-ask, no record — not the outcome.
deck158's item adds the cheapest repro to build the fixture from (severity LOW, repro quality
HIGH — the two fields are deliberately separate).

### 2.3 The wave's epistemic through-line: symptom-clean is not validated

Four independent seats produced the same shape — an instrument reporting clean while the thing it
was pointed at was untested or wrong:

- **deck146**: three freeze validations, zero card-fact checks → a game-costing false teach.
- **deck139**: "0 crash signatures, 4/4 mutates arrived" → the crashing CONJUNCTION (Pollywog on the
  battlefield + a discounted mutate actually PAID) occurred **zero** times; Pollywog reached play in
  1 of 6 games, its one discounted offer was declined, and it died before the mutate that was taken.
  In a release build the same underflow computes a wrong cost *silently*, so a clean corpus is
  compatible with the bug being fully live.
- **deck152**: fallback rate **0/243** while the deck's best card sat payable-and-unoffered twice in
  the game it lost at 1 life. A silently absent option is invisible to every counter.
- **deck137**: logged fallbacks **1**, arrival-traced convoke failures **6** — the third consecutive
  wave in which this class hides from the counter (wave-27 "harmless by-design"; wave-30 2 logged vs
  5 real; wave-31 1 vs 6).

This is why headlines 1 and 4 are both gates rather than advice: in every case the reviewer's
instinct was right and the instrument was the thing that had to change.

### 2.4 Rotation reads that the pool, not the guides, produced

The pool's cap-adjudication rate collapsed 12/21 → 3/21 when deck136 (grind) was replaced by
deck158 (aggro). That single pool property moved at least three records — deck122 down (6/6 → 3/6,
kills flat), deck148 up (2/6 → 4/6, ceiling unchanged), deck146 down (3/6 → 2/6, one loss with a
real pilot component). Any wave that swaps an archetype must decompose EVERY record against the new
adjudication rate before reading any of them as guide signal (wave-31 HL7).

### 2.5 Positive results worth recording

- **h1 (N-139h crash fix): corpus gate PASSED** — zero crash/abort/assert/core-dump signatures in
  all 21 stderr files, confirmed independently at 5 seats. No regression. (Positive revalidation is
  a separate, unmet question — §2.3.)
- **h3 (reveal chooser), h4 (MDFC land text), h5 (display-toggle auto-pass), h6 (walker magnitude)
  all landed.** h5 is the most decisive: deck152 priority asks 148 → 22, display-toggle-only windows
  **121 → 0**, and the wave-30 unparsed fallback born of a toggle-only window has no analogue.
  h6: deck146's walker magnitude annotations 14/14 bogus → 0.
- **N-152a's blast radius is SMALLER than assumed**: Teferi is *not* blocked (cast and piloted via
  Deserted Beach, loyalty 4/5/3/1 coherent). Only the Pathway back-face colors are unreachable.
- **The werewolf day/night surface is finally EXERCISED** (night in 4/6 games, both directions, no
  flip-thrash, no identity desync) — wave-30's open item closes with exactly one render defect
  (N-152d).
- **deck158's guideless core confirmations** (the cleanest read available on what core already
  does): 30/30 full attacker declarations, ping-arithmetic correct 5/5, blocker survival annotation
  quoted and obeyed 11/11, mulligan land-count default kept all six 3-land openers with no
  deliberation.

---

## 3. Core prompt — PASS, 26th consecutive (7 of 7 seats)

No seat proposed a core change, and four seats argued the PASS positively rather than by absence
(deck148: 0 fallbacks / 187 decisions, 0 parser contradictions; deck152: 0/243; deck139: 0/154;
deck158: four named core surfaces confirmed working from a guideless seat).

The boundary held under this wave's four strongest temptations, each with its routing argument on
the record:

1. **The attack-into-a-tapped-board misplay** (deck122 vs158 seq30) would motivate a core line at
   the attack seam. Rejected by the seat itself: the model writes the correct rule unprompted three
   decisions later (seq37). A core restatement of a fact the executor demonstrably holds buys
   nothing and taxes every decision in the corpus → guide/posture layer (skill.md P1).
2. **The `Mana available:` colour-set misread that lost deck158 its only game** (vs148 seq19, 1
   life, an offered `Cast Snarling Warg {3}{b}` declined as unaffordable off 5 untapped sources).
   The core already says this in **three** places. A rule stated and still bypassed routes to
   REPRESENTATION, never to a fourth copy → N-158g (count-first wording), with a guide stopgap.
3. **"Summoning-sick creatures can still block"** (deck158): one misread and one correct read at one
   seat. Held at the deck layer. Promotion shape is pre-agreed — a SECOND seat makes it a one-clause
   COMPLETION of the existing summoning-sickness sentence, not new doctrine.
4. **The low-life creature-decline family** (deck152/deck146 seq21+seq26): the REJECTED appendix has
   turned this down twice on single-seat evidence; it now lives as a deck-guide deploy floor. The
   named promotion trigger is a second seat reporting a low-life creature-decline against an offered
   body.

Two standing MODEL watches carried with no core change: the invent-an-off-menu-option shape
(deck146, witness 2 at that seat, now driven by a GUIDE IMPERATIVE rather than the model's own
PLAN) and the answer-first/reason-after protocol tension (deck148 seq17), which remains resolvable
only by a harness A/B.

**One WATCH raised as data by deck139 and owned by synthesis:** the ~200s latency tail is in its
third consecutive corpus at that seat and has **left the combat seams** — of six decisions above
195s, three are casting asks, one is mulligan bottoming, one is priority, one is attackers, and all
six are 12.2–13.3k-char replies. So it is a reply-LENGTH behaviour at the token cap, not a property
of any decision kind, and the guide-side "decide combat fast" teach cannot be expected to fix a
seam-agnostic phenomenon. Every plausible intervention (max-token trim, "stop after N sentences", a
reasoning brake) trades decision quality for wall-clock at a seat whose decision quality is already
the problem. With the pool at 3 cap-adjudications, the pressure to act is low. **No change this
wave; re-read next wave with the cross-seat tail distribution in hand.**
