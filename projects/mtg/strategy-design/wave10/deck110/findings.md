# Deck-110 (Etched Affinity) — Wave-10 game review (RESUME wave, answer-after-plan protocol)

**Corpus.** PRIMARY `matchups-20260715-031825/`, 6 deck110 seat translogs (round-robin over
44,109,110,131,133,135,140). Binary `/tmp/wagic-c8c054be8`, wave-9 guides deployed (the live
`deck110_strategy.txt` = the wave-8/9 frozen guide), **answer-after-plan reply protocol**
(scratch reasoning -> `PLAN:` -> answer LAST on a `CHOICE:`/`ATTACK:`/`BLOCKS:` line),
max_tokens 2048. Card facts vs `bin/Res/sets/primitives/mtg.txt`; representation facts vs the
live prompts. Choices are **1-indexed** in this corpus (`CHOICE: 1` = first option).
Citations = `<epoch-tag>` seat + seq (+turn).

## Record: 2 WIN / 4 LOSS (variance/construction, NOT guide regression)

| seat | opp | result | end T | end life (my/op) | one-line cause |
|---|---|---|---|---|---|
| ...deck44  | Faerie Archmage (44)    | **WIN**        | 9    | 20 / 0  | flooded bodies + Blasts, opp to 0 |
| ...deck131 | Mind Control (131)      | **WIN** (adj1) | 8/9  | 25 / 6  | Signal Pest + Skirge race, dominant |
| ...deck109 | Hellrider (109)         | **LOSS**       | 9    | -5 / 7  | got opp to 5 early, stalled for bodies, raced down |
| ...deck135 | Modern Snow (135)       | **LOSS** (adj0)| 11   | 7 / 16  | drew few creatures, out-tempoed |
| ...deck133 | Phyrexian Asphodel (133)| **LOSS**       | 13   | -1 / 21 | no creatures drawn, midrange blowout |
| ...deck140 | Wipe Them Out! (140)    | **LOSS** (adj0)| 18   | 17 / 18 | Staff-of-Nin grind, behind by 1 at cap |

adj rows sanity-checked vs seat translogs' final life: vs140 my17/op18 = results `adj0 18 17` OK;
vs131 my25/op6 = `adj1 6 25` OK; vs135 my7/op16 = `adj0 16 7` OK. All consistent.

Win/loss is variance + deck design (USER DOCTRINE, context only). deck110 swung 4/1/1 -> 2/6
between corpora on **identical guides** with no matchup change (vs133 WIN->LOSS, vs135 WIN->LOSS,
vs140 TIMEOUT->LOSS-by-1, vs131/vs44 WIN->WIN, vs109 LOSS->LOSS). The losses share one
precondition the guide cannot create: **few creatures drawn into a flood deck** (vs133 cast only
Galvanic Blast + Thoughtcast T0-T6; vs135/vs109 stalled for bodies). Standing construction axis
(DC1), not obedience. **Fifth consecutive OBEYED-BUT-LOSING / freeze wave.**

---

## HARNESS / PROTOCOL SWEEP (answer-after-plan = first-class review dimension)

### V1 — INTENT-COLLAPSE CLASS IS GONE at this seat (the protocol shift landed).
- **0 true head-vs-plan mismatches** across 169 ask/priority records: for every record carrying a
  `CHOICE: N` label, parsed `N` == recorded `choice`. Under head-first (wave-9 control) the defect
  was a committed head token the later PLAN reversed; **there is no pre-committed head now** — the
  answer is emitted AFTER the plan, so the structural cause is absent.
- **`intent-collapse-metric.py` is a FALSE-POSITIVE generator under this protocol.** It flags 135
  records corpus-wide / 16 at the deck110 seat as "reversal-language + concluded-option !=
  recorded choice," but every one inspected is a crude word-match artifact: the real `CHOICE: N`
  at the end agrees with the reasoning (e.g. deck133 seq13 "...cast Galvanic Blast targeting the
  opponent... CHOICE: 1"; detector matched option-0 words in the plan tail). Do NOT read the 135
  flags as intent-collapse; the control's 120-flag baseline is not comparable (different reply
  shape). The metric's OWN reply-protocol-ab.md caveat ("UPPER BOUND, false positives from
  future-tense planning") is confirmed dominant.
- **Confirmed positive (a would-have-collapsed window now resolves to the plan):** deck135 seq14
  T5 — plan "removing the Coatl is the only way to deal damage this turn. I will cast Galvanic
  Blast targeting Ice-Fang Coatl," `CHOICE: 2` = Cast Galvanic Blast. Wave-9's cousin (deck135
  vs131 s14 head `4`=Cast nothing, PLAN reversed) was the labeled intent-collapse; here the
  reversal had no stale head to fight. **VALIDATED.**

