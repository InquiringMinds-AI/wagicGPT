# Deck-140 review — wave 8 findings

Deck: **"Wipe Them Out!"** — RBW control, 14 board wipes, ~2 creatures (Blightsteel Colossus x2),
finishers = Rakdos's Return / Staff of Nin / Blightsteel Colossus. Plan: sweep every creature board,
deploy a finisher, grind the opponent's face to 0 on reach.

Corpus: PRIMARY control arm `matchups-20260714-044131`, 6 deck140 seat games. Guide in play = the
wave-7 re-anchored guide (countable SWEEP TRIGGER + three false-belief kills). Binary carries the
wave-7 engine ledger, incl. the shipped representation fix this review was built to test.

**Record: 0 W / 4 L / 2 timeouts** — the SAME raw column as wave-7. Per the user doctrine this is NOT
the verdict. The decision-level story is the opposite of the column: **the wave-7 fix landed, the
overshoot class is GONE, and every game got longer and closer.** The 4 losses + 2 timeouts are now
STRUCTURAL (reach starvation / no closer), not model-fixable holds.

| game | wave-7 | wave-8 | end state | class |
|---|---|---|---|---|
| vs110 (affinity) | L t11, my-8 | L t15, my-10/opp20 | opp | reach starvation (affinity clock) |
| vs44  (UB faeries)| L t12, my-9 | L t15, my0/opp20 | opp | snowball evasive threat + reach |
| vs109 (mono-R aggro)| L t15, my0 | L t27, my0/opp6 | opp (barely) | reach starvation |
| vs133 (mono-B blitz)| L t11, my0 | L t17, my-3/opp18 | opp | reach starvation |
| vs131 (UWx control)| T (ahead 22-14) | T (behind 24-33, t50) | opp | no closer (Blightsteel countered) |
| vs135 (snow)| T (19/34 fallbacks) | T (AHEAD 21-16, t28) | deck140 | slow clock, winning |

Severity tags: [GUIDE] model-fixable · [STRUCTURAL] deck/infra.

---

## Harness / executability sweep (run FIRST)
- **Fallbacks / defers: ZERO across all 6 games** (`deferred_to_heuristic` = 0 in every deck140 file).
  Contrast wave-7 vs135's 19/34 fallback cascade — it did NOT recur.
- **Empty replies: 0. Broken replies: 0. Reply-head!=choice desyncs: 0.** Harness layer clean for
  deck140 this corpus; every leak below is play or deck construction.
- **Latency is still heavy `-j` saturation.** Median 7-10s; spikes to 68.4s (vs110), 59.6s (vs135),
  57.3s (vs109), 52s (vs131). Control mirrors make 90-104 decisions/game, so the two timeouts are
  decision-density x latency, not play (triage in Finding 5).
- **X-menu offset: clean 4/4.** Every X-announcement (vs109 X=1 & X=6, vs44 X=5, vs110 X=3) picked
  option 1 = the biggest X offered. The wave-4 index/value trap is dead four waves running.

---

## TOP FINDING

### 1. [VALIDATED — the wave-7 fix worked; the overshoot class DISAPPEARED] Held-sweeper-while-creatures-present = 0
Wave-7's collapse was 8 decisions holding a castable sweeper while the opponent showed creatures,
reached via three false-belief bridges (tapped / artifact / shroud). **This wave that class is ZERO.**
I ran the precise metric — every `Cast nothing` chosen while `(creatures: N>=1)` AND life<=16 — and
joined each to castability (P4 discipline). 7 candidates surfaced; **none is an overshoot**:
- vs44 seq22/24 (t9, C=2, L15): mana was `{b}{w}{w}{w}` — Pyroclasm needs red (none), BSZ/Damnation
  need `{b}{b}` (one black). No castable sweeper. Pilot names the mana lock and sweeps with Damnation
  the next turn (t11). Not overshoot.
- vs109 seq20/32/36 (t12/18/20, C=1): each is a FIRST priority pass in a turn where the pilot then
  cast a sweeper/Edict/Staff same turn (seq21 Staff, seq33 Cruel Edict, seq37 Cruel Edict). The single
  creature (Legion Loyalist 1/1) was removed. Not held.
