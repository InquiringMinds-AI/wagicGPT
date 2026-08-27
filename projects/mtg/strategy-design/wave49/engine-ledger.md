# Wave-49 engine ledger — THE WAVE-50 DOCKET (engine / UX only)

Corpus `matchups-20260827-094106` (sixth fair-hand corpus), binary master **7e94544f1** (owner
Vita fixes 19e2b04f7 + 6819183f7; lanes R fa1fddf02, U 2d6b16d8f, T a2fd9684a, S 63e456fdf;
PARSETEST 1783/0; suite 1139/0 + 37/0). 42 seat logs, **1,999 records**, **1,915 decisions**,
**21/21 games natural — 42/42 seats carry `gameend`** (verified on disk by this step:
`grep -l '"kind": "gameend"'` = 42/42, including `1787841709-ai_baka_deck123-0x563fceb01d00-vs-ai_baka_deck162.jsonl`
and its opposite, deck162 won 22-0 at turn-field 16). The `seat-146-152-162.md` E-1 item
("excluded deck123-vs-deck162, no gameend, orchestrator killed after ~9h", pointer 0x55e867ceaa40)
describes the wave-47 run `matchups-20260826-182155` and is **DROPPED as a stale-note artifact**;
its compound-loop concern is recorded under CARRIED with no record in this corpus. Corpus root:
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260827-094106/`. Seat names `deckA-vs-deckB` = the
deckA seat's JSONL; `seq` is the record's own field. NOTE (D23): the JSONL `turn` field is the
narration's turn minus one; seat files quote the field.

**Health.** Baka-executed fallbacks **4 / 1,915 = 0.21%** (`unparsed_reply` 3 — two garbage
decodes + the worked-example parse miss; `stale_echo` 1); 6 / 1,915 = 0.31% counting the two NEW
one-shot re-asks (`repeat_count_reask` 1, `named_row_reask` 1), both recovered. `commit_retracted`
0. `answer_replaced` **10** (all audited; 9 genuine re-answers, 1 executed a REJECTION — D7).
`parse_note`: `repeat_count_under_two` 3, `attack_last_line_taken` 3, `named_row_not_offered` 3
(0 executed), `echo_index_conflict` 2 (+1 ambiguous), `multi_answer_first_taken` 2,
`blocks_last_line_taken` 1. `repeat_count_missing` **0** (was 4). Explicit `CHOICE: 0` passes on
priority **205**. Max prompt **34,333** (one record; next 28,487). Max priority windows in one
turn **16** (was 9). `mana_only_windows_skipped` max **265** (was 835). Latency max 727 s (a
7,582-char reply on a 6 KB prompt — reasoning, not prompt size).

**Ownership (owner doctrine 2026-08-26).** Engine/UX only below. Guide content is the
reviewers'; the guide-side halves are named only where a docket item retires one. Hard caps on
the model's legal choices are rejected; every item adds a true token, a true row, a receipt or
one re-ask, and deletes nothing.

---

# THE DOCKET, RANKED

## D1 — HIGH — casts that get RESPONDED TO pay twice: a regression of the wave-48 binary — **IN FLIGHT (lane V, `~/Projects/wagicGPT-w50-v`)**
(= seat-engine **D-4 HIGH** / `general-strategy.md` **R48**)
**Repro.** `1787841675-ai_baka_deck146-0x5560568482a0-vs-ai_baka_deck125.jsonl` seq **32**: `Cast
Silverquill Silencer {b}{w}` with 6 sources -> next window `Mana available: 2 total ... Those
sources: Swamp #1 {b}; Plains #1 {w} | Already in pool: {b}{w} (2 mana ALREADY produced and
floating right now ...)`, battlefield `Plains #2 [tapped]; Brightclimb Pathway [tapped]; Hive of
the Eye Tyrant [tapped]; Swamp #2 [tapped]` — 4 sources for a 2-pip spell. Also seq 39/49/76 of
the same file; `1787841668-ai_baka_deck162-0x561eb7164be0-vs-ai_baka_deck125.jsonl` seq **33**
(`Cast Howling Mine {2}`, 7 sources -> 3 untapped + `{u}{u}` floating, all four blue sources gone
with Cancel on the stack); `1787841678-ai_baka_deck123-0x561c789917c0-vs-ai_baka_deck125.jsonl`
seq **46**, **69** (`Idyllic Tutor {2}{w}`, 6 tapped); `1787841701-ai_baka_deck125-0x562af3b37450-vs-ai_baka_deck126.jsonl`
seq **118** (`Elixir {1}`, 2 tapped). **12 of 234 non-X casts tapped exactly 2x their pips; 11 of
the 12 were answered on the stack** (`- You cast X / - Opponent cast Cancel|Essence Scatter|Fall
of the Gavel`); 13 other answered casts did not double-tap. Wave 48: 7/7 the same shape
(`146 vs125` seq 47 Silverquill Command 8 tapped, seq 53 Kaya 10 tapped). Wave 47
(`matchups-20260826-182155`): **0**. Began with the wave-48 binary eccf8284c (E1 + lanes
P/O/N/Q), not lane T.
**Ask.** Trace whether the cast's `AIAction` (or the auto-tap via the cast click) is applied a
second time when the interrupted priority round re-enters the GPT seat after the opponent's
response; fix so a cast pays once. D18's receipt makes the fix visible by string.
**Locus.** the cast path in `AIPlayerGPT.cpp` / `AIPlayer` action queue re-entry after an
opponent response; mechanism NOT traced by any seat (none edits src/); lane V owns the trace.
**Wave-50 prediction.** Casts with `tapped == 2 x pips` by the source-count diff: **0/N** (was
12/234, 7/7); floating mana after a countered cast with no X/ability spend: 0.

