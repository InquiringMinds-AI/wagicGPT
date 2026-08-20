# Wave-35 step 1 — engine + instrumentation verdicts on the wave-34 audit

Every card claim below was checked against the card's REAL Oracle text on Scryfall
(`api.scryfall.com/cards/named?exact=...`, fetched 2026-08-20) BEFORE the script or the
engine was touched, per the standing rule. Three of the five engine-tier findings turned
out to be the audit's own rules errors — two of them repeats of the hallucinated-Oracle
class the campaign already tracks (an agent asserting a clause the card does not have).

## 1. Scarecrone's sacrifice cost is never paid (b6 F1) — **FIXED (engine)**

**Oracle**: `{1}, Sacrifice a Scarecrow: Draw a card.` / `{4}, {T}: Return target artifact
creature card from your graveyard to the battlefield.` — Artifact Creature — Scarecrow 1/2.

- The companion claim (b2 C1 / b6 F1b: "real Oracle is *Sacrifice an artifact creature*")
  is **WRONG**. `mtg.txt`'s `text=` and `auto={1}{S(scarecrow|myBattlefield)}:draw:1` are
  FAITHFUL. No card-script change was made or is warranted.
- **Real defect, and it is general, not Scarecrone-specific**: `AIPlayerBaka::chooseCard`
  — the scan behind both `chooseCostTarget` and the `CanHandleCost` probe — skips
  `card == source` and `card == tc->source`. Right for chooseCard's normal job, fatal for a
  COST whose only legal payment is the ability's own source: the scan returns NULL,
  `payTheManaCost` returns false, no AIAction is queued, and the activation vanishes with
  no fallback, no defer and no counter — while `SacrificeCost::canPay()` (which does NOT
  exclude the source) keeps reporting it payable, so the engine re-offers it every priority
  window. Hence 12+ takes, 0 sacrifices, and 18 of that file's 30 decisions spent on the
  contradiction. `AIPlayerGPT::chooseCostTarget` mirrored the exclusion, so both seats
  fizzled; Baka additionally scored the ability 0 through `AIAction::getEfficiency`.
- **Fix**: `AIPlayerBaka::selfAsCostPayment` — after the ordinary scan finds nothing, offer
  the source itself iff it is ON THE BATTLEFIELD and the cost's own TargetChooser accepts
  it. Legality stays the chooser's call, so an `other`/`another` filter still excludes the
  source exactly as the rules do, and a spell being cast from hand can still never pay with
  itself. Applied at `chooseCostTarget`, at the `CanHandleCost` probe, and (as a
  battlefield-scoped carve-out) in the GPT seat's candidate filter.
- **Fixture**: `test/scarecrone_self_sacrifice_cost_wave35.txt` — RED on the pre-fix binary
  (Scarecrone still in play, library 5 not 4, nothing drawn, and it even attacked for 1),
  GREEN after. Registered in `_tests.txt`.
- **Prediction (falsifiable, wave-35 corpus)**: zero traces in which the same activated
  ability with a `Sacrifice`/`Tap`/`Discard` extra cost is offered and taken more than
  twice without its cost being paid; Scarecrone activations arrival-trace 1:1 with a
  Scarecrow leaving the battlefield.

## 2. Fateful Absence "offers the caster's OWN creatures" (b5 #6) — **ENGINE RIGHT, trace wrong**

**Oracle**: `Destroy target creature or planeswalker. Its controller investigates.` There is
**no "an opponent controls" clause**. The model quoted one ("text says 'an opponent
controls'") and the auditor adopted the quote; the rendered `text=` in `borderline.txt` does
not contain it either. `target=*[creature;planeswalker]` is faithful, and offering the
caster's own creatures is rules-legal (a real MTG play: dodging exile, triggering a death
payoff, or making a Clue while removing a liability).

Per the owner ruling, a legal play is never suppressed. Rerouted to the annotation lane, and
the surface already carries the warning shape the audit wanted — the same emitter that
produced *"the only legal targets are YOUR OWN right now (warning: this would harm your own
side...)"* in b6 F2. No change. The separable half of that finding — the cast missing from
the log before the target sub-prompt — is a NARRATION item and stays docketed for that lane.

## 3. Artifact count off-by-one vs Master of Etherium (b2 R5) — **ENGINE RIGHT, both surfaces true**

**Oracle**: P/T each equal to the number of artifacts you control (Master is an artifact, so
it counts itself); "**Other** artifact creatures you control get +1/+1" (never itself).

The render was `Master of Etherium #1 {2}{u} (8/8)` against `Artifacts in play: you 7`. The
tell is `#1`: `instanceHandle()` emits a `#N` suffix ONLY when the controller's battlefield
holds two or more permanents of that name, so that board had **at least two Masters**. Seven
artifacts → each Master is 7/7 from its own CDA and 8/8 after the OTHER Master's lord. Both
numbers were true; the missing term was the second Master's +1/+1, applied by neither the
model nor the deck guide. The count line is not undercounting and must not be "fixed".

- Cross-check, pre-existing: `test/master_of_etherium.txt` (one Master + 2 other artifacts)
  attacks for exactly 3 — the CDA counts itself and does not self-lord.
- **Fixture added**: `test/master_of_etherium_two_copies_wave35.txt` — two Masters + Mind
  Stone, one Master attacks for **4**, not 3. Passes as written (no code change).
- **Guide action (other lane)**: the deck guide's "Master lands one bigger than the line
  says" is correct for the CAST case (Master is not on the battlefield yet when the line is
  computed) but must also teach the multiple-Masters term.

