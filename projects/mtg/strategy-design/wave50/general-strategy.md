# Wave-50 general strategy — RENDER / CORE-PROMPT proposals, layer-routed (R1-R71 revised, R72-R104 new)

Corpus `matchups-20260827-115759` (seventh fair-hand corpus), binary master **67aeffe45** (lanes
V d9144d62c / Z 5b6b44c44 / X c37805358 / Y 7858a403f / W b39b80375; PARSETEST 1930/0). 42 seat
logs, **1,864 records**, **1,780 decisions** (ask 1,214 / priority 376 / attackers 102 / blockers
37 / discard 28 — a NEW kind / reveal 15 / bottom 8), **21/21 games natural — 42/42 seats carry a
`gameend`** (verified on disk by this step: 42 `gameend` records, 3 `fallback` records, all
`stale_echo`; the `seat-125-126.md` header's "deck123-vs-deck162 dead-loop game is excluded" note
describes an older run and is DROPPED for the second wave running — every rate below uses 21/21).
Longest seat 166 records (`deck123-vs-deck125`, 54 turns). Sources: the four seat files, seven
`deckN/general-suggestions.md`, seven `deckN/skill.md`, and the corpus directory for every
reconciliation.

**Routing rule (owner doctrine 2026-08-26).** The core loop — engine mechanics, LLM UX, bugs — is
the purpose. Everything below is a change to what the ENGINE RENDERS or to the CORE PROMPT's own
text. Guide-side answers are named only where a rule must stay correct AFTER the render ships.
Engine BUGS with call sites are in `engine-ledger.md` (the wave-51 docket); this file is the
ranked routing view with the evidence and the wave-51 falsifier for each item.

