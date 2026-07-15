# Deck-110 wave-11 — development notes (self-contained; layer-routed engine/harness ledger)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-072254/`, 6 deck110 seat translogs
(`*ai_baka_deck110-*.jsonl`). Binary `/tmp/wagic-54a5a9d01` (wave-10 batch + 4096 cap + ramble guards
+ stack-target annotation fix + fetch pre-targeting keying), wave-10 guides deployed, answer-after-plan
reply protocol, max_tokens 4096. Card facts vs `projects/mtg/bin/Res/sets/primitives/mtg.txt`. Choices
1-indexed (`CHOICE: 1` = option 1). Each item carries a full repro; engine/harness items belong HERE,
never in prompt/guide text.

## E1 — CLOSED (5th running wave): the re-equip "(ALREADY attached... change NOTHING)" marker.
Wave-6 inert -> wave-7 fired+obeyed -> wave-8/9/10 stable -> wave-11 stable.
REPRO: per seat log, count `options_text` containing `ALREADY attached` (marked offers) vs
`chosen_text` containing `ALREADY attached` (taken). Wave-11: **90 marked offers, 0 taken; 20 Equip
picks, 0 marked no-ops.** Marker renders exactly as `(ALREADY attached to it - this would change
NOTHING)`; the two-power cue renders e.g. `Signal Pest (4/1) (printed 0/1)`. Field fix at
`src/AIPlayerGPT.cpp:1163-1173`. No engine or guide work. Guide Rule #1 stays frozen verbatim.

## E2 — ENGINE/REPRESENTATION (unfixed, ledger #4): the equip re-offer's no-op marker is keyed on
## TARGET-IDENTITY and is blind to a low/negative-value move between MULTIPLE Platings or EQUIVALENT
## carriers. RECURRED this wave in the CONSOLIDATION shape (milder than wave-10's net-zero bounce).

Status: wave-10 F1 confirmed it costly on a pure net-zero A->B->A->B bounce between two identical
Ornithopters (3 wasted `{1}`, a loss). Wave-11 it recurred milder — no net-zero identical-carrier
bounce; one costly two-Plating CONSOLIDATION with confused reasoning, plus one minor same-turn re-pick.

REPRO 1 (COSTLY, in a LOSS) — deck109 seat, T8, my4/op10. Two Cranial Platings; one on Ornithopter
(rendered 6/3), one available. Single Main Phase 1:
- seq25: options include [Equip Plating -> Ornithopter {1}], [Equip Plating -> Master {1}],
  [attach {b}{b} -> Master], [attach {b}{b} -> Ornithopter], and two `(ALREADY attached...)` Ornithopter
  options. Pilot chose `CHOICE: 2` = Equip -> Master of Etherium. Plan states it is "attaching the
  SECOND Plating to Master ... then attack with Ornithopter (6/3)".
- seq26: state now shows a Plating on Master AND a Plating on Ornithopter. Options include
  `[... Master (ALREADY attached...), Equip -> Master {1} ...]`. Pilot chose `CHOICE: 4` = Equip ->
  Master again (the SECOND Plating, off the Ornithopter). Plan again asserts "attack with my 6/3
  Ornithopter (which already has a Plating)".
Net effect of seq25+seq26: BOTH Platings consolidated onto Master (a defensive blocker at 4 life); the
Ornithopter is stripped to base and its flyer clock is gone — while the reasoning at both windows
believed the Ornithopter kept a Plating. Cost = 2 wasted `{1}` equips + the lost attacker at 4 life in a
race it lost. The `(ALREADY attached...)` marker fires only on the instance currently holding a Plating,
so "move the second Plating onto a creature that already has one" is UNmarked and reads as a real gain.

REPRO 2 (MINOR, in a WIN) — deck135 seat, T5, my20/op17: seq11 equip Plating -> Memnite; seq13 (Memnite
marked ALREADY) equip Plating -> Ornithopter (unmarked, correctly taken). One `{1}` wasted equipping
Memnite first; the move to the evasive Ornithopter is a legitimate ground->flyer upgrade. Marginal.

