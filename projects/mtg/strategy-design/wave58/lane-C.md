# Wave-58 lane C — D2, D4, D9

Base: master 6976b839a. Branch `w58-lane-C`. Files touched: `projects/mtg/src/AIPlayerGPT.cpp`,
`projects/mtg/include/AIPlayerGPT.h`. No Res asset, no fixture, no engine-behaviour change —
all three items are render/record surfaces, so PARSETEST is the whole gate by the brief's own
split.

## D2 — the false `NO LIVE CAST ROW ON THIS MENU` header

**What it was.** `everyCastRowDead` folded `rowSaysNoOp` over each row's whole `{right now:}`
string. A verdict is not one statement: it is a LIST of per-scope verdicts joined by `;`. The
`123v130` s45 row renders

    {right now: they control 1 creature - Rorix Bladewing (6/5) [flying, haste, doesn't untap
    during its controller's untap step] is sacrificed, they gain 5 - the sacrificing player
    gains, not you; YOU control 0 creatures - targeting yourself does nothing}

and the whole-string predicate matched the SELF scope's "does nothing" while the OPPONENT scope
named a 6/5 flier dying. Same shape one level down: Tragic Slip's `-1/-1 (no creature has died
this turn, so Morbid does NOT apply)` matched on a parenthetical QUALIFIER while the operative
magnitude, `-1/-1`, stood outside it.

**What shipped.** `AIPlayerGPT::verdictReadsZero` (public, pure): split the verdict body into
scopes on `;` at bracket depth 0, strip each scope's parenthetical qualifiers, and require EVERY
scope's operative text to read zero under `rowSaysNoOp` — which stays the engine's single
zero-predicate; this only decides what it is applied to. A scope that strips to nothing is not a
zero, so the miss direction is the header staying SILENT. `everyCastRowDead` calls it instead of
`rowSaysNoOp`. Nothing is removed and no row changes: the header is a token that now folds over
the same units the rows print, so it cannot contradict one.

**RED on base, measured on the corpus rather than asserted.** Replaying wave-57's 497 header
windows (`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-174505`) through both predicates:
472 windows keep the header, **25 lose it**, and the 25 are exactly the ledger's 8 narrow sites
(`123v130` s7/s45/s49/s50/s53/s54, `123v146` s16, `126v125` s438) plus the 17-window Tragic Slip
class the ledger's wider predicate named. Zero true headers are lost.

## D4 — the drop line's outcome, and a record that survives a release build

**What it was.** 108 stale drops, 0.82 h of inference, and the only evidence was one stderr line
naming the ARM. A drop's outcome could be recovered only by reading the next `AIPlayerGPT:` line
of a 40 KB stderr, and `DebugTrace` is `(void (0))` in a release build, so a console left no
trace at all. (`asyncSlotDriftKind` "rendering 0 times" is the literal, not the field: the drift
kind was already on the line as prose.)

**What shipped.** Three pure helpers beside `asyncSlotDriftKind`: `asyncDropOutcome(bool)` —
`re-asked` / `gave-up-to-heuristic`, the only two exits a drop has; `asyncDropTraceLine(...)`,
which keeps wave-57's leading literal and arm parenthetical BYTE-IDENTICAL (every existing census
keys on them) and appends `[outcome: ...]`; and `asyncDropStamp(...)` = `arm/drift/outcome`. The
trace line is compile-gated diagnostics. The stamp is not: each drop pushes one token onto
`mAsyncDropStamps`, and the next translog record this seat writes carries `async_drops` (count)
and `async_drop_events` (the tokens), consumed on write so a drop is stamped exactly once.

**Not done, deliberately: D4's ask (a)** — narrowing the slot key's board half so a Baka land
auto-tap cannot invalidate an answer. Two reasons, one of them a measurement: the wave-57 drop
census is `question (or turn/phase)` 96 of 108 and `question and board` 12, so the board half is
the sole cause of NONE of them and narrowing it can reach at most those 12; and desensitising a
slot key is the doctrine's blind-cache direction — it must be paid for with evidence that the
answer is still correct on the moved board, which this lane has no corpus to produce. Recorded
for the wave-58 ledger.

## D9 — `INCOMING THIS COMBAT` gated on combat phases

**What shipped.** `incomingCombatForm`'s gate moves from the opponent's COMBAT phases to the
opponent's TURN, upkeep through end of turn. Their upkeep, draw and main 1 now carry the FORECAST
form (`not declared yet - N of their creatures can attack, for up to M - you would be at K`);
their post-combat windows re-render the LATCHED total, or the closed "they declared no attackers"
when they had bodies that could have attacked; untap, cleanup and the post-end steps stay silent,
as does the seat's own turn. Two truth guards: the DECLARED form still requires the combat phases
(an attacker flag outside them is not a declaration this window may price), and a post-combat
window with nothing able to attack prints nothing — the board frame's `0 of them able to attack
right now` already says it. The counts the form consumes (`ableAttackers` / `ableDamage`) were
already computed unconditionally at the call site; no new evaluation was added.

D6's under-claim rule is untouched: the forecast is an upper bound over able attackers and makes
no trample carry-over claim, and no blocker is assigned in that window to carry past.

**RED on base:** the wave-57 PARSETEST case at the same site asserted
`incomingCombatForm(true, MTG_PHASE_UPKEEP, 0, -1, 7, 3) == 0` — the defect, pinned. That CHECK is
replaced (marked SUPERSEDED in the comment) by nine that assert the new gate, including
`125v146` s23's own numbers: 3 life, 7 power, `you would be at -4; that would KILL you`.

## Gate

Hermetic incremental build (clean link), then a detached unit under `MemoryMax=4G`,
`WAGIC_TESTSUITE_THREADS=1` (ground truth):

- suite **1245 tests, 0 failed, 0 timed out**; **61 AI tests, 0 failed** (base: 1245 with 2
  concurrency-only failures at THREADS=16, 0 at THREADS=1; 61 AI/0)
- PARSETEST **3375 passed, 0 failed** (base 3350; **+25** = D2 10 + D4 8 + D9 net 7 — 8 new
  CHECKs replacing the 1 superseded one; the sum is exact)
- `git diff | grep -c $'\357\277\275'` = 0

## Predictions for the wave-58 corpus (falsifiable, one per item)

- **D2** — header windows carrying a cast row whose `{right now:}` names a live effect in ANY
  scope: **0 of N** (wave 57: 8 of 497 narrow, 25 of 497 including the Tragic Slip class). The
  header's own volume should fall by roughly 5% of its renders and no more; a header that
  disappears from an all-dead menu is a lane break.
- **D4** — every `dropping stale async answer` line carries `[outcome: ...]`, and every translog
  record written after a drop carries `async_drops` ≥ 1: **100% of drops**, both channels. The
  drop COUNT is not predicted to move (this lane changed no key and no gate) — a drop count
  materially below 108 would need a different cause.
- **D9** — opponent-turn windows from upkeep on with ≥ 1 creature able to attack and no
  `INCOMING THIS COMBAT` line: **0 of N** (wave 57: 20). And the negative: no `not declared yet`
  line on any window at or past the opponent's second main.

## What I did NOT verify

- **No live run.** Nothing here was observed in a game: no corpus, no probe, no model call. The
  D2 claim is measured by replaying wave-57 prompts through both predicates in python, not by a
  fresh render; D9's new windows are proven only as a pure function over phase ordinals — the
  first corpus is the first time the forecast line is seen in an actual upkeep prompt.
- **D9's token cost is unmeasured.** The forecast now prints on every pre-combat opponent-turn
  window with a live attacker; on a wide board that is one extra line on a lot of windows, and
  whether it earns its tokens is a seat-review question this lane cannot answer.
- **D34 (the two-combat latch keyed on `observer->turn`) is untouched** and is still unguarded;
  my post-combat branch inherits it — a second combat in one turn would read the first combat's
  latch as this combat's settled total.
- **D2's discard-menu half is lane A's** (the `rowSaysNoOp` whitelist and the 6-of-455 discard
  gap). ⚠ MERGE NOTE: lane A owns `rowSaysNoOp` and one of its listed call sites is
  `everyCastRowDead` (~18320), which this lane also edits — expect a small conflict there. The
  two changes compose: this lane decides WHAT the zero-predicate is applied to, lane A decides
  what the predicate recognises.
- **`async_drop_events` was not observed in a written translog record** — no run produced a drop.
  The field's shape is pinned in PARSETEST; its presence on a real record is not.
