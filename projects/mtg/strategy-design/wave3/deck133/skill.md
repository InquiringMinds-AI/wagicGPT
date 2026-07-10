# Skill: Writing a per-deck strategy guide for the qwen pilot

## Revision notes (deck 133)

Field test: the wave2 guide for deck 133 (Phyrexian Asphodel, mono-black devotion)
went 0/6 — one loss, five 40-minute timeouts. The pilot chose "Cast nothing" on 76
of 101 casting decisions and cast Bloodghast 0 times in 77 offers, Gray Merchant 0
in 25. Reading the logs showed the guide itself CAUSED part of that. Changes below
are marked **[deck133]** in the body.

1. **Passivity-framed identity language is a leak vector** (new pitfall + a rule in
   Step 4.1 and Step 5). The wave2 guide's identity said "You are not a fast deck;
   you win late... stabilize first" — the weak executor took tempo-negative
   self-description as license to do nothing, every game. Evidence: five timeouts of
   near-total non-casting under that framing. The skill never warned that IDENTITY
   prose, not just rules, drives behavior.
2. **Prohibitions can be circular and self-locking** (new rule in Step 5 + pitfall).
   "NEVER cast Gray Merchant as your only black permanent" deadlocked: the pilot
   never built a board, so the precondition never held, so Merchant was declined 22
   of 25 offers. Fix pattern: convert prohibitions whose precondition depends on the
   pilot's OWN other behavior into numeric GO thresholds ("cast at drain 4+").
3. **Mine the REPLY text for false world-model beliefs, not just the choices** (Step
   2 addition). The choice stream showed WHAT (cast nothing); only the replies showed
   WHY: a systematic hallucination that "Mana in your pool: (none)" = cannot cast
   (61 reply occurrences, while 4-5 untapped Swamps sat in the printed battlefield),
   plus "landfall will return Bloodghast" said of a Bloodghast in HAND. The wave2
   skill's digest recipe ignored `reply` almost entirely.
4. **The guide must counter known executor misconceptions until a lower layer does**
   (new Step 3 include-rank entry). Affordability ("every offered cast is payable;
   the game auto-taps lands") is representation-layer truth, not deck strategy — but
   the deck guide is the only surface being revised, so it carries the corrective
   fact explicitly. Flag such items for promotion to the general prompt/representation.
5. **A HOLD instruction must be bounded by a mandated concurrent action** (Step 5
   rule). "HOLD Fatal Push" generalized into holding everything; the general
   prompt's hold-instants prior amplified it. Fix: "holding Push must NEVER stop you
   casting your creature."
6. **Turn-numbered deploy scripts beat priority prose** (Step 4.3 strengthened).
   "Turn 2: CAST BLOODGHAST" is executable; "deploy black threats mid-game" was not.
7. **The carried PLAN line makes procrastination self-reinforcing** (new trap). The
   pilot wrote "I will attack next turn," re-read its own plan next decision, and
   deferred again — for whole games. Guides need explicit anti-deferral phrasing
   ("the attack happens THIS turn") when logs show deferred-plan loops.
8. **Citing the observed failure stat in the guide itself** (Step 5, experimental).
   The revised guide tells the pilot "last run you picked Cast nothing 76 of 101
   times and died with a full hand." Unvalidated; marked as such.

What the wave2 skill got RIGHT and this revision keeps load-bearing: the two-model
frame; extract-don't-eyeball digesting (the offered-vs-declined tally produced the
smoking gun instantly); confirm-before-you-accuse (attack declarations and discard
picks were FINE — they stayed out of the DO NOT list); verify card text against
primitives (caught Last Hope's -2 milling 2, Yawgmoth's sacrifice-ANOTHER cost);
the cast-nothing tally instruction literally predicted where the value was.

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
- **[deck133] False world-model beliefs.** qwen holds runtime misconceptions about the
  game interface itself — e.g. reading "Mana in your pool: (none)" as "I cannot cast
  anything" while untapped lands sit on its printed battlefield, or believing a card in
  its HAND will return "via landfall" (a graveyard-only ability). These beliefs recur
  systematically and silently veto whole classes of correct actions. The guide must
  state the corrective FACT flatly ("every cast the list offers is payable; the game
  taps your lands automatically") — a strategy rule built on top of an uncorrected
  misconception never fires.

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
   **[deck133]** One caveat to the no-restatement rule: an OBSERVED, recurring executor
   misconception may be restated/countered in the deck guide even when it is "general"
   (e.g. the mana-pool misread), because the deck guide is the only artifact being
   revised right now. Flag every such line in your handoff as a candidate for promotion
   into the general prompt or the board representation, so it can later be removed from
   every deck guide at once.

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

**[deck133] Read the REPLY text, not just the choice — mine the stated WHY.** The choice
stream tells you WHAT went wrong; the `reply`/PLAN prose tells you WHY, and the why decides
what kind of fix works. Grep the replies for recurring false beliefs ("no mana", "cannot
cast", "my lands are tapped", a card claimed to be in the wrong zone, an ability claimed on a
card that lacks it) and CHECK each against the same record's printed board — if the stated
reason contradicts the prompt's own board state, you have a systematic misconception, and the
fix is a flat corrective FACT in the guide, not a strategy rule. (Deck133: 61 replies claimed
"no mana" while the battlefield line showed untapped Swamps — no amount of "deploy threats"
phrasing fixes a pilot that believes it cannot pay.) Tally the recurring phrases; one-off
confusions don't earn guide lines.

**[deck133] Also tally offered-vs-acted per card.** For every "Cast X" option across all
games, count offers vs. takes (and declines-into-"Cast nothing"). A card offered 77 times and
taken 0 is the lever announcing itself; the table takes ten lines of Python and removes all
argument.

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
tapped out, and durdled — that single delta became Rule #1. For deck135, a "crack fetchland"
action was offered 16 times in a game and passed 16 times; qwen made no mana, cast nothing, and
lost — that became Rule #1. For deck133, 76 of 101 casting decisions were "Cast nothing" on a
mana-pool misread — every timeout traced to it.) Most decks have one such lever. Find it before
you write a word.

