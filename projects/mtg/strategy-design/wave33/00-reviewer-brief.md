# Wave-33 reviewer brief (shared context for all 7 deck agents + synthesis)

Nineteenth cycle. Pool UNCHANGED from wave-32: holdovers **146, 139, 152, 158** +
second-corpus seats **105 (Infected Wounds), 36 (Master of Ether), 116 (Time of Need)**
(all three now have wave-32-authored guides). No rotation this wave — **this corpus IS
the poison-pair validation re-run** (see the asterisk in Corpus data). Rotation
projection: if trajectories hold, wave-34 is a 3-seat exit (146 slipped w33→w34, 152 at
w34; 139 exit-eligible conditional on N-139n validating here).

PROCESS RULE: NEVER kill any wagic process (pkill FORBIDDEN); kill only your own PIDs.

Binary: **bdb30d8e7** — the wave-33 step-1 batch shipped at `948578887` PLUS the whole
Aug alpha-release arc. Gate at this binary: main 1045/0 + 24 AI/0 single-threaded,
PARSETEST 328/0.

## ⚠ INTERIM CHANGES (948578887..bdb30d8e7) — attribute deltas correctly

The alpha arc changed card SCRIPTS and effect IMPLEMENTATIONS between the wave-32
corpus binary and this one. A behavior delta vs wave-32 is NOT automatically the step-1
batch; check this list (and `git log 948578887..bdb30d8e7 -- <file>`) before crediting
or blaming a wave-33 fix. Decision-relevant interim commits:

- **Card scripts**: `115a474f3` (18 cards fixed by oracle-lint), `mtg.txt` ~59 lines
  changed overall; `2c493e6d6`/`d3f44bf0a` coin-flip branch parsing (Wirefly Hive);
  `43453c5a6` Spirit Flare strike duration; `7f60218ca` Glimmer of Genius {E}{E}.
- **Offer side (what the model is SHOWN as legal)**: `cd2983c2a` activations lacking
  distinct payment candidates not offered; `80d2ac217` one creature can't pay two
  targeted sac components; `128a3600c` unpayable kick lines not offered; `42bc7631f`
  cast verdicts spread across duplicate copies; `af7806b43` double-paying combo hint;
  `216c3ab77` ring half-price fix.
- **Perception/render**: `519c7e48d` scry-only cards no longer reveal the new top card;
  `a6600e507` board activatable-ability rendering ("what the board can do");
  `b2c2a2e9d` defender gets the attacker's signal.
- **Combat/rules**: `d083a0629`+`81b0ae431` LLM damage-assignment order seam (was dead
  code) + Baka greedy ordering fallback; `bdd07223c` defender's blocker window opens on
  the attacker's turn; `840963584` CR 514.2 heal backstop; `0280a08b4` counter events
  from every engine path + evolve as a real stacked trigger; `e4b3cc96e` ORDER-step
  click gate; `a46095a08`/`4dab86746` cancelled-attacker guards.
- **Reply protocol**: `f46dd58ee` — **the model may OMIT an unchanged PLAN line**
  instead of rewriting it. This shifts the commit-counter semantics below: a reply with
  no `PLAN:` line is now protocol-compliant; `post_plan_overrun` applies only to
  replies that DO carry one.
- **Transport/GUI (should be corpus-invisible, listed for completeness)**: OpenAI
  subscription preset, OpenRouter pinning/hidden-reasoning, Vita worker seam, model
  picker, patience-prompt auto-dismiss, mingw port.

## The step-1 batch under live validation this corpus

All step-1 agents' predictions below are FALSIFIABLE and recoverable from the diffs at
`948578887`. Report annotation-fix coverage as **FRACTIONS, never samples** (HL5).

