# Skill: Writing a per-deck strategy guide

## What this is
A repeatable method for writing the `{STRATEGY_GUIDE}` text that teaches the runtime
qwen model to pilot ONE deck. Follow it for any deck in this project. The output is a
plain-text guide that layers ON TOP of the general system prompt (`Res/ai/gpt/system_prompt.txt`)
— it adds what is deck-specific and never repeats the general Magic advice.

## The one constraint that governs everything: WHO executes this
The pilot is **qwen** — a small model, under time pressure, that sees your guide + live
board + a numbered action list and replies with ONE number. It is much weaker than you.
It does not infer, plan several turns ahead, or reconcile a subtle guide with a messy
board. Write for THAT reader:
- **Imperative, not advisory.** "DO X." "ALWAYS X." "NEVER X." Not "consider," "you may
  want to," "it can be good to." Every soft verb is a rule qwen will drop.
- **Front-loaded.** The most game-deciding instruction goes FIRST, in its own loud block.
  qwen weights the top of the guide most; a crucial rule buried at line 40 is invisible.
- **Low-branching.** Give the default line, then at most one or two named exceptions. A
  guide with five conditional forks is a guide qwen executes randomly.
- **Concrete.** Name the exact card. Name the exact plan. Where possible, name the action
  text qwen will SEE in its menu (e.g. `search basic land`, `Put in Play`, `Cast X`), so
  it can map your rule onto the numbered option in front of it.
- **Short.** Every extra sentence dilutes the load-bearing ones. Cut anything qwen can't act on.

If a guide reads like Magic theory, rewrite it as orders. A guide qwen can't execute is worthless.

## Method — four phases

### Phase 1 — Read the deck as the ENGINE sees it (ground truth, not your Magic memory)
1. Read the decklist: `bin/Res/ai/baka/deckN.txt` (`#NAME/#DESC/#HINT`, then `Card (SET) * qty`;
   bare numbers = basic-land ids; SET code is irrelevant).
2. For every nonland card, read its real `text=`/`auto=` from the primitives
   (`bin/Res/sets/primitives/mtg.txt`, or grep the set files if absent). **Oracle text ≠
   printed cardface ≠ engine script — the `text=`/`auto=` line is what the AI and engine
   actually use.** Verify power/toughness, keywords, ETB, and activated-ability costs there.
   Do not trust your memory of the card; cards get templated oddly and some are custom.
3. From that, state to yourself: the archetype, the game plan (how this deck actually
   wins), the mana base (colors, ramp, fixing, fetchlands), and which cards are the engine
   vs. filler. Identify SYNERGIES the deck is built around (e.g. snow-permanent count,
   artifact count, graveyard) — these are what the guide must protect.

### Phase 2 — Read the games to find how qwen ACTUALLY mispilots this deck
This is the highest-value phase. The guide's biggest win is almost always fixing a
recurring MECHANICAL misplay you can only find by reading the logs.
- Logs: the deck's self-play JSONL (`~/.Wagic/ai/gpt/selfplay-runs/<run>/*-<deck>-*.jsonl`).
  Each line = one decision: `{kind: ask|priority|attackers|blockers, prompt, reply, choice}`.
  The `prompt` holds the exact board + numbered `Your legal actions` qwen saw; `choice` is
  what it picked. `ask` = mulligan / casting (FindCardToPlay) / modal menus; `priority` =
  hold-priority actions (activated abilities, cracking fetches, instants); `attackers`/`blockers`
  = combat.
- Don't eyeball raw files. Script an extraction that, for every decision, prints the CHOSEN
  action's line text (regex the `choice`-numbered line out of the prompt) plus the phase. Now
  you can see the whole game as a sequence of what qwen did.
