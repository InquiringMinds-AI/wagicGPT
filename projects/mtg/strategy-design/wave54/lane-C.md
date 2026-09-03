# Wave-54 lane C — cast-row pricing (D4, D10, D11, D18, D5)

Branch `w54-lane-C`, worktree `worktrees/lanes/w54-C`, base master `42b3bf5da`.
All five items are RENDER changes inside `AIPlayerGPT.cpp`. No engine rule, no legality
gate, no cache key and no option ORDER changed; nothing was removed, capped or hidden.
Every item adds a true token to a surface that was silent or in the wrong scope.

## Gate (this worktree, `make -f Makefile.sdl -j4`, memory-capped runs)

| | base (42b3bf5da, re-confirmed here) | after |
|---|---|---|
| PARSETEST | 2258 passed / 0 failed | **2322 passed / 0 failed** (+64) |
| suite | 1219 tests, **2 failed**, 49 AI / 0 failed, 0 timeouts | 1219 tests, **1 failed** (`lifeline.txt`), 49 AI / 0 failed, 0 timeouts |

The one suite failure is a member of the brief's known concurrency-only pair
(`lifeline.txt` + `merrow_reejerey.txt`); the base run of the same invocation on this
worktree produced both. No third failure, no timeout.
`git diff | grep -c $'\357\277\275'` = 0 (AIPlayerGPT.cpp was byte-spliced with python rb/wb).

## Live probe (the falsifiable half)

One full 43-turn GPT-vs-GPT game, `qwen36-35b-a3b` at `:8084`, probe decks 198/199 stacked
with Hammer of Bogardan / Path to Exile / Doom Blade / Silverquill Command against Shield
Sphere + Grizzly Bears + Rorix. 113 decision records. Decks and both translogs are preserved
in `wave54/probe-lane-C/`; the probe deck files were DELETED from `bin/Res/ai/baka/`
(highest deckN is back to 164).

Rendered live, verbatim from the probe translog:

* `{kills 0 of the 1 CREATURE target at 3 damage - and 3 to the opponent at life 19 leaves them at 16}` (D4)
* `{no creature target - and 3 to the opponent at life 20 leaves them at 17}` (D4, the player-only shape)
* `The opponent (player, life 19) {right now: takes 3 damage - they would be at 16}` (D4 iii)
* `Yourself (player, life 20) {right now: takes 3 damage - you would be at 17}` (D4 iii)
* `{removes: Shield Sphere}` on a Path to Exile row, `{kills: Shield Sphere}` on a Doom Blade row (D11)
* `{leaves 4 sources - no other row on this menu needs more than 4}` (D18, positive form)
* `{modes live right now: none; dead (no legal object right now): creature gains 3/3 and return creature, ...}`
  and, two turns later, `{modes live right now: creature gains 3/3 and you draw, ... ; dead ...}` (D5)

## D4 — the ledger's repro B is REFUTED; the defect is the denominator and the ask

The ledger records `130v162` seq 62 as "the opponent is not on the cast row's list at all".
It is. The full row in the wave-53 corpus reads
`... {kills 0 of the 2 legal targets at 3 damage} - legal targets right now: Shield Sphere ...,
Ob Nixilis, the Hate-Twisted ..., Fate Unraveler ..., **the opponent, you** {card text: ...}` —
the players are enumerated, after the opponent's permanents and before the card text (a scan of
all 21 `Cast Hammer of Bogardan` rows in that corpus finds 0 rows missing `the opponent`).
`castKillSummaryTag`'s player enumeration has been correct since wave-53 lane O.

What was really wrong at seq 62 is scope, not omission: five enumerated objects summarised as
"0 of the **2 legal targets**", at 7 life against an opponent on **1**. A true number in a false
scope is a lie under the trust doctrine, and the number that won the game was on no clause at all.
Shipped:
* the denominator says `CREATURE` (`{kills 0 of the 2 CREATURE targets at 3 damage}`);
* `castPlayerDamageTail` adds `- and 3 to the opponent at life 1 WINS THE GAME`, or the honest
  non-lethal form `- and 3 to the opponent at life 14 leaves them at 11`, whenever `tc->canTarget`
  says the opponent is on this row's enumeration;
