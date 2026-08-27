# Wave-47 seat review — ENGINE / NARRATION

Corpus `matchups-20260826-182155`, binary 5b1bf9668. 42 seat logs, 5,726 decisions.
20/21 games natural; game `deck123 vs deck162` DEAD-LOOPED (no gameend) and is excluded from
every rate below unless named. All strings quoted here are the emitter's ACTUAL output, read
out of the corpus with python. Card facts verified against
`bin/Res/sets/primitives/{mtg,borderline}.txt`.

Seat scope: engine + narration only. No deckN/ outputs, no guide edits.

---

## PRIORITY 1 — the dead loop (deck123 vs deck162): full characterisation

Repro: `1787786540-ai_baka_deck123-0x55e867ceaa40-vs-ai_baka_deck162.jsonl`, seq **51 → 1921**
(the whole of turn 10). Peer seat: `...deck162-0x55e8669ce290...` — **14 records total**, last
decision seq 13 (turn 10 Main-1, `Cast Fate Unraveler`). The peer was starved of every window
from that point on.

### The engine on the board
- `Thraben Doomsayer` (mtg.txt:120581) `auto={T}:token(Human,Creature Human,1/1,white)`.
- `Intruder Alarm` (mtg.txt:58849) `auto=@movedTo(creature|myBattlefield):untap all(creature)`,
  plus `auto=lord(creature) doesnotuntap`.
Tap Doomsayer → token enters → Alarm untaps all creatures → Doomsayer is untapped again.
**This is a legal, rules-correct, unbounded optional loop.** Nothing here is an engine bug in
the rules sense; the defect is entirely in the DECISION SURFACE.

### The offer shape, every window (verbatim, seq 1921)
```
Your legal actions (Main phase 1, opponent's turn):
1. Create human with Thraben Doomsayer [cost: Tap] [repeat: activated this turn 1868 times
   already; you control 1897 creatures] {card text: "{T}: Put a 1/1 white Human creature
   token onto the battlefield. -- Fateful hour - As long as you have 5 or less life, other
   creatures you..."}

You will have priority again later this turn, so instants and activated abilities you hold
stay castable this turn.

Which action do you take? On the FIRST line write CHOICE: followed by the number
(0 = pass priority) ...
```
Measured over the 1,871 turn-10 windows: **options == 1 in 1,871/1,871**; choice == 1 in
1,869; two `unparsed_reply` (seq 560, 658). **Zero passes.** Phases: Upkeep 510 (seq 51-560),
Main-1 1,361 (seq 561-1921).

Four facts about that shape:
1. **There is no decline ROW.** The house rule "declines go LAST" is not honoured here: pass
   exists only inside the instruction sentence as `0 = pass priority`. The model's option-1
   bias has nothing to bias against.
2. **The `[repeat: N]` tag was present, truthful, and inert** — it counted correctly to 1,868.
   It is a *description* of the loop, not an *affordance* for leaving it. Prediction
   "single-option loop windows > 100 s: 0" — **FAIL**, 8/3,818 corpus-wide (5 of the 8 inside
   the three token-loop games), and the tag stopped nothing.
3. **The last two lines argue FOR staying.** `You will have priority again later this turn` is
   true and, in a loop, is a standing invitation.
4. **The `{card text:}` is truncated mid-word**, dropping the Fateful-hour lord clause.

### Why the two-decline allowance / last-offer clause (lane L) never engaged
`AIPlayerGPT.cpp:12097` — `mPassDeclineCount[...]++` runs **only when `choice == 0`**. The
model never passed, so the count stayed 0, `declines >= declineCap` never held, and
`lastOfferClause(declines + 1 >= declineCap)` (line 11715) rendered the empty string at every
one of the 1,871 windows. **Lane L is structurally unreachable from inside a loop**: the brake
is only armed by the behaviour it is meant to correct. (Verified: `stripRepeatAnnotation` is
correctly keeping the moving `[repeat: N]` digits out of the key — #W41-6 works; that is not
the failure.)

### Why the deadlock breaker never fired
`askKey = serializeGameState() + tail.str()` (line 11994). The breaker fires only when
`askKey == mLastAskKey`. Every activation changes BOTH halves: the battlefield gains a
creature, and the tail's `[repeat: activated this turn N times]` digits increment. The breaker
detects **no-op repetition**; this loop is *productive* repetition. It is blind by design.

### Whether the mana-only gate touched it
**No.** The single option is `AGenericActivatedAbility`→`ATokenCreator`, not
`AManaProducer`, so `isManaOnlyAction` is false and `allManaOnly` never held. `mana_only_windows_skipped`
for this seat is unreadable anyway (no gameend record).

