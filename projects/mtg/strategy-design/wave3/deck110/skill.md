# Skill: Writing a per-deck strategy guide for the qwen pilot (wave-3 revision, deck 110)

## Revision notes (deck 110)

Field test: the wave-2 skill was followed faithfully to produce the deployed deck110 guide —
and deck110 fell from the strongest deck to 0/6 (5 timeouts, 1 loss). The method's analysis
steps worked (they found the failure fast); its GUIDANCE steps had gaps that let the guide
itself cause the collapse. Changes, each marked `[REV-D110-n]` at its edit site:

1. **[REV-D110-1] Brakes over-fire into global hoarding — every prohibition needs a scope and
   a release.** The wave-2 guide's "DO NOT TAP OUT — leave one land untapped to equip" was a
   correct fix for the previous wave's leak, but the weak executor generalized it into total
   paralysis: 44–88% of casting decisions per game were "Cast nothing," Plating held in hand
   10+ turns "keeping mana open for the crucial equip next turn," and a free 4-damage Blast
   refused "because I'd have no untapped land to equip." Evidence: qwen's PLAN text cites the
   guide's own words while misplaying. New rules in Step 5: prefer positive sequencing over
   prohibitions; scope every brake to a visible condition; give it an explicit release.
2. **[REV-D110-2] Teach the INTERFACE, not just strategy.** qwen repeatedly hallucinated mana
   constraints ("Mana in your pool: (none)" ⇒ "I have no mana"; "my Mountain is untapped but I
   cannot produce Red") and declined listed casts it could trivially pay. The engine only
   offers PAYABLE actions — until the general prompt says so, each guide must. New Step 2
   diagnostic + Step 5 rule: state mechanical facts about the prompt surface (listed = payable;
   empty pool is normal; "(5/1) (printed 1/1)" means the equipment is already attached).
3. **[REV-D110-3] Read the `reply` field, not just the choices.** The digest of chosen options
   showed WHAT went wrong; the PLAN text showed WHY (hallucinated costs, guide sentences echoed
   verbatim, plans that say "cast X now" while the chosen number is "Cast nothing"). New Step 2
   instruction to mine replies for misconception fingerprints and plan/number mismatches.
4. **[REV-D110-4] The corpus is guide-conditioned — attribute to the deployed guide FIRST.**
   Wave-2's Step 2 assumed logs reveal qwen's raw defaults. After any wave, logs reveal
   qwen×guide interaction: match misplay rationales against the deployed guide's sentences
   before blaming model defaults. A rule the executor CITES while misplaying is a rule to
   rewrite or delete.
5. **[REV-D110-5] New diagnostic patterns:** perpetual deferral ("I will do it next turn,"
   every turn); repeated-activation waste loops (the inverse of offered-and-passed — equip
   re-taken every turn on an already-equipped creature "to ensure the bonus is active");
   plan/number mismatch.
6. **[REV-D110-6] Wall-clock economics.** Each decision costs ~60–70s of inference; games hit
   the 40-min cap at only ~35–50 decisions. Every durdle decision and waste loop burns clock;
   a guide that shortens the game (kill by turn 5–6) is also the timeout fix. Added to Step 2.
7. **[REV-D110-7] Validation is not optional after a revision wave** — a guide can REGRESS a
   deck (deck110 is the proof). Strengthened the handoff section: treat re-run self-play as
   part of done-ness, and diff behavior against the PREVIOUS wave's, not just against "better."

What the wave-2 skill got right and was relied on unchanged: the two-models framing; extract-
don't-eyeball digest scripting; isolate-the-ONE-lever; confirm-before-you-accuse (reading full
prompts caught that the `{B}{B} attach` line is a real engine option, not a hallucination);
card text from primitives, never memory; front-loading; DO-NOT-only-from-observed-misplays.

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
- **[REV-D110-1] Brake over-generalization.** A weak executor does not scope a caution; it
  universalizes it. "Leave one land untapped to equip" becomes "never spend mana"; "don't
  cast redundant copies" becomes "don't cast." Any DON'T you write will be applied far
  beyond the situation you meant. Design accordingly (see Step 5).
