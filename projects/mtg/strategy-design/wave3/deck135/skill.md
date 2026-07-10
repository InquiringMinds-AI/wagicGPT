# Skill: Writing a per-deck strategy guide for the qwen pilot (wave-3 revision, deck 135)

## Revision notes (deck 135)

Field test: the wave-2 skill produced a deck-135 guide whose Rule #1 (crack fetchlands)
WORKED — fetches were cracked promptly in every corpus game — and the deck still went 2/6,
because the failure moved one step downstream: the guide's own "fetch Forest by default"
directive caused a blue-mana screw that stranded half the deck's spells in hand for
10+ turns. Changes, each with the evidence behind it:

1. **NEW Step 0 — "Revising an existing guide" wave loop.** A fixed rule SHIFTS the failure
   downstream, and a prior guide's own default can BE the new leak. Evidence: fetch-cracking
   fixed → every fetch took a Forest (the old guide's default) → Ice-Fang Coatl held from T5
   to T11 uncast, an 8/8 Treefolk stranded in hand, "Cast nothing" turns, 1-2 total damage in
   11-16 turns, three timeouts. The skill previously had no revision procedure at all.
2. **Step 1 addition — castability/color-dependency analysis.** Count sources of each color,
   list which spells are stranded without each color, and verify what each search effect can
   legally find (Windswept Heath cannot fetch an Island; Into the North can). The old skill
   measured redundancy but not castability, and castability was the whole game.
3. **Step 2 addition — mine the WINS as mirrors of the losses.** The two wins were the same
   deck with one variable flipped (Island fetched by T3/T6 → threats deployed → attacks every
   turn); the lever was isolated by the win/stall delta, not by staring at losses. The old
   skill said "compare games played well against ones it lost" in one clause; promoted to a
   named technique with instructions, and timeouts explicitly classed as stall-evidence.
4. **Step 5 addition — a deck-guide directive LOSES to a conflicting general prior unless the
   guide names the prior and states the exemption.** Evidence: the old guide said "cast every
   copy" of Arcum's Astrolabe; qwen declined the second copy as "redundant and mana
   inefficient" — the general prompt's anti-redundant-copies line beat the guide. "Override
   by name" existed for observed misfires; strengthened to: scan your own directives for
   collisions with general priors and pre-empt them.
5. **Step 5 addition — the stale-plan antidote.** The carried PLAN line can contain
   hallucinated state that beats the board in qwen's reasoning (it declined to crack a
   Prismatic Vista because its plan said the Vista "has already been sacrificed"). When logs
   show plan-vs-board contradictions, add a grounding line: "if the option is still in the
   action list, it has NOT been done — believe the action list, not your memory of your plan."
6. **Step 2/5 addition — legality-of-plan errors (rules mechanics).** qwen committed three
   consecutive decisions to "attack with Treefolk this turn for 9" the turn Treefolk was cast
   (summoning sickness); the 9 damage never existed and the race plan built on it lost the
   game. Watch for plans that assume illegal actions; a one-line mechanics restatement tied to
   the specific card ("Treefolk CANNOT attack the turn you cast it") is cheap and earns its place.
