# Engine / representation / harness items -- deck139 (mutate) wave-30 FIRST-GUIDED
# Corpus matchups-20260726-181642, binary 6e168aa05. Card facts verified vs primitives + Scryfall.

## N-139h -- NEW, HIGH: ManaCost::remove assertion / CRASH on mutate under a cost discount
Mutating an Apex while Pollywog Symbiote's mutate-cost reduction ({1} less) is active can drive a
mana-cost component negative in ManaCost::remove, tripping a debug assertion and CORE-DUMPING the
process. Repro (game-136v139-1785109073): deck139 seat, Pollywog Symbiote on battlefield, Nethroi
in hand displayed at the discounted cost {1}{g}{b}{w}; on resolving the Nethroi mutate the stderr
ends:
  Resolving Action on stack: StackAbility.
  wagic: src/ManaCost.cpp:973: int ManaCost::remove(ManaCost*): Assertion `cost[i] >= 0' failed.
  timeout: the monitored command dumped core
This is the true cause of the recorded "136 139 adj0 20 18 4" result (a turn-4 "adjudication" at
near-full life = the harness recording the last state before the core dump), so it COST deck139
one of its six games. Scope: fired 1/21 games this corpus, so it is intermittent/state-dependent
-- vs148 mutated Nethroi at the SAME displayed {1}{g}{b}{w} WITHOUT crashing, so the fault is in
the specific cost-removal path for a particular mana configuration, not every discounted mutate.
In a release build (assertions compiled out) this same underflow would silently compute a
wrong/negative cost. Layer: ENGINE (ManaCost::remove / mutate-cost reduction interaction). Repro
lever for the fix: pin a deck139 self-play with Pollywog in play and force a Nethroi (or other
multi-symbol/hybrid mutate) cast; instrument ManaCost::remove near line 973 to log the (cost,
delta) that goes negative. Highest-priority item this seat -- it is a crash AND it silently
corrupts costs in release.

## N-139a/b/c/d -- CLOSED: the wave-29 mutate render batch is validated FIXED (before/after)
Binary 6e168aa05 shipped all four fixes; this seat confirms each with a live render quote:
- N-139b loot->DISCARD verb: was "TARGET CHOICE for Pollywog ... Pick the ONE target it will
  affect" (hand cards, misread as mutate target). NOW "DISCARD ONE OF YOUR OWN CARDS: your own
  effect (Pollywog Symbiote) makes you discard a card ... choose the ONE card to give up"
  (vs148 seq16). Model discarded a spare card correctly, 11,457 ms, clean. Misplay class DEAD.
- N-139a role-naming + CR725: the normal/mutate menu now reads "mutate [cast for the MUTATE cost:
  merge onto one of your non-Human creatures (you pick over/under, then the host) instead of
  casting a fresh separate body]" (vs148 seq15); sub-asks headed "Choose an option for <card>:"
  with a running breadcrumb. Flow followed with no confusion.
- N-139c merged pile -> ONE line: now "<top> (P/T) [keywords] [mutated pile - ONE merged creature
  (top card's name + P/T shown above, plus the combined abilities of every card in the pile);
  1 card underneath: Pollywog Symbiote]" (vs148 seq20+). Was two ambiguous lines. Ambiguity gone.
- N-139d cost label: "alternative cost" now 0 occurrences across all six seat logs; "mutate cost"
  is the sole label. Unified.
Retire all four from the open ledger.

## N-139e -- CLOSED (reconfirmed): Migratory Greathorn land-search is mutate-ONLY, do not "fix"
Primitive (`@mutated` only, no ETB `@movedTo(this|battlefield)`) and Oracle both scope the
basic-land search to "Whenever this creature MUTATES." A hardcast correctly ramps nothing.
Reconfirmed here so no future reviewer re-opens it as a "missing ETB" bug.

## N-139g -- ANSWERED POSITIVE (retire the open probe): under-card statics FUNCTION on the pile
vs148 seq29: Pollywog is the BURIED under card ("1 card underneath: Pollywog Symbiote") and
Nethroi in hand still shows the Pollywog-DISCOUNTED mutate cost {1}{g}{b}{w} -- so the under-card
static (mutate-cost reduction) STILL APPLIES while buried. The engine honors CR 725 (merged
creature has all abilities of all cards in the pile) for UNDER-card statics. Verified, not
asserted. (This is the same discount path that crashes in N-139h -- correct behavior and a latent
underflow bug share the code.)

## N-139-latency -- representation/model: the mutate-render fix did NOT collapse the latency tail
The 195-199k ms tail persists (max 209,562 ms; 4 decisions >195k; 8 >150k; max reply 14,424 chars
= new campaign high, parsed OK). But it MOVED off the mutate render (flow now 11-17k ms) onto
combat / big-board deliberation: vs152 s30 blockers T21 (209,562), vs152 s27 attackers T18
(205,120), vs137 s20 attackers T9 (202,137), vs122 s4 mulligan (200,023), vs137 s19 cast
(181,216), vs148 s29 cast (165,574). This is deck-intrinsic (double-strike/trample/reanimation
math), routed to the GUIDE (strengthened "decide combat fast" with a clock rationale this wave)
and flagged to the core WATCH (general-suggestions.md). Harness-adjacent consequence: on the
3000s wall clock these 200s decisions cause early adjudication; deck139's slowest-seat profile is
disadvantaged by adjudication-behind (pool had 12 adjudications). Not a bug -- a real cost.

## Steady state at this seat
- 0 fallbacks / 0 retries / 0 unparsed / 0 truncation-abandons in 151 decisions.
- g4 tapped-tag reword "[tapped - cannot attack or block this turn]" present on creatures; lands
  correctly bare "[tapped]"; no misread. g4 steady at this seat.
- No parser contradictions.

## ROTATION PROJECTION (wave-31)
deck139 is FIRST-GUIDED, 1/6 this corpus (this was its first GUIDED validation wave). The g3
render arc is CLOSED (validated). BUT this seat is NOT ready to rotate out:
1. N-139h is a NEW crash+cost-corruption bug that PREVENTED a full game (vs136) -- the mutate-
   under-discount surface at scale is therefore NOT fully observed; it must be fixed and then
   revalidated live before the mutate mechanic can be called closed.
2. The guide was REVISED this wave (stopgap retirement) -- a revised guide needs one validation
   corpus before it can be byte-frozen (the pipeline never exits a deck the wave its guide
   changed).
3. The combat-latency tail is the deck's biggest live lever and is unproven under the
   strengthened "decide-fast" teach.
PROJECTION: **STAY in wave-31**, gated on the N-139h fix. Exit gate for wave-32: N-139h fixed and
revalidated (a Pollywog+Nethroi mutate resolves without crash), the revised guide validated and
byte-frozen, and no new mutate-surface issues. This is the normal first-guided -> revalidate ->
exit arc (deck122's shape), extended by one wave for the crash fix.