- **[REV-D110-2] A broken model of the game interface.** qwen misreads the prompt surface
  itself: an empty mana pool reads as "no mana," untapped lands don't read as castability,
  a displayed power bonus doesn't read as "equipment already attached," and it invents
  costs and color requirements. Where strategy rules assume the executor understands the
  interface, they silently fail. State the interface facts it needs as flat rules.

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
   [REV-D110-2] One interface fact is NOT yet in the general prompt (flag it upstream, state it
   in your guide until it is): **the engine's numbered action list is affordability-filtered —
   every listed cast/activation is payable RIGHT NOW, and lands tap automatically.** The prompt's
   "Mana in your pool: (none)" line actively misleads a weak executor into believing it has no
   mana. Corpus-wide, this misconception drove cast-nothing paralysis (deck110: 44–88% of casting
   decisions per game were "Cast nothing," with PLAN text like "I cannot produce Red mana" while
   a payable red spell sat in the options list).

3. **The deployed guide the corpus was played under** — [REV-D110-4] if this deck already had a
   strategy guide in play during the games you are about to mine, READ IT FIRST and keep it open
   beside the logs. The logs show qwen×guide behavior, not qwen's raw defaults. For every misplay,
   check whether qwen's PLAN text echoes a sentence of the deployed guide (deck110's pilot quoted
   "keep mana open to equip" almost verbatim while durdling to death). A guide rule the executor
   cites while misplaying is YOUR bug: rewrite or delete that rule — do not stack a new rule on
   top of it, and do not attribute the behavior to the model until the guide is ruled out.