## 4. Vanishing Verse "excludes opponent lands" (b6 F2) — **ENGINE RIGHT, trace's rules error**

**Oracle**: `Exile target monocolored permanent.` Basic lands and almost all other lands are
**colorless**, not monocolored, so the opponent's `Forest`/`Island` were never legal targets.
`target=*[-multicolor;-colorless]|battlefield` expresses "monocolored" exactly. The engine's
"the only legal targets are YOUR OWN right now" annotation was TRUE for that board. No change.
(The three copies the model held dead in hand were held on the model's own rules error, not
on a false surface — a guide/annotation matter, not an engine one.)

## 5. Glaze Fiend read as static +2/+2-per-artifact (b4 F9, b5 #9) — **SCRIPT RIGHT, rerouted**

**Oracle**: `Flying` / `Whenever another artifact you control enters, this creature gets
+2/+2 until end of turn.` The script's `auto=@movedTo(other artifact|myBattlefield):2/2
ueot` and its `text=` are faithful, the full text fits inside the 140-char option snippet
(103 chars — no truncation), and the board printed the true current `(0/1)`. The audit's own
b5 #10 quotes the model reading the card CORRECTLY two turns earlier from the identical
render, then inventing a static "+2/+2 for each artifact = 16/17" read.

No surface is false, so under the trust doctrine there is nothing here to fix in the script
or the engine — every candidate change would be decorating a true render. Rerouted to the
annotation/guide lane with the audit's own proposal: anchor the model to the printed P/T as
the LIVE value (a board-line or core-prompt statement), which is that lane's call, not this
one's.

## 6. `budget_hit` fired at 12,058 chars but not at 29,027 (b6) — **DETECTION WAS WRONG, FIXED**

**First, the corpus, measured** (`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260819-135510`,
699 records carrying a reasoning trace): **9** records carry `reasoning_budget_hit`, at
12,058 / 14,517 / 15,335 / 16,982 / 17,298 / 22,020 / 23,489 / **29,027** / 33,370 chars.
So the audit's framing is wrong on its own terms — the 29,027-char trace **did** fire. The
one genuine outlier is the 12,058. Two further facts the audit could not see:
`reasoning_tokens` is **absent from every record in the corpus** (this vLLM build does not
report `completion_tokens_details.reasoning_tokens`), so char counts are the only measure
available and the "read the next budget off the token distribution" plan needs the char
proxy; and the 12,058-char record (`1787186026` seq2, kind `ask`) is plain English with a
top-shingle count of 2 out of 1,503 — not a degenerate decode — but its `latency_ms` is
**168,679**, far past the ordinary round trip. At ~4 chars/token it is ~3k tokens, which
cannot bind an 8,000-token budget; something else ended that generation.

Which is exactly what the detection could not tell you. The marker was set on **every
forced close**. The forced close fires whenever a reply comes
back reasoning-only with empty content — which has TWO causes: (a) the decode stopped at the
cap (`finish_reason == "length"`) — a real budget hit; (b) the model ended its thinking
naturally and simply never wrote an answer line — no budget involvement at all. `(b)` is the
12,058-char record. The 29,027-char traces did not fire because their thinking ENDED on a
coded answer line, which `answerTailFromReasoning` recovers as the answer — so a long trace
that answers is correctly not a budget hit. The cap is in TOKENS, so char counts were never
going to line up either way.

- **Fix**: `reasoning_budget_hit` now requires `mLastFinishLength`; the rescue itself is
  counted separately as `reasoning_forced_close`. Both are still emitted, the rescue
  behaviour is unchanged (case (b) still gets its answer forced out).
- **On "the field was absent from all records"**: that is by design and always was —
  `reasoning_budget_hit`, like `retry` and `reasoning_hidden`, is PRESENT-ONLY-WHEN-TRUE.
  Absence means false, not unimplemented. An analysis needing a denominator counts records.
- **Prediction (falsifiable)**: on the wave-35 corpus, records carrying
  `reasoning_budget_hit` cluster at the TOP of the reasoning-length distribution (near the
  6,000-token budget, ~24k chars at this stack's ~4 chars/token), while
  `reasoning_forced_close` records WITHOUT it — the rescued non-answers — scatter across
  lengths, including short ones, and correlate with outlier `latency_ms` rather than with
  length. If the 12,058-class record recurs, it will now carry `reasoning_forced_close`
  alone.

## 7. Trace-degeneracy counter — **ADDED**

New translog field `reasoning_degenerate` (double, present whenever a reasoning trace was
captured): the share of the trace's 40-char shingles (stride 8, linear and cheap) that are
copies of the single most-repeated one. Normal prose lands near 0; the 415-repeat mojibake
trace and the 13.8k-char "No. Okay." loop — both of which returned WELL-FORMED replies and
were therefore invisible to every answer-side metric — land near 1. Traces under 400 chars
report 0 (the shape is meaningless there). No behaviour change and no threshold: it is a
number to cut on after the wave-35 corpus produces a distribution.

**Validated against real data before shipping** (the same 708 reasoning traces): median
**0.0013**, p90 **0.0023**, and the single top value **0.126** — ~100x the median, at
`1787172913` seq1 (23,489 chars). Its most common shingle repeats **370 times**:
`"\n[Analysis of the Study] (9),\n[Review of"`. That is a decode collapse nothing in the
corpus counted, in a record whose answer parsed cleanly. Two orders of magnitude of
separation on the first look, and the flagged record is a true positive.