7. **Confirmed-working practices kept verbatim** (relied on directly this wave): extract-don't-
   eyeball digest scripting; confirm-before-you-accuse (the "Cast nothing" at T11 looked like
   timidity until the prompt showed Treefolk wasn't even offered — no blue mana); quoting exact
   option text; naming opponent cards from the logs (the Lightmine Field 2-for-0 blowout became
   a named combat warning); front-loading; the DO-NOT list traced to observed decisions.

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

Two failure tendencies to design against:
- **Locally-reasonable, globally-blind.** Each pick looks fine in isolation while the
  overall plan quietly never happens (the threat never gets deployed, the payoff never
  gets activated). Your job is to name the plan-critical action so plainly qwen takes it
  every time.
- **Shallow-heuristic defaults.** When the guide leaves a decision open, qwen falls back
  to bad defaults: pick by list position, by "biggest creature," by "a card is a card,"
  or by the safe/passive option when unsure. Every "consider / think about / weigh / it
  depends" is a sentence that hands the decision back to those defaults. Convert judgment
  into directives: **"DO X when Y," never "consider X."**

Write imperative, front-loaded, low-branching, concrete, evidence-driven. If a sentence
doesn't change which number qwen picks, cut it.

## Step 0 — Revising an EXISTING guide (the wave loop) *(new — deck 135)*

When a deployed guide was in play during the corpus you are mining, do this before
anything else:

- **Audit last wave's rules against the new logs: did each one get FOLLOWED?** A rule
  that now gets followed has done its job — DEMOTE and compress it (keep it, shorter;
  deleting it risks regression), and spend the reclaimed space on the new leak.
- **Expect the failure to have MOVED downstream.** Fixing the blocking action exposes
  the next bottleneck in the same chain (fetches now get cracked → but every fetch takes
  the wrong land → the deck still casts nothing). Mine the new corpus fresh; do not
  assume last wave's diagnosis still names the biggest leak.
- **Suspect your own defaults.** Any default the old guide prescribed ("fetch Forest by
  default," "hold X for Y") is a candidate CAUSE of the new failure, because qwen follows
  it more literally than you meant it. For each old default, check the logs for games
  where obeying it was the mistake. Re-derive the deck's dependencies (colors, sequencing)
  from scratch each wave rather than inheriting the old guide's answers.
- **Keep what worked, verbatim where possible** — churn in lines qwen already obeys is
  pure risk.

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
   deck and must be overridden explicitly (see the logs step) — **and also check the reverse
   direction: for every directive YOUR guide gives, ask which general prior argues AGAINST it,
   and pre-empt that prior by name** (see Step 5). *(strengthened — deck 135)*

3. **Card text from the ENGINE, not your memory** — for every non-basic card, pull its
   `text=` and `auto=` lines from `bin/Res/sets/primitives/*.txt` (grep by `name=` across
   `mtg.txt`, and also `planeswalkers.txt` / `borderline.txt` if present). **This is what
   the engine actually enforces and what the AI is shown. Oracle text you remember, the
   printed cardface, and the engine `auto=` script can all DISAGREE — the script wins.**
   Verify the exact numbers you will cite (mana cost, power/toughness, keywords, ETB,
   activated-ability costs, devotion/threshold counts, X values) against it. Note that
   hybrid mana ({B/R}, {R/W}) is castable with EITHER color, so a "gold" card may be
   mono-color-castable in this deck. **Verify search/fetch RESTRICTIONS too — what can each
   tutor, fetchland, or ramp spell legally find? A "fetch a land" card that can only find
   two of the deck's three colors is a trap qwen will walk into every game.** Use
   `/usr/bin/grep` — bare `grep` here is a wrapper that can silently drop matches.
   *(fetch-restriction clause new — deck 135)*

4. **The game logs** — the deck's self-play translogs, JSONL, one line per decision qwen
   made piloting THIS deck: `~/.Wagic/ai/gpt/selfplay-runs/<run>/*-<deck>-*.jsonl`. Fields:
   `kind` (ask|priority|attackers|blockers), `prompt` (the FULL board + numbered legal
   actions qwen actually saw), `reply` (its text), `choice` (the index it picked). (`options`
   is just a count — ignore it.) **This is your primary evidence for what qwen gets wrong.**

## Step 1 — Read the deck: archetype, engine, role, redundancy, castability

Answer these before writing a word — they are the guide's spine:

- **Archetype & clock.** Aggro / tempo / midrange / control / combo / prison? How does it
  actually kill (fast evasive creatures + burn, a lock, a big finisher, a devotion drain)?
  Roughly how many turns is its clock ("kill by turn 4-6")?
- **The win engine / win condition.** Name the specific 1-3 cards that end games and the
  exact interaction that makes them win (Guttersnipe + Young Pyromancer turning every cheap
  spell into damage; a Vise + Howling Mine lock; Cranial Plating on an evasive body). A weak
  pilot will never find these unaided — the guide's spine is "set up, protect, and deploy the
  payoff." Guides teach the DECK'S engine, not general Magic.
- **Castability — the color/source dependency map.** *(new — deck 135)* For each color the
  deck uses: count its SOURCES (basics of that color, duals, any-color rocks/dorks — verify
  against engine text), and list the spells STRANDED without it. A splash color with 2 sources
  feeding 10+ spells is a live failure mode: if qwen's land choices never produce that color,
  those spells rot in hand and the deck plays half its cards. When the map shows a bottleneck,
  the guide needs an explicit, sequenced source-acquisition rule ("your FIRST fetch that can
  take an Island takes it") — including which searchers CAN and CANNOT find the bottleneck
  land. Do not prescribe a fetch/tutor default by frequency of use ("Forest by default")
  without first checking what that default starves.
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

**Mine the WINS as mirrors of the losses.** *(promoted to a named technique — deck 135)*
The single cleanest way to isolate the deciding lever is the win/loss delta on the SAME deck:
digest a won game and a lost/stalled game side by side and find the one variable that flipped
(in the wins an Island arrived by turn 3-6 and every threat got cast; in the stalls every
fetch took a Forest and the same threats sat in hand all game). The wins also hand you the
guide's positive content for free: the healthy opening sequence you observed is the sequence
you prescribe — you are describing play qwen has already proven it can execute, not play you
hope it can. **Treat TIMEOUTS as stall-evidence, not missing data:** print the final life
totals and turn count; a 15-turn game where the opponent sits at 19-20 life is a
failure-to-close game and usually the richest source of leaks.

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
tapped out, and durdled — that single delta became Rule #1. For deck135 in wave 2, a "crack
fetchland" action was offered 16 times in a game and passed 16 times — that became Rule #1;
in wave 3, with cracking fixed, the delta became WHICH land the fetch took.) Most decks have
one such lever. Find it before you write a word.

**Diagnostic patterns to hunt for (weight the guide toward the ones that recur):**
- **Offered-many-times, passed-every-time** — a capability qwen doesn't understand it should use.
- **Cast-nothing / durdle rate.** TALLY the fraction of casting decisions where qwen chose "cast
  nothing," and list which cards it NEVER cast across all games. (Deck140's pilot passed 82% of
  casting decisions and cast its card-engine and all three finishers ZERO times — that one
  statistic defined the whole guide.) Whole turns of passing with castable cards and open mana =
  the deck isn't developing. **When a key card was never cast, check WHY before prescribing:
  never drawn (variance), never castable (mana/color problem — fix the mana rule, not the
  timidity), or castable-and-declined (fix the timidity).** *(clause new — deck 135)*
- **Hoarding — answers/threats that pile up UNUSED in hand** while the opponent stays alive.
  Over-holding is as real a failure as over-firing. (Deck109 sat on Pillar of Flame / Searing
  Spear as if they were reactive removal, when they were reach it should have thrown at the face —
  "burn is damage, not removal.")
- **Failure to close — end-state sweep.** Print each game's final life totals; a game where the
  opponent's life PLATEAUED for many turns while qwen had gas is a failure to close.
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
- **Plans built on ILLEGAL actions.** *(new — deck 135)* Read the PLAN lines around big turns:
  qwen commits multi-decision plans to actions the rules forbid (attacking with a creature the
  turn it was cast; "blocking" with a tapped creature) and then makes real choices — racing,
  declining blockers — as if the phantom damage existed. When observed, a one-line mechanics
  restatement TIED TO THE NAMED CARD ("Treefolk CANNOT attack the turn you cast it — count its
  damage NEXT turn") is cheap and prevents the whole downstream chain.
- **Stale-plan/board contradictions.** *(new — deck 135)* The carried PLAN line is qwen's memory,
  and it hallucinates: it declined a live fetch-crack because its plan said the fetch "has already
  been sacrificed" while the option sat in the action list. When you see the plan contradicting the
  board, add the grounding line to the guide: "if the option is still in your action list, it has
  NOT been done — believe the action list, not your memory of your plan."
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
until the prompt showed it was countering a real Cruel Edict — so it stayed OUT of the mistakes list.
In wave 3, a "Cast nothing" holding an 8/8 finisher looked like timidity until the prompt showed the
finisher wasn't even in the cast list — no blue mana; the fix belonged in the MANA rule, not a
be-braver rule.) Never teach against a non-problem, and never list a "mistake" you didn't observe.

## Step 3 — Decide what goes in vs. what gets cut

Include a line ONLY if it changes a decision qwen will actually face and gets wrong (or would).
Rank candidates by **frequency × cost-to-winning:**
1. The fix for the biggest observed leak / the deciding lever (always first).
2. The win engine and how to set it up, protect it, and deploy it.
3. Role + matchup flips (concrete, deck-specific).
4. Target-priority and removal/sweeper-selection rules.
5. Per-card timing for the cards that decide games; a lethal-math check if the deck closes with reach.
6. Mulligan, in a line or two.

**On a revision wave, last wave's now-obeyed Rule #1 does not keep its top slot** — compress it
into a short numbered rule below the new #1; keep it present (regression insurance) but spend the
prime attention real estate on the new lever. *(new — deck 135)*

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
  three conditions, qwen will drop two. **And audit every default you write for what it silently
  starves: qwen executes a default MORE literally than you mean it ("Forest by default" became
  Forest-every-time and no blue source all game). If the default must ever yield, sequence it
  explicitly ("FIRST Island, THEN Forest with everything else") instead of trusting qwen to notice
  the exception.** *(clause new — deck 135)*
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
  Hellrider out) + burn to face; if ≥ opponent life, attack with everyone and fire all burn."
