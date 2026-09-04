# Wave-58 lane E — D42 board grouping: two implementation bugs + a tokens-only setting

Branch `w58-lane-E`, base `4581580f5`. Frontend/render + one profile option. No engine, no AI
seat, no prompt surface, no parsed string — PARSETEST's count is unchanged at 3455, which is
itself the check that nothing model-facing moved.

Owner's instruction, verbatim: *"well, fix the bugs. and.. give it a setting to apply only to
tokens. ill try it like that."*

---

## What to look for on the Vita

Three things, one per change. If any of them reads wrong, that is the report I need.

1. **The opponent's lands should now sit still.** Play a normal game against the AI. Every time
   it casts something, the whole board used to come apart into single cards and then re-collapse
   when the spell resolved — including your lands and its lands, none of which it was touching.
   That should be gone. Land piles should now change **only** when a land in them actually taps
   or untaps, or when one of them becomes a spell's target. If you still see the board opening
   and closing around a spell nobody aimed at a land, the fix missed a second trigger.
2. **The `xN` badge should stay readable.** Move the cursor along a row of piles: the badge on
   the pile you are *not* on used to get erased by whichever card happened to be drawn after it
   (its neighbour, or the selector's re-draw of the card you are on). It should now be legible
   at all times. It is still *deliberately* covered by the foreground panels — the stack, the
   combat window, the mana bars — and it is hidden for the frame the big card preview is up.
   Two things it still does NOT do, by design, so you can tell me if you wanted them:
   a pile that is expanded shows no badge (the members are each visible, so there is no count
   to give), and while you are choosing targets the whole board is expanded, so there are no
   badges at all during targeting.
3. **The setting is now three-state, defaulting to Tokens only.** Options → Game tab, "Stack
   identical permanents": **Off / Tokens only / All permanents**. Your existing profile (which
   has the wave-57 boolean set to on) comes up as **Tokens only**. In that mode a non-token
   permanent renders exactly as it does with the option Off — your lands and creatures never
   stack, only tokens do, and tokens still split by state (a tapped token never joins an
   untapped one, a sick one never joins an unsick one, an equipped one never joins a bare one).
   Off now actually persists across a restart; under wave 57 it could not (see below).

---

## G1 — the opponent's lands stacking and unstacking

**Mechanism (measured, not guessed).** `GuiPlay::stacksPinnedNow()` had four conditions, any of
which expanded the ENTIRE battlefield to the pre-D42 one-card-per-slot layout. The fourth was
"the stack holds any unresolved entry". That is not a property of any permanent — it is true for
a few hundred milliseconds every single time either player casts a spell or activates an
ability, and it blew every pile on both sides of the board open and re-collapsed it on
resolution.

Proven with a compile-time-gated churn probe added for this lane (`WAGIC_BOARDGROUP_CHURN=1`,
`GuiPlay::churnProbe`), which prints one line every time the drawn shape of the board changes,
with the cause broken out: the pin flipping (with each of its conditions), the focus moving, or a
named card's stack key moving (with the key field that moved). One headless Baka-vs-Baka game,
grouping on:

| cause of a board-shape change | count |
|---|---|
| pin flip | 160 of 205 |
| ... of which pin-ON with `stack>=1` and no chooser / no waiting ability / no combat flag | **76 of 80** |
| ... of which pin-ON from a real target chooser | 4 of 80 |
| a land's own tap/untap | 84 |
| everything else (combat, P/T, counters, attachments) | 45 |

So four fifths of every pin was a spell on the stack, and the lands' own state accounted for
none of it.

**Fix.** The stack-count condition is removed. The positional-memory rule it was defending is
kept where it belongs and made per-card instead of global:

- A live `TargetChooser`, an ability waiting for an answer, and the two combat declaration
  windows still pin the whole board — those are the moments the player is *choosing* and needs
  every body in its own slot.
- A pile whose members are currently **marked** by a stack entry expands on its own.
  `forcedBorderA` (a target) and `forcedBorderB` (the source) are set every frame by
  `Spell::Render` / `StackAbility::Render` — wave 57 added the spell half — and both are already
  fields of the stack key, so a marked card has already split away from its unmarked siblings by
  the time the grouping runs. The cards you aimed at stay individually visible and in their own
  slots for as long as the entry is on the stack; nothing else on the board moves.

**Counterfactual, same binary.** The old condition is retained behind a development-only env
switch, `WAGIC_BOARDGROUP_STACKPIN=1`, so "was it me" is one variable rather than a build swap.
Three deck-pinned headless games per arm:

