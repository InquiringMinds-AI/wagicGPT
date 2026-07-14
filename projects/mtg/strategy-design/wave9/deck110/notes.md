# Deck-110 wave-9 — development notes (self-contained; PAUSE-clean)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260714-094942/`, 6 deck110 seat translogs
(`*deck110*.jsonl`). Binary `/tmp/wagic-a40671057`, wave-8 guides deployed. Card facts vs
`projects/mtg/bin/Res/sets/primitives/mtg.txt`. Because of the project pause each item carries a
full repro so no wave-10 context is assumed.

## E1 — CLOSED & RE-VALIDATED (3rd running wave): the re-equip "(ALREADY attached... change
## NOTHING)" marker.
Wave-6 inert -> wave-7 fired+obeyed -> wave-8 stable -> wave-9 stable at LARGER sample.
REPRO: for each `*deck110*.jsonl`, count decisions whose `options_text` contains a line with
`ALREADY attached` (=marked offers) and whose `chosen_text` contains `ALREADY attached` (=taken).
Wave-9: **47 marked offers, 0 taken; 14 equips chosen, 0 marked no-ops.** Marker string renders
exactly as `(ALREADY attached to it - this would change NOTHING)`; the two-power cue renders as
e.g. `Signal Pest (4/1) (printed 0/1)`. No engine or guide work. Keep the field fix at
`src/AIPlayerGPT.cpp:1163-1173`. Guide Rule #1 stays frozen verbatim.

## E2 — ENGINE/REPRESENTATION WATCH (unchanged from wave-7/8): per-instance marker blind to
## multi-copy churn.
The marker fires only on "already attached to THIS target." With two Cranial Platings out, moving
Plating-A off a carrier that holds Plating-B is a genuine, unmarked state change the marker cannot
flag. REPRO/status this wave: equip target sequences per game — vs140 had 7 equips over 29 turns
(Vault Skirge->Master->Etched Champion->Steel Overseer->Signal Pest->Signal Pest->Ornithopter);
the two Signal-Pest equips (seq42, seq48) are 6 turns apart across heavy board turnover, NOT an
A<->B loop. All other games had 1-2 equips, each a first-attach or a carrier upgrade. Churn did NOT
recur as a costly loop. Candidate engine fix stands: suppress/demote the equip re-offer once EVERY
copy of the equipment is attached to SOME carrier. Low priority; promote only if a second
multiples-of-an-equipment deck reproduces a costly instance. NO guide text.

## E3 — ENGINE/REPRESENTATION (SHARPENED, the wave-9 headline lever): the `Artifacts in play: you N`
## summary is accurate but the pilot RE-DERIVES from an INCOMPLETE per-permanent tag set and
## folds Glimmervoid + basic lands into the count.
Wave-8 flagged that the pilot ignores the accurate summary and hand-counts (E3, informational,
fixed at the guide layer via a READ instruction). Wave-9 is the first test of that guide edit, and
it landed only PARTWAY — the READ helped in the common case but the false belief survived. This
note upgrades E3 from "informational" to a concrete REPRESENTATION lever.

FACTS (all verified this corpus):
- The summary line `Artifacts in play: you N | opponent N` is 100% accurate — recomputed across
  every deck110 record, ZERO offset. It counts Mox Opal + Darksteel Citadel + artifact creatures
  and correctly EXCLUDES Glimmervoid and basic lands.
- The PER-PERMANENT battlefield line (`Your battlefield (creatures: N): ...`) tags only SOME
  artifacts. Verified example (vs131 seat 557deb94, seq15, my12/op13):
  `Your battlefield (creatures: 2): Darksteel Citadel; Signal Pest {1} (4/1) (printed 0/1);
   Mountain [tapped]; Mountain [tapped]; Cranial Plating {2} [artifact] [attached to: Signal Pest];
   Etched Champion {3} (2/2) [protection ...]`
  Here Cranial Plating shows `[artifact]` but Darksteel Citadel (an artifact land) and the artifact
  creatures (Signal Pest, Etched Champion) do NOT. Summary said `you 4` and was correct
  (Citadel + Signal Pest + Plating + Champion).
- Because the tags are incomplete, a pilot that re-derives gets it wrong in BOTH directions:
  UNDER-counts untagged artifact lands/creatures, and OVER-adds Glimmervoid + basics.

