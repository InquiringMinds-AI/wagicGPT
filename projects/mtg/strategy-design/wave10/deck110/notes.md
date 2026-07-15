# Deck-110 wave-10 — development notes (self-contained; layer-routed engine/harness ledger)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-031825/`, 6 deck110 seat translogs
(`*ai_baka_deck110-*.jsonl`). Binary `/tmp/wagic-c8c054be8`, wave-9 guides deployed,
answer-after-plan reply protocol, max_tokens 2048. Card facts vs
`projects/mtg/bin/Res/sets/primitives/mtg.txt`. Choices are 1-indexed (`CHOICE: 1` = option 1).
Each item carries a full repro (engine items belong HERE, never in prompt/guide text).

## E1 — CLOSED (4th running wave): the re-equip "(ALREADY attached... change NOTHING)" marker.
Wave-6 inert -> wave-7 fired+obeyed -> wave-8/9 stable -> wave-10 stable.
REPRO: for each seat log, count `options_text` lines containing `ALREADY attached` (marked offers)
and `chosen_text` containing `ALREADY attached` (taken). Wave-10: **31 marked offers, 0 taken; 11
Equip picks, 0 marked no-ops.** Marker renders exactly as `(ALREADY attached to it - this would
change NOTHING)`; the two-power cue renders as e.g. `Signal Pest (4/1) (printed 0/1)`. Field fix at
`src/AIPlayerGPT.cpp:1163-1173`. No engine or guide work. Guide Rule #1 stays frozen verbatim.

## E2 — ENGINE/REPRESENTATION (PROMOTED from watch to CONFIRMED COSTLY): the equip re-offer's
## no-op marker is keyed on TARGET-IDENTITY and is blind to a NET-ZERO shuffle between EQUIVALENT
## instances (multi-copy churn, wave-9's carried watch, now fired costly).

Status change: wave-7/8/9 kept this as a low-priority watch ("did not recur as a costly loop").
Wave-10 it FIRED costly twice.

REPRO 1 (COSTLY, in a LOSS) — deck109 seat `...-vs-ai_baka_deck109.jsonl`, T8, my3/op9, ONE
Cranial Plating + TWO identical Ornithopters (both `(2/4) (printed 0/2) [counters: 2x +1/+1]`):
- seq27: Plating `[attached to: Steel Overseer]` (8/3) -> `CHOICE:` equip to Ornithopter (opt
  unmarked). Defensible (evasive carrier at low life).
- seq28: Plating `[attached to: Ornithopter]` -> options are `[Ornithopter (ALREADY attached...),
  Steel Overseer, Ornithopter]`; picks `CHOICE: 3` = the OTHER (unmarked) Ornithopter.
- seq29: on Ornithopter -> picks the unmarked Ornithopter again. seq30: same, `CHOICE: 3`.
Net board change of seq28-30 = zero; cost = 3 wasted `{1}` equips in a turn deck110 needed to
attack, at 3 life in a race it lost. The `(ALREADY attached... change NOTHING)` marker fires only
on the instance CURRENTLY holding the Plating, so every move to the equivalent OTHER Ornithopter is
UNmarked and reads as a genuine state change.

REPRO 2 (net-zero, in a WIN) — deck131 seat, T7, my18/op14: seq24 Plating Vault Skirge -> Signal
Pest; seq25 Signal Pest -> Vault Skirge. A->B->A within one turn, ~2 mana wasted.

