# Wave-61 engine-narration seat — corpus `matchups-20260905-092408` (the RERUN)

Binary: master `8097c12cc` (= `5faa9e970` + the async slot-key fix). Every figure below was
re-counted from the rendered `prompt` of the 42 seat translogs with python; HOLD takes are
prefix-matched against the rendered row. Card facts were read out of
`projects/mtg/bin/Res/sets/primitives/*.txt` in this worktree. Nothing under `bin/Res`, `src/` was
edited; no git, no wagic run.

---

## 1. Corpus census

| | wave 60 (`171140`) | **wave 61 rerun (`092408`)** |
|---|---|---|
| games | 21 natural / 0 hung / 0 crashed | **21 natural / 0 hung / 0 crashed** (results.tsv, 42 `gameend` records) |
| decisions (records with a `prompt`) | 2,330 | **2,035** |
| fallbacks | 12 | **6 = 0.29%** — `named_row_reask` 4, `plan_missing` 1, `wall_miss_unrecorded` 1 |
| recoveries | — | **5** `recovery` records, one per fallback except `plan_missing` |
| stale drops (`async_drops`) | 64 stderr lines | **16** (16 records × 1; stderr `dropping stale async` = 16, exact match) |
| `stale_livelock` | 0 | **0** |
| transport failures (`transport` field) | 2 | **1** — `curl=28,http=0,empty=1,connect_ms=20000,phase=wall`, `latency_ms` 900,024, `deadline_pct` 100.0, classed `wall_miss` (130v126 seat-130 seq 12) |
| `transport_error` retries | 0 | **0** |
| timeouts / wall misses | 2 / 2 | **1 / 1** (`wall_miss_events` 1, `wall_miss_unrecorded` 1) |
| `deadline_pct` emitted | absent from the whole corpus | **2,035 of 2,035** records with `latency_ms >= 0`; max 100.0; **0 records > 100** |
| HOLD renders / takes | — | **1,324 windows render the row / 330 taken** (prefix-match) |
| `hold_windows_skipped` | 2,013 (86.4% of decisions) | **1,707 (83.9%)** |
| other suppressions | — | `mana_only_windows_skipped` 267 · `identical_ask_answers_reserved` 128 · `identical_option_asks_resolved` 19 · `dropped_assignments` 0 |
| wave-58/59 crash signatures | — | `empty_reply` **0**, `transport_error` **0**, `bad_reply` **0**, `reveal_stall_forced` **0**, `deadline_pct > 100` **0**, SEGV/abort/`Segmentation`/watchdog lines in the 21 stderr files **0**, no translog gap (every file ends in a `gameend`) |
| prompt economy | GAME LOG 62.9% of bytes | 23.4 MB of prompt, median 10,237 B, max 30,436 B; **GAME LOG 57.6%**; 668 of 2,035 decisions (32.8%) are pure declines, 330 of them HOLD; 19.9 h of model latency in 1.4 h wall |

**The livelock fix (`8097c12cc`) is confirmed by this corpus.** The killed run
(`matchups-20260904-220227`) produced 15,900 stale-drop stderr lines and 2,644 `stale_livelock`
refusals over 3,586 decisions (73.7%). This run produced **16 stale drops and 0 livelocks over 2,035
decisions (0.79%)** — below even the wave-60 baseline of 64 — and 21/21 games finished naturally in
1.4 h against 17/21 unfinished at 11 h. The C14 hold-check note is the mechanism that was blamed and
it is present here in **1,275 prompts**, exactly one `[hold check: ...]` line per prompt (0 windows
with two), and its `(N windows in a row now)` counter now **maxes at 15** across the whole corpus
(`152v125` seat-152 seq 96) against the per-tick 125→132 delta recorded in the prompt diff that
diagnosed the livelock. The note measures per window, not per rebuild. Direct proof that the slot
key is note-free is not available from the translog (only answered windows are recorded), so the
census figures above are the whole of the evidence.

---

## 2. Prediction adjudication (D22 skipped as closed)

### Lane R

