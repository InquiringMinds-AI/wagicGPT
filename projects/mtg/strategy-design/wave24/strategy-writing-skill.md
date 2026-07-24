# Skill: Writing a per-deck strategy guide for the qwen pilot (wave 24)

## Synthesis notes (wave 24) — TENTH per-deck-rotation cycle; pool = 6 stayers (27 UB Zombies / 102 mono-B Tergrid / 109 mono-R Aggro / 133 mono-B Devotion / 131 UR Spellslinger / 137 GW Adventures) + 1 rotated-in (59 WB Persistence, GUIDELESS Step-0-bis; 140 rotated out to canary). Binary d7ff8b6c4 = wave-24 step-1 batch (5f78e2934) + the FIRST-EVER PREGAME MULLIGAN PHASE (86df3b482, London-conformant) + the N9 retracted_choice hotfix. PRIMARY corpus `matchups-20260724-125739` (a FIRST run, -073032-N9B-OVERFIRE, is QUARANTINED — the N9 branch (b) over-fired 131 retracted_choice on hypothetical rules prose; never read as decision evidence). Corpus health: 21 games, **2 timeout-DRAWS (first timeouts in 9 corpora — 27v137 and 102v133, both ~turn 10 at the 2400s cap, even life)**, 7 adj. Fallbacks 22/1293 = 1.7% raw but 19 are deck137's by-design convoke defers → **TRUE model-fallbacks 3/1293 (0.2%, cleanest corpus yet)**. HOTFIX VALIDATED: retracted_choice 131→1 (and that 1 is a residual FP, headline 2). Latency median 17.3s / p90 73s; answer-locked retry fired 0× (correct — the decode-collapse class did not recur). **Pilot repetition_penalty=1.05 (2nd corpus): KEEP — SAME at every seat, no prose degradation, no spiral cure (a GUIDE/representation commit-rule cures spirals the sampler cannot; deck137's worst spiral RELOCATED from the now-floored attackers seam to the blockers seam).** Core (general prompt): **PASS, 19th consecutive.**

**Method headline 1 — DECODE-COLLAPSE vs COMMIT-LOCK graduates to a DURABLE classification rung (2 corpora, 2 distinct members, both harmful, clean owner split); and the parser-lane candidate SHARPENS to "prefer the model's FINAL coded line ONLY on a naturally-terminated reply" (deck27, PROMOTED from wave-23 headline-3).** deck27 vs137 seq5 (pregame BOTTOM phase) is the first HARMFUL NON-TRUNCATED commit-lock: coded line 1 `PUT: 3, 5, 6` bottomed the deck's ONLY blue source, the reply then reasoned to the correct `5, 6, 7` and STOPPED NATURALLY, and answer-first froze the wrong first line → colorstuck opener, fed the vs137 timeout-draw. This is the OTHER member of the wave-23 pair (decode-collapse was the first), independently re-hit with a harmful instance, so the classification promotes to a standing rung. The reviewer test is one question: **does the failed/misplayed reply contain a coded line?** NO → decode-collapse (owner: sampler/harness — rep_penalty, max-tokens fast-fail, answer-locked retry; the retry's 0 corpus-wide fires this wave are CORRECT, the class did not occur). A WRONG coded line + a later self-correction → commit-lock (owner: GUIDE make-the-snap-right AND/OR PARSER). The wave-24 instance SHARPENS the parser candidate: the freeze is NOT only a truncation artifact — a fully-completed reply is still frozen to line 1 — so the parser fix must key on "reply reached a natural stop / did not hit max-tokens → prefer the LAST coded line (the model's conclusion)," GATED so it never touches truncated replies (where answer-first line-1 is the only safe floor). **RECONCILE carefully with INDEX-WINS and answer-first semantics — this is the top parser DESIGN question for the between-wave ledger, not a settled change** (a naive last-line rule collides with INDEX-WINS's trust of a re-affirmed in-range first-line index; see headline 2's protocol leak, which is the exact hazard — a quoted/example coded line as a spurious "last" index). Distinct-corpus, distinct-member, both-harmful → PROMOTED.

**Method headline 2 — the PROTOCOL-EXAMPLE LEAK: the reply-protocol's worked example `e.g. "CHOICE: 2 (Cast Fatal Push)"` seeds phantom coded indices; now 2-SEAT CONVERGENT and owns BOTH residual parser items.** deck133 vs131 s21 (the corpus's ONLY retracted_choice = the N9 hotfix's live acceptance test) is a RESIDUAL FALSE POSITIVE: a clean first-line `CHOICE: 1 (Cast Fatal Push)` re-affirmed throughout, but the hotfix's second-coded-index scan matched the prompt's quoted example `"CHOICE: 2 (Cast Fatal Push)"` (echoed 4× as the model agonized over OUTPUT FORMAT) as a contradictory second index → deferred (non-deciding; heuristic cast it, deck133 won). deck102 vs133 s5 is the sibling: the ONLY coded line was `CHOICE: 2 (Cast Commander's Sphere)` — OUT OF RANGE (only 0/1 legal) — pattern-filled from the same example's literal index 2, compounded by a stale PLAN naming an unaffordable card (non-deciding; heuristic passed, correct). ONE ledger item: the retraction/index scan must count only **LINE-ANCHORED** coded indices and **EXCLUDE `CHOICE:` tokens inside quotation marks or that are verbatim echoes of the protocol's `e.g.` example**; and/or make the example index-agnostic (`CHOICE: <the number> (<action name>)`) and/or ROTATE the example text per-ask so it cannot be latched. This is a REPLY-PROTOCOL layer change (code-appended text), NOT the core prose — route to the engine ledger; the core prose PASSES (a parseable first-line CHOICE was emitted in all 206/205 non-empty replies at both seats). Aggravators to encode: the example's card name equals a live option's name (maximizes the quote-latch), and a carried PLAN naming an illegal/unaffordable action seeds the phantom cast (plan-hygiene: do not feed a plan whose named action is not in the current legal/affordable set as authoritative intent).

**Method headline 3 — ATTACKER/DECISION-SCORING NEEDS A THIRD DIFF: answer-vs-conclusion fidelity (deck131, single-seat, GAME-DECIDING; promotion threshold named).** The wave-23 two diffs — (1) execution fidelity (declared-set == recorded-set) and (2) decision-input fidelity (claimed-board-state == actual `[tapped]` tags) — BOTH PASS a reply whose board read is CORRECT and whose reasoning explicitly CONCLUDES the guide-correct action, but whose FIRST line names the OPPOSITE action. deck131 vs27 s30: the reply reasons impeccably and QUOTES the guide ("you must leave Guttersnipe home ... attack with none"), then a "Correction: Wait ..." spiral flips it and the first line is `ATTACK: A1`; the harness locked the first token, Guttersnipe swung into a wither wall and died. Add **diff 3: the leading action line == the reply's own concluding recommendation.** When a coherent reply's body argues to a DIFFERENT action than its first line, the harness locked a wrong answer-first token → score the decision by what the model CONCLUDED, and route the miss to the HARNESS lane (NEVER the guide — no prose fixes a first-line-vs-body contradiction). This separates a MODEL-COMPREHENSION miss (diff 2, the model was wrong) from a HARNESS-EXTRACTION miss (diff 3, the model was RIGHT but the first token wasn't); they are identical from the win column (a lost Guttersnipe) but route to opposite lanes — deck131 had BOTH in the same matchup (vs59 s23 = diff 2 tapped-misread; vs27 s30 = diff 3). Same family as headline 1's commit-lock parser question: a last-coded-line preference on natural-stop replies is the structural fix for diff-3 too. Promotion threshold: a 2nd seat showing a first-line-vs-conclusion contradiction promotes diff 3 to a standing decision-scoring step.

**Method headline 4 — Step 0-bis rule 3 gains a SIXTH idiosyncrasy face: the SYMMETRIC EFFECT that is ASYMMETRIC IN YOUR FAVOR via recursion (deck59, single-seat) — the clean INVERSE of the fourth face.** Where the FOURTH (deck102) face OVER-fires a symmetric card (reads its self-cost as one-sided disruption, the number's SIGN ignored → a STOP-gate), deck59 UNDER-fires a symmetric MASS-REMOVAL (Wrath of God / Day of Judgment / Damnation) because it does not see that the deck's own PERSIST recursion makes the wipe one-sided IN ITS FAVOR — your creatures return as 1/1s, the opponent's stay dead. It treats the sweeper as generic "slow removal" and declines it in the spot that wins (deck109 s12: at 6 life facing lethal, "Day of Judgment is too slow and risky," played a blocker, swept only at 2 life — too late — and LOST; contrast the deck131 WIN where it was AHEAD and correctly NEVER swept). Same mechanism as the fourth face (a symmetric card mis-read as if the symmetry did not apply to the side it helps/hurts) but OPPOSITE lever: a scoped GO-gate keyed to a board-readable trigger — "cast the symmetric effect WHEN behind / raced / opponent gone wide, because YOUR side recovers and theirs does not; NEVER when ahead (you would kill your own attackers)" — that MUST carry the recursion WHY or the weak executor keeps pattern-matching "board wipe = slow / kills my stuff = bad." This is also a rule-4 COUNTERMAND-A-CORE-REFLEX case: the core's correct-in-general "don't overcommit into a sweeper / a wipe kills your board" prior is anti-teaching for a deck whose board survives its own wipes. Tag single-seat; promote to a named rule-3 face if a second deck whose OWN engine inverts a symmetric effect (another persist/undying deck, a "you gain what they lose" symmetric payoff) reproduces the UNDER-fire. ALSO: the belief-that-VETOES face (17/21) now has a THIRD seat — deck59 (W/B hybrid creatures off Plains+Swamp, no green source) vetoes/taxes its own casts thinking it needs green — AND a newly-observed MULLIGAN-quality cost: it mulliganed a KEEPABLE all-Plains 7 to color-anxiety ("no black") and got run over. The same corrective FACT (rule #1 mana simplification + pip table + trust-the-rendered-number) fixes the mulligan too — worth one line that the veto-face's cost is now cast-tax AND mulligan-quality.

**Method headline 5 — the PREGAME MULLIGAN surface (first fleet outing) adds two reviewer diffs and closes one crutch-lifecycle witness; SCORE THE NEW PHASE against a zero-coverage guide before scoring any decision in it as a pilot error.** The phase is MECHANICALLY SOUND at all 7 seats (no double-asks, London bottom-N math correct = mulligans-taken, first-draw integrity, the wave-23 missing-mulligan gap RESOLVED — deck131 vs133 now renders an ask; one known cosmetic: the "opening hand" narration can reflect the pre-mulligan deal, harmless). But it exposed THREE decision-quality failures the win/keep-count checks MISS, so the reviewer method extends: **(a) the MULLIGAN HAND-READ diff (deck102 vs59: mulliganed a keepable 2-Swamp hand while its reply claimed "zero basic Swamps")** — extend method-2's claimed-vs-actual diff to the pregame HAND: never accept a "no lands / no black / all bombs" rationale without counting the literal `Your hand:` line, and check bottom-pick quality (did the keep FLOOD — bottom a payoff to keep an Nth land, deck102/deck59/deck131 each showed a flood-or-wrong-card bottom). **(b) NEW-PHASE ZERO-COVERAGE audit:** when a brand-new engine phase appears, audit the guide for zero-coverage BEFORE scoring a decision in it as a pilot error — an uncovered surface warrants an ADDITIVE guide line reusing the guide's existing valuations (deck131 got a bottom-priority line from its Prism-Ring-lowest ranking; deck27 got a bottom-phase section: keep color-fixing first, never bottom your only source of a color, decide the list once on line 1), which is NOT the sanctioned-against "louder at a covered line." Note (deck109/deck27): bottom/mulligan are arithmetic/color-counting SNAP decisions and answer-first is unreliable for them — the guide-lane (front-loaded heuristics) and the parser-lane (headline 1's last-line preference) are complementary, not either/or. **(c) CRUTCH-LIFECYCLE clean witness — R-ADV-FACE CLOSED-BY-FACT (deck137):** the engine FACT shipped (adventure-cost options now render `{adventure spell: <effect>}`, no wrong (P/T)), live acceptance PASSED conclusively (the Usher self-bounce misfire recurred 0×), and deck137 DEMOTED the interim P/T-TRAP crutch per the removal condition. Mark the wave-23 FIFTH idiosyncrasy face WATCH as **CLOSED-BY-FACT** (a REPRESENTATION bug the option-annotation fix eliminated — a single-seat misrender, not a durable model belief), NOT promoted; this is the reference witness of the crutch-lifecycle matched pair (annotate to teach → ship the FACT → demote the crutch), and it CONTRASTS the genuine model-belief faces (1,4,6) which persist because no FACT can fully fix a belief. Corollary (deck137, single-seat WATCH): a per-seat commit-rule/floor is not a one-time fix — flooring one high-stakes seam RELOCATES the worst spiral to the next unfloored neighbor (deck137's max spiral moved attackers→blockers once the attack floor landed), so audit ALL high-stakes seams (attackers, blockers, targets, X) for a floor.

**Harness / between-wave ledger (this wave):** (i) both timeout-DRAWS are INFERENCE-LATENCY SATURATION at -j3, NOT engine/phase stalls — 27v137 combined decision latency 2352s/2400s (98%; deck137 decision-DENSITY 2.4× the opponent — adventures double cast options, convoke adds defers, go-wide multiplies choices), 102v133 1939s/2400s (81%; mono-B attrition mirror = many pass-priority windows, each a full model round-trip). The known timeout-economics rule updates with numbers: for decision-DENSE pools (adventures/convoke/go-wide, or stalled attrition mirrors) raise WAGIC_GPT_TIMEOUT or drop to -j2, or accept cap-adjudication as expected. (ii) The 19 convoke defers (deck137) are BY-DESIGN and HARMLESS (the heuristic tap-selection never stranded a wanted attacker/blocker) — leave by-design, do NOT over-engineer a "trust the convoke option" core line on one convoke deck. (iii) Representation ledger items surfaced: R-SHOCKLAND-ETB-UNLABELED (deck137, sibling of R-PAINLAND — the shock "pay 2 life / tap" ETB menu has an empty subject), R-STALE-ECHO-QUALIFIER (deck137, INDEX-WINS residual: an in-range `CHOICE: 2 (Tap Temple Garden)` downgraded to stale_echo because the card-name qualifier collides with a stale sibling option), R-BLOCKERS-CONFLICTING-LINES (deck137, flag-not-diagnosed: an illegal first BLOCKS line + corrected later lines applied partially), persist narration gaps (deck59: returns not narrated, "both die" ignores the persist return), and a wither-under-first-strike annotation clause (deck27: "(wither shrinks it to 0/0)" is false when the first striker kills the withering wall before it deals damage). (iv) Surface-unexercised carry-forward (deck109 observation): a seat can go a full 6-game corpus without a mulligan/marginal hand — do not read an untested mulligan/bottom surface as "confirmed clean"; carry it forward as unexercised.

## Superseded wave-23 synthesis notes (retained only as lineage)

## Synthesis notes (wave 23) — NINTH per-deck-rotation cycle; pool CHANGED (4 stayers 27/102/109/140 + 3 rotated-in: 137 GW Adventures GUIDELESS Step-0 + 133 mono-B Devotion / 131 UR Spellslinger = the roster's LAST two guided-never-cycled decks); the wave-23 step-1 batch (forced-self-discard FORCED-LOSS framing + R-DFC-FLIP flip-annotation/castable-faces + Echo INDEX-WINS + ATTACK-line CoT-hijack fix + the FIRST-EVER sampling pilot repetition_penalty=1.05) got its live validation; fallbacks 1.6% (20/1237) — 10 empty_reply (a transient corpus-start endpoint outage, PROVEN not pilot-caused) + 4 unparsed (3 decode-collapse garbage + 1 out-of-range conflation, all parser-correct rejections) + 3 retracted_choice + 3 deferred_to_heuristic (by-design convoke defers); core (general prompt) 18th PASS.

MERGE THIS WAVE (convergence-weighted): the load-bearing merge is a 3-SEAT-CONVERGENT expansion of the CRUTCH-LIFECYCLE — three independent seats sharpened the terminus from three complementary angles, so it is adopted as a durable body rung (see the Wave-23 refinement appended to the crutch-lifecycle terminus in Step 0). Everything else is a TAGGED single-seat reviewer-method or Step-0-bis rung, adopted with an explicit promotion threshold. No core-prompt change (18th PASS). The repetition_penalty=1.05 pilot verdict is a decode/sampler synthesis output, NOT skill text.

**Method headline 1 — CLASSIFY THE ENGINE SURFACE *BEFORE* APPLYING THE CRUTCH-LIFECYCLE TERMINUS; the demote/retire criteria fork by (i) FACT-vs-JUDGMENT surface, (ii) information-supplying-vs-belief-correcting crutch, and (iii) oracle-truth-vs-engine-behavior clause (3-SEAT CONVERGENT: deck133 + deck109 + deck102).** The wave-22 terminus was stated one-directionally: "when the engine now SHOWS a fact the guide narrated, DEMOTE/RETIRE the crutch." Three seats this wave, from three angles, showed the terminus needs a CLASSIFY-FIRST step because it fires differently by kind:
- **(deck133 — FACT vs JUDGMENT surface split.)** A new engine surface is a FACT surface (a value/target/cost/count previously computed in the guide) → DEMOTE/RETIRE per wave-22; OR a JUDGMENT surface (a generic heuristic/nudge rendered on the prompt: "pick the least valuable," "block the biggest threat"). A JUDGMENT surface RAISES THE FLOOR generically → do NOT add a guide rule duplicating it (method-1a anti-pattern), BUT audit whether the deck has a SPECIFIC card/line the generic judgment gets wrong or cannot see, and ADD that deck-specific EXCEPTION. It COEXISTS with the engine framing rather than being demoted by it (deck133: the forced-self-loss framing raised the floor; the ADD is "Bloodghast is uniquely safe to give up — it recurs on your next land," a deck-fact the generic "least valuable" prior cannot access). So rotation-in = new-leak hunt (headline-2 of wave-22) sharpens: the new-leak a JUDGMENT surface exposes is the deck-specific EXCEPTION, not a missing rule.
- **(deck109 — information-supplying vs belief-correcting crutch, with a 2-CORPORA retirement bar.)** The CLOSED-criteria (failure→0 AND the durable annotation renders) safely RETIRE a crutch that supplied MISSING information (a hidden outcome, hybrid payability, an absent-signal note) — once the engine renders the fact, the model has the input it lacked. A crutch that CORRECTS A WRONG PRIOR BELIEF the model holds independently of the display (a cost it mis-recalls, a rules interaction it mis-applies) is different in kind: rendering the correct fact is NECESSARY but NOT SUFFICIENT — the model can (and stochastically does) reason from memory and ignore the render ("belief overrides display"). Such a crutch needs a STRONGER bar: failure stays 0 across at least TWO corpora WITH THE BELIEF-TRIGGERING SHAPE PRESENT. On a recurrence, RESTORE it — lean and sharpened to name the specific observed conflation, not the old verbose lecture. Worked example: deck109's Pillar-{R} cost retirement (an information/belief-correcting crutch) REGRESSED round-2 (vs137 s12: declined an affordable Pillar reasoning "Pillar requires {1}{R}," conflating it with Searing Spear's cost, ignoring the correctly-rendered `Cast Pillar of Flame {r}`); restored as a lean line naming the Pillar-{R}-vs-Spear-{1}{R} conflation. The hybrid-mana crutch retired ALONGSIDE it (information-supplying) HELD cleanly — same corpus, same seat, opposite outcomes, isolating the variable.
- **(deck102 — oracle-truth vs engine-behavior narration split; the demotion's now-FALSE-clause audit gains a fork.)** When you DEMOTE a crutch, the wave-22 rung says audit its text for clauses the fix made FALSE and delete them. deck102 adds a fork: SEPARATE the crutch's ORACLE-truth clauses from its ENGINE-BEHAVIOR-narration clauses. DEMOTE/trim the engine-behavior narration (the fix now carries it), but do NOT rewrite an ORACLE-truth clause to chase a SUSPECTED engine BUG on a single observation — route the bug to notes, keep the guide oracle-accurate-but-GENERIC, and only correct the oracle clause once the engine's actual behavior is confirmed durable. Worked example: the Tergrid crutch mixed a mechanical "the flip toggle does nothing / cast a face" (engine-behavior → DEMOTE, the DISPLAY-TOGGLE annotation carries it, misplay 11→1) with "the Lantern is a repeatable {T}: drain" (oracle description); this wave surfaced R-DFC-BACKFACE-RESOLVES-FRONT (casting the Lantern back-face deploys the God front-face), so the oracle clause may not match what the engine executes — softened to "Lantern = cheaper face" pending the engine answer, NOT rewritten to assert the bug. Guides describe correct PLAY; engine bugs get fixed in the engine, not encoded into the guide from one game.

**Method headline 2 — ATTACKER-SCORING NEEDS TWO DIFFS NOW: execution fidelity AND decision-input fidelity (deck131, single-seat, GAME-DECIDING; promotion threshold named).** Wave-22 headline-4 (declared-set vs recorded-set for every window) catches UNDER-RECORDING — a valid coded line dropped to fewer attackers (execution fidelity). deck131 vs27 s15 exposed a DIFFERENT failure the first diff misses entirely: the DECLARED set was faithfully recorded, but the DECISION rested on a FALSE board read — the model wrote "Vodalian Zombie and Lord of the Undead are both tapped" and swung its only Guttersnipe, but the prompt's `[tapped]` tag was on Vodalian ONLY; Lord of the Undead was untapped, blocked, and traded away the deck's only reach. A `chosen_text` glance and a swing-was-declared check both MISS this. So when a pilot declares an attacker with a "it is safe, the opponent has no untapped blockers" rationale, DIFF the model's claimed tapped-set against the prompt's literal `[tapped]` tags on the opponent-battlefield line — never accept the rationale, never infer safety from the fact that the swing was declared. Attacker-scoring now runs TWO diffs: (1) declared-set == recorded-set (execution fidelity), and (2) claimed-board-state == actual-board-state (decision-input fidelity) for any "this swing is safe because X" rationale. Guide-lane corollary (applied at deck131): audit every "protect the keystone permanent" rule for ONE-SIDED coverage — a lone reach source / lone finisher needs never-block AND never-attack-into-a-possible-trade, and because the trigger is a tap-tag misread, the attack-side guard should instruct a PER-CREATURE `[tapped]`-read ("a creature printed without [tapped] can block, even if another creature on the same line is tapped"). Promotion threshold: a 2nd seat showing a per-creature tapped-state misread promotes the diff to a standing attacker-scoring step and the tap-tag read to a cross-seat combat-safety guide pattern.

**Method headline 3 — CLASSIFY A FAILED REPLY DECODE-COLLAPSE vs COMMIT-LOCK BEFORE ROUTING IT (deck27 reviewer instrument; single-seat producer, but a general routing aid).** Two distinct failure classes share the "answer-first didn't save it" surface and need different OWNERS: **commit-lock** (wave-22) = a WRONG coded answer emitted on line 1, self-corrected in later prose, truncation freezes the wrong snap — THERE IS A CODED LINE → owner GUIDE (make the snap right) / possibly PARSER (prefer last coded line on non-truncated replies); **decode-collapse** (wave-23, deck27 vs137 s12-14) = pure degenerate token-repetition with NO coded line at all → unparsed_reply — answer-first cannot save it (nothing committed), the parser is CORRECT to reject it (nothing to recover) → owner MODEL/DECODE-SAMPLING (rep_penalty, max-tokens fast-fail), NOT parser, NOT guide. The reviewer test is one question: does the failed reply contain a coded line (CHOICE/ATTACK/BLOCKS)? YES → commit-lock family (guide/parser). NO → decode-collapse (sampling/harness). Mis-routing a collapse to a parser change wastes engine work on a non-parser fault. (This interacts with the pilot verdict: rep_penalty=1.05 did NOT prevent the deck27 collapse it was theorized to fix — the classification keeps the collapse owned by the sampler ledger.)

**Method headline 4 — Step 0-bis rule 3 gains a FIFTH idiosyncrasy face: the ALTERNATIVE-COST DUAL-OPTION card whose alt-cost option carries the OTHER face's stats (deck137, tagged single-seat).** See the rule-3 body addition in Step 0-bis. Distinct from the four existing faces because the fact is MISRENDERED, not hidden (the option shows a REAL (P/T), just the WRONG face's), so the model is actively MISLED — closest sibling is the FOURTH (symmetric-cost) face where the number is right but its meaning is misread. Evidence: deck137 Adventures (deck131 s15 confirmed wrong outcome — cast an adventure bounce spell believing it was a 3/1 body, self-bounced its own flyer; deck27 s19 a 9.3k tax talking past the label). Authoring move matches the rung's doctrine (annotate/teach, never suppress): a per-FACE capability table + the corrective fact "the (P/T) on the alternative-cost option is the creature you get LATER from exile, NOT what you deploy now." Tag single-seat; promote to a named rule-3 face if a second alternative-cost-dual-option deck reproduces the misfire.

**Method headline 5 — fixing the REPRESENTATION defect on the path to a keystone can unblock the keystone IN THE RANDOM POOL too — a probe deck is the guaranteed path, not always a required one (deck102).** The wave-22 probe-deck/keystone-sequencing rung said a rare keystone blocked by a representation defect needs the defect fixed BEFORE probing the seam. deck102 VALIDATED the mechanism WITHOUT a probe: once batch (b) fixed the DFC-flip representation, the random pool's single Tergrid draw converted all the way through cast → steal-seam fire in ONE game (vs27, 0/18 → cast → steal fired → won). One-line addition to the probe-deck rung: a probe deck is guaranteed, but a clean representation can let the random pool reach the keystone on its own.

## Superseded wave-22 synthesis notes (retained only as lineage)

## Synthesis notes (wave 22) — EIGHTH per-deck-rotation cycle; pool CHANGED (4 stayers 135/62/27/102 + 3 rotated-in older-guided decks 109 Mono-R Aggro / 44 Faerie Tempo / 140 RBW Control re-validating multi-generation-old guides under the modern engine); the wave-22 step-1 batch (ENGINE-F1 fetchland-fizzle fix + HARNESS-N9 reference-scoping + echo (c) source-prefix/label-filter fixes + ENGINE-R6 transform-eligibility annotation + R-PAINLAND self-damage surfacing) got its live validation; fallbacks 0.6% (8/1368), 0 unparsed / 0 empty; core (general prompt) 17th PASS

Unified from seven independent deck-agent reviews mining the wave-22 PRIMARY corpus
(`matchups-20260723-173843`, 21 games round-robin over 135/62/27/102/109/44/140, binary `/tmp/wagic-1d8ed8194`).
EIGHTH per-deck-rotation cycle; the pool SWAPPED (49/35/14 rotated out to canary last cycle; 109/44/140 rotated IN as
old-guided re-validations), so cross-wave record comparison is WEAK evidence this cycle (pool-swap changes matchup
difficulty for everyone). **deck102 (Tergrid)** revised round 2 (two edits: painland-crutch DEMOTE + Tergrid-DFC
awareness ADD — already deployed live) and STAYS; **deck27** revised round 3 (one +6-line free-kills-first block-
allocation edit) and STAYS; **decks 135/62 FROZE** byte-identical; the three rotated-in seats **44** (crutch-demotion
+ Cruel-Edict clause), **109** (crutch-RETIREMENT: hybrid + cost annotations now engine-carried), **140** (crutch-
demotion + a NEW forced-discard rule) revised their aging guides. Binary 1d8ed8194 carried the wave-22 step-1 batch;
THIS CORPUS is that batch's live validation. Fallbacks **0.6% (8/1368)**: **2 retracted_choice (BOTH correct fires —
deck102 owns both, real self-disavowals), 6 stale_echo, 0 unparsed, 0 empty.** 9th consecutive clean corpus (0
segfaults, 0 timeouts, 0 draws, 8 life-adjudicated at cap — audited legitimate). Core **PASS, 17th consecutive** —
every finding routes BELOW core (per-deck guide) or to the engine/harness/decode ledger; the one core-filed proposal
(deck109 commit-and-stop nudge) is judged decode-lane, and the top new item (forced-self-discard framing) belongs at the
REPRESENTATION layer (its home is the same as the reveal-framing header — engine, not core prose). See
`synthesis-notes.md` for the per-seat table, the wave-23 engine ledger, and the reconciliations.

**Method headline 1 — the CRUTCH LIFECYCLE MATURES ACROSS A 4-SEAT CONVERGENCE (44/109/140/102); the demotion/retire
terminus is confirmed at EVERY representation-fix shape (option-line annotation, marker-string, CAUTION-line +
battlefield-tag, printed `(creatures: N)` count), and gains four sharpenings — the wave's strongest convergent method
theme.** Wave-21 wrote the terminus's second half (RETIRE if the rule is gone, DEMOTE if intact-and-engine-carried); this
wave four seats independently exercised it and refined it: (a) **DELETE clauses the fix made FALSE, not merely redundant
(deck102).** A crutch that narrates the ABSENCE of a signal the fix now PROVIDES ("the Mana available line does NOT warn
you") is not a redundant tax — it is a LIE the model reads against the new representation in the SAME prompt (the CAUTION
line now sits right there), strictly worse than redundancy. When you demote a crutch, audit for such now-false narration
and DELETE it, do not paraphrase. (b) **SITUATIONS-section crutch for an engine-carried NON-DECIDING case → DELETE
outright; mechanics-section crutch → DEMOTE to anchor (deck44).** A SITUATIONS entry must earn its slot by having
DECIDED a game; an engine-carried case that never decided one fails that test twice over — different homes, different
terminus. (c) **the CLOSED-criteria generalize to ANY representation crutch — cost, mana, hybrid, reveal, trade — once
(i) the failure → 0 AND (ii) the durable annotation renders on the option line (deck109, hybrid-mana + pillar-cost
retired; a 2nd instance after deck102's Thoughtseize).** (d) **crutch-retirement PACING (deck135): do not retire a crutch
on the SINGLE corpus that first validates the engine fix it guarded — wait for a SECOND confirming corpus (the crutch is
the safety net if the fix regresses), and separate the still-TRUE half of the crutch from the fix-specific half (retire
only the latter).** Confirmed the DEMOTE terminus fires the same on the CAUTION-line/battlefield-tag shape (deck102
painland) as on the marker-string and option-annotation shapes.

**Method headline 2 — ROTATING A CONVERGED DECK BACK IN AGAINST A CHANGED POOL IS A NEW-LEAK HUNT, not a re-validation
of the obeyed lines (deck140, strong single-seat + validated by the rotated-in cohort's structure).** "Proven against
pool P" is not "proven": a frozen guide's coverage is exactly the union of its PAST opponents' threats, so a pool swap is
a fresh adversary set the guide is UNDEFENDED against. deck140 (converged 6 corpora ago) found 100% of its real guide
work in the ONE seam the old pool never exercised — Tergrid's forced self-discard, for which it had no rule; it pitched
its two live sweepers and lost a game it was winning at 23 life. Re-confirming the obeyed lines was near-zero-value (a
changed pool cannot un-prove them, only change matchup difficulty). The rung: on rotation-in, DIFF the new pool's decks
for mechanics ABSENT from the seat's prior corpora (forced discard/sacrifice, mill, specific evasion, lifegain, combo)
and point attention THERE. Companion sharpening (deck140 rung 2): a long-FROZEN guide is where DEMOTION DEBT accumulates
— every representation fix that shipped while it sat frozen is un-applied demotion debt; on rotation-in re-audit every
belief-kill against what the representation LITERALLY surfaces TODAY (a printed count dissolves "how many" beliefs but
NOT "how the spell interacts" — demote per-belief, not uniformly). And (deck102) SEQUENCE THE PROBE: when a keystone seam
sits behind a representation defect (Tergrid steal behind R-DFC-FLIP), fix the defect on the PATH first — probing the
keystone through a defect the model thrashes on tests nothing.

**Method headline 3 — VALIDATE A FALSE-POSITIVE-SUPPRESSION FIX ON THE SHAPE STILL BEING PRODUCED, not on its absence;
shape-present-and-passing is STRONG evidence, shape-absent is WEAK (deck62, the back-half of wave-21's headline-1).**
When a prior wave logs a safety-route OVER-FIRE and the next batch ships a scoping fix, "the fallback didn't recur" is
the WEAK form — a zero-fire count is consistent with two worlds: the fix works and correctly no-fires on the trigger, OR
the triggering SHAPE simply didn't occur (the detector was never re-tested). The discriminator: CHARACTERIZE the
triggering shape mechanically (so you can grep it independently of any fallback flag), then CONFIRM the shape is PRESENT
and PASSING. deck62 produced the HARNESS-N9 shape (one first-line CHOICE + a downstream "Wait,"/"Actually," self-
correction, no contradictory second coded line) 27× this corpus — 10 in the EXACT wave-21 land-drop FP context — and
every one parsed clean: the detector was re-exposed to its own trigger and correctly no-fired. Had the shape been absent,
the honest write-up is "fix UNTESTED at this seat (shape did not recur)," explicitly weaker, never "validated." Two
corollaries: grep the trigger's PHRASE FAMILY, not the one literal token that happened to appear in the prior corpus
("Correction:" never recurred; "Wait," did 27×); and route validation to the WITNESS seat whose behavioral signature
REPRODUCES the trigger, holding it ONLY until confirmed on the live shape, then rotate (discharge-of-named-keep-reason).

**Method headline 4 — SCORE AN ATTACKER DECLARATION BY DECLARED-SET-vs-RECORDED-SET FOR EVERY WINDOW, regardless of
declaration FORM; the pure-INDEX form is NOT safe-by-construction (deck109, falsifies a wave-13 premise).** Wave-13's
Ledger-A scored only mixed/name attack forms for silent drops, on the premise that a full-index `ATTACK: A1, A2, A3, A4`
"parses correctly." This wave falsifies it: a clean pure-index line under-recorded to 2 attackers when the reply spiraled
to max_tokens and a prose-salvage arm overrode the answer-first line (vs62 s21). The tell is answer-first-line-set !=
recorded-set — a window-count or a `chosen_text` glance both MISS it (chosen_text shows the recorded subset, internally
consistent). Compare the coded answer-first line's declared set against the recorded set (`chosen_text` / next-record
combat log) for every window, not only the mixed/name forms. (The mechanism — a combat prose-intent salvage overriding a
valid coded line — is an ENGINE/harness integration-order candidate, routed to the ledger for code verification, NOT a
guide/skill line.)

**Method headline 5 — ANSWER-FIRST COMMIT-LOCK: for a decision kind where the SNAP first-line answer is easy to get
wrong, the GUIDE must front-load the correct heuristic so the SNAP is right — do NOT rely on last-well-formed-wins to
rescue it, because a truncation can pre-empt the corrected line (deck27, 1st HARMFUL instance; guide-authoring rung,
parser/decode side is a cross-seat WATCH).** Answer-first's floor property (a truncation-safe first draft) INVERTS when
the snap answer is wrong and the completed reasoning would have fixed it: deck27 vs109 s15 committed a lord-chump misplay
on line 1, reasoned correctly that the chump was bad, then TRUNCATED before emitting the corrected BLOCKS line — the
parser locked the snap misplay. This is the harmful twin of the benign rescues (deck62 vs27 s16 snap-chump → reasoned
`none`, last-wins took it; wave-21 vs35 s18). The GUIDE-lane fix at the layer answer-first actually commits: front-load
a free-kills-first ALLOCATION rule so the multi-attacker blocker snap gets right the FIRST time (deck27 did this). The
PARSER/decode side stays a WATCH — promotion threshold is a 2nd HARMFUL seat (single-seat now; do not open a parser
change on N=1), and the decode-time repetition/length guard (repetition_penalty pilot) is the real close, since the
harmful lock only occurs when the repetition spiral TRUNCATES the reply before the correction.

**Engine/harness/card-script items DO NOT live in this skill (layer-routing) — they live in each wave's
`synthesis-notes.md` ledger.** The wave-23 top items (reviewer awareness only): forced-self-discard rendered with the
detrimental-INVERTING generic target template (deck140 game-deciding + deck102 spiral, 2-seat convergent — the corpus's
top new item; fix = a detrimental-self-choice framing header at the REPRESENTATION layer, analogous to reveal-framing);
R-DFC-FLIP DFC face-toggle blocks the Tergrid keystone (deck102); the ECHO/stale_echo FAMILY is NOT closed by the (c)
fixes — 3-seat label-mismatch convergence (deck140 ANNOUNCE_X `Cast <spell> with X=N`, deck135 transform dual-face-name,
deck102 DFC invented-verb) argues an INDEX-WINS root fix (trust an in-range numeric CHOICE index on an uncontested
first-line answer regardless of the trailing label); the deck109 prose-salvage-override integration-order candidate
(FLAG, code-verify); the repetition_penalty pilot (now 2-wave convergent, wider evidence). CLOSED this wave: ENGINE-F1
fetch-fizzle (deck135, 10/10 clean incl. the exact same-turn-land-drop repro); ENGINE-R6 Azcanta transform annotation
(3/3 transformed, the multi-wave decline block broken); ENGINE-R1 reveal-partition (n=3); R-PAINLAND self-death class
(deck102, PASSED at 4 life instead of self-tapping); HARNESS-N9 over-fire (deck62 witness, 27× shape re-exposed clean).

## Superseded wave-21 synthesis notes (retained only as lineage)

## Synthesis notes (wave 21) — SEVENTH per-deck-rotation cycle; pool UNCHANGED (135/62/49/35/14/27/102, 7-of-7 repeat = a clean repeated-opponent control); the wave-21 engine batch (answer-first reply protocol + parser hardening + reveal-framing + wither-annotation + attacker-sparing) got its live validation; fallbacks 4x down to 0.7% (0 unparsed, 0 empty) but the composition CHANGED to a new parser-safety-route over-fire class; core (general prompt) 16th PASS

Unified from seven independent deck-agent reviews mining the wave-21 PRIMARY corpus
(`matchups-20260723-084938`, 21 games round-robin over 135/62/49/35/14/27/102, binary `/tmp/wagic-3be7f5a67`).
SEVENTH per-deck-rotation cycle, pool identical to wave-20 — so every record delta isolates to draws/matchup on a
repeated control. **deck102 (Tergrid)** played its FIRST GUIDED corpus (Step 0-ter → three surgical edits); **deck27**
round 2 (byte-frozen — its wave-20 revision deployed and held); **deck14** round 3 (one DEMOTION diff); **deck49**
round 2 of its revised guide (byte-frozen — edits validated); **decks 135/35/62 FROZE** byte-identical. Binary
3be7f5a67 carried the wave-21 engine batch: **ANSWER-FIRST reply protocol** (emit the coded CHOICE/ATTACK/BLOCKS/PUT
line FIRST, then reasoning, then PLAN; parser order-agnostic), **parser hardening** (retracted_choice route; template-
placeholder drop; last-well-formed-CHOICE-wins), **prose-intent salvage** (combat), **reveal framing** (whose-hand +
pick-EXACTLY-ONE), **bounce-on-stack annotation/header**, **wither trade annotations**, **partial attacker-sparing**,
**mana-count integrity** (disabled producers uncounted; refused-click aborts the cast plan). **THIS CORPUS is that
batch's live validation.** Fallbacks **0.7% (8/1119)** — a 4x drop from wave-20's 3.0%, composition COMPLETELY changed:
**0 unparsed, 0 empty** (wave-20 had 29 unparsed), 5 retracted_choice (NEW deliberate safety route), 3 stale_echo; 0
prose salvages fired. 8th consecutive clean corpus (0 segfaults, 0 timeouts, 0 draws, 4 life-adjudicated at cap). Core
**PASS, 16th consecutive** — every wave-21 finding routes BELOW core (per-deck guide) or to the engine/harness ledger;
the parser/decode items stay at the harness + decode-sampler layers (do not double-layer into the core prompt). See
`synthesis-notes.md` for the per-seat table, the wave-22 engine ledger, and the reconciliations.

**Method headline 1 — a NEW PARSER SAFETY ROUTE must be AUDITED ADVERSARIALLY FOR OVER-FIRE (false positives) with
the same rigor a SALVAGE route is scored for correct no-fires; a fallback logged against a reply that CONTAINS a
correct, in-range, UNCONTESTED first-line coded answer is a FALSE POSITIVE owned by the HARNESS — not the guide or the
model (deck62 + deck135 + deck49, 3-seat; deck35 supplies the true-positive counter-example that fixes the
discriminating shape — the wave's core instrument addition).** The batch's `retracted_choice` route (a CHOICE
"retracted with no replacement" → heuristic) fired 5x corpus-wide and **4 of the 5 are over-fires.** deck62 owns 3
(vs27 s10, vs135 s5, vs102 s3): each emitted the correct `CHOICE: 1 (Play Forest)` on line 1 (answer-first working),
NEVER retracted it, and only used self-correction language ("Wait"/"Correction:") in the DOWNSTREAM PLAN body about a
FUTURE cast — one `^CHOICE:` line, no contradictory second, no disavowal. deck135's 1 (vs27 s26) is the same: a single
un-superseded `CHOICE: 2 (Cast Ice-Fang Coatl)` thrown away over a `Correction:` about *Islandwalk mechanics*. The
detector keys on retraction PHRASES appearing anywhere after the CHOICE, not on retraction OF the CHOICE.
**The discriminating shape (deck35 vs62 s18 is the ONE CORRECT fire):** the model emitted `CHOICE: 3 (Cast Heart)`
first, then reasoned to a DIFFERENT card and its PLAN concluded "Cast Fury" — a genuine disavowal of the choice with no
clean re-emitted CHOICE. So: fire `retracted_choice` ONLY when the retraction targets the CHOICE/decision itself AND no
valid CHOICE remains, OR a CONTRADICTORY second coded line is emitted (already handled by last-well-formed-wins); a
single well-formed in-range CHOICE with no contradictory successor must be TAKEN regardless of prose "Wait/Correction"
in the reasoning body. The instrument rung: **score every new safety route for false positives, and pick its WITNESS
seat by matching the route's trigger to a seat's documented behavioral signature** — the route triggers on
"self-correction-after-the-answer," and deck62's over-deliberation-of-a-settled-decision signature IS exactly that, so
deck62 was the predictable exposing seat (owns 3 of 5, all over-fires) and is the ideal fix-validator (bears on
rotation, headline 5). Harm this corpus was ZERO (all over-fires landed on land drops / on plays the heuristic argmax
happened to match) — but latent-consequential on a CAST/TARGET ask. Route: ledger HARNESS-N9.

**Method headline 2 — ANSWER-FIRST VALIDATION DISCIPLINE: "0 unparsed" is NOT sufficient proof a fix worked; four
distinct checks are load-bearing once answer-first is in play (deck49 + deck62 + deck35 + deck27, strong multi-seat —
the wave's most reusable audit method).** Answer-first put the coded line first, so long deliberations now PARSE instead
of truncating (0 unparsed this corpus vs 29 in wave-20). That mask makes naive validation unsafe. The four checks:
(a) **LENGTH-CHECK THE SEAM.** A still-oscillating co-firing spiral now parses, so a *failed* guide fix would hide
behind a 0-unparse count. The discriminator is reply LENGTH at the seam: deck49's develop-seam fork resolved in
719–1202 chars across 6/6 games (SHORT clean commits = the guide edit genuinely dissolved the trigger); the residual
tax in OTHER pockets stays LONG-but-parsed (5–13k chars = answer-first absorbing a still-live spiral). Always
length-check, or you credit answer-first for a guide fix, or miss a failed guide fix answer-first is hiding.
(b) **CHECK FIRST==FINAL.** For a reasons-then-revises model the SNAP first answer is often the WORSE option and the
model reasons to a better one, re-emitting a corrected coded line (deck62 vs14 s26 snap `CHOICE: 4` → reasoned
`CHOICE: 2`; vs135 s21 snap `BLOCKS: B2:A1` deathtouch-instinct → reasoned `BLOCKS: B2:A2` saving 8 life; deck27 vs35
s18 snap `CHOICE: 0 pass` → reasoned `CHOICE: 1 Put-in-Hand`). So "last-well-formed-CHOICE/BLOCKS wins" is
QUALITY-PRESERVING, not a mere parse convenience — a naive "take the FIRST coded line" reading would execute the worse
snap answer. Confirm the parser took the LATER reasoned line; distinguish this cleanly from the headline-1 over-fire
(the revise case emits a SECOND coded line; the over-fire has ONE coded line + prose self-correction).
(c) **RE-POINT THE INSTRUMENT parse-risk → token/latency cost.** Once answer-first removes a failure MODE, do not keep
scoring the old mode at zero and call it a win — the disease (the >12k-char decode-repetition spiral) is undiminished
(deck35 12.5k chars on a LAND DROP; deck27 vs135 s18 the 3rd-instance ahead-position repetition loop). Re-point the
metric to reply-length as a token/latency cost (p90 was 70s) and route it to the decode sampler (repetition_penalty is
shipped-but-OFF), never the guide — a guide off-case cannot reach a decode loop (the model quotes correct guide lines
mid-loop).
(d) **WATCH THE TRUNCATION-BETWEEN-SNAP-AND-CORRECTION exposure.** The structural residual of answer-first for a
revise-heavy model: a reply that truncates AFTER the snap answer but BEFORE the correction executes the worse snap
answer with no rescue. None truncated this corpus, so it did not bite — flag any truncated reply whose parsed (snap)
answer diverges from where its cut-off reasoning was heading (the answer-first quality-regression signature, distinct
from a parse regression). NEW consequence class (deck49 HARNESS-49a): the residual spiral no longer breaks the PARSER
but now trips a DIFFERENT gate — a stale_echo false-positive when a re-deriving spiral near-duplicates the prior turn's
PLAN. When auditing "did answer-first fix the tax," check whether the residual spiral now costs a decision via a
different gate.

**Method headline 3 — the CRUTCH LIFECYCLE closes: RETIRE or DEMOTE an interim guide crutch once its DURABLE
representation fix ships AND is confirmed fired+obeyed — the second half of the demotion terminus (deck14 demote +
deck102 retire, 2-seat).** The standing doctrine wrote the FIRST half ("teach the fact interim, fix representation
durably"); this wave writes the second. Two sub-cases, distinguished by whether the underlying rule is still TRUE:
(a) **DEMOTE (rule still true; engine now carries it at the decision point).** deck14's bounce-on-stack #1-leak had its
durable fix ship in the batch (CAST-option annotation `[this cannot target the spell on the stack]` + TARGET-menu
header), confirmed FIRED on 26 cast windows and OBEYED (25 declines; the 1 cast was a correct aura-fizzle bounce; the 1
menu picked the opponent's biggest threat; 0 self-bounces). Per the marker-string terminus (confirmed fired AND
obeyed → the interim guide defense is now DOUBLE COVERAGE / attention tax), deck14 DEMOTED the hoisted ~10-line
`#1 MISFIRE` procedure to ~3 lines — keeping the corrective FACT + the shipped marker STRING + the never-self-bounce
anchor, dropping the (1)/(2) walkthrough the engine now carries. The OPTION-LINE-ANNOTATION shape hits the terminus like
the marker-string shape: verify by (i) misplay-count → 0 AND (ii) a DECLINE-RATE signal on the annotated cast (25/26).
(b) **RETIRE (rule no longer describes a real prompt).** deck102's wave-20 Thoughtseize crutch explained a "top N of
your library" mislabel and warned against over-selection; wave-21 item (e) fixed the prompt to render "The opponent
revealed their hand … choose the ONE card." The crutch now describes a mislabel the model will NOT see and fights an
over-selection that no longer happens (0/2 over-picks) — so it is RETIRED and replaced with lean strategic content. A
crutch that outlives its defect is not neutral: it re-teaches a stale prompt shape and burns guide budget. The rung: at
each revision, audit every guide line written as an interim crutch for a representation defect; if the ledger shows
that defect CLOSED, retire (rule gone) or demote (rule intact, engine carries it). Confirmation-read for a discard-pick
reveal fix (companion to the deck135 partition lifecycle): CLOSED only when (i) over-selection → 0, (ii) the named card
leaves the opponent's hand exactly once in `events`, AND (iii) the framing names whose hand + pick-EXACTLY-ONE —
"0 fallbacks this wave" is NEVER the confirmation. This EXTENDS the Step-0 demotion terminus (see Wave-21 changelog).

**Method headline 4 — a chosen ACTIVATED ABILITY is scored by the STATE DELTA it produced (cost-paid AND effect-landed),
not by "the model chose it" — the reveal zone-outcome instrument abstracted one level to catch SILENT engine failures
(deck135, reinforced by deck102's reveal confirmation-read).** The reveal-seam rung said: score a `kind:reveal` by the
ZONE OUTCOME in the next record's `events`, never "did the model answer." This wave the same discipline caught a
different seam: the fetchland-fizzle bug (ENGINE-F1) is invisible to EVERY model-facing signal — the model chose the
fetch, its plan narrates success, no fallback fires, the reply parses clean — and is visible ONLY in the engine state
delta: a `Put in Play with <fetchland>` CHOICE followed by `Your <fetchland> life -N` with NO `<fetchland>: battlefield
-> graveyard` (sacrifice) and NO `Snow-Covered X: -> battlefield` (land). Portable rung: for ANY chosen activated
ability with a cost, score the seam by confirming (i) the FULL cost resolved (each cost event present) AND (ii) the
effect resolved (the intended state change appears) — a cost paid WITHOUT the effect, or an effect WITHOUT the cost, is
a fizzle the model cannot self-report. The tell that promoted it: a whole game (deck135 vs35) lost with the model
believing its mana was fine, detectable only by diffing chosen-action against produced-events. (deck102's reveal
confirmation-read — zone outcome + pick cardinality, NOT fallback count — reinforces the same "score by state delta,
not by the model answering" family.)

**Instrument / rotation-discipline additions this wave (measurement/observation, single-seat-acceptable):**
- **On a FROZEN-guide + UNCHANGED-slate seat, decompose a win-column swing by CHANGE-IN-ENGINE-BUG-INCIDENCE FIRST,
  before crediting guide/model/draw (deck135, extends wave-20's "a validated engine fix can move the record by ~0").**
  This wave's dual: an intermittent engine bug present in BOTH waves moved deck135 −3 (4/6 → 1/6) purely by striking
  more games. On a repeated-opponent control, for each flipped game diff the ENGINE state-delta health (here: did
  fetches resolve?) between the seat's win and loss games — the only win is the only game with resolving fetches; the
  three flips are the three worst fetch-fizzle games. Rung: a frozen-guide record swing is an engine-bug-incidence
  hypothesis FIRST, play-quality hypothesis LAST.
- **Rotation: candidacy is triggered by the DISCHARGE of the specific NAMED keep-reason, not by guide maturity; and the
  two rotation gates (guide-mod? new-work-signal?) can DISAGREE when the new-work-signal is HARNESS/ENGINE-lane rather
  than guide-refinement — when they disagree, NAME which gate holds the seat (deck35 + deck62 + deck49).** deck35's
  wave-20 keep-reason (the open non-issuance work its shape uniquely surfaced) DISCHARGED this wave (payment fix
  validated clean) → candidate; deck62's guide CONVERGED (byte-freeze earned) but it is held purely as the WITNESS for
  the N9 over-fire it uniquely exposes → state "guide = rotation-eligible; held only as harness-witness for N9"; deck49
  converged with ALL residual off-guide (harness/card/decode) → frozen-canary. This prevents two errors: recording a
  harness open-item as a reason the GUIDE isn't done (it is), and rotating out the best witness for a live harness
  defect before its fix validates. When a NAMED keep-reason discharges and nothing seat-specific replaces it, rotate —
  do NOT manufacture a fresh reason to hold a converged seat.
- **When a post-fix corpus validates a mechanism you previously REJECTED, say "my prior hypothesis is SUPERSEDED"
  plainly — not a soft "additive" hedge (deck35).** Wave-20 deck35 ran the instrument, saw the symptom persist, and
  named a "combat-gate non-issuance" bug, REFUTING the batch's payment attribution. Wave-21 the reframe (all-or-nothing
  sparing tapped the attackers; the gate was always correct) validated on behavior (0-attacker games gone). The
  instrument's credibility comes from letting it overturn your OWN prior call, not just the batch's.
- **The strongest validation of a payment/sparing fix is the MAXIMUM-mana-pressure POSITIVE case in a deck that LACKS
  the confound, not the absence of the symptom (deck35).** The clean proof was not "vs49 no longer 0-attacker"
  (confoundable — that game drew haste) but vs14 T8: a `{4}{g}`=5-mana 5-drop paid from all 5 lands, leaving THREE
  non-haste attackers untapped, all three then attacked. Hunt the highest-cost last-cast in a deck with NO alternative
  mechanism masking the symptom (here: no haste enabler) and read the post-cast tapped-state directly.
- **Score a named-threat DEPLOYMENT RATE as deployed/DRAWN, gated on the mana line, not deployed/GAMES (deck14, sharpens
  the wave-20 displacement rung).** deck14's Rule #1B looked to fall 4/6 → 3/6, but it was drawn in exactly 3 games and
  deployed in 3/3, each the turn it became castable — a falling deployed/games with a perfect deployed/drawn is DRAW
  VARIANCE, not the rule failing. Corollary: gate any "sat on the finisher" claim on the MANA line (vs35 Mahamoti sat
  from t1 but 6 mana wasn't up until t11 — a false positive the castability check dissolves).
- **An off-case revision may require MANY corpora to draw its trigger; do NOT delete or re-litigate an unexercised
  off-case for lack of validation (deck27, single-seat — HOLD, promote on a 2nd seat).** deck27's late-stall durdle
  revision has now gone TWO corpora without its walled-ground-stall trigger occurring (both times the deck drew evasion
  and won the grind first) — the expected behavior of a low-frequency off-case, not evidence it is wrong. Hold and keep
  re-flagging the trigger condition (bias the next opponents toward ground-wall decks if the harness allows).
- **Model-layer failures that prose cannot touch are sometimes best DEFUSED at the PARSER/harness layer (make the bad
  output harmless), not the DECODE layer (prevent it) (deck27).** The 3rd-instance ahead-position repetition loop was
  NOT fixable by a guide "keep it simple when ahead" line — but answer-first NEUTRALIZED its harm (decision locked on
  line 1 before the tail degenerated; 0 fallbacks). When a recurring decode-layer failure is already made harmless by a
  parser change, prefer NOT to open a new decode-rung campaign — track it, try the cheap knob (repetition_penalty), and
  treat the harm-defusal as the primary win.
- **stale_echo gains a FOURTH downgrade cause — a FORMAT-echo where the model prefixes the SPELL name on a TARGET menu
  (deck14 + deck62, 2-seat, PARSER-owned).** Both seats hit the same shape: `CHOICE: N (<spell> targeting <target>)`
  where N is the correct in-range target and the target name appears as a SUBSTRING of the parenthetical, but the
  leading spell-name qualifier broke the option-N name match → stale_echo → heuristic (deck14 vs27 s29 Unsummon; deck62
  vs14 s29 Web). Not a guide/matcher-loosening fix (the header already says "answer with the TARGET's name, not the
  spell") — PARSER robustness: when a `CHOICE: N (...)` has an in-range N whose option-N name is a substring of the
  parenthetical, prefer N. Route: ledger.

**Engine/harness/card-script items DO NOT live in this skill (layer-routing) — they live in each wave's
`synthesis-notes.md` ledger.** The wave-22 top items (for reviewer awareness only): ENGINE-F1 fetchland-fizzle (deck135,
game-deciding — a Tap+Life+Sacrifice fetch pays Tap+Life then aborts before the Sacrifice+search, silently; the
corpus's top new engine finding); HARNESS-N9 retracted_choice over-fire (deck62 witness); the stale_echo family
(spell-name-prefix on target menus + the deck49 near-duplicate-PLAN false-positive); ENGINE-R6 Azcanta transform
threshold-eligibility annotation (deck135); R-PAINLAND self-damaging-mana life-cost surfacing (deck102); CARD Ashenmoor
Liege 4/1-vs-4/4 primitive drift (deck49, Oracle-verify first per project rule); N8 baka blocker gang-chump
(code-shipped, still UNEXERCISED). ENGINE-R1 reveal-finalize CLOSED (n=2, deck135 vs14 s23); wither trade-annotation
CLOSED (deck27, item g); attacker-sparing PAYMENT fix VALIDATED CLEAN (deck35); bounce-on-stack representation CLOSED
(deck14, item f); item-8 counting/plan-abort (a)/(b) confirmed (deck102), residual (c) reclassified into ENGINE-F1.

## Superseded wave-20 synthesis notes (retained only as lineage)

## Synthesis notes (wave 20) — SIXTH per-deck-rotation cycle; one guideless newcomer (102 Tergrid) that produced an initial guide; one first-GUIDED-corpus validation (27 Zombies); three revised-guide re-validations (14 Deep Blue round 2, 62 Enchantresses round 4, 49 Dragons — rotation-HOLD discharged, now a revised seat); two veteran freezes (135/35); the reveal seam's wave-19 partition bug FIXED + validated at deck135; core (general prompt) 15th PASS

Unified from seven independent deck-agent reviews mining the wave-20 PRIMARY corpus
(`matchups-20260719-151849`, 21 games round-robin over 135/62/49/35/14/27/102, binary `/tmp/wagic-95e89c204`).
SIXTH per-deck-rotation cycle. deck110 rotated OUT last cycle (THIRD converged exemplar; its guide stays deployed
as a canary). **deck102 (Tergrid)** entered GUIDELESS (Step 0-bis → initial guide); **deck27 (Zombies)** played its
FIRST GUIDED corpus (Step 0-ter → surgical revision); **deck14** ran round 2 and **deck62** round 4 of their revised
guides; **deck49's one-cycle rotation HOLD discharged** (its flood off-case was finally exercised — PASSED on behavior)
and it re-enters as a revised seat (two constrained-diff hunks); **decks 135/35 FROZE** byte-identical. Binary
95e89c204 carried the wave-20 engine batch: **ENGINE-R1 reveal-partition fix** (`driveInteractiveReveal` finalizes
option-one picks same-tick — the wave-19 Glacial Revelation to-hand drop), attacker-sparing mana payment, and the
perception bundle (reveal ELIGIBILITY surfacing + combat `options_text` + `#N` parse-side ordinal disambiguation +
`salvageLoopedChoice` extended to BLOCKS/ATTACK/PUT). **THIS CORPUS is that batch's live validation.** Fallbacks
**3.0% (32/1067)** (29 unparsed, 3 stale_echo, 0 defer, 0 empty; 7th clean corpus, 0 segfaults). Core **PASS, 15th
consecutive** — the one 2-seat method convergence (co-firing-imperative fork) is a SKILL method rung, not a
general-prompt line; the losing-position / decode spirals it touches stay at the GUIDE off-case + decode-sampler
layers (do not double-layer into the core prompt). See `synthesis-notes.md` for the per-seat table, the engine ledger,
and the `#N` code verdict.

**Method headline 1 — CO-FIRING IMPERATIVES need an explicit precedence or an "either is fine — COMMIT" release
valve; a guide that hands the executor two rules that both fire on one board state with no tiebreak causes a
GUIDE-CAUSED decode/deliberation loop (deck49 + deck62, 2-seat, the wave's core method addition).** deck49's two-wave
zero-fallback streak broke on THREE unparses all at the same seam (early Main-1 develop turn, seq 8, lone-1/1 board),
each an echo-test-positive spiral in which the model QUOTES two guide sentences that pull opposite ways — "burn goes
DEFAULT to the FACE" vs "aim at a CREATURE walling your board," and "deploy a body every turn" vs "burn is your
clock" — and cannot COMMIT, deliberating past the token cap (one loops verbatim, one progresses, one collapses onto
the wrong menu). deck62's vs14 s26 is the same shape on defense: the model loops verbatim litigating edit B's
chump-block caution against the prompt's accurate "(you kill it, your blocker lives)" annotation. Per the Step-0 echo
test, a rule the executor CITES while looping is a guide bug. The authoring moves: (a) when two imperatives can co-fire
on one state, KILL the false branch — if BOTH answers are actually fine (deck49's deploy-vs-burn: both won on the
follow-on), do not legislate a universal winner (that trips the brake-absolutism trap); write "either is fine — pick
one and COMMIT," plus a soft board-state lean, and soundness-check the lean against what the recovered turns actually
did; (b) when the tension is a loud CAUTION vs an ACCURATE prompt annotation, the executor resolves toward the most
concrete clause and an accurate option-line annotation LOSES to a loud guide caution unless the guide explicitly cedes
to it — add a decisive EXCEPTION naming the adjacent case the caution must NOT touch, deferring to the annotation; (c)
tighten an over-broad EXCEPTION against the degenerate board it misfires on ("a key blocker walling your whole board"
reads as ANY blocker when your board is a lone attacker — scope it to the state where it is actually wrong). Bound the
confidence: the CARRIER is a decode loop below the guide layer, so a scope-sharpen REMOVES THE FUEL, it is not a
claimed fix — frame it like a nudge. Distinct from wave-19's off-case-condition-keying (that was one rule mis-scoped;
this is TWO rules with no order between them).

**Method headline 2 — HOIST a buried, passive, #1-leak line into a named TRAP is a legitimate turnaround-mode diff,
and is NOT the forbidden "third copy of a well-placed tell" (deck14, NEW rung).** deck14's "bounce cannot answer a
spell on the stack" line was buried at the end of a bullet, phrased passively — and its misplay ESCALATED wave-over-
wave (1 instance → 1 costly self-bounce misplay + 3 of 6 fallbacks, the seat's single biggest model-fixable cluster).
The standing doctrine ("present-and-disobeyed → representation/engine, do not restate louder") is right about the
DURABLE fix, but this wave sharpens the INTERIM: when the disobeyed line is (a) buried, (b) passive, AND (c) the deck's
#1 leak, a turnaround-mode constrained diff that hoists it to a prominent named trap with BOTH the upstream behavior
("do not cast the bounce to answer a stack spell") and the downstream menu fact ("the target menu is battlefield-only;
never self-bounce to reach the stack") is the FIRST prominent, actionable placement — not a redundant restatement.
The test for hoist-vs-leave: is the current line prominent+concrete+obeyed-sometimes (leave it, fix representation) or
buried+passive+never-landing (hoist once, still flag the representation fix as durable)?

**Method headline 3 — a TIER-2 clock-quality rule (deploy the finisher / swing the whole board) DOES cure the
slow-clock soft-durdle on the NEXT corpus — the wave-19 two-tier aggression method is now VALIDATED, not just
written (deck14, promote).** Wave-19's two-tier headline predicted that a model taught only the literal attack-FLOOR
mutates the durdle down one level (attacks trivially while the finisher sits in hand). deck14's Rule #1B ("be a REAL
clock: deploy your flyers, swing the whole board, race the mana to the finisher") MOVED the behavior: finisher
deployment 1/6 → 4/6, all three wins are finisher games, and the finisher is cast the turn it becomes available (no
sit-on-the-flyer). Generalizes: after shipping a tier-2 clock-quality rule, score DEPLOYMENT RATE of the named threat
and split "not deployed because not drawn" (displacement — vs135 never drew a flyer) from "not deployed while
castable" (the rule failing). Only the second is a rule failure.

**Method headline 4 — the parse layer has THREE states, not two: parsed / salvageable-mis-format / truncated-before-
any-line — and a salvage-extension NO-FIRE on a truncated-before-line reply is a CORRECT result, not a salvage miss
(deck14 + deck62, 2-seat, refines the wave-19 salvage rung).** The wave-20 batch extended `salvageLoopedChoice` to
BLOCKS/ATTACK/PUT; deck62's two blocker fallbacks and deck14's two blocker fallbacks are the live test, and it fired
at NONE — correctly, because all four truncated BEFORE emitting any coherent coded line (deck62 vs14 s26 had only
`BLOCKS: none`, the OPPOSITE of its stated intent — salvaging it would have executed the wrong answer). When
validating a salvage extension, first classify each fallback reply as (a) committed-but-mis-formatted (salvage's job),
(b) truncated-pre-commit (a decode/length problem BELOW the salvage layer), or (c) parsed-but-wrong; score the
extension only against class (a), and do NOT "improve" it to grab a contradictory literal token from a class-(b)
reply. Companion (deck49): do NOT extend loop-salvage to a genuinely-UNRESOLVED ask/target deliberation either — its
repeated line is the LOSING side of an argument the model talked itself out of (deck27 seq8 looped "bolt the creature"
then the follow-on correctly sent the burn face); salvage is safe for a format-mangled SINGLE intent, never for an
oscillation between two aims. The only reply-level net for the truncated-pre-commit class is an ANSWER-FIRST protocol
(emit the coded line, THEN reason) or a PROSE-INTENT salvage (recover the decision stated in the reasoning); the
decode-repetition half needs a decode-time repetition/max-token guard.

**Instrument additions this wave (measurement/observation, single-seat-acceptable):**
- **The reveal-seam PARTITION-TRACE completed its full lifecycle (deck135) — bug FOUND (wave-19), FIXED, VALIDATED by
  the SAME zone-outcome read.** ENGINE-R1 (Glacial Revelation's chosen snow permanents milled to graveyard) is fixed:
  vs27 s18, the corpus's ONE Glacial Revelation, put all four chosen snow permanents to HAND and only the two unchosen
  non-snow fetches to graveyard — exactly the Oracle partition. Method rung: a reveal-fix confirmation needs the
  zone-outcome read on the SAME card that broke (the `events`), NOT "no fallbacks this wave" — the fallback count went
  to 0 partly from the eligibility fix and partly from draw variance (n=1 cast). Hold the item open for a second-corpus
  confirmation (rare-event-power discipline); deck135 is the ONLY seat that exercises the reveal seam.
- **A per-option ELIGIBILITY annotation is verified by REASONING-TAX COLLAPSE at the annotated windows, not by a
  marker-string grep (deck135).** The eligibility-surfacing fix (`[eligible for "…"]` / `[does NOT qualify …]`) shipped
  and its success shows as Into the North reveals collapsing from ~12k-char "which are snow lands" spirals + 2
  fallbacks to short 421–609-char clean picks, 0 fallbacks, 3/3 outcome-correct. Score such an annotation CLOSED when
  (i) ineligible picks → 0 AND (ii) the derivation shortens at the annotated windows vs the un-annotated ones.
- **The DISCARD-PICK reveal is a distinct pick-CARDINALITY sub-class of the reveal seam — read the `<N>` token in the
  card's `auto=` and score OVER-SELECTION (deck102, NEW sub-class).** Targeted discard (Thoughtseize/Duress) routes
  through the same reveal-partition prompt as a library reveal but is a pick-EXACTLY-N effect
  (`target(<1>*[-land]|reveal)` = pick ONE). Rendered as choose-a-SUBSET it drives OVER-SELECTION (deck102 picked both
  Boomerangs, all 3 nonlands) — harmless ONLY when the true best card is the lowest eligible index, a LATENT quality
  bug otherwise. The prompt also mislabels the OPPONENT's revealed hand as "top N of your library." Defect is
  REPRESENTATION (→ ledger); the guide teaches the pick-ONE fact interim. Mechanical integrity CLEAN — the ENGINE-R1
  same-tick finalize is confirmed on this seam too (the lowest-index eligible pick leaves the hand exactly once).
- **VALIDATE-a-fix does not attribute the fix to the win column — check how many windows exercised it (deck135, deck35).**
  ENGINE-R1 is validated correct but Glacial Revelation was cast ONCE (a lost game), so it moved the record by ~0; the
  attacker-sparing PAYMENT fix works (deck35 vs49 seq8: Winged paid with two lands, Gemhide left untapped) but did NOT
  collapse the 0-attacker games. Use the repeated-opponent control (5 of 6 opponents identical wave-over-wave at
  deck135) to attribute a record swing to slate + variance rather than the batch. **And a validated engine fix can
  REFUTE the batch's own attribution:** the wave-20 brief attributed the 0-attacker durdle to payment; deck35 shows
  the would-be attackers were UNTAPPED, so payment was never the cause — the ROOT is the attackers-decision
  NON-ISSUANCE engine bug (now 3-wave convergent), which the payment fix does not touch. Do NOT close a durdle on the
  strength of an adjacent fix.
- **`#N` ordinal reconciliation — a PARSE-side feature is not a RENDER-side one; verify which surface a batch item
  actually touched before scoring it "working" (deck14 vs deck27/deck62, settled in code).** `AIPlayerGPT.cpp`
  (`nameOrdinal` + `uniqueNameMatch`'s `ordinal` arg, lines ~3020/3046/3484) HONORS a `#N` the MODEL appends to a
  reply segment to break a same-name tie; the engine renders `(P/T)` and combat A#/B# position codes onto menus but
  NEVER renders `#N` itself. So deck27 (vs49 s19) and deck62 (vs14 s36) "worked" on COMBAT surfaces where the A#/B#
  position codes and bare-name→multi-target mapping did the disambiguation ("no #N ordinal even needed") — NOT a test
  of the `#N` path; deck14's "no `#N` on any TARGET menu" is CORRECT and names the real gap (non-combat duplicate-name
  target menus have no position code and no rendered `#N`, only per-line `(P/T)`, which cannot separate two IDENTICAL
  same-name/same-stat creatures in different states). No conflict — different surfaces. Residual: no seat produced a
  forced identical-duplicate target SELECTION, so the parse-side `#N` is real but unexercised end-to-end.
- **The retracted-first-CHOICE HIDDEN loop — the fallback count UNDERCOUNTS the decode-loop tax (deck135, single-seat
  WATCH).** vs35 s10 spiraled 15k chars, converged in prose on "cast Icehide Golem," but had emitted an early
  `CHOICE: 4 (Cast nothing)` it then retracted ("Wait, I made a mistake") and never re-emitted; the parser took the
  retracted digit (`fallback: null`), logging the wrong action. When tallying the decode-loop tax, also grep
  clean-parsed records for a repetition signature + a self-retraction; the true count is fallbacks PLUS these. Adopt
  into the scoring rung on a second seat. (Routing: HARNESS — prefer the LAST well-formed CHOICE line.)
- **The 0-attacker durdle disqualifier list grows a THIRD item: "is the creature still YOURS?" (deck49).** deck14's
  4/4 Thunderbreak sat "idle" while the opponent was at 8 — but it was under opponent control (Persuasion). Before
  scoring a 0-attacker window as a durdle: (a) eligible untapped/unsick attacker? (b) attackers decision issued? (c)
  is the creature still yours? A stolen/controlled threat is not an attack you failed to make.
- **Attribute a fallback-path misplay to the HEURISTIC, not the guide/model, and check whether the heuristic makes the
  exact mistake a guide clause forbids (deck62).** Both deck62 blocker fallbacks ended with the baka heuristic
  gang-blocking a small attacker with the 0/1 Argothian engine when a single real blocker sufficed — the precise
  behavior edit B forbids, occurring BELOW the model's never-emitted answer where guide prose cannot reach. A guide
  clause obeyed by the model AND violated by the heuristic on the same class is NOT a guide failure; separate the
  ownership lanes before crediting or faulting the guide (→ engine ledger).

**Engine/harness/card-script items DO NOT live in this skill (layer-routing) — they live in each wave's
`synthesis-notes.md` ledger.** The wave-20 top items (for reviewer awareness only): attackers-decision NON-ISSUANCE
(deck35, 3-wave convergent, sharpened correlate — fires with haste or an explicit cast-nothing, skips when the last
spell is cast in main-1 with non-haste attackers); targeted-discard reveal MISREPRESENTATION (deck102, cross-deck);
wither trade-annotation "(both die)" wrong when the wither blocker's power < attacker toughness (deck27); bounce-on-
stack menu misconception (deck14); the decode/truncation guard (cross-seat class — repetition-penalty + max-token +
answer-first); parser retracted/duplicate-CHOICE lock-in (deck135 + deck62); baka blocker-fallback chumps engine
pieces (deck62); repeated cast-offer of an already-cast singleton (deck102, low confidence). ENGINE-R1 reveal-finalize
CLOSED (deck135 + deck102 cross-confirmed); combat `options_text` CLOSED (deck14 40/40, deck62 confirmed).

## Superseded wave-19 synthesis notes (retained only as lineage)

### (wave-19 header) FIFTH per-deck-rotation cycle; one guideless newcomer (Zombies); one first-GUIDED-corpus validation that produced a revision (14 Deep Blue); two revised-guide re-validations (49 Dragons, 35 Slivers, each round 2) plus one revised round 3 (62 Enchantresses); two veteran freezes (110/135); the GPT reveal seam went live at deck135; core 14th PASS

Unified from seven independent deck-agent reviews mining the wave-19 PRIMARY corpus
(`matchups-20260719-092058`, 21 games round-robin over 110/135/62/49/35/14/27, binary `/tmp/wagic-72b05535d`).
FIFTH per-deck-rotation cycle. deck21 rotated OUT last cycle (2nd converged exemplar). **deck27 (Zombies)**
entered GUIDELESS (Step 0-bis → initial guide); **deck14 (Deep Blue)** played its FIRST GUIDED corpus (Step
0-ter → produced a revision); **decks 49/35** ran their revised guides round 2; **deck62** ran its revised
guide round 3; **decks 110/135** FROZE byte-identical. Binary 72b05535d carried the wave-19 engine batch
(combat name tolerance `parseBlockAssignments` name→label second pass + ineligible-only ATTACK→none;
per-pairing TRADE OUTCOME block annotations; alt-cast PITCH surfacing; `salvageLoopedChoice`), the GPT REVEAL
SEAM (`Player::decideReveal`, 18 decisions ALL at deck135), and the suppression→annotation replacement.
Fallbacks **2.4% (27/1146)** (21 unparsed, 6 stale_echo, 0 defer, 0 empty; 6th clean corpus, 0 segfaults).
Core **PASS, 14th consecutive** — every core-adjacent proposal is single-seat method/instrument (cannot add a
core line) and/or an engine/representation item, and the losing-position spiral it touches stays at the GUIDE
off-case layer (do not double-layer). See `synthesis-notes.md`.

**Method headline 1 — write an aggression countermand in TWO TIERS (deck14, NEW method).** A first guide's
attack-FLOOR rule reliably kills the 0-attacker durdle — deck14's model literally quotes Rule #1 verbatim
before attacking (vs49 s28) — but the durdle MUTATES one level down into a SLOW-CLOCK soft-durdle: the model
satisfies the literal floor (declares an attacker) while its finishers sit in hand and the opponent's life
barely moves (flyers deployed in only 1 of 6 games; vs110/vs135 pokes with a lone small body and loses).
A model that learns to satisfy the literal floor will satisfy ONLY the literal floor. State the aggression
rule at the altitude of the WIN CONDITION, not the combat step: declare attackers AND deploy the real threat /
race the mana to the finisher / swing the whole board. Applies to any deck with a distinct finisher behind a
cheap opening (aggro-with-a-top-end, evasive-finisher tempo, reanimator-with-a-bomb).

**Method headline 2 — the 0-attacker durdle instrument must separate MODEL-durdle from ENGINE-NON-ISSUANCE
(deck35 S1, the wave's sharpest instrument refinement).** The instrument counts "0-attacker games" as a durdle
proxy. This wave deck35's one 0-attacker game (vs135, adj LOSS) had an explicitly aggressive stated plan,
eligible untapped/unsick attackers, and the engine simply NEVER ISSUED a declare-attackers decision — the same
non-issuance recurred in a WON game (vs27) costing only tempo. Naively that reads as an FC1 durdle relapse; the
cause is an ENGINE gap. RUNG: before scoring a 0-attacker game as a durdle, check two disqualifiers — (a) did
the seat have an eligible untapped/unsick attacker that turn, and (b) was an `attackers` decision actually
ISSUED? A 0-attacker game with an eligible board and no attacker decision belongs in notes.md (engine ledger),
not against the guide. The instrument measures OUTCOME; attributing CAUSE requires the eligibility-and-issuance
check. Without this, an engine bug is logged as a guide regression.

**Method headline 3 — the combat-answer FORMAT-DRIFT rung graduates from single-seat WATCH to VALIDATED
(deck14 before/after).** Wave-18 flagged the combat format-drift tax as a single-seat watch (3 of 5 fallbacks
were 8.6-13.1k-char combat rambles that drifted out of the coded format). Wave-19, with the guide's Rule #2
short-answer + code-format discipline line in place AND the engine's combat name-tolerance batch alongside, the
class DIED: combat replies collapsed to 300-800 chars, and the two residual tax spikes (vs35 s26 8307, vs49 s28
4492) both PARSED to clean coded lines instead of falling back. For ANY derivation-heavy-combat deck (flyers +
lords/anthems + recursion), ship the short-answer + code-format rung as a standard line; expect it to convert
the tax from a FALLBACK source into a merely-verbose-but-PARSED decision. The engine parser tolerance is the
belt; the guide line the suspenders — they compound.

**Method headline 4 — key the off-case branch on the CONDITION, not the STATE (deck62 #2, refines the wave-18
promoted off-case rung).** The wave-18 anti-spiral line was authored from behind-at-low-life spirals and scoped
"WHEN YOU ARE BEHIND." Wave-19 shows the SAME lethal-hunt pathology (re-deriving the board hunting exact
lethal-this-turn, to truncation) firing while the model is AHEAD (deck62 vs135 s32 at 18/13 with a winning
board → unparse) — while the behind-case the line was written for now resolves correctly (deck49 at 2 life
parses to the right survival play). An off-case keyed on a board-STATE proxy leaks the moment the behavior
appears in the opposite state. Key it on the DECISION CONDITION the behavior actually tracks: "you cannot reach
lethal THIS turn — ahead OR behind." Sibling to the wave-18 name-the-trigger lesson (bind the rule to the
specific WANT / CONDITION, not the mood-proxy that co-occurred in the first corpus).

**Instrument additions this wave (measurement/observation, single-seat-acceptable):**
- **Reveal-seam PARTITION-TRACE (deck135, found the wave's top engine bug).** For any deck that reveals, score
  the ZONE OUTCOME — the next record's `events` ("Your revealed X goes to <zone>") — never just "did the model
  answer." A seam's DECISION-SURFACING can be fixed (the model now drives the choice) while its RESOLUTION
  stays broken. Separate three look-alike failure modes: (a) RESOLUTION bug (engine acknowledges the pick then
  moves it to the wrong zone — the Glacial Revelation to-hand drop); (b) ELIGIBILITY gap (model picks cards the
  restriction forbids because it was not surfaced — the Azcanta activation); (c) RENDERING misdirection (the
  effect is mislabeled — Into the North search shown as a choose-subset-to-hand menu). Only (a) is an
  engine/card-script bug; (b) and (c) are representation.
- **Bucket truncation-unparses by MECHANISM before proposing a fix (deck62 #3 / deck135 / deck27).** Read
  whether the reply's tail REPEATS (decode-repetition loop — a model/decode pathology, route to a decode-time
  repetition-penalty / max-token guard, NOT guide, NOT token-budget) or PROGRESSES (advancing reasoning-spiral
  — the only kind a guide off-case branch can reach). The model even quotes the correct guide line inside a
  decode loop and loops anyway. Extends the wave-18 repeat-loop signature with a second fork: some truncations
  are neither behavior nor budget.
- **Score a representation annotation on TWO axes: correct AND obeyed (deck62 #4 / deck27 #4, 2-seat
  converging).** A trade/warning annotation that is accurate but IGNORED is a guide/behavior gap, not an
  annotation defect — route the residual to the GUIDE (e.g. a defensive protect-the-engine clause), never to a
  change in the accurate annotation. Evidence: deck62 chump-blocks its 0/1 engine against the "[deals 0 — only
  absorbs damage]" tag; deck27 declines a FREE "(you kill it, your blocker lives)" block. Mirror of the wave-15
  "count and impact are different measures."
- **Annotation SCOPE + mask-lift discipline (deck49).** (a) A class recorded "masked, re-test when the mask
  lifts" MUST be re-tested on the corpus where the mask lifts, and the verdict is valid only then (C4 /
  enemy-only War Paint closed exactly as scheduled once the suppression→annotation swap shipped). (b) Before
  trimming a strategic rule as "redundant with an engine annotation," check the annotation's SCOPE — a
  conditional annotation (fires only on the enemy-ONLY menu) leaves the complementary case (mixed menu) to the
  guide rule, so it is NOT a trim candidate. Apportion by the annotation's ACTUAL trigger surface, not the
  class name (boundary condition on the wave-18 concurrent-engine-fix apportionment). (c) A correct decision
  under an outcome annotation is NOT evidence the underlying card-script stat is right — the annotation masks
  stat drift at the decision surface (Ashenmoor Liege engine 4/1 vs real 4/4, "(both die)" rendered correctly),
  so verify stats separately against the Oracle queue.
- **Verify the focus surface is LOGGED at your seat before reporting (deck14 Proposal 4, reviewer discipline).**
  When a brief assigns a decision-surface validation, first confirm the surface is logged; if not, say so with
  the field name. An observability gap is a coverage HOLE, not a clean pass (deck14's empty combat
  `options_text` defeated its TRADE-OUTCOME assignment — reported honestly, not silently skipped).

**Engine/harness/card-script items DO NOT live in this skill (layer-routing) — they live in each wave's
`synthesis-notes.md` ledger.** The wave-19 top items (for reviewer awareness only): ENGINE-R1 Glacial
Revelation to-hand partition drop (card-script, game-affecting); attackers-decision non-issuance (2-game
convergent); combat-reply parse robustness (salvage BLOCKS/PUT/prose + partial-valid recovery + "(P/T) #N"
suffix strip, one DECISIVE dropped block); combat `options_text` observability gap; reveal/search eligibility
surfacing. The wave-18 block-echo name-format item CLOSED (name-tolerance batch shipped + validated at every
bleeding seat).

## Superseded wave-18 synthesis notes (retained only as lineage)

### (wave-18 header) FOURTH per-deck-rotation cycle; one guideless newcomer (Deep Blue); two first-guide validations that produced revisions (49 Dragons, 35 Slivers); one revised-round-2 revision (62 Enchantresses); three veteran freezes (110/135/21, deck21 = second converged exemplar → rotates out); core 13th PASS

Unified from seven independent deck-agent reviews mining the wave-18 PRIMARY corpus
(`matchups-20260716-185930`, 21 games round-robin over 110/135/21/62/49/35/14, binary `/tmp/wagic-f664539a3`).
FOURTH per-deck-rotation cycle. deck17 rotated OUT last cycle (first converged exemplar); **deck14 (Deep Blue)**
entered GUIDELESS (Step 0-bis → initial guide); **decks 49/35** played their FIRST GUIDED corpus (Step 0-ter, each
produced a revision); **deck62** ran its REVISED guide round 2 (surgical revision); **decks 110/135/21** FROZE
byte-identical (11th / 13th / first-post-revision freezes; deck21 is the SECOND converged exemplar and rotates
out). Binary f664539a3 carried the wave-17 engine batch (TARGET CHOICE sub-menu framing; hand-card PUT_INTO_PLAY
dead-end filter [Mox E6]; beneficial opponent-only-target suppression [E-49c]; "BLOCKS: none" example + paraphrase
catch; "(player, life N)" echo strip). Fallbacks **2.2% — NEW BEST** (22 unparsed, 2 stale_echo, 1 defer, 0 empty;
5th clean corpus, 0 segfaults). Core **PASS, 13th consecutive** — every core-adjacent proposal (deck49 reasoning-
length brake, deck35 forced-outcome stop-and-answer + POWER-is-first-number) is single-seat (cannot add a core
line) and/or an ENGINE/representation item, and the losing-position spiral it touches is being handled at the
GUIDE off-case layer this wave (do not double-layer into core). See `synthesis-notes.md`.

**Method headline 1 — the LOSING-POSITION / OFF-CASE SPIRAL doctrine graduates to a PROMOTED Step-0-ter rung (3
seats independently authored an off-case behavioral branch this wave).** Rule 6 said the losing corpus lives in
the off-case; wave-18 named its dominant TAX. Behind and unable to win THIS turn, the weak pilot re-derives its
whole board hunting for impossible lethal ("10 is not 32, I must have made a mistake"), blows the token budget,
truncates → unparse. Three seats independently shipped the fix as a BEHAVIORAL off-case branch in their revision:
deck62 ("when behind you cannot always win this turn; make the best play and pass, do not hunt impossible
lethal"), deck35 (a forced-lethal escape clause), deck49 ("when your curve fails you, the burn is your deck").
CRITICAL (deck62 #5): this is a BEHAVIOR lever, not a token-budget lever — raising max-tokens makes the model
spiral longer and still truncate. Author the off-case branch on the first revision even when the winning corpus
never engaged it.

**Method headline 2 — measure the residual tax by GIANT-REPLY COUNT, not fallback rate (4 seats: 62/49/21/35).**
When a good guide collapses the fallback rate, the reasoning tax RELOCATES and survives as reply LENGTH: deck62's
fallbacks stayed flat (2.1→2.7%) while its >8k giants went 1→8; deck49 had 0 fallbacks yet 20/125 replies ≥600
words; deck21's 4 benign giants all sat in won games; deck35's single fallback was an 11k block-seam giant. Count
giants (>8k) and BUCKET by board-state (winning vs behind) — a win-rate drop from a harder pool shows up as a
giant EXPLOSION localized to the losing off-case, not a taught-class regression. Extends rule 4 (tax-relocation).

**Method headline 3 — a "trust the rendered number" rule must be TWO-SIDED (deck35 worked example + applied;
deck14 combat face).** The promoted trust rule scoped only to the pilot's OWN board leaves the model free to
DISTRUST the other side and re-open the very computation the annotation closes: deck35 obeyed its own lord-stack
[X/Y] perfectly but re-derived the OPPONENT's 10/12 aura creature's power and spiraled when its (wrong) arithmetic
disagreed with the correct "deal up to N" annotation (the corpus's only fallback). Scope "trust the rendered
number" to BOTH boards AND to any damage/effect annotation the representation provides. deck14 adds the COMBAT
face (trust the rendered [X/Y] through flyer/lord/persist math). The durable fix is engine-side (block-outcome
annotation, ledger); the two-sided guide line is the interim.

**Instruments ADOPTED (measurement-only, single-seat, bias no guide content — the durdle-instrument exception):**
deck49 BURN-TARGET-MENU FACE-RATE (face-resolved / total burn-target menus, and how many face choices declined an
offered enemy creature — deck49: 17/17 face, 9 declining an offered enemy body = countable proof BURN-GOES-FACE
internalized); deck135 REPEAT-LOOP unparsed signature (a decode-time repetition spiral — a short phrase repeated
dozens-to-hundreds of times to truncation, carrying NO stale prior PLAN — distinct from the PLAN-caveat stale-plan
class; severity-gate by whether it landed in a WINNABLE game; route to model/decode watch, NOT the guide).

**Method sharpenings ADOPTED:** deck49 CONCURRENT-ENGINE-FIX APPORTIONMENT (when a first-guided corpus also ships
an engine change hitting the same seam, apportion the collapse before crediting the guide — credit the guide only
for the classes the engine did NOT touch; drives the trim; sharpens rule 5). deck62 NAME-THE-TRIGGER (a validated
tightening can persist-and-DISOBEY under a specific trigger — bind the rule to the trigger, do not restate it
harder; sharpens rule 3). deck21 TRIM-IS-A-SEPARATE-PASS (treat "freeze" and "trim" as two method operations; a
length trim is a behavior-risking edit needing its own validation corpus — never bundle it with the freeze that
closes the last open class).

**WATCHES recorded (single-seat, not promoted — promote on a second clean seat):**
- **deck14 COMBAT-ANSWER FORMAT-DISCIPLINE guide line** — for a combat-derivation-heavy deck (flyers + lords/
  anthems + recursion), pair the "you rarely block / trust the rendered [X/Y]" rule with "answer ONE short line
  using the B#/A# CODES, never card names, never a trade enumeration." Self-flagged promote-on-a-second-combat-
  format-drift-seat. The DURABLE fix is the engine parser accepting card-name assignments (ledger #1); the guide
  line is interim. (The ENGINE side is already 2-seat — deck14 + deck110 — but that lives in the ledger, not here.)
- **deck110 POSITION-PARTITION for ramble fallbacks** — split won-vs-lost before attributing a giant reply to a
  decision defect (deck110's 4 rambles all in the 3 LOST games). Reviewer habit, sibling to the durdle instrument;
  already implicit in the seq-repro-unit + tax-relocation discipline.

**Reinforcements (confirmations, no new text):** the 0-ATTACKER durdle instrument delivered a clean DIED verdict
(deck35 2/6→0/6, both former durdle-losses changed character) AND fired at a new deck TYPE (deck14 vs35, a tempo
deck that looks like control) — keep it the first thing tabulated for any "attack every turn" deck. "Quotes the
guide verbatim" is the strongest death-certificate for a taught class (deck21 power-floor + kill-engine, the model
citing the exact clause then obeying it). Attribute the DECISION, not the win/loss (deck21 vs49: kill-on-sight
fired correctly INSIDE a game lost to an unrelated flood). Losses tracing to matchup speed/size or a color/curve
the guide cannot change route to CONSTRUCTION (the roster owner), not new guide prose (deck35 S4, deck49 G-49d,
deck62 #4, deck21 three-wave flood) — a guide steers priority; it cannot draw an Island.

See `synthesis-notes.md` for the ranked engine/harness ledger, the wave-18 validation table, per-guide
dispositions (three freezes, the 62 revision, the 49/35 revisions, the deck14 new guide), the ROTATION table, and
the guide-application list.

## Superseded wave-17 synthesis notes (retained only as lineage)

### (wave-17 header) THIRD per-deck-rotation cycle; two guideless newcomers (Dragons, Slivers); two first-guide revisions (62 surgical, 21 race-math tighten); three veteran freezes; core 12th PASS

Unified from seven independent deck-agent reviews mining the wave-17 PRIMARY corpus
(`matchups-20260716-151559`, 21 games round-robin over 110/135/17/21/62/49/35, binary `/tmp/wagic-7cdcf9c73`).
THIRD per-deck-rotation cycle. deck44/deck133 rotated out; **deck49 (Dragons)** and **deck35 (Slivers)**
entered GUIDELESS (Step 0-bis); **decks 17/21** ran their REVISED guides (17 FROZE this wave, 21 revised
again); **deck62** ran its FIRST GUIDED corpus (Step 0-ter, produced a surgical revision). Three veterans
(110/135/17) FROZE byte-identical. Binary 7cdcf9c73 carried the wave-16 engine batch (PLAN-carry stale-intent
caveat; legalCasts normalEntry fix; own-target-only BAD-effect suppression). Fallbacks **3.3% — BEST EVER**
(18 unparsed, HALVED from wave-16's 40; the PLAN caveat is the confirmed cause). Core **PASS, 12th consecutive**
— every core-adjacent proposal (deck49 target-sub-menu-in-core, deck17 already-lethal cue + block-restriction
annotation, deck35 combat-outcome annotation) is either single-seat (cannot add a core line) OR properly an
ENGINE/representation item that belongs in the ledger, not core prose. See `synthesis-notes.md`.

**Method headline 1 — TRUST-THE-RENDERED-NUMBER graduates from a tagged single-seat refinement to a CONFIRMED
general Step-0-bis first-pass rule (4 seats).** deck62's wave-16 S4 ("trust the printed [X/Y], do not recompute")
is now confirmed at deck17 (mana-available line, held under the revised guide), deck49 (mono-red "0 mana / land
tapped" false-belief off Mountains), deck62 (Blanchwood computed +1/+1-per-Forest), and deck35 (sliver lord
stack). Two faces of ONE rule — (a) mana availability ("if a Cast option is listed it is payable; trust the
Mana-available line; do not recount lands") and (b) derived board stats ("the battlefield [X/Y] already counts
auras/lords/anthems/counters — read it, do not re-derive"). For ANY newcomer whose mana or board math is derived,
this is a load-bearing rule-#1-or-#2 candidate, paired with a representation-ledger lever. De-tagged; folded into
Step 0-bis. (The representation is present but not yet WINNING against the false belief on mono-color decks —
deck49/deck135 still spiked over the surfaced mana line — so the guide trust-line stays load-bearing, do not demote.)

**Method headline 2 — aura/pump-deck guides need an explicit OPPONENT-ONLY-TARGETS OFF-RAMP, not just an
own-creature-only rule (2 seats).** deck62 established "never enchant the opponent"; deck49 sharpened the FAILURE
MODE: when a beneficial aura (Goblin War Paint +2/+2+haste) is offered with ONLY opponent-creature targets, the
weak pilot does not silently skip — it LOOPS to `unparsed_reply` hunting for a use (deck17 s18 12.4k tax, deck135
s22 13.8k tax, both fallbacks). Pair the own-creature-only rule with a terminal off-ramp ("if your only legal aura
targets are opponent creatures, DO NOT cast it — Cast-nothing / play something else; do not agonize"). Both wave-17
newcomer/revision aura guides (deck49, deck62) now carry it. The DURABLE fix is engine-side (E-49c: suppress a
beneficial-aura cast when every legal target is opponent-controlled — the mirror of the shipped bad-effect
suppression, ledger item); the guide off-ramp is the interim.

**Method headline 3 — review-discipline instrument (adopted single-seat, justified): the 0-ATTACKER-GAME durdle
signature.** deck35 contributes a cheap, countable reviewer instrument (a MEASUREMENT tool, not a behavioral
doctrine — hence acceptable single-seat, zero risk of biasing guide content, applies to EVERY aggro seat): for any
deck the guide labels "attack every turn," tabulate the per-game `attackers`-decision COUNT and own-creature count
over time. A game with ZERO `attackers` records (and/or an empty own board despite many creatures cast) is proof
the durdle fired, independent of win/loss and cheaper to read than the prose — then check WHY the attack step was
never reached (chumped-away vs tapped vs summoning-sick). deck35's two clean losses had 0 attacker records + the
opponent at 20 life; its wins had 4-5. Sibling to the wave-16 "reconcile looks-disobeyed against the eligible set."

**WATCHES recorded (single-seat, not adopted — promote on a second clean seat):**
- **deck49 — spell/burn-heavy newcomer's dominant idiosyncrasy is the TARGET SUB-MENU seam** (a THIRD Step-0-bis
  rule-3 face, after belief-that-vetoes and card-text-fabrication): for a deck whose plays mostly target something,
  expect the stale_echo + tax cluster at the target sub-menu, and teach the flow explicitly ("choosing a targeted
  cast opens a second menu listing the TARGETS; pick by number; no Cast-nothing after committing; do not re-name
  the spell"). deck49 shipped this as its guide rule-2. Single-seat; the durable fix is the engine sub-menu
  representation (ledger #1).
- **deck21 — scope a COUNTABLE-COST rule to the input QUANTITY, not the eligibility gate.** A rule of the form "an X
  costs you N per turn, so it is worth a burn" fires on ANY eligible target even when N=0 (deck21 vs62 s6: the
  wave-16 race-math exception, scoped on TOUGHNESS/killability, fired on a 0-POWER Birds of Paradise whose race cost
  is 0 — a game lost by 1 life). Name the input quantity and give its zero/degenerate case a go-face branch. Single-
  seat this wave, 2-seat-ADJACENT to deck17's wave-16 "biggest flyer THAT CAN ATTACK" superlative-scoping — same
  shape (model reads the eligibility surface, not the intent quantity). deck21 fixed it in-guide (POWER FLOOR); hold
  the METHOD generalization pending a second direct instance.
- **deck21 — auto-no-block seats generalize race-math from flyers to ground.** At an engine-auto-declares-no-blocks
  seat, every persistent attacker (ground or flying) connects every turn, so "no-blocker race math" is not flyer-
  specific. Do NOT broaden the guide rule on one confounded instance (risks re-broadening the exact rule just
  tightened); separate the STRUCTURAL fact from the guide CHANGE. WATCH pending a second clean unconfounded instance.

**Reinforcements (confirmations, no new text):** deck17 — a "died quiet" verdict is a re-check candidate (a class
can stop producing WRONG CHOICES while still producing WASTED REASONING; Cloud-Sprite block MUTATED wrong-choice→
token-ramble); a root-cause fix that reverses its causal matchup (vs110 loss→win) is stronger evidence than an
aggregate win-rate tick. deck62 — when the fallback rate collapses, RE-CLASSIFY the survivors by MECHANISM (reply
length + what the model did), not by kind label: a surviving "unparsed" after a good guide is usually a new, narrower
bug (deck62 vs135 s19 = a crisp list-vs-board misread), not the old tax persisting. deck35 — score the ACTION, not
the arithmetic or the verbal model, on comprehension watches: a correct number does not prove comprehension and a
wrong verbal rule ("lords buff future slivers only") does not prove a misunderstanding if the action (buff-math) is
right — extends the wave-16 "read RESULT not INTENT."

See `synthesis-notes.md` for the ranked engine/harness ledger, the wave-17 validation table, per-guide
dispositions (three freezes, the 62/21 revision vetting, the 49/35 new guides), the ROTATION table, and the
guide-application list.

## Superseded wave-16 synthesis notes (retained only as lineage)

### (wave-16 header) the FIRST first-guided-corpus VALIDATION cycle; a second guideless newcomer (an ENGINE deck); four veteran freezes; core 11th PASS

Unified from seven independent deck-agent reviews mining the wave-16 PRIMARY corpus
(`matchups-20260716-084532`, 21 games round-robin over 44/110/133/135/17/21/62, binary `/tmp/wagic-69793930a`).
This is the SECOND per-deck-rotation cycle. deck140 rotated out; **deck62 (Enchantresses) entered GUIDELESS** as the
FIRST engine/combo deck through Step 0-bis; **decks 17 and 21 played their FIRST GUIDED corpus** (initial guides
shipped wave-15) and are the first two seats to run a first-guide VALIDATION; four veterans (44/110/133/135) FROZE
byte-identical. Binary 69793930a carried the wave-15 engine batch at scale (free zero-mana alt-casts, the stale_echo
superstring matcher, the hybrid-pip affordability annotation). Core **PASS, 11th consecutive** — no 2+-seat
core-wording candidate survived; every adoption owns a layer BELOW the core.

**Method headline 1 — the FIRST-GUIDE VALIDATION doctrine, converged from two independent first-guided seats (decks
17 and 21) → ADOPTED as the new Step 0-ter.** A guide's first guided corpus is validated by reporting, per taught
class, DIED / PERSISTED / MUTATED with seq repros — separately from the RECORD. Load-bearing at BOTH seats: (a)
died/persisted/mutated per class before touching the win-rate; (b) reconcile a "looks-disobeyed" choice against the
OFFERED/eligible set before calling it a play failure (deck17: every "under-attack" turn had exactly ONE eligible
attacker — the attacker-seat cousin of the wave-15 defender-drop lesson); (c) separate the guide's JOB (did classes
die? did the tax collapse?) from the RECORD, which the pool swap + variance move more than the guide does (deck21
3/6→2/6 with the tax epicenter GONE and 3 of 4 losses within 4 life); (d) a first revision often TIGHTENS an
over-broad rule, not just adds facts — the more-specific matchup line silently overrides a general exception
(deck21's "all burn to face vs a flyer swarm" countermanded its own kill-the-flyer exception; deck17's "cast the
biggest flyer" cast a 4/4 wall), so scope a superlative to the INTENT ("biggest flyer THAT CAN ATTACK"), not the
surface property; (e) when a taught belief DIES but the TAX SIGNATURE persists at the same seam, the tax RELOCATED to
a successor belief — name the successor and route it correctly, do not conclude the fix failed (deck17: the hybrid
off-color belief died, the tax moved to stale WITHIN-TURN mana tracking). Single-seat WATCH sub-lessons folded in:
write the off-case branch on the first revision even if the winning corpus never needed it (deck17 — the losing
corpus lives entirely in the off-case); validate the fix-WINDOW, not just the fix direction, when the target GROWS
(deck21 — a small flyer pumps out of burn range, so "kill it EARLY or never"); replace a value-evocation ("worth
more than 3 face damage") with the countable MECHANIC the board proves ("a 1/1 flyer you cannot block costs you 1
per turn for the rest of the race").

**Method headline 2 — deck62's four ENGINE-DECK refinements to Step 0-bis (single-seat, first engine deck, tagged;
each EXTENDS an existing 2-seat rung).** ADOPTED as engine-deck instantiations, not new headlines: (S1) for an
engine/combo deck the rule-#1 idiosyncrasy is a CAPABILITY TABLE — the pilot's failure mode is FABRICATING card text
(qwen invented a "kill" for a pump aura), not a belief that VETOES a real option (the 17/21 shape), so rank one
plain-effect line per key card by mis-description tax (extends rule 3); (S2) teach engine SEQUENCING as an explicit
ORDER for any order-dependent trigger (an enchantress/prowess/sacrifice payoff) — a weak pilot will not infer play
order from a static ability line (generalizes rule 5's "write the floor first" from ATTACKING to ORDERING); (S3)
broaden rule 5's attack-floor exclusion from "name the walls" to "name every creature the model should NOT declare —
walls, 0-power engine/mana dorks — and state the eligible-attacker list already drops summoning-sick bodies so the
model must not re-derive eligibility" (2-seat-adjacent: deck17 named Defenders, deck62 shows it generalizes); (S4)
tell the pilot to TRUST the rendered [X/Y] on the battlefield line, do not recompute a pumped creature's P/T from its
aura stack — this CONVERGES at 2 seats with deck17's "trust the Mana-available line / cast list, do not recount your
Islands" into a general TRUST-THE-RENDERED-NUMBER guide line (both paired with a representation-ledger lever). All
four are guide-authoring METHOD; none touches the core.

**Method headline 3 — review-discipline, converged across three seats: do not build a change on an unverified
premise.** (a) deck110: do NOT tighten a rule (the mulligan "keep 2+ creatures") on the strength of losses when the
"loose" behavior is UNIVERSAL across the WINNING games too — all 6 hands were 1-creature keeps and the deck went 4/2,
so the tempting "loose mulligans seeded the losses" hypothesis is falsified by the wins; check the counter-hypothesis
before proposing the tightening. (b) deck44: the wave-15 "combat asks removed corpus-wide" false finding (built from
one seat's phase histogram, refuted by per-seat counts) is the concrete cost — a claim asserted as CORPUS-WIDE must
be checked against ≥2 other seats' logs before it is written, and NEVER used to justify a core/method change from a
single seat. (c) deck44+deck133: a recurring misplay the guide ALREADY addresses (self-GFTT / self-Fatal-Push, both
present-and-disobeyed) is evidence FOR an engine/structural fix, not for a third prose warning — route it to the
ledger and freeze. These converge with deck17 WATCH-1 (a "looks-disobeyed" behavior is usually obeyed against a set
you didn't read): verify the premise before the change.

**Reviewer-facing sharpening (single-seat WATCH) — a THIRD stale_echo downgrade cause.** deck133 names a
degenerate-LABEL false-positive (the echo names a coherent, present intent but the OPTION ITSELF serializes to a
contentless stub — a Liliana +1 target menu rendered `Becomes ` with no creature/stats), distinct from the (a) LEAK
and (b) SUPERSTRING causes; it is a HARNESS/SERIALIZATION defect upstream of the matcher, and a reviewer must bucket
it separately, not credit it as a clean catch or "fix" it by loosening the matcher. WATCH (single-seat, non-deciding)
pending a second seat. See the Step-2 Mechanism-A/B scoring rung.

See `synthesis-notes.md` for the ranked engine/harness ledger, the wave-16 validation table, per-guide dispositions
(the four freezes, the 17/21 revision vetting, the deck62 new guide), and the ROTATION table.

## Superseded wave-15 synthesis notes (retained only as lineage)

Unified from seven independent deck-agent reviews (decks 44, 110, 133, 135, 140 — veterans, guides FROZEN
since wave-11 — plus decks **17 and 21, GUIDELESS first-pass newcomers writing their INITIAL guide**) mining the
wave-15 PRIMARY corpus (`matchups-20260716-040138`, 21 games round-robin, binary `/tmp/wagic-aad32bc7f`). This
is the FIRST cycle under the PER-DECK rotation rule (deck109/deck131 rotated out after wave-14 validated their
seats clean) and the FIRST corpus carrying, at scale, three fixes that were wave-14-validated as infrastructure:
the **target-cancel use-after-free crash fix** (`aad32bc7f`), the **absent-echo staleness route + attackers
name→index reconcile** (`d0f88326b`), and the **Galvanic Blast metalcraft fix** (`3cb901295`). Latency regime
unchanged (Spark @0.75, med 11.4s / p90 93s); 0 raw timeouts, 0 segfaults, adjudications are a latency artifact —
score adjudicated POSITIONS, not counts. **Dominant mode is STILL FREEZE: all 5 veteran guides are byte-identical
to the live baka guides** (`cmp`-verified). **Core status: PASS (10th consecutive wave, no prompt-body edit)** —
but this wave carried a REAL promotion question that resolved toward hardening the representation route (below).

**Method headline 1 — the FIRST guideless-first-pass doctrine, converged from two independent newcomer seats
(decks 17 and 21). A guideless corpus is a GIFT: it shows exactly what the core prompt gets right (do NOT re-teach)
and — via the reasoning-tax spikes — exactly which deck facts are load-bearing (teach precisely those).** See the
new **Step 0-bis** for the full doctrine. Its convergent, 2-seat rules: (a) **the reasoning-tax spikes ARE the
saliency signal** — every place qwen burned 800–3,000 words (or looped into `unparsed_reply`) re-deriving something
was a deck fact the core does not carry; rank candidate teaching points by BOTH (i) did it cause a wrong/near-wrong
action AND (ii) tax magnitude, and write the guide to PRE-ANSWER the highest-tax derivations (deck21: Spark-Elemental
end-step timing 2,773 words, summoning-sick token math 1,266, burn-can't-kill-a-6/6 loop 2,099→unparsed all mapped
1:1 to the top teaching points; deck17: hybrid oscillations drove the p90 latency that cost 3 adjudication losses).
(b) **When one idiosyncrasy dominates the deck, the corrective FACT earns the #1-RULE slot ABOVE the win engine**
(deck17: ~all-hybrid mana off mono-Island → the "your mana is simple, every spell casts off Islands alone" fact is
rule #1; deck21: burn-is-a-clock / Spark-timing above the anthem engine) — because the belief taxes/vetoes the
engine's own plays. (c) **Teach ENGINE reality, not Oracle memory** — the engine's cards DIVERGE from real MTG
(deck21: Scuzzback Scrapper `{R/G}` 1/1 no-haste vs real `{2}{R/G}` 3/1 haste-persist; Boartusk Liege 3/4-trample
vs real vanilla 4/4); derive every stat/keyword/cost from the option/board annotation strings and cross-check
keyword claims against a real observed window (a named-but-dropped attacker = confirmation of no-haste), and route
card-script divergences to notes.md for Oracle-verification — the guide uses what the engine ACTUALLY does. (d) **A
first guide's job is often to COUNTERMAND a generic core line, not just add deck facts** — a bare fact-list will not
stop the model obeying a core reflex (deck21 vs140: qwen quoted the core's "hold instant-speed interaction" line
verbatim and held its Lightning Bolts into an empty board while a lifegain deck stabilized → the adjudicated loss;
deck109's frozen guide already encodes the same override for Searing Spear). (e) **Write the attack/deploy FLOOR on
the first pass** for any evasion/aggro deck — the attack-floor-does-not-transfer rung (skill's deck135 wave-5 lesson)
recurred on BOTH newcomers (deck17 went passive vs140 while an opponent gained to 64; deck21 durdled to a naked
board vs140) — do not defer it to a revision. Convergence: (a),(b),(c),(e) each have 2-seat support (17+21); (d) is
grounded at deck21 with deck109 corroboration.

**Method headline 2 — the three wave-14 shipped fixes are VALIDATED AT SCALE; all three PASS with visible play
improvement, and the wave-13 losing classes they targeted did not recur.** (i) **Galvanic Blast metalcraft
(`3cb901295`) — CLOSED.** deck110 confirmed 4 damage at N≥3 artifacts / 2 at N<3 across deck135/deck44/deck17/deck133
by reading the engine's `Damage:` events, and the fix VISIBLY improved play: deck133 flipped loss→win and at the
exact wave-13 blunder spot (Obliterator creature-burn) the model instead fired the Blast at the FACE for real reach;
no toughness/creature-burn blunder recurred. **Verification caveat (folds under method-headline-3 "read RESULT not
INTENT"): the metalcraft +2 ships as a SECOND resolution instance, so a working Blast logs TWO identical `Damage: 2`
lines — a naive grep that dedupes the event STRING mis-reads it as "2"; COUNT the instances / read the life delta.**
(ii) **Attackers name→index reconcile (`d0f88326b`) — CLOSED.** deck110 audited 20 attacker windows: every eligible
named creature unioned in, every ineligible (summoning-sick) name correctly EXCLUDED, 0 eligible attackers dropped;
the wave-13 MIXED-form silent-drop is not reproducible (corroborated deck135, deck21 — both had mixed `ATTACK: A1,
<name>` lines whose dropped names were correctly ineligible). (iii) **Absent-echo staleness route (`d0f88326b`) +
crash-fix (`aad32bc7f`) — STRONG PASS.** deck133 (whose own Fatal-Push target-decline caused the wave-14 segfault)
exercised the freed-object family 11 times with 0 segfaults, and a full Mechanism-B scan found 0 index leaks — the
wave-13 game-losing GM-hallucination class (echo names an absent card → index-wins → arbitrary cast) is now caught
every time (corroborated deck44: 2 correct absent-echo refusals, 0 self-destroys, 4/4 removal sub-menus resolved to
the opponent; deck110, deck140: all stale_echo events correct catches). ONE confirmed false-positive MECHANISM
surfaced (WATCH, single-seat): deck133 vs140 s37, the route OVER-fired when the echo named an offered option as a
SUPERSTRING (`Attack with Yawgmoth, Thran Physician` ⊇ the option's short `Attack with Yawgmoth`); non-deciding,
durable fix is a HARNESS prefix/substring match, not a guide/core edit.

**Method headline 3 — the FIRST completed live Force counter ever captured, and the "name and date the first live
confirmation of a long-open engine item" method proved its worth (deck135, seat owner of the Force seam).** deck135
deck140 seq18: the free exile alt-cast Force of Negation drove Black Sun's Zenith `stack → EXILE` (COUNTERED), the
load-bearing play of a WIN; seq30 hardcast Force countered Lightmine Field. Through wave-13 this path had 0 casts
and was unvalidated end-to-end — ENGINE #1 (Force never counters) is, on this evidence, FIXED. Paired NEW engine
bug (HIGH value, 3-repro mechanistic, deck135-owned ledger #4, SUPERSEDES the async-race theory): **the free
alt-cast SILENTLY FAILS TO EXECUTE when the caster controls NO untapped mana sources** — `(no untapped sources)`
correlates 3/3 with fizzled answered casts (deck21 s27/s29, deck110 s4), while the one execution (seq18) had `{w}`
up; NOT latency (a 93s cast executed, an 8s cast fizzled). An empty (exile) cost must be payable with a fully tapped
board; the alt-cost cast path is wrongly routed through the mana-payment / untapped-source machinery. The brief's
per-item "celebrate the first completed instance with its seq" instruction turned this from a buried line into the
corpus headline AND cleanly separated "the engine capability now exists" from "the model uses it well" — worth the
skill carrying as a standing method (name+date the first live confirmation of a long-open engine item).

**Reviewer-method sharpenings (folded into the review procedure, no new headline):** (i) *count and impact are
DIFFERENT measures — report BOTH.* deck140 quantified the `Mana available` dual-hiding defect at **48 instances / 5
of 6 games with ZERO misplays** (the model treated the offered-options list as authoritative — it even cast an
"unaffordable-looking" red spell); a reviewer reporting only the raw count over-states harm, one reporting only "no
misplay" under-states a real pervasive engine defect that WILL bite the day the hidden color is the sole path (2
seats: deck140 dual-hiding, deck133 stale_echo true-vs-false-positive split; deck110's Galvanic dedup-trap is the
same principle applied to the verification tool). (ii) *NOT-EXERCISED ≠ NOT-REPRODUCED ≠ RESOLVED.* For an engine
item that only manifests during a specific action (an X-spell cast, a forced sacrifice, a freeze re-render), first
confirm the ACTION OCCURRED before scoring recurrence — a symptom-absence grep would falsely retire it (2 seats:
deck140's Rakdos X-underpay got 0 casts = UNTESTED not resolved; deck135's inverted-freeze had its one window end
the game = UNVALIDATED not fixed; record adjacent-mechanism evidence as partial, keep the specific item OPEN).

**Convergence-watch verdict — the colored-pays-generic / trust-the-option-list PROMOTION TRIGGER FIRED cross-seat;
core stays PASS and the durable fix HARDENS the REPRESENTATION route.** Wave-13 parked this family and named the
exact promotion trigger: "a cross-seat ACTUAL offered-payable creature decline citing inability to pay a generic pip
with colored mana." It FIRED, cross-seat, THIS corpus: **deck135 s3 (game-losing, no self-correction)** — declined an
OFFERED `Cast Icehide Golem with its alternative cost {1}` by hallucinating a `{1}{g}` requirement, cast Astrolabe
instead, lost the game; **deck17 (guideless, 2 real under-deployments)** — cast Scion 1/1 over the OFFERED Wasp
Lancer 3/2 believing the hybrid `{u/b}{u/b}{u/b}` uncastable (once miscounting it as 6 mana); **deck21 F5** — declined
Scuzzback Scrapper `{R/G}` "I lack green mana," self-corrected next turn. This is no longer wave-13's fired-then-self-
corrected belief — it is completed wrong actions across ≥3 seats. **BUT the verdict is NOT a core wording edit.** The
core body already states BOTH governing facts VERBATIM — the rule fact (line 4: hybrid = either color, any color pays
generic) AND the option-list-authority meta-rule (line 36: "Never decline a listed action because you believe … you
cannot pay for it; if you truly could not, it would not be listed"). deck135 s3 is a PRESENT-AND-DISOBEYED failure —
the model distrusts the authoritative list and INVENTS a cost to override it — which is the exact signature of the
representation-beats-instruction wall; a 3rd restatement in fresh words is the sanctioned-against escalation. The tax
(deck17's thousand-token oscillations that cost adjudications) is ALSO invisible to any core wording and directly
addressable by representation. **So: core PASS, and the hybrid-pip / affordability OPTION-LINE annotation (an
`offered=payable` breakdown / "(pay with any color)" tag the engine already partly computes) is PROMOTED to the TOP
representation item** — 2 seats (deck17 + deck44 cross-witness, deck21 corroborating). Promotion of the item to
top-priority, NOT of a core line.

**Second core question adjudicated — the "hold instant-speed interaction" line is PER-DECK GUIDE territory, not a
core qualifier.** deck21 flagged that the core's line-6 "hold instant-speed interaction" is anti-teaching for pure
burn-aggro (it caused the vs140 held-Bolts loss). Verdict: NO core edit. The core ALREADY carries the burn-as-clock
override in **line 21** ("Direct damage and burn are part of your CLOCK, not reactions … send them at the opponent's
face … once the opponent is at 8 life or less always fire damage at their face"); the model latched onto the more
general line-6 and ignored the specific line-21 rule — again a model-adherence / wrong-line-selection failure, not a
missing teaching. deck21's new guide countermands it LOCALLY and explicitly (the sanctioned route per method-headline
1(d)); it is a single LIVE witness this corpus (deck109's is a prior-wave guide-encoded override). Cross-seat trigger
to reconsider conditioning line 6: a 2nd deck's reviewer finding an offered burn/instant HELD as interaction against
a deck that wants it on a clock — NOT met. If it ever promotes, note line 21 may already be the correct home.

**REJECTED (orchestrator-verified) — deck44's "combat asks removed corpus-wide / 0 attacker-blocker asks at any
seat."** deck44 F2 claimed combat is now fully auto-resolved and built its 1/6 loss narrative (all-in auto-attacker
left no crackback blocker) and a "surface-audit before crediting any block/lethal prose / trim block sections
corpus-wide" method on it. This is FALSE: the per-seat corpus counts are attackers=29/blockers=12 (deck44),
20/2 (deck110), 16/0 (deck133), 18/4 (deck135), 20/17 (deck17), 33/2 (deck21), 0/0 (deck140, creatureless). The
corpus itself refutes it — deck110 audited **20 attacker windows**, deck135/deck21 show live `ATTACK:` declarations
this very corpus. deck44's phase-enumeration mis-scanned (likely counted only a subset of its own seat's asks). The
auto-combat finding, any surface-audit/trim-block method built on it, and deck44's loss attribution are REJECTED and
NOT adopted. deck44's byte-identical FREEZE stands; its OTHER validations (absent-echo self-destroy CLOSED, sweeper
counter now tested) corroborate other seats and are fine. **deck44 → HOLD one cycle for a corrected loss read.**

**Core status this wave: PASS (10th consecutive, no prompt-body edit).** `general-strategy.txt` is a byte-identical
copy of the live core prompt. No 2+-seat CORE-WORDING candidate survived: the affordability family promoted its
REPRESENTATION fix (not a core line); the hold-instants question is per-deck guide territory (line 21 already covers
it). Every adoption owns a layer BELOW the core: the guideless-first-pass doctrine is guide-authoring METHOD; the
three shipped fixes are engine/harness/card-script items now validated; the Force counter + no-untapped-sources bug
are engine ledger; the count-vs-impact and not-exercised distinctions are reviewer method. See `synthesis-notes.md`
for the ranked engine/harness ledger, the wave-15 validation table, guide dispositions, and the ROTATION table.

## Superseded wave-13 synthesis notes (retained only as lineage)

Unified from seven independent deck-agent revisions (decks 44, 109, 110, 131, 133, 135, 140) of the wave-11
skill, mining the wave-13b PRIMARY corpus (`matchups-20260715-183228`, 21 games round-robin, binary
`/tmp/wagic-a75e2625e` = Force round-2 cost-target fallback `eb5145082` + Force round-3 offer-legality/dead-end
removal `a75e2625e` + auto-tap two-pass `d5d7b1f2e` + name-echo protocol `c3e6d95b2` + BLOCKS:none `42e8bc180`
+ 240s timeout; wave-11 guides all frozen; core prompt unchanged, 8th PASS standing). Two intervening corpora
(wave-12 `matchups-20260715-120540`, wave-13a `matchups-20260715-151947`) were CONTAMINATED by Spark crashes
and are engine-evidence only — this is the FIRST clean end-to-end validation of the name-echo protocol, the
Force round-2+3 fixes, the auto-tap two-pass fill, and BLOCKS:none at scale. **Dominant mode: FREEZE — all
seven guides are byte-identical to the live baka guides** (`cmp`-verified, zero bytes changed, 7th–10th
consecutive freeze depending on seat), and all seven seats voted core PASS. Every adoption below is a
VERIFICATION/SCORING-METHOD refinement — how to SCORE a shipped (or still-unshipped) fix — routed to a layer
BELOW the guide (reply-protocol / attackers-parser / card-script / representation); none is a guide-prose or
core-prompt edit. Per the epistemics rule no single-seat evidence cut a method line; every adoption has 2+
witnessing seats or completes an existing rung.

The shipped-fix validation table (full detail in `synthesis-notes.md`): **name-echo 100.0% compliant**
(982/982 CHOICE lines carry `(name)`), 0 WRONG remaps, index-wins-on-conflict validated on every
PRESENT-option conflict; **BLOCKS:none clean** (parses at deck131/deck109/deck135, 0 misparses of intended
blocks, the wave-11 bare-`BLOCKS: none` parser bug CLOSED); **Force round-3 confirmed at deck135** (0 dead-end
"exile a blue card" picks corpus-wide, 1 legal alt-cast offer vs deck44 t8 correctly gated on a payable exile
target and correctly DECLINED — opponent was GFTT-ing its own creature); **empty_reply = 0** corpus-wide (the
wave-12/13a empty-reply flood was Spark dying — class CLOSED); **240s timeout validated** (0 raw game
timeouts; 49 unparsed_reply = 3.7%, the benign long-ramble discard class). The one interface cost is the
LATENCY REGIME, not a play regression: Spark ran at GPU_MEM_UTIL=0.75 (crash-avoidance trial — it PASSED, no
crash, memory flat), costing decision latency med 11.2s / p90 87s / p99 185s under 3-concurrent KV preemption;
13/21 games hit the 2400s cap and were LIFE-adjudicated. That latency — not play quality — drove the
adjudication count; the adjudicated positions must be scored on their own merits (see the deck135 vs110
"won-board-lost-adjudication" fixture in `synthesis-notes.md`).

**Method headline 1 — the reconcile-named-action arm's ABSENT-ECHO branch is the load-bearing completion; the
shipped name-echo protocol did NOT rescue it, and its "index wins on conflict" default is the WRONG tie-break
when the echo names NO offered option — absent echo = STALENESS signal, route to the heuristic / Cast-nothing,
NEVER the raw index (3 seats: deck133, deck140, deck44; GAME-LOSING ×2).** Wave-11 headline 1 established that
the reconcile arm must route a plan's NAMED action that is ABSENT from the options to the intent-consistent
best-available option, never an arbitrary index, and reject-and-repair an out-of-range index. Wave-13's
name-echo shipment (`(name)` on every CHOICE, index-wins on conflict) was the natural place to close it — and
it did NOT: name-echo only helps when the echoed name MATCHES a listed option (then the arm can remap/confirm).
When the echo names a card that is in NO option, there is no remap target, index-wins is the ONLY behavior, and
it executes an unrelated in-list card. Three seats witness it in one corpus, TWO game-losing: deck133 vs140
seq45 (echo "Gray Merchant", not offered → index 1 → the guide-FORBIDDEN thin-hand Thoughtseize → −2 life,
flipped a 1-life adjudication loss); deck140 vs44 s40-42 ("Play Badlands" fixation after the land was already
played → index 1 was now a *Cast* → dumped a held Rakdos kill shot at X=8 and a Wrath into `creatures:0`);
deck44 vs135 s24 (echo "Cast Go for the Throat" naming the PARENT action at a target SUB-MENU → index 1 = own
creature → SELF-DESTROY) and vs133 s14 (echo names the just-cast Surveilling Sprite → index 1 now GFTT →
removal cast against the model's own explicit refusal). Two mechanisms produce the absent echo and both point
the same way: (a) the echo names the PARENT action while the menu is a target sub-menu, and (b) the option list
SHIFTED after a same-turn cast and the echo names the just-cast card. Adopted: **the reconcile arm's
absent-named-action branch must treat an echo that matches NO current option as a staleness signal and fall
through to the engine heuristic / Cast-nothing (or re-prompt) — never the raw index; and it must re-anchor
indices after a same-turn cast.** This is the SHIPPED name-echo protocol's completion, routed to the
code-appended reply-protocol / attackers-parser layer; the guide's "confirm the card you name appears in the
list; else cast the best listed card" was PRESENT-AND-DISOBEYED, so a louder guide line is the sanctioned-
against escalation. Paired SCORING gate (same 3 seats): a "100% echo compliance" + "index-wins-on-conflict"
statistic is necessary-but-INSUFFICIENT — split echo-index conflicts into **Mechanism A** (echo names an
OFFERED option ≠ index; remap to the unique match, index-wins a defensible tiebreak on genuine ambiguity —
the case the protocol was built for, and it behaves) vs **Mechanism B** (echo names NO option = hallucinated
presence; index-wins is an uncontrolled cast), and COUNT Mechanism-B events and their executed-card outcomes;
compliance measures "carries a name," not "the name matched or the intent survived," and the kill-shot dumps
are invisible to a compliance grep — they surface only by reading the PLAN against the option list at every
"Cast X into a spent-land-drop / sub-menu" seam.

**Method headline 2 — score attacker declarations by the parsed INTENDED-vs-RECORDED count off the `ATTACK:`
line, NEVER by window count or `chosen_text`; the reply has three forms failing at different layers, and the
MIXED index+name form is a SILENT non-fallback drop (2 seats: deck109, deck44; corroborated deck131, deck135,
deck110).** Sharpens the confirmed-fired terminus's "measure in the reasoning, not the recorded choice" applied
to the ATTACKERS seam. The three forms: **FULL-INDEX (`A1, A2, A3`)** parses clean; **FULL-NAME (`Hellrider` /
`Ornithopter, Signal Pest`)** UNPARSES (`fallback=unparsed_reply`, `choice=-1`) → the baka heuristic attacks
with ALL eligible creatures, which for a pure-aggro deck COINCIDES with intent → OUTCOME-NEUTRAL (verify in the
NEXT record's `declared attackers:` log, not by the fallback flag, which makes it LOOK like a miss); **MIXED
(`A1, Rakdos Cackler`)** — the dangerous one — the parser consumes the leading A-indices, IGNORES the trailing
name token(s), and records a NORMAL `choice=N` with NO `fallback` field, so it looks like a clean declaration
while it UNDER-declares (deck109: 6 dropped declarations across 5 windows / 3 games, combat-confirmed at vs140
s7 "attack with both to deal 3" → log `declared attackers: Stromkirk Noble`; deck131: 5/18 incl. an unparsed
win-game alpha strike; deck135, deck110 corroborations). Adopted: **parse the `ATTACK:` line into A-index
tokens vs NAME tokens and compare the INTENDED count to the recorded `choice` count — a window is a full
declaration ONLY when they match; the full-name form is scored by the combat log (neutral if the heuristic
attacked with everyone), the mixed form by the intended-vs-recorded delta (a real drop).** A raw window count
or a `chosen_text` read silently passes the mixed-form drop (this is why wave-11 scored "26/26 full
declaration" and MISSED the class). Both forms are attackers-PARSER items → notes ledger; NEVER re-word the
guide (the model already NAMES every attacker — the parse seam drops it). The durable fix is the attackers
name→index reconcile arm mirroring parseChoice/name-echo: after consuming A-indices, resolve any residual NAME
tokens against the eligible-attacker roster by unique-name match, respecting eligibility, and UNION them into
the declared set — fixing the mixed-form silent drop AND upgrading the full-name form from "heuristic guess"
to "the exact creatures the model named."

**Method headline 3 — verify RESOLVED effects against the game-log events, NEVER the model's asserted
magnitude; extend confirmed-fired-representation from "did the surface render" to "did the mechanic RESOLVE as
rendered" (deck110, rung-completing — the Galvanic Blast lesson).** The skill already teaches confirmed-fired
(did a lever render) and RESOLVED-VS-ASSERTED at the cast-happened anchor (did the SPELL resolve or get
countered). Wave-13 completes the terminus at the EFFECT-MAGNITUDE layer: when the MODEL asserts an effect
size in its reasoning ("Galvanic Blast deals 4 with metalcraft"), the review must confirm that magnitude
against the ENGINE's own resolution record (`Damage:` / `sacrifice` / `dies` events), because the asserted and
delivered effects can DIVERGE — and when they do, the divergence is a card-script/engine bug hiding behind
fluent-but-wrong model prose. Wave-11 deck110 credited every Galvanic Blast as "genuine metalcraft, correct 4
dmg" read straight off the model's reasoning; wave-13 checked the `Damage:` events and found EVERY Blast in all
6 games dealt 2, never 4 — the metalcraft `+2` line (`aslongas(...) damage:2 >2`, missing `oneshot`, the
STATIC-effect form used for a ONE-SHOT) never fires, so a HIGH, outcome-bearing bug on the deck's core wincon
sat undetected for ≥2 waves. Adopted: **for any decision whose value depends on an effect MAGNITUDE the model
computes (burn, pump, life swing, sacrifice count, counters), locate the resolution in the game log and confirm
the number; where the deck has a guide-stated magnitude, spot-check the engine delivers it at least once — a
magnitude that NEVER appears in the log across a whole corpus is a bug, not a coincidence. The model's prose is
a statement of INTENT; only the game log is a statement of RESULT.** The card-script fix (ledger item) is a
DATA fix in `Res/sets/primitives/mtg.txt` — verify against real Galvanic Blast Oracle text first (flat 4 with
metalcraft, not +2) and via the wagic-cardscript skill's `oneshot` idiom; the GUIDE is Oracle-correct and NOT
edited.

**Single-seat refinement (WATCH-gated) — the representation-CONTRADICTION-multiplies-reasoning class lives on
the BOARD-STATE serialization too, not only OPTION annotations, and under the latency cap its worst outcome is
a DECISION TIMEOUT surrendered to the heuristic (deck135, completes the deck135-owned reasoning-tax rung; the
timeout-consequence promotion needs a 2nd seat).** deck109 s19: a creature FROZEN by Abominable Treefolk's ETB
serialized as `[tapped - untaps and can attack next turn] [attacking]` while NOT in the numbered Attackers
list — four board defects (a stale `[attacking]` tag on a non-attacker, and a freeze-state not represented but
actively INVERTED by `untaps next turn`) collide into a self-contradiction that the model spent its ENTIRE
reply re-deriving, hit the latency wall, and produced `unparsed_reply` → heuristic fallback on a COMBAT
decision. Adopted (completing the tax rung's grep target): **grep the BOARD-STATE serialization for
self-contradictory tags (a creature both `[attacking]` and absent from the declared-attacker list; a state
annotation contradicting a known effect, e.g. `untaps next turn` on a frozen permanent), not only the OPTION
annotations, and score cost across BOTH surfaces — reasoning-tax AND whether the inflated derivation TIMED OUT
into a heuristic fallback.** Single-seat/single-instance this corpus (outcome-neutral — the position was lethal
regardless); the board-state LOCUS extension rides the existing rung, but the timeout-as-terminal-cost
promotion is WATCH pending a 2nd seat where a board/annotation contradiction co-locates with `unparsed_reply`
on a NON-lost decision.

**Reviewer-method sharpenings (folded into the review procedure, no new headline):** (i) *deck131 — when a
block-side (or attack-side) guardrail appears VIOLATED, cross-check the OPTION/BLOCKER LEGEND against the
battlefield line before attributing the violation to the model.* At vs109 s29 the NEVER-block-Guttersnipe
absolute fired perfectly in reasoning yet the engine blocked Guttersnipe — because the blocker LEGEND OMITTED a
legal untapped blocker (the Elemental), leaving the model only `B1. Guttersnipe`, so it forced B1 onto "the
Elemental" and the engine executed the forbidden block. A missing-legal-option menu defect masquerades as a
guardrail failure; the leak layer here is the menu itself, below both the guide and the parser — read the
reasoning AND the offered options AND the executed combat, never `chosen_text` alone. (ii) *deck110 — when a
guardrail's seam becomes newly EXERCISED (here the seat went from 0 defensive blocks to 5), re-audit it against
the PROMPT'S OWN annotation before attributing to the guide.* deck135 s17 over-blocked (10/2 equipped attacker
into a 2/2, both die) DESPITE the prompt spelling out the correct line verbatim ("block only where the trade
favors you") under a "blocking is mandatory" misconception — score it MODEL/representation-wall, not a guide
gap; don't reach for a guide line the prompt already states.

**Convergence-watch verdict — colored-pays-generic did NOT harden; core stays PASS.** The wave-10/11 parked
false-belief family produced ZERO actual offered-payable declines this corpus. At deck135 (vs110 s5) it fired
in first-draft reasoning ("I cannot cast Icehide Golem… no generic mana") and SELF-CORRECTED within the same
reply, casting on curve — no wrong action, confirming the read is model-UNSTABLE (fires wrong on some
derivations, corrects on others). The one adjacent single-seat witness (deck140 vs109 s9: declined an offered
Cruel Edict at a mandated sweep window) is the `Mana available` line HIDING a dual-producible color, not the
core belief — an engine representation item (the annotation showed `{r}{w}{w}{w}{w}`, no black, while the SAME
prompt OFFERED the payable `{1}{b}` Cruel Edict; black was in fact available, proven by s11 casting it next
window). The core body already states both governing facts verbatim (line 4); a 3rd copy is the
sanctioned-against escalation. Promotion trigger unchanged: a CROSS-SEAT 2nd deck's reviewer finding an ACTUAL
offered-payable decline citing inability to pay a generic pip with colored mana. Durable fix if it ever
promotes = REPRESENTATION (an option-line affordability breakdown the engine already partly does via
offered=payable), never louder core prose.

**Core status this wave: PASS (9th consecutive wave, no prompt-body edit).** No 2+-seat core candidate; the
false-belief family did not harden. `general-strategy.txt` is a byte-identical copy of the live core prompt.
Every adoption owns a layer BELOW the guide: the absent-echo staleness routing and the attackers name→index
reconcile own the code-appended reply-protocol / attackers-parser layer; the game-log-resolution and
board-state-contradiction refinements own the verification method; the Galvanic Blast fix is a card-script data
item. See `synthesis-notes.md` for the ranked engine/harness ledger, the wave-13 validation table, guide
dispositions, and the ROTATION verdict (loop CONTINUES — rotation does NOT fire: the cycle surfaced multiple
non-context engine/harness changes, chief among them the absent-echo GAME-LOSING branch).

## Superseded wave-11 synthesis notes (retained only as lineage)

Unified from seven independent deck-agent revisions (decks 44, 109, 110, 131, 133, 135, 140) of the wave-10
skill, mining the wave-11 PRIMARY corpus (`matchups-20260715-072254`, 21 games round-robin, binary
`/tmp/wagic-54a5a9d01` = the wave-10 engine batch: max_tokens 4096 + parse ramble guards + stack-target
annotation fix + fetch pre-targeting keying; answer-after-plan protocol; wave-10 guides incl. deck109's +1
hybrid line; core prompt unchanged). This is the SECOND corpus after the 2026-07-14 pause, and its job was to
VERIFY the wave-10 batch end-to-end at scale, not to open new authoring diagnoses. **Dominant mode: FREEZE —
all seven guides are byte-identical to the live baka guides** (verified by diffing each
`wave11/deckN/strategy.txt` against `bin/Res/ai/baka/deckN_strategy.txt`; zero bytes changed), and all seven
seats voted core PASS. Every adoption below is a VERIFICATION-METHOD refinement — how to SCORE a shipped (or
still-unshipped) fix — and per the epistemics rule no single-seat evidence cut a method line; every adoption
has 2+ witnessing seats or completes an existing rung. The wave-10 batch VALIDATED (see the validation table
in `synthesis-notes.md`): the stack-target annotation contradiction is GONE (0 `NO legal target right now`
corpus-wide); the ramble/truncation guards killed the wave-10 prose-hijack/scavenge class (0 scavenged casts;
every fallback -> heuristic choice -1); unparsed_reply fell 60 -> 11; the deck109 hybrid line drove its
decline/spiral class to zero; the Force reasoning-tax collapsed ~470 -> ~100 words with Force target
discipline restored. The one interface cost is DISPLACEMENT, not regression: the 4096 cap converted the
2048-truncation class into a LATENCY class — 72 empty_reply HTTP-timeout fallbacks (4.7%), all self-healing,
none game-deciding; the 240s `WAGIC_GPT_TIMEOUT` default is already committed (95cf9f5f9) for the next corpus.

**Method headline 1 — the answer-after-plan trailing PLAN/CHOICE mismatch has a SINGLE ROOT, the
UNLISTED/UNAFFORDABLE-CARD FIXATION, and it is BIDIRECTIONAL; the reply-protocol A/B's reconcile-named-action
arm must handle "named action ABSENT from the options." (PROMOTED from wave-10's single-seat WATCH — 3
witnessing seats this wave + the wave-10 lineage.)** Wave 10 named the trailing mismatch as an answer-LAST
shape that fails ASYMMETRICALLY (casts a SPELL over a HOLD) and flagged deck110-P3 (plan fixates on an
UNLISTED card, emits a poorly-mapped fallback) as a single-seat sub-variant to promote only with a 2nd seat.
Wave 11 promotes it to the MAIN shape and shows it is the COMMON ROOT of BOTH failure directions. At every
genuine deck140 mismatch this corpus the plan's NAMED action is a card NOT among the offered options because
it is unaffordable this turn (verified against the prompt's `Mana available` line), and the trailing index
mis-maps in one of two directions: **OVER-cast** — wants an unlisted FINISHER on an empty board, maps onto a
listed SWEEPER over Cast-nothing (deck140 vs135 s10, vs131 s27: Pyroclasm into `creatures:0`); **UNDER-cast**
— wants an unlisted SWEEPER/removal at C>=1, maps onto Cast-nothing over an OFFERED payable removal (deck140
vs109 s9: ignored the offered Cruel Edict at a 5-creature aggro board, in a loss). The class now carries 3
witnessing seats in wave 11 alone plus the wave-10 lineage: deck140 (own seat, both directions), deck135 Q5
(deck44 s34 under-develop: an unoffered "transform Azcanta" desire mapped to Cast-nothing over an offered
develop spell), and deck109 (vs135 seq12: the PLAN abandons its own listed Krenko's Command and fixates on
the unlisted Hellrider, emitting `CHOICE: 5` out-of-range -> -1 -> heuristic — the out-of-range flavor, the
index faithfully encoding an ILLEGAL choice as if the imagined card were an extra 5th option). Adopted: **the
reconcile-named-action A/B arm (reply-protocol ledger) must parse the plan's NAMED action and, when that
action is ABSENT from the options, route to the BEST AVAILABLE LISTED option — a payable removal if the plan
wanted removal, the pass/Cast-nothing if the plan wanted to hold or deploy an unaffordable finisher — never
an arbitrary listed cast, and reject-and-repair an out-of-range index rather than mapping it.** This DOMINATES
both "accept the last bare number" and a naive named-action match that only handles present options. Route to
the code-appended reply-protocol layer; never re-word the guide over it (the guide is read correctly
throughout). Companion single-seat DIAGNOSTIC (deck140 P2, additive, completes the wave-6 hallucinated-board
rung): before routing a "sweeper into `creatures:0`" symptom, READ the plan — the trailing-index mismatch
(plan reads C=0, concludes HOLD, only the index casts = reply-protocol) and the PHANTOM-BOARD misread (the
plan itself believes a creature present and the index AGREES = representation, here affinity's within-turn
create-then-die churn carried forward by the stale `YOUR PLAN` echo: deck110 s68/s82) share the symptom but
sit at different layers; the tell is PLAN-vs-CHOICE mismatch (reply-protocol) vs PLAN-vs-BOARD mismatch
(representation).

**Method headline 2 — the confirmed-fired-representation terminus's verification signal now REQUIRES
classifying the leak by MECHANISM and LAYER before crediting or re-diagnosing; a raw self-cast/obey-rate/usage
count conflates owners.** Wave 10 sharpened the "mis-cast -> 0" signal to "measure in the REASONING, not the
recorded choice." Wave 11 shows the leak can live at several DISTINCT layers with different owners, converging
across four seats in four shapes: (a) *reasoning-DISOBEY via a model rules-misconception* (deck133, NEW: twice
the model read `the only legal targets are YOUR OWN right now` and reasoned straight past it, fabricating that
Fatal Push can destroy an opponent creature SPELL still ON THE STACK; `CHOICE:` matched the wrong reasoning;
obey-rate held ~95%, 39/41) — owner is representation/model (the annotation may need to name WHY the on-stack
creature is excluded: stack-vs-battlefield, destroy-vs-counter), never a louder guide line; (b) *number/PARSE
leak* (the wave-10 mechanism) — CLOSED this wave: the ramble guard + range check killed the formatless int-grab
and the unoffered-desired-card case now emits an out-of-range index that is REJECTED, not mapped (deck133 vs44
seq29 `CHOICE:3` into a 2-option list -> unparsed -> heuristic); (c) *a BLOCKER-PARSER layer* (deck131, NEW,
GAME-RELEVANT): the NEVER-block-Guttersnipe absolute fired PERFECTLY in reasoning at all 3 windows (`BLOCKS:
none` emitted) yet at vs109 the ENGINE blocked Guttersnipe into a fatal trade, because bare `BLOCKS: none` is
systematically UNPARSED (corpus-wide `BLOCKS: none` -> unparsed 4/4, `BLOCKS: B<n>:none` -> parsed 3/3) and the
baka blocker fallback is NOT a no-op — for a `kind==blockers` record, `chosen_text='None'` records the PARSE
result, not the executed combat, so block-side obedience must be scored by the reply's reasoning AND the next
record's combat `events`, never `chosen_text`; (d) *the truncation/parse-GUARD signature* (deck44 + deck133):
a shipped ramble/truncation guard is CLOSED when 0 scavenged casts appear AND every fallback record carries
`fallback=unparsed_reply|empty_reply` with a HEURISTIC action (choice 0/-1). Adopted: **when scoring a shipped
annotation/guardrail (or verifying a shipped parse guard), classify each apparent leak by MECHANISM and LAYER —
reasoning-disobey / number-parse / blocker-parser / truncation-scavenge / churn-vs-correct-play — before
crediting the fix or re-opening the representation; the shapes have different owners and a raw count conflates
them.** The specific reasoning-disobey-via-stack-misconception instance stays WATCH (single-seat, 2 instances,
both won) as a named sub-shape; the routing DOCTRINE (never re-word the guide to emit `B1:none` — papering a
parser bug in prose is the sanctioned-against escalation) is firm. The bare-`BLOCKS: none` parser bug is
SHIPPED-PENDING-CORPUS (explicit all-decline now declares no blockers; AIPlayerGPT.cpp working tree).

**Method headline 3 — an annotation-SUPPRESSION / contradiction-REMOVAL fix has its own confirmed-fired
signals: the contradiction string -> 0, a reasoning-tax COLLAPSE (a category change, not a modest drop), and
the RECOVERY of any ADJACENT faculty the noise was corrupting (deck135, completes the wave-8 reasoning-tax-drop
rung).** Wave 10 diagnosed that the permanent-target `NO legal target right now` annotation was mis-firing on
the stack-targeting Force of Negation counter option, rendering a self-CONTRADICTION (`NO legal target right
now` AND `can target on the stack: X` at once) and driving ~470 words of oscillation. The suppression fix (drop
the permanent-target annotation on counter-type options) SHIPPED and VALIDATED: `grep 'NO legal target right
now'` = 0 corpus-wide (the deletion-end analogue of "marker appears"). The load-bearing SECOND-ORDER signal is
a reasoning-tax COLLAPSE — not a drop but a category change, ~470 -> ~100 words (deck140 s30-s32 Force on
Pyroclasm) — because a surface contradiction is a MULTIPLIER on a weak model's derivation length; removing it
lets the model reach the RIGHT decision it was drowning in. And a mis-applied annotation spreads WRONG ACTIONS,
so the fix's benefit is measured across an ADJACENT faculty: Force TARGET DISCIPLINE recovered (15 Force
windows / 0 casts, every 1-for-1 declined cleanly, the discard-exclusion obeyed; the wave-10 soft-target
mis-Forces of Downsize/Elixir/Cyclonic Rift did not recur). Generalized: when a shipped fix REMOVES a surface
contradiction (rather than surfacing a fact), score it by (i) the contradiction string -> 0, (ii) a
reasoning-tax COLLAPSE at the affected decision, and (iii) the recovery of any adjacent decision faculty the
noise was corrupting. **Pause-aware corollary (do not over-credit):** the ENGINE resolution bug UNDER the
annotation (Force's free alternative cost never counters — the spell resolves anyway) is UNCHANGED and
deliberately not fixed this corpus; the one clean Force cast was adjudicated mid-resolution, so the
annotation-fix win is on TAX + DISCIPLINE, NOT yet on a confirmed counter — the interface fix fired and
validated; the underlying engine fix is the separate, still-pending rung (ledger #1).

**Method headline 4 — a KNOWN-UNFIXED engine defect is re-verified by RECURRENCE with OUTCOME-COUPLING
reported SEPARATELY, and a harmless-recurrence corpus yields a cleaner regression fixture than a loss-tangled
one (deck44, the confirmed-STILL-BROKEN mirror of the confirmed-fired terminus).** The auto-tap color-stranding
defect (ledger #2) is unfixed by design; wave 10 it COST the vs140 Blightsteel game. Wave 11 it RECURRED
mechanically (deck44 vs131 s30: Archmage {4}{u} tapped all 3 Islands with 3 Swamps free; s21: Sprite {1}{u}
stranded a blue) but was OUTCOME-NEUTRAL (no counter window followed before untap; the game was won). Adopted:
when re-verifying a known-unfixed engine defect, report two things SEPARATELY — (a) did the mechanical class
RECUR (grep the cast->next-mana delta), and (b) did it COUPLE to a decision cost this corpus — because a defect
can be live-and-harmless in one corpus and live-and-fatal in another purely by trajectory; do NOT downgrade the
ledger item because a corpus was harmless, and do NOT re-inflate it into a guide line because it recurred. The
harmless corpus is USEFUL: it yields a clean, loss-decoupled repro (vs131 s30) that is a better regression
fixture for the eventual engine fix than a loss-tangled one. This is "verify the fix FIRED" applied to a fix
that has NOT YET shipped: confirm the DEFECT still fires, quantify its outcome-coupling, keep the routing, add
the clean repro. (Companion, deck110, the equip-seam instance: when scoring an OBSERVED-VIOLATED guardrail's
leak, separate the CORRECT multi-instance play from the churn leak before quantifying — a raw "N equips, marker
90/0 obeyed" tally hides the churn and a raw "same-name consecutive equips" tally falsely flags the sanctioned
two-Plating stacks; classify each run correct-stack / correct-upgrade / churn against the OUTCOME DELTA, then
quantify only the churn.)

**Convergence-watch verdict — the offered=payable / colored-pays-generic false-belief family did NOT harden
further; core stays PASS.** The wave-10 2-seat family (deck135 colored-pays-generic + deck109 hybrid-misread)
did not recur at deck135 this wave (the model self-corrected the "generic" inference in every window) and the
deck109 hybrid class went fully to ZERO under its validated card-note. The one fresh witness is single-seat
(deck140 vs109 s9: the plan asserted "I have exactly the mana for Pyroclasm {1}{R}" with `Mana available
{b}{w}{w}{w}` showing no red — the same family, bundled INTO headline 1's fixation event). The core body
already carries the governing generic/hybrid facts verbatim (line 4), so this stays a REPRESENTATION candidate
(the option-line hybrid annotation + a colored-pays-generic affordability breakdown), never a core edit;
cross-seat grep remains the promotion trigger. All seven seats independently voted core PASS.

**Core status this wave: PASS (8th consecutive wave, no prompt-body edit).** No 2+-seat core candidate; the
false-belief family did not harden. `general-strategy.txt` is a byte-identical copy of the live core prompt.
The reply-protocol residuals (headline 1) own the code-appended reply-protocol A/B; the leak-classification,
annotation-suppression, and confirmed-still-broken refinements (headlines 2-4) own the verification method, not
the core prose. See `synthesis-notes.md` for the ranked engine ledger, the wave-11 validation table, guide
dispositions, and the ROTATION verdict (loop CONTINUES — gate 1 fails: the BLOCKS:none fix shipped mid-cycle
and the engine batch remains outstanding).

## Superseded wave-10 synthesis notes (retained only as lineage)

Unified from seven independent deck-agent revisions (decks 44, 109, 110, 131, 133, 135, 140) of the
wave-9 skill, mining the wave-10 PRIMARY corpus (`matchups-20260715-031825`, one round-robin, 21 games,
binary `/tmp/wagic-c8c054be8`). This is the RESUME wave after the 2026-07-14 pause: the four wave-9
parked ledger items ALL shipped between corpora (answer-after-plan reply protocol; fetch consume-on-
choose; legal-target names on targeted casts; thin-hand discard annotation; artifact-tag completeness),
so this corpus's job was to VERIFY those shipped fixes end-to-end, not to open new authoring diagnoses.
The dominant mode was FREEZE with VERIFICATION: **six of seven guides are byte-identical** (only deck109
added one card-cost line), and every method adoption below is a VERIFICATION refinement — how to score a
fix under a CHANGED reply protocol — not a new guide-authoring headline. Per the epistemics rule no
single-seat evidence cut a method line; every adoption has 2+ witnessing seats or completes an existing
rung. The shipped fixes all VALIDATED (see the validation table in `synthesis-notes.md`): answer-last
killed the head-first intent-collapse class at all 7 seats; fetch consume-on-choose dropped the crack
window ~48% with no strategic damage; the thin-hand and empty-hand annotations were obeyed 100%; artifact
tags agreed 177/177; legal-target names killed the deck44 fabrication class; `[deals 0]`/GM bare-N fired
correctly; adj0/adj1 rows sanity-checked against seat life.

**Method headline 1 — verifying a shipped REPLY-PROTOCOL change: the structural head signal is the
truth, the old protocol's diagnostic metric is NOT, and a fuzzy detector's hit is a lead to READ, never
a count.** The wave-9 A/B (move the answer AFTER the plan) effectively SHIPPED this corpus (head-first ->
answer-after-plan: PLAN first, answer last on a labeled `CHOICE:/ATTACK:/BLOCKS:` line). Four seats
converge (deck135 P1, deck140 P1, deck131 closure, deck110 V1) on how to verify the prior protocol-
artifact class is dead: (a) the head-first intent-collapse class is confirmed DEAD by the STRUCTURAL
signal — `intent-collapse-metric.py` showing every reply now NON-NUMERIC-HEADED (every reply opens
`PLAN:`, so there is no head token to lock) AND `last-bare-int-differs-from-head = 0`; that pair is the
answer-last signature and cannot be faked by a still-broken protocol (deck135 246/246 non-numeric, 0
differ; deck110 0 true head-vs-choice mismatches). (b) **DISTRUST the metric's "reversal-language +
concluded-option != recorded choice" count across a protocol boundary** — it was built for the head-first
reply SHAPE and silently mis-scores the new one in BOTH directions: its head parser is now vacuous
(`215/215 non-numeric`), and its reversal heuristic throws index-offset/mid-plan-aside FALSE POSITIVES
(deck135: 30 flags, 0 genuine on manual read; deck140: 21 flags, ~all false, and it MISSED all 4 genuine
answer-last mismatches). The naive cross-arm read (wave-10 ~135 flags vs the 120 head-first control) would
falsely register intent-collapse as WORSENING. The generalized rule (deck140 P1, extends the verification-
metric-SPLITS rung to a protocol CHANGE): **before citing any diagnostic metric's count across a protocol
boundary, confirm the metric's PARSE still matches the reply shape; if not, hand-audit is ground truth and
the metric is rebuilt before any number is trusted.** A fuzzy reversal-flag is the reply-protocol twin of
the wave-7 "read the REPLY of the lone counterexample" — a pointer to read, never a defect count. (c)
A shipped reply-protocol change is a HYPOTHESIS until the WITNESSED defect-class goes to zero AT THE
WITNESSING SEAT, verified the same way a representation fix is (grep the corpus, count the class), with
reasoning-vs-outcome SPLIT so the close is legible: deck131 is the clean end-to-end close — the same
no-Mountain mulligan whose PLAN concludes Mulligan now LANDS on Mulligan 2/2 (wave-9 was reasoning 1/1 /
outcome 0/1; this wave outcome caught up to 2/2). This is the mirror of the wave-9 split: there the fix
fired in the reasoning while the outcome stayed wrong (leak in a lower layer); here the lower-layer fix
closed the outcome gap.

**Method headline 2 — the answer-after-plan protocol has its OWN residual (a THIRD reply-protocol shape),
and under it the recorded CHOICE is NOT a reliable metric of annotation/plan obedience.** Two convergences:
- *The reply-protocol taxonomy gains a THIRD shape (deck140 P2, 2nd witnessing seat after deck135):* the
  answer-LAST trailing PLAN/CHOICE mismatch. The reasoning PRECEDES the answer and reaches the CORRECT
  decision, yet the trailing `CHOICE:N` contradicts the plan's stated conclusion — and it fails
  ASYMMETRICALLY, casting the offered SPELL when the plan wanted to HOLD/cast-nothing (deck140 4/4: 2 wasted
  Rakdos kill shots, Pyroclasm-into-C0, Cruel-Edict-into-C0; harness indexing verified 1-based, so this is a
  MODEL number-emission defect, not an off-by-one). Distinct from head-first intent-collapse (no head token
  exists) and from classic PLAN/CHOICE MISMATCH (here the reasoning precedes and is CORRECT; only the emitted
  index is wrong). Because **the model reliably NAMES its choice in prose** ("I will hold Rakdos," "I will
  cast Staff of Nin"), the sharper A/B target is to reconcile the trailing index against the plan's NAMED
  action (or require echoing the option TEXT, not a bare integer) — this DOMINATES "accept the last bare
  number," which would have taken the wrong number in all 4 cases. deck110 P3 adds a single-seat sub-variant
  (the plan fixates on an UNLISTED card and emits a poorly-mapped fallback label that contradicts its own
  sub-conclusion about the listed options) — WATCH, promote only with a 2nd seat. Route all of this to the
  code-appended reply-protocol layer; never re-word the guide over it.
- *An option-line annotation can be OBEYED-IN-REASONING yet leak at the number/parse layer — score
  annotation obedience against the REPLY'S REASONING, reported separately from the recorded CHOICE
  (deck133 P1 + deck44 P1, 2 seats).* Wave 9 established the option-line annotation as the last
  representation rung before the number, verified by mis-cast->0 + a reasoning-tax drop. Wave 10 adds the
  failure mode that SURVIVES it: at deck133 the own-targets Fatal Push annotation was read and obeyed in the
  PLAN prose at ALL 29 windows ("Fatal Push has no valid targets... I will not cast it"), yet the recorded
  choice cast the self-Fatal-Push in 2 of them — the divergence lives DOWNSTREAM of the annotation (an
  unoffered-desired-card fallback digit mapped onto the annotated-bad option; a formatless-truncated reply
  int-grabbed to it). deck44 P1 is the same lesson from the truncation side: under answer-after-plan the
  answer is emitted LAST, so a reply that blows the token cap mid-reasoning has NO `CHOICE:` line and the
  recorded `choice` is whatever the parser SCAVENGES — a valid-looking index that can CONTRADICT a
  derivation that explicitly obeyed the annotation ("Go for the Throat: the only legal targets are YOUR OWN
  right now. So no." then a scavenged `1`). Both seats converge on: **the confirmed-fired terminus's
  "mis-cast -> 0" signal must be measured in the REASONING, not the recorded choice** — a mis-cast count that
  treats the recorded number as the obedience metric misattributes a downstream number/parse/truncation leak
  to the annotation (concluding it "failed" and inviting louder wording, when the annotation SUCCEEDED and the
  leak is a harness item). This is the annotation-shaped instance of the wave-9 "the fix fired in the
  reasoning; the leak moved to a lower layer" split, now applied to the representation rung itself. A NEW
  truncation surface is specific to answer-LAST: a reviewer scoring an annotation by mis-cast count must
  FIRST check, for every recorded mis-cast, whether the reply reached a `CHOICE:` line at all — a truncated
  reply with a scavenged digit is the 2048-cap HARNESS class (4096 shipped next corpus), not annotation-
  ignore, and must not inflate the mis-cast count or re-open the representation.

**Method headline 3 — a CONFIRMED-FIRED count-agreement representation succeeds by surface-agrees +
outcome-neutral, NOT by the false belief leaving the reasoning; a model will re-derive over an accurate
surface, and that residual stays OFF the guide when it is outcome-neutral (deck110 P1).** Wave-9's E3 lever
(tag EVERY artifact so a re-derived count agrees with the summary) SHIPPED cleanly — 177/177 windows now
agree — yet the pilot STILL folds Glimmervoid/basics into a hand-recomputed count and now mis-types
artifact creatures held in HAND. Method point (sharpens the wave-7/8 confirmed-fired terminus for a
COUNT-AGREEMENT shape): the success signal is NOT "the false belief disappeared from the reasoning" but
(a) the surface is now complete/agrees AND (b) the defect is OUTCOME-neutralized (no Galvanic Blast fired
expecting metalcraft-4 at a sub-3 board; no metalcraft-gated pick flipped). When both hold the loop is
CLOSED even though the reasoning artifact persists — do NOT re-open it with louder guide wording (the
escalate-at-a-wall anti-pattern). This is the shipped-and-obeyed-but-the-reasoning-still-wanders case, the
mirror of shipped-but-inert. Single-seat but 2+-wave and rung-completing; the hand-card type-tag sub-gap
(tag artifact cards in the HAND serialization) is a low-priority engine micro-lever, not a guide line.

**Method headline 4 — before routing an offered=payable / can't-pay fabrication to representation or core,
check whether the SPECIFIC card-cost fact is taught where the pilot reads it — the generic interface
contract does NOT inoculate against a specific-mechanic misread (deck109, refines the offered=payable
ladder).** deck109 seq21 looks exactly like the read-then-disobey wall — the pilot QUOTES the core
`offered=payable` contract and still declines an offered `{B/R}` creature — but the deciding card fact
(`{B/R}` is HYBRID, ONE red pays it) was NOT surfaced anywhere the pilot reads correctly: the option shows
`{b/r}` (misread as `{b}{r}`) and the guide had no cost note for that card, so the GUIDE rung was UNTRIED.
The generic contract and the specific-mechanic belief can BOTH be held as true, and the model spirals on
the contradiction rather than resolving it. Authoring test (extends the confirmed-fired ladder): before
routing an offered=payable fabrication to representation or core, ask **"is the SPECIFIC card-cost fact
taught where the pilot reads it?"** If the guide teaches the PARALLEL fact for a SIBLING card and the pilot
OBEYS it (deck109 casts Boros Reckoner `{R/W}` every window because the guide says "three Mountains cast
it"), the cheapest correct move is to add the same card-specific fact for the misfiring card — that attacks
the ROOT false belief before it forms. Only AFTER the guide rung is tried-and-insufficient does the
option-line annotation (representation) or a core sub-clause become the next rung. The durable cross-deck
backstop is the representation item (annotate hybrid costs on the option line: `Cast Rakdos Cackler {b/r} -
hybrid: pay with {R} or {B}`, engine ledger); the deck-guide card note is the interim (deck109 shipped it).

**Method headline 5 — a no-op / low-value marker on a REPEATABLE move/attach action must be computed from
the OUTCOME DELTA, never from target-IDENTITY (deck110 P2, the multi-copy-churn watch now fired COSTLY).**
The wave-7/8/9 multi-copy-equip-churn watch RECURRED costly this wave: deck109 T8 bounced ONE Cranial
Plating between TWO identical Ornithopters 4x/turn (net-zero, ~4 mana, in a loss), and the `(ALREADY
attached... change NOTHING)` marker fired only on the CURRENT holder, so every move to the equivalent OTHER
Ornithopter was UNmarked and read as a real state change. The marker's predicate ("already attached to THIS
target") is not the no-value predicate; the value-neutral predicate is "moving the equipment here would not
increase this target's resulting power above its current carrier's" (equivalent-or-worse carrier), which
catches BOTH the two-Platings case (wave-9 E2) and the identical-carriers case with one rule. Method note
for the representation rung: **an identity-keyed no-op marker has a structural blind spot exactly where
multiple equivalent sources/targets exist — compute it from the OUTCOME DELTA (power/board change), not
target-identity.** Routes to the engine ledger (the pilot has no surfaced gain signal to obey); the guide
already forbids the behavior, so NO guide line.

**Convergence-watch verdict — the offered=payable false-belief family reached 2 SEATS but core stays
PASS, because the core ALREADY carries both governing facts.** The wave-9 parked candidate (a specific
false mana-payment belief overriding offered=payable) now has two witnessing seats in TWO shapes: deck135
(deck110 s21: declined offered `{1}{u}`/`{1}{g}` casts believing `{g}{u}` has "0 generic, can't cast" —
colored-pays-generic) and deck109 (deck131 seq18/19/21: `{B/R}` misread as `{B}{R}`, declined a payable
creature / spiraled the budget — hybrid-misread). The FAMILY convergence is real. But the promotion is
REFUSED and core stays PASS on the decisive ground that **the core prompt body ALREADY states both facts
verbatim** (line 4: "Hybrid symbols like {U/B} can be paid with EITHER of the two colors, and any color can
pay a generic cost"). deck135's proposed core sentence restates "colored pays generic," which is already
present; deck109's hybrid fact is already present. A pilot that read a prompt containing these exact
universal facts and fabricated the false belief anyway is the representation-beats-instruction wall at FULL
strength — a louder/third copy of a present fact is the sanctioned-against escalation. BOTH witnessing
seats independently voted core PASS. The durable fix is REPRESENTATION (project doctrine: representation
beats instruction — a surfaced fact carries the load a present-but-disobeyed sentence cannot): the
option-line hybrid-cost annotation (deck109), and for the colored-pays-generic class an affordability/
payability breakdown on the option line. See `general-strategy.txt` (verbatim copy of the live core) and
`synthesis-notes.md` for the ranked engine ledger and the wave-10 validation table.

**Core status this wave: PASS (7th consecutive wave, no prompt-body edit).** The one 2-seat convergence
(offered=payable false-belief family) routes to representation, not the core prose — the core already
carries both governing facts. The reply-protocol residuals own the code-appended reply-protocol layer (the
answer-last A/B refinement), never the core prompt body.

## Superseded wave-9 synthesis notes (retained only as lineage)

Unified from seven independent deck-agent revisions (decks 44, 109, 110, 131, 133, 135, 140) of the
wave-8 skill, mining the wave-9 PRIMARY corpus (`matchups-20260714-094942`, one round-robin, 21 games,
binary `/tmp/wagic-a40671057` = wave-8 engine ledger + own-targets annotation + cast-restriction
gating + livelock breaker + broadened punisher rider, post-A/B core with the `hand -> stack` anchor
GONE). **THIS IS THE FINAL WAVE BEFORE A PROJECT PAUSE** (user decision 2026-07-14). The wave's job
was to VERIFY the shipped fixes end-to-end, freeze the guides into a standalone/shippable state, and
route every open item to a durable, self-contained layer — not to open new diagnoses. The dominant
mode across the pool was FREEZE: six of seven seats returned a zero- or near-zero guide diff, and the
method contributions are all refinements/terminus-closures of existing rungs, not new headlines. Per
the epistemics rule, no single-seat evidence cut a method line; every adoption below either has 2+
witnessing seats or completes an existing rung.

**Method headline 1 — the DEMOTION terminus gains a PAUSE-AWARE precondition: demote a guardrail only
once its class was OBSERVED OBEYED, never merely because it was ABSENT.** deck109 is the wave's
sharpest method point. Its NEVER-block-Phyrexian-Obliterator absolute (a guardrail against a KNOWN
game-loser: block-at-10 -> sac board -> loss) got ZERO test windows this corpus (deck133 was raced out
T10, Obliterator never entered a combat the pilot answered). The turnaround/demotion terminus would
normally invite compressing a guardrail once the seat is winning (deck109 went 6/6). But "no
counterexample off zero opportunities" is NOT "the rule is proven obeyed" — and with NO wave-10 to
catch a regression, demoting an untested guardrail into the pause is unsafe. Adopted: the demotion
terminus's trigger is the class having been OBSERVED OBEYED (the marker fired AND was followed / the
defect went to zero AFTER a real precondition window occurred), never mere absence; a guardrail whose
precondition did not recur stays at FULL prominence, and the pause hardens this — a closing wave
FREEZES untested guardrails, it does not demote them. The thin-sample corollary (deck109 #1): when a
re-anchor validates on few windows, state validation PER-CLASS against the windows that actually
occurred and keep any zero-window class at WATCH (the re-anchored chump rule validated 3/3 for the
high-life gang-block class but had zero Obliterator-block windows — validated for one class, untested
for the other).

**Method headline 2 — reply-protocol INTENT-COLLAPSE (answer-before-reasoning) is now MULTI-SEAT
(4 seats, 2 waves) and CROSSES the single-seat bar: the durable fix (a reply-protocol A/B) is DUE, and
the layer-routing discipline is now firm doctrine — NEVER re-word or score a guide over a
reply-protocol defect.** The head-first reply protocol commits the answer token BEFORE the model
reasons in its PLAN; when the PLAN reasons to a DIFFERENT option for the SAME window, the choice stays
locked to the stale head. Witnesses this wave: deck131 (vs44 seq1 mulligan — head `1`=Keep, PLAN
concludes Mulligan and explicitly rejects the bait, NOT self-corrected — a one-shot window, cost the
game), deck135 (vs131 s14 — head `4`=Cast nothing, PLAN reverses in text "wait, I *can* cast Coatl!",
self-corrected next phase), deck140 (2-3 instances, incl. vs131 t40 head cast Wrath into creatures:0
and vs109 t10 head Cast-nothing at 1 life, mostly self-healing via many priority windows), plus a NEW
SHAPE from deck109 (vs44 seq7 attackers — a NON-NUMERIC head, the literal word `none`, parsed to
option 0 = the WORST option at the attacker seam). Two method points adopted: (1) the routing is fixed
doctrine — route to the code-appended REPLY-PROTOCOL layer, never re-word the guide over it and never
score it against a guide rule (obeyed cleanly by 131/135/140/109); (2) the verification metric SPLITS —
when a guide/gate fix targets a bad OUTCOME, score it by whether the REASONING now reaches the right
verdict, reported SEPARATELY from outcome (deck131: no-red mulligans reasoning 1/1, outcome 0/1 — the
guide fix is VALIDATED, the residual outcome gap belongs to the reply-protocol layer). A fix that
corrects the reasoning while the outcome stays wrong is the mirror of confirm-the-fix-FIRED: the fix
fired in the reasoning; the leak moved to a lower layer. The A/B (accept the LAST bare number / move
CHOICE after the PLAN / reconcile a trailing named index against the head / reject-and-repair a
non-numeric head) is now DUE, judged by decision-quality on the symmetric core. It is NOT a core
prompt-body edit.

**Method headline 3 — an else-branch develop-ORDER must gate on the OPTION LIST, not the executor's
HAND, and must name every SHAPE the develop action can take.** deck135's wave-8 else-ORDER
(develop when no creature is castable) VALIDATED for its DIRECT case (directly-castable Into the North
declined-to-hold went to zero; vs44 casts on curve and takes the opponent to 2) but the defect
PERSISTED in a harder sub-case: when the enabling play is not a directly-castable spell but CRACKING AN
UNCRACKED FETCH for a missing color. The pilot read a creature in its HAND as "castable" and let Rule
#1's own creature-before-fetch ordering VETO the fetch that would enable it, quoting the guide while
misplaying ("Do not fetch lands if I have a creature to cast... adhering to Rule #1," verbatim vs133
s24). Adopted (deepens the wave-7 "state BOTH branches as countable conditions" + the wave-8 else-ORDER
one level): (a) re-gate the else-branch on "no creature among your numbered Cast OPTIONS" and state
flatly that a creature in HAND absent from the options is NOT castable, so the fetch that fixes its
color IS the creature play (not "fetching instead of a creature"); (b) enumerate the develop SHAPES
symmetrically — a directly-castable fix AND a fetch-crack for the missing color — de-biased from one
color (the guide's "almost always the ISLAND / short BLUE" blinded the pilot when the missing color was
GREEN). Also adopted from deck135 P2 (completing the wave-6 "a FREE reaction still costs a CARD" line
from the target side): a reactive spell that carries a fixed card COST must write its target exclusion
as the ECONOMY rule ("never spend a 2-card answer on a 1-card problem") and NAME each 1-for-1 class the
executor treats as in-scope (face burn, discard, a single small creature/token spell) — an
ONLY-on-[list] positive is under-specified because the executor reads any counterable spell as in-scope
(deck135 Forced an Inquisition of Kozilek this wave, a discard the "board-wipe/mass-bounce/kill-only"
list did not explicitly exclude).

**Method headline 4 — the OPTION-LINE ANNOTATION is a FOURTH representation-fix shape, verified by
mis-cast->0 AND a REASONING-TAX DROP, and it is the LAST rung before the number is emitted when a
surfaced fact is still fabricated over.** The wave-9 binary shipped the own-targets annotation ("the
only legal targets are YOUR OWN right now") on a targeted-removal option whose only legal targets are
the pilot's own permanents. deck44: fired 47×, **0/47 self-target casts**, and the reasoning tax
DROPPED (median plan 519 chars at annotated windows vs 871 at un-annotated "no target" windows; the
confused self-negotiation "X is an artifact? No... Wait..." appeared ONLY where the annotation was
absent). Adopted as a fourth shape on the confirmed-fired-representation terminus (alongside
weak-marker / absent-option / re-anchor-to-count), with two shape-specific verification signals: the
mis-cast count -> 0, AND a new one — a REASONING-TAX DROP (shorter/cleaner plans at the annotated
windows vs the un-annotated windows of the same decision class; a shorter derivation is fewer places
for a weak model to talk itself into the wrong branch, so an annotation can fire, be obeyed, AND still
be worth measuring by whether it shortened the derivation). deck133 #3 is the same family from the
opposite pressure: `Opponent hand size: 0` was surfaced AND the guide forbade Thoughtseize-into-empty
in TWO places, yet the pilot FABRICATED a card over it ("the opponent may have a removal spell") —
the read-then-disobey wall in its hallucinated-fact shape, whose only remaining rung is annotating the
OPTION ITSELF (`Cast Thoughtseize {b} - opponent's hand is EMPTY; this only costs you 2 life`), the
last place before the number is chosen. The authoring test: if a fact is BOTH surfaced in CURRENT
SITUATION AND stated in the guide and the pilot still fabricates over it, the only rung left is the
option-line annotation — never a third copy of the rule. **Demotion caveat (conditional-surface
granularity, per the wave-8 per-belief rule):** demote the guide stopgap the annotation replaces ONLY
when the annotation's surface covers EVERY case the stopgap covered. The own-targets annotation fires
only when the pilot HAS its own creatures as legal targets; it cannot fire on the ZERO-legal-target
case (pilot holds no creature, opponent holds none), where the option renders bare and the executor
still gets confused — so the guide fallback is PARTIALLY double-covered, KEEP it, and finish the
demotion only once the zero-target case is also represented (`[no legal target]` marker — engine
ledger).

**Convergence-watch verdict — reflexive/mis-framed blocking's SHIPPED remedy (the blocker-seam lethal
line) is now PRESENT-AND-WINNING where windows occurred, but its PUNISHER-rider half is only now
verified.** deck109's blocker-seam lethal line ("Unblocked... you would be at M - NOT lethal / LETHAL")
fired in all 3 deck109 blocker windows and was OBEYED 3/3 (the wave-8 read-and-disobey "present-and-
losing" wall did NOT reappear on the windows that occurred — but the sample is THIN: 3 windows, only
the high-life gang-block class tested). The Obliterator punisher rider (wave-8's un-verified half) is
now CONFIRMED rendering and respected — but only from the CONSUMING seat, which is the OPPONENT of
deck133 (deck133 #2): OPP135 seq38 + OPP131 seq56 both rendered the `{text: ...sacrifices that many
permanents}` rider on the incoming Obliterator and both defenders DECLINED the block. Verdict
UNCHANGED: still REPRESENTATION, NOT a core sentence (rejected 5×). The `[deals 0]` / `[not lethal]`
blocker-option rider remains the un-shipped completion (deck44 Archmage-blocks-Ornithopter did not
recur only because vs110 had no blocker phase). Method note adopted (extends composition-seam-across-
seats): to verify a representation that renders for the OPPONENT of the reviewed deck, grep the
OPPONENT's seat file at the seam the reviewed deck's card creates — the fix and its consumer live in
different files AND different seats.

**Core status this wave: PASS (6th consecutive wave, no prompt-body edit).** No 2+-seat core candidate.
The one general-flavored single-seat candidate — deck135's "colored mana pays generic / offered =
payable" false belief (vs140 s24: pilot declined an OFFERED `Cast Astrolabe with cost {1}` believing
{G} cannot pay a generic {1}) — is the same FAMILY as the core's existing "Mana in your pool: (none)
read as I cannot cast" correction, guide-patched locally this wave, and PARKED as a core candidate
pending a 2nd seat (grep other seats' declines of offered casts). Reply-protocol intent-collapse is
NOT a core prompt-body item (it owns the code-appended reply-protocol layer; the fix is the A/B).
`general-strategy.txt` is an unchanged copy of the live post-A/B prompt. See `synthesis-notes.md` for
the ranked parked ledger, the wave-9 validation table, and the resume ordering.

## Superseded wave-8 synthesis notes (retained only as lineage)

Unified from seven independent deck-agent revisions (decks 44, 109, 110, 131, 133, 135, 140) of the
wave-7 skill, mining the wave-8 PRIMARY corpus (`matchups-20260714-044131`, one round-robin, 6
games/deck, the UNCHANGED-core control arm; a SECONDARY arm removed the `hand -> stack` anchor
sentence for the A/B). Wave 8 is the wave where the REPRESENTATION fixes the prior three waves
designed all SHIPPED at once and were put to the end-to-end test — and the dominant result is a
CLOSING wave: multiple multi-wave loops reached their terminus together, and the method's job this
wave was to codify the terminus as a single generalized doctrine, not to open new diagnoses.

**Method headline 1 — the confirmed-fired-representation DEMOTION terminus GENERALIZES across every
representation-fix SHAPE, and the verification signal is shape-specific.** Wave 7 closed the terminus
for ONE shape (deck110's weak-marker re-equip fix: grep the marker string, then demote). Wave 8
proves the SAME terminus for two more shapes at three seats, and at the deletion end at a fourth:
- **ABSENT-OPTION fix** (deck131): the wave-7 land-menu defect (a guide rule named "Play Mountain,"
  an option the engine never co-offered) shipped as land-drop enumeration. Verified NOT by a
  board/reply read but by the OPTION APPEARING — 12 decisions co-offered "Play Mountain"+"Play
  Island" (0 last wave), pilot chose Mountain 12/12 by NAME. Confirmation is the demotion trigger:
  deck131 dropped the "hold non-Mountain lands to reach the Mountain" stopgap AND the now-stale
  "one-land-at-a-time menu" explanation (a stale interface caution reinforces the wrong posture).
- **RE-ANCHOR-TO-A-COUNT fix** (deck140): the wave-7 sweep re-anchor (fire if `(creatures: N)`
  N>=1 & under pressure; hold if N==0) shipped BOTH as the guide's trigger AND as printed
  representation (`Opponent battlefield (creatures: N)` + `[tapped - untaps and can attack next
  turn]`, 86 firings). The overshoot class went 8 -> 0. A re-anchor-to-a-count and a surfaced-count
  representation are THE SAME FIX AT TWO LAYERS and share the terminus: once the count is both the
  trigger AND the printed marker, the inline false-belief kills the count subsumes are demotable.
- **DELETION end** (deck133): the Gray Merchant drain-display fix shipped; `{right now: drains 8}`
  resolved as exactly 8 (own devotion included). The guide's "+2" workaround is DELETED — a second,
  cleaner instance of the deletion-end lifecycle, and the deletion fired correctly off a SINGLE cast
  because it agreed with the independent probe. **Propagation swept: no other guide carries a GM
  "+2" note** (deck44 only lists GM as an opponent threat to counter).
Adopted: the Step-0 verify-FIRED bullet's terminus now names all three fix shapes and their
shape-specific verification signals (marker-string grep / option-appears / defect-class->0).

**Method headline 2 — demote a belief-kill block PER-BELIEF against what the representation LITERALLY
SURFACES; a count carries "how many," never "how your spell interacts."** deck140's shipped
`(creatures: N)` count dissolves "they're tapped -> no creatures" (tapped creatures are IN N, and the
tag says they untap) and "artifact board -> hold" (artifacts don't change N) — both demoted. It does
NOT dissolve "shroud stops my sweeper," because shroud is a TARGETING misconception and a creature
COUNT says nothing about whether your removal targets — KEPT (compressed). Adopted: before demoting a
block of belief-kills against a newly-shipped representation, check EACH belief against the
representation's literal surface; demote only the beliefs the surface answers, keep the rest as
guide-only facts. This refines the demotion terminus to per-belief granularity.

**Method headline 3 — validate a re-anchor the way you validate a representation fire (defect class ->
0), but ONLY after joining castability + intra-turn context; and a validated re-anchor puts the seat
in TURNAROUND mode.** deck140's raw "held-with-creatures-present" tally produced 7 candidates, ALL 7
non-overshoot on inspection (2 mana-locked, 3 a first pass in a turn that DID sweep, 2 a lost position
with no killing sweeper) — reported naively, "7 holds" would have looked like the overshoot
persisting. Adopted (extends confirm-before-accuse + turnaround-preservation): a re-anchor is
validated by its defect class going to zero AFTER the castability/intra-turn join, never by the raw
tally; and after it validates on its first corpus, treat the seat as turnaround — demote the
double-covered lines, freeze the obeyed ones, and hold any NEW permissive/fire condition to a WATCH
(a new high-life fire condition is exactly the widening that produced the original overshoot).

**Merged into the method this wave (multi-seat, cross-cutting, or completes an existing line):**
- *The confirmed-fired-representation DEMOTION terminus generalizes across fix shapes (weak-marker /
  absent-option / re-anchor-to-count) with shape-specific verification signals; the DELETION end has
  its own propagation sweep* — 3+ seats (deck140 P1, deck131 #1, deck133 #1), the wave's headline.
- *Demote a belief-kill block PER-BELIEF against what the representation literally surfaces* (deck140
  P2) — a count carries "how many," a tag carries "state," neither carries "how your spell interacts."
- *A NEW engine-surfaced value re-opens the manual-recompute audit — run it against EVERY value the
  engine STARTS surfacing this wave, not only ones a prior wave flagged* (deck110 P1) — the pilot
  hand-computes the old way until the guide points at the new line, and that hand-computation is
  exactly where the deck-specific miscount lives (deck110 folded Glimmervoid+lands into a hand-counted
  artifact total while `Artifacts in play: you N` sat accurate 180/180).
- *Validate a re-anchor by defect-class->0 AFTER the castability/intra-turn join; a validated re-anchor
  = turnaround mode, constrained diff not new fire conditions* (deck140 P3, P4).
- *An else-branch of a priority rule needs its own IMPERATIVE scoped to a countable condition, not a
  PERMISSION — and it must NAME the anti-pattern the executor will otherwise invent* (deck135 P1). The
  displacement-into-the-else-branch third witness: deck135's obeyed "deploy before value" pushed the
  leak to the "nothing to deploy" branch, where a permission ("or spend leftover mana on value") lost
  to the executor's self-generated "hold mana for a creature" instinct (Into the North, the blue fix,
  declined 4× then cast at 5 life, died next turn). Completes the wave-5 DISPLACEMENT line + wave-7
  deck140 "state BOTH branches as countable conditions together"; the authoring move is else-PERMISSION
  -> else-ORDER with a scope guard ("fires ONLY when no creature is castable — a listed creature still
  wins point 1").
- *A keep-side GUARDRAIL added to stop a false-mulligan can itself INVERT into a false KEEP — make the
  gate STRICTLY ORDERED and subject the keep on the SCARCE RESOURCE, not the payoff* (deck131 #2). The
  false-belief-bridge lesson (deck140 wave-7) applied to a two-sided decision: the executor read the
  keep-side guardrail ("a hand with a Mountain and a Young Pyromancer is ALWAYS a keep") as the WHOLE
  rule, dropped "Mountain and," and kept a 0-Mountain no-draw hand on the standalone "Young Pyromancer"
  token. Kill the bridge by STRUCTURAL ordering, not more words: the disqualifying step (no red source)
  decides on its own and is un-overridable by any keep-side clause; no keep-side sentence may name a
  card that could stand alone as a trigger. Corrects the wave-7 deck131 P3 line that ADDED the
  guardrail (both error directions must be tracked every wave — a guardrail can invert).
- *Score a rule that carries a VALUE FLOOR by DECISION-CORRECTNESS, not raw usage-rate* (deck44 P1) —
  when a reactive/removal rule installs restraint (decline the cheap targets), a FALLING take-rate is
  consistent with IMPROVED play; classify each window correct-take / correct-let / error and report
  n-correct/n-windows. deck44's counter seam fell 13/14 -> 9/15 raw but was 15/15 CORRECT (6 correct
  value-floor let-resolves). The mirror of wave-7's "count OFFER windows, not just re-picks" (here the
  trap is the opposite direction: reading correct restraint as under-use); aligns with the USER
  EVALUATION DOCTRINE (the unit of analysis is the DECISION, not usage-rate).

**Adopted from single seats (additive diagnostic or watch, not core):**
- *REPLY-PROTOCOL INTENT-COLLAPSE (answer-before-reasoning)* as a distinct harness-sweep sub-class —
  the "number-first, then PLAN" protocol commits the head token BEFORE the model reasons in its PLAN,
  so a model that reasons itself to a DIFFERENT same-window option is already locked to a stale head
  number (deck135 vs131 s48: head `4`=Cast nothing, PLAN concludes "Option 2, cast Icehide Golem,"
  self-corrected next phase). Distinct from PLAN/CHOICE MISMATCH (there the head int MATCHES a passive
  option and the reasoning is genuinely for a different action; here head and reasoning are for the
  SAME window but the head was emitted first and never revised). Route to the reply-protocol layer,
  never re-word the guide over it, never score it against a guide rule. Single instance -> the durable
  fix is a reply-protocol A/B (accept the LAST bare number / move CHOICE after the PLAN), judged by
  decision-quality not win-rate (symmetric core). WATCH.

**Convergence-watch verdict — reflexive/mis-framed blocking's SHIPPED remedy (engine ledger #5
lethal-arithmetic) is now PRESENT-AND-LOSING, and its PUNISHER-RIDER half did NOT ship.** deck109 is
the wave's sharpest data point: the blocker-seam lethal line ("Unblocked... you would be at M - NOT
lethal") FIRED in 3/3 blocker prompts and the pilot READ-AND-DISOBEYED it in BOTH loss games
(gang-blocked a 6/6 at 19 life; blocked Phyrexian Obliterator into its sac-punisher at 10 life, going
manaless). Separately the #5 PUNISHER rider did NOT fire for Obliterator (A2 showed only "(5/5)
[trample]," no sacrifice-text rider, while A1 got a full `{text:}`). deck44 vs110 adds a `[deals 0]`
gap (Archmage win-con blocked a 0/2 Ornithopter, no not-lethal marker). Verdict UNCHANGED: still
REPRESENTATION, still NOT a core sentence (rejected 4×) — but the routing note sharpens to "the shipped
half (lethal math) is insufficient alone; complete the PUNISHER/deals-0 rider (ledger #3) AND consider
a harder directive form of the not-lethal rider." A representation that is CORRECT, SURFACED, and STILL
bypassed is the representation-beats-instruction WALL at the blocker seam — do not escalate wording.

**Core status this wave: PASS (5th consecutive wave, no prompt-body edit) — and the scheduled A/B is
DONE.** The `hand -> stack` cast-happened anchor A/B RAN and the removal is ADOPTED (cast-nothing 41.5%
vs 41.9%, consecutive same-pick 12 vs 12, same-name double-casts 13 vs 14 — no regression; defect fixed
at source 353f1a150 + e11b97a77). The live prompt now CARRIES the removal (commit 6d02cb68c) — the
anchor sentence is GONE. No new prompt-body edit this wave: all four single-seat core candidates
(deck131 `damage>=toughness=dead`; deck135 reply-protocol intent-collapse — reply-protocol layer, not
prompt-body; deck140 shroud-vs-non-targeted; deck140 tapped-attacker — now carried by representation)
fail the convergence gate. See `synthesis-notes.md` (ranked engine ledger + wave-9 watch list); the
core prompt is an unchanged copy of the live post-A/B prompt.

## Superseded wave-7 synthesis notes (retained only as lineage)

Unified from seven independent deck-agent revisions (decks 44, 109, 110, 131, 133, 135, 140) of the
wave-6 skill, mining the wave-7 corpus (`matchups-20260713-222831`, one round-robin, 6 games/deck).
Wave 7 is the wave that CLOSED two multi-wave representation loops the earlier waves only opened, and
its value was almost entirely in the REPRESENTATION rung — NOT in guide rewrites. Most seats were
OBEYED-BUT-LOSING or turnaround; the one real model-fixable guide fix (deck140's sweeper collapse)
turned out to be a NEW overshoot mechanism worth a headline.

**Method headline 1 — a CONFIRMED-FIRED representation fix has a NEXT step: DEMOTE the guide stopgap
it replaced.** The wave-6 skill opened the trichotomy (shipped-but-inert / shipped-but-weak /
shipped-and-obeyed) and stopped at "verify it FIRED." deck110's re-equip marker is the first
end-to-end CLOSE: the field fix landed (`AIPlayerGPT.cpp:1163-1173`), the marker string now appears,
and it is OBEYED — corpus-wide **75 offered / 1 taken**, deck110 seat **29/28, no-op 50%→6%** (the one
"taken" is a plan/choice mismatch, not the marker ignored). So the interim GUIDE defense is now DOUBLE
COVERAGE — pure attention tax — and must be demoted per the existing three-part rule. Adopted: the
verify-FIRED bullet gains its terminus (demote the replaced stopgap to the corrective fact + marker
string + release + destination + ONE backup cue), and a companion — read the REPLY of the lone
counterexample before re-opening the representation (an intent-collapse at a correctly-represented
option is a reply-protocol defect, not a weak marker).

**Method headline 2 — a widened HOLD/brake over-fires INDIRECTLY, through the executor's pre-existing
false beliefs (the FALSE-BELIEF BRIDGE).** deck140's wave-6 widened hold ("hold when the opponent has
NO creatures") collapsed the deck (0/4+2TO) not by direct over-application but because the executor
reached the hold on 2-3-creature boards via three bridges, each quoted from its own PLAN and
contradicted by the same board: "they're TAPPED" → "no (untapped) creatures"; "mostly ARTIFACTS" →
"sweeper useless"; "SHROUD" → "can't remove." Adopted: after widening any permissive/hold/brake
condition, enumerate the false beliefs the executor already holds and KILL each bridge inline as a
flat falsity-fact OR re-anchor the condition to a HARD board-readable COUNT the belief cannot
reinterpret. Corollaries adopted: a corrective FACT over-generalizes exactly like a brake — scope it
to a COUNT the executor reads directly, never to a board ARCHETYPE it pattern-matches (deck140 keyed
on "artifact board" and dropped the "no-creature" qualifier); and the mid-calibration after an
overshoot is RE-ANCHOR, not swing-back — a HOLD decision has two branches and EACH needs its own
countable condition stated together (fire when C≥1 and under-pressure; hold only when C==0), which
excludes both failure modes at once instead of recreating the prior wave's waste.

**Method headline 3 — run the REPRESENTATION rung on the MENU, not just the board: a guide rule is
UNEXECUTABLE when the option it names is never OFFERED.** deck131's "Mountain-first" rule assumed a
combined land menu that does not exist — the engine offers lands ONE AT A TIME as "Play <land> /
Hold <land>" and never co-offers two (verified: 0/6 games offered both "Play Mountain" and "Play
Island"). This is distinct from the wave-4/5 hidden-FACT lesson: here the ACTION ITSELF is absent
from the list, so rewording the rule forever is the trap. Adopted: when a guide rule tells the pilot
to PICK a specific option, grep the corpus to confirm that option is actually offered at the
decisions it targets — the composition-seam rule pointed at the option list. Corollary adopted: a
color/mana loss attributed to the manabase can be PARTLY an interface artifact — separate "the source
wasn't drawn" (deck construction) from "the source was in hand but unofferable" (representation); the
two have opposite owners (deck131 vs44/vs109: red source in hand from T1, menu withheld it).

**Merged into the method this wave (multi-wave, cross-cutting, or completes an existing line):**
- *A confirmed-fired representation fix licenses DEMOTING the guide stopgap it replaced* — the
  terminus of the shipped-but-inert/weak/obeyed trichotomy; plus read-the-reply on the lone
  counterexample (110 P1, P2). Multi-wave (5→6→7).
- *A deleted stopgap gets a next-wave REGRESSION GREP for the misreasoning it patched* — the mirror of
  "verify a shipped fix FIRED," closing the stopgap lifecycle at the deletion end (109). deck109's
  deleted Hellrider fallback left 0 recurrences of the phantom "missed cast" framing across 6 games.
- *Run the representation rung on the MENU — a rule naming an UNOFFERABLE option is a platform defect*
  (131 P1); *separate not-drawn from in-hand-unofferable before attributing a mana loss to the
  decklist* (131 P2).
- *OVERSHOOT VIA FALSE-BELIEF BRIDGE — a widened hold is satisfied through a pre-existing false belief;
  kill the bridge inline or re-anchor to a hard count* (140 P1); *scope a corrective FACT to a COUNT,
  not a board ARCHETYPE* (140 P2); *mid-calibration is RE-ANCHOR, not swing-back — state both branches
  of a hold as countable conditions together* (140 P3).
- *A capability/target ABSOLUTE must never be nested inside a MODE rule that later commands its
  opposite* — the intra-guide contradiction fires at a LIFE BRACKET / decision seam, not only at the
  top of a ranked list; hoist the absolute above the mode rule and re-state it as the mode's explicit
  exception (44 P1). deck44 obeyed "don't block Obliterator" at L20 and violated it at L11 when the
  chump-mode paragraph that BURIED it activated.
- *Resolved-vs-asserted narration check* — before crediting a cast's EFFECT (a drain, counter, kill),
  confirm `stack -> battlefield` (or the effect's own life/zone events); `stack -> graveyard` after an
  opponent spell = COUNTERED, and the PLAN will assert the intended effect regardless. The
  resolution-stage twin of the cast-happened anchor (133 P1).
- *A per-deck stopgap for a deck-AGNOSTIC engine defect must be class-checked across SEATS* — grep ALL
  stderr (not just the reviewer's deck) for the defect string to confirm the class MIGRATED vs
  resolved; the composition-seam rule across seats (44 P2). GFTT's no-target defect walked to
  Liliana(4×)/Fatal Push(2×) exactly as predicted.
- *When quantifying a RE-OFFER defect's cost, count OFFER WINDOWS, not just consecutive re-picks* — a
  re-offered repeatable activation has TWO cost surfaces (the waste-loop AND the attention/latency tax
  of declining it every window); 44-97 held-fetch windows/game fed deck140's timeout (135). *And do
  NOT read a raw HELD-count as overshoot without JOINING the board state* — filter each held decision
  by the deciding board fact before scoring, or a raw tally conflates correct holds against an empty
  board with fatal holds against a real threat (140 P4).
- *A sharpened COUNTABLE rule can INVERT into a false positive when the executor cannot reliably
  compute the count* — anchor the trigger to a LITERAL STRING-PRESENCE test ("is 'Mountain' in your
  hand list?") rather than an arithmetic count, add the keep-side guardrail, and track BOTH error
  directions (131 P3: the wave-6 "count the Mountains; zero = mulligan" false-mulliganed a 2-Mountain
  keeper, the pilot reading its own two-Mountain hand as "zero").
- *A pilot that manually RECOMPUTES a value the option already displays is a REPRESENTATION defect —
  even in a WIN* — the display isn't trusted / isn't the resolved value; make the option show the
  FINAL value and the guide's "add N" workaround becomes deletable. A win-only review misses this
  (133 P2: pilot ignored `{right now: drains 5}`, hand-counted devotion, erred, won anyway).

**Adopted from single seats (additive method or watch, not core):**
- *STALE-PLAN-AROUND-AN-UNCASTABLE-CARD* as a multi-turn variant of plan-diverges-from-legality — the
  pilot re-affirms a cast the engine never surfaces because the mana never arrives; NOT phantom-board
  (the board read is CORRECT), NOT a single-window unaffordable reconcile; owner is construction
  (manabase) + stale-plan poisoning, stopgap is a list-anchor reinforcement tied to the named card
  (135). Sub-note under the plan-diverges diagnostic.
- *DISCARD threat-axis blind spot* — a disruption line's value is bounded by whether the opponent's
  clock is CARD-bound (discard helps) or BOARD/WIDTH-bound (discard barely helps; the answer is a
  wall/sweeper); check before crediting a discard plan vs an aggro matchup (133 P3). One-line watch in
  Step 1 redundancy/role analysis.
- *UNTOUCHABLE-BOARD matchup class* — opponent's whole threat base is unblockable AND untargetable by
  your deck; reads like a play problem, is a deck-construction/meta fact ("race harder" is already the
  plan). The evasion cousin of the Obliterator named-matchup note; a good exemplar for "route to deck
  construction, don't write louder prose" (109).

**Convergence-watch verdict — reflexive/mis-framed blocking gains a THIRD guise; still REPRESENTATION,
NOT a core sentence.** Wave-6's one 2-seat convergence now has three shapes across five witnessing
seats: high-life chump (deck44/deck109 wave-6), block the ONLY reach creature at mid-life framed as
survival (deck131 vs110, L14 — the "above 8" floor does not catch L14), and block a damage-PUNISHER
(deck44 Obliterator at L11, reframed this wave as an intra-guide nesting defect). The core floor
EXISTS and the pilot disobeys at distance → surface life-vs-lethal AND the punisher rider at the
blocker seam (engine ledger #5). NO new core sentence; "racing creatures don't block" would misplay
every control deck. Per-deck floors stand as interim.

**Core status this wave: NO prompt-body edits (PASS, 4th wave running).** All seven seats returned no
core edit; the only convergence (reflexive blocking) routes to representation. ONE scheduled change:
the `hand -> stack` cast-happened anchor A/B is now GO for wave-8 — the wave-6 condition (a SECOND
consecutive clean no-op corpus) is MET (0 cast no-ops across all 7 seats, 2 waves; defect fixed at
source 353f1a150). It stays in the shipped prompt until the A/B runs (an A/B needs a control arm). See
`core-prompt-revision.md` and `synthesis-notes.md` (ranked engine ledger + harness knobs +
wave-8 inheritance).

**Historical (wave 6) synthesis, condensed for continuity:** wave 6 established verify-a-shipped-
representation-fix-FIRED (shipped-but-inert looks like shipped-but-weak in the win column); the
intra-guide contradiction (a #1 rule under-fires because a sibling rule forbids its action);
precondition-counting before calling a leak fixed (rare-event-A/B-power trap); size the residual
lever's CEILING before the loud #1 slot; hallucinated-board phantom lethal; VALIDATION-DROP (chosen
recorded, heuristic played); a block-seam survival rule needs its attack-seam partner; a conditional
payoff at the top of a ranked list needs its ELSE stated; split same-role cards by SELF-COST; META
SHIFT. All held in wave 7 and remain in the method below.

## Superseded wave-6 synthesis notes (retained only as lineage)

Unified from seven independent deck-agent revisions (decks 44, 109, 110, 131, 133, 135, 140) of the
wave-5 skill, mining the wave-6 corpus (one round-robin, 6 games/deck). Wave 6 is the wave where the
wave-5 representation fixes were PUT TO THE TEST and the results split sharply by whether the fix
actually FIRED. deck44's stack surfacing is the clean win — the counter seam went 2/27 → 13/14 with
ZERO hallucinated resolutions and the two sweeper-loss matchups flipped to wins: end-to-end proof
(bug → route → ship → win) that a first-class CURRENT SITUATION line beats any amount of louder
wording. But deck110's re-equip marker SHIPPED-BUT-INERT: it checks `auraParent` while equipment
stores its host in `source->target`, so the "(ALREADY attached)" string appeared ZERO times and the
no-op rate was unchanged (9/18). Same diagnosis, opposite outcome, and the ONLY way to tell them
apart is to grep the new corpus for the marker string. That contrast is the wave's method headline.

**The dominant method theme: a shipped representation fix is a HYPOTHESIS until you see it FIRE.** A
shipped-but-inert fix looks identical to a shipped-and-obeyed-but-weak fix in the win column, but the
remedy is opposite (fix the code vs. abandon the approach). Before re-diagnosing any leak a prior wave
routed to representation, confirm the representation ACTUALLY APPEARS on the decisions it targets — the
composition-seam rule applied across waves, since the fix and the diagnosis live in different files
(deck110 P1). The re-equip marker is being FIXED for wave-7 (also test the equipment host field); wave-7
authors: verify it fires before crediting it.

**The second theme: a #1 rule can UNDER-fire not from weakness but because your OWN guide forbids it.**
deck140's "HELD FOR LETHAL" Rakdos rule was disobeyed 6/7 — not ignored, but VETOED by a sibling
cast-nothing clause ("otherwise cast something") that did not list "hold" as a permitted output. The
executor resolves a contradiction toward the rule that NAMES THE OPTION IN FRONT OF IT: concreteness
beats prominence, the intra-guide mirror of representation-beats-instruction. When a #1 rule under-fires,
audit the rest of the guide for a rule whose scope EXCLUDES its action before shouting it louder (deck140
P1). Adjacent: when a wave's engine change REMOVES a guard (353f1a150 lifted the combo-hold hint-vetoes),
re-audit for the misplays the guard was silently masking — they resurface as new guide work (sweepers
fired into artifact-only boards the moment the "2+ creatures" veto lifted; deck140 P2).

**Merged into the method this wave (multi-seat or completes an existing line):**
- *Verify a shipped representation fix FIRED before crediting or re-diagnosing it* — grep the corpus for
  the marker string; shipped-but-inert and shipped-but-weak look identical in the win column (110 P1).
- *Intra-guide contradiction — a #1 rule under-fires because a sibling rule forbids its action*; widen
  the lower rule's exception, do not shout #1 louder (140 P1). *Removing an engine guard resurfaces the
  behavior it masked — re-audit for it* (140 P2).
- *Precondition-count before calling a prior-wave leak FIXED* — a zero-count of the target misplay is
  confounded when the precondition rarely occurred; a vanished misplay off <5 precondition windows stays
  at full prominence, not demoted (rare-event-A/B-power trap applied to leak resolution; 131 P1).
- *Size the residual lever's CEILING before giving it the loud #1 slot* — a lever dominated by the
  deck-construction residual earns a labeled seam block, not the top slot (131 P2).
- *HALLUCINATED-BOARD PHANTOM LETHAL* as a distinct Step-2 plan-failure class — the PLAN asserts a
  permanent on its OWN battlefield that was never cast, computes a lethal, and declines development;
  distinct from plan/choice-mismatch, stale-plan deferral, and plan-diverges-from-legality; the deciding
  fact is fully surfaced so it routes to model-experiments/representation, not louder wording (135 P2).
- *VALIDATION-DROP (chosen recorded, heuristic played)* as a fifth harness-defect-sweep class — a
  targeted pick that failed target-validation and the engine silently deferred to the heuristic; visible
  ONLY in `game-*.stderr`, invisible in the JSONL (44 P1).
- *A survival rule at the BLOCK seam needs its ATTACK-seam partner* — if the pilot taps out attacking,
  the block seam never arrives and "chump at ≤8" can never fire; write the survival instruction at the
  attack seam ("don't attack with a creature you need to block"); the mirror of wave-5's role-check-
  names-its-seam (110 P3).
- *A conditional payoff at the TOP of a ranked list needs its ELSE stated* — the executor reads the
  POSITION ("#1 = my best play") and drops the gating clause; state the false branch inside the entry
  (133 P1). *Split two same-role cards that differ in SELF-COST* so the cost-gated brake attaches only to
  the one that pays it (Thoughtseize 2 life vs free Inquisition; 133 P2).

**Adopted from single seats (additive method or watch, not core):**
- *META SHIFT as an attribution note* — a rule winning because the opponent COULDN'T punish it, now
  losing because a platform fix made the punishment work (the surfaced stack revived opponents' dead
  counters against deck110's affinity); record as deck-construction/meta, do not "fix" the guide against
  it (110 P4).
- *A FREE reaction still costs a CARD — scope by card cost* — Force of Negation's mana is free but it
  pitches a blue card; the executor reads "free" as "always fire" (135 P4). SINGLE SEAT → per-deck +
  watch; promote only if a second free-alt-cost reaction deck reproduces it.

**Convergence-watch verdict — reflexive high-life blocking: route to REPRESENTATION, not a new core
sentence.** Two seats now show it (deck109 wave-5 terminal-DON'T fired 0×; deck44 wave-6 chumps flyers /
blocks Phyrexian Obliterator at L20 asserting a false "prevent lethal"); deck135 and deck131 show it
CLEAN, deck110 N/A (0 blocker windows). Both witnessing seats agree the fix is NOT a louder core line:
the core ALREADY carries the high-life blocking floor ("blocking is optional... block only when you can
name the gain") and the pilot disobeys it at distance — a rule present-and-losing, which routes to
REPRESENTATION (surface life-vs-lethal / a `[not lethal]` marker at the blocker seam), never a second
paragraph of the same instruction. Kept per-deck as interim (deck109/deck44 floors); NO new core
sentence, NO "racing creatures don't block" promotion (it would misplay every control deck). deck109
also VINDICATED the wave-5 per-seam-positive-floor prediction (4 high-life blocks → 0 after the floor
shipped) — the same leak, fixed the right way when the guide carries a seam-labeled floor.

**Core status this wave: NO prompt edits.** The only 2-seat convergence (reflexive blocking) routes to
representation, not the prompt. The wave-5 threat-before-value Edit 2 is CONFIRMED working on three
seats (deck135 8/10 conflicts to the creature; deck133; deck131 — every value cast was a no-threat
window). The core `hand -> stack` cast-happened anchor's removal condition is now MET cross-deck (0 cast
no-ops in ALL 7 seats; the Hellrider defect fixed at source by 353f1a150) — but it is KEPT for wave-7 as
generic anti-repick insurance and put on the removal SHORTLIST for a deliberate A/B (removal is an A/B,
not a wave edit; deck109 the owning seat agrees). See `general-strategy.txt` (unchanged copy) and
`core-prompt-revision.md`.

**Historical (wave 5) synthesis, condensed for continuity:** wave 5 established representation-beats-
instruction (double-confirmed), DISPLACEMENT as the sibling of overshoot, OBEYED-BUT-LOSING as a third
Step-0 mode, the attacker denominator = engine `A#.` list, PLAN/CHOICE MISMATCH as a sweep class, the
log-tail-fact-is-hidden rung, the re-offer proven-ceiling, count-brakes keyed to the bounding resource,
prefer-engine-surfaced-numbers-and-verify-offset, two-uses-two-triggers, role-check-names-its-seam, and
the threat-before-value core promotion. All held in wave 6 and remain in the method below.

## Superseded wave-5 synthesis notes (retained only as lineage)

Unified from seven independent deck-agent revisions (decks 44, 109, 110, 131, 133, 135, 140) of the
wave-4 skill, all mining the wave-5 corpus (one round-robin run, 6 games/deck). Wave 5 is the wave
where the platform caught up to the diagnosis: the head-first reply parser is CLEAN across all seven
seats (0 desyncs), the X-menu was reversed to largest-first, the plan-echo splice is gone, the
negative-life state-based loss fires, and — landed AFTER this corpus — the STACK is now a first-class
CURRENT SITUATION section ("ON THE STACK, waiting to resolve — top resolves first; you can respond
now") with controller/name/[spell|ability]/targets, and counter-type options are labeled with what
they can hit ("Cast Counterspell {u}{u} - can target on the stack: Master of Etherium"). With the
transport and several representation defects fixed, the wave's signal is sharper and its two lessons
are structural.

**The dominant theme: representation beats instruction, now double-confirmed — stop escalating wording
at a read-then-disobey wall.** deck110's `seq84` is the clean proof: the pilot READ the two-power-number
done-signal, VERBALIZED "already attached... waste of mana," and STILL took the re-offered equip.
deck44 is the same shape at the stack: the pending spell lived only in the log tail, and the pilot
fabricated a confident false resolution over it ("the spell moved to the battlefield"). deck133's
plan/list disconnect is a third. When the corpus shows the pilot correctly deriving a fact from the
board/log and then acting against it, instruction has failed at full strength — record it as a
representation defect and route it, do not write a louder rule. The stack-surfacing that shipped this
wave is exactly the deck44 fix, vindicating the pattern; the attachment marker (task #26) is the
pending deck110 A/B.

**The second theme: a fix that is obeyed, correct, and non-overshooting can STILL fail, because the
leak moved.** Wave 4 taught OVERSHOOT (a loud fix over-applied). Wave 5 adds DISPLACEMENT (deck135):
the attack floor was obeyed 13/13 and caused zero suicide attacks, yet the deck still lost — the leak
moved UPSTREAM to the cast seam (durdling on value before deploying a body). Re-audit the WHOLE chain,
not just the fixed rule and its inverse.

**Merged into the method this wave (multi-seat or completes an existing line):**
- *DISPLACEMENT as the sibling of overshoot* — a fix can land cleanly and expose the adjacent/upstream
  seam; tally the fixed seam's success AND whether it was even fed good material before concluding
  overshoot (135).
- *OBEYED-BUT-LOSING as a third Step-0 mode* — record down, obedience high, losses attributable to
  deck construction/variance: treat like turnaround mode (freeze obeyed lines, spend the wave on the
  one model-fixable lever), not like a failure to fix (131).
- *A fixed-defect stopgap written against a SURFACE can INVERT, not just go inert* — an engine fix that
  reverses/relabels a menu/label/board surface makes a positionally-phrased stopgap point at the WORST
  option; a guide line the corpus shows the pilot SUCCEEDING WHILE DISOBEYING is a flip/delete signal;
  prefer value-anchored phrasing over positional (140).
- *PLAN/CHOICE MISMATCH (intent collapse) as a distinct Step-2 leak class* — correct plan names a
  listed action, choice is a different passive option, head-int == choice (not a desync, not an
  unaffordable-plan reconcile); weakly guide-mitigable, durable fix is representation/labeling (44).
- *A fact living only in the LOG TAIL is functionally hidden* — the pilot skims it and fabricates a
  confident false reading; the fix is a first-class CURRENT SITUATION line, not a third copy of the
  tell (44); the shipped ON THE STACK section is the template.
- *Attacker-participation denominator = the engine's OFFERED `A#.` list, never the battlefield* —
  summoning-sick/tapped/just-made creatures legitimately shrink the legal set; the battlefield
  denominator manufactures a phantom subset-attack leak (131).
- *Prefer an engine-SURFACED number over a hand-computed one — and verify its OFFSET* — the engine now
  embeds computed values in option text (`{right now: drains N}`); teach the pilot to READ it, but
  audit the display against the RESOLVED result (Gray Merchant under-counts its own devotion by 2)
  (133).
- *A card with TWO distinct uses needs TWO distinct keyed triggers* — the executor fires only the use
  you wrote and misses the adjacent role (Gray Merchant as finisher AND as a lifegain stabilizer)
  (133).
- *A ROLE-CHECK must name the SEAM it fires at* — a defender role that only changes blocking does
  nothing for a deck whose stabilization happens at the CAST seam (135).
- *THREAT-before-VALUE / highest-impact act-floor* — three seats show a low-impact value spell casting
  to "satisfy the act-floor" while win conditions sit in hand; PROMOTED to the core prompt as a
  completion of the existing deploy/impact lines (135, 133, 131).

**Adopted from single seats (additive method, not core):**
- *Utility/mana-dork carve-out INSIDE the attack floor* — the free case taps a mana creature for a
  1-point chip and strips a blocker; name it as an exception with where it goes instead (135).
- *The re-offer three-part fix has a proven CEILING* — say so; once the cue is provably read, stop
  escalating and route to representation (110).
- *Count-brakes key on the BOUNDING RESOURCE, not the turn* — "one Equip per turn" mis-fires with two
  Platings; key it to unattached copies / unused charges (110).
- *Menu-adjacency index carryover* as a third numeric-offset shape — usually immaterial, route to the
  engine ledger, don't write a guide line (140).
- *A bare DON'T aimed at a decision SEAM is the weakest possible placement* — negative AND, if
  terminal, out of position; a seam always needs its own positive floor at a seam-labeled block (109).
- *Digest gotchas* — isolate the `Your battlefield:` line for on-board checks (splitting on `--- CURRENT
  SITUATION ---` also matches hand + carried PLAN); extract the `A#.` list for attacker records (131).

**The counter-tell stopgap is RETIRED core-side.** The wave-4 core carried a stopgap that derived the
pending spell from the log tail and asserted "there is no spell on the stack is always false while the
option is listed." The ON THE STACK section + labeled counter options shipped this wave; the stopgap's
removal condition is MET. The core prompt is rewritten to point at ON THE STACK and to carry the NEW
general insight from the wave-4-E3 investigation (a response window opens only when a response is
PAYABLE — a seat that tapped out on its own turn gets no window, so "died with counters in hand" is a
MANA-DISCIPLINE leak, not a stack-invisibility one; counter-deck guides now teach holding counter mana
open). Wave-6 authors: DELETE every guide copy of the counter-tell; teach reading ON THE STACK instead.
See the general-strategy rationale and `core-prompt-revision.md`.

**Historical (wave 4) synthesis, condensed for continuity:** wave 4 added the FOURTH attribution
bucket (HARNESS DEFECT) and the REPRESENTATION rung; countable completion conditions on PURSUITS as
well as brakes; per-item combat floors; demote-with-quantified-obedience; timeout triage; residual →
deck construction; the negative-framing ruling (positive default + two sanctioned constructs);
winning-template transcription; the tell audit; turnaround-preservation freeze mode; the stopgap
lifecycle triple; two-gear identity. All held up in wave 5 and remain in the method below.

## Superseded wave-4 synthesis notes (retained only as lineage)

Unified from seven independent deck-agent revisions (decks 44, 109, 110, 131, 133, 135, 140)
of the wave-3 skill, all mining the same wave-4 corpus (12 games/deck across two runs). Wave 4
is the wave where the wave-3 fixes LANDED and the failures moved: cast-nothing paralysis fell
sharply (deck110 turned around 0/6 → best-in-pool; deck109 8/12; deck133 affordability 75%→36%
cast-nothing), and the residue is a different, more structural set of problems — harness/engine
defects the reviewer must not blame on the pilot, combat-seam passivity the cast-floor never
reached, menu-shaped (index-vs-value) interface traps, and pursuit rules that over-ran their
purpose the same way wave-3's brakes did. What changed and why:

**Merged (proposed independently by 3+ agents — the convergent core):**
- *A FOURTH attribution bucket — HARNESS DEFECT* (the pilot chose right and the system did
  something else: reply-parser desync, engine no-op, misrender) added to the Step 2 taxonomy,
  because the whole corpus is now clean enough that engine bugs are a leading loss source
  (109, 140, 44). Only *bad play* is the guide's to fix; harness defects go on the bug ladder.
- *Per-record chosen-vs-happened / plan-vs-consumed diagnostics* using the new `chosen_text`
  field: desync detection, engine-no-op detection, and the systematic numeric-offset trap
  (109, 140, 44). These are how you SEE the fourth bucket.
- *A REPRESENTATION rung on the attribution ladder* — before any wording diagnosis, check
  whether the deciding fact was even VISIBLE to the executor (44, and implied by 109/140). A
  misplay whose deciding fact is invisible is a platform defect wearing a guide costume.
- *Countable completion conditions on PURSUIT and PREFERENCE rules, not just brakes* — wave-3
  made brakes carry releases; wave 4 proves a loud PURSUIT ("GET AN ISLAND", "Mountain first")
  over-runs identically and becomes the next durdle (135, 131). Every seek/prefer rule ships
  with a done-state.
- *Format/interface contracts are GENERAL-layer furniture* — the reply-format contract, the
  list-anchor, and the interface "offered = payable + it's your main phase" contract are
  harness properties identical for every deck; guides REFERENCE, never restate (109, 133, 135).
- *Teach the UI surface AS THE MODEL SEES IT for menu-shaped decisions* — for X-values, modal
  picks, damage order, transcribe the literal menu, state the index-vs-value mapping, give a
  pick procedure over the LIST, add a verify step (140, and the interface-contract-anchor form
  in 135, 133). A how-it-operates line about the CARD does not fix a menu trap.
- *Runnable per-item combat checks over posture prose* — the act-floor is PER-SEAM; "attack
  every turn" does not reach the blocker/attacker seams. Each passive seam gets its own floor
  written as a one-yes/no-test-per-listed-item procedure with a closed empty-choice exception
  (135, 131, 133).
- *Demote-and-compress with a QUANTIFIED obedience threshold, keeping the load-bearing tokens
  verbatim* — a won rule is compressed (not deleted, not kept at full prominence) once
  obedience is counted and total-or-near AND the fix lives at a stronger layer; what survives
  compression is the corrective FACT and the named forbidden SENTENCE verbatim, deleting the
  war-story, the self-citation stats, and anything the general layer now carries (109, 110,
  133, 140).
- *Timeout attribution — check WHO WAS AHEAD and split stalls from latency* — a timeout is no
  longer automatically passivity: winning-but-slow and latency-starved timeouts are not guide
  bugs (133, 140, 131). Normalize timeout counts by decision density before comparing decks.
- *Residual → DECK CONSTRUCTION* — when the same guide produces faithful-execution wins AND
  losses that share a precondition the guide cannot create (no threat drawn, color starved),
  the residual is the decklist; flag it upstream, don't write louder exhortations (131, 133).

**Adopted from single/paired agents (divergent but genuinely additive method):**
- *Winning-template extraction* — when the corpus contains a game the pilot handled correctly,
  TRANSCRIBE that sequence as a numbered procedure and tell the pilot it's its own proven line;
  transcription beats invention (133).
- *The TELL AUDIT* — before teaching any "when you see X, Y is true" inference rule, script a
  check that Y held in EVERY window X appeared; one counterexample means a false-positive the
  executor obeys off a cliff (44).
- *Verify the MECHANISM against `auto=`, including your OWN causal claims* — not just cited
  numbers: who a trigger hits, what a restriction scopes, which zone it fires in; the author's
  intuition is as unreliable as the pilot's (135).
- *Turnaround-preservation freeze mode* — when last wave's revision demonstrably worked, revise
  as a constrained diff off a copy of the deployed guide, freeze the obeyed lines, and produce
  a freeze-check (110).
- *Engine-defect BEHAVIORAL FALLBACK, never explanation* — the guide carries a positively-framed
  look-at-this fallback keyed to observable state, never "the engine is buggy" (109).
- *Stopgap lifecycle* — interface-defect defusal / tell lines are a tracked class: each listed
  in the handoff as (guide line, defect + citation, removal condition) so the platform fix
  sweeps them from every guide at once (110, 44).
- *Two-gear identity keyed to a VISIBLE board fact* for reactive shells around a narrow win
  package (131, 133). *Per-turn ROLE CHECK on board-readable facts, never opponent archetype*
  (133). *Life-as-resource needs a BUDGET keyed to the role check* (133).
- *Partial answers to bundled asks* — a new leak class now that bundled attacker/blocker
  decisions exist: the pilot answers with a subset (declares one attacker of three) (131).
  *Schema update + option-LINE counting* — the offered-vs-taken tally must count numbered
  option lines, not substrings, or it silently corrupts in both directions (135).

**The negative-framing adjudication (RULED — see Step 5, "Positive default and its two
sanctioned exceptions"):** the standing lesson "negative framing bleeds" is A/B-proven again
this wave (deck131: draw casts 0.17/game → 1.0/game after a negatively-framed clause was
rewritten positively, held across 12 games; deck44's "do NOT save them" suppressed its
neighbor). Four reviewers (44, 131, 133, 140) converted DO-NOT sections to positive form;
deck110 argued a reasoned defense of two negative constructs, deck135 flagged the tension.
Ruling: POSITIVE is the default for all operative and terminal-recap content; exactly two
negative constructs survive, tightly constrained (the quote-and-forbid FALSITY-FACT and, only
where no positive form is as sharp, a single bare NEVER on the worst leak). The former
terminal "DO NOT" section becomes a positive situation→play recap. This resolves the dispute
once so every guide is consistent.

**Task-consistency note (updated after the wave-4 general-strategy revision):** the general
system prompt has now absorbed both the wave-3 stopgaps (affordability/auto-tap contract, rules
facts, closed-exception act-floor, hold-release valves, stale-plan grounding, anti-deferral,
mulligan default, guide-beats-priors precedence) AND the wave-4 additions: the runnable combat
block (per-creature attack tests, punisher recompute, survival-first blocking at 8), the
first-strike fact, the race re-check, the interface-contract completions (timing anchor,
bundled-declaration contract), plan-FROM-the-list + numeric plan-binding, the activation
count-brake + ping arithmetic, and two core-layer STOPGAPS (counterspell offer-is-the-signal
tell; cast-happened anchor). The reply-format contract was routed to the code-appended
reply-protocol layer, NOT the prompt. Inputs #2 carries the full updated inventory; the
restatement ban covers all of it. Where a deck still shows a general line LOSING locally, the
guide reinforces it tied to the named card — the wave-3 rule, unchanged. The Output/handoff
section now also carries the four-layer routing discipline, the single-seat-can't-cut-core
epistemics, and the threshold-reuse rule this wave's core merge proved out.

---

A reusable method for producing one `{STRATEGY_GUIDE}` — the plain-text guide that
teaches the local qwen model to pilot ONE Wagic deck well. Follow it end to end for
any `deckN` in this project. This is craft method, not a guide for a specific deck.

## The one idea that governs everything: TWO models, and the executor is weak

- **You (the author)** are a strong model. You read the code, the card rules, and the
  game logs, and reason deeply about Magic. Spend that capability on ANALYSIS.
- **The executor is qwen** — a weaker model, under time pressure, that at runtime sees
  ONLY: the general system prompt + your guide + the live board + a numbered list of
  legal actions, and replies with ONE number. It does not get your reasoning. It cannot
  plan several moves ahead. It reasons LOCALLY — one decision at a time — and it
  pattern-matches your words against the board in front of it.

So: **do all the thinking, then hand qwen the CONCLUSIONS as orders.** A subtle,
"correct," hedged guide that qwen can't act on is worthless no matter how right it is.

Four failure tendencies to design against:
- **Locally-reasonable, globally-blind.** Each pick looks fine in isolation while the
  overall plan quietly never happens (the threat never gets deployed, the payoff never
  gets activated). Your job is to name the plan-critical action so plainly qwen takes it
  every time.
- **Shallow-heuristic defaults.** When the guide leaves a decision open, qwen falls back
  to bad defaults: pick by list position, by "biggest creature," by "a card is a card,"
  or by the safe/passive option when unsure. Every "consider / think about / weigh / it
  depends" is a sentence that hands the decision back to those defaults. Convert judgment
  into directives: **"DO X when Y," never "consider X."**
- **False world-model beliefs.** qwen holds stable, recurring misconceptions about the game
  and its interface, and they silently veto whole classes of correct actions: "Mana in your
  pool: (none)" read as "I cannot cast anything" while untapped lands sit on its printed
  battlefield; "attacking taps my lands"; hybrid {U/B} read as needing a Swamp; X-spells
  read as dealing 0; "it is the opponent's turn" while a sorcery-speed cast sits in the list.
  A strategy rule built on an uncorrected misconception never fires — the guide must state
  the corrective FACT flatly, and the strongest form ties it to the visible list (see Step 5).
- **Brake AND pursuit over-generalization / obedient absolutism.** A weak executor does not
  scope a caution; it universalizes it — "leave one land untapped to equip" becomes "never
  spend mana"; "save your removal" becomes "never cast anything." It also universalizes a
  PURSUIT: "GET AN ISLAND BY TURN 3" becomes "spend every turn fetching Islands while
  creatures sit home" (deck135: up to 16 of 59 decisions in a game hunting a land the deck
  owns two of). And it OBEYS your hard rules into losses: an absolute with an unstated
  exception is a trap you set yourself (deck109's pilot declined to burn a Young Pyromancer,
  quoting the guide's "burn goes face" rule, while Pyromancer tokens killed it). Every DON'T
  AND every "get/prefer X" you write will be applied far beyond the situation you meant.
  Design accordingly (see Step 5).

Write imperative, front-loaded, low-branching, concrete, evidence-driven. If a sentence
doesn't change which number qwen picks, cut it.

## Step 0 — Revising an EXISTING guide (the wave loop)

When a deployed guide was in play during the corpus you are mining, do this before
anything else. The logs no longer show qwen's raw defaults — they show the qwen×guide
interaction, and the guide itself is a suspect.

- **Attribute each misplay to a specific line — but climb the ladder in order.** Missing,
  wrong, or too weak to override a general-prompt prior — AND, before settling on any WORDING
  diagnosis, check the REPRESENTATION rung: could the executor even SEE the fact the correct
  play depends on? Pull the full `prompt` for 2-3 instances of the misplay and verify the
  deciding fact appears somewhere the executor attends to (CURRENT SITUATION or the option
  line — the log tail barely counts). A misplay whose deciding fact is invisible is a platform
  defect wearing a guide costume: rewording the guide will not fix it, and the wave will
  re-diagnose it forever (deck44 "died with counters in hand" was diagnosed as hoard-y wording
  in wave 3; wave 4 showed the STACK was never surfaced — one prompt-read a wave earlier would
  have caught it; wave 5 shipped an ON THE STACK section, the representation fix, and that residual
  is now a MANA-DISCIPLINE leak instead — see the counter-tell retirement in Inputs #2). **A fact
  that lives ONLY in the game-log TAIL is a special case of invisible: it is present but functionally
  hidden — a weak executor skims the tail and will FABRICATE a confident false reading over it
  (assert a resolution that never happened, e.g. "the spell moved to the battlefield"), distinct
  from expressing uncertainty. When the reply asserts a specific false event the tail contradicts,
  the fix is REPRESENTATION — a first-class CURRENT SITUATION line and/or a labeled option — NOT a
  third copy of the tell (deck44: the counter tell was present in BOTH core and guide and the pilot
  still fabricated over the tail; the shipped ON THE STACK line is the template for this class).**
- **Grep the replies for ECHOES of the guide's own phrasing.** When qwen QUOTES a guide
  sentence in its PLAN while misplaying, the guide caused the play; that is a guide bug, not a
  model bug (it also proves your exact wording propagates — phrasing has leverage in both
  directions). A rule the executor cites while misplaying is a rule to rewrite or delete.
- **Audit last wave's rules against the new logs: did each one get FOLLOWED? — and DEMOTE the
  winners precisely.** A rule the new corpus shows OBEYED has done its job; demote it, but this
  is a three-part move, not deletion-lite:
  1. **Quantify obedience** (violations / opportunities: 0 delusion instances in 174 casting
     asks; 67/67 combats attacked; cast-nothing 75%→36%). Demote only when obedience is
     total-or-near AND the fix now lives at a stronger layer (a representation line like
     `Mana available: N untapped sources`, or the general prompt) so the guide is no longer the
     only defense. Keep FULL prominence while either is unmet.
  2. **Keep the load-bearing TOKENS verbatim** — the corrective FACT (what to look at) and the
     NAMED FORBIDDEN SENTENCE ("the thought 'I have no mana' is always false when the cast is
     listed"). Those exact words are what the executor pattern-matched into compliance; they
     are the active ingredient. Cut the explanation, the war-story, and the self-citation stats
     ("last run you picked Cast nothing 76 of 101 times" — that describes a pilot that no longer
     exists).
  3. **Delete — not compress — everything the general prompt has since absorbed** (check the
     CURRENT general prompt line by line against the rule first), and give the vacated slot to
     the new biggest leak. Record in your handoff WHICH sentences you kept and why, so the next
     reviser doesn't erode the kept core to nothing over successive waves.
- **Audit last wave's #1 RULE specifically for OVERSHOOT.** The louder a fix, the more the
  executor over-applies it; a cured leak often returns INVERTED. Count decisions spent on the
  fixed behavior per game vs. decisions spent on the win condition — a pursuit or brake now
  running past its purpose is this wave's leak (deck135's Island fetch; deck110's re-equip loop).
- **Overshoot is only ONE of the two ways a fix moves the leak — also check for DISPLACEMENT.** A
  rule can be OBEYED, correct, and non-overshooting and the deck STILL loses, because the leak moved
  to the ADJACENT seam — usually the one UPSTREAM that FEEDS the fixed seam. Before concluding a loud
  rule overshot, tally the fixed behavior's success AND count how often the seam even had good
  material to work with; if the seam is healthy but STARVED, the fix landed and the new leak is
  upstream, not an overshoot (deck135 wave 4 predicted the attack floor would overshoot into suicide
  attacks; wave 5 it was obeyed 13/13 with zero suicide losses, but only 17 windows in 6 games ever
  offered a real creature — the loss lived at the CAST seam that never deployed a body). A review that
  only hunts overshoot-of-the-loud-rule re-polishes a working rule and misses the real lever.
- **A rule that never FIRED in the new corpus** (its misplay no longer occurs) is a candidate
  to DROP: stale cautions aren't just wasted attention, they can reinforce the wrong posture
  (deck131's "don't counter into an empty stack" was backing the hoarding).
- **Expect the failure to have MOVED downstream, and re-audit the fixed chain END-TO-END.**
  Fixing the blocking action exposes the next bottleneck in the same chain. Concretely: follow
  the SAME card/action through the logs all the way to its effect RESOLVING on the board — cast
  happened is not effect happened. Verify each stage: offered → taken → parameters set correctly
  (X, mode, targets) → resolved with the intended magnitude (deck140's render fix made Rakdos's
  Return get CAST 0/26→~17, but the very next stage — X announcement — blanked it to X=0).
- **Suspect your own defaults.** Any default the old guide prescribed is a candidate CAUSE of
  the new failure, because qwen follows it more literally than you meant. Re-derive the deck's
  dependencies from scratch each wave.
- **TURNAROUND-PRESERVATION mode — when the previous revision demonstrably WORKED** (the deck's
  record or its key metric turned around), revise as a constrained diff, not a rewrite. The
  stakes are asymmetric: deck110 is the pool's proof that a "faithful" full rewrite took the
  strongest deck to 0/6. (1) Before drafting, list the guide lines the logs show being OBEYED —
  echoed in winning PLANs, or whose target misplay vanished — and FREEZE them: they may move
  position, their wording does not change. (2) Draft by editing a COPY of the deployed guide,
  never from a blank page. (3) After drafting, produce a FREEZE-CHECK: for every frozen line,
  show where it survived (verbatim / moved / demoted-per-above) — a frozen line you cannot point
  to is a regression you just wrote. (4) Aim NEW text at the new leak only; if a section reads
  awkward but the logs show it landing, it stays awkward.
- **OBEYED-BUT-LOSING mode — when the RECORD dropped but the obedience metrics are HIGH** and the
  losses attribute to deck construction / variance (opponent life at death near 20 across most losses;
  the wins/near-wins are the guide working as written), treat it like turnaround mode, NOT like a
  failure to fix: VERIFY obedience first (attack participation, on-curve casts, brake/pursuit
  adherence), then freeze the obeyed lines and spend the wave only on the one residual model-fixable
  lever (deck131 wave 5: the underused non-combat engine), routing the record itself to a
  deck-construction flag. A falling record on n≤6 against a harder opponent slate is variance, not
  evidence the guide broke — and a loud rewrite churns the obeyed lines and risks deck110's
  faithful-rewrite→0/6 outcome in reverse. Distinguish this from turnaround mode (which keys on
  IMPROVEMENT); both forbid the reflexive rewrite a bad short record tempts.
- **Verify every prior-wave representation/engine fix actually FIRED before crediting or re-diagnosing
  it.** When a past wave routed a leak to representation (a marker, a surfaced number, a relabeled
  option) and that fix has since "shipped," your FIRST step is to grep the new corpus for the marker
  STRING and confirm it appears on the decisions it targets. A shipped-but-INERT fix looks identical to
  a shipped-and-obeyed-but-weak fix in the win column — but the remedy is opposite (fix the code vs.
  abandon the approach). This is the composition-seam rule across waves: the fix lives in a different
  file than the diagnosis, and only end-to-end observation catches the mismatch (deck110 wave 6: the
  re-equip "(ALREADY attached)" marker checked `auraParent` while equipment stores its host in
  `source->target`, so the string appeared 0 times and the 50% no-op rate was unchanged — a wave spent
  re-diagnosing a fix that never ran would have been wasted). **When the grep confirms the fix DID fire
  AND is obeyed** (quantified: deck110 wave 7, the field fix landed and the marker went 29 offered /
  28 obeyed, no-op 50%→6%), that is not the end — it is the DEMOTION trigger. The interim GUIDE defense
  that was covering for the missing representation is now DOUBLE COVERAGE (pure attention tax) and must
  be demoted per the three-part demotion rule: the guide keeps only the corrective FACT + the marker
  STRING + the RELEASE + the mana DESTINATION + ONE demoted board-cue backup line (for the case the
  representation is ever absent), and drops the full perception procedure the representation now carries
  (deck110 wave 7 cut a 19-line two-power PERCEPTION PROCEDURE + count-brake to 10 lines). **And read
  the REPLY of the lone counterexample before re-opening the representation:** a single "took the
  marked option" that, on reading its reply, shows the pilot READ and understood the marker but wanted
  a legit DIFFERENT action and TYPED THE WRONG INDEX is a PLAN/CHOICE MISMATCH (an existing Step-2
  class), NOT a weak marker — do not score it against the fix or re-open the representation over it
  (deck110 wave 7 vs131 s33 was the corpus's only "taken," and it was intent-collapse, not the marker
  ignored). **The deletion end of the lifecycle has its own audit too:** after you DELETE a stopgap
  (because its engine fix shipped), grep the NEXT corpus for the STRING of the misreasoning it used to
  produce and confirm it stays at zero — the mirror of this fired-check (deck109 wave 7: the deleted
  Hellrider fallback left 0 recurrences of the phantom "missed cast" framing across 6 games).
  **The terminus GENERALIZES across every representation-fix SHAPE, and the verification SIGNAL is
  shape-specific — do not assume the marker-string grep is the only check** (wave 8 proved the same
  terminus for three shapes at four seats):
  - **WEAK-MARKER fix** (a representation that appears but may be ignored): verify by GREPPING the
    marker string and confirming it is OBEYED, then demote (deck110 re-equip).
  - **ABSENT-OPTION fix** (a guide rule named an option the engine never offered): verify by the
    OPTION APPEARING in the list at the decisions it targets (count decisions whose option list now
    contains it), not by a board/reply read; the appearance IS the demotion trigger (deck131 wave 8:
    land-drop enumeration, 12 co-offers vs 0 last wave, Mountain chosen 12/12 by name — dropped the
    hold-to-reach-your-color stopgap AND the stale one-land-at-a-time explanation).
  - **RE-ANCHOR-TO-A-COUNT fix** (the guide re-anchored a hold/brake to a countable board fact that
    then SHIPS as printed representation): a re-anchor-to-a-count and a surfaced-count representation
    are THE SAME FIX AT TWO LAYERS. Verify by the defect class going to ZERO — but only AFTER joining
    castability + intra-turn context (the raw held-with-condition-present tally over-counts; join it
    or a mana-lock and a same-turn-sweep read as persistent overshoot: deck140 wave 8, 7 raw
    candidates, all 7 non-overshoot on inspection). Once the count is BOTH the guide's trigger AND a
    printed marker, the inline false-belief kills the count subsumes are demotable.
  **Demote a belief-kill block PER-BELIEF against what the representation LITERALLY surfaces — the
  demotion is NOT uniform.** A count `(creatures: N)` carries "how MANY" (dissolves "they're tapped ->
  no creatures" and "artifact board -> hold"; both demote); a tag carries "state"; NEITHER carries
  "how your spell INTERACTS," so a TARGETING misconception (shroud stops a non-targeted sweep) survives
  the demotion a count triggers and stays as a guide-only fact (deck140 wave 8: tapped + artifact
  bridges DEMOTED to one-line marker-anchored nods, shroud kill KEPT compressed). Before demoting each
  belief, check it against the representation's literal surface. **A validated re-anchor puts the seat
  in TURNAROUND mode** (its key metric turned around on the first corpus): demote the double-covered
  lines, freeze the obeyed ones, and hold any NEW permissive/fire condition to a WATCH — a new high-life
  fire condition is exactly the widening that produced the original overshoot (deck140 wave 8: the
  snowball-single-threat edge is a real miss but legislating a high-life fire re-opens the pendulum).
  **And a fix confirmed at the DELETION end propagates ACROSS guides:** when an engine display/value fix
  ships and you delete its per-deck workaround, SWEEP every OTHER guide for the same workaround and
  delete it in the same pass (deck133 wave 8: the Gray Merchant `{drains N}` display fix deleted the
  "+2" workaround; the propagation sweep confirmed no other guide carried it).
  - **OPTION-LINE ANNOTATION fix** (a fourth shape — the engine writes the deciding fact onto the
    OPTION the executor is about to pick, when a fact surfaced elsewhere is still reasoned-around or
    FABRICATED over): verify by TWO shape-specific signals — (i) the mis-play count -> 0 (did the pilot
    ever take the option against its own side / into the empty target?), and (ii) a REASONING-TAX DROP
    (SHORTER, cleaner plans at the annotated windows, measured against the un-annotated windows of the
    same decision class). The reasoning-tax drop is a representation-fired signal in its own right: an
    annotation can fire, be obeyed, AND still be worth measuring by whether it shortened the executor's
    derivation, because a shorter derivation is fewer places for a weak model to talk itself into the
    wrong branch (deck44 wave 9: the own-targets annotation "the only legal targets are YOUR OWN right
    now" fired 47×, 0/47 self-target casts, median plan 519 vs 871 chars, and the confused
    self-negotiation appeared ONLY where the annotation was absent). This is the LAST rung before the
    number is emitted: when a fact is BOTH surfaced in CURRENT SITUATION AND stated in the guide and the
    pilot STILL fabricates over it (deck133 wave 9: `Opponent hand size: 0` surfaced + forbidden in TWO
    guide spots, the pilot cast Thoughtseize inventing "the opponent may have a removal spell"), do not
    write a third copy of the rule — annotate the OPTION itself (`Cast Thoughtseize {b} - opponent's
    hand is EMPTY; this only costs you 2 life`). **Demote the replaced stopgap only when the
    annotation's surface covers EVERY case** (conditional-surface granularity, per the per-belief rule):
    the own-targets annotation cannot fire on the ZERO-legal-target case (pilot holds no creature AND
    opponent holds none), where the option renders bare and the executor still gets confused — so the
    guide fallback stays PARTIALLY double-covered; KEEP it and finish the demotion only once the
    zero-target case is also represented (a `[no legal target]` marker).
    **Wave-10 refinement — score the option-line annotation (and any confirmed-fired representation) in
    the REASONING, not the recorded CHOICE, and read for OUTCOME-neutrality, not silence of the false
    belief.** Two answer-after-plan lessons sharpen the "mis-cast -> 0" signal: (a) *obeyed-in-reasoning,
    leaked-at-number* (deck133 + deck44, 2 seats) — an annotation can be read and obeyed in the PLAN prose
    at EVERY window ("Fatal Push has no valid targets... I will not cast it," 29/29) while the recorded
    choice casts the annotated-bad option in a few, because the leak lives DOWNSTREAM: an unoffered-desired-
    card fallback digit maps onto the annotated option, a formatless reply is int-grabbed, or (specific to
    answer-LAST) a reply blows the token cap before any `CHOICE:` line and the parser SCAVENGES a body digit
    that contradicts a derivation that explicitly obeyed the annotation. So measure obedience in the
    REASONING and report it SEPARATELY from the recorded choice; a mis-cast that treats the number as the
    obedience metric misattributes a harness/reply-protocol leak to the annotation and invites a wrongful
    louder-wording escalation. FIRST check, for every recorded mis-cast, whether the reply reached a
    `CHOICE:` line at all — a truncated/scavenged-digit mis-cast is the 2048-cap HARNESS class, not
    annotation-ignore, and must not inflate the count or re-open the representation. (b) *For a COUNT-
    AGREEMENT fix, success is surface-agrees + outcome-neutral, NOT the false belief leaving the reasoning*
    (deck110, 2+ waves) — a model will re-derive over an accurate surface (177/177 agree, yet the pilot
    still hand-folds Glimmervoid/basics into a metalcraft count); score it CLOSED when the surface is
    complete/agrees AND the defect is outcome-neutralized (no metalcraft-gated pick flipped, no Blast fired
    expecting metalcraft-4 at a sub-3 board), and do NOT re-open a non-outcome-affecting reasoning residual
    with louder wording. Also (deck109, refines the offered=payable ladder): before routing an
    offered=payable / can't-pay fabrication to representation or core, ask "is the SPECIFIC card-cost fact
    taught where the pilot reads it?" — the generic interface contract does not inoculate against a
    specific-mechanic misread (`{B/R}` read as `{B}{R}`); if a SIBLING card's parallel cost fact is obeyed
    (Boros Reckoner `{R/W}`), add the same card-specific fact for the misfiring card FIRST (attacks the root
    belief), and only THEN reach for the option-line annotation or a core sub-clause.
    **Wave-11 refinement — before crediting a confirmed-fired annotation/guardrail (or re-opening a
    representation), classify each apparent leak by MECHANISM and LAYER; and an annotation-SUPPRESSION fix has
    its own signals.** The leak can live at several distinct layers with different owners, and a raw
    self-cast/obey-rate count conflates them (deck133 + deck131 + deck44 + deck110, 4 seats): (a) reasoning-
    DISOBEY via a model rules-misconception — the model reads the annotation and reasons straight past it
    (deck133: `only legal targets are YOUR OWN` obeyed 39/41, but twice fabricated that Fatal Push can destroy
    an opponent creature SPELL still ON THE STACK) -> owner is representation/model (name WHY the excluded
    target is illegal — stack-vs-battlefield), never a louder guide line; (b) number/PARSE leak — the wave-10
    mechanism, CLOSED this wave (the ramble guard now rejects an out-of-range index instead of int-grabbing
    it); (c) a BLOCKER-PARSER layer — a guardrail obeyed PERFECTLY in reasoning (`BLOCKS: none`) can still be
    VIOLATED because bare `BLOCKS: none` is unparsed and the baka blocker fallback is not a no-op (deck131:
    blocked Guttersnipe into a fatal trade), so for a `kind==blockers` record `chosen_text` records the PARSE,
    not executed combat — score block-side obedience by the reasoning AND the next record's combat `events`,
    never `chosen_text` (bug SHIPPED-PENDING-CORPUS: explicit all-decline now declares no blockers); (d) a
    shipped truncation/parse GUARD is CLOSED when 0 scavenged casts appear AND every fallback carries a
    `fallback=` field with a heuristic action (deck44 + deck133). Never re-word the guide to satisfy a parser
    (emitting `B1:none` papers a parser bug in prose — the sanctioned-against escalation). SUPPRESSION shape
    (deck135, completes the reasoning-tax-drop rung): when a shipped fix REMOVES a surface contradiction (not
    surfaces a fact), score it by the contradiction string -> 0 AND a reasoning-tax COLLAPSE (a category
    change — ~470 -> ~100 words when the self-contradictory `NO legal target` clause was dropped from Force's
    counter option) AND the RECOVERY of any adjacent faculty the noise was corrupting (Force target discipline
    recovered: 0 soft-target mis-Forces). CONFIRMED-STILL-BROKEN mirror (deck44): when re-verifying a
    KNOWN-UNFIXED engine defect, report RECURRENCE and OUTCOME-COUPLING separately — a defect can be
    live-and-harmless one corpus (auto-tap color-stranding recurred outcome-neutral) and live-and-fatal the
    next purely by trajectory; a harmless recurrence is a cleaner, loss-decoupled regression fixture, so do not
    downgrade the ledger item nor re-inflate it into a guide line.
    **Wave-13 refinement — verify the RESOLVED effect against the game-log, and score a name-echo protocol by
    the Mechanism-A/B split; a "100%-compliance" statistic is necessary-but-insufficient.** Two extensions of
    the terminus: (a) *RESOLVED-EFFECT-MAGNITUDE* (deck110, rung-completing — the Galvanic Blast lesson):
    confirmed-fired grows one rung DEEPER — from "did the surface render" (representation) and "did the SPELL
    resolve or get countered" (the RESOLVED-VS-ASSERTED anchor in Step 2) to "did the MECHANIC resolve at the
    magnitude the model asserted." When a decision's value depends on an effect SIZE the model computes (burn
    damage, pump, life swing, sacrifice count, counters added), locate the resolution in the game log's
    `Damage:`/`sacrifice`/`dies` events and confirm the number; where the deck's guide states a magnitude
    ("4 with metalcraft"), spot-check the engine delivers it at least once — a magnitude that NEVER appears in
    the log across a whole corpus is a bug, not a coincidence (wave-11 credited every Galvanic Blast as "4 dmg,
    genuine metalcraft" off the model's PROSE; the `Damage:` events showed every Blast dealt 2 — the
    metalcraft `+2` `aslongas` line never fires, a HIGH wincon bug hidden ≥2 waves behind fluent-but-wrong
    reasoning). The model's prose is INTENT; only the game log is RESULT. Route the card-script fix to the
    ledger (a DATA fix — verify against real Oracle text first); the guide is Oracle-correct, do not edit it.
    (b) *NAME-ECHO EVALUATION SPLIT* (deck133 + deck140 + deck44, 3 seats): when scoring a shipped name-echo
    (or any echo-the-option-text) protocol, a "100% echo compliance" statistic and an "index-wins-on-conflict"
    rule are BOTH necessary-but-insufficient — split echo-index conflicts into Mechanism A (echo names an
    OFFERED option ≠ the index: the arm can remap to the unique match, index-wins is a defensible tiebreak on
    genuine ambiguity — the case the protocol was built for, and it behaves) and Mechanism B (echo names a card
    in NO option = hallucinated presence: there is no remap target, index-wins fires an unrelated in-list cast),
    and COUNT the Mechanism-B events and their executed-card outcomes. Compliance measures "carries a name," not
    "the name matched or the intent survived"; the harmful cases (a hallucinated-absent echo whose index lands
    on a guide-forbidden or held cast) are INVISIBLE to a compliance grep and surface only by reading the PLAN
    against the option list at every sub-menu / spent-land-drop seam. The routing side is in the Step-2
    reply-protocol taxonomy (absent echo = staleness → heuristic/Cast-nothing, never the raw index).
    **Wave-16 refinement (single-seat WATCH) — a shipped absent-echo/stale_echo route has (at least) THREE
    downgrade causes a reviewer must bucket separately, not two: (a) the LEAK direction (echo names NO option,
    executes a raw index — closed wave-13); (b) the SUPERSTRING false-positive (echo names an option only as a
    superstring — closed wave-15); and (c) the DEGENERATE-LABEL false-positive (the echo names a coherent, PRESENT
    intent but cannot match because the OPTION ITSELF serialized to a contentless stub).** Witness: deck133 vs62 s29
    — a Liliana, the Last Hope `+1` target sub-menu serialized as `['Becomes ', 'Decline - do nothing']`, so the
    correct echo `CHOICE: 1 (Activate Liliana's +1 on Canopy Spider)` could not match the bare `Becomes ` label →
    stale_echo → heuristic (which fired the +1 anyway; non-deciding). Cause (c) is invisible to a superstring check
    (there is no name to superstring-match) and is a HARNESS/SERIALIZATION defect UPSTREAM of the matcher — do NOT
    "fix" it by loosening the matcher (there is nothing to match) and do NOT credit it as a clean catch; the fix is
    to emit the full target-menu label (creature + resulting P/T). Single-seat, 1 non-deciding firing → WATCH; adopt
    into the scoring rung only if a second seat shows a degenerate-label downgrade (cross-seat by construction: any
    deck running a Liliana-the-Last-Hope +1 or similar stat-change target menu will hit it).
    **Wave-23 refinement — CLASSIFY THE SURFACE AND THE CRUTCH BEFORE APPLYING THE TERMINUS (3-seat convergent:
    deck133 + deck109 + deck102). The demote/retire criteria are NOT uniform; they fork three ways, and applying the
    wave-8 CLOSED-criteria to the wrong kind either leaves a redundant rule or retires a load-bearing one.** (a) *FACT
    surface vs JUDGMENT surface* (deck133). A new engine surface that shows a value/target/cost/count previously
    computed in the guide is a FACT surface → DEMOTE/RETIRE per the wave-8 terminus. A surface that renders a GENERIC
    HEURISTIC/NUDGE ("pick the least valuable," "block the biggest threat," a forced-loss "pick the card you can best
    afford to lose" header) is a JUDGMENT surface → it RAISES THE FLOOR generically, so do NOT add a guide rule
    duplicating it (the method-1a anti-pattern), BUT audit whether the deck has a SPECIFIC card/line where the generic
    judgment is wrong or under-informed and ADD that deck-specific EXCEPTION, which COEXISTS with the framing rather
    than being demoted by it (deck133 wave-23: the forced-self-loss framing raised the floor; the ADD is "Bloodghast is
    uniquely the safest card to give up — it recurs on your next land," a deck-fact the generic "least valuable" prior
    cannot see). The guide's remaining job shifts from "provide the judgment" to "provide the deck exception the generic
    judgment gets wrong or cannot see"; the rotation-in new-leak hunt (wave-8 headline-2) then hunts for the EXCEPTION,
    not the missing rule. (b) *information-supplying crutch vs belief-correcting crutch, with a 2-CORPORA bar for the
    latter* (deck109). The wave-8 CLOSED-criteria (failure→0 AND the durable annotation renders) safely RETIRE a crutch
    that supplied MISSING information the model had NO OTHER WAY to know (a hidden trade outcome, a hybrid pip's
    payability, an absent-signal note) — once the engine renders the fact, the model has the input it lacked. A crutch
    that CORRECTS A WRONG PRIOR BELIEF the model holds independently of the display (a cost it mis-recalls, a rules
    interaction it mis-applies) is different in kind: the render is NECESSARY but NOT SUFFICIENT, because the model can
    and stochastically DOES reason from memory and ignore the correctly-rendered fact ("belief overrides display" — a
    display fix cannot reach it). Such a crutch retires only when failure stays 0 across at least TWO corpora WITH THE
    BELIEF-TRIGGERING SHAPE PRESENT; on a recurrence, RESTORE it lean and sharpened to name the specific observed
    conflation (not the old verbose lecture). Worked example (deck109 wave-23): the Pillar-{R} cost retirement REGRESSED
    round-2 (vs137 s12 declined an affordable Pillar reasoning "Pillar requires {1}{R}," conflating Searing Spear's
    cost, ignoring `Cast Pillar of Flame {r}`) → restored as a lean line; the hybrid-mana crutch retired ALONGSIDE it
    (information-supplying) HELD cleanly — same corpus, same seat, opposite outcomes, isolating the variable. (c)
    *oracle-truth clauses vs engine-behavior-narration clauses when you demote* (deck102, a fork on the wave-22
    now-FALSE-clause audit). DEMOTE/trim the ENGINE-BEHAVIOR narration (the fix now carries it), but do NOT rewrite an
    ORACLE-truth clause to chase a SUSPECTED engine BUG from a single observation — route the bug to notes, keep the
    guide oracle-accurate-but-GENERIC, and correct the oracle clause only once the engine's actual behavior is confirmed
    durable. Worked example (deck102 wave-23): the Tergrid crutch mixed a mechanical "the flip toggle does nothing /
    cast a face" (engine-behavior → DEMOTE, DISPLAY-TOGGLE annotation carries it, misplay 11→1) with "the Lantern is a
    repeatable {T}: drain" (oracle description); R-DFC-BACKFACE-RESOLVES-FRONT surfaced (casting the Lantern back-face
    deploys the God front-face), so the oracle clause may not match what the engine executes → softened to "Lantern =
    cheaper face" pending the engine answer, NOT rewritten to assert the bug. Guides describe correct PLAY; engine bugs
    get fixed in the engine, not encoded into the guide from one game.
  - **A representation that renders for the OPPONENT of the reviewed deck is verified at the OPPONENT's
    seat, not the deck's own log.** When the fix's CONSUMER is the player DECIDING against the reviewed
    deck's card (a punisher/rider on an attacker that the DEFENDER reads to decline a block), grep the
    OPPONENT's seat file at the seam the reviewed deck's card creates — the fix and its consumer live in
    different files AND different seats, so a reviewer reading only the reviewed deck's log will report
    it as still unverified (deck133 wave 9: the Obliterator sac-punisher rider was confirmed rendering +
    respected only in OPP135 seq38 / OPP131 seq56, never in deck133's own log — deck133 had 0 blocker
    windows all corpus).
  **PAUSE-AWARE demotion precondition — demote a guardrail only once its class was OBSERVED OBEYED,
  never merely because it was ABSENT this corpus.** "No counterexample off ZERO opportunities" is not
  "the rule is proven": a guardrail whose precondition did NOT recur is UNTESTED, not vindicated, and
  the turnaround/demotion terminus does not apply to it — keep it at FULL prominence. This hardens into
  the pause: a CLOSING wave with no next wave to catch a regression FREEZES untested guardrails,
  especially any that guard a KNOWN game-loser (deck109 wave 9: the NEVER-block-Phyrexian-Obliterator
  absolute — guarding block-at-10 -> sac-board -> loss — got zero test windows because deck133 was
  raced out T10; a 6/6 seat would normally invite compressing it, but it was never exercised, so it
  stays). The thin-sample corollary: when a re-anchor validates on FEW windows, state validation
  PER-CLASS against the windows that actually occurred and keep any zero-window class at WATCH — do not
  fold "no recurrence off zero opportunities" into "the rule is proven" (deck109 wave 9: the re-anchored
  chump rule validated 3/3 for the high-life gang-block class but had zero Obliterator-block windows —
  validated for the tested class, WATCH for the untested one).
- **Audit for an INTRA-GUIDE CONTRADICTION when a #1 rule UNDER-fires.** A cured-then-relapsed or
  disobeyed #1 rule is not always too weak or ignored — a DIFFERENT rule lower in the SAME guide may
  forbid the exact action #1 commands. The executor resolves the contradiction toward the rule that
  NAMES THE OPTION IN FRONT OF IT (concreteness beats prominence — the intra-guide mirror of
  representation-beats-instruction). So when #1 is disobeyed, before concluding "make it louder," scan
  the rest of the guide for a rule whose scope EXCLUDES #1's action — ESPECIALLY any cast-floor /
  cast-nothing / "otherwise cast something" clause, whose closed exception list silently vetoes any hold
  or action it omits. The fix is to WIDEN the lower rule's exception to name #1's action, never to shout
  #1 (deck140 wave 6: "HELD FOR LETHAL" was disobeyed 6/7 because the cast-nothing rule permitted no
  "hold" output for a non-sweeper burst — the model took the cast because "hold" was not on the list;
  a two-option `1. Cast Rakdos / 2. Cast nothing` window with a PLAN saying "hold both" proves it).
  **A second shape of the same mechanism fires at a LIFE BRACKET / decision SEAM, not only at the top
  of a ranked list: a capability/target ABSOLUTE ("never block X", "never target Y") must never be
  authored as a SUB-CLAUSE of a MODE rule that will later command its opposite** (a chump / desperation
  / all-in clause). When a #1-band absolute is OBEYED at one life bracket and VIOLATED at another, the
  diagnosis is POSITION, not prominence — pull the absolute OUT to its own standalone line ABOVE the
  mode rule and re-state it as the mode rule's explicit EXCEPTION; concreteness-beats-prominence applies
  WITHIN a paragraph too (the option the mode rule NAMES beats the caveat the same paragraph buries).
  Litmus: for every "NEVER do Z" in the draft, check that no later rule's scope ("when low, do the
  desperate thing") re-authorizes Z; if one does, hoist and except (deck44 wave 7: "Do NOT block
  Phyrexian Obliterator" lived INSIDE the "at ≤8 life, chump the biggest attacker" paragraph; the pilot
  obeyed it at L20 and, once chump-mode activated at L11 with Obliterator the biggest attacker, the
  chump rule vetoed the buried prohibition — feeding the sac punisher and losing the game).
- **When this wave's ENGINE change REMOVED a guard, re-audit for the misplays it was masking.** A
  removed veto/restriction is an A/B that exposes the model's unmediated behavior: do not just verify
  the newly-allowed action succeeds (it does — that's the fix), also ask "what BAD version of this
  action did the guard silently block?" and grep the new PLANs for it — those misplays resurface as
  new guide work, because the guide must now carry what the engine used to (deck140 wave 6: 353f1a150
  lifted the "2+ creatures" combo-hold hint-veto on sweepers, and a latent false belief the guard had
  been absorbing — "board wipes hit artifacts" — surfaced immediately, ~7/22 sweeps wasted into
  artifact-only boards; the fix is a corrective FACT anchored to the sweeper `auto=`).
- **Precondition-count before calling a prior-wave leak FIXED.** When last wave's #1 lever shows ZERO
  instances of its target misplay in the new corpus, do NOT record "fix validated" on the zero alone:
  first count how many windows the fixed behavior's PRECONDITION even existed. A zero-count off few
  precondition windows is CONSISTENT with the fix working but is under-powered evidence, not proof (the
  rare-event-A/B-power trap applied to leak resolution). Add this to the "quantify obedience" step: for
  a rule whose target misplay VANISHED, also count the windows where it COULD have fired — a vanished
  misplay off <5 precondition windows stays on the watch list at FULL prominence and is not demoted
  (deck131 wave 6: "cast-nothing while Guttersnipe out" logged 0 declines, but Guttersnipe reached the
  board with a castable spell in only 3 windows all game — "0 declines" tested the override in n=3, so
  it stays live, not "resolved").
- **In OBEYED-BUT-LOSING mode, SIZE the residual lever's CEILING before giving it the loud #1 slot, and
  sharpen the variance-entry test.** (a) Before allocating the one residual model-fixable lever the
  guide's loudest position, estimate its ceiling against the deck-construction residual — if the
  construction cause DOMINATES the outcome even after a perfect fix, the lever earns a sharpened labeled
  seam block, NOT the #1 top slot (which churns a frozen, obeyed rule for little expected gain); the
  frozen winners keep their positions (deck131 wave 6: the mulligan-discipline lever was real but a
  no-red 7→6 off a 14-Island/8-Mountain base still misses red, so the manabase stays the #1 upstream
  flag and the mulligan sharpen goes in a promoted MULLIGAN block, not the top slot). (b) Entry test:
  if the WIN and the adjudicated-timeout-wins show the deck executing its plan, and the losses each LACK
  a precondition the guide cannot create (a threat curve, a color), the record drop is variance — freeze
  and route to deck construction, do not rewrite (deck110 wave 6: 5/6→1/6 raw / 3/6 adjudicated, every
  obedience metric healthy, all three losses draw-variance — the deck110-in-reverse of its own
  faithful-rewrite→0/6 warning).
- **META SHIFT — a fifth thing to separate from a guide bug.** A rule can be winning because the
  opponent COULDN'T punish it and start losing because a PLATFORM fix made the punishment work — the
  guide did not change, the world did. Record it as a deck-construction/meta flag, not a guide bug, and
  do not "fix" the guide against it; the correct response is deck-list or sequencing, not louder prose
  (deck110 wave 6: affinity's wave-4/5 dominance was partly an artifact of a blind counter seam; wave 6
  surfaced the stack, opponents' counters came alive, and affinity's discrete threats now get answered).
- **Keep what worked, verbatim where possible** — churn in lines qwen already obeys is pure risk.

## Step 0-bis — Writing a NEW guide from scratch (the GUIDELESS first pass)

New in wave 15, converged from the first two guideless newcomer seats (decks 17, 21). When a deck played the
corpus with NO guide (core prompt only), you are writing its INITIAL guide, and the output contract differs from
Step 0's revise loop. A guideless corpus is the single most information-rich input you will get: it shows the
qwen×core interaction with NO guide confound, so it reveals **exactly what the core already gets right** (which you
must NOT re-teach — re-teaching a core-handled fact is pure tax and dilutes the load-bearing lines) and **exactly
where deck knowledge is load-bearing** (which you teach precisely). Procedure:

1. **Read the deck deeply from the ENGINE, not from real-MTG memory.** Derive every card's cost / stats / keywords
   from the translog option lines and battlefield annotations, and the win path from how the games actually played.
   The engine's cards DIVERGE from real Magic (deck21: Scuzzback Scrapper is a `{R/G}` 1/1 with no haste here vs the
   real `{2}{R/G}` 3/1 haste-persist; Boartusk Liege is a 3/4 trampler vs the real vanilla 4/4). A guide written
   from Oracle memory misstates costs, stats, and — worst — the haste roster, which is load-bearing (which creatures
   swing the turn they land). Cross-check any behavior-bearing keyword claim against a REAL observed window (a
   named-but-engine-dropped attacker confirms no-haste). Route every card-script divergence to notes.md for
   Oracle-verification (per the wagicgpt-verify-oracle-text discipline) — but the GUIDE uses what the engine does.

2. **Let the REASONING-TAX SPIKES rank your teaching points — they are the saliency signal.** In a guideless corpus,
   every place qwen burned 800–3,000 words (or looped into `unparsed_reply`) re-deriving something is a deck fact the
   core does not carry, surfaced for free. Rank candidate teaching points by BOTH (a) did it cause a wrong or
   near-wrong action, AND (b) reasoning-tax magnitude. The two map together (deck21: the top tax spikes — Spark
   end-step timing 2,773 words, summoning-sick token alpha-strike math 1,266, burn-can't-kill-a-6/6 loop
   2,099→unparsed — were the top teaching points 1:1). The guide's SECOND job (after correctness) is to short-circuit
   these loops, and the tax map tells you exactly which derivations to pre-answer. This also matters because tax
   drives the p90 latency that costs adjudication losses (deck17 lost 3 games grinding slowly while even/ahead).

3. **When ONE idiosyncrasy dominates the deck, make its corrective FACT rule #1 — ABOVE the win engine.** If a single
   false belief taxes or vetoes the engine's own plays, the fact that kills it outranks the engine in the guide
   (deck17: ~all-hybrid mana off mono-Islands → "your mana is simple; every spell casts off Islands alone; a hybrid
   pip is ONE mana" is rule #1, above the anthem/flyer engine, because the belief was the #1 source of both lost
   decisions and tax; deck21: burn-is-a-clock and Spark-timing sit above the anthem engine). Tie the fact to the
   visible list ("if a Cast option is listed, it is payable right now") AND give the positive follow-through ("cast
   the BIGGEST flyer offered"), because the belief's downstream cost is under-deployment, not just tax.

4. **A first guide's job is often to COUNTERMAND a generic core line, not just add facts.** A bare deck-fact list
   will not stop the model obeying a core reflex that is wrong for this archetype. When a new archetype's core
   resource collides with a generic core-prompt line, NAME and REVERSE the reflex explicitly for THIS deck's cards
   (deck21 vs140: qwen quoted the core's "hold instant-speed interaction" verbatim and held its Lightning Bolts into
   an empty board while a lifegain deck stabilized → the loss; deck21's guide now says "NEVER hold burn for
   interaction — that advice is for control decks; your burn is a clock, fire it at the face on your turn"; deck109's
   frozen guide already encodes the same override for Searing Spear). This is the sanctioned per-deck route for a
   core line that is correct in general but anti-teaching for one archetype — cheaper and safer than a core edit.

5. **Write the ATTACK / DEPLOY floor on the FIRST pass** for any evasion or aggro deck — do not wait for a revision
   to expose the passivity. The attack-floor-does-not-transfer lesson (this skill's deck135 wave-5 rung) recurred on
   BOTH newcomers this cycle (deck17 went passive with unblockable flyers vs a lifegain deck that climbed to 64;
   deck21 durdled to a naked board). Key the floor on a board-readable fact ("opponent has no flyer/reach → all your
   flyers connect → attack with every untapped flyer, every turn") with the closed exception form ("no attackers
   only when every flyer is tapped or would die for no gain"). If the deck mixes attackers and walls, name the walls
   as an explicit exclusion INSIDE the attack floor (deck17: Oona's Gatewarden and Plumeveil are Defenders — never
   declare them as attackers), because the engine silently drops ineligible attackers so the mistake is invisible in
   `choice`; score attacker declarations against creature ELIGIBILITY, not the recorded choice. (Derive-the-defender-
   set is single-seat deck17 → WATCH as a general rule, but apply it whenever a deck has walls.)

6. **Match density to the veteran band (~one screen).** deck17 landed 85 lines / 6.3 KB and deck21 similar — at the
   top of the target band, justified by needing a full steal/anthem section + the #1 idiosyncrasy rule. On the first
   REVISION, trim any section the next corpus shows as pure tax.

**Engine-deck refinements (wave 16, deck62 — the FIRST engine/combo deck through Step 0-bis; single-seat, tagged
pending a second engine deck; each EXTENDS a rung above, not replaces it).**
- **(rule 3, engine-deck face) When the deck is an ENGINE/combo deck, rule #1 is a CAPABILITY TABLE.** The dominant
  idiosyncrasy on an engine deck is not a belief that VETOES a real option (the 17/21 hybrid / hold-burn shape) — it
  is the pilot FABRICATING card text: inventing effects the cards do not have (deck62: qwen "cast Fists of Ironwood
  to kill a 0/2", a pump aura that deals zero damage; 13k tax across three records). The corrective is not "you CAN
  do X" but "your card does NOT do X; here is what it actually does." Make rule #1 a capability table — one plain-
  effect line per key card — ranked by mis-description tax (the cards qwen wrote the most wrong words about go
  first). A capability table pre-empts the whole fabrication class.
- **(rule 5, generalized to ORDERING) Teach engine SEQUENCING as an explicit order.** A weak pilot will not infer a
  play-order from a static ability description (deck62: the Argothian Enchantress "draw on enchantment cast" line was
  present in 47 records and qwen NEVER cast her before dumping its enchantments). When a deck has a trigger whose
  value depends on PLAY ORDER (an enchantress, a prowess/magecraft body, a sacrifice payoff), write the sequence as
  an explicit rule ("cast her FIRST, THEN dump enchantments"), not just the card's ability text — this generalizes
  rule 5's "write the floor on the first pass" from ATTACKING to SEQUENCING.
- **(rule 5, exclusion clause broadened) Name every creature the model should NOT declare as an attacker, and say
  the eligible list already excludes summoning-sick bodies.** deck17 named its Defenders as the attack-floor
  exclusion; deck62 shows the SAME exclusion applies to 0-power ENGINE/mana creatures (Argothian 0/1, un-pumped
  Birds 0/1) and to SUMMONING-SICK tokens the model re-counts (deck62 drowned counting 4 sick Saprolings the offered
  list had already dropped). Broaden the clause from "name the walls" to "name every creature the model should NOT
  declare — walls, 0-power engine/mana dorks — and state the 'creatures that can attack' list is the eligible set
  (summoning-sick bodies already dropped), so the model must not re-derive eligibility." (2-seat-adjacent.)
- **(rule 2, stat-stacking corollary → TRUST THE RENDERED NUMBER) On an aura/counter/anthem stat-stacking deck, tell
  the pilot to read the printed [X/Y], not recompute it.** deck62 re-derived a printed [9/10] Birds from its aura
  stack even in a WON game (10.4k tax) instead of reading the battlefield line the engine already computed. This
  CONVERGES at a second seat with deck17's stale-within-turn-mana "trust the Mana-available line / cast list, do not
  recount your Islands" — the same TRUST-THE-RENDERED-NUMBER shape (both paired with a representation-ledger lever
  that renders the final number authoritatively). Write the guide line ("trust the printed [X/Y] and swing / cast
  from the list you are given") whenever a deck stacks stats or spends mana across multiple same-turn casts.

The output contract is otherwise Step-0-identical: findings.md drives it; engine/card-script items go to notes.md,
never guide prose; the guide is CONCLUSIONS as orders for a weak local executor (the "one idea that governs
everything"). The newcomer's guide must be VALIDATED next corpus — it is a hypothesis, not a freeze.

**Wave-17 additions / promotions to Step 0-bis (two guideless newcomers: deck49 Dragons, deck35 Slivers).**
- **(rule 1/2, PROMOTED — TRUST THE RENDERED NUMBER is now a CONFIRMED general first-pass rule, 4 seats).** The
  wave-16 deck62 S4 refinement de-tags: confirmed at deck17 (mana line), deck49 (mono-red mana), deck62 (Blanchwood
  computed P/T), deck35 (sliver lord stack). For ANY newcomer whose mana or board math is DERIVED, write a rule-#1-
  or-#2 trust line with BOTH faces where they apply: (a) mana — "if a Cast option is listed it is payable; trust the
  Mana-available line; do not recount lands / re-derive payability"; (b) board stats — "the battlefield [X/Y]
  already counts auras/lords/anthems/counters; read it and swing, do not re-derive P/T from base stats + each lord."
  Pair with the representation-ledger lever. NOTE the representation is present but not yet WINNING against the
  false belief on mono-color decks (deck49/deck135 still spiked over the surfaced mana line) — so the guide trust-
  line is load-bearing; do NOT demote it to the representation lever alone yet.
- **(rule 4/aura, ADDED — the opponent-only-targets OFF-RAMP, 2 seats).** An aura/pump deck's guide needs more than
  "never enchant the opponent": when a beneficial aura is offered with ONLY opponent-creature targets, the weak
  pilot LOOPS to unparsed hunting for a use (deck49 Goblin War Paint: deck17 s18 12.4k, deck135 s22 13.8k, both
  fallbacks). Pair the own-creature-only rule with the terminal off-ramp: "if your only legal aura targets are
  opponent creatures, DO NOT cast it — Cast-nothing / play something else; do not agonize." (Durable fix is engine-
  side, ledger E-49c — suppress the beneficial-aura cast when every legal target is opponent-controlled, mirror of
  the shipped bad-effect suppression; the guide off-ramp is the interim.)
- **(rule 3, WATCH — a THIRD face of "the dominant idiosyncrasy": the TARGET SUB-MENU seam, single-seat deck49).**
  Rule-3 currently names belief-that-VETOES (17/21) and card-text FABRICATION (62). deck49 (spell/burn/aura-heavy)
  is a third face: the idiosyncrasy is an INTERFACE seam — after committing to a targeted cast, the engine opens a
  separate target sub-menu the weak model does not model, concentrating the stale_echoes + tax there. Teach the flow
  ("choosing a targeted cast opens a second menu listing the TARGETS; pick by number; there is no Cast-nothing after
  committing; do not re-name the spell"). Single-seat → WATCH; the durable fix is the engine sub-menu representation
  (ledger #1). deck49 shipped it as its guide rule-2.

**Wave-20 additions to Step 0-bis (one guideless newcomer: deck102 Tergrid, a single-payoff-card ENGINE deck; single-
seat, tagged pending a second instance; each EXTENDS a rung above).**
- **(NEW face) The ABSENT-ENGINE-CARD newcomer — write the guide for the payoff-ABSENT MAJORITY case, with a
  prioritize-and-protect-WHEN-DRAWN clause on top.** When a guideless deck is built around a SINGLE payoff card and the
  corpus never drew it (deck102: Tergrid, a 1-of in a 100-card singleton, appeared in ZERO prompts across 6 games —
  ~38% expected whiff), the corpus is a pure sample of the deck's HEADLESS mode. Two rungs: (a) do NOT build the
  guide's spine around the engine card the pilot rarely sees — make the deck FUNCTION in the ~2/3 of games where the
  payoff never shows (disrupt + deploy a real threat + attack), then carry the engine as a prioritize+protect-when-
  drawn clause ABOVE it; a guide whose #1 rule is "assemble the loop" is dead text in most games. (b) VERIFY LIBRARY vs
  stranded-zone before concluding the engine is "unreachable" — a payoff appearing 0 times is an engine bug (a
  command-zone card with no cast path) OR ordinary variance; disambiguate with library-size arithmetic (deck102:
  library 93 at turn 1 = 100−7, and the list has 99 non-commander cards, so the 100th IS shuffled in → variance). Do
  not log a variance whiff as an engine defect, and do not "fix" the guide against a card the pilot simply never drew.
  (Corollary for corpus-readers: a build-dependent ENGINE deck whose whole plan keys on one card over-represents the
  headless mode in its win-rate; to VALIDATE the engine specifically, use a probe deck that stacks the payoff + its
  enablers rather than waiting on the 1-of.)
- **(rule 3, a FOURTH idiosyncrasy face) The SYMMETRIC-cost card the pilot reads as one-sided.** Rule 3 now names four
  idiosyncrasy faces: belief-that-VETOES (17/21), card-text FABRICATION (62), the target-SUB-MENU interface seam (49),
  and — deck102 — a SYMMETRIC-cost card (Pox/Smallpox: each player loses life, discards, sacrifices) fired as if it
  were one-sided disruption, paying life/resources it cannot afford while behind (vs35 Pox at L8, vs49 Pox at L6 vs
  aggro). The tell: the option line RENDERS the self-cost (`{right now: life −7, life −3}`) and the model casts anyway
  — so it is a DECK-FACT gap, not a representation gap (the fact IS surfaced; the model does not know the card is a
  payoff-card without its payoff). Corrective is a scoped FIRE-GATE keyed to board/life state ("cast a symmetric
  mass-effect ONLY when the payoff is out, OR you are ahead on board, OR your own hand/board is empty; NEVER while
  behind or being raced"), NEVER a hard "you can't cast it" (doctrine: annotations/gates, not suppression of a legal
  play). Sibling to the TRUST-THE-RENDERED-NUMBER face — there the number is right and the model re-derives it; here
  the number is right and the model IGNORES its sign. (Flagged for eventual CORE-PROMPT promotion IF a second symmetric
  deck appears — a single core sentence would then save re-authoring it per deck; single-seat now, so it stays in the
  guide.)
- **(rule 3, a FIFTH idiosyncrasy face — tagged single-seat) The ALTERNATIVE-COST DUAL-OPTION card whose alt-cost
  option carries the OTHER face's stats.** Rule 3 now names five idiosyncrasy faces: belief-that-VETOES (17/21),
  card-text FABRICATION (62), the target-SUB-MENU interface seam (49), the SYMMETRIC-cost card read as one-sided
  (102), and — deck137 (GW Adventures) — a single card the engine offers as TWO cast options (a base-cost face and a
  "with its `<alternative>` cost" face) where the ALTERNATIVE option MISLABELS itself with the PRIMARY face's (P/T)
  and carries no effect text. The executor reads the (P/T) as the body it will deploy by paying the alternative cost
  and casts the wrong thing (deck131 s15: options `1. Cast Shepherd of the Flock // Usher to Safety with its adventure
  cost {w} (3/1)`, reasoned "Shepherd is a 3/1 creature ... adds another body," chose 1 — the game cast the Usher to
  Safety BOUNCE spell and it self-bounced its own deployed Faerie Guidemother; deck27 s19 a 9,315-char tax talking
  past the label before playing correctly). Mechanism split from the four existing faces: it is NOT a fabricated
  ability (62) and NOT a fully hidden fact — the fact is MISRENDERED (a real (P/T), just the WRONG face's), so the
  model is actively MISLED rather than uninformed; the closest sibling is the FOURTH (symmetric-cost) face — there the
  number is right and its SIGN is misread, here the (P/T) is right for the OTHER face and misread as belonging to this
  option. Authoring move (matches the rung's annotate/teach, never-suppress doctrine): make rule #1 a CAPABILITY TABLE
  with one plain line per card (what the CREATURE does, what the SPELL/adventure does) plus the corrective fact "the
  (P/T) on the alternative-cost option is the creature you get LATER from exile, NOT what you deploy now" — the same
  corrective template as the 62 capability table, extended to a per-FACE table. Pair it with the representation-ledger
  item (annotate the alt-cost option with its own effect text / drop the wrong P/T) so the guide block DEMOTES once
  the option carries the effect. Generality note: this shape recurs for ANY engine card offered as multiple cast
  options at different costs where the alt-cost option inherits the primary face's annotation — adventures, and
  plausibly other "alternative name(...)" cards (convoke/overload render "with its `<X>` cost" too, though those share
  ONE effect so the mislabel is milder). Tag single-seat; promote to a named rule-3 face if a second alternative-cost-
  dual-option deck reproduces the misfire. (A weaker sibling observation, NOT a rung: the convoke X-cost cards drove
  the corpus's three deferred_to_heuristic tax spikes — the model re-derives "do I convoke / which do I tap" when the
  engine already offers a payable "with its convoke cost" option and defers the tap-selection to the heuristic; if a
  second convoke deck reproduces the tax, "trust the offered alternative-cost option, the engine handles payment"
  earns a general rung alongside TRUST-THE-RENDERED-NUMBER — the same family: trust the surface, don't re-derive the
  mechanic.)
  **(wave-24 status: CLOSED-BY-FACT, not promoted.)** The engine fix shipped (adventure-cost options now render
  `{adventure spell: <effect text>}` with NO wrong (P/T)); deck137's live acceptance PASSED conclusively (the Usher
  self-bounce misfire recurred 0×; adventure-spell casts are now effect-driven), and per the removal condition the
  interim P/T-TRAP crutch was DEMOTED to pure strategy (when to use each face). This face was a REPRESENTATION
  misrender, not a durable model belief, so a FACT fully fixed it → retire the WATCH as resolved-at-representation
  rather than promoting it. It is the reference witness of the crutch-lifecycle matched pair (annotate to teach → ship
  the FACT → demote the crutch); contrast the genuine model-belief faces (first/fourth/sixth), which persist because
  no FACT can fully fix a belief. (Convoke defers, wave-24: 19 by-design tap-selection routes, all HARMLESS — the
  heuristic never stranded a wanted attacker/blocker; leave by-design, the "trust the convoke option" rung stays
  single-seat pending a second convoke deck.)
- **(rule 3, a SIXTH idiosyncrasy face — tagged single-seat) The SYMMETRIC EFFECT that is ASYMMETRIC IN YOUR FAVOR via
  recursion — the clean INVERSE of the FOURTH face.** Rule 3 now names six idiosyncrasy faces: belief-that-VETOES
  (17/21/59), card-text FABRICATION (62), the target-SUB-MENU interface seam (49), the SYMMETRIC-cost card read as
  one-sided (102, the fourth), the ALTERNATIVE-COST DUAL-OPTION mislabel (137, the fifth — CLOSED-BY-FACT above), and —
  deck59 (WB Persistence) — a symmetric MASS-REMOVAL (Wrath of God / Day of Judgment / Damnation) the pilot UNDER-fires
  because it does not see that the deck's OWN persist recursion makes the wipe one-sided IN ITS FAVOR: your creatures
  return (as 1/1s with a -1/-1 counter), the opponent's stay dead. The pilot treats the sweeper as generic "slow
  removal" and declines it in exactly the spot that wins the game (deck109 s12: at 6 life facing lethal, "Day of
  Judgment is too slow and risky," played a blocker instead, swept only at 2 life — too late — and LOST; contrast the
  deck131 WIN, racing AHEAD, where it was offered sweepers 4× and correctly NEVER cast them). This shares the fourth
  face's MECHANISM — a symmetric card mis-read as if the symmetry did not apply to the side it helps or hurts — but
  needs the OPPOSITE lever: where the fourth face wants a scoped STOP-gate (don't fire when the symmetry hurts you), the
  sixth wants a scoped GO-gate keyed to a board-readable trigger — "cast the symmetric effect WHEN [behind / raced /
  opponent gone wide] because YOUR side recovers and theirs does not; NEVER when ahead (you would kill your own
  attackers)" — AND the GO-gate MUST carry the recursion WHY, or the weak executor keeps pattern-matching "board wipe =
  slow / kills my stuff = bad." A bare capability line ("Wrath destroys all creatures") does NOT countermand the
  reflex; only "your creatures COME BACK, so the wipe is one-sided FOR YOU, cast it when behind" does. This is also the
  rule-4 COUNTERMAND-A-CORE-REFLEX move applied to combat math: the core's correct-in-general "don't overcommit into a
  sweeper / a wipe kills your board" prior is anti-teaching for a deck whose board survives its own wipes. Pair with a
  representation note (the block-outcome annotation should mark that a persist blocker returns — deck59 notes: "(both
  die)" understates chump/trade value for a persist deck) so the recursion value is visible at the combat seam too.
  Tag single-seat; promote to a named rule-3 face if a SECOND deck whose own engine inverts a symmetric effect (another
  recursion/persist deck, an undying deck, a "you gain what they lose" symmetric payoff) reproduces the UNDER-fire.
  (Corpus note: the belief-that-VETOES face is now a THIRD seat here — deck59's W/B hybrid creatures off Plains+Swamp,
  no green source, vetoes/taxes its own casts thinking it needs green — and it now carries a MULLIGAN-quality cost, not
  only a cast-tax cost: deck59 mulliganed a KEEPABLE all-Plains 7 to color-anxiety ("no black") and got run over. The
  same corrective FACT — rule #1 mana simplification + pip table + trust-the-rendered-number — fixes the mulligan too.)
- **(reveal-seam instrument, NEW sub-class) The DISCARD-PICK / choose-from-opponent-hand reveal is a pick-CARDINALITY
  surface — read the `<N>` token in the `auto=` and score OVER-SELECTION.** deck135's reveal rung covered LIBRARY-
  partition reveals (choose-subset-to-a-zone); deck102 adds the TARGETED-DISCARD face (Thoughtseize/Duress: choose
  from the OPPONENT's revealed hand). It looks identical in the prompt (same "choose discards / look" partition
  framing) but is a pick-EXACTLY-N effect. Read the `<N>` (Thoughtseize `target(<1>*[-land]|reveal)` = pick ONE); a
  pick-ONE effect rendered as choose-a-SUBSET drives OVER-SELECTION (deck102 picked both Boomerangs, all 3 nonlands) —
  harmless ONLY when the true best card is the lowest eligible index, a LATENT quality bug when it is not. Score
  mechanical integrity by "which card actually left the hand" in the next record's `events` (the ENGINE-R1 same-tick
  finalize is confirmed here too — lowest-index eligible pick leaves once). Defect is REPRESENTATION (the prompt
  mislabels the opponent's hand as "top N of your library" and uses subset framing on a pick-ONE) → ledger; the guide
  teaches the pick-ONE fact interim.
- **(wave-18 spiral rung, AMPLIFIER note) The losing-position spiral scales with the REVIEWED deck's own board
  complexity.** A PRISON/ATTRITION deck amplifies the behind/stalled giant-reply spiral because its OWN win-more
  pieces (No Mercy, Painful Quandary, Oppression, Megrim) build LARGE stalled boards → more objects to re-derive →
  longer replies → more truncations (all 6 deck102 fallbacks were >13k). When scoring the spiral for a deck that
  intentionally grinds to a big board, expect the tax to be worse than for an aggro deck and weight the off-case
  branch accordingly — the off-case is where most of these archetypes' turns LIVE, not a rare corner. Fix is still the
  GUIDE off-case behavior branch, not a token-budget raise (that only makes the spiral longer).

## Step 0-ter — Validating a FIRST GUIDED corpus (the newcomer guide's first at-scale test)

New in wave 16, converged from the first two first-guided seats (decks 17 and 21). When a deck plays its FIRST corpus
WITH its Step-0-bis initial guide, the review is neither a freeze audit nor a blind revise — it is a VALIDATION:
report, per taught class, whether the class DIED (fixed, no recurrence), PERSISTED (working as taught, or present-
and-disobeyed), or MUTATED (the belief died but a successor tax appeared), each with seq repros — and only THEN
decide the revision. Procedure:

1. **Report died/persisted/mutated per taught class BEFORE you read the win-rate.** The record is a coin-flip
   surface at this stage — the pool swap and variance move it more than the guide does (deck21 dropped 3/6→2/6 with
   its entire wave-15 tax epicenter GONE and 3 of 4 losses within 4 life; the pool lost a deck it would now beat and
   gained a near-unwinnable one). The guide's JOB is: did the taught classes die, and did the reasoning-TAX collapse?
   Read the tax collapse (decision counts, reply lengths, fallback rate at the taught seams) as the primary evidence
   the guide worked; the win-rate is context.

2. **Reconcile a "looks-disobeyed" choice against the OFFERED / eligible set before calling it a play failure.** A
   taught floor that "looks disobeyed" is often obeyed against a set you did not read (deck17: the tempting "chronic
   under-attacking" read was FALSE — every single-attacker turn had exactly ONE eligible creature in the engine's
   `A1..An` list, the rest summoning-sick or defenders; the model attacked optimally). This is the attacker-seat
   cousin of the wave-15 defender-drop lesson (score `ATTACK:` against the eligibility list, never the raw `choice`).

3. **A first revision often TIGHTENS an over-broad rule, not just adds facts.** The biggest first-corpus RISK of a
   guideless-pass guide is an OVER-BROAD countermand hiding in a MATCHUP section — the more-specific matchup line
   silently OVERRIDES a general exception in the model's reasoning (deck21's "send EVERY burn at the face vs a flyer
   swarm" lumped a killable small-flyer swarm with an unbreakable big wall and countermanded its own kill-the-flyer
   exception → two races lost by exactly 4 life; deck17's "cast the biggest flyer" cast a 4/4 Defender wall the model
   itself reasoned "does not advance the clock"). When you give a "cast/kill the biggest/best Y" rule and the pool
   contains a Y that satisfies the surface property but not the INTENT, scope the superlative to the intent ("biggest
   flyer THAT CAN ATTACK"); check every general exception against every matchup section for contradiction.

4. **When a taught belief DIES but the TAX SIGNATURE persists at the same seam, the tax RELOCATED — name the
   successor, do not conclude the fix failed, and do not re-teach the corpse.** deck17's hybrid off-color belief is
   dead (zero recurrence) but the giant replies at the cast seam did not vanish; they moved to stale WITHIN-TURN mana
   tracking (the model recounts mana from its Island total on a second same-turn cast and distrusts the correct cast
   list). Reply-length at the seam is the tax gauge that survives the belief swap; route the successor to its correct
   layer (here: a representation lever + a guide trust-line, NOT another hybrid paragraph).

5. **Trim the validated-DEAD sections to flat statements; spend the bytes on the classes still failing.** A class the
   corpus proves solved is pure length — compress its verbose explanation to one flat line (deck17 trimmed the
   defender-attack, Cloud-Sprite-block, and mulligan sections, held the guide at 80 lines while ADDING the race +
   deployment fixes). Do not trim a class merely because it did not recur this corpus if its precondition never
   engaged (see rule 6).

6. **Write the OFF-CASE branch on the first revision even if the winning corpus never needed it — the LOSING corpus
   lives entirely in the off-case.** A first guide built on an "almost always" board fact leaves the off-case
   unguided, and that is exactly where a validated guide loses (deck17's spine "the opponent has no flyer, which is
   almost always" won all 4 ground-bound matchups cleanly and LOST both has-flyer/lifelink matchups, where the
   precondition silently failed and nothing told the model how to RACE). And validate the fix-WINDOW, not just the
   fix direction, when the target GROWS: a bare "kill the small flyer" rule fired too late is worthless once the
   flyer pumps out of burn range, so the rule must carry the timing ("early or never"). Replace value-evocations
   ("worth more than 3 face damage") with the countable MECHANIC the board proves ("a 1/1 flyer you cannot block
   costs you 1 life every turn of the race") — word choice selects the procedure the model runs.

The output contract is Step-0-identical. A validated guide that produced a revision is NOT a rotation candidate (it
carries a new-work signal by construction); re-validate the revision next corpus, focused on the matchups that
exposed the off-case.

**Wave-17 additions to Step 0-ter (three first-guide/revision validations: 62 first-guide, 17/21 revised).**
- **(review instrument, ADOPTED — the 0-ATTACKER-GAME durdle signature; single-seat deck35, justified as a
  MEASUREMENT not a doctrine).** For any deck the guide labels "attack every turn," tabulate the per-game
  `attackers`-decision COUNT and the own-creature count over time. A game with ZERO `attackers` records (and/or an
  empty own board despite many creatures cast) is proof the durdle fired — independent of win/loss, cheaper to read
  than the prose. Then check WHY the attack step was never reached (chumped-away vs tapped vs summoning-sick).
  Sibling to rule 2's "reconcile looks-disobeyed against the eligible set" — here the eligible set was empty because
  the model chumped it away. (Adoptable single-seat because it only makes an existing signal countable; it biases no
  guide content.)
- **(re-check rung, CONFIRMED) a "died quiet" verdict is a RE-CHECK candidate.** A class that stops producing WRONG
  CHOICES may still be producing WASTED REASONING — deck17's Cloud-Sprite block restriction did not die, it MUTATED
  from a wrong-choice into a token-budget ramble; without the explicit died/persisted/MUTATED pass it would be mis-
  recorded as settled. When the fallback rate collapses, RE-CLASSIFY the survivors by MECHANISM (reply length + what
  the model actually did), not by kind label: a surviving "unparsed" after a good guide is usually a new, narrower
  bug (deck62 vs135 s19 = crisp list-vs-board misread), not the old tax persisting (rule 4's tax-relocation cousin).
- **(evidence rung, CONFIRMED) a root-cause fix that REVERSES its causal matchup beats an aggregate win-rate tick.**
  deck17 vs110 flipping loss→win under the new walls-aren't-flyers + race rules is stronger evidence the fix fired
  than 4/6→5/6. The skill already privileges the decision over the record; this is a clean worked example.
- **(comprehension-watch rung, CONFIRMED) score the ACTION, not the arithmetic or the verbal model.** A correct
  number does not prove comprehension and a wrong verbal rule does not prove a misunderstanding: deck35 computed
  team power correctly (buff-math PASS) while stating a wrong rule ("lords buff future slivers only") — the action
  was right, so the verdict is PASS and the guide addresses the verbal tic cheaply without a representation change.
  Extends "read RESULT not INTENT" to the does-the-model-understand-X watch.
- **(scoping WATCH, single-seat deck21 + 2-seat-adjacent) when you write/revise a COUNTABLE-COST rule, name the
  input QUANTITY and give its zero/degenerate case a branch.** "An X costs you N per turn, so it is worth a burn"
  fires on any eligible target even when N=0 (deck21 vs62 s6: the race-math exception scoped on TOUGHNESS fired on a
  0-POWER Birds, game lost by 1 life). Same shape as deck17's "biggest flyer THAT CAN ATTACK" superlative-scoping —
  the model reads the eligibility surface, not the intent quantity. deck21 fixed it in-guide (POWER FLOOR); hold the
  method generalization pending a second direct (unconfounded) instance. Related structural WATCH: at an auto-no-
  block seat the race-math generalizes from flyers to ANY persistent attacker — separate the structural fact from
  the guide change; do not broaden on one confounded instance.

**Wave-18 additions to Step 0-ter (one guideless newcomer [14]; two first-guide validations [49/35]; one
revised-round-2 [62]; three freezes).**
- **(PROMOTED rung — the LOSING-POSITION / OFF-CASE SPIRAL, 3-seat convergence).** Rule 6 said the losing corpus
  lives in the off-case; wave-18 names its dominant TAX: behind and unable to win THIS turn, the weak pilot
  re-derives its whole board hunting for impossible lethal ("10 is not 32, I must have made a mistake"), blows the
  token budget, and truncates → unparse. Three seats independently authored the fix as a BEHAVIORAL off-case
  branch in their revision — deck62 ("when behind you cannot always win this turn; make the best play and pass, do
  not hunt impossible lethal"), deck35 (a forced-lethal escape clause), deck49 ("when your curve fails you, the
  burn is your deck"). CRITICAL: this is a BEHAVIOR lever, NOT a token-budget lever — raising max-tokens makes the
  model spiral longer and still truncate (deck62 #5). Author the off-case branch on the first revision even if the
  winning corpus never engaged it. (Its CORE-layer cousin — a general "you already did the math / the outcome is
  forced, state the choice" brake — is a single-seat-each CORE WATCH this wave; the GUIDE off-case branch is the
  shipped lever, so measure whether it collapses the spiral before adding any core line — do not double-layer.)
- **(PROMOTED measurement rung — the tax gauge is GIANT-REPLY COUNT, not fallback rate; 4 seats 62/49/21/35).**
  After a good guide the fallback rate collapses or flattens while the reasoning tax RELOCATES and survives as
  reply LENGTH (deck62: fallbacks flat 2.1→2.7% but >8k giants 1→8; deck49: 0 fallbacks yet 20/125 replies ≥600
  words; deck21: 4 benign giants, all in won games; deck35: an 11k block-seam giant was the corpus's only
  fallback). Count giant replies (>8k) and BUCKET them by board-state (winning vs behind) — a win-rate drop from a
  harder pool shows up as a giant EXPLOSION localized to the losing off-case, not a taught-class regression.
  Extends rule 4. (Corollary — distinguish "recount" FLAVORS before crediting a trust-line ban: recounting the
  printed P/T of a body ALREADY wearing the aura/anthem is the ban's target; PROSPECTIVE cast-value math on a body
  NOT yet wearing it is legitimate and unbannable — deck62 #4.)
- **(SHARPENED rung — "trust the rendered number" must be TWO-SIDED; deck35 worked example + applied, deck14
  combat face).** The promoted trust rule scoped to the pilot's OWN board leaves the model free to DISTRUST the
  other side and re-open the very computation the annotation closes: deck35 obeyed its own lord-stack [X/Y]
  perfectly yet re-derived the OPPONENT's 10/12 aura creature's power and spiraled when its (wrong) arithmetic
  disagreed with the correct "deal up to N" annotation (the corpus's only fallback). Scope "trust the rendered
  number" to BOTH boards AND to any damage/effect annotation the representation provides; deck14 adds the COMBAT
  face (trust the rendered [X/Y] through flyer/lord/persist math). Durable fix = engine-side block-outcome
  annotation (ledger); the two-sided guide line is the interim.
- **(SHARPENED rule 5 — apportion a concurrent ENGINE fix before crediting the guide).** When a first-guided
  corpus ALSO ships an engine change targeting the same seam, ask for each dead class "would the engine change
  alone have killed it?" deck49's whole fallback collapse (7 stale_echo + 2 unparsed → 0) was ENGINE-attributable
  (TARGET CHOICE framing + "(player, life N)" strip + E-49c), so its paired target-menu Rule #2 became
  trim-eligible on the fallback dimension; the guide's real credit is the class the engine did NOT touch (C1
  mana-belief — Rule #1 owns that kill, the engine never changed mana representation). Apportionment DRIVES the
  trim: trim guide prose the ENGINE now carries in the representation, keep the prose for classes it does not.
- **(SHARPENED rule 3 — a validated tightening can persist-and-DISOBEY under a TRIGGER; name the trigger).**
  deck62's A-list-only tightening did not die; it recurred twice, BOTH when the model was behind in "I MUST attack
  to win" mode and overrode the list with a wish-read. Restating the rule harder is weak; bind eligibility to the
  list FOR THE TRIGGER ("this holds EVEN when you feel you must attack to win; wanting the damage does not add a
  summoning-sick creature to combat"). Sibling to the "biggest flyer THAT CAN ATTACK" superlative-scoping — the
  model reads the surface it WANTS, so pre-empt the specific want.
- **(instrument ADOPTED — the BURN-TARGET-MENU FACE-RATE; single-seat deck49, measurement-only).** For any "aim
  your burn" deck, tabulate (target menus resolved to FACE)/(total burn/player target menus) AND how many face
  choices had an enemy creature on the menu and declined it (deck49: 17/17 face, 9 declining an offered enemy
  creature). Countable proof BURN-GOES-FACE internalized; sibling to the durdle instrument; biases no guide content.
- **(instrument ADOPTED — the REPEAT-LOOP unparsed signature; single-seat deck135, measurement-only).** A residual
  unparsed can be a DECODE-TIME repetition spiral (one/two sentences repeated dozens-to-hundreds of times until
  the token cap truncates), NOT the PLAN-caveat stale-plan class. Detect: a short phrase repeated many times with
  NO stale prior-turn PLAN. Trigger profile: low-life / ambiguous spots on a phase-restricted / binary / trigger
  menu. Severity-gate by whether the loop landed in a WINNABLE game (deck135's 3 all sat in already-lost or
  post-adjudication spots → cosmetic/tax → route to model/decode watch: repetition penalty / max-token guard with
  a last-well-formed-CHOICE salvage — NOT the guide). Its ABSENCE from the PLAN-caveat class is the healthy signal.
- **(method — treat FREEZE and TRIM as two separate operations; never bundle).** A converged guide flagged for a
  length trim (deck21, grew two waves running) gets the trim as a DEDICATED, separately-validated pass — a trim is
  a behavior-risking edit, and bundling it with the freeze that closes the last open class forfeits attribution (a
  next-wave regression can't be pinned to trim vs noise). The redundancy may be LOAD-BEARING (deck21 quoted the
  power-floor from more than one section, plausibly WHY it fired so reliably) — trim conservatively and re-validate
  on a corpus that re-includes the motivating opponent types before shipping.

## Inputs to read, in this order

1. **The decklist** — `bin/Res/ai/baka/deckN.txt`. Format: `#NAME/#DESC/#HINT` headers,
   then `CardName (SET) * qty`; bare numbers are basic-land ids; the SET code is
   irrelevant. This tells you the archetype's intent.
   - **The `#HINT` lines are for the OTHER (heuristic Baka) AI, not qwen.** Read them for
     the designer's intent and any encoded combo/attack rule, but note where a hint would
     MISLEAD this deck's ideal play. Don't propagate a hint blindly.

2. **The general system prompt** — `bin/Res/ai/gpt/system_prompt.txt`. EVERY deck already
   gets this, so READ THE CURRENT VERSION. As of the wave-5 revision it carries, as UNIVERSAL
   priors and rules facts: decide who's the beatdown PLUS a per-turn race re-check on
   board-readable damage-flow facts (their damage landing while yours isn't = losing the race,
   switch to defense); act every turn under an explicit act-floor ("Cast nothing" allowed only
   via a closed exception list) — now completed with the HIGHEST-IMPACT rule (the floor is
   satisfied by your best legal play, not the cheapest: deploy a threat before a same-turn
   value/ramp/dig/discard spell); deploy your win conditions; instant-vs-non-instant timing WITH
   release valves on holding, and reactions now keyed to the SHIPPED ON THE STACK section (each
   pending spell named with controller/targets) + labeled counter options ("can target on the
   stack: <name>") — the wave-4 offer-is-the-signal / log-tail tell is RETIRED (see below) — PLUS
   the mana-discipline fact (a response window opens only while a response is PAYABLE, so a seat
   that taps out on its own turn gets no window; hold reaction mana open); choose targets by IMPACT;
   activation gas AND brakes — now including the once-per-turn same-activation brake with the
   two-power-numbers perception cue, and the ping arithmetic (N damage targets a creature only
   when N ≥ its toughness, else face); cards-vs-tempo; a COMBAT block that is now runnable
   per-item, not posture: the free-case attack floor, the per-creature three-test attack check
   with a closed "no attackers" exception, the attacker-punisher recompute rule, lethal math run
   twice, the reverse survival check, and survival-first blocking at the 8-life threshold with
   the named-gain/rules-text-check block procedure; the mechanics facts (attacking taps
   creatures not lands, summoning sickness is never a casting veto, X is chosen at cast, hybrid
   pays either color, FIRST STRIKE deals first and kills before damage back); a mulligan
   land-count default; the interface contract (every listed action is legal AND payable, lands
   auto-tap, empty pool is normal, offered cast = YOUR turn and YOUR window, lists are complete,
   attacker/blocker replies are the COMPLETE declaration, listed ≠ recommended); stale-plan
   grounding (board beats plan, still-offered = not done) plus the cast-happened `hand -> stack`
   verification anchor (a CORE-LAYER STOPGAP for the engine no-op bug); anti-deferral ("next
   turn" = now); plan/number binding PLUS plan-FROM-the-list (a missing card is never a reason
   to choose "Cast nothing") and numeric plan-binding (the option TEXT must contain the exact
   number the plan names); cost paying incl. prompt fetch-cracking; and a PRECEDENCE clause —
   the deck guide beats the general priors on conflict. **Wave-5 authors: do NOT restate the new
   combat block, the interface-contract completions, or the plan-from-the-list line — the
   restatement ban covers them now.** Stopgap status changed this wave: the counterspell tell's
   removal condition (stack surfaced in CURRENT SITUATION) is now MET — it is RETIRED from core and
   replaced by the ON THE STACK pointer + mana-discipline fact, so DELETE every guide copy of the
   counter-tell and teach reading ON THE STACK instead. **The cast-happened `hand -> stack` anchor
   (the last core stopgap) is now REMOVED: its wave-8 A/B RAN and the removal is ADOPTED and LIVE in
   the prompt (commit 6d02cb68c — the anchor sentence is GONE; cast-nothing 41.5% vs 41.9%,
   same-name double-casts 13 vs 14, no regression; defect fixed at source 353f1a150 + e11b97a77).**
   Core now carries NO cast-anchor stopgap. Do NOT
   author new guide fallbacks keyed to it; deck109 already DELETED its guide copy with no regression. Never
   duplicate a live core stopgap in a guide, and drop any guide copy of a defense the core has since
   absorbed (plan-from-list copies shed now). The reply-format contract is NOT in the prompt body —
   it routes to the code-appended reply-protocol layer; a guide carries it only while that layer
   lacks it, tagged interim. **Your guide LAYERS ON TOP — never RESTATE any of this; add only
   what is specific to THIS deck.** The general guide states PRINCIPLES; a weak pilot won't apply
   an abstract principle to the card in front of it, so your per-deck job is the concrete
   INSTANTIATION: it says "activate your permanents" — you say "EQUIP Cranial Plating turn 3,
   keep {1} open." Litmus test: if a line you're about to write is already true of EVERY deck,
   the general guide has it — cut it and write the named-card version instead.
   **The same layering applies to the INTERFACE and REPLY-FORMAT contracts.** The reply-format
   contract ("first character of the reply is the option number, bare number on its own line, no
   option text or mana costs before it, then PLAN:"), the list-anchor ("only cards in the
   numbered list are castable NOW; if your intended card is not listed, pick the best listed card
   and say the intended one isn't castable"), and the interface-contract anchor ("when Cast
   <card> is in your list, the engine has already checked — you CAN pay and it IS your main
   phase") are properties of the HARNESS, identical for every deck. They belong in the GENERAL
   prompt, each stated once with a RIGHT/WRONG example pair (the wrong reply-format example being
   an echo of option text — `2. Cast Hellrider {2}{r}{r} (3/3)` — since echoed costs/stats are
   exactly the digits the parser mis-grabs). A per-deck guide carries any of these only as an
   INTERIM stopgap while the general layer lacks it, tagged for removal — seven copies of
   interface rules is the restatement failure this section bans. Flag such lines in your handoff.
   **One caveat to the no-restatement rule:** a corrective fact or behavioral guard the general
   prompt ALREADY carries earns a deck-guide line only when the NEW corpus shows the general
   statement still LOSING to a local pattern — then reinforce it tied to the specific card and the
   exact recurring wrong thought. A recurring misconception the general prompt does NOT yet cover
   may be countered in the deck guide as a stopgap — flag every such line for promotion so it can
   later be removed from every guide at once.

3. **Card text from the ENGINE, not your memory** — for every non-basic card, pull its `text=`
   and `auto=` lines from `bin/Res/sets/primitives/*.txt` (grep by `name=` across `mtg.txt`, and
   also `planeswalkers.txt` / `borderline.txt` if present). **This is what the engine actually
   enforces and shows the AI. Oracle text you remember, the printed cardface, and the engine
   `auto=` script can all DISAGREE — the script wins.** Verify against it the exact numbers you
   will cite AND every MECHANISM claim: who a trigger hits, what a restriction scopes, which
   zone/step it fires in. This applies to your OWN analysis in findings, not only guide sentences:
   an author's rules intuition and the executor's stated beliefs are both unreliable, and adopting
   the pilot's (or your own) mechanical story without reading the `auto=` ships a guide rule
   teaching a false exemption (deck135's findings claimed Diamond Faerie "dodges Lightmine" as a
   flier — Lightmine's `auto=` damages ALL attackers). Litmus: for each "X avoids / only affects
   Y" claim in your draft, point to the `auto=` token that says so. Note hybrid mana ({B/R},
   {U/B}) is castable with EITHER color, so a "gold" card may be mono-color-castable in this deck.
   Use `/usr/bin/grep` — bare `grep` here is a wrapper that can silently drop matches.

4. **The game logs** — the deck's self-play translogs, JSONL, one line per decision qwen made
   piloting THIS deck: `~/.Wagic/ai/gpt/selfplay-runs/<run>/*-<deck>-*.jsonl`. Fields:
   `kind` (ask|priority|attackers|blockers|gameend), `prompt` (the FULL board + numbered legal
   actions qwen actually saw), `reply` (its text — the PLAN/reasoning), `choice` (the index it
   picked), `chosen_text` (the RESOLVED text of the chosen option — use it directly, don't
   re-derive from `choice`), and `turn`, `phase`, `my_life`/`opp_life`, `latency_ms`. (`options`
   is just a count — ignore it.) **This is your primary evidence.**

## Step 1 — Read the deck: archetype, engine, role, redundancy

Answer these before writing a word — they are the guide's spine:

- **Archetype & clock.** Aggro / tempo / midrange / control / combo / prison? How does it
  actually kill? Roughly how many turns is its clock ("kill by turn 4-6")?
- **The win engine / win condition.** Name the specific 1-3 cards that end games and the exact
  interaction that makes them win. A weak pilot will never find these unaided — the guide's spine
  is "set up, protect, and deploy the payoff." Guides teach the DECK'S engine, not general Magic.
- **Role — as a per-turn ROLE CHECK keyed on BOARD-READABLE facts, never on archetype.** State
  the DEFAULT posture in one line, then the condition that flips it, keyed on facts the pilot can
  read off the CURRENT board and life totals — "their creatures are hitting you (your life falling
  while theirs isn't)", "they have flyers you cannot block", "their board is empty" — NEVER on the
  opponent's archetype, which the pilot cannot classify ("faster all-in aggro" is a classification;
  "flyers you cannot block" is a line in its prompt). Give each role its own numbered action line,
  say the check reruns EVERY turn, and close with the invariant that survives the flip ("you still
  cast a creature every turn in BOTH roles") so the switch can't read as permission to go passive.
  **Bind each role's action line to the DECISION KIND that carries it.** A defender role that only
  changes BLOCKING behavior does nothing for a deck whose stabilization actually happens at the CAST
  seam — the pilot already blocks when asked; it never builds the board to block WITH. If the losing
  pattern is "raced while durdling on setup," the role flip must reorder the CAST priority ("cast a
  body before any fetch/dig this turn"), not just add a blocking rule (deck135 wave 5: the defender
  role-check fired the right condition and the pilot blocked correctly when asked, yet still got run
  over because between blocks it spent turns fetching and digging instead of casting the next blocker).
- **Two-gear identity for reactive shells around a NARROW win package.** When few cards actually
  win and the rest is a support shell, the posture is decided less by the matchup than by which
  half of the deck was DRAWN. Write identity as TWO GEARS switched by one fact visible in the
  pilot's own prompt ("is Guttersnipe or Young Pyromancer on your battlefield or in your cast list
  right now? YES → race gear; NO → dig gear"), and write the passive gear as actions per turn
  ("cast draw spells to find the package, spend the mana"), never as waiting. A single-identity
  guide ("you are the beatdown in every matchup") is a lie in half the deck's games and the pilot
  durdles incoherently in exactly those games.
- **Redundancy vs scarcity — MEASURE it.** Count the deck's copies of each effect. A deck with
  12+ removal spells can spend them freely; a deck with one bomb must protect it. Redundancy flips
  the correct advice, so measure before you prescribe.
- **Castability / color dependency.** Count the sources of each color; list which spells are
  STRANDED without each color; verify what each search effect can legally find (Windswept Heath
  cannot fetch an Island; Into the North can). If one color gates half the deck, the guide's
  fetch/land-drop rules must prioritize it explicitly — WITH a countable completion condition
  (see Step 5), because an open-ended "get color X" over-runs into a durdle.
- **The curve / sequencing that matters.** What it wants to do turns 1-2-3-4, plus any hard
  ordering ("payoff before the cheap spells," "land before spells to keep mana open").

## Step 2 — Mine the logs: find the deciding lever (this is where the value is)

**FIRST: the EXECUTABILITY AUDIT.** Before judging any play, establish which decision surfaces
the harness actually gives this deck: tally `kind` counts per game. Deck131's wave-3 corpus had
ZERO opponent-turn `priority` windows in 5 of 6 games — its guide's whole spine ("hold these, act
on the opponent's turn") was structurally impossible, and its 12+ reactive instants were near-dead
cards. If the windows a plan needs rarely arrive, the guide must not build on them — demote those
cards explicitly and route the deck into proactive lines. "Keep mana open" converts directly into
stalled turns when no window ever consumes the mana. For every posture-level rule you draft, ask:
which `kind` of decision, at which phase, would carry it out — and does that decision actually
appear in the logs? A rule the executor cannot execute displaces the rule you should have written.

**SECOND: the HARNESS-DEFECT SWEEP (required — the corpus is now clean enough that engine bugs are
a leading loss source).** Before scoring ANY play as a misplay, run these per-record checks; each
resolves to fact, not guess, from the stored fields:
- **DESYNC (parser mis-grab).** Compare the first standalone integer at the head of the `reply`
  against the recorded `choice` / `chosen_text`. Any record where they differ is a harness defect,
  never a decision — the parser grabs a digit from echoed option text (`{2}{r}{r}`→2, `(3/3)`→3,
  "life 2"→2). Collect them ALL before judging play quality (deck109: 8 desyncs across 5 games, one
  game-losing — exact-lethal burn consumed onto a 1/1 instead of the face).
- **ENGINE NO-OP (chosen-many-times, HAPPENED-never).** The same action selected repeatedly (and
  correctly parsed) while game state never reflects it. Anchors, all in the record: (a) `Mana
  available:` is IDENTICAL before and after the "cast" (a real cast spends mana); (b) the GAME LOG
  never shows the card's `hand -> stack` transition (every real cast prints `hand -> stack` then
  `stack -> battlefield`); (c) the card is still in the hand line next decision. Two+ consecutive
  no-ops of the same pick = engine bug: stop counting it as passivity, find a game where the SAME
  action resolved (the A/B repro), and file it (deck109 "Cast Hellrider" no-op'd 16× then 43× while
  the model did exactly what the guide ordered; louder CAST text is what caused the 43× loop).
- **RESOLVED-VS-ASSERTED (crediting an effect that never resolved).** The cast-happened anchor's
  RESOLUTION-stage twin: before you credit any cast's EFFECT in your review (a drain, a counter, a
  kill, a win MECHANISM), confirm the effect actually resolved in the narration — `stack -> battlefield`
  for a permanent, or the effect's own life/zone events. **`stack -> graveyard` right after an opponent
  `hand -> stack` = the spell was COUNTERED and never resolved**, and the pilot's PLAN will assert the
  intended effect REGARDLESS. A reviewer who trusts `chosen_text` + the plan prose will record a
  phantom "finisher win" and mis-attribute the win mechanism (deck133 wave 7 vs44 seq27: cast Gray
  Merchant, seq28 PLAN asserted "opp to 3, I gain 8" — narration showed `Gray Merchant: hand -> stack`
  → `Counterspell: hand -> stack` → `Gray Merchant: stack -> GRAVEYARD`; the drain never happened, the
  win was Obliterator beats). This changed the reviewer's OWN attribution mid-review — it is not
  optional. (Route the durable fix to the evidence ledger: a `resolved: battlefield|countered|fizzled`
  field on the next record removes the manual narration-scrape.)
- **ATTACKER-DECLARATION DROP (silent under-declaration on the mixed index+name form).** Score the
  "declare EVERY creature that can attack" guardrail by the parsed INTENDED-vs-RECORDED count off the
  `ATTACK:` line, NEVER by window count or `chosen_text` — the reply has THREE forms that fail at
  DIFFERENT layers (deck109 + deck44, 2 seats; corroborated deck131/135/110): **FULL-INDEX (`A1, A2, A3`)**
  parses clean; **FULL-NAME (`Hellrider` / `Ornithopter, Signal Pest`)** UNPARSES (`fallback=unparsed_reply`,
  `choice=-1`) → the baka heuristic attacks with ALL eligible creatures, OUTCOME-NEUTRAL for a pure-aggro
  deck but WRONG for a control/midrange seat where "attack with everything" is a misplay — verify neutrality
  in the NEXT record's `declared attackers:` log, not by the fallback flag (which makes it LOOK like a miss);
  **MIXED (`A1, Rakdos Cackler`)** — the dangerous one — the parser consumes the leading A-indices, IGNORES
  the trailing name token(s), and records a NORMAL `choice=N` with NO `fallback` field, so it looks like a
  clean declaration while it UNDER-declares (deck109: 6 dropped declarations / 5 windows / 3 games,
  combat-confirmed vs140 s7 "attack with both to deal 3" → log `declared attackers: Stromkirk Noble`;
  deck131: 5/18 incl. an unparsed win-game alpha strike). A raw window count or a `chosen_text` read
  SILENTLY passes the mixed-form drop (wave-11 scored "26/26 full declaration" and missed the class). Route
  to the attackers PARSER, NEVER the guide (the model already NAMES every attacker it wants): a name→A-index
  reconcile arm mirroring parseChoice/name-echo — after consuming A-indices, resolve residual NAME tokens
  against the eligible-attacker roster by unique-name match (respecting eligibility) and UNION them in,
  fixing the mixed-form drop AND upgrading the full-name form from "heuristic guess" to the exact named set.
- **NUMERIC-OFFSET TRAP (intent betrayal on menus).** For every NUMERIC menu (X values, quantities,
  damage split), compare the number the PLAN says it wants against `chosen_text`, across ALL such
  decisions. A SYSTEMATIC offset (chosen = intended − 1; or collapse to option 1) is an interface
  trap, not a Magic misjudgment — the fix is menu-space teaching (Step 5) or an upstream menu
  change, never strategy prose. One mismatch is noise; the same offset in 15/17 decisions is the
  deciding lever (deck140 reasoned correct X in its PLAN then replied the value as the index). Two
  more shapes to test when a numeric pick misses but the value/index offset does NOT fit: **(i)
  MENU-ADJACENCY CARRYOVER** — the chosen index equals the PRIOR decision's chosen index, carried onto
  a different menu that armed right after it (deck140 vs110 seq74→75: a bare "2" from the cast menu
  landed on the next X menu → X=5 not 6). Usually low-frequency and immaterial; the fix is a per-ask
  reset / distinct second-menu labeling (engine layer), NOT a guide line — do not over-weight one
  instance.
- **PLAN/CHOICE MISMATCH (intent collapse).** The reply's PLAN names an action that IS a numbered
  option, but `choice` is a different, usually passive option, AND head-int == choice (rule out DESYNC
  first, and rule out plan-diverges-from-legality — here the planned card IS in the list). The model
  betrays its own correct plan at the number, not the parser and not an unaffordable-plan reconcile
  (deck44 e8528 s26/s49: PLAN "I will cast Arcane Denial to counter Wrath of God," options `1. Cast
  Arcane Denial / 2. Cast nothing`, choice = 2; the board was wiped and the game lost). It is only
  weakly guide-mitigable (labeling the option so the passive line reads worse than the action); the
  durable fix is representation/reply-protocol. Attributing it to "passivity" would ship a louder
  act-floor against a model that already stated the correct action — distinguish it in findings.
  **A distinct sibling: REPLY-PROTOCOL INTENT-COLLAPSE (answer-before-reasoning).** The "number-first,
  then PLAN" protocol commits the head token BEFORE the model reasons inside its PLAN, so a model that
  reasons itself to a DIFFERENT option FOR THE SAME WINDOW is already locked to a stale head number
  (deck135 wave 8 vs131 s48: head `4`=Cast nothing, PLAN then concludes "Option 2, cast Icehide Golem,"
  self-corrected next phase). Tell it apart from PLAN/CHOICE MISMATCH: there the head int MATCHES a
  passive option and the reasoning is genuinely for a different action; HERE the reasoning and the head
  are for the SAME window but the head was emitted first and never revised (a "cast X next turn" FUTURE
  intent is neither — it is stale-plan). Route to the REPLY-PROTOCOL layer, never re-word the guide over
  it and never score it against a guide rule; the durable fix is a reply-protocol A/B (accept the LAST
  bare number / move CHOICE after the PLAN / reconcile a trailing named-option index against the head),
  judged by decision-quality not win-rate (symmetric core). **The A/B SHIPPED in wave 10 (answer-after-
  plan: PLAN first, answer LAST on a labeled `CHOICE:/ATTACK:/BLOCKS:` line) and the head-first intent-
  collapse class is VERIFIED DEAD at all 7 seats** — the structural signature is every reply NON-NUMERIC-
  HEADED (no head token to lock) AND `last-bare-int-differs-from-head = 0` (deck135 246/246; deck110 0 true
  head-vs-choice mismatches). Verify a shipped protocol change the same way as a representation fix — the
  WITNESSED defect-class to zero AT THE WITNESSING SEAT, reasoning-vs-outcome split (deck131 wave-10: the
  no-Mountain mulligan closed reasoning 2/2 AND outcome 2/2, up from wave-9's outcome 0/1). **But the
  answer-last protocol has its OWN residual — a THIRD reply-protocol shape (wave-10, decks 140 + 135):
  the trailing PLAN/CHOICE MISMATCH.** The reasoning PRECEDES the answer and reaches the CORRECT decision,
  yet the trailing `CHOICE:N` contradicts the plan's stated conclusion, failing ASYMMETRICALLY toward
  CASTING the offered spell when the plan wanted to HOLD/cast-nothing (deck140 4/4: wasted Rakdos kill
  shots, sweeps into `creatures:0`; indexing verified 1-based, so a MODEL number-emission defect, not an
  off-by-one). Tell it apart: head-first collapse has NO head token; classic PLAN/CHOICE MISMATCH has
  head==choice with reasoning for a different action; the answer-last third shape has CORRECT preceding
  reasoning and only the emitted index wrong. Because the model reliably NAMES its choice in prose, the
  sharper A/B target is to **reconcile the trailing index against the plan's NAMED action** (or require
  echoing the option TEXT, not a bare integer) — this DOMINATES "accept the last bare number," which would
  take the wrong number in all 4 deck140 cases. **Wave-11 PROMOTES the deck110-P3 sub-variant to the MAIN
  shape and shows it is the COMMON ROOT of BOTH mismatch directions — the UNLISTED/UNAFFORDABLE-CARD FIXATION
  (3 witnessing seats: deck140 own, deck135 Q5, deck109; + wave-10 lineage).** At every genuine mismatch the
  plan's NAMED action is a card NOT in the offered options because it is unaffordable this turn (verified
  against `Mana available`), and the trailing index mis-maps in two directions from that one root: OVER-cast
  (wants an unlisted FINISHER on an empty board -> maps onto a listed SWEEPER over Cast-nothing: deck140
  Pyroclasm into `creatures:0`) and UNDER-cast (wants an unlisted SWEEPER/removal at C>=1 -> maps onto
  Cast-nothing over an OFFERED payable removal: deck140 vs109 s9 ignored an offered Cruel Edict at a 5-creature
  board). One flavor emits an OUT-OF-RANGE index (deck109 vs135 seq12: fixates on the unlisted Hellrider,
  `CHOICE: 5` for 4 options -> -1 -> heuristic, the illegal choice faithfully encoded as an imagined 5th
  option). So the reconcile-named-action A/B arm must parse the plan's NAMED action and, **when that action is
  ABSENT from the options, route to the BEST AVAILABLE LISTED option (a payable removal if the plan wanted
  removal; the pass/Cast-nothing if it wanted to hold or deploy an unaffordable finisher) — never an arbitrary
  listed cast — and reject-and-repair an out-of-range index rather than mapping it.** This DOMINATES both
  "accept the last bare number" and a naive named-action match that only handles present options. Companion
  DIAGNOSTIC (deck140 P2, completes the wave-6 hallucinated-board rung): before routing a "sweep into
  `creatures:0`" symptom, READ the plan — a PLAN-vs-CHOICE mismatch is reply-protocol; a PLAN-vs-BOARD mismatch
  (the plan itself believes a creature present, driven by affinity's within-turn create-then-die churn + the
  stale `YOUR PLAN` echo) is REPRESENTATION. **Two metric cautions across the protocol boundary:** (i) the old head-first
  metric silently mis-scores the new shape in BOTH directions — its head parser is now vacuous
  (`non-numeric heads: 215/215`) and its reversal heuristic throws index-offset/mid-plan-aside FALSE
  POSITIVES (deck135: 30 flags, 0 genuine; deck140: 21 flags, ~all false, MISSED all 4 genuine mismatches),
  so a naive cross-arm read (~135 wave-10 vs 120 control) would falsely register WORSENING — before citing
  any diagnostic metric's count across a protocol boundary, confirm the metric's PARSE still matches the
  reply shape, else hand-audit is ground truth; (ii) a fuzzy reversal-flag is a POINTER to read the reply,
  never a defect count. The routing above is fixed DOCTRINE, obeyed cleanly by every witnessing seat. Three
  wave-9 refinements (the non-numeric-head one is now VERIFIED FIXED — deck109's attacker `none` collapse
  went 0-recurrence, 30/30 fully declared): (1) a NON-NUMERIC
  HEAD is a distinct shape — the parser defaults a word head to option 0, which at the attacker seam is
  the WORST option (deck109 vs44 seq7: head = the literal word `none`, PLAN wanted the attack, parsed
  to `no attackers`); add "reject/repair a non-numeric head" to the A/B scope. (2) The verification
  metric SPLITS when a guide/gate fix targets a bad OUTCOME: score the fix by whether the REASONING now
  reaches the right verdict, reported SEPARATELY from the outcome — a fix that corrects the reasoning
  while the outcome stays wrong is VALIDATED (freeze it; the residual is the reply-protocol layer's, not
  the guide's), the mirror of confirm-the-fix-FIRED (deck131 wave 9: the strict mulligan gate made the
  PLAN reason correctly to Mulligan and explicitly reject the bait — reasoning 1/1 — while the stale
  head `1`=Keep cost the game — outcome 0/1; the guide can do no more). (3) The cost depends on whether
  a later window can self-correct: a repeated-priority seat (deck140) mostly self-heals on the next
  window, but a ONE-SHOT window (a mulligan) makes the stale head FINAL — weight the two differently
  when triaging. **Wave-13: the NAME-ECHO protocol SHIPPED as the reconcile-named-action A/B's
  realization (`(name)` echoed on every CHOICE, index-wins on conflict) — and it VALIDATED for the
  PRESENT-option case (982/982 compliant, 0 wrong remaps, every index-vs-echo conflict on an OFFERED
  option resolved correctly) but did NOT close the ABSENT-echo branch, which is now the load-bearing
  completion (3 seats, GAME-LOSING ×2).** Name-echo only helps when the echoed name MATCHES a listed
  option; when the echo names a card in NO option there is no remap target, index-wins is the ONLY
  behavior, and it fires an unrelated in-list card. Two mechanisms produce the absent echo, both pointing
  the same way: (a) the echo names the PARENT action at a target SUB-MENU (deck44 vs135 s24: echo "Cast
  Go for the Throat" vs a 3-target creature list → index 1 = own creature → SELF-DESTROY), and (b) the
  option list SHIFTED after a same-turn cast and the echo names the just-cast card (deck44 vs133 s14: echo
  "Cast Surveilling Sprite" (already cast) → index 1 now GFTT; deck133 vs140 seq45: echo "Gray Merchant"
  not offered → index 1 = guide-forbidden thin-hand Thoughtseize → −2 life, flipped a 1-life adjudication
  loss; deck140 vs44 s40-42: "Play Badlands" fixation after the land was already played → index 1 now a
  *Cast* → dumped a held Rakdos at X=8 and a Wrath into `creatures:0`). Adopted, routed to the
  reply-protocol layer (NEVER the guide — "confirm the card you name appears in the list" was
  present-and-DISOBEYED): **an echo that matches NO current option is a STALENESS signal — fall through to
  the engine heuristic / Cast-nothing (or re-prompt), never the raw index; and re-anchor the indices after
  a same-turn cast.** This completes wave-11 headline 1's "named action absent from options → best-available
  listed option" at the name-echo layer, and its SCORING side is the Mechanism-A/B split in the Step-0
  confirmed-fired terminus (count Mechanism-B executed-card outcomes; a compliance grep hides them).
- **VALIDATION-DROP (chosen recorded, HEURISTIC played — visible only in stderr).** A defect the JSONL
  CANNOT show: a pick that was recorded to `chosen_text` but never executed because it failed
  target-validation and the engine silently deferred to the heuristic AI. When a chosen action is a
  targeted spell/ability and the same decision's board shows NO legal target, cross-reference the run's
  `game-*.stderr` for `fails validation; deferring to heuristic`; those records are harness defects (the
  engine offered a targetless action), not decisions — do NOT score the recorded `chosen_text` as what
  happened. This is a sibling of ENGINE NO-OP: no-op = pick executes nothing repeatedly with mana
  unspent; validation-drop = pick is illegal-target and is replaced by a heuristic move the JSONL never
  records. Digest guidance: when a decision's board has no legal target for the chosen targeted spell,
  flag it and check stderr (deck44 wave 6: `Cast Go for the Throat` logged at 7 decisions across two
  games onto boards with no legal nonartifact creature; the stderr showed the pick dropped to a
  heuristic pass — a JSONL-only reviewer would mis-score these as bad targeting or passivity). This
  also flags an ENGINE contract violation to route upstream: the prompt promises "the game has already
  checked your... targets," so a targeted spell offered with an empty target set is a menu-contract bug.

These feed the ATTRIBUTION taxonomy: do not theorize about qwen's play — read what it did, then
separate **FOUR things a guess would blur: bad play vs. never-had-the-option vs. bad hand
(variance) vs. HARNESS DEFECT** (the pilot chose right and the system did something else — desync,
no-op, misrender). The `prompt`, `reply`, `choice`, and `chosen_text` fields together resolve all
four as fact. **Only bad play is the guide's to fix;** harness defects go on the engine bug ladder
with repro citations (and get at most a Step-5 mitigation); variance and no-option are not yours.
The old three-bucket taxonomy structurally ASSUMES the system executes the model's pick — under it,
deck109's review would have shipped a completely wrong guide (louder Hellrider text, anti-self-
targeting rules) against non-problems.

**Extract, don't eyeball.** Files are large; script a compact digest over the JSONL that prints,
per decision: the `kind`, `turn`, `phase`, life totals, hand, both battlefields, the numbered
options, the `chosen_text`, `latency_ms`, **and the `reply`/PLAN text**, plus the DESYNC flag
above. A ~30-line Python loop does this. Sample ACROSS games and all decision kinds. Two parsing
gotchas that cost reviewers time (`options` is still an int COUNT, so option/attacker lines must be
regexed out of the `prompt`): for "was card X on the BATTLEFIELD at decision D," isolate the `Your
battlefield:` line specifically — splitting on `--- CURRENT SITUATION ---` and searching the remainder
also matches `Your hand:` and the carried PLAN, giving false "on board" reads; and for attacker
participation, extract the engine's `A#.` offered list (see the attackers/blockers bullet), never the
battlefield.

**Read the REPLY text, not just the choice — mine the stated WHY.** The choice stream tells you
WHAT went wrong; the reply tells you WHY, and the why decides which fix works. Grep replies for
recurring false beliefs ("no mana", "cannot cast", "my lands are tapped", "it is the opponent's
turn", a card claimed in the wrong zone, an ability claimed on a card that lacks it) and CHECK
each against the SAME record's printed board — a stated reason that contradicts the prompt's own
board is a systematic misconception, and the fix is a flat corrective FACT tied to the visible
list, not a strategy rule (deck133: 61 replies claimed "no mana" while untapped Swamps sat on the
battlefield). Also hunt refused-legal-actions whose stated reason is a rules error (hybrid "needs a
Swamp"; an offered cast declined as unaffordable — the engine only offers PAYABLE actions), and
plan/number mismatches (reply says "cast X now" while the chosen option is "Cast nothing"). Tally
recurring phrases; one-offs are noise.

**Tally offered-vs-taken per card — required, and count OPTION LINES, not substrings.** A short
script counting, per key card, how often "Cast <card>" (or the key activation) appeared as a
NUMBERED OPTION LINE (`^\d+\.\s`) vs. was chosen, plus whether the card ever reached hand. Count
ONLY the numbered option lines — card names recur in the narration, the carried PLAN, and
battlefield lines, and substring counting silently corrupts the tally in both directions (deck135's
first digest produced garbage "Force 6/48"; the option-line count gave the wave's headline, Diamond
Faerie offered 22 / taken 0). Distinguish "Cast <card>" offers from activation/targeting mentions
of the same card. This is the single most decisive diagnostic and it catches upstream causes too (a
win engine "never drawn" traced to draw spells never cast, not variance). Pair it with the
CAST-NOTHING TALLY (fraction of casting decisions where qwen chose "Cast nothing"). For a menu
trap, add a REALIZED-MAGNITUDE column (offered → taken → parameters correct → resolved).

**But when a rule installs a VALUE FLOOR — restraint (decline the cheap targets, let the weak spell
resolve) — do NOT score it by the raw take-rate or a consecutive-take count: score it by
DECISION-CORRECTNESS.** A value floor makes correct restraint show up as a LOWER take-rate, so a raw
tally penalizes exactly the discipline the guide installed. Classify EACH offered window as
correct-take / correct-let / error, and report n-correct / n-windows; only the ERROR count is the
quality signal, and a falling take-rate under a value floor is consistent with IMPROVED play (deck44
wave 8: the counter seam fell 13/14 -> 9/15 RAW — reads as regression — but was 15/15 CORRECT, the 6
"declines" being correct value-floor let-resolves of land search / a mana rock / a 0/2 Ornithopter).
This is confirm-before-accuse applied to a usage tally, and the mirror of "count OFFER windows, not
just re-picks" — the opposite-direction trap (reading correct restraint as under-use). It is also the
USER EVALUATION DOCTRINE at the metric layer: the unit of analysis is the DECISION, never the rate.

**THE TELL AUDIT (required for any inference rule you intend to teach).** Before writing any rule of
the form "when you see X, Y is true," scan the corpus for EVERY window where X appeared and confirm
Y held in all of them — script it, don't sample. One counterexample means the tell trains a
false-positive the executor will obey off a cliff (it cannot judge exceptions). Record the check in
your findings ("all N counter-offer windows had a live pending spell") so the synthesis agent can
trust the rule stated as an absolute — and absolutes are the only register the executor reliably
executes (deck44 nearly wrote "offered = counterable" from two games; all 12 games' counter-offer
windows had a live `hand -> stack` spell, which is what made it safe to state absolutely).

**By decision kind, what to inspect:**
- **ask** — casting, targeting, land drops, mulligan, modal menus. Watch sequencing/tempo, removal
  fired at the wrong target or too early, whether it targets ITSELF. Check LAND-DROP picks
  explicitly when the deck has a splash color.
- **priority** — activated abilities (equip, tap-for-value, cracking fetches, mana, instants).
  **THE most overlooked seam.** Check whether the deck's engine/payoff ability is EVER activated —
  and, per the executability audit, whether these windows even arrive.
- **attackers / blockers** — is it playing its role? Two bad defaults live here:
  - **Reflexive blocking** (symmetric to "Cast nothing" at the cast seam): offered a block, the
    executor BLOCKS — "there is an attacker, I have a creature" — regardless of role. Audit every
    `blockers` record against the plan: a racing deck that blocks is spending its clock; check what
    the blocker WAS (win condition? token?) and what it blocked (a real threat? a 1/1?). A guide
    that says "attack every turn" but is silent on blocking has NOT set the blocker seam's default —
    silence there is block-by-default (deck131: Guttersnipe, the deck's whole kill, traded to a
    1/1). For any racing deck, blocking IS the non-obvious combat.
  - **Partial answers to bundled asks (under-commitment inside one decision).** Bundled asks let the
    pilot answer with a SUBSET. Count, per attackers record, creatures declared vs listed: repeatedly
    declaring one attacker while three are listed loses damage invisibly — no single record looks
    wrong, and "attack every turn" is technically satisfied by one attacker. The fix is a
    count-and-match procedure ("count the list; your reply names all of them"), not louder posture.
    **The denominator is the engine's OFFERED legal-attacker list (the `A#.` "creatures that can
    attack" lines), NEVER the creatures on your battlefield** — summoning sickness, tapped status, and
    just-created tokens legitimately shrink the legal set, and counting against the battlefield
    manufactures a PHANTOM subset-attack leak (deck131 wave 5: "declared 1 while 2-3 on board" reads
    were 100% participation once counted against the `A#.` list; vs135 T11 the reply even named a
    just-cast summoning-sick creature the engine did not offer — full commitment, not
    under-commitment). Extract the `A#.` list for attacker records the same way you extract numbered
    option lines for casts. A reply naming a creature the engine did NOT offer is the pilot committing
    fully, not under-committing.
  - **A survival rule at the BLOCK seam needs its ATTACK-seam partner — because the block seam may never
    ARRIVE.** Per the executability audit, check whether `blockers` decisions even occur for this deck: a
    deck that taps out attacking every turn generates ZERO blocker windows, so a "chump at ≤8 life" or
    "keep a blocker home" instruction written at the block seam can never fire. When the pilot needs a
    body to survive but keeps tapping out to attack, the survival instruction must live at the ATTACK
    seam instead ("don't attack with the creature you need to block; keep it home when the crackback
    would reach your life"), not the block seam. This is the mirror of the role-check-names-its-seam rule
    (deck110 wave 6: 0 blocker windows in 6 games — every would-be blocker was tapped attacking, so THE
    RACE's block-to-survive clause was structurally dead; vs109 the pilot attacked with a needed blocker
    at 8 life and died to the crackback).
- **target choices** — was the highest-impact target available, and did it take it?

**The single highest-value move: isolate the ONE lever.** Compare the games qwen played WELL against
the ones it LOST or STALLED and find the one repeated difference. Treat WINS as mirrors of losses
(deck135's two wins were the same deck with one variable flipped: Island fetched early → threats
deployed → attacks every turn). Audit wins with a CLOCK lens: compare earliest-possible kill vs
actual kill per game — "won anyway" hides the same leak that loses the close games. **And when the
comparison surfaces a game where the pilot handled the problem situation CORRECTLY, TRANSCRIBE that
winning sequence into the guide as a numbered procedure, in the order the pilot executed it, and
tell the pilot it is its own proven line** ("the exact line that won you the burn matchup last
run"). A sequence the executor has already performed under this prompt stack is stronger evidence
than any theory of what it *should* do: it is known reachable from the options the engine offers,
expressible in moves the pilot takes, and compatible with its habits. Prefer transcription over
invention whenever the corpus contains the win (deck133's DEFENDER block IS its own vs109 win,
numbered 1-4).

**Treat TIMEOUT as its own outcome class — but TRIAGE it before blaming the guide.** A timeout is no
longer automatically passivity. Print final life totals AND check `latency_ms` and record count:
- **Genuine durdle-stall** — life totals frozen for 10-20 turns at normal latencies. The passivity
  failure in pure form; a guide that shortens the game (kill by turn 5-6) is the fix. Wall-clock
  economics make it strategy: each decision costs inference time and games hit the cap at only
  ~35-50 decisions, so every durdle burns clock.
- **Winning-but-slow** — the pilot AHEAD at the cap (opponent at low or negative life). Different
  diagnosis, different owners: opponent lifegain/reset (Elixir of Immortality), inference latency ×
  decision count, engine adjudication gaps — none of which a guide line fixes. Adding anti-passivity
  pressure here REGRESSES the guide against a problem it doesn't have (all three deck133 timeouts
  were the pilot ahead).
- **Latency-starved** — a LOW-turn timeout (game ended mid-progress) with high per-decision latency
  (avg 15s+, spikes to the HTTP timeout) = the run's concurrency artifact, NOT a play defect; report
  it as infra, write nothing.
- **Normalize by DECISION DENSITY before comparing decks.** A control deck generates 3-4× the
  decisions per game of an aggro deck, so identical per-decision latency gives control archetypes
  disproportionate wall-clock exposure — a raw timeout column penalizes the archetype, not the
  piloting.

**Attribute the RESIDUAL — bound what a guide can fix, route the rest upstream.** When the same
deployed guide produces both faithful-execution WINS and losses, diff them: if the wins are the
guide working as written and the losses share a precondition the guide CANNOT create (no threat
drawn, color starved), the residual is DECK CONSTRUCTION, not guide or model. The deliverable is an
explicit upstream flag (manabase skew, threat density) plus guide text that teaches the deck to play
the hand it was actually dealt (the two-gear identity, Step 1) — not louder exhortations to execute a
plan whose pieces aren't there. Fast discriminator: opponent life at the moment of death — losses
ending with the opponent near 20 mean the deck never had a plan running; check the hand/draw stream
before blaming the pilot (deck131: 2 textbook wins; 8 losses ended opp 13-27, traced to 6 creatures
+ 22 lands vs an all-red payoff suite → a construction flag, not stronger beatdown prose).

**Diagnostic patterns to hunt for (weight the guide toward the ones that recur):**
- **Offered-many-times, passed-every-time** — a capability qwen doesn't understand it should use
  (the offered-vs-taken table finds these).
- **Chosen-many-times, HAPPENED-never (engine no-op)** — see the harness-defect sweep above.
- **False-belief vetoes.** A recurring reply-side misconception silently vetoing legal actions.
  Distinguish from strategic passivity — the fixes differ (corrective fact vs. directive).
- **Cast-nothing / durdle rate** and the cards NEVER cast across all games.
- **Hoarding — answers/threats piling up UNUSED in hand** while the opponent stays alive. Watch the
  general prompt's hold-instants prior METASTASIZING: "hold interaction" generalizes in a weak model
  to "hold everything," and a guide's own HOLD line feeds it.
- **Deferred-plan loops / stale-PLAN poisoning.** The PLAN line is carried into the next prompt, so
  "I will attack NEXT turn" gets re-affirmed forever. Hunt for the same "next turn" intention in 3+
  consecutive plans while the action stays untaken, and for plans carrying hallucinated state that
  beats the visible board.
- **Plan-diverges-from-legality-at-the-moment-it-is-written.** The pilot plans an unaffordable cast
  ("PLAN: Cast Gray Merchant immediately" with 4 lands, Merchant costs 5, so it was never offered)
  and then selects "Cast nothing" rather than reconcile the plan with the menu. The fix is the
  list-anchor (Step 5 / general-layer promotion).
- **Hallucinated-board phantom lethal (a distinct plan-failure class).** The PLAN confidently asserts a
  PERMANENT is on its OWN battlefield and computes a lethal from it, then declines development ("Cast
  nothing" over a real creature) because it believes it has ALREADY won — while the visible `Your
  battlefield:` line shows the permanent is NOT in play and it was never cast. Distinguish it from the
  three existing plan failures: it is NOT plan/choice-mismatch (it does not name a listed action it then
  betrays — it names a NON-EXISTENT board), NOT stale-plan deferral (not a re-affirmed future ACTION — a
  false CURRENT state), NOT plan-diverges-from-legality (nothing unaffordable — it just isn't real). The
  deciding fact is FULLY surfaced (the battlefield line) and the general prompt already says "count this
  turn's damage only from creatures actually in the list" and "trust the board over the plan" — both
  PRESENT and losing — so this routes to model-experiments residuals / representation, not a louder
  core/guide line; a guide can add at most a cheap board-read reinforcement ("if the creature isn't
  printed on your battlefield line it is NOT in play") as a stopgap (deck135 wave 6 vs131: PLAN
  "lethal board state (Abominable Treefolk 5/5 and Icehide Golem 2/2 for 7)" carried across 9
  consecutive decisions; Treefolk was offered 0 / cast 0 all game; the pilot declined Ice-Fang Coatl
  twice and lost at T11). SINGLE SEAT this wave → convergence watch.
- **Waste loops** — a repeatable-cost action re-taken every turn for no new effect (re-equipping an
  already-equipped creature; untapping a summoning-sick creature). Burns mana AND clock — and a lone
  state-recognition cue does NOT hold when the engine re-offers the completed action every window
  (deck110: 47 equips, ~12 justified, with `(6/2) (printed 2/2)` printed on the very board). See the
  Step-5 three-part fix pattern.
- **Pursuit over-run** — a "get/prefer X" rule spending turns past its purpose (Step 0 overshoot).
- **Failure to close — end-state sweep.** Opponent life PLATEAUED for many turns while qwen had gas.
- **Missing lethal.** Sum a turn's potential damage; check whether qwen had lethal and didn't take it.
- **Reactive spells fired into nothing** — a counter on an empty stack, a trick on an empty board.
- **Wrong modal/menu picks** — color choices, X values, overload-vs-single (run the numeric-offset
  check before calling these misjudgments).
- **Inconsistency** — the SAME decision made differently across games means qwen has no stable rule.
- **Generic-prior misfires** — a universal prior WRONG for THIS deck. Name it and override it.
- **Cost-line misreads.** Any action whose cost LOOKS bad but is routine (fetch sacrifice, life
  payment, phyrexian) needs "this cost is the point — always pay it."
- **Illegal-plan errors.** Plans built on rules-forbidden actions (attacking with a summoning-sick
  creature "for 9"). A one-line mechanics restatement tied to the specific card earns its place.
- **Subtle leaks in decks qwen already plays "okay"** — burn hoarded vs thrown at the face; holding
  "bad attacks" a go-wide payoff makes correct; not knowing when to stop trading and race.
- **Never-drawn ≠ validated — but distinguish two zero-offer cases.** Zero log evidence about a card
  is VARIANCE, not proof; keep its lines proportional, don't build Rule #1 on an unobserved card, and
  don't cut it either. BUT a card repeatedly in HAND yet never castable (a color/cost the manabase
  rarely meets) is a different animal — a STANDING castability fact, not variance: its hero treatment
  in the guide is dead weight, and the evidence is the pilot's own replies declining it for a REAL
  affordability reason ("I lack white"), not the false-belief delusion the hero rule was written to
  intercept. Demote the spotlight; the correct fix is a deck-construction flag, not a louder cast
  exhortation (deck135 wave 5: Diamond Faerie was hero-worshipped off a wave-4 22/0 refusal, then
  offered 0 times this wave and the one decline was correct — a GWU 5-drop off a 2-Plains base).

**Confirm before you accuse.** When a play looks wrong, read that decision's full `prompt` to check
what was actually legal / on the stack (deck135's Force of Negation LOOKED reflexive until the prompt
showed a real Cruel Edict; deck131's priority passes were correctly reasoned shroud/mana calls; the
Island-flood losses looked like a Mountain-first rule violation until the prompt showed the Mountain
was never in hand before the turn it was played). Also check the translog for harness artifacts (the
sweep above) before attributing a misplay. Never teach against a non-problem, and never list a
"mistake" you didn't observe.

## Step 3 — Decide what goes in vs. what gets cut

Include a line ONLY if it changes a decision qwen will actually face and gets wrong (or would).
Rank candidates by **frequency × cost-to-winning:**
1. The fix for the biggest observed leak / the deciding lever (always first).
2. The corrective FACT for any systematic false belief the replies exposed — but check the CURRENT
   general prompt first: if it already states the fact, include a deck-guide line only when the new
   logs show that statement still losing locally; if it doesn't, carry the fact and flag it for
   promotion upstream.
3. The win engine and how to set it up, protect it, and deploy it.
4. Role + matchup flips as a per-turn board-readable ROLE CHECK (Step 1).
5. Target-priority and removal/sweeper-selection rules.
6. Per-card timing for the cards that decide games; a lethal-math check if the deck closes with reach.
7. Mulligan, in a line or two.

**On a REVISION pass, re-tally each existing rule against the new corpus FIRST** (this is the Step 0
demotion, applied here to the include/exclude decision): a rule the pilot now follows gets compressed
to its one-line load-bearing core keeping its early position; its explanation, cited failure stats,
and DO-NOT reinforcements have done their job and now only dilute the new #1 rule. The freed attention
IS the budget for the new wave's lever.

**Triage model-capability failures by ONE question: can the guide PRE-COMPUTE the answer so the pilot
needs no runtime reasoning?** If yes, it's guide material — do the arithmetic IN the rule ("one
attacker = 1 damage total; Viper 1/3 survives and draws you a card"). If the pilot would still have
to compute/compare/parse at runtime (degenerate output, illegal assignment syntax, arithmetic over a
quoted card it must perform fresh each time), do NOT spend guide lines on it — route it to the
model-experiments residuals list as a litmus case. Guide lines against model incapacity are dead
weight (deck135's solo-attacker Lightmine case was pre-computable → guide; the "** **" degenerate
blockers reply and the illegal double-block were not → residuals).

**CUT:** anything the general prompt already covers; general Magic theory; card-by-card tours of
filler; deep multi-turn combos qwen can't sequence; long matchup trees; hedged "it depends" prose;
rare edge cases; anything you can't turn into a "do/don't with a named card"; and (in revision mode)
inherited rules that never fired in the new corpus. When unsure, CUT — length dilutes the rules that
matter.

## Step 4 — Structure the guide (front-loaded by impact)

qwen weights early text heavily and may not deeply read the tail. Order by what most changes its
play. A reliable skeleton (adapt, don't pad):

1. **Identity + role**, 2-4 lines: archetype, how it wins, the clock, who it's the beatdown against,
   and the posture as a per-turn ROLE CHECK. **Identity prose drives behavior as much as rules do**:
   a tempo-negative self-description ("you are not fast; you win late; stabilize first") reads as
   license to do nothing. Even a slow deck's identity must be written as actions per turn ("you win
   late BY casting a threat every turn and draining in chunks"), never as patience. For a narrow-win
   shell, write the two-gear identity (Step 1).
2. **THE #1 RULE** — the fix for the single biggest leak, hammered in its own visually loud block at
   the very top: WHAT the action is, the exact option text qwen will SEE, its enabling constraint,
   and a one-clause WHY. **State it as a procedure over the visible option list, not a posture.**
   "DEPLOY, DO NOT SIT" loses to the general prompt's hold prior every time; "if any Cast option
   shows a creature, take it" does not. Exhortations always lose to a locally-plausible excuse — use
   a take-the-first-that-appears checklist with a closed exception (see Step 5).
3. **THE ENGINE / how the deck wins** — name the finishers and the exact sequence. Turn-numbered
   deploy scripts beat priority prose: "Turn 2: CAST BLOODGHAST" is executable; "deploy black threats
   mid-game" is not. Write multi-step engines as a NUMBERED mechanical sequence — and prefer the
   pilot's OWN winning line transcribed from the corpus (Step 2) over an invented one.
4. **Standing operating rules / the core action loop.** Where the observed failure lives in one
   specific phase, key the checklist to the visible phase label ("MAIN PHASE 2 CHECKLIST — run it
   every turn after combat") — it outperforms turn-generic principles the model must map onto the
   phase itself. For each decision SEAM with an observed passivity leak (casting, attackers,
   activations), state that seam's own floor as a per-item checkable procedure (Step 5).
5. **Key cards — one imperative line each**, ordered by how often each decides the game. Only cards
   with a non-obvious right line; skip vanilla creatures. Include a HOW-IT-OPERATES line for every
   X-spell and priority-seam ability the pilot provably fumbles — and where the fumble is on a
   MENU-SHAPED decision, TEACH THE MENU, not the card (see Step 5's UI-surface pattern).
6. **A lethal / math check** the pilot can run mechanically, if the deck closes with reach —
   written as a per-turn CHECK bound to a trigger the pilot meets every turn (Step 5).
7. **Combat notes** — for a racing deck this is NOT optional: it carries the blocker-seam default and
   the count-and-match attacker rule (Step 2). "Only if the deck has non-obvious combat" still holds,
   but for a racing deck, blocking IS the non-obvious combat.
8. **MULLIGAN** — the general prompt carries a land-count default, so include a line ONLY to
   SPECIALIZE it ("count Astrolabe as a land"; "ship any hand without a red source") or when the logs
   show the default losing — keyed on a COUNTABLE, never on hand "quality."
9. **DECIDING SITUATIONS** — a tight closing list of the 3-5 situations that actually decided games,
   each written POSITIVELY as "situation the executor will recognize → the correct play," recapping
   rules already stated positively above (the recap re-fires the rule at the tail; see the
   negative-framing ruling in Step 5 for why this is NOT a "DO NOT" list). Placed LAST, it has no
   positive neighbor to suppress.

**Audit for cross-seam leakage before shipping.** Every loud sentence will be pattern-matched at
EVERY decision seam it lexically fits, not just the one you wrote it for: deck44's in-game line "a
hand of counters and removal with NO creatures is LOSING" fired at the MULLIGAN seam and shipped good
openers. Reread each rule asking "which other decision kinds could this sentence match?" — and state
mulligan/blocker/priority rules in their own seam's terms.

Match the deck8 / deck44 example density and length (~one screen, roughly 40-80 lines). Longer is not
better — every extra line dilutes the early ones.

## Step 5 — Word it for the executor (non-negotiable style rules)

- **Imperative and directive.** "EQUIP the Plating." "Cast Wrath when the opponent has 2+ creatures."
  Never "consider," "you might," "weigh whether" — qwen does not act on hedges.

- **Default + narrow, enumerable exceptions** — not open judgment and not a decision tree. If a rule
  needs three conditions, qwen drops two. **Name the exceptions INSIDE your absolutes**: qwen obeys
  hard rules off a cliff, so an ALWAYS/NEVER whose exception you know about must carry it ("burn goes
  FACE — EXCEPT kill Young Pyromancer or Guttersnipe on sight"). Opponent decklists are known — name
  the actual cards it will face.

- **The act-floor comes first, above any hold language — AND it is PER-SEAM, not global.** "Cast
  every turn" does not transfer to the attackers seam: a pilot obeying the cast floor can still
  declare "no attackers" forever (deck135 had the cast floor and lost every passive game at the
  ATTACK seam; wins declared 3-8 attacks, losses/timeouts 0-1). For EACH decision kind where the logs
  show passivity, state that seam's own floor AS A CHECKABLE PROCEDURE the pilot runs over the offered
  list — one yes/no test per listed item with the answer's consequence ("a creature attacks if ANY
  of: no untapped blocker / toughness > biggest blocker's power / it trades up or draws a card"),
  plus a closed exception for the empty choice ("'no attackers' ONLY when every listed attacker dies
  for zero gain"). Posture prose ("be aggressive," "attack every turn") loses to the first plausible
  friction; a per-item test does not. Never write "pass with mana open" or "hold X" as a default
  posture — on a weak executor a hold instruction becomes the whole personality. **Name any mana-dork
  / utility creature the deck runs as an explicit EXCEPTION INSIDE the attack floor** — the free case
  ("no blocker → attack with everyone") will otherwise tap a mana source for a 1-point chip and strip
  a blocker; state where it goes instead ("keep the Druid for mana and blocking; attack with it only
  as your last body when the chip is lethal-relevant"). Minor per instance but pure downside, and the
  floor endorses it by omission (deck135 wave 5: Boreal Druid sent to attack under the free case while
  behind).

- **Prohibitions AND pursuits over-fire — prefer positive sequences, and put a countable release on
  BOTH.** The executor universalizes any DON'T ("DO NOT TAP OUT" → "never spend mana") AND any
  seek/prefer rule ("GET AN ISLAND" → "fetch Islands forever"). Rules:
  1. Say what to DO in what ORDER and let the order imply the caution: "cast your creatures FIRST;
     equip costs only {1}" instead of "don't tap out." Sequencing within a turn must be stated
     ("payoff creature BEFORE the cheap spells") — it is invisible to the executor otherwise.
  2. A brake that must exist needs (a) a SCOPE keyed to something visible on the board, (b) a numeric
     FIRE TRIGGER / release ("hold Blast for a blocker — but when the opponent is at 8 or less, ALWAYS
     fire it at their face"), and (c) a concurrent-action bound ("holding Fatal Push must NEVER stop
     you casting your creature this turn"). A brake without a release becomes permanent. Value framing
     ("premium," "save," "don't waste") trains infinite holding.
  3. **A PURSUIT or standing PREFERENCE needs the same countable completion condition, stated IN the
     rule** ("choose Play Mountain — until you have two Mountains on the battlefield; Islands after
     that"; "ONE Island or one Astrolabe in play is ENOUGH — after that, every fetch takes Forest and
     your turns go to attacking"). Key the terminator to a count the pilot can take from the printed
     battlefield, never to a judgment ("enough red"). An unterminated pursuit becomes the deck's whole
     personality exactly like a brake without a release.
  4. Never write a prohibition whose precondition depends on the pilot's OWN other behavior — it
     self-locks ("NEVER cast Gray Merchant as your only black permanent" + a pilot that never builds a
     board = Merchant never cast). Convert to a numeric GO threshold ("cast Merchant at 4+ devotion").
  5. After drafting, reread every brake AND pursuit and ask: "if the executor applies this sentence to
     EVERY decision for the rest of the game, does the deck still function?" If not, rewrite it.
  Where logs show flash creatures held forever, DE-FLASH them ("treat as a normal main-phase
  creature") — the lost finesse is cheap; the observed holding is fatal.

- **Two imperatives that can BOTH fire on one board state need an explicit precedence or an "either is fine —
  COMMIT" release valve, or the guide itself causes a deliberation/decode LOOP (wave-20, deck49 + deck62, 2-seat).**
  A weak executor handed two rules that both apply and no order between them cannot COMMIT: it deliberates past the
  token cap and unparses, and it will QUOTE both of your sentences while doing it (the Step-0 echo test — a rule the
  executor cites while looping is a guide bug). This is distinct from a mis-scoped SINGLE rule (Method headline 4 of
  wave 19): the fault is the missing tiebreak between two rules, not the scope of one. The tell is a truncation-unparse
  (or a verbatim-repetition loop) whose reasoning oscillates between two of YOUR clauses on an early, low-stakes turn
  (deck49 seq-8 develop seam: "burn goes DEFAULT to the FACE" vs "aim at a CREATURE walling your board", and "deploy a
  body every turn" vs "burn is your clock" — three unparses, one seam, three opponents). Authoring moves:
  1. **If BOTH answers are actually fine, do NOT legislate a universal winner** — that trips the brake-absolutism trap
     (a weak executor over-applies the mandated one). Write "either is fine — pick ONE and COMMIT," add a soft
     board-state lean ("no board yet → deploy the body; a clock already going → send the burn face"), and SOUNDNESS-
     CHECK the lean against what the recovered follow-on turns actually did (deck49: the lean retroactively matched all
     three games' winning follow-ons — a good check that a tiebreak is not just plausible but right).
  2. **If the tension is a loud guide CAUTION vs an ACCURATE prompt annotation, the guide must explicitly CEDE to the
     annotation** — the executor resolves toward the most concrete clause, and an accurate option-line annotation
     LOSES to a loud caution unless you name the exception. Add a decisive exception naming the adjacent case the
     caution must NOT touch (deck62 edit D: "a favorable trade with a REAL creature tagged '(you kill it, your blocker
     lives)' is NOT the forbidden 0/1-engine chump — just make it; do not re-litigate whether a good block counts as
     chumping").
  3. **Tighten an over-broad EXCEPTION against the degenerate board it misfires on** — "a key blocker walling your
     whole board" reads as ANY blocker when your board is a lone attacker; scope it to the state where it is actually
     wrong (a small creature that merely TRADES with one of your attackers is not a "key blocker"). Same family as the
     wave-9 own-targets annotation: name the STATE, not the class.
  Bound the confidence: the CARRIER is a decode loop below the guide layer, so a scope-sharpen REMOVES THE FUEL — it is
  not a claimed fix. Frame the edit like a nudge (mirror of deck62's edit-C mulligan-clause framing), and pair it with
  the decode-side guard (repetition-penalty / max-token / answer-first), which owns the loop tendency itself. Do NOT
  double-layer the tiebreak into the core prompt — dissolve the TRIGGER at the deck-guide layer and measure it first.

- **The ELSE-branch of a priority rule needs its own IMPERATIVE, not a permission — and it must NAME
  the anti-pattern the executor will otherwise invent.** A "deploy a threat before value" rule can be
  fully OBEYED while the leak DISPLACES to the branch where NO threat is castable: there the executor
  reaches for a self-generated shallow default ("hold mana for a creature," "don't waste mana on
  non-threats") and freezes, and a mere PERMISSION clause ("or spend leftover mana on value") does not
  beat a self-generated instinct. Write the else-branch as an else-ORDER, front-loaded, scoped to the
  countable condition that triggers it, with a scope guard so it cannot invert into "value over a
  creature," and QUOTE the wrong thought as a falsity-fact: "no creature in your list? DEVELOP your
  fixing/engine THIS turn — 'Cast nothing / hold mana for a creature' is WRONG when no creature is
  listed; a listed creature still wins point 1" (deck135 wave 8: Rule #1's obeyed deploy-before-value
  displaced the loss to the else-branch — Into the North, the blue fix, was castable and DECLINED 4×
  to "hold mana for a creature" it had not drawn; cast at 5 life, died next turn). This is the wave-5
  DISPLACEMENT lesson and wave-7's "state BOTH branches of a hold as countable conditions together,"
  applied to a priority rule's else. **The else-branch develop-ORDER must gate on the OPTION LIST, not
  the executor's HAND, and must name every SHAPE the develop action can take** (wave 9, deck135, one
  level deeper): the wave-8 else-ORDER validated for its DIRECT case (a directly-castable fix went to
  zero) but the defect PERSISTED when the enabling play was not a directly-castable spell but CRACKING
  AN UNCRACKED FETCH for a missing color — the pilot read a creature in its HAND as "castable" and let
  the guide's own creature-before-fetch ordering VETO the fetch that would enable it, quoting the rule
  while misplaying ("Do not fetch lands if I have a creature to cast... adhering to Rule #1," verbatim
  vs133 s24). The authoring move: (a) re-gate the else-branch on "no creature among your numbered Cast
  OPTIONS" and state flatly that a creature in HAND absent from the options is NOT castable, so the
  fetch that fixes its color IS the creature play (not "fetching instead of a creature"); (b) enumerate
  the develop SHAPES symmetrically — a directly-castable fix AND a fetch-crack for the missing color —
  de-biased from one color (deck135's "almost always the ISLAND / short BLUE" blinded the pilot when the
  missing color was GREEN). **And when the reactive spell in the else-branch carries a fixed card COST,
  write its target exclusion as the ECONOMY rule, not an ONLY-on-[list] positive** (wave 9, deck135 P2,
  completing the wave-6 "a FREE reaction still costs a CARD" line from the target side): a card that
  costs TWO cards to fire only breaks even against a target that costs YOU more than two cards' worth,
  so state "never spend a 2-card answer on a 1-card problem" and NAME each 1-for-1 class the executor
  reads as in-scope (face burn, discard, a single small creature/token spell) — an ONLY-on-[list]
  positive is under-specified because the executor treats any counterable spell as in-scope (deck135
  Forced an Inquisition of Kozilek, a discard the "board-wipe/mass-bounce/kill-only" list did not
  explicitly exclude).

- **NAME the specific anti-pattern the executor will invent — in three more places than the
  else-branch.** "Name the anti-pattern" (above) is a general authoring move, not an else-branch-only
  one; wave 9 found three more shapes where a correct-in-intent line is inverted or bypassed by a
  self-generated phrase, and the fix is always to quote the WRONG thought inline, never to add a
  paragraph or escalate:
  - **An inverted-RATIONALE priority entry.** When a priority entry's justification is "this is the one
    you have NO other answer for," the executor's default instinct ("un-removable + slow = ignorable")
    is the PRECISE inversion of the rule. State the WHY as a flat imperative INSIDE the entry ("a counter
    is your ONLY answer — counter ON SIGHT, there is no 'later'") and attach the growth fact that refutes
    "slow" for the card the executor most under-weights (deck44 wave 9: category #2 "an artifact you
    cannot Go for the Throat" was read as "un-removable + slow = deal with it later," the pilot let Steel
    Overseer — pumps the whole board +1/+1 every turn — resolve and spent the counter on cheap face
    burn, losing). Companion: a let-resolve VALUE FLOOR must NAME the cheap-face-burn class explicitly
    (Galvanic Blast, Searing Spear, Lightning Bolt, whiffing Cruel Edict); an executor that sees
    damage-to-me and cannot find it in the let-resolve list defaults to countering it even where taking
    it and racing is correct — the floor's coverage gap is where the counter leaks.
  - **RESURRECTION in a NEW guise the demoted wording does not literally name.** A false belief demoted
    against a shipped representation can re-enter through a DIFFERENT phrase the representation does not
    surface; the fix is NOT to re-inflate the whole block (the full-strength line was present and still
    failed = evidence against demotion) and NOT to add a paragraph, but to NAME THE SPECIFIC INVENTED
    PHRASE inline, one clause anchored to the tag text (deck140 wave 9: the killed "tapped -> no threat"
    bridge resurrected as "it isn't attacking THIS turn," which the standing "Tapped is NEVER a reason to
    wait" line did not literally name -> add "'it isn't attacking THIS turn' is not a reason to hold — it
    hit you last turn and attacks next turn").
  - **A BOUNDARY VALUE the executor mis-resolves is a WORDING defect, NOT the read-then-disobey wall —
    disambiguate the number, do not escalate.** Distinguish by quoting the pilot's own PLAN: if it states
    the condition as SATISFIED and acts against it -> read-then-disobey wall (do NOT escalate; route to
    representation). If it states the condition as UN-satisfied via a boundary/threshold reading -> the
    condition was never unambiguously TRUE to the executor, so DISAMBIGUATE the literal number (deck140
    wave 9: "your life is 16 or less" at life exactly 16 was computed as "above the 16 threshold" and
    became the pilot's PRIMARY stated reason to hold; the fix is "16 or LOWER - 16 itself counts," and
    the escape — a secondary tapped rationalization — closes with the boundary error).

- **A two-sided GATE (mulligan keep/ship) whose keep-side carries a GUARDRAIL can INVERT — the
  guardrail itself becomes a false KEEP; fix it by STRUCTURAL ORDERING, not more words.** When you add
  a keep-side guardrail to stop a false-mulligan, the executor can read the guardrail as the WHOLE rule
  and drop its precondition — the over-generalization jumps to whichever side carries the salient
  token. Make the gate STRICTLY ORDERED: the disqualifying step (the scarce-resource gate — "no red
  source in the list") decides ON ITS OWN and is un-overridable by any keep-side clause; and no
  keep-side sentence may NAME a card (a payoff) that could stand alone as a keep trigger. State the
  SUBJECT of the keep as the scarce RESOURCE (Mountain-in-list), never the payoff (deck131 wave 8: the
  wave-7 guardrail "a hand with a Mountain and a Young Pyromancer is ALWAYS a keep" was read as the
  whole rule — the pilot dropped "Mountain and" and kept a 0-Mountain no-draw hand on the standalone
  "Young Pyromancer" token). This is the false-belief-bridge lesson (deck140 wave 7) applied to a
  two-sided decision — kill the bridge by ordering, not by more sentences — and it CORRECTS the wave-7
  guidance that ADDED the guardrail: a guardrail is not a one-way fix, so re-check BOTH error directions
  every wave (false-mulligan AND false-keep).

- **When the engine RE-OFFERS a completed action every window (equip, attach, repeat activation), a
  lone state-recognition cue does not hold — write the defense as THREE mandatory parts:** (a) a
  DONE-SIGNAL read off the board line itself as a perception procedure ("two power numbers — like
  '(6/2) (printed 2/2)' — mean the bonus is already on it; the equip is DONE"), (b) a COUNTABLE brake
  keyed to the RESOURCE that bounds legitimacy — one activation per UNATTACHED copy / per unused charge,
  NOT a flat per-turn cap — that caps the damage even when (a) misfires, and (c) a NAMED RELEASE so the
  brake doesn't over-generalize ("equip again only when the equipped creature DIED or a strictly better
  carrier arrived"). A flat "one Equip per turn" mis-fires when the deck runs multiples: with two
  Cranial Platings in play, TWO equips in one turn are correct (attach each), so the flat cap is
  simultaneously violated AND wrong — key it to unattached copies instead (deck110 wave 5). State where
  the saved resource goes ("your remaining mana belongs to creatures and Blasts") — a brake with a
  positive destination out-competes the re-offered line; a bare prohibition just loses to it again.
  (The re-offer means the temptation is re-prompted every priority window, so a cue the model must
  remember loses to an option it can see; the count brake is the only piece robust to the cue being
  misread.) **This three-part guard is a STOPGAP with a KNOWN CEILING.** When the corpus shows the
  pilot correctly READING the done-signal in its reply and STILL taking the re-offered action,
  instruction has failed at full strength — do NOT escalate wording (the exact "make it louder" trap
  the skill warns against). Record it as a representation defect (marker on the option line, or suppress
  the re-offer) and stop (deck110 wave 5 seq84: the pilot read "already attached... waste of mana," then
  equipped anyway — the clean read-then-disobey proof that no wording closes this, the second
  representation-beats-instruction confirmation after the mana line).

- **Repeatable-cost activations: write the rule per PROMPT WINDOW, not per condition.** Name each
  window where the engine will offer the activation and give the action for each ("Upkeep prompt:
  pass. Opponent's turn: pass. Your Main Phase 1 on a turn it attacks: pay it, ONCE."), and state
  explicitly that a passed option RETURNS ("it will still be offered in Main Phase 1"). A
  condition-scoped rule ("only in main phase 1 when attacking") tells the executor when paying is
  right but not what to do at the OTHER windows — it pays early anyway, fearing the window won't come
  back, and can satisfy the rule AND waste mana in the same turn (deck44 paid a Sleep-Cursed untap in
  Upkeep and again in Main 1 of the same turn — 4 mana burned in a race lost at opp 7 life).

- **Teach the mechanic qwen provably lacks — as fact, anchored to the VISIBLE LIST.** The universal
  mechanics/interface facts (offered = payable, lands auto-tap, empty pool is normal, attacking
  doesn't tap lands, X chosen at cast, hybrid mana) are IN the general prompt — do not restate them
  wholesale. Reinforce one in the deck guide only when the new logs show it still losing, and then
  the strongest corrective anchor is the INTERFACE CONTRACT restated as a property of the list tied to
  the named card: "when 'Cast Diamond Faerie' is in your list, the engine has already checked — you
  CAN pay, and it IS your main phase." This converts an unwinnable argument about the world-model ("I
  have no mana," "it's the opponent's turn") into a fact about the thing the pilot is looking at —
  prefer it over re-teaching pools/phases/turn-structure; the pilot doesn't need to understand mana to
  trust the list. Where a delusion resists flat facts, give a LOOK-AND-COUNT perception procedure
  ("count the Mountains NOT marked [tapped] — that number is your available mana"). Include
  state-recognition cues ("(5/1) (printed 1/1)" means the equipment is ALREADY attached).

- **TEACH THE MENU for menu-shaped decisions (X announcement, modal choice, color pick, damage
  order).** A how-it-operates line about the CARD is not enough when the fumble is index-vs-value. The
  pattern, all four parts:
  1. transcribe the literal menu into the guide as the model will see it (`1. X = 0` / `2. X = 1` / …);
  2. state the index/value mapping flatly ("the reply is the OPTION number, NOT the X value; option 1
     is always X = 0");
  3. give a one-step pick procedure over the LIST, phrased against a STABLE PROPERTY of the option,
     never a POSITION ("reply with the number of the line whose value is largest," NOT "reply with the
     LAST line"). The X menu was REVERSED to largest-first this wave, so the largest X is now the FIRST
     line — a positionally-phrased pick ("the LAST line") now names X = 0, the worst option. A
     value-anchored pick survives a menu reordering; a position-anchored one inverts with it.
  4. add a verify step phrased over visible text ("the line you picked must literally read 'X = <the
     biggest number shown>'"), plus one worked example of the exact observed wrong reply.
  (deck140 reasoned correct X in its PLAN then replied the value as if it were the index, collapsing 4
  kill spells to X=0; "pick the LARGEST number offered" was value-space advice for an index-space
  channel. After the reversal shipped, deck140 hit max X in 6/7 — but ONLY because the pilot ignored
  the guide's stale "pick the LAST line"; a guide line the pilot succeeds by disobeying is one tighter
  model away from a blanked kill.)

- **Prefer an engine-SURFACED number over a hand-computed one — and VERIFY its offset.** The engine
  now embeds computed values in option text (Gray Merchant renders `{right now: drains N}`). A number
  the pilot can READ beats a formula it must run, so teach the pilot to read it rather than recompute
  the guide's hand-summed version. BUT before shipping the read, verify the displayed value against the
  RESOLVED result across every instance in the corpus: engine displays can carry a systematic offset
  (Gray Merchant's `drains N` under-counts by 2 because the card itself is not yet on the battlefield
  when the option is built — verified +2 in 3/3 casts). If there is an offset, state the correction as
  a stopgap with a display-fix removal condition, or route the fix to representation. Same audit
  discipline as the index-vs-value tell; the surface is different. **When THIS wave's engine ledger
  adds a NEW surfaced value the guide used to hand-compute (an artifact count, a devotion, a damage
  total), re-run the manual-recompute audit against it IMMEDIATELY — do not wait for a loss.** The
  pilot keeps hand-computing the OLD way until the guide points at the new line, and that
  hand-computation is exactly where the deck-specific miscount lives (deck110 wave 8: the engine added
  `Artifacts in play: you N`, accurate 180/180, and the pilot kept hand-counting artifacts and
  over-counted — folding Glimmervoid + basic lands in, "6 artifacts" vs surfaced `you 3`; the guide
  edit is a pure representation READ-swap). The GM display fix ALSO reached its deletion terminus this
  wave (deck133: `{drains N}` now includes GM's own +2, so the "+2" workaround is DELETED and swept
  from every guide) — the same prefer-surfaced-number rung, closed at both the read-swap and the
  delete-the-stopgap ends.

- **When the PROMPT HIDES a deciding fact, teach the TELL that proves it — but a tell is a LAST
  RESORT, and a fact living only in the log tail is a REPRESENTATION bug, not a tell candidate.** If
  the board representation omits a fact the pilot needs, you can sometimes find a reliable proxy in
  what the pilot DOES see — often the option list itself, since the engine only offers legal actions —
  and write a perception procedure that derives the fact. Verify any such tell with the TELL AUDIT
  (Step 2) before shipping, and mark it as a STOPGAP in the handoff with its deletion trigger (see
  stopgap lifecycle below). CAUTION: a tell that asks the pilot to read the GAME-LOG TAIL is weak —
  the executor skims the tail and fabricates over it (Step 0 representation rung). The canonical
  example, the counter/stack tell ("the counter option appearing IS the proof; the pending spell is
  the last 'hand -> stack' log line"), was present in BOTH core and guide and the pilot STILL
  fabricated a false resolution — so it was fixed the right way, by REPRESENTATION: the engine now
  ships an ON THE STACK section and labeled counter options, the tell is RETIRED, and guides point at
  ON THE STACK. Reach for a tell only when the fact genuinely cannot be surfaced; if it lives in the
  tail, file the representation ask instead.

- **When Step 2 found an ENGINE DEFECT, the guide carries a BEHAVIORAL FALLBACK, never an
  explanation.** The real fix is an engine bug report (filed separately with the repro records). In
  the guide: do NOT mention the engine is buggy, do NOT restate rules to "correct" it, and do NOT
  prohibit the affected action — the executor must keep taking it when it works. Instead give a
  positively-framed fallback keyed to state the pilot can SEE: name the observable check ("a real cast
  shows `hand -> stack` in the GAME LOG; if your pick is still in your hand with your mana unspent…"),
  name the alternative action for THIS decision, and permit a retry NEXT turn with a cap ("one retry
  per turn"). The fallback must degrade gracefully — when the bug is fixed, the line becomes harmless
  (deck109 needed exactly this twice; "Hellrider is broken, don't cast it" loses the games where it
  resolves; silence loses the 43-repick games; an explanation wastes the attention budget).

- **Interface-defect defusal lines are a tracked STOPGAP CLASS with a lifecycle — and the class
  now spans BOTH layers.** A defusal line explains what a misleading label ACTUALLY does and
  redirects ("'Cast Card Normally with Mox Opal' just makes mana with Mox Opal — your creatures
  and spells come first"); it never teaches strategy. In the handoff, list each one as a TRIPLE:
  the line, the platform defect it patches (with the log citation), and the REMOVAL CONDITION
  ("delete when the engine relabels mana-ability activations"). These are pure attention tax once
  the defect is fixed and invisible to a future reviser who doesn't know the defect existed — the
  explicit removal condition is what lets wave N+1 delete them (and the platform fix propagate to
  every guide) in one sweep. The CORE prompt USED to carry a stopgap — the cast-happened `hand -> stack`
  anchor — but it reached its removal condition (2 consecutive clean no-op corpora; defect fixed at
  source 353f1a150 + e11b97a77) and was retired by a deliberate A/B in wave 8 (removal adopted, live in
  commit 6d02cb68c); core now carries NO cast-anchor. The same triple discipline governed it, and its
  clean retirement is the model for retiring a CORE stopgap (an A/B with a control arm, never a silent
  edit) — recorded in the wave's general-strategy rationale. **How a wave-N+1 author knows which
  stopgaps are still needed:** at wave start, for each inherited stopgap (guide OR core), check (a) the
  rerouted engine-ledger in the previous wave's synthesis reply/notes — is the defect's engine fix
  shipped? (b) the NEW corpus — does the defect still manifest (the tell audit re-run; the no-op
  sweep; the menu-offset table)? and (c) the CURRENT core prompt — has it since absorbed the defense?
  A stopgap whose defect is fixed is DELETED; one whose defense moved to core is DROPPED from the
  guide; one whose defect is still live is KEPT and re-cited. **AND one more check for any stopgap
  written against a MENU ORDER, OPTION LABEL, or BOARD-REPRESENTATION surface: pull one fresh prompt
  and confirm the stopgap's INSTRUCTION still MATCHES the current surface.** An engine fix that
  reverses or relabels the surface does not just make a positionally-phrased stopgap inert — it
  INVERTS it to point at the WORST option. A guide line the new corpus shows the pilot SUCCEEDING
  WHILE DISOBEYING is a flip/delete signal, not a validation (the mirror of "grep replies for echoes
  while MISPLAYING"). Concretely for wave 6: the X-menu was REVERSED to largest-first and the X
  reversal SHIPPED — so the X-teaching's positional step ("pick the LAST line") is now INVERTED (it
  names X = 0) and must be FLIPPED to the value-anchored form ("the line whose value is largest," verify
  "X = <biggest number>"), not merely kept; deck140 won 6/7 max-X only because the pilot ignored the
  stale line. The value-anchored verify step survives a reversal; a positional pick does not.

- **Anti-deferral wording.** For the plan-critical action say "NOW / the first turn it appears / in
  THIS reply," and license the imperfect version explicitly ("cast it even if you cannot also equip
  this turn"). The general prompt carries universal anti-deferral, plan/number-binding, and stale-plan
  grounding — add a deck-guide version only when the new logs show the pattern persisting, anchored to
  the deck's own action ("the EQUIP happens THIS turn — if 'Equip' is still listed, it has NOT been
  done").

- **Low branching; use a ranked list or a selection TABLE, not nested IF/THEN.** A priority order (1,
  2, 3) with a one-clause reason each, or a board→card mapping ("one creature → Edict; many small →
  Pyroclasm; anything big → Wrath"). Key unavoidable branches on something qwen can SEE. The table
  pattern is FIELD-PROVEN twice now: deck140's sweeper table was followed correctly in essentially
  every observed sweep across 12 games in wave 4, as in wave 3. **A CONDITIONAL payoff placed at the TOP
  of a ranked/impact list must carry its ELSE inside the entry, or the executor reads the POSITION ("#1
  = my best play, take it") and drops the gating clause.** State the false branch explicitly ("Gray
  Merchant WHEN it wins or saves you; if it does NEITHER, take a creature from lower on the list to
  build devotion") — the ranked position otherwise overrides the condition and the payoff fires
  unconditionally (deck133 wave 6 vs131: Gray Merchant written as impact-list #1 was cast at 2 devotion,
  drain 4, opp at 22, and countered — position beat the gate; stating the else fixed it in the diff).

- **Tie every rule to the in-game surface.** Quote the phrasing the engine actually shows: "When you
  see 'Equip with Cranial Plating [cost {1}]', take it." Name cards EXACTLY as they appear.

- **Concrete, named, numeric — and match the numbers to the OBSERVED pool.** Name cards and costs;
  state thresholds numerically and say what they turn on ("cast at least 3 other black permanents
  first," not "develop your board"). A numeric threshold is only real if it MATCHES the games: before
  shipping any "X+ mana / power Y+" rule, check it against the threats that ACTUALLY appeared and
  decided games. Derive selection rules as a NAMED-CARD table from the observed pool first ("counter
  Master of Etherium, Cranial Plating, Guttersnipe on sight"), then add ONE numeric catch-all keyed on
  visible state for the unnamed remainder ("any creature or equipment while your life is 12 or less").
  Generic-Magic thresholds inherited from prior waves are stale-rule candidates like any other line
  (deck44's inherited "counter the first 4+ mana spell or power-4 creature" matched ZERO of the cards
  that actually killed it — Master of Etherium is {2}{u} printed 0/0, Cranial Plating {2}).

- **Give arithmetic it can run — as a per-turn CHECK bound to a trigger, not a formula to recall.**
  "Damage = unblocked attacker power + (1 per attacker if Hellrider out) + burn to face; if ≥ opponent
  life, attack with everyone and fire all burn." For a deck whose win condition is a sized/announced
  spell (X-burn, drain, alt-cost dump), bind the arithmetic to a trigger the pilot meets EVERY turn
  ("EVERY TURN it is in hand: X = your untapped lands minus 2; if X ≥ the opponent's life, cast it NOW
  — you win this turn"), not a lethal formula the pilot must remember to invoke — posture prose about
  the same card ("this IS your kill") does not produce sized casts (deck140: named "your kill," never
  closed, opp finished 14-20 with the spell in hand).

- **A card with TWO distinct uses needs TWO distinct KEYED TRIGGERS.** The weak executor learns
  exactly the trigger you write and no adjacent use — it fires only the use you spelled out and misses
  the card in its other role. When a single card has two correct uses (a burn spell that is also reach;
  a drain that is also lifegain; a creature that is also a blocker), write a separate keyed trigger for
  each (deck133: Gray Merchant is a lethal finisher AND a lifegain stabilizer; the wave-4 guide gave it
  only the lethal trigger "drain ≥ opp life," so the pilot never fired it as a survival button and died
  at 1 life with it castable — the wave-5 guide adds "your life 10 or less and Gray Merchant castable:
  cast it NOW, the drain is also your lifegain," reusing the core's 8/10-life threshold family).
- **Life-as-resource needs a BUDGET, not just permission.** When the deck pays LIFE across several
  cards (fetches + Thoughtseize + Arena + Yawgmoth), "always pay it" is half the rule — unbudgeted it
  compounds into the pilot racing itself. Sum the deck's self-damage surfaces; if there are 2+, add a
  budget line keyed to the ROLE CHECK, naming which payments continue when defending and which stop
  ("when defending, play a Swamp and a creature instead of Thoughtseize; skip Arena"), each phrased as
  the positive alternative action, never as a list of forbidden cards. **And when two same-role cards
  differ in SELF-COST, give them SEPARATE guide entries so the cost-gated caution attaches only to the
  one that pays it** — a single merged entry lets the executor apply the free card's freedom to the
  costly one (deck133 wave 6: the guide lumped "Thoughtseize/Inquisition" as one discard slot, but
  Thoughtseize costs the caster 2 life and Inquisition is free; the pilot paid Thoughtseize's 2 life into
  a KNOWN-EMPTY hand in a race it lost by 2 life — the budget rule cannot bite while the free and the
  life-costing card share one line). This is "measure redundancy before you prescribe" applied to a
  card's COST, not its count.

- **State the WHY in ONE clause, only for load-bearing rules** — so a rule that looks locally wrong is
  trusted anyway ("attack with the 0-power creature — its battle cry pumps the team").

- **Give explicit PERMISSION where the pilot is wrongly timid and explicit BRAKES where it is wrongly
  reckless** — but the two are not symmetric in risk: an over-fired permission costs a card; an
  over-fired brake or pursuit can cost every turn of the game. This executor's dominant failure mode is
  passivity — when in doubt, default to permission.

- **Override the misleading generic prior BY NAME** ("The general prompt says hold interaction —
  IGNORE that here; this burn is reach, throw it at the face"). The precedence clause only fires when
  the executor RECOGNIZES a conflict, so still name the prior. Don't wait for an observed misfire: scan
  your OWN directives for collisions with general priors and pre-empt them ("cast every copy — the
  'redundant copies' caution does NOT apply to Astrolabe").

- **Turn `#HINT` / `auto=` behaviors into one-liners the pilot can rely on** ("Blightsteel
  auto-attacks once in play — your only job is to CAST it").

### Positive framing default and its two sanctioned exceptions (the negative-framing ruling)

The lesson "negative framing bleeds" is A/B-proven: a clause phrased as "do NOT save them…"
suppressed the positive instruction adjacent to it, and deck131's negatively-framed draw-spell clause
held draw casts to 0.17/game until it was rewritten positively (then 1.0/game, held across 12 games).
**POSITIVE form is the default for all OPERATIVE rules and for the terminal recap.** Exactly two
negative constructs survive, tightly constrained:

1. **Quote-and-forbid a verbatim recurring WRONG THOUGHT — as a FALSITY FACT, not a behavior
   prohibition.** This is an interception keyed to the executor's own phrasing; there is no positive
   paraphrase with the same trigger (the executor must recognize its own sentence), and it is
   load-bearing (the mana interception survived from wave 3 into deck110's turnaround; deck135's
   22/0 Diamond Faerie refusal). KEEP it, under three constraints: (a) it FOLLOWS its positive rule in
   the same block, never leads and never sits adjacent to a positive instruction it could suppress;
   (b) phrase it as a falsity fact about the thought ("the thought 'I have no mana' is ALWAYS FALSE
   when a Cast option is listed"), not as a "do NOT" imperative; (c) at most one such line per rule.
2. **A single bare NEVER/DON'T on the SINGLE worst leak, only where no positive restatement is as
   sharp** — and even then attach its positive redirect in the same sentence. For a CAPABILITY
   restriction (can't block, can't target) state where the capability GOES instead ("Bloodghast
   CANNOT BLOCK — as defender its jobs are devotion, Yawgmoth fodder, Brutality fuel"), never a bare
   prohibition.

Everything else that was a "DO NOT" — including the guide's terminal list — is written POSITIVELY as
situation→play (Step 4 item 9). A terminal recap placed LAST has no positive neighbor to suppress and
serves the same pattern-match-the-replay function the old DO-NOT list did, without the bleed risk. Do
NOT place a negative clause mid-guide beside the positive instruction that carries the plan.

**A bare DON'T aimed at a decision SEAM (blocking, attacking, mulligan) is the single WEAKEST possible
placement** — it is both negative AND, if terminal, out of position, so it "covers" the seam on paper
while contributing nothing. A seam always needs its own POSITIVELY-framed floor at a seam-labeled
block, never a tail-end prohibition (deck109 wave 5: a terminal "Do not block or play a control game —
race" did not fire once; the pilot reflexively blocked 4 times at high life, exactly the "silence at
the blocker seam is block-by-default" pattern the per-seam-floor rule predicts).

- **Short.** It rides in every prompt and competes with the live board for attention. Cut anything
  that fails the "does this change a chosen number?" test.

## Pitfalls

- **Writing for yourself, not qwen** — nuanced, hedged, branch-heavy prose reads well to you and gets
  ignored or misread. Spell out the conclusion.
- **Blaming the pilot for a harness defect** — the corpus is clean enough now that a "misplay" is
  often a desync, an engine no-op, or a menu offset. Run the harness-defect sweep before scoring any
  play; the three-bucket taxonomy assumes an executor the system faithfully obeys, and it no longer
  always does.
- **Diagnosing wording before checking VISIBILITY** — a misplay whose deciding fact never appeared in
  the prompt is a platform defect; rewording the guide re-diagnoses it forever.
- **Fixing last wave's leak so hard you cause this wave's.** A brake written against an over-spend
  becomes the next corpus's paralysis (deck110: "don't tap out" → 0/6, five timeouts); a loud PURSUIT
  becomes the next corpus's durdle (deck135: "GET AN ISLAND" → 16/59 decisions hunting a land).
  Every fix is a new default the executor over-applies — run the Step 5 brake-AND-pursuit audit.
- **Reading a timeout as passivity by reflex** — check who was ahead and the latencies first;
  winning-but-slow and latency-starved timeouts are not guide bugs.
- **Owing the whole record to the guide** — a residual traceable to deck construction gets an upstream
  flag and a play-the-hand-you-drew line, not louder exhortations.
- **Identity prose that licenses passivity** — "you win late / stabilize first" is a rule to the
  executor even though you wrote it as flavor.
- **Circular, self-locking prohibitions** — a DON'T whose escape condition depends on behavior the
  DON'T itself prevents.
- **Cross-seam leakage** — a loud in-game line firing at the mulligan/blocker seam it lexically
  matches.
- **Substring-counting the offered-vs-taken tally** — count numbered option lines only, or the tally
  corrupts in both directions.
- **Card text from memory** instead of the engine `auto=` — you'll teach a rule the engine doesn't
  implement. This includes your OWN causal claims about who a trigger hits.
- **Reading only choices, not replies** — the digest shows the misplay; the PLAN text shows the
  misconception (and, via plan-vs-consumed, the interface trap) that caused it.
- **Compressing a working rule to nothing over successive waves** — keep the load-bearing tokens
  verbatim and record which they are, or wave N+1 erodes the active ingredient.
- **Burying the lede** — a correct rule in paragraph nine is a rule qwen skips.
- **Over-length** — a long guide buries its own most important line.

## Definition of done (checklist)

- [ ] Every card claim (numbers AND mechanism) matches the engine's `text=`/`auto=`, not memory,
      including your own causal claims in findings.
- [ ] The harness-defect sweep (desync, engine no-op, numeric-offset) ran; every loss is attributed to
      one of FOUR buckets, and only bad-play items became guide lines.
- [ ] The #1 rule at the top fixes the single biggest BAD-PLAY leak in THIS deck's logs, stated as a
      procedure over the visible option list (not a posture), in imperative form.
- [ ] Every posture-level rule passed the EXECUTABILITY AUDIT; every passive SEAM has its own per-item
      checkable floor.
- [ ] The `reply` text was mined; every systematic false belief is either already covered by the
      general prompt (reinforced only if still losing) or countered with a corrective FACT — anchored
      to the visible list where possible — flagged for promotion upstream.
- [ ] Every brake AND every pursuit/preference has a visible scope, a countable release/completion
      condition, and survives the "applied to every decision, does the deck still function?" test.
- [ ] Menu-shaped decisions are taught as the MENU (transcribe + index/value mapping + pick-over-list
      + verify), not as card prose; any inference tell passed the TELL AUDIT.
- [ ] The win condition/engine is named and told to be deployed near the top; where the corpus
      contains a winning line, it is transcribed as the pilot's own proven procedure.
- [ ] Role-by-matchup is a per-turn ROLE CHECK on board-readable facts; a narrow-win shell has a
      two-gear identity.
- [ ] A mechanical lethal/math check bound to a per-turn trigger is present if the deck closes with
      reach.
- [ ] The terminal list is POSITIVE situation→play; negative framing appears only as the two
      sanctioned constructs (falsity-fact interception; one bare NEVER on the worst leak with its
      redirect).
- [ ] Generic-prompt priors that misfire OR collide with your directives are overridden by name.
- [ ] Mulligan keyed on a countable. Cross-seam leakage audited.
- [ ] On a revision pass: obeyed rules demoted (load-bearing tokens kept verbatim, general-absorbed
      lines deleted, self-citation stats cut); a turnaround deck revised as a frozen-line diff.
- [ ] Every engine-defect mitigation is a positive behavioral fallback, and every stopgap/defusal/tell
      line is listed in the handoff as (line, defect+citation, removal condition).
- [ ] Every rule is imperative, concrete, low-branching, and executable from the board + action list
      alone. Nothing repeats the general prompt; it fits on roughly one screen (~40-80 lines).

## Output and handoff

Write the finished guide as PLAIN TEXT (it slots into `{STRATEGY_GUIDE}` verbatim). No markdown
headers; short ALL-CAPS section labels read fine. The deck8 / deck44 guides are the format and density
reference.

**Layer-routing discipline for everything you flag upward.** Your handoff routes each item to one
of FOUR layers, and the routing IS the deliverable — argue it explicitly:
- **CORE PROMPT** — deck-agnostic defects/facts, and the burden of proof is ON core: every deck pays
  for each line, so ship core text only when the failure showed up deck-agnostic (or is mechanically
  guaranteed to). Prefer a COMPLETION of an existing core line over new doctrine.
- **PER-DECK** — anything role-, posture-, or named-card-dependent. A racing deck's "creatures don't
  block" promoted to core would misplay every control deck; the deck-agnostic FLOOR goes core, the
  posture stays in the guide.
- **REPLY-PROTOCOL (code-appended)** — anything that must co-evolve with the parser (the reply-format
  contract). It ships from code, next to the parser it serves — not from the prompt body and not from
  seven guide copies.
- **ENGINE/REPRESENTATION** — anywhere a core line is PRESENT but LOSING at distance, or the deciding
  fact is invisible. Instruction cannot substitute for visibility, and repetition is not a mechanism:
  a rule already stated and still bypassed routes to representation (surface the fact, stamp the
  header, label the option), never to a second paragraph of the same instruction.
Two epistemic rules proven this wave: **single-seat evidence cannot cut a core line** — a line your
deck never exercised is untested, not dead; only cross-deck agreement removes core text (removal
experiments are a deliberate A/B, not a wave edit). And **reconcile thresholds by REUSE**: when your
proposal needs a numeric trigger and the core prompt already carries a nearby number (the 8-life
burn-at-face threshold), reuse that number rather than minting a second one — one number reused is
easier for a weak executor than two close ones, and it forces convergent proposals to merge.

The draft is LLM-authored; the user does a Magic-judgment authorship pass and owns the final guide —
surface the card-rules details you verified and flag anything uncertain, plus:
- every corrective-fact line that belongs in the general prompt or board representation. (Status
  after the wave-4 general revision: the list-anchor and the interface-contract timing anchor
  SHIPPED to core — drop guide copies; the reply-format contract was ROUTED to the reply-protocol
  layer, not the prompt — guides carry it interim only until that layer ships.)
- every ENGINE/HARNESS defect found in the sweep, filed SEPARATELY as a bug report with repro records
  (desync citations, no-op A/B pairs, menu-offset tables), NOT smuggled into the guide;
- every stopgap/defusal/tell line as a triple (guide line, defect + log citation, removal condition);
- any residual attributed to DECK CONSTRUCTION, as an explicit decklist flag (manabase skew, threat
  density), set as an expectation, not fixed with guide length;
- any runtime-computation failure routed to the model-experiments residuals track (with its litmus
  record), rather than written as a guide line.

**VALIDATION IS PART OF DONE-NESS, not optional**: a guide can REGRESS a deck (deck110 went from
strongest to 0/6 after a faithful wave-2 revision). Re-run self-play with the guide in place, read the
new logs, and diff behavior against the PREVIOUS wave's corpus — did the plan-critical action start
happening, did cast-nothing / timeout / durdle rates drop, and did a new downstream leak appear? Run
the end-to-end chain re-audit (offered → taken → parameters → resolved). If Rule #1 isn't being taken,
make it louder, shorter, more directive and iterate on that lever before adding anything else; then
return to Step 0. Note: shipping the guide as a Res asset needs `git add -f` (`bin/Res` is gitignored).
End by naming, in your own words, the deck's plan and the single biggest piloting problem the guide is
built to fix.

---

## Appendix — considered and REJECTED (do not re-propose)

- **Citing the observed failure stat inside the guide** ("last run you picked Cast nothing 76 of 101
  times") — self-flagged unvalidated in wave 3; wave 4 (deck133) confirms these describe a pilot that
  no longer exists and should be DELETED on demotion, not kept. Not guide content.
- **Per-deck inline markers and deck-specific rule content in the SKILL** — evidence stays as examples
  that teach the pattern; the skill is craft method, not a rule library.
- **Relaxing the ~40-80-line budget for tell / stopgap / defusal sections** (deck44 self-rejected) —
  the budget pressure is what forces the Step-0 compression that pays for new lines; keep the
  constraint. Stopgaps earn space by displacing a demoted rule, not by growing the guide.
- **A deck-specific doctrine section** (counterspell doctrine, lifegain/unwinnable-matchup section,
  matchup trees) — the generalizable parts live in the method (tell authoring/verification,
  pool-derived tables, residual attribution); a matchup-tree rule violates the CUT list.
- **Carrying interface/format contracts as PER-GUIDE content long-term** — they are general-layer
  furniture; a guide carries them only as a tagged interim stopgap. Seven copies is the restatement
  failure Inputs #2 bans.
- **A bare terminal DO-NOT list** — superseded by the positive situation→play recap (the
  negative-framing ruling). deck110's defense of it is satisfied by the positive form, which keeps the
  last-position + self-contained-redirect properties without the bleed risk.
- **Updating the executability-audit EXAMPLE because deck131's zero-priority-window case was fixed
  engine-side** — the audit's METHOD is unchanged and the example still teaches it; a "(since fixed)"
  footnote suffices. The audit's new frontier (partial answers to bundled asks) is covered in Step 2.
- **deck44's proposed EXTENSION of the counter falsity-fact with a second wrong phrasing ("the spell
  already resolved / moved to the battlefield")** — the whole falsity-fact/tell apparatus was RETIRED
  when the stack was surfaced; extending an obsolete tell is the wrong move. Fixed by representation.
- **Promoting reflexive-high-life-blocking to a sharper standalone CORE sentence** (deck109) — SINGLE
  SEAT this wave; kept per-deck and put on the wave-6 convergence watch. Only cross-deck agreement
  cuts or adds a core line.
- **A core "cast-triggers-payoff" line** (deck131's Guttersnipe/Young-Pyromancer exception to
  "reactions need a target") — per-deck by construction; only deck131 runs such permanents. Revisit if
  future decks add Talrand/Firebrand-Archer/Murmuring-Mystic-class cards.
- **Promoting reflexive-high-life-blocking to a standalone CORE sentence** (deck109 wave-5, now 2 seats
  with deck44 wave-6) — REJECTED AGAIN. Two seats reproduce the leak but BOTH agree the core floor
  already exists and the pilot disobeys at distance; a rule present-and-losing routes to REPRESENTATION
  (surface life-vs-lethal at the blocker seam), not a second core paragraph. Kept per-deck. "Racing
  creatures don't block" would misplay every control deck — never a core line.
- **Adding "a FREE reaction still costs a CARD" to core** (deck135 wave-6, Force of Negation over-fired
  5/5) — SINGLE SEAT. Per-deck (scope the reaction by card cost) + convergence watch; promote only if a
  second free-alt-cost reaction deck (Force of Will / Solitude class) reproduces it.
- **Overriding the burn-is-clock core prior for a scarce X-burst finisher in CORE** (deck140 Rakdos's
  Return) — SINGLE SEAT, per-deck by construction (the prior is correct for real burn decks); handled by
  a by-name in-guide override. Watch only if a second deck runs a scarce X-reach finisher.
- **A core-level "guide keep/ship rule OVERRIDES the mulligan land-count default" reinforcement**
  (deck131 wave-6, general keep-default beat the guide's red-source override 2/3) — SINGLE SEAT.
  Per-deck (promoted by-name-override mulligan block) + watch; the core already says "your strategy
  guide may override this."
- **Promoting reflexive/mis-framed blocking to a CORE sentence** (now 3 guises across 5 seats: high-life
  chump, mid-life block-with-the-only-reach-creature, block-a-punisher) — REJECTED AGAIN. Every
  witnessing seat agrees the core floor exists and the pilot disobeys at distance → REPRESENTATION
  (surface life-vs-lethal + the punisher rider at the blocker seam), never a core paragraph. Per-deck
  floors interim.
- **Tightening the phantom-lethal / plan-from-list core lines for "fixated on an unaffordable bomb →
  decline a listed body"** (deck109 wave 7, 1 instance, not outcome-changing) — HELD to watch; the core
  already forbids declining a listed action, and deck135's stale-plan-around-uncastable-Treefolk is a
  DISTINCT shape (never offered), so it is not 2 seats of the same pattern. Promote only on a 2nd seat.
- **Adding "a tapped attacker is still a threat" / "shroud does not stop non-targeted sweeps" to the
  core mechanics-facts block** (deck140 wave 7) — SINGLE SEAT (only deck140 exercised sweepers). WATCH;
  the preferred end-state is engine ledger #3 ("creatures: N" surface), which dissolves both without
  core prose. Promote only if a 2nd control/removal deck reproduces the hold.
- **Retiring the `hand -> stack` cast-happened anchor as a WAVE EDIT** — REJECTED as a wave edit;
  it was retired the RIGHT way, by a deliberate wave-8 A/B with a control arm (removal adopted, live in
  commit 6d02cb68c; no regression). Removal of a deck-agnostic core line is always an A/B, never a
  silent wave edit — this is the model instance.
- **A `damage >= toughness = dead` core rules-fact** (deck131 wave 8, Pyroclasm-on-stack: pilot
  declined a counter believing its 2/2 Guttersnipe survives 2 damage) — SINGLE SEAT. A clean member of
  the core mechanics-facts class (X-spells-read-as-0, attacking-taps-lands), but single-seat can't add a
  core line. WATCH; handled per-deck via a falsity-fact tied to Guttersnipe. Promote on a 2nd seat
  showing "my N-toughness creature survives N damage."
- **An "offer-side no-legal-target suppression" ENGINE item** (deck44 wave 8) — REJECTED as a
  MISDIAGNOSIS (verified by probe post-review): both cited GFTT records had the opponent at
  `(creatures: 0)` but deck44's OWN nonartifact Faerie on board, and "destroy target nonartifact
  creature" legally targets your own creature (601.2c) — a legal target existed, the offer was correct.
  A dedicated Baka census probe confirmed the genuinely-targetless case IS already suppressed
  (6994/6994 castable=0). Do NOT re-propose it. The residual is a REPRESENTATION nicety (annotate the
  option line when the only legal targets are the caster's own permanents — see ledger), and deck44's
  guide fallback (name an enemy target) is valuable as STRATEGY, not a contract workaround.
- **A reply-protocol intent-collapse CORE-PROMPT edit** (deck135 wave 8, answer-before-reasoning) —
  the item is real but its owning layer is the code-appended REPLY PROTOCOL, not the prompt body; and
  it is a single instance. WATCH via a reply-protocol A/B (judged by decision-quality, symmetric core),
  not a prompt-body sentence. Adopted only as a Step-2 diagnostic distinction (vs PLAN/CHOICE MISMATCH).
- **Adding shroud-vs-non-targeted / tapped-attacker-still-a-threat to the core mechanics block**
  (deck140 wave 8) — REJECTED AGAIN. Tapped-attacker is now CARRIED BY REPRESENTATION (the shipped
  `[tapped - untaps and can attack next turn]` tag fired 86× and the pilot obeyed it), dissolving the
  core-prose case. Shroud-vs-non-targeted is single-seat and NOT carried by any shipped representation
  (a count says nothing about targeting) — a genuine core mechanics-fact candidate IF a 2nd removal deck
  reproduces it; WATCH.
- **A `colored mana pays a generic cost` / `offered = payable` CORE line** (deck135 wave 9, vs140 s24:
  pilot declined an OFFERED `Cast Astrolabe with cost {1}` believing {G} cannot pay a generic {1}) —
  SINGLE SEAT. Same FAMILY as the core's existing "Mana in your pool: (none) read as I cannot cast"
  correction (a different instance), and the core ALREADY carries "hybrid pays either color" +
  "generic can be paid by any color" + "every listed choice is legal AND PAYABLE." Guide-patched locally
  (Astrolabe {1} paid by green). PARKED as a core candidate: promote to a core line ("any spell shown in
  your options is payable right now; your colored mana pays generic costs") only if a 2nd deck shows a
  pilot declining an OFFERED cast on a "colored can't pay generic" or "I can't afford the listed option"
  belief. One seat cannot cut core.
- **A reply-protocol intent-collapse CORE-PROMPT edit** (now decks 131/135/140/109, 2 waves) — STILL
  NOT a prompt-body edit: the owning layer is the code-appended REPLY PROTOCOL, not the prompt. What
  CHANGED wave 9: it is now MULTI-SEAT and the durable fix (the reply-protocol A/B) is DUE, escalated
  from watch — but the resolution is a harness A/B judged by decision-quality on the symmetric core,
  never a core sentence.
- **A combined-lethal core/guide clause** (deck133 wave 9, vs135 s28: pilot correctly cast Gray
  Merchant off-condition because drain 10 + a 7-power swing = combined lethal, bridging the guide's
  "N >= opponent's life" win condition) — REJECTED as an edit at BOTH layers: the pilot bridged it
  WITHOUT being told, and a combined-lethal clause risks premature small-GM casts the guide already
  warns against as counter-bait. WATCH only; route: none.
- **Demoting an UNTESTED guardrail into the pause** (deck109 wave 9, the NEVER-block-Obliterator
  absolute at 6/6) — REJECTED: demotion requires the class OBSERVED OBEYED, not merely absent, and a
  closing wave with no next wave freezes untested guardrails against known game-losers (folded into the
  Step-0 demotion terminus as the pause-aware precondition).

---

## Wave-24 changelog (each reviewer proposal: adopted / adapted / rejected + why)

- **deck27 P — PROMOTE decode-collapse-vs-commit-lock to a durable classification rung (2 corpora, 2 members, both
  harmful) + a NEW-candidate note that answer-first is unreliable for arithmetic-heavy decision kinds.** ADOPTED as
  Method headline 1. The wave-23 rung was single-seat; deck27 vs137 seq5 (bottom-phase, NON-truncated, harmful)
  independently re-hit the OTHER member with a harmful instance, meeting the promotion bar. The sharpening (the freeze
  is not only a truncation artifact → the parser candidate must key on "natural stop → prefer the LAST coded line,"
  gated off truncated replies) is ADOPTED as a design question routed to the between-wave ledger, NOT a settled change
  (it must reconcile with INDEX-WINS + headline 2's protocol-leak hazard). The arithmetic-snap note folded into headline
  5's guide-lane-vs-parser-lane split. Confirmations (record-swing-is-not-evidence DOWN direction; layer-first routing;
  off-case unexercised 5th corpus): RECORDED, no text change.
- **deck133 P — reinforcement of ACCEPTANCE-TEST discipline (name the should-still-fire negative case; a fallback's
  classification is a hypothesis to confirm against the raw reply).** ADOPTED as corroboration, no new rung — folded
  into headline 2 (the retracted_choice "hotfix fired ≠ hotfix was right" residual-FP finding). The Yawgmoth
  SacrificeCost acceptance (37 offers, all creatures≥2, proliferate mode still offered at creatures:1) is the clean
  positive witness that an acceptance test must also name the case that SHOULD still fire.
- **deck102 P1 — a THIRD commit-lock sub-shape (example-seeded / stale-plan-seeded OUT-OF-RANGE line-1 index).**
  ADOPTED into Method headline 2 (the protocol-example leak), now 2-seat convergent with deck133's retracted_choice FP.
  Both trace to the reply-protocol's quoted `e.g. "CHOICE: 2 (Cast Fatal Push)"`; the single ledger item (line-anchored
  indices, exclude quoted/example echoes, index-agnostic/rotated example, plan-hygiene) owns both. deck102 P2 (the
  mulligan HAND-READ diff extending method-2 to the pregame hand): ADOPTED into Method headline 5(a).
- **deck131 P — a THIRD decision-scoring diff: answer-vs-conclusion fidelity (leading action line == the reply's own
  conclusion).** ADOPTED as Method headline 3 (tagged single-seat, GAME-DECIDING; promotion threshold = a 2nd seat
  with a first-line-vs-conclusion contradiction). Distinct from the wave-23 diffs (execution + decision-input): it
  separates a MODEL-COMPREHENSION miss from a HARNESS-EXTRACTION miss, which look identical from the win column but
  route to opposite lanes. The new-phase-zero-coverage corollary (audit the guide before scoring a decision in a
  brand-new phase as a pilot error) folded into headline 5(b).
- **deck59 P — a SIXTH rule-3 idiosyncrasy face: the SYMMETRIC EFFECT ASYMMETRIC IN YOUR FAVOR via recursion (the
  inverse of the fourth face).** ADOPTED as Method headline 4 + the rule-3 body addition (tagged single-seat; promotion
  = a second recursion/undying/symmetric-payoff deck reproducing the under-fire). The confirm (belief-that-VETOES now a
  THIRD seat, with a mulligan-quality cost) RECORDED into headline 4 + the rule-3 belief-face note.
- **deck137 P1 — the crutch-lifecycle CLOSED-BY-FACT witness (R-ADV-FACE fifth face resolved at the representation
  layer, crutch demoted).** ADOPTED as Method headline 5(c) + the wave-24 status note on the fifth-face body: mark the
  fifth-face WATCH CLOSED-BY-FACT (a representation bug, not a durable belief), NOT promoted. deck137 P2 (the spiral
  RELOCATES to the next unfloored seam; audit ALL high-stakes seams for a floor): ADOPTED as the headline-5 corollary
  (single-seat WATCH; promote if a second deck's worst spiral is a lethal-block after its attackers seam was floored).
- **deck109 P — PASS; the 2-corpora belief-correcting-crutch clock rung worked as intended (kept the seat one more
  cycle rather than declaring convergence on corpus 1).** No change. The OBSERVATION (a mulligan/marginal-hand surface
  can go a full corpus unexercised on draw variance → needs an explicit "surface-unexercised" carry-forward, not a
  silent "confirmed clean"): ADOPTED as the wave-24 harness-ledger carry-forward note (iv).
- **Core / general-prompt: PASS (19th consecutive), 7-of-7 seats.** No general-prompt line added or cut. The one item
  touching core-appended text (the protocol-example leak, headline 2) is a REPLY-PROTOCOL layer change routed to the
  engine ledger, NOT core prose — the distinction (code-appended reply protocol vs the shared core wrapper) is
  documented so the example fix is not mistaken for a core-prose edit.

## Wave-23 changelog (each reviewer proposal: adopted / adapted / rejected + why)

- **deck133 P — FACT-vs-JUDGMENT surface split on the crutch-lifecycle (classify the engine surface before
  applying the terminus; a JUDGMENT surface invites a deck-specific ADD, not a DEMOTE).** ADOPTED as the (a) leg of
  the Wave-23 refinement to the crutch-lifecycle terminus (Step 0) and Method headline 1. Single-seat origin
  (deck133 Bloodghast-fodder ADD) but part of the 3-seat convergence on the same rung; the underlying discipline was
  implicit in method-1a — this names the split so the ADD case isn't mistaken for demotion-debt.
- **deck109 P1 — information-supplying vs belief-correcting crutch, with a 2-CORPORA retirement bar for the latter.**
  ADOPTED as the (b) leg of the Wave-23 refinement and Method headline 1. Strong single-witness (the Pillar-cost
  retirement REGRESSED round-2; the hybrid-mana retirement HELD — same seat/corpus, opposite outcomes, isolating the
  variable). deck109 P2 (attacker-scoring generalizes to the FIX side): RECORDED, no new text — a clean confirmation
  that the wave-22 declared-vs-recorded instrument validated the fix (26/26) that closed the defect it found. P3:
  PASS on new doctrine and core.
- **deck102 P — oracle-truth vs engine-behavior-narration split when demoting (a fork on the wave-22 now-FALSE-clause
  audit).** ADOPTED as the (c) leg of the Wave-23 refinement and Method headline 1. Caught a real fork (R-DFC-BACKFACE-
  RESOLVES-FRONT made the Lantern oracle clause suspect; kept generic, not rewritten from one game). deck102's second
  proposal (fixing the representation defect can unblock the keystone in the RANDOM pool too): ADOPTED as Method
  headline 5 + a one-line addition to the probe-deck rung — validated live (0/18 → cast → steal fired → won, no probe).
- **deck131 P — score a "safe swing" rationale against the ACTUAL `[tapped]` tags, not the model's claim; attacker-
  scoring needs a SECOND (decision-input-fidelity) diff.** ADOPTED as Method headline 2 (tagged single-seat, GAME-
  DECIDING; promotion threshold = a 2nd seat with a per-creature tapped-state misread). Distinct from wave-22
  headline-4 (that catches UNDER-recording; this catches a faithfully-recorded decision resting on a false board
  read). Guide-lane corollary (audit "protect the keystone permanent" rules for one-sided coverage) folded in.
- **deck137 P — Step 0-bis rule 3 FIFTH idiosyncrasy face: the alternative-cost dual-option card whose alt-cost
  option carries the OTHER face's (P/T).** ADOPTED as a tagged single-seat rule-3 face (Method headline 4 + the
  rule-3 body addition). Confirmed wrong-outcome repro (deck131 s15 self-bounce). The convoke-tax sibling: RECORDED
  as a not-yet-a-rung observation (single-seat, likely engine-side) — promote to a "trust the offered alternative-cost
  option" rung alongside TRUST-THE-RENDERED-NUMBER only if a second convoke deck reproduces the tax.
- **deck27 P — separate DECODE-COLLAPSE from COMMIT-LOCK when classifying unparsed/truncated fallbacks (the reviewer
  test: does the failed reply contain a coded line?).** ADOPTED as Method headline 3 (reviewer routing aid). Single-
  seat producer, but a general classification aid that prevents mis-routing sampler faults to a parser change; it
  also frames the pilot verdict (rep_penalty=1.05 did not fix the deck27 collapse → sampler ledger, not guide/parser).
  deck27's other items: confirmations of existing rungs (record-swing-is-not-evidence UP-direction; fix-at-the-
  parser-commit-layer; write-the-off-case) — RECORDED, no text change. Core: PASS.
- **deck140 P — PASS (no skill change).** A textbook end-to-end validation of the existing crutch-lifecycle demotion
  doctrine (interim forced-discard crutch → engine absorbed the durable fix → demoted to a marker-anchored nod). The
  observation that the cleanest demotions are the ones the PREDECESSOR review named the trigger for in advance is
  already produced by the existing route-durable-fixes-to-the-ledger + mark-interim guidance; RECORDED, no new
  instruction.
- **Core (general prompt): PASS — 18th consecutive.** All seven seats routed core to PASS. No line added or cut.
  The repetition_penalty=1.05 pilot verdict is a decode/sampler synthesis output, not skill text (see the wave-23
  synthesis-notes.md pilot section).

## Wave-22 changelog (each reviewer proposal: adopted / adapted / rejected + why)

- **deck102 P1 + deck44 P1 + deck109 P2 + deck135 (crutch-pacing) — the CRUTCH LIFECYCLE matures across a 4-SEAT
  convergence; the demotion/retire terminus is confirmed at every representation-fix shape and gains four sharpenings.**
  ADOPTED as the wave's strongest convergent theme (Method headline 1). deck102: DELETE now-FALSE clauses (a crutch
  narrating the ABSENCE of a signal the fix now provides is a lie, not redundancy). deck44: SITUATIONS-crutch for an
  engine-carried non-deciding case → DELETE; mechanics-crutch → DEMOTE to anchor (distinct homes). deck109: the
  CLOSED-criteria generalize to any representation crutch (cost/mana/hybrid/reveal/trade). deck135: retirement PACING
  (wait n=2; separate the still-true half). Guide-AUTHORING method, not core — extends the wave-21 second-half terminus.
- **deck140 rung 1 + rung 2 + deck102 (probe-sequence) — ROTATION-IN AGAINST A CHANGED POOL = NEW-LEAK HUNT, plus a
  demotion-debt audit on frozen guides, plus sequence the probe behind a path-blocking representation defect.** ADOPTED
  (Method headline 2). Strong single-seat (deck140) but validated by the rotated-in cohort's structure — all three
  rotated-in seats found their real work at new surfaces (140 forced-discard; 44/109 crutch-demotions the frozen guides
  had accrued). A rotation-discipline addition; routes as an adopt-if-convergent targeting instruction, which the wave's
  structure supplies.
- **deck62 (PROPOSED RUNG) — VALIDATE A FALSE-POSITIVE-SUPPRESSION FIX ON THE SHAPE STILL BEING PRODUCED, not its
  absence; shape-present-and-passing STRONG, shape-absent WEAK.** ADOPTED (Method headline 3), the back-half of wave-21's
  headline-1 (which found the over-fire; this validates its FIX). Reviewer-instrument rung, clean worked example (N9
  shape re-exposed 27× / 0 fires). Corollaries: grep the phrase FAMILY not the token; witness = signature-match seat,
  held only until confirmed. Not core; core PASS holds.
- **deck109 P1 — SCORE ATTACKER DECLARATIONS BY DECLARED-SET-vs-RECORDED-SET FOR EVERY WINDOW, regardless of form; the
  pure-INDEX form is NOT safe-by-construction.** ADOPTED (Method headline 4) as a sharpening of the wave-13 Ledger-A
  attacker-scoring instrument (falsifies its premise). The underlying MECHANISM (prose-salvage overriding a valid coded
  ATTACK line, vs62 s21) is an ENGINE integration-order candidate → ledger (FLAG, code-verify), NOT a guide/skill line.
- **deck27 (WATCH) — ANSWER-FIRST COMMIT-LOCK: front-load the correct SNAP heuristic in the guide for decision kinds
  where the snap is easy to get wrong; do not rely on last-wins (truncation may pre-empt the correction).** ADOPTED as a
  guide-authoring rung (Method headline 5). 1st HARMFUL instance (vs109 s15) vs benign rescues elsewhere; deck27 applied
  the guide-lane fix (free-kills-first allocation). The PARSER/decode side is a cross-seat WATCH — promote to a parser
  change only on a 2nd HARMFUL seat; the decode-time repetition guard (repetition_penalty pilot) is the structural close.
- **deck44 P2 — state the PRINCIPLE, not just the triggering CASE, for a "let it resolve" rule (the Cruel-Edict muddle).**
  CONSIDER (single-seat). Recorded as a concrete instance of the existing Step-5 word-for-the-executor guidance (the model
  pattern-matches a guide line as a rule, so an incomplete rule produces a muddle); promote to a Step-5 example on a 2nd
  witness of the "case stated, complement muddled" shape.
- **deck135 skill.md + deck62 (existing rungs re-confirmed) — CONFIRMED, no new text:** deck135 PASS (only the
  crutch-pacing qualifier, folded into headline 1). deck62 re-confirmed answer-first validation discipline (re-point
  parse-risk → latency; CHECK-FIRST==FINAL) and discharge-of-named-keep-reason (applied in its own rotation verdict) —
  worked examples of wave-21 rungs, no new skill text.
- **REJECTED / not promoted:** no single-seat proposal added or cut a general-PROMPT (core) line (core = 17th PASS).
  deck109's commit-and-stop anti-spiral nudge for the general prompt was NOT promoted to core — single-seat, 6 of 7 seats
  route the spiral to the decode sampler, direct precedent (wave-21 rejected the identical deck49 G-49a), and it composes
  BADLY with last-well-formed-CHOICE-wins (a "never re-open a coded decision" line fights the harness's deliberate
  mind-change affordance — a composition seam); the sanctioned fix is the repetition_penalty pilot, which reaches the
  decode mechanism prose cannot. The forced-self-discard framing (deck140, filed as core) belongs at the REPRESENTATION
  layer (same home as reveal-framing) → ledger, not core prose. No engine/harness/card item became prompt text.
- **Core (general prompt):** PASS, 17th consecutive. No `general-strategy.txt` produced (no change to verify). The
  skill's METHOD gained rungs via the wave-22 synthesis-notes header (crutch-lifecycle 4-seat maturation; rotation-in-as-
  new-leak-hunt; FP-suppression-fix validation; attacker-declared-vs-recorded scoring; answer-first commit-lock
  guide-authoring rung) — guide-authoring/reviewer-instrument method, not the core prompt, and do not reset the core
  PASS count.

## Wave-21 changelog (each reviewer proposal: adopted / adapted / rejected + why)

- **deck62 #1 + deck135 CONFIRMS + deck49 NEW rung — a NEW PARSER SAFETY ROUTE must be audited ADVERSARIALLY for
  OVER-FIRE; a fallback logged against a reply with a correct, in-range, UNCONTESTED first-line coded answer is a
  FALSE POSITIVE (HARNESS-owned), and the witness seat is the one whose signature matches the route trigger.** ADOPTED
  as the wave's core instrument addition (Method headline 1). 3-seat (deck62 owns 3 of 5 corpus retracted_choice,
  deck135 1, deck49 the stale_echo analogue), with deck35 vs62 s18 supplying the ONE correct fire that fixes the
  discriminating shape (retraction must target the CHOICE, or a contradictory second coded line must exist). Not a
  core-PROMPT line — a reviewer-instrument + harness rung, so core PASS holds. Route: ledger HARNESS-N9.
- **deck49 + deck62 + deck35 + deck27 — ANSWER-FIRST VALIDATION DISCIPLINE: "0 unparsed" is not sufficient; (a)
  length-check the seam, (b) check FIRST==FINAL and confirm last-wins took the reasoned line, (c) re-point the
  instrument parse-risk→token/latency cost, (d) watch the truncation-between-snap-and-correction exposure.** ADOPTED
  (Method headline 2), strong multi-seat. deck49's length-check discriminator (co-firing-fork verification = seam reply
  LENGTH, post-answer-first) is the reusable audit method; deck62's snap-then-revise + last-wins-is-quality-preserving
  is its quality face; deck35's re-point-the-instrument (parse-risk → cost) and deck27's harm-defusal are its cost/decode
  face. deck49 HARNESS-49a (residual spiral now trips stale_echo, not truncation) is the new-consequence-class evidence.
- **deck14 P1 + deck102 P1 — the CRUTCH LIFECYCLE closes: RETIRE (rule gone) or DEMOTE (rule intact, engine carries it)
  an interim guide crutch once its durable representation fix ships and is confirmed fired+obeyed.** ADOPTED (Method
  headline 3), 2-seat. This EXTENDS the Step-0 demotion terminus (the confirmed-fired-representation terminus) with its
  explicit second half; it is guide-AUTHORING method (not core). deck14 shipped the DEMOTION diff (bounce-on-stack
  procedure → 3 lines); deck102 shipped the RETIRE (stale Thoughtseize crutch replaced with lean strategic content). A
  future editor with full Step-0 context may inline the retire/demote distinction next to the terminus bullet; recorded
  in the header this wave.
- **deck135 — a chosen ACTIVATED ABILITY is scored by the STATE DELTA (cost-paid AND effect-landed), generalizing the
  reveal zone-outcome instrument to catch SILENT engine failures (the fetchland fizzle).** ADOPTED (Method headline 4,
  instrument). Reinforced by deck102's reveal confirmation-read (zone outcome + pick cardinality, not fallback count).
  Also deck135: the reveal-partition trace CLOSED at n=2 (ENGINE-R1, vs14 s23 — 6 picks → 6 to-hand, 0 graveyard).
- **deck135 — on a FROZEN-guide + UNCHANGED-slate seat, decompose a win-column swing by CHANGE-IN-ENGINE-BUG-INCIDENCE
  FIRST.** ADOPTED (instrument additions), extends the wave-20 "a validated fix moves the record by ~0" rung to its dual
  (an intermittent bug present both waves moves the record by striking more games).
- **deck35 S1/S3 + deck62 #3 + deck49 — ROTATION discipline: candidacy triggers on DISCHARGE of the NAMED keep-reason;
  the two rotation gates can DISAGREE when new-work is HARNESS-lane, so NAME which gate holds the seat.** ADOPTED
  (rotation-discipline additions). deck35 discharged its keep-reason (rotation candidate); deck62's guide converged but
  held as harness-witness; deck49 → frozen-canary. Also deck35 S1: SUPERSEDE your own prior root-cause plainly when the
  fix's behavior demands it. deck35 S2: the max-mana-pressure positive case is the strongest sparing-fix validation.
- **deck14 P2 — score named-threat DEPLOYMENT as deployed/DRAWN gated on the mana line, not deployed/GAMES.** ADOPTED as
  the quantitative sharpening of the wave-20 displacement rung (Method headline 3 of wave-20).
- **deck14 P3 + deck62 (N7-stale) — stale_echo gains a FOURTH downgrade cause: a FORMAT-echo prefixing the SPELL name on
  a TARGET menu.** ADOPTED as a 2-seat catalogued cause; PARSER-owned (route to ledger), NOT a guide/matcher change.
- **deck14 P4 — coupled leaks routed to the engine in one wave must be scored per-fix AND at the SHARED failure surface
  to confirm the coupling term dissolved.** ADOPTED as a closing-loop method refinement (E1 bounce-on-stack + E2
  truncation both shipped; the bounce-menu intersection produced neither a fallback nor a truncation).
- **deck27 — an off-case revision may require MANY corpora to draw its trigger; do NOT cut/re-litigate an unexercised
  off-case; model-layer failures prose can't touch are sometimes best DEFUSED at the parser layer, not the decode
  layer.** ADOPTED as single-seat instrument notes (HOLD the off-case; promote the "many-corpora off-case" rung on a 2nd
  seat). The record-vs-tax rung CONFIRMED in the OTHER direction (record rose 3/6→5/6 on VARIANCE while the tax
  collapsed — a record jump is not evidence a change worked; attribute per-game).
- **deck62 #4 / deck35 G3 — CONFIRMED, no change:** computed-P/T distrust pocket DEAD at round 5 (deck62, cheap negative
  grep); per-blocker trade tags continue to drive correct SELECTIVE blocking (deck35, 4th wave). deck35 G4: the
  "haste-dodges-the-bug" correlate is RETIRED by the payment fix — a low-attacker aggro game post-fix is diagnosed as
  draw-screw / all-sick / forced mana-dork tap, not the old engine bug.
- **deck102 P2 — a co-firing gate resolved by supplying the deciding FACT (a symmetric sac is the OPPONENT's choice),
  not a fourth hard rule.** RECORDED as convergence with the existing co-firing doctrine (Method headline 1 of wave-20),
  not a new method.
- **REJECTED / not promoted:** no single-seat proposal added or cut a general-PROMPT (core) line (core = 16th PASS).
  deck49's COMMIT-brake-in-the-general-prompt (G-49a) again NOT promoted to core — single-seat, the guide layer already
  dissolves the develop-seam trigger, and the decode-repetition half routes to the sampler (repetition_penalty
  shipped-but-OFF); do not double-layer. deck27's "keep it simple when ahead" was NOT written into any guide (decode
  loop, defused by answer-first). No engine/harness/card item became prompt text (ENGINE-F1, HARNESS-N9, R-PAINLAND,
  Azcanta R6, Ashenmoor drift all live in the ledger). No proposal hard-suppressed a legal play.

- **Core (general prompt):** PASS, 16th consecutive. No `general-strategy.txt` produced (no change to verify). The
  skill's METHOD gained rungs via the wave-21 synthesis-notes header (parser-safety-route over-fire audit; answer-first
  validation discipline; the crutch-lifecycle second half of the demotion terminus; the silent-failure state-delta
  scoring rung) — those are guide-authoring/reviewer-instrument method, not the core prompt, and do not reset the core
  PASS count.

## Wave-20 changelog (each reviewer proposal: adopted / adapted / rejected + why)

- **deck49 NEW rung — a COMPETING-IMPERATIVE FORK the model CITES is a guide bug, and it fails as a TRUNCATION,
  not a wrong choice; resolve by killing the false branch ("either is fine, COMMIT") + tightening the over-broad
  exception.** ADOPTED as the wave's CORE method addition (Method headline 1 + a new Step-5 bullet). 2-seat with
  deck62 (below), echo-test-positive at both. Not a core-PROMPT line — a guide-authoring method rung, so core PASS
  holds. deck49 shipped its own two constrained-diff hunks on it.
- **deck62 #2 — a newly-added off-case CAUTION can become DECODE-LOOP FUEL when it near-contradicts an ACCURATE
  prompt annotation; fix by SHARPENING the caution's scope (cede to the annotation), not by deleting it.** ADOPTED,
  merged with deck49's rung as Method headline 1 point (b). The CAUTION-vs-ACCURATE-ANNOTATION face of the
  co-firing-imperative problem. deck62 shipped edit D on it.
- **deck14 Proposal 2 — HOIST a buried, passive, #1-leak line into a named TRAP is a legitimate turnaround-mode
  diff, distinct from the forbidden "third copy of a well-placed tell."** ADOPTED (Method headline 2). Single-seat
  but a guide-authoring method generalization (the hoist TEST is portable), acceptable per the method/core split.
  deck14 shipped the bounce-on-stack trap on it.
- **deck14 Proposal 1 — the wave-19 two-tier aggression method LANDED on the next corpus (finisher 1/6 → 4/6);
  promote from "written" to "validated," and split non-deployment into displacement vs rule-failure.** ADOPTED
  (Method headline 3). A clean second-corpus validation of a prior wave's headline method.
- **deck14 Proposal 3 / deck62 #1 — the parse layer has THREE states (parsed / salvageable-mis-format /
  truncated-before-line); a salvage-extension NO-FIRE on a truncated-before-line reply is CORRECT, not a miss.**
  ADOPTED (Method headline 4), 2-seat converging. Companion (deck49 loop-salvage caution — do NOT salvage a
  genuinely-unresolved oscillating deliberation) ADOPTED into the same headline.
- **deck135 — the reveal-seam PARTITION-TRACE completed its FOUND→FIXED→VALIDATED lifecycle; a reveal-fix
  confirmation needs the ZONE-OUTCOME read on the SAME card, not "no fallbacks."** ADOPTED (instrument additions).
  Also: a per-option ELIGIBILITY annotation is verified by REASONING-TAX COLLAPSE, not a marker grep. ADOPTED.
- **deck135 — the retracted-first-CHOICE HIDDEN loop undercounts the decode-loop tax; grep clean-parsed records
  for a repetition + self-retraction signature.** ADOPTED as a single-seat WATCH (adopt into the scoring rung on a
  2nd seat). Routing HARNESS (prefer the LAST well-formed CHOICE line).
- **deck102 Proposal 1 — the ABSENT-ENGINE-CARD newcomer: write for the payoff-ABSENT majority + prioritize-when-
  drawn; verify LIBRARY vs stranded-zone before calling an engine "unreachable."** ADOPTED as a NEW Step-0-bis face
  (wave-20 additions block). Single-seat newcomer-method generalization.
- **deck102 Proposal 2 — the SYMMETRIC-cost card read as one-sided is a FOURTH rule-3 idiosyncrasy face; corrective
  is a scoped FIRE-GATE, never suppression.** ADOPTED as the 4th idiosyncrasy face in Step 0-bis. Flagged for
  eventual CORE-PROMPT promotion IF a second symmetric deck appears (single-seat now → stays in the guide).
- **deck102 Proposal 3 — the DISCARD-PICK reveal is a pick-CARDINALITY sub-class; read the `<N>` token and score
  over-selection.** ADOPTED as a NEW reveal-seam sub-class (Step 0-bis additions + instrument additions).
- **deck102 Proposal 4 — the losing-position spiral scales with the reviewed deck's own board complexity
  (prison/attrition amplifier).** ADOPTED as an amplifier note on the wave-18 spiral rung.
- **deck62 #3 — attribute a fallback-path misplay to the HEURISTIC, not the guide/model; check whether the
  heuristic makes the exact mistake a guide clause forbids (structurally unreachable).** ADOPTED (instrument
  additions / ownership-lane discipline).
- **deck49 CONFIRMED — the 0-attacker durdle disqualifier list grows a THIRD item: "is the creature still YOURS?"
  (a stolen/controlled threat is not a failed attack).** ADOPTED as a sharpening of the wave-19 Method-headline-2
  instrument.
- **deck14 Proposal 4 / deck35 / deck62 — CLOSED items recorded: combat `options_text` observability gap FIXED
  (deck14 40/40, deck62 confirmed); computed-P/T distrust pocket DEAD at round 4 (deck62, cheap negative grep);
  attacker-sparing PAYMENT fix validated working (deck35).** CONFIRMED / recorded closed.
- **deck27 Step-0-ter confirmations — tax-collapse-is-the-primary-signal (record and tax can DISAGREE, tax is
  load-bearing); died-but-tax-persists = tax RELOCATED (re-classify survivors by mechanism); write the off-case
  even if the winning corpus never needed it.** CONFIRMED, no skill change (the newcomer re-fired existing rungs).
  deck27 shipped its one surgical revision (late-stall durdle off-case) on the third confirmation.
- **deck27 / deck110 WATCH — the AHEAD-position giant is a DECODE degeneration, not over-reasoning (2 seats:
  deck110 wave-19 + deck27 vs49 s16); route to the sampler, not a guide "keep it simple" line.** RECORDED as a
  2-seat decode-routing watch; hold for a 3rd instance before a rung. And the FREE-BLOCK-DECLINE class re-fired
  (deck27 vs49 s5) but on the SAME opponent/card as wave-19 → does NOT promote (WATCH); it reinforces the existing
  representation-beats-instruction principle (a maximally-explicit rule the model ignores is a MODEL-OBEDIENCE gap
  more prose cannot close).
- **REJECTED / not promoted:** no single-seat proposal was allowed to add or cut a general-PROMPT (core) line
  (core = 15th PASS). deck49's COMMIT-brake-in-the-general-prompt (G-49a) was NOT promoted to core — single-seat,
  and the guide layer already dissolves the trigger (do not double-layer; measure the guide fix first); its decode-
  repetition half routes to the sampler. The decode-repetition loops were NOT written into any guide (model/decode
  watch, all seats). No proposal hard-suppressed a legal play (doctrine): the symmetric-mass-effect corrective is a
  scoped FIRE-GATE, not a "cannot cast"; the bounce-on-stack fix is a trap + representation, not a suppression. The
  correct-but-ignored annotations (deck14 self-harm warning, deck27 free-block) were NOT "fixed" by weakening the
  accurate annotation.

- **Core (general prompt):** PASS, 15th consecutive. No `general-strategy.txt` produced (no change to verify). The
  skill's METHOD sections gained rungs (Step 5 co-firing-imperative bullet; Step 0-bis wave-20 additions) — those
  are guide-authoring method, not the core prompt, and do not reset the core PASS count.

## Wave-19 changelog (each reviewer proposal: adopted / adapted / rejected + why)

- **deck14 Proposal 1 — PROMOTE the combat-answer FORMAT-DRIFT rung from single-seat WATCH to validated
  method.** ADOPTED (Method headline 3). Wave-18 self-flagged it "promote on 2nd seat / on a clean before/after";
  deck14's before/after (8-13k combat rambles → 300-800-char parsed coded lines; two residual spikes both
  parse) is that validation. Now a standard rung for derivation-heavy-combat decks.
- **deck14 Proposal 2 — write aggression countermands in TWO tiers (attack AND be a real clock).** ADOPTED as
  a NEW Step-0-bis/0-ter method (Method headline 1). Single-seat but a guide-AUTHORING method generalization
  (how to word a first guide's aggression rule), not a core-prompt line, so acceptable per the method/core
  split. deck14 shipped its own Rule #1B on it.
- **deck14 Proposal 3 — name the MENU/INTENT MISMATCH sub-shape of the list-authority family.** ADOPTED as a
  naming refinement (no new core text): narrate action X, emit an index pointing at option Y (distinct from
  out-of-range over-reach and unpayable-decline over-restriction). Durable lever = representation; recorded in
  synthesis-notes cross-seat watches.
- **deck14 Proposal 4 — reviewer discipline: verify the focus surface is LOGGED at your seat before
  reporting.** ADOPTED (instrument additions). Born from deck14's empty combat `options_text` defeating its
  TRADE-OUTCOME assignment.
- **deck35 S1 — 0-attacker durdle instrument must distinguish MODEL-durdle from ENGINE-NON-ISSUANCE.** ADOPTED
  (Method headline 2). The wave's most important instrument refinement — it kept an engine bug (attackers
  non-issuance) from being logged as an FC1 durdle regression.
- **deck35 S2 — tax-relocation is a recurring MULTI-WAVE dynamic; re-scan the other low-stakes seams after
  taming a tax at one seam.** ADOPTED as a sharpening of the existing tax-relocation rung (rule 4). Evidence:
  the block tax tamed → jumped to trivial LAND-DROP decisions (both of deck35's fallbacks).
- **deck35 S3 — a "trust the rendered number" rule made TWO-SIDED is validated end-to-end.** CONFIRMED (already
  in skill via the wave-18 headline); the wave-19 per-blocker trade tags gave the model a computed answer to
  trust and it trusted them (vs27 seq13, vs49 seq17). Reinforces: compute-and-print the distrusted quantity,
  then point the guide line at that named printed tag; prose alone is weaker.
- **deck35 S4 — a validated guide's win-rate dip decomposes into per-game NON-guide causes (variance / engine /
  matchup-speed); do not invent recovery prose.** ADOPTED as a Step-0-ter decomposition rung. deck35's 3 losses
  = one variance (race lost at opp-life-1), one engine (attackers non-issuance), one turn-6 matchup wall — none
  guide-fixable, so the guide stays near-frozen.
- **deck62 #1 — a reviewer-filed ENGINE item that ships and CLOSES its class is the highest-value review
  outcome; credit the collapse to the ENGINE, verify at the FILING seat, then check whether the interim guide
  line is now redundant (keep it frozen as belt-and-suspenders).** ADOPTED as a cross-wave apportionment
  refinement. deck62's own N2 (A-list-disobey→unparse) shipped as the name-tolerance batch; 3 saves at the
  filing seat, 0 fallbacks.
- **deck62 #2 — key the off-case branch on the CONDITION ("cannot win THIS turn"), not the STATE ("behind").**
  ADOPTED (Method headline 4). deck62 shipped its own edit A on it.
- **deck62 #3 — bucket truncation-unparses by MECHANISM (decode-repetition vs advancing-spiral) before
  proposing a guide fix.** ADOPTED (instrument additions). Only advancing-spirals are guide-addressable.
- **deck62 #4 / deck27 #4 — score a representation annotation on TWO axes (correct AND obeyed); route
  correct-but-ignored to the GUIDE.** ADOPTED (instrument additions), 2-seat converging.
- **deck49 CONFIRMED rungs — mask-lift re-test discipline; annotation SCOPE before trimming a rule; the
  representation-beats-instruction boundary (apportion by the annotation's actual trigger surface).** ADOPTED
  (instrument additions / mask-lift discipline). C4 closed exactly as scheduled.
- **deck49 NEW rung — a trade-outcome ANNOTATION masks a stale card-script stat at the decision surface; verify
  the stat separately.** ADOPTED as an Oracle-queue method note (Ashenmoor Liege engine 4/1 vs real 4/4).
- **deck49 WATCH — an off-case authored proactively can go a whole corpus UNEXERCISED; track it as an OPEN
  validation thread, not a closed one.** ADOPTED as a Step-0-ter tracking note; it is the deck49
  rotation-hold caveat.
- **deck135 — reveal-seam PARTITION-TRACE instrument; SURFACE-THE-ELIGIBILITY-FILTER representation principle;
  "a card-SCRIPT bug can drive win-rate independent of the guide."** ADOPTED (instrument additions). The reveal
  seam is the newest proving-ground; score the ZONE OUTCOME, not "did the model answer."
- **deck135 / deck110 — pitch-card-naming annotation VALIDATED; promote "when the engine auto-selects a hidden
  cost, name what it will consume" as a template.** CONFIRMED (already the wave-18 direction); the finisher was
  not eaten this corpus. Recorded as a positive annotation-design template.
- **deck27 Step-0-bis confirmations (trust-the-rendered-number/mana face; losing-position off-case; 0-attacker
  durdle instrument, with a TIME-TO-FIRST-LORD proxy for decks with no cap game).** CONFIRMED; the newcomer
  re-fired existing rungs, no new headline. The time-to-first-lord proxy adopted at reviewer discretion.
- **deck110 (freeze/rotate-out seat) — no skill-content proposal (skill.md byte-identical to baseline).**
  N/A. Its contribution is the block-echo-fix VALIDATION + rotation (recorded in synthesis-notes).
- **REJECTED / not promoted:** no single-seat proposal was allowed to add or cut a CORE line (core = 14th PASS);
  the decode-repetition loop was explicitly NOT written into any guide (model/decode watch); no 4th Force
  whitelist line (already-maximal, prior-wave ruling holds); the correct-but-ignored annotation was NOT
  "fixed" by weakening the accurate annotation.

## Wave-18 changelog (each reviewer proposal: adopted / adapted / rejected + why)

FOURTH per-deck-rotation cycle. Seven seats (110/135/21 frozen; 62 revised-round-2; 49/35 first-guide validations
→ revisions; 14 guideless newcomer → initial guide). Gate for a skill change: ≥2-seat convergence OR an
exceptional / measurement-only single-seat.
- **deck62 #1 (rule-6 off-case spiral) + deck35 (forced-lethal escape) + deck49 (flood off-case)** → ADOPTED as a
  PROMOTED Step-0-ter rung (LOSING-POSITION / OFF-CASE SPIRAL, 3-seat convergence; each authored the fix in-guide).
- **deck62 #3 / deck49 rule-4 / deck21 #5 / deck35 S2 (giant-reply gauge)** → ADOPTED as a PROMOTED measurement
  rung (4-seat convergence): tax = giant-reply count bucketed by board-state, not fallback rate.
- **deck35 S3 (two-sided trust-the-rendered-number) + deck14 Proposal-1 combat face** → ADOPTED as a SHARPENING of
  the promoted trust rule (scope to both boards + damage annotations).
- **deck49 (concurrent-engine-fix apportionment)** → ADOPTED, sharpens rule 5.
- **deck62 #2 (name-the-trigger)** → ADOPTED, sharpens rule 3.
- **deck21 #4 (trim = separate pass)** → ADOPTED as a method rule.
- **deck49 (burn-target-menu face-rate) + deck135 (repeat-loop signature)** → ADOPTED as measurement instruments
  (single-seat, measurement-only exception; bias no guide content).
- **deck14 Proposal-1 (combat FORMAT-DISCIPLINE guide line)** → WATCH (single-seat, self-flagged
  promote-on-2nd-combat-format-drift-seat; the durable fix is the engine parser, ledger #1 — the guide line is
  interim). The ENGINE side IS 2-seat (deck14 + deck110) and is promoted in the ledger, not here.
- **deck110 (position-partition for rambles)** → reinforcement, already implicit; recorded as a reviewer habit.
- **deck135 #1 (off-whitelist counter warning + name the pitch card)** → ROUTED to the engine annotation-design
  ledger (owner-ruling warning annotations), NOT the skill.
- **deck35 G1 (POWER is the first number in [X/Y])** → ROUTED to the representation ledger (single-seat core → not
  core; it has a representation cure, and the reviewer's own instruction is to pick the layer, not both).
- **deck49 G-49a + deck35 G2 (reasoning-length / forced-outcome brake)** → CORE WATCH (single-seat each, same
  family; the losing-position GUIDE off-case branch is this wave's shipped lever — do not double-layer into core
  yet). Core PASS #13.
- **Reinforcements (no new text):** 0-attacker durdle instrument (clean DIED verdict + new deck type); "quotes the
  guide verbatim" = death-certificate; attribute the DECISION not the win/loss; construction-axis losses route to
  the roster owner.
- **Core:** PASS, 13th consecutive. No `general-strategy.txt` produced (no change to verify).

## Wave-17 changelog (each reviewer proposal: adopted / adapted / rejected + why)

**SKILL — adopted:**
- deck62-S4 (wave-16, tagged single-seat) + deck17 + deck49 + deck35 *TRUST THE RENDERED NUMBER* — **PROMOTED**
  to a confirmed general Step-0-bis first-pass rule (4 seats, two faces: mana availability + derived board stats).
- deck49 (proposal 2) + deck62 (established rule) *aura opponent-only-targets OFF-RAMP* — **ADOPTED** as a
  Step-0-bis aura corollary (2 seats); durable fix cross-referenced to engine ledger E-49c.
- deck35 *0-attacker-game durdle signature reviewer instrument* — **ADOPTED** into Step 0-ter as a review
  instrument, justified single-seat (a MEASUREMENT tool, not a behavioral doctrine; biases no guide content; applies
  to every aggro seat).

**SKILL — recorded as WATCH (single-seat, not adopted; promote on a 2nd clean seat):**
- deck49 (proposal 1) spell/burn-heavy newcomer → teach the TARGET SUB-MENU flow (a third rule-3 face). Durable fix
  is the engine sub-menu representation (ledger #1). deck49 shipped it in-guide meanwhile.
- deck21 (proposal 1) scope a COUNTABLE-COST rule to the input quantity + give the zero/degenerate case a branch —
  single-seat + 2-seat-adjacent to deck17's superlative-scoping; strong candidate, hold for a 2nd direct instance.
- deck21 (proposal 2) auto-no-block race-math generalizes flyer→ground — explicitly WATCH per reviewer (risk of
  re-broadening the just-tightened rule on one confounded instance).

**SKILL — recorded as CONFIRMATION (no new text):** deck17 (died-quiet = re-check candidate; causal-matchup reversal
> win-rate tick), deck62 (re-classify fallback survivors by mechanism when the rate collapses), deck35 (score the
ACTION not the verbal model on comprehension watches). Folded as reinforcement rungs in Step 0-ter.

**SKILL — no delta:** deck110 (methodology held cleanly; layer-routing + validated-by-absence + canary all applied
without friction), deck135 (no binding delta; the multi-step-menu observation routes to the engine ledger, already
handled by existing engine-item routing).

**CORE — PASS (12th consecutive), no edit.** deck49-G1 (explain the target sub-menu in core) is single-seat AND is
properly a representation/engine item (the durable fix serializes the sub-menu; deck62 reports the shipped own-vs-
opponent target annotation is ALREADY the working fix) → ledger, not core. deck17 (already-lethal cue, block-
restriction annotation), deck35 (combat-outcome annotation), deck110 (mana-ability no-op suppression), deck21
(Spark Elemental [treason] relabel) are all single-seat and/or representation/engine items → ledger, not core prose.

**GUIDES:** three FREEZES (deck110/135/17, byte-identical, cmp-verified); two REVISIONS (deck62 surgical: Blanchwood
trust-line + A-list-only attacks + aura-section trim; deck21: POWER FLOOR on the race-math exception + kill-on-sight-
engine sharpening); two NEW guides (deck49 Dragons, deck35 Slivers, Step-0-bis, format-vetted in band).

## Wave-16 changelog (each reviewer proposal: adopted / adapted / rejected + why)

**SKILL — adopted (multi-seat or completes a method line):**
- deck17 (WATCH-1/2/3/4 + reconfirmed) + deck21 (V1/V2/V3/V4) *the FIRST-GUIDE VALIDATION doctrine* — ADOPTED as the
  new **Step 0-ter**. Two independent first-guided seats converged on: died/persisted/mutated-per-class before the
  win-rate; reconcile a "looks-disobeyed" choice against the eligible/offered set (deck17 WATCH-1, attacker-seat
  cousin of the wave-15 defender-drop); separate the guide's JOB (classes died? tax collapsed?) from the RECORD
  (deck21 V4); a first revision TIGHTENS an over-broad matchup-section countermand that silently overrides a general
  exception, and scope a superlative to intent not surface property (deck21 V1 + deck17 WATCH-3, 2-seat); a dead
  belief's tax RELOCATES to a successor at the same seam (deck17 WATCH-2). Single-seat WATCH sub-lessons folded:
  write the off-case branch first (deck17 WATCH-4), validate the fix-WINDOW when the target grows / early-or-never
  (deck21 V3), replace value-evocations with countable mechanics (deck21 V2, also reinforcing Step 5 word-choice).
- deck62 (S1/S2/S3/S4) *engine-deck refinements to Step 0-bis* — ADOPTED as engine-deck instantiations (single-seat,
  first engine deck, tagged pending a 2nd). S1 capability-table-as-rule-#1 for the FABRICATE-card-text failure shape
  (extends rule 3); S2 teach engine SEQUENCING as an explicit order (generalizes rule 5 from attacking to ordering);
  S3 broaden the attack-floor exclusion to walls + 0-power engine/dorks + the summoning-sick-already-dropped note
  (2-seat-adjacent to deck17's defender exclusion); S4 TRUST-THE-RENDERED-NUMBER (printed [X/Y]), which CONVERGES at
  a 2nd seat with deck17's trust-the-Mana-available-line into a general guide line.
- deck110 (mulligan counter-hypothesis) + deck44 (S2 corpus-wide-verification, S1 present-and-disobeyed→structural)
  *review-discipline: verify the premise before the change* — ADOPTED as Method headline 3 (do not tighten a rule on
  losses when the loose behavior is universal across the wins; a corpus-wide claim needs ≥2-seat verification; a
  recurring already-addressed misplay is evidence FOR an engine fix, not a third prose warning). Converges with
  deck17 WATCH-1.

**SKILL — WATCH (single-seat, not yet a method line):**
- deck133 (Proposal 1) *the DEGENERATE-LABEL third stale_echo cause* — WATCH; folded into the Step-2 Mechanism-A/B
  scoring rung as bucket (c), pending a second seat. Harness/serialization, upstream of the matcher.

**SKILL — no-change proposals confirmed (reviewers validated existing method, no edit):** deck110 skill.md (VALIDATION
+ FREEZE wave, no new method headline; Galvanic/attackers/equip-outcome-delta rungs all re-confirmed), deck135 skill.md
(explicit NONE — the exercise-check, layer-routing, and verify-the-menu reflexes all resolved the wave's hardest
calls; the one OBSERVATION — "an owned fix can SHIP and still get ZERO clean exercise; report NOT-EXERCISED and carry
the validation TODO forward, do not let a shipped fix read as validated" — is captured in the wave-16 validation table
and the standing not-exercised≠not-fixed rung), deck44 skill.md (S3 freeze discipline + S4 hybrid-annotation as
positive controls). No method was cut.

**CORE PROMPT — PASS (11th consecutive), no edit.** `general-strategy.txt` is a byte-identical copy of the live core.
No 2+-seat core-wording candidate survived: every seat voted PASS or routed its item below the core. The two
recurring cross-seat WATCHes that touch core wording — the "hold instant-speed interaction" conditioning (deck21
validated the burn-deck countermand works; deck17's steal-hoarding is a related but distinct shape) and the
stale-within-turn-mana / trust-the-rendered-number family — are NOT yet 2-seat on the SAME shape and route to the
per-deck guide + the representation ledger, not a core line. REJECTED for core (single-seat / present-and-disobeyed):
deck62 G1 "don't invent effects" (unobservable core line; the capability table + option-line rules-text snippet is
the fix), deck44 G2 stack-vs-battlefield targeting (model-watch, engine suppression neutralizes it), deck135 Force
reflexive-cast (model-experiments).

## Wave-15 changelog (each reviewer proposal: adopted / adapted / rejected + why)

**SKILL — adopted (multi-seat, cross-cutting, or completes a method line):**
- deck17 (WATCH-1/2/3) + deck21 (D1/D2/D3, F7) *the GUIDELESS-first-pass doctrine* — ADOPTED as the new **Step 0-bis**.
  The two newcomer seats independently converged on: reasoning-tax-spikes-as-saliency (deck21 D1 + deck17); one-
  dominant-idiosyncrasy earns rule #1 above the win engine (deck17 hybrid + deck21 burn-as-clock/Spark); teach ENGINE
  reality not Oracle memory + route card-script divergences to notes (deck21 D3 + deck17 engine-derived rules text);
  a first guide often must COUNTERMAND a generic core line, not just add facts (deck21 D2, deck109-corroborated);
  write the attack/deploy floor first-pass for evasion/aggro decks (deck17 WATCH-3 + deck21 F7, reconfirming the
  deck135 wave-5 rung on new archetypes). Overlapping newcomer lessons counted as 2-seat per the wave-15 brief.
- deck140 (P1) + deck133 (P1) *count and impact are DIFFERENT measures — report BOTH* — ADOPTED as a reviewer-method
  sharpening (deck140 dual-hiding 48 instances / 0 misplays; deck133 stale_echo true-catches-vs-false-positives;
  deck110 Galvanic dedup-trap = same principle on the verification tool). Reviewer procedure, not guide/core.
- deck140 (P2) + deck135 (ENGINE #2 UNVALIDATED) *NOT-EXERCISED ≠ NOT-REPRODUCED ≠ RESOLVED — confirm the action
  occurred before scoring an engine item's recurrence* — ADOPTED as a reviewer-method sharpening (deck140 Rakdos-X
  0 casts = untested; deck135 inverted-freeze window ended the game = unvalidated).

**SKILL — validations of shipped fixes (no new method; confirm + close):**
- deck110 (skill) + deck133 (skill) + deck135 (skill) *the three wave-14 fixes (Galvanic metalcraft `3cb901295`,
  attackers name→index reconcile + absent-echo route `d0f88326b`, crash-fix `aad32bc7f`) VALIDATED at scale* — no
  method edit; they CONFIRM wave-13 headlines 1/2/3 and CLOSE the classes those headlines named. Folded the Galvanic
  multi-instance verification caveat (COUNT instances / read the delta, don't dedupe the event string) under
  wave-13 method-headline 3 ("read RESULT not INTENT") — single-seat deck110, rung-completing.
- deck135 (general-suggestions) *the FIRST completed live Force alt-cast counter (deck140 seq18, load-bearing win)
  proves the "name and date the first live confirmation of a long-open engine item" method* — the seat owner offered
  it as a cross-seat call; folded into the synthesis method (celebrate+seq the first live instance), engine ledger
  owns the Force items.

**SKILL — adopted as WATCH (single-seat, rung-riding):**
- deck133 (P1) *the absent-echo route has a SECOND error direction — a FALSE POSITIVE via name-SUPERSTRING mismatch
  (echo names an offered option as a superstring, downgraded anyway); scoring must split true catches vs false
  positives* — WATCH (single-seat, 1 non-deciding firing; no 2nd seat showed a superstring false-positive this
  corpus). Durable fix = HARNESS prefix/substring match; not a guide/core edit. Adopt to method if a 2nd seat repros.
- deck17 (WATCH-2) *derive-the-defender-set and score attacker declarations against eligibility* — WATCH (only
  deck17 had walls this corpus); applied inside Step 0-bis(5) as a conditional rule.
- deck135 (MODEL) *Force reflexive-cast swing (disciplined-decline → cast-all-6); over-Forces marginal noncreature
  spells but got the two wipes right* — WATCH, model-side; guide already scopes Force correctly.
- deck110 (Finding 6) *model names SUMMONING-SICK creatures as attackers and folds them into lethal math* — WATCH,
  model-side; harmless (engine roster + reconcile exclude them), latent if it ever gates a lethal/hold-back call.

**SKILL — no change (confirmations, freezes):**
- deck135 / deck140 / deck44 / deck110 / deck133 (skill) *NONE / re-witness* — the freeze-vs-adopt discrimination,
  layer-routing, pause-aware demotion, and the reply-protocol termini all held with no method edit. Board-state
  contradiction timeout-promotion stays WATCH (deck110 s23, deck17 s21/s23 witnesses did NOT time out).

**SKILL — REJECTED:**
- deck44 (F2 / notes #1) *combat asks removed corpus-wide; add a "surface-audit before crediting block/lethal prose"
  method and trim block sections corpus-wide* — REJECTED. Orchestrator-verified per-seat counts show attacker/blocker
  asks are NONZERO at every non-creatureless seat (deck110 audited 20 attacker windows; deck135/deck21 show live
  `ATTACK:` lines this corpus). deck44's phase-enumeration mis-scanned; the auto-combat premise, the surface-audit/
  trim-block method, and deck44's loss attribution are all rejected. deck44 guide FREEZE stands; seat → HOLD one cycle.

**CORE — PASS (10th consecutive):** the colored-pays-generic / trust-the-option-list PROMOTION TRIGGER fired
cross-seat (deck135 s3 game-losing offered-payable decline + deck17 2 under-deployments + deck21 F5) — but the
verdict is NOT a core edit: the core states both governing facts VERBATIM (line 4 rule fact + line 36 option-list
authority), deck135 s3 is present-and-disobeyed (the representation-beats-instruction wall), and the durable fix is
the hybrid-pip / affordability OPTION-LINE annotation, PROMOTED to the top representation item. The "hold instant-
speed interaction" question is per-deck guide territory (core line 21 already carries the burn-as-clock override).
`general-strategy.txt` is a verbatim copy of the live core.

**GUIDES — 5 FREEZE (veterans 44/110/133/135/140, byte-identical `cmp`-verified) + 2 NEW initial guides (17, 21,
guideless newcomers).** The 2 new guides ship for validation next corpus; no veteran guide edited.

## Wave-13 changelog (each reviewer proposal: adopted / adapted / rejected + why)

**SKILL — adopted (multi-seat, cross-cutting, or completes a method line):**
- deck133 P1 + deck140 P1 + deck44 F1 *the reconcile-named-action arm's ABSENT-ECHO branch — an echo naming NO
  offered option is a STALENESS signal, route to heuristic/Cast-nothing, never the raw index; re-anchor indices
  after a same-turn cast* — ADOPTED (3 seats, GAME-LOSING ×2) into the Step-2 reply-protocol taxonomy as the
  name-echo protocol's load-bearing completion of wave-11 headline 1; the paired Mechanism-A/B SCORING split
  went into the Step-0 confirmed-fired terminus. Reply-protocol layer; NOT a guide/core edit.
- deck109 (method) + deck44 F2(item 2) *score attacker declarations by parsed INTENDED-vs-RECORDED count off
  the `ATTACK:` line; the MIXED index+name form is a SILENT non-fallback drop, the FULL-NAME form unparses to a
  heuristic that is aggro-neutral* — ADOPTED (2 seats; corroborated deck131/135/110) as a new HARNESS-DEFECT
  SWEEP bullet in Step 2. Durable fix = attackers name→index reconcile arm; attackers-parser layer, NOT a guide.
- deck110 (skill sharpening) *verify RESOLVED effects against the game-log events, not the model's asserted
  magnitude — the Galvanic Blast lesson* — ADOPTED (single-seat, RUNG-COMPLETING: extends confirmed-fired from
  "surface rendered" to "mechanic resolved as rendered") into the Step-0 terminus. Card-script DATA fix to the
  ledger (verify Oracle first); guide is Oracle-correct, not edited.

**SKILL — adopted as WATCH / reviewer-method (single-seat, rung-riding, or folded into procedure):**
- deck135 (skill) *the representation-contradiction-multiplies-reasoning class lives on the BOARD-STATE
  serialization too (stale `[attacking]`, inverted `untaps next turn` on a frozen permanent), and under the
  latency cap its worst outcome is a TIMEOUT surrendered to the heuristic* — ADOPTED single-seat as a
  board-state LOCUS extension of the deck135-owned reasoning-tax rung (grep the board serialization, not only
  option annotations); the timeout-as-terminal-cost PROMOTION is WATCH pending a 2nd seat on a non-lost decision.
- deck131 (method) *when a block/attack guardrail appears VIOLATED, cross-check the OPTION/BLOCKER LEGEND
  against the battlefield line first — a missing-legal-option menu defect masquerades as a guardrail failure*
  — FOLDED into the reviewer procedure (Synthesis notes, reviewer-method sharpenings).
- deck110 (verification sharpening) *when a guardrail's seam becomes newly EXERCISED, re-audit against the
  PROMPT'S OWN annotation before attributing to the guide (score model/representation-wall, not a guide gap)*
  — FOLDED into the reviewer procedure.

**SKILL — no change (confirmations, freezes):**
- deck131 + deck110 *name-echo index-wins-on-conflict validated on PRESENT-option conflicts; BLOCKS:none parses
  clean (wave-11 parser bug CLOSED)* — positive layer-routing confirmations, no method edit.
- deck109 (reconfirm) *hybrid card-note stopgap VALIDATES a 3rd wave + engine-execution confirmed; option-line
  hybrid annotation (Ledger B) still UNSHIPPED — keep the card-notes at full prominence, frozen* — no change.

**CORE — PASS (9th consecutive):** all 7 seats voted PASS; no 2+-seat core candidate. The colored-pays-generic
family produced ZERO actual offered-payable declines (deck135 fired-then-self-corrected; the deck140 vs109 s9
decline is the `Mana available` line hiding a dual color, an engine representation item). Core body already
carries both governing facts; a 3rd copy is the sanctioned-against escalation. `general-strategy.txt` is a
verbatim copy of the live core.

**GUIDES — 7 FREEZE** (all byte-identical to live baka guides, `cmp`-verified). No guide edits.

## Wave-11 changelog (each reviewer proposal: adopted / adapted / rejected + why)

**SKILL — adopted (multi-seat, cross-cutting, or completes a method line):**
- deck140 P1 + deck135 Q5 + deck109 A' *the answer-after-plan trailing PLAN/CHOICE mismatch has ONE root (the
  UNLISTED/UNAFFORDABLE-CARD FIXATION) and is BIDIRECTIONAL — over-cast (a listed sweeper over Cast-nothing) /
  under-cast (Cast-nothing over an offered payable removal); the reconcile-named-action A/B arm must handle
  "named action absent from the options" -> best AVAILABLE listed option / pass, and reject-and-repair an
  out-of-range index* — ADOPTED (3 seats this wave + wave-10 deck110-P3 lineage; PROMOTED from single-seat
  WATCH) into the Step-2 reply-protocol taxonomy as the trailing-mismatch shape's unified root and A/B target.
- deck140 P2 *distinguish the trailing-index MISMATCH (plan-vs-CHOICE, reply-protocol) from the PHANTOM-BOARD
  misread (plan-vs-BOARD, representation) before routing a "sweep into creatures:0" symptom* — ADOPTED
  (single-seat, completes the wave-6 hallucinated-board rung) as an additive DIAGNOSTIC in the reply-protocol
  block.
- deck133 P1 + deck131 H1 + deck44 (notes) + deck110 (verification sharpening) *classify a confirmed-fired
  annotation/guardrail leak by MECHANISM and LAYER before crediting/re-diagnosing — reasoning-disobey /
  number-parse / blocker-parser / truncation-scavenge / churn-vs-correct-play have different owners; a raw
  self-cast/obey-rate/usage count conflates them; for `kind==blockers`, chosen_text records the PARSE, not
  executed combat* — ADOPTED (4 seats; extends the wave-10 "score in the reasoning" qualifier) into the Step-0
  confirmed-fired terminus.
- deck135 (skill) *an annotation-SUPPRESSION / contradiction-REMOVAL fix's confirmed-fired signals are the
  contradiction string -> 0 + a reasoning-tax COLLAPSE (category change) + the recovery of an ADJACENT faculty
  the noise was corrupting* — ADOPTED (single-seat, 2+ waves, completes the wave-8 reasoning-tax-drop rung)
  into the Step-0 terminus as the suppression/deletion shape's signals.
- deck44 (notes) *re-verify a KNOWN-UNFIXED engine defect by RECURRENCE with OUTCOME-COUPLING reported
  separately; a harmless-recurrence corpus yields a cleaner regression fixture — the confirmed-STILL-BROKEN
  mirror of the confirmed-fired terminus* — ADOPTED (single-seat, rung-completing) into the Step-0 terminus as
  a method note; the actionable lever stays ENGINE (ledger #2), NO guide line.

**SKILL — recorded as ledger/watch (single seat, additive but not a method cut):**
- deck133 (skill) *reasoning-DISOBEY of an own-targets annotation via a stack-vs-battlefield / destroy-vs-
  counter rules misconception (self-Fatal-Push onto an opponent creature SPELL on the stack)* — WATCH
  (single-seat, 2 instances, both won); the representation refinement (name WHY the on-stack target is
  excluded) is engine-ledger; promote the method sub-shape only with a 2nd seat.
- deck109 A' + deck131 H3 *the out-of-range-index flavor of the unlisted-card fixation (the imagined card
  treated as an extra option)* — folded into headline 1's promotion (2nd/3rd seats), reply-protocol
  reject-and-repair.
- deck131 H4 *block-when-not-lethal via a FIRST-STRIKE combat-math error (treated a first striker as a
  simultaneous trade)* — WATCH (single-seat); strengthens the un-shipped blocker-option trade-outcome rider
  (`[you deal 0 - first strike]`); do NOT promote off one instance, do NOT touch the guide.
- deck135 (notes) *Azcanta-transform card-fact hallucination (Search for Azcanta believed to flip into a 4/4
  flyer, not a land) driving an under-develop* — WATCH (single-seat, single window); a KEY CARDS micro-note is
  the cheapest fix only if a 2nd window recurs.
- deck44 #3 + deck131 H1 *strict free-form block/attacker parser rejects reasonable declarations (bare
  `BLOCKS: none`, card-NAME assignments, out-of-range indices)* — the bare `BLOCKS: none` half is
  SHIPPED-PENDING-CORPUS (explicit all-decline now declares no blockers; AIPlayerGPT.cpp working tree, suite
  running); the name-based/out-of-range half is a harness WATCH, route to the parse layer, never the guide.

**SKILL — no-change proposals confirmed (all seven guides FROZE byte-identical to the live baka guides; verified
by diff):** deck44 (auto-tap #2 recurred outcome-neutral = clean regression fixture; truncation-scavenge class
CLOSED 29/29; 8th consecutive freeze), deck109 (hybrid card-note VALIDATED, class -> 0, fact echoed verbatim;
demotion caveat KEEPS it at full prominence while the option-line annotation is unshipped; attacker seam 26/26;
Obliterator absolute untested a 3rd wave, KEPT), deck110 (artifact-tag 211/211 a 4th wave; re-equip marker
90/0; outcome-delta churn re-validated by the consolidation shape; turnaround/variance freeze a 6th wave),
deck131 (mulligan gate 7/7 both directions a 2nd corpus; Mountain-first 12/12 a 4th corpus; YP-swarm
second-win-path FIELD-PROVEN in the vs135 win; BLOCKS:none the sole game-relevant finding, routed to parse —
SHIPPED-PENDING-CORPUS), deck133 (best corpus 5/6 clean kills; empty/thin-hand Thoughtseize 100% obeyed 48
windows; GM bare-N confirmed 3/3; both Lilianas coexist; ramble guard end-to-end), deck135 (Force
annotation-suppression validated by tax-collapse + discipline recovery; develop-branch held; byte-identical),
deck140 (sweep trigger validated a 4th corpus 23/24; adjudicate-by-life confirmed a 2nd corpus; ramble guard
machine-checked over all 6 seats). No method line was cut.

**CORE PROMPT — see `synthesis-notes.md`: PASS (no prompt-body edit; 8th consecutive wave).**
- No 2+-seat core candidate this wave. The wave-10 offered=payable / colored-pays-generic family did NOT harden
  (deck135 no recurrence; deck109 hybrid class -> 0 under the card-note; the lone fresh witness deck140 vs109
  s9 is single-seat and bundled into headline 1's fixation event). The core body already carries the governing
  facts verbatim (line 4); the durable fix routes to REPRESENTATION (option-line hybrid + affordability
  annotations). All seven seats voted core PASS.
- `general-strategy.txt` is a byte-identical copy of the live core prompt (verified via `cmp`).

## Wave-10 changelog (each reviewer proposal: adopted / adapted / rejected + why)

**SKILL — adopted (multi-seat, cross-cutting, or completes a method line):**
- deck135 P1 + deck140 P1 + deck131 closure + deck110 V1 *verify a shipped REPLY-PROTOCOL change by the
  STRUCTURAL head signal (all replies non-numeric-headed + last-bare-int-differs-from-head = 0), NOT the
  old protocol's fuzzy metric; a diagnostic built for the prior reply SHAPE mis-scores the new one in both
  directions, so re-derive its parse before comparing counts across a protocol boundary; a reversal-flag is
  a pointer to READ, never a defect count; and verify the witnessed defect-class to zero at the witnessing
  seat with reasoning-vs-outcome split* — ADOPTED (4 seats) as wave-10 method headline 1 + folded into the
  Step-2 reply-protocol block (extends the verification-metric-SPLITS rung to a protocol CHANGE).
- deck140 P2 + deck135 (notes) *the answer-after-plan protocol's THIRD reply-protocol shape — trailing
  PLAN/CHOICE mismatch that casts-a-SPELL-over-a-HOLD asymmetrically, with CORRECT preceding reasoning; the
  model NAMES its choice in prose so the A/B arm reconciles the trailing index against the named action
  (dominates accept-last-bare-number)* — ADOPTED (2 seats) into the Step-2 reply-protocol taxonomy; the
  head-first intent-collapse class is recorded VERIFIED DEAD at all 7 seats (A/B shipped this corpus).
- deck133 P1 + deck44 P1 *an option-line annotation (and any confirmed-fired representation) can be
  OBEYED-IN-REASONING yet leak at the number/parse/truncation layer — score it in the REASONING, reported
  separately from the recorded CHOICE; check every recorded mis-cast reached a `CHOICE:` line before
  counting it against the annotation* — ADOPTED (2 seats) into the Step-0 confirmed-fired terminus (the
  "mis-cast -> 0" signal gains a "measured in the reasoning, not the recorded choice" qualifier).
- deck110 P1 *a COUNT-AGREEMENT representation fix succeeds by surface-agrees + OUTCOME-NEUTRAL, NOT by the
  false belief leaving the reasoning; keep a non-outcome-affecting reasoning residual OFF the guide* —
  ADOPTED (single-seat, 2+ waves, rung-completing) into the Step-0 terminus as the count-agreement shape's
  verification signal.
- deck109 P1 *before routing an offered=payable / can't-pay fabrication to representation or core, check
  whether the SPECIFIC card-cost fact is taught where the pilot reads it; the generic interface contract
  does not inoculate against a specific-mechanic misread; try the sibling-parallel card-cost fact FIRST* —
  ADOPTED (single-seat, refines the offered=payable ladder) into the Step-0 terminus as a ladder rung.
- deck110 P2 *a no-op / low-value marker on a REPEATABLE move/attach action must be computed from the
  OUTCOME DELTA, never from target-IDENTITY — an identity-keyed marker is blind exactly where multiple
  equivalent sources/targets exist* — ADOPTED (single-seat, 2+ waves, multi-copy-churn watch now fired
  COSTLY) as a representation-rung method note; the actionable lever is ENGINE (ledger), NO guide line.

**SKILL — recorded as ledger/watch (single seat, additive but not a method cut):**
- deck109 P2 + deck44 P3 (notes) *distinguish a SPIRAL/LOOP truncation from a LENGTH-CUT truncation — a
  bigger token cap fixes the cut, not the loop; a loop indicates a false-belief-vs-contract contradiction
  to fix at the SOURCE* — WATCH (single-seat); recorded in synthesis-notes, harness-layer.
- deck110 P3 + deck133 (notes) *answer-after-plan plan/choice mismatch when the PLAN fixates on an UNLISTED
  card and emits a poorly-mapped fallback contradicting its own sub-conclusion* — WATCH (single-seat
  reply-protocol sub-variant); promote with a 2nd seat.
- deck44 P2 *a representation can fire INCORRECTLY (a mis-annotation) and be robustly OVERRIDDEN — that is
  an engine-ledger item, not a validation and not a licence for compensating guide text* — recorded as the
  "verify a representation fired" terminus's fired-but-WRONG sibling; ENGINE ledger #1, no guide/skill cut.

**SKILL — no-change proposals confirmed (reviewers validated existing method — six of seven FROZE
byte-identical; deck109 added one card-cost line):** deck44 (counter-#2 tightening validated 15/15;
named-target GFTT annotation killed the wave-9 fabrication class; 7th consecutive freeze), deck109 (blocker
re-anchor OBSERVED OBEYED 2/2; one hybrid-cost line added, spine frozen + freeze-checked), deck110
(re-attach marker 31/0 a 4th wave; artifact-tag 177/177; turnaround/variance freeze held a 5th wave),
deck131 (mulligan intent-collapse CLOSED under answer-last 2/2; Mountain-first 10/10; OBEYED-BUT-LOSING
6th wave -> FREEZE byte-identical), deck133 (empty-hand Thoughtseize annotation obeyed 100%; GM bare-N
confirmed; ZERO-edit frozen diff), deck135 (develop-branch VALIDATED; Force-of-Negation NEVER-list held;
byte-identical), deck140 (sweep trigger validated a 3rd corpus; adjudicate-by-life-at-cap confirmed;
byte-identical). No method line was cut.

**CORE PROMPT — see `synthesis-notes.md`: PASS (no prompt-body edit; 7th consecutive wave).**
- The one 2-seat convergence (offered=payable false-belief family: deck135 colored-pays-generic + deck109
  hybrid-misread) is REFUSED promotion because the core body ALREADY carries both governing facts verbatim
  (line 4: "Hybrid symbols like {U/B} can be paid with EITHER of the two colors, and any color can pay a
  generic cost"). A pilot disobeying a present universal fact is the representation-beats-instruction wall;
  a louder/third copy is the sanctioned-against escalation. Both witnessing seats voted core PASS. Durable
  fix routes to REPRESENTATION (option-line hybrid-cost + colored-pays-generic affordability annotations).
- `general-strategy.txt` is a byte-identical copy of the live post-A/B core prompt (verified via `cmp`).

## Wave-9 changelog (each reviewer proposal: adopted / adapted / rejected + why)

**SKILL — adopted (multi-seat, cross-cutting, or completes/refines a method line):**
- deck109 #2 *pause-aware demotion precondition — demote a guardrail only once its class was OBSERVED
  OBEYED, never merely ABSENT; a closing wave freezes untested guardrails against known game-losers* +
  #1 *thin-sample re-anchor validation is PER-CLASS against the windows that occurred* — ADOPTED into
  the Step-0 demotion terminus. The wave's method headline.
- decks 131/135/140/109 *reply-protocol INTENT-COLLAPSE is now MULTI-SEAT — the A/B is DUE, the
  layer-routing is fixed doctrine (never re-word/score a guide over it), + the reasoning-vs-outcome
  metric SPLIT (deck131) + the NON-NUMERIC-HEAD shape (deck109)* — ADOPTED into the Step-2 diagnostic;
  escalated from WATCH to A/B-DUE. Still NOT a core prompt-body edit.
- deck135 P1 *an else-branch develop-ORDER must gate on the OPTION LIST not the HAND, and name every
  SHAPE (directly-castable fix AND fetch-crack), de-biased from one color* + P2 *a fixed-card-cost
  reactive spell's exclusion is the ECONOMY rule, naming each 1-for-1 class* — ADOPTED into the Step-5
  else-branch bullet (deepens wave-8 else-ORDER + wave-6 free-reaction-costs-a-card).
- deck44 P1 + deck133 #3 *OPTION-LINE ANNOTATION as a fourth confirmed-fired-representation shape,
  verified by mis-cast->0 AND a REASONING-TAX DROP; the last rung before the number when a surfaced
  fact is fabricated over; demote only on conditional-surface completeness* — ADOPTED into the Step-0
  terminus (2 seats).
- deck44 P2 + deck140 Refinement 2 + deck140 Refinement 1 *NAME the invented anti-pattern in three more
  shapes — inverted-rationale priority entry (deck44), resurrection-in-a-new-guise (deck140), and the
  BOUNDARY-VALUE-misresolve = wording defect not disobey-wall distinction (deck140)* — ADOPTED as a new
  Step-5 bullet (extends the "name the anti-pattern" move; 2+ seats on the shared rung).
- deck133 #2 *a representation that renders for the OPPONENT of the reviewed deck is verified at the
  OPPONENT's seat* — ADOPTED into the Step-0 verify-FIRED terminus (extends composition-seam-across-
  seats).

**SKILL — recorded as ledger/watch (single seat, additive but not a method cut):**
- deck110 E3 *tag-completeness — a READ instruction over a surfaced value only PARTIALLY kills a
  re-derivation belief; the durable fix is an engine tag-completeness so a re-derived count AGREES with
  the summary in both directions* — the actionable lever is ENGINE/REPRESENTATION (parked ledger #3),
  not a guide/skill edit; the method point (verify BOTH the summary AND the per-item tags agree, not
  just the summary offset) refines the prefer-surfaced-number rung and is recorded in synthesis-notes.
- deck135 OBSERVATION 1 *colored-mana-pays-generic / offered=payable* — PARKED core candidate (1 seat);
  guide-patched; promote on a 2nd seat. (Appendix.)

**SKILL — no-change proposals confirmed (reviewers validated existing method — all seats FROZE or
near-froze):** deck44 (own-targets annotation win; plan-binding CLOSED the plan/choice-mismatch class
0 over ~65 windows; frozen spine 6th wave), deck109 (blocker re-anchor validated 3/3 where windows
existed; Pillar {R} corrective 0 false-cost; byte-identical frozen guide), deck110 (re-attach marker
47/0 a THIRD wave; turnaround/variance freeze held a 4th wave), deck131 (strict mulligan gate validated
at the REASONING level, reasoning 1/1 keepable 5/5; land-menu 9/9 a 2nd corpus; OBEYED-BUT-LOSING 5th
wave -> FREEZE; one freeze-safe de-specification of a corpus-stale self-citation), deck133 (GM bare-N
confirmed on a 3-cast multi-role sample; Obliterator rider verified cross-seat; ZERO-edit frozen diff),
deck135 (develop-imperative direct case -> 0; livelock retired; constrained-diff for the fetch-crack
residual + Force discard exclusion), deck140 (sweep trigger validated a 2nd corpus; boundary + BSZ +
invented-phrase constrained diff; timeout triage separated 3 latency-timeouts from 3 real losses).
No method line was cut.

**CORE PROMPT — see `synthesis-notes.md`: PASS (no prompt-body edit; 6th consecutive wave).**
- No 2+-seat core candidate. `general-strategy.txt` is an unchanged copy of the live post-A/B prompt
  (0 anchor references verified).
- PARKED/REJECTED for core (single-seat -> watch, or wrong layer): colored-mana-pays-generic (deck135,
  1 seat, parked pending 2nd seat); reply-protocol intent-collapse (reply-protocol layer, A/B due, not
  prompt-body); combined-lethal (deck133, pilot self-bridges, risks counter-bait).

## Wave-8 changelog (each reviewer proposal: adopted / adapted / rejected + why)

**SKILL — adopted (multi-seat, cross-cutting, or completes a method line):**
- deck140 P1 + deck131 #1 + deck133 #1 *the confirmed-fired-representation DEMOTION terminus
  GENERALIZES across fix shapes (weak-marker / absent-option / re-anchor-to-count) with shape-specific
  verification signals; the DELETION end propagates across guides* — ADOPTED (3+ seats) as the wave's
  headline, folded into the Step-0 verify-FIRED terminus. A re-anchor-to-a-count and a surfaced-count
  representation are the same fix at two layers.
- deck140 P2 *demote a belief-kill block PER-BELIEF against what the representation literally surfaces*
  — ADOPTED into the same Step-0 terminus (a count carries "how many," not "how your spell interacts";
  shroud survives, tapped/artifact demote).
- deck140 P3 *validate a re-anchor by defect-class->0 AFTER the castability/intra-turn join* + P4 *a
  validated re-anchor = turnaround mode, constrained diff not new fire conditions* — ADOPTED into the
  Step-0 terminus (extend confirm-before-accuse + turnaround-preservation).
- deck110 P1 *a NEW engine-surfaced value re-opens the manual-recompute audit — run it against every
  value the engine STARTS surfacing this wave* — ADOPTED into the prefer-engine-surfaced-number bullet.
- deck135 P1 *an else-branch of a priority rule needs its own IMPERATIVE scoped to a countable
  condition, not a permission, and must NAME the anti-pattern the executor will invent* — ADOPTED as a
  new Step-5 bullet; the displacement-into-the-else-branch third witness (completes wave-5 DISPLACEMENT
  + wave-7 deck140 "state both branches together").
- deck131 #2 *a keep-side GUARDRAIL can INVERT into a false KEEP — strict-ordered gate, subject the keep
  on the scarce resource, no keep-side clause names a standalone payoff* — ADOPTED as a new Step-5
  bullet; corrects the wave-7 deck131 P3 line that added the guardrail (re-check both directions).
- deck44 P1 *score a VALUE-FLOOR rule by DECISION-CORRECTNESS, not raw usage-rate* — ADOPTED into the
  Step-2 offered-vs-taken tally (mirror of "count OFFER windows"; aligns with the USER EVALUATION
  DOCTRINE — the unit is the decision).

**SKILL — recorded as watch / diagnostic (single seat, additive but not yet cross-cut):**
- deck135 P3 *reply-protocol intent-collapse (answer-before-reasoning)* — ADOPTED as a Step-2
  diagnostic distinction under PLAN/CHOICE MISMATCH (route to reply-protocol, never score against a
  guide); the durable fix is a reply-protocol A/B. WATCH.

**SKILL — no-change proposals confirmed (reviewers validated existing method):** deck44 (P2 plan-binding
CLOSED the counter plan/choice-mismatch watch 0/15; P3 pre-computable combat-math carve-out), deck109
(verify-fired discipline extended past firing — #5 lethal math PRESENT-AND-LOSING; intra-guide-hoist
method reproduced at a 2nd seat re-anchoring the chump threshold to the surfaced line; body-decline watch
held at 1 seat), deck110 (full trichotomy lifecycle closed inert->fired->obeyed->demoted->RE-VERIFIED;
OBEYED-BUT-LOSING + variance-entry test; META SHIFT as construction), deck133 (two-uses-two-triggers
finally exercised and WON — don't cut a low-density keyed trigger; freeze-mode sizing held to a 1-edit
diff), deck135 (P2 add a THIRD mana-loss bucket "fixing in-hand-offered-and-DECLINED = piloting";
join-the-board-before-counting cut a 13->4 tally), deck131 (OBEYED-BUT-LOSING + precondition-counting
held a 4th wave; stale winning-line self-citation must be re-checked each wave), deck140 (timeout triage
caught two non-guide timeouts; resolved-vs-asserted on a countered finisher; reach residual ->
construction). No method was cut.

**CORE PROMPT — see `synthesis-notes.md`: PASS (no prompt-body edit; 5th consecutive wave).**
- The scheduled `hand -> stack` anchor A/B RAN; removal ADOPTED and LIVE (commit 6d02cb68c). No
  regression. The core prompt is otherwise unchanged.
- REJECTED for core (all single-seat -> watch): `damage>=toughness=dead` (deck131); reply-protocol
  intent-collapse (deck135, reply-protocol layer not prompt-body); shroud-vs-non-targeted (deck140);
  tapped-attacker (deck140, now carried by representation).

## Wave-7 changelog (each reviewer proposal: adopted / adapted / rejected + why)

**SKILL — adopted (multi-wave, cross-cutting, or completes a method line):**
- deck110 P1 *a CONFIRMED-fired representation fix licenses DEMOTING the guide stopgap it replaced* —
  ADOPTED as the terminus of the Step-0 verify-FIRED bullet. Multi-wave (5→6→7); the first end-to-end
  close of the shipped-but-inert/weak/obeyed trichotomy (marker 29/28, no-op 50%→6%).
- deck110 P2 *read the REPLY of the lone counterexample before re-opening the representation* — ADOPTED
  into the same bullet; an intent-collapse at a correctly-represented option is a plan/choice mismatch,
  not a weak marker.
- deck109 *a deleted stopgap gets a next-wave regression grep for the misreasoning it patched* —
  ADOPTED into the verify-FIRED bullet (the deletion end of the stopgap lifecycle).
- deck131 P1 *run the representation rung on the MENU — a rule naming an unofferable option is a
  platform defect* — ADOPTED as the wave's Method headline 3 and into Step 2 (confirm the option is
  OFFERED at the decisions it targets). Engine ledger #1 (land-drop seam).
- deck131 P2 *separate not-drawn from in-hand-unofferable before attributing a mana loss to the
  decklist* — ADOPTED into the residual-attribution guidance.
- deck140 P1 *OVERSHOOT VIA FALSE-BELIEF BRIDGE — a widened hold is satisfied through a pre-existing
  false belief; kill the bridge inline or re-anchor to a hard count* — ADOPTED as Method headline 2.
- deck140 P2 *scope a corrective FACT to a COUNT, not a board ARCHETYPE* — ADOPTED (corollary of
  headline 2; extends "brake keyed to a visible count" to corrective facts).
- deck140 P3 *mid-calibration is RE-ANCHOR, not swing-back — state both branches of a hold as countable
  conditions together* — ADOPTED (corollary; the conditional-payoff-needs-its-ELSE applied to a hold
  whose ELSE is a fire trigger).
- deck140 P4 *do NOT read a raw HELD-count as overshoot without joining the board state* — ADOPTED into
  the digest discipline (confirm-before-accuse applied to a hold tally).
- deck44 P1 *a capability/target ABSOLUTE must never nest inside a MODE rule that later commands its
  opposite* — ADOPTED into the Step-0 intra-guide-contradiction bullet (fires at a life bracket / seam;
  the Obliterator block). Single seat but craft METHOD, generalizable, not a core edit.
- deck44 P2 *a per-deck stopgap for a deck-agnostic engine defect must be class-checked across SEATS* —
  ADOPTED into the stopgap lifecycle (grep ALL stderr for the class-migration; GFTT → Liliana/Fatal
  Push confirmed the prediction).
- deck133 P1 *resolved-vs-asserted narration check* — ADOPTED as a Step-2 harness-sweep class
  (crediting an effect that never resolved; `stack -> graveyard` = countered).
- deck133 P2 *a pilot that manually recomputes a displayed value is a representation defect even in a
  WIN* — ADOPTED into the prefer-engine-surfaced-number rung; win-only review misses it.
- deck135 *count OFFER windows, not just consecutive re-picks, when quantifying a re-offer defect* —
  ADOPTED into the waste-loop / re-offer measurement (two cost surfaces; 44-97 windows/game).
- deck131 P3 *a sharpened COUNTABLE rule can INVERT — anchor to a literal STRING-PRESENCE test + a
  keep-side guardrail + track both error directions* — ADOPTED into the Step-5 count-brake guidance.

**SKILL — recorded as watch / per-deck (single seat, additive but not yet cross-cut):**
- deck135 *stale-plan-around-an-uncastable-card* — sub-note under plan-diverges-from-legality.
- deck133 P3 *discard threat-axis (card-bound vs width-bound clock)* — one-line watch in Step 1.
- deck109 *UNTOUCHABLE-BOARD matchup class* — exemplar for the route-to-deck-construction bucket.
- deck131 P4 *"never block with your one kill-condition creature" needs its own line* — the
  reflexive-block convergence in a new guise (mid-life, protected creature); per-deck, on the watch.

**SKILL — no-change proposals confirmed (reviewers validated existing method):** deck109 (OBEYED-BUT-
LOSING + ceiling-sizing steered a record DROP correctly; blocker floor + 0 desyncs held), deck133
(freeze mode produced a 5-clause diff on a 4/6 best-ever wave — no rewrite; two-uses-two-triggers,
timeout triage), deck135 (phantom-lethal anchor 0 recurrences; Force per-deck scoping obeyed 9/10;
variance-entry test; verify-fired-first; harness sweep), deck140 (timeout triage caught two non-guide
timeouts; harness sweep first; reach residual → construction; demote-the-winner on the Rakdos hold),
deck110 (attack-seam survival clause now testable and fired; META SHIFT re-confirmed as construction),
deck44 P3 (three-corpus counter-seam success; counter-value floor held). No method was cut.

**CORE PROMPT — see `core-prompt-revision.md`: NO prompt-body edits (PASS).**
- All seven seats returned no core edit; the only convergence (reflexive/mis-framed blocking, now 3
  guises) routes to REPRESENTATION (engine ledger #5), not the prompt.
- SCHEDULED: the `hand -> stack` cast-happened anchor A/B is GO for wave-8 — condition (2 consecutive
  clean no-op corpora) MET (0 cast no-ops all 7 seats, 2 waves; 353f1a150). Stays shipped until the A/B
  runs; exact old→new + measurement in core-prompt-revision.md §1.
- Threat-before-value (wave-5 Edit 2) CONFIRMED again (deck133/135/131) — keep verbatim.
- REJECTED for core (single-seat → watch): phantom-lethal-fixation → decline-a-listed-body (deck109);
  tapped-attacker-still-a-threat + shroud-vs-non-targeted mechanics facts (deck140); free-reaction card
  cost (deck135); mulligan false-positive / reflexive block-with-win-condition (deck131).

## Wave-6 changelog (each reviewer proposal: adopted / adapted / rejected + why)

**SKILL — adopted (multi-seat or completes a method line):**
- deck110 P1 *verify a shipped representation fix FIRED before crediting/re-diagnosing it* — ADOPTED as
  a Step-0 opener bullet. The wave's method headline: the re-equip marker shipped-but-inert (checked the
  wrong field) looks identical to shipped-but-weak in the win column; grep the corpus for the marker
  string first.
- deck140 P1 *intra-guide contradiction — a #1 rule under-fires because a sibling rule forbids its
  action* — ADOPTED as a Step-0 audit bullet. Concreteness beats prominence; widen the lower rule's
  exception, don't shout #1 louder.
- deck140 P2 *removing an engine guard resurfaces the behavior it masked — re-audit for it* — ADOPTED as
  a Step-0 bullet. Topical: 353f1a150 lifted the combo-hold hint-vetoes this wave.
- deck131 P1 *precondition-count before calling a prior-wave leak fixed* — ADOPTED into the Step-0
  "quantify obedience" step (rare-event-A/B-power trap; <5 precondition windows → stay at full
  prominence).
- deck131 P2 *size the residual lever's ceiling before the loud #1 slot* + deck110 P2 *sharpen the
  OBEYED-BUT-LOSING variance-entry test* — ADOPTED, merged into the OBEYED-BUT-LOSING Step-0 bullet.
- deck135 P2 *HALLUCINATED-BOARD PHANTOM LETHAL* — ADOPTED as a distinct Step-2 diagnostic-patterns
  bullet, routed to model-experiments/representation, on the convergence watch (single seat).
- deck44 P1 *VALIDATION-DROP (chosen recorded, heuristic played; stderr-only)* — ADOPTED as a fifth
  harness-defect-sweep class. Also flags the ENGINE menu-contract violation (targeted spell offered with
  no legal target) to route upstream.
- deck110 P3 *a survival rule at the block seam needs its attack-seam partner (block seam may not
  arrive)* — ADOPTED into the Step-2 attackers/blockers bullet.
- deck133 P1 *a conditional payoff at the TOP of a ranked list needs its ELSE stated* — ADOPTED into the
  Step-5 low-branching/ranked-list guidance.
- deck133 P2 *split two same-role cards that differ in self-cost so the brake keys on the one that pays*
  — ADOPTED into the Step-5 "Life-as-resource needs a BUDGET" bullet.
- deck110 P4 *META SHIFT — a rule winning because the opponent couldn't punish it, now losing because a
  platform fix made the punishment work* — ADOPTED as a Step-0 attribution note, routed to
  deck-construction/meta.

**SKILL — recorded as watch / per-deck (single seat, additive but not yet method):**
- deck135 P4 *a FREE reaction still costs a CARD; scope by card cost* — per-deck + Appendix watch.

**SKILL — no-change proposals confirmed (reviewers validated existing method):** deck109 (stopgap
lifecycle correctly retired the Hellrider guide fallback; per-seam positive-floor + negative-framing
ruling VINDICATED by prediction — 4 high-life blocks → 0; OBEYED-BUT-LOSING handled the 133 loss),
deck135 P1 (displacement fix landed on its ORIGIN deck without inverting — doctrine confirmed), deck133
(prefer-engine-surfaced-number + offset re-confirmed; turnaround mode; two-uses-two-triggers
untested-not-cut; timeout triage; transcribe-the-win), deck140 (X-menu value-anchored flip held 7/7;
harness sweep first; timeout triage caught a wave-5 mis-attribution), deck131 (threat-before-value
confirmed; attacker denominator; timeout triage; wins-as-mirrors), deck44 P2 (representation-beats-
instruction vindicated end-to-end at the stack seam, 2/27→13/14). No method was cut.

**CORE PROMPT — see `core-prompt-revision.md`: NO edits this wave.**
- The only 2-seat convergence (reflexive high-life blocking) routes to REPRESENTATION, not the prompt.
- Threat-before-value (wave-5 Edit 2) CONFIRMED working on 3 seats — keep verbatim.
- The core `hand -> stack` cast-happened anchor's removal condition is MET cross-deck (0 cast no-ops in
  all 7 seats; Hellrider fixed at source by 353f1a150) — KEPT for wave-7 as generic anti-repick
  insurance, on the removal SHORTLIST for a deliberate A/B.
- REJECTED for core (single-seat/per-deck): reflexive-blocking core sentence, free-reaction card cost,
  burn-prior override, mulligan-precedence reinforcement, attack-seam survival strengthening (deck110
  vs109 — single seat, per-deck reinforcement + watch).

## Wave-5 changelog (each reviewer proposal: adopted / adapted / rejected + why)

**SKILL — adopted (multi-seat or completes a method line):**
- deck135 P1 *displacement vs overshoot* — ADOPTED as a Step-0 sibling bullet. The wave's headline
  method contribution; a fix can be obeyed/correct/non-overshooting and still fail because the leak
  moved upstream.
- deck131 P2 *OBEYED-BUT-LOSING Step-0 mode* — ADOPTED. Fills a real gap (turnaround mode keyed only on
  improvement); forbids the reflexive rewrite a bad n≤6 record tempts.
- deck131 P1 *attacker denominator = engine's `A#.` offered list, not battlefield* — ADOPTED into the
  bundled-asks sub-bullet + digest guidance. Prevents a phantom subset-attack leak (false positive).
- deck44 P1 *PLAN/CHOICE MISMATCH (intent collapse)* — ADOPTED as a Step-2 sweep class, distinct from
  desync and plan-diverges-from-legality.
- deck44 P2 *log-tail-only facts are functionally hidden / fabricated over* — ADOPTED into the Step-0
  representation rung and the Step-5 tell pattern; the shipped ON THE STACK line is its template.
- deck110 P1 *re-offer three-part fix has a proven CEILING* — ADOPTED into the Step-5 re-offer block
  (read-then-disobey → stop escalating, route to representation).
- deck110 P2 *count-brake keys on the bounding resource, not the turn* — ADOPTED (unattached copies /
  unused charges, not a flat per-turn cap).
- deck140 P1 *a fixed-defect stopgap on a SURFACE can INVERT; success-while-disobeying is a flip tell;
  prefer value-anchored over positional* — ADOPTED into the Step-0 stopgap re-check + the TEACH THE
  MENU pattern (step 3 flipped to value-anchored; wave-6 X-teaching must FLIP, not merely keep).
- deck140 P2 *menu-adjacency index carryover* — ADOPTED as a third numeric-offset shape (route to
  engine ledger; don't write guide lines).
- deck133 P1 *prefer engine-surfaced number, verify its offset* — ADOPTED next to TEACH THE MENU.
- deck133 P2 *two distinct uses need two distinct keyed triggers* — ADOPTED into "Give arithmetic."
- deck135 P2 *two zero-offer cases (never-drawn vs in-hand-uncastable)* — ADOPTED into "Never-drawn ≠
  validated."
- deck135 P3 *a ROLE-CHECK must name the SEAM it fires at* — ADOPTED into the Step-1 role-check bullet.
- deck135 P4 *utility/mana-dork carve-out inside the attack floor* — ADOPTED into the Step-5 act-floor.
- deck109 *a bare DON'T at a decision seam is the weakest placement* — ADOPTED as a compact line in the
  negative-framing ruling.
- deck131 *digest gotcha (isolate `Your battlefield:`)* — ADOPTED into "Extract, don't eyeball."

**SKILL — no-change proposals confirmed (reviewers validated existing method):** deck109 (stopgap
lifecycle, negative-framing prediction both fired correctly — no edit), deck133/135/140 explicit
non-proposals (tell audit, harness sweep, wins-as-mirrors, turnaround mode, threshold reuse). No
method was cut.

**CORE PROMPT — see `core-prompt-revision.md`:**
- Edit 1: RETIRE the counterspell offer-is-the-signal / log-tail tell; point at the shipped ON THE
  STACK section + labeled counter options; add the mana-discipline fact (window opens only when a
  response is payable). Supersedes deck44 #1 (which asked to EXTEND the obsolete tell).
- Edit 2 (CONVERGENCE, 3 seats): the act-floor is satisfied by the HIGHEST-impact play; deploy a threat
  before a same-turn value/ramp/dig spell (deck135 G1 + deck133 #2 + deck131 Prism Ring).
- Edit 3: name the ON THE STACK section in the "During the game you will receive" paragraph.
- REJECTED for core: deck109 reflexive-blocking (single-seat → watch); deck131 Guttersnipe exception
  (per-deck by construction).
