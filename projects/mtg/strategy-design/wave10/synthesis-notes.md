# Wave-10 synthesis notes — the RESUME/VERIFICATION wave (cold-resume record)

**Corpus:** PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-031825` — 21 games round-robin over
decks 44, 109, 110, 131, 133, 135, 140. Binary `/tmp/wagic-c8c054be8`, wave-9 guides deployed,
**answer-after-plan reply protocol** (scratch reasoning -> `PLAN:` -> final `CHOICE:/ATTACK:/BLOCKS:`
line), max_tokens 2048. Control baseline = wave-9 `matchups-20260714-094942` (head-first protocol; the
intent-collapse control arm, 120 detector flags — see `reply-protocol-ab.md`).

This is the first corpus after the 2026-07-14 pause. The four wave-9 parked ledger items ALL shipped
between corpora (commits c8c054be8 + fd64c6fc6). The wave's job was to VERIFY those shipped fixes
end-to-end, not to open new authoring diagnoses. **Dominant mode: FREEZE — six of seven guides are
byte-identical; only deck109 added one card-cost line.** Every method adoption is a verification
refinement (how to score a fix under a CHANGED reply protocol), not a new guide headline.

---

## 1. Corpus verdict — validation table (which shipped fixes were confirmed)

| Shipped fix | Verified? | Evidence |
|---|---|---|
| **Answer-after-plan protocol killed head-first intent-collapse** | ✅ at ALL 7 seats | Every reply non-numeric-headed (`PLAN:` first, no head token to lock) + `last-bare-int-differs-from-head = 0` (deck135 246/246; deck110 0 true head-vs-choice mismatches over 169 ask/priority). deck131: the wave-9 no-Mountain mulligan false-Keep did NOT recur (Mulligan 2/2, outcome caught up from wave-9's 0/1). deck109: the wave-9 non-numeric `none`-head attacker collapse did NOT recur (30/30 fully declared). |
| **Fetch consume-on-choose** | ✅ ~48% window drop, no strategic damage | Control (deck135) avg 21.3 crack-windows/game -> wave-10 66 total / ~11/game (deck135); deck133 ~3.3/game. Cracks still fire when the guide wants them (crack-for-missing-color both ways). Residual: a CHOSEN-but-unresolved fetch is re-offered across a few windows (decision-count inflation only, ledger #9). |
| **Thin-hand / empty-hand discard annotation** | ✅ obeyed 100% | deck133: option renders `- the opponent's hand is EMPTY: nothing to strip` / `- the opponent holds only N card(s)`; every empty-hand window with a real choice DECLINED Thoughtseize (deck109 s16/18/20; deck110 s13/15/19/22/25/27). Zero empty-hand seizes, zero fabricated-card replies. Closes the wave-9 fabrication class. |
| **Artifact-tag completeness** | ✅ 177/177 agree, zero offset | deck110: a re-derived per-line `[artifact]` count agrees with `Artifacts in play: you N` across 177/177 windows. (Residual reasoning fold-in persists but is OUTCOME-neutral — see ledger #10.) |
| **Legal-target names on targeted casts** | ✅ killed the deck44 s54 fabrication class | `Cast Go for the Throat {1}{b} - legal targets right now: <names>` renders; the wave-9 "is it an artifact? Wait..." self-negotiation appears NOWHERE this corpus (deck44 vs131 s8/s9, vs109 s6). Own-targets form fired 36×, 36/36 obeyed in reasoning. |
| **`[deals 0]` / blocker-marker + GM bare-N** | ✅ correct where windows occurred | Gray Merchant bare-N confirmed on both casts (deck44 s32 drains6 = +6/-6; deck110 s35 lethal). `[deals 0]`/not-lethal blocker markers fired at the blocker windows that occurred (5 across seats); no reflexive high-life block; blocker seams clean (deck109 2/2, deck110 4 windows). |
| **adj0/adj1 life-adjudication-at-cap** | ✅ rows sanity-checked | deck140's own wave-9 ask: vs110 t18 18/17 = `adj0 18 17`, vs131 t32 27/26 = `adj0 27 26` — the wave-9 ahead-but-timed-out grinds now bank as wins. deck131 adj rows match seat life (vs110 6==6, vs133 15==15). |

Net: all four resumed-ledger fixes VALIDATED; no shipped fix was inert or regressed.

---

## 2. Ranked engine / harness ledger (merged from all 7 notes.md — repros in the per-deck files)

Layer-routing: these live HERE, never in prompt/guide text. Ranked by severity × evidence.

1. **Force of Negation's FREE ALTERNATIVE COST never counters — the spell RESOLVES anyway and the pilot
   bleeds blue cards (HIGHEST; 2 seats).** deck135 ENGINE #1. REPRO A deck131 (Downsize s40 -> resolved
   6/10; Elixir s41 -> resolved to battlefield; Cyclonic Rift s51 -> bounced Treefolk; 3 Force casts, 0
   counters, 7 `exile a blue card` cost-actions). REPRO B deck140 (Rakdos's Return s58 -> resolved for 6
   to face, Force to graveyard). Two coupled faults: (1) the counter option renders the `- NO legal target
   right now` mis-annotation (see #3) even with a counterable spell on the stack; (2) with no target bound,
   Force resolves as a pure cost-payment. FIX: bind the stack noncreature spell as Force's target (the `can
   target on the stack: X` hint already knows it exists — the two code paths disagree) + suppress the
   no-legal-target annotation for counter options. The deck135 guide actively directs Force onto board
   wipes, so this is on the engine's critical path (until fixed, the guidance pitches needed blue cards —
   guide intentionally NOT changed, layer-routing). Fold in ENGINE #2 (Force offered when the only blue
   card is Force itself — unpayable alt-cost offered, `offered=payable` violation).

2. **Auto-tap greedily taps COLORED sources for GENERIC costs, stranding held counter mana — COST deck44
   the vs140 game (GAME-CHANGING; single-seat but a clean loss).** deck44 #2. s61: cast `Archmage of Echoes
   {4}{u}` with 6 Island + 4 Swamp untapped and Counterspell {u}{u} in hand; auto-tap spent 5 Islands for
   the {4} generic, leaving one blue -> s64 Blightsteel Colossus on the stack, Counterspell uncastable and
   NOT OFFERED -> Blightsteel resolved -> lost to infect. The guide's "leave two blue sources untapped" is
   UNEXECUTABLE (the pilot cannot choose which lands tap). FIX: auto-tap generic costs from colorless/
   off-color sources first, preserving colored sources (esp. when the player holds an instant of that color).

3. **`- NO legal target right now` mis-annotation on stack-targeting COUNTER options (shared root with #1;
   latent-but-overridden on ordinary counters).** deck44 #1 + deck135. The counter option renders the
   no-legal-target clause AND `can target on the stack: <name>` on the same line — self-contradictory
   (the no-legal-target logic inspects only permanent/creature targets, not a stack spell). deck44: 15/15
   counter windows OVERRIDDEN correctly (0 misplays) — LATENT risk (a weaker moment could obey the wrong
   text; this is exactly the class the own-targets annotation was built to prevent, now mis-firing on the
   counter path). FIX: suppress the clause when `can target on the stack:` is non-empty. Fixing this is on
   #1's critical path.

4. **Formless / truncated-reply parse EVASION of the `unparsed_reply` guard — two guard-gap shapes (2+
   seats).** The truncation-FAIL heuristic-safe path keys on a reply LEADING with `PLAN:`, so it misses
   (a) a formatless reply that dropped the `PLAN:`/`CHOICE:` format and is int-grabbed from prose (deck133
   deck110-seat seq20: `2` = self-Fatal-Push, a self-2-for-1), and (b) an answer-LAST reply that blows the
   2048 cap before any `CHOICE:` line and has a body digit SCAVENGED that contradicts the reasoning (deck44
   s68 scavenged `1` = self-GFTT while the derivation obeyed the annotation; deck140 s34 scavenged `1` =
   wasted Rakdos X=0). FIX: flag `unparsed_reply` (or `truncated_no_label`) whenever a reply lacks BOTH a
   `PLAN:` prefix AND a valid trailing answer label — never silently record a scavenged parse. Answer-LAST
   makes truncation costlier than answer-FIRST (the whole reasoning is discarded, not the head). 4096-token
   cap already shipped for future runs (helps length-cuts, NOT loops — see #6).

5. **Trailing PLAN/CHOICE mismatch — the answer-after-plan residual, casts-a-SPELL-over-a-HOLD (reply-
   protocol A/B refinement; 2 seats).** deck140 4/215 (2 wasted Rakdos kill shots, Pyroclasm-into-C0,
   Cruel-Edict-into-C0) + deck135. Reasoning PRECEDES and is CORRECT; only the emitted index is wrong, and
   it fails asymmetrically toward CASTING. Indexing verified 1-based (192/200) = a MODEL number-emission
   defect, not off-by-one. The model reliably NAMES its choice in prose, so the winning A/B arm reconciles
   the trailing index against the plan's NAMED action (or requires echoing the option TEXT, not a bare
   integer) — DOMINATES "accept the last bare number." Sub-variant (deck110 P3, single-seat WATCH): the
   plan fixates on an UNLISTED card and emits a poorly-mapped fallback contradicting its sub-conclusion.

6. **`tools/intent-collapse-metric.py` is uncalibrated for answer-after-plan — recalibrate before trusting
   its cross-arm counts (multiple seats: 135, 140, 110).** Under answer-LAST its head parser is vacuous
   (`non-numeric heads: 215/215`) and its reversal heuristic throws index-offset/mid-plan-aside FALSE
   POSITIVES (deck135 30 flags / 0 genuine; deck140 21 / ~all false, and it MISSED all 4 genuine
   mismatches). The naive cross-arm read (~135 wave-10 vs 120 control) would falsely register WORSENING.
   FIX: parse the trailing `CHOICE:N` as the answer; derive the plan's concluded option from its NAMED
   action with stronger option-text disambiguation; report a mismatch only when named != recorded. RELATED
   (deck109 B / deck131): a SPIRAL/LOOP truncation (the model re-states one contradiction ~12× until the
   cap) is distinct from a LENGTH-CUT truncation — a bigger cap fixes the cut, not the loop; a loop signals
   a false-belief-vs-contract contradiction to fix at the SOURCE (fixing #8 removed deck131's loop trigger).

7. **Multi-copy equip-churn no-op marker is keyed on TARGET-IDENTITY and blind to a NET-ZERO shuffle
   between EQUIVALENT instances (PROMOTED watch -> CONFIRMED COSTLY; MEDIUM; 2 games).** deck110 E2. deck109
   T8: ONE Cranial Plating bounced between TWO identical Ornithopters 4×/turn (net-zero, ~4 mana, in a loss
   at 3 life); deck131 T7 Skirge->Pest->Skirge. The `(ALREADY attached... change NOTHING)` marker fires
   only on the current holder, so each move to the equivalent OTHER carrier is UNmarked. FIX: compute the
   marker from the OUTCOME DELTA — mark/suppress "Equip targeting X" whenever moving the equipment to X
   would NOT increase X's resulting power above the current carrier's (equivalent-or-worse carrier); one
   predicate catches both the two-Platings shape (wave-9 E2) and identical-carriers (this wave). NO guide
   line (the guide already forbids it; the pilot has no surfaced gain signal to obey).

8. **Hybrid-cost option-line annotation (representation; cross-deck backstop).** deck109 ledger A. deck131
   seq18/19/21: the pilot read `Cast Rakdos Cackler {b/r}` as `{b}{r}` (black AND red), declined a payable
   creature (s19) / spiraled to the cap (s21). FIX: append a hybrid hint to any option whose cost carries
   `{X/Y}`, e.g. `Cast Rakdos Cackler {b/r} - hybrid: pay with {R} or {B}`. Covers every deck with a hybrid
   card in one place; the deck109 card-guide note is the interim. Owner: the option-text builder (same site
   as `{right now: ...}` / `- legal targets right now: ...`). ALSO note the colored-pays-generic class
   (deck135 core candidate, routed here): an affordability/payability breakdown on the `{N}{color}` option
   line is the durable representation lever (core stays PASS — see §5).

9. **Fetch chosen-but-unresolved re-offer residual (minor; decision-count inflation only).** deck135
   ENGINE #3: deck44 s5/s6/s7 chose `Put in Play with Misty Rainforest` 3× before it resolved at s8 — ONE
   crack, ONE life, ONE land (3 windows for 1 crack). If consume-on-choose is meant to withdraw the crack
   the instant it is CHOSEN it is not doing so for a pending activation. Low-impact.

10. **Artifact hand-card type-tag sub-gap + the non-outcome-affecting count fold-in (low).** deck110 E3.
    The `[artifact]` tag lives on the BATTLEFIELD line only, so a card in HAND carries no type tag and the
    pilot guesses (deck133 s17/18 mis-typed Etched Champion / Vault Skirge as non-artifacts). CANDIDATE
    micro-lever: tag artifact cards in the HAND serialization (`Cast Etched Champion {3} [artifact
    creature]`). The residual battlefield count fold-in (Glimmervoid/basics hand-folded into metalcraft)
    is OUTCOME-neutral (no metalcraft-gated pick flipped, no Blast fired expecting metalcraft-4 sub-3) —
    do NOT re-open with louder guide wording (representation-beats-instruction wall).

Reviewer-facing / minor: `resolved:` field still not emitted (deck133 #3 — `events` carries the deltas;
an explicit `resolved: battlefield|countered|fizzled` would remove the manual narration-scrape); attacker
name-vs-`A#` parse miss (deck133 #2, 1 instance, heuristic recovered); "no blocks" prose declaration on the
`BLOCKS:` line dropped to heuristic (deck44 benign). All game-neutral.

---

## 3. Guide dispositions (VERIFIED by diffing each wave10/deckN/strategy.txt against the live baka guide)

`diff bin/Res/ai/baka/deckN_strategy.txt strategy-design/wave10/deckN/strategy.txt`:

| Deck | Disposition |
|---|---|
| **deck109** | **CHANGED — one line added.** A Rakdos Cackler `{B/R}` HYBRID cost note in its KEY CARDS entry ("ONE mana that a Mountain pays... NOT a black card needing two mana... cast it; never decline it for 'lacking black mana'"), mirroring the already-obeyed Boros Reckoner `{R/W}` treatment. Interim for engine ledger #8; retire once the option-line hybrid annotation ships. |
| deck44 | FROZEN — byte-identical to live. |
| deck110 | FROZEN — byte-identical to live. |
| deck131 | FROZEN — byte-identical to live. |
| deck133 | FROZEN — byte-identical to live. |
| deck135 | FROZEN — byte-identical to live. |
| deck140 | FROZEN — byte-identical to live. |

`general-strategy.txt` = byte-identical copy of the live core prompt (`cmp` clean). See §5.

---

## 4. Deck-construction flags (restated for the user; user owns the decklists — NOT relitigated)

- **deck44 (UB tempo/control):** the all-reactive/creature-light slow keep is the loss signature (vs110
  fast affinity out-raced it T9; vs140 grind). Levers: black-source density vs the black spell load; 1-2
  more cheap flyers over a 3rd/4th reactive card. No answer to an infect ARTIFACT bomb (Blightsteel) —
  matchup fact. Loss ROTATES by matchup/draw (a variance signature, not a guide regression).
- **deck109 (RDW):** no answer to evasion (flyers/shroud/pro-red) — lost vs44 on the race, won vs110 same
  class; structural. No card advantage / mana sink — inherent to RDW. Do not change at 5/6.
- **deck110 (affinity):** few creatures drawn into a flood deck owns the 4-loss column. If ever tuned: trim
  0-power filler (Signal Pest 0/1, Ornithopter 0/2) and/or Mox/land count toward more mid bodies + reach/
  evasion-block redundancy.
- **deck131 (Guttersnipe spellslinger):** threat density 6/60 (3 Guttersnipe + 3 Young Pyromancer) cannot
  reliably present a clock; no non-incremental finisher / no reach vs lifegain (vs140 lost adjudication by
  ONE life — a single reach finisher likely flips it); manabase 14I/8M leans slightly wrong for two red
  win-cons. Sideboard already carries Spellheart Chimera + Aetherling as candidate swaps. 0/6, obeyed-but-
  losing 6th wave — wins come from CONSTRUCTION, not guide prose.
- **deck133 (Golgari devotion/drain):** no reach/flyer answer except Gray Merchant (GM density 4/60, no
  selection); no early blocker (Bloodghast can't block); discard-suite + fetch self-damage = close-race
  margin. Consider +1 GM / cheap selection / a flyer-defense source / a 1-copy Thoughtseize->Inquisition
  shift.
- **deck135 (Modern Snow):** the manabase strands the deck's own 2-color win creatures (Ice-Fang Coatl
  {G}{U}, Ohran Viper {1}{G}{G}, Treefolk {2}{G}{U}, Diamond Faerie {2}{G}{W}{U} off only 2 Snow-Island +
  2 Snow-Plains + fetches); slow clock is structural (Treefolk the only real closer, lands T6-T11). Raise
  effective double-pip sources or shave a pip. (The pilot-side underuse is fixed — develop-branch validated.)
- **deck140 (BR/grixis control):** reach starvation + no early speed bump is the ENTIRE loss column (vs109,
  vs133 both swept faithfully, held Rakdos correctly, out-raced). 14 sweepers is the right density — it
  needs TEETH, not text: (a) a cheaper/second recurring clock that survives a counter; (b) a 1-2 mana early
  lifegain/blocker vs aggro; (c) more untapped RED so Pyroclasm {1}{r} is a reliable turn-2/3 sweeper.

---

## 5. Core adjudication — PASS (no prompt-body edit; 7th consecutive wave)

**THE CENTRAL DECISION: the wave-9 parked "offered=payable false-belief" candidate reached 2 SEATS but core
stays PASS.** Two witnessing seats in TWO shapes:
- **deck135** (deck110 s21): declined offered `Cast Into the North {1}{g}` and `Cast Search for Azcanta
  {1}{u}` at `Mana available: {g}{u}`, reasoning "I lack the generic {1}... I cannot cast any creature" —
  the colored-pays-generic false belief; a same-seat recurrence of the wave-9 witness (vs140 s24 Astrolabe).
- **deck109** (deck131 seq18/19/21): read `{B/R}` as `{B}{R}` (needing both colors), declined a payable
  Rakdos Cackler (s19) / spiraled the budget quoting the offered=payable contract (s21) — hybrid-misread.

The FAMILY convergence (a specific false mana-payment belief overriding offered=payable) is REAL. Promotion
is nonetheless REFUSED on decisive ground: **the core prompt body ALREADY states both governing facts
verbatim** — line 4: *"Hybrid symbols like {U/B} can be paid with EITHER of the two colors, and any color
can pay a generic cost."* deck135's proposed core sentence restates "colored pays generic" (already
present); deck109's hybrid fact is already present. A pilot that read a prompt carrying these exact
universal facts and fabricated the false belief anyway is the representation-beats-instruction wall at FULL
strength — a louder/third copy of a present fact is the sanctioned-against escalation (positive-default
ruling stands). Both witnessing seats independently voted core PASS.

