# Wave-55 lane C — cast-row / X / target render (D6, D7, D10, D15, D16, D17)

Branch `w55-lane-C`, worktree `worktrees/lanes/w55-C`, base master **8a1768d1c**.
All shipped items are RENDER changes inside `src/AIPlayerGPT.cpp`. No engine rule, no
legality gate, no option ORDER, no cache/staleness key and no window moved; `req.optionTexts`
is untouched everywhere (`shown` / menu copies only). Nothing was removed, capped or hidden —
every item adds a true token to a surface that was silent or spoke in the wrong scope.
`git diff | /usr/bin/grep -c $'\357\277\275'` = **0** (the file was byte-spliced with python rb/wb).

## Gate (this worktree; hermetic `make -B -f Makefile.sdl -j4`, every run memory-capped)

| | base 8a1768d1c (re-confirmed here first) | after |
|---|---|---|
| PARSETEST | 2489 passed / **0 failed** | **2532 passed / 0 failed** (+43) |
| suite | 1228 tests, **2 failed**, 51 AI / 0 failed, **0 timed out** | 1228 tests, **2 failed**, 51 AI / 0 failed, **0 timed out** |

Both suite failures are the brief's known concurrency-only pair, by name on both runs:
`lifeline.txt` + `merrow_reejerey.txt`. No third failure, no timeout. Logs:
`~/.gatelogs/w55-C-base-{parsetest,suite}.log`, `~/.gatelogs/w55-C-{build,parsetest,suite}.log`.
The `after` build is a full `make -B` (144 objects), so no seeded object can be carrying old code.

## Live probe (the falsifiable half)

One full 19-turn GPT-vs-GPT game, `qwen36-35b-a3b` at `:8084`, probe decks 198/199 (Starstorm /
Stone Rain / Devour Flesh / Pyrite Spellbomb / Grizzly Bears against Grizzly Bears + Craw Wurm),
natural end (`WAGIC_SELFPLAY_RESULT winner=1 life0=-4 life1=22 turn=19`). Decklists and both
translogs are preserved in `wave55/probe-lane-C/`; the probe deck files were DELETED from
`bin/Res/ai/baka/` (highest deckN back to 164, verified).

Rendered live, verbatim from the probe translog:

* **D6** `130v152`'s own failure shape, reversed — seq 21:
  `X = 2 {X pricing: same kills as X=3, for 1 less mana} [<- most kills at any affordable X that
  costs you nothing - at X=2 that is THEIRS: Grizzly Bears; YOURS: none]`, and that row was the
  one **taken**. The mark landed on the BOTTOM row of the collapsed run and carried the kill
  lists the collapse had removed.
* **D7 (a)** seq 4: `Cast Devour Flesh {1}{b} {right now: they control 0 creatures - at 0 this
  does nothing; YOU control 0 creatures - targeting yourself does nothing}` — 16 rows.
* **D7 (b)** seq 4: `- legal targets right now: the opponent, you {this row does not pick a
  target yet - taking it asks you next which of the 2 legal targets above}` — 16 rows.
* **D15** seq 11: `Cast Stone Rain {2}{r} … {kills whichever you target: THEIRS - Forest #1,
  Forest #2; YOURS - Mountain, Swamp #1, Swamp #2}` — 6 rows, the exact `130v146` s23 shape.

## D17 — REFUTED on the wave-54 corpus; **nothing was changed**

The docket says fetch rows were "still colourless at deck123 (21 renders) although lane E shipped
`fetchLandColorsClause` — find why it did not fire." It **did** fire. Re-derived on disk over all
42 `.jsonl` files of `matchups-20260903-034355`:

* option rows matching `isFetchCrackLine` (`Put a card onto the battlefield with` / `Put in Play
  with` / `search basic land with`): **24**;
* of those, rows carrying `{this land makes no mana …}`: **24 / 24**;
* of those, rows carrying `, and it adds …`: **24 / 24**; **colourless rows: 0**.
* deck123's own five seat logs hold all 24, e.g. `123v130` seq 8:
  `… {this land makes no mana - crack it for a land: your untapped mana sources go from 0 to 1,
  and it adds {W} (you cannot make {W} right now)}`, and `123v125` seq 61
  `… and it adds {W} or {B}` / `… and it adds {W} or {U}`.

The seat report says so itself — E7 opens "**Carried unchanged from wave-53 M2**" — so the 21-render
count is wave-53's number restated over a corpus that had already fixed it, not a wave-54
measurement. The correct wave-54 statement is "lane E's D20 landed, 24/24". I changed no code for
this item and instead pinned lane E's clause with a REGRESSION case in PARSETEST so a later lane
cannot lose it silently. **D17 should be struck from the docket, not re-worked.**

