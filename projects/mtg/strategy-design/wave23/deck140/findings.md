# Deck-140 review — wave 23 (RBW Control; forced-loss framing validation OWNER)

Deck: **"Wipe Them Out!"** — RBW control, 14 board wipes, ~2 creatures, finishers =
Rakdos's Return / Staff of Nin / Blightsteel Colossus. Binary `0e6361732`.
Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260724-013710`, 6 deck140 seat games.
Deployed guide under test = wave-22's revised strategy.txt (8567 B, byte-identical to
`wave22/deck140/strategy.txt` — the forced-discard interim rule WAS live this corpus).
`choice` 1-based; `-1` = heuristic fallback; `CHOICE: 0` = pass.

**Record: 2 W / 4 L** (140:2/6). From results.tsv:

| game | opp | outcome | class |
|---|---|---|---|
| vs131 | (rotated-in) | **WIN adj 27-20 T26** | clean grind; 3 empty_reply fallbacks in opening (outage) — no harm |
| vs27 | Zombies | **WIN adj 7-6 T27** | razor life-race won by firing Rakdos's Return as reach (X=6, X=8); X-menus clean |
| vs102 | mono-B Pox/discard | **LOSS death 24->-4 T26** | THE forced-loss seat; picks all correct; died to discard-ATTRITION, not a pitched sweeper |
| vs133 | (rotated-in, 5/6) | **LOSS death 27->-5 T17** | STRUCTURAL: raced (L15->L4 in ~4 turns); deployed survival tools too late |
| vs137 | GW Adventures | **LOSS adj 12-18 T12** | STRUCTURAL color-screw: 1 black source, could not cast its {B}{B} sweepers |
| vs109 | Mono-R Aggro | **LOSS death ~T8** | STRUCTURAL: sorcery-speed control can't race haste+burn (repeat of wave-22 vs109) |

**All 4 losses are structural or representation-driven — NONE is a SWEEP-TRIGGER,
sweeper-selection, Rakdos-hold, or FORCED-LOSS failure.** Every guide line was OBEYED
where it fired. Two continuing structural leaks (mono-R race, no early interaction)
recurred; the two new losses are the rotated-in deck133 (raced) and a color-screw vs137.

Tags: [OWNS (a) forced-loss framing — VALIDATED] · [CRUTCH-LIFECYCLE: demote forced-discard
rule] · [(c) INDEX-WINS validated — wave-22 stale-echo cluster GONE] · [3 fallbacks = the
s5-7 empty_reply cluster, no harm] · [pilot (e): SAME] · [STRUCTURAL loss column unchanged].

---

## HEADLINE — (a) FORCED-LOSS FRAMING VALIDATES; THE WAVE-22 CATASTROPHE DID NOT RECUR

The engine header renders correctly and the model obeyed it on every forced self-discard/
sacrifice. Verbatim header (vs102, rendered as an option prefix on each own-card choice):

> **FORCED discard/sacrifice OF YOUR OWN CARD: the opponent's effect ... forces YOU to
> discard/sacrifice one of your OWN cards from the list below - each option is a card YOU
> will LOSE, not something you affect or attack. Pick the card you can best AFFORD TO LOSE
> (usually your LEAST valuable: pitch a spare land or a redundant/dead card, and KEEP your
> best spells, answers, and threats).**

All forced-loss decisions were concentrated in **vs102** (a Smallpox / Necrogen Mists /
Tourach / Tinybones mono-B discard-attrition deck — a Pox-class opponent that force-discards
nearly every turn). Every one, with the pick audited:

| seq | T | effect | options | chose | verdict |
|---|---|---|---|---|---|
| s5 | 6 | Smallpox discard | Akoum Refuge[land], Pyroclasm, Plains[land], Pyroclasm, BSZ, Rakdos, Staff | **Akoum Refuge [LAND]** | OK pitched a land over 5 spells |
| s6 | 6 | Smallpox sac-a-land | Badlands, Plateau, Scrubland (all lands) | Scrubland [land] | OK all lands, any fine |
| s8 | 9 | Necrogen discard | Pyroclasm, Plains[land], Pyroclasm, BSZ, Rakdos, Staff | **Plains [LAND]** | OK pitched the land over sweepers/finishers |
| s13 | 13 | discard (no land in set) | Pyroclasm, BSZ, Rakdos, Staff | **Pyroclasm** | OK weakest/most-redundant sweeper; kept BSZ+finishers |
| s14 | 15 | discard (no land) | BSZ, Rakdos, Staff, Damnation | **BSZ** | OK kept Damnation (better sweeper) + both finishers |
| s16 | 17 | discard (no land) | Rakdos, Staff, Damnation | Damnation | ~ forced to give up last-wipe OR a finisher; kept both win-cons — defensible (below) |

**Contrast wave-22 (the class this delta targets):** last corpus deck140 pitched
**Damnation** then **Pyroclasm** — its two live sweepers — into a go-wide board with a LAND
sitting in the option set, then died to the board those sweepers would have cleared. This
corpus, with the SAME opponent archetype, it pitched the **LAND first** (s5, s8) and, once
no land remained, its **most-redundant sweeper first** (Pyroclasm s13, then BSZ s14) while
keeping Damnation + both finishers. The exact inversion the delta was built to fix is fixed.

**The death was NOT a pitched-sweeper catastrophe.** deck140 kept and USED sweepers the
whole game — Cruel Edict (s9), and it drew into and CAST **Wrath of God** at T21 (s21). It
died at T26 to slow discard-attrition (Necrogen Mists emptied its hand to hellbent; Tinybones/
chip ground it 24->-4 over many turns, ~2-3/turn), a structural control-vs-dedicated-discard
matchup problem — not a forced-loss mispick. The one arguable pick (s16 Damnation over the
two finishers, opp board = a lone 2/2 Solemn Simulacrum) was immaterial: life fell only ~2/
turn afterward, no creature swarm arose, and deck140 had a live Wrath to cast at T21. Keeping
both win-cons over a last sweeper for a single 2/2 is sound; the engine's "KEEP your best
spells, answers, and threats" correctly protected the finishers every time.

**Attribution caveat (crutch-lifecycle honesty):** the guide's interim forced-discard rule
was ALSO in the prompt this corpus, so I cannot fully separate engine-header credit from
guide credit with a counterfactual. But the engine header is now decision-local (fires at the
exact choice, with the actual options tagged), comprehensive (loss-not-cast + pitch-land +
keep-threats), and the model's own reasoning ranked sweepers correctly without any explicit
guide ranking. That is the crutch-lifecycle trigger: the durable fix has landed in the engine;
the guide rule is now double coverage. **-> DEMOTE (done in strategy.txt).**

---

## CRUTCH-LIFECYCLE APPLIED — forced-discard rule DEMOTED (strategy.txt)

Per the wave-22 predecessor's own recommendation ("after this diff deploys and validates the
seat is a clean rotation candidate; the durable fix is routed to the engine") and the skill's
crutch-lifecycle: the 11-line **FORCED TO DISCARD OR SACRIFICE** block is demoted to a 6-line
marker-anchored nod that explicitly DEFERS to the engine ("the engine already tells you to
give up your least valuable - follow it") and keeps ONLY the deck-specific residual the engine
cannot supply: the pitch ORDER (land -> most-redundant sweeper, Pyroclasm before Wrath/
Damnation/BSZ) and the named finisher list. The redundant DECIDING-SITUATIONS bullet is
re-anchored to the engine marker text. Net guide size ~flat (8567 -> 8865 B) because the
demotion offset three small observed-friction refinements (below).

## THREE SMALL OBSERVED-FRICTION REFINEMENTS (secondary; synthesis may veto)

1. **Black-source count (WHICH SWEEPER, +2 lines).** vs137 was LOST to color-screw: hand held
   Damnation {2}{B}{B} + BSZ {B}{B}{X} but only ONE black source (Badlands) in play, so
   neither was castable against a 3-creature board. The model reasoned this out CORRECTLY but
   at enormous length (its longest spirals are exactly these black-source re-derivations).
   Added a line: if the black sweepers aren't offered you're short a black source, not out of
   sweepers — play a black land, cast Wrath/Cruel Edict meanwhile.
2. **X-menu lethal-trust (THE #1 RULE, +2 lines).** vs27 s46: the model fired Rakdos's Return
   believing it could set **X=20** for lethal (opp at 20) — but the X MENU capped X at **6**
   (its real affordable X). It over-read its own mana. Added: trust the menu's offered values
   for lethal math; don't assume a bigger X than the menu lists.
3. **Rakdos grind-to-cap reach (THE #1 RULE below-lethal exception, +1 clause).** vs27 s50:
   at T25 in a grind-to-adjudication, opp hellbent (0 cards, so the discard is dead) and
   deck140 with no other play, the model fired Rakdos below-lethal (X=8, 14->6) as pure reach
   and WON the life race 7-6. The guide's stated exception ("3+ cards AND attacking") did NOT
   cover this; the model overrode correctly via judgment. The guide previously CONTRADICTED
   the winning play — added a tight clause sanctioning grind-to-cap reach so a rigid reader
   wouldn't hold the kill shot into a lost adjudication.

These three are corpus-evidenced and deck-specific, not speculative — but they are additions
during a demotion pass; flagged for synthesis veto. The demotion is the load-bearing change.

---

## (c) INDEX-WINS — the wave-22 stale-echo cluster is GONE

Wave-22's owned leak was a 4/4 false-positive: the BSZ ANNOUNCE_X menu tripped stale_echo
(model answered `CHOICE: N (Cast Black Sun's Zenith with X=N)`, the detector matched the
consumed parent cast -> discarded a valid in-range index -> heuristic). This corpus my X-menus
fired at **vs27 s36 (BSZ, X=5), s48 (Rakdos, X=6), s52 (Rakdos, X=8)** — ALL recorded
`choice=1` (biggest X) TRUSTED, zero fallback. The wave-22 shape did not recur; INDEX-WINS
trusts the well-formed in-range index. Strictly better than wave-22 (there harm was 0 only
because the heuristic argmax happened to match; here the model's own answer is taken directly,
lower latency, no FP). BSZ was otherwise DISCARDED not cast in vs102, so X-menu volume was low
this corpus, but every instance is clean.

