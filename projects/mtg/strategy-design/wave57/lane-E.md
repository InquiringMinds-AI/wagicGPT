# Wave-57 lane E — pregame, X, latches (D2, D9, D15, D19, D20, D23)

Branch `w57-lane-E`, worktree `worktrees/lanes/w57-E`, base master `4d4ea0bdf` (wave-57 brief
on top of `7ee3a51e8`). Six render items, all in `src/AIPlayerGPT.cpp` (+4 members in
`include/AIPlayerGPT.h`). No engine-behaviour change, no card data touched, nothing deleted.

## D2 (HIGH) — the pregame hand header counts MDFC back-face lands

**The tally ruling** (lane W docketed the semantics; the brief settled it and this is the
arithmetic it asked to be carried): the card is counted in **BOTH** tallies — as a land AND as
a spell — and one line reconciles the sum. Counting it as a land only would have dropped a
Pelakka Predation out of `Cheapest spell in this hand:` and out of the coverage clause, which
trades one false number for another; counting it in both is true of the card and breaks the
sum, so the sum is stated rather than hidden:

```
Your hand (7 cards), counted by the engine: 5 lands, 3 spells.
1 card is counted TWICE above, on purpose: Agadeem's Awakening is a spell you may instead play
as the land Agadeem, the Undercrypt. So 5 lands + 3 spells = 8 for a 7-card hand, not 7: 1 card
is in both counts. You may use each such card as ONE of the two, not both.
Mana sources among those lands, counted by the engine: {W} 4, {B} 2. ...
```

That block is a **live capture** (probe below), not a mock: four printed lands + Agadeem's
Awakening, and `{B} 2` = Shineshadow Snarl + the back face. Wave 56 rendered `0 lands, 7 spells`
on the identical class of hand and it decided both of that corpus's mulligans and its only
bottom.

* **Detection is data-only** and works before any ability is registered, which is what the
  pregame needs — but **not through `magicText`**: `MTGDeck` files an `autohand=` line under
  `magicTexts["hand"]`, and `magicText` is the `auto=` bucket only. Lane W's design note says
  `c->magicText` carries `forcetype(land)`; it does not, and a scan of it finds nothing. New
  `mdfcHandBackLandName()` reads the `"hand"` bucket, anchors on `forcetype(land)`, and takes
  the `flip(...)` name from the SAME script line. All 58 `forcetype(land)` uses across the
  primitives carry a `flip(...)`; only the 55 MDFC entries are hand-zone entries.
* **`sources[]` gains the back face's colours**, read through `landTapMana` on the collection's
  printed text — the same helper the option row uses to print `taps for {B}`, so the header
  and the row cannot disagree. (The helper is declared at file scope now: the header lives in
  the anonymous namespace and a declaration there names a different function.)
* **The dual-role card cannot fund its own cast.** Playing it as a land is what removes it from
  the hand, so in the coverage scan it is judged against `lands - 1` and its own colour
  contribution removed. Without that, the clause could claim a cast paid for by the land half
  it had already spent.
* **Still HAND-ONLY** (owner directive): the note is built from tallies and card data, no board
  word can reach the surface, and a PARSETEST case asserts the absence.

**Falsifiable prediction.** Hands containing a `forcetype(land)` card whose header land count
omits it: **0 of N** (wave 56: 4 of 4 at deck146 seats). And the second half, which catches a
detector that silently never fires: pregame prompts on such a hand carrying the `counted TWICE
above` note: **N of N** — this lane measured **3 of 3** live.

## D9 (MED) — the mana-fit clause at ANNOUNCE_X: a wiring bug, as the docket's falsifier branch predicted

The corpus is unanimous and it is not a builder fault. **Every one of the 9 X menus renders
`Mana available: 0 total (no untapped sources) | Already in pool: {u}{u}... floating right
now`**: the classic X route announces X out of a **pre-floated pool**
(`DecisionContract.cpp`: `maxX = pool CMC - base cost CMC`), so at that window the mana is
already tapped. Lane C's per-row `selectAutoTapProducers` call is correct code on the **wrong
resource** — `potentialColorReach` is 0 there by construction, `leavesUntappedTag` returns ""
on its first line, and no row could ever have carried the clause. 0 of 40 was structural.

The resource that window spends is the POOL, so the clause prices the pool:

