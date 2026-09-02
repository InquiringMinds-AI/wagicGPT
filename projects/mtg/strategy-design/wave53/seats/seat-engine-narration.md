# Wave-53 seat review — ENGINE / NARRATION

Corpus `matchups-20260902-023342` (binary = hermetic `make -B` of merge head c628dd531; lanes M 614312289,
O 0ba5ae2eb, N 3ab5c27c4, Q ba33a9adb, T 00549b391, S 6a4d5e119, P 1057c7c9d, R 1bb08b2b1). Model
qwen36-35b-a3b on Spark :8084. **42 seat logs, 3,572 records** = 42 `gamestart` + 40 `gameend` +
**7 `recovery`** (the NEW kind) + **3,483 decisions** (ask 2,754 / priority 519 / attackers 120 /
discard 34 / blockers 30 / reveal 21 / bottom 5). **20/21 natural; game 152 v 125 HUNG** (HIGH #1 below)
and is excluded from every rate. Max seat **607** records (`deck146-vs-deck125`, an 84-turn game). Max
prompt **32,068** (`deck146-vs-deck123` seq 92, priority, turn 33). Total inference **20.96 h**.

Every string quoted is the emitter's ACTUAL output, read with python over the JSONL (`prompt`,
`options_text`, `reply`, `chosen_text`, `events`) and `/usr/bin/grep` over the `.stderr` files. `seq` is
the record's own field. Narration counts are DEDUPED per record from `events`. Card facts checked against
`projects/mtg/bin/Res/sets/primitives/*.txt` — Sigarda, Champion of Light; Ranger Class; Path to Exile;
Overgrown Battlement — plus `bin/Res/sets/*/_cards.dat` for the image ids in the hang trace. None
contradicted a render except where called out (D-2).

---

## HIGH #1 — the hang: game 152 v 125, reconstructed end to end

**Verdict: an engine hang in the interactive-reveal driver, on the ONE path where option one has zero
legal targets. Not a model loop** — the last model call in either seat completed 43.8 s before the freeze,
and the last 13 h of the process are a render loop with no engine line at all.

### The exact sequence (deck152 seat log seq 26-28 + `game-152v125-1788334434.stderr:1097-1214`)

| stderr | what happened |
|---|---|
| 1097 | `AIPlayerGPT: declared attack (4 of 4) in one reply` — seat log **seq 26**, turn 15, attackers `A1..A4` = Elite Spellbinder, Moonrage Brute, Sigarda, Wolf; reply `ATTACK: A1, A2, A3, A4`, 13 power vs 9 life |
| 1098 | `Action added to stack: StackAbility.  (Source: Sigarda, Champion of Light)` — the **Coven attack trigger** (bottom of stack) |
| 1099 | `Action added to stack: StackAbility.  (Source: Ranger Class)` — the **level-2 attack trigger** (top of stack) |
| 1101, 1107(doReactTo 148) | Ranger Class resolves and asks for its target: seat log **seq 27**, kind `ask`, phase **Blockers**, 4 rows (the four attackers), reply `CHOICE: 3 (Sigarda, Champion of Light)`, 43.8 s. stderr: `TARGET CHOICE for Ranger Class - its "put a +1/+1 counter" ability … -> chose 3 of 4` then `targeting with Ranger Class -> 1 target(s), first: Sigarda, Champion of Light` |
| 1105-1106 | Ranger Class's counter resolves. **This is the last thing that ever completes.** |
| 1107 | `Resolving Action on stack: StackAbility.  (Source: Sigarda, Champion of Light)` — the Coven `reveal:5` opens |
| 1108-1145 | the reveal display renders its five cards: `ZNR/491910` ×2, `KHM/503867`, `DBL/296863`, `AFR/527489` = **Branchloft Pathway ×2, Barkchannel Pathway, Fateful Absence, Ranger Class** (ids resolved from `bin/Res/sets/*/_cards.dat`). **Not one is a Human creature card.** |
| — | seat log **seq 28**, kind `reveal`, phase Blockers, `options: 5`, `choice: -1`, `chosen_text: "none (no legal target)"`, `latency_ms: -1`, **prompt 0** — the engine answered for the seat with **no model call** |
| 1146 (doReactTo 149) | `Action added to stack: StackAbility.  (Source: Sigarda, Champion of Light)` — option TWO ("put on bottom") re-armed |
| 1214 | `Resolving Action on stack: StackAbility.  (Source: Sigarda, Champion of Light)` — resolves with **no card selected**, so nothing leaves the reveal zone |
| 1215-1262 | the same five thumbnails re-render, then **nothing** for ~13 h until the harness killed it (`adj0`, 20/9, turn 15). No `gameend` on either seat. |

### The seam that never returned

Primitive (`borderline.txt:102246`):
`auto=@combat(attacking) source(this) restriction{coven}:name(Look top 5 cards) reveal:5 optionone name(Get a human) target(<upto:1>human|reveal) moveto(hand) optiononeend optiontwo name(put on bottom) target(*|reveal) bottomoflibrary and!( all(*|reveal) bottomoflibrary )! optiontwoend revealend`

Read-only trace of the two code sites (I did not build or run):

1. `MTGRevealingCards::toResolve()` (`src/AllAbilities.cpp:446-478`) picks the branch **up front**: option one's chooser `<upto:1>human|reveal` has `countValidTargets() == 0`, so it takes the else branch — `repeat = false; abilitySecond = contructAbility(abilityTwo); fireOneShot(abilitySecond);` — and **`abilityFirst` is never constructed**. The chooser now live belongs to option TWO (`*|reveal`, accepts everything) and `ownChooser()` returns it because its source is the same card.
2. `MTGRevealingCards::driveInteractiveReveal()` phase 0 (`src/AllAbilities.cpp:723-928`) sees that non-NULL chooser and proceeds as if it were option one's. It rebuilds eligibility from **abilityOne's** predicate (the N-136a fix), gets `eligCount == 0`, and `AIPlayerGPT::decideReveal` (`src/AIPlayerGPT.cpp:24366-24375`) takes the deliberate no-ask shortcut: it writes the `reveal` record and returns **-1**, documented as "the display then sends nothing to option one - the safe keep-on-top default". Phase 0 then clicks nothing, sees `tcAfter` still non-NULL, issues `CheckUserInput(JGE_BTN_NEXT)` **with zero targets selected**, and sets `mAIPhase = 3`. Phase 3's first two waits are skipped (`abilitySecond` is set), `zone->cards.size()` is 5, `if (!abilitySecond)` is false, and it falls to **`if (!tc) return;`** — "option two built but its chooser has not armed yet". The chooser was consumed by the BTN_NEXT and will never arm again, so phase 3 returns forever. `mAIDriveDone` stays false; the reveal display stays open; nothing else can advance.

