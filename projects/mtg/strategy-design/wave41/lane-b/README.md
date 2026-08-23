# Wave-41 lane B — the NARRATION CHANNEL (#W41-3 a-e, owner-ruled)

Docket item **#W41-3** from `wave40/engine-ledger.md`. One lane, five sub-fixes, one emitter family.

Modified files: `src/AIPlayerGPT.cpp`, `include/AIPlayerGPT.h`, `src/MTGAbility.cpp`,
`include/WEvent.h`. Changes are UNCOMMITTED in the lane worktree.

## Mechanisms

### (a) Opponent-side activation / loyalty narration — 165 / 0 → symmetric

The acting seat wrote its activations from its *consumed decision*
(`actionTakenNarration` → `"You used: <label> with <Card>"`). The engine raised **no event at
all** for an activation, so the observing seat had no channel: it saw the loyalty counter, the
drain, the shuffle, with no cause line above it.

- New event `WEventAbilityActivated{source, controller, abilityText}` (`include/WEvent.h`),
  raised at the single choke point `ActivatedAbility::activateAbility()` on its **non-mana**
  path — mana producers `return` above it, so a tap-for-mana (plumbing, narrated nowhere)
  raises nothing. Loyalty abilities are `ActivatedAbility` subclasses and ride the same path.
  The event is informational: no engine behaviour reads it.
- `AIPlayerGPT::describeEvent` renders it **only when `controller != this`** — the acting seat
  already has its own line, and emitting on both paths would double it.
- `abilityActivationNarration(mine, abilityText, cardName)` is pure, and PARSETEST pins it
  **byte-identical to `actionTakenNarration`'s own line** for the same activation. That
  identity IS the symmetry metric.

### (b) Public-origin naming (OWNER RULING) — the mask was OBSERVER-scoped

Old code: `if (!mine && (to == hand || to == library)) → "Opponent put a card into their <zone>"`.
That masked a *graveyard recursion* (public information the opponent watched happen) exactly as
hard as a draw. New code masks by **ORIGIN**:

- `isPublicOriginZone(z)` = battlefield / graveyard / stack / exile / reveal.
- Public origin → falls through to `zoneChangeNarration`, which **names the card**.
- Hidden origin → `hiddenOriginMoveNarration`, which keeps the card unnamed but **states the
  origin** ("Opponent put a card from their hand into their library"). The library→hand draw
  keeps the owner's count-only ruling ("Opponent drew a card").
- `zoneChangeNarration` now states the origin on the lines that previously omitted it:
  battlefield→graveyard (non-creature), battlefield→hand, battlefield→library, and two new
  explicit graveyard→hand / graveyard→library forms. **Deliberately unchanged**: the verbs that
  already encode their origin and are the owner's verbatim W35 register — "You drew X",
  "You milled X", "You played X", "You discarded X", "X died".

### (c) Bulk-shuffle collapse

Consecutive graveyard→library zone changes accumulate in the seat
(`mBulkMoveCount/Mine/FirstLine/Source`) and flush as ONE line. **Any** other event flushes the
accumulator first, so log ordering is untouched, and `appendNarration` itself flushes (clearing
the count *before* appending, so the re-entry is one no-op, never a loop). A run of length 1
flushes as the ordinary named line, so nothing is ever counted at "1 card".

Source attribution: `resolvingStackSource()` reads the latest `NOT_RESOLVED` stack object
(`ActionStack::resolve` marks state only *after* `resolve()` returns, so the causing object is
still there while its zone events fire). ⚠ `StackAbility` never fills `Interruptible::source` —
its ctor sets only `type` — so the owning card must be read off `sa->ability->source`. The first
probe run caught this live: every collapsed line came out unattributed until that was fixed.

### (d) #23 effect-as-source (OWNER: *"this should be the effect source, not the effect"*)

`chooseTarget` derived `effectName = tc->source->getDisplayName()`, and when a granted/inner
ability rides a **nameless fake card** the old fallback promoted the ABILITY's display name into
the SOURCE slot — `You targeted Swamp with Put in Play`. The fallback now takes the name from the
card that OWNS the waiting ability (`waiting->source`), then that card's raw `name`, then
`tc->source->name`, and finally the honest `"this effect"` — **never** the ability's own name.
The effect stays in the ability slot, where the target-choice header and `targetChoiceNarration`
already render it.

`effectName` feeds the ask header, the echo-strip hint and the narration from one derivation, so
all three are fixed at once. **Class sweep**: `targetChoiceNarration` has exactly one caller; the
only other emitter that could carry an ability name as a source is the stack render, which
already routes through `stackAbilityName()` and says `"ability: <menu text>"` rather than
pretending it is a card. No other instance found.

### (e) F3 same-zone no-ops

