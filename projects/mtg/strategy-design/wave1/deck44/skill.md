# Skill: Writing a per-deck strategy guide for the qwen pilot

## What you are producing

A plain-text guide that slots into the `{STRATEGY_GUIDE}` placeholder of the AI
opponent's system prompt. At runtime qwen (a WEAKER model, under time pressure)
receives: a general strategy guide + YOUR per-deck guide + the live board + a
numbered list of legal actions, and replies with the number it picks. Your guide
teaches qwen to pilot ONE specific deck well.

**The executor constraint is everything.** You are not writing for a strong reader
who will infer your intent. You are writing for a weak model that will do the literal,
front-loaded, low-effort thing. A guide qwen cannot execute is worthless no matter how
correct it is. Every craft rule below serves that constraint.

## The core method (in order)

### 1. Read the deck and nail the archetype in one sentence
Read the decklist (`Res/ai/baka/deckN.txt`: `#NAME/#DESC/#HINT` headers, then
`CardName (SET) * qty`; bare numbers are basic lands; the SET code is irrelevant).
Before anything else, answer: **what is this deck's plan to win, and what is its win
condition?** Aggro-flyers? Prison/lock? Ramp-into-bomb? Combo? Midrange goodstuff?
The whole guide hangs on this. Note the `#HINT` lines — they often flag the deck's
intended combo or key play.

### 2. Ground every card in the ENGINE text, never your memory
Get the real rules text from the primitives/sets files:
`Res/sets/primitives/mtg.txt` first, else `grep -rli "^name=Card$" Res/sets/`.
Read the `text=` AND `auto=` lines and `power=/toughness=`. **Oracle text ≠ engine
script ≠ the printed cardface — they can disagree, and the AI/engine act on the
engine script.** A guide built on misremembered card text will mis-teach qwen. Use
`/usr/bin/grep` (bare `grep` here is a wrapper that can silently drop matches).

### 3. Read the general prompt so you LAYER, don't repeat
Read `Res/ai/gpt/system_prompt.txt`. It already teaches generic Magic: beatdown/
control decision, holding instants, combat math, not overextending into sweepers.
**Do not re-teach these.** Your guide adds only what is deck-specific: this deck's
plan, its card synergies, its role by matchup, and the corrections for how qwen
actually misplays THIS deck.

### 4. Read the game logs — this is what makes the guide real
Sample the deck's self-play translogs (`selfplay-runs/.../*-deckN-*.jsonl`; each line
is one decision: `{kind, prompt, reply, choice}`, kind ∈ ask|priority|attackers|
blockers). Files are large — extract compact views, don't read raw. For each
decision pull: life totals, hand, your battlefield, opponent battlefield, the
numbered options, and the `choice`. Sample ACROSS games and across decision kinds.

Look for the gap between what the deck WANTS to do and what qwen actually did:
- **Role/plan errors** (the big one): is qwen executing the archetype? The deck44
  review found qwen piloting a tempo deck as passive draw-go — holding a grip of
  counters/removal with an empty board, never deploying its threats, building no
  clock. That single systemic failure became the guide's front-loaded #1 rule.
- **Resource misuse**: firing premium removal/counters at trivial targets (deck44
  Go-for-the-Throat'd a 2/2); wasting mana on marginal activations.
- **Combat errors**: leaving attackers home, bad blocks, missing evasion.
- **Sequencing errors**: casting cards in the wrong order, ignoring an enabler that
  should come down first.
- Distinguish **bad hand vs bad play vs never-had-the-option** — the prompt shows
  exactly what qwen was offered, so this is fact, not guess. Only the last-two are
  yours to fix; a flooded hand isn't a guide problem.

Whatever systemic misplay you find, that correction leads the guide.

### 5. Decide what goes in vs. what gets cut
Include a line ONLY if it changes a decision qwen will actually face. Cut card
trivia, flavor, edge cases qwen will almost never hit, and anything the general
prompt already covers. When in doubt, cut — a short guide qwen reads and follows
beats a thorough one it skims. Prioritize by frequency-times-impact: the plays that
happen every game and decide games come first.

## How to structure the guide (front-loaded)

qwen weights the top. Order sections by how often they bite and how much they matter:

1. **Identity + role in 2-4 lines.** Archetype, win condition, and whether it is the
   beatdown or the control deck by default. Blunt.
2. **The #1 rule** — the single biggest correction from the logs, stated as a hard
   directive. This is the line that fixes the systemic misplay you found.
3. **Curve / sequencing** — what to play turn by turn, in priority order. Name cards.
4. **Combat / clock** — when and how to attack or block for THIS deck.
5. **Key cards — how to use each one that needs explaining.** One card, one directive
   about the play that matters (an enabler to land first, a protection effect to
   deploy, a blocker to keep home). Skip cards that need no explanation.
6. **Role by matchup** — how the plan shifts vs faster vs slower opponents. Keep it to
   2-3 concrete branches, not a matchup table.
7. **Mistakes to avoid** — a short bullet list of the exact misplays you saw in the
   logs, phrased as "Do NOT X."

## Wording rules (the executor constraint, concretely)

- **Imperative, not deliberative.** "DO X." "Cast the cheapest flyer." NOT "consider
  whether it might be advantageous to..." qwen does what you tell it, so tell it.
- **Low branching.** Give the default line, then at most one or two named exceptions.
  Every "it depends" is a decision you're handing to a weak model. Make the call for
  it and state the default; reserve branches for the few forks that truly flip.
- **Concrete: name the cards and name the play.** "Save Counterspell for their bomb,
  not a 2/2." Never "use removal wisely." Abstract advice is invisible to qwen.
- **Front-load within every section.** Most important sentence first.
- **Short.** Aim for something qwen reads in full every decision. Tight beats complete.
- **State thresholds and targets numerically** where you can ("attack when 3+ gets
  through," "hold removal for creatures 4 power or bigger") — qwen executes a number
  better than a vibe.
- **Give the WHY only when it changes the action**, and keep it to a clause. qwen
  doesn't need theory; it needs the move. "Cast Scion to give your faeries shroud so
  removal can't hit them" — the why justifies the directive in one breath.

## Pitfalls

- Writing for yourself, not for qwen: nuanced, hedged, branch-heavy prose reads well
  to you and gets ignored or misread by the executor. Re-read every line asking "will
  a weak model do the literal thing this says, and is that the right play?"
- Repeating the general prompt (wasted space, dilutes your deck-specific signal).
- Building on remembered card text instead of the engine's `text=`/`auto=`.
- Writing an idealized guide divorced from the logs — the logs tell you what qwen
  actually gets wrong; that is where your words earn their place.
- Burying the biggest correction in the middle. If qwen systemically misplays the
  deck one way, that fix is line one, not a footnote.
- Over-length. If you can't say it in a screen or so, you're including things that
  don't change a decision. Cut.

## Output

Write the guide as plain text (it fills `{STRATEGY_GUIDE}` verbatim). No markdown
headers required; short ALL-CAPS section labels read fine and help qwen find the part
it needs. End by naming, in your own summary, the deck's plan and the single biggest
piloting problem the guide is built to fix.
