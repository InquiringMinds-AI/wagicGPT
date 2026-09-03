# Wave-55 engine ledger — THE WAVE-56 DOCKET (engine / UX only)

Corpus `matchups-20260903-074354` (TWELFTH fair-hand corpus), binary master **b228b8648**
(hermetic `make -B`; wave-55 step-1 lanes A 8fe5c6682, B f7d13d99d, C 38ccaae55, D 8f9ee5736,
E d56efcdaf on base **8a1768d1c** = wave-54 step-1 + P purge + Q autotap search + **R stall-floor
hotfix**; audit lanes G-N carried from wave 54. Gate on b228b8648: suite **1229** with the 2 known
concurrency-only failures (`lifeline`, `merrow_reejerey`), **51 AI / 0**, **0 timeouts**,
**PARSETEST 2636 / 0**; archived `archives/wagic-b228b8648-w55step1`). Pilot **qwen36-35b-a3b** on
Spark :8084. Corpus root `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-074354/`; results file
`corpus-results.tsv`. Seat names `deckA-vs-deckB` = the deckA seat's JSONL, written `AvB` below;
`seq` is the record's own field. **Every number in this file was re-derived on disk by the synthesis
step** with python over the 42 JSONL — **counting rows from the rendered `prompt`, never from the
`options_text` array** (skill #240) — and `/usr/bin/grep` over the 21 `.stderr`.

**INVARIANT 00: THIS CORPUS IS 21/21 NATURAL.** 42 `gamestart`, **42 `gameend`**, no seat log without
one, 0 timeouts, 0 crashes, 0 hangs, 0 adjudications, 0 `Segmentation` / ASAN / assert lines. Wall
time **2 h 03 min**. Second consecutive natural corpus.

**Health.** **3,264 records** = 42 `gamestart` + 42 `gameend` + **7 `recovery`** + **2 `wall_miss`** +
**3,171 decisions** (ask 2,416 / priority 555 / attackers 90 / discard 68 / blockers 21 / reveal 12 /
bottom 9). Decision fallbacks **14 / 3,171 = 0.44%** (wave 54: 1.04%; wave 53: 0.43% — back to the
floor): `plan_choice_conflict` **9**, `named_row_reask` **3**, `stale_echo` 1, `unparsed_reply` 1 —
**no `all_assignments_illegal`, no `stale_livelock`, no `empty_reply`, no `timeout`, no
`engine_answered`**; 2 further `fallback` fields sit on the non-decision `wall_miss` records.
**Baka executed 2 = 0.063%** (`chose -1 of N` twice); `giving this decision to the heuristic` **0**.
`choice: -1` on **5 decisions + 2 `wall_miss` = 7**, and **7 `recovery` records, 0 unpaired**.
`parse_note`: `decision_reversed_in_prose` 14, `echo_index_conflict` 10, `name_over_index` 5,
`hold_row_named` 5 (all five resolved to the HOLD row), `plan_choice_conflict_exhausted` 5,
`plan_choice_conflict_recovered` 4, `stale_echo_in_range` 4, `named_row_reask_recovered` 3,
`attack_last_line_taken` 3, `latched_row_mismatch` 1, `long_reply` 1, `repeat_count_under_two` 1,
`rejected_line_skipped` 1. **`pass_hold_ambiguous` 0** (was 16), `named_row_not_offered` 0,
`named_row_reask_exhausted` 0, `plan_contradicts_noop_row` 0, `multiblock_first_wins` 0,
`commit_retracted` 0, `http_error` 0, `index_name_conflict` 0, `plan_missing` 0. `answer_replaced` 11.
stderr: **`dropping stale async answer` 43** (wave 54: 488), **`releasing so the game advances` 0**
(was 468), **`interrupt window held` 0** (was 936), `auto-passing without a model call` 2,130,
`all actions pass-declined` 734, `land-drop ask NOT issued` 895, `cast ask NOT issued` 149,
`combat decision suppressed` 446, `hold re-opened` 166, `repeat plan iteration` 46,
`no reply after 900s - one retry` 3, `FORCE-CLOSING` 0, `with no source tapped` 0,
`AttemptNew failed to load (404)` **1,725** over 37 distinct resources.
`gameend` counters, present on 42/42: `hold_windows_skipped` sum **735**, non-zero on **21 of 42**;
`mana_only_windows_skipped` sum 544 on 2; `identical_option_asks_resolved` **9** on 4;
`wall_miss_events` 3 on 3; `wall_miss_unrecorded` 2 on 2.
Max seat log **318** (`152v125`, 64 turns); max prompt **30,031** (`152v125` seq 314); **1 prompt over
30,000** (was 2), **252 over 25,000** (was 37, on 43% more decisions and three 60-turn seats).
Total inference **23.48 h over all positive latencies / 22.98 h over decisions only = 26.1 s per
decision** (two units, both stated, #208; the 0.50 h difference is the two 900 s `wall_miss` records).

**THE INSTRUMENT NOTICE — and this wave it is a clearance, not a contamination.** Lane R's hotfix
closed wave-54 D1 completely: 0 stall-floor releases, 0 `interrupt window held`, 43 drops, 0 heuristic
handoffs. **The window economy is uncontaminated for the first time since wave 53**: opponent-turn
share of decisions **19.9% -> 33.8%** (1,071 of 3,171), opponent-turn casting windows 403 -> 890,
seats recording zero opponent-turn decisions 16/42 -> 9/42. Every wave-54 rate marked
UNTESTED (denominator moved) is scored normally below. **The one number that is still not an engine
metric is per-decision inference — see D19.**

**Mulligans (real hands).** 51 opening asks over 42 seats: **33 seats kept 7; 9 mulliganed once and
all 9 then kept 6**; **9 `bottom` records**, one per mulligan, all the model's. Zero double-mulligans,
zero mull-to-zero, **no mulligan decided by the heuristic**. Bottomed: Tovolar's Huntmaster, Vision
Skeins, Hammer of Bogardan ×2, Teferi's Puzzle Box, Talisman of Impulse, Lightning Greaves, Damnation,
Drowned Catacomb. **THE OWNER QUESTION, RULED AND CLOSED:** the standing trigger is a *repeated*
mulligan against an explicit guide floor. **There is no repeated mulligan anywhere in this corpus** —
every chain stops at (keeping 6) — and the four-corpus colour-family break did NOT recur (lane E's
D12 scope tail rendered 3 times and all three decisions were correct). The one recorded oddity,
`162v126` s1 shipping a THREE-land seven under `(3 lands are not enough for your cheapest spell at
mana value 4)`, is a COUNT-family clause, is not a floor break by that deck's own odds-form rule, is
not repeated, and is a deck-content call owned by that deck's reviewer, who recorded it. **No
deck-content question is open for the owner.** deck130 shipped its first mulligans in five corpora
(3 of them), lifting the standing #132-UNTESTABLE tag at that seat.

**Ownership (owner doctrine 2026-08-26).** Engine/UX only below. Guide content is the reviewers';
guide-side halves are named only where a docket item retires one. **Hard caps on the model's legal
choices are rejected on sight**; every item adds a true token, a true row, a receipt, a stamp or one
re-ask, and deletes nothing. Card-behaviour defects go CARD-SCRIPT FIRST. Four window-removing or
window-reordering proposals are rejected this wave and are recorded in `general-strategy.md`.

**STANDING PROCESS FINDING, second wave.** Wave 54's rule — *a lane whose change can alter any emitted
string, window or timing ships its disable flag in the same commit* — gains a clause:
**a disable flag that CRASHES is worse than no flag, because the lane reads as A/B-able.**
`WAGIC_GPT_BOARDINDEX=0` segfaults on this binary AND on its base `8a1768d1c` (lane E, exit 139), so
lane L's board index has had no working A/B for any reviewer for two waves. **D17.**

---

# THE DOCKET, RANKED

## D1 — HIGH — the HOLD latch's re-open predicate is the BOARD key while the model's answer was about the OPTION SET: a stack object that leaves every rendered row byte-identical re-opens a window the model has already closed. 113 of 282 takes were re-asked in the same turn; 12.4% of the corpus's inference is inside identical-declined runs; lane A's "< 5 decisions per loop" FAILS at 13, 15 and 16
(= `general-strategy.md` **R225**; seat-123-130 **E1 HIGH** — phenomenon confirmed, mechanism
REFUTED and corrected; seat-125-126 **A verdict / MED #3**; seat-146-152-162 **headline 1**'s engine
half; the engine seat filed the same class under HIGH #1's cost figures)

**Repro, re-derived record by record.** `123v126` seq **75-87**: 13 consecutive decisions, turn 13,
all Upkeep, `my_life` **20 -> 2** and `opp_life` 23 -> 41 three points at a time, the two-row menu
(`Cast Devour Flesh {1}{b} ...` / `Cast nothing right now` / the HOLD row) **byte-identical
throughout**, and the declined note running 15, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27.
**The HOLD row was taken at s81, s82, s83, s84 and s85 and stopped nothing.** The stack top alternates
every iteration:
`1 (top): ability: Sanguine Bond's Life Loss [from their Sanguine Bond]` /
`1 (top): ability: Exquisite Blood's Life [from their Exquisite Blood]`,
and `holdBoardKeyOf` contains the stack line, so the latch retires on every single iteration.
Primitives verified: Sanguine Bond `mtg.txt:99004` `auto=@lifeof(player) from(*[-lifefaker]|*):
life:-thatmuch opponent`; Exquisite Blood `mtg.txt:37966` `auto=@lifelostfoeof(player):life:thatmuch
controller` — a closed loop. **Mirror:** `123v162` seq **36-50**, 15 windows in one Draw step, life
10 -> 1, the stack top cycling `Underworld Dreams's deal 1 damage` / `Fate Unraveler's deal 1 damage`
/ `Howling Mine's Draw 1`, HOLD taken at s36 and s42 and neither held. **0 casts inside either run** —
the pilot's behaviour is correct at every window (skill #265).

**Scale, corpus-wide.** HOLD takes **282**; takes followed by at least one byte-identical same-turn
re-ask **113 = 40.1%**, with **301 such re-asks** (deck123 59/106, deck125 29/83, deck130 20/64,
deck152 4/16, deck126 1/4, deck146 0/7, deck162 0/2 — seat-123-130's *79 of 170 at its own pair*
reproduces exactly). Identical-declined runs of >= 3 (key `(turn, options_text)`, any decline
including a HOLD take): **798 of 3,171 = 25.2%, 111 runs, max run 16**, and **80 of the 111 runs have
a VARYING stack top while the rendered rows never change**; 74 contain a triggered-ability stack top.
Those runs consumed **2.91 h = 12.4% of the corpus's 23.48 h**. A further **8 of the 43 residual stale
drops** are immediately preceded by the engine's own
`AIPlayerGPT: hold re-opened at the cast/priority seam - the board changed`, so the predicate also
discards in-flight answers (D18).

**What lane A got RIGHT and must not be reverted.** (a) The option-SET key WORKS: the declined note
counts **unbroken to 27** across 31 windows whose per-row life annotations moved every iteration —
precisely what defeated the wave-54 key at 21 distinct rendered tuples. (b) The hold key's life drop
WORKS: **0 of the 113 re-opens is a life-only change.** Every one carries a real stack, battlefield,
hand or library change, classified by diffing the CURRENT SITUATION block across each pair
(`BF+HAND` 20, `BF+HAND+STACK` 19, `HAND+STACK` 6, `BF+LIFE` 5, `LIFE+STACK` 5, `STACK` alone 4,
`BF` alone 4, …). **seat-123-130's stated mechanism — "a life total moving between windows re-opens
the byte-identical option set" — is REFUTED on disk**, and its own cited repro `123v125` s104 -> s105
re-opens on a stack top (`Staff of Nin's Draw 1` -> `Staff of Nin's deal 1 damage`) plus a hand-size
and library change, not a life tick. **Its REMEDY is nevertheless the right one and is adopted.**

**Ask.** Make the hold's re-open predicate the same key the declined-note already uses: the RENDERED
option rows. If every printed row — name, cost, ordinals AND its `{right now: ...}` price — is
byte-identical to the set the model held, the hold stands; anything that changes a printed row
re-opens it immediately, as today. **This removes no row and caps nothing**: it honours a choice the
model made by pressing a row, and it withholds nothing from a seat that has not pressed one.
**Stated risk, in the seat's own terms:** a board change that alters the answer's VALUE without
altering any printed row — an incoming lethal attack while the menu is unchanged — would not re-open
that seam until something printed moves or the turn ends. Bounded (the model already declined that
exact screen; every combat declaration and every cast changes a printed row) but stated, because it is
the reason this is a predicate change and not a suppression.

**Wave-56 prediction (this is the discharge).** Decisions inside a single-phase mandatory life-loss
loop **< 5** (this corpus 13, 15, 16); HOLD takes followed by a byte-identical same-turn re-ask
**< 20 of N** (was 113 of 282); identical-declined runs **< 15%** of decisions and max run **< 10**
(was 25.2% / 16); inference inside those runs **< 6%** of the corpus (was 12.4%);
`identical_option_asks_resolved` **> 30** (was 9). **Falsifier:** the predicate ships and the runs do
not shrink -> the re-opener is not the key, and the item becomes the loop's own priority grant.

## D2 — HIGH — every ability row that points damage at the opponent prints a life total computed from a CONSTANT BASE OF 166: 238 of 248 rows, at every real opponent life from 34 down to 1, and two seats scored it PASS on presence
(= **R226**; seat-123-130 **E2 HIGH**; lane C's **D16** landed with the wrong life source; skill #259)

**Repro.** `130v152` seq **6**, option 2:
`Deal 2 damage with Pyrite Spellbomb targeting the opponent {right now: takes 2 damage - they would
be at 164}` on a screen whose CURRENT SITUATION block reads `Your life: 20 | Opponent life: 20`.
Also `130v152` s10/s13/s30/s34, `130v162` s20, `130v125` s4, `130v123` s6, and every Staff of Nin ping
row at the deck125 seats.
**Re-derived, and the pattern is exact and total.** 248 rendered rows carry `they would be at K`.
**238 imply a base of 166 with no variance**: `2 damage -> at 164` on **176** rows, `1 damage -> at
165` on **61**, plus one; sources `Pyrite Spellbomb` (154 rows), `Staff of Nin` (61),
`Siege-Gang Commander` (23). The **10 correct rows are TARGET-MENU player rows**
(`The opponent (player, life 27) {right now: takes 3 damage - ...}`) — a different emitter — and the
SPELL cast-row tail (`- and 1 to the opponent at life 15 leaves them at 14`) is correct throughout, as
is the `Yourself (player, life N)` branch. **The defect is confined to the ability-row branch.**
Primitives verified: Pyrite Spellbomb `mtg.txt:90168` (`{R}{S}:Damage:2 target(anytarget)`),
Staff of Nin `mtg.txt:112425` (`{T}:damage:1 target(anytarget)`), Siege-Gang Commander
`mtg.txt:105071` (`{1}{R}{S(goblin|myBattlefield)}:Damage:2 target(anytarget)`).
**Why HIGH.** By the trust doctrine a rendered statement is an INSTRUCTION. A pilot reading these rows
can never see a lethal face row from an ability menu, and deck130's entire face-damage rule is a
comparison between `Opponent life:` and a row's damage; 123 of that seat pair's 873 windows carried
the false number. **No guide sentence hedging against it exists or may be written.**
**How it was missed, and it is the wave's silent-instrument case.** The engine seat's #188 audit
recorded old-renders 0 / new-renders 251 / takes 6 and scored **PASS 0/251** — a PRESENCE check on a
clause whose every value is wrong. Skill **#259** is the standing fix: an audited literal carrying a
computed number gains a fourth column, the number checked against the record's own field.
**Wave-56 prediction.** Rows printing `they would be at K` where K != `opp_life` − damage: **0 of N**
(this corpus 238 of 248). A lethal face row is reached from an ABILITY menu at least once.

## D3 — HIGH — the `[RE-ASK]` conflict notice quotes the FIRST coded line while naming the LATCHED line's row number, and one such notice converted a legitimate cast into a pass and was recorded as a recovery
(= **R227**; engine seat **HIGH #2**)

**Repro, verified on the records.** `130v123` seq **109** (turn 26, Upkeep, `priority`, 6 rows):
`coded_answers: 2`, `latched_coded_line: 2`, executed **row 5**. The reply's first coded line is
`CHOICE: 0 (pass)`; 400 words later it reasons to `CHOICE: 5 (cycling with Starstorm)`. The engine
latched the second and executed it — **correct** — then stamped `plan_choice_conflict` and re-asked
with, verbatim:
`[RE-ASK] Your CHOICE line takes row 5 ("CHOICE: 0 (pass)") but your reply says this window is a pass
("I will pass"). Answer again: 0 (pass) if you meant to pass, or the number of the row you want
performed now.`
The row number comes from the LATCHED line, the quoted string and the prose evidence from the
DISCARDED first line. Seq **110**'s entire reply is `CHOICE: 0 (pass)` and the record carries
`plan_choice_conflict_recovered` — **the engine recorded a false success while the notice destroyed a
legitimate Starstorm cycle.** Second firing, same shape: `152v123` seq **78**
(`takes row 2 ("CHOICE: 1 (+1: tap or untap permanents with Teferi ...)")`; the model then answered
row 1, which happens to be what it wanted, so this one cost nothing).
**Denominator.** 12 `[RE-ASK]` notices corpus-wide across 8 games: **8 of the CHOICE-line form —
2 mismatched, and both are exactly the records where `coded_answers = 2` and `latched_coded_line = 2`;
the other 6 have `coded_answers = 1` and 6 of 6 recovered the model's real row** — 3 of the
`"<name>" is not on this list` form (all 3 recovered, 0 wrong casts from a named-row reply, third
corpus), 1 repeat-count form.
**Ask.** Quote the LATCHED coded line, and take the prose evidence from the same region of the reply.
**Wave-56 prediction.** `[RE-ASK]` notices whose quoted `CHOICE: n` index differs from the row number
the same sentence names: **0 of N** (this corpus 2 of 8); `plan_choice_conflict_recovered` records
whose recovered row is a pass while the latched row was live: **0** (this corpus 1).

## D4 — MED — `chosen_text` is empty on 9 records that carry a live `choice`
(= **R230**; seat-123-130 **E3**, deck123 **G3**)
9 of 3,171 decisions, every one on the re-ask path: `146v152` s17, `123v152` s56, `152v123` s77,
`123v126` s25 (+1), `123v146` s16, `130v123` s109 and s122, `162v126` s33. Adjudicating them requires
reading the FOLLOWING record. This is instrument quality, not gameplay: a harvest predicate keyed on
`chosen_text` silently under-counts every class that passes through a re-ask — including this wave's
own HOLD-take census, which had to fall back to the rendered row text. **Ask:** `chosen_text` is
always the executed row's text or an explicit reason string (`"<refused: named_row_not_offered>"`),
never empty while `choice >= 0`. **Prediction:** records with `choice >= 0` and empty `chosen_text`:
**0** (this corpus 9).

## D5 — MED — 445 of 525 rendered `discard` rows carry no verdict of any kind, and the evaluator that would price them is already running on the same cards' cast rows in the same turn
(= **R228**; seat-125-126 **HIGH #2**, deck125 **G2**, deck130 **G10**, seat-123-130 **E4**)
Lane D's three clauses shipped and the LAND half worked: `{spare: you control N lands already}` **44
rows / 25 windows**, `{dead right now: 0 legal targets on the board for it}` **31 / 25** on exactly
the targeted-removal family (Path to Exile 13, Tragic Slip 9, Vanishing Verse 3 — **never a
counterspell**; the withholding rule is genuinely exercised against 114 counterspell rows),
`{you already control one}` **5 / 3**; and **cleanup discards sending a counterspell past a listed
spare land went 3 of 4 -> 0**. Rendered discard rows **525**, bare **445 = 84.8%**.
**Repro of the gap on one menu:** `123v125` seq 48 — rows 2-4 (`Tragic Slip x3`) correctly carry the
dead verdict while, on the same screen and with the opponent at zero creatures, a `Tribute to Hunger`
(an edict at N=0), an `Idyllic Tutor` whose only target is already on the battlefield, and a
`Damnation` on a board with no creature at all carry nothing. Same class at `125v162` s64,
`125v126` s166, `125v152` s216 (a lone Supreme Verdict / Final Judgment at header 0),
`125v152` s104 (an Emrakul below its land count).
**Ask.** Run the SAME dead-verdict family the cast rows compute over the discard menu, so that the
ABSENCE of a verdict means "this card is live" — the rule a pilot most wants at that ask, and one no
guide can currently teach.
**Wave-56 prediction.** Rendered `discard` rows carrying no annotation of any kind: **< 20%** (this
corpus 84.8%); cleanup discards sending a card the engine has priced dead ahead of a live one: **0**.

## D6 — MED — no window during the opponent's combat prints the incoming damage total: 50 windows, 7 of them at or past lethal, and the seat that met them had no `blockers` ask because it had no creature
(= **R229**; seat-125-126 **HIGH #1**; skill #255)
**Repro.** `125v146` seq **32** (Main 1) and **35-41** (Attackers plus six Blockers-phase `priority`
windows), turn 13, `my_life` **10**. The opponent line prints `3 of them are attacking right now` and
then each attacker with its live P/T — `Barrowin of Clan Undurr {2}{b}{w} (5/5) (printed 3/3)
[counters: 1x +1/+1] [tapped - attacking]; Goblin (2/2) (printed 1/1) [tapped - attacking]; Nadaar,
Selfless Paladin {2}{w} (4/4) [vigilance] [attacking]` — **eleven power against ten life, and nothing
anywhere in the prompt says eleven or says the seat is dead.** The seat controlled no creature, so no
`blockers` ask with its existing `you would be at N` header was ever generated; the combat reached it
only through `priority` windows, where that header does not exist. It declined a Sphinx's Revelation
(`max affordable X=2` = 12 life) at every one of the seven and died at **-1**.
**Denominator, re-derived.** 50 non-blockers windows corpus-wide have attackers declared against the
seat; **0 carry an incoming total**; **7 are at or past lethal** by the sum of the attacking
creatures' printed power (all 7 in this one game).
**Ask.** `INCOMING THIS COMBAT: 3 attackers, 11 unblocked damage - you would be at -1.` on any window
during the opponent's combat, with a blockable/unblockable split when the seat has bodies. Every other
price in this engine is a finished subtraction (`[DRAW PRICE: ... - you would be at K]`,
`{right now: destroys 3 of their creatures}`, `{leaves N of your M}`); the one that ends games is not.
**Wave-56 prediction.** Windows during the opponent's combat with attackers declared and no printed
incoming total: **0 of N** (this corpus 50 of 50).

## D7 — MED — the monotone / no-kill X family carries no marker and, alone among cast-priced rows, no mana-fit clause. The take pattern is NOT evidence for this item
(= **R231**; engine seat **HIGH #3, RE-SCOPED**; skill #260, #261)
14 X menus this corpus: **3 kill-priced, all 3 marked** (lane C PASSES in its own stated scope — see
the discharge table) and **11 monotone Sphinx's Revelation menus carrying no marker of any form**,
because all three of lane C's marker families rank a kill dimension this card does not have.
Primitive `mtg.txt:110851`: `auto=life:X && draw:X`, `mana={X}{W}{U}{U}` — strictly increasing in X,
the only cost is mana. The rows read `X = 12 {X pricing: X=12 - you gain 12 life and draw 12 cards}`
down to `X = 0` and — unlike every cast row in the corpus — **carry no `{leaves N sources}` /
`{taps you out}` fit clause**, so the single real trade-off (holding up a counterspell) is not on the
screen at all. Repro `125v152` seq **161**, turn 52, 13 rows, max affordable X = 12.
**The take pattern is a guide rung, and the proposed wave-56 experiment is unnecessary.** The pilot
answered `X = 3` on **6 of 6** menus where a larger X was affordable and took the max on **5 of 5**
menus topping out at X <= 3 (the engine seat's "7 of 7" is one over). deck125's guide carries a
section headed `=== SPHINX'S REVELATION - X IS ALWAYS 3 ===` ending *"reply with the OPTION number
whose line reads 'X = 3'. Every time, and with no arithmetic in front of it."*, and the deck seat
scored the same records **11/11 CORRECT**. Skill **#261**.
**Ask, reduced to what the render still does not say.** (a) carry the `{leaves N sources}` fit clause
onto X rows as onto every cast row; (b) a fourth marker family for a monotone menu that states a fact
rather than a recommendation (`[<- largest affordable X - X=12 gains 12 and draws 12; no listed X does
more]`); (c) collapse the identical middle of a monotone run as lane D collapses `#N` runs.
**Wave-56 prediction.** X rows carrying no mana-fit clause: **0** (this corpus 11 menus of 11).
**No prediction is made about the taken X** — that is deck125's rung.

## D8 — MED — an MDFC's land back face is never offered as a land drop, and it cost a game
(= **R232**; seat-146-152-162 **E-1**)
deck146 runs two Emeria's Call and one Agadeem's Awakening. Primitives `borderline.txt:33968` and
`:1177` both gate the land face behind
`autohand={0}:restriction{can play land,compare(isflipped)~equalto~1} name(<back face>) ...
flip(<back face>) forcetype(land)`, i.e. behind an `isflipped == 1` state nothing appears to set from
hand. **Re-derived over all 3,171 decisions: 0 land-drop asks name either back face.** The only rows
naming them are `Flip Side with Emeria's Call -> DISPLAY TOGGLE only (this card's other face is a
LAND): it currently shows "Emeria's Call"; its other face is "emeria, shattered ..."` — **15 rows,
and they say so honestly**, which is the render telling the truth about an engine limitation. Those
rows also generate windows the seat can do nothing with (wave-54 D23's auto-passed window was one:
`only display-toggle (Flip Side) options; auto-passing without a model call`).
**Repro.** `146v130` seq **22 / 23 / 24** — turns 21, 23 and 25 with the battlefield line reading
`3 are lands: Plains #1-#3` then `2 are lands: Plains #1; Plains #2`, an Emeria's Call still in hand,
and **no `Land drop:` ask issued on any of the three turns** (the only records at those turns are
cleanup discards). The seat spent nine turns at 2-3 mono-white sources holding five black-costed
spells and lost 19 -> 0.
**Ask.** Pin whether `isflipped` is ever set for a card in HAND, or give the land face a separate
`anyzone` hand entry. **Guide obligation on the fix:** deck146's "NOT LANDS FOR YOU" block is deleted
in the same wave — its P10 prediction exists precisely to detect this (skill #238).
**Wave-56 prediction.** `Land drop:` asks offering `Emeria, Shattered Skyclave` or `Agadeem, the
Undercrypt` while the front face is in hand: **still 0 unless the engine changes** — this prediction
exists to detect the FIX.

## D9 — MED — an enumerating render clause with no quantifier was read as an effect list in two consecutive plans
(= **R233**; seat-146-152-162 **E-2**, deck146 **R-2**; skill #249)
`146v123` seq **28**, row 2: `Cast Soul Shatter {2}{b} {right now: 24 tied at MV 0 (their highest):
Human #1; Human #2; ... }`. Primitive `borderline.txt:106069-70`:
`sacrifice notaTarget(...manacost=convertedcost:highest...)`, `text=Each opponent sacrifices a
creature or planeswalker with the highest mana value among creatures and planeswalkers they control.`
— **one card.** The seat's plans at seq 24 AND seq 28 both read *"cast Soul Shatter to sacrifice all
24 Human tokens, wiping their board"*. Lane C's D15/D16 added the commit clause to both-player edicts;
the missing piece is a COUNT on the tie list. **Ask:** `{right now: they sacrifice ONE of these 24,
their choice - all tied at MV 0 (their highest): ...}`. **Prediction:** replies whose PLAN asserts an
enumerating clause removes more than one object: **0 of N**.

## D10 — MED — the opponent's open mana is not printed as a number
(= **R234**; seat-146-152-162 **E-4**, deck152 **R-1**)
Every prompt lists the opponent's permanents with `[tapped]` markers and the pilot must count the
unmarked ones to know whether a counterspell or an instant is live; our own side gets
`Mana available: N total (untapped sources ...)` with the sources enumerated. **Repro:** `152v125` —
**eight creature spells countered across turns 25-51**, every one cast with 5-8 unmarked opponent
permanents on screen and their hand at 5-7; the single cast made at 2 unmarked (seq 101, Tovolar's
Huntmaster 6/6) resolved. The seat led 34-11 on T19 and lost at -11 on T65.
**Ask:** `Their untapped sources: N (colours they could make: {u}{w})` under the opponent battlefield
line. A derived fact the engine already has, and exactly the number the wave-55 deck152 posture
(152-B) now asks the pilot to compute by hand.

## D11 — MED — plan staleness has quadrupled, and a served PLAN is carried alongside the note that retracts it
(= **R235**; engine seat **D-5**, seat-146-152-162 **E-6**, deck152 **R-3**)
`plan_echo_count` max **37 -> 143**, p90 **80**, and **328 of 1,511 records serve a plan more than 40
windows old**. The mitigations fire (`"<card>" is no longer on your menu` renders **622**;
`stale_echo_in_range` 4 and `named_row_reask` 3, all recovered; served PLAN blocks longer than one
paragraph **0 of 3,044**), so this is not yet a correctness defect — it is the mechanism behind every
`named_row_reask` in the corpus and it scales with the restored window economy. **Repro:** `152v125`
seq **101** serves `YOUR PLAN (as you last stated it, 6 windows ago on turn 39; "Katilda, Dawnhart
Prime" is no longer on your menu): Cast Katilda. Attack with nothing (no creatures) ...` immediately
followed by the note that retracts it, at a seat whose prompts reach 30,031 chars.
**Ask, two independent halves.** (a) Force a PLAN refresh at a hard age (~40 windows), not only on
menu invalidation. (b) When the retraction note fires AND the plan is more than ~4 windows old, serve
the note alone and drop the plan text.
**Wave-56 prediction.** `plan_echo_count` max **< 60** (this corpus 143); records serving a plan over
40 windows old **< 60** (this corpus 328).

## D12 — MED — `reveal_stall` is stamped on 100% of reveal records with a value that duplicates `latency_ms`, and `reveal_stall_phase` never varies: the field cannot select the case it was built for
(= **R236**; three seats independently — engine seat set-B **E**, seat-125-126 **MED #4**,
seat-146-152-162 **E-5**; skill #263)
**THE ADJUDICATION THE BRIEF ASKED FOR, in three parts.**
1. **The livelock half of D5(a) is a genuine PASS.** `stale_livelock` **0** corpus-wide (was 1);
   wave-54 HIGH #4 does not recur; no reveal parked; 12 of 12 reveal records resolved and were
   answered by the model (`choice: 1` on all twelve, no fallback).
2. **The force-close half is UNTESTED (N = 0), and now for the right reason.** `FORCE-CLOSING` renders
   **0** in all 21 stderr — because no reveal stalled, not because the guard is too narrow.
3. **The stamp half is a TELEMETRY DEFECT and is docketed as one.** Lane E built the triple to be
   "present only when the driver was parked". Re-derived: present on **12 of 12** reveal records;
   `reveal_stall_secs` equals `latency_ms / 1000` **to the second on all twelve** (4, 5, 14, 24, 24,
   42, 47, 55, 60, 148, 211, 474); `reveal_stall_phase` is **0** on all twelve; the poll-churn floor
   is max(1,800 s, 3 × 900 s) = **2,700 s** and the largest value is 474. So it is a duration field
   duplicating one the record already carries, with a constant third field, and nothing selects a
   park. Repro pair, identically stamped: `152v123` seq 65 (`reveal_stall: 1405, _secs: 4`) versus
   `126v162` seq 25 (`87917 / 474`).
**Ask.** Emit the pair only above the structural threshold the lane intended, or rename it
`reveal_wait_ticks` / `reveal_wait_secs` and reserve `reveal_stall` for the guarded case with a
boolean; and make `reveal_stall_phase` carry the driver's real phase or drop it.
**Wave-56 prediction.** `reveal_stall` present on a reveal answered inside the poll-churn floor: **0**
(this corpus 12 of 12). Every new stamp reports the fraction of its record kind that carries it.

## D13 — MED — the `(blocking trigger: ...)` gain is nested INSIDE the survival verdict, and the standing cost of the block is printed nowhere
(= **R242**; deck126 **G7 / STRATEGY D-3**; skill #258)
`126v146` seq **6**: `B1. Pride Guardian (0/3) [defender] [deals 0 - this block kills nothing, but it
STOPS all 1 damage from reaching you THIS COMBAT] - may block A1 (your blocker dies, attacker lives
(blocking trigger: you gain 3 and may gain 2 more))`. Both halves are true; nesting the gain inside
the survival verdict makes a losing block read as a priced one, and it is the exact pull the deck's
STEP 1 exists to resist. The seat blocked with both walls at 20 life against a lone 1/1 [first strike,
deathtouch], banked 7 life, lost Pride Guardian, and at seq 16 met **five attackers with one blocker**
and lost at -14. **Ask:** three SEPARATE annotations in the order the decision uses them —
`{your blocker DIES, attacker lives} {this combat: you gain 3, and Perimeter Captain may gain 2 more}
{after this combat: you control 1 fewer blocker}`. The third is the number the seat needed and no
annotation carries. **Note:** the deck seat classified its own break as STRATEGY and shipped the guide
half; this is the render half and neither substitutes for the other.

## D14 — MED — the equipment cast row is the one recurring cast row in the pool that has never been priced, fifth corpus
(= **R246**; seat-123-130 **E6**, deck123 **G5**; skill #236)
`123v130` seq **11**: a three-row menu (`Cast Lightning Greaves {2}` / `Cast nothing right now` / the
HOLD row) on a battlefield line reading `of which 0 are creatures`; taken, tapping out, on a turn the
seat had no creature to equip. The live guide attacks this in four separate places and it broke again.
**Every sibling rung at that seat closed when its ROW got a verdict**: edicts at N=0
(`at 0 this does nothing`) **0 of 171 rows**; Tragic Slip on a dead row
(`kills 0 of the N legal targets`) **0 for three corpora**; the second Alarm 0 this corpus.
**Ask:** `{right now: you control 0 creatures - this equips nothing}` on an equipment cast row at zero
own creatures, and `{right now: <name> already wears Lightning Greaves #1}` on a re-offered equip line.
**Prediction:** equipment casts at `of which 0 are creatures`: **0** (this corpus 1 of 3).

## D15 — MED — two rows naming the same card at two prices with identical verdicts, and no mark on the cheaper one
(= **R247**; seat-123-130 **E5**, deck130 **G11**; skill #266)
`123v152` seq **104**, at 2 life against 18: `1. Cast Damnation {2}{b}{b} {right now: destroys 1 of
their creature (1 without a restriction against attacking), 5 of yours}` and `3. Cast Damnation
{4}{b}{b} [from exile] {castable from exile - your card, exiled by their Elite Spellbinder ... it
costs {2} more than printed} {right now: <the same verdict>}`. **The pilot answered 3**, paid two
extra mana, swept its own five-creature board (built over eight turns) and lost the game. The X menu
already solves exactly this shape with `{X pricing: same kills as X=N, for K less mana}` plus its
bottom-of-run marker. **Ask:** `{same effect as row N, for K more mana}` on the dearer row. One
string, and the pool will meet the shape again from flashback, adventure and any future exile-cast
row. **Prediction:** takes of a row while the same card with the same `{right now:}` verdict is on the
same menu at a lower cost: **0** (this corpus 1 of 1).

## D16 — MED — `plan_choice_conflict` over-fires on the pool's own mandated PLAN vocabulary: 9 firings, the corpus's largest fallback class
(= **R244**; deck123 **G2**, carried from wave-54 unchanged in shape)
All 9 co-stamped `decision_reversed_in_prose` (`146v152` 17, `123v152` 56, `152v123` 77, `123v126`
42 / 69, `123v146` 16, `130v123` 109 / 122, `162v126` 33); 4 `_recovered`, 5 `_exhausted`;
**0 of the 9 executed the HOLD row** (lane A's D2b collision stays closed). The shape is a CHOICE line
taking a live row beside PLAN prose containing the pool's own mandated arithmetic vocabulary ("this
window: pass", "stop reached"). **Ask:** fire only when the prose names a card or row label that
(a) appears on THIS menu and (b) is not the row the CHOICE line took. **Pairs with D3** — that is a
separate defect on the same seam and must be fixed even if this narrowing lands.
**Prediction:** `plan_choice_conflict` **<= 4**, none of them a reply whose prose names no other row
on the menu.

## D17 — MED (PROCESS, and it outranks any new instrumentation) — `WAGIC_GPT_BOARDINDEX=0` SEGFAULTS, on this binary and on its base
(= **R255**; lane E run B, exit 139 turn 12; confirmed on the archived base `8a1768d1c`, exit 139
turn 10; flagged independently by the engine seat and by `review-carry.md` §C)
Lane L's board index is the one wave-54 change with a named disable lever, and the lever crashes the
game, so **no reviewer has been able to A/B the board index for two waves** while the lane reads as
A/B-able. No backtrace has been taken and no fix attempted; the legacy per-host-scan path is the
suspect. **This is the standing silent-instrument rule's second clause, now written:** a disable flag
that crashes is worse than no flag. **Prediction:** `WAGIC_GPT_BOARDINDEX=0` completes a full selfplay
game with exit 0.

## D18 — MED — the residual 43 stale drops are the turn-flip seam alternation plus the hold's own re-open
(= **R237**; lane E's D5(b) drift finding; engine seat §1; seat-125-126 **LOW #5**;
`review-carry.md` named this class for wave-56 lane A)
**43 drops** (wave 54: 488), independently classified by the line immediately preceding each across
all 21 stderr: **16 follow a resolving `10DrawAction`** (the turn's own draw), **10 follow a resolving
`NextGamePhase`**, **8 follow the engine's own `AIPlayerGPT: hold re-opened at the cast/priority seam
- the board changed`** (so D1's predicate is also discarding in-flight answers), 9 other
(`StackAbility` resolution 2, a `named_row_reask` re-ask, a wall-miss retry, an added `Cancel`, …).
The NEXT window is a `Casting decision` in **20 of 43** and a `Land drop:` ask in **16** — the
land-drop and casting seams contending for the single async slot across the turn flip. Lane E proved
the shape outright on a stub game with `WAGIC_GPT_DRIFT=1`: 24 of 30 drops alternate two questions on
an unchanged board and the dump prints the first divergent byte
(`inflight: ... Land drop: which land do you play now, if any?` vs
`rebuilt: ... Casting decision (Main phase 1, YOUR turn):`, lengths swapping tick after tick).
**Cost 43 × 26.1 s ≈ 0.31 h** (wave 54: ≈3.2 h). **Ask:** key the async slot on
`(seam, turn, phase, boardKey)` and discard-without-re-issue only when the SEAM changed.
**Priority: MED and no lane of its own** — it is a rounding error now, and D1 removes 8 of the 43 for
free. **Prediction:** `dropping stale async answer` **< 20** corpus-wide with the slot re-keyed.

## D19 — MEASUREMENT / RULING — per-decision inference at a fixed `-j` on a shared server is NOT an engine metric, and this docket stops carrying it as a target
(= **R257**; the engine seat's contention diagnosis, **ACCEPTED and now confirmed by a within-corpus
control it did not run**; skill #262)
Per-decision inference went 21.7 -> 23.9 -> **26.1 s** and lane R's discharge target (`<= 22 s`) reads
as a FAIL. **It is not one.** The 21 games launched together at `-j 21` and finished over 1 h 58 min,
so concurrency falls monotonically while each game's prompts GROW — a control in which the
prompt-size hypothesis predicts the OPPOSITE sign. Measured, first quartile -> last quartile of the
three longest games:

| game | median latency Q1 -> Q4 | mean prompt Q1 -> Q4 |
|---|---|---|
| `125v152` (573 timed decisions, 118 min) | **17.8 s -> 3.0 s** | 12,622 -> **26,919** |
| `123v125` (301, 90 min) | **24.5 s -> 4.8 s** | 6,196 -> **16,431** |
| `125v126` (490, 96 min) | **20.7 s -> 2.6 s** | 6,670 -> **17,847** |

Latency falls six- to eightfold while prompt bytes more than double. Across all 21 games,
**corr(mean concurrency during the game, that game's median latency) = +0.82**; inside the corpus
`corr(prompt_chars, latency)` is negative. The engine seat's own cross-corpus bins and concurrency
arithmetic (effective concurrency 10.2× -> 11.4×, median 11.9 -> 13.4 s, calls > 120 s 45 -> 78)
agree. **Ruling: accepted.** Consequences: (a) per-decision inference is REPORTED with its concurrency
profile and never carried as a falsifiable engine prediction; (b) a prompt-byte trim may not be
justified by a latency argument at this concurrency; (c) the honest inference lever remains the number
of WINDOWS, which is D1.

## D20 — LOW — the `(xN)` event-log collapse still misses phase lines and draws, and it got worse
(= **R250**; wave-54 D24 carried)
**514** duplicate event lines inside single records (was 471): `- Phase: Draw` **230**,
`- Opponent drew a card` **101**, `- Phase: Main phase N` **80**, `- Phase: Cleanup` 5,
loyalty/level/token lines 3-6 each. Visible in a shipped artifact: the `162v146` seq 15 `wall_miss`
record's `events` block reads `- You drew Master of the Feast` twice. GAME LOG is **58.1%** of the
mean prompt (wave 54: 56.1%) and 82.4% on the late-game specimens. The wave-53 verdict declining the
ADJACENT collapse stands and is not re-opened; this is the within-record class and it is free space.

## D21 — LOW — `You named "<card>" with the spell`: 20 of 98 narration lines fall back to a generic source label
(= **R251**; lane D's D22 residual) The other 78 read `... with Silverquill Silencer`. Same
generic-fallback family as the wave-54 stack-line item, which IS fixed (`ability: X's X` **0 of
1,103**; 566 lines carry `[triggered/activated ability]` and still name the effect).

## D22 — LOW — the conditional `{kills N of the ...}` form has no THEIRS/YOURS split
(= **R252**; engine seat **D-7**) 31 rows: `{kills 1 of the 1 CREATURE target at 3 damage - and 3 to
the opponent at life 12 leaves them at 9}` (19) and `{kills 1 of the 1 CREATURE target at -1/-1}`
(12). Ground-truth-checked against each seat's own battlefield line: **none names a creature the seat
controls**, so nothing is FALSE — but the split the 223 unconditional rows carry is absent and the
"1 of the 1" phrasing is opaque.

## D23 — LOW — 1,725 resource 404s over 37 distinct resources, all audio plus `thumbnail`
(= **R253**; wave-54 D26 carried, IMPROVED) Down from 1,927 over 118. Every remaining miss is an audio
file (`mana.wav` 105, `land.wav` 105, `creature.wav` 91, `basic.wav` 88) plus `thumbnail` 205. The
image-cycling loop wave 54 flagged is **gone** and `Destroying WCachedResource` now names stable
backdrops. The residual is the silent `JSfx` stub, not lane J's memo. Set-C watch, no action asked.

## D24 — LOW — the DRAW FORECAST line could print the product, and is LOW precisely because it worked without it
(= **R254**; deck162 **R-3**) `162v123` seq **30** declared `ATTACK: none` at 20 life against 10 and
won on the opponent's draw step, reasoning entirely from two rendered lines
(`DRAW PUNISHERS on the battlefield: yours - ... Every card the OPPONENT draws costs them 3 life` and
`DRAW FORECAST (theirs): their next draw step draws 5 cards (1 + Howling Mine #1 1 + Dictate of
Kruphix 1 + Howling Mine #2 1 + Howling Mine #3 1)`), including the 5 × 3 = 15 multiplication it does
not always do. Suggested tail: `- that is 15 damage on their next draw step; they are at 10`.
**Recorded so the two lines are never simplified away.**

---

# CARRIED ENGINE ITEMS WITH NO CORPUS SURFACE

## D25 — MED — lane M's cache-hit counters reach no log, THIRD corpus
(= **R256**) New `gameend` fields DO land (`hold_windows_skipped` 735, `mana_only_windows_skipped` 544,
`identical_option_asks_resolved` 9, `wall_miss_events` 3, `wall_miss_unrecorded` 2 — all present on
42/42). A `cache_hits` field on the same record is the cheap fix; the lane's own stated watch item has
been unscoreable by every reviewer for three waves.

## D26 — MED — lane B's `all_assignments_illegal` re-ask arm and its `_exhausted` branch have never executed
The class fired once ever (wave 54) and **0 times** this corpus; all 21 blockers records carry
`dropped_assignments: 0`; the menace header renders 0 because **no menace attacker reached any of the
21 blockers windows**. Both halves are UNTESTED, not PASS. Lane D's `nameChoiceMenu` likewise has no
suite pin (needs a `chooseaname` register). Carried from `review-carry.md`.

## D27 — MED — the `{feeds:` clause counts a conditional converter unconditionally and names no release
(= **R238 / D20** carried from wave 54) Unchanged; no lane took it. deck162's guide hand-carries the
qualifier, and its wave-55 release condition (a punisher NAME on the hand line) is a fact the engine
already has. Three prompts whose `{feeds:}` clauses are byte-identical and whose correct answers
differ: `162v123` s18, `162v126` s23 (punisher in hand, takes correct) vs `162v125` s98 (none, break).

## D28 — MED — replay is unusable on human-vs-Baka dumps, and it blocks every owner play report
(wave-54 D29) Four independent failures carried unchanged: the RNG-stream split (**owner said YES**;
its own lane after this corpus), the `read the bones` chooser-order divergence, lane W's two vpk12
dumps, and the equip auto-tap replay blocker. **This is the item that compounds** — every owner play
report is still investigated from a transcript plus a fixture rather than from the board that produced
it. Not corpus-testable.

## D29 — MED — `kMaxOptSources` is 14 and the owner's option-preserving autotap is a no-op above it
(wave-54 D30) Unchanged; `WAGIC_OPT_SOURCES_CAP` / `WAGIC_OPT_WORK_BUDGET` exist in this binary and
were not exercised by any reviewer (`mana_only_windows_skipped` is a different counter). Human seat
only; not corpus-testable.

## D30 — MED — `planPayment` / `potentialMana` / `selectAutoTapProducers` cannot see a `&&`-wrapped mana producer
(wave-54 D31) Unchanged; one call site fixed, the residual needs its own pin. Not corpus-testable.

## D31 — LOW — the Dominating Vampire livelock candidate, unreproduced
(wave-54 D32) Stated as a hypothesis. Not in the pool; the next step that settles it is a per-tick
instrumented run. Carried with the Vita open reports (8 lag games; `bug` game 1788398189).

## D32 — LOW — `GuiPlay::receiveEventPlus` calls `Replace()` unconditionally per ETB
(wave-54 D33) An O(n) play-zone relayout per token created; not measured on the console. Frontend /
human seat, carried.

## D33 — MED (frontend / human seat) — Arena-style grouping + the `CardGui::Render` per-card cost — OWNER ITEM
(wave-54 D34) Carried as the owner's, measured not redesigned. Every number is desktop; **the console
is the test and the next vpk's telemetry is what decides it.**

## D34 — MED — the audit lanes' central watch items are not observable from a corpus
(wave-54 D35) Three seats again report **UNTESTED, not clean**, for lanes G / H / I / K / N: no RSS,
no ASAN, no per-tick verdict trace, 2 Baka samples, no Vita exposure. Lane I's
`In GetCost Seems ManaCost was not properly initialized` renders **532** times and is unchanged in
character from wave 54 (pre-existing noise).

## D35 — MEASUREMENT / DECISION — the log window — OWNER ITEM
(= **R258**; wave-54 D28 continued) Prompts over 30,000: **1**. Mean prompt 10,740 -> **12,174
(+13.4%)** and total volume 23.8 M -> 38.6 M chars, both attributable to the **+43% decision count**
and the new lane clauses (the HOLD row on own turns alone is ~0.52 M chars = 1.3% of the total), not
to a trim regression. GAME LOG share **58.1%** mean, **82.4%** on the late-game specimens.
**D19 changes the argument for compaction**: at this concurrency a byte trim does not buy latency, so
log compaction is now a decision-quality and context-hygiene question, not a cost one. It remains the
owner's call and nothing here proposes one; **D20 is the free half.**

---

# DISCHARGED — SET B: the wave-55 docket D1-D35 and lanes A-E + P/Q/R, by the emitter's actual string

| item | lane / commit | verdict | counts (re-derived) |
|---|---|---|---|
| **D1** the `dt`-denominated stall floor | **R** (in base 8a1768d1c) | **SHIPPED — PASS. DISCHARGED** | `releasing so the game advances` **0** (was 468); `interrupt window held` **0** (was 936; the line is compile-gated out); `dropping stale async answer` **43** (was 488, target < 100); `giving this decision to the heuristic` **0**; opponent-turn share of decisions **33.8%** (target >= 28%); opponent-turn casting windows 403 -> **890**. Seats with zero opponent-turn decisions 16/42 -> **9/42** (predicted <= 4: **FAIL**, but all nine are 16-31-decision seats in 9-15-turn games). Per-decision inference **26.1 s** vs `<= 22 s`: **NOT SCORED — see D19**. Residual 43 drops classified in **D18** |
| **D2(a)** the option-SET de-dup key | A 8fe5c6682 | **SHIPPED — PASS** | the declined note counts **unbroken to 27** across 31 windows whose row annotations moved every iteration (wave 54: 21 distinct tuples defeated the key over 40 windows). `identical_option_asks_resolved` 9 on 4 gameends |
| **D2(b)** the HOLD key's life drop | A | **SHIPPED — PASS 0** | **0 of 113** hold-then-re-ask pairs re-opened on a life change alone; every one carries a real stack / battlefield / hand / library change. seat-123-130's E1 mechanism is REFUTED by this number |
| **D2** "< 5 decisions per single-phase mandatory life-loss loop" | A | **FAIL — 13, 15 and 16** | `123v126` s75-s87 (13, one Upkeep, life 20 -> 2), `123v162` s36-s50 (15, one Draw step, 10 -> 1), and seat-125-126 counted 16 at its own seat. **The engine seat's "UNTESTED (no window)" is wrong** — the loop arose at `123v126` and it is the wave-54 D2 shape from the other seat. Cause located: the re-open predicate, not the key — **D1** |
| **D4** the HOLD row on own-turn windows | A | **SHIPPED — PASS** | 2,352 renders on decisions (2,354 windows), **own 1,323 / opponent 1,029**, LAST row **2,352/2,352**, both retired strings **0**. Longest own-turn identical run **12** (was 50); own-turn runs >= 20 **0**. Wave-54 HIGH #2's coverage half is CLOSED |
| **D11** the exact-label tiebreak | A | **SHIPPED — PASS 0** | `pass_hold_ambiguous` **0** (was 16); `CHOICE: 0 (pass)` never stamps; `hold_row_named` 5, all five naming the HOLD row from index 0 and all five EXECUTING the hold row, each co-stamped `echo_index_conflict`. Name-over-index wins where it should |
| **D19** the own-turn declined note | A | **SHIPPED — PASS** | the note renders on **1,212 windows**, reaching own-turn Main 1 and Main 2 (the `castDeclineRow` phase clause is out of the key). Max N **27** |
| **D21** the HOLD row's benefit clause + take rate | A | **SHIPPED 2,352/2,352 — RATE FAILS CORPUS-WIDE, PASSES WHERE A GUIDE COMMANDED IT** | takes in declined-note N >= 3 windows **67 / 753 = 8.9%** against >= 20%. **Coverage is not the problem: the row is present 753/753.** Split by guide (N >= 2): deck123 **54/141 = 38.3%**, deck130 8/48, deck125 25/381, deck126 1/155, deck146+152+162 **0/190**. `hold_windows_skipped` sum **735** (target > 72), non-zero on **21 of 42** (was 7). See the RULING in `general-strategy.md`; residual **D1** |
| **D3** `all_assignments_illegal` re-ask | B f7d13d99d | **SHIPPED — UNTESTED (N = 0)** | the class **never fired**; all 21 blockers records carry `dropped_assignments: 0`. `_exhausted` 0 renders. The `[RE-ASK]` machinery DID fire on the sibling path: 12 notices, 3 `named_row_reask` all recovered, `named_row_reask_exhausted` **0** (was 2), **0 wrong casts from a named-row reply**, third corpus |
| **D13** the menace blockers header | B | **SHIPPED — half UNTESTED, half PASS 21/21** | **0 of 21 blockers windows had a menace attacker** (`[menace -` and `need TWO or more blockers each` both render 0), so the positive arm is UNTESTED; the negative arm PASSES — the header is silent on all 21 |
| **D14** the fallback class names the pairs | B | **UNTESTED (N = 0)** | no `all_assignments_illegal` record exists |
| **B residual** the `[RE-ASK]` quote | B | **FAIL 2 of 8, and one cost a play** | notices whose quoted `CHOICE: n` comes from coded line 1 while the row number comes from the latched line 2 — **D3 of this docket** |
| **D6** the X marker | C 38ccaae55 | **SHIPPED — PASS 3/3 IN LANE C'S SCOPE; the docket's unscoped wording FAILS 11/14** | 14 X menus; **3 kill-priced, all 3 marked, marked row taken 2 of 3**; both marker strings verbatim; both collapsed-run marks on the BOTTOM row with the kill lists restated. `X = 0` no-op marker 3 renders / 0 takes. Takes of a `kills THEIRS: none` row while another affordable row named an opponent creature **0 of 7**. Takes of the TOP of a collapsed run **0** (fourth corpus, first clean). The 11 unmarked are the monotone Revelation family — **D7**, and skill #260 |
| **D7** the both-player `target=player` verdict | C | **SHIPPED — PASS 0 / 151** | every Devour Flesh row prints both halves; `targeting yourself` on **151 windows**; the commit clause `{this row does not pick a target yet - taking it asks you next which of the N legal targets above}` on **217 windows / 219 rows**, and the wave-54 model's 900-word prose question is answered on the row. 6 takes. **Sibling still open:** `target=anytarget` damage rows (Spark Spray, Hammer of Bogardan) still enumerate both players and price one — engine seat **D-4**, 29 rows |
| **D10** the `[NAMED BY THEIR` price | C | **SHIPPED — UNTESTED (N = 0)** | `[named: <card>]` tags render on **83 windows** (byte-identical, as two seats asked); `[NAMED BY THEIR` renders **0** — in no window did a seat hold a `Cast <that card>` row |
| **D15** the `{kills:}` ownership split | C | **SHIPPED — PASS** | `{kills whichever you target: THEIRS - ...; YOURS - ...}` **215 windows / 223 rows**, `{removes whichever ...}` 19; the wave-54 `130v146` s23 repro renders correctly; all-opponent lists byte-identical as promised. Residual: the conditional form — **D22** |
| **D16** ability-row life totals | C | **SHIPPED, AND EVERY NUMBER IS FALSE** | the arithmetic renders on 100% of the rows and **238 of 248 `they would be at K` values are computed from a constant 166** — **D2 of this docket**. Two seats scored this PASS on presence |
| **D17** fetch-row colours | C | **STRUCK from the docket and REGRESSION-PINNED** | lane C re-derived wave 54 on disk (24/24 already carried the clause; the docket's "21 colourless" was wave 53's number restated). This corpus: 60 rows carry `and it adds {W} or {B} (you cannot make {W} right now)`, 7 takes |
| **D8** the 2-D (source × target) collapse | D 8f9ee5736 | **SHIPPED — PASS** | decode line `12-15. The same 4 options as 8-11, with <source> #2 as the source instead of #1 ...` on **123 windows / 137 rows**; **0 records with more than 3 rendered rows of one `#N` shape**; largest rendered menu 21 rows. (The wave-54 "403 redundant rows / 160,803 chars" was an `options_text` artifact — lane D's own correction, skill #240) |
| **D18** the copy-tag collapse | D | **SHIPPED — PASS, and it was the bigger item** | `(copies k-m of n ...)` on **123 windows / 200 occurrences**, reaching reveal AND cleanup-discard menus; the wave-54 50-row uncollapsed Tutor menu renders as ranged rows inside a 9 KB prompt. Residual identical rendered rows corpus-wide **6 in 4 records** (target <= 12). The 7,000+ `(copy N of N in your hand)` literals are the hand DISAMBIGUATOR and are a KEEP |
| **D22** the `chooseaname` header | D | **SHIPPED — PASS** | old `Choose one mode for` **0**; the new card-NAME header on **5 windows**; `{visible now: ...}` on **5 windows / 17 rows**, public zones only, no hidden-zone leak; narration `You named X with ...` present. N is 5, not 9 — the card came up less often. Residual: 20 of 98 narration lines use a generic source label — **D21** |
| **D9** the discard verdicts | D | **SHIPPED — PARTIAL; the land half PASSES and 84.8% of rows are still bare** | `{spare:}` 44 rows / 25 windows, `{dead right now:}` 31 / 25 (targeted-removal family only, **never a counterspell** against 114 counterspell rows), `{you already control one}` 5 / 3. **Cleanup discards sending a counterspell past a listed spare land 3 of 4 -> 0.** Rendered rows 525, bare **445** against a predicted 0 — **D5** |
| **D5(a)** reveal force-close + `reveal_stall` | E d56efcdaf | **SPLIT: livelock PASS 0; force-close UNTESTED (N = 0); the STAMP is a defect** | `stale_livelock` **0** (was 1); `FORCE-CLOSING` 0 with no reveal parked; the triple present on **12 of 12** reveal records with `_secs` == `latency_ms/1000` and `_phase` constant 0 — **D12** |
| **D5(b)** the per-tick prompt instability | E | **DIAGNOSED, NOT FIXED — and the A/B verdict is "none of the three flags"** | lane E's drift dump put the cause outside `WAGIC_GPT_AUDIT_M_OFF`, `WAGIC_GPT_BOARDINDEX` and `WAGIC_GPT_SCRIPTMEMO`: it is a land-drop/cast SEAM alternation. Confirmed in this corpus's residual — **D18**. Lane E also found the **`WAGIC_GPT_BOARDINDEX=0` segfault** — **D17** |
| **D23** the wall-miss record | E | **SHIPPED — PASS 3/3** | 3 `no reply after 900s` events: 2 wrote `kind: wall_miss` records (`162v146` s15, `130v125` s25, both `latency_ms` ~900,03x, `choice: -1`, `fallback: wall_miss_unrecorded`, `recovery` at seq+1) and 1 landed as `wall_miss: 1` on the consuming record (`126v162` s24). `gameend` carries `wall_miss_events` (3) and `wall_miss_unrecorded` (2) on **42/42**. Wave-54 D23 CLOSED |
| **D12** the mulligan colour-clause scope | E | **SHIPPED — PASS 0 of 2, thin N** | 3 clause windows: 1 at ONE land -> `... alone on turn one` (KEPT); 2 at FOUR and TWO lands -> `... alone even with every land in this hand in play` (both KEPT). **The four-corpus colour-family mulligan break did NOT recur.** The count family is byte-identical (4 renders) |
| **Q** autotap search / option cap | Q (in base) | **UNTESTED** | `WAGIC_OPT_SOURCES_CAP` / `WAGIC_OPT_WORK_BUDGET` not exercised; `- Paid` receipts render throughout with **0 `with no source tapped`** in all 21 stderr; the fit clause `{leaves N sources - no other row on this menu needs more than N}` renders and is taken. No anomaly attributable |
| **P** purge | P (in base) | **no drift attributable** | no menu a reviewer read was truncated; 0 crashes |
| **G** ability ownership | 6dee5d2cd | **not observable; UNTESTED** | 0 crashes, 0 ASAN, 0 segfaults, 21/21 natural, no dangling-ability trace |
| **H** ability epoch gate | e078c2501 | **not observable from a corpus** | `{right now:}` renders 1,415×; every sampled verdict matched its own enumeration |
| **I** ManaCost / garbage / cleanup | 85c703453 | **no regression visible; RSS untested** | `In GetCost Seems ManaCost was not properly initialized` **532**, unchanged in character (pre-existing noise) |
| **J** render caches / resource memo | fbd46fa04 | **IMPROVED — 1,725 lines / 37 resources** (was 1,927 / 118) | the image-cycling loop is GONE; every residual miss is audio + `thumbnail`, i.e. the silent `JSfx` stub — **D23**, LOW |
| **K** Baka `selectAbility` | afd88acd0 | **effectively UNTESTED (n = 2)** | Baka executed 2 decisions, both ordinary casts, no planner anomaly |
| **L** transport / narration / trim | 27f867152 | **PASS on the falsifiable half; its disable flag CRASHES** | `http_error` **0**; prompts > 30 K **1**; mean prompt +13.4% on +43% decisions. **`WAGIC_GPT_BOARDINDEX=0` segfaults — D17** |
| **M** ask cache / prefill / combat cache | ba784277f | **PARTIAL — unmeasurable on its own watch item, THIRD corpus** | no cache-hit counter reaches any log (**D25**). 21 blockers records, `dropped_assignments` 0 on all, `multiblock_first_wins` 0, `blocks_last_line_taken` 0. Lane E's drift dump REFUTES the ask cache as the stale-drop cause |
| **N** Vita NPOT textures | c2f34b05a | **UNTESTED** | no desktop exposure |
| carry: 21/21 natural | | **PASS** | invariant 00 satisfied, second consecutive corpus |
| carry: fallbacks <= 0.5% | | **PASS 0.44%** | 14 / 3,171 (was 1.04%) |
| carry: Baka-executed <= 0.20% | | **PASS 0.063%** | 2 / 3,171 |
| carry: `recovery` pairing | | **PASS 7/7, 0 unpaired** | 7 `choice: -1` (5 decisions + 2 `wall_miss`), 7 `recovery`, each at seq+1 with `recovers_seq` / `_kind` / `_fallback` |
| carry: name-vs-executed mismatch <= 1/3,000 | | **PASS 0 genuine** | 2,716 parentheticals; 16 label disagreements, 15 cosmetic short-forms and 1 a method artifact; `latched_row_mismatch` 1, a TRUE positive |
| carry: `with no source tapped` 0 | | **PASS 0** | all 21 stderr |
| carry: `while ahead on LIFE` at `my_life <= opp_life` 0 | | **PASS 0 of 8** | the render falsehood stays closed, second corpus |
| carry: attackers blocker-count header | | **PASS 90 / 90** | both singular and plural forms; the coverage deck152's re-keyed attack rule depends on |
| carry: no prompt > 30,000 | | **FAIL 1** (was 2) | `152v125` seq 314 — **D35** |
| carry: per-decision inference <= 22 s | | **NOT SCORED** | **D19** — the metric is concurrency-bound, not engine-bound |
| owner batches / O1 / O2 / lanes S, T, U, V, W | | **UNTESTED** | no `may pay` prompt, no equip-cost interrupt, no evolve/counters card, no adventure/split/morph, no DFC flip, no console exposure |

# DISCHARGED — SET A: wave-54 guide edits + the boundary pass, by deck (reviewer verdicts, summarised)

| deck | verdict summary | routed |
|---|---|---|
| **123** (1-5) | P1 fetch **FAIL 1 of 7** (5 duals, 1 licensed basic, 1 break); P2 Tragic Slip **PASS**, 1 cast on a live `{kills:}` row, 0 on a dead one, third corpus; P3 `[second copy:` **PASS 0** on a thin 2-row denominator (was 20); P4 `plan_choice_conflict` **FAIL** (6 at the seat pair, and 2 leave `chosen_text` empty — **D4**); P5 declined-note **PASS both halves** (runs of 3+ at deck123 43.7% -> **27.5%**, max run 50 -> 12); P6 the life-loop attack rule **PASS 0**, `123v126` s53 answered `no attackers` on 37 creatures with the loop clause printed; P7 **SPLIT** (edicts at N=0 **PASS 0 of 171 rows**; Greaves at `0 are creatures` **FAIL 1 of 3**, fifth corpus — **D14**); P8 the life-loss loop **FAIL 26/15** — **D1**. Two dead casts in 108 all-dead menus (was 5) | **D1**, **D14**, **D15**, **D16**; skill #233/#235/#236/#238/#239/#243/#265/#266 |
| **125** (3-3) | W96 counters **PASS 8/8 at draw engines, FAIL 2 textless targets** (both at 44 life in a won game); W97 Revelation under a punisher line **PASS 0/11**, the Elixir half re-scoped as over-broad; W98 Path **PASS 0/7 and 7/7** with 2/2 target picks; W99 cleanup **PASS 0 real breaks** (was 3 of 4 — lane D's land verdicts did it); W100 **PASS 27/27**; W101 tap-out **FAIL 2, both correct and both a FIRST Staff** (skill **#256**); W102 the stall floor **PASS, wave-54 HIGH #1 CLOSED at this seat** (367 releases -> 0). The `vs162` loss is one decision the guide had no rule for; the `vs146` loss is **D6** | **D5**, **D6**, **D7**; skill #233/#240/#255/#256/#257 |
| **126** (2-4) | D100 entry 1 **PASS 9/9 windows**, and the one wrong answer is a missing TIEBREAK (skill **#241**); D101 `[named:` **PASS 0/2**, denominator collapsed 54 -> 2; D102 **PASS 0/218 zero-Tribute** (was 0/99), **PASS 0/18 dead tutor**, **FAIL 1 of 51** `[second copy:` (a second Lantern at 3 life off a menu whose only other cast row was dead — skill **#234**); D103 **PASS 0/5 blocks, 0 concede, 24/24 land drops**, `{paying this taps:` **UNTESTED** (30 rows, 0 takes). **Every wave-54 prediction with a window PASSED, second wave running** | **D13**; skill #234/#241/#242/#243/#258 |
| **130** (4-2) | X menu **3 menus / 3 markers / 2 of 3 marked rows taken** — the render half is the corpus's best result and the miss is licensed by the guide's own retired ZERO-times recital (skill **#238**); `kills THEIRS: none` **PASS 0**; TOP-of-collapsed-run **PASS 0**, fourth corpus; face damage **FAIL 2** above the floor (was 1); land plan **PASS both clauses** (2 of 19 at 5+ lands, 0 inside the damage in hand); cycling **FAIL 4 of 11 — the only rung that lost ground**, all four at the proxy's boundary (skill **#237**); draw punishers **PASS 0 of 10**, fifth corpus; Talisman life **PASS**, fifth corpus; legend rule **FAIL 1 of 1**, a NEW rung the decklist × primitive pass would have pre-empted (skill **#247**); **3 mulligans, lifting #132-UNTESTABLE at this seat** | **D2**, **D15**; skill #237/#244/#245/#246/#247 |
| **146** (3-3) | P1 mulligan **PASS 0 of 6 at 2+ lands**, third corpus, and the cross-guide control for lane E's D12; P2 cleanup **FAIL 2 of 7 for a second corpus**, and the shape is list-position anchoring — 4 of 7 answers begin `PUT: 1` (skill **#248**); P3 **UNTESTED** (no Pelakka window); `[DRAW PRICE:` **UNTESTED** (1 render, 0 casts). The decisive loss is an attack under `[doesn't untap]` (**D40**/R240) plus a Soul Shatter tie clause read as a board wipe (**D9**) | **D8**, **D9**; skill #248/#249/#238 |
| **152** (3-3) | P4 attack tags **PASS 0/N both halves**; **P5 mulligan PASSES for the first time in five corpora** — the colour family rendered twice, both at deck146 seats, both KEEPS, and deck152's one mulligan is the 1-land COUNT family, so lane E's D12 rewording did what it was for; P6 blocks **PASS 0/6**, `all_assignments_illegal` 0. The opened lane is a MATCHUP one with an 8-event repro (**D10**) and a one-window trust lane (skill **#250**) | **D10**, **D11**, **D12**; skill #250/#251/#252 |
| **162** (5-1) | P7 **FAIL 3** (was 10) — the lane is down 70% and the residual is a THIRD paraphrase of a banned plan sentence (skill **#254**); P8 first-Mine carve-out **PASS 0/4 and 0/11 second copies, second corpus — CLOSES**; the opened lane 162-B is rule STALENESS with the discriminator on the same screen, and **two of its three "violations" were the seat's best engine casts of the corpus** (skill **#253**). `162v123` s30 is the corpus's best positive: `ATTACK: none` reasoned entirely off two rendered lines, and won on the opponent's draw step | **D24**, **D27**; skill #253/#254 |
| **pool** | wave-54 boundary B1-B10: **B3 (deck123's ahead-on-LIFE derivation) HONOURED — cut**; **B4 (deck152's count-the-distinct-names derivation) HONOURED — cut**; **B5 (deck130's ZERO-times recital) HONOURED — retired**, and replaced by a new count-as-render-fact of the same shape (B4 below); **B2 (deck125's dead HOLD quotation) NOT HONOURED — it is dead again, third wave** (B1 below); B9/B10 sizes measured below. **Trust doctrine CLEAN on all seven** | skill #238/#264 |

---

# FALLBACK CLASSES — 14 / 3,171 = 0.44% on decisions; Baka-executed 2 = 0.063%

| n | class | records | disposition |
|---|---|---|---|
| 9 | `plan_choice_conflict` | `146v152` 17, `123v152` 56, `152v123` 77, `123v126` 42 / 69, `123v146` 16, `130v123` 109 / 122, `162v126` 33 | all 9 co-stamped `decision_reversed_in_prose`; **0 executed the HOLD row** (lane A's D2b collision stays closed). 4 `_recovered`, 5 `_exhausted`. **One "recovery" is a FALSE success** (`130v123` 109 -> 110, a Starstorm cycle turned into a pass) — **D3**. Two leave `chosen_text` empty — **D4**. Root cause unchanged: the pool's own mandated PLAN vocabulary — **D16** |
| 3 | `named_row_reask` | `123v126` 25, `123v130` 9, `162v125` 52 | every one an off-menu name from a stale plan; all 3 co-stamped `stale_echo_in_range`; **all 3 recovered**, each with a `recovery` record. `named_row_reask_exhausted` **0** (wave 54: 2). `162v125` s52 named a card that was neither a row nor on the battlefield — a real hallucination, refused cleanly |
| 1 | `stale_echo` | `126v146` seq 12 | `choice: -1`, `recovery` at 13; **Baka** cast Wall of Omens. The named card had left the menu one window earlier (the seat had spent to 2 mana) — correct classification |
| 1 | `unparsed_reply` | `125v152` seq 146 | latency **767.8 s**, a 900 s-adjacent degenerate reply; `choice: -1`, `recovery` at 147; **Baka** cast Augur of Autumn |
| 2 | (non-decision) `wall_miss_unrecorded` | `162v146` 15, `130v125` 25 | lane E's D23 arm working; `recovery` at seq+1 on both |
| 0 | `all_assignments_illegal`, `stale_livelock`, `empty_reply`, `timeout`, `named_row_not_offered`, `engine_answered`, `multiblock_reask`, `retracted_choice`, `commit_retracted` | | all nine at zero |
| — | not a fallback, and it is **D1** + **D18** | 43 `dropping stale async answer` | no record of any kind is written; 8 of the 43 are the hold latch's own re-open |

---

# PROMPT CHARS PER DECISION KIND — wave 54 -> wave 55 (21/21 games)

| kind | n54 | **n55** | mean | p50 | p90 | **max** | lat p50 | lat p90 | lat max |
|---|---|---|---|---|---|---|---|---|---|
| ask | 1,683 | **2,416** | 12,022 | 10,355 | 24,045 | **30,031** | 12.4 s | 49.2 s | **1,391.2 s** |
| priority | 332 | **555** | 13,088 | 11,829 | 22,636 | 29,808 | 16.1 s | 63.8 s | 523.1 s |
| attackers | 99 | **90** | 11,546 | 9,986 | 22,215 | 27,352 | 24.6 s | 67.8 s | 285.9 s |
| discard | 34 | **68** | 12,319 | 10,919 | 21,062 | 28,689 | 17.8 s | 55.6 s | 263.8 s |
| blockers | 36 | **21** | 10,562 | 8,972 | 17,798 | 21,973 | 24.6 s | 99.2 s | 224.4 s |
| reveal | 20 | **12** | 14,790 | 13,373 | 25,212 | 27,813 | 46.7 s | 210.5 s | 474.0 s |
| bottom | 8 | **9** | 2,197 | 2,219 | 2,302 | 2,302 | 78.3 s | 91.6 s | 91.6 s |

Total prompt volume **38.60 M** chars over 3,171 decisions (wave 54: 23.76 M / 2,212); mean prompt
10,740 -> **12,174 (+13.4%)**; **1 prompt over 30,000** (was 2), 252 over 25,000 (was 37).
**GAME LOG share 58.1%** of prompts carrying a CURRENT SITUATION marker (was 56.1%), 82.4% on the
late-game specimens. **Latency is negatively correlated with prompt size inside this corpus and
positively correlated with concurrency across games (r = +0.82) — D19.**

---

# LATEGAME SPECIMENS — invariant 0 (for the owner's per-wave prompt review)

**RECOMMENDED PRIMARY, already written to `wave55/lategame-specimen.txt`:**
`1788439453-ai_baka_deck125-0x5636d58a0090-vs-ai_baka_deck152.jsonl` seq **69** (`125v152`), turn 42,
Main phase 1, **21,279 chars, 6 rows, 5.3 s latency, 22 life vs 33**, log share 75.8%. It carries a
Sphinx's Revelation `{X pricing: max affordable X=3 ...}` ladder (D7 in situ, and the one X value the
guide's rung and the render agree on), three distinct `{leaves N sources ...}` fit forms plus
`{taps you out}`, `{right now: ...}` verdicts, **`[you declined this exact list 10 times already this
turn]`**, and the HOLD row last on the menu with its benefit clause — and the answer is
**`Cast nothing right now`**. **That poses the owner's criterion as one question: 21 KB and every
render clause we ship bought a decline at turn 42, on the tenth identical look at the same list.** Its
neighbour seq 71 is the same board one phase later, which makes the repetition cost legible on the
same page. It is also this wave's headline (D1) in a single screen.

**RECOMMENDED SECONDARY, relabelled as the log-cost exhibit and nothing else**
(`wave55/lategame-specimen-logcost.txt`): `125v152` seq **190**, turn 60, Main phase 2, **27,249
chars, 4 rows, 3.6 s**, log share **82.4%**, answer `Cast nothing right now`. Four real cast rows in
front of sixty turns of log.

**seat-125-126's `125v152` seq 218 is DECLINED as a substitute and recorded as an alternate.** It is
turn 63, Upkeep, `priority`, **29,808 chars, 5 rows** — 2.5 KB larger than the staged secondary and
carrying no clause the staged pair does not already show, so regenerating for it buys size and no new
question. **seat-146-152-162's `152v125` seqs 289-316** (28 records, turns 64-65, the corpus's largest
prompt at seq 314, the corpus's only `answer_replaced` at seq 313, eleven consecutive
`Cast nothing right now` on a menu carrying the HOLD row) is **DECLINED for invariant 0 and PROMOTED
for the lanes**: it is the best available exhibit for D1 and D11, and invariant 0 asks its question of
ONE screen. **The harvest default (`152v125` seq 309) was correctly replaced** — its 17 rows are bare
mana-source lines and it exhibits nothing this wave shipped. **If the owner wants one file, ship the
primary.**

**Defect specimens (for the lanes, NOT for the prompt review):**
`123v126` seq **75-87** and `123v162` seq **36-50** (D1, the two mandatory life-loss loops, 13 and 15
windows, the HOLD row taken five times inside the first and honoured none) ·
`130v152` seq **6** (D2, `they would be at 164` at opponent life 20) ·
`130v123` seq **109 -> 110** (D3, the re-ask that turned a cast into a pass and logged it as a
recovery) · `125v146` seq **32, 35-41** (D6, seven windows at 11 power against 10 life with no total) ·
`123v125` seq **48** (D5, three dead cards unannotated on one discard menu) ·
`146v130` seq **22/23/24** (D8, three land-drop-less turns at 2 lands with an Emeria's Call in hand) ·
`146v123` seq **24 / 28** (D9 + R240, the `[doesn't untap]` attack and the Soul Shatter tie clause) ·
`123v152` seq **104** (D15, two Damnation rows and the dearer one taken) ·
`152v125` turns 25-51 (D10, eight creature spells countered) ·
`162v123` seq **30** (the POSITIVE control — `ATTACK: none` reasoned off two render lines, won the
game on the opponent's draw step).
