# Wave-34 trace audit — batch 5 (auditor 5 of 6)

Corpus: 7 jsonl files in `~/.Wagic/ai/gpt/logs/`, 110 lines → **102 traces carrying a
`reasoning` field** (81 `ask`, 9 `attackers`, 6 `blockers`, 3 `priority`, 2 `reveal`,
1 `bottom`). Total reasoning: **1,583,855 chars ≈ 396k tokens**. Model `qwen35`.
Log-reading only; no game was run.

Files (short prefixes used throughout):

| prefix | file | decks | traces |
|---|---|---|---|
| 1787165715 | `1787165715-ai_baka_deck105-0x55b7c5a11920-vs-ai_baka_deck158.jsonl` | Infect vs Orcs | 16 |
| 1787172915 | `1787172915-ai_baka_deck158-0x5586c04ede70-vs-ai_baka_deck36.jsonl` | Orcs vs Affinity | 21 |
| 1787178825 | `1787178825-ai_baka_deck146-0x55e1fd17e040-vs-ai_baka_deck105.jsonl` | Orzhov Dungeon vs Infect | 18 |
| 1787186026 | `1787186026-ai_baka_deck152-0x56295e150b60-vs-ai_baka_deck158.jsonl` | Selesnya vs Orcs | 16 |
| 1787193226 | `1787193226-ai_baka_deck116-0x55e5ae8d0150-vs-ai_baka_deck146.jsonl` | Time of Need vs Orzhov | 13 |
| 1787200425 | `1787200425-ai_baka_deck116-0x564ed1887660-vs-ai_baka_deck36.jsonl` | Time of Need vs Affinity | 7 |
| 1787207626 | `1787207626-ai_baka_deck116-0x555dd92a5100-vs-ai_baka_deck139.jsonl` | Time of Need vs Gruul | 11 |

Method: stratified. **28 traces close-read in full** (the single `reasoning_budget_hit`
trace, the 10 longest, all 6 kinds, and a spread of flagged/random asks) — 14 of those
hand-scored for diagnostic depth. Remaining 74 swept by python regex over the jsonl for
`the prompt says / contradict / confus / unclear / typo / I cannot / wait, / actually /
glitch / display error / discrepan / weird / misread`, with every flagged window
close-read as an excerpt. Depth for the un-hand-scored 74 comes from a lexical-novelty
proxy (last sentence introducing ≥4 unseen content words) calibrated against the 14
hand scores — proxy mean churn 0.146 vs hand-scored 0.187, so the reported churn is
**conservative (understates) by roughly 4 pp**.

---

## JOB 1 — Diagnostic-depth measurement

Depth = char offset of the **last novel development-diagnostic element** (false render
belief in the model's own words, invented card fact/rule, verbalized prompt confusion,
first plan derivation, annotation/echo misparse evidence). Correct arithmetic
re-verification, recounting, no-new-belief oscillation and board restatement are churn.

| statistic | chars | ≈ tokens (/4) |
|---|---|---|
| depth p50 | 13,063 | 3,266 |
| depth p90 | 19,434 | 4,859 |
| depth max | 24,500 | 6,125 |
| depth mean | 13,270 | 3,318 |

**Mean post-diagnostic churn fraction: 0.144** (median 0.106, p90 0.334, max 0.547).
Hand-scored subset alone: **mean 0.187**.

Traces needing ≥18,000 chars of depth: 17/102. Needing ≥20,000: 8/102.
Trace length itself: p50 15,223 · p90 22,505 · max 28,309.

Notable: depth is **late-loaded**. The deepest-diagnostic traces put their last novel
element in the final quarter, not the first — `1787186026 seq15` (attackers, 26,457
chars, **1 legal attacker**) does not discover that a Main Phase 2 exists, and therefore
does not fix its plan, until char ~24,500. `1787193226 seq11` (reveal) derives its real
mana shortfall at ~23,800. Truncating these at 12k would have cost the finding *and* the
correct plan. Conversely the model reaches p50 depth by ~13k in the median case.

Only **one** trace hit the budget: `1787165715 seq8` (`reasoning_budget_hit: true`,
15,335 chars). Its last novel element was at ~10,000; the remaining 5,300 chars are six
consecutive re-readings of one boilerplate note (see Finding 1). The budget hit was
**not** caused by demand for depth.

Answer hygiene across 81 asks: `answer_replaced` 0, `commit_retracted` 0,
`post_answer_overrun` > 0 on 41/81 (mean 153, max 270 chars). No dropped assignments.
Reply protocol is healthy; the cost is upstream, in the thinking channel.

