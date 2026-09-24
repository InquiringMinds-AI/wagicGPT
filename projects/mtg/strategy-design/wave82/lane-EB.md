# Wave 82 - lane EB: the casting menu, land rows, hold rows, target/discard/reveal windows

Base `35e1e82dc`. Worktree `worktrees/lanes/w82-EB`, branch `w82-lane-EB`.
Corpus evidence: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260917-191141-final` (binary
`6e0c51238`), records `kind: window` + `seam`; every repro below was read from the record's
`prompt` field (the full message the seat saw) before code was touched.

Gates on the branch tip: build `make -f Makefile.sdl -j8` (both link-time guards OK:
`check-ctor-init: OK (123 files)`, `check-reply-instructions: OK (5 source files, 38 guides)`);
PARSETEST **7370 passed / 0 failed** (baseline 7314 / 0); suite THREADS=1 **__SUITE_FINAL__** (baseline 1314
tests / 82 AI, 0 failed; the known flake is `intrepid_adversary_repeated_payment`). Every wagic run
was under `systemd-run --user --scope -p MemoryMax=4G -p MemorySwapMax=0`. No corpus, no pilot
request, no VPK, no config change; `git diff | grep -c $'\357\277\275'` = 0.

Two items on the list turned out to be **already true on the corpus binary** and are recorded as
such, not fixed (M4, M5): the reviewer read the record's `options_text` field - which is the
answer key, bare card names by design - not the `prompt`. This is a reviewer-instrument finding for
the orchestrator (a field a reviewer can mistake for the rendered rows), not a render defect.

One commit per item, explicit paths, no AI attribution: H9 `da9751364`, H8 `1cd11b2d6`,
H1 `391327d60`, H2 `a89e1d711`, H10 `5f820eb82` (its narration-strip fixup squashed in, so
the four commits after H2 were rewritten once), M14 `858e52fd0`, M10 `e95483cdd`, M12 `7f0ee975a`,
M9 `644b35424`.

---

## H9 - a hold expires when the turn it was taken in ends (DONE)

**False before:** the row said "YOU CANNOT COME BACK ... LATER THIS TURN" but the latch
(`w73HoldExpiredByUntap`) measured an OWN-turn hold to the holder's NEXT untap - two turns. Repro
`1789690324-ai_baka_deck125-…-vs-ai_baka_deck162.jsonl` seq 211 (T27 End, `CHOICE: 2 (Hold
priority)`): T28 Underworld Dreams and Teferi's Puzzle Box resolved with no window asked;
`hold_windows_skipped` 277 that game.

**Now:** `nowTurn > heldTurn` whoever's turn the hold was taken on (a hold on the opponent's turn
still dies when THAT turn ends - the same boundary as before; only the own-turn case moved). Every
surface that stated the scope moved with it: the four row spellings (`it stands until this turn
ends`), `[HOW A HOLD ENDS ...]` (`released when this turn ends whatever the rows do`), the
stack-drain note (`released when this turn ends`), the release trace. DK's re-open logic untouched.

**Evidence:** PARSETEST pins rewritten to the new rule: `(27,true,28)` now TRUE (the seq-211
shape), `(13,true,14)` TRUE, `(13,true,13)` false, `(14,false,15)` TRUE, plus a MUST-NOT-MATCH that
no surface still says `until your next turn` / `start of your next turn` / `next untap`. 7315 / 0.

**Weakest evidence:** the corpus cannot be re-run from a lane; the proof is the pure predicate and
the strings, not a game. The one behavioural consequence not pinned: an own-turn hold taken in Main
1 now also expires at that turn's end rather than covering the opponent's turn - which is exactly
the row's promise, but it will raise `hold_events` per game (more holds re-taken) in the next corpus;
that is the expected direction, not a regression, and the corpus's `hold_windows_skipped` should
fall from the 277-class figures.

## H8 - the hold row's short name says what it does (DONE)

**False before:** the two-word head `Hold priority` was read in its Magic sense 25 times across
the corpus; repro `1789690335-ai_baka_deck123-…-vs-ai_baka_deck130.jsonl` seq 78: `PLAN: Hold
priority to block Rorix with Vampire #1 and Vampire #2 in combat. | CHOICE: 3 (Hold priority)` -
the turn's windows and the game forfeited.