REPRO of the surviving fold-in (grep replies for `Glimmervoid` co-occurring with `artifact`, then
compare stated count to surfaced `you N`):
- vs44 (5643102c) seq7 T5, surfaced `you 2`: "Play Glimmervoid to increase my artifact count to 3,
  activating Metalcraft." Glimmervoid is a LAND — no count change. (Non-consequential: the seq7
  options were land-drops only; playing Glimmervoid is correct regardless.)
- vs109 (5637160a) seq28 T8, surfaced `you 4`: "I have 5 artifacts on board (Mox Opal, 2x Darksteel
  Citadel, Cranial Plating, Glimmervoid)" — folds Glimmervoid.
- vs140 (555beb04) seq59 T27 (`you 5`) and seq65 T29 (`you 8`): enumerates Mountains and Islands as
  artifacts ("8 artifacts: Darksteel Citadel, Mox Opal, Cranial Plating, Mountain, Glimmervoid,
  Island, Island...").

OUTCOME IMPACT: none proven this wave (as wave-8). No Galvanic Blast fired at a surf<3 board (all
Blast casts were surf3/4/6, genuine metalcraft ON, dealing a correct 4); the metalcraft boundary
(N crossing 3) was never DECIDED by a fold-in because the deck floods artifacts past 3 quickly. The
risk (Blast expecting 4 on a 2-artifact board; false Champion protection at N=2) stays LIVE.

LEVER (representation, not guide): make the per-permanent battlefield tags COMPLETE and consistent
with the summary — tag EVERY artifact `[artifact]` (Darksteel Citadel, Mox Opal, all artifact
creatures) and leave Glimmervoid + basic lands untagged. Then a re-derived count agrees with N in
both directions and the fold-in has no surface to feed on. Owner: representation/engine (likely the
battlefield-serialization in `AIPlayerGPT.cpp`, same area as the `[artifact]` tag emission). GUIDE
IS FROZEN — the wave-8 READ wording is retained verbatim; escalating it would hit the
representation-beats-instruction wall.

## M1 — MODEL / DECISION WATCH (n=1, low confidence): single-attacker "lethal" into an untapped
## blocker (phantom-lethal cousin).
vs109 (5637160a) seq25 T6, my13/op5: pilot declared ONLY Memnite (7/1, base 1 + 6 artifacts) and
asserted "opponent is at 5, deals 7, lethal, game ends." The opponent's Stromkirk Noble was
available to block and chumped it — opp stayed at 5 into T7 (seq27). The Plating math was CORRECT
(no over-count); the miss was asserting a game-ending attack without pricing the untapped blocker.
Single instance, in a race deck110 was already losing on material (only 2 bodies, Blast spent), so
it changed little. Route to the model-experiments / phantom-lethal watch; NOT guide-fixable at n=1.
Freeze the RACE clauses.

## DC1 — DECK-CONSTRUCTION / META FLAG (standing, reaffirmed — owns the loss column).
deck110's non-wins this wave are the standing axes, no new mechanism:
1. **vs109 race-loss (Hellrider aggro).** Got the opponent to 5 by T4 then stalled for bodies while
   a fast red clock raced it 15->-3. The RACE the guide already teaches ("go WIDER"); the shortfall
   was material (few creatures drawn), not obedience.
2. **vs140 timeout, deck110 BEHIND (~1 vs 18).** The known deck140 Staff-of-Nin/Lightmine grind
   ground deck110 out over 29 turns. Triage latency, not a play failure.
Not a guide task. If the decklist is ever tuned: trim ~0-power filler (Signal Pest 0/1, Ornithopter
0/2) and/or the Mox/land count toward more mid bodies + reach/evasion-block redundancy, to cut
flood + single-threat-counter + evasion + grind exposure at once.

## Positive validations (no action, continuity)
- Harness CLEAN: 0 desyncs, 0 defer, 0 unparsed across 200 decisions (wave-8 had 1 defer + 1
  degenerate reply; both absent this wave).
- Land-drop enumeration FIRED (multi-land co-offers render). No color-priority land stopgap in this
  guide to drop.
- Blocker seam: 4 windows, all correct (chump to deny a trigger; free favorable block). Attack-seam
  survival clause continues to behave; no reflexive high-life block.
- Core `hand -> stack` cast-happened anchor REMOVAL: 0 engine no-ops this seat — corroborates
  no-regression.
