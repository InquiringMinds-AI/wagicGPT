# Skill: Writing a per-deck strategy guide for the qwen pilot

A procedure for producing one `{STRATEGY_GUIDE}` — the plain-text guide that
teaches the local qwen model to pilot ONE deck well. Works for any deck in
this project. You (the author) are a capable model; the EXECUTOR is not. Every
rule below serves one goal: a guide qwen actually follows in the moment.

## The core constraint (never lose sight of this)

The guide is executed by a weaker model, under time pressure, that reads the
guide + a big board-state prompt + a numbered action list and replies with ONE
number. It decides ONE action at a time and reasons LOCALLY — it does not hold
your whole game plan in its head. So the guide must convert strategy into
per-decision REFLEXES: "when you see X, do Y." Prose about the deck's theory is
worthless if it doesn't tell qwen which numbered action to pick.

Its dominant failure mode is locally-reasonable-but-globally-blind: each pick
looks fine in isolation while the overall plan quietly never happens. Your job
is to name the plan-critical action so plainly that qwen takes it every time.

## Step 1 — Read the inputs (in this order)

1. **Decklist** (`bin/Res/ai/baka/deckN.txt`): `#NAME/#DESC/#HINT` headers,
   then `CardName (SET) * qty`. Ignore SET codes; bare numbers are basic lands.
   The `#HINT` lines are for the OTHER (heuristic) AI, not qwen — read them for
   intent, but note where they MISLEAD for this deck (e.g. a
   "don't-attack-with-0-power" hint is wrong for a battle-cry creature that
   should always attack). Get the archetype, the curve, the payoff cards.
2. **General system prompt** (`Res/ai/gpt/system_prompt.txt`): every deck gets
   this. It already teaches beatdown-vs-control, tempo, combat math, holding
   interaction. Your guide LAYERS ON TOP — never repeat it; add only what is
   specific to THIS deck.
3. **Card texts from the engine** (`Res/sets/primitives/mtg.txt`, `text=` /
   `auto=` lines). AUTHORITATIVE — what the AI sees and the engine enforces.
   Oracle text, the printed cardface, and the engine script can all disagree;
   trust the engine script. Confirm costs, thresholds, and how abilities are
   worded, because you will quote them to qwen.
4. **The game logs** (`selfplay-runs/.../*-deckNNN-*.jsonl`): one JSONL line per
   decision this deck's pilot made — `{kind, prompt, reply, choice}`. This is
   your primary evidence. Sample across ALL games and all decision kinds.

## Step 2 — Mine the logs for the deciding lever

Each record's `prompt` holds the exact board + numbered options qwen was
OFFERED; `choice` is what it took. Read them to separate three things a guess
would blur: bad play vs. never-had-the-option vs. bad hand.

Decision `kind`s and what to inspect:
- **ask** — casting, targeting, land drops. Watch sequencing/tempo: is it
  deploying threats, or durdling on support spells? Is it firing removal/burn
  at the wrong target or too early? Is it hitting ITSELF as a legal target?
- **priority** — activated abilities (equip, tap-for-value, mana). THE most
  overlooked seam. Plan-critical actions often hide here (e.g. "Equip with
  Cranial Plating targeting X [cost: {1}]") and qwen passes them. Check whether
  the deck's ENGINE/payoff ability is ever actually being activated.
- **attackers / blockers** — is it playing its role? A beatdown deck that
  chooses "none" repeatedly, or a control deck that chump-races, is misplaying.

The highest-value move: **compare the games it played well against the ones it
lost, and isolate the ONE difference.** For deck110 every good game equipped
Cranial Plating early and swung; every bad game cast Platings and never
equipped them, tapped out, and durdled with no board. That single delta becomes
Rule #1. Most decks have one such lever — find it before you write a word.

Also note engine realities that gate the plan: qwen can't equip if it taps out
(no {1} left), can't attack profitably with an unequipped 0-power body, etc.
These become concrete "keep a land open" style rules.

## Step 3 — Decide what goes in vs. gets cut

Include ONLY what changes a decision qwen actually faces and gets wrong (or
would). Rank by how often it comes up × how badly it loses when missed.

CUT: general Magic theory (the base prompt has it); multi-turn combos qwen
can't sequence; fine matchup nuance beyond a one-line role rule; card
descriptions that don't tell it to DO something; anything that makes the guide
long. A guide qwen won't finish reading is worse than a short sharp one. When
unsure, cut — length dilutes the rules that matter.

## Step 4 — Structure (front-loaded, most game-deciding first)

1. **Identity + role**, 1-3 sentences: the archetype, and whether it is the
   beatdown or the control deck, and the simple rule for when that flips. Give
   a rough clock ("kill by turn 4-6").
2. **Rule #1 — the deciding lever** from Step 2, hammered hard: WHAT the action
   is, WHY it wins/loses the game, the exact option text qwen will see, and the
   enabling constraint (mana to hold open, board to build first). This is the
   most important block; make it unmissable.
3. **Deployment / sequencing**: the curve and what to play when.
4. **Key cards**: ONE imperative line each — "ALWAYS attack with X", "TAP it
   every turn", "HOLD this for the kill". Name the card; state the action.
5. **Combat**: when to attack/block given the role.
6. **MISTAKES TO AVOID**: a short bulleted do-not list, each item a real misplay
   you observed in the logs. This is where you inoculate against the specific
   errors qwen made.

## Step 5 — Word it for qwen (the rules that make it executable)

- **Imperative, not deliberative.** "EQUIP the Plating." "ALWAYS attack with
  Signal Pest." Never "consider", "think about", "you may want to" — qwen does
  not act on hedges.
- **Low branching.** When a choice has options, give a PRIORITY ORDER (1, 2, 3)
  with a one-clause reason, not a decision tree. qwen follows a ranked list;
  it gets lost in "if/then/unless".
- **Tie each rule to the in-game surface.** qwen sees option text and phases,
  not your synthesis. Write the trigger→action: "When you see 'Equip with X
  [cost {1}]', take it." Quote the phrasing the engine actually shows.
- **Concrete, named, numeric.** Name the cards. State thresholds ("metalcraft =
  3+ artifacts") and which they turn on. No abstractions qwen must re-derive.
- **Front-load and keep it short.** Most important line first, in every section.
  Aim for the deck8/deck110 length (~40-60 lines), not a treatise.
- **State the WHY in one clause** for the load-bearing rules, so a rule that
  looks locally wrong ("attack with a 0-power creature") is trusted anyway
  ("its battle cry pumps the team; it can't be blocked").

## Pitfalls when authoring

- Repeating the general prompt instead of adding deck-specific reflexes.
- Writing the deck's theory instead of the actions — qwen needs verbs and
  numbers, not an essay on the archetype.
- Card text from your own memory rather than the engine script (they disagree;
  the engine wins).
- Decision trees and conditionals qwen can't hold — flatten to ranked lists.
- Burying the deciding lever mid-document, or drowning it in secondary tips.
- Length creep. Every extra sentence lowers the odds qwen follows the vital one.

## Ground truth check

The guide is validated by real play, not by looking correct. When possible,
re-run self-play with the guide in place and read the new logs: did qwen start
taking the plan-critical action? If not, the guide isn't blunt enough — make
Rule #1 louder, shorter, more directive. Iterate on that lever before adding
anything new.