### Memory / prompt growth — the ranged collapse HELD (partially)
Prompt length across the loop: **min 10,926 / mean 25,775 / max 28,476 chars**, i.e. it
plateaued rather than growing with the 1,897 creatures. Two mechanisms, and only one is R8's:
- The **battlefield line** collapsed correctly: `Human #3-#1895 (1/1) [...] x1893` — one row for
  1,893 tokens. This is the ranged collapse working exactly as intended and is the single
  reason this prompt is 28 KB and not ~2 MB.
- The **GAME LOG narration did NOT collapse.** ~90% of every prompt is the literal repetition
  `- You used: Create human with Thraben Doomsayer` / `- Your Thraben Doomsayer created a 1/1
  Human token`, ~180 pairs deep, held flat only by the log's `(...earlier events trimmed...)`
  cap. So the plateau is a TRIM, not a collapse: the model is shown ~180 identical sentences
  and no count.

Cost: 1,871 round trips, **48.2 M prompt characters (~12 M prompt tokens)**, **5.7 h of
measured round-trip latency**, ~9 h wall before the orchestrator killed it (invariant 00).

### The model was not confused — it was correctly following an instruction
Its PLAN never drifted: *"Continue tapping ... every opponent turn to build an infinite blocker
army. On my next main phase, attack with all creatures to win the game."* The turn is the
**opponent's**, so its own stated payoff is unreachable without passing, and nothing in the
window says so.

Cross-seat note for the deck123 reviewer / synthesis (NOT my edit to make): the live guide
`deck123_strategy.txt:69` says verbatim — *"P is at or above the opponent's life and it is the
OPPONENT'S turn: take it anyway. A token made on their turn is a blocker now and an attacker on
your next turn, and it costs nothing."* — with `THERE IS NO CREATURE-COUNT CAP` at line 72. On
the opponent's turn the guide's own stop condition is switched off and no other bound exists.
The same guide's own-turn branch **works**: see PRIORITY 1b. The engine must not depend on
guide prose to terminate a legal infinite loop, but the deck123 seat should know its
opponent's-turn branch is unbounded.

### PRIORITY 1b — the model CAN stop, and the parser threw the stop away (NEW, HIGH)
Two `retracted_choice` records, both in `...deck123-0x55f1128f75d0-vs-ai_baka_deck146.jsonl`,
**seq 34** (t11 Upkeep) and **seq 221** (t13 Upkeep). In both the model opened `CHOICE: 1`,
then re-derived its guide's own stop rule and concluded, in prose:
> `So, CHOICE: 0 (pass).`
`choiceRetractedNoReplacement` (line 11490) scans for a replacement **line-leading** only
(skipping `space \t * # -`). `So, CHOICE: 0 (pass).` is mid-line, so no replacement was found,
`choice = -1`, and the heuristic decided. Two further consequences:
- a `retracted_choice` is `choice == -1`, so it does **not** increment `mPassDeclineCount` —
  the model's explicit stop is not even counted toward the decline cap.
- both were single-option loop windows (the two 216 s / 190 s outliers in the >100 s set), i.e.
  the corpus's two most expensive deliberations were the two that were discarded.
This is the exact behaviour the loop item wants, produced by the model, and dropped by the
parser. See docket **E1**.

### PRIORITY 1c — why the other two loop games terminated
`deck123 vs deck126` (1,550 priority records, ended t11, opp life **-4,722**) and
`deck123 vs deck146` (223, ended t13, opp life **-775**) ran the same engine and **finished**,
because the loop ran on deck123's OWN turn and the attack step consumed it. The dead-lock is
therefore not "the loop exists" but **"the loop is offered in a window whose payoff is not
reachable in that window"**. Any fix that ends the loop must not break the two games where the
loop is the whole win condition.

### Proposed fix shape (LLM-UX axis; legality enforced, choice unconstrained)
The doctrine bars capping the play. Three affordances, ranked, none of which removes a legal
option:

**F1 (preferred) — an engine-level repeat-N offer.** When an activated ability has been taken
K times this turn (K = the existing `aa->counters`, already tracked and already rendered) and
its cost is repayable from state the activation itself restores, render a SECOND row for the
same ability:
```
2. Create human with Thraben Doomsayer, repeated N times, then stop  [you name N on the CHOICE
   line, e.g. "CHOICE: 2 (Create human x50)"; the engine performs it N times and then returns
   priority to you here]
```
This is MTG rule 720/721 (taking a shortcut by announcing a number) expressed as an option
row. It ADDS a choice, removes none, is legality-safe (the engine re-checks the cost each
iteration and stops early if it becomes unpayable), and collapses 1,871 round trips into one.
It is the only option that also fixes the two *winning* loop games (1,550 and 223 round trips
for one attack).

