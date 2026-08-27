# deck130 - core-prompt / render proposals (wave 50), layer-routed

Engine defects are in `wave50/seats/seat-123-130.md` (H1-H2, M1-M3, L1-L2). These are the
prompt/render proposals; each names its evidence and its layer.

**G1 (render, ability target rows) - the damage-outcome annotation on activated-ability rows.**
Seat H2. `Deal 2 damage with Siege-Gang Commander targeting Fate Unraveler [opponent's
battlefield]` (130v162 seq 23) and `... Pyrite Spellbomb targeting Ob Nixilis, the Hate-Twisted`
(seq 17) print no `{right now: takes 2 damage - SURVIVES (toughness 4)}` / `(loyalty 3)` while the
spell menus do (130v146 seq 17, 130v123 seq 38). Same emitter string, same place on the row.
Layer: render.

**G2 (render, upkeep return row) - price the Hammer return against the hand.** Seat M3. `Put a card
into hand with Hammer of Bogardan [cost: {2}{r}{r}{r}]` at five sources with the Commander in hand
(130v123 seq 18; wave 49 x2). Ask: `{spends 5 of your 5 untapped sources this turn; Siege-Gang
Commander {3}{r}{r} in your hand needs 5}` - the same shape as the `{leaves N of your M untapped
mana sources untapped}` clause the cast rows already carry, applied to an activated ability whose
cost is most of the turn. Layer: render.

**G3 (render, blockers header) - name the punisher on the GANG BLOCK clause.** 130v162 seq 22:
`GANG BLOCK: your 3 biggest together deal 4, enough to kill this attacker` on Fate Unraveler while
the `DRAW PUNISHERS` line named it two lines up. Ask: when the attacker is one of the permanents
the DRAW PUNISHERS line lists, the clause says so (`... enough to kill this attacker - it is a
DRAW PUNISHER`). Layer: render; one string join.

**G4 (render, X-announce menu) - an exit row, or the (a)/(b) verdict on the cast row.** 130v126
seq 60: every killing X read `YOURS: Siege-Gang Commander`; the menu has no "cast nothing" row;
the pilot chose the least bad X and lost the Commander. The `Choose an option` menu one screen
earlier has `Decline - do nothing`. Ask (either): a `0. Do not announce - keep the card` row on
the X menu when the spell has not yet left the hand (legality permitting), or a `{every affordable
X that kills something of theirs also kills Siege-Gang Commander}` clause on the cast row's `{X
pricing:}`. Layer: render / engine (the X ask's cancel semantics need the engine seat's ruling).

**G5 (render, cycling rows) - the "not offered again this turn" tag as a neutral fact.** Seat L1
(inference, one window): 130v152 seq 16 `cycling with Lay Waste ... {if you pass here, this option
is not offered again this turn}` taken at four lands with 2 mana left. If the tag's purpose is the
two-decline cap (lane W's design), a wording that states the cap without the "pass here" framing
(`{offered twice per turn at most}`) removes the urgency reading. Layer: render; low.

**G6 (render, Land drop / Molten Rain) - the pain-source payment at low life.** 130v123 seq 56:
`- Paid {1}{r}{r} for Molten Rain with Mountain #1, Talisman of Impulse #1, Talisman of Impulse
#2` at 2 life (-> 1). The CAUTION line exists on the mana line; the cast row's `{leaves N ...}`
clause does not say the payment will cost life. Ask: `{paying this taps a Talisman - 1 damage to
you}` on cast rows when the only affordable payment includes a pain source. Layer: render.
