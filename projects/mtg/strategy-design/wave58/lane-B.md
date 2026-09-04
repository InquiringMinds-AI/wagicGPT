# Wave-58 lane B — D1, D5, D7

Base: master `6976b839a`. One file touched: `projects/mtg/src/AIPlayerGPT.cpp`. Every change is
render/verdict computation on the prompt surface — no engine behaviour, no legal set, no row
removed, no window closed.

## D1 — the life-payment row carries its own subtraction

**Mechanism.** `payLifeAmountFromOption()` reads the amount off the row's own label (the same
`pay N life` shape `isPayLifeOption` already matches, now parsed: digits must belong to the LIFE
clause, so `pay 2 mana and 3 life` and `pay half your life` yield nothing rather than a guess).
`lifePaymentVerdict(life, cost)` renders the finished subtraction in the brace channel:
`{this payment puts you at N}`, or, when `N <= 0`,
`{you are at L life: paying C puts you at N and you LOSE the game}`.

A new pass in the menu decoration (after the ETB pay-or-tap block, before the header recovery)
appends the verdict to **every** option on the menu that is a computable life payment — not only
the shockland/MDFC pay-or-tap shape. It appends LAST, so the option short name and its echo
anchor are untouched, and both prefixes were added to `stripNarrationDecoration`'s brace
whitelist so the verdict is decision-time only and never enters the append-only record.

The lethal row is **badged, never removed** — the legal choice stands.

`146v130` seq 20's row now renders:
`pay 3 life - Emeria, Shattered Skyclave enters UNTAPPED [usable (tap for mana / attack) this turn] {you are at 1 life: paying 3 puts you at -2 and you LOSE the game}`

## D5 — the trade marker's selection rule uses both sides

**Mechanism.** `xBestTradeX` counted only THEIRS, which is how a 1-for-5 was marked "the most of
THEIRS at the smallest cost to YOURS". It now takes both counts (`xTradeCountsAt`) and applies the
rule the words already claim: an X whose cost to YOURS **exceeds** what it takes of THEIRS is never
endorsed. Both counts are monotone non-decreasing in X, so the first X reaching the maximal THEIRS
also carries the smallest YOURS of that set — the cheapest-X tie-break is unchanged.

When no X trades at or better than even but a kill of theirs still exists, the row is still named
(denying a real kill would be a false surface) with a marker that **prices** rather than endorses:
`[<- the only X that kills anything of THEIRS costs you more of YOURS than it takes - THEIRS 1 / YOURS 5]`.
The endorsed form now carries its evidence inline too:
`[<- best trade: the most of THEIRS at the smallest cost to YOURS - THEIRS 2 / YOURS 1]`.
A FREE kill still outranks every trade form; the no-kill verdict is byte-identical to wave 55.

The sibling `[<- best X for this cast: ...]` (#W57-E) is built from the menu marker's own text, so
it inherits both the selection fix and the counts by construction — proven in PARSETEST, not
assumed.

## D7 — the assignable bound is a true floor

**Mechanism.** The caller used to hide a trampler/menace attacker by passing its damage as `0`,
which removed it from the matching *and from the baseline it subtracts from* — so the printed
number was the remainder of a board that omitted the damage nobody could stop
(`126v152` seq 14: `one legal assignment gets you to 1` at 5 life, true floor -5).

`assignableRemainderDamage` now takes an optional `preventable[]` column. `damage[j]` is always
the attacker's face damage; an unpreventable attacker counts in the total and is never matched,
and the blockers it would have absorbed stay free for the rest — which can only *increase* the
prevented total. The result is therefore an over-estimate of prevention, i.e. a true floor on the
damage that lands: never optimistic in the lethal direction.

Two further baseline leaks closed: an already-blocked **trampler**'s excess over its blockers'
toughness is added back (`trampleOverflow`, full toughness used — the conservative side), and
`exactAssignment` drops for it.

The rendered claim split into two kinds of number:
- exact (proven maximum): `- best case with every blocker assigned: you would be at N`, and when
  `N <= 0`, `; no block saves you` — the lethal badge the maximum can actually prove.
- non-exact (a floor): `- at least D of that lands whatever you block (trample/menace counted as
  unblocked): you would be at N or better`. It claims no death it cannot prove, and no longer
  asserts an achievable assignment it does not have.

## Gate (hermetic `make -B -j6`, then both legs under `MemoryMax=4G MemorySwapMax=0`)

| | base (brief) | lane B |
|---|---|---|
| PARSETEST | 3350 passed / 0 failed | **3394 passed / 0 failed** (+44) |
| suite `==Test Failed !==` | 0 @ THREADS=1 | **0** |
| suite `==Test timed out` | 0 | **0** |
| suite total | 1245 | **1245 (0 failed)** |
| AI tests | 61 / 0 | **61 (0 failed)** |

Two pre-existing expectations were deliberately updated (not deleted) because the fixes changed
what they assert: `#W57-B D24`'s lethal maximum now carries `; no block saves you`, and its
non-exact sibling reads the floor form instead of `one legal assignment gets you to -5`.
`#W55-C D6 (a)`'s trade marker check became a prefix match plus its counts.

## Predictions (falsifiable, for the wave-58 corpus)

- **D1**: `pay N life` rows whose payment is computable render the verdict on **N of N**; takes at
  `my_life <= cost` (a lethal payment) **0 of N**. A hit is a lane break only if the take was
  lethal *and* the row carried the verdict.
- **D5**: `[<- best trade:` renders where `YOURS > THEIRS`: **0 of N** (wave 57: 1 of 1). The
  lopsided form, where it renders, states `THEIRS a / YOURS b` with `b > a` and is **not** taken.
- **D7**: printed `best case` / `at least ... or better` values that are optimistic against the
  true floor: **0 of N** (wave 57: 1 of 34). Every `; no block saves you` badge sits on a window
  the seat did in fact die to unless it acted outside blocking.

## What I did NOT verify

- **No live probe.** No corpus game, no Spark call, no translog was run for this lane. All three
  items are verified only by PARSETEST over pure functions plus the composed row strings.
- **No suite fixture, and none is reachable.** All three changes live inside `AIPlayerGPT`'s
  prompt rendering, which runs only for a seat with a live endpoint; the test harness has no GPT
  seat and never renders an option line, so the D1 menu row is **not** reachable in a suite game.
  The suite is a no-regression gate here, not a proof.
- **D1's amount parse is label-driven**, not script-driven: a card whose menu label does not spell
  the number (`pay half your life`, an X life cost) gets no verdict rather than a wrong one. I did
  not enumerate how many primitives that leaves unpriced.
- **D7's blocked-trampler excess** uses full printed toughness, not remaining toughness, and does
  not model deathtouch-assigned trample (1 damage per deathtouch blocker). Both under-state the
  excess, so the number stays a floor, but the floor is looser than the rules allow.
- **D7's floor does not model** damage-is-still-dealt-when-blocked effects, protection, or damage
  prevention shields; it prices only trample and menace as unpreventable.
- The `-1` / 32-a-side-cap paths are exercised in PARSETEST only, never on a live board.
