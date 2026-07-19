# deck35 -- proposals for the strategy-writing skill (Step 0-ter, revised-guide round 3)

All items single-seat; tagged. Layer-routing respected (engine items -> notes.md; decode/represent
items -> notes.md/general-suggestions). This wave's theme: a wave-20 engine fix was VALIDATED yet
mis-attributed, and the durdle instrument's ENGINE-vs-MODEL split (my wave-19 S1) paid off decisively.

## S1 [SHARPEN -- when an engine batch claims to fix a symptom YOU instrumented, verify the SYMPTOM collapsed, not just that the fix fired]

The wave-20 brief attributed deck35's wave-19 "0-attacker durdle" to PAYMENT (auto-tapping Gemhide for
mana) and shipped an attacker-sparing payment fix. Two DIFFERENT things had to be checked separately,
and conflating them would have written a false "fixed" verdict:
- **Did the fix FIRE correctly?** YES -- vs49 seq8: casting Winged tapped two lands and spared the
  Gemhide untapped. The mechanism works.
- **Did the SYMPTOM (0-attacker games) collapse?** NO -- vs49 is still a complete 0-attacker,
  0-combat-damage loss with an untapped eligible board. The fix addressed a real hazard but NOT the
  one that produced the symptom; the symptom's true cause (attackers-decision non-issuance) was
  untouched.
**Rung:** a fix VALIDATED-AS-FIRING is not a symptom VALIDATED-AS-GONE. When a batch says "we fixed
the thing your instrument measured," run the instrument again on the post-fix corpus and report the
OUTCOME count, not the mechanism. Here the instrument (0-attacker games with the eligibility-and-
issuance disqualifier from wave-19 S1) returned the SAME reading -> the attribution was wrong, and
saying so is the seat's most important finding. This is the payoff of wave-19 S1's model-vs-engine
split: because the instrument already separated "no attack happened" from "why," it could refute a
plausible-but-wrong root-cause claim with a clean counter-example (untapped attackers + 0 attack step).

## S2 [CONFIRM -- tax-relocation (my wave-19 S2) is now a 3-wave pattern, and it can relocate INTO a fragile-but-parsing state rather than disappearing]

Wave-18: block-CHOICE tax -> block seam. Wave-19: block tax tamed -> LAND-DROP seam (2 fallbacks).
Wave-20: the land-drop DISCIPLINE revision killed the pure-oscillation land drop (vs135 land drops now
390-544 chars, committed), and the tax RELOCATED AGAIN -- to cast/sequencing decisions AND a
pressure land-drop, now as 12-14k-char DECODE-REPETITION spirals that still terminate in a parseable
CHOICE (0 fallbacks). **Rung:** taming a tax at seam A does not remove it; it moves AND it can change
FAILURE MODE -- from unparsed (pure indecision, no CHOICE) to merely-verbose-but-parsed (repetition
loop that still lands a choice). A "0 fallbacks" result can therefore be FRAGILE: the same behavior is
one bad decode from truncating unparsed. Gauge it with reply-length-by-seam, and when the longest
replies are repetition loops, report the zero as fragile and route to a decode-time guard, not the
guide. (Sibling to wave-19 skill.md bucket-by-mechanism: repetition != reasoning-spiral.)

## S3 [CONFIRM -- a validated guide revision can be VALIDATED WORKING and the seat still NOT be a rotation candidate, when the live new-work signal is ENGINE]

deck35's guide is maturing: FC1 durdle dead 3 waves, land-drop discipline validated, two revisions now
confirmed. From a guide-maturity lens this is freeze-and-consider-rotation territory. But rotation
requires no-new-work-signal, and this seat is the PRIMARY witness to the corpus's most important engine
finding (attackers non-issuance, now 3-wave convergent, and the refutation of the wave-20 payment
attribution). **Rung:** decouple "guide converged -> FREEZE" from "seat has no new work -> ROTATE."
A converged guide + a live engine bug that only this deck's shape (creature-mana aggro with a haste
enabler that dodges the bug) surfaces cleanly = FREEZE the guide, KEEP the seat. State both verdicts
separately in notes.md.

## S4 [CONFIRM -- decompose a flat/dipped record into per-game CAUSE before touching the guide (wave-19 S4, reinforced)]

3/6 flat. The three losses decompose to one ENGINE (vs49 non-issuance), two RACE/matchup (vs27, vs135
-- attacked every offered turn, lost a coin-flip damage race to a faster deck). None is a model durdle
and none is guide-fixable. **Rung (reinforced):** a flat or dipped win-rate on a validated guide is not
a revision trigger by itself -- classify each non-win as ENGINE / VARIANCE-RACE / MATCHUP-WALL /
CONSTRUCTION first. When zero of them route to the guide, FREEZE. Do NOT invent guide prose to "recover"
a win-rate whose flips are all non-guide causes.