### V2 — LABEL COMPLIANCE high; the only gaps are truncations.
`CHOICE:` present on 163/169 ask+priority. The 6 misses are all `unparsed_reply` truncations
(reply cut at the 2048 cap before its answer line) — known corpus-wide, 4096 already shipped.
Per game: vs44 21/21, vs131 25/26, vs109 26/26, vs133 22/23, vs135 24/27, vs140 45/46. Attackers
`ATTACK: A1, A2` clean; blockers present. The one recorded "mismatch" (deck135 seq27, label 0 vs
choice -1) is a truncation fallback, not intent-collapse.

### V3 — DEFER = 2, both the deliberate Glimmervoid gate (EXPECTED, not a defect).
deck140 seq3 T1 and deck131 seq3 T1, both `deferred_to_heuristic`, nopts=0, early land window.
Matches the brief exactly.

### V4 — FALLBACKS: 13 total (11 unparsed_reply + 2 defer). NONE changed a game.
The 11 unparsed are token-cap truncations (reply_len 6.4k-7.5k chars, cut before the answer label;
known). Combat-seam truncations (deck109 seq31 attackers T8, seq32 blockers T9; deck133 seq24
blockers T11) all in already-lost positions (my3/my5, opp ahead). **Two fell in an EVEN game** —
deck135 seq10 (priority, 1 option) + seq12 (ask, 3 options) at T5 my19/op19 — dropping to
heuristic; deck135 was ultimately lost 7-16 but the heuristic picks are not provably wrong and the
loss traces to few creatures drawn (deck110 20-0'd this same opp in wave-9). Flagged, not a proven
flip. Both truncated replies were high-tax "can I only do one of these?" agonizing (same shape as
deck133 seq13) — the 2048 cap bites hardest on affinity multi-option windows.

---

## VALIDATION TARGET (brief NEW engine change) — ARTIFACT-TAG COMPLETENESS: SHIPPED & CLOSED.

Wave-9's headline REPRESENTATION lever (E3: tag EVERY artifact so a re-derived count agrees with
the `Artifacts in play: you N` summary) **shipped and works end-to-end**:
- Every artifact on the battlefield line now carries `[artifact]`: **artifact CREATURES** (Memnite,
  Ornithopter, Signal Pest, Etched Champion, Steel Overseer, Vault Skirge, Master of Etherium) AND
  the **artifact LAND Darksteel Citadel** AND Mox Opal AND Cranial Plating. Repro (deck140 seq20):
  `Island; Mox Opal [artifact]; Darksteel Citadel [artifact]; Cranial Plating {2} [artifact];
  Mountain; Glimmervoid` — Glimmervoid and both basics correctly UNTAGGED. Contrast wave-9, where
  artifact creatures + Darksteel Citadel were untagged.
- **Re-derived count (count of `[artifact]` tags on the "Your battlefield" line) AGREES with the
  summary N in 177/177 windows, ZERO offset.** The wave-9 E3 lever is CLOSED at the representation
  layer (surfaced count accurate + per-permanent tags now complete + they agree).

### RESIDUAL — the fold-in false belief SURVIVES in the REASONING despite complete tags
### (representation-beats-instruction WALL, 3 waves deep; still NON-outcome-affecting).

Complete tags did NOT kill the re-derivation error. The pilot still folds non-artifacts in when it
recomputes by hand instead of reading N:
- **Glimmervoid folded (LAND):** deck131 seq6 (surf `you 2`) "play glimmervoid to increase the
  artifact count to 3, activating metalcraft"; deck135 seq9/seq10 (surf `you 2`) "I now have 3
  artifacts (signal pest, cranial plating, glimmervoid), triggering metalcraft."
- **Basics folded:** deck140 seq24 (surf 3) "play the mountain to increase my artifact count";
  deck133 seq4 (surf 1) "artifact count to 3 (mountain, mox opal, ornithopter), activating
  metalcraft."
- **NEW error direction — hand-card mis-typing (UNDER-count):** deck133 seq17/seq18 (surf 1)
  "Etched Champion is not an artifact... Vault Skirge is not an artifact... only Mox Opal is an
  artifact." Both ARE artifact creatures. The `[artifact]` tag lives on the BATTLEFIELD line; a
  card in HAND carries no type tag, so the pilot guesses its type and errs. A NEW representation
  sub-gap the tag-completeness fix does not reach (hand cards are untagged).

**Outcome impact this wave: NONE proven (third wave running).**
- **No Galvanic Blast fired expecting metalcraft-4 at a sub-3 board.** All 12 Blast casts: 9 at
  surf>=3 (genuine metalcraft, correct 4); the 3 at surf<3 (deck109 seq5 surf1, deck133 seq6/seq13
  surf1-2) each explicitly acknowledged "2 damage without metalcraft" in the reply — no fold-in
  flip. The deck floods past 3 so fast the metalcraft boundary is rarely the deciding fact.
