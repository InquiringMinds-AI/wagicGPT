# Skill: Writing a per-deck strategy guide for the qwen pilot

## Revision notes (deck 44)

Field test: the wave2 skill was followed end to end to produce the deployed deck44 guide;
that guide then played 6 corpus games (1-2-3 with three timeouts). The revisions below are
each grounded in an observed failure of a guide that FOLLOWED the wave2 skill — i.e. holes
in the method, not just in one guide. Inline markers `[REV-44 #n]` show where each change
landed in the body.

1. **Hold-language always beats deploy-language — interaction needs FIRE TRIGGERS.**
   The old guide said "Go for the Throat and Counterspell are PREMIUM. Save them" plus a
   loud DEPLOY rule. Result: qwen died at 4 life with GfT + Counterspell unspent (vs109),
   and let a 13/13 resolve holding two counters (vs135). Value framing ("premium", "save",
   "don't waste") trains infinite holding; an abstract deploy exhortation never overrides
   it at the moment of choice. New style rule: every hold/save instruction MUST ship with
   explicit numeric fire triggers and a dump-at-low-life clause. (Step 5, new bullet.)
2. **Teach the rules mechanics the executor provably lacks.** vs133, the engine OFFERED
   "Cast Oona's Gatewarden {u/b}" off Islands four turns running; qwen's PLAN said "I
   cannot cast it without a Swamp" and passed. The wave2 skill told the AUTHOR about hybrid
   mana but never said to teach the EXECUTOR. New: mine logs for refused-legal-actions
   where the model's stated reason is a rules error, and put the rules line in the guide
   ("{U/B} pays with either color"; "if the engine offers a cast, the cost IS payable").
   (Step 2 diagnostic + Step 5 bullet.)
3. **Repeatable-cost activations need mechanical when-NOT-to rules.** The old guide already
   cautioned "only pay {1}{U} to untap Sleep-Cursed Faerie when doing so lets it ATTACK or
   BLOCK this turn" — and qwen still paid it twice on the summoning-sick cast turn (vs133)
   and three times post-combat/on empty boards (vs140). "When it helps" requires the
   reasoning qwen doesn't do. The rule must be keyed to observable state: "NEVER the turn
   it entered; ONLY in main phase 1 when it will attack this combat." (Step 5 bullet.)
4. **Guide lines leak into the wrong decision seam.** "A hand of counters and removal with
   NO creatures is LOSING" was written as an in-game deploy line; it fired at the MULLIGAN
   seam instead — qwen shipped 3 of 6 openers, including 4 lands + creature + interaction,
   once into a one-lander. Every loud sentence will be pattern-matched at EVERY seam it
   lexically fits (mulligan, blockers, priority). Audit for cross-seam leakage; state
   mulligan as explicit keep/ship conditions. (New pitfall + Step 4 item 8 hardened.)
5. **State the #1 rule as a per-option-list procedure, not a posture.** "DEPLOY, DO NOT
   SIT" lost to the general prompt's "hold instant-speed cards" prior: Archmage of Echoes
   (4 copies, the marquee card) was cast ZERO times in six games — offered turn after turn
   vs an empty board and declined for counter mana. A posture competes with priors; a
   procedure over the visible option list ("if any Cast option shows a creature, take it")
   doesn't. Corollary: when logs show flash creatures held for whole turn cycles and never
   flashed in, explicitly DE-FLASH them ("treat as normal main-phase creatures") — the lost
   finesse is cheap, the observed holding is fatal. (Step 4 item 2 + Step 5 bullet.)
6. **The translog can contain harness artifacts — check `reply` against `choice` before
   attributing a misplay.** vs109 seq18: the reply text said "4. Stromkirk Noble" but the
   engine consumed choice 3 — Go for the Throat destroyed qwen's OWN Obyra. That is a
   parse/consume bug, not a model decision; teaching against it would be teaching a
   non-problem. Also seen: `choice: -1` with an empty reply (model/timeout failure).
   Extends "confirm before you accuse." (Step 2, new paragraph.)
