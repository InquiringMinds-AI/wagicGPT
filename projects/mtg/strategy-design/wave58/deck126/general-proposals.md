# deck126 — general-guide / render proposals (wave 58)

Three items, all deck-agnostic. Evidence lines are re-derived from rendered `prompt` text in
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-015059/`; full context in `wave58/deck126/review.md`.

---

## P1 — a HOLD take must not be re-opened by a life-total change alone
**Surface.** The HOLD row promises "do not ask me again unless the board changes (any change re-opens
this window; you give up no cast)". While a decided Sanguine Bond + Exquisite Blood loop resolves,
the life totals change on every iteration, so the predicate fires every iteration and the row cannot
absorb the run it exists for.

**Evidence.** `1788504693-ai_baka_deck126-0x557cda71a050-vs-ai_baka_deck130.jsonl` seq **56-87**: 32
consecutive asks inside turn 25's Upkeep, menu byte-identical throughout
(`1. Cast Tribute to Hunger {2}{b} {right now: they control 0 creatures - at 0 this does nothing}` /
`2. Cast nothing right now` / `3. Hold priority for the rest of this turn: ...`), header life running
17/17 → 33/0, declined-note reaching `[you declined this exact list 32 times already this turn]`.
Same shape at `1788504668-...deck126-...deck123.jsonl` seq 43-62 (20 asks, 20/20 → 40/0).
`identical_option_asks_resolved` reads **0** on every `gameend` in this seat.

**Proposal.** Scope the HOLD latch's re-open predicate to the things a decision can act on: the
`options_text` of the menu, the contents of `ON THE STACK`, and permanents entering/leaving. A change
confined to the life totals in the header, with `options_text` byte-identical, does **not** re-open a
window the pilot has already held. This removes no window the pilot did not itself choose to close,
adds no cap, and deletes nothing — the pilot is still asked the moment a row changes.

**Why this and not "stop asking during a loop".** The standing ruling rejects the engine deciding a
menu is not worth asking (wave-57 REJECTED list). This is the opposite: it makes the pilot's own
"hold" mean what its printed sentence promises. Cost of not doing it, this seat alone: 52 model
round-trips in two games narrating a win that cannot be interrupted.

**Falsifiable prediction if adopted.** In a wave-59 game where the pair completes and the pilot takes
the hold row once during the drain, `hold_windows_skipped` rises by the loop's iteration count and
the max declined-note in that game drops below 5.

---

## P2 — a search / reveal row must carry the price its cast row will carry
**Surface.** A tutor or reveal list renders every candidate as
`N. <name> (<type>) {text: ...} [eligible for "choose card"]` with no annotation. The cast decision it
determines is one window later, and *that* row is fully priced.

**Evidence.** `1788504664-ai_baka_deck126-0x55f8e7326c20-vs-ai_baka_deck146.jsonl`
seq **47** (turn 30, life 1): 34 unannotated rows including `10. Sanguine Bond`, `28. Exquisite
Blood`, with `Silverquill Silencer {b}{w} (4/3) [named: Sanguine Bond]` on the opponent battlefield
line. Pick = Sanguine Bond. seq **48**, the very next window:
`3. Cast Sanguine Bond {3}{b}{b} [NAMED BY THEIR Silverquill Silencer: casting this costs you 3 life
and draws them a card - you would be at -2; this KILLS you. This price is PER CAST ...]`.
The seat then correctly declined the row — and had nothing else. The search had spent the game's last
live window on a card the engine itself says is unsurvivable.

**Proposal.** On every search/reveal row whose card would enter the hand as a castable spell, echo
the same finished verdict the cast row would print, at minimum the `[NAMED BY THEIR ...]` family with
its `you would be at N` / `this KILLS you` tail. Every input is already on the board at search time.
Nothing is withheld and no row is removed: the pilot may still fetch it.

**Prediction.** Wave-59: 0 searches that fetch a `this KILLS you`-priced card while an unpriced
alternative of the same type is on the same list.

---

## P3 — say why a hand card has no cast row
**Surface.** The prompt prints `Your hand (N cards): ...` and the `Mana available:` block, but never
the shortfall the emitter already computed when it decided not to emit a cast row for that card. The
pilot re-derives it, and gets it wrong.

**Evidence.** `1788504695-ai_baka_deck126-0x55a199fd93d0-vs-ai_baka_deck125.jsonl` seq **34**:
4 untapped sources, hand line `... Exquisite Blood {4}{b} [enchantment]`, menu = one Tribute-at-0 row
plus decline plus hold. Reply `CHOICE: 1 (Cast Exquisite Blood)` → `<refused: named_row_reask>`,
window spent. The string `not castable` / `needs N sources` occurs **0 times** in 305 prompts of this
seat; the only appearance of the idea is a model-written PLAN echo doing the arithmetic itself
("need {4}{B}, have {B} 4 but generic 5 is enough, wait —") and reaching the wrong answer.

**Proposal.** Append one true clause to each hand-line card that has no cast row this window, from
the same computation that suppressed the row: `Exquisite Blood {4}{b} [enchantment] [no cast row:
needs 5 sources, you have 4]`, and for a colour shortfall `[no cast row: needs 2 sources that make
{b}, you have 1]`. It is a true fact, it adds a token rather than removing one, and it is the direct
antidote to the `named_row_reask` class the reply-protocol paragraph currently has to spend a
paragraph of every guide on.

**Prediction.** Wave-59: `named_row_reask` fallbacks fall to 0 on seats whose guides do not change.
