# Skill: Writing a per-deck strategy guide for the qwen pilot (wave3 revision — deck 131)

## Revision notes (deck 131)

Field test: followed the wave2 skill against the 20260709 corpus for deck 131 (blue-red
"Mind Control", 1W/1L/4 timeouts). The wave2-era guide it replaced was well-formed by the
skill's own rules — imperative, named cards, observed-mistake DO-NOTs — and still produced
four timeouts, because the skill let a guide be built on a posture the harness cannot
execute. Changes, each with its evidence:

1. **NEW hard step: the EXECUTABILITY AUDIT (Step 2, first substep).** Before writing any
   rule, verify the harness actually OFFERS the decision windows the rule needs. Count
   `kind:priority` records per game. Evidence: deck 131 received ZERO opponent-turn
   priority windows in 5 of 6 games (4 total, all in one game). The deck carries 12+
   reactive instants; the deployed guide's spine was "hold these, act on the opponent's
   turn, cast draw on their end step" — a plan that was structurally impossible to
   execute. Zero counterspells, zero Aetherize, zero Opportunity were cast across the
   entire corpus. A guide built on unoffered windows is dead text no matter how correct.

2. **NEW style rule: passivity language is load-bearing — never write "pass with mana
   open" as a default.** Evidence: the deployed guide's timing section said "otherwise
   PASS with mana up" and "HOLD these"; qwen over-applied it into choosing "Cast nothing
   right now" in 75% of casting decisions (70/93), including 20 consecutive turns sitting
   on 10 untapped lands and a full grip (vs deck140, life frozen 20–19 → timeout). On a
   weak executor a hold instruction becomes the whole personality. Hold-rules must be
   narrowly scoped, and every guide needs an explicit act-floor ("cast a spell every
   turn") stated ABOVE them.

3. **NEW diagnostic: the offered-vs-cast tally per card.** Script a per-card count of
   "appeared as a castable option" vs "was actually cast" across the corpus, plus
   "ever reached hand." Evidence: this one table exposed deck 131 instantly (Opportunity
   0 casts, counters 0 casts, Elixir 0 activations, Guttersnipe in hand in 1 of 6 games).
   It also catches the upstream cause: the win engine "never drawn" traced to draw spells
   never cast, not to variance.

4. **NEW: treat TIMEOUT as its own outcome class in the end-state sweep.** Evidence:
   4 of deck 131's 6 games were 40-minute timeouts with life totals frozen for 10-20
   turns — mutual durdle. A timeout is not noise; it is the passivity failure in its
   purest form, and the corpus's dominant result (13/21). The fix is always an act-floor
   (cast every turn / attack every turn), not more card-specific advice.

5. **STRENGTHENED: when revising an existing guide, re-verify every inherited rule and
   DO-NOT against the NEW corpus and DROP the ones that never fire.** Evidence: the
   deployed guide's DO-NOT list warned against firing counters into an empty stack — a
   misplay that never occurred in this corpus (the four priority passes it did make were
   correctly reasoned: shroud + insufficient mana). Meanwhile that warning's real effect
   was to reinforce hoarding. Stale cautions aren't just wasted attention; they can back
   the wrong posture.

6. **ADDED diagnostic: land-drop choice.** Evidence: the vs-deck44 loss was substantially
   caused by choosing "Play Island" three turns running while the hand's only Mountain
   and the deck's payoff creature (Young Pyromancer, {1}{R}) sat stranded — payoff landed
   turn 12 vs an opponent at 20. Land-drop picks are `ask` decisions and easy to miss;
   check them explicitly when the deck has a splash color.

7. **ADDED to Step 5: sequencing-within-a-turn rules need stating.** Evidence: qwen cast
   Downsize BEFORE casting Young Pyromancer in the same turn — no token. "Payoff first,
   then spells" is invisible to a weak executor unless written.

What the wave2 skill got RIGHT and I relied on (kept verbatim in spirit): the two-models
frame; extract-don't-eyeball (the compact digest made every finding above readable);
confirm-before-you-accuse (saved me from listing the shroud passes as misplays); engine
`auto=` text over memory (Prism Ring's mechanism, Downsize's targeting); isolate-the-one-
lever; front-loading; the DO-NOT-traces-to-observed-decision rule.

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
- **(NEW — deck131) Hold-instructions metastasize.** Any "hold / wait / pass with mana
  open" rule gets over-applied far beyond its intended scope — the executor treats
  passivity as the safe choice and your hold-rule as license. "Cast nothing right now"
  is always on the menu and is qwen's favorite exit. Passive language anywhere in the
  guide must be narrowly scoped ("hold X ONLY until Y") and outranked by an explicit
  act-floor near the top ("cast a spell every turn"; "attack every turn unless Z").

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
   **(NEW — deck131) The "hold instants for the opponent's turn" prior deserves special
   suspicion**: it is only executable if the harness actually grants opponent-turn priority
   windows, which for most decks it barely does (see the executability audit below). For an
   instant-heavy deck, expect to OVERRIDE this prior by name, not instantiate it.

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

5. **(NEW — deck131) The previous guide, if one was deployed** — the corpus you are reading
   was played WITH it in the prompt, so every pattern in the logs is partly its output.
   Attribute each observed behavior: did the old guide CAUSE it (a directive followed too
   well), fail to prevent it, or have nothing to do with it? Rules the executor followed
   into failure must be REMOVED or inverted, not just supplemented — text you keep, you
   re-endorse.

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
- **(NEW — deck131) Color access for the splash.** If the deck splashes (e.g. 8 Mountains in
  a 22-land blue deck for two red payoffs), the guide needs a land-drop rule ("play Mountain
  before Island while a red card is in hand") and a mulligan clause for color-stranded hands.
  The executor picks lands by list position otherwise.

## Step 2 — Mine the logs: find the deciding lever (this is where the value is)

**(NEW — deck131) FIRST: the EXECUTABILITY AUDIT.** Before judging any play, establish which
decision surfaces the harness actually gives this deck. Tally `kind` counts per game. In the
deck-131 corpus, 5 of 6 games contained ZERO `priority` decisions — the pilot was never asked
anything on the opponent's turn — and blockers asks were nearly absent corpus-wide. The
consequences are structural, not stylistic:
- Reactive plans (counterspells, fogs, combat tricks, flash, "cast draw on their end step")
  need opponent-turn windows. If the logs show those windows rarely arrive, **the cards are
  near-dead and the guide must not build on them** — demote them explicitly ("these are the
  LAST cards in your hand to matter; never keep mana open waiting") and route the deck's
  mana into proactive lines instead.
- "Keep mana open" advice converts directly into stalled turns when no window ever consumes
  the open mana.
- A rule the executor CANNOT execute doesn't just underperform — it displaces the rule you
  should have written. Check executability for every posture-level claim in your draft:
  which `kind` of decision, at which phase, would carry it out, and does that decision
  actually appear in the logs?

A rule that corrects an OBSERVED mistake is worth ten generic tips. Do not theorize about
qwen's play — read what it did, then separate three things a guess would blur: **bad play
vs. never-had-the-option vs. bad hand (variance).** The `prompt` shows exactly what qwen was
OFFERED, so this is fact, not guess — and only the first two are yours to fix.

**Extract, don't eyeball.** Files are large; script a compact digest over the JSONL that
prints, per decision: the `kind`, the phase, life totals, hand, both battlefields, the
numbered options, and — crucially — the TEXT of the CHOSEN action (regex the `choice`-indexed
line out of the `prompt`). A ~30-line Python loop does this. Now you can read a whole game as
the sequence of what qwen actually did. Sample ACROSS games and across all decision kinds.

**(NEW — deck131) Second mandatory table: the offered-vs-cast tally.** For every nonland card,
count across the whole corpus: (1) decisions where it was a castable option, (2) times it was
actually cast, (3) games where it ever reached hand. This one table finds the leaks fastest:
- Cards at many-offers/zero-casts are capabilities qwen refuses (deck 131: Opportunity 0,
  Elixir 0 activations, all counters 0 — across six games).
- A win-condition that rarely reaches hand (Guttersnipe: 1 game of 6) is often DOWNSTREAM of
  another refusal — the draw spells never being cast — not variance. Chase the cause upstream
  before blaming luck.
- Compute the cast-nothing rate: fraction of casting decisions where "Cast nothing right now"
  was chosen while something was castable. Above ~50% the deck is not playing Magic; the #1
  rule is an act-floor before it is anything card-specific.

**By decision kind, what to inspect:**
- **ask** — casting, targeting, land drops, mulligan, modal menus. Watch sequencing/tempo (is
  it deploying threats or durdling on support spells?), removal/burn fired at the wrong target
  or too early, and whether it ever targets ITSELF. **(NEW — deck131) Include land-drop picks:**
  playing the wrong basic while a color-stranded spell sits in hand is an `ask` misplay that
  cost deck 131 its winnable loss (three straight "Play Island" picks with the only Mountain
  and the {1}{R} payoff stranded until turn 12).
- **priority** — activated abilities (equip, tap-for-value, cracking fetches, mana, instants).
  **THE most overlooked seam.** Plan-critical actions often hide here (e.g. "Equip with Cranial
  Plating [cost: {1}]", "crack fetchland") and qwen passes them. Check whether the deck's engine/
  payoff ability is EVER actually being activated — and, per the executability audit, whether
  priority windows exist at all for this deck.
- **attackers / blockers** — is it playing its role? A beatdown deck that declares "none" over
  and over, or a control deck that chump-races, is misplaying. **(NEW — deck131) Count attackers
  decisions per game:** one attackers ask in a 30-turn game means the pilot stopped presenting
  attacks entirely (deck 131 vs deck140: attacked once, then 20 straight turns of no combat
  against an empty board).
- **target choices** (removal, discard/hand-strip) — where the biggest gaps show. qwen picks by
  superficial cues, not impact: stripping a 1/1 mana dork or a mana rock while a sweeper/counter/
  bomb sits in the same hand. Judge each pick: was the highest-impact target available, and did
  it take it?

**The single highest-value move: isolate the ONE lever.** Compare the games qwen played WELL
against the ones it LOST and find the one repeated difference. (For deck110, every good game
equipped Cranial Plating early and swung; every bad game cast Platings, never equipped them,
tapped out, and durdled — that single delta became Rule #1. For deck135, a "crack fetchland"
action was offered 16 times in a game and passed 16 times; qwen made no mana, cast nothing, and
lost — that became Rule #1. For deck131, the win was the one game it attacked every turn; the
timeouts were 75% "Cast nothing" — the lever was an act-floor: cast a spell and attack every
turn.) Most decks have one such lever. Find it before you write a word.

**Diagnostic patterns to hunt for (weight the guide toward the ones that recur):**
- **Offered-many-times, passed-every-time** — a capability qwen doesn't understand it should use.
- **Cast-nothing / durdle rate.** TALLY the fraction of casting decisions where qwen chose "cast
  nothing," and list which cards it NEVER cast across all games. (Deck140's pilot passed 82% of
  casting decisions and cast its card-engine and all three finishers ZERO times; deck131 passed
  75% and cast zero counters, zero Opportunity across six games.) Whole turns of passing with
  castable cards and open mana = the deck isn't developing.
- **(NEW — deck131) Frozen-life timeouts.** Treat TIMEOUT as its own outcome class, not a
  no-result. Print each game's life-total trajectory; many consecutive turns with BOTH totals
  frozen is mutual durdle, and your deck's share of it is the thing to fix. 13/21 games in the
  20260709 corpus timed out — for a stalled game, ask "what should this deck have been casting
  every one of those turns?" (usually its card draw) and "why did it stop attacking?".
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
- **Wrong modal/menu picks** — color choices, X values, overload-vs-single. (Deck131 once named red
  instead of blue every time; after the wave2 guide's "choose BLUE" line it chose blue every time —
  proof that a concrete named directive lands.)
- **Inconsistency** — the SAME decision made differently across games (unleash taken here, declined
  there) means qwen has no stable rule; give it one.
- **Generic-prior misfires** — a universal prior that is WRONG for THIS specific deck. (The old
  blanket passivity and cost-refusal misfires are now fixed in the general guide; what remains are
  deck-specific overrides — e.g. a fourteen-sweeper deck that should spend freely against the generic
  sweeper caution, or a deck whose payoff must break the second-main-phase default, or — deck131 —
  an instant-heavy deck for which "hold instants for the opponent's turn" is unexecutable and must
  be inverted.) Name the prior and override it explicitly with the deck-specific rule.
- **Cost-line misreads.** Any deck action whose cost LOOKS bad but is routine (fetch sacrifice, life
  payment, phyrexian mana, sac-for-value) needs an explicit "this cost is the point — always pay it."
- **Engine realities that gate the plan.** qwen can't equip if it taps out (no {1} left), can't
  attack profitably with an unequipped 0-power body. These become concrete "keep a land open" rules.
  **(NEW — deck131) Within-turn ordering is an engine reality too:** a spell cast before the payoff
  creature lands produces no trigger (deck 131 cast Downsize, then Young Pyromancer, same turn — no
  token). Write "payoff first, spells second" explicitly.
- **Subtle leaks in decks qwen already plays "okay"** (aggro especially — it curves out and mostly
  looks fine). Hunt the quiet ones: burn hoarded vs thrown at the face; a go-wide payoff (Hellrider,
  a battalion enabler) out but qwen holding back "bad attacks" that are correct because of the
  trigger; not knowing when to stop trading and just race; the cheap repeated micro-choice it gets
  subtly wrong every game. A leak that costs a fraction of a turn every turn is invisible
  per-decision and decisive over a game.

**Confirm before you accuse.** When a play looks wrong, read that decision's full `prompt` to check
what was actually legal / on the stack. (Deck135's Force of Negation LOOKED like a reflexive misfire
until the prompt showed it was countering a real Cruel Edict — so it stayed OUT of the mistakes list.
Deck131's four priority passes looked like fog-hoarding until the prompts showed the targets had
shroud and the mana was short — correctly reasoned, so NOT a misplay.) Never teach against a
non-problem, and never list a "mistake" you didn't observe.

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

**(NEW — deck131) When REVISING a deployed guide: re-earn every inherited line.** The new corpus
was played WITH the old guide, so it is a direct test of each old rule. Three dispositions:
- KEEP a rule the logs show working (deck131's "Prism Ring: choose BLUE" — followed every time).
- INVERT/REMOVE a rule the executor followed into failure ("PASS with mana up" → the 75%
  cast-nothing rate; do not soften it, delete it and state the opposite).
- DROP a DO-NOT whose misplay never occurred in the corpus ("don't fire counters into an empty
  stack" — never happened; the warning only reinforced hoarding). A DO-NOT must trace to a
  decision in THIS corpus, not to a previous wave's.

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
   the turn-by-turn deployment priority. **(NEW — deck131) If the deck carries reactive spells the
   harness can't reliably let it cast, add a short "the truth about your reactive spells" block:
   state plainly that the window rarely comes, demote the cards by name, and say what to do with
   the mana instead.** The executor cannot discover a harness limitation on its own; it will keep
   waiting for a turn that never arrives.
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
- **(NEW — deck131) Audit your own draft for passivity verbs.** "Hold," "save," "wait," "keep mana
  open," "pass" — each one is an instruction the executor will over-obey. Every hold-rule must carry
  its release condition in the same sentence ("hold X ONLY until Y; if Y hasn't happened by turn N /
  this window, cast it") and must sit BELOW an act-floor rule. If your draft's most memorable verb is
  "hold," you have written a timeout.
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
  **(NEW — deck131) "Cast nothing right now" is itself an in-game surface** — the guide can and
  should name it: "Do NOT pick 'Cast nothing right now' while X is castable."
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
  creatures" is as necessary as "NEVER chain two sweepers on one board." **(NEW — deck131) For a deck
  the harness forces to be proactive, permission includes "you will NOT be punished for tapping out" —
  otherwise the general prompt's keep-mana-open prior wins and the deck durdles.**
- **Override the misleading generic prior BY NAME**, with a half-line of why, so the base prompt's prior
  doesn't win ("The general prompt says hold interaction — IGNORE that here; this burn is reach, throw
  it at the face").
- **Turn `#HINT` / `auto=` behaviors into one-liners the pilot can rely on** ("Blightsteel auto-attacks
  once it's in play — your only job is to CAST it").
- **Hard NEVER/ALWAYS for known bad defaults** ("NEVER strip their weakest card when a sweeper, counter,
  or bomb is in their hand"; "NEVER cast Guttersnipe with no burn to follow").
- **(NEW — deck131) State within-turn ORDER where a trigger depends on it** ("cast Young Pyromancer
  FIRST, then your instants — a spell cast before the creature lands makes no token").
- **Short.** It rides in every prompt and competes with the live board for attention. Cut anything that
  fails the "does this change a chosen number?" test.

## Pitfalls

- **Writing for yourself, not qwen** — the subtlest trap. Nuanced, hedged, branch-heavy prose reads well
  to you and gets ignored or misread by the executor. If a line requires the reader to do the reasoning
  you did, qwen won't; spell out the conclusion.
- **(NEW — deck131) Writing for a harness that doesn't exist.** The guide can only act through the
  decision windows the harness actually offers. A textbook-correct control plan ("hold counters, act at
  instant speed") is a timeout machine if opponent-turn priority never arrives. Executability first,
  correctness second — a slightly worse plan the executor can actually run beats the ideal one it can't.
- **Repeating the general prompt** — wastes qwen's attention budget and buries the deck-specific signal.
- **Card text from memory** instead of the engine `auto=`/`text=` — you'll teach a rule the engine
  doesn't implement.
- **Guessing at misplays** instead of reading the translogs — the real failures are often not the ones
  you'd predict; or fixing a one-off slip instead of the recurring pattern; or listing a "mistake" you
  never observed (invented problems waste attention and can steer qwen wrong).
- **(NEW — deck131) Inheriting stale rules on revision.** A previous guide's DO-NOTs and card advice are
  hypotheses the new corpus just tested. Keeping a caution whose misplay never occurs isn't harmless —
  it spends attention and can reinforce the executor's worst default (a "don't waste your counters"
  line becomes one more reason to hold everything).
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
- [ ] **(NEW — deck131) Every posture-level rule passed the executability audit: the decision window
      it needs (`kind` + phase) actually appears in this deck's logs.**
- [ ] **(NEW — deck131) The offered-vs-cast tally was computed; every never-cast card is either given
      an explicit directive or explicitly demoted — none left to rot silently.**
- [ ] The win condition/engine is named and told to be deployed, near the top.
- [ ] Role-by-matchup is stated as a default + flip condition, not left for the pilot to derive.
- [ ] A mechanical lethal/math check is present if the deck can close with reach.
- [ ] Every "DO NOT" traces to a real observed decision in THIS corpus — inherited DO-NOTs whose
      misplay never occurred were dropped.
- [ ] Generic-prompt priors that misfire for this deck are explicitly overridden by name.
- [ ] **(NEW — deck131) No unscoped passivity verb survives: every "hold/save/wait/keep mana open"
      carries its release condition, and an act-floor rule outranks them all.**
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
shorter, more directive, and iterate on that lever before adding anything else. **(NEW — deck131) In the
validation logs, also re-check the harness-level counters: priority-window count, cast-nothing rate,
attackers-asks per game, and the offered-vs-cast tally — these move even when win/loss is noisy, so they
are your low-variance signal that the guide changed behavior.** Note: shipping the guide as a Res asset
needs `git add -f` (`bin/Res` is gitignored). End by naming, in your own words, the deck's plan and the
single biggest piloting problem the guide is built to fix.
