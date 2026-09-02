# Wave-52 general strategy — RENDER / CORE-PROMPT proposals, layer-routed (R1-R135 revised, R136-R160 new)

Corpus `matchups-20260901-223430` (NINTH fair-hand corpus), binary master **dddb6f62b** (lanes
G 8c3e10e28 / H d58aeba6b / I 4bdca64bf / J aa8509282 / K b1fba8eff / L 0f7dff3b0 + the owner
batches bd515aad7 / c15106f4d / 4abcf5dd0 / the 2026-09-01 Vita batch / c1a0bf814 option-preserving
autotap / 5ca11d6e2 Baka targeting; PARSETEST 2121/0, gate 1210 + 44 AI/0). Pilot
**qwen36-35b-a3b** (FP8) on Spark :8084 — the 122B is retired (owner, 2026-09-02). 42 seat logs,
**3,384 records**, **3,300 decisions** (ask 2,540 / priority 535 / attackers 107 / discard 54 /
blockers 41 / reveal 15 / bottom 8), **21/21 games natural — 42/42 seats carry a `gameend`**
(re-verified on disk by this step). **15 `fallback` fields = 0.45%**: `plan_choice_conflict` 5,
`empty_reply` 3, `unparsed_reply` 3, `multiblock_reask` 3, `named_row_reask` 1; **Baka-executed 6
= 0.18%**. Longest seat **353** records (`deck152-vs-deck125`, 53 turns); max prompt **31,551**
(`deck146-vs-deck152` seq 107, priority, turn 27) with **7 prompts over 30,000**. Total inference
**79,362 s = 22.0 h**. Sources: the four seat files, seven `deckN/general-suggestions.md`, seven
`deckN/skill.md`, and the corpus directory for every reconciliation.

**Routing rule (owner doctrine 2026-08-26).** The core loop — engine mechanics, LLM UX, bugs — is
the purpose. Everything below is a change to what the ENGINE RENDERS, what the PARSER executes, or
the CORE PROMPT's own text. Guide-side answers are named only where a rule must stay correct AFTER
the render ships. Engine BUGS with call sites are in `engine-ledger.md` (the wave-53 docket); this
file is the ranked routing view with the evidence and the wave-53 falsifier for each item.

**Governing constraint on every item.** Legality is enforced structurally; the model's CHOICE is
never constrained. Every proposal ADDS a true token, a true row, a receipt or one re-ask, and
DELETES nothing. Hard caps are rejected on sight. **Four proposals this wave would REMOVE a legal
window and are rejected in that form** — a blind identical-option-set cache (seat-146 E-2,
deck152 G5a), a fixed one-ask-per-opponent-turn cadence at the end step (deck162 G9), skipping any
window all of whose cast rows carry a "does nothing" clause (deck146 G2, deck125 G4, deck126 #6,
seat-123-130 M1), and requiring a prose verdict to NAME a row before `plan_choice_conflict` may
fire (seat-123-130 H1, deck130 G8). The evidence for each rejection is under R137 and R136-note;
the adopted forms are a model-owned HOLD answer (R137) and better telemetry (R145, R147).

---

## STATUS OF R1-R104, by the emitter's actual string

**R1-R71 (wave-48/50 items).** Verdicts unchanged from wave 51 except where a wave-52 lane touched
them; the touched ones are re-stated here, the rest CARRY as recorded in `wave51/general-strategy.md`.

| item | wave-52 verdict | evidence |
|---|---|---|
| **R1** loop affordance | **HOLDS and improved**: max priority windows in one turn **7** (was 10); `repeat plan iteration` 62 with `cast ask NOT issued - repeat plan in progress` 124 — lane G's deferral firing | `[repeat:` rows 61, takes 11; `repeat_count_zero_pass` 1 (first live firing, `123 vs162` s35) |
| **R2** A-row collapse | HOLDS on attackers/battlefield lines; **STILL FAILS on ability target menus** | `123 vs162` seq 41 = 40 rows / **2** distinct shapes after masking `#N`, seq 42 = 39 rows / **1** — **R142** |
| **R13/R71 -> R104 -> R132** GAME LOG share | **the log is now hard-capped and the tail is capped-log + rows** | 165 prompts carry `(...earlier events trimmed...)`; mean log share 69%; 7 prompts > 30,000 — **R160** |
| **R19/R38 -> R108** what paying taps | **forecast == receipt on variable sources, 0 rows name two Battlements** (was 2/2); ONE residual receipt over-tap | `126 vs125` seq 86 forecast names ONE Battlement, seq 87 receipt names TWO — ledger D-LOW |
| **R32/R64 -> R73** auto-tap order | **HOLDS** (not exhaustively re-audited this wave); forecast-vs-receipt subset check 10/10 | receipts 431 unique lines, **0 `with no source tapped`** |
| **R67 -> R79 -> R114** second copy | **SHIPPED and split correctly; a NEW residual: the tag answers legality, not usefulness** | **R146** |
| **R70** counter-target size | CARRIED; third clean corpus -> **CLOSED** | |

**R72-R104 (wave-50 items).** All those recorded SHIPPED-and-PASSED in wave 51 held by string this
corpus and are not re-litigated: R72 (Soul Shatter script — but see **R139**, a different defect on
the same card), R73, R74, R76, R80, R81, R82, R87, R88, R89. R75's `x0`-is-decline had its **first
live firing** (`123 vs162` s35, `repeat_count_zero_pass`, executed pass) — **PASS 1/1**. R83, R85,
R90, R92, R93, R94, R95, R98, R99, R100, R101 carried with no failing window. R84/R123 (edict victim
tags) carried: 3 named-victim Tribute rows, all non-defender, all taken in their first window — no
failing window this corpus. R91 -> R118 discharged by lane J (below). R96/R126 (X-menu exit): the
failing shape did **not** recur (deck130 reached 3 X menus, marker taken 2/2, `Cast Card Normally`
on a nothing-dies row **0**, `X = 0` **0**) — carried LOW, still gated on the engine's cancel ruling.
R97 -> R111 discharged by lane K. R103/R133 latency: max **900.0 s** — three decisions hit the
`WAGIC_GPT_TIMEOUT` wall exactly and returned empty (**R145**).

---

## STATUS OF R105-R135 — the wave-52 lanes, by the emitter's actual string

