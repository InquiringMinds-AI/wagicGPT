# deck139 (Mutants -- mutate, CAMPAIGN-FIRST) -- wave-29 FIRST READ / Step-0 (2/6)

Corpus matchups-20260726-115018, binary 50eeed8be, -T 3000, REPPENALTY=1.05.
Seat record 2/6: **W** vs146 (adj1), **W** vs148; **L** vs136 (adj0, died at 1 life),
vs122, vs93 (-18 turn 12), vs137 (0 life turn 10). Fallbacks at this seat: **0**.
Truncation-abandon at this seat: **0** (the corpus's single f6 fire is deck148's, not mine
-- see notes N-139-f6). All card facts below verified against primitives + Scryfall.

## VERDICT (headline)

**Mutate is MECHANICALLY CORRECT in the engine and the MODEL UNDERSTANDS the merged object
-- the whole surface is dragged down by RENDER gaps, not by rules bugs or model ignorance.**
The engine merges a mutate pile into ONE creature with the top card's P/T (CR 725) and
correct creature-count; combat A-lines render the pile as a single legal actor and the model
attacks/blocks with it correctly. Every card-fact checked (Gemrazer, Migratory Greathorn,
Pollywog, all five Apexes) is FAITHFUL to Oracle. What leaks is a scrambled multi-ask mutate
FLOW, a mislabeled loot-discard, and a battlefield render that splits the pile into two lines
and hides the merged creature's combined abilities -- and these render gaps drive a brutal
rules-lawyering LATENCY TAX (six decisions at 195,000-199,000 ms this seat, one reply at
14,181 chars, one char short of the truncation edge deck148 fell off).

## (1) HOW MUTATE CASTS RENDER

The mutate cast is a MULTI-ASK sequence. Two variants observed:

