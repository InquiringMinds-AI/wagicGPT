# Deck-135 wave-4 notes — catch-all for the platform, process, and model tracks

Companion to `findings.md` / `strategy.txt` / `skill.md` / `general-suggestions.md`
in this directory. Organized by audience. Citations: R1 =
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260712-192832`, R2 =
`.../matchups-20260712-230636`, files `<epoch>-ai_baka_deck135-*.jsonl`.

---

## FOR THE ENGINE OWNER — work orders, in priority order

### E1. Fetch re-offer loop (wave-3 F2, PERSISTS) — biggest inference-budget drain

A chosen fetch activation is re-offered and re-answered across subsequent phases
before it executes. Reproduction records (each a run of near-identical asks for
the SAME activation, answered each time):
- R2 `1783916692` (vs44): seq 2-3 (Flooded Strand, Main1→Main2), seq 23-24
  (Main2→next Upkeep), seq 31-32, seq 36-37 — four separate runs in one game.
- R2 `1783919025` (vs140): seq 2-4 (Prismatic Vista, Main1→Main2→next Upkeep),
  seq 8-9 (Windswept Heath).
- R1 `1783904575` (vs110): seq 2-4 (Misty Rainforest across three phases), and
  seq 10-12 (second Rainforest, Main1→Main2→next-turn Upkeep).
Pattern is unchanged from wave-3: the life payment / land arrival lands only on
the LAST answer of the run, so the earlier answers did not execute. Every re-ask
is a full model call (~10-25s at current latencies); the two vs140 games hit the
40-min cap at 143 and 100 decisions. Wave-3's suggestion stands: trace the crack's
click choreography at the first offer, and/or suppress a re-ask when an identical
option set was answered within the same turn and only the phase changed.

### E2. Reply-choice consistency re-ask (cheap, high value)

When the reply text names an action string that appears verbatim in the option
list but the consumed choice is pass / "Cast nothing", re-ask ONCE with a
one-line notice ("your plan named option N - confirm or change"). Two
game-relevant reproductions:
- R2 `1783919025` seq 33: reply plan "Cast Diamond Faerie this turn..." — chose
  pass, on its own Main 1, Faerie listed.
- R2 `1783916692` seq 12: reply says "Cast nothing this turn ... but this is
  likely a mistake given the strategy guide" — passes anyway.
The parser already extracts both strings; this is a string-containment check at
the existing seam. Guard against loops: one re-ask max, then accept.

### E3. Turn-ownership stamp on the choice-list header

The phase line ("Phase: Main phase 1 | It is your turn.") sits far above the
numbered list, past the game log and battlefields. The observed false beliefs
("it is the opponent's turn", "wrong phase") are about the OPTIONS, and the
model demonstrably attends to the option lines (it quotes them). Stamp ownership
where the decision is: e.g. "Your legal actions (YOUR Main Phase 1):". One
narration-format change, no prompt cost.

### E4. Plan-echo splice pollution

Many replies open with a stray fragment of the PREVIOUS decision's chosen text
before their own number: R2 `1783919025` seq 41-57 replies begin ". Cast Glacial
Revelation {2}{g}  PLAN: ..." regardless of the current question; same shape in
R1 `1783906513` (seq 54, 56, 60-63). Suspect the carried-plan splice or the
reply logger includes leading tokens from the prior exchange. Effect: each
reply's first tokens re-assert the previous intention — raw material for the
deferral loops (identical unexecuted plan seq 67-73, T16→T20). Strip leading
echo before logging/parsing; consider carrying only the text after "PLAN:".

### E5. Degenerate-reply detection at the blockers seam

R1 `1783905418` (vs109) seq 17: reply is "** ** ** ** ..." repeated, choice
empty, and a real blocker decision (Coatl could block any of 3 attackers) was
consumed as nothing. A repetition-collapse check (same token >N times, no
digit/option token) should trigger one re-ask. Rare (1 of ~800 decisions in my
12 games) but it silently eats a combat decision when it fires.

### E6. Translog: wave-4 schema is a big step up — two residual asks

`chosen_text`, `turn`, `phase`, lives, `gameend` all landed (wave-3 F6 mostly
DONE — review time roughly halved). Still missing:
- `options` is still a count; the actual option STRINGS would make
  offered-vs-taken tallies trivial and kill the substring-counting trap
  (skill.md P7).
- A `reask_of: <seq>` marker when the same activation/ask repeats (would make E1
  measurable without regexing prompts).
- `fallback`/disposition never appears populated in my files — confirm it is
  wired (wave-3 F5 asked for parse-failed/stale-dropped visibility; E5's record
  shows empty choice with no disposition).

---

## FOR THE NEXT REVIEWER / WAVE COORDINATOR — process lessons

### N1. The over-correction loop is real: check THIS wave's loud rules for overshoot

Wave-3's loud "GET AN ISLAND BY TURN 3" fixed the blue screw and became wave-4's
durdle (fetch-hunting = 15/65, 16/59 of decisions in losses). The wave-4 guide's
own loud rules are the next candidates. Specifically audit next corpus for:
- **Attack-floor overshoot**: suicide attacks that fail all three of Rule #1's
  tests being taken anyway, or attacking into open mana/tricks because "no
  attackers is a misplay" over-fired. The closed exception ("every listed
  attacker dies for zero gain") is the release — check whether the executor
  applies it or steamrolls it.
- **Lightmine solo-attack rule misapplied** to multi-punisher or bigger-punisher
  boards (the rule pre-computes X=1; a different punisher changes the number).
- **"One Island is ENOUGH"** undershooting: games where the second blue-heavy
  hand (double Force, Treefolk+Coatl same turn) actually needed the second
  Island. I judged this rare (2 Islands in deck; Astrolabe covers it) but it is
  the mirror-risk of the fix.

### N2. Two quote-and-forbid lines survive in strategy.txt Rule #2 — deliberate

Coordinator flagged the negative-framing-bleeds tension; skill.md P6 records the
constrained form used (falsity-facts following the positive rule, not DO-NOT
imperatives) and asks synthesis to adjudicate. If next-wave logs show Rule #2's
positive instruction being suppressed, those two lines are the first suspects.

### N3. Reviewer tooling traps hit this wave (details in skill.md P7)

Substring-matching card names across the whole prompt corrupts offered-vs-taken
tallies (narration + PLAN echo + battlefield lines all contain card names).
Count numbered option lines only. Also: my first digest crashed on records
lacking `prompt`/`phase` (gameend records) — guard every field access.

### N4. Outcome accounting: timeouts still hide signal

Both vs140 games timed out with deck135 BEHIND on life (5v25, 13v30) — under
wave-3 F7's proposed life-adjudication these become losses and deck135's real
record is closer to 5W-7L. Worth keeping in mind when comparing wave-over-wave
win rates: the blocker-seam fix (real improvement) and the adjudication gap
(accounting) both move the same number.

---

## FOR THE DECK BUILDER

### D1. The mana base, not the pilot, set up the Island story

Mainboard has exactly 2 Snow-Covered Islands for a deck where {U} gates Coatl,
Treefolk, Search, and Force (18 blue-touched cards). Both waves' failure modes
(wave-3 blue screw, wave-4 Island-hunting durdle) are downstream of that 2-Island
count + 11 fetches/ramp that mostly can take them. If deck tuning is on the
table: a third Snow-Covered Island (over the 2nd Plains — white is only Gelid
Shackles {W} and half of Diamond Faerie) would defuse both failure modes more
cheaply than any prompt text. Astrolabe partially covers it, but Astrolabe's
engine mana ability is `{1}{T}` (costs a mana to use), so it is a weaker fixer
than its reputation.

### D2. Sideboard is dead weight in this harness

`#SB:` cards (On Thin Ice, Negate, Spell Pierce...) never appear in any translog
— no sideboarding step exists in selfplay. If deck files are being tuned for the
GPT track, main-deck slots are the only real estate that matters.