| item | lane / docket | wave-52 verdict | evidence (re-derived on disk) |
|---|---|---|---|
| **R105** livelock inside a repeat loop | G / D1 | **SHIPPED, PASS** | `stale_livelock` **0** in 3,384 records and 0 in stderr; `consecutive stale drops` 0; 47 single stale drops, none consecutive; `cast ask NOT issued - repeat plan in progress` 124 over 62 `repeat plan iteration` |
| **R106** pass-row echo vs annotation | G / D2 | **SHIPPED, PASS 0/543** | 543 rows render `at 0 this does nothing`; **0 taken**; no reply naming "nothing" executed one |
| **R107** PUT executed a different row | G / D3 | **SHIPPED, PASS 0/77** | 77 reveal/discard/bottom records; 0 replies carried both `PUT:` and `CHOICE:`; every `PUT:` executed its printed row (deck162 3/3, deck123 8/8, deck125 37/37) |
| **R108** planner over-taps variable sources | H / D4 | **SHIPPED, PASS on the forecast (0/101 name a wall twice); ONE residual receipt** | `- Paid {1}{g} for Overgrown Battlement with Overgrown Battlement` (one tap, two mana; primitive `{T}:foreach(creature[defender]|myBattlefield) add{G}` confirmed). Residual: `126 vs125` s86 forecast ONE wall, s87 receipt TWO |
| **R109** no opponent-turn Cast row | I / D5 | **SHIPPED — the wave's biggest capability change; the COST is the new item** | **884 opponent-turn casting asks** (wave 51: 0) + 82 opponent-turn priority windows; **68 casts** (Cancel 16, Essence Scatter 14, Fall of the Gavel 12, Dream Fracture 10, Path 5, Soul Shatter 4, Tribute 3, Spark Spray 1, Vanishing Verse 1, Tragic Slip 1, Revelation 1). The false "priority again later this turn" sentence: **0 on opponent-turn windows**. Cost -> **R137** |
| **R110** index/name conflict executes the index | J / D6 | **SHIPPED, PASS 1/1** | `126 vs152` seq 45 `CHOICE: 3 (Play no land right now)` on rows 3 = `Play Sunpetal Grove`, 4 = `Play no land right now` -> `echo_index_conflict;name_over_index`, executed **4**. The wave-51 shape 0/N. Residual: an off-menu `(Pass)` parenthetical — **R144** |
| **R111** pain-source lethal clause | K / D7 | **SHIPPED, PASS 0/N; the LETHAL tail UNTESTED (0 renders)** | 15 rows carry `{paying this costs you N life (Talisman of Impulse: 1 damage) - you would be at 19}`; 1 taken at 20 life; casts at <= the damage with no clause **0**. The same arithmetic on a DRAW price is untagged and killed a seat — **R138** |
| **R112** per-token rows uncollapsed | L / D8 | **SPLIT: PASS on cast-row target clauses (97 uses) and battlefield lines; FAIL on ability menus** | `123 vs162` seq 41/42 — **R142** |
| **R113** self-hit tag on exile/loyalty rows | K / D9 | **SHIPPED, PASS 179 rows / 0 takes** | `-3: exile non-land permanent` 15 tagged (was 74 untagged), `-2: destroy ... draw two` 8, `-6:` 1, damage 155. Wave-51's Kaya self-exile has no successor |
| **R114** `[already owned:` classes | K / D10 | **SHIPPED, PASS on all three forms** | `[already owned:` **0 renders**; `[legendary:` 19 rows, **19/19 carry the legend clause**, 4 taken (all legend-rule refreshes); `[second copy:` 70 rows / 28 takes; tutor `[finds only ...]` 44 rows, 4 variants, **0 warn against the half the pilot lacks**, 0 takes of a `none left` row. Residual -> **R146** |
| **R115** `[from exile]` cause and tax | K / D11 | **SHIPPED, PARTIAL — the cause is not durable, 4/17** | `146 vs152` seq 32 carries `exiled by their Elite Spellbinder`; seq 36/56/68/75/81/87/98/104 do not. **Mechanism pinned this step** — **R143** |
| **R116** `[repeat:` `becomes` re-offer | L / D12 | **SHIPPED, PASS 0** | 138 `becomes` rows, 2 takes, **no `becomes` row re-offered in the same window**; `[repeat:` rows on deck146's Hive **0 offered** (was 9 offered / 3 taken) |
| **R117** animated land tapped for mana | L / D13 | **SHIPPED, PASS 0** | every `- Paid` receipt cross-checked against the same turn's `becomes ... with <land>` narration: 0 same-turn taps; rows carry `[this land is TAPPED: animated, it still cannot attack this turn]` |
| **R118** conflict scope | J / D14 | **SHIPPED, PASS — 5 firings, 5 recovered, and it CHANGED the executed action in 3 of 5** | see the reconciliation note below; `plan_missing` **UNTESTED** (0 renders, no counted repeat take arrived without a PLAN) |
| **R119** -N/-N DIES/SURVIVES | L / D15 | **SPLIT: PASS 9/9 on target rows; NOT APPLIED on cast rows 0/85** | 85 `Cast Tragic Slip` rows carry `{right now: -1/-1 (no creature has died this turn, so Morbid does NOT apply)}` and no per-target verdict — **R140** |
| **R120** fetch makes no mana | L / D16 | **SHIPPED, PASS 69 renders** | `{this land makes no mana - crack it for a land: your untapped mana sources go from N to N+1}`; deck123 passed-while-a-spell-needed-the-source **0** (was 6) |
| **R121** `{spends K of M}` on cast rows | L / D17 | **SHIPPED, PASS** | 736 rows carry `{spends `, **546 on `Cast ...` rows**; companion `{leaves K of M}` 2,390 |
| **R122** floating-pool `{leaves N}` | D18 | CARRIED; 2 receipts read `from mana already floating` (a legitimate form) | |
| **R123** edict victim tags | D24 | CARRIED — **no failing window** | 3 named-victim Tributes, none a `[defender]`, all taken in the first window |
| **R124** receipts without a payee | D19 | CARRIED, **narrower: 3** (was 7) | all the Intrepid Adversary ETB class (`152 vs123` 15, `152 vs146` 16, `152 vs162` 10) |
| **R125** `#N` on duplicate rows | D20 | CARRIED, no new window | |
| **R126** X-menu exit | D22 | CARRIED LOW — the failing shape did not recur; still gated on the cancel ruling | |
| **R127** pass-note noise | D21 | CARRIED LOW — 1 `stale_echo_in_range` (was 2) | |
| **R128** WALL CHECK summary line | deck130 R2 | CARRIED LOW — the guide's ordering fix ships this wave (#177); the failure mechanism was the DECIDING SITUATIONS bullet dropping a branch, not missing arithmetic | `130 vs146` s35 |
| **R129** self-life-loss row | deck146 G-1 | CARRIED LOW — no failing window (Lolth `+0` 12 offered, 1 taken at 21 life, floor 6) | |
| **R130** chooser-attributed narration | | CARRIED LOW, not misread this corpus | |
| **R131** Caress-only `{feeds:}` K | D26 | CARRIED, and a stronger form is proposed — **R153** | |
| **R132** log share | D23 | **RE-MEASURED with the cap in**: 165 trims, log segment max 25,431 / p90 21,953 / p50 8,452; max non-log segment **11,361**; 7 prompts > 30,000, all 70-86% log — **R160**, the owner's decision | |
| **R133** latency | | 22.0 h total; p90 down at 5 of 7 kinds; **3 decisions at the 900,000 ms wall** — **R145** | |
| **R134** X-menu header life | | CARRIED LOW, 0 windows | |
| **R135** opponent deck name | | no change | |