**F2 — a truthful stopping annotation on the repeat tag.** Extend the existing `[repeat:]` tag
with the fact the model cannot derive, restriction-first and only where true:
`[repeat: activated this turn N times already; you control M creatures. This turn will not
advance while you keep taking this option; nothing else on your board has changed for the last
K activations.]` No affirmative "you should stop" — the model owes the surface belief, the
surface owes it the mechanism. Cheap, but it is prose against an option-1 bias with no decline
row, so I rate it a companion to F1, not a substitute.

**F3 — render the decline as a ROW.** In a single-option priority window, append
`2. Pass priority (take no action this window)` as a real row, so the house ordering rule
("declines go LAST") actually applies and the model has something to select. This is the
cheapest change in the set and is independently correct: 3,818 of 4,128 priority windows in
this corpus were single-option, and in every one of them the only way to decline was to invent
a number that is not in the list.

**Rejected: a hard offer cap.** A cap with a truthful annotation ("this option has been taken
N times and is withdrawn for this turn") does bound the loop, but it constrains the choice on
exactly the two boards where the loop is the deck's win condition (deck123 vs 126/146), and it
would have to fire at some N nobody can justify. Per the owner doctrine — legality enforced
structurally, choices not constrained — F1+F3 achieve the same termination by *giving* the
model a reachable stopping act instead of *taking away* a legal one. If the loop must be
bounded defensively as well, bound it at the ENGINE tick level (a per-turn activation budget
that converts to an F1-style "the engine will perform the remaining N and pass" offer), never
by deleting the row.

---

## PRIORITY 2 — `mana_only_windows_skipped` = 8,673 (was 180)

Per-seat, from the 40 gameend records (the two loop-game seats have none and contribute 0):

| skips | seat | vs | turn | own priority records |
|---|---|---|---|---|
| **8,610** | deck126 | deck123 | 11 | 3 |
| 32 | deck126 | deck125 | 53 | 0 |
| 30 | deck126 | deck152 | 16 | 0 |
| 1 | deck126 | deck162 | 12 | 0 |
| 0 | *all other 36 seats* | | | |

**The counter is not inflated by the loop seat, and lane L did not change it.** 8,610 of 8,673
(99.3%) come from ONE seat, and the mechanism is the *opponent's* Intruder Alarm: every one of
deck123's 1,533 `Create vampire` activations untaps **all** creatures on both boards, including
deck126's `Overgrown Battlement` / `Pride Guardian`, handing deck126 a fresh priority window
whose only non-declaration option is `Add N mana with Overgrown Battlement`. ~5.6 windows per
opponent activation. Verified: deck126's last two asked windows (seq 18/19) carried 432 and 431
options, so real windows were still being asked normally right up to the end.

**No window with a non-mana option was skipped.** Two lines of evidence:
1. By construction: the gate (line 11930) requires `isManaOnlyAction` true for **every**
   entry of `shown`, plus `!mExtraPayment && !menuObject && !getCurrentTargetChooser() &&
   !getManaPool()->getConvertedCost()`. One non-mana entry sets `allManaOnly=false`.
2. By reconstruction from narration: across the whole corpus, every seat's narration between
   consecutive asked windows contains only opponent events and the seat's own untap/token
   traffic — no seat's log shows a self-action it did not first answer for, and deck126's own
   plays (`Cast Staff of Nin`, `+1 Sorin`, `Deal 1 damage with Staff of Nin`) all appear as
   asked records with their prompts.
Verdict: **the gate is working and is the reason this game finished at all** — without it
deck126 would have paid 8,610 round trips. The counter's 48× jump is a *symptom of the loop
family*, not a regression in the gate.

One LOW caveat: `shown` is built AFTER the two-decline retirement filter, so a window holding
"mana ability + an option already declined twice this turn" is classified mana-only and
counted. That skips nothing live (the retired option was already suppressed), but it means the
counter slightly over-reports pure mana-only windows. Docket **E11**.

---

## PRIORITY 3 — the `Cast Example Card` exemplar in the priority tail

Measured: the hard-coded exemplar appears in **4,128 / 4,128 priority prompts** (100%; it is
in the fixed instruction sentence, not the menu emitter). It is **NOT latched**:
- replies naming a `Cast *` action where the option list contained **no** `N. Cast ` row:
  **1 / 4,128** (0.024%) — `...deck146-0x55f47fabb1c0-vs-ai_baka_deck125.jsonl` **seq 36**,
  `CHOICE: 8 (Cast Acererak the Archlich)` into a 2-option list, logged `unparsed_reply`. That
  is an off-menu hallucination of a real card in hand, not an echo of the placeholder (the
  placeholder's own words never appear in any reply).
- the MENU emitter's exemplar was correctly rewritten by a7310f01c and now reads
  `e.g. "CHOICE: 1 (creature gains 3/3 and return creature)" (a worked example of the format
  using the first option - choose the option YOU want)` — **PASS**, verified on all 4
  `A choice is required` records and the mode/X/land menus.
Verdict: **PASS** on latching; leave the priority tail's placeholder alone (it already carries
the `(a placeholder - copy a real number and short name from the list)` disclaimer, which is
doing its job at 4,127/4,128).

---

## Set B adjudication — by the emitter's actual string, with counts

### a7310f01c
| prediction | verdict | evidence |
|---|---|---|
| menu header names the ability carrier; `A choice is required` 0/N (was 4) | **FAIL — still 4/4** | emitter's actual string is unchanged: `A choice is required - choose an option:`. Repros: `...deck146-0x558706737270-vs-deck130` seq 30; `...deck146-0x55f47fabb1c0-vs-deck125` seq 20; `...deck146-0x561e0aa9c320-vs-deck126` seq 20 (all three **Silverquill Command**, the exact card the fix's comment at line 14338 names); `...deck162-0x55681e07a590-vs-deck152` seq 21 (**Peer into the Abyss**, a different family — an `auto=choice name(...)` target menu). Both cards verified: `borderline.txt:102762` / `:82589`, both `auto=choice name(...)`. The three-rung ladder does not recover a name for the `choice`-menu family. |
| exemplar from option 1; `Cast ...` into cast-free menus 0/N; stale_echo on non-card menus 0 | **PASS** | exemplar rewritten (see PRIORITY 3); 0 `Cast *` replies into any cast-free MENU; 1 corpus-wide `stale_echo` and it is a card-menu land ask (`...deck152-0x55ac0c573fd0-vs-deck125` seq 8, `CHOICE: 1 (Plains)`), not a non-card menu. |
| repeat tag from the first repeat; single-option loop windows > 100 s: 0 | **FAIL 8/3,818** | tag correct and present (3,755 prompts); 8 single-option windows exceeded 100 s, max 231.7 s. 5 of 8 are in the three token-loop games; 2 of those are the discarded retractions (E1). |

