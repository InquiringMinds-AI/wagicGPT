# Wave-49 general strategy — RENDER / CORE-PROMPT proposals, layer-routed (R1-R47 revised, R48-R71 new)

Corpus `matchups-20260827-094106` (sixth fair-hand corpus), binary master **7e94544f1** (owner
Vita fixes 19e2b04f7 + 6819183f7; lanes R fa1fddf02 / U 2d6b16d8f / T a2fd9684a / S 63e456fdf).
42 seat logs, **1,999 records**, **1,915 decisions** (ask 1,422 / priority 364 / attackers 91 /
blockers 21 / reveal 9 / bottom 8), **21/21 games natural — 42/42 seats carry a `gameend`**
(verified on disk this step; the `seat-146-152-162.md` E-1 "excluded deck123-vs-deck162, no
gameend" note is a stale artifact of the wave-47 run `matchups-20260826-182155` and is
DROPPED — every rate below uses 21/21). Longest seat 129 records; longest games turn-field 60,
54, 50. Sources: the four seat files, seven `deckN/general-suggestions.md`, seven `deckN/skill.md`.

**Routing rule (owner doctrine 2026-08-26).** The core loop — engine mechanics, LLM UX, bugs —
is the purpose. Everything below is a change to what the ENGINE RENDERS or to the CORE PROMPT's
own text. Guide-side answers are named only where a rule must stay correct AFTER the render
ships. Engine BUGS with call sites are in `engine-ledger.md` (the wave-50 docket); this file is
the ranked routing view with the evidence and the wave-50 falsifier for each item.

**Governing constraint on every item.** Legality is enforced structurally; the model's CHOICE
is never constrained. Every proposal ADDS a true token, a true row, a receipt or one re-ask, and
DELETES nothing. Hard caps are rejected. The one COUNTER-INSTANCE this corpus (R56): a
raw-count plan expiry would have wrongly expired a CORRECT loop-lockout plan — the expiry keys
on content, never on a count.

---

## STATUS OF R1-R47 (wave-48 items), by the emitter's actual string