**With a mutate-host on board, no Pollywog** (deck146 T7, Gemrazer over Dryad):
- SEQ15 cast menu: `Cast Gemrazer {3}{g} (4/4)` / `Cast Gemrazer with its mutate cost {1}{g}{g} (4/4)` / `Cast nothing` -> chose mutate cost (correct).
- SEQ16 `Cast Card Normally / mutate / Decline` -> mutate.
- SEQ17 `mutate over / mutate under` -> over.
- SEQ18 TARGET CHOICE (battlefield non-Human creatures: Grazer#1, Grazer#2, Dryad) -> Dryad.
Order: cast -> over/under -> TARGET. The target is chosen LAST, AFTER over/under.

**With Pollywog on board** (deck93 T9, Migratory Greathorn over Pollywog):
- SEQ18 cast menu -> SEQ19 `Cast Card Normally / mutate` -> mutate
- SEQ20 **"TARGET CHOICE for Pollywog Symbiote ... Pick the ONE target it will affect"**,
  options are HAND cards (Nethroi, Grazer, Brokkos) + Island. This is Pollywog's
  draw-then-DISCARD (`_LOOT_`) mis-rendered as a mutate/target choice. The model MISREAD it
  as the mutate target ("requires a target creature you control to mutate") and discarded
  Island by accident.
- SEQ21 `mutate over / mutate under` -> over
- SEQ22 TARGET CHOICE for Migratory Greathorn (battlefield: Grazer#1, Pollywog, Grazer#2) -> Pollywog
- SEQ23 land-search fetch (Migratory Greathorn's mutate trigger) -> Forest.

**Cost label is INCONSISTENT (render bug N-139d):** Gemrazer's option reads "with its
**mutate cost** {1}{g}{g}"; Migratory Greathorn's reads "with its **alternative cost**
{1}{g}" (deck122 s16, deck93 s18). Same mechanic (`other={..} name(Mutate)` in both
primitives), two different labels. When the label says "alternative cost" the model does not
recognize the mutate mechanic up front (deck122 s17 the model got stuck on mana math instead
of seeing the cheap mutate line).

## (2) DOES THE MODEL UNDERSTAND THE MERGED OBJECT? -- YES

- deck136 s22 (life 1) the model states CR 725 correctly, unprompted: "put it over or under
  target non-Human creature ... They mutate into the creature on top plus all abilities from
  under it. If Gemrazer (4/4) on top of Snapdax (3/5): the creature becomes a 4/4 (Gemrazer's
  stats) with Snapdax's abilities (Double Strike) AND Gemrazer's abilities (Reach, Trample)."
  Top-card P/T + combined abilities -- exactly right.
- deck146: after Gemrazer mutated over Dryad, the merged `Gemrazer (4/4) [trample,reach,mutate]`
  is offered as ONE attacker (A3) and the model attacks with it five turns running (s19,20,22,
  24,27) and wins the game. It correctly waits out summoning sickness (attacks next turn).
- deck148 s25 the model blocks with a merged `Illuna, Apex of Wishes (6/6) [trample,flying,
  mutate]` as one creature -- correct.
- over/under: the model chose **over** every time (deck93 s21, deck146 s17) and reasons that
  over keeps the Apex's P/T + all abilities on top -- the correct default for this deck.
- **which host to mutate onto:** decent ability-value reasoning -- chose Dryad (keeps ramp/
  fixing, deck146 s18) and Pollywog (keeps cost-reduction+loot, deck93 s22) over the vanilla
  0/3 Grazers. Good.

## (3) ENGINE MUTATE CORRECTNESS (observable) -- CORRECT

- Merged P/T = TOP card's P/T (CR 725): Gemrazer-over-Dryad renders 4/4 (Gemrazer's), Dryad
  under renders with NO P/T. Correct.
- Creature COUNT treats the pile as ONE: deck146 board goes "creatures: 4" (pre-merge) ->
  "creatures: 3" (post-merge) and stays 1 object through combat/death. Correct.
- Mutate triggers fire per mutate: Migratory Greathorn's land-search fired (deck93 s23).
- Card facts verified faithful to Scryfall: Gemrazer {3}{G} 4/4 mutate {1}{G}{G} reach/trample
  + destroy-artifact/ench-on-mutate; Migratory Greathorn {3}{G} 3/4 mutate {2}{G}, land-search
  **on mutate only** (Oracle says "Whenever this creature mutates" -- NOT "enters or mutates";
  the primitive's mutate-only scripting is CORRECT, not a bug -- flagged so no future reviewer
  re-opens it); Pollywog {1}{U} 1/3 cost-reducer + draw-then-discard on mutate cast.
- Open (not observable this corpus): whether an UNDER card's static abilities actually FUNCTION
  on the merged creature (e.g. did buried Dryad still grant the extra land drop?). Route as an
  engine-verify item (N-139g), not asserted.

## (4) STEP-0 READ + 2/6 DECOMPOSITION

Deck139 is a Simic-splash **ramp/mutate midrange** deck: ramp with Arboreal Grazer (ETB puts
a land from hand), Dryad of the Ilysian Grove (extra land + omni-fixing), Migratory Greathorn
(mutate = fetch a basic), Beanstalk // Fertile Footsteps (adventure ramp), Nissa; then land a
cheap non-Human host and **mutate the Apex bombs onto it to fire game-swinging triggers**
(Snapdax 4 dmg + gain 4; Illuna free permanent; Nethroi mass-reanimate; Vadrok recast a spell;
Nethroi/Brokkos big trampling bodies).

**What it does well:** understands the merged object and combat with it; picks "over" and a
value host correctly; ramps; hardcasts a body when it needs blockers at low life (deck136 s22
chose normal at 1 life -- defensively correct: two bodies block two attackers, a mutate adds
no body).

**Where it leaks:**
- **APEX TRIGGERS ARE MUTATE-ONLY, and the model does not treat them as the payoff.** Every
  Apex's value line is "Whenever this creature MUTATES" -- a HARDCAST fires NOTHING. The deck
  wins by keeping a cheap host and mutating Apexes onto it for removal/reanimation/card
  advantage. The model has no framing for this and treats mutate as a generic "make one
  creature bigger" option. This is the #1 strategy teach.
- **When to mutate vs cast a fresh body (tempo/blocker math) is inconsistent.** deck93 s19 at
  9 life vs three 4/4 Rats it MUTATED Migratory Greathorn onto Pollywog (adds NO body) when a
  normal cast (a fresh 3/4 blocker) was the survival play; it then got run over (-18, turn 12).
  Rule: under aggro when you need blockers, HARDCAST a body; save mutate for value/when stable.
- **Slow to stabilize vs aggro.** All four losses are to faster decks (93 Rats, 137, 136, and
  122 out-values); the top-heavy Apex curve arrives too late without ramp. Both WINS came where
  it landed a mutate threat and raced (vs146 merged Gemrazer; vs148 aggro mirror won at 2 life).
- **RULES-LAWYERING LATENCY TAX** (see below) -- the single largest observable cost at this
  seat, concentrated entirely on mutate/combat decisions.

## (5) LATENCY / STEADY STATE

- 155 decisions, **0 fallbacks, 0 retries, 0 truncation-abandons** at this seat -- parser clean.
- Latency: median 17,325 ms, mean 33,751 ms, **max 198,711 ms**; >10k = 151/155 (97%),
  >14k = 115/155 (74%). Reply chars: median 1,090, **max 14,181**.
- The six ~195-199k ms decisions are ALL mutate-flow / mutate-combat rules-lawyering:
  deck136 s22 (1-life Gemrazer normal-vs-mutate, 198,711 ms / 13,894 chars),
  deck93 s20 (loot-discard confusion, 194,660 ms / **14,181 chars**),
  deck93 s26/s27, deck146 s25/s27 (double-strike/trample combat math on merged creatures).
- deck93 s20's 14,181-char reply is one class short of the truncation edge that made
  deck148's s31 (12,676 chars) fire the f6 no-blocks safety. The mutate render gaps are
  actively pushing THIS seat toward the same cliff. Render fixes (correct discard framing,
  a merged-creature combined-ability line) are the lever that shrinks both the tax and the
  truncation risk -- they remove the ambiguity the model spends three minutes resolving.

## Ledger items raised (detail in notes.md)
- N-139a mutate multi-ask order (over/under before target).
- N-139b loot / draw-then-discard mis-rendered as "TARGET CHOICE ... target it will affect"
  (hand cards) -- MISSED in deck93, decoded in deck122. Repro pair.
- N-139c mutate pile split into two battlefield lines; merged combined-abilities not surfaced.
- N-139d cost-label inconsistency ("mutate cost" vs "alternative cost").
- N-139g (verify-open) under-card static abilities functioning on the merged creature.
- N-139-f6 (attribution) the corpus's single truncated_abandoned is deck148 s31, not deck139.
