# Deck-135 (Modern Snow) — Wave-10 game review

Corpus: PRIMARY `matchups-20260715-031825` (21-game round-robin over 44/109/110/131/133/135/140;
binary `/tmp/wagic-c8c054be8`; wave-9 guides live = current `deck135_strategy.txt`; **ANSWER-LAST
reply protocol** — scratch reasoning -> PLAN: -> CHOICE:/ATTACK:/BLOCKS: final line; max_tokens 2048).
All six deck135 seat games read decision-by-decision from the translogs; opponent seats read at the
Force/combat seams. Doctrine applied: decision-level opportunity-conversion audit; the win column is
CONTEXT ONLY. Game->file map at bottom.

## RECORD (context only — NOT a verdict) — 2W / 4L

| game    | opp (name)              | outcome (deck135)                 | my/opp life | turn |
|---------|-------------------------|-----------------------------------|-------------|------|
| 135v131 | 131 Mind Control        | **WIN**                           | 18 / -2     | 16   |
| 135v110 | 110 Etched Affinity     | **WIN** (adj0, life-cap)          | 16 / 7      | 11   |
| 135v133 | 133 Phyrexian Asphodel  | LOSS (adj1 — near-win, by **1**)  | 2 / 3       | 14   |
| 135v44  | 44 Faerie Archmage      | LOSS (near-win — opp at **2**)    | -3 / 2      | 16   |
| 135v109 | 109 Hellrider           | LOSS                              | -6 / 18     | 13   |
| 135v140 | 140 Wipe Them Out!      | LOSS (was TIMEOUT last wave)      | 0 / 32      | 23   |

Comparable to / mildly better than wave-9 (1W/4L/1TO): vs110 flipped L->W (adj); vs133 flipped a
completed loss into a near-win adjudicated by ONE life; vs44 again a near-win (opp 2); vs140 no longer
times out (now an adjudicated loss at the cap). Every loss is opponent-life-near-full or a slow-clock
grind — construction/variance-bound. This remains an OBEYED-BUT-LOSING / turnaround-mode wave: freeze
the obeyed spine; every residual routes OFF the guide (engine / core / model). Win-rate is context only.

---

## Q1 — PROTOCOL SHIFT (answer-last): the intent-collapse class DIED. Clean headline.

