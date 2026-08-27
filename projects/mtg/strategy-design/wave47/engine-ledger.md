# Wave-47 engine ledger — THE WAVE-48 DOCKET

Corpus `matchups-20260826-182155` (fourth fair-hand corpus), binary master **5b1bf9668**
(wave-46 engine fixes a7310f01c + hand count 1c71d5301 + lanes J/K/L/M). 42 seat logs,
**5,726 decisions**, **20/21 games natural**. The 21st — `deck123 vs deck162` — **DEAD-LOOPED
with no gameend** and was killed by the orchestrator; it is excluded from every rate and is
docket item **D1**.

Corpus root: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260826-182155/`. Every repro below is a
file plus a `seq`. Source-line references are the seats' reads of HEAD, not traced call sites,
except where marked verified.

**Health.** Fallbacks **9 / 3,710 = 0.243%** in the 20 natural games (baseline 0.10-0.28%: in
band); 11/5,686 = 0.193% counting all seats. `stale_echo` **4 -> 1**. `commit_retracted` 0.
`answer_replaced` 0. PARSETEST 1519/0 on the corpus binary.

---

# THE DOCKET, RANKED

## D1 — HIGH — **HEADLINER**: an unbounded legal loop has no stopping affordance
(= seat-engine **E4** / seat-123-130 **H1** / seat-125-126 **HIGH #1**; render/core view: `general-strategy.md` **R1**)

**Repro (the dead-locked game).**
`1787786540-ai_baka_deck123-0x55e867ceaa40-vs-ai_baka_deck162.jsonl`, seq **51 -> 1921** (the
whole of turn 10). Peer seat `...deck162-0x55e8669ce290...`: **14 records total**, last decision
seq 13 — starved of every window from that point.
**Repro (the loop that DID terminate, and is inside every rate):**
`1787786558-ai_baka_deck123-0x55fa8add5b20-vs-ai_baka_deck126.jsonl` — **1,572 records, 1,550
`priority`**; seq **1570** declares 1,184 attackers of 1,525 offered; the opposing gameend
records **-4,722 life at turn 11**. Third instance:
`1787786547-ai_baka_deck123-0x55f1128f75d0-vs-ai_baka_deck146.jsonl`, 223 records, ended t13 at
-775.

**Mechanism (rules-correct, so the defect is entirely in the DECISION SURFACE).**
`Thraben Doomsayer` (`mtg.txt:120581`, `auto={T}:token(Human,Creature Human,1/1,white)`) or
`Lord of Lineage` (`auto={T}:token(Vampire,Creature Vampire,2/2,black,flying)`) plus
`Intruder Alarm` (`mtg.txt:58849`, `auto=@movedTo(creature|myBattlefield):untap all(creature)` +
`auto=lord(creature) doesnotuntap`): tap the maker -> token enters -> Alarm untaps everything ->
the maker is untapped again. Legal, optional, unbounded.

**Four facts about the window, measured over the 1,871 turn-10 windows** (options == 1 in
**1,871/1,871**; choice == 1 in 1,869; two `unparsed_reply`; **zero passes**):
1. **There is no decline ROW.** Pass exists only inside the reply-format sentence as
   `0 = pass priority`. Corpus-wide **3,818 of 4,128 priority windows were single-option**, and
   in every one the only way to decline was to invent a number not in the list. The house
   ordering rule ("declines go LAST") is not honoured on any of them.
2. **The `[repeat:]` tag was present, truthful and inert** — it counted to 1,868 and changed
   nothing. See **D13** for what its scope actually is.
3. **The tail argues FOR staying**: `You will have priority again later this turn, so instants
   and activated abilities you hold stay castable this turn` is true and, in a loop, a standing
   invitation.
4. **The model was not confused.** Its PLAN never drifted: *"Continue tapping ... On my next main
   phase, attack with all creatures to win the game."* The turn was the OPPONENT'S, so its own
   stated payoff was unreachable without passing, and nothing in the window said so. The
   diagnosis is not "the loop exists" but **"the loop is offered in a window whose payoff is not
   reachable in that window"** — the two games where the loop ran on the pilot's own turn both
   terminated at the attack step.

**Why every existing brake was structurally unreachable** (each verified by the engine seat):
- **Lane L's decline cap**: `mPassDeclineCount[...]++` runs only when `choice == 0`
  (`AIPlayerGPT.cpp:12097`). The model never passed, so `lastOfferClause` (`:11715`) rendered
  the empty string at all 1,871 windows. The brake is armed only by the behaviour it corrects.
- **The deadlock breaker**: `askKey = serializeGameState() + tail` (`:11994`) fires only on
  `askKey == mLastAskKey`. Every activation changes BOTH halves. It detects NO-OP repetition;
  this is PRODUCTIVE repetition. Blind by design.
- **The mana-only gate**: the option is `AGenericActivatedAbility`->`ATokenCreator`, not
  `AManaProducer`, so `allManaOnly` never held. (`stripRepeatAnnotation` is working — #W41-6 is
  not the failure.)

**Cost.** 1,871 round trips, **48.2 M prompt characters (~12 M prompt tokens)**, **5.7 h of
measured round-trip latency**, ~9 h wall, one invariant-00 failure, and — from the OTHER seat —
a denial of decision: deck126 was handed **8,610 priority windows in one turn** and auto-passed
through all of them (D11).

**THE ASK — three affordances, ranked, none of which removes a legal option.**
- **F1 (preferred) — an engine-level repeat-N row.** When an activated ability has been taken K
  times this turn (K = the existing `aa->counters`, already tracked and already rendered) and its
  cost is repayable from state the activation itself restores, render a SECOND row:
  `2. Create human with Thraben Doomsayer, repeated N times, then stop [you name N on the CHOICE
  line, e.g. "CHOICE: 2 (Create human x50)"; the engine performs it N times, re-checking the cost
  each iteration and stopping early if it becomes unpayable, and returns priority to you here]`.
  This is MTG 720/721's announced shortcut expressed as an option row. It ADDS a choice, removes
  none, is legality-safe, collapses 1,871 round trips into one, and is the only option that also
  fixes the two games where the loop is the whole win condition.
- **F3 — render the decline as a REAL LAST ROW** on single-option priority windows:
  `0. Pass priority (take no action this window)`. Cheapest change in the set and independently
  correct across 3,818 windows.
- **F2 (companion, not a substitute) — the repeat tag states the mechanism it hides**:
  `[repeat: activated this turn N times already; you control M creatures. This turn will not
  advance while you keep taking this option; nothing else on your board has changed for the last
  K activations.]` Restriction-first, no affirmative "you should stop" — the model owes the
  surface belief; the surface owes it the mechanism.
- **EXPLICITLY REJECTED: a hard offer cap.** A cap with a truthful annotation does bound the
  loop, but it constrains the choice on exactly the two boards where the loop is the deck's win
  condition, and it would have to fire at an N nobody can justify. Per the owner doctrine —
  legality enforced structurally, choices not constrained — F1+F3 achieve the same termination by
  GIVING the model a reachable stopping act instead of TAKING AWAY a legal one. If a defensive
  bound is wanted anyway, bound it at the ENGINE TICK level (a per-turn activation budget that
  CONVERTS to an F1-style "the engine will perform the remaining N and pass" offer), never by
  deleting the row.
- **Open design question the engine seat could not close by reading**: whether F1's repeat-N
  shortcut is expressible through the existing `DecisionManager` build/apply pair without a new
  contract kind. Answer it before scoping.

**Locus.** priority-window option construction, `AIPlayerGPT.cpp:11719`-`:11890`; the `[repeat:]`
tag at `:9959`; the decline counter at `:12097`.

**Wave-48 prediction.** No game reaches 500 priority windows in one turn; the three deck123 token
games close in **< 60** priority records each (was 1,871 / 1,550 / 223); 21/21 natural. Separable
from the guide half: deck123's guide now ships its own stop (EDIT A47-1, keyed to the board's
creature count) plus a `[repeat:] >= 20` tripwire, so if the loop recurs WITH the tripwire in the
guide, only F1/F3 can fix it — and note **D13**, which says the tripwire cannot fire across a
turn boundary.

---

## D2 — HIGH — the ranged collapse landed on the wrong side of combat
(= seat-engine **E2** / seat-123-130 **H2** / deck152 **G-5** / deck126 **G5**; = `general-strategy.md` **R2**+**R3**)

R8 collapsed the B-rows. **Max B-rows in ANY blockers prompt corpus-wide: 5.** The width is on
the A-rows, and on target menus.
- **Attackers:** `1787786558-ai_baka_deck123-0x55fa8add5b20-vs-ai_baka_deck126.jsonl` seq
  **1570** — **233,662 chars / 1,525 option rows**, 1,524 differing only in a `#N` handle, 178 s,
  answered with a **7,188-char** hand-enumerated `A1, A2, ... A1525`. Second specimen:
  `1787786547-ai_baka_deck123-0x55f1128f75d0-vs-ai_baka_deck146.jsonl` seq **239** (198 rows).