ROOT CAUSE: the no-op predicate is "already attached to THIS target," which is not the
no-value predicate. Two churn shapes evade it: two Platings (wave-9 E2) and two equivalent carriers
(this wave). ENGINE LEVER (representation): compute the equip re-offer's no-op/low-value marker
from the OUTCOME DELTA — mark/suppress "Equip ... targeting X" whenever moving the equipment to X
would NOT increase X's resulting power above the current carrier's (equivalent-or-worse carrier).
That single predicate catches both shapes. Alternatively/additionally, suppress additional
same-turn re-equips of an equipment that already moved this turn unless a strictly-higher-power
carrier is available. Owner: `AIPlayerGPT.cpp` equip-option serialization (same area as the
`[artifact]` tag + marker emission). Priority raised to MEDIUM (costly, recurrent, 2 games). NO
guide text — the guide (Rule #1 "equip once... only when a strictly better carrier arrived")
already forbids the behavior; the pilot has no surfaced gain signal to obey it.

## E3 — ENGINE/REPRESENTATION: artifact-tag completeness SHIPPED and the count now AGREES; the
## RESIDUAL reasoning fold-in persists (representation wall) and a NEW hand-card type sub-gap.

SHIPPED & VERIFIED (the wave-9 headline lever): every artifact on the "Your battlefield" line now
carries `[artifact]` — artifact CREATURES (Memnite/Ornithopter/Signal Pest/Etched Champion/Steel
Overseer/Vault Skirge/Master of Etherium), the artifact LAND Darksteel Citadel, Mox Opal, Cranial
Plating — while Glimmervoid + basics are UNtagged. REPRO (deck140 seq20):
`Island; Mox Opal [artifact]; Darksteel Citadel [artifact]; Cranial Plating {2} [artifact];
Mountain; Glimmervoid`. A re-derived count (count of `[artifact]` on the battlefield line) AGREES
with the summary `Artifacts in play: you N` in **177/177 windows, zero offset**. The wave-9 E3
representation lever is CLOSED.

RESIDUAL (representation-beats-instruction WALL, 3rd wave, NON-outcome-affecting): the pilot still
re-derives by hand and folds non-artifacts in:
- Glimmervoid folded (a LAND): deck131 seq6 surf `you 2` "play glimmervoid to increase the artifact
  count to 3, activating metalcraft"; deck135 seq9/10 surf `you 2` "3 artifacts (signal pest,
  cranial plating, glimmervoid), triggering metalcraft."
- Basics folded: deck140 seq24 surf 3 "play the mountain to increase my artifact count"; deck133
  seq4 surf 1 "artifact count to 3 (mountain, mox opal, ornithopter)."
- NEW sub-gap (UNDER-count, hand cards): deck133 seq17/18 surf 1 "Etched Champion is not an
  artifact... Vault Skirge is not an artifact... only Mox Opal is an artifact." Both ARE artifact
  creatures. The `[artifact]` tag lives on the BATTLEFIELD line only; a card in HAND carries no
  type tag, so the pilot guesses and errs. CANDIDATE MICRO-LEVER: tag artifact cards in the HAND
  serialization too (e.g. `Cast Etched Champion {3} [artifact creature]`) so the pilot's forward
  projection ("cast X to reach N") uses the right type. Low priority — non-outcome-affecting.

OUTCOME IMPACT: none proven (3rd wave). No Galvanic Blast fired expecting metalcraft-4 at a sub-3
board (all 3 surf<3 Blast casts explicitly acknowledged "2 damage without metalcraft": deck109
seq5, deck133 seq6/13); no metalcraft-gated pick flipped; the deck floods past 3 too fast for the
boundary to be the deciding fact. Self-corrections now occur (deck135 seq29, deck140 seq9), a small
positive from the completer tags. GUIDE FROZEN — escalating the READ wording hits the wall.

## M1 — MODEL / REPLY-PROTOCOL WATCHES (single-seat, low confidence).
(a) **Plan/choice mismatch on an UNLISTED card (answer-after-plan variant).** deck133 seq6 T0,
options `[Cast Galvanic Blast, Cast nothing]`: PLAN reasons about casting Cranial Plating (not an
option) and concludes "I will hold Galvanic Blast for a later finisher," then emits `CHOICE: 1` =
Cast Galvanic Blast — the held card. Not head-first intent-collapse (head==choice==label); it is
plan-diverges-from-legality + a reconcile failure at the emit step. Route to the reply-protocol
layer; NOT guide-fixable. n=1. See general-suggestions #1.
(b) **Galvanic Blast burned as filler to satisfy "cast nothing = lost turn."** deck133 seq6 + seq13
spent BOTH Blasts by T4 for 2 face damage each; seq13 is a ~1500-word agonizing reply that quotes
the guide's "A turn where you cast nothing is a lost turn" to justify a value-negative early burn of
a finisher. NON-outcome-affecting: deck133 lost my-1 vs op21 (a 22-life material blowout; deck110
drew no creatures and never contested). n=2 in ONE lost, non-contested game. A per-deck guide
carve-out ("a held finisher/removal is not a lost turn — do not fire Galvanic Blast for 2 chip
damage just to avoid a blank turn") is a CANDIDATE only if a 2nd seat/game reproduces it in a
CONTESTED game where the wasted burn could have closed. Guide FROZEN meanwhile.
(c) **Phantom-lethal / single-attacker-into-blocker (wave-9 M1 carried watch): did NOT recur.**
No single-attacker "asserted lethal into an untapped blocker" this corpus; deck44 seq25 declared
2 attackers for a correct 4+3=7 lethal (opp at 7, won). Freeze the RACE/attacker clauses.

## DC1 — DECK-CONSTRUCTION / META (standing, reaffirmed — owns the loss column).
deck110's 4 losses share one precondition the guide cannot create: **few creatures drawn into a
flood deck.**
- vs133 (Asphodel/Bloodghast midrange), LOSS my-1 vs op21: deck110's only T0-T6 plays were Galvanic
  Blast + Thoughtcast — no creatures drawn. A 22-life blowout, never contested.
- vs135 (Modern Snow), LOSS my7/op16: stalled for bodies (it 20-0'd this same opp in wave-9 going
  wide — pure draw variance).
- vs109 (Hellrider aggro), LOSS my-5: got opp to 5 by T4 then stalled for bodies (+ the E2 churn
  turn burned T8); the RACE the guide already teaches, shortfall was material.
- vs140 (Staff-of-Nin grind), LOSS-adj my17/op18: behind by 1 at the cap; the known deck140 grind.
Not a guide task. If the decklist is ever tuned: trim ~0-power filler (Signal Pest 0/1, Ornithopter
0/2) and/or the Mox/land count toward more mid bodies + reach/evasion-block redundancy, to cut
flood + single-threat exposure + grind exposure at once.

## Positive validations (no action, continuity)
- Answer-after-plan protocol killed intent-collapse at this seat: 0 true head-vs-choice mismatches
  across 169 ask/priority; `CHOICE:` label present 163/169 (6 gaps = truncations). Confirmed
  positive at deck135 seq14 (a would-have-collapsed window resolved to the plan).
- `intent-collapse-metric.py` flags (135 corpus / 16 deck110) are FALSE POSITIVES under this
  protocol (crude option-word matching; the real `CHOICE: N` agrees with the reasoning). Not
  comparable to the 120-flag head-first control baseline.
- Defer = 2 (deck140 seq3, deck131 seq3, both T1) = the deliberate Glimmervoid gate (expected).
- Fallbacks = 13 (11 unparsed_reply truncations at the 2048 cap + 2 defer); NONE changed a game
  (combat truncations in lost positions; 2 even-game truncations at deck135 T5 s10/s12 had no
  provable outcome effect). 4096 already shipped for future runs.
- Blocker seam clean (4 windows); no reflexive high-life block; attack-seam survival clause behaves.