This corpus is the first under **answer-after-plan** (CHOICE emitted on the LAST line, after the PLAN).
The wave-8/9 head-first intent-collapse (a numeric HEAD committed BEFORE the model reasons, then the
PLAN reverses but the choice stays locked to the stale head — deck135's textbook vs131 s14 in wave-9)
is now **structurally impossible**: there is no head to lock.

`tools/intent-collapse-metric.py` on the six deck135 seat files:
- **total ask/priority records: 246 | non-numeric heads: 246** (every reply now begins `PLAN:`, not a
  number — the answer-last signature) | **last-bare-int differs from head: 0**.
- 30 "reversal-language + concluded-option != recorded choice" flags — **all 30 are heuristic FALSE
  POSITIVES**: the tool's `plan_concluded_option` reports 0-indexed guesses and matches intermediate
  mentions, so it fires on an off-by-one or a mid-plan aside. On manual read of every flag, the recorded
  CHOICE equals the plan's FINAL concluded option. Cleanest proof = **deck109 s18**: the PLAN reasons
  toward Into the North/Search, then REVERSES ("Icehide Golem IS a castable creature ... So I must cast
  Icehide Golem"), and `CHOICE: 4` = Cast Icehide Golem — the reversal is CAPTURED, not lost. Under
  head-first this is exactly the shape that locked to the stale head; under answer-last it resolves
  correctly.

Verdict: the reply-protocol A/B that was "DUE" in wave-9 has effectively SHIPPED (this corpus IS the
"move CHOICE after the PLAN" arm) and it WORKS. Compared with the head-first control's 120 detector
flags, the deck135 seat shows 0 genuine collapses. Route the metric's index-offset false-positive to a
one-line tool note (notes.md) so synthesis does not misread 30 as real. **No guide or judgment action.**

## Q2 — FORCE OF NEGATION FREE ALT-COST = HARD ENGINE BUG (carried watch — CONFIRMED). Report prominently.

The brief's carried watch ("free-alt-cost side of Force of Negation") is now a **confirmed engine
defect, not a strategic misfire**: Force cast on the OPPONENT'S turn via its exile-a-blue-card alt-cost
**never counters** — the targeted noncreature spell RESOLVES anyway. Witnessed in two seats:

- **deck131 (WIN, but bled cards):**
  - s40 (T11): Force cast on **Downsize** targeting the 10/10 Treefolk. EVENTS s41: `Downsize:
    stack -> graveyard` and Treefolk shown **6/10** — Downsize RESOLVED (power reduced), NOT countered.
  - s41-s44 (T11): Force "cast" on **Elixir of Immortality**. EVENTS s42: `Elixir: stack ->
    battlefield` (resolved) while the pilot fires `exile a blue card from hand with Force` at s42/s43/s44.
  - s51-s52 (T13): Force on **Cyclonic Rift**. EVENTS s52: `Abominable Treefolk: battlefield -> hand`
    (Rift bounced it) + `Cyclonic Rift: stack -> graveyard` (resolved), Force cost paid — NOT countered.
  - Seat total: **3 Force casts, 0 counters; 7 `exile a blue card` cost-actions** (s42,43,44,49,52,53,54).
- **deck140 (LOSS):** s58 (T21, L13): Force on **Rakdos's Return** (big-X burn-to-face + hand strip).
  EVENTS s59: `Rakdos's Return: stack -> graveyard | Your Force of Negation: hand -> graveyard | Your
  life -6 (now 7)` — Rakdos RESOLVED for 6 to the face, Force went to the graveyard, no counter.

The option renders a **self-contradiction** that is also driving huge reasoning-tax:
`Cast Force of Negation with its exile a blue card from hand cost  - NO legal target right now - can
target on the stack: <SpellName>`. The wave-9 no-legal-target annotation is MISFIRING on a counterspell
(its "target" is a spell on the stack, not a permanent), so it prints "NO legal target right now" AND
"can target on the stack: X" at once. deck140 s58's reply is ~470 words of oscillation ("Is Force a blue
card? ... If I exile it ... No, usually a *different* card ...") caused directly by that contradiction +
having only Force itself in hand to exile.

Routing (BINDING): this is an **engine defect -> notes.md ledger**, NOT guide text. I did NOT re-word or
gut the guide's Force section — it encodes correct MTG strategy (Force board wipes) and is right the
moment the engine is fixed. **Flagged tension:** until the engine is fixed, the guide is actively
directing the pilot into a card-bleeding no-op that pitches the very blue cards the deck needs to cast
its {G}{U} creatures. That is an argument for engine-fix PRIORITY, not for writing the guide around the
bug (forbidden by layer-routing). See notes.md ENGINE LEDGER item 1.

Force strategic scoping, insofar as it can be judged behind the engine bug:
- Face-burn over-fire: **0** (watch holds).
- Discard-exclusion (wave-9 clause): **OBSERVED OBEYED** once — deck133 opp cast Thoughtseize (s12),
  the pilot did NOT Force it (correct). Single window; KEEP the clause (pause-aware, single obey).
- Card-draw declines: deck131 s26 (Hydrolash), s34 (Artificer's Epiphany), s36 (Prism Ring) — all
  correctly DECLINED, with the pilot reasoning the alt-cost cannot be paid (only Force itself in hand,
  no second blue card). These declines are on a CORRECT belief, NOT the parked false-belief (Q3).
- Soft misfires by the strict guide list (moot behind the bug): Elixir (lifegain/recursion) and Downsize
  (power-reduction, not a kill) are neither board-wipe/mass-bounce/kill-only-threat. Not worth a guide
  edit for one-offs on a broken card in a won game.

## Q3 — PARKED CORE CANDIDATE RECURRED (report PROMINENTLY): "colored mana can't pay generic".

**deck110 s21 (T6 MP2, L19/19, creatures:1 Golem in play).** Mana available `{g}{u}`. Options:
`1. Cast Into the North {1}{g}` · `2. Cast Search for Azcanta {1}{u}` · `3. Cast nothing`. The pilot
first reasons it **correctly** ("Search for Azcanta {1}{u} ... {1} is generic. So I can cast Search for
Azcanta"), then TALKS ITSELF INTO THE FALSE BELIEF and reverses:
> "The prompt says 'Mana available: {g}{u}'. ... I do not have a generic mana. So I cannot pay {1}{u}
> for Search for Azcanta because I lack the generic {1}. I cannot cast Into the North {1}{g} because I
> lack the generic {1}. I cannot cast any creature because they all cost generic mana. Therefore, I must
> cast nothing." -> `CHOICE: 3`.

This is a genuine decline of **offered, payable** casts ({G}/{U} each pays a generic {1}) on the exact
"colored mana can't pay generic / offered=payable" belief that was PARKED as a core candidate in wave-9
(vs140 s24). Cost here: one wasted develop turn in a WON game (dug next turn), so low material impact —
but it is the clean recurrence.

Two calibrations (both important):
1. **This is the SAME SEAT (deck135), a different game — NOT the cross-seat "2nd seat" that the brief
   says triggers core promotion.** A within-seat recurrence hardens the candidate; it does not by itself
   promote. The 2nd-seat trigger requires ANOTHER deck's reviewer to find the same decline (I flag the
   grep for synthesis in general-suggestions.md).
2. **The belief is model-UNSTABLE, not un-taught.** Same corpus, the pilot got the identical inference
   RIGHT three times: s25 ("Wait, {W} is a colored mana ... I can pay {1}"), deck133 s10 ("using the
   Plains' white mana to pay the generic portion ... I CAN cast Abominable"), deck140 s12 ("Into the
   North {1}{g}, which I can pay with one generic and one green mana"). So the wave-9 local point-3b
   Astrolabe patch is mostly working; s21 is an oscillation the model reasons itself into under a long
   derivation, on cards OTHER than Astrolabe (Search/Into the North).

Disposition: durable fix is **CORE** (deck-agnostic; same family as the general prompt's existing "Mana
pool: (none) read as I cannot cast" correction) — see general-suggestions.md OBSERVATION 1. I did NOT add
a 4th per-deck restatement (would be the "3rd copy" anti-pattern on a fact already surfaced by
offered=payable + stated in point-3b). Guide stays FROZEN on this.

## Q4 — FETCH CONSUME-ON-CHOOSE: works, large tax drop, no strategic damage.

Fetch-crack option windows (`Put in Play with` / `search basic land with` in options_text) this corpus:
deck131 10, deck133 12, deck44 15, deck140 9, deck110 9, deck109 11 -> **66 total, avg 11.0/game**, down
from the wave-9 control's **21.3/game** (~48% drop). Cracks STILL fire when the guide wants them:
- Cracks CHOSEN per game 4-7 (deck109 7), lands resolving from library 2-6/game — cracks happen and
  resolve freely.
- **Crack-for-missing-color works both ways:** deck131 s16/s19 crack for a Snow-Covered **Forest**
  (green); deck110 s6/s8/s9 + deck109 s6/s9/s13 crack for a Snow-Covered **Island** (blue). The wave-9
  green-blindness residual (crack Forest when short green) is exercised and obeyed.
- **No strategic damage:** every game deploys creatures on curve; no window where a needed crack was
  consumed-away and left the pilot color-screwed by the fix.

Minor residual (engine ledger, not guide): a crack CHOSEN but not yet RESOLVED is re-offered across a
couple of windows until it resolves — deck44 s5/s6/s7 chose `Put in Play with Misty Rainforest` three
times (targets flipping Island/Forest/Island as the board changed) but only ONE land entered and ONE
life was paid (EVENTS s8: `Misty Rainforest: battlefield -> graveyard | Snow-Covered Island: library ->
battlefield`, `life -1`). So 3 decision windows for 1 crack — decision-count inflation only, zero
resource loss. Far below the old every-window tax.

## Q5 — DEVELOP-BRANCH (Rule #1 point 3) rewrite: VALIDATED. wave-9's vs133 dead-board class DIED.

Wave-9's headline residual was vs133: 4 turns (T6->T10) of creatureless dead board, the pilot quoting
"Do not fetch lands if I have a creature to cast" while a green-fixing fetch sat one click away. **This
wave vs133 develops on curve to a near-win (2 v 3, adj loss by 1):** Astrolabe T0 (s3), Prismatic Vista
crack T2 (s5), Into the North T4 (s9, fetched Forest for green), Abominable Treefolk 5/5 T6 (s11),
Search T8. No develop-paralysis with a directly-castable fix appeared in any game. The option-list gate +
fetch-crack-for-missing-color rewrite is doing its job.

## Q6 — UNCASTABLE-PLAN HALLUCINATION residual (already covered; model-side; KEEP anchors).

Three windows where the pilot PLANS an unoffered bigger creature it wrongly believes is castable, and
under-develops:
- **deck44 s12** (T7 MP1, creatures:0): Ice-Fang Coatl IS offered (opt 2), but the plan asserts it can
  cast the unoffered Abominable Treefolk ("I have the necessary mana"), chooses Cast nothing — then
  casts Coatl at MP2 (s14). Point-1 slip; self-corrected same turn.
- **deck110 s19** (T6 MP1, creatures:1): plans the unoffered Treefolk ("using ... my two Arcum's
  Astrolabes"), Treefolk NOT in options (only 2 mana), casts nothing — instead of Search/Into the North
  to dig. Dug/deployed next turn; won the game.
- **deck109 s35/s36** (T12, L6, creatures:0, lost position): plans the unoffered Coatl claiming an
  untapped blue source that is tapped (`Mana available: {g}{g}{g}`, Coatl NOT offered), casts nothing.
  Dead regardless.

The option list already surfaces uncastability (a creature absent = unpayable), and the guide already
carries the corrective anchors (Rule #1 point 1 "read the OPTION LIST, not your hand"; KEY CARDS
Treefolk/Coatl "if NOT listed you can't pay yet"; DECIDING SITUATIONS "Your plan says cast Treefolk ...
but NOT in your cast list"). The pilot fabricates a mana-count over all of it — the representation-beats-
instruction wall. A 3rd/4th copy is forbidden and would not help. KEEP anchors at full prominence
(class still leaking -> not a demotion candidate); route the residual as a model castability/mana-count
hallucination watch (notes.md). No game was lost to it (self-corrected or already-lost).

## Q7 — combat / mulligan / representation spot-checks.

- **Combat clean.** deck109 blocked correctly and profitably (Golem blocks Legion Loyalist s4 / Boros
  Reckoner s21; Ohran Viper 1/3 blocks Gore-House Chainwalker s24 — walls/trades up). deck131/deck44
  pushed the 9/9-10/10 Treefolk for real pressure. No reflexive high-life chump; no missed must-block.
  Boreal-Druid mana carve-out held (attacked with Druid only when profitable/last body).
- **Legal-target-names render** (brief item 3): deck110 s30 `Cast Gelid Shackles {w} - legal targets
  right now: ...`. The one representation MISS is the Force "NO legal target right now" contradiction
  (Q2) — a counterspell wrongly getting the permanent-targeting annotation.
- **No phantom-lethal / board hallucination** (board-read anchor held, 0 recurrences).
- **adj rows sane** (brief item 7): deck133 seat final my/opp = 2/3 at T14 == results.tsv `adj1 2 3 14`;
  deck110 seat final 16/7 at T11 == `adj0 16 7 11`.
- **Unparsed_reply fallbacks** (brief item 8): 11 across the seat (deck131 2, deck133 3, deck44 1,
  deck140 2, deck110 3, deck109 0) — the known 2048-cap truncations (4096 already shipped). All self-
  healed: e.g. deck133 s10 (Into the North land-pick) fallback still fetched a Forest (EVENTS s11
  `Snow-Covered Forest: library -> battlefield`); s12 fallback passed and s13 cast Search + Golem. **None
  changed a game.** Worth noting only that deck133 is the adj-loss-BY-ONE, so the 4096 bump matters most
  there.

## WAVE-9 LEDGER STATUS
CONFIRMED / HELD:
- Develop-imperative (Rule #1 point 3 rewrite) — VALIDATED; vs133 dead-board class died (Q5).
- Fetch consume-on-choose — WORKS; tax 21.3 -> 11/game, no strategic damage (Q4).
- Force scoping (face-burn 0; discard-exclusion observed obeyed once) — HELD, but see the engine bug.
- Answer-last protocol — intent-collapse class DIED (Q1).

NEW / PERSISTING (all routed OFF the guide):
- **Force free alt-cost never counters** — ENGINE BUG, 2 seats (notes.md ENGINE #1). Carried watch closed.
- **"colored can't pay generic" false belief** recurred at this seat (deck110 s21) — CORE candidate
  (general-suggestions OBSERVATION 1); needs a cross-seat 2nd deck to promote.
- **Uncastable-plan mana-hallucination** — model-side; existing anchors KEPT (Q6, notes.md MODEL).
- **Fetch chosen-but-unresolved re-offer** (deck44 s5-s7) — minor engine residual (notes.md ENGINE #2).
- Manabase strands 2-color creatures / slow clock — standing construction flags (notes.md).

## GUIDE DISPOSITION: FROZEN (byte-identical copy of the live guide).
No new guide-fixable failure class this corpus. Every residual routes off the guide: false-belief ->
core; Force -> engine; uncastable-plan -> existing anchors + model; fetch residual -> engine. Full
freeze-check in notes.md. Nothing demotable (pause-aware: the phantom-lethal anchor had zero test windows
= untested; the uncastable-plan anchors are still leaking; the discard clause has a single obey window).

## GAME -> FILE MAP (run = matchups-20260715-031825)
| epoch | opp | outcome | turns | final (me v opp) |
|-------|-----|---------|-------|------------------|
| 1784103512 | vs131 | WIN        | 16 | 18 v -2 |
| 1784106391 | vs133 | LOSS (adj1)| 14 | 2 v 3   |
| 1784108791 | vs44  | LOSS       | 16 | -3 v 2  (near-win) |
| 1784108897 | vs140 | LOSS       | 23 | 0 v 32  |
| 1784109864 | vs110 | WIN (adj0) | 11 | 16 v 7  |
| 1784110926 | vs109 | LOSS       | 13 | -6 v 18 |