**Governing constraint on every item.** Legality is enforced structurally; the model's CHOICE is
never constrained. Every proposal ADDS a true token, a true row, a receipt or one re-ask, and
DELETES nothing. Hard caps are rejected. The one proposal this wave that would have REMOVED a
window (deferring Upkeep mana activations to Main 1, deck146 G-1's first form) is adopted only in
its annotation form (R79).

---

## STATUS OF R1-R71, by the emitter's actual string

**R1-R47 (wave-48 items).** Verdicts unchanged from wave 49 except where a wave-50 lane touched
them; the touched ones are re-stated here, the rest CARRY as recorded in `wave49/general-strategy.md`.

| item | wave-50 verdict | evidence |
|---|---|---|
| **R1** loop affordance | **HOLDS; max priority windows/turn 10 (was 16)** | repeat row offered 18, taken 9 (x21, x0, x17, x1, x1, x3, x3, x28, x25), 9 passes; the carry prediction ≤ 10 PASSED (`deck123-vs-deck130` turn-field 16) |
| **R2** A-row collapse | HOLDS | attackers max 25,087; `A1-A21. Human #1-#21 (1/1) ... x21` read correctly at `130 vs123` seq 49 (18 through, 2 life — the best assignment on the screen) |
| **R3 -> R49** target-clause collapse | **SHIPPED (lane X), PASS** | `legal targets right now:` 373 rows, max clause **288** chars, 8 carry an `xN`; max row 1,715; the 28,755-char class is gone |
| **R4** attacking-header scope | PASS | no counter-example in 102 attackers asks |
| **R6 -> R36/R63** back-face marker | **PASS on the wave-49 path (4/4 + 1/1); FAIL on a THIRD path (15)** | all 15 are the `{target text:}` of a Pathway land inside a `legal targets right now:` clause on land-destruction rows (`deck130-vs-deck152` seq 15, 19) — **R80** |
| **R9/R31 -> R62** DRAW FORECAST mirror | **SHIPPED (lane X), PASS 50/50** | `DRAW FORECAST (theirs): their next draw step draws N cards ... = N life to you` on every `DRAW PUNISHERS ... yours` prompt (was 0/63) |
| **R11** no plan where it cannot change | HOLDS | land drops 0 declines at every seat (deck125 72/72, deck152 23/23, deck162 28/28, deck146 15/15, deck123 41/41) |
| **R12** converter LOOP clause | **HOLDS — deck146 finally had windows: ATTACK half 4/4** | `146 vs126` seq 28/34/39/48 `ATTACK: none`; the BLOCK half of the guide's own bullet was the false sentence (skill #146), not the clause |
| **R13/R71** GAME LOG share | **MEASURED: the size driver in every kind** | mean log share ask 54% / priority 58% / attackers 62%; the six largest prompts are 63-83% log (`deck125-vs-deck123` seq 96-98, 20.5-21.0 KB of 25.3 KB) — **R84** |
| **R16/R17/R35** index-past-menu re-ask | **HOLDS on its class (0 re-asks needed); a SIBLING class fell through** | in-range index + off-menu name -> `stale_echo` 3/3 fallbacks — **R74** |
| **R19/R38** what paying taps | **HOLDS; Katilda 0/4; the Hive/Lair rows tagged and mostly obeyed** | `[this land is TAPPED ...]` taken 2/34 at deck152, `[repeat:]` 2/34 (guide-side 152-E); never fired on Battlement — R68 still open, receipts show the Battlement was never in a payment |
| **R20 -> R65** degenerate decode | **SHIPPED (lane Y), UNTESTED** | 0 replies > 15% non-ASCII, 0 `degenerate_decode`, 0 `unparsed_reply` |
| **R21 -> R58** mana-only gate | **SHIPPED (lane Z), PASS 0 (was 6)** | stderr `only mana production and no pending cost; auto-passing` 379; `mana_only_windows_skipped` max 203; 0 windows whose action rows are all `Add ...` |
| **R25/R33 -> R51** edict gain recipient | **SHIPPED (lane X), PASS 0/7** | Devour Flesh `they gain N - the sacrificing player gains, not you` (`123 vs152` seq 6); Tribute `you gain N` unchanged and confirmed by `You gained N life` 8/8 |
| **R25/R33 -> R59** sweeper K | **SHIPPED (lane X), PASS on own turn; the opponent-turn form UNTESTED** | `(N without a restriction against attacking)` / `(all of them carry a restriction against attacking)` 81 renders; `(0 able to attack)` 0 occurrences; `able to attack right now` never arose on a sweeper row |
| **R29/R54** last-clean-line + rejection | **SHIPPED (lane Y), PASS 0/1 on the executed line; the guard's firing is unobservable** | `126 vs125` seq 19's rejected `CHOICE: 4 ... Wait, the list only goes to 3` was not executed but no `rejected_line_skipped` was stamped (also out of range) — **R98** |
| **R30/R61** repeat row count + placeholder | **SHIPPED (lane Z), PASS** | `[you control M creatures right now; ... e.g. "CHOICE: 2 (<action> x<N>)"]` on 18 offers; takes of exactly 50 with no arithmetic 0; every take with N ≥ 2 carried a `Stop at M = ...` line; overshoot max 28 (was 300); but `x0` executed once — **R75** |
| **R32/R64** auto-tap order + receipt | **RECEIPT SHIPPED (lane Z), PASS 396/396 one per payment; ORDER FAIL 69/386** | generic pips paid from a dual while a mono source could pay: `162 vs123` seq 6 `Paid {2} for Howling Mine with Drowned Catacomb #1, Drowned Catacomb #2` (Swamp untapped) — **R73**, IN FLIGHT; receipts MISSING on 3/9 land animations — **R76** |
| **R34/R56** plan-carry expiry on content | **SHIPPED (lane Y), PASS** | "The game is lost" carries 0 (was 5); longest identical run 50 (`deck125-vs-deck123` seq 49-98, a correct guide-sourced plan — by design); `plan_echo_count` max 26; `(your plan's target is not on this window)` rendered 6; one "pass and hope" plan carried 5 (`130 vs126` seq 71) — observation, R104 |
| **R39 -> D15** feeds count | **NOT SHIPPED; fifth corpus, 5/9 breaks** | 0 `{feeds` strings; 3 of the 5 re-scoped correct by the Caress converter (skill #149) — **R97** re-specifies the count |
| **R42 -> D23** 1-based `turn` | **CARRIED, fourth corpus** | every `discard` record: `turn` = log header − 1 (`deck123-vs-deck162` seq 23 `turn: 9` under `=== Turn 10`) — **R81** |
| **R48 -> D1** double-tap | **SHIPPED (lane V), PASS 0/236** | `Mana available` diff 0 at 2x; counter-then-payment 0/26 in stderr; one `- Paid` per cast 396/396; the Sigarda 5-for-4 at `152 vs126` seq 23 is Katilda's own tap, not a double payment |
| **R49 -> D5** | see R3 | |
| **R50 -> D2** stale reveal zone | **SHIPPED (lane W), PASS — the wave-49 loss shape did NOT recur; a DIFFERENT no-sacrifice edict appeared** | `You revealed N cards but none was a legal target` 0/15 reveals; `126 vs146` Tribute at seq 15 sacrificed (skill #132's replay); Soul Shatter `146 vs162` seq 25 resolved with no sacrifice for a different mechanism — **R72**, IN FLIGHT |
| **R51 -> D3** | see R25 | |
| **R52 -> D4** cleanup discard | **SHIPPED (lane W), PASS 26/26** | 28 `discard` records; every `Cleanup discard (hand N, limit 7): you chose ...` preceded by its record; the 2 unnarrated are followed by `gameend`; pilot picks sane 7/7 at deck123; the guide-side ORDER was missing at every seat (skill #136) |
| **R53 -> D6** self-target rows / vanishing rows | **SHIPPED (lane W), PASS 0/124 taken; the all-self line UNTESTED (0 windows); the "vanish" was the designed two-decline cap** | `{this hits YOUR permanent}` 124 rows, 0 taken (was 2) |
| **R55 -> D8** exemplar parse | **SHIPPED (lane Y), PASS 0/507** | 507 exemplar-equal first lines, 0 fallbacks |
| **R57 -> D9** `CHOICE: 0` on a no-pass ask | **SHIPPED (lane Y), UNTESTED** | 0 such replies |
| **R60 -> D14** named card | **SHIPPED (lane X), PASS 0/76** | `[named: Bloodline Keeper]` on 76 prompts; casts of the named card while the Silencer lived 0; narration `Opponent's Silverquill Silencer named X` |
| **R66** owner-side LOOP wording | CARRIED, no failing window | deck126 both-out windows all correct (`vs130` seq 33 win) |
| **R67** second-copy count | **CARRIED, third corpus at deck126 (1 Lantern), and now deck123 (a second Alarm, `123 vs125` seq 156)** | promoted with the Tutor tag — **R82** |
| **R68** Battlement `{paying this taps:}` | CARRIED; receipts show the Battlement never paid for anything (34 mana rows rendered, never in a `- Paid`) | LOW |
| **R69** reading aids | CARRIED | |
| **R70** counter-target size | CARRIED; counter-spreading 0/11 this corpus | close on a second clean corpus |

Of R48-R71: **R48, R49, R50 (its own shape), R51, R52, R53, R55, R58, R60, R61, R62, R64 SHIPPED
and PASSED**; R54, R56 shipped and passed with one observation each; R57, R65 shipped and untested;
R59 shipped and half-tested; R63 passed and re-opened on a third path; **R32's ORDER failed by the
receipt R64 gave it**; R39 carried a fifth corpus; the rest carried.

---

## HIGH

### R72 — Soul Shatter resolves with NO sacrifice when the opponent's highest-MV permanent is not a creature; the script also never takes a planeswalker. (= ledger **D1**; engine seat **D-1 HIGH**, seat-146-152-162 **E-1 HIGH**) — **IN FLIGHT: lane A, `~/Projects/wagicGPT-w51-a`**
`1787849903-ai_baka_deck146-0x5568f6f12330-vs-ai_baka_deck162.jsonl` seq **25** (turn 12, 7 life):
`Cast Soul Shatter {2}{b}` with their line `Master of the Feast {1}{b}{b} (5/5) [flying]; Ob
Nixilis, the Hate-Twisted {3}{b}{b} [planeswalker] [counters: 1x loyalty]` -> seq 26 events
`- Paid {2}{b} for Soul Shatter with Shineshadow Snarl #2, Swamp #1, Swamp #2 / - You cast Soul
Shatter / - Your Soul Shatter resolved and went to your graveyard` and nothing else (verified on
disk by this step); the 162 seat's seq 16 agrees; stderr `game-146v162-1787849900.stderr` line
1363 `Resolving Action on stack: Soul Shatter` is followed by the next window with no
`doReactTo` / `targeting with` / `StackAbility` (the working trace at `game-146v152-...` has all
three). Two contrast cases sacrificed (`146 vs152` seq 25 Intrepid Adversary, `146 vs123` seq 28
Bloodline Keeper) — neither board had a planeswalker. **Mechanism, reconciled by this step
against source (`src/WParsedInt.cpp` 386-438):** the primitive (`borderline.txt:106093`)
filters `creature[manacost=convertedcost:highest:*:myBattlefield]`; `convertedcost:highest:*`
builds a target chooser over `*|myBattlefield` and takes the highest MV over EVERY permanent on
that battlefield — Ob Nixilis MV 5 outranks Master MV 3, no creature has MV 5, the filter
matches nothing, nothing is sacrificed. Seat-146's hypothesis is confirmed; the engine seat's
alternative (lane W's `ownChooser()` gate) is not needed to explain it. Second defect on the same
card: Oracle (and the primitive's own `text=`) says "a creature OR PLANESWALKER with the highest
mana value among creatures and planeswalkers" — the script never takes a walker. Fix is
CARD-SCRIPT first (owner rule: express the Oracle faithfully in script — a `creature,planeswalker`
class with the ranking restricted to `creature,planeswalker:myBattlefield`), engine only if the
filter grammar cannot express it; a suite pin for both boards (walker-highest -> walker
sacrificed; creature-highest -> creature sacrificed). The guide teaches the card fact meanwhile
(skill #145). Cost this corpus: one game (the only punisher was the out under a 4-card FORECAST).
**Wave-51 prediction.** Edict/sacrifice takes whose next events carry no victim line: **0/N**
(was 1/13); a planeswalker sacrificed when it is their highest-MV creature-or-walker: N/N;
the pin passes.

### R73 — generic pips are paid from dual lands while a mono source sits untapped: 69/386 receipts. (= ledger **D2**; engine seat **D-2 HIGH by count**, seat-146-152-162 **E-3**, deck152 G-1) — **IN FLIGHT: lane B, `~/Projects/wagicGPT-w51-b`**
Lane Z's receipt made lane T's ordering adjudicable and it failed: `deck162-vs-deck123` seq 5
sources `Drowned Catacomb #1 {u} or {b}; Drowned Catacomb #2 {u} or {b}; Swamp {b}` -> seq 6
`- Paid {2} for Howling Mine with Drowned Catacomb #1, Drowned Catacomb #2` (Swamp untapped;
verified on disk), seq 8 the same with two Swamps untapped; `deck125-vs-deck152` seq 34->35
`Paid {w} for Path to Exile with Seachrome Coast` with two Plains untapped; `deck152-vs-deck146`
seq 6/9/16 `Paid {1}{w} ... with Deserted Beach #1, Deserted Beach #2` with a Plains untapped.
Engine seat's method: 78/386 receipts used more multi-colour sources than the exact minimum, 69
of them on costs with generic pips; by deck 125 22, 152 17, 146 14, 123 12, 162 10, 126 3.
Seat-146's land-only heuristic gives 33/146 on its seats — consistent. No colour strand was
observed this corpus (the idle basic was always the colour the next spell needed), so the cost is
latent — but it strands colours in exactly the response windows counters live in, and deck125
(the counterspell deck) is the worst offender. Ask: pay generic from mono sources first, coloured
pips from the scarcest colour; the receipt is the test.
**Wave-51 prediction.** Receipts using more multi-colour sources than the minimum over the
untapped set: **0/N** (was 78/386; 69/386 on generic pips).

### R74 — an in-range index with an off-menu name is classified `stale_echo` and falls to Baka with NO re-ask; all three of the corpus's fallbacks, and Baka cast the wrong card in two. (= ledger **D3**; seat-125-126 **HIGH #1**, deck126 G1, engine seat **D-4**; skill #151)
`1787849909-ai_baka_deck126-0x5601b9f034e0-vs-ai_baka_deck125.jsonl` seq **14**: menu `1. Cast
Overgrown Battlement ... 2. Cast nothing right now`; reply `CHOICE: 1 (Cast Sanguine Bond)` (Bond
in hand, `{B} 1` on the mana line); record `choice: -1, fallback: stale_echo`; next events
`- Paid {1}{g} for Overgrown Battlement` (Baka cast the wall). The prior reply was `Play Savannah`
— nothing was echoed; the label is wrong. Same at `...deck126-0x55fc821cb030-vs-...deck162.jsonl`
seq 16 (Lantern cast by Baka) and `...deck126-0x56221e2c00d0-vs-...deck130.jsonl` seq 13.
`parseChoice` sets `staleEcho` via `echoNoMatch && choice > 0 && echoStaleForIndex(choice)`
(`AIPlayerGPT.cpp` ~13565) and returns −1. Lane S's contract covered "off-menu name replacements
executed 0" (held) and "index PAST the menu -> `named_row_reask`" (held, 1/1 in wave 49); an
in-range index with an off-menu name is the gap between them. Ask: route it to the named-row
`[RE-ASK]`, and print the reason the render already has: `Sanguine Bond is on your hand line but
has no "Cast" row this window: its {3}{b}{b} needs two {B} sources and your mana line counts {B}
1. Answer with a numbered row.` The model's answer was a perception error one line corrects;
Baka's pick is not what the model would have chosen from the real menu.
**Wave-51 prediction.** Baka-executed `stale_echo` on a reply whose index is in range: **0/N**
(was 3/3); every off-menu name -> one `[RE-ASK]`, ≥ 50% recovered.

---

## MEDIUM

### R75 — a named repeat count of 0 is executed once, and a CHOICE count that contradicts the PLAN line's `this window: pass` executes the count. (= ledger **D4**; seat-123-130 **H1** + **M1**, engine seat **D-3**, deck123 G2; skill #133, #134)
`1787849886-ai_baka_deck123-0x55de8e828b00-vs-ai_baka_deck162.jsonl` seq **35**: `CHOICE: 2 (Create
human with Thraben Doomsayer x0) / PLAN: Stop at M = 25. M is 26 now. Pass.` -> seq 36 `You used:
Create human with Thraben Doomsayer - ran 1 time (you named 0)`, `parse_note:
repeat_count_under_two`. Three more windows where the CHOICE named x<N> and the PLAN beneath said
pass: `123 vs130` seq 31 (x1), seq 46 (x3, "stop reached"), `123 vs126` seq 32 (x25 under "This
window: pass" — 25 tokens past a stop of 30). Ask: (a) a count of 0 on the repeat row is a
decline — execute the pass row (or one `[RE-ASK]` naming it), never floor to 1; the `x1` branch is
correct as is. (b) Proposal, not a defect: when a repeat-row reply's PLAN line contains `this
window: pass` / `stop reached` and the CHOICE line names N > 0, one `[RE-ASK]` quoting both lines
(`plan_choice_conflict`). Neither constrains a choice; both add a re-ask on a self-contradiction.
**Wave-51 prediction.** `ran 1 time (you named 0)`: **0/N** (was 1); CHOICE-vs-PLAN
conflicts executed without a re-ask: 0/N (was 4).

### R76 — the `- Paid` receipt is missing on 3 of 9 mana-cost land animations. (= ledger **D5**; seat-146-152-162 **E-2**, deck152 G-4)
`1787849885-ai_baka_deck146-0x5620ff65a3b0-vs-ai_baka_deck126.jsonl` seq 32 and 36 events: `- You
used: becomes beholder with Hive of the Eye Tyrant` with no `- Paid {3}{b} ...` line (verified on
disk); `1787849889-ai_baka_deck152-0x55988eb1a020-vs-ai_baka_deck126.jsonl` seq 25 `becomes a 1/1
hydra with Lair of the Hydra` with no receipt; whereas `152 vs125` seq 17/45/59/60/73/74 each
print `- You used: becomes ... / - Paid {1}{g} for Lair of the Hydra with ...` (verified). Both
Hive misses are Upkeep windows; the Lair miss is on a `[this land is TAPPED]` row — the seam is
plausibly the `becomes` ability of a land whose own mana joins the payment. Cost: the receipt is
now the instrument for R73 and R48; a silent activation is a hole in the measurement (silent-
instrument rule).
**Wave-51 prediction.** `- Paid` on every activation with a mana cost: N/N (was 6/9 on land
animations, 144/144 on casts).

### R77 — activated-ability damage rows print no `{right now: takes N damage - DIES / SURVIVES}`; the spell target menus do. (= ledger **D6**; seat-123-130 **H2 HIGH**, deck130 G1; skill #139)
0/49 rows of the form `Deal 2 damage with <Siege-Gang Commander | Pyrite Spellbomb> targeting
<creature> [opponent's battlefield]` carry the outcome, while the spell menus print `Goblin (1/1)
... {right now: takes 3 damage - DIES}` (`130 vs146` seq 17) and `Rorix Bladewing (6/5) ...
SURVIVES (toughness 5)}` (`130 vs123` seq 38). Cost: `130 vs162` seq 23 (4 life, two punishers) a
Goblin sacrificed for 2 at Fate Unraveler (3/4); seq 17 a Spellbomb's 2 at Ob Nixilis at
`[counters: 3x loyalty]` — both lived; the seat died on the draw step. Ask: the same clause (and
`- loyalty L` for planeswalkers) on ability target rows.
**Wave-51 prediction.** Ability-damage rows taken at a target whose toughness/loyalty exceeds the
damage: 0 (was 2); the clause on 100% of ability damage rows.

### R78 — mana-cost land animations are offered in UPKEEP; the Hive is offered ONLY there (11/11), and two Upkeep takes burned 8 mana before the draw. (= ledger **D7**; seat-146-152-162 **E-4**, deck146 G-1, deck152 G-2)
deck146 `becomes beholder`: 11 offers, 11 Upkeep, 0 main phase; deck152's Lair: 19 Upkeep / 13
Main 1 / 1 Main 2 / 1 Blockers; Katilda's pump 3 Upkeep / 1 Main 1. `146 vs126` seq 31 and 35
(17/22 and 16/22): animated in Upkeep, `ATTACK: none` after (seq 34, 39). The guide's "Upkeep ->
PASS" rule is doing the engine's job and failed 2/11. **Adopted in the ANNOTATION form only:**
the Upkeep row carries `{before your draw step - the same row is offered again in Main 1}` when
the ability's permanent will be offered in Main 1. The DEFER form (suppress the Upkeep ask when
the only non-toggle rows are own-permanent mana activations) removes a legal window and is
rejected under the governing constraint — an Upkeep animation is occasionally the right play
(a response window the model may want before the draw), and the row is the model's to decline.
**Wave-51 prediction.** Hive/Lair Upkeep takes with the annotation rendered and the Main-1 offer
following: 0/N (was 2/11 Hive, 19-offer Lair); the annotation on 100% of such Upkeep rows.

### R79 — a "choose a name" / already-owned fact on the cast row: the Tutor and the second copy. (= ledger **D8**; seat-123-130 **M2**, deck123 G3; R67 promoted; skill #138)
`123 vs125` seq 118 and 139: `Cast Idyllic Tutor` with Intruder Alarm on the battlefield line,
plan "already on battlefield, but must cast it to dig for Bloodline Keeper" (a Tutor finds only
an enchantment); seq 156 a second Intruder Alarm cast with one out; deck126 `vs146` seq 37 a
second Lantern with `Chromatic Lantern` printed on its own line (the reply argued "I do not have
a CL"). Third corpus of dead Tutors (wave 49: 3, wave 50: 2); the guide rule has held 0 times
where it mattered and is left at one line. Ask: `{Intruder Alarm is already on your battlefield -
this finds only an enchantment}` on the Tutor row when an Alarm is owned; `{a copy is already on
your battlefield}` on any non-stacking permanent's cast row.
**Wave-51 prediction.** Tutor casts with the tag rendered: 0 (was 2); second-copy casts with the
tag rendered: 0 (was 2 across two decks).

### R80 — the Hammer of Bogardan upkeep return row does not price itself against the hand. (= ledger **D9**; seat-123-130 **M3**, deck130 G2; skill #138)
`130 vs123` seq 18: `Put a card into hand with Hammer of Bogardan [cost: {2}{r}{r}{r}]` at five
sources with Siege-Gang Commander {3}{r}{r} in hand; taken under a plan reading "I have 5 mana,
Siege-Gang is {3}{R}{R}, so I can cast it"; no Commander that turn; the Doomsayer made 19 Humans
the next. Third corpus (wave 49: 2). Ask: `{spends 5 of your 5 untapped sources this turn;
Siege-Gang Commander {3}{r}{r} in your hand needs 5}` — the cast rows' `{leaves N of your M ...}`
clause applied to an activated ability whose cost is most of the turn.
**Wave-51 prediction.** Hammer returns with a castable body in hand and the clause rendered: 0
(was 1).

### R81 — the Soul Shatter row prints only its card text; edict rows print `{right now: ...}`. (= ledger **D10**; deck146 G-2)
`146 vs162` seq 25 and `146 vs123` seq 20 (cast into `0 creatures`): no `{right now: they
sacrifice <name>}` / `{right now: they control 0 creatures - this does nothing}`. Lane X's edict
clause is keyed to the `sacrifice!$ targetedplayer` shape; Soul Shatter's `!$ opponent` shape
missed it. Would have exposed both the empty-board cast and R72 at the row. Ships with lane A.
**Wave-51 prediction.** Soul Shatter casts at `0 creatures` with the clause rendered: 0 (was 1);
the clause's named victim equals the narrated sacrifice N/N.

### R82 — the draw-engine cast row's feeds count, now with the discard converter. (= ledger **D11** = wave-49 D15 carried; deck162 G-1; skill #149)
Fifth corpus, 5/9 breaks (`162 vs123` seq 7/10/13/14, `162 vs146` seq 8); 3 of the 5 were
correct plays because Liliana's Caress converted the discards (14 damage before any punisher, the
win's mechanism). Re-specified: `{feeds: the opponent draws N extra per turn; converters on your
battlefield: <punishers> + Caress}`.
**Wave-51 prediction.** Engine casts at converters 0 with opp `0 creatures`: 0 (was 2 after
re-scoping).

### R83 — a creature cast row carries a converter tag when the opponent controls Exquisite Blood / Sanguine Bond or has resolved an edict this game. (deck125 G1; skill #135)
`deck125 vs126` seq 42: `Cast Emrakul, the Aeons Torn {15} (15/15)` beside an opponent line
printing `Exquisite Blood ... {effect: "Whenever an opponent loses life, you gain that much
life."}`; the guide's rule was in the reply's plan and still lost to the row; the seat then
Verdicted its own Emrakul (`1 of yours`) and lost 33 -> 0 to the loop. Lane U's LOOP clause fires
only with BOTH halves out. Ask: `{their Exquisite Blood turns this creature's death into life for
them}` on creature cast rows — the `{right now:}` grammar on the other side of the table.
**Wave-51 prediction.** Creature casts at deck125 with the tag rendered and a converter out: 0
(was 1).

### R84 — the edict row's victim carries its tags. (= ledger **D12**; deck123 G4 + deck126 G2 — convergence; skill #135)
`{right now: they control 1 creature - Shield Sphere is sacrificed, you gain 6}` (`126 vs162` seq
6) and `... Katilda, Dawnhart Prime is sacrificed, they gain 1 ...` (`123 vs152` seq 6): the
decision that remained was "is this worth a card", keyed by both guides to `[defender]` / power
0 / a lord's first text clause — facts the row already knows. Ask: `Shield Sphere [defender] is
sacrificed`, `Katilda, Dawnhart Prime (1/1) [lord: ...]` on the row.
**Wave-51 prediction.** The tag on 100% of named-victim edict rows; Tribute at a `[defender]`
victim with an entry 1-6 row beside it: 0 (was 1).

### R85 — the opponent-draws abilities are priced the way DRAW FORECAST prices the draw step. (= ledger **D14**; deck162 G-2; skill #150)
Ob Nixilis's -2 row reads `destroy target creature and draw two cards` with nothing about the
2 x P the draws deal under the seat's punishers; passed at their 3 life with P = 4 (`162 vs126`
seq 23/24). Ask: `{their draw: 2 cards = 2 x P = <N> life}` on the -2 rows and `= half their
library x P` on Peer into the Abyss.
**Wave-51 prediction.** -2 passed with 2 x P ≥ their printed life and the clause rendered: 0/N
(was 2/2).

### R86 — the `discard` list prints bare names; the bottoming ask prints cost and type. (= ledger **D15**; deck125 G3 + deck126 G3 — convergence)
`deck125 vs123` seq 52: nine bare names (`1. Tribute to Hunger`, ...); `deck126 vs123` seq 7 the
same. The order rules (skill #136) key on cost and type; put the hand line's grammar on the list.
**Wave-51 prediction.** `discard` rows with cost/type: 100%.

---

## LOW

### R87 — bare back faces on the `{target text:}` of MDFC lands inside target clauses: 15, a third emitter path. (= ledger **D16**; engine seat **D-6**)
`Hengegate Pathway (land) {target text: "{T}: Add {W}. // Mistgate Pathway"}` on Molten Rain /
Stone Rain / Lay Waste rows (`deck130-vs-deck152` seq 15, 19; Mistgate 11, Grimclimb 4).
`markBareBackFaces` reaches the hand row and the option-menu row, not the target clause.

### R88 — the receipt's source separator collides with card names. (= ledger **D17**; engine seat **D-5**)
`- Paid {1}{w} for Intrepid Adversary with Katilda, Dawnhart Prime, Elite Spellbinder` reads as
three sources for two pips (10 receipts). Use `; ` — the `Those sources` line already does.

### R89 — the JSONL `turn` field is the narration's turn minus one, fourth corpus. (= ledger **D18**)
Every `discard` record shows it (28/28).

### R90 — `rejected_line_skipped` is not stamped when the rejected line is also out of range. (= ledger **D19**; engine seat **D-8**)
`126 vs125` seq 19: the right thing executed; the D7 guard's first live firing is invisible.

### R91 — a `decision_reversed_in_prose` counter. (= ledger **D20**; seat-146-152-162 **E-5**, deck146 G-3; skill #133)
`146 vs123` seq 20: coded `CHOICE: 3 (Cast Soul Shatter)` then 2,700 chars ending "I will cast
Pelakka Predation", no second coded line. Metric first; if non-trivial corpus-wide, one core-
prompt sentence ("if you change your answer, write the new CHOICE line last").

### R92 — the repeat row carries the opponent's life and creature count beside M. (= ledger **D21**; deck123 G1)
`123 vs162` seq 34 read C as 1 with the line saying 2; `123 vs130` seq 26 wrote the stop as 24
with C 4 on the screen. The three inputs of the subtraction on one line.

### R93 — enters-tapped on the `Play` rows. (= ledger **D22**; deck123 G5)
`123 vs162` seq 6: `Play Arcane Sanctum` over `Play Underground Sea` with a three-mana spell
castable off the untapped one.

### R94 — Lightmine Field priced on the attackers header. (= ledger **D23**; deck123 G6)
`123 vs125` seq 39, 63: two 2/2s sent twice, both died each time; the tag priced blockers, not
the Field.

### R95 — the GANG BLOCK clause names the punisher. (= ledger **D24**; deck130 G3; skill #141)
`130 vs162` seq 22: the attacker was on the `DRAW PUNISHERS` line two lines up.

### R96 — an exit on the X menu, or the (a)/(b) verdict on the cast row. (= ledger **D25**; deck130 G4; skill #140)
`130 vs126` seq 60: every killing X read `YOURS: Siege-Gang Commander`; no "cast nothing" row.
Cast-row clause form (`{every affordable X that kills something of theirs also kills Siege-Gang
Commander}`) adds a token and is safe; the `0. Do not announce` row needs the engine seat's
ruling on cancel semantics after `Cast Card Normally`.

### R97 — the pain-source payment at low life on the cast row. (= ledger **D26**; deck130 G6)
`130 vs123` seq 56: `- Paid {1}{r}{r} for Molten Rain with Mountain #1, Talisman of Impulse #1,
Talisman of Impulse #2` at 2 life -> 1.

### R98 — the state block is stale across a sequential sacrifice ask. (= ledger **D27**; seat-146-152-162 **E-7**, deck146 G-4)
`146 vs125` seq 60-65 (annihilator 6): the option list shrinks, the battlefield line does not.

### R99 — `[damage ORDER, not a block]` on the combat-damage-order ask. (= ledger **D28**; E-8, deck152 G-3)
Answered as a block twice (`152 vs146` seq 13; `146 vs152` seq 42); numbers valid both times.

### R100 — `destroys 1 of their creature` (singular). (= ledger **D29**; deck125 G2)

### R101 — the cycling row's `{if you pass here, this option is not offered again this turn}` reads as urgency. (deck130 G5, L1; one window, inference)
`130 vs152` seq 16 cycled Lay Waste at four lands. If the tag exists for the two-decline cap, a
neutral wording (`{offered twice per turn at most}`) is a free change; carried LOW.

### R102 — the "pass and hope" plan carried 5 windows past the content test. (engine seat observation)
`130 vs126` seq 71: PLAN "The opponent controls Sanguine Bond and Exquisite Blood, creating a
lethal loop ..." named cards, so the names-no-action test passed. Observation only; no change.

### R103 — long-deliberation latency. (E-6, seat-123-130 L2, seat-125-126 LOW #5)
Max 587 s (`126 vs125` seq 18, a 10,054-char reply on an 8.7 KB prompt); 16 replies > 120 s on
18 logs; blockers p90 76 s with 37 asks. Reasoning, not prompt size; reported for the efficiency
line.

### R104 — measurement: the game-log window is 54-62% of the mean prompt and 63-83% of the six largest. (= ledger **D30**; R71 continued)
Ask mean fell 13% and max 27% once the 34 KB row went; the residual is log. No prompt > 26 KB.
Decision for the owner's per-wave prompt review: older-turn compaction is the next 10 KB if it
moves latency; report per-kind log share again next wave, no change asked.

---

## SHIPPED SINCE THE CORPUS — recorded, NOT docketed (owner, Vita, 2026-08-27)
- Ability borders priced by strict potential + planner (**ca8a71e14**) — resolves the Ironroot
  Warlord dual double-count. Human-seat; not LLM-visible; no window in this corpus.
- Morph "Face Up" ability outliving the flip — AAMorph liveness on the `->next` chain + oracle
  skip of superseded instances; **commit pending gate**. Verify by string next corpus if any morph
  is in the pool (none is).
- Lane A (R72) and lane B (R73) are IN FLIGHT and docketed as such, not as shipped.

## GUIDE-ONLY — stays in guides after the render ships
- deck123: A50-1 (the subtraction before line one; #133), A50-3 (no Doomsayer tap with the Alarm
  in hand), A50-4 (K-0 cross-reference to CHECK 1; #137), A50-5 (Tutor at one line; #138), A50-7
  (Lightmine attackers), A50-8 (enters-tapped land choice), A50-9 (cleanup order; #136), A50-10
  (Katilda; #135), A50-12/13 (mulligan/bottoming).
- deck125: W20 (Emrakul keyed to `{effect:}`), W21 (`1 of yours`; #135), W22 (the gate re-derived;
  #142), W23-W25 (sweeper-at-1 contradiction, Lightmine `[tapped]` read, cleanup order).
- deck126: F16 (row-keyed Tribute), F17 (`[defender]` carve-out; #135), F18 (discard order; #136),
  F19 (the menu is the list; #151), F20 (Lantern counts).
- deck130: D50-1 (Hammer, one line; #138), D50-2/3 (maker-first, Commander block), D50-4/8 (the X
  menu exit; #140), D50-5 (ability rows print no result; #139), D50-6 (GANG kill case; #141),
  D50-7 (the walls check ported; #137), D50-12 (pain sources).
- deck146: 146-G/H (coverage key, PUT content; #144, #143), 146-K/L (the pair; #146), 146-C
  (Hive Upkeep, one line).
- deck152: 152-E (Lair section; #147), 152-F (Katilda B-row; #148).
- deck162: 162-D (the -2 burst; #150), 162-E (Caress converter; #149), 162-F (all-lands
  sentence).

## MEASUREMENT items for the wave-51 reviewer briefs
1. Edict/sacrifice takes and their victim line, per seat (R72), and Soul Shatter's `{right now:}`
   once R81 ships.
2. `- Paid` receipts: one per payment (R48 regression detector), missing on activations (R76), and
   the exact-minimum multi-colour check (R73) — report N/N per seat.
3. Fallback classes with the reply's index range and name (R74); `[RE-ASK]` count and recovery.
4. Repeat row: CHOICE-vs-PLAN disagreements, `x0`, takes at M ≥ stop (R75; skill #133).
5. Cleanup `discard`: sends against each guide's order (skill #136), per seat.
6. Offers by phase for every Upkeep sink (skill #147), with the annotation count once R78 ships.
7. `decision_reversed_in_prose` (R91) if the counter ships; otherwise the hand count.
8. Per-kind prompt chars and LOG SHARE (R104); no exclusions (21/21).
9. Quoted-literal grep per ROW FAMILY (skill #139) and the cross-guide KEY diff (skill #144).

## PASS / KEEP — measured working, do not re-open
Lane V's double-tap fix (0/236, 0/26 counter-then-pay) · lane Z's receipt (396/396 one per
cast), repeat-row count + placeholder (overshoot 300 -> 28, 0 exact-50 takes), mana-only gate (0
leaks, was 6) · lane X's gain recipient (0/7 wrong), target-clause collapse (max clause 288, was
28,755), sweeper K wording (`(0 able to attack)` 0), `[named:]` (0/76), FORECAST mirror (50/50),
MDFC menu marker (5/5 on the wave-49 path) · lane Y's exemplar parse (0/507), content-keyed
expiry ("lost" 0, the 50-window correct plan carried), target-absent line (6), rejection guard
(0/1 executed) · lane W's own-chooser reveal (0/15), cleanup ask (26/26 narrated, 28 records),
self-target tag (0/124 taken) · lane U's LOOP clause (deck146 ATTACK 4/4 on its first windows)
· land drops 0 declines pool-wide · **no render falsehood this corpus** (wave 49 had one) —
everything open is an omission, an ordering, a counter, or the one engine defect (R72).
