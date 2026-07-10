# Engine / integration / harness findings from the deck-109 review

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260709-220729/`. Deck-109 translogs cited
as: w44=1783652855, L131=1783652856, w135=1783654166, t110=1783655262, w133=1783655265,
w140=1783656460 (`seq` = record index in the deck-109 file). Severity tags:
**[GAME-LOSING]**, **[QUALITY]** (wrong-but-survivable), **[TOOLING]** (review/eval cost only).

---

## 1. [GAME-LOSING] Reply-parse picked the wrong option number — reply said "1", engine consumed "9" (own creature killed)

**Observed.** L131 seq 19, target menu for Pillar of Flame (9 options). The model's reply
begins, verbatim: `1\nMy plan is to cast Pillar of Flame targeting the opponent's face to
deal 2 damage, reducing their life from 12 to 10.` — an unambiguous leading "1" (option 1 =
"The opponent (player, life 12)"). The logged `choice` is **9** = "Goblin (1/1) [your
battlefield]", the LAST option, and the next record (seq 20) confirms the pilot's own Goblin
died. This is in the game deck 109 LOST.

**Probable mechanism.** The reply's tail contains "…close the game on **Turn 9** with a
final burn spell" — the only "9" in the text. The parser appears to have matched a number
other than the leading one (last-match, or a regex that skipped the bare `1\n` form). Worth
reading the reply-parsing code in AIPlayerGPT with this exact record as the test case.

**Suggestion.** Parse rule: take the FIRST standalone integer at the start of the reply
(before the PLAN: line); validate it is within [1, options]; on ambiguity prefer the earliest
match, never the last. Add this record as a regression fixture. Also log the parsed-vs-raw
value (see §6) so future desyncs are visible without diffing reply text by hand.

## 2. [GAME-LOSING] `choice: -1` records = empty model reply; fallback behavior is invisible

**Observed.** Five `ask` records across the corpus's 109 logs have `choice: -1` with an
EMPTY `reply` string (w140 seq 6/11/19, w133 seq 8/13). The game continued — e.g. after w140
seq 6 (-1), the following record shows Gore-House Chainwalker was cast and its unleash
counter prompt appeared — so some fallback (retry? default pick? Baka heuristic?) chose an
action that is recorded NOWHERE.

**Why it matters.** (a) If the model call is failing/timing out on a visible share of ask
decisions, that's lost decision quality and wasted wall-clock; (b) the translog cannot be
replayed or reviewed through these gaps — I could not attribute those plays to
guide/model/fallback.

**Suggestion.** Log the failure cause (timeout/parse-fail/API error), the fallback path
taken, and the action it consumed, as fields on the same record. If the fallback is a silent
retry, log each attempt.

## 3. [GAME-LOSING] Board representation: add "Untapped lands / available mana" and mark affordability — the cheapest fix for the corpus's biggest leak

**Observed.** The single most costly behavior in the six games was the model asserting it had
no mana while untapped Mountains sat in the `Your battlefield:` line, then choosing "Cast
nothing": t110 seq 13/15/17 (held exact burn lethal at opp 6 on turn 8; game hit the 40-min
timeout), w140 seq 14/16 ("all lands are tapped from attacking"), L131 seq 23/25/26 (held
castable creatures at 6 life vs 6, in the lost game). The model has to COUNT untapped lands
out of a long comma-separated battlefield line; it fails, and it holds a prior that attacking
taps lands.

**Suggestions (each independently useful, ordered by value/effort):**
1. Add one line to CURRENT SITUATION: `Untapped lands: N (can produce: {R}{R}{R})`. This is
   computed state the engine already knows; it kills the delusion at the source instead of
   asking a weak model to do visual counting.
2. The casting menu is already legality-filtered — SAY so in the framing line ("every listed
   action is affordable right now"), and/or annotate each cast option with `[affordable]` or
   the post-cast floating mana. The model declined LISTED options claiming it couldn't pay
   (w140 seq 14 declined four listed casts "lacking the mana").
3. `Mana in your pool: (none)` is actively misleading mid-main-phase — the model reads it as
   "I have no mana" when it means "nothing floating". Rename to
   `Floating mana (unspent from tapped sources): none` or merge it into the untapped-lands line.

## 4. [GAME-LOSING amplifier] The YOUR-PLAN carryover re-injects false premises for the rest of the game

**Observed.** The "YOUR PLAN (as you last stated it)" block feeds each decision's stated plan
into the next prompt. When a plan contains a false premise, it becomes self-sustaining:
t110's "all three Mountains are tapped from attacking on Turn 6" appears in the seq-15 plan
and is then RE-READ and re-asserted at seq 17 — two turns later, four untapped Mountains on
the board — because the stale plan text outweighs the board line for the executor. The same
loop drives the "next turn I will cast X and finish them" deferral spiral (t110 seq 13→15→17,
three consecutive re-deferrals of the same kill, into the timeout).

**Suggestions.**
- Cheap: prepend one clause to the block: "YOUR PLAN (stated earlier — the board may have
  changed; re-check mana and life from CURRENT SITUATION, not from this text):".
- Better: age it — drop the carried plan at the start of each of the pilot's own turns
  (plans written during a previous turn are exactly the stale ones observed).
- Consider truncating plans to ~3 sentences at capture time; the multi-paragraph plans in
  these logs bury the actionable clause and give false premises more surface to live in.

## 5. [QUALITY] Blockers seam is effectively untested by selfplay

**Observed.** Deck 109 received ZERO `blockers` decisions in six games (corpus-wide: 3).
For an aggro deck whose opponents rarely attacked, selfplay simply never exercises the
blocking prompt, parse, or strategy text. Any bug there (representation, indexing like §1,
guide wording) is invisible to this evaluation loop.

**Suggestion.** Don't rely on selfplay for coverage of rare decision kinds. Build a small
scenario harness (scripted board states via the existing TestSuiteAI machinery) that forces
specific decision kinds — a lethal-in-hand main 2, a must-block-or-die turn, a modal choice —
and asserts on the consumed action. This also fixes the variance problem: Hellrider, the
deck's namesake payoff, was never DRAWN in six games, so guide changes about it are
unvalidatable by selfplay at this game count.

## 6. [TOOLING] Translog improvements that would have materially sped up this review

Per record:
- **`chosen_text`** — the resolved text of the consumed option. Every reviewer currently
  regexes the choice index back into the prompt; a stored field would also have exposed the
  §1 reply-vs-choice desync immediately.
- **Timestamp (epoch ms) + model latency** — indispensable for the timeout question (§7): I
  cannot tell from the logs where t110's 40 minutes went (109's log ends at seq 17, turn 8).
- **Turn number and phase as structured fields** — currently re-parsed out of prose.
- **Raw parse artifacts** — the integer the parser extracted, pre-validation (§1, §2).
- **`options` as the actual option list** (array of strings), not a bare count — the count is
  useless; the list makes digests trivial.

Per game:
- A **final summary record**: winner, end reason (kill/timeout/concede), final life totals,
  turn count. results.tsv's bare `timeout` hides that t110 ended with 109 at 17 life vs 6 —
  a game 109 was clearly winning (see §7).
- **Provenance**: hash/version of system_prompt.txt and the deck strategy file in play, so
  corpora remain attributable after guides are revised (this pipeline's whole premise).

## 7. [QUALITY] Timeout economics: adjudicate stalled games and cap turns, don't just cap wall-clock

**Observed.** 13/21 games hit the 40-minute cap — >60% of corpus wall-clock spent on games
yielding a no-signal `timeout` row. t110 is the instructive case: deck 109 led 17-to-6 at its
last logged decision (turn 8, seq 17), then the log goes silent; the remaining ~35 minutes
produced nothing reviewable.

**Suggestions.**
- **Adjudicate on timeout**: record final life totals and declare the leader the winner (or
  at least a distinct `timeout-leader-N` result). This turns dead wall-clock into usable
  win-rate signal for the revision loop.
- **Turn cap** (e.g. 30 turns) in addition to wall-clock — a turn cap ends stalls at a
  semantically meaningful boundary and is robust to per-decision latency differences.
- **Stall detector**: no life-total change AND no permanents added for N consecutive turns →
  end early with the adjudication above. The corpus's prime suspect is mutual passivity;
  this measures it directly, per game.

## 8. [MODEL] (d)-layer observations for model/prompt experiments

- **Stable rules delusions.** "Attacking taps my lands / they stay tapped" (4 of 6 games) and
  "summoning sickness means casting it does nothing" (L131 seq 13/20, w140 seq 37+) are
  qwen-internal priors that persist against on-screen contradiction. Representation (§3) and
  prompt-facts (general-suggestions S3/S5) both attack this; a model-side experiment worth
  running: does a short structured recap immediately BEFORE the options list ("You have N
  untapped lands. Opponent life: M.") outperform the same facts placed higher in the prompt?
  The failure pattern smells like attention/recency, not knowledge.
- **Plan-to-action inconsistency.** t110 seq 15's plan computes the exact winning line
  ("cast Krenko's Command… Searing Spears to deal lethal"), and seq 17 — the very decision
  where that plan was executable — picks "Cast nothing". The model can DERIVE the play and
  still not select it one decision later. Checklists keyed to the visible phase label helped
  in guide form; a decoding-side experiment (lower temperature on `ask` decisions, or a
  two-pass "state your mana and lethal math, then choose") may help more.
- **"Cast nothing" position bias suspicion.** The pass option is always the LAST numbered
  option and was chosen in a large share of ask decisions, sometimes against the model's own
  stated plan. Worth a cheap A/B: shuffle or reposition the pass option and measure pass-rate
  delta before attributing all passivity to strategy.
- **Positive capability note.** The model reliably handles: wide attacker declarations
  (A1..A5 comma lists parsed and consumed correctly every combat), unleash may-counter
  choices (100% consistent), hybrid-mana castability ({B/R} Cackler cast off Mountains), and
  multi-step cast→target flows (when the parse works, §1 aside). The integration seams, not
  the action grammar, are where quality leaks.