### 1c71d5301
| prediction | verdict | evidence |
|---|---|---|
| `Your hand (N cards):` — replies stating a wrong own-hand size 0/N (was 3/4) | **PASS 0/5,644** | header present on all 5,644 decision prompts; zero replies asserting a hand size that disagrees with it. |

### Lane J (2860f78c2)
| prediction | verdict | evidence |
|---|---|---|
| DRAW PUNISHERS line + `[DRAW PRICE:]` row tags; draws at/below summed cost 0/N | **PARTIAL / thin** | the summary line fired widely (**2,043 prompts**) and its wording is correct. The per-row `[DRAW PRICE: ...]` tag fired **twice, both in one seat** (`...deck146-0x562e93be9370-vs-deck162` seq 27, 32). On seq 32 the tagged draw was **declined** (`pass`) — the one clean window the tag got, it won. 0 draws taken at/below the summed cost. Verdict PASS on the metric, but n=2 for the row tag: it is **effectively UNTESTED**. |
| annotated `Cast Card Normally / cycling` menu; Cast-Normally chosen with a cycle plan 0/N | **PASS 0/8** | 8 menus offered a `Cast Card Normally` row; no reply paired it with a cycling plan. |
| X=0 null-cast callout + life/draw preview; X=0 answered where X>=1 affordable 0/N | **PASS 0/29** | the callout string is `{X pricing: X=0 - this cast does NOTHING: you gain 0 life and draw 0 cards, and the spell is spent}`. Across 29 X announcements X=0 was chosen **zero** times (X=3 ×9, X=2 ×2, X=1 ×2, X=4 ×1, decline ×11). |

### Lane K (d30871747)
| prediction | verdict | evidence |
|---|---|---|
| converter doubling folded into blocking-trigger / lifelink tags | **PASS** | emitter's actual strings: `(blocking trigger: they may gain 4, and if they do their converter takes 4 off you)` (`...deck162-0x55be348e91c0-vs-deck126` seq 12) and `(lifelink: they gain 1, and their converter takes 1 off you)` (`...deck146-0x561e0aa9c320-vs-deck126` seq 40). The multiplication is bound to the tag as asked. |
| life lost between attackers records 0 | **PASS, thin** | no seat repeated the wave-46 20→11→5 pattern; the converter boards this corpus (deck126/deck146) never reached three consecutive converted combats. |
| affirmative `[no creature they control can block this attacker]`; ATTACK: none on it 0/N | **PARTIAL — tag PASS, metric FAIL 11/60** | the affirmative string rendered on 60 attacker rows. 11 windows answered `no attackers`, and **10 of the 11 are one creature in one seat**: `Dwarven Blastminer (1/1)` in `...deck130-0x564bb37cbef0-vs-deck152` seq 12/17/25/38/68/80/88/97/106/116 (t5-t?, 20-20 the whole way). Verified `mtg.txt:33251` — `auto={2}{R}{T}:destroy target(land[-basic])`. **Attacking taps it and forfeits that ability**, so declining is a defensible trade, not a misread. The row states the upside and is silent on the cost. See docket **E9**. |
| zero-power STOPS scoped `THIS COMBAT`; wall on a dies-row while a lives-row is offered 0/N | **PASS 0/12** | the scope string is present (`...it STOPS all of the damage from whichever attacker it blocks THIS COMBAT - each A-line above says how much`), 12 blockers prompts; no wall was sent into a dies-row while a lives-row was available. |

