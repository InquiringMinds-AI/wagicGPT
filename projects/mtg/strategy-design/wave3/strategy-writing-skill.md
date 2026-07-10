# Skill: Writing a per-deck strategy guide for the qwen pilot (wave 3)

## Synthesis notes (wave 3)

Unified from seven independent deck-agent revisions (decks 44, 109, 110, 131, 133, 135, 140)
of the wave2 skill, all mining the same 21-game corpus (13 timeouts; dominant failure:
"Cast nothing" paralysis driven by the "Mana in your pool: (none)" misread, amplified by
hold/conserve guide language). What changed and why:

**Merged (proposed independently by 3+ agents — the convergent core):**
- *False world-model beliefs* as a first-class failure tendency, with the fix pattern
  "state the corrective fact flatly, then quote-and-forbid the recurring wrong thought"
  (44, 109, 110, 133, 140).
- *Mine the `reply` text, not just choices* — the digest recipe now requires it; the WHY
  decides which kind of fix works (109, 110, 133, 140).
- *Offered-vs-taken per-card tally* as a required Step 2 extraction (131, 133, 140).
- *Brake/hold discipline*: prefer positive sequencing; any surviving hold needs a visible
  scope, a numeric fire trigger/release, and a concurrent-action bound, under an explicit
  act-floor (44, 110, 131, 133). The four formulations compose rather than conflict; I made
  110's "positive sequences first" the primary rule because prohibitions are the root risk,
  and folded the others in as the requirements a surviving brake must meet.
- *Step 0 revision mode* — the corpus is guide-conditioned; attribute misplays to the
  deployed guide first; expect the failure to have moved downstream (109, 110, 131, 135).
  Adopted 135's full Step-0 formulation (richest), folded in 109's guide-echo evidence rule
  and 131's drop-stale-rules rule, rather than 109's lighter "input #0" placement.
- *Stale-PLAN poisoning / deferred-plan loops* diagnostic + grounding counter-move
  (110, 133, 135, 140). *Anti-deferral now-phrasing* and plan/number-mismatch check (110, 133).
- *Timeouts as an outcome class + wall-clock economics* (110, 131): a timeout is the
  passivity failure in pure form, and shortening the game is the timeout fix.
- *Closed-exception pattern*: exhortations lose to locally-plausible excuses; "Cast nothing"
  needs an explicit, enumerated permission list (44, 140 — 44's "procedure over the option
  list, not a posture" and 140's "checklist with closed exception" are the same idea; kept
  140's wording, 44's option-list framing).