* the tail also rides the `{kills: <name>}` form, and a row with player targets and no creature
  targets now prints `{no creature target - and 3 to the opponent ...}` where it printed nothing;
* the TARGET ask's player rows get `damagePlayerVerdict` (`they would be at -2; THIS WINS THE GAME`
  / `you would be at 4` / `you would be at 0; this KILLS you`);
* the target ask's PLANESWALKER rows get `damagePlaneswalkerVerdict` — the helper already existed
  and only the ability path was calling it, which is why seq 63's Ob Nixilis row was bare too.

## D10 — `drawPriceRowTag` gains the same tail `castDrawPriceRowTag` already had

`… so taking it costs you N life right now - you would be at K` / `; this KILLS you`, with the
same `life < 0 means not supplied` convention. Three call sites now pass the seat's life (the
cast row, the ability row, the alternative-cost/cycling mode row — the 21 cycling rows the
ledger counted). The Forced Fruition form is untouched: PARSETEST pins the four-argument shape
byte-identical and the three-argument shape byte-identical.

## D11 — `{kills: …}` / `{removes: …}` for unconditional removal

`spellRemovalVerb` accepts ONLY an instant/sorcery whose whole script is one unconditional
removal payload: a bare `destroy`, a `moveto(<removal zone>)`, or Fateful Absence's
`transforms((,newability[destroy],...)) oneshot`. Any `if`/`ifnot`/`may`/`choice`/`rand`/`all(`/
bare `target(`/`restriction{` disqualifies it, tested as WORDS so that Path to Exile's
`notaTarget(land[basic]|mylibrary)` land-fetch rider does not disqualify the card the item is
about. INDESTRUCTIBLE targets are NAMED, not silently dropped
(`{kills: A - INDESTRUCTIBLE, destroy does nothing: B}`, `{kills nothing: every legal target is
INDESTRUCTIBLE (B)}`) — a silent omission is the shape the model confabulates into.

## D18 — the menu, not the row

`applyMenuFitTags` runs per ASK ATTEMPT over the menu copy (a row cannot know its own number
until the livelock suppression and any re-ask removal have settled), tagging each priced row
with what its own payment leaves and which numbered rows need more than that:
`{taps you out - rows 3 and 4 need more mana sources than the 0 this leaves}` /
`{leaves 5 sources - no other row on this menu needs more than 5}`.
The claim is deliberately a COUNT claim — the same per-source rule `{leaves N of your M}` and the
strand clause already make — so it is true by construction and asserts nothing about colour; a
"stays affordable" wording would have been a render falsehood the moment colours bound.
`{X}` rows are unpriceable and are left alone; a menu with fewer than two priced rows gets no
clause. `opts` stays the UNTAGGED row identity, so `mLastCastLine` / `mStuckCastLines` (the
livelock breaker) key on a string that does not drift with the rest of the menu, and `rowUses`
is erased in step with `opts` on a re-ask removal.

## D5 — per-mode live/dead, and what the second surface actually is

