# Skill: Writing a per-deck strategy guide for the wagicGPT LLM opponent

A reusable methodology for authoring the `{STRATEGY_GUIDE}` text that teaches the
runtime pilot to play ONE deck well. Follow this for any deck in the project.

## 1. Know who executes the guide (this constraint drives everything)

The guide is NOT read by you. It is read by **qwen** — a smaller, weaker model,
under time pressure, that sees the general system prompt + your guide + the live
board + a numbered list of legal actions, and replies with one number. It cannot
plan several moves, hold much nuance, or infer your intent. Write for THAT reader:

- **Imperative, not deliberative.** "DO X." / "HOLD X until Y." / "NEVER X."
  Not "consider whether", "you might", "it can be good to". Every sentence should
  tell it what to do or not do.
- **Low-branching.** One clear default line, plus at most a couple of named
  exceptions. Avoid decision trees more than one level deep. If a rule needs
  three conditions, qwen will drop two of them.
- **Front-loaded.** The single most important thing about the deck goes FIRST.
  qwen weights early text heavily and may not act on late text. Order by
  importance, not by turn order or card list.
- **Concrete.** Name the actual cards, the actual mana costs, the actual trigger.
  "Cast Downsize on their attacker" beats "use your removal on threats." qwen
  matches card names against the action list; abstractions don't map.
- **Short.** Long guides dilute the load-bearing rules. Cut anything qwen can't
  act on. A tight one-screen guide it follows beats a thorough one it ignores.

## 2. Read the deck (build the model you'll teach)

Read `bin/Res/ai/baka/deckN.txt`. Then, for EVERY nonland card, read its real
`text=` and `auto=` lines from `bin/Res/sets/primitives/mtg.txt`
(`grep -iA6 "^name=CardName$"`). **Do not trust card memory** — the engine's
`auto=` script is what the AI and engine actually enforce, and it can differ from
oracle text or the printed card. Confirm each card's true effect, cost, speed
(instant vs sorcery), and targeting from the script.

From that, answer for yourself (this becomes the guide's spine):
- **Archetype in one line** (aggro / tempo / control / combo / prison / midrange).
- **The engine / win condition**: the 1-3 cards that actually win, and the
  specific interaction that makes them win. Deck131's engine was Guttersnipe +
  Young Pyromancer turning every cheap spell into damage + a body. deck8's was
  Black Vise + Howling Mine. Find this; it is the heart of the guide.
- **Role, and how it flips by matchup**: is this deck the beatdown or the
  controller? Against what does it flip? The engine's general prompt already
  teaches "decide who's the beatdown" abstractly — your job is to make it
  CONCRETE for this deck ("vs slow decks you are the beatdown; vs aggro you
  survive and block").
- **Sequencing that matters**: any "play A before B" ordering (payoff before
  spells; Vise before Mine; land before spells for mana-up).
- **The mana-cost tiers and land base** (e.g. mostly-blue → Prism Ring names blue).

## 3. Mine the game logs (this is what makes the guide specific and true)

Read the deck's self-play translogs
(`~/.Wagic/ai/gpt/selfplay-runs/<run>/*-<deck>-*.jsonl`). Each line is one
decision: `{kind, prompt (full board + numbered legal actions), reply, choice}`.
Sample across all games and decisions — write a small Python digest that prints,
per decision: phase, life totals, the chosen action's TEXT (match `choice` to the
numbered option in the prompt), and hand/board. Then hunt for **misplays** — this
is the highest-value input, because a rule that corrects an observed mistake is
worth ten generic tips. Look specifically for:

- **Reactive spells fired into nothing**: counters/tricks cast with no valid
  target or off-combat (deck131 cast a -4/-0 combat trick on an empty board and a
  counter on an empty stack). → a DO-NOT + a "hold until" rule.
- **Held answers that never fire**: sitting on a blowout (Aetherize, a sweeper)
  while losing. Over-holding is as real a failure as over-firing.
- **Passive death / durdling**: casting do-nothing permanents and passing while
  life drops. → "deploy a threat or interact; do one."
- **Missed sequencing**: casting the payoff after the spells, or spells with no
  payoff in play. → the ordering rule.
- **Wrong modal/menu picks**: color choices, X values, overload vs single-target
  (deck131 named red instead of blue every time). → a flat concrete instruction.
- **Bad combat**: declining all blocks vs aggro, tapping out into a swing-back,
  gang-blocking. → block/attack rules.

Verify a suspected misplay by reading that decision's full prompt (what it was
actually OFFERED) before writing a rule about it — sometimes the "misplay" was the
only legal/sane option, or a flooded hand (variance, not a teachable error).

## 4. Choose what goes in vs. gets cut

Prioritize ruthlessly; the guide is short. Rank candidate content by:
1. The engine / win condition (always in, always first).
2. Rules that fix an OBSERVED misplay (high value — qwen provably needs them).
3. Role + matchup flips (concrete, deck-specific).
4. Per-card timing for the cards that decide games.
5. Mulligan.

CUT: anything the general system prompt already says in the abstract (don't repeat
"trade favorably" — your guide LAYERS specifics on top); card-by-card rules for
filler cards; deep multi-condition lines; strategy qwen can't execute (bluffing
subtlety, long-term resource math). If a rule can't be turned into a "do/don't
with a named card," it probably shouldn't be in a qwen guide.

## 5. Structure (a reliable skeleton)

1. **Deck identity** — 2-4 sentences: archetype, how it wins, the one posture note.
2. **THE ENGINE** — the win-condition cards and the exact sequence to make them go.
3. **WHO IS THE BEATDOWN** — beatdown vs control, split by matchup type, concrete.
4. **KEY CARDS AND WHEN TO CAST THEM** — one tight bullet per decisive card:
   what it does, and the specific moment/target to use it (name the phase).
5. **TIMING RULES** — when this deck acts (e.g. "hold instants, act on THEIR turn").
6. **DO NOT** — a bullet list of the observed misplays, phrased as bans.
7. **MULLIGAN** — what to keep, what to ship.

Adapt the headings to the deck, but keep the ENGINE-first, DO-NOT-list shape —
the DO-NOT list is where log-mined mistakes land and it's the highest-signal part.

## 6. Pitfalls to avoid

- **Repeating the general prompt.** It already teaches beatdown theory, holding
  interaction, combat math. Add deck-specifics, don't restate priors.
- **Writing for yourself.** Nuanced conditional lines you'd execute, qwen won't.
  When in doubt, make it more imperative and less branchy.
- **Trusting card memory over the script.** Re-read `auto=`/`text=` for every card.
- **Card-cataloguing.** A guide that lists every card's rules text with no lines
  or priorities is just the decklist again. Teach the PLAN.
- **Rules with no card names.** "Remove their threats" is unactionable; "Counter
  their creature with Essence Scatter" is executable.
- **Ignoring the logs.** Generic Magic advice without log evidence misses the
  actual mistakes THIS pilot makes with THIS deck.

## 7. Final check before you hand it off

- Could a weak model act on every sentence using only the board + action list?
- Is the win condition the first thing it reads?
- Does every DO-NOT correspond to a real observed (or clearly likely) mistake?
- Are all cards named exactly as they appear in the action list?
- Did you cut everything the general prompt already covers?
- Is it about one screen long?

Write the finished guide as plain text (it slots into `{STRATEGY_GUIDE}`), then
hand the draft off for the user's authorship/Magic-rules pass — guides are
LLM-drafted, user-owned.