- Metalcraft-at-surf<3 assertions are overwhelmingly correct FORWARD projections ("cast X to reach
  3"). The genuine present-tense over-counts (deck135 seq9/10, deck133 seq4) sat on decisions (cast
  Master of Etherium / cast a {0} creature) whose correctness does not depend on metalcraft, so
  none flipped.
- **Self-corrections now appear** (a small positive from the completer tags): deck135 seq29
  "mountain (no, mountain is a land, not an artifact. wait... not an artifact.)"; deck140 seq9
  "wait, metalcraft requires *my* artifacts. let's recount."

**Doctrinal reading:** the number is surfaced, accurate, Glimmervoid-clean, the per-permanent tags
are now COMPLETE, the guide POINTS at N and flatly forbids the fold-in — and the pilot still
re-derives and mis-tags. Representation-beats-instruction wall. The wave-9 lever (complete tags)
landed as designed and did NOT eliminate the reasoning artifact; it only made re-derivation AGREE
when the pilot does read the board, and enabled self-correction. Non-outcome-affecting, so no
further lever is worth pulling. **GUIDE: FREEZE** — escalating the READ wording would be the
escalate-at-a-wall anti-pattern. The hand-card type-tag sub-gap is logged in notes as a candidate
representation micro-lever, NOT guide text.

---

## NEW FINDING F1 — MULTI-COPY EQUIP CHURN RECURRED COSTLY (graduates wave-9's watch to confirmed).

The carried "multi-copy equip churn / re-equip no-op" watch — CLEAN in wave-9 — **fired as a
costly loop this corpus**, via a variant the per-instance marker structurally cannot catch:
bouncing ONE equipment between two EQUIVALENT carriers.

**deck109 (...deck109) T8, seq27-30, my3/op9 (a LOSS):** deck110 controls ONE Cranial Plating and
TWO identical Ornithopters (both base 0/2, both `[counters: 2x +1/+1]` = 2/4). In a single Main
Phase 1 it equips the Plating **four times**, bouncing between the two Ornithopters:
- seq27: Plating on Steel Overseer (8/3) -> equip to Ornithopter-A (defensible: trades raw power
  for a flying/evasive carrier at 3 life needing reach).
- seq28: on Ornithopter-A -> `CHOICE: 3` = equip to Ornithopter-B (that option was UNmarked; the
  marker fired only on Ornithopter-A "ALREADY attached").
- seq29: on Ornithopter-B -> equip Ornithopter-A. seq30: on Ornithopter-A -> equip Ornithopter-B.
Net board change of seq28-30 = **zero**; cost = **3 extra {1} equips** in a turn deck110 needed to
ATTACK, at 3 life in a race it lost. The `(ALREADY attached... change NOTHING)` marker cannot flag
this — each move is to a genuinely different instance, never a same-target no-op.

**deck131 (...deck131) T7, seq24-25, my18/op14 (a WIN):** Plating on Vault Skirge -> seq24 equip
to Signal Pest -> seq25 equip back to Vault Skirge. A->B->A net-zero within one turn, ~2 mana. Less
costly (winning), same class.

Re-equip marker itself is FULLY OBEYED for the same-instance case it guards: **31 marked offers, 0
taken; 11 total Equip picks, 0 marked no-ops** (Rule #1's core class OBSERVED OBEYED again — freeze
it). The leak is the marker's structural blind spot (equivalent-carrier / multi-copy bounce),
owned by REPRESENTATION/engine, not the guide (notes E2, sharpened & PROMOTED). Rule #1's "equip
again only when... a strictly better carrier arrived" was violated in spirit (an identical
Ornithopter is not "strictly better"), but the pilot has no surfaced signal that target-B is
equivalent — escalating the guide hits the same wall.

## NEW FINDING F2 — a PLAN/CHOICE MISMATCH shape survives the new protocol when the plan fixates
## on an UNLISTED card (reply-protocol/model layer, not guide).

**deck133 (...deck133) seq6 T0, options `[Cast Galvanic Blast, Cast nothing]`:** the PLAN reasons
entirely about **casting Cranial Plating** (not in the option list) and concludes "**I will hold
Galvanic Blast for a later finisher or removal**" — then emits `CHOICE: 1` = **Cast Galvanic
Blast**, the exact card it said to hold. The intended action (Cranial Plating) is unlisted; forced
to pick from {Blast, nothing}, it should map "hold Blast" -> "Cast nothing" (option 2) but fired
option 1. NOT head-first intent-collapse (the answer is after the plan); it is
plan-diverges-from-legality + a failure to reconcile the plan's conclusion against the actual
option list. Distinct from V1 (there head==choice==plan; here choice CONTRADICTS the plan's own
stated sub-conclusion). Single instance; route to reply-protocol / model-experiments layer.

