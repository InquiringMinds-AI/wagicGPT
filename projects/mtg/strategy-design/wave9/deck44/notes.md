# Notes for wagicGPT development — deck44 wave 9 (self-contained; no wave-10 assumed)

Run: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260714-094942/`. deck44 = deck0 in all 6
games (confirmed via `gamestart`). **Record 4W-2L.** Corpus health for deck44: 0 desyncs, 0
fallbacks, 0 timeouts, 0 validation-drops, 0 head/choice mismatches; all 6 games 11-22 turns.
Win column is context only (doctrine). **This is the final wave before a project pause — every item
below is self-contained with a full repro.**

game->file map (map by `gamestart` opp_deck + gameend life/turn vs results.tsv; epoch order != game order):
- `1784040585` = vs140 WIN (20/-5 T14)   | `1784040589` = vs109 **LOSS** (0/17 T11)
- `1784041072` = vs110 **LOSS** (-12/13 T13) | `1784044834` = vs133 WIN (4/-2 T14)
- `1784046476` = vs135 WIN (2/-4 T16)     | `1784046898` = vs131 WIN (18/0 T22)

## What changed in the guide this wave (single tightening, from the vs110 loss)
Base = the deployed wave-8 guide. Additive/sharpening only, spine frozen:
1. Counter category #2 gained its WHY as a flat imperative — "you have NO removal for these, a counter
   is your ONLY answer, counter ON SIGHT, no 'deal with it later'; Steel Overseer pumps their whole
   artifact board +1/+1 EVERY turn it lives."
2. The counter value-floor now NAMES cheap face burn as a let-resolve class — "Let cheap burn to YOUR
   FACE resolve and race (Galvanic Blast, Searing Spear, Lightning Bolt, whiffing Cruel Edict) unless
   lethal/near-lethal; at 15+ life taking 2-4 is cheaper than an Arcane Denial that draws THEM 2 cards."
3. One SITUATIONS line pairing the two; plus additive observed-pool card names.

## Decision-level repro — the vs110 counter INVERSION (the loss)
File `1784041072-ai_baka_deck44-0x56430f3193f0.jsonl`. deck44 deployed a creature at every castable
window (s4,s7,s37,s44; 0 declined) — NOT a durdle loss. The counters were mis-prioritized:
- **s31 (L20/18): let Steel Overseer resolve.** Plan: "it buffs other artifacts, but it is not a
  board sweeper... I can deal with it later if necessary." Steel Overseer is guide-named category #2
  (un-GFTT-able artifact) AND #3 (growing engine: pumps every opp artifact +1/+1 each turn). The pilot
  had {u}{b}{b}{b}{b} open — could have countered.
- **s33 (L20/18): countered Galvanic Blast (face burn) with Arcane Denial.** Plan: "The Steel Overseer
  is a slow threat I can ignore for now." Spent the counter on cheap burn at 20 life (gave them 2
  cards) while the engine it waved through grew the board that killed it.
- **s21 (L20/19): let Cranial Plating resolve** (also #2) — softer, no creature to equip yet.
Class echo: vs140 s12 countered a whiffing Cruel Edict with 0 creatures on board (`1784040585` s12,
won anyway); vs109 s16 countered Searing Spear at L18 while being raced (`1784040589` s16, borderline).
The guide tightening above targets exactly this axis (un-answerable growing artifact vs cheap face
burn). Owner: GUIDE (the fix shipped this wave). Validate post-pause by the inversion class -> 0 vs an
affinity seat.

## Representation WIN to record — the own-targets annotation
`Cast Go for the Throat {1}{b} - the only legal targets are YOUR OWN right now` fired **47×** on
deck44 (vs110 22, vs135 12, vs131 14 — the games where the opponent had no nonartifact creature).
- **0/47 self-target casts** — the pilot never destroyed its own faerie.
- **Reasoning tax dropped** — median plan 519 chars at annotated windows vs 871 at un-annotated
  "no target" windows; the confused self-negotiation appeared ONLY where the annotation was absent.
  Repro of the confusion (annotation ABSENT): `1784046898-...-deck44` s54 ("Young Pyromancer is an
  artifact? No... Wait..."). Repro of a clean annotated decline: `1784046898` s7 ("only my own
  creatures are on the battlefield").
- **Residual edge (engine ask #1 in general-suggestions.md):** the annotation cannot fire when GFTT
  has ZERO legal targets (pilot holds no creature AND opponent has none) — the option renders bare and
  the pilot still gets confused. Add `[no legal target]` for that case; then the guide's GFTT
  no-target fallback can be fully demoted. Until then, KEEP the fallback (still load-bearing).

## Frozen / untested this wave (keep as-is)
- **Phyrexian Obliterator no-block absolute + GFTT-hold:** vs133 (mono-black) did not put Obliterator
  on the board (1 fleeting prompt mention, 0 GFTT casts all game; won on flyer tempo + counters).
  UNTESTED, frozen — the answer for when the hard draw appears.
- **Gatewarden first-strike carve-out (wave-8 micro-diff):** vs109 had NO blocker-declaration phase
  this wave (only Main phases; fast mono-red race). UNTESTED, frozen.
- **Reflexive high-life block of a win-con (wave-8 vs110 s19):** did NOT recur — vs110 had no blocker
  phase this wave. Route stays REPRESENTATION (block-seam `[deals 0]`/`[not lethal]` rider).

## Deck-construction flags (for the user / decklist owner) — persists waves 5-9
- **16 Island / 10 Swamp + 6 reactive cards (2 Counterspell, 2 Arcane Denial, 2 Go for the Throat).**
  The counters EARN their slots (protected the 4 wins: Lightmine Field, Abominable Treefolk, Young
  Pyromancer, Hydrolash, Diamond Faerie). "Trim the counters" is the WRONG lever. Sharper: manabase
  black-source density vs the black spell load (Obyra {u}{b} / Bladecrafter {2}{b} / GFTT {1}{b}),
  and/or 1-2 more cheap flyers over the 3rd/4th reactive card — to lower the all-reactive slow-keep
  rate that lost vs109 (fast aggro) and set up the vs110 loss (reactive keep: Surveilling Sprite + 3
  counters). Not guide-fixable.
- **The loss ROTATES by matchup/draw.** Wave 7 lost vs133; wave 8 lost vs109; wave 9 lost vs109 AND
  vs110. The reactive half is a slow keep against a faster clock — a variance signature. The one
  model-fixable loss factor this wave was the vs110 counter inversion (fixed above); the rest is
  construction/variance.
