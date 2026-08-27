# deck162 - core-prompt / render proposals (wave 51), layer-routed

G-1 (render, lane F - working, one refinement). `{feeds: ...; converters on your battlefield: K -
<names>}` rendered on 78/78 engine rows at my seats and the guide is now keyed to it (162-G). The
engine's K counts Liliana's Caress unconditionally; Caress converts DISCARDS, which only happen
once their hand is above seven. Proposal: when the converter list is Caress-only, append the hand
size the count depends on - `converters on your battlefield: 1 - Liliana's Caress (converts only
past 7 cards in hand; their hand: 5)` - so the row carries the qualifier the guide has to add
today. LOW; `162 vs125` s8 was the only Caress-only K this corpus and their hand was 7.

G-2 (protocol, list answers). The cleanup-discard ask says "write PUT: followed by the 1 card
number"; the reply `PUT: 3 (Forced Fruition)` mirrored the CHOICE line's "number (name)" habit
the protocol teaches everywhere else, and the engine executed row 2. Either the PUT parser
honours a bracketed name the way `parseChoice`'s named-row family does (engine E-1, preferred), or
the ask text says "the number only - no name" (the guide says it now, 162-H). Not both.

G-3 (efficiency). The mis-parsed discard was also the corpus's slowest reply at my seats (477,591
ms, `162 vs146` s17): a cleanup ask arriving after a 1,000-char narration turn produced 2,000+
chars of board re-derivation before the PUT line. A one-line "PUT: n - nothing else is read"
reminder is already in the ask; the length is the model's. Reported for D30's line.