**Diagnostic patterns to hunt for (weight the guide toward the ones that recur):**
- **Offered-many-times, passed-every-time** — a capability qwen doesn't understand it should use.
- **Cast-nothing / durdle rate.** TALLY the fraction of casting decisions where qwen chose "cast
  nothing," and list which cards it NEVER cast across all games. (Deck140's pilot passed 82% of
  casting decisions and cast its card-engine and all three finishers ZERO times — that one
  statistic defined the whole guide. Deck133 repeated the shape at 75%.) Whole turns of passing
  with castable cards and open mana = the deck isn't developing.
- **[deck133] False-belief vetoes.** A recurring reply-side misconception (can't-pay, wrong-zone,
  wrong-ability) silently vetoing legal actions. Distinguish this from strategic passivity — the
  fixes differ (corrective fact vs. directive).
- **Hoarding — answers/threats that pile up UNUSED in hand** while the opponent stays alive.
  Over-holding is as real a failure as over-firing. (Deck109 sat on Pillar of Flame / Searing
  Spear as if they were reactive removal, when they were reach it should have thrown at the face —
  "burn is damage, not removal.")
- **Failure to close — end-state sweep.** Print each game's final life totals; a game where the
  opponent's life PLATEAUED for many turns while qwen had gas is a failure to close. (Deck133 had
  an opponent at 4 life for four straight turns with six castable spells in hand, and timed out.)
- **Missing lethal.** Sum a turn's potential damage (attackers + payoff/keyword triggers + burn to
  face) and check whether qwen had lethal and didn't take it.
- **[deck133] Deferred-plan loops.** The pilot's PLAN line is carried into its next prompt, so a
  plan of "I will attack / cast it NEXT turn" gets re-read and re-affirmed forever — procrastination
  is self-reinforcing by construction. Hunt for the same "next turn" intention appearing in 3+
  consecutive plans while the action stays untaken; the fix is explicit now-phrasing in the guide
  ("the attack happens THIS turn").
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
  and override it explicitly with the deck-specific rule. **[deck133]** Watch especially for the
  hold-instants prior METASTASIZING: "hold interaction" generalizes in a weak model to "hold
  everything," and a guide's own HOLD line feeds it.
- **Cost-line misreads.** Any deck action whose cost LOOKS bad but is routine (fetch sacrifice, life
  payment, phyrexian mana, sac-for-value) needs an explicit "this cost is the point — always pay it."
  (Deck133 refused to crack fetchlands "to save 1 life" and stalled on two lands.)
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
until the prompt showed it was countering a real Cruel Edict — so it stayed OUT of the mistakes list.
Deck133's attack declarations and discard-target picks were fine on inspection and stayed OUT of its
DO NOT list.) Never teach against a non-problem, and never list a "mistake" you didn't observe.

## Step 3 — Decide what goes in vs. what gets cut

