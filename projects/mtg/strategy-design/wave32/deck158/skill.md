# deck158 — proposals for the strategy-writing skill (wave 32)

Seat context: 2nd corpus, FIRST GUIDED outing of an initial guide drafted from a 5/6 Step-0 debut.
3/6, three clean kills, median winning turn 13 -> 9. Two step-1 items under validation here
(i12 mana line, i14 magnitude cluster). Every proposal below is earned by a witness in
findings.md and is cited to it.

---

## P1 (PROMOTION-GRADE, mechanical, cheap) — A guide sentence that QUOTES a rendered string is a VERSIONED DEPENDENCY. Grep every quoted render substring against the current corpus before you read anything else.

**Witness.** The deployed deck158 guide's Rule #1 — its #1 slot, 14 lines — opened with
*"The board line reads like 'Mana available: {r}{b} from 5 untapped sources'."* The i12 fix
replaced that emitter with `Mana available: 5 total (five untapped sources, ...)`. **The quoted
string has zero occurrences in the entire wave-32 corpus.** The rule was not merely stale: it
instructed the pilot to interpret a line it would never see, in a guide that rides in every
single prompt.

**Why the existing rungs miss it.** Wave-31 HL10 (guide/annotation reconciliation) asks *"did the
engine change make any sentence of the guide FALSE?"* and its procedure is "extract the surfaces
the h-batch touched, grep the guide for each surface's VOCABULARY." That catches semantic
contradictions. It does not catch a guide sentence that has become *unreachable* — Rule #1's
vocabulary ("mana", "untapped sources") is all still live, and nothing it says is false about
Magic. It is false about the PROMPT.

**THE RULE.** Before reading logs, extract from the deployed guide every substring presented as a
quotation of something the pilot will SEE — a render line, an annotation, an option label, a tag.
Grep each one, verbatim, against the corpus's `prompt` fields. **A quoted render string with zero
corpus hits is a defect at the same severity as a false card fact, and it is repaired or deleted
before any revision budget is spent.** Record the sweep as a line item in findings.md alongside
the HL1 card-fact audit; both are cheap, both are mechanical, and both close a blind spot the
freeze machinery cannot see by construction.

