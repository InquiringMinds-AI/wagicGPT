# Suggestions: improving the general strategy guide

Target file: `bin/Res/ai/gpt/system_prompt.txt`. These are systemic fixes for
failures I saw across deck133's games that are NOT deck-specific — they are how
the weak qwen executor mishandles decisions every deck faces. Line numbers refer
to the current file.

## The systemic failure

qwen does not value options by their effect on WINNING. It picks by surface cues
— list position, raw power/toughness, "a card is a card", "I can afford it now".
Two concrete instances, both generalizable:

- **Discard took the WEAKEST card** in the opponent's hand (a mana rock, a 1/1
  mana dork) while sweepers, counters, and real threats sat right beside it.
  Generalizes to every target choice: which card to strip, which creature to
  kill, which to block.
- **Cast the payoff for a fraction of its value** (Gray Merchant with no other
  black permanents = a 2-life drain instead of a game-swinging one). Generalizes
  to every card whose value scales with the board or game state.

The current prompt *does* gesture at both — line 23 buries "firing removal or
tricks too early or at the wrong target" in a comma-list of errors at the very
tail (lowest attention), and it is framed as a negative, not an executable
positive rule. qwen needs a prominent, positive directive.

## Change 1 (most important): add an IMPACT-selection rule, high in the list

**What:** insert a new bullet as the THIRD item in the STRATEGY list — right
after the "decide who is the beatdown" bullet (line 17), before "Two resources"
(line 18). Target/card selection is the single most frequent decision in a game,
so it belongs near the top where attention is highest, not in the tail error
list.

**Wording to insert:**

> - Choose by impact, not by convenience. Any time you pick a target or pick
>   which card to take, keep, or kill, rank the options by how much each one
>   changes who wins, and act on the one that matters most: take the opponent's
>   most dangerous card, kill the threat that actually beats you, keep the card
>   that does the most. Never default to the first option on the list, the
>   cheapest one, or the biggest raw power/toughness. A weak target or a weak
>   discard wastes the whole card.

This is imperative, low-branching, and names the exact bad defaults (list
position, cheapness, raw stats) so qwen overrides them. It covers discard,
removal, and blocking targets in one rule.

## Change 2: add a payoff-timing rule

**What:** add one bullet, next to the "Act at the latest useful moment" bullet
(line 19) — same theme (timing), so they cluster.

**Wording to insert:**

> - Cast each card when it does the most, not the moment you can afford it. If a
>   card's effect grows with your board or the game (it drains, pumps, or hits
>   for more as you have more), hold it until that payoff is real; casting it
>   early for a fraction of its value throws the card away.

Justification: payoffs that scale (devotion drains, go-wide anthems, X-spells,
overrun, graveyard payoffs) exist in many decks, and firing them early is a
recurring, game-losing error. One line earns its place; per-deck guides then
name the specific threshold.

## Change 3 (optional sharpening): make "hold instants" concrete

Line 5 says "hold instant-speed interaction for the opponent's threats" and line
19 says "Act at the latest useful moment" — yet qwen fired Fatal Push (an
instant) at sorcery speed on its own turn. The abstraction isn't landing. If you
want to reinforce it cheaply, append to line 19:

> ...Pass priority (choose 0) whenever there is nothing worth doing right now,
> and do not cast an instant or use removal on your own main phase if you could
> wait for the opponent's turn and see more first.

(Optional — per-deck guides can also carry this. Include only if space allows.)

## Keeping it short — what to cut/condense to offset

Adding two bullets should be paid for. Candidate: the "Control the opponent
through the information you show them" bullet (line 20). It is the longest,
most advanced item — deliberate bluffing with open mana you don't hold is a
strong-player concept the weak executor rarely executes and can misread into
bad passes. Condense it to one line:

> - Your untapped mana and open cards represent tricks even when your hand is
>   empty; leaving mana up can deter an attack. Back a bluff with the real card
>   when you have it.

That recovers roughly the space the two new bullets cost, so net length is about
flat. Net effect: two frequently-decisive, executable rules move to high-
attention positions; one rarely-executed advanced rule shrinks.

## Priority if only one change ships

Change 1 (impact selection). It is the root of the most damaging and most
frequent misplays, it is fully general, and it is a single high-value line.