Include a line ONLY if it changes a decision qwen will actually face and gets wrong (or would).
Rank candidates by **frequency × cost-to-winning:**
1. The fix for the biggest observed leak / the deciding lever (always first).
2. **[deck133]** The corrective FACT for any systematic false belief the replies exposed (can't-pay,
   wrong-zone) — a misconception that vetoes actions outranks any strategy rule it would veto. Flag
   these lines for later promotion to the general prompt / representation layer.
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
   **[deck133] Frame the identity as ACTIONS, never as tempo.** The identity paragraph steers
   behavior as strongly as any rule. Write "you win by casting a creature every turn and finishing
   with X," never "you are not a fast deck / you win late / grind them out / stabilize first" — a
   weak executor reads slow-deck self-description as permission to pass. Audit the finished identity
   for the words "late," "slow," "grind," "wait," "stabilize," "hold" and recast each as the action
   it actually implies. Even a genuinely controlling deck gets action-framing ("answer their threat
   EVERY turn, then land your finisher") rather than posture-framing.
2. **THE #1 RULE** — the fix for the single biggest leak / the deciding lever from Step 2, hammered
   hard in its own visually loud block at the very top: WHAT the action is, the exact option text
   qwen will SEE, its enabling constraint (mana to hold open, board to build first), and a one-clause
   WHY. Make it unmissable.
3. **THE ENGINE / how the deck wins** — name the finishers/engine cards and the exact sequence to
   make them go. Say to DEPLOY them; passive pilots never close on their own.
4. **Standing operating rules / the core action loop** — develop-every-turn, protect-the-synergy,
   the turn-by-turn deployment priority. **[deck133] Prefer a LITERAL turn-numbered script** over
   priority prose wherever the deck has a natural curve: "Turn 1: land + Thoughtseize. Turn 2: land
   + CAST BLOODGHAST. Turn 3: ..." with one line for substitution ("missing the card? cast the
   closest creature you hold"). The pilot executes a numbered script far more reliably than "deploy
   threats early, payoff mid-game."
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
- **[deck133] No self-locking prohibitions — convert to GO thresholds.** Before shipping any
  NEVER/DON'T, check its precondition: if escaping the prohibition depends on the pilot's OWN other
  behavior, the rule can deadlock into a permanent veto ("NEVER cast Gray Merchant as your only black
  permanent" + a pilot that never develops = Merchant never cast, 22 of 25 offers declined). Rewrite as
  a numeric threshold the pilot can read straight off the board and that tells it when TO act: "cast it
  once drain reaches 4+." Every prohibition should have a visible, reachable GO state.
- **[deck133] Bound every HOLD with a mandated action.** Any "hold X" line must state what the pilot
  DOES this turn while holding ("hold Fatal Push for their turn — but holding it must NEVER stop you
  casting your creature"), or the hold generalizes into holding everything. An unbounded HOLD is a
  passivity license.
- **[deck133] State interface facts the pilot provably gets wrong.** When Step 2 found a systematic
  false belief, put the flat corrective fact in the guide even though it is "not strategy": "the
  action list only offers spells you CAN pay for; the game taps your lands automatically; 'Mana in
  your pool: (none)' is normal and does NOT mean you can't cast." Rules layered on an uncorrected
  misconception never execute.
- **Low branching; use a ranked list or a selection TABLE, not nested IF/THEN.** When a choice has
  options, hand qwen a priority order (1, 2, 3) with a one-clause reason each, or a board→card mapping
  it reads off ("one creature → Edict; many small → Pyroclasm; anything big → Wrath"). qwen follows a
  checklist; it flounders on "if/then/unless." Where a branch is unavoidable, key it on something qwen
  can SEE (opponent's life, a card on the battlefield, its own hand size), never a hidden read.
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
  Hellrider out) + burn to face; if ≥ opponent life, attack with everyone and fire all burn." **[deck133]**
  Per-card constants beat abstract quantities: "drain = 2 (its own {B}{B}) + 2 per Bloodghast + 3 per
  Geralf's + 4 per Obliterator" executes; "count your devotion" does not.
- **State the WHY in ONE clause, only for load-bearing rules** — so a rule that looks locally wrong is
  trusted anyway ("attack with the 0-power creature — its battle cry pumps the team"; "always pay the
  fetch sacrifice — that cost is the point"). qwen needs the move, not the theory.
- **Give explicit PERMISSION where the pilot is wrongly timid and explicit BRAKES where it is wrongly
  reckless.** Weak models need both stated. "Sweep proactively — it's fine to trade one card for two
  creatures" is as necessary as "NEVER chain two sweepers on one board."
- **Override the misleading generic prior BY NAME**, with a half-line of why, so the base prompt's prior
  doesn't win ("The general prompt says hold interaction — IGNORE that here; this burn is reach, throw
  it at the face").
- **Turn `#HINT` / `auto=` behaviors into one-liners the pilot can rely on** ("Blightsteel auto-attacks
  once it's in play — your only job is to CAST it").
- **Hard NEVER/ALWAYS for known bad defaults** ("NEVER strip their weakest card when a sweeper, counter,
  or bomb is in their hand"; "NEVER cast Guttersnipe with no burn to follow") — subject to the
  no-self-locking check above.
- **[deck133, experimental] Cite the observed failure back to the pilot** when the leak was
  catastrophic: "last run you chose 'Cast nothing' 76 of 101 times and died with a full hand — never
  again." The concrete self-referential stat may anchor better than an abstract warning. Unvalidated;
  keep to the #1 leak only.
- **Short.** It rides in every prompt and competes with the live board for attention. Cut anything that
  fails the "does this change a chosen number?" test.

## Pitfalls

- **Writing for yourself, not qwen** — the subtlest trap. Nuanced, hedged, branch-heavy prose reads well
  to you and gets ignored or misread by the executor. If a line requires the reader to do the reasoning
  you did, qwen won't; spell out the conclusion.
- **[deck133] Passivity-framed identity prose.** "You are not a fast deck / you win late / stabilize
  first" reads to the executor as standing permission to pass; the deck133 pilot cast nothing for whole
  games under exactly that framing. Identity language is behavior-steering text, not flavor — audit it
  like a rule.
- **[deck133] Circular prohibitions.** A NEVER whose escape condition the pilot itself must create can
  lock the payoff out of the game permanently. Check every prohibition for a reachable GO state.
- **Repeating the general prompt** — wastes qwen's attention budget and buries the deck-specific signal.
- **Card text from memory** instead of the engine `auto=`/`text=` — you'll teach a rule the engine
  doesn't implement.
- **Guessing at misplays** instead of reading the translogs — the real failures are often not the ones
  you'd predict; or fixing a one-off slip instead of the recurring pattern; or listing a "mistake" you
  never observed (invented problems waste attention and can steer qwen wrong).
- **[deck133] Diagnosing from choices alone.** The same "Cast nothing" stream can be strategic timidity
  (fix: directives) or a false can't-pay belief (fix: corrective fact). Only the reply text
  distinguishes them, and the wrong fix does nothing.
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
- [ ] The win condition/engine is named and told to be deployed, near the top.
- [ ] Role-by-matchup is stated as a default + flip condition, not left for the pilot to derive.
- [ ] A mechanical lethal/math check is present if the deck can close with reach.
- [ ] Every "DO NOT" traces to a real observed decision in the logs.
- [ ] Generic-prompt priors that misfire for this deck are explicitly overridden by name.
- [ ] **[deck133]** Replies were mined for false world-model beliefs; any systematic one is countered
      by a flat corrective fact in the guide and flagged for promotion to the general layer.
- [ ] **[deck133]** The identity paragraph is action-framed — no "late/slow/grind/stabilize/hold"
      posture language left unconverted.
- [ ] **[deck133]** Every NEVER/prohibition has a reachable GO state (numeric threshold where
      possible); every HOLD names the action the pilot takes while holding.
- [ ] Every rule is imperative, concrete (named cards, real numbers), low-branching, and executable
      from the board + action list alone — no inference required.
- [ ] Nothing repeats the general prompt; it fits on roughly one screen (~40-70 lines).

## Output and handoff

Write the finished guide as PLAIN TEXT (it slots into `{STRATEGY_GUIDE}` verbatim). No markdown headers
are needed; short ALL-CAPS section labels read fine and help qwen find the part it needs. The deck8 /
deck44 guides are the format and density reference.

The draft is LLM-authored; the user does a Magic-judgment authorship pass and owns the final guide — so
surface the card-rules details you verified and flag anything you were unsure of. **[deck133]** Also hand
off the list of interface-fact lines you embedded (affordability, zone corrections) as candidates for the
general prompt / board representation, so they can eventually be deleted from every deck guide at once.
Where possible, VALIDATE by real play, not by looking correct: re-run self-play with the guide in place
and read the new logs — did qwen start taking the plan-critical action? If not, the guide isn't blunt
enough; make Rule #1 louder, shorter, more directive, and iterate on that lever before adding anything
else. Note: shipping the guide as a Res asset needs `git add -f` (`bin/Res` is gitignored). End by naming,
in your own words, the deck's plan and the single biggest piloting problem the guide is built to fix.
