# Deck 44 (UB Faerie Tempo) — Wave 9 review (FINAL wave before pause)

Corpus: PRIMARY `matchups-20260714-094942/`. deck44 = **deck0 in all 6 games** (confirmed by
`gamestart`: `my_deck=ai_baka_deck44`). Binary `/tmp/wagic-a40671057` adds the **own-targets
annotation** ("the only legal targets are YOUR OWN right now") among other oracle/seam changes.

**Record 4W-2L.** WON vs140 (20/-5 T14), vs133 (4/-2 T14), vs135 (2/-4 T16), vs131 (18/0 T22).
LOST vs109 (0/17 T11), **vs110 (-12/13 T13)** — vs110 was a WIN last wave (7/-11). Waves 4->9:
3/6 -> 2/6 -> 3/6 -> 5/6 -> 5/6 -> **4/6**. Win column is context only (doctrine); the two losses
are what the decision audit is for.

game->file map (via `gamestart` opp_deck + results.tsv life/turn; start-epoch order != game order):
`1784040585`=vs140 W . `1784040589`=vs109 L . `1784041072`=vs110 **L** . `1784044834`=vs133 W .
`1784046476`=vs135 W . `1784046898`=vs131 W. Citations: `vsNNN sN`.

Posture: **FROZEN-SPINE / turnaround, 6th wave running.** The spine (hold {U}{U}, sweeper-first
counters, cast a faerie every turn) is intact and still won 4. The corpus yields ONE clean
representation WIN to record (the own-targets annotation) and ONE decision-level failure class in
the vs110 loss (counter-priority INVERSION) that earns the wave's single tightening.

---

## HEADLINE 1 — the OWN-TARGETS ANNOTATION is a clean confirmed-fired representation WIN

