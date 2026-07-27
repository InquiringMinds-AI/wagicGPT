# deck139 (Temur/Sultai Mutants -- mutate) -- wave-31 review (1/6, flat vs wave-30)

Corpus `matchups-20260727-025026`, binary `cf7cb2e53`, -T 3000, REPPENALTY=1.05.
Seat record **1/6**: **W** vs146 (adj1, 8 v 4 life, T14); **L** vs137 (adj0, 11 v 16, T16),
vs148 (dead -4, T9), vs158 (dead 0 v 3, T11), vs152 (dead -2, T13), vs122 (dead -7 v 37, T13).
Fallbacks at this seat: **0 / 154 decisions** (3rd consecutive clean corpus).
Crash signatures in the six deck139-seat stderr files: **ZERO**.
All card facts verified against `Res/sets/primitives/borderline.txt`; all renders quoted from the
translog `prompt` field.

---

## VERDICT (headline)

**N-139h is CRASH-FREE but its motivating path is UNEXERCISED, so it is NOT revalidated.** Every
mutate that was offered AND chosen this corpus ARRIVED (4/4, arrival-traced to the `[mutated pile]`
render). But **not one of those four was a Pollywog-discounted cast**: Pollywog Symbiote reached the
battlefield in only 1 of 6 games, the single discounted mutate it enabled was DECLINED, and Pollywog
died to a block two turns later. Per the wave-30 UNEXERCISED-SURFACE rung, the honest verdict is
that the crash fix removed the crash from the corpus but the corpus never re-drove the crashing
code. **Missing trigger condition: a mutate spell actually CAST while Pollywog Symbiote is on the
battlefield.** Only a controlled probe closes this.

**Second headline: the 1/6 is NOT construction-only and NOT variance. The dominant, measurable
cause is a MULLIGAN SPIRAL -- 2.50 mulligans/game at this seat vs 0.00-1.33 everywhere else in the
pool** (table in section 3). Mean kept-hand size 5.0 cards against a field mean of ~6.4. Three of
the shipped hands satisfied the guide's own keep criterion verbatim. One game was mulliganed to a
3-card hand. That is a guide + executor failure, not a power-level failure -- though a genuine
construction ceiling exists underneath it (3b) and is separately demonstrable from the mana base.

---

## (1) THE MUTATE ARRIVAL TABLE (h1 -- the centerpiece)

Every offered variant (mutate AND normal-cast) of every mutant, all six games. "Arrived" = the
`[mutated pile - ONE merged creature ...]` line appears on a later battlefield render.