## What shipped, per item

### D6 — the X menu always marks exactly ONE row, and the mark says what it means
`xBestFreeKillX` only ever speaks when some affordable X kills something of THEIRS and nothing of
YOURS; that held on neither X menu in the corpus, so the marker rendered 0 times in 42 logs while
`130v152` s17 answered a `kills THEIRS: none` row. Three members now, ranked, and one always fires
on a kill-priced menu:
1. `[<- most kills at any affordable X that costs you nothing]` — **byte-identical**, still first.
2. `[<- best trade: the most of THEIRS at the smallest cost to YOURS]` on `xBestTradeX` = the
   CHEAPEST X whose `kills THEIRS` list is maximal. The kill set only grows with X, so that row
   also holds the smallest YOURS of the maximal set — the marker's own words are provable, not a
   heuristic.
3. `[<- no X on this menu kills anything of THEIRS]` on the X=0 row when no X kills any of theirs.
   A fact about the whole menu, stated on the row that spends nothing; never an instruction.

(b) is discharged **by construction rather than by re-expansion**: both "cheapest" rules already
land on the BOTTOM row of a collapsed `same kills as X=N` run — and when the marked row carries no
kill list of its own (a collapse row, or the X=0 row), the MARKER restates both lists inside its
own brackets (`- at X=2 that is THEIRS: Grizzly Bears; YOURS: none`), from the same
`xVictimList` the expanded rows use. No row is removed, added, re-ordered or re-expanded; largest-
first stands.

### D7 — the row's own other half, and whether it commits a target
(a) `edictSelfClause` appends the seat's side of a `target=player` edict's own enumeration:
`; YOU control 58 creatures - targeting yourself sacrifices one of them, your choice, and you gain
its toughness` / `; YOU control 1 creature - targeting yourself sacrifices Goblin #1, and you gain 1`
/ `; YOU control 0 creatures - targeting yourself does nothing`. It prints ONLY when
`spellCanTargetSelf` — the engine's own `TargetChooserFactory` chooser, the same object the row's
`legal targets right now:` list is built from — says the caster is a legal target, so the clause and
the list cannot disagree. The opponent half is byte-identical to wave 49. The gain recipient is the
wave-50 `targetcontroller` read, unchanged: on a self-target the sacrificing player IS the seat.

(b) `targetCommitClause`: `{this row does not pick a target yet - taking it asks you next which of
the N legal targets above}`, N ≥ 2 only — at exactly one legal target the engine commits with no
model call, so no claim is made there. **Scoped to enumerations that contain a PLAYER row**: that
is the shape the model asked about in writing (`123v126` seq 104), and a clause on every
multi-target cast row would be prompt weight for a question the corpus does not show being asked
there. That scoping is mine, not the ledger's — the ledger's ask is unscoped.

### D10 — the price a naming permanent of THEIRS puts on the cast row
`namedCastPenaltyScan` reads the naming permanent's own trigger line
(`@movedto(*[chosenname]|opponentstack):life:-3 opponent && draw:1 controller`) and counts a life
loss only when its payee is the naming player's opponent and a draw only when its drawer is the
naming player's controller. `namedCastPriceTag` renders
`[NAMED BY THEIR Silverquill Silencer #1: casting this costs you 3 life and draws them a card - you
would be at 21]` (`; this KILLS you` at or below 0), reusing lane C's subtraction. Matched on
`nc->chooseaname` vs the card's name — the same field the `[named: <card>]` tag already renders.
**The tag itself is untouched** (two seats asked for that in writing); only the row gains a price.
Placed after both cast forms, so an alternative-cost cast is priced too.

