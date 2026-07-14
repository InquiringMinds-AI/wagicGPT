# Wave 9 review — deck133 (Phyrexian Asphodel, mono-black devotion midrange)

Corpus: PRIMARY `matchups-20260714-094942` (21 games, round-robin over 44,109,110,131,133,135,140;
binary a40671057 — cast-restriction oracle gating + cast-seam livelock breaker + broadened punisher
rider + own-targets annotation; wave-8 guides; post-A/B core prompt, the `hand -> stack` anchor GONE).
deck133 was the GPT seat in 6 games. **The 135v133 livelock is FIXED — that game COMPLETED to a T11
win and is INCLUDED this wave** (excluded nothing, per brief).

**Real record 3W / 3L** — WON vs140, vs135, vs131; LOST vs110, vs44, vs109.
vs wave-8 (control arm): vs109 W->L (variance — different corpus/draw), vs135 wedge->W (engine fix),
rest unchanged. This is a DIFFERENT round-robin from wave-8's control arm, so column swings are draw
luck, not the guide (which is byte-identical to the wave-8 output — see freeze-check).

| opp | result | end my/opp | last T | recs | note |
|-----|--------|-----------|--------|------|------|
| 140 | **WON** | 24 / **-1** | 15 | 60 | beatdown; GM drain-8 mid-game + Bloodghast/Obliterator/GM beats |
| 135 | **WON** | 27 / **-1** | 11 | 33 | LIVELOCK FIXED, first completion; GM combined-lethal (drain 10 + 7 swing) |
| 131 | **WON** | 5 / **-1** | 23 | 71 | 23-turn grind; Obliterator/Geralf's/Bloodghast beats; opp rebounded to 19 late |
| 110 | LOST | **-4** / 10 | 12 | 46 | affinity Memnite+Plating race; no early blocker drawn |
| 44  | LOST | **-2** / 4 | 14 | 44 | Faerie flyer tempo; no flyer answer; self-damage margin |
| 109 | LOST | **0** / 18 | 10 | 32 | Hellrider aggro; threat-light draw, no wall/GM, both attack windows forced-empty |

**Mode: OBEYED-BUT-LOSING / TURNAROUND-PRESERVATION.** Wins execute the guide's own lines (beatdown
curve; GM as finisher AND stabilizer AND combined-lethal component). Losses are construction/variance
races the deck cannot answer (no early blocker, no flyer/reach, threat-light draws). Revised as a
ZERO-edit frozen diff — the wave-8 GM `+2`-deletion is already shipped in the live guide and vindicated.

## Harness / representation sweep — CLEAN
- **0 real desyncs** (ask/priority: first-int-of-reply == choice in all 6 games; no DESYNC flag in any
  stderr). Naive multi-target/attacker replies are not desyncs.
- **0 `fails validation`** in all 6 stderr; **0 defer records** in the JSONL.
- **No engine no-op decided a game.** One infra artifact only: priority re-ask multiplier on
  single-option "Put in Play with <fetch>" re-offers (mana unspent) — decision-count inflation, not a leak.

---

## VERIFICATION LIST (brief item 5 + carried watches)

