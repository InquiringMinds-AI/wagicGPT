# Wave 7 review — deck133 (Phyrexian Asphodel, mono-black devotion midrange)

Corpus: run `matchups-20260713-222831`, 6 games, deck133 the GPT seat in each.
Record **4/6** — deck133's best wave (waves 4->7: 2/6 -> 3/6 -> 3/6 -> 4/6), and it BEAT the
corpus's #1 deck (44v133, winner=133). WON vs44, vs140, vs110, vs131; LOST vs135, vs109.
Mode: **OBEYED-BUT-LOSING / TURNAROUND-PRESERVATION.** Record rising, every wave-6 fix
validated, both losses are close construction/variance races (died at **-1** in both). Revised
as a near-zero constrained DIFF — freeze-check at the bottom.

| opp | seat epoch | result | end my/opp | last T | recs | fallbacks | latmed | latmax | n>=60s |
|-----|-----------|--------|-----------|--------|------|-----------|--------|--------|--------|
| 44  | 1784002523 | **WON**  | 13 / **0**  | 13 | 41 | -- | 6.9s | 47.7s | 0 |
| 140 | 1784005333 | **WON**  | 17 / **0**  | 11 | 30 | -- | 7.4s | 44.4s | 0 |
| 110 | 1784007189 | **WON**  | 12 / **-2** | 17 | 87 | -- | 7.6s | 40.9s | 0 |
| 131 | 1784008428 | **WON**  | 23 / **-1** | 17 | 77 | -- | 8.4s | 54.9s | 0 |
| 135 | 1784003384 | LOST     | **-1** / 16 | 12 | 54 | empty_reply x1 | 7.6s | 120.0s | 1 |
| 109 | 1784004400 | LOST     | **-1** / 14 | 10 | 34 | -- | 8.2s | 17.3s | 0 |

Harness sweep: **0 desyncs** across all 6 games (first-int-of-reply == choice; parser clean).
**1 fallback total** (empty_reply, vs135, on a single latency timeout at 120s — the only
>=60s call in the deck's corpus). No engine no-op decided a game. No numeric-offset trap. The
priority re-ask multiplier persists (vs110: 57 priority; vs131: 40) — infra, not a leak.
Latency is healthy this corpus (median 6.9-8.4s; 1 timeout in 323 deck133 decisions).

---

## The four wave-6 fixes — ALL VALIDATED

### (1) Thoughtseize / Inquisition SELF-COST SPLIT (wave-6 finding #1) — HELD and well-calibrated.
The wave-6 decisive misplay was Thoughtseize paid into a KNOWN-EMPTY hand (vs135 wave-6 seq24,
the -2 that lost a 2-life race). This wave it **did not recur**, and the split rule fired with
precision:
- **vs135 (loss): only ONE Thoughtseize all game — T1 seq2** (inside the sanctioned turns-1-3
  window). No second Thoughtseize, no empty-hand cast. The wave-6 empty-hand misplay is gone.
- **Past-turn-3 Thoughtseize fired ONLY on a real named threat**, exactly the guide's exception:
  - vs140 seq17 (T9, my19/opp16): Thoughtseize took **Damnation** — a sweeper, the textbook
    "a specific card you can see will kill you" case. Directly enabled the win (its Geralf's
    Messengers survived and closed opp 16->0).
  - vs44 seq18 (T9, my17/opp18): "strip the opponent's counterspells/removal before committing
    to an attack" — stripping counters vs the counter deck (justified).
  - vs110 seq30 (T9, my14/opp22): vs affinity at healthy life (not the empty-hand trap).
- **Inquisition cast freely** (free) in every game. The split kept the free card liberal and
  put the brake only on the 2-life card. Clean end-to-end validation.

### (2) Gray Merchant #1-entry ELSE clause (wave-6 finding #2) — disobeyed ONCE, harmless; premature-into-counter did NOT recur.
The wave-6 misplay was a drain-2 GM cast into open counter mana that got countered (vs131
wave-6 seq69). This wave:
- **vs131 seq72 (T15, my16/opp16, shown drains 5):** the pilot cast GM even though NEITHER gate
  held (real drain 7 < opp 16; my 16 > 10) — a technical ELSE-clause violation. BUT it was
  **harmless-to-beneficial**: deck131 had no counter mana open (unlike wave-6), a **second GM**
  backed it (recast next turn), and it WON (opp 16->9->...->-1). The destructive version of the
  misplay (premature cast INTO a counter) did not recur. Partial validation.
- **Caveat surfaced (routes to representation):** at seq72 the pilot IGNORED the guide's "read
  the shown N and ADD 2" instruction and instead **manually counted devotion (got 7, then 9
  for the drain) — both wrong**; the real drain was 7 (shown 5 + 2). The manual count was noisy
  and only coincidentally landed near the truth. The guide workaround is being bypassed in
  favor of error-prone runtime arithmetic.

