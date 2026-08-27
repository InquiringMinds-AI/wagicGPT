# deck146 - core-prompt / render proposals (wave 48), layer-routed

Engine BUGS are in the seat file's ENGINE / RENDER list (E-1 ATTACK-line replacement, E-2
tapped-Hive row, E-6 off-menu numbers). These are the prompt-text and render-affordance halves.

G-1 (render, pairs with seat E-1). **State the reply-grammar rule for a re-answer on the
`attackers` and `blockers` asks, or apply the CHOICE rule there.** The core prompt's answer-first
protocol now (E1) takes the last coded `CHOICE:`; the attackers ask has no sentence saying what
happens when two `ATTACK:` lines disagree, and the executed answer was the first
(`146 vs126` s48, `answer_replaced: true`). Either the same "your last ATTACK: line is the one
taken" sentence goes in the attackers/blockers tail, or the grammar rejects a second line. Either
is fine; silence is what produced a record whose field contradicts its action.

G-2 (render, pairs with seat E-2). **On a `becomes <creature>` activated-ability row, print the
source's tapped state and what the auto-payer will tap.** `[this land is TAPPED: animated, it
cannot attack this turn]` and, where the only source for a pip is a creature that could attack,
`{paying this taps: <name>}`. Both are facts the engine already holds (the battlefield line
prints `[tapped]`; the mana line prints `Those sources`); the row is the surface the decision is
made on (#89). Restriction-first, deletes nothing.

G-3 (render, R5 follow-up). **Lane P's `most expensive to attack into:` clause works - 1/1 on my
seats, and the reply named the expensive defender.** The pilot then sent anyway, so the residual
is guide-side (146-A). No change asked; recorded so the lane is not re-opened.

G-4 (core prompt, converter block). **Add one sentence of DIRECTION to the converter block.** The
block reads `life ITS CONTROLLER gains also makes the other player lose that much life` and the
fold reads `their converter takes N off you` - both correct - and the pilot read them as a wash
three times ("net 0"). One clause, `this is not a trade: their total goes UP by N and yours goes
DOWN by N in the same event`, is a true token that names the arithmetic the pilot keeps getting
wrong. Guide-side answer applied at 146 (146-A) so the rule stays right if this never ships.

G-5 (render, R17 follow-up). **`Cast nothing right now` is still bare.** 18 answers on deck146,
20 of 71 last corpus; the free-looking row remains the only one with no consequence annotation.
R17's `{you keep N untapped sources you cannot carry into your next turn}` stands.

G-6 (measurement). `becomes beholder` was offered 94 times on one seat, 58 in Upkeep, and taken
0 times in Upkeep - the guide fix worked, but 58 round trips at ~10 s each is the cost of the
lane-46 Upkeep skew (E-3 carried) that a Main-1-only offer would not incur. Report the phase
distribution of activated-ability priority windows per seat next wave as a cost line.
