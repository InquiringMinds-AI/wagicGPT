# Skill contribution — deck135 (Modern Snow / Snow-Force control) seat, wave 19

Per-seat proposals for the synthesis step to fold into `strategy-writing-skill.md`. deck135 remains a
FROZEN veteran (guide unchanged since wave-11; frozen again, `cmp`-verified byte-identical to live). No
guide-CONTENT change proposed. The value this seat adds is METHOD: the reveal-seam verification
instrument, the "engine bug can drive the win-rate, not the guide" lesson, and the eligibility-surfacing
representation principle. All are measurement/observation contributions (not guide-content changes), so
acceptable as single-seat inputs per existing doctrine.

## NEW reviewer instrument (single-seat, high-value): the REVEAL-SEAM PARTITION-TRACE.

The wave-19 reveal seam (`Player::decideReveal`, translog kind `reveal`) bundles a whole
reveal-and-partition decision into one ask. Verifying it is NOT the same as verifying a target or menu
pick — the outcome lives in the NEXT record's events (the zone moves), and the model's `chosen_text`
can be faithfully ACKNOWLEDGED by the seam yet moved to the WRONG zone by the resolving card script.
Portable procedure for any reviewer whose deck reveals:
1. For each `kind:reveal`, read `options_text` (what was offered), `chosen_text` / the `PUT:` line
   (what the model asked for), AND the next record's events (`"You revealed N and put X to <verb>"`
   then the per-card `"Your revealed X goes to <zone>"` lines).
2. Cross-check the DESTINATION of each chosen card against the card's Oracle/`text=`: did the "to-hand"
   partition actually reach hand, or was it swept to the "rest" zone?
3. Separate THREE failure modes that look alike from the option list: (a) RESOLUTION bug — engine
   acknowledges the pick then moves it to the wrong zone (deck135 Glacial Revelation: `all(*|reveal)`
   optiontwo overwrote the to-hand picks); (b) ELIGIBILITY gap — the model picked cards the option
   restriction forbids because the restriction wasn't surfaced (deck135 Azcanta activation: chose
   creatures for a `-land;-creature` slot); (c) RENDERING misdirection — the effect is mislabeled
   (deck135 Into the North search rendered as a whole-library "choose subset to hand" menu). Only (a)
   is an engine/card-script bug; (b) and (c) are representation.

Method note for Step-0-ter: a reveal seam's DECISION-SURFACING can be fixed (the model now drives the
choice) while its RESOLUTION stays broken — score the ZONE OUTCOME, never just "did the model answer."

## CONFIRMS (promote from watch): a card-SCRIPT bug can drive win-rate independent of the guide, and the reveal seam is how you catch it.

deck135's 3/6->2/6 dip this corpus is partly an ENGINE-BUG artifact — Glacial Revelation, a card the
guide teaches as a card-ADVANTAGE engine, actively MILLS the deck (to-hand partition dropped) and
worsened 2 of 3 adjudicated losses (milled the only blocker at 11 life; milled the finisher at 10 life).
Lesson for the win-rate-is-context doctrine: when a frozen-guide veteran's record moves, check the
DECISION-LEVEL zone outcomes before attributing it to play or matchup — the interactive-vs-heuristic
routing (ishuman lens) can expose latent card-script bugs the heuristic AI never hit, and those are
invisible from win-rate alone.

## NEW representation principle (for the annotation/option-line skill section): SURFACE THE ELIGIBILITY FILTER on reveal/search to-hand partitions.

Two of the seat's giant-reply fallbacks (d35 s10, d27 s8) and the Azcanta whiff (d27 s24) share one
root: the reveal option list offers the model cards it CANNOT legally move to the target zone, with no
mark. Into the North offered the whole 49-card library for a "search a snow LAND" effect; Glacial
Revelation offered 6 cards without marking which are snow permanents; Azcanta offered 4 without marking
which are noncreature-nonland. The model burns thousands of tokens deducing eligibility and either
spirals (unparsed) or picks ineligible cards (0 to hand). General lesson, same family as wave-18's
"name the pitch card": for any reveal/search/tutor chooser, the option line should either OFFER ONLY the
legal targets for that partition, or annotate each card's eligibility — the model cannot make a clean
partition it has to reverse-engineer the legality of.

## Carried instruments (unchanged, still valid): the REPEAT-LOOP unparsed signature + the pitch-card-naming validation.

- The REPEAT-LOOP signature (wave-18) reconfirmed: 4 of 9 fallbacks are decode-time repetition spirals
  (>12k chars, lethal-math / mana-confusion at hard spots), all in already-lost/won contexts, none
  flipped a winnable game — cosmetic tax, route to decode-time mitigation, count per-game.
  salvageLoopedChoice did NOT rescue any (0/9) — the salvage keys on CHOICE/ATTACK and misses BLOCKS +
  reveal PUT lines; note that gap.
- The wave-18 pitch-card-naming annotation is now VALIDATED at this seat (d62 s19: the option named the
  auto-exile pool; the engine pitched a dig spell, not the finisher). A confirmed annotation-design win
  worth promoting as a template: when the engine auto-selects a hidden cost, name what it will consume.
