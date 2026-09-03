# Wave-53 engine ledger — THE WAVE-54 DOCKET (engine / UX only)

Corpus `matchups-20260902-023342` (TENTH fair-hand corpus), binary master **c628dd531** (lanes
M 614312289, N 3ab5c27c4, O 0ba5ae2eb, P 1057c7c9d, Q ba33a9adb, R 1bb08b2b1, S 6a4d5e119,
T 00549b391; PARSETEST 2249/0; gate 1215 + 46 AI/0). Pilot **qwen36-35b-a3b** (FP8) on Spark :8084.
42 seat logs, **3,572 records**, **3,483 decisions** (ask 2,754 / priority 519 / attackers 120 /
discard 34 / blockers 30 / reveal 21 / bottom 5), 42 `gamestart`, **40 `gameend`**, **7 `recovery`**
(the NEW kind — not a decision; a seq-arithmetic count of decisions must filter it). Corpus root:
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260902-023342/`; the results file is `results.tsv`.
Seat names `deckA-vs-deckB` = the deckA seat's JSONL, written `AvB` below; `seq` is the record's own
field. Every number in this file was re-derived on disk by the synthesis step.

**INVARIANT 00: THIS CORPUS IS 20/21.** Game `152v125` HUNG for ~13 h at turn 15 and was
cap-adjudicated (`results.tsv`: `152 125 adj0 20 9 15`). **Neither seat carries a `gameend`** and it
is the only such pair in the corpus. Per the owner's binding rule a corpus completes a full game for
every matchup or the test has FAILED, and an adjudicated run is never re-labelled a result: that
matchup is recorded as a FAILED test, it is excluded from every rate in this file, and no verdict
below rests on it. **No rerun is owed for wave 53.** A rerun on `c628dd531` reproduces the hang (the
fix was not in it), and a rerun on the FIXED binary is a wave-54 datum on different code — mixing it
into wave-53 rates is exactly the relabelling the invariant forbids. The replacement datum comes
from the wave-54 corpus, which reruns all 21 matchups on the fixed binary; what is owed NOW is
D1's falsifier (no seat log without a `gameend`) and D7's no-progress watchdog, because the
harness's feasibility watchdog measures LATENCY and a hang produces none — the process sat 13 h
writing nothing and nothing fired.

**Health.** Fallbacks **15 / 3,483 = 0.43%** (wave 52: 0.45%): `plan_choice_conflict` **8**,
`unparsed_reply` 3, `named_row_reask` 3, `retracted_choice` **1** (a new class, working).
**Baka executed 4 decisions = 0.11%** (`chose -1 of N` x4; wave 52: 0.18%) — under the <= 0.20%
carry. `giving this decision to the heuristic` **0**; `stale_livelock` **0**; `consecutive stale
drops` **0**; `with no source tapped` **0**; `index_name_conflict` **0**; `plan_missing` **0**.
`parse_note` 35: `decision_reversed_in_prose` 8 (+3 in the compound
`decision_reversed_in_prose;plan_choice_conflict_exhausted`), `plan_choice_conflict_recovered` 5,
`named_row_not_offered` 4, `named_row_reask_recovered` 3, `attack_last_line_taken` 3,
`echo_index_conflict;name_over_index` 3, `multi_answer_first_taken` 2, **`multiblock_first_wins` 1**
(first live firing), `blocks_last_line_taken` 1, `echo_index_conflict` 1, `stale_echo_in_range` 1.
`answer_replaced` **12** (exactly the 12 records with `latched_coded_line: 2`), `coded_answers > 1`
**43**, `commit_retracted` **1**, `retry` **0**, `pass_row_named` **0**,
`blocker_handle_in_attacker_slot` **0**. `latched_coded_line` present on **39** records (39 of the
43 with `coded_answers >= 2`; the 4 without are `attackers`/`blockers`, which use their own parsers).
stderr: `dropping stale async answer` 63 (never consecutive), `auto-passing without a model call`
2,528, `all actions pass-declined` 809, `land-drop ask NOT issued` 977, `cast ask NOT issued` 257
over `repeat plan iteration` 60. `mana_only_windows_skipped` sum **406** (max 276);
`hold_windows_skipped` sum **102**, non-zero on **2 of 40** gameends.
Max seat log **607** (`146v125`, 84 turns). Max prompt **32,068** (`146v123` seq 92, priority, turn
33); **12 prompts > 30,000**, **588 > 25,000**; **476** carry `(…earlier events trimmed…)`.
`- Paid` receipts **496** deduped from `events`, **0 with no source tapped**, **2 with no payee**,
11 in the legitimate `from mana already floating` form. Total inference **20.96 h**.

**Mulligans (real hands).** 42 opening asks; **6 mulligan decisions across 5 seats** — four to 6
cards, one seat to 5 (`152v162`, two mulligans). 5 `bottom` asks, all 7 options.
**No mull-to-zero, and — unlike wave 52 — no mulligan decided by the heuristic** (0 timeouts).
deck130 shipped **0** mulligans for a third corpus, so its section stays #132-UNTESTED (now
UNTESTABLE-AT-THIS-SEAT per skill #200). The one repeated mulligan (`152v162` s1 at **2 lands**
against its guide's explicit two-land floor, s2 at 1 land, keep at 5) is a **guide** matter, is owned
by that deck's reviewer, was diagnosed by it (#171's missing scoping sentence) and fixed by it
(152-B, prediction P5) — **no deck-content question is open for the owner.** Its render half (the
coverage line collapsing "too few lands" and "wrong colour" into one string) is **D16**.

**Ownership (owner doctrine 2026-08-26).** Engine/UX only below. Guide content is the reviewers';
guide-side halves are named only where a docket item retires one. **Hard caps on the model's legal
choices are rejected on sight**; every item adds a true token, a true row, a receipt, a stamp or one
re-ask, and deletes nothing. Card-behaviour defects go CARD-SCRIPT FIRST. **Three window-removing
proposals were rejected this wave** (recorded under D2 and in `synthesis-notes.md`), and one — the
PLAN capture bound — is adopted in a form that caps the pilot's own PROSE and no choice.

---

# THE DOCKET, RANKED

## D1 — HIGH — a predicate-gated reveal whose option one has zero legal targets parks the engine forever: one game lost, 13 hours, invariant 00 failed. **FIX SHIPPED (lane X) — carried as a wave-54 verification item**
(= `general-strategy.md` **R161**; engine seat **HIGH #1**, seat-125-126 **HIGH #1**, seat-146 **E-4**)
**Repro.** `1788334437-ai_baka_deck152-0x5583d9910230-vs-ai_baka_deck125.jsonl` seq **26-28** with
`game-152v125-1788334434.stderr:1097-1214`. seq 26 (turn 15, `ATTACK: A1, A2, A3, A4`, 13 power vs
9 life — a correct lethal swing); seq 27 (`ask`, phase **Blockers**, Ranger Class's level-2 target,
answered Sigarda, 43.8 s — **the last completed model call**); seq **28**: `kind: "reveal"`,
`options: 5`, `options_text` **null**, `choice: -1`, `chosen_text: "none (no legal target)"`,
`latency_ms: -1`, `prompt` **0**, `reply` `""`. stderr then shows `doReactTo 149` +
`Action added to stack: StackAbility. (Source: Sigarda, Champion of Light)` and **not one further
`AIPlayerGPT:` line** — only repeated `AttemptNew failed to load (404): ZNR/491910.jpg` until the
harness killed it. The opposing seat's log simply stops at seq 17, turn 12. This is the ONLY record
in the corpus with `latency_ms < 0` and the only seat pair without a `gameend`.
**Mechanism (lane X, pinned).** `MTGRevealingCards::toResolve()` (`src/AllAbilities.cpp:446-478`)
picks the branch UP FRONT: Sigarda's `optionone target(<upto:1>human|reveal)` has
`countValidTargets() == 0` on Branchloft Pathway x2 / Barkchannel Pathway / Fateful Absence / Ranger
Class, so `abilityFirst` is never built and **option two** is armed and fired. `driveInteractiveReveal()`
phase 0 treated that chooser as option one's, asked the seat (`decideReveal` self-declines through
the N-136a empty-eligible shortcut — that is the seq-28 record, no model call, return -1), then
issued its finalize `CheckUserInput(JGE_BTN_NEXT)` with zero targets selected; `CheckUserInput`'s
decline branch is gated on `!abilitySecond`, so the press could not take it and option two's window
was spent on nothing. Phase 3 then fell to `if (!tc) return;` and waited for a chooser that had
already come and gone. The reveal display stays open and an open display holds every phase-advance
path including `userRequestNextGamePhase`. `REVEAL_DBG` is `WAGIC_REVEAL_DEBUG`-gated, which is why
13 hours produced **zero stderr**.
**Not card-specific.** Any reveal whose option one is predicate-gated and whose fail branch is
itself targeted (`optiontwo target(*|reveal) …`) is exposed. **5** Sigarda Coven reveals in the
corpus; four found a Human and completed: **1 in 5 Coven triggers, 1 in 21 reveal windows.**
**Status: SHIPPED to master by lane X, after the review opened, so it is NOT in this corpus's
binary.** Phase 0 now hands straight to phase 3 when `!abilityFirst && abilitySecond` (picks
explicitly dropped, a `DebugTrace` line on the path); phase 3 rebuilds option two once, latched by
`mAISecondRebuilt`, with an ungated stderr line. Fixture `w53_reveal_no_eligible_option_one.txt`
(GPT-free, drives Lexicon Hurkyl Wizard because Sigarda's name carries commas): **RED on base**
(`phase problem. Expected [Cleanup](12), got [End](11)`, five cards stranded, library 1 of 6),
GREEN after. Gate 1216/2-known + 46 AI/0, PARSETEST 2249/0.
**Wave-54 prediction (this is the discharge).** A reveal whose option one has zero eligible cards
resolves to option two and the game continues: **N/N**; **no seat log without a `gameend`**; 21/21
natural; every `reveal` record with `latency_ms: -1` and `prompt: 0` followed by a further record
from the same seat inside the same turn: **N/N**. Falsifier: any seat pair with no `gameend`.

## D2 — HIGH — the HOLD row works and three defects hold it at 2.5%: an index-0 echo that degrades it to a plain pass, a plan-conflict collision worth 5 of the corpus's 15 fallbacks, and a hold key that the opponent's own draw step retires every opponent turn
(= **R162**; engine seat **D-1 HIGH**, seat-125-126 **HIGH #2**, seat-146 **E-1 HIGH**, seat-123-130 **M1**, deck123 G5, deck146 R-3, deck162 R-10)
**What shipped and works.** `Hold priority - do not ask me again this turn unless the board changes`
— **1,173 renders, LAST row 1,173/1,173, opponent's turn only, exactly one distinct row string**;
**29 takes** across six seats; `hold_windows_skipped` **102**. **What failed:** opponent-turn casting
windows **884 -> 1,052 (+19%)**, casts **68 -> 58 (-15%)**, runs of 3+ byte-identical declined menus
**912 / 3,483 = 26.2%** and **147.4 min** (target < 10%, baseline 23.0% / 143.2 min). Seven phases
produced **0 casts in 445 windows and 3,939 s**: Main 2 (87), Combat begins (83), End (80), Combat
ends (80), Cleanup (79), Blockers (19), Combat damage (17). (On the engine seat's six-phase set:
365 windows / 3,269 s. Both true; the phase list is the difference.)

**(a) Index-0 degradation — 2 of 2 firings wrong, no note, no re-ask.** `125v146` seq **74** (2-row
menu, turn 23) and `146v125` seq **372** (turn 61) both reply `CHOICE: 0 (Hold priority)`; both
record `choice: 0`, `chosen_text: "pass"`. The menu never numbers the HOLD row 0 — row 0 is always
`0. Pass priority (take no action this window)` and HOLD is the LAST numbered row — so this is an
index/name conflict resolved index-wins, discarding the hold intent. It is lane N's own D9 fix
inverted: `0` and `pass` are now RESERVED echoes binding ahead of every index and name verdict, so
the HOLD name can never win. (Context: **75** replies answered `CHOICE: 0` on a menu that offered a
HOLD row; 283 of the corpus's 286 `CHOICE: 0` replies executed `pass` correctly.)
**Ask.** Reserve the HOLD row's short name the way `pass` is reserved; where a reserved pass echo
and a reserved HOLD name conflict, take the NAMED row or re-ask — never silently execute the third
thing. PARSETEST: `CHOICE: 0 (Hold priority)` on a menu whose last row is the HOLD row binds the
HOLD row and stamps a note; NEGATIVE: `CHOICE: 0 (pass)` on the same menu still binds row 0.

**(b) The D14 collision — 5 of the 8 `plan_choice_conflict` records.** Taking HOLD means "I do
nothing this turn", so the PLAN prose says *"I must pass priority"*; the detector reads a non-zero
index plus a pass verdict and re-asks. `146v125` s150 (`CHOICE: 4 (Hold priority)`), s371, s398,
s470, s491 — plus s392 and s555, which are the same vocabulary collision on a Hive animation, and
s22 at `123v126`, the one true false positive. Two re-asks degraded to a plain pass and three hit
`plan_choice_conflict_exhausted`; **each costs a full extra model call**. Every one of the 8 carries
`coded_answers: 1`, so lane N's `latched_coded_line` prediction had no window.
**Ask.** Exempt the HOLD row from the pass-verdict conflict test — a pass verdict CONFIRMS a hold.
This narrows the detector for no other shape (see the R118 reconciliation in `general-strategy.md`);
the wave-52 rejection of a general narrowing STANDS.

**(c) The hold key carries hidden-zone counters, so the opponent's draw step retires it by
construction.** `holdBoardKeyOf` (`src/AIPlayerGPT.cpp:12904`) is `serializeGameState()` minus its
leading phase line ONLY. `serializeGameState` emits `opponentZoneCountsLine(opp->game->hand->nb_cards,
…)` at line **12239** — `Opponent hand size: N | Opponent library: M cards` — and ends at line
**12314** with `yourLibraryLine(game->library->nb_cards, …)`. `holdStillStands` (12917) treats any
key change as a re-opener. **Repro:** `125v126` seq **128** (turn 30, opponent's Upkeep, `priority`,
2-row menu, HOLD taken) -> seq **130** (turn 30, opponent's **Draw**, the byte-identical menu, asked
again), with `game-125v126-1788334422.stderr` carrying exactly one
`hold re-opened at the priority seam - the board changed`; twelve more windows followed that turn.
This is why `hold_windows_skipped` is **0 on 38 of 40 gameends** while six different seats took the
row, and why the two non-zero seats (88 at `146v125`, 14 at `130v125`) are the only ones that took
it at a phase past the draw step.
**Ask.** Drop the hidden-zone counters (opponent hand size, both library counts) from
`holdBoardKeyOf`. A card moving into a hidden zone is not a board change the hold was about. Every
other re-opener the lane designed is untouched: life, poison, both battlefields, the stack top-first,
a newly affordable row, the turn ending. PARSETEST: a hold taken at the opponent's Upkeep survives
their draw step on an otherwise unchanged board; NEGATIVE: it does NOT survive a life change, a new
stack object, or a newly affordable row.

**(d) Adoption — the wording, once (a)-(c) are fixed.** **703** windows already carried
`[you declined this exact list N times already this turn]` with N >= 3 (N reaches 18) and the pilot
took HOLD in **3** of them (0.4%). Two additive asks: lead with the verb the model already uses
(*"Pass priority, and do not ask me again this turn unless the board changes"* — the pilot does not
read "hold priority" and "pass priority" as different acts, which is precisely what (b) measures),
and put the guarantee ON the row (*"(any change re-opens this window; you give up no cast)"*), which
today it never states.

**THREE WINDOW-REMOVING PROPOSALS ARE REJECTED IN THAT FORM.** (i) deck146 R-2 — withhold, or stop
re-offering, a cast row the engine has marked dead (`at 0 this does nothing` **613 renders, 2
takes**): it removes a legal action; the ceiling is the model's, not the engine's; and wave 52 showed
such a clause can be stale against a pending stack object. (ii) deck125 G1 — skip an opponent-turn
window whose stack is empty at a phase with no cast history: a phase's cast history is not a legality
fact, and this corpus supplies its own counter-example, `125v152` seq **15**, a real
`Cast Path to Exile` at the **Attackers** step. (iii) any blind identical-menu cache: wave 52
measured **30 real actions** following a decline on a byte-identical set in the same turn; not
re-opened.
**Wave-54 prediction.** `CHOICE: 0 (Hold priority)` executes the HOLD row or is re-asked **N/N**
(was 0/2); HOLD takes that also trip `plan_choice_conflict` **0** (was 5);
`hold_windows_skipped > 0` on every gameend where the row was taken **N/N** (was 2 of 6);
HOLD takes in windows carrying the declined-note at N >= 3 **>= 20%** (was 3/703 = 0.4%);
identical-declined runs **< 15%** of decisions (was 26.2%); opponent-turn casts **>= 90% of 58**.

## D3 — HIGH — a COMPENSATION search is framed to its beneficiary as a loss to minimise: Path to Exile's basic-land fetch renders "EXILE ONE OF YOUR OWN CARDS", 8/8
(= **R163**; engine seat **D-2 HIGH**)
**Repro.** 8 renders, all Path to Exile, all correct as to which side is choosing and wrong as to
what happens: `152v125` seq **12** (t9) and **19** (t12); `146v125` 11 / 37 / 64 / 171; `130v125` 69;
`162v125` 26. Header: `EXILE ONE OF YOUR OWN CARDS: your own effect (Path to Exile) makes you exile
a card from the list below - each option is a card YOU will LOSE, NOT a target you attack or affect.
Pick the card you can best AFFORD TO LOSE (usually your LEAST valuable: a spare land or a
redundant/dead card; KEEP your best spells, answers, and threats).` Rows are library basics.
Primitive `mtg.txt:84676`: `auto=moveto(exile) && ability$!name(search land)
notaTarget(land[basic]|mylibrary) moveTo(mybattlefield) and!(tap(noevent))!!$ targetcontroller` —
**the chosen card is put onto the battlefield tapped.**
**Mechanism (pinned this step, read-only).** `handRemovalVerb` (`src/AIPlayerGPT.cpp:20084`) matches
the lowercased chooser-source-name + ability-verb string in order — `discard`, `exploit`,
`sacrifice`, **`exile`**, `bury`, `put in library` — and `"path to exile"` contains `exile`, so the
verb is decided by the SOURCE CARD'S NAME before anything reads the destination. `buildHandRemovalAsk`
(20105) then correctly picks the SELF branch (`selfInflicted = tc->source->controller() == this`,
20470 — the search ability really is granted to `targetcontroller`) and prints the loss wording for
a gain. Consequence: the header inverts the value ordering (pick the land you least want, when the
right pick is the land you most need); it is also why two of the corpus's mega-menus exist (17
library Mountains at `130v125` seq 69; 14 library Swamps at `162v125` seq 26).
**Ask.** Gate the framing on the ability's DESTINATION ZONE, not the source name: a
`moveTo(mybattlefield)` / `moveTo(hand)` destination is a GAIN ask ("CHOOSE A CARD TO PUT ONTO THE
BATTLEFIELD … pick the land you most need"); only `exile` / `graveyard` / `library` destinations keep
the loss framing. PARSETEST: the verb table is already pure — add a destination argument and pin the
Path shape both ways.
**Wave-54 prediction.** Asks whose candidate zone is `mylibrary` and whose destination is the
battlefield or the hand, framed as a loss: **0** (was 8/8).

## D4 — HIGH — the kill summary counts creatures only, the cast row's target enumeration can omit a player entirely, and the target ask prints a survival verdict for every creature and none for the player: 26 rows, one lethal, one lost game
(= **R164**; seat-123-130 **H2 / G6**, engine seat **D-4**; skill #189)
**Repro A (the enumeration is right, the count is not) — 26 rows.** `130v125` seq **125** (turn 20,
`Opponent life: 14`): `Cast Hammer of Bogardan {1}{r}{r} … {kills 0 of the 1 legal target at 3
damage} - legal targets right now: **the opponent**, Rorix Bladewing … (creature 6/5) [flying,
haste], you` — the row lists three targets, the summary says one, and "kills 0" is true only of
creatures. Answered `Cast nothing right now`. Two seqs later at `Opponent life: 8` (seq 127) it took
the same row. Nineteen of the 26 are that one game; `130v123` seq 20/45/47 and `130v162` seq 62 are
the rest.
**Repro B (the enumeration itself omits the player) — the lost game.** `130v162` seq **62**, turn 22,
**7 life vs `Opponent life: 1`**, ten untapped sources:
`Cast Hammer of Bogardan {1}{r}{r} {leaves 7 of your 10 untapped mana sources untapped} {kills 0 of
the 2 legal targets at 3 damage} - legal targets right now: Shield Sphere (artifact creature 0/6)
[defender] …, Fate Unraveler (3/4) …` — **the opponent is not on the cast row's list at all**, though
Hammer of Bogardan is `target=anytarget auto=damage:3` (`mtg.txt:52046`). The row WAS taken; the
target ask at seq **63** then opened `0. The opponent (player, life 1)` — and printed
`{right now: takes 3 damage - SURVIVES (toughness 6)}` on the Shield Sphere, `- SURVIVES (toughness
4)` on the Fate Unraveler, and **nothing at all on the player row that wins the game**. The pilot
answered row 3 (Ob Nixilis). It declined a lethal Pyrite Spellbomb at seq 65 and died 0 to 1.
**This is by design and the design note is lane O's own:** "the D5 cast-row summary counts only
creature targets currently on their controller's battlefield; player targets and non-creature
permanents are excluded from the denominator."
**Ask, three additive parts.** (i) Enumerate player targets on the cast row of an `anytarget` spell,
as the target ask already does. (ii) Count them separately and carry lane O's own lethal tail across:
`{kills 0 of the 2 creature targets at 3 damage - and 3 to the opponent at life 1 WINS THE GAME}`;
failing the lethal half, print the denominator honestly (`of the 2 CREATURE targets`). (iii) Give the
player row in the target ask the same `{right now: …}` verdict its creature siblings get. The
arithmetic already exists twice (lane K's life clause, lane O's DRAW PRICE tail).
**Wave-54 prediction.** `{kills …}` rows whose enumerated targets include a player and whose summary
does not say so: **0** (was 26). `anytarget` cast rows omitting the players from the enumeration:
**0** (was 1, and it was the game). Player rows in a target ask with no damage verdict while a
creature sibling carries one: **0**.

## D5 — HIGH — a modal cast row prints no per-mode live/dead annotation, and neither does the mode sub-menu: 28 + 9 renders, 0 annotated
(= **R165**; seat-146 **E-2 HIGH**, deck146 R-1)
**Repro.** `146v125` seq **240** (T47) and seq **528** (T79, 20 untapped sources), with ~18 siblings
across turns 29-83. All **28** `Cast Silverquill Command {2}{b}{w}` rows in the corpus print exactly
`{leaves N of your M untapped mana sources untapped}` and `{card text: "Choose two — …"}`; the
literal `{modes live` renders **0** times. At s240 the reply asserts *"my graveyard has no creature
cards with mana value 2 or less"* with `- Your Triumphant Adventurer was COUNTERED …` and
`- Your Silverquill Silencer was COUNTERED …` in the log immediately above it. Verified:
`Triumphant Adventurer mana={B}{W}`, `Silverquill Silencer mana={B}{W}`, `Silverquill Command
auto=choice name(Return creature and you draw) target(creature[manacost<=2]|mygraveyard)
moveto(mybattlefield) && draw:1 controller && life:-1 controller`.
**A second modal surface has the same hole.** Nine records render `Choose one mode for Silverquill
Silencer:` followed by ten to fourteen rows of **bare card names only** — no mana, no board effect,
no card text, no dead-mode marker (`146v125` s10/s81/s506; `126v146` s6/s10; `130v146` s6;
`146v162` s19/s23; `146v123` s66). **0 of 9 carry any annotation.**
**Ask.** Extend to modal spells exactly the treatment targeted and edict rows already get
(`{kills: <name>}` 40 renders; `{right now: they sacrifice <name>}` inside 1,586 `{right now:`
renders): one clause naming which printed modes have a legal object right now and which do not —
`{modes live right now: return (Triumphant Adventurer, Silverquill Silencer); draw. Dead: pump (you
control 0 creatures), sacrifice (they control 0 creatures)}` — and the same per-row facts on the mode
sub-menu. This is information the engine must already compute to build that sub-menu.
**Wave-54 prediction.** Modal cast rows carrying a per-mode live/dead clause: **N/N** (was 0/28);
mode sub-menus whose rows carry no annotation: **0** (was 9/9); replies asserting a mode's object is
absent against a log or graveyard line that names one: **0** (was 2).

## D6 — MED — the OPPONENT's annihilator makes the seat sacrifice and the ask says `your own effect`: 18 of 26 renders
(= **R166**; seat-125-126 **MED #3**)
**Repro.** `126v125` seq **111-116** (turn 53) and `130v125` seq **181-186** and **189-194** (turns
48 and 50): `SACRIFICE ONE OF YOUR OWN CARDS: your own effect (Emrakul, the Aeons Torn) makes you
sacrifice a card from the list below … Choose card 1 of exactly 6`. Emrakul is on the **opponent's**
battlefield. Of the corpus's **26** `your own effect` renders, **18 are the opponent's Emrakul** and
8 are the seat's own Path (correct). `buildHandRemovalAsk` already has the right branch —
`byOpponent` prints `FORCED SACRIFICE OF YOUR OWN CARD: the opponent's effect (<name>) forces YOU
to …` — and was not taken.
**Mechanism.** `selfInflicted = tc->source && tc->source->controller() == this`
(`src/AIPlayerGPT.cpp:20470`). An ability GRANTED to a player is parsed onto a **nameless dummy
card** controlled by the victim, so that test reads TRUE for an opponent's annihilator. **The fix is
already in the tree one lane over:** lane P's D4 pinned exactly this indirection for the stack line —
the real granter is `MTGCardInstance::storedSourceCard`, set at `include/AllAbilities.h:4470`
(`myDummy->storedSourceCard = source;`), the same route `MTGAbility.cpp` uses for token creation and
mana production inside that keyword. Resolve `selfInflicted` through `storedSourceCard` when the
chooser's source is nameless.
**Cost this time.** At seq 111 the reply picked row **23**, its lone 1/1 lifelink Vampire, from a
23-row list holding 15 lands; at seq 112, identical framing, it reasoned correctly and began
sacrificing Sunpetal Groves. The record's `phase` also reads `Blockers` for an annihilator trigger.
**Wave-54 prediction.** Sacrifice/edict asks sourced from a permanent the seat does not control and
framed as `your own effect`: **0** (was 18 of 26).

## D7 — MED — the reveal driver has no bounded wait, a `-1` with no fallback is invisible to the recovery lane, and the harness watchdog cannot see a hang
(= **R167**; engine seat **D-3**)
Generalisation of D1, with two gaps the records add. (a) `152v125` seq 28 is a `choice: -1` with **no
`fallback` field**, so lane Q's `recovery` contract — **7/7 elsewhere** — does not see it; a `-1`
with no fallback tag is currently invisible to the ledger. It is also the ONLY record in the corpus
with `latency_ms < 0`. (b) **The feasibility watchdog measures LATENCY**, and a hang produces none:
the process wrote nothing for 13 h and nothing fired.
**Ask.** A tick budget per reveal phase that force-closes the display and writes a record on expiry
(lane X's latched phase-3 rebuild is the first half and already carries its stderr line); a fallback
class on any engine-answered `-1` so it earns a `recovery` record; and a harness-side **no-progress**
watchdog keyed on wall-clock since the last translog write per seat.
**Wave-54 prediction.** Records with `choice: -1` and no `fallback` field: **0** (was 1). Any game
whose two seats write nothing for > 30 min while the harness runs: **0**.

## D8 — MED — the `#a-#b` collapse does not reach ability, sacrifice or library-reveal menus, and a menu whose de-duplicated option list is length 1 is not a decision
(= **R168**; engine seat **D-5**, seat-123-130 **M5 / L2**, deck152 **R-6**, deck130 G8; D7's residual)
Four shapes, four denominators, all re-derived (skill #208):
* **> 12 rows collapsing to <= 2 masked shapes:** **2** masking `#N` and every digit (`130v125` seq
  **69** = **17 rows / 1 shape**, seventeen library `Mountain [land] [your library] - "R"`;
  `162v125` seq 26 = 14 / 2); **3** on lane P's own mask (adding `123v126` seq 30, 26 / 2); **5** on
  a looser mask. 36 records carry > 12 rows. Two of the three are D3's mis-framed Path search.
* **Ability menus:** `125v126` seq **220** = **28 rows** of `Deal 1 damage with Staff of Nin #N
  targeting …` (seq 189 and 213 at 26). The collapse works one seam away, on cast-row target clauses
  and on the battlefield block (`Glacial Fortress #1-#4 x4; Island #1-#8 x8`).
* **Forced-sacrifice menus:** `130v125` s181-s186 and s189-s194 — twelve consecutive annihilator asks
  of 18, 17, 16, 15, 14, 13 / 13, 12, 11, 10, 9, 8 rows, **fourteen byte-identical Mountains**, each
  prompt ~20 KB and ~99% game log, growing +675 chars per step, to pick a Mountain. The battlefield
  line directly above collapses the same permanents.
* **De-duplicated list of length 1:** **16 asks / 147 rows / 2.9 min**, 14 of them at `152v123`
  s23-s37 — a damage-assignment-order menu shrinking 12 -> 6 rows in which every row is the literal
  string `Vampire (2/2) [flying, doesn't untap during its controller's untap step]`. One of them
  (s29, a six-row menu) answered `CHOICE: 7` — out of range, one of the corpus's 15 fallbacks,
  correctly recovered by lane Q at s30.
**Ask, in this order.** (a) Extend lane P's `groupNumberedRows` permutation (already proven
index-safe with its map-back) to ability, sacrifice and library-reveal menus. (b) **Only then**,
where the collapsed list has a single entry and the engine's own handle map says the members are
interchangeable, resolve internally and log it as `mana_only_windows_skipped` (406) already does.
(b) is second on purpose: it is not a hard cap only once (a) has demonstrated the choice is vacuous,
and until then the model keeps the ask.
**Wave-54 prediction.** Ability/sacrifice/reveal menus with > 12 rows over one verb+source and `#N`
handles: **0** (was 28 at one seat). Asks whose de-duplicated option list has length 1: **0** (was
16). Max non-log prompt segment stays **< 12 KB** (this wave 11,231-11,256).

## D9 — MED — a reply that eats 96.5% of the deadline and ANSWERS is indistinguishable from one that took five seconds
(= **R169**; seat-125-126 **LOW #8**, seat-146 **E-7**)
Lane Q's timeout arm is correct and had **no window**: it stamps on an EMPTY body at >= 95% of the
deadline, and **no reply in this corpus was empty**. Re-derived, **six** replies reached 600 s, all
answered, none stamped: `126v125` seq **13** at **868,729 ms = 96.5%** of the 900 s wall on a
**3-row, 5,780-char** menu (and it is one of the two genuine mis-executions in D13); `126v146` seq
22 (**718,196**) and 23 (**608,145**), the two windows that lost that game; `162v126` seq 10
(**703,161**); `123v126` seq 1 (**660,402**, the opening mulligan on a **1,545-char** prompt);
`152v123` seq 16 (**629,642**, a three-row land drop). Five seats. Only ONE record exceeds 855,000 ms
(95%). The mulligan ask is systematically the most expensive decision of a game (`146v125` s1 at
595 s; five seats above 140 s).
**Ask.** Stamp any reply above the same `>= 95%` threshold with a `long_reply` note and the elapsed
fraction. `noAnswerClassFor` is already pure and this is the branch it does not cover. **Not** a
deadline change: `WAGIC_GPT_TIMEOUT` is the owner's dial and this item is observability, exactly as
wave 52 ruled for D10.
**Wave-54 prediction.** Replies reaching >= 95% of `WAGIC_GPT_TIMEOUT` carrying no stamp of any
kind: **0** (was 1; 6 above 600 s).

## D10 — MED — the `[DRAW PRICE:` tail is total on one of the tag's two forms and absent on the other: 58 of 85 rows state a life cost with no resulting total
(= **R170**; deck125 G3, deck130 G7, seat-123-130 **M3**)
Corpus-wide **85 row-instances in 71 records**, in exactly two emitter forms, and the split is clean:
* **Forced Fruition form** (`casting this draws YOU N cards (their Forced Fruition), and their … deal
  you N - you would be at K`): **27 rows, 27 tailed (100%)**, **16 carrying `; this KILLS you`** in 8
  records, all at `125v162`, **0 casts from a KILLS row**. **Every lethal row in the corpus carries
  the tail: 16/16.** Lane O's D3 is a clean PASS and the single most valuable render change of the
  wave — last corpus the identical menu at 3 life took the counter and lost the game.
* **cost form** (`this draws N card(s), and the opponent's <punishers> punish every draw, so taking
  it costs you N life right now`): **58 rows** (37 cast + **21 cycling ability rows**), **0 tailed**.
**Ask.** Give the cost form the same finished subtraction (`… costs you N life right now - you would
be at K`, `; this KILLS you` at K <= 0). The guides now teach "read the number after `you would be
at`" and 58 of 85 rows do not have one. None of the 58 was lethal this corpus, which is why this is
MED.
**Wave-54 prediction.** `[DRAW PRICE:` rows of any form stating a life cost with no
`- you would be at K`: **0** (was 58/85). Lethal rows carrying the tail: **N/N** (16/16, must stay).
Casts from a `this KILLS you` row: **0**.

## D11 — MED — unconditional removal has a perfectly knowable victim list and no `{kills: <name>}`: 244 of 413 rows
(= **R171**; engine seat **D-4** second half)
**244 of the 413 creature-targeting removal cast rows** carry no kill verdict — Path to Exile 224,
Fateful Absence 15, Vanishing Verse 5 — because lane O's magnitude test ("kills at N damage") is
inapplicable to a spell that simply exiles. `125v126` seq **13-23** is eleven consecutive Path rows
each printing a full target list and no verdict. The information is free: the enumeration is already
on the row.
**Ask.** Emit `{kills: <name>}` (or `{removes: <name>}`) for unconditional removal from the row's own
target list, using the same `castKillSummaryTag` writer.
**Wave-54 prediction.** Cast rows for unconditional removal carrying a victim clause: **N/N** (was
0/244).

## D12 — MED — the PLAN block is bounded by BYTES and not by SHAPE, and a stale plan is never diffed against the current menu
(= **R172**; deck123 G1, deck162 **R-9**; skill #207)
**The filed repro is refuted; the class is real.** seat-123-130 H3 reports `123v130` s57's `PLAN:`
swallowing ~2,000 characters and s63/s64 being served all of it. On disk the served block at s63 and
s64 is **151 characters** with no Morbid claim and no phase claim: `consumePlan` anchors on the
**LAST** line-leading `PLAN:` marker (s57's reply has markers at char 35 and char **3,755**) and
bounds the result at **1,600 bytes** cut at a sentence boundary. Both mechanisms worked; the largest
served PLAN block in the corpus is ~1.8 KB. The false Morbid assertion is in s64's and s65's **own**
replies, which is D14's class. (The 2,000-char tail the seat saw is real but is the reply's
`post_answer_overrun`, not the stored plan — 75 of the 78 replies over 1,500 chars are long because
of post-PLAN sprawl, `post_plan_overrun` non-zero on 64 records, max 11,305.)
**What is open.** (a) 1,600 bytes is far more than the protocol's "a few sentences", and a BYTE
bound is the wrong instrument: capture to the first blank line, or to the first sentence starting a
new line without a connective. **This bounds the pilot's own prose, not any menu or choice, and
removes no legal window.** (b) The genuine staleness instances are `162v152` s11 -> s12 (the reply's
plan names `Cast Master of the Feast` while the row it took in the same breath made Master
unaffordable; s12 was served that plan verbatim) and `125v126` seq 294/332 (recognition drift across
repeats of an identical board, 30 life). Lane N's age stamp works — **3,329 renders, 0 unstamped** —
so staleness is a NUMBER and not a FACT.
**Ask.** The paragraph bound, plus a one-clause diff of the served plan against the CURRENT option
list: `YOUR PLAN (as you last stated it, 1 window ago on turn 10; "Master of the Feast" is no longer
on your menu)`.
**Wave-54 prediction.** Served PLAN blocks longer than one paragraph: **0**. Decisions taken on a
served plan naming a card absent from that window's option list, with no contradiction clause: **0**
(was 2).

## D13 — MED — the latch path can execute a row that neither the index nor the name chose, with no note
(= **R173**; engine seat **D-8**)
`126v125` seq **13** (the 868.7 s record): `CHOICE: 2 (Cast Idyllic Tutor)` on a 3-row menu executed
row **1**, `Cast Perimeter Captain`; `coded_answers: 2`, `answer_replaced: true`,
`latched_coded_line: "2"`, `parse_note: None`. Seq 14 repeats it. Both times the index and the name
agreed with each other and disagreed with the executed row, which points at the latch/replacement
path rather than the echo parser. The corpus-wide reply-vs-executed audit is **25 disagreements over
3,253 parentheticals**: 6 cosmetic (a correct prefix of a longer label), 15 prose reversals the
engine correctly resolved to the last coded line, 2 the D2(a) HOLD degradation, and these **2 genuine
mis-executions** — **2 / 3,253, AT the carry's `<= 1/3,000` ceiling rather than under it.**
**Ask.** A note when the latched coded line's index AND its parenthetical both differ from the
executed row; better, a re-ask, as the named-row path already does.
**Wave-54 prediction.** Records whose index and parenthetical agree with each other and neither
matches the executed row: **0**, or **N/N** carry a note.

## D14 — MED — a reply whose PLAN contradicts its own CHOICE, on a row the engine itself labelled a no-op, is executed silently
(= **R174**; seat-125-126 **MED #4**, seat-123-130 **H1** false-negative half)
`126v125` seq **73** and **74** (turn 42, 6 life), two windows of one turn. Both menus are two rows:
`1. Cast Tribute to Hunger {2}{b} {right now: they control 0 creatures - at 0 this does nothing}` /
`2. Cast nothing right now`. Seq 73: `CHOICE: 1 (Cast Tribute to Hunger)` + `PLAN: … Avoid casting
Tribute to Hunger as there are no creatures to target.` Seq 74: the same choice +
`PLAN: The opponent has no creatures, so Tribute to Hunger does nothing. Pass the turn.` Both
executed; `answer_replaced` false, no parse note. Same class at `123v125` s84 (a dead Damnation cast
under prose reaching the opposite verdict with no second coded line) and `123v130` s64/s65 (a PLAN
asserting Morbid against a row printing `no creature has died this turn, so Morbid does NOT apply`,
then a target pick on a `- SURVIVES` row). Lane J's detector reads the prose BEFORE the `CHOICE:`
line and structurally cannot see a PLAN line after it.
**Ask, strictly ADDITIVE — a stamp, not a suppression and not a re-ask.** Fire when the chosen row's
own annotation says the action does nothing (`at 0 this does nothing`, `deals 0`, `destroys 0 of
their creatures`, `kills 0 of the`, `does NOT apply`) AND the reply's PLAN line names that same
action negatively. Whether it should then re-ask is a decision for the wave after the count exists.
**Wave-54 prediction.** Records whose chosen row carries a no-op annotation and whose PLAN argues
against that row, carrying no stamp: **0** (was 4 at two seats).

## D15 — MED — the echo matcher binds a card name that appears only inside another row's annotation
(= **R175**; seat-125-126 **MED #5**)
`126v130` seq **21** (turn 21). Rows: `1. Cast Idyllic Tutor {2}{w} [finds only an enchantment card -
every enchantment left in your library is a copy of one you already control or hold: Exquisite Blood,
Sanguine Bond] …` / `2. Cast Overgrown Battlement` / `3. Cast nothing right now (combat comes next
this turn)`. Reply `CHOICE: 3 (Cast Sanguine Bond)`. `Sanguine Bond` is on no row's LABEL and appears
exactly once on the whole menu — inside row 1's `[finds only …]` list. Result:
`parse_note: echo_index_conflict` (no `name_over_index`), **row 1 executed** — neither the index nor
a row offering the named card. The same shape at `126v125` seq 9 correctly produced
`named_row_not_offered` and a re-ask, so the seam resolves one shape two ways.
**Ask.** Match echoed names against **annotation-stripped row cores on every tier**, not only on the
exact-head path (`stripRenderAnnotationsLc()` exists), so a name occurring only inside
`[finds only …]`, `{kills: …}`, `{right now: … <name> is sacrificed}` or `[second copy: you already
control <name>]` can never bind.
**Wave-54 prediction.** Records whose reply names a card occurring on the menu only inside an
annotation and which execute a row rather than re-asking: **0** (was 1).

## D16 — MED — the mulligan coverage line collapses two different causes into one string
(= **R176**; deck146 **R-4**; skill #203)
`Playing every land in this hand would not cover any spell in it` is printed for "too few lands" AND
for "enough lands of the wrong colour". The one wrong mulligan on the reviewers' seats (`152v162` s1:
`Your hand (7 cards), counted by the engine: 2 lands, 5 spells` + `Mana sources among those lands,
counted by the engine: {G} 2`, against five white spells) is the COLOUR case, and the reply's own
reason was the colour: *"Mulligan for a hand with at least one white mana source"*. The engine prints
both facts on adjacent lines already.
**Ask.** Append the cause: `… (you have 2 lands; no spell in it is castable off {G} alone)` versus
`… (1 land is not enough for your cheapest spell at mana value 2)`.
**Wave-54 prediction.** `would not cover any spell` renders with no cause clause: **0**.

## D17 — MED — the attackers prompt lists every potential blocker per attacker and never prints how many blockers exist
(= **R177**; deck152 **R-5**; skill #202)
`152v126` s26 (31 life vs 17), s32 (25 vs 16), s40 (17 vs 16): four A-lines each time, the same
**two** distinct blocker names re-listed on every one, two attackers declared every time; their life
moved 17 -> 16 -> 16 and the seat lost 0-21 holding a 4/4 and a 7/5 whose worst printed price was
1 life.
**Ask.** One header line beside the existing LETHAL/converter lines: `They have N untapped creatures
able to block; declaring more than N attackers leaves at least (your attackers - N) of them
unblocked.` The engine has N — it built the per-attacker lists from it. This is the attack-side twin
of the block header's `Unblocked, these attackers deal up to X - you would be at Y`, which works
(9 block windows at those seats, 0 floor breaks).

## D18 — MED — a cast menu prices each row alone; nothing says which rows fit together in this window
(= **R178**; deck162 **R-8**; skill #205)
`162v152` s11 (turn 10, **4 life**, five untapped sources), one menu:
`Ob Nixilis, the Hate-Twisted {3}{b}{b} {leaves 0 of your 5 … - casting this taps you out}` /
`Master of the Feast {1}{b}{b} (5/5) {leaves 2 of your 5 …}` / `Shield Sphere {0} (0/6) {leaves 5 of
your 5 …}`. Rows 3 and 4 are affordable **together** — two blockers against the four attackers their
header had just printed. The seat took row 2, tapped out, s12's menu had two rows, and its own losing
reply's PLAN said *"Cast Master of the Feast"* — a plan the row it took had already made impossible.
**Ask.** Point the existing `{spends K of your M untapped mana sources this turn; <card> in your hand
needs N}` computation (666 renders) at the MENU instead of the hand:
`{taps you out - rows 3 and 4 on this menu become unaffordable}`, or the positive form on rows that
do not. The engine recomputes affordability for the next ask anyway.

## D19 — MED — the effect label degenerates to the source name on 53 stack lines
(= **R179**; engine seat **D-5** second half)
Lane P's D4 shipped clean — **706 stack lines, 0 bare**, every one `ability: <source>'s <effect>
[from your/their <source>] [triggered/activated ability]`, 385 `[from your …]` / 321 `[from their
…]`, 9 carrying the aim clause (e.g. `ability: Path to Exile's put a card onto the battlefield
(aimed at Plains) [from your Path to Exile]`). The residual is systematic and cosmetic: **53** lines
read `ability: Ob Nixilis, the Hate-Twisted's Ob Nixilis, the Hate-Twisted` because the effect label
falls back to the source name when the ability has none.
**Ask.** When the effect label equals the source name, print the ability's own menu text, or the
generic (`triggered ability`), rather than the name twice.

## D20 — MED — fetch rows name a land and never its colours: 90 renders, and it decided a game
(= **R180**; seat-123-130 **M2**, deck123 G3; skill #191)
`123v130` s36: a seven-row Marsh Flats menu — 3 Plains, 4 Scrubland, 5 Swamp, **6 Tundra,
7 Underground Sea** — each carrying `{this land makes no mana - crack it for a land: your untapped
mana sources go from 2 to 3}` and **no colour on any row**, while the mana line read
`colours you can make: {b}{w}` and the hand held two `{2}{u}` Intruder Alarms. The reply says "I need
a source that makes {U}" and answers **5, Swamp**. `Tundra subtype=Plains Island` (`mtg.txt:124691`),
`Underground Sea subtype=Island Swamp` (`mtg.txt:125732`); both are legal for Marsh Flats'
`target(*[swamp;plains]|myLibrary)` (`mtg.txt:71436`) and both make {U}. 90 renders of the clause at
that seat, colourless every time; dead turn 18.
**Ask.** Continue the clause with the colours in the words the mana line already uses:
`… sources go from 2 to 3, and it adds {W} or {U} (you cannot make {U} right now)`. The emitter
already prints `(land: taps for {W}{U})` on hand lines.

## D21 — MED — the non-lethal blockers hint asserts "while ahead on LIFE" without checking the life totals
(= **R181**; seat-123-130 **M4**, deck123 G4)
`123v130` s55, at **7 life against 12**: `Unblocked, these attackers deal up to 6 - you would be at 1
- NOT lethal: block only where the trade favors you; taking damage while ahead on LIFE is often
correct`. The pilot was BEHIND on life, answered `BLOCKS: none`, went to 1 and died next turn.
**This is the corpus's ONLY render falsehood** — every other open item is an omission, a stale
denominator, a framing or a missing stamp.
**Ask.** Gate the second clause on the two totals the emitter already holds: print it only when
`my_life > opp_life`, otherwise print nothing (the guide owns the rest).
**Wave-54 prediction.** `while ahead on LIFE` printed at a record where `my_life <= opp_life`: **0**.

## D22 — LOW — the `{paying this taps:` clause names the creatures but not the count
(= **R182**; deck152 **R-7**; skill #204)
Lane O's rework landed cleanly (0 old-string renders on a defender; 27 new; 41 rows total, always
exactly one of the two clauses, never both and never absent). What the clause still does not say is
how many attackers it removes, which is the whole trade: `152v130` s11/s14 held **one** name and
taking the row was right (that game was won 11 to -4); `152v162` s29/s32 held **two** and passing was
right. **Ask (cheap):** lead with the count — `{paying this taps 2 of your creatures: Luminarch
Aspirant, Intrepid Adversary - they cannot attack this turn}`.

## D23 — LOW — the LIFE-TO-DAMAGE CONVERTER block never says who it is bad for
(= **R183**; deck126 **G5**; skill #193)
Rendered **551** times corpus-wide (353 to deck125 as `theirs - Sanguine Bond`, 147 to deck126 as
`yours`), it is the best single-purpose surface either seat has and it is what deck125 re-keyed two
rules onto. It ends *"This is not a trade: when you gain N, your total goes UP by N and theirs goes
DOWN by N in the same event"* and leaves the pilot to derive the consequence for the card in front of
it — while the two decisions it should have decided were both about one row (an Emrakul an edict
turns into 15 life for the converter's owner: `125v126` seq 294 turned 36/8 into **21/21** in one
window; and a 1/1 lifelink token that is a 2-per-combat clock). The per-row fold DOES exist, but only
inside combat blocker annotations (**28** occurrences of `takes N off`, e.g. `(lifelink: they gain 1,
and their converter takes 1 off you)`).
**Ask.** When the block says `theirs`, append one clause naming the class most affected — *"any
creature of theirs with lifelink is worth double its power against you, and any creature of YOURS
they can make you sacrifice pays them its toughness"*. One sentence on a block that exists.

## D24 — LOW — the `{spends K of M …; <card> in your hand needs N}` clause names a card that may not be the binding one
(= **R184**; deck125 **G2**)
666 renders, and deck125 re-keyed its tap-out gate onto it. At `125v162` seq **23** it read
`{spends 2 of your 2 untapped mana sources this turn; Path to Exile {w} in your hand needs 1}` —
naming a Path, while the card the seat had to protect was a Fall of the Gavel at 5.
**Ask.** With more than one unpayable card in hand, name the **most expensive**, or name the count
(`3 cards in your hand need more than that`). The subtraction half is correct in every render checked.

## D25 — LOW — the forced-sacrifice list arrives in battlefield order, cheapest last
(= **R185**; deck126 **G8**)
`126v125` seq **111**: 23 rows in battlefield order — 15 lands, an enchantment, a planeswalker, two
artifacts, a wall, and the seat's only lifelink Vampire **last** — under a header saying "usually
your LEAST valuable: a spare land or a redundant/dead card". The pilot picked row **23**; at seq 112,
identical list, it picked lands. Six sequential picks from a 23-row list is the widest ask either
deck faces. **Ask.** Sort cheapest-first (lands, then duplicates, then the rest) or tag the obvious
candidates (`[spare land]`). Pairs with D8's collapse of the fourteen identical Mountains.

## D26 — LOW — the `(xN)` collapse still misses phase lines, draws and loyalty counters, and the miss GREW
(= **R186**; engine seat **D-6**)
**571** duplicate event lines inside single records (wave 52: 280): `- Phase: Draw` 237,
`- Opponent drew a card` 116, `- Phase: Main phase 1` 71, `- Phase: Combat damage` 9,
`- Opponent's Ob Nixilis, the Hate-Twisted lost a loyalty counter` 5. Lane Q measured the ADJACENT
case and declined it correctly (0 adjacent runs >= 3 survive; collapsing non-adjacent repeats across
turn boundaries would erase turn structure) — that verdict stands and is not re-opened. What grew is
the within-record duplicate count, at a log that is 75-80% of every 30 KB prompt with 476 trims
firing. Free space, and the only lever on the tail besides D32, which is the owner's.

## D27 — LOW — receipts without a payee: 2 (7 -> 3 -> 2)
(= **R187**; engine seat **D-7**)
`152v126` seq 25 (`- Paid {1}{w} with Plains #2; Deserted Beach`) and `152v162` seq 29 — both the
Intrepid Adversary pay-any-number-of-times ETB; two distinct receipt LINES, repeated 38 times across
the logs. **496 receipts, 0 with no source tapped**, 11 legitimate `from mana already floating`.

## D28 — LOW — ability rows that point damage at a player print no life total
(= **R188**; seat-123-130 **L1**, deck130 G9; wave-52 D22 carried)
`130v125` s120, s158; `130v162` s35 — `Deal 2 damage with Pyrite Spellbomb targeting the opponent`
beside target menus that print `The opponent (player, life 8)`. With D4 this is the same ask from the
other side: every row that can point damage at a player prints that player's life.

## D29 — LOW — the bare `[second copy: … no legend rule]` now reads as the "safe" tag by contrast
(= **R189**; deck126 **G7**, recorded as a wording RISK created by a good change, not a defect)
Lane P gave the Lantern a longer tag ending "this copy adds only its own abilities" and deck126
declined **6 of 6**; on the same seat `[second copy: you already control Sanguine Bond; both stay on
the battlefield - no legend rule]` — correct, two Bonds do both trigger — was taken at `126v125` seq
22. With one form carrying a "does nothing extra" tail, the ABSENCE of that tail reads as "does
something extra". deck126's own recommendation is to leave the tag alone and let the guide carry it
(D86), which it did. Recorded so a later wave does not read the take as a render failure. Census:
86 rows / 12 takes — Lightmine Field 35 rows / **0 takes**, Talisman 19/3, Greaves 7/**0**, Lantern
6/**0**, Underworld Dreams 6/2, Howling Mine 5/2, Staff 3/3, Sanguine Bond 2/1, Intruder Alarm
1/**0**, Pyrite Spellbomb 1/1, Exquisite Blood 1/0.

## D30 — LOW — the priority seam's ask line does not say whose turn it is
(= **R190**; deck125 **G4**)
Casting asks read `Casting decision (Draw, opponent's turn): …`; the **priority** seam's ask line
carries no turn attribution, and the priority seam is where deck125's repeating two-row Elixir menu
lives (121 of the corpus's HOLD-bearing windows are `priority`). Cheap and purely additive.

## D31 — LOW — `pass_row_named` never renders: the behaviour is right and the tag is missing
(= **R191**; R144's residual)
284 replies wrote a reserved pass echo on a menu carrying `0. Pass priority` and **284/284 executed
row 0** — lane N's D9 is a clean behavioural PASS — but **0 records carry the predicted
`pass_row_named` note**, so the next corpus cannot distinguish "the reserved-echo path fired" from
"the index happened to be 0". A one-field stamp. (Two records executed a CYCLING row on `CHOICE: 0`;
both menus had no pass row, so the index was correct.)

## D32 — MEASUREMENT / DECISION — the log window, with the cap working harder and the tail still growing — OWNER ITEM
(= **R192**; wave-52 D27 continued)
**476** prompts carry `(…earlier events trimmed…)` (wave 52: 165); log segment max **24,107**, p50
9,107; max non-log segment **11,231-11,256** (lane P's `< 12 KB` prediction PASSES); **12 prompts
exceed 30,000** (was 7) and **588 exceed 25,000** (was 330); mean `ask` prompt up ~13% while the max
is flat. Every prompt above 29 KB is ~75-80% log. Exhibit: `146v123` seq **92** (32,068 chars, turn
33, priority). Log compaction is the only remaining lever on that tail and the decision is the
owner's.

## D33 — MED (frontend / human seat) — Arena-style grouping + the O(n^2) `RenderSpell` scan — OWNER ITEM, CARRIED, now with numbers
(= wave-52 D28)
Lane W profiled it at **-O2** with a 30 fps `dt` (both corrections mattered: the tracked desktop
makefile builds `-O0`, and `WAGIC_FASTCLOCK=0.1` expires Baka's decision timer every tick).
Per-frame at 12-22 permanents: RENDER **1.28 ms** total, of which `CardGui::Render` **0.81 ms**
(**0.061 ms per card on screen**, ~88% the drawing itself), `GuiGameZone::Render` x10 **0.24 ms**,
`CardStack::RenderSpell` (D33's named O(n^2) scan) **0.13 ms**, `GuiAvatar::Render` x2 0.08 ms;
UPDATE **2.02 ms** total, of which `[AI Act]` **1.68 ms avg / 58.5 max**. Update is super-linear in
board width and is almost entirely `Act` (Update-minus-Act at 22 permanents is **0.46 ms**); Render
is linear at **+0.03 ms per permanent**. Bounding `CardGui::Render` means drawing fewer or cheaper
cards, i.e. Arena-style grouping — **the owner's item, measured not redesigned.** Lane W shipped the
bounded part (skip a `GuiGameZone` that cannot draw anything + one icon fetch instead of six:
zone renders executed 10.0 -> 0.6 per frame, `GuiGameZone::Render` **4.2x**, whole-frame Render
3.03 -> 1.86 ms, ~18% of the render frame removed at every board size) and the alpha-gated Vita
frame telemetry that will price the console. **New, not measured on the console:**
`GuiPlay::receiveEventPlus` (`src/GuiPlay.cpp:394`) calls `Replace()` **unconditionally** on every
permanent entering play — a full play-zone re-layout per ETB event, before the card is even added to
the CardSelector. On the token-swarm boards lane W profiled that is an O(n) relayout per token
created. Worth a look in the next frame-rate pass.

## D34 — MED — replay is unusable on human-vs-Baka dumps for four separate reasons, and it blocks every owner play report
(= **R194** in the carried set; lanes T, U, W, R)
(a) **RNG-stream drift (NOT fixed).** `GameObserver::randomGenerator` is ONE stream serving both
`MTGGameZone::shuffle` and the AI seats' own `random_shuffle` in `AIPlayer.cpp`; AI seats are passive
during a replay, so they do not consume their values, every later draw is offset, and a mid-game
shuffle produces a different library (lane T: replay drew Ramunap Excavator on turn 6 where the
device drew Predator Ooze). Fix: a separate `RandomGenerator` per AI seat — the class comment already
says that was the intent — or record their draws separately. (b) **A chooser-order divergence**
(lane U): the vpk12 report-A dump diverges at **action 83, turn 6**, `chooser=read the bones` — a
scry/surveil window resolving in a different order, firing BEFORE the game's only shuffle, so it is a
distinct defect. (c) Lane W's two dumps: one diverges at **action 94** (`p2.no 1` vs `p2.choice 0`),
one **aborts in `dumpAssert` at load**. (d) Lane R's dump diverges at **action 49** with the hand one
card short. Lane T's index-drift fix shipped (name-based resolution inside the recorded zone under
`WAGIC_REPLAY`, nearest slot wins) and carried one dump from action 112 to 116; the rest is open.
**Consequence, stated by four lanes independently: every owner play report is investigated from a
transcript plus a fixture, never from the board that produced it.** Note also that even a perfect
replay cannot re-decide an AI decision — the AI's clicks are IN the record, so `chooseBlockers` does
not run during a replay.

## D35 — MED — `kMaxOptSources` is 14 and the owner's option-preserving autotap is a no-op above it
(lane S; carried from the wave-53 carry-list)
`refineForOptions` bails at `sources.size() > kMaxOptSources` (14); the board the owner reported
offered **16** candidate producers, so **the spec he wrote never applied in the game he reported**
(it cost 0.08 ms, not the freeze). Raising the cap needs its own cost measurement — flagged, not
changed. Related and separate: lane S's shipped perf work (batch `usableAbilityCards`, the
non-mana-tap index) took the per-tick hand refresh **1.64 -> 0.44 ms** and the per-focus-change
refresh **2.84 -> 1.29 ms**, with the owner's autotap spec untouched.

## D36 — MED — `planPayment` / `potentialMana` / `selectAutoTapProducers` cannot see a `&&`-wrapped mana producer
(lanes R and S; the same blindness at three call sites)
`ActionLayer::manaObjects` is built through `AbilityFactory::getCoreAbility`, but its readers use a
plain `dynamic_cast<AManaProducer*>`, so `{T}:add{G} && counter(0/0,1,nest)` (Twitching Doll) is in
the index and invisible to the reader. Lane R fixed the ONE site where this hung the phase ring — the
unpayable-commitment release in `MenuAbility::Update`, now a deliberately permissive local walk that
reads the index the way it was written, matching by POINTER against the battlefield array (matching
by `hasCard()` counted parse-time template producers with id 0 and kept a fixture hanging — measured,
then fixed). It deliberately left `potentialMana` alone: widening it there regressed
`oracle_selftap_westvale_5_lands`, `oracle_ability_dual_not_double_4_lands`,
`bestow_no_envoy_human_gate_lands` plus 2 AI tests (measured, then reverted). **Residual:** such a
source must still be clicked by hand, and AI payment planning cannot see it. Needs its own pin.

## D37 — LOW — the Dominating Vampire livelock candidate, unreproduced
(lane R, investigation only; stated as a hypothesis, not a finding)
`MayAbility::testDestroy` returns 0 while ANY `menuObject` is armed and `MayAbility::Update` fires
once (`triggered`), so a `may` that trips its no-valid-targets early return on a tick when another
card's menu is open stays registered forever with `triggered == 1` — never updating again, but still
answering `isReactingToTargetClick` once targets become legal later. Combined with
`AIPlayerBaka::computeActions` / `chooseTarget` both returning 0 outright for a seat that is not
`currentlyActing()`, a chooser armed for the AI during the HUMAN's turn can never be answered, and
`ActionLayer::Update` early-returns on `menuObject`, pinning the phase ring exactly as the owner's
transcript shows. `MayAbility::reactToTargetClick` also overwrites `mClone` on every click without
destroying the previous one. **Four fixtures failed to arm it** (a legal-target cast, an all-illegal
cast, a DV+Odric turn-11 reconstruction, and a later-legalising target) and `getIndexOf(NULL)` was
ruled out. **The next step that settles it** is a per-tick instrumented run printing `menuObject` /
`currentWaitingAction` / `currentlyActing` and flagging any menu armed for a non-acting seat — cheap,
and it converts the hypothesis into a repro or a refutation. Dominating Vampire is not in the corpus
pool, so this is not corpus-testable. Also noted, not investigated: the same transcript shows the AI
clicking a spent planeswalker 13 times in one turn, every click returning 0.

## D38 — LOW — there is no `assertmtgid` suite command, so a post-flip printing cannot be pinned
(lane V)
Lane V's DFC fix has a pure core pinned in PARSETEST on the real MOM ids
(`wagicPickFaceSiblingId`, nearest-id-in-either-direction; a forward-only rule got back->front wrong
the moment a second printing existed further up `_cards.dat`, and PARSETEST caught it). The
ASSEMBLED `MTGAllCards::getOtherFaceCard` is observed by no harness: PARSETEST runs before the card
database loads, and the suite deliberately SKIPS `setMTGId` on flip
(`playMode != MODE_TEST_SUITE`). **Ask:** an `assertmtgid` suite command plus lifting that skip
under the test-suite play mode.

---

# DISCHARGED — SET B: the wave-53 docket D1-D28 and lanes M-X, by the emitter's actual string

| item | lane / commit | verdict | counts |
|---|---|---|---|
| **D1** silent zero-block after `multiblock_reask` | M 614312289 | **SHIPPED — PASS 0/30** | `multiblock_reask` does not appear as a fallback class at all (was 1 of 3 with no follower); all 30 `blockers` records carry a `chosen_text`; 30 asks <-> 30 stderr `declared blocks from N assignment(s)` lines, 1:1, including the 5 `no blockers` answers. `blockers` prompts **34% smaller** (mean 9,541 vs 14,350) |
| **D1b** `multiblock_first_wins` note | M | **SHIPPED — PASS 1/1, first live firing** | `152v130` seq 16, turn 12: `BLOCKS: B1:A1, B1:A2` -> `dropped_assignments: 1`, `parse_note: multiblock_first_wins`, executed `Sigarda, Champion of Light blocks Siege-Gang Commander`. Exactly one record in the corpus carries `dropped_assignments: 1`; the other 29 are 0 |
| **D2** the opponent-turn window economy / HOLD row | N 3ab5c27c4 | **SHIPPED — the mechanism is correct and EVERY headline prediction FAILED** | windows 884 -> **1,052** (+19%; 1,126 / 1,243 on looser keys); casts 68 -> **58** (-15%); runs of 3+ identical declined **26.2%** vs a target of <10%; HOLD **1,173 renders / 29 takes (2.5%)**, LAST row 1,173/1,173, opponent's turn only, ONE distinct row string; `hold_windows_skipped` **102**, non-zero on **2 of 40** gameends. Zero-cast phases: 7 phases / 445 windows / 3,939 s. The falsifier's stated form ("row never taken") did NOT fire — **the row is taken and cannot be honoured** — **D2 above** |
| **D3** the `[DRAW PRICE:` lethal tail | O 0ba5ae2eb | **SHIPPED — PASS 16/16 lethal rows, 0 casts** | 85 rows / 71 records; the Forced-Fruition form is **27 rows, 27 tailed**, 16 with `this KILLS you` in 8 records, all declined. Eight lethal declines at `125v162` seq 79-88, including seq 88 where all three cast rows were counters aimed at an Underworld Dreams on the stack — the identical menu at 3 life took the counter and lost the game last corpus. Residual: the cost form, 58 rows / 0 tailed — **D10** |
| **D4** the edict forecast vs a pending stack object | P 1057c7c9d | **SHIPPED — UNTESTED (0 renders)** | ` - a sacrifice is already on the stack aimed at this permanent` renders **0**; no second edict aimed at a permanent already under one arose. Old-string 0, new 0, takes 0. The wave-52 triple did NOT recur: `146v125` T50 resolved to `Opponent's Emrakul, the Aeons Torn died`, the T64 copy was countered |
| **D4a** stack lines name source and victim | P | **SHIPPED — PASS 0/706 bare** | **706** emitted, every one `ability: <source>'s <effect> [from your/their <source>] [triggered/activated ability]`; 385 `[from your …]`, 321 `[from their …]`; 9 carry the aim clause. Wart: 53 lines print the source name twice — **D19** |
| **D5** the cast row's kill summary | O | **SHIPPED — PASS on emission (169 rows), FAIL on the denominator** | 129 `{kills N of the M legal targets …}` + 40 `{kills: <names>}` (was 0/85); 4 count-row takes, 3 named-row takes; **0 disagreements between the cast summary and the target-ask verdict**. But 26 rows list a player and exclude it, one of them lethal — **D4** — and 244 of 413 unconditional-removal rows carry no verdict at all — **D11** |
| **D5b** removal cast where every target survives 0 | O | **FAIL 4** | `123v130` s64 (Tragic Slip), `130v123` s47 (Spark Spray), `130v125` s127 and `130v162` s62 (Hammer of Bogardan). The render was correct and in front of the model each time — a deck-seat call — **except** s127 and s62, where the summary itself under-counts (**D4**) |
| **D6** the defender tap consequence | O | **SHIPPED — PASS** | 41 `{paying this taps:` rows in 32 records, always exactly one clause. **Old string on a defender: 0.** New string `- it cannot block on their turn`: **27**, all Overgrown Battlement (`mtg.txt:83596` `abilities=defender`), **8-10 takes**. The remaining 14 old-string rows name only non-defenders (Katilda 10, Luminarch Aspirant 4), for which `cannot attack this turn` is correct. Forecast source count == receipt source count **27/27** — wave-52's residual **CLOSED**. Residual: the clause carries no count — **D22** |
| **D7** ability-menu collapse | P | **SHIPPED — FAIL** | 2 / 3 / 5 menus of > 12 rows collapse to <= 2 shapes depending on the mask; 28-row Staff of Nin ability menus; twelve uncollapsed forced-sacrifice menus; 16 asks whose de-duplicated list is length 1 — **D8**. Max non-log segment **PASS** (11,231 < 12 KB) |
| **D8** the from-exile cause | P | **SHIPPED — PASS 2/2, sample too small to re-test** | only 2 `{castable from exile …}` rows exist and both name the cause (`- your card, exiled by their Elite Spellbinder, which lets you cast it from there; it costs {2} more…`); the causeless fallback form renders **0**; both taken. D8's durability is not really re-tested at N = 2 |
| **D9** the `(Pass)` echo | N | **SHIPPED — PASS on behaviour 284/284; the NOTE never renders** | 284 reserved pass echoes on a menu carrying `0. Pass priority`, **284/284 executed row 0**; 0 in-range non-zero index carried a pass parenthetical. `pass_row_named` **0 renders** — **D31**. And the reservation is what makes **D2(a)** possible: `CHOICE: 0 (Hold priority)` binds row 0 by index, 2/2 |
| **D10** timeout observability | Q ba33a9adb | **SHIPPED — UNTESTED, N = 0** | **0 empty replies**, **0 `retry` fields**, `fallback: "timeout"` **0 renders**, `no reply after` in stderr **0**. Max latency **868,729 ms = 96.5%** of the wall and it ANSWERED; only 1 record exceeds 95%. Wave-52's `empty_reply` within 1 s of the wall 3 -> **0**; Baka-executed opening keeps 2 -> **0**. NOT a PASS — the arm never had a window. New residual: a non-empty reply at the wall is unstamped — **D9** |
| **D11** the `[second copy:` usefulness verdict | P | **SHIPPED — PASS, and the clearest render-to-behaviour result of the wave** | 86 rows / 12 takes, **all on genuinely stacking scripts** (Staff 3, Talisman 3, Underworld Dreams 2, Howling Mine 2, Sanguine Bond 1, Pyrite Spellbomb 1). The one Intruder Alarm row carries the dead tail and was **not** taken. Chromatic Lantern's partial form rendered **6** and was declined **6/6** — against **5 of 5 taken** last corpus under the bare string, **with no guide change at all**. Lightmine Field 35 rows / 0 takes, Greaves 7 / 0 |
| **D12** plan age + the latched coded line | N | **SHIPPED — PASS 3,329/3,329 on the stamp; `latched_coded_line` UNTESTED** | two forms only (`, 1 window ago on turn N` 2,375; `, N windows ago on turn N` 954); max `plan_echo_count` **43** (was 51); `plan_echo_count` present on 1,577 records. All **8** `plan_choice_conflict` records carry `coded_answers: 1`, so the predicted case never arose; the field itself works — **39** records carry it, and the 12 with value 2 are exactly the 12 `answer_replaced: true` records |
| **D13** the converter drain folded into its rows | O | **SHIPPED — UNTESTED (0 renders)** | ` - and their <name> takes N off YOU: life A -> B` renders **0**; no edict/gain row coincided with a converter. The per-row fold exists only inside combat blocker annotations (**28** `takes N off` occurrences); the static CONVERTER block renders **551** times |
| **D14** the battlefield header counts lands | P | **SHIPPED — PASS 3,429/3,429** | e.g. `Your battlefield (27 permanents listed, of which 0 are creatures, 0 of them able to attack right now, and 25 are lands)`. deck130's land-destruction casts past its own four-land gate: **6 of 26** (was 8 of 19) — the guide half, not the render's |
| **D15-D18, D21-D22, D25-D26** carried LOW | not shipped | **RE-MEASURED, carried** | D15/D150 converter branch: guide-side (#202/#204) ships first. D16 punisher tag: **CLOSED in effect by behaviour** (`130v162` s37-s39 took Starstorm X=4 the window it reached the punisher; 34 punisher windows, 0 draws taken). D17 discard duplicate markers: **CLOSED** (markers printed on all 5 asks; the duplicate was sent). D18 `{feeds:}` K-0: carried, and the MIRROR lane opened — 11 first-Mine over-brake windows (was 1), guide-side (#206). D21 payee: **2** — **D27**. D22 face row's life: carried — **D28**. D25/D26: re-measured, no new window |
| **D19** `Bn:Bm` in the attacker slot | M | **SHIPPED — UNTESTED** | no `Bn:Bm` assignment arrived in 30 blockers replies; `blocker_handle_in_attacker_slot` renders 0 |
| **D20** the `(xN)` collapse | Q | **MEASURED and DECLINED (correctly); the within-record miss GREW** | 0 adjacent runs >= 3 survive; 571 duplicate lines inside single records (was 280) — **D26** |
| **D23** the BLOCKS format line | M | **SHIPPED — PASS 30/30** | `…e.g. "BLOCKS: B1:A2, B3:A1, B2:none" - each B-number at most ONCE, and several B-numbers may share one A-number - or exactly "BLOCKS: none"…`; repeated B-numbers fell from 3 per corpus to **0** |
| **D24** the recovery record | Q | **SHIPPED — PASS 7/7, 0 unpaired** | exactly 7 `kind: "recovery"` records and exactly 7 decision records with `choice: -1` AND a fallback; every one paired by `recovers_seq` with `recovers_kind`/`recovers_fallback`/`recovered_by`. The 8 `plan_choice_conflict` records (choice != -1) correctly get none. The new `retracted_choice` class had its **first live firing** (`146v125` seq 313: `CHOICE: 1 (becomes beholder…)` followed by 200 words ending "So I must PASS", `commit_retracted: true`, and the recovery record shows Baka then did row 1 anyway). **Gap:** the hang record is a `choice: -1` with NO fallback and gets no recovery — **D7** |
| **D27** the log window | n/a | **RE-MEASURED** | **D32** |
| **D28** frontend / human seat | owner item | **CARRIED, now measured by lane W** | **D33** |
| **lane R** 1bb08b2b1 (Go-Shintai `may pay({1})` interrupt) | shipped | **UNTESTED in corpus** | 0 `may`-prompt fallback records; Go-Shintai and Dominating Vampire are not in the pool (all seven deck lists checked). Suite pin `w53_pay_interrupt_creature_mana.txt` RED on base (`assertpt 2/2` got `1/1`), GREEN after. Report 1 NOT fixed — **D37** |
| **lane S** 6a4d5e119 (hand-refresh perf) | shipped | **UNTESTED by construction** | human-seat only, no corpus exposure. Equivalence pin `w53_usable_ability_batch_board.txt` + `assertusable` cross-check across 8 existing fixtures. Raised, not changed: **D35** |
| **lane T** 00549b391 (Baka aggregate-lethal blocking sweep) | shipped | **UNTESTED in corpus** | **0** `blockers` records carry a fallback anywhere in the corpus — the heuristic never made a block decision. Fixture `ai_block_lethal_set_wave53.txt` RED on base (defender at **-7**, zero blockers) / GREEN after (defender at **1**, three Hill Giants up). The owner's Vita play is the test. Residuals stated by the lane: no trample top-up on an already-blocked trampler; "cheapest body" is power-then-toughness with no notion of card value |
| **lane U** (Baka deathtouch blocking; the per-observer game-end one-shot) | **NOT in this binary** | **SHIPPED-UNTESTED** | Fixture `ai_block_deathtouch_gang_w53u.txt` RED on base (a guard spent for nothing, the Gorgon alive; `life expected 18, got 20`) / GREEN after. **Pass 1's gang guard has NO red-on-base fixture** and the lane says so: pass 1's accept branch is gated on `AIStats::isInTop(attacker,3,false)`, which returns false whenever the stat list holds fewer than 3 entries, so pass 1 is unreachable on a fresh fixture board. The transcript half is proved by a 16/16 correlation between the missing `#result` line and the missing memlog `gameend` — two independent `static GameObserver *` one-shots sharing one operand — with the desktop unable to reproduce it (42/42 wrote `#result`; 0 pointer repeats in 47 games; glibc does not recycle, newlib does). Wave-54 test: the owner's next Vita session |
| **lane V** (DFC back-face printing + art) | **NOT in this binary** | **SHIPPED-UNTESTED; art regenerated, NOT uploaded; no VPK built** | Two stacked defects, each sufficient alone: `AAFlip::resolve` resolved the other face by NAME and both printings carry `rarity=T`, so the token-fallback branch returned the LOWEST id (607030) and a borderless Heliod became the regular printing's back face; and `CardGui::Render`/`RenderBig` re-resolved by name again. Separately the bulk downloader mapped every multiverse id onto `card_faces[0]`, so **207 of the 740 real back faces in `Res/sets` were front-art duplicates** (`MOM/607030.jpg` byte-identical to `607029.jpg`). 207 repaired, 103 misses (ids Gatherer never assigned), 14 zips spliced at the exact geometry. **The deploy and the VPK are the owner's.** Pin gap: **D38** |
| **lane W** (Vita frame rate) | **NOT in this binary** | **SHIPPED-UNTESTED on the console** | `GuiGameZone::Render` skip + one icon fetch: zone renders 10.0 -> 0.6 per frame, that leg **4.2x**, whole-frame Render 3.03 -> 1.86 ms, ~18% of the render frame removed at every board size. Alpha-gated per-turn and slow-frame telemetry shipped for the Vita (`WAGIC_VITAMEMLOG`, compiled out of every other build). **Every number is desktop**; the console is the test. Two instrument corrections named — **D33** |
| **lane X** (the reveal hang) | **NOT in this binary — merged to master after the review opened** | **SHIPPED-UNTESTED** | **D1** |
| carry: 21/21 natural | | **FAIL 20/21 — invariant 00** | `152v125` HUNG and was adjudicated; excluded from every rate; no rerun owed (see the header) |
| carry: Baka-executed <= 0.20% | | **PASS 0.11%** | 4 / 3,483 |
| carry: `stale_livelock` 0 | | **PASS 0** | third corpus |
| carry: `with no source tapped` 0 | | **PASS 0** | 496 receipts |
| carry: name-vs-executed mismatch <= 1/3,000 | | **AT the ceiling, not under it: 2/3,253** | **D13** |
| carry: no prompt > 30,000 | | **FAIL 12** (was 7) — every one 75-80% log | **D32** |
| wave-52 owner batches (bd515aad7 / c15106f4d / 4abcf5dd0 / the Vita batch) | | **UNTESTED** | no adventure/split/aftermath/morph/evolve card, no Calamity Bearer, Drake Hatcher, Delver, Hapatra, Wilt-Leaf, Hurkyl or Pridemate in the seven-deck pool (all deck lists checked) |
| c1a0bf814 option-preserving autotap (HUMAN seat only) | | **UNTESTED; no leak** | no forecast-vs-receipt mismatch arose at all this corpus (0/27 at the seat that produced last corpus's 1/7). Separately **D35**: it is a no-op above 14 producers |
| 5ca11d6e2 Baka pump targeting | | **UNTESTED** | Baka executed 4 decisions, none of them a pump or an ability target |

# DISCHARGED — SET A: wave-52 guide edits + the boundary pass, by deck (reviewer verdicts, summarised)

| deck | verdict summary | routed |
|---|---|---|
| **123** (2-4) | P1 Slip **FAIL 1** (and the row now says `{kills 0 of the 3 legal targets}` outright — #187); P2 fetch **FAIL 3 of 8** (a basic taken with duals listed; one was the game) — **the carve-out was unreachable**, #191; P3 second Alarm **PASS by absence** (1 row, 0 takes; 7 Greaves rows likewise); P4 Damnation plan-text **0/6, THIRD corpus at zero** — the guide lane is exhausted, the ask becomes a row annotation; P5 edicts under a converter **PASS 0**; P7 cleanup **FAIL 1**; P9 repeat chains **PASS 4/4 with `L, C, stop` filled**, both games won; P10 dead opponent-turn rows **PASS 234/240 correct declines** with 1 HOLD take in 240 | **D20**, **D2**, skill #187/#191 |
| **125** (4-1 natural) | W71 lethal DRAW PRICE **PASS 0/24 rows** — the wave's best single render result; W72 tap-out gate **FAIL 8** (Elixir 5/16, Staff 2/7, Lightmine 1/2), none punished because the opponent-turn window stayed open; W73 `X = 3` **PASS 15/15**, low-X Revelation **FAIL 2/116**; W74 land drop **85/85**, sweeper at header 0 **0/155** (fourth corpus), Lightmine **2/117** both at the right branch, Path **FAIL 4/18** (three right in substance under a printed CONVERTER line); W75 target order **PASS 7/7**; W76 second-copy **PASS 3/3**, cleanup **MIXED**. The single loss is a threshold SHAPE, not a reading — #192 | **D23**, skill #192/#193/#194 |
| **126** (2-4) | D75 **FAIL 0/2 + 2** (two `Cast nothing` at 4 life with both enchantments out) but the false-direction sentence is **GONE, 0 recurrences**; D76 Lantern **PASS 0/6 — the lane-P render fix converted 5-of-5 takes into 6-of-6 declines with no guide change**, enchantment half FAIL 1/3; D77 plan board-claims **PASS 0 + 0**; D78 land drop **52/52**; D79 Tribute at zero **FAIL 2/39 — both in ONE turn** (#198), named-victim **PASS 3/3** incl. eating a 15/15 Emrakul; D80 dead Tutor **FAIL 2/22**; D81 blockers **PASS 0/14** with one opposite error at 4 life; D82 `{paying this taps:}` **PASS 27/27 + 0/27 — wave-52 engine #2 CLOSED** | **D15**, skill #195/#196/#197/#198 |
| **130** (2-4) | #0 draw punishers **PASS 0/34** (third corpus); #1 cycling **PASS 12/12** (was 11/13); #2 face damage above the floor **FAIL 4** (was 6) and **three lethal windows declined in one game** — the new failure and the lost game (#199); X-menu marker **PASS 2/2** (second corpus); land plan **FAIL 6 of 26** (was 8 of 19), two of them at an opponent on 1 life; Blastminer **PASS 0 self-targets**; Talisman life **PASS 0** at life <= 3 over 110 clause renders; second-copy Talisman 3 takes, correct; mulligan **0 ships in three corpora** -> UNTESTABLE-AT-THIS-SEAT (#200) | **D4**, skill #199/#200 |
| **146** (4-2) | P1 mulligan **PASS 0/N at 2+ lands** (was 3 of 4) — and it is the cross-guide CONTROL for #203; P2 empty-tag A-lines **PASS 0/44** (80 A-lines, 78 sent, 42 empty-tag, 0 missed); P3 the show-the-work ritual **15/165 raw, 15/26 live, protecting 0 misroutes -> DELETED** (#201); entry 5 keyed to a literal that renders **0 of 28** on its own card (#188) — ~20 declines of a free body-plus-a-card across turns 29-83; cleanup order **0 of 3** with the right card on all three lists, filed 500 lines from its ask (#190) | **D5**, skill #188/#190/#201 |
| **152** (2-3 +1 adj) | P4 `{paying this taps:}` **FAIL 2** (was 3 of 10) — both survivors are ONE-name clauses on body casts in a won game, i.e. the floor is over-broad (#204); P5 `ATTACK: none` **PASS 0/22** and the mirror opened (the partial hold, 3 windows, 6 A-lines — #202); P6 `[legendary:` **PASS 0/6**; P7 blocks **PASS 0/0/0**; the mulligan floor broke a THIRD corpus on the same shape, and the passing guide's sentence is the fix (#203) | skill #202/#203/#204 |
| **162** (4-2) | P8 second-Mine brake **PASS 0/N at K=0 and 9/9 at K>=1** — clean — **and the edit that closed it opened its mirror: 11 first-Mine over-brake windows across three games** (was 1), delaying the engine four turns twice (#206); the deck's one loss with a window is a survival gate sitting at the block window instead of the cast window (#205); #186's citation count worked again (the Master-of-the-Feast carve-out met at K=1 and K=2, declined at K=0). **NO LONGER exit-eligible** | skill #205/#206 |
| **pool** | wave-52 boundary B1-B9: **B3's three-number obligation was honoured by all seven guides this wave** and it immediately paid — deck146's entry-5 literal was found unsatisfiable by it (#188). B4's D6 re-key landed (0 old-string renders on a defender). B5's D11 re-key landed (0 dead-form takes). B8's deck130 holdout resolved to UNTESTABLE (#200) and deck123's twelve hands were completed. B9's watch (A52-12 drifting into "do not answer these windows") did NOT happen: 234 of 240 declines correct at that seat | skill **#187/#188** |

---

# CARRIED — open, re-measured, not re-derived
See D15-D38. The wave-53 OPEN questions and their answers: "does the HOLD row get taken" — **yes,
29 times, and it cannot be honoured** (D2); "does the timeout arm fire" — **no window, 0 empty
replies** (D10 discharge); "does the kill summary change behaviour" — **169 rows, 7 takes, 0
emitter disagreements, and one lethal row it could not describe** (D4); "is the `[second copy:`
verdict enough" — **yes, 6/6 with no guide change** (D11 discharge); "did the log cap hold" — **the
cap works harder and the tail grew** (D32).

---

# FALLBACK CLASSES — 15 / 3,483 = 0.43%; Baka-executed 4 = 0.11%

| n | class | records | disposition |
|---|---|---|---|
| 8 | `plan_choice_conflict` (lane J's D14) | `146v125` 150, 371, 392, 398, 470, 491, 555; `123v126` 22 | **8/8 recovered** — 5 `plan_choice_conflict_recovered`, **3 `plan_choice_conflict_exhausted`** (the re-ask repeated the same take and the lane honoured it). **5 of the 8 are the HOLD-row vocabulary collision** and one (`123v126` 22) is a true false positive; all 8 carry `coded_answers: 1`, so `latched_coded_line` had no window. Routed to **D2(b)**; the wave-52 rejection of a general narrowing STANDS |
| 3 | `unparsed_reply` — degenerate decode | `125v126` 365 (**202.5 s**, turn 55 Upkeep, a multilingual decode), `152v123` 29 and 37 (3.7 s / 3.3 s, both turn 11 Blockers, both on the byte-identical Vampire menu of **D8**) | **Baka executed all 3**; each followed by a `recovery` record; the next window answered normally |
| 3 | `named_row_reask` — in-range index + off-menu name | `126v125` 9 and `130v146` 15 (`named_row_not_offered`), `162v146` 13 (`stale_echo_in_range`) | **3/3 `named_row_reask_recovered`**; lane C working |
| 1 | `retracted_choice` — **new class, working** | `146v125` 313 (turn 56 Upkeep): `CHOICE: 1 (becomes beholder with Hive of the Eye Tyrant)` followed by 200 words ending "So I must PASS"; `choice: -1`, `commit_retracted: true` | **Baka executed**; the `recovery` record at 314 shows it then did row 1 anyway (`- Paid {3}{b} for Hive of the Eye Tyrant … / - You used: becomes beholder …`) — a clean new observability win |
| 0 | `empty_reply`, `timeout`, `stale_livelock`, `multiblock_reask` | | all four at zero; `multiblock_reask` was 3 last corpus and `empty_reply` 3 |
| — | **not a fallback but executed wrong**: `126v125` seq 13 and 14 (index AND name agreed, a third row executed) | | **D13** — 2 / 3,253 parentheticals, AT the ceiling |
| — | **not a fallback and not seen by this lane**: `152v125` seq 28, `choice: -1` with no `fallback` field | | **D7** — the hang record earns no `recovery` |

---

# PROMPT CHARS PER DECISION KIND — wave 52 -> wave 53 (20/21 games; `152v125` excluded)

| kind | n52 | max52 | **n53** | mean53 | p50 | p90 | **max53** | p90 lat (s) | max lat (s) |
|---|---|---|---|---|---|---|---|---|---|
| ask | 2,540 | 29,581 | **2,754** | 14,245 | 12,945 | 26,175 | 30,146 | 45.0 | **868.7** |
| priority | 535 | 31,551 | **519** | 15,847 | 13,844 | 28,028 | **32,068** | 48.7 | 319.3 |
| attackers | 107 | 30,277 | **120** | 12,583 | 10,245 | 25,843 | 28,743 | 64.5 | 193.1 |
| discard | 54 | 28,071 | **34** | 16,309 | 15,964 | 26,925 | 28,587 | 37.8 | 51.5 |
| blockers | 41 | 30,307 | **30** | **9,541** | 8,564 | 16,052 | 22,499 | 87.6 | 100.6 |
| reveal | 15 | 26,658 | **21** | 13,740 | 13,344 | 17,186 | 25,312 | 75.2 | 78.2 |
| bottom | 8 | 2,667 | **5** | 2,229 | 2,236 | 2,341 | 2,341 | 96.1 | 96.1 |

Overall: mean 21,665 ms, median 9,508 ms per decision; **12** prompts exceed 30,000 (was 7), **588**
exceed 25,000 (was 330), **476** carry a trim header (was 165). The `ask` mean is up ~13% while the
max is flat — the growth is the log. `blockers` prompts are **34% smaller** than wave 52 (9,541 vs
14,350): lane M's leaner blocker render is the visible win. Six decisions exceeded 600 s (**D9**) and
none reached the wall.

---

# LATEGAME SPECIMENS — invariant 0 (for the owner's per-wave prompt review)

**Harvest default, KEPT as the exhibit** (already written to `wave53/lategame-specimen.txt`):
`1788334427-ai_baka_deck125-0x55be9ff4b380-vs-ai_baka_deck146.jsonl` seq **357**, turn **84**, Main
phase 1, **28,481 chars**, **6 rows**, 33 life vs 14 — the deepest DECISION turn in the corpus and
the decking endgame (`Opponent library: 0 cards`). It carries the `#1-#4 x4` battlefield collapse,
`{X pricing: max affordable X=20 (23 mana total); each point of X gains you 1 life and draws you 1
card}`, three dead sweeper rows each honestly priced (`{right now: destroys 0 of their creatures …,
0 of yours}`), the PLAN age stamp, the log-trim header with its graveyard/exile inventory, and
`{card text:}` on five rows. **84.2% of it is the game log** (23,974 of 28,481), which is itself the
D32 exhibit.
**Recommended alongside it — the render-dense companion the engine seat wrote this wave**
(`wave53/lategame-specimen-render-dense.txt`):
`1788334424-ai_baka_deck125-0x55c56d4af610-vs-ai_baka_deck126.jsonl` seq **66**, turn **22**, Draw,
**10,412 chars**, **4 rows**, 27 life vs 20. Verified on disk: it carries **ten** annotation families
to the default's seven — the **HOLD row**, `[you declined this exact list N times already this
turn]`, the PLAN age stamp, `{X pricing:}`, `{spends K of M}`, the `LIFE-TO-DAMAGE CONVERTER` block,
the `#a-#b` collapse, `{card text:}`, `{target text:}` and a `- Paid` receipt — and it is **the only
single screen in the corpus carrying all three of this wave's new forms** (the HOLD row, the
declined-count note, the plan age stamp) at a size a human reads end to end. That is what the owner's
criterion (decision value per token) is measured on.
**The size ceiling, if he wants the worst case:**
`1788334464-ai_baka_deck146-0x55629e998440-vs-ai_baka_deck123.jsonl` seq **92** (32,068 chars, turn
33, priority) — the corpus maximum and the exhibit for D32.
**The defect specimens** (for the lanes, NOT for the prompt review):
`152v125` seq **26-28** + `game-152v125-1788334434.stderr:1097-1214` (D1, the hang) ·
`130v162` seq **62-65** (D4, the lethal row that printed `kills 0`, and the target ask that printed
`SURVIVES` on two creatures and nothing on the player) · `130v125` seq **181-186** and **189-194**
(D6's `your own effect` on the opponent's Emrakul, D8's fourteen uncollapsed Mountains, and D25's
battlefield-ordered list — three items in one arc; **seat-123-130 proposed this as its prompt-review
specimen and it is the right EXHIBIT for those three lanes but the wrong one for invariant 0**: six
20 KB prompts at ~99% log to pick a Mountain shows the engine at its worst, not the render at its
most informative) · `146v125` seq **520-560** (D2 and D5 in one arc) · `152v123` s23-s37 (D8's
fourteen identical-option asks) · `125v162` seq **79-88** (D3's eight lethal declines — the wave's
best result, and worth keeping as the positive control).