### D15 — ownership on a single-target kill list
`removalVictimTag` (Stone Rain's emitter) and `castKillSummaryTag` (the damage / −X/−X emitter)
both take the caster's own victims as a separate list and render
`{kills whichever you target: THEIRS - Plains #1, Plains #2; YOURS - Mountain #1-#2}`, with
`THEIRS - none` printed rather than omitted when the whole list is the caster's own. **When nothing
of the caster's is on the list both strings are byte-identical to wave 54**, so the common case
does not move and every wave-54 pin still holds. The INDESTRUCTIBLE half still rides the mixed form.
Deviation from the ledger's wording: it asks for "whichever **ONE** you target"; I dropped ONE. The
single-payload rail these tags ride does not by itself prove a one-target `target=` line, and a
cardinality claim that the script has not established is exactly the kind of true-sounding false
token the trust doctrine forbids.

### D16 — ability rows that point damage at a player print the life total
The `action.player` / `action.playerAbilityTarget` branch now calls the same
`damagePlayerVerdict` the cast row and the target ask use, off the same `unwrapDamagerAbility`
+ `rand` guard the creature/planeswalker branches one block above already use:
`Deal 1 damage with Staff of Nin targeting the opponent {right now: takes 1 damage - they would be
at 11}`. Corpus denominator re-derived: **73** ability rows targeting a player in wave 54, **0**
with arithmetic.

## Per-item predictions (falsifiable, for the wave-55 corpus)

* **D6** — X menus (kill-priced) rendering with no marker: **0** (was 2 of 2). Takes of a
  `kills THEIRS: none` row while another affordable row names an opponent creature: **0** (was 1).
  Takes of the TOP row of a collapsed `same kills as X=N` run: **0** (was 1, third corpus).
  Marked rows carrying no kill list anywhere on the row: **0**.
* **D7 (a)** — `target=player` rows enumerating both players whose `{right now:}` verdict names
  only one: **0 / N** (was 178 of 180). A take of the self-target is NOT predicted: that is a
  deck-seat call.
* **D7 (b)** — replies asking, in prose, whether the cast row commits a target: **0** (was 1
  measured, and it cost a win attempt). Rows with a player on the enumeration and no commit
  clause: **0**.
* **D10** — casts of a card named in an opponent `[named: <name>]` tag with no price on the row:
  **0** (was 2 of 2 at the seat it cost). `[named:` tag renders: unchanged in shape and count.
* **D15** — `{kills:` / `{removes:` lists naming a permanent of the caster's own with no ownership
  mark: **0**. All-opponent lists: byte-identical, so the corpus's `{kills: ` count must not
  collapse.
* **D16** — ability rows pointing damage at a player with no life arithmetic: **0** (was 73).
* **D17** — no prediction: no code changed. Fetch rows carrying colours stays **N/N** (24/24).

## What I did NOT verify

* **D10 was never rendered live.** The probe opponent has no `chooseaname` permanent, and a
  `chooseaname` menu is not steerable to a specific name from here — PARSETEST + a code reading of
  the `chooseaname` field only. Its scan is pinned on the real Silverquill Silencer payload
  (`borderline.txt:102764`), including the two wrong-payee negatives, but no live board has ever
  exercised the battlefield loop I added.
* **D16 was never rendered live.** No Pyrite Spellbomb / Staff of Nin activation targeting a player
  armed in the probe game. PARSETEST pins the helper (already wave-54 pinned) and the strip shape;
  the `unwrapDamagerAbility` branch itself is a code reading of the sibling branch 60 lines above.
* **D6's trade and no-kill markers were never rendered live** — the probe's X menu had a free X,
  so branch 1 fired. Branches 2 and 3 are PARSETEST only, on the exact `130v152` s17 victim shape.
* **D7 (a)'s non-zero branches were never rendered live** — the probe seat controlled 0 creatures
  in every Devour Flesh window, so only the `YOU control 0 creatures` branch printed. The
  `123v126` seq 104 shape (58 creatures) is PARSETEST only.
* **No suite fixture ships with this lane.** Nothing here changes an engine rule, a legality gate
  or a game action — every item is a string on the AI seat's prompt, which the test suite does not
  render. The suite was run as a regression gate only.
* **`spellCanTargetSelf` builds a chooser per edict row rendered.** I did not measure its cost on
  wide boards; it is one `createTargetChooser` + one `canTarget` per rendered edict cast row, and
  it only runs on the edict branch of `boardTurnOnClause`.
* **The D7 (b) claim is about the ASK SEQUENCE, not the rules.** It says a follow-up ask happens,
  which is the c4 seam's behaviour at N ≥ 2; I did not audit every path that could commit a target
  without one (an engine change that auto-targets at N ≥ 2 would make the clause false).
* **D15's ownership split changes no INDESTRUCTIBLE bookkeeping** — an immune permanent of the
  caster's own still lands in the `immune` list unmarked. Not observed live, not fixed.
* **No corpus was run**, and the wave-54 numbers I quote (73 ability rows, 24 fetch rows, 178/180
  Devour Flesh rows, 8 X menus of which 2 kill-priced) are my own re-derivation over
  `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-034355/*.jsonl`, not the ledger's.
