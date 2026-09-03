# Wave-55 seat review — ENGINE / NARRATION

Corpus `matchups-20260903-074354` (binary = hermetic `make -B` of master **b228b8648**; wave-55 step-1
lanes A 8fe5c6682, B f7d13d99d, C 38ccaae55, D 8f9ee5736, E d56efcdaf on base 8a1768d1c = wave-54
step-1 + P purge + Q autotap + **R stall-floor hotfix**). Model qwen36-35b-a3b on Spark :8084.
**42 seat logs, 3,264 records** = 42 `gamestart` + 42 `gameend` + 7 `recovery` + 2 `wall_miss` +
**3,171 decisions** (ask 2,416 / priority 555 / attackers 90 / discard 68 / blockers 21 / reveal 12 /
bottom 9). **21/21 natural**, 0 timeouts, 0 crashes, 0 hangs. Wall 2 h 03 min; recorded inference
**22.98 h = 26.09 s per decision**. Max seat 316 (`152v125`, 65 turns). Max prompt **30,031**
(`152v125` seq 314); prompts > 30 K: **1**.

Every string quoted is the emitter's ACTUAL output, read with python over the JSONL (`prompt`,
`options_text`, `reply`, `chosen_text`, `events`, gameend fields) and `/usr/bin/grep` over the
`.stderr` files. **All row counts are taken from the rendered `prompt`** (numbered `N. ` lines), never
from the `options` array — the difference is large this corpus and is itself a finding (see D-6).
Card facts verified against `projects/mtg/bin/Res/sets/primitives/mtg.txt`: Sphinx's Revelation,
Starstorm, Devour Flesh, Spark Spray. No Scryfall cross-check was needed (no claim turns on a
printing detail). Read-only throughout: no build, no run, no git, no edit under `bin/Res` or `src/`.

---

## 1. D1 DISCHARGE — confirmed by the records, and the residual class named

| measurement | wave 53 | wave 54 | **wave 55** | verdict |
|---|---|---|---|---|
| ` - releasing so the game advances` (stall-floor releases) | — | 468 | **0** | **PASS** |
| `AIPlayerGPT: dropping stale async answer` | 63 | 488 | **43** | **PASS** (< 100) |
| `giving this decision to the heuristic` | — | 1 | **0** | **PASS** |
| `interrupt window held by …` | — | 936 | **0** | line is compile-gated out |
| opponent-turn share of decisions | 30.2 % | 19.9 % | **33.8 %** (1,071 / 3,171) | **PASS** (≥ 28 %) |
| per-decision inference | 21.7 s | 23.9 s | **26.09 s** | **FAIL** (≤ 22 s) |

Lane R closes wave-54 HIGH #1 completely. The window economy is not merely restored, it is the best
of the three corpora: opponent-turn **casting** windows 403 → **890** (wave-54 prediction ">700":
PASS), seats recording **zero** opponent-turn decisions 16/42 → **9/42** (predicted ≤ 4: FAIL, but
all nine are short seats of 16–31 total decisions in 9–15-turn games).

### The 43 drops, every one classified

Method: for each `dropping stale async answer` line, the nearest preceding GPT ask/seam line, the
next ask issued, and the stack activity in between (`10DrawAction`, `StackAbility`, `NextGamePhase`,
`Resolving Action`). Files/lines are in the table.

