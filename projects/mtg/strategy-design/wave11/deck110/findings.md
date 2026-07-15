# Deck-110 (Etched Affinity) — Wave-11 game review (answer-after-plan, 4096 cap corpus)

**Corpus.** PRIMARY `matchups-20260715-072254/`, 6 deck110 seat translogs (round-robin over
44,109,110,131,133,135,140). Binary `/tmp/wagic-54a5a9d01` (wave-10 engine batch + max_tokens 4096
+ parse ramble guards + stack-target annotation fix + fetch pre-targeting keying), wave-10 guides
deployed (live `deck110_strategy.txt` = the wave-8/9/10 frozen guide, md5 `00849aa9`). Reply
protocol = **answer-after-plan** (scratch reasoning -> `PLAN:` -> answer LAST on a
`CHOICE:`/`ATTACK:`/`BLOCKS:` line). Card facts vs `bin/Res/sets/primitives/mtg.txt`; representation
facts vs the live prompts. Choices are **1-indexed**. Citations = seat opp + seq (+turn).

## Record: 3 WIN / 3 LOSS (variance/construction, NOT guide regression)

| seat opp | result | end life (my/op) | T | one-line cause |
|---|---|---|---|---|
| deck44  (Faerie Archmage)    | **WIN** (adj1) | 17 / 1  | 16 | flooded bodies + Blasts, opp adj to 1 |
| deck135 (Modern Snow)        | **WIN**        | 20 / -2 | 7  | went wide, Champion+Blast closed |
| deck131 (Mind Control)       | **WIN**        | 18 / -6 | 11 | Signal Pest + Skirge race, dominant |
| deck109 (Hellrider)          | **LOSS**       | 0 / 9   | 9  | raced down; T8 equip-consolidation burned the turn |
| deck133 (Phyrexian Asphodel) | **LOSS**       | -2 / 9  | 17 | few creatures, ground out |
| deck140 (Wipe Them Out!)     | **LOSS** (adj0)| 14 / 24 | 28 | Staff-of-Nin grind, behind at cap |

adj rows sanity-checked vs `results.tsv` (deck0 deck1 winner life0 life1 turn): `44 110 adj1 1 17`,
`140 110 adj0 24 14`, `135 110 1 -2 20`, `131 110 1 -6 18`, `110 133 1 -2 9`, `110 109 1 0 9` — all
consistent with seat life. Win/loss is variance + deck design (USER DOCTRINE, context only). The three
losses again share the one precondition the guide cannot create: **few creatures / ground-out into a
flood deck** (DC1). vs135 flipped LOSS->WIN vs wave-10 on identical guides (pure draw variance — it
went wide this time). **Sixth consecutive OBEYED-BUT-LOSING / freeze wave.**

---

## FOCUS 1 — MULTI-COPY EQUIP CHURN (ledger #4 / E2, unfixed): RECURRED, less costly than wave-10.

Wave-10 F1 graduated this watch to CONFIRMED-COSTLY on a pure net-zero A->B->A->B bounce between two
identical Ornithopters (3 wasted `{1}` in a loss). This wave the class RECURRED but in a **milder,
consolidation-shaped** form; **no pure net-zero identical-carrier bounce occurred**.