---

## JOB 2 — Development findings

### 1. The stale-plan boilerplate note fires when the plan IS available (ENGINE, highest value)

The note appended after `YOUR PLAN (as you last stated it):` —

> `(note: the actions your plan names are no longer among the options available right
> now - the game state has advanced past that plan; re-derive your choice from the
> current board and the options below.)`

— is emitted on **15 of 102 traces**, including cases where the planned action *is* a
listed option. `1787186026 seq16` (ask, 15,321): plan says "Cast Sigarda in Main Phase
2"; option 4 is literally `Cast Sigarda, Champion of Light`.

> "This parenthetical note seems to contradict the fact that Option 4 IS available.
> 'Cast Sigarda in Main Phase 2' was the plan. Option 4 is 'Cast Sigarda...'. So the
> action IS available. Perhaps the note is boilerplate text included in every prompt to
> warn me to check availability. I will ignore the implication that it's unavailable
> since it clearly is." — 1787186026 seq16 @ ~13,700

Same note is the sole occupant of the budget-hit trace's tail. `1787165715 seq8`
(reasoning_budget_hit) re-reads it six times across chars 10,900 → 15,000:

> "This note is a bit confusing. It says 'actions your plan names are no longer among
> the options'. My plan named 'Block Orc Army'. That is not an option. But it also named
> 'play land'. That IS an option." — 1787165715 seq8 @ ~14,900

The note appears to be gated on *partial* rather than *total* unavailability, or on
nothing at all. **Fix: gate it on the whole plan being unreachable, and drop it
entirely when any planned action appears in the list.** This alone plausibly returns the
one budget hit in the batch.

### 2. Degenerate repetition collapse — 13,800 chars of null output (MODEL/BUDGET, severe)

`1787178825 seq4` (ask, 27,792 chars, turn 1, a 4-option land drop). Last novel element
at ~12,600. From ~14,000 to the end the trace is a locked loop over a free-associating
noun list, one line per noun, ~50 chars apart:

> "Wait, I need to check if I need to mention the network. No. Okay.
>  Wait, I need to check if I need to mention the latency. No. Okay.
>  Wait, I need to check if I need to mention the ping. No. Okay. …
>  Wait, I need to check if I need to mention the religion. No. Okay. …
>  Wait, I need to check if I need to mention the profitability. No. Okay."
> — 1787178825 seq4 @ 16,100 – 27,300 (verbatim, ~180 consecutive lines)

It exits only with "Okay, I think I have analyzed enough." Churn fraction **0.547** —
the worst in the batch and by far the largest single waste (≈3,450 tokens). The trigger
is the unresolvable PLAN-line question (Finding 3); the loop is a repetition attractor
the model cannot break without a stop condition. Corpus-wide `"wait,"` count is **1,972
across 102 traces (~19/trace)**; this one trace holds 186 of them.

### 3. The PLAN-line protocol is the largest structural cost: 7.0% of ALL reasoning

**111,468 chars (7.0% of the corpus) across 102/102 traces** are spent litigating when
to emit a `PLAN:` line. Three distinct unresolvable cases, each recurring:

**(a) No prior plan exists.** "ONLY IF your plan changed" is undefined at turn 1.
> "Since there is no previous plan, is it a change? Yes, from null to something. I'll
> include it. … Wait, I need to make sure I don't violate the 'ONLY IF your plan changed'
> rule." — 1787178825 seq4 @ ~9,800 (this is what seeds Finding 2's loop)
> "This implies there MUST be a last stated plan for it to carry forward. So I must state
> one now." — 1787193226 seq1 @ ~14,000
> "Since there is no previous plan, I cannot change it. I will just output the PUT line."
> — 1787178825 seq3 @ ~6,300

**(b) Plan executed early.** Plan said "next turn cast X"; the model can cast X now.
> "The old text says 'Next turn cast Barrowin or Soul Shatter'. After this turn, that
> text is wrong. So I must update it." — 1787178825 seq11 @ ~22,400

