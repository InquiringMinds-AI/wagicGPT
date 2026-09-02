# Wave-52 engine ledger — THE WAVE-53 DOCKET (engine / UX only)

Corpus `matchups-20260901-223430` (NINTH fair-hand corpus), binary master **dddb6f62b** (lanes
G 8c3e10e28, H d58aeba6b, I 4bdca64bf, J aa8509282, K b1fba8eff, L 0f7dff3b0 + the owner batches
bd515aad7 / c15106f4d / 4abcf5dd0 / the 2026-09-01 Vita batch / c1a0bf814 / 5ca11d6e2; PARSETEST
2121/0; gate 1210 + 44 AI/0). Pilot **qwen36-35b-a3b** (FP8) on Spark :8084 — the 122B is retired
(owner, 2026-09-02). 42 seat logs, **3,384 records**, **3,300 decisions** (ask 2,540 / priority 535
/ attackers 107 / discard 54 / blockers 41 / reveal 15 / bottom 8), **21/21 games natural — 42/42
seats carry `gameend`** (re-verified on disk by this step over every JSONL). Corpus root:
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260901-223430/`. Seat names `deckA-vs-deckB` = the deckA
seat's JSONL, written `AvB` below; `seq` is the record's own field.

**Health.** Fallbacks **15 / 3,300 = 0.45%** (wave 51: 5 / 2,129 = 0.23%) — but **9 of the 15 are
re-ask classes that did not exist in wave 51** (`plan_choice_conflict` 5, `multiblock_reask` 3, plus
`named_row_reask` 1), 8 of those 9 recovered on the next record, and **Baka executed 6 decisions
(0.18%)** against wave 51's 2 (0.09%). `giving this decision to the heuristic` **0** —
`stale_livelock` is gone. `parse_note` 20: `decision_reversed_in_prose` 5,
`plan_choice_conflict_recovered` 5, `attack_last_line_taken` 2, `blocks_last_line_taken` 2 (new),
`multiblock_reask_recovered` 2, `stale_echo_in_range` 1, `named_row_reask_recovered` 1,
`echo_index_conflict` 1, `name_over_index` 1 (new), `repeat_count_zero_pass` 1 (new).
`answer_replaced` **16**, `coded_answers > 1` **50**, `commit_retracted` **0**. stderr:
`stale_livelock` **0**, `consecutive stale drops` **0**, `dropping stale async answer` 47 (never
consecutive), `cast ask NOT issued - repeat plan in progress` **124** over 62 `repeat plan
iteration`, `auto-passing without a model call` 2,270, `land-drop ask NOT issued` 942,
`with no source tapped` **0**, `one-blocker-many-attackers reply -> re-asking once` 3,
`declared blocks from N assignment(s)` 31, `chose -1 of N` 6. Max priority windows in one turn **7**
(wave 51: 10). Max seat log **353** (`152v125`, 53 turns). Max prompt **31,551** (`146v152` seq 107,
priority, turn 27); **7 prompts > 30,000**. `- Paid` receipts **431 unique lines**, **0 with no
source**, 3 with no payee, 2 `from mana already floating`. Total inference **79,362 s = 22.0 h**.

**Mulligans (real hands).** 42 opening asks; **10 mulligan decisions taken across 8 seats** — six
seats to 6 cards, two to 5 (`146v126` bottomed Nadaar/Lolth, `123v130` bottomed both Intruder
Alarms). **No mull-to-zero and no repeated mulligan against a guide floor.** ⚠ **Two of the 42
opening decisions were NOT the model's**: `126v146` seq 1 and `123v130` seq 3 timed out at 900 s and
the heuristic decided them (D10).

**Ownership (owner doctrine 2026-08-26).** Engine/UX only below. Guide content is the reviewers';
guide-side halves are named only where a docket item retires one. **Hard caps on the model's legal
choices are rejected on sight**; every item adds a true token, a true row, a receipt or one re-ask,
and deletes nothing. Card-behaviour defects go CARD-SCRIPT FIRST. **Four window-removing proposals
were rejected this wave** and are recorded under D2 and in `synthesis-notes.md`.

---

# THE DOCKET, RANKED

## D1 — HIGH — a `multiblock_reask` can end with ZERO blocks declared, silently: no follower record, no `chosen_text`, no stderr confirmation
(= engine seat **D-1 HIGH** / `general-strategy.md` **R136**)
**Repro.** `1788320102-ai_baka_deck152-0x557298116fa0-vs-ai_baka_deck162.jsonl` seq **26** (turn 14,
blockers, 24 life). Rows: `A1. Fate Unraveler #1 (3/4) deals 3 / A2. Fate Unraveler #2 (3/4) deals 3
/ B1. Katilda (2/2) … / B2. Elite Spellbinder (6/4) [flying] - may block A1, A2 (all: you kill it,
your blocker lives) / B3. Luminarch Aspirant #3 (1/1) …`. Reply `BLOCKS: B2:A1, B2:A2`. Record:
`fallback: multiblock_reask`, `dropped_assignments: 1`, `chosen_text` **null**, `choice: 1`, and
**no follower record of any kind**. `game-152v162-1788320098.stderr:1123`
`AIPlayerGPT: one-blocker-many-attackers reply -> re-asking once` is followed immediately by
`Action added to stack: NextGamePhase. (Current phase is: Blockers)` — no
`declared blocks from N assignment(s) in one reply`, no second model call. seq 27's events:
`- Phase: Combat damage / - Opponent's Fate Unraveler dealt 3 damage to you (now 21) / - Opponent's
Fate Unraveler dealt 3 damage to you (now 18)`. Contrast the two that recovered: `152v146` seq 76 ->
77 (`multiblock_reask_recovered`, stderr 3539 -> 3556) and `126v152` seq 54 -> 55 (stderr 2354 ->
2366).
**Mechanism (not pinned to a line).** The re-ask path in the blockers seam issues the second ask but
its answer never reaches the declaration before the phase advances; the first, LEGAL assignment
(`B2:A1` — a 6/4 killing a 3/4 for free) is discarded with the illegal duplicate.
**Ask.** (a) an illegal duplicate costs the DUPLICATE, not the whole reply — keep the first legal
assignment per blocker and drop only the extras (the parser already counts them); (b) always write
the re-ask's record, including when it yields no assignment. PARSETEST: `BLOCKS: B2:A1, B2:A2` on
that A/B list -> B2 blocks A1, one dropped assignment noted. Suite pin: a blockers reply repeating a
B-number declares the first pairing.
**Wave-53 prediction.** `multiblock_reask` records with no follower and no `chosen_text`: **0/N**
(was 1/3); every one-blocker-many-attackers reply followed in stderr by `declared blocks from N
assignment(s)`: **N/N**.

