# deck131 wave-26 -- dev notes (engine / harness / model / deck-construction; self-contained).

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260725-115216/`, 6 deck131 games, **2W/4L (2/6)**.
Binary 77e91ef4e (step-1 batch 79e39511f + 77e91ef4e, -T 3000). Pilot: rep_penalty=1.05. Live guide =
wave-25 REVISED (12161 bytes, deployed==wave25 byte-verified via cmp exit 0). This wave: ONE guide edit
(favorable-block exception on the never-block absolute) + a one-line lethal-attack reconciliation clause
-> 13264 bytes. deck131 is deck0 vs 133/22/137/18, deck1 vs 102/27 (see results.tsv). Zero fallbacks.

Seat->game map (results.tsv): vs102 WIN 19/0 T15 | vs22 WIN 20/0 T12 | vs137 adj LOSS 1/8 T12 |
vs18 LOSS 0/15 T19 | vs133 LOSS -7/13 T25 | vs27 LOSS -6/14 T16.

Layer-routing: engine/harness/model items live HERE, never in the guide. Deck-construction flags are
RESTATED for the user, not relitigated.

## Reproducible engine/harness/model ledger

### I-1 -- [MODEL-COMPREHENSION, WATCH, guide-unfixable] tapped-read misread: NO RECURRENCE round 4
- ROUND 4 of the Guttersnipe tapped-read guard. Every Guttersnipe attack window this corpus was correct or
  correctly-conservative; the wave-25 "attackers-tapped => all-tapped" false generalization did NOT recur.
  Cleanest signal: vs27 held Guttersnipe home at ALL FIVE windows facing an untapped Zombie Outlander (2/2)
  (s14/s20/s27/s29 ATTACK: none; s9 held Gut, swung YP). The one Guttersnipe swing into a blocker (vs22 s17,
  into an untapped Stinkdrinker 1/3) was a CORRECT lethal alpha strike (opp at 3; enumerated all 4 scenarios
  as lethal), NOT a misread.
- **Guide UNTOUCHED on the per-creature tapped-read guard prose** (sanctioned-against to shout louder at an
  already-explicit line that held). The WATCH stays SINGLE-SEAT (deck131 only, 3 prior corpora; 0 this
  corpus) -- promote to a cross-seat combat-safety item ONLY if a 2nd SEAT shows the misread. Repro: for
  kind=='attackers' with Guttersnipe among options, diff the reply's claimed tapped-set vs the prompt's
  opponent-battlefield [tapped] tags; flag a reply that claims "all/no untapped" while a creature lacks
  [tapped] AND the swing is not lethal-justified. See I-5 for the still-below-bar core mitigation candidate.

### I-2 -- [GUIDE-DEFECT via CORRECT DEVIATION, FIXED] the never-block absolute was STILL too tight (favorable-block)
- vs102 s15 (blockers, T8, life 19): Guttersnipe (2/2) blocks Elvish Doomsayer (**1/1**) -- "favorable
  trade: the Doomsayer dies, my Guttersnipe survives with 1 damage marked... shroud, so combat is the only
  way to remove it." vs102 s20 (blockers, T10, life 19): Doomsayer now **0/1** (Animate Dead -1/-0),
  Guttersnipe blocks again -- "guarantees destruction while Guttersnipe survives (taking 0)... overwhelmingly
  favorable." BOTH CORRECT (Guttersnipe's 2 toughness > attacker power => survives, kills the attacker, stays
  untapped to attack -- blocking does not tap). BOTH were PART of the vs102 WIN.
- BOTH VIOLATE the wave-25 guide's "NEVER block with Guttersnipe... it stays alive in EVERY case." The
  wave-25 edit covered only the LETHAL-and-only-blocker CHUMP case; it did NOT cover a FAVORABLE block where
  Guttersnipe LIVES. -> guide FIXED this wave: allow Guttersnipe to block an attacker with power < 2 (0 or 1)
  -- it survives, is free removal, not a trade. Preserved the never-TRADE core (power 2+ and not-lethal = do
  not block; verified by vs27 s16/s23/s28 CORRECT holds) and the lethal-only-blocker exception. Rules-
  correctness fix, NOT louder (the covered behavior was WRONG, not under-emphasized). Repro: for
  kind=='blockers' where the model blocks with Guttersnipe, it is CORRECT if EITHER (attacker power < 2 =>
  Guttersnipe survives) OR (incoming power >= my_life AND Guttersnipe is the only creature).

### I-3 -- [RULES-CORRECTNESS RECONCILIATION, guide clause added] lethal alpha strike vs the Guttersnipe-hold rule
- vs22 s17 (attackers, T12, opp 3): model swung Guttersnipe into an untapped Stinkdrinker Daredevil (1/3),
  which the literal Guttersnipe-attack rule ("ONLY when opponent has ZERO creatures that can block") forbids
  -- but it was LETHAL (YP 2/1 + Gut 2/2 + Elem 1/1 = 5 power vs one 1/3 blocker; the model enumerated all 4
  block scenarios as opp->0). The model resolved the tension CORRECTLY on its own (via the separate LETHAL
  CHECK section). -> added a one-line clause to the Guttersnipe-attack paragraph reconciling the two guide
  sections (attack Guttersnipe into a blocker IS right when the swing is lethal). This reconciles two
  contradicting guide lines; it is NOT shout-louder (the two sections literally disagreed).

### I-4 -- [MODEL SELF-COHERENCE on answer-first formats, WATCH, NOT a parser misfire] first-line vs concluded answer
- TWO instances this corpus where the model emitted its CODED / answer-first line, then reasoned to a
  DIFFERENT conclusion it never re-emitted in coded form:
  (a) vs137 s6 (bottom): first line `PUT: 1, 2, 4, 6`; prose conclusion (twice) "the indices are 2, 3, 4, 6"
      / "Bottom cards 2, 3, 4, and 6". Parser recorded the only PUT:-format line (1,2,4,6).
  (b) vs102 s14 (attackers): first line `ATTACK: none`; prose then concludes "Guttersnipe is safe to attack.
      Why did I initially think 'none'?" Parser took the answer-first line (`none`).
- **NEITHER is a c4 hardening misfire.** The model's alternate conclusion was NEVER re-emitted in coded
  format, so it is unparseable by construction -- the parser correctly took the only coded/answer-first line
  per the precedence spec (ATTACK answer-first; PUT: is answer-first by prompt design). The defect, if any,
  is MODEL self-coherence: it commits the coded answer before finishing deliberation, so for answer-first
  formats the pre-reasoning answer wins. BOTH non-decisive (vs137 both keeps defensible + game lost on the
  mull-to-3; vs102 the conservative hold cost 2 face but the game was won 19/0).
- This is guide-UNFIXABLE and below-bar for a core-prompt change (single-seat, non-decisive, and the
  answer-first design is deliberate -- attack replies ramble AFTER the answer, which is WHY first-line wins).
  Routed to general-suggestions as a below-threshold WATCH for the synthesis agent. Promote only on cross-
  seat convergence where the first-line/conclusion divergence flips a decisive decision. Repro: for
  kind in {attackers, bottom}, compare the first coded line to any later "the answer is / bottom cards X"
  restatement in prose; a divergence that the parser cannot see (no re-emitted coded line) is this class.

### I-5 -- [CORE-PROMPT REPRESENTATION, STILL BELOW-BAR] affirmative untapped-blocker tagging
- Unchanged from wave-25 I-6. The tapped-read misread class (I-1) had NO recurrence this corpus, so the
  evidence for an affirmative "[untapped - can block]" tag on opponent creatures did NOT strengthen. Still
  single-SEAT, and the current representation is correct-not-defective. Routed to general-suggestions as a
  below-threshold candidate; promote to a real core-prompt proposal ONLY on cross-SEAT convergence.

## Interface validations (clean this corpus)
- **Mulligan all correct/defensible** -- 4 KEEP with a Mountain + YP/Guttersnipe (vs133/vs22/vs18/vs27); the
  STEP-1 no-red mull fired at vs102 (once) and vs137 (FOUR straight -- brutal variance, all correctly no-red
  or dig-uncastable-on-curve hands; the vs137 seq3 hand had 2 digs but only 2 Islands, so the {2}{u} dig was
  NOT cleanly castable "this turn or next" -> mull is defensible under the STEP-1 exception's mana clause).
  Both post-mull bottoms exercised the recalibrated ladder correctly (I-2 rank-1 Elixir-first; the mull-to-3
  ladder-walk). Strongest mulligan-DISCIPLINE corpus; worst mulligan-LUCK corpus (vs137 mull-to-3).
- **Guttersnipe attack discipline clean** -- held every untapped-blocker board (vs27 x5, vs102 s14 even when
  a swing was safe = conservative), swung every empty/tapped board, one correct lethal swing into a blocker
  (vs22 s17). No dangerous misread (I-1).
- **Guttersnipe BLOCK discipline correct + expanded** -- 2 favorable survive-blocks (vs102 s15/s20, CORRECT,
  now guide-endorsed I-2), 1 lethal-only-blocker chump (vs27 s31, CORRECT, wave-25 edit validated), and
  correct HOLDS (BLOCKS: none) vs a non-lethal 2/2 (vs27 s16/s23/s28). No Guttersnipe traded away when not
  forced.
- **Attacker/blocker declarations pure-index/name, all parsed** (no name-leak fallback).
- **Zero fallbacks** (no empty_reply/retracted_choice/stale_echo/unparsed) -- cleanest deck131 corpus, 5th
  running. c4 corpus gate PASSED (no dropped/mis-picked legitimate reply; the two self-coherence wobbles are
  model-side, not parser, I-4).

## DECK-CONSTRUCTION ledger (RESTATED for the user; user owns; not relitigated)
The standing 9+-wave diagnosis holds; the 2/6 is draw variance over the same construction-capped deck:
- **Threat density 6/60** (3 Guttersnipe + 3 YP). vs18 drew ZERO Guttersnipe all game (single-YP token
  race, lost); vs137 mulled to 3 on four no-red hands. Variance the deck cannot absorb.
- **No reach vs lifegain/drain/go-wide** -- vs133 (drain, opp 13 at T25), vs27 (unblockable + go-wide, opp
  14), vs18 (anthem wide, opp 15). 131 could not push face past a wall.
- **No early defense vs aggro** -- vs18 (Kithkin) out-raced 131 by T19.
- **The two WINS do NOT raise the ceiling:** vs102 is a variance flip (1-life loss last wave -> win now, same
  Tergrid-durdle ceiling); vs22 is a free win vs an un-guided Step-0 deck. Against the GUIDED field
  (18/27/133/137) 131 went 0/4. Ceiling unchanged.
- Manabase 14 Island / 8 Mountain: mulligan gate held (no keep-and-screw loss); the vs137 4-mull is red-
  variance the gate correctly respected, not a manabase-config error.
- Sideboard candidates unchanged (Spellheart Chimera reach, Aetherling finisher) for the no-reach axis.
- These are the user's to decide; recorded, not argued.

## For a future deck131 reviewer -- gotchas
- n=6, single run: win-rate is noise; 2/6 here vs 1/6 wave-25 vs 0/6 wave-24 are all the SAME construction-
  terminal deck under draw variance. Do NOT read 0->1->2 as a trend. vs102 and vs137 both FLIPPED on variance
  (vs102 loss->win, vs137 win->loss).
- Positions vary per row: deck131 is deck0 in vs133/22/137/18 and deck1 in vs102/27 THIS corpus. Read
  my_life / results.tsv columns per record; do not assume a fixed column.
- **Guttersnipe BLOCK is not automatically a slip** (I-2, expanded): CORRECT if (attacker power < 2 =>
  survives, FAVORABLE) OR (incoming >= my_life AND Guttersnipe is the only creature => lethal chump). A power
  2+ trade while NOT lethal IS a slip.
- **Score Guttersnipe attack windows against ACTUAL [tapped] tags** (I-1) AND check lethality (I-3): a swing
  into a blocker is CORRECT if the attack is lethal.
- **The answer-first self-coherence wobble** (I-4): for kind in {attackers, bottom}, the parser takes the
  first coded line; a later prose "actually the answer is X" that is never re-emitted in coded form is a
  MODEL wobble, NOT a parser misfire -- do not report it as a c4 hardening defect.

## ROTATION VERDICT

**DO NOT ROTATE OUT this wave -- guide MODIFIED (Gate 1 fails). Seat is at guide-near-exhaustion; projected
rotation candidate at WAVE-27 under the same terms.**
- Gate 1 (guide-mod?): FAILED to be a candidate -- the guide WAS modified (favorable-block exception + a
  lethal-attack reconciliation clause). A revised guide is never a rotation candidate; it must re-validate.
- Gate 2 (new-work-signal?): thin-but-present. The favorable-block finding (I-2) is a genuine new guide-
  fixable decision class this corpus (a THIRD miscalibrated absolute, same CORRECT-DEVIATION shape). Both
  wave-25 edits VALIDATED (bottom rank-1 Elixir-first + ladder-walk; lethal-only-block fired correctly).

**UNSENTIMENTAL read for the roster owner:** deck131 is CONSTRUCTION-TERMINAL (owner's call, 9+ waves) and
the guide-fixable work is now VERY narrow and diminishing -- three successive waves have each produced ONE
edge-case recalibration of a too-absolute rule (wave-25: bottom-absolute + block-lethal-absolute; wave-26:
block-favorable-absolute), all non-decisive, all already PLAYED CORRECTLY by the model with the guide merely
lagging the play. After this wave the never-block rule is fully recalibrated (survives-block + lethal-chump +
never-trade all covered) and the bottom ladder is complete. The tapped-read misread is stochastic-unfixable
(0 recurrence this corpus). There is no new decision CLASS surfacing beyond "yet another too-absolute rule
softened," and the construction ceiling is fixed. **Projected: deck131 becomes a genuine ROTATION CANDIDATE
at wave-27** if (a) the favorable-block exception validates (or simply is not contradicted), and (b) no new
guide-addressable decision class appears -- which, with the block and bottom rules now fully recalibrated, is
the likely outcome. It is not eligible THIS wave only because the guide changed. The construction lever
(threat density / reach / early interaction) remains the owner's, off the loop's guide track.