**(c) Plan half-consumed / stale.** Part of the plan is done and cannot recur.
> "If I don't write a plan line, the system assumes my old plan carries forward. My old
> plan was 'Cast Grishnakh... Attack...'. That is impossible now (Grishnakh is out, it's
> opponent's turn). So I should write a PLAN line to reset the context."
> — 1787172915 seq10 @ ~14,700

Recommendation: make the rule mechanical and total — e.g. *always* emit `PLAN:` (or
`PLAN: unchanged`), or state explicitly "if there is no prior plan, write one". The
current conditional is the single cheapest budget win available.

### 4. `Your battlefield (creatures: N)` header contradicts its own list (RENDER)

The header counts creatures but the list contains every permanent. Confirmed in two
independent games:

> "Wait, 'creatures: 2' but lists 4 lands? … This formatting is weird. It lists lands
> under 'creatures: 2'? Probably it means 'Your battlefield (Lands: 4, Creatures: 2)'."
> — 1787178825 seq17 @ ~15,600

> "This looks like the log is listing *all* permanents under 'Opponent battlefield', but
> labeling the header 'creatures: 1'. This is contradictory. Silver Myr is a creature
> (1/1). Ancient Den is a land." — 1787172915 seq9 @ ~10,500

Compounding it, **artifact lands render as `[artifact]` with no land marker**, so the
model cannot tell blockers from lands on the opponent's board:

> "Ancient Den (land/artifact?) … Why does the log say 'creatures: 1'? Maybe it's a
> mistake in the log provided in the prompt simulation, or it refers to the Silver Myr?"
> — 1787172915 seq9 @ ~9,800

NEW emitter, not on the docketed list. Cost: ~4k chars in that trace alone.

### 5. Hand render has no duplicate-copy disambiguation → model discards real cards (RENDER)

The battlefield numbers copies (`Forest #1`, `Glimmerpost #2`); the hand does not.
Every duplicate therefore reads as a render bug. Four instances, three decks:

> "'Glimmerpost (land)' … 'Glimmerpost (land)'. Duplicate entry? … The prompt text lists
> 'Glimmerpost (land)' twice. Likely a typo in the prompt generation. **I will ignore the
> duplicate.**" — 1787200425 seq3 @ ~1,400 and ~17,500

That is a **false belief acted on** — the model dropped a real second Glimmerpost from
its model of the hand and reasoned from 7 cards when the render said 8.

> "If Glimmerpost is on the battlefield (4 lands listed), why is it in the hand? … This is
> contradictory. Usually, a card is either in hand or battlefield. … Maybe the prompt
> implies I have *another* Glimmerpost in hand? Or it's a display error."
> — 1787200425 seq7 @ ~2,100

> "the Hand list entry for Quicksilver Amulet is likely a display error in the prompt
> simulation. I should treat Quicksilver Amulet as ON THE BATTLEFIELD. … The Hand list is
> definitely wrong about Amulet." — 1787207626 seq9 @ ~2,800 and ~15,400

> "So I have *two* Intrepid Adversaries? One on the battlefield, one in hand. … This is
> confusing." — 1787186026 seq6 @ ~700

Fix: number hand copies the way the battlefield does (`Glimmerpost #1`, `#2`).

### 6. Illegal targets offered to a "target an opponent controls" spell (ENGINE, card-script)

`1787186026 seq11` — target choice for **Fateful Absence** ("Destroy target creature or
planeswalker **an opponent controls**"). Options 1 and 2 are the model's *own* creatures.

> "Options 1 and 2 are MY creatures. Fateful Absence cannot target my own creatures
> (unless specified, but text says 'an opponent controls'). So 1 and 2 are invalid
> targets logically, even if the game presented them (maybe as a test)."
> — 1787186026 seq11 @ ~6,300

The card script's controller restriction is missing from the legal-target set. Cross-check
against Oracle before touching the engine (per the standing wagicGPT rule). Same trace
also shows the **cast is absent from the game log** before its target sub-prompt:

> "This means I have ALREADY cast Fateful Absence, or it is somehow on the stack. … Why
> is it on the stack? It wasn't in the log as cast." — 1787186026 seq11 @ ~1,400

### 7. Mana line contradicts itself: `Mana available: 1 total` + `Already in pool: {g}{w}` (RENDER)

> "This is slightly confusing phrasing. … If I have {g}{w} in the pool, that means 2 mana
> is ready to spend. But the text says 'Mana available: 1 total'." — 1787186026 seq11 @ ~350
> "This is contradictory. … maybe it's a typo in the prompt generation."  @ ~3,100
> "So where did {G} come from? Ranger Class is an enchantment, doesn't make mana. Maybe
> the prompt text regarding mana is slightly hallucinated or refers to something I
> missed." @ ~4,200

The pool line's provenance is unexplained and its arithmetic does not reconcile with the
availability line. Separately, the availability line's own grammar is ambiguous:

> "'1 total (one untapped source … colours you can make: {b})'. This phrasing is slightly
> ambiguous. Does '1 total' mean 1 mana of any color? Or 1 Black mana?"
> — 1787165715 seq13 @ ~7,400

### 8. Library count renders 0 during a search/reveal (RENDER)

`1787193226 seq11` — Time of Need resolving, 48 cards listed for selection, header says
`Your library: 0 cards`.

> "The library count says 0, which is contradictory to 'looked at the top 48'. Maybe it
> means 'Remaining cards in library after this search'? Or maybe it's just a display
> quirk." @ ~12,100
> "If my library is 0, how can I search it? Time of Need says 'Search your library'. If
> library is empty, the spell does nothing? … The '0 cards' status might be a display
> error or refers to the library *after* the top 48 were pulled?" @ ~18,200

Also **the reveal register describes a tutor as a top-N look**: `"Reveal: you looked at
the top 48 cards of your library"` for a *search your library* effect. The model
concluded "This suggests I am searching the top 48" — a wrong mental model of the effect.

### 9. Reveal log line names a card that was played from hand that same turn (LOG BUG)

`1787178825 seq15` — turn 11 log contains, on consecutive lines, `Your Brightclimb
Pathway: hand -> battlefield` and then `Your Brightclimb Pathway is revealed (from
library) - Your revealed Brightclimb Pathway goes to library`.

> "This seems like a weird log entry, possibly indicating a card was put into the library
> or it's a flavor text quirk." @ ~7,900
> "But Turn 3 log says 'Your Brightclimb Pathway: library -> hand'. So it was in hand.
> Turn 11 says 'Your Brightclimb Pathway: hand -> battlefield' (Play Land). Then 'Your
> Brightclimb Pathway is revealed (from library)'. **This is contradictory.**" @ ~10,300

The reveal emitter is naming the wrong card, or the venture reveal is emitting against a
permanent already in play. NEW emitter (distinct from the docketed dungeon-room gap).

### 10. Invented card rule overriding the printed P/T — Glaze Fiend (MODEL, but render-fixable)

Glaze Fiend's real Oracle is a **triggered, until-end-of-turn** pump. Same game, two
turns apart, the model reads it two opposite ways with the same render in front of it.

Correct read, `1787200425 seq3` @ ~2,100:
> "Glaze Fiend text: 'Whenever another artifact enters under your control, Glaze Fiend
> gets +2/+2 until end of turn.' If the artifacts entered previously, the buff might have
> expired… The prompt explicitly states '(0/1)'. So treat it as 0/1."

Invented static read, `1787200425 seq7` @ ~2,000:
> "Glaze Fiend is 0/1 base, but gains +2/+2 for each artifact. 8 artifacts = +16/+16.
> **So Glaze Fiend is effectively 16/17.**"

It then spends chars ~12,600 → 20,300 (≈7,700 chars, 34% of the trace) computing that it
dies next turn to a creature the board render printed as `(0/1)`. The board's printed P/T
did not anchor it. Cheap mitigation: annotate the board entry with base-vs-current, or
mark until-end-of-turn pumps explicitly in the card-text render.

### 11. Reply-format contradiction between the general and per-decision instructions (PROMPT)

The HOW TO REPLY block states the parenthesised-name format applies to `CHOICE:`,
`ATTACK:` and `BLOCKS:`; the per-decision line for combat shows only `ATTACK: A1, A3`.

> "This specific instruction contradicts the general 'HOW TO REPLY' format slightly
> (doesn't mention parentheses). Usually specific instructions override general ones.
> However, to be safe and consistent with the 'HOW TO REPLY' header … I should probably
> include the name in parentheses if the system expects it."
> — 1787165715 seq16 @ ~13,900

It oscillated chars 12,600 → 16,100 and emitted `ATTACK: A1 (Blackcleave Goblin)` — a
format the per-decision example does not sanction. `reply-format` reasoning appears in
**70/102 traces, 14,867 chars (0.9%)**. Parsed fine here, but this is a live divergence
risk. Fix: exempt `ATTACK:`/`BLOCKS:` from the parenthesis clause, or show the
parenthesised form in the combat example.

### 12. Combat-step prompts do not say what is still possible (PROMPT/RENDER)

The `Phase: Attackers` prompt states neither that instants remain castable nor that a
Main Phase 2 follows. `1787186026 seq15` (26,457 chars, **one legal attacker**) burns
~20,000 chars on this:

> "Can I cast spells before attacking? Yes, usually there's a Main Phase 1 before combat.
> But the prompt says 'Phase: Attackers'. … Did I miss the chance to cast Sigarda?"
> @ ~2,800
> "It does not offer me the option to cast Fateful Absence *before* attacking in this
> specific prompt format." @ ~21,700
> "Wait, I should check if I can cast Sigarda *after* combat (Second Main Phase). … So I
> could cast Sigarda *today* after combat!" @ ~23,100 — the last novel element, and the
> one that materially improved the plan.

Same class in `1787193226 seq8` (priority/Upkeep, 19,043 chars, one listed action):
> "Usually, there are more options like 'Cast Time of Need', 'Play Land', etc. The prompt
> seems to be presenting a specific decision point, possibly generated by an engine that
> filtered options based on priority." @ ~2,100

And in `1787172915 seq12`, a castable-looking card is absent from the list with no reason
given: *"Sauron is in my hand but NOT in the choice list. I cannot choose it based on the
interface provided. … Why is Sauron not an option? Perhaps the system filtered it out."*

Fix: one line per combat/priority prompt stating what phases remain this turn and that
instants are still available. Cheap; it is the deepest churn driver measured.

### 13. Blocker annotations describe creature survival but not damage-to-you (ANNOTATION)

The `B1 … may block A1 (both die), A2 (your blocker dies, attacker lives)` annotation
never states the consequence the decision actually turns on.

> "The annotation 'your blocker dies, attacker lives' implies the attacker survives
> combat. Does it deal damage to me? Standard Magic Rules: Blocked creatures deal damage
> to blockers." — 1787172915 seq10 @ ~4,200

The model had to re-derive it from base rules. Adding "…and its damage does not reach
you" to the annotation removes the derivation.

### 14. Strategy-guide internal contradiction (GUIDE, not engine)

Two blocking clauses conflict: "assign a blocker to the biggest attacker you legally can…
even when the annotation says 'your blocker dies, attacker lives'" vs "If the unblocked
total is already below your life total, block only where the annotation says 'you kill
it, your blocker lives' or 'both die'". `1787172915 seq10` re-litigates the pair **six
separate times** over 23,170 chars. Similar re-litigation of the Rule#5 search ranking
(`Fabricate > Explore > … > Time of Need`) vs Rule#7's Time-of-Need endorsement occupies
~14,000 chars of `1787207626 seq6` and recurs in `1787200425 seq3`, `1787207626 seq9`.
Guide-clause conflicts are a first-class budget cost, not a play-quality cost — the model
reached the same answer every time.

### 15. Trace-level pathologies (budget-relevant structure)

- **Repetition attractor** — Finding 2; one trace, 55% churn, the batch's worst.
- **Token degradation under length** — `1787193226 seq8` writes `Amunt` for `Amulet` from
  char ~11,900 onward, ~15 consecutive times. First observed corruption of a card name
  mid-trace; a length-correlated signal worth watching.
- **Effort decoupled from decision size** — the two longest traces in the batch are a
  1-option attackers declaration (26,457) and a 2-option land drop (28,309). Options
  count does not predict length; **prompt ambiguity does**.
- **Bounded oscillation** — the common non-pathological shape: 3–6 re-readings of one
  boilerplate clause, converging. Median trace carries ~19 `"wait,"` markers; most are
  this. It is the tax the findings above are levying.

---

## Supported budget

Depth p90 is **19,434 chars ≈ 4,900 tokens**; max is **24,500 ≈ 6,100 tokens**. Setting
the reasoning budget at **~6,500 tokens (≈26,000 chars)** covers every diagnostic in this
batch with headroom, and would have truncated nothing of value — the only trace that hit
the current budget hit it on boilerplate churn, not on depth.

A tighter **~5,000 tokens (20,000 chars)** covers p90 and costs 8/102 traces their final
novel element — but note *which* 8: they include the two traces where the last novel
element materially changed the play (`1787186026 seq15` finding Main Phase 2 at 24,500,
`1787193226 seq11` finding its mana shortfall at 23,800). For a development corpus whose
only harvest is the traces, the wider budget is the right call.

The real lever is not the ceiling. **Fixing findings 1, 3, 11, 12 and 13 — all prompt or
annotation edits, no engine work — attacks the ~14% mean churn plus the 7.0% PLAN-line
tax directly**, and would let the same depth fit in a materially smaller budget.
