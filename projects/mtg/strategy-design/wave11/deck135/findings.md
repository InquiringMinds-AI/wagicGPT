# Deck-135 (Modern Snow) — Wave-11 game review

Corpus: PRIMARY `matchups-20260715-072254` (21-game round-robin over 44/109/110/131/133/135/140;
binary `/tmp/wagic-54a5a9d01` = wave-10 engine batch + max_tokens 4096 + parse ramble guards +
**stack-target annotation fix** + fetch pre-targeting keying; wave-10 guides live = current
`deck135_strategy.txt`; ANSWER-LAST reply protocol; core prompt unchanged, 7th PASS). All six deck135
seat games read decision-by-decision from the translogs; opponent seats read at the Force/combat seams.
Doctrine applied: decision-level opportunity-conversion audit; the win column is CONTEXT ONLY. This is a
VERIFICATION wave — the center of gravity is (a) confirming the wave-10 stack-annotation fix at the
Force seam, and (b) any decision class newly visible beneath the fixed interface noise. Game->file map
at bottom.

## RECORD (context only — NOT a verdict) — 1W / 5L

| game    | opp (name)              | outcome (deck135)          | my/opp life | turn |
|---------|-------------------------|----------------------------|-------------|------|
| 135v140 | 140 Wipe Them Out!      | **WIN** (adj0, life-cap)   | 17 / 16     | 13   |
| 135v109 | 109 Hellrider           | LOSS                       | -2 / 17     | 11   |
| 135v133 | 133 Phyrexian Asphodel  | LOSS (drain grind)         | -2 / 19     | 19   |
| 135v131 | 131 Mind Control        | LOSS (adj1 — near-win at **1**) | 1 / 19 | 15   |
| 135v44  | 44 Faerie Archmage      | LOSS                       | -7 / 18     | 18   |
| 135v110 | 110 Etched Affinity     | LOSS (fast, T7)            | -2 / 20     | 7    |

Softer than wave-10 (2W/4L) but win-rate is CONTEXT ONLY and this deck is construction/variance-bound
(brief win table: 135:1/6). Every loss is opponent-life-near-full or a slow-clock/aggro-race:
vs110 flipped W(adj0)->L this wave as a T7 aggro loss (mana-light: only Boreal Druid T0 + Viper T6 hit);
vs140 flipped L->W(adj0) — the Pyroclasm Force window (see Q1); vs131 again a near-win adjudicated by the
pilot sitting at 1 life; vs133 the same drain-grind that starves the creature draw. No dead-board
develop-paralysis; creatures deploy on curve everywhere the draw supplies them (Q4). This stays an
OBEYED-BUT-LOSING / turnaround-mode seat: freeze the obeyed spine; every residual routes OFF the guide.

---

## Q1 — STACK-ANNOTATION FIX: the Force "NO legal target" contradiction is DEAD, and reasoning-tax COLLAPSED. Clean validation headline.

The wave-10 ledger #1 second fault — the Force option rendering a self-contradiction
(`Cast Force of Negation ... - NO legal target right now - can target on the stack: <Spell>`) — is
**GONE at this seat: 0 occurrences** (brief: 0 corpus-wide). The counterspell option now renders cleanly,
e.g. `Cast Force of Negation {1}{u}{u} - can target on the stack: Pyroclasm`.

**The predicted reasoning-tax drop is CONFIRMED and large.** Wave-10's textbook oscillation was deck140
s58: ~470 words of "Is Force a blue card? ... If I exile it ... No, usually a *different* card ..."
driven directly by the contradiction. This wave, at the analogous window (deck140 s30-s32, Force on
Pyroclasm), the three replies are **96 / 120 / 109 words** — clean, correct, no self-negotiation. Verbatim
s30: *"The opponent is casting Pyroclasm ... This will kill my Ohran Viper ... I must counter this board
wipe. I have a blue card in hand (Search for Azcanta) to pay the alternative cost ... so I will exile it
to counter Pyroclasm for free."* The oscillation class is dead; the annotation fix did exactly what wave-10
predicted.

**Downstream, Force DISCIPLINE also improved** — the pilot now cast Force only on a STRATEGICALLY CORRECT
target and declined cleanly on every 1-for-1:
- **Force-offered windows this seat: 19** (deck140 3, deck133 15, deck44 1).
- **Cast-Force choices: 2** — both in deck140, both on the SAME Pyroclasm (s30 exile-alt version, s31 the
  re-offered mana version), i.e. ONE actual Force cast attempt. Pyroclasm is a board wipe = exactly the
  guide's sanctioned Force target. **Correct.**
