# Core-prompt revision (wave 7) — exact edits for `bin/Res/ai/gpt/system_prompt.txt`

## Verdict: NO core-prompt edits this wave (PASS — fourth wave running).

`system_prompt.txt` STAYS byte-identical to `general-strategy.txt`. Verified:
`diff wave7/general-strategy.txt bin/Res/ai/gpt/system_prompt.txt` is empty. No owner action to the
prompt body this wave.

All seven seats independently returned NO core edit. The convergence gate (2+ seats OR completion of
an existing core line) admitted no new core text. Every fixable item routes to ENGINE/REPRESENTATION,
PER-DECK, or MODEL-EXPERIMENTS. Details below, plus the ONE scheduled change: the `hand -> stack`
anchor A/B is now GO for wave-8 (its condition — two consecutive clean no-op corpora — is met).

---

## 1. SCHEDULED A/B (not a wave edit): retire the `hand -> stack` cast-happened anchor — condition MET

Wave-6 put the anchor on the removal SHORTLIST with the explicit trigger: *"if wave-7 is a SECOND
consecutive clean no-op corpus, retire the anchor via a deliberate A/B."* **Wave 7 IS that second
consecutive clean corpus** — the no-op sweep found **0 cast no-ops and 0 cast-repeat loops across ALL
7 seats** (deck109 0 across 6 games *after its own guide fallback was DELETED with no regression*;
deck110 0 across 188 decisions; deck131/133/135/140/44 all clean). Two waves, seven seats, defect
fixed at source by **353f1a150**.

- **This is an A/B experiment recommendation, not a blind wave edit** — the skill's own epistemics
  require a deliberate A/B (with a control arm) to cut a deck-agnostic core line, never a silent
  removal. So the anchor STAYS in the shipped prompt until the A/B runs; wave-8 runs the A/B.
- **Exact old → new for the A/B removal arm** (line 35 of `system_prompt.txt`, the second sentence
  of the "Your last stated plan is a note from your past self…" paragraph):

  OLD (remove):
  > A cast that really happened shows in the GAME LOG as the card moving "hand -> stack"; if your
  > last choice was a cast and the card is STILL in your hand with your mana unspent, the cast did
  > not happen - spend this decision on a DIFFERENT castable card and try the stuck one again next
  > turn; repeating the identical pick in the same turn changes nothing.

  NEW (A/B removal arm — replace the deleted sentence with just the generic anti-repick clause it
  contained, so the loop-insurance survives the anchor):
  > If your last choice was a cast and the card is STILL in your hand with your mana unspent, spend
  > this decision on a DIFFERENT castable card and try the stuck one again next turn - repeating the
  > identical pick in the same turn changes nothing.

  Rationale for the two-part removal: the anchor bundled (a) the Hellrider-no-op tell ("`hand ->
  stack` proves the cast happened") — now inert, this is what the A/B removes — and (b) a generic
  anti-repick-loop clause that outlives the bug. Keep (b), drop (a). If wave-8 prefers a clean full
  removal for a sharper A/B signal, the whole sentence goes.
- **Measurement for the A/B:** repick-loop rate and Cast-nothing rate, removal arm vs control arm,
  across the round-robin. Expect no regression (the defect is gone). Cite 353f1a150.

## 2. Reflexive high-life blocking + its new guises — STILL routes to REPRESENTATION, not core

Wave-6's only 2-seat convergence. Wave 7 adds a THIRD guise but the routing is unchanged:
- deck44 `vs133 s18`: blocked Phyrexian Obliterator at **L11** (feeding its sac punisher) after
  correctly declining at L20 — reframed this wave as an intra-guide *nesting* defect (the absolute
  was buried inside the low-life chump rule) PLUS a punisher-visibility gap.
- deck131 `vs110`: chump-blocked Guttersnipe (its ONLY reach) into a NON-lethal attack at **L14** —
  the block framed as "survival," so the high-life floor (keyed on "above 8") did not catch it.
- deck135 CLEAN (1 low-life survival block), deck109 CLEAN (wave-5 floor holding, 0 high-life
  blocks), deck110 the block seam now appears and fired correctly.

The core ALREADY carries the floor (line 29). The pilot reads enough to state a false "gain" and
blocks anyway — **present-and-losing, which routes to REPRESENTATION**, never a second core paragraph.
The engine fix (ledger item 5): at the `blockers` decision surface the decision-relevant number
(`Your life: 20 (unblocked this turn: 2 -> you'd be at 18)` / a `[not lethal]` marker) AND annotate a
punisher blocker with its rider ("blocking this makes YOU sacrifice/take X"). "Racing creatures don't
block" would misplay every control deck — NEVER core. Per-deck floors stand as interim.

## 3. Candidate core TIGHTENINGS raised this wave — all held to WATCH (single-seat)

- **Phantom-lethal fixation → decline a listed body** (deck109 general #2): at L4 with two bodies
  listed the pilot chose "Cast nothing" citing "cannot cast Boros Reckoner" (FALSE — it was listed),
  fixated on an unassemblable combo. The core already forbids this (never-decline-a-listed-action +
  plan-from-list). ONE instance, not outcome-changing. deck135's stale-plan-around-uncastable-Treefolk
  is a RELATED but distinct shape (the card was never offered — a manabase strand, not a decline). NOT
  2 seats of the SAME shape → WATCH. If a 2nd seat shows "fixated on an unaffordable bomb → decline a
  listed body," tighten the phantom-lethal / plan-from-list lines ("a lethal or bomb you cannot pay
  for this turn is not a reason to cast nothing — deploy the best body you CAN pay for").
- **"A tapped attacker is still a threat"** and **"shroud/hexproof does not stop non-targeted sweeps
  / sacrifice effects"** (deck140 watches): deck-agnostic mechanics facts; the prompt already has a
  mechanics-facts block. SINGLE SEAT (only deck140 exercised sweepers). WATCH; promote to the
  mechanics-facts block only if a 2nd control/removal deck reproduces the "they're tapped, I'll wait"
  hold or shroud-conflation suppressing a non-targeted spell. Preferred end-state is the ledger-item-3
  "creatures: N" surface, which dissolves both without core prose.

## 4. Threat-before-value (wave-5 Edit 2) — CONFIRMED again, keep verbatim

deck133 (deployed removal/creatures over the cheapest discard), deck135 (Force scoped, bodies before
digs), deck131 (every value cast was a no-threat window). No change.

## 5. Single-seat items explicitly routed OFF core (see synthesis-notes engine ledger)

- Land-drop seam (deck131): ENGINE — enumerate distinct playable lands as options. Ledger #1.
- No-legal-target menu, migrated to Fatal Push/Liliana (deck44): ENGINE — shared target-existence
  probe. Ledger #2.
- Held-fetch re-offer tax / multi-copy equip churn (deck135, deck110): ENGINE — suppress the re-offer
  of a completed repeatable activation. Ledger #4.
- Gray Merchant resolved-drain display (deck133): ENGINE/representation A/B. Ledger #6.
- Free-reaction card cost (deck135), mulligan false-positive (deck131): PER-DECK + watch.
