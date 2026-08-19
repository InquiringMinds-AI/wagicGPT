# deck36 → core / general-prompt proposals (wave 33)

**Core general prompt: PASS (28th consecutive).** No sentence of the general prompt was cited,
misread, or quoted-and-inverted at this seat in 201 asks. The two rules the general prompt owns that
have historically failed here — generic-cost payability and the free-spell class — both held at
0/201. Nothing routes to a new core paragraph.

Two items route to CORE PROTOCOL / REPRESENTATION rather than to the general prompt's prose, plus
the directed evidence contribution on owner-directive item 1.

---

## C1 — PHASE-HEADER CONTRADICTION: **SECOND WITNESS. PROMOTE.** Name the turn OWNER before the phase.

**Verdict on the wave-32 convergence check: the second witness exists and it is this seat.**

**Wave-32:** one seat verbalized it — deck36, vs152 s15/s16: *"the prompt says 'Phase: Main phase 1 |
It is the opponent's turn.' This is a direct contradiction. Let's assume the game is actually in my
Main Phase 1."* Recorded as low-volume, cross-deck shape, held pending a second witness.

**Wave-33:** it recurred, on a different corpus, a different binary, and a different game state
(vs152 s21, kind=priority, 177 s / 11,969 chars, chose `pass`):

> *"However, looking closely at the 'Current Situation' block, it explicitly states **"Phase: Main
> phase 1 | It is the opponent's turn."** This is a contradiction in the provided text (Main Phase 1
> is typically a player's own turn)."*

