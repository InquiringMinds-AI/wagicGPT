# Skill contribution — deck135 (Modern Snow) seat, wave 20

deck135 remains a FROZEN veteran (guide unchanged since wave-11; frozen again this wave, `cmp`-verified
byte-identical to live). No guide-CONTENT change proposed. All contributions are METHOD / instrument
(measurement-and-observation), acceptable as single-seat inputs per existing doctrine. Single-seat items
cannot add/cut a core line — none below attempts to.

## PROMOTE (validated across a full lifecycle now): the REVEAL-SEAM PARTITION-TRACE completed its arc — bug FOUND (wave-19), FIXED, and VALIDATED at the same seat with the ZONE-OUTCOME instrument.

The wave-19 skill added the reveal-seam partition-trace as a NEW single-seat instrument: for a `kind:reveal`,
score the ZONE OUTCOME in the NEXT record's `events` ("Your revealed X goes to <zone>"), never just "did the
model answer." This wave that instrument closed its own loop — it caught ENGINE-R1 (Glacial Revelation to-hand
drop) in wave-19 and CONFIRMED the fix in wave-20 by the same zone-outcome read (vs27 s18: 4 chosen snow
permanents -> `goes to hand`, 2 unchosen -> `goes to graveyard`, exactly the Oracle partition). This is a clean
demonstration of the skill's own terminus rule ("a seam's DECISION-SURFACING can be fixed while its RESOLUTION
stays broken — score the ZONE OUTCOME") working in BOTH directions: the same read that exposed the resolution
bug is the read that verifies the fix. Method note worth carrying forward: **a reveal-fix confirmation needs the
zone-outcome read on the SAME card that broke, not just "no fallbacks this wave"** — the fallback count went to
0 partly from the eligibility fix and partly from draw variance (only 1 Glacial Revelation cast), so the
partition proof had to come from the events, not the fallback tally.

## PROMOTE (representation-fix lifecycle, ABSENT-MARK shape): the ELIGIBILITY-SURFACING fix shipped and its verification signal is the SPIRAL COLLAPSE, not a marker-string grep.

Wave-19 this seat proposed "surface the eligibility filter on reveal/search to-hand partitions" (three cards
offered the model options it could not legally move, unmarked -> giant-reply deduction spirals + ineligible
picks). The fix shipped as `[eligible for "..."]` / `[does NOT qualify - goes to "..."]` marks on every revealed
card plus an `ELIGIBILITY:` header. Verification signal for THIS shape (adds to the skill's shape-specific
representation-fix roster): the marker is not a single string to grep — it is a per-option annotation whose
success shows as a **REASONING-TAX COLLAPSE at the annotated windows** (Into the North reveals: wave-19 ~12k-char
"which are snow lands" spirals + 2 fallbacks -> wave-20 short 421-609-char clean picks, 0 fallbacks, 3/3
outcome-correct). Same family as the wave-9 own-targets-annotation reasoning-tax-drop rung, now confirmed for a
reveal/search chooser. Portable rule: for a per-option eligibility annotation, score CLOSED when (i) ineligible
picks -> 0 AND (ii) the derivation shortens at the annotated windows measured against the un-annotated ones.

## NEW single-seat WATCH (instrument correction): the FALLBACK COUNT UNDERCOUNTS the decode-loop tax when a loop opens with a RETRACTED false-start CHOICE line.

The seat's decode-loop tax is measured by the fallback count (unparsed_reply on >12k-char repetition spirals).
This wave surfaced a loop that is INVISIBLE to that metric: vs35 s10 spiraled 15k chars, converged correctly in
prose on "cast Icehide Golem", but had emitted an early `CHOICE: 4 (Cast nothing)` that it then retracted
("Wait, I made a mistake") and never re-emitted a corrected CHOICE line. The parser took the retracted
false-start as a clean parse (`fallback: null`), logging the wrong action. So a decode-loop with an early
parseable-but-retracted CHOICE line is a hidden member of the same tax pocket — it costs a decision (here a
creature deploy) yet does not inflate the fallback tally. Instrument rung: **when tallying the decode-loop tax,
also grep clean-parsed records for a repetition signature + a "Wait, I made a mistake"/multiple-CHOICE
signature; the true loop count is fallbacks PLUS these hidden retracted-first-CHOICE parses.** This is the
answer-order sibling of the skill's wave-10 "obeyed-in-reasoning, leaked-at-number" class, extended to the case
where the leak is a RETRACTED first CHOICE the parser prefers over the (missing) final one. Single-seat, 1
non-fatal firing in a won game -> WATCH; adopt into the scoring rung if a second seat shows a retracted-first-
CHOICE hidden loop. (Routing: HARNESS — prefer the LAST CHOICE line / detect retracted early CHOICE; plus the
carried decode-time repetition guard.)

## CONFIRMS (carried, still valid)
- The REPEAT-LOOP unparsed signature (wave-18/19): 7 fallbacks, all >12k-char decode spirals on lethal-math /
  mana-confusion / survival-enumeration at hard-or-hopeless spots, all in LOST games, none flipped a winnable
  game; salvageLoopedChoice 0/7 (keys on CHOICE/ATTACK; these spirals never emit a clean trailing line).
  Reconfirmed as cosmetic-in-outcome, route to decode-time mitigation, count per-game.
- The pause-aware WATCH discipline: Force of Negation was cast 0 times this corpus, so the reflexive
  off-whitelist-Force class had NO test window -> stays at WATCH (absence is not vindication). Correct application
  of "demote a guardrail only once its class was OBSERVED OBEYED, never merely because it was ABSENT."
