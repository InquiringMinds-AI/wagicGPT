# deck59 (WB Persistence) — Step-0 baseline + INITIAL guide (wave 24)

GUIDELESS Step-0 (Step-0-bis). Corpus: `matchups-20260724-125739` (binary d7ff8b6c4,
carries the new pregame mulligan phase). 6 games, 6 deck59-perspective translogs mined.
Record 3/6 (per brief). Confirmed from gameend records: **WIN vs deck131 "Mind Control"**
(16 vs -5), **WIN vs deck102 "Tergrid Commander"** (22 vs 0), **LOSS vs deck109
"Hellrider"** (-1 vs 14). Three games (deck137 Selesnya Adventures, deck27 Undead
Infiltrator, deck133 Phyrexian Asphodel) have **NO gameend record in EITHER perspective
log** — trajectory: 137 ahead (19v6 T11), 27 behind on clock (11v20 T17), 133 razor
(2v5 T15, but deck59 had lethal on board at its last decision). Scored on decisions, not record.

## The deck (from the engine, NOT Oracle memory)

Mono **W/B** deck — lands are ONLY Plains and Swamp (no Forest), yet every creature
carries a green or hybrid pip. This is the load-bearing fact (Finding 1). Engine costs/stats:

| Card | Cost | Body | Pays with | Note |
|---|---|---|---|---|
| Safehold Elite | {1}{G/W} | 2/2 persist | 1 Plains + 1 any (2) | cheapest body |
| Restless Apparition | {W/B}{W/B}{W/B} | 2/2 persist | 3 any W/B | pump {W/B}{W/B}{W/B}: +3/+3 EOT (finisher) |
| Rendclaw Trow | {2}{B/G} | 2/2 wither, persist | 1 Swamp + 2 any (3) | wither = dmg as -1/-1 counters |
| Kitchen Finks | {1}{G/W}{G/W} | 3/2 persist | **2 Plains** + 1 any (3) | ETB gain 2 life |
| Heartmender | {2}{G/W}{G/W} | 2/2 persist | **2 Plains** + 2 any (4) | upkeep: remove a -1/-1 from EACH of your creatures |
| Lingering Tormentor | {3}{B} | 2/2 fear, persist | 1 Swamp + 3 any (4) | fear = ~unblockable |
| Kithkin Spellduster | {4}{W} | 2/3 flying, persist | 1 Plains + 4 any (5) | 1-of; sac to destroy enchantment |
| Damnation | {2}{B}{B} | sweeper | 2 Swamp + 2 any (4) | destroy all, no regen |
| Day of Judgment | {2}{W}{W} | sweeper | 2 Plains + 2 any (4) | destroy all |
| Wrath of God | {2}{W}{W} | sweeper | 2 Plains + 2 any (4) | destroy all, no regen |

Rule: **{G/W}=pay White (Plains); {B/G}=pay Black (Swamp); {W/B}=either. No green source
exists or is needed.** One trap inside the simplification: Kitchen Finks and Heartmender
need TWO white sources (two Plains) — still count pips.

Win engine: grind a **persist** board that recurs through combat AND through your OWN
sweepers, while the sweepers wipe the opponent clean. Heartmender re-arms it. Attrition
midrange, not aggro.

## Finding 1 (RULE #1) — hybrid/green mana belief = #1 tax + a veto + the losing mulligan

The dominant idiosyncrasy: the deck17/deck21 "belief-that-VETOES" face. The pilot
believes it needs GREEN mana it lacks and re-derives the mana base every decision.

- **deck27 s2** (T1 main, 12,479 chars): "Safehold Elite {1}{g/w}: Needs Green. I have no
  green land ... the only creature I can cast *right now* is Restless Apparition" —
  temporarily VETOED Safehold/Kitchen Finks/Rendclaw before self-correcting "Ah, {g/w}
  means {G} OR {W}." A truncated reply ships the veto.
- **deck109 s5** (land play, ~6,000 chars): opened "CHOICE: 2 (Play Swamp)", spent 6k
  chars re-deriving payability, self-corrected to "CHOICE: 1 (Play Plains)". Parser took
  the LAST CHOICE line correctly — no parser bug.
- Reply-length median 1,220 but **33 giants >4,000 / 16 >6,000**; the largest (13,030 /
  12,607 / 12,479 / 12,336 / 12,158) are hybrid-mana or losing-position spirals. Latency
  p90 ~90s, max 216s — adjudication-loss risk on slow games.

