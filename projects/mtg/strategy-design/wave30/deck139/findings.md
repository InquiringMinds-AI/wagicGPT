# deck139 (Mutants -- mutate) -- wave-30 FIRST-GUIDED review (1/6, DOWN from 2/6)

Corpus matchups-20260726-181642, binary 6e168aa05, -T 3000, REPPENALTY=1.05.
Seat record 1/6: **W** vs148 (clean, opp dead 0 life T14); **L** vs136, vs122, vs146, vs152,
vs137. Fallbacks at this seat: **0 / 151 decisions** (parser clean, 2nd corpus running).
All card/render facts below are quoted from the deck139-seat translogs of THIS corpus.

## VERDICT (headline)

**The g3 mutate-render batch is VALIDATED WORKING -- all four wave-29 gaps (N-139a/b/c/d) are
closed at their motivating seat -- AND the guide did NOT mislead. The 2/6 -> 1/6 drop is
(one ENGINE CRASH counted as a loss) + (pool/variance in the grindiest pool yet). One NEW
high-severity finding: mutating an Apex while Pollywog's mutate-cost discount is active CRASHED
the process (ManaCost assertion), which is what the vs136 "turn-4 adjudication" actually was.**

The render fix did its job: the mutate FLOW is now fast and unambiguous (flow decisions in
vs148/vs137 ran 11-24k ms, no misreads). BUT the 195-199k ms latency tail did NOT collapse --
it MOVED off the mutate render onto combat math / big-board deliberation, where it is
deck-intrinsic (top-heavy double-strike/trample/reanimation Apexes invite deep rules-lawyering).

## (1) g3 VERIFICATION -- the mutate render batch (all four gaps CLOSED)

### N-139b loot -> DISCARD verb label: FIXED (the cross-seat misplay class is DEAD)
- BEFORE (wave-29, deck93 s20): "TARGET CHOICE for Pollywog Symbiote (this spell/ability is
  already on the stack and needs a target ...). Pick the ONE target it will affect ..." listing
  HAND cards, NO discard verb. Model MISREAD it as the mutate target, discarded Island by
  accident. 194,660 ms / 14,181-char reply.
- AFTER (wave-30, deck139 vs148 seq16): "DISCARD ONE OF YOUR OWN CARDS: your own effect
  (Pollywog Symbiote) makes you discard a card ... choose the ONE card to give up from the list
  below, and answer with the chosen card's name." Model correctly discarded a spare Beanstalk
  Giant (held two), latency 11,457 ms, clean, no fallback. The loot no longer misreads as a
  target. **Class dead.**

### N-139a mutate step role-naming + CR-725 context: FIXED
- BEFORE (wave-29): bare "mutate over / mutate under" and a bare "Cast Card Normally / mutate"
  menu with no explanation of what mutate does.
- AFTER (vs148 seq15): the normal/mutate menu now reads "1. Cast Card Normally  2. mutate [cast
  for the MUTATE cost: merge onto one of your non-Human creatures (you pick over/under, then the
  host) instead of casting a fresh separate body]". The step is self-describing; the over/under
  and mutate-target sub-asks are headed "Choose an option for <card>:" with the running
  breadcrumb ("-> mutate [...]"). The model followed the whole flow (seq15 mutate -> seq16
  discard -> seq17 over -> seq18 host) with no confusion.

### N-139c merged pile -> ONE line: FIXED
- BEFORE (wave-29): pile rendered as TWO adjacent battlefield lines (top card w/ P/T, under card
  a bare name+cost, no P/T, no tag) -- ambiguous "one creature or two?".
- AFTER (vs148 seq20+): "Everquill Phoenix {1}{u} (4/4) [flying, mutate] [mutated pile - ONE
  merged creature (top card's name + P/T shown above, plus the combined abilities of every card
  in the pile); 1 card underneath: Pollywog Symbiote]" -- ONE line, explicitly labeled ONE
  creature, names the under card, states the combined-ability rule. **Ambiguity gone.**

