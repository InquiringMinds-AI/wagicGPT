# Wave-57 lane G — D42 Arena-style board grouping (human seat, frontend only)

Branch `w57-lane-G`. Frontend/render + input choreography only: **no engine, no AI-seat and
no prompt surface was touched**, and PARSETEST's count is unchanged at 2736, which is itself
the check that no model-facing string moved.

## The ruling this implements

Owner, 2026-09-03: *"i like this, but it should seperate out any unique copies, for instance if
they have summoning sickness, or if an equipment is attached, or if some are tapped. only items
with identical names and states should stack."*

Owner, same day, on how it must behave while a spell is choosing targets: *"i am unsure of how
this interacts with targeting, for instance in a spell like fireball, where you may want to
spread its damage across several grouped targets"* and *"afaik, it doesnt [show target borders],
but since all the cards maintain positioning, the user remembers what they have targeted."*

Two rules fall out of those, and the implementation is shaped by them rather than by the
frame-cost motive:

1. **A pile is a claim that the members are interchangeable.** So the split predicate is
   over-inclusive by construction: any observable difference splits. A missed merge costs one
   un-merged pile; a false merge is a lie about the board, which the trust doctrine forbids.
2. **Nothing may move under the player while a decision is in flight.** Positional memory is
   how the player tracks what they targeted, so the whole battlefield expands to its familiar
   ungrouped layout for the entire duration of a chooser / an unresolved stack / a combat
   declaration window, and only re-collapses once all of that has cleared.

## The stack key (`wagicBoardStackKey`, CardGui.cpp)

Two battlefield permanents draw as one pile **iff** this string matches byte-for-byte.
Fields, in order:

- **printed identity** — `getName()`, `getMTGId()` (the printing), and the model's own name
  (so a transformed / renamed instance never merges with an untransformed one)
- **ownership** — `controller()`, `owner`
- **tap / sickness / phasing** — `isTapped()`, `isUntapping()`, `hasSummoningSickness()`,
  `isPhased`, `phasedTurn`, `frozen`, `exerted`
- **power / toughness, every channel** — `power`, `life`, `toughness`, `getCurrentPower()`,
  `getCurrentToughness()`, `origpower`, `origtoughness`, `basepower`, `basetoughness`,
  `pbonus`, `tbonus`, `isSwitchedPT` (+ `swapP`/`swapT` only while switched — see the
  uninitialised-member finding below), `wasDealtDamage`. `life` vs `toughness` is the marked
  damage, and `power`/`life` are literally what the small card renders.
- **combat assignment** — `isAttacker()`, `isDefenser()`, `isBlocked()`, `didattacked`,
  `didblocked`, `notblocked`, `willattackplayer`, `willattackpw`, the `defenser` pointer
  (WHICH attacker it is blocking), the `isAttacking` pointer (WHAT it is attacking), `banding`,
  `blockers.size()`, and its damage-order rank via `getDefenserRank`
- **face / flip / morph / transform** — `isFacedown`, `morphed`, `isMorphed`, `turningOver`,
  `isFlipped`, `hasCopiedToken`, `copiedID`, `copiedSetID`, `blinked`, `isACopier`, `MeldedFrom`
- **counters** — every counter's `name`, `nb`, `power`, `toughness`
- **live keywords and types** — the whole `basicAbilities` bitset (so an until-EOT keyword grant
  splits), the live `types` vector, `colors`
- **every display flag that becomes a border or a dim** — `castableNow`, `willPayForFocused`,
  `canAttackNow`, `hasUsableAbilityNow`, `canBlockNow`, `forcedBorderA`, `forcedBorderB`,
  `isExtraCostTarget`, `NECROED`. Two cards wearing different borders are not the same object
  to the player.
- **identity riders** — `isToken`, `isCommander`, `isRingBearer`, `isBestowed`, `suspended`,
  `miracle`, `isDefeated`, `isCascaded`, `isDualWielding`
- **pending values** — `X`, `castX`, `setX`, `kicked`, `sunburst`, `mutation`, `auras`,
  `equipment`, `regenerateTokens`, `flanked`, `MaxLevelUp` (levelers only), `chooseacolor`,
  `chooseasubtype`, `chooseaname`
- **linked permanents** — `myPair`, `shackled`, `seized`, `storedCard`, `hauntedCard` pointers,
  `parentCards.size()`, `childrenCards.size()`, `imprintedCards.size()`