## WATCH — Galvanic Blast burned as filler to satisfy "cast nothing = lost turn" (n=2, non-outcome).

deck133 spent BOTH Galvanic Blasts by T4, each for 2 face damage:
- seq6 T0 (the F2 mismatch — fired the Blast it wanted to hold).
- seq13 T4 (surf1): a ~1500-word agonizing reply concludes "2 damage is not a win condition...
  inefficient... but the guide says 'A turn where you cast nothing is a lost turn'... I must cast
  Galvanic Blast." The cast-nothing floor over-pressured a value-negative early burn of a finisher.
**Not outcome-affecting:** deck133 lost my-1 vs op21 — a 22-life material blowout (deck110 drew no
creatures; the wasted 2-4 Blast damage was irrelevant to a game it never contested). n=2 in one
lost game; the guide has NO explicit "a held finisher is not a lost turn." Logged as a per-deck
candidate ONLY if a 2nd seat/game reproduces it in a CONTESTED game (M1). Freeze meanwhile.

---

## ENGINE-CHANGE VERIFICATION (per brief)
1. **Answer-after-plan protocol** — label compliance 163/169 (gaps = truncations only); 0 true
   head-vs-choice mismatches; intent-collapse class STRUCTURALLY GONE; detector flags are false
   positives (V1). Confirmed positive at deck135 seq14.
2. **Artifact tag completeness** — SHIPPED; re-derived count AGREES with summary N in 177/177
   windows; Glimmervoid + basics correctly untagged; artifact creatures + Darksteel Citadel now
   tagged. Wave-9 E3 lever CLOSED at representation layer. Residual reasoning fold-in persists but
   non-outcome-affecting.
3. **Frozen guardrail classes stayed dead:** re-equip marker 31/0 obeyed (Rule #1); reading
   `Artifacts in play: you N` — READ instruction holds in the common case (majority match) but the
   fold-in survives (representation wall, non-outcome). No new class of guide regression.
4. **Blocker seam** — clean; no reflexive high-life block, no missed must-block that changed a game
   (combat truncations were in lost positions, V4).
5. **Defer** — 2, the expected Glimmervoid gate (V3).

## Corpus health
- cast-nothing 15/85 cast-decisions (18%; wave-9 10%, wave-8 17%) — within range, concentrated in
  the flooded/creature-light losses (vs133/vs135/vs140). No durdle regression; deck133 the tension
  inverted (cast a filler Blast rather than holding — F2/watch).
- attackers exercised; blockers 4; equips 11.

## HANDOFF (OBEYED-BUT-LOSING / frozen-line diff; 5th freeze wave)
1. **FREEZE the entire guide verbatim** (strategy.txt is a byte-identical copy, md5 confirmed). The
   wave-8/9 artifact-count READ edit is as far as the guide can push the fold-in; the wall is
   confirmed a 3rd wave. No line earned a change; no guardrail earned a demotion (the churn class
   was OBSERVED VIOLATED, so Rule #1 stays full-prominence; the marker class OBSERVED OBEYED stays
   frozen as-is).
2. Route to notes (not guide): F1 multi-copy/equivalent-carrier equip churn -> PROMOTED engine
   lever E2; the hand-card type-tag sub-gap -> E3 candidate micro-lever; F2 plan-fixates-on-unlisted
   -> reply-protocol/model watch (M1); the Blast-filler burn -> per-deck candidate pending a
   contested-game 2nd instance (M1); vs109/vs135/vs133/vs140 -> construction/variance (DC1).

## FREEZE-CHECK (obeyed-but-losing mode) — EVERY line survives verbatim.
- Identity / kill-by-5-6 / cast-nothing=lost-turn / beatdown-by-default — VERBATIM. (The
  cast-nothing edge case — burning a finisher Blast as filler — is n=2 non-outcome, watch.)
- Rule #1 (marker, two-power cue, release, mana destination) — VERBATIM (31/0 obeyed; the churn
  leak is the marker's blind spot, an engine item, not a guide gap).
- CASTING falsity-fact + Memnite/Ornithopter {0} — VERBATIM.
- THE ENGINE (`Artifacts in play: you N` READ + Glimmervoid-already-excluded note) — VERBATIM.
  Held: wording correct, fix lives at representation (tags now complete), guide no longer the
  escalation surface.
- THE RACE / ATTACK EVERY TURN / Signal Pest / Champion / Lightmine — VERBATIM.
- LETHAL CHECK (metalcraft anchored to `Artifacts in play: you N`) — VERBATIM.
- PLAYING AROUND COUNTERS — VERBATIM (vs44 flooded to a WIN again).
- OTHER KEY CARDS / MULLIGAN / DECIDING SITUATIONS — VERBATIM.