| arm | pin-ONs caused by the stack | board-shape changes / turn |
|---|---|---|
| `WAGIC_BOARDGROUP_STACKPIN=1` (wave-57 behaviour) | 14, 15, 45 | 3.3, 4.5, 7.0 |
| shipped | **0, 0, 0** | 2.5, 2.1, 1.6 |

The zero is categorical (the condition is gone); the rate column is indicative only — the harness
does not pin the RNG, so the six games are different games of different lengths, and the rate is
not a controlled measurement.

**Prediction (falsifiable).** On a dev build with `WAGIC_BOARDGROUP_CHURN=1` and grouping set to
All permanents, no line reading `cause=PIN 0->1 (tc=0 wait=0 stack=<n>0)` can appear for any
`n>0`; every remaining land-pile change carries `cause=KEY <land> field=tap/sick/phase`. If a
land pile still churns with neither of those causes printed, there is a second mechanism and this
fix did not find it.

**Residual churn I did NOT remove, and why.** On the human seat `GuiHandSelf::Update` refreshes
`willPayForFocused` four times a second, marking the lands the auto-tap plan would tap for the
hand card under the cursor. That flag is in the stack key, so scrolling your hand does re-split
**your own** land piles. It is a real border appearing on those cards — a genuine change in a
member's grouping state, not phantom churn — and removing it from the key would make a pile that
mixes bordered and unbordered lands, which is a lie about the board. With the new Tokens-only
default it cannot fire at all. Flagged rather than fixed; say the word if it reads as churn too.

## G2 — the indicators not staying on top

**Mechanism (render order, exactly).** Two occlusion paths, both real:

1. *Inside the layer.* The badge sits in the gutter at `actX+12 .. +25` (scaled). A card is 28
   wide and slots are `CARD_WIDTH` = 31 apart, so the next slot's card covers `actX+17 .. +45`.
   `cards[]` is zone-arrival order, not left-to-right, so whether the neighbour is drawn before
   or after this card is arbitrary — hence "*sometimes* not visible".
2. *Between layers.* `DuelLayers::Render` walks its layers back-to-front (`for i = nbitems-1
   down to 0`). GuiPlay is the third of twelve to draw. `GuiHandOpponent`, `GuiAvatars`,
   `GuiHandSelf`, `CardSelector` — which re-renders the **focused** card on top, the wave-56
   `castableNow` lesson and the "especially when targeting" half of the report — the action
   layer, `GuiCombat`, the whole `ActionStack` and the mana bars all paint over GuiPlay
   afterwards. A badge drawn "last within one card's Render" is not last on screen.

**Fix.** The badge draw is removed from `CardGui::Render` and becomes `GuiPlay::RenderStackBadges()`,
called by `DuelLayers::Render` **immediately after the card selector layer**. That puts every
badge above every card of its own pile and above the focused card's re-render, which is the
requirement, and leaves the deliberate foreground panels (action layer, combat, stack, mana)
covering it, which they should. Position, colours and size are unchanged from wave 57 — only the
z-order moved. It is held back for any frame where the selector is showing its big 200x285 card
preview (`CardSelector::isShowingBigCard()`), because little badges floating over that would read
as a glitch.

**Prediction (falsifiable).** With a pile on the board, the badge is legible in all three of:
the pile focused, the pile's right-hand neighbour focused, and the pile un-focused with the hand
open. If it still vanishes in any of those, the occlusion is coming from a layer that draws after
the selector and the call has to move further down the loop.

## The setting

`Options::BOARDGROUPING` becomes a three-state enum, `OptionBoardGrouping`:
**1 = Off, 2 = Tokens only, 3 = All permanents**, and the `GameOptions::get` factory mints
**Tokens only** as the default.

- The values start at **1 on purpose**. `GameOption::write` treats `number == 0` as "absolutely
  default. No need to write it" and never writes it — so the wave-57 boolean could not persist an
  explicit Off at all: turning grouping off never survived a restart. 0 now means "no value in
  this profile" and reads back as the default.
- `GameOptionEnum::write` refuses anything outside `[1, values.size())` because it treats the
  stored number as an index, which would have silently dropped the third value.
  `GameOptionBoardGrouping` overrides `write` (name out, all three values) and `read`.
