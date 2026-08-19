# Wave-33 synthesis notes

Nineteenth per-deck-rotation cycle. Pool = 7, UNCHANGED from wave-32: holdovers **146** (4/6),
**139** (0/6), **152** (3/6), **158** (6/6) + the three second-corpus seats **105** (2/6),
**36** (3/6), **116** (3/6), all three now carrying wave-32-authored guides on their first
validation. Corpus `matchups-20260819-070739`, binary `bdb30d8e7`, -T 3000, REPPENALTY=1.05, -j 3.

Deliverables: this file, `engine-ledger.md` (the ranked wave-34 step-1 source of truth),
`strategy-writing-skill.md` (wave-33 edition), `rotation-reconciliation.md`. Deployment is the
orchestrator's step, not this agent's.

---

## 0. Corpus health, stated once

| metric | wave-31 | wave-32 | **wave-33** |
|---|---|---|---|
| games | 21 | 21 | **21** |
| timeouts / draws | 0 / 0 | 0 / 0 | **0 / 0** |
| cap adjudications | 3 | 1 (campaign low) | **4** |
| crash signatures | 0 / 21 | 0 / 21 | **0 / 21, fourth consecutive clean corpus** |
| stub decisions | -- | -- | **0 (Spark healthy throughout)** |
| fallbacks | 6 / 1,432 (0.42%) | 3 / 1,117 (0.27%) | **1 / 1,277 (0.08%, CAMPAIGN RECORD)** |
| defers / truncations / validation rejects | 2 / 3 / n-a | 0 / 0 / 0 | **0 / 0 / 0** |
| `unparsed_reply` | 2 | 2 | **0** |
| `all_assignments_illegal` | n/a | n/a | **1 (deck152 vs36 s8) -- the new class working as designed** |

**The single fallback is not a defect.** deck36 s8, `kind=blockers`: `BLOCKS: B1:A2` against a
flying attacker B1's own line does not list, `pruned_pairs: "Intrepid Adversary -> Glaze Fiend"`,
`choice_source: None`. Under wave-32's code this exits as a SILENT `unparsed_reply` (N-139s). It is
now named, its pruned pair recorded, and the diagnosis took one line instead of a reply read.
**i8's new class is validated at one live witness and N-139s is CLOSED.**

Pilot `repetition_penalty=1.05`, 11th corpus: **KEEP.**

**Commit-field corpus totals** (every seat reported them as counted fields; 1,209 seat-side
decisions across the seven seats):