`modalChoiceModes` parses `choice name(<label>) <payload>` lines out of `magicText`, taking the
payload's OUTER `target(...)` and, separately, the object set of a sub-ability handed to another
controller (`ability$! … notaTarget(creature|mybattlefield) sacrifice !$ opponent` →
`creature|opponentbattlefield`, because that mode's objects live on the other side of the board —
the exact confusion in s240's reply). Liveness is asked of the engine's own
`TargetChooserFactory` + `countValidTargets`, never re-derived; a mode with no requirement is
live; an unnamed or unparseable choice line annotates NOTHING rather than guessing. The clause is
`{modes live right now: A, B; dead (no legal object right now): C, D}`. Labels render lowercased
because `CardPrimitive::addMagicText` lowercases every `auto=` line at load — that is the engine's
own token and it is what the mode sub-menu shows too, so the two surfaces cannot disagree.

**The ledger's "second modal surface" is not a modal surface.** The nine records rendering
`Choose one mode for Silverquill Silencer:` over "ten to fourteen rows of bare card names" are
Silverquill Silencer's `auto=chooseaname` menu — a CHOOSE-A-CARD-NAME menu whose rows are card
names, not modes (`Silverquill Silencer auto=chooseaname transforms((,newability[@movedto(
*[chosenname]|opponentstack):life:-3 opponent && draw:1 controller])) forever chooseend nonland`).
Annotating it is a different item (what the named card is worth against THIS opponent's deck) and
is NOT shipped here; it should be re-docketed under its own heading. What IS shipped for the
sub-menu is the genuine `CHOOSE_MODE` path: each row is matched by label and marked
`{this mode has a legal object right now}` / `{DEAD right now: no legal object for this mode}`,
built on a `shownModes` COPY so `req.optionTexts` (the staleness key) and the option order are
untouched.

## Per-item predictions (wave-54 corpus)

* **D4** — `{kills …}` rows whose enumeration includes a player and whose summary does not say so: **0** (was 26). Rows saying "legal targets" where they mean creatures: **0**. Player rows in a damage target ask with no verdict while a creature sibling carries one: **0**. Planeswalker rows in a damage target ask with no loyalty verdict: **0**.
* **D10** — `[DRAW PRICE:` rows of either form stating a life cost with no `- you would be at K`: **0** (was 58/85). Forced Fruition lethal rows carrying the tail: **N/N** (16/16 must hold). Casts from a `this KILLS you` row: **0**.
* **D11** — cast rows for unconditional single-payload removal carrying a victim clause: **N/N** (was 0/244). Rows claiming a kill on an INDESTRUCTIBLE target: **0**.
* **D18** — priced cast rows on a multi-priced menu with no fit clause: **0**. Replies whose PLAN names a row the choice they just made had made unaffordable: expected to fall from the `162v152` s11/s12 shape; a single instance is not a refutation, a rate at the wave-53 level is.
* **D5** — modal cast rows carrying a per-mode live/dead clause: **N/N** (was 0/28). Genuine `CHOOSE_MODE` sub-menus whose rows carry no marker: **0**. Replies asserting a mode's object is absent while the clause lists that mode as live: **0**.

## What I did NOT verify

* **D10's tail was never rendered live** — the probe decks carry no draw punisher, so all 0 of the
  probe's rows exercised it. PARSETEST only.
* **The planeswalker verdict on the target ask was never rendered live** — no planeswalker was on
  the probe board. PARSETEST only (the helper itself is wave-50 pinned).
* **`{taps you out - rows … need more …}`, the NEGATIVE D18 form, was never rendered live** — no
  probe menu ever had a row that stranded another. PARSETEST only.
* **The `CHOOSE_MODE` sub-menu markers were never rendered live** — no `Choose one mode for`
  record armed in the probe game (Silverquill Command was offered seven times and never resolved
  to its mode menu). PARSETEST + label-matching logic only.
* **`spellRemovalVerb` has no PARSETEST case of its own** — it takes an `MTGCardInstance`, which
  the parse corpus cannot construct. Its parse RAIL (`scriptHasWord`) is pinned, and the verb
  itself is live-verified on Path to Exile (`{removes:}`) and Doom Blade (`{kills:}`) only;
  Fateful Absence's `transforms(... oneshot)` branch and Vanishing Verse are unexercised.
* **No suite fixture ships with this lane.** Nothing here changes an engine rule, a legality gate
  or a game action — all five items are strings on the AI seat's prompt, which the test suite
  does not render. The suite was run as a regression gate only.
* **The D18 count claim is not a colour claim.** A row it says fits may still be unpayable on
  colours; the wording was chosen so nothing false is asserted, but the model may still read
  "fits" as "castable".
* **No 21/21 corpus was run** (that is D1's discharge, not this lane's).