### Lane L (2a4ff28be)
| prediction | verdict | evidence |
|---|---|---|
| `{if you pass here, this option is not offered again this turn}` on rows at their cap | **PASS as a string, UNREACHABLE in the failure mode** | clause rendered on **696 priority prompts**; 82 of those windows (12%) were then passed. But see PRIORITY 1: the clause can only appear after the model has already declined once, so it is structurally absent from any window the model never declines — which is precisely the loop. |
| lethal-in-upkeep activations taken 100% (was 0/2) | **UNTESTED** | 30 manland-activation windows (22 Upkeep / 6 Main-1 / 2 Blockers / **0 Main-2** — the R4 timing skew is unchanged, as expected from the annotate-rather-than-resurface route). Lowest opponent life at any such window was **5**; no lethal window arose. Take rate 14/30. |
| priced `Mulligan (a keep after this one would keep N cards)` + NO carried plan on pregame asks; third-look chains 0/N | **PASS** | 50 priced mulligan asks; **0** carry a `YOUR PLAN` block; 42 keeps / 8 mulligans; no chain reached a third look; no seat kept below 6. |
| land-drop asks: no PLAN request + consequence sentence; max 2-option latency < 200 s (was 453); land-drop declines 0/N | **PASS on 3 of 4** | emitter's actual tail: `...this decision needs no PLAN line. Write nothing else.` and the consequence sentence `(playing a land costs no mana and uses up no cast: it does not reduce what you can cast this turn)`. 386 land asks; **max latency 50.8 s**, ≤2-option max 30.9 s. Declines **2/386** (`...deck162-0x562e93be9370` seq 29; `...deck126-0x55fa89ab8040` seq 2) — a technical FAIL of a 0/N prediction at 0.5%, not worth a fix. **Residual:** the modal-DFC Pathway land route uses a different emitter (`Choose an option for Barkchannel Pathway:`) and still asks for a PLAN and omits the consequence sentence — 28 records. Docket **E10**. |

### Lane M (8b6545b48)
| prediction | verdict | evidence |
|---|---|---|
| per-clause `{effect:}` budget keeps the ultimate; truncated loyalty abilities 0/N | **PASS 0/61** | 61 rendered planeswalker `{effect:}` strings, **0 truncated**, **61/61 carry a negative-loyalty clause**. |
| own-side `{effect:}` minus permanents with an option row; own bare permanents 0/N | **PASS** | no own-side bare permanent appeared with an `{effect:}` duplicated onto its own option row. |
| blockers ranged collapse `B2-B22 ... x21`; blockers p90 ≤ 60 s, tail mean ≤ 4,600 chars | **PASS on the B side / FAIL on the metric** | The collapse works and is correct — but it landed on the side that never gets wide. Corpus-wide: **max B-rows in any blockers prompt = 5**; **max A-rows = 198**; B-range collapse present in **2/38** blockers prompts; **A-range collapse present in 0/38**, and **0/109** attackers prompts. Measured p90 latency: 53.0 s on the 20 natural games (**PASS**) / 180.3 s including the loop games. Tail (top-quartile) mean chars: **16,678** natural / **23,724** all — against a 4,600 prediction, **FAIL by 3.6×**. Worst records: attackers prompt of **233,662 chars / 1,525 options / 178 s** (`...deck123-0x55fa8add5b20-vs-deck126` **seq 1570**); blockers prompt of **74,063 chars with 2 real options** (`...deck146-0x55f1115be320-vs-deck123` **seq 26**, 198 identical A-rows each carrying the same 120-char `[NONE of your available blockers can block this attacker...]` tag). Docket **E2**. |
| `{c}` in colour sets; `{1}`/`{x}` in a colour set 0/N | **PASS 0/N** | zero prompts contain a bare count or `{x}` inside `colours you can make:`; `{c}` present in 186. |
| `N of them able to attack right now` + `{leaves N of your M untapped mana sources untapped}`; sweeper casts into all-defender boards 0/N | **PASS on the strings, RENDER FALSEHOOD in one scope** | both strings ship (5,586 and 678 records); no sweeper was cast into an all-defender board. **But**: in a blockers window after attackers are declared, the header reads `Opponent battlefield (220 permanents listed, of which 215 are creatures, 0 of them able to attack right now)` on a board where **197 rows are marked `[tapped - attacking]`** (`...deck146-0x55f1115be320-vs-deck123` seq 26). The number is the engine's own `canAttack()` and is true to that predicate, but in this scope it is a true statement in the wrong scope — the trust doctrine's definition of a lie. **HIGH, docket E3.** |

