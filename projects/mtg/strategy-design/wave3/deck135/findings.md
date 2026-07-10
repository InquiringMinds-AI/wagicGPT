# Engine / integration / harness findings from the deck-135 review (wave 3)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260709-220729/`. Citations use
translog filename fragments + `seq`. Severity flagged per item.

---

## F1. Missing blockers decision on a game-deciding alpha strike — SEV: GAME-LOSING (engine seam)

**Observed.** Game 135v109 (loss). Turn 10: deck109 declared 5 attackers (its translog
`1783654166-...deck109...` seq 21, reply "A1, A2, A3, A4, A5", 7 total power) against
deck135 at 7 life. Deck135 had an untapped Icehide Golem (2/2, cast T9 — summoning-sick
creatures CAN block) and received **no blockers ask**: its translog
(`1783654166-...deck135...`) ends at seq 32 (T9 Main 2) with zero `blockers` records all
game. `game-135v109-*.stderr` shows the Blockers phase resolving via what looks like the
Baka path: `AIPlayer:Using Activated ability` / `AIPlayerBaka: AI attempting to pay a mana
cost. - Target: Icehide Golem - Cost: NULL`, then straight to `11DamageStack` and
`WAGIC_SELFPLAY_RESULT winner=1`.

**Why it matters.** One blocker vs 5 attackers is a real decision (6 outcomes). The correct
block (Golem on Stromkirk Noble, or any goblin chump) made the race survivable — deck135
had a 10/10 Treefolk swinging for lethal the following turn. The game was decided at a seam
the GPT player apparently never saw. Corpus-wide there were only **3 blockers decisions in
21 games** while attacks happened constantly, so this is systemic, not a one-off.

**Suggestion.** Instrument `AIPlayerGPT::chooseBlockers`: log (a) every entry, (b) the legal
assignment count computed, (c) any early-out (the `mBlocksDoneTurn` re-entry guard, the
"only one outcome" skip, kChoicePending unwind) and (d) any fallback to Baka with the
reason. The stderr trace suggests either a silent Baka fallback or the seam never being
reached for the defending GPT player; one pinned selfplay game (probe deck full of small
blockers vs an attack deck) would reproduce it in minutes. This is the highest-value
engine item in this review.

## F2. Fetch-crack re-offer loop: 3-5 duplicate asks per activation — SEV: MODERATE (cost + narration pollution, and timeout fuel)

**Observed.** A chosen fetch activation gets re-offered and re-answered across subsequent
phases before it visibly executes: v140 (`1783652862-...deck135...`) seq 2-5 (Main1 → Main2
→ End → next-turn Upkeep, all "Put in Play with Windswept Heath..."), seq 8-11, seq 40-46
(chosen at T11 Main2, life decrements only at T13 Upkeep — SEVEN phases later); v133
(`1783652871-...`) seq 18-22 including two consecutive re-asks within the same T6 Upkeep;
v131 seq 2-5, 9-11, 42-44. The life payment consistently lands only on the LAST of the run,
so the earlier "choices" apparently did not execute — this does not look like the ~2%
stale-answer drop-and-re-ask, it looks structural to fetch/priority activations.

**Why it matters.** Three costs: (1) each re-ask is a full model call — in v140, ~14 of 59
decisions were fetch re-asks (~24% of the game's inference budget); with 13/21 games dying
at the 40-min cap, this is direct timeout fuel. (2) Each consumed duplicate grows the
narration, so later prompts carry repeated phantom decisions. (3) It desyncs the model's
world-model — see F3: the hallucinated "Vista was already sacrificed" belief (v140 seq 58)
is a predictable consequence of the model having answered the same crack four times.

**Suggestion.** Trace one occurrence at the click layer: does the crack's click choreography
fail to land in Main1 (interrupt-window timing?) so the offer regenerates next phase? The
ask cache is keyed on board state + question — a phase change alters the key, so identical
re-offers across phases re-ask by design; consider excluding phase from the key for
activation offers, or suppressing a re-ask when an identical option set was answered within
the same turn and nothing but phase changed. Fixing F2 probably buys back a meaningful
share of the timeout rate on its own.

## F3. Plan-carry has no provenance, and stale plans override the live board — SEV: GAME-LOSING in instances (representation design)

**Observed, two shapes:**
- **Turn-ownership confusion:** v131 (`1783655257-...`) seq 51 — prompt correctly says
  "Phase: Main phase 1 | It is your turn." The carried PLAN was written during the
  OPPONENT'S previous turn ("Pass priority. The opponent is in their Main Phase 2..."), and
  the model continued executing it: chose "Cast nothing" and replied "let the opponent
  proceed to their End Step" — on its own main, holding an 8/8 Treefolk and Diamond Faerie.
  The label was right; the plan was the confounder.
- **Hallucinated completed actions:** v140 seq 58 — declined a live Prismatic Vista crack
  (offered as option 2) because "My Prismatic Vista ... has been tapped/sacrificed in
  previous turns." Plan-memory beat the action list. (F2's duplicate answers likely seeded
  this belief.)