## FALLBACKS — all 3 are the s5-7 empty_reply cluster (transient outage), zero harm

My only fallbacks: **vs131 s5, s6, s7** — all `choice=-1`, `reply` length 0, latency
~2502-2503ms (the uniform fast-fail HTTP error the brief flagged as the corpus-start outage
cluster: "deck131 s3-6 + deck140 s5-7"). Zero unparsed/retracted/deferred at my seat. All 3
fell on LOW-STAKES opening decisions — s5 land-drop (3 lands + no-land), s6 single-option
Elixir activation, s7 land-drop — exactly the decisions the heuristic handles reliably. Life
climbed 20->25 across the window and **deck140 WON that game 27-20**. The heuristic answers
during the outage did NOT hurt (no board impact). Pilot-correlation OPEN per brief, but at my
seat there is no adverse signal (empties are HTTP-level, not model-content).

## PILOT (e) VERDICT — SAME (repetition_penalty=1.05)

My seat reply-length: p50 1229 / p90 4586 / p95 7277 / max 13107 (n=157). Corpus-wide this
corpus is p50 1106 / p95 7784 / max 15901; wave-22 was p50 1029 / p95 8186 / max 16882. My
seat's p95 and max sit BELOW both the current corpus and wave-22 — **no spiral lengthening**,
tails marginally shorter. My longest replies (vs137 s7/s12, vs27 s13, vs131 s8) are all the
same shape: black-source / lethal-X mana RE-DERIVATION loops driven by the known mana-display
friction, NOT degenerate token repetition. Prose stayed coherent and reached the CORRECT
answer even in the 11-13k-char replies (vs137 s12 quoted the guide and concluded correctly).
No prose degradation, no structured-output harm. **Verdict at deck140: SAME.** The pilot
neither shortened nor lengthened my seat's spirals; the long tail is representation-driven and
would be cut by fixing the mana display (see notes), not by a sampling knob.