`REVEAL_DBG` is `getenv("WAGIC_REVEAL_DEBUG")`-gated (`AllAbilities.cpp:644`), which is why **the entire hang leaves zero stderr**.

**Why it is a live, common hazard, not a freak.** The corpus contains **5** Sigarda Coven `reveal:5`
windows at Blockers. Four (`deck152-vs-deck126` seq 22, 27, 33, 41) found a Human, asked the model, and
resolved normally. The fifth found none and hung: **1 in 5 Coven triggers, 1 in 21 reveal windows**. Any
predicate-gated reveal whose fail branch is itself targeted (`optiontwo target(*|reveal) …`) is exposed.

**Fix shape (engine, D-lane).** (a) In phase 0, when `abilityFirst == NULL` the armed chooser is option
TWO's — do not treat it as option one's and do not "finalize" it empty; go straight to the phase-4
remainder click loop and sweep every revealed card into option two. (b) Give phase 3 a bounded wait: if
`abilitySecond` is set, the zone is non-empty and no chooser has armed for K ticks, re-arm (BTN_NEXT) or
declare `mAIDriveDone` and let the display close. (c) `decideReveal`'s -1 contract says the display sends
nothing to option one — that contract is only safe when option two is untargeted; make the empty-eligible
path **select the whole remainder for option two** rather than answering nothing.

**Is the deck125 seat's Path to Exile part of it?** **No.** deck125 seq 16 (`Cast Path to Exile`) and
seq 17 (target `Sigarda, Champion of Light … [opponent's battlefield]`) are turn **12**, three turns
before the freeze, and they resolved cleanly: the deck152 seat's seq 19 `events` read
`- Opponent cast Path to Exile / - Your Sigarda, Champion of Light was exiled from the battlefield /
- Opponent's Path to Exile resolved`, and seq 20 (turn 13) is deck152 **re-casting** a second Sigarda,
which is the one that attacked on turn 15. The Path's "may search for a basic land" half is the deck152
seat's seq 19 ask and it completed (`- Your Plains entered the battlefield from your library`). Its only
connection to the hang is causal-at-a-distance: it forced the re-cast that produced the turn-15 attack.
It IS, separately, the subject of D-2 below.

**Card-script fidelity note (not the hang).** Ranger Class's level-2 trigger is scripted
`auto=@each my blockers restriction{compare(hascntlevel)~morethan~1}:name(Put 1/1 counter) target(creature[attacking]|battlefield) counter(1/1)` (`borderline.txt:89050`) while its Oracle text is
"Whenever you attack, put a +1/+1 counter on target attacking creature." `@each my blockers` fires the
trigger at the **Blockers** step, i.e. after blockers are declared, instead of on attack declaration —
which is both a rules error and why seq 27/28 are stamped `phase: Blockers`. Card-script layer, not
engine; flagged here because it is on the hang's path and both records' phase field looks wrong.

---

## Health — the 15 fallbacks, each classified

`fallback` **15 / 3,483 = 0.43 %** (wave 52: 15 / 3,300 = 0.45 %). **Baka actually executed 4 decisions**
(`chose -1 of N` in stderr, 4 occurrences = **0.11 %** of decisions; wave 52 0.18 %) — under the ≤0.20 %
carry. `giving this decision to the heuristic` **0**; `stale_livelock` **0**; `consecutive stale drops`
**0**.

| class | n | root cause | Baka? | recovered |
|---|---|---|---|---|
| `plan_choice_conflict` | 8 | **lane J D14 firing, but 5 of the 8 are a NEW lane-N collision**: the reply takes the **HOLD row** and the PLAN prose then says "I must pass" — the detector reads that as a reversal. `deck146-vs-deck125` 150, 371, 398, 470, 491 (all `CHOICE: 4/5 (Hold priority)`); the other three are ordinary prose reversals (`deck146-vs-deck125` 392, 555; `deck123-vs-deck126` 22) | no (choice 1/4/5) | **8/8** — 5 `plan_choice_conflict_recovered`, **3 `plan_choice_conflict_exhausted`** (seq 399, 471, 492 — the re-ask repeated the same HOLD take and the lane gave up and honoured it) |
| `unparsed_reply` | 3 | model degeneration. `deck125-vs-deck126` seq 365 (**202.5 s** latency, turn 55 Upkeep), `deck152-vs-deck123` seq 29 and 37 (3.7 s / 3.3 s, both turn 11 Blockers) | **yes ×3** | `recovery` record each; the next window answered normally |
| `named_row_reask` | 3 | in-range index + off-menu name. `deck126-vs-deck125` seq 9 and `deck130-vs-deck146` seq 15 (`named_row_not_offered`), `deck162-vs-deck146` seq 13 (`stale_echo_in_range`) | no | **3/3** `named_row_reask_recovered` |
| `retracted_choice` | 1 | **new class, working.** `deck146-vs-deck125` seq 313 (turn 56 Upkeep): `CHOICE: 1 (becomes beholder with Hive of the Eye Tyrant)` followed by 200 words ending "So I must PASS". `choice: -1`, `commit_retracted: true` | **yes** | `recovery` seq 314's `recovered_by` shows Baka then **did row 1 anyway**: `- Paid {3}{b} for Hive of the Eye Tyrant … / - You used: becomes beholder with Hive of the Eye Tyrant` |

**NO `empty_reply` and NO `timeout`.** `retry` field: **0 records**. The closest approach to the 900 s
wall was **868,729 ms = 96.5 %** (`deck126-vs-deck125` seq 13) and it **answered**. So lane Q's timeout
arm (`fallback: "timeout"` + `retry: 1` + the two stderr lines) is **UNTESTED — N = 0 windows reached the
wall**, not PASS. `no reply after` in stderr: 0. Wave 52's `empty_reply within 1 s of the wall` 3 → **0**.