- vs110 seq36/38 (t14, C=1, L2): opponent is a 15/5 Master of Etherium; the only sweeper in hand is
  Pyroclasm (2 dmg, can't kill toughness 5) which would deal 2 to itself and kill the pilot at L2.
  Dead position, no answer. Not overshoot.

**The bridges are provably dead — the pilot now sweeps INTO exactly the boards it froze on last wave:**
- **Tapped:** vs110 t6 cast Pyroclasm on a *tapped* Ornithopter (4/2, tag `[tapped - untaps and can
  attack next turn]`); vs109 swept through boards full of tapped-tagged attackers.
- **Artifact:** vs110 t8 Lightmine Field then t10 Black Sun's Zenith X=3 into a Mox/Glimmervoid/
  Cranial-Plating affinity board WITH creatures — the exact "artifact board, sweeper useless" freeze
  of wave-7, now swept.
- **Shroud:** vs44 t11 Damnation and t13 BSZ X=5 cast into a `[flying, shroud]` Faerie Bladecrafter —
  the exact card the pilot named-then-declined in wave-7. Damnation `bury all(creature)` ignores
  shroud; the pilot cast it. Dead bridge.

Attribution: the shipped representation (`(creatures: N)` count + `[tapped - untaps and can attack
next turn]` tag, 86 tag-firings this corpus) carries the tapped and artifact corrections; the guide's
shroud line carries the shroud correction (representation does not address targeting). Fix landed,
class gone. This is the wave-8 guide DEMOTION trigger (Finding 2).

### 2. [GUIDE — demote the now-double-covered false-belief lines] Representation carries tapped + artifact
Per the confirmed-fired-representation demotion rule: the tapped-bridge and artifact-bridge kill
paragraphs are now DOUBLE COVERAGE (the `(creatures: N)` count and the `[tapped - untaps...]` tag do
the perceiving) and drop to a compressed nod anchored to the marker strings. **Kept, compressed:** the
SHROUD line — representation does NOT surface "shroud doesn't stop a non-targeted sweep," and it is the
one false belief still carried only by the guide. Net: the three ~4-line bullets collapse to one
4-line block anchored to `(creatures: N)` / `[tapped - untaps...]`, shroud spelled out. The countable
trigger itself is FROZEN (obeyed, 0 overshoot). See strategy.txt SWEEP TRIGGER.

### 3. [MINOR GUIDE — one safe refinement] Black Sun's Zenith cast for an X below the target's toughness
vs109 seq10 (t4): BSZ cast with the menu capped at X=1 (mana-limited) into a 2/2 Rakdos Cackler — X=1
gives -1/-1, leaves a 1/1 alive, wastes the whole sweep turn. The X-menu discipline is fine (it picked
option 1 = biggest offered); the error is casting BSZ AT ALL when the affordable X falls short of the
toughness. One compressed guardrail added to the BSZ entry ("only cast BSZ when the biggest X you can
afford >= the toughness you need dead; else Wrath/Damnation, which kill any toughness"). Single
instance, low-risk refinement — does NOT touch the fire trigger.

### 4. [STRUCTURAL — the whole loss column now] Reach starvation + no closer
With the overshoot gone, all four losses collapse to one deck-construction fact: **deck140 stabilizes
the board correctly and cannot close before it dies.**
- **vs109 (t27!):** the pilot swept ~a dozen times correctly (BSZ, Pyroclasm x2, Damnation x2, Cruel
  Edict x3, Lightmine) and dragged a mono-red aggro deck from t15-dead (wave-7) to t27, opp at 6, and
  still lost — mono-red rebuilds faster than one-for-one sweeps and deck140 has no early lifegain, no
  blocker, and a 1-damage/turn clock. Faithful execution, no reach.
- **vs133 (t17):** mono-B blitz; extended from t11 to t17 but the same reach wall.
- **vs110 (t15):** swept three times (Pyroclasm t6, Lightmine t8, BSZ X=3 t10); affinity's clock
  (Cranial-Plating'd Ornithopter -> 15/5 Master of Etherium) outran the one-shot sweeps.
- **vs44 (t15):** a single Faerie Bladecrafter snowballed with a +1/+1 counter every turn (2/2 -> 7/7)
  while the guide's fire trigger correctly held at L20-18; by the time life dropped, sweeps couldn't
  catch the snowball (see WATCH). Reach + a snowball edge case.

Do not answer reach starvation with louder prose — the deck needs teeth (notes.md #4). The only guide
lever here (already shipped wave-7) is the Lightmine Field deploy line, and it is being OBEYED
(deployed vs110 t8, vs109 t24, vs135 t11).

### 5. [INFRA / STRUCTURAL] Timeout triage x2 — NEITHER is a play failure
- **vs135 (TIMEOUT, deck140 AHEAD 21-16 at t28, 89 decisions, 0 fallbacks):** a clean slow grind the
  pilot was WINNING — Staff of Nin pinging the opponent's face every upkeep (20 -> 16), zero creatures
  to answer for long stretches, correct holds. Timed out on wall-clock (decision density x ~10s median
  + six >30s spikes), not on play. This is wave-7's vs135 de-noised of its fallback cascade: same slow
  clock, now WINNING and clean.
- **vs131 (TIMEOUT, deck140 BEHIND 24-33 at t50, 103 decisions):** opponent this run was a UWx control
  build (Islands + **Essence Scatter**), not the wave-7 burn deck. deck140 reached 13 mana and cast
  **Blightsteel Colossus at t44 — COUNTERED by Essence Scatter** (`Blightsteel: stack -> library`,
  machine-readable in `events`). With its only closer countered and Rakdos X=11 not lethal at opp-33,
  the pilot correctly held Rakdos and cast nothing with a full board of mana — no line to close. Pure
  no-closer grind vs counter-control. Structural, not play.

---

## OVERSHOOT / DISPLACEMENT AUDIT (mandatory)
1. **Wave-7 SWEEP TRIGGER + three false-belief kills.** VALIDATED, not overshot: 0 held-sweeper-
   while-C>=1 decisions (Finding 1). The fire trigger did NOT over-fire either — no sweeper wasted into
   a C=0 board (every C=0 turn deployed a finisher or held correctly). Clean middle calibration.
2. **Displacement check:** with the hold seam fixed, did the leak move upstream/adjacent? Audited the
   full chain — sweep (obeyed) -> finisher deploy (Staff cast every game, Lightmine 3 games, Blightsteel
   the one game it was affordable) -> close (fails: countered vs131, too-slow everywhere else). The
   residual is not a new play seam; it is the CLOSER being too slow / answerable. Deck construction
   (Findings 4/5), not a displaced guide leak.
3. **Rakdos hold (#1 rule).** FROZEN and obeyed: held on every non-lethal window (incl. vs131 X=11 at
   opp-33), one defensible desperation fire (vs109 t26, X=6 at L3 dying). No early-chip waste. Freeze.
4. **X-menu / Staff targeting / Lightmine / Mulligan.** All obeyed (Staff pinged the face on 100% of
   activations; X picked option 1 4/4). Freeze/compress.
5. **No attack-floor issue** — deck runs ~no creatures; Blightsteel was cast when affordable.

## WATCH (do NOT legislate yet — single seat, re-widening risk)
**Snowballing single evasive threat at high life (vs44 Faerie Bladecrafter).** The guide's fire
trigger correctly holds a single creature until life<=16, but a creature that gains a +1/+1 counter
every turn grows past sweep range while you wait. A real opportunity-conversion miss (it partly cost
vs44) BUT adding a new high-life fire condition risks re-opening the wave-7 overshoot just closed.
This is the FIRST clean corpus for the re-anchored trigger (turnaround mode) -> NOT adding a fire
condition this wave. WATCH: promote to a tight countable line ("a creature whose printed power keeps
RISING each turn — remove it on sight, it won't get smaller") only if a second seat reproduces the
snowball-past-sweep-range loss.

## STRUCTURAL residual (unchanged, now the entire loss column)
Reach starvation + no closer. 14 sweepers is the right density; the deck cannot beat a rebuilding aggro
clock or a counter-control durdle because it has no early stabilizer and its finishers are slow (Staff
1/turn), gigantic (Blightsteel {12}), or held (Rakdos for lethal). Teeth, not text — notes.md.

## Wave-7 findings status
- **W7 #1 re-anchored SWEEP TRIGGER (countable C)** — VALIDATED. Overshoot class 8 -> 0. Freeze.
- **W7 false-belief kills (tapped/artifact/shroud)** — CONFIRMED FIRED + obeyed. Representation carries
  tapped+artifact -> DEMOTE those two; keep shroud compressed (Finding 2).
- **W7 Rakdos hold** — still obeyed. Freeze.
- **W7 Lightmine Field deploy line** — obeyed (3 deploys). Freeze.
- **Reach starvation** — PERSISTS, now the whole loss column. Deck construction (notes.md).
