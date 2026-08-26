# Wave-46 engine ledger — THE WAVE-47 DOCKET

Corpus `matchups-20260826-155842`, binary master `486c83189` (wave-46 lanes F/G/H/I +
wave-45 lane E; PARSETEST 1332/0), serve FP8-35B `qwen36-35b-a3b` @ spark:8084, pool
146/152/125/126/162/123/130, -j 21, **NO CAP**, timeout 900, maxtokens 6000. **42 seat logs,
21 games, 2113 decisions, 21/21 NATURAL** (0 timeouts, 0 draws, 0 cap adjudications, 0
crashes). Games ended turn 10-64. Fallbacks **6 / 2113 = 0.28%** (w45 0.11%, w44 0.28%).
Prompt cost **+17.4% chars/decision**, post-answer overrun **-17.6%**, p99 latency
**-17.2%**.

Deduped across `seat-123-130.md` (H1-H2, M1-M3, L1-L4), `seat-125-126.md` (HIGH #1, MED
#2-#3, LOW #4-#5), `seat-146-152-162.md` (E-1..E-5) and `seat-engine-narration.md`
(E-1..E-8), plus the seven `deckN/general-suggestions.md`. Where two seats filed the same
locus the entry carries both repros. The layer-routed argument and the wave-47 prediction
for each ranked item live in `general-strategy.md` (R-numbers cross-reference it); this
file is the docket.

**Emitter authority (skill #44).** Every render verdict was adjudicated against the literal
string in `src/AIPlayerGPT.cpp` / `src/DecisionContract.cpp`, never a lane nickname. The
engine seat verified `git diff 486c83189 HEAD -- src/AIPlayerGPT.cpp src/DecisionContract.cpp
include/GptPlanCaveat.h` was EMPTY at review time, so the tree it read IS the corpus binary.
Synthesis independently re-read `manaAvailableLine` (:2530), `describeZoneCards` (:3009,
call sites :8642 own / :8647 opponent), `boardEffectSnippetLen` (:355), `converterSummaryText`
(:8319), `gangBlockPriceTag` (:2299), `kRepeatActivationFloor` (:8974) and the CHOOSE_MENU
header ladder (:13109).

---

## SHIPPED SINCE THE CORPUS — record as discharged-at-ship, DO NOT re-docket

On master as `a7310f01c` + the pending hand-count commit; all four verified present in the
tree by synthesis at the loci named. Each carries a wave-47 prediction in
`general-strategy.md` under **ALREADY SHIPPED**.

| id | item | verified at |
|---|---|---|
| **S1** | CHOOSE_MENU header name ladder -> `resolveOwningCardName(ctx)`; the E-3 rescue re-gated on the NAME being unresolvable instead of a NULL pointer (Silverquill Command's `auto=choice` carrier) | `AIPlayerGPT.cpp:13109`; `DecisionContract.cpp` ~:370 |
| **S2** | Ask exemplar drawn from THIS window's option 1 (`askExemplar(options)`) — removes the `Cast Example Card` latch from cast-free menus | `AIPlayerGPT.cpp:11207-11210` |
| **S3** | `kRepeatActivationFloor = 1` — the FIRST repeat carries the `[repeat: ...]` tag | `AIPlayerGPT.cpp:8974`, gate `:9033` |
| **S4** | In-game `Your hand (N cards):` count on the pilot's own hand line | `AIPlayerGPT.cpp:8603` (pre-game shape `:1381`) |

S1+S2 together account for **4 of the corpus's 6 fallbacks**.

---

## RANKED DOCKET

### HIGH

| id | item | locus | repro |
|---|---|---|---|
| **R1** | No **DRAW-PUNISHER summary line**, while the mirror class has one (`converterSummaryText`). The facts exist only as scattered `{effect: "Whenever an opponent draws ..."}` entries and the cost never reaches the row that causes it (a `cycling` row prints mana and nothing about the draw). Ask: `DRAW PUNISHERS on the battlefield:` in CURRENT SITUATION, summed from `@drawfoeof(player):damage:N`; cheaper half, per-draw cost on the drawing row | new summary beside `converterSummaryText` (`AIPlayerGPT.cpp:8319`); row-level annotation on draw options | `1787777944-ai_baka_deck130-0x5634cf21aa60-vs-ai_baka_deck162.jsonl` **seq 27** (life 3, three punishers listed, cycled, dead at 0 same turn); `1787777964-ai_baka_deck125-0x55a7d1167450-vs-ai_baka_deck162.jsonl` **seq 36** (life 6, Revelation X=1, drew to 4) |
| **R2** | The **`Choose an option for <card>:` modal menu is the only bare menu in the loop** — `1. Cast Card Normally` / `2. cycling` / `3. Decline`, no cost, no effect, no draw — between a cast row carrying `{X pricing:}` and an announce menu carrying a per-row annotation. Same seat, same card, same loss shape, TWO corpora; guide lane declared exhausted per skill 85. Subsumes carried `#W46-15` | modal menu emitter | `1787777928-ai_baka_deck130-0x563ec8c07390-vs-ai_baka_deck125.jsonl` **seq 60 -> 61 -> 62**; w45 control same seat vs123 seq 17 |
| **R3** | **The converter's doubling is not folded into the price it doubles.** The paragraph names blocking triggers; the A-lines print `(blocking trigger: they may gain 2)`; the multiplication is left to the reader. TWO seats lost games between the two strings in one corpus. Ask: bind it to the tag, as `#W46-9`'s lifelink binding did. Companion: one footnote sentence that a benefit-to-THEM parenthetical is a PRICE on the outcome, not part of it | `combatTradePreviewStats` blocking-trigger tail; attackers footnote | `1787777968-ai_baka_deck152-0x558324388f00-vs-ai_baka_deck126.jsonl` seq **29 / 37 / 46** (20 -> 11 -> 5, lost at -1); `1787777935-ai_baka_deck162-...-vs-ai_baka_deck126.jsonl` **seq 17** (19 -> 0 in one combat) |
| **R4** | **Offer-TIMING: a manland activation is surfaced almost only at UPKEEP priority** — `becomes beholder` 31x Upkeep / 11x Blockers / 4x Main-1 / **0x Main-2**, against 17 Main-1 and 13 Main-2 priority windows in the same games. The model cannot distinguish "wait for main phase" from "there is no main-phase offer". Ask: surface at a main-phase window when one will occur, or annotate the row as the turn's last offer | priority-window offer construction; activated-ability option row | `1787777936-ai_baka_deck146-...-vs-ai_baka_deck125.jsonl` `priority` seq **50** and **51** (t23, opponent at 1 life, lethal animation, both passed; lost t41) |

### MED

| id | item | locus | repro |
|---|---|---|---|
| **R5** | X-pricing row **leads with the smallest kill and never labels the largest**; the announce collapse frames MANA SAVED as the only axis with no marker for the most-kills row. Ask: `best affordable X: ...` in the same clause shape, plus a one-off `<- most kills at any affordable X` marker | `xKillRowCore` / X-pricing clause builder | `1787777926-ai_baka_deck130-0x5639f9b6b630-vs-ai_baka_deck123.jsonl` **seq 16 -> 18** (X=1 over a strictly free X=2; 19 to -31) |
| **R6** | `{effect:}` **truncates a planeswalker mid-loyalty-ability, dropping the ultimate** — Sorin x81 (the -6 gone), Lolth x83 (the -8 gone), Ob Nixilis x18, Teferi x32, Kaya x8, Ranger Class x34, Greaves x72. Visible truncation, so no falsehood. Ask: split on ` -- ` and budget per clause, keeping the last | `boardEffectSnippetLen` (`AIPlayerGPT.cpp:355`) + the gloss builder in `describeZoneCards` | `1787777938-ai_baka_deck126-0x5618ce439fc0-vs-ai_baka_deck123.jsonl` **seq 14** onward |
| **R7** | `{effect:}` is **opponent-only**; the pilot's OWN Sanguine Bond / Exquisite Blood / Staff of Nin render bare while the opponent's identical cards are glossed, and the model re-derived its own combo in the PLAN field every window. Ask: extend to own non-creature, non-land permanents with **no option row this window** | `describeZoneCards(effectText)` call sites — `:8642` own (default `false`) vs `:8647` opponent (`true`) | `1787777938-ai_baka_deck126-0x5618ce439fc0-vs-ai_baka_deck123.jsonl` **seq 41** |
| **R8** | **No ranged collapse on the blockers screen** — 22 B-rows differing only in a `#N` handle, 2,887 of 16,887 prompt chars, while the battlefield line five lines above collapses the same bodies. The only decision kind that got both bigger and slower this wave (tail mean +10.2%, p90 +22.0%; latency mean +28.4%, p90 +56.2%; N=30, low power, NOT called a regression) | blockers prompt emitter | `1787777926-ai_baka_deck123-0x5639f884b1d0-vs-ai_baka_deck130.jsonl` **seq 55** |
| **R9** | The **pre-game mulligan chain re-shows the carried PLAN into an identical repeated ask**, and the plan answers it; the `Mulligan` row is the loop's one unpriced row. Ask: price the row (`2. Mulligan (a keep after this one would keep 4 cards)`), and on pre-game asks either omit the plan block or re-scope it truthfully (`PLAN YOU STATED ABOUT A HAND YOU HAVE ALREADY SHIPPED ...`) | pre-game mulligan ask emitter; carried-plan block gate | `1787777938-ai_baka_deck123-0x5618cf757b20-vs-ai_baka_deck126.jsonl` **seq 1-8** (7 mulligans, kept ZERO, lost 42-0); `1787777971-ai_baka_deck123-0x56503bcdf9e0-vs-ai_baka_deck162.jsonl` **seq 1-8** (6 mulligans, kept ONE, dead t13) |
| **R10** | A colourless source renders `{1}` inside a list called "colours", and the SAME card renders `{x}` in one window and `{c}` in another **in the same game**. Traced to `ManaCost::toString` rendering the `MTG_COLOR_ARTIFACT` slot as `{<count>}` vs `MTGColorChars[0]='x'` / `[6]='c'`; **which path flips it is NOT determined — the lane's job**. Ask: `{c}` in both places, never a bare count inside a colour set | `ManaCost.cpp:1248`; `MTGDefinitions.cpp:7`; `perColorSourceCountClause` sub-list | `1787777926-ai_baka_deck130-0x5639f9b6b630-vs-ai_baka_deck123.jsonl` **seq 8** vs **seq 14** (18 emissions at one seat pair) |
| **R11** | An A-line with **no `their untapped blockers` tag says nothing**, and the model reads absence as danger — two free attacks declined at opponent life 13 and 4, one free lifelink point thrown away. Ask: the affirmative form `[no creature they control can block this attacker]`, in the same positive style as the menace tag. Subsumes carried `#W46-18` | attackers row emitter | `1787777935-...deck162-...-vs-...deck123.jsonl` seq **15**, **19**; `1787777938-ai_baka_deck126-...-vs-...deck123.jsonl` **seq 18** |
| **R12** | The `[deals 0 - ... STOPS all N damage from reaching you]` clause is **unscoped about DURATION** and reads as a standing property of the wall, beating the verdict beside it. Ask: `... THIS COMBAT`. The render must NOT carry the verdict — that is guide content (skill 70 ships that half) | `zeroPowerBlockerTag` | `1787777959-ai_baka_deck126-0x55e4f7ca6610-vs-ai_baka_deck146.jsonl` seq **5**, **8** (both blocks lost the game) |
| **R13** | The **land-drop binary is the latency tail** — the corpus's two slowest 2-option asks are land drops (453 s, 367 s with `answer_replaced`), both full-turn monologues in PLAN over a decision with one right answer; and 9 declines corpus-wide all reason the drop against a spell. Ask: suppress the PLAN request on land-drop asks, and/or state the consequence (`playing this land does not reduce what you can cast this turn`) | land-drop ask emitter; PLAN-request gate | `1787777956-ai_baka_deck146-...-vs-ai_baka_deck130.jsonl` **seq 27** |
| **R14** | Two engine-side COUNTS the header could carry, both same call site: **(a)** `(of which N are creatures, M of them able to attack)` — the header says "2 creatures" on a board of two walls and a Supreme Verdict went into it twice; **(b)** `{leaves N untapped}` on the cast row, from the affordability check already run — every tap-out rule in every guide is a subtraction the pilot performs (deck125's has read 6/11, 3-4/7, 4/7 across three waves). **Explicitly NOT** a "hand is full" style boolean warning | battlefield header builder; cast option row | deck125 sweeper casts `...-vs-ai_baka_deck126.jsonl` seq **13**, **43**; Staff casts `vs146` s23, `vs162` s21, `vs152` s25 |
| **R15** | **Carried `#W46-4`, re-armed:** ANNOUNCE_X has no preview for the non-damage X class (`sv.priceable` has nothing to price on `auto=life:X && draw:X`) **and X=0 is never called out as a null cast** — new evidence: deck130 announced X=0 on a damage spell it meant to cycle, at an unmarked row, and lost the game | `xAnnounceRowKills` / `sv.priceable` gate | `1787777928-...deck130-...-vs-...deck125.jsonl` **seq 62**; w45 `...deck125-...-vs-deck126` seq 58, 64 |

### LOW

| id | item | locus | repro |
|---|---|---|---|
| **R16** | `manaAvailableLine`'s number-word stops at ten (`if (sources <= 10)`), so wide boards read `11 total (untapped sources,` — the word exists so "a digit cannot be misbound to a colour" per the function's own comment, and it vanishes exactly where boards get complicated. 42 emissions at one seat pair | `AIPlayerGPT.cpp:2530-2537` | `1787777948-ai_baka_deck123-0x55fba47b0300-vs-ai_baka_deck125.jsonl` **seq 48**; 25-source specimen `...deck125-...-vs-...deck152` **seq 170** |
| **R17** | **Token permanents are textless** — `Clue [artifact]` bare on both battlefields (23 opponent-board entries corpus-wide) while every non-token artifact beside it carries `{effect:}`. Tokens have no primitive `text=`; the fix reads the instance's GRANTED abilities (`token(Clue,...)` + `transforms((,newability[...]))`) | `describeZoneCards` effect gloss; `isEngineTokenText` path | `1787777970-ai_baka_deck125-0x557f248fbfe0-vs-ai_baka_deck152.jsonl` **seq 32-38**; `152 vs162 attackers` **seq 19** |
| **R18** | Damage **OBJECT** still bare in history lines — lane E's declared residual, counted at **128 corpus-wide**. Checked against both battlefield lines in the same prompt: **0 cases where the bare name was on BOTH battlefields**, so the hazard is structural, not realised. LOW, not MED, in this pool | history-line emitter | any seat; `- Your Staff of Nin dealt 1 damage to Human` |
| **R19** | Mana-only auto-pass leaks 3 windows via the FLOATING-POOL half of the gate (`!getManaPool()->getConvertedCost()`), all after a declined CAST menu in the same phase, none showing an `Already in pool:` clause. 3/183 = 1.6%, all passed, gate behaving as its comment is written | `AIPlayerGPT.cpp:10945-10952` | `...deck126-...-vs-...deck123` **seq 24**; `...deck126-...-vs-...deck125` **seq 28**, **30** |
| **R20** | The target list prints CURRENT size and nothing says so, while the battlefield line has a separate convention (`(6/5) (printed 2/3)`). One preamble clause makes a class of guide rules keyable to a rendered observable | target-list preamble | deck152 counter placements (`Sigarda, Champion of Light (9/9) ... [your battlefield]`) |
| **R21** | The mana line's `mana of ANY colour pays a generic cost like {2}` did not stop a 350-second reply reasoning about whether it "has generic mana"; strengthen to state generic is never a separate resource you can lack | `manaAvailableLine` tail | `...deck162-...-vs-...deck152` **seq 10** |

### CARRIED unchanged (no new evidence beyond re-observation)

- **`#W46-10`** activated-ability STATE on the option row (`becomes beholder` is
  byte-identical whether the Hive is a land or already a 3/3). **Distinct from the repeat
  tag, which shipped as S3** — S3 answers "how many times this turn", not "is it already a
  creature". deck146's counts fell an order of magnitude on the guide edit alone (46 offers,
  3 taken, 1 redundant), so this is now MED-to-LOW pressure, not HIGH.
- **`#W46-11`** stale-plan note repeats on byte-identical plan text. **Do not reopen
  `#W45-4`, which PASSED** (11.3% fire rate this wave, bar 25%; strict false fires 0 of 66,
  tested on all 66 — the two candidates by a card-name test both name the card only as a
  TARGET inside a row while the ACTION the plan named really was absent).
- **`#W46-12`** the plan-vs-AFFORDABILITY clause (plan names a card the menu DOES offer and
  the seat cannot pay for; fire only on exact card-name match with mana value > printed
  total). deck123's thirteen-window `{2}{u}`-as-two loop is the wave-45 specimen; wave 46's
  own R9 chain is the pre-game analogue.
- **`#W46-14`** `gangBlockPriceTag` ignores the declaration-set minimum — suppressed at
  `need < 2` even when menace makes 2 bodies legally mandatory, so the cheapest LEGAL lethal
  gang is never priced. Floor `need` at `max(2, minBlockersRequired())`.
- **`#W46-16`** lexicon leaks on live rows: `Transform:backside` in 141 prompts including
  **3 numbered OPTION ROWS**; `ability: teferieffect Counter Removed` on the STACK in 31.
- **`#W46-19`** singular loyalty `(now N)`; the bare `[defender]` gloss (its content is
  correct — this is brevity, not truthfulness).
- **`#W46-20`** no LETHAL headline on the ATTACK side (the block side has one).
- **Baka-side menace gang-block behaviour — RED PIN, probe deck still owed, FOURTH wave
  carried.** No menace blockers window arose at a Baka seat this corpus either.
- **Trample carve-out on block-ladder rung 3 — SEVENTH consecutive null.** Sigarda has
  trample and attacked five times; no trampler ever reached a rung-3 block decision.
  Unreachable-in-pool per skill #21: not deleted, not re-filed as a prediction.
- **Targeted-X `"(one target only)"` phrasing — THIRD wave with no targeted X spell reaching
  an ANNOUNCE_X menu.** Probe or harness, never another corpus wait.
- **`[combattrace] ... suppressed by unresolved-stack`** — the wave-45 count of 67 was not
  recounted this wave and no per-instance test was run. Still recorded so nobody reads it as
  N skipped blocks.

### DISCHARGED AND CLOSED this wave

- **`#W46-17`** `dropped_assignments` conflating `Bn:none` padding: **0 on every record
  corpus-wide**, including a reply reading `B1:A2, B2:none`. No split needed. CLOSED.
- **`#W46-3`'s card half:** wave-45's textless Staff of Nin / Sanguine Bond / Exquisite
  Blood / Intruder Alarm / Talisman are all glossed; 1,320 name-groups covered, 0 uncovered.
  Residual is tokens only (R17). CLOSED as filed.
- **`#W46-6`** side-ask subject: Teferi's chooser (all 9 wave-45 headers) is gone; the 4
  residual headers were one different emitter and are **SHIPPED (S1)**. CLOSED.

---

## SET A — wave-45 GUIDE EDITS + the reviewer boundary pass, discharged per deck

Guides deployed as the wave-45 reviewer revisions + boundary pass `3ca0b2622`; this is the
first corpus on them. Per the carry's binding rule, **no win-rate or game-length movement is
attributed to any edit** — both records are counts.

| deck | edit / prediction | verdict | counts |
|---|---|---|---|
| 123 | RULE 0 land drop (P1) | **PASS** | 0 declines of 16 (w45: 13 of 34, all in one game). The cleanest rung this seat owns |
| 123 | RULE 0 costs-as-integers (P2) | **PASS** | 0 plan lines named a spell above the window's printed `Mana available: N total` |
| 123 | A45-3 Lightning Greaves gate (P3) | **PASS 0 of 1** | 4 offers, 1 cast at "of which 3 are creatures" (w45: 3 of 6 violations). Denominator too thin to call it fixed |
| 123 | A45-5 blockers STEP 1/2 (P4) | **PASS 0 of 7** | no token-maker blocked on a `(your blocker dies, attacker lives)` row (w45: the Bloodline Keeper block that lost a game). One skill-58 letter divergence at vs130 s55 (card over token) — n=1 and it was the winning line; NOT patched |
| 123 | A45-6 bottoming step (P5) | **UNTESTED as written** | both `bottom` asks are the mull-to-0 (no choice existed) and the mull-to-1 |
| 123 | EDIT A wall-board `ATTACK: none` (P6) | **PASS 1/1 — FIRST EVER ARMED** | vs126 s39: 14 attackers held on a Sanguine Bond + Perimeter Captain + Pride Guardian board; the hold is correct on the arithmetic |
| 123 | B14 lethal-alpha falsifier (re-check due) | **STILL UNREACHABLE-IN-POOL** | no lethal window on a wall board (best was 14 power into 22 life). A DIFFERENT falsifier landed: CHECK 1's release clause names two ENCHANTMENTS this deck cannot remove -> skill 68 |
| 123 | CHECK 1 release clause | **DECISIVE FAIL** | the unreachable release made CHECK 1 unconditional; the pilot then cast Damnation "to remove the Sanguine Bond" (`auto=bury all(creature)`, both enchantments verified `type=Enchantment`), killed its own 14 creatures, lost 42-0 |
| 123 | edict N-check (A45 EDIT L) | **FAIL, and the 1 cast is a triple violation** | **57 edict offers, 1 cast** — at N=7, aimed at a planeswalker (`target=opponent ... notaTarget(creature|mybattlefield) sacrifice` cannot touch one), got a 1/1 token; Sorin ticked to 8 |
| 123 | Damnation discipline | **REGRESSED** | 37 offers, 1 cast, and the 1 is the game-losing one (w45: 59/1 and correct) |
| 123 | 12 -> 20 token cap (boundary pass; sunset due) | **FAIL, BOTH DIRECTIONS** | 114 token-line offers, 95 taken. **6 takes at 20-23** (vs130 s51/52, vs152 s39-42), both games WON; **6 declines below 20** at counts 3-15 where the rule says take it. WHY re-measured and TRUE (largest prompt at >=23 creatures = **19,082 chars, identical to its largest at >=12**). Reviewer DELETED and replaced it with power-vs-their-printed-life — skill 84 |
| 123 | mulligan chain STEP 1 (boundary pass) | **FAIL 9 of 9** | 7-mulligan (kept ZERO, lost 42-0) and 6-mulligan (kept ONE, dead t13) chains, both a flat violation, both decisive before turn one. Mechanism is placement + the carried plan, not wording — skill 69, docket R9 |
| 123 | Vision Skeins / Tragic Slip discipline | **HELD** | 32 offers / 4 casts, 0 into a draw-punish board; 36 Slip offers, no violation found |
| 125 | W16 land drop | **PASS 40/40, 0 declines** | on 40 distinct turns (w45: 67 of 81 with 38 declines). The two-place hoist's cleanest confirmation |
| 125 | W17 X ceiling + no cast at hand >= 8 | **FAIL both halves — ROOT CAUSE IS A RENDER GAP** | 17 X windows, conformed 4/17; 7 of 17 casts at a hand line of 8+. 3 of the 4 replies stating a hand size stated it WRONG (once "hand size 6" against an eleven-card list). Fixed at ship by **S4**. Largest-offered-X take fell **14/14 -> 4/12 -> 1/17** |
| 125 | W18 counter `[defender]` clause | **PASS both halves** | 43 windows, 39 taken, **0 of 39 onto a `[defender]` body** (w45: 4 of 33). New residual one class over: 2 of 39 onto mana artifacts (Chromatic Lantern, Talisman) — the unnamed `(artifact) with no P/T` clause, skill 74 |
| 125 | W19 sweeper 0-creature gate | **PASS 0 of 123** | 165 sweeper windows, 10 casts, **123 windows at a 0-creature header, 0 casts** (w45 1/9, w44 3/14); 0 casts at a lone defender. NEW one branch down: 2 of 10 into boards whose whole creature count is 0-power defenders |
| 125 | W20 Staff of Nin tap-out | **FAIL 4/7, flat for three waves** | violations at `Mana available:` 6, 7, 6; conformers at 17, 10, 13, 22. n=7, reported directionally (#45); the four-way branch is the failure, not the principle — skill 73 |
| 125 | W21 mulligan | **PASS** | 1 mulligan in 6, on a hand whose coverage line read exactly `would not cover any spell in it` |
| 125 | Emrakul absolute (body-only, not in DECIDING SITUATIONS) | **FAIL 4 of 5** | printed in the numbered list and answered `Cast nothing right now` four times. Third instance of the two-place hoist pattern — skill S4 in the changelog |
| 125 | Elixir at library <= 16 | **UNTESTED, 2nd wave** | 35 activation windows, 15 taken; library minimum across offered windows was 20. 0 decking losses |
| 125 | Staff pings / `{target text:}` | **PASS** | 75/75 pings, every window with a player option took the opponent; `{target text:}` on 100% of removal/counter rows, 4th consecutive wave |
| 126 | D16 land drop | **PASS 17/17** | denominator down from 41 because a decline used to re-ask |
| 126 | D17 `Add N green` with no cast | **FAIL 1/16** | and **the denominator changed**: the auto-pass removed 180 windows before they reached the model, so 1/16 and w45's 2/39 are different populations — skill 87 |
| 126 | D18 flying-Tribute conjunct | **UNTESTED, 2nd wave** | the conjunct never arose; NOT MOVED (#66 forbids moving a floor on zero windows) |
| 126 | D19 Chromatic Lantern gate | **FAIL 5/5 and 0/1 — the GUIDE'S CARD FACT WAS WRONG** | `Urborg, Tomb of Yawgmoth / auto=lord(land) transforms((swamp))` makes every land a Swamp, so the guide's "SAVANNAH AND SUNPETAL GROVE DO NOT [make black]" is false on exactly the board where it was applied twice. The render's `{B} 3` / `{B} 6` was right; the guide's by-name "2" was wrong — skill 75 |
| 126 | D20 blocking | **PASS / PASS / FAIL** | 12 windows, `BLOCKS: none` **0 times**; every non-excluded blocker assigned; the survival tiebreak failed **2/2** and is the seat's only loss (a 0/4 wall traded for ONE stopped damage against a 1/1 first-strike deathtoucher; a 0/3 wall took the attacker that kills it over the one it survives) — skill 70, docket R12 |
| 126 | D21 Sanguine Bond coloured-pip discipline | **PASS, 0 occurrences** | every Bond/Blood cast at a per-colour clause reading `{B} 3` or better. The wave-45 coloured-pip disaster did not recur in 211 decisions |
| 126 | B9 collapsed-tag rule (boundary pass) | **PASS 2/2 — first live adjudication** | vs123 s37 and s43, both `they have 14 untapped creatures ...`, both correctly `ATTACK: none`. Two misses at N<=1 in the other direction -> the untagged-A-line class (R11) |
| 126 | Idyllic Tutor picks | **PASS 3/3** | every window picked the missing half |
| 130 | B7 mulligan coverage line (boundary pass) | **PASS 6/6** | 6 straight keeps, kept-land counts 4,1,3,2,1,2 — including two ONE-LAND sevens kept on the coverage line, one won, one lost. The edit did exactly what it was written to do |
| 130 | EDIT 45-1 Starstorm X=0 / cycling (P7) | **FAIL 1 of 1 each, SECOND CORPUS IDENTICAL** | vs125 s61 "Cast Card Normally" at the guide's own release state, s62 X=0 — while the plan cited the rule BY NUMBER. Guide lane exhausted (skill 85) -> R2 |
| 130 | EDIT 45-2 RACE CHECK (P8) | **PASS-with-1, 5 of 6** | the 1 (vs123 s37, 22 untapped fliers totalling 89 power) was the pilot being RIGHT against the rule's letter. The wave-45 losing shape did not recur -> the defect the rule was written for is UNTESTED |
| 130 | EDIT 45-3 Starstorm quick-reference qualifier | **PASS** | the closing DECIDING SITUATIONS entries were followed in all 6 hold windows; no unqualified TAKE-IT cast |
| 130 | EDIT 45-4 token-vs-card `(both die)` (P9) | **PASS 1/1** | vs146 s12: Goblins assigned to both attackers, Siege-Gang Commander kept out |
| 130 | EDIT 45-5 lethal legality qualifier (P10) | **UNTESTED** | 2 blockers windows, neither LETHAL-flagged |
| 130 | EDIT 45-6 per-row X reading (P11) | **FAIL 3 of 4** | vs123 s18 (X=1 over a strictly larger free kill), vs123 s23 and vs126 s31 (top of a collapsed equal-kill run) -> R5 |
| 130 | EDIT 8 LD-targeting backstop retired | **PASS** | every Blastminer/Lay Waste/Molten Rain/Stone Rain row taken read `[opponent's battlefield]`; 0 self-hits |
| 130 | Blastminer attack floor / FACE BURN | **PASS 2/2 / HELD** | the only two sole-attacker windows both `ATTACK: none`; no damage spell fired at a player above 6 life |
| 130 | Blastminer LD gate (internal contradiction) | **FAIL 2/2** | the card section says "activate him every turn you have three spare mana and their board shows a non-basic land" — ungated — against the deck's own FOUR-OR-FEWER-lands gate elsewhere; both activations were at 7-8 opposing lands, at 11 and 5 life, dead at -2 on t12 |
| 146 | P1 THE WHOLE CAST ORDER, entry 2 over entry 3 | **PASS 7/8** (w45: 0/1, and that miss lost a game) | the biggest single win on this seat. Overall first-match **46/73** — and the residual is STRUCTURAL: 9 breaks are cards the list does not contain (Silencer x8, Emeria's Call x1), 10 are Command declines with no observable -> skill 76 |
| 146 | P2 `(neither dies)` + `(blocking trigger:` | **UNTESTED** | no A-row carried both. Not deleted |
| 146 | P3 Hive already-on / upkeep / second-in-chain | **NEAR-PASS: 1 / 1 / 1, from 10 / 18 / 6** | 46 offers, 3 taken (was 78 / 27). The 4-mana bleed is essentially closed; the residual's cause shipped as **S3** |
| 146 | Hive CHECK 1's upkeep absolute | **COST A GAME** | opponent at 1 life, lethal animation available, the only offer that turn was in Upkeep, both windows passed — skill 71, docket R4 |
| 146 | Lolth `+0: draw and lose life` | **FAIL** | activations at 16, 12, 9, **5** and **2** life (`auto=... draw:1 controller && life:-1 controller`, verified); the seat died at 0 to a Staff ping. "Take it most turns you are not desperate" has no observable — skill 72(c) |
| 146 | dungeon / room / Vanishing Verse / Agadeem's | **PASS** | LOST MINE by name 11/11; room body-or-drain 19/19; `targets are YOUR OWN` -> do not cast **22/23** (the one break states its false belief out loud: "exile Nadaar ... to cycle it back to hand"); Agadeem's 0 offers, UNTESTED, not deleted |
| 146 | land drops | **PASS 43/43** | the control observation that makes the deck152/deck162 land edits evidence-based |
| 146+152+162 | P4 per-creature `your attacker dies` STOP | **FAIL but MOVED: 3 of 5, was 7 of 7** | 5 windows on a non-token CARD with the lethal check unmet, 2 held. The three residual loopholes are three DIFFERENT arguments (skill 53 confirmed): a friendly-sibling theory ("Triumphant Adventurer blocks Fate Unraveler"), a not-consulted stop, and a collapsed-tag rule losing to "she flies and tramples". The two clauses named hardest in wave 45 — vigilance and lifelink — **both held on their first live windows** |
| 152 | deploy floor | **PASS 50/50** (w45 33/33, w44 45/47) | second corpus at ceiling |
| 152 | P5 Fateful Absence `targets are YOUR OWN` | **PASS 30/30 — first live windows** | wave 45 recorded UNTESTED |
| 152 | P5 collapsed tag / GANG BLOCK | **FAIL 0/1 priced, PASS 2/3 collapsed** | the priced send is the shape the rule did not anticipate: **every parenthesis friendly, the GANG BLOCK clause fatal**. The other loss is Sigarda sent past a 9-blocker collapse on "she flies" |
| 152 | P6 `BLOCKS: none` while a rung 0-3 block is offered | **FAIL 1 of 5** (w45 2 of 2) | header "you would be at **8** — NOT lethal" against a floor of 9; the reply QUOTED the 8 and never compared it. Second corpus running -> skill 79 |
| 152 | counter spreading ("from the fifth counter on, a SECOND creature") | **FAIL 7/7 in one game** | Sigarda 4/4 -> 11/11. A tally the executor cannot keep, against an option row that prints current size -> skill 72(b) |
| 152 | counter placement (P6 other half) | **PASS** | 0 counters onto a Fog-Bank/wall-blocked body; both vs162 placements went to the body that was connecting |
| 152 | land drops | **FAIL 51/56** | five declines, every one weighing the drop against the spell it meant to cast. deck146, with a land rule, is 43/43 |
| 152 | converter vs lifegain-wall precedence | **THE DECIDING LOSS** | both rules present, no precedence stated; the POSTURE rule won three combats running, 20 -> 11 -> 5 while the opponent gained the same, lost at -1 -> skill 78, docket R3 |
| 152 | bottoming land floor | **REGIME FAIL** | "never bottom a land unless the hand you keep would still hold at least FOUR" is right at 6 and 5 cards and absurd at 4, where the seat correctly bottomed three lands from a five-land keep -> skill 80 |
| 162 | Rule 1 punisher-first | **PASS 20/21** (w45 17/20, w44 13/17) | best yet; still the pool's single best per-deck priority metric and the argument for skill 62/76 |
| 162 | P7 162-A LETHAL survival gate | **PASS 1/1 — first live window** | vs146 s16, header "-1 - LETHAL": chose the assignment surviving at 3 over the one surviving at 1, which is what STEP 4 orders |
| 162 | S6 affirmative send rung | **PASS on its clean window, FAIL on the lifegain shape** | fired correctly where all three names read the free line; but the SAME rung licensed the 19 -> 0 combat, because the guide has no lifegain/converter price at all — three 1/1 lifelink blockers, Sanguine Bond, then Exquisite Blood |
| 162 | P8 collapsed tag + GANG BLOCK kill clause | **UNTESTED, 0 windows** | |
| 162 | P9 bottoming / MULLIGAN section | **UNTESTED — 0 mulligans this corpus** | the sibling rule PASSED at deck152 (bottomed a Forest from a 5-land keep, leaving four — the wave-45 carve-out **VALIDATED on its first live window**) |
| 162 | Rule 2 engine brake | **5/7, VIOLATED-UNPUNISHED** | both breaks are the exact shape the rule names; both games survivable. Per #45/#64 NOT loosened, NOT re-worded; carried as a wave-47 prediction |
| 162 | land drops | **FAIL 35/39** | three of the four declines in the game it lost, holding three lands, the drop weighed against a cast. No land rule in this guide |
| 162 | ATTACKING rungs vs an untagged A-line | **FAIL 2 of 2** | free damage declined at opponent life 13 and 4; attack rate 12/12 -> **6/8** -> skill 82, docket R11 |
| 162 | keep-it-home rule (names one card) | **FAIL 1/1** | swung its only untapped body (Master of the Feast, which the guide separately calls "your fastest clock") at 3 life, dead at -2 -> skill 83 |
| 146+152+162 | P10 mulligan floors | **FAIL 1 of 18** | 152 vs162 s3, at `(keeping 5)` with a land in hand, cascading to a four-card hand — **and it WON that game**. Floor NOT moved (#66); all other 17 openers conform |
| 146+152+162 | P11 `dropped_assignments` = 0 | **PASS, 0 on every record** | including a reply reading `B1:A2, B2:none` |

**Set A totals: 26 PASS, 6 PASS-with-1 / near-PASS, 17 FAIL, 10 UNTESTED.** Of the 17 FAILs,
**5 are the guide text's fault rather than the pilot's** (123's release clause, 126's Urborg
colour claim, 130's Blastminer contradiction, 152's unstated precedence, 152's regime-less
land floor) and are recorded as evidence against the rule per skill 59/64/75/78/80. **Three
FAILs closed at ship** (125 W17 -> S4; 146 P3's residual -> S3; the `A choice is required`
class -> S1).

---

## SET B — wave-46 ENGINE LANES, discharged per lane with counts

| lane | verdict | counts |
|---|---|---|
| **#W46-1** per-colour source counts (F, 421bd5f62) | **PASS — exact, whole-corpus** | **1708/1708** windows recounted from their own per-source sub-list, paren-aware: 0 colour-count mismatches, 0 source-total mismatches. Conventions hold on the specimens that test them (a five-colour dual counted under all five; `Overgrown Battlement {g} (VARIABLE output...)` = ONE source). Behaviour: **coloured-pip overcommits 0 at every seat**; deck126's wave-45 game-losing overcommit did not recur in 211 decisions, and every combo-piece cast was at `{B} 3`+. `Add N green` taken with no cast **1/16** at deck126, **1 of 1** corpus-wide priority windows where any mana ability was taken. Two defects on the same line -> **R10** (`{1}`/`{x}` vs `{c}`, 18 emissions) and **R16** (number-word above ten, 42 emissions) |
| **#W46-2** copies-left-behind (F) | **PASS on all three legs, decisively** | Clause on **5,904 lines / 1,154 windows** (w45: 0), nine shapes covering hand, own battlefield and opponent battlefield. Correctness: 107 hand-zone clauses cross-checked, 96 exact, all 11 apparent mismatches explained as same-`events`-block snapshot lag, **0 falsehoods**. Contradiction-arbitration replies **5 -> 0** (the word appears in no reply in the corpus). p99 on 2-option asks **117.8 s** (bar 120; w45 141.3), n=619, max 453.2 s. Sub-finding: the tail is not the clause, it is the **land-drop binary** -> R13 |
| **#W46-3** opponent permanent `{effect:}` + converter scope (G, bcea9f63c) | **PASS with one residual class** | **1,320 name-groups covered, 0 uncovered** over 1,958 windows carrying an opponent battlefield line; clause fidelity **21/21** exact prefixes of the primitives' `text=`, no invented text. Every "textless" instance is a numbered LATER copy under the deliberate `{effect (each copy of this card does this): ...}` collapse. Wave-45's five named textless cards all now carry text. Converter line: **10/10 seat-logs** with Sanguine Bond on a battlefield carried it on a non-attackers window (**147 non-attackers windows**, vs 5 total in w45); the one apparent gap has Exquisite Blood (loss->gain), correctly outside `lifeToDamageConverterScript`'s class. Budget measured: max clause 143 chars, max distinct names 4 (the 55-char tier never reached), ~70 chars/decision = 4.6% of the wave's growth — **board width is the cost driver, not the budget**. Residuals: R6 (planeswalker truncation), R7 (opponent-only), R17 (tokens) |
| **#W46-5** blockers GANG BLOCK mirror (H, 2003520a1) | **RENDER PASS; behavioural half PASS 0/N; token cost is the watch item** | 30 blockers windows; the string reached the blockers screen **once**, and its arithmetic is correct (1+3 = 4 >= toughness 4). **Hand-derived group arithmetic in blockers replies 0/30** (w45: 1 of 1 wide window, ~250 words). **All-`your blocker dies` windows with a lethal legal group and no price: 0/13** — every silence audited against the emitter's own gate (`need = 1`, or a `gangOk` exclusion such as first strike), not by eye. Token cost measured as asked: tail mean 4,022 -> **4,431** (+10.2%), p90 5,381 -> **6,567** (+22.0%); latency mean 35.8 -> **46.0 s**, p90 55.8 -> **87.2 s**. N=30 vs 33 — low power, NOT called a regression, pre-registered as R8's wave-47 gate |
| **#W46-9** lifelink tail bound to its branch (H) | **PASS on render and behaviour** | **82 tails, 4 exact shapes, 0 loose tails** anywhere in any prompt; all on `A<n>.`/`B<n>.` trade lines (56 attackers / 26 blockers). Scoping correct: unbound `(lifelink: you/they gain N)` appears only under `both die` / `you kill it, your attacker lives` / `your blocker dies` heads. **Replies adding keyword benefit to face damage: 0** (w45 had the "gain 3 life each AND deal 6 damage" reply), and at deck152 vs123 s24 **the bound row was the row that was HELD**. Magnitudes spot-checked correct for Wagic's damage assignment |
| **#W46-6** `A choice is required` headers (I, 81cefd06e) | **PARTIAL at corpus time, SHIPPED since** | **9 -> 4**; lane I's `storedSourceCard` unwrap did work on the `ability$!...!$` dummy class and Teferi's side chooser is at **0** (`Choose an option for Teferi, Who Slows the Sunset:` renders 10 times). All 4 residuals were ONE card (Silverquill Command's `auto=choice` menu) and **2 of the 4 produced `stale_echo`**. Root cause proven from the code (a 2-rung header ladder where `resolveOwningCardName` is 3, plus a rescue gated on a NULL pointer instead of an empty NAME) and confirmed by the engine naming the same card one tick later in the history line. **Now S1** |
| **#W46-7** mana-only priority auto-pass (I) | **PASS on the counter and the loss-proof; 3 leaked; the take-rate prediction is MIS-STATED** | `mana_only_windows_skipped` present in **all 42 gameend records**. **180 skipped, all at deck126** (98 + 54 + 28; 39 seats report 0), ~85% of that seat's priority churn. **3 all-mana windows leaked** via the floating-pool half of the gate — all passed, 3/183 = 1.6% -> R19. "No skipped window had a non-mana option" **PROVEN two ways**: by the emitter (`isManaOnlyAction` is type-exact; the gate breaks on the first non-matching option) and by reconstruction (**13/13** windows where a Sorin loyalty or Staff ping coexisted with the Battlement survived and were asked; **0 lost non-mana options**). **"Take-rate unchanged" is FALSIFIED AS WRITTEN and must be restated**: raw priority take-rate rose 47.8% -> 62.3%, which is the denominator effect the gate exists to produce — skill 87 |
| **#W46-8** X collapse wording (I) | **PASS** | **21/21** ANNOUNCE_X windows carry `{X pricing: same kills as X=N, for M less mana}`; the bare wave-45 `same as X=` shape appears **0 times**. Of the 4 windows offering a collapsed cheaper row the model took the cheaper row **4 of 4** — the wave-45 failure (plan X=1, answered X=2) did not recur. The 2-of-4 FAIL one seat reported is a DIFFERENT metric (answering the TOP of a collapsed equal-kill RUN) and routes to R5 |
| **Lane E** (#W45-20, 607ca6bb7 — first corpus carrying it) | **PASS, decisively** | Full replay of the final prompt in all 42 seats, **9,302 history lines**: unprefixed lines **586 -> 4**, unprefixed **card-SUBJECT** lines **586 -> 0**. The 4 survivors are `- It became Day`, a CR 730 designation with no card subject. Declared residual confirmed and quantified: **damage OBJECT bare on 128 lines**, and checked against both battlefield lines in the same prompt — **0 realised ambiguities in this pool** -> R18, LOW |
| **#W45-4** plan-mismatch note (carried, continuing) | **PASS** | Fires **11.3%** of decisions (bar 25%; w45 13.7%, w44 42.8%). **Strict false fires 0 of 66, tested on all 66** — the two candidates by a card-name test both name the card only as a TARGET inside a row while the ACTION the plan named really was absent. Residual is usefulness -> `#W46-11`; **do not reopen** |
| **Fair hands** (lane A, carried) | **PASS, 3rd corpus** | 42 openers, engine land counts 0-7, 8 mulligan chains with correct running "having already taken N mulligans ... (keeping M)" text at every step — including a full SEVEN-mulligan London chain — and a CR 103.5 `bottom` ask correctly reading "Your ENTIRE hand goes to the bottom" at M=0. 8 `bottom` records, 7 options each, 8-42 s, 0 fallbacks |

**New render/narration FALSEHOODS found this wave: ZERO.** Every wave-46 string checkable
against ground truth checked out — 1708/1708 mana counts, 107/107 copies clauses, 21/21
effect clauses vs primitives, 82/82 lifelink tails on their branch, 10/10 converter games,
21/21 X-pricing rows, 0 unprefixed card-subject lines. Every open item is an OMISSION, a
SCOPE, or an ORDERING.

---

## FALLBACK CLASSES — 6 / 2113 = 0.28% (w45 0.11%, w44 0.28%)

The parent hypothesis — "a NEW wave-46 render string is being echoed and mis-read by the
INDEX-WINS pass" — is **REFUTED 0/5**: every `stale_echo` reply was grepped for `was 1 of`,
`still there`, `{effect`, `GANG BLOCK`, `sources that can make each`, `X pricing`,
`LIFE-TO-DAMAGE`, `lifelink:`, `copy 1 of`, `copies in your hand` — **zero hits in any of the
five**. Every echoed label is a plain CARD NAME, and INDEX-WINS behaved exactly as documented
(`AIPlayerGPT.cpp:10376-10380`). The regression is upstream of the parser.

| class | count | records | mechanism | status |
|---|---|---|---|---|
| `stale_echo` — **subject-less modal header** | 2 | `1787777934-...deck146-...-vs-...deck152` **seq 13**; `1787777956-...deck146-...-vs-...deck130` **seq 23** — both `CHOICE: 5 (Silverquill Command)` against option 5 `you draw and sacrifice creature` | The CHOOSE_MENU header had no subject to name, so the model named the source card. Root cause proven in the code (2-rung ladder; rescue gated on a NULL pointer) | **CLOSED at ship — S1**. Consequence audit honest: at vs130 s23 the heuristic substituted mode 4 for the model's intended mode 5, both carry the `sacrifice creature` leg and the opponent chose the same body — **not game-deciding**, though the seat reviewer notes the Command was deck146's only answer to a Rorix Bladewing in that game |
| `stale_echo` — **cast-menu exemplar on a cast-free screen** | 2 | `1787777935-...deck162-...-vs-...deck126` **seq 18** (`CHOICE: 1 (Cast Forced Fruition)` into three identical `Vampire (1/1) [lifelink]` rows); `1787777968-...deck152-...-vs-...deck126` **seq 10** (`CHOICE: 1 (Cast Elite Spellbinder)` into `Perimeter Captain (0/4) [defender]`) | Both are the `Combat damage order:` screen. The fixed exemplar `"CHOICE: 3 (Cast Example Card)"` was the only `Cast ...`-shaped string in context on a screen with no cast option, and the model copied from its own PLAN. Corpus-wide: replies whose label starts with `Cast ` into a menu where no option does — w45 3/1662, w46 **4/1868** | **CLOSED at ship — S2**. Consequence audit: neither changed a game (three identical Vampires; two 0/4 walls — the heuristic's order was materially equivalent) |
| `stale_echo` — **plan-name echo into a real cast menu** | 1 | `1787777938-...deck126-...-vs-...deck123` **seq 41** — `CHOICE: 1 (Cast Exquisite Blood)` into a menu of `Cast Idyllic Tutor` / `Cast nothing` | NOT covered by S1 or S2: this menu HAS cast options and a correct exemplar. The model named the card its plan wanted, not the card at index 1. The parser rejected it correctly. Note the same prompt renders the seat's own Exquisite Blood **bare** on `Your battlefield:` (R7) while the opponent's cards are glossed | **CARRIED, 1 event.** Pinned; a second occurrence in wave 47 makes it a reply-protocol item |
| `empty_reply` — **900 s timeout on a byte-identical re-offer** | 1 | `1787777926-ai_baka_deck123-0x5639f884b1d0-vs-ai_baka_deck130.jsonl` **seq 13** (t8, Main 1, `latency_ms=900047`, prompt **5,645 chars — bottom decile**, so size is NOT the cause) | A Thraben Doomsayer + Intruder Alarm token loop: one option, and `kRepeatActivationFloor = 2` meant the FIRST repeat's row was byte-identical to the previous window's. The moment the counter reached 2 and the tag appeared, latency collapsed monotonically 20.5 -> 10.5 -> 7.9 -> 7.5 s — the annotation demonstrably terminates the spiral. Server side NOT verified (no inference logs harvested), so "decode spiral" is inference; the option-row identity is fact | **CLOSED at ship — S3** (`floor = 1`) |

Baseline movement 0.17% (w43) -> 0.28% (w44) -> 0.11% (w45) -> 0.28% (w46), reported
directionally (#45); n cannot separate these, and 4 of the 6 have a named, shipped cause.
`commit_retracted` **0** corpus-wide; `answer_replaced` **8** (146 x3, 126 x2, 152 x2,
125 x1), coinciding with the latency tail but **not causally established** — not docketed.

Also pinned, not a fallback: the corpus's longest 2-option decisions are the two land-drop
binaries (453 s, 367 s) -> R13, and deck126's 386.8 s single-window outlier (a 2-option
casting ask spent six minutes re-deriving its own mana, ending in the right answer).

---

## DECK-CONTENT DECISIONS — made in the REVIEWER lane, recorded here

Per the owner's 2026-08-26 doctrine, the deck reviewer owns every guide change and every
deck-content question is decided from evidence. **No question was routed to the owner and
none is open.** Synthesis records, rather than makes, the following:

1. **deck123's token cap: DELETED, one wave early, and REPLACED.** The wave-45 sunset asked
   for 0 violations at 20 and got **6 crossings at 20-23 (both games won) plus 6 declines at
   counts 3-15** where the rule says take the line. The second signal is the stronger one:
   the number is inert in both directions. The WHY was **re-measured and confirmed TRUE**
   (largest prompt at >=23 creatures = 19,082 chars, identical to its largest at >=12), and
   confirming it is what removed the last argument for any headcount. Skill #35's
   do-not-delete-the-only-stop is satisfied by REPLACEMENT (power-vs-their-printed-life),
   not by deletion. Generalised as skill amendment 84.
2. **Mulligan floors: STILL NOT MOVED.** 20 windows this corpus. deck152's one
   `(keeping 5)` violation **won its game** and deck130's two one-land keeps split 1-1 — the
   odds run against the guide about as often as against the pilot, which is exactly the
   situation #66 forbids resolving by moving a number. deck123's chain catastrophes are a
   PLACEMENT and CARRIED-PLAN failure of a correct text (skill 69 + docket R9), not evidence
   about the floor.
3. **deck162's Rule 2 engine brake: NOT loosened at 5/7**, both breaks in the exact shape the
   rule names, both games survivable — VIOLATED-UNPUNISHED (#64), carried as a prediction.

---

## FOR THE ORCHESTRATOR TO ASK

**None.** Every question the seats raised is either a deck-content question (decided in the
reviewer lane, recorded above) or an engine/render item (docketed above). No loop-rule
question — process, render-shape convention, or scope — is open.

One process note, recorded not asked: **four docket items were fixed between the corpus and
this synthesis** (S1-S4), which is the loop working as the owner's doctrine describes — the
review exists to feed the core loop, and the core loop moved before the ledger was written.
The consequence for wave 47 is that its set-B gate must adjudicate S1-S4's predictions
alongside the wave-47 lanes, and that R2, R4 and R15 are now the oldest unshipped items with
a game-losing repro each.