- **granted-ability population** — `cardsAbilities.size()`
- **attachments** — every permanent on either battlefield whose `target` or `auraParent` is this
  card, as `name:tapped:counters:controller`, sorted so attachment ORDER does not split a pile

Excluded on purpose: a card with `target` set (an attached aura/equipment) never stacks — it
has no slot of its own, `RenderSpell` draws it on its host, so it follows whatever the host
does. A card still fading in (`actA < 32`) is also excluded, because the selector cannot click
it either and hiding an arrival mid-animation reads as a dropped card.

## Input choreography

- **Collapsed pile** = one drawn card carrying an `xN` badge; the members it stands in for share
  its exact slot coordinates. The selector's directional tests need a *strictly* greater
  coordinate, so a pile is one cursor stop for the d-pad; a pointer click on it resolves to the
  drawn member (`closest()` breaks ties in `cards[]` order, which is how the drawn member was
  chosen). Every member is state-identical, so clicking any of them is the same act.
- **Focus expands, in place.** When the cursor enters a pile its members fan out at a fixed
  18-unit pitch from the pile's own slot. The pile keeps its one slot, so nothing else on the
  row moves while the player is reading it; the offsets are distinct, so the same left/right
  presses that walk piles now walk members, and any one member can be clicked.
- **A live decision expands the whole board** (`GuiPlay::stacksPinnedNow`). Pinned when any of:
  a `TargetChooser` is live; the action layer `isWaitingForAnswer()`; the stack has an
  unresolved entry; any permanent carries `canAttackNow` / `canBlockNow` (i.e. the acting seat
  is in declare-attackers or declare-blockers); or a click asked for expansion. While pinned
  every permanent gets its own slot — **byte-identical to the pre-D42 layout** — and stays there
  until all of it clears. That is what makes "the user remembers what they targeted by position"
  hold: through targeting, through the response window, through resolution.
  The first cut fanned only the *affected* piles during combat; it was cramped and, worse, put
  the bodies somewhere the player had not seen them. Pinning is the same answer the chooser case
  gets, for the same reason.
- **Belt to those braces** (`CardSelector::CheckUserInput`): an OK press on a pile that is still
  collapsed while a chooser is live sets `mStackForceExpand` and swallows the press — it expands,
  it never targets. In practice the pin has already expanded the board; this covers a pile that
  collapsed on the same tick the chooser came up.
- **Option**: `Options::BOARDGROUPING` ("Stack identical permanents", Options → Game tab),
  **default ON**. The default is minted in `GameOptions::get`'s factory switch, not just
  advertised by the menu — an unset option reads 0 whatever the screen says, and that trap has
  shipped two features dead in this codebase before. Dev builds also honour
  `WAGIC_BOARD_GROUPING=0/1`.

## Stack-entry surfaces (the owner's "wagic doesnt easily have an equivalent" point)

- **Target thumbnails collapse too** (`w57gCollapseTargetIcons`, ActionStack.cpp). When several
  of a stack entry's targets are members of one pile, the row draws ONE thumbnail with an `xN`
  badge instead of N identical ones. The header's `(N)` still reports the TRUE total target
  count — collapsing the icons must not shrink the number, or the row would be true-in-the-
  wrong-scope, which is the trust doctrine's definition of a lie.
- **PRE-EXISTING GAP CLOSED, independent of grouping**: Wagic already marks the board targets of
  a multi-target *ability* (`StackAbility::Render` sets `forcedBorderA` on each target and
  `forcedBorderB` on the source; `CardGui::Render` draws those as rims, one behind the art and
  one as an outline). A *spell* on the stack — the owner's Fireball case — set **nothing**: the
  only record of which copy you aimed at was the stack entry's thumbnail. `Spell::Render` now
  sets the same flags from its own `tc->getTargetsFrom()`, so a targeted spell marks its targets
  on the board in the language the game already speaks. The existing `GameObserver` sweep clears
  them when the stack empties, so no new lifecycle was introduced. This is on for everyone,
  grouping or not.

## Diagnostic (compile-time gated)

