# Skill: Writing a per-deck strategy guide for the qwen pilot (wave 5)

## Synthesis notes (wave 5)

Unified from seven independent deck-agent revisions (decks 44, 109, 110, 131, 133, 135, 140) of the
wave-4 skill, all mining the wave-5 corpus (one round-robin run, 6 games/deck). Wave 5 is the wave
where the platform caught up to the diagnosis: the head-first reply parser is CLEAN across all seven
seats (0 desyncs), the X-menu was reversed to largest-first, the plan-echo splice is gone, the
negative-life state-based loss fires, and — landed AFTER this corpus — the STACK is now a first-class
CURRENT SITUATION section ("ON THE STACK, waiting to resolve — top resolves first; you can respond
now") with controller/name/[spell|ability]/targets, and counter-type options are labeled with what
they can hit ("Cast Counterspell {u}{u} - can target on the stack: Master of Etherium"). With the
transport and several representation defects fixed, the wave's signal is sharper and its two lessons
are structural.

**The dominant theme: representation beats instruction, now double-confirmed — stop escalating wording
at a read-then-disobey wall.** deck110's `seq84` is the clean proof: the pilot READ the two-power-number
done-signal, VERBALIZED "already attached... waste of mana," and STILL took the re-offered equip.
deck44 is the same shape at the stack: the pending spell lived only in the log tail, and the pilot
fabricated a confident false resolution over it ("the spell moved to the battlefield"). deck133's
plan/list disconnect is a third. When the corpus shows the pilot correctly deriving a fact from the
board/log and then acting against it, instruction has failed at full strength — record it as a
representation defect and route it, do not write a louder rule. The stack-surfacing that shipped this
wave is exactly the deck44 fix, vindicating the pattern; the attachment marker (task #26) is the
pending deck110 A/B.

**The second theme: a fix that is obeyed, correct, and non-overshooting can STILL fail, because the
leak moved.** Wave 4 taught OVERSHOOT (a loud fix over-applied). Wave 5 adds DISPLACEMENT (deck135):
the attack floor was obeyed 13/13 and caused zero suicide attacks, yet the deck still lost — the leak
moved UPSTREAM to the cast seam (durdling on value before deploying a body). Re-audit the WHOLE chain,
not just the fixed rule and its inverse.

**Merged into the method this wave (multi-seat or completes an existing line):**
- *DISPLACEMENT as the sibling of overshoot* — a fix can land cleanly and expose the adjacent/upstream
  seam; tally the fixed seam's success AND whether it was even fed good material before concluding
  overshoot (135).
- *OBEYED-BUT-LOSING as a third Step-0 mode* — record down, obedience high, losses attributable to
  deck construction/variance: treat like turnaround mode (freeze obeyed lines, spend the wave on the
  one model-fixable lever), not like a failure to fix (131).
- *A fixed-defect stopgap written against a SURFACE can INVERT, not just go inert* — an engine fix that
  reverses/relabels a menu/label/board surface makes a positionally-phrased stopgap point at the WORST
  option; a guide line the corpus shows the pilot SUCCEEDING WHILE DISOBEYING is a flip/delete signal;
  prefer value-anchored phrasing over positional (140).
- *PLAN/CHOICE MISMATCH (intent collapse) as a distinct Step-2 leak class* — correct plan names a
  listed action, choice is a different passive option, head-int == choice (not a desync, not an
  unaffordable-plan reconcile); weakly guide-mitigable, durable fix is representation/labeling (44).
- *A fact living only in the LOG TAIL is functionally hidden* — the pilot skims it and fabricates a
  confident false reading; the fix is a first-class CURRENT SITUATION line, not a third copy of the
  tell (44); the shipped ON THE STACK section is the template.
- *Attacker-participation denominator = the engine's OFFERED `A#.` list, never the battlefield* —
  summoning-sick/tapped/just-made creatures legitimately shrink the legal set; the battlefield
  denominator manufactures a phantom subset-attack leak (131).
- *Prefer an engine-SURFACED number over a hand-computed one — and verify its OFFSET* — the engine now
  embeds computed values in option text (`{right now: drains N}`); teach the pilot to READ it, but
  audit the display against the RESOLVED result (Gray Merchant under-counts its own devotion by 2)
  (133).
- *A card with TWO distinct uses needs TWO distinct keyed triggers* — the executor fires only the use
  you wrote and misses the adjacent role (Gray Merchant as finisher AND as a lifegain stabilizer)
  (133).
- *A ROLE-CHECK must name the SEAM it fires at* — a defender role that only changes blocking does
  nothing for a deck whose stabilization happens at the CAST seam (135).
- *THREAT-before-VALUE / highest-impact act-floor* — three seats show a low-impact value spell casting
  to "satisfy the act-floor" while win conditions sit in hand; PROMOTED to the core prompt as a
  completion of the existing deploy/impact lines (135, 133, 131).

**Adopted from single seats (additive method, not core):**
- *Utility/mana-dork carve-out INSIDE the attack floor* — the free case taps a mana creature for a
  1-point chip and strips a blocker; name it as an exception with where it goes instead (135).
- *The re-offer three-part fix has a proven CEILING* — say so; once the cue is provably read, stop
  escalating and route to representation (110).
- *Count-brakes key on the BOUNDING RESOURCE, not the turn* — "one Equip per turn" mis-fires with two
  Platings; key it to unattached copies / unused charges (110).
- *Menu-adjacency index carryover* as a third numeric-offset shape — usually immaterial, route to the
  engine ledger, don't write a guide line (140).
- *A bare DON'T aimed at a decision SEAM is the weakest possible placement* — negative AND, if
  terminal, out of position; a seam always needs its own positive floor at a seam-labeled block (109).
- *Digest gotchas* — isolate the `Your battlefield:` line for on-board checks (splitting on `--- CURRENT
  SITUATION ---` also matches hand + carried PLAN); extract the `A#.` list for attacker records (131).

**The counter-tell stopgap is RETIRED core-side.** The wave-4 core carried a stopgap that derived the
pending spell from the log tail and asserted "there is no spell on the stack is always false while the
option is listed." The ON THE STACK section + labeled counter options shipped this wave; the stopgap's
removal condition is MET. The core prompt is rewritten to point at ON THE STACK and to carry the NEW
general insight from the wave-4-E3 investigation (a response window opens only when a response is
PAYABLE — a seat that tapped out on its own turn gets no window, so "died with counters in hand" is a
MANA-DISCIPLINE leak, not a stack-invisibility one; counter-deck guides now teach holding counter mana
open). Wave-6 authors: DELETE every guide copy of the counter-tell; teach reading ON THE STACK instead.
See the general-strategy rationale and `core-prompt-revision.md`.

**Historical (wave 4) synthesis, condensed for continuity:** wave 4 added the FOURTH attribution
bucket (HARNESS DEFECT) and the REPRESENTATION rung; countable completion conditions on PURSUITS as
well as brakes; per-item combat floors; demote-with-quantified-obedience; timeout triage; residual →
deck construction; the negative-framing ruling (positive default + two sanctioned constructs);
winning-template transcription; the tell audit; turnaround-preservation freeze mode; the stopgap
lifecycle triple; two-gear identity. All held up in wave 5 and remain in the method below.

## Superseded wave-4 synthesis notes (retained only as lineage)

Unified from seven independent deck-agent revisions (decks 44, 109, 110, 131, 133, 135, 140)
of the wave-3 skill, all mining the same wave-4 corpus (12 games/deck across two runs). Wave 4
is the wave where the wave-3 fixes LANDED and the failures moved: cast-nothing paralysis fell
sharply (deck110 turned around 0/6 → best-in-pool; deck109 8/12; deck133 affordability 75%→36%
cast-nothing), and the residue is a different, more structural set of problems — harness/engine
defects the reviewer must not blame on the pilot, combat-seam passivity the cast-floor never
reached, menu-shaped (index-vs-value) interface traps, and pursuit rules that over-ran their
purpose the same way wave-3's brakes did. What changed and why:

**Merged (proposed independently by 3+ agents — the convergent core):**
- *A FOURTH attribution bucket — HARNESS DEFECT* (the pilot chose right and the system did
  something else: reply-parser desync, engine no-op, misrender) added to the Step 2 taxonomy,
  because the whole corpus is now clean enough that engine bugs are a leading loss source
  (109, 140, 44). Only *bad play* is the guide's to fix; harness defects go on the bug ladder.
- *Per-record chosen-vs-happened / plan-vs-consumed diagnostics* using the new `chosen_text`
  field: desync detection, engine-no-op detection, and the systematic numeric-offset trap
  (109, 140, 44). These are how you SEE the fourth bucket.
- *A REPRESENTATION rung on the attribution ladder* — before any wording diagnosis, check
  whether the deciding fact was even VISIBLE to the executor (44, and implied by 109/140). A
  misplay whose deciding fact is invisible is a platform defect wearing a guide costume.
- *Countable completion conditions on PURSUIT and PREFERENCE rules, not just brakes* — wave-3
  made brakes carry releases; wave 4 proves a loud PURSUIT ("GET AN ISLAND", "Mountain first")
  over-runs identically and becomes the next durdle (135, 131). Every seek/prefer rule ships
  with a done-state.
- *Format/interface contracts are GENERAL-layer furniture* — the reply-format contract, the
  list-anchor, and the interface "offered = payable + it's your main phase" contract are
  harness properties identical for every deck; guides REFERENCE, never restate (109, 133, 135).
- *Teach the UI surface AS THE MODEL SEES IT for menu-shaped decisions* — for X-values, modal
  picks, damage order, transcribe the literal menu, state the index-vs-value mapping, give a
  pick procedure over the LIST, add a verify step (140, and the interface-contract-anchor form
  in 135, 133). A how-it-operates line about the CARD does not fix a menu trap.
- *Runnable per-item combat checks over posture prose* — the act-floor is PER-SEAM; "attack
  every turn" does not reach the blocker/attacker seams. Each passive seam gets its own floor
  written as a one-yes/no-test-per-listed-item procedure with a closed empty-choice exception
  (135, 131, 133).
- *Demote-and-compress with a QUANTIFIED obedience threshold, keeping the load-bearing tokens
  verbatim* — a won rule is compressed (not deleted, not kept at full prominence) once
  obedience is counted and total-or-near AND the fix lives at a stronger layer; what survives
  compression is the corrective FACT and the named forbidden SENTENCE verbatim, deleting the
  war-story, the self-citation stats, and anything the general layer now carries (109, 110,
  133, 140).
- *Timeout attribution — check WHO WAS AHEAD and split stalls from latency* — a timeout is no
  longer automatically passivity: winning-but-slow and latency-starved timeouts are not guide
  bugs (133, 140, 131). Normalize timeout counts by decision density before comparing decks.
- *Residual → DECK CONSTRUCTION* — when the same guide produces faithful-execution wins AND
  losses that share a precondition the guide cannot create (no threat drawn, color starved),
  the residual is the decklist; flag it upstream, don't write louder exhortations (131, 133).

**Adopted from single/paired agents (divergent but genuinely additive method):**
- *Winning-template extraction* — when the corpus contains a game the pilot handled correctly,
  TRANSCRIBE that sequence as a numbered procedure and tell the pilot it's its own proven line;
  transcription beats invention (133).
- *The TELL AUDIT* — before teaching any "when you see X, Y is true" inference rule, script a
  check that Y held in EVERY window X appeared; one counterexample means a false-positive the
  executor obeys off a cliff (44).
- *Verify the MECHANISM against `auto=`, including your OWN causal claims* — not just cited
  numbers: who a trigger hits, what a restriction scopes, which zone it fires in; the author's
  intuition is as unreliable as the pilot's (135).
- *Turnaround-preservation freeze mode* — when last wave's revision demonstrably worked, revise
  as a constrained diff off a copy of the deployed guide, freeze the obeyed lines, and produce
  a freeze-check (110).
- *Engine-defect BEHAVIORAL FALLBACK, never explanation* — the guide carries a positively-framed
  look-at-this fallback keyed to observable state, never "the engine is buggy" (109).
- *Stopgap lifecycle* — interface-defect defusal / tell lines are a tracked class: each listed
  in the handoff as (guide line, defect + citation, removal condition) so the platform fix
  sweeps them from every guide at once (110, 44).
- *Two-gear identity keyed to a VISIBLE board fact* for reactive shells around a narrow win
  package (131, 133). *Per-turn ROLE CHECK on board-readable facts, never opponent archetype*
  (133). *Life-as-resource needs a BUDGET keyed to the role check* (133).
- *Partial answers to bundled asks* — a new leak class now that bundled attacker/blocker
  decisions exist: the pilot answers with a subset (declares one attacker of three) (131).
  *Schema update + option-LINE counting* — the offered-vs-taken tally must count numbered
  option lines, not substrings, or it silently corrupts in both directions (135).

**The negative-framing adjudication (RULED — see Step 5, "Positive default and its two
sanctioned exceptions"):** the standing lesson "negative framing bleeds" is A/B-proven again
this wave (deck131: draw casts 0.17/game → 1.0/game after a negatively-framed clause was
rewritten positively, held across 12 games; deck44's "do NOT save them" suppressed its
neighbor). Four reviewers (44, 131, 133, 140) converted DO-NOT sections to positive form;
deck110 argued a reasoned defense of two negative constructs, deck135 flagged the tension.
Ruling: POSITIVE is the default for all operative and terminal-recap content; exactly two
negative constructs survive, tightly constrained (the quote-and-forbid FALSITY-FACT and, only
where no positive form is as sharp, a single bare NEVER on the worst leak). The former
terminal "DO NOT" section becomes a positive situation→play recap. This resolves the dispute
once so every guide is consistent.

**Task-consistency note (updated after the wave-4 general-strategy revision):** the general
system prompt has now absorbed both the wave-3 stopgaps (affordability/auto-tap contract, rules
facts, closed-exception act-floor, hold-release valves, stale-plan grounding, anti-deferral,
mulligan default, guide-beats-priors precedence) AND the wave-4 additions: the runnable combat
block (per-creature attack tests, punisher recompute, survival-first blocking at 8), the
first-strike fact, the race re-check, the interface-contract completions (timing anchor,
bundled-declaration contract), plan-FROM-the-list + numeric plan-binding, the activation
count-brake + ping arithmetic, and two core-layer STOPGAPS (counterspell offer-is-the-signal
tell; cast-happened anchor). The reply-format contract was routed to the code-appended
reply-protocol layer, NOT the prompt. Inputs #2 carries the full updated inventory; the
restatement ban covers all of it. Where a deck still shows a general line LOSING locally, the
guide reinforces it tied to the named card — the wave-3 rule, unchanged. The Output/handoff
section now also carries the four-layer routing discipline, the single-seat-can't-cut-core
epistemics, and the threshold-reuse rule this wave's core merge proved out.

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
  read as dealing 0; "it is the opponent's turn" while a sorcery-speed cast sits in the list.
  A strategy rule built on an uncorrected misconception never fires — the guide must state
  the corrective FACT flatly, and the strongest form ties it to the visible list (see Step 5).
- **Brake AND pursuit over-generalization / obedient absolutism.** A weak executor does not
  scope a caution; it universalizes it — "leave one land untapped to equip" becomes "never
  spend mana"; "save your removal" becomes "never cast anything." It also universalizes a
  PURSUIT: "GET AN ISLAND BY TURN 3" becomes "spend every turn fetching Islands while
  creatures sit home" (deck135: up to 16 of 59 decisions in a game hunting a land the deck
  owns two of). And it OBEYS your hard rules into losses: an absolute with an unstated
  exception is a trap you set yourself (deck109's pilot declined to burn a Young Pyromancer,
  quoting the guide's "burn goes face" rule, while Pyromancer tokens killed it). Every DON'T
  AND every "get/prefer X" you write will be applied far beyond the situation you meant.
  Design accordingly (see Step 5).

Write imperative, front-loaded, low-branching, concrete, evidence-driven. If a sentence
doesn't change which number qwen picks, cut it.

## Step 0 — Revising an EXISTING guide (the wave loop)

When a deployed guide was in play during the corpus you are mining, do this before
anything else. The logs no longer show qwen's raw defaults — they show the qwen×guide
interaction, and the guide itself is a suspect.

- **Attribute each misplay to a specific line — but climb the ladder in order.** Missing,
  wrong, or too weak to override a general-prompt prior — AND, before settling on any WORDING
  diagnosis, check the REPRESENTATION rung: could the executor even SEE the fact the correct
  play depends on? Pull the full `prompt` for 2-3 instances of the misplay and verify the
  deciding fact appears somewhere the executor attends to (CURRENT SITUATION or the option
  line — the log tail barely counts). A misplay whose deciding fact is invisible is a platform
  defect wearing a guide costume: rewording the guide will not fix it, and the wave will
  re-diagnose it forever (deck44 "died with counters in hand" was diagnosed as hoard-y wording
  in wave 3; wave 4 showed the STACK was never surfaced — one prompt-read a wave earlier would
  have caught it; wave 5 shipped an ON THE STACK section, the representation fix, and that residual
  is now a MANA-DISCIPLINE leak instead — see the counter-tell retirement in Inputs #2). **A fact
  that lives ONLY in the game-log TAIL is a special case of invisible: it is present but functionally
  hidden — a weak executor skims the tail and will FABRICATE a confident false reading over it
  (assert a resolution that never happened, e.g. "the spell moved to the battlefield"), distinct
  from expressing uncertainty. When the reply asserts a specific false event the tail contradicts,
  the fix is REPRESENTATION — a first-class CURRENT SITUATION line and/or a labeled option — NOT a
  third copy of the tell (deck44: the counter tell was present in BOTH core and guide and the pilot
  still fabricated over the tail; the shipped ON THE STACK line is the template for this class).**
- **Grep the replies for ECHOES of the guide's own phrasing.** When qwen QUOTES a guide
  sentence in its PLAN while misplaying, the guide caused the play; that is a guide bug, not a
  model bug (it also proves your exact wording propagates — phrasing has leverage in both
  directions). A rule the executor cites while misplaying is a rule to rewrite or delete.
- **Audit last wave's rules against the new logs: did each one get FOLLOWED? — and DEMOTE the
  winners precisely.** A rule the new corpus shows OBEYED has done its job; demote it, but this
  is a three-part move, not deletion-lite:
  1. **Quantify obedience** (violations / opportunities: 0 delusion instances in 174 casting
     asks; 67/67 combats attacked; cast-nothing 75%→36%). Demote only when obedience is
     total-or-near AND the fix now lives at a stronger layer (a representation line like
     `Mana available: N untapped sources`, or the general prompt) so the guide is no longer the
     only defense. Keep FULL prominence while either is unmet.
  2. **Keep the load-bearing TOKENS verbatim** — the corrective FACT (what to look at) and the
     NAMED FORBIDDEN SENTENCE ("the thought 'I have no mana' is always false when the cast is
     listed"). Those exact words are what the executor pattern-matched into compliance; they
     are the active ingredient. Cut the explanation, the war-story, and the self-citation stats
     ("last run you picked Cast nothing 76 of 101 times" — that describes a pilot that no longer
     exists).
  3. **Delete — not compress — everything the general prompt has since absorbed** (check the
     CURRENT general prompt line by line against the rule first), and give the vacated slot to
     the new biggest leak. Record in your handoff WHICH sentences you kept and why, so the next
     reviser doesn't erode the kept core to nothing over successive waves.
- **Audit last wave's #1 RULE specifically for OVERSHOOT.** The louder a fix, the more the
  executor over-applies it; a cured leak often returns INVERTED. Count decisions spent on the
  fixed behavior per game vs. decisions spent on the win condition — a pursuit or brake now
  running past its purpose is this wave's leak (deck135's Island fetch; deck110's re-equip loop).
- **Overshoot is only ONE of the two ways a fix moves the leak — also check for DISPLACEMENT.** A
  rule can be OBEYED, correct, and non-overshooting and the deck STILL loses, because the leak moved
  to the ADJACENT seam — usually the one UPSTREAM that FEEDS the fixed seam. Before concluding a loud
  rule overshot, tally the fixed behavior's success AND count how often the seam even had good
  material to work with; if the seam is healthy but STARVED, the fix landed and the new leak is
  upstream, not an overshoot (deck135 wave 4 predicted the attack floor would overshoot into suicide
  attacks; wave 5 it was obeyed 13/13 with zero suicide losses, but only 17 windows in 6 games ever
  offered a real creature — the loss lived at the CAST seam that never deployed a body). A review that
  only hunts overshoot-of-the-loud-rule re-polishes a working rule and misses the real lever.
- **A rule that never FIRED in the new corpus** (its misplay no longer occurs) is a candidate
  to DROP: stale cautions aren't just wasted attention, they can reinforce the wrong posture
  (deck131's "don't counter into an empty stack" was backing the hoarding).
- **Expect the failure to have MOVED downstream, and re-audit the fixed chain END-TO-END.**
  Fixing the blocking action exposes the next bottleneck in the same chain. Concretely: follow
  the SAME card/action through the logs all the way to its effect RESOLVING on the board — cast
  happened is not effect happened. Verify each stage: offered → taken → parameters set correctly
  (X, mode, targets) → resolved with the intended magnitude (deck140's render fix made Rakdos's
  Return get CAST 0/26→~17, but the very next stage — X announcement — blanked it to X=0).
- **Suspect your own defaults.** Any default the old guide prescribed is a candidate CAUSE of
  the new failure, because qwen follows it more literally than you meant. Re-derive the deck's
  dependencies from scratch each wave.
- **TURNAROUND-PRESERVATION mode — when the previous revision demonstrably WORKED** (the deck's
  record or its key metric turned around), revise as a constrained diff, not a rewrite. The
  stakes are asymmetric: deck110 is the pool's proof that a "faithful" full rewrite took the
  strongest deck to 0/6. (1) Before drafting, list the guide lines the logs show being OBEYED —
  echoed in winning PLANs, or whose target misplay vanished — and FREEZE them: they may move
  position, their wording does not change. (2) Draft by editing a COPY of the deployed guide,
  never from a blank page. (3) After drafting, produce a FREEZE-CHECK: for every frozen line,
  show where it survived (verbatim / moved / demoted-per-above) — a frozen line you cannot point
  to is a regression you just wrote. (4) Aim NEW text at the new leak only; if a section reads
  awkward but the logs show it landing, it stays awkward.
- **OBEYED-BUT-LOSING mode — when the RECORD dropped but the obedience metrics are HIGH** and the
  losses attribute to deck construction / variance (opponent life at death near 20 across most losses;
  the wins/near-wins are the guide working as written), treat it like turnaround mode, NOT like a
  failure to fix: VERIFY obedience first (attack participation, on-curve casts, brake/pursuit
  adherence), then freeze the obeyed lines and spend the wave only on the one residual model-fixable
  lever (deck131 wave 5: the underused non-combat engine), routing the record itself to a
  deck-construction flag. A falling record on n≤6 against a harder opponent slate is variance, not
  evidence the guide broke — and a loud rewrite churns the obeyed lines and risks deck110's
  faithful-rewrite→0/6 outcome in reverse. Distinguish this from turnaround mode (which keys on
  IMPROVEMENT); both forbid the reflexive rewrite a bad short record tempts.
- **Keep what worked, verbatim where possible** — churn in lines qwen already obeys is pure risk.

## Inputs to read, in this order

1. **The decklist** — `bin/Res/ai/baka/deckN.txt`. Format: `#NAME/#DESC/#HINT` headers,
   then `CardName (SET) * qty`; bare numbers are basic-land ids; the SET code is
   irrelevant. This tells you the archetype's intent.
   - **The `#HINT` lines are for the OTHER (heuristic Baka) AI, not qwen.** Read them for
     the designer's intent and any encoded combo/attack rule, but note where a hint would
     MISLEAD this deck's ideal play. Don't propagate a hint blindly.

2. **The general system prompt** — `bin/Res/ai/gpt/system_prompt.txt`. EVERY deck already
   gets this, so READ THE CURRENT VERSION. As of the wave-5 revision it carries, as UNIVERSAL
   priors and rules facts: decide who's the beatdown PLUS a per-turn race re-check on
   board-readable damage-flow facts (their damage landing while yours isn't = losing the race,
   switch to defense); act every turn under an explicit act-floor ("Cast nothing" allowed only
   via a closed exception list) — now completed with the HIGHEST-IMPACT rule (the floor is
   satisfied by your best legal play, not the cheapest: deploy a threat before a same-turn
   value/ramp/dig/discard spell); deploy your win conditions; instant-vs-non-instant timing WITH
   release valves on holding, and reactions now keyed to the SHIPPED ON THE STACK section (each
   pending spell named with controller/targets) + labeled counter options ("can target on the
   stack: <name>") — the wave-4 offer-is-the-signal / log-tail tell is RETIRED (see below) — PLUS
   the mana-discipline fact (a response window opens only while a response is PAYABLE, so a seat
   that taps out on its own turn gets no window; hold reaction mana open); choose targets by IMPACT;
   activation gas AND brakes — now including the once-per-turn same-activation brake with the
   two-power-numbers perception cue, and the ping arithmetic (N damage targets a creature only
   when N ≥ its toughness, else face); cards-vs-tempo; a COMBAT block that is now runnable
   per-item, not posture: the free-case attack floor, the per-creature three-test attack check
   with a closed "no attackers" exception, the attacker-punisher recompute rule, lethal math run
   twice, the reverse survival check, and survival-first blocking at the 8-life threshold with
   the named-gain/rules-text-check block procedure; the mechanics facts (attacking taps
   creatures not lands, summoning sickness is never a casting veto, X is chosen at cast, hybrid
   pays either color, FIRST STRIKE deals first and kills before damage back); a mulligan
   land-count default; the interface contract (every listed action is legal AND payable, lands
   auto-tap, empty pool is normal, offered cast = YOUR turn and YOUR window, lists are complete,
   attacker/blocker replies are the COMPLETE declaration, listed ≠ recommended); stale-plan
   grounding (board beats plan, still-offered = not done) plus the cast-happened `hand -> stack`
   verification anchor (a CORE-LAYER STOPGAP for the engine no-op bug); anti-deferral ("next
   turn" = now); plan/number binding PLUS plan-FROM-the-list (a missing card is never a reason
   to choose "Cast nothing") and numeric plan-binding (the option TEXT must contain the exact
   number the plan names); cost paying incl. prompt fetch-cracking; and a PRECEDENCE clause —
   the deck guide beats the general priors on conflict. **Wave-5 authors: do NOT restate the new
   combat block, the interface-contract completions, or the plan-from-the-list line — the
   restatement ban covers them now.** Stopgap status changed this wave: the counterspell tell's
   removal condition (stack surfaced in CURRENT SITUATION) is now MET — it is RETIRED from core and
   replaced by the ON THE STACK pointer + mana-discipline fact, so DELETE every guide copy of the
   counter-tell and teach reading ON THE STACK instead. ONE core stopgap remains: the cast-happened
   `hand -> stack` anchor (dies when the Hellrider-class engine no-op bug is fixed — still LIVE this
   wave, deck109 confirmed a clean A/B, so KEEP it and any guide fallback keyed to it). Never
   duplicate a live core stopgap in a guide, and drop any guide copy of a defense the core has since
   absorbed (plan-from-list copies shed now). The reply-format contract is NOT in the prompt body —
   it routes to the code-appended reply-protocol layer; a guide carries it only while that layer
   lacks it, tagged interim. **Your guide LAYERS ON TOP — never RESTATE any of this; add only
   what is specific to THIS deck.** The general guide states PRINCIPLES; a weak pilot won't apply
   an abstract principle to the card in front of it, so your per-deck job is the concrete
   INSTANTIATION: it says "activate your permanents" — you say "EQUIP Cranial Plating turn 3,
   keep {1} open." Litmus test: if a line you're about to write is already true of EVERY deck,
   the general guide has it — cut it and write the named-card version instead.
   **The same layering applies to the INTERFACE and REPLY-FORMAT contracts.** The reply-format
   contract ("first character of the reply is the option number, bare number on its own line, no
   option text or mana costs before it, then PLAN:"), the list-anchor ("only cards in the
   numbered list are castable NOW; if your intended card is not listed, pick the best listed card
   and say the intended one isn't castable"), and the interface-contract anchor ("when Cast
   <card> is in your list, the engine has already checked — you CAN pay and it IS your main
   phase") are properties of the HARNESS, identical for every deck. They belong in the GENERAL
   prompt, each stated once with a RIGHT/WRONG example pair (the wrong reply-format example being
   an echo of option text — `2. Cast Hellrider {2}{r}{r} (3/3)` — since echoed costs/stats are
   exactly the digits the parser mis-grabs). A per-deck guide carries any of these only as an
   INTERIM stopgap while the general layer lacks it, tagged for removal — seven copies of
   interface rules is the restatement failure this section bans. Flag such lines in your handoff.
   **One caveat to the no-restatement rule:** a corrective fact or behavioral guard the general
   prompt ALREADY carries earns a deck-guide line only when the NEW corpus shows the general
   statement still LOSING to a local pattern — then reinforce it tied to the specific card and the
   exact recurring wrong thought. A recurring misconception the general prompt does NOT yet cover
   may be countered in the deck guide as a stopgap — flag every such line for promotion so it can
   later be removed from every guide at once.

3. **Card text from the ENGINE, not your memory** — for every non-basic card, pull its `text=`
   and `auto=` lines from `bin/Res/sets/primitives/*.txt` (grep by `name=` across `mtg.txt`, and
   also `planeswalkers.txt` / `borderline.txt` if present). **This is what the engine actually
   enforces and shows the AI. Oracle text you remember, the printed cardface, and the engine
   `auto=` script can all DISAGREE — the script wins.** Verify against it the exact numbers you
   will cite AND every MECHANISM claim: who a trigger hits, what a restriction scopes, which
   zone/step it fires in. This applies to your OWN analysis in findings, not only guide sentences:
   an author's rules intuition and the executor's stated beliefs are both unreliable, and adopting
   the pilot's (or your own) mechanical story without reading the `auto=` ships a guide rule
   teaching a false exemption (deck135's findings claimed Diamond Faerie "dodges Lightmine" as a
   flier — Lightmine's `auto=` damages ALL attackers). Litmus: for each "X avoids / only affects
   Y" claim in your draft, point to the `auto=` token that says so. Note hybrid mana ({B/R},
   {U/B}) is castable with EITHER color, so a "gold" card may be mono-color-castable in this deck.
   Use `/usr/bin/grep` — bare `grep` here is a wrapper that can silently drop matches.

4. **The game logs** — the deck's self-play translogs, JSONL, one line per decision qwen made
   piloting THIS deck: `~/.Wagic/ai/gpt/selfplay-runs/<run>/*-<deck>-*.jsonl`. Fields:
   `kind` (ask|priority|attackers|blockers|gameend), `prompt` (the FULL board + numbered legal
   actions qwen actually saw), `reply` (its text — the PLAN/reasoning), `choice` (the index it
   picked), `chosen_text` (the RESOLVED text of the chosen option — use it directly, don't
   re-derive from `choice`), and `turn`, `phase`, `my_life`/`opp_life`, `latency_ms`. (`options`
   is just a count — ignore it.) **This is your primary evidence.**

## Step 1 — Read the deck: archetype, engine, role, redundancy

Answer these before writing a word — they are the guide's spine:

- **Archetype & clock.** Aggro / tempo / midrange / control / combo / prison? How does it
  actually kill? Roughly how many turns is its clock ("kill by turn 4-6")?
- **The win engine / win condition.** Name the specific 1-3 cards that end games and the exact
  interaction that makes them win. A weak pilot will never find these unaided — the guide's spine
  is "set up, protect, and deploy the payoff." Guides teach the DECK'S engine, not general Magic.
- **Role — as a per-turn ROLE CHECK keyed on BOARD-READABLE facts, never on archetype.** State
  the DEFAULT posture in one line, then the condition that flips it, keyed on facts the pilot can
  read off the CURRENT board and life totals — "their creatures are hitting you (your life falling
  while theirs isn't)", "they have flyers you cannot block", "their board is empty" — NEVER on the
  opponent's archetype, which the pilot cannot classify ("faster all-in aggro" is a classification;
  "flyers you cannot block" is a line in its prompt). Give each role its own numbered action line,
  say the check reruns EVERY turn, and close with the invariant that survives the flip ("you still
  cast a creature every turn in BOTH roles") so the switch can't read as permission to go passive.
  **Bind each role's action line to the DECISION KIND that carries it.** A defender role that only
  changes BLOCKING behavior does nothing for a deck whose stabilization actually happens at the CAST
  seam — the pilot already blocks when asked; it never builds the board to block WITH. If the losing
  pattern is "raced while durdling on setup," the role flip must reorder the CAST priority ("cast a
  body before any fetch/dig this turn"), not just add a blocking rule (deck135 wave 5: the defender
  role-check fired the right condition and the pilot blocked correctly when asked, yet still got run
  over because between blocks it spent turns fetching and digging instead of casting the next blocker).
- **Two-gear identity for reactive shells around a NARROW win package.** When few cards actually
  win and the rest is a support shell, the posture is decided less by the matchup than by which
  half of the deck was DRAWN. Write identity as TWO GEARS switched by one fact visible in the
  pilot's own prompt ("is Guttersnipe or Young Pyromancer on your battlefield or in your cast list
  right now? YES → race gear; NO → dig gear"), and write the passive gear as actions per turn
  ("cast draw spells to find the package, spend the mana"), never as waiting. A single-identity
  guide ("you are the beatdown in every matchup") is a lie in half the deck's games and the pilot
  durdles incoherently in exactly those games.
- **Redundancy vs scarcity — MEASURE it.** Count the deck's copies of each effect. A deck with
  12+ removal spells can spend them freely; a deck with one bomb must protect it. Redundancy flips
  the correct advice, so measure before you prescribe.
- **Castability / color dependency.** Count the sources of each color; list which spells are
  STRANDED without each color; verify what each search effect can legally find (Windswept Heath
  cannot fetch an Island; Into the North can). If one color gates half the deck, the guide's
  fetch/land-drop rules must prioritize it explicitly — WITH a countable completion condition
  (see Step 5), because an open-ended "get color X" over-runs into a durdle.
- **The curve / sequencing that matters.** What it wants to do turns 1-2-3-4, plus any hard
  ordering ("payoff before the cheap spells," "land before spells to keep mana open").

## Step 2 — Mine the logs: find the deciding lever (this is where the value is)

**FIRST: the EXECUTABILITY AUDIT.** Before judging any play, establish which decision surfaces
the harness actually gives this deck: tally `kind` counts per game. Deck131's wave-3 corpus had
ZERO opponent-turn `priority` windows in 5 of 6 games — its guide's whole spine ("hold these, act
on the opponent's turn") was structurally impossible, and its 12+ reactive instants were near-dead
cards. If the windows a plan needs rarely arrive, the guide must not build on them — demote those
cards explicitly and route the deck into proactive lines. "Keep mana open" converts directly into
stalled turns when no window ever consumes the mana. For every posture-level rule you draft, ask:
which `kind` of decision, at which phase, would carry it out — and does that decision actually
appear in the logs? A rule the executor cannot execute displaces the rule you should have written.

**SECOND: the HARNESS-DEFECT SWEEP (required — the corpus is now clean enough that engine bugs are
a leading loss source).** Before scoring ANY play as a misplay, run these per-record checks; each
resolves to fact, not guess, from the stored fields:
- **DESYNC (parser mis-grab).** Compare the first standalone integer at the head of the `reply`
  against the recorded `choice` / `chosen_text`. Any record where they differ is a harness defect,
  never a decision — the parser grabs a digit from echoed option text (`{2}{r}{r}`→2, `(3/3)`→3,
  "life 2"→2). Collect them ALL before judging play quality (deck109: 8 desyncs across 5 games, one
  game-losing — exact-lethal burn consumed onto a 1/1 instead of the face).
- **ENGINE NO-OP (chosen-many-times, HAPPENED-never).** The same action selected repeatedly (and
  correctly parsed) while game state never reflects it. Anchors, all in the record: (a) `Mana
  available:` is IDENTICAL before and after the "cast" (a real cast spends mana); (b) the GAME LOG
  never shows the card's `hand -> stack` transition (every real cast prints `hand -> stack` then
  `stack -> battlefield`); (c) the card is still in the hand line next decision. Two+ consecutive
  no-ops of the same pick = engine bug: stop counting it as passivity, find a game where the SAME
  action resolved (the A/B repro), and file it (deck109 "Cast Hellrider" no-op'd 16× then 43× while
  the model did exactly what the guide ordered; louder CAST text is what caused the 43× loop).
- **NUMERIC-OFFSET TRAP (intent betrayal on menus).** For every NUMERIC menu (X values, quantities,
  damage split), compare the number the PLAN says it wants against `chosen_text`, across ALL such
  decisions. A SYSTEMATIC offset (chosen = intended − 1; or collapse to option 1) is an interface
  trap, not a Magic misjudgment — the fix is menu-space teaching (Step 5) or an upstream menu
  change, never strategy prose. One mismatch is noise; the same offset in 15/17 decisions is the
  deciding lever (deck140 reasoned correct X in its PLAN then replied the value as the index). Two
  more shapes to test when a numeric pick misses but the value/index offset does NOT fit: **(i)
  MENU-ADJACENCY CARRYOVER** — the chosen index equals the PRIOR decision's chosen index, carried onto
  a different menu that armed right after it (deck140 vs110 seq74→75: a bare "2" from the cast menu
  landed on the next X menu → X=5 not 6). Usually low-frequency and immaterial; the fix is a per-ask
  reset / distinct second-menu labeling (engine layer), NOT a guide line — do not over-weight one
  instance.
- **PLAN/CHOICE MISMATCH (intent collapse).** The reply's PLAN names an action that IS a numbered
  option, but `choice` is a different, usually passive option, AND head-int == choice (rule out DESYNC
  first, and rule out plan-diverges-from-legality — here the planned card IS in the list). The model
  betrays its own correct plan at the number, not the parser and not an unaffordable-plan reconcile
  (deck44 e8528 s26/s49: PLAN "I will cast Arcane Denial to counter Wrath of God," options `1. Cast
  Arcane Denial / 2. Cast nothing`, choice = 2; the board was wiped and the game lost). It is only
  weakly guide-mitigable (labeling the option so the passive line reads worse than the action); the
  durable fix is representation/reply-protocol. Attributing it to "passivity" would ship a louder
  act-floor against a model that already stated the correct action — distinguish it in findings.

These feed the ATTRIBUTION taxonomy: do not theorize about qwen's play — read what it did, then
separate **FOUR things a guess would blur: bad play vs. never-had-the-option vs. bad hand
(variance) vs. HARNESS DEFECT** (the pilot chose right and the system did something else — desync,
no-op, misrender). The `prompt`, `reply`, `choice`, and `chosen_text` fields together resolve all
four as fact. **Only bad play is the guide's to fix;** harness defects go on the engine bug ladder
with repro citations (and get at most a Step-5 mitigation); variance and no-option are not yours.
The old three-bucket taxonomy structurally ASSUMES the system executes the model's pick — under it,
deck109's review would have shipped a completely wrong guide (louder Hellrider text, anti-self-
targeting rules) against non-problems.

**Extract, don't eyeball.** Files are large; script a compact digest over the JSONL that prints,
per decision: the `kind`, `turn`, `phase`, life totals, hand, both battlefields, the numbered
options, the `chosen_text`, `latency_ms`, **and the `reply`/PLAN text**, plus the DESYNC flag
above. A ~30-line Python loop does this. Sample ACROSS games and all decision kinds. Two parsing
gotchas that cost reviewers time (`options` is still an int COUNT, so option/attacker lines must be
regexed out of the `prompt`): for "was card X on the BATTLEFIELD at decision D," isolate the `Your
battlefield:` line specifically — splitting on `--- CURRENT SITUATION ---` and searching the remainder
also matches `Your hand:` and the carried PLAN, giving false "on board" reads; and for attacker
participation, extract the engine's `A#.` offered list (see the attackers/blockers bullet), never the
battlefield.

**Read the REPLY text, not just the choice — mine the stated WHY.** The choice stream tells you
WHAT went wrong; the reply tells you WHY, and the why decides which fix works. Grep replies for
recurring false beliefs ("no mana", "cannot cast", "my lands are tapped", "it is the opponent's
turn", a card claimed in the wrong zone, an ability claimed on a card that lacks it) and CHECK
each against the SAME record's printed board — a stated reason that contradicts the prompt's own
board is a systematic misconception, and the fix is a flat corrective FACT tied to the visible
list, not a strategy rule (deck133: 61 replies claimed "no mana" while untapped Swamps sat on the
battlefield). Also hunt refused-legal-actions whose stated reason is a rules error (hybrid "needs a
Swamp"; an offered cast declined as unaffordable — the engine only offers PAYABLE actions), and
plan/number mismatches (reply says "cast X now" while the chosen option is "Cast nothing"). Tally
recurring phrases; one-offs are noise.

**Tally offered-vs-taken per card — required, and count OPTION LINES, not substrings.** A short
script counting, per key card, how often "Cast <card>" (or the key activation) appeared as a
NUMBERED OPTION LINE (`^\d+\.\s`) vs. was chosen, plus whether the card ever reached hand. Count
ONLY the numbered option lines — card names recur in the narration, the carried PLAN, and
battlefield lines, and substring counting silently corrupts the tally in both directions (deck135's
first digest produced garbage "Force 6/48"; the option-line count gave the wave's headline, Diamond
Faerie offered 22 / taken 0). Distinguish "Cast <card>" offers from activation/targeting mentions
of the same card. This is the single most decisive diagnostic and it catches upstream causes too (a
win engine "never drawn" traced to draw spells never cast, not variance). Pair it with the
CAST-NOTHING TALLY (fraction of casting decisions where qwen chose "Cast nothing"). For a menu
trap, add a REALIZED-MAGNITUDE column (offered → taken → parameters correct → resolved).

**THE TELL AUDIT (required for any inference rule you intend to teach).** Before writing any rule of
the form "when you see X, Y is true," scan the corpus for EVERY window where X appeared and confirm
Y held in all of them — script it, don't sample. One counterexample means the tell trains a
false-positive the executor will obey off a cliff (it cannot judge exceptions). Record the check in
your findings ("all N counter-offer windows had a live pending spell") so the synthesis agent can
trust the rule stated as an absolute — and absolutes are the only register the executor reliably
executes (deck44 nearly wrote "offered = counterable" from two games; all 12 games' counter-offer
windows had a live `hand -> stack` spell, which is what made it safe to state absolutely).

**By decision kind, what to inspect:**
- **ask** — casting, targeting, land drops, mulligan, modal menus. Watch sequencing/tempo, removal
  fired at the wrong target or too early, whether it targets ITSELF. Check LAND-DROP picks
  explicitly when the deck has a splash color.
- **priority** — activated abilities (equip, tap-for-value, cracking fetches, mana, instants).
  **THE most overlooked seam.** Check whether the deck's engine/payoff ability is EVER activated —
  and, per the executability audit, whether these windows even arrive.
- **attackers / blockers** — is it playing its role? Two bad defaults live here:
  - **Reflexive blocking** (symmetric to "Cast nothing" at the cast seam): offered a block, the
    executor BLOCKS — "there is an attacker, I have a creature" — regardless of role. Audit every
    `blockers` record against the plan: a racing deck that blocks is spending its clock; check what
    the blocker WAS (win condition? token?) and what it blocked (a real threat? a 1/1?). A guide
    that says "attack every turn" but is silent on blocking has NOT set the blocker seam's default —
    silence there is block-by-default (deck131: Guttersnipe, the deck's whole kill, traded to a
    1/1). For any racing deck, blocking IS the non-obvious combat.
  - **Partial answers to bundled asks (under-commitment inside one decision).** Bundled asks let the
    pilot answer with a SUBSET. Count, per attackers record, creatures declared vs listed: repeatedly
    declaring one attacker while three are listed loses damage invisibly — no single record looks
    wrong, and "attack every turn" is technically satisfied by one attacker. The fix is a
    count-and-match procedure ("count the list; your reply names all of them"), not louder posture.
    **The denominator is the engine's OFFERED legal-attacker list (the `A#.` "creatures that can
    attack" lines), NEVER the creatures on your battlefield** — summoning sickness, tapped status, and
    just-created tokens legitimately shrink the legal set, and counting against the battlefield
    manufactures a PHANTOM subset-attack leak (deck131 wave 5: "declared 1 while 2-3 on board" reads
    were 100% participation once counted against the `A#.` list; vs135 T11 the reply even named a
    just-cast summoning-sick creature the engine did not offer — full commitment, not
    under-commitment). Extract the `A#.` list for attacker records the same way you extract numbered
    option lines for casts. A reply naming a creature the engine did NOT offer is the pilot committing
    fully, not under-committing.
- **target choices** — was the highest-impact target available, and did it take it?

**The single highest-value move: isolate the ONE lever.** Compare the games qwen played WELL against
the ones it LOST or STALLED and find the one repeated difference. Treat WINS as mirrors of losses
(deck135's two wins were the same deck with one variable flipped: Island fetched early → threats
deployed → attacks every turn). Audit wins with a CLOCK lens: compare earliest-possible kill vs
actual kill per game — "won anyway" hides the same leak that loses the close games. **And when the
comparison surfaces a game where the pilot handled the problem situation CORRECTLY, TRANSCRIBE that
winning sequence into the guide as a numbered procedure, in the order the pilot executed it, and
tell the pilot it is its own proven line** ("the exact line that won you the burn matchup last
run"). A sequence the executor has already performed under this prompt stack is stronger evidence
than any theory of what it *should* do: it is known reachable from the options the engine offers,
expressible in moves the pilot takes, and compatible with its habits. Prefer transcription over
invention whenever the corpus contains the win (deck133's DEFENDER block IS its own vs109 win,
numbered 1-4).

**Treat TIMEOUT as its own outcome class — but TRIAGE it before blaming the guide.** A timeout is no
longer automatically passivity. Print final life totals AND check `latency_ms` and record count:
- **Genuine durdle-stall** — life totals frozen for 10-20 turns at normal latencies. The passivity
  failure in pure form; a guide that shortens the game (kill by turn 5-6) is the fix. Wall-clock
  economics make it strategy: each decision costs inference time and games hit the cap at only
  ~35-50 decisions, so every durdle burns clock.
- **Winning-but-slow** — the pilot AHEAD at the cap (opponent at low or negative life). Different
  diagnosis, different owners: opponent lifegain/reset (Elixir of Immortality), inference latency ×
  decision count, engine adjudication gaps — none of which a guide line fixes. Adding anti-passivity
  pressure here REGRESSES the guide against a problem it doesn't have (all three deck133 timeouts
  were the pilot ahead).
- **Latency-starved** — a LOW-turn timeout (game ended mid-progress) with high per-decision latency
  (avg 15s+, spikes to the HTTP timeout) = the run's concurrency artifact, NOT a play defect; report
  it as infra, write nothing.
- **Normalize by DECISION DENSITY before comparing decks.** A control deck generates 3-4× the
  decisions per game of an aggro deck, so identical per-decision latency gives control archetypes
  disproportionate wall-clock exposure — a raw timeout column penalizes the archetype, not the
  piloting.

**Attribute the RESIDUAL — bound what a guide can fix, route the rest upstream.** When the same
deployed guide produces both faithful-execution WINS and losses, diff them: if the wins are the
guide working as written and the losses share a precondition the guide CANNOT create (no threat
drawn, color starved), the residual is DECK CONSTRUCTION, not guide or model. The deliverable is an
explicit upstream flag (manabase skew, threat density) plus guide text that teaches the deck to play
the hand it was actually dealt (the two-gear identity, Step 1) — not louder exhortations to execute a
plan whose pieces aren't there. Fast discriminator: opponent life at the moment of death — losses
ending with the opponent near 20 mean the deck never had a plan running; check the hand/draw stream
before blaming the pilot (deck131: 2 textbook wins; 8 losses ended opp 13-27, traced to 6 creatures
+ 22 lands vs an all-red payoff suite → a construction flag, not stronger beatdown prose).

**Diagnostic patterns to hunt for (weight the guide toward the ones that recur):**
- **Offered-many-times, passed-every-time** — a capability qwen doesn't understand it should use
  (the offered-vs-taken table finds these).
- **Chosen-many-times, HAPPENED-never (engine no-op)** — see the harness-defect sweep above.
- **False-belief vetoes.** A recurring reply-side misconception silently vetoing legal actions.
  Distinguish from strategic passivity — the fixes differ (corrective fact vs. directive).
- **Cast-nothing / durdle rate** and the cards NEVER cast across all games.
- **Hoarding — answers/threats piling up UNUSED in hand** while the opponent stays alive. Watch the
  general prompt's hold-instants prior METASTASIZING: "hold interaction" generalizes in a weak model
  to "hold everything," and a guide's own HOLD line feeds it.
- **Deferred-plan loops / stale-PLAN poisoning.** The PLAN line is carried into the next prompt, so
  "I will attack NEXT turn" gets re-affirmed forever. Hunt for the same "next turn" intention in 3+
  consecutive plans while the action stays untaken, and for plans carrying hallucinated state that
  beats the visible board.
- **Plan-diverges-from-legality-at-the-moment-it-is-written.** The pilot plans an unaffordable cast
  ("PLAN: Cast Gray Merchant immediately" with 4 lands, Merchant costs 5, so it was never offered)
  and then selects "Cast nothing" rather than reconcile the plan with the menu. The fix is the
  list-anchor (Step 5 / general-layer promotion).
- **Waste loops** — a repeatable-cost action re-taken every turn for no new effect (re-equipping an
  already-equipped creature; untapping a summoning-sick creature). Burns mana AND clock — and a lone
  state-recognition cue does NOT hold when the engine re-offers the completed action every window
  (deck110: 47 equips, ~12 justified, with `(6/2) (printed 2/2)` printed on the very board). See the
  Step-5 three-part fix pattern.
- **Pursuit over-run** — a "get/prefer X" rule spending turns past its purpose (Step 0 overshoot).
- **Failure to close — end-state sweep.** Opponent life PLATEAUED for many turns while qwen had gas.
- **Missing lethal.** Sum a turn's potential damage; check whether qwen had lethal and didn't take it.
- **Reactive spells fired into nothing** — a counter on an empty stack, a trick on an empty board.
- **Wrong modal/menu picks** — color choices, X values, overload-vs-single (run the numeric-offset
  check before calling these misjudgments).
- **Inconsistency** — the SAME decision made differently across games means qwen has no stable rule.
- **Generic-prior misfires** — a universal prior WRONG for THIS deck. Name it and override it.
- **Cost-line misreads.** Any action whose cost LOOKS bad but is routine (fetch sacrifice, life
  payment, phyrexian) needs "this cost is the point — always pay it."
- **Illegal-plan errors.** Plans built on rules-forbidden actions (attacking with a summoning-sick
  creature "for 9"). A one-line mechanics restatement tied to the specific card earns its place.
- **Subtle leaks in decks qwen already plays "okay"** — burn hoarded vs thrown at the face; holding
  "bad attacks" a go-wide payoff makes correct; not knowing when to stop trading and race.
- **Never-drawn ≠ validated — but distinguish two zero-offer cases.** Zero log evidence about a card
  is VARIANCE, not proof; keep its lines proportional, don't build Rule #1 on an unobserved card, and
  don't cut it either. BUT a card repeatedly in HAND yet never castable (a color/cost the manabase
  rarely meets) is a different animal — a STANDING castability fact, not variance: its hero treatment
  in the guide is dead weight, and the evidence is the pilot's own replies declining it for a REAL
  affordability reason ("I lack white"), not the false-belief delusion the hero rule was written to
  intercept. Demote the spotlight; the correct fix is a deck-construction flag, not a louder cast
  exhortation (deck135 wave 5: Diamond Faerie was hero-worshipped off a wave-4 22/0 refusal, then
  offered 0 times this wave and the one decline was correct — a GWU 5-drop off a 2-Plains base).

**Confirm before you accuse.** When a play looks wrong, read that decision's full `prompt` to check
what was actually legal / on the stack (deck135's Force of Negation LOOKED reflexive until the prompt
showed a real Cruel Edict; deck131's priority passes were correctly reasoned shroud/mana calls; the
Island-flood losses looked like a Mountain-first rule violation until the prompt showed the Mountain
was never in hand before the turn it was played). Also check the translog for harness artifacts (the
sweep above) before attributing a misplay. Never teach against a non-problem, and never list a
"mistake" you didn't observe.

## Step 3 — Decide what goes in vs. what gets cut

Include a line ONLY if it changes a decision qwen will actually face and gets wrong (or would).
Rank candidates by **frequency × cost-to-winning:**
1. The fix for the biggest observed leak / the deciding lever (always first).
2. The corrective FACT for any systematic false belief the replies exposed — but check the CURRENT
   general prompt first: if it already states the fact, include a deck-guide line only when the new
   logs show that statement still losing locally; if it doesn't, carry the fact and flag it for
   promotion upstream.
3. The win engine and how to set it up, protect it, and deploy it.
4. Role + matchup flips as a per-turn board-readable ROLE CHECK (Step 1).
5. Target-priority and removal/sweeper-selection rules.
6. Per-card timing for the cards that decide games; a lethal-math check if the deck closes with reach.
7. Mulligan, in a line or two.

**On a REVISION pass, re-tally each existing rule against the new corpus FIRST** (this is the Step 0
demotion, applied here to the include/exclude decision): a rule the pilot now follows gets compressed
to its one-line load-bearing core keeping its early position; its explanation, cited failure stats,
and DO-NOT reinforcements have done their job and now only dilute the new #1 rule. The freed attention
IS the budget for the new wave's lever.

**Triage model-capability failures by ONE question: can the guide PRE-COMPUTE the answer so the pilot
needs no runtime reasoning?** If yes, it's guide material — do the arithmetic IN the rule ("one
attacker = 1 damage total; Viper 1/3 survives and draws you a card"). If the pilot would still have
to compute/compare/parse at runtime (degenerate output, illegal assignment syntax, arithmetic over a
quoted card it must perform fresh each time), do NOT spend guide lines on it — route it to the
model-experiments residuals list as a litmus case. Guide lines against model incapacity are dead
weight (deck135's solo-attacker Lightmine case was pre-computable → guide; the "** **" degenerate
blockers reply and the illegal double-block were not → residuals).

**CUT:** anything the general prompt already covers; general Magic theory; card-by-card tours of
filler; deep multi-turn combos qwen can't sequence; long matchup trees; hedged "it depends" prose;
rare edge cases; anything you can't turn into a "do/don't with a named card"; and (in revision mode)
inherited rules that never fired in the new corpus. When unsure, CUT — length dilutes the rules that
matter.

## Step 4 — Structure the guide (front-loaded by impact)

qwen weights early text heavily and may not deeply read the tail. Order by what most changes its
play. A reliable skeleton (adapt, don't pad):

1. **Identity + role**, 2-4 lines: archetype, how it wins, the clock, who it's the beatdown against,
   and the posture as a per-turn ROLE CHECK. **Identity prose drives behavior as much as rules do**:
   a tempo-negative self-description ("you are not fast; you win late; stabilize first") reads as
   license to do nothing. Even a slow deck's identity must be written as actions per turn ("you win
   late BY casting a threat every turn and draining in chunks"), never as patience. For a narrow-win
   shell, write the two-gear identity (Step 1).
2. **THE #1 RULE** — the fix for the single biggest leak, hammered in its own visually loud block at
   the very top: WHAT the action is, the exact option text qwen will SEE, its enabling constraint,
   and a one-clause WHY. **State it as a procedure over the visible option list, not a posture.**
   "DEPLOY, DO NOT SIT" loses to the general prompt's hold prior every time; "if any Cast option
   shows a creature, take it" does not. Exhortations always lose to a locally-plausible excuse — use
   a take-the-first-that-appears checklist with a closed exception (see Step 5).
3. **THE ENGINE / how the deck wins** — name the finishers and the exact sequence. Turn-numbered
   deploy scripts beat priority prose: "Turn 2: CAST BLOODGHAST" is executable; "deploy black threats
   mid-game" is not. Write multi-step engines as a NUMBERED mechanical sequence — and prefer the
   pilot's OWN winning line transcribed from the corpus (Step 2) over an invented one.
4. **Standing operating rules / the core action loop.** Where the observed failure lives in one
   specific phase, key the checklist to the visible phase label ("MAIN PHASE 2 CHECKLIST — run it
   every turn after combat") — it outperforms turn-generic principles the model must map onto the
   phase itself. For each decision SEAM with an observed passivity leak (casting, attackers,
   activations), state that seam's own floor as a per-item checkable procedure (Step 5).
5. **Key cards — one imperative line each**, ordered by how often each decides the game. Only cards
   with a non-obvious right line; skip vanilla creatures. Include a HOW-IT-OPERATES line for every
   X-spell and priority-seam ability the pilot provably fumbles — and where the fumble is on a
   MENU-SHAPED decision, TEACH THE MENU, not the card (see Step 5's UI-surface pattern).
6. **A lethal / math check** the pilot can run mechanically, if the deck closes with reach —
   written as a per-turn CHECK bound to a trigger the pilot meets every turn (Step 5).
7. **Combat notes** — for a racing deck this is NOT optional: it carries the blocker-seam default and
   the count-and-match attacker rule (Step 2). "Only if the deck has non-obvious combat" still holds,
   but for a racing deck, blocking IS the non-obvious combat.
8. **MULLIGAN** — the general prompt carries a land-count default, so include a line ONLY to
   SPECIALIZE it ("count Astrolabe as a land"; "ship any hand without a red source") or when the logs
   show the default losing — keyed on a COUNTABLE, never on hand "quality."
9. **DECIDING SITUATIONS** — a tight closing list of the 3-5 situations that actually decided games,
   each written POSITIVELY as "situation the executor will recognize → the correct play," recapping
   rules already stated positively above (the recap re-fires the rule at the tail; see the
   negative-framing ruling in Step 5 for why this is NOT a "DO NOT" list). Placed LAST, it has no
   positive neighbor to suppress.

**Audit for cross-seam leakage before shipping.** Every loud sentence will be pattern-matched at
EVERY decision seam it lexically fits, not just the one you wrote it for: deck44's in-game line "a
hand of counters and removal with NO creatures is LOSING" fired at the MULLIGAN seam and shipped good
openers. Reread each rule asking "which other decision kinds could this sentence match?" — and state
mulligan/blocker/priority rules in their own seam's terms.

Match the deck8 / deck44 example density and length (~one screen, roughly 40-80 lines). Longer is not
better — every extra line dilutes the early ones.

## Step 5 — Word it for the executor (non-negotiable style rules)

- **Imperative and directive.** "EQUIP the Plating." "Cast Wrath when the opponent has 2+ creatures."
  Never "consider," "you might," "weigh whether" — qwen does not act on hedges.

- **Default + narrow, enumerable exceptions** — not open judgment and not a decision tree. If a rule
  needs three conditions, qwen drops two. **Name the exceptions INSIDE your absolutes**: qwen obeys
  hard rules off a cliff, so an ALWAYS/NEVER whose exception you know about must carry it ("burn goes
  FACE — EXCEPT kill Young Pyromancer or Guttersnipe on sight"). Opponent decklists are known — name
  the actual cards it will face.

- **The act-floor comes first, above any hold language — AND it is PER-SEAM, not global.** "Cast
  every turn" does not transfer to the attackers seam: a pilot obeying the cast floor can still
  declare "no attackers" forever (deck135 had the cast floor and lost every passive game at the
  ATTACK seam; wins declared 3-8 attacks, losses/timeouts 0-1). For EACH decision kind where the logs
  show passivity, state that seam's own floor AS A CHECKABLE PROCEDURE the pilot runs over the offered
  list — one yes/no test per listed item with the answer's consequence ("a creature attacks if ANY
  of: no untapped blocker / toughness > biggest blocker's power / it trades up or draws a card"),
  plus a closed exception for the empty choice ("'no attackers' ONLY when every listed attacker dies
  for zero gain"). Posture prose ("be aggressive," "attack every turn") loses to the first plausible
  friction; a per-item test does not. Never write "pass with mana open" or "hold X" as a default
  posture — on a weak executor a hold instruction becomes the whole personality. **Name any mana-dork
  / utility creature the deck runs as an explicit EXCEPTION INSIDE the attack floor** — the free case
  ("no blocker → attack with everyone") will otherwise tap a mana source for a 1-point chip and strip
  a blocker; state where it goes instead ("keep the Druid for mana and blocking; attack with it only
  as your last body when the chip is lethal-relevant"). Minor per instance but pure downside, and the
  floor endorses it by omission (deck135 wave 5: Boreal Druid sent to attack under the free case while
  behind).

- **Prohibitions AND pursuits over-fire — prefer positive sequences, and put a countable release on
  BOTH.** The executor universalizes any DON'T ("DO NOT TAP OUT" → "never spend mana") AND any
  seek/prefer rule ("GET AN ISLAND" → "fetch Islands forever"). Rules:
  1. Say what to DO in what ORDER and let the order imply the caution: "cast your creatures FIRST;
     equip costs only {1}" instead of "don't tap out." Sequencing within a turn must be stated
     ("payoff creature BEFORE the cheap spells") — it is invisible to the executor otherwise.
  2. A brake that must exist needs (a) a SCOPE keyed to something visible on the board, (b) a numeric
     FIRE TRIGGER / release ("hold Blast for a blocker — but when the opponent is at 8 or less, ALWAYS
     fire it at their face"), and (c) a concurrent-action bound ("holding Fatal Push must NEVER stop
     you casting your creature this turn"). A brake without a release becomes permanent. Value framing
     ("premium," "save," "don't waste") trains infinite holding.
  3. **A PURSUIT or standing PREFERENCE needs the same countable completion condition, stated IN the
     rule** ("choose Play Mountain — until you have two Mountains on the battlefield; Islands after
     that"; "ONE Island or one Astrolabe in play is ENOUGH — after that, every fetch takes Forest and
     your turns go to attacking"). Key the terminator to a count the pilot can take from the printed
     battlefield, never to a judgment ("enough red"). An unterminated pursuit becomes the deck's whole
     personality exactly like a brake without a release.
  4. Never write a prohibition whose precondition depends on the pilot's OWN other behavior — it
     self-locks ("NEVER cast Gray Merchant as your only black permanent" + a pilot that never builds a
     board = Merchant never cast). Convert to a numeric GO threshold ("cast Merchant at 4+ devotion").
  5. After drafting, reread every brake AND pursuit and ask: "if the executor applies this sentence to
     EVERY decision for the rest of the game, does the deck still function?" If not, rewrite it.
  Where logs show flash creatures held forever, DE-FLASH them ("treat as a normal main-phase
  creature") — the lost finesse is cheap; the observed holding is fatal.

- **When the engine RE-OFFERS a completed action every window (equip, attach, repeat activation), a
  lone state-recognition cue does not hold — write the defense as THREE mandatory parts:** (a) a
  DONE-SIGNAL read off the board line itself as a perception procedure ("two power numbers — like
  '(6/2) (printed 2/2)' — mean the bonus is already on it; the equip is DONE"), (b) a COUNTABLE brake
  keyed to the RESOURCE that bounds legitimacy — one activation per UNATTACHED copy / per unused charge,
  NOT a flat per-turn cap — that caps the damage even when (a) misfires, and (c) a NAMED RELEASE so the
  brake doesn't over-generalize ("equip again only when the equipped creature DIED or a strictly better
  carrier arrived"). A flat "one Equip per turn" mis-fires when the deck runs multiples: with two
  Cranial Platings in play, TWO equips in one turn are correct (attach each), so the flat cap is
  simultaneously violated AND wrong — key it to unattached copies instead (deck110 wave 5). State where
  the saved resource goes ("your remaining mana belongs to creatures and Blasts") — a brake with a
  positive destination out-competes the re-offered line; a bare prohibition just loses to it again.
  (The re-offer means the temptation is re-prompted every priority window, so a cue the model must
  remember loses to an option it can see; the count brake is the only piece robust to the cue being
  misread.) **This three-part guard is a STOPGAP with a KNOWN CEILING.** When the corpus shows the
  pilot correctly READING the done-signal in its reply and STILL taking the re-offered action,
  instruction has failed at full strength — do NOT escalate wording (the exact "make it louder" trap
  the skill warns against). Record it as a representation defect (marker on the option line, or suppress
  the re-offer) and stop (deck110 wave 5 seq84: the pilot read "already attached... waste of mana," then
  equipped anyway — the clean read-then-disobey proof that no wording closes this, the second
  representation-beats-instruction confirmation after the mana line).

- **Repeatable-cost activations: write the rule per PROMPT WINDOW, not per condition.** Name each
  window where the engine will offer the activation and give the action for each ("Upkeep prompt:
  pass. Opponent's turn: pass. Your Main Phase 1 on a turn it attacks: pay it, ONCE."), and state
  explicitly that a passed option RETURNS ("it will still be offered in Main Phase 1"). A
  condition-scoped rule ("only in main phase 1 when attacking") tells the executor when paying is
  right but not what to do at the OTHER windows — it pays early anyway, fearing the window won't come
  back, and can satisfy the rule AND waste mana in the same turn (deck44 paid a Sleep-Cursed untap in
  Upkeep and again in Main 1 of the same turn — 4 mana burned in a race lost at opp 7 life).

- **Teach the mechanic qwen provably lacks — as fact, anchored to the VISIBLE LIST.** The universal
  mechanics/interface facts (offered = payable, lands auto-tap, empty pool is normal, attacking
  doesn't tap lands, X chosen at cast, hybrid mana) are IN the general prompt — do not restate them
  wholesale. Reinforce one in the deck guide only when the new logs show it still losing, and then
  the strongest corrective anchor is the INTERFACE CONTRACT restated as a property of the list tied to
  the named card: "when 'Cast Diamond Faerie' is in your list, the engine has already checked — you
  CAN pay, and it IS your main phase." This converts an unwinnable argument about the world-model ("I
  have no mana," "it's the opponent's turn") into a fact about the thing the pilot is looking at —
  prefer it over re-teaching pools/phases/turn-structure; the pilot doesn't need to understand mana to
  trust the list. Where a delusion resists flat facts, give a LOOK-AND-COUNT perception procedure
  ("count the Mountains NOT marked [tapped] — that number is your available mana"). Include
  state-recognition cues ("(5/1) (printed 1/1)" means the equipment is ALREADY attached).

- **TEACH THE MENU for menu-shaped decisions (X announcement, modal choice, color pick, damage
  order).** A how-it-operates line about the CARD is not enough when the fumble is index-vs-value. The
  pattern, all four parts:
  1. transcribe the literal menu into the guide as the model will see it (`1. X = 0` / `2. X = 1` / …);
  2. state the index/value mapping flatly ("the reply is the OPTION number, NOT the X value; option 1
     is always X = 0");
  3. give a one-step pick procedure over the LIST, phrased against a STABLE PROPERTY of the option,
     never a POSITION ("reply with the number of the line whose value is largest," NOT "reply with the
     LAST line"). The X menu was REVERSED to largest-first this wave, so the largest X is now the FIRST
     line — a positionally-phrased pick ("the LAST line") now names X = 0, the worst option. A
     value-anchored pick survives a menu reordering; a position-anchored one inverts with it.
  4. add a verify step phrased over visible text ("the line you picked must literally read 'X = <the
     biggest number shown>'"), plus one worked example of the exact observed wrong reply.
  (deck140 reasoned correct X in its PLAN then replied the value as if it were the index, collapsing 4
  kill spells to X=0; "pick the LARGEST number offered" was value-space advice for an index-space
  channel. After the reversal shipped, deck140 hit max X in 6/7 — but ONLY because the pilot ignored
  the guide's stale "pick the LAST line"; a guide line the pilot succeeds by disobeying is one tighter
  model away from a blanked kill.)

- **Prefer an engine-SURFACED number over a hand-computed one — and VERIFY its offset.** The engine
  now embeds computed values in option text (Gray Merchant renders `{right now: drains N}`). A number
  the pilot can READ beats a formula it must run, so teach the pilot to read it rather than recompute
  the guide's hand-summed version. BUT before shipping the read, verify the displayed value against the
  RESOLVED result across every instance in the corpus: engine displays can carry a systematic offset
  (Gray Merchant's `drains N` under-counts by 2 because the card itself is not yet on the battlefield
  when the option is built — verified +2 in 3/3 casts). If there is an offset, state the correction as
  a stopgap with a display-fix removal condition, or route the fix to representation. Same audit
  discipline as the index-vs-value tell; the surface is different.

- **When the PROMPT HIDES a deciding fact, teach the TELL that proves it — but a tell is a LAST
  RESORT, and a fact living only in the log tail is a REPRESENTATION bug, not a tell candidate.** If
  the board representation omits a fact the pilot needs, you can sometimes find a reliable proxy in
  what the pilot DOES see — often the option list itself, since the engine only offers legal actions —
  and write a perception procedure that derives the fact. Verify any such tell with the TELL AUDIT
  (Step 2) before shipping, and mark it as a STOPGAP in the handoff with its deletion trigger (see
  stopgap lifecycle below). CAUTION: a tell that asks the pilot to read the GAME-LOG TAIL is weak —
  the executor skims the tail and fabricates over it (Step 0 representation rung). The canonical
  example, the counter/stack tell ("the counter option appearing IS the proof; the pending spell is
  the last 'hand -> stack' log line"), was present in BOTH core and guide and the pilot STILL
  fabricated a false resolution — so it was fixed the right way, by REPRESENTATION: the engine now
  ships an ON THE STACK section and labeled counter options, the tell is RETIRED, and guides point at
  ON THE STACK. Reach for a tell only when the fact genuinely cannot be surfaced; if it lives in the
  tail, file the representation ask instead.

- **When Step 2 found an ENGINE DEFECT, the guide carries a BEHAVIORAL FALLBACK, never an
  explanation.** The real fix is an engine bug report (filed separately with the repro records). In
  the guide: do NOT mention the engine is buggy, do NOT restate rules to "correct" it, and do NOT
  prohibit the affected action — the executor must keep taking it when it works. Instead give a
  positively-framed fallback keyed to state the pilot can SEE: name the observable check ("a real cast
  shows `hand -> stack` in the GAME LOG; if your pick is still in your hand with your mana unspent…"),
  name the alternative action for THIS decision, and permit a retry NEXT turn with a cap ("one retry
  per turn"). The fallback must degrade gracefully — when the bug is fixed, the line becomes harmless
  (deck109 needed exactly this twice; "Hellrider is broken, don't cast it" loses the games where it
  resolves; silence loses the 43-repick games; an explanation wastes the attention budget).

- **Interface-defect defusal lines are a tracked STOPGAP CLASS with a lifecycle — and the class
  now spans BOTH layers.** A defusal line explains what a misleading label ACTUALLY does and
  redirects ("'Cast Card Normally with Mox Opal' just makes mana with Mox Opal — your creatures
  and spells come first"); it never teaches strategy. In the handoff, list each one as a TRIPLE:
  the line, the platform defect it patches (with the log citation), and the REMOVAL CONDITION
  ("delete when the engine relabels mana-ability activations"). These are pure attention tax once
  the defect is fixed and invisible to a future reviser who doesn't know the defect existed — the
  explicit removal condition is what lets wave N+1 delete them (and the platform fix propagate to
  every guide) in one sweep. The CORE prompt carries a stopgap too — the cast-happened `hand -> stack`
  anchor (the counterspell tell was RETIRED this wave when the stack was surfaced; only the anchor
  remains, live until the Hellrider-class no-op is fixed) — and the same triple discipline applies
  there, recorded in the wave's general-strategy rationale. **How a wave-N+1 author knows which
  stopgaps are still needed:** at wave start, for each inherited stopgap (guide OR core), check (a) the
  rerouted engine-ledger in the previous wave's synthesis reply/notes — is the defect's engine fix
  shipped? (b) the NEW corpus — does the defect still manifest (the tell audit re-run; the no-op
  sweep; the menu-offset table)? and (c) the CURRENT core prompt — has it since absorbed the defense?
  A stopgap whose defect is fixed is DELETED; one whose defense moved to core is DROPPED from the
  guide; one whose defect is still live is KEPT and re-cited. **AND one more check for any stopgap
  written against a MENU ORDER, OPTION LABEL, or BOARD-REPRESENTATION surface: pull one fresh prompt
  and confirm the stopgap's INSTRUCTION still MATCHES the current surface.** An engine fix that
  reverses or relabels the surface does not just make a positionally-phrased stopgap inert — it
  INVERTS it to point at the WORST option. A guide line the new corpus shows the pilot SUCCEEDING
  WHILE DISOBEYING is a flip/delete signal, not a validation (the mirror of "grep replies for echoes
  while MISPLAYING"). Concretely for wave 6: the X-menu was REVERSED to largest-first and the X
  reversal SHIPPED — so the X-teaching's positional step ("pick the LAST line") is now INVERTED (it
  names X = 0) and must be FLIPPED to the value-anchored form ("the line whose value is largest," verify
  "X = <biggest number>"), not merely kept; deck140 won 6/7 max-X only because the pilot ignored the
  stale line. The value-anchored verify step survives a reversal; a positional pick does not.

- **Anti-deferral wording.** For the plan-critical action say "NOW / the first turn it appears / in
  THIS reply," and license the imperfect version explicitly ("cast it even if you cannot also equip
  this turn"). The general prompt carries universal anti-deferral, plan/number-binding, and stale-plan
  grounding — add a deck-guide version only when the new logs show the pattern persisting, anchored to
  the deck's own action ("the EQUIP happens THIS turn — if 'Equip' is still listed, it has NOT been
  done").

- **Low branching; use a ranked list or a selection TABLE, not nested IF/THEN.** A priority order (1,
  2, 3) with a one-clause reason each, or a board→card mapping ("one creature → Edict; many small →
  Pyroclasm; anything big → Wrath"). Key unavoidable branches on something qwen can SEE. The table
  pattern is FIELD-PROVEN twice now: deck140's sweeper table was followed correctly in essentially
  every observed sweep across 12 games in wave 4, as in wave 3.

- **Tie every rule to the in-game surface.** Quote the phrasing the engine actually shows: "When you
  see 'Equip with Cranial Plating [cost {1}]', take it." Name cards EXACTLY as they appear.

- **Concrete, named, numeric — and match the numbers to the OBSERVED pool.** Name cards and costs;
  state thresholds numerically and say what they turn on ("cast at least 3 other black permanents
  first," not "develop your board"). A numeric threshold is only real if it MATCHES the games: before
  shipping any "X+ mana / power Y+" rule, check it against the threats that ACTUALLY appeared and
  decided games. Derive selection rules as a NAMED-CARD table from the observed pool first ("counter
  Master of Etherium, Cranial Plating, Guttersnipe on sight"), then add ONE numeric catch-all keyed on
  visible state for the unnamed remainder ("any creature or equipment while your life is 12 or less").
  Generic-Magic thresholds inherited from prior waves are stale-rule candidates like any other line
  (deck44's inherited "counter the first 4+ mana spell or power-4 creature" matched ZERO of the cards
  that actually killed it — Master of Etherium is {2}{u} printed 0/0, Cranial Plating {2}).

- **Give arithmetic it can run — as a per-turn CHECK bound to a trigger, not a formula to recall.**
  "Damage = unblocked attacker power + (1 per attacker if Hellrider out) + burn to face; if ≥ opponent
  life, attack with everyone and fire all burn." For a deck whose win condition is a sized/announced
  spell (X-burn, drain, alt-cost dump), bind the arithmetic to a trigger the pilot meets EVERY turn
  ("EVERY TURN it is in hand: X = your untapped lands minus 2; if X ≥ the opponent's life, cast it NOW
  — you win this turn"), not a lethal formula the pilot must remember to invoke — posture prose about
  the same card ("this IS your kill") does not produce sized casts (deck140: named "your kill," never
  closed, opp finished 14-20 with the spell in hand).

- **A card with TWO distinct uses needs TWO distinct KEYED TRIGGERS.** The weak executor learns
  exactly the trigger you write and no adjacent use — it fires only the use you spelled out and misses
  the card in its other role. When a single card has two correct uses (a burn spell that is also reach;
  a drain that is also lifegain; a creature that is also a blocker), write a separate keyed trigger for
  each (deck133: Gray Merchant is a lethal finisher AND a lifegain stabilizer; the wave-4 guide gave it
  only the lethal trigger "drain ≥ opp life," so the pilot never fired it as a survival button and died
  at 1 life with it castable — the wave-5 guide adds "your life 10 or less and Gray Merchant castable:
  cast it NOW, the drain is also your lifegain," reusing the core's 8/10-life threshold family).
- **Life-as-resource needs a BUDGET, not just permission.** When the deck pays LIFE across several
  cards (fetches + Thoughtseize + Arena + Yawgmoth), "always pay it" is half the rule — unbudgeted it
  compounds into the pilot racing itself. Sum the deck's self-damage surfaces; if there are 2+, add a
  budget line keyed to the ROLE CHECK, naming which payments continue when defending and which stop
  ("when defending, play a Swamp and a creature instead of Thoughtseize; skip Arena"), each phrased as
  the positive alternative action, never as a list of forbidden cards.

- **State the WHY in ONE clause, only for load-bearing rules** — so a rule that looks locally wrong is
  trusted anyway ("attack with the 0-power creature — its battle cry pumps the team").

- **Give explicit PERMISSION where the pilot is wrongly timid and explicit BRAKES where it is wrongly
  reckless** — but the two are not symmetric in risk: an over-fired permission costs a card; an
  over-fired brake or pursuit can cost every turn of the game. This executor's dominant failure mode is
  passivity — when in doubt, default to permission.

- **Override the misleading generic prior BY NAME** ("The general prompt says hold interaction —
  IGNORE that here; this burn is reach, throw it at the face"). The precedence clause only fires when
  the executor RECOGNIZES a conflict, so still name the prior. Don't wait for an observed misfire: scan
  your OWN directives for collisions with general priors and pre-empt them ("cast every copy — the
  'redundant copies' caution does NOT apply to Astrolabe").

- **Turn `#HINT` / `auto=` behaviors into one-liners the pilot can rely on** ("Blightsteel
  auto-attacks once in play — your only job is to CAST it").

### Positive framing default and its two sanctioned exceptions (the negative-framing ruling)

The lesson "negative framing bleeds" is A/B-proven: a clause phrased as "do NOT save them…"
suppressed the positive instruction adjacent to it, and deck131's negatively-framed draw-spell clause
held draw casts to 0.17/game until it was rewritten positively (then 1.0/game, held across 12 games).
**POSITIVE form is the default for all OPERATIVE rules and for the terminal recap.** Exactly two
negative constructs survive, tightly constrained:

1. **Quote-and-forbid a verbatim recurring WRONG THOUGHT — as a FALSITY FACT, not a behavior
   prohibition.** This is an interception keyed to the executor's own phrasing; there is no positive
   paraphrase with the same trigger (the executor must recognize its own sentence), and it is
   load-bearing (the mana interception survived from wave 3 into deck110's turnaround; deck135's
   22/0 Diamond Faerie refusal). KEEP it, under three constraints: (a) it FOLLOWS its positive rule in
   the same block, never leads and never sits adjacent to a positive instruction it could suppress;
   (b) phrase it as a falsity fact about the thought ("the thought 'I have no mana' is ALWAYS FALSE
   when a Cast option is listed"), not as a "do NOT" imperative; (c) at most one such line per rule.
2. **A single bare NEVER/DON'T on the SINGLE worst leak, only where no positive restatement is as
   sharp** — and even then attach its positive redirect in the same sentence. For a CAPABILITY
   restriction (can't block, can't target) state where the capability GOES instead ("Bloodghast
   CANNOT BLOCK — as defender its jobs are devotion, Yawgmoth fodder, Brutality fuel"), never a bare
   prohibition.

Everything else that was a "DO NOT" — including the guide's terminal list — is written POSITIVELY as
situation→play (Step 4 item 9). A terminal recap placed LAST has no positive neighbor to suppress and
serves the same pattern-match-the-replay function the old DO-NOT list did, without the bleed risk. Do
NOT place a negative clause mid-guide beside the positive instruction that carries the plan.

**A bare DON'T aimed at a decision SEAM (blocking, attacking, mulligan) is the single WEAKEST possible
placement** — it is both negative AND, if terminal, out of position, so it "covers" the seam on paper
while contributing nothing. A seam always needs its own POSITIVELY-framed floor at a seam-labeled
block, never a tail-end prohibition (deck109 wave 5: a terminal "Do not block or play a control game —
race" did not fire once; the pilot reflexively blocked 4 times at high life, exactly the "silence at
the blocker seam is block-by-default" pattern the per-seam-floor rule predicts).

- **Short.** It rides in every prompt and competes with the live board for attention. Cut anything
  that fails the "does this change a chosen number?" test.

## Pitfalls

- **Writing for yourself, not qwen** — nuanced, hedged, branch-heavy prose reads well to you and gets
  ignored or misread. Spell out the conclusion.
- **Blaming the pilot for a harness defect** — the corpus is clean enough now that a "misplay" is
  often a desync, an engine no-op, or a menu offset. Run the harness-defect sweep before scoring any
  play; the three-bucket taxonomy assumes an executor the system faithfully obeys, and it no longer
  always does.
- **Diagnosing wording before checking VISIBILITY** — a misplay whose deciding fact never appeared in
  the prompt is a platform defect; rewording the guide re-diagnoses it forever.
- **Fixing last wave's leak so hard you cause this wave's.** A brake written against an over-spend
  becomes the next corpus's paralysis (deck110: "don't tap out" → 0/6, five timeouts); a loud PURSUIT
  becomes the next corpus's durdle (deck135: "GET AN ISLAND" → 16/59 decisions hunting a land).
  Every fix is a new default the executor over-applies — run the Step 5 brake-AND-pursuit audit.
- **Reading a timeout as passivity by reflex** — check who was ahead and the latencies first;
  winning-but-slow and latency-starved timeouts are not guide bugs.
- **Owing the whole record to the guide** — a residual traceable to deck construction gets an upstream
  flag and a play-the-hand-you-drew line, not louder exhortations.
- **Identity prose that licenses passivity** — "you win late / stabilize first" is a rule to the
  executor even though you wrote it as flavor.
- **Circular, self-locking prohibitions** — a DON'T whose escape condition depends on behavior the
  DON'T itself prevents.
- **Cross-seam leakage** — a loud in-game line firing at the mulligan/blocker seam it lexically
  matches.
- **Substring-counting the offered-vs-taken tally** — count numbered option lines only, or the tally
  corrupts in both directions.
- **Card text from memory** instead of the engine `auto=` — you'll teach a rule the engine doesn't
  implement. This includes your OWN causal claims about who a trigger hits.
- **Reading only choices, not replies** — the digest shows the misplay; the PLAN text shows the
  misconception (and, via plan-vs-consumed, the interface trap) that caused it.
- **Compressing a working rule to nothing over successive waves** — keep the load-bearing tokens
  verbatim and record which they are, or wave N+1 erodes the active ingredient.
- **Burying the lede** — a correct rule in paragraph nine is a rule qwen skips.
- **Over-length** — a long guide buries its own most important line.

## Definition of done (checklist)

- [ ] Every card claim (numbers AND mechanism) matches the engine's `text=`/`auto=`, not memory,
      including your own causal claims in findings.
- [ ] The harness-defect sweep (desync, engine no-op, numeric-offset) ran; every loss is attributed to
      one of FOUR buckets, and only bad-play items became guide lines.
- [ ] The #1 rule at the top fixes the single biggest BAD-PLAY leak in THIS deck's logs, stated as a
      procedure over the visible option list (not a posture), in imperative form.
- [ ] Every posture-level rule passed the EXECUTABILITY AUDIT; every passive SEAM has its own per-item
      checkable floor.
- [ ] The `reply` text was mined; every systematic false belief is either already covered by the
      general prompt (reinforced only if still losing) or countered with a corrective FACT — anchored
      to the visible list where possible — flagged for promotion upstream.
- [ ] Every brake AND every pursuit/preference has a visible scope, a countable release/completion
      condition, and survives the "applied to every decision, does the deck still function?" test.
- [ ] Menu-shaped decisions are taught as the MENU (transcribe + index/value mapping + pick-over-list
      + verify), not as card prose; any inference tell passed the TELL AUDIT.
- [ ] The win condition/engine is named and told to be deployed near the top; where the corpus
      contains a winning line, it is transcribed as the pilot's own proven procedure.
- [ ] Role-by-matchup is a per-turn ROLE CHECK on board-readable facts; a narrow-win shell has a
      two-gear identity.
- [ ] A mechanical lethal/math check bound to a per-turn trigger is present if the deck closes with
      reach.
- [ ] The terminal list is POSITIVE situation→play; negative framing appears only as the two
      sanctioned constructs (falsity-fact interception; one bare NEVER on the worst leak with its
      redirect).
- [ ] Generic-prompt priors that misfire OR collide with your directives are overridden by name.
- [ ] Mulligan keyed on a countable. Cross-seam leakage audited.
- [ ] On a revision pass: obeyed rules demoted (load-bearing tokens kept verbatim, general-absorbed
      lines deleted, self-citation stats cut); a turnaround deck revised as a frozen-line diff.
- [ ] Every engine-defect mitigation is a positive behavioral fallback, and every stopgap/defusal/tell
      line is listed in the handoff as (line, defect+citation, removal condition).
- [ ] Every rule is imperative, concrete, low-branching, and executable from the board + action list
      alone. Nothing repeats the general prompt; it fits on roughly one screen (~40-80 lines).

## Output and handoff

Write the finished guide as PLAIN TEXT (it slots into `{STRATEGY_GUIDE}` verbatim). No markdown
headers; short ALL-CAPS section labels read fine. The deck8 / deck44 guides are the format and density
reference.

**Layer-routing discipline for everything you flag upward.** Your handoff routes each item to one
of FOUR layers, and the routing IS the deliverable — argue it explicitly:
- **CORE PROMPT** — deck-agnostic defects/facts, and the burden of proof is ON core: every deck pays
  for each line, so ship core text only when the failure showed up deck-agnostic (or is mechanically
  guaranteed to). Prefer a COMPLETION of an existing core line over new doctrine.
- **PER-DECK** — anything role-, posture-, or named-card-dependent. A racing deck's "creatures don't
  block" promoted to core would misplay every control deck; the deck-agnostic FLOOR goes core, the
  posture stays in the guide.
- **REPLY-PROTOCOL (code-appended)** — anything that must co-evolve with the parser (the reply-format
  contract). It ships from code, next to the parser it serves — not from the prompt body and not from
  seven guide copies.
- **ENGINE/REPRESENTATION** — anywhere a core line is PRESENT but LOSING at distance, or the deciding
  fact is invisible. Instruction cannot substitute for visibility, and repetition is not a mechanism:
  a rule already stated and still bypassed routes to representation (surface the fact, stamp the
  header, label the option), never to a second paragraph of the same instruction.
Two epistemic rules proven this wave: **single-seat evidence cannot cut a core line** — a line your
deck never exercised is untested, not dead; only cross-deck agreement removes core text (removal
experiments are a deliberate A/B, not a wave edit). And **reconcile thresholds by REUSE**: when your
proposal needs a numeric trigger and the core prompt already carries a nearby number (the 8-life
burn-at-face threshold), reuse that number rather than minting a second one — one number reused is
easier for a weak executor than two close ones, and it forces convergent proposals to merge.

The draft is LLM-authored; the user does a Magic-judgment authorship pass and owns the final guide —
surface the card-rules details you verified and flag anything uncertain, plus:
- every corrective-fact line that belongs in the general prompt or board representation. (Status
  after the wave-4 general revision: the list-anchor and the interface-contract timing anchor
  SHIPPED to core — drop guide copies; the reply-format contract was ROUTED to the reply-protocol
  layer, not the prompt — guides carry it interim only until that layer ships.)
- every ENGINE/HARNESS defect found in the sweep, filed SEPARATELY as a bug report with repro records
  (desync citations, no-op A/B pairs, menu-offset tables), NOT smuggled into the guide;
- every stopgap/defusal/tell line as a triple (guide line, defect + log citation, removal condition);
- any residual attributed to DECK CONSTRUCTION, as an explicit decklist flag (manabase skew, threat
  density), set as an expectation, not fixed with guide length;
- any runtime-computation failure routed to the model-experiments residuals track (with its litmus
  record), rather than written as a guide line.

**VALIDATION IS PART OF DONE-NESS, not optional**: a guide can REGRESS a deck (deck110 went from
strongest to 0/6 after a faithful wave-2 revision). Re-run self-play with the guide in place, read the
new logs, and diff behavior against the PREVIOUS wave's corpus — did the plan-critical action start
happening, did cast-nothing / timeout / durdle rates drop, and did a new downstream leak appear? Run
the end-to-end chain re-audit (offered → taken → parameters → resolved). If Rule #1 isn't being taken,
make it louder, shorter, more directive and iterate on that lever before adding anything else; then
return to Step 0. Note: shipping the guide as a Res asset needs `git add -f` (`bin/Res` is gitignored).
End by naming, in your own words, the deck's plan and the single biggest piloting problem the guide is
built to fix.

---

## Appendix — considered and REJECTED (do not re-propose)

- **Citing the observed failure stat inside the guide** ("last run you picked Cast nothing 76 of 101
  times") — self-flagged unvalidated in wave 3; wave 4 (deck133) confirms these describe a pilot that
  no longer exists and should be DELETED on demotion, not kept. Not guide content.
- **Per-deck inline markers and deck-specific rule content in the SKILL** — evidence stays as examples
  that teach the pattern; the skill is craft method, not a rule library.
- **Relaxing the ~40-80-line budget for tell / stopgap / defusal sections** (deck44 self-rejected) —
  the budget pressure is what forces the Step-0 compression that pays for new lines; keep the
  constraint. Stopgaps earn space by displacing a demoted rule, not by growing the guide.
- **A deck-specific doctrine section** (counterspell doctrine, lifegain/unwinnable-matchup section,
  matchup trees) — the generalizable parts live in the method (tell authoring/verification,
  pool-derived tables, residual attribution); a matchup-tree rule violates the CUT list.
- **Carrying interface/format contracts as PER-GUIDE content long-term** — they are general-layer
  furniture; a guide carries them only as a tagged interim stopgap. Seven copies is the restatement
  failure Inputs #2 bans.
- **A bare terminal DO-NOT list** — superseded by the positive situation→play recap (the
  negative-framing ruling). deck110's defense of it is satisfied by the positive form, which keeps the
  last-position + self-contained-redirect properties without the bleed risk.
- **Updating the executability-audit EXAMPLE because deck131's zero-priority-window case was fixed
  engine-side** — the audit's METHOD is unchanged and the example still teaches it; a "(since fixed)"
  footnote suffices. The audit's new frontier (partial answers to bundled asks) is covered in Step 2.
- **deck44's proposed EXTENSION of the counter falsity-fact with a second wrong phrasing ("the spell
  already resolved / moved to the battlefield")** — the whole falsity-fact/tell apparatus was RETIRED
  when the stack was surfaced; extending an obsolete tell is the wrong move. Fixed by representation.
- **Promoting reflexive-high-life-blocking to a sharper standalone CORE sentence** (deck109) — SINGLE
  SEAT this wave; kept per-deck and put on the wave-6 convergence watch. Only cross-deck agreement
  cuts or adds a core line.
- **A core "cast-triggers-payoff" line** (deck131's Guttersnipe/Young-Pyromancer exception to
  "reactions need a target") — per-deck by construction; only deck131 runs such permanents. Revisit if
  future decks add Talrand/Firebrand-Archer/Murmuring-Mystic-class cards.

---

## Wave-5 changelog (each reviewer proposal: adopted / adapted / rejected + why)

**SKILL — adopted (multi-seat or completes a method line):**
- deck135 P1 *displacement vs overshoot* — ADOPTED as a Step-0 sibling bullet. The wave's headline
  method contribution; a fix can be obeyed/correct/non-overshooting and still fail because the leak
  moved upstream.
- deck131 P2 *OBEYED-BUT-LOSING Step-0 mode* — ADOPTED. Fills a real gap (turnaround mode keyed only on
  improvement); forbids the reflexive rewrite a bad n≤6 record tempts.
- deck131 P1 *attacker denominator = engine's `A#.` offered list, not battlefield* — ADOPTED into the
  bundled-asks sub-bullet + digest guidance. Prevents a phantom subset-attack leak (false positive).
- deck44 P1 *PLAN/CHOICE MISMATCH (intent collapse)* — ADOPTED as a Step-2 sweep class, distinct from
  desync and plan-diverges-from-legality.
- deck44 P2 *log-tail-only facts are functionally hidden / fabricated over* — ADOPTED into the Step-0
  representation rung and the Step-5 tell pattern; the shipped ON THE STACK line is its template.
- deck110 P1 *re-offer three-part fix has a proven CEILING* — ADOPTED into the Step-5 re-offer block
  (read-then-disobey → stop escalating, route to representation).
- deck110 P2 *count-brake keys on the bounding resource, not the turn* — ADOPTED (unattached copies /
  unused charges, not a flat per-turn cap).
- deck140 P1 *a fixed-defect stopgap on a SURFACE can INVERT; success-while-disobeying is a flip tell;
  prefer value-anchored over positional* — ADOPTED into the Step-0 stopgap re-check + the TEACH THE
  MENU pattern (step 3 flipped to value-anchored; wave-6 X-teaching must FLIP, not merely keep).
- deck140 P2 *menu-adjacency index carryover* — ADOPTED as a third numeric-offset shape (route to
  engine ledger; don't write guide lines).
- deck133 P1 *prefer engine-surfaced number, verify its offset* — ADOPTED next to TEACH THE MENU.
- deck133 P2 *two distinct uses need two distinct keyed triggers* — ADOPTED into "Give arithmetic."
- deck135 P2 *two zero-offer cases (never-drawn vs in-hand-uncastable)* — ADOPTED into "Never-drawn ≠
  validated."
- deck135 P3 *a ROLE-CHECK must name the SEAM it fires at* — ADOPTED into the Step-1 role-check bullet.
- deck135 P4 *utility/mana-dork carve-out inside the attack floor* — ADOPTED into the Step-5 act-floor.
- deck109 *a bare DON'T at a decision seam is the weakest placement* — ADOPTED as a compact line in the
  negative-framing ruling.
- deck131 *digest gotcha (isolate `Your battlefield:`)* — ADOPTED into "Extract, don't eyeball."

**SKILL — no-change proposals confirmed (reviewers validated existing method):** deck109 (stopgap
lifecycle, negative-framing prediction both fired correctly — no edit), deck133/135/140 explicit
non-proposals (tell audit, harness sweep, wins-as-mirrors, turnaround mode, threshold reuse). No
method was cut.

**CORE PROMPT — see `core-prompt-revision.md`:**
- Edit 1: RETIRE the counterspell offer-is-the-signal / log-tail tell; point at the shipped ON THE
  STACK section + labeled counter options; add the mana-discipline fact (window opens only when a
  response is payable). Supersedes deck44 #1 (which asked to EXTEND the obsolete tell).
- Edit 2 (CONVERGENCE, 3 seats): the act-floor is satisfied by the HIGHEST-impact play; deploy a threat
  before a same-turn value/ramp/dig spell (deck135 G1 + deck133 #2 + deck131 Prism Ring).
- Edit 3: name the ON THE STACK section in the "During the game you will receive" paragraph.
- REJECTED for core: deck109 reflexive-blocking (single-seat → watch); deck131 Guttersnipe exception
  (per-deck by construction).
