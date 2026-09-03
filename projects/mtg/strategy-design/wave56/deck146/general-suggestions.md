# deck146 -> core-prompt / render proposals (wave 56), layer-routed

Engine DEFECTS go to the seat file's engine list (E-1..E-5), not here. These are
information-presentation proposals for the composed per-decision prompt.

## R-1 (HIGH, render) - the pregame hand header must count land-backed spells as lands
ROUTE: render (pregame serializer), not prompt prose. The header is
`Your hand (7 cards), counted by the engine: 1 land, 6 spells.` and it is the tally the trust
doctrine makes load-bearing for every mulligan. It counts printed card types, so a card whose BACK
face is a land and which the engine now lets you play as one reads as a spell.
ASK: count a hand card as a land when it carries a `forcetype(land)` hand entry, and print the
split, e.g. `counted by the engine: 2 lands (1 of them a land on the back of a spell: Agadeem's
Awakening), 5 spells.` The colour line and the coverage clause follow the same count.
REPRO: `146v130` seq 1/2/3 and `146v152` seq 1. Full case in the seat file, **E-1**.

## R-2 (MED, render) - the `Land drop:` summary line and the MDFC land row contradict each other
ROUTE: render. 17 of the 62 windows carrying a numbered `PLAY THIS AS A LAND` row also print
`Land drop: you have no land you could play right now (none playable from your hand or any other
zone).` One of those windows is `146v125` seq 13, where the seat took the row anyway and the land
entered - so the summary line is the false one.
ASK: derive the `Land drop:` line from the same set that produces the option rows. Two rendered
statements about the same question must not disagree; per the trust doctrine the model believes
the one it reads first, and here that is the summary.

## R-3 (MED, prompt bytes) - the `Flip Side` display-toggle row has no reader
ROUTE: render / option-set construction. `Flip Side` rendered **109 rows in 90 windows** at my
seats and was taken **0 times**; each row is ~500-650 characters of explanation whose whole content
is "you do not need this row". Below the prompt, the same toggle is the ONLY option in **760**
priority windows, each of which the engine constructs and then discards
(`AIPlayerGPT[phN]: only display-toggle (Flip Side) options; auto-passing without a model call`).
ASK: a display toggle is a HUMAN-UI affordance; an LLM seat has no display. Suppress the
`doubleside` ability from the AI seat's option set entirely. That deletes 109 rendered rows, ~60 KB
of prompt text across the corpus, and all 760 auto-pass windows, and removes nothing the model can
use. This is the `ishuman` lens applied in the opposite direction from usual.

## R-4 (LOW, render) - the back-face land row should say whether it enters tapped
ROUTE: render. Every one of Emeria, Shattered Skyclave / Agadeem, the Undercrypt / Pelakka Caverns
answers a follow-up menu (`1. pay 3 life - enters UNTAPPED [usable this turn] / 2. tap - enters
TAPPED`). The land row itself says only `puts "<name>" onto the battlefield as a land (taps for
{B})`. At `146v126` s19 the reply's plan asserted "the land enters untapped" before that menu was
shown; it was not free.
ASK: append `{it will ask you to pay 3 life to enter untapped}` to the row for the pay-life class,
and nothing for the Pathways (which have no such menu).