- **deck133: 15 Force windows, 0 casts — all correct declines.** Opponent cast Fatal Push (s20, targeted
  removal, let resolve), Collective Brutality (s22), and Inquisition of Kozilek (s37, discard). The pilot
  chose Cast nothing / pass at every one. The **discard-exclusion clause is OBSERVED OBEYED** at s37 (did
  NOT Force the Inquisition); the Inquisition then DISCARDED the Force itself (EVENTS s38
  `Your Force of Negation: hand -> graveyard`) — a correct, costless outcome for the pilot.
- Face-burn over-fire: **0** (watch holds).

## Q2 — FORCE RESOLUTION BUG (ledger #1, still broken by design): no NEW confirming data this wave; bleed COLLAPSED.

The engine defect (free-alt-cost Force resolves without countering) is next cycle's top fix and was NOT
fixed this corpus. **Quantified bleed this seat:**
- **Force casts: 1** (deck140 Pyroclasm). **Counters achieved: 0 confirmed.** **Blue cards exiled: ~0-1.**
- The one clean cast had its resolution **cut off by adjudication**: the deck140 game ended adj0 (WIN,
  17-16) at turn 13, the instant the Force/Pyroclasm interaction went pending. Cross-checked the deck140
  OPPONENT seat (`1784118177-...deck140-...-vs-...deck135`): it ends at seq 14 / turn 13 with
  `Cast Pyroclasm {1}{r}` and no resolution logged either side. The pilot chose "exile a blue card from
  hand" at s32 but s33 was an empty_reply and the game closed; the board at s32 still showed Search for
  Azcanta in hand, so **no blue card is confirmed lost.**
- Net: this seat produced **no new evidence for OR against** the resolution bug (only one cast, adjudicated
  mid-resolution) — consistent with the carried defect. It is **~0 confirmed material bleed** this wave,
  down from wave-10's 4 casts / 0 counters / ~7-8 blue exiles (deck131+deck140). The collapse is driven
  jointly by (a) different game trajectories and (b) the fixed annotation restoring Force discipline (the
  pilot no longer mis-Forces soft targets like Downsize/Elixir/Cyclonic Rift that the confused annotation
  provoked in wave-10).

Routing (BINDING): resolution bug = **engine ledger, carried** (notes.md ENGINE #1). The guide's Force
section is strategically correct (Pyroclasm=board wipe was Forced correctly) and unchanged. The flagged
tension stands: until the engine counters, a correct Force still pitches a blue card for no effect — an
argument for engine-fix PRIORITY, not for writing the guide around the bug.

## Q3 — FETCH PRE-TARGETING KEYING: chosen-but-unresolved re-ask REDUCED (3->2 windows/crack) but NOT gone; new duplicate-option artifact.

Fetch-crack option windows this seat: **61** (deck140 7, deck109 9, deck133 14, deck131 16, deck44 11,
deck110 4) — essentially FLAT vs wave-10's 66. The consume-on-choose fix still holds (far below wave-9's
per-window tax); cracks fire and resolve freely; crack-for-missing-color works both ways
(Forest when short green, Island when short blue).

**The wave-10 deck44 s5-s7 chosen-but-unresolved re-ask (3 windows for 1 crack) is REDUCED to 2 windows,
not eliminated.** Every crack this corpus shows a `choose -> re-offered once -> resolve` pattern: the fetch
activation goes on the stack, the pilot gets priority AGAIN with the SAME "Put in Play with <fetch>" option
before it resolves, then the land enters. Traced at deck140 s5->s6->s7 (Misty Rainforest; land resolves at
s7 with `battlefield -> graveyard | Snow-Covered Forest: library -> battlefield | life -1`); identical
2-window shape at deck109 s3/s4, s7/s8; deck133 s12/s13, s25/s26, s30/s31, s41/s43; deck44 s6/s7, s15/s17,
s28/s29; deck110 s7/s8. **One life, one land per crack — DECISION-COUNT inflation only, zero resource loss.**

**New artifact from the keying itself:** the two priority windows render **two IDENTICAL option lines**
(`Put in Play with Misty Rainforest targeting Snow-Covered Forest` twice) — the pre-targeting keying
enumerates target lands but collapses to duplicates when the deck holds multiple copies of the same snow
basic (4 Snow-Covered Forest). Cosmetic; the pilot picks correctly. Route both (the 1-window re-ask
residual + the duplicate rendering) to notes.md ENGINE #2/#3. Not guide text.

