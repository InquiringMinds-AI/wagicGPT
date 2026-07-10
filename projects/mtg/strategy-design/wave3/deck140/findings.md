# Engine / representation / harness findings from the deck-140 review (wave3)

Evidence base: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260709-220729/`, deck 140 games.
Cited by short name: LOSS = `1783656459-...deck140-0x55b80bb28f70.jsonl` (loss vs 109),
V131 = `1783652858-...deck140...`, V110 = `1783652860-...deck140...` (6 records total),
V133 = `1783657219-...deck140...` (Staff of Nin decline at seq 41).

Severity legend: [GAME-LOSING] directly cost games in this corpus; [DEGRADING] hurts
play or evaluation quality; [ANNOYANCE] friction only.

---

## 1. [GAME-LOSING] {X} in mana costs renders as {l} — confirmed source bug

**Observed.** Every hand/option line for Rakdos's Return ({X}{B}{R}) prints as
`Rakdos's Return {r}{b}{l}` (LOSS seq 16-28 hand lines; V131 seq 21-27 option lines).
The pilot cannot see the cost contains X, reads `{l}` as an unknown symbol (once
guessed "{R}{B}{U}", LOSS seq 16), concluded the spell "deals 0 damage", and took it
0 of 26 offers. Black Sun's Zenith is likewise affected (0/19).

**Source mechanism (confirmed in code).** `ManaCost::toString()`
(`src/ManaCost.cpp:1208`): the X amount is stored one slot past the color slots at
`cost[Constants::NB_Colors]` (see `hasX()`, `src/ManaCost.cpp:562`), and the loop runs
`for (int i = 0; i <= Constants::NB_Colors; i++)` — so at `i == NB_Colors` it falls
into the generic branch and prints `Constants::MTGColorChars[i]`. That table
(`src/MTGDefinitions.cpp:7`) is `{'x','g','u','r','b','w','c','l'}`; with NB_Colors = 7
the X slot prints `'l'` (the LAND placeholder char). Fix: special-case
`i == Constants::NB_Colors` (or `hasX()`) to emit `{X}`. Note `toString()` is also used
by debug dumps; the fix is safe there too. AIPlayerGPT consumes it at
`src/AIPlayerGPT.cpp:279/607/1043/1493/1513` (hand lines, option lines, alternative
costs), so one fix corrects every surface at once.

## 2. [GAME-LOSING] "Mana in your pool: (none)" triggers a false can't-pay belief

**Observed.** ~47 of deck140's 86 "Cast nothing" picks reasoned "I have no mana" or
"must tap lands first" — while the option list, which is affordability-filtered by the
engine, was offering the cast. LOSS seq 18-28: 3 life, 7 untapped lands, "Cast Damnation
{2}{b}{b}" offered repeatedly, declined with *"all my lands are untapped but I have no
mana in my pool yet, and I cannot tap lands to pay for a sorcery without first..."*.
Died with two sweepers in hand.

**Why it matters.** The pool line is technically true (mana is produced at payment
time), but a weak model reads it as a resource gauge. This one line likely depresses
every deck's act-rate, not just 140's.

**Suggestions (either/both):**
- Reword when empty: `Mana in your pool: (none - lands pay automatically when you
  cast)`, or omit the line entirely when the pool is empty (its information content
  when empty is nil; it only matters mid-payment or with rituals/mana abilities).
- Add one assembler-owned sentence directly above the numbered choices: `All listed
  actions are legal and affordable right now.` This is cheap, deck-agnostic, and
  robust even if a future filter bug leaks an unaffordable option (then remove it).
(Also proposed as general-prompt text in general-suggestions.md — belt and suspenders;
the representation fix is the stronger of the two.)

## 3. [DEGRADING] Stale-PLAN poisoning — the carried plan outranks the visible board

