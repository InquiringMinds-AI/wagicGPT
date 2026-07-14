# Wave 6 review — deck133 (Phyrexian Asphodel, mono-black devotion midrange)

Corpus: run `matchups-20260713-190011`, 6 games, deck133 the GPT seat in each.
Record **3/6** (WON vs44, vs109, vs110; LOST vs135; TIMEOUT vs131 draw, vs140 loss).
Mode: **OBEYED-BUT-LOSING / TURNAROUND-PRESERVATION.** The record is rising and the
three wins are the wave-5 guide working as written. Revised as a constrained DIFF, not a
rewrite (freeze-check at the bottom).

| opp | seat epoch | result | end my/opp | last T | recs | latmed | latmax | n>=60s |
|-----|-----------|--------|-----------|--------|------|--------|--------|--------|
| 44  | 1783991689 | **WON**    | 16 / **-2** | 13 | 57 | 12.1s | 78.8s | 4 |
| 109 | 1783996045 | **WON**    | 6 / **-2**  | 15 | 55 | 10.3s | 28.0s | 0 |
| 110 | 1783995814 | **WON**    | 10 / **0**  | 11 | 35 | 10.2s | 85.2s | 1 |
| 135 | 1783987218 | LOST       | **-1** / 1  | 14 | 42 | 10.7s | 71.9s | 1 |
| 131 | 1783987216 | TIMEOUT(draw) | 17 / 17 | 31 | 89 | 11.7s | 66.7s | 3 |
| 140 | 1783989616 | TIMEOUT(loss) | 15 / 25 | 20 | 105| 10.6s | 69.8s | 3 |

Harness sweep: **desyncs 0/? across all 6 games** (first-int-of-reply vs `choice`; parser
clean again). No engine no-op observed (the vs131 "Yawgmoth" repeats are plan/list
disconnect on an UNAFFORDABLE card, not a no-op — Yawgmoth was never in the option list).
No numeric-offset trap. The only harness artifact is the single-option **priority re-ask
multiplier** inflating decision counts (vs140: 81 priority records; vs131: 20) — infra, see
notes.md.

---

## Answers to the three central questions

