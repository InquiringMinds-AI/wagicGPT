# deck158 — core-prompt proposals (wave 33)

## VERDICT: **PASS.** No core-prompt change proposed from this seat. (28th consecutive)

138 decisions, zero fallbacks, zero retractions, zero parser contradictions, zero
protocol violations. Every defect I found this corpus is engine/render-lane (notes.md)
or guide-lane (strategy.txt). Nothing in the core protocol misfired at this seat, and
the wave-32 flag I raised — the commit-terminator, off deck158's own `retracted_choice`
at 139 s16 — **I am withdrawing**, both because it did not recur (0 retractions in 138
decisions) and because the owner ruling supersedes the mechanism it proposed.

The rest of this file is evidence for the two OWNER DIRECTIVES, per the brief and the
mid-corpus correction. No counter-proposals.

---

## OWNER DIRECTIVE 1 — eliminate the post-choice reasoning block; move reasoning to the request's native channel

### (a) The counted fields, deck158 (the commit-counter's natural seat)

| measure | count |
|---|---|
| decisions | **138** |
| replies carrying a `PLAN:` line | 131 |
| replies with **no** `PLAN:` line (compliant per `f46dd58ee`) | **7** |
| `post_plan_overrun` > 0 | **11 / 131 = 8.4 %** |
| of those, `commit_retracted` | **0** |
| of those, fatal (fallback, lost decision, wrong action taken) | **0** |
| mean `post_plan_overrun` over replies with a PLAN line | 335 chars |
| largest | 146 s19 **11,519** · 146 s22 **11,075** · 116 s2 **6,591** · 146 s27 5,560 · 146 s26 4,801 |

**Promotion trigger NOT met at this seat** (it requires ≥2 seats with a fatal
overrun+retraction; I contribute zero of either).

The `post_plan_overrun` field measures overrun past the PLAN line. The quantity the
owner's diagnosis is actually about — text emitted after the ANSWER — is much larger,
because the protocol *invites* it:

| measure (chars after the first `CHOICE:`/`BLOCKS:`/`ATTACK:` line) | value |
|---|---|
| mean per decision | **2,035** (corpus-wide figure in the brief: ~1,900 — consistent) |
| median | 1,002 |
| p95 | 11,862 |
| max | 14,547 |
| total across 6 games | **280,919 chars** |
| **share of ALL reply text at this seat** | **98.0 %** |

Latency at this seat: p50 17.4 s, p90 80.6 s, p95 **186.3 s**, max **219.0 s**. Reply
length p95 rose 5,287 → **11,906** since wave-32 while the seat's decision quality
improved — i.e. the tax is not buying the decisions.

### (b) Does the 4.2 % post-answer self-correction carry decision VALUE?

At my seat the rate is **6 / 138 = 4.3 %** — matching the corpus figure. In all six the
engine bound the LAST label. I read all six in full:

| seq | first label → last | engine took | value of the re-commit |
|---|---|---|---|
| 36 s16 t8 | Swarming of Moria → **Olog-hai Crusher** | last | **REAL** — the scratch found exact lethal (Army 6/6 +March trigger =7, +Olog-hai 4 = 11 into 11 life) |
| 139 s20 t11 | Cast nothing → **Witch-king** | last | **REAL** — a pass became the play that won the game |
| 139 s23 t12 | B1:A1 → **B1:A2** | last | **REAL** — Witch-king survives the Symbiote block instead of trading with the Greathorn |
| 152 s10 t7 | Briarbridge Tracker → **the face** | last | **REAL** — no toughness-1 creature present; the ping would have bounced off a 4/3 |
| 146 s22 t13 | Feed the Swarm → **Ugluk** | last | neutral (plan-following) |
| **105 s11 t6** | **Ichorclaw Myr (1/1) → Cystbearer (2/3)** | last | **NEGATIVE — it destroyed a correct answer.** The first label was right; 1 damage kills a 1/1 and does literally nothing to a 2/3. The scratch talked itself into "Cystbearer is a bigger threat" and never once asked whether the ping kills it. |

**The honest read: 4 of 6 improved, 1 neutral, 1 actively worse.** And — this is the
part that matters for the ruling — **not one of the four wins required the reasoning to
be POST-answer.** Every one is "the model thought harder and reached a better answer".
Native pre-answer reasoning delivers that same benefit to **100 %** of decisions instead
of 4.3 %, and it removes the 105 s11 failure mode entirely, because there is no first
answer left standing to be argued away from. Nothing at this seat needs replacing that
native reasoning would not supply.

### (c) Was the discarded scratch load-bearing for THIS review?

Yes, in two places, and both are answered by the dev-loop half of the ruling (native
reasoning captured to the translog as `reasoning_content`):