**Rate at this seat: 1 / 42 opponent-turn main-phase priority windows.** Low, and the rate is not the
argument — the argument is that the same string produced the same false-contradiction inference twice,
across a binary change that touched the priority path (`bdd07223c` defender's blocker window opens on
the attacker's turn), and the pilot resolves it by DISCARDING the rendered turn owner. In wave-32 it
resolved to *"assume the game is actually in my Main Phase 1"* — a wrong belief about whose turn it
is, held while deciding whether to spend mana.

**Cost when it lands.** Both witnesses are among the seat's most expensive replies of their corpus.
The wave-33 instance is 177 s and 11,969 chars in a game that was ADJUDICATED AT THE CAP two turns
later, at 6-12 — this seat spent 1,095 s of a shared 3,000 s budget. It is not the only reason that
game ran out of clock, but it is 16% of this seat's whole reply time in that game, on a `pass`.

### The representation change

The header is `Phase: <phase> | It is <the opponent's|your> turn.` The phase name is read first and
becomes the frame; the ownership clause arrives after the pilot has already decided whose main phase
it is looking at, and loses. Every English MTG habit the model has says "Main Phase 1" means "my main
phase" — the ownership clause is fighting an idiom from behind.

**Proposed: name the owner FIRST, and make the phase possessive.**

```
It is the OPPONENT'S turn | Phase: their Main phase 1 (you have priority)
It is YOUR turn | Phase: your Main phase 1
```

Three properties, each doing work:
1. **Owner leads.** The frame is set before the phase name can capture it.
2. **The phase is possessive** (`their Main phase 1`), so the contradiction cannot be constructed —
   there is no bare "Main phase 1" left to attach the wrong owner to.
3. **`(you have priority)`** on the opponent's turn answers the follow-on question the pilot asks
   both times — *"Can I activate this on the opponent's turn?"* (vs152 s21 spent a paragraph on
   exactly that, about Academy Ruins).

Zero behaviour change, one format string, and it is a strict improvement on the negative control
(the header on YOUR turn reads at least as clearly).

**Falsifier for wave-34:** if the string ships, zero replies containing the word "contradiction"
within 200 chars of "Main phase" across the pool. Baseline: this seat 1/42 windows; wave-32 same seat
2 replies. **Report the fraction at every seat**, since the promotion rests on a low per-seat rate
holding across many seats rather than on any one seat's volume.

---

## C2 — THE COMMIT COUNTER NEEDS A THIRD FIELD: a post-ANSWER boundary, not a post-PLAN one. One line of code.

**The instrument gap, stated precisely.** `postPlanOverrun` (`AIPlayerGPT.cpp` ~2210-2245) measures
characters after the first `PLAN:` line. `commitRetracted` is the disjunction of three existing
fallback exits. The reply protocol puts `PLAN:` LAST. Therefore a reply that **answers, reasons at
length, and then re-answers** scores `post_plan_overrun` 0 and `commit_retracted` false — and the
parser executes the second answer.

**Measured at this seat: 8/201 replies (4.0%) did exactly that.** Verbatim on the one that decided a
game (vs146 s18, 78 s, `post_plan_overrun` 0, `commit_retracted` false):

```
CHOICE: 2 (Cast Yotian Soldier {3} (1/4))
… 4,600 chars …
CHOICE: 1 (Cast Akroma's Memorial {7})
PLAN: Cast Akroma's Memorial to grant all creatures Flying, First Strike, …
```

`chosen_text` in the translog is Akroma's Memorial. Akroma's resolved on turn 9 and won the game on
turn 13.

**Why this matters to the owner's directive rather than being a bookkeeping nit.** The directive's
promotion trigger is "≥2 seats with a fatal overrun+retraction". At this seat that count is **0**,
and it would be 0 even if every reply re-answered, because the fields cannot see re-answering. A
synthesis that reads only the shipped fields will under-count the phenomenon the wave-34 protocol
change is being sized against, in the direction that makes the change look cheaper than it is.

**Proposed field: `post_answer_overrun`** — characters after the end of the FIRST line-leading coded
answer (`CHOICE:` / `ATTACK:` / `BLOCKS:` / `PUT:`), trailing whitespace excluded, on the same
`</think>`-stripped normalisation the existing fields use. It reuses `hasCodedAnswerLine`'s scanner
and adds no behaviour.

**Proposed companion boolean: `answer_replaced`** — true when a line-leading coded answer occurs 2+
times and the last differs from the first. This is the exact class the corpus-wide 4.2% statistic
already counts by post-hoc regex; putting it in the record makes it cross-tabbable against latency
and against the fatal/non-fatal split without a re-parse.

At this seat those two fields would read: `post_answer_overrun` nonzero on essentially every reply
(mean reply 2,008 chars, and the answer is on line 1), and `answer_replaced` true on 8/201 — the
number the directive actually needs.

---

## OWNER-DIRECTIVE ITEM 1 — EVIDENCE ONLY (no counter-proposal offered; the ruling is binding)

Per the directive, this seat contributes (a) counted fields, (b) whether the post-answer
self-correction carries decision VALUE, (c) whether discarded scratch text was load-bearing for
review. No reordering, no brevity clause, no terminator is proposed.

**(a) Counted fields, 201 asks.**
- `post_plan_overrun` > 0: **25/201 (12.4%)**. Mean of the nonzero **2,275** chars; max **7,777**.
  Full set: 7777, 7417, 7312, 6716, 5787, 3997, 3653, 3554, 1401, 1338, 1308, 1267, 993, 890, 787,
  637, 428, 401, 327, 299, 262, 178, 66, 59, 17.
- `commit_retracted`: **0/201**. Of the 25 overruns: retracted **0**, fatal **0**.
- `pruned_pairs`: never written.
- Replies with no `PLAN:` line: **6/201**, all compliant under interim `f46dd58ee`.
- Latency: p50 18.1 s, mean 31.1 s, max 227 s. Mean reply 2,008 chars, max 13,930.
- **Promotion trigger for the commit-terminator is NOT met at this seat** (0 fatal overruns with
  retraction). See C2 for why that zero is weaker evidence than it looks.

**(b) Does the ~4.2% post-answer self-correction carry decision VALUE that native pre-answer
reasoning would need to replace? At this seat, YES, in 8 of 8 — and one is the deciding play of a
win.**

| game / seq | first answer | final answer (executed) | value |
|---|---|---|---|
| **146 s18** | `2 (Cast Yotian Soldier {3} (1/4))` | **`1 (Cast Akroma's Memorial {7})`** | **GAME-DECIDING.** Won turn 13 off the seven keywords Akroma's granted. Found in the post-answer block: *"With Akroma's Memorial, Cathodion has First Strike. It kills the 1/1 Adventurer before it deals damage… This is a huge difference."* |
| 116 s31 | `8 (Tolarian Academy)` | `4 (Island #1)` | keeps its 8-mana land under Annihilator |
| 116 s38 | `9 (Glaze Fiend)` | `1 (Mox Jet)` | keeps its only creature |
| 116 s39 | `8 (Glaze Fiend)` | `4 (Nuisance Engine #1)` | same, again |
| 158 s15 | `1 (Play Swamp)` | `2 (Play Seat of the Synod)` | artifact land = +1 artifact count |
| 152 s11 | `1 (Play Swamp)` | `2 (Play Seat of the Synod)` | same |
| 116 s14 | `2 (tap Tolarian Academy)` | `1 (Create pest with Nuisance Engine)` | correct order — the Pest must exist before the Academy counts it |
| 158 s30 | `1 (Put in Play with Scarecrone targeting Salvage Slasher)` | `2 (Draw 1 with Scarecrone)` | defensible either way; deliberate |

**8/8 improved or held; 0/8 degraded.** The value is concentrated exactly where the directive's
diagnosis predicts it would be — decisions with a non-obvious second-order consequence (a keyword
grant, a sacrifice priority, an activation ordering), i.e. the decisions that need reasoning at all.

**The consequence for the target protocol, stated as evidence and not as a counter-proposal:** the
decision value here is real, so the ANSWER + optional-PLAN target protocol only preserves it if the
native reasoning channel is actually enabled in the dev loop and, for the shipped user default,
only if the quality cost of turning it off is measured rather than assumed. The directive already
specifies both (native reasoning ON + captured to translog for dev; OFF for users) and the A/B
corpus it names is exactly the measurement this table argues is necessary. This seat's contribution
is the magnitude: at deck36, removing the post-choice block WITHOUT a replacement channel would have
cast Yotian Soldier over Akroma's Memorial and most likely lost the vs146 game — 1 of the seat's 3
wins.

**(c) Was the discarded scratch text load-bearing for REVIEW? Yes, twice, and both times it was the
only record of a live defect.**
- vs105 s35's ABANDONED first plan (char 501, superseded by the real one at char 5,359) is what
  exposed N-36e — the plan splitter anchors on the FIRST `PLAN:`, so the next prompt was fed 1,424
  chars of retracted reasoning as "YOUR PLAN (as you last stated it)" while the committed plan was
  discarded. The bug is undiagnosable from the committed plan alone.
- vs116 s30/s38's *"The prompt says 'Choose card 1 of exactly 6'. This is confusing. Maybe the list
  is truncated"* is the sole evidence for N-36j, the sequential-sacrifice header defect that
  contributed to the seat's 1,910 s / 42-ask blowout in a cap-adjudicated loss.

**Both survive intact under the target protocol**, since the directive routes native reasoning to
`reasoning_content` in the translog for seat review. No objection is raised, and none of the review
value depends on the scratch text remaining in the REPLY.

**One adjacent measurement the directive's item 3 asked for, at this seat.** Consumed-decision
narration decoration (`{card text: "..."}` / `[cost: ...]` inside the GAME LOG block, e.g.
`- You: add blue mana for each artifact with Tolarian Academy [cost: Tap] {card text: "{T}: Add {U}
for each artifact you control."}`): **72/201 prompts carry any; p50 0, mean 81 chars, max 501**,
against a p50 prompt of 5,754 chars and a max of 20,153. Confirms "measured small" — it is
wrong-by-architecture, not a cost driver, and stripping it will not move the clock.

**On cap 3000→3600 vs -j 2 (synthesis weighs this; one data point offered, no recommendation).**
This seat's vs116 loss consumed 2,570 s of the 3,000 s cap across both players, of which **1,910 s
(74%) was this seat's own replies over 42 asks** — and **831 s of that (44% of its budget) went to
six decisions, four of which were "which of my tapped lands do I throw away"**. A cap raise buys that
game about two more turns; the guide's new sacrifice section and N-36j's header fix are aimed at the
same 831 s directly. The two are not alternatives, but the cheap one is not the cap.