## OVERSHOOT / DISPLACEMENT AUDIT (mandatory)
1. **SWEEP TRIGGER / sweeper-selection:** obeyed; Cruel Edict on single creatures (vs27 s6,
   vs102 s9, vs133 s12/14, vs137 s4/6/10, vs109 s5/7), Pyroclasm/Wrath/BSZ on multi-boards,
   no cast-into-C=0. No overshoot, no displacement.
2. **#1 Rakdos-hold:** obeyed ~13x in vs27 (held below-lethal every offer T5-T21); the two
   fires (s46 intended-lethal but mana-capped, s50 knowing grind-to-cap reach) both won the
   life race. Not over-firing, not held-to-death. The residual is the MANA over-read (engine).
3. **FORCED-LOSS (owned delta):** validated; no overshoot. The demotion removes a now-double
   crutch — the residual moved OFF the guide onto the (settled) engine header.
4. **Residual sits at:** (a) ENGINE — mana-display friction (black-source count unreadable +
   over-optimistic lethal X), the source of both the longest spirals and the s46 mis-read;
   (b) DECK-CONSTRUCTION — no early interaction vs hasty aggro (vs109) / no answer to
   dedicated discard-attrition (vs102) / fragile black base for its black sweepers (vs137).
   None of it is guide-refinement. -> notes.md.