| class | n | shape | example |
|---|---|---|---|
| **E1 — turn-flip seam alternation, land-drop arm** | **17** | in-flight cast/priority-seam answer for the last window of the opponent's turn is displaced by the seat's own **land-drop ask** at Main 1 | `game-123v130-1788439454.stderr:479` (prev `priority seam` turn 4 → next `Land drop: play Mountain now?`); also :820, :899, :998, :1139, :1486 |
| **E2 — turn-flip seam alternation, cast arm** | **11** | same shape, but the displacing window is the seat's own **casting ask** at the SAME named phase (`Upkeep/opp` → `Upkeep/own` 7×; `cast seam` → `Main 1/own` 4×) | `game-125v123-1788439466.stderr:2576` (cast seam turn 32 → `Casting decision (Upkeep, YOUR turn)`) |
| **E3 — stack resolution (wave-53's shape)** | **9** | a `StackAbility` resolved under the in-flight ask; the prompt legitimately changed | `game-125v123-1788439466.stderr:1191, :1450, :1865` (Staff of Nin / activated-land abilities) |
| **E4 — phase-only rebuild, board unchanged** | **2** | only `NextGamePhase` actions in between; no draw, no stack resolution | `game-146v130-1788439468.stderr:1928`, `game-152v123-1788439438.stderr:1986` |
| **E5 — reveal driver double-drop** | **2** | two consecutive drops with NOTHING between them, then `reveal put 1 of 3 to option one in one reply` | `game-152v123-1788439438.stderr:3460-3461` |
| **E6 — targeting sub-ask (`Cancel`)** | **2** | `targeting with Cancel -> 1 target(s)` then a rebuilt cast ask | `game-125v130-1788439456.stderr:672, :689` |

**Answering lane E's question directly: 28 of 43 (65 %) are the seam-alternation class** (E1+E2) —
the land-drop ask and the casting ask alternating in the single async slot across the turn flip.
They are NOT on a byte-identical board: 28 of 28 have `10DrawAction` resolving in between, so the
hand changed even though no permanent did. Only **4 of 43 (E4+E5) are drops on a genuinely
unchanged board**. **9 of 43 are wave-53's stack-resolution shape.** Cost: 43 × 26.09 s ≈
**0.31 h** of bought-and-discarded inference (wave 54: ≈ 3.2 h). The class is real but is now a
rounding error; I do **not** recommend spending a wave-56 lane on it beyond the cheap fix below.

**Cheap fix shape (lane A of wave 56, as review-carry directs):** the async slot is keyed on the
prompt string. Key it on `(seam, turn, phase, boardKey)` instead and let a superseded window's answer
be *discarded silently without a re-issue* only when the seam changed; when only the phase advanced
inside the same seam, the in-flight answer is still valid for the row set that survived. Falsifier
for wave 56: with the slot keyed on the seam, E1+E2 fall below 6 and the total stays under 20.

### Why per-decision inference went UP (26.09 s vs 23.9 / 21.7) — measured, not guessed

Three candidate causes were tested and two are refuted.

1. **Longer replies — REFUTED.** Reply length went **down**: mean 311 → **284** chars, p50 206 →
   **191**, p99 3,257 → **2,440**. Total reply volume 0.69 M → 0.90 M chars, but that is the
   decision count (+43 %), not the reply.
2. **Bigger prompts — TRUE but not the cause.** Prompt mean 10,740 → **12,174** (+13.4 %), total
   volume 23.8 M → **38.6 M** (+62 %). But `corr(prompt_chars, latency) = −0.226` inside this corpus
   — bigger prompts are *faster*, not slower (late-game prompts get short decisive replies and hit
   the prefix cache). Prompt bytes cannot explain a rise.
3. **Server contention — the actual cause.** Binned like-for-like, wave 55 is slower in **every**
   prompt-size bin and **every** reply-size bin:

   | prompt bin | w54 n / median | w55 n / median | ratio |
   |---|---|---|---|
   | 5 K–10 K | 743 / 17.7 s | 1,040 / 24.2 s | **1.37** |
   | 10 K–15 K | 605 / 6.7 s | 794 / 11.8 s | **1.76** |
   | 15 K–20 K | 297 / 4.2 s | 453 / 5.4 s | 1.28 |
   | 20 K–25 K | 149 / 3.9 s | 229 / 4.4 s | 1.15 |

   Effective concurrency on the shared :8084 server: wave 54 ≈ (14.67 h + ~3.2 h dropped) / 1.75 h
   wall = **10.2×**; wave 55 ≈ (22.98 h + 0.31 h) / 2.05 h wall = **11.4×** (+12 %). Median latency
   rose +12 % (11.9 → 13.4 s). Those two numbers match. **The recovered opponent-turn windows raised
   aggregate queue depth and the server charged for it.** The body of the distribution barely moved
   (p75 29.4 → 29.7 s, p90 54.4 → 55.0 s); the mean rose because the tail thickened — calls > 120 s
   went 45 → **78**, carrying 19.2 % → **24.5 %** of all inference.

   **This is not an engine regression and no engine change will fix it.** The lever is either fewer
   concurrent games or fewer windows, and the second is exactly what the HOLD row is for (§3).

---

## 2. VERDICT TABLE — set B, adjudicated by the emitter's actual strings

#188 three-number audits appear as **old renders / new renders / takes**.

| Lane | Prediction | Verdict | Evidence |
|---|---|---|---|
| **A** | HOLD row text is the new wording; old strings 0 | **PASS** | **0 / 2,352 / 282.** Verbatim: `Hold priority for the rest of this turn: pass now, and do not ask me again unless the board changes (any change re-opens this window; you give up no cast) {taking this row skips the rest of this turn's identical windows}`. **LAST row in 2,352 / 2,352.** Both old strings render **0** |
| A | HOLD offered on OWN-turn windows too | **PASS** | Renders by turn: **own 1,323 / opp 1,029**; by kind ask 1,797 / priority 555. Wave 54 offered 426, opponent-turn only |
| A | HOLD takes, #208 both ways | **PASS on rate, both ways** | **all 282 / 2,352 = 12.0 %**; **opponent-turn 145 / 1,029 = 14.1 %** (wave 54's comparable: 43 / 426 = **10.1 %**); **own-turn 137 / 1,323 = 10.4 %**. The row works on own turns — that half of wave-54 HIGH #2 is discharged |
| A | `hold_windows_skipped` sum > 72 | **PASS** | non-zero on **21 / 42** gameends (was 7/42), **sum 735**, max 175. No seat has skips without a take |
| A | identical-declined runs < 12 %, max run < 20 | **max PASS, share FAIL — and the share is a denominator artifact** | Wave-53 predicate (key `(turn, options_text)`, run ≥ 3, HOLD-take NOT a decline) reproduces the harvest exactly: **681 / 3,171 = 21.5 %, 96 runs, max 16** (wave 54, same code path re-run by me: **356 / 2,212 = 16.1 %, max 50** — reproduced exactly). #208 both ways: **opponent-turn only 619 / 1,071 = 57.8 %** vs wave 54's **296 / 477 = 62.1 %** — a genuine **−4.3 pt** improvement once the window mix is normalised. Own-turn only: **71 / 2,040 = 3.5 %**. The headline rise is the opponent-turn share going 19.9 % → 33.8 %, not a regression. **Max run 50 → 16 is a clean win and closes wave-54 HIGH #2** |
| A | own-turn runs ≥ 20 **0** | **PASS (0)** | Longest own-turn run is **12** (16 own-turn runs ≥ 3, sum 71). Wave-54's 50-run Blockers class does not recur |
| A | `pass_hold_ambiguous` ≤ 4; `CHOICE: 0 (pass)` never stamps | **PASS (0)** | The note renders **0** times (was 16). The exact-label tiebreak works |
| A | HOLD takes in declined-note N ≥ 3 windows ≥ 20 % | **FAIL (67 / 753 = 8.9 %)** | Down from 10.4 %. **Coverage is not the problem: the HOLD row is on the menu in 753 / 753 of these windows.** Own-turn 20/201 = 10.0 %, opponent-turn 47/552 = 8.5 %. See HIGH #1 |
| A | declined-note reaches the own-turn Main 1 / Main 2 pair | **PASS** | Declined-note renders on own-turn **Main 1 112**, **Main 2 160** (plus Upkeep 50, Draw 36, Blockers 33, End 17, Cleanup 2, Combat begins 3). Forms: `[you declined this exact list N times already this turn]` 915 + singular 297 |
| A | decisions inside one mandatory life-loss loop phase < 5 | **UNTESTED (no window)** | No mandatory life-loss loop arose; the longest single-phase identical run is 16, in an ordinary priority seam |
| A | served PLAN blocks > one paragraph **0**; age stamp N/N | **PASS (0 / 3,044)** | 3,044 served blocks, **0** multi-paragraph, **0** missing the `, N window(s) ago on turn N` stamp. `"<card>" is no longer on your menu` renders **622** |
| **B** | `all_assignments_illegal` records with no follower `[RE-ASK]` **0/N** | **UNTESTED (N = 0)** | The fallback **never fired**. All 21 blockers records carry `dropped_assignments: 0` — no assignment was pruned anywhere in the corpus. The `_exhausted` branch is still un-executed (carried) |
| B | menace header on 100 % of blockers windows with a menace attacker, 0 % otherwise | **half PASS, half UNTESTED** | **0 of 21 blockers windows had a menace attacker** (`menace` does not appear in any blockers prompt), so the positive arm is UNTESTED (N = 0). The negative arm is **PASS 21/21**: the header string `need TWO or more blockers` renders **0** times. **0 / 0 / 0** |
| B | the fallback field carries `: <pairs + reason>` | **UNTESTED (N = 0)** | No `all_assignments_illegal` record exists |
| B | `[RE-ASK]` arm live | **PASS on firing, FAIL on the quote (see HIGH #2)** | 12 renders: 8 of the `Your CHOICE line takes row N ("CHOICE: n (name)")` form + 4 of the `"<name>" is not on this list` form. 6 of the 8 recovered the model's real row; **2 quote the WRONG coded line** |
| **C** | X menus with no marker **0** | **FAIL (11 / 14) — see HIGH #3** | 14 X menus. **3** carry a `[<- …]` marker, all three on Starstorm (a kill-list X). The 11 Sphinx's Revelation menus carry **no marker of any form** and no `{leaves N sources}` fit clause. Marker forms, all three distinct and correctly on the BOTTOM row of a collapsed run with both kill lists restated: `[<- most kills at any affordable X that costs you nothing - at X=4 that is THEIRS: Briarbridge Tracker, Elite Spellbinder; YOURS: none]` (2) and `[<- best trade: the most of THEIRS at the smallest cost to YOURS - at X=1 that is THEIRS: Silverquill Silencer; YOURS: Goblin x2]` (1). **0 / 3 / 2** |
| C | X menu largest-first (owner ruling) | **PASS 14/14** | Option 1 is the largest X in 14/14, and 14/14 carry the header `Every listed value is affordable; option 1 is the LARGEST X (X = N)` |
| C | takes of a `kills THEIRS: none` row while another affordable row names an opponent creature **0** | **PASS (0 / 7)** | 220 `{kills whichever you target: THEIRS - none; YOURS - …}` rows render and **7** are taken (all Spark Spray / Hammer of Bogardan, deck130 seats). On every one of the 7 I checked the rest of the menu: **0 carried another row naming an opponent creature**, so the guard's actual condition never fired |
| C | both-player `target=player` rows with a one-sided verdict **0/N** (was 178/180) | **PASS (0 / 151)** | Every Devour Flesh row now prints both halves, e.g. `{right now: they control 1 creature - Fate Unraveler (2/3) is sacrificed, they gain 3 - the sacrificing player gains, not you; YOU control 1 creature - targeting yourself sacrifices Bloodline Keeper, and you gain …}`. Primitive verified: `target=player / auto=…targetedplayer`. **Takes 6.** Wave-54 D-2 CLOSED |
| C | `edictSelfClause` + `{this row does not pick a target …}` commit clause at N ≥ 2 | **PASS (170 / 170)** | Every both-player row carries `{this row does not pick a target yet - taking it asks you next which …}` — the exact question the wave-54 model asked in prose and got no answer to |
| C | casts of a card named in an opponent `[named:` tag with no `[NAMED BY THEIR <src>: … you would be at K]` **0** | **UNTESTED (N = 0)** | 49 opponent-side `[named: X]` tags render (byte-identical: `[named: Howling Mine]` 22, `[named: Pyrite Spellbomb]` 21, `[named: Staff of Nin]` 10 on their side; 33 own-side). **In 0 of those windows did the seat hold a `Cast <that card>` row**, so the price never had a window. `[NAMED BY THEIR` renders **0 / 0 / 0** |
| C | single-target `{kills:` lists without the THEIRS/YOURS split **0** | **PASS (0 / 341, by ground truth)** | 620 kills/removes annotations: split forms 242 (`{kills whichever you target: THEIRS - …; YOURS - …}` 223, `{removes whichever …}` 19), bare forms 378. I cross-checked every bare list against the seat's own `Your battlefield` line: **0 of 341 bare `{kills:`/`{removes:}` lists name a creature the seat controls**, so the missing split is correct in every case |
| C | ability rows pointing damage at a player without a life total **0** (was 73) | **PASS (0 / 251)** | All 251 rows aiming damage at the opponent carry `they would be at N` / `at life N leaves them at N` |
| **D** | reveal/cleanup menus collapse `(copy k of n)` duplicates | **PASS on the row count; the tag itself is a KEEPER, not a removal** | The collapse is real and visible in the array-vs-render gap: reveal `126v146` seq 9 array **50 → 18 rendered rows**, `126v130` seq 7 **51 → 16**, `126v162` seq 6 **49 → 15**, `123v125` seq 9 **47 → 10**, `126v152` seq 7 **50 → 16**, `126v162` seq 25 **31 → 21**; and 23 of the 68 `discard` records render exactly **3 fewer** rows than their array. Discard menus now total **502 rendered rows, max 11, mean 7.4**. The literal `(copy N of N …)` still renders **7,071** times (`(copy N of N in your hand)` 7,003 + `(copy N of N in this list)` 68) — it is the hand/list DISAMBIGUATOR, kept deliberately so a two-copy pick is nameable, not the duplicate-row waste wave 54 measured. Total residual duplicate *identical* rendered rows corpus-wide: **6** in 4 records (target ≤ 12) |
| D | one decode line per later source block; residual `#N` rows ≤ 12 | **PASS** | Decode line renders **14**, verbatim e.g. `12-15. The same 4 options as 8-11, with Siege-Gang Commander #2 as the source instead of #1 (12 is the same choice as 8, 13 as 9, and so on to 15 as 11) x4` (Siege-Gang 8, Staff of Nin 6). **Records with > 3 rendered rows of one `#N` shape: 0** (wave 54: 41 records / 425 rows / ~153,591 chars). **Wave-54 D-3 CLOSED.** Largest rendered menu is 21 rows |
| D | `chooseaname` header names a CARD NAME menu 9/9 | **PASS (4 / 4)** | **0 / 4 / 4.** Old string `Choose one mode for Silverquill Silencer:` renders **0**. New: `Choose a card NAME for Silverquill Silencer - this is a card-NAME choice, NOT a mode: every row below is the NAME of a card, and the one you pick becomes the name Silverquill Silencer is set to.` N is 4 not 9 — the card came up less often |
| D | `{visible now:}` per row (public zones only) | **PASS (17 occurrences / 5 records)** | Forms: `{visible now: 1 in their graveyard}` 7, `{visible now: 1 on their battlefield}` 5, `{visible now: ...}` 5. No hidden-zone leak |
| D | narration `You named X with …` 9/9 | **PASS on presence, LOW defect on the source label** | 98 renders / 84 records. 78 name the source (`You named "Howling Mine" with Silverquill Silencer` 46, …), but **20 read `You named "Pyrite Spellbomb" with the spell`** — a generic fallback where the source name was available. See D-8 |
| D | `discard` rows carry `{spare:}` / `{dead right now:}` / `{you already control one}`; bare discard rows **0** | **PASS** | All three land on `discard` records at Cleanup (40 records carry at least one). `{spare: you control N lands already}` **0 / 44 / —**; `{dead right now: 0 legal targets on the board for it}` **0 / 31 / —**, and the cards carrying it are exactly the targeted-removal family — Path to Exile 13, Tragic Slip 9, Vanishing Verse 3, never a counterspell; `{you already control one: <card>}` **0 / 5 / —** (Sanguine Bond, Chromatic Lantern, Underworld Dreams, Wall of Omens, Perimeter Captain) |
| D | cleanup discards sending a counterspell past a listed spare land **0** | **PASS (0), with a real N** | All 68 `discard` records checked. **114 counterspell rows** appear in discard menus (Essence Scatter 41, Fall of the Gavel 28, Dream Fracture 22, Cancel 20, Vision Skeins 3) alongside **44 `{spare: you control N lands already}` land rows** across 15 land names — and **0** discard chose a counterspell while a spare land was on the menu. **0** of the 114 counterspell rows carries a `{dead right now:}` — the withholding rule is genuinely exercised, not merely unexercised |
| **E** | `reveal_stall` / `_secs` / `_phase` on any parked reveal | **PASS on the stamp; NO reveal parked** | The three fields are present on **12 / 12** reveal records. **`reveal_stall` is a poll-TICK counter and `reveal_stall_secs` is wall seconds** — and `_secs` tracks `latency_ms/1000` to within 1 s on 12/12 (e.g. 60 s / 59,978 ms; 211 s / 210,482 ms; 474 s / 473,965 ms). Ticks and seconds are uncorrelated (9,270 ticks ↔ 60 s; 81,423 ↔ 211 s; 87,917 ↔ 474 s), which is the structural-signature design working. `_phase` is **0** on all 12. The poll-churn floor is max(1,800 s, 3 × 900 s) = **2,700 s**; the largest `_secs` is **474**. **No reveal parked; the field is an unconditional stamp, not a park flag** |
| E | `stale_livelock` on a `reveal` record **0** | **PASS (0)** | The class does not appear anywhere. Wave-54 HIGH #4 does not recur. `force-close` renders **0** — still **UNTESTED**, and now for the right reason (no reveal stalled) |
| E | every 900 s wall miss leaves `wall_miss: 1` on the consuming record or a `kind: wall_miss` record | **PASS 3/3** | stderr `no reply after 900s - one retry` in exactly 3 files (`game-125v130:688`, `game-126v162:1508`, `game-146v162:747`). Two produced `kind: wall_miss` records (`162v146` seq 15, Main 1 turn 10, 900,023 ms; `130v125` seq 25, Attackers turn 10, 900,030 ms); the third landed on a consuming decision (`126v162` seq 24, `wall_miss: 1`). **Wave-54 D-8 CLOSED** |
| E | `gameend` carries `wall_miss_events` + `wall_miss_unrecorded` | **PASS 42/42 present; 3 / 2 firing** | `wall_miss_events` = 1 on `162v146`, `130v125`, `126v162`; `wall_miss_unrecorded` = 1 on the first two. Consistent with the record-level evidence |
| E | mulligan colour clause `on turn one` (1 land) / `even with every land in this hand in play` (2+); mulligans under it at 2+ lands **0** | **PASS (0 / 2), thin N** | 3 clause windows: `126v162` seq 1 at **1 land** → `no spell in it is castable off {G} alone on turn one` (KEPT); `146v152` seq 1 at **4 lands** and `146v130` seq 1 at **2 lands** → `… off {W} alone even with every land in this hand in play` (both KEPT). The land-count switch is correct in 3/3 |
| E | attackers prompts carry the blocker-count line | **PASS (90 / 90)** | Three forms, all counted: `They have N untapped creatures able to block: every attacker you declare this turn is unblocked unless something changes first.` 55, plus a new second clause `; declaring more than N attackers leaves at least (your attackers - N) of them unblocked.` 18 plural + 17 singular |
| E | `while ahead on LIFE` at `my_life <= opp_life` **0** | **PASS (0 / 8)** | 8 renders, all with a positive life margin |
| E | `ability: X's X` name-twice stack lines **0** | **PASS (0 / 1,103)** | 78 distinct `ability:` forms, 0 name-twice. 566 carry the `[triggered/activated ability]` suffix but now name the effect (`ability: Staff of Nin's Draw 1 [from your Staff of Nin] [triggered/activated ability]`) — the wave-54 bare-generic complaint is answered |
| **Q** carry | recovery rule: every `choice: -1` + fallback gets exactly one `recovery` at seq+1 | **PASS 7/7** | 7 records with `choice: -1` (5 decisions + 2 `wall_miss`), 7 `recovery` records, each at seq+1 with `recovers_seq` / `recovers_kind` / `recovers_fallback`. The 9 `plan_choice_conflict` records have `choice != -1` and correctly get none |
| **carry** | Baka-executed ≤ 0.20 % | **PASS (2 / 3,171 = 0.063 %)** | `chose -1 of N` twice: `game-146v126-1788439436.stderr:1030` (→ `126v146` seq 12, `stale_echo`) and `game-152v125-1788439450.stderr:3927` (→ `125v152` seq 146, `unparsed_reply`) |
| **carry** | genuine mis-executions ≤ 1 / 3,000 | **PASS (0)** | 2,716 replies carry a `CHOICE: n (name)` parenthetical; 16 disagree with the executed row's label = 5.89 / 1,000 (wave 54: 13.7 / 1,000). 15 are cosmetic short-forms (`Plains #1 - "W"` vs `Plains #1 [land] [your battlefield] - "W"`; `Create human with Thraben Doomsayer x14` vs the `repeated N times, then stop` row); the 16th (`130v146` seq 38) is an artifact of my own last-coded-line method — the engine correctly took the header line and stamped `rejected_line_skipped;latched_row_mismatch`. **0 genuine mis-executions** |
| **carry** | `latched_row_mismatch` never stamps a legitimate name-over-index remap | **PASS (1 / 1)** | The single firing is `130v146` seq 38 above: the latch refused a later prose line whose index and name both differ from the executed row. Correct |
| **carry** | `hold_row_named` behaviour | **PASS (5 / 5 resolved to the HOLD row)** | All 5 are `CHOICE: 0 (Hold priority for the rest of this turn…)` — index 0 (pass) naming the HOLD row — and all 5 executed the HOLD row (choice 3, 4, 3, 3, 8), each co-stamped `echo_index_conflict`. Name-over-index wins where it should. `123v152` 82, `123v126` 18 / 40 / 44, `130v126` 9 |
| **carry** | 21/21 natural | **PASS** | corpus-results.tsv: 0 timeouts / draws / crashes / hangs |

---

## 3. HIGH items

### HIGH #1 — the HOLD row is now offered EVERYWHERE it is needed and is still refused: 8.9 % adoption in thrice-declined windows, and those windows cost **2.73 h (11.9 %) of the corpus's inference**

**Coverage is solved.** Wave 54's two HOLD complaints (own-turn exclusion; not last on the menu)
are both discharged: 2,352 renders, last row 2,352/2,352, own-turn 1,323 of them, and in the 753
windows carrying `[you declined this exact list ≥3 times already this turn]` the row is present
**753 / 753**. There is no longer any window where the pilot *could not* stop the loop.

**Adoption did not follow.** Takes in those 753 windows: **67 = 8.9 %** (wave 54: 22 / 212 = 10.4 %;
target ≥ 20 %). Overall take rate 12.0 %. The cost is measurable: the 753 thrice-declined windows
consumed **2.73 h = 11.9 % of the corpus's 22.98 h**; the whole opponent-turn declined surface
consumed **3.75 h = 16.3 %**. Opponent-turn casting windows: **890, with 45 casts (5.1 %)**, and
**zero** casts at Upkeep (199 windows), Draw (90), Cleanup (52), Combat ends (60), Attackers (64),
Blockers (25), Combat damage (4) — **seven phases, 494 windows, no cast, 1.76 h of inference (7.7 % of the corpus).**

**Repro:** `125v152` seq 66 → 71 (turn 42, Main 1 → Main 2), 21.1–21.3 KB prompts, the declined-note
at 3+, the HOLD row last on every one, `Cast nothing right now` every time.

Because coverage is complete, the remaining lever is **not another render of the same row**. Two
falsifiable asks, in cost order:

- **(a) Make the declined-note the row's own annotation.** Today the note is a header line far above
  a 6-row menu and the row's own text says nothing about how many times this exact list was already
  declined. Append it to the row: `{you have declined this exact list 5 times this turn - taking this
  row ends the repetition}`. **Wave-56 falsifier:** takes in N ≥ 3 windows stay below 12 % with the
  count on the row → the row's wording is not the lever and the next candidate is engine-side
  auto-hold after K identical declines (which the owner's "no hard caps on legal choices" rule does
  NOT forbid: it removes no row, it takes one).
- **(b) Price the window.** Nothing in the prompt tells the pilot a decision costs anything. The
  cheapest true statement available to the emitter is the count itself, above.

### HIGH #2 — the `[RE-ASK]` conflict notice quotes the WRONG coded line when the reply has two, and it flipped a correct play into a pass

**Repro: `130v123` seq 109 → 110, turn 26, Upkeep, priority, 6 rows.** Seq 109's reply has two coded
lines: `CHOICE: 0 (pass)` first, then 400 words of reasoning ending `CHOICE: 5 (cycling with
Starstorm)`. The engine latched the second (`coded_answers: 2`, `latched_coded_line: 2`, executed
row 5) — correct — then stamped `plan_choice_conflict` and re-asked. The re-ask line rendered into
seq 110 reads:

```
[RE-ASK] Your CHOICE line takes row 5 ("CHOICE: 0 (pass)") but your reply says this window is a pass
("I will pass"). Answer again: 0 (pass) if you meant to pass, or the number of the row you want
performed now.
```

The **row number comes from the latched line (5) and the quoted string comes from coded line 1
(`CHOICE: 0 (pass)`)**, and the "your reply says this window is a pass" evidence quote is lifted from
the *discarded* first line's prose. The model was shown a self-contradicting notice and answered
`CHOICE: 0 (pass)` — **the re-ask converted a legitimate Starstorm cycle into a pass** (parse_note
on seq 110: `plan_choice_conflict_recovered`, i.e. the engine recorded this as a success).

Second firing of the same shape: `152v123` seq 78 — `takes row 2 ("CHOICE: 1 (…)")`, model then
answered row 1. **2 of the 8 `CHOICE-line` re-asks are mismatched, and both are exactly the records
where `coded_answers = 2` and `latched_coded_line = 2`.** The other 6 (`coded_answers = 1`) are
correct and 6/6 recovered the model's real row.

**Ask:** quote the LATCHED coded line, not the first one — and quote the prose evidence from the same
region of the reply. **Wave-56 falsifier:** `[RE-ASK]` notices whose quoted `CHOICE: n` index differs
from the row number the same sentence names: **0/N** (this corpus 2/8).

### HIGH #3 — the monotone X family (Sphinx's Revelation) renders no marker, no collapse and no mana-fit clause, and the pilot picked X = 3 on **7 of 7** menus where a larger X was affordable

Primitive verified: `name=Sphinx's Revelation / auto=life:X && draw:X / mana={X}{W}{U}{U}` — value is
strictly increasing in X; the only cost is mana.

| record | turn | max affordable X | rows rendered | marker | chose |
|---|---|---|---|---|---|
| `125v152` seq 161 | 52 | **12** | 13 | none | **X = 3** |
| `125v152` seq 49 | 36 | 7 | 8 | none | X = 3 |
| `125v152` seq 56 | 40 | 8 | 9 | none | X = 3 |
| `125v152` seq 95 | 44 | 8 | 9 | none | X = 3 |
| `125v126` seq 145 | 25 | 8 | 9 | none | X = 3 |
| `125v126` seq 157 | 27 | 6 | 7 | none | X = 3 |
| `125v123` seq 38 / 43 / 53, `125v162` seq 37, `125v126` seq 47 | — | 2 / 2 / 3 / 3 / 3 | 3–4 | none | max |

Where max X ≤ 3 it takes the max; where max X > 3 it takes 3, every time. The render gives it nothing
to argue with: the rows read `1. X = 12 {X pricing: X=12 - you gain 12 life and draw 12 cards}` down
to `13. X = 0`, with **no `[<- …]` marker** (the three ranked markers are all kill-list phrased and
never fire on a menu with no kill dimension) and — unlike every cast row in the corpus — **no
`{leaves N sources}` / `{taps you out}` fit clause**, so the one real trade-off (holding up mana)
is invisible too. Contrast the Starstorm menus, which do get a marker and where the pilot took the
marked row 2 of 3 times.

**Ask:** (a) add a fourth marker for the monotone/no-kill family, e.g. `[<- largest affordable X -
X=12 gains 12 and draws 12; no listed X does more]`; (b) carry the `{leaves N sources}` clause onto
X rows as it is carried onto cast rows; (c) collapse the identical middle of a monotone run the way
lane D collapses `#N` runs (13 rows for a one-dimensional choice is the D-6 waste class again).
**Wave-56 falsifier:** with a marker on the monotone family, takes of X < max on a Revelation-shaped
menu fall below 3 of N. If the pilot still answers X = 3 with the marker on the row, the anchor is in
the guide, not the render, and it is deck125's item.

---

## 4. The 14 fallbacks + 2 Baka executions, each classified

`fallback` on decisions **14 / 3,171 = 0.44 %** (wave 54: 23 / 2,212 = 1.04 %; wave 53: 0.43 %) — back
to the wave-53 floor. Two further `fallback` fields sit on the non-decision `wall_miss` records.

| class | n | root cause | Baka? | outcome |
|---|---|---|---|---|
| `plan_choice_conflict` | 9 | all 9 stamped `decision_reversed_in_prose`; header line takes a live row, the PLAN prose then reasons to a pass. `146v152` 17, `123v152` 56, `152v123` 77, `123v126` 42 / 69, `123v146` 16, `130v123` 109 / 122, `162v126` 33. **0 of the 9 executed the HOLD row** — lane A's D2b collision stays closed | no (`choice != -1`) | 4 `plan_choice_conflict_recovered`, 5 `plan_choice_conflict_exhausted`. One "recovery" is a false success — HIGH #2 |
| `named_row_reask` | 3 | off-menu name from a stale plan: `123v126` 25 (`Create human with Thraben Doomsayer`), `123v130` 9 (`Cast Thraben Doomsayer`), `162v125` 52 (`Cast Underworld Dreams`). All 3 co-stamped `stale_echo_in_range` | no | 3 `named_row_reask_recovered`; all 3 have a `recovery` record. **`named_row_reask_exhausted` 0** (wave 54: 2) |
| `stale_echo` | 1 | `126v146` seq 12, ask, turn 8. `choice: -1` | **yes** | `recovery` seq 13; `game-146v126:1030` shows Baka cast Wall of Omens |
| `unparsed_reply` | 1 | `125v152` seq 146, ask, turn 51, latency **767.8 s** — a 900 s-adjacent reply the parser could not read | **yes** | `recovery` seq 147; `game-152v125:3927` shows Baka cast Augur of Autumn |
| (non-decision) `wall_miss_unrecorded` | 2 | `162v146` seq 15, `130v125` seq 25 — the D23 arm working | no | `recovery` at seq+1 on both |

**NO `all_assignments_illegal`, NO `stale_livelock`, NO `empty_reply`, NO `timeout` class, NO
`engine_answered`.** `chose -1 of N` = 2 = **0.063 %** of decisions.

`parse_note` totals: `decision_reversed_in_prose` 14, `echo_index_conflict` 10, `plan_choice_conflict_exhausted` 5,
`name_over_index` 5, `hold_row_named` 5, `stale_echo_in_range` 4, `plan_choice_conflict_recovered` 4,
`named_row_reask_recovered` 3, `attack_last_line_taken` 3, `latched_row_mismatch` 1, `long_reply` 1,
`repeat_count_under_two` 1, `rejected_line_skipped` 1. **`pass_hold_ambiguous` 0**,
`plan_contradicts_noop_row` 0, `named_row_not_offered` 0, `multiblock_first_wins` 0,
`blocks_last_line_taken` 0. `answer_replaced` 11, `commit_retracted` **0**.

stderr: `auto-passing without a model call` 2,130, `all actions pass-declined` 734, `land-drop ask NOT
issued` 895, `cast ask NOT issued` 149, `combat decision suppressed` 446, `hold re-opened` 166,
`repeat plan iteration` 46, `http_error` **0**, `with no source tapped` **0**, `index_name_conflict`
**0**, `plan_missing` **0**, `force-close` **0**, `Segmentation`/`ASAN`/`assert` **0**.

### Mulligans (new-baseline record)

51 mulligan asks over 42 seats: **33 seats kept 7**, **9 mulliganed once and all 9 then kept 6**.
Zero double-mulligans, zero mull-to-zero, 9 `bottom` records (one per mulligan), all the model's —
**no OWNER QUESTION**. Bottomed: Tovolar's Huntmaster, Vision Skeins, Hammer of Bogardan ×2,
Teferi's Puzzle Box, Talisman of Impulse, Lightning Greaves, Damnation, Drowned Catacomb.
**deck130 mulliganed 3 times this corpus** (`130v152`, `130v126`, `130v123`) — its mulligan section
is exercised for the first time in five corpora; the standing **#132-UNTESTED** tag can be lifted by
the deck130 seat.

---

## 5. Audit-lane watch — behaviour drift with no engine-lane explanation

| lane | what the corpus shows | verdict |
|---|---|---|
| **G** ability ownership | 0 crashes, 0 ASAN signatures, 0 segfaults, 21/21 natural; no dangling-ability trace | **clean as far as logs can say** |
| **H** ability epoch gate | `{right now:}` renders 1,415×; every sampled verdict matched its own enumeration. No delayed-condition bump observable | **not observable from this corpus** |
| **I** ManaCost / garbage / cleanup | No RSS telemetry. `In GetCost Seems ManaCost was not properly initialized` **532** in stderr — present in wave 54 too, unchanged in character | **no regression visible; RSS untested** |
| **J** render caches / resource-miss memo | `AttemptNew failed to load (404)` **1,725** over **37 distinct** resources — down from 1,927 / 118. Every remaining miss is an **audio** file (`mana.wav` 105, `land.wav` 105, `creature.wav` 91, `basic.wav` 88, …) plus `thumbnail` 205. The image-cycling loop wave 54 flagged is **gone**; `Destroying WCachedResource` now names stable backdrops | **improved; the residual is the silent-audio stub, not the memo. LOW** |
| **K** Baka `selectAbility` | Baka executed 2 decisions, both ordinary casts. No planner anomaly | **effectively UNTESTED (n = 2)** |
| **L** narration / board index / http classes | `http_error` **0**. Prompt mean UP 13.4 % and total volume up 62 % — attributable to the new lanes (HOLD row on own turns alone is 0.52 M chars = 1.3 % of 38.6 M) and to the +43 % decision count, not to a trim regression: prompts > 30 K fell 2 → **1**, > 25 K rose 37 → 252 purely because there are 1,000 more decisions and three 60-turn seats | **PASS on the falsifiable half** |
| **M** ask cache / prefill / combat window cache | Still no cache-hit counter in any log — the central watch item remains **unmeasurable**. New gameend fields *do* land: `mana_only_windows_skipped` **544**, `identical_option_asks_resolved` **9**, `hold_windows_skipped` **735**. Ask for `cache_hits` on the gameend the same way | **instrument gap, third corpus running** |
| **N** Vita textures | No desktop exposure | **UNTESTED here** |

**Behaviour with no engine-lane explanation: none.** Every movement this corpus traces to a named
wave-55 lane or to server contention (§1). The three things that *look* like drift and are not:
per-decision latency (contention, measured), prompt volume (lane A/C/D additions + decision count),
and `plan_echo_count` max 37 → **143** (D-4 below — a consequence of the restored window economy).

**⚠ `WAGIC_GPT_BOARDINDEX=0` SEGFAULTS** (review-carry §C; also on the wave-54 base binary). Lane L's
disable flag is unusable, so no A/B of the board index is available to any reviewer until it is
fixed. That is a silent-instrument-rule breach in its own right and belongs on the wave-56 docket
above any new instrumentation: **a disable flag that crashes is worse than no flag, because the
lane reads as A/B-able.**

---

## 6. NEW / CARRIED DEFECTS, ranked

**D-1 HIGH — see HIGH #1** (HOLD adoption 8.9 % on complete coverage; 2.73 h of thrice-declined inference).
**D-2 HIGH — see HIGH #2** (`[RE-ASK]` quotes the wrong coded line; 2/8; cost one play).
**D-3 HIGH — see HIGH #3** (monotone X menus: no marker, no fit clause, no collapse; 7/7 sub-maximal).

**D-4 MED — `target=anytarget` damage rows enumerate both players and price only one. 29 rows.**
Emitter string, `130v152` seq 8 row 1: `Cast Spark Spray {r} {leaves 1 of your 2 untapped mana sources
untapped} {no creature target - and 1 to the opponent at life 20 leaves them at 19} - legal targets
right now: the opponent, you {this row does not pick a target yet - taking it asks you next which …}`.
Primitive verified: `name=Spark Spray / target=anytarget / auto=damage:1`. **21 Spark Spray rows and
8 Hammer of Bogardan rows** enumerate `the opponent, you` with a verdict that speaks only of the
opponent — the exact shape lane C fixed for `target=player` (Devour Flesh 151/151 two-sided). Self-
targeting is never right here, so the cost is confusion rather than a lost line, but it is the same
class and the same fix. **Wave-56 falsifier:** both-player rows of any target class with a one-sided
`{right now:}` verdict **0/N** (this corpus 29/180).

**D-5 MED — plan staleness has quadrupled: `plan_echo_count` max 37 → 143, p90 = 80.**
1,510 records carry the counter; **328 records serve a plan that is more than 40 windows old**, and
the record holder (`125v152`) serves one for 143 consecutive windows. The mitigations fire (`"<card>"
is no longer on your menu` renders 622×; `stale_echo_in_range` 4, `named_row_reask` 3 — all three
re-asks recovered), so this is not yet a correctness defect. It is the mechanism behind every
`named_row_reask` in the corpus and it will scale with the window economy. **Ask:** force a PLAN
refresh at a hard age (say 40 windows) rather than only on menu invalidation. **Falsifier:**
`plan_echo_count` max above 60 in wave 56 with a refresh armed.

**D-6 MED — the `(xN)` event-log collapse still misses phase lines and draws: 514 duplicate lines
inside single records** (wave 54: 471 — worse, not better). `- Phase: Draw` **230**, `- Opponent drew
a card` **101**, `- Phase: Main phase N` **80**, `- Phase: Cleanup` 5, loyalty/level/token lines 3–6
each. Visible in a shipped artifact: the `162v146` seq 15 `wall_miss` record's `events` block reads
`- You drew Master of the Feast` **twice**. With the GAME LOG at **58.1 %** of the mean prompt this is
the cheapest byte reduction on the board.

**D-7 LOW — the `{kills N of the …}` conditional form still omits the THEIRS/YOURS split.**
31 rows: `{kills 1 of the 1 CREATURE target at 3 damage - and 3 to the opponent at life 12 leaves them
at 9}` (19) and `{kills 1 of the 1 CREATURE target at -1/-1}` (12). Ground-truth-checked: none names
a creature the seat controls, so nothing is *false*; but the split that the 223 unconditional rows
carry is absent here, and the "1 of the 1" phrasing is opaque.

**D-8 LOW — `You named "<card>" with the spell`: 20 of 98 named-narration lines fall back to a
generic source label** where the source name is available (the other 78 read `… with Silverquill
Silencer`). Same generic-fallback family as wave-54 D-6, now in the narration rather than the stack line.

**D-9 LOW — 1,725 resource 404s, 37 distinct, all audio (`mana.wav` 105, `land.wav` 105, …) plus 205
`thumbnail`.** Down from 1,927 / 118 and no longer cycling images. This is the silent `JSfx` stub, not
lane J. Set-C watch, no action asked.

**Closed this wave:** wave-54 D-1 (stall-drop storm — lane R), D-2 (Devour Flesh one-sided verdict —
151/151 two-sided), D-3 (the `#N` mega-menu — 0 records with > 3 rows of one shape), D-4
(`all_assignments_illegal` — never fired; re-ask arm shipped but UNTESTED), D-5 (HOLD own-turn
exclusion — 1,323 own-turn renders, longest own-turn run 12), D-7 (`- Paid` with no payee — the 220
remaining are the legitimate `- Paid {2} for Elixir of Immortality from mana already floating` form,
not the wave-54 no-source class), D-8 (900 s wall miss with no trace — 3/3 recorded).

---

## 7. Prompt economics for the owner's invariant-0 review

| kind | n | mean | p50 | p90 | max | lat p50 | lat p90 | lat max |
|---|---|---|---|---|---|---|---|---|
| ask | 2,416 | 12,021 | 10,331 | 24,019 | 30,031 | 12.4 s | 49.2 s | 1,391.2 s |
| priority | 555 | 13,088 | 11,829 | 22,622 | 29,808 | 16.1 s | 63.6 s | 523.1 s |
| attackers | 90 | 11,545 | 9,818 | 21,089 | 27,352 | 24.2 s | 65.6 s | 285.9 s |
| discard | 68 | 12,319 | 10,878 | 20,303 | 28,689 | 16.6 s | 55.0 s | 263.8 s |
| blockers | 21 | 10,562 | 8,972 | 17,798 | 21,973 | 24.6 s | 99.2 s | 224.4 s |
| reveal | 12 | 14,790 | 10,665 | 20,029 | 27,813 | 42.4 s | 148.1 s | 474.0 s |
| bottom | 9 | 2,197 | 2,219 | 2,263 | 2,302 | 78.3 s | 91.4 s | 91.6 s |

Total prompt volume **38.60 M** chars over 3,171 decisions (wave 54: 23.76 M / 2,212). Mean prompt
10,740 → **12,174 (+13.4 %)**; per-decision that is +1,434 chars, of which the HOLD row accounts for
~165 (0.52 M / 3,171). **Prompts > 30 K: 1** (wave 54: 2). **GAME LOG share of the prompt: 58.1 %**
mean over the 3,111 prompts carrying a `CURRENT SITUATION` marker (wave 54: 56.1 %) — and it reaches
**82.4 %** on the late-game specimens.

**Where the 22.98 h went:** opponent-turn windows **4.56 h (19.9 %)**, of which **3.75 h (16.3 %)
ended in a decline**; windows already carrying `[you declined this exact list ≥3 times]` **2.73 h
(11.9 %)**; the three 900 s wall misses **0.75 h (3.3 %)**; the 43 dropped calls **≈0.31 h**. The
single largest inference lever available this wave is the 11.9 % spent on thrice-declined windows
(HIGH #1), which is worth more than any prompt-byte edit: cutting the mean prompt by 10 % would save
nothing at all, because latency is negatively correlated with prompt size inside this corpus.

### Specimen recommendation

The harvest default `wave55/lategame-specimen.txt` = **`152v125` seq 309** (turn 65, 29,337 chars,
79.2 % log) scores **0** on a 24-term render-vocabulary probe: it is a bare mana-source/board menu
whose rows are `Deserted Beach #1 [land] [your battlefield] - "…"`. Like wave 54's default it is an
excellent exhibit of *log cost* and a poor exhibit of everything the owner would be reviewing.

**Ship two, primary first:**

1. **PRIMARY — `1788439453-ai_baka_deck125-…-vs-ai_baka_deck152.jsonl` seq 69** (`125v152`), turn 42,
   Main phase 1, **21,279 chars, 6 rows, 5.3 s, 22 life vs 33, log share 75.8 %.** It carries
   `{X pricing:` (a Sphinx's Revelation ladder — HIGH #3 in situ), three `{leaves N sources …}` fit
   forms plus `{taps you out …}`, `{right now: …}`, the `[you declined this exact list N times]`
   note, and the HOLD row last on the menu — and the answer is **`Cast nothing right now`**. That is
   the invariant-0 question in one file: *21 KB and every render feature we ship bought a decline at
   turn 42.* Its neighbour seq 71 is the same board one phase later, which makes the repetition cost
   legible on the same page.
2. **SECONDARY — `125v152` seq 190**, turn 60, Main phase 2, **27,249 chars, 4 rows, 3.6 s, log share
   82.4 %**, answer `Cast nothing right now`. The pure log-cost exhibit, relabelled as such, and a
   better one than the harvest default because its four rows are real cast rows rather than lands.

If only one is wanted, ship #1.

---

## 8. Falsifiable predictions for wave 56

1. **Stale drops.** With the async slot keyed on `(seam, turn, phase, boardKey)`, the E1+E2
   seam-alternation class falls below 6 and `dropping stale async answer` below 20 corpus-wide.
   *Falsifier:* drops stay ≥ 35 with the key changed → the rebuild is in the prompt builder, not the
   slot, and `WAGIC_GPT_AUDIT_M_OFF=1` is the next flag (`WAGIC_GPT_BOARDINDEX=0` is unusable).
2. **HOLD adoption (HIGH #1).** With the declined count moved onto the row itself, takes in
   declined-note N ≥ 3 windows rise above 20 % and the inference spent on those windows falls below
   8 % of the corpus total (this corpus 8.9 % / 11.9 %). *Falsifier:* takes stay below 12 % → the
   wording is not the lever; escalate to engine-side auto-hold after K identical declines.
3. **`[RE-ASK]` quote (HIGH #2).** Notices whose quoted `CHOICE: n` index differs from the row number
   the same sentence names: **0/N** (this corpus 2/8), and `plan_choice_conflict_recovered` records
   whose recovered row is `pass` while the latched row was live: **0/N** (this corpus 1).
4. **Monotone X (HIGH #3).** With a fourth marker and a `{leaves N sources}` clause on X rows, takes
   of X < max affordable on a gain-X/draw-X menu fall below 3 of N (this corpus 7 of 7). *Falsifier:*
   the pilot still answers X = 3 with the marker on the row → the anchor is in deck125's guide.
5. **Both-player rows (D-4).** Rows enumerating `the opponent, you` with a one-sided `{right now:}`
   verdict: **0/N** for every target class, not only `target=player` (this corpus 29/180).
6. **Plan staleness (D-5).** With a hard age refresh, `plan_echo_count` max below 60 (this corpus
   143) and records over 40 windows old below 60 (this corpus 328).
7. **Event-log dedup (D-6).** Duplicate event lines inside single records below 150 (this corpus 514);
   `- Phase: Draw` duplicates 0.
8. **Menace / all-illegal (lane B).** Both remain **UNTESTED** unless a menace attacker arises; if one
   does, the header renders on 100 % of those windows and 0 % otherwise, and any
   `all_assignments_illegal` firing is followed by exactly one `[RE-ASK]` carrying the pruned pairs.
9. **Reveal.** `reveal_stall_secs` above the 2,700 s poll-churn floor on any record: **0**;
   `stale_livelock` on a reveal: **0**; `force-close`: expected to stay **UNTESTED**.
10. **Per-decision cost.** Held at 21 concurrent games, per-decision inference tracks queue depth, not
    prompt bytes: if the decision count stays within ±10 % of 3,171, per-decision latency stays within
    ±10 % of 26.1 s regardless of any prompt-size edit. *Falsifier:* a prompt-byte reduction of ≥ 15 %
    moves per-decision latency by more than 10 % at constant decision count → prefill dominates after
    all and the trim work is worth reopening.

---

## What I did NOT check

- **I did not build, run, or A/B anything.** Every flag recommendation is a proposal. In particular I
  could not confirm the server-contention explanation for the latency rise by re-running at lower
  concurrency; the evidence is the within-bin comparison plus the concurrency arithmetic, which is
  strong but is inference.
- **`WAGIC_GPT_BOARDINDEX=0` segfaults**, so lane L has no working A/B for anyone this wave.
- **Lanes G / H / I / K / N are largely unobservable from this corpus** — no RSS, no ASAN, no per-tick
  verdict trace, 2 Baka samples, no Vita exposure. I reported "not observable" rather than "clean".
- **Lane M's cache-hit counters still reach no log**, a third corpus running.
- **I did not diff prompts across polls** (only the final prompt is recorded), so the E1/E2 drop class
  is characterised from stderr ordering, not from two prompt strings side by side.
- **I did not read any deck guide or measure guide sizes** — this seat owns no deck; the 41–71 KB band
  belongs to the deck seats and synthesis. The X = 3 anchor in HIGH #3 may well be a deck125 guide
  line; I state the render gap and leave the guide half to that seat.
- **Card facts verified:** Sphinx's Revelation, Starstorm, Devour Flesh, Spark Spray (primitives only,
  no Scryfall cross-check needed).
- **I could not test the menace header, `all_assignments_illegal`, the NAMED-BY price, the reveal
  force-close, or lane A's life-loss-loop prediction** — no window arose for any of them. Those are
  UNTESTED, not PASS.
