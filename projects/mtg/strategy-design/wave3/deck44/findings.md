# Engine / integration / harness findings from the deck-44 review

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260709-220729/`, all six deck44 games read
decision-by-decision. Citations are `<file> line <n>` (0-based line = record index) with the
record's own `seq`. Severity: **P0** = observed game-losing bug; **P1** = likely large
play-quality/eval cost, mechanism unconfirmed; **P2** = improvement/annoyance.

---

## F1 (P0) — Reply says option 4, engine consumed option 3: AI destroyed its OWN creature with Go for the Throat

**The record** — `1783652855-ai_baka_deck44-0x555f85bf3f70.jsonl` line 18:

- `seq=18, kind=ask, model=qwen35, options=6, choice=3`
- The prompt is the target chooser: "Choose the target for Go for the Throat", options
  1-3 = deck44's OWN Surveilling Sprite / Sleep-Cursed Faerie / Obyra ("[your battlefield]"),
  options 4-6 = opponent's Stromkirk Noble / Gore-House Chainwalker / Ash Zealot.
- `reply` (verbatim start): `'4. Stromkirk Noble (4/4) [opponent\'s battlefield] [tapped] - "Stromkirk
  Noble can\'t be blocked by Humans. -- ..."\n\n**PLAN**: Cast **Go for the Throat** targeting
  **Stromkirk Noble** ...'`
- Logged/consumed `choice` = **3** = its own Obyra, Dreaming Duelist.
- Ground truth from the next record (line 19, same turn's attackers): Obyra is GONE from
  "Your battlefield", Go for the Throat gone from hand, Stromkirk Noble alive at 4/4. The
  spell really did kill the AI's own 2/2 while at 4 life vs red aggro — maximally game-losing.

**Two things to check when debugging:**
1. **Plan-split robustness.** The model wrote `**PLAN**:` (markdown bold), not the protocol's
   `PLAN:`. If the plan splitter doesn't match the asterisked form, the whole plan prose stays
   in the choice-parse buffer — and the plan text is full of numbers. The choice parser then has
   many integers to mis-pick from. Whatever the root cause, the parser should tolerate
   `**PLAN**:`/`PLAN :`/case variants, and the choice scan should stop at the first
   line-leading integer.
2. **Interaction with the preceding fallback.** The immediately prior record (line 17,
   `seq=17`) is a `choice=-1` empty-reply failure on the "cast Go for the Throat / cast
   nothing" ask — the cast then happened anyway, so the fallback (Baka?) initiated it. It is
   possible the consumed TARGET also came from the fallback path (Baka's target scoring
   picking Obyra) while the late/parsed GPT reply got logged against it. Either way the log
   pairs a reply and a consumed choice that disagree; the fix must make the translog record
   the actual decision source (see F5).

**Cheap safety net regardless of root cause:** at the target seam, if the consumed target is
the AI's OWN permanent for a plainly negative effect (destroy/damage/sacrifice) and any
opponent target was legal, re-ask or fall back. A strong pilot essentially never Throats its
own creature; a structural guard here is cheap insurance against every parse bug class.

## F2 (P1) — The blockers seam is nearly silent: 1 blockers ask in six games while lethal swings went unblocked

Decision-kind counts across all six deck44 translogs: `ask=101, priority=54, attackers=21,`
**`blockers=1`** (corpus-wide the coordinator counts 3 total).

Concrete cost: in the vs109 loss (`1783652855-...deck44...jsonl`), deck44 took 7-damage
swings on opponent turns 6 and 8 (life 19→12→4, see seq10/seq16 upkeep records) with an
UNTAPPED Surveilling Sprite (1/1 flyer) on the battlefield both times. Blocking was legal
(Noble's "can't be blocked by Humans" doesn't exclude a Faerie; only Chainwalker
self-restricts) and at 12→4 life a chump was mandatory — yet NO blockers record exists for
either combat. The one blockers ask that did fire (vs135, seq12) worked normally.

Open question — engine seam or logging? Hypotheses to check in `AIPlayerGPT::chooseBlockers`:
(a) a precondition (e.g. a Baka-side "is any block worth considering" gate, or the
`mBlocksDoneTurn` re-entry guard) skipping the GPT ask entirely; (b) the ask-cache keyed on
board+question returning a cached "no blocks" without emitting a translog record; (c) fallback
answering without logging. Suggest env-gated instrumentation at the seam entry vs. translog
emission to see which decisions never reach the model. Given blockers bundling shipped
recently, this looks like the least-validated seam — and "never blocks" both loses games and
poisons the corpus for the future policy model.

## F3 (P1) — Empty-reply failures (`choice: -1`): silent fallbacks at critical moments

Two observed: `1783652855-...deck44` line 17 (`seq=17, kind=ask, options=2, reply=''`) — at
4 life, the decision whether to cast Go for the Throat; and `1783652868-...deck44` line 56
(`seq=56, kind=priority, options=1, reply=''`). Rate ≈ 2/177 (~1%) for this deck.

Why it matters: the fallback is invisible in play and in eval — F1 shows a fallback-adjacent
decision chain producing a catastrophic result. Suggestions: (i) log the failure REASON
(timeout / HTTP error / unparseable) in the record; (ii) log the fallback's chosen action as a
separate field so the corpus stays causally readable; (iii) alert/count in the harness summary
so a bad endpoint day is distinguishable from bad play.

## F4 (P2) — Representation: hybrid mana, "offered = payable", and two rendering nits

- **Hybrid mana:** vs133 (`1783655267-...deck44`, lines 6-12) the model refused an OFFERED
  "Cast Oona's Gatewarden {u/b}" for four straight turns, PLAN saying "I cannot cast my
  creatures ... without a Swamp". Recommendation: fix at BOTH layers. General prompt gets the
  rules fact + "every listed option is legal and affordable" (already proposed in
  general-suggestions.md S3); representation could additionally render hybrid symbols
  self-descriptively the first time they appear in an option line — e.g. `{U/B}` →
  `{U/B (either color)}` — since a per-line hint survives even when the prior is forgotten.
  The option line is the highest-attention surface we have; use it for exactly this kind of
  deciding fact (consistent with the P1/P2/P4 design).
- **Magnitude annotation ambiguity:** "Cast Faerie Bladecrafter {2}{b} (2/2) {right now:
  life -2}" (vs110 seq7, vs133 seq14, vs135 seq17). That `-2` is the evaluated DIES-trigger
  drain, but reads like a cost of casting. qwen cast it anyway, so no observed damage — but
  labeling the trigger would remove the trap: `{if it dies now: opponent -2}` or prefix with
  the trigger keyword the evaluator came from.