NON-CHURN (correct) for contrast — deck140 seq32/33 (two Platings both -> Ornithopter) and seq46/47
(two Platings both -> Etched Champion): the pilot equipped the first, then equipped the SECOND unattached
Plating to the same best carrier, explicitly declining the ALREADY option ("I will not waste mana on the
already-attached option"). Rule #1's sanctioned two-Plating stack. Correct, marker obeyed.

ROOT CAUSE (unchanged): the no-op predicate is "already attached to THIS target," which is not the
no/low-value predicate. Two shapes evade it — two Platings (wave-9 E2 / this wave's consolidation) and
two equivalent carriers (wave-10 F1). ENGINE LEVER (representation): compute the equip re-offer's
no-op/low-value marker from the OUTCOME DELTA — mark/suppress "Equip ... targeting X" whenever moving the
equipment to X would NOT increase X's resulting power above the current carrier's (equivalent-or-worse),
and specifically suppress moving a SECOND Plating onto a creature that already carries one when a
higher-value carrier or a naked body exists. One predicate catches both shapes. Owner:
`AIPlayerGPT.cpp` equip-option serialization (same area as the `[artifact]` tag + marker emission).
Priority MEDIUM (recurrent, costly in one loss). NO guide text — Rule #1 already forbids the behavior;
the pilot has no surfaced gain signal to obey it.

## E3 — ENGINE/REPRESENTATION: artifact-tag completeness SHIPPED, count AGREES 211/211; the RESIDUAL
## reasoning fold-in persists (representation wall); the hand-card type sub-gap PERSISTS (no new instance).

SHIPPED & VERIFIED: every artifact on the current "Your battlefield" line carries `[artifact]` —
artifact CREATURES + the artifact LAND Darksteel Citadel + Mox Opal + Cranial Plating — while
Glimmervoid + basics are UNtagged. Re-derived count (count of `[artifact]` on the current battlefield
line) AGREES with `Artifacts in play: you N` in **211/211 windows, zero offset** (wave-10: 177/177).
Representation layer CLOSED.

RESIDUAL (representation-beats-instruction WALL, 4th wave, NON-outcome-affecting): the pilot still
re-derives by hand and folds non-artifacts in — 21 Glimmervoid + 2 basic genuine over-counts. Reps:
deck133 s6 "two glimmervoids in hand, which are artifacts"; deck44 s16 "4 artifacts (darksteel citadel,
glimmervoid, cranial plating, signal pest)"; deck109 s15 "play the land (island) to increase artifact
count to 4"; deck140 s10 "play island... artifact count to 3". Self-corrections still occur (small
positive). OUTCOME IMPACT: NONE — all 7 Galvanic Blast casts fired at surf>=3 (genuine metalcraft);
none at sub-3 expecting metalcraft-4; no metalcraft-gated pick flipped. The deck floods past 3 too fast
for the boundary to decide. GUIDE FROZEN — escalating the READ wording hits the wall.

HAND-CARD TYPE SUB-GAP (candidate micro-lever, unchanged): hand serialization tags NONCREATURE artifacts
(`Cranial Plating {2} [artifact]`, `Galvanic Blast {r} [instant]`) but artifact CREATURES in hand show
only P/T (`Etched Champion {3} (2/2)`, `Vault Skirge {1} (1/1)`, `Memnite (1/1)`). The wave-10 under-count
instance (deck133 seq17/18 "Etched Champion is not an artifact") did NOT recur clearly this wave (0 clean
hits). CANDIDATE: tag artifact cards in the HAND serialization too (e.g. `Cast Etched Champion {3}
[artifact creature]`) so forward projections ("cast X to reach N") use the right type. Low priority —
non-outcome-affecting. Owner: `AIPlayerGPT.cpp` hand serialization. NO guide text.