## D2 — HIGH — Tribute to Hunger resolved with NO sacrifice and NO life gain: a stale reveal zone was consulted as the choice set
(= seat-125-126 **HIGH #1** / **R50**)
**Repro.** `1787841695-ai_baka_deck126-0x55ab47d01810-vs-ai_baka_deck146.jsonl` seq **13** (t9 main
2, life 24): `Cast Tribute to Hunger {2}{b} {right now: they control 2 creatures - they choose
which one}` taken; opponent controls Nadaar, Selfless Paladin (3/3), a Goblin token (1/1). Seq 14
events: `You cast Tribute to Hunger / Your Tribute to Hunger resolved ... / Opponent used: Gain
life equal to its toughness with Tribute to Hunger / Your revealed Savannah went to your hand /
Your revealed Savannah went to your hand` — no `died`, no `You gained`. Opposite seat
`1787841695-ai_baka_deck146-0x55ab469c8240-vs-ai_baka_deck126.jsonl` seq **15** events: **`You
revealed 2 cards but none was a legal target - took none / You used: Gain life equal to its
toughness with Tribute to Hunger / Opponent put the revealed Savannah into their hand x2`**. One
turn earlier that seat resolved Pelakka Predation (`reveal:type:*:opponenthand
revealzone(opponenthand) optionone ... target(*[manacost>=3]|reveal) reject ... optiontwo ...
all(*|reveal) moveto(hand)`) taking Staff of Nin; the two revealed Savannahs stayed in the reveal
zone. Oracle: "Target opponent sacrifices a creature. You gain life equal to that creature's
toughness."
**Ask.** (inference, not stepped) the `notaTarget(creature|mybattlefield)` sacrifice chooser shares
the reveal/choice machinery and Pelakka's option-two `moveto(hand)` did not empty `revealzone` at
resolution — the put-back ran only when the next chooser fired. Empty the reveal zone at the
revealing ability's resolution; assert the edict chooser's candidate set is the target's
battlefield. Suite pin: opponent resolves any `revealzone(opponenthand)` reveal; next turn the
revealed player casts Tribute to Hunger / Devour Flesh / any `sacrifice!$ targetedplayer` edict;
expect one creature sacrificed and the gain event.
**Locus.** the reveal-zone lifecycle (`revealzone` / `moveto(hand)` in the ability resolver) and
the `notaTarget` chooser; verify with the /wagic-cardscript loop (card-script first, engine only
if the primitive is faithful and the engine still misbehaves).
**Wave-50 prediction.** Edict resolutions at N ≥ 1 with no sacrifice event: **0/N** (was 1);
"revealed N cards but none was a legal target" at an edict resolution: 0; the pin passes.

## D3 — HIGH — the Devour Flesh edict row prints `you gain N`; the TARGETED player gains (the corpus's one render falsehood)
(= seat-123-130 **H1** / **R51**)
**Repro.** `1787841689-ai_baka_deck123-0x556ee08ba8f0-vs-ai_baka_deck146.jsonl` seq **6**: `Cast
Devour Flesh {right now: they control 1 creature - Silverquill Silencer is sacrificed, you gain
2}`; `1787841709-ai_baka_deck123-0x563fceb01d00-vs-ai_baka_deck162.jsonl` seq **8**: `... Master
of the Feast is sacrificed, you gain 5` -> reply plan "remove Master of the Feast and gain 5
life"; narration: opponent 20 -> 25; `1787841678-ai_baka_deck123-0x561c789917c0-vs-ai_baka_deck125.jsonl`
seq **96**: `... Emrakul, the Aeons Torn is sacrificed, you gain 15` -> plan "gain 15 life (now
17)". 10 such rows at the pair, believed twice. Primitive: `Devour Flesh: auto=...
dynamicability<!mytgt toughnesslifegain targetcontroller!> ... targetedplayer; text=Target
player sacrifices a creature, then gains life equal to that creature's toughness.`
**Ask.** Lane T's edict emitter reads the gain recipient from the primitive (`targetcontroller`
vs the caster) and prints `- the opponent gains N` when the target is the opponent; `you gain N`
only when the recipient is the caster (Tribute to Hunger; a self-targeted Devour). The guide
already states the truth (deck123:199-204) and correctly does not teach doubt of the row.
**Locus.** lane T's `{right now: they control N creatures ...}` emitter (a2fd9684a).
**Wave-50 prediction.** Edict rows whose gain recipient disagrees with the primitive: **0/N**;
replies planning "gain N" on a Devour Flesh at the opponent: 0 (was 2).