**Adopted from single agents (divergent but genuinely additive method):**
- Executability audit — verify the harness OFFERS the decision windows a rule needs, before
  writing the rule (131; kept as Step 2's first substep, not a new step, to preserve structure).
- Castability/color-dependency analysis in Step 1 (135).
- Mine WINS with a clock lens / wins as mirrors of losses (109 + 135, merged).
- Named exceptions inside absolutes — "obedient absolutism" (109).
- Rules-delusion perception procedures ("count the Mountains NOT marked [tapped]") (109).
- Phase-localized checklists (109). Cross-seam leakage audit (44). Repeatable-activation
  when-NOT-to keyed to observable state (44). Harness-artifact check (reply vs `choice`) (44).
- Circular self-locking prohibitions → numeric GO thresholds (133). Identity prose drives
  behavior (133). Corrective facts carried in the guide but flagged for promotion upstream (133).
- Pre-empt collisions with general priors, not just observed misfires (135). Waste loops (110).
- Land-drop diagnostic (131). Mulligan keyed on a countable (140). Never-drawn ≠ validated (109).
- Mandatory post-revision validation with a diff against the PREVIOUS wave (110).

**Rejected:**
- Citing the observed failure stat inside the guide itself ("last run you picked Cast nothing
  76 of 101 times") (133) — self-flagged as unvalidated; risks the pilot pattern-matching the
  failure text; not method until tested.
- Per-deck inline markers and deck-specific rule content (all agents) — evidence stays as
  examples only where it teaches the pattern.
- Separate "de-flash the flash creatures" style rule (44) — sound, but it is one instance of
  the brake/hold rule; kept as an example clause there, not a rule of its own.
- Duplicated restatements of existing wave2 text that agents re-worded without changing the
  method (several) — kept the wave2 wording.

**Task-3 consistency pass (after the wave3 general-strategy revision):** the wave3 general
prompt now carries, at the general layer, what wave2-era guides had to carry as stopgaps —
the affordability/auto-tap contract, the rules facts (attacking taps creatures not lands,
summoning sickness is not a casting veto, X chosen at cast, hybrid mana), a universal
closed-exception "Cast nothing" rule with an act-floor, hold-release valves, stale-plan
grounding, anti-deferral, activation brakes, a mulligan land-count default, and an explicit
guide-beats-priors precedence clause. This skill was updated accordingly: Inputs #2 now
describes the new inventory and converts "carry the corrective fact + flag for promotion"
into "reinforce only where the logs show the general line still losing"; Step 3 rank #2,
the Step 5 mechanics/closed-exception/override bullets, Step 4's mulligan item, and the
checklist were aligned to the same rule. No method rule was invalidated by the general
revision — placement changed, the craft did not.

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

Four failure tendencies to design against:
- **Locally-reasonable, globally-blind.** Each pick looks fine in isolation while the
  overall plan quietly never happens (the threat never gets deployed, the payoff never
  gets activated). Your job is to name the plan-critical action so plainly qwen takes it
  every time.
- **Shallow-heuristic defaults.** When the guide leaves a decision open, qwen falls back
  to bad defaults: pick by list position, by "biggest creature," by "a card is a card,"
  or by the safe/passive option when unsure. Every "consider / think about / weigh / it
  depends" is a sentence that hands the decision back to those defaults. Convert judgment
  into directives: **"DO X when Y," never "consider X."**
- **False world-model beliefs.** qwen holds stable, recurring misconceptions about the game
  and its interface, and they silently veto whole classes of correct actions: "Mana in your
  pool: (none)" read as "I cannot cast anything" while untapped lands sit on its printed
  battlefield; "attacking taps my lands"; hybrid {U/B} read as needing a Swamp; X-spells
  read as dealing 0. A strategy rule built on an uncorrected misconception never fires —
  the guide must state the corrective FACT flatly (see Step 5).
- **Brake over-generalization / obedient absolutism.** A weak executor does not scope a
  caution; it universalizes it — "leave one land untapped to equip" becomes "never spend
  mana"; "save your removal" becomes "never cast anything"; identity prose like "you win
  late, stabilize first" becomes license to do nothing all game. And it OBEYS your hard
  rules into losses: an absolute with an unstated exception is a trap you set yourself
  (deck109's pilot declined to burn a Young Pyromancer, quoting the guide's "burn goes
  face" rule, while Pyromancer tokens killed it). Every DON'T you write will be applied
  far beyond the situation you meant. Design accordingly (see Step 5).

Write imperative, front-loaded, low-branching, concrete, evidence-driven. If a sentence
doesn't change which number qwen picks, cut it.

## Step 0 — Revising an EXISTING guide (the wave loop)

When a deployed guide was in play during the corpus you are mining, do this before
anything else. The logs no longer show qwen's raw defaults — they show the qwen×guide
interaction, and the guide itself is a suspect.

- **Read the deployed guide as evidence, and attribute each misplay to a specific line:**
  missing, wrong, or too weak to override a general-prompt prior. Grep the replies for
  ECHOES of the guide's own phrasing — when qwen QUOTES a guide sentence in its PLAN while
  misplaying, the guide caused the play; that is a guide bug, not a model bug. (It also
  proves your exact wording propagates into the executor's plans — phrasing has real
  leverage in both directions.) A rule the executor cites while misplaying is a rule to
  rewrite or delete.
- **Audit last wave's rules against the new logs: did each one get FOLLOWED?** A rule that
  now gets followed has done its job — DEMOTE and compress it (keep it, shorter; deleting
  it risks regression) and spend the reclaimed space on the new leak. A rule that never
  fired in the new corpus (the misplay it warns against no longer occurs) is a candidate
  to DROP: stale cautions aren't just wasted attention, they can reinforce the wrong
  posture (deck131's "don't counter into an empty stack" was backing the hoarding).
- **Expect the failure to have MOVED downstream.** Fixing the blocking action exposes the
  next bottleneck in the same chain (fetches now get cracked → but every fetch takes the
  wrong land → the deck still casts nothing). Mine the new corpus fresh; do not assume
  last wave's diagnosis still names the biggest leak.
- **Suspect your own defaults.** Any default the old guide prescribed ("fetch Forest by
  default," "hold X for Y") is a candidate CAUSE of the new failure, because qwen follows
  it more literally than you meant. For each old default, check the logs for games where
  obeying it was the mistake. Re-derive the deck's dependencies from scratch each wave.
- **Keep what worked, verbatim where possible** — churn in lines qwen already obeys is
  pure risk.

## Inputs to read, in this order

1. **The decklist** — `bin/Res/ai/baka/deckN.txt`. Format: `#NAME/#DESC/#HINT` headers,
   then `CardName (SET) * qty`; bare numbers are basic-land ids; the SET code is
   irrelevant. This tells you the archetype's intent.
   - **The `#HINT` lines are for the OTHER (heuristic Baka) AI, not qwen.** Read them for
     the designer's intent and any encoded combo/attack rule, but note where a hint would
     MISLEAD this deck's ideal play. Don't propagate a hint blindly.

2. **The general system prompt** — `bin/Res/ai/gpt/system_prompt.txt`. EVERY deck already
   gets this, so READ THE CURRENT VERSION — the wave3 revision covers, as UNIVERSAL priors
   and rules facts: decide who's the beatdown; act every turn under an explicit act-floor
   ("Cast nothing" allowed only via a closed exception list); deploy your win conditions;
   instant-vs-non-instant timing WITH release valves on holding (rare opponent-turn windows,
   fire-at-life-thresholds, holding-a-card-never-holds-the-turn, burn/draw as your-turn
   spells); choose targets by IMPACT; activation gas AND brakes (this-turn-effect only, no
   repeat activations); cards-vs-tempo; lethal math run twice (combat + second main) plus a
   survival check and a free-case attack floor; a mulligan land-count default; the
   interface contract (every listed action is legal AND payable, lands auto-tap, empty pool
   is normal, lists are complete, listed ≠ recommended); the mechanics facts (attacking
   taps creatures not lands, summoning sickness is never a reason to hold a creature, X is
   chosen at cast, hybrid pays either color); stale-plan grounding (board beats plan,
   still-offered = not done); anti-deferral ("next turn" = now); plan/number binding; cost
   paying incl. prompt fetch-cracking; and a PRECEDENCE clause — the deck guide beats the
   general priors on conflict. **Your guide LAYERS ON TOP — never RESTATE any of this; add
   only what is specific to THIS deck.** The general guide states PRINCIPLES; a weak pilot
   won't apply an abstract principle to the specific card in front of it, so your per-deck
   job is the concrete INSTANTIATION: it says "activate your permanents" — you say "EQUIP
   Cranial Plating turn 3, keep {1} open." Litmus test: if a line you're about to write is
   already true of EVERY deck, the general guide has it — cut it and write the named-card
   version instead. Separately, note where a general prior still MISLEADS this specific
   deck and must be overridden explicitly (see the logs step).
   **One caveat to the no-restatement rule:** a corrective fact or behavioral guard the
   general prompt ALREADY carries earns a deck-guide line only when the NEW corpus shows
   the general statement still LOSING to a local pattern — then reinforce it tied to the
   specific card and the exact recurring wrong thought, not as a restatement. A recurring
   misconception the general prompt does NOT yet cover may be countered in the deck guide
   as a stopgap — flag every such line in your handoff as a candidate for promotion into
   the general prompt or the board representation, so it can later be removed from every
   guide at once.

3. **Card text from the ENGINE, not your memory** — for every non-basic card, pull its
   `text=` and `auto=` lines from `bin/Res/sets/primitives/*.txt` (grep by `name=` across
   `mtg.txt`, and also `planeswalkers.txt` / `borderline.txt` if present). **This is what
   the engine actually enforces and what the AI is shown. Oracle text you remember, the
   printed cardface, and the engine `auto=` script can all DISAGREE — the script wins.**
   Verify the exact numbers you will cite against it. Note that hybrid mana ({B/R}, {U/B})
   is castable with EITHER color, so a "gold" card may be mono-color-castable in this deck.
   Use `/usr/bin/grep` — bare `grep` here is a wrapper that can silently drop matches.

4. **The game logs** — the deck's self-play translogs, JSONL, one line per decision qwen
   made piloting THIS deck: `~/.Wagic/ai/gpt/selfplay-runs/<run>/*-<deck>-*.jsonl`. Fields:
   `kind` (ask|priority|attackers|blockers), `prompt` (the FULL board + numbered legal
   actions qwen actually saw), `reply` (its text — the PLAN/reasoning), `choice` (the index
   it picked). (`options` is just a count — ignore it.) **This is your primary evidence.**

## Step 1 — Read the deck: archetype, engine, role, redundancy

Answer these before writing a word — they are the guide's spine:

- **Archetype & clock.** Aggro / tempo / midrange / control / combo / prison? How does it
  actually kill? Roughly how many turns is its clock ("kill by turn 4-6")?
- **The win engine / win condition.** Name the specific 1-3 cards that end games and the
  exact interaction that makes them win. A weak pilot will never find these unaided — the
  guide's spine is "set up, protect, and deploy the payoff." Guides teach the DECK'S
  engine, not general Magic.
- **Role by matchup.** State the DEFAULT posture in one line, then the narrow condition
  that flips it, as IF-THEN keyed on the opponent archetype; do NOT make qwen re-derive it.
- **Redundancy vs scarcity — MEASURE it.** Count the deck's copies of each effect. A deck
  with 12+ removal spells can spend them freely; a deck with one bomb must protect it.
  Redundancy flips the correct advice, so measure before you prescribe.
- **Castability / color dependency.** Count the sources of each color; list which spells
  are STRANDED without each color; verify what each search effect can legally find
  (Windswept Heath cannot fetch an Island; Into the North can). If one color gates half
  the deck, the guide's fetch/land-drop rules must prioritize it explicitly — castability
  can be the whole game (deck135's "fetch Forest by default" caused a blue screw that
  stranded its hand for 10+ turns).
- **The curve / sequencing that matters.** What it wants to do turns 1-2-3-4, plus any
  hard ordering ("payoff before the cheap spells," "land before spells to keep mana open").

## Step 2 — Mine the logs: find the deciding lever (this is where the value is)

**FIRST: the EXECUTABILITY AUDIT.** Before judging any play, establish which decision
surfaces the harness actually gives this deck: tally `kind` counts per game. Deck131's
corpus had ZERO opponent-turn `priority` windows in 5 of 6 games — its guide's whole spine
("hold these, act on the opponent's turn") was structurally impossible to execute, and its
12+ reactive instants were near-dead cards. If the windows a plan needs rarely arrive:
the guide must not build on them — demote those cards explicitly ("the LAST cards in your
hand to matter; never keep mana open waiting") and route the deck into proactive lines.
"Keep mana open" converts directly into stalled turns when no window ever consumes the
mana. For every posture-level rule you draft, ask: which `kind` of decision, at which
phase, would carry it out — and does that decision actually appear in the logs? A rule the
executor cannot execute doesn't just underperform; it displaces the rule you should have
written.

A rule that corrects an OBSERVED mistake is worth ten generic tips. Do not theorize about
qwen's play — read what it did, then separate three things a guess would blur: **bad play
vs. never-had-the-option vs. bad hand (variance).** The `prompt` shows exactly what qwen
was OFFERED, so this is fact, not guess — and only the first two are yours to fix.

**Extract, don't eyeball.** Files are large; script a compact digest over the JSONL that
prints, per decision: the `kind`, the phase, life totals, hand, both battlefields, the
numbered options, the TEXT of the CHOSEN action (regex the `choice`-indexed line out of the
`prompt`), **and the `reply`/PLAN text.** A ~30-line Python loop does this. Sample ACROSS
games and across all decision kinds.

**Read the REPLY text, not just the choice — mine the stated WHY.** The choice stream tells
you WHAT went wrong; the reply tells you WHY, and the why decides which fix works. Grep
replies for recurring false beliefs ("no mana", "cannot cast", "my lands are tapped", a
card claimed in the wrong zone, an ability claimed on a card that lacks it, "attacking taps
my lands") and CHECK each against the same record's printed board — a stated reason that
contradicts the prompt's own board state is a systematic misconception, and the fix is a
flat corrective FACT, not a strategy rule (deck133: 61 replies claimed "no mana" while
untapped Swamps sat on the battlefield; no amount of "deploy threats" fixes a pilot that
believes it cannot pay). Also hunt refused-legal-actions where the stated reason is a rules
error (hybrid mana "needs a Swamp"; an offered cast declined as unaffordable — the engine
only offers PAYABLE actions), and plan/number mismatches (the reply says "cast X now"
while the chosen option is "Cast nothing"). Tally recurring phrases; one-offs are noise.

**Tally offered-vs-taken per card — required.** A ~15-line script counting, per key card,
how often "Cast <card>" (or the key activation) appeared in the options vs. was chosen,
plus whether the card ever reached hand. This is the single most decisive diagnostic in the
corpus (deck140: Rakdos's Return offered 26 / taken 0, Elixir 55/0; deck133: Bloodghast
77/0) and it catches upstream causes too (a win engine "never drawn" traced to draw spells
never cast, not variance). Pair it with the CAST-NOTHING TALLY: the fraction of casting
decisions where qwen chose "Cast nothing."

**By decision kind, what to inspect:**
- **ask** — casting, targeting, land drops, mulligan, modal menus. Watch sequencing/tempo,
  removal fired at the wrong target or too early, whether it ever targets ITSELF. Check
  LAND-DROP picks explicitly when the deck has a splash color — "Play Island" three turns
  running while the payoff needs the hand's only Mountain is an easy-to-miss game-loser.
- **priority** — activated abilities (equip, tap-for-value, cracking fetches, mana,
  instants). **THE most overlooked seam.** Check whether the deck's engine/payoff ability
  is EVER actually activated — and, per the executability audit, whether these windows
  even arrive.
- **attackers / blockers** — is it playing its role? A beatdown deck declaring "none" over
  and over, or a control deck chump-racing, is misplaying.
- **target choices** — was the highest-impact target available, and did it take it?

**The single highest-value move: isolate the ONE lever.** Compare the games qwen played
WELL against the ones it LOST or STALLED and find the one repeated difference. Treat WINS
as mirrors of the losses: deck135's two wins were the same deck with one variable flipped
(Island fetched early → threats deployed → attacks every turn); the lever came from the
win/stall delta, not from staring at losses. And audit wins with a CLOCK lens: compare
earliest-possible kill vs actual kill per game — deck109's four wins all closed 2-6 turns
later than the hand allowed; "won anyway" hides the same leak that loses the close games
and causes the timeouts.

**Treat TIMEOUT as its own outcome class.** A 40-minute cap with life totals frozen for
10-20 turns is mutual durdle — the passivity failure in its purest form, and this corpus's
dominant result (13/21). Wall-clock economics make it strategy: each decision costs
~60-70s of inference and games hit the cap at only ~35-50 decisions, so every durdle
decision and waste loop burns clock. A guide that shortens the game (kill by turn 5-6) is
also the timeout fix. The remedy is an act-floor (cast every turn / attack every turn),
not more card-specific advice.

**Diagnostic patterns to hunt for (weight the guide toward the ones that recur):**
- **Offered-many-times, passed-every-time** — a capability qwen doesn't understand it
  should use (the offered-vs-taken table finds these).
- **False-belief vetoes.** A recurring reply-side misconception (can't-pay, wrong-zone,
  wrong-ability, wrong-rule) silently vetoing legal actions. Distinguish from strategic
  passivity — the fixes differ (corrective fact vs. directive).
- **Cast-nothing / durdle rate** and the cards NEVER cast across all games.
- **Hoarding — answers/threats piling up UNUSED in hand** while the opponent stays alive.
  Over-holding is as real a failure as over-firing, and it is this executor's dominant
  direction. Watch the general prompt's hold-instants prior METASTASIZING: "hold
  interaction" generalizes in a weak model to "hold everything," and a guide's own HOLD
  line feeds it.
- **Deferred-plan loops / stale-PLAN poisoning.** The pilot's PLAN line is carried into
  its next prompt, so "I will attack NEXT turn" gets re-read and re-affirmed forever —
  procrastination is self-reinforcing by construction. Hunt for the same "next turn"
  intention in 3+ consecutive plans while the action stays untaken, and for plans carrying
  hallucinated state that beats the visible board ("the Vista has already been sacrificed"
  while the crack option is still listed; "my lands are tapped" while the board shows them
  untapped).
- **Waste loops** — the inverse of offered-and-passed: a repeatable-cost action re-taken
  every turn for no new effect (re-equipping an already-equipped creature "to ensure the
  bonus is active"; untapping a summoning-sick creature). Burns mana AND clock.
- **Failure to close — end-state sweep.** Print each game's final life totals; opponent
  life PLATEAUED for many turns while qwen had gas = failure to close.
- **Missing lethal.** Sum a turn's potential damage and check whether qwen had lethal and
  didn't take it.
- **Reactive spells fired into nothing** — a counter on an empty stack, a trick on an
  empty board.
- **Wrong modal/menu picks** — color choices, X values, overload-vs-single.
- **Inconsistency** — the SAME decision made differently across games means qwen has no
  stable rule; give it one.
- **Generic-prior misfires** — a universal prior that is WRONG for THIS deck. Name the
  prior and override it explicitly.
- **Cost-line misreads.** Any action whose cost LOOKS bad but is routine (fetch sacrifice,
  life payment, phyrexian mana) needs "this cost is the point — always pay it."
- **Engine realities that gate the plan** (can't equip if tapped out) → concrete "keep a
  land open" rules — but per Step 5, written as scoped positive sequences, never blanket
  prohibitions.
- **Illegal-plan errors.** Plans built on actions the rules forbid (attacking with a
  summoning-sick creature "for 9" three decisions running). A one-line mechanics
  restatement tied to the specific card is cheap and earns its place.
- **Subtle leaks in decks qwen already plays "okay"** — burn hoarded vs thrown at the
  face; holding back "bad attacks" that a go-wide payoff makes correct; not knowing when
  to stop trading and race. A leak that costs a fraction of a turn every turn is invisible
  per-decision and decisive over a game.
- **Never-drawn ≠ validated.** Zero log evidence about a card is VARIANCE, not proof the
  guidance works. Keep its lines proportional; don't build Rule #1 on an unobserved card,
  but don't cut it either.

**Confirm before you accuse.** When a play looks wrong, read that decision's full `prompt`
to check what was actually legal / on the stack (deck135's Force of Negation LOOKED
reflexive until the prompt showed a real Cruel Edict; deck131's priority passes were
correctly reasoned shroud/mana calls). Also check the translog itself for harness
artifacts before attributing a misplay: compare the `reply`'s stated pick against the
`choice` the engine consumed (deck44 saw a reply naming option 4 while choice 3 was
consumed — a parse bug, not a decision; also `choice: -1` with an empty reply = timeout).
Never teach against a non-problem, and never list a "mistake" you didn't observe.

## Step 3 — Decide what goes in vs. what gets cut

Include a line ONLY if it changes a decision qwen will actually face and gets wrong (or
would). Rank candidates by **frequency × cost-to-winning:**
1. The fix for the biggest observed leak / the deciding lever (always first).
2. The corrective FACT for any systematic false belief the replies exposed (can't-pay,
   wrong-rule, wrong-zone) — but check the CURRENT general prompt first: if it already
   states the fact, include a deck-guide line only when the new logs show that statement
   still losing locally; if it doesn't, carry the fact and flag it for promotion upstream.
3. The win engine and how to set it up, protect it, and deploy it.
4. Role + matchup flips (concrete, deck-specific).
5. Target-priority and removal/sweeper-selection rules.
6. Per-card timing for the cards that decide games; a lethal-math check if the deck closes
   with reach.
7. Mulligan, in a line or two.

**CUT:** anything the general prompt already covers; general Magic theory; card-by-card
tours of filler; deep multi-turn combos qwen can't sequence; long matchup trees; hedged
"it depends" prose; rare edge cases; anything you can't turn into a "do/don't with a named
card"; and (in revision mode) inherited rules that never fired in the new corpus. When
unsure, CUT — length dilutes the rules that matter.

## Step 4 — Structure the guide (front-loaded by impact)

qwen weights early text heavily and may not deeply read the tail. Order by what most
changes its play. A reliable skeleton (adapt, don't pad):

1. **Identity + role**, 2-4 lines: archetype, how it wins, the clock, who it's the
   beatdown against, and the posture. **Identity prose drives behavior as much as rules
   do**: a tempo-negative self-description ("you are not fast; you win late; stabilize
   first") reads as license to do nothing. Even a slow deck's identity must be written as
   actions per turn ("you win late BY casting a threat every turn and draining in chunks"),
   never as patience.
2. **THE #1 RULE** — the fix for the single biggest leak, hammered in its own visually
   loud block at the very top: WHAT the action is, the exact option text qwen will SEE,
   its enabling constraint, and a one-clause WHY. **State it as a procedure over the
   visible option list, not a posture.** "DEPLOY, DO NOT SIT" loses to the general
   prompt's hold prior every time (deck44's marquee creature: 4 copies, cast ZERO times in
   six games under a loud DEPLOY posture); "if any Cast option shows a creature, take it"
   does not. Exhortations always lose to a locally-plausible excuse — use a
   take-the-first-that-appears checklist with a closed exception (see Step 5).
3. **THE ENGINE / how the deck wins** — name the finishers and the exact sequence.
   Turn-numbered deploy scripts beat priority prose: "Turn 2: CAST BLOODGHAST" is
   executable; "deploy black threats mid-game" is not. Write multi-step engines as a
   NUMBERED mechanical sequence.
4. **Standing operating rules / the core action loop.** Where the observed failure lives
   in one specific phase, key the checklist to the visible phase label ("MAIN PHASE 2
   CHECKLIST — run it every turn after combat") — it outperforms turn-generic principles
   the model must map onto the phase itself.
5. **Key cards — one imperative line each**, ordered by how often each decides the game.
   Only cards with a non-obvious right line; skip vanilla creatures. Include a
   HOW-IT-OPERATES line for every X-spell and priority-seam ability the pilot provably
   fumbles.
6. **A lethal / math check** the pilot can run mechanically, if the deck closes with reach.
7. **Combat notes** — only if the deck has non-obvious combat.
8. **MULLIGAN** — the general prompt now carries a land-count default (keep 2-5 lands +
   a castable early spell), so include a line ONLY to SPECIALIZE it for this deck ("count
   Astrolabe as a land"; "ship any hand without a red source") or when the logs show the
   default losing — and keep it keyed on a COUNTABLE, never on hand "quality": qwen mulled
   a keepable 3-land hand for a verbal reason into an unkeepable one-lander.
9. **DO NOT** — a tight list of the 3-5 specific misplays you actually observed, each as a
   flat imperative phrased as the exact situation qwen was in.

**Audit for cross-seam leakage before shipping.** Every loud sentence will be
pattern-matched at EVERY decision seam it lexically fits, not just the one you wrote it
for: deck44's in-game line "a hand of counters and removal with NO creatures is LOSING"
fired at the MULLIGAN seam and shipped good openers. Reread each rule asking "which other
decision kinds could this sentence match?" — and state mulligan/blocker/priority rules in
their own seam's terms.

Match the deck8 / deck44 example density and length (~one screen, roughly 40-70 lines).
Longer is not better — every extra line dilutes the early ones.

## Step 5 — Word it for the executor (non-negotiable style rules)

- **Imperative and directive.** "EQUIP the Plating." "Cast Wrath when the opponent has 2+
  creatures." Never "consider," "you might," "weigh whether" — qwen does not act on
  hedges; it falls back to its bad default.
- **Default + narrow, enumerable exceptions** — not open judgment and not a decision tree.
  If a rule needs three conditions, qwen will drop two. **And name the exceptions INSIDE
  your absolutes**: qwen obeys hard rules off a cliff, so an ALWAYS/NEVER whose exception
  you know about must carry it ("burn goes FACE — EXCEPT kill Young Pyromancer or
  Guttersnipe on sight"). Opponent decklists are known — name the actual cards it will face.
- **The act-floor comes first, above any hold language.** Never write "pass with mana
  open" or "hold X" as a default posture — on a weak executor a hold instruction becomes
  the whole personality (deck131: 75% "Cast nothing," 20 consecutive turns on 10 untapped
  lands). State the floor explicitly and early: "cast a spell every turn; a turn where you
  cast nothing is a misplay unless [closed exception]."
- **Prohibitions over-fire — prefer positive sequences, and scope every surviving brake.**
  The executor universalizes any DON'T ("DO NOT TAP OUT" → "never spend mana"). Rules:
  1. Say what to DO in what ORDER and let the order imply the caution: "cast your
     creatures FIRST; equip costs only {1}" instead of "don't tap out." Sequencing within
     a turn must be stated ("payoff creature BEFORE the cheap spells") — it is invisible
     to the executor otherwise.
  2. A brake that must exist needs (a) a SCOPE keyed to something visible on the board,
     (b) a numeric FIRE TRIGGER / release ("hold Blast for a blocker — but when the
     opponent is at 8 or less, ALWAYS fire it at their face"; "dump everything when YOU
     are at 5 or less"), and (c) a concurrent-action bound ("holding Fatal Push must NEVER
     stop you casting your creature this turn"). A brake without a release becomes
     permanent. Value framing ("premium," "save," "don't waste") trains infinite holding.
  3. Never write a prohibition whose precondition depends on the pilot's OWN other
     behavior — it self-locks ("NEVER cast Gray Merchant as your only black permanent" +
     a pilot that never builds a board = Merchant never cast). Convert to a numeric GO
     threshold ("cast Merchant at 4+ devotion; at 6+ it usually wins").
  4. After drafting, reread every DON'T and ask: "if the executor applies this sentence to
     EVERY decision for the rest of the game, does the deck still function?" If not,
     rewrite it.
  Where logs show flash creatures held forever, DE-FLASH them ("treat as a normal
  main-phase creature") — the lost finesse is cheap; the observed holding is fatal.
- **Closed exception for "Cast nothing."** The general prompt now carries the universal
  closed-exception rule; the deck guide's job is to TIGHTEN it for this deck when the logs
  demand ("'Cast nothing' is allowed ONLY when every offered spell is a sweeper AND the
  opponent has no creatures") — a permission left open is a permission slip qwen writes
  itself every turn.
- **Teach the mechanic qwen provably lacks — as fact, then refute the false belief
  verbatim.** The universal mechanics/interface facts (offered = payable, lands auto-tap,
  empty pool is normal, attacking doesn't tap lands, X chosen at cast, hybrid mana) are
  now IN the general prompt — do not restate them wholesale. Reinforce one in the deck
  guide only when the new logs show it still losing, and then tie it to the specific card
  ("Treefolk CANNOT attack the turn you cast it") and quote-and-forbid the recurring wrong
  thought by name ("NEVER pick 'Cast nothing' with the excuse 'I have no mana' — that
  excuse is always false when a cast is offered") — qwen pattern-matches its own phrasing;
  a named forbidden sentence intercepts where a paraphrase misses. Where a delusion
  resists flat facts, give a LOOK-AND-COUNT perception procedure instead: "count the
  Mountains NOT marked [tapped] — that number is your available mana." Include
  state-recognition cues ("(5/1) (printed 1/1)" means the equipment is ALREADY attached).
- **Repeatable-cost activations need a when-NOT-to keyed to observable state.** "Only when
  it helps" requires reasoning qwen doesn't do; write "NEVER the turn it entered; ONLY in
  main phase 1 when it will attack this combat."
- **Anti-deferral wording.** For the plan-critical action say "NOW / the first turn it
  appears / in THIS reply," and license the imperfect version explicitly ("cast it even if
  you cannot also equip this turn"). The general prompt now carries universal anti-deferral,
  plan/number-binding, and stale-plan-grounding lines — add a deck-guide version only when
  the new logs show the pattern persisting, and anchor it to the deck's own action ("the
  EQUIP happens THIS turn — if 'Equip' is still listed, it has NOT been done").
- **Low branching; use a ranked list or a selection TABLE, not nested IF/THEN.** A
  priority order (1, 2, 3) with a one-clause reason each, or a board→card mapping ("one
  creature → Edict; many small → Pyroclasm; anything big → Wrath"). Key unavoidable
  branches on something qwen can SEE. (The table pattern is field-proven: deck140's pilot
  followed its sweeper table correctly every time it cast at all.)
- **Tie every rule to the in-game surface.** Quote the phrasing the engine actually shows:
  "When you see 'Equip with Cranial Plating [cost {1}]', take it." Name cards EXACTLY as
  they appear in the action list.
- **Concrete, named, numeric.** Name cards and costs; state thresholds numerically and say
  what they turn on. "Develop your board" is invisible to qwen; "cast at least 3 other
  black permanents first" is executable.
- **Give arithmetic it can run.** "Damage = unblocked attacker power + (1 per attacker if
  Hellrider out) + burn to face; if ≥ opponent life, attack with everyone and fire all burn."
- **State the WHY in ONE clause, only for load-bearing rules** — so a rule that looks
  locally wrong is trusted anyway ("attack with the 0-power creature — its battle cry
  pumps the team").
- **Give explicit PERMISSION where the pilot is wrongly timid and explicit BRAKES where it
  is wrongly reckless** — but the two are not symmetric in risk: an over-fired permission
  costs a card; an over-fired brake can cost every turn of the game. This executor's
  dominant failure mode is passivity — when in doubt, default to permission.
- **Override the misleading generic prior BY NAME** ("The general prompt says hold
  interaction — IGNORE that here; this burn is reach, throw it at the face"). The general
  prompt now states that the deck guide WINS on conflict — but that precedence clause only
  fires when the executor recognizes a conflict, so still name the prior. And don't
  wait for an observed misfire: scan your OWN directives for collisions with general
  priors and pre-empt them ("cast every copy — the 'redundant copies' caution does NOT
  apply to Astrolabe") — an unnamed collision is resolved by the executor's habit, not by
  the precedence clause.
- **Turn `#HINT` / `auto=` behaviors into one-liners the pilot can rely on** ("Blightsteel
  auto-attacks once in play — your only job is to CAST it").
- **Hard NEVER/ALWAYS for known bad defaults** — with their exceptions named (see above).
- **Short.** It rides in every prompt and competes with the live board for attention. Cut
  anything that fails the "does this change a chosen number?" test.

## Pitfalls

- **Writing for yourself, not qwen** — nuanced, hedged, branch-heavy prose reads well to
  you and gets ignored or misread by the executor. Spell out the conclusion.
- **Fixing last wave's leak so hard you cause this wave's.** A brake written against an
  observed over-spend becomes the next corpus's paralysis (deck110: "don't tap out" →
  0/6, five timeouts, the model citing the guide's own words while durdling). Every fix
  is a new default the executor will over-apply — run the Step 5 brake audit on your fixes.
- **Identity prose that licenses passivity** — "you win late / stabilize first" is a rule
  to the executor even though you wrote it as flavor.
- **Circular, self-locking prohibitions** — a DON'T whose escape condition depends on
  behavior the DON'T itself prevents.
- **Cross-seam leakage** — a loud in-game line firing at the mulligan/blocker seam it
  lexically matches.
- **Repeating the general prompt** — wastes qwen's attention budget and buries the
  deck-specific signal.
- **Card text from memory** instead of the engine `auto=`/`text=` — you'll teach a rule
  the engine doesn't implement.
- **Reading only choices, not replies** — the digest shows the misplay; the PLAN text
  shows the misconception that caused it, and they need different fixes.
- **Guessing at misplays** instead of reading the translogs; fixing a one-off instead of
  the recurring pattern; listing a "mistake" you never observed.
- **Burying the lede** — a correct rule in paragraph nine is a rule qwen skips.
- **Branch explosion** — collapse to the best default; branch only when the default
  genuinely loses.
- **Symmetry blindness** — prescribing spend-freely or hoard without measuring redundancy.
- **Over-length** — a long guide buries its own most important line.

## Definition of done (checklist)

- [ ] Every card claim matches the engine's `text=`/`auto=`, not memory.
- [ ] The #1 rule at the top fixes the single biggest leak found in THIS deck's logs,
      stated as a procedure over the visible option list (not a posture), in imperative form.
- [ ] Every posture-level rule passed the EXECUTABILITY AUDIT: the decision window it needs
      actually appears in the logs.
- [ ] The `reply` text was mined; every systematic false belief is either already covered
      by the current general prompt (reinforced in the guide only if the logs show it still
      losing) or countered with a corrective FACT flagged for promotion upstream.
- [ ] An act-floor is stated above any hold language; every brake has a visible scope, a
      numeric release, and survives the "applied to every decision, does the deck still
      function?" test.
- [ ] "Cast nothing" is governed by a closed exception — the general prompt's universal
      rule, tightened or forbidden outright in the guide where this deck's logs demand it.
- [ ] The win condition/engine is named and told to be deployed, near the top.
- [ ] Role-by-matchup is stated as a default + flip condition.
- [ ] A mechanical lethal/math check is present if the deck can close with reach.
- [ ] Every "DO NOT" traces to a real observed decision in the logs; inherited DO-NOTs
      that never fired in the new corpus were dropped.
- [ ] Generic-prompt priors that misfire OR collide with your directives are overridden
      by name.
- [ ] Mulligan is keyed on a countable. Cross-seam leakage audited.
- [ ] Every rule is imperative, concrete, low-branching, and executable from the board +
      action list alone.
- [ ] Nothing repeats the general prompt; it fits on roughly one screen (~40-70 lines).

## Output and handoff

Write the finished guide as PLAIN TEXT (it slots into `{STRATEGY_GUIDE}` verbatim). No
markdown headers; short ALL-CAPS section labels read fine. The deck8 / deck44 guides are
the format and density reference.

The draft is LLM-authored; the user does a Magic-judgment authorship pass and owns the
final guide — surface the card-rules details you verified and flag anything uncertain,
plus every corrective-fact line that belongs in the general prompt or board representation
(representation defects found in log mining — misleading pool text, mangled cost rendering
— get reported upstream too; the guide carries a defensive line only while the defect is
live). **VALIDATION IS PART OF DONE-NESS, not optional**: a guide can REGRESS a deck
(deck110 went from strongest to 0/6 after a faithful wave2 revision). Re-run self-play
with the guide in place, read the new logs, and diff behavior against the PREVIOUS wave's
corpus — did the plan-critical action start happening, did cast-nothing and timeout rates
drop, and did a new downstream leak appear? If Rule #1 isn't being taken, make it louder,
shorter, more directive and iterate on that lever before adding anything else; then return
to Step 0. Note: shipping the guide as a Res asset needs `git add -f` (`bin/Res` is
gitignored). End by naming, in your own words, the deck's plan and the single biggest
piloting problem the guide is built to fix.
