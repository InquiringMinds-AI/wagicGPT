# Wave 8 review — deck133 (Phyrexian Asphodel, mono-black devotion midrange)

Corpus: PRIMARY control arm `matchups-20260714-044131` (wave-7 guides, UNCHANGED core prompt,
full wave-7 engine ledger). deck133 was the GPT seat in 6 games; **the 135v133 game is the
KNOWN ENGINE WEDGE** (turn-2 livelock, 6 records, all turn 1 — brief item 7) and is EXCLUDED
from all play-quality claims.

**Real record 3W / 2L** (wedge excluded): WON vs140, vs131, vs109; LOST vs110, vs44.
vs wave-7: vs44 W->L, vs110 W->L, vs109 L->W. All swings are variance — this is the CONTROL
arm running the SAME wave-7 guide, so a flipped column is draw luck, not the guide changing.

| opp | result | end my/opp | last T | recs | note |
|-----|--------|-----------|--------|------|------|
| 140 | **WON** | 18 / **-3** | 17 | 47 | clean beatdown (Geralf's + Bloodghasts) |
| 131 | **WON** | 16 / **-2** | 15 | 40 | relentless Geralf's+Bloodghast, opp 18->13->8->3->-2 |
| 109 | **WON** | 3 / **-4** | 13 | 48 | DEFENDER line: Obliterator wall + GM stabilizer (my2->10) then close |
| 110 | LOST | **-3** / 18 | 8 | 26 | affinity Memnite+Plating race; no early interaction drawn |
| 44  | LOST | **0** / 16 | 14 | 47 | Faerie flyer tempo; threats+flyer-answer undrawn; self-damage margin |
| 135 | (wedge) | 20 / 20 | 2 | 6 | ENGINE LIVELOCK — excluded |

**Mode: OBEYED-BUT-LOSING / TURNAROUND-PRESERVATION.** The wins execute the guide's own lines
(beatdown curve; defender/Obliterator wall; the GM <=10 stabilizer). The losses are
construction/variance races the deck cannot answer. Revised as a near-zero constrained DIFF
(freeze-check at the bottom); the only change is engine-fix-driven.

## Harness-defect sweep — CLEAN
- **0 desyncs** across all real games (first-int-of-reply == choice everywhere; no DESYNC flag fired).
- **0 `fails validation`** in every game's stderr; **0 defer records** in the JSONL — the wave-7
  **Liliana no-legal-target loop is FIXED and confirmed at this seat**: both Lilianas coexist-cast
  in different games (Liliana, the Last Hope in vs110 seq16; Liliana of the Veil in vs44 seq34),
  neither looped.
- **No engine no-op decided a game.** No numeric-offset trap. One infra artifact only: the
  priority re-ask multiplier (vs44 has many single-option "Put in Play with Bloodstained Mire"
  re-asks, seq37-40, mana unspent = re-offers, not real fetches) — decision-count inflation, not a leak.

---

## THE guide change this wave (engine-fix-driven, high confidence)

### Gray Merchant drain display now shows the RESOLVED TOTAL — the "+2" workaround is now WRONG. DELETE it.
Brief inheritance item #6 (probe: drains 6 on devotion-4) is **CONFIRMED AT THIS SEAT.**
- **vs109 seq36 (T11, my2/opp18):** option read `Cast Gray Merchant of Asphodel {3}{b}{b} (2/4)
  {right now: drains 8}`. Narration resolved `Your life +8 (now 10)` / `Opponent's life -8 (now 10)`.
  **Shown 8 == resolved 8.** Board devotion was 6 from other black pips + GM's own {B}{B}=2 = 8,
  and the display INCLUDED GM's own 2. There is no longer a +2 offset.
- The wave-5/6/7 guide carries a "the shown N does NOT count Gray Merchant himself; the REAL drain
  is N + 2; use shown + 2" workaround. That is now **false and actively harmful**: a pilot obeying
  it would compute 10 on a real-8 drain and mis-evaluate lethal / the <=10 stabilizer.
- This is the skill's confirmed-fired-representation-fix terminus (Step 0): a shipped display fix
  DELETES the guide stopgap it replaced. **Applied:** the GM block now reads "the shown N is the
  FINAL drain — use it directly, do NOT add anything, and do NOT hand-count devotion (you will
  miscount)." The anti-hand-recompute clause is KEPT (wave-7's vs131 seq72 hand-count error is
  still a real risk; a correct display plus "trust it, don't recompute" kills both the stale +2
  AND the recompute). Every `N+2` / `N + 2` threshold in the guide (impact-list rule 1, the two GM
  triggers, three DECIDING-SITUATIONS lines) changed to `N`.