The durable fix is REPRESENTATION (project doctrine: representation beats instruction — a surfaced fact
carries the load a present-but-disobeyed sentence cannot): (a) the hybrid-cost option-line annotation
(ledger #8), (b) a colored-pays-generic affordability/payability breakdown on the `{N}{color}` option line.
The reply-protocol residuals (§2 #5, #6) own the code-appended reply-protocol A/B, never the core body.

`general-strategy.txt` is a byte-identical copy of the live post-A/B core prompt.

---

## 6. POOL-ROTATION verdict — the loop CONTINUES on this pool (rotation NOT triggered)

Per the user's standing rotation directive, three gates:
1. **Did this cycle surface no non-context (engine/harness) changes?** — **NO.** A substantial engine/
   harness batch exists (Force free-alt-cost never-counters #1; auto-tap color-stranding #2; the counter
   no-legal-target mis-annotation #3; formless/truncated-reply parse evasion #4; the answer-last trailing
   PLAN/CHOICE A/B refinement #5; metric recalibration #6; multi-copy churn outcome-delta marker #7; hybrid
   annotation #8). So the pool STAYS: **the loop continues on THIS set for at least one more cycle after the
   engine batch ships** (the engine work must land + be verified before rotating exposes those fixes to a
   fresh pool).
2. **≥2 loop passes on this set?** — **YES**, many (this is wave 10).
3. **General strategy unchanged in the last loop?** — **YES** (core PASS, byte-identical; §5).

Gates 2 and 3 are met, but gate 1 is NOT (engine items outstanding), so rotation does not fire this cycle.
**Verdict: keep the current 7-deck pool; ship + verify the engine batch (priority: #1 Force, #2 auto-tap,
#3 counter mis-annotation, #4 truncation-parse guard), then re-evaluate rotation next cycle.**

### Resume ordering for the next reviewer
1. Confirm the engine batch shipped (grep the next corpus for regression: a Force cast where the targeted
   stack spell EVENTS show a counter — not `stack -> graveyard/battlefield` — and no `NO legal target`
   clause on counter options; auto-tap preserving colored sources; no scavenged-digit mis-casts).
2. Re-verify the 4096-token cap landed (truncation fallbacks down; loops distinct from cuts).
3. Re-derive `intent-collapse-metric.py` for answer-last BEFORE any cross-arm count (§2 #6).
4. Watches to hold (do NOT promote without a 2nd instance): reasoning-loop truncation (deck109/131);
   deck110-P3 unlisted-card plan/choice sub-variant; deck131 attack-seam-partner-to-NEVER-block; Obliterator
   punisher-rider USE (still owed at the deck133 seat; guardrail KEPT at full prominence on zero windows).
