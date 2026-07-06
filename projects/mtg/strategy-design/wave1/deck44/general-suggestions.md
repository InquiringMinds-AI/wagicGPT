# Suggested improvements to the GENERAL strategy guide (system_prompt.txt)

Scope: the shared prior every deck gets. Grounded in systemic failures I saw qwen
make piloting deck44 that are NOT deck-specific — they'd hurt any deck. The general
guide is well-reasoned for a strong reader, but it is read by the WEAK executor, and
several of its (correct) nuances get over-generalized into bad play. The theme of
these edits: the guide currently leans control/patience, and a weak model turns that
into passivity. Net length is roughly neutral — one addition is paid for by trimming
an advanced paragraph that actively misleads qwen.

---

## Suggestion A — FIX THE PASSIVITY BIAS (highest priority)

**Observed failure:** In multiple games qwen held full grips of counters/removal on
an EMPTY board and chose "Cast nothing" turn after turn, building no clock and losing
on the back foot. It generalized the guide's "hold instants," "waiting costs
nothing," and "pass priority (0) whenever there is nothing worth doing" to its ENTIRE
hand — including creatures and sorceries it should have been deploying. This is the
single biggest systemic misplay and it isn't unique to deck44; any proactive deck
loses this way.

**Root cause in the text:** Line 19 tells the model to hold and wait without
distinguishing INSTANTS (correctly held for the opponent's turn) from
CREATURES/SORCERIES (which can only be cast on your own main phase, so holding them is
pure waste). A weak model can't be trusted to make that distinction implicitly.

**Change line 19.** Current:
> Act at the latest useful moment. Hold instants, removal, and tricks until you must
> use them or until the opponent's turn or end step - waiting costs nothing and gives
> you information. Play a land before your spells for mana flexibility. When you
> expect the opponent to hold an answer, prefer committing a lesser threat before
> your best card. Pass priority (choose 0) whenever there is nothing worth doing right
> now.

Replace with:
> On YOUR main phase, DEVELOP first: play a land, then cast a creature or threat if
> you can. An empty board loses - do not sit on a hand of playable threats. "Hold"
> and "wait" apply to INSTANTS (counters, removal, tricks) - keep THOSE for the
> opponent's turn or end step, because you can cast them anytime. They do NOT mean
> holding your creatures and sorceries: those cast only on your own main phase, so a
> turn you don't develop is a turn thrown away. Play a land before your spells for
> flexibility, and when you expect an answer, commit a lesser threat before your best
> one. Choose 0 (pass) only when you truly have nothing worth casting - never as a
> substitute for developing your board.

**Why it's worth the words:** it directly overwrites the exact mistake that lost
deck44 games and the words it misread. This is the change I'd make first.

---

## Suggestion B — Give a concrete beatdown DEFAULT

**Observed failure:** The "decide who is the beatdown" logic (line 17) is sound but
abstract — it asks the model to compare clocks, which the weak executor doesn't do
reliably, so it defaults to durdling (the safe-feeling passive line).

**Change:** append one sentence to the end of line 17:
> If you cannot clearly tell who is the beatdown, be the aggressor by default: deploy
> threats and attack. A passive empty board loses on its own; force the opponent to
> answer you.

**Why:** costs one line, converts an under-executed judgment call into a safe default
that biases the weak model away from its worst failure mode.

---

## Suggestion C — Sharpen removal/counter TARGETING with a threshold

**Observed failure:** qwen fired Go for the Throat (premium removal) at a 2/2 Icehide
Golem — spending a one-use card on a trivial target. The guide flags "firing removal
or tricks too early or at the wrong target" (line 23) but only as an abstract item in
a long error list, which the weak model doesn't operationalize.

**Change:** add one concrete rule to line 18 (the cards-and-mana paragraph):
> Spend one-use removal and counters on the opponent's BIGGEST or most dangerous
> threat, not the first or smallest creature you see - killing a 2/2 is wasted if a
> bigger threat follows. EXCEPTION: when a faster deck is racing you, kill whatever is
> attacking you now.

**Why:** a numeric/comparative rule ("biggest, not first/smallest") is executable by a
weak model in a way "wrong target" is not; the exception prevents it from hoarding
removal to death against aggro.

---

## Suggestion D — TRIM the bluffing/information paragraph (pays for the above)

**Problem:** Line 20 (representing instants, bluffing with an empty hand, making plays
"for their effect on what the opponent believes") is advanced, high-branching content
that a weak executor cannot use well — and worse, its line "Leaving mana open
represents an instant... even when you hold nothing" actively REINFORCES the passivity
failure from Suggestion A by rewarding not-casting. High cost in attention, low yield,
and partly counterproductive for this executor.

**Change:** replace the entire line 20 paragraph with one sentence:
> Leaving mana open represents an instant and can deter an attack - but only bluff when
> you already have a board worth protecting; never skip developing just to "represent"
> something.

**Why:** keeps the one useful kernel, removes the mislead, and reclaims the length
spent on Suggestion A so the guide stays short.

---

## Left intentionally unchanged
The turn-structure/rules preamble (lines 1-5), the combat-arithmetic paragraph (21),
the sweeper caution (22), and the closing cost/benefit instruction (26-27) are sound
and I saw no systemic failure against them. I'd resist adding anything else — the
guide's length is itself a cost to the weak reader, and these four edits target the
failures that actually decided games.