| # | game | seq/turn | card | variants OFFERED | chosen | paid | over/under | host | ARRIVED? |
|---|------|----------|------|------------------|--------|------|-----------|------|----------|
| 1 | vs148 | seq11 / T2 | Migratory Greathorn | mutate `{2}{g}` only (hardcast unaffordable) | **mutate** | yes | seq12 OVER | auto (only host: Arboreal Grazer) | **YES** -- seq13 `Migratory Greathorn {g} (3/4) [reach, mutate] [mutated pile ... 1 card underneath: Arboreal Grazer]`; trigger FIRED (fetched Mountain) |
| 2 | vs158 | seq14 / T4 | Migratory Greathorn | mutate `{2}{g}` only | **mutate** | yes | seq15 OVER | auto | **YES** -- seq16 pile line; trigger FIRED (fetched Mountain) |
| 3 | vs152 | seq9 / T2 | Migratory Greathorn | mutate `{2}{g}` only | **mutate** | yes | seq10 OVER | auto | **YES** -- seq11 pile line; trigger FIRED (fetched Forest) |
| 4 | vs146 | seq21 / T9 | Gemrazer | hardcast `{3}{g}` + mutate `{1}{g}{g}` | **mutate** | yes | seq23 OVER | seq24 Arboreal Grazer #1 | **YES** -- seq25 `Gemrazer {g} (4/4) [trample, reach, mutate] [mutated pile ... 1 card underneath: Arboreal Grazer]`; trigger had no legal target |
| 5 | vs146 | seq17 / T7 | Gemrazer, **Pollywog-DISCOUNTED** | hardcast `{2}{g}` + mutate `{g}{g}` (both `{1}` off; Pollywog in play) | **declined** (cast Dryad instead) | -- | -- | -- | **n/a -- the ONE discounted mutate of the corpus, not taken** |
| 6 | vs158 | seq12 / T4 | Migratory Greathorn | mutate `{2}{g}` only | **declined** (203,770 ms) | -- | -- | -- | n/a -- retaken at seq14 same turn (row 2) |
| 7 | vs158 | seq26 / T10 | Migratory Greathorn | hardcast `{3}{g}` + mutate `{2}{g}` | **declined** | -- | -- | -- | n/a (see 4c) |
| 8 | vs158 | seq29 / T10 | Migratory Greathorn | mutate `{2}{g}` only | **declined** (224,282 ms) | -- | -- | -- | n/a |
| 9 | vs152 | seq19 / T8 | Gemrazer | hardcast `{3}{g}` + mutate `{1}{g}{g}` | **hardcast** (no host alive) | yes | -- | -- | n/a -- correct, no legal host |
| 10 | vs152 | seq26 / T12 | Illuna, Apex of Wishes | hardcast `{2}{g}{u}{r}` + **Everquill Phoenix** mutate `{3}{r}` | **Illuna hardcast** | yes | -- | -- | **NO -- and the model believed it was mutating** (2a) |
| 11 | vs146 | seq27 / T11 | Snapdax, Apex of the Hunt | hardcast `{1}{r}{b}{w}` only (mutate `{2}{BR}{W}{W}` unaffordable -- 1 white source) | **hardcast** | yes | -- | -- | **NO -- and the model believed it was mutating** (2a) |