### N-139d cost label unified: FIXED
- "alternative cost" = **0 occurrences** across all six deck139-seat logs; "mutate cost" is now
  the sole label (18-44 hits/game). The wave-29 "mutate cost vs alternative cost" split is gone.

## (2) GUIDE VALIDATION under the record drop -- NO teach misled

Harsh audit: every loss checked for a guide-steered wrong decision. None found.

- **vs146 (the W->L flip; last wave W-adj, this wave clean L, died -5 T16):** every cast was
  guide-CONSISTENT. seq26 hardcast Migratory Greathorn at board-creatures=0 (FORCED -- no legal
  host; model reasoned "no other creatures on my battlefield to mutate onto"). seq28 hardcast
  Snapdax at board-creatures=0 (FORCED -- the Greathorn had died blocking a deathtouch Adventurer
  at seq27). seq31/32 hardcast Greathorn at 1 life, board-creatures=1 (this is the #2-rule play:
  under aggro you need the fresh 3/4 BODY, not a bodiless mutate; the only mutate available was
  Greathorn-onto-Snapdax = land fetch, no blocker, at 1 life). The loss is the known wave-29 leak
  -- top-heavy Apex curve slow to stabilize vs aggro, host died early -- and is a close-margin
  variance flip in a harder pool, NOT a misteach.
- **vs137 (L, adj0 T10):** the mutate teach FIRED CORRECTLY. seq21 chose "Cast Illuna with its
  mutate cost" (MUTATE, not hardcast), seq23 "mutate over", seq24 host = Pollywog (keeps
  discount), seq25 resolved Illuna's dig trigger ("put in hand"). Textbook Apex-mutate line.
  Lost to GW convoke tempo, not to a bad decision. Adjudicated at the wall clock (latency tail --
  see (4)).
- **vs122 (L, died -4 T13):** seq17 hardcast Greathorn at board-creatures=0, life 4 (FORCED, and
  already losing). vs122 went 6/6 (first-guided PERFECTA) and out-valued the whole pool.
- **vs152 (L, died -1 T21):** 0 mutate choices, 0 hardcast-with-host -- the deck simply never
  assembled host+Apex in a 21-turn grind. No misplay; matchup/variance.
- **vs136 (recorded L adj0 T4):** NOT A REAL GAME -- see (3). Engine crash.

Teaches validated: **Apex-mutate-only** (vs137 Illuna, vs148 Everquill+Nethroi all mutated);
**fresh-body-under-aggro** (vs146 seq32 hardcast at 1 life -- correct). Guide is sound.

## (3) NEW HIGH-SEVERITY: mutate + Pollywog discount = ManaCost crash (this is the vs136 "loss")