`parse_note` 35: `decision_reversed_in_prose` 8, `plan_choice_conflict_recovered` 5,
`named_row_not_offered` 4, `named_row_reask_recovered` 3, `attack_last_line_taken` 3,
`decision_reversed_in_prose;plan_choice_conflict_exhausted` 3, `echo_index_conflict;name_over_index` 3,
`multi_answer_first_taken` 2, **`multiblock_first_wins` 1**, `blocks_last_line_taken` 1,
`echo_index_conflict` 1, `stale_echo_in_range` 1. `answer_replaced` 12, `commit_retracted` 1.
stderr: `dropping stale async answer` 63 (never consecutive), `auto-passing without a model call` 2,528,
`all actions pass-declined` 809, `land-drop ask NOT issued` 977, `cast ask NOT issued` 257 over
`repeat plan iteration` 60, `with no source tapped` **0**, `index_name_conflict` 0, `plan_missing` 0.
`mana_only_windows_skipped` sum 406, max 276 (`deck126-vs-deck125`).

### The 7 `recovery` records — lane Q D24 verified N/N

Exactly **7** records carry `kind: "recovery"`, and exactly **7** decision records carry `choice: -1`
together with a `fallback` (seq 365 / 9 / 313 / 15 / 29 / 37 / 13). **7 / 7**: every one is followed at
`seq + 1` by exactly one `recovery` record whose `recovers_seq` points back at it, carrying
`recovers_kind`, `recovers_fallback` and `recovered_by` (the events the recovery actually produced).
The 8 `plan_choice_conflict` records have `choice != -1` and correctly get **no** recovery record.
The kind carries no `prompt`/`latency` and must not be counted as a decision.

### Reply-vs-executed-row audit

3,253 replies carried a `CHOICE: n (name)` parenthetical; **25** disagreed with the executed row's label.
Classified: 6 are cosmetic (the name is a correct prefix/suffix of a longer row label — `Plains - "W"` vs
`Plains [land] [your library] - "W"`, `Level Ranger Class` vs `Put a level counter with Ranger Class`);
15 are prose reversals the engine correctly resolved to the last coded line (`Cast Damnation` → `Cast
nothing`, `becomes beholder` → `pass`); **2 are the HOLD-index degradation (D-1 below)**; **2 are genuine
mis-executions** (`deck126-vs-deck125` seq 13, `CHOICE: 2 (Cast Idyllic Tutor)` on a 3-row menu executed
row 1 `Cast Perimeter Captain` — `coded_answers: 2`, `latched_coded_line: "2"`, no note; and its
neighbour seq 14). Rate 2 / 3,253 — at the carry's `≤ 1 / 3,000` ceiling, not under it.

**Wave-52 D-3 is closed.** 284 replies wrote `CHOICE: 0 (Pass)` / `(pass)` / `(Pass priority)`; **284/284
executed row 0**. Zero in-range non-zero index carried a pass parenthetical. The predicted
`pass_row_named` note never renders — the behaviour is right, the observability tag is missing.

### Narration

`turn` == the last `=== Turn N` header in the prompt **3,429 / 3,429**. `- Paid` receipts **496** deduped
from `events`; **0 with no source** (11 lack a ` with ` clause and all 11 are the legitimate
`from mana already floating` form); **2 with no payee**, both the Intrepid Adversary
pay-any-number-of-times ETB (`deck152-vs-deck126` seq 25, `deck152-vs-deck162` seq 29) — wave 51 had 7,
wave 52 had 3, the class is narrowing but not closed. Battlefield headers naming a land count:
**3,429 / 3,429**. Duplicate event lines inside a single record: **571** (wave 52: 280) — `- Phase: Draw`
237, `- Opponent drew a card` 116, `- Phase: Main phase 1` 71 lead it; the `(xN)` collapse still does not
reach phase lines, draws or loyalty counters (D-6).

### Mulligans (new-baseline record)

**37 of 42 seats kept 7.** 5 seats mulliganed: 4 to 6 cards (`deck146-vs-deck125` bottomed
`Agadeem's Awakening`; `deck152-vs-deck126` bottomed `Lair of the Hydra`; `deck146-vs-deck152` bottomed
`Silverquill Command`; `deck162-vs-deck125` bottomed `Liliana's Caress`) and 1 to 5
(`deck152-vs-deck162`, two mulligans, bottomed `Barkchannel Pathway, Barkchannel Pathway`).
**No mull-to-zero, no repeated mull against a guide floor, and — unlike wave 52 — no mulligan decided by
the heuristic** (0 timeouts). deck130 shipped **0** mulligans in this corpus, so its mulligan section
stays **#132-UNTESTED** (three corpora now).

---

## VERDICT TABLE — Set B, adjudicated by the emitter's actual strings