### (3) Anti-aggro discard-strip line — FIRED (counts below), but mis-targeted at the wrong threat axis vs deck109.
- **vs109 (loss, mono-red GOBLIN go-wide):** T3 Inquisition took **Searing Spear** (burn) — the
  anti-burn strip fired. But it **did not matter**: deck109 killed via a **Goblin token SWARM**
  (the narration is a wall of "1 dealt by Goblin to you", my17->15->11->7->4->-1), not burn to
  the face. Discard strips ONE card; a go-wide token engine floods faster than discard can
  answer, and deck133's only removal that touches width (Liliana edict) removes ONE creature.
  See finding 1.
- **vs44 (win, counter deck):** T1 Thoughtseize + T9 Thoughtseize + T9 Inquisition explicitly
  stripped **Counterspell / Arcane Denial** ("strip counterspells before attack"). Strip FIRED
  and bought the tempo that won — see finding 2.
- **vs135 (loss):** T1 Thoughtseize took Arcum's Astrolabe, T3 Inquisition took Search for
  Azcanta — stripped card-advantage engines (not burn); reasonable, did not change the race.

### (4) Gray Merchant +2 drain OFFSET — CONFIRMED AGAIN on the one cleanly-resolving cast.
3 GM casts this corpus:
- **vs131 seq72 (RESOLVED):** option `{right now: drains 5}`; narration `Opponent's life -7
  (now 9)` + `Your life +7 (now 23)`. **Actual drain = 7 = shown 5 + 2. Offset CONFIRMED**
  (`auto=lifeleech: -type:manab:mybattlefield` — GM's own {B}{B}=2 devotion added on
  resolution; consistent with wave-5 3/3 and wave-6 1/1 -> now 4/4 across waves).
- **vs131 seq74 (RESOLVED):** second GM (recurred to hand T17), shown drains 5, helped close
  opp 4->-1.
- **vs44 seq27 (NOT resolved — COUNTERED):** shown drains 8. deck44 topdecked a fresh
  Counterspell (`Gray Merchant: hand -> stack` -> `Counterspell: hand -> stack` -> `Gray
  Merchant: stack -> graveyard`) AFTER the two discards had stripped its earlier counters. So
  vs44 was **won by BEATS, not by the drain** (finding 2). A clean object lesson: read the
  NARRATION, not the pilot's plan prose (seq28's plan confidently asserted "opponent goes to 3,
  I gain 8" over a spell that never resolved).

---

## The findings that matter

### 1. [LOSS ATTRIBUTION — vs109, a W->L swing that is DRAW VARIANCE, not a guide regression] Raced to -1 by a Goblin swarm deck133 structurally cannot block.
**vs109 (LOST my-1/opp14 at T10) — mono-red go-wide (Rakdos Cackler, Goblins, Boros Reckoner,
Stromkirk Noble, Searing Spear).** deck133 WON this matchup in wave-6; this wave it lost — the
swing is variance, not the guide breaking:
- **The keep was guide-correct:** opening hand = 3 lands (inc. a fetch) + 2 Inquisition +
  Thoughtseize + **Bloodghast** — a valid "2-4 lands with a creature" keep. Not a mulligan
  error.
