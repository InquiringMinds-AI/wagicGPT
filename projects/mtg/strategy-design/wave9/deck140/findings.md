# Deck-140 review — wave 9 (FINAL wave before pause)

Deck: **"Wipe Them Out!"** — RBW control, 14 board wipes, ~2 creatures (Blightsteel Colossus x2),
finishers = Rakdos's Return / Staff of Nin / Blightsteel Colossus. Sweep every creature board, deploy
a finisher, grind the face to 0 on reach.

Corpus: PRIMARY `matchups-20260714-094942`, 6 deck140 seat games. Guide in play = the wave-8 guide
(countable SWEEP TRIGGER, three false-belief kills incl. full-strength tapped line, BSZ-X guardrail).

**Record: 0 W / 3 L / 3 timeouts.** NOT the verdict (user doctrine). Decision-level story: **the
count-anchored SWEEP TRIGGER is validated a SECOND corpus (freeze); the loss column is pure
reach-starvation (deck construction); all THREE timeouts are LATENCY-STARVATION with deck140 AHEAD or
even.** Only new play-signal = one tapped-bridge resurrection off a "16 or less" boundary misread, plus
self-correcting reply-protocol intent-collapses.

| game | opp | outcome | deck140 state | class |
|---|---|---|---|---|
| vs109 | mono-R aggro | LOSS t15 | -1/20 | reach starvation |
| vs44  | UB faeries   | LOSS t14 | -5/20 | snowball evasive + reach |
| vs133 | mono-B blitz | LOSS t15 | -1/24 | reach starvation |
| vs135 | snow control | TIMEOUT  | **AHEAD 36-13** (t23,63dec) | latency starvation (winning) |
| vs131 | UWx control  | TIMEOUT  | close 25-29 (t58,103dec)    | latency starvation (no closer) |
| vs110 | affinity     | TIMEOUT  | **AHEAD 18-3** (t29,62dec)  | latency starvation (winning) |

Tags: [GUIDE] model-fixable · [STRUCTURAL] deck/infra · [REPLY-PROTOCOL] harness layer.

## Harness / executability sweep (FIRST)
- Reply-head!=choice desyncs: 0. Empty replies: 1 — vs110 t10 seq14, empty reply on a LAND drop
  (`fallback: empty_reply`, choice -1) inside vs110's 120,003 ms latency spike. Model wall-clock
  timeout, not a decision failure. Only fallback in the corpus. Negligible.
- Latency = heavy `-j` saturation and is the whole timeout story (Finding 5). Median ~10.4 s, p90 ~29 s,
  max 120 s; 20 decisions >30 s.
- X-menu offset: clean (biggest X picked every time: X=1 vs109, X=2 vs44, X=5 vs110). Dead 5 waves.

## TOP FINDINGS

### 1. [VALIDATED — freeze] Count-anchored SWEEP TRIGGER holds on a 2nd corpus
Fire direction (over-fire into C=0): CLEAN as a trigger failure. Two sweepers cast into `(creatures:0)`
but NEITHER is the count-trigger misfiring:
- vs131 t40 seq61 Wrath into C=0 — PLAN opens *"opponent has 0 creatures, so casting a board wipe is
  unnecessary and wastes mana"* and wanted Blightsteel; head `2` cast Wrath. Reply-protocol
  intent-collapse (Finding 6), not the trigger.
- vs110 t22 seq39 Pyroclasm into C=0 — a SECOND Pyroclasm the same turn after seq38 cleared the Signal
  Pest; stale PLAN still names the dead Pest. Within-turn re-offer waste at C=0, cost one card, deck140
  AHEAD 18-3.