Wave-9 verification item for deck44 ("own-targets annotation at GFTT windows — did the reasoning tax
drop?"). **Answer: yes, cleanly, and with zero self-target casts.**

The annotation `1. Cast Go for the Throat {1}{b} - the only legal targets are YOUR OWN right now`
renders exactly at the GFTT windows where the opponent has no nonartifact creature but the pilot's
own faeries are legal targets. It fired **47 times** in deck44: vs110 (22), vs135 (12), vs131 (14).
It correctly did NOT fire in vs133/vs109/vs140 (GFTT had a real opponent target, or the pilot held
no creature so there was no "your own" target to name).

Two signals, both positive:
1. **0/47 self-target casts.** Across every window the annotation was present, the pilot chose "Cast
   nothing" or an unrelated creature/counter — it NEVER cast Go for the Throat into its own faerie.
2. **Reasoning tax DROPPED.** Plan length at GFTT-decline windows: WITH annotation (own=True) median
   **519 chars** (n=47); WITHOUT it but plan still deriving "no target" (own=False) median **871**
   (n=5, mean 1923). Qualitatively sharper: annotated plans are short and decisive, echoing the
   annotation almost verbatim — vs131 s7 *"Do not cast Go for the Throat as it has no legal targets
   (only my own creatures are on the battlefield)."* Where the annotation is ABSENT, the pilot
   self-negotiates — vs131 s54 *"...(Young Pyromancer is an artifact? No, Young Pyromancer is a
   creature, but it is on the battlefield. Wait, the prompt says..."* — a 900+ char loop the
   annotation would collapse to one line. The confusion appears ONLY where the annotation is missing.

**GFTT no-target fallback: KEEP, do NOT demote.** Wave 8 kept it because the GFTT option was offered
bare. This wave the representation the wave-8 note asked for has shipped as this annotation, so for
the own-creature case the fallback is now double coverage — BUT the annotation only fires when the
pilot HAS its own creatures as legal targets. When the pilot holds NO creature AND the opponent has
no nonartifact creature (GFTT has ZERO legal targets), the annotation cannot fire and the option is
still bare — exactly the vs131 s54 confused window. Per per-belief demotion granularity, demote only
what the surface answers: the surface answers the own-creature case, not the zero-target case →
partial coverage → keep (also final-wave/frozen-spine caution). Documented for a post-pause wave to
finish the demotion once the zero-target edge is also represented.

---

## HEADLINE 2 — the vs110 LOSS: the counter seam INVERTED (the wave's one decision-level failure)

vs110 (affinity) was a WIN last wave, a LOSS this wave (-12/13). NOT a development/durdle loss — the
pilot cast a creature at **every** castable-creature window (s4, s7, s37, s44; 0 declined) and
deployed its whole reactive-half hand. The loss is a **counter-priority inversion**:

- **s31 (L20) — let Steel Overseer resolve.** A guide-NAMED category-#2 counter target AND a
  textbook category-#3 growing engine: it pumps EVERY opponent artifact creature +1/+1 each turn and
  the pilot can never Go for the Throat it. Plan: *"it buffs other artifacts, but it is not a board
  sweeper... I can deal with it later if necessary."* The exact INVERSE of the rule's intent — it is
  on the counter list *because* you cannot deal with it later.
- **s21 (L20) — let Cranial Plating resolve** (also #2). Softer (no creature to equip yet), same
  "slow artifact, ignore it" read.
- **s33 (L20) — countered Galvanic Blast** (2-4 face burn) with Arcane Denial, plan explicitly:
  *"The Steel Overseer is a slow threat I can ignore for now."* So it spent its counter on cheap face
  burn at 20 life (handing them 2 cards) while the growing engine it had just waved through built the
  board that killed it.

**Root cause (guide-fixable).** Category #2 listed these artifacts but carried the WHY only
implicitly. The weak executor completes "an artifact you cannot Go for the Throat" into its own
instinct — *"can't remove it → but it's slow → ignore it"* — which is backwards. And the value-floor
never named FACE BURN, so the pilot defaults to countering damage-to-me even at high life (Galvanic
Blast; Searing Spear vs109 s16; the whiffing Cruel Edict vs140 s12 with 0 creatures on board).

**A class, not a one-off:** the game-losing inversion in vs110 (s31+s33) plus the "counter cheap
face damage" instinct in vs140 s12 and vs109 s16. Decision-level, pre-computable, named cards →
guide material.

---

## THE WAVE-9 TIGHTENING (single edit, additive/sharpening — no rewrite, spine frozen)

Per method (name the anti-pattern the executor invents; carry the WHY as a flat imperative; tie to
named cards). This SHARPENS a priority the pilot inverted — does NOT widen a permission, and leaves
sweeper-first (#1) intact so counters stay reserved for sweepers when a sweeper deck is across.

1. **Counter category #2 gets its urgency + WHY:** "you have NO removal for these — a counter is your
   ONLY answer, counter ON SIGHT, no 'deal with it later.' Steel Overseer pumps their whole artifact
   board +1/+1 EVERY turn it lives." (kills the "slow artifact = ignorable" bridge with a flat fact.)
2. **Value floor names face burn:** "Let cheap burn to YOUR FACE resolve and race — Galvanic Blast,
   Searing Spear, Lightning Bolt, Cruel Edict — unless lethal/near-lethal; at 15+ life taking 2-4 is
   cheaper than an Arcane Denial that draws THEM two cards." (closes the counter-the-burn default.)
3. **SITUATIONS recap** gains one line pairing the two (counter the growing artifact engine; take the
   cheap burn and race).

Validated (post-pause, if resumed) by: the counter-inversion class -> 0 vs an affinity seat, and no
over-countering of artifacts when a sweeper deck is across.

---

## RE-CONFIRMED / FROZEN (all held; no change)
- **Plan/choice mismatch: 0** across the whole corpus (0 head-int vs choice mismatches over ~18
  counter windows + 47 GFTT windows). Wave-8 P2 closed watch STAYS closed — largest sample yet. The
  plan-binding sentences obeyed perfectly.
- **Counter spine on SWEEPERS/ENGINES: correct where it matters.** Correct takes: vs140 s23 Lightmine
  Field, vs135 s28 Abominable Treefolk, vs135 s36 Diamond Faerie (L6), vs131 s34 Young Pyromancer,
  vs131 s57 Hydrolash. Correct let-resolves: vs140 s20 Venser's Journal, vs133 s7 Inquisition, vs135
  s17/s18 small bodies, vs110 s19/s22 Ornithopter/Thoughtcast. The inversion is specifically the
  un-GFTT-able GROWING artifact vs cheap face burn — that one axis, addressed above.
- **Own-targets annotation: 0 self-targets / 47 windows.** **GFTT no-target fallback: 0 drops, KEEP.**
- **Obliterator no-block absolute + GFTT-hold: UNTESTED again** (vs133 Obliterator not a board factor
  — 1 fleeting prompt mention, 0 GFTT casts all game). Frozen, keep.
- **Gatewarden first-strike carve-out (wave-8 micro-diff): UNTESTED** — vs109 had NO
  blocker-declaration phase (fast race; removed threats with GFTT-target selection). Frozen, keep.
- **Reflexive high-life block of a win-con (wave-8 vs110 s19 watch): did NOT recur** — vs110 had no
  blocker phase this wave. Route stays REPRESENTATION (a `[deals 0]`/`[not lethal]` block-seam rider).
- **Mulligan, the engine, all card notes: verbatim.**

## HARNESS-DEFECT SWEEP (run first, per skill)
- DESYNC / head-choice mismatch: 0. ENGINE NO-OP: none. VALIDATION-DROP (deck44): 0.
  `deferred_to_heuristic`: none. Fallbacks/unparsed/timeouts: 0 (all 6 games 11-22 turns).
  NUMERIC-OFFSET / X-menu: N/A. Corpus-wide: still grep ALL `game-*.stderr`.

## THE TWO LOSSES — ownership
- **vs110 (L, -12/13):** DECISION-LEVEL (counter inversion) on top of a reactive-half keep. The
  inversion is the model-fixable part; the slow reactive keep is the construction residual.
- **vs109 (L, 0/17 T11):** fast mono-red out-raced a slow keep; play reasonable (deployed, removed
  threats, countered where it could). Owner: construction + variance. No blocker phase arose.

## RESIDUAL (construction, not guide) — persists waves 5-9
16 Island / 10 Swamp + 6 reactive cards. Counters earn their slots (protected the 4 wins). "Trim
the counters" is the WRONG lever. The all-reactive slow-keep rate loses to fast ground clocks (vs109;
vs110's reactive keep) — sharper levers are manabase black-density vs black spell load, and/or 1-2
more cheap flyers over the 3rd/4th reactive card. Route to the user (notes.md).

## FREEZE-CHECK (every kept line accounted for)
All wave-8 load-bearing lines survive VERBATIM: #1 cast-a-creature; the engine block; ATTACK +
lethal-check; the BLOCKING block (Obliterator never-block absolute + Gatewarden designated-blocker +
growing/killable carve-out + wither-through-first-strike + 9+ no-block naming Archmage + ≤8 chump);
counter priority #1/#3/#4 + counter-value floor + BOTH plan-binding sentences + hold-{U}{U} + "cast
before hold"; GFTT (target Obliterator) + no-target fallback; mulligan; all card notes; SITUATIONS.
CHANGES (additive/sharpening only, no reword of an obeyed line): (a) counter category #2 gains the
"counter on sight / no other answer / Steel Overseer pumps every turn" WHY [sharpen the inverted line
with the named card]; (b) value floor gains the face-burn let-resolve carve-out with named cards; (c)
one SITUATIONS line pairing the two; plus additive observed-pool card names (Thoughtcast, Gore-House
Chainwalker, Ice-Fang Coatl, Lightmine Field, Ornithopter/Steel Overseer to the GFTT can't-hit list).
Displacement audit: ~+9 lines net, nothing removed, still ~one screen at deck44 reference density.

## One-line handoff
The top deck's spine is frozen and still won 4; the own-targets annotation is a clean representation
win (0/47 self-targets, reasoning tax dropped); the whole model-fixable diff is one counter-priority
tightening from the vs110 loss — counter the un-answerable growing artifact engine (Steel Overseer)
on sight, and let cheap face burn resolve and race instead of spending a counter on it.