The vs136 result ("adj0 20 18 4" -- a bizarre turn-4 adjudication at near-full life) is an ENGINE
CRASH, not an adjudication. game-136v139-1785109073.stderr ends:

    AIPlayerGPT: targeting with -> Nethroi, Apex of Death (5/5) ... "Mutate {4}{GW}{B}{B} ..."
    Action added to stack: StackAbility.
    Resolving Action on stack: StackAbility.
    wagic: src/ManaCost.cpp:973: int ManaCost::remove(ManaCost*): Assertion `cost[i] >= 0' failed.
    timeout: the monitored command dumped core

Pre-crash board (deck139-seat seq13): **Pollywog Symbiote in play** (mutate-cost reducer),
Nethroi in hand shown at the DISCOUNTED cost {1}{g}{b}{w}. Mutating Nethroi while Pollywog's
{1} discount is active drove a mana-cost component negative in ManaCost::remove, tripping the
cost[i] >= 0 assertion and core-dumping. Scope: the assertion fired in only this game this
corpus (1/21), so it is intermittent/state-dependent (a color-bucket underflow that only asserts
for a particular mana configuration). Note vs148 mutated Nethroi at the SAME displayed
{1}{g}{b}{w} and did NOT crash -- so the bug is in the specific cost-removal path, not every
discounted mutate. In a release build (assertions off) this underflow would silently compute a
wrong/negative cost rather than crash. **Routed to notes.md as N-139h (HIGH).** This crash cost
deck139 a game and is the single largest real event in the drop.

## (4) LATENCY -- tail did NOT collapse; it MOVED to combat math (and is throttling games)

- deck139-seat: 151 decisions, median 17,421 ms / mean 35,240 ms / **max 209,562 ms**;
  4 decisions >195k, 8 >150k; max reply 14,424 chars (new campaign high; parsed, no fallback).
  Wave-29 was median 17,325 / mean 33,751 / max 198,711 / 6 decisions 195-199k / max 14,181 chars
  -- essentially UNCHANGED (slightly worse max).
- WHERE the tail lives now: the mutate FLOW is fast (vs148 seq15-19 = 11-17k). The top latencies
  are all COMBAT / big-board deliberation: vs152 s30 blockers T21 (209,562 ms, 14,424 chars),
  vs152 s27 attackers T18 (205,120), vs137 s20 attackers T9 (202,137), vs122 s4 mulligan
  (200,023), vs137 s19 cast T9 (181,216), vs148 s29 cast-Nethroi T14 (165,574). The render fix
  removed the mutate-PERCEPTION latency; the residual tail is deck-intrinsic combat rules-
  lawyering on double-strike/trample/reanimation Apexes -- exactly what the guide's "DECIDE
  COMBAT FAST" section already targets, and the model is not heeding it enough.
- **The tail is COSTING GAMES via wall-clock adjudication.** deck139's 200s decisions eat the
  3000s cap fast: two losses are cap/crash artifacts (vs136 crash; vs137 adj0 T10 after three
  150-202k combat decisions), and the pool's 12 adjudications skew against the slowest seat.
  This is the deck's biggest remaining lever and it is NOT a render problem.

## (5) N-139g PROBE ANSWERED -- POSITIVE: under-card statics FUNCTION on the merged creature

vs148 seq29: Everquill Phoenix renders "[mutated pile ... 1 card underneath: Pollywog Symbiote]"
(Pollywog BURIED as the under card) AND Nethroi in hand shows the Pollywog-DISCOUNTED mutate cost
{1}{g}{b}{w}. So Pollywog's static mutate-cost reduction STILL APPLIES while Pollywog is the
under card -- direct evidence the engine honors CR 725 ("the merged creature has all abilities of
all cards in the pile") for UNDER-card static abilities. The wave-29 open probe is answered
POSITIVE. (The same discount-applies-while-buried code path is what underflowed in the vs136
crash -- N-139g's good behavior and N-139h's bug are two faces of one path.)

## (6) STEADY STATE (g4 tapped-tag; parser)

- g4 reworded tag "[tapped - cannot attack or block this turn]" present on creatures across all
  games (1-52/game); lands correctly stay bare "[tapped]". No tapped-tag misread at this seat.
- 0 fallbacks / 0 unparsed / 0 retries / 0 truncation-abandons in 151 decisions. The CHOICE-number
  contract parsed the full multi-ask mutate flow (cost/normal-mutate/discard/over-under/host/
  trigger) with no drift. The 14,424-char blockers reply (vs152 s30) parsed cleanly -- the f6
  truncation safety was NOT needed at this seat.
- No parser contradictions observed.

## Ledger items raised (detail in notes.md)
- **N-139h (NEW, HIGH):** ManaCost::remove cost[i] >= 0 assertion / crash when mutating an Apex
  under Pollywog's cost discount. Repro: deck139 self-play, Pollywog in play, mutate Nethroi.
- N-139a/b/c/d: CLOSED (validated fixed this corpus).
- N-139e: CLOSED (Greathorn mutate-only ramp -- confirmed still correct, not re-opened).
- N-139g: ANSWERED POSITIVE (under-card statics function) -- retire as an open probe.
- Combat latency tail (deck-intrinsic; guide + possible core reasoning-length watch) -- see
  general-suggestions.md.