4. **Card text from the ENGINE, not your memory** — for every non-basic card, pull its
   `text=` and `auto=` lines from `bin/Res/sets/primitives/*.txt` (grep by `name=` across
   `mtg.txt`, and also `planeswalkers.txt` / `borderline.txt` if present). **This is what
   the engine actually enforces and what the AI is shown. Oracle text you remember, the
   printed cardface, and the engine `auto=` script can all DISAGREE — the script wins.**
   Verify the exact numbers you will cite (mana cost, power/toughness, keywords, ETB,
   activated-ability costs, devotion/threshold counts, X values) against it. Note that
   hybrid mana ({B/R}, {R/W}) is castable with EITHER color, so a "gold" card may be
   mono-color-castable in this deck. [REV-D110-2] Also mine the `auto=` lines for EXTRA
   action lines the executor will be offered: a card with two routes to the same effect
   (Cranial Plating's `{1}:equip` AND `{B}{B}:attach`) shows BOTH as options, and the
   executor cannot tell the cheap one from the trap — your guide must say which line to
   take and which to ignore, quoting the option text. Use `/usr/bin/grep` — bare `grep`
   here is a wrapper that can silently drop matches.

5. **The game logs** — the deck's self-play translogs, JSONL, one line per decision qwen
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

**[REV-D110-3] Then read the `reply` field for the decisions that look wrong — it is your WHY.**
The digest tells you WHAT the executor did; the PLAN text in `reply` tells you the misconception
behind it, and that misconception is what your rule must attack. Hunt three fingerprints:
- **Hallucinated constraints** — invented mana/color/cost reasons for passing ("my Mountain is
  untapped but I have no way to produce Red"; a `{B}{B}` cost imagined onto a {1} ability).
  These mark INTERFACE facts the guide must state flatly.
- **Guide echo** — your own deployed guide's sentence quoted in a misplaying PLAN. That rule
  over-fired; fix the rule (see input 3).
- **Plan/number mismatch** — the PLAN says "cast X now" while the chosen number is "Cast
  nothing." The guide can partially patch this ("your chosen number must match your plan"),
  but log it as model weakness too.

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
  it take it?

**The single highest-value move: isolate the ONE lever.** Compare the games qwen played WELL
against the ones it LOST and find the one repeated difference. (For deck110, every good game
equipped Cranial Plating early and swung; every bad game cast Platings, never equipped them,
tapped out, and durdled — that single delta became Rule #1. A wave later, the lever had MOVED:
the new #1 leak was cast-nothing paralysis caused by the previous fix — re-derive the lever
from the CURRENT corpus every wave; never assume last wave's lever still holds. For deck135, a
"crack fetchland" action was offered 16 times in a game and passed 16 times; qwen made no mana,
cast nothing, and lost — that became Rule #1.) Most decks have one such lever. Find it before
you write a word.

**Diagnostic patterns to hunt for (weight the guide toward the ones that recur):**
- **Offered-many-times, passed-every-time** — a capability qwen doesn't understand it should use.
- **[REV-D110-5] Taken-every-time-when-once-was-enough — waste loops.** The inverse of the above:
  a repeatable action the executor re-takes every turn after it has already done its job
  (deck110 re-equipped Cranial Plating onto the SAME already-equipped creature turn after turn
  "to ensure the bonus is active" — mana and wall-clock burned for nothing). The engine keeps
  offering the action; the executor cannot tell "available" from "still useful." Give a
  recognition cue tied to the board display ("(5/1) (printed 1/1)" = already attached → pass).
- **Cast-nothing / durdle rate.** TALLY the fraction of casting decisions where qwen chose "cast
  nothing," and list which cards it NEVER cast across all games. (Deck140's pilot passed 82% of
  casting decisions and cast its card-engine and all three finishers ZERO times — that one
  statistic defined the whole guide.) Whole turns of passing with castable cards and open mana =
  the deck isn't developing.
- **[REV-D110-5] Perpetual deferral.** PLAN text that promises the plan-critical action "next
  turn" — every turn, indefinitely (deck110 held Plating in hand 10+ turns "keeping mana open
  for the crucial equip next turn" while 0-power creatures attacked for nothing). Distinct from
  simple passivity: the executor KNOWS the right action and never fires it. The fix is an
  act-NOW rule: "cast it the first turn it appears in your options, even if you cannot also
  <follow-up> this turn."
- **Hoarding — answers/threats that pile up UNUSED in hand** while the opponent stays alive.
  Over-holding is as real a failure as over-firing. (Deck109 sat on Pillar of Flame / Searing
  Spear as if they were reactive removal, when they were reach it should have thrown at the face —
  "burn is damage, not removal.")
- **Failure to close — end-state sweep.** Print each game's final life totals; a game where the
  opponent's life PLATEAUED for many turns while qwen had gas is a failure to close.
  [REV-D110-6] Failure to close is also THE timeout mechanism: each decision costs ~60-70s of
  model inference, so a 40-minute game is only ~35-50 decisions. Durdle decisions and waste
  loops burn that budget directly; a deck that kills by turn 5-6 finishes inside the cap, a
  deck that plateaus cannot. When a deck times out repeatedly, count decisions-per-turn and
  find what inflates them — the guide that shortens the game IS the timeout fix.
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
  sweeper caution, or a deck whose payoff must break the second-main-phase default.) Name the prior
  and override it explicitly with the deck-specific rule.
- **[REV-D110-2] Interface misreads.** The executor misunderstanding the prompt surface itself:
  believing an empty mana pool means it cannot cast; not recognizing that every listed action is
  payable; not recognizing attached equipment in the power display; treating "untapped lands" and
  "available mana" as different things. These produce misplays that look strategic but are
  mechanical. Fix them with flat interface-fact rules, and flag them upstream for the general
  prompt — they will be hurting every deck.
- **Cost-line misreads.** Any deck action whose cost LOOKS bad but is routine (fetch sacrifice, life
  payment, phyrexian mana, sac-for-value) needs an explicit "this cost is the point — always pay it."
- **Engine realities that gate the plan.** qwen can't equip if it taps out (no {1} left), can't
  attack profitably with an unequipped 0-power body. These become concrete "keep a land open" rules —
  [REV-D110-1] but see Step 5: write them as scoped positive sequences, never as blanket
  prohibitions, or they will over-fire into paralysis.
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
2. [REV-D110-2] Interface-fact rules the executor demonstrably lacks (listed = payable; display
   cues for "already done") — these outrank strategy rules because every strategy rule silently
   depends on them.
3. The win engine and how to set it up, protect it, and deploy it.
4. Role + matchup flips (concrete, deck-specific).
5. Target-priority and removal/sweeper-selection rules.
6. Per-card timing for the cards that decide games; a lethal-math check if the deck closes with reach.
7. Mulligan, in a line or two.

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
   [REV-D110-5] Write multi-step engines as a NUMBERED mechanical sequence (1. cast it the first
   turn offered, even if you can't finish the combo this turn; 2. take the "<exact option text>"
   action; 3. recognition cue for "done" → stop/pass; 4. which duplicate option lines to ignore).
   A numbered sequence with per-step act-now/stop conditions is what killed both the perpetual-
   deferral and the waste-loop failure modes; prose principles fixed neither.
4. **Standing operating rules / the core action loop** — develop-every-turn, protect-the-synergy,
   the turn-by-turn deployment priority.
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
- **[REV-D110-1] Prohibitions over-fire — prefer positive sequences, and scope every brake.** The
  executor universalizes any DON'T: "DO NOT TAP OUT (to keep equip mana)" became "never spend mana,"
  "don't cast redundant Platings" became "don't cast Plating," and games were lost from 20 life with
  a full hand. Three concrete rules:
  1. Say what to DO in what ORDER, and let the order imply the caution: "cast your creatures FIRST;
     equip costs only {1}" instead of "don't tap out."
  2. If a brake must exist, scope it to a condition the executor can SEE on the board and give it an
     explicit RELEASE: "hold Blast for a blocker — but when the opponent is at 8 or less, ALWAYS fire
     it at their face." A brake without a release becomes permanent.
  3. After drafting, reread every DON'T and ask: "if the executor applies this sentence to EVERY
     decision for the rest of the game, does the deck still function?" If not, rewrite it.
- **Low branching; use a ranked list or a selection TABLE, not nested IF/THEN.** When a choice has
  options, hand qwen a priority order (1, 2, 3) with a one-clause reason each, or a board→card mapping
  it reads off ("one creature → Edict; many small → Pyroclasm; anything big → Wrath"). qwen follows a
  checklist; it flounders on "if/then/unless." Where a branch is unavoidable, key it on something qwen
  can SEE (opponent's life, a card on the battlefield, its own hand size), never a hidden read.
- **Tie every rule to the in-game surface.** qwen sees option text and phases, not your synthesis.
  Write trigger→action and quote the phrasing the engine actually shows: "When you see 'Equip with
  Cranial Plating [cost {1}]', take it." Name cards EXACTLY as they appear in the action list.
  [REV-D110-2] This includes STATE-recognition cues, not just action text: teach the executor to
  read the board display ("Memnite (5/1) (printed 1/1)" means the Plating is ALREADY attached — pass,
  don't re-equip"; "Mana in your pool: (none)" is normal, not a constraint). And where the engine
  offers two option lines for one effect, name the one to take and the one to IGNORE, by their
  bracketed cost text.
- **[REV-D110-2] State the interface facts the executor is missing.** Until the general prompt covers
  it, every guide for a deck showing cast-nothing paralysis needs: "every 'Cast X' line in your
  options is already checked — you CAN pay it; the game taps your lands automatically; never pick
  'Cast nothing' because you think you lack mana or colors." This one flat rule attacks the single
  largest observed leak class in the corpus.
- **Concrete, named, numeric.** Name the cards and their mana costs; name the opponent cards seen in
  the logs; state thresholds numerically and say what they turn on ("metalcraft = 3+ artifacts,"
  "2 damage kills toughness 2 or less," "attack when 3+ gets through," "hold removal for creatures 4
  power or bigger"). "Develop your board" / "use your removal wisely" is invisible to qwen;
  "cast at least 3 other black permanents first" / "counter their creature with Essence Scatter" is
  executable. [REV-D110-5] Numeric triggers doubly matter for brake releases ("opponent at 8 or
  less → all burn to the face") — the executor can compare two numbers on the board; it cannot judge
  "when the time is right."
- **Give arithmetic it can run.** A lethal check as an explicit sum is gold — qwen can evaluate it
  against the board with no strategic insight: "damage = unblocked attacker power + (1 per attacker if
  Hellrider out) + burn to face; if ≥ opponent life, attack with everyone and fire all burn."
- **State the WHY in ONE clause, only for load-bearing rules** — so a rule that looks locally wrong is
  trusted anyway ("attack with the 0-power creature — its battle cry pumps the team"; "always pay the
  fetch sacrifice — that cost is the point"). qwen needs the move, not the theory.
- **Give explicit PERMISSION where the pilot is wrongly timid and explicit BRAKES where it is wrongly
  reckless.** Weak models need both stated. "Sweep proactively — it's fine to trade one card for two
  creatures" is as necessary as "NEVER chain two sweepers on one board." [REV-D110-1] But the two are
  not symmetric in risk: an over-fired permission costs a card; an over-fired brake can cost every
  turn of the game. When in doubt between the two, this executor's dominant failure mode is
  passivity — default to permission.
- **Override the misleading generic prior BY NAME**, with a half-line of why, so the base prompt's prior
  doesn't win ("The general prompt says hold interaction — IGNORE that here; this burn is reach, throw
  it at the face").
- **[REV-D110-5] Anti-deferral wording.** For the plan-critical action, say "NOW / the first turn it
  appears / in THIS reply," and explicitly license the imperfect version: "cast it even if you cannot
  also equip this turn." Add "your chosen number must match your plan — never describe casting X and
  then pick 'Cast nothing'" when the logs show plan/number mismatch.
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
- **[REV-D110-1] Fixing last wave's leak so hard you cause this wave's.** The wave-2 deck110 guide's
  anti-tap-out rule was a correct fix for the observed equip failure — and it regressed the strongest
  deck to 0/6 by inducing total hoarding. Every corrective rule is a new prior the executor will
  over-apply; run the "applied to every decision" test (Step 5) on each one, and expect the lever to
  MOVE between waves.
- **Repeating the general prompt** — wastes qwen's attention budget and buries the deck-specific signal.
- **Card text from memory** instead of the engine `auto=`/`text=` — you'll teach a rule the engine
  doesn't implement.
- **Guessing at misplays** instead of reading the translogs — the real failures are often not the ones
  you'd predict; or fixing a one-off slip instead of the recurring pattern; or listing a "mistake" you
  never observed (invented problems waste attention and can steer qwen wrong).
- **[REV-D110-3] Reading only choices, not replies** — the digest shows the misplay; the PLAN text shows
  the misconception (hallucinated mana, guide echo, deferral). A rule aimed at the behavior instead of
  the misconception behind it usually misses.
- **Burying the lede** — the deciding rule below nice-to-know card notes; a correct rule in paragraph
  nine is a rule qwen skips.
- **Branch explosion** — every "but if…" is a fork qwen can get wrong; collapse to the best default,
  branch only when the default genuinely loses. Don't over-hedge exceptions until the default gets lost.
- **Symmetry blindness** — prescribing spend-freely or hoard without measuring the deck's redundancy.
- **Over-length** — a long guide buries its own most important line.

## Definition of done (checklist)

- [ ] Every card claim matches the engine's `text=`/`auto=`, not memory.
- [ ] The #1 rule at the top fixes the single biggest leak you actually found in THIS deck's logs,
      in imperative form.
- [ ] [REV-D110-4] If a prior guide was in play during the corpus, every misplay was checked against
      that guide's sentences, and any rule the executor echoed while misplaying has been rewritten or
      deleted — not merely counter-weighted with a new rule.
- [ ] [REV-D110-1] Every DON'T/brake passes the "applied to every decision, the deck still functions"
      test, is scoped to a visible board condition, and has an explicit release.
- [ ] [REV-D110-2] Interface facts the logs show the executor lacks (listed = payable; already-attached
      display cues; duplicate option lines) are stated flatly, and any corpus-wide ones are flagged for
      the general prompt.
- [ ] The win condition/engine is named and told to be deployed, near the top — as a numbered
      sequence with act-NOW and stop conditions if it is multi-step.
- [ ] Role-by-matchup is stated as a default + flip condition, not left for the pilot to derive.
- [ ] A mechanical lethal/math check is present if the deck can close with reach.
- [ ] Every "DO NOT" traces to a real observed decision in the logs.
- [ ] Generic-prompt priors that misfire for this deck are explicitly overridden by name.
- [ ] Every rule is imperative, concrete (named cards, real numbers), low-branching, and executable
      from the board + action list alone — no inference required.
- [ ] Nothing repeats the general prompt; it fits on roughly one screen (~40-70 lines).

## Output and handoff

Write the finished guide as PLAIN TEXT (it slots into `{STRATEGY_GUIDE}` verbatim). No markdown headers
are needed; short ALL-CAPS section labels read fine and help qwen find the part it needs. The deck8 /
deck44 guides are the format and density reference.

The draft is LLM-authored; the user does a Magic-judgment authorship pass and owns the final guide — so
surface the card-rules details you verified and flag anything you were unsure of. [REV-D110-7] VALIDATE
by real play — this is now part of done-ness, not a nice-to-have: a revision can REGRESS a deck (the
wave-2 deck110 guide took the previous wave's strongest deck to 0/6). Re-run self-play with the guide in
place and read the new logs with two questions: (1) did qwen start taking the plan-critical action, and
(2) did any NEW rule get echoed in misplaying PLAN text — the signature of an over-firing correction.
Compare against the PREVIOUS wave's behavior for this deck, not just against "looks better." If the
lever didn't move, the guide isn't blunt enough; make Rule #1 louder, shorter, more directive, and
iterate on that lever before adding anything else. Note: shipping the guide as a Res asset needs
`git add -f` (`bin/Res` is gitignored). End by naming, in your own words, the deck's plan and the single
biggest piloting problem the guide is built to fix.
