# Wave-57 engine ledger — THE WAVE-58 DOCKET (engine / UX only)

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-174505`, binary **67b2bb38a**, model
`qwen36-35b-a3b` on Spark :8084, `-j 21`, `-T 0`, `WAGIC_GPT_TIMEOUT=900`, `WAGIC_GPT_LOGWINDOW`
unset. 42 seat JSONL + 21 `game-*.stderr`. **21/21 natural — invariant 00 satisfied. 0
`Segmentation` in 21 stderr.**

Every number below was re-derived on disk by this step: python over the JSONL **counting rows from
the rendered `prompt`, never from `options_text`**, and `/usr/bin/grep` over the stderr with the
emitter's own quoted line. Where a seat, a lane and the harvest disagree, all predicates are named and
the disk is quoted (#295, #298, #327). Guide-side items are NOT here — they are the reviewers'
(owner doctrine 2026-08-26).

**Lane V (Baka blocker policy) was merged AFTER the corpus binary and is NOT in this corpus.** Its
predictions are carried as wave-58 items **D22-D24** with its own A/B numbers, marked so nothing here
is read as an adjudication of it. **Lane S (the vpk16 softlock) was NOT reproduced** in ten
variations; its two named watchdog-blind states are carried as **D36**.

**Owner items: none new.** D42 (Arena grouping) is **BUILT** — lane G, human seat, measured
−37…−50% `GuiPlay::Render` at 39-55 cards; console/touch/divided-damage verification is D38. D43 (the
log window) is **MEASURING** — lane H built the mechanism default-OFF and the A/B is a wave-58 step,
but **D7 blocks it**: the control arm's own instrument is incomplete. **The owner's Vita framing,
verbatim: "on the vita, ive been playing only against the heuristic ai"** — his play reports are
**Baka-seat** games and route to `AIPlayerBaka` / the GUI / card script, **never to a guide and never
to the GPT prompt surface**. Lane V is the first item to arrive through that channel and it is
correctly a Baka-policy change.

---

## FALLBACK CLASSES — 8 of 3,317 = **0.24%** (wave 56: 6 of 2,762 = 0.22%)

| class | n | note |
|---|---|---|
| `named_row_reask` | 6 | the model named a row not on the list; the re-ask is the recovery. `<refused: named_row_reask>` renders in `chosen_text` — **first renders ever**, and both instances the deck123/130 seat traced are correct, with no wrong cast following |
| `retracted_choice` | 1 | `126v146` s34 — two `CHOICE:` lines on a naturally-terminated reply. **Terminal**: the seat's last live window, dead at 2 life turn 20 → **D11** |
| `unparsed_reply` | 1 | — |
| **`abandoned_in_flight`** | **0** | **lane U PASS** — the endpoint answered; the bound was never reached. `WAGIC_STALLPROBE` 0 lines (dev-gated), `giving this decision to the heuristic` **0**, **Baka executed 0 decisions** |
| `empty_reply` / `timeout` / `plan_choice_conflict` / `all_assignments_illegal` / `stale_livelock` / `wall_miss` | 0 | — |

`recovery` records **7**, pairing **7/7** against the 7 `choice: -1` records, **0 unpaired**.
`identical_option_asks_resolved` 5. `dropped_assignments: 0` on **35 of 35** `blockers` records.

## PER-KIND PROMPT CHARS

| kind | n | mean | max | total |
|---|---|---|---|---|
| `ask` | 2,642 | 12,022 | 30,228 | **31.76 MB** |
| `priority` | 481 | 12,895 | 30,090 | 6.20 MB |
| `discard` | 66 | **15,872** | 30,127 | 1.05 MB |
| `attackers` | 76 | 11,360 | 24,732 | 0.86 MB |
| `blockers` | 35 | 11,174 | 23,717 | 0.39 MB |
| `reveal` | 13 | 14,208 | 22,824 | 0.18 MB |
| `bottom` | 4 | **2,354** | 2,864 | 0.01 MB |
| **all** | **3,317** | **12,198** | **30,228** | **40.46 MB** |

Prompts over 30,000: **4**. Over 25,000: 108. **GAME LOG above `--- CURRENT SITUATION ---` = 64.0%
of the mean prompt** (wave 56: 54.0%); **everything above the first option row = 87.8%** (was 82.7%).
Per-seat the spread is real and is NOT one number: deck123/130 **56.9%**, deck125/126 **67.6%**,
deck162 **48.3%** — three boundaries, three true figures (#208), and the ledger prints the boundary
with the number. Inference **25.38 h / 3,317 = 27.55 s per decision** (wave 56: 27.97 at the same
`-j 21`, on a mean prompt that ROSE 9.1% — R257's third confirmation).

---

# THE DOCKET, RANKED

## D1 — HIGH — the pay-3-life MDFC arrival row is the only menu in the game that can kill the pilot outright, and it is the only menu with no verdict. A seat killed itself on it at 1 life

**Record.** `1788475506-ai_baka_deck146-…-vs-ai_baka_deck130.jsonl` **seq 20**, turn 19, Main phase 2.
Board frame: `Your life: 1 | Opponent life: 20`, `Mana available: 1 total`. Two rows, verbatim:

```
1. pay 3 life - Emeria, Shattered Skyclave enters UNTAPPED [usable (tap for mana / attack) this turn]
2. tap - Emeria, Shattered Skyclave enters TAPPED [decline the payment; unusable until your next untap step]
```

Row 1 taken. **The next record in the file is `gameend`: `my_life: -2`, `won: false`, turn 19,
`opp_life: 20`.** The opponent did nothing. The reply's plan was to cast Kaya the Inexorable
(`{3}{b}{w}`) off the newly-untapped land, against a printed `Mana available: 1 total`.

**Census.** `pay 3 life` renders **116 occurrences in 104 windows, 9 takes**. Windows where the seat
was at `my_life ≤ 3` when the row rendered: **10**, nine of them in this one game. The arrival clause
itself is correct and complete (lane D's D26: **0 of 7 non-Pathway back-face land rows missing it**,
23 Pathways correctly silent, `enters TAPPED` 34 renders / 6 takes, false untapped-arrival plans
**0 of 4**) — the defect is that the clause prices USABILITY and never prices LIFE.

**Why HIGH and not MED.** The engine renders a lethality verdict on every other menu it prints:
`they would be at K` (196 occurrences / 168 windows, **3 of them `THIS WINS THE GAME`, taken 3 of 3,
won 3 of 3**), `this KILLS you` on blockers headers (27), `best case with every blocker assigned` (34).
**This is the one row in the game that ends it, and it is the one row with no number.** The magnitude
is not hidden — it is `life:-3` in the card's own script (`borderline.txt:1199`), which is where the
row's own text is built from.

**Ask.** Price the payment on its own row, unconditionally: `{this payment puts you at N}`, and when
`N ≤ 0`, `{you are at L life: paying 3 puts you at N and you LOSE the game}`.

**Prediction (falsifiable).** `pay 3 life` takes at `my_life ≤ 3` with the payment lethal: **0 of N**;
and the verdict renders on **N of N** rows whose payment is computable. **A hit is a lane break only
if** the take was lethal and the row carried the verdict — a take at 4 life that survives is not a
break. The deck146 seat's guide floor (`146-A`) is a #285/#287 route-around and **this render deletes
it** — the guide carries the `REMOVE THIS RULE WHEN` line already.

## D2 — HIGH — `NO LIVE CAST ROW ON THIS MENU` is FALSE on 8 of its 497 renders, and the pilot obeyed it three times while a 6/5 flier killed it

**Record.** `1788475549-ai_baka_deck123-…-vs-ai_baka_deck130.jsonl` **seq 45, 49, 50** (turns 22, 24,
24). Header, verbatim: `NO LIVE CAST ROW ON THIS MENU: all 2 cast rows below carry a verdict computed
from the board that reads zero - not one of them changes a number on the board as it stands.` Row 1,
three lines below it: `Cast Devour Flesh {1}{b} {right now: they control 1 creature - Rorix Bladewing
(6/5) [flying, haste, doesn't untap during its controller's untap step] is sacrificed, …}`.

**Census, this step's predicate** (header present AND a cast row whose own `{right now:}` brace names a
live effect): **8 of 497 header windows** — deck123 ×7 (`123v130` s7/s45/s49/s50/s53/s54,
`123v146` s16), deck126 ×1 (`126v125` s438, Tribute to Hunger with **Emrakul, the Aeons Torn (15/15)**
on their board). **1 cast was taken from a false-header window.**
**The deck123/130 seat reads 6 of its own 92** on a wider predicate that also counts the Tragic Slip
class (`{right now: -1/-1 (no creature has died this turn, so Morbid does NOT apply)}`). **Both are
true of different units** and the ledger prints both.

**Mechanism (hypothesis, stated as one — not traced).** The header is computed from a menu-level
predicate that does not consume the same per-row verdicts it claims to summarise; rows whose brace
contains a negative substring (`0 without a restriction`, `does NOT apply`, `YOU control 0`) alongside
a real kill or sacrifice clause read as zero to the header and as live on the row.

**Ask.** Derive the header from the SAME per-row verdicts the rows print — the header is a fold over
the rows, not a second evaluation. Where a row has no computable verdict, the header must not claim
it reads zero.

**Prediction.** Header windows carrying a cast row whose `{right now:}` names a live effect: **0 of
N** (wave 57: 8 of 497). **A hit is a lane break only if** the row's brace and the header disagree
about the same row — a header that is TRUE on a menu the pilot then casts off is a guide item, not
this. Skill #301 covers the guide half and stands until this ships.

## D3 — HIGH — `rowSaysNoOp` is a VERB WHITELIST and `exiles 0` is not in it, so a provably-zero six-mana sorcery badges as live on 9 rows while the identical-shape `destroys 0` badges dead on 443

**Record.** `1788475532-ai_baka_deck125-…-vs-ai_baka_deck130.jsonl` **seq 14**, and 7 more windows.
The row: `Final Judgment {4}{w}{w} (sorcery) {card text: Exile all creatures.} {right now: exiles 0 of
their creatures (0 without a restriction against attacking), 0 of yours}`. **Not one of the 9 rows in
8 windows is badged `{dead right now:}`, and `NO LIVE CAST ROW` fires on none of them** (`hdr = False`
on all 8) — while `Cast Supreme Verdict … {right now: destroys 0 …}` and `Cast Damnation … {right
now: destroys 0 …}` fire the header on **443 windows**.

**Sites.** `125v123` s187/s188/s189, `125v130` s14/s16/s98/s99/s100. Two are casting windows
(s98/s99, correctly declined), three are discard windows (correctly discarded), and the seat answered
**7 of the 9 correctly anyway** — the honest half.
The whitelist as the deck125/126 seat read it in source (`AIPlayerGPT.cpp:19862`, quoted as its read):
`{"does nothing","deals 0","destroys 0","kills 0","removes 0","drains 0","does not apply","gains 0","draws 0"}`.

**The same defect at one level up.** The engine seat found the general form independently: **6 of 455
discard rows carry a verdict the CAST evaluator computes and the DISCARD evaluator does not** —
Lightning Greaves ×2 (`123v125` s77/s81), Silverquill Command and Barrowin (`146v162` s17/s24).

**Ask.** Delete the whitelist. The evaluated magnitude that BUILDS the sentence is already a number;
badge on the number, not on a string match over the sentence. And route the discard menu through the
cast menu's evaluator.

**Prediction.** Rows whose `{right now:}` reads a computed zero and carry no dead badge: **0 of N**
(wave 57: 9 of 9 for `exiles 0`; 6 of 455 for the discard gap). A regression test ships one positive
per verb the whitelist held and one for a verb it did not.

**Bare-discard status, four predicates, none of them wrong** (#298): this step, tag family enumerated
over 455 non-decline rows — **292 bare (64.2%)**; the engine seat's "computable" predicate — **334 of
455 (73.4%)**; seat-125-126 at its own seats — **250 of 374 (66.8%)**; the harvest's uncalibrated
`{`-absent predicate — **569 of 569**. Wave 56 was 83.8% and at deck123/deck130 the class is now
**closed at 36 of 36 annotated**. **The volume item is discharged; D3 is what remains.**

## D4 — HIGH — the second async slot WORKS and the drops went UP: 108 drops, 0 cross-arm, 45 intra-arm on the land side and 63 on the casting side. `asyncSlotDriftKind` renders zero times

**Record.** 21 stderr. `AIPlayerGPT: dropping stale async answer (casting arm; the question (or
turn/phase) moved)` ×51, `… (land-drop arm; the question (or turn/phase) moved)` ×45, `… (casting
arm; the question and board moved)` ×12 = **108** against a target of **< 20** (wave 56: 65).

**And the mechanism the lane built passed.** Cross-tabbing every drop against the next
`AIPlayerGPT:` ask in the same stderr:

| drop arm | next ask `Land drop:` | next ask `Casting decision` | other seam | none in window |
|---|---|---|---|---|
| land-drop (45) | **45** | 0 | 0 | 0 |
| casting (63) | **0** | 48 | 13 | 2 |

**Cross-arm displacement: 0 of 108.** Wave 56's 44 land-drop displacements of a casting answer are
gone; every land-arm drop is now a land-arm answer displaced by a land-arm re-ask. **The second slot
does exactly what lane A built it for, and the headline number is worse anyway** — the residual is
intra-arm drift, and at **0.82 h** it is the corpus's largest single inference sink after the
identical-declined runs. The D18 rescue fires **11 times** (wave 56: 8) and is not where the volume is,
for the second consecutive wave.

**Sub-shapes named by the seats and reproduced here.** (a) hold-reopen churn, 17 drops, repro
`game-126v130-1788475528.stderr:1617`; (b) turn-flip land-drop, 45 drops, repro
`game-125v123-1788475508.stderr:409`. The casting-arm growth is concentrated: `game-126v130` alone
carries **28 of its 29 drops directly under the Sanguine Bond / Exquisite Blood trigger loop**
(see D8's specimen).

**Observability, and it is half the item.** **`asyncSlotDriftKind` renders 0 times in 21 stderr and
0 times in 3,317 prompts.** The wave-56 carry's expectation of that literal was simply wrong; the real
field is the arm-labelled parenthetical. The drop line names its ARM and never its OUTCOME.

**Ask.** (a) Narrow the slot key's board half so a Baka land auto-tap (`AIPlayerBaka: Mana cost is
NULL.` × 310) does not invalidate an answer to a menu it cannot touch. (b) Emit the drop's decision
once per drop — `fired` / `seam-changed` / `no matching slot` — as a literal that exists.

**Prediction.** Drops **< 40** corpus-wide; cross-arm **0** (already 0, kept as the control); every
drop line carries an outcome token; and `asyncSlotDriftKind` — or whatever it is actually named —
**renders on 100% of drops.**

## D5 — MED-HIGH — `[<- best trade:]` rendered exactly once in the corpus, was taken, and marked a 1-for-5. And its sibling moved seam without any guide knowing

**Record.** `1788475529-ai_baka_deck130-…-vs-ai_baka_deck152.jsonl` **seq 33**, turn 14, Main phase 1.
The marked row, verbatim: `X = 4 {X pricing: kills THEIRS: Sigarda, Champion of Light; YOURS: Dwarven
Blastminer, Siege-Gang Commander, Goblin x3} {leaves 0 of your 6 floating mana unspent - this X spends
your whole pool} [<- best trade: …]`. Rows 2-4 kill nothing of theirs and nothing of yours. The pilot
took row 1. **deck130 lost that game −20.**

**Census.** `[<- ` renders **518 windows / 21 takes**. `[<- best trade:` **1 render / 1 take**.
`[<- best X for this cast:` **408 windows / 11 takes** — and that is a NEW SEAM: lane E promoted the
marker off the ANNOUNCE_X menu onto the CAST row, which makes stale every guide sentence locating it
"one screen later" (two guides carried one). Coverage itself is clean: X cast rows with an affordable
ladder and no marker **0**; 526 rows, 506 marked, the 20 unmarked all `X = 0`.

**The tension is #248, not a coverage gap.** The marker is the most reliably followed annotation this
render produces (marked-row takes 6 of 9 and 7 of 9 in prior corpora, 4 of 7 and 3 of 3 at seats this
wave). High compliance on a marker whose DEFINITION does not match the deck's goal is more expensive
than low compliance.

**Ask.** Print the two counts inline (`THEIRS 1 / YOURS 5`), and/or suppress the `best trade` form when
`YOURS > THEIRS` — the free-trade form already exists and is correct.

**Prediction.** `[<- best trade:` renders where `YOURS > THEIRS`: **0 of N** (wave 57: 1 of 1). **A hit
is a lane break only if** the row was also taken; a suppressed-but-rendered marker on a good trade is
not. Skill #318/#319 carry the guide half.

## D6 — MED-HIGH — the log-window instrument cannot measure its own experiment, and the owner's D43 ruling depends on it

**Owner's ruling, verbatim: "measure."** Lane H built the mechanism default-OFF
(`WAGIC_GPT_LOGWINDOW`: unset/`full` / `kind[:K]` / `N`), proved the `full` arm byte-identical on 352
assembled prompts, and designed the A/B (three arms, same pool, `-r 1` = 21 games/arm, **`-j 3`, never
`-j 6`** — at `-j 6` median latency hits the timeout and 26% of decisions become `empty_reply`, which
would be read as a window effect). **This corpus is the control arm and it verifies: `log_window` =
`full` on 3,317 of 3,317.**

**Two defects block the harvest, both found by the engine seat and both reproduced here.**
**(a) The elided-turn field does not exist in the record set at all.** 46 distinct keys enumerated
across 3,408 records; none of them carries it. The control-arm prediction "`elided` is 0 on 100% of
records" is therefore **UNTESTED — the field is absent, not zero** — and arm B has nothing to verify
itself against.
**(b) `log_window_kind` labels only 28.3% of decisions.** `land_drop` 377, `empty_stack_pass` 368,
`combat` 111, `cleanup_discard` 66, `target_or_reveal` 13, `pregame` 4 — and **2,378 (71.7%)
`unclassified`**. The `kind` arm's budget is defined ON those labels, so arm B would window ~28% of
the corpus and silently leave the rest at `full`. Lane H's own residual note predicted exactly this
(the classifiers are PARSETEST-proven, never corpus-observed) and named the control-arm census as the
harvest's first job. It is now measured: **the classifier is wrong before the A/B starts.**

**Ask.** Emit the elided-turn count as a real field, and classify the `ask` majority (`land_drop`
11.4% is close to wave 56's 10.1%, so the classifier is right about what it labels and wrong about
what it skips).

**Prediction.** Field present on **100%** of records; `log_window_kind` labels **> 90%** of decisions
(wave 57: 28.3%). **Both must pass before the A/B is run** — a three-arm 63-game corpus on a broken
arm-verification census is 63 games spent on nothing. **§D of the carry stands as written otherwise**;
the ranked metric order (composition → per-decision inference → fallback rate → seat-reviewed misplays
→ win-rate BAND only, SE ≈ 10.9 points) is correct, and the rare-event rule applies: no single
favourable run is a result.

## D7 — MED-HIGH — `assignableRemainderDamage` drops trample from its own baseline and printed a SURVIVABLE number on a lethal board. A trusted bound was optimistic in the lethal direction

**Record.** `1788475557-ai_baka_deck126-…-vs-ai_baka_deck152.jsonl` **seq 14**, turn 11, seat at
**5 life**. Printed: `one legal assignment gets you to 1`. True floor computed by the deck125/126 seat:
**−5** (**−1** counting both triggers as certain). Root cause named as its read, not as a trace:
`AIPlayerGPT.cpp:14991` / `assignableRemainderDamage` weights tramplers and menace at 0 in the
max-weight matching and then omits their damage from the baseline it subtracts from.

**Census.** `best case with every blocker assigned` **34 renders**; `one legal assignment gets you to`
**6 occurrences in 5 windows**. Lethal-header blockers windows requiring the seat to subtract for
itself: **0 of 20** (deck146/152/162), **1 of 6** (engine seat — the documented 32-a-side cap),
**1 of 8 FALSE** (deck125/126). One false line in 34, and it is the shape that matters: **the model
self-caught the error and blocked correctly anyway**, which is the only reason this is not a loss.

**Why it ranks above its frequency.** The core prompt instructs the model to believe what it sees, and
this line is a BOUND — the single class of number where the direction of error decides whether trust
is safe. deck125's #G2 states the general precondition and it is adopted into the docket as a rule:
**any printed bound must be conservative in the non-lethal direction, or it must not be printed.**

**Prediction.** Printed `best case` / `one legal assignment` values that are optimistic against the
true floor: **0 of N** (wave 57: 1 of 34). Ships with a fixture per damage keyword (trample, menace,
first strike, deathtouch) and a negative for the 32-a-side cap.

## D8 — MED — the identical-declined economy went the wrong way: 19.2% of all decisions, 96 runs, max 13, 8.9% of the wave's inference. And one turn holds 73 of them

**Census, wave-53 predicate** (key `(turn, options_text)`, runs ≥ 3, a HOLD take breaks the run):
**636 of 3,317 = 19.2%**, **96 runs**, **max run 13**, **2.26 h = 8.9% of inference**. Wave 56 was
14.99% / 68 runs / max 13 / 5.5%. The engine seat reads 17.6% / 88 runs / 8.0% on its own predicate and
the harvest 17.1% / max 12 — **three numbers, one sign, and every predicate fails the wave-56 target**
(< 15%, max run < 10).

**The specimen is one phase.** `130v126` **turn 16, seq 62-134: 73 decisions in one Main phase**
(38 `priority` + 35 `ask`), **24.0 minutes of inference**, **45 of 73 pure declines**, and the declined
note reaching **N = 43** — the corpus maximum, against a wave-56 maximum of 19. The board is an
unstoppable Sanguine Bond / Exquisite Blood trigger loop; `game-126v130` also carries **28 of its 29
async drops** inside it (D4). **2.2% of the wave's decisions are one phase of one game.**

**And the re-ask predicate must be pinned before this can be adjudicated again (#327).** Byte-identical
same-turn re-asks after a HOLD take read **0 of 450** (strict adjacency — the NEXT record), **8 of 450**
(same turn AND same phase), **29 of 450** (any later same-turn record with identical `options_text`),
and **14** in the carry, which this step could not reproduce under four predicates. Three readers filed
PASS, FAIL and "the cleanest result in loop history" about one behaviour.

**Ask.** (a) Pin the predicate in the prediction sentence, executable: *the NEXT record at the same
seat, same turn, with a byte-identical `options_text`*. (b) The residual is the **unstoppable-loop
phase**, not the HOLD latch: a phase in which the same menu re-arms because a trigger re-resolves is a
window class the hold latch's board-change predicate correctly re-opens and the pilot correctly
declines. Ask for a loop-aware re-open predicate, or accept the class and stop targeting the aggregate.

**Prediction.** Pinned predicate **0 of N** (wave 57: 0 of 450), reported ALONGSIDE the loose predicate
(29 of 450) so the two never merge again; identical-declined share **< 15%**, max run **< 10**.

## D9 — MED — `INCOMING THIS COMBAT` is gated on combat PHASES, so a seat at 3 life facing 7 power at Main 1 sees no number

The emitter's own window set is clean: **0 of 112 missing**, four forms (the engine seat resolves them
as declared 48 / closed 60 / forecast 4 / **settled 0, UNTESTED**), arithmetic **48/48 + 4/4**. The
brief's *"437 opponent-combat-phase windows"* is a LOOSE phase predicate; the true emitter set is
**256** and the 144 silent windows all read `0 of them able to attack right now`.

**The residual is the phase gate.** **20 opponent-turn windows carry attackers ready and no total** —
Main 1 ×10, Upkeep ×6, Draw ×4. Sharpest: `125v146` **s23**, seat at **3 life**, **7 power** able to
attack, no line. This is wave-56's R264 one level up: the number that decides the game is computed and
withheld from the windows where a response is still possible.

**Ask.** Emit the `forecast` form at every opponent-turn window from Upkeep onward.
**Prediction.** Opponent-turn windows with ≥ 1 creature able to attack and no `INCOMING THIS COMBAT`
line: **0 of N** (wave 57: 20). Carried from lane B's own residual list: **the latch is keyed on
`observer->turn` and a two-combat turn mis-renders, unguarded and never exercised** — that is D34.

## D10 — MED — the identical-declined note counts option LISTS, not board states, and over-selects ~5×

`[you declined this exact list N times already this turn]` renders on **1,154 prompts**, **801 at
N ≥ 2**, **max N = 43**. The deck146/152/162 seat's independent count of windows whose SITUATION BLOCK
genuinely repeats its predecessor: **163 corpus-wide**; at deck152, **0 of 66** note-carrying
`Cast nothing` windows had an unchanged situation block. Specimen `152v123` **s69-s82** (turn 15): N
climbs 2 → 13 on a three-row menu while the stack drains 4 → 1, life moves 22 → 24, and a trigger
resolves.

**This is why deck152's rung failed 66 of 73 and why sharpening it would have been wrong** (skill
#321): the rung's "breaks" were costless correct declines against a key that measures the wrong thing.

**Ask.** Print a second number: `the board has not changed since M of them`.
**Prediction.** Note-carrying windows at N ≥ 2 where M is absent: **0 of N**; and the reviewer rung
keyed to M (rather than N) is testable for the first time. **A hit is a lane break only if** M is
printed and wrong.

## D11 — MED — a reply with two `CHOICE:` lines is refused rather than revised, and the one occurrence was terminal

`126v146` **s34**: `CHOICE: 3`, ~900 words of reasoning, `So CHOICE: 1`. Refused as
`retracted_choice` — **1 in 3,317** — and it was the seat's last live window; the seat died at 2 life
on turn 20.

**Ask (CORE-PROMPT / parser).** On a **naturally-terminated** reply carrying **exactly two** legal
`CHOICE:` lines, take the **LAST** and stamp `choice_revised`. Ships with its negatives as PARSETEST
cases: a truncated reply keeps the refusal; three or more `CHOICE:` lines keep the refusal; an
illegal last line keeps the refusal.
**Prediction.** `retracted_choice` **0 of N**; `choice_revised` renders and every stamped record's
chosen row is legal. Skill #314 carries the guide half and explicitly does not substitute for this.

## D12 — MED — `{their converter:}` prices one iteration where the same prompt names a LOOP

`125v126` **s293**: a creature cast row prices `52 → 37` while a Sanguine Bond + Exquisite Blood
paragraph 40 lines above names the board fatal — the true cost is `52 → 0`. Coverage itself is fine:
`{their converter:` renders **2 occurrences in 2 windows, 2 takes**, and creature cast rows under a
converter on THEIR battlefield with no clause read **0 of 2** (the brief's *"80 of 82"* counted the
whole prompt including OWN-converter boards; that scope error is recorded, not carried).
**Ask.** When a life-loop is on the board, the converter price prints the LOOP verdict.
**Prediction.** Converter-priced rows on a board carrying a closed life loop that print a finite
single-iteration price: **0 of N**. Fourth corpus for this row's arithmetic.

## D13 — MED — `{identical verdict right now to row N}` renders zero times, and one live unmarked window exists

Lane C predicted **≥ 1**; the literal renders **0 in 3,317 prompts**. D15's sibling tag rendered twice,
so the mechanism is reachable and this specific tag is not wired. The live case:
`146v126` **s25** — Agadeem's Awakening and Silverquill Command, byte-identical verdicts, different
prices, unmarked. **Prediction.** Tag renders ≥ 1 AND unmarked cross-card duplicate pairs **0 of N**
(wave 57: tag 0, pairs 1 of 3 by the engine seat's count, 2 of the 3 marked by D15's tag).

## D14 — MED — three shipped recovery arms are now THREE waves un-executed

`all_assignments_illegal` **0** (`dropped_assignments: 0` on 35 of 35 `blockers`); the `[RE-ASK]`
CHOICE-line form **0** (both renders are the "not on this list" form); `plan_choice_conflict_recovered`
**0**; D11's plan-withdrawal sentence `is withdrawn (you stated it,` **0**. Every one is UNTESTED with
cause (a) — no window — and has been for three waves. **The ask is unchanged and is now overdue: a
forcing flag or a fixture per arm.** Under the silent-instrument rule, an arm that has never executed
is indistinguishable from an arm that cannot.

## D15 — MED — the pilot's own life has no trend line

`since turn N` renders on **3,267 of 3,267** board frames for the OPPONENT (D15 PASS, was 1 of 137) and
nowhere for the seat. `130v126` t16: **18 → 0 in one phase**, no slope shown. Same emitter, one more
call. **Prediction.** Board frames with `Your life:` and no own-life trend: 0 of N.

## D16 — MED — one fact, three wordings, and the X=0 form has no fit clause

`{X pricing:` renders **580 occurrences in 538 windows**; the mana-fit clause exists in three wordings
across 401 cast rows and 28 announce rows, and **7 rows carry none** — all of them the
`X = 0 {X pricing: X=0 - this cast does NOTHING}` form. D9 itself is a clean PASS on four independent
counts (**0 of 55 / 0 of 52 / 0 of 136 / 0 of 28** X rows without a clause, against wave 56's 40 of 40
with none). **Ask.** Give X=0 the `{leaves N of your M floating mana unspent}` sibling.

## D17 — MED — the attacker's lifelink still nests inside the survival verdict at the ATTACKERS seam

D10 is clean at the blockers seam (**0 of 51 B-lines**, and 1,457/1,457 on the deck125/126 seat's own
count, against wave 56's 10 of 36). `152v126` **s20** carries **3 nested occurrences in one record** at
the attackers seam. Lane B's own residual says the un-nesting assumed a blockers-only call site; this
is the unguarded case. **Prediction.** Nested gains at the attackers seam: 0 of N.

## D18 — MED — the fetch row's NEGATIVE colour form drops the colour it CAN make

D19 is otherwise a clean PASS: **0 of 30** fetch windows with no colour tag of either polarity (was 1
of 5); positive form 117 occurrences in 29 records, negative 13 in 4. The residual is 3 renders
(`123v126` s17, `123v130` s31/s40) where the negative form names only what cannot be made.

## D19 — MED — `{feeds:}` states a FACT where every brace around it states a VERDICT

`{feeds:` renders **88 occurrences in 60 windows / 20 takes**, and 88 of 88 carry the hand half D23
shipped. Takes at battlefield-converters = 0 with a hand converter: **4 of 20** (was 1 of 3) — and the
engine seat's verdict is the right one: **the render is TRUE and the rule is being broken**, so the
behaviour routes to deck162's guide (skill #322/#326) and only the shape routes here. Its neighbours
read `{dead right now: …}`, `{right now: returns NOTHING …}`, `[legendary: …]`. **Explicitly not a
legality ask** — deck162's reviewer states this and is correct. Sibling, LOW: Dictate of Kruphix feeds
BOTH players and its row does not say so.

## D20 — MED — lane M's cache-hit counters reach no log, FIFTH corpus

`cache_hits` present on **0 of 42** gameends. Unchanged.

## D21 — MED — `WAGIC_SELFPLAY_SEED` does not exist, so no A/B in this loop can pair its arms

The harness runs `initRand(0)` → `srand(time(0))`; lane H's §D therefore reports unpaired arms and eats
the full SE. A real seed lever makes the log-window A/B (and every future one) game-for-game
comparable at the same cost. Lane H filed it; the docket adopts it.

---

# LANE V — CARRIED, NOT ADJUDICATED (merged after the corpus binary)

Lane V changed `AIPlayerBaka::chooseBlockers` in response to the owner's own Vita play — **and his
framing is the reason it is a Baka change: "on the vita, ive been playing only against the heuristic
ai."** Four transcripts tagged "bad blocking"; his words: *"mostly good, aside from some bad blocks
from the ai, legal, but deterministically bad."* Root causes fixed: pass-1 committed a blind click
gated only on "top-3 damage source + tracked toughness positive" with no trade test and no per-attacker
cap, and `evaluateDeclaration()` compared raw power against toughness, ignoring first/double strike in
both directions. The rewrite resolves a whole declaration over both damage steps (CR 510.4), models
the attacker's cheapest-lethal-first allocation across gang members (CR 510.1c) and handles
deathtouch/indestructible/protection.

**Its A/B, 10 fixed Baka-vs-Baka matchups, control binary (diagnostic only) vs lane binary:**

| metric, per 10 games | base + diagnostic | lane V |
|---|---|---|
| blocks declared | 83 | 82 |
| …by value passes (1-3) | 80 | 57 |
| …by survival sweep | 3 | 25 |
| value-pass blocks losing a creature with no attacker death, not facing lethal | **26 of 80 (32.5%)** | **0 of 57** |
| value-pass declarations spending more creature value than the attacker is worth | **20 of 80 (25%)** | **0 of 57** |
| value-pass gangs (2-body / 3-body on one attacker) | 15 / 6 | **0 / 0** |

The survival sweep's rise (3 → 25) is expected, not a regression: value passes stopped wasting bodies,
so more attackers reach the sweep, which blocks only when incoming is actually lethal. **Per
`feedback-rare-event-ab-power`, 10 games is a direction, not a p-value** — the two headline census rows
are shape claims over real declarations, not rate estimates. Fixtures 57 → 61 AI tests, three RED→GREEN
taken on the archived base binary plus one negative control GREEN both sides.

## D22 — HIGH (wave 58) — adjudicate lane V's four predictions on the wave-58 corpus with `WAGIC_BLOCKLOG=1`

(1) `stage=value` blocks where the blocker dies, the attacker does not, and the seat is not facing
lethal: **0 of N** (lane's own A/B: 0 of 57). (2) `stage=value` declarations spending more than the
attacker is worth: **0 of N** (0 of 57). (3) No `stage=value` multi-body declaration on one attacker
except a menace / three-or-more requirement — every other multi-body declaration must be a
survival-sweep top-up. (4) Against a first-striker, the seat blocks only with a body that survives the
first-strike step or kills in it. **The corpus is Baka-vs-Baka or the GPT seat's fallback path**; the
GPT bundled-blockers override is untouched by lane V and must be excluded from the census.

## D23 — MED — `MTGCardInstance::toggleDefenser`-committed blocks are never click-logged, so two whole sweeps are invisible to the transcript AND unreplayable

Lane V's own residual, named explicitly. The wave-53 survival sweep and the CR 509.1c repair sweep
commit blocks through `toggleDefenser`, which writes no click record — so **every block in lane V's
four transcript reconstructions is a click-path block by construction**, a sweep block in the same
combat would not appear at all, and a transcript carrying one **cannot replay**. This is a hole in the
evidence channel the owner's play reports arrive through, and it silently biases every blocking
diagnosis toward the click path. **Ask.** Log a click-equivalent record at the `toggleDefenser` seam.
**Prediction.** A Baka game with a survival-sweep block replays to the identical end state (wave 57:
untestable — the record does not exist).

## D24 — MED — the deck84 five-on-one pile has no fixture, because `AIStats::stats` cannot be populated inside a single-combat `[AI]` fixture

The headline board of the owner's report — Ironroot Warlord + Squirrel Sovereign + two 2/2 Squirrels
attacking, the seat putting **all five untapped bodies on the Warlord** and letting three Squirrels
through — is covered only by the live A/B and by prediction (3). The old pass-1 gate required
`AIStats::isInTop`, and `AIStats::stats` is built from observed damage events, so it is empty until
three distinct sources have dealt damage: **no single-combat fixture can reach it.** **Ask.** A
multi-turn `[AI]` fixture that lands three damage sources before the combat under test, or a suite hook
to prime `AIStats`. Until then this rule is pinned only indirectly, by the deck58/deck19 fixtures on
the same pass-1 path.

---

# CARRIED ENGINE ITEMS WITH NO CORPUS SURFACE

## D25 — MED — lane S's two watchdog-blind states, and the vpk16 softlock is still NOT REPRODUCED
Ten variations (A-L), all advanced green; two kept as negative-control fixtures. `ActionStack::Update`
is structurally blind in two states: a non-NULL `menuObject` returns early with **no watchdog running
at all**, and a live target chooser skips the priority block while `GameObserver::Update` re-pins the
acting seat to the chooser's owner every tick — which the stall floor's `cancelInterruptOffer` cannot
undo. Both match every reported symptom. Named ownership hole, unproven as the cause:
`ActionStack::cancelPendingChoice()` deletes the chooser with no ownership check, reachable from two
`endOfInterruption(false)` call sites on the Cancel branch. **Not shippable as a fix because no
fixture can reach it.** The cheapest discriminator is a question for the owner's next report, and it is
lane U's too: **did the seat show "opponent is thinking"?**

## D26 — MED — lane U's bound is unverified on hardware, and `WAGIC_GPT_WEDGE` was only run wedge-on-every-request
`abandoned_in_flight` **0** this corpus (PASS — the endpoint answered). The desktop repro table is
complete (6 rows, positive control confirmed via `WAGIC_INFLIGHT_BOUND=0`), and the falsifier is
pinned: **if a future vpk report of the same shape shows no `model call abandoned after N s` line, the
freeze is NOT this defect** and D25 is the remaining suspect. The generation-stamp race is unexercised
in a mixed wedge/answer run.

## D27 — MED — the Intruder Alarm / Thraben Doomsayer activation livelock
Lane F's new residual (`146v123` stub exit 124, NOT the phase-7 shape). **Did not recur** in this
corpus: `126v123` reached 74 attackers on turn 16 and ended naturally; `123v146` ended naturally on
t10. Carried unfixed with no live surface.

## D28 — MED — replay defects 4 and 5, and the human-seat transcript that will not replay
(4) a stack-target click logged without its result digit; (5) engine-generated `choice` ordering.
Lane F fixed the third (`0<name>` refusals 60 → 2). Lane V adds the live case: the owner's own
human-seat transcript diverges at action 61, and lane S's replay of his vpk16 dump diverges at action
25 because the desktop card pool lacks the Vita's card ids. **A human-seat transcript that cannot
replay is the diagnostic channel for his reports failing** — this is worth more than its MED rank
suggests and should be read alongside D23.

## D29 — MED — D34's whose-board term, and the chooser as a per-seat pointer
Lane F read `TargetChooser::Owner` into the watchdog's forced-release path and explicitly did NOT do
the second half (`AbilityFactory::abilityEfficiency`'s whose-board term). The chooser is still ONE
global pointer; a genuine two-simultaneous-choosers case is uncovered, and D25 is in the same
territory.

## D30 — MED — the 30 `otherrestriction` SPELL back-face gates (lane D's D33)
Produced no window; the one spell-face toggle row lane D expected to survive did not render either.

## D31 — MED — the `can play <type>` legacy handler's actor bug, ~60 non-land entries (lane W)
Carried from wave 56's close.

## D32 — MED — Runed Crown / Mantle of the Ancients dormancy (lane X residual); **D33** — Pathway land/land coverage grammar (lane E's deliberate deferral; see D45's pregame half); **D34** — D6's two-combat-turn latch, keyed on `observer->turn`, unguarded and never exercised (lane B); **D35** — a D24 model audit for non-trample blocked-hits-face effects (lane B).

## D36 — MED — lane T's hang guard unwinding through real engine frames
`SoftlockAbort` has never unwound through a real engine hang, only a synthetic spin — **which is why it
ships default OFF** (`WAGIC_HANG_GUARD=1`). The menu entry `Softlock: dump diagnostics and quit to
menu` is always-on and is what covers the owner's actual shape; **the first Vita softlock dump is the
next play report's payload.** Fixture asserts 26 field markers read back from the file.

## D37 — MED — lane G's console / touch / divided-damage verification is the FIRST thing on vpk17
Desktop numbers are measured (−37% at 44 cards, −40% at 39, ~−50% at 55; a small net loss below ~15).
**Never built or run on console**, touch never exercised, and the owner's own Fireball question — a
human dividing damage across three members of a pile — was **not driven** (the test game ended too
early; `tc=1` never appears in AI-vs-AI probe logs by construction). His two binding adjustments are
implemented (expanded groups freeze until every referencing stack item resolves; a board marker on
every pending stack target — and the lane confirms the previously-claimed per-card target border did
NOT exist for spells and now does).

## D38 — MED — the shared-`~/.Wagic/` shutdown race
Three suite runs terminated early with a SIGSEGV core in `GameStateDuel::End` → `SAFE_DELETE(transcriptMenu)`
while other lanes ran wagic concurrently; clean on a quiet machine. Carried from the wave-57 carry-list.

## D39 — LOW — `initMTGCI`'s other 21 unset-but-constructor-covered members
Lane F zeroed six (`swapP`, `swapT`, `MaxLevelUp`, `handEffects`, `castX`, `nb_damages`) after lane G
observed garbage values breaking board grouping on two identical Memnites. The rest are a candidate
sweep.

## D40 — LOW — `repeat_n` has no harvest consumer (14 records, second wave); resource 404s (unchanged in character, no correctness effect).

## D41 — MEASUREMENT / RULING — per-decision inference at a fixed `-j` is not an engine metric, CONFIRMED A THIRD TIME
Mean prompt **ROSE 9.1%** (11,185 → 12,198) while per-decision inference **FELL 1.5%** (27.97 → 27.55 s)
at the same `-j 21`. The prompt-size-drives-latency hypothesis has now predicted the wrong sign three
waves running. **Byte trims are context-headroom items, never latency items**, and the log-window A/B
must not be scored on latency alone.

## D42 — OWNER, ANSWERED — Arena-style grouping. **Built** (lane G). Remaining work is D37.

## D43 — OWNER, ANSWERED — the log window: **"measure."** Mechanism built (lane H), control arm
verified, **A/B blocked on D6.** The owner's ruling stands and the decision is his after the numbers.

## D44 — CARRIED — Vita. vpk16 is his live build; **vpk17 = 67b2bb38a** carries both candidate softlock
fixes (F + U), lane T's dump-and-quit menu entry, and lane G's grouping — **all three unverified
against his hardware**. His open reports: the vpk16 softlock, and "counters only once" (parked by him).
**Lane V is not in vpk17** — it merged after this corpus's binary and belongs to the next port gate.
Port builds remain OWNER-INITIATED.

## D45 — LOW cluster (one lane, one pass)
The GAME LOG's lowercased script tokens — **698 occurrences in 422 prompts** re-derived here
(`boulderloft` 313, `tidechannel` 181, `grimclimb` 79, `agadeem` 75, `pelakka` 22, `emeria` 14,
`skyclave` 14); the deck146/152/162 seat reads 684 and the engine seat 75 at its own seats on a
narrower token list, and `chosen_text` is **clean (0)** — D28 fixed the row head and not the `used:`
emitter. Specimen `123v146` s2: `- Opponent used: grimclimb pathway with Brightclimb Pathway`.
· The `NO LIVE CAST ROW` token's verb disagreement on **443 of 497** renders (`all 1 cast row below
carry a verdict`). · A multi-select `chosen_text` loses the instance handle (`126v123` s25, three
distinct Walls of Omens as one blocker; lane A's D4 fixed the single-row seam and did not audit
multi-select). · The pregame `Mana sources among those lands` line counts only the shown face of a
land/land Pathway (`152v125` s1; deck152 played the alternate face 7 times). · A one-row menu still
costs a round trip (`130v162` s8) — **header text only, recorded as UX, not a cap.**

---

# DISCHARGED — SET B: the wave-57 docket D1-D44 and lanes A-H/S/T/U, by the emitter's actual string

| wave-56 item | lane | wave-57 verdict | disk |
|---|---|---|---|
| **D1** MDFC land-row legality | W | **CLOSED — PASS** | `PLAY THIS AS A LAND` 108 occurrences / 76 windows / **15 takes**, **0 offered where a land play is illegal** (wave 56: 44 of 65) |
| **D2** pregame MDFC land count | E | **CLOSED — PASS 0 of 6** | `counted TWICE above, on purpose` on 6 pregame prompts; MDFC hands under-counted 0 of 6 (was 4 of 4). 38 of 42 seats kept 7; 4 mulliganed once, all kept 6; **0 double-mulls, 0 mull-to-zero** |
| **D3** the false `Land drop:` summary line | W | **CLOSED — PASS** | no contradicting summary reproduced |
| **D4** `chosen_text` fidelity | A | **CLOSED — PASS, and it is the wave's cleanest lane result** | **0 of 2,960** single-row takes mismatch `options_text[choice-1]`; **0** records with `choice ≥ 0` and empty `chosen_text`; 7 `choice: -1` all carry a refusal value. **The EXACT HOLD census reads 450 of 450** — it does. Prefix-matching is retired |
| **D5** the shared async slot | A | **HEADLINE FAIL 108, MECHANISM PASS 0 cross-arm** | → **D4** above |
| **D6** `INCOMING THIS COMBAT` gating | B | **PASS on the emitter's set (0 of 112), residual = the phase gate (20 windows)** | → **D9** |
| **D7** the opponent-converter price | C | **PASS 0 of 2 — thin; the brief's 80-of-82 was a SCOPE error** | → **D12** |
| **D8** bare discard rows | C | **PASS on the class (83.8% → 64.2-73.4%), FAIL on the residual** | → **D3** |
| **D9** the X mana-fit clause | E | **CLOSED — PASS 0 of 55 / 0 of 52 / 0 of 136 / 0 of 28** (was 40 of 40 without) | → **D16** for the X=0 form |
| **D10** nested blocking-trigger gain | B | **PASS 0 of 51 B-lines** (was 10 of 36) | → **D17** for the attackers seam |
| **D11** the stack-aware `{right now:}` | C | **PASS 1 of 1 / 0 of 2 — thin** | redundant second Tributes 0 of 12 (was 3 of 10) |
| **D12** the menu-level dead verdict | C | **SHIPPED AND FALSE ON 8 OF 497** | → **D2** |
| **D13** the narration budget | D | **MEASURED, NOT DECIDED — and the measurement moved** | 64.0% / 87.8% / three per-seat figures. Owner's A/B decides → **D6** |
| **D14** the `Flip Side` toggle | D | **CLOSED — PASS 0 renders** (was 109) | auto-pass line 760 → **1,309** with `(suppressed 1 land-face toggle row(s))`; max run 4, unchanged — higher is the fix working |
| **D15** the opponent LIFE TREND | E | **CLOSED — PASS 3,267 of 3,267** (was 1 of 137) | → **D15 (new)** for the own-life half |
| **D16** the broad conflict census | A | **CLOSED — PASS** | `decision_reversed_in_prose` 6 == narrowed 6 + firings 0 |
| **D17** three un-executed recovery arms | C | **FAIL — third wave, 0 executions** | → **D14** |
| **D18** D11's plan-withdrawal rules | — | **UNTESTED, cause (a)** | `is withdrawn (you stated it,` 0 |
| **D19** the fetch colour form | E | **PASS 0 of 30** | → **D18** for the negative form |
| **D20** the `[<- …]` marker promotion | E | **PASS on coverage (0 unmarked of 526), FAIL on definition** | → **D5** |
| **D21** cycling row pricing | C | **PASS on the clause (0 of 32), UNTESTED on behaviour** | cycle takes at opp life ≤ 6: 2 of 11 vs 3 of 17 — flat, N too small |
| **D22** the blocking-trigger total | B | **PASS 0 of 6 — thin; converter half UNTESTED (0 renders)** | `BLOCKING THIS COMBAT:` 6 |
| **D23** `{feeds:}` in hand | E | **PASS on render (88 of 88), FAIL on behaviour (4 of 20) — routed to the GUIDE** | → **D19** for the shape |
| **D24** the assignable remainder | B | **SHIPPED AND OPTIMISTIC IN THE LETHAL DIRECTION** | → **D7** |
| **D25** `MTGAbility::target` ownership | F | **PASS — 0 SEGV in 21 stderr, 21/21 natural**; the disable flag was not exercised (UNTESTED by construction) |
| **D26** the pay-3-life arrival clause | D | **PASS 0 of 7 on the CLAUSE, and the LETHALITY half is a new HIGH** | → **D1** |
| **D27** `[NAMED BY THEIR …]` per-cast | D | **PASS 16 of 16**, 2 takes |
| **D28** the lowercased script token | D | **PASS on `chosen_text` (0), FAIL on narration (698 occurrences / 422 prompts)** | → **D45** |
| **D29** adjacent duplicate log lines | D | **CLOSED — PASS 0** (was 1,767 in 670 records) | `(x2)` 1,754 / `(x3)` 127; `drew N cards` 4,435 |
| **D30** the cross-card duplicate tag | C | **FAIL — 0 renders** | → **D13** |
| **D31** `hold_windows_skipped`'s stderr companion | A | **CLOSED — PASS 21 of 21**, total 1,863 |
| **D32** resource 404s | — | OPEN, unchanged in character |
| **D33-D41, D44** carried items | — | carried forward as **D30, D29, D28, D20, D27, D39, D25/D37, D41, D44** |
| **D42** Arena grouping | G | **OWNER-ANSWERED, BUILT** | → **D37** |
| **D43** the log window | H | **OWNER-ANSWERED ("measure"), MECHANISM BUILT, A/B BLOCKED** | → **D6** |
| lane **F** hardening | F | **PASS on every observable** — 0 SEGV, 0 phase-stuck, 0 `releasing so the game advances` |
| lane **S** the vpk16 softlock | S | **NOT REPRODUCED (10 variations)**; 2 negative-control fixtures kept | → **D25** |
| lane **T** the softlock escape | T | **UNTESTED — no softlock occurred**; the guard ships OFF | → **D36** |
| lane **U** the in-flight bound | U | **PASS — `abandoned_in_flight` 0**, endpoint answered | → **D26** |
| lane **H** the control arm | H | **PASS on identity (`full` 3,317/3,317), FAIL on instrumentation** | → **D6** |
| lane **V** Baka blockers | V | **UNTESTED BY CONSTRUCTION — not in this binary** | → **D22-D24** |

# DISCHARGED — SET A: wave-56 guide edits, by deck (reviewer verdicts; this step re-derived only what a docket item rests on)

**deck123** P1 PASS 0/45 (7th corpus) · P2 PASS both · P3 PASS 1/1 · P4 PASS on every clause
(Damnation 0/54, edict 0/267, `plan_choice_conflict` 0) with **one break, the Alarm second-copy bracket
1 of 6** · P5 **MOVED and not by an edit** — board-presence windows with ≥ 1 creature **15.9% → 41.0%
(151/368)**, in losses **1.0% → 25.8%**; the seat attributes it to token-makers sticking, not to a
−189-byte guide edit · P6 PASS 14/14, first-tested · P12 SPLIT (life floor 0/26 PASS; the 5+-lands
clause 5 of 26, worse by the letter, 3 of the 5 in the game it WON by mana denial) · P14 retired, 9th
corpus.
**deck125** W110 PASS 59/59 · **W111 FAIL 1 of 1** (`125v126` s293, the Emrakul row) · W112 PASS ·
W113 PASS on the wall half, FAIL 2 of 39 on mana rocks · W114 PASS on every branch with a window
(land drop 83/83, sweeper-at-dead-header 0/45, Path 0, Lightmine **0 casts of 24 — and one decline is
the vs146 loss**, which is #308's origin).
**deck126** D112 PASS 1/1 · D113 FAIL 3, improved (`[second copy:` 2 of 23, was 4; dead Tutor 1 of 10,
was 3 of 17) · D114 PASS 9 of 9 both halves · D115 FAIL 1 of 5 · D116 PASS on the zero stop (445 of 456
rows correctly 0-taken, **fourth corpus at zero**), FAIL on first-window · D117 UNTESTED — **0 of 588
records are `attackers`**.
**deck130** P7 FAIL 2 by the letter, re-scored 2 cast-row breaks · P8 FAIL · P9 UNTESTED 0 of 0 ·
P10 PASS on the shape (0 creature-form of 25) · P11 SPLIT.
**deck146** P1 PASS/PASS/PASS 6 of 6 · **P2 FAIL 1 of 6** (`146v130` s7, Emeria's Call discarded at one
land) · P3 re-keyed (2 of 8 by the key, **both plays correct**) · P4 PASS 0/N.
**deck152** P5 PASS 0 of 0 + 0 of 7 · **P6 FAIL 66 of 73 AND THE KEY IS INVALID** — 0 of 66 had an
unchanged situation block (skill #321, ledger D10).
**deck162** P7 FAIL 1 of N (won anyway) · **P8 FAIL 5 of 20**, 3 of the 5 inside the exception's own
exclusion (skill #326).
**Engine-side wave-56 carries**: P9 PASS ×5 · P10 PASS 0 of 30 · P11 PASS 0 of 109 · **P12 FAIL 108**.
**No win rate is attributed to any edit**: deck152 4/6; deck123, deck126, deck162, deck125, deck130 all
**3/6**; deck146 2/6 — the flattest band on record, n = 6 per deck, real hands, no seed pinning.
All seven seats correctly recorded **NO EXIT** (#87, #167, #217, #292).

---

# SPECIMENS

**The wave's owner-review specimen** (invariant 0, standing) is `wave57/lategame-specimen.txt`:
`1788475511-ai_baka_deck125-…-vs-ai_baka_deck123.jsonl` **seq 176**, turn 45, **15,319 chars, 78%
narration, 2 rows**, and the decision is a land drop. The deck125/126 seat's framing is the one to put
to him: **decision value ≈ zero, and 78% of the prompt is spent on the part of the game that can no
longer be reached.** It is the sharpest artefact this loop has produced for the D43 question he already
answered with "measure."

**Correctness specimens, in docket order.**
· **D1** `146v130` **s20** — `Your life: 1`, `pay 3 life`, taken, `gameend my_life: -2` in the next
record. Under 8 KB end to end (s19-s21) and it is the corpus's cleanest single exhibit.
· **D2** `123v130` **s45/s49/s50** — `NO LIVE CAST ROW ON THIS MENU` above
`Cast Devour Flesh … Rorix Bladewing (6/5) [flying, haste] is sacrificed`, declined three times.
· **D3** `125v130` **s14** — `exiles 0 … 0 of yours` with no dead badge and no header.
· **D4** `game-126v130-1788475528.stderr` — 29 stale drops, 28 of them inside one trigger loop.
· **D5** `130v152` **s33** — `[<- best trade:]` on a 1-for-5, taken, game lost −20.
· **D7** `126v152` **s14** — `one legal assignment gets you to 1` at 5 life against a true floor of −5.
· **D8** `130v126` **seq 62-134** — 73 decisions, 24.0 minutes of inference, 45 pure declines, declined
note at **N = 43**, in one Main phase.
· **The largest prompt in the corpus** is **`125v162` seq 175, 30,228 chars** (turn 27, `ask`) — **not**
`125v130` as the harvest recorded; the engine seat caught the transposition and the disk confirms it.
Runners-up: `125v162` s207 (30,127, `discard`), `125v162` s145 (30,090, `priority`), `162v125` s56
(30,052).
· **The board specimen**, for anyone arguing render cost: `152v123` **seqs 119-145** (turns 21-24) — a
42-permanent, 32-creature opponent board, and the seat's own reply still claims a lethal attack the
printed board refutes.