- **State the WHY in ONE clause, only for load-bearing rules** — so a rule that looks locally wrong is
  trusted anyway ("attack with the 0-power creature — its battle cry pumps the team"; "always pay the
  fetch sacrifice — that cost is the point"). qwen needs the move, not the theory.
- **Give explicit PERMISSION where the pilot is wrongly timid and explicit BRAKES where it is wrongly
  reckless.** Weak models need both stated. "Sweep proactively — it's fine to trade one card for two
  creatures" is as necessary as "NEVER chain two sweepers on one board."
- **Override the misleading generic prior BY NAME**, with a half-line of why, so the base prompt's prior
  doesn't win ("The general prompt says hold interaction — IGNORE that here; this burn is reach, throw
  it at the face"). **This includes priors that collide with YOUR OWN directives: a deck-guide order
  loses to a conflicting general prior unless the guide names the prior and states the exemption.
  "Cast every copy of Astrolabe" lost to the general anti-redundant-copies rule until rewritten as
  "a second copy is NEVER redundant — the redundant-copies rule does not apply; each one is a free
  card." For each directive you write, ask: which general prior argues against this, and does my
  wording beat it?** *(strengthened — deck 135)*
- **Ground qwen in the action list, not its plan.** *(new — deck 135)* Where the logs show the carried
  plan contradicting the board (claiming an action already happened, counting phantom damage), give
  the explicit tiebreak: "the action list is the truth — if an option is still offered, it has NOT
  been done; if an attacker is not in the attackers list, it CANNOT attack this turn."
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
- **Burying the lede** — the deciding rule below nice-to-know card notes; a correct rule in paragraph
  nine is a rule qwen skips.
- **Branch explosion** — every "but if…" is a fork qwen can get wrong; collapse to the best default,
  branch only when the default genuinely loses. Don't over-hedge exceptions until the default gets lost.
- **Symmetry blindness** — prescribing spend-freely or hoard without measuring the deck's redundancy.
- **Over-length** — a long guide buries its own most important line.
- **Freezing last wave's diagnosis.** *(new — deck 135)* Assuming the previous #1 leak is still the
  leak, or that a rule that fixed it is beyond suspicion. Once obeyed, a rule's own default becomes
  a candidate cause of the NEXT failure — re-mine fresh every wave.
- **Misdiagnosing never-castable as timidity.** *(new — deck 135)* "It never cast its finisher" has
  three different fixes depending on whether the card was never drawn, never castable, or declined —
  prescribing courage for a mana problem writes a rule qwen cannot follow.

## Definition of done (checklist)

- [ ] Every card claim matches the engine's `text=`/`auto=`, not memory — including what each
      search/fetch effect can legally find.
- [ ] The #1 rule at the top fixes the single biggest leak you actually found in THIS deck's logs,
      in imperative form.
- [ ] On a revision wave: last wave's rules were audited for compliance; obeyed rules are compressed,
      not deleted; every old default was checked as a possible cause of the new failure. *(new — deck 135)*
- [ ] The win condition/engine is named and told to be deployed, near the top.
- [ ] The deck's color/castability bottleneck (if any) has an explicit, sequenced acquisition rule.
      *(new — deck 135)*
- [ ] Role-by-matchup is stated as a default + flip condition, not left for the pilot to derive.
- [ ] A mechanical lethal/math check is present if the deck can close with reach.
- [ ] Every "DO NOT" traces to a real observed decision in the logs.
- [ ] Generic-prompt priors that misfire for this deck are explicitly overridden by name — including
      priors that collide with the guide's own directives. *(extended — deck 135)*
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
shorter, more directive, and iterate on that lever before adding anything else. **When a wave's rule DOES
land, expect the next corpus to fail somewhere new downstream — that is the loop working, not the guide
failing; return to Step 0.** Note: shipping the guide as a Res asset needs `git add -f` (`bin/Res` is
gitignored). End by naming, in your own words, the deck's plan and the single biggest piloting problem
the guide is built to fix.
