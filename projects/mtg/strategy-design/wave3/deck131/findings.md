# Engine / harness / representation findings — deck-131 review (wave3)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260709-220729/`. Deck 131 games cited
by log file. Severity flags: [CRITICAL] = decides games / invalidates an archetype,
[HIGH] = materially distorts results, [MED] = wastes budget or blurs evaluation,
[LOW] = quality-of-life.

## 1. [CRITICAL] The GPT seam grants essentially no opponent-turn priority windows

**Observed.** `kind` counts per game for deck 131:

| game | opponent | result | ask | priority | attackers | blockers |
|---|---|---|---|---|---|---|
| 1783652856 | 109 | WIN | 23 | 0 | 5 | 0 |
| 1783652858 | 140 | timeout | 44 | 0 | 1 | 0 |
| 1783655258 | 135 | timeout | 20 | 0 | 3 | 0 |
| 1783655259 | 110 | timeout | 16 | 0 | 4 | 0 |
| 1783657026 | 133 | timeout | 26 | 0 | 1 | 0 |
| 1783657658 | 44 | LOSS | 24 | 4 | 1 | 0 |

Four opponent-turn decision windows in six games, all in one cluster (vs44, turn 10,
while the opponent cast flash faeries). The clean counterexample is vs140
(1783652858): the opponent cast **Cruel Edict — a SORCERY, a legal Counterspell
target — twice** (its T5 and T9), each time while deck 131 held Counterspell with
{U}{U}+ untapped. The narrated game log shows the resolution as a single
uninterruptible event ("Opponent's Cruel Edict: hand -> stack; ... stack -> graveyard;
Your Young Pyromancer: battlefield -> graveyard") — the GPT player was never asked.
Both Pyromancers died with the answer in hand and the mana up.

**Why it matters.** This decides whether reactive archetypes can exist in the harness
at all. Deck 131 carries 7 hard counters + 4 Aetherize + fogs — roughly a third of the
deck is castable only in response, and the response moment is never offered. Any
counterspell/flash/trick deck in the pool is playing with a third of its cards blank.
It also means the general prompt's "hold instant-speed interaction" prior (see my
general-suggestions.md S1) instructs behavior the game cannot express — a large
contributor to the 13/21 corpus timeout rate.

**Characterization for the engine dig.** The pattern in the data: windows were offered
in exactly one game, during opponent spell-casts, and the offered action list contained
only the instants the player could AFFORD at that instant ({U} open → only Downsize
listed, not Counterspell {U}{U} — correct filtering). So a window-offering code path
EXISTS and reaches the GPT seam (`kind:priority` with "0 = pass priority" prompt), but
it fires extremely rarely. Questions to answer in `AIPlayerGPT` / the interrupt plumbing
(Wagic's human-player "interrupt?" mechanism, and however AIPlayer normally
receives/declines it):
- Is the engine auto-passing priority for AI players unless some condition (timer,
  interrupt setting, spell type?) holds? Why did Obyra/flash-creature casts (vs44)
  open a window when Cruel Edict (vs140) did not?
