# deck146 -> strategy-writing-skill proposals (wave-29)

Context: deck146's FIRST-GUIDED corpus. The guide performed well (removal-on-curve, Lost Mine
100%, completion validated), but the live render exposed THREE fact drifts in a guide that was
correct when written against Scryfall. These generalize.

## PROPOSAL 1 (STRONG) -- Verify-oracle rail extends to the ENGINE RENDER's NUMBERS, not just
Scryfall. When a render surfaces a countable fact the model reads (room counts, loyalty, MV,
P/T), a guide must not assert a DIFFERENT number for the same thing. deck146's guide said Lost
Mine "completes in just 3 rooms"; the render the model reads says "7 rooms". Both can be "true"
(3 = min branch path; 7 = total rooms) but the model sees only the 7 and the guide's 3 reads as
a contradiction. RULE: for any numeric claim, check it against what the CURRENT render prints for
that same field; if they differ, phrase around the render's number or drop the number and argue
from effect/value. (This is the render-side twin of the existing Scryfall verify rail.)

## PROPOSAL 2 (STRONG) -- Never anchor a guide directive to an OPTION NUMBER when the engine
orders options dynamically or asks for an answer-by-name. deck146's guide said "pick Lost Mine,
option 2 - do not default to option 1". In this corpus Lost Mine was option 2 at the first
selection but option **3** at every re-selection (the engine re-orders). The render literally
says "answer with its name". RULE: reference choices by NAME (or by the effect on the option
line), never by ordinal, unless the ordering is provably fixed. Ordinals are the single most
fragile thing a guide can hard-code.

## PROPOSAL 3 (MODERATE) -- A guide line that DESCRIBES an engine render GAP becomes a LIE the
moment the gap is fixed, and it will actively mislead. deck146's guide said "room branches show
only the room NAME with no text" - true in wave-28, false in wave-29 (branches now carry
{room effect}). A guide that tells the model "you must recall this from memory" when the render
now shows it wastes the fix and can suppress correct reasoning. RULE: any guide sentence that
narrates an engine LIMITATION ("X is not shown", "you cannot see Y") is a re-verify-EVERY-corpus
item - grep the guide for "no text/not shown/cannot see/truncated" each review and re-check
against the live render. When a gap closes, SHED the compensating prose.

## PROPOSAL 4 (MODERATE) -- When a batch fix lands, credit-check the guide's OLD leak framing.
deck146's wave-28 guide framed "removal hoarding" as a decision leak to coach against. The real
cause was a WINDOW gap (fixed engine-side this corpus). The coaching prose ("do not hoard
removal") turned out harmless-to-helpful, but the LESSON is: before writing a behavioral coaching
line, confirm the behavior is a genuine DECISION and not an artifact of a render/window gap the
findings flagged as an engine item. Coach decisions; do not coach around bugs (route those to the
ledger). deck146 got this right by luck (the coaching aligned with the fix); make it deliberate.

## PROPOSAL 5 (MILD, campaign-first) -- For multi-step campaign mechanics (dungeons, sagas,
classes), the guide should teach the STEADY-STATE loop the model will actually face, not just the
entry decision. deck146's completion + re-entry loop ("after you complete one, keep picking the
same dungeon to re-complete for cards while the anthem stays on") only became teachable once the
corpus showed completion working. RULE: once a campaign mechanic's full cycle is observed live,
add the post-completion / repeat-cycle guidance - the model will re-enter the decision many times
per game and the first-entry framing under-serves the 2nd..Nth.
