# Skill: Writing a per-deck strategy guide for the qwen pilot (wave3 revision, deck 140)

## Revision notes (deck 140)

Field test: the wave2 skill produced the deployed deck140 guide; that guide FAILED in
play (0/6, five timeouts, 90% "Cast nothing" rate, all six payoff cards deployed 0-1
times in six games despite the guide screaming "ACT" and "DEPLOY Staff of Nin" at the
top). Every change below traces to that corpus.

1. **NEW failure tendency: false game-model beliefs.** The dominant loss mechanism was
   not a strategy gap — qwen holds provably WRONG beliefs about how the game works
   ("Mana in your pool: (none)" = can't cast; "must tap lands as a separate step first";
   "Rakdos's Return deals 0 damage" because it doesn't know X is chosen at cast). It
   declined an OFFERED, already-affordable Damnation three turns in a row and died at
   3 life. Added as a third design-against tendency, a Step 2 mining target (read the
   REPLY reasoning, not just the choices), and a Step 5 rule (teach the mechanic +
   refute the false belief verbatim).
2. **Exhortations always lose to a locally-plausible excuse.** "ACT / do not durdle /
   deploy the moment you are stable" was IN the failed guide — qwen agreed with it in
   its own PLAN text ("...before deploying a win condition like Staff of Nin") while
   picking "Cast nothing", because "stabilize first" always felt locally justified.
   Added the checklist-with-closed-exception pattern to Step 5: convert every
   exhortation into a take-the-first-that-appears priority list PLUS an explicit,
   narrow enumeration of when declining is allowed ("'Cast nothing' is allowed ONLY
   when ..."). An open-ended "don't be passive" is not executable; a closed exception
   list is.
3. **Quantify offered-vs-taken.** One ~15-line script over the translogs (count, per
   key card, how often "Cast <card>" appeared in the options vs. was chosen) produced
   the single most decisive diagnostic of the whole review: Staff of Nin 1/0, Rakdos's
   Return 26/0, Black Sun's Zenith 19/0, Elixir 55/0, Lightmine 26/0. Added to Step 2
   as a required extraction alongside the cast-nothing tally.
4. **Stale-PLAN poisoning.** The carried "YOUR PLAN (as you last stated it)" line
   re-injects a false belief every turn ("I have 5 tapped lands, untap next turn" —
   while the board showed all lands untapped). Added as a diagnostic pattern and a
   guide counter-move ("trust the CURRENT battlefield over your last plan").
5. **Counter the exact losing thought verbatim.** When a wrong reply recurs with the
   same wording across games, quote it and forbid it by name ("Never say 'Rakdos's
   Return deals 0 damage'"; "'I will stabilize first and deploy later' is the losing
   thought"). qwen pattern-matches its own phrases; a named forbidden thought lands
   where a paraphrased principle does not. Added to Step 5.
6. **Representation defects surface here — triage, don't absorb silently.** Log mining
   exposed prompt bugs ({X} in a mana cost renders as {l}; "Mana in your pool: (none)"
   actively misleads). These are channel-(c) fixes to report upstream, but the guide
   should ALSO carry a defensive line while the defect is live. Added to Step 2.
7. **Confirmed working (kept, now marked as proven):** the board→card selection TABLE
   (qwen followed the sweeper table correctly every time it cast at all); named-card
   imperatives; the DO-NOT list built only from observed misplays; engine-text
   verification (caught that the deck's own guide had mis-modeled Elixir).
8. **Mulligan rules must be keyed on a countable** (land count), not hand quality —
   qwen mulled a 3-land two-sweeper keep for a verbal reason ("too many Pyroclasms")
   into an unkeepable 1-land hand. Folded into Step 4's mulligan line.

---

A reusable method for producing one `{STRATEGY_GUIDE}` — the plain-text guide that
teaches the local qwen model to pilot ONE Wagic deck well. Follow it end to end for
any `deckN` in this project. This is craft method, not a guide for a specific deck.

## The one idea that governs everything: TWO models, and the executor is weak

- **You (the author)** are a strong model. You read the code, the card rules, and the
  game logs, and reason deeply about Magic. Spend that capability on ANALYSIS.
- **The executor is qwen** — a weaker model, under time pressure, that at runtime sees
  ONLY: the general system prompt + your guide + the live board + a numbered list of
  legal actions, and replies with ONE number. It does not get your reasoning. It cannot
  plan several moves ahead. It reasons LOCALLY — one decision at a time — and it
  pattern-matches your words against the board in front of it.

So: **do all the thinking, then hand qwen the CONCLUSIONS as orders.** A subtle,
"correct," hedged guide that qwen can't act on is worthless no matter how right it is.

Three failure tendencies to design against:
- **Locally-reasonable, globally-blind.** Each pick looks fine in isolation while the
  overall plan quietly never happens (the threat never gets deployed, the payoff never
  gets activated). Your job is to name the plan-critical action so plainly qwen takes it
  every time.
- **Shallow-heuristic defaults.** When the guide leaves a decision open, qwen falls back
  to bad defaults: pick by list position, by "biggest creature," by "a card is a card,"
  or by the safe/passive option when unsure. Every "consider / think about / weigh / it
  depends" is a sentence that hands the decision back to those defaults. Convert judgment
  into directives: **"DO X when Y," never "consider X."**
- **False game-model beliefs.** qwen can hold flatly WRONG beliefs about the game's
  mechanics and act on them with total confidence: that it lacks mana when the engine is
  offering it an affordable cast, that lands must be tapped as a separate step, that an
  X-spell "deals 0 damage" because it doesn't know X is chosen at cast time, that hand
  size is a resource worth skipping land drops for. No amount of strategy fixes a wrong
  physics model — the guide must TEACH the mechanic and refute the false belief in so
  many words. Hunt these in the reply reasoning (Step 2); they outrank strategy errors.

Write imperative, front-loaded, low-branching, concrete, evidence-driven. If a sentence
doesn't change which number qwen picks, cut it.

## Inputs to read, in this order

1. **The decklist** — `bin/Res/ai/baka/deckN.txt`. Format: `#NAME/#DESC/#HINT` headers,
   then `CardName (SET) * qty`; bare numbers are basic-land ids; the SET code is
   irrelevant. This tells you the archetype's intent.
   - **The `#HINT` lines are for the OTHER (heuristic Baka) AI, not qwen.** Read them for
     the designer's intent and any encoded combo/attack rule, but note where a hint would
     MISLEAD this deck's ideal play (e.g. a "don't attack with 0-power" hint is wrong for
     a battle-cry creature that should always attack). Don't propagate a hint blindly.

2. **The general system prompt** — `bin/Res/ai/gpt/system_prompt.txt`. EVERY deck already
   gets this, so READ THE CURRENT VERSION — it was recently strengthened and now covers, as
   UNIVERSAL priors: decide who's the beatdown (be the aggressor by default); develop/act every
   turn and deploy your win conditions (the anti-passivity fix); the instant-vs-non-instant
   timing scoping (hold instants for the opponent's turn; cast non-haste creatures and sorceries
   in your SECOND main by default); choose targets and cards by IMPACT, not by first/cheapest/
   biggest-stats; complete your synergies (activate your permanents, keep mana open for your own
   key ability); treat burn/reach as a clock, not an answer to hoard; a lethal-math check;
   balanced blocking; cards-vs-tempo; don't overcommit into a sweeper; and pay costs that advance
   your plan. **Your guide LAYERS ON TOP — never RESTATE any of this; add only what is specific
   to THIS deck.**
   The general guide states these as PRINCIPLES; a weak pilot still won't apply an abstract
   principle to the specific card in front of it. So your per-deck job is the concrete
   INSTANTIATION, not a restatement: the general guide says "activate your permanents" — your
   guide says "EQUIP Cranial Plating turn 3, keep {1} open"; it says "choose by impact" — your
   guide says "counter their Wrath, not their mana dork"; it says "deploy your win condition" —
   your guide NAMES the finisher and the turn to cast it. Litmus test: if a line you're about to
   write is already true of EVERY deck, the general guide has it — cut it and write the
   named-card version instead. Separately, note where a general prior still MISLEADS this specific
   deck and must be overridden explicitly (see the logs step).
   ⚠ Know that an abstract prior ALREADY PRESENT in the system prompt provides near-zero
   protection at the decision point — the deployed deck140 guide AND the general prompt both
   said "deploy your win conditions / don't be passive," and the pilot still cast nothing 90%
   of the time. Presence of the principle is not evidence the behavior is handled; only the
   logs are.

3. **Card text from the ENGINE, not your memory** — for every non-basic card, pull its
   `text=` and `auto=` lines from `bin/Res/sets/primitives/*.txt` (grep by `name=` across
   `mtg.txt`, and also `planeswalkers.txt` / `borderline.txt` if present). **This is what
   the engine actually enforces and what the AI is shown. Oracle text you remember, the
   printed cardface, and the engine `auto=` script can all DISAGREE — the script wins.**
   Verify the exact numbers you will cite (mana cost, power/toughness, keywords, ETB,
   activated-ability costs, devotion/threshold counts, X values) against it. Note that
   hybrid mana ({B/R}, {R/W}) is castable with EITHER color, so a "gold" card may be
   mono-color-castable in this deck. Use `/usr/bin/grep` — bare `grep` here is a wrapper
   that can silently drop matches.

4. **The game logs** — the deck's self-play translogs, JSONL, one line per decision qwen
   made piloting THIS deck: `~/.Wagic/ai/gpt/selfplay-runs/<run>/*-<deck>-*.jsonl`. Fields:
   `kind` (ask|priority|attackers|blockers), `prompt` (the FULL board + numbered legal
   actions qwen actually saw), `reply` (its text), `choice` (the index it picked). (`options`
   is just a count — ignore it.) **This is your primary evidence for what qwen gets wrong.**

## Step 1 — Read the deck: archetype, engine, role, redundancy

Answer these before writing a word — they are the guide's spine:

- **Archetype & clock.** Aggro / tempo / midrange / control / combo / prison? How does it
  actually kill (fast evasive creatures + burn, a lock, a big finisher, a devotion drain)?
  Roughly how many turns is its clock ("kill by turn 4-6")?
- **The win engine / win condition.** Name the specific 1-3 cards that end games and the
  exact interaction that makes them win (Guttersnipe + Young Pyromancer turning every cheap
  spell into damage; a Vise + Howling Mine lock; Cranial Plating on an evasive body). A weak
  pilot will never find these unaided — the guide's spine is "set up, protect, and deploy the
  payoff." Guides teach the DECK'S engine, not general Magic.
- **Role by matchup.** State the DEFAULT posture (proactive/beatdown vs reactive/control) in
  one line, then the narrow condition that flips it. A creatureless answer-deck is essentially
  always control; a cheap-creature deck is usually the beatdown. Hand qwen the answer as
  IF-THEN keyed on the opponent archetype ("you are the beatdown unless the opponent is a
  faster aggro deck that's ahead on board — then trade to survive"); do NOT make it re-derive.
- **Redundancy vs scarcity — MEASURE it.** Count the deck's copies of each effect. A deck with
  12+ removal/sweep spells can spend them freely; a deck with one bomb must protect it.
  Redundancy flips the correct advice, so measure before you prescribe — advice right for a
  one-bomb deck is wrong for a fourteen-sweeper deck.
- **The curve / sequencing that matters.** What it wants to do turns 1-2-3-4, plus any hard
  ordering ("payoff before the cheap spells," "land before spells to keep mana open").
- **Mechanics inventory.** List every card whose OPERATION a weak pilot may not know: X-spells
  (X is chosen at cast; the payment is invisible in the option line), activated abilities that
  only fire from the priority seam, replacement effects, alternative costs. Each of these is a
  candidate "teach the mechanic" line — confirm against the logs in Step 2 whether qwen
  actually stumbles on it.

## Step 2 — Mine the logs: find the deciding lever (this is where the value is)

A rule that corrects an OBSERVED mistake is worth ten generic tips. Do not theorize about
qwen's play — read what it did, then separate three things a guess would blur: **bad play
vs. never-had-the-option vs. bad hand (variance).** The `prompt` shows exactly what qwen was
OFFERED, so this is fact, not guess — and only the first two are yours to fix.

**Extract, don't eyeball.** Files are large; script a compact digest over the JSONL that
prints, per decision: the `kind`, the phase, life totals, hand, both battlefields, the
numbered options, and — crucially — the TEXT of the CHOSEN action (regex the `choice`-indexed
line out of the `prompt`). A ~30-line Python loop does this. Now you can read a whole game as
the sequence of what qwen actually did. Sample ACROSS games and across all decision kinds.

**Two required corpus-wide tables (cheap to script, highest signal per line):**
- **Offered-vs-taken, per key card.** For each plan-critical card, count how many times a
  "Cast <card>" (or activate) line appeared in the options vs. how many times it was chosen,
  across ALL the deck's games. A row like "Rakdos's Return: offered 26, taken 0" or "Staff of
  Nin: offered 1, taken 0" is a complete indictment in one line and tells you exactly which
  cards the guide must force. It also separates never-DRAWN (variance, not your problem) from
  never-TAKEN (your problem).
- **Cast-nothing rate + reason classification.** Tally the fraction of casting decisions
  answered "Cast nothing," then CLASSIFY the reply reasoning into buckets (regex the reply
  text): claimed-no-mana, holding/waiting, nothing-worth-casting, other. The bucket sizes tell
  you whether the leak is a mechanics delusion, timidity, or a real hand problem — three
  different guide fixes.

**Read the REPLY reasoning, not only the choices.** The replies are where false game-model
beliefs show up in qwen's own words ("all my lands are untapped but I have no mana in my
pool yet"; "Rakdos's Return deals 0 damage"). When the same wrong sentence recurs across
turns or games, capture it VERBATIM — you will refute it by name in the guide.

**By decision kind, what to inspect:**
- **ask** — casting, targeting, land drops, mulligan, modal menus. Watch sequencing/tempo (is
  it deploying threats or durdling on support spells?), removal/burn fired at the wrong target
  or too early, and whether it ever targets ITSELF.
- **priority** — activated abilities (equip, tap-for-value, cracking fetches, mana, instants).
  **THE most overlooked seam.** Plan-critical actions often hide here (e.g. "Equip with Cranial
  Plating [cost: {1}]", "crack fetchland") and qwen passes them. Check whether the deck's engine/
  payoff ability is EVER actually being activated. Corollary: a near-empty priority log is
  itself a finding — for deck140 it meant the permanents whose abilities live there were never
  deployed at all.
- **attackers / blockers** — is it playing its role? A beatdown deck that declares "none" over
  and over, or a control deck that chump-races, is misplaying.
- **target choices** (removal, discard/hand-strip) — where the biggest gaps show. qwen picks by
  superficial cues, not impact: stripping a 1/1 mana dork or a mana rock while a sweeper/counter/
  bomb sits in the same hand. Judge each pick: was the highest-impact target available, and did
  it take it?

**The single highest-value move: isolate the ONE lever.** Compare the games qwen played WELL
against the ones it LOST and find the one repeated difference. (For deck110, every good game
equipped Cranial Plating early and swung; every bad game cast Platings, never equipped them,
tapped out, and durdled — that single delta became Rule #1. For deck135, a "crack fetchland"
action was offered 16 times in a game and passed 16 times; qwen made no mana, cast nothing, and
lost — that became Rule #1. For deck140, the lever was a MECHANICS delusion: it declined
offered, affordable casts believing it had no mana — no strategy rule could fire until that
belief was corrected.) Most decks have one such lever. Find it before you write a word.

**Diagnostic patterns to hunt for (weight the guide toward the ones that recur):**
- **False mechanics beliefs** — the reply asserts something about the game that is untrue
  (can't afford an OFFERED cast; must tap lands as a separate step; an X-spell has a fixed,
  zero, or unknowable X; skipping a land drop preserves a resource). These override everything:
  fix the belief first, strategy second.
- **Offered-many-times, passed-every-time** — a capability qwen doesn't understand it should use.
- **Cast-nothing / durdle rate.** TALLY the fraction of casting decisions where qwen chose "cast
  nothing," and list which cards it NEVER cast across all games. (Deck140's pilot passed 90% of
  casting decisions and cast its card engine and all three finishers ZERO times across six games
  — that one statistic defined the whole guide.) Whole turns of passing with castable cards and
  open mana = the deck isn't developing.
- **Stale-plan poisoning.** The prompt carries qwen's own last PLAN line, and qwen trusts it
  over the current board — a wrong belief written into one plan re-arms itself every turn
  ("I have 5 tapped lands" carried across turns while the board showed them untapped). Look for
  plans that contradict the CURRENT SITUATION they ride with. Guide counter-move: "trust the
  current battlefield over your last plan," stated explicitly.
- **The deferred-forever payoff.** The reply AGREES the payoff should be deployed "once stable /
  later" while declining it now — and "later" never arrives (deck140 named Staff of Nin as its
  win condition in the very reply that declined casting it). An exhortation to act cannot beat
  this; only a checklist with a closed exception can (see Step 5).
- **Hoarding — answers/threats that pile up UNUSED in hand** while the opponent stays alive.
  Over-holding is as real a failure as over-firing. (Deck109 sat on Pillar of Flame / Searing
  Spear as if they were reactive removal, when they were reach it should have thrown at the face —
  "burn is damage, not removal.")
- **Failure to close — end-state sweep.** Print each game's final life totals; a game where the
  opponent's life PLATEAUED for many turns while qwen had gas is a failure to close. In a
  timeout-heavy corpus this is the first thing to check: a 40-minute cap hit with life totals
  frozen near 20-20 is a durdle-stall, and the deck most likely to durdle (control) is the
  deck whose guide must carry the strongest deploy rules.
- **Missing lethal.** Sum a turn's potential damage (attackers + payoff/keyword triggers + burn to
  face) and check whether qwen had lethal and didn't take it.
- **Reactive spells fired into nothing** — a counter on an empty stack, a combat trick on an empty
  board, an instant with no worthwhile target.
- **Wrong modal/menu picks** — color choices, X values, overload-vs-single. (Deck131 named red
  instead of blue every time.)
- **Inconsistency** — the SAME decision made differently across games (unleash taken here, declined
  there) means qwen has no stable rule; give it one.
- **Generic-prior misfires** — a universal prior that is WRONG for THIS specific deck. (The old
  blanket passivity and cost-refusal misfires are now fixed in the general guide; what remains are
  deck-specific overrides — e.g. a fourteen-sweeper deck that should spend freely against the generic
  sweeper caution, or a deck whose payoff must break the second-main-phase default, or a no-instants
  deck for which "keep mana open for interaction" is pure poison.) Name the prior and override it
  explicitly with the deck-specific rule.
- **Cost-line misreads.** Any deck action whose cost LOOKS bad but is routine (fetch sacrifice, life
  payment, phyrexian mana, sac-for-value) needs an explicit "this cost is the point — always pay it."
- **Engine realities that gate the plan.** qwen can't equip if it taps out (no {1} left), can't
  attack profitably with an unequipped 0-power body. These become concrete "keep a land open" rules.
- **Subtle leaks in decks qwen already plays "okay"** (aggro especially — it curves out and mostly
  looks fine). Hunt the quiet ones: burn hoarded vs thrown at the face; a go-wide payoff (Hellrider,
  a battalion enabler) out but qwen holding back "bad attacks" that are correct because of the
  trigger; not knowing when to stop trading and just race; the cheap repeated micro-choice it gets
  subtly wrong every game. A leak that costs a fraction of a turn every turn is invisible
  per-decision and decisive over a game.

**Triage representation defects out — but also defend against them.** Log mining is where
prompt-representation bugs surface: a mangled mana symbol ({X} rendering as {l}), a board line
that misleads ("Mana in your pool: (none)" reading as "you cannot pay"), an ambiguous option
label. These are NOT guide problems — record them for the upstream representation channel.
But while the defect is live, the guide should carry ONE defensive line neutralizing it
("if a Cast option is listed, you can pay for it") — a guide that ignores a live prompt bug
fails in play no matter how good its strategy is.

**Confirm before you accuse.** When a play looks wrong, read that decision's full `prompt` to check
what was actually legal / on the stack. (Deck135's Force of Negation LOOKED like a reflexive misfire
until the prompt showed it was countering a real Cruel Edict — so it stayed OUT of the mistakes list.)
Never teach against a non-problem, and never list a "mistake" you didn't observe.

## Step 3 — Decide what goes in vs. what gets cut

Include a line ONLY if it changes a decision qwen will actually face and gets wrong (or would).
Rank candidates by **frequency × cost-to-winning:**
1. The fix for the biggest observed leak / the deciding lever (always first). If the lever is a
   false mechanics belief, the fix is a mechanics lesson + verbatim refutation, not a strategy rule.
2. The win engine and how to set it up, protect it, and deploy it.
3. Role + matchup flips (concrete, deck-specific).
4. Target-priority and removal/sweeper-selection rules.
5. Per-card timing for the cards that decide games; a lethal-math check if the deck closes with reach.
6. Mulligan, in a line or two.

**CUT:** anything the general prompt already covers; general Magic theory; card-by-card tours of
filler and vanilla creatures; deep multi-turn combos or mana micro-optimization qwen can't sequence
(the engine handles most mana); long matchup trees; hedged "it depends" prose; rare edge cases; and
anything you can't turn into a "do/don't with a named card." When unsure, CUT — length dilutes the
rules that matter, and an executable subset beats a correct essay a weak model skims.

## Step 4 — Structure the guide (front-loaded by impact)

qwen weights early text heavily and may not deeply read the tail. Order by what most changes its
play — NOT by turn order, card list, or card-type "importance." A reliable skeleton (adapt, don't
pad):

1. **Identity + role**, 2-4 lines: archetype, how it wins, the clock, who it's the beatdown against
   (default + the flip condition), and the posture ("kill the opponent, do not durdle").
2. **THE #1 RULE** — the fix for the single biggest leak / the deciding lever from Step 2, hammered
   hard in its own visually loud block at the very top: WHAT the action is, the exact option text
   qwen will SEE, its enabling constraint (mana to hold open, board to build first), and a one-clause
   WHY. Make it unmissable. If the lever is a false belief, the #1 rule is the corrected mechanic
   stated as fact, plus the forbidden thought quoted verbatim.
3. **THE ENGINE / how the deck wins** — name the finishers/engine cards and the exact sequence to
   make them go. Say to DEPLOY them; passive pilots never close on their own — and say it as a
   checklist with a closed exception, not an exhortation (Step 5).
4. **Standing operating rules / the core action loop** — develop-every-turn, protect-the-synergy,
   the turn-by-turn deployment priority.
5. **Key cards — one imperative line each**, ordered by how often each decides the game: what it does
   (from the verified engine text) and the single directive/target/phase that matters. Only cards with
   a non-obvious right line; skip vanilla creatures. Include a HOW-IT-OPERATES line for every X-spell
   and priority-seam ability the pilot provably fumbles.
6. **A lethal / math check** the pilot can run mechanically, if the deck closes with reach.
7. **Combat notes** — only if the deck has non-obvious combat (deathtouch/wither blockers, evasion,
   pump tricks, go-wide attacking rules).
8. **MULLIGAN** — one or two lines, keyed on a COUNTABLE (usually land count: "keep any hand with
   2-4 lands"), never on hand "quality" — qwen has mulled fine hands for verbal reasons ("too many
   sweepers") when left to judge quality.
9. **DO NOT** — a tight list of the 3-5 specific misplays you actually observed, each as a flat
   imperative phrased as the exact situation qwen was in. This closes the loop from Step 2 and is the
   highest-signal part of the guide.

Match the deck8 / deck44 example density and length (~one screen, roughly 40-70 lines). Longer is not
better — every extra line dilutes the early ones.

## Step 5 — Word it for the executor (non-negotiable style rules)

- **Imperative and directive.** "EQUIP the Plating." "Cast Wrath when the opponent has 2+ creatures."
  "ALWAYS attack with Signal Pest." Never "consider," "you might," "it can be good to," "weigh whether"
  — qwen does not act on hedges; it falls back to its bad default.
- **A checklist with a CLOSED exception beats any exhortation.** "ACT / do not durdle / deploy when
  stable" fails in play even when qwen agrees with it — at each decision it finds a locally-plausible
  excuse ("stabilize first," "keep answers open," "next turn") and defers forever. Convert every
  behavioral demand into (a) a priority list keyed to option text ("take the FIRST of these that
  appears: 1. Cast X, 2. Cast Y, ..."), plus (b) an explicit, narrow enumeration of the ONLY
  situation where declining is allowed ("'Cast nothing' is allowed ONLY when every offered spell is
  a sweeper AND the opponent has no creatures"). If declining is ever legitimate, the guide must say
  exactly when — otherwise qwen writes its own permission slip every turn.
- **Teach the mechanic qwen provably lacks — as fact, then refute the false belief verbatim.** When
  Step 2 found a wrong game-model belief, state the correct mechanic bluntly ("The game ONLY offers
  spells you can afford right now; lands tap by themselves when you cast") and then quote-and-forbid
  the recurring wrong thought ("NEVER pick 'Cast nothing' with the excuse 'I have no mana' — that
  excuse is always false when a cast is offered"; "Never say 'Rakdos's Return deals 0 damage' — YOU
  choose X when you cast it"). qwen pattern-matches its own phrasing; naming the forbidden sentence
  intercepts it where a paraphrased principle misses.
- **Defuse stale-plan poisoning when observed:** "Trust the CURRENT battlefield over your last PLAN —
  if the plan contradicts what the board shows, the plan is stale; ignore it."
- **Default + narrow, enumerable exceptions** — not open judgment and not a decision tree. Give the
  default action hard, then a short explicit list of the ONLY conditions that change it. If a rule needs
  three conditions, qwen will drop two.
- **Low branching; use a ranked list or a selection TABLE, not nested IF/THEN.** When a choice has
  options, hand qwen a priority order (1, 2, 3) with a one-clause reason each, or a board→card mapping
  it reads off ("one creature → Edict; many small → Pyroclasm; anything big → Wrath"). qwen follows a
  checklist; it flounders on "if/then/unless." Where a branch is unavoidable, key it on something qwen
  can SEE (opponent's life, a card on the battlefield, its own hand size), never a hidden read.
  (PROVEN: the deck140 sweeper table was followed correctly in every game where the pilot cast at all
  — tables survive contact with the executor; prose exhortations do not.)
- **Tie every rule to the in-game surface.** qwen sees option text and phases, not your synthesis.
  Write trigger→action and quote the phrasing the engine actually shows: "When you see 'Equip with
  Cranial Plating [cost {1}]', take it." Name cards EXACTLY as they appear in the action list.
- **Concrete, named, numeric.** Name the cards and their mana costs; name the opponent cards seen in
  the logs; state thresholds numerically and say what they turn on ("metalcraft = 3+ artifacts,"
  "2 damage kills toughness 2 or less," "attack when 3+ gets through," "hold removal for creatures 4
  power or bigger"). "Develop your board" / "use your removal wisely" is invisible to qwen;
  "cast at least 3 other black permanents first" / "counter their creature with Essence Scatter" is
  executable.
- **Give arithmetic it can run.** A lethal check as an explicit sum is gold — qwen can evaluate it
  against the board with no strategic insight: "damage = unblocked attacker power + (1 per attacker if
  Hellrider out) + burn to face; if ≥ opponent life, attack with everyone and fire all burn."
- **State the WHY in ONE clause, only for load-bearing rules** — so a rule that looks locally wrong is
  trusted anyway ("attack with the 0-power creature — its battle cry pumps the team"; "always pay the
  fetch sacrifice — that cost is the point"). qwen needs the move, not the theory.
- **Give explicit PERMISSION where the pilot is wrongly timid and explicit BRAKES where it is wrongly
  reckless.** Weak models need both stated. "Sweep proactively — it's fine to trade one card for two
  creatures" is as necessary as "NEVER chain two sweepers on one board."
- **Override the misleading generic prior BY NAME**, with a half-line of why, so the base prompt's prior
  doesn't win ("The general prompt says hold interaction — IGNORE that here; this burn is reach, throw
  it at the face"; "you have NO instants — never pass to 'keep mana open'").
- **Turn `#HINT` / `auto=` behaviors into one-liners the pilot can rely on** ("Blightsteel auto-attacks
  once it's in play — your only job is to CAST it").
- **Hard NEVER/ALWAYS for known bad defaults** ("NEVER strip their weakest card when a sweeper, counter,
  or bomb is in their hand"; "NEVER cast Guttersnipe with no burn to follow").
- **Short.** It rides in every prompt and competes with the live board for attention. Cut anything that
  fails the "does this change a chosen number?" test.

## Pitfalls

- **Writing for yourself, not qwen** — the subtlest trap. Nuanced, hedged, branch-heavy prose reads well
  to you and gets ignored or misread by the executor. If a line requires the reader to do the reasoning
  you did, qwen won't; spell out the conclusion.
- **Assuming the executor shares your game model** — the deck140 trap. You know options are
  affordability-filtered, lands tap automatically, and X is chosen at cast; qwen demonstrably did not,
  and every strategy rule downstream of the false belief was dead on arrival. Audit the mechanics
  (Step 1 inventory + Step 2 reply mining) before polishing strategy.
- **Exhorting instead of enumerating** — "ACT," "do not durdle," "deploy when stable" all shipped in a
  guide that produced a 90% cast-nothing rate. If the desired behavior isn't a checklist with a closed
  exception, the executor will excuse itself out of it one plausible turn at a time.
- **Repeating the general prompt** — wastes qwen's attention budget and buries the deck-specific signal.
  And do not TRUST the general prompt's coverage either: a principle being present there is not evidence
  the behavior happens — verify against the logs.
- **Card text from memory** instead of the engine `auto=`/`text=` — you'll teach a rule the engine
  doesn't implement.
- **Guessing at misplays** instead of reading the translogs — the real failures are often not the ones
  you'd predict; or fixing a one-off slip instead of the recurring pattern; or listing a "mistake" you
  never observed (invented problems waste attention and can steer qwen wrong).
- **Burying the lede** — the deciding rule below nice-to-know card notes; a correct rule in paragraph
  nine is a rule qwen skips.
- **Branch explosion** — every "but if…" is a fork qwen can get wrong; collapse to the best default,
  branch only when the default genuinely loses. Don't over-hedge exceptions until the default gets lost.
- **Symmetry blindness** — prescribing spend-freely or hoard without measuring the deck's redundancy.
- **Over-length** — a long guide buries its own most important line.
- **Swallowing a representation bug** — if the prompt misrenders a cost or a board fact, report it for
  the representation channel AND write the one-line defensive rule; don't silently work around it with
  strategy prose, and don't blame the pilot for it in the DO-NOT list without the defense in place.

## Definition of done (checklist)

- [ ] Every card claim matches the engine's `text=`/`auto=`, not memory.
- [ ] The #1 rule at the top fixes the single biggest leak you actually found in THIS deck's logs,
      in imperative form — and if that leak is a false mechanics belief, the rule teaches the mechanic
      and quotes the forbidden thought verbatim.
- [ ] The win condition/engine is named and told to be deployed, near the top, as a priority checklist
      with a closed "decline allowed ONLY when..." exception — not as an exhortation.
- [ ] Offered-vs-taken and cast-nothing-rate tables were extracted from the logs and the guide's rules
      trace to them.
- [ ] Role-by-matchup is stated as a default + flip condition, not left for the pilot to derive.
- [ ] Every X-spell and priority-seam ability the pilot fumbled has a how-it-operates line.
- [ ] A mechanical lethal/math check is present if the deck can close with reach.
- [ ] Every "DO NOT" traces to a real observed decision in the logs.
- [ ] Generic-prompt priors that misfire for this deck are explicitly overridden by name.
- [ ] Mulligan rule is keyed on a countable (land count), not hand quality.
- [ ] Any prompt-representation defect found while mining is recorded for the upstream channel AND
      neutralized by one defensive guide line.
- [ ] Every rule is imperative, concrete (named cards, real numbers), low-branching, and executable
      from the board + action list alone — no inference required.
- [ ] Nothing repeats the general prompt; it fits on roughly one screen (~40-70 lines).

## Output and handoff

Write the finished guide as PLAIN TEXT (it slots into `{STRATEGY_GUIDE}` verbatim). No markdown headers
are needed; short ALL-CAPS section labels read fine and help qwen find the part it needs. The deck8 /
deck44 guides are the format and density reference.

The draft is LLM-authored; the user does a Magic-judgment authorship pass and owns the final guide — so
surface the card-rules details you verified and flag anything you were unsure of. Where possible, VALIDATE
by real play, not by looking correct: re-run self-play with the guide in place and read the new logs —
did qwen start taking the plan-critical action? If not, the guide isn't blunt enough; make Rule #1 louder,
shorter, more directive, and iterate on that lever before adding anything else. Judge the re-run with the
same instruments you mined with: the offered-vs-taken table and the cast-nothing rate must MOVE, not just
the game results. Note: shipping the guide as a Res asset needs `git add -f` (`bin/Res` is gitignored).
End by naming, in your own words, the deck's plan and the single biggest piloting problem the guide is
built to fix.
