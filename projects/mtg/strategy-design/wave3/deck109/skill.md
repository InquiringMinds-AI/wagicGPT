# Skill: Writing a per-deck strategy guide for the qwen pilot

## Revision notes (deck 109)

Field-tested against the 20260709 corpus (6 games: 4 wins, 1 loss, 1 timeout) revising the
deployed deck-109 guide. Substantive changes below; each edited/added passage in the body is
tagged **[D109]** so a synthesis agent can find it.

1. **Read the `reply` field, not just prompt/choice** (Step 2, digest recipe). The corpus's
   single biggest lever — qwen believing "attacking taps my lands" and durdling every
   post-combat main phase — was visible ONLY in the model's stated reasoning, never in the
   board or the chosen option alone. The wave2 digest recipe omitted `reply`; it now requires it.
2. **Hunt for RULES DELUSIONS and counter them with perception procedures** (new diagnostic
   pattern + new style rule). qwen held stable FALSE beliefs about game rules ("attacking taps
   lands", "summoning-sick creature = don't cast it"), repeated across 4 of 6 games, costing a
   won game to timeout (turn-8 lethal in hand, 4 untapped Mountains, "I have no mana"). A guide
   can defeat a delusion the prompt can't fix, by giving a LOOK-AND-COUNT procedure ("count the
   Mountains NOT marked [tapped] — that is your mana") instead of restating the rule abstractly.
3. **Design named exceptions INTO absolutes — the executor obeys your absolutes into losses**
   (new failure tendency + Step 5 rule). Deck 109's only loss came from qwen OBEYING the old
   guide's #1 rule: it explicitly declined to burn Young Pyromancer, quoting the "burn goes
   face" rule in its plan, while two Pyromancers built the token army that killed it. An
   absolute a weak model will follow off a cliff needs its exception cards NAMED (opponent
   decklists are known — name the actual cards it will face).
4. **Guide-echo is attribution evidence** (Step 2). When qwen QUOTES the current guide's
   phrasing in its PLAN while misplaying, the guide caused the play — that's a guide bug, not a
   model bug. Conversely it proves exact guide wording propagates into the executor's plans:
   phrasing has real leverage in both directions. Revision passes must grep replies for echoes
   of the deployed guide's language.
5. **Localize the fix to the PHASE where the failure lives** (Step 4/5). Deck 109's paralysis
   was specifically in Main phase 2 after attacking. A checklist keyed to the visible phase
   label ("MAIN PHASE 2 CHECKLIST — run it every turn after combat") outperforms turn-generic
   principles the model must map onto the phase itself.
6. **Audit wins with a clock lens** (Step 2). All four wins closed 2-6 turns later than the
   hand allowed (e.g. exact burn lethal held for two extra turns). Compare earliest-possible
   kill vs actual kill per game; "won anyway" hides the same leak that loses the close ones.
7. **Never-drawn ≠ fine** (Step 2 caveat). Hellrider — the namesake payoff — was never drawn
   in six games. Zero log evidence about a card is VARIANCE, not validation; keep its guidance
   proportional and don't build Rule #1 on an unobserved card, but don't cut it either.
8. **Revision mode** (new note in Inputs). Wave2 was written for authoring fresh; when a guide
   is already deployed, that guide's text is itself evidence — read it as input #0 and
   attribute each observed misplay to a specific line (missing / wrong / too weak to override
   the general prompt's prior).

What the skill already got right and I leaned on hard: extract-don't-eyeball digests; the
"isolate the ONE lever" discipline (it found the mana delusion); the missed-lethal and hoarding
patterns (both fired); "confirm before you accuse" (unleash play was fine — stayed out of the
guide); every DO-NOT tracing to an observed decision; mechanical arithmetic checks; front-loading.

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

Failure tendencies to design against:
- **Locally-reasonable, globally-blind.** Each pick looks fine in isolation while the
  overall plan quietly never happens (the threat never gets deployed, the payoff never
  gets activated). Your job is to name the plan-critical action so plainly qwen takes it
  every time.
- **Shallow-heuristic defaults.** When the guide leaves a decision open, qwen falls back
  to bad defaults: pick by list position, by "biggest creature," by "a card is a card,"
  or by the safe/passive option when unsure. Every "consider / think about / weigh / it
  depends" is a sentence that hands the decision back to those defaults. Convert judgment
  into directives: **"DO X when Y," never "consider X."**
- **[D109] Obedient absolutism.** qwen FOLLOWS your hard rules — including into a loss.
  An absolute with an unstated exception is a trap you set yourself: deck 109's pilot
  quoted "burn is damage, not removal — face" in its plan while declining to kill the
  Young Pyromancer that beat it. Whenever you write NEVER/ALWAYS/DEFAULT, ask "what is
  the board where obeying this loses?" and NAME that exception (the opponent decklists
  in the matchup pool are known — name the actual cards). One named exception; not a
  judgment clause.
- **[D109] Rules delusions.** qwen holds stable FALSE beliefs about the game rules
  themselves (observed: "attacking taps my lands / my lands are still tapped from
  attacking"; "summoning sickness means casting this creature does nothing"). These
  recur across games, survive contradicting board text, and masquerade as passivity.
  The guide is the right place to defeat them — see the perception-procedure rule in
  Step 5.

Write imperative, front-loaded, low-branching, concrete, evidence-driven. If a sentence
doesn't change which number qwen picks, cut it.

## Inputs to read, in this order

**[D109] Revision mode:** if a guide for this deck is ALREADY DEPLOYED (it was in play when
the corpus was collected), read it as input #0 and treat its text as evidence. For every
misplay you find in the logs, ask which of its lines was missing, wrong, or too weak to
override a general-prompt prior — and grep the replies for its phrasing (guide-echo, Step 2).

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
   **[D109]** A prior can beat your override even when your guide addresses it: deck 109's
   wave2 guide said "do not hoard burn," yet qwen still held Searing Spear "for the
   opponent's tricks" (the general hold-instants prior winning). When logs show a prior
   overpowering an existing override, the fix is not more prose — it's naming the prior,
   naming the card, and stating the override as a flat rule at higher prominence ("the
   hold-instants rule DOES NOT apply to Searing Spear — it is damage on a clock").

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
   actions qwen actually saw), `reply` (its text — **[D109] read it; it states the model's
   reasoning and false beliefs, which the board and choice alone never reveal**), `choice`
   (the index it picked). (`options` is just a count — ignore it.) **This is your primary
   evidence for what qwen gets wrong.**

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

## Step 2 — Mine the logs: find the deciding lever (this is where the value is)

A rule that corrects an OBSERVED mistake is worth ten generic tips. Do not theorize about
qwen's play — read what it did, then separate three things a guess would blur: **bad play
vs. never-had-the-option vs. bad hand (variance).** The `prompt` shows exactly what qwen was
OFFERED, so this is fact, not guess — and only the first two are yours to fix.
**[D109]** Variance cuts the other way too: a key card that was NEVER DRAWN across the corpus
(deck 109's Hellrider, 0 draws in 6 games) gives you zero evidence for OR against its
handling. Keep its guidance, keep it proportional, and never build Rule #1 on a card the
logs never showed.

**Extract, don't eyeball.** Files are large; script a compact digest over the JSONL that
prints, per decision: the `kind`, the phase, life totals, hand, both battlefields, the
numbered options, the TEXT of the CHOSEN action (regex the `choice`-indexed line out of the
`prompt`), and **[D109] a snippet of the `reply`** — the stated plan is where false premises
("I have no mana", "it can't attack so casting it is pointless") and guide-echoes live. A
~30-line Python loop does this. Now you can read a whole game as the sequence of what qwen
actually did AND why it thought so. Sample ACROSS games and across all decision kinds.

**By decision kind, what to inspect:**
- **ask** — casting, targeting, land drops, mulligan, modal menus. Watch sequencing/tempo (is
  it deploying threats or durdling on support spells?), removal/burn fired at the wrong target
  or too early, and whether it ever targets ITSELF.
- **priority** — activated abilities (equip, tap-for-value, cracking fetches, mana, instants).
  **THE most overlooked seam.** Plan-critical actions often hide here (e.g. "Equip with Cranial
  Plating [cost: {1}]", "crack fetchland") and qwen passes them. Check whether the deck's engine/
  payoff ability is EVER actually being activated.
- **attackers / blockers** — is it playing its role? A beatdown deck that declares "none" over
  and over, or a control deck that chump-races, is misplaying.
- **target choices** (removal, discard/hand-strip) — where the biggest gaps show. qwen picks by
  superficial cues, not impact: stripping a 1/1 mana dork or a mana rock while a sweeper/counter/
  bomb sits in the same hand. Judge each pick: was the highest-impact target available, and did
  it take it? **[D109]** Also check WHOSE permanent it picked: deck 109's pilot once resolved
  its own burn onto its OWN token while its reply said "face." When the reply and the chosen
  option disagree, the pick was a menu error — countable, and worth a flat "NEVER pick a card
  from [your battlefield]" line if it recurs.

**The single highest-value move: isolate the ONE lever.** Compare the games qwen played WELL
against the ones it LOST and find the one repeated difference. (For deck110, every good game
equipped Cranial Plating early and swung; every bad game cast Platings, never equipped them,
tapped out, and durdled — that single delta became Rule #1. For deck135, a "crack fetchland"
action was offered 16 times in a game and passed 16 times; qwen made no mana, cast nothing, and
lost — that became Rule #1. **[D109]** For deck109, the lever wasn't in the choices at all —
it was a false premise in the replies: "my lands are tapped from attacking," which turned
every second main phase into "Cast nothing" and once left exact lethal in hand while a game
ran to the 40-minute timeout.) Most decks have one such lever. Find it before you write a word.

**Diagnostic patterns to hunt for (weight the guide toward the ones that recur):**
- **Offered-many-times, passed-every-time** — a capability qwen doesn't understand it should use.
- **Cast-nothing / durdle rate.** TALLY the fraction of casting decisions where qwen chose "cast
  nothing," and list which cards it NEVER cast across all games. (Deck140's pilot passed 82% of
  casting decisions and cast its card-engine and all three finishers ZERO times — that one
  statistic defined the whole guide.) Whole turns of passing with castable cards and open mana =
  the deck isn't developing.
- **[D109] Recurring false rule claims (rules delusions).** Grep the replies for the model's
  stated REASONS to do nothing, and check each against the board text in the same record. A
  claim that contradicts the visible board ("all my Mountains are tapped" while the battlefield
  line shows them untapped; "casting it does nothing — summoning sickness") and repeats across
  games is a delusion, and it is guide-fixable — see Step 5.
- **[D109] Guide-echo (revision mode).** Grep replies for phrases from the DEPLOYED guide. An
  echo attached to a good play validates the line; an echo attached to a misplay convicts it —
  the model obeyed you into the mistake (deck 109's pilot cited "saving the burn for the face"
  while sparing the Young Pyromancer that won the game). Either way it proves your exact wording
  reaches the executor's plans.
- **Hoarding — answers/threats that pile up UNUSED in hand** while the opponent stays alive.
  Over-holding is as real a failure as over-firing. (Deck109 sat on Pillar of Flame / Searing
  Spear as if they were reactive removal, when they were reach it should have thrown at the face —
  "burn is damage, not removal.")
- **Failure to close — end-state sweep.** Print each game's final life totals; a game where the
  opponent's life PLATEAUED for many turns while qwen had gas is a failure to close.
  **[D109] Run this clock audit on the WINS too:** for each win, find the earliest turn the hand
  + board could have killed and compare to the actual kill turn. Deck 109 won four games and
  every one closed 2-6 turns late for the same reason it timed out the fifth — "won anyway"
  conceals the leak that loses the close matchups and burns the corpus's wall-clock budget.
- **Missing lethal.** Sum a turn's potential damage (attackers + payoff/keyword triggers + burn to
  face) and check whether qwen had lethal and didn't take it. **[D109]** Check the SECOND main
  phase specifically — post-combat is where deck 109 repeatedly held exact burn lethal, and where
  the mana delusion made "cast nothing" feel justified to the model.
- **Reactive spells fired into nothing** — a counter on an empty stack, a combat trick on an empty
  board, an instant with no worthwhile target.
- **Wrong modal/menu picks** — color choices, X values, overload-vs-single. (Deck131 named red
  instead of blue every time.)
- **Inconsistency** — the SAME decision made differently across games (unleash taken here, declined
  there) means qwen has no stable rule; give it one.
- **Generic-prior misfires** — a universal prior that is WRONG for THIS specific deck. (The old
  blanket passivity and cost-refusal misfires are now fixed in the general guide; what remains are
  deck-specific overrides — e.g. a fourteen-sweeper deck that should spend freely against the generic
  sweeper caution, or a deck whose payoff must break the second-main-phase default.) Name the prior
  and override it explicitly with the deck-specific rule.
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

**Confirm before you accuse.** When a play looks wrong, read that decision's full `prompt` to check
what was actually legal / on the stack. (Deck135's Force of Negation LOOKED like a reflexive misfire
until the prompt showed it was countering a real Cruel Edict — so it stayed OUT of the mistakes list.)
Never teach against a non-problem, and never list a "mistake" you didn't observe.

## Step 3 — Decide what goes in vs. what gets cut

Include a line ONLY if it changes a decision qwen will actually face and gets wrong (or would).
Rank candidates by **frequency × cost-to-winning:**
1. The fix for the biggest observed leak / the deciding lever (always first).
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
   WHY. Make it unmissable.
3. **THE ENGINE / how the deck wins** — name the finishers/engine cards and the exact sequence to
   make them go. Say to DEPLOY them; passive pilots never close on their own.
4. **Standing operating rules / the core action loop** — develop-every-turn, protect-the-synergy,
   the turn-by-turn deployment priority. **[D109]** Where the observed failure lives in ONE phase,
   write this as a PHASE-KEYED CHECKLIST headed by the exact phase label the prompt shows ("MAIN
   PHASE 2 CHECKLIST — run it every turn after combat: 1. lethal burn check… 2. cast leftovers…
   3. never end the turn with 2+ untapped lands and castable cards"). The model sees "Phase: Main
   phase 2" in every prompt; anchoring the routine to that string is what makes it fire.
5. **Key cards — one imperative line each**, ordered by how often each decides the game: what it does
   (from the verified engine text) and the single directive/target/phase that matters. Only cards with
   a non-obvious right line; skip vanilla creatures.
6. **A lethal / math check** the pilot can run mechanically, if the deck closes with reach.
7. **Combat notes** — only if the deck has non-obvious combat (deathtouch/wither blockers, evasion,
   pump tricks, go-wide attacking rules).
8. **MULLIGAN** — one or two lines: what to keep, what to ship.
9. **DO NOT** — a tight list of the 3-5 specific misplays you actually observed, each as a flat
   imperative phrased as the exact situation qwen was in. This closes the loop from Step 2 and is the
   highest-signal part of the guide.

Match the deck8 / deck44 example density and length (~one screen, roughly 40-70 lines). Longer is not
better — every extra line dilutes the early ones.

## Step 5 — Word it for the executor (non-negotiable style rules)

- **Imperative and directive.** "EQUIP the Plating." "Cast Wrath when the opponent has 2+ creatures."
  "ALWAYS attack with Signal Pest." Never "consider," "you might," "it can be good to," "weigh whether"
  — qwen does not act on hedges; it falls back to its bad default.
- **Default + narrow, enumerable exceptions** — not open judgment and not a decision tree. Give the
  default action hard, then a short explicit list of the ONLY conditions that change it. If a rule needs
  three conditions, qwen will drop two.
  **[D109] Stress-test every absolute before shipping it: "what board makes obeying this rule lose?"**
  The executor WILL obey it there. If the losing board is realistic in this matchup pool, write the
  exception as a NAMED CARD LIST, not a judgment clause — "kill Young Pyromancer on sight" survives a
  weak reader; "unless the creature is a value engine" does not. The matchup pool's decklists are
  known: pull the actual danger cards from the opponents' lists and name them.
- **[D109] Counter a rules delusion with a PERCEPTION PROCEDURE, not a rules statement.** When Step 2
  found the model asserting something false about the game state, telling it the correct rule once is
  weaker than telling it what to LOOK AT and COUNT: "Attacking taps CREATURES only — your lands do not
  tap when you attack. Before saying 'Cast nothing', count the Mountains in 'Your battlefield:' NOT
  marked [tapped]; that number is your mana," plus a mana-count → castable-spells table it can read
  off. Ground the procedure in the exact board syntax the prompt uses ([tapped], the line names). This
  is the guide compensating for a model/representation weakness — legitimate and often the highest-value
  content in the file.
- **Low branching; use a ranked list or a selection TABLE, not nested IF/THEN.** When a choice has
  options, hand qwen a priority order (1, 2, 3) with a one-clause reason each, or a board→card mapping
  it reads off ("one creature → Edict; many small → Pyroclasm; anything big → Wrath"). qwen follows a
  checklist; it flounders on "if/then/unless." Where a branch is unavoidable, key it on something qwen
  can SEE (opponent's life, a card on the battlefield, its own hand size), never a hidden read.
- **Tie every rule to the in-game surface.** qwen sees option text and phases, not your synthesis.
  Write trigger→action and quote the phrasing the engine actually shows: "When you see 'Equip with
  Cranial Plating [cost {1}]', take it." Name cards EXACTLY as they appear in the action list.
  **[D109]** This includes the phase labels ("Phase: Main phase 2"), the battlefield tags ([tapped],
  [your battlefield]), and the target-menu wording ("The opponent (player, life N)") — quoting these
  is what lets a checklist actually trigger at the right moment.
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
  it at the face"). **[D109]** If the logs show the prior STILL winning against an existing override
  (deck 109: Spears held "for tricks" despite a whole anti-hoarding section), escalate: move the
  override up the guide, flatten it ("the hold-instants rule DOES NOT apply to Searing Spear"), and
  attach it to the named card rather than the category.
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
- **Repeating the general prompt** — wastes qwen's attention budget and buries the deck-specific signal.
- **Card text from memory** instead of the engine `auto=`/`text=` — you'll teach a rule the engine
  doesn't implement.
- **Guessing at misplays** instead of reading the translogs — the real failures are often not the ones
  you'd predict; or fixing a one-off slip instead of the recurring pattern; or listing a "mistake" you
  never observed (invented problems waste attention and can steer qwen wrong).
- **[D109] Skipping the replies** — the choice tells you WHAT went wrong; the reply tells you WHY. The
  biggest lever of the deck-109 revision (a rules delusion) was invisible in the choices alone.
- **[D109] Absolutes without exits** — a hard rule the executor obeys into a realistic losing board is
  a bug you authored. Every NEVER/ALWAYS ships with its named-exception list or a verified "no realistic
  exception in this pool."
- **Burying the lede** — the deciding rule below nice-to-know card notes; a correct rule in paragraph
  nine is a rule qwen skips.
- **Branch explosion** — every "but if…" is a fork qwen can get wrong; collapse to the best default,
  branch only when the default genuinely loses. Don't over-hedge exceptions until the default gets lost.
- **Symmetry blindness** — prescribing spend-freely or hoard without measuring the deck's redundancy.
- **Over-length** — a long guide buries its own most important line.
- **[D109] Grading wins as clean** — a win reached slower than the hand allowed carries the same leak
  as a loss; audit the clock, not just the outcome.

## Definition of done (checklist)

- [ ] Every card claim matches the engine's `text=`/`auto=`, not memory.
- [ ] The #1 rule at the top fixes the single biggest leak you actually found in THIS deck's logs,
      in imperative form.
- [ ] The win condition/engine is named and told to be deployed, near the top.
- [ ] Role-by-matchup is stated as a default + flip condition, not left for the pilot to derive.
- [ ] A mechanical lethal/math check is present if the deck can close with reach.
- [ ] Every "DO NOT" traces to a real observed decision in the logs.
- [ ] Generic-prompt priors that misfire for this deck are explicitly overridden by name.
- [ ] **[D109]** Every NEVER/ALWAYS was stress-tested against "what board makes obeying this lose?"
      and carries its named exceptions (or verified none exist in this matchup pool).
- [ ] **[D109]** Every recurring false belief found in the replies is countered by a look-and-count
      perception procedure grounded in the prompt's own board syntax.
- [ ] **[D109]** The wins were clock-audited (earliest-possible kill vs actual), not just the losses.
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
shorter, more directive, and iterate on that lever before adding anything else. **[D109]** In revision
cycles, also re-run the guide-echo grep: your new rule's phrasing appearing in the replies attached to
the RIGHT plays is the direct evidence the revision landed. Note: shipping the guide as
a Res asset needs `git add -f` (`bin/Res` is gitignored). End by naming, in your own words, the deck's plan
and the single biggest piloting problem the guide is built to fix.