- Only ONE GM cast occurred all corpus (GM density 4/60, reaches the list rarely), so this rests on
  the single vs109 cast + the brief's probe. Both agree; no counter-evidence (no cast showed the old
  drains-N-resolves-N+2 behavior). Safe to delete.

**Bonus validation:** the GM **<=10 stabilizer trigger** — flagged "STILL unexercised across two
waves" in wave-7 findings — **finally fired and WON the game** (vs109 seq36: cast at my2, gained 8
to my10, dropped opp to 10, then GM joined the beats T13 to close). Two-uses-two-triggers vindicated;
the defender/GM-panic-button line the guide teaches is now corpus-proven, and it flipped vs109 L->W.

---

## Losses — both construction/variance (freeze the guide, route upstream)

### vs110 (LOST T8) — affinity Memnite+Cranial-Plating race, no early interaction drawn.
- Keep was guide-correct (3 lands + 2 discard + Bloodghast). **Fatal Push was NOT in the opening
  hand** and none was drawn in time; the deck's only early body (Bloodghast) CANNOT block. Memnite
  grew 1/1 -> 4/1 -> 6/1 under Cranial Plating and raced deck133 20->16->8->2 by T7.
- Inquisition stripped one Cranial Plating (seq5); a second Plating + the equip still buffed Memnite.
  At my2 (seq16) the pilot cast Liliana, the Last Hope and used +1 (-2/-1) to KILL the Memnite
  (6/1 -> 4/0) — correct stabilization — but opp had a flyer + wide artifact board and closed T8.