## D2 — HIGH — the opponent-turn window economy: 966 windows for 85 acts, 286 of them at six phases that have never produced one, and 143 minutes spent re-asking byte-identical menus inside a single turn
(= engine seat **D-2 HIGH** / seat-146 **E-2 HIGH** / deck123 G6 / deck125 G4 / deck126 #6 /
deck152 G5 / deck162 G9 / **R137**)
**Re-derived corpus-wide by this step** (three seats used three denominators; each is right on its
own, and none is reproducible as stated): **884** `Casting decision (…, opponent's turn)` asks with
**68 casts (7.7%)** and 10,979 s, plus **82** opponent-turn `priority` windows with 17 acts —
**966 windows / 85 acts (8.8%)** against 1,001 own-turn casting asks at 37.8%. The engine seat's
"913 windows / 97 acted (10.6%)" is neither denominator; its per-phase WINDOW counts are within 3%
of mine and its per-phase zero-act rows are exact, but its prose total of 2,924 s for the dead
phases is an arithmetic slip — its own table sums to **3,124 s**.

| opponent-turn phase | windows | casts | inference s |
|---|---|---|---|
| Main phase 1 | 196 | 58 | 3,448 |
| Upkeep | 163 | 1 | 1,597 |
| Draw | 92 | 1 | 1,088 |
| End | 78 | **0** | 724 |
| Main phase 2 | 77 | 6 | 897 |
| Combat begins | 70 | 2 | 825 |
| Cleanup | 68 | **0** | 739 |
| Attackers | 67 | **0** | 761 |
| Combat ends | 67 | **0** | 763 |
| Blockers + Combat damage | 6 | **0** | 137 |

**286 windows across six phases produced zero casts and cost 3,124 s (52 min).** Repetition, two
ways: **760 of 3,300 decisions (23.0%)** sit in a run of 3+ CONSECUTIVE records with a byte-identical
option set in one turn, all declined — **143.2 min = 10.8% of the corpus's 22.0 h**, 95 runs, longest
**14** (`123v152` turn 9, seq 22-35, 699 s). Grouping non-consecutively by (turn, option set): 116
groups / 921 records / 188.2 min. Specimens: `162v126` seq **7-18** (twelve asks in one opponent
turn, `Cast Dictate of Kruphix` / `Cast nothing right now`, twelve declines, 452 s); `126v125` turns
21-37 (thirteen per turn, ten turns running); `146v125` seq 216-227.
**⚠ REJECTED REMEDIES, on the records.** A blind "identical option set already declined this turn ⇒
reuse the decline" cache would have suppressed **30 real actions corpus-wide** (the list is in
`synthesis-notes.md`); the cleanest specimen is `130v152` seq **12 -> 13**: two BYTE-IDENTICAL
`Cast Spark Spray {r} … / Cast nothing right now` menus in turn 3, declined at Main 1 and **cast at
Combat begins** — the model reads phase progression as information the menu does not carry.
deck162 G9's "one ask per opponent turn, at their end step" is a hard cap AND targets the one stop
that produced 0 of 78. deck146 G2 / deck125 G4 / deck126 #6 / seat-123-130 M1's "skip a window all of
whose cast rows carry a dead-row clause" removes a legal action from the menu (543 such rows this
corpus, 0 taken) — and D4 below shows the clause itself can be stale.
**Ask (adopted, constraint-safe).** Add a row the MODEL may take:
`Hold priority - do not ask me again this turn unless the board changes`, honoured by the engine and
re-opened on any board change, new stack object or newly affordable row. The choice stays the
model's, no window is removed, and nine dead stops become one answer. Cheaper half if the row is not
built: carry `[you declined this exact list N times already this turn]` on the row.
**Wave-53 prediction.** Opponent-turn casting windows per game **down >= 30%** with opponent-turn
casts held at **>= 90%** of 68; End / Cleanup / Attackers / Combat-ends contribute **0** casts again;
consecutive identical-declined runs of 3+ **< 10%** of decisions (was 23.0%).

## D3 — HIGH — a `[DRAW PRICE:]` total at or above the pilot's life carries no lethal clause, while lane K prints one for the same arithmetic on a mana cost — 41 rows, one take, fatal
(= seat-125-126 **HIGH #3** / deck125 G3 / **R138**)
**Repro.** `1788320083-ai_baka_deck125-0x557cd81d6290-vs-ai_baka_deck162.jsonl`, turns 18-20: **41
rows across 22 windows** carry `[DRAW PRICE: casting this draws YOU 7 cards (their Forced Fruition),
and their Underworld Dreams deals you 7]` while the seat's life is **7 or less**. seq **91**
(3 life) is the positive control — seven rows, all carrying the clause, answered `CHOICE: 7 (Cast
nothing right now)`. seq **97** (3 life, same menu, next turn) took row 1 (`Cast Essence Scatter`);
seq 98's events show seven draws and `Underworld Dreams dealt 1 damage to you (now 2)`, seq 99 is at
1, `gameend` 0/18.
**Mechanism.** `castDrawPriceRowTag` (src/AIPlayerGPT.cpp ~6797-6809) closes with `]` after the
damage total. `paymentLifeCostClause` (~3131-3153) already computes `- you would be at K` and
`- this KILLS you` for the mana path (15 renders this corpus, 0 breaks, and `- this KILLS you` has
**0 renders** only because no life-cost window reached it).
**Ask.** Extend the draw-price tag with the same tail:
`[DRAW PRICE: … deals you 7 - you would be at -4; this KILLS you]`. PARSETEST: the exact seq-97 row
at 3 life; a negative at 20 life (no lethal tail); the echo shape unchanged.
**Wave-53 prediction.** `[DRAW PRICE:]` rows whose damage >= the seat's life carrying the lethal
tail: **N/N** (was 0/41); casts taken from such a row: **0** (was 1).

## D4 — HIGH — the edict forecast is computed from the battlefield and ignores a sacrifice ALREADY ON THE STACK aimed at the same permanent: three Soul Shatters, one Emrakul, one death
(= seat-146 **E-1 HIGH** / deck146 G1 / **R139**)
**Repro.** `1788320094-ai_baka_deck146-0x55c97c0102d0-vs-ai_baka_deck125.jsonl` seq **327 / 328 /
329** (turn 50, Main phase 1, 9 life vs 48). Each row: `Cast Soul Shatter {2}{b} {right now: they
sacrifice Emrakul, the Aeons Torn (MV 15, their highest)} {leaves N of your M …}`; each taken.
**Mechanism, PINNED on disk by this step** (both readings the seat offered are wrong): seq 328's
events read `- Paid {2}{b} for Soul Shatter … / - You cast Soul Shatter (that Soul Shatter was 1 of
3 copies in your hand …) / - Your Soul Shatter resolved and went to your graveyard / - Opponent
used: sacrifice a creature or planeswalker with Soul Shatter targeting Emrakul, the Aeons Torn` —
the GRANTED ability is put on the stack and has **not resolved**, so the opponent's battlefield line
truthfully still lists Emrakul and the cast row is offered again. seq 328's stack block reads
`1 (top): ability: sacrifice a creature or planeswalker [triggered/activated ability]`; seq 329's
reads the same line **twice**. Only at seq **330** do the events carry
`- Opponent's Emrakul, the Aeons Torn died`, once. **NOT protection** — Emrakul is
`abilities=nofizzle,flying,protectionfromcoloredspells` (mtg.txt:35731) and Soul Shatter is
`auto=ability$!… sacrifice notaTarget(*[creature;planeswalker;manacost=convertedcost:highest:
creature^planeswalker:myBattlefield]|myBattlefield)!$ opponent` (borderline.txt:106068), a
non-targeting edict; wave 51 already recorded Emrakul sacrificed correctly. **NOT coalescing** —
three abilities were created and one found a permanent. **Wave-51 lane A is NOT reopened**: no
sacrifice failed to happen.
**Ask.** Two true tokens; no window removed. (a) The stack block names the ability's SOURCE and
victim: `ability: Soul Shatter's sacrifice (aimed at Emrakul, the Aeons Torn) [from your Soul
Shatter]` instead of an anonymous effect name — the emitter already has the ability's owner and its
`notaTarget` pick. (b) The `{right now: they sacrifice <name> (MV N, their highest)}` clause
(src/AIPlayerGPT.cpp:1146) continues `- a sacrifice is already on the stack aimed at this permanent;
this one would find their next-highest` when an unresolved stack object in this window already names
it. Either alone stops casts #2 and #3 (6 mana and 2 cards at 9 life).
**Wave-53 prediction.** Edict cast rows whose `{right now:}` names a permanent already named by an
unresolved stack object, without a such-clause: **0/N** (was 2/3 in one window); stack-block lines
reading a bare `ability: <effect>` with no source: **0/N**.

## D5 — HIGH — a targeted spell's CAST row carries no kill summary: the refusable window has less information than the forced ask that follows
(= seat-123-130 **H3** / engine seat **D-7** / deck123 G2 / **R140**; the other half of wave-52 D15)
**Repro.** `1788320077-ai_baka_deck123-…-vs-ai_baka_deck146.jsonl` seq **18** (turn 10, 6 life):
`1. Cast Tragic Slip {b} {right now: -1/-1 (no creature has died this turn, so Morbid does NOT
apply)} {leaves 0 of your 1 untapped mana source untapped - casting this taps you out} - legal
targets right now: Nadaar, Selfless Paladin, Triumphant Adventurer, Goblin` — taken; seq **19**'s
target ask printed `{right now: -1/-1 - SURVIVES (toughness N, -1/-1 more kills it)}` on **all
three** rows above the sentence "this ask has no pass row". Corpus-wide: **85 `Cast Tragic Slip`
rows carry no verdict; 9 target rows carry one.** Damage cast rows have the same asymmetry, and the
`wolf of werewolf fights` two-step (`152v146` seq 49 -> 50 -> 51) carries no fight forecast on either
half. Working templates already exist: deck130's `Cast Starstorm` X-pricing clause (0 wasted casts
in 60+ windows) and `123v125` seq 19's `- the only legal targets are YOUR OWN right now:`.
**Ask.** Append the summary the target rows already compute to the CAST row:
`{kills 0 of the 3 legal targets at -1/-1}` / `{kills: Elite Spellbinder}`. PARSETEST: a -1/-1 cast
row with three toughness-2+ targets -> `kills 0 of the 3`; a positive with one legal kill; the echo
shape of the new clause.
**Wave-53 prediction.** Targeted-spell cast rows carrying a per-target verdict or a kill count:
**N/N** (was 0/85); removal spells cast where every legal target survives: **0** (was 1).