`#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)` only — never in a release build.
`WAGIC_BOARDGROUP_PROBE=1` prints one stderr line per 120 `GuiPlay::Render` calls:
ms/frame in the layer, cards present, cards drawn, and the grouping/pin state with its three
causes broken out. `=2` additionally dumps the computed stack key per card, which is how the
uninitialised-member defect below was found.

## Measurements (desktop; the console is the real test)

Windowed self-play, both seats a 40x Memnite probe deck, same binary, same probe decks, arms
differing only by `WAGIC_BOARD_GROUPING`. Matched by cards-present, since the two arms are
separate games (the harness gives no seed pin) and the ms-vs-cards curve is what is comparable.
Times are wall time inside `GuiPlay::Render` only.

| battlefield cards | OFF ms/frame | ON ms/frame | ON: cards actually drawn |
|---|---|---|---|
| 12.9 | 0.459 | 0.575 | 5.5 |
| 18.2 | 0.672 | 0.548 | 6.4 |
| 23.4 | 0.834 | 0.594 | 7.3 |
| 28.7 | 0.994 | 0.771 | 7.5 |
| 33.9 | 1.147 | 0.890 | 7.9 |
| 39.1 | 1.430 | 0.860 | 8.9 |
| 44.3 | 1.474 | 0.927 | 8.8 |
| 49.5 | 1.887 | 1.381 | 9.2 |
| 55 / 54.6 | 2.323 | 1.158 | 9.6 |
| 65.0 | — | 1.150 | 10.5 |

So: **-37% at 44 cards, -40% at 39, ~-50% at 55**, and the curve flattens (65 cards costs what
25 did) because the drawn count saturates while the card count keeps growing. Below ~15 cards
grouping is a small LOSS — the key computation is paid per relayout whether or not anything
merges. That is the honest shape: this is a wide-board optimisation and it does nothing for a
narrow one. Relayout is already lazy (wave-54 lane J), so the key cost is per layout change,
not per frame.

Prediction (falsifiable, for the console): on the owner's vpk memlog `frames` lines, `avg_upd`
on a wide board should fall by roughly the same fraction, and the drawn-vs-present ratio in a
dev-build probe run should track the desktop table. If avg_upd does NOT move on the Vita while
the desktop numbers hold, the console's frame cost is not in `GuiPlay::Render` and D42 is the
wrong lever for it.

## Windowed verification — what I actually saw

Driven with ydotool on a real 1920x1080 window (Hyprland workspace 9), deck = Memnite / Mountain
/ Bonesplitter / Arc Lightning / Lightning Bolt vs Evil Twin, `WAGIC_AI=baka`.

- **N identical copies collapse.** Three Memnites played the same turn rendered as ONE card with
  an `x3` badge and two offset plates behind it; the probe read `4.0 cards present, 2.0 drawn`.
  In self-play with the 40x Memnite decks the same thing at scale: `x5` and `x6` piles side by
  side, `20.4 present / 6.8 drawn`, and a peak of `65.0 present / 10.5 drawn`.
- **Summoning sickness splits.** With three un-sick Memnites already stacked as `x3`, playing a
  fourth left the `x3` pile intact and drew the new one as its own badge-less card, in the same
  row. This is the owner's named case, seen directly.
- **An equipment splits.** The opponent's board showed `Memnite 1/1` and `Memnite 3/1` with a
  Bonesplitter attached as two separate cards — split by both the attachment and the P/T it
  granted.
- **State splits within one name.** The opponent's five Memnites rendered as an `x3` pile beside
  an `x2` pile after combat (the two groups differing in tap/combat state), and its Mountains as
  an `x2` pile beside a single un-merged one. This is the "3 tapped + 5 untapped" case, observed
  as 3-and-2 rather than 3-and-5.
- **Declare-attackers expands.** In the attackers window the piles came apart and each body
  carried its own orange can-attack halo, individually selectable. (Seen on the fan-in-place
  build; the shipped build pins instead, which expands strictly more.)
- **Unresolved stack expands.** Repeatedly in the probe logs: `24.0 cards present, 24.0 drawn,
  pinned=1 (stack=1)` — whenever anything was on the stack the board rendered fully expanded,
  and re-collapsed on the next line once it cleared. This is the positional-stability rule
  firing, observed.
- **Badge placement was fixed by looking at it.** The first placement (top-left, then
  bottom-right) overlapped the printed name and then clipped the toughness digit — a badge that
  hides information is worse than no badge. It now sits in the gutter off the card's top-right
  corner, beside the offset plates; name and P/T both read cleanly at full zoom.