---

## Fallbacks — 9 in the 20 natural games / 3,710 decisions = **0.243%** (baseline 0.10-0.28%: in band)

By mechanism:
| n | mechanism | records |
|---|---|---|
| 3 | **model decode degeneration** (CJK/word-salad output) | `...deck125-0x555dc22a73c0-vs-deck123` seq 39 (798 s); `...deck123-0x55fa8add5b20-vs-deck126` seq 401, 403 |
| 2 | **off-menu hallucination** — a real card named with an index past the list | `...deck130-0x558707a70550-vs-deck146` seq 24 (`CHOICE: 4` of 3, Rorix Bladewing); `...deck146-0x55f47fabb1c0-vs-deck125` seq 36 (`CHOICE: 8` of 2, Acererak) |
| 2 | **retraction with a mid-line replacement** — the model's own stop discarded | `...deck123-0x55f1128f75d0-vs-deck146` seq 34, 221 → **E1** |
| 1 | **`empty_reply` at the 900 s wall** | `...deck123-0x55610f8f41d0-vs-deck130` **seq 7**, `latency_ms 900046`, a 2-option ask at t? — exactly the configured `gpt timeout=900s`. One occurrence; the timeout is doing its job and the heuristic answered. No action. |
| 1 | **`stale_echo`** | `...deck152-0x55ac0c573fd0-vs-deck125` seq 8 |
Plus 2 more in the excluded loop game (`unparsed_reply` seq 560, 658) — and note that **the
only thing that ever advanced the dead-locked turn from Upkeep to Main-1 was one of them**
(seq 560): the loop's sole escape hatch in 9 hours was a model failure.

---

## Prompt chars per decision kind — wave 46 → wave 47

20 natural games only (loop-family games excluded on both sides for comparability):

| kind | w46 n | w46 mean | w47 n | w47 mean | Δ |
|---|---|---|---|---|---|
| ask | 1,365 | 9,800 | 1,245 | 10,412 | **+612 (+6.2%)** |
| priority | 504 | 11,554 | 450 | 14,368 | **+2,814 (+24.4%)** |
| attackers | 106 | 9,227 | 101 | 10,101 | **+874 (+9.5%)** |
| blockers | 30 | 10,153 | 35 | 10,345 | **+192 (+1.9%)** |
| reveal | 15 | 15,820 | 19 | 13,623 | **-2,197 (-13.9%)** |
| bottom | 8 | 2,564 | 6 | 2,215 | **-349 (-13.6%)** |

Read: R7/R6/R14/J's additions outweighed R8's removal on every asked kind except `reveal` and
`bottom`. The priority window took the largest hit (+24%), which is where the corpus spends
72% of its decisions. Including the loop games the priority mean is 21,860 (+89%).

---

## NEW → wave-48 docket

Ranked. Every item has a file+seq repro. Render falsehoods are HIGH by rule.