**The mulligan it lost on is the same belief.** deck109 (Hellrider, LOSS): opening 7 = 3
Plains + Safehold + Restless Apparition + Rendclaw Trow + Kitchen Finks. Casts 3 of its 4
spells off Plains alone (only Rendclaw needs black) — a clean keep. The pilot MULLIGANED
it ("4 Plains and 0 Swamps ... zero black sources"), went to 6 vs aggro, got run over.

NOTE: the engine ALREADY annotates cast options `[hybrid: each {g/w} pays with G or W -
total N mana]` — correct, helps AT the cast ask, but ABSENT on land-play/priority
decisions where most spiral lives, so the model re-derives anyway. Fix = a GUIDE fact to
internalize; representation is already good (do not re-diagnose it).

## Finding 2 (RULE #2) — persist + symmetric-sweeper ASYMMETRY: pilot doesn't see it → the LOSS

Deck's point: **your sweeper is one-sided in YOUR favor** — your persist creatures return
(as 1/1s with a -1/-1 counter) while the opponent's stay dead. The pilot treats
Wrath/Day of Judgment/Damnation as generic "slow removal."

- **deck109 (Hellrider) s12 — the loss pivot.** myL **6**; opponent = Stromkirk Noble
  (4/4, growing) + Ash Zealot (2/2 first strike haste), both untap next turn = 6+ lethal.
  deck59 held Day of Judgment + Kitchen Finks + Heartmender with a Safehold Elite out. It
  chose Kitchen Finks, reasoning **"Day of Judgment is too slow and risky."** Correct play
  = Day of Judgment: wipes both threats, deck59's Safehold persists back as a 1/1, opp
  left empty with 1 card — a hard stabilize from 6. It only swept at s20/s22 (**myL 2**,
  both turns) — far too late — and lost T16.
- Contrast **deck131 (Mind Control) WIN.** Racing AHEAD (opp 21→6), OFFERED sweepers at
  s11/s13/s17/s22 and correctly NEVER cast them, deploying persist bodies instead.

Behavior is **right when ahead, wrong when behind.** Missing rule = the WHEN/WHY: sweep to
stabilize when behind on board / being raced, because your creatures come back and theirs
don't.

## Finding 3 (positive) — persist representation is STRONG; two minor gaps

Board surfaces `Kitchen Finks {1}{g/w}{g/w} (2/1) (printed 3/2) [persist] [counters: 1x
-1/-1]` — current P/T, printed P/T, `[persist]`, AND explicit `[counters: 1x -1/-1]`. The
model CAN see a persisted body's reduced stats and that it already carries a counter (so
won't persist again). No representation fix needed. Minor gaps (notes.md):
1. A persist RETURN logs only as generic `Your Safehold Elite: stack -> battlefield` — no
   persist narration, death→return not linked.
2. Block-outcome annotations read "(both die)" for a persist blocker without noting it
   returns — understates chump/trade value for THIS deck.

## Finding 4 — pregame mulligan validation

- 5 of 6 opening 7s were 3-land hands with castable spells → all correctly KEPT.
- The 6th (deck109) = the over-mulligan of Finding 1. Its **bottom** pick was also
  suboptimal: from 4 lands (3 Plains 1 Swamp) + Safehold + Kitchen Finks + Restless, it
  bottomed **Restless Apparition (a threat)** instead of a surplus 4th land (Plains) — 4
  lands/2 spells is worse than 3 lands/3 spells. 1 mild bottom miss.
- Game-start integrity OK: on the draw, drew on first turn (non-starting player turn 1).
  No double-asks / wrong-N. No leyline cards → no 103.6 window. No anomalies beyond the
  known cosmetic.

## Finding 5 — attack/deploy floor intact; cast-nothing clean; no fallbacks

- Attack windows: 24 declared / 25 (the 1 "none" = Hellrider keeping blockers home while
  raced — correct). No durdle-to-empty games.
- 3 cast-nothing-on-own-main: deck133 s35 CORRECT (exactly lethal on board, extra creature
  summoning-sick). deck131 s26 / deck137 s19 = minor durdle while ahead (opp 6). Not a lever.
- **No hard fallbacks at deck59.** One `choice_source: prose` parse (deck131 s18) — benign.

## Ranked teaching points

1. Hybrid/green mana fact + per-card pip table (kills #1 tax, the veto, the mull error) — rule #1, above the engine.
2. Persist + sweeper asymmetry: WHEN to wrath (behind/raced → yes; ahead → never) — rule #2.
3. Mulligan override: keep the all-Plains / low-Swamp 7; do not fear "no black."
4. Attack/deploy floor + Heartmender re-arm synergy (light — largely working already).
