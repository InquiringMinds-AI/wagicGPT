# Wave-51 general strategy — RENDER / CORE-PROMPT proposals, layer-routed (R1-R104 revised, R105-R135 new)

Corpus `matchups-20260827-155545` (eighth fair-hand corpus), binary master **c010ddb47** (lanes A
42f2eff2b / B df8ff7641 / C 4cf3745a7 / D ac7c5f5be / E 9c772cd3d / F 7c0a04d1d + Vita fixes
ca8a71e14 / dd8663146; PARSETEST 2022/0). 42 seat logs, **2,213 records**, **2,129 decisions**
(ask 1,493 / priority 445 / attackers 110 / blockers 31 / discard 31 / reveal 13 / bottom 6),
**21/21 games natural — 42/42 seats carry a `gameend`** (verified on disk by this step: 42
`gamestart`, 42 `gameend`, 5 `fallback` fields = `named_row_reask` 3 / `unparsed_reply` 1 /
`stale_livelock` 1; the `seat-125-126.md` header's "deck123-vs-deck162 dead-loop game is excluded"
note describes an older run and is DROPPED for the third wave running — every rate below uses
21/21). Longest seat **301** records (`deck146-vs-deck125`, an **80-turn** creatureless stalemate);
max prompt **33,536** (that game, seq 279, 71% log) — 3 prompts over the 30,000 bar, all in that
game. Sources: the four seat files, seven `deckN/general-suggestions.md`, seven `deckN/skill.md`,
and the corpus directory for every reconciliation.

**Routing rule (owner doctrine 2026-08-26).** The core loop — engine mechanics, LLM UX, bugs — is
the purpose. Everything below is a change to what the ENGINE RENDERS, what the PARSER executes, or
the CORE PROMPT's own text. Guide-side answers are named only where a rule must stay correct AFTER
the render ships. Engine BUGS with call sites are in `engine-ledger.md` (the wave-52 docket); this
file is the ranked routing view with the evidence and the wave-52 falsifier for each item.

