# Wave-33 consolidated engine ledger -- the wave-34 step-1 docket

Every item below comes from a seat's `notes.md` (or a seat's `general-suggestions.md` where the
owner directives route it) with the seat's own repro. Corpus
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260819-070739`, binary `bdb30d8e7`, -T 3000,
REPPENALTY=1.05, -j 3, unless stated. Ranked by (game-cost observed) x (mechanism located) x
(repro quality). **Severity and repro quality remain SEPARATE fields** (wave-31 HL4).

Five cross-cutting directives for the fix agents, before the list:

- **#1 IS AN OWNER RULING, NOT AN ADJUDICABLE ITEM.** The post-choice reasoning block is
  eliminated; native request-level reasoning replaces it. Seats supplied evidence only and every
  seat complied. What is DESIGNED here is the implementation and its measurement. **The
  instrumentation half is not optional and not deferrable: without `post_answer_overrun`,
  `answer_replaced` and `reasoning_content` capture, the wave-34 A/B measures a boundary the
  phenomenon does not have and blinds every seat review.**
- **THE SHIPPED COMMIT FIELDS MEASURE THE WRONG BOUNDARY. Do not read their zeros as absence.**
  `post_plan_overrun` is post-PLAN; the phenomenon is post-ANSWER (deck116 measured the field at
  **5.3%** of the real quantity). `commit_retracted` is fallback-scoped
  (`AIPlayerGPT.cpp:2259`) and is a counter of retractions that reached the HEURISTIC. Corpus-wide
  they report **0/1,209** while **45/1,209** replies ended on a different answer label than they
  began with, one of them the deciding play of a win and one of them a fatal reversal.
- **A FIX GATED ON A PROXY MUST BE VALIDATED AT BOTH SETTINGS OF WHAT THE PROXY DIVERGES ON**
  (wave-33 HL8). #4 (N-158r) exists because i9's `observer->turn == 0` is a proxy for "pregame" that
  is correct on the draw and wrong on the play. A reviewer sampling mulligan prompts -- or sitting
  on the draw in every game -- reports a clean PASS.
- **AN ABSENT FIELD IS NOT A ZERO FIELD** (wave-33 HL11, and it is i5's own residual). `if (value)
  print(value)` deletes real zeros; `print(value ?? 0)` invents zeros where the field is absent.
  Both are one-line bugs and both ship a confident wrong card fact that no audit rung can see.
  **Enumerate the populations that reach the emitter with the field ABSENT and write the fixture on
  the ABSENT population** -- the existing i5 fixture rides Moxen, the branch that works.
- **REPORT EVERY ANNOTATION FIX AS A COVERAGE FRACTION** (offers-with-annotation / offers-total,
  per card, on the next corpus), never as "it renders". This wave that discipline turned i6's
  headline from "Foray renders" into **3/52 -> 59/59 of non-trigger-gated offers**, and turned i3's
  into **falsifier passes / positive half unexercised**.

---

## Tier 1 -- the owner ruling and the game-costing engine defects

| # | id | sev | one-line mechanism | file:line | repro | validated next by |
|---|---|---|---|---|---|---|
| **1a** | **REPLY-PROTOCOL RULING** | **P0, OWNER-BINDING** | The current protocol answers first, THEN invites scratch reasoning ("you may think the decision through briefly... that scratch text is discarded"), THEN puts `PLAN:` on the LAST line -- a composition that teaches reasoning-follows-output. **Target: ANSWER + a concise, changed-only PLAN. Nothing else invited.** **The dev loop KEEPS reasoning -- it is what makes the model's experience auditable -- but it moves to the model's NATIVE window (`enable_thinking` / request-level reasoning), where (a) it precedes and benefits the choice, (b) it uses the reasoning the model was actually TRAINED to do rather than an improvised scratch register, and (c) it still yields auditable content.** Dev-loop/corpus runs = thinking ON + captured; shipped user default = thinking OFF, terse replies | the reply-protocol block in the system prompt; the request assembly for the native reasoning flags (plumbing already exists); **the sampling block, which must switch WITH the mode** -- Qwen thinking wants **temp 0.6 / top_p 0.95** against the non-thinking defaults (0.7 / 0.8 / top_k 20 / presence_penalty 1.5). Shipping thinking under non-thinking sampling is a confound the A/B cannot separate from the protocol change; `repetition_penalty=1.05` is orthogonal and stays on both arms | Corpus-wide post-answer text **~1,800-2,035 chars/decision** at four independently-measuring seats (deck158 **98.0% of ALL reply text**, 280,919 of 286,554 chars in six games; deck116 263,247; deck105 221,905 seat-side / 394,864 both seats; deck139 275 KB). Latency: deck158's two worst replies are a **three-option LAND DROP at 214.5s / 14,570 chars** and a target pick at 192.1s -- **both answered correctly on line 1** | **A/B corpus at wave 34.** No seat proposed a reordering, a brevity clause or a terminator; deck158 WITHDREW its own wave-32 terminator flag. **The 2-seat fatal-retraction promotion trigger is 0/7 and moot** |
| **1b** | **REASONING CAPTURE + PROTOCOL INSTRUMENTATION** (deck36 C2 + deck116 + deck105 P6 + deck139 N-139x) | **P0, SHIPS WITH 1a -- the A/B cannot be scored without it, and seat review goes blind without it** | **(A) CAPTURE, robust to the server's parser state.** Spark's vLLM may or may not run a reasoning parser for the served model, and the two cases put the reasoning in different places, so **the client implements BOTH paths unconditionally**: if `message.reasoning_content` is present, log it verbatim; otherwise **strip a leading `<think> ... </think>` block out of `content`** into the same translog field and parse only what remains. Two hardening requirements the corpus WILL exercise: an **UNCLOSED `<think>`** (truncation, or a model that opens and never closes) must not swallow the answer -- with no `</think>`, treat the reply as reasoning-only and re-ask rather than parse think text as an answer; and **every length/answer scanner must run on the STRIPPED text** on the same normalisation, or `post_plan_overrun` / `post_answer_overrun` / `hasCodedAnswerLine` silently measure thinking tokens instead of reply tokens. **(B) Three new fields, no behaviour change**: **`post_answer_overrun`** (chars after the end of the FIRST line-leading coded answer -- `CHOICE:`/`ATTACK:`/`BLOCKS:`/`PUT:`; reuses `hasCodedAnswerLine`'s scanner); **`answer_replaced`** (bool) + **`coded_answers`** (int); **`dropped_assignments`** (int, blocker records only -- `B:A` pairs the reconciler discarded as illegal or duplicate) | `postPlanOverrun` ~`AIPlayerGPT.cpp:2210-2245` (leave it alone -- first-occurrence is correct FOR A TAIL METER); `commitRetracted` **`AIPlayerGPT.cpp:2259`** (document as fallback-scoped, do not widen); `pruned_pairs` -- **emitted on exactly ONE record corpus-wide; five seats report it never written. Check it is wired on the blocker path at all** | The gap, measured: deck116 post-ANSWER **263,247** chars vs post-PLAN **14,025** = the field sees **5.3%**, and it is format-sensitive (scores **262** on a 14,206-char post-answer reply writing `Plan:` lowercase-last; **0** on a 13,326-char reply with no PLAN line -- both legal under `f46dd58ee`). deck36: **8/201 (4.0%)** re-answers, ALL scoring `post_plan_overrun` 0, **one of them the deciding play of a win** (vs146 s18: `CHOICE: 2 (Yotian Soldier)` -> 4,600 chars -> `CHOICE: 1 (Akroma's Memorial)`, resolved turn 9, won turn 13). deck139: 6/178, **4 with overrun 0**. deck105: 7/240, **`commit_retracted` 0/240** | **Gate the A/B on `reasoning_content` being present and non-empty at BOTH seats before the corpus runs** (deck152's condition, adopted). Validation: `answer_replaced` goes to zero by construction (that is the change landing, NOT an improvement); `post_answer_overrun` distribution is the primary length metric |
| **1c** | **THE A/B'S REQUIRED METRICS** | **SPEC, not a defect** | **The A/B must show NET latency AND NET decision quality -- neither alone.** Thinking tokens are still decode tokens (~30 tok/s on this stack), so removing ~2,000 chars of post-answer scratch and adding an unbounded thinking block can come out latency-neutral or worse. **Primaries: (1) TOTAL DECODE TIME per decision, split into thinking tokens vs reply tokens; (2) FIRST-LABEL-vs-LAST-LABEL divergence (`answer_replaced`) -- which goes to zero by construction, so its disappearance is the change LANDING, not an improvement; (3) each seat's own falsifiable predictions re-measured; (4) the §5a value ledger re-run -- the ~35 improvements the post-answer block was delivering must still happen, on the FIRST line, or the change bought latency with decision quality.** Alongside, per seat: (a) `post_answer_overrun`, never `post_plan_overrun`, measured on the STRIPPED reply; (b) **ILLEGAL-ASSIGNMENT RATE as first-class** (`all_assignments_illegal` + `dropped_assignments` + the deck146-s29 shape) -- the one axis where the discarded text is doing SAFETY work; (c) latency p95 + >100s decision count + p95 reply chars; (d) `YOUR PLAN` block length distribution; (e) **seat review reads `reasoning_content` in place of scratch text -- say so in the brief**, or every rung that names a belief in the pilot's own words scores off the decision alone | wave-33 baselines to beat: per-seat latency median **15.8-18.1s**, p90 **53-82s**, p95 **130-186s**; 21 games, 0 timeouts, **4 cap adjudications**; post-answer text **~1,800-2,035 chars/decision** | deck152: the post-answer block **caught one illegal block and missed one (1 of 2)** -- `B1:A1, B1:A4` self-repaired at deck146 s29, and `B1:A2` onto a flier NOT self-repaired at deck36 s8 (the corpus's only fallback). Value ledger consolidated in synthesis §5a: **improved ~35, neutral ~6, HARMFUL 3** (deck105 x2 incl. one fatal, deck158 x1) | **STRATIFICATION IS MANDATORY (deck146 (d)):** the three worst spirals in the corpus are re-derivations of state the prompt does not state (**#5, N-146n**). If the A/B moves reasoning native without closing that ambiguity the reasoning spirals in the native channel and the latency result is confounded -- **ship N-146n in the same batch OR record room-choice asks as a separate stratum.** Baseline note for the comparison: **the engine binds the LAST label today** (verified at 4 seats) |
| **2** | **N-116g** | **HIGH, GAME-DECIDING, REGRESSION** | **A cheat-into-play activation is offered, chosen, narrated and PAID FOR, and then never reaches the stack.** `MTGAbility::reactToClick`/`addToGame` succeeds through payment and fails to push the `StackAbility`. **3 of 11 taken activations (27%) silently no-op**; wave-32 arrival-traced this exact class **5/5 clean** on `7fabd9bd0` | stderr signature is exact: a `- Target: <card>` / `- Cost: <cost>` / payment line **NOT followed by** `Action added to stack: StackAbility.  (Source: <same card>)` -- the failures read `Action added to stack: NextGamePhase.  (Current phase is: Upkeep)` instead. Bisect candidates: `e4b3cc96e` (ORDER-step click gate), `cd2983c2a` / `80d2ac217` (activation offer / payment-candidate changes), `a46095a08` / `4dab86746` (cancelled-attacker guards) | **Arrival-traced from BOTH seats.** deck116 vs105 **s11** (my T10 Upkeep, `{4}, Tap`, target Emrakul): s12 shows Emrakul still in hand, `Your battlefield (creatures: 0)`, the Amulet **not** `[tapped]`, mana **not** spent; **s15** the same activation re-taken, same non-arrival; the game ends on the next record -- **loss at 23 life holding a 15/15 annihilator-6 with the enabler online.** Independent cross-check: deck105's whole event stream never contains `Opponent's Emrakul, the Aeons Torn: hand -> battlefield`, while the Amulet's own arrival IS there (the stream is not lossy). Third instance deck116 vs36 s22 (`{g}, Tap`, Pelakka Wurm) -- re-taken at s23 and that one ARRIVED. **CORRELATE, stated as a correlate: all three are Upkeep priority windows with a `NextGamePhase` pending; opponent's-turn upkeep 0/2 arrived, own-turn upkeep 5/6, main phase 8/8. Mana headroom is NOT the discriminator** (a success at `Mana available: 4` / `{4}` and a failure at the same 4/`{4}` both exist) | **deck116** (holds). **FIXTURE MUST RIDE THE BROKEN BRANCH (HL5.4):** an Upkeep-phase priority activation of `{4}{T}:moveTo(myBattlefield) target(creature|myhand)` with a phase advance pending, on **BOTH players' turns**. A main-phase fixture passes today and proves nothing. **Validation: `Put in Play` take-count == arrival-count, both parities, and the grep signature above returns zero orphaned payments corpus-wide** |
| **3** | **THE ANSWER-SHAPED-THING FAMILY: N-146m + N-36e + N-139x + N-139y** | **HIGH (N-146m decision-fatal), one family, one consolidating direction** | **Four consumers of a reply disagree about which answer-shaped object is authoritative.** `parseChoice` anchors **LAST** (correct, and decision-critical). **N-146m**: the reconciler resolves a `CHOICE: N (name)` by **INDEX** even when the NAME anchors to no option -- so an answer to the PREVIOUS question commits a real choice. **N-36e**: `consumePlan` anchors on the **FIRST** `PLAN:`, so an ABANDONED plan is carried into the next prompt and the committed one is discarded. **N-139x**: `commitRetracted` cannot see a re-commit the parser ACCEPTS. **N-139y**: a partial illegal-assignment drop is invisible to both fallback classes | N-146m: the `CHOICE: N (name)` reconciler (index-first resolution) + the plan-staleness detector, which did NOT fire on the offending record though it fired on the two before it. N-36e: `consumePlan` -- **the fix is already proven in this file**, `parseChoice` uses last-line-leading-label and is right; `postPlanOverrun` (~`:2219`) must KEEP first-occurrence, only the CONSUMER moves | **N-146m, deterministic:** deck146 vs116 **seq34**, kind `ask`, 11.6s, `choice=1`, **no fallback**. Options are three DUNGEONS; the reply's first line is `CHOICE: 1 (Nadaar, Selfless Paladin #1 (5/5) [vigilance] [your battlefield])` -- seq33's answer, carried in as `YOUR PLAN`. `chosen_text` = `Tomb of Annihilation`, against the guide's #1 dungeon teach. **Downstream cost: the Tomb room chain, a 214s spiral, a 228s spiral, Acererak discarded to a Tomb room, and the game hitting the cap.** Rate 1/189 (the detector must exempt the 27 `CHOICE: 0 (pass)` hits on option-less priority asks). **N-36e:** vs105 s35 has two `PLAN:` -- char **501** (abandoned) and char **5,359** (committed); the s36 prompt's plan field is **1,424 chars beginning at char 501** and running through the retracted deliberation. **107/201 prompts (53%) carry a >400-char plan field** (wave-32: 39%; max 1,541). **N-139y:** vs105 s24 -- `BLOCKS: Illuna blocks Cystbearer, Illuna blocks Hand of the Praetors, Illuna blocks Ichorclaw Myr #1, Illuna blocks Ichorclaw Myr #2` against a prompt line two rows above that forbids it; first-wins parser correct, **three assignments dropped silently, 7 poison connected, 3+7 = 10, game over at 14 life** | **deck146 + deck36 + deck139.** **Direction: every consumer of a reply anchors LAST, uniformly; a name/index disagreement is a REJECTION (`stale_echo` class: re-ask or fall back and LOG it), never a coincidence; cap the extracted plan length.** Note the owner ruling reduces the POPULATION of multi-answer replies but fixes none of the anchoring rules -- N-146m fires on a stale echo, not on a spiral. Validation: zero committed choices whose parenthetical name anchors to no option; the carried plan equals the LAST plan line; `dropped_assignments` populated |
| **4** | **N-158r** | **HIGH (cost), i9 DEFECT, PROXY-GATED** | **The pregame hand-only serializer latches through the ON-THE-PLAY player's first land drop.** `bool pregame = (observer && observer->turn == 0);` gates on the TURN COUNTER, not on the ask. The player on the play takes their first main phase while `turn` is still 0, so an ordinary in-game land drop is served `serializePregameState()` -- **no battlefield, no opponent board, no `Mana available:` line** -- and carrying a HAND-WIDE satisfiability header that is true across the game and false for this turn. `planActionsStale` is suppressed on the same predicate, so that seat also loses its stale-plan notice | **`AIPlayerGPT.cpp:2544`** (the predicate), **`:2556-2557`** (the emit), **`:2582`** (staleness suppression). Pregame entry points that already identify themselves: `pregameMulliganDecision` **`:8466`**, `pregameChooseBottom` **`:8493`**, `pregameLeyline` **`:8568`** | **3/3 on-the-play games, 0/3 on the draw**, deterministic and source-read. Worst specimen deck158 vs116 seq2: **214.5s / 14,570 chars on a THREE-OPTION LAND DROP**, of which **6,591 chars land after its own PLAN line**. Verbatim: *"If I play a Swamp, I have 1 mana. I can't cast Bowmasters ({1}{B}) either. Wait, {1}{B} is 2 mana. Is it possible the prompt is flawed...? ... This is a contradiction."* | **deck158** (holds). **Fix: gate on the ASK, not the clock** -- a flag set on entry to the three pregame entry points and cleared on exit; second choice `turn == 0 && the ask is one of the pregame kinds`. **Validation metric: zero `--- YOUR OPENING HAND ---` prompts on any ask whose option list is a land drop or a cast, measured at BOTH SEATS of every game** -- the on-the-play seat is the only one that exercises it, so a single-seat check reports a false PASS half the time |
| **5** | **N-146n** | **HIGH, RENDER -- the largest latency driver in the corpus** | **The dungeon room-branch menu never states which room you are ON.** It renders two room names plus their `{room effect}` text and nothing else -- not the dungeon, not the current position, not which room each option advances TO, not whether the two are a branch or a sequence. The venture state is engine-side and knowable. The model then reconstructs its position from the narration's explore counters and never reaches certainty | the venture room-choice ask emitter. Both facts are already in the engine | **Three 200s+ specimens, all at deck146.** vs116 **seq35, 214.1s, overrun 9,091**: options are exactly `1. veils of fear {room effect: ...}` / `2. oubliette {room effect: ...}` -> *"the options are weirdly skipping 'Sandfall Cell'. Or perhaps 'Oubliette' is the name of the branch? [...] Perhaps 'Oubliette' is a typo in the prompt's options list"*. vs152 **seq45, 213.8s, overrun 12,155**: the reply HAND-COUNTS explore counters turn by turn to locate itself. vs36 **seq22, 201.0s, overrun 15,163** -- the largest overrun in the corpus. **Related, same ask family (LOWER):** the `veils of fear` DISCARD ask (vs116 seq37, **228.3s**) never says whose discard it is or which effect is asking (*"is this MY discard or theirs?"*) | **deck146** (holds). Fix: two facts on the ask -- `Tomb of Annihilation - you are entering room 2 of 5` and per option `1. veils of fear (room 2 of 5) {room effect: ...}`; plus one line naming the source and reason on the discard ask. **Falsifiable prediction, the seat's own: the 200s+ room-choice spiral class goes to ZERO and the seat's nonzero `post_plan_overrun` rate (22/227) falls by ~26k of its 68.5k overrun chars.** ⚠ **#1c makes this a same-batch item: without it the wave-34 latency A/B is confounded** |

## Tier 2 -- correctness and representation, mechanism located

| # | id | sev | one-line mechanism | file:line | repro | validated next by |
|---|---|---|---|---|---|---|
| **6** | **N-158s** | **HIGH (correctness)** | **A magnitude annotation is emitted for an effect that CANNOT OCCUR.** Foray of Orcs renders `{right now: Army 6/6 -> 8/8, damage 8}` against an opponent board with **zero creatures**; `dynamicMagnitudes` computes the magnitude without consulting the target set. **A defect on i6's OWN new surface** | `dynamicMagnitudes` (the damage-clause emitter) -- gate a targeted rider's magnitude on the rider having a legal target | deck158 vs116 **seq12 t8**, `Opponent battlefield (creatures: 0)`. Reply, verbatim: *"Since there are no opponent creatures to target, the spell deals 8 damage directly to the opponent (**as per the 'right now' text: Army 6/6 -> 8/8, damage 8**). This reduces the opponent's life to 1."* The `{card text: ...}` **on the same line** says *"target creature an opponent controls"* (Scryfall-confirmed); seq13 narration: two counters, **no damage event, opponent life unchanged at 9**. No harm THIS game (it swung for 9 into an empty board and won) | **deck158** (holds). **Severity is about what it threatens, not what it cost:** three waves of magnitude work have taught the pilot to prefer the annotation to the card text -- which is the behaviour we asked for -- so a wrong annotation is strictly worse than an absent one. Fix: suppress (`{right now: Army 6/6 -> 8/8}`) or qualify inline. **Generalises past Foray: any magnitude on a targeted rider must be gated on a legal target existing. Validation: zero `damage N` clauses on an offer whose targeted rider has an empty legal-target set** |
| **7** | **N-152k / N-139w / N-158v** (i5 residual, one item) | **MEDIUM, 194 measured false renders, cost ZERO so far** | **`{0}` is printed for a cost that is ABSENT, not zero.** i5 correctly inverted `if (cost && cost->getConvertedCost())`, but the new guard cannot tell "cost object exists and is 0" from "no cost object at all". Transformed DFC back faces and day/night DESIGNATIONS (which are not cards, CR 730) now print `{0}` | `manaCostToken()` -- return the empty string when the card has **no mana cost object**, `{0}` only when a cost object exists whose converted cost is 0. Second half (deck158): the battlefield serializer should exclude non-permanent designations from both battlefield lines, or render day/night ONCE as a game-state line | **Attribution verified against the wave-32 corpus: all four shapes have ZERO occurrences there** (`Day`/`Night` rendered bare, x81). Wave-33: deck152 **`Day {0}`/`Night {0}` 142, `Moonrage Brute {0}` 11, `Tovolar's Packleader {0}` 13 = 166 false against 222 CORRECT `Mox Jet {0}`/`Mox Sapphire {0}`**; deck158 20 (12 of them `Night {0}` **on its OWN battlefield line**); deck139 8. **0 replies reason about any of them** -- reported before it bites | **deck152 + deck158.** **FREE DISCRIMINATOR, no code reading required: the two populations are separable in the DATA** -- Moxen carry `mana={0}` in the primitives; Moonrage Brute, Tovolar's Packleader, Day and Night carry **no `mana=` line at all**. **FIXTURE (HL5.4): POSITIVE, currently failing** = a board with a transformed `Moonrage Brute` and a `Night` designation, PARSETEST pins that neither line contains `{0}`; **NEGATIVE GUARD, must stay green** = `Mox Jet` in hand and on the battlefield, both lines still `{0}`. The existing i5 fixture rides Moxen -- the branch that works. ⚠ It also makes deck36's shipped falsity fact (*"a card with no cost printed costs {0}"*) TRUE for Moxen and FALSE for back faces **in the same prompt** |
| **8** | **N-152g** | **MEDIUM, DIAGNOSED, FIX STAGED -- SHIP ON THE CODE READING** | `LegalActions.cpp:66` and `:256` suppress casting a legend while a same-name copy is in play. **RULES-WRONG**: CR 704.5j is a state-based action and `MTGNewLegend` implements it correctly. **Fix = delete both lines** | **`LegalActions.cpp:66`, `LegalActions.cpp:256`** | **0 suppressed windows witnessed this corpus, at three seats that checked.** deck152 came within one decision (deck36 held **two** Teferi and resolved one at s26 -- the game's LAST casting window before the cap); deck116's legends are all 1-ofs; deck36 runs one Tolarian Academy and one Academy Ruins. **Neither confirmed nor falsified -- not sampled** | **Ship it; do not wait for a corpus.** The code reading is unambiguous and two waves of waiting have produced no witness. Fixture: a legend in hand with a same-name legend on the battlefield -> the cast line is OFFERED, the cast resolves, and the SBA puts one in the graveyard |
| **9** | **N-139v** | **HIGH (guide-layer repair shipped, UNTESTED; render escalation specified)** | **Creature TYPE is a declared primitive field (`subtype=`) the render emits ZERO times in 190 records.** The hand/board line reads `Pollywog Symbiote {1}{u} (1/3) [creature]` -- type-class only. The pilot filled the gap by INVENTION four different ways and the confabulation **overrode the engine's own legality decision** | the hand/board creature-line emitter. Ladder: (2) `Pollywog Symbiote {1}{u} (1/3) [creature: Frog]` -- prefer the general TYPE field over a mutate-specific token; (3) if that fails, the aggregate `Your battlefield (creatures: 4, non-Human: 4)` | **5 replies in 3 games**, e.g. *"my only creature, Pollywog Symbiote, is a Human"*, *"Arboreal Grazer (Human) and Pollywog Symbiote (Human)"*. Ground truth: Frog / Beast / Nymph / Phoenix / Nightmare / Dinosaur / Cat / Elemental -- **no Human in the deck** -- and every mutate card carries `otherrestriction=type(creature[-human]|mybattlefield)~morethan~0`, **so the option list the model is reading is ITSELF proof a legal host exists.** Self-refuting inside one game: vs158 s23 names them AS the non-Human creatures, s24 types both as Human. **Cost: mutate take-rate 6/7 (86%) -> 1/6 (17%), the deck's #1 rule off in five of six games. ZERO instances in the wave-32 corpus** | **deck139** (holds). **The interim beat everything and costs nothing: make the ENGINE'S OWN OFFER the evidence** -- *"the engine prints 'with its mutate cost' ONLY when a legal non-Human host is already on your battlefield; if the line is there, take it"* (shipped this wave). **Falsifiable prediction, guide layer only: zero replies asserting a deck139 creature is Human; mutate take-rate >= 5/6 of offered windows. If it fails, the guide layer has failed twice on this belief and the render escalation is mandatory** |
| **10** | **N-105g / L2** | **MEDIUM-HIGH, RENDER** | **Instance suffixes are a POSITIONAL RANK computed at render time, so they re-index on death and are dropped on zone moves.** `instanceHandle()` walks the controller's battlefield and takes the positional rank among same-named cards, returning `""` when `total < 2` or when the card is not on the battlefield. Three consequences, all observed: a dying permanent loses its suffix entirely; survivors are RENUMBERED while earlier log lines are never rewritten; a permanent gains and loses its suffix as copies come and go | **`AIPlayerGPT.cpp:810-834`** | **PERFECT, with a verbatim self-diagnosis.** deck139 v105 s18 (**11,089 chars / 115.5s**): the log says `- Counter added to Pollywog Symbiote #2: -1/-1` then `- Opponent's Pollywog Symbiote: battlefield -> graveyard` (no handle), while the board snapshot in the SAME prompt reads `Pollywog Symbiote #1 ... [counters: 1x -1/-1]`. Reply: *"This contradicts the game log where they died. ... Ah, the 'CURRENT SITUATION' block is the source of truth. ... Maybe the log description of 'graveyard' was a simulation error or I misread?"* -- **it resolved by distrusting the LOG**, the same shape as wave-32's *"then my count is wrong"* relocated to permanent identity | **deck105.** Fix: the handle must be a stable IDENTITY, not a live rank -- assign a per-controller per-name monotonic ordinal on battlefield entry, store it on the instance, render it everywhere INCLUDING zone moves, never re-derive from position, never suppress once assigned. **Emitting the handle on the zone-move line is the single highest-value half.** Validation: no `#N` that names one physical permanent earlier and a different one later in the same log; every `battlefield -> graveyard` for an ever-duplicated name carries a handle |
| **11** | **N-105f / N-158u** | **MEDIUM, RENDER, two seats, one defect** | **Batched counter narration prints the POST-BATCH P/T on every line.** The `WEventCounters` renderer prints the target's CURRENT power/toughness; the in-source comment justifies that correctly for a SINGLE counter, but N counters from one damage event are narrated after the whole batch is applied, so all N lines print the same final pair | **`AIPlayerGPT.cpp:3051-3059`**; capture P/T INTO the event at fire time (`Counters::addCounter`) and print the captured pair -- **or suppress the per-counter run entirely when the aggregate `Infect damage:` line already covers the event** (it states the aggregate correctly, so the run is redundant AND wrong) | deck105: `- Counter added to Arboreal Grazer #1: -1/-1 (now -3/0)` **x3** for a 0/3 taking three counters (correct: `-1/2`, `-2/1`, `-3/0`); same at Kitchen Finks and Pollywog Symbiote. deck158: `Orcish Bowmasters: -1/-1 (now -2/-2)` **x3** for a 1/1, across 18 distinct prompts. **The `+1/+1` path is the CONTROL and is CORRECT** (`Ugluk: +1/+1 (now 4/4)` then `(now 5/5)`; Orc army running totals correct across 21 lines / 94 renders) -- so the defect is specific to the batched infect `-1/-1` application | **deck105 + deck158.** Contributes one of the two ingredients of deck139's 11,089-char board-state spiral, and makes the append-only log un-replayable (intermediate toughness matters for first-strike and multi-blocker orderings). Validation: in any window where a creature receives N>1 counters from one source the N `(now X/Y)` values are strictly monotone -- grep for two consecutive identical `Counter added to <same name>: <same delta> (now <same X/Y>)` and expect zero |
| **12** | **N-36j** | **MEDIUM -- contributes to the corpus's worst latency sink** | **The sequential-sacrifice header says "of exactly 6" without ever saying six WHAT.** Annihilator resolves as N sequential one-card asks; the "6" is the number of SACRIFICES the trigger demands and the list below it holds 9 or 11 permanents. The pilot reads it as a list length and concludes the list is broken | the SACRIFICE ask header. Fix is string-only: `This is sacrifice 4 of 6 that this effect demands. Choose ONE card from the list below; you will be asked again for the remaining 2.` (optionally `After all 6, 5 of your 11 permanents remain.`) | **16 sacrifice windows, all in deck36 vs116.** Verbatim s38 (129s / 8,793 chars): *"The prompt says 'Choose card 1 of exactly 6'. This is confusing. Maybe the list is truncated or I should just pick from the 9."*; same at s30 (137s / 9,127). **Cost: the six worst decisions of that game total 831s = 44% of this seat's 1,910s budget, in a game the cap adjudicated at turn 13 with this seat consuming 74% of the wall clock** -- and four of the six are "which of my tapped lands do I throw away", all answered correctly | **deck36** (holds). A stronger fix (ask ONCE for an ordered set of 6) changes the answer protocol and is **not** proposed. **Falsifiable prediction, the seat's own: no reply at a `SACRIFICE ONE OF YOUR OWN CARDS` window exceeds 60s or 3,000 chars and none asks what "of exactly N" means** (baseline: 16 windows, 4 replies over 90s, 2 asking) |
| **13** | **PHASE-HEADER REPRESENTATION** (deck36 C1) | **MEDIUM, PROMOTED ON ADJUSTED GROUNDS -- see synthesis §8c** | `Phase: Main phase 1 \| It is the opponent's turn.` The phase name is read first and becomes the frame; the ownership clause arrives after the pilot has decided whose main phase it is looking at, and loses to an English MTG idiom. **Change: name the OWNER first and make the phase POSSESSIVE** -- `It is the OPPONENT'S turn \| Phase: their Main phase 1 (you have priority)` / `It is YOUR turn \| Phase: your Main phase 1` | the situation-block phase header. **One format string, zero behaviour change** | deck36 vs152 **s21** (kind=priority, **177s / 11,969 chars on a `pass`**): *"it explicitly states 'Phase: Main phase 1 \| It is the opponent's turn.' **This is a contradiction in the provided text**"*. Wave-32's witness is the SAME SEAT (vs152 s15/s16), where it resolved to *"assume the game is actually in my Main Phase 1"* -- a wrong belief about whose turn it is, held while deciding whether to spend mana. **HONEST CAVEAT, recorded: the wave-32 promotion condition asked for a SECOND SEAT and deck152 -- whose games produced both witnesses -- reports ZERO occurrences and says the watch should not promote on its data.** Promoted anyway on recurrence across a corpus AND a binary change that touched the priority path (`bdd07223c`), on cost (both witnesses are among their corpus's most expensive replies; the wave-33 one is 16% of that seat's reply time in a game adjudicated at the cap two turns later), and on the fix being one format string | **EVERY seat, reported as a FRACTION** -- the promotion rests on a low per-seat rate holding across many seats, not on one seat's volume. **Falsifier: zero replies containing "contradiction" within 200 chars of "Main phase", pool-wide.** Baseline: deck36 1/42 opponent-turn main-phase priority windows; every other seat 0. **Kept SEPARATE: #24 (N-36k)**, the log-vs-phase reconciliation, a different mechanism with the same symptom |
| **14** | **L-158-2 + owner docket 3** (narration decoration) | **MEDIUM -- and the docket's estimate is 5-6x LOW** | Two separable things. **(a) The docket's target**, `{card text: ...}` / `[cost: ...]` / dynamic magnitudes on CONSUMED decisions: **measured small and confirmed** -- deck36 72/201 prompts, mean **81** chars, max 501 against a p50 prompt of 5,754; deck139 20/178; deck105 ~320 chars/line. **Wrong by architecture; will not move the clock.** **(b) The large sibling deck158 found: the menu echo replays a CONSUMED ask's ENTIRE INSTRUCTION TEXT, in the imperative, as history** | (a) `describeAction` consumer **`AIPlayerGPT.cpp:4787`** + the menu echo **`:4923`**. (b) the same menu echo -- the narrated form of a consumed decision needs only WHAT WAS CHOSEN; everything before the `->` is live instruction addressed to a decision that is over | (b) measured at deck158: **950 / 1,672 / 1,980 chars = 15.4% / 18.7% / 18.1% of the game log** in its vs36 / vs152 / vs146 last prompts, **growing with game length**, i.e. largest where the prompt is already largest. Specimen: a full `- TARGET CHOICE for Orcish Bowmasters - its "damage any target" ability (this spell/ability is already on the stack and needs a target - it is NOT a cast or phase step). Pick the ONE target ... and answer with the chosen TARGET's name (not "Orcish Bowmasters") -> Salvage Slasher (1/1) ...` The mulligan echo is the same shape | **deck158 + deck36.** Ship (a) and (b) together; (b) is where the bytes are. Validation: consumed-decision log lines contain the choice and no imperative instruction text; measure the game-log share at deck158 again (baseline 15-19%) |
| **15** | **N-116h** | **MEDIUM, HARNESS/PROMPT** | **The PLAN field is treated as authoritative over the board snapshot in the same prompt** -- including over the engine's own staleness warning attached to it | the plan-block emitter. **Direction: when the staleness note fires, the plan block is already known-stale -- consider SUPPRESSING the plan text rather than annotating it** | deck116 vs105 seq13, downstream of #2's no-op: *"The game state indicates that Emrakul has already been put onto the battlefield ... (**'Your Plan' confirms this sequence occurred**)"* while the same prompt reads `Your battlefield (creatures: 0)` and `Your hand: ... Emrakul, the Aeons Torn {15}` **and** carries `note: the actions your plan names are no longer among the options available right now`. **Two decisions spent on the false belief.** Related and separately evidenced: deck146's 9,091-char spiral re-injected VERBATIM as `YOUR PLAN` into two following prompts, where s37 re-ran the identical failed arithmetic; deck158's 146 s19 plan line is a raw arithmetic breakdown (*"16-14=2, wait, 16-14=2? No..."*) replayed into s20 | **deck116 + deck146 + deck158.** Interacts with #3 (N-36e) and with #1a: under ANSWER + changed-only PLAN, with reasoning in the native channel, none of these shapes can form -- which is a PREDICTED effect and must be measured, not assumed (#1c metric (e)) |
| **16** | **N-146o** | **MEDIUM, CARD SCRIPT** | **Nadaar's anthem buffs Nadaar.** Oracle: *"**Other** creatures you control get +1/+1 as long as you've completed a dungeon."* The primitive is `auto=this(variable{pdungeoncompleted}>0) lord(creature\|myBattlefield) 1/1`, and `lord(creature\|myBattlefield)` includes the source | **`borderline.txt:75541`**. Fix: `lord(other creature\|myBattlefield) 1/1`, the idiom other anthem primitives use | Double-confirmed. deck146 vs116 seq35: `Nadaar, Selfless Paladin {2}{w} (4/4) (printed 3/3) [vigilance]` with **one** Nadaar in play and 2 dungeons completed -- the +1 can only be its own. seq33: **two** Nadaars each rendering `(5/5)` = 3/3 +1 (the other) +1 (itself); correct semantics give 4/4 | **deck146.** No decision was lost to it (it inflates the pilot's attack math in its own favour and the render is self-consistent), but it is a rules-wrong script in a deck whose whole payoff is the anthem, **and the guide teaches the anthem explicitly.** Verify with a PARSETEST/board-render case: exactly one Nadaar + a completed dungeon -> `(3/3)` with NO printed tag on Nadaar and +1/+1 on everything else |
| **17** | **N-158t** | **MEDIUM, RENDER** | **The blocker forecast's naive 1-on-1 trade ignores "whenever this becomes blocked, it gets +X/+X"** -- and it is wrong in the direction that loses a blocker for nothing | the blocker-seam trade forecast; apply becomes-blocked static/triggered P/T modifiers to the attacker before computing the trade. **Cheap floor if the general case is expensive: when the attacker's text contains a becomes-blocked clause, emit `(trade unknown - it grows when blocked)` rather than a confident wrong verdict** | deck158 vs105 seq13: `B1. Orcish Bowmasters (1/1) [flash] - may block A1 (**both die**)` against `A1. Ichorclaw Myr (1/1) ... {text: ... Whenever Ichorclaw Myr becomes blocked, it gets +2/+2 until end of turn.}` -- the rendered card text on the same screen already carries it. Resolution: Bowmasters dealt 1, took 3 infect counters, **died; the Myr survived.** Truth was `your blocker dies, attacker lives`. **The model reproduced the engine's error independently, so it is not purely an echo -- but the render agreed with it and removed any chance of correction.** No harm (the block was correct on poison grounds either way) | **deck158.** The class is small and enumerable (becomes-blocked pumps, blocked-by-one-creature bonuses). **The render's honesty at this seam is what the entire guide-side blocker procedure is built on** -- deck158's own 8/8 blocker record is keyed to the `- may block An` list |
| **18** | **N-36i** (supersedes N-36d) | **MEDIUM, RENDER, arrival-traced twice** | **A non-artifact land carries NO type tag on the battlefield line, and two emitters disagree about the same card.** Artifact lands render `Vault of Whispers [artifact]`; plain lands render bare. In a list otherwise composed of `[artifact]`, the pilot has no discriminator | the battlefield-line emitter. **`[land]` already EXISTS as a token** -- the SACRIFICE menu emitter prints `Swamp [land] [your battlefield] [tapped]` / `Tolarian Academy [land] [your battlefield]`. Fix: emit `[land]` on the battlefield line for every land, so an artifact land reads `Vault of Whispers [land] [artifact]` | deck36 vs105 s11 against `Artifacts in play: you 5`: six items, five tagged, `Tolarian Academy` bare (`type=Legendary Land`, so the engine's 5 is CORRECT). Reply: *"Total = 6 artifacts. The prompt says 'Artifacts in play: you 5'... Or maybe the prompt's summary count is slightly off?"* Repeated at s23. **No wrong decision** (the guide's read-don't-count rule held both times); the cost is reply budget and a rehearsed doubt about the one number this deck's plan is denominated in. **This is wave-32's N-36d with the failure mode INVERTED** (was under-reading artifact lands; now over-reading non-artifact lands) | **deck36** (holds). **Per HL5, enumerate EVERY emitter of a permanent's type tags before declaring it closed** -- at least the battlefield line, the sacrifice/choose-a-card menu, the hand line (`Island (land: taps for {U})`, a THIRD distinct form) and the attacker/blocker option lines. **Falsifiable prediction: zero replies recount the artifact total against the battlefield list** (baseline 2) |

## Tier 3 -- low severity, latent, and the annotation-hygiene tail

| # | id | sev | disposition |
|---|---|---|---|
| **19** | **N-116i** | **LOW, RENDER** | The `colours you can make` field uses **two different colourless tokens**, sometimes in the same line: over 126 `Mana available:` lines at deck116, `{1}` and `{c}` both appear and **6 lines print BOTH** (`{1}{g}{u}{c}`). One measured consequence, deck116 vs152 s24: `colours you can make: {1}{g}` produced a false unpayability claim about a card in its own offer list (self-corrected, cast anyway). **Fix: emit `{c}` uniformly, never `{1}`, in a COLOUR list.** Adjacent and separately carried: **N-36f / deck152's belief 1** -- *"generic is not a colour"*, the wave-32 representation proposal (state what the mana BUYS) did NOT ship; the belief is **0/201 at deck36** and cost **one false decline of an offered payable cast plus a burned activation window at deck152** (deck36 s14/s15). Two seats, two waves; the guide clause is deployed at deck152 and the representation change stays available, not escalated |
| **20** | **N-116j** | **LOW, RENDER, on i1's own new surface** | The poison AGGREGATE line is emitted BEFORE the per-source lines that cause it, and each per-source line then repeats the already-final total (`- Poison: you take 4 poison counters - now 8 of 10` then three `- Infect damage: ...` / `- Poison: you now have 8 of 10`). A reader cannot attribute counters to sources. **Cosmetic; i1's decision-relevant job is done correctly and the pilot read it correctly every time -- this must NOT dilute the i1 verdict.** Second witness at deck146 (a poison line stating a total with no increment, which could read as a no-op). Cheapest to fix now, on a brand-new emitter |
| **21** | **N-36i-b** | **LOW, owner call, cost ZERO** | The situation-block poison pair is SUPPRESSED while both totals are 0 (absent s1-s19 of deck36 vs105, present 17/17 from s20). The blockers header does print `0 of 10`, so the one decision-relevant window is covered. Flagged because it is the same `if (value) print(value)` family i5 just closed, and because for a deck with no infect the OPPONENT's `0 of 10` is the only view it ever gets of an alt-win it could theoretically race |
| **22** | **N-36g** | **LOW, cosmetic, RECURS and SPREADS** | A token's / Clue's `{card text}` payload renders as an empty parenthesis, and the `source:` suffix reads as if the token WERE its creator: `Draw 1 with Clue [cost: {2}, Sacrifice] {card text: "() source: Glaze Fiend"}`; new this wave on the token family, `Pest #1 (0/1) [your battlefield] - "( artifact) source: Nuisance Engine"`. The option LABEL carries the real information and the pilot answered correctly every time |
| **23** | **N-105d / N-105e / N-139l / N-139p / N-139r / N-139t / N-158d / N-36l** | **LOW, carried** | N-105d (`(creatures: N)` header read as a type claim) -- **did NOT recur in 123 decisions, steady state.** N-105e (bottom-line card text truncating mid-word) -- UNEXERCISED (0 mulligans). N-139l (merged-pile cost pip drift) -- **RECURS, 4th corpus**, decision-neutral. N-139p (bottom ask drops its hand-size sentence at N=7) -- no bottom asks at all this corpus; unexercised, NOT closed. N-139r (`@mutated(mytgt)` absent on Snapdax and Illuna) -- data question, unexercised. N-139t (loot DISCARD header attribution) -- both instances CORRECT this corpus; one-sided evidence, not closed. N-158d (Foray's target menu names the Army) -- UNEXERCISED (Foray cast once with no legal target). N-36l (the pilot attributed an aura to the wrong permanent, 1/11, self-corrected) -- a rate, not a render defect |
| **24** | **N-36k** | **MEDIUM, the seat's single worst reply** | **The game log's turn-scoped entries read as THIS-turn entries against the phase header.** deck36 vs158 s16, **227s / 13,930 chars on a `pass`**: *"'Phase: Attackers' -> Attack ... Then 'CURRENT SITUATION' says 'Phase: Main phase 1'. **This is contradictory.** ... Unless the log is describing a hypothetical or previous turn? No, the log says 'Turn 6'."* The cited entries belong to an EARLIER turn under the same `=== Turn N ===` heading convention and the pilot cannot tell the current turn's entries have not yet been written. **Cheapest fix: mark the in-progress turn (`=== Turn 7 - YOUR turn (IN PROGRESS) ===`) or emit the current phase as the last log line so the log and the header cannot disagree.** Deliberately NOT counted toward #13's witness -- different mechanism. **Related and cheaper, recorded not proposed:** several `priority` replies burn budget on *"if playing a land is legal it should be listed; if it's not listed, maybe I'm not allowed to?"* (4 instances, 2 seats); one line on the priority prompt -- *this list is complete for this step* -- would close a recurring paragraph |
| **25** | **N-36c** | **MEDIUM, CARRIED, NOT FIXED -- now a pure representation debt** | Master of Etherium's cast-line CDA is built while the card is still in hand, so its self-inclusive artifact count is ONE SHORT. Re-verified **3/3** this corpus (vs105 s23 `(8/8)` at `you 8` -> blocks as `(9/9)`; vs152 s24 `(6/6)` -> board `(7/7)`; vs139 s11 `(7/7)` -> attacker lines `(10/10)` then `(11/11)`). **The guide's `+1` stopgap is doing the work perfectly, 4/4 plans state the post-resolution number** (wave-32 baseline: 0/2), so this is no longer a decision risk. Fix remains: build the cast-line CDA against the POST-RESOLUTION battlefield, as the Gray Merchant `{drains N}` fix did. **POSITIVE OBLIGATION ON THE WAVE-34 REVIEWER: the first corpus in which the cast line prints the post-resolution value, deck36's "add 1" block is DELETED that same wave and the prediction inverts** |
| **26** | **DAMAGE-ORDER ASK BUNDLING** | **COST OBSERVATION, not a defect -- bundling candidate** | The new LLM damage-assignment ORDER seam (`d083a0629`) is **EXERCISED, correct, and expensive**: deck139 v105 s14/s15/s16, one attacker blocked by four creatures produced **three sequential position asks costing 140s and 11,915 chars** for an outcome worth one creature (picks legal and sensible -- the 0/3 first, killed exactly). A 4-blocker combat can add three full model round-trips to a single combat step. **A single "give the whole order in one reply" ask would collapse it, mirroring the existing one-reply attacker and blocker declarations.** Weigh against #1a: the protocol change reduces the per-ask cost but not the ask COUNT |
| **27** | **N-116a / N-116d** | **CARRIED, ROUTED -- counted, not re-diagnosed** | **N-116a: 9 whole-library reveals, 439 options rendered for 40 eligible = 11.0 : 1** (Fabricate 50/3, 49/4, 50/4; Time of Need 46/5, 51/5, 51/5, 47/4, 46/5, 49/5), prompts 12,535-24,984 chars. **NEW datum that moves the fix's docket: the reveal REPLY tax has COLLAPSED** -- all nine replies 622-1,054 chars at 11-17s (wave-32: 13,796 chars / 220.6s), picks 9/9 legal and 9/9 correct. **The remaining cost is entirely PROMPT-side, so this is now a CONTEXT-BUDGET item, not a latency item**, which changes where it sits against the cap work. **N-116d: 0 occurrences -- UNEXERCISED, NOT fixed.** All nine reveals came from `<1>`-tokened effects; the only un-tokened effect in deck116 is Farseek, **offered 36 times and cast ZERO times because the guide demoted it.** ⚠ **deck116 is DISQUALIFIED as the witness for any subset-framing fix while this guide is deployed** (wave-33 HL10) |
| **28** | **N-139m** / **N-146h layer 2** / **N-158f + N-158i** / **N-36h-2** | **PROBE LANE -- and one of them just retired itself** | **N-139m** (HARNESS): trigger predicate fired **1/1 with a full arrival** (vs116 s15->s21, discounted `{g}{g}` paid, loot fired twice, `[mutated pile]` arrived, N-139g confirmed a third time). **Report the denominator's cause: the predicate had 6 opportunities and 5 were consumed by a guide-layer decision defect (#9), not by fading risk.** Harness scoping stands. **N-146h layer 2** (Agadeem): **STILL UNEXERCISED, 0 cast offers / 0 casts / 0 menus, 4th corpus.** The probe requested in wave-32 was not produced; the guide's crutch STAYS and its demotion is not eligible. **Discharge by PROBE, not by another corpus of absence.** **N-158f / N-158i** (Assault on Osgiliath): **THIRD consecutive UNEXERCISED verdict across three binaries** despite 2 copies in the 60-card list -- **per wave-33 HL12 this retires from the corpus ledger to a PROBE**; spec exists (deck199: 4x Assault, 4x March, 8 Mountain / 8 Swamp, one pinned game; observables are an Attackers phase in the same turn Assault resolves, and the first-strike ordering in the combat log). **N-36h-2** (i10): **the wave-32 probe spec is CANCELLED -- the POOL exercised it** (11/11 both render directions, correct against the primitive). **Residual narrowed to two: (a) instance handles in the reverse render** (host here was a singleton; trigger = an Aura/Equipment on a permanent whose name appears 2+ times, corpus count 0) -- narrowed probe spec in deck36's notes; **(b) Equipment entirely, 0/201 and 0/42 last wave -- a ROSTER request, not a probe** |
| **29** | **RELIC PUTRESCENCE probe (wave-32 spec)** | **STILL OWED, narrowed** | Cast once and correctly attached to an opponent's `Mox Jet` (11 targets offered, the right one picked), then sat attached for three turns with deck36's poison frozen at `1 of 10`. **Whether the enchanted artifact ever became tapped is not rendered, so `@tapped(mytgt):damage:1 targetcontroller` on an `abilities=infect` Aura is neither validated nor falsified.** The probe question stands: does it yield a POISON counter or 1 life loss? **The OTHER half of the same wave-32 spec -- Hand of the Praetors -- is DISCHARGED by natural play** (lord bonus in the rendered P/T, cast-trigger per infect creature spell, 3/3 correct targets, carried a win) |
| **30** | carried, unchanged | -- | **N-152a** (MDFC back-face land placement; still unofferable, engine-true at this binary -- deck146 vs116 seq30 offered only `Cast Card Normally` / `Flip Side [display toggle only]`) · **N-152h** (MDFC-spell Flip Side wording, THIRD path; **UNEXERCISED at deck152 and deck146 this corpus, 0 hits -- must be cited from a seat that draws MDFC spells**) · **N-146p** (Soul Shatter never takes a planeswalker; Oracle says creature **or planeswalker**, primitive is creatures only; unexercised; the guide stays ENGINE-TRUE) · **N-139q** (Nethroi returns ANY NUMBER of creatures each with power<=10, Oracle is *total* power 10 or less; unexercised, guide stays generic) · **Kaya +1 render/engine divergence** (option `{card text}` shows the full Oracle *"up to one target nontoken creature"*, primitive targets your own only; no cost this corpus; the guide teaches the ENGINE version) · **N-158e** (flash windows -- KNOWN-OPEN, steady state 3rd corpus, 2 priority windows in 6 games both Upkeep, Bowmasters never offered at instant speed; **not counted against any guide**) · **`creaures` typo** in the activated-ability option text (deck158) · **L-18-1** planning tax (length-only watch; **WORSE at deck36, 39% -> 53%**, and it is #3's N-36e) · **N-134b/d · ETB header card-name · N8 · Azcanta · undying · commit-lock · prose-reversal · untapped-tagging · I-4** · **i5-reask probe debt** (still probe-validated only -- **zero validation rejects have now occurred in ELEVEN corpora**; consider closing it as unreachable-by-corpus per HL12) |

## CLOSED this wave

- **N-105a** (poison rendered nowhere) -- **CLOSED, VALIDATED, 100%.** 120/120 at deck105's two
  seats and 100% at every other exercising seat; non-combat `alterpoison` narrates; distinct infect
  narration for both the player and the creature half; correctly suppressed at 0/0. All three of the
  item's own validation predicates pass. The wave-32 four-silent-events class is DEAD.
- **N-105b** (false life forecast on infect) -- **CLOSED, VALIDATED, BOTH DIRECTIONS.** 10/10 + 3/3
  + 4/4 + 1/1 x4 partitioned; **zero** `you would be at <life-N> - NOT lethal` emissions on any
  infect window at any seat; the *"taking damage while ahead"* misteach has zero occurrences on the
  infect branch; negative controls unchanged at all seven seats. **Behaviour: infect blocker windows
  ending in a declared block 8/12 -> 10/10.** Aliases closed with it: N-116b, N-139o, N-146l,
  N-152i, N-158j, N-36a.
- **deck105 POOL RE-RUN (the wave-32 MANDATE)** -- **DISCHARGED. 6/6 -> 2/6.** The perfecta was
  substantially a render artifact; the wave-32 win table must not be cited as a baseline.
- **N-152f** (`deferFlexibleSource` mutual-deferral deadlock) -- **CLOSED.** Two arrivals on the
  BROKEN branch (deck36 s26, deck116 s29), a three-window negative-control ladder in the same game
  isolating the fix to a one-source delta, and a **0/44 false-suppression sweep**. Recommend the
  wave-32 fixture pair be registered as regression tests and the POSITIVE one confirmed green on
  `bdb30d8e7`.
- **N-152d layer 2** (`(printed X/Y)` on the board snapshot) -- **CLOSED.** 22 -> 0; Packleader
  24/24 clean (wave-32's zero was coverage, this is proof), Cathar 110/110, Huntmaster 49/49; **51
  distinct tagged renders at TRUE deltas as the positive control**; confirmed at three further seats.
- **N-158k** (Feed the Swarm costs absent on the target menu) -- **CLOSED, and the wave-33 brief's
  KNOWN-OPEN entry was STALE.** It shipped in the step-1 batch at `948578887`
  (`AIPlayerGPT.cpp:6363-6376`, PARSETEST `[W33-N158k]` at `:10825`); live at **3/3 menus, 11/11
  target lines, every number correct**, and it is **load-bearing at the COMMIT seat** (deck158 146
  s28 paid 2 life for the card blanking its Army over two free Spiders). **Method consequence:
  wave-33 HL10 -- a KNOWN-OPEN item owned by your seat gets a MEASUREMENT, not a citation.**
- **N-158l** (empty `[from ]`) -- **CLOSED, 0 occurrences** (was 38 prompts). Attribution: interim
  `0280a08b4`, **not** the step-1 batch.
- **N-158m** (amass macro scanner) -- **CLOSED.** 3/52 (5.8%) -> **59/63 = 100% of non-trigger-gated
  offers**; the four misses are `_DIES_`-gated Easterling Vanguard and are correctly suppressed.
  ⚠ The multi-branch negative control (Assault on Osgiliath) remains a CODE READ, never offered.
- **N-146k** (pregame battlefield mana line) -- **CLOSED.** 0/8 pregame prompts carry a
  `Mana available:` line or any battlefield fact; the echo class produced **0 replies** (was 5/14).
- **N-146j** engine half -- **CLOSED.** `TargetChooser.cpp:1066-1092` now sets only
  `setisMultiColored(-1)`; the falsifier passes (0 colourless / artifact / land targets in 21 target
  lists). **The POSITIVE half is UNEXERCISED and stays open at #27's severity: no coloured artifact
  permanent appeared at deck146 in six games.** Discharge from deck36's seat or a probe.
- **N-139n** (mulligan hand-count header) -- **CLOSED, VALIDATED, the strongest single-fix result
  the seat has produced.** Mulligans 9 -> 0, false mulligans 6/9 -> 0/0, bottom asks 3 -> 0,
  `zero green sources` 0 hits, header arithmetically correct 6/6. **Untested direction recorded, not
  claimed: no unkeepable hand appeared, so a count header's FALSE-KEEP failure mode is unmeasured.**
- **N-139i** (London bottoming) -- stays closed. **N-139j** (0-power attacker tag) -- stays closed,
  4th corpus, 33 windows, 0 declarations of a `[deals 0 ...]` creature.
- **N-139s** (an all-illegal blocker set exits as a silent `unparsed_reply`) -- **CLOSED by i8's new
  `all_assignments_illegal` class**, at one live witness with its pruned pair named (deck152 vs36
  s8). **Trigger shape for the ledger: a single ground blocker, a two-attacker set one of which is
  evasive, and a reply that reasons the trade math without ever consulting the per-blocker
  `may block A#` list.**
- **N-36b** ({0}-cost token deletion) -- **CLOSED at the natural witness.** 508/508 cost emitters
  carry `{0}`, plus a fourth new emitter; 0 confabulations in 201 replies; `Cast nothing right now`
  **0 taken of 45 offers**. **Residual opened as #7 -- the absent-cost case.**
- **N-36d** (Academy burned in the upkeep) -- **CLOSED BY THE GUIDE, not the engine.** Upkeep
  activations 6/8 -> **0/7**, opponent's-turn 0/3. (Its render half is superseded by **#18**,
  N-36i, which is the same defect with the failure mode inverted.)
- **N-36h** (the i10 attachment probe spec) -- **CANCELLED; the pool exercised it.** See #28 for the
  two narrowed residuals.
- **N-36f** (generic-payability belief at deck36) -- **belief ABSENT 0/201, representation
  unchanged.** Recorded as steady state, NOT closed as an item: it cost a false decline at deck152
  this wave (#19).

---

**OWNER NOTE ON #1a/#1c (2026-08-19, post-synthesis):** latency is not the concern —
"that's a whole other issue." The available bound, held in reserve rather than shipped
as a default: **a reasoning BUDGET with a forced answer** (Qwen thinking supports
capping think tokens and injecting the close-of-thinking marker so the model must
answer from what it has; the Codex backend's effort tiers are the analogous knob).
Per the generous-defaults rule this ships UNBOUNDED; the budget is the documented
lever to pull only if the A/B shows runaway native thinking, not a starting clamp.

**SUPERSEDED — OWNER RULING, 2026-08-19 (later the same day, during wave-34 step 1).
The budget is BOUNDED FROM THE START; the paragraph above stands only as the
position it replaced.** His derivation, and then his override of it:

- **t** = the time bound expressed as tokens = `WAGIC_GPT_TIMEOUT 240s x ~30 tok/s` = **7200**
- **p** = max expected PLAN = p95 592 chars => **200 tokens**
- **c** = the coded choice line = p95 74 chars => **30 tokens**
- **a** = t - p - c = **6970**;  **b = 1.5a ≈ 10450 tokens** of reasoning budget
- **his override on that number: "oof. thats a lot. lets make it 8000"** — so the SHIPPED
  starting budget is a flat **8000**, and it is explicitly a **first-round CALIBRATION
  value**: *"we'll almost certainly tune this lower... one round with this budget should
  give us a better idea of where the budget limit should actually be."*

**Shipped in wave-34 step 1** (`AIPlayerGPT.cpp`, `GptConfig.*`):

1. `reasoning_budget=` in endpoints.txt / `WAGIC_GPT_REASONING_BUDGET`, default **8000**
   when thinking is ON, 0-or-less = unbounded. Thinking OFF is untouched in every respect.
2. **Budget-then-FORCE-ANSWER, two-phase** — a bare `max_tokens` truncation that loses the
   answer is not acceptable. Phase 1 caps at `budget + 400` (the answer reserve: p95 PLAN
   200 + choice line 30 + margin). If that reply comes back with an **UNCLOSED `<think>`**,
   phase 2 re-sends the same decision with the model's own truncated thinking as an
   **assistant prefill** with `</think>` injected and a 400-token cap, so it must answer
   from what it has. Qwen's documented budget-forcing pattern, expressed through vLLM's
   `continue_final_message` rather than a raw `/v1/completions` prompt render — the client
   holds no copy of the chat template, and a mis-rendered prompt would be a silent quality
   change. Scoped to endpoints that accept a prefill (not Codex, not OpenRouter, not
   api.openai.com — those hide reasoning entirely and have nothing to close).
3. **Translog:** `reasoning` (verbatim), `reasoning_chars`, `reasoning_tokens` (the
   server's own count when it reports one — the budget is denominated in tokens, so the
   next budget is read off *this* distribution: p95/p99 by decision kind against the
   `reasoning_budget_hit` rate), and `reasoning_budget_hit` when a forced close fired.

**LIVE-PROBED against the real endpoint (Spark vLLM 0.23.1rc1 + qwen35, 2026-08-19) —
three facts that overturned plausible defaults, and what each one changed:**

- **The field is `message.reasoning`, not `reasoning_content`**, and with the parser active
  `content` arrives ALREADY stripped — no inline `<think>` ever reaches the client. Capture
  now reads `reasoning_content` → `reasoning` → inline `<think>` fallback, in that order.
  Betting on the OpenAI spelling alone would have produced a corpus with an empty reasoning
  column and a blind seat review — the exact failure #1b exists to prevent.
- **Server-side default thinking is ON for this model** (a request with no
  `chat_template_kwargs` generated into `reasoning`). So the thinking-OFF arm is only OFF
  because the client sends `enable_thinking: false` EXPLICITLY; it does, unconditionally
  outside api.openai.com. Omitting the field on the OFF arm would have run both arms with
  thinking on and produced a null A/B.
- **The reasoning parser classifies by GENERATED tokens only.** `continue_final_message` is
  accepted, but our PREFILLED `</think>` is never generated, so the whole phase-2 generation
  lands in `reasoning` with `content` null (verified: prefill + continue → `reasoning` =
  `"\n\nCHOICE: 1"`). The injected close still does its job on the MODEL — it answered
  immediately — just not on the server's field routing. So the answer candidate is `content`
  when non-empty, else the tail of `reasoning` **from its last coded answer line, and only
  when nothing substantive follows that line**. Anchored to the end deliberately: phase-1
  mid-thinking text is full of candidates the model was still weighing, and a reply that
  stopped at the cap (`finish_reason == "length"`) is mid-thought by definition and is
  refused whatever it looks like.
- Consequences recorded: a **phase-1 budget hit** on this build reads as
  `finish_reason == "length"` + empty content + non-empty reasoning (there is no inline
  block to test, so that shape — not an unclosed `<think>` — is the normal detector).
  `usage` carries **no** `reasoning_tokens` on this build (prompt/completion/total only, and
  `completion_tokens` covers thinking+reply combined), so `reasoning_chars` is the working
  length metric; the `reasoning_tokens` read stays as forward compatibility.
- **New fallback class `reasoning_only`.** `empty_reply` has always meant TRANSPORT — nothing
  came back. A reply that arrives complete and paid for with `content` null because the whole
  generation was filed as thinking is a MODEL behaviour, and scoring it as an endpoint fault
  would corrupt the one distinction the A/B turns on. Seats read `reasoning_only` separately.

**HIDDEN-TRACE PROVIDERS (owner requirement, 2026-08-19) — the third reasoning shape, and the
one that governs what ships to USERS rather than what the dev loop measures:**

Some providers reason and never return the trace: OpenAI and Anthropic withhold raw
chain-of-thought as policy, and OpenRouter can hide it depending on the upstream — this
project has already been bitten by exactly that, as the 40s mystery latency behind a
140-token answer (`71f4f615c`). The client now treats this as NORMAL:

- **Shape:** reasoning requested + `content` NON-EMPTY + no trace anywhere (no
  `message.reasoning`, no `reasoning_content`, no inline `<think>`). It parses exactly as the
  thinking-off path parses, and is marked **`reasoning_hidden`** in the translog. It is the
  INVERSE of `reasoning_only` (empty content, trace present) and is never conflated with it.
  Without the marker the A/B cannot separate *"reasoned invisibly"* from *"did not reason"* —
  both write no reasoning field, and only one of them is paying for thinking tokens.
- **No parse or fallback anywhere gates on reasoning being present.** The *"reasoning present
  and non-empty at BOTH seats before the corpus runs"* rule (deck152's condition, adopted at
  #1b) is a **Spark/dev-loop precondition for the A/B**, not a client invariant — encoding it
  in the client would turn every OpenAI/Anthropic user's game into a heuristic-only game.
- **The two-phase forced close cannot exist on a hidden-trace provider** (there is no trace to
  prefill), and is guarded twice: `gptForceCloseSupported()` excludes those endpoints by name,
  and the fire condition now states `content.empty() && !reasoning.empty()` explicitly rather
  than leaving it implied — a content-bearing reply needs no rescue in the first place.
- **`reasoning_budget` therefore applies only where a raw thinking channel exists** (vLLM /
  llama.cpp families). On the subscription (Codex) backend the budget's analogue is the
  existing `reasoning_effort` tier, which is what that adapter already sends; on OpenRouter the
  unified `reasoning` switch is the only lever. The config key is inert on all of them, and
  `reasoningRequested()` reads the Codex tier rather than the thinking flag so a withheld trace
  is still recognised there.
- **Latency accounting:** hidden reasoning still costs decode time, invisibly. `latency_ms`
  already carries it; the pair to read is `reasoning_hidden` + a long round trip = a WITHHELD
  trace, never a defect class. `reasoning_chars` 0 on such a record means nothing was returned,
  not that nothing was spent.
4. **Timeout ≥ the full two-phase worst case.** The HTTP timeout is the ONLY watchdog that
   falls back to the heuristic (the patience window raises a human prompt and never decides
   on its own — verified, and it is gated on a human seat so self-play never sees it).
   Worst case at ~30 tok/s: phase 1 = prefill (~10-20s) + (8000 + ~230) tokens ≈ **295s**;
   a budget hit adds phase 2 = re-prefill of prompt + thinking (~15-25s) + ~400 tokens
   ≈ **40s**; total ≈ **335-360s**. Thinking-mode default is therefore **420s**, a margin
   rather than a shave, and `tools/selfplay-harness.sh` — which pinned
   `WAGIC_GPT_TIMEOUT=240` unconditionally and would have overridden it for the whole A/B
   corpus — now defaults to 420 under `--thinking`. Explicit env/config still wins.

**ORCHESTRATOR NOTE, wave-34 corpus launch (2026-08-19):** pre-corpus probe (109v133,
--thinking, -j 1) PASSED the reasoning gate (36/36 decisions captured at both seats;
replies collapsed to p50 44 chars; one live budget_hit recovered by the forced close;
0 fallbacks) but measured native thinking LONGER than the prose it replaced (p50 ~12k
chars ≈ 3.5k tokens; latency p50 16s → ~80s solo). The 3000s cap would adjudicate most
games AND censor the thinking-length distribution the calibration exists to measure —
so the wave-34 corpus runs at **-T 7200** (deviation from "hold both levers",
calibration-forced; -j 3 and REPPENALTY held). Cap-sensitive game-outcome comparisons
vs wave-33 are accordingly OFF the table for this corpus; decision-level metrics remain
the A/B currency.

**OWNER DIRECTIVE — wave-35+ corpus throughput shape (2026-08-19):** "dial in the
permitted reasoning length, make the wall clock guard very generous, then push
concurrency up — otherwise we're losing a lot of throughput and paying it in wallclock
time." Concretely: (1) tune reasoning_budget from the wave-34 calibration distribution
(p99 by decision kind + budget_hit rate); (2) BOTH wall-clock guards go very generous —
the per-decision fallback timeout scales with contention (at -j 8 expect 3-4x solo
latency: think ~900s+, never the tight default), and the game cap likewise (-T 14400
class); (3) THEN raise -j (Spark's batched decode raises aggregate tok/s with
concurrency; ceiling = max-num-seqs 16, request load cannot trip Spark's power
ceiling). RE-ADJUDICATE the old "-j 6 halves decision quality" caution — it was
measured under a 120s HTTP timeout and its fallbacks were timeout artifacts; under
generous guards the quality cost may be zero. Validate on the first high-j corpus:
fallback rate + decision-quality spot-check vs a -j 3 baseline before adopting as
standing.

**OWNER METHOD — derive the wall-clock guard from measured throughput (2026-08-19):**
guard(j) = worst_case_response_tokens / per_stream_tok_s_p10(j) x 1.5, where
worst_case = reasoning_budget + reply (p95 PLAN+answer) + phase-2 (~630 tok), and the
p10 (slowest-decile) per-stream rate is MEASURED at the target concurrency, never
assumed. Live wave-34 measurement at -j 3 (61 decisions): p50 36.5 tok/s, p10 19.8
=> guard(3) ~= 671s. FINDING: the hand-set 420s thinking timeout is BELOW the derived
guard — a full-budget decision at slow-decile rate can fall back mid-thinking (rare:
budget_hit ~1/36 in the probe; visible as counted fallbacks). Wave-35: set the timeout
FROM this formula at whatever -j is chosen (measure tok/s(j) first — a short sweep or
the previous corpus's translogs); per-stream p10 will drop as -j rises, so guard scales
up as concurrency scales up. The tuned (smaller) budget shrinks worst_case and pulls
the guard back down — the three knobs move together.

**OWNER RULING — GAME-LOG NARRATION REGISTER (2026-08-19, P0 for wave-35 step 1):**
"the historical log should be like 'you drew _' or 'you cast _ targeting _' or
'_ resolved' or 'opponents _ died' etc. this present history is nothing like i
intended." Source evidence: ~/Downloads/wagicgpt-prompt-example.txt (surfaced by
another session). The log must read as PAST-TENSE DECLARATIVE GAME EVENTS. Three
offending classes observed in one 96-line prompt:
(1) raw zone mechanics ("Your Arboreal Grazer: hand -> stack") → "You cast Arboreal
    Grazer" / "<X> resolved"; library->hand → "You drew <X>" (opponent's stays
    count-only: "Opponent drew a card");
(2) consumed-ask ECHOES — generic header + full decorated option text ("A choice is
    required - choose an option: -> pay 2 life [this permanent then enters ...
    UNTAPPED - usable ... this turn]") → record the CONSEQUENCE, naming the subject:
    "Stomping Ground entered untapped (you paid 2 life)". The bracketed guidance
    annotations are decision-time surfaces and NEVER enter history (extends the
    shipped stripNarrationDecoration to this emitter — the ledger #14(b) menu-echo
    half lane A deferred);
(3) question-text echoes on target choices (instructional parentheticals, rules-text
    tails) → "You targeted <X> with <source>'s <ability>" (fold into the cast line
    where it is a cast: "you cast _ targeting _").
IMPLEMENTATION NOTES: this is the model's perception layer corpus-wide — per-emitter
sweep with the HL5 discipline (enumerate every narrateDecision/appendNarration caller
+ every zone-change narration branch); ship narration goldens in PARSETEST (echo
shapes change); the information content of consumed decisions is KEPT (what was
decided + its consequence), only the register changes. Validate on the wave-35 corpus:
zero "A choice is required" / "TARGET CHOICE" / "->" zone-arrow strings in any GAME
LOG section.
ADDENDUM (full-file audit of the same example): (4) `stack -> graveyard` is AMBIGUOUS
between resolved and countered — identical line for both outcomes; the register fix
must distinguish ("<X> resolved" / "<X> was countered") — this is informational, not
stylistic; (5) counter-source attribution is path-inconsistent (line 42 has
[from March from the Black Gate]; lines 65/67, same Army, other trigger paths, have NO
source) — sweep all WEventCounters narration paths; (6) the mulligan ask is another
full Q->A echo emitter ("Pre-game mulligan decision (London mulligan). ... ? -> Keep
this hand" → "You kept your opening hand"); (7) token creation should attribute its
creator ("March from the Black Gate created a 0/0 Orc Army token" vs bare
"created -> battlefield").

**OWNER DOCTRINE — zone attention + the log's zone duty (2026-08-19):** primary focus
(rendered in CURRENT SITUATION, always): STACK, BATTLEFIELD, both LIFE totals, LIBRARY
counts, HAND (own contents / opponent count). Graveyard and exile are NOT rendered in
state — "the historical log is the optimal method to present graveyard and exile
state" (conditional-attention zones, consulted only when interacting). CONSEQUENCES for
the wave-35 narration rewrite: (a) death/discard/exile/countered events are
LOAD-BEARING zone records, not flavor — completeness of those emitters is a zone-
integrity requirement; (b) the 24k narration trim drops early zone events — when the
trim fires, the trim marker must carry a compact zone digest of what fell off. PARKED
(owner's road-not-taken, cache-motivated): per-card draw-chance/held-chance rendering;
note if ever revisited: CURRENT SITUATION is already the uncached tail (no cache
cost), BUT an always-rendered remaining-copies line is ~150-250 tokens/ask x ~60
asks/game = 9-15k tokens/game (owner: "potentially quite alot of tokens") — fails the
value-per-token criterion at the always-on position. The surviving shape is
CONDITIONAL: render draw math only on asks where it IS the decision (mulligan, scry/
surveil order, tutor picks, draw-spell pricing), possibly scoped to guide-named engine
cards. FINAL OWNER RULING (2026-08-19): PARKED, and DO NOT MEASURE IT — "dont waste our time
trying to measure it." His Magic judgment: "you can play at a very high level of magic
without a firm grasp of the probabilities of your deck mid game... during play it's
just a sliver of the pie." No counting question in any reviewer brief, no seat routing,
no proposals. Revisit only if HE raises it.

## Wave-35 step 1 — SHIPPED (2026-08-20)

Four worktree lanes off the wave-34 trace audit (spec: wave34-trace-audit/00-consolidated.md),
merged sequentially to master, gated, pushed. Merge chain: 41216cc4c (lane A protocol surgery:
three-case PLAN rule, stale-plan gate rebuilt in GptPlanCaveat.h, template sync — the stale
"two or three sentences" text lived in deck139_strategy.txt, echo contract = number + short
name, MP2/instants facts, budget default 6000), 18a9eab20 (lane B narration register: all
emitters past-tense declarative, WEventSpellCountered for countered-vs-resolved, graveyard/
exile digests on the trim marker, composed-log golden, WEventLife::settledLife), 5b09cdaea
(lane C render truth: hand "(copy R of N)" tags, honest battlefield header, per-source mana
line + foreach-unwrap in potentialColorReach, land-drop statement, alt-cost divergence note,
amass plus(N), stackAbilityName, poison zero-state when poison live), 20e9db397 (lane D:
selfAsCostPayment fix — Scarecrone's self-sacrifice cost was silently unpayable, general
chooseCard source-exclusion defect; budget_hit gated on finish_reason=="length" with
reasoning_forced_close split out; reasoning_degenerate shingle metric; 01-engine-verdicts.md —
Fateful Absence / Vanishing Verse / Glaze Fiend / Master of Etherium all ENGINE-RIGHT, the
traces' rules claims were hallucinated). Post-merge addition 9833cf39e: cast log lines carry
targets ("You cast X targeting Y" — owner register verbatim; only historical record of a
countered spell's target).

GATES at 9833cf39e: suite 1046/0 + 28 AI/0 single-threaded; PARSETEST 552/0 (404 → 548 via
lanes +34/+45/+65, +4 targeting). Binary /tmp/wagic-9833cf39e. Smoke (146v139, -j1,
--thinking, budget 6000): register verified live — all six negative greps clean, 0 fallbacks
through 34 decisions, latency p50 91s solo, reasoning p50 ~3.3k tok, budget-hit rate ~7% at
the tighter cap (real hits, post-gating).

WAVE-35 CORPUS CONFIG (guard method per owner directive): -j 8, WAGIC_GPT_TIMEOUT=1100
(guard(8) = 6,860 worst-case tok / ~9.5 p10 tok/s × 1.5, extrapolated generously from the
-j3 measurement), -T 14400, budget 6000 default, --thinking, REPPENALTY 1.05, seats
146,139,152,158,105,36,116. Validation gate: fallback rate audited on first completions;
>~1% ⇒ stop, drop to -j 6.

## Concurrency conversion (owner directive, 2026-08-20): saturate the server

Owner: "we definitely want to convert our testing design to maximize concurrency gains" —
run corpora at/above the batch ceiling. Design: wall clock = total generated tokens /
AGGREGATE server tok/s; aggregate rises with batch (weight-read amortization on the MoE), so
the saturated batch is the fastest corpus. Slight OVERSUBSCRIPTION (more games than
max-num-seqs 16) beats exact-16: queued requests backfill batch slots the moment a game is
between decisions, so the server never decodes under-batch. Harness -j cap raised 16 → 64
(runaway guard only). Knobs that scale with j: WAGIC_GPT_TIMEOUT via guard(j) (at j≥16,
p10 per-stream ~5-7 tok/s → guard ~1,700-1,900s), and -T becomes corpus-length (all games
finish together at queue pace). Voyager side: N wagic processes × ~300MB — raise the scope
MemoryMax deliberately. WAVE-36 ADOPTION GATE: pin the aggregate curve first — this wave's
-j 8 measured point + one measured point at 16; convert with measured guards, not
extrapolations (a mis-set guard at batch 16 = a fallback storm discovered hours in).
Wave-35 corpus deliberately left at -j 8 mid-flight (no config changes mid-corpus).

OWNER RULING (2026-08-20): finish wave-35 on the current -j 8 run as-is. THE OWNER does the
serve-configuration work between waves — Claude does not touch or re-examine the Spark serving
config; the measured batch-8 findings above (aggregate 68 vs 110 at -j3, step time 84→357ms,
MTP acceptance healthy at 69%) are the handoff data for that work. Next-wave corpus config
waits on his serve changes.

OWNER RULING (2026-08-20, supersedes "finish the wave as-is" above — INVARIANT): a selfplay
corpus COMPLETES A FULL GAME FOR EVERY DECK MATCHUP or the test has FAILED. Verbatim: "if the
test is not completing games, then it is a failure... this test requires a full game for every
deck matchup. if its not getting that, then the test is wrong." Cap adjudication is a backstop
for rare stragglers, never an accepted outcome class; truncated runs are not "calibration
corpora." The -j 8 run was killed on his order (matchups-20260820-115109, marked
FAILED-NOT-A-CORPUS). ENFORCEMENT: selfplay-harness.sh now runs a feasibility watchdog —
at ≥15 measured decisions it projects median latency × expected decisions/game
(WAGIC_CORPUS_DECISIONS, default 130) against -T and kills the whole corpus with a loud
INFEASIBLE verdict if a full game cannot fit (verified: today's 419s median → INFEASIBLE;
90s → OK). Wave-35 corpus reruns after the owner's serve work, when the arithmetic passes.
