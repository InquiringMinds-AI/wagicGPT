# Strategy-writing skill proposals from the deck123 seat (wave 53)

Only what this seat's twelve-game evidence supports. Amendment numbers are the wave-52 edition's.

## S1 (extends #154, "a re-keyed literal replaces its derivation, it does not sit beside it")
**Add the deadline.** A derivation that a shipped literal now answers must be cut IN THE SAME
WAVE the literal first renders, not the wave after. Evidence: RULE 4's Slip test asked the pilot
to scan a name list for "toughness 2 or more" while the row itself printed
`{kills 0 of the 3 legal targets at -1/-1}`; the pilot cast the Slip anyway (123v130 s64) with
both surfaces on screen. The derivation is a second, weaker copy of the literal's answer, and
where they disagree in a pilot's head the derivation is what it acts on. A53-2 makes the cut.

## S2 (new) - A GUIDE THAT KEYS TO A COUNTING LITERAL STATES THE LITERAL'S DENOMINATOR
#171 makes a mulligan-literal sentence say what the literal does NOT decide. The same failure
mode reached pricing literals this corpus, and it cost a won game at the other seat
(130v162 s62: `{kills 0 of the 2 legal targets at 3 damage}` on a row whose target list began
`The opponent (player, life 1)` - lane O counts creature targets only, by its own note).
**Proposed rule:** when a guide sentence keys to an engine literal that COUNTS something, the
sentence names what is outside the count. "kills 0" means "kills 0 CREATURES"; "N permanents
listed, of which K are creatures" is not a hand count; "max affordable X" is not "the X you can
afford after this cast". One clause, on the rung that reads the literal.

## S3 (new) - CARVE-OUT REACHABILITY (a #131-shaped check for exceptions, not for hands)
#131 constructs the twelve hands at every rung whose text changed. Exceptions need the same
treatment: **construct the case where the carve-out fires and ask whether its action can still
satisfy the rule it interrupts.** deck123's RULE 2 said "a colour at 0 comes first" and then
carved out "a basic only against deck130" - and Marsh Flats fetches only Plains or Swamp, so in
the exact case where the carve-out fired (123v130 s36, {U} at 0, two Intruder Alarms in hand) no
action inside the carve-out could satisfy the rule above it. The pilot took the carve-out and
lost the game. A53-1 fences it. Cheap to run: for each exception, one line - "the exception's
allowed actions, and the primary rule's requirement, in the same sentence".

## S4 (new, method) - #168 COUNTS ROW LITERALS FROM THE ROWS, NOT FROM THE PROMPT
The guide is part of the prompt, so any literal a guide quotes is counted once per window by a
naive `prompt`-scan. Two of this seat's literals are quoted in the guide itself
(`as you last stated it`, `Hold priority - do not ask me again`), and one of my own new sentences
quotes `a second copy changes nothing`, which produced a phantom "1 render" of a string that has
**0 row renders** in the whole corpus. The three-number audit reads `options_text` (and the
instruction block) for renders, `chosen_text` for takes, and says which field it read.

## S5 (new, trust doctrine boundary) - THE CARRIED PLAN IS NOT THE RENDER
"Never teach doubt of the render" governs what the ENGINE states. The `YOUR PLAN` block is the
pilot's own older text quoted back, and since lane N it arrives with its age printed
(`as you last stated it, N windows ago on turn T`). A guide may therefore instruct the pilot to
prefer a `{right now: ...}` clause over its own carried plan without breaching the doctrine - the
plan is not a render of the game state, and the stamp is the engine's own statement that it is
old. Evidence: 123v130 s63/s64, a plan asserting Morbid beside a row denying it, believed at 1
life. Proposed as an explicit carve-out sentence in the doctrine so later reviewers do not have
to re-derive that this is allowed.