**Why it generalises beyond render churn.** Every wave ships render fixes, and the better a guide
is at pointing the pilot at concrete surfaces (which the skill demands — "quote the render
substring the pilot must read", wave-31 HL10), the MORE quoted strings it accumulates and the
larger its exposure. This rung is the maintenance cost of the skill's own best practice.

---

## P2 (adopt as a refinement of wave-31 HL9's tax-spike instrument) — A cured belief is usually REPLACED, not removed. Before you retire a stopgap, re-read the top tax rows and name what the model is rehearsing NOW.

**Witness.** Wave-31's S-158b established the instrument: sort replies by length, read the
LOW-BRANCHING rows first, because a 13k-char land-drop reply is the model rehearsing a belief it
cannot settle. It found the mana-pool misread and produced the i12 fix.

Wave-32: the false-decline class is dead (0/33), p95 reply length fell 46%, and the land-drop rows
**survived** — 10,763 chars / 161.8s on `Play Swamp` (vsk36 s16). Reading that row is what
distinguishes two very different retirements:
- The row contains **no mana doubt at all** (*"I have 5 untapped lands ... With 5 sources, I can
  easily cast a 2-mana sorcery"*). The stopgap's belief is genuinely gone -> retire it.
- But a DIFFERENT belief surfaced in the same corpus, one seam over:
  *"Grishnakh cost: {2}{R}. I need 2 generic and 1 Red. I have 2 sources. Can I pay {2}{R}? Yes,
  2 sources = 2 mana"* (vs139 s16). The model now trusts the count line and **mis-parses the COST
  notation** — the exact inverse error.

Had the reviewer stopped at "class dead, p95 down, delete Rule #1", the guide would have shed the
stopgap and shipped nothing for its successor.

**THE RULE.** A stopgap retirement is a two-step verdict, not one: (a) is the ORIGINAL class dead
(the counted audit), and (b) **what does the residual tax rehearse?** Answer (b) by re-reading the
top three tax rows and the replies of any surviving fallback, and state one of exactly two
outcomes — *"the tax residue is not a belief"* (retire cleanly; here the residue was whole-turn
planning displaced onto the cheapest seam, L-158-1), or *"the successor belief is X"* (DEMOTE the
stopgap to one line aimed at X, per the falsity-fact construct). The successor is cheap to install
because the slot is already being vacated; discovering it two waves later costs a game.

---

## P3 (PROMOTION-GRADE; hardens wave-31 HL5) — Path-scope a fix by its DATA indirection, not only its code paths — and report an annotation's coverage as a FRACTION of live offers, never as "it renders".

**Witness.** i14's amass preview was specified over "the four fixed-count cards (Foray x2 paths,
Grishnakh, Widespread Brutality, Surrounded by Orcs)" — an enumeration derived from cards whose
`auto=` script writes `counter(1/1.N)` LITERALLY. Measured on the live corpus at the deck the
annotation was built for:

| card | cast offers | with `{right now: Army X/X -> Y/Y}` |
|---|---|---|
| Foray of Orcs | 3 | 3 |
| Easterling Vanguard / Mordor Muster / March / Bowmasters / Crebain / Swarming | 49 | **0** |

**3 of 52 (5.8%).** Mechanism, read from source: `amassCountersFromScript` scans
`card->magicText` for `counter(1/1` (`src/AIPlayerGPT.cpp:246-259`), but macro expansion runs on a
LOCAL copy inside the ability factory (`src/MTGAbility.cpp:6650`) and is never written back to
`CardPrimitive::magicText`. Every card using the `_AMASSORCn_` macro (`_macros.txt:68-72`) —
which is every amass card in the deck except Foray — presents the scanner with an opaque token.

**Why HL5 as written does not catch this.** HL5's sweep is over EMITTERS ("enumerate every path
that emits that surface, grep the new string and the old string"). Here there is exactly ONE
emitter and it works perfectly; the old string is gone; a grep of the new string finds hits. The
scope failure is on the INPUT side — the fix reads card data through one representation and the
data has two.

**THE RULE, two halves.**
1. **When a fix's trigger is derived by SCANNING CARD DATA, enumerate the data's indirection
   layers before you claim coverage** — macros (`#AUTO_DEFINE`), `transforms(...)` payloads,
   `ability$!...!$` wrappers, alternative-cost faces — and state which representation the scan
   sees. "The scanner reads pre-expansion text" is a one-line check that would have caught this
   at design time.
2. **An option-line annotation is validated by a COVERAGE FRACTION on the live corpus, per card:
   offers-with-annotation / offers-total.** Never by "it rendered correctly" plus a sample. The
   fraction is free to compute (one regex over the corpus's option lines) and it is the only
   number that distinguishes a shipped annotation from a shipped annotation nobody sees. It also
   turns the reviewer's null into a fix target instead of a shrug: 49 blank offers names the
   defect, "Foray renders correctly" hides it.

This is the third path-scoped recurrence class in two waves and the first one on the INPUT side,
which is why it deserves its own half-rung rather than a caution appended to HL5.

---

## P4 (adopt; consumer-seat rule applied to multi-step choreography) — In a two-step decision, the deciding number belongs at the step where the choice is COMMITTED, not only where it is announced.

**Witness.** i14 gave Feed the Swarm per-target life costs, and they work:
`2. Cast Feed the Swarm {1}{b} - legal targets right now: Ornithopter (costs you 0 life),
Salvage Slasher (costs you 2 life), Cathodion (costs you 3 life)` (vsk36 s9). At vsk36 s17/s18
the pilot used exactly those numbers to pick the 0-life target and killed on the swing.

But casting and targeting are two engine decisions. The second one renders
`- TARGET CHOICE for Feed the Swarm ... Ornithopter (0/2) [flying] [opponent's battlefield] -
"Flying"` — rules text, **no cost**. The pilot commits the target at a seam where the deciding
number is absent, and only survives by having carried it from the previous ask, across an
intervening board snapshot. At vs152 s26 and s31, on 11 and 6 life, the target menu was answered
with no price visible at all.

**THE RULE.** When a magnitude annotation is added to an option line for a decision that will
later re-ask for a component of that same choice (targets, modes, X, damage order), the annotation
belongs on BOTH surfaces — and the reviewer verifies it at the LATER one. Phrase the check as the
skill already phrases the consumer-seat rule: *the seat that needs the number is the seat that
commits, not the seat that announces.* Costs one grep per annotation.

---

## P5 (PROMOTION-GRADE, corpus-integrity instrument) — Before a wave reads ANY win record, verify that every ALTERNATE WIN CONDITION in the pool is rendered. An unrendered clock silently adjudicates the whole corpus.

**Witness.** deck158 lost to deck105 at `Your life: 15 | Opponent life: 8` on turn 13, ahead on
life and board, with the opponent's only creature tapped. It died to poison counters, and
**poison is rendered nowhere at either seat.** The situation block carries `Your life:` /
`Opponent life:` and no poison line; the narration prints `- Damage: 5 dealt by Putrefax to you`
and `- Damage: 2 dealt by Blackcleave Goblin to you` **with no accompanying life change and no
counter line** — an event whose entire consequence is invisible. The infect player's own prompts
are equally blind (`Your life: 3 | Opponent life: 15`, three infect creatures, no count).

deck105 went **6/6 — the first Step-0 perfecta in the campaign** — on a clock neither pilot could
read, and the wave-32 brief already frames that record as a headline result. Every seat that
faced it has a loss it could not have played around, and every conclusion drawn from those six
games about guides, teaches or decision quality is contaminated.

**Why this is not covered.** The skill's nulls are all about MECHANICS THE CORPUS DID NOT SAMPLE
(wave-30 seeded-but-unexercised, wave-31 HL4 trigger predicate). This is the opposite failure: the
mechanic fired in six of six games and DECIDED them, while being absent from the only channel the
pilot has. The perception-vs-strategy routing predicts the outcome exactly — a state the render
omits cannot be cured by any guide prose, so no amount of deck105 guide work would touch it.

**THE RULE.** At corpus intake, before the win table is written: enumerate every ALTERNATE WIN OR
LOSS CONDITION in the pool's decklists (poison/infect, mill/empty-library, "you win the game"
static abilities, life-total thresholds, counter-based alt-wins), and for each one grep the corpus
prompts for its STATE VARIABLE — the counter, the library count, the threshold. **A pool win
condition with no rendered state variable is a P0 render item, and every record in that corpus is
annotated with it before it is interpreted.** The check is one grep per condition and it runs once
per pool composition change, i.e. exactly when new decks rotate in — which is exactly when the
risk exists.

Corollary for the reviewer: **when a loss's final board shows you AHEAD on the rendered
statistics, the first hypothesis is a hidden state variable, not a misplay.** deck158's vs105 loss
looks like nothing else in the corpus.

---

## P6 (adopt; small, sharpens the fallback taxonomy) — Classify a `retracted_choice` by WHERE the retraction begins: before the PLAN line (a two-rule deliberation loop) or after it (a COMMIT failure). Different defects, different layers.

**Witness.** vs139 s16, 12,180 chars / 206s, the corpus's largest reply. It opens with a complete,
correct, protocol-compliant answer — `CHOICE: 1 (Cast Mordor Muster {1}{b})`, full reasoning, and
a `PLAN:` line — and then continues for ~10,000 more characters (`"Wait, looking at the
opponent's board..."`, `"Actually, let's re-evaluate..."`) without ever re-committing. The safety
route correctly refused a retracted answer; a CORRECT decision was discarded and Baka answered.

The existing rungs record retracted_choice as a parser-safety event (wave-26: the c4 line-anchoring
firing as designed) and record oscillation loops as a GUIDE bug (wave-20's two-imperative rung —
the tell being reasoning that oscillates between two of YOUR clauses on an early low-stakes turn).
Neither describes this: there is no second imperative in tension, the guide is not quoted, the
turn is not low-stakes, and the answer was already produced. The trigger shape is a hopeless board
(life 2, opponent lethal next turn), a two-option list, and a self-audit that starts AFTER the
protocol's own terminator.

**THE RULE.** Report the retraction's POSITION relative to the PLAN line as a required field.
*Pre-PLAN* -> route to the guide layer (two-rule tiebreak, wave-20). *Post-PLAN* -> route to the
reply-protocol layer as a COMMIT item ("your answer ends at the PLAN line"), and note that the
first-line answer's correctness is recoverable evidence: a post-PLAN retraction that discarded a
CORRECT answer is a pure loss with no compensating safety benefit, which is the strongest case the
protocol layer will ever get. Do not propose the protocol change from one seat — but do record the
position, so convergence can be seen when a second seat produces one.

---

## P7 (small, corrects a reporting shape in Step 0-ter) — The teach-firing table needs a third verdict: SUPERSEDED BY THE RENDER. It is neither a firing success nor a falsified teach.

**Witness.** The deck158 guide carries *"SUMMONING-SICK OPPONENT CREATURES CAN STILL BLOCK. The
tag '[summoning sick - cannot attack this turn]' restricts ATTACKING only."* Wave-31 wrote it to
cure a real, game-costing misread (C3). In wave-32 the engine prints
`[summoning sick - cannot attack this turn, but CAN block]` — 95 occurrences in my prompts, zero
misreads, zero paradox verbalizations.

The teach's CLASS is dead. But the guide sentence did no work: the render did. Reported as
"teach fired", it inflates the guide's credit and the line survives every future freeze. Reported
as "teach failed", it invites a rewrite of something that is correct. It is neither — it is
**superseded**, and the correct action is to DELETE it and credit the engine.

This is adjacent to but distinct from HL10 (which asks whether the engine made a sentence FALSE —
this one is still true) and from wave-30 stopgap retirement (which is about a stopgap for a
temporary defect — this was a knowledge teach). **THE RULE:** the firing table's verdict column
takes four values — FIRED / MISSED / FALSIFIED / SUPERSEDED — and a SUPERSEDED teach is deleted in
the same revision that records it, with the superseding render string quoted so the deletion is
auditable.

---

## Not proposed (considered, rejected)

- **A blocker-seam floor as a general rung.** deck158's fatal blocker fallback (§4 R1 of findings)
  is real, but the skill already holds the per-seam-floor rung (Step 5) and the deck139/deck109
  lineage; this seat is one more instance, not a new shape. Handled entirely in strategy.txt.
- **"Do not attack with borrowed creatures."** vs152 s32 attacked with a stolen Sigarda and the
  crack-back contributed to the loss — but the steal is `until end of turn` and NOT attacking with
  it forfeits the card's entire purpose. The loss traces to s35, not s32. Legislating here would
  be the deck135 pursuit-overshoot shape against a seam with a 21/21 record.
