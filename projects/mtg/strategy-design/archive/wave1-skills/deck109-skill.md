# Skill: Writing a per-deck strategy guide for the qwen pilot

A reusable method for writing the `{STRATEGY_GUIDE}` text that teaches the local
qwen model to pilot ONE deck well. This is a craft document, not a guide for any
single deck. Follow it end to end for any `deckN`.

## The one idea that governs everything

There are TWO models involved and they are not the same.

- **You** (the author): a strong model. You can hold branches, weigh lines, reason
  about the whole game tree.
- **qwen** (the executor): weaker, under time pressure, sees only the current board +
  a numbered action list, and picks one number. It does NOT re-derive strategy each
  turn. It pattern-matches your words against the situation in front of it.

Every decision about what to write and how to word it serves ONE goal: **qwen, given
one board state and no memory of your reasoning, does the right thing.** A subtle,
"correct," hedged guide that qwen can't act on is worthless. Write imperative,
low-branching, front-loaded, concrete instructions. "DO X when Y," never "consider
X" or "think about whether." If a sentence doesn't change what number qwen picks, cut
it.

## Inputs to read (in this order)

1. **The decklist**: `bin/Res/ai/baka/deckN.txt`. `#NAME/#DESC/#HINT` headers, then
   `CardName (SET) * qty`; bare numbers are basic lands; the SET code is irrelevant.
   The `#HINT` combo lines sometimes name the deck's intended engine - read them.
2. **The general system prompt**: `bin/Res/ai/gpt/system_prompt.txt`. Every deck
   already gets this. Your guide LAYERS ON TOP - do not repeat it. More important:
   find where its GENERIC advice will MISLEAD this specific deck (see below).
3. **Card text - from the engine, not memory**: `bin/Res/sets/primitives/mtg.txt`.
   Grep each card by `name=`; read its `text=` and `auto=` lines. This is what the
   engine enforces and what the AI sees. Oracle text you remember, the printed
   cardface, and the `auto=` script can all disagree - the script wins. Note hybrid
   mana ({B/R}, {R/W}) is castable with either color, so a "gold" card may be
   mono-color-castable in this deck.
4. **The game logs**: `~/.Wagic/ai/gpt/selfplay-runs/<pool>/*-ai_baka_deckN-*.jsonl`.
   One JSONL line per decision qwen made piloting THIS deck. Fields: `kind`
   (ask|priority|attackers|blockers), `prompt` (full board + legal actions it saw),
   `reply` (its text), `choice` (index it picked). `options` is just a count, ignore
   it. These are your evidence for what qwen actually does wrong. Read them.

## Step 1 - Read the deck: archetype, role, engine

Answer these four before writing a word:

- **Archetype & clock**: aggro / tempo / midrange / control / combo-prison? How does
  it actually kill - fast creatures + burn, evasion, a lock, a big finisher? How many
  turns is its clock?
- **Who is the beatdown, by matchup?** Compare this deck's clock to each opponent
  archetype it faces. Aggro is the beatdown against almost everything; control is the
  beatdown against almost nothing. State the DEFAULT role and the narrow condition
  that flips it (e.g. "you are the beatdown unless the opponent is a faster aggro deck
  that's ahead on board - then trade to not die"). Do not make the pilot re-derive
  this; hand it the answer.
- **The win engine** - the specific interaction the deck is built around (Hellrider
  making a wide board's every attacker ping the face; a Vise+Mine lock; a lord that
  turns walls into an army). Guides teach the DECK'S engine, not general Magic. If the
  deck has a payoff card, the guide's spine is "set up and protect the payoff."
- **The curve / sequencing** - what it wants to do on turns 1-2-3-4.

## Step 2 - Read the logs: find the leaks (this is where the value is)

Do not theorize about qwen's play - read what it did. Efficient method: script over
the JSONL, print for each record the `kind`, `choice`, and the tail of `prompt` from
the last `Your life:` onward (that holds the board + the numbered legal actions).
Focus your reading on:

- **attackers / blockers records** - combat is where games are won and lost.
- **target records** ("Choose the target for X") - where removal/burn goes.
- **casting (ask) records with multiple real options** - sequencing and hoarding.
- **unleash / modal / mulligan** decisions - cheap, high-frequency, easy to get
  wrong consistently.

What to look for:

- **End-state sweep**: print the final `Your life` / `Opponent life` of each game.
  Games where the opponent's life PLATEAUED for many turns while qwen had gas = a
  failure to close. Compare the games it won convincingly to the games it stalled -
  the difference is usually one repeated mistake.
- **What accumulated UNUSED in hand.** If burn / removal / threats piled up in hand
  across many turns while the opponent stayed alive, qwen is hoarding. This was
  deck109's biggest leak: it sat on Pillar/Searing Spear as if they were reactive
  removal, when they were reach it should have thrown at the face.
- **Inconsistency**: the SAME decision made differently across games (unleash taken in
  one game, declined in another) tells you qwen has no stable rule - so give it one.
- **Generic-prior misfires**: the general prompt says "hold instants and removal until
  you must use them." A weak model over-applies that and holds burn forever. Wherever
  the general advice is WRONG for this deck, your guide must explicitly override it and
  say why in one line.

### Finding SUBTLE leaks in a deck qwen already plays "okay"

Aggro is the archetype the weak pilot handles best - it attacks, it curves out, it
mostly looks fine. So the leaks are quiet and you must hunt them:

- **Reach / burn sequencing**: is burn going face when it should, or being hoarded /
  spent on irrelevant creatures? Is the pilot missing that some spells are damage, not
  removal?
- **Go-wide payoffs**: with a payoff like Hellrider or a Battalion enabler out, does it
  attack with EVERYTHING (including small creatures and tokens into blockers, because
  the trigger/keyword makes that correct), or does it hold back "bad attacks"?
- **Knowing when to stop trading and just race** - does it chump-hold creatures on
  defense when it should be turning them sideways?
- **Missing lethal**: add up a turn's potential damage (attackers + payoff triggers +
  burn to face) and check whether qwen had lethal available and didn't take it.
