# Wave-16 reviewer brief (shared context for all 7 deck agents + synthesis)

Second per-deck-rotation cycle. deck140 rotated out; deck62 (Enchantresses) entered GUIDELESS.
decks 17/21 play their FIRST GUIDED corpus (their initial guides shipped in wave-15 commit
04bf8f316). Binary 69793930a adds the wave-15 engine batch: free alternative casts at zero
mana (Force's exile-pitch now considered when tapped out), the stale_echo option-subset-of-echo
fallback matcher, and the hybrid-pip affordability annotation on cast-option lines
("[hybrid: each {u/b} pays with U or B - total N mana]").

## Corpus data

- **PRIMARY**: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260716-084532` — 21 games, round-robin
  over 44,110,133,135,17,21,62. Binary /tmp/wagic-69793930a.
- Win table: 133:4/6, 17:4/6, 110:4/6, 44:4/6, 62:2/6, 21:2/6, 135:1/6. 0 raw timeouts,
  only 4 life-adjudicated (down from 9-13 — games completing). 0 segfaults (crash fix's 2nd
  clean corpus).
- Fallbacks 5.2%: 40 unparsed (ramble guard), 14 stale_echo, 5 defer. Latency regime unchanged
  (Spark @0.75).

## KNOWN CORPUS-WIDE (do not re-diagnose)

- The hybrid-pip annotation was OFFERED on 43 decisions — the seats that see it most are
  17/44/21. Reviewers at those seats: measure its EFFECT (did the hybrid misread/decline class
  shrink vs wave-15? cite before/after decisions).
- Only ~1 Force of Negation cast happened corpus-wide (deck135-vs-21: hand->stack, resolved).
  The zero-mana free-cast fix got LITTLE exercise — not-exercised ≠ not-fixed; deck135's
  reviewer checks what windows existed.
- stale_echo 14: wave-15's superstring false-positive fix shipped. Check your seat's instances:
  a superstring echo (extra words beyond the option's) should now MATCH, so any remaining
  stale_echo should be genuine menu confusion. Flag wrongful downgrades with seq. One
  pre-flagged suspect: deck135 s33 (echo "search basic land with Prismatic Vista targeting
  Snow-Covered Forest" tagged stale — determine which menu was actually shown).

## Seat assignments

- **deck44 — CORRECTED READ (priority):** wave-15's reviewer produced a FALSE "combat asks
  removed" finding (rejected in wave15/synthesis-notes.md — the corpus had 29 attacker asks at
  this seat) and its 1/6 loss attribution was unreliable. Re-read this seat fresh: 4/6 now.
  Also owns: hybrid annotation effect (UB Faeries costs).
- **deck110:** open items E2 (equip churn A→B→A net-zero move) and E6 (Mox Opal
  "Cast Card Normally" shows the Mox's own text). Standing validator for Galvanic/attackers.
- **deck133:** items A (removal offered with only friendly targets) + B superstring (verify the
  fix caught the Yawgmoth shape); self-FP watch.
- **deck135 (1/6):** free-Force-at-zero-mana exercise check; stale [attacking] tag; the s33
  stale_echo suspect; construction-vs-play loss split.
- **deck17 — FIRST-GUIDE VALIDATION:** 3/6 guideless → 4/6 guided. Did the guide's rules fire
  (hybrid-off-Islands as rule #1, defender roster, attack floor, steal-aura timing)? Which
  taught classes died, which persist, what needs revision? Guide revision IS expected at this
  stage (first-revision trim of core-restating sections was pre-flagged).
- **deck21 — FIRST-GUIDE VALIDATION:** 3/6 guideless → 2/6 guided. Same charge: did burn-as-
  clock, haste roster, anthem math fire? Is the record drop signal or variance? Revise as
  needed.
- **deck62 — GUIDELESS FIRST PASS (Enchantresses):** the skill's Step 0-bis doctrine applies
  (reasoning-tax spikes = saliency; idiosyncrasy-as-rule-#1; teach engine reality; countermand
  core lines where the deck demands; write the initial guide).

## Doctrine (BINDING)

Win-rate is context; the unit of analysis is the DECISION with seq repros. Layer-routing
enforced. Single-seat items cannot add/cut core lines. Per-deck rotation: state your seat's
verdict explicitly in notes.md (no-guide-mod AND no-new-work-signal = rotation candidate).

## Output contract (per deck agent, sequential, in strategy-design/wave16/deckN/)

findings.md -> strategy.txt (veterans: byte-identical if frozen, REVISED if warranted;
newcomer 62: the NEW initial guide) -> skill.md -> general-suggestions.md -> notes.md (engine
items + rotation verdict). CURRENT skill = strategy-design/wave15/strategy-writing-skill.md.
Veterans read their wave-15 deckN continuity files.