1. **N-158s** (findings §5 R2) was diagnosed from the scratch: the reply naming the
   annotation as its authority (*"as per the 'right now' text: Army 6/6 -> 8/8, damage
   8"*) is what proved the render caused the false belief rather than merely coexisting
   with it. Without a reasoning channel of some kind, that item would have been a mute
   correct choice.
2. **The 105 s11 misplay** is only legible as a *retraction* because both labels are in
   the reply. Under the target protocol it would surface as a single wrong answer — still
   catchable, still traceable to the missing lethality fact, and the guide fix is the
   same. So this is a small loss of forensic resolution, not of detection.

**A harm the counter does not capture, offered as additional support for the ruling.**
The protocol places the PLAN line *after* the scratch invitation, so a spiral that starts
mid-plan writes itself into the persisted plan — the only durable state the pilot carries
between decisions. 146 s19's PLAN line, replayed verbatim into the s20 prompt:
```
YOUR PLAN (as you last stated it): Resolve Orcish Bowmasters targeting Triumphant
Adventurer to destroy it and grow the Army to 10/10. Then attack with the Orc Army
(10/10) and Snarling Warg (4/4) for 14 damage, winning the game (16 - 14 = 2, wait,
16-14=2? No, 16-14=2. Wait, 16-14=2. I need 16 damage to win. Let me re-calculate.
```
146 s23's plan is a quotation of its own previous plan
(`"Cast Feed the Swarm... to clear a potential blocker". This is correct.`). Under
ANSWER + (changed-only) PLAN with reasoning in the native channel, neither shape can
form.

**One implementation fact for the A/B, offered as data not as a proposal:** the engine
currently binds the LAST emitted label (verified in all 6 re-commits above — `choice`
matches the final label every time, including 139 s23 where the reply's first line reads
`BLOCKS: B1:A1` and the engine executed `B1:A2`). Whichever parser semantics ship, the
A/B's decision-quality comparison should be aware that today's baseline is
last-label-wins, not first-label-wins.

---

## OWNER DIRECTIVE 2 — dev-vs-user verbosity split

Seat measurements, offered as the user-cost side of the ledger:

- **98.0 % of all reply text at this seat is post-answer scratch** (280,919 of 286,554
  chars across 6 games, ≈ 46,800 chars ≈ 12k output tokens per game of text that the
  engine discards; deck158's games are among the corpus's shorter ones).
- **Latency p95 186.3 s, max 219.0 s.** The two worst are a **three-option land drop**
  (116 s2, 214.5 s, 14,570 chars) and a Bowmasters target pick (146 s19, 192.1 s, 12,317
  chars). Both were answered correctly in their first line. A user waited ~3.5 minutes
  for a choice the model had already made.
- Reply-length p95 **more than doubled** since wave-32 (5,287 → 11,906) while decision
  quality improved and fallbacks went to zero — the two are not coupled.

Nothing at this seat argues for keeping the verbose register on the shipped path.

---

## OWNER DIRECTIVE 3 (related docket) — consumed-decision narration decoration

Routed to notes.md as **L-158-2**, with the measurement, because it revises the docket's
size estimate upward by 5-6×.

The docket names `{card text: ...}`, `[cost: ...]` and dynamic magnitudes at ~320 chars
in a 12k prompt. **At this seat that decoration measures 0 chars inside the game log** —
already clean. What is not clean is the **menu echo replaying a consumed ask's entire
instruction text in the imperative, as history**: 950 / 1,672 / 1,980 chars in the vs36,
vs152 and vs146 last-prompt logs, i.e. **15.4 % / 18.7 % / 18.1 % of the game log**, and
growing with game length. Specimen (game log, vs36):
```
- TARGET CHOICE for Orcish Bowmasters - its "damage any target" ability (this
spell/ability is already on the stack and needs a target - it is NOT a cast or phase
step). Pick the ONE target it will affect from the list below, and answer with the
chosen TARGET's name (not "Orcish Bowmasters") -> Salvage Slasher (1/1) [opponent's
battlefield] [tapped] - "Salvage Slasher gets +1/+0 for each artifact card in your
graveyard."
```
Everything before the `->` is live instruction addressed to a decision that is over.
The narrated form needs only what was chosen.

---

## Cap / concurrency (`-T 3000`, `-j 3`)

No contribution from this seat beyond the numbers above: **zero truncations, zero
defers, zero fallbacks in 138 decisions**, so the cap never bound here. The two ~200 s
specimens at this seat are the latency-spiral shape the brief describes — non-retracting,
correctness intact — and one of them (116 s2) has an identified engine cause (**N-158r**)
that a cap increase would not touch and that a protocol fix would only partly mask.
