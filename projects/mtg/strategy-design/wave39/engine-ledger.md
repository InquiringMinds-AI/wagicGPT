# Wave-39 consolidated engine ledger — the wave-40 step-1 docket

Sources, all four: `wave39/hold-seat-validation.md` (validator, seats 139/36/152/146 + anomaly D),
`wave39/deck125/findings.md`, `wave39/deck126/findings.md`, `wave39/deck162/findings.md`.
Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260823-040744`, binary **eabf463b1**, pilot
`qwen36-35b-a3b` (FP8 35B, standing serve :8084 — NEVER cycle, owner ruling), 21 games,
**1,436 decisions, 8 fallbacks (0.56%)**, 20/21 natural ends. Suite baseline 1047/0 + 30/0,
PARSETEST 627/0.

Ranked by (game cost observed) × (mechanism located) × (repro quality). **Severity and repro
quality stay SEPARATE fields** (wave-31 HL4). Three seats independently found item **#3**; it is
merged here with all three sets of repros, and any lane that touches it must satisfy all three.

## Cross-cutting directives for the wave-40 fix agents

- **THE OWNER RULING ON ZERO-SLACK X IS BINDING AND IS NOT ADJUDICABLE** (2026-08-23, verbatim):
  *"casting a spell, with x as zero is legal. not only that, there obviously times when a player
  should do so."* Item **#2** therefore contains **NO menu filtering**. Any patch that drops an
  {X} spell from the cast list — including the "price it with `maxAnnounceableX` at menu-build
  time" shape proposed in `hold-seat-validation.md` D1 and in `deck125/findings.md` 5.1 — is
  **REJECTED at the docket**, not at review. X=0 casts fall under the never-hard-suppress rule.
- **A pointer is not a diagnosis.** Item #1 carries the deck125 seat's crash-lane pointer
  verbatim and flagged as unverified. The lane starts from the core, not from the pointer.
- **Every render fix reports a COVERAGE FRACTION** on the next corpus (emissions-with-the-new-
  string / occasions-total), never "it renders" — items #3, #4, #5, #7, #8, #10, #12, #13.
- **The 139v125 rerun is OWED** the moment #1 lands (completeness invariant, owner ruling
  2026-08-20). Until then that matchup has no result: its `results.tsv` row is bogus (item #16).
- **Verify every card fact before acting on a reviewer's claim** (campaign standing rule). Items
  #11 and #12 are `text=` edits asserted from a Scryfall/engine diff — re-read both sources at
  the fix, do not trust this ledger's transcription.

---

# Tier 1 — completeness loss and the game-costing defects

## #1 — [P0, COMPLETENESS BLOCKER] Affinity SBA crash, 139v125 t15

- **id**: W39-CRASH-AFFINITY (status.md item 1; `deck125/findings.md` §7; validator §Engine-1)
- **Mechanism**: `__dynamic_cast` (libstdc++) inside `GameObserver::Affinity()` ←
  `GameObserver::gameStateBasedEffects()` ← `GameObserver::Update()`. Crashing *inside*
  `__dynamic_cast` rather than on a NULL return indicates a **dangling** pointer, not a
  wrong-type one. Distinct signature from the wave-37 ALord/AAMover SEGV (which item #18 records
  as VALIDATED-fixed this corpus).
- **Artifacts**: `wave39/crash/stack.txt` (full trace, thread 528628) + `w39-139v125.core`
  (gitignored, 116 MB), `game-139v125-1787476066.stderr`.
- **POINTER, NOT A DIAGNOSIS** (deck125 seat, explicitly offered as a pointer):
  `src/GameObserver.cpp:1488` holds an **unchecked** `dynamic_cast` dereference inside that
  function —
  ```cpp
  vector<Targetable*> targetlist = mExtraPayment->costs[ec]->tc->getTargetsFrom();
  for (...) { Targetable * cardMasked = *it;
              dynamic_cast<MTGCardInstance*>(cardMasked)->isExtraCostTarget = true; }
  ```
  — consistent with `mExtraPayment` holding a **stale extra-cost target list pointing at a freed
  card**. Suggested first moves, all unverified: (a) null-check the cast result before the write;
  (b) clear `mExtraPayment` when a payment completes or is abandoned; (c) fixture: mutate onto a
  host while an extra-cost payment is pending.
- **The unusual state belongs to deck139, not deck125.** deck125's board at the crash was 6 lands
  + Staff of Nin, **0 creatures**, no artifact/aura/equipment — it contributes nothing to an
  affinity computation. deck139 held
  `Everquill Phoenix {g} (4/4) [flying, reach, mutate] [mutated pile — 1 card underneath: Arboreal
  Grazer]` plus a `Feather Eve [artifact]` token. **A mutate merge destroying/merging card
  instances is the first thing to model in the fixture.**
- **Lane must produce**: line-level frames off the core (`gdb` symbol breakpoints, not line
  breakpoints — see the project skill), a deterministic fixture, the fix, suite + PARSETEST green,
  and then **the 139v125 rerun**.
- **Validated next by**: seat 139 and seat 125 in the wave-40 corpus + the rerun's paired
  `gameend` records. deck125 was **ahead 28-16** at the crash — do not read the rerun as a
  re-test of the seat's play.

## #2 — [HIGH] An explicit model pick is vetoed by Baka's DESIRABILITY roll, then replayed to phase end

- **id**: W39-D1 (validator §D + ledger D1/D2; `deck125/findings.md` §5.1 + §6). **Framed per the
  OWNER RULING above: the fix is the VALIDATOR and the REPLAY LOOP. There is no menu change.**
- **Mechanism, part (a) — the veto.** `AIPlayerGPT::FindCardToPlay` prices the model's pick
  through `AIPlayerBaka::FindCardToPlay(pMana, "*")` with `aiForcedCandidate` set.
  `AIPlayerBaka.cpp:3367` applies the X-slack penalty
  `shouldPlayPercentage -= (shouldPlayPercentage * 1.9f) / (1 + xDiff)`; at `xDiff == 0` that is
  `P − 1.9P` → **negative** (stderr: `shouldPlayPercentage = -9`). `AIPlayerBaka.cpp:3422` then
  reads
  ```cpp
  if ((forceBestAbilityUse || aiForcedCandidate) && shouldPlayPercentage > 0)
      chance = 0;
  if (chance > shouldPlayPercentage)
      continue;
  ```
  — the forced-candidate override is **gated on `shouldPlayPercentage > 0`**, so at a negative
  score the model's explicit pick can never be honoured and the candidate is `continue`d;
  `validated == NULL`. **Direction: an explicit model pick must not be subject to a desirability
  roll at all** — drop the `&& shouldPlayPercentage > 0` gate for `aiForcedCandidate` (or bypass
  the roll entirely for a forced candidate). **LEGALITY CHECKS STAY** — legality is already
  established by `LegalActionsOracle`; the roll is a *preference* heuristic and preference is the
  model's job.
- **Mechanism, part (b) — the churn.** `AIPlayerGPT.cpp` `for (int attempt = 0; ; attempt++)`
  loop (≈L8060–8150; exhaustion write ≈L8135–8150). `lastChance` is true at attempt 0 whenever
  `candidates.size() <= 1`, so the class jumps straight to `validation_reject_reask_exhausted`
  with **no intermediate reject record**. The rejected line is deliberately NOT added to
  `mStuckCastLines` (comment ≈L8078: doing so would rebuild the menu and turn a cache hit into a
  fresh HTTP call), so the next priority poll rebuilds the identical menu, `askModel` **replays
  the cached answer** (`latency_ms -1`, `prompt ""`, `reply ""` — **no model call at any of the
  five records**), validation rejects again, and another exhaustion record is written. The loop
  ends only when the phase advances. **Direction: once `lastChance` fires the window has been
  conceded to the heuristic and there is no answer left to protect — add the rejected line to
  `mStuckCastLines` for the remainder of the turn.** Turns 5 records into 1. Independently useful
  even if (a) lands alone.
- **Repro (deterministic, one binary, with a same-game control)**:
  `1787476069-ai_baka_deck125-…-vs-ai_baka_deck139.jsonl` **seq 11–15**, turn 5, Main phase 2,
  `kind: defer`, `choice -1`, `chosen_text "Sphinx's Revelation"`, `options 2`, `latency_ms -1`,
  `coded_answers 0`. Preceding real call, **seq 10**: `Mana available: 3 total` (Island {u},
  Glacial Fortress {w}|{u}, Plains {w}) → `CHOICE: 1 (Cast Sphinx's Revelation)` /
  `PLAN: Cast Sphinx's Revelation for X=1`. Three mana pays exactly `{u}{u}{w}` → announceable
  X = 0. **Control, same game, same card**: seq 18–19 (t7, one more land) →
  `shouldPlayPercentage = 1`, the forced-candidate branch zeroes the roll, and the cast is
  honoured through `Announce the value of X … X = 1` → `Action added to stack` → `Resolving`.
  Discriminator = X-affordability slack: 0 (veto) vs 1 (pass). Cost this corpus: **5 wasted
  priority windows, 0 tokens, no wrong play made**; the resolution at seq 16 shows the card still
  in hand and the mana unspent.
- **Why it is HIGH despite the vetoed play being bad**: the class is **not "X spells"**. It is
  *any* model pick whose Baka desirability score is negative — the pick is silently discarded, the
  prompt's own guarantee that every listed option is legal and payable is violated, and the pilot
  is never told. The status.md hypothesis (c3 staleness gate churning on a new-deck menu) is
  **WRONG and is retired**: the c3 gate is not involved and no menu is stale.
- **Regression fixture** (owner-directed shape): an **{X} spell with exactly its coloured pips
  available and no slack** — the GPT seat must be able to announce and commit **X = 0**, and the
  translog must show ONE record, with a model call.
- **Validated next by**: seat 125 (the pool's only {X} deck). Metrics:
  `validation_reject_reask_exhausted` count → 0 or 1 per window (never 5); a committed X=0 cast
  exists in the corpus or the fixture; zero `latency_ms -1` defer records on the cast seam.

## #3 — [HIGH, TRUST DOCTRINE] A COUNTERED spell is narrated as `resolved` — three seats, one defect

- **id**: W39-COUNTERED (deck125 §5.3 “5.3 HIGH”, deck126 **E1**, deck162 **E-162a**; merged).
  **Three independent seats found this without contact.** It affects every deck that ever faces a
  counterspell and is invisible to every per-decision metric.
- **Mechanism**: the W35 lane-B register exists — `AIPlayerGPT.cpp:2036` emits
  `"<name> was countered"`, gated on `mCounteredSpell`, set from `WEventSpellCountered` raised by
  `ActionStack::Fizzle` (`src/ActionStack.cpp:1592`) — and **never fires in this corpus**. Every
  countered spell instead narrates byte-identically to a normal sorcery resolution:
  `- Opponent's <name> resolved and went to the opponent's graveyard` /
  `- Your <name> resolved and went to your graveyard`. Start at the gate: is `mCounteredSpell`
  ever set on this path, is the event raised, and does the narration branch read it before the
  generic resolution line wins?
- **Corpus-wide fact**: `"was countered"` appears **0 times in any prompt across all 21 games**
  (the 22 corpus hits are inside models' own reasoning prose). deck125's seat alone renders
  447 + 634 `resolved and went to … graveyard` lines covering both genuine resolutions and every
  counter.
- **Repros, all three seats**:
  - deck125 vs126 s4 → s5: Essence Scatter → *"Opponent's Overgrown Battlement resolved and went
    to the opponent's graveyard"* — a **creature spell** that would have entered the battlefield
    had it truly resolved.
  - deck126 vs125 (`1787476073-…-vs-deck125.jsonl`): Chromatic Lantern (t6), Idyllic Tutor (t8),
    Exquisite Blood (t26) — all vs Dream Fracture; Pride Guardian (t30) vs Fall of the Gavel. The
    last two are **permanents narrated as having "resolved" while never entering the battlefield**.
  - deck162 `125v162` s12 verbatim: `- You cast Ob Nixilis, the Hate-Twisted` / `- Opponent cast
    Cancel` / `- Your Ob Nixilis, the Hate-Twisted resolved and went to your graveyard`; same file
    s27 the same shape for Howling Mine.
- **Observable harm already in the corpus**: deck162 s29 reasoning — *"'…went to your graveyard'
  immediately after casting. Usually, the game logs resolution. Maybe it was countered? Or maybe
  the prompt just truncated the resolution details."* The pilot is instructed to believe the
  narration; `"resolved"` is simply FALSE for a countered spell (trust doctrine, first kind).
- **Ask**: `- Your <card> was COUNTERED by <spell> and went to your graveyard` (and the
  opponent's-side twin). Distinguishing WHO countered it is decision-relevant for a combo deck
  (play-around-counters vs nothing-happened).
- **Validated next by**: seats 125 (caster of counters) and 126/162 (victims). Metric: coverage
  fraction — `"was countered"` emissions / counter events, and **zero** `resolved and went to`
  lines on a fizzled spell. **Until this ships, every seat review must treat `resolved` as
  ambiguous on any game against deck125** (carried into the wave-39 skill as a review rung).

## #4 — [HIGH, REPRESENTATION] The counter seam renders no fact you can decide on

- **id**: W39-STACKFACTS (`deck125/findings.md` §5.2). Offer-side twin of the wave-32 deck36 P1
  absent-field audit.
- **Mechanism**: `ON THE STACK` prints only `1 (top): opponent's Silver Myr [spell]`, and the
  counter option prints only `Cast <counter> - can target on the stack: <name> {card text: <the
  COUNTER's own text>}`. **The target spell's type, mana cost and P/T are absent at BOTH
  emitters** — 30 distinct stack lines this corpus, **0** carrying any of the three.
- **Consequence, measured**: a guide cannot write *"counter any creature spell with power 3+"*
  because the number is not on the screen; the pilot must recall the card from the decklist block
  at the top of a very long prompt, and does so badly — deck125 spent **9 of 25 counters** on mana
  producers / walls / 0-power bodies, the decisive one being **vs36 s24**: a 5-mana Fall of the
  Gavel on a **Silver Myr** with a **Master of Etherium (8/8)** already on the battlefield hitting
  for 8-9 a turn. Life 26 → 17 → 13 → 3 → −7 over the next four turns.
- **Ask**: annotate the stack entry and the `can target on the stack:` clause with the spell's
  **type, mana cost and (P/T)** — e.g.
  `opponent's Master of Etherium {2}{u} (creature 0/0*) [spell]`.
- **Payoff beyond this seat**: it converts deck125's named-card counter table (a pool-specific
  list that ages the moment the pool rotates) into a **numeric rule that generalises**. Also
  discharges the guide-side stopgap at `deck125_strategy.txt` L54-55 (see violation V4 in
  `synthesis-notes.md`).
- **Validated next by**: seat 125. Metric: coverage fraction of stack lines carrying all three
  fields; counters-spent-on-non-threats ratio (baseline **9/25**).

## #5 — [MEDIUM-HIGH, ANNOTATION WORDING] The wall-block annotation buries its load-bearing clause — 9 life in one game

- **id**: W39-WALLBLOCK / **E-162c** (`deck162/findings.md` §4 + §7).
- **Mechanism**: the blocker line reads
  `B1. Shield Sphere #1 (0/6) [defender] [deals 0 - this block kills nothing, it only absorbs
  damage] - may block A1 (neither dies)`. The two salient tokens are **`deals 0`** and
  **`kills nothing`**; the clause that actually decides the question — that the block STOPS the
  damage — is the trailing, unquantified *"it only absorbs damage"*. This is the wave-29
  annotation-wording rung (restriction/verb FIRST, no vague affirmative tail) violated in its own
  register.
- **Repro, two windows, one game, cost traced**: `146v162` s7 (t4, life 17) — verbatim
  *"If I block with one or both, the opponent takes 0 damage. **I take 3 damage (if one blocks) or
  6 damage (if both block).** … There is no benefit to blocking."* Took 3. `146v162` s10 (t6,
  life 14) — *"Blocking with a defender **doesn't prevent damage to me unless the blocker
  prevents damage**"*, quoting the core hint as licence. Took 6. **9 free life declined; the game
  was won at 4 life** — the declined life was the entire margin.
- **Ask**: `[deals 0 - this block kills nothing, but it STOPS all N damage from reaching you]`
  with **N filled in**.
- **Guide-side interim exists and is registered** (`deck162_strategy.txt` Rule 4 line; removal
  condition recorded in `deck162/findings.md` §10: delete when
  `/usr/bin/grep -c 'STOPS all' <corpus>/*.jsonl` is non-empty).
- **Validated next by**: seat 162 (and any wall seat). Metric: coverage fraction of `deals 0`
  annotations carrying `STOPS all N`; `no blockers` chosen with a `(neither dies)` line offered
  and a non-lethal header: **2 → 0**.

---

# Tier 2 — correctness and representation, mechanism located

## #6 — [MEDIUM] Blocker lines never say WHY an attacker is unreachable

- **id**: W39-UNREACHABLE (validator §D “two other fallbacks” + ledger 5).
- **Mechanism**: the blocker line states which attackers a blocker MAY block and never why the
  others are absent. The attacker line carries `[flying]`, but that is a positive tag on a
  different line; the "may block A2, A3" list is the only negative signal and the model read past
  it.
- **Repro**: `146v36` seq 22 (t13) — answered `BLOCKS: B1:A1` where B1 (Nadaar, no reach) could
  legally block only A2/A3; **A1 was Ornithopter with flying**. This is the corpus's ONE
  `all_assignments_illegal`; the guard caught it and the heuristic answered.
- **Ask**: put the reason on the attacker line —
  `A1 … [flying — your ground blockers cannot block it]` — or an explicit
  `(none of your blockers can block A1 — flying)`. **Perception lane, not guide prose** (the
  deciding fact is a capability the render never negates).
- **Validated next by**: seats 146, 152 (both run ground boards into fliers). Metric:
  `all_assignments_illegal` = 0; coverage fraction of evasive attackers carrying the reason.

## #7 — [MEDIUM, TRUST DOCTRINE] A mutated pile renders the WRONG card's rules text

- **id**: W39-MUTATEPILE (validator ledger 4).
- **Mechanism**: board and target option lines render a merged pile as
  `<top card name> (P/T) [combined keywords] - "<BOTTOM card's rules text>"`. The `[mutated pile]`
  annotation truthfully promises *"the combined abilities of every card in the pile"*, then the
  quoted text shows ONE card's — and not the named one. A true statement in the wrong scope.
- **Repros**: `139v162` seq 27 offers `Migratory Greathorn (3/4) [mutate] - "You may play an
  additional land on each of your turns…"` (that is **Dryad's** text; Greathorn is on top);
  `139v162` seq 36 offers `Brokkos, Apex of Forever (6/6) [trample, reach, mutate] - "Reach --
  When Arboreal Grazer enters…"` (**Grazer's** text).
- **Ask**: render the pile's text as the concatenation the annotation already promises, or
  attribute it per card (`"<name>: …"`).
- **Note**: the pile machinery is otherwise **VALIDATED** this wave (see #18) — this is the last
  open mutate render item, and it is at the seat that owns the crash in #1. Same lane if the
  crash lane ends up in `GameObserver`/mutate code anyway.
- **Validated next by**: seat 139. Metric: coverage fraction of `[mutated pile]` lines whose
  quoted text names or concatenates every card in the pile.

## #8 — [MEDIUM] The cast window is asked twice and the SECOND ask shows post-tap mana

- **id**: W39-DOUBLEASK (`deck125/findings.md` §5.4).
- **Mechanism**: the same `Cast <spell>` question is issued back-to-back; the first is answered,
  the second re-asks with the **post-payment** `Mana available:` figure while the following X menu
  correctly offers the pre-payment size.
- **Repros**: deck125 vs126 s16/s17 (s17 re-asks at `Mana available: 1` while the X menu offers up
  to X=3, i.e. six mana's worth); same shape vs139 s3/s4 and s9/s10, vs126 s13/s14.
- **Why it matters beyond the render**: **any guide rule keyed to `Mana available:` is unsafe at
  the second ask of a pair** — and rules keyed to that string are now shipped in three guides. Fix
  = do not re-ask, or print the pre-payment number on the re-ask.
- **Interacts with #2**: some of this shape is the same rebuild-the-menu-and-replay loop; check
  whether killing the churn in #2(b) also removes the duplicate ask before writing a second fix.
- **Validated next by**: seat 125. Metric: zero back-to-back identical cast asks; or, if the
  re-ask stays, the second ask's `Mana available:` equals the first's.

## #9 — [MEDIUM, MAGNITUDE TRUTH] The Overgrown Battlement activation label hard-codes "Add 1"

- **id**: W39-BATTLEMENT / **E2** (`deck126/findings.md` §4).
- **Mechanism**: all **22** activation option lines read
  `Add 1 green mana with Overgrown Battlement [cost: Tap]` regardless of defender count, while the
  card adds {G} **per defender** (`auto={T}:foreach(creature[defender]|myBattlefield) add{G}`).
  Two surfaces in the same prompt disagree: the `Mana available:` line correctly annotates it as
  `(VARIABLE output: this ONE source adds more than one mana …)`.
- **Repro inside one game**: `126v162` s20 takes the "Add 1" option; s21 prints
  `Already in pool: {g}{g}{g}{g} (4 mana ALREADY produced and floating right now)`.
- **Class**: HL7 annotation truth — an emitted magnitude ASSERTS; this one is false at any
  defender count above 1.
- **Guide interaction (must be read before fixing)**: `deck126_strategy.txt` **quotes this false
  label verbatim** as its #1 rule's window identifier (L11, L128) — see violation **V1** in
  `synthesis-notes.md`. Fixing the label **breaks that guide rule's anchor**; the guide edit and
  this fix should be sequenced (guide re-keys to the phase, then the label is fixed).
- **Validated next by**: seat 126. Metric: the label's number equals the defender count on every
  emission.

## #10 — [MEDIUM] `{right now: draws 0}` on cards whose whole function is drawing

- **id**: W39-DRAWS0 / **E-162b** (`deck162/findings.md` §7).
- **Mechanism**: `dynamicMagnitudes` evaluates a *future-step* draw at cast time and emits 0.
  Dictate of Kruphix renders `Cast Dictate of Kruphix {1}{u}{u} {right now: draws 0}`; Teferi's
  Puzzle Box renders `{right now: draws 0, draws 0}`. Both draw a nonzero number for both players
  every draw step.
- **Status**: **latent falsity, not yet a measured loss** — at `139v162` s13 the pilot ignored the
  annotation and reasoned from card text. Docketed at MEDIUM because three waves of magnitude work
  have taught the pilot to prefer the annotation to the card text, which makes a wrong magnitude
  strictly worse than an absent one.
- **Ask**: suppress the clause for triggered-on-a-future-step draws, or name the branch inline
  (`{each draw step: draws 1 each}`).
- **Guide interaction**: `deck162_strategy.txt` L113 currently teaches the pilot to **disbelieve**
  this annotation — a trust-doctrine violation (**V3** in `synthesis-notes.md`); that line is being
  cut regardless of this fix.
- **Validated next by**: seat 162. Metric: zero `draws 0` clauses on a card whose `auto=` contains
  a `draw:` on a future step.

## #11 — [LOW, CARD TEXT] Tribute to Hunger's `text=` drops Oracle's "of their choice"

- **id**: W39-TRIBUTE / **E3** (`deck126/findings.md` §4 + F2).
- **Mechanism**: primitives `text=` reads *"Target opponent sacrifices a creature. You gain life
  equal to that creature's toughness."*; Scryfall reads *"Target opponent sacrifices a creature
  **of their choice**."* The `auto=` is CORRECT (the opponent picks:
  `ability$!… notaTarget(creature|mybattlefield) sacrifice!$ targetedplayer`) — only the rendered
  text is ambiguous.
- **Cost measured**: the pilot resolved the ambiguity the wrong way in **three games**, at ~12 k
  reasoning chars each. **vs36 s10** (13,336 chars, the corpus's largest reply): *"Sacrificing
  Master of Etherium (toughness 8) is huge! … I gain 8 life. I go from 5 to 13"* — actual at s11:
  life **5 → 8** (+3), Master of Etherium **still on the battlefield** at s13. Also `146v...` s22
  and `162v...` s22 PLAN lines naming a specific victim.
- **Ask**: one-word `text=` fix. ⚠ `Res/sets/primitives/*.txt` holds non-UTF8 accent bytes —
  byte-splice, then verify `git diff | /usr/bin/grep -c $'\357\277\275' == 0`.
- **Note**: the guide's corrective line stays after this fix (registered stopgap with **removal
  condition: none** — the model belief survives the `text=` change).
- **Validated next by**: seat 126. Metric: zero PLAN lines naming a specific opponent creature as
  a Tribute target.

## #12 — [LOW, CARD TEXT] Perimeter Captain's `text=` says "you MAY gain 2 life"; the `auto=` is mandatory

- **id**: W39-CAPTAIN / **E4** (`deck126/findings.md` §4).
- **Mechanism**: `auto=@combat(blocking) source(creature[defender]|mybattlefield):life:2
  controller` — no `may`, and the corpus confirms the gain happens with no prompt (`139v...` s24
  logs seven `You gained N life` lines before `Phase: Combat damage`). Oracle DOES read "may", so
  the divergence is `auto=` vs Oracle; **the rendered text and the rendered behaviour disagree with
  each other**, which is the half that costs decisions.
- **Ask**: either add the may-prompt (Oracle-faithful; consult `/wagic-cardscript`) or drop "may"
  from `text=` (engine-faithful, one word). **Owner call on which** — this is a rules-fidelity
  choice, not a bug fix. The deployed guide states only what the engine does and does not instruct
  the pilot to disbelieve the line.
- **Validated next by**: seat 126.

## #13 — [LOW] Peer into the Abyss flattens both modal branches into one magnitude line

- **id**: W39-PEER / **E-162d** (`deck162/findings.md` §7).
- **Mechanism**: `Cast Peer into the Abyss {4}{b}{b}{b} {right now: life -4, life -8, draws 25}`
  at deck162 life 16 / opponent life 8. `life -4` is the target-opponent branch, `life -8` the
  target-controller branch, and the line does not say which is which.
- **Status**: no loss attributable (the pilot picked correctly, 1/1) — but a pilot reading
  `life -8` as its own cost could decline a lethal spell.
- **Ask**: label the branches, or emit only the branch the option will take.
- **Validated next by**: seat 162 (n=1 surface; re-check, do not probe for it).

---

# Tier 3 — watch items, harness, and observations (no fix scheduled)

## #14 — [LOW, WATCH n=2] Priority-ask protocol leakage

`unparsed_reply` ×2, both in the `146v152` seat log: seq 8 (t4, priority) replied in prose with no
`CHOICE:` line; **seq 36 (t16, priority) replied `ATTACK: Nadaar, Selfless Paladin, Goblin`** — the
attackers protocol answering a priority ask. Self-healing (the heuristic answered). **Watch;
revisit if it reaches n ≥ 4 in one corpus.** Validator ledger 6.

## #15 — [INFO] The blockers echo prints one blocker name per assignment with no disambiguator

`139v...` s24 narrates `Pride Guardian blocks Forest; Pride Guardian blocks Forest; Pride Guardian
blocks Forest; Perimeter Captain blocks Forest; Vampire blocks Forest` — **three DIFFERENT Pride
Guardians rendered identically**. The reply was legal (`B1:A2 … B5:A2`) and the engine executed it
correctly (seven separate gain triggers), so this is legibility, not correctness. The board and
blocker lists already carry `#1/#2/#3`; the echo does not. deck126 **E5**.

## #16 — [LOW, HARNESS] `results.tsv` credits a winner for a core-dumped game

The row `139 125 adj1 16 28 14` records deck139 as the winner of the crashed game **while deck125
was ahead 28-16**. A crashed game must record **no winner** (a distinguishable `crash` verdict).
Cheap, and it protects every future wave's record table from a false row. deck125 §5.5;
validator §preamble. Blocks nothing, but the wave-40 record table must not read this row.

## #17 — [OBSERVATION, core/reply-protocol lane — NOT a guide item] PLAN-protocol churn

At `126v152` s10 — a **three-option LAND DROP that took 11,603 chars and 236 s** — roughly half the
reasoning is the model litigating rule (a)/(b)/(c) of the PLAN-line test against itself and
reversing four times (*"I'll write one to be safe" / "I'll stick to (c) and omit it" / "Actually,
writing it doesn't hurt"*); the same oscillation closes `126v36` s10. Single-seat evidence,
recorded, **not proposed as a core edit** — the core prompt's PASS streak is not being contested on
one seat's two records. Re-check next corpus; promote if a second seat reproduces.

## #18 — [CLOSED THIS WAVE, no action] two wave-38 lanes VALIDATED

Recorded so no wave-40 lane re-opens them.
- **Mutate role headers + host-intent carry** (`5ea04c6ba`): `MUTATE PLACEMENT` ×10,
  `MUTATE - CHOOSE THE HOST` ×8, carry line ×7 of 8 (the 8th is the designed ambiguity negative);
  **10/10 mutate casts landed on the intended host, over AND under honoured**; 0 generic-wording
  regressions. The wave-37 docket item "mutate host-intent carry" is **CLOSED**.
- **ALord/AAMover SEGV** (`5e6e4ccf9` / `7f33d0dc7`): crash class absent, and the exact
  precondition was **exercised twice and survived** — `152v139` seq 20 resolved Elite Spellbinder
  against an **empty opponent hand** (the `all(opponenthand)` reveal clause that segfaults on
  `5104a2433`), `152v126` seq 21-22 rode the non-empty clause. Not a lucky miss.

---

# ROSTER-OWNER FLAGS — not fix lanes, no agent acts on these

These route to the roster owner. **Do NOT "fix" either deck**; do not write a guide rule that
compensates beyond what the shipped guides already do.

- **deck126 CONSTRUCTION — a 75-card deck with 39 lands. ⚠ ALREADY EDITED IN THE WORKING TREE —
  read this before the wave-40 corpus.** As reviewed, `deck126.txt` commented `#24 lands` but
  listed 4+4+4+4+2+2+1+1+1+**16** = 39, and the seat flagged **`Forest (SHM) * 16` as almost
  certainly meant to be `* 1`** — the owner's call, and only the owner's. **At synthesis time
  (2026-08-23) the working tree already carries that edit, uncommitted:
  `git diff projects/mtg/bin/Res/ai/baka/deck126.txt` = `-Forest (SHM) * 16` / `+Forest (SHM) * 1`,
  taking the deck to 60 cards / 24 lands.** Nobody on this wave made that edit. **CONSEQUENCE THE
  WAVE-40 REVIEW MUST CARRY: the wave-40 deck126 corpus will not be the same deck the wave-39 guide
  was authored against** — the guide's mulligan floor, its deploy ranking and the whole
  combo-dilution reading were measured on the 75-card list, so a behaviour delta at that seat is
  NOT a guide property until the construction change is attributed. Confirm the edit is intended
  and committed before the corpus runs. Corroborated in-game (on the 75-card list): the Idyllic Tutor reveal
  menus enumerate the whole library at **61 options on turn 9** (`126v152` s17) and 58 on turn 19
  (`126v139` s31); a 60-card list would be near 48 and 38. Visible consequences: 52 % lands, long
  `Play Forest` / `Cast nothing right now` stretches (`126v125` turns 13-29), and eight combo
  pieces diluted across 75 cards — which is the entire content of the three losses' zero column
  (Exquisite Blood **never drawn** in all three).
- **deck125 is CONSTRUCTION-CEILINGED (note, not a request).** Its nominal finisher costs `{15}`
  in a deck whose **observed mana ceiling across six games is 12** (26 lands, no ramp, no rock, no
  cost reduction). Emrakul: **0 option lines in 34 prompts holding it**. The deck's only deployable
  win condition is Staff of Nin ×2 at `{6}`, 1 damage per turn; three of five completed losses
  ended with the opponent at 20, 20 and 19 life. The shipped guide plays the deck it actually is
  (answer everything, grind on a Staff clock) and **is validated independent of the win column**
  per the ceilinged-seat rule. If the owner ever wants this deck to close, the cheapest edits are
  replacing Emrakul with something castable off 26 lands, or a second pair of Staff-class win
  conditions. **No fix agent touches deck125.txt.**
- **deck162 `#HINT` lines are not enforced for the GPT seat** (N-162e, INFO). `deck162.txt` carries
  `#HINT:dontattackwith(Fate Unraveler)` / `#HINT:dontblockwith(Fate Unraveler)`; AIHints is a Baka
  surface, so the engine offered Fate Unraveler as an attacker in 4 windows and a blocker in 1.
  **Expected, not a defect** — recorded because the deck author's instruction has to be carried by
  the GUIDE (it now is, as an ordering rule with a lethal exception; a blanket "never attack" would
  have cost the vs146 win).

---

# Recommended lane batching for wave-40 step 1

Disjoint files, merge order engine → parser/seam → render (per the campaign's worktree rule; the
merged-tree gate on master is the only gate that counts).

| lane | items | primary files | why grouped |
|---|---|---|---|
| **A — crash** | **#1** (+ #7 if the fix lands in mutate/`GameObserver` code) | `src/GameObserver.cpp`, a new fixture | P0, single-owner, blocks the 139v125 rerun; nothing else may touch `GameObserver.cpp` this wave |
| **B — decision seam** | **#2a** (`AIPlayerBaka.cpp:3422` gate), **#2b** (`AIPlayerGPT.cpp` ≈L8060-8150 churn), **#8** | `src/AIPlayerBaka.cpp`, `src/AIPlayerGPT.cpp` (cast-planning region) | one causal chain; #8 may be a free consequence of #2b — check before writing a second fix |
| **C — narration truth** | **#3** (countered-vs-resolved), **#16** | `src/AIPlayerGPT.cpp` (narration region ≈:2036), `src/ActionStack.cpp`, harness `results.tsv` writer | the wave's biggest render lie + the record-integrity one-liner; disjoint region from lane B |
| **D — option/annotation render** | **#4** (stack facts), **#5** (wall-block wording), **#6** (unreachability reason), **#10** (`draws 0`), **#13** (Peer branches) | `src/AIPlayerGPT.cpp` (option/board/blocker emitters + `dynamicMagnitudes`) | all option-line emitters, all coverage-fraction-validated, all cheap; one agent avoids merge churn in one region |
| **E — card data (optional 4th/5th)** | **#9**, **#11**, **#12** | `Res/sets/primitives/*.txt` (byte-splice), option-label emitter for #9 | no C++ overlap; **#9 must be sequenced AFTER the deck126 guide re-key** (V1) |

Lanes B, C and D all touch `AIPlayerGPT.cpp` in different regions — that merges fine (proven), but
apply in the order **B → C → D** and expect `patch -p1 --fuzz=3` on the third. PARSETEST count on
the merged tree must equal the exact SUM of the lanes' added cases.