(Lightmine casts into C=0 = the punisher being deployed pre-emptively, per guide — not misfires.)
The PLAN reads C off `(creatures:N)` correctly every time (e.g. vs110 t10 *"opponent has 0 creatures,
so Pyroclasm/Damnation is wasteful"*; swept the affinity board when C>=1). The count is being read.
FREEZE. Hold direction: exactly ONE miss (Finding 3).

### 2. [FREEZE] BSZ-X guardrail no violations; belief-kills mostly clean
- BSZ-X guardrail (wave-8): NOT violated. vs109 X=1 into a 1/1 (X>=T), vs110 X=5 into T-1, vs44 X=2
  killed the 2/2 and shrank the 4/4 (no Wrath/Damnation offered = best available). The wave-8
  X=1-into-2/2 miss did not recur. Freeze.
- Artifact belief-kill: NO resurrection (pilot read affinity boards correctly). Freeze.
- Shroud belief-kill: NO DATA — zero shroud creatures this corpus; line neither exercised nor bypassed.
  Untested-but-kept. The "shroud core candidate at 2nd seat" cannot advance without a shroud board;
  keep the guide line, no core promotion.

### 3. [GUIDE — one minimal, safe diff] Tapped-bridge resurrection off a "16 or less" boundary misread
Single held-castable-sweeper decision: **vs135 t13 seq28**. C=1 (Ohran Viper 1/3), my_life **16**, Cruel
Edict {1}{b} offered and trivially payable ({b}{b}{b}{b}{w}{w}{w}). Representation FULLY correct — board
printed `Ohran Viper (1/3) [tapped - untaps and can attack next turn]` AND the log showed *"Turn 13 ...
Damage: 1 dealt by Ohran Viper to you"* (attacked LAST turn). BOTH trigger conditions satisfied
(16<=16; attacked last turn). Pilot held (cast Venser's Journal):
> *"...which is tapped; since it is not attacking THIS turn and I am at 16 life (ABOVE the 16 threshold
> for mandatory sweeping unless attacked), I do not need to sweep yet."*

Two failures, in priority order:
1. PRIMARY — boundary misread. Computed life 16 as *"above the 16 threshold,"* reading "16 or less" as
   strict less-than and excluding itself. A wording ambiguity the weak executor mis-resolved (NOT a
   read-then-disobey wall) -> disambiguation is the correct rung.
2. SECONDARY — invented hold-phrase. Substituted the guide's "attacked you LAST turn" with a
   self-generated *"not attacking THIS turn,"* using the current tapped state as cover. The tag says
   "attacks NEXT turn"; it attacked last turn. Tapped bridge resurrecting in a new guise, despite the
   full-strength "Tapped is NEVER a reason to wait" line still present.

Cost low (lone 1/3 in a game deck140 WON-timeout 36-13; swept the Viper at t15). But a clean
decision-level failure with a precise, in-doctrine wording cause. THE ONE GUIDE DIFF THIS WAVE:
(a) make the life boundary unambiguous ("16 or LOWER - 16 itself counts") in the SWEEP TRIGGER and
DECIDING SITUATIONS recap; (b) name the invented anti-pattern on the tapped bullet ("'it isn't attacking
THIS turn' is not a reason to hold"). No threshold change, no new fire condition, no widening. Skill
method: name the anti-pattern the executor will otherwise invent (deck135 P1, wave-8).

### 4. [STRUCTURAL — whole loss column] Reach starvation + no closer (unchanged)
All three losses = same deck-construction fact: deck140 stabilizes correctly and cannot close before it
dies. vs109/vs44/vs133 swept faithfully, out-raced; no early lifegain/blocker, 1-dmg/turn clock. vs131's
grind shows the other face: the only castable closer, Blightsteel {12}, is one counterspell away and
Rakdos X=13 fell short of opp-29. Teeth, not text (notes.md #4). Do NOT spend guide length on reach.

### 5. [INFRA / STRUCTURAL] Timeout triage x3 — ALL latency-starvation, deck140 winning/even
Decisive number = decision-density x latency:

| game | decisions (ask+priority) | cumulative latency | state at cap |
|---|---|---|---|
| vs135 TIMEOUT | 63  | **1259.6 s (~21 min)** | AHEAD 36-13 |
| vs131 TIMEOUT | 103 | **1356.5 s (~23 min)** | close 25-29 |
| vs110 TIMEOUT | 62  | **1297.0 s (~22 min)** | AHEAD 18-3 |
| vs44 LOSS     | 23  | 239.0 s | died t14 |
| vs109 LOSS    | 18  | 280.1 s | died t15 |
| vs133 LOSS    | 37  | 426.6 s | died t15 |

The timeouts are the LONG games (60-103 decisions at ~10 s median + 30-120 s spikes) — they blow the
wall-clock cap while deck140 plays correctly. vs131: ~50 correct "Cast nothing" passes t2-t58 (control
mirror, no opponent creatures, deck140 ramping toward a 12-mana closer). NONE is a play failure. Fix is
infra: cap-by-decision-count / adjudicate-by-life-at-cap, or `-j 3` / higher `WAGIC_GPT_TIMEOUT`.

### 6. [REPLY-PROTOCOL — route to the reply-protocol layer, NOT the guide] Intent-collapse, 2nd seat
deck140 is now a SECOND witnessing seat for the wave-8 answer-before-reasoning class (deck135 first).
2-3 clear instances, ALL self-corrected on the next priority window (deck140 gets many windows):
- vs109 t10 seq10: head `3`=Cast nothing at 1 life; PLAN reasons to *"I MUST cast Black Sun's Zenith
  X=1"* to survive. Self-corrected seq11 (cast BSZ X=1, killed the Loyalist).
- vs131 t40 seq61: head `2`=Cast Wrath; PLAN *"I will not cast Wrath of God as there are no creatures."*
- deck135 t19 seq53: head=Cast nothing; PLAN *"So I will cast Lightmine Field."* Self-corrected seq54.
Head committed before the PLAN reasons; PLAN reaches a different same-window action; head never revised.
Do NOT re-word the guide over it, do NOT score against a guide rule. Durable fix = reply-protocol A/B
(accept LAST bare number / move CHOICE after PLAN), judged by decision-quality. This corpus STRENGTHENS
the A/B case (2nd seat).

## OVERSHOOT / DISPLACEMENT AUDIT (mandatory)
1. SWEEP TRIGGER + belief-kills: VALIDATED 2nd corpus. Fire clean; hold 1 miss (wording-cause, minimal
   diff). No re-widening.
2. Displacement: residual did NOT move to a new play seam — it stayed at (a) the CLOSER (slow/counterable
   = deck construction) and (b) the REPLY PROTOCOL (intent-collapse = harness). Neither is a displaced
   guide leak.
3. Rakdos hold (#1): FROZEN, obeyed every non-lethal window incl. vs131 X=13 at opp-29 across 50 turns.
4. BSZ-X / X-menu / Staff face-ping / Lightmine / Mulligan / Lands: all obeyed. Freeze.
5. No attack-floor issue (deck runs ~no creatures).

## WATCH (do NOT legislate — single seat / re-widening risk)
- Snowball single evasive threat (carried from wave-8): no new instance. Hold the WATCH.
- Within-turn double-sweep at C=0 (new, single seat): vs110 t22 second Pyroclasm into the board its
  first just cleared (stale plan naming the dead Pest). One instance, C=0, one card, winning. WATCH.

## Wave-8 findings status
- Count-anchored SWEEP TRIGGER — VALIDATED 2nd corpus. Freeze (+ boundary disambiguation only).
- BSZ-X guardrail — no violation; freeze.
- Tapped/artifact demotion — NOTE: the LIVE guide still carries the FULL-strength tapped line (the
  wave-8 notes described a demotion the shipped guide did not apply). Tapped bridge resurrected once
  anyway (Finding 3) = evidence AGAINST further demoting it; keep full strength.
- Shroud line — untested this corpus; keep, no core promotion.
- Reach starvation — persists, whole loss column. Deck construction.