| Lane | Prediction | Verdict | Count / citation |
|---|---|---|---|
| M 614312289 | `multiblock_reask` records with no follower and no `chosen_text` **0/N** (was 1/3) | **PASS (0/30)** | All 30 `blockers` records carry a `chosen_text`; `multiblock_reask` as a fallback class does not appear at all |
| M | every blockers ask followed in stderr by exactly one `declared blocks from N assignment(s)` | **PASS (30/30)** | 30 `blockers` records ↔ 30 `declared blocks from` lines, 1:1, including the 5 `no blockers` answers. `one-blocker-many-attackers reply -> re-asking once` fires **once** and is followed by its declaration |
| M | new note `multiblock_first_wins` where a conflicted reply held a legal pairing | **PASS (1/1) — first live firing** | `deck152-vs-deck130` seq 16, turn 12: reply `BLOCKS: B1:A1, B1:A2` → `dropped_assignments: 1`, `parse_note: multiblock_first_wins`, executed `Sigarda, Champion of Light blocks Siege-Gang Commander`. Wave 52's D-1 (zero blocks declared, game lost) does **not** recur |
| M | `blocker_handle_in_attacker_slot` noted | **UNTESTED** | No `Bn:Bm` assignment arrived in 30 blockers replies; `dropped_assignments` is 0 on 29 of 30 |
| M | BLOCKS format line carries the one-per-blocker clause | **PASS (30/30)** | `…e.g. "BLOCKS: B1:A2, B3:A1, B2:none" - each B-number at most ONCE, and several B-numbers may share one A-number - or exactly "BLOCKS: none"…` |
| N 3ab5c27c4 | opponent-turn casting windows per game **down ≥ 30 %** | **FAIL (up 19 %)** | Counting records whose prompt carries `Casting decision (<phase>, opponent's turn): which card do you cast now`: **1,052** over 21 games = 50.1/game vs wave 52's 884/21 = 42.1. Confound stated honestly: turn counts are far higher this wave (one 84-turn game supplies 442 of the 1,052), but the raw prediction fails on its own terms |
| N | opponent-turn casts **≥ 90 %** of 68 | **FAIL (58 = 85.3 %)** | Main phase 1 54, Upkeep 2, Draw 1, Attackers 1 |
| N | End / Cleanup / Attackers / Combat-ends contribute **0** casts | **PASS 3 of 4, FAIL on Attackers (1)** | End 80 windows / 0 casts; Cleanup 79 / 0; Combat ends 80 / 0; **Attackers 82 / 1** — and that one is a *good* cast: `deck125-vs-deck152` seq 15, turn 9, `Cast Path to Exile` on the attack step. Main 2 87/0, Combat begins 83/0, Blockers 19/0, Combat damage 17/0. **Six phases, 365 windows, 3,269 s of inference, zero casts.** |
| N | consecutive identical-declined runs **< 10 %** of decisions (was 23.0 %) | **FAIL (26.0 %)** | 906 / 3,483 decisions sit in a run of 3+ consecutive byte-identical declined menus within one turn; run lengths reach **17**. Method: key = (turn, `options_text` tuple) on a chosen_text starting `Cast nothing` / `pass` / `Hold priority` / `Play no land` |
| N | FALSIFIER: `hold_windows_skipped` ~0 (row never taken) | **falsifier NOT met — the row fires** | HOLD row offered in **1,173** windows, **always the LAST row (1,173/1,173)**, **taken 29 times** (2.5 %), producing `hold_windows_skipped` **102** — but concentrated in 2 seats (`deck146-vs-deck125` 88, `deck130-vs-deck125` 14); the other 4 seats that took it saved **0** windows. See D-1: adoption, not the mechanism, is the problem |
| N | `CHOICE: n (Pass)` on a `0. Pass priority` menu executes row 0 **N/N**; note `pass_row_named` | **PASS on behaviour (284/284); the note never renders** | See the audit above. 0 records carry `pass_row_named` |
| N | PLAN blocks without an age stamp **0/N** | **PASS (3,329/3,329)** | Two forms only: `, N window ago on turn N` (2,375) and `, N windows ago on turn N` (954). Max `plan_echo_count` **43** (wave 52: 51) |
| N | `plan_choice_conflict` records with `coded_answers ≥ 2` carry `latched_coded_line` | **UNTESTED** | All 8 `plan_choice_conflict` records have `coded_answers: 1`. The field itself works: **39** records carry `latched_coded_line`, and 39 of the 43 records with `coded_answers ≥ 2` have it (the 4 without are `attackers`/`blockers` kinds, which use their own parsers) |
| O 0ba5ae2eb | `[DRAW PRICE:` rows with damage ≥ life carry `this KILLS you` **N/N** (was 0/41) | **PASS (16/16)** | 85 `[DRAW PRICE:` rows in 71 windows; 16 carry the lethal tail, e.g. `deck125-vs-deck162` seq 79/80 on `Cast Sphinx's Revelation … DRAW PRICE …`. Old strings without the tail at ≥-life damage: **0** |
| O | casts from such rows **0** | **PASS (0/16)** | The single `[DRAW PRICE:` take corpus-wide is `deck125-vs-deck162` seq 78 `Cast Essence Scatter` on a non-lethal row |
| O | targeted-spell cast rows with knowable fixed damage / −N/−N carry a kill count or victim **N/N** (was 0/85) | **PASS (169/169)** | 129 rows `{kills 0 of the N legal target(s) at …}` + 40 `{kills: <names>}` (e.g. `deck130-vs-deck123` seq 24 `Cast Spark Spray … {kills: Goblin #1, Goblin #2, Goblin #3}`). Of the 413 creature-targeting removal cast rows, the 244 without a verdict are all **unconditional** removal (Path to Exile 224, Fateful Absence 15, Vanishing Verse 5) where "kills at N damage" is inapplicable — see D-4 for the residue |
| O | removal cast where every target survives **0** | **FAIL (4)** | 4 takes of a `{kills 0 of the …}` row: `deck123-vs-deck130` seq 64 (Tragic Slip), `deck130-vs-deck123` seq 47 (Spark Spray), `deck130-vs-deck125` seq 127 and `deck130-vs-deck162` seq 62 (Hammer of Bogardan). The render was correct and in front of the model each time — a deck-seat call, except seq 127/62 where the render itself under-counts (D-4) |
| O | defender-naming tap rows printing `cannot attack` **0/N** (was 22); `cannot block on their turn` **N/N** | **PASS (0 defender-naming; 27 new-form)** | New string: **27** rows, all `{paying this taps: Overgrown Battlement - it cannot block on their turn}` (primitive `mtg.txt:83597` `abilities=defender`), **8 taken**. Old string still renders **14** times but names only **non-defenders** — Katilda, Dawnhart Prime (9), Luminarch Aspirant (2+2 shared), Elite Spellbinder (1) — for which `cannot attack this turn` is the correct clause. Three-number audit: old-string renders on a defender **0**, new-string renders **27**, takes **8** |
| O | converter drain folded onto `they gain N` rows | **UNTESTED (0 renders)** | The literal ` - and their <name> takes N off YOU: life A -> B` renders **0** times: no edict/gain row coincided with a Sanguine-Bond-class converter on the board this corpus. Old-string renders 0, new-string renders 0, takes 0 |
| O | CARRY: fight rows arrive payload-stripped, no forecast expected | **holds** | No `powerstrike eachother` window arose |
| P 1057c7c9d | bare `ability: <effect>` stack lines **0/N** | **PASS (0/708)** | 708 emitted stack lines, every one in the new form `ability: <source>'s <effect> [from your/their <source>] [triggered/activated ability]`; 9 carry the aim clause, e.g. `ability: Path to Exile's put a card onto the battlefield (aimed at Plains) [from your Path to Exile]`. (Two apparent "bare" hits are the model's own prose echoed back inside a PLAN block, not emitter output.) Wart: 53 lines read `ability: Ob Nixilis, the Hate-Twisted's Ob Nixilis, the Hate-Twisted` — the effect label degenerates to the source name (D-5) |
| P | edict rows naming a permanent already on the stack without the such-clause **0/N** (was 2/3) | **UNTESTED (0 renders)** | ` - a sacrifice is already on the stack aimed at this permanent` renders 0 times; no second edict aimed at a permanent already under one arose. Old-string renders 0, new 0, takes 0 |
| P | no ask menu > 12 rows collapsing to ≤ 2 shapes | **FAIL (3)** | `deck123-vs-deck126` seq 30 (26 rows, 2 shapes, a Vampire target list), `deck130-vs-deck125` seq 69 (**17 rows, 1 shape** — 17 library `Mountain [land] [your library] - "R"`), `deck162-vs-deck125` seq 26 (14 rows, 2 shapes, library Swamps). Both library cases are D-2's mis-framed Path search. Separately the wave-52 D-5 class survives in a new costume: `deck125-vs-deck126` seq 220 offers **28** rows of `Deal 1 damage with Staff of Nin #N targeting …` (also seq 189/213 at 26) |
| P | max non-log segment **< 12 KB** | **PASS (11,231)** | Largest post-log segment: `deck126-vs-deck162` seq 9, a 50-row reveal. Log segment max 24,107, p50 9,107; **476** prompts carry `(...earlier events trimmed…)` (wave 52: 165) |
| P | `{castable from exile}` naming its source **17/17** (was 4/17) | **PASS on the corpus's N (2/2), tiny sample** | Only **2** `{castable from exile}` rows exist this corpus and both read `{castable from exile - your card, exiled by their Elite Spellbinder, which lets you cast it from there; it costs {2} more…}`. The causeless fallback form renders **0** times. D6's durability is not really re-tested at N = 2 |
| P | `[second copy:` takes on non-stacking scripts **0** (was 4/14 Intruder Alarm) | **PASS (0/1)** | 86 `[second copy:` rows, 12 takes, all on genuinely stacking permanents (Staff of Nin 3, Talisman of Impulse 3, Underworld Dreams 2, Howling Mine 2, Sanguine Bond 1, Pyrite Spellbomb 1). The one Intruder Alarm row (`deck123-vs-deck126` seq 38) carries the new tail — `…both stay on the battlefield - no legend rule, but its effect is already on the battlefield and a second copy changes nothing` — and was **not** taken |
| P | Lantern never called dead | **PASS (0/6)** | 6 Chromatic Lantern second-copy rows, all the stacking form, 0 takes |
| P | battlefield header counts lands | **PASS (3,429/3,429)** | e.g. `Your battlefield (27 permanents listed, of which 0 are creatures, 0 of them able to attack right now, and 25 are lands)` |
| Q ba33a9adb | empty replies at ≥ 95 % of the deadline stamped `fallback: "timeout"` + `retry: 1` + two stderr lines **N/N** | **UNTESTED (N = 0)** | 0 empty replies; 0 records carry `retry`; max latency 868,729 ms (96.5 % of the wall) and it answered. `fallback: "timeout"` renders 0 times. **Not a PASS** — the arm never had a window |
| Q | `empty_reply` within 1 s of the wall **0** (was 3) | **PASS (0)** | No `empty_reply` at all |
| Q | Baka-executed opening keeps from a wall miss **0** (was 2) | **PASS (0)** | All 42 opening keeps and all 5 mulligan chains are the model's |
| Q | every `choice: -1` + fallback record followed by exactly one `kind: "recovery"` record | **PASS (7/7)** | See above; `recovers_seq` chains verified |
| Q | D20 declined on measurement (0 adjacent runs) | **holds** | Nothing to test |
| T 00549b391 | aggregate-lethal SURVIVAL sweep in Baka's `chooseBlockers` | **UNTESTED in corpus** | **0** `blockers` records carry a `fallback`; the heuristic never made a block decision. Owner's Vita play is the test |
| S 6a4d5e119 | hand-refresh perf | **UNTESTED by construction** | Human-seat only; no corpus exposure |
| R 1bb08b2b1 | the `may pay({1})` pay-interrupt fix also affects Baka's `may` answers — report any `may`-prompt fallback | **UNTESTED** | 0 `may`-prompt fallback records; Go-Shintai and Dominating Vampire are not in the pool (checked the seven deck lists) |
| carry | wave-52 owner batches, Vita evolve, Baka pump-targeting fix 5ca11d6e2, option-preserving autotap c1a0bf814 | **UNTESTED** | Baka received 4 decisions, none of them a pump or ability target; no evolve/adventure/morph card is in the pool. `with no source tapped` **0** and no forecast-vs-receipt mismatch surfaced, so the autotap's AI-seat isolation still holds passively |
| carry | 20/21 natural; Baka-executed ≤ 0.20 % | **PASS on Baka (0.11 %); the 21st game HUNG** | See HIGH #1 |

---

## Prompt chars per decision kind — wave 52 → wave 53

| kind | n52 | max52 | **n53** | mean53 | p50 | p90 | **max53** | p90 lat (s) | max lat (s) |
|---|---|---|---|---|---|---|---|---|---|
| ask | 2,540 | 29,581 | **2,754** | 14,245 | 12,954 | 26,175 | 30,146 | 45.0 | 868.7 |
| priority | 535 | 31,551 | **519** | 15,847 | 13,844 | 28,028 | **32,068** | 48.7 | 319.3 |
| attackers | 107 | 30,277 | **120** | 12,583 | 10,372 | 25,843 | 28,743 | 64.5 | 193.1 |
| discard | 54 | 28,071 | **34** | 16,309 | 16,478 | 26,925 | 28,587 | 37.8 | 51.5 |
| blockers | 41 | 30,307 | **30** | 9,541 | 8,680 | 16,052 | 22,499 | 87.6 | 100.6 |
| reveal | 15 | 26,658 | **21** | 13,740 | 13,344 | 17,186 | 25,312 | 75.2 | 78.2 |
| bottom | 8 | 2,667 | **5** | 2,229 | 2,236 | 2,341 | 2,341 | 96.1 | 96.1 |

**12** prompts exceed 30,000 (wave 52: 7), **588** exceed 25,000 (wave 52: 330). The mean is up ~13 % on
`ask` while the max is essentially flat — the growth is the log, and the log cap is working harder
(476 trims vs 165). Every prompt above 29 KB is ~75-80 % log. The D23 log-compaction decision remains the
only lever on the tail, and it is the owner's. `blockers` prompts are **34 % smaller** than wave 52
(9,541 vs 14,350) — lane M's leaner blocker render is the visible win here.

---

## The two long games

| game | turns | seat records | winner | what kept it going |
|---|---|---|---|---|
| **146 v 125** | 84 | deck146 **604** decisions / deck125 362 | deck125, 39 to 14 | **Neither seat could kill the other, and only one could refill.** deck125 cast `Elixir of Immortality` 4 times and shuffled its graveyard back twice (`- You shuffled your graveyard (11 cards) into your library with Elixir of Immortality`, and a 5-card one); deck146's only clock was two `Hive of the Eye Tyrant` manlands at 3 power. deck146 spent **480 of 604 decisions (79 %) on no-ops** and 5,293 s of inference to lose. The game ended by **decking**: the seat's `Opponent library:` line goes 48 → 30 → 19 → 9 → 2 → **0** across turns 11-84, and the last events are deck146's Cleanup discards. This game alone contributes 442 of the corpus's 1,052 opponent-turn windows and 88 of its 102 `hold_windows_skipped` |
| **125 v 126** | 54 | deck125 371 / deck126 120 | deck125, 19 to 0 | A pure **Staff of Nin ping race** — `Staff of Nin` appears 87 times in the deck125 seat's events; the seat's two most common actions after "Cast nothing" are `Deal 1 damage with Staff of Nin #2 targeting the opponent` (16) and `#1` (15). deck126 answered **only 120 decisions in 54 turns** because `mana_only_windows_skipped` reached **276** — the highest in the corpus — so its Sorin/Emrakul recursion loop ran mostly without asks. The last event pair is `- Opponent's Emrakul, the Aeons Torn died / - Your Emrakul, the Aeons Torn entered the battlefield from your graveyard` |

The corpus-wide shape from wave 52 holds: **decision volume scales with permanent count, not turns**, and
the losing seat routinely burns 2-3× the winner's inference doing nothing.

---

## NEW / CARRIED DEFECTS (record + emitter string), ranked

**D-1 HIGH — `CHOICE: 0 (Hold priority)` silently executes a plain pass, and the HOLD row collides with
the D14 plan-conflict detector; net adoption 29 / 1,173 (2.5 %).**
The mechanism ships and works: 1,173 windows offer `Hold priority - do not ask me again this turn unless
the board changes`, always as the last row; 29 takes; 102 windows skipped. But three things eat it.
(a) **Index-0 degradation, 2/2 firings wrong.** `deck125-vs-deck146` seq 74 (2-row menu) and
`deck146-vs-deck125` seq 372 both reply `CHOICE: 0 (Hold priority)`; the engine takes index 0 = pass, the
name is ignored, **no note, no re-ask** — the hold never registers. This is exactly wave-52 D-3 inverted:
`0` is a reserved echo, so the name can never win. Fix: reserve the HOLD row's short name the way `pass`
is now reserved, and re-ask (or take the named row) when the two disagree.
(b) **D14 collision, 5 of 8 `plan_choice_conflict` firings.** Taking the HOLD row means "I do nothing this
turn", so the model's PLAN prose always says "I must pass"; the detector reads a non-zero index plus a
pass verdict and re-asks. `deck146-vs-deck125` 150/371/398/470/491. Two of those re-asks then degraded to
a plain pass (151, 372) and three hit `plan_choice_conflict_exhausted` (399, 471, 492). Each costs a full
extra model call. Fix: exempt the HOLD row from the pass-verdict conflict test — a pass verdict *confirms*
it.
(c) **Adoption.** In **703** windows the prompt already said `[you declined this exact list N times
already this turn]` with N ≥ 3 (N reaches **18**), and the pilot took HOLD in **3** of them. The row is
last on the menu, phrased as an instruction to the engine rather than as an action, and carries no cost
statement. Repro for the raw shape: `deck146-vs-deck125` seq 216-227.
Cost: the 1,173 HOLD-bearing windows plus the 365 zero-cast opponent-turn windows at End / Cleanup /
Combat-ends / Combat-begins / Main 2 / Blockers are **3,269 s** of inference that produced no action.

**D-2 HIGH (correctness of the surface) — Path to Exile's basic-land COMPENSATION search is rendered to
the victim as "EXILE ONE OF YOUR OWN CARDS … each option is a card YOU will LOSE". 8/8 firings wrong.**
Repro: `deck152-vs-deck125` seq **12** (turn 9) and seq **19** (turn 12); also `deck146-vs-deck125`
11/37/64/171, `deck130-vs-deck125` 69, `deck162-vs-deck125` 26. The emitted ask:
`EXILE ONE OF YOUR OWN CARDS: your own effect (Path to Exile) makes you exile a card from the list below -
each option is a card YOU will LOSE, NOT a target you attack or affect. Pick the card you can best AFFORD
TO LOSE (usually your LEAST valuable: a spare land or a redundant/dead card; KEEP your best spells,
answers, and threats).` The rows are `1. Forest [land] [your library] - "G" / 2. Plains [land] [your
library] - "W" / …`. The primitive (`mtg.txt:84676`) is
`auto=moveto(exile) && ability$!name(search land) notaTarget(land[basic]|mylibrary) moveTo(mybattlefield)
and!(tap(noevent))!!$ targetcontroller` — the chosen card is **put onto the battlefield tapped**, a gain.
Seam: `handRemovalVerb()` (`AIPlayerGPT.cpp:20079-20096`) matches the lowercased chooser-source-plus-verb
string for `"exile"`, and the *source card's name* is "Path to Exile", so the name wins over the ability's
actual destination. Consequence: the framing inverts the value ordering (it tells the seat to pick the
land it least wants, when the correct pick is the land it most needs), and it is also the reason two of
the three ≤2-shape mega-menus exist (17 library Mountains, 14 library Swamps). Fix: gate the removal
framing on the ability's destination zone (`moveTo(mybattlefield)`/`hand` ⇒ a **gain** ask), not on the
source name.

**D-3 MED — the reveal seam has no bounded wait and no trace.** Generalisation of HIGH #1: the whole
interactive-reveal driver can park in phase 0 or phase 3 forever with `mAIDriveDone == false`, and its
only instrumentation is `REVEAL_DBG`, gated on `WAGIC_REVEAL_DEBUG`. In a 21-game unattended corpus that
turned a 13-hour hang into a silent one. Minimum ask: a tick budget per phase that, on expiry, force-closes
the display and writes a record; and an ungated one-line stderr for each phase transition (or a
`reveal_stall` field on the record) so the next corpus can be triaged without a rebuild.

**D-4 MED — the kill summary counts creatures only and hides a player target.**
`deck130-vs-deck125` seq **127** (turn 20) renders
`Cast Hammer of Bogardan {1}{r}{r} … {kills 0 of the 1 legal target at 3 damage} - legal targets right
now: the opponent, Rorix Bladewing {3}{r}{r}{r} (creature 6/5) [flying, haste], you` — three legal
targets are listed, the summary says "1", and "kills 0" is true only of creatures while 3 to the face is
the row's actual value. Same shape at `deck130-vs-deck162` seq 62. Two of the four `{kills 0 …}` takes are
these rows, i.e. the model was right and the summary was misleading. Also in this family: 244 of 413
creature-targeting removal rows (Path to Exile 224, Fateful Absence 15, Vanishing Verse 5) carry no
`{kills: <victim>}` at all even though unconditional removal has a perfectly knowable victim list
(`deck125-vs-deck126` seq 13-23 is 11 consecutive Path rows with a target list and no verdict).
Fix: count player targets separately (`… and N to the face`), and give unconditional removal a
`{kills: <name>}` from its own target list.

**D-5 MED — the `Staff of Nin #N` ability menu is the wave-52 D-5 class, uncollapsed.**
`deck125-vs-deck126` seq **220**: 28 rows of `Deal 1 damage with Staff of Nin #N targeting …`; seq 189 and
213 carry 26. The `#a-#b` collapse works one line away on cast-row target clauses and on the battlefield
block (`Glacial Fortress #1-#4 x4; Island #1-#8 x8` in the specimen) but still does not reach ability
menus. Related label wart in the same lane: 53 stack lines render
`ability: Ob Nixilis, the Hate-Twisted's Ob Nixilis, the Hate-Twisted` because the effect label falls back
to the source name when the ability has none.

**D-6 LOW — the `(xN)` log collapse still misses phase lines, draws and loyalty counters, and the miss
grew.** 571 duplicate event lines inside single records (wave 52: 280): `- Phase: Draw` 237,
`- Opponent drew a card` 116, `- Phase: Main phase 1` 71, `- Phase: Combat damage` 9,
`- Opponent's Ob Nixilis, the Hate-Twisted lost a loyalty counter` 5. At a log that is 75-80 % of every
30 KB prompt, and with 476 trims firing, this is free space.

**D-7 LOW — two `- Paid` receipts still have no payee.** `deck152-vs-deck126` seq 25
(`- Paid {1}{w} with Plains #2; Deserted Beach`) and `deck152-vs-deck162` seq 29 — both the Intrepid
Adversary pay-any-number-of-times ETB. 7 → 3 → **2** across three waves.

**D-8 LOW — two genuine name-vs-index mis-executions with no note.** `deck126-vs-deck125` seq **13**:
`CHOICE: 2 (Cast Idyllic Tutor)` on a 3-row menu executed row **1** `Cast Perimeter Captain`;
`coded_answers: 2`, `latched_coded_line: "2"`, `parse_note: None`. Seq 14 repeats it. Both times the
name and the index agreed with each other and disagreed with the executed row, which points at the
latch/replacement path rather than the echo parser.

**Observed, not defects.** `answer_replaced` 12, all last-clean-line reads over a reasoning body, and the
`events` confirm each. `retracted_choice` (1) is a clean new observability win — the record now shows the
model retracted and the `recovery` record shows Baka's substitute action. `multi_answer_first_taken` (2)
and `attack_last_line_taken` (3) behaved. The 4 takes of a `{kills 0 …}` row and the 12 second-copy casts
made **with** the clause in front of the model are deck-seat calls, not render failures.

---

## Lategame specimen for the owner's per-wave prompt review

**Keep the harvest default.** `wave53/lategame-specimen.txt` =
`1788334427-ai_baka_deck125-0x55be9ff4b380-vs-ai_baka_deck146.jsonl` seq **357**, turn **84**, Main phase 1,
28,481 chars, 6 rows. It is the deepest turn in the corpus and it happens to be the decking endgame: it
shows `Opponent library: 0 cards`, a 27-permanent battlefield rendered with the `#1-#4 x4` collapse, the
`{X pricing: max affordable X=20 (23 mana total)}` form, three dead sweeper rows each honestly priced
`{right now: destroys 0 of their creatures …, 0 of yours}`, the log trim header with its graveyard/exile
inventory, and the PLAN age stamp `(as you last stated it, 2 windows ago on turn 84)`.

**Companion (written this wave): `wave53/lategame-specimen-render-dense.txt`** =
`1788334424-ai_baka_deck125-0x55c56d4af610-vs-ai_baka_deck126.jsonl` seq **66**, turn **22**, 10,412 chars,
4 rows, 27 life vs 20. It is the corpus's densest window a human can read in one screen — 10 annotation
families: the **HOLD row**, `[you declined this exact list N times already this turn]`, the PLAN age
stamp, `{X pricing:}`, `{spends K of M}`, the `LIFE-TO-DAMAGE CONVERTER` block, the `#a-#b` collapse,
`{card text:}`, `{target text:}` and a `- Paid` receipt.

**If the owner wants the size ceiling:**
`1788334464-ai_baka_deck146-0x55629e998440-vs-ai_baka_deck123.jsonl` seq **92** (32,068 chars, turn 33,
priority) — the corpus maximum and the exhibit for the D23 log question.

---

## WAVE-54 DOCKET PROPOSAL (engine / UX only), ranked

1. **HIGH #1 — the reveal hang.** A predicate-gated reveal whose option one has zero legal targets must
   resolve, not park. Three parts in the fix shape above. Repro: `deck152-vs-deck125` seq 27→28 +
   `game-152v125-1788334434.stderr:1097-1214`. Cost: a whole game, 13 h of wall clock, and a 20 %
   per-firing hazard on every Sigarda Coven trigger.
2. **D-1 — make the HOLD row usable.** Reserve its short name so `CHOICE: 0 (Hold priority)` cannot
   degrade to a bare pass; exempt it from the D14 pass-verdict conflict; and reposition/reprice it so the
   703 windows already carrying a `declined N times` note actually reach for it. Repro:
   `deck125-vs-deck146` seq 74, `deck146-vs-deck125` seq 150/371/372/398/470/491.
3. **D-2 — stop calling a Path-to-Exile land search a loss.** Route the framing off the ability's
   destination zone. Repro: `deck152-vs-deck125` seq 12.
4. **D-3 — bound and instrument the reveal driver** (a tick budget per phase + one ungated stderr line
   or a `reveal_stall` record field).
5. **D-4 — kill summaries: count player targets, and give unconditional removal a victim name.**
   Repro: `deck130-vs-deck125` seq 127; `deck125-vs-deck126` seq 13-23.
6. **D-5 — extend the `#a-#b` collapse to ability menus** (`deck125-vs-deck126` seq 220, 28 rows).
7. **D-8 — the latch path can execute a row neither the index nor the name chose** (`deck126-vs-deck125`
   seq 13/14) — add a note at minimum.
8. **D-6 / D-7** — `(xN)` collapse for phase/draw/loyalty lines; payee on the Intrepid Adversary receipt.
9. **Lane-N re-scope.** Opponent-turn windows went **up** 19 % and casts **down** 15 %; six phases spent
   365 windows and 3,269 s on zero casts. The HOLD row is the right lever and it is not being pulled —
   fix the lever (item 2) before narrowing the re-opener set, since a blind identical-menu cache would
   still have suppressed real casts (`deck125-vs-deck152` seq 15's Attackers-step Path to Exile is this
   wave's counter-example).
10. Watch (rendered correctly, deck seats' call): 4 takes of a `{kills 0 …}` row; 12 second-copy casts made
    with the clause visible; 8 takes of a `cannot block on their turn` tap row.

---

## Falsifiable predictions for wave 54

- A reveal whose option one has **zero** eligible cards resolves to option two and the game continues:
  **N/N**; no seat log ends without a `gameend` record; 21/21 natural.
- `reveal` records with `latency_ms: -1` and `prompt: 0` are each followed by a further record from the
  same seat within the same turn: **N/N**.
- `CHOICE: 0 (Hold priority)` (or any HOLD-named echo) executes the HOLD row or is re-asked: **N/N**;
  HOLD-row takes that also trip `plan_choice_conflict`: **0**.
- HOLD take rate in windows already carrying `[you declined this exact list N times…]` with N ≥ 3: **≥ 20 %**
  (this wave 3/703 = 0.4 %); `hold_windows_skipped` non-zero on **≥ 6** seats (this wave 2).
- Opponent-turn casting windows per opponent-turn down **≥ 25 %** with casts **≥ 90 %** of 58;
  End / Cleanup / Combat-ends / Combat-begins / Main 2 / Blockers still contribute **0**.
- `EXILE / DISCARD / SACRIFICE ONE OF YOUR OWN CARDS` asks whose candidate zone is `mylibrary` and whose
  destination is the battlefield or hand: **0** (this wave 8/8).
- Ability menus with **> 12** rows of one verb+source over `#N` handles: **0** (this wave: 28).
- Cast rows for unconditional removal carry `{kills: <victim>}`: **N/N**; `{kills 0 of the N …}` rows that
  also have a player target say so: **N/N**.
- Consecutive identical-declined runs (3+, same turn, byte-identical `options_text`): **< 15 %** of
  decisions (this wave 26.0 %).
- Holding from this wave: `stale_livelock` 0; `with no source tapped` 0; `turn` == narration turn N/N;
  `- Paid` receipts with no source 0; bare `ability:` stack lines 0; PLAN blocks without an age stamp 0;
  battlefield headers without a land count 0; `multiblock_reask` with no `chosen_text` 0; blockers records
  without a matching `declared blocks from` line 0; Baka-executed ≤ 0.15 %; every `choice: -1` fallback
  followed by exactly one `recovery` record.

---

## What I did NOT check

- **Any Set A guide prediction, any deck-strategy call, and skill #177's prose-vs-bullet diff across the
  seven guides.** Mulligans are recorded, not adjudicated. deck130's 0 mulligan ships are reported as a
  #132-UNTESTED fact, not a guide question.
- **I did not build, run wagic, or run git.** HIGH #1's code attribution is a read-only trace of
  `AllAbilities.cpp` (`toResolve`, `driveInteractiveReveal`) and `AIPlayerGPT.cpp` (`decideReveal`)
  against the stderr and the two seat logs. The phase-3 parking point is the only path consistent with
  every observable (record written, option two fired and resolved with no target, zero further engine
  lines, `REVEAL_DBG` gated off) — but it is inference from reading, not from a debugger.
- **The wave-52 D-2 opponent-turn cost analysis was not repeated at phase-by-phase inference granularity
  per seat** — I report corpus totals per phase only.
- **Autotap optimality (lane B)** beyond the no-source / no-payee counts; I did not redo the exhaustive
  pip match over all 496 receipts.
- **The Vita/owner batches with no pool card**: adventure/split/aftermath, Calamity Bearer, Drake Hatcher,
  Delver, evolve, morph, Go-Shintai, Dominating Vampire. Absence verified from the seven deck lists.
- Whether the 63 single stale drops share a common shape (none consecutive; not traced).
- `User/transcripts/` — not a review input this wave.