### (A) Bare-N Gray Merchant thresholds — CONFIRMED WORKING, now on a 3-cast sample.
The wave-8 deletion of the "+2" workaround (shipped display shows the RESOLVED total incl. GM's own +2)
is validated across THREE casts this corpus — up from the single wave-8 cast — all shown == resolved:
- **vs110 seq28 (T9, my8/opp18):** `{right now: drains 6}` -> resolved my8->14, opp18->12 (+6/-6).
  Shown 6 == resolved 6. Cast under the **<=10 stabilizer** trigger (my8) — correct, the panic-button use.
- **vs140 seq41 (T11, my18/opp22):** `{right now: drains 8}` -> resolved my18->26, opp22->14 (+8/-8).
  Shown 8 == resolved 8.
- **vs135 seq28 (T11, my17/opp14):** `{right now: drains 10}` -> resolved my17->27, opp14->4 (+10/-10).
  Shown 10 == resolved 10 (devotion: Obliterator {B}{B}{B}{B}=4 + GM {B}{B}=2 + Bloodghast {B}{B}=2 +
  other pips = 10; display included GM's own 2).
No cast anywhere showed the old drains-N-resolves-N+2 behavior. The `N+2 -> N` deletion is SAFE and
CLOSED — a pilot obeying the current bare-N guide computes lethal correctly. **FREEZE.**

Two off-condition GM casts (WATCH, not a change — see notes.md #1):
- **vs140 seq41** (N=8 < opp22, my18>10) — strictly rule-1-ELSE says WAIT/build devotion, but the ONLY
  offered plays were GM, Fatal Push (own-targets-only = useless), or Cast nothing. **No creature was
  offered**, so GM was the only real development. Defensible.
- **vs135 seq28** (N=10 < opp14, my17>10) — the pilot cast GM then swung Bloodghast+Obliterator (7 power)
  for opp 14 -> 4 -> -1: a correctly-recognized **COMBINED lethal** (drain 10 + 7 attack = 17 > 14).
  The guide's rule-1 win condition ("N >= opponent's life") ignores the attack step, yet the pilot
  bridged it correctly. Latent guide gap the pilot self-covered — WATCH, no edit (a combined-lethal
  clause risks encouraging premature small-GM casts the guide already warns against).

### (B) 135v133 livelock — FIXED, confirmed at this seat.
The matchup completed to a **T11 WIN** (my27/opp-1), 31 decisions, 0 defer, 0 fails-validation. The
wave-8 turn-2 wedge (6 records, all turn 1) is gone. **Exclusion lifted — this game is in the
play-quality set this wave.**

### (C) Obliterator punisher-rider — SIGHTED and RESPECTED (closes the wave-8 open item).
deck133 attacked with Phyrexian Obliterator; in the OPPONENT (defender) seats the incoming attacker
rendered the FULL broadened rider on its `A#.` descriptor:
```
A2/A3. Phyrexian Obliterator (5/5) [trample] {text: Trample -- Whenever a source deals damage to
Phyrexian Obliterator, that source's controller sacrifices that many permanents.}
```
- **OPP135 blockers seq38 (T11):** rider rendered; defender chose "Boreal Druid blocks Bloodghast" —
  declined to block Obliterator.
- **OPP131 blockers seq56 (T23):** rider rendered; defender chose "Young Pyromancer blocks Geralf's
  Messenger" — declined to block Obliterator.
Both defenders saw the sac-punisher text and did NOT block into it. This closes the wave-8 verdict
("#5 punisher rider did NOT fire for Obliterator / didn't meet a block window this corpus"): the
broadened rider fires for the DEFENDER seat and opponents respected it. (deck133's own DEFENDER/wall
use of Obliterator was untestable — 0 blocker windows for deck133 all corpus, structural: Bloodghast
can't block, walls draw-dependent, games were beatdowns or races without a wall drawn.)

### (D) Liliana coexist-casts — STEADY.
Liliana of the Veil cast twice (vs131 seq18 T7, seq54 T19); Liliana, the Last Hope cast once
(vs110 seq19 T7). All resolved; 0 fails-validation, 0 defer, no loop. The wave-7 no-legal-target loop
stays FIXED at this seat.

### (E) Thoughtseize discipline — RECURRED and STRENGTHENED (route to REPRESENTATION, freeze guide).
FOUR late Thoughtseizes into <=2-card hands across THREE games (up from wave-8's 2 instances):
- **vs131 seq58 (T19, my7, opp hand = 0):** cast Thoughtseize into a **KNOWN-EMPTY hand** — the guide's
  sharpest prohibition ("hand shows 0 cards ... DO NOT cast Thoughtseize"). The ONLY other option was
  "Cast nothing," so the pilot **paid ~2 life for literally zero effect**. Its reply FABRICATED a target:
  *"the opponent may have a removal spell or a counterspell this turn"* — inventing cards in a hand the
  prompt showed as `Opponent hand size: 0`. WON but genuinely CLOSE game (opp rebounded to 19, deck133
  finished at my5) — the sharpest, closest-to-deciding instance yet.
- **vs131 seq19 (T7, my13, opp hand = 1):** late seize into a 1-card hand.
- **vs44 seq21 (T7, my15, opp hand = 2):** late seize in a LOSS; contributed self-damage to a race lost at 0.
- **vs140 seq46 (T13, my26, opp hand = 2):** late seize in a blowout win — non-deciding.

Diagnosis: the rule is present, correct, and **maximally prominent** (stated twice — impact-list #5 AND
DECIDING SITUATIONS). The pilot READ `Opponent hand size: 0` and FABRICATED cards over it. This is the
representation-beats-instruction wall at a HALLUCINATED-EMPTY-HAND — louder guide prose cannot fix a
pilot that invents cards in a 0-card hand (and the skill forbids escalating wording at a read-then-
disobey wall). **The wave-8-anticipated escalation now has its recurrence:** route to REPRESENTATION —
an OPTION-LINE annotation on the Thoughtseize option when the opponent's hand is empty, exactly
analogous to the shipped own-targets annotation. Proposed string: `Cast Thoughtseize {b} - opponent's
hand is EMPTY; this only costs you 2 life`. Routed to general-suggestions.md; guide FROZEN.

---

## MANDATORY AUDITS

### (1) Displacement / overshoot on the impact floor + discard split.
No overshoot into low-impact discard in the wins: 140/135/131 satisfied the floor with creatures/GM +
beats. Impact ranking held (creature/removal above discard). Inquisition cast freely T1-3. The
Thoughtseize misfires were UNDER-braked (leak E), not over-braked. No displacement into an else-branch.

### (2) Obeyed-but-losing on the three losses.
Obedience HIGH: curve deployed; attacked when it had bodies (vs44 all 4 windows Bloodghast+Geralf's;
vs110 Bloodghast beats; vs135/131/140 full swings). vs109's two "no attackers" windows were FORCED
(0 attackers offered — threat-light draw, no board). GM stabilizer/combined-lethal executed in the wins.
All three losses attribute to construction (no early blocker / no flyer answer / threat-light draw vs
aggro). NOT a rewrite trigger.

### (3) Convergence watch — reflexive high-life blocking: NEGATIVE for this deck.
0 blocker windows for deck133 all corpus (structural). No reflexive high-life chump possible.

### (4) Timeout triage.
No genuine timeout in deck133's real games (all 6 ended by state-based win/loss). The prior wedge (135)
now completes cleanly. No anti-passivity pressure warranted.

### (5) Executability audit.
`ask`/`attackers` seams well-populated; `blockers` = 0 (structural, draw-dependent walls). The
DEFENDER/wall line could not execute (no wall drawn in a defending spot), but the wall MECHANIC is
proven live via the opponent-side rider rendering (C). Not a guide-writable gap.

---

## FREEZE-CHECK (turnaround-preservation — every kept line accounted for; ZERO edits this wave)
The live guide is BYTE-IDENTICAL to the wave-8 output (`diff -q` confirmed). Every line stands:
- **Identity + control/counter beats-are-the-win paragraph** — vindicated by 3 wins (esp. vs135
  combined-lethal, vs131 grind); KEPT VERBATIM.
- **Impact-ranked cast list + GM #1 ELSE (bare-N)** — OBEYED; the wave-8 bare-N is shipped and validated
  on 3 casts (A). KEPT VERBATIM.
- **GRAY MERCHANT block (bare-N "shown N is the FINAL drain, use directly, do not recompute")** — the
  wave-8 fix; CONFIRMED (A). KEPT VERBATIM. (Combined-lethal gap is a WATCH, not an edit.)
- **ROLE CHECK + DEFENDER/Obliterator-wall line** — untestable at deck133's seat (0 blocks) but the wall
  mechanic renders and is respected opponent-side (C); KEPT VERBATIM.
- **Thoughtseize rule (impact-list #5 + DECIDING SITUATIONS empty-hand line)** — present, correct,
  maximally prominent; disobeyed via hallucinated-empty-hand; fix routes to representation (E). KEPT
  VERBATIM — no louder prose.
- **VS RED/BURN, Bloodghast, Fatal Push, Collective Brutality, Liliana, fetch, Yawgmoth, Geralf's,
  Arena, attack, mulligan, DECIDING SITUATIONS** — untested or obeyed; KEPT VERBATIM.
Nothing dropped. **The only change this wave is ZERO — the guide is frozen and stands alone for the pause.**

## In my words
deck133's plan is unchanged and working: cast a black creature every turn, remove what races you, and
finish or stabilize with Gray Merchant's blocker-proof drain — and this corpus finally exercised all
three GM roles (finisher via combined-lethal vs135, stabilizer via the <=10 trigger vs110, mid-game
tempo drain vs140), every one with a correct bare-N display. The engine caught up on two fronts the
guide had been carrying (GM display now bare-N; the Obliterator wall's punisher now renders for and is
respected by defenders). The single live piloting leak is the pilot inventing cards in an empty hand to
justify a self-damaging Thoughtseize — a hallucinated-hand problem that belongs to representation, not
to another paragraph of a rule already stated twice. The losses remain a decklist that cannot wall
early or reach through flyers. So the guide freezes clean for the pause, and the one open item ships as
an option-line annotation, not guide prose.
