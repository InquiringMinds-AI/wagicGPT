# deck116 "Time of Need" (GU cheat-into-play) — wave-32 STEP-0 GUIDELESS DEBUT

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260727-115847`, binary `7fabd9bd0`, 6 games.
Record **1/6** (the pool floor). Seat decisions: 106 consumed records across 6 logs.
**Fallbacks at this seat: 0 / 106.** Zero defers, zero unparsed, zero `validation_reject_reask`,
zero `truncated_abandoned`. Zero crash signatures.

Results (results.tsv, cols deck0 deck1 winner life0 life1 turns):

| opp | result | end life (me/them) | turn | creatures I deployed |
|---|---|---|---|---|
| 139 | **WIN** | 18 / -10 | 13 | Piper x2, Kozilek, Thragtusk, Finks x2 |
| 158 | loss | -3 / 19 | 8 | **0 permanents of any kind** |
| 152 | loss | -2 / 38 | 12 | Sheoldred (removed) |
| 105 | loss (poison) | **23** / 20 | 12 | Piper x2 (both chump-blocked to death) |
| 146 | loss | -2 / 21 | 8 | **0 creatures** |
| 36  | loss | -8 / 20 | 8 | **0 creatures** |

**Three of six games ended with ZERO creatures ever deployed.** That single line is the seat.

---

## (a) CHEAT-INTO-PLAY AUDIT — the surface is CLEAN. 5 offered / 5 taken / 5 ARRIVED.

Both enablers verified against `Res/sets/primitives/mtg.txt`:
- Elvish Piper `mana={3}{G}` 1/1, `auto={G}{T}:moveTo(myBattlefield) target(creature|myhand)`
- Quicksilver Amulet `mana={4}` artifact, `auto={4}{T}:moveTo(myBattlefield) target(creature|myhand)`

Every window in the corpus that offered a Put-in-Play activation, and its arrival trace:

| seat/seq | turn | kind/phase | offer | taken | ARRIVAL (next record `events`) |
|---|---|---|---|---|---|
| vs139 s12 | t9 | priority / Upkeep | 4 targets (Thragtusk, Kozilek, Piper, Finks) | Kozilek | s13 `Your Kozilek, Butcher of Truth: hand -> battlefield` OK |
| vs139 s16 | t11 | priority / Upkeep | 2 targets | Thragtusk | s17 `hand -> battlefield` + `Your life +5 (now 14)` OK ETB fired |
| vs139 s20 | t13 | priority / Upkeep | 2 targets | Kitchen Finks | s21 `hand -> battlefield` + `life +2 (now 16)` OK |
| vs139 s23 | t13 | priority / **Main phase 1** | 1 target (Amulet) | Kitchen Finks #2 | s24 `hand -> battlefield` + `life +2 (now 18)` OK |
| vs152 s10 | t9 | priority / Upkeep | 1 target (Amulet) | Sheoldred | s11 `Your Sheoldred, Whispering One: hand -> battlefield` OK |

**5/5 arrival-traced. ZERO silent no-ops on the activation class.** Downstream effects also fire:

- **Annihilator fires on a CHEATED-IN body** — vs139 s19 t12, `You declared attackers: Kozilek`
  then four `Opponent's <permanent>: battlefield -> graveyard` lines (2 Forests, Temple Garden,
  Arboreal Grazer) then `Damage: 12 dealt by Kozilek`. Correct: `auto=_ATTACKING_...sacrifice`
  is an attack trigger, not a cast trigger.
- **Cast triggers correctly DO NOT fire** — Kozilek's `autostack=if casted(this) then draw:4`
  produced no draw at s13 (events show only `hand -> battlefield`, then the normal Draw step's
  one card). Engine is RIGHT; **the pilot's belief is WRONG** (s12 reply: *"As soon as he enters,
  his 'When you cast' trigger resolves, drawing four cards"*). Guide teach, not a bug.