`describeEvent` suppresses a zone change whose origin and destination are the same zone — by
identity **and** by (same owner, same `zoneDesc`). The name test is the load-bearing one: dungeon
venturing moves a dungeon between two *different* generic `MTGGameZone` objects that both report
`getName() == "zone"`, which is why the corpus read
`moved from the opponent's zone to the opponent's zone`. The correct
`- <X> ventured into <Dungeon>: venture step N` line is untouched.

## Gates

| gate | result |
|---|---|
| build (`make -f Makefile.sdl -j8`) | clean link |
| PARSETEST | **692 / 0** (baseline 671 + **21** new cases; 2 existing cases re-pinned in place for the origin-stating shapes) |
| ST suite, `WAGIC_TESTSUITE_THREADS=1` | **1057 (0 failed) + 30 AI (0 failed)** |
| `git diff` U+FFFD | 0 |

## Live arrival evidence (`translogs/`)

Probe decks **198/199** (numbering gap; the roster ends at deck164 and was verified back to 164
after deletion). Stub-server pattern: a fake OpenAI endpoint (`stub_openai.py`,
`stub_choose.py`) with **real prompt assembly, real translog, no inference** —
`stub_openai.py` always replies plan-only so the heuristic answers every window (proves the
observer channel works *independently of the model deciding*); `stub_choose.py` answers the
seat's own reply protocol so the ACTING seat also writes its consumed-decision lines.

Run shape, memory-capped, both seats GPT:

```
timeout 1500 systemd-run --user --scope -q -p MemoryMax=4G -p MemorySwapMax=0 -- \
  env -u WAYLAND_DISPLAY -u DISPLAY WAGIC_HEADLESS=1 WAGIC_FASTCLOCK=0.1 \
  WAGIC_SELFPLAY=1 WAGIC_SELFPLAY_ONESHOT=1 WAGIC_AI=gpt \
  WAGIC_SELFPLAY_DECK0=198 WAGIC_SELFPLAY_DECK1=199 \
  WAGIC_GPT_URL=http://127.0.0.1:8317 WAGIC_GPT_MODEL=stub-narration-probe \
  WAGIC_GPT_TRANSLOG=1 WAGIC_GPT_TIMEOUT=20 ./wagic
```

⚠ Sibling lanes write into the SAME `~/.Wagic/ai/gpt/logs/`. Identify your own runs by
CONTENT, not by mtime — an `ls -t | head` grab pulled another lane's deck198 log once.

| file | run | what it shows |
|---|---|---|
| `final198/199.jsonl` | heuristic stub, 59 turns | `Opponent used: Life with Elixir of Immortality` ×3/×5, `Opponent used: Put in Play with Windswept Heath` ×4, and **`Opponent shuffled their graveyard (3 cards) into their library with Elixir of Immortality`** / `(6 cards)` — the owner's shape verbatim. Zero anonymous `put a card into their …`. |
| `dng198/199.jsonl` | deciding stub, 23 turns | venturing on both seats: `Opponent used: choose a new dungeon with Cloister Gargoyle` → `Opponent ventured into Dungeon of the Mad Mage: venture step 1 of that run`, with **zero** `zone to the opponent's zone` lines. |
| `tgt198/199.jsonl` | deciding stub | **(d) live**: `You targeted Lost Mine of Phandelver … with Cloister Gargoyle's choose a new dungeon ability` — the source slot names the CARD, the effect sits in the ability slot. Also `You used: Life with Elixir of Immortality` on the actor beside `Opponent used: Life with Elixir of Immortality` on its pair — **the same activation, both chairs, one wave after 165/0**. |

### Metrics, measured over the probe seats

| metric | baseline (wave-40 corpus) | this lane |
|---|---|---|
| `You used:` / `Opponent used:` | 165 / **0** | present on **both** seats for the same activation |
| cause-less loyalty/activation effects on the observing seat | 107 | 0 in probe (every effect run has its cause line above it) |
| anonymous public-origin moves (`put a card into their …`) | 47 | **0** |
| bulk shuffle | 47 anonymous lines | **1 counted, attributed line per shuffle** |
| effect-as-source (`with Put in Play`) | present | **0** |
| same-zone no-ops | 25 | **0** |

## Scope notes / honest limits

- The observer line is emitted for anything reaching `ActivatedAbility::activateAbility()`,
  which includes an inner *choice* ability wrapped by a triggered ability — observed as
  `Opponent used: choose a new dungeon with Cloister Gargoyle`. Truthful in scope (the
  opponent's card did that, and it is the cause of the venture line that follows) but the verb
  "used" reads as an activation. Flag for the wave-41 review if the owner wants a separate
  register for trigger-internal choices.
- The 107 / 47 / 25 baselines are wave-40 corpus counts. The probe discharges each class
  categorically (present ↔ absent), not at corpus scale; the next corpus's seat review is the
  real gate.