7. **What was relied on and confirmed working (kept, unchanged):** extract-don't-eyeball
   digesting; the bad-play / never-offered / bad-hand triage (it's what proved the hybrid
   refusal was a teachable model gap, not an engine gap); engine-text verification (caught
   the old guide claiming Archmage copies "faeries and counterspells" — the `auto=` copies
   Faerie/Wizard PERMANENT spells only); front-loading; the DO-NOT-from-observed-plays
   closing section.

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
   [REV-44 #5] Know which priors are STRONG in practice: "hold instant-speed cards for the
   opponent's turn" reliably beats any abstract deploy posture your guide states. If your deck's
   plan depends on casting things the model can find a hold-shaped excuse for (flash creatures,
   a payoff it wants to "protect", counter mana), your guide must override that prior BY NAME
   with a procedure, not a posture (see Steps 4 and 5).

3. **Card text from the ENGINE, not your memory** — for every non-basic card, pull its
   `text=` and `auto=` lines from `bin/Res/sets/primitives/*.txt` (grep by `name=` across
   `mtg.txt`, and also `planeswalkers.txt` / `borderline.txt` if present). **This is what
   the engine actually enforces and what the AI is shown. Oracle text you remember, the
   printed cardface, and the engine `auto=` script can all DISAGREE — the script wins.**
   Verify the exact numbers you will cite (mana cost, power/toughness, keywords, ETB,
   activated-ability costs, devotion/threshold counts, X values) against it. This step also
   audits the PREVIOUS guide when revising: deck44's old guide told qwen Archmage of Echoes
   copies "faeries and counterspells" — the `auto=` copies Faerie/Wizard PERMANENT spells
   only. [REV-44 #7] Note that hybrid mana ({B/R}, {R/W}) is castable with EITHER color, so
   a "gold" card may be mono-color-castable in this deck. Use `/usr/bin/grep` — bare `grep`
   here is a wrapper that can silently drop matches.

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

[REV-44 #6] **Separate the harness's mistakes from the model's.** The translog records what
was CONSUMED, and the consumer can be wrong. Before attributing any misplay, read the raw
`reply` for that record and check it agrees with `choice`: deck44's log has a record where the
reply plainly says "4. Stromkirk Noble" but the engine consumed choice 3 and Go for the Throat
destroyed qwen's OWN creature — a parse/consume bug, and a guide written against it would
teach a non-problem. Also expect `choice: -1` with an empty reply (a failed/timed-out model
call that fell back). Log these as harness findings to report upstream; keep them OUT of the
guide's mistakes list.

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
lost — that became Rule #1. For deck44, the marquee 4-of Archmage of Echoes was CAST ZERO TIMES
across six games while "Cast nothing" was chosen with it castable — that became Rule #1.)
Most decks have one such lever. Find it before you write a word.

**Diagnostic patterns to hunt for (weight the guide toward the ones that recur):**
- **Offered-many-times, passed-every-time** — a capability qwen doesn't understand it should use.
- **Cast-nothing / durdle rate.** TALLY the fraction of casting decisions where qwen chose "cast
  nothing," and list which cards it NEVER cast across all games. (Deck140's pilot passed 82% of
  casting decisions and cast its card-engine and all three finishers ZERO times — that one
  statistic defined the whole guide.) Whole turns of passing with castable cards and open mana =
  the deck isn't developing.
- **Hoarding — answers/threats that pile up UNUSED in hand** while the opponent stays alive.
  Over-holding is as real a failure as over-firing. (Deck109 sat on Pillar of Flame / Searing
  Spear as if they were reactive removal, when they were reach it should have thrown at the face —
  "burn is damage, not removal." Deck44 died at 4 life with Go for the Throat and Counterspell
  still in hand, and separately let a 13/13 resolve holding two counters.)
- [REV-44 #2] **Refused-legal-actions with a stated rules error.** When qwen declines an OFFERED
  action, read its PLAN text for the reason. If the reason is a rules misconception ("I cannot
  cast Oona's Gatewarden without a Swamp" — {U/B} is hybrid, an Island pays it), you have found a
  teachable gap: the guide must state the rules fact flatly, plus the umbrella rule "if the engine
  offers a cast, the cost IS payable." The option list is the engine's LEGAL set — the model
  second-guessing legality is always wrong, and it provably does.
- [REV-44 #3] **Repeatable-cost abilities burned at useless times.** The inverse of
  passed-every-time: an activation taken when it does nothing (paying to untap a summoning-sick
  creature the turn it entered; post-combat activations with no effect on the game). Mana burned
  on these is development not happening.
- **Failure to close — end-state sweep.** Print each game's final life totals; a game where the
  opponent's life PLATEAUED for many turns while qwen had gas is a failure to close. Treat a
  TIMEOUT the same way: a 40-minute cap with qwen ahead on life and a hoarded hand is a loss the
  results table just can't record.
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
  sweeper caution, or a deck whose payoff must break the second-main-phase default, or flash
  creatures the "hold instants" prior keeps in hand forever.) Name the prior and override it
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

**Confirm before you accuse.** When a play looks wrong, read that decision's full `prompt` to check
what was actually legal / on the stack — and its full `reply` to check the model actually chose it
(see the harness-artifact paragraph above). (Deck135's Force of Negation LOOKED like a reflexive
misfire until the prompt showed it was countering a real Cruel Edict — so it stayed OUT of the
mistakes list.) Never teach against a non-problem, and never list a "mistake" you didn't observe.

[REV-44 #4] **Audit the PREVIOUS guide's lines against the logs.** When revising a deployed guide,
each of its loud sentences is a hypothesis that already ran. Check what each one actually did:
deck44's "a hand of counters and removal with NO creatures is LOSING" was written to force in-game
deployment — but qwen pattern-matched it at the MULLIGAN seam and shipped keepable openers (3 of 6,
once into a one-lander). A sentence fires at EVERY decision seam its words fit, not the seam you
wrote it for. For each guide line ask: at which other seams (mulligan, blockers, priority,
targeting) could this wording plausibly match — and did it, in the logs?

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
   WHY. Make it unmissable. [REV-44 #5] Phrase it as a PROCEDURE OVER THE OPTION LIST, not a posture:
   "DEPLOY, DO NOT SIT" ran for a whole corpus and lost every time to the "hold instants" prior;
   "if any Cast option shows a creature and you haven't cast one this turn, take it" is checkable
   against the visible list and leaves nothing to override.
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
8. **MULLIGAN** — one or two lines, as explicit KEEP/SHIP conditions on countable things (land count,
   creature count), never as vibes. [REV-44 #4] This section also GUARDS the mulligan seam: any loud
   "hand of X is losing"-shaped line elsewhere in the guide will be applied here unless the mulligan
   rule explicitly keeps that hand ("KEEP 3+ lands with removal and no creature — you will draw
   creatures").
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
- [REV-44 #1] **Never write a hold/save instruction without FIRE TRIGGERS.** "PREMIUM — save it for
  real threats" reads to a weak executor as "never spend it": deck44 died with removal and counters
  in hand in both real losses. Every "hold/save" rule must carry (a) explicit numeric conditions that
  FORCE the spend ("counter the FIRST spell costing 4+ mana or any creature with power 4+"; "kill any
  creature that grows with +1/+1 counters ON SIGHT"), (b) a low-life dump clause ("at 10 life or less,
  spend ALL interaction NOW"), and (c) the one-clause why ("dying with it in hand loses games").
  Value framing is not an instruction; a trigger is.
- [REV-44 #2] **Teach the rules mechanics the logs prove the executor lacks.** One flat line each, only
  for observed misconceptions: "{U/B} is hybrid — ONE mana of EITHER color; an Island casts it." Plus
  the umbrella: "if the engine offers a cast, the cost IS payable — never refuse an offered action
  because you think it's illegal." Do not pre-teach the whole comp rules; teach the gap you saw.
- [REV-44 #3] **Repeatable-cost activations get a mechanical NEVER/ONLY pair.** "Only when it helps"
  demonstrably fails (the old deck44 guide had exactly that and qwen paid the untap on a summoning-sick
  creature anyway). Key both sides to observable state: "NEVER pay {1}{U} to untap it the turn it
  entered. Pay ONLY in main phase 1 when it will attack this combat."
- [REV-44 #5] **De-finesse flash when the logs show holding.** If flash creatures sit in hand across
  turn cycles and never get flashed in, write "treat X and Y as normal creatures: cast them in your
  main phase; do NOT hold them for a better moment." The ambush finesse you give up is worth far less
  than the turns of development you get back.
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
  it at the face").
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
  doesn't implement. This includes text INHERITED from the previous guide: re-verify its card claims
  too (deck44's old guide misdescribed its own marquee card's copy trigger). [REV-44 #7]
- **Guessing at misplays** instead of reading the translogs — the real failures are often not the ones
  you'd predict; or fixing a one-off slip instead of the recurring pattern; or listing a "mistake" you
  never observed (invented problems waste attention and can steer qwen wrong).
- [REV-44 #6] **Blaming the model for the harness** — a `choice` that contradicts its own `reply`, or a
  `choice: -1`, is a parse/consume or call failure. Report it upstream; never write guide text against it.
- [REV-44 #4] **Cross-seam leakage** — a loud line written for one decision (deploy!) pattern-matching at
  another (mulligan) and doing damage there. Reread every emphatic sentence asking "what does this tell
  the pilot at the mulligan / blockers / priority seams?" and pin the seams you don't mean with explicit
  rules of their own.
- **Burying the lede** — the deciding rule below nice-to-know card notes; a correct rule in paragraph
  nine is a rule qwen skips.
- **Branch explosion** — every "but if…" is a fork qwen can get wrong; collapse to the best default,
  branch only when the default genuinely loses. Don't over-hedge exceptions until the default gets lost.
- **Symmetry blindness** — prescribing spend-freely or hoard without measuring the deck's redundancy.
- **Over-length** — a long guide buries its own most important line.

## Definition of done (checklist)

- [ ] Every card claim matches the engine's `text=`/`auto=`, not memory — including claims carried
      over from the previous guide. [REV-44 #7]
- [ ] The #1 rule at the top fixes the single biggest leak you actually found in THIS deck's logs,
      in imperative form — phrased as a procedure over the option list, not a posture. [REV-44 #5]
- [ ] The win condition/engine is named and told to be deployed, near the top.
- [ ] Role-by-matchup is stated as a default + flip condition, not left for the pilot to derive.
- [ ] A mechanical lethal/math check is present if the deck can close with reach.
- [ ] Every "DO NOT" traces to a real observed decision in the logs — and none of them is actually a
      harness artifact (reply/choice mismatch, choice -1). [REV-44 #6]
- [ ] Generic-prompt priors that misfire for this deck are explicitly overridden by name.
- [ ] Every hold/save rule carries numeric fire triggers and a low-life dump clause. [REV-44 #1]
- [ ] Rules misconceptions observed in the logs (hybrid mana, offered-means-legal) are taught as flat
      one-liners. [REV-44 #2]
- [ ] Repeatable-cost activations have a NEVER/ONLY pair keyed to observable state. [REV-44 #3]
- [ ] Every emphatic line has been checked for cross-seam leakage (mulligan especially); the mulligan
      section states explicit KEEP conditions for hands other lines might scare it off. [REV-44 #4]
- [ ] Every rule is imperative, concrete (named cards, real numbers), low-branching, and executable
      from the board + action list alone — no inference required.
- [ ] Nothing repeats the general prompt; it fits on roughly one screen (~40-70 lines).

## Output and handoff

Write the finished guide as PLAIN TEXT (it slots into `{STRATEGY_GUIDE}` verbatim). No markdown headers
are needed; short ALL-CAPS section labels read fine and help qwen find the part it needs. The deck8 /
deck44 guides are the format and density reference.

The draft is LLM-authored; the user does a Magic-judgment authorship pass and owns the final guide — so
surface the card-rules details you verified and flag anything you were unsure of. Also hand upstream any
harness findings your log mining surfaced (reply/choice mismatches, silent decision seams, empty-reply
fallbacks) — they are not guide material but they cost games. [REV-44 #6] Where possible, VALIDATE
by real play, not by looking correct: re-run self-play with the guide in place and read the new logs —
did qwen start taking the plan-critical action? If not, the guide isn't blunt enough; make Rule #1 louder,
shorter, more directive, and iterate on that lever before adding anything else. Note: shipping the guide as
a Res asset needs `git add -f` (`bin/Res` is gitignored). End by naming, in your own words, the deck's plan
and the single biggest piloting problem the guide is built to fix.