## D4 — HIGH — the cleanup-step discard is chosen by the engine and never routed to the seat
(= seat-123-130 **H2** / deck123 S4 -> skill #127 / **R52**)
**Repro.** `1787841683-ai_baka_deck123-0x55afd8ccf160-vs-ai_baka_deck152.jsonl` seq **4** (t3, `Your
hand (7 cards)`): `Cast Vision Skeins`; seq **5** narration: `You drew Thraben Doomsayer / You
drew ... / Phase: --- / You discarded Intruder Alarm` — no decision record between seq 4 and
seq 5. The engine discarded the deck's only combo enchantment; the game was lost at t12.
**Ask.** Route the hand-size discard to the seat as an ask (`Discard down to 7: choose N cards`),
the London-bottoming ask's shape (kind `bottom` or a new `discard`), with the hand rows and
`{card text:}`. Guide-side: A49-8's hand-size check stays correct; deck123:569 "the engine
discards for you at end of turn" goes false when this ships (boundary B1).
**Locus.** the cleanup-step discard for AI players (`AIPlayer`/`AIPlayerGPT` where Baka's
`chooseDiscard` equivalent runs); the GPT seat currently inherits the heuristic.
**Wave-50 prediction.** `You discarded <card>` lines with no preceding discard-kind decision
record: **0/N**; the ask renders with the hand rows (report N).

## D5 — HIGH — `legal targets right now:` on a cast row enumerates every token: the last uncollapsed enumeration (34,333-char prompt)
(= seat-engine **D-1 HIGH** / seat-125-126 **HIGH #2** / deck125 G1 / **R49**)
**Repro.** `1787841678-ai_baka_deck125-0x561c77675570-vs-ai_baka_deck123.jsonl` seq **13** (ask,
Main 1, turn-field 10): **34,333 chars**; `3. Cast Path to Exile {w} {leaves 5 of your 6 untapped
mana sources untapped} - legal targets right now: Lord of Lineage (creature 5/5) [...] {target
text: "..."}, Vampire #1 (creature 4/4) [flying, doesn't untap during its controller's untap
step], Vampire #2 (...), ..., Vampire #322 (...) {card text: ...}` = **28,755 chars, 323 handles,
ONE distinct body**, while the battlefield line reads `Vampire #1-#322 (4/4) (printed 2/2)
[flying, ...] x322`. 25 rows pool-wide carry ≥ 10 targets (`Stone Rain` / `Lay Waste` vs 30 lands,
`1787841687-ai_baka_deck130-0x558429a816d0-vs-ai_baka_deck125.jsonl` seq 115-125, 1.2 KB each).
**Ask.** Reuse the battlefield/A-row range grammar on the target clause for consecutive same-name
same-body handles (`Vampire #1-#322 (creature 4/4) [...] x322`); cap distinct entries with `(+N
more like these)`. The lane-O equivalence key already exists.
**Locus.** the cast-row `legal targets right now:` clause emitter (R3's numbered target-MENU
collapse shipped; the inline clause did not get it).
**Wave-50 prediction.** No prompt > 30,000 chars; no single option row > 3,000 chars (was
28,755).

## D6 — HIGH — target rows VANISH between main 1 and main 2, and self-only target windows are offered every priority
(= seat-123-130 **H3** / deck130 G1 -> skill #124 / **R53**)
**Repro (drop).** `1787841699-ai_baka_deck130-0x562d3ed7c8d0-vs-ai_baka_deck146.jsonl` seq **14**
(t5 main 1, priority): five Pyrite Spellbomb rows (targeting Dwarven Blastminer `[your
battlefield]`, Nadaar, the opponent, you, Draw 1) -> pass; seq 15 `Cast nothing`; seq **16**
(main 2, priority): ONE action row `1. Deal 2 damage with Pyrite Spellbomb targeting Dwarven
Blastminer [your battlefield] ... {if you pass here, this option is not offered again this
turn}` + `0. Pass priority`; reply `CHOICE: 1`; narration `Your Dwarven Blastminer died`. No
board change between 14 and 16.
**Repro (self-only).** `1787841670-ai_baka_deck130-0x55a0f8e4de00-vs-ai_baka_deck126.jsonl` seq
**34, 38, 42, 45-48**: with no opposing nonbasic, every window lists `Destroy with Dwarven
Blastminer targeting Forgotten Cave #1/#2 [your battlefield]` as its only Blastminer rows; seq 38
took one.
**Ask.** (a) root-cause the seq 14 -> 16 drop (seat's suspicion, marked inference: the
"already declined this turn" dedup keyed on something the self-target row escaped); (b) on a
damage/destroy row whose target's controller is the acting player append `{this hits YOUR
permanent}`; on a window where every action row carries it print `Every action here targets
your own permanent; 0 (pass) is the usual answer.` above the rows. Legality unconstrained.
**Locus.** the per-turn declined-option filter on priority windows; the target-row builder.
**Wave-50 prediction.** Main-2 target sets that are a strict subset of main 1's with no board
change: **0**; self-target rows taken: 0 (was 2); self-only windows carry the line (report N).

## D7 — MED — the "last CLEAN coded line" rule executes a line the reply wrote to REJECT (and skips one ending in a period)
(= seat-engine **D-2 HIGH** / seat-123-130 **M3** / **R54**)
**Repro.** `1787841678-ai_baka_deck123-0x561c789917c0-vs-ai_baka_deck125.jsonl` seq **94**: first
line `CHOICE: 3 (Cast Thraben Doomsayer)`; last coded line `CHOICE: 1 (Cast Lightning Greaves) is
wrong because we need to cast Doomsayer first`; reasoning ends "So Choice 3 is correct";
`chosen_text` = Lightning Greaves, `answer_replaced: true`; next log: Greaves cast, Cancelled;
Doomsayer stayed in hand at 2 life vs 57. Same file seq **12**: `CHOICE: 2 (... x19)` then
`CHOICE: 2 (... x20).` -> 19 executed. 1/10 replacements this corpus was a rejection.
**Ask.** A coded line is CLEAN only if nothing but whitespace or one terminal `.` follows its
closing parenthesis; `is wrong` / `would` / `instead` / `not` after `)` disqualify it; on
disqualification take the earlier clean line; count `rejected_line_skipped`. PARSETEST: the seq
94 reply (expect 3), the seq 12 reply (expect x20), a negative with `).` alone.
**Locus.** lane S's clean-line scanner (63e456fdf).
**Wave-50 prediction.** `answer_replaced` whose executed line has trailing prose after `)`:
**0/N**; a trailing `.` alone never disqualifies.

## D8 — MED — the prompt's own worked example `CHOICE: 1 (Goblin (1/1))` is unparseable
(= seat-engine **D-3 HIGH** / **R55**)
**Repro.** `1787841672-ai_baka_deck123-0x55cfc41f41b0-vs-ai_baka_deck130.jsonl` seq **31**
(damage-order ask; rows `1. Goblin (1/1) [...]`); format line `e.g. "CHOICE: 1 (Goblin (1/1))"`;
reply `CHOICE: 1 (Goblin (1/1))` -> `unparsed_reply`; the trailing `CHOICE: 0 (Pass)` executed
(`answer_replaced: true`, choice 0 on a 3-option ask with no pass row).
**Ask.** Strip `(P/T)` from the example's short name, or parse a balanced nested parenthetical.
PARSETEST: the exact example string on a row whose name carries `(P/T)`.
**Wave-50 prediction.** `unparsed_reply` on a reply equal to the prompt's own worked example:
**0/N**.

## D9 — MED — `CHOICE: 0` on an ask with NO pass row falls back without a re-ask
(= seat-123-130 **M4** / deck123 G4 / **R57**)
**Repro.** the D8 record: index 0 on a 3-row damage-order ask went straight to `unparsed_reply`
(129 s) with a valid first line above it; lane S's index-past-menu `[RE-ASK]` did not fire for 0.
**Ask.** Treat 0 on a no-pass ask as index-past-menu: one `[RE-ASK] this ask has no pass -
answer with a number from 1 to N`; print `(this ask has no pass row)` in mandatory asks' format
line. With D7, the valid first line would have been taken anyway; both.
**Wave-50 prediction.** `CHOICE: 0` on a no-pass ask reaching Baka without a `[RE-ASK]`: **0/N**.

## D10 — MED — plan-carry expiry keys on CONTENT, never on a raw echo count; a verdict-opening plan is not carried
(= seat-engine **D-5** / seat-123-130 **M2** / seat-146-152-162 **E-3** / deck123 G3 / deck130 G4 / **R56**)
**Repros.** `1787841701-ai_baka_deck125-0x562af3b37450-vs-ai_baka_deck126.jsonl` seq **69-96**: 28
identical carries; the model re-stated the plan in only 5/27 replies, so the 5-echo counter
(reset by a restatement) never fired — and the plan names an action ("ping their face"), so it
correctly carries. `1787841678-...deck123-...-vs-...deck125.jsonl` seq **53-56** and
`1787841687-...deck130-...-vs-...deck125.jsonl` seq **125**: `The game is lost. ...` carried (the
text goes on to name cards, so "names no action" cannot fire). **Counter-instances — the echo is
CORRECT:** `1787841705-ai_baka_deck162-0x564372dbcfd0-vs-ai_baka_deck146.jsonl` seq **2-10** (the
opening plan, harmless) and `1787841693-ai_baka_deck152-0x5614ff716010-vs-ai_baka_deck126.jsonl`
s31-41 (the loop-lockout plan "... ATTACK: none", the wave-48 loss's fix); a raw-count trigger
would have expired both. Neighbour (deck130 G4): both self-hits (D6) carried a plan naming a
target the window did not list.
**Ask.** (i) do not carry a plan that opens with a verdict (`The game is lost`, `Nothing to do`)
or names no card and no option verb on the CURRENT menu; (ii) print `(your plan's target is not
on this window)` under a carried plan whose named target matches no row; (iii) retire the
"5 identical echoes" trigger — keep the count as a report field only.
**Locus.** lane U's carried-plan block (2d6b16d8f).
**Wave-50 prediction.** `The game is lost` carries **0** (was 5); wrongful expiries of a
correct action-naming plan: 0; the target-absent line renders on every such window (report N).

## D11 — MED — the repeat row carries the current creature count and a placeholder example; the pilot overshoots its own stop with the count on a different row
(= deck123 G1/G2 / seat-123-130 L2 / **R61**; guide half = deck123 A49-1, skill #120)
**Repro.** `1787841678-...deck123-...-vs-...deck125.jsonl` seq **16-22**: x1, x1, x2, x25, x50,
x120, x100 with the `[repeat: activated this turn N times already; you control 223 creatures
...]` tag on the SINGLE row and the repeat row's only number its example `x50`; 8 of 11 takes at
the pair past the guide's stop (own-upkeep loop; 322 Vampires, then a Verdict). 16 priority
windows in that turn (carry prediction ≤ 10 FAILED).
**Ask.** The repeat row prints the creature count in the battlefield line's own words (`you
control M creatures` — computed already for the tag) and the example as `x<N>`; the `[repeat:]`
tag leads with the creature count. No cap; the pilot names N. The `PLAN: stop at M = ...` shape
is guide-side and stays correct after this ships.
**Wave-50 prediction.** Every repeat row prints the count and `x<N>`; takes of exactly the old
example's 50 with no arithmetic in the reply: 0 (was 1); max priority windows in a turn ≤ 10.

## D12 — MED — the mana-only gate leaks a single `Add N green mana with Overgrown Battlement` row, three times with an EMPTY stack
(= seat-125-126 **MED #3** / **R58**)
**Repro.** `1787841701-ai_baka_deck126-0x562af4e53780-vs-ai_baka_deck125.jsonl` seq **66** (Cancel on
Sorin on the stack), **68 (empty stack)**, 75 (Dream Fracture on Lantern), **77 (empty)**, 85
(Cancel on Blood), **87 (empty)**: each `priority`, Main 1, options_text = that one row + pass,
14-16 KB, all `pass`; `mana_only_windows_skipped: 265` in the same game. Floating mana `Already
in pool: {g}{b}{b}{w}` on the seq 66 line (after a countered cast — D1's residue) may defeat the
"nothing but mana" test. Not traced.
**Wave-50 prediction.** All-mana single-row windows reaching the model: **0** (was 6).

## D13 — MED — the sweeper row's `(K able to attack)` is 0 for summoning-sick / tapped creatures as well as walls
(= seat-125-126 **MED #4** / deck125 G2 / **R59**)
**Repro.** `1787841678-...deck125-...-vs-...deck123.jsonl` seq 13 `destroys 323 of their creatures
(0 able to attack)` (323 fresh tokens); `1787841701-...deck125-...-vs-...deck126.jsonl` seq **60**
`exiles 5 of their creatures (0 able to attack)` (five walls; taken at 35 life with no clock).
**Ask.** The row carries the header's "without a restriction against attacking" count and the
reason: `(323 able to attack next turn)` / `(all 5 defenders)`.
**Wave-50 prediction.** The row's K agrees with the header's count; sweeper casts on a row
reading `all N defenders` with no clock on the caster: 0.

## D14 — MED — a "choose a name" effect never renders the chosen name
(= seat-123-130 **M1** / **R60**)
**Repro.** `1787841689-ai_baka_deck123-0x556ee08ba8f0-vs-ai_baka_deck146.jsonl` seq **8**:
narration `Opponent used: Choose a name with Silverquill Silencer`, battlefield `Silverquill
Silencer {b}{w} (3/2)` with no name, each cast an unattributed `You lost 3 life`; the naming seat
`1787841689-ai_baka_deck146-0x556edf581350-vs-ai_baka_deck123.jsonl` seq 8 chose `Vision
Skeins`. The pilot cast the named card at 15, 4 and 1 life (6 of its 20).
**Ask.** `[named: Vision Skeins]` on the permanent's line; `- named Vision Skeins` in the
narration; the life-loss line attributed to the source.
**Wave-50 prediction.** Named-card casts with `[named: <that card>]` on the prompt: 0/N;
unattributed `You lost N life` lines: 0.

## D15 — MED — the draw-engine cast row carries the punisher count (R39 PROMOTED: deck162's guide lane is exhausted)
(= deck162 general / seat-146-152-162 P10 / **R39**)
**Repro.** `1787841668-ai_baka_deck162-0x561eb7164be0-vs-ai_baka_deck125.jsonl` seq **8** (Master
of the Feast with 0 punishers, opp `0 creatures`; PLAN "stack ... once a punisher is on the
battlefield"), seq 10 (Dictate); `1787841709-ai_baka_deck162-0x563fcd7e5300-vs-ai_baka_deck123.jsonl`
seq 8. Breaks 3/3/6/3 across four corpora, all in wins; the #102 plan-text remedy failed; skill
#102's exhaustion clause routes here.
**Ask.** `{feeds: the opponent draws N extra card(s) per turn; punishers on your battlefield:
K}` on `Cast Master of the Feast / Howling Mine / Dictate of Kruphix / Teferi's Puzzle Box /
Forced Fruition` rows — the count the engine computes for the opposing `DRAW PUNISHERS` line.
**Wave-50 prediction.** Engine casts at `punishers: 0` with opp `0 creatures`: **0** (was 3).

## D16 — LOW — `DRAW FORECAST` renders only for the punished seat
(= seat-engine **D-6** / **R62**)
98/98 `DRAW PUNISHERS ... theirs` prompts carry it; 0/63 `...: yours` do
(`1787841668-...deck162-...-vs-...deck125.jsonl` seq 13). Mirror: `their next draw step draws K
cards = K x N life to you`.

## D17 — LOW — `(text omitted)` marker missing on the `Choose an option for <MDFC>:` menu's `Cast Card Normally` row
(= seat-engine **D-7** / seat-146-152-162 E-4 / **R63**)
177 marked, 6 bare: `1787841675-...deck146-...-vs-...deck125.jsonl` seq 6, 88;
`1787841677-ai_baka_deck146-0x561bcc4b92c0-vs-ai_baka_deck152.jsonl` seq 39, 52; `deck146-vs-deck126`
seq 11; `deck146-vs-deck130` seq 10 (Pelakka Predation / Emeria's Call). A second emitter path.
**Wave-50 prediction.** Bare `// <name>` endings 0/N (was 6).

