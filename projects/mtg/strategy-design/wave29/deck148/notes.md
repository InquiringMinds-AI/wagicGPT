# deck148 (Kor Army) dev notes - engine / harness / representation / construction (wave 29)

Step-0 seat. Per the loop, Step-0 notes never rotate. Layer-routed items below.

## HARNESS - N-18e truncation safety: FIRST LIVE FIRE, WORKED AS DESIGNED (close, do not re-open)
Record: matchups-20260726-115018 / 1785095144-ai_baka_deck148-0x55e38783d0d0-vs-ai_baka_deck139.jsonl
seq31, kind=blockers, fallback=truncated_abandoned, turn 12, my_life 4.
- The wave-28 N-18e candidate fix ("finish_reason=length AND no terminator after the last coded line
  -> prefer the SAFE phase default (blockers: no blockers), not the stale mid-reply line") shipped as
  f6 and fired here for the first time. The model opened with an ILLEGAL, then SELF-RETRACTED, line
  (`BLOCKS: B1:A1, B1:A2, B1:A3` - one blocker on three attackers), spiraled ~2,500 words, and
  truncated mid-sentence with no `BLOCKS:` terminator. The safety abandoned the stale line and took
  choice 0 / "no blockers" (the safe default). Position was provably lost (1 blocker vs 7 power at 4
  life), so the default cost nothing.
- VERDICT: the safety is validated - it correctly abandoned a self-contradicted, truncated
  commitment. No harness change needed. The RESIDUAL is the MODEL non-termination (below).

## MODEL / SKILL ledger - L-18-1 planning-tax (over-computation in a lost position)
Same record: the model recomputed the identical lethal math ~6 times and reconstructed equipment
attachment state at length in a position it had already proven lost, eating the whole token ceiling.
Second cross-archetype witness (first: deck18 vs93 seq20, wave-28) of the losing-position spiral.
Route: SKILL/CORE ledger (candidate line "when the position is lost or you are far ahead on a forced
choice, commit briefly - do not recompute lethal") - single family, promote on cross-family
agreement, NOT this wave. See general-suggestions.md.

## ENGINE - ORACLE DIVERGENCES (verify-oracle records; the GUIDE uses engine behavior)
Sourced from bin/Res/sets/primitives/mtg.txt + borderline.txt. These are the engine's `auto=`/`text=`
truth, cross-checked against real MTG oracle for the record (per wagicgpt-verify-oracle-text). None
require an engine fix - they are content facts the guide must reflect - but they are logged so the
roster owner can decide whether to align content to paper.
- Armament Master: engine gives OTHER Kor +2/+2 PER equipment attached to Armament Master (steps
  gear=1..>=20). Real oracle = +1/+1 per equipment. Engine is DOUBLED. Also: no gear=0 line, so a
  bare Armament Master is a pure 2/2 with no team buff (correct - matches "for each Equipment
  attached"). Load-bearing for the guide's #2 rule.
- Kitesail Apprentice: engine base 1/1 (power=1 toughness=1), while equipped +1/+1 and flying -> 2/2
  flyer. Real oracle base is 1/2. Minor; guide uses engine 1/1.
- Cranial Plating: HAS both `{1}:equip` and `{B}{B}:name(attach)`. In mono-white only Equip {1} is
  usable. +1/+0 per artifact you control. Matches paper.
- Stone Haven Outfitter: engine implements the +1/+1-to-equipped anthem and the equipped-creature-
  dies-draw trigger, but does NOT implement the paper `{2}{W},{T}: attach target Equipment to target
  creature` ability (only two auto lines). Not a bug for this deck's plan; logged for completeness.
- Bone Saw: cast cost mana={0} (free to play), equip {1}. Correct.
- Argentum Armor: +6/+6, on-attack destroy target permanent, equip {6}, cast {6}. Correct.
- Zamriel, Seraph of Steel: 3/4 flyer, {2}{W}{W}; equipped creatures indestructible on your turn.
  Correct.
- Emeria, the Sky Ruin: functions (returns a creature at upkeep with 7+ Plains). Deck has ~18 Plains,
  so reachable but slow.

## REPRESENTATION - equipment surface is CLEAN (no ledger item)
Equip options bind the attach target + equip cost + card text into the option; board shows
`[attached to: X]` and modified [P/T]; "Artifacts in play: you N | opponent N" line surfaces the
Cranial Plating count. No fall-off / illegal-attach / double-equip defect across six seat logs.
Positive confirmation - the equipment render needs no work (contrast deck146 venture render).

## CONSTRUCTION - decklist flags (route to ROSTER OWNER, NOT guide length)
deck148 is CONSTRUCTION-CEILINGED (low-power aggro that loses close), distinct from deck134's
construction-TERMINAL (clockless control that cannot win to cap). It has a clock and gets there.
- Greedy creature:equipment ratio (~20 creatures + 2 token-makers vs ~17 equipment); equipment-heavy
  draws are dead (vs136: 3 Bone Saws + Sword, ~1 creature, could not function). Much equipment is
  low-impact (Bone Saw x4 +1/+0, Bladed Pinions x2).
- No removal, no reach, no card advantage beyond Outfitter death-draw + slow Emeria. Cannot claw
  back from behind.
- Loses CLOSE (vs139 died at opp 2; vs93 6-1; vs136 12-6). Expectation: the guide's attack-floor +
  gear-lord-first + close-fast teaches should move 1-2 of these close games next corpus; validate
  each teach INDEPENDENTLY of the win column (wave-28 headline 4 method). If teaches validate and the
  record stays flat with residual losses tracing to raw power, THEN route to roster owner (trim weak
  equipment / add a threat or a splash of interaction), not another guide revision.