### D3. Matchup note for pool balance

deck140 (the Lightmine/wipes prison deck) produced 4 of the pool's timeouts in
R1+R2 against multiple decks, not just 135 — its design stalls THIS field's
pilots. Fine if intended (it stress-tests punisher handling), but its games eat
a disproportionate share of run wall-clock.

## FOR THE MODEL EXPERIMENTER (qwen35 residuals — litmus cases for candidates)

All three are executor-capability, not prompt-fixable (triage rule in skill.md P5):

1. **Repetition collapse under a structured reply format**: R1 `1783905418` seq
   17 ("** **..." at a blockers ask). Candidate models should be smoke-tested on
   the bundled-blockers reply grammar specifically — it is the most syntactically
   demanding seam.
2. **Constraint violation in assignments**: R1 `1783910753` seq 35 replies
   "B1:A1, B1:A2" — same blocker on two attackers, directly against the prompt's
   stated constraint. Tests instruction-following under combinatorial choice.
3. **Comparison-after-retrieval**: quotes Lightmine's text, then fails "does my
   1/3 survive 1 damage?" — both waves, opposite directions (wave-3
   over-attacked into it, wave-4 under-attacked around it). A canned board state
   with a punisher + one surviving attacker is a 30-second candidate-model test.
   NOTE for the experiment design: the wave-4 guide now pre-computes this case
   for deck135, so THIS deck's next-wave logs no longer measure the capability —
   use a canned probe, not selfplay, to compare models on it.

qwen35 overall: clearly better this corpus at block math (correct
survive-exactly-lethal assignment R2 `1783917446` seq 35) and at Force-of-
Negation discipline than wave-3. The remaining failures are concentrated in
sustained multi-turn intention execution (deferral loops) — which argues the
next model experiment should weight long-horizon consistency probes over
single-decision accuracy probes.
