# Deck-140 review — wave 11 (2nd cycle after the resume; validates the wave-10 batch at scale)

Deck: **"Wipe Them Out!"** — RBW control, 14 board wipes, ~2 creatures (Blightsteel Colossus x2),
finishers = Rakdos's Return / Staff of Nin / Blightsteel Colossus. Sweep every creature board, deploy a
finisher, grind the face to 0 on reach.

Corpus: PRIMARY `matchups-20260715-072254`, 6 deck140 seat games, binary `/tmp/wagic-54a5a9d01`
(wave-10 engine batch + **max_tokens 4096** + parse **ramble guards** + stack-target annotation fix +
fetch pre-targeting keying), wave-10 guide (byte-identical to the live guide), ANSWER-LAST reply
protocol (scratch reasoning -> `PLAN:` -> `CHOICE:` final line). Every deck140 seat file read
decision-by-decision; opponent seats read at the sweep/combat seams; adjudication cross-checked against
`results.tsv`. Win column is CONTEXT ONLY (user doctrine); the unit of analysis is the DECISION.

**Record: 3 W / 3 L** (brief win table: 140:3/6). Decision-level story: **the count-anchored SWEEP
TRIGGER is READ correctly a 4th corpus (23 clean fires; guide FREEZES); the ramble/truncation guard
SHIPPED and is VALIDATED — every formless or out-of-range reply now falls to the heuristic instead of
mis-casting (the wave-10 vs44 s34 evasion is GONE); the wave-10 trailing PLAN/CHOICE mismatch residual
PERSISTS as a 3rd witnessing wave but has SHARPENED — this corpus every genuine instance is the
UNLISTED/UNAFFORDABLE-CARD FIXATION and it is now BIDIRECTIONAL (over-casts a sweeper over Cast-nothing
AND under-casts, mapping a wanted-but-unlisted sweeper to Cast-nothing over an offered payable removal);
Rakdos was HELD every window (no wasted kill shot — wave-10's 2-Rakdos-waste class did NOT recur);
adjudication rows are sane; the loss column is unchanged reach-starvation + mana-screw.**

| game | opp | outcome | deck140 state at end | class |
|---|---|---|---|---|
| vs110 | affinity     | **WIN (adj0)** 24-14 T28 | AHEAD | swept clean; capped-adjudicated |
| vs131 | UWx control  | **WIN (adj0)** 26-23 T30 | AHEAD (mana-screwed) | control grind; capped-adjudicated |
| vs44  | UB faeries   | **WIN (adj1)** 25-17 T32 | AHEAD (opp raced out) | shroud+Blightsteel handled; capped-adjudicated |
| vs135 | snow control | LOSS (adj0) 16-17 T13    | behind by 1 | 4 early HTTP-timeout fallbacks; 1-life adj loss |
| vs133 | mono-B blitz | LOSS (opp win) -2 T27    | dead | reach starvation |
| vs109 | mono-R aggro | LOSS -5 T9              | dead | reach starvation + no red (mana-screw) |

Tags: [GUIDE] model-fixable · [STRUCTURAL] deck/infra · [REPLY-PROTOCOL] harness layer.

## Harness / executability sweep (FIRST)
- **Decision volume**: ask+priority ~= 249 records across the 6 seats; 20 are fallback records (below),
  so ~= 229 parseable pilot decisions.
- **Label compliance (answer-after-plan)**: every reply that reached a `CHOICE:/ATTACK:/BLOCKS:` line
  parses; the 20 that don't are the fallbacks below (all correctly flagged). High compliance.
- **Fallbacks (20)**: 19 `empty_reply` (choice -1) + 1 `unparsed_reply` (choice -1). Every one is a
  HTTP-120s timeout (`latency_ms` ~= 120003) on a tail generation, the KNOWN corpus-wide harness class
  (72 corpus-wide; 240s `WAGIC_GPT_TIMEOUT` fix ships next corpus). **None landed on a critical sweep
  window** (machine-checked: no fallback at C>=1 with a payable sweeper offered AND life <=16). None
  changed a game outcome (self-healing). See Finding 4 for the guard validation and notes.md HARNESS #1/#2.
- **X-menu offset**: max-appropriate at every window. vs135 seq13 X=4 of [4..0] (max affordable — a
  guide-sanctioned PARTIAL sweep of an 8/8 with no Wrath/Damnation offered); vs44 seq52 X=3 exactly
  kills a 3/3 Faerie Bladecrafter. No offset miss.
- **Latency**: still drives long games; 4 of 6 deck140 games reached the decision cap and were
  ADJUDICATED by life (Finding 5). No raw timeouts.

## TOP FINDINGS

### 1. [FREEZE — 4th corpus] Count-anchored SWEEP TRIGGER is READ correctly at every window; 23 clean fires
Machine-checked across all 6 games: **23 windows fired a sweeper at `(creatures: N>=1)` with the trigger
CITED correctly**, and the trigger's conditions were applied faithfully — the empty-board HOLD, the
"16 or lower (16 counts)" boundary, and the "attacked recently" arm all read correctly:
- vs109 seq9 (life 20, C=5, wide tapped goblin board): *"the opponent has a wide board of attackers that
  will swing for lethal... I must cast a sweeper immediately when C is 1+."* Trigger read correctly.
- vs110 seq82 (Etched Champion, protection-from-all-colors): the pilot correctly reasoned Pyroclasm (red)
  is prevented by protection, Black Sun's Zenith targets (protection), so a non-targeting Wrath/Damnation
  is required — an impressively rules-correct WHICH-SWEEPER derivation.

**The trigger GUIDE stays FROZEN.** Two nuances, both routed OFF the guide:
- **Over-fire into C=0 (2)**: vs135 seq10 Pyroclasm and vs131 seq27 Pyroclasm cast into an empty board —
  but in BOTH the PLAN reads C=0 correctly and concludes HOLD; only the trailing `CHOICE:N` casts. These
  are the reply-protocol mismatch (Finding 3), NOT the trigger misfiring.
- **The one held-sweeper MISS this corpus (vs109 seq9)** is likewise NOT a trigger misread — the plan
  correctly invoked the trigger and WANTED to sweep; the leak is the reply-protocol mis-map (Finding 3,
  under-cast direction). So the wave-10 "zero held-sweeper misses" claim now carries one exception, and
  it is attributed BELOW the guide, at the reply protocol. The trigger text itself was read correctly at
  all 24 relevant windows.

### 2. [FREEZE] #1 rule (hold Rakdos) obeyed every window; WHICH-SWEEPER + X-menu clean
Rakdos's Return was OFFERED at many windows (deck44 ~15 windows, deck131, deck133) and **HELD every
single one** — 0 casts. The wave-10 wasted-Rakdos class (2 kill shots dumped via the trailing mismatch)
did **NOT recur** this wave. Black Sun's Zenith X picks max-appropriate (Finding above). Blightsteel
deployed when reachable (vs44). WHICH-SWEEPER logic obeyed where exercised. Freeze.

### 3. [REPLY-PROTOCOL — route to the A/B, NOT the guide] Trailing PLAN/CHOICE mismatch, 3rd wave, now BIDIRECTIONAL — the UNLISTED/UNAFFORDABLE-CARD FIXATION
The answer-after-plan protocol's residual persists (deck140 = 3rd witnessing wave, after deck135/deck140
wave-10 and deck135 wave-11 Q5). This corpus it SHARPENS to a single root: **the plan's NAMED action is a
card that is NOT among the offered options (because it is unaffordable / not castable this turn), and the
trailing `CHOICE:N` fails to reconcile — it maps the plan's intent onto a WRONG listed option.** It now
fails in BOTH directions:

**Over-cast (2) — wants an unlisted FINISHER on an empty board -> casts a listed SWEEPER over Cast-nothing:**
1. **vs135 seq10 t9 (C=0)** — plan: *"the opponent's board is currently empty of creatures... I should NOT
   cast Pyroclasm... Instead, I will deploy... Staff of Nin."* Options were only [Pyroclasm, Cast nothing];
   Staff was UNLISTED. `CHOICE: 1` cast **Pyroclasm into C=0** — a wasted sweeper (the correct listed action
   was Cast nothing, option 2). Lost the game by 1 life at adjudication (context: 4 HTTP-timeouts also).
2. **vs131 seq27 t20 (C=0)** — plan: *"the opponent has no creatures... I should not waste my sweepers...
   Instead, I will deploy... Staff of Nin... I will hold Rakdos's Return."* Options [Cruel Edict, Pyroclasm,
   Cast nothing]; Staff UNLISTED. `CHOICE: 2` cast **Pyroclasm into C=0** — wasted. Won (adj).
   **ROOT confirmed: Staff {6} was genuinely unaffordable** — Mana available `{b}{w}{w}{w}{w}` = 5 sources
   for a 6-cost artifact (the pilot is mana-screwed at 5 lands on T20). The engine CORRECTLY withheld Staff;
   the model fixated on the unaffordable finisher and, rather than resolving "I can't afford it -> Cast
   nothing," mapped its deploy-intent onto the nearest listed cast.