**Observed.** LOSS seq 17 stated a plan containing a false premise (*"I have 5 tapped
lands and need to untap them next turn"*). The plan-carry line re-presented it for the
next several decisions while the CURRENT SITUATION printed millimeters below showed all
lands untapped; the pilot kept trusting the plan (seq 18-28) and the delusion persisted
to death. The plan channel converts one bad inference into a persistent one — it is a
positive-feedback loop for exactly the hallucinations it was meant to dampen.

**Design question + suggestions.** Should the carried plan be dropped or reframed when
the board contradicts it? Contradiction detection is hard in general, but two cheap
mechanical options:
- **Age-stamp it:** `YOUR PLAN (stated on turn 9 - it is now turn 12; re-check it
  against the board):` — turn numbers are already tracked; staleness becomes visible
  and the model can discount.
- **Expire it:** drop the carried plan after N turns (2-3) or at each of the pilot's
  own untap steps, forcing a fresh read of the board. The plan's value is intra-turn /
  next-turn coherence; a 5-turn-old plan is almost pure liability.
The prompt-side guard ("when plan and board disagree, trust the board" — see
general-suggestions.md §6) is complementary, not sufficient on its own.

## 4. [DEGRADING] Priority-seam confusion: model "casts" inside a priority ask

**Observed.** LOSS seq 22 (kind=priority): choice `0` (pass), but the reply text says
*"Tap my lands ... and cast **Damnation** immediately"* — the model believed picking 0
executed a cast. The priority menu never offers sorcery casts (correct rules-wise), but
nothing tells the model that; it narrated an action the seam cannot perform, then its
next casting ask (seq 23-24) re-declined for the no-mana reason. Suggestion: one fixed
line in the priority-ask template: `This is a priority window for instants and
activated abilities only - you will get a separate casting decision on your main
phase.` Cheap, removes a whole confusion class.

## 5. [DEGRADING] Timeout economics: durdle-stalls are indistinguishable from slow games

**Observed.** 5 of deck140's 6 games hit the 40-min cap with life totals frozen for
dozens of turns (19/20, 21/16...). These were not slow-but-live games: they were
strategic stalls (no win condition ever deployed) burning wall-clock on ever-growing
prompts — the append-only narration makes each decision's prompt longer, so inference
per decision RISES as a stall drags on; stalled games are the most expensive games in
the corpus. Meanwhile V110 timed out with only SIX deck140 decisions total (mana screw
+ opponent durdle), which looks identical in results.tsv.

**Suggestions:**
- **Stalemate detector in the harness/engine:** N consecutive full turns (e.g. 8) with
  no change in life totals, board card-count, and hand sizes -> end the game as a draw
  with reason `stall`, distinct from `timeout`. results.tsv gains an `end_reason`
  column plus `final_life0/1` and `turns` — the failure-to-close diagnostic I computed
  by hand becomes free.
- **Narration compaction for old turns** (e.g. turns older than ~10 collapse to
  one-line summaries) would bound prompt growth; the CURRENT SITUATION block already
  carries the full live state, so old-turn detail is mostly redundant by design.

## 6. [ANNOYANCE->DEGRADING] Translog quality — what would have made this review cheaper

- **`options` is just a count; the option texts must be regexed out of the prompt.**
  Emit the actual option array (`["Cast Pyroclasm {1}{r}", ...]`). This alone would
  make offered-vs-taken tables trivial and regex-proof.
- **No structured `turn`/`phase` fields.** I regexed `=== TURN n` headers; the pattern
  failed on some games (LOSS rendered turn=? throughout in my digest). Add `turn` and
  `phase` fields per record.
- **No game-end record.** A final summary record (winner, end reason, final life,
  turn count, per-kind decision counts) would let per-game analysis skip the stderr
  files entirely.
- **No wall-time per decision.** A `ms` field would let us separate model latency from
  game length in timeout forensics (V110's 6-decision timeout is currently
  unexplainable from the translog alone).
- **choice=-1 records** (V110 seq 2) appear with empty reply — presumably a failed/
  fallback ask. A `fallback:true` or error field would distinguish "model failed" from
  parse artifacts.

## 7. [SUGGESTION] Build the review instruments into the harness harvest

The two tables that decided this whole review were trivially scriptable and should be
emitted per deck per run by the harness summary step:
- **Offered-vs-taken per card** (count "Cast <X>" occurrences in options vs chosen).
  Deck140's row set — Staff of Nin 1/0, Rakdos's Return 26/0, BSZ 19/0, Elixir 55/0,
  Lightmine 26/0 — was a complete play-quality indictment in five lines, and cleanly
  separates never-drawn (variance) from never-taken (fixable).
- **Cast-nothing rate + reason buckets** (regex replies for no-mana / hold-wait /
  no-target). Bucket sizes route the fix: mechanics delusion -> representation/prompt;
  timidity -> guide; no-target -> fine.
These make guide iteration measurable: after a guide revision, the tables must move,
not just win rates (win rate is blind to the symmetric core prompt anyway).

## 8. [(d) MODEL] Weaknesses that survive good instructions — for model/prompt experiments

Documented so the model-experiments track knows what to probe; none of these are
fixable by strategy text alone (the deployed guide already exhorted against several):
- **Invented game physics, held with confidence:** the tap-lands-first model, the
  pool-gauge misread, "X spells deal 0 damage." qwen does not flag uncertainty about
  rules; it asserts and acts. A rules-primer line helps but the model generalizes badly
  to the next unfamiliar mechanic.
- **Agrees-then-defers:** it restates the strategy principle verbatim in its PLAN while
  choosing the opposite (V133 seq 41: "...before deploying a win condition like Staff
  of Nin" while declining "Cast Staff of Nin"). Exhortation-shaped instructions are
  structurally ineffective on this executor; only closed-form checklists bind it.
- **Self-reinforcing plan trust** (item 3): it weighs its own prior text above fresh
  observations. Relevant to any plan-carry / scratchpad design, not just this one.
- **Confabulated card semantics:** attributed Venser's Journal's hand-size life gain to
  Elixir of Immortality and skipped a land drop "to keep my hand size high" (LOSS
  seq 20) — it blends similar cards in its own deck despite full rules text in the
  system prompt.
- **Cold-check idea:** replay logged decision prompts (they are self-contained) against
  a stronger model or a revised prompt offline — no game needed — and diff choices on
  exactly the decisions above. The translog format already supports this; it is the
  cheapest A/B instrument available, and sidesteps the rare-event/low-power problem of
  judging by game outcomes.

## 9. [MINOR] Sorcery-only hands still get two casting asks per turn

Main-1 and main-2 casting asks both fire (LOSS seq pairs 2/3, 5/6, 8/9, 11/12 show
identical boards asked twice). Correct rules-wise, and the second-main default matters
for other decks — but when the hand contains only sorceries and the pilot's main-1
answer was "cast nothing" on an unchanged board, the main-2 ask is a guaranteed-
duplicate inference call. If ask-cache keys already cover this, verify why these pairs
both hit the model; if they don't (phase is presumably part of the state key), consider
folding phase out of the key when the legal option set is identical. Pure cost
optimization; low priority.