- Sheoldred's opponent-upkeep sacrifice trigger fired (vs152 t11 `Opponent's Wolf: battlefield ->
  graveyard` at their upkeep) before she was removed by Fateful Absence.

**No missing-offer instances.** Every window where the precondition held (untapped, unsick enabler
+ a creature in hand + payable) produced an offer. The reason only 5 windows exist in 6 games is
NOT an engine gap — it is that the enabler reached the battlefield in only 3 of 6 games, and in
vs105 both Pipers were chump-blocked to death before they could untap (see (c)).

**Take rate 5/5 (100%).** No guide teach is needed on *taking* the offer. The guide's job is
entirely upstream: get the enabler down, and keep it alive.

Interface facts worth recording: the activation rides the **priority** menu (kind=`priority`),
not the main-phase cast list — five main-phase `ask` windows (vs139 s13/s14/s17/s21/s22) had a
live Piper on the battlefield and creatures in hand and did **not** carry a Put-in-Play line.
Priority windows do arm in Main phase 1 (vs139 s23), so it is a menu split, not a phase lock.
Minor: the offer list includes strictly-dominated targets — `Put in Play with Elvish Piper
targeting Elvish Piper` (vs139 s12), spending {G}+tap for a 1/1 that is also hard-castable.

---

## (b) TUTOR / SEARCH RENDERS — the WAVE-31 ELIGIBLE-COUNT RULE IS VIOLATED, 14/14.

**Mechanically clean, representationally broken.** 14 reveal decisions across 6 games. Every one
enumerates the **whole library** (44-51 options), each line carrying full card text.

| card | eligible set in this deck | options rendered |
|---|---|---|
| Fabricate `{2}{U}` `target(<1>artifact\|reveal)` | 4 (Quicksilver Amulet x4) | **49, 51, 47, 51, 49** |
| Time of Need `{1}{G}` `target(<1>creature[legendary]\|reveal)` | 5 legends | **45, 45, 49, 46** |
| Farseek `{1}{G}` `target(plains,island,swamp,mountain\|reveal)` | 7 (3 Island + 4 Tropical Island) | **51, 44, 51, 48, 45** |

The render is a *labelled* whole-library list, not a raw one — it carries an ELIGIBILITY header
(*"only a legendary creature may go to 'choose card' ... Pick ONLY from the cards marked
[eligible]"*) and per-line `[does NOT qualify - goes to "shuffle"]` tags. So this is the N-136a
`revealzone(mylibrary)` path with the wave-30 g2 filter still absent, mitigated by labelling.
The wave-31 rule is *eligible-count or no-ask*; 45 lines for 5 eligible cards is neither.

**Cost is measured, not hypothetical:** the corpus's single largest reply is a Farseek reveal —
vs36 s10, **13,796 chars / 220,606 ms** on a 48-option list to pick a land. Three of the top six
tax rows are reveals.

**Two distinct framings, driven by the `<N>` token — and the un-tokened one over-selects.**

- `target(<1>...)` (Time of Need, Fabricate) renders **"Choose the ONE card"** -> pilot answers a
  single index. 9/9 correct picks (Amulet x6, Emrakul x2, Kozilek, Ulamog).
- `target(plains,island,...)` (Farseek, **no `<1>`**) renders **"Decide, in ONE reply, which of them
  go to 'choose card'"** -> subset framing on a pick-EXACTLY-ONE effect -> **over-selection in 5/5
  Farseeks** (`PUT: 14, 23, 25, 26, 29, 36` — six cards; vs105 s5 and vs36 s10 picked seven).
  This is the deck102 pick-cardinality rung reproduced at a second seat, on a *library* tutor.
  **Outcome-neutral here**: the engine finalizes one (vs152 s5 -> `Your Tropical Island: zone ->
  battlefield`, exactly one land). Latent quality bug — the pilot does not choose *which*, and
  here every over-selected card was interchangeable (Island vs Tropical Island). Not so if a
  future deck's eligible set is heterogeneous.

**Fetched card arrival-traced to hand: 14/14** (12 verifiable; 2 were the last record before
gameend). Each next record shows `You revealed N and put <card> to choose card` and the card in
the following `Your hand:` line — e.g. vs36 s8 -> `Kozilek, Butcher of Truth {10} (12/12)
[creature]` in hand at s9. **The tutor seam does its job. The defect is render-lane only.**

---

## (c) THE 1/6 DECOMPOSITION — this is a PLAY floor, NOT a construction floor.

Per the skill's construction taxonomy the honest verdict is: **NEITHER construction-terminal NOR
construction-ceilinged.** The deck's engine demonstrably closes games — vs139 it killed from 14
to -10 in three turns off one cheated-in Kozilek. The losses are not close races lost on raw
power; they are games in which **the deck never did the thing it is built to do.**

Decomposed per loss:

**vs158 — 100% MULLIGAN DECISION. Never played a land.** Seven mulligans, then a forced keep and
`PUT: 1,2,3,4,5,6,7` — a zero-card hand. The trigger is a FALSE BELIEF over a CORRECT render:

- s1: hand = `Glimmerpost (land); Tropical Island (land: taps for {U}{G}); Island (land: taps for
  {U}); Farseek; Farseek; Time of Need; Kitchen Finks` — **three lands and two Farseeks.** Reply:
  *"This hand has zero lands and only one source of mana."* Mulliganed.
- s7: hand = `Island (land: taps for {U}); ... Forest (land: taps for {G}); ...` — **two lands, one
  of them green.** Reply: *"This hand has zero lands."* Mulliganed.

This is the deck139 "zero lands with lands in hand" class at a new seat. The land-identity
annotation (`(land: taps for {G})`) is present and correct on every line, so it is a STRATEGY /
belief failure, not a representation gap -> guide prose (perception-vs-strategy routing).
One contributing render nit: **Glimmerpost renders as bare `(land)`** with no taps-for clause
(its `auto={T}:Add{1}` is colourless), and the pilot's s1 reply singles it out as *"only one
source of mana ... produces only one mana"* — see notes N-116c.

**vs36 — 100% DEPLOY FLOOR + a false hardcast plan.** Kitchen Finks `{1}{g/w}{g/w}` (3/2) was
OFFERED and DECLINED at s7 (t5, 20-20), s12 (t7, **7**-20) and s14 (t7, 7-20). It cast Time of
Need twice instead, fetching **Kozilek {10}** and **Ulamog {11}** into a hand with no Piper and no
Amulet, on six lands, at 7 life. The plan is stated verbatim at s14: *"Farseek ... accelerating my
mana base significantly towards casting Ulamog next turn."* The deck cannot hardcast these — 24
lands, one extra land per Explore/Urban Evolution, no rituals. Zero creatures deployed; dead t8.
(s14 also asserts *"Farseek ... 2) It draws a card"* — false; that is Explore.)

**vs146 — 100% DEPLOY FLOOR, strict-dominance version.** Cast Fabricate at s7 fetching Quicksilver
Amulet; at s10 the option list read `['Cast Quicksilver Amulet {4}', 'Cast Fabricate {2}{u}',
'Cast Farseek {1}{g}', 'Cast nothing right now']` with the Amulet **already in hand and payable
(4 sources)** — and it cast Fabricate to fetch a **second** Amulet. Zero creatures; dead t8.

**vs105 — PAYOFF-PERMANENT DESTRUCTION, twice, then loss by poison at 23 LIFE.** The blocker
render is explicit and correct: `B1. Elvish Piper (1/1) - may block A1 (both die), A2 (your
blocker dies, attacker lives)`. The pilot chose **A2** — the branch the render names as a pure
loss — at s8 (t6) and again at s11 (t8), killing both copies of the deck's cheapest enabler
against an infect deck. Both replies frame it as poison prevention. It then held Emrakul, Pelakka
Wurm and Jin-Gitaxias in hand with an Amulet cast on the last turn, and lost with more life than
its opponent. **Positive control same seat**: vs139 s11, offered only `(your blocker dies,
attacker lives)` blocks with the Piper, it correctly answered `no blockers`. The discriminator is
INFECT — the pilot trades the engine to stop poison it cannot see counted (see notes N-116b).

**vs152 — mixed: enabler landed, payoff removed, then flood.** Amulet -> Sheoldred (arrived,
trigger fired, killed by Fateful Absence). Afterwards the pilot spent t11 on Time of Need ->
Emrakul, Farseek, Explore and a land at 3 life with `Your library: 0 cards` and no creatures.
~50% opponent power, ~50% a no-outs turn spent on tutors.

**Aggregate deploy-floor instrument** (wave-31 HL9, cast-nothing split): **24 cast windows;
6 declined an offered BODY or ENABLER for a search/ramp spell.** Five of the six are in losses:
vs105 s15 (Amulet declined for Time of Need), vs146 s10 (Amulet declined for Fabricate), vs36
s7/s12/s14 (Kitchen Finks declined x3). The sixth (vs139 s22, `Cast nothing` at 16-2 with a full
board) is harmless.

**Was the payoff ever in range? YES — this is NOT the deck102 absent-engine-card shape.** The
1-of Eldrazi were reached repeatedly: Kozilek in hand vs139 (opening) and vs36 s9; Ulamog vs36
s14; Emrakul vs152 s16 and vs105 s17; Sheoldred vs152 s11. Time of Need is a 3-of tutor into five
legends and hit on 4/4 casts. **The engine assembles fine.** What failed was ORDER — the payoff
was fetched before the enabler existed, and the enabler was traded away in combat.

**Verdict: the guide is load-bearing and gets a full first pass.** No construction routing.
The one construction observation worth flagging (notes N-116e, roster lane, low priority) is that
Sheoldred `{5}{B}{B}` is HARD-uncastable in a deck with no black source — she is a cheat-only card
by construction, which is arguably intentional for this shell but should be stated as such.

---

## (d) STEP-1 BATCH STEADY STATE AT THIS SEAT

- **(i8) combat tags — CLEAN.** `[tapped - cannot attack or block this turn]` x105, `, but CAN
  block` x77, `[tapped - attacking]` x9. **Zero lines carrying both `[tapped - attacking]` and
  `cannot attack`** (grepped every prompt line). Zero paradox verbalizations, zero "no untapped
  creatures to block" misreads in any reply.
- **(i11) creature markers on hand lines — CLEAN.** `[creature]` x151; every mulligan reply
  reasoned about creatures correctly; **zero "zero creatures" mulligans**. (This seat's mulligan
  failure is the LAND axis, not the creature axis.)
- **(i13) broad card text on Cast lines — FIRING, and load-bearing here.** Every `Cast` option
  carries `{card text: "..."}`, including the two enablers' full activation text, and the Put-in-Play
  option lines carry it too. This is what let the pilot take 5/5 activations with no guide. It is
  also the seat's main prompt-length cost driver alongside the reveal lists.
- **(i7) London bottoming — VALIDATED, 3/3.** vs139 1 mull -> header *"you took 1 mulligan and
  kept, so you must put 1 card ... on the BOTTOM"*, 7 labels, 1 bottomed. vs146 identical. vs158 7
  mulls -> *"Your ENTIRE hand goes to the bottom"*, 7 labels, 7 bottomed. Labels constant, count
  exact. **The 0-card hand is rules-correct London, not the N-139i clamp bug.**
- **(i12) count-first mana line — present** (`Mana available: 4 total (four untapped sources,
  tapped automatically when you cast; colours you can make: {g}{u}{c})`) and **zero "I cannot
  afford" declines of an offered payable cast.** But it is still being re-derived: vs105 s14
  spends 9,775 chars re-counting lands and lands on "5 sources" while the line says 4.
- **(i5/i6) — UNEXERCISED at this seat.** Zero re-ask markers, zero truncations, zero fallbacks
  in 106 decisions. Nothing to report either way.
- **(i1) castable-but-not-offered audit** (Amulet / Piper / Kitchen Finks in every loss): **zero
  instances.** Every payable signature card appeared in its window's offer list. No N-152c shape.
- **(i4, i10, i14, i15) — not exercised** (no lifelink grants, no equipment, no amass, no DFC).

**Tax table, low-stakes rows first (wave-31 HL9):** the three biggest LOW-BRANCHING spikes are
2-option land drops — vs105 s14 (9,775 ch / 145s), vs105 s9 (8,794 ch / 149s), vs146 s9 (8,248 ch
/ 111s). Reading s14: the entire budget is mana arithmetic and enabler-vs-tutor sequencing. This
is the same belief that lost vs36 and vs146, outnumbering the decisive instances — the guide's #2
rule is aimed exactly here.

---

## CARD-FACT AUDIT (wave-31 HL1, done at draft time for the initial guide)

Every number in `strategy.txt` was read off `Res/sets/primitives/mtg.txt` this session, not from
memory: Elvish Piper `{3}{G}` 1/1 act `{G}{T}`; Quicksilver Amulet `{4}` act `{4}{T}`; Time of
Need `{1}{G}`; Fabricate `{2}{U}`; Farseek `{1}{G}` (`plains,island,swamp,mountain` — a plain
Forest does NOT qualify, Tropical Island does via `subtype=Forest Island`); Explore `{1}{G}`
(extra land + draw 1); Urban Evolution `{3}{G}{U}` (draw 3 + extra land); Kitchen Finks
`{1}{GW}{GW}` 3/2 persist, ETB gain 2; Thragtusk `{4}{G}` 5/3, ETB gain 5, LTB 3/3 Beast token;
Pelakka Wurm `{4}{G}{G}{G}` 7/7 trample, ETB gain 7, dies->draw 1; Jin-Gitaxias `{8}{U}{U}` 5/4
flash, end-step draw 7; Sheoldred `{5}{B}{B}` 6/6 swampwalk; Kozilek `{10}` 12/12 annihilator 4
(+`autostack` draw 4 **on cast only**); Ulamog `{11}` 10/10 indestructible annihilator 4
(+`autostack` destroy **on cast only**); Emrakul `{15}` 15/15 flying, protection from coloured
spells, annihilator 6 (+`autostack` extra turn **on cast only**); Glimmerpost Locus, ETB gain 1
per Locus, `{T}: Add {1}`; Hinterland Harbor enters tapped unless you control a Forest or Island.

**PERSISTENCE check (HL2):** no card the guide names as a body self-bounces or self-sacrifices.
Kitchen Finks *persists* (returns as a 2/1 with a -1/-1 counter) — stated positively in the guide.
Elvish Piper is named as a body the guide forbids ever putting in combat, for the opposite reason.

---

## PRESERVE LEDGER (wave-31 HL8 rule 0) — what the pilot already does right, guideless

Frozen territory the initial guide must not legislate over:

1. **Put-in-Play take rate 5/5 (100%)**, correct target every time (biggest available threat).
2. **Tutor picks 9/9 correct** on the `<1>` reveals — never picked an ineligible card despite
   45-option lists; picked Emrakul/Kozilek/Ulamog over Jin-Gitaxias/Sheoldred sensibly.
3. **Attacker declarations 2/2 full** (vs139 s18, s24) — no `ATTACK: none`, correctly swung the
   annihilator body into a 6-creature board.
4. **Blocks 3/4 correct**, including the hard one (vs139 s11 declined a losing Piper block; s15
   blocked with Kozilek `(you kill it, your blocker lives)`; s19 blocked with Thragtusk). The two
   wrong ones are BOTH the same Piper-vs-infect instance.
5. **Zero fallbacks in 106 decisions**; zero illegal or out-of-range answers.

So per HL8: no loud attack floor, no combat procedure. The guide is written for the residue —
sequencing, deploy floor, the hardcast delusion, the Piper preservation, and the mulligan.
