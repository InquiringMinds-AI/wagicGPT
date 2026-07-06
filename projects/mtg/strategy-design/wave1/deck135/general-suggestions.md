# Suggested improvements to the general strategy guide

Target file: `bin/Res/ai/gpt/system_prompt.txt` (the shared prior every deck receives).

Two systemic failures showed up piloting deck135 that are NOT deck-specific — they are
qwen misreading the general guide, so they will recur across decks:

1. **Durdling / never developing.** In multiple games qwen passed whole turns with
   castable spells and open mana, and lost having done almost nothing. The current guide
   tells it to hold interaction and to "pass whenever there is nothing worth doing" — true
   for instants, but the weak model over-applies it to its OWN main phase and just stops
   developing.
2. **Refusing a beneficial cost.** qwen played fetchlands and then never cracked them —
   16 offered crack-actions passed in one game — apparently reading `[cost: Life, Tap,
   Sacrifice]` as a bad trade. The guide actively primes this: it lists "SACRIFICING one
   of your own permanents" and "paying life" as costs, and says "a valuable permanent is
   rarely worth trading for something lesser." A cost that is HOW A CARD WORKS gets refused.

Both are high-value to fix because they cost whole games and hit every deck with fetchlands,
sacrifice effects, life payments, or a proactive plan.

---

## Change 1 — Add a "develop every turn" imperative (the biggest win)

The guide has no clear order to use your mana proactively; "develop your mana, use your
mana efficiently each turn" is buried mid-sentence in line 5 and is easy to ignore.

**Edit line 5.** Current:
> General play principles: develop your mana, use your mana efficiently each turn, trade
> resources favorably, hold instant-speed interaction for the opponent's threats, and
> attack when the math favors you.

Replace with:
> General play principles: On your own main phase, DEVELOP every turn — play a land and
> spend your mana to advance your plan (add a threat, use a card-advantage effect, or
> improve your board); passing your turn with mana unused and castable cards in hand is a
> mistake. Trade resources favorably, hold instant-speed interaction for the opponent's
> threats, and attack when the math favors you.

This gives the weak model a default action ("do something on your turn") to counterbalance
the many "hold / wait / pass" instructions elsewhere.

## Change 2 — Stop the guide from teaching cost-refusal (the fetchland fix)

**Edit line 26**, the sentence:
> Many activated abilities cost more than mana - the action line states its cost in
> brackets, and a creature or other valuable permanent is rarely worth trading for
> something lesser.

Replace with:
> Many actions cost more than mana — the action line states its cost in brackets. A cost is
> often just HOW A CARD WORKS: a fetchland is meant to be sacrificed for a real land, and
> effects that pay life, discard, tap, or sacrifice a spent or expendable permanent are
> doing their job when you pay. Do not refuse a cost that advances your plan. Only decline
> when what you give up is clearly worth more than what you get — for example, don't
> sacrifice a strong creature for a minor effect.

This keeps the real caution (don't throw away a good permanent for little) but removes the
blanket bias that made qwen sit on uncracked fetchlands.

## Change 3 — Qualify the "pass priority" clause so it doesn't license durdling

**Edit line 19**, final sentence:
> Pass priority (choose 0) whenever there is nothing worth doing right now.

Replace with:
> Pass priority (choose 0) when you have nothing worth doing AND you gain nothing by acting
> now — but on your own main phase, prefer to develop (play a land, cast a spell) rather
> than pass with mana unused.

Small change; it stops the weak model from reading "pass whenever" as a general license.

---

## Keeping it short — what to cut to offset the additions

The three edits add roughly two sentences net. To stay lean:

- **Trim line 5's redundancy.** Its old comma-list ("trade resources favorably… attack when
  the math favors you") largely repeats the detailed STRATEGY bullets below (lines 17, 18,
  21). If length is tight, cut line 5 down to just the new develop-every-turn imperative and
  let the bullets carry the rest.
- **Consider trimming line 20 (bluffing / "control through the information you show them").**
  It is the most advanced idea in the guide — representing an instant you don't hold, making
  plays for their effect on opponent belief. A weak model cannot execute multi-layer bluffs
  reliably, and it's several lines long. Lower value-per-word than the two fixes above; a
  candidate to shorten to one line or drop if the guide needs room.

Net: with the line 5 trim, the additions are roughly length-neutral while converting two
buried, misread priors into two blunt, executable orders — which is what the weak pilot needs.