- **Migration.** The option now writes NAMES, so a numeric value in a profile can only be the
  wave-57 boolean: `1` -> **Tokens only** (the owner's choice for the next build), `0` -> **Off**.
  An unparseable value falls back to Tokens only rather than to 0, which would re-arm the default
  on a configured profile.
- Verified as a live round trip on the built binary, not by reading the code: a profile carrying
  the legacy `board_grouping=1` loaded as `number=2 menuStr='Tokens only'`; saving Off wrote
  `board_grouping=Off`, which loaded back as `number=1 menuStr='Off'`; saving All wrote
  `board_grouping=All permanents`. The temporary instrumentation used for that is not in the
  commit, and the profile/settings files it touched were restored byte-identical to master's.
- Menu row: `WDecoEnum(OptionInteger(BOARDGROUPING, "Stack identical permanents", ALL, 1,
  TOKENS, "", OFF))` — the same shape the Kicker Cost row uses.
- **"Tokens only"** is enforced at one place in `computeStacks`: a permanent with
  `!card->isToken` is never offered to the grouping map, so it is neither a leader nor a member
  and renders exactly as it does with the option Off. The key is untouched, so the unique-state
  split (tapped / sick / equipped / countered / bordered ...) holds in both modes.
  `isToken` is the engine's own flag on `MTGCardInstance` and is already a key field.
- The dev-build env override `WAGIC_BOARD_GROUPING` now takes the same 1/2/3 values (a legacy 0
  still reads as Off).

## Gates

Incremental on top of a hermetic `make -B` of the whole tree, memory-capped, detached:

- **PARSETEST: 3455 passed, 0 failed** (unchanged — no case added, because no parsed or
  model-facing string moved).
- **Suite at `WAGIC_TESTSUITE_THREADS=1`: 1245 tests, 0 failed, 61 AI tests, 0 failed,
  0 timed out.**
- Suite at the default 16 threads: 1245 tests, 2 failed — `lifeline.txt` and
  `merrow_reejerey.txt`, the two known concurrency-only files by name, both green at THREADS=1.
- `git diff | grep -c U+FFFD` = 0.

## Files

- `include/CardGui.h`, `src/CardGui.cpp` — `wagicBoardGroupingMode()` / `WagicBoardGroupingMode`;
  the badge draw removed from `CardGui::Render`
- `include/GuiPlay.h`, `src/GuiPlay.cpp` — the pin fix and its dev-only counterfactual switch,
  the marked-pile expansion, the Tokens-only filter, `RenderStackBadges()`, `churnProbe()`
- `include/CardSelector.h`, `src/CardSelector.cpp` — `isShowingBigCard()`
- `include/DuelLayers.h`, `src/DuelLayers.cpp` — the badge pass, drawn after the selector layer
- `include/GameOptions.h`, `src/GameOptions.cpp` — `OptionBoardGrouping`,
  `GameOptionBoardGrouping` (write + migration), the Tokens-only factory default
- `src/GameStateOptions.cpp` — the three-state menu row

Every diagnostic added here is inside `#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`.

## What I did NOT verify

- **Anything on screen.** No windowed run was driven. Two `wagic` processes belonging to other
  work were live on this host and the owner's session was on workspace 1, so taking the display
  was not safe, and the memory-flagged cost of screenshot-driving is real. **Both G2 claims rest
  on the render-order proof, not on eyes**: the badge is now drawn after the selector layer, and
  the layer order is a counted loop in `DuelLayers::Render` — but nobody has SEEN the new badge.
  If it does not appear at all on the Vita, look first at the font lookup in `RenderStackBadges`
  (`observer->getResourceManager()->GetWFont(Fonts::MAIN_FONT)`), which is the one call that can
  return NULL and make the whole pass a silent no-op.
- **The Vita / PSP build.** Not compiled, not run. The change is platform-neutral C++14 in shared
  GUI files with no new include and no new API, but the console is where this is judged and it is
  untested. Port builds are owner-initiated.
- **A human seat.** The churn measurements are AI-vs-AI, where `GuiHandSelf::Update` skips its
  oracle refresh entirely, so the human-only display flags (`castableNow`, `willPayForFocused`,
  `canAttackNow`, `canBlockNow`, `hasUsableAbilityNow`) never moved in any of my runs and their
  contribution to churn is **unmeasured**. The `willPayForFocused` residual described above is
  reasoned from the code, not observed.
- **The marked-pile expansion actually firing.** `forcedBorderA`/`forcedBorderB` are set inside
  `Spell::Render` / `StackAbility::Render`, which do not run headless, so the replacement for the
  old whole-board pin was never exercised. It is the single least-tested line in this lane.
- **Touch input, and `GuiCombat`'s own layer.** Untouched and unexercised, same as wave 57.
- **Whether "not visually intuitive" is addressed.** It is not — that is the owner's separate
  design verdict and nothing here touches it.