### (a) Wave-5 Gray Merchant rules — validated where testable; mostly UNEXERCISED.
- **GM reached the cast list in only 2 of 6 games** (vs44, vs131). It was NEVER offered in
  vs109 (my6), vs110 (my10), or vs135 (my-1) — the three games deck133 got low. So the
  **stabilize-at-≤10-life trigger was never exercised this corpus** (GM not drawn/castable
  during any low-life crunch). It gets no confirmation AND no counter-evidence — KEEP it
  (single-seat can't cut a rule the corpus never tested; it still fixes last wave's decisive
  vs110 miss, which only failed to recur here because GM wasn't offered).
- **Merchant casts: 2.**
  - vs44 seq55 (**correct lethal finisher**): option `... {right now: drains 10}`, my4/opp10.
    Cast → game ended my16/opp-2. **Actual swing = 12 (my 4→16, opp 10→-2). Shown 10 → +2
    offset CONFIRMED** (1/1 resolved cast; consistent with wave-5's 3/3). `auto=lifeleech:
    -type:manab:mybattlefield` verified — GM's own {B}{B}=2 devotion is added only on
    resolution. The "read the shown number and ADD 2" workaround HELD.
  - vs131 seq69 (**premature misplay**): cast at `drains 2`, my18/opp22 — NEITHER lethal
    (2+2=4 < 22) NOR a stabilize (my18 > 10). It got **COUNTERED** (pilot's own seq70 reply:
    "Gray Merchant... which was countered"), tapping deck133 out into deck131's open counter
    mana. The pilot had planned Obliterator first (seq68) then deviated to GM. See finding 2.

### (b) The vs109 WIN — the corpus's only defeat of deck109. PRESERVE THIS PATTERN.
Opponent = mono-red burn-aggro (Pillar of Flame, Searing Spear, Legion Loyalist, Rakdos
Cackler, Goblins). deck133 won my6/opp-2 at T15, by BEATS — **Gray Merchant never appeared.**
The winning line, in the order it executed:
1. **T1 Inquisition → took Pillar of Flame; T3 Thoughtseize → took Searing Spear.** Front-
   loaded discard that STRIPPED THEIR BURN — i.e. removed their reach to deck133's face.
   Discard here was interaction, not durdle.
2. **T4 Fatal Push on Legion Loyalist** (their 1/1 haste attacker) — point removal on the clock.
3. **T5 Bloodghast; T7 Collective Brutality ESCALATE** (discard Bloodghast → "opponent loses
   life AND creature gets -2/-2", killing a Goblin and draining). **The Collective Brutality
   removal mode FIRED** — the exact card-mode wave-5 flagged as MISSED last run. Fixed.
4. **T9 Geralf's Messenger** (ETB drain 2); **T12 blocked Rakdos Cackler with Messenger** to
   hold life at 8 (undying recursion makes it a repeatable wall).
5. **T13 + T15 double alpha-strike** with 2 Bloodghasts + Geralf's Messenger (recurring off
   landfall) → opp 14→6→-2.
The lever: strip their burn early, remove the haste threat, use Collective Brutality AS
removal, then go wide with recursive Bloodghasts. This validates the impact-ranked floor and
the wave-5 Collective-Brutality removal line. **Do not let the "rank discard last" rule revise
away the T1-3 anti-burn discard that won this game.**

### (c) Impact-ranked act-floor (Merchant → removal → creature → discard-last) — mostly OBEYED.
- **Removal/creature above discard: OBEYED in the wins.** vs109 (Collective Brutality removal
  + Geralf's + Bloodghasts), vs110 (Obliterator + Bloodghast beats). No low-impact-discard
  overshoot on the wins.
- **Merchant top slot: obeyed when lethal (vs44); DISOBEYED-in-reverse in vs131** — the #1
  GM entry got taken even though its gating condition ("wins OR saves") was false (finding 2).
- **Discard-last: obeyed, with one decisive residual** — Thoughtseize's 2-life cost paid in a
  race (finding 1).

---

## The findings that matter

### 1. [#1 GUIDE-FIXABLE LEAK — decisive in the only real loss] Thoughtseize's 2 life is the losing margin; it was paid into a KNOWN-EMPTY hand.
**vs135 (LOST my-1 / opp1 at T14 — a race lost by 2 life).** deck133 raced deck135 down to 1
but died first at -1. Its own life went 20→…→10→8→4→-1. **It cast Thoughtseize twice** (T7
seq12 my14; T9 seq24 my10) for −4 life total — exactly the margin it lost by.
- **seq24 is the clean misplay: it cast Thoughtseize at my10/opp6 into an opponent with 0
  cards in hand.** The pilot's OWN next reply (seq26) states "The opponent has 0 cards in hand,
  so Thoughtseize is redundant." It paid (or committed to pay) 2 life to strip an empty hand
  while 2 life from winning the race. `auto=` confirms Thoughtseize's `afterrevealed
  transforms(...life:-2 controller)` always costs the caster 2 life on resolution.
- Root cause: the guide lumps **Thoughtseize (costs YOU 2 life) with Inquisition of Kozilek
  (FREE)**. Inquisition can be cast liberally; Thoughtseize cannot. The pilot has no rule
  telling it the empty/irrelevant hand makes Thoughtseize's 2 life a dead loss.
- Fix (positive, guide diff): separate the two. "Inquisition is free — cast it freely. Pay
  Thoughtseize's 2 life ONLY turns 1-3, or to take a specific card you can see will kill you
  (a sweeper, a burn spell, a bomb). If the opponent's hand shows 0 cards, or holds nothing
  that threatens you, cast something else — its 2 life is the race you lose." Opp hand size is
  on the board.

### 2. [SECONDARY — GM cast when it neither wins nor saves] Premature drain-2 Gray Merchant into open counter mana.
**vs131 seq69** (above). The impact list's line #1 ("Gray Merchant when it wins OR saves you")
and the deploy-curve line ("Turn 5+: Gray Merchant, then keep casting…") together read to the
pilot as "GM is the top pick / cast GM at turn 5+", so it cast GM at 2 devotion (drain 4, not
lethal, not a stabilize) into deck131's open counter mana — and it was countered. The gated
block below already says cast ONLY if lethal or ≤10 life, but the **ELSE is never stated** and
the deploy-curve line invites the unconditional cast.
- Fix: (a) make the #1 impact entry carry its ELSE — "if N+2 < opp life AND your life > 10,
  do NOT cast Merchant yet: cast a creature/Obliterator to build devotion; a bigger drain
  later wins more"; (b) reword the deploy-curve T5+ line so it does NOT say "cast Gray
  Merchant" unconditionally. Low stakes here (game was a draw), but it is the one GM misplay
  and it directly answers the act-floor overshoot audit.

### 3. [DECK CONSTRUCTION — not guide-fixable] Two structural residuals decided the non-wins.
- **No reach except Gray Merchant, and GM density is 4/60 with no selection.** GM reached
  castability in only 2 of 6 games and never during a low-life crunch. vs140 (lost, opp
  gaining life to 25 behind Elixir/Venser's Journal + Wrath resets) is unwinnable without
  reach — deck133 has none but GM, which never came. vs135's race was lost by the discard
  self-damage margin. Route upstream (see notes.md): +1 Merchant or cheap selection/2nd Arena.
- **Discard suite self-damage in a race-heavy pool.** 4 Thoughtseize at 2 life each; both the
  loss (vs135) and the grind (vs140) featured self-inflicted life as a real damage source. A
  1-2 copy Thoughtseize→Inquisition shift would fit the pool; guide mitigation (finding 1) is
  now in place regardless.

### 4. [TIMEOUT TRIAGE] Both capped games are infra + matchup, NOT durdle. Write no anti-passivity pressure.
- **vs131 (draw 17-17, T31):** 89 recs, 3 calls ≥60s (max 66.7s), grindy control mirror-ish
  vs a token/Liliana deck. Neither ahead. Latency + genuine grind; a mild self-durdle
  contribution from the plan/list Yawgmoth loop (seq40-45: PLAN repeats "cast Yawgmoth now"
  while Yawgmoth is unaffordable and unlisted — the pilot correctly takes the listed Fatal
  Push/Inquisition instead, so no bad CHOICE, just a stale PLAN). General-prompt territory.
- **vs140 (loss, behind 15-25, T20):** 105 recs, **81 priority re-asks** (single-option
  multiplier), 3 calls ≥60s. Opponent lifegain/control (Elixir of Immortality, Venser's
  Journal, Wrath of God) — opp life ROSE to 22-25; Wrath reset deck133's board. deck133 was
  slow (first threat Bloodghast T13, Yawgmoth T19) off a threat-light draw. Latency + huge
  re-ask decision density + lifegain matchup + variance. Route the re-ask inflation and the
  no-reach-vs-lifegain to handoff; **no guide change**.

---

## MANDATORY AUDITS

### (1) Displacement / overshoot on the impact-ranked floor + discard-last demotion.
- **No overshoot into low-impact discard on the wins** — vs109/vs110 satisfied the floor with
  removal and creatures, not the cheapest discard. The wave-5 impact-ranking HELD.
- **Discard-last did NOT get UNDER-used where it was right.** There is no instance of the pilot
  needing a protective discard (a sweeper strip) and skipping it. The residual is the OPPOSITE:
  discard OVER-used at a life cost (finding 1), not under-used. The demotion caused no missed
  protective discard.
- **Guardrail for the diff:** the vs109 win proves T1-3 discard that strips BURN is correct and
  must survive the "rank discard last" rule. The revision keys the caution to Thoughtseize's
  LIFE cost and to past-turn-3 / empty-hand, not to discard-as-a-category.

### (2) Obeyed-but-losing on vs135.
Obedience high: deployed the curve, used the Collective Brutality −2/−2 removal mode (seq19),
attacked every legal turn, got opp to 1. The loss is **4 life of Thoughtseize self-damage**
(one into an empty hand) in a race lost by 2 — half guide-fixable (finding 1), half
construction (no reach; GM never drawn). Not a rewrite trigger; freeze the obeyed lines, spend
the wave on finding 1.

### (3) Convergence watch — reflexive high-life blocking: NEGATIVE for this deck.
Only ONE `blockers` decision in the whole corpus (vs109 seq48: Geralf's Messenger blocks
Rakdos Cackler at my **8** life — a survival block, undying makes Messenger a repeatable wall;
correct). Every other block-shaped reasoning appeared inside `attackers` replies as survival
planning at 8-10 life. **No reflexive high-life blocking observed.** deck133 does not exhibit
the deck109 pattern; nothing to add.

### (4) Timeout triage — see finding 4. Both capped games: infra (latency + re-ask density) +
matchup (lifegain) + variance. Neither is a durdle; guide writes nothing.

---

## FREEZE-CHECK (turnaround-preservation — every frozen line accounted for)
Lines the corpus shows OBEYED and driving results; wording preserved, position may move:
- **Act-floor / impact-ranked cast list** (guide lines 7-17) — OBEYED, drove vs109/vs110/vs44.
  KEPT VERBATIM; only the GM #1 entry gains its explicit ELSE (finding 2), and the
  discard #4 entry is split Inquisition/Thoughtseize (finding 1).
- **DEFENDER role + Phyrexian Obliterator wall** (lines 21-36) — OBEYED (vs44, vs110). KEPT.
- **Gray Merchant +2 read + two gated triggers** (lines 38-46) — the +2 held (vs44); triggers
  untested but retained. KEPT VERBATIM; deploy-curve T5+ line reworded so it no longer invites
  an unconditional GM cast.
- **Collective Brutality −2/−2 removal mode** (lines 68-72) — USED this wave (vs109, vs135).
  KEPT; a one-clause "this is your creature removal vs burn-aggro" reinforcement added.
- **Bloodghast can't-block + recursion + go-wide** (lines 57-61) — OBEYED (vs109 double strike).
  KEPT VERBATIM.
- **Fatal Push at the actual attacker** (lines 63-66) — OBEYED (vs109 Legion Loyalist). KEPT.
- **Liliana of the Veil edict / Yawgmoth / fetch lines / mulligan** — untested or obeyed; KEPT.
Nothing frozen was dropped. New text targets only findings 1 and 2.
