# Skill: Writing a per-deck strategy guide

A procedure for writing the `{STRATEGY_GUIDE}` text that teaches the qwen AI
opponent to pilot ONE Wagic deck well. Follow it for any deck. This is craft
method, not a guide for a specific deck.

## The one idea that governs everything

TWO different models are involved and you must never confuse them:

- **You (the author)** are a strong model. You read code, sample logs, reason
  about Magic deeply. Spend that capability on ANALYSIS.
- **The executor** is qwen — weaker, under time pressure, sees only the general
  prompt + your guide + the live board + a numbered action list, and replies
  with one number. It does NOT reason its way to your conclusions. It follows
  what is written, superficially, and it defaults to shallow heuristics (picks
  by list position, by "biggest creature", by "a card is a card") when the guide
  leaves a decision open.

So: **do all the thinking, then hand qwen the CONCLUSIONS as orders.** Every
sentence that says "consider", "think about", "weigh", "it depends" is a
sentence qwen cannot execute — it will fall back to its bad default. Convert
judgment into directives with concrete triggers. A guide qwen can't execute is
worthless no matter how correct.

## Inputs, and the order to read them

1. **Decklist**: `bin/Res/ai/baka/deckN.txt` (`#NAME/#DESC/#HINT`, then
   `CardName (SET) * qty`; bare numbers are basic lands; the SET code is
   irrelevant). This tells you the archetype's intent.
2. **Card rules text — GROUND TRUTH**: for every non-basic card, pull its
   `text=` and `auto=` line from `bin/Res/sets/primitives/*.txt`
   (`mtg.txt`, `planeswalkers.txt`, `borderline.txt` — grep all of them by
   `name=`). This is what the engine actually enforces and what the AI is shown.
   Oracle text you remember, the printed cardface, and the engine script can all
   DISAGREE — trust the script. Verify the exact numbers you will cite (mana
   value cutoffs, devotion counts, loyalty costs, triggers) against it.
3. **The general prompt**: `bin/Res/ai/gpt/system_prompt.txt`. Your guide LAYERS
   ON TOP of it. It already teaches generic Magic (who's the beatdown, hold
   instants, combat math, don't overcommit into sweepers). Do NOT repeat that.
   Add only what is SPECIFIC to this deck.
4. **The games**: the deck's self-play translogs (JSONL, one decision per line:
   `{kind, prompt, reply, choice}`; `kind` ∈ ask|priority|attackers|blockers).
   These are your evidence for what qwen actually gets wrong with THIS deck.

## Reading the games (this is where the real content comes from)

Do not theorize about misplays — read them. Script the logs; don't eyeball raw
JSONL. Extract, per decision: the phase, your hand, both battlefields, life
totals, the numbered legal actions, and the `choice`. Then look specifically at
the decisions where the deck's PLAN lives:

- **Target choices** (discard: options are opponent's-hand cards; removal:
  options are creatures). This is where the biggest gaps show. qwen tends to
  pick by superficial cues, not strategic impact — e.g. stripping a mana rock or
  a 1/1 mana dork while a sweeper or counterspell sits in the same hand. Judge
  each pick: was the highest-impact card available and did it take it?
- **Payoff timing** (the card the deck is built to cast: a devotion drain, a
  combo piece, an overrun). qwen fires payoffs early / into an empty board for a
  fraction of their value. Check the board state when it cast the payoff.
- **Casting sequence** — did it build the enabling board before the payoff, or
  durdle? Did it deploy threats vs control, or sit back?
- **Role** — in each matchup, was it the beatdown or the control, and did it
  play that role? Cross-reference the opponent's cards (visible in the log).
- **Attacks / blocks / holding interaction** — did it race a faster deck, tap
  out into danger, or fire held instants at the wrong time?

Every clear misplay you find becomes a specific `DO NOT` line in the guide,
phrased as the exact situation qwen was in. Concrete observed failures teach far
better than abstract advice.

## Deriving the guide's content

From the deck + card text + games, settle these before writing:

1. **The plan in one breath** — how this deck actually wins. Name the engine.
2. **Role by matchup** — beatdown vs control vs grind, as IF-THEN on what the
   opponent's deck contains. Roles flip; say when.
3. **The 3-8 cards that carry the deck** and the ONE rule each needs. Not every
   card — the load-bearing ones. For each: what it does (from the script) and
   the single directive that makes qwen use it right.
4. **The specific failures** the games exposed, turned into orders.

## Writing for the executor (the non-negotiable style rules)

- **Imperative, not advisory.** "Cast discard turn 1." not "It's often good to
  consider casting discard early."
- **Front-load.** Most important first: the win condition and the single most
  common/most punishing decision go at the TOP. qwen weights early text more and
  may not deeply read the tail.
- **Low branching.** Prefer one clear default line over a decision tree. Where a
  branch is unavoidable, make it a flat IF-THEN keyed on something qwen can SEE
  in the board state (opponent's life, a card on the battlefield, its hand
  size), not on hidden reads.
- **Concrete over general.** Name the cards. Name the targets. Give thresholds
  and numbers ("drain 5+", "mana value 2 or less", "at 10 or less life"). "Build
  your board" is unexecutable; "cast at least 3 other black permanents first" is.
- **Hard rules for the failure modes.** Where qwen has a known bad default, write
  an explicit NEVER/ALWAYS that overrides it ("NEVER cast X as your only Y";
  "NEVER strip their weakest card when a sweeper/counter/bomb is available").
- **Short.** It rides in every prompt and competes with the live board for
  attention. Cut anything the general prompt already covers or that doesn't
  change a decision.

## A structural template (adapt, don't pad)

```
Deck identity: <archetype + how it wins, 2-4 lines>
WHO YOU ARE BY MATCHUP: <beatdown / control / grind as IF-THEN, decide first>
<PAYOFF CARD>: what it does + the RULE for casting it well (front-loaded)
<PRIMARY ACTION LINE, e.g. discard/removal>: when to do it + TARGET PRIORITY
  rules per matchup + a NEVER line for the bad target
<KEY THREATS>: one directive each
<ENGINES / SUPPORT>: one directive each, incl. "don't cast with no fodder" type
  guards
SEQUENCING: the default turn-by-turn shape, land-before-spells, payoff timing
DO NOT: the observed misplays as a tight list of orders
```

Order sections by how often the decision comes up and how badly a mistake hurts,
not by the card types' "importance" in the abstract.

## Prioritize in / cut out

KEEP: the win condition; target-priority rules; payoff-timing rule; matchup role;
the specific observed failures; any line that changes a concrete decision qwen
faces.

CUT: generic Magic theory (the general prompt has it); card-by-card tours of
filler; flavor and history; hedged "it depends" prose; anything you cannot tie
to a decision qwen actually makes.

## Pitfalls (learned on this project)

- **Writing for yourself, not qwen.** The subtlest trap. If a line requires the
  reader to do the reasoning you did, qwen won't — spell out the conclusion.
- **Card-text drift.** Citing oracle/remembered text instead of the engine
  script; you'll teach a rule the engine doesn't implement. Grep primitives.
- **Guessing at misplays.** Read the translogs; the actual failures are often
  not the ones you'd predict, and they're the ones worth correcting.
- **Repeating the general prompt.** Wastes the qwen's attention budget and buries
  the deck-specific signal.
- **Burying the lede.** A correct rule in paragraph nine is a rule qwen skips.
- **Branch explosion.** Every "but if..." you add is a fork qwen can get wrong.
  Collapse to the best default; only branch when the default genuinely loses.

## Where the file goes / handoff

Write the guide as plain text (it fills `{STRATEGY_GUIDE}`). The draft is
LLM-authored; the user does a Magic-judgment authorship pass, so surface the
card-rules details you verified and flag anything you were unsure of. When the
guide ships as a Res asset it needs `git add -f` (`bin/Res` is gitignored).