**Reconciliation note on R118 (the two seats disagreed and the records settle it).** seat-123-130
**H1** and `deck130/general-suggestions.md` **G8** report `decision_reversed_in_prose` as firing on
deliberation, "right 0 times", and ask that a prose verdict NAME a row before it counts. Read on
disk, all five records carry `coded_answers: 2` or a prose pass verdict, and **three carry
`answer_replaced: True`** — the reply's first coded line was `CHOICE: 0 (pass)`, the model then
re-derived at length and emitted a SECOND coded line taking the row, and the engine had latched
that second line (`choice: 1`). The re-ask reverted it to pass in `130 vs125` s48 -> s49, s66 -> s67
and `146 vs125` s59 -> s60, and confirmed the row in `130 vs125` s82 -> s83 and `146 vs125`
s133 -> s134. So the check **changed the executed action in 3 of 5 firings**, each time away from a
5-mana Hammer return or a Lolth activation the model's own final answer refused. The engine seat's
"3 of 5 were genuine reversals" is right; seat-123-130's H1 read the head of a 3,290-character reply
and did not reach its tail. **The proposal is REJECTED**: requiring a named row would suppress
exactly the three firings that changed the answer. What IS adopted is the telemetry half (seat-146
E-4): the record must say which coded line was latched and must log the re-ask's own reply — **R147**.

---

## HIGH

### R136 — a `multiblock_reask` can end with ZERO blocks declared, silently, with no follower record and no `chosen_text`. (= ledger **D1**; engine seat **D-1 HIGH**, seat-146 blocks)
`1788320102-ai_baka_deck152-0x557298116fa0-vs-ai_baka_deck162.jsonl` seq **26** (turn 14, blockers,
24 life): rows `A1. Fate Unraveler #1 (3/4) deals 3 / A2. Fate Unraveler #2 (3/4) deals 3 / B1.
Katilda (2/2) / B2. Elite Spellbinder (6/4) [flying] - may block A1, A2 (all: you kill it, your
blocker lives) / B3. Luminarch Aspirant #3 (1/1)`; reply `BLOCKS: B2:A1, B2:A2`;
`fallback: multiblock_reask`, `dropped_assignments: 1`, **`chosen_text` null**, **no follower
record**. `game-152v162-1788320098.stderr:1123` `AIPlayerGPT: one-blocker-many-attackers reply ->
re-asking once` is followed by NO `declared blocks from N assignment(s) in one reply` — the next
lines are `Action added to stack: NextGamePhase. (Current phase is: Blockers)` and the damage
resolution. seq 27's events: `- Opponent's Fate Unraveler dealt 3 damage to you (now 21) / -
Opponent's Fate Unraveler dealt 3 damage to you (now 18)` — **both attackers unblocked**, though the
reply's FIRST assignment (`B2:A1`, a 6/4 killing a 3/4 for free) was legal. The other two
`multiblock_reask` records DO carry a follower (`152 vs146` 76 -> 77, `126 vs152` 54 -> 55, both
`multiblock_reask_recovered`). The seat went 24 -> 18 -> 3 and lost.
**Ask.** Keep the first legal assignment per blocker and drop only the extras (the parser already
counts them in `dropped_assignments`); always write the re-ask's record. Removes nothing: the model's
first assignment is its own choice.
**Wave-53 prediction.** `multiblock_reask` records with no follower and no `chosen_text`: **0/N**
(was 1/3); every one-blocker-many-attackers reply is followed in stderr by `declared blocks from N
assignment(s)`.

### R137 — the opponent-turn window is the wave's biggest capability win and its biggest cost: 884 casting asks + 82 priority windows for 85 acts, and 286 windows at six phases that have never produced one. (= ledger **D2**; engine seat **D-2 HIGH**, seat-146 **E-2/E-3 HIGH**, deck123 G6, deck125 G4, deck126 #6, deck152 G5, deck162 G9)
Re-derived corpus-wide by this step (the seats used three different denominators and each is right
on its own): **884** `Casting decision (…, opponent's turn)` asks, **68 casts (7.7%)**, 10,979 s;
**82** opponent-turn `priority` windows, 17 acts; **966 opponent-turn windows / 85 acts (8.8%)**
against 1,001 own-turn casting asks at 37.8%. Per phase (windows / casts / seconds): Main 1
196/58/3,448 · Upkeep 163/1/1,597 · Draw 92/1/1,088 · End 78/**0**/724 · Main 2 77/6/897 · Combat
begins 70/2/825 · Cleanup 68/**0**/739 · Attackers 67/**0**/761 · Combat ends 67/**0**/763 ·
Blockers+Combat damage 6/**0**/137. **286 windows across six phases produced zero casts and cost
3,124 s (52 min).** The repetition, measured two ways: **760 of 3,300 decisions (23.0%)** sit in a
run of 3+ CONSECUTIVE records with a byte-identical option set inside one turn, all declined,
**143.2 min = 10.8% of the corpus's 22.0 h**; grouping non-consecutively by (turn, option set) gives
116 groups / 921 records / 188.2 min. Purest specimens: `162 vs126` seq 7-18 (twelve asks in one
opponent turn, `1. Cast Dictate of Kruphix {1}{u}{u} / 2. Cast nothing right now`, twelve declines,
452 s); `123 vs152` turn 9 seq 22-35 (**14** consecutive, 699 s); `126 vs125` turns 21-37 (thirteen
per turn, ten turns running).
**⚠ THE OBVIOUS REMEDIES ARE REJECTED, ON THE RECORDS.** A blind "identical option set already
declined this turn ⇒ reuse the decline" cache would have suppressed **30 real actions corpus-wide**
— the specimen is `130 vs152` seq 12 -> 13, two BYTE-IDENTICAL `Cast Spark Spray / Cast nothing`
menus in turn 3, declined at Main 1 and **cast at Combat begins**; the model treats phase
progression as information the menu does not carry. deck162 G9's "one ask per opponent turn, at
their end step" is a hard cap on legal windows AND aims at the one stop that produced **0 of 78**;
rejected twice over. deck146 G2 / deck125 G4 / deck126 #6 / seat-123-130 M1's "do not open a window
all of whose cast rows carry a dead-row clause" removes a legal action from the menu (543 such rows,
0 taken — and R139 shows the clause itself can be stale against a pending stack object).
**Ask (adopted, constraint-safe).** Give the pilot a HOLD answer it may take: a row on the
opponent-turn window reading `Hold priority - do not ask me again this turn unless the board changes`
(the engine re-opens on any board change, new stack object or newly affordable row). The choice
stays the model's, no window is removed, and the 286 dead-phase windows become one answer instead of
nine. Second, cheaper half if the row is not built: carry the count on the row
(`[you declined this exact list N times already this turn]`) so a guide can say PASS without
re-deriving.
**Wave-53 prediction.** Opponent-turn casting windows per game **down >= 30%** with opponent-turn
casts held at **>= 90%** of this wave's 68; the End / Cleanup / Attackers / Combat-ends stops
contribute **0** casts again; consecutive identical-declined runs of 3+ **< 10%** of decisions
(was 23.0%).