| item | wave-49 verdict | evidence | residual |
|---|---|---|---|
| **R1** loop affordance (lane N) | **HOLDS; one turn at 16 windows** | repeat row offered 16, taken 12, pass chosen 4; receipts `ran N of the N times you named` on 8 counted takes; 21/21 natural; max priority windows in one turn **16** (`deck123-vs-deck125` turn-field 9: singles + x19/x1/x1/x2/x25/x50/x120/x100 built 322 Vampires) vs the ≤ 10 carry prediction | the pilot overshoots its own stop with the count on the tag and the example `x50` on the row — the row does not put the two numbers side by side (**R61**, docket D11); guide-side skill #120 |
| **R2** A-row ranged collapse | **HOLDS** | attackers max 25,701, p90 46.6 s; `ATTACK: A1, A2, A3-A257`-class replies parsed; one reply answered by NAME on a 4-row screen and was sent (`146 vs125` s70) | names-form answer tolerated, not requested (LOW, R46) |
| **R3** target-menu collapse | **HOLDS on numbered target MENUS; the cast row's `legal targets right now:` clause was NEVER collapsed** | `deck125-vs-deck123` seq 13: **34,333 chars**, 28,755 of them one Path to Exile row enumerating 323 handles with ONE body (**R49**, docket D5) | |
| **R4** attacking-header scope | **PASS 39/39** | `K of them are attacking right now, M more able` with K+M ≤ N on every header | none |
| **R5** priced representative | **HOLDS (thin)** | no collapsed-tag pricing miss | none |
| **R6** `{card text:}` budget | **PASS 0 mid-word; back-face marker 177/183** | `// <Name> (text omitted)` 177; 6 bare endings, all on the `Choose an option for <MDFC>:` menu's `Cast Card Normally` row (**R63**, docket D17) | |
| **R7** `auto=choice` header | **PASS** | 14 `Choose an option` menus at deck130, 14/14 read correctly | none |
| **R8** stack keyword tags | **PASS 0/8** | deck125 counters on `[defender]` / power-0 stack rows 0/8 (W42) | none |
| **R9 -> R31** draw price | **SHIPPED (lane U), PASS where tested** | `[DRAW PRICE:]` 48 renders incl. the opposing cast-trigger class (`deck125-vs-deck162` seq 27: `casting this draws YOU 7 cards (their Forced Fruition) ... deal you 21`), Skeins/cycling/Spellbomb rows tagged; 0 draws at a tagged row at deck130 (12/12); `DRAW FORECAST` on 98/98 punished-seat prompts | `draw:X` / Clue rows UNTESTED (0 rendered under a punisher); the FORECAST is absent on the PUNISHER's own 63 prompts (**R62**, docket D16) |
| **R10** ANNOUNCE_X | **PASS 3/3 + 8/8** | deck130 marker 3/3; deck125 X=3 whenever the menu reached 3, 8/8 | none |
| **R11** no plan where it cannot change | **HOLDS** | pregame 0 plan blocks; land asks 0 declines at five seats; deck125 4/88 late declines at 12+ lands (first corpus, counted) | |
| **R12** converter PAIR loop clause | **SHIPPED (lane U), PASS — THE WAVE-48 LOST GAME DID NOT RECUR** | 223 prompts carry `Both halves of a life LOOP`; 0 both-halves prompts lack it; `152 vs126` s31, s40 `ATTACK: none` (was `ATTACK: A1`, 20 -> 0); 0/1,915 "net 0" replies | owner-side wording (**R66**); deck146 0 windows two corpora (skill #132) |
| **R13** GAME LOG collapse | **HOLDS; the log is now the size driver** | late prompts are 78-84% game log (28,487 / 28,406 / 28,153 at turn-fields 31, 53, 40; `146 vs152` seq 55 blockers 17,616 of which 14,547 is log) | watch, **R71** |
| **R14** battlefield grouping | **HOLDS** | `Vampire #1-#322 (4/4) ... x322`; same-name rows split by another name 0/112 (lane R D13) | |
| **R15** `{leaves N}` on activated rows | **CARRIED** | no new instance | LOW (docket D24) |
| **R16/R17 -> R35** index-past-menu re-ask + exit row | **SHIPPED (lane S), PASS** | `named_row_reask` 1/1 recovered (`146 vs126` s9 -> s10); `Cast nothing right now (combat comes next this turn)` 525; `kCastAnsweredFact` 125; index-past-menu Baka fallbacks **0** (was 4/5) | `CHOICE: 0` on a NO-PASS ask (damage order) went to `unparsed_reply` with a valid first line above it — the re-ask did not fire for index 0 (**R57**, docket D9) |
| **R18** DFC Pathway tail | **UNTESTED** | | LOW |
| **R19 -> R38** what paying TAPS | **SHIPPED (lane T), PASS, OBEYED** | `{paying this taps: ...}` 10 rows, 2 taken (neither lethal-on-board); Katilda in Upkeep/Main-1 **0/7** (was 8/9, one fatal); `[this land is TAPPED: ...]` on the Hive row rendered and PASSED (`146 vs152` s59) | never fired on Overgrown Battlement in six deck126 games — payment order or defender suppression unknown (**R68**) |
| **R20** degenerate-decode counter | **CARRIED, NOW WITH TWO EVENTS** | `146 vs125` s82 (286 chars mojibake), `125 vs130` s101 (409 chars CJK) — both `unparsed_reply`, indistinguishable in the field from an off-menu name | promoted (**R65**, docket D19) |
| **R21** mana-only gate fuzz | **CARRIED; six LEAKS** | `mana_only_windows_skipped` max 265; `deck126-vs-deck125` seq 66/68/75/77/85/87 each a single `Add N green mana with Overgrown Battlement` row, three with an EMPTY stack, 14-16 KB, all `pass` (**R58**, docket D12) | |
| **R22** `dropped_assignments` pads | **CARRIED, 0** | lane R's note: a B-range reply counts one cosmetic `dropped` | LOW |
| **R23** cycling `[cost:]` | **PASS 14/14** | deck130 `Choose an option` menus | LOW, close if a third corpus passes |
| **R24** number word above ten | **CARRIED** | | LOW |
| **R25 -> R33** `{right now:}` on edict/sweeper rows | **SHIPPED (lane T), PASS — with one FALSE row** | edict rows 123 rendered, `at 0 this does nothing` chosen 0; sweeper `destroys 0` rows 87, chosen 0/87; deck126 Tribute-at-1 4/4, Tribute-at-0 0/~120 | **Devour Flesh's row says `you gain N` and the TARGET gains** (`123 vs162` seq 8: plan "gain 5", opponent 20 -> 25) — the corpus's one render falsehood (**R51**, docket D3); `(K able to attack)` reads 0 for summoning-sick tokens AND walls, so `destroys 323 ... (0 able to attack)` and `exiles 5 ... (0 able to attack)` look alike (**R59**, docket D13); Final Judgment `exiles 5 (0 able)` and Lightmine `deals 0 until they have an attacker` were taken once each (deck125 W39 — guide re-keyed to the row) |
| **R26** LETHAL castability fact | **CARRIED** | | LOW |
| **R27 -> R43** keyword wordings / menace re-ask | **CARRIED, no window** | 0 menace attackers at deck126's 4 blockers windows | LOW (docket D21) |
| **R28** B-row `may block` list | **SHIPPED (lane R), PASS on the enumeration; FAIL on the 15 KB bar by log growth** | 52 `may block` clauses, 8 `(all: ...)`, 3 ranges; blockers max 61,240 -> **17,616** (log 14,547 of it); 0 B-row parenthetical > 2x | none on the emitter |
| **R29** E1 name/index + ATTACK/BLOCKS | **SHIPPED (lane S), PASS on both predictions; one NEW hole** | off-menu-name replacements executed 0 (`named_row_not_offered` 3, none executed); `answer_replaced` 10/10 truthful; all ATTACK/BLOCKS replacements last-line and correct (`146 vs123` s11, `152 vs146` s33) | the "last CLEAN coded line" executed a line the reply wrote to REJECT (`123 vs125` seq 94: `CHOICE: 1 (Cast Lightning Greaves) is wrong because ...` -> Greaves cast, countered) and skipped a trailing-period line (seq 12: x19 over x20) (**R54**, docket D7) |
| **R30** repeat-count grammar | **SHIPPED (lane S), PASS** | `repeat_count_missing` 0 (was 4); `[RE-ASK] You chose the repeat row but named no count` 1/1 recovered -> `x19` -> receipt; counts x17/x5/x19/x2/x25/x50/x120/x100 all executed exactly; `repeat_count_under_two` 3 (harmless) | see R61 |
| **R31** draw price classes + FORECAST | see R9 | | |
| **R32** auto-tap colour choice | **SHIPPED (lane T), UNADJUDICABLE BY STRING** | no line names which sources a payment tapped; one diff-sample against the ordering (`162 vs125` seq 26 -> 27: Fog Bank's generic paid from a dual while an Island sat untapped); and the double-tap regression (R48) dominates any diff-based measurement | `- Paid {cost} with <sources>` receipt (**R64**, docket D18) |
| **R33** edict/sweeper `{right now:}` | see R25 | | |
| **R34** immortal action-less plan | **SHIPPED (lane U), PARTIAL** | "the game is lost" carries 48 -> **5** (`123 vs125` seq 53-56 one-window carries of a just-said plan; `130 vs125` seq 125); `Yourself` damage targets 0/14 | the guard cannot fire when the verdict goes on to name cards; a plan re-said every ~6 windows never expires (`125 vs126` seq 69-96, 28 identical); the opening plan rode 8-9 windows through land asks (`123 vs126` seq 2-9, `162 vs146` seq 2-10) (**R56**, docket D10) |
| **R35** index-past-menu re-ask | see R16 | | |
| **R36** back-face marker | see R6 | | |
| **R37** vigilance double-count | **SHIPPED (lane R), PASS 39/39; the vigilance case did not arise** | | close on a second corpus |
| **R38** what paying taps | see R19 | | |
| **R39** draw-engine row's feeds count | **PROMOTED TO MED — the guide lane is EXHAUSTED at deck162** | Rule 2 breaks 3/3/6/3 across four corpora, all in wins; the #102 plan-text remedy FAILED (`162 vs125` s8 plans "stack ... once a punisher is on the battlefield" while casting Master with none) | `{feeds: the opponent draws N extra per turn; punishers on your battlefield: K}` on the engine cast rows (docket D15) |
| **R40** overrun counter | **CARRIED; the overrun shrank** | multi-`CHOICE:` replies deck125 2/479, deck126 1/272 (was 9/174); `multi_answer_first_taken` 2 | LOW (docket D22) |
| **R41** `#n` sort straddle | **SHIPPED (lane R), PASS 0/112** | | closed |
| **R42** 1-based `turn` | **CARRIED, third corpus quoting off-by-one turns** | `deck125-vs-deck123` seq 13 `turn: 10` under `=== Turn 11` | LOW (docket D23) |
| **R43** menace re-ask | **CARRIED, no window** | | LOW (docket D21) |
| **R44** ATTACK range trimmed silently | **CARRIED, one new instance** | `126 vs123` seq 27 `ATTACK: A1, A2` on a one-row screen, no `dropped` count | LOW (docket D24) |
| **R45** X marker on the top row | **CARRIED, no failing window** | deck130 3/3 marker windows correct | LOW (docket D20) |
| **R46** small residuals | **CARRIED** | + names-form ATTACK answer tolerated; `x1` takes lose their suffix in `chosen_text` | LOW |
| **R47** Ob Nixilis target text | **CARRIED, no maker/token split arose** | 3 four-way `-2` menus, no error | LOW (docket D24) |

Of R1-R47: **R12, R28, R29, R30, R33, R35, R37, R38, R41 SHIPPED and PASSED** (three of them
replayed the wave-48 lost or fatal window correctly: R12 at `152 vs126`, R38 at `152 vs162`
Upkeep Katilda and `146 vs152` tapped Hive, R35 at `146 vs126`); **R31 shipped and passed on
the classes it met** (two untested); **R34 shipped and partial**; **R32 shipped and
unadjudicable**; R39 promoted; R20 promoted; the rest carried with the new evidence on each.

---

## HIGH

### R48 — casts that get RESPONDED TO pay twice: the seat's response mana is gone in the exact window a counterspell is on the stack. (= ledger **D1**; seat-engine **D-4 HIGH**) — **IN FLIGHT: lane V is hunting it in `~/Projects/wagicGPT-w50-v`**
Diffing `Mana available: N total` across consecutive windows for 234 non-X casts: **12 tapped
exactly twice their pips and left the difference floating** — `deck146-vs-deck125` seq **32**
`Cast Silverquill Silencer {b}{w}` with 6 sources -> `Mana available: 2 total ... Already in
pool: {b}{w} (2 mana ALREADY produced and floating ...)`, battlefield showing four lands
`[tapped]` for a two-pip spell; `deck162-vs-deck125` seq **33** `Cast Howling Mine {2}` with 7
sources -> 3 untapped + `{u}{u}` floating, all four blue sources gone with Cancel on the stack
and no blue to respond; `deck146-vs-deck125` seq 39/49/76, `deck123-vs-deck125` seq 46/69
(`Idyllic Tutor {2}{w}`, 6 tapped), `deck125-vs-deck126` seq 118 (`Elixir {1}`, 2 tapped).
**11 of the 12 are casts the opponent answered on the stack** (`- You cast X / - Opponent cast
Cancel|Essence Scatter|Fall of the Gavel`); 13 other answered casts did not double-tap. Wave 48
had 7/7 the same shape (`146 vs125` seq 47 Silverquill Command 8 tapped, seq 53 Kaya 10
tapped); the wave-47 corpus has **0** — this began with the wave-48 binary (E1 + lanes
P/O/N/Q), NOT lane T. The floating mana drains at end of phase, so each such cast costs its
price twice and empties the response mana. Repro shape: GPT seat casts, the opponent responds
before the cast's own priority window; check whether the cast's `AIAction` (or the auto-tap via
the cast click) is applied a second time when the interrupted priority round re-enters the seat.
This is engine correctness — not a render item, not a guide item; nothing in a guide may
describe it (trust doctrine).
**Wave-50 prediction.** Casts with `tapped == 2 x pips` by the source-count diff: **0/N** (was
12/234); with R64's receipt, one `- Paid` line per cast, never two.

### R49 — the `legal targets right now:` clause on a cast row is the last uncollapsed enumeration; one row was 28,755 chars. (= ledger **D5**; seat-engine **D-1 HIGH**, seat-125-126 **HIGH #2**, deck125 G1)
`deck125-vs-deck123` seq **13** (ask, Main 1, turn-field 10): **34,333 chars** — `3. Cast Path to
Exile {w} {leaves 5 of your 6 untapped mana sources untapped} - legal targets right now: Lord of
Lineage (creature 5/5) [...], Vampire #1 (creature 4/4) [flying, doesn't untap during its
controller's untap step], Vampire #2 (...), ..., Vampire #322 (...)` — 323 handles, ONE distinct
body, while the battlefield line two sections up reads `Vampire #1-#322 (4/4) (printed 2/2)
[flying, ...] x322` and the Supreme Verdict row beside it reads `destroys 323 of their creatures`.
25 rows pool-wide carry ≥ 10 targets (`Stone Rain` / `Lay Waste` against 30 lands,
`deck130-vs-deck125` seq 115-125, 1.2 KB each). Lane R's "no prompt > 30,000" fails on this one
record and nothing else. Ask: reuse the battlefield/A-row range collapse on the target clause
for consecutive same-name same-body handles (`Vampire #1-#322 (creature 4/4) [...] x322`, ~150
chars) and cap distinct entries with `(+N more like these)`.
**Wave-50 prediction.** No prompt > 30,000 chars; no single option row > 3,000 chars (was
28,755); the 322-token board's cast ask ≤ 8,000 chars.

### R50 — Tribute to Hunger resolved with NO sacrifice and NO life gain against a two-creature board; a stale reveal zone is the suspect. (= ledger **D2**; seat-125-126 **HIGH #1**)
`1787841695-ai_baka_deck126-0x55ab47d01810-vs-ai_baka_deck146.jsonl` seq **13** (t9 main 2, life
24): `Cast Tribute to Hunger {2}{b} {right now: they control 2 creatures - they choose which one}`
taken against Nadaar, Selfless Paladin (3/3) and a Goblin token. Seq 14 events: resolved, "Opponent
used: Gain life equal to its toughness with Tribute to Hunger", "Your revealed Savannah went to
your hand" x2 — no "died", no "You gained". The opponent's seat
(`...deck146-0x55ab469c8240-vs-...deck126.jsonl` seq 15) says it outright: **"You revealed 2 cards
but none was a legal target - took none"**. One turn earlier that seat had resolved Pelakka
Predation (`reveal:type:*:opponenthand revealzone(opponenthand) ... optiontwo ... all(*|reveal)
moveto(hand)`), and the two revealed Savannahs sat in the reveal zone until Tribute's
`notaTarget(creature|mybattlefield)` chooser fired and found "2 revealed cards, none a legal
target". Oracle: "Target opponent sacrifices a creature. You gain life equal to that creature's
toughness" — a 3/3 or a 1/1 had to go. Inference, not stepped in code: the sacrifice picker
shares the reveal/choice machinery and Pelakka's option-two `moveto(hand)` did not empty the
reveal zone at resolution. Repro for the suite: opponent resolves any `revealzone(opponenthand)`
reveal; next turn the revealed player casts Tribute to Hunger / Devour Flesh / any
`sacrifice!$ targetedplayer` edict. Rules-correctness, not a render item.
**Wave-50 prediction.** Edict resolutions at N ≥ 1 with no `died`/sacrifice event: **0/N** (was
1); "revealed N cards but none was a legal target" at an edict resolution: 0; a suite pin for the
reveal-then-edict sequence.

### R51 — the Devour Flesh edict row prints `you gain N`; the TARGETED player gains. The corpus's one render falsehood. (= ledger **D3**; seat-123-130 **H1**)
Primitive: `Devour Flesh ... text=Target player sacrifices a creature, then gains life equal to
that creature's toughness` (`targetedplayer`). The row prints the TRIBUTE wording: `123 vs146`
seq 6 `Cast Devour Flesh {right now: they control 1 creature - Silverquill Silencer is
sacrificed, you gain 2}`; `123 vs162` seq 8 `... Master of the Feast is sacrificed, you gain 5`
-> plan "remove Master of the Feast and gain 5 life", narration: the opponent went 20 -> 25;
`123 vs125` seq 96 `... Emrakul ... you gain 15` -> plan "gain 15 life (now 17)" (countered). 10
such rows at the pair, believed twice. The guide states the truth (deck123:199-204 — "Naming the
opponent does NOT gain YOU anything") and, correctly, does not teach doubt of the row; the row
must be fixed: `- the opponent gains N` when the target is the opponent; `you gain N` only for
Tribute to Hunger / a self-targeted edict (read the primitive's gain recipient, not the edict class).
**Wave-50 prediction.** Edict rows whose gain recipient disagrees with the primitive's
recipient: **0/N**; replies planning "gain N" on a Devour Flesh cast at the opponent: 0.

### R52 — the cleanup-step discard is chosen by the engine and never reaches the seat. (= ledger **D4**; seat-123-130 **H2**; skill #127)
`123 vs152` seq 4 (t3, `Your hand (7 cards)`): `Cast Vision Skeins`; seq 5 narration: `You drew
Thraben Doomsayer / You drew ... / Phase: --- / You discarded Intruder Alarm` — no record between
seq 4 and seq 5. The engine discarded the deck's only combo enchantment. Ask: route the
hand-size discard to the seat as an ask (`Discard down to 7: choose N cards`), the same shape as
the London bottoming ask. Guide-side mitigation (A49-8, a hand-size check before a draw spell)
stays correct after the ask ships; the guide's sentence "the engine discards for you at end of
turn" (deck123:569) does NOT — boundary B1.
**Wave-50 prediction.** `You discarded <card>` narration lines with no `discard`/`bottom`-kind
decision record preceding them: **0/N** (was 1 seen; the seat lists them per skill #127).

### R53 — target rows VANISH between main 1 and main 2, and a window whose only action row hits the acting player's own permanent is offered every priority. (= ledger **D6**; seat-123-130 **H3**; deck130 G1; skill #124)
`130 vs146` seq **14** (t5 main 1, priority): five Pyrite Spellbomb rows (targeting Dwarven
Blastminer `[your battlefield]`, Nadaar, the opponent, you, Draw 1) -> pass; seq 15 `Cast
nothing`; seq **16** (main 2, priority): ONE action row — `Deal 2 damage with Pyrite Spellbomb
targeting Dwarven Blastminer [your battlefield] ... {if you pass here, this option is not offered
again this turn}` + `0. Pass priority` — taken; the Blastminer died. Where did the Nadaar /
opponent / Draw rows go while the self-target row survived? (Suspect, marked as inference: the
"already declined this turn" dedup keyed on something the self-target row escaped.) Second shape:
`130 vs126` seq 34-48, with no opposing nonbasic, every window lists `Destroy with Dwarven
Blastminer targeting Forgotten Cave #1/#2 [your battlefield]` as its only Blastminer rows; seq 38
took one. Ask: (a) root-cause the seq 14 -> 16 drop; (b) on a damage/destroy row whose target's
controller is the acting player, append `{this hits YOUR permanent}`, and on a window where every
action row carries it print one line above the rows: `Every action here targets your own
permanent; 0 (pass) is the usual answer.` No option removed.
**Wave-50 prediction.** Windows where a main-1 target set shrinks to a strict subset in main 2
with no board change: **0**; self-target rows taken: 0 (was 2); the self-only windows carry the
line (report N).

---

## MEDIUM

### R54 — the "last CLEAN coded line" rule executed a line the reply wrote to REJECT, and skipped a line that ended in a period. (= ledger **D7**; seat-engine **D-2 HIGH**, seat-123-130 **M3**)
`123 vs125` seq **94**: first line `CHOICE: 3 (Cast Thraben Doomsayer)`; last coded line `CHOICE:
1 (Cast Lightning Greaves) is wrong because we need to cast Doomsayer first`; reasoning ends "So
Choice 3 is correct for this window"; `chosen_text` = Lightning Greaves, `answer_replaced: true`;
next log: Greaves cast, Cancelled; Doomsayer stayed in hand at 2 life vs 57. Seq **12**: `CHOICE:
2 (... x19)` then `CHOICE: 2 (... x20).` -> 19 executed; the pilot's arithmetic said 20. 1/10
replacements this corpus was a rejection; the other 9 were genuine re-answers. Ask: a coded line
is CLEAN only if nothing but whitespace or a single terminal `.` follows its closing parenthesis;
`... is wrong`, `... would`, `... instead`, `... not` after the parenthesis disqualify it; on
disqualification fall back to the earlier clean line, and count `rejected_line_skipped`.
**Wave-50 prediction.** `answer_replaced` records whose executed line has trailing prose after
`)`: **0/N**; a trailing `.` alone never disqualifies.

### R55 — the prompt's own worked example is unparseable when the option name contains parentheses. (= ledger **D8**; seat-engine **D-3 HIGH**)
`123 vs130` seq **31**, damage-order ask: rows `1. Goblin (1/1) [...]`; the format line reads
`e.g. "CHOICE: 1 (Goblin (1/1))" (a worked example ...)`; the reply was exactly `CHOICE: 1 (Goblin
(1/1))` -> `unparsed_reply`; its trailing `CHOICE: 0 (Pass)` was executed on a 3-option order ask
with no pass row. Ask: strip the `(P/T)` from the example's short name, or parse a balanced
nested parenthetical.
**Wave-50 prediction.** `unparsed_reply` on a reply equal to the prompt's own worked example:
**0/N**.

### R56 — plan-carry expiry keys on CONTENT ("names no action from the current menu", or opens with a verdict), never on a raw echo count — the loop-lockout echo is CORRECT. (= ledger **D10**; seat-engine **D-5**, seat-123-130 **M2**, seat-146-152-162 **E-3**, deck123 G3, deck130 G4; skill #115)
Three shapes. (a) `125 vs126` seq 69-96: 28 identical `YOUR PLAN (as you last stated it): Answer
their threats, resolve Staff of Nin ..., ping their face every turn.`; the model re-stated it in
only 5/27 replies, so the 5-echo counter (which resets on a restatement) never fired — and the
plan names an action, so by the lane's own rule it carries: NOT a defect of the rule, a defect
of the counter. (b) `123 vs125` seq 53-56 and `130 vs125` seq 125: `The game is lost. ...` carried
5 windows — the text goes on to name cards, so "names no action" cannot fire; a rule keyed on the
OPENING CLAUSE catches all 5. (c) `162 vs146` seq 2-10 (9 windows) and `152 vs126` s31-41 (8
windows, "The opponent controls both Sanguine Bond and Exquisite Blood ... ATTACK: none"): the
echo is HARMLESS and the second is the correct loop-lockout plan — a purely-count trigger would
have expired it wrongly. Companion (deck130 G4): both self-hits carried a plan naming a target
the window did not list ("destroy Brightclimb Pathway", "destroy Forest") — the neighbouring
case. Ask: (i) drop the carry when the plan opens with a verdict (`The game is lost`, `Nothing
to do`) or names no card and no option verb on the CURRENT menu; (ii) when no row on the window
contains the plan's named target, print `(your plan's target is not on this window)` under the
carried plan; (iii) the "5 identical echoes" clause is RETIRED as a trigger and kept only as a
report counter. Deletes nothing the model said; adds one true line.
**Wave-50 prediction.** `The game is lost` carries **0** (was 5); self-hit windows carrying a
plan whose target is absent print the line (report N); the `152 vs126` loop-lockout plan is
carried unbroken through its stretch (0 wrongful expiries).

### R57 — `CHOICE: 0` on an ask with NO pass row falls back without a re-ask, even with a valid first line above it. (= ledger **D9**; seat-123-130 **M4**, deck123 G4)
`123 vs130` seq 31 (`Combat damage order ... position 1`, rows 1-3): coded lines `CHOICE: 1
(Goblin (1/1))` [unparsed, R55] then `CHOICE: 0 (Pass)` -> `unparsed_reply`, engine default,
129 s spent. Lane S's `[RE-ASK]` for an index outside the menu did not fire for index 0. Ask:
treat 0 on a no-pass ask as index-past-menu (one `[RE-ASK]`: `this ask has no pass - answer 1 to
N`), and print `(this ask has no pass row)` in the format line of mandatory asks.
**Wave-50 prediction.** `CHOICE: 0` on a no-pass ask reaching Baka without a `[RE-ASK]`: **0/N**.

### R58 — the mana-only gate leaks a single `Add N green mana with Overgrown Battlement` row — three times with an EMPTY stack. (= ledger **D12**; seat-125-126 **MED #3**)
`1787841701-ai_baka_deck126-0x562af4e53780-vs-ai_baka_deck125.jsonl` seq 66 (Cancel on Sorin on the
stack), **68 (empty)**, 75 (Dream Fracture on Lantern), **77 (empty)**, 85 (Cancel on Blood), **87
(empty)**; each `priority`, Main 1, options_text = that one row + pass, 14-16 KB, all `pass`;
`mana_only_windows_skipped: 265` in the same game, so the gate is live and these are its residue.
The empty-stack three are the clean repro; floating mana (`Already in pool: {g}{b}{b}{w}` on the
seq 66 line, after a countered cast — R48's residue) may be what defeats the "nothing but mana"
test. Not traced.
**Wave-50 prediction.** All-mana single-row windows reaching the model: **0** (was 6);
`mana_only_windows_skipped` per game reported.

### R59 — the sweeper row's `(K able to attack)` is 0 for summoning-sick and tapped creatures as well as walls, so `destroys 323 (0 able to attack)` and `exiles 5 (0 able to attack)` read alike. (= ledger **D13**; seat-125-126 **MED #4**, deck125 G2)
`deck125-vs-deck123` seq 13 `destroys 323 of their creatures (0 able to attack)` (323 fresh
tokens); `deck125-vs-deck126` seq 60 `exiles 5 of their creatures (0 able to attack)` (five walls;
taken — W39). The header's `N of them without a restriction against attacking` is the count the
guide keys on. Ask: the row carries that count and the reason — `(323 able to attack next turn)`
/ `(all 5 defenders)` — so the row alone carries the decision.
**Wave-50 prediction.** Sweeper casts whose row reads `all N defenders` with no clock on the
caster: **0**; the row's K agrees with the header's "without a restriction" count.

### R60 — a "choose a name" effect never renders the chosen name. (= ledger **D14**; seat-123-130 **M1**)
`123 vs146` seq 8: the opponent's seat (their seq 8) named `Vision Skeins` for Silverquill
Silencer; this seat's narration read `Opponent used: Choose a name with Silverquill Silencer`,
the battlefield line `Silverquill Silencer {b}{w} (3/2)` with no name, and each cast produced an
unattributed `You lost 3 life`; the pilot cast the named card at 15, 4 and 1 life (6 of its 20).
Ask: `[named: Vision Skeins]` on the permanent's line; `... - named Vision Skeins` in the
narration; the life-loss line attributed (`You lost 3 life (Silverquill Silencer: named card)`).
**Wave-50 prediction.** Named-card casts by a seat whose prompt shows `[named: <that card>]`:
**0/N**; unattributed `You lost N life` lines: 0.

### R61 — the repeat row does not put the stop and the count side by side; its example number is the number the pilot writes. (= ledger **D11**; deck123 G1/G2, seat-123-130 L2; skill #120)
`123 vs125` seq 16-22: x2, x25, x50, x120, x100 with the `[repeat:]` tag on the SINGLE row reading
`you control 223 creatures`; the repeat row's example `e.g. "CHOICE: 2 (Create human with Thraben
Doomsayer x50)"` is the only number on it; 8 of 11 takes at the pair were past the guide's own
stop. The guide's `PLAN: stop at M = ...` shape (A49-1, skill #120) is the guide-side half; the
engine-side option: the repeat row carries the current creature count in the battlefield line's
own words (`you control M creatures` — already computed for the tag) and prints the example with
a placeholder (`x<N>`). No cap: the pilot names N.
**Wave-50 prediction.** Every repeat row prints the creature count; the example carries `x<N>`;
takes of exactly 50/100/120 with no arithmetic in the reply: 0 (was 3).

---

## LOW

### R62 — `DRAW FORECAST` renders only for the punished seat. (= ledger **D16**; seat-engine **D-6**)
98/98 prompts headed `DRAW PUNISHERS on the battlefield: theirs` carry it; 0/63 headed `...: yours -
Underworld Dreams. Every card the OPPONENT draws costs them 1 life` do (`deck162-vs-deck125` seq
13). The mirror (`their next draw step draws K cards = K x N life to you`) is the punisher's clock.

### R63 — the `(text omitted)` marker misses the `Choose an option for <MDFC>:` menu's `Cast Card Normally` row. (= ledger **D17**; seat-engine **D-7**, seat-146-152-162 E-4)
177 marked, 6 bare on `deck146-vs-deck125` seq 6/88, `deck146-vs-deck152` seq 39/52,
`deck146-vs-deck126` seq 11, `deck146-vs-deck130` seq 10 (Pelakka Predation / Emeria's Call) — a
second emitter path; the same card's hand row carries the marker on the same seat.

### R64 — no payment receipt: nothing in the log names which sources a cast tapped. (= ledger **D18**; seat-engine **D-9**)
`- Paid {1}{u} with Island #2, Drowned Catacomb #2` per cast (already computed by
`selectAutoTapProducers`) makes lane T's ordering adjudicable by string and makes R48 visible
(two receipts for one cast). `162 vs125` seq 26 -> 27 (Fog Bank's generic paid from a dual while
an Island sat untapped) suggests the ordering has a case to answer once R48 is out of the way.

### R65 — degenerate-decode counter, now with two events. (= ledger **D19**; R20 promoted; seat-146-152-162 E-2, seat-125-126 LOW #5)
`146 vs125` s82 (286 chars mojibake, 25 s), `125 vs130` s101 (409 chars CJK, 48 s): `unparsed_reply`
with `coded_answers: 0`, indistinguishable in the field from an off-menu name. Ask:
`degenerate_decode` when a reply has no coded line and no printable option name; one retry.

### R66 — the owner-side LOOP clause names the victim's fate, not the owner's trigger. (deck126 G1)
`deck126-vs-deck152` seq 24: `Both halves of a life LOOP are on YOUR battlefield ...` ends on the
mechanism; the opponent-side clause ends "so ANY nonzero payment ... is fatal". The owner-side
could end from the owner's chair: `any row above that gains you life, or costs them life, ends
the game` — the seat held both halves three windows with the Tribute row on the menu (seq 24-27).
Guide F10 carries it meanwhile (skill #121).

### R67 — a cast row for a name already on the caster's line could say so. (deck126 G3; skill #125)
`Cast Exquisite Blood {4}{b} {you already control 1 - a second copy adds nothing}` for
non-stacking permanents; `deck126-vs-deck162` seq 12 (second Blood read as "both enchantments"),
four second-Lantern casts (`vs130` seq 34; `vs125` seq 73/78/100). The count alone is enough.

### R68 — `{paying this taps:}` never fired on Overgrown Battlement in six deck126 games. (deck126 G2)
Either the payment order never reached the Battlement (no `[tapped]` Battlement on any
opponent-turn line) or the clause is suppressed for defenders; if the latter, a tapped wall
cannot BLOCK, which for that deck is the whole cost — the clause should read "cannot attack or
block until your next untap" and fire for defenders. UNTESTED which; R64's receipt decides it.

### R69 — target-ask and X-row reading aids. (deck130 G2, G3)
When every creature target on a damage ask reads `SURVIVES`, say so once at the top (`no creature
on this list dies to 3 damage`; `130 vs146` seq 25 ignored the per-row form once). The Starstorm
cast row's annotation leads with the `smallest X that kills` clause; the owner's largest-first
ruling on the X MENU could order the cast-row annotation the same way.

### R70 — the counter-target row could carry the target's current size. (deck152 general; R39's sibling)
deck152's counter-spreading rule at its fourth corpus of ~2/13 (`vs126` s24 Briarbridge 6/5, s30
7/6); the guide lane is inert at 2/N (#45/#64); `{already 6/6}` on the Luminarch Aspirant /
Katilda target rows is the R25-family remedy.

### R71 — measurement: the game-log window is now the size driver in every prompt kind. (seat-engine, watch)
Ask mean rose 28% (8,162 -> 10,485) on three 50-60-turn games whose late prompts are 22-24 KB
of log; the residual above lane R's 15 KB blockers bar is 14.5 KB of log on a 1.8 KB combat
block. No prompt exceeded 35 KB; no change asked — report per-kind log share next wave, and
older-turn compaction is the next 10 KB if it moves latency.

---

## SHIPPED SINCE THE CORPUS — recorded, NOT docketed
None reported to this step beyond the four lanes above. Lane V (R48) is IN FLIGHT and is
docketed as such, not as shipped.

## GUIDE-ONLY — stays in guides after the render ships
- deck123's `PLAN: stop at M = <L + C + 3>; M is <M> now; this window: <x<N> / pass>` (A49-1;
  skill #120) and the own-upkeep timing rule (A49-2) — both stay correct after R61.
- deck125's gate-first plan sentence (W15; #120) and Emrakul counter-shape (W14; #123).
- deck126's Tribute-first win button keyed to the row (F10; #121), the pair-as-two-names (F11;
  #125), STEP 1 re-keyed to the parenthesis (F14; #126).
- deck130's post-sweep subtraction (D49-4; #122), self-target pass rule (D49-5; #124), upkeep
  Hammer rule (D49-2; #129), Starstorm-first-in-time (D49-3; #130).
- deck123's edict/sweeper re-keys to the row literal (A49-4/5; #121) — correct after R51/R59.
- deck146/152/162: unchanged this wave; every wave-48 rule held or was untested.

## MEASUREMENT items for the wave-50 reviewer briefs
1. Casts with `tapped == 2 x pips` per seat (R48), and — once R64 ships — `- Paid` lines per cast.
2. `answer_replaced` READ and split: rescued / rejection-executed / trailing-period-skipped (R54).
3. Plan-carry: longest identical run per seat, `The game is lost` carries, wrongful expiries of a
   correct loop-lockout plan (R56).
4. Narrated actions with no decision record, per seat (skill #127; R52).
5. Per-kind prompt chars and LOG SHARE (R71); no exclusions (21/21).
6. Offers by phase for every upkeep sink (skill #129) and windows by kind; UNTESTABLE-AT-THIS-SEAT
   flags (skill #132: deck146 146-A/146-B, deck152 152-B/152-D, deck162 162-B).
7. Quoted-literal grep of every guide against its seat's prompts (skill #128).

## PASS / KEEP — measured working, do not re-open
Lane R's B-row ranging/factoring (0 repeats > 2x, 0 sort straddles, 39/39 headers) · lane S's
name-vs-index validation (0 off-menu executed), truthful `answer_replaced` (10/10), last-line
ATTACK/BLOCKS (4/4 correct self-revisions), repeat grammar + `[RE-ASK]` (1/1 recovered,
`repeat_count_missing` 0), index-past-menu `[RE-ASK]` (1/1 recovered, class 4/5 -> 0), the exit
row (525) and `kCastAnsweredFact` (125) · lane T's edict row (123 rendered, 0 casts at N=0;
Tribute-at-1 4/4 at deck126), sweeper row (0/87 on `destroys 0`), `{paying this taps:}` /
`{tapping X now:}` / `[this land is TAPPED:]` (Katilda 0/7 in Upkeep, tapped Hive passed) · lane
U's LOOP clause (223 prompts, `152 vs126` ATTACK: none twice, 0 "net 0"), `[DRAW PRICE:]` on the
opposing cast-trigger class (48), `DRAW FORECAST` on the punished seat (98/98), `(text omitted)`
177/183 · lane N's pass row and receipts · land drops 0 declines at five seats · the pregame
no-plan ask · **one render falsehood this corpus (R51, Devour Flesh's gain recipient)** —
everything else open is an omission, an enumeration, a counter, or an engine defect.
