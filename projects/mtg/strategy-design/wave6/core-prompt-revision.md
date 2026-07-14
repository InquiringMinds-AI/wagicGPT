# Core-prompt revision (wave 6) — exact edits for `bin/Res/ai/gpt/system_prompt.txt`

## Verdict: NO core-prompt edits this wave.

`system_prompt.txt` is byte-identical to `general-strategy.txt` and STAYS byte-identical —
`wave6/general-strategy.txt` is an unchanged copy of the wave-5 core. Verified:
`diff wave6/general-strategy.txt bin/Res/ai/gpt/system_prompt.txt` is empty; no owner action needed.

The convergence gate (2+ seats OR completion of an existing core line) admitted no new core text this
wave. The one 2-seat convergence routes to REPRESENTATION, not the prompt. Details below, plus the one
core line whose removal condition is now met (kept for wave-7, on the A/B shortlist).

---

## 1. Reflexive high-life blocking — 2 seats, but routes to REPRESENTATION, not a core sentence

- **Evidence (2 seats):** deck109 wave-5 (terminal "do not block... race" fired 0×; pilot blocked 4× at
  high life) + deck44 wave-6 (`e9289 s4/s7` chumps racing flyers into a 2/2 at **L20**; `e1689 s47`
  blocks Phyrexian Obliterator at **L20**, feeding its sacrifice punisher). deck135 CLEAN (its 2 blocks
  were a correct low-life survival block and a favorable high-life trade-up), deck131 CLEAN, deck110 N/A
  (0 blocker windows).
- **Why NOT a new core sentence.** The core ALREADY carries the floor (line 29: "When you would stay
  above 8, blocking is optional and 'no blockers' is often the right answer: block only when you can name
  the gain"). The pilot READS enough to state a (false) gain — "prevent lethal" at 20 life — and blocks
  anyway. This is a rule PRESENT and LOSING at distance, which the skill's own layer-routing sends to
  REPRESENTATION, never to a second paragraph of the same instruction. deck109 (which FIXED it per-deck
  with an 8-life blocker floor, 4 high-life blocks → 0) and deck44 both explicitly reach this conclusion.
- **Routed to ENGINE/REPRESENTATION (see ledger, item 4):** at the blockers decision, surface the
  decision-relevant number the pilot is ignoring — annotate the blocker prompt with
  `Your life: 20 (unblocked damage this turn: 2 -> you would be at 18)` or a `[not lethal]` marker when
  the total incoming is well below your life. Same shape as the ON THE STACK win — a surfaced number
  dissolves a world-model error. Per-deck floors (deck109/deck44) stand as interim.
- **Do NOT promote "racing creatures don't block" to core** — it would misplay every control deck.

## 2. `hand -> stack` cast-happened anchor — removal condition MET cross-deck; KEEP for wave-7 (A/B shortlist)

The core carries the cast-happened stopgap (line 35: "A cast that really happened shows in the GAME LOG
as the card moving 'hand -> stack'; if your last choice was a cast and the card is STILL in your hand
with your mana unspent, the cast did not happen — spend this decision on a DIFFERENT castable card and
try the stuck one again next turn; repeating the identical pick in the same turn changes nothing"). Its
stated removal condition was "dies when the Hellrider-class engine no-op is fixed."

- **The defect is FIXED at source and confirmed gone cross-deck.** deck109 (the owning/canonical seat)
  traced the "no-op" to the deck's own combo-hold `#HINT` vetoing the model's Hellrider pick; commit
  **353f1a150** removed that veto and 2/2 Hellrider casts resolved. The wave-6 no-op sweep found **0
  cast no-ops in ALL 7 seats** (deck109 0, deck131 none, deck133 none, deck140 0, deck110's "no-op
  re-equips" are the SEPARATE marker bug, deck135/deck44 none). So the removal condition is met with
  cross-deck agreement, not a single seat.
- **KEEP it for wave-7 anyway; put on the removal SHORTLIST.** Two reasons: (a) removal of a
  deck-agnostic core line is a DELIBERATE A/B, not a wave edit (the skill's own epistemic rule; deck109
  the owning seat explicitly recommends "do NOT retire on this seat alone — put on the shortlist");
  (b) the anchor's second clause ("repeating the identical pick in the same turn changes nothing") is
  generic anti-repick-loop insurance that outlives the Hellrider bug. The wave-6 harm deck109 found (the
  pilot citing "I must have missed casting Hellrider" to justify a Cast-nothing pass) was in the GUIDE
  copy of the fallback, which deck109 DELETED — the core wording is more careful and did not misfire.
- **Wave-7 action:** if wave-7 is a SECOND consecutive clean no-op corpus, retire the anchor via a
  deliberate A/B (measure repick-loop / Cast-nothing rate before/after). Cite 353f1a150.

## 3. Attack-seam survival strengthening — SINGLE SEAT, per-deck + watch

deck110 vs109: the pilot attacked with a creature it needed to block at 8 life and died to the crackback.
The core line EXISTS (line 29: "do not tap out into a lethal swing back") but LOST to a local pattern.
SINGLE SEAT → per-deck reinforcement (deck110 added it to THE RACE) + convergence watch. If a second
seat shows the same "tapped out my blocker attacking" pattern, consider strengthening the core phrasing
from "do not tap out into a lethal swing back" to also cover "do not ATTACK with a creature you need to
block." The general method (a survival rule at the block seam needs its attack-seam partner) is adopted
into the SKILL this wave; the CORE strengthening waits for convergence.

## 4. Threat-before-value / highest-impact act-floor (wave-5 Edit 2) — CONFIRMED, keep verbatim

Three seats confirm it working: deck135 (8/10 creature-vs-value conflicts to the creature; Icehide Golem
8/8; drove the durdle reduction that pushed 3 of 6 opponents below the wave-5 18-23 opp-life floor),
deck133 (deployed removal/creatures over the cheapest discard), deck131 (every Prism Ring / Elixir cast
was a no-threat-available window — the line either fired correctly or had nothing to move). No change.

## 5. Other single-seat items routed OFF core

- **GFTT targeted-spell-with-no-legal-target offered** (deck44) — ENGINE menu-contract violation, not a
  prompt edit. Routed to the engine ledger (see synthesis-notes.md), high priority; the prompt already
  promises "the game has already checked your... targets," and the fix is a target-existence check at
  option-build time, not more prose.
- **Gray Merchant drain display under-counts by 2** (deck133, re-confirmed vs44 shown 10 → actual 12) —
  ENGINE/representation, per-deck "shown N + 2" stopgap until the display fix ships.
- **Free-reaction card cost** (deck135), **burn-prior on scarce X-finisher** (deck140),
  **mulligan guide-precedence** (deck131) — all single-seat, per-deck + convergence watch. See the
  skill Appendix.