**The poison pair (the wave's mandate):**
(i1) **N-105a — poison is RENDERED.** Poison status line with an explicit 10-threshold;
infect/wither/toxic damage narrated DISTINCTLY from ordinary damage; the missing
`WEventplayerPoisoned` handler added; `legibleKeywordName()`. PREDICTIONS: poison total
visible in **100%** of infect-game prompts at BOTH seats; non-combat poison (Ichor Rats
/ `alterpoison`) narrates — the wave-32 four-silent-events class is dead.
(i2) **N-105b — the blocker forecast partitions attackers by damage type** and prices
poison against 10. PREDICTION: **zero** "you would be at N — NOT lethal" emissions on
infect windows; a swing crossing 10 poison must read as lethal. The wave-32 game-losing
repro (deck36 s25 t12, no-blocks at 20 life) is the reference shape. Same-seam
non-infect windows are the negative control — they must be unchanged.

**Render/perception lane:**
(i3) **N-146j `[-multicolor]`** — compiled colour INDICES to TYPE bits; artifact and
land exclusions BOTH dropped; `CardDescriptor` colourless test fixed via `countColors`;
`-colorless` added to 3 primitives for Oracle fidelity ("monocolored" ≠ colourless).
PREDICTION + FALSIFIER: Verse target lines list mono artifact creatures **and ZERO
colourless targets**. deck146/deck36 natural witnesses.
(i4) **N-152f `deferFlexibleSource` per-ability** (was per-card mutual-deferral
deadlock on all-flexible manabases). PREDICTION: all-flexible-manabase multi-pip offers
appear — **ARRIVAL-TRACE them** (never fallback-count; the fixture now rides the broken
branch). deck152 focus.
(i5) **N-36b — SIX falsy-zero print guards**, all cost emitters routed through one
`manaCostToken()`. PREDICTION: **zero `{0}`-confabulations** — deck36 ({0}-cost
artifact seat) is the natural witness.
(i6) **N-158m — the magnitude scanner runs `AutoLineMacro::Process` per line AFTER the
raw-line trigger filter.** PREDICTION: amass previews on ALL non-trigger-gated offers
(wave-32 fraction was 3/52 = 5.8%; report the new fraction). deck158 focus.
(i7) **N-152d layer 2 — `AAFlip::testDestroy` resets `isFlipped` post-transform**, new
`cardShowsOtherFace()` discriminator. PREDICTION: **zero "(printed 2/2)" on Moonrage
Brute** (the board-snapshot path, 22 wave-32 occurrences); no tag when delta is 0.
(i8) **N-152j/N-158n — name-form `BLOCKS:` lines PARSE** (3 legs; the reconciler split
on the label's own colon — counterfactual-proven). PREDICTION: `unparsed_reply` fires
only on truly unparseable replies; the wave-32 name-form shape (deck139 s21, deck158
s35) parses. NEW FALLBACK CLASS `all_assignments_illegal` replaces the silent
`unparsed_reply` exit on an all-illegal assignment set (this closes N-139s) — report
every occurrence with its trigger shape.
(i9) **N-146k/N-139n — pregame prompts are HAND-ONLY (OWNER DIRECTIVE).**
`serializePregameState()` renders hand + an engine-computed hand header
(`Your hand (7 cards): lands 3 (green sources: 1) | creatures 4` form) across mulligan,
bottoming and leyline paths; PARSETEST pins the absence of battlefield facts.
PREDICTIONS: **zero zero-lands/zero-green false mulligans** (deck139's six-of-nine
class — the count header is the escalation past the spent exemplar lever, §3 of
wave-32 synthesis); zero `Mana available: 0 total` echoes in mulligan replies
(deck146's N-146k class). ⚠ Known consequence awaiting an owner call: a leyline
resolving in pregame is invisible to subsequent pregame asks — COUNT occurrences if
any deck exercises it.

## The commit-failure COUNTER (instrument only — no behaviour change shipped)

New translog fields, quoted verbatim: **`post_plan_overrun`** (int — chars past the
first `PLAN:` line), **`commit_retracted`** (bool), **`pruned_pairs`** (string).
EVERY seat reports, as counted fields: replies whose text continues past their own
`PLAN:` line; of those, how many retracted; how many were fatal. **PROMOTION TRIGGER:
≥2 seats with a fatal overrun+retraction → the commit-terminator becomes a core
proposal with its measurement already in hand** (wave-32 §5, deck158's flag).

## KNOWN-OPEN (cite, don't re-diagnose)

- **N-152g (DIAGNOSED, fix deferred to wave-34)**: `LegalActions.cpp:66` and `:256`
  suppress casting a legend while a same-name copy is in play. RULES-WRONG (CR 704.5j
  is an SBA; `MTGNewLegend` implements it correctly). Fix = delete both lines. Count
  suppressed windows if witnessed.
- **N-139m** re-scoped to the HARNESS lane (script-pump menu-flow divergence; live path
  clean; 5 reproducer fixtures preserved unregistered in `bin/Res/test/ai/`).
- **N-158e flash windows**: Baka's computeActions builds no casts outside main phases
  on its own turn. Hold-for-flash teaches CANNOT fire; don't count against guides.
- **N-146h layer 2** (Agadeem): needs a PROBE, not a corpus — cite only if exercised.
- **N-116a whole-library tutor reveals**: routed, not yet fixed — the reveal tax is
  EXPECTED at deck116; count it, don't re-report as new. N-116d subset framing ditto.
- **N-158k**: Feed-the-Swarm costs present on the cast line, still ABSENT on the target
  menu (the COMMIT seat). Cite occurrences.
- **Probe debts**: i5-reask remains probe-validated only (no validation rejects have
  ever occurred in a corpus); i10 attachments UNEXERCISED (probe spec N-36h).

## Corpus data

- **PRIMARY**: (filled by the orchestrator after the run — matchups-<ts>, binary
  bdb30d8e7, -T 3000, REPPENALTY=1.05, -j 3.)
- ⚠ **deck105's wave-32 6/6 is ASTERISKED/CONFOUNDED — its opponents were
  poison-blind** (N-105b told all six of them at the decisive seam that the swings were
  survivable). **This corpus IS the poison-pair validation re-run**: with poison
  rendered and the forecast honest, deck105's record here is the first clean
  measurement. If it still goes 5-6/6, that is a construction/roster question; if it
  drops, the perfecta was substantially a render artifact. Do not cite the wave-32 win
  table as a baseline for deck105.
- Wave-32 priors for the other seats: 36:4/6, 158:3/6, 152:3/6, 146:2/6, 139:2/6,
  116:1/6; fallbacks 0.27% with zero defers/truncations — bookkeeping only, never a
  verdict on a guide.

## Per-seat focus

- **deck105 (THE RE-RUN SEAT)**: does the record hold with honest opponents? Validate
  arithmetic-on-the-RENDERED-count (the wave-32 diagnostic: correct strategy, wrong
  count every time it mattered — *"my count is wrong"*); i1/i2 coverage fractions at
  both seats of its games; symmetric-card pricing steady state (Ichor Rats).
- **deck139 (exit-eligible, conditional on N-139n)**: the count header vs the mutated
  membership belief — the six-false-mulligans class must DIE; the recite-the-list-
  then-exclude-a-member diagnostic is the signature to grep; latency tail (the 213s
  bottom-ask was the same belief); mutate steady state.
- **deck146 (HOLD, exit projected wave-34)**: HL1 retroactive card-fact audit is a HARD
  GATE on the exit; i9 kills the N-146k echo class (its 19-mulligan wave — floor
  satisfiability now provable against the header); i3 Verse take-rate with colourless
  falsely-eligible targets gone; Acererak class stays dead (watch, not under-spec).
- **deck152**: i4 arrival-traces on the all-flexible windows; i7 zero false printed
  tags on the snapshot path; menu-label-scoped prohibitions (the Flip-Side/Huntmaster
  surface — guide-side HL5 scoping) steady state; deploy floor + safety axis.
- **deck158 (commit-counter natural seat)**: i6 amass fraction; the counter fields on
  its own s16 shape; N-158k cited where it bites; Bowmasters under N-158e known-open.
- **deck36**: i5 zero-confab witness; i2's reference repro was ITS loss — blocker
  windows against 105 are the direct re-test; phase-header contradiction WATCH (one
  wave-32 seat; a second witness promotes the representation change).
- **deck116**: reveal tax counted under N-116a (routed); cheat-into-play steady state;
  1-of Eldrazi absent-engine-card shape; its wave-32 chump-blocks-vs-invisible-poison
  games re-read cleanly now — confirm.
- **EVERYONE**: crash gate (0 signatures); commit-counter fields; coverage FRACTIONS;
  `all_assignments_illegal` occurrences with trigger shapes; parser contradictions
  verbatim.

## Layer-routing (unchanged)

Engine/harness → notes.md with seq repros. Guide → strategy.txt per
**wave32/strategy-writing-skill.md** (CURRENT edition — HL1 hard gate; HL5 input-side +
fixture-rides-broken-branch; HL6 count escalation; HL7 mulligan rung; HL10
instruments). Skill → skill.md. Core → general-suggestions.md (27 consecutive PASSes;
deck105 G1 alt-win clause is HELD pending THIS corpus — re-check it with poison
rendered, adopt only if the conflation PERSISTS). Rotation: UNSENTIMENTAL.

## Open ledger carried (cite if exercised)

N-152g (top, fix staged w34); N-139m (harness); N-158e; N-146h layer 2; N-158k;
N-116a/N-116d; N-36f (mana-buys representation, recorded); N-139q Nethroi card-script;
N-152a MDFC back-face land placement; N-152h MDFC-spell wording (third path); Rankle
mode-bleed; L-18-1 planning tax (length-only watch); N-134b/d; L2 cross-zone dup-name;
ETB header card-name; N8; Azcanta; undying; commit-lock; prose-reversal;
untapped-tagging; I-4.

## ⚠ OWNER DIRECTIVES (2026-08-19, mid-corpus — route to synthesis as core-protocol items)

1. **OWNER RULING: eliminate the post-choice reasoning block from the reply protocol
   entirely.** His diagnosis, confirmed against the protocol text: the current
   protocol instructs answer-first, THEN invites scratch reasoning ("you may think
   the decision through briefly... that scratch text is discarded"), THEN puts PLAN
   on the LAST line — a composition that teaches reasoning-follows-output and
   ambiguously folds reasoning into the PLAN step. The measured post_plan_overrun
   spirals (mean ~1,900 chars/reply post-answer corpus-wide; two ~9k specimens) are
   the model REPEATING THE INSTRUCTED PATTERN, not disobeying. The fix is NOT
   reordering the block and NOT a terminator: **if reasoning is wanted, request it
   natively** (enable_thinking / reasoning-effort in the request — the plumbing
   already exists), where it precedes and informs the answer and never pollutes the
   reply. Target protocol: ANSWER + (optional, changed-only, concise) PLAN. Nothing
   else invited. Dev-vs-user split falls out naturally: dev loop = native reasoning
   ON + captured to translog (reasoning_content) for seat review; shipped user
   default = reasoning OFF, terse replies (owner: verbose replies are "useful for
   our development loop, but unconscionable for an actual user" — ~30-55k wasted
   output tokens/game on the user's own key, ~200s stalls).
   OWNER'S CONCLUDED POSITION (2026-08-19, post-review, near-verbatim): "our dev loop
   needs reasoning, because it makes the model's experience more auditable. so we'll
   use reasoning. but structurally, reasoning should be placed in the model's NATIVE
   reasoning window, where it benefits the model's ability to make good choices, and
   the model has been taught to reason, and should still give us something to audit."
   => dev loop: enable_thinking ON, reasoning captured to translog; reply protocol:
   answer + concise changed-only PLAN, nothing else; user default: thinking OFF.
   SEATS: do not counter-propose reorderings or brevity clauses; your evidence
   contribution is (a) counted overrun/retraction fields, (b) whether the 4.2%
   post-answer self-correction rate (52/1,249 last-label-differs re-commits) carries
   decision VALUE that native pre-answer reasoning would need to replace, (c) any
   reply where the discarded scratch text was load-bearing for review. Wave-34
   step 1 implements; an A/B corpus validates decision quality + latency.
3. Related docket (this session): consumed-decision narration must strip decoration —
   `{card text: ...}`, `[cost: ...]`, dynamic magnitudes — from the NARRATED copy
   (describeAction consumer at AIPlayerGPT.cpp:4787 and the menu echo at :4923);
   measured small (~320 chars in a 12k prompt) but wrong-by-architecture. Wave-34
   step 1 candidate. Cap adjudications this corpus are latency-spiral driven (see
   the two ~200s/13k-char specimens, spiral-examples.md in scratch; non-retracting,
   correctness intact) — synthesis weighs cap 3000→3600 and/or -j 2 vs the protocol
   fixes above.