## D18 — LOW — `- Paid {cost} with <sources>` receipt: makes lane T adjudicable and D1 visible
(= seat-engine **D-9** / **R64**)
No line names which sources a payment tapped. `1787841668-...deck162-...-vs-...deck125.jsonl` seq
**26 -> 27** (`Cast Fog Bank {1}{u}`: sources `Drowned Catacomb #1 {u} or {b}; Island #1; Island
#2; Drowned Catacomb #2 {u} or {b}` -> after: `Drowned Catacomb #1; Island #1` — the generic paid
from a dual with an Island untapped, against colour-count ASC) is one sample. Emit one line per
payment from `selectAutoTapProducers`. Cheap; ship with D1.
**Wave-50 prediction.** One `- Paid` line per cast (two = D1 recurring); generic pips paid from a
multi-colour source while a mono source could pay: 0/N (report).

## D19 — LOW — degenerate-decode counter (R20 promoted: two events)
(= seat-146-152-162 **E-2** / seat-125-126 LOW #5 / **R65**)
`1787841675-...deck146-...-vs-...deck125.jsonl` seq **82** (286 chars mojibake, 25 s);
`1787841686-ai_baka_deck125-0x5584287653f0-vs-ai_baka_deck130.jsonl` seq **101** (409 chars CJK,
48 s): `unparsed_reply`, `coded_answers: 0`. Ask: `degenerate_decode` when a reply has no coded
line and no printable option name; one retry before Baka.
**Wave-50 prediction.** Garbage-decode fallbacks reaching Baka without a retry: 0/N (was 2).

## D20 — LOW — CARRIED wave-48 D14: the X free-kill marker on the top row
Deck130 3/3 marker windows correct this corpus (no failing window). Ask unchanged: echo the mark
on row 1 (`[X=2 kills the same for 4 less mana - see row 5]`). Companion (deck130 G3): the
Starstorm cast-row annotation could lead with the largest affordable X's kill list, matching the
owner's largest-first menu ruling.

## D21 — LOW — CARRIED wave-48 D15 + D19: menace single-block re-ask; keyword canon
No menace attacker at any blockers window this corpus (deck126 4 windows). Ask unchanged
(`multiblock_reask` reuse; one canonical substring per keyword).

## D22 — LOW — CARRIED wave-48 D16: overrun measurement
Multi-`CHOICE:` replies shrank (deck125 2/479, deck126 1/272, was 9/174); `multi_answer_first_taken`
2 (a listed sequence; first executed, second re-offered — correct). Counter still wanted before
any prompt-side change.

## D23 — LOW — CARRIED wave-48 D17: the JSONL `turn` field is the narration's turn minus one
Third corpus quoting off-by-one turns (`1787841678-...deck125-...-vs-...deck123.jsonl` seq 13 `turn:
10` under `=== Turn 11`). Emit the same 1-based number in both.

## D24 — LOW — CARRIED wave-48 D18: parity residuals, re-measured not re-derived
`{leaves N}` on activated rows (R15) · `manaAvailableLine` number word (R24) · cycling `[cost:]`
(R23, 14/14 — close on a third pass) · DFC Pathway tail (R18, untested) · `dropped_assignments`
(R22; lane R's cosmetic one-per-B-range count) · `ATTACK:` ranges/labels trimmed silently (R44,
new instance `1787841703-ai_baka_deck126-0x55ce886fa020-vs-ai_baka_deck123.jsonl` seq 27 `ATTACK:
A1, A2` on a one-row screen, no `dropped`) · `Keep this hand (keeping N)` (R46) · Ob Nixilis `-2`
target text (R47, no split arose) · LETHAL castability fact (R26) · `x1` repeat takes lose their
suffix in `chosen_text` (seat-123-130 L1) · names-form `ATTACK:` answers tolerated (E-5) ·
`stale_echo` of the previous window's cast row on a 2-row target menu
(`1787841677-...deck146-...-vs-...deck152.jsonl` seq 54). NEW residuals filed here: the owner-side
LOOP clause wording (R66), the second-copy count on a cast row (R67), `{paying this taps:}` on
defenders / the Battlement question (R68), the `SURVIVES`-all line on damage asks (R69), the
counter-target `{already 6/6}` annotation (R70), the log-share watch (R71).

## D25 — LOW — CARRIED (no record this corpus): the compound Doomsayer + Intruder Alarm loop
Named by seat-146-152-162 E-1 against a game that is NOT in this corpus (wave-47 artifact — see
the header). This corpus's own evidence points the other way: the 322-Vampire own-upkeep turn
closed in 16 windows by the repeat row + pass row, 21/21 natural. Kept as a watch with the
lane-N ceiling (200 per take) unchanged; re-open only on a record from a current corpus.

## D26 — MED (frontend / human seat) — CARRIED wave-48 D20: Arena-style grouping + kill the O(n^2) RenderSpell scan (owner item)
Unchanged; this corpus produced a 322-body board (`Vampire #1-#322`), so the human-seat pile
problem remains reachable. Measure the frame-time knee with a windowed Doomsayer+Alarm probe
before designing; kill the O(n^2) scan regardless.

---

# DISCHARGED — SET B: wave-49 engine lanes, by the emitter's actual string

| lane / commit | prediction | verdict | counts |
|---|---|---|---|
| **R fa1fddf02** B-row ranged + factored | no blockers prompt > 15,000; no prompt > 30,000; no B-row parenthetical > 2x | **PASS on the emitter; FAIL on both bars by other causes** | 52 `may block` clauses, 8 `(all: ...)`, 3 ranges, 0 repeats > 2x; blockers max 61,240 -> **17,616** (14,547 of it game log — `146 vs152` seq 55); the 30,000 bar fell to ONE record, the `legal targets right now:` clause (**D5**) |
| **R** `#n` sort interleave (D13) | 0 same-name rows split by another name | **PASS 0/112** | closed |
| **R** vigilance double-count (D9) | K + M ≤ N on every header | **PASS 39/39** (vigilance case did not arise) | close on a second corpus |
| **U 2d6b16d8f** converter PAIR (D5) | every both-halves prompt says `Both halves of a life LOOP`; 0 non-`none` ATTACK into it; 0 "net 0" | **PASS, and the wave-48 lost game was REPLAYED CORRECTLY** | 223 prompts; `152 vs126` s31, s40 `ATTACK: none` (was `A1`, 20 -> 0); 0/1,915 "net 0"; deck146 0 windows (UNTESTABLE-AT-THIS-SEAT, skill #132) |
| **U** `[DRAW PRICE:]` classes (D6) | Revelation / Clue / opposing cast-trigger rows priced | **PASS on the cast-trigger class (48 renders); Revelation/Clue UNTESTED (0 rows)** | `deck125-vs-deck162` seq 27 `casting this draws YOU 7 cards (their Forced Fruition) ... deal you 21` |
| **U** `DRAW FORECAST` | wherever `DRAW PUNISHERS` | **PASS by intent (98/98 punished-seat), absent on the punisher's 63** | **D16** |
| **U** carried-plan expiry (D7) | no verbatim echo > 5; 0 "the game is lost" | **FAIL by the string on both; the rule's CONTENT half is right, its COUNT half is wrong** | longest run 28 (`125 vs126`, a correct action-naming plan); "lost" carries 5 (was 48); two correct plans echoed 8-9 windows that a count trigger would have expired -> **D10** |
| **U** back-face marker (D12) | bare `// <name>` 0/N | **PASS 177/183; 6 on a second emitter path** | **D17** |
| **T a2fd9684a** auto-tap order (D4) | 0 casts tapping a colour-C source while a non-C could pay | **UNADJUDICABLE by string; one diff-sample against; the D1 regression dominates any diff** | `162 vs125` seq 26 -> 27; receipt = **D18** |
| **T** payment-taps rows (D11) | creature-tapping casts in a lethal-on-board window 0; `becomes` on a `[tapped]` source 0 | **PASS 0 / 0, OBEYED** | 10 `{paying this taps:}` rows, 2 taken (not lethal-on-board); Katilda 0/7 in Upkeep (was 8/9); tapped-Hive row passed (`146 vs152` s59); never fired on Overgrown Battlement (R68) |
| **T** edict row (D10) | edict casts at N=0 0/N | **PASS 0/123; ONE FALSE ROW** | `at 0 this does nothing` chosen 0; named-sacrifice form 9 chosen at N=1; **Devour Flesh's `you gain N` is the wrong recipient -> D3** |
| **T** sweeper row (D10) | casts on `destroys 0` 0/N | **PASS 0/87** | `exiles 5 (0 able to attack)` and Lightmine `deals 0 until they have an attacker` taken once each (the K semantics -> **D13**; guide W17 re-keyed) |
| **S 63e456fdf** name-vs-index (D2) | off-menu-name replacements executed 0; false `answer_replaced` 0 | **PASS 0; PASS 10/10; NEW hole** | `named_row_not_offered` 3 (1 -> re-ask, 2 informational); all ATTACK/BLOCKS replacements last-line and correct; the clean-line rule executed a REJECTION once -> **D7** |
| **S** repeat grammar (D3) | `repeat_count_missing` without a prior re-ask 0 | **PASS** | 0/1,915; `[RE-ASK]` 1/1 recovered -> `x19` -> `ran 19 of the 19` |
| **S** index-past-menu re-ask (D8) | Baka fallbacks of the class without a `[RE-ASK]` 0; ≥ 50% recovered | **PASS 0/1; 1/1 recovered** (class was 4/5 of wave-48 fallbacks) | `146 vs126` s9 -> s10; but `CHOICE: 0` on a no-pass ask escaped it -> **D9** |
| **S** exit row + `kCastAnsweredFact` | rendered | **PASS** | 525 / 125 |
| carry: max priority windows/turn ≤ 10 | | **FAIL (16)** | the 322-Vampire own-upkeep turn -> **D11** (+ guide A49-1/A49-2) |
| carry: fallback rate ≤ 0.31% | | **PASS** | 0.21% Baka-executed |
| carry: `mana_only_windows_skipped` ≤ 1,000 | | **PASS** | 265; six single-row leaks -> **D12** |
| Vita/human-seat fixes | not LLM-visible | no window | |

# DISCHARGED — SET A: wave-48 guide edits + boundary pass, by deck (reviewer verdicts, summarised)

| deck | verdict summary | routed |
|---|---|---|
| **123** (1-6) | A48-1 repeat stop **FAIL 8/11** (own-upkeep loop to 322, plan lost its number) -> A49-1/A49-2 (skill #120); Tutor-with-Alarm FAIL 3 (A49-7); RULE 5/3 re-keyed to the lane-T rows (A49-4/5, skill #121); Skeins dig PASS 3/3; "no blockers" text 0/3; Damnation absolute violated CORRECTLY at 0-vs-(wall + 1/1) (skill #121); cleanup discard by the engine (D4); Devour row false (D3); named card unrendered (D14) | D3, D4, D5, D7, D8, D9, D11, D14 |
| **125** (3-3) | X 8/8; sweeper-at-0 **FAIL 2/4, sixth wave** (Final Judgment on `exiles 5 (0 able)`, Lightmine on `deals 0`) -> W17 re-key + D13; Staff gate **FAIL 3/6, sixth wave, plan line 6/6 verbatim** -> W15 gate-first (skill #120); Emrakul absolute obeyed into a -12 loss -> W14 (skill #123); counters on walls 0/8; Path on tokens 2, on a wall 1 (W19); land 84/88 (first corpus); mulligans 0/6 | D5, D13 |
| **126** (4-2) | Tribute-at-1 **PASS 4/4** (ceiling); Tribute-at-0 0/~120; win button FAILED 3 windows with the Tribute row on the menu -> F10 (skill #121); second Blood as "both" -> F11 (skill #125); Lantern over a wall 2 + 4 second Lanterns (F13); STEP 1 third corpus -> F14 (skill #126); floors retired (F15); **Tribute resolved with no sacrifice (D2)**; mana-only leaks 6 (D12) | D2, D12 |
| **130** (0-6) | mulligan floors retired (D49-1, skill #131); Hammer return with a body in hand 2/2 -> D49-2 (skill #129); Starstorm after Molten Rain -> D49-3 (skill #130); SURVIVAL FIRST obeyed and wrong -> D49-4 (skill #122); two self-hits -> D49-5 (skill #124) + **D6**; planeswalker-first PASS 2/2; X marker 3/3; `Yourself` 0/14; "lost" carried 1 | D6, D10 |
| **146** (4-2) | every wave-47/48 rung PASS or held; converter lanes 0 windows two corpora (UNTESTABLE-AT-THIS-SEAT, skill #132); tapped-Hive row rendered + passed (lane T); `named_row_reask` recovered (lane S); guide UNCHANGED; garbage decode s82 (D19); bare back-face on the MDFC menu (D17) | D17, D19 |
| **152** (4-2) | 152-A loop pair **PASS — the wave-48 fatal attack replayed as ATTACK: none**; 152-C Katilda 0/7 in Upkeep (was 8/9); deploy 35/35; Fateful Absence 11/11; counter-spread 2/13 fourth corpus -> row annotation (R70); guide UNCHANGED | D10 (harmless-echo counter-instance) |
| **162** (5-1) | RULE 1 17/18; Rule 2 3 breaks, plan-text remedy FAILED -> lane exhausted -> **D15** (R39 promoted); walls/sends/land clean; mulligan by look held (1/1); guide UNCHANGED; **closest to exit** — on the wave-50 agenda if D15 ships | D15, D16 |
| **pool** | wave-48 boundary B1 (deck123 "No other spelling is read") CUT — PASS; B2 mulligan floors: gone at deck130/deck126 this wave, pool converged on the odds form (deck123's and deck152's residual "(keeping 5) + a land: keep" lines are the #119 odds-by-look shape, not a floor); B3 pair line at 146/162 present (1 mention each); B4 edict derivations re-keyed to the row (deck123) — deck126 F10 keeps one pointer | |

---

# CARRIED — open, re-measured, not re-derived
See D20-D26. The wave-48 OPEN questions: "does the repeat row get taken" — CLOSED again (12/16,
every counted take executed exactly); "the ~800 s ceiling" — max 727 s, a complete 7.6 KB reply,
not a truncation; the floating-mana disappearance (wave 46) — now EXPLAINED as D1's residue
(`Already in pool` after an answered cast), not a separate item.

---

# FALLBACK CLASSES — 4 Baka-executed / 1,915 = 0.21% (6 / 0.31% counting recovered re-asks)

| n | class | records | disposition |
|---|---|---|---|
| 2 | **garbage decode** (no coded line) | `deck146-vs-deck125` seq 82; `deck125-vs-deck130` seq 101 | **D19** counter + one retry |
| 1 | **the prompt's own worked example, unparseable** (+ `CHOICE: 0` on a no-pass ask) | `deck123-vs-deck130` seq 31 | **D8** + **D9** |
| 1 | **stale echo** of the previous window's cast row on a 2-row target menu | `deck146-vs-deck152` seq 54 (`CHOICE: 1 (Cast Vanishing Verse)` on `Wolf / Wolf`) | D24 (watch; the name-vs-index check already refuses it) |
| 2 (recovered) | designed re-asks | `repeat_count_reask` `deck123-vs-deck125` seq 11; `named_row_reask` `deck146-vs-deck126` seq 9 | lane S working |
| 0 | index-past-menu executed by Baka (was 4) | | **lane S paid** |
| 0 | retraction-to-Baka (was 0) | | E1 holds |

---

# PROMPT CHARS PER DECISION KIND — wave 48 -> wave 49 (ALL games, 21/21)

| kind | n48 | mean48 | max48 | n49 | mean49 | max49 | p90 latency 48 -> 49 (s) |
|---|---|---|---|---|---|---|---|
| ask | 1,240 | 8,162 | 27,987 | 1,422 | 10,485 | **34,333** | 55.7 -> 49.2 |
| priority | 364 | 11,331 | 27,721 | 364 | 12,204 | 28,487 | 47.7 -> 52.4 |
| attackers | 71 | 9,588 | 24,153 | 91 | 10,690 | 25,701 | 50.6 -> 46.6 |
| blockers | 26 | 12,733 | 61,240 | 21 | 9,782 | 17,616 | 56.1 -> 55.1 |
| reveal | 19 | 12,006 | 15,555 | 8 | 11,883 | 16,303 | 49.7 -> 56.0 |
| bottom | 8 | 2,107 | 2,370 | 8 | 2,170 | 2,660 | 82.3 -> 79.3 |

Blockers max 61,240 -> 17,616 (lane R did its job; the residual is log). The ask max is one
record (D5). Ask mean rose 28% on three 50-60-turn games whose late prompts are 22-24 KB of
log — the log window, not an emitter, is now the size driver in every kind (R71, watch).

---

# LATEGAME SPECIMEN — invariant 0 (for the owner's per-wave prompt review)

**Primary (one turn, both seats):** `1787841678-ai_baka_deck125-0x561c77675570-vs-ai_baka_deck123.jsonl`
seq **13** (34,333 chars: `Vampire #1-#322 ... x322` collapsed on the battlefield line, 323
handles enumerated on the Path to Exile row — **D5** — beside `Cast Supreme Verdict ... destroys
323 of their creatures (0 able to attack)` — **D13**) paired with
`1787841678-ai_baka_deck123-0x561c789917c0-vs-ai_baka_deck125.jsonl` seq **11-23** (the repeat-row
`[RE-ASK]`, the x19/x25/x50/x120/x100 takes and receipts, the `[repeat: ... you control 223
creatures]` tag — **D11**). Every lane-S string and the one enumeration lane R missed.
**Engine-correctness specimen:** `1787841695-ai_baka_deck126-0x55ab47d01810-vs-ai_baka_deck146.jsonl`
seq **13-14** with the opposite seat's seq 15 (the Tribute that sacrificed nothing — **D2**).
**Double-tap specimen:** `1787841675-ai_baka_deck146-0x5560568482a0-vs-ai_baka_deck125.jsonl` seq
**32 -> 33** (4 lands tapped for `{b}{w}`, `Already in pool: {b}{w}`, Cancel on the stack — **D1**).
**Natural late grind:** the same file seq **93-99** (turn 34-36: `Your library: 1 cards`, Emrakul
forcing six consecutive self-sacrifice asks, a 15-permanent board, the garbage decode at s82).
**Ordinary lategame:** `1787841701-ai_baka_deck125-0x562af3b37450-vs-ai_baka_deck126.jsonl` seq
**117** (t60, 19,035 chars: 21 lands, both loop halves on the opponent line with the LOOP clause,
an Emrakul row at `leaves 6`, and the losing choice — guide W14).