```
1. X = 2 {X pricing: X=2 - you gain 2 life and draw 2 cards} {leaves 0 of your 5 floating mana
   unspent - this X spends your whole pool} [<- largest affordable X - ...]
```

(live capture). The base cost is derived from the MENU's own cap (`pool - capX`), the same
contract rail the annotations beside it ride, so the two cannot disagree; when they would (a
`pay[[{X}]] announcement round, where the pool is empty by design) nothing is claimed and the
shipped untapped-source path runs unchanged. Same `{leaves ` channel as the cast row, so the
3,498 existing `{leaves` keys still find it and `stripNarrationDecoration` already drops it.

**Prediction.** X rows carrying no mana-fit clause: **0 of N** (wave 56: 40 of 40). Live in this
lane: 17 rows, all clause-carrying.

## D15 (MED) — the board frame carries the opponent's LIFE TREND

One line, in every board frame, beside the scalar:

```
Opponent life trend: turn 4: 20, turn 5: 25, turn 6: 30, now 30 (+10 since turn 4).
Opponent life trend: unchanged at 30 since turn 4.
```

Samples are the opponent's life at THIS seat's **first look each turn**, each labelled with its
own turn number — a turn with no window of ours is absent, never interpolated. Sampling is
gated on the turn number, so it is idempotent within a window and the two variants
`serializeGameStateImpl` serves (prompt and ask-cache key) cannot diverge. Numbers only: "they
are gaining" is the pilot's inference, and a PARSETEST case pins that no verdict word appears.

**One live-probe correction:** the first build printed `since turn 0` under a `=== Turn 1`
header. The narration prints `observer->turn + 1` (#W51-D's rule), so the label now goes
through `translogTurn`. 1,384 renders in the second probe, all labelled against the narration.

**Prediction.** Board frames carrying `Opponent life: N` with no trend line: **0 of N** (wave
56: 136 of 137 on `130v125`), and deck130's CROSS-CHECK latch becomes decidable from one window
instead of a cross-window reconstruction.

## D19 (MED) — the fetch row's colour parenthesis gets a positive form

The negative form is **byte-identical** (guides key on it). What was silent now speaks:

```
... and it adds {W} (you can already make {W} - you have 1 source of {W} right now)
... and it adds {W} (you cannot make {W} right now)          <- unchanged
```

The count is the per-SOURCE breakdown from `potentialColorReach`'s own `outSources`, so it is
counted the way the mana line counts (one per source, a dual counted under each colour). Both
polarities captured live in the same game.

**Prediction.** Fetch windows where no row carries a colour tag of either polarity: **0 of N**
(wave 56: 1 of 5).

## D20 (MED) — the `[<- ...]` marker promoted to the CAST row

The docket names two places; the second (two rows, one verdict, two prices) is lane C's D30, so
this lane took the first: the cast row one screen earlier, where WHETHER is decided.

```
1. Cast Sphinx's Revelation {u}{u}{w}{x} {X pricing: max affordable X=2 (5 mana total); each
   point of X gains you 1 life and draws you 1 card} [<- best X for this cast: X=2 - largest
   affordable X - X=2 gains 2 life and draws 2 cards; no listed X does more] {card text: ...}
```

(live capture). The judgement is the SAME judgement — `xMenuMarkX` / `xMonotoneMarker`, ranked
off the same survey, with `maxX` from `ManaEngine::maxAnnounceableX`, which is the number that
builds the menu one screen later — so the two screens cannot name different X values. It is
stated as a fact about the ladder THIS cast opens, never as an instruction to take the row:
two X spells in one menu each state their own ladder without either claiming to be the menu's
answer. Bracketed, so it never enters history (echo case pins it). The no-kill verdict names no
X and is carried across verbatim.

**Prediction.** Cast rows for an X spell with an affordable ladder carrying no best-X marker:
**0 of N**; marked-row takes on the ANNOUNCE_X family stay at >= 6 of 9; and takes of an X
BELOW the marked one on a monotone menu do not rise.

## D23 (MED) — `{feeds:}` counts converters in HAND too

```
{feeds: the opponent draws 1 extra card per turn; converters on your battlefield: 0 (nothing of
yours punishes their draws or discards yet - ...); in your hand: 2 - Underworld Dreams, Fate Unraveler}
```

(live capture — the exact `162v125` seq-15 shape, where the seat read a FEEDER as a converter
and gave away four turns of cards). The hand half prints **including the zero**, so a rule keyed
to it is never silent; the two zones are never merged into one list. Battlefield names keep
their instance handles, hand names do not (a hand card has no board instance to point at).

**Prediction.** `{feeds:}` takes at `converters on your battlefield: 0` with a converter in hand
and no other reason: **0 of N** (wave 56: 1 of 3).

## Gate (this worktree, hermetic `make -B -f Makefile.sdl -j4`)

| | baseline (brief, 4366e8eec) | w57-lane-E |
|---|---|---|
| build | clean | clean (`make -B`, rc 0) |
| suite (`WAGIC_TESTSUITE_THREADS=1`) | 1241 / 0 failed | **1241 / 0 failed** |
| AI fixtures | 55 / 0 | **55 / 0** |
| suite timeouts | 0 | **0** |
| PARSETEST | 2736 / 0 | **2778 / 0** (+42) |

`git diff | /usr/bin/grep -c $'\357\277\275'` = **0**. Every wagic run was under the 4G memory
cap; no run was killed. Logs: `~/.gatelogs/w57E-{build,parsetest,suite}.log`.

Two shipped PARSETEST cases changed their expected literal because this lane changed the
literal, and both changes are the item: `#W51-F D11`'s `{feeds:}` string gains `; in your hand:
0`, and `#W54-E D20`'s "earns no cannot-make tail" case now asserts the POSITIVE form (its
negative claim — no cannot-make tail — still holds and is still asserted). No other shipped
expectation moved.

## Live probes

`strategy-design/wave57/d2-pregame-probe.sh` (stub endpoint, real prompt assembly / HTTP /
parse / translog, deterministic `CHOICE: 1`, no inference). Two runs, 10 one-shot games:
deck146 vs deck125 (MDFC pregame + X menus) and deck123 vs deck162 (fetchlands + converters).
Renders observed: D2 note 3 of 3 MDFC-holding pregame prompts; D9 17 rows; D15 1,384 frames;
D19 3 positive / 4 negative; D20 14 cast rows; D23 20 rows. Probe home
`~/.gatelogs/w57E-d2probe/` (translogs kept there, not in the repo).

## What I did NOT verify

* **No live model ran, and no corpus.** Every game was answered by the stub, so no reply in
  this lane was a decision: what is proven is that the strings RENDER and what they say, not
  that the pilot acts on them. Every prediction above is a wave-58 corpus question.
* **D2's back-face colours are read from the collection's printed `text=`** via `landTapMana`,
  the same route the option row takes. A back face whose mana ability is scripted but not
  printed would contribute no colour — the count would then be conservative (a missing source),
  never invented. Not observed; not searched for across all 55 entries.
* **A Pathway-class card (LAND front, LAND back) is untouched**: it already counted as one
  land, and its BACK face's colour is deliberately NOT added to `sources[]`. A Pathway makes
  one of its two colours, chosen at ETB, so adding both would over-claim the coverage clause in
  exactly the way this item exists to stop. That is a residual, not an oversight — it needs its
  own "one of these, not both" grammar. **Docket it.**
* **D9's pool arithmetic is not colour-aware.** `leaves N of your M floating mana unspent`
  counts mana, and a pool whose remainder is the wrong COLOUR for the next spell is still
  reported as a remainder. The cast row's own remainder has the same property. Sufficient for
  the trade the menu never priced; not a castability claim.
* **D9's untapped-source path is now unreachable in the live route by construction** and was
  not re-exercised: no `pay[[{X}]]` announcement round occurred in any probe game, so the
  fallback arm is reasoned + PARSETEST-pinned only.
* **D15 samples only turns in which this seat renders a frame.** A turn the seat never sees
  leaves no sample; the labels carry real turn numbers, so nothing is interpolated, but the
  three samples are not guaranteed to be three CONSECUTIVE turns. The line does not claim they
  are.
* **D20's marker was not seen on a KILL-class cast row live** (no Starstorm/Rolling Thunder
  window occurred); the monotone family was. The kill families are PARSETEST-pinned through the
  same `xMenuMarkX` the menu already uses.
* **No Vita/PSP build, no GUI drive, no `cardauto` sidecar regeneration** (no card data
  changed). No human seat exercised any of these surfaces.