## M1 — MODEL / REPLY-PROTOCOL WATCHES.
(a) **Answer-last trailing PLAN/CHOICE mismatch — casts the offered SPELL when the plan concluded HOLD
(2nd seat, across waves).** deck140 seat, seq6 T1, options `[Cast Galvanic Blast (metalcraft 4 dmg),
Cast nothing]`: the PLAN concludes in prose "I will not cast Galvanic Blast now because I need to attack
first ... holding the instant allows me to respond," then emits `CHOICE: 1` = Cast Galvanic Blast. The
reasoning precedes and is CORRECT; only the trailing index is wrong, failing ASYMMETRICALLY toward
casting over holding. Identical shape to wave-10 deck140's own 4/4 class (2 wasted Rakdos kill shots
etc.) — now witnessed at the deck110 seat. Reinforces the reply-protocol A/B: reconcile the trailing
index against the plan's NAMED action, or require echoing the option TEXT rather than a bare integer
(that A/B would have taken "Cast nothing" here). NON-outcome-affecting (T1 face-4 in a grind loss;
Blast-to-face defensible). Route to the code-appended reply-protocol layer; NOT guide-fixable. n=1 this
seat.
(b) **Unlisted-card fixation (wave-10 F2): did NOT recur as a failure.** deck131 seq8/seq11 T3, options
`[Cast Thoughtcast, Cast nothing]`: plan concludes "I will cast one of the Cranial Platings now"
(Plating unlisted) and correctly emits `CHOICE: 2` = Cast nothing (did NOT fire the unwanted Thoughtcast
— the exact window wave-10 deck133 seq6 got wrong, now played right). Positive; watch closed to green
this corpus, but keep it open as a WATCH (the s6 trailing-mismatch above shows the emit-step reconcile
is still imperfect).
(c) **Blast-as-filler burn (wave-10 watch): did NOT recur costly.** deck133 cast only ONE Galvanic Blast
this game (seq20 T8, surf4, at opp face — legitimate finisher chip), not the wave-10 both-Blasts-by-T4
value-negative burn. No new instance; keep as a dormant watch (promote only in a CONTESTED game).

## DC1 — DECK-CONSTRUCTION / META (standing, reaffirmed — owns the loss column).
deck110's 3 losses share the precondition the guide cannot create: **few creatures / ground-out into a
flood deck.** vs133 (Asphodel midrange) LOSS my-2: ground out. vs109 (Hellrider aggro) LOSS my0:
raced down (+ the E2 consolidation burned T8). vs140 (Staff-of-Nin grind) LOSS-adj my14/op18: behind by
1 class at the cap. vs135 flipped LOSS->WIN vs wave-10 on identical guides = pure draw variance (it went
wide this time). Not a guide task. If the decklist is ever tuned: trim ~0-power filler (Signal Pest 0/1,
Ornithopter 0/2) and/or Mox/land count toward more mid bodies + reach/evasion redundancy, to cut flood +
single-threat + grind exposure at once.

## Positive validations (no action, continuity)
- Answer-after-plan killed intent-collapse at this seat: 0 true head-vs-choice mismatches across 223
  ask/priority; `CHOICE:` label present 216/223 (7 gaps = empty_reply timeouts, not 2048-cap truncations
  — 4096 shipped; the residual bite is the 120s HTTP timeout, WAGIC_GPT_TIMEOUT 240s shipped next).
- Re-derived artifact count agrees with summary N 211/211 (up from 177/177).
- Re-equip marker 90 offered / 0 taken (Rule #1 core class OBSERVED OBEYED again).
- Blocker seam clean: deck110 aggressor in all 6; the 3 Blockers-phase windows are own-side priority
  passes, no defensive block, no reflexive high-life block, no missed must-block.
- All 7 empty_reply fallbacks fell in decided/won positions; NONE changed a game.
