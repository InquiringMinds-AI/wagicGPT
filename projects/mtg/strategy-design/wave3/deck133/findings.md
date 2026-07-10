# Engine / representation / harness findings from the deck 133 review

Evidence base: run `matchups-20260709-220729`, deck 133's six translogs (0/6: one
loss vs 109, five timeouts). Severity flagged per item. (a)-layer fixes went into
the revised guide; (b)-layer into general-suggestions.md; this file is (c)/(d) plus
tooling.

---

## 1. [GAME-LOSING, P0] `Mana in your pool: (none)` — the single most damaging line in the representation

**Observed.** The pilot reads the pool line as "I cannot cast," and it vetoes
everything: 61 replies across 6 games assert "no mana / cannot cast / my lands are
tapped" while the SAME prompt's battlefield line shows 4-5 untapped Swamps; 76 of
101 casting decisions ended "Cast nothing." Sharpest citations: vs deck131 seq
73-77 (opponent at 4 life, six castable spells, five untapped Swamps, four straight
turns of "I have no untapped mana... my lands are tapped" — they were not — until
the 40-min cap); vs deck109 seq 31-32 (at 6 life, 4 untapped Swamps: "I have no
mana available... my lands are untapped", passed to its death). Note the seq 31
reply contains the tell verbatim: *"Wait, the prompt says 'Mana in your pool:
(none)'. We have no mana available."* — the model quotes the line as its authority.

**Where it renders.** The CURRENT SITUATION block of every assembled user message
(AIPlayerGPT prompt assembly), line 3: `Mana in your pool: (none)`. Technically
true (pools are empty between payments; the engine auto-taps on cast) and almost
never decision-relevant — pools only matter mid-payment, which the seams never
expose. The line is a trap with no compensating value.

**Proposal.** Replace with an affordability statement computed from untapped mana
sources, e.g.:

    Mana available: 5 (5 untapped lands; the game taps them automatically when you cast)

or minimally `Untapped lands: 5 | Mana pool: (empty - normal)`. If a floating-mana
case ever needs surfacing, append `+N floating` only when nonzero. Also consider
one framing clause on the casting ask itself: `Casting decision: which card do you
cast now, if any? (every listed cast is affordable right now)` — cheap, and it
attacks the misread at the exact decision it corrupts. Any of these lets the
corrective paragraphs now stuffed into the deck guide and proposed for the general
prompt be deleted later.

## 2. [HIGH] The graveyard is not in the board snapshot

**Observed.** CURRENT SITUATION shows phase, lives, pool, hand, both battlefields,
opponent hand size, and library counts — no graveyard line. Deck 133 is a
graveyard deck (Bloodghast landfall recursion, Geralf's undying, Liliana the Last
Hope -2 rebuy), and the pilot repeatedly hallucinated graveyard contents in both
directions: claimed Bloodghast would "return from the graveyard" when it was in
hand and had never been cast (vs deck44 and vs deck109 plans; e.g. vs109 seq 30
plan narrates a landfall return that never happened), and could never actually
verify what a recursion ability could fetch.

**Why it matters.** Any ability whose targets live in a graveyard (recursion,
flashback, delve, undying bookkeeping) is currently piloted blind — the model
substitutes confabulation, and reviewers can't attribute zone errors cleanly
between representation and model.

**Proposal.** Add one line per player to the snapshot: `Your graveyard: <cards or
(empty)>` and `Opponent graveyard: ...` (opponent's is public information in
Magic). If prompt size is a concern, cap at the most recent N cards + count, or
include only when nonempty.

## 3. [MEDIUM] Plan carry-over makes procrastination self-reinforcing

**Observed.** The carried `YOUR PLAN (as you last stated it)` line turns "I will
attack / cast it next turn" into a standing instruction the model re-reads and
re-affirms. vs deck131: "attack with my full board next turn" repeated across
consecutive decisions while declaring nothing, opponent at 4 life; vs deck109: the
"on my next turn I will..." formula in nearly every plan while turns 2-5 passed
spell-less. The plan also goes stale silently — seq 23 vs 109 still carried a plan
about a Pillar of Flame that had resolved turns earlier, and the model echoes
stale plan text verbatim into new plans.

**Design question + proposal.** The plan should carry its age. Cheapest: stamp it
— `YOUR PLAN (stated on turn 6; it is now turn 8):` — so "next turn" is visibly
overdue; the prompt-side companion is the general-prompt "that time is NOW" line
proposed in general-suggestions.md §3. A stronger option: expire the plan at the
start of the pilot's own turn (blank it or mark `[from a previous turn - may be
stale]`). I would try the timestamp first; full expiry costs the legitimate
cross-turn intentions (e.g. "hold Push for their attacker") that the mechanism
exists for.

## 4. [MEDIUM] Repeated single-option priority asks burn inference time (timeout economics)

**Observed.** The same single-option fetch activation ("Put in Play with Polluted
Delta targeting Snow-Covered Swamp") is re-asked at every priority window across
phases: vs deck109 seq 3-12 shows the identical offer declined in MP2, End,
Upkeep, MP1 across turns 2-4; vs deck131 seq 3-6 likewise. Each re-ask is a full
round trip to Spark with an ever-growing narration prefix. Deck 133's games ran
33-78 decisions; at 122B latency with long prompts that is the whole 40-minute
budget — the timeouts are decision-count × inference-latency, and passivity
inflates decision count (nothing resolves, so nothing prunes future asks).

**Proposal.** (a) Consider a within-turn "declined this exact action" memo:
if the ask-cache key includes phase, drop phase for single-option priority offers
so one decline holds until the board actually changes. (b) Narration compaction
for old turns (summarize turns older than ~5 into one line each) to cap prefill
growth. (c) Both are secondary to fixing item 1 — active games are shorter games.

## 5. [LOW/INFO] `choice: -1` records (failed decisions)

**Observed.** Occasional records with `choice: -1` and empty/partial replies: vs
deck109 seq 22; vs deck135 seq 24, 26, 35. Rate is low (~4 of 297 deck-133
records) and the Baka fallback appears to absorb them, but they are invisible in
results — a game can silently mix GPT and Baka decisions.

**Proposal.** Log the failure reason (timeout / parse fail / stale-drop) in the
record, and count fallbacks in the harness summary per game so corpora can be
filtered for purity.

## 6. [INFO] Blockers-seam near-absence is downstream of passivity, probably not a bug

3 blockers decisions corpus-wide is consistent with what deck 133's logs show:
neither side deploys creatures (this pilot cast 8 creatures in six games), so
combats are unblockable-or-uncontested by construction; deck 133 specifically also
fields Bloodghast (can't block). Expect blocker decisions to rise sharply once the
cast-nothing pathology is fixed; I would not spend engine effort on this seam
until a post-fix corpus confirms it is still quiet.

## 7. [(d)-layer] Model-weakness notes for prompt/model experiments

- **Confident contradiction of visible state.** The "my lands are tapped" claims
  directly contradict `[tapped]`-annotated battlefield lines in the same prompt.
  qwen does not re-check the board against its own assertions. Experiment: require
  a one-line structured extraction before the choice (e.g. `STATE: untapped_lands=5
  opp_life=4`) — forced grounding may beat any amount of advice; it is also
  machine-checkable in the translog, giving a direct hallucination metric.
- **Self-argument in plans.** Long plans contain audible mid-reasoning reversals
  ("it has haste now? No, it doesn't... Wait, Geralf's enters tapped" — vs deck44;
  the seq 76 vs-131 plan argues itself from a correct intent into the no-mana
  veto). The PLAN field effectively invites chain-of-thought, and the model's own
  confusion then gets carried forward as authoritative context (item 3). Worth an
  A/B: cap plan length (e.g. 2 sentences) vs. current.
- **Zone model is weak** (hand vs graveyard, item 2) and **negation is weak**
  (untapped read as tapped). Both are classic small-model failure shapes — a
  model-comparison corpus (same seeds, larger/different model) would separate
  representation ceiling from model ceiling cheaply.
- **What the model does well** (worth preserving in experiments): impact-ranked
  discard picks (Pillar of Flame vs burn 109, Cranial Plating vs 110, Young
  Pyromancer vs 131), correct multi-attacker arithmetic when creatures exist
  ("3 + 5 = 8, reducing them to 4"), and mulligan reasoning was consistently sane.

## 8. [TOOLING] Translog + results improvements that would have sped this review

- **Record the chosen option TEXT, not just the index.** Reviewers (and any future
  training pipeline) currently re-derive it by regexing the `choice`-indexed line
  out of `prompt` — fragile when numbered lines also appear inside carried plans
  (I hit exactly that false-match on vs-131 seq 68).
- **Top-level `turn` and `phase` fields** per record (currently regexed from the
  narration/situation text).
- **Per-decision latency** (ms) — required for the timeout-economics analysis in
  item 4; currently unmeasurable from logs.
- **A final summary record per game** (result, final life totals, turn count,
  decision counts by kind, fallback count). results.tsv rows for timeouts say only
  `timeout` — I had to open each translog to learn deck 133 was at 16 life with
  the opponent at 4 when the cap hit, which is the difference between "stalled
  while winning" and "stalled while losing."
- **System-prompt provenance**: a hash or path of the system prompt + strategy
  guide in each translog header, so corpora remain attributable after guides
  change (this review depended on knowing the deployed guide text exactly).

## Priority order if effort is scarce

1 (mana line) is strictly first — it is upstream of every strategy artifact and
cheap. Then 2 (graveyard line), then 3 (plan timestamp), then the tooling batch
(8), then 4. Items 5-7 are information for later passes.