- Then look specifically for:
  - **Actions offered many times and passed every time** — a capability qwen doesn't
    understand it should use. (deck135: the "crack fetchland" action was offered 16 times in
    a game and passed 16 times; qwen never cracked a fetch, made no mana, cast nothing, and
    durdled to a loss. That one finding became RULE #1 of the guide.)
  - **Whole turns of passing** with castable cards and open mana → the deck isn't developing.
  - **Cards cast at the wrong time** (e.g. a flash creature dumped on the main phase; an
    instant fired with no worthwhile target).
  - **Cost-line misreads.** qwen sees `[cost: Life, Tap, Sacrifice]` and the general prompt's
    "a valuable permanent is rarely worth trading" and wrongly concludes it shouldn't pay. Any
    deck action whose cost LOOKS bad but is routine (fetch sacrifice, life payment, phyrexian
    mana, sac-for-value) needs an explicit "this cost is the point, always pay it" rule.
- Confirm before you accuse. When a play looks wrong, read that decision's full prompt to
  check what was actually on the stack / legal. (deck135: Force of Negation *looked* like a
  reflexive misfire until the prompt showed it was countering a real Cruel Edict / Searing
  Spear — so it stayed out of the "mistakes" list. Don't teach against a non-problem.)

### Phase 3 — Decide the role and the priorities
- **Who is the beatdown?** State the deck's DEFAULT role (proactive/beatdown vs
  reactive/control) in one line, then the one or two matchup shifts that flip it (vs aggro:
  defend; vs control: pressure). Keep it to named matchup buckets, not a case analysis.
- **Rank the failure modes by cost to winning.** The mechanical game-losing bug (Phase 2)
  outranks everything. Card-by-card finesse is last. Order the guide by that ranking.

### Phase 4 — Write the guide, in this shape
Use `deck8_strategy.txt` and the deck135 guide as models. Recommended structure:
1. **Identity line(s)** — archetype, how it wins, and its default posture (proactive or
   grindy). Two or three sentences.
2. **THE #1 RULE** — the single most important, most-violated mechanical instruction, in its
   own visually loud block at the very top (deck135 put fetch-cracking here). If the game
   logs revealed a game-losing habit, this is where its corrective goes.
3. **Standing operating rules** — develop-every-turn / hold-up-interaction / protect-the-
   synergy. A few blunt imperatives.
4. **Key cards** — ordered by how often each decides the game (not by mana cost, not by deck
   order). For each: what it does (from the verified engine text) and the ONE line of how to
   use it. Cut cards that need no instruction.
5. **Role / matchups** — default role + the one-or-two flips.
6. **Combat notes** — only if the deck has non-obvious combat (deathtouch blockers, evasion,
   pump tricks).
7. **Mistakes to avoid** — a short bulleted list drawn DIRECTLY from the misplays you saw in
   the logs, phrased as "DO NOT X." This closes the loop from Phase 2.

## What goes in vs. what gets cut
- **In:** anything deck-specific qwen would get wrong without being told — synergy payoffs,
  non-obvious card uses, sequencing that matters, cost-lines it will misread, and the exact
  corrective for each misplay the logs showed.
- **Cut:** general Magic advice already in the system prompt (combat math, card/tempo
  theory, "hold instants"); card explanations qwen doesn't need to pilot well; anything you
  couldn't turn into an order; long lists of situational exceptions.
- **Where the general prompt's priors actively MISLEAD this deck, override them explicitly.**
  The general strategy is a set of priors; a specific deck can contradict them (deck135's
  fetch-sacrifice vs. the generic "don't trade valuable permanents"). Name the conflict and
  give the deck-specific rule; don't assume qwen will resolve it.

## Guide-writing pitfalls
- Teaching Magic instead of giving orders. (Biggest one.)
- Burying the decisive rule below nice-to-know card notes.
- Branching too much — qwen needs a default line far more than a decision tree.
- Referring to a card by a nickname or a mechanic qwen can't match to its menu; use the
  card's real name and, when useful, the literal action text it will see.
- Writing card advice from your memory of the card instead of its verified `text=`/`auto=`.
- Listing a "mistake to avoid" you never actually observed — invented problems waste the
  reader's attention and can steer it wrong.
- Over-length. If it's longer than roughly the deck8/deck135 guides, you're including
  theory qwen won't use.

## Definition of done
- Every card claim matches the engine's `text=`/`auto=`.
- The top block is the single most game-deciding rule, in imperative form.
- Every "mistake to avoid" traces to a real decision in the logs.
- Every instruction is an order a weaker model can execute from the board in front of it,
  with no inference required.
