# Wave-77 deck126 review — SANGUINE BLOOD (WGB wall prison, Exquisite Blood + Sanguine Bond)

Corpus `matchups-20260911-010157-final`. SINGLE binary `wagic-42c41fc66-w77step1`. **1/6** (wave 76:
3/6; wave 75: 2/6; 74: 2/6). Five of the six are reruns (epochs 1789114824-1789114889); **`126v162`
is the corpus's ONE first-run game** (epoch 1789106561) and it finished naturally before the pilot
wedge — it carries a gameend and 0 timeout taint like the rest.

**208 records, 190 with a prompt**, per game (162/125/123/130/146/152): 24/82/24/34/30/14.
**0 fallbacks at this seat** — 0 `unparsed_reply`, 0 `noop_row_zero_reask`, 0 `repeat_count_reask`,
0 `defer`, 0 `recovery`, 0 `reply_truncated`, 0 `action_before_plan`, **0 `protocol_deviation_replies`
and 0 `plan_line_missing` in 190 replies** (wave 76: 1 and 1). `thinking: on` and a non-empty
`reasoning` on every record (invariant 000): median 5,180-6,046 chars per game, max 22,703.
Max prompt **22,273 B** (`146` seq 27), 7 of 190 over 20 KB. `async_drops` 1 (123) + 1 (152).
The corpus's 5 `unparsed_reply` are all on deck125's seat; one of those files is deck125's seat in
**my** `125v126` game — not this seat, so I quote none (the engine seat owns them).

