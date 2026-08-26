# Wave-45 engine ledger — THE WAVE-46 DOCKET

Corpus `matchups-20260826-131833`, binary fd69fadc2 (four step-1 lanes merged; PARSETEST
1241/0), serve FP8-35B `qwen36-35b-a3b` @ spark:8084, pool 146/152/125/126/162/123/130,
-j 21, no cap, timeout 900, maxtokens 6000. 42 seat logs, 21 games, **1909 records / 1825
decisions**, fallbacks **2 / 1825 = 0.11%** (w44 0.28%, w43 0.17%). Games ended turn 10-54,
mean 20.0. Total inference 11.25 h.

Deduped across the four seat files (`seat-123-130.md` M1-M3/L4-L5, `seat-125-126.md` HIGH#1 +
MED/LOW, `seat-146-152-162.md` E-1..E-5, `seat-engine-narration.md` NEW-1..NEW-10) and the seven
`deckN/general-suggestions.md`. Where two seats filed the same locus the entry carries both
repros. The layer-routed argument and the wave-46 prediction for each ranked item live in
`general-strategy.md` (R-numbers below cross-reference it); this file is the docket.

Emitter authority: every render verdict was adjudicated against the literal string in
`projects/mtg/src/AIPlayerGPT.cpp`, not a lane nickname (skill #44). Synthesis independently
re-verified the two HIGH emitter claims (R1 at :1318-1345 / :2415 / :8222-8236, R2 at :1515-1522)
and the E-3 residual build path (:12716-12743).

---

## RANKED DOCKET

### HIGH

| id | item | locus | repro |
|---|---|---|---|
| **#W46-1** (R1) | `Mana available:` prints a colour SET, never a per-colour SOURCE COUNT — while `pregameHandHeaderText` already prints the count shape and stops at the opening hand. Ship `(sources that can make each: {G} 3, {B} 1, {W} 2)`; state the dual-counts-twice and variable-source-counts-once conventions IN the clause | `AIPlayerGPT.cpp:2415` `manaAvailableLine()`; data at `:8222-8236`; existing count shape at `:1326` | `1787768348-ai_baka_deck126-0x55d603332df0-vs-ai_baka_deck130.jsonl` **seq 30-34** (t14, life 6 -> 1, game lost) |
| **#W46-2** (R2) | A lost card and a held card are the same sentence: the zone-change history line has no copy disambiguator and `copyOfTag` returns "" once `total < 2`, so the two surfaces read as a contradiction the model must arbitrate — the trust doctrine's own failure mode | `AIPlayerGPT.cpp:1515-1522` (`copyOfTag`), zone-change history emitter | `1787768324-ai_baka_deck146-0x55ab501e54a0-vs-ai_baka_deck162.jsonl` **seq 26** (539.0 s on a 2-option land drop; **seq 24** discard line, **seq 2** both draws) |
| **#W46-3** (R3, = carried #W45-6) | Opponent non-creature permanents carry NO text on any decision surface, and the one line that explains a converter fires only on ATTACKERS windows — a creatureless control seat never reaches it. Third wave carried; first wave with a fully assembled loop as the specimen | opponent battlefield-line emitter; converter summary scope | `1787768318-ai_baka_deck125-0x55ed9b6ba460-vs-ai_baka_deck126.jsonl` **seq 67-70** (Sanguine Bond + 2x Exquisite Blood + Staff of Nin, textless; deck125 died from 30) |

### MED

| id | item | locus | repro |
|---|---|---|---|
| **#W46-4** (R4) | ANNOUNCE_X has no preview for any non-damage X spell (12/14 windows bare), and X=0 is never called out as a null cast — 3 of those 12 answered X=0 with 4/5/6 affordable | `xAnnounceRowKills` / `sv.priceable` gate | `...deck125-0x55ed9b6ba460-vs-deck126` **seq 58, 64**; `...deck125-0x562f799014b0-vs-deck123` **seq 30**; control `...deck130-0x559cc3d27570-vs-deck123` seq 21 |
| **#W46-5** (R5) | The BLOCKERS screen prices only 1-on-1; the lane-A `GANG BLOCK:` mirror clause does not exist there, and the pilot re-derived the group math by hand against the prompt's own "do not re-derive" doctrine | blockers prompt emitter (mirror of `gangBlockPriceTag`) | `1787768352-ai_baka_deck123-0x56177edb6e40-vs-ai_baka_deck152.jsonl` **seq 47** (13 rows, all `(your blocker dies, attacker lives)`, ~250 words of hand arithmetic) |
| **#W46-6** (R6) | CHOOSE_MENU `ctx` missing on TargetChooser SIDE sub-asks — all 9 surviving subject-less headers are Teferi's +1 side chooser; the same walker's loyalty menu names itself 15 times. 0 fallbacks: legibility, not parse risk | `AIPlayerGPT.cpp:12716-12743` (generic fallback at :12743) | `1787768338-ai_baka_deck152-0x55bef9967010-vs-ai_baka_deck126.jsonl` **seq 21** (+24, 38, 41); `...deck152-0x55b6b7adf0d0-vs-deck125` seq 24, 42, 55, 76, 87 |
| **#W46-7** (R7, = carried #W44-9) | Mana-only priority windows auto-pass. Recount kills the wave-44 "game composition" verdict: **39/188 decisions (20.7%) and 39/46 priority windows (85%)** at deck126, across THREE games. Gate must be "no legal action but mana production AND no pending cost" | priority-window gate | `...deck126-...-vs-deck125` seq 12,16,19,20,22,24,26,27,30,32,33,35,36,38; `...vs-deck162` seq 5,8,9,10,14,15,16,19; `...vs-deck130` seq 8,13,16,17,19,20,21,25,26,27,28,32,33,34,36,37,38 |
| **#W46-8** (R8) | `{X pricing: same as X=N}` points UP, so the cheaper equal X reads as derivative. The corpus's only plan-vs-chosen-X divergence is exactly this | `xKillRowCore` collapse wording | `1787768342-ai_baka_deck130-0x55f269ac9490-vs-ai_baka_deck146.jsonl` **seq 17 -> 18** (plan X=1, answered X=2) |
| **#W46-9** (R9) | A benefit inside a fatal parenthesis (`(your attacker dies... (lifelink: you gain 3))`) reads as an addition — bind the number to its branch | `combatTradePreviewStats` keyword tail | `1787768326-ai_baka_deck152-...-vs-ai_baka_deck162.jsonl` **seq 28** (reply adds +6 life to 6 face damage) |
| **#W46-10** (R10) | Activated-ability state belongs on the option row: `becomes beholder` is byte-identical whether the Hive is a land or already a 3/3. 78 offers, 27 taken, **10 redundant**, 18 in upkeep, 6 double-in-one-chain. Skill amendment 65 routes the guide half here | activated-ability option-row emitter | `1787768353-ai_baka_deck146-0x55bb5ae9e2f0-vs-ai_baka_deck125.jsonl` **seq 98, 100** (t30); doubles at seq 87+88, 124+125, 133+134, 147+148, 154+155, 160+161 |
| **#W46-11** (R11) | Stale-plan note is a nag on standing-strategy plans: **129/261 fires (49%) repeat on byte-identical plan text**; deck125's mandated plan string alone accounts for 136. REFINEMENT of a lane that PASSED — do not reopen #W45-4 | `kStalePlanNote` gate, `AIPlayerGPT.cpp:265` / `:6667` | `1787768318-ai_baka_deck125-0x55ed9b6ba460-vs-ai_baka_deck126.jsonl` **seq 28, 30, 31** |
| **#W46-12** (R12) | The plan-vs-AFFORDABILITY contradiction the note cannot catch: plan names a card the menu DOES offer and the seat cannot pay for. Strictly decidable; fire only on exact card-name match with mana value > printed total | new clause, same channel as `kStalePlanNote` | `1787768332-ai_baka_deck123-0x564b59dd4a40-vs-ai_baka_deck126.jsonl` **seq 12-24**; `...deck130-...-vs-deck123` seq 27 |

### LOW

| id | item | locus | repro |
|---|---|---|---|
| **#W46-13** (R13) | Name the cost of declining the land drop, once, on the row — never forbid the choice | land-drop ask emitter | `...deck123-...-vs-deck126` seq 12-24 |
| **#W46-14** (R14) | `gangBlockPriceTag` ignores the declaration-set minimum: suppressed at `need < 2` even when menace makes 2 bodies legally mandatory, so the cheapest LEGAL lethal gang is never priced. Floor `need` at `max(2, minBlockersRequired())` | `gangBlockPriceTag` | `1787768324-ai_baka_deck146-0x55ab501e54a0-vs-ai_baka_deck162.jsonl` **seq 27** |
| **#W46-15** (R15) | The modal row does not say which path disposes of the card: `1. Cast Card Normally` carries no outcome and no pointer to the X menu behind it | modal menu emitter | `1787768330-ai_baka_deck130-0x559cc3d27570-vs-ai_baka_deck123.jsonl` **seq 20** |
| **#W46-16** (R16) | Lexicon leaks reach live surfaces: `Transform:backside` in 141 prompts incl. **3 numbered OPTION ROWS**; `ability: teferieffect Counter Removed` on the STACK in 31 | ability-label emitter (needs a display-name map) | option rows: `...deck123-...-vs-deck130` **seq 37**, `...deck123-...-vs-deck146` **seq 15**, `...deck123-...-vs-deck125` **seq 55**; stack: grep `ability: teferieffect` |
| **#W46-17** (R17) | `dropped_assignments` conflates `Bn:none` padding — corpus reads 7, true count 0, all from one record; the metric has zero signal until split | blockers reply parser metric | `1787768326-ai_baka_deck162-0x560dd1e6c400-vs-ai_baka_deck152.jsonl` **seq 14** |
| **#W46-18** (R18) | The `[held back, it CANNOT block ...]` tag has no converse on the attack row for a seat's ONLY untapped creature | attackers row emitter | `1787768342-ai_baka_deck130-...-vs-ai_baka_deck146.jsonl` **seq 21, 25** |
| **#W46-19** | Singular loyalty `(now N)` and the bare `[defender]` gloss — carried unchanged, no new evidence this wave beyond re-observation | render | carried from wave-44 MED docket |
| **#W46-20** | No LETHAL headline on the ATTACK side (the block side has one) — carried, no new evidence | render | carried from wave-44 MED docket |

---

## SET A — wave-44 GUIDE EDITS, discharged per deck

Deployed as `5cd6a860b`. **Wave-45 games loaded the PRE-edit guides for the three post-launch
changes** (deck125 carve-out removal a9bd0ce21, deck146 one-land odds rule de9a1d6dd/c01ece06a) —
those are excluded and re-predicted for wave 46.

| deck | edit / prediction | verdict | counts |
|---|---|---|---|
| 123 | EDIT A / P1 wall-board `ATTACK: none` | **UNTESTED** | 0 arming windows in 13 attack windows (2nd consecutive null — re-key to a probe deck per skill #21 if wave 46 is also 0) |
| 123 | EDIT A / P2 Check 3, no false holds | **PASS** | 0 `ATTACK: none` of 13; wave 44's 11-attackers-held-against-one-blocker did not recur |
| 123 | EDIT B / P3 lethal biggest-first, menace | **UNTESTED** | 4 blockers windows, none LETHAL-flagged, none with a menace attacker |
| 123 | EDIT K Lightning Greaves gate | **FAIL** | 3 of 6 casts at "of which 0 are creatures" (vs130 s5, vs162 s8, vs152 s5); wave 44 was 0/35 |
| 123 | EDIT L / P4 edict N-check | **PASS-with-1** | 63/64; the miss (vs126 s10, N=2, both `[defender]`, named "The opponent" into a Sanguine Bond deck) is a triple error |
| 123 | 12-creature token stop | **FAIL** | 10 violations (vs162 s45/47/51, vs146 s28/36/37/38/41/42, vs152 s45), 0 in wave 44; all ten odds-favourable, two of three games WON -> **VIOLATED-UNPUNISHED** (skill 64). Cap DECIDED below |
| 123 | EDIT M spend every blocker | **UNTESTED** | no lethal-flagged blocking window |
| 125 | W10 zero sweeper casts at 0-creature header | **FAIL, 1 of 9 casts** | 42 zero-creature windows, 41 declined; the one violation is in MAIN PHASE 2, which the section never names (vs126 s73). Wave 44 was 3/14 — the hoist moved the rate hard |
| 125 | W11 X ceiling + no cast at hand >= 7 | **FAIL both halves** | library ceiling 12/12 conformed; hand ceiling violated 6/12; 6/12 casts at hand >= 7. **Largest-offered-X fell 14/14 -> 4/12** — the "biggest number" pull is gone; what remains is two subtractions (skill 48) |
| 125 | W12 counter engine-text creatures | **PASS and overshot** | 33/33 counter windows taken (w44 39/45 with 3 wrong declines); the new failure is 4 of 33 spent on `[defender]` bodies, 2 of them the last counters in a game then lost |
| 125 | W13 Elixir at library <= 16 | **UNTESTED** | offered 10, taken 8, library never below 23 in an offered window — the eager activation is why the threshold never armed. 0 decking losses |
| 125 | W14 Staff leaves cheapest counter payable | **FAIL, 3-4 of 7** | n=7, reported directionally (skill #45): no better than wave 44's 6/11, denominator cannot separate |
| 125 | W15 mulligan carve-out | **VOID** | removed post-launch |
| 125 | Emrakul-attacks clause (w44 carry) | **PASS 2/2** | vs146 s93, s99 (t52, t54) — game-winning |
| 126 | D11 Bond-alone clock | **PASS 3/3** | vs123 s26, vs152 s14, vs130 s42 — no pass-ups |
| 126 | D12 survival floor 7b (life <= 10 + one creature + Tribute + mana) | **UNTESTED** | 0 of 39 Tribute windows matched all four conjuncts. The condition that actually arose was a KEYWORD, not a life total (see the vs162 loss) |
| 126 | D13 Chromatic Lantern gate | **FAIL 2/14 — and the RULE is the defect** | both casts correct play; `colours you can make:` answers presence, the decision needs a COUNT (skill 59). Retired, replaced by a two-black-LANDS test |
| 126 | D14 blocker assignment | **PASS** | 8/8 offered blockers assigned in 5 windows, 0 `BLOCKS: none`. Small denominator (w44: 15 windows) |
| 126 | D15 Idyllic Tutor with neither piece | **PASS 2/2**, tutor picks **7/7** | vs125 s8/s47, vs123 s9/s11/s30, vs162 s13, vs130 s30 |
| 126 | Savannah / Battlement colour facts (F3) | **PASS** | re-verified against primitives this wave: Savannah `subtype=Forest Plains`, Battlement `{T}:foreach(creature[defender]|myBattlefield) add{G}` |
| 130 | EDIT 1(a) / P5 `YOURS:` names the clock | **UNTESTED** | 0 arming windows |
| 130 | EDIT 1(b) / P6 `THEIRS:` all `[defender]` | **PASS 1/1** | vs126 s18 — declined X=4 killing five `[defender]` bodies, cast Rorix, won |
| 130 | EDIT 2 hold Starstorm once Rorix/Siege-Gang out | **PASS 4/4** | vs126 s27, 31, 33, 38; won turn 17 |
| 130 | EDIT 3 / P7 face burn above 6 life | **PASS 5/5** | vs123 s12/16/19/22/23 at opponent life 19, 0 fired (wave 44: 9/9 violations). 18/18 target rows went at a permanent |
| 130 | EDIT 4 / P8 lethal chump biggest | **PASS in effect, LETTER FALSIFIED** | vs162 s19: the biggest attacker was tagged unblockable; the pilot blocked the only legal one and lived at 3. Rule needs the legality qualifier (skill 59) |
| 130 | EDIT 5 cycling rung | **FAIL 1/1** | vs123 s20: "Cast Card Normally" taken at the guide's own release state, X=0 announced; game lost |
| 130 | EDIT 7 lethal-chain exception | **UNTESTED** | 2nd consecutive corpus with no window |
| 130 | EDIT 8 backstop retired | **PASS** | 28/28 owner-tagged LD rows, 0 self-hits |
| 130 | P9 plan-vs-chosen X | **PASS-with-1** | 1 of 2 at the seat; corpus 1 of 4 windows naming an X (w44: 2 of 18) |
| 146 | P1 `(neither dies)` into 2+-defender board | **SPLIT: letter FAIL 1, mechanism PASS** | the one send was Nadaar (vigilance) into two Walls of Omens — primitive-verified NO block trigger, so the cost was ZERO. Metric restated (M-a). Secondary PASS: block-trigger life in a deck146 game = 3 (w44: 12-13 in single turns) |
| 146 | P3 Hive already-on / upkeep | **FAIL** | 10 redundant (was 12), 18 upkeep (was ~19) of 27 taken / 78 offers -> routed to render (#W46-10, skill 65) |
| 146+152 | P2 LETHAL with fewer blockers than attackers | **UNTESTED** | both windows arithmetically unsurvivable; the model took the max-damage-stopped assignment both times, which is what the gate orders. The satisfiable shape recurred on **deck162**, which has no gate |
| 146 | S3 Shield Sphere named exception | **PASS 5/5** — now LIVE | vs162 s8/11/16/23/27, proof on the board line: 0/6 -> 0/5 -> 0/4 |
| 146 | dungeon LOST MINE 8/8; room body-or-drain 14/14; "targets are YOUR OWN" 27/27; Agadeem's never cast 21/21 | **PASS** | all earning their tokens |
| 146+152+162 | P4 per-creature `your attacker dies` STOP | **FAIL 0 of 7**, 5 cards died | THE CROSS-DECK HEADLINE — three guides, three loopholes (skill 53, 54, 55) |
| 152 | deploy floor | **PASS 33/33** (w44 45/47) | |
| 152 | P6 counter placement | **PASS both halves** | 5 placements, 0 onto a Fog-Bank-walled body (was 8 consecutive), 2 onto a flier |
| 152 | P5 Fateful Absence on "targets are YOUR OWN" | **UNTESTED** | 0 windows; deck146's parallel render-string rule ran 27/27 |
| 152 | counters-into-Fog-Bank | **PASS** — superseded by a CONCENTRATION failure | 7 counters onto one Aspirant which then died to a gang block (skill: connection then concentration) |
| 162 | P7(a) all-FREE tags sent | **PASS 2/2** | S6 restoration validated |
| 162 | P7(b) collapsed tag N>=2 -> none | **FAIL 0/1** | vs123 s23 — **VIOLATED-UNPUNISHED**: opponent lone-blocked, attacker lived, game won. Rule stands, gains the GANG BLOCK string |
| 162 | P7(c) attack rate above 2-of-6 | **PASS** | **12 attack windows, a send in all 12** (was 2 of 6) — skill #35/#36 confirmed by the repair |
| 162 | P8 Rule-2 engine brake | **FAIL, 3 in the exact named shape** | overall 22/26 (w44 13/17); game won regardless — reported directionally, brake NOT loosened (skill #45) |
| 162 | P9 bottoming names no Shield Sphere / punisher | **PASS on the letter, FAIL on the rule's other half** | bottomed a LAND from a 5-land keep; the rule's unqualified "never bottom a land" is the defect (skill 67) |
| 162 | Rule 1 punisher-first | **17/20** (w44 13/17) | the single best per-deck priority metric in the pool; the argument for skill 62 |

**Set A totals:** 15 PASS, 4 PASS-with-1 / PASS-in-effect, 10 FAIL, 9 UNTESTED, 1 VOID, 1 SPLIT.
Of the 10 FAILs, **3 are the guide text's fault, not the pilot's** (126 D13, 130 EDIT 4's letter,
162 P9's other half) and are recorded as evidence against the rule per skill 59/64.

---

## SET B — wave-45 ENGINE LANES, discharged per lane

| lane | verdict | counts |
|---|---|---|
| **#W45-1** menace block side (lane A, e4589e7d1) | **PASS on all three legs — low N** | Exactly ONE blockers window in 1,825 offered a menace attacker (`...deck162-...-vs-deck146` seq 19). Bare 1-on-1 verdicts on menace B-rows **0/3**; `zeroPowerBlockerTag`'s `stops NOTHING there` branch **2/2 eligible** (its only 2 firings); solo menace blocks **0**; menace-caused `dropped_assignments` **0**. Attack side re-verified: 16 menace A-lines / 13 windows, all tagged, 13 correctly carrying no blockers tag, no over-claim. Wave-44's 8/8 defect is gone. **One window is not a rate — the Baka-side probe deck is still owed** |
| **#W45-2** GANG BLOCK / collapsed tag (lane A) | **RENDER PASS; BEHAVIOUR restated** | 231 A-lines, 97 with a blockers tag, **4 collapsed**, **17 with a GANG BLOCK clause**. Priceability audit by re-running the caller's arithmetic over 52 enumerated multi-candidate A-lines: **15/15 priceable lines priced, 0 silently skipped, 0 clauses the arithmetic did not support** — this answers the carry's open measurement. 2 of 4 collapsed lines priced, 2 correctly silent. Behaviour on the lane's own mechanism (priced gang): **0/2 attacker deaths -> PASS**; on the literal metric (any collapsed tag): 1/4, and that death was a truthfully-predicted lone-block, so the metric is restated (M-a). `gangOk` deathtouch/wither/infect/prevention suppressors: **UNTESTED**, no candidate had one |
| **#W45-3** block-trigger life + converter (lane B, 56e852a78) | **PASS — FIRST LIVE OBSERVATION of the TargetChooser leg** | Tag renders in all three voices incl. the scripted `may`: `(blocking trigger: you gain 3)` (126 vs146 s9), `(blocking trigger: they gain 3)` (146 vs126 s7), `(blocking trigger: they may gain 2)` (130 vs126 s19 — Perimeter Captain's trigger correctly attributed to a Wall of Omens' pairing, both primitives verified). Converter line **5 prompts**, correctly scoped (absent at s35 with the Bond in hand, present at s42 after it resolved). Block-trigger life in any deck146 game **max 3 < 6 -> PASS** (wave 44: 13 -> 42 in one game). Residual: converter scope, -> #W46-3 |
| **#W45-4** plan-mismatch note (lane D, 6b5c7d93c) | **PASS on every leg** | Fires **261/1909 = 13.7%** (bar <25%; lane D estimate ~12.5%; wave 44 42.8%). **Land-drop fires 0/374** (was 327/419) and EVERY `castFreeAskHeader` family at zero (target 0/60, mode 0/57, mulligan 0/54, sacrifice 0/18, room 0/14, ANNOUNCE_X 0/14, `A choice is required` 0/9, venture 0/8, exile 0/7). **Strict false fires 0/261, tested on ALL 261, not a sample**; precision 45/45 on a seeded hand-read + 49/49 on an independent seat audit. Residual is usefulness -> #W46-11 |
| **#W45-5** ANNOUNCE_X kill facts (lane C, 4a4df6ee4) | **PASS on the priceable class; TARGETED class UNTESTED (2nd wave)** | 14 windows: 2 priceable (both Starstorm), **6/6 rows annotated** with full or collapsed form; 12 Sphinx's Revelation, correctly bare (`auto=life:X && draw:X`, nothing for a damage pricer) — verified at the `sv.priceable` gate, not assumed. Casts at an X whose `YOURS:` names the seat's clock **0/2** (w44 2/2), but arming windows were 0 — a clean sheet, not a discharge. Plan-vs-chosen X divergence **1/4 windows naming an X** (w44 2/18); mechanism is the collapse wording -> #W46-8. `"(one target only)"` phrasing still UNTESTED. Coverage gap for non-damage X -> #W46-4 |
| **E-3** subject-less modal headers (dd8bdb503) | **PASS on the modal class** | 57 `Choose an option for <spell>:` + 5 `Choose one mode for <spell>:`, **0 subject-less**, **0 `stale_echo` anywhere in the corpus**. The 9 residual `A choice is required` headers are a DIFFERENT emitter branch, localized and named -> #W46-6 |
| **Lane E** (#W45-20, 607ca6bb7, merged 2d5954da0) | **NOT IN THIS BINARY — baseline recorded** | 1,897 unique history lines / 8,386 instances; **355 unique / 586 instances** open on neither a player subject nor a possessive. `It became Day` / `It became Night` (14 instances) exempted by nature. Mirror-ambiguous OBJECT-side specimens now on the record: `Silverquill Silencer dealt 3 damage to Shield Sphere` (x10), `Nadaar, Selfless Paladin dealt 4 damage to Wall of Omens` (x4) |
| **Fair hands** (lane A, carried) | **PASS, 2nd corpus** | 42 openers, land counts 0-6 (0,1,1,1,1,2,2,2,2,2,2,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,6 across the recorded seats), 8 mulligans, real London chains with correct running "having already taken N mulligans" text, real `bottom` asks citing CR 103.5, no aggressivity-pin artifact |
| **#W44-4** Day/Night (carried FAIL) | **NOW PASSES** | 75 prompts carry `It is currently Day.` / `It is currently Night.` The wave-44 0/2035 was a shorthand grep — skill #44 in action for the second wave running |

---

## FALLBACK CLASSES — 2 / 1825 = 0.11%, both at one seat, both mechanisms named

| class | count | record | mechanism |
|---|---|---|---|
| `retracted_choice` | 1 | `1787768352-ai_baka_deck123-0x56177edb6e40-vs-ai_baka_deck152.jsonl` **seq 22** (t11, 4 options, 237.5 s, `post_answer_overrun` 3172) | The reply's FIRST line is a well-formed `CHOICE: 3 (Cast Idyllic Tutor)`; the PLAN line then argues the seat out of it verbatim and never emits a replacement. `commit_retracted: true`, `choice: -1`, Baka took over. The known no-decode-stop / b2 shape; the parser rejected it correctly. **Reply-protocol seam** |
| `unparsed_reply` | 1 | same file **seq 61** (t17, priority, 6 equip rows, 154.5 s) | Degenerate multilingual token soup, no `CHOICE:` anywhere, on an unremarkable menu. **Model/serving-side generation collapse, not a prompt defect.** PINNED: a second occurrence makes it a serving item |

Baseline movement: 0.17% (w43) -> 0.28% (w44) -> 0.11% (w45). Reported directionally (skill #45);
n cannot separate these. **stale_echo: 0 corpus-wide** — the E-3 prediction's strongest leg.

Also pinned, not a fallback: the corpus's longest decision (539 s, #W46-2) and one 326 s / one
281 s reply at deck123. `answer_replaced` 0 and `commit_retracted` 0 at four of the seven seats.

---

## CARRIED, unchanged (no new evidence this wave beyond re-observation)

- #W46-19 singular loyalty `(now N)`; bare `[defender]` gloss.
- #W46-20 no LETHAL headline on the attack side.
- **Baka-side menace gang-block behaviour** — RED PIN, probe deck still owed. The corpus's single
  menace blockers window was a GPT seat, so it says nothing about Baka. Third wave carried.
- **Trample carve-out on block-ladder rung 3** — SIXTH consecutive null. Recorded
  unreachable-in-pool per skill #21; not deleted, and not re-filed as a prediction.
- **Targeted-X `"(one target only)"` phrasing** — second wave with no targeted X spell reaching
  an ANNOUNCE_X menu. Same treatment: probe or harness, never another corpus wait.
- `[combattrace] ... suppressed by unresolved-stack` fires **67 times** corpus-wide. One instance
  was checked per-instance and is benign (the seat controlled 0 creatures at that tick, so
  `hasLegalBlock` was false regardless). The other 66 need the same per-instance test before
  anyone dockets them; recording the count so it is not later read as 67 skipped blocks.

---

## DECK-CONTENT QUESTIONS THE SEATS ROUTED TO THE OWNER — DECIDED HERE

Per the wave-45 brief and skill amendment 66: there is no owner channel for deck strategy, and a
question parked there is a question nobody answers. Each is decided from the corpus, with the
rationale recorded so a later wave can re-derive it (skill 61). **These are decisions for the
DECK REVIEWER lane to apply — synthesis owns the skill and the general strategy, not the guides.**

### DECISION 1 — deck123's 12-creature token cap: RAISE TO 20, with a recorded WHY and a
sunset test.

*Evidence.* 10 violations this corpus (vs162 s45/47/51 at counts 12/13/14; vs146
s28/36/37/38/41/42 at 12..17; vs152 s45 at 12) after 42/42 and 64/41 obedience in earlier waves.
Every one of the ten was at 16 life with no lethal on the board, and every one bought a free
flying token for a deck whose only plan is bodies. Two of the three games were WINS. No
wave-40..44 artefact records what the 12 is FOR, and the obvious candidate is excluded by
measurement: prompt size is not it — the seat's largest prompt this corpus is 19,155 chars and
the ranged collapse (`Human #2-#11 (1/1) ... x10`) keeps wide boards flat.

*Decision.* Raise to **20**, not delete. WHY, in the form skill 61 now requires: *T = 20 because
the ranged-collapse render holds a 22-permanent board to ~19k chars, so board width is not
costing decision quality below that, and every violation of T = 12 was odds-favourable.* Deleting
outright fails skill #35's sufficiency test today — the cap is the only rule in the section that
ever says stop, and #35 was written off exactly this shape (a low-frequency, high-cost gate cut
on a token-cost argument, falsified twice in six games). Raising is the bounded move that keeps a
stop while removing the range where it is provably wrong.

*Sunset.* If wave 46 shows 0 violations at 20, no prompt-size cost and no decision-quality cost,
wave 47 deletes it under #35 with the deletion recorded as a prediction (#20).

*Prediction.* Token-line takes at a creature count of 12-19: unconstrained and expected to rise;
takes at 20 or more: 0/N. Largest deck123 prompt stays under 25k chars.

### DECISION 2 — the mulligan chain: HARD STOP AT THE SECOND LOOK, in every guide.

*Evidence.* 8 mulligans in 42 seats. **Two seats reached FOUR cards, and both lost.** deck123
vs126 took three, then bottomed Damnation, Vision Skeins AND Idyllic Tutor — both of its ways to
find a token-maker — keeping three lands and an Intruder Alarm its own guide calls a card that
does nothing alone; it never put a creature on the battlefield and lost 42-0 on turn 25. deck146
vs123 took three from a `(keeping 5)` window holding two Plains and a castable Nadaar {2}{W}, in
flat violation of its own STEP 1, and lost at -41. No mull-to-zero, no repeated mull elsewhere.

*Decision.* Every guide's mulligan section carries a hard stop: **at `(keeping 5)` or lower, keep
any hand holding at least one land and read no further.** The London mulligan already lets a
7-card look be shaved to 6, so a third look buys a fresh seven at the price of two cards where
the shave costs one. deck146's STEP 1 is already exactly this text and its ONE violation is the
seat's worst loss — that is evidence FOR the floor, and skill 66 forbids moving it on one window
in either direction, so it is not moved, it is propagated. Guides currently missing the stop:
**deck125, deck152** (see the boundary findings in `synthesis-notes.md`).

*Prediction (skill 66/67).* No seat reaches `(keeping 4)`: 0/N (wave 45: 2). No mulligan taken at
`(keeping 5)` or lower with a land in hand: 0/N (wave 45: 1, and it lost at -41).

### DECISION 3 — mulligan FLOORS themselves: not moved, but re-formed.

*Evidence.* Deviations ran in both directions and the odds ran against the guide about as often
as against the pilot: deck125 shipped a 3-land seven whose cheapest spell was mana value 4 and
whose coverage line read `would not cover any spell in it` (odds favour the ship, floor says
keep); deck146 shipped a 2-land seven with the same shape; deck146 vs123 s1 kept-then-shipped a
2-land `{B}{B}` hand with a real curve off it (odds against the ship); deck162 bottomed a LAND
from a 5-land keep against an unqualified prohibition (odds favour the deviation).

*Decision.* No land-count band is moved (skill 66's anti-n=1 core stands). The FORM changes: the
keep test is written as a comparison over the three fields the engine already prints — the
land/spell counts, `Mana sources among those lands`, and the `Playing every land in this hand
would cover the cost of:` coverage line — with the land count as the first, cheapest branch. Five
of seven guides already do this; deck130 does not use the coverage line and deck126 uses it only
as a secondary keep. Bottoming rules take deck146's land carve-out ("never bottom a land unless
the hand you keep would still hold at least four") plus a floor on castable spells.

*Prediction.* Every mulligan taken is on a hand whose engine count is 0 or 1 land, or 2 lands
with no spell reachable before turn 4: target 100% (wave 45: 5 of 8). A ship of a hand whose
coverage line NAMES a card is a FAIL of the new text, not of the pilot.

---

## FOR THE ORCHESTRATOR TO ASK

**None.** Every question the seats raised is either a deck-content question (decided above and in
`synthesis-notes.md`) or an engine/render item (docketed above). No loop-rule question — process,
render-shape convention, or scope — is open. One process note, recorded not asked: the wave-45
brief's own routing (reviewer owns the guide, synthesis owns the skill and the general strategy)
is what makes the three decisions above deliverable as findings rather than edits, and the
boundary findings are written to the reviewer lane accordingly.
