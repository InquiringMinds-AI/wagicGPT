# Wave-13 synthesis notes

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-183228` (wave-13b), 21 games round-robin over decks
44, 109, 110, 131, 133, 135, 140. Binary `/tmp/wagic-a75e2625e` (Force round-2 `eb5145082` + Force round-3
`a75e2625e` + auto-tap two-pass `d5d7b1f2e` + name-echo protocol `c3e6d95b2` + BLOCKS:none `42e8bc180` + 240s
timeout). Wave-11 guides all frozen; core prompt unchanged (8th PASS standing → 9th this wave). This is the
FIRST clean validation after wave-12 (`matchups-20260715-120540`) and wave-13a (`matchups-20260715-151947`)
were Spark-crash contaminated (engine-evidence only). Win table: 140:5/6, 109:5/6, 110:3/6, 44:3/6, 133:2/6,
131:2/6, 135:1/6. 0 raw timeouts; 13 life-adjudicated (a latency artifact — see below).

---

## (a) Wave-13 shipped-fix validation table

| Shipped fix | Binary | Verdict | Evidence (seat-attributed) |
|---|---|---|---|
| **Name-echo protocol** (`(name)` on every CHOICE, index-wins on conflict) | `c3e6d95b2` | **VALIDATED for PRESENT-option case; ABSENT-echo branch OPEN** | 982/982 CHOICE lines compliant (100.0%), **0 wrong remaps**; every echo-vs-index conflict on an OFFERED option resolved correctly (deck110 s7, deck131 163/163). BUT the echo-names-NO-option case is uncontrolled (see ledger #1). All 7 seats confirm compliance; deck110/deck131 explicitly vindicate index-wins on PRESENT conflicts. |
| **BLOCKS: none** (explicit all-decline declares no blockers) | `42e8bc180` | **CLEAN — 3 seats** | Parses clean at deck131 (deck109 s7/s12 `BLOCKS: none`/`BLOCKS: None`), deck109 (vs131 s25, NOT-lethal branch obeyed), deck135. **0 misparses of intended blocks.** The wave-11 bare-`BLOCKS: none`→unparsed→baka-blocks-Guttersnipe bug is CLOSED. Vindicates "never re-word the guide to paper a parser bug." |
| **Force round-3** (offer-legality gate + dead-end "exile a blue card" removed) | `a75e2625e` | **CONFIRMED at deck135 (owning seat), incl. correct decline** | **0 dead-end picks corpus-wide**; **1 legal alt-cast offer** (vs deck44 t8, GFTT on stack) correctly gated on a payable exile target and correctly formed; the model **DECLINED it correctly** (opponent was GFTT-ing its OWN creature → a Force would burn 2 cards to save the opponent's creature). 0 Force casts → 0 RESIDUAL-RACE this corpus. NOTE: the underlying engine RESOLUTION bug (free-alt-cost Force never counters, ledger #4) got NO new data and remains unvalidated — round-3 fixed the INTERFACE, not the resolution. |
| **empty_reply class** (wave-12/13a flood was Spark dying) | — | **CLOSED** | `empty_reply = 0` corpus-wide. The wave-12/13a empty-reply flood was Spark death, not a protocol defect. |
| **240s timeout** | — | **VALIDATED** | 0 raw game timeouts. 49 unparsed_reply (3.7%) = the benign long-ramble discard class (heuristic answered), all self-healing, none game-deciding. |
| **Auto-tap two-pass** (`d5d7b1f2e`, ManaEngine::planPayment) | `d5d7b1f2e` | **RE-VERIFIED UNFIXED (outcome-neutral this corpus)** | The two-pass landed in `planPayment` but live taps still come from `selectAutoTapProducers` (unpatched). deck44 all-basics fixture was clean; deck140 vs140 s39 Archmage {4}{u} tapped 4 Islands from a u4b4 base (stranding). deck140 corroboration. See ledger #5. |

**Latency regime (context, NOT a play regression):** Spark ran at GPU_MEM_UTIL=0.75 (crash-avoidance trial —
it PASSED, no crash, memory flat). Cost: decision latency med 11.2s / p90 87s / p99 185s under 3-concurrent KV
preemption. That latency drove **13/21 games to the 2400s cap → life-adjudicated**; it is NOT a decision-quality
regression. Adjudicated positions must be scored on merit — canonical fixture: **deck135 vs110 (adj1, 9/10,
T12) = "won-board-lost-adjudication"** — deck135 was 1 life behind but held an 8/8 trample Treefolk + a 1/1
deathtouch-flying Coatl BOTH attacking, opp at 10 with a lone TAPPED 5/2 blocker (heavy favourite next turn);
the clock cut it a turn early and the life tiebreak recorded a loss. This is the reference case for "was the
seat ahead for the RIGHT reasons" when clean-latency validation resumes.

---

## (b) Ranked cross-seat engine/harness ledger (rank by outcome-impact × breadth)

| # | Item | Seats / severity | Repro | Fix owner |
|---|---|---|---|---|
| **1** | **parseChoice ABSENT-ECHO branch — an echo naming NO offered option falls through to index-wins and executes an unintended in-list card.** Two mechanisms: parent-action echo at a target sub-menu, and a just-cast card after the option list shifted same-turn. | **3 seats (deck133, deck140, deck44); GAME-LOSING ×2** | deck133 vs140 seq45 (echo "Gray Merchant" not offered → index 1 = forbidden thin-hand Thoughtseize → −2 life, flipped a 1-life adjudication loss); deck140 vs44 s40-42 ("Play Badlands" fixation after land played → index 1 now *Cast* → dumped held Rakdos X=8 + Wrath into `creatures:0`); deck44 vs135 s24 (self-GFTT own creature) + vs133 s14 (cast against explicit refusal via index drift). | **Reply-protocol / parseChoice:** absent echo = STALENESS signal → route to heuristic / Cast-nothing, NEVER the raw index; also RE-ANCHOR indices after a same-turn cast. NOT a guide edit (guide's "confirm the card is in the list" was present-and-disobeyed). |
| **2** | **Attackers reply name→index gap — MIXED `ATTACK: A1, Name` silently DROPS the named attacker (non-fallback false success, evades window-count scoring); pure-NAME forms unparse to a heuristic.** | **4 seats (deck109, deck131, deck135, deck110); mostly OUTCOME-NEUTRAL** | deck109: 6 dropped declarations / 5 windows / 3 games (vs140 s7/s40, vs110 s10/s14, vs135 s8), combat-confirmed vs140 s7; deck131: 5/18 incl. an unparsed WIN-GAME alpha strike; deck135/deck110 corroborations (`ATTACK: Ornithopter, Signal Pest` unparsed; `A1, A2, Memnite` mixed-parsed). | **Attackers parser:** name→index reconcile arm mirroring parseChoice — after consuming A-indices, resolve residual NAME tokens against the eligible-attacker roster by unique-name match (respect eligibility) and UNION them in. Fixes the mixed-form drop AND upgrades the full-name form. |
| **3** | **Galvanic Blast metalcraft never fires — flat 2 damage always.** Card-script bug in `Res/sets/primitives/mtg.txt`: the `+2` metalcraft line uses the continuous `aslongas(artifact\|myBattlefield) damage:2 >2` STATIC-effect form for a ONE-SHOT damage, omitting `oneshot`, so it never resolves. | **1 seat (deck110); DECK-WINCON-BREAKING** | deck110 s21 repro + every Blast in all 6 games dealt 2, never 4 (checked against `Damage:` events; surface `Artifacts in play: you 4`). The deck's LETHAL CHECK counts 4/Blast — engine under-delivers the deck's reach. | **DATA FIX** in `mtg.txt` via the wagic-cardscript skill. VERIFY against real Oracle text FIRST (per project rule): Galvanic Blast deals FLAT 4 with metalcraft (not base-2 +2), so the fix is a metalcraft alternate one-shot, not an additive static. Guide is Oracle-correct — do NOT edit it. |
| **4** | **Force of Negation free-alternative-cost never COUNTERS** (the spell resolves anyway). The engine RESOLUTION layer under the interface. | 1 seat (deck135); re-flagged, no new data this corpus (0 Force casts) | No cast to observe; round-3 fixed only the offer-legality interface. Remains next-cycle's top RESOLUTION-layer item. | Engine (`MTGAbility`/alt-cost resolution). Interface (offer legality) now clean; resolution still unvalidated. |
| **5** | **Auto-tap color-stranding — `d5d7b1f2e` two-pass landed in `ManaEngine::planPayment` but live taps come from `selectAutoTapProducers` (unpatched).** | 1 seat (deck44) + deck140 corroboration; RE-VERIFIED UNFIXED, outcome-neutral | deck44 all-basics fixture clean vs deck140 s39 Archmage {4}{u} tapping 4 Islands from a u4b4 base. | Engine: port the two-pass (colored-needs-first) into `selectAutoTapProducers`. Harmless corpus = a clean loss-decoupled regression fixture. |
| **6** | **`Mana available` line hides dual-producible colors and contradicts the same prompt's offered-legal set.** | 1 seat (deck140) | vs109 s9: line read `{r}{w}{w}{w}{w}` (no black) while the SAME prompt OFFERED payable Cruel Edict `{1}{b}`; model trusted the line, DECLINED a mandated sweep; black was in fact available (s11 cast it next window). | Engine/representation: annotate option-line affordability from the SAME engine that generated the legal-move set, so "offered" and "Mana available" cannot contradict. Also the surface behind the parked colored-pays-generic decline. |
| **7** | **Rakdos's Return X auto-underpaid — no X-announce surfaced, engine paid a minimal X.** | 1 seat (deck140) | vs109 s43: intended lethal X=9 (opp at 9) fired for X=3 with 6 mana left in pool and NO X-menu decision logged (contrast vs44 s41 where the X-menu appeared). Converted a T30 lethal into a T32 adjudication. | Engine: ensure the X-choice prompt fires on EVERY X-cast and auto-tap MAXIMIZES X for a lethal-range spell. (Distinct from the name-echo X-menu mis-map — the tell is whether an X-menu `ask` record exists.) |
| **8** | **Stale `[attacking]` tag + INVERTED freeze annotation self-contradiction on the board-state line → reasoning-tax → TIMEOUT into heuristic (under the latency cap).** | 1 seat (deck135) | vs109 s19: a Treefolk-frozen Reckoner serialized `[tapped - untaps and can attack next turn] [attacking]` while absent from the A1-A3 list; model spent its whole reply re-deriving, hit the latency wall → `unparsed_reply` → heuristic block. Outcome-neutral (lethal regardless). | Engine/representation: fix the stale `[attacking]` tag on non-attackers and represent freeze state (not `untaps next turn`). WATCH for the timeout-consequence promotion (2nd seat). |
| **9** | **Forced sub-menu bare-list representation** — target/sacrifice sub-menus render as bare creature lists with no committing-spell/effect header and no cancel; the model calls them "glitches" and re-litigates. Also a FORCED-DISCARD menu rendered like a cast/stack menu. | 1 seat (deck44), 3 repros; deck140 corroboration (discard) | deck44 F2 (vs135 s24, vs133 s15 sacked a Bloodghast-class body, vs133 s19 Liliana sacrifice misread); deck140 vs133 s12 (Liliana +1 forced discard read as a stack response → CHOICE:0 → unparsed → heuristic). | Engine/representation: add a committing-effect header ("Choose the target for Go for the Throat you are casting" / "Liliana forces a sacrifice — not optional"), an own-vs-opponent `[your battlefield]` danger annotation, and list opponent targets first. |
| **10** | **Equip re-offer no-op marker computed from target-IDENTITY, not OUTCOME DELTA** (recurred milder). | 1 seat (deck110, E2); ledger-standing | deck131 s29 net-zero Cranial Plating relocation (non-outcome). | Engine: one predicate — suppress "Equip … targeting X" whenever the move would not raise X's resulting power above its current carrier's (catches equivalent-carrier bounce AND second-Plating-onto-equipped shapes). |

**Watch (below the ledger, single-instance, sub-promotion):** deck131 first-mulligan STEP-1 gate misfire
(vs135 s1 — a maximally-explicit guide line MIS-CITED by the model; the hand WON, so a variance-rewarded
misfire; 1 instance in 7 waves → model-adherence WATCH, hold the freeze); deck131 F4 blocker LEGEND omitted a
legal untapped blocker (menu-completeness defect masquerading as a guardrail violation — engine/representation,
0 game impact); model rules gaps (deck110 s21/s22 — believes burn permanently reduces toughness; inverted
Phyrexian Obliterator's sacrifice trigger and fired burn into it — MODEL-layer, flag only if cross-seat).

**Latency-regime note:** 13/21 adjudications were the GPU_MEM_UTIL=0.75 artifact, NOT play regressions. Include
the **deck135 vs110 "won-board-lost-adjudication"** fixture (above) as the canonical example when reading any
adjudicated result.

---

## (c) Guide disposition table (7 FREEZE, diff-verified)

All 7 diff-verified `cmp`-clean against the live `bin/Res/ai/baka/deckN_strategy.txt`:

| Deck | Bytes | Disposition |
|---|---|---|
| 44  | 12084 | **FREEZE** — byte-identical (10th wave) |
| 109 | 7711  | **FREEZE** — byte-identical (hybrid card-notes now engine-execution-confirmed; keep at full prominence, unshipped backstop) |
| 110 | 7931  | **FREEZE** — byte-identical (Galvanic Blast is a card-script bug, NOT a guide error — guide is Oracle-correct) |
| 131 | 9036  | **FREEZE** — byte-identical (8th consecutive) |
| 133 | 8960  | **FREEZE** — byte-identical |
| 135 | 13357 | **FREEZE** — byte-identical |
| 140 | 7364  | **FREEZE** — byte-identical (SWEEP trigger validated a 5th corpus) |

No demotions: the pause-aware demotion terminus requires OBSERVED-OBEYED with a replacing surface. The
untested guardrails (deck109/deck131 LETHAL-branch + Obliterator absolutes — 0 windows again; the raced-out
matchups gave no combat) stay at FULL prominence.

---

## (d) ROTATION VERDICT — rotation does NOT fire (loop CONTINUES)

Evaluated honestly against the three gates:

1. **Gate 1 — did the cycle surface only context (guide/prompt) changes, no non-context work?** FAILS. The
   cycle surfaced multiple non-context engine/harness changes, chief among them a GAME-LOSING one (ledger #1
   parseChoice absent-echo branch, 3 seats, ×2 game-losing), a 4-seat silent attacker-drop (ledger #2), and a
   deck-wincon-breaking card-script bug (ledger #3 Galvanic Blast). Rotation cannot fire while these are open.
2. **Gate 2 — are the guides at terminus?** MET (7/7 byte-identical freeze; adopt-a-revision bar held). This
   gate is satisfied and has been for several waves — but it is necessary, not sufficient.
3. **Gate 3 — is the core at standing PASS?** MET (9th consecutive; no 2+-seat candidate; the colored-pays-
   generic family produced zero actual offered-payable declines and did not harden).

**Verdict: rotation does NOT fire.** Two of three gates are met, but Gate 1 is decisively failed by the
engine/harness ledger. **What must clear for rotation to fire next wave:** the top engine/harness batch must
SHIP and be validated clean — specifically (i) the parseChoice absent-echo staleness route + index re-anchor
(ledger #1), (ii) the attackers name→index reconcile arm (ledger #2), and (iii) the Galvanic Blast card-script
DATA fix (ledger #3, Oracle-verified). When those ship and a subsequent CLEAN-LATENCY corpus shows the
absent-echo class → 0 (no unintended in-list casts), the mixed-form attacker drops → 0, and Galvanic Blast
resolving at 4 under metalcraft — with the guides still frozen and the core still PASS — Gate 1 clears and
rotation becomes eligible. A clean-latency re-run (GPU_MEM_UTIL back to ≤0.85, `-j 3`) is a precondition for
reading that validation, since this corpus's 13 adjudications mask decision quality.