- **Structural problem: Bloodghast CANNOT BLOCK** (guide line, `auto=` confirmed), so deck133's
  only early body was a non-blocker while 4-5 Goblin tokens chip-raced it. The designated walls
  — Geralf's Messenger (undying) and Phyrexian Obliterator — arrived late (Obliterator T7 at
  my11) or not at all, and **Geralf's Messenger was never drawn**. Obliterator blocks ONE
  attacker; the swarm has many.
- **The strip mis-fired at the wrong axis:** Inquisition took Searing Spear (burn), but the
  Goblins (width) did the killing. **Not guide-fixable at the guide layer** — deck133 has no
  sweeper, and an edict removes one token. Route to deck construction (finding 3).
- **Attribution: deck construction (no sweeper / no early blocker vs go-wide; Bloodghast can't
  block) + variance (walls not drawn).** FREEZE the guide; write no anti-passivity pressure
  (deck133 acted every turn — it simply had no answer to width).

### 2. [THE WIN OVER deck44 — the #1 counter deck — CONFIRMED FROM THE LOG] Discard strips the EARLY counters and buys tempo; Phyrexian Obliterator's trample + sacrifice-punish wins; the GM finisher was a topdecked-Counterspell casualty.
**vs44 (WON, deck44 to 0 at T13).** The on-paper answer (discard strips counters) is CONFIRMED
with an important qualification the narration makes plain:
- **Discard stripped the early counters:** T1 Thoughtseize; T9 Thoughtseize (reply: "strip
  counterspells/removal before committing to an attack"); T9 Inquisition (reply: "likely Arcane
  Denial... even if they have a Counterspell"). This bought the tempo to land Obliterator and
  Liliana under a counter deck.
- **The kill was BEATS, not the drain:** at T12 the GM finisher (shown drains 8) was
  **Counterspelled by a topdeck** deck44 drew after its earlier counters were gone. deck133 won
  anyway because **Phyrexian Obliterator (5/5 trample) + its damage-punish** dismantled deck44:
  when Obyra (2/2 flash flyer) blocked Obliterator, its damage forced deck44 to sacrifice
  **Scion of Oona + 2 Islands** (`auto=` "whenever dealt damage, sacrifice that many
  permanents"), and Bloodghast+Obliterator beats took opp 11->7->0.
- **Durable lesson (not a guide change):** discard strips the counters IN HAND; it cannot strip
  the one drawn later, so the win depended on the **beatdown backup** (Obliterator + recursive
  Bloodghast) the guide's impact curve already deploys. Optional one-clause note: GM can still
  be countered by a topdeck vs an open-mana control deck, so keep beating down.

### 3. [DECK CONSTRUCTION — not guide-fixable] The same two structural residuals decided both losses.
- **No early blocker; Bloodghast (the T2 play) cannot block.** Both losses (vs109 Goblins,
  vs135 Icehide Golems) were races deck133 lost because it could not WALL early — its walls
  (Geralf's Messenger undying, Phyrexian Obliterator) are 3-4 drops and draw-dependent. A deck
  that races but cannot block loses the close ones (both at -1). Visible at the seam level:
  only 1 `blockers` decision in the whole corpus.
- **No reach except Gray Merchant; GM density 4/60 with no selection.** GM reached the cast
  list in only **2 of 6 games** (vs44, vs131) — unchanged from wave-6 — and NEVER during a
  low-life crunch, so the stabilize-at-<=10 trigger is STILL unexercised across two waves. In
  both losses GM never came. The discard/fetch self-damage (Thoughtseize 2 + fetch 1s) supplied
  the exact 2-life margin at -1 in both losses. Route upstream: +1 GM or cheap selection; a
  1-copy Thoughtseize->Inquisition shift would fit the pool.

### 4. [REPRESENTATION — routes to engine, not the guide] The GM drain option under-displays by 2 and the pilot bypasses the guide workaround with error-prone manual counting.
The option shows `{right now: drains N}` where the real drain is N+2. The guide's "read N, add
2" workaround is being **ignored** — vs131 seq72 the pilot manually recomputed devotion and got
the wrong number. The clean fix is REPRESENTATION: make the option display the RESOLVED drain
(include GM's own {B}{B} = +2), which would let every deck's guide DELETE the +2 workaround.
Flagged for an A/B, not a wave edit (single-seat; the workaround is KEPT this wave).

---

## MANDATORY AUDITS

### (1) Displacement / overshoot on the impact-ranked floor + discard-split.
- No overshoot into low-impact discard: wins satisfied the floor with removal/creatures (vs140
  Geralf's + strip-the-Damnation; vs110 Geralf's + beats; vs131 Fatal Pushes + Geralf's + GM).
- Discard-split did NOT under-fire where right: Thoughtseize fired on Damnation (sweeper) and
  counters — the sanctioned exceptions — and did not fire into empty/harmless hands. Correctly
  scoped, not over-braked.
- Impact ranking held: removal/creature above discard in every win.

### (2) Obeyed-but-losing on the two losses.
Obedience HIGH in both: guide curve deployed, Collective Brutality -2/-2 removal used (vs135
seq31), Fatal Push at the actual attacker, blocked/attacked when it had bodies, only 1
Thoughtseize in the loss. Both attribute to construction (no early blocker / no sweeper vs
go-wide / no reach drawn) + variance (died at -1). NOT a rewrite trigger — freeze the obeyed
lines, route residuals upstream.

### (3) Convergence watch — reflexive high-life blocking: NEGATIVE for this deck.
Only ONE `blockers` decision all corpus (vs109 seq25: Phyrexian Obliterator blocks Boros
Reckoner at my11 — correct survival block; Obliterator 5/5 survives Reckoner's 3 and its punish
forces sacrifices). No reflexive high-life chump. Nothing to add.

### (4) Timeout triage.
No game timed out this corpus (all 6 ended by state-based loss/win). The single 120s latency
call (vs135) produced one empty_reply in a game already being lost on board — not a durdle, not
decision-relevant. No anti-passivity pressure warranted.

### (5) Executability audit.
`ask`/`attackers` seams well-populated every game; `blockers` RARE (1 all corpus) because
deck133's early creatures either can't block (Bloodghast) or aren't drawn — the structural
blocker-absence from finding 3, at the seam level. The guide's DEFENDER/wall lines are correct
but the deck often lacks the body to execute them. Not a guide-writable fix.

---

## FREEZE-CHECK (turnaround-preservation — every frozen line accounted for)
- **Act-floor / impact-ranked cast list + GM #1 ELSE clause** (guide lines 8-27) — OBEYED; the
  Thoughtseize/Inquisition split held and was well-calibrated. KEPT VERBATIM.
- **GM +2 read + two gated triggers** (lines 53-62) — +2 offset confirmed again (vs131); ELSE
  disobeyed once but harmless. KEPT VERBATIM; ONE micro-diff: add "IGNORE your own devotion
  count — use ONLY the shown number + 2" (pilot manually miscounted at seq72).
- **DEFENDER role + Phyrexian Obliterator wall** (lines 29-44) — OBEYED (vs44 Obliterator won
  it; vs109 Obliterator blocked). KEPT VERBATIM.
- **VS RED/BURN section** (lines 46-51) — fired (vs109 stripped Searing Spear). KEPT; the
  go-wide qualification is in findings, NOT added as guide text (construction-bound; the edict
  line already says "best vs ONE creature").
- **Collective Brutality -2/-2, Bloodghast can't-block + recursion, Fatal Push, Liliana edict,
  Yawgmoth, fetch, mulligan, deploy curve** — untested or obeyed. KEPT VERBATIM.
Nothing frozen was dropped. The ONLY new text is the one-clause GM devotion-read tightening.