| field | corpus | per-seat range |
|---|---|---|
| `post_plan_overrun` > 0 | **~121 / 1,209 = 10.0%** | 7.3% (139) - 12.4% (36) |
| `commit_retracted` == true | **0 / 1,209** | 0 at all seven |
| fatal overrun + retraction | **0** | 0 at all seven -- **the wave-32 promotion trigger is NOT met and is now MOOT (owner ruling)** |
| replies with no `PLAN:` line (compliant since `f46dd58ee`) | **48 / 1,209** | 1 - 20 |
| `pruned_pairs` populated | **1** (deck152) | never written at 105/116/139/146/36 |
| **last-label-differs re-commits** (the class the fields CANNOT see) | **45 / 1,209 = 3.7%** seat-side (owner's corpus figure 52/1,249 = 4.2%) | 1.8% (146) - 5.2% (152) |

**Every seat independently reported that the two shipped fields measure the wrong boundary.** The
phenomenon is post-**ANSWER**; the instrument is post-**PLAN**. deck116 measured the gap exactly:
263,247 post-answer chars against 14,025 the field counts -- **the field sees 5.3%.** deck158
measured it from the other end: post-answer text is **98.0% of all reply text** at its seat
(280,919 of 286,554 chars in six games). This is §5 and it is the single most consequential
instrument finding of the wave.

---

## 1. THE WAVE'S DOMINANT PATTERN -- THE POISON ARC CLOSED, AND IT COST A PERFECTA

**The step-1 mandate was the poison pair. It shipped, it validated at every exercising seat at
100%, and the record it was predicted to move MOVED: deck105 went 6/6 -> 2/6.**

### 1a. The pair, measured as FRACTIONS at seven seats (HL5, never a sample)

| seat | i1 poison rendered (post-first-event prompts) | i2 infect blocker windows partitioned | `you would be at N - NOT lethal` on an infect window | non-infect negative control |
|---|---|---|---|---|
| **105** | **120/120 = 100%** (63/63 own seat, 57/57 opponents) | **10/10** | **0/10** | 17/17 unchanged |
| **36** | 17/17 | **3/3** | 0/3 | 7/7 unchanged |
| **139** | 4/4 | **4/4** | 0/4 | 3/3 unchanged |
| **146** | 15/15 | 1/1 | 0/1 | unchanged |
| **152** | 25/25 own, 19/20 opponent (gap is pre-first-counter) | 1/1 | 0/1 | 8 + 2 LETHAL, unchanged |
| **158** | 11/11 | 1/1 | 0/1 | 7/7 unchanged |
| **116** | 10/10 own, 12/12 opponent | 1/1 | 0/1 | 8/8 unchanged |

Non-combat poison (`alterpoison` / Ichor Rats) narrates at **every** seat that saw it; the wave-32
four-silent-events class is **dead corpus-wide**. Per-attacker annotation reaches 100% of infect
attacker lines. Zero leakage into ordinary combat at any seat.

### 1b. The behaviour delta, which is the evidence -- not the win table (deck105 P5, adopted)

> **infect blocker windows ending in a DECLARED BLOCK: 8/12 (wave-32) -> 10/10 (wave-33).**

n=22, measured at the OPPONENTS' seats where the fix landed. That single line is what makes the
record delta interpretable. Both halves are required and both are present: a behaviour delta with
no record delta is a fix that did not matter; a record delta with no behaviour delta is not
attributable to the fix at all. **This is now a rung (HL1's re-run leg).**

The wave-32 reference repro is inverted at its own seat: deck36 s25 t12 was `no blocks` at 20 life
into four infect attackers and a lost game; wave-33's rematch is **3/3 blocks and a WIN at 20 life
on turn 11**, with the replies naming the new fact as the reason (*"every counter is permanent and
brings you closer to losing"*). deck152's pilot chump-blocked a 2/1 with a legendary lord at 20
life after quoting the forecast's arithmetic back, and won the game it lost at 23 life last wave.
deck146 won the rematch of its wave-32 loss-at-21-life at 19 life on turn 10.

### 1c. deck105's asterisk is DISCHARGED. The perfecta was substantially a render artifact.

| | wave-32 (poison-blind opponents) | wave-33 (honest forecast) |
|---|---|---|
| deck105 record | 6/6, all poison alt-wins | **2/6**, both poison alt-wins |
| deck105 life at gameend | 20, 6, 3, 20, 2, 8 | **0, -5, 0, 0**, 20, 17 |
| poison delivered in games LOST | n/a | **1, 1, 4, 1** -- never within 5 counters of the win |

The arrival trace is worked end to end at 105 v 158 (findings §0): the blocker window now reads
`you would be at 4 of 10 poison ... every counter you take is permanent progress toward losing`,
deck158 blocked, removed the other half of the clock at the source, and killed deck105 on turn 8 at
0 life. **Roster verdict: deck105 is not a pool-breaking construction; it is a fragile linear deck
the pool answers by blocking and by removing or stealing its two-power bodies. No difficulty-ladder
pricing may use the wave-32 number.** **N-105a and N-105b are CLOSED.**

The wave-32 diagnostic ("correct strategy, wrong count every time it mattered") is **discharged at
the base**: 0/123 hand-count re-derivations, 4/5 poison-arithmetic replies exactly right off the
rendered base, and the residual error is a per-creature POWER misread -- a different and much
cheaper class.

### 1d. The cost of the fix, stated honestly

The tax RELOCATED rather than vanishing. deck105's p90 latency rose 61.8s -> 75.6s and its four
largest replies moved from poison-counting to the combat seam, where the successor belief is *"the
guide forbids me to block and I am about to die"* (§2). **A stopgap's retirement must ask what the
residual tax rehearses NOW** -- HL3 rung 2 firing exactly as written, at the seat that wrote it.

---

## 2. SECOND PATTERN -- THE GUIDE LAYER IS NOW THE LARGEST LOSS SOURCE, AND THREE SEATS LOST GAMES TO THEIR OWN SENTENCES

Wave-32's guide-lane losses were mostly *absent* teaches. Wave-33's are *authored* ones. Four
distinct shapes, all curable in prose, all with verbatim self-citations:

| seat | the sentence | what it did |
|---|---|---|
| **105** | *"That 'you would be at N' number ... N is 9 or more -> BLOCKS: none"* | the render line contains TWO numbers; the pilot bound `N` to the DAMAGE, found its own guide self-contradictory (*"This is a contradiction. Let's assume the example is correct and the rule is misstated"*, 7,788 chars on a LAND DROP), then at 10 life vs a 9-power swing wrote *"re-reading the guide's specific math ... Therefore, I block with none"* -> 1 life -> **dead turn 10** |
| **152** | *"Mulligan only hands ... that cannot make GW mana"* | read as a DISQUALIFIER it fires on every mono-coloured opener, and deck152's 25 lands split 14 G / 15 W with **only 4 making both** -> ~40% of three-land openers. **Two false mulligans of three; keep 5; Sigarda bottomed; the vs158 LOSS** |
| **139** | the wave-32 revision **DELETED** *"Every creature in this deck is non-Human"* while KEEPING the condition *"when a non-Human creature is on your battlefield, take the mutate line"* | **5 replies asserting a deck creature is Human, mutate take-rate 6/7 (86%) -> 1/6 (17%)**, the deck's #1 rule off in five of six games. Zero instances in the entire wave-32 corpus |
| **116** | *"Deploy a body every turn"* read as a ban on anything else | **7 turn-3 windows** where a payable {1}{G} spell was the only offer answered `Cast nothing` -- *"None of the available spells are Enablers or Creatures"* -- against RULE #5's own last line licensing exactly that |

Three of the four are the same underlying error in different clothes: **a rule the pilot cannot
evaluate, or can evaluate two ways.** deck105's threshold has an unnamed variable; deck139's
condition names a property with no fact and no surface behind it; deck152's criterion is stated in
the direction that reads as a description and used in the direction that reads as a gate. Only
deck116's is an ordinary over-broad absolute.

**Two promotion-grade rungs come out of this** (HL2 and HL4 below): *a threshold rule must name its
number by the render substring that precedes it*, and *before deleting any guide sentence, name the
SURFACE that will carry its fact -- a zero occurrence count is not the answer, it is the question.*

deck139's case is the sharpest thing the campaign has produced on demotion. The wave-32 audit
checked that exact sentence, recorded it **TRUE**, and deleted it under the three-part demotion of
validated teaches -- because nothing in the rung distinguished *"the RENDER absorbed this teach"*
(safe to delete, HL3's SUPERSEDED verdict, and its safeguard is the quote requirement) from *"the
render never carried it and my own sentence is the only thing holding the line"* (never safe). And
the mechanical tell fires cleanly: `subtype=` is a declared primitive field the render emits
**ZERO times in 190 records**.

---

## 3. THIRD PATTERN -- A COUNT RETIRES ONE AXIS, NOT THE MECHANISM (the i9 result, and its shadow)

**i9/N-139n is the cleanest single-fix result the campaign has produced.** At deck139: mulligans
**9 -> 0**, false mulligans **6/9 -> 0/0**, bottom asks 3 -> 0, `zero green sources` **0 hits**,
mean kept hand 5.8 -> **7.0**, header arithmetically correct 6/6 including two one-green-source
hands the wave-32 belief would have thrown away. At deck146: mulligans **19 -> 2**, smallest kept
hand 0 cards -> 5, `Mana available:` in a pregame prompt **0/8**, the N-146k echo class **0 replies**
(was 5/14). At deck116: **6/6 keeps, zero mulligans** (wave-32: 7 and a 0-card hand). The
satisfiability line (`Playing every land in this hand would cover the cost of: ...`) closes HL7
rung 3 by construction -- it does not merely count, it demonstrates a castable opening exists.

**And the belief migrated, to the nearest class with no aggregate.** deck139's `{G} 3` killed the
land/colour axis and the identical mechanism -- recite the class, exclude a member, invent a
disqualifier -- re-landed on creature TYPE (5 instances) and on summoning sickness on an OFFERED
attacker (1 instance). Three axes, one mechanism, and **the axis with the engine-computed aggregate
is the only clean one**:

| axis | guide sentence | per-item tag | engine COUNT | misreads |
|---|---|---|---|---|
| green mana sources | present | `(land: taps for {R}{G})` | **`{G} 3` (i9)** | **0** (was 6) |
| non-Human creature type | deleted w32 | absent (`subtype=` renders 0x) | absent | **5** |
| summoning sickness on an offered attacker | absent | absent on the A-line | absent | **1** |

deck152 supplies the boundary case from the other side: its count header fired, was arithmetically
correct, was RECITED in both false mulligans -- and did not bind, because the pilot's disqualifier
was about the DECK'S PLAN, not the hand's contents. **That is not membership denial, it is COVERAGE
denial, and no count answers it. Only removing the licence does.** The two diagnostics now sit side
by side in HL6.

The ranked escalation ladder that comes out of this wave (deck139 P2, adopted): **(a)
offer-as-evidence** -- *"the engine only prints this line when the condition holds"*, which removes
the property from the pilot's reasoning entirely and needs no render change wherever the legality
is gated by an `otherrestriction`; **(b) engine-computed count; (c) per-item tag; (d) guide
assertion.** A fourth form beat all three of the wave-32 ladder's rungs here and costs nothing.

---

## 4. FOURTH PATTERN -- THE ANSWER-SHAPED-THING FAMILY: FOUR CONSUMERS, THREE ANCHORING RULES

Four independently-found defects at four seats are one family. **The reply contains more than one
answer-shaped object, and the engine's consumers disagree about which one is authoritative.**

| item | consumer | anchoring rule | consequence |
|---|---|---|---|
| `parseChoice` | the DECISION | **LAST** line-leading coded label | correct, and decision-critical: deck36 vs146 s18 emitted `CHOICE: 2 (Yotian Soldier)`, 4,600 chars, `CHOICE: 1 (Akroma's Memorial)` -- Akroma's resolved and **won the game** |
| **N-36e** `consumePlan` | the NEXT PROMPT | **FIRST** `PLAN:` | deck36 vs105 s35: the abandoned char-501 plan is carried forward as `YOUR PLAN (as you last stated it)` (1,424 chars of retracted deliberation) and the committed char-5,359 plan is **discarded**. **107/201 prompts (53%) carry a >400-char plan field** (wave-32: 39%) |
| **N-146m** reconciler | the DECISION | **INDEX wins over NAME** | deck146 vs116 s34: `CHOICE: 1 (Nadaar, Selfless Paladin ...)` -- the answer to the PREVIOUS question, naming a card absent from the menu -- committed as `Tomb of Annihilation`, against the guide's #1 dungeon teach. Everything downstream (the Tomb chain, two 200s+ spirals, Acererak discarded, the cap) is its cost. **1/189, silent, decision-fatal** |
| **N-139x** `commitRetracted` | the INSTRUMENT | fallback-scoped only | cannot see a re-commit the parser ACCEPTS. **45 corpus-wide; 4 of deck139's 6 also score `post_plan_overrun` 0** |

Plus the partial-drop face: **N-139y** (one blocker named against four attackers; first-wins
parser correct; three assignments dropped SILENTLY -- `unparsed_reply` does not fire because one
survived, `all_assignments_illegal` does not fire because not all were illegal; 7 poison connected
and the game ended) and deck116's `BLOCKS: B1:A1, B1:A2` lenience datum.

**Ranked as one family in the ledger.** The owner's protocol change reduces the *population* of
multi-answer replies but does not fix any of the anchoring rules: N-146m fires on a stale echo, not
on a spiral, and N-36e's consumer would still take the first of two PLAN lines. The cheap
consolidation is: **every consumer of a reply anchors LAST, uniformly, and any name/index
disagreement is a rejection, not a coincidence.**

---

## 5. THE OWNER RULING -- WAVE-34 STEP-1 SPEC AND ITS A/B (binding; not adjudicated)

The ruling (brief OWNER DIRECTIVES item 1) eliminates the post-choice reasoning block. Seats were
instructed to supply evidence only, and all seven complied -- **zero counter-proposals, zero
reorderings, zero terminators offered.** deck158 additionally **WITHDREW** its own wave-32
terminator flag, on two grounds (0 retractions in 138 decisions, and the ruling supersedes the
mechanism). Synthesis's job is the implementation spec and the measurement, below.

### 5a. The consolidated value ledger (owner question (b))

| seat | last-label-differs | improved | neutral | **harmful** | notes |
|---|---|---|---|---|---|
| **152** | 11 / 211 (5.2%) | 9 | 1 | 0 | **+1 LEGALITY RESCUE** (`B1:A1, B1:A4` -> `B1:A1`) |
| **36** | 8 / 201 (4.0%) | 8 | 0 | 0 | one **GAME-DECIDING** (Akroma's Memorial, won turn 13) |
| **158** | 6 / 138 (4.3%) | 4 | 1 | **1** | the 1 **destroyed a correct answer** (1 damage moved off a 1/1 onto a 2/3) |
| **139** | 6 / 178 (3.4%) | 5 | 1 | 0 | every first answer preceded the deliberation that corrected it |
| **116** | 5 / 109 multi-option (4.6%) | 3 correct conclusions, **only 1 realised** | 1 | 1 | the value materialises only when a second `CHOICE:` line happens to be emitted |
| **146** | 4 / 227 (1.8%) | 4 | 0 | 0 | incl. the game-winning Emeria's Call and a legality self-repair |
| **105** | 7 / 240 both seats (2.9%) | 2 (low-stakes) | 3 | **2 decision-relevant, BOTH harmful, ONE FATAL** | `BLOCKS: B1:A2, B2:A1` -> scratch -> `BLOCKS: none` -> 1 life -> loss |

**Adjudicated answer to (b): the value is REAL, it is roughly 4% of decisions, it is net positive
at five seats and net negative at one -- and not one of the ~35 improvements required the reasoning
to be POST-answer.** Every one is "the model thought harder and reached a better answer." Three
seats state the consequence in the same words: native pre-answer reasoning does not merely preserve
this 4%, it **strictly dominates** it, because it delivers the deliberation to 100% of decisions
and removes the failure mode entirely (there is no first answer left standing to be argued away
from). deck105's two harmful flips and deck158's one are only possible under answer-first.

**The caveat that must not be lost:** at deck116 three of five correct conclusions were DISCARDED
because no second `CHOICE:` line was emitted -- including one that quotes its own guide rule
verbatim, applies it correctly, concludes *"So I will cast Time of Need"*, and casts Fabricate.
**Today's value capture is an accident of formatting, not a mechanism.**

### 5b. Load-bearing scratch for REVIEW (owner question (c)) -- and the hard prerequisite

**Six of seven seats report that discarded scratch text was load-bearing for the REVIEW**, and
three of them (152, 116, 105) state that capture is a **hard prerequisite, not a nice-to-have**:

- **152**: *"the whole diagnostic surface of this seat's review"* -- both false mulligans, the
  successor mana belief, the i2 validation and the core-prompt PASS are recoverable ONLY from prose
  the protocol calls discardable. Every rung that depends on naming a belief in the pilot's own
  words (HL6's recite-then-exclude tell, HL10's distinct-belief count, HL7's anti-pattern-verbatim
  rule, the tax table) would have nothing to read.
- **116**: without the scratch at vs158 s7 *"this wave's revision would have rewritten a rule that
  is working perfectly."*
- **105**: two of its five headline items (the ambiguous-threshold discovery and N-105g's
  root cause) exist only because the scratch reached the translog.
- **36**: N-36e and N-36j are each evidenced by nothing else. **139**, **158**: same shape.

**SPEC CONSEQUENCE, non-negotiable: the wave-34 dev loop must persist `reasoning_content` into the
translog, and the A/B must be gated on that field existing and being non-empty at BOTH seats before
the corpus runs.** A corpus that turns native reasoning on without capture leaves seat review with
`choice` + `chosen_text` + a terse answer and blinds the method.

### 5c. Wave-34 step-1 implementation spec (protocol + plumbing)

**Owner's final ruling, and the frame the spec is built to:** the dev loop KEEPS reasoning, because
it is what makes the model's experience auditable. What changes is where it lives. Moving it to the
model's NATIVE reasoning window buys three things at once -- **(a)** it precedes and benefits the
choice instead of trailing it, **(b)** it uses the reasoning the model was actually trained to do
rather than an improvised scratch register, and **(c)** it still yields auditable content. The
scratch block is not being deleted to save tokens; it is being replaced by a better-sited channel.

1. **Reply protocol text**: ANSWER + a concise, changed-only PLAN. **Nothing else invited.** Delete
   the post-choice scratch invitation and the "that scratch text is discarded" sentence; do NOT
   replace them with a terminator or a brevity clause (owner-ruled, and deck158's flag is withdrawn).
2. **Native reasoning**: request-level `enable_thinking` / reasoning-effort. **Dev loop / corpus runs
   = ON. Shipped user default = OFF, terse replies.** The plumbing exists; the work is the wiring
   and the option surface.
3. **Capture, and it must be ROBUST TO THE SERVER'S PARSER STATE.** Spark's vLLM may or may not be
   running a reasoning parser for the served model, and the two cases put the reasoning in different
   places:
   - **parser ON** -> the reasoning arrives as `message.reasoning_content` and the visible `content`
     is already clean. Log `reasoning_content` verbatim to the translog.
   - **parser OFF** -> the reasoning arrives INLINE in `content`, wrapped in `<think> ... </think>`.
     **The client must strip it and log it**, not pass it to the parser.
   **The client therefore implements both paths unconditionally**: read `reasoning_content` if
   present; otherwise strip a leading `<think>...</think>` block from `content` into the same
   translog field. Two hardening requirements the corpus will exercise: an **UNCLOSED** `<think>`
   (truncation, or a model that opens but never closes) must not swallow the answer -- if no
   `</think>` is found, treat the reply as reasoning-only and re-ask rather than parse the think
   text as an answer; and the existing `post_plan_overrun` / `post_answer_overrun` /
   `hasCodedAnswerLine` scanners must run on the **stripped** text, on the same normalisation, or
   every length field silently measures thinking tokens instead of reply tokens.
4. **Sampling parameters follow the mode, per the model card.** Qwen's thinking mode wants
   **temp 0.6 / top_p 0.95**; the non-thinking defaults in use are temp 0.7 / top_p 0.8 / top_k 20 /
   presence_penalty 1.5. **The request must switch the sampling block together with
   `enable_thinking`** -- shipping thinking under non-thinking sampling is a confound the A/B cannot
   separate from the protocol change. `repetition_penalty=1.05` (11th corpus, KEEP) is orthogonal
   and stays on both arms.
5. **Instrumentation, three fields, no behaviour change:**
   - **`post_answer_overrun`** (int) -- chars after the end of the FIRST line-leading coded answer
     (`CHOICE:` / `ATTACK:` / `BLOCKS:` / `PUT:`), on the same `</think>`-stripped normalisation the
     existing fields use. **This is the field the ruling is legislated against; `post_plan_overrun`
     sees 5.3% of it** (deck116, measured) and is format-sensitive besides (it scores 262 on a
     14,206-char post-answer reply that writes `Plan:` lowercase and last, and 0 on a 13,326-char
     reply with no PLAN line -- both legal under `f46dd58ee`).
   - **`answer_replaced`** (bool) / **`coded_answers`** (int) -- last line-leading label differs
     from first. This is the 4.2% the owner cites; it is currently produced only by post-hoc regex
     and is invisible to both shipped fields.
   - **`dropped_assignments`** (int) on blocker records -- count of `B:A` pairs the reconciler
     discarded as illegal or duplicate. N-139y's three silent drops decided a game and no counter
     recorded them. (`pruned_pairs` exists but was emitted on exactly ONE record corpus-wide; check
     whether it is wired on this path at all -- five seats report it never written.)
6. **`commit_retracted` is retained but must be documented as fallback-scoped** (`AIPlayerGPT.cpp:2259`
   returns false unless the fallback is `retracted_choice` / `truncated_abandoned` /
   `truncated_abandoned_heuristic`). It is a counter of retractions that reached the HEURISTIC, not
   a counter of retractions. Reporting it as the latter under-counts by 45/45 this wave.
7. **Seat review re-points at the captured reasoning.** Reviewers currently read post-answer scratch
   to name a belief in the pilot's own words; from wave 34 they read `reasoning_content` instead.
   The reviewer brief must say so explicitly, because the rungs that depend on it (HL6's
   recite-then-exclude tell, HL11's distinct-belief count, HL7's anti-pattern-verbatim rule, the tax
   table, HL6's BELIEF-MISS vs COMMIT-MISS split) will otherwise read an empty reply and score every
   taught class off the decision alone. **The reasoning is now attached to the decision it actually
   produced, instead of trailing the one it did not** -- that is a gain in review resolution, not a
   loss, and the one thing that gets structurally harder is distinguishing a commit miss (HL6): if
   reasoning moves pre-answer and commit misses vanish, a seat that scored them as BELIEF misses
   this wave will read the disappearance as its own revision working.

### 5d. The A/B corpus -- required metrics

**The A/B must show NET latency and NET decision quality, not either alone.** Thinking tokens are
still decode tokens and Spark decodes at roughly the same rate whichever window they land in, so a
protocol change that removes ~2,000 chars of post-answer scratch and adds an unbounded thinking
block can come out latency-NEUTRAL or worse. **Total decode time per decision is a primary metric,
not a diagnostic.** Report it as: total decode time, split into thinking tokens and reply tokens,
per decision and per game, against the wave-33 baselines (median 15.8-18.1s, p90 53-82s, p95 130-186s
per seat; 21 games at 0 timeouts and 4 cap adjudications).

**Primary (decision quality):** **first-label-vs-last-label divergence** (`answer_replaced`) --
which must go to **zero** by construction, so its disappearance is NOT evidence of improvement; it
is the change landing. **The real quality primary is per-seat re-measurement of each seat's own
falsifiable predictions** (each seat wrote them; deck139's and deck36's are the most explicit), plus
the value ledger of §5a re-run: the ~35 improvements the post-answer block was delivering must
still happen, on the FIRST line, or the change has bought latency with decision quality.

**Mandatory alongside, each named by a seat:**
- **`post_answer_overrun`** distribution, not `post_plan_overrun` (all seven seats), measured on
  the `<think>`-stripped reply.
- **ILLEGAL-ASSIGNMENT RATE** as a first-class metric -- `all_assignments_illegal` +
  `dropped_assignments` + the deck146-s29 shape. deck152's evidence: the post-answer block caught
  one illegal block and missed one (1 of 2), so this is the one axis where the discarded text is
  doing SAFETY work rather than value work, and it is already instrumented.
- **Latency + p95 reply chars per seat**, and the >100s decision count (the cap question, §7).
- **Parser semantics baseline**: today the engine binds the **LAST** label (verified at 4 seats).
  Whatever ships, the comparison must be aware the baseline is last-label-wins, not first.
- **STRATIFICATION** (deck146 (d)): the three worst spirals in the corpus are re-derivations of
  state the prompt does not state (N-146n). If the A/B moves reasoning native without closing that
  ambiguity, the reasoning simply spirals in the native channel and the latency finding is
  confounded. **Either ship N-146n in the same batch or record room-choice asks as a separate
  stratum.**
- **Plan-field pollution**: `YOUR PLAN` block length distribution. Three seats show the scratch
  register writing into the only durable state the pilot carries (deck158 146 s19's plan is a
  verbatim arithmetic breakdown replayed into the next prompt; deck146's 9,091-char spiral
  re-injected into two following prompts; deck36's N-36e). Under the target protocol none of these
  shapes can form -- that is a predicted effect and should be measured, not assumed.

### 5e. Owner directive 2 (dev-vs-user split) -- the measured user cost

| seat | post-answer text | per game |
|---|---|---|
| 158 | 280,919 chars over 6 games = **98.0% of all reply text** | ~46,800 chars ≈ 12k output tokens |
| 116 | 263,247 chars, mean 2,009/reply | ~44k chars |
| 105 | 221,905 chars (seat), 394,864 (both seats), mean 1,804 | ~37k chars ≈ 10k tokens |
| 139 | 275 KB of reply text, mean 1,583 | ~46 KB ≈ 11k tokens |

Corpus-wide the per-decision post-answer mean lands at 1,800-2,035 chars at four independently
measuring seats, reproducing the owner's ~1,900 figure three times over. **Latency corroborates:
deck158's two worst replies are a three-option LAND DROP at 214.5s / 14,570 chars and a target pick
at 192.1s -- both answered correctly on their first line. A user waited ~3.5 minutes for a choice
the model had already made.**

### 5f. Owner directive 3 (narration decoration) -- the estimate is right, and it is the wrong target

Three seats measured it: deck36 **72/201 prompts, mean 81 chars, max 501** against a p50 prompt of
5,754; deck139 20/178 prompts; deck105 confirms the ~320-char per-line estimate. **Verdict:
confirmed SMALL, wrong-by-architecture, not a cost driver -- ship it as correctness, do not expect
it to move the clock.**

**But deck158 found the large sibling and it revises the docket upward 5-6x: the menu echo replays
a CONSUMED ask's ENTIRE instruction text, in the imperative, as history** -- 950 / 1,672 / 1,980
chars = **15.4% / 18.7% / 18.1% of the game log** in its three measured games, growing with game
length. Everything before the `->` is live instruction addressed to a decision that is over. The
narrated form needs only what was chosen. **Routed as L-158-2 and ranked ABOVE the decoration
strip in the ledger.**

---

## 6. THE STEP-1 BATCH -- per-item verdicts

| item | verdict | evidence |
|---|---|---|
| **i1** N-105a poison rendered | **VALIDATED, 100%, CLASS CLOSED** | 120/120 at deck105's two seats; 17/17, 15/15, 11/11, 25/25, 10/10, 4/4 at the other five. Non-combat poison narrates everywhere; correctly suppressed at 0/0 so non-infect duels pay nothing. Three of deck105's own validation predicates run as written and all three pass. **Residual: N-116j (aggregate line precedes its per-source causes, cosmetic) and N-36i-b (the pair is suppressed while both totals are 0 -- the same falsy-zero family i5 just closed; owner call, cost zero).** |
| **i2** N-105b honest blocker forecast | **VALIDATED, both directions, CLASS CLOSED** | 10/10 + 3/3 + 4/4 + 1/1 x4 partitioned; **0 occurrences** of the forbidden `you would be at <life-N> - NOT lethal` on ANY infect window at ANY seat; the misteach clause has 0 occurrences on the infect branch; negative controls unchanged at all seven. Behaviour: blocks 8/12 -> 10/10. |
| **i3** N-146j `[-multicolor]` | **PARTIAL PASS -- falsifier PASSES, positive half UNEXERCISED** | deck146: 21 windows with a real target list, 23 distinct names, **zero artifacts, zero lands, zero Clue slips**. The positive half (a monocolored ARTIFACT offered) has **no witness**: a whole-seat regex for a permanent with `[artifact]` and a coloured cost returns 0, and wave-32's witnesses (Glaze Fiend, Salvage Slasher) were never drawn. deck36's board is 100% generic-cost artifacts. The 8 `only legal targets are YOUR OWN` windows are now **RULES-CORRECT**. **Discharge from deck36's seat or a probe -- not by a seventh corpus of waiting.** |
| **i4** N-152f `deferFlexibleSource` | **VALIDATED, ARRIVAL-TRACED ON THE BROKEN BRANCH, CLOSED** | Two arrivals, not offers: deck36 s26 (Teferi `{2}{U}{W}` off exactly four sources, every white flexible, the only blue also the only white) and deck116 s29 (Sigarda `{1}{G}{W}{W}` off three flexible white sources incl. **Katilda tapping under her own lord ability** -- `Mana available: 8 total` over 7 lands). **NEGATIVE-CONTROL LADDER in the same game**: s12/s19/s24 correctly suppress, the offer appears at s29, the window the Overgrown Farmland arrived -- a ONE-SOURCE delta. **False-suppression sweep 0/44 casting windows.** |
| **i5** N-36b six falsy-zero guards | **VALIDATED at the natural witness, with a NEW residual (N-152k)** | deck36: **508/508** cost emitters carry `{0}` (55 hand / 26 cast / 427 battlefield) plus a fourth new emitter; **0 confabulations in 201 replies**; `Cast nothing right now` **0 taken of 45 offers** (wave-32: 7 taken, 4 declining a free permanent). **RESIDUAL: the guard flipped from "print nothing when falsy" to "print `{0}` when falsy", which is right for a real zero and WRONG for an ABSENT cost** -- `Day {0}` / `Night {0}` / `Moonrage Brute {0}` / `Tovolar's Packleader {0}`, **166 false renders at deck152 alone**, 8 at deck139, 20 at deck158, **all zero in the wave-32 corpus.** |
| **i6** N-158m amass macro scanner | **VALIDATED, CLASS CLOSED** | **3/52 (5.8%) -> 59/63 (93.7%), which is 59/59 = 100% of non-trigger-gated offers.** The four misses are all Easterling Vanguard, whose amass is `_DIES_`-gated and correctly suppressed. Both branch forms render and both are correct against the narration. Assault on Osgiliath -- the negative control -- **was never offered for the third consecutive corpus** (S6). |
| **i7** N-152d layer 2 `(printed X/Y)` | **VALIDATED, CLOSED, with live positive controls** | `Moonrage Brute (3/3) (printed 2/2)` **22 -> 0**; Brute clean 9/9, Packleader clean **24/24** (wave-32's zero was coverage; this is proof), Cathar 110/110, Huntmaster 49/49. **Positive control: 51 distinct tagged renders at TRUE deltas in the same corpus.** Confirmed at three further seats: deck116 0 false tags in 131 prompts, deck139 106 tags 0 zero-delta, deck158 46 tags all true. |
| **i8** name-form `BLOCKS:` + `all_assignments_illegal` | **PARTIAL: the new fallback class VALIDATED; the name-form legs PARTIALLY exercised** | `unparsed_reply` **0 corpus-wide** (wave-32: 2). Name-form parsed at deck139 (3 witnesses incl. the colon-inside-the-label shape that broke the wave-32 reconciler) and deck116 (both forms); **UNEXERCISED at deck158 and deck152** (all replies used `Bn:An`). `all_assignments_illegal` fired **once**, with its pruned pair named -- **N-139s CLOSED.** Trigger shape recorded in the ledger. |
| **i9** pregame hand-only + count header | **VALIDATED for its own scope, CLASS DEAD -- and the SCOPE LEAKS (N-158r)** | Mulligans 9->0 (139), 19->2 (146), 7->0 (116); 0 battlefield facts in 8/8, 6/6, 8/8 pregame prompts; **0 `Mana available: 0 total` echoes corpus-wide**; header arithmetically correct at every checking seat. **BUT the gate is `observer->turn == 0`, a PROXY** -- the player ON THE PLAY takes their first main phase at turn 0, so their first LAND DROP is served the pregame prompt: 3/3 of deck158's on-the-play games, 0/3 on the draw, one at **214.5s / 14,570 chars on a three-option land drop**. **N-158r, HIGH by cost.** Leyline-in-pregame: **0 occurrences**, the known consequence did not arise. **Untested direction: no unkeepable hand appeared at deck139 or deck36, so the FALSE-KEEP failure mode of a count header is unmeasured.** |

---

## 7. THE CAP QUESTION -- 4 adjudications, and the verdict is HOLD BOTH LEVERS

Four cap adjudications (wave-32: 1). Every one is latency-driven with the board alive and
correctness intact; **not one is a stall, a retraction, or a decision-starved game.** ~95% of a
capped game's budget is inference.

| game | shape | the actual driver |
|---|---|---|
| 116 v 146 | joint 2,853s of 3,000 | deck146's seat alone 30.4 min, p90 199.7s; **three 200s+ dungeon-room spirals (N-146n)** |
| 116 v 152 | joint 2,895s | deck116: **four decisions consume 562s = 52% of its game budget** out of 31 |
| 116 v 36 | joint 2,569s | deck36 **1,910s = 74% of the wall clock**; **831s (44% of its budget) on six decisions, four of them "which of my tapped lands do I throw away" (N-36j)** |
| 152 v 36 (won, adjudicated ahead) | 38 decisions in 3,000s = 79s/decision | per-decision cost at -j 3; reply lengths FLAT vs wave-32 |

**VERDICT: wave-34 keeps `-T 3000` and `-j 3`. Do not raise the cap; do not drop to -j 2.**

Three reasons, in order of weight:

1. **The A/B needs the cap held constant.** Wave-34's step 1 is a protocol change whose primary
   predicted effect is a collapse in output length and therefore in latency. Changing the cap or the
   concurrency in the same wave makes the latency result uninterpretable -- the exact confound
   §5d's stratification clause is written to avoid.
2. **The expected mechanism is large and specific -- but it is NOT a free win, and the check must be
   stated as a net.** Decode latency is ~linear in output tokens and this stack decodes at roughly
   the same rate whichever window the tokens land in. Post-answer text is 98.0% of reply text at
   deck158 and ~2,000 chars/decision at four seats, so the protocol change removes a large,
   measured quantity -- and **native thinking tokens are still decode tokens and will take some of
   it back.** The wave-34 result may therefore be a length collapse with a smaller latency
   collapse, and the A/B is specified to report total decode time split into thinking vs reply
   tokens for exactly that reason (§5d). What the change buys unconditionally is that the tokens
   are spent BEFORE the answer instead of after it. At deck116 the four decisions costing 52% of a
   game budget are all post-answer text; at deck152 the two largest replies in the adjudicated loss
   are 15,082 and 11,573 chars. **A cap raise buys deck36's vs116 game about two more turns; the
   protocol change plus N-36j plus N-146n are aimed at the same 831s directly.**
3. **The residue is render-lane, not clock-lane.** deck36's sacrifice seam (N-36j), deck146's room
   menu (N-146n) and deck158's pregame latch (N-158r) each produce 200s+ specimens that a cap raise
   would not touch and a protocol change would only partly mask. **All three are in the wave-34
   step-1 batch.**

**The falsifiable check for wave-35** (state it in the wave-34 brief): report, per seat, p95 reply
chars, **total decode time split into thinking vs reply tokens**, and the >100s decision count. **If p95 reply chars fall by >60% and cap adjudications are
still >=3, the prose lever is exhausted and the next lever is `-j 2`** (which trades corpus wall
clock for per-game decision budget) -- not a cap raise, because a raise rewards the slow game
rather than making it faster. If adjudications fall to <=1 with the length collapse, both levers
stay untouched.

---

## 8. CORE PROMPT -- PASS, 28th consecutive (7 of 7 seats)

No core-prompt change is proposed by any seat. Fallbacks 1/1,277 corpus-wide, 0 out-of-range
answers, 0 example-echoes, 0 protocol violations.

### 8a. deck105's G1 (the held alt-win clause) -- **DISCHARGED. DROP, do not adopt.**

Wave-32 HELD the proposed widening of `AIPlayerGPT.cpp:72` (was :63) *"You win by reducing the
opponent's life from 20 to 0"* on the explicit condition that it be re-checked with poison rendered
and adopted **only if the conflation PERSISTS**. It does not, at four independent seats:

- **105** (the owner seat): **0 hits** for the wave-32 conflation shape in 123 decisions (the
  exemplar was *"the opponent has 21 life. I need 20 poison. I have 11"*); 0 reconciliation spirals;
  5 replies quote the RENDERED threshold back correctly. The three *"reduce the opponent's life to
  0"* statements in these six games are all at OPPONENT seats and all TRUE for those decks.
- **139**: priced the alt-win correctly and unprompted in every infect window; lost that game to a
  blocker-assignment rules error, not to a win-condition misconception.
- **152**: *"the false core sentence was in front of it in every prompt and the RENDERED number
  overrode it -- which is the routing the loop already believes."*
- **146** and **116**: same, independently.

**ADJUDICATED: DROP G1 from the held list.** The false sentence is INERT once the alternate win
condition has a rendered state variable -- which is exactly what the wave-32 self-hold predicted.
Widening it would spend a core edit on a defect the render has already retired and add a clause
every non-alt-win duel pays for. It is replaced by one line in HL1's intake check: **a pool win
condition with no rendered state variable is a P0 RENDER item; the core sentence is cosmetic
residue, corrected only if a core edit is being made for another reason** (minimal correct form
recorded in deck105's general-suggestions.md for whoever makes that edit).

### 8b. deck158's COMMIT terminator -- **WITHDRAWN BY ITS OWN SEAT.** 0 retractions in 138
decisions, the wave-32 specimen did not recur, and the owner ruling supersedes the mechanism. The
promotion trigger (>=2 seats with a fatal overrun+retraction) is **0/7** and is now moot.

### 8c. deck36's phase-header contradiction -- **PROMOTED, on adjusted grounds. Adopt the
representation change as a wave-34 step-1 item.**

The wave-32 condition was *"if a SECOND SEAT verbalizes it"*. **The honest adjudication is that the
condition as written is NOT met and the item is promoted anyway.** Both witnesses are deck36's own
seat, and deck152 -- the seat whose GAMES produced both -- states plainly: *"zero occurrences at
this seat this corpus. I am the seat that produced last wave's evidence and I cannot reproduce it.
The watch should not promote on my data."* That correction is recorded, not glossed.

What promotes it instead:
1. **Recurrence across a corpus AND a binary change that touched the priority path** (`bdd07223c`,
   defender's blocker window opens on the attacker's turn) -- the intent of the second-witness
   condition (is this a one-off?) is satisfied even though its letter is not.
2. **Cost when it lands.** Both witnesses are among their corpus's most expensive replies: wave-33's
   is **177s / 11,969 chars on a `pass`**, in a game adjudicated at the cap two turns later, and it
   is 16% of that seat's reply time in that game.
3. **The failure mode is a WRONG BELIEF, not just tax.** In wave-32 the pilot resolved the
   contradiction by *"assume the game is actually in my Main Phase 1"* -- discarding the rendered
   turn owner while deciding whether to spend mana.
4. **The fix is one format string with zero behaviour change**, and it is a strict improvement on
   the negative control.

**Adopted change:** name the owner FIRST and make the phase possessive --
`It is the OPPONENT'S turn | Phase: their Main phase 1 (you have priority)` /
`It is YOUR turn | Phase: your Main phase 1`. The three properties each do work: owner leads so the
frame is set before the phase name captures it; the possessive leaves no bare "Main phase 1" to
attach the wrong owner to; and `(you have priority)` answers the follow-on question the pilot asked
in both witnesses.

**Falsifier, pool-wide and reported as a FRACTION at every seat** (the promotion rests on a low
per-seat rate holding across many seats, not on any one seat's volume): zero replies containing
"contradiction" within 200 chars of "Main phase". Baseline: deck36 1/42 opponent-turn main-phase
priority windows this wave, 2 replies last wave; every other seat 0.

**Kept separate, deliberately: N-36k** (the game log's turn-scoped entries read as this-turn
entries against the phase header -- deck36 vs158 s16, **227s / 13,930 chars on a `pass`**, the
seat's worst reply). Different mechanism, same symptom class, ranked separately.

### 8d. What core got RIGHT, recorded against future thinning

The **completeness guarantee is now honest and is being obeyed without spiralling.** deck152 s20
verbatim: *"the prompt explicitly states: 'If a spell is a numbered "Cast..." option it IS
payable... If a spell is not in the cast list you CANNOT cast it right now'. Since Teferi is not
listed ... you cannot cast him this turn"* -- 1,300 characters, correct outcome, no contradiction
claim, on a CORRECT suppression. Compare wave-32's kilobyte-scale *"This is a contradiction"*
spirals at the identical seam. **The wave-32 pressure on that sentence was misattributed to the
core prompt and has been resolved in the engine (N-152f).** No widening, no carving.

---

## 9. FIXES THAT LANDED -- recorded, because the loop's failures are easier to see than its wins

- **The mulligan cluster -- the wave's largest guide-lane cost -- is DEAD pool-wide.** 19 -> 2 at
  deck146 (two literal 0-card hands -> a 5-card floor), 9 -> 0 at deck139, 9 -> 0 at deck116,
  0 at deck105 and deck36. Joint credit, and **it cannot be split**: i9's header and the guides'
  hard floors both fired and no measurement here separates them. deck146 says so explicitly and
  deck116 goes further, scoring its own RULE #6 **SUPERSEDED** by the engine header.
- **deck116's target class collapsed.** Games ending with ZERO creatures ever deployed: **3/6 ->
  1/6**, and the survivor drew no castable creature all game (verified across 10 records). Deploy
  floor: 13 windows offered a body or an enabler, 12 taken, and the one decline is CORRECT. The
  wave-32 hardcast delusion (*"accelerating my mana base towards casting Ulamog"*) is at **zero
  occurrences in 131 replies**. Record 1/6 -> 3/6.
- **deck116's reveal REPLY tax collapsed and it is the guide's win.** Wave-32: up to 13,796 chars /
  220.6s. Wave-33: **622-1,054 chars, 11-17s, all nine reveals**, picks 9/9 legal and 9/9 correct
  off 46-51 option lists. The remaining N-116a cost is entirely PROMPT-side, which moves the fix
  from the latency docket to the context-budget docket.
- **deck158's loss-tracing residues both CLOSED.** Fallbacks 2/120 -> **0/138**; the fatal wave-32
  `unparsed_reply` shape is answered by the guide's blocker-seam procedure (**8/8 label-form, every
  assignment legal**); cast floor 32/33 -> **42/42**; the cost-notation falsity fact added in
  wave-32 is **VALIDATED, 0 misreads in 138 decisions**.
- **deck36's decision lane improved on every seam**: attackers 20/21 -> **30/30**, blocks 14/14 ->
  **10/10 including the infect windows that used to be its game-losing shape**, `Cast nothing`
  0/45, N-36d Academy-in-upkeep **6/8 -> 0/7** (fixed by the guide, not the engine).
- **deck139's latency tail is CLOSED and the closure is diagnostic**: >195s decisions **6 -> 1 ->
  0** across three waves on one lever. The wave-32 survivor was the 213s BOTTOM ask, and wave-32
  proved its deliberation WAS the mulligan belief; i9 removed the mulligans and there is no bottom
  record in this corpus. **The tail and the belief were the same object.**
- **deck146's Acererak class STAYS DEAD** (2nd consecutive corpus: 7 offers, 1 cast in the guide's
  exact sanctioned condition, zero passages counting his 5 power), the **attack tax fires** (0
  block-with-my-attackers replies, two explicit holds), and its wave-32 poison loss is **inverted
  into a win**.
- **i10 attachments: the wave-32 probe spec is CANCELLED -- the POOL exercised it.** deck105's
  `Relic Putrescence` attached to deck36's Mox Jet and both render directions fired in **11/11**
  prompts (`Mox Jet {0} [artifact] {attached: Relic Putrescence}` /
  `Relic Putrescence {2}{b} [enchantment] [attached to: Mox Jet]`), correct against the primitive.
  **The wave-32 null was derived from "does MY deck contain a trigger" + "did the corpus sample
  one" and skipped "does ANY POOL deck contain a trigger" -- the third is a construction fact and
  cannot be zero by luck.** That is method headline 12. Residual narrowed to two items: instance
  handles in the reverse render (untested) and Equipment (0/201, no pool deck runs any).
- **Hand of the Praetors EXERCISED and correct end to end** (wave-32: drawn 0 times) -- lord bonus
  in the rendered P/T, cast-trigger per infect creature spell, 3/3 targets at the opponent, carried
  a win. Half of the wave-32 probe debt discharged by natural play.

---

## 10. CROSS-SEAT ITEMS THE SYNTHESIS IS ROUTING RATHER THAN ADOPTING

- **N-116g -- the highest-severity engine item of the wave, and a REGRESSION.** 13 `Put in Play`
  windows offered, 11 taken, **8 arrived, 3 did not (27%)**; wave-32 arrival-traced this class 5/5
  clean on `7fabd9bd0`. Payment succeeds and the `StackAbility` is never pushed
  (`Action added to stack: NextGamePhase` where every success reads
  `Action added to stack: StackAbility. (Source: <card>)`). **All three failures are Upkeep
  priority windows** with a phase advance pending; opponent's-turn upkeep 0/2 arrived, own-turn
  5/6. Mana headroom is not the discriminator. **It cost the vs105 game outright** -- deck116 died
  at 23 life holding a 15/15 annihilator-6 with the enabler online and `creatures: 0` on its board,
  having taken the winning line TWICE. No fallback, no defer, no counter records anything:
  **arrival-tracing caught what every instrument missed.** Bisect candidates named in notes
  (`e4b3cc96e`, `cd2983c2a`/`80d2ac217`, `a46095a08`/`4dab86746`).
- **The re-answer / anchoring family (§4)** -- ranked as one item family in the ledger with a single
  consolidating direction (all consumers anchor LAST; name/index disagreement is a rejection).
- **The i5 absent-cost residual (N-152k / N-139w / N-158v)** -- one defect, three seats, 194
  measured false renders, **zero cost so far and reported before it bites.** The discriminator is
  in the DATA and needs no code reading: Moxen carry `mana={0}` in the primitives; Day, Night,
  Moonrage Brute and Tovolar's Packleader carry **no `mana=` line at all.** The fixture must ride
  the ABSENT population (HL5.4), because the existing one rides Moxen -- the branch that works.
- **N-158s (Foray of Orcs renders `damage N` with zero legal targets)** -- the pilot cited the
  annotation BY NAME as its authority for face damage and planted a false term in a lethal
  computation. It did not cost that game. **It is ranked HIGH because of what it threatens:** three
  waves of magnitude work have taught the pilot to trust the annotation ABOVE the card text on the
  same line, which is the behaviour we asked for, and which makes a wrong annotation strictly worse
  than an absent one. That is method headline 7.
- **N-139v (creature TYPE is a declared primitive field the render never emits)** -- the guide-layer
  repair shipped this wave and is untested; the render escalation ladder is specified (print the
  type on creature lines; then, if needed, the aggregate). **The interim that beat everything at
  this seat costs nothing: make the ENGINE'S OWN OFFER the evidence.**
- **N-146n (the dungeon room menu never states the current room)** -- the largest latency driver in
  the corpus, three 200s+ specimens, and the falsifiable prediction is written: ship the position
  and the per-option destination and the spiral class goes to zero.
- **N-116a / N-116d** -- counted per the brief, not re-diagnosed. **9 reveals, 439 options for 40
  eligible = 11.0:1.** N-116d is **0 occurrences and UNEXERCISED, not fixed**: the only un-tokened
  effect in deck116 is Farseek, offered 36 times and cast **zero** times because the guide demoted
  it. **A guide teach silently retired an engine docket item** -- that is method headline 10, and
  deck116 must not be the witness for the subset-framing fix while this guide is deployed.
- **N-146o (Nadaar's anthem buffs Nadaar)**, **N-146p (Soul Shatter never takes a planeswalker)**,
  **N-139q (Nethroi's total-power budget)** -- card-script divergences. Per the standing rule the
  guide stays ENGINE-TRUE and the deviation routes here.
- **deck105's damage-assignment ORDER cost observation** -- the new LLM order seam (`d083a0629`) is
  EXERCISED, correct, and expensive: one blocked attacker produced THREE sequential position asks
  costing **140s and 11,915 chars** for an outcome worth one creature. Bundling candidate (one
  reply gives the whole order), mirroring the existing one-reply attacker and blocker declarations.
- **N-105g / L2 (instance handles are a positional RANK, re-indexed on death, dropped on zone
  moves)** -- the append-only log contradicts the board snapshot, and the pilot resolved it by
  **distrusting the LOG**, which is the wrong half. Same shape as wave-32's *"then my count is
  wrong"*, relocated to permanent identity.
- **The shockland tax** -- deck139 answered `pay 2 life` at **20 of 20** windows = **40 life over
  six games**, 19/20 the wave before, and **neither wave's review measured it**, because every
  existing rung looks at casts, attacks, blocks, mulligans and fallbacks. It cost 8 life in a game
  lost by 3 and was the ONLY life the seat lost in the game it lost at 14. Now a standing
  measurement (HL9) and a guide-authoring rung.

---

## 11. METHOD-HEADLINE MAP (what the skill gained, and from whom)

| HL | rung | seats |
|---|---|---|
| 1 | the RE-RUN leg of the alt-win doctrine: a re-run seat leads with the BEHAVIOUR DELTA at the fixed seam, and the win table is its consequence | 105 |
| 2 | a threshold rule names its number by the render substring that precedes it; a bare variable letter is a defect at the severity of a false card fact; every example is re-derived from the rule | 105 |
| 3 | consolidated TEACH-RETIREMENT doctrine: name the carrying surface before deleting (a zero count is the QUESTION); a stopgap's removal condition is an executable grep; falsity facts are versioned dependencies; a teach whose subject was a BUG is FALSE not stale, and its general half survives and is re-aimed | 139, 105, 152, 146, 36 |
| 4 | a COUNT retires one AXIS, not the mechanism -- enumerate every property a guide CONDITION or an engine legality predicate names, and rank the remedies: offer-as-evidence > count > tag > sentence | 139 |
| 5 | a KEEP CRITERION is a DISQUALIFIER -- count how often it FIRES against the decklist, prefer a criterion the prompt PRINTS as a positive, and recognise COVERAGE denial as distinct from membership denial | 152, 105 |
| 6 | BELIEF MISS vs COMMIT MISS: read the text after the committed answer before scoring a taught class MISSED, or you will rewrite a rule that is working | 116 |
| 7 | ANNOTATION TRUTH: an emitted magnitude asserts the effect will happen; the pilot now trusts the annotation above the card text, so a confident number on an impossible effect is worse than no annotation | 158 |
| 8 | PROXY-GATED FIXES must be validated at BOTH settings of whatever the proxy diverges on -- both parities, both seats, both phases; name the proxy in the validation metric | 158 |
| 9 | statistics discipline: never report a clock conditioned on WINS; decompose a strong record against each opponent's corpus-wide record and opener quality; report a trigger-predicate count with its denominator's cause; measure self-inflicted life | 158, 139 |
| 10 | a teach that SUPPRESSES a card makes that card's defects untestable -- name the disqualified witness seat; and a KNOWN-OPEN item owned by your seat gets a MEASUREMENT, not a citation | 116, 158 |
| 11 | instruments measure a BOUNDARY: state which boundary the field has and which the phenomenon has, report the field as a LOWER BOUND where they differ, and add the absence table's OFFERED / TAKEN / ARRIVED split | 36, 116, 105, 146 |
| 12 | a null on a CARD-PROPERTY trigger is not final until the POOL's decklists are grepped; report corpus count AND pool count; a probe is warranted only at pool count 0; and a third consecutive UNEXERCISED verdict retires the item to a probe or closes it | 36, 158 |

**Adoption honesty.** 31 seat proposals adjudicated: **25 ADOPTED, 6 ADAPTED, 0 REJECTED.** Nothing
was rejected because every proposal arrived with a witness -- but six were not adopted as their
reviewers wrote them:

1. **deck146 P2** (quoted render strings are a dependency) -- **ADAPTED**: it substantially
   restates HL3 rung 1, which already exists. What was NEW and is adopted is the reporting form
   (*answer each grep with a FRACTION, not a sample*) and the verdict wording (*zero occurrences =
   retired, not reworded*). Merged into the consolidated HL3 rather than added beside it.
2. **deck36 P1** and **deck152 P3** -- **ADAPTED, merged into the same consolidated HL3.** All three
   of P1, P3 and deck105 P1 describe one shape from three angles (what the teach said the prompt
   LOOKS like; a teach whose subject was a BUG; a stopgap whose falsity fact inverted). Shipping
   them as three rungs would have three reviewers running three greps for one question.
3. **deck158 S3** (a seat at 5/6 or better publishes the opponent tables) -- **ADAPTED: the trigger
   is widened.** deck139's 0/6 needed the identical tables and its own review said so
   (*"any seat reading its own delta this wave is reading a pool that moved under it"*). The rung
   now fires on any record MOVE of >=2 games in either direction, not on a high record.
4. **deck152 P1 clause 2** (a colour criterion cannot be carried by a two-colour deck whose duals
   are a minority) -- **ADAPTED, scoped against deck158's wave-32 colour rung**, which says the
   opposite for a deck with ZERO relevant sources. Reconciled: the distinguishing question is
   *"can the deck cast anything at all off one colour?"* -- if yes, the colour clause is a
   TIEBREAKER and never a gate; if no, deck158's rung stands.
5. **deck139 P4 rung 1** (self-inflicted-life measurement) -- **ADAPTED in scope**: mandatory for
   any seat whose deck has a repeated pay-or-decline seam (shocklands, painlands, fetches,
   Phyrexian costs, "pay N to do X now"), not for every seat.
6. **deck105 P6** (define the commit counter on the ANSWER LABEL) -- **ADAPTED**: adopted as
   instrument doctrine inside HL11 and, more importantly, promoted out of the skill into the
   **wave-34 step-1 measurement spec** (§5c), where it is a shipped field rather than a reviewer
   procedure.

**Two declines recorded by their own seats, so a later wave does not read the omission as an
oversight**: deck105 DECLINED writing an attack-side "hold a blocker back" rule (n=1, and it would
sit directly in front of the seat's strongest preserved behaviour -- 19/19 attacker declarations;
the block-gate rewrite reaches the same game one decision earlier); deck152 DECLINED the
front-loaded blocker-eligibility render and the attacker-decline wording corner (one witness each,
guide-side clause is cheaper and is deployed). deck36 declined a sacrifice-latency rung and an
opponent-seat-latency rung as deck findings rather than method findings. All four are candidates on
a second witness.