**20 equip picks corpus-wide.** Marker fully obeyed for its guarded case: **90 marked offers, 0 taken;
0 marked no-ops.** Two-Plating same-turn stacking onto the best carrier is the dominant multi-equip
shape and it was played CORRECTLY (marker obeyed, second unattached Plating identified):
- **deck140 s32/s33 T19** (Ornithopter x2) and **deck140 s46/s47 T25** (Etched Champion x2): both times
  the pilot equipped the FIRST Plating, then equipped the SECOND, unattached Plating to the SAME best
  carrier, explicitly declining the `(ALREADY attached... change NOTHING)` option ("I will not waste
  mana on the already-attached option"). This is Rule #1's sanctioned "two equips ONLY when you cast two
  Platings" — **correct, not churn.**

**The one costly instance — deck109 s25/s26 T8, my4/op10 (a LOSS):** two Platings, one on Ornithopter
(6/3), one available. In a single Main Phase 1 the pilot ran two `{1}` equips to CONSOLIDATE both
Platings onto Master of Etherium (a defensive blocker at 4 life) — while its reasoning at BOTH windows
believed the Ornithopter would keep a Plating and still attack as a 6/3 ("attack with my 6/3 Ornithopter
which already has a Plating"). By end of s26 the Ornithopter is stripped to base and the 6/3 flyer clock
is gone. Cost = **2 wasted `{1}` equips + a lost attacker in a race it lost at 4 life.** This is the
**two-Plating shape of E2** (the marker is keyed on target-identity and carries no outcome-delta signal,
so "move the second Plating onto a creature that already has one" reads as a real gain). Not a net-zero
bounce, but the same blind spot: the pilot has no surfaced signal that the move degrades the board.

**deck135 s11/s13 T5 (a WIN):** ONE Plating equipped to Memnite (s11), then moved to Ornithopter (s13,
Memnite marked ALREADY, pilot correctly took the unmarked evasive carrier). 1 wasted `{1}` (should have
equipped the flyer directly); the move itself is a legitimate ground->flyer upgrade. Minor, in a
dominant win.

**Quantify:** ~3 wasted `{1}` equips corpus-wide attributable to churn/confused consolidation, 2 of them
in the deck109 loss. **Class status: RECURRED, milder than wave-10.** The E2 engine lever (compute the
equip re-offer's no-op/low-value marker from the OUTCOME DELTA — power/board change — not target
identity) remains the right, unfixed fix; it catches both the two-Plating and the equivalent-carrier
shapes with one predicate. Owned by REPRESENTATION/engine (notes E2), **NOT the guide** — Rule #1 already
forbids the behavior and the pilot has no surfaced gain signal to obey it. GUIDE FROZEN.

---

## FOCUS 2 — ARTIFACT-TAG RE-DERIVATION STEADY; hand-card type-tag sub-gap PERSISTS (no new instance).

**Re-derived count AGREES with summary N in 211/211 windows, zero offset** (wave-10: 177/177). Every
artifact on the current "Your battlefield" line carries `[artifact]` (creatures + Darksteel Citadel +
Mox Opal + Cranial Plating); Glimmervoid + basics correctly UNtagged. The wave-9 E3 representation lever
stays CLOSED at the representation layer.

**Reasoning fold-in RESIDUAL persists (representation-beats-instruction WALL, now 4 waves), NON-outcome-
affecting.** The pilot still hand-recomputes and folds non-artifacts in — **21 Glimmervoid + 2 basic**
genuine over-counts (e.g. deck133 s6 "two glimmervoids in hand, which are artifacts"; deck44 s16 "4
artifacts (darksteel citadel, glimmervoid, cranial plating, signal pest)"; deck109 s15 "play the land
(island) to increase artifact count to 4"; deck140 s10 "play island... artifact count to 3"). Self-
corrections still appear (a small positive from the completer tags). **Outcome impact: NONE** — all 7
Galvanic Blast casts fired at surf>=3 (genuine metalcraft, correct 4 dmg; deck44 s21 & deck140 s6/s12 at
exactly surf3, still genuine); **no Blast fired expecting metalcraft-4 at a sub-3 board; no metalcraft-
gated pick flipped.** The deck floods past 3 too fast for the boundary to be the deciding fact.

**Hand-card type-tag sub-gap (wave-10 E3 candidate micro-lever): PERSISTS, no NEW mis-typing instance.**
Hand serialization now tags NONCREATURE artifacts (`Cranial Plating {2} [artifact]`,
`Galvanic Blast {r} [instant]`) but artifact CREATURES in hand still show only P/T
(`Etched Champion {3} (2/2)`, `Vault Skirge {1} (1/1)`, `Memnite (1/1)` — no `[artifact creature]` tag).
The wave-10 under-count instance (deck133 seq17/18 "Etched Champion is not an artifact... only Mox Opal
is an artifact") did **not recur clearly** this corpus (0 clean hand-card under-count hits). Sub-gap
unchanged; micro-lever candidate stands at low priority (non-outcome-affecting). Logged in notes E3, NOT
guide text.

---

## FOCUS 3 — UNLISTED-CARD FIXATION (wave-10 F2, single-seat): the FAILURE did NOT recur.

Wave-10 F2 was a single-seat plan/choice reconcile failure: at deck133 seq6 the plan fixated on casting
Cranial Plating (unlisted) and, forced to pick from {Blast, nothing}, wrongly fired the Blast it said to
hold instead of mapping to "Cast nothing." **This wave the analogous windows resolved CORRECTLY:**
- **deck131 s8/s11 T3**, options `[Cast Thoughtcast, Cast nothing]`: the plan concludes "I will cast one
  of the Cranial Platings now" (Plating unlisted) and correctly emits **CHOICE: 2 = Cast nothing** — it
  did NOT fire the unwanted Thoughtcast. This is the exact scenario wave-10 deck133 seq6 got wrong,
  played right. **Positive.**

**BUT the PARENT class — the answer-last trailing PLAN/CHOICE mismatch (cast the offered SPELL when the
plan concluded HOLD) — recurred ONCE, 2nd-seat reinforcement:**
- **deck140 s6 T1**, options `[Cast Galvanic Blast (metalcraft, 4 dmg), Cast nothing]`: the PLAN reaches
  the correct conclusion in prose — "**I will not cast Galvanic Blast now because I need to attack first
  ... holding the instant allows me to respond**" — then emits **CHOICE: 1 = Cast Galvanic Blast**, the
  card it just said to hold. This is the wave-10 synthesis Method-headline-2 shape (deck140's own class:
  reasoning precedes and is CORRECT, only the trailing index is wrong, failing ASYMMETRICALLY toward
  CASTING the offered spell over HOLDING). It is now witnessed at the deck110 seat too — a 2nd-seat
  reinforcement of the reply-protocol A/B (reconcile the trailing index against the plan's NAMED action /
  require echoing the option TEXT, not a bare integer). **NON-outcome-affecting** (T1 face-4 in a grind
  loss; a Galvanic-Blast-to-face line is defensible anyway). Route to the code-appended reply-protocol
  layer; **NOT guide-fixable.** Distinct from unlisted-card fixation (both options here are listed).

Label parse itself is clean: **0 true label-vs-choice mismatches** across 223 ask/priority records (every
`CHOICE: N` == recorded `choice`) — the s6 defect is a plan-prose-vs-emitted-index contradiction, invisible
to a label-parse check, exactly the class the reply-protocol A/B targets.

---

## FOCUS 4 — FROZEN GUIDE VERIFICATION: Artifacts-count reading HELD; all guardrail classes stable.

1. **Answer-after-plan protocol** — label present 216/223 (7 gaps = empty_reply timeouts, choice -1); 0
   true head-vs-choice mismatches; intent-collapse class STRUCTURALLY GONE (no pre-committed head). The
   one residual is the F2/deck140-s6 trailing hold-vs-cast mismatch (reply-protocol layer).
2. **Artifact tag completeness / count READ** — re-derived count AGREES with summary N in 211/211;
   Glimmervoid + basics untagged; artifact creatures + Darksteel Citadel tagged. READ instruction HELD in
   the common case; fold-in survives (representation wall, non-outcome). No new class of guide regression.
3. **Re-equip marker (Rule #1)** — 90 marked offers / 0 taken; 0 marked no-ops among 20 equip picks.
   Rule #1's guarded same-instance no-op class OBSERVED OBEYED again -> stays FULL prominence (the E2
   multi-copy leak is the marker's structural blind spot, an engine item). Freeze.
4. **Blocker seam** — CLEAN. deck110 was the aggressor in all 6 games; the only 3 "Blockers"-phase
   windows are its own attack-side priority PASSES (deck133 s10/s41, deck44 s44), no defensive block
   decision, no reflexive high-life block, no missed must-block that flipped a game.
5. **Defer / Glimmervoid gate** — the expected early land-window gate; no surprise deferrals.

## Corpus health
- cast-nothing 10/67 cast-decisions (15%; wave-10 18%, wave-9 10%) — within range, concentrated in the
  creature-light losses. No durdle regression.
- Fallbacks = 7, all **empty_reply** (choice -1, reply_len 0) = the known corpus-wide 120s-HTTP-timeout
  class (WAGIC_GPT_TIMEOUT 240s shipped next corpus). All 7 fell in DECIDED or WON positions (deck44 WIN
  x3, deck135 WIN x1, deck133 LOSS x2 at my-2, deck140 LOSS x1) — **NONE changed a game.** Distinct from
  wave-10's 2048-cap truncations (4096 let tails run; the remaining bite is the HTTP timeout, not the
  token cap).
- attackers exercised (19 records); equips 20 picks; blockers 3 (all own-side passes).

## HANDOFF (OBEYED-BUT-LOSING / frozen-line diff; 6th freeze wave)
1. **FREEZE the entire guide verbatim** (strategy.txt byte-identical, md5 `00849aa9` confirmed). No line
   earned a change; no guardrail earned a demotion (the churn class was OBSERVED VIOLATED -> Rule #1 stays
   full-prominence; the marker class OBSERVED OBEYED -> stays frozen).
2. Route to notes (not guide): FOCUS-1 multi-copy/consolidation equip churn -> engine lever E2 (unchanged,
   outcome-delta marker); hand-card type-tag sub-gap -> E3 candidate micro-lever (unchanged); the deck140
   s6 trailing hold-vs-cast mismatch -> reply-protocol A/B (M1, now 2 seats across waves); the
   losses -> construction/variance (DC1).