| # | verdict | evidence |
|---|---------|----------|
| **1 · C1a** ATTACK TOTAL floor vs `blocking trigger:` | **PASS** | 10 of 10 windows carrying both `ATTACK TOTAL` and a `blocking trigger:` tag; **0** carry a bare `they would be at N` on the ATTACK TOTAL line (falsifier = 0/10). 2 of the 10 had a computed `blockGain` and rendered the new shape (`162v126` seat-162 s37: *"At least 5 damage lands whatever they block - that damage alone puts them at 32, but … up to 12 life back across their 3 blockers, so blocking can leave them as high as 44"*; `123v126` seat-123 s52). The one measurable band held: s37's post-combat opponent life settled at **44**, the top of `[32, 44]` (opp 37 → 49 at s38 before damage, 44 at s40). The other 8 claimed no floor at all (the "their N untapped blockers can cover every attacker" branch), which is not a floor claim. |
| **2 · C1b** no `KILLS them` over an attack punisher | **PASS** | 8 windows carry a permanent matching `attackDeclarationPunishers` (Lightmine Field, `152v125` s41/61/99 and `126v125` s29/35/50/77/85); **all 8** carry `Both figures are BEFORE their attack triggers: Lightmine Field fires on the declaration …`; **0 of 8** contain `KILLS them`. The 8 windows that do say `KILLS them` are a disjoint set. |
| **3 · C2** blockers-ask verdict | **PASS** | 26 `blockers` records. `may legally block every attacker in that total` renders 18×, **0** of them on a record whose `B<n>.` rows number fewer than its attacker count (falsifier 0/18). The count-respecting replacement fired: `162v152` s10 — *"your creatures can legally block at most 1 of those 2 attackers at once, so at least 1 of them go unblocked"*. **26 of 26** blockers records carry a `best case with every blocker assigned` / `at least N of that lands` / `AT BEST` clause — the wave-60 47-attacker drop did not recur (no board exceeded 97 attackers). |
| **4 · C3** crack-back floor / evasion | **SPLIT: render FAIL-as-worded (superseded by R6) · overrun PASS · evasion PASS** | 303 `CRACK-BACK NEXT TURN` lines. 101 read `or lower` and carry the `FLOOR, not a ceiling` clause; 26 carry the new evasion sub-total (`of that, X from F attackers nothing you control can legally block`) — wave 60 had 0 of 51 netting evasion. **But 53 lines read the plain ceiling on a board that DOES carry one of the three classes** (Hive of the Eye Tyrant 40, Siege-Gang Commander 13), which falsifies prediction 4 as written. Every one of the 53 is correct under lane V's **R6** affordability gate, verified against the primitives: Hive's animation is `{3}{B}` (borderline.txt:53394) = 4 mana and the seat never saw the opponent above 3 untapped sources; Siege-Gang's is `{1}{R}` (mtg.txt:105073) = 2 mana against 0–1 sources. R6 narrowed C3's promise and the corpus shows the narrowing 53×. **Overrun**: over the 57 crack-back turn-forecasts that have a following turn, the seat's minimum life in turn T+1 fell below the printed K in **8 (14.0%)**, down from wave 60's 6 of 26 (23%) at deck125. 3 of the 8 carried the floor clause. The 5 that did not are classes none of (a)/(b)/(c) names — new permanents cast on the opponent's turn (`125v146` s26: Lolth cast next turn, 18 → 0) and non-combat draw-punisher damage (`123v162` s50, two Fate Unravelers) — which is what the prediction allowed. |
| **5 · C4** `ON THE STACK` prices a chosen-player damage ability | **PASS (N=2 of 2)** | 386 windows render a stack block; 64 carry damage and a `ON THE STACK: N damage to you` total, 14 carry damage with no total, **all 14 aimed at a creature or at the opponent** (Lightmine Field at attackers, Pyrite Spellbomb *(aimed at Thraben Doomsayer)*, the seat's own Fate Unraveler / Ob Nixilis / Staff of Nin). Staff of Nin's `{T}:damage:1 target(anytarget)` (mtg.txt:112427) reached the stack 4× — the 2 renders `[from their Staff of Nin]` **both** carry the total, the 2 `[from your Staff of Nin]` correctly carry none. **0** unpriced player-aimed damage abilities. (The 86 other Staff-of-Nin stack rows are its `@each my upkeep:draw:1` trigger, not the damage ability.) |

### Lane S

| # | verdict | evidence |
|---|---------|----------|
| **C6** sequential DRAW PRICE fold | **UNTESTED** | 54 rows carry a `[DRAW PRICE` clause; **0 rows carry two**. No window arose. (0 rows anywhere carry two `you would be at` figures, so the wave-60 defect did not recur — but the enabling shape did not occur either.) |
| **C12** one-shot draw grant | **PASS** | 18 `[DRAW GRANT (one-shot, on resolution):` rows — Silverquill Command 16, Peer into the Abyss 2. Both Peer rows carry the converter COUNT and NAMES (`162v123` seat-162 s16 *"converters on your battlefield: 2 - Underworld Dreams, Fate Unraveler, so those 22 draws take 44 off them"*; s21 *"3 … 21 draws take 63 off them"*), and **every named permanent is on the seat's own battlefield line of the same prompt** (checked both). The wave-60 shape (a Peer row with no converter clause, and a reply asserting a punisher nobody controlled) did not recur. Outcome: s21's cast took the opponent 15 → 0 on the next record. |
| **C10 (NET half)** best-X badge | **PASS** | 156 badge rows. 9 carry a negative NET, and all 9 are lethal (`125v162` seat-125 s38–s46, life 8 → 1). **All 9** carry the new tail — *"but NET -20 life for this cast puts you at -18; this KILLS you. No listed X leaves you alive"*. **0 bare lethal badges** (wave 60: 1 of 1, and the seat took it). Outcome: the seat declined the row 9 of 9. |
| **C5 (a)** LOOP HALF PENDING affordability | **PASS** | 137 `LOOP HALF PENDING` renders: 70 graveyard/exile, 45 their hand, 4 other, **18 with the held half in the seat's OWN hand**. **18 of 18** carry `You CAN cast` (7) or `You CANNOT cast` (11). **0** CANNOT beside a live `Cast <half>` row — the one apparent hit (`126v162` s29, Exquisite Blood) is the string appearing inside the PLAN echo, not a menu row. |
| **C5 (b)** closed-pair clause (the refutation) | **PASS** | 219 prompts have Sanguine Bond + Exquisite Blood on ONE battlefield line (83 the seat's, 136 the opponent's); **219 of 219** carry `Both halves of a life LOOP are on YOUR|THEIR battlefield (…)`. 255 prompts carry a `LIFE-TO-DAMAGE CONVERTER` block. The wave-60 "5 of 5 closed-pair renders silent" finding stays refuted. |

### Lane T

| # | verdict | evidence |
|---|---------|----------|
| **1 · C7a** hand castability coverage | **PASS** | 1,991 `Your hand (N cards):` renders, **8,763 non-land entries, 0 untagged** (co-occurrence 1.0). 1,414 prompts carry `[castable now]`, 897 `[cannot pay now:`, 1,010 `[no cast row now:`. The wave-60 `named_row_reask`-on-a-HAND-CARD cluster (8 across deck123 + deck162) is **0**: the 4 remaining `named_row_reask` fallbacks all name a card that is not in the hand at all (`126v125` s44 "Attack with Vampire", s65 "Sorin, Lord of Innistrad", s82 "Staff of Nin" — the hand was two Tributes to Hunger). |
| **2 · C7a (outcome)** no PLAN on a blocked cast | **FAIL** | 1,161 windows have at least one blocked hand card and a PLAN line; **457 PLANs name a blocked cast** literally, most of them legitimately deferred ("next turn", "in Main 2"). Filtering to a first-sentence, non-deferred, `cannot pay now` card leaves **12** hard violations, e.g. `123v125` s30 *"Cast Thraben Doomsayer {1}{w}{w}"* over `[cannot pay now: needs {1}{w}{w}, your 5 untapped sources cannot pay it]` (that record is one of the 4 re-asks), `130v126` s27 *"Cast Starstorm at X=4"* over `[cannot pay now: … you have 0 untapped sources]`, `130v123` s62 Hammer of Bogardan. The `0` claim is false; the magnitude is ~1% of the windows where it could fire. |
| **3 · C7b** land-drop enters-tapped bracket | **FAIL (correctness) / PASS (coverage)** | See HIGH-1. Coverage: every land in the corpus whose primitive carries `tap(noevent)` got a bracket, and the 11 land names with no bracket have no `tap(noevent)` (checked each). Correctness: **131 of 172 `Play <land>` brackets assert unconditional `enters TAPPED - it makes no mana this turn` on a land that only conditionally enters tapped.** |
| **4 · C8** choose-ONE reveal arity | **PASS** | 10 `reveal` records. **10 of 10** carry the ONE-number protocol (`PUT: followed by the ONE card number … ONE number, never a list`); **0** carry `comma-separated (e.g. "PUT: 1, 3")`; **0** `PUT:` replies carry more than one number; **10 of 10** narrations (`chosen_text`) name exactly one card. The `146v125` s18/s19 two-card-take shape did not recur. |
| **5 · C9** dungeon ventures priced to completion | **SPLIT: render PASS · outcome FAIL** | 14 dungeon menus, 42 dungeon rows, **0 bare `[dungeon: N rooms;`**. Counts are exactly the predicted 4 / 7 / 4: Tomb of Annihilation *"completes after 4 ventures (one branch of its room path completes at 3); 5 rooms printed"*, Lost Mine *"completes after 4 ventures; 7 rooms printed"*, Mad Mage *"completes after 7 ventures; 9 rooms printed"*. The symmetry clause fired on **2 of 2** Acererak-sourced asks, all three rows carrying a source verdict. **Outcome fails: Lost Mine of Phandelver was taken 14 of 14 times overall and 2 of 2 on Acererak** — the pick is not source-driven and the symmetry fix did not move it (N=2). |
| **6 · C11** `{spare: …}` conditioned | **PASS** | 18 `{spare: you control L lands already; the most expensive card in your hand costs H}` renders; **L > H+1 in 18 of 18** (lowest margin 8 vs 6). 25 bare-count clauses render the unverdicted form. The `130v123` s9 bait shape did not recur; every cleanup discard that sent a land was at 13–15 lands. |

### Lane U

| # | verdict | evidence |
|---|---------|----------|
| **1 · C10a** sweeper CAST-row rank | **PASS (render and outcome)** | 23 windows carry a `[<- board sweep: THEIRS n / YOURS m …]` marker; **exactly one marker per window, 0 with two**, always on the Cast row whose own `{right now: destroys/exiles N}` clause matches the marker's THEIRS. The `YOURS > THEIRS` rail fired truthfully twice (`130v162` s45/46: `THEIRS 1 / YOURS 5 … (it takes more of YOURS than of THEIRS)` — and the seat declined both). No cast menu offered a `destroys N>=1` row with no marker. **Outcome: deck130's Starstorm went from 0 of 51 to 4 casts of 64 offers, and 2 of the 4 marker windows were taken** (`130v126` s25 at THEIRS 2, `130v162` s19). |
| **2 · C10b** damage-wipe survivors | **PASS** | 8 `{right now: … deals N to each if all N attack}` clauses; **8 of 8** carry `DIES at N (before prevention): …` and `SURVIVES: … - P power of theirs survives it`. **0** survivors listed with toughness <= N and no out named. Wave 60's bare count did not recur. |
| **3 · C13** transport phase + deadline field | **PASS** | The single `transport` stamp carries `connect_ms=20000` and `phase=wall` and is classed `wall_miss` — the exact adjudication lane U pinned. **0** `phase=connect` records classed `wall_miss_unrecorded`, **0** `phase=wall` classed `transport_error`. `deadline_pct` is present on **2,035 of 2,035** records (wave 60: absent from the whole corpus), and the abandoned wall-miss record now carries its real `latency_ms` (900,024) rather than `-1`. |
| **4 · C14a** HOLD across the turn boundary | **PASS** | 1,707 `holding priority at the … seam` stderr lines across 21 games. Walking each file per seam and clearing on `hold re-opened`, **60 held windows continue past a turn header with no intervening re-open** (e.g. `125v126` stderr lines 467–481, priority seam, turn 6 → turn 7). The falsifier (a held window whose rendered rows differ from the held set) is **not checkable from the translog** — suppressed windows are not recorded. |
| **5 · C14b** per-menu re-open rule | **SPLIT: render PASS · accuracy UNTESTED** | 1,275 prompts carry `[hold check: …]`, **exactly one line each, 0 with two**; 849 carry none (first window at a seam claims nothing, as designed). Forms split 974 `N row(s) above … new since the last window` / 301 `every row above was also on the menu`. The accuracy falsifier cannot be evaluated: the comparator is the last window **at that seam**, which is frequently a suppressed window absent from the translog — every apparent mismatch I chased (e.g. `152v125` s25 priority-seam vs s26 cast-seam, both counters reading 1 then 2) turned out to be two different seams' independent counters. |
| **6 · C14c** record stops carrying the overrun | **PASS** | **0** records with `post_plan_overrun > 1200` and no `reply_trimmed_bytes`. 31 records were trimmed, dropping 76,335 B from the stored replies while the measurement is unchanged — summed `post_plan_overrun` 99,241 B and `post_answer_overrun` 552,751 B are still computed on the full reply. Largest single overrun still recorded: 10,449 B (`126v125` s87). |

### Lane V (the eight Codex-review fixes)

| # | verdict | evidence |
|---|---------|----------|
| **R1** mandatory chooser offered no decline | **PASS** | **`PUT: none` appears 0 times in 2,035 prompts.** The 2 mandatory bare-chooser reveals (`146v152` seat-152 s44/s67, Augur of Autumn) carry `This choice is NOT optional: while a card qualifies you must take one.` alongside the ONE-card protocol. No Pelakka Predation window arose, so the narration half is unexercised. |
| **R2** hold retires on ANY row change | **PASS** | `hold_windows_skipped` fell 2,013 → 1,707 (86.4% → 83.9% of decisions) even though C14a removed the turn gate, which is the predicted direction. **1** `stale_echo_in_range` parse note in the whole corpus, no cluster. `hold re-opened` fired 291×. |
| **R3** enters-tapped reads the land's own ETB line | **UNTESTED as worded, and a NEW defect of the same family** | No Bant Panorama and no shockland was ever offered, so the fetch-land and shockland halves had no window. But see HIGH-1: the classifier still mis-reads the **checkland** shape, where the condition is on the line AFTER `tap(noevent)`. |
| **R4** late-HTTP-error retry budget | **UNTESTED** | The corpus's only transport failure is `http=0, curl=28` at 100% of the deadline. No non-200 status appeared anywhere, so `gptDeadlineMissed`'s new arm never fired. **0** records carry `wall_miss` with an HTTP status other than 0/200 — vacuously. |
| **R5** wipe survivors read protection/prevention | **PASS (scope) / UNTESTED (protection)** | **8 of 8** rosters carry `(before prevention)` exactly once. All 8 are Lightmine Field at deck125 and every one has an EMPTY `DIES at N` list, so no roster ever had to classify a protected or regenerating creature. |
| **R6** crack-back sources must be activatable | **FAIL** | See HIGH-2. 21 renders of `they can also aim ability damage at you that no block stops`: 4 name Staff of Nin (correct — `{T}` cost, no mana) and **17 name Lolth, Spider Queen "(8 per activation)"** on a board where Lolth stood at **1 loyalty**. |
| **R7** no 4-branch cap on modal draw scans | **UNTESTED** | Every one of the 18 `DRAW GRANT` tags renders exactly **2** branches (Silverquill Command's draw modes; Peer's two targets). No card with 5+ priced draw branches was offered, so the cap's removal is unobserved. |
| **R8** blocking-lifegain ceiling uses the pairwise gate | **PASS (no adverse window)** | **0** `ATTACK TOTAL` lines claim blocking lifegain where no offered attacker can be blocked. The 2 lines that do claim it are correct: `162v126` s37 sums 3 blockers against A1 (blockable) while A2 Master of the Feast is separately flagged `no creature they control can block this attacker`; `123v126` s52 sums 2 blockers against 97 blockable attackers. The discriminating shape (an all-evasion attack beside a blocking-trigger body) did not occur. |

**Tally over all 30 predictions (R 5, S 5, T 6, U 6, V 8): 20 PASS · 3 FAIL · 4 UNTESTED · 3 SPLIT.**
FAIL = C7a-outcome, C7b, R6. UNTESTED = C6, R3, R4, R7 (no window arose in any of the four).
SPLIT = C3, C9, C14b — the render half passed and an outcome or sub-clause did not; counted in
neither column. R5 is counted PASS on its scope tag with its protection half unexercised.

---

## 3. Engine / render / narration defects

### HIGH-1 — the land-drop `enters TAPPED` bracket contradicts the sentence it quotes, on 131 of 172 rows

`Play Glacial Fortress [enters TAPPED - it makes no mana this turn: "Glacial Fortress enters tapped
unless you control a Plains or an Island."]`
— `125v152` seat-125 seq 31, turn 24, and the seat's own battlefield line in that prompt lists both
Plains and Island, so the land would have entered **untapped**.

The classifier (`landEntersTappedTag`, kept by lane V's R3) decides "unconditional" from the
`tap(noevent)` line alone. Three primitive shapes defeat it:

* **the condition on the NEXT line** — `auto=tap(noevent)` / `auto=aslongas(plains,island|myBattlefield) untap`
  (Glacial Fortress mtg.txt:46617-8, Drowned Catacomb 32505-6, Sunpetal Grove 115716-7). **70 rows.**
* **`if <expr> then tap(noevent)` with no paren after `if`** — Deserted Beach
  (borderline.txt:27515), Overgrown Farmland (81143), Lair of the Hydra (64291), Hive of the Eye
  Tyrant (53392). **56 rows.**
* **a reveal-gated ETB** — Shineshadow Snarl `if type(*[plains;swamp]|myhand)~equalto~0 then
  tap(noevent)` (101328). **5 rows.**

Only Arcane Sanctum (9), Dismal Backwater (19) and Forgotten Cave (13) — the 41 rows whose text
really is unconditional — are right. Isolated Chapel, Woodland Cemetery and Seachrome Coast render
the correct `UNLESS` form only because their primitives happen to put `aslongas(...)` in FRONT of
`tap(noevent)` on one line. This is a false surface in the direction wave-60 H1 named (the pilot
plans its mana off this bracket), reversed: it now UNDER-states available mana on the most common
duals in the pool. The bracket also self-contradicts inside one pair of brackets — a verdict of
"makes no mana this turn" immediately followed by the card's own "unless" sentence.
Repro: `/usr/bin/grep -c "enters TAPPED" ` any prompt containing `Play Glacial Fortress`.

### HIGH-2 — `CRACK-BACK` names an unusable planeswalker ultimate as an aimable damage source, 17 renders

`CRACK-BACK NEXT TURN: 2 of their creatures will be able to attack …, for up to 4 from combat as
their board stands - you would be at 9 or lower - and that number is a FLOOR, not a ceiling: they
can also aim ability damage at you that no block stops - Lolth, Spider Queen (8 per activation)`
— `146v152` seat-152 seq 41 (and s42-49, s59-61, s63-68), turn 18+.

The same prompt's opponent battlefield line reads `Lolth, Spider Queen {3}{b}{b} [planeswalker]
[counters: 1x loyalty]`. The matched script line is
`auto={C(0/0,-8,Loyalty)}:name(-8: Get emblem on combat damage) emblem transforms((,newability[@combatdamagefoeof(player) … damage:8minusoplifelostminusend opponent]))`
(planeswalkers.txt:2170). Three separate errors compound:

1. the cost head is **loyalty, not mana** — `crackBackCostAffordable` prices it against
   `potentialColorReach` and finds it free, so R6's new gate passes an ability that needs 8 loyalty
   on a permanent holding 1;
2. the damage is an **emblem's triggered rider on combat damage from their creatures**, so the
   clause's own promise "that no block stops" is exactly backwards — blocking is what stops it;
3. the amount `8minusoplifelostminusend` is an **expression**, which lane R's own scope note said is
   skipped; the render prints a flat `8`.

The consequence is not cosmetic: the clause is what promotes the head from a ceiling to
`or lower … a FLOOR, not a ceiling`, so 17 windows over-state the incoming crack-back by 8 on a
board where the source can never fire. Symmetrically, the SAME gate is silently dropping 53 true
clauses (Hive of the Eye Tyrant, Siege-Gang Commander) because their real mana costs exceed the
opponent's CURRENT open mana — a next-turn forecast priced against this turn's untapped sources.
The gate is wrong in both directions at once.

### MED-1 — the hand castability bracket contradicts a live Cast row for the same card, 2 windows

`Your hand (…): Overgrown Battlement {1}{g} (0/4) [creature] [cannot pay now: needs 2 mana, you have
0 untapped sources]` over `1. Cast Overgrown Battlement {1}{g} (0/4) {card text: …}`
— `126v162` seat-126 seq 22. Second instance: `130v125` seat-130 seq 75, Stone Rain
`[cannot pay now: needs 3 mana, you have 0 untapped sources]` over `1. Cast Stone Rain {2}{r} {kills
whichever you target: …}`. Lane T's C7a states the two surfaces read one oracle and "the hand line
cannot contradict the cast rows in the same prompt"; they do, twice. Everywhere else the bracket's
source count agrees with the `Mana available: N total` line **0 exceptions in 897 blocked entries**,
so the divergence is in the cast-row list, not the counter — the row survived a mana snapshot the
hand pass did not see.

### MED-2 — the venture pick is not source-driven and the C9 symmetry clause did not move it

14 of 14 dungeon choices across 4 different venture sources (Triumphant Adventurer 8, Nadaar 3,
Acererak 2, Barrowin 1) are **Lost Mine of Phandelver**, including 2 of 2 Acererak asks that now
carry the symmetry clause on all three rows. Lost Mine and Tomb of Annihilation both read
`completes after 4 ventures`; Lost Mine's reward is "Draw a card" against Tomb's 4/4 deathtouch
token. The prompt is not what is deciding this row — position 2 or the name is. A per-deck reader
sees one instance; across decks it is 14/14 with no variance at all.

### MED-3 — narration typo in the reveal ask, in every reveal prompt that uses it

`Choose ONE card that goes to "put in hand"; every other card goes to "put on bopttom".`
— `146v152` seat-152 seq 44 and 67. The engine's own zone label is misspelled in the ask the model
answers from.

### LOW-1 — a `deals N to each` roster has never had to name a victim

All 8 damage-wipe rosters read `DIES at 1 (before prevention): none.` The R5 protection/regeneration
half and the C10b split are both unexercised by this corpus, so nothing about them is validated —
they are a shipped surface with zero live coverage. Worth an explicit fixture rather than another
wave of hope.

### LOW-2 — prompt economy is still a third declines

668 of 2,035 decisions (32.8%) are pure declines (`Cast nothing right now`, `Hold priority`,
`Pass priority`), at a median 10.2 KB and a mean 11.5 KB per ask — about 7.7 MB of the 23.4 MB
corpus spent to say "no". C14a's unbounded hold cut `hold_windows_skipped` per decision only
slightly (86.4% → 83.9%), because R2's stricter retirement pulls the other way. GAME LOG is down to
57.6% of prompt bytes from 62.9%.

---

## 4. Lategame specimen

`wave61/lategame-specimen.txt` — the deck125 seat of `125v123`, **seq 105, turn 33, Upkeep**,
my_life 61 / opp_life 1. The second Staff of Nin ping; the very next record in that file is
`gameend`. 20,930 bytes of prompt for a two-row menu at a decision with one right answer.

---

## What I did NOT check

* **No git, no build, no wagic run, no suite, no PARSETEST.** Every verdict above is read off the
  rendered `prompt` and the primitives; I did not open `AIPlayerGPT.cpp` to confirm which code path
  produced any line, and I did not re-run any lane's gate.
* **The C14b accuracy half and the C14a falsifier are unverifiable from this instrument.** Suppressed
  windows write no translog record, so neither "the hold-check note's `same` claim is true" nor "no
  held window's rows differed" can be tested. I state that rather than infer either way.
* **The slot-key fix is confirmed only by its symptom** (16 drops, 0 livelocks, 1.4 h). No record in
  the corpus shows the ask key or the prompt tail directly.
* **C3's overrun attribution is partial.** I traced 3 of the 8 crack-back overruns to a named cause
  and left `146v130` seat-146 s12 (forecast 2, actual -3) unattributed; a fuller trace needs the
  opposing seat's log read end to end, which I did not do.
* **The 457 "PLAN names a blocked cast" figure is a string match**, not a semantic read. The 12
  strict violations were eyeballed; the other 445 were classified as deferred by keyword, and some
  of those are certainly misclassified in both directions.
* **Card facts**: I read Glacial Fortress, Drowned Catacomb, Sunpetal Grove, Woodland Cemetery,
  Isolated Chapel, Arcane Sanctum, Deserted Beach, Overgrown Farmland, Shineshadow Snarl, Seachrome
  Coast, Dismal Backwater, Forgotten Cave, Lair of the Hydra, Hive of the Eye Tyrant, Siege-Gang
  Commander, Staff of Nin and Lolth, Spider Queen out of the primitives. **I checked none of them
  against Scryfall.** Lolth's Oracle -8 wording is quoted from the primitive's own `text=` line.
* **I did not read any deck guide**, did not adjudicate any per-deck reviewer's items, and wrote no
  proposals — no evidence here paid for one.