**Governing constraint on every item.** Legality is enforced structurally; the model's CHOICE is
never constrained. Every proposal ADDS a true token, a true row, a receipt or one re-ask, and
DELETES nothing. Hard caps are rejected. Two proposals this wave would REMOVE a window and are
rejected in that form: the Upkeep-animation DEFER, re-filed with a lethal-check condition
(deck152 G-1) — still a removed window, still rejected, the planner half adopted instead (R117);
and "auto-pass other asks during a repeat loop" (seat-123 H4's first half) — adopted only as
"do not COUNT their drops toward the livelock breaker / do not launch a model request the loop
will invalidate" (R105), which removes no window the model would have answered.

---

## STATUS OF R1-R104, by the emitter's actual string

**R1-R47 (wave-48 items).** Verdicts unchanged from wave 50 except where a wave-51 lane touched
them; the touched ones are re-stated here, the rest CARRY as recorded in `wave50/general-strategy.md`.

| item | wave-51 verdict | evidence |
|---|---|---|
| **R1** loop affordance | **HOLDS; max priority windows/turn 10 (= wave 50)** | repeat row offered 6, taken 3 (x17, x23, x17); `deck123-vs-deck126` turn 14 = 10 windows; but one take was INTERRUPTED by the engine — **R105** |
| **R2** A-row collapse | HOLDS on attackers; **FAILS on ability target menus** | `A1-A24` read correctly (`123 vs126` seq 52, -30); `Equip with Lightning Greaves targeting Vampire #N` 26-34 rows per window — **R112** |
| **R13/R71 -> R104** GAME LOG share | **MEASURED AGAIN: now the whole overrun** | mean log share ask 57% / priority 61% / attackers 61% / discard 61%; 102 prompts exceed wave 50's max, 96 of them in one 80-turn game; 3 > 30,000 — **R132** |
| **R16/R17/R35 -> R74** index re-ask | **HOLDS on its class; a THIRD sibling class fell through** | in-range index + name ON the menu at another index -> index executed (`123 vs125` seq 48) — **R110** |
| **R19/R38** what paying taps | **`{paying this taps:}` rendered; the FORECAST DISAGREES WITH THE RECEIPT on variable sources** | `126 vs152` seq 18 names two Battlements, seq 19 receipt taps three + floats 13 {g} — **R108** (lane H) |
| **R32/R64 -> R73** auto-tap order | **SHIPPED (lane B), PASS 1/421 plannable** | see R73 |
| **R39 -> R82** feeds count | **SHIPPED (lane F), PASS 78/78** | see R82 |
| **R42 -> R89** 1-based `turn` | **SHIPPED (lane D), PASS 2,070/2,070** | `discard` records read `turn: 10` under `=== Turn 10` |
| **R66** owner-side LOOP wording | CARRIED, no failing window | deck126 both-out 3/3 |
| **R67 -> R79** second copy | **SHIPPED (lane E) — the tag CONFLATES stackable and dead** | **R114** |
| **R68** Battlement `{paying this taps:}` | **REACHED at last — and it is WRONG on the count** | **R108** |
| **R70** counter-target size | CARRIED; 0 spreading, second clean corpus -> **CLOSE** | |

**R72-R104 (wave-50 items), by string:**

| item | wave-51 verdict | evidence |
|---|---|---|
| **R72 -> D1** Soul Shatter | **SHIPPED (lane A, card script), PASS 0/15 no-sacrifice; walkers 2/2** | `146 vs162` seq 40 `{right now: they sacrifice Ob Nixilis, the Hate-Twisted (MV 5, their highest)}` -> seq 41 victim; `146 vs126` seq 43 Sorin; `146 vs125` seq 170 Emrakul (MV 15, protection irrelevant — does not target, correct CR) |
| **R73 -> D2** generic from duals | **SHIPPED (lane B), PASS 1/421 plannable (was 69/386)** | engine seat's exhaustive subset check over 443 receipts; residuals as predicted: 14 `{X}`, 8 creature/foreach sources, **0 over-taps**; the one land case `152 vs125` seq 39 `Paid {g}{w} for Katilda ... with Overgrown Farmland #1; Plains #2` with `Lair of the Hydra {g}; Hengegate Pathway {w}` untapped (verified on disk); 4 deck152 "suboptimal" are the planner correctly preferring lands over Katilda-granted creature mana |
| **R74 -> D3** off-menu name re-ask | **SHIPPED (lane C), PASS 2/2 re-asked, 2/2 recovered; `stale_echo` to Baka 0** | `123 vs130` seq 25 -> 26 `[RE-ASK] "Cast Thraben Doomsayer" is not on this list. Answer with a number from 1 to 4.`; `126 vs146` seq 22 -> 23; third re-ask (index past menu) also recovered |
| **R75 -> D4** `x0` + conflict | **SHIPPED (lane C), UNTESTED both halves; the SCOPE is short** | 0 `x0` written; 0 `plan_choice_conflict` renders — the one over-take (`123 vs126` seq 47, x17 at M 27 vs stop 24) carried NO PLAN line; four single-row `[cost: Tap]` takes under "this window: pass"; one "we must pass" in prose executed Lolth 0 — **R118** |
| **R76 -> D5** `- Paid` on activations | **SHIPPED (lane D), PASS 53/53 observable** | Hive `- Paid {3}{b} for Hive of the Eye Tyrant with ...` 4 sources; `with no source tapped` 0 corpus-wide |
| **R77 -> D6** ability DIES/SURVIVES | **SHIPPED (lane E), rendered 428/428; takes at SURVIVES 4/12 (2 deliberate)** | `130 vs126` seq 37 two-step (second killed), seq 49 wasted at 3 life; render correct -> deck130 D51-2 |
| **R78 -> D7** Upkeep annotation | **SHIPPED (lane E), PASS 0/35 Upkeep Hive takes; the Lair took it 1/35 and was then TAPPED FOR MANA** | `152 vs125` seq 36 `[Upkeep offer: ...]` taken -> seq 39 `{paying this taps: Lair of the Hydra}` — **R117**; Hive main-phase re-offer taken 3/9 after a same-window animation — **R116** |
| **R79 -> D8** already-owned tag | **SHIPPED (lane E), rendered 139; 21 takes — 15 STACKABLE (Mines, Talismans, Dreams), 2 legendary binned, 4 Tutors** | `152 vs146` seq 41 Teferi #2 -> legend rule bins one; `126 vs125` seq 21 the tag warned against the CORRECT Tutor — **R114** |
| **R80 -> D9** Hammer priced | **SHIPPED (lane E), PASS 0/141 by class; 0/2 Hammer returns over a body** | `{spends 5 of your 5 ...; Siege-Gang Commander needs 5}` `130 vs152` seq 25/30 declined |
| **R81 -> D10** Soul Shatter `{right now:}` | **SHIPPED (lane D), PASS 6/6 victim == narration** | tie / 0-board forms UNTESTED (0 renders) |
| **R82 -> D11** feeds count | **SHIPPED (lane F), PASS 78/78 rows; deck162 casts at K 0: 3 (2 first-Mine exemptions, 1 break)** | 162-G re-keys the guide; the Caress-only K lacks its hand-size qualifier — **R131** |
| **R83** creature converter tag | **NOT SHIPPED; 0 windows with a converter printed at deck125** | W52: the converter half PASS 0; carried |
| **R84 -> D12** edict victim tags | **NOT SHIPPED; a failing window arrived** | `126 vs162` seq 17 Tribute on Fog Bank (a `[defender]`) at 13 life over a printed Bond row — the tag would have been on the row; **promoted MED, carried D24** |
| **R85 -> D14** opponent-draws pricing | NOT SHIPPED; 0 windows (P6 UNTESTED) | carried |
| **R86 -> D15** discard list grammar | NOT SHIPPED; 31 `discard` asks, bare names; the PUT mis-execution rode on this list — **R107** | carried |
| **R87 -> D16** bare back faces | **SHIPPED (lane F), PASS 0** | `(text omitted)` 278 |
| **R88 -> D17** receipt separator | **SHIPPED (lane D), PASS** | `; ` throughout |
| **R89 -> D18** `turn` | **SHIPPED (lane D), PASS** | |
| **R90 -> D19** `rejected_line_skipped` | NOT SHIPPED; no window | carried |
| **R91 -> D20** reversed-in-prose counter | NOT SHIPPED; hand count 2 (`146 vs125` seq 282 "we must pass" -> Lolth 0; `130 vs125` seq 25 answer_replaced 652 s) | **R118** absorbs the natural-language pattern |
| **R92 -> D21** repeat row inputs | NOT SHIPPED; `123 vs162` seq 15 counted Shield Sphere as 0 (row says 1) | carried |
| **R93 -> D22** enters-tapped | NOT SHIPPED; 0 breaks | carried LOW |
| **R94 -> D23** Lightmine header | NOT SHIPPED; 0 windows | carried LOW |
| **R95 -> D24** GANG BLOCK punisher | NOT SHIPPED; `123 vs146` seq 14 GANG-BLOCK kill declined (deck123 had no rule; A51-10) | carried LOW |
| **R96 -> D25** X-menu exit | NOT SHIPPED; **second corpus**: `130 vs123` seq 40 `Cast Card Normally` then `X = 0` — the card spent for nothing | **R126** |
| **R97 -> D26** pain source at low life | NOT SHIPPED; **three takes in one game, the last LETHAL** | **R111** (HIGH) |
| **R98 -> D27** stale state block | NOT SHIPPED; `146 vs125` seq 162-167 annihilator asks again | carried LOW |
| **R99 -> D28** damage-order ask | NOT SHIPPED; `146 vs126` seq 52 answered "Wall of Omens" -> lane C re-asked and recovered | carried LOW |
| **R100** singular | NOT SHIPPED | carried LOW |
| **R101** cycling tag wording | NOT SHIPPED; `130 vs146` seq 6 Lay Waste cycled at 2 lands (the guide's exception now) | carried LOW |
| **R102** "pass and hope" | observation, closed | |
| **R103** latency | max **849 s** (`126 vs162` seq 22, 10,969-char reply, answer_replaced, correct); 26 replies > 120 s on 18 logs; 77/680 > 60 s at deck123/130 — the three longest at deck130 are 9-14-row own-permanent sacrifice menus | **R133** |
| **R104 -> D30** log share | **the D30 "no change" is now the sole cause of the 30 KB breach** | **R132** |

Of R72-R104: **R72, R73, R74, R76, R80, R81, R82, R87, R88, R89 SHIPPED and PASSED**; R75 shipped,
untested, scope short; R77, R78, R79 shipped and rendered but each exposed a sibling (R114, R116,
R117); R84 and R97 carried and PROMOTED by a failing window; the rest carried.

---

## HIGH

### R105 — a repeat-N activation launches a casting ask per iteration, each dropped answer counts toward the 6-streak livelock breaker, and Baka gets a MID-LOOP decision it answers by CASTING. (= ledger **D1**; engine seat **D-1 HIGH**, seat-123-130 **H4**) — **IN FLIGHT: lane G, `~/Projects/wagicGPT-w52-g`**
`1787864189-ai_baka_deck123-0x5593ba17b190-vs-ai_baka_deck126.jsonl` seq **47**: `CHOICE: 29 (Create
vampire with Lord of Lineage x17)` on `[repeat: you control 28 creatures; activated this turn 2
times already ...] x17`; seq **48** `kind: ask, fallback: stale_livelock, latency_ms: -1, reply: ""`
(no model call; verified on disk) with rows `Cast Devour Flesh {1}{b} ... / Cast nothing right now`
— the heuristic took row 1; seq 49 the model, asked to target a spell it never cast, named
`Yourself (player, life 17)`; seq 50 `Lord of Lineage (5/5)`; seq 51 events `Your Lord of Lineage
died / You gained 5 life (now 22) / Your repeated activation ran 7 of the 17 times you named (the
cost could no longer be paid)`. stderr `game-126v123-1787864185.stderr` 2145-2231: each `repeat
plan iteration k/17 (no model call)` is preceded by `dropping stale async answer` for a Casting
decision launched between iterations; ×6 -> `6 consecutive stale drops - giving this decision to
the heuristic`. The breaker (`kStaleLivelockLimit = 6`, AIPlayerGPT.cpp ~8117) was built for an
unstable prompt on an UNCHANGED state; here the state changes legitimately every iteration.
Precondition wave 50's four >=17 repeats lacked: a castable spell in hand during the loop. Ask:
no model request is launched (or none is counted toward the streak) while `repeat plan iteration`
runs — the row's own contract says "then returns priority to you here"; and a fallback of any
class never executes a non-pass row when a pass row exists (seat-123 H4's second half). Removes
no window: the loop's contract already defers them.
**Wave-52 prediction.** `stale_livelock` **0/N** (was 1); every repeat take runs `N of the N times
you named` unless a board reason is narrated in the same block; Baka-executed non-pass rows 0.

### R106 — the `Cast nothing right now` echo is matched against the FULL row text including `{...}` annotations, so `{right now: ... at 0 this does nothing}` on edict rows makes the pass row ambiguous: one dead Tutor cast, one `unparsed_reply` to Baka. (= ledger **D2**; engine seat **D-2 HIGH**) — **IN FLIGHT: lane G**
`1787864180-ai_baka_deck123-0x5611df0fea40-vs-ai_baka_deck125.jsonl` seq **48**: reply `CHOICE: 5
(Cast nothing right now)`; rows `1. Cast Tribute to Hunger {2}{b} {right now: they control 0
creatures - at 0 this does nothing} ... 5. Cast Idyllic Tutor {2}{w} [already owned: Intruder Alarm
...] 6. Cast nothing right now`; `parse_note: echo_index_conflict_ambiguous`, **executed 5** (the
dead Tutor; Cancelled at seq 50). seq **49**: `CHOICE: 4 (Cast nothing right now)` on a 3-row menu
-> `unparsed_reply`, Baka (both verified on disk). Mechanism (AIPlayerGPT.cpp ~13620-13660): the
echo's significant words (`nothing`, `right`) match every 0-creature edict row's annotation ->
`echoConflict` -> index wins / no remap out of range. Ask: match the echo against the row's SHORT
LABEL (text before the first ` {` / ` [`), or reserve the pass-row label as an exact echo.
Same-shape risk: any annotation vocabulary overlapping a row label.
**Wave-52 prediction.** `echo_index_conflict_ambiguous` / `unparsed_reply` on a reply whose
parenthetical equals a row's short label: **0/N** (was 2).

### R107 — the cleanup `discard` PUT parser executed a DIFFERENT card than the numbered row when the reply carried a bracketed name. (= ledger **D3**; seat-146-152-162 **E-1 HIGH**, deck162 G-2; skill #152) — **IN FLIGHT: lane G**
`1787864179-ai_baka_deck162-0x55a817091b20-vs-ai_baka_deck146.jsonl` seq **17** (T12 cleanup, hand
8): list `1. Peer into the Abyss / 2. Liliana's Caress / 3. Forced Fruition / ... / 7. Forced
Fruition / 8. Swamp`; reply `PUT: 3 (Forced Fruition)`; record `choice: 1, chosen_text: "Liliana's
Caress"` (verified on disk); narration `Cleanup discard (hand 8, limit 7): you chose Liliana's
Caress`. Path: `AIPlayerGPT::cleanupDiscard` (:22651) -> `parseAttackerSet(..., &names)` (:20288);
the name pass splits on commas and matches "forced fruition" against TWO identical names (rows 3
and 7) -> `uniqueNameMatch` -1 — which explains a miss, not row 2; mechanism NOT pinned by the
seat. The 11 other PUT lines (no bracket) mapped correctly. Cost: the deck's discard converter
binned on a board of three Mines and a full-handed opponent. Ask: the PUT parser honours a
bracketed name the way `parseChoice`'s named-row family does (index first; a name that matches
the indexed row confirms; a name that matches a different row -> one re-ask); PARSETEST on the
exact string against that 8-name list. The ask text's "number only" and 162-H's guide clause
are the interim, not the fix.
**Wave-52 prediction.** `PUT:` replies whose executed `chosen_text` differs from the printed
row(s) named: **0/N** (was 1/12).

### R108 — the payment planner taps EVERY Overgrown Battlement (a variable source worth {G} x defenders) for a cost ONE covers, floats the rest, and the row's `{paying this taps:}` forecast disagrees with the receipt — three walls tapped into a combat that took 18. (= ledger **D4**; seat-125-126 **HIGH #1**, deck126 G1; wave-50 D32's "Hive over-tap" carried) — **IN FLIGHT: lane H, `~/Projects/wagicGPT-w52-h`**
`1787864171-ai_baka_deck126-0x564fc96be4a0-vs-ai_baka_deck152.jsonl` seq **18** row `Cast Tribute
to Hunger {2}{b} ... {paying this taps: Overgrown Battlement, Overgrown Battlement - they cannot
attack this turn}` (TWO) -> seq **19** events `- Paid {2}{b} for Tribute to Hunger with Overgrown
Battlement #1; Overgrown Battlement #2; Overgrown Battlement #3; Woodland Cemetery` (THREE) and
`Already in pool: {g}x13 (13 mana ALREADY produced and floating)` (both verified on disk). seq
26 -> 27: Exquisite Blood, three Battlements tapped again; the same record's blockers window
offered only `B1. Pride Guardian`, `B2. Wall of Omens` against Huntmaster + two Wolves + two
Adversaries + fliers; 32 -> 14 (`you would be at 3`). seq 30/31: Sanguine Bond, five sources for
seven mana when one Battlement made five. Ask: when a variable source's output alone covers the
remaining generic cost the plan stops there; the `{paying this taps:}` clause is computed from
the plan the payment will actually execute so forecast == receipt. Suite pin: Tribute {2}{b} with
three 5-defender Battlements + one {b} land taps ONE Battlement.
**Wave-52 prediction.** `{paying this taps:}` count == receipt count on variable sources: N/N (was
1/2); Battlements tapped for a payment one covers: **0** (was 3/3).

### R109 — NO empty-stack priority window on the OPPONENT'S turn carries a `Cast` row — the pilot's instants are castable only in response, and the core prompt promises a window that never comes. (= ledger **D5**; seat-123-130 **H1 HIGH**, deck123 R4)
Seat-123's corpus-wide script: 51 opponent-turn empty-stack `priority` windows, 41 with an
`[instant]` in hand, **0 with a Cast row**; this step's stricter re-count (header `It is the
opponent's turn`, stack line empty or absent) finds 36 / 24 / **0** — concordant on the zero.
Costs: `123 vs125` seq 111 -> 112: Emrakul resolved in their turn-60 main 1 (seq 111 had it on
the stack, Tribute at N 0); the extra turn ran upkeep -> draw -> main 1 -> attack with no window,
so `Tribute to Hunger {right now: they control 1 creature - Emrakul ... you gain 15}` (which
resolved for +15 at seq 65 in the FIRST Emrakul's main 2) was never offered before annihilator 6
took the Alarm and 15 damage ended the game. `123 vs130` seq 8 (their upkeep, Tragic Slip in hand,
Blastminer on their line): five Flats rows, no Cast row — and that prompt's own text reads "You
will have priority again later this turn, so instants and activated abilities you hold stay
castable this turn" (verified on disk). Ask (engine/orchestrator): offer the pilot's castable
instants on the opponent's turn at the stops the activated abilities already get (upkeep, main
phases, beginning of combat), or at minimum one stop after a creature resolves on their side.
Ask (core prompt, until it ships): the "priority again later this turn" sentence is dropped or
restricted to the pilot's own turn — it is false on every opponent-turn window it prints on.
**Wave-52 prediction.** Opponent-turn empty-stack windows with an `[instant]` in hand and a Cast
row: > 0 (was 0/41); the false sentence on opponent-turn windows: 0.

### R110 — an index/name conflict where BOTH are on the menu executes the INDEX. (= ledger **D6**; seat-123-130 **H2 HIGH**; sibling of R74 / lane C)
`123 vs125` seq **48** (the R106 window): `CHOICE: 5 (Cast nothing right now)` — row 5 was the
Tutor, row 6 `Cast nothing right now`; `echo_index_conflict_ambiguous`, row 5 executed. Lane C's
`off_menu_name_in_range` covers a name NOT on the menu; here the name IS on the menu at another
index. The one `echo_index_conflict` where the name won (`146 vs125` seq 295 `CHOICE: 3 (Plains
#3)` on rows `1. Plains #2 / 2. Plains #3 / 3. Plains #4` -> executed 2) was correct. Ask: when the
parenthesised short name matches a menu row exactly and the index points elsewhere, the NAME
wins (or one `[RE-ASK]` quoting both, `index_name_conflict`) — the name is the pilot's intent,
the index a miscount. Partly discharged by R106's short-label match (the ambiguity goes away);
the rule stands for a genuine two-row conflict.
**Wave-52 prediction.** Replies whose parenthetical exactly names a row other than the indexed
one, executed at the index: **0/N** (was 1).

### R111 — a cast whose payment plan taps a self-damaging source for the pilot's LAST life point prints nothing lethal. (= ledger **D7**; seat-123-130 **H3 HIGH**, deck130 R1; wave-50 R97/D26 PROMOTED; skill #155)
`1787864160-ai_baka_deck130-0x556e1bc31180-vs-ai_baka_deck126.jsonl` seq **85** (`Your life: 1`):
`Cast Spark Spray {r} {leaves 0 of your 1 untapped mana source untapped - casting this taps you
out}`; the only source was Talisman of Impulse; receipt `Paid {r} ... with Talisman of Impulse`,
`Your Talisman of Impulse dealt 1 damage to you (now 0)`; gameend 0/24 with the opponent never
attacking (verified on disk). seq 82-83 the same at 2 -> 1, seq 49 at 3 -> 2. The frame prints
`CAUTION - some usable mana sources DAMAGE YOU`; the row that kills is silent. Lane B's planner
correctly used Talismans for generic pips WITHOUT damage (seq 49 `Paid {1} ... Talisman #2`, seq
79) — the clause is needed only when a COLOURED pip is planned onto a pain source. Ask: `{pays
{r} with Talisman of Impulse: you take 1 - life 1 -> 0, LETHAL}` on cast rows whose plan taps a
pain source for a coloured pip; the same for pain lands and phyrexian mana.
**Wave-52 prediction.** Casts paid by a pain source's coloured pip at life <= the damage with the
clause rendered: **0/N** (was 1 lethal + 2).

---

## MEDIUM

### R112 — per-token ability target rows are NOT collapsed: 26-34 `Equip with Lightning Greaves targeting Vampire #N` rows (~600 chars each) = the only non-log prompt near 30 KB; 9-14-row own-Goblin sacrifice menus = the three longest replies at deck130. (= ledger **D8**; engine seat **D-4**, seat-123-130 **M2**, deck123 R2, deck130 R3)
`1787864189-ai_baka_deck123-0x5593ba17b190-vs-ai_baka_deck126.jsonl` seq **44** (29,469 chars at
23% log, 35 rows, 26 equip rows — verified on disk), 42 (36), 47, 51 (34); each row repeats the
200-char `(Lightning Greaves is ALREADY attached to Lord of Lineage - this MOVES it ...)` clause,
the `[repeat: ...]` tag and the card text. `130 vs123` seq 26: 20 sacrifice rows; `130 vs126` seq
15/19/23/27/33/35: 11 rows each; latency 592 s / 652 s / 602 s on those windows. The target
clause already collapses `Vampire #1-#27 x27`; the option menu does not. Ask: one row per
identical-token group with `xN` (or the target as a sub-ask), as X-menu rows do.
**Wave-52 prediction.** No priority window with > 12 rows of the same verb+source over `#N`
targets; no non-log-driven prompt > 20 KB (was 29,469).

### R113 — `{this hits YOUR permanent}` is on damage/destroy rows only; exile / loyalty rows targeting the pilot's own permanent carry nothing, and Kaya exiled HERSELF at 1 life. (= ledger **D9**; engine seat **D-3**, seat-146 **E-5**, deck146 G-2)
`1787864173-ai_baka_deck146-0x55930e561360-vs-ai_baka_deck125.jsonl` seq **284** (`Your life: 1`):
`-3: exile non-land permanent with Kaya the Inexorable targeting Kaya the Inexorable [your
battlefield] [cost: Counters] {card text: ...}` — no self-hit clause (the full row read on disk
by this step; seat-146's unread `{if yo...` tail is not on this row), taken. Rendered: `Deal N
damage ... {this hits YOUR permanent}` 229, `Destroy ...` 30; untagged: `Equip ... [your
battlefield]` 118 (legitimate self-targets), Kaya `-3` self-exile 74, `+1 ghostform` 47 (benign),
`-3: emblem` 9, `-3: destroy` 2. Ask: the tag on every exile / destroy / sacrifice-class row whose
target is the pilot's own permanent; equip and buff rows stay untagged.
**Wave-52 prediction.** Self-exile / self-destroy loyalty rows carry the tag N/N; takes 0 (was 1).

### R114 — `[already owned: <name> on your battlefield]` marks SAME-NAME, not dead: 15 of 21 takes were stackable duplicates that were the plan, the two legendary takes bought a legend-rule bin with no clause, and the Tutor form names the half the pilot HAS and reads as a warning against the correct Tutor. (= ledger **D10**; engine seat **D-5**, seat-125-126 **MED #3**, deck126 G3; R79's second draft)
Stackables: Howling Mine ×4, Talisman ×5, Underworld Dreams ×3, Staff ×2, Elixir ×1 — every one a
live stack (`162 vs126` seq 9/13; `125 vs130` seq 20 Elixir #2). Legendary: `152 vs146` seq 41
`Cast Teferi ... [already owned: Teferi ... on your battlefield]` -> seq 43 `put into your
graveyard ... (that Teferi was 1 of 2 copies)`; `146 vs126` seq 20 Lolth — 4-5 mana to bin one
(the Lolth was a loyalty refresh, legitimate; the Teferi was not). Tutor: `126 vs125` seq 21
`Cast Idyllic Tutor {2}{w} [already owned: Exquisite Blood on your battlefield - this finds only
an enchantment card]` — Sanguine Bond was the card to find and the Tutor was RIGHT (taken, won);
`126 vs152` seq 31 both out — the warning right. Ask: three forms — `[a second copy stacks]` on
stackables (or no tag), `[legendary: the legend rule will put one into your graveyard]` on
legendaries, and on Tutor rows the half NOT owned: `[finds: Sanguine Bond - not on your
battlefield or hand]` / `[both halves already on your battlefield - a copy adds nothing]`.
**Wave-52 prediction.** Legendary second-copy casts with the legend clause rendered: **0/N** (was
2); Tutor rows naming the missing half N/N; dead-Tutor takes with that form 0 (was 4).

### R115 — `[from exile]` cast rows carry no cause and no tax; the model called the row a distractor / illegal for four windows at one seat and 248 s at another. (= ledger **D11**; seat-125-126 **MED #2**, deck126 G2, deck152 G-2; skill #154)
`126 vs152` seq **26** (248 s, 4,472-char reply): `2. Cast Idyllic Tutor {4}{w} [from exile]` ->
"Option 2 is likely a distractor ... 'from exile' implies it is in exile. But it's not listed in
my hand ... So Option 2 is illegal" (Elite Spellbinder's ETB, `canplayfromexile` +
`changecost(colorless:2)`; the exile line sits 30 lines up the log); seq 19 `Cast Staff of Nin {8}
[from exile]` (base {6}) cast with the tax unexplained; seq 30 `Cast Sanguine Bond {5}{b}{b} [from
exile]` taken, won. `123 vs152` seq 20/24/26/27: `Cast Intruder Alarm {4}{u} [from exile]` with the
PLAN "Intruder Alarm is exiled, I cannot cast it" four windows running (A51-3). Ask: `[from
exile - your card, exiled by their Elite Spellbinder; you may cast it from there at {2} more than
printed]` once per emitter path (Spellbinder, Kaya, the Cathar's blink); on the Spellbinder's
own hand-reveal ask, `{exiled this way it stays castable for {2} more}` on the header.
**Wave-52 prediction.** PLAN lines reading "exiled ... cannot cast" / "distractor" beside a `[from
exile]` row: **0** (was 5).

### R116 — the `becomes beholder` row RETURNS in the same window after a successful animation, tagged `[repeat: activated this turn 1 times already ...]`, and was taken 3 of 9 times — twelve mana for three 3/3s that already existed. (= ledger **D12**; seat-146 **E-2**; wave-50 D32 Hive carry; skill #165)
`146 vs125` seq 89 -> 91 (T47 Main 1), 110 -> 112 (T51), 260 -> 262 (T76, the row also carrying
`{spends 4 of your 7 ...; Silverquill Command needs 4}`); 0/3 at deck152. The guide's absolute
exists and was skipped in sequence (#138 — no fourth restatement). Ask: after a `becomes`
animation resolves, the identical row is not re-offered until the phase advances, OR is offered
with `{already animated this turn - a second activation adds nothing}` (the D4 `x0`-is-decline
family). The re-offer removes nothing the model could use; the annotation form is the
constraint-safe minimum.
**Wave-52 prediction.** `[repeat:` `becomes` rows taken after a same-window animation: **0/N**
(was 3/12).

### R117 — an `[Upkeep offer:]` Lair animation was taken and the animated land was then TAPPED FOR MANA by the Main-1 cast — the D7 annotation was not enough; the DEFER form is re-filed and re-rejected; the planner half is the adoptable lever. (= ledger **D13**; seat-146 **E-3**, deck152 G-1, 152-E)
`152 vs125` seq **36** (T17 Upkeep) `becomes a 1/1 hydra ... [Upkeep offer: ... offered again in
your main phase - declining here counts ...]` taken (plan "attack with Sigarda, Adversary and the
Hydra"); seq **39** `Cast Elite Spellbinder ... {paying this taps: Lair of the Hydra - it cannot
attack this turn}` taken; seq 41 attackers = Sigarda, Adversary. Both tags rendered; 34 of 35
tagged Upkeep offers at the seat were declined as round trips. deck152 G-1 re-files the DEFER
(skip the Upkeep offer unless the lethal check holds) — **rejected again under the governing
constraint**: it removes a legal window, and the lethal-check condition is a guide judgment the
engine should not encode. Adopted: (a) lane B's planner prefers a non-animated source over an
animated creature-land when another source can pay the pip (a creature that was just animated is
a source of last resort, like Katilda-granted mana already is); (b) the Main-1 cast row's
`{paying this taps: Lair of the Hydra}` gains `- it was animated this turn`.
**Wave-52 prediction.** Animated lands tapped by the planner while another source could pay:
**0/N** (was 1/1); Upkeep-animation takes with the tag: report (guide 152-E).

### R118 — `plan_choice_conflict` misses the single-row take, the no-PLAN reply, and the natural-language "we must pass". (= ledger **D14**; seat-123-130 **M3**, deck146 G-3, seat-146 **E-4**; R75/R91 residual)
Single-row `CHOICE: 1 (Create vampire ...)` under `PLAN: ... this window: pass` / "do not create
more tokens": `123 vs162` seq 29, `123 vs126` seq 37/44, `123 vs125` seq 15 (one token each). The
one repeat-row over-take (`123 vs126` seq 47, x17 at M 27 vs stop 24) carried NO PLAN line — the
conflict check had nothing to compare. `146 vs125` seq 282 (2 life): PLAN "Lolth's 0 ability is
suicidal ... We must pass and hope", CHOICE `1 (Lolth 0)`, executed, 2 -> 1. Ask (proposal, adds
one re-ask each): (a) extend the conflict to the single `[cost: Tap]` row when the PLAN says pass;
(b) a repeat-row reply with no PLAN line -> `plan_missing` re-ask (the guide's stop lives in that
line); (c) the PLAN-side pattern gains `\bmust pass\b|\bwe pass\b|answer is pass`. D20's counter
ships alongside so the class is measured.
**Wave-52 prediction.** Executed takes whose PLAN says pass in any of the three forms without a
re-ask: **0/N** (was 5); `plan_missing` re-asks on repeat rows: N/N.

### R119 — -N/-N spell target rows print `(P/T)` but no `{right now: -1/-1 - SURVIVES (toughness T)}`; damage rows do. (= ledger **D15**; seat-123-130 **M1**, deck123 R1)
`123 vs152` seq 11 Slip at `Elite Spellbinder (4/2)` under "it is 3/1, -1/-1 kills it" — lived;
`123 vs130` seq 21 at `Siege-Gang Commander #2 (2/2)` — lived. Ask: the lane-E clause computed
from the printed toughness on Tragic Slip-class rows. Guide half A51-5.
**Wave-52 prediction.** -N/-N takes at a target whose toughness exceeds N with the clause
rendered: 0 (was 2).

### R120 — fetch rows do not say the Flats makes no mana. (= ledger **D16**; seat-123-130 **M4**, deck123 R3; skill #166)
`123 vs130` seq 6-13: six passes with "when I have 3 mana" in the plan; `Mana available: 2` with
the Flats on the battlefield. Ask: `{this land makes no mana until sacrificed; taking this now
raises your untapped sources from N to N+1}`. Guide half A51-1.
**Wave-52 prediction.** Fetch rows passed while a hand spell needs the source: 0 (was 6).

### R121 — the `{spends K of your M ...; <card> in your hand needs K}` clause belongs on own-turn CAST rows whose `leaves N` is below the cheapest instant in hand, not only on ability rows. (= ledger **D17**; deck125 G1; skill #155)
`125 vs152` seq 24 `Cast Staff of Nin {6} {leaves 1 of your 7}` with Cancel {1}{u}{u} in hand;
`125 vs162` seq 24 the same with Fracture + Gavel + Scatter — three creatures / Ob Nixilis + Mine
#2 resolved next turn with no window; the gate sentence was in both plan lines (#102 exhausted).
`125 vs130` seq 8 shows the emitter path on an Elixir row. Ask: `Cast Staff of Nin {6} {leaves 1
of your 7 untapped mana sources untapped; Dream Fracture {1}{u}{u} in your hand needs 3}`.
**Wave-52 prediction.** Staff / Elixir / Revelation taken from a row whose `leaves N` is below the
cheapest counter in hand with the clause rendered: 0 (was 2).

### R122 — cast rows paid entirely from a floating pool print no `{leaves N}` clause. (= ledger **D18**; seat-125-126 **LOW #5**, deck126 G4)
`126 vs152` seq 12 `1. Cast Chromatic Lantern {3} {card text: ...}` with `Mana available: 0 total |
Already in pool: {g}{g}{g}`; seq 19 `Cast Staff of Nin {8} [from exile]` bare at 13 floating. Both
guides' gates key to `leaves N`. Ask: `{leaves 0 untapped sources and N floating}`.

### R123 — the edict row's named victim carries its tags. (= wave-50 R84 / ledger **D24** carried, PROMOTED by a failing window; deck126 F22)
`126 vs162` seq 17 `{right now: they control 1 creature - Fog Bank is sacrificed, you gain 2}` taken
at 13 life over a printed Bond row — Fog Bank is a `[defender]`, the guide's carve-out keys on the
tag. Ask unchanged: `Fog Bank [defender] (0/2) is sacrificed`.

---

## LOW

### R124 — receipts without a payee: 7 read `- Paid {1}{w} with <sources>` with no `for <card>`. (= ledger **D19**; engine seat **D-6**)
`152 vs123` seq 35, `152 vs125` seq 36 ×2 / 48, `152 vs162` seq 10, `152 vs146` seq 20 — all the
Intrepid Adversary ETB pay-any-number-of-times cost. Ask: `- Paid {1}{w} for Intrepid Adversary's
ability with ...`.

### R125 — duplicate identical rows without `#N` on damage-assignment / hand-reveal / library asks. (= ledger **D20**; engine seat **D-7**)
`146 vs126` seq 52 two `Perimeter Captain (0/4) [defender]` rows (the model's "Wall of Omens"
re-ask); `152 vs126` seq 25-28 `Overgrown Battlement (0/4) [defender]` ×2, seq 10 `Idyllic Tutor
[sorcery] [opponent's hand]` ×2; `146 vs125` seq 61 `Plains [land] [your library]` ×4. A name echo
cannot disambiguate them.

### R126 — an exit on the X menu: second corpus. (= ledger **D22** = wave-50 D25 carried; seat-123-130 **L1**, deck130 R4; skill #140)
`130 vs123` seq 40: `Cast Card Normally` answered on `even at X=4 NOTHING the OPPONENT controls
dies`; the only non-spending row was `X = 0 {... this cast does NOTHING ... the spell is spent}`.
Ask: carry the `Choose an option` menu's `Decline - do nothing` row onto the X menu — still
gated on the engine ruling whether the card can return to hand after `Cast Card Normally`.

### R127 — pass replies stamped with name notes. (= ledger **D21**; engine seat **D-8**)
`146 vs125` seq 160 `CHOICE: 0 (pass)` -> `stale_echo_in_range;named_row_not_offered`; `123 vs146`
seq 15 the same. Executed pass; noise for the corpus counts.

### R128 — a per-window WALL CHECK summary above the A-rows. (deck130 R2)
`130 vs126` seq 20/28: the converter clause rendered per attacker; the one number the pilot never
summed. `WALL CHECK: N of your attackers meet a lifegain blocker; blocking all of them costs you
2N life`. The guide half (D51-1) ships first; the row form if #158's ordering fix fails a third
time.

### R129 — a self-life-loss ability row carries the post-action life. (deck146 G-1)
Lolth's `0: draw, lose life` at 3 and at 2 life (`146 vs125` seq 269, 282) — `{right now: you go
to N}`; the controller-side twin of lane E's clause.

### R130 — chooser-attributed "used" narration reads as the wrong player acting. (deck125 G3, seat-125-126 LOW #4)
`125 vs123` seq 53 `You used: Gain life equal to its toughness with Tribute to Hunger targeting
Emrakul` is the OPPONENT's spell resolving on the seat. `You chose the sacrifice for their
Tribute to Hunger: Emrakul`. Cosmetic; not misread this corpus.

### R131 — the Caress-only `{feeds:}` K carries the hand size it depends on. (deck162 G-1; skill #156)
`converters on your battlefield: 1 - Liliana's Caress (converts only past 7 cards in hand; their
hand: 5)`. One window this corpus (`162 vs125` seq 8, hand 7).

### R132 — MEASUREMENT / DECISION: the game-log window is now the WHOLE 30 KB overrun — 3 prompts > 30,000, 102 above wave 50's max, 96 of them in one 80-turn game at 71-82% log; latency did not move with size. (= ledger **D23**; engine seat watch, seat-146 E-6, deck146 G-4; R104 continued)
Per-kind means up 2-19% (priority +19%) on the long game alone; discard p90 latency 54 -> 123 s
(31 asks; the 477 s max is a 29 KB cleanup prompt in the long game); the three longest replies
at deck130 are ROW-driven (R112), not log-driven. Decision for the owner: a turn-bounded or
size-bounded older-turn compaction is the only lever left on the log side; R112's collapse is
the lever on the row side and ships first. No change asked this wave; re-measure with R112 in.

### R133 — long-deliberation latency. (seat-146 E-6, seat-123-130 L2, seat-125-126 LOW #6)
Max 849 s (`126 vs162` seq 22, a 10,969-char self-revision that reached the right answer); 26 >
120 s on 18 logs; 77/680 > 60 s at deck123/130. Reasoning-bound; the row-count correlation at
deck130 routes to R112. Reported for the efficiency line.

### R134 — the X-menu header carries the seat's life when it is at or below the largest attacking power. (deck125 G2)
`125 vs152` seq 39 (X=7 offered at 1 life, not cast). The guide edit (W30) ships first; LOW.

### R135 — the opponent's deck NAME is not in the prompt. (deck125 G4; recorded, no change)
Every opponent-keyed rule keys to printed cards or the log, which is what the guides do — so
nobody writes "against Sanguine Blood" into a guide.

---

## SHIPPED SINCE THE CORPUS — recorded, NOT docketed
- Lane G (R105, R106, R107) and lane H (R108) are IN FLIGHT and docketed as such, not as shipped.

## GUIDE-ONLY — stays in guides after the render ships
- deck123: A51-1/A51-2 (Flats as a cost, the doubled pip; #166), A51-3 (`[from exile]` named;
  #154), A51-4 (the dictated Damnation sentence; #102), A51-5 (toughness = second number), A51-6/7
  (Tutor at one line + spare-first vs counters; #138), A51-8 (annihilator order; #143), A51-9/10
  (lone attacker, GANG-BLOCK port; #144), A51-11 (#151/#159), A51-12 (Skeins absolute cut; #118),
  A51-13 (own-upkeep narrowed to sweeper logs; #157), A51-14 (PUT shape).
- deck125: W26 (Revelation X floor; #131 pair), W27 (Scatter counts vs every opponent; #161), W28
  (Path target order), W29 (Emrakul vs edict-without-converter at <= 15), W30 (Revelation as a
  life spell; #162), W31 (cleanup under Caress).
- deck126: F21 (Lantern re-derived; #157), F22 (defender victim + printed row payable; #159), F23
  (both ownership lines; #160), F24 (two-land no-coverage hand; #153c).
- deck130: D51-1 (wall check first in the floor; #158), D51-2 (silent-family sentence retired;
  #154), D51-3 (Talisman keyed to `leaves 0`; #155), D51-4 (Starstorm two-list sentence), D51-5
  (damage spells never touch a land), D51-6 (Blastminer job; #157), D51-7 (Lay Waste exception),
  D51-8 (Captain + Bond tie).
- deck146: 146-M/Q (colour clause cut, carried plan; #153), 146-N (Kaya all-`[your battlefield]`
  -> +1 or pass), 146-O (Silencer naming order), 146-P (discard order: second walker before a
  ninth land).
- deck152: 152-G (Katilda A-row; #163), 152-H (Spellbinder tax + Tutor name; #164), 152-E (Upkeep
  tag re-key; #154).
- deck162: 162-G (RULE 2 keyed to `{feeds:}` K with the first-Mine and Caress exceptions in the
  sentence; #156), 162-H (PUT numbers only — interim until R107).

## MEASUREMENT items for the wave-52 reviewer briefs
1. Repeat takes: `N of the N times you named` N/N; `stale_livelock` 0; any ask record between a
   repeat take and its receipt (R105).
2. Echo/index conflicts by class: `echo_index_conflict`, `_ambiguous`, `unparsed_reply` with the
   reply's parenthetical and whether it equals a row's short label (R106/R110).
3. Every `PUT:` / list answer: reply numbers vs `chosen_text` (skill #152a), then PUT-vs-PLAN by
   ACTION words (#152b) — both numbers.
4. `{paying this taps:}` count vs receipt count on variable sources; Battlement taps per payment
   (R108); the lane-B minimum check again (R73 regression).
5. Opponent-turn empty-stack windows with an `[instant]` in hand: Cast rows offered / taken (R109).
6. Pain-source coloured-pip payments at life <= damage, with/without the clause (R111).
7. Rows per window by verb+source; max non-log prompt; per-kind LOG SHARE (R112/R132), 21/21.
8. `[already owned:` takes by class (stackable / legendary / Tutor) with the new forms (R114);
   `[from exile]` rows and the PLAN text beside them (R115).
9. `[repeat:` `becomes` re-offers and takes; animated lands tapped by the planner (R116/R117).
10. Self-target takes by row family with/without `{this hits YOUR permanent}` (R113).
11. Mulligan chains: looks past (keeping 5) with >= 1 land; replies naming a colour as the reason
    (skill #153) — per seat, odds text quoted.
12. Quoted-literal grep per ROW FAMILY with the tag-coverage column (skill #154) and the
    cross-guide KEY diff (#144).

## PASS / KEEP — measured working, do not re-open
Lane A's Soul Shatter script (0/15 no-sacrifice, walkers 2/2, `{right now: they sacrifice}` 6/6)
· lane B's planner (1/421, 0 over-taps) · lane C's named-row re-ask (3/3 recovered, `stale_echo`
to Baka 0) · lane D's receipts (53/53 activations, `; ` separator, `turn` 2,070/2,070, `with no
source tapped` 0) · lane E's ability DIES/SURVIVES (428/428), Upkeep annotation (0/35 Hive takes),
`{spends K ...}` (0/141 Hammer-class) · lane F's `{feeds:}` (78/78), `(text omitted)` (bare
endings 0) · lane W's self-target tag on damage/destroy rows (0 takes on 259) · the wave-50 PASS
set (double-tap, mana-only gate, target-clause collapse, gain recipient, FORECAST mirror, exemplar
parse, content-keyed expiry) — no regression observed · land drops 0 declines pool-wide (deck125
47/47, deck152 34/34, deck162 33/33, deck146 40/41 with lethal on board) · 21/21 natural, Baka
fallbacks 0.23%, max priority windows/turn 10 · **no render falsehood this corpus** — everything
open is an omission, a parser scope, a planner ordering, or the engine acting between answers
(R105, R107, R108).