- Is there an options/interrupt setting (the human client's interrupt configuration)
  that the selfplay harness inherits with interrupts mostly off?

**Suggestion.** Instrument first: log engine-side every time a spell/ability goes on an
opponent's stack while the GPT player has a castable response, and whether a window was
offered. Then either (engine-side) offer the window whenever the GPT player has a legal
instant-speed response — with a cheap heuristic pre-filter to avoid an LLM call when the
only "response" is pointless — or (prompt-side, already proposed) keep S1's reframe and
accept that reactive decks are second-class. Decide explicitly; today it's undecided by
accident. Note the cost coupling: every offered window is an LLM call (see #4), so
"offer everything" needs the pre-filter or it will blow the game clock.

## 2. [HIGH] Blockers windows nearly absent — corroborating, not proven, from this deck

**Observed.** 0 blockers decisions for deck 131 in six games (3 corpus-wide per the
brief). Partially explainable legitimately for this deck: in the vs44 loss the attackers
were fliers (Scion of Oona, Obyra — deck131's grounded 2/1 can't block them), and in
other games it was attacked while creatureless. So from deck 131 alone I cannot
distinguish "engine never asks" from "no legal blocks existed." But 20 → 4 life over
several combats without a single blockers ask, across a corpus where only 3 such asks
exist among 21 games, smells like the same auto-pass seam as #1.

**Suggestion.** Same instrumentation as #1: log every declare-blockers step where the
GPT player controls an untapped creature that could legally block, and whether a
blockers decision was surfaced. One run answers it.

## 3. [HIGH] "Cast nothing right now" — the passivity exit is always available, neutral-sounding, and repeated

**Observed.** Deck 131 chose "Cast nothing right now" in 70 of 93 casting decisions.
Three design properties compound the model's passivity bias:
- It reads as neutral/safe, with no stated cost ("right now" even implies a better
  moment is coming — for a holding-biased model that's an endorsement).
- The engine re-asks within the same phase after each choice, so a single turn offers
  the exit 2-4 times (vs44 seqs 22-24: three consecutive identical asks, all passed;
  vs109 seqs 16-17 same). Each re-ask is a fresh chance to durdle — and a fresh LLM
  call (see #4).
- It sits at a stable list position (last), and weak models over-select by position.

**Suggestion.** Cheap experiment, likely high yield: rename the option to carry its
cost, e.g. "Cast nothing - end my main phase without acting". Also collapse the re-ask
loop: after a "cast nothing," don't re-offer the identical menu in the same phase unless
the board changed. Both are harness-side, no engine rules change.

## 4. [HIGH] Timeout economics: the 40-minute cap is spent on redundant LLM calls, not game turns

**Observed.** Timeout games reached only turn ~19-30 with 20-45 logged decisions per
player. Decision inventory shows heavy waste:
- **Land drop is its own ask every turn** ("Play Island / Hold Island - do not play it
  now") and was answered "Play" in essentially every observed case (one pathological
  exception, see #7). That's ~1 call per turn per player of near-zero information.
- **Duplicate consecutive asks**: identical mulligan prompt asked 2-4 times
  (1783655258 seqs 0-3 are four identical "Keep this hand" decisions; 1783657658 seqs
  0-1 duplicated too); identical cast menus re-asked after a pass (#3).
- Priority windows, if #1 is fixed naively, multiply calls further.

**Why it matters.** 13/21 timeouts is the corpus's dominant result, and it's partly a
budget artifact, not a strategic one: games are unfinished, so win-rate signals are
mostly censored. Every redundant call also doubles as a fresh passivity opportunity.

**Suggestion.** (a) Auto-resolve degenerate decisions harness-side: single-legal-option
menus, land drop when holding the land has no possible upside (or fold land drop into
the cast menu), duplicate re-asks of an unchanged menu. (b) Consider a soft turn-cap or
sudden-death adjudication (life totals / board value) instead of a pure wall-clock cap,
so stalled games still produce a learnable outcome. (c) Log per-decision latency in the
translog (see #8) to quantify where the 40 minutes goes.

## 5. [MED] Reply-parse failure is silent: `choice: -1`

**Observed.** 1783652856 seq 19: `choice: -1`, matching no option; the next record
(seq 20) is the follow-on target menu, so the harness recovered — but nothing in the
log says how (re-prompt? default? fall-through?). If -1 ever defaults to "pass/nothing,"
parse failures silently manufacture more passivity.

**Suggestion.** Log the raw parse outcome and the recovery action explicitly (e.g.
`"parse":"failed","resolution":"reprompt"`). Cheap, and it removes an ambiguity class
from every future review.

## 6. [MED] Offered actions can be strictly illegal-in-effect (no valid target)

**Observed.** vs44 seqs 15-19: priority window offered "Cast Card Normally with
Downsize" while every opponent creature had shroud — no legal target existed. The model
reasoned it out and passed (impressively — see #9), but the harness spent 4 LLM calls
offering a spell that could not resolve.

**Suggestion.** Target-availability pre-filter before offering a targeted spell in a
window; don't ask the LLM to discover engine-level illegality. (Also the S7 one-liner
in general-suggestions.md as a prompt-side backstop.)

## 7. [MED] Representation gap: nothing marks color-stranded cards, and the hand gives no castability cue beyond cost strings

**Observed.** The vs44 loss pivoted on land-drop choices: three consecutive "Play
Island" picks (seqs 4/6/10, turns 6-10) while the hand's only Mountain and the deck's
{1}{R} payoff sat stranded — Young Pyromancer landed turn 12 vs an untouched opponent.
The prompt shows raw cost strings ({1}{r}) and the land list, and expects the model to
join them; qwen35 didn't, three turns running. (I fixed this deck-side with an explicit
guide rule, but the gap is general — any two-color deck with an off-color payoff hits
it.)

**Suggestion (optional, prompt-representation).** In the hand line, annotate uncastable
cards with the missing color: `Young Pyromancer {1}{r} (2/1) [needs R - you have no red
source]`. One deterministic string-join harness-side saves the weak model a multi-step
inference it demonstrably fails. Alternatively, when a land-drop choice differs in the
colors it adds, say so in the option text ("Play Mountain - adds your first red
source").

## 8. [LOW] Translog quality — what would have made this review faster

All feasible harness-side, no engine changes:
- **Game id + both decks + result in the filename or a header record.** I had to pair
  logs to results.tsv rows by epoch arithmetic (log epoch = game epoch + 3-5s, ambiguous
  when two games start 2s apart) and then confirm by reading opponent card names out of
  prompts. A first JSONL line `{"game":"131v140","seat":0,"result":"timeout"}` ends
  that.
- **Structured fields for turn/phase/life** per record instead of regex-parsing them
  out of the prompt text.
- **Echo the resolved action text** (`"chosen_text": "Cast Young Pyromancer..."`), not
  just the index — the index is only meaningful relative to a prompt that must be
  re-parsed.
- **Per-decision wall-clock latency** — required to attack #4 quantitatively.
- **A final record with end-of-game board + life totals** — end-state sweeps currently
  reconstruct this from the last prompt, which precedes the actual end.
- Include a **system-prompt/guide version hash** per game so corpora are attributable
  to the exact strategy text in play (this review depended on knowing the deployed
  guide text; a hash makes that audit-proof).

## 9. [(d)-layer] Model observations — qwen35 as pilot, given good instructions

For model/prompt experiments; these survived controls for guide and representation:
- **Micro-reasoning is genuinely good.** At the vs44 priority windows it correctly
  chained: Scion of Oona grants shroud → Obyra untargetable → Downsize/counters dead →
  pass; and separately computed {U} < {U}{U}. Board-detail reading is not the weakness.
- **The weakness is a global passivity/safety bias plus no cross-turn goal pursuit.**
  Its PLAN lines said "deploy Young Pyromancer or Guttersnipe as soon as I can afford
  them" for 20 straight turns (vs140) while it declined the draw spells that would find
  them and declined combat vs an empty board. The PLAN text is aspirational narration,
  not a binding commitment — it re-derives each decision locally and defaults to the
  passive option.
- **Obedience to explicit directives is HIGH — this is the leverage.** Wave2's "Prism
  Ring: choose BLUE" was followed 4/4 times (pre-guide it chose red). Guide/prompt
  phrases show up verbatim in PLAN lines. Concrete named rules land; principles don't.
  Corollary: wrong priors are expensive for the same reason (the "hold instants" prior
  was obeyed into four timeouts), and the guide-authoring loop (wave3 revisions) is
  attacking the right layer.
- **List-position and "safe option" heuristics dominate under uncertainty** (Island
  picked over Mountain; "Cast nothing" as the default exit) — consistent with the
  strategy-skill's shallow-default model; representation nudges (#3, #7) should measure
  well.

## Cross-references

- Prompt-side counterpart of #1: general-suggestions.md S1/S2/S3 (this deck's dir).
- The strategy-authoring method updates driven by #1/#3: skill.md revision notes 1-4.
- Deck-side mitigation already shipped: wave3/deck131/strategy.txt (act-floor, draw
  spells on own main, counters demoted, Mountain-first land rule).