**Now:** the head of all three spellings and the identity head is `Stop asking me this turn`; the
body is unchanged (no new body words). Parser tolerance: `isReservedHoldEcho` accepts the new short
name, the old short name (`hold priority`, `hold`) and the old head (`hold priority - pass now, and
do not ask me again`); a new predicate `isHoldRowText` (either head) replaced every spelled-out
13-byte prefix (`holdRowIndexOf`, `w78HoldRowShortName`, `appendStackDeathToDeclineRows`, the
X-menu class test in `AIPlayerGPTParse.cpp`, `menuRowProseName`); the decline-class lists
(`declineRowText`, `logWindowInertRow`, `w72RowIsDeclineOrHold`) carry the new head. Keys
unchanged in kind: pinned that the benefit brace still stays outside the hold key and the rename
adds no number.

**Evidence:** PARSETEST: the seq-78 reply verbatim over a 3-row activation menu binds row 3 under
the OLD name; the new name binds; the old head echoed through its first clause binds; a real row on
a hold-bearing menu still executes; `isHoldRowText` rejects `Stop the bleeding with Healer` and
`Hold the line with Wall of Omens`. 7326 / 0 (+11).

**Weakest evidence:** whether the model's attention lands differently on `Stop asking me this
turn` is a corpus question; what is proven here is that both names parse and every consumer moved.

## H1 - the `Land drop:` line names the window's shape (DONE)

**False before:** on every folded casting menu (deck130 44/44 windows) the situation block said
`The land drop is its OWN decision (a "Land drop:" question with its own Play options), so the
absence of a land from the choices below does not mean the drop is gone` three lines above a `Play
Plains` row. Repro `1789690312-ai_baka_deck152-…-vs-ai_baka_deck130.jsonl` seq 26: `PLAN: Play
Plains ... CHOICE: 5 (Cast nothing right now)` - the drop was lost.

**Now:** the seat stamps the window's shape (`mLandShapeForPrompt`, `kLandShape*`) around its
askModel call and `landDropStatusLine` renders by it:

- on the casting menu that carries the rows: `Land drop: a land play IS available to you right
  now, and the "Play <land>" row(s) on the list below ARE this turn's land drop: taking one plays
  that land instead of casting in this window, and this casting decision is asked again after it
  enters. Taking any other row plays no land in this window; the drop is then asked again on its
  own while it is still available this turn.` (+ the D15 MDFC exception)
- on a fold-regime window with no land rows (priority, target, reveal): `... It is offered as its
  own "Play <land>" row on your CASTING menu (or, when no casting menu is put to you, as a "Land
  drop:" question of its own), not on this menu - so the absence of a land from the choices below
  does not mean the drop is gone.`
- `WAGIC_GPT_LAND_SEPARATE=1`: the pre-P9 paragraph, byte for byte (pinned equal to the
  three-argument call).

`w73SiblingBoardKey` normalises the shape lines so a casting window and its sibling priority window
on one board still compare EQUAL (pinned: two boards differing only in that line yield identical
sibling keys). No number in any new line (pinned digit-free).

**Weakest evidence:** the re-render of seq 26 through the live composition needs the live caller;
what is pinned is the pure line per shape and the key equality. The "asked again on its own while it
is still available this turn" clause rests on the corpus (deck50: "the 16 windows where a spell was
taken over the land row were all re-put in the same turn"), not on a fixture.

## H2 - one land regime: the standalone ask is the fold's degenerate case, tagged (DONE)

**Before:** both regimes ran at once - a standalone `Land drop:` ask (`['Play Marsh Flats', 'Play
no land right now']`, `1789690346-ai_baka_deck123-…-vs-ai_baka_deck152.jsonl` seq 34/37) fired
whenever the cast branch put no menu to the model, and nothing said which shape a window was.

**Decision (documented):** the standalone ask STAYS, as the fold's degenerate case. It runs only
when the cast branch above it put no menu this window (nothing castable, a hold standing, a
no-progress marker, or the phase's casting decision already closed - which is how the drop gets
re-offered after a cast is taken over it). Routing it through the menu builder would have meant a
casting menu with no cast rows; the seams already agree on the row text (`buildLandDropRows`), so
the two shapes are consistent by construction. Its own `Land drop:` line now says so: `... the
"Play <land>" rows below ARE this turn's land drop, asked on their own because no casting menu is
being put to you in this window.`

**Tag:** every window record that carried land rows now has `land_shape`: `folded` (cast menu with
land rows), `standalone` (the separate ask under the P9 regime), `separate` (the same ask under
`WAGIC_GPT_LAND_SEPARATE=1`); absent on windows with no land rows. `w82LandShapeField` is pure and
pinned for all three values and four negatives. The owner reads the P9 revert criterion off the next
corpus's counts of the two shapes.

**Weakest evidence:** the field is written where the record is built (`AIPlayerGPTTranslog.cpp`,
one insertion after `window_seq`) from the same stamp the prompt read; no live window was produced
in this lane, so the first corpus is the first sight of the field on a real record.

## H10 - the cast row leads with the target verdict when every legal target survives (DONE; decline = engine item)

**False before:** `1789690327-ai_baka_deck130-…-vs-ai_baka_deck162.jsonl` seq 10/11 - the cast
row read `{kills 0 of the 1 CREATURE target at 3 damage - and 3 to the opponent at life 20 leaves
them at 17}`, the row was taken, and the target ask printed `(this ask has no pass row)` over
`1. The opponent ... 2. Master of the Feast ... SURVIVES (toughness 5) 3. Yourself` - every decline
rule in the guide unreachable.

**Now (least-invasive, on the row that can still decline):** the same clause opens with the fact
it adds up to - `{every legal target SURVIVES - casting this kills nothing: kills 0 of the 1
CREATURE target at 3 damage - and 3 to the opponent at life 20 leaves them at 17}` - claimed ONLY
when no creature dies on either side, the player tail names no win, and no legal target went
unpriced (a planeswalker or battle among the targets withholds the claim: `unpricedTargets`
counted at the call site). Everything after the colon is the wave-54 wording byte for byte. The lead
rides the `{...}` channel: pinned out of the narration (`stripNarrationDecoration` learned the new
opening) and out of every key.

**Not done - engine item:** a decline on the target ask that unwinds the cast. The spell is on the
stack with mana paid when `chooseTarget` runs; unwinding needs the engine (refund + stack removal),
not the GPT layer. Recorded, stopped.

**Weakest evidence:** the pure tag is pinned on the seq-11 numbers; the call-site walk that counts
unpriced targets was not exercised by a fixture (a suite fixture on this path tests the heuristic).

## M10 - the hold row says when every other row is free (DONE)

`1789690314-ai_baka_deck146-…-vs-ai_baka_deck152.jsonl` seq 36: Lolth +0, Lolth -3, hold, pass,
under `[crack-back verdict: LETHAL ...]` - nothing on the menu cost mana. The row is NOT suppressed;
it carries ` [nothing here costs mana - holding gives away free actions]` when every acting row
(mana-only rows excluded) is a zero-mana activation. `w82RowsCostNoMana` is pure over the rendered
row text - a `[cost: ...]` bracket with no mana pip; a row with no cost bracket (a cast, a flip) is
NOT proven free, so the marker never fires on a guess. Pinned: the seq-36 rows fire it, a `{2}`
cost or a bare cast row does not; the marker moves no hold key and no option-set key, strips from
the narration, and the marked row still binds by its short name.

**Weakest evidence:** the marker is appended in the priority seam (`AIPlayerGPTSeams.cpp`), not
in a fixture-reachable path; the predicate and the key/echo properties are what is pinned.

## M9 - the life/draw X badge is refusal-first when the NET kills (DONE)

`1789690324-ai_baka_deck125-…-vs-ai_baka_deck162.jsonl` seq 296: `[<- best X for this cast: X=9 -
largest affordable X ... but NET -20 life ... this KILLS you. X=2 is the largest listed X whose NET
(-6) leaves you alive, at 2]` - recommendation head, refusal in the middle, a survivable X at the
end; the pilot cast X=2; `x_cast_row_refusal_markers` 0. Now, when the NET kills, `xMonotoneMarker`
opens `[<- REFUSED by NET life: the largest affordable X (X=9: gains 9 life and draws 9 cards; no
listed X does more) is NET -20 life for this cast, counted from the 8 life the 2 damage ALREADY ON
THE STACK leaves you on, and puts you at -12; this KILLS you. X=2 is the largest listed X whose NET
(-6) leaves you alive, at 2 - alive is not a reason to cast: this badge names no X to take]`;
`xCastRowMarkerFrom` never puts the `best X for this cast:` head on a body that opens `REFUSED `;
the `x_cast_row_refusal_marker` event counts the new shape. A survivable NET keeps the wave-63
badge byte for byte (pinned). PARSETEST 7370 / 0 (+7).

**Weakest evidence:** the seq-296 badge is re-rendered through the pure builder with the record's
numbers (cap 9, life 10, stack 2, NET -20 / safe X=2 NET -6), not through the live survey.

## M14 - the Upkeep animation clause names the block it cannot make (DONE)

`1789690314-ai_baka_deck146-…-vs-ai_baka_deck152.jsonl` seq 45 planned `Animate Hive to block
Brute` at Upkeep. The clause now reads ` [Upkeep offer: this animation lasts only until end of turn
- it is over before the opponent's turn begins, so it CANNOT block on their turn - and the same
row is offered again in your main phase]`. Pinned: both wave-51 facts kept, digit-free, one `[...]`
tail that strips from the narration.

## M4 - discard / bottom / reveal rows (ALREADY TRUE on `6e0c51238`; no change)

Repro `1789690333-ai_baka_deck123-…-vs-ai_baka_deck162.jsonl` seq 41, read from its `prompt`:
`1. Bloodline Keeper (copy 1 of 2 in your hand) {2}{b}{b} (3/3 creature) {card text: ...}`,
`2. Bloodline Keeper (copy 2 of 2 in your hand) ...`, `4. Thraben Doomsayer ... [you already
control one: Thraben Doomsayer]`, `5. Marsh Flats (land) ... {you control 3 lands already; the
most expensive card in your hand you could still reach costs 4}` (the NOT-spare form; `{spare: ...}`
is the same emitter's other branch), `6. Damnation ... {right now: destroys 1 of their creature ...
2 of yours ...}`, `7. Tribute to Hunger ... {right now: ...}`, and the legend line for `{dead right
now: ...}` / `{right now: ...}`. The reveal seam's rows (seq 45) carry `[cost: ...]`, the copy
ordinal and the eligibility mark; the bottom seam (`AIPlayerGPTSeams.cpp` ~15236) carries the copy
tag, the land/mana token and the text snippet. The bare list the seat quoted is the record's
`options_text` field. Nothing to fix in the render; the cheap truthful improvement (a rendered-row
field on the record, or a reviewer-brief note that `options_text` is the answer key) belongs to the
translog/brief, not to this lane.

## M5 - tutor/search window (ALREADY TRUE on `6e0c51238`; no change)

Repro `1789690335-ai_baka_deck123-…-vs-ai_baka_deck130.jsonl` seq 45, from its `prompt`: the
header `ELIGIBILITY: only an enchantment may go to "choose card" ... Pick ONLY from the cards marked
[eligible for "choose card"] below.`; every row carries `[does NOT qualify - goes to "shuffle"]` or
`[eligible for "choose card"]`; identical entries are collapsed with the count and the copy range
(`1-4. Underground Sea (copies 1-4 of 4 in this list) ... x4`, `21-23. Intruder Alarm ... [eligible
for "choose card"] x3`). The 45 rows are the engine's legal set (the whole library), marked, not
removed. Same instrument finding as M4.

## M12 - guide literal sweep (DONE for what was live; the rest recorded)

Sweep of `deck{146,130,123,162,50}_strategy.txt` for the six literals:

| literal | hits | action |
|---|---|---|
| `blocking can leave them as high as` | 0 in all five | nothing to re-key |
| `unless the board changes` (as the hold row's words) | 0 in all five | nothing to re-key |
| `[no cast row now: it must have a target` | 0 in all five | nothing to re-key |
| `you SACRIFICE this` | deck162 L185 | **LEFT** - it is a LIVE emitter (`forcedSacrificeRowTag`, `AIPlayerGPT.cpp:36583`, ` [you SACRIFICE this; they gain N life (its toughness)...`) that no edict reached this corpus; 0 renders is reachability, not deadness |
| `LIFE-TO-DAMAGE CONVERTER` / `LOOP` | deck146 L150; deck123 L77/L144/L200/L223; deck162 L106/L163/L164 | **LEFT, and it cannot render in this pool**: the emitter's own script predicates (`lifeToDamageConverterScript` - a `@lifeof(` trigger with `:life:-` aimed at `opponent`; `lifeLossMirrorScript` - `@lifelostfoeof(`) match no card in any of the seven pool decks (every card of deck146/130/123/162/50/125/152 resolved against `sets/primitives/*.txt`; the four unresolved names are basic-land set variants). A guide-content call for the review seat. |
| the hold row's live text (after H8/H9) | deck146 L203, deck130 L260, deck50 L142-143 | **re-keyed**: deck146 19999 -> 19999 B, deck130 19988 -> 19974 B, deck50 14671 -> 14661 B (and its `until your next turn` -> `until this turn ends`) |

`check-reply-instructions: OK (5 source file(s), 38 guide(s))`; all five <= 20,000 B; committed
with `git add -f`.

---

## Items for the orchestrator / other lanes

- **H10 decline-unwind** = engine work (refund + stack removal from `chooseTarget`); not started.
- **Reviewer instrument (from M4/M5):** `options_text` on window records is the bare answer key;
  two seats read it as the rendered rows. Either a `rows_rendered` field (translog, lane EA's TU)
  or a line in the reviewer brief.
- **H2's tag** lands in `AIPlayerGPTTranslog.cpp` (one insertion after `window_seq`) - a
  potential merge touch with EA's M3 side record.