## Q4 — DEVELOP-IMPERATIVE (Rule #1 point 3): STEADY. wave-10's validated rewrite holds; no dead-board recurrence.

Creatures deploy on curve everywhere the draw supplies them: deck140 Coatl T4 / Viper T6 / Treefolk T10 /
Viper T12; deck109 Golem T2 / Coatl T4 / Viper T8,T10; deck131 Coatl T2 through Golem/Viper T14 (7 bodies);
deck44 Golem T3 / Viper T7 / Coatl T9,T11. **No creatureless dead-board paralysis in any game** (wave-10
Q5's vs133 class stays dead). The two sparse games are draw/construction-bound, not develop-paralysis:
deck133 drew only 2 castable creatures (Coatl T5, T14 — the drain-grind loss); deck110 was a T7 aggro loss
where the deck was mana-light (Druid T0, Viper T6).

Cast-nothing-with-a-creature-offered windows examined; **none is a guide-fixable develop miss:**
- **deck140 s26** (MP1, Treefolk+Viper offered, Cast nothing): the pilot had a live Treefolk already,
  went to combat to attack, then cast Ohran Viper in **MP2 the same turn** (s28). A sequencing choice
  (attack between), creature deployed on curve — correct, and it WON.
- **deck131 s35 / s47** (lost-position, 1 life at s47): declined creatures reasoning about survival
  ("cannot pay life to accelerate into a board state I cannot survive"). Soft point-1/defense misses in an
  already-lost position (loses next turn); covered by the existing WHO-IS-THE-BEATDOWN anchors. Not a class.
- **deck44 s34**: a distinct MODEL hallucination — see Q5.

## Q5 — NEW residual: Azcanta-transform card-fact hallucination (single window, model-side).

**deck44 s34 (T15, L5, facing two flyers).** Options: `Cast Into the North {1}{g}` · `Cast Search for
Azcanta {1}{u}` · `Cast nothing`. The pilot's plan fabricates a card fact and an unoffered action:
> "I have a Search for Azcanta on the battlefield that has met the condition to transform ... Transforming
> it into Azcanta, the Sunken Ruin gives me a **4/4 flying creature** that can block their threats ... I
> will transform Azcanta now to get a blocker on board." -> `CHOICE: 3` (Cast nothing).

Two stacked errors: (1) **card-fact hallucination** — Azcanta, the Sunken Ruin is a LAND, not a 4/4 flyer;
(2) **unoffered-action fixation** — "transform Azcanta" is not among the options, so the pilot maps its
desire to `Cast nothing` rather than casting an offered develop spell (Into the North / Search). This is
the same FAMILY as wave-10 Q6 (plans an unoffered play, under-develops) and the wave-10 deck110 P3
plan-fixates-on-an-unlisted-card sub-variant. Single window, in an already-lost game (5 life, two flyers,
loses T18) — did NOT change the outcome. Routes to MODEL (notes.md), with a WATCH on a possible KEY CARDS
micro-note (Search for Azcanta flips to a LAND, card advantage, not a blocker) ONLY if it recurs at a
second window/seat. Does NOT clear the HIGH adopt-a-revision bar this wave; guide FROZEN.

## Q6 — colored-pays-generic false belief: NO new instance this seat.

The wave-10 recurrence (deck110 s21: declined offered `{1}{u}`/`{1}{g}` casts believing colored can't pay
generic) did NOT reproduce. Scanned every reply mentioning "generic" + a payment negation: the model got
the inference RIGHT in every window, including explicit self-corrections — deck131 s52
*"Icehide Golem costs {1}. I can pay {1} with {w}? No, {1} is generic. I can pay it with any color. I have
{w} available."*; deck44 s28 *"I still need {1} generic. I have the Plains {w} which can pay generic."*;
deck133 s5 *"Into the North costs {1}{g}. I can pay {g} and {1} (generic)."* No offered-payable decline on
the false belief. Consistent with the wave-10 read that the belief is model-UNSTABLE (oscillates wrong on
long derivations, right this corpus), NOT un-taught. The parked CORE candidate does NOT harden from this
seat this wave; the cross-seat grep for a 2nd deck remains the promotion trigger (general-suggestions.md).

## Q7 — protocol / combat / spot-checks.

- **Answer-last protocol CLEAN, intent-collapse stays DEAD.** All 6 seat files: **0 numeric-headed replies**
  (every reply opens `PLAN:`). No head token exists to lock; the wave-8/9 head-first collapse is
  structurally impossible and did not reappear. (Note the wave-10 `intent-collapse-metric.py`
  reversal-flag count remains an unreliable pointer, not a defect count — HARNESS #1, carried.)
- **Combat clean.** deck131/deck44 pushed real Treefolk pressure; deck109 blocked profitably early; no
  reflexive high-life chump; Boreal-Druid mana carve-out held (attacked only when last body / profitable).
- **Legal-target-names render** correctly (deck133 s34 `Cast Gelid Shackles {w} - legal targets right now:
  Bloodghast`). The one wave-10 representation MISS (Force "NO legal target" contradiction) is now FIXED.
- **Fallbacks: 19 empty_reply** this seat (deck44 8, deck133 4, deck131 4, deck140 2, deck109 1, deck110 0)
  — the KNOWN corpus-wide HTTP-120s-timeout class on 4096 tail generations (240s harness fix shipped next
  corpus). The fallback TYPE shifted from wave-10's `unparsed_reply` (2048 truncation) to `empty_reply`
  (timeout), as expected. All self-healed; **none changed a game** — deck44's 8 are all late (T15-17) in an
  already-lost game and the heuristic still played lands/Search/fetches.
- **adj rows sane:** deck131 seat final 1/19 T15 == results `135 131 adj1 1 19 15`; deck140 seat final
  17/16 T13 == `135 140 adj0 17 16 13`.

## WAVE-10 LEDGER STATUS

CONFIRMED / VALIDATED this wave:
- **Stack-target annotation fix** — VALIDATED: "NO legal target" contradiction 0/corpus; Force reasoning-tax
  ~470w -> ~100w; Force discipline restored (Q1).
- Develop-imperative (Rule #1 point 3 rewrite) — HELD; no dead-board recurrence (Q4).
- Fetch consume-on-choose — HELD; 61 windows (~flat vs 66); re-ask reduced 3->2 windows/crack (Q3).
- Answer-last protocol — intent-collapse stays DEAD (Q7).
- Force discard-exclusion clause — OBSERVED OBEYED again (deck133 s37).

CARRIED / PERSISTING (all routed OFF the guide):
- **Force free alt-cost never counters** — ENGINE #1 (carried; next cycle's top fix). No new data this
  wave (1 cast, adjudicated mid-resolution).
- **Fetch chosen-but-unresolved 1-window re-ask + duplicate-identical option** — ENGINE #2/#3 (minor).
- **Azcanta-transform card-fact hallucination** (deck44 s34) — MODEL watch (Q5).
- **colored-pays-generic** — did NOT recur this seat; CORE candidate parked, cross-seat grep is the trigger.
- Manabase strands 2-color creatures / slow clock — standing construction flags.

## GUIDE DISPOSITION: FROZEN (byte-identical copy of the live guide; `cmp` clean, 13357 bytes).
No new guide-fixable failure class. Every residual routes off the guide: Force -> engine (carried);
fetch re-ask -> engine; Azcanta hallucination -> model watch; colored-pays-generic -> core (no recurrence);
develop misses -> lost-position judgment covered by existing anchors. Nothing demotable (pause-aware:
phantom-lethal anchor had 0 test windows = untested; uncastable-plan anchors still leaking; the discard
clause has a single obey window). Full freeze-check in notes.md.

## GAME -> FILE MAP (run = matchups-20260715-072254)
| seat epoch | opp | outcome | turns | final (me v opp) | result row |
|------------|-----|---------|-------|------------------|------------|
| 1784118177 | vs140 | WIN (adj0) | 13 | 17 v 16 | `135 140 adj0 17 16 13` |
| 1784118181 | vs109 | LOSS       | 11 | -2 v 17 | `135 109 1 -2 17 11` |
| 1784120577 | vs133 | LOSS       | 19 | -2 v 19 | `135 133 1 -2 19 19` |
| 1784124329 | vs131 | LOSS (adj1)| 15 | 1 v 19  | `135 131 adj1 1 19 15` |
| 1784124541 | vs44  | LOSS       | 18 | -7 v 18 | `44 135 0 18 -7 18` |
| 1784126886 | vs110 | LOSS       | 7  | -2 v 20 | `135 110 1 -2 20 7` |