| id | sev | item | locus | repro |
|---|---|---|---|---|
| **E1** | **HIGH** | **The model's explicit stop is discarded.** `choiceRetractedNoReplacement` scans for the replacing `CHOICE:` **line-leading only**; a mid-line `So, CHOICE: 0 (pass).` is not found, the answer becomes `-1`, the heuristic decides, and (because `choice != 0`) `mPassDeclineCount` is not incremented either. Both corpus occurrences are the model correctly reasoning itself out of a token loop. Ask: on the retraction re-scan only, accept a mid-line `CHOICE: N` that appears AFTER the retraction marker and validates against the option set; a resolved `0` is a legal, always-safe answer and should also count as a decline. | `AIPlayerGPT.cpp:11490` `choiceRetractedNoReplacement`; the decline counter at `:12097` | `1787786547-ai_baka_deck123-0x55f1128f75d0-vs-ai_baka_deck146.jsonl` seq **34** and **221** |
| **E2** | **HIGH** | **The ranged collapse landed on the wrong side of combat.** R8 collapsed B-rows (max observed width **5**); the width is on the A-rows of the blockers screen (max **198**, 0 collapsed) and on the attackers screen (max **1,525 options**, 0 collapsed). One prompt is **233,662 chars / 178 s**; another is **74,063 chars for 2 real options**, 198 of its rows carrying a byte-identical `[NONE of your available blockers can block this attacker...]` tag. Ask: apply the same `joinZoneEntries` rule to the A-rows on BOTH combat screens (agreeing name/stats/tags/trade → one ranged row + count), keeping every label individually nameable as R8 already proves it can. | `AIPlayerGPT.cpp:17472`/`:17591` (R8's B-row builder) and the attackers-screen row builder | blockers `...deck146-0x55f1115be320-vs-deck123.jsonl` seq **26**; attackers `...deck123-0x55fa8add5b20-vs-deck126.jsonl` seq **1570** |
| **E3** | **HIGH** | **Render falsehood by scope: `0 of them able to attack right now` printed over 197 rows marked `[tapped - attacking]`.** The count is `canAttack()` and true to that predicate, but in a post-declaration combat window it contradicts the rows beneath it. Ask: in any window where attackers are already declared, either suppress the clause on that board or state the true fact for that scope (`N of them are attacking`). Never delete the token silently. | `battlefieldHeaderText` / `boardCreatureCanAttackNow`, `AIPlayerGPT.cpp:2172`-`:2206` | `1787786547-ai_baka_deck146-0x55f1115be320-vs-ai_baka_deck123.jsonl` seq **26** |
| **E4** | **HIGH** | **Unbounded legal loop has no stopping affordance** (PRIORITY 1). Ship **F1** (an `activate N times, then stop` option row, rule-720 shortcut, engine re-checks cost each iteration) + **F3** (render `Pass priority` as a real LAST row in single-option priority windows — 3,818/4,128 windows currently have no decline row at all), with **F2** (the repeat tag states that the turn will not advance while the option keeps being taken) as the annotation companion. Adds options, removes none. | priority-window option construction, `AIPlayerGPT.cpp:11719`-`:11890`; `[repeat:]` tag at `:9959` | `1787786540-ai_baka_deck123-0x55e867ceaa40-vs-ai_baka_deck162.jsonl` seq **51-1921** |
| **E5** | **MED** | **`{card text:}` on option rows truncates mid-word and drops whole abilities.** 7,291 occurrences, 28 distinct tails. At least four drop a real ability, not reminder text: Thraben Doomsayer `"...other creatures you..."` (2,015× — the Fateful-hour lord clause gone), Lord of Lineage `"...onto the battlefield. //"` (1,751× — the entire back face gone), Siege-Gang Commander `"...{1}{R}, Sacrifice a Goblin: Siege-Gang"` (165×), Sorin `"-2: Destroy target creature. Its"` (32×). R6's per-clause budget was applied to `{effect:}` only. Ask: extend the same clause-aware budget to the option row's `{card text:}` — never cut mid-word, never cut inside a `--` clause, and never cut before a `//` face separator. | the option-row card-text budget (companion to R6's `{effect:}` budget) | `1787786520-ai_baka_deck123-0x55c267f9bff0-vs-ai_baka_deck152.jsonl` seq **5**; loop file seq **1921** |
| **E6** | **MED** | **The menu header still cannot name an `auto=choice` carrier** — `A choice is required - choose an option:` 4/4, three of them Silverquill Command (the card the fix names) and one Peer into the Abyss. The three-rung ladder does not reach this family. | `AIPlayerGPT.cpp:14341`-`:14359` `resolveOwningCardName` | `...deck146-0x558706737270-vs-deck130` seq **30**; `...deck146-0x55f47fabb1c0-vs-deck125` seq **20**; `...deck146-0x561e0aa9c320-vs-deck126` seq **20**; `...deck162-0x55681e07a590-vs-deck152` seq **21** |
| **E7** | **MED** | **Raw script tokens leak into a target menu.** Peer into the Abyss renders `1. target opponent` / `2. target controller` — the literal `name(...)` strings from `borderline.txt:82590-1`. "controller" means *you*, which no reader can derive. Ask: map the two self/opponent branch names to `you` / `the opponent` at the menu emitter. | menu option text for `auto=choice name(...)` target branches | `1787786530-ai_baka_deck162-0x55681e07a590-vs-ai_baka_deck152.jsonl` seq **21** |
| **E8** | **MED** | **The battlefield ranged collapse is run-length-only, so interleaving defeats it.** A board of alternating `Vampire`/`Human` tokens rendered `Vampire #200 ...; Human #4 ...; Vampire #202 ...` one row each, while the adjacent run above collapsed to `x174`. Ask: group by identical rendered text, not by adjacency (or sort the entry list before collapsing) — the label-nameability note R8 already ships covers the non-contiguous case. Same record also shows the two collapse runs `#1-#22` and `#23-#196` split only by a single interposed Doomsayer row. | `joinZoneEntries` | `1787786547-ai_baka_deck146-0x55f1115be320-vs-ai_baka_deck123.jsonl` seq **26** |
| **E9** | **LOW** | **Attacker rows state the upside of attacking and never its cost.** `[no creature they control can block this attacker]` fired 60×; 11 declines, 10 of them one `Dwarven Blastminer` whose `{2}{R},{T}` land-destruction is forfeited by attacking (`mtg.txt:33254`). The decline is defensible; the row cannot say so. Ask: where an attacker has an untapped-only activated ability, name it on the A-row (`[attacking taps it: you lose {2}{R},{T}: destroy target nonbasic land this turn]`) — restriction-first, no recommendation. | attackers-screen A-row builder | `1787786551-ai_baka_deck130-0x564bb37cbef0-vs-ai_baka_deck152.jsonl` seq **12, 17, 25, 38** |
| **E10** | **LOW** | **The modal-DFC Pathway land route is not covered by R13.** 28 records: the `Choose an option for <Pathway>:` menu still requests a PLAN line and omits the `costs no mana / uses up no cast` consequence sentence that the plain land ask now carries. | the DFC land menu emitter vs the land-drop ask emitter | `1787786532-ai_baka_deck152-0x5605e480dfb0-vs-ai_baka_deck126.jsonl` (Barkchannel Pathway menu, first of 28) |
| **E11** | **LOW** | **`mana_only_windows_skipped` slightly over-reports**: `shown` is built after the two-decline retirement filter, so "mana ability + an already-retired option" counts as a mana-only window. Skips nothing live; makes the counter a fuzzy instrument. Ask: count on the pre-filter list, or rename the counter. | `AIPlayerGPT.cpp:11930`-`:11943` | corpus-wide (see PRIORITY 2 table) |
| **E12** | **LOW** | **The GAME LOG narration does not collapse repetition.** ~180 identical `You used: ... / created a 1/1 Human token` pairs per prompt, held flat only by the trim cap, so ~90% of a 26 KB prompt is one repeated sentence and the model is never given a count. Ask: collapse consecutive identical narration events to `- You used: Create human with Thraben Doomsayer (x180, creating 180 1/1 Human tokens)`. Fixes the token cost that E4 fixes the decision cost of. | narration append / trim | loop file seq **1921** |

---

## Lategame specimen candidate (invariant 0)

`1787786547-ai_baka_deck146-0x55f1115be320-vs-ai_baka_deck123.jsonl` **seq 26** —
turn 13 blockers, 74,063 chars, 2 real options, 198 A-rows, and the `0 of them able to attack
right now` header over 197 attacking rows. It is the corpus's clearest single artefact of
decision-value-per-token failing: the owner's criterion ("minimum tokens needed to provide the
full value") is violated by ~70 KB while the two facts that decide the window fit in six lines.
Runner-up, if a *natural* game is preferred: `1787786551-ai_baka_deck130-0x564bb37cbef0-vs-ai_baka_deck152.jsonl`
seq **116** (t?, the Blastminer attackers window — small, clean, and shows E9 exactly).

## Falsifiable predictions for wave 48
1. **E4/F1+F3**: with a repeat-N row and a rendered `Pass priority` row, no game reaches 500
   priority windows in one turn; the three deck123 token games close in **< 60** priority
   records each (was 1,871 / 1,550 / 223).
2. **E1**: `retracted_choice` count with a resolvable mid-line replacement → **0/N**; any
   remaining `retracted_choice` has no parseable `CHOICE:` anywhere after the retraction.
3. **E2**: max attackers-prompt chars **< 30,000** and max blockers-prompt chars **< 25,000**
   corpus-wide; blockers top-quartile mean **≤ 6,000** chars; attackers p90 latency **< 45 s**.
4. **E3**: **0** records in which `N of them able to attack right now` prints a number lower
   than the count of rows marked `[tapped - attacking]` on the same board.
5. **E5**: occurrences of `{card text: "..."}` cutting mid-word **0/N**; Thraben Doomsayer's
   Fateful-hour clause and Lord of Lineage's back face present on **every** row that names them.
6. **E6**: `A choice is required` **0/N**; every `auto=choice` menu header names its card.
7. **E12**: mean priority prompt chars in a token-loop game **< 8,000** (was 25,775).

## What I did NOT check
- I did not build, run, or instrument the binary; every mechanism claim above is read off the
  source at HEAD plus the corpus, and the two I could not close by reading are marked.
- I did not trace WHY `resolveOwningCardName` fails for the `auto=choice` family (E6) — that
  needs a probe, not a corpus read.
- I did not verify the 8,610 mana-only skips by counting engine ticks; the reconstruction is
  from narration continuity + the gate's own preconditions, not from a tick log.
- I did not adjudicate Set A (wave-46 guide edits) — no deckN seat, per my brief.
- I did not measure R3's "life lost between attackers" against a matched wave-46 window pair;
  the boards that produced the wave-46 evidence did not recur.
- I did not check whether F1's repeat-N shortcut is expressible through the existing
  `DecisionManager` build/apply pair without a new contract kind.