- **The cheap repeated choice** it gets subtly wrong every game (unleash, which land,
  which token to sacrifice).

A leak that costs a fraction of a turn every turn is invisible per-decision and
decisive over a game. Those are the ones worth a rule.

## Step 3 - Structure the guide (front-loaded by impact)

Order sections so the highest-win-rate-impact instruction is FIRST. qwen weights early
text more, and a guide may be read under pressure - put the thing that most changes
outcomes at the top.

A reliable skeleton (adapt, don't fill blindly):

1. **Identity + role**, 2-4 lines: archetype, how it kills, who it's the beatdown
   against, and the posture ("kill the opponent, do not durdle / do not trade evenly").
2. **THE #1 RULE** - the fix for the single biggest leak you found in the logs, stated
   as a hard directive with its narrow exceptions. (For deck109: "burn is damage, not
   removal - throw it at the face; redirect to a creature ONLY when [two named cases].")
3. **The core action loop** - what to do every turn (attack with everything; deploy the
   cheapest threat; etc.).
4. **A lethal / math check** the pilot can execute mechanically when the opponent is
   low (see wording below).
5. **Key cards - how to use each**, named, with mana cost and the one line that
   matters for piloting it. Only cards with a non-obvious right line; skip vanilla
   creatures.
6. **Mulligan** in one or two lines: what to keep, what to ship.
7. **DO NOT** list: the 3-5 mistakes you actually observed, as flat imperatives.

## Step 4 - Word it for qwen

- **Imperative and directive.** "Attack every turn with everything." Not "you should
  usually consider attacking."
- **DEFAULT + narrow exceptions**, not open judgment. Give the default action, then an
  explicit short list of the ONLY conditions that change it. qwen executes a checklist;
  it flounders on "weigh the situation."
- **Low branching.** One rule per idea. If a rule needs five conditions, you've pushed
  author-work onto the executor - simplify or split.
- **Concrete.** Name the cards and their mana costs. "Cast Pillar of Flame ({R}) at the
  opponent," not "use your removal appropriately."
- **Give arithmetic it can run.** A lethal check as an explicit sum is gold, because
  qwen can evaluate it against the board without strategic insight:
  `damage = unblocked attacker power + (1 per attacker if Hellrider out) + burn to
  face; if >= opponent life, attack with everyone and fire all burn.`
- **Encode overrides of the general prompt explicitly**, with a half-line of why, so
  the generic prior doesn't win.
- **Short.** Every line competes for the pilot's attention. A tight one-page guide
  beats a thorough two-page one. Cut anything that doesn't change a chosen number.

## Prioritize: what goes in vs. what gets cut

IN: the biggest observed leak's fix (first); the win engine and how to set it up; the
role-by-matchup call; the lethal check; the 3-5 real DO-NOTs from the logs; the
non-obvious line on each key card.

CUT: anything in the general prompt already; general Magic theory; vanilla-creature
explanations; long matchup trees; hedged "it depends" prose; any card interaction qwen
will almost never face. When in doubt, cut - a guide qwen finishes and remembers beats
one it skims.

## Pitfalls

- **Writing for yourself, not qwen.** Nuanced correctness qwen can't execute is wasted.
- **Repeating the general prompt** - dilutes the deck-specific signal.
- **"Consider / think about / weigh"** - these do not tell qwen what number to pick.
- **Card text from memory** - verify against `auto=`/`text=` in primitives.
- **Burying the lede** - the most important instruction must be near the top.
- **Over-hedging exceptions** so the default gets lost. State the default hard; keep
  exceptions short and enumerable.
- **Teaching lines qwen already plays well** while missing the quiet leak that actually
  loses games. Spend your words where the logs show error, not where they show fine.

## Before you finish - checklist

- [ ] Card text grounded in primitives `auto=`/`text=`, not memory.
- [ ] The #1 rule fixes the biggest leak you found in THIS deck's logs.
- [ ] Role-by-matchup stated as a default + flip condition, not left to the pilot.
- [ ] A mechanical lethal/math check is present if the deck can close with reach.
- [ ] Every rule is imperative, concrete (named cards), and low-branching.
- [ ] Generic-prompt priors that misfire for this deck are explicitly overridden.
- [ ] It fits on roughly one page; nothing in it fails the "does this change a chosen
      number?" test.