- **Attribution: deck construction (no early blocker, Bloodghast can't block, no sweeper) + variance
  (Fatal Push / walls undrawn).** Not guide-fixable. FREEZE.
- One SUBTLE watch (single instance, not a change): seq12 (T5, my8, being raced) the pilot had
  `Cast Liliana, the Last Hope` offered alongside Bloodghast and chose Bloodghast (a can't-block
  body) — Liliana's +1 (-2/-1) would have killed the 6/1 Memnite a full turn earlier. The pilot does
  not recognize Liliana-the-Last-Hope +1 as removal (the guide lists only Liliana-of-the-Veil -2 as
  removal). Narrow (only kills X/1) and in an already-losing race; see notes.md, NOT added to the guide.

### vs44 (LOST T14, opp at 16) — Faerie flyer tempo; threats undrawn; self-damage was the margin.
- deck133 drew only ONE Bloodghast + a LATE Geralf's (T13); Obliterator, Yawgmoth, Gray Merchant
  never appeared. Its board was a single can't-block Bloodghast for 13 turns while unblockable flyers
  (Faerie Miscreant, Obyra, Archmage of Echoes 4/4) chipped it out — deck133 has **no flyer defense
  and no reach** except the undrawn GM. Died at exactly 0.
- **Self-damage discipline leak (contributing, not deciding):** seq21 (T7, my14->12) cast Thoughtseize
  into an **opp hand of 1** past turn 3 with no visible lethal card — a violation of the guide's own
  Thoughtseize rule ("later ONLY to take a specific card you can SEE will kill you"). Combined with 3
  fetch cracks it self-inflicted ~7 life; it died at 0. Those 2 life were the margin (it never
  stabilized). Also declined to attack with Bloodghast as the early beatdown (seq26/32, opp at 20/18).
- **Attribution: deck construction/variance (no threats, no flyer answer) is the DECIDING factor**
  (opp ended at 16 — deck133 never had a plan running). The Thoughtseize self-damage + non-attacks are
  real decision-level leaks but not outcome-changing here. FREEZE the guide (see the Thoughtseize watch).

### Thoughtseize discipline — a WATCH, not a change.
Both past-turn-3 Thoughtseizes in the whole corpus were into <=1-card hands, **both in losses**:
- vs44 seq21: T7, opp hand 1, my14->12 (above).
- vs110 seq23: **T7, opp hand 0, at my2** — cast Thoughtseize into a KNOWN-EMPTY hand while dying (the
  guide's sharpest prohibition: "hand shows 0 cards ... DO NOT cast Thoughtseize"). The deck was already
  dead to the board (gameend my-3 next turn), so it was non-deciding, but it is a stark disobedience.

The rule is PRESENT, CORRECT, and was OBEYED in the early game (all T1-3 Thoughtseizes were fine; no
empty-hand cast in the 3 wins). Two disobediences, both in already-lost/construction spots. Per
turnaround-preservation freeze mode this is NOT a rewrite trigger — a rule present-and-losing at
distance in non-deciding spots stays frozen and goes on the watch, not into a louder paragraph that
churns the obeyed early-game behavior. If it recurs in a DECIDING spot next wave, route to
representation (surface opp-hand-size at the cast decision) rather than more guide prose.

---

## MANDATORY AUDITS

### (1) Displacement / overshoot on the impact floor + discard split.
No overshoot into low-impact discard in the wins: 140/131 satisfied the floor with Geralf's + beats,
109 with removal + wall + GM. Impact ranking held (creature/removal above discard). The discard split
did not over-brake where right (Inquisition cast freely; the Thoughtseize misfires were UNDER-braked,
not over-braked — the discipline watch above).

### (2) Obeyed-but-losing on the two losses.
Obedience HIGH: guide curve deployed, defender line executed (vs110 Liliana +1 killed Memnite; vs44
Collective Brutality -2/-2 killed the flyers, Liliana of the Veil -2 edicted the Archmage), attacked
when it had bodies (all 3 wins). Both losses attribute to construction (no early blocker / no flyer
answer / threats undrawn). NOT a rewrite trigger.

### (3) Convergence watch — reflexive high-life blocking: NEGATIVE for this deck.
Only ~2 `blockers`-window decisions all corpus (and vs44 seq37 was a fetch re-ask inside a blockers
window, not a real block choice). No reflexive high-life chump. The structural blocker-absence
(Bloodghast can't block, walls undrawn) is the same seam-level construction fact as wave-7.

### (4) Timeout triage.
No genuine timeout in deck133's real games (all 5 ended by state-based win/loss). The one wedge (135)
is the excluded engine livelock, not a durdle. No anti-passivity pressure warranted.

### (5) Executability audit.
`ask`/`attackers` seams well-populated; `blockers` RARE (structural — Bloodghast can't block, walls
draw-dependent), exactly as wave-7. The DEFENDER/wall lines are correct and DID execute when the deck
drew a wall (vs109 Obliterator, vs110 Liliana). Not a guide-writable gap.

---

## FREEZE-CHECK (turnaround-preservation — every frozen line accounted for)
- **Identity + control/counter beats-are-the-win paragraph** (lines 1-9) — vindicated by the 3 wins;
  KEPT VERBATIM.
- **Impact-ranked cast list + GM #1 ELSE clause** — OBEYED in all wins; the ONLY change is
  `N+2 -> N` in rule 1's ELSE (engine display fix). Load-bearing tokens (the ELSE gate, "build
  devotion / drain bigger later") KEPT VERBATIM.
- **ROLE CHECK + DEFENDER line** — OBEYED and WON vs109 (Obliterator wall + GM stabilize); KEPT VERBATIM.
- **VS RED/BURN + go-wide qualification** — untested this wave (no red/goblin opp in the real games);
  KEPT VERBATIM.
- **GRAY MERCHANT block** — the ONLY substantive edit: DELETE the "+2" workaround, replace with "the
  shown N is the final drain, use it directly"; keep the anti-hand-recompute clause; both triggers and
  the ELSE change `N+2 -> N`. Driven by the confirmed engine display fix (vs109 seq36).
- **Bloodghast / Fatal Push / Collective Brutality / Liliana of the Veil / fetch / Yawgmoth / Geralf's
  / Arena / attack / mulligan** — untested or obeyed; KEPT VERBATIM.
- **DECIDING SITUATIONS** — KEPT VERBATIM except three `N+2 -> N` GM threshold edits.
Nothing frozen was dropped. The ONLY change is the mechanical GM-display `N+2 -> N` correction +
deletion of the stale "+2" explanation.

## In my words
deck133's plan: cast a black creature every turn, kill what races you, and finish (or stabilize) with
Gray Merchant's blocker-proof drain — winning by beats when GM doesn't come. The single piloting
problem the guide is built to fix has been the pilot mis-valuing Gray Merchant; this wave the engine
fixed the display, so the guide's job shrinks to "trust the shown drain and fire the two triggers,"
and the residual losses are a decklist that cannot wall early or reach through flyers — a construction
problem, not a piloting one.
