# Skill: Writing a per-deck strategy guide (wagicGPT)

A method for producing the `{STRATEGY_GUIDE}` text that teaches the runtime
LLM to pilot ONE deck. Follow it for any deck in this project. This is a
craft procedure, not a guide for a specific deck.

## The one fact that governs everything

There are TWO models. YOU (the author, a strong model) read the deck, the
card rules, and the games. The EXECUTOR is qwen — a weaker model, under time
pressure, that at runtime sees only: a general strategy prompt + your guide +
the live board + a numbered action list, and must reply with one number. It
does not get your reasoning, cannot plan several moves, and defaults to the
safe/passive option when unsure. Every wording choice below exists to survive
that gap. A guide qwen cannot execute is worth nothing, no matter how correct.

Write for qwen, not to show your own analysis. Imperative, low-branching,
front-loaded, concrete. "DO X" not "consider X."

## Step 1 — Read the deck as an engine, not a list

Read `Res/ai/baka/deckN.txt` (the `#NAME/#DESC/#HINT` headers name the
archetype and often encode intended combos/attack rules). Pull each nonland
card's `text=`/`auto=` line from `Res/sets/primitives/mtg.txt` — this is what
the engine actually enforces, which can differ from remembered oracle text and
from the printed cardface. Verify anything load-bearing against the `auto=`
script.

Then answer, for yourself, before writing anything:
- What is the deck's WIN CONDITION? Name the specific cards that end games.
  Many decks (control especially) have only 2-3, and a weak pilot will never
  find them unaided.
- What is the deck's ROLE — beatdown or control — and does it flip by
  matchup or board state? A creatureless answer-deck is essentially always
  control; a cheap-creature deck is usually the beatdown. State it plainly.
- What are the KEY DECISIONS this deck forces (when to sweep, when to hold a
  trick, when to deploy the threat, what to race)? These become the guide.
- What is REDUNDANT? Count effects. A deck with 12+ removal/sweep spells can
  spend them freely; a deck with one bomb must protect it. Redundancy flips
  the correct advice, so measure it.

## Step 2 — Read the games as evidence, not vibes

The logs are JSONL in the selfplay-runs directory, one line per decision:
`{kind, prompt, reply, choice}`. The `prompt` holds the full board + the
numbered legal actions the pilot saw; `choice` is what it picked. Diagnose
from these records FIRST — never theorize a misplay you have not seen.

Practical extraction (small files, do it directly):
- Parse each record's board summary (life, hands, battlefields) and the
  numbered options, and print choice vs options compactly so you can scan a
  whole game. A ~30-line python loop over the files does this.
- TALLY, don't eyeball. The single most revealing number I found: the
  fraction of casting decisions where qwen chose "cast nothing," and which
  cards it NEVER cast across all games. Deck140's pilot passed 82% of casting
  decisions and cast its card-engine and all three finishers ZERO times —
  that one statistic defined the whole guide. Compute the analogous numbers
  for your deck.
- Look for the RECURRING shape, not one-off slips: passivity (hoarding
  answers/threats), wrong tool for the board (a 2-damage sweep into a big
  creature), never deploying the win condition, racing when behind, firing
  interaction too early, mulligan errors. Note which are frequent — those are
  what the guide must fix.
- If a decision kind is absent from the logs (e.g. this deck rarely reaches
  combat), don't burn guide space on it. Weight the guide toward the
  decisions the pilot actually faces and fails.

The general prompt at `Res/ai/gpt/system_prompt.txt` already teaches generic
Magic (who's the beatdown, hold interaction, combat math). Read it so you
LAYER on top — add only deck-specific content — AND so you can spot where its
generic advice, over-applied by a weak model, becomes the failure. Deck140's
passivity was the general prompt's "hold interaction / pass when nothing
urgent" taken to an extreme; the guide had to explicitly counter it. This
"generic caution over-applied" pattern is common — look for it.

## Step 3 — Structure the guide, most important first

qwen weights early text more and may not deeply integrate the whole thing.
Order by what most changes its play:
1. One-line deck identity + role.
2. THE #1 rule — the single behavior that most often decides the deck's games
   (usually the exact correction to the biggest misplay you measured). Make it
   unmissable and give it the "why" briefly.
3. The core decisions, as concrete triggers (see Step 4).
4. How the deck WINS — name the finishers/engines and say to deploy them.
   Passive pilots never close on their own.
5. Matchup/role adjustments, kept short.
6. A DO-NOT list that names the specific observed mistakes.

Match the deck8_strategy.txt example's density and length (~40-70 lines).
Longer is not better — every extra line dilutes the early ones.

## Step 4 — Word it so qwen can execute

- IMPERATIVE and directive: "Cast Wrath when the opponent has 2+ creatures."
  Not "Wrath is good against multiple creatures" (qwen won't convert an
  observation into an action).
- LOW BRANCHING: give a decision as a short mapping from board state to the
  card to play. A selection TABLE ("one creature -> Edict; many small ->
  Pyroclasm; anything big -> Wrath") is ideal — the pilot matches the board
  and reads off the answer. Avoid nested conditionals.
- CONCRETE: name the cards, name the numbers ("2 damage kills toughness 2 or
  less"), name the opponent cards seen in the logs. Never "your removal,"
  "a threat," "develop your board" — say which card.
- FRONT-LOAD the trigger, then the reason: "FIRE A SWEEPER when they have 2+
  creatures — you have 14, you will draw more." The reason earns compliance
  but the action comes first.
- Give explicit PERMISSION where the pilot is wrongly timid, and explicit
  BRAKES where it is wrongly reckless. Weak models need both stated. "Sweep
  proactively, it is fine to trade one card for two creatures" is as important
  as "never chain two sweepers on one board."
- COUNTER the specific over-applied generic rule by name when it is the cause.
- Turn the deck's `#HINT`/auto behaviors into one-liners the pilot can rely on
  ("Blightsteel auto-attacks once in play — your only job is to CAST it").

## Step 5 — Prioritize in vs. cut

Keep: the win condition, the 2-3 decisions the pilot repeatedly gets wrong,
the sweeper/removal selection rule, the deploy-your-threats push, the biggest
matchup swing. Cut: exhaustive card-by-card text (only cover cards with a
non-obvious use), deep sequencing/mana micro-optimization (high branching,
low payoff for a weak pilot, and the engine handles most mana), rare edge
cases, and anything the general prompt already covers well. When unsure,
prefer fewer, sharper rules over completeness — an executable subset beats a
correct essay.

## Pitfalls

- Writing for yourself: analysis prose the pilot can't act on. Every sentence
  must change a choice.
- Verifying card behavior from memory instead of the `auto=` script.
- Guessing misplays instead of reading the translog; or fixing a one-off slip
  instead of the recurring pattern.
- Over-respecting the general prompt (repeating it) OR ignoring where it
  actively misleads a weak model — do neither; layer and correct.
- Length creep. A long guide buries its own most important line.
- Symmetry blindness: the deck's redundancy/scarcity dictates whether to
  spend or hoard — advice that is right for a one-bomb deck is wrong for a
  fourteen-sweeper deck. Measure before you prescribe.

## Output

Plain text, slots into `{STRATEGY_GUIDE}`. No markdown headers needed in the
guide itself; the deck8 example is the format reference. The guide teaches ONE
deck's engine — not general Magic, which the pilot already has.
