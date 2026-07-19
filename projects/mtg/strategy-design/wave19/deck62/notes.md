# deck62 notes -- engine/harness items + rotation verdict (wave 19)

Seat: deck62 (Enchantresses), THIRD guided corpus (revised-guide round 3). 182 decisions (ask 136,
attackers 27, blockers 19) across 6 games. Record 2/6 (wins vs135 adj + vs27; losses vs35, vs14 adj,
vs49, vs110). Fallbacks 7 (all unparsed_reply): vs35 s10, vs135 s14, vs135 s32, vs14 s21, vs14 s40,
vs110 s1, vs27 s27 = 3.8% (up from wave-18's 5/182 = 2.7%, but an ENTIRELY DIFFERENT class -- see
below). Giant replies (>8k) 8 -> 13, driven by decode-loops + ahead-can't-close, NOT behind-spiral.

## Carryover checks (the brief's two explicit asks)
- **Blanchwood recount-ban: HELD DEAD into round 3.** No recount-of-already-printed-P/T. The only
  Forest-counting (vs14 s40, vs49 s23) is PROSPECTIVE cast-value math (Blanchwood not yet attached),
  legitimate and unbannable -- same verdict as wave-18 N3. The retired watch stays retired.
- **A-list-only attacks: DISOBEDIENCE PERSISTS, but the ENGINE now absorbs it (cost = 0).** The model
  still over-names absent (summoning-sick) creatures under must-attack pressure (vs14 s8, vs35 s33),
  but the wave-19 combat name-tolerance drops them and attacks with the eligible subset -> zero
  unparses (was 2 last wave). This is the durable close of the class deck62 filed as N2.

## Engine / harness / representation items (NOT guide prose)

- **N2 (harness -- filter named-but-ineligible attackers) -- CLOSED, shipped + validated.** Filed
  wave-18; shipped as the wave-19 name-tolerance batch. Confirmed at deck62 with 3 saves (vs14 s8,
  vs35 s33 attacks; vs27 s23 NAME:NAME block). Retire the item; keep it as a cross-wave validation
  record (see skill.md #1). The wave-18 guide A-list sharpen is now redundant with the engine but
  kept frozen (belt-and-suspenders).

- **N3 (computed-P/T distrust) -- stays RETIRED.** The recount-ban held round 3; no re-open.

- **N6 (representation, NEW, model/decode watch -- DECODE-REPETITION-LOOP unparse).** vs35 s10, vs135
  s14, vs14 s21, vs110 s1: a short phrase repeated verbatim to truncation, no advancing reasoning.
  This is the deck135 REPEAT-LOOP signature at a SECOND seat, now the dominant unparse driver (4 of
  7). NOT guide-addressable and NOT token-budget-fixable -- a decode pathology. Route to the
  model/decode watch. Severity note: 3 of the 4 landed in games deck62 lost/was-losing anyway
  (matchup floors); vs110 s1 was a mulligan (heuristic kept a keepable hand). None flipped a winnable
  game, but the class is real and cross-seat.

- **N7 (harness/parser, NEW, LOW -- ATTACK line to a CAST ask).** vs27 s27 (Main phase 2, a "Cast
  Primal Rage / Cast nothing" menu): the model emitted an `ATTACK: ...` line (it had already attacked
  this turn) and the whole reply is DUPLICATED (sampling artifact) -> no Cast/CHOICE line -> unparse.
  Correct answer was "Cast nothing" (the model even reasoned Primal Rage is redundant). Cheap harness
  tolerance: a stray `ATTACK:`/combat line arriving at a non-combat CAST ask could map to
  Cast-nothing/pass rather than unparsing. Single instance; LOW.

- **N-redundant-global (representation, LOW, re-filed).** A hand card whose global effect is already
  active (Primal Rage in hand while in play) amplifies the decode loops (vs14 s21, vs110 s1). Annotate
  it, e.g. "Primal Rage (already in play - a 2nd copy does nothing)". Cross-seat; see
  general-suggestions #2.

- **Trade-outcome annotations (wave-19 batch) -- VALIDATED accurate at this seat.** All 19 deck62
  block prompts carried correct "(both die)/(neither dies)/(you kill it, blocker lives)/(blocker
  dies, attacker lives)/[deals 0 - only absorbs]" labels + the lethal gate; ZERO misleads. They
  enabled the value blocks (vs135 s25 deathtouch-flyer trade; vs14 s23 Spider-eats-Merfolk). One
  correct-but-IGNORED residual (chump-blocking the 0/1 engine while ahead) is routed to the GUIDE
  (edit B), not the annotation. Also: the stale-plan correction note fired correctly at vs14 s37
  ("Canopy Spider is tapped... you have three untapped Argothians") and the model then blocked right.

## Guide edits shipped this wave (round 3, surgical)
1. **(A) Generalized the anti-spiral line** from "WHEN YOU ARE BEHIND" to "YOU CANNOT ALWAYS WIN THIS
   TURN ... (AHEAD OR BEHIND)" -- the ahead/close lethal-hunt spirals (vs135 s32 unparse, vs135 s17,
   vs14 s40) were uncovered by the behind-scoping; the behind case is already handled (vs49 parses).
2. **(B) Added a defensive protect-the-engine clause** -- don't chump-block the 0/1 Argothian/Birds to
   absorb NON-lethal damage while ahead (vs135 s25, vs14 s36), per the block prompt's own "only
   absorbs damage" / "NOT lethal" annotation.
3. **(C, minor) Mulligan clause** -- "decide KEEP or MULLIGAN from what the hand HAS, do not plan out
   future turns" -- targets the vs110 s1 mulligan over-planning decode loop (low-confidence; decode
   loops resist prose, so this is a cheap nudge, not a claimed fix).
Everything else byte-frozen.

## Rotation verdict
**NOT a rotation candidate -- GUIDE REVISED (surgical, round 3) + NEW-WORK-SIGNAL present.** Guide-mod
= YES (edits A/B, plus minor C). New-work-signal = YES and durable: the ahead-can't-close broadening
(A) and the defensive-protect-engine clause (B) both need a next corpus to confirm; the
decode-repetition-loop class (N6, now the dominant unparse driver) and the channel-confusion (N7) are
new and need another corpus to characterize. deck62 fails BOTH rotation gates (guide-mod = yes,
new-work = yes) and STAYS in the pool. Wave-20 must validate that (a) the generalized anti-spiral line
reduces the ahead/close lethal-hunt giants/unparses, (b) the defensive clause stops the engine
chump-blocks, and (c) whether the decode-repetition-loop class is a persistent model/decode issue or
a corpus-specific pocket.

## Engine items count: 4 open/active (N6 decode-loop watch; N7 ATTACK-to-cast-ask harness; N-redundant-global representation; trade-annotation validated-keep) + 1 closed (N2 shipped).
