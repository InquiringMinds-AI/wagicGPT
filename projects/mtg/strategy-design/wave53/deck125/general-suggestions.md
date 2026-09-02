# Core-prompt / render proposals from the deck125 seat (wave 53)

Layer-routed. Engine defects (the hang, the hold key, the sacrifice framing, the echo matcher, the
prose/CHOICE detector) are in `wave53/seats/seat-125-126.md`'s engine list and are NOT repeated here.
Everything below is a change to what the prompt SAYS, not to what the engine does.

## G1 - Skip the opponent-turn window whose stack is empty at a phase this seat has never cast in

deck125 answered **479** opponent-turn windows in six games and cast 47 spells, 45 of them in the
opponent's Main phase 1. The opponent's End (35), Cleanup (34), Combat begins (37) and Combat ends
(34) produced **zero casts in three consecutive corpora**. Two of them opened at random
(`1788334427-ai_baka_deck125-...-vs-ai_baka_deck146.jsonl` seq 27, seq 41) have no `ON THE STACK`
section at all and one cast row - a Sphinx's Revelation at `max affordable X=1`, which this deck's
guide forbids at any life above 10.

Ask: extend the `mana_only_windows_skipped` idea to **empty-stack opponent-turn windows at phases
with no cast history for the seat**, counted into the `gameend` record the same way. This is a
prompt-economy change, not a legality change - and it must not narrow the Main-phase-1 window, which
is where all 45 casts and the whole draw-go thesis live.

Why it matters beyond tokens: 30.7% of my 1,254 ask/priority decisions sit inside runs of 3+
consecutive byte-identical DECLINED menus (deck125 34.7%). Lane N's target was under 10%. The
HOLD row cannot reach that figure while the hold key re-opens on every draw step (seat file HIGH #2),
so this is the independent lever.

## G2 - The `{spends K of your M ...; <card> in your hand needs N}` clause names a card that may not
be the one the rule is about

The clause is doing real work - 434 renders last corpus, and I re-keyed deck125's tap-out gate onto
it (W80). But at `1788334465-ai_baka_deck125-...-vs-ai_baka_deck162.jsonl` **seq 23** it read
`{spends 2 of your 2 untapped mana sources this turn; Path to Exile {w} in your hand needs 1}` -
naming a Path, while the card the seat had to protect was a Fall of the Gavel at 5. A guide that
keys to the clause has to warn the pilot that the NAME is not the price, which is a sentence the
render could make unnecessary.

Ask: when more than one unpayable card is in hand, name the **most expensive** one, or name the
count (`3 cards in your hand need more than that`). Either removes the trap; the subtraction
(`spends K of M`) is already the useful half and is correct in all 434 renders I looked at.

## G3 - `[DRAW PRICE:` should carry the finished subtraction on the ability form too

Lane O's cast form is excellent and it is why this seat declined eight lethal rows
(`vs162` seq 79-88, `- you would be at -4; this KILLS you`). The DRAW-ABILITY form beside it on the
same menus still reads only `[DRAW PRICE: this draws 1 card, and the opponent's Underworld Dreams,
Ob Nixilis, the Hate-Twisted punish every draw, so taking it costs you 2 life right now]` - a cost,
with no resulting total (`vs162` seq 78 row 3, seq 88 row 2). No harm arose this corpus because
2 life never mattered; the asymmetry is worth closing while the code is warm, because the guide now
teaches "read the number after `you would be at`" and one form does not have one.

## G4 - Name the opponent-turn phase in the ask line

Ask lines read `Casting decision (Draw, opponent's turn): which card do you cast now, if any?`,
which is already good. Where it is missing is the **priority** seam, whose ask line does not say
whose turn it is; on this seat the priority seam is where the two-row Elixir menu repeats. Cheap and
purely additive.