## D6 — MED — `{paying this taps: <defender> - it cannot attack this turn}` states a cost of zero for a wall and hides the only cost that exists
(= seat-125-126 **HIGH #1** / deck126 G6 / **R141**)
**Repro.** `1788320110-ai_baka_deck126-0x557974bba060-vs-ai_baka_deck130.jsonl` seq **13** (turn 9,
14 life, Rorix Bladewing 6/5 flying on their line): `Cast Overgrown Battlement {1}{g} … {paying this
taps: Overgrown Battlement - it cannot attack this turn}`, taken; 14 -> 8 on the next combat. Also
`126v125` seq **86**. **Counts re-derived**: 101 `{paying this taps:` rows corpus-wide, **91 name a
defender**, and **22 of those carry the "cannot attack this turn" tail** (the seat's "91 such rows"
is the defender-naming denominator; the tail's own count is 22).
**Mechanism.** `paymentTapsClause` (src/AIPlayerGPT.cpp ~3108-3120) appends
`" - " << (one ? "it" : "they") << " cannot attack this turn"` whenever the caller sets the flag,
with no test on the source's abilities. `Overgrown Battlement` is `abilities=defender`
(mtg.txt:83596) — as are all sixteen creatures in deck126.
**Ask.** When the tapped source has `defender`, print `- it cannot block on their turn`; with
neither restriction keep the current words; when a row taps several sources with different
restrictions, name each with its own consequence. Suite/PARSETEST pin: a cast row tapping one
Overgrown Battlement prints "cannot block on their turn" and never "cannot attack this turn".

## D7 — MED — lane L's `#a-#b` collapse does not reach ABILITY target menus: 40 rows, one distinct shape
(= engine seat **D-5** / **R142**; wave-52 D8's residual)
**Repro.** `1788320108-ai_baka_deck123-…-vs-ai_baka_deck162.jsonl` seq **41** (40 rows, **2**
distinct shapes after masking `#N`) and seq **42** (39 rows, **1** shape), turn 12, priority. Every
row: `Equip with Lightning Greaves targeting Human #N [your battlefield] (Lightning Greaves is
ALREADY attached to Thraben Doomsayer - this MOVES it to Human #N, and Thraben Doomsayer loses what
it grants) [repeat: activated this turn 1 times already …] {card text: "Equipped creature has haste
and shroud…"}`. Primitive: `Lightning Greaves … auto={0}:equip` (mtg.txt:67192) — the equip is free,
so the menu is offered at every priority window while the tokens exist. The same collapse already
works on cast-row target clauses (`Overgrown Battlement #1-#3 {1}{g} (creature 0/4) [defender] x3`,
97 uses) and battlefield lines (`Vampire #1-#33 (4/4) … x33`). These are the only two windows in the
corpus with > 12 rows collapsing to <= 2 shapes; every other big menu (40-row attacker lists, 50-row
library reveals) is genuinely distinct.
**Ask.** One option row per identical-target group with `#a-#b` and `xN`, parser still accepting any
handle — the same treatment the target clause has.
**Wave-53 prediction.** No ability target menu with > 12 rows of the same verb+source over `#N`;
max non-log prompt segment stays **< 12 KB** (this corpus: 11,361).

## D8 — MED — `{castable from exile}` loses its cause when the granting permanent leaves the battlefield: 4/17
(= engine seat **D-6** / **R143**; wave-52 D11's residual)
**Repro.** `1788320088-ai_baka_deck146-…-vs-ai_baka_deck152.jsonl` seq **32** reads
`{castable from exile - your card, exiled by their Elite Spellbinder, which lets you cast it from
there; it costs {2} more…}`; seq **36, 56, 68, 75, 81, 87, 98, 104** (same card, same exile) read
the causeless `{castable from exile - a legal cast, not a dead row; it costs {2} more than printed,
already counted in the cost shown}`. The one take (seq 56) had no cause.
**Mechanism, PINNED by this step.** `exileCastNote` (src/AIPlayerGPT.cpp ~12959-12987) derives the
cause by scanning BOTH battlefields for a non-token permanent whose `magicText` contains
`canplayfromexile`. Checked against the board lines at each of those records: at seq 32 Elite
Spellbinder is on the opponent's battlefield line; at seq 36 onward it is **not**. The grant survives
on the exiled card (`newability[canplayfromexile forever]`); the cause does not.
**Ask.** Stamp the cause on the card when the grant is applied (the emitter path that adds
`canplayfromexile`) and read it back from the card, so the clause is independent of the granting
permanent's survival. PARSETEST: the clause with the cause after the source has left play.
**Wave-53 prediction.** `{castable from exile}` naming the exiling source: **17/17** (was 4/17).

## D9 — MED — an off-menu `(Pass)` parenthetical with an in-range index executes the index, with no re-ask and no note
(= engine seat **D-3** / **R144**; wave-52 D6's residual)
**Repro.** `1788320088-ai_baka_deck146-…-vs-ai_baka_deck152.jsonl` seq **91** (priority, turn 23):
reply `CHOICE: 1 (Pass)` + `PLAN: Activate Kaya's -3 to exile Tovolar's Packleader…` on rows
`1. +1: don't target any creature with Kaya the Inexorable … / 2. +1: target creature gets ghostform
… / 3. Flip Side … / 4. Flip Side …` plus the standard `0. Pass priority`. Executed row 1 — seq 92's
events confirm `- You used: +1: don't target any creature with Kaya the Inexorable`. "Pass" IS the
label of row 0, which IS on the menu; the lane-C/J machinery treats it as off-menu and lets the index
win. This is the **only** mis-execution in 3,063 `CHOICE: n (name)` parentheticals — the whole
residual of the class.
**Ask.** Reserve `pass` / `Pass priority` / `0` as an echo that resolves to the pass row (the same
short-label rule lane G applied to `Cast nothing right now`). PARSETEST: the exact reply on that
menu -> row 0.
**Wave-53 prediction.** `CHOICE: n (Pass)` on a menu carrying `0. Pass priority` executes the pass
row **N/N** or is re-asked; total name-vs-executed mismatches **<= 1 / 3,000**.

## D10 — MED — a 900 s model timeout is invisible in stderr, indistinguishable from a refusal, and decided two opening hands
(= engine seat **D-4** / seat-123-130 L2 / deck126 G8 / **R145**)
**Repro.** Three records carry `fallback: empty_reply`, `reply: ""`, `latency_ms` 900,018-900,021:
`126v146` seq **1** (the opening keep; prompt **1,418** chars, a two-row menu), `123v130` seq **3**
(the third mulligan ask; prompt **1,639** chars — the seat then bottomed 2 and kept 5), `130v162`
seq **18** (a Draw-step casting ask, prompt 6,330). stderr shows only `… -> chose -1 of N`, with no
timeout line and no retry. Exactly ONE record in the corpus carries a `retry` field (`125v152` seq
126, `retry: 1`), so the path exists and did not engage. All three occurred with 21 games in flight,
so the proximate cause is server queueing at `-j 21`, not the engine.
**Ask.** (a) stamp `fallback: timeout` distinct from `empty_reply`; (b) log
`AIPlayerGPT: no reply after Ns - heuristic`; (c) one retry before the handoff; (d) consider a
per-ask deadline scaled to prompt size, so a 1.4 KB two-row mulligan menu cannot consume 15 minutes.
**`WAGIC_GPT_TIMEOUT=900` is the harness's dial and stays the owner's** — the docket item is
observability, not the number.
**Wave-53 prediction.** Non-answers at the wall stamped `fallback: timeout` with a stderr line:
**N/N**; Baka-executed mulligans **0** (was 2).

## D11 — MED — `[second copy:` answers a legality question where `[already owned:` answered a usefulness question: four dead Intruder Alarms in 14 rows
(= seat-123-130 **H2** / deck123 G1 / deck126 G7 / **R146**; wave-52 D10's residual)
**Repro.** 70 `[second copy: you already control <name>; both stay on the battlefield - no legend
rule]` rows, **28 takes**. By card: Staff of Nin 7, Chromatic Lantern 5, Talisman of Impulse 5,
**Intruder Alarm 4** (of 14 rows: `123v125` s41 at 15 life, s59 at 6, `123v126` s74 at 8, `123v162`
s39), Howling Mine 3, Exquisite Blood 2, Ranger Class 2. Under wave 51's `[already owned:` wording
the dead-Alarm class was 1 in 68.
**Card facts (verified with `/usr/bin/grep` against `bin/Res/sets/primitives/`).** Intruder Alarm
(mtg.txt:58850) is `auto=lord(creature) doesnotuntap` plus two `@movedTo(creature|…):untap
all(creature)` triggers — a second copy adds nothing. Talisman of Impulse (mtg.txt:117529) has three
separate `{T}:Add` abilities — it genuinely stacks. **Chromatic Lantern (mtg.txt:19630) carries the
non-stacking `lord(land|mybattlefield) transforms(…)` AND five own `{T}:Add{G/R/U/B/W}` abilities —
a second Lantern IS a mana source and is NOT a dead cast**; seat-125-126's reading of its five takes
as a rule failure is a deck judgment, not a render falsehood.
**Ask.** Keep the legality clause and append the verdict when the card's own script carries no
stacking term: `[second copy: legal, but its effect is already on the battlefield and a second copy
changes nothing]`. Where only PART of the card is redundant, print deck126 G7's completion form
(`both stay, but the "lands you control have…" effect is already on - this copy adds only its own
{T}: Add`). **The distinguishing test is the script, not the card type.**
**Wave-53 prediction.** `[second copy:` takes on a card whose script has no stacking term:
**0** (was 4/14 at Intruder Alarm); Staff / Talisman / Ranger Class takes unchanged.

## D12 — MED — the PLAN block has no age, and a `plan_choice_conflict` record does not say which coded line was latched
(= engine seat **D-9** / seat-146 **E-4** / deck126 G9 / **R147**)
**Repro (staleness).** `146v125` seq **177-227** (turns 34-38) render the same
`YOUR PLAN (as you last stated it): nothing right now: entry 1 Soul Shatter was on this menu…`,
first stated at seq 163 (turn 32) and never refreshed, while the opponent went 27 -> 35 life;
`plan_echo_count` reaches **51**, and 337 of 1,410 records carry an echo >= 10.
**Repro (false board claim).** `126v146` seq **11 -> 12 -> 13**: a 1,500-char PLAN ending "Both combo
pieces (Exquisite Blood and Sanguine Bond) are now on the battlefield" (the Bond had been exiled by
Vanishing Verse at t11) re-served verbatim into two later decisions; the seq-12 decision cast a
redundant second Exquisite Blood at 11 life over a live Tribute row.
**Repro (telemetry).** On the five `plan_choice_conflict` records the `reply` field holds the FIRST
answer, `choice` holds the row that was latched (three of them carry `answer_replaced: True` and
`coded_answers: 2`), `chosen_text` is null, and the re-ask's own reply is never logged.
**Ask.** (a) `YOUR PLAN (as you last stated it, N windows ago on turn T)`; (b) log the re-ask's
reply or mark which coded line the record's `choice` came from; (c) core prompt: the reply rules ask
for a forward INTENT and say a board claim will be re-served verbatim into later windows.
**Wave-53 prediction.** PLAN blocks with no age stamp: **0/N**; decisions taken on an echoed plan
whose board claim is false: **0** (was 3 windows in one game).

## D13 — MED — the LIFE-TO-DAMAGE CONVERTER block is not folded into the rows it prices
(= seat-123-130 **M2** / deck123 G3 / **R148**) `123v126` seq **66** (14 life, opponent 26): the
frame printed the converter block for the opponent's Sanguine Bond and the edict row printed
`{right now: they control 1 creature - Overgrown Battlement is sacrificed, they gain 4}`; the
consequence — 4 off the pilot — appeared nowhere. 14 -> 10 and 26 -> 30 in one window. Ask: continue
any "they gain N" annotation with `- and their Sanguine Bond takes N off YOU: life 14 -> 10`, the
construction lane K already uses for pain-source payments. Companion (deck123 G4, wave-52 D24's
family): at N=1 the edict row names the victim — append its printed `(P/T)` and `[defender]` tag.

## D14 — MED — the battlefield header counts creatures but not lands
(= seat-123-130 **M3** / deck130 G9 / **R149**) `130v125` seq **61**: `Glacial Fortress; Island
#1-#5 x5; Plains #1-#5 x5; Seachrome Coast [tapped]; Staff of Nin …` — nine to twelve lands in four
printed rows — under `Opponent battlefield (19 permanents listed, of which 1 is a creature, …)`.
Eight of nineteen land-destruction casts went past deck130's four-land gate (s61, s70, s75, s95,
s97, s100; `130v126` s18; `130v146` s38). Ask: `(N permanents listed, of which K are creatures and
L are lands)`.

## D15 — MED — the converter block does not say which BRANCH the board is in
(= deck152 G7 / **R150**) `152v126` s24 (their life 28) and s45 (32): the pilot read
`LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond`, wrote a both-names
conclusion, and held **five** A-lines all reading `[no creature they control can block this
attacker]`. Ask: `… theirs - Sanguine Bond (1 of the 2 loop halves; Exquisite Blood is NOT on the
battlefield, so damage you deal them gains them nothing)`. The engine has both facts.

## D16 — MED — nothing marks a draw punisher on the rows that could kill it
(= seat-123-130 **M6** / deck130 G10 / **R151**) `130v162` s66/s71: `Cast Starstorm … At X=4 - kills
THEIRS: Fog Bank, Fate Unraveler` with `DRAW PUNISHERS on the battlefield: theirs - Fate Unraveler`
twenty lines above and no link. Rule #0 held **59/59** and the deck still lost 0-20 to FORCED draws.
Ask: `Fate Unraveler (3/4) [DRAW PUNISHER: costs you 1 per card drawn]` wherever it is a target, the
same construction as `[defender]` / `[planeswalker]`.

## D17 — MED — the cleanup discard's numbered list drops the duplicate markers and castability the hand line carries
(= seat-123-130 **M5** / deck130 G11 / **R152**; wave-52 D25's D15 carry) `130v162` s16: hand line
`Talisman of Impulse (copy 1 of 2 in your hand) {2} [artifact]; … (copy 2 of 2 in your hand) …`;
numbered list `1. Hammer of Bogardan {1}{r}{r} (sorcery) {card text: …}` with no marker. Sent a
castable Hammer and the {1} Spellbomb, kept four duplicates. Ask: carry `(copy N of M in your hand)`
onto the numbered rows and append `{castable this turn}` where the current mana covers it.

## D18 — MED — the `{feeds:}` K-0 clause and the `[second copy:` tag are one decision rendered as two annotations
(= deck162 G10 / **R153**; wave-52 D26's Caress carry) 64 `{feeds:` rows at deck162, K correct
everywhere checked. Both of the deck's rule breaks (`162v130` s11, `162v123` s8) were a **second**
Howling Mine on a two-row ask whose row already printed `[second copy: you already control Howling
Mine; …]` beside `converters on your battlefield: 0`. Ask: `{feeds: … converters on your
battlefield: 0 (nothing of yours punishes their draws yet) - and you already control one of these}`,
carrying the Caress qualifier (`converts only past 7 cards in hand; their hand: N`) in the same
clause.

## D19 — LOW — a blocker handle used in the attacker slot is dropped silently
(= engine seat **D-10** / **R154**) `126v162` seq **21**: reply `BLOCKS: B1:A1, B2:A1, B3:B1, B4:B1,
B5:B1`, `dropped_assignments: 6`, **no fallback and no parse_note**; the engine executed the two
legal walls and discarded three lifelink Vampires. Correct in effect; same family as D1.

## D20 — LOW — the log's `(xN)` collapse does not reach draws, loyalty counters or discards
(= engine seat **D-8** / **R155**) 280 verbatim-repeated event lines inside single records:
`- Opponent drew a card` **139**, `… lost a loyalty counter` 14, `… created a 2/1 Spider token` 12,
`- Opponent discarded Supreme Verdict` 5. The collapse exists for damage and repeated activations.
At a 69% mean log share this is free prompt space.

## D21 — LOW — receipts without a payee: 3 (was 7)
(= engine seat **D-11** / **R156**) All the Intrepid Adversary pay-any-number-of-times ETB
(`152v123` 15, `152v146` 16, `152v162` 10). Ask: `- Paid {1}{w} for Intrepid Adversary's ability
with …`.

## D22 — LOW — the face row prices itself in one shape and not the other
(= seat-123-130 L1 / deck130 G12 / **R157**) Target menus read `The opponent (player, life 19)`;
ability rows read `… targeting the opponent` with no number (`130v125` s36, s83; `130v123` s11).

## D23 — LOW — the BLOCKS format line could carry the one-per-blocker constraint
(= deck152 G8 / **R158**) Two of three `multiblock_reask` fallbacks came from a repeated B-number.
Adding *"each B-number at most once; several B-numbers may share one A-number"* to the format line
costs one clause. Efficiency only — D1 is the correctness half.

## D24 — LOW — an `unparsed_reply` produces no recovery record
(= seat-146 **E-5** / **R159**) `146v125` s282: `reply: 'method4 * is_ k ind  *'`, `choice: -1`,
`chosen_text` null, two-row menu; the game continued and nothing says what answered. All three
`unparsed_reply` this corpus are degenerate decodes at 3.2-3.6 s (`125v146` 50 `跟 \.8`, `125v152`
136 a single space) — decode, not prompt format.

## D25 — LOW — CARRIED, re-measured not re-derived
Wave-52 D18 (floating-pool `{leaves N}` — 2 legitimate `from mana already floating` receipts, no
window), D20 (`#N` on duplicate assignment/reveal rows — no new window), D21 (pass replies stamped
with name notes — 1 `stale_echo_in_range`, was 2), D22 (X-menu exit — the failing shape did NOT
recur: deck130 reached 3 X menus, marker taken 2/2, `Cast Card Normally` on a nothing-dies row 0,
`X = 0` 0; still gated on the engine's cancel-semantics ruling), D24 (edict victim tags — 3
named-victim Tributes, none a `[defender]`, all taken in their first window), D26's remaining LOW
renders (self-life-loss `{right now: you go to N}` — 0 failing windows; chooser-attributed
narration; the off-menu `[RE-ASK]`'s unpayable pip; the X-menu header's life at or below the largest
attacking power — 0 windows), plus the wave-52 D25 set. The WALL CHECK summary row (wave-51 R128)
stays LOW: the third failure (`130v146` s35) was the DECIDING SITUATIONS bullet dropping a branch,
not missing arithmetic — the guide fix (#177, D52-3) ships first.

## D26 — LOW — new LOW renders, one line each
An activation row whose `{paying this taps:}` names a creature says the row is offered again after
combat (deck152 G6; `152v162` s27, a Katilda activation in Upkeep that tapped three Aspirants out of
the following combat) · a `plan_arithmetic_conflict` re-ask when a PLAN line carries both `= <k>` and
a differently-valued `stop <j>` for the same quantity (deck123 G5; `123v162` s33, "Stop creating
humans at M=40 (L+C+3 = 20+2+3=25)") · one projected mana line (`Next turn, with a land drop, this
becomes N+1 sources`) to stop the model recomputing a projection by hand — the three longest replies
at deck123/deck130 are all that shape (deck123 G7) · an X-cost row that names its OUTCOME as well as
its price (`{right now: returns 0 cards - your graveyard holds no creature cards at mana value <= X}`;
deck146 G3, the Agadeem's Awakening rows, 8 offered / 8 correctly declined but only because a guide
paragraph did the work) · a Sorin Vampire token rendered `[lifelink, doesn't untap during its
controller's untap step]` when `planeswalkers.txt:3057` creates `token(Vampire,Creature
Vampire,1/1,black,lifelink)` — cosmetic, cause not investigated (seat-125-126 LOW #8, `126v123` s63;
Intruder Alarm was on the opposing board, which is the likely source).

## D27 — MEASUREMENT / DECISION — the log window, with the cap in
(= engine seat watch / seat-146 E-6 / **R160**; wave-52 D23 continued) 165 prompts carry
`(...earlier events trimmed…)`; log segment max **25,431**, p90 21,953, p50 8,452; max NON-log
segment **11,361** (`123v126` seq 47, a 47-row reveal). 7 prompts over 30,000, every one 70-86% log;
330 over 25,000; mean log share **69%**. Ask volume rose **+70%** on lane I alone while the tail came
DOWN (33,536 -> 31,551). The only remaining lever on the log side is a turn-bounded or size-bounded
older-turn compaction — **the owner's decision**; D2's window economy is the lever on the call-count
side and ships first. **No change asked this wave; re-measure with D2 in.**

## D28 — MED (frontend / human seat) — CARRIED unchanged: Arena-style grouping + the O(n^2) RenderSpell scan — OWNER ITEM
This corpus produced a 34-Vampire board (`123v126`, `126v123` seq 63 with 34 flying blockers), the
40-row equip menus of D7 and 50-row library reveals. The human-seat pile problem remains reachable.

---

# DISCHARGED — SET B: the wave-52 docket D1-D27, by the emitter's actual string

| item | lane / commit | verdict | counts |
|---|---|---|---|
| **D1** livelock inside a repeat loop | G 8c3e10e28 | **SHIPPED — PASS** | `stale_livelock` **0** / 3,384 records and 0 in stderr; `consecutive stale drops` 0; `cast ask NOT issued - repeat plan in progress` **124** over 62 `repeat plan iteration`; Baka-executed non-pass rows 0 |
| **D2** pass-row echo vs annotations | G | **SHIPPED — PASS 0/543** | 543 rows render `at 0 this does nothing`, **0 taken**; no reply naming "nothing" executed an edict row |
| **D3** PUT parser executed a different row | G | **SHIPPED — PASS 0/77** | 77 reveal/discard/bottom records; 0 replies carried both `PUT:` and `CHOICE:`; deck162 3/3, deck123 8/8, deck125 37/37 executed the printed row |
| **D4** planner over-taps variable sources | H d58aeba6b | **SHIPPED — PASS on the forecast (0/101); ONE residual receipt** | 0 of 101 `{paying this taps:` clauses name a Battlement twice (was 2/2); the correct form renders (`- Paid {1}{g} for Overgrown Battlement with Overgrown Battlement`). Residual: `126v125` s86 forecasts ONE wall, s87's receipt names TWO — carried into D6's suite pin |
| **D5** opponent-turn instants | I 4bdca64bf | **SHIPPED — PASS, the wave's biggest capability change** | **884** opponent-turn casting asks (was 0) + 82 opponent-turn priority windows; **68 casts** (52 counters, 14 removal, 2 other); the false "priority again later this turn" sentence **0** on opponent-turn windows. **The COST is the new D2** |
| **D6** index/name conflict executes the index | J aa8509282 | **SHIPPED — PASS 1/1** | `126v152` seq 45 `echo_index_conflict;name_over_index`, the NAMED row executed; the wave-51 shape 0/N. Residual: the `(Pass)` echo — **D9** |
| **D7** pain-source lethal clause | K b1fba8eff | **SHIPPED — PASS 0/N; the LETHAL tail UNTESTED** | 15 rows carry `{paying this costs you N life (Talisman of Impulse: 1 damage) - you would be at 19}`; 1 taken at 20 life; `- this KILLS you` **0 renders** (no window reached it). The same arithmetic on a DRAW price is untagged — **D3** |
| **D8** per-token row collapse | L 0f7dff3b0 | **SHIPPED — SPLIT: PASS on cast-row target clauses (97) and battlefield lines; FAIL on ability menus** | `123v162` seq 41 (40 rows / 2 shapes), seq 42 (39 / 1) — **D7** |
| **D9** self-hit tag on exile/loyalty rows | K | **SHIPPED — PASS 179 rows / 0 takes** | `-3: exile non-land permanent` **15 tagged** (was 74 untagged), `-2: destroy … draw two` 8, `-6:` 1, damage 155; wave-51's Kaya self-exile has no successor |
| **D10** `[already owned:` three forms | K | **SHIPPED — PASS on all three** | `[already owned:` **0 renders**; `[legendary:` 19/19 carry the legend clause (4 taken, all refreshes); `[second copy:` 70 rows / 28 takes; tutor `[finds only …]` 44 rows / 4 variants / **0 warn against the missing half** / 0 takes of a `none left` row. Residual: the usefulness verdict — **D11** |
| **D11** `[from exile]` cause and tax | K | **SHIPPED — PARTIAL 4/17** | the cause is present exactly while the granting permanent is on a battlefield line — **D8** |
| **D12** `[repeat:` `becomes` re-offer | L | **SHIPPED — PASS 0** | 138 `becomes` rows, 2 takes, no same-window re-offer; `[repeat:` rows at deck146's Hive **0 offered** (was 9 offered / 3 taken) |
| **D13** animated land tapped for mana | L | **SHIPPED — PASS 0** | every receipt cross-checked against the same turn's animation narration; rows carry `[this land is TAPPED: animated, it still cannot attack this turn]` |
| **D14** `plan_choice_conflict` scope | J | **SHIPPED — PASS: 5 firings, 5 recovered, 3 CHANGED the executed action** | `130v125` s48 -> s49 and s66 -> s67 (a 5-mana Hammer return reverted to pass), `146v125` s59 -> s60 (a Lolth `+0` reverted to pass); `130v125` s82 -> s83 and `146v125` s133 -> s134 confirmed the row. Three of the five carry `answer_replaced: True` + `coded_answers: 2`. `plan_missing` **UNTESTED** (0 renders) |
| **D15** -N/-N DIES/SURVIVES | L | **SHIPPED — SPLIT: PASS 9/9 on target rows; NOT APPLIED on cast rows 0/85** | **D5** |
| **D16** fetch rows say "makes no mana" | L | **SHIPPED — PASS 69 renders** | deck123 passed-while-a-spell-needed-the-source **0** (was 6) |
| **D17** `{spends K of M}` on cast rows | L | **SHIPPED — PASS 546** | 736 rows carry `{spends `, 546 on `Cast …` rows, 126 on ability rows, 64 on `becomes` rows; `{leaves K of M}` 2,390 |
| **D18** floating-pool `{leaves N}` | not shipped | **CARRIED — 0 failing window** | 2 receipts read `from mana already floating` (legitimate) |
| **D19** receipts without a payee | not shipped | **CARRIED — narrower: 3** (was 7) | **D21** |
| **D20** `#N` on duplicate rows | not shipped | **CARRIED — no new window** | **D25** |
| **D21** pass replies stamped with name notes | not shipped | **CARRIED — 1** (was 2) | **D25** |
| **D22** X-menu exit | not shipped | **CARRIED — the failing shape did NOT recur** | deck130: 3 X menus, marker taken 2/2, `Cast Card Normally` on a nothing-dies row 0, `X = 0` 0. Still gated on the cancel ruling |
| **D23** log measurement | n/a | **RE-MEASURED** | **D27** |
| **D24** edict victim tags | not shipped | **CARRIED — no failing window** | 3 named-victim Tributes, none a `[defender]`, all first-window takes |
| **D25** carried-LOW set | n/a | **RE-MEASURED** | **D25** above |
| **D26** LOW renders | partly superseded | the Caress `{feeds:}` K is promoted into **D18**; the rest carried | **D26** |
| **D27** frontend / human seat | owner item | **CARRIED unchanged** | **D28** |
| carry: 21/21 natural, no crash / timeout adjudication | | **PASS** | 42/42 `gameend`; results.tsv has a winner for all 21 |
| carry: Baka-executed <= 0.31% | | **PASS 0.18%** | 6 / 3,300 |
| carry: max priority windows per turn <= 10 | | **PASS 7** | |
| carry: no prompt > 30,000 | | **FAIL 7** — every one 70-86% log | **D27** |
| owner batch bd515aad7 (per-half instant timing) | | **UNTESTED** | no adventure / split / aftermath / morph card in the seven-deck pool (deck lists checked) |
| owner batch c15106f4d (Calamity Bearer, Drake Hatcher, morph pricing) | | **UNTESTED** | neither card in the pool |
| owner batch 4abcf5dd0 (phase survives one tick) | | **UNTESTED as stated; no regression** | no Delver-class upkeep reveal in the pool; Staff of Nin upkeep draws fire normally across a 69-turn game |
| Vita batch (equip registration, evolve, counters) | | **equip PASS (0 growth); evolve UNTESTED** | the Greaves menus track the creature count exactly (40 targets -> 40 rows, 39 -> 39), one row per target, no per-cycle growth; no evolve / Hapatra / Wilt-Leaf / Hurkyl / Pridemate card in the pool |
| c1a0bf814 option-preserving autotap (HUMAN seat only) | | **PASS 0 leaks** | 10/10 AI-seat forecasts are a subset of the receipt that followed; `126v125` s86's mismatch is D4's residual, not a preserveOptions leak |
| 5ca11d6e2 Baka pump targeting | | **UNTESTED** | Baka executed 6 decisions (2 mulligans, 4 casting); no ability-target decision reached the heuristic |

# DISCHARGED — SET A: wave-51 guide edits + boundary pass, by deck (reviewer verdicts, summarised)

| deck | verdict summary | routed |
|---|---|---|
| **123** (2-4) | P1 Flats **PASS** (20 offers, 6 takes, 0 passed-while-needed; was 6); P3 plan half PASS 0/1, cast half **FAIL** (the one `[from exile]` row was entry 0 and passed over); P4 Damnation plan-text **0/2 FAIL**, entry-0 **0/1 FAIL** (third corpus); P5 Slip **FAIL 1** (three `- SURVIVES` rows, and the guide's "no verdict is printed" sentence was the defect); P6 dead Tutor **FAIL 1**, spare-first PASS 2/2; P9 own-turn chains **PASS 2/2**, both won; P10 opponent-turn instants **PASS 3**; P12 `SURVIVES` ability takes **PASS 0**; P15 `[your battlefield]` **PASS 0/40**; second Alarm **4** | D5, D11, D25 |
| **125** (5-1) | W58 Revelation from a low-X row **PASS 0/18**; W59 Staff gate **FAIL 4/11**, Elixir **FAIL 2/11**, both unpunished because lane I kept re-opening the window; W60 target order **PASS 5/5**; W61 **UNTESTED**; W62 Emrakul **PASS 3/3**; W63 ceilings **PASS** (land drop 98/98, sweeper at header 0 **220/220**, `X = 3` 11/11, Lightmine 1/102 at the 3+ branch, cleanup 1/37); the single loss is the `[DRAW PRICE:]` window | **D3** |
| **126** (1-5) | D58 Tribute **PASS 0+0+0** (206 `at 0 this does nothing` rows, 0 cast); D59 Lantern half-1 PASS, **half-2 FAIL 5/5 — the literal was retired underneath the rule**; D60 Tutor **PASS 0 + 0/6**; D61 **PASS 3/3 + UNTESTED**; D62 **PASS 0 + 0**; D63 mulligan — the one no-coverage seven was never answered (900 s); D64 `{paying this taps:}` **PASS 0/91** with one receipt residual | D6, D11, D10, D4-residual |
| **130** (3-3) | #0 draw punishers **PASS 59/59**; #1 cycling 11/13; #2 face damage **FAIL 6** above the floor, `SURVIVES` takes **PASS 0**, `{this hits YOUR permanent}` **PASS 0/40**; X menu marker 2/2; land plan **FAIL 8** past the four-land gate; Blastminer **PASS 0 self-targets**; Talisman life **PASS 0** at life <= 3 (120 clause renders); blockers 1/2 (the gang-block loss) | D14, D16, D17, D22 |
| **146** (4-2) | P1 mulligan **FAIL 2/4** (the coverage line read as a ship at 2-3 lands); P2 Kaya **PASS 0/15**; P3 Silencer **PASS 0/4 + 4/4**; P9 `[repeat:` **PASS — 0 rows offered at all**; empty-tag A-lines 38/39; cast-order misroutes **0/21**; the show-the-work ceremony **20/84** | D12 (closed), skill #171/#181/#182 |
| **152** (1-5) | P4 Katilda A-row **PASS 0/1**; P5 Spellbinder **UNTESTED**; P6 Lair `[Upkeep offer:` **UNTESTED** (0 rows at this seat); `{paying this taps:}` **FAIL 3/10** (two Main-1 casts and an Upkeep Katilda activation that tapped three Aspirants); deploy floor 10 flagged, **9 correct declines** on `[legendary:` rows; empty-tag A-lines 14/19; `BLOCKS: none` at N<=9 **2**; 2 illegal multi-assignments (1 recovered, 1 = **D1**) | **D1**, D26, skill #183/#184 |
| **162** (5-1) | RULE 1 punisher-first **20/21**; RULE 2 vs `{feeds:}` K **FAIL 2** (both second Mines from a two-row ask) with 13/13 correct at K >= 1 and two named-exception casts the reply cited by name; P8 PUT **PASS 3/3**; walls 6/6; `{this hits YOUR permanent}` **0/8**; blocks 0 breaks; cleanup discards 3/3 on the printed row | **D18**, skill #185/#186 |
| **pool** | wave-51 boundary B1-B8: B3's re-key obligation was **NOT done at deploy** for deck123 and deck126 (the carry-list caught it and both were re-keyed in the wave-52 revision) — the cost is measured: deck126's Lantern rule went 9/9 -> 0/5; B4/B5/B6/B7/B8 carried with no failing window; B2's `{feeds:}` exemptions held | skill **#168** |

---

# CARRIED — open, re-measured, not re-derived
See D19-D28. The wave-52 OPEN questions: "does the repeat row get taken" — 11 takes, `x0` executed
as a pass for the first time (`123v162` s35), no ask record between a take and its receipt; "the
~800 s ceiling" — replaced by a hard 900 s wall hit three times (D10); the floating-mana class —
0 unexplained floats, 2 legitimate `from mana already floating` receipts; the double-tap — 0
over-taps in 101 forecasts, one receipt residual (D4/D6).

---

# FALLBACK CLASSES — 15 / 3,300 = 0.45%; Baka-executed 6 = 0.18%

| n | class | records | disposition |
|---|---|---|---|
| 5 | `plan_choice_conflict` (lane J's D14, **working as designed**) | `146v125` 59, 133; `130v125` 48, 66, 82 | **5/5 recovered**; 3 CHANGED the executed action, 2 confirmed the row. Telemetry gap -> **D12**. The proposal to require a named row is **REJECTED** |
| 3 | `empty_reply` — the 900 s timeout wall | `126v146` 1 (opening keep), `123v130` 3 (third mulligan), `130v162` 18 | **Baka executed all 3**; two are opening-hand decisions -> **D10** |
| 3 | `unparsed_reply` — degenerate decode (3.2-3.6 s) | `125v146` 50 (`跟 \.8`), `146v125` 282 (`method4 * is_ k ind  *`), `125v152` 136 (a single space) | **Baka executed all 3**; no prompt-format cause; no recovery record -> **D24** |
| 3 | `multiblock_reask` — one blocker on several attackers | `152v146` 76 (recovered), `126v152` 54 (recovered), **`152v162` 26 (NO follower, zero blocks declared)** | **D1**; format-line half **D23** |
| 1 | `named_row_reask` — in-range index + off-menu name | `123v126` 70 -> recovered at 71 | lane C working |
| 0 | `stale_livelock` (was 1) | | lane G paid |
| 0 | `stale_echo` executed / garbage decode / `CHOICE: 0` on a no-pass ask | | holding |
| — | **not a fallback but executed wrong**: `146v152` seq 91 (`CHOICE: 1 (Pass)` executed a Kaya `+1`) | | **D9** — the only mis-execution in 3,063 parentheticals |

---

# PROMPT CHARS PER DECISION KIND — wave 51 -> wave 52 (ALL games, 21/21)

| kind | n51 | mean51 | max51 | n52 | mean52 | p90-52 | max52 | p90 lat 51 -> 52 (s) | max lat 52 |
|---|---|---|---|---|---|---|---|---|---|
| ask | 1,493 | 10,418 | 29,355 | **2,540** | 12,594 | 24,638 | 29,581 | 56.8 -> **44.0** | 900.0 |
| priority | 445 | 14,302 | 33,536 | 535 | 14,935 | 25,937 | **31,551** | 59.3 -> **53.3** | 393.0 |
| attackers | 110 | 11,324 | 26,911 | 107 | 12,497 | 21,735 | 30,277 | 62.9 -> **61.0** | 382.1 |
| blockers | 31 | 10,374 | 20,779 | 41 | 14,350 | 23,317 | 30,307 | 79.1 -> 66.7 | 377.7 |
| discard | 31 | 12,506 | 27,450 | 54 | 16,579 | 26,421 | 28,071 | 123.0 -> **41.8** | 57.7 |
| reveal | 13 | 14,544 | 19,789 | 15 | 15,559 | 22,196 | 26,658 | 43.2 -> 63.6 | 205.4 |
| bottom | 6 | 2,266 | 2,469 | 8 | 2,458 | 2,590 | 2,667 | 70.2 -> 97.0 | 97.5 |

Ask volume **+70%** (lane I's opponent-turn windows are essentially all of it). Means up 6-33% but
the **tail came down**: max prompt 33,536 -> 31,551 with 7 over 30 KB out of twice as many asks; 330
prompts exceed 25,000. Mean log share **69%**; the log window is hard-capped (165 trims, log segment
max 25,431). The remaining tail is capped log + 5-6 KB of rows — D27's compaction question is the
only lever left on that side, and it is the owner's.

---

# LATEGAME SPECIMENS — invariant 0 (for the owner's per-wave prompt review)

**Harvest default, KEPT as the exhibit** (already written to `wave52/lategame-specimen.txt`):
`1788320112-ai_baka_deck125-0x55ded94e0310-vs-ai_baka_deck130.jsonl` seq **149**, turn **70**, Main
phase 1, **22,643 chars**, 4 rows — the deepest turn in the corpus, and it carries three of this
wave's new forms in one screen: `[second copy: you already control Staff of Nin; both stay on the
battlefield - no legend rule]`, `{X pricing: max affordable X=12 (15 mana total); each point of X
gains you 1 life and draws you 1 card}`, and the self-hit form on a sweeper (`{right now: destroys
0 of their creatures …, 1 of yours}`).
**Render-dense companion** (the engine seat's second candidate, recommended alongside):
`1788320091-ai_baka_deck123-0x55b67b2f87a0-vs-ai_baka_deck126.jsonl` seq **88**, turn 26, Main phase
1, 15,761 chars, 6 rows, the seat at **5 life vs 29** with 34 creatures — the `#1-#33 x33`
battlefield collapse, the `LIFE-TO-DAMAGE CONVERTER` block, `{effect (each copy of this card does
this): …}`, `[finds only an enchantment card - …]`, the Morbid clause, `{target text:}` and
`{card text:}`: eight annotation families at a size a human can read.
**The size ceiling, if the owner wants it:** `1788320088-ai_baka_deck146-0x5560cfe861d0-vs-ai_baka_
deck152.jsonl` seq **107** (31,551 chars, turn 27, priority, 79% log) — the corpus maximum and the
exhibit for D27.
**The defect specimens** (not for the prompt review; for the lanes):
`146v125` seq **326-330** (D4, the three Soul Shatters and the anonymous stack lines) ·
`152v162` seq **26-27** (D1, the silent zero-block) · `125v162` seq **91 vs 97** (D3, the same menu
at the same life, declined then taken) · `123v162` seq **41-42** (D7, the 40-row equip fan-out) ·
`130v152` seq **12-13** (D2's counter-evidence: two byte-identical menus, declined then cast).