## What I did NOT verify

- **The Vita / PSP build.** Not compiled, not run. The code is platform-neutral C++ in shared
  GUI files, and the diagnostic is compile-time gated out of anything that is not `_DEBUG` /
  `WAGIC_DEVLOGS`, but the console is where the frame-cost claim actually has to land and it is
  untested. Port builds are owner-initiated.
- **Touch input.** Not exercised at all. The pointer path goes through the same
  `closest<CardSelectorTrue>` that the mouse uses, so a tap on a collapsed pile should select
  the drawn member and expand it — but "should" is inference, not observation.
- **A human-seat targeting session over a pile.** The specific choreography the owner asked
  about — a divided-damage spell taking members 1, 3 and 4 of an 8-token pile, and a blockers
  declaration putting two members of one pile onto different attackers — was NOT driven live.
  The game I built for it ended before I had three lands. What IS verified is the mechanism
  underneath: the pin expands the board whenever the stack is non-empty (seen repeatedly), and
  the chooser and combat-window conditions are the same predicate evaluated in the same place.
  `tc=1` never appears in the AI-vs-AI probe logs because `getCurrentTargetChooser()` is the
  human-facing chooser, so those runs cannot exercise it. **This is the first thing to drive in
  the next windowed session.**
- **The `xN` collapse on stack-entry thumbnails**, and the new spell target rim, were not seen
  on screen — no multi-target spell resolved in a driven game. Both are small, local, and
  compile-clean, but unobserved.
- **Interaction with `GuiCombat`'s own layer** during blocker ordering. GuiCombat draws its own
  card lists and is untouched by this lane; grouping is a `GuiPlay` concept only. Not exercised.

## Engine finding for the ledger (NOT fixed here)

`MTGCardInstance::swapP`, `swapT` and `MaxLevelUp` are **never initialised**. `swapP`/`swapT`
are assigned only inside `MTGCardInstance::switchPT()`; `MaxLevelUp` only by the leveler parser.
On every other card they hold stack garbage that differs per instance — observed live as
`swapP=1999648119, swapT=1818587231, MaxLevelUp=1919249251` on two otherwise identical Memnites.

This is benign today only because nothing reads them unconditionally. It was not benign for this
lane: reading them in the stack key made **every** pair of identical permanents look different
and the whole feature never fired once, until the `=2` key dump showed it. The key now reads
them only when `isSwitchedPT` / `isLeveler` says they are meaningful. The members themselves
should be zeroed in `initMTGCI()` by whoever owns engine hardening — a render lane quietly
patching around uninitialised engine state is exactly the shape that hides the next bug.

## Gates

Baseline re-confirmed on this worktree before any edit, hermetic `make -B`:
PARSETEST 2736/0; suite 1241 with exactly 2 failed (`lifeline.txt`, `merrow_reejerey.txt` — the
known concurrency-only pair) and 55 AI / 0 failed, 0 timeouts.

After the change, same numbers exactly: **PARSETEST 2736 passed / 0 failed; suite 1241 (2 failed
— the same two files by name); 55 AI tests (0 failed); 0 timeouts.** `git diff` contains 0
U+FFFD bytes. No `[AI]` fixture or PARSETEST case was added: the lane changes no parsed or
model-facing string, and an unchanged 2736 is the assertion that it did not.

## Files

- `projects/mtg/include/CardGui.h`, `projects/mtg/src/CardGui.cpp` — stack key, grouping
  enable, per-view stack state, the `xN` badge
- `projects/mtg/include/GuiPlay.h`, `projects/mtg/src/GuiPlay.cpp` — `computeStacks`,
  `stacksPinnedNow`, slot allocation for drawn cards only, follower placement, the pile plates,
  the render skip, the focus/pin dirty signal, the probe
- `projects/mtg/src/CardSelector.cpp` — the expand-instead-of-target press guard
- `projects/mtg/include/ActionStack.h`, `projects/mtg/src/ActionStack.cpp` — target-icon
  collapse with true total count, and the spell target rim
- `projects/mtg/include/GameOptions.h`, `projects/mtg/src/GameOptions.cpp`,
  `projects/mtg/src/GameStateOptions.cpp` — the option, its ON default, its menu row