**Suggestion.** Cheap representation fixes, no engine surgery: stamp the carried plan with
provenance — `YOUR PLAN (stated on turn 10, during the OPPONENT'S turn):` — so the model
can see it is stale; optionally suffix the plan with a fixed reminder line ("this plan may
be out of date — the CURRENT SITUATION below is the truth"). The general-prompt companion
fix is proposed in `general-suggestions.md` S2. Consider also whether the plan should be
dropped (not carried) across a turn-ownership boundary; provenance-stamping is the safer
first step since plans legitimately span turns ("next turn I attack for lethal").

## F4. Mulligan asked twice — SEV: ANNOYANCE

**Observed.** Win vs 44 (`1783655263-...deck135...`) seq 0 AND seq 1 are both the mulligan
ask ("Keep this hand"), at T2 Untap and T2 Upkeep respectively. Duplicate model call +
duplicate narration entry. Same dedupe question as F2 (phase in the cache key?).

## F5. `choice: -1` records are undocumented — SEV: ANNOYANCE (translog quality)

**Observed.** Scattered `choice: -1` records: v135-loss seq 13 (priority), v140 seq 54
(ask), win44 seq 3 and 5 (ask, immediately after land plays), v131 seq 28/37/39. Unclear
from the log whether these are parse failures (reply didn't map to an option), stale-answer
drops, or something else — and whether a Baka fallback then acted.

**Suggestion.** Add a `fallback`/`disposition` field to the translog record (parsed-ok |
parse-failed→baka | stale-dropped | ...). During review I could not tell whether -1
decisions cost the game anything.

## F6. Translog improvements that would have materially sped this review — SEV: QOL

Everything below I reconstructed by regexing the `prompt` blob; structured fields would
make the digest script trivial and merge-safe across agents:
- `turn`, `phase`, `whose_turn` as top-level fields.
- `chosen_text`: the resolved text of the chosen option (choice index alone requires
  re-parsing the prompt; index drift would silently corrupt analysis).
- `options` as the actual list of option strings, not a count (the count is near-useless).
- `life_self` / `life_opp` at decision time.
- A `reask_of: <seq>` marker when the dedupe/re-offer machinery knows this repeats an
  earlier consumed decision (directly exposes F2/F4 for measurement).
- A terminal record per game: result, final life totals, end turn (I had to join
  results.tsv + stderr tails to know how games ended).

## F7. Timeout economics — SEV: EVALUATION-BLOCKING at current settings

**Observed.** 13/21 games hit the 40-min cap with no result; deck135 alone had 3. The
timeouts are inference-bound (fastclock already strips pacing): v140 spent 59 model
decisions to reach T16. Win-rate — the harness's blind outcome signal — currently exists
for only 38% of games.

**Suggestions**, compounding: (a) fix F2/F4 (fewer wasted calls per turn); (b) on timeout,
adjudicate and record a life-total winner in results.tsv (`winner=0/1 (adjudicated)`) so
stall-heavy matchups still produce signal — final life was strongly directional in every
deck135 timeout I read (17v19, 12v20, 11v19); (c) consider a turn-cap alongside the
wall-clock cap so slow-inference days don't change the effective game length.

## F8. Model-weakness notes for the prompt/model experiment track — (d) layer

Instructions were adequate and the facts were in the prompt; the executor failed anyway:
- **Arithmetic with a quoted card:** v140 seq 37 — the model quoted Lightmine Field's
  effect, then attacked with a 1/1 + 2/2 ("Lightmine will deal 2 damage to each"), losing
  its whole board for zero damage. It performed the retrieval and failed the <= comparison.
- **Summoning-sickness planning:** v109 seq 18-22 — three consecutive decisions built on
  attacking Abominable Treefolk the turn it was cast, despite the rules primer stating the
  rule and the attackers list offering only Boreal Druid. Abstract rule != applied rule for
  this model; the option-list-is-ground-truth framing (general-suggestions S3) is the cheap
  mitigation, but this is a capability marker worth re-testing on candidate models.
- **Mana-color model:** v131 seq 18 — planned "{U} from Snow-Covered Plains"; repeatedly
  promised to cast blue spells with no blue source in play. Land-name → color mapping is
  not reliable knowledge; the affordability-filtered cast list (S3) is the backstop.
- These three make good, cheap **litmus prompts for model experiments** (the OpenRouter
  track): a candidate model that clears Lightmine math + summoning-sickness planning +
  land-color mapping on canned board states will likely dominate qwen35 at same latency.

## F9. What is working — do not regress

- **Bundled attackers/blockers replies** parse robustly, including sloppy replies: deck109's
  "A1, A2, Stromkirk Noble, Goblin, Goblin..." was consumed correctly ("declared attack
  (5 of 5) in one reply" in stderr). The tolerant parser is earning its keep.
- **Option-line rules text** is read and used: the model quotes card text (Lightmine,
  Force of Negation's exile clause, fetch cost lines) in nearly every reasoned reply. The
  P1/P2/P4 fact-rides-the-option design is functioning as intended — failures are now in
  the model's use of the fact, not its availability.
- **Fetch cost-payment** (wave-2's headline fix): cracks happen promptly in all six deck135
  games; the cost-refusal failure mode is gone. Residuals are F2 (mechanical re-offer) and
  F3 (stale-plan decline), not cost aversion.
- **Narration/CURRENT SITUATION quality**: every misplay in this review was diagnosable
  from the prompt alone — the representation carries enough truth that reviewer disputes
  ("bad play vs never-had-the-option") resolve as fact. That property is the foundation of
  this whole pipeline; protect it.