**Offered-and-chosen mutates: 4/4 ARRIVED. Zero silent no-ops, zero defers, zero fallbacks, zero
crashes.** The engine-lane mutate cast path is healthy at every step the corpus exercised: cost
selection -> normal/mutate confirmation -> over/under -> host target -> merge -> mutate trigger.
Three of the four fired their mutate trigger (Greathorn's basic-land fetch, 3/3); the fourth
(Gemrazer) had no legal target, verified against the primitive
`auto=@mutated(this):name(Destroy artifact or enchantment) target(*[artifact;enchantment]|opponentBattlefield) destroy`
and an opponent board with no artifact or enchantment at vs146 seq25.

**But the N-139h path itself is untouched.** Pollywog's primitive is
`auto=lord(creature[mutate]|mycastingzone) altercost(colorless,-1)` -- the generic-pip decrement that
underflowed `ManaCost::remove` in wave-30. It was live on the board only in vs146 seq10-19; the one
mutate it discounted (row 5) was declined at seq17, and Pollywog died at seq19 (blocked a
first-strike deathtouch Triumphant Adventurer). By seq21 the Gemrazer mutate rendered at its full
`{1}{g}{g}`, i.e. the discount was gone -- **row 4 is a FULL-COST mutate and cannot validate the
discounted path.** Wave-30's crashing case (Nethroi `{4}{GW}{B}{B}` discounted to `{3}{gw}{b}{b}`)
has no analogue in this corpus at all.

**Also unexercised: Pollywog's loot.** `auto=@movedTo(creature[mutate]|mystack):_LOOT_` never fired --
`DISCARD ONE OF YOUR OWN CARDS` appears **0 times** in all six seat logs, because no mutate spell was
ever cast while Pollywog lived. The wave-30 g3 verb-labeled-loot fix therefore has **no second
witness** this corpus. Reported as a null, not as "clean".

---

## (2) GUIDE VALIDATION -- the wave-30 revised guide's first corpus (b)

### (2a) FAILED: the Apex-mutate-only teach is defeated by a hardcast/mutate LINE MISREAD

The guide's #1 rule and its "It is always labeled 'mutate cost'" clause did NOT prevent the pilot
from picking the plain hardcast line while its own reasoning described a mutate. Twice, on the only
two big-Apex casts of the corpus:

- **vs146 seq27 (T11)** -- options were exactly `1. Cast Snapdax, Apex of the Hunt {1}{r}{b}{w} (3/5)`
  / `2. Cast nothing right now`. The model chose 1 and wrote: *"Snapdax is the perfect tool here:
  casting it for its mutate cost onto my existing Gemrazer ... will trigger its ability to deal 4
  damage to a target creature or planeswalker and gain 4 life ... PLAN: Cast Snapdax for its mutate
  cost targeting Gemrazer, choosing to mutate OVER"*. No mutate line existed. Primitive:
  `mana={1}{R}{W}{B}` (the line it took) and `other={2}{BR}{W}{W} name(Mutate)`; the seat controlled
  one white source (Plains), so the offer was CORRECT and the misread is the pilot's.
  Result: a 3/5 double-striker, no 4 damage, no 4 life.
- **vs152 seq26 (T12, at 5 life)** -- options `1. Cast Illuna, Apex of Wishes {2}{g}{u}{r} (6/6)` /
  `2. Cast Everquill Phoenix with its mutate cost {3}{r} (4/4)` / `3. Cast nothing`. The model chose
  1 and wrote *"Hardcasting Illuna is a waste; I must mutate it onto one of my Arboreal Grazers to
  trigger the 'exile top of library until a nonland permanent' effect ... PLAN: Cast Illuna, Apex of
  Wishes for its mutate cost targeting one of the Arboreal Grazers."* Primitive `mana={2}{G}{U}{R}`
  = exactly the line it took; `other={3}{RG}{U}{U} name(Mutate)` was unaffordable off 5 sources.

This is a distinct failure from wave-29's render ambiguity: the render is now unambiguous and the
pilot still cannot tell the two lines apart when only one is present. The guide currently states the
label as a *description of the render*; it needs it as a **verification step the pilot performs on
the option it is about to take**, plus the missing else-branch ("no mutate line = the mutate is not
available this turn"). Both hardcasts were arguably fine plays on their boards; the defect is that
the pilot **cannot report which spell it cast**, which makes every downstream plan wrong.

### (2b) FAILED: the MULLIGAN teach

`MULLIGAN: keep 2-5 lands with at least green, plus an early ramp creature ... A Forest is a land -
never call a hand with lands "no lands."` Three shipped hands satisfied this verbatim:

- **vs122 seq1**: `Plains; Stomping Ground; Temple Garden; Pollywog Symbiote; Dryad of the Ilysian
  Grove; Dryad of the Ilysian Grove; Pollywog Symbiote` -- 3 lands, two of them green, plus Dryad
  (named ramp) and Pollywog (cheap host). Model: *"This hand has zero lands to cast spells with,
  despite having four creatures and three dual lands."* **MULLIGANED.**
- **vs122 seq3**: `Snapdax; Temple Garden; Dryad; Snapdax; Everquill Phoenix; Breeding Pool; Sacred
  Foundry` -- 3 lands, two green, plus Dryad. Model: *"This hand has zero lands and five high-cost
  cards (... and three dual lands that require paying life to untap)."* **MULLIGANED.**
- **vs122 seq6**: `Forest; Overgrown Tomb; Breeding Pool; Migratory Greathorn; Gemrazer; Nissa;
  Illuna` -- 3 green lands including an actual **Forest**, plus Greathorn (named ramp). Model:
  *"This hand has zero lands that can produce Green mana immediately (no Forests ...)"*. **MULLIGANED.**

Two systematic misreads drive it, and neither is a render defect -- the hand render already tags
every land, e.g. `Stomping Ground (land: taps for {R}{G})`:
1. **Dual/shock lands are not counted as lands** ("zero lands ... despite having three dual lands"),
   and `taps for {R}{G}` is not read as a green source. All five of the deck's green lands
   (Breeding Pool, Overgrown Tomb, Stomping Ground, Temple Garden, Forest) were miscounted at least
   once.
2. **Lands in hand are treated as non-mana** -- vs148 seq2: *"zero untapped mana sources (all lands
   are in hand, none are on the battlefield to tap), meaning I cannot cast any spells on Turn 1"*.

The existing one-line stopgap ("A Forest is a land") fires on the wrong exemplar: the failures are
overwhelmingly on the DUALS, and once (vs122 seq6) on a literal Forest. There is also **no floor**:
the pilot's own PLAN lines pre-authorise the spiral (*"If the 5-card hand is still unplayable,
mulligan again to 4"*), and vs122 rode that to seven mulligans and a 3-card keep. **Revised in
strategy.txt -- this is the corpus's single largest guide-lane lever.**

### (2c) FAILED: attacking with 0-power creatures (a seam the guide never floors)

vs137, four consecutive attack declarations sending 0/3 Arboreal Grazers at the opponent:
seq16 T5 (`ATTACK: A1, A2`), seq18 T7, seq19 T9, seq21 T11 -- each dealing zero damage. The pilot's
own reasoning shows both an anti-passivity prior and an arithmetic error:
- seq21: *"Attacking with all three 0/3 Grazers deals 3 damage, bringing the opponent from 16 to 13
  life."* (Primitive: `power=0`.)
- seq19 (208,327 ms): *"Sending all three Arboreal Grazers (0/3) deals 0 damage ... Wait, Arboreal
  Grazer is 0/3. It has 0 power. Attacking with 0 power creatur"* -- the self-correction arrives
  AFTER the answer-first `ATTACK: A1, A2, A3` line and cannot retract it (the known commit-lock
  ledger item; second witness at this seat).
The attackers menu carries no power annotation, unlike the blockers menu which does
(`[deals 0 - this block kills nothing, it only absorbs damage]`) -- routed to notes.md as a render
proposal (N-139j). The guide side gets a per-seam attack floor with the 0-power exception named
inside it.

### (2d) VALIDATED: fresh-body-under-aggro (#2 rule)

- **vs152 seq19 (T8, 14 life)**: hardcast Gemrazer over its mutate -- *"I have no legal host creature
  on the battlefield (both Arboreal Grazers are in the graveyard) ... having a 4/4 attacker is
  crucial."* Correct and correctly reasoned; the mutate line was offered and correctly refused.
- **vs152 seq26 (T12, 5 life)**: took the 6/6 body over Everquill's 4/4 mutate. Board-correct under
  #2 even though the pilot mislabeled it (2a).
- **vs146 seq17 (T7)**: took Dryad (host + fixer) over both Gemrazer lines, reasoning explicitly
  *"I cannot mutate it yet because I have no other non-Human creatures ... Dryad sets up the board
  state to immediately mutate Gemrazer onto her next turn."* Guide-perfect sequencing.

### (2e) VALIDATED: stopgap retirement (the wave-30 render re-alignment held byte-for-byte)

Every render string the wave-30 guide describes matches the live corpus exactly:
- cast menu: `mutate [cast for the MUTATE cost: merge onto one of your non-Human creatures (you pick
  over/under, then the host) instead of casting a fresh separate body]` (vs146 seq22, vs152 seq20).
- over/under step: `mutate over` / `mutate under` under `Choose an option for <card>:` (vs148 seq12,
  vs158 seq15, vs152 seq10, vs146 seq23).
- host step: `TARGET CHOICE for Gemrazer ... -> Arboreal Grazer #1 (0/3) [reach] [your battlefield]`
  (vs146 seq24) -- the guide's "listing creatures on YOUR BATTLEFIELD" is accurate.
- merged pile: `[mutated pile - ONE merged creature (top card's name + P/T shown above, plus the
  combined abilities of every card in the pile); 1 card underneath: Arboreal Grazer]` -- ONE line,
  every occurrence, all four merges. The guide's paraphrase is exact.
No stale stopgap prose remains. **Retirement CLOSED.**

One flow detail the guide does not cover and now should: when both cost variants are offered, the
pilot is asked TWICE -- once to pick the cost line (`Cast Gemrazer with its mutate cost {1}{g}{g}`)
and again to confirm (`1. Cast Card Normally / 2. mutate [...] / 3. Decline`). Observed twice
(vs146 seq21->22 mutate->mutate; vs152 seq19->20 hardcast->normal). Both were internally consistent
this corpus, but a pilot that picks the mutate cost and then answers "Cast Card Normally" would
silently hardcast. Added as a one-line flow note.

---

## (3) THE 1/6, DECOMPOSED HONESTLY (c)

### (3a) Mulligan spiral -- the dominant term, and it is fixable at the guide layer

| deck | mulligans / 6 games | per game | wave-31 record |
|---|---|---|---|
| **deck139** | **15** | **2.50** | **1/6** |
| deck152 | 8 | 1.33 | 3/6 |
| deck146 | 5 | 0.83 | 2/6 |
| deck137 | 4 | 0.67 | 3/6 |
| deck122 | 3 | 0.50 | 3/6 |
| deck148 | 2 | 0.33 | 4/6 |
| deck158 | 0 | 0.00 | 5/6 |

Per-game kept hand at this seat: vs137 **6**, vs146 **7**, vs148 **4**, vs158 **4**, vs152 **6**,
vs122 **3** -- mean **5.0** against a pool mean of ~6.4. The rank order of mulligan rate is
essentially the inverse of the win table. Three of the four death-losses were games started at 4 or
fewer cards (vs148 T9 and vs158 T11, kept 4 each) or 3 (vs122 T13). This is not variance: the same
pilot, same sampler, same corpus, mulliganed 2.5x the field, on hands its own guide told it to keep.

### (3b) A REAL construction ceiling sits underneath it -- the Apex mutate costs are near-uncastable

Colour sources in the 24-land base (counted from deck139.txt): green 13 (Breeding Pool 2, Forest 3,
Overgrown Tomb 2, Stomping Ground 3, Temple Garden 3), red 8, white 7 (Plains 2, Sacred Foundry 2,
Temple Garden 3), blue 5 (Breeding Pool 2, Island 2, Steam Vents 1), black 4 (Overgrown Tomb 2,
Swamp 2). Against the primitives:

| Apex | mutate cost (primitive `other=`) | hard requirement | sources |
|---|---|---|---|
| Snapdax | `{2}{BR}{W}{W}` | **WW** | 7 |
| Illuna | `{3}{RG}{U}{U}` | **UU** | 5 |
| Nethroi | `{4}{GW}{B}{B}` | **BB** | 4 |
| Vadrok | `{1}{WU}{R}{R}` | **RR** | 8 |
| Brokkos | `{2}{UB}{G}{G}` | GG | 13 |
| Everquill Phoenix | `{3}{R}` | R | 8 |
| Gemrazer | `{1}{G}{G}` | GG | 13 |
| Migratory Greathorn | `{2}{G}` | G | 13 |

The guide's #1 rule ("almost always mutate an Apex") is aimed at five cards whose mutate costs demand
a double pip off 4-7 sources in a five-colour 24-land deck. Empirically: **all four mutates of the
corpus were the two mono-green mutators** (Greathorn 3x, Gemrazer 1x); Snapdax's and Illuna's mutate
lines were never once offered as legal options in six games. Dryad and Nissa are the fixers and the
deck runs 2 of each. **This is a genuine construction ceiling and should be stated to the roster
owner as such** -- but it is the SECOND-order term, not the first: a deck averaging 5.0 opening cards
would lose with a perfect mana base.

### (3c) Variance / matchup -- the residual, and it is small
- vs137 (adj0 loss, 11 v 16): mana-light 6-card keep, drew three Arboreal Grazers and little else;
  no mutant was castable in 16 turns. Genuine draw variance, aggravated by (2c).
- vs158 (lost 0 v 3 at T11): decided by a single render misread, not by power -- see (4c).
- vs122: a 3-card keep; not a game.

**Bottom line: construction-ceilinged YES, but not construction-ceilinged ONLY.** deck148's wave-30
exit criterion ("teaches validate, residual losses trace to raw power") is NOT met here -- two
teaches failed and one seam is unfloored. This seat still has guide work.

---

## (4) STEADY STATE AND THE OTHER FOCUS ITEMS

### (4a) Mutate render batch (wave-30 g3) -- steady state (d)
- **role-named steps**: HELD. `Choose an option for <card>:` with the running breadcrumb on every
  over/under and normal/mutate ask; the self-describing mutate line quoted verbatim in (2e).
- **merged pile as ONE line**: HELD, 4/4 merges, ~40 renders across four games, no two-line pile
  anywhere. `alternative cost` = **0 occurrences** in all six seat logs (N-139d stays closed).
- **verb-labeled loots**: **UNEXERCISED** -- 0 occurrences of `DISCARD ONE OF YOUR OWN CARDS`.
  Missing trigger: a mutate spell cast while Pollywog Symbiote is on the battlefield.
- One cosmetic drift: after merging, the pile's cost renders as a single pip, e.g.
  `Gemrazer {g} (4/4)` (vs146 seq25) where the pre-merge line read `Gemrazer {3}{g} (4/4)`, and
  `Migratory Greathorn {g} (3/4)` in three games. No decision was affected. notes.md N-139l (LOW).

### (4b) g4 tapped-tag -- held; but the SUMMONING-SICK tag is now the misread
`[tapped - cannot attack or block this turn]` on creatures and bare `[tapped]` on lands, read
correctly throughout. The adjacent tag `[summoning sick - cannot attack this turn]` says nothing
about blocking, and the pilot inferred it cannot block (4c). Same fix shape as g4. -> notes.md N-139k.

### (4c) The vs158 loss traces to one tag misread (0 v 3 at T11)
At seq26 (T10, 7 life v 6) the opponent board rendered
`Orc army (3/3) ... [tapped - cannot attack or block this turn]; ... Sauron, the Lidless Eye {3}{r}{b}
(4/4) [summoning sick - cannot attack this turn]`. The pilot wrote *"The opponent has no untapped
creatures to block (their Orc army and Sauron are tapped ...), meaning my attackers will deal 6
damage directly to the opponent, reducing their life to 0 and winning the game immediately"* and on
that basis DECLINED to cast Migratory Greathorn (both variants offered, mana sufficient). Sauron was
untapped and blocked; Sacred Foundry died; the opponent survived at 3 and killed deck139 the
following turn. The declined fresh body at seq26 and seq29 is therefore **downstream of the render
misread, not a failure of the #2 rule** -- the pilot believed it had already won.

### (4d) A block that cost the whole Pollywog arc
vs146 seq19 (120,280 ms): the pilot blocked `Triumphant Adventurer (1/1) [first strike, deathtouch]`
with Pollywog Symbiote, having been told by the render `B1. Pollywog Symbiote (1/3) - may block A1
(your blocker dies, attacker lives)`. Pollywog died for nothing, taking the deck's mutate discount --
and with it the corpus's only chance to exercise N-139h -- off the board. The guide names Pollywog as
a card to keep around but gives the blocker seam no rule about it.

### (4e) Combat latency (e) -- NOT improved; and the tail has MOVED again
154 timed decisions: median **16,867 ms**, mean **32,254**, p90 **77,085**, **max 224,282**.
Wave-30 was median 17,421 / mean 35,240 / max 209,562. Decisions >195 s: **6** (wave-30: 4;
wave-29: 6). Top of the tail:

| ms | game | seq | kind | turn | reply chars |
|---|---|---|---|---|---|
| 224,282 | vs158 | 29 | ask (cast) | 10 | 12,878 |
| 219,913 | vs158 | 30 | priority | 10 | 12,283 |
| 208,327 | vs137 | 19 | attackers | 9 | 13,206 |
| 203,770 | vs158 | 12 | ask (cast) | 4 | 12,241 |
| 200,461 | vs122 | 11 | bottom (mulligan) | 0 | 12,497 |
| 195,651 | vs152 | 20 | ask (normal/mutate) | 8 | 13,337 |

**Wave-30's read ("the tail lives in combat math") no longer holds.** Only 1 of the 6 is a combat
seam; three are casting asks and one is mulligan bottoming. Every one of the six produced a
12.2-13.3 k-char reply, i.e. the tail is a **reply-LENGTH phenomenon at the token cap**, seam-agnostic
-- all six replies show the same oscillating self-argument ("Wait, let me re-read...") that the
answer-first protocol has already locked out of effect. The guide's "DECIDE COMBAT FAST" section is
correctly motivated but mis-scoped to combat; retargeted to all seams in strategy.txt. The
core-prompt side (a reasoning-length brake) is raised in general-suggestions.md -- third consecutive
corpus at this seat with a ~200 s tail, second where it cost tempo on a wall-clock-capped harness.

### (4f) Parser
0 fallbacks / 154 decisions; 0 unparsed, 0 retracted, 0 truncation-abandons, 0 retries. The
multi-ask mutate flow (cost -> normal/mutate -> over/under -> host -> trigger target) parsed cleanly
every time. **One parser/prompt contradiction**, verbatim, at the London bottoming seam -- notes.md
N-139i: the prompt says *"you kept after 6 mulligans, so you must put EXACTLY 6 cards from your hand
on the BOTTOM of your library"* when the player kept after **7**, and the engine then bottoms exactly
**one** card and re-asks. Net effect at vs122: 4 cards bottomed where CR 103.5 required 7.

---

## (5) ROTATION -- where each wave-32 exit-gate condition stands

The gate: *N-139h fixed AND revalidated + revised guide validated + frozen + combat-latency teach
proven.*

| gate condition | status | what is missing |
|---|---|---|
| **N-139h fixed** | **MET** -- 0 crash signatures in 6/6 seat stderr; 4/4 offered-and-chosen mutates arrived; 0 silent no-ops | -- |
| **N-139h REVALIDATED** | **NOT MET -- UNEXERCISED** | a mutate CAST while Pollywog is on the battlefield. 1/6 games had Pollywog in play; its single discounted offer was declined; it died 2 turns later. Recommend a **controlled probe** (scripted board: Pollywog + a host + Nethroi/Snapdax in hand) rather than another rotation slot -- the corpus cannot be relied on to produce the state |
| **revised guide validated** | **PARTIAL** -- stopgap retirement CLOSED, fresh-body-under-aggro VALIDATED; mulligan teach FAILED, Apex-mutate-only teach DEFEATED by a line misread, attacker seam unfloored | the wave-31 revision (mulligan floor + green-source naming + mutate-line verification + attack floor) needs its own corpus |
| **guide frozen** | **NOT MET -- freeze DECLINED** | strategy.txt is REVISED this wave, not byte-copied |
| **combat-latency teach proven** | **NOT MET** | tail worsened (max 209 s -> 224 s; >195 s count 4 -> 6) and RELOCATED off combat onto casting/mulligan asks. Teach retargeted; needs re-measurement |

**Recommendation: HOLD deck139 for wave-32.** Three of five conditions are open and two of them
(mulligan floor, mutate-line verification) are freshly-authored teaches that have never been tested.
The seat is NOT new-work-exhausted. Pair the hold with the N-139h probe so the crash validation does
not depend on drawing Pollywog again.

Construction hand-off note for the roster owner (independent of rotation): the five double-pip Apex
mutates are unsupported by this mana base (3b). Either cut to the green/red mutators
(Gemrazer / Greathorn / Everquill / Brokkos) or add fixing; a third and fourth Dryad would do more
than any guide sentence.

---

## Ledger

- **N-139h** -- crash fix HOLDS (0 signatures); **motivating path UNEXERCISED**, probe requested.
- **N-139g** -- under-card statics: no new evidence (Pollywog never survived under a pile). Stays
  answered-positive from wave-30, not re-tested.
- **N-139j (NEW, HIGH)** -- attacker options lack the blocker menu's power annotation; 0-power
  creatures sent to attack 4x. notes.md.
- **N-139k (NEW, HIGH)** -- `[summoning sick - cannot attack this turn]` read as "cannot block";
  decided the vs158 game. notes.md.
- **N-139i (NEW, MED)** -- London bottoming: prompt mis-states the mulligan count and demands N cards
  while the engine consumes 1; vs122 bottomed 4 of a required 7 (CR 103.5). notes.md.
- **N-139l (NEW, LOW)** -- merged-pile cost pip render drift (`{3}{g}` -> `{g}`). notes.md.
- Commit-lock (existing ledger item) -- second witness at this seat, vs137 seq19.