- **Empty counter bracket:** vs140 seq39/seq44 render Sleep-Cursed Faerie as `[counters:]`
  once its stun counters reach zero — should drop the bracket entirely (cosmetic, but it
  wastes prompt tokens and looks like a glitch to the model).

## F5 (P2) — Translog improvements that would have materially sped this review

- **`options` is only a count.** Store the option TEXTS (or at least the consumed option's
  text) as a field. Every reviewer re-derives "what did it actually pick" by regexing the
  choice index back into the prompt — and F1 shows the log can't currently prove
  reply-vs-consumed disagreement without that reconstruction.
- **Structured turn/phase/life fields** per record (they're parseable from the prompt, but
  every analysis script re-writes the same regexes).
- **A game-id shared by both players' logs** (currently paired by epoch±few-seconds) and a
  final record with result + final life totals + turns played.
- **Decision-source field**: gpt / gpt-stale-retry / fallback-baka / cache-hit (fixes F1's
  ambiguity and F2's diagnosis, and keeps the future policy-model corpus honest).

## F6 (P2) — Timeout/pacing economics of the eval loop

13/21 corpus games hit the 40-minute cap. From deck44's slice: these are NOT infinite loops —
they're slow inference × games that don't close. vs110 produced only 14 deck44 decisions in
40 minutes (opponent deck110 similarly few) ≈ 1-1.5 min/decision round-trip; a two-GPT game
has a budget of roughly 30-60 total decisions, i.e. ~turn 14-16. Any game whose pilots don't
close by then "times out" regardless of quality — vs140 timed out with deck44 ahead 20-9.

Suggestions: (i) record final life totals + turn count in `results.tsv` so a timeout can be
scored (life-lead = win-ish signal instead of dead data — 3 of deck44's 6 games are currently
invisible to win-rate); (ii) consider a TURN cap (e.g. 25 turns) instead of/alongside wall
time, so the cap is fair under varying inference load; (iii) the strategy fixes (hoarding →
deploying) should themselves shorten games — re-measure timeout rate after wave3 guides land
before buying more compute.

## F7 (d-layer) — Model-weakness notes (qwen35) for prompt/model experiments

- **Second-guesses engine legality** (the hybrid refusal, F4) — doesn't trust the option list
  as the legal set. Prompt-fixable; also a good probe question for candidate models.
- **Unbounded "hold" bias**: with any hold-shaped justification available (flash, counters,
  "keep mana open"), it holds forever. The strategy layers now push against this, but model
  choice matters: a model that can compare "value of holding vs value of deploying" would
  need less scaffolding.
- **Protocol drift under pressure**: `**PLAN**:` instead of `PLAN:` (F1), occasional empty
  replies (F3), long self-correcting arithmetic monologues in PLAN ("2+2+4+5 = 13 ... Wait,
  let's recount") — harmless when the parser is robust, fatal when it isn't. Parser
  hardening > model scolding.
- **What qwen35 does WELL** (don't spend experiment budget re-fixing): explicit lethal
  arithmetic when the general prompt asks for it (vs131 seq17: "13 > 4. I win the game." —
  then took exactly that line and won); consistent attack declarations; no cost-refusal
  dithering on life-payment casts.

## F8 (P2) — Strategy-pipeline observation: the deployed guide's own text is part of the system under test

Two of deck44's observed failure modes were CAUSED by the previous guide's wording ("save
your removal, it's PREMIUM" → hoarded to death; "a hand of counters with no creatures is
LOSING" → over-mulligans at the mulligan seam). The evaluation loop should treat guide text
like code: when a misplay recurs, first ask which deployed sentence (guide OR general prompt)
the model is obeying. The translog makes this answerable — the model frequently quotes its
reasoning in PLAN lines ("I will hold Go for the Throat for a critical threat" is the guide's
own language echoed back, vs109 seq17 prompt's carried plan). Grep PLAN lines for guide
phrases when diagnosing: it attributes the fault in minutes.