**Under-cast (1) — wants an unlisted SWEEPER at C>=1 -> maps to Cast-nothing over an OFFERED payable removal:**
3. **vs109 seq9 t6 (C=5, life 20, wide aggro board)** — plan correctly invoked the trigger and concluded
   *"cast Pyroclasm to destroy all 5"* — but the pilot had **NO red mana** (Mana available `{b}{w}{w}{w}`,
   Pyroclasm needs `{1}{R}`), so Pyroclasm was correctly UNLISTED; options were only [Cruel Edict, Cast
   nothing]. `CHOICE: 2` cast **Cast nothing**, ignoring the OFFERED payable Cruel Edict — a held-sweeper
   miss in the loss. Compounded by a MANA MISREAD: the plan asserted *"I have exactly the mana required for
   Pyroclasm ({1}{R})"* while holding no red source (the colored-mana/offered=payable false-belief family;
   notes.md MODEL #5). Cruel Edict (sac 1 of 5) would not have saved a reach-starved position, but the
   DECISION was wrong (a payable removal offered, cast nothing).

**Caught by the guard (1 near-miss):**
4. **vs131 seq22 t16 (C=0)** — plan wanted Staff of Nin (unlisted; unaffordable) and emitted
   `CHOICE: 5` — **out of range** (only 4 options). Correctly flagged `unparsed_reply`, choice -1 ->
   heuristic fallback. The guard WORKED: an unmappable index fell back rather than mis-casting.