- **Blockers:** `1787786547-ai_baka_deck146-0x55f1115be320-vs-ai_baka_deck123.jsonl` seq **26** —
  **74,063 chars for 2 real options**, 198 A-rows of which 196 normalise to one string, each
  carrying the same 120-char `[NONE of your available blockers can block this attacker...]` tag.
- **Target menus:** `1787786558-ai_baka_deck126-0x55fa89ab8040-vs-ai_baka_deck123.jsonl` seq
  **18** and **19** — **432 and 431 options, 116,128 / 116,148 chars**, ~425 of them
  `Deal 1 damage with Staff of Nin targeting Vampire #N ...` repeating the SAME Staff's full card
  text on every row. That seat's next-largest prompt is 18 KB.
- A-range collapse present in **0/38** blockers prompts and **0/109** attackers prompts; the
  battlefield line in the SAME prompts collapses correctly, so the mechanism exists and is
  pointed at the list that never gets wide.

**Ask.** (a) apply `joinZoneEntries`' rule to A-rows on BOTH combat screens and to target-menu
rows (agreeing name/stats/tags/trade -> one ranged row + count), keeping every label individually
nameable as R8 already proves it can; (b) print a repeated `{card text:}` once per distinct
SOURCE; (c) accept `ATTACK: all` and a range form (`ATTACK: A2-A1525`) in the reply grammar.
**Locus.** `AIPlayerGPT.cpp:17472` / `:17591` (R8's B-row builder), the attackers-screen row
builder, the target-menu builder.
**Wave-48 prediction.** Max attackers prompt **< 30,000** chars, max blockers prompt **< 25,000**,
max target menu **< 25,000**; blockers top-quartile mean **<= 6,000** chars (was 16,678 natural /
23,724 all); attackers p90 latency **< 45 s**; no reply enumerates more than 30 attacker labels.

## D3 — HIGH — render falsehood by scope: `0 of them able to attack right now` over 197 `[tapped - attacking]` rows
(= seat-engine **E3**; = **R4**)

`1787786547-ai_baka_deck146-0x55f1115be320-vs-ai_baka_deck123.jsonl` seq **26**: header reads
`Opponent battlefield (220 permanents listed, of which 215 are creatures, 0 of them able to
attack right now)` on a board where **197 rows below it carry `[tapped - attacking]`**. The count
is `canAttack()` and true to that predicate; in a post-declaration combat window it contradicts
the rows beneath it. Trust doctrine: a true statement in the wrong scope is a lie. **The only
render falsehood found this corpus.**
**Ask.** In any window where attackers are already declared, either suppress the clause on that
board or state the true fact for that scope (`N of them are attacking`). Never delete the token
silently.
**Locus.** `battlefieldHeaderText` / `boardCreatureCanAttackNow`, `AIPlayerGPT.cpp:2172`-`:2206`.
**Wave-48 prediction.** 0 records in which the clause prints a number lower than the count of
`[tapped - attacking]` rows on the same board.

## D4 — HIGH — the collapsed blocker tag picks its representative by BODY SIZE; the decision is about PRICE
(= seat-146-152-162 **E-1** / deck146 **G-1**; = **R5**)

`1787786538-ai_baka_deck146-0x561e0aa9c320-vs-ai_baka_deck126.jsonl`, kind `attackers`, seq
**50** (turn 18, life **18** vs 9): every A-row reads `they have 5 untapped creatures that could
block this one, biggest Vampire (1/1) (you kill it, your attacker lives (lifelink: they gain 1,
and their converter takes 1 off you))`. The four hidden blockers: `Perimeter Captain (0/4)`
(`auto=@combat(blocking) source(creature[defender]|mybattlefield):may life:2 controller` — fires
for EVERY defender they block with), `Pride Guardian (0/3)` (`:life:3 controller`), two
`Wall of Omens (0/4)`; `Sanguine Bond` = `auto=@lifeof(player) from(*[-lifefaker]|*):life:-thatmuch
opponent`. All verified in the primitives. The printed representative carried the **smallest**
price on the board. The seat priced the swing at 1, paid **11 life**, lost the game at -1 two
turns later, and repeated it at seq **59** (life 6 vs 5).
Lane K's converter fold has made the PRICE the load-bearing half of the tag, so "biggest" is now
the wrong selector.
**Ask (restriction-first, nothing deleted).** Keep the biggest-body representative and add a
price-selected one (`most expensive to attack into, Perimeter Captain (0/4) (neither dies
(blocking trigger: they may gain 2, ...))`), or append the summed worst case (`[if they block
with every creature on that list, they gain up to N and your converter loss is N]`).
**Second-order consequence to carry into the reviewer briefs:** any guide rule keyed to a COUNT
of printed clauses is unsatisfiable under this collapse — deck146's was, which is why it never
fired (skill amendment 91).
**Wave-48 prediction.** No reply on any seat prices a collapsed-tag swing using only the printed
representative's number while a larger price sits on the same battlefield line.

## D5 — MED — `{card text:}` on option rows truncates mid-word and drops whole abilities
(= seat-engine **E5**; = **R6**)

7,291 occurrences, 28 distinct tails. At least four drop a REAL ability: Thraben Doomsayer
`"...other creatures you..."` (**2,015x**, the Fateful-hour lord clause gone), Lord of Lineage
`"...onto the battlefield. //"` (**1,751x**, the entire back face gone), Siege-Gang Commander
`"...{1}{R}, Sacrifice a Goblin: Siege-Gang"` (165x), Sorin `"-2: Destroy target creature. Its"`
(32x). R6's per-clause budget was applied to `{effect:}` only.
**Repro.** `1787786520-ai_baka_deck123-0x55c267f9bff0-vs-ai_baka_deck152.jsonl` seq **5**;
`1787786540-...-vs-ai_baka_deck162.jsonl` seq **1921**.
**Ask.** Extend the clause-aware budget to `{card text:}`: never cut mid-word, never cut inside a
`--` clause, never cut before a `//` face separator.
**Wave-48 prediction.** Mid-word cuts **0/N**; Doomsayer's Fateful-hour clause and Lord of
Lineage's back face present on every row that names them.

## D6 — MED — the menu header cannot name an `auto=choice name(...)` carrier, and that family's labels are raw script tokens
(= seat-engine **E6** + **E7**; = **R7**)

`A choice is required - choose an option:` is **4/4** (was 4). Emitter class now identified:
**3 are Silverquill Command's mode chooser** — `1787786526-ai_baka_deck146-0x558706737270-vs-ai_baka_deck130.jsonl`
seq **30**, `1787786536-ai_baka_deck146-0x55f47fabb1c0-vs-ai_baka_deck125.jsonl` seq **20**,
`1787786538-ai_baka_deck146-0x561e0aa9c320-vs-ai_baka_deck126.jsonl` seq **20** — and **1 is Peer
into the Abyss's target chooser**, `1787786530-ai_baka_deck162-0x55681e07a590-vs-ai_baka_deck152.jsonl`
seq **21**. Both cards are `auto=choice name(...)` alternative lists (`borderline.txt:102762` /
`:82589`, verified); the three-rung `resolveOwningCardName` ladder does not reach that family.
Wave 45's Teferi emitter is fixed and now renders `Choose an option for Teferi, ...`.
Same repro carries **E7**: `1. target opponent` / `2. target controller` — the literal `name(...)`
strings from `borderline.txt:82590-1`. **"controller" means YOU**, which no reader can derive.
**Harm is currently ZERO** — all four menus were answered correctly with **0 fallbacks** (was 2
of 4); the exemplar fix did that. This is a naming/labelling residual.
**Locus.** `AIPlayerGPT.cpp:14341`-`:14359`; the menu option text for `auto=choice name(...)`
branches. The engine seat did NOT trace why the ladder fails here — that needs a probe.
**Wave-48 prediction.** `A choice is required` **0/N**; every `auto=choice` menu header names its
card; `target controller` **0** occurrences in any option label; fallbacks on those menus stay 0.

## D7 — MED — the stack line and the counter-target clause carry no keyword tags
(= seat-125-126 **MED #2** / deck125 **G1**; = **R8**)

`1787786522-ai_baka_deck125-0x5555ed1493c0-vs-ai_baka_deck126.jsonl` seq **66** (turn 33): stack
reads `1 (top): opponent's Perimeter Captain {w} (creature 0/4) [spell]`; the counter row reads
`- can target on the stack: Perimeter Captain {w} (creature 0/4) {target text: "Defender -- ..."}`.
Neither carries `[defender]`. The SAME card on the battlefield line in the same corpus reads
`Perimeter Captain #1 {w} (0/4) [defender]`. Every counter/removal rule in the pool is written in
battlefield vocabulary, so a rule keyed to a tag is unsatisfiable exactly where the decision is
made — and this seat spent a Cancel on a wall accordingly (1 of 28 counters). Same defect class
as the own-hand count: the fact is at the call site.
**Ask.** Emit the battlefield line's keyword set on the stack line and on `can target on the
stack:` clauses.
**Guide dependency this retires:** the deck125 guide currently works around it AND carries a
negative-existence sentence about the render (boundary finding **B3**). Both go when tags land.
**Wave-48 prediction.** Stack/target clauses carry the same keyword set as the battlefield entry;
a counter spent on a stack object of printed power 0 or whose `{target text:}` begins "Defender":
**0**.

## D8 — MED — lane J's `[DRAW PRICE:]` row tag is not reaching the rows that draw
(= seat-123-130 **M2**; = **R9**)

The `DRAW PUNISHERS` summary line shipped widely (**2,043 prompts**) and works (0 voluntary draws
at any punisher window corpus-wide; 0/19 at deck130, 0/13 Revelations at deck125). The per-row
tag fired **twice in the whole corpus**, both in
`1787786557-ai_baka_deck146-0x562e93be9370-vs-ai_baka_deck162.jsonl` seq **27** and **32** — on
seq 32 the tagged draw was declined, so the one clean window it got, it won. n=2: **effectively
UNTESTED**. Counter-repro where it should have fired and did not:
`1787786551-ai_baka_deck130-0x564bb37cbef0-vs-ai_baka_deck152.jsonl` — and at deck130 vs deck162
seq **34/35/42/43**, where the summary line and a `cycling with Forgotten Cave [cost: {r}, Cycle]`
row are on the SAME screen and the row carries no price.
**Ask.** Emit `[DRAW PRICE: N]` on every row that causes its controller to draw while a punisher
line is printed. General principle behind it (deck130 G47-5): where the frame carries a summary
of a hidden cost, the ROWS that incur it carry the number.
**Wave-48 prediction.** The tag emits on every such row (report N); draws at or below the summed
cost stay **0/N**.

## D9 — MED — ANNOUNCE_X re-shows a carried plan that already names an X, and nothing marks the maximal row
(= seat-123-130 **M1** / deck130 **G47-4**; = **R10**)

`1787786534-ai_baka_deck130-0x562ec751eb60-vs-ai_baka_deck126.jsonl` seq **37**: `YOUR PLAN (as
you last stated it): Cast Starstorm at X=3 to kill both Pride Guardians`, reply `CHOICE: 3
(X = 3)` restating it verbatim, over `2. X = 4 {kills THEIRS: Pride Guardian x2, Perimeter
Captain; YOURS: none}`. The X was fixed at the cast row one screen earlier, before the kill lists
existed. This is the wave-46 pregame-chain shape (fixed for pregame asks in this binary)
relocated onto the X menu. Every comparative phrase on that screen is about MANA SAVED; nothing
marks the row that kills the most.
**Ask.** Suppress the carried-plan block on ANNOUNCE_X, and/or mark the selected row once per
menu (`<- most kills at any affordable X that costs you nothing`) — a summary of rows already
printed, the same status as the header's existing `option 1 is the LARGEST X`.
**Wave-48 prediction.** Announce answers restating a carried plan's X verbatim while a larger
free-kill row is on screen: **0**; announce answers landing on the largest free-kill row N of N.

## D10 — MED — a cast ask and the priority ask that follows it are answered as one menu
(= seat-146-152-162 **E-5** / deck146 **G-2**; = **R16**)

`1787786536-ai_baka_deck146-0x55f47fabb1c0-vs-ai_baka_deck125.jsonl` seq **35** (cast menu, 5
rows, answered `Cast nothing right now`) immediately followed by seq **36** (priority, 2 rows:
`becomes beholder`, `Flip Side`) answered `CHOICE: 8 (Cast Acererak the Archlich)` — out of range,
naming a card from the PREVIOUS menu. One of the corpus's two off-menu hallucinations (the other:
`1787786526-ai_baka_deck130-0x558707a70550-vs-ai_baka_deck146.jsonl` seq **24**, `CHOICE: 4` of 3,
Rorix Bladewing). **The parser was right to refuse**; the prompt composition invited it.
**Ask (prompt-side, restriction-first).** (a) when a priority ask directly follows a cast ask in
the same phase, say so — "you have already answered this turn's casting question; these are
ACTIVATED ABILITIES only, and no card in your hand appears below"; (b) state the range
explicitly — "answer with a number from 1 to 2, or 0".
**Wave-48 prediction.** Off-menu replies naming a card from the immediately preceding menu: **0**.

## D11 — MED — the GAME LOG narration does not collapse repetition
(= seat-engine **E12**; = **R13**)

~180 identical `- You used: Create human with Thraben Doomsayer` / `- Your Thraben Doomsayer
created a 1/1 Human token` pairs per prompt, held flat only by the log's
`(...earlier events trimmed...)` cap — so ~90% of a 26 KB prompt is one repeated sentence and the
model is never given a count. The battlefield line in the same prompt collapses 1,893 tokens into
`Human #3-#1895 (1/1) [...] x1893`, which is the only reason the prompt is 28 KB and not ~2 MB;
the narration's flatness is a TRIM, not a collapse. Repro: loop file seq **1921**.
**Ask.** Collapse consecutive identical narration events to `- You used: Create human with
Thraben Doomsayer (x180, creating 180 1/1 Human tokens)`.
**Wave-48 prediction.** Mean priority prompt chars in a token-loop game **< 8,000** (was 25,775).

## D12 — MED — the battlefield ranged collapse is run-length-only, so interleaving defeats it
(= seat-engine **E8**; = **R14**)

`1787786547-ai_baka_deck146-0x55f1115be320-vs-ai_baka_deck123.jsonl` seq **26**: alternating
`Vampire`/`Human` tokens rendered one row each (`Vampire #200 ...; Human #4 ...; Vampire #202
...`) while the adjacent run collapsed to `x174`; two runs `#1-#22` and `#23-#196` were split
only by a single interposed Doomsayer row.
**Ask.** Group by identical rendered TEXT, not by adjacency (or sort the entry list before
collapsing). R8's label-nameability note already covers the non-contiguous case.
**Locus.** `joinZoneEntries`.
**Wave-48 prediction.** No prompt contains more than 5 uncollapsed rows whose normalised text is
identical to another row on the same board line.

## D13 — MED — **the `[repeat:]` receipt is scoped per-ability and per-TURN, so the loop's only annotation resets under it**
(= seat-125-126 **HIGH #1(a)** / deck126 **G6**, mechanism corrected here by direct measurement)

The seats reported the tag "under-counting 15 against 1,540 creatures" and read it as the tag
counting one ability among several. **Measured directly on
`1787786558-ai_baka_deck123-0x55fa8add5b20-vs-ai_baka_deck126.jsonl`**, the fuller mechanism is:
- seq **1099**, turn 10, opponent's Main-1: `Create vampire with Lord of Lineage [cost: Tap]
  [repeat: activated this turn 650 times already; you control 1075 creatures]` — max in that file
  is **1,099**;
- seq **1565**, turn **11**, Upkeep: `[repeat: activated this turn 14 times already; you control
  1539 creatures]`; seq **1567**: `15 ... 1540 creatures`.
So the counter is **truthful to its own wording** ("this turn") and **resets at every turn
boundary**, while the loop does not; and it is scoped per ABILITY, and this deck feeds the same
loop from two different makers. **The defect is at the level of the affordance's purpose**: it is
the only receipt on the screen, it is D1's F2 candidate, and as scoped it reads as a fresh small
number on a board that has already looped 1,099 times.
**Direct downstream consequence, and it is why this is docketed rather than filed as a note:**
deck123's wave-47 guide now ships a `[repeat:] N >= 20` hard tripwire (EDIT A47-1). On the
turn-11 board above the tag reads **15** at **1,540 creatures** — the tripwire cannot fire in the
game shape it was written for. The board-side half of that guide's stop (the `of which M are
creatures` count) is unaffected and is the reliable half. Reported to the deck123 reviewer as
boundary finding **B5**.
**Ask.** Either widen the tag's scope to the loop rather than the turn (`activated N times this
turn, M times since this board state last changed otherwise`), or state the scope in the tag so
neither a pilot nor a guide keys a threshold to a number that resets under it. F2's wording in D1
already carries the "this turn will not advance" clause that makes the scope legible.
**Wave-48 prediction.** Report, per token-loop game, the tag's maximum value against the seat's
own activation count for that game; the two should be reconcilable from the tag's own words.

## D14 — LOW — attacker rows state the upside of attacking and never its cost
(= seat-engine **E9**; = **R19**)

The affirmative `[no creature they control can block this attacker]` rendered on 60 attacker rows
and **60 were attacked** at three seats (42/42) — a clean win, and it has ABOLISHED the untagged
A-line. The 11 corpus-wide declines are **10 of one creature in one seat**: `Dwarven Blastminer
(1/1)` (`mtg.txt:33251`, `auto={2}{R}{T}:destroy target(land[-basic])`) — attacking taps it and
forfeits the ability, so the decline is defensible and guide-mandated. The row states the upside
and is silent on the cost.
**Repro.** `1787786551-ai_baka_deck130-0x564bb37cbef0-vs-ai_baka_deck152.jsonl` seq **12, 17, 25,
38** (and 68/80/88/97/106/116).
**Ask.** Where an attacker has an untapped-only activated ability, name it on the A-row —
`[attacking taps it: you lose {2}{R},{T}: destroy target nonbasic land this turn]`. No
recommendation. **Do not chase the 64%-at-deck130 attack rate; the tag is not the cause.**

## D15 — LOW — the modal-DFC Pathway land route is not covered by the land-drop treatment
(= seat-engine **E10**; = **R18**)

28 records: the `Choose an option for <Pathway>:` menu still requests a PLAN line and omits the
`(playing a land costs no mana and uses up no cast: it does not reduce what you can cast this
turn)` consequence sentence the plain land ask now carries.
**Repro.** `1787786532-ai_baka_deck152-0x5605e480dfb0-vs-ai_baka_deck126.jsonl` (Barkchannel
Pathway menu, first of 28).

## D16 — LOW — two counters are fuzzy instruments
(= seat-engine **E11** + **E-6** / deck126 **G7** / deck152 **G-7**; = **R21**+**R22**)

(a) `mana_only_windows_skipped` is built from `shown` AFTER the two-decline retirement filter, so
"mana ability + an option already retired this turn" counts as a mana-only window. Skips nothing
live; over-reports. Locus `AIPlayerGPT.cpp:11930`-`:11943`. (b) `dropped_assignments` counts
`Bn:none` pads — `1787786520-ai_baka_deck152-0x55c266c5afc0-vs-ai_baka_deck123.jsonl` blockers
seq **26** answered `BLOCKS: B1:A1, B2:none, B3:none` on a two-blocker screen and recorded
`dropped_assignments: 1`; nothing was lost, and the counter is the seat's instrument for REAL
drops. (c) Reporting fix: the gameend record should carry the count of windows that DID reach the
model alongside the skip count.

## D17 — LOW — a degenerate decode deserves a counter distinct from `unparsed_reply`
(= seat-125-126 **MED #3** / deck125 **G4**; = **R20**)

`1787786524-ai_baka_deck125-0x555dc22a73c0-vs-ai_baka_deck123.jsonl` seq **39** (turn 22, 4
options, 25,138-char prompt): a **16,271-character** reply of one repeated token (`ai茧ai...`),
no CHOICE line, `latency_ms` **798,216**, recorded only as `unparsed_reply`. deck126's max is
800,769 ms (`...vs-ai_baka_deck162.jsonl` seq 15, which DID answer), so an ~800 s ceiling exists
somewhere in the client. 3 such events corpus-wide. Inference-layer, not a prompt defect — but an
operator reading the ledger cannot tell a 16 KB token-loop from an ordinary parse miss.
**Not investigated:** whether the ~800 s figure is a client timeout.

## D18 — LOW — surfaces that are parity gaps, not defects
`{leaves N ...}` is on cast rows only and not on mana-costing activated-ability rows (15 Elixir
windows; deck125's tap-out gate covers "cast OR activated" — **R15**) · the `cycling` row in a
`Choose an option for <card>:` menu carries no `[cost:]` bracket while its sibling does
(`1787786526-ai_baka_deck130-0x558707a70550-vs-ai_baka_deck146.jsonl` seq **41**; 0 wrong choices
in 4 windows — **R23**) · `manaAvailableLine` drops the spelled-out number above ten (41 emissions
at deck130 vs152 past turn 20, 4 at deck123 — **R24**, carried from wave 46) · `Cast nothing right
now` is the only unannotated row on a screen where every other row carries a consequence
(**R17**) · the converter block names only one direction of a converter PAIR (**R12**) · the
`{right now: ...}` per-branch preview is not general (**R25**) · a LETHAL blockers window does not
carry the castability fact its own priority promise invites (**R26**).


## D19 — LOW — one keyword, three different rendered wordings across three surfaces (found in the boundary pass)
(= `general-strategy.md` **R27**)

`menace` renders as three different strings depending on which surface prints it, and none is a
substring of another:
- battlefield line: `[menace (can't be blocked except by two or more creatures)]` — **64**
- attacker A-row: `[menace - cannot be blocked by fewer than two creatures; they have N untapped
  creatures that could join such a block, ...]` — **36**
- blockers-screen A-line: `[menace - only a block by TWO OR MORE of your creatures counts; one
  creature alone does not block it at all]` — **10**
A guide rule keyed to any one of them is unsatisfiable on the other two, which is D4's and D7's
defect class in a third place. Evidence that it already bites: deck146's guide quotes a bare
`[menace]` as the battlefield form (it never renders), and deck126's new blocking rule keys on
the third string only — correct for a blocking rule, silent everywhere else.
**Ask.** One canonical keyword string per keyword, plus whatever surface-specific clause the
window adds after it, so a rule can key to the keyword and read the clause.
**Wave-48 prediction.** Every rendered `[menace ...]` tag begins with the same canonical
substring; report the distinct-form count for `menace`, `defender`, `flying` and `lifelink`.

---

# DISCHARGED — SET A: wave-46 GUIDE edits, per deck

Adjudicated by the reviewers on their own seats' denominators. `->` names this wave's edit where
the rung was re-formed.

| deck | rung / edit | verdict | evidence |
|---|---|---|---|
| **123** | A46-1 RULE -1 mulligan chain | **PASS 0/0** | 8 asks, longest chain **1** mulligan, no third look (wave 46: chains of 7 and 6 in 2 of 6 games) |
| 123 | A46-2 CHECK 1 enchantment branch | UNTESTED | shape did not recur |
| 123 | A46-3 Damnation | **PASS 0 of 23 offers** | 0 casts, 0 enchantment-removal claims (wave 46: a game-losing cast) |
| 123 | A46-4 edict planeswalker fact | **SPLIT: PASS on planeswalkers, FAIL 2/2 on the count** | seq 23 at N=5, seq 408 at N=3; new false belief "I choose the body" -> **A47-2** |
| 123 | A46-5 cap deleted + power stop | **take-rate PASS 3,743/3,763 (99.5%); the STOP FAILED** | the stop was written on a quantity the action cannot move -> **D1** + **A47-1**, skill 88 |
| 123 | RULE 0 land drop | **PASS 0 of 19 declines** | second corpus at zero |
| 123 | RULE 0 costs-as-integers | **FAIL 1** | `...deck123-...-vs-deck152` seq 27: `Mana available: 1 total`, plan on a `{2}{b}` instant -> **A47-3** |
| **125** | W22 land drop | **PASS 93/93, 0 declines** | denominator up from 40 |
| 125 | W23 X == 3 | **VOID by its own terms** (the hand count shipped) — measured **12 casts, 0 above 3**, X=3 in 8/8 affordable windows | superseded by W30 |
| 125 | W24 Staff below the counter threshold | **FAIL 6/7** | four waves: 6/11, 3-4/7, 4/7, 6/7 -> **W4**, skill 89 |
| 125 | W25 sweeper into an all-defender board | **FAIL 4/13**; second half **PASS 0/64** | all 4 read `0 of them without a restriction against attacking`; all 9 correct casts read >=1 — **the header discriminates 13/13** -> **W2** |
| 125 | W26 counters on a mana-only artifact | **PASS 0/28** (was 2/39) | one counter hit a wall via the untagged stack line -> **D7** + **W6** |
| 125 | W27 Revelation under a draw punisher | **PASS 0/13** | wave 46: 1/1, at 6 life, in a loss |
| 125 | W28 Emrakul taken | **FAIL 1/4** | rule collision with the tap-out gate -> **W7**, skill 97 |
| 125 | W29 own-turn spends below the counter threshold | **FAIL, 12 identifiable** | Staff 6, Verdict 2, Path 2, Lightmine 1, Judgment 1 |
| **126** | D22 blocker benched with no enchantment above 8 | **PASS as written; its mirror failed** | the one `BLOCKS: none` was at `you would be at 7` -> **F4** |
| 126 | D23 "dies"-vs-"lives" offered together | **0 of 0 — no window** | not a pass |
| 126 | D24 Chromatic Lantern | **near-PASS 1/3 wrong; positive half DISCHARGED; duplicates 0/~40** | wave 46: 5/5 wrong, 0/1 positive. **Biggest single-rule improvement in the pool**; not re-edited (#35) |
| 126 | D25 `ATTACK: none` / Vampire surplus | UNTESTED both halves | 2 attack windows |
| 126 | D26 land bottomed below three | UNTESTED | 0 bottoming windows |
| 126 | D27 Battlement tap with no cast | **PASS 0/3** (was 1/16) | denominator restated per skill 87 |
| 126 | D28 `BLOCKS: none` | **FAIL 1/11**; D18's flying conjunct UNTESTED a THIRD wave | -> **F4** |
| 126 | P-C5 boundary totality close | **PASS, and the wave's cleanest guide result** | 47 of 107 `Cast nothing`, but **38 of the 47 had nothing but a duplicate Lantern** and only **4** had a matched entry (wave 46: 24 of 24). Reading the raw rate alone would have condemned a working edit |
| **130** | D46-1 draw punishers | **PASS 0 of 19** | includes a correct NEGATIVE check at 3 life with no punisher line |
| 130 | D46-2 step 1 largest free X | **FAIL 1/2** | internal contradiction with the KEY CARDS entry -> **D47-1**, skill 96 |
| 130 | D46-2 step 2 collapsed runs | **PASS 0/2** | |
| 130 | D46-3 cycling tripwire | **PASS 0/4** | third-corpus failure did not recur; guide + annotation shipped together — **unattributable, and stated as such** |
| 130 | D46-4 Blastminer gate | **PASS 0/17** | wave-46 contradiction closed |
| 130 | D46-5 race carve-out | **PASS** | 30 attackers windows; 13 declines all the deliberate Blastminer floor |
| **146** | 146-A Silencer / Emeria's Call entries | **SPLIT: primary PASS, first-match 39/67** (was 46/73) | -> **146-B** |
| 146 | 146-B Silverquill Command ordering | **FAIL** | 29 offers, 3 casts, 13 declines with nothing above matching; **unmoved by the wave-46 edit** -> accountability clause, not a fourth restatement |
| 146 | 146-C CHECK 0 lethal carve-out | **PRIMARY UNTESTED / SECONDARY FAIL** | **10 non-lethal Upkeep animations of 27 offers**, 40 mana; my own wave-46 edit over-fired -> **146-C/E**, skill 93 |
| 146 | 146-D Lolth `+0` life floor | **PASS** | 16 offers, 1 taken at life 11 (wave 46 took it at 5 and 2) |
| 146 | lifegain-wall rule | **could not fire** | keyed to `(blocking trigger:` and to a COUNT the collapse suppresses -> **D4**, skill 90 + 91 |
| **152** | 152-A RULE -1 M=6 branch | **FAIL 1/1, and it is the seat's deciding loss** | `152 vs130` seq 1-3: guide-mandated keep of a one-land six; **land drops at turns 0,10,12,14,22,24,28,32 and not one cast in 33 turns**, -7 to 17 — the only game in the corpus with no cast in it |
| 152 | 152-B block floor | **PASS 0/4**; vocabulary half 0/4 | the model acts on the header without quoting it — skill 93 |
| 152 | 152-D counter spreading | **FAIL 2/13** (was 3/7) | both in games WON; violated-unpunished at n=2, neither loosened nor re-worded (#45/#64) |
| 152 | 152-E land drops | **PASS 16/16** | confounded with lane L's R13; stated as unattributable |
| 152 | shared cross-deck STOP | **FAIL 2 of 3 card-windows, and both are ONE decision** | `152 vs146` seq 18, a carried plan restated over two `your attacker dies` tags -> **152-B**, skill 94. Row 3 of the table is the rule working better than ever: the CARD held while three TOKENS on the identical tag were sent |
| **162** | 162-B converter/lifegain hold | **PASS 2/2** | the exact matchup that produced a 19-to-0 turn last corpus; **won 21-0 at turn 12** |
| 162 | 162-C untagged A-line rung | **UNTESTABLE BY CONSTRUCTION** | lane K's affirmative tag abolished the untagged A-line — 0 such rows on any seat |
| 162 | 162-D/E blocking floors | **PASS 0/0 and 0/0, near-vacuous** | 5 blockers windows, all at life 20-21 |
| 162 | Rule 2 draw-engine brake | **39/42 on the rule's own text** | 3 breaks, all in games won or excluded; third corpus carried without an edit |
| 162 | RULE 1 punisher-first | **compliance excellent; the ONLY loss was scored INSIDE it** | two punishers on one menu, no tiebreak -> **162-A**, skill 95 |
| **pool** | wave-46 boundary C1-C5 | C1/C2 (RULE -1 hoist + carried-plan clause) **PASS, vacuously** — lane L removed the mechanism; C3 (deck126 defender-allocation) **PASS 0** — no reply argues from a body count; C4 **PASS 0**; C5 (deck126 ordered list) **shipped and is the wave's cleanest result** | |

---

# DISCHARGED — SET B: wave-47 engine lanes, per lane, with counts

Adjudicated against the emitter's ACTUAL string.

| lane / commit | prediction | verdict | counts |
|---|---|---|---|
| **1c71d5301** | `Your hand (N cards):`; replies stating a wrong own-hand size 0/N (was 3/4) | **PASS** | header on **5,644/5,644** decision prompts; **0** wrong-size assertions. Directly unblocked deck125's X lane: **4/17 -> 12/12** |
| **a7310f01c** (i) | menu header names ability carriers; `A choice is required` 0/N (was 4) | **FAIL — still 4/4** | emitter string unchanged; class identified as `auto=choice name(...)` -> **D6**. Harm zero: all four answered correctly, **0 fallbacks** (was 2/4) |
| **a7310f01c** (ii) | exemplar from option 1; `Cast ...` into cast-free menus 0/N; stale_echo on non-card menus 0 | **PASS** | exemplar rewritten and quoted verbatim on every menu; **0** `Cast *` replies into a cast-free menu in 4,128 priority prompts; 1 corpus-wide `stale_echo` and it is a card-menu land ask. The priority tail's placeholder is **not latched** (4,127/4,128) — leave it alone |
| **a7310f01c** (iii) | repeat tag from the first repeat; single-option loop windows > 100 s: 0 | **RENDER PASS, PREDICTION FALSIFIED / BEHAVIOUR FAIL** | tag fires from repeat 1 and counts correctly; **8 of 3,818** single-option windows exceeded 100 s (max 231.7 s), 5 of them in the three loop games, 2 of those the discarded retractions. **The windows were FAST and there were 1,894 of them** — the failure mode was never latency. Take rate on tagged rows: 146 3/6, 152 1/3. -> **D1**, **D13** |
| **J 2860f78c2** (i) | DRAW PUNISHERS line; draws at/below summed cost 0/N (was 2/2 lethal) | **PASS** | line on **2,043** prompts; **0** voluntary draws at any punisher window corpus-wide |
| **J** (ii) | `[DRAW PRICE:]` row tags | **NOT SHIPPING / effectively UNTESTED** | **2** emissions corpus-wide, both in one seat -> **D8** |
| **J** (iii) | annotated `Cast Card Normally / cycling` menu; Cast-Normally with a cycle plan 0/N | **PASS 0/8** | third-corpus failure did not recur (guide edit shipped in the same binary — unattributable) |
| **J** (iv) | X=0 null-cast callout + life/draw preview; X=0 answered where X>=1 affordable 0/N | **PASS 0/29** | X=3 x9, X=2 x2, X=1 x2, X=4 x1, decline x11. Menus largest-first 12/12 |
| **K d30871747** (i) | converter doubling folded into blocking-trigger/lifelink tags; life lost between attackers records 0 | **RENDER PASS; BEHAVIOUR SPLIT** | both voices ship. **162: 0 sends, 0 life lost. 146: 3 windows, all sent, 11 then 5 life.** The difference is entirely which guide has the rule |
| **K** (ii) | affirmative `[no creature they control can block this attacker]`; ATTACK: none on it 0/N; attack rate >= 90% | **TAG PASS, DECISIVELY; metric FAIL 11/60 and the 11 are correct** | **60 rows, 60 attacked at three seats (42/42); 1,726/1,726 at deck123.** The 11 declines are **10 of one Dwarven Blastminer** whose `{T}` ability attacking forfeits -> **D14**. The untagged A-line no longer exists |
| **K** (iii) | zero-power STOPS scoped `THIS COMBAT`; wall on a dies-row while a lives-row is offered 0/N | **PASS 0/12** | 5/5 used at deck162; not over-read into a general licence |
| **L 2a4ff28be** (i) | `{if you pass here, this option is not offered again this turn}` at the decline cap; lethal-in-upkeep activations 100% | **RENDER PASS (696-1,060 rows), BEHAVIOUR UNTESTED, AND STRUCTURALLY UNREACHABLE IN THE FAILURE MODE** | 82 of those windows were then passed; no lethal-in-upkeep window arose (lowest opponent life at any manland window: **5**). The clause can only appear AFTER a decline, so it is absent from any window the model never declines — precisely the loop. -> **D1** |
| **L** (ii) | priced Mulligan row + NO carried plan on pregame asks; third-look chains 0/N (was 9) | **PASS — the biggest single behavioural change in the corpus** | 50 priced asks, **0** carrying a `YOUR PLAN` block; **8 mulligans, every one a single look, 0 chains, no seat below keeping 6** |
| **L** (iii) | land-drop asks: no PLAN request + consequence sentence; max 2-option latency < 200 s (was 453); declines 0/N | **PASS on 3 of 4** | 386 land asks, **max latency 50.8 s**, <=2-option max 30.9 s; declines **2/386** (0.5%) — a technical FAIL not worth a fix. Residual: the DFC Pathway route -> **D15** |
| **M 8b6545b48** (i) | per-clause `{effect:}` budget keeps the ultimate; truncated loyalty abilities 0/N | **PASS 0/61** | 61 rendered planeswalker `{effect:}` strings, **61/61** carry a negative-loyalty clause |
| **M** (ii) | own-side `{effect:}` minus permanents on an option row; own bare permanents 0/N | **PASS** | 0 unexplained bare own permanents in 612 decisions at the two seats that measured it |
| **M** (iii) | blockers ranged collapse; blockers p90 <= 60 s, tail mean <= 4,600 chars | **PASS on the B side / FAIL on the metric by 3.6x** | max B-rows corpus-wide **5**; max A-rows **198**; B-range collapse in 2/38 blockers prompts, A-range in **0/38** and **0/109**. p90 latency 53.0 s natural (PASS) / 180.3 s all; tail mean **16,678** natural / 23,724 all. -> **D2** |
| **M** (iv) | `{c}` in colour sets; `{1}`/`{x}` in a colour set 0/N (was 18) | **PASS 0/N** | `{c}` in 186 emissions; 0 of 549 clauses carry a bare count or `{x}` |
| **M** (v) | `N of them able to attack right now` + `{leaves N ...}`; sweeper casts into all-defender boards 0/N | **STRINGS PASS (5,586 / 678 records); one RENDER FALSEHOOD by scope; the behavioural half FAILS at one seat** | the two header wordings are a perfect complement by turn. The count **discriminated deck125's sweeper casts 13/13** and made the four-wave Staff lane measurable for the first time — but the guide had to be rewritten onto them (W2, W4) and was not, so 4/13 sweepers still went into all-defender boards. Scope falsehood -> **D3** |
| **R7 prior** | mana-only auto-pass gate | **PASS — the gate is why the pathological game finished** | **8,673 skips, all at one seat, 8,610 in ONE game**; the other five games total **63** against wave 46's 180 across six. **0 all-mana menus leaked** (wave 46: 3). Counter increments at exactly one site, verified. Fuzziness -> **D16** |
| **Fair hands (lane A)** | carried | **PASS** | opening land counts 2-4 across 13 kept hands, including three 2-land sevens |

---

# CARRIED — open, re-measured, not re-derived
- **`manaAvailableLine` number word above ten** (wave-46 L1): 41 + 4 emissions. -> **D18/R24**.
- **Clue token textless** (wave-46 L3): **UNTESTED** — no Clue on any seat this corpus.
- **damage OBJECT bare** (27/273, wave 46) and **the target list's current-size clause**: not
  re-measured this corpus; carried without new evidence.
- **`ability$` pay-or-lose punishers undetected** (lane J residual): carried.
- **R7 name-substring skip**: carried.
- **R4's one-window-turn case** (no clause when declines == 0): now understood as the general
  structural fact in **D1**; the manland offer-TIMING skew persists unchanged (**22 Upkeep / 6
  Main-1 / 2 Blockers / 0 Main-2**, as expected from the annotate-rather-than-resurface route).
- **Baka menace gang-block probe** (red pin): still open, and the corpus has now produced the
  GUIDE side of the same mechanic — deck126 put all five blockers on one 3/2 twice while two
  `[menace]` attackers went unblocked (skill amendment 101). The render was complete both times.
- **The floating-mana disappearance observation** (wave 46, deck126): **not reproduced** on any
  seat this corpus. Left un-docketed, as wave 46 left it.

---

# FALLBACK CLASSES — 9 in the 20 natural games / 3,710 decisions = **0.243%** (in band)

| n | class | records | disposition |
|---|---|---|---|
| 3 | **model decode degeneration** (CJK / word-salad, up to 16 KB of one token) | `deck125-0x555dc22a73c0-vs-deck123` seq **39** (798 s); `deck123-0x55fa8add5b20-vs-deck126` seq **401**, **403** | inference-layer; wants its own counter -> **D17** |
| 2 | **off-menu hallucination** — a real card named with an index past the list | `deck130-0x558707a70550-vs-deck146` seq **24** (`CHOICE: 4` of 3, Rorix); `deck146-0x55f47fabb1c0-vs-deck125` seq **36** (`CHOICE: 8` of 2, Acererak) | the parser was right to refuse; prompt composition invited the second -> **D10** |
| 2 | **retraction with a mid-line replacement** — the model's own stop discarded | `deck123-0x55f1128f75d0-vs-deck146` seq **34**, **221** | **SHIPPED 5ce8f8a5a** (below) |
| 1 | **`empty_reply` at the 900 s wall** | `deck123-0x55610f8f41d0-vs-deck130` seq **7**, `latency_ms 900046` | exactly the configured `gpt timeout=900s`; the heuristic answered. **No action** |
| 1 | **`stale_echo`** | `deck152-0x55ac0c573fd0-vs-deck125` seq **8**: menu `1. choose a land / 2. Decline`, reply `CHOICE: 1 (Plains)` | a legal, unambiguous number thrown away for its parenthetical, which named the land the model intends at the NEXT ask. Wave-46's E-1 fix shape (take the number when in range and unambiguous, log the mismatch) still applies to this one shape. Cost: 1 decision, down from 4 |
| +2 | in the EXCLUDED loop game (`unparsed_reply` seq 560, 658) | | note: **the only thing that ever advanced the dead-locked turn from Upkeep to Main-1 was one of them** — the loop's sole escape hatch in 9 hours was a model failure |

---

# ALREADY SHIPPED ON MASTER SINCE THE CORPUS — recorded, NOT docketed

| commit | item | prediction for wave 48 |
|---|---|---|
| **5ce8f8a5a** | **E1 — prose re-answer replacement.** `choiceRetractedNoReplacement` (`AIPlayerGPT.cpp:11490`) scanned for the replacing `CHOICE:` **line-leading only** (skipping `space \t * # -`), so `So, CHOICE: 0 (pass).` after the reasoning was not found, `choice` became -1, the heuristic decided — and because `choice != 0` the stop did not increment `mPassDeclineCount` (`:12097`) either. Both occurrences are the model correctly reasoning ITSELF out of a token loop, and they are the corpus's two most expensive deliberations (216 s and 190 s, single-option loop windows). A mid-line `CHOICE: N` after the retraction marker that validates against the option set is now the decision, and a resolved `0` counts as a decline. | `retracted_choice` records carrying a resolvable mid-line replacement: **0/N**; any remaining `retracted_choice` has no parseable `CHOICE:` anywhere after the retraction. Secondary: at least one loop window exits by a resolved mid-line pass, and that pass arms lane L's decline clause on the next window. |

---

# PROMPT CHARS PER DECISION KIND — wave 46 -> wave 47 (loop-family games excluded on both sides)

| kind | w46 n | w46 mean | w47 n | w47 mean | delta |
|---|---|---|---|---|---|
| ask | 1,365 | 9,800 | 1,245 | 10,412 | **+612 (+6.2%)** |
| priority | 504 | 11,554 | 450 | 14,368 | **+2,814 (+24.4%)** |
| attackers | 106 | 9,227 | 101 | 10,101 | **+874 (+9.5%)** |
| blockers | 30 | 10,153 | 35 | 10,345 | **+192 (+1.9%)** |
| reveal | 15 | 15,820 | 19 | 13,623 | **-2,197 (-13.9%)** |
| bottom | 8 | 2,564 | 6 | 2,215 | **-349 (-13.6%)** |

R7/R6/R14/J's additions outweighed R8's removal on every asked kind except `reveal` and `bottom`.
**Priority took the largest hit (+24.4%) and is 72% of all decisions**; including the loop games
its mean is 21,860 (+89%). **D2, D3, D11 and D13 are the items that pay this back** — the wave-48
docket is, by weight, a decision-value-per-token docket.

---

# LATEGAME SPECIMEN — invariant 0 (for the owner's per-wave prompt review)

**Primary:** `1787786547-ai_baka_deck146-0x55f1115be320-vs-ai_baka_deck123.jsonl` seq **26** —
turn 13 blockers, **74,063 chars, 2 real options, 198 A-rows**, and the `0 of them able to attack
right now` header printed over 197 rows marked `[tapped - attacking]`. It is the corpus's
clearest single artefact of decision-value-per-token failing: ~70 KB spent while the two facts
that decide the window fit in six lines, and it carries **D2** and **D3** in one file.
**Runner-up (natural-sized):** `1787786551-ai_baka_deck130-0x564bb37cbef0-vs-ai_baka_deck152.jsonl`
seq **116** — the small, clean Blastminer attackers window that shows **D14** exactly.
**Extreme, if the pathological case is wanted:**
`1787786558-ai_baka_deck126-0x55fa89ab8040-vs-ai_baka_deck123.jsonl` seq **19** (116,148 bytes,
431 options) or `...deck123-0x55fa8add5b20-vs-deck126.jsonl` seq **1570** (233,662 bytes).