Counters, per game (162/125/123/130/146/152): `hold_windows_skipped` 2/505/33/0/0/0, **all cast
seam, 0 priority**; `hold_released_turn` 0/15/0/0/0/0; `ask_replays_reserved` 5/46/0/11/0/0;
`identical_ask_answers_reserved` 5/36/0/11/0/0; `crossphase_identical_reputs` 2/5/0/0/0/0 with
`crossphase_board_unchanged` **0 in all six**; `own_loop_windows_asked` 0/0/0/**1**/0/0;
`own_turn_windows_skipped` 0/0/0/0/38/48; `mana_only_windows_skipped` 6/8/23/16/5/11;
`main_phase_windows_skipped` 0 in all six; `plan_names_uncastable_zone_card` 1/1/0/4/3/0 (9);
`plan_names_stranded_card` 1/0/1/1/0/0; `phase2_answer_recovered` 0/1/0/1/1/0, `phase2_answer_missing`
**1 (162)**; `forced_close_unrecorded` 0/0/0/0/0/**1 (152)**; `chain_selfharm_rows_cast` 63 (125);
`blocker_forecast_*`, `force_close_arms_refused/_deferred/_same_arm_deferred/_defer_bound_hits`,
`stop_reached_windows_skipped`, `repeat_annotated_takes`, `identical_option_asks_resolved`,
`declined_face_latches`, `menu_pass_no_progress`, `wall_miss_events` — **0 in all six**.

**R1's storm is gone.** Max asks in one (turn, phase) at this seat: **4** (`123` t13 M1, `130` t15 M2,
`130` t17 M1, `146` t1 Untap). Wave 76 measured **35** in one (`123` t17 M1). The `[you declined this
exact list N times already this turn]` note reaches **N=1** anywhere in this corpus (wave 76: 31).

## 1. Game by game

| # | opp | result | deciding decisions |
|---|-----|--------|--------------------|
| 1 | **162** (first-run) | **LOST** 0 to 14, t12 | Double Tutor (t5 seq8 -> Exquisite Blood... then the Blood was never castable). Their line was **Underworld Dreams + Ob Nixilis + Howling Mine**: `DRAW FORECAST` at seq 20 reads `your next draw step draws 2 cards (1 + Howling Mine 1) = 2 x 2 = 4 life LOST BY YOU`, from 5 life. Sanguine Bond landed t9 (seq16); Exquisite Blood {4}{b} was never payable (seq 20: `your untapped sources: 3`). seq 18 (t11, life 3) cast Tribute for 2 and took them 20 -> 14 through the Bond — the only damage the seat dealt. seq 20 declined at 5 life under an unblockable 5-power flier: correct, every line is dead (their attack alone is exactly lethal, and the compulsory draw is lethal without it). No decision at fault; the deck lost the draw-punisher race. |
| 2 | **125** | **LOST** 0 to 48, t41 | **Pure variance, verified**: in 41 turns and ~40 draws the seat **never saw a Sanguine Bond or an Idyllic Tutor** — `Cast Sanguine Bond` and `Cast Idyllic Tutor` appear on **zero** menus in the whole game. It drew two Exquisite Bloods, cast one t14 (seq22) and the redundant copy t24 (seq51, §2 MED-3). `Cast Staff of Nin` (seq19, t12) was **countered** — the Staff never reaches a battlefield line. 505 cast-seam windows skipped by holds; the final window (seq 126, life 1) printed `{answers the stack: NO - 1 damage is ALREADY ON THE STACK ... that puts you at 0 and KILLS you}` on the hold row, with a single dead Tribute (`at 0 this does nothing`) as the only alternative. Correct and unavoidable. |
| 3 | **123** | **LOST** -1 to 18, t14 | **Opponent combo, not a misplay.** At seq 16 (t11) the crack-back is `2 from 1 of their creatures`; at seq 17 (t13) it is **`32 from 31 of their creatures`**. The deck123 seat (its seq 34-39) cast **Intruder Alarm** and tapped Thraben Doomsayer for 31 Human tokens in one turn. Both halves were in hand from t7 but neither was payable before t13 (seq 14 t11: 4 sources, neither half on the menu; seq 17 t13: 6 sources, `row 2 needs more mana sources than the 2 this leaves` — one half only). seq 17 took Sanguine Bond, which is entry 1's tie-break by the letter. seq 21 blocked 3 of 31. Nothing was available. |
| 4 | **130** | **WON** 17 to -1, t17 | The clean line: Tutor t5 -> Sanguine Bond, Sorin t9 (seq12) and **+1 every window it printed** (seqs 13/16/19/39/43), Bond t11 (seq15), Tutor t15 -> Exquisite Blood (seq 37/38), **Blood t17 (seq40)**, then **seq 41 Tribute on Rorix Bladewing (6/5)** — the row itself read `is sacrificed, you gain 5 - and both halves of YOUR life LOOP are in play, so that chains without limit until they are at 0: THIS WINS THE GAME`, taken in the first window it printed. 14 -> -1 in three windows (seq 41/42/43), the whole loop closed in **3 asks**. This is the win button's first bullet and rung 1, both by the letter. |
| 5 | **146** | **LOST** -3 to 21, t21 | Removal deck: Exquisite Blood cast t14 (seq20) is off the battlefield line by t18; Sanguine Bond re-tutored (seq22/23) and cast t18 (seq25); the pair is never complete. **seq 26 (t19) is the misplay and it is the guide's — see §3.** seq 27 (t20, life 4) then cast Perimeter Captain off rung 0 correctly, and seq 28 (t21) blocked under `this KILLS you`, gaining 2 — the KILLS-you paragraph working. |
| 6 | **152** | **LOST** -4 to 20, t13 | Curve-out on an empty board: the seat controlled **0 creatures from turn 1 to death**. seq 12 (t12, life 5) cast Exquisite Blood into a stated `9 from 4 of their creatures ... you would be at -4; that would KILL you`. It was dead under every line — the Wall of Omens row's own cover clause prices the alternative exactly: `Counting only the checked bodies you cover 2 of 9, leaving 7 -> you would be at -2, which still KILLS you`. So the answer cost nothing. What it cost is **~4,000 characters of reasoning oscillating between rung 0 and entry 1** (§2 MED-4). |

**1/6, down from 3/6.** Four of the five losses have no decision at fault (162 draw-punisher race,
125 a 41-turn flood that never offered a Bond or a Tutor, 123 a 31-token turn, 152 a dead board).
The fifth, 146, has one. Play quality this wave: **land drops 43 of 43 (100%)**; **66 `at 0 this
does nothing` exposures, 0 takes**; **hold row taken on 43 of 55** dead/repeat menus (wave 76: 16 of
75 — the hold-row rule is finally landing); 7 reveal picks + 1 bottoming window; 0 fallbacks; 0 protocol deviations.

## 2. Engine / interface / card items

**HIGH-1 — R1'S MARKER ROW `[own loop verdict: ...]` NEVER RENDERS, IN THIS CORPUS OR ANY OTHER: IT
IS A HOLD-KEY SENTINEL, NOT A ROW.** Repro `1789114844-ai_baka_deck126-0x55b08ca40bc0-vs-ai_baka_
deck130.jsonl` **seq 43** (t17 Main 1) is the corpus's ONE `own_loop_windows_asked` window. A
grep for `own loop verdict` over **all 42 seat logs** returns **0**. What printed is the per-ROW
tag — every acting row carries
`[your loop is resolving on its own - this row is not needed to win it; the HOLD row on this menu
covers every link of it until the loop ends or something else changes]` (1 occurrence corpus-wide,
this record). `AIPlayerGPT.cpp:32055` pushes `w77OwnLoopVerdictNow()` into `rowsWithVerdict` and
`:32144` into the hold key set — it is never emitted into a menu, so CU F4's three-way verdict
IDLE/RESOLVING/THREATENED is **unobservable from the prompt and from the record**. The verdict here
was TRUE (both halves on the seat's line, an Exquisite Blood trigger on the stack, life 2 -> 7
between seq 42 and seq 43, no removal of theirs anywhere). **The seat did NOT hold**: it answered
`CHOICE: 1 (+1: create a 1/1 vampire ...)` on a row the annotation had just called unnecessary, and
the game ended on that window. Cost nothing here because the loop was already lethal; but R1's
mechanism — the hold that makes one answer serve every link — went untaken at its only test, and
there is no surface a reviewer or the model can read to tell RESOLVING from THREATENED.
Falsification: any window with `own_loop_windows_asked` incrementing and a THREATENED board.

**HIGH-2 — `hold_check_ref_seq` IS ON A DIFFERENT AXIS FROM `seq` AND CANNOT BE USED AS THE BRIEF
ASKS.** Every asked record carries both `seq` and `hold_check_ref_seq`, and they are not
commensurable: `126v162` seq 4 carries `ref 1` where the previous land-seam record is seq 3;
`126v125` seq 121 (land) carries `ref 59` where the previous land window is seq 85. The referent is
`mHoldMemory.measuredRef[seam]`, stamped from `mWindowSeq` (`AIPlayerGPT.cpp:30906`) — a per-seam
WINDOW counter that includes windows the hold skipped and the translog never records. Two adjacent
fields named `seq` and `..._ref_seq` in one JSON object that index different sequences is a trap the
audit walked straight into; I had to discard 130 apparent "mismatches" as my own error. Fix shape:
name it for what it is (`hold_check_ref_window`) **or** emit the referent's record `seq` when the
referent was recorded and `-1` when it was skipped.

**MED-3 — THE `[second copy:` ANNOTATION NAMES ITS OWN COST WITH A WORD THE MODEL READS AS A
BENEFIT, AND THE MODEL QUOTED IT BACK.** Repro `1789114824-...-vs-ai_baka_deck125.jsonl` **seq 51**
(t24). Row 2 reads
`Cast Exquisite Blood {4}{b} [second copy: you already control Exquisite Blood; both stay on the
battlefield - no legend rule, and this copy is one more of the same effect - each line it repeats
happens again; the price is a card and this window's cast spent DOUBLING an effect you already have
on the battlefield, not adding one you do not]`. The reply: `PLAN: Cast Exquisite Blood to double
the life gain trigger, maximizing future Sanguine Bond damage once I find the second half of the
loop.` **Six windows earlier the same seat read the same clause correctly** — seq 45 (t22), same row
byte-for-byte: `casting a second Exquisite Blood is redundant, so I will hold priority`. The only
text that moved between them is the `{leaves N}` figure. `DOUBLING` and `each line it repeats
happens again` are the two upside-shaped phrases in a clause whose whole job is to price a waste.
Fix shape: state the consequence, not the mechanism — `this copy changes no number on the board` —
and drop `DOUBLING`. Cost here: one card and 5 mana in a game already lost; the shape is the danger.

**MED-4 — RUNG 0 AND ENTRY 1 ARE BOTH ABSOLUTE AND THE PROMPT ALREADY HOLDS THE TIE-BREAK THE MODEL
SPENT 4,000 CHARACTERS FAILING TO INVENT.** Repro `1789114889-...-vs-ai_baka_deck152.jsonl`
**seq 12** (t12, life 5). The reasoning asks the same question five separate times —
`Does Wall of Omens count as Entry 0? "any row that removes an attacker or adds a blocker".
Wall of Omens adds a blocker. So maybe it is Entry 0?` — and finally resolves it with a gloss it
made up: `the guide's Entry 0 is for when the blocker actually stops lethal ... Here it doesn't.`
**That invented gloss is exactly what row 2's own `{crack-back cover:}` clause states in numbers**:
`Counting only the checked bodies you cover 2 of 9, leaving 7 -> you would be at -2, which still
KILLS you`. The model never cited it. This is a prompt-ordering item, not a falsehood: the cover
clause that settles rung 0 is ~600 B inside row 2's brace group, while the rung-0 trigger
(`CRACK-BACK NEXT TURN ... that would KILL you`) is a board-block header 40 lines above, and nothing
joins them. A survival-floor reader has to find both. Fix shape: the CRACK-BACK header names the
best cover any single row on this menu buys (`the best any one row below buys is -2, which still
kills you`), so the header that raises the question also answers it.

**MED-5 — `phase2_answer_missing` HAS NO PER-RECORD LOCATOR, SO THE ONE INSTANCE CANNOT BE QUOTED
FROM THE TRANSLOG.** `126v162` gameend carries `phase2_answer_missing: 1`; no record in that file
carries the key (it exists only on the gameend object), and the seat log has a seq gap 21-25 with no
annotation on either side. `phase2_answer_recovered` (1 in each of 125/130/146) is in the same
shape. The counters are real but unreproducible from the seat log alone — every other wave-77
counter got a per-record field (`hold_check_ref_seq`, `plan_names_uncastable_zone_card`,
`off_protocol_bytes`); these two did not.

**LOW-6 — THE `{effect:}` GLOSS (R5) CUTS TWO OF 26 MID-CLAUSE, AND ONE OF THE TWO DROPS A
DURATION.** 26 distinct creature/permanent glosses rendered at this seat, sampled all 26 against
their `text=`. Twenty-four are complete. Two truncate: `When this creature enters or transforms into
Brutal Cathar, exile target creature an opponent controls until this...` (`146` battlefield block)
and `When Elite Spellbinder enters, look at target opponent's hand. You may exile a nonland card
from it. For as long as that...` (`152`). The Brutal Cathar cut is the one that matters: it stops
one word before `until this creature leaves the battlefield`, so a temporary exile reads as
permanent. The trailing `until this...` does signal the cut, which is why this is LOW and not MED.
No gloss on this seat asserts a false fact outright.

**LOW-7 — THE HOLD ROW OVERRAN THE 650 B BUDGET: 726 B.** `1789114824-...-vs-ai_baka_deck125.jsonl`
**seq 126** (t41 Upkeep). The base cast-seam hold row plus
`{a hold taken in your first main phase also covers your second main phase while these rows do not
change}` plus `{answers the stack: NO - 1 damage is ALREADY ON THE STACK, and taking this row lets
it resolve: that puts you at 0 and KILLS you}` = 726 B on one row. Wave 76's max at this seat was
593 B under the same 650 B budget (P19). Both trailing groups are true and the second is the most
useful sentence on the menu; the budget is what needs restating, not the clauses.

**Wave-76 items, re-checked on this corpus.**
- **HIGH-1 (32 consecutive decodes on a won loop) — FIXED, verified.** Max asks in one (turn, phase)
  at this seat **4**, against 35. The one own-loop window is HIGH-1 above. The `130` win closed a
  complete loop from 14 to -1 in **three** windows.
- **HIGH-2 (blockers `AT BEST` floor ignoring the metered blocking gain) — FIXED, verified.** R4's
  fold prints on every header where a gain is metered: `146` seq 16
  (`20 if you also take every OPTIONAL gain the BLOCKING THIS COMBAT line below meters (4 more;
  those are "may" gains and yours to take)`), seq 19 (`13 AT BEST`), seq 21 (`14 AT BEST`), and
  **seq 28 — the wave-76 repro's exact shape** (`you would be at -5 AT BEST (no assignment of your
  blockers does better), -3 AT BEST if you also take every OPTIONAL gain ... (2 more)`), whose
  BLOCKING line also carries the converter half (`and your converter takes that much off them - up
  to 2 off their 23`). CU F7's prohibition gate reads TRUE at the two headers where no fold appears
  (`146` seq 24 and seq 26): the only blocker there is a **Wall of Omens**, which has no gain
  trigger, and no `BLOCKING THIS COMBAT` line prints at all. **0 false folds in 6 exposures.**
- **MED-3 (hold-check bracket false on 20 of 106 cast-seam windows) — FIXED, verified, and the new
  `"land"` seam is clean too.** Method: rows extracted from the menu, balanced `{...}`/`[...]`
  groups stripped, hold/decline rows excluded (the engine treats a pass row differing only by which
  step comes next as the same row), seam taken from the menu header with `Which action do you take?`
  tested BEFORE `Casting decision (` — the latter also appears as prose inside priority prompts.
  **162 brackets: land 43, cast 112, priority 7. 0 FALSE.** The 13 `first window` sentences all sit
  at a seam with no asked predecessor. Wave 76's 20/106 is closed at this seat, and the priority
  seam — unadjudicable last wave — is adjudicated and clean.
- **MED-4 (forced-sacrifice gain clause missing the OWN-converter note) — UNTESTED.** No forced
  sacrifice of theirs reached this seat in six games; the one sacrifice window is `146` seq 14,
  the seat's OWN Overgrown Battlement, with no converter out.
- **MED-5 (`plan_names_uncastable_zone_card` third exemption) — REFUTED, do not add it.** 9 flags at
  this seat, all real under R9's cast-intent restriction, and they split two ways. Four are the
  wave-76 family, a tutor target named in a search plan (`130` seqs 13/14/16/17, `Idyllic Tutor`
  itself, which is in the library). **Three are not**: `146` seqs 9, 10, 11 name `Sanguine Bond` as
  the plan's NEXT CAST with no search step anywhere in the sentence — `PLAN: Play Bayou, then cast
  Sanguine Bond to start the combo loop` with a four-card hand holding Overgrown Battlement,
  Perimeter Captain, Savannah and Bayou. That is the misconception the flag exists to catch, three
  windows running. Wave 76's proposed exemption (a card named as the object of a search step in the
  same sentence) would still have flagged all three — it is the right shape and it would not have
  blinded the flag here; an exemption for "any future-turn cast" would have.
- **LOW-6 (the hold row is the answer the seat will not give) — LARGELY FIXED.** 55 menus printed
  `NO LIVE CAST ROW ON THIS MENU` or `[you declined this exact list N times already this turn]`;
  the seat answered hold **43**, `Cast nothing right now` **9**, a live row 3. Wave 76: 16 hold, 23
  `Cast nothing`. No edit needed and none made.
- **LOW-7 (Q13 board clause) — `crossphase_identical_reputs` 7 at this seat (162 x2, 125 x5), and
  `crossphase_board_unchanged` 0 in all six.** R8's positive clause never fired here; all 7 boards
  had genuinely moved, which is the intended reading.
- **LOW-8 (own-clock tag horizon) — UNTESTED.** No own-clock tag printed beside a loop on the stack
  at this seat; the Staff of Nin never reached a battlefield line in any of the six games.
- **Q5 `{crack-back cover:}` — 23 exposures, arithmetic TRUE on all 23** (spot-checked `152` seq 12
  `cover 2 of 9, leaving 7 -> you would be at -2` from life 5; `130` seqs 40/41 `6 - 6 = 0 is the
  MOST that can be left -> you could still be as low as 2, which you SURVIVE` from life 2;
  `146` seq 27 `6 from 4 ... puts you at -2 OR LOWER` from life 4). The edict floor (R3) reads both
  branches correctly — `162` seqs 9-18 print `1 of their 2 creatures is NOT in that total, so they
  can hand over a body that was not going to attack`, `123` seq 17 and `130` seq 40 print the
  all-in-the-total branch. **CU F3 (regenerating/returning bodies not subtracted) is UNTESTED**: no
  such body appeared. The legality filter (R7) prints and is used: `152` seq 12
  `1 of their 4 could not legally be blocked by it at all`.
- **Q2 prompt size** — max **22,273 B**, 7 of 190 over 20 KB (wave 76: 24,039 B, 22 of 266). CT's
  38,657 B bound is far away at this seat.

## 3. Guide verdict: **EDIT** (`strategy.txt`, **19,901 B**, DOWN 89 B from the live 19,990)

One rule, rewritten in three places because the rule's condition appears in three. **The guide does
not grow; it shrinks by 89 B.**

**A. STEP 1's release was the wrong predicate. "An enchantment of yours on your battlefield line"
released the bench on a board where no block gained anything, and it threw away the seat's last
body. Paid by the cleanest A/B pair this corpus produced:
`1789114859-ai_baka_deck126-0x555caa005410-vs-ai_baka_deck146.jsonl` seq 24 vs seq 26.**
The two windows are the SAME board. Both print three attackers —
`A1. Triumphant Adventurer (1/1) deals 1 [first strike, deathtouch]`, `A2/A3. Spider #1/#2 (2/1)
deals 2 [reach, menace]` with `[NONE of your available blockers can block this attacker]` on both —
and the SAME single blocker line, byte-for-byte:
`B1. Wall of Omens (0/4) [defender] [deals 0 - this block kills nothing, but it STOPS all 1 damage
from reaching you THIS COMBAT] - may block A1 (your blocker dies, attacker lives) {after this
combat: you control 1 fewer blocker - 1 available now, 0 if this one dies here}`. Neither window
prints a `BLOCKING THIS COMBAT` line or a `{blocking trigger:` clause — there is **no gain to be
had**, because a Wall of Omens has no trigger and no Perimeter Captain is out. Neither header is
lethal (`NOT lethal: block only where the trade favors you`, at 14 and at 8). **The only thing that
changed between them is that Sanguine Bond resolved at seq 25.** At seq 24 the seat declined:
`Preserve my only blocker for future turns; trading it for 1 life is unfavorable. BLOCKS: none`.
At seq 26 the enchantment was out, STEP 1 went off by the letter, and it blocked:
`Block Triumphant Adventurer with Wall of Omens to trade the wall for 1 life saved`. The wall died
for **1 damage stopped**. Two turns later, at seq 28 under `this KILLS you` with 4 attackers and 10
damage, the seat had **one** blocker where it would have had two, blocked once for 2 gain, and died
at **-3**. The guide's own WIN BUTTON already states the truth the rule contradicted: `Without a
Captain, a Battlement's or Wall of Omens' block gains nothing.`

The rewrite makes the printed clause the predicate, and deletes the life-total release with it (the
`this KILLS you` paragraph is the absolute one and it already covers the low-life case; the `8 or
less` branch fired at seq 26, at `you would be at 3`, and would have forced the same block under a
clause-only rule otherwise). Three sites, one rule:

before (STEP 1 bullet 1): `unless one of three is printed: an enchantment of yours on your battlefield line, the header's "you would be at N" at 8 or less, or the blocker is a lifelink Vampire.`
after: `unless one of two is printed: a "{blocking trigger: you may gain N ...}" clause on its own B-line, or the blocker is a lifelink Vampire.`

before (STEP 1 bullet 2): `ONCE AN ENCHANTMENT OF YOURS IS OUT, STEP 1 IS OFF AND EVERY BLOCKER BLOCKS, on any line. The B-line prints "{blocking trigger: you may gain 2, and if you do your converter takes 2 off them}" - that clause IS the deck's damage on a turn with no Tribute, ping or Vampire, so "BLOCKS: none" is never your answer while it shows. Same at "you would be at 8" or less. ONE LIMIT: with no enchantment out, when every attacker is already blocked by a blocker that lives, an extra "dies" blocker only feeds them a body.`
after: `THAT CLAUSE, ON THE B-LINE, IS THE WHOLE TEST - NOT AN ENCHANTMENT ON YOUR BATTLEFIELD LINE AND NOT YOUR LIFE TOTAL. Where it prints, that blocker BLOCKS on any line: the gain it meters IS the deck's damage on a turn with no Tribute, ping or Vampire, so "BLOCKS: none" is never your answer while it shows. Where it does NOT print, a wall that dies gains you nothing, so STEP 1 benches it at every life total under a header that does not read "this KILLS you" - read the row's "{after this combat: ... 0 if this one dies here}" and keep that body for the combat that is lethal. When every attacker is already blocked by a blocker that lives, an extra "dies" blocker only feeds them a body.`

before (closing paragraph): `"BLOCKS: none" is right only when EVERY blocker falls under STEP 1, and at a header of 8 or less STEP 1 benches nobody - so there it is right only when no blocker is offered.`
after: `"BLOCKS: none" is right only when EVERY blocker falls under STEP 1, and under a "this KILLS you" header STEP 1 benches nobody - so there it is right only when no blocker is offered.`

**B (the cut that pays for A). THE HOLD ROW section restated, in 250 B, a paragraph the engine
prints in full on every single menu.** Cut:
`A HOLD NEVER COSTS YOU THE WINDOW YOU ARE WAITING FOR: it ends the moment a row appears,
disappears, or names a different card, cost or target, so a Tribute reading "at 0 this does nothing"
re-opens this menu by itself the turn they have a creature.` Every casting and priority menu in all
six games carries `[HOW A HOLD ENDS: ... What re-opens the window: a row appearing, a row
disappearing, a row naming a different card, cost or target, and a change in whether the board now
kills you.]` — the same fact, on screen, at the moment of the decision. The section keeps its rule
(`It costs NOTHING and does not end your turn. TAKE IT instead of "Cast nothing right now" ...`) and
its live-row stop. **Stated risk**: the cut sentence is the reassurance, and hold-row uptake just
rose from 16/75 to 43/55; if uptake falls next wave this is the first thing to restore.

**Net bytes.** A adds 161 B across three sites, B cuts 250 B: 19,990 -> **19,901**.

**What KEPT and earned it.** RUNG 1 and THE WIN BUTTON's first bullet won game 4 outright
(`130` seq 40 cast the missing half taps-out, seq 41 took the Tribute with `THIS WINS THE GAME`
printed on the row). THE ZERO STOP: **66 exposures, 0 takes**. LAND DROP: **43 of 43** — `"Play no
land right now" is never your answer while a land is listed` now has a perfect record. THE HOLD ROW:
43 of 55. SORIN's `+1 IN EVERY WINDOW BUT ONE` (the wave-76 edit A): `-2` was printed on 7 menus
this corpus and **taken 0 times**; the wave-76 emblem misplay does not recur, and the `+1` was taken
in every window that offered it in the game the seat won. `NEVER HOLD ON A MENU THAT PRINTS [a live
activated row]`: 0 violations. ENTRY 1's `"CASTING THIS TAPS YOU OUT" IS A MANA NOTE, NOT A WARNING`
fired correctly at `130` seq 40 and defensibly at `152` seq 12. MULLIGAN + BOTTOMING: 8 mulligan windows and 1 bottoming window,
all by the letter (`146` seqs 2-5 shipped two sevens and bottomed Overgrown Battlement + Staff of
Nin off the kept six). ATTACKING: 2 attacker windows, both correct (`130` seq 20, seq 35).

**REJECTED edits.** (1) **A rule against the second Exquisite Blood (`125` seq 51).** Stated twice
already — the not-a-live-cast-row list and entry 1's `A copy of a name already there is not this
entry` — and the seat obeyed it at seq 45 on the identical row six windows earlier. What moved is
the annotation's wording, which is MED-3, an engine item. (2) **A rung-0/entry-1 tie-break for
`152` seq 12.** The seat's answer was right (every line dies), and the tie-break it needed is
already printed inside row 2 — writing a third absolute into the guide to arbitrate two existing
absolutes is how a guide gets unreadable; MED-4 names the interface fix. (3) **A rule about naming a
library card as the next cast (`146` seqs 9/10/11).** The guide already says `A hand card with NO
"Cast" row is not castable this window - never name one`; extending it to library cards costs bytes
for a plan-line defect that changed no decision (no better play existed on any of the three
windows). (4) **Anything about the draw-punisher race (`162`).** The DRAW PUNISHERS section is
already correct and the seat read it correctly; it lost to a board it could not answer. (5) **A
Staff-of-Nin upkeep rule.** The Staff reached a battlefield line in **zero** of six games.

No general-guide proposal and no skill proposal. Edit A is a deck-specific fact about which surface
carries this deck's life gain; every item in §2 belongs to the engine seat, and none of them
restates an existing amendment.

## 4. Not checked
I read no engine source except `AIPlayerGPT.cpp` around `w77OwnLoopVerdictKey` / `holdReopenNote`
(HIGH-1, HIGH-2) and built no fixture; every other mechanism sentence in §2 is a falsifiable repro,
not a claim about the code. I opened **stderr only for `126v162` and `152v126`, and only to grep for
`phase2`/force-close strings** — both greps came back empty, so I contribute nothing on
`deadline_pct`, `transport`, `askreplay/`, the stderr forced-close count, the `{X}`-announcement
payment loss, or `Parser returned NULL` noise; the one `forced_close_unrecorded` (`152`), the one
`phase2_answer_missing` (`162`) and the three `phase2_answer_recovered` are counted and
**unreconciled** (MED-5 is why). I did not verify the 2 `async_drops` (`123`, `152`) were re-asked.
I traced opponent decisions only in `123v126` seqs 31-52, to establish the 31-token turn; the other
five opponent seats I did not read, and I quote none of the corpus's 5 `unparsed_reply` because
none is on this seat. I could not adjudicate `own_turn_windows_skipped` (38 in `146`, 48 in `152`)
or `mana_only_windows_skipped` — what those skips suppressed is not visible from the translog.
`main_phase_windows_skipped` is 0 in all six here, so Q7 stays UNTESTED from this seat. CU F1's
balanced-strip re-ask test I ran only as part of the hold-check audit (0 false brackets implies no
CG-shaped re-ask was mis-keyed at this seat, but I did not count byte-identical-after-strip re-asks
separately). CU F3 (regenerating/returning bodies), CU F2 (single-target vs sweep), the ANNOUNCE_X
plan echo (R10) and the own-converter forced-sacrifice case (wave-76 MED-4) are all **UNTESTED
here**: none of those boards occurred. Card verification: I checked **Sorin, Lord of Innistrad,
Exquisite Blood, Sanguine Bond, Tribute to Hunger, Perimeter Captain, Wall of Omens, Pride Guardian,
Overgrown Battlement, Staff of Nin and Chromatic Lantern** against their rendered `card text:` in
the prompts, and **Brutal Cathar and Elite Spellbinder** against the truncated gloss only (LOW-6).
**Intruder Alarm, Thraben Doomsayer, Underworld Dreams, Ob Nixilis the Hate-Twisted, Howling Mine,
Master of the Feast, Rorix Bladewing, Shield Sphere, Essence Scatter, Elixir of Immortality,
Triumphant Adventurer, Lolth Spider Queen, Nadaar, Augur of Autumn, Dwarven Blastminer and
Pyrite Spellbomb were NOT verified** beyond what the render stated.