**Rate this wave: 3 genuine mis-maps / ~=229 parseable (~1.3%) + 1 caught-by-fallback**, vs wave-10's
4/215 (~1.9%) — comparable, trending slightly down. **The asymmetry is no longer purely casts-over-holds:
it is bidirectional, unified by the unlisted/unaffordable-card fixation.** The durable-fix target sharpens
(the reconcile-named-action A/B arm, ledger #3): **when the plan's NAMED action is ABSENT from the
options, the reconciler must route to the best AVAILABLE listed option (a payable removal if the plan
wanted removal; the pass/Cast-nothing if the plan wanted to hold/deploy an unaffordable finisher) — NEVER
an arbitrary listed cast.** The model reliably NAMES its choice in prose, so this is tractable. **Do NOT
re-word the guide** (it is read correctly throughout) and do NOT score it against a guide rule. This is
exactly the wave-10 deck110-P3 sub-variant, now with clean bidirectional multi-seat evidence
(deck135 wave-11 Q5 independently witnessed the under-develop direction — the deck44 s34 Azcanta case —
citing the same lineage). See general-suggestions.md and notes.md MODEL #4.

### 4. [RESOLVED — guard validated] Truncated / formless-reply guard-evasion is FIXED
The wave-10 vs44 s34 evasion — a formless truncated reply (2048-cap, mid-deliberation, `fallback: None`)
that mis-recorded a Rakdos cast the plan had rejected, then wasted it X=0 — does **NOT recur**. This
corpus, machine-checked: **every reply lacking a `CHOICE:/ATTACK:/BLOCKS:` label is correctly flagged**
(19 `empty_reply` + 1 out-of-range `unparsed_reply`, all choice -1 -> heuristic); **zero formless replies
mis-cast**, and the out-of-range `CHOICE: 5` (vs131 seq22) also fell back cleanly. The ramble/label guard
+ 4096 cap did what wave-10 asked: the fallback TYPE shifted from `unparsed_reply` (2048 truncation) to
`empty_reply` (HTTP timeout on the longer tail), and the silent-stray-parse path is closed. Guard
VALIDATED at the deck140 seat.

### 5. [INFRA — confirmed 2nd corpus] Adjudicate-by-life-at-cap; rows sane
deck140 reached the decision cap in 4 of 6 games and all were ADJUDICATED by life (3 adj wins, 1 adj
loss). Rows cross-checked against seat final life: **3/4 exact** — vs135 seat 16/17 T13 = `135 140 adj0
17 16 13`; vs131 26/23 T30 = `140 131 adj0 26 23 30`; vs110 24/14 T28 = `140 110 adj0 24 14 28`. One
minor timing nit — vs44 seat's last logged decision is 22/17 at T33 vs the adjudicated `44 140 adj1 17 25
32` (deck140 25 at T32): deck140 gained a few life after the last logged seat decision (Venser's/Elixir)
and the adjudicator snapshotted a beat earlier; the VERDICT is unambiguous (deck140 clearly ahead either
way). Route the snapshot-timing nit to notes.md INFRA #7. adjudicate-by-life-at-cap CONFIRMED working.

### 6. [STRUCTURAL — whole loss column, unchanged] Reach starvation + mana-screw vs fast aggro
Both aggro losses are the same deck-construction fact, now with a mana-screw overlay: vs109 (mono-R)
swept where it could but had **no red source** for Pyroclasm (Finding 3.3) and ran out of reach; vs133
(mono-B blitz) out-raced. The 1-damage Staff clock is far too slow against a T9-T15 kill, and the finisher
(Staff {6} / Blightsteel {12}) sits uncastable when the manabase stalls (vs131 also: 5 lands on T20 — the
control face of the same gap, won only via cap adjudication). Options in priority (unchanged): (a) a
cheaper recurring clock that survives a counter; (b) a 1-2 mana early lifegain/blocker vs aggro; (c) more
untapped RED so Pyroclasm {1}{r} is a reliable turn-2/3 sweeper. 14 sweepers is the right density — needs
TEETH, not text. Do NOT spend guide length on reach.

## PHANTOM-BOARD WATCH (non-fatal this wave; distinct from Finding 3)
- **Stale-plan / phantom-creature sweep vs affinity (2)**: vs110 seq68 Pyroclasm and seq82 Wrath cast
  into a board whose CURRENT line reads `(creatures: 0)`, while the plan believed a creature present
  ("exactly one creature, Ornithopter 0/2" / "Etched Champion"). Here the plan AGREES with the choice —
  it is a board-MISREAD (the wave-6 hallucinated-board / stale `YOUR PLAN (as you last stated it)` echo),
  driven by affinity's rapid within-turn create-then-die churn, NOT the trailing-index mismatch of Finding
  3. Only 2 instances, both in a WIN (vs110 24-14). Route to representation/harness (the stale plan-echo
  can misinform); WATCH, do not legislate. See notes.md ENGINE #6.

## OVERSHOOT / DISPLACEMENT AUDIT (mandatory)
1. SWEEP TRIGGER + belief-kills: READ correctly 4th corpus (23 clean fires; empty-board holds, boundary,
   recent-attack arm all cited right). Over-fire into C=0 = reply-protocol, not the trigger. FREEZE.
2. Displacement: the residual did NOT move to a new guide seam. It sits at (a) the REPLY PROTOCOL
   (unlisted/unaffordable-card fixation, bidirectional = harness) and (b) the CLOSER
   (slow/counterable/mana-screwed = deck construction). Neither is a displaced guide leak.
3. Rakdos hold (#1): obeyed at EVERY window it appeared (0 casts). The wave-10 wasted-Rakdos class is gone.
4. BSZ-X / X-menu / Staff face-ping / Lightmine / Mulligan / Lands: obeyed where exercised. Freeze.
5. No attack-floor issue (deck runs ~no creatures; Blightsteel deployed vs44).

## WATCH (do NOT legislate — single seat / re-widening risk / precondition-absent)
- **Phantom-board stale-plan sweep vs affinity** (new this wave, 2 windows, non-fatal) — WATCH; route to
  representation/harness. Do not widen any hold (re-opens the wave-7 overshoot).
- **SNOWBALL single evasive threat** (carried) — vs44's Sleep-Cursed Faerie / Faerie Bladecrafter were
  swept (Cruel Edict / Wrath / BSZ X=3). No snowball-past-sweep-range loss. Hold the WATCH.
- **Within-turn double-sweep at C=0** (carried) — no clean re-offer double-sweep this corpus. Hold.
- **BSZ partial-sweep guardrail** — exercised ONCE (vs135 seq13 X=4 partial of an 8/8, guide-sanctioned
  as no Wrath/Damnation was offered). Read correctly. FREEZE.
- **Lightmine second-copy line** — precondition (a resolved Lightmine + a 2nd copy offered) did not recur
  cleanly. FREEZE, do not demote.