### R138 — a `[DRAW PRICE:]` total at or above the pilot's life carries no lethal clause, while lane K prints one for the same arithmetic on a mana cost. (= ledger **D3**; seat-125-126 **HIGH #3**, deck125 G3)
`1788320083-ai_baka_deck125-0x557cd81d6290-vs-ai_baka_deck162.jsonl`: **41 rows across 22 windows**
(turns 18-20) carry `[DRAW PRICE: casting this draws YOU 7 cards (their Forced Fruition), and their
Underworld Dreams deals you 7]` while the seat's life is **7 or less**. seq **91** (3 life) is the
positive control — a seven-row menu, every row carrying the clause, answered `CHOICE: 7 (Cast
nothing right now)`. seq **97** (turn 20, still 3 life) is the same menu and the reply cast `Essence
Scatter`; seq 98's events read seven draws and `Underworld Dreams dealt 1 damage to you (now 2)`,
seq 99 is at 1, `gameend` 0. Emitter: `castDrawPriceRowTag` (AIPlayerGPT.cpp ~6797-6809) has no
lethal tail; `paymentLifeCostClause` (~3131-3153) already computes `- you would be at K` and
`- this KILLS you` for the mana path (15 renders, 0 breaks). The two clauses are the same
arithmetic reaching the same conclusion through different costs.
**Ask.** `[DRAW PRICE: ... deals you 7 - you would be at -4; this KILLS you]`. One string; it turns
a rule the guide has to teach (#173) into a fact the row states.
**Wave-53 prediction.** A cast taken from a row whose printed price names a number at or above the
seat's life: **0/N** (was 1); every `[DRAW PRICE:]` whose damage >= the seat's life carries the
lethal tail: **N/N** (was 0/41).

### R139 — the edict forecast is computed from the battlefield and ignores a sacrifice ALREADY ON THE STACK aimed at the same permanent: three Soul Shatters, one Emrakul, one death. (= ledger **D4**; seat-146 **E-1 HIGH**, deck146 G1)
`1788320094-ai_baka_deck146-0x55c97c0102d0-vs-ai_baka_deck125.jsonl` seq **327 / 328 / 329** (turn
50, Main phase 1, 9 life vs 48). Each row reads `Cast Soul Shatter {2}{b} {right now: they sacrifice
Emrakul, the Aeons Torn (MV 15, their highest)}` and each was taken. **The mechanism, pinned on disk
by this step** (both readings offered by the seat are wrong): seq 328's events are `- Paid {2}{b}
for Soul Shatter ... / - You cast Soul Shatter / - Your Soul Shatter resolved and went to your
graveyard / - Opponent used: sacrifice a creature or planeswalker with Soul Shatter targeting
Emrakul` — the granted ability is **put on the stack and has not resolved**, so the battlefield line
truthfully still lists Emrakul, and the seat is offered the cast row again. seq 328's stack block
reads `1 (top): ability: sacrifice a creature or planeswalker [triggered/activated ability]`; seq
329's reads the same **twice**. Only at seq 330 do the events show `- Opponent's Emrakul, the Aeons
Torn died`, once. Not protection (`abilities=nofizzle,flying,protectionfromcoloredspells`; Soul
Shatter's `auto=ability$!... sacrifice notaTarget(...)!$ opponent` does not target, and wave 51
recorded Emrakul being sacrificed correctly) and not a coalescing bug: **three abilities were
created, one found a permanent to take.** Lane A is NOT reopened — nothing failed to sacrifice.
**Ask.** Two true tokens, no window removed: (a) the stack block names the ability's source and its
victim — `ability: Soul Shatter's sacrifice (aimed at Emrakul, the Aeons Torn) [from your Soul
Shatter]` — instead of an anonymous `ability: sacrifice a creature or planeswalker`; (b) the
`{right now: they sacrifice <name> (MV N, their highest)}` clause continues `- a sacrifice is
already on the stack aimed at this permanent; this one would find their next-highest` when an
unresolved object in this window already names it. Either alone stops casts #2 and #3.
**Wave-53 prediction.** Edict cast rows whose `{right now:}` names a permanent already named by an
unresolved stack object, with no such clause: **0/N** (was 2/3 in one window); stack-block lines
reading a bare `ability: <effect>` with no source: **0/N**.

### R140 — a targeted spell's CAST row carries no kill summary, so the REFUSABLE window has less information than the forced ask that follows it. (= ledger **D5**; seat-123-130 **H3**, engine seat **D-7**, deck123 G2; R119's other half)
`1788320077-ai_baka_deck123-...-vs-ai_baka_deck146.jsonl` seq **18**: `1. Cast Tragic Slip {b}
{right now: -1/-1 (no creature has died this turn, so Morbid does NOT apply)} {leaves 0 of your 1
untapped mana source untapped - casting this taps you out} - legal targets right now: Nadaar,
Selfless Paladin, Triumphant Adventurer, Goblin` — taken at 6 life; seq **19**'s target ask then
printed `- SURVIVES (toughness N, -1/-1 more kills it)` on **all three** rows above the sentence
"this ask has no pass row". Corpus-wide: **85 `Cast Tragic Slip` rows carry no verdict; 9 target
rows carry one.** The engine already computes the number one screen too late to be refusable.
deck130's `Cast Starstorm` row is the working template (`{X pricing: ... even at X=N NOTHING the
OPPONENT controls dies}` — 0 wasted casts in 60+ windows), and `123 vs125` seq 19 shows a second one
(`- the only legal targets are YOUR OWN right now:` on a cast row).
**Ask.** On a targeted spell's cast row, append the summary the target rows already compute:
`{kills 0 of the 3 legal targets at -1/-1}` / `{kills: Elite Spellbinder}`. Same for damage cast
rows and for the two-step fight flow (`152 vs146` seq 49 -> 50 carries no forecast on either half).
**Wave-53 prediction.** `Cast <targeted spell>` rows carrying a per-target DIES/SURVIVES or an
explicit kill count: **N/N** (was 0/85); takes of a targeted removal spell whose every legal target
survives: **0** (was 1).

---

## MEDIUM

### R141 — `{paying this taps: <defender> - it cannot attack this turn}` states a consequence of ZERO for a wall and hides the only consequence that exists. (= ledger **D6**; seat-125-126 **HIGH #1**, deck126 G6)
Emitter: `paymentTapsClause` (AIPlayerGPT.cpp ~3108-3120) appends `" - " << (one ? "it" : "they")
<< " cannot attack this turn"` whenever the caller sets the flag, with no test on the source's
abilities. Re-derived: **101 `{paying this taps:` rows; 91 name a defender** (`Overgrown Battlement`,
`abilities=defender`, `mtg.txt:83596`), and **22 of those 91 carry the "cannot attack this turn"
tail** — the seat's "91 such rows" is the defender-naming denominator, the tail's own count is 22.
All sixteen creatures in deck126 carry `defender`, so the stated cost is empty and the real one —
the wall is tapped through the opponent's combat and cannot BLOCK — is unstated. Repro:
`126 vs130` seq **13** (turn 9, 14 life, `Cast Overgrown Battlement {1}{g} ... {paying this taps:
Overgrown Battlement - it cannot attack this turn}`, taken; 14 -> 8 on the next combat, Rorix
Bladewing 6/5 flying on their line) and `126 vs125` seq **86**.
**Ask.** When the tapped source has `defender`, print `- it cannot block on their turn`; with
neither restriction keep the current words; when a row taps several sources with different
restrictions, name each with its own consequence. Suite pin: a cast row tapping one Overgrown
Battlement prints "cannot block on their turn" and never "cannot attack this turn".

### R142 — lane L's `#a-#b` collapse does not reach ABILITY target menus: 40 near-identical Equip rows, one distinct shape. (= ledger **D7**; engine seat **D-5**; R112's residual)
`1788320108-ai_baka_deck123-...-vs-ai_baka_deck162.jsonl` seq **41** (40 rows, **2** distinct shapes
after masking `#N`) and seq **42** (39 rows, **1** shape), turn 12, priority: every row is `Equip
with Lightning Greaves targeting Human #N [your battlefield] (Lightning Greaves is ALREADY attached
to Thraben Doomsayer - this MOVES it to Human #N, and Thraben Doomsayer loses what it grants)
[repeat: activated this turn 1 times already...] {card text: ...}`. Primitive confirms
`Lightning Greaves ... auto={0}:equip` — the equip is free, so the menu is offered at every priority
window while the tokens exist. The same collapse works one line away on cast-row target clauses
(`Overgrown Battlement #1-#3 {1}{g} (creature 0/4) [defender] x3`, 97 uses) and on battlefield lines
(`Vampire #1-#33 (4/4) ... x33`). **Adjudication:** seat-123-130 scored this PASS ("40 distinct
creatures, one row each"); on the records the 40 rows collapse to 2 shapes and 39 to 1 — the engine
seat is right and D8 FAILED on this surface. Cost here was only 10.6 KB because the log was young;
on a turn-40 board it is the whole prompt.
**Wave-53 prediction.** No ability target menu with > 12 rows of the same verb+source over `#N`
handles; max non-log prompt segment stays **< 12 KB** (this corpus: 11,361).

### R143 — `{castable from exile}` loses its cause after the granting permanent leaves the battlefield: 4/17. (= ledger **D8**; engine seat **D-6**; R115's residual)
17 rows; **4** read `{castable from exile - your card, exiled by their Elite Spellbinder, which lets
you cast it from there; it costs {2} more…}` and **13** the causeless fallback. **Mechanism pinned
by this step**: `exileCastNote` (AIPlayerGPT.cpp ~12959-12987) derives the cause by scanning both
battlefields for a permanent whose `magicText` contains `canplayfromexile`; when that permanent dies
the grant survives on the exiled card (`newability[canplayfromexile forever]`) but the cause cannot
be found. Verified against the board lines: `146 vs152` seq 32 has the cause **and Elite Spellbinder
on the opponent's battlefield line**; seq 36/56/68/75/81/87/98/104 have neither. The one take
(seq 56) had no cause.
**Ask.** Stamp the cause on the card at exile time (the emitter path that grants
`canplayfromexile`), and read it back from the card instead of re-deriving it from a battlefield.
**Wave-53 prediction.** `{castable from exile}` naming the exiling source: **17/17** (was 4/17).

### R144 — an off-menu `(Pass)` parenthetical with an in-range index executes the index, with no re-ask and no note. (= ledger **D9**; engine seat **D-3**; R110's residual)
`1788320088-ai_baka_deck146-...-vs-ai_baka_deck152.jsonl` seq **91** (priority, turn 23): reply
`CHOICE: 1 (Pass)` + a PLAN describing Kaya's -3, on rows `1. +1: don't target any creature with Kaya
the Inexorable / 2. +1: target creature gets ghostform / 3. Flip Side / 4. Flip Side` plus the
standard `0. Pass priority`. Executed row 1 — seq 92's events confirm `- You used: +1: don't target
any creature with Kaya the Inexorable`. "Pass" IS the label of row 0; the lane-C/J machinery treats
it as off-menu and lets the index win. **This is the only mis-execution in 3,063 parentheticals**
(the whole residual of the class).
**Ask.** Reserve `pass` / `Pass priority` / `0` as an echo that resolves to the pass row.
**Wave-53 prediction.** A `CHOICE: n (Pass)` reply on a menu carrying `0. Pass priority` executes
the pass row **N/N**, or is re-asked; total name-vs-executed-row mismatches **<= 1 / 3,000**.

### R145 — a 900 s model timeout is invisible in stderr, indistinguishable from a refusal, and decided two opening hands. (= ledger **D10**; engine seat **D-4**, seat-123-130 L2, deck126 G8)
Three records carry `fallback: empty_reply` with `latency_ms` 900,018-900,021 and `reply: ""`:
`126 vs146` seq **1** (the opening keep, prompt **1,418** chars), `123 vs130` seq **3** (the third
mulligan ask, prompt **1,639** chars — the seat then kept 5), `130 vs162` seq **18** (a Draw-step
casting ask). stderr shows only `… -> chose -1 of N` with no timeout line and no retry; exactly ONE
record in the corpus carries a `retry` field (`125 vs152` seq 126), so the retry path exists and did
not engage. All three occurred with 21 games in flight, so the proximate cause is server queueing —
but the engine gives the reviewer no way to separate "the model refused" from "the model never
answered", and a silent heuristic mulligan is a game-shaping decision.
**Ask.** Stamp `fallback: timeout` (distinct from `empty_reply`), log
`AIPlayerGPT: no reply after Ns - heuristic`, and consider one retry before the handoff; a per-ask
deadline scaled to prompt size would have spent seconds on a 1.4 KB two-row mulligan menu instead of
15 minutes. **WAGIC_GPT_TIMEOUT=900 is the harness's setting and stays the owner's dial** — the item
is observability, not the number.
**Wave-53 prediction.** Every non-answer at the timeout wall is stamped `fallback: timeout` and
carries a stderr line: **N/N**; Baka-executed mulligans **0** (was 2).

### R146 — `[second copy:` answers a legality question where its predecessor answered a usefulness question; four dead Intruder Alarms in 14 rows. (= ledger **D11**; seat-123-130 **H2**, deck123 G1, deck126 G7; R114's residual; skill #169)
`[second copy: you already control <name>; both stay on the battlefield - no legend rule]`: **70
rows / 28 takes**, by card — Staff of Nin 7 (a second clock, right), Talisman of Impulse 5 (three
separate `{T}:Add` abilities, right), Howling Mine 3, Ranger Class 2, Chromatic Lantern 5,
Exquisite Blood 2, **Intruder Alarm 4 of 14 rows** (`123 vs125` s41 at 15 life, s59 at 6, `123 vs126`
s74 at 8, `123 vs162` s39). Intruder Alarm's primitive is `auto=lord(creature) doesnotuntap` plus
two `@movedTo(creature|...):untap all(creature)` triggers — a second copy adds nothing. Under
wave 51's `[already owned:` wording the same class was 1 in 68.
**Ask.** Keep the legality clause and append the verdict when the card's own script carries no
stacking term: `[second copy: legal, but its effect is already on the battlefield and a second copy
changes nothing]`. **The distinguishing test is the script, not the card type** — Chromatic Lantern
(`lord(land|mybattlefield) transforms(...)` PLUS five own `{T}:Add` abilities) is NOT dead and gets
deck126 G7's completion form instead (`this copy adds only its own {T}: Add`); Talisman genuinely
stacks and keeps the current wording. seat-125-126's reading of the five Lantern takes as dead casts
is a deck judgment, not a render falsehood.
**Wave-53 prediction.** `[second copy:` takes on a card whose script has no stacking term:
**0** (was 4/14 at Intruder Alarm).

### R147 — the PLAN block has no age and no provenance, and a `plan_choice_conflict` record does not say which coded line was latched. (= ledger **D12**; engine seat **D-9**, seat-146 **E-4**, deck126 G9; skill #176)
`146 vs125` seq **177-227** (turns 34-38) all render `YOUR PLAN (as you last stated it): nothing
right now: entry 1 Soul Shatter was on this menu; it did not match…` — the model's own text, first
stated at seq 163 (turn 32) and never refreshed, while the opponent went 27 -> 35 life.
`plan_echo_count` reaches **51**; 337 of 1,410 records carry an echo >= 10. Separately, `126 vs146`
seq 11 -> 12 -> 13 shows a 1,500-char PLAN asserting a FALSE board state ("Both combo pieces … are
now on the battlefield" after a Vanishing Verse exile) re-served verbatim into two later decisions,
one of which cast a redundant second Exquisite Blood at 11 life. And on the five
`plan_choice_conflict` records the `reply` field holds the FIRST answer while `choice` holds the
latched row and the re-ask's own reply is never logged (the reconciliation note above).
**Ask.** (a) stamp the plan: `YOUR PLAN (as you last stated it, N windows ago on turn T)`;
(b) log the re-ask's reply, or mark which of the two coded lines the record's `choice` came from;
(c) core prompt: the reply rules ask for a forward INTENT for the next window and say that a claim
about the current board will be re-served verbatim and should be written as a condition to re-read.
**Wave-53 prediction.** PLAN blocks rendered without an age stamp: **0/N**; decisions taken on an
echoed plan whose board claim is false: **0** (was 3 windows in one game).

### R148 — the LIFE-TO-DAMAGE CONVERTER block is not folded into the rows it prices. (= ledger **D13**; seat-123-130 **M2**, deck123 G3)
`123 vs126` seq **66** (14 life, opponent 26): the frame printed the converter block for the
opponent's Sanguine Bond; the edict row printed `{right now: they control 1 creature - Overgrown
Battlement is sacrificed, they gain 4}`; the consequence — 4 off the pilot — appeared nowhere.
Result: 14 -> 10 and 26 -> 30 in one window.
**Ask.** Where a converter is on the battlefield, continue any "they gain N" annotation with
`- and their Sanguine Bond takes N off YOU: life 14 -> 10`, the same construction lane K uses for
pain-source payments (15 clean renders). Companion: at N=1 the edict row already names the victim —
append its printed `(P/T)` and `[defender]` tag (deck123 G4, R123's family).

### R149 — the battlefield header counts creatures but not lands, and lands are the input to deck130's central gate. (= ledger **D14**; seat-123-130 **M3**, deck130 G9; skill #178)
`130 vs125` seq **61**: opponent line `Glacial Fortress; Island #1-#5 x5; Plains #1-#5 x5;
Seachrome Coast [tapped]; Staff of Nin …` — nine to twelve lands in four printed rows — under the
header `Opponent battlefield (19 permanents listed, of which 1 is a creature, …)`. Eight of nineteen
land-destruction casts went past the four-land gate (s61, s70, s75, s95, s97, s100; `130 vs126` s18;
`130 vs146` s38).
**Ask.** `(N permanents listed, of which K are creatures and L are lands)`. The creature gates
already read off exactly this shape.

### R150 — the converter block does not say which BRANCH the board is in. (deck152 G7)
`LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond` is accurate and long, and does
not say the thing every guide branches on: whether the OTHER half is present. `152 vs126` s24 (their
life 28) and s45 (32) are two windows where the pilot read the block, wrote a both-names conclusion,
and held **five** A-lines all reading `[no creature they control can block this attacker]`.
**Ask.** `… theirs - Sanguine Bond (1 of the 2 loop halves; Exquisite Blood is NOT on the
battlefield, so damage you deal them gains them nothing)`. The engine has both facts; the branch
turns on the second. Guide half: #183.

### R151 — nothing marks a draw punisher on the rows that could kill it. (= ledger **D16**; seat-123-130 **M6**, deck130 G10)
`130 vs162` s66/s71: `Cast Starstorm ... At X=4 - kills THEIRS: Fog Bank, Fate Unraveler` with the
frame's `DRAW PUNISHERS on the battlefield: theirs - Fate Unraveler` twenty lines above and no link
between them. Rule #0 held **59/59** (0 voluntary draws) and the deck still lost 0-20 to FORCED
draws off two Howling Mines.
**Ask.** Tag it inline wherever it appears as a target: `Fate Unraveler (3/4) [DRAW PUNISHER: costs
you 1 per card drawn]`, the same construction as `[defender]` / `[planeswalker]`. Guide half: #179.

### R152 — the cleanup discard's numbered list drops the duplicate markers and castability the hand line carries. (= ledger **D17**; seat-123-130 **M5**, deck130 G11; wave-50 D15)
`130 vs162` s16: the hand line reads `Talisman of Impulse (copy 1 of 2 in your hand) {2} [artifact];
… (copy 2 of 2 in your hand) …`; the numbered list beneath prints `1. Hammer of Bogardan {1}{r}{r}
(sorcery) {card text: …}` with no marker and no castability note. The seat sent a castable Hammer
and the {1} Spellbomb while four duplicates sat on the screen.
**Ask.** Carry `(copy N of M in your hand)` onto the numbered rows and append `{castable this turn}`
where the current mana covers it — both facts are already computed for the casting menu.

### R153 — the `{feeds:}` K-0 clause and the `[second copy:` tag are one decision rendered as two annotations. (= ledger **D18**; deck162 G10; R131's stronger form)
64 `{feeds:` rows at deck162, K correct on every one checked. Both of the deck's rule breaks
(`162 vs130` s11, `162 vs123` s8) were a **second** Howling Mine on a two-row ask, and the row
already knew it: it printed `[second copy: you already control Howling Mine; …]` in the same line as
`converters on your battlefield: 0`.
**Ask.** `{feeds: the opponent draws 1 extra card per turn; converters on your battlefield: 0
(nothing of yours punishes their draws yet) - and you already control one of these}`. Carry R131's
Caress qualifier (`converts only past 7 cards in hand; their hand: N`) in the same clause.

---

## LOW

### R154 — a blocker handle used in the attacker slot is dropped silently. (= ledger **D19**; engine seat **D-10**)
`126 vs162` seq **21**: reply `BLOCKS: B1:A1, B2:A1, B3:B1, B4:B1, B5:B1`, `dropped_assignments: 6`,
**no fallback and no parse_note**. The engine executed the two legal walls and discarded three
lifelink Vampires. Correct in effect; same family as R136, not re-asked or noted.

### R155 — the log's `(xN)` collapse reaches damage and repeated activations but not draws, loyalty counters or discards. (= ledger **D20**; engine seat **D-8**)
280 verbatim-repeated event lines inside single records: `- Opponent drew a card` **139**,
`- Your/Opponent's Teferi … lost a loyalty counter` 14, `- … Lolth … created a 2/1 Spider token` 12,
`- Opponent discarded Supreme Verdict` 5. The collapse already exists (`… dealt 3 damage to Intrepid
Adversary (x3)`, `Opponent used: Create vampire with Bloodline Keeper (x33)`). At a 69% mean log
share this is free prompt space.

### R156 — receipts without a payee: 3, unfixed. (= ledger **D21**; R124)
All the Intrepid Adversary pay-any-number-of-times ETB (`152 vs123` 15, `152 vs146` 16,
`152 vs162` 10). Was 7. Ask unchanged: `- Paid {1}{w} for Intrepid Adversary's ability with …`.

### R157 — the face row prices itself in one shape and not the other. (= ledger **D22**; seat-123-130 L1, deck130 G12)
Target menus read `The opponent (player, life 19)`; ability rows read `… targeting the opponent`
with no number (`130 vs125` s36, s83; `130 vs123` s11). Carry the life total onto the ability row.

### R158 — the BLOCKS format line could carry the one-per-blocker constraint. (= ledger **D23**; deck152 G8)
Two of the three `multiblock_reask` fallbacks came from a repeated B-number. The format line names
the syntax; adding *"each B-number at most once; several B-numbers may share one A-number"* costs one
clause and removes a fallback class. Efficiency, not correctness — the re-ask works when it writes
its record (R136).

### R159 — an `unparsed_reply` produces no recovery record. (= ledger **D24**; seat-146 **E-5**)
`146 vs125` s282 (turn 46, Draw): `reply: 'method4 * is_ k ind  *'`, `choice: -1`, `chosen_text`
null, two-row menu. The game continued, so something answered it; nothing in the log says what.
Three `unparsed_reply` this corpus, all degenerate decodes at 3.2-3.6 s (`125 vs146` 50 `跟 \.8`,
`125 vs152` 136 a single space) — recorded so the fallback jump is attributed to decode, not format.

### R160 — MEASUREMENT / DECISION: with the log cap in, the 30 KB tail is capped-log plus rows, and D23 is the only lever left. (= ledger **D27**; R132 continued)
165 prompts carry `(...earlier events trimmed…)`; log segment max **25,431**, p90 21,953, p50 8,452;
max NON-log segment **11,361** (`123 vs126` seq 47, a 47-row reveal). 7 prompts over 30,000, every
one 70-86% log; 330 over 25,000. Mean log share 69%. Ask volume rose **+70%** (1,493 -> 2,540 asks)
on lane I alone while the tail came DOWN (33,536 -> 31,551). Decision for the owner: a turn-bounded
or size-bounded older-turn compaction on the log side; R137's window economy is the lever on the
call-count side and ships first; re-measure with it in. **No change asked this wave.**

---

## SHIPPED SINCE THE CORPUS — recorded, NOT docketed
None. Lanes G-L and the owner batches are all IN this corpus's binary (dddb6f62b) and are
adjudicated above.

## GUIDE-ONLY — stays in guides after the render ships
- **deck123**: A52-1 (the Slip's cast-row rule moved to the refusable window; #170), A52-3 (dead
  Tutor by the `[finds only …]` form), A52-4 (second Alarm; #169), A52-5 (the dictated Damnation
  plan line), A52-6 (edict under a converter; #178 family), A52-7 (Greaves at 0 creatures), A52-8
  (cleanup duplicate order), A52-9 (the Flats is not a source at the bottom-ask), A52-10 (the
  mulligan qualifier's reach cut; #171), A52-11 (`L <>, C <>, stop <>`; #172), A52-12 (the dead-edict
  window is answered, not used).
- **deck125**: W64 (the Fruition rule re-keyed to the printed clause; #173), W66-W70 (count
  refreshes on Revelation X, the Staff/Elixir gates, cleanup order).
- **deck126**: D65/D73 (the `[second copy:` re-key; #168/#169), D66 (no redundant combo half),
  D67/D68/D74 (Exquisite Blood's trigger DIRECTION as an always-false sentence + the no-starter
  branch; #174/#175), D71 (land-drop rule with no exception), D72 (the three-or-more-land
  no-coverage mulligan sentence, odds form; #153).
- **deck130**: D52-1 (face-damage floor), D52-2 (the land gate re-keyed to the collapsed form;
  #178), D52-3 (the gang-block branch put INTO the DECIDING SITUATIONS bullet; #177), D52-4
  (Blastminer's job restated from the tags; #157/#179), D52-5 (Starstorm rule (b) defenders),
  D52-6/D52-7 (cycling conditions), D52-8 (cleanup order), D52-9 (the deck125 archetype paragraph;
  #180), D52-10 (kill the punisher; #179).
- **deck146**: 146-Q/146-R (the coverage line scoped, the colour clause cut; #171), 146-S (the empty
  blocker tag absolute ported; #182), 146-T (the ceremony gated on a printed observable; #181).
- **deck152**: 152-I (`{paying this taps:}` as the rule's trigger; #168/#184), 152-J (the one-name
  converter branch specified symmetrically; #183), 152-K (the deploy floor's `[legendary:` carve-out;
  #184), 152-L (block floor + the BLOCKS syntax sentence).
- **deck162**: 162-I (the two-row-ask empty-alternatives sentence; #185).

## MEASUREMENT items for the wave-53 reviewer briefs
1. `multiblock_reask` records: follower present, `chosen_text` present, stderr `declared blocks
   from N assignment(s)` — N/N (R136).
2. Opponent-turn windows per phase: offered / acted / seconds, and consecutive identical-declined
   run lengths per turn (R137). Report the HOLD row's take rate if it ships.
3. Every printed price clause (`[DRAW PRICE:`, `{paying this costs you`, `{feeds:`) against the
   seat's life at that record: rows at-or-above life, lethal tail present, takes (R138).
4. Edict cast rows whose `{right now:}` names a permanent already named by an unresolved stack
   object; stack-block lines with no source named (R139).
5. Targeted-spell cast rows: verdict present / absent, and takes where every legal target survives
   (R140). Report cast-row and target-row counts separately.
6. `{paying this taps:}` rows by source type (defender / non-defender) and the tail each carries
   (R141); forecast source count vs receipt source count (R108 residual).
7. Rows per ability menu by verb+source over `#N`; max non-log prompt segment (R142).
8. `{castable from exile}` rows: cause named / not, with the granting permanent's presence on a
   battlefield line at that record (R143).
9. Reply parentheticals vs executed row, by class (`name_over_index`, `echo_index_conflict`,
   `(Pass)` on a menu with a pass row) (R144).
10. Every `latency_ms >= 890000`: fallback class, stderr line, retry field, and whether the decision
    was a mulligan (R145).
11. `[second copy:` takes by card, with the card's primitive `auto=` lines quoted (R146).
12. `plan_echo_count` distribution; decisions taken on an echoed plan whose board claim is false
    (R147). And, per skill #168, the three-number literal audit for every guide-quoted string.

## PASS / KEEP — measured working, do not re-open
Lane G's livelock deferral (`stale_livelock` 0/3,300, `cast ask NOT issued` 124) · lane G's pass-row
echo (0/543 dead-edict takes) · lane G's PUT parser (0/77) · lane H's variable-source planner (0/101
forecasts name a wall twice; `- Paid {1}{g} for Overgrown Battlement with Overgrown Battlement`) ·
lane I's opponent-turn windows as a CAPABILITY (68 casts, 52 counters, 14 removal — deck125's whole
thesis, unavailable in eight prior corpora) · lane J's `name_over_index` (1/1, the named row
executed) and `plan_choice_conflict` (5 firings, 5 recovered, 3 changed the action) · lane K's
`{this hits YOUR permanent}` (179 rows, **0 takes**), `[legendary:` (19/19 clause), `[finds only …]`
(44 rows, 0 warn against the missing half, 0 takes of a `none left` row), the life-cost clause (15
rows, 0 breaks) · lane L's `becomes` re-offer (0), animated-land tap (0), fetch "makes no mana" (69),
`{spends K of M}` (546 on cast rows), the log cap (165 trims) · lane C's `x0`-is-decline (first live
firing, `repeat_count_zero_pass` 1/1) · receipts (431 unique lines, **0 `with no source tapped`**,
`turn` == narration 3,240/3,240) · land drops (deck146 33/33, deck152 30/30, deck162 27/27, deck125
98/98) · 21/21 natural, Baka-executed **0.18%**, max priority windows in one turn **7** (was 10) ·
**no render falsehood this corpus** (third wave running) — everything open is an omission, a stale
forecast against the stack, a parser scope, a cadence, or a missing record.
