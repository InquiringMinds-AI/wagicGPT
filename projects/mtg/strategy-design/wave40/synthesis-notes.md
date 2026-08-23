# Wave-40 synthesis notes — TWENTY-SECOND per-deck-rotation cycle

Pool = 7: holdovers **146** (Orzhov Dungeons WB) / **139** (Temur/Sultai Mutants) / **152** (Bant
Midrange GWU) + **three Step-0 seats taking their FIRST VALIDATION corpus** — **125** Revelation
Fracture (UW draw-go), **126** Sanguine Blood (WGB wall-prison combo), **162** Draw and Die!
(UB forced-draw punish) — + **one Step-0 GUIDELESS entrant, 123** Intruders Of Thraben (WBU token
combo). PRIMARY corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260823-110407`, binary
`465422fcf`, pilot `qwen36-35b-a3b` (FP8 35B, standing serve :8084 — **NEVER cycle**, owner
ruling), -j 3, **NO CAP** (owner ruling 2026-08-21).

**21 games, 21/21 NATURAL — the first fully complete corpus this pool has produced.**
2,073 decisions, **7 fallbacks (0.34%)**: unparsed ×2, stale_echo ×1, empty ×3 (one 3-record
endpoint hiccup, not pilot behaviour), reasoning_only ×1. Suite 1057/0 + 30/0, PARSETEST 671/0.

**ROTATION: one EXIT (139, to canary) and one ENTRANT (130, guideless).** Wave-41 pool =
146, 152, 125, 126, 162, 123, 130.

---

## 1. THE CORPUS-LEVEL STORY

**This is the wave the fix batch landed at near-100% coverage on every metric it was given, the
wave completeness came back, and the wave a shipped guide's OMISSION decked a deck at 70 life
to 7.**

### (a) THE WAVE-39 DOCKET CLOSED ALMOST WHOLESALE, AND IT CLOSED ON FRACTIONS, NOT ON "IT RENDERS"

Ten items closed with independent denominators (`wave40/fix-validation.md`, whose method is now a
skill rung — see §5 M1): #3 at **40/40 = 100%** with **0** leak on two independent KILL metrics;
#4 at **108/109 = 99.1%** and **39/39 = 100%**; #5 at **89/89 = 100%**; #6 at **12/12**; #9 at
**57/57**; #8 at **0 defect-shape pairs in 697 cast asks**; #10 and #16 at a clean zero. The
crash (#1) is discharged on the exact matchup that broke it (139v125, natural, t26). The payoff
metrics moved with the fixes, which is the part that matters: **counters-spent-on-non-threats
9/25 → 2/21** (#4), **`no blockers` with a free block offered 2 → 0/35** (#5),
**`all_assignments_illegal` 1 → 0** (#6), **`validation_reject_reask_exhausted` 5 → 0** (#2).

The one thing the fraction discipline bought that no other instrument would have: **it found
what the fixes did NOT cover.** #6's 12/12 is **single-branch** (every occasion was
flying-vs-no-reach; menace, protection, hexproof-from-blockers and can't-be-blocked-by-walls never
occurred) — an untested branch, not a passing one. #3's 40/40 never touched a **modal** counter
(Silverquill Command: 8 resolutions, 0 counter modes). #10 took the *suppress* branch and left the
*name-the-branch* half unimplemented — which is why the same defect reappeared this wave on the
two cards that win deck126 the game (`{right now: life 0}`, 27 emissions). **A coverage fraction
is a denominator statement; report what the denominator did not contain.**

### (b) THE BIGGEST NEW ITEM IS A DECISION THE MODEL WAS NEVER ASKED TO MAKE — AND IT DECIDED A GAME

`seat125-validation.md` §5.1: **14 Sphinx's Revelation casts, only 10 raised
`Announce the value of X`.** The four that did not all resolved far below what was affordable
(X=3 at 16 mana; X=9 at 18; X=3 at 18; no menu at all at 14). There is **no ANNOUNCE_X record in
the translog** for any of them. The repro is as good as this campaign gets: **the same stderr file
carries a with-menu control 100 lines earlier**, same card, same code path, and the divergence
sits at the `could be Paid with potential mana` branch.

And it decided `125v123`: deck125 was at **70 life against 7**, two Staff of Nin pinging for an
unopposed 4-turn kill, its own prompt printing **`Your library: 1 cards`** — and the un-asked
Revelation drew 3 from a one-card library and lost the game to a draw-from-empty. **This is the
`#2` silent-discard class on a different seam.** The owner's ruling that X=0 is a legal choice a
player should sometimes make is violated by a path that CHOOSES X for the pilot exactly as surely
as by a path that filters the menu.

### (c) THE MASK WAS OBSERVER-SCOPED, NOT ORIGIN-SCOPED, AND THE REAL NUMBER IS 165/0

Wave-39 filed #19 as *"the zone-change narrator masks card identity regardless of ORIGIN zone"*.
`fix-validation.md` F2 refutes the framing while confirming the defect is bigger:
**the origin logic is already right on the ACTOR's seat and wrong only on the OBSERVER's.**
`- You used: …` appears **165 times** corpus-wide; `- Opponent used: …` appears **0 times**.
Every activated ability and every planeswalker loyalty ability is invisible on the opposing seat —
50 `got a loyalty counter` lines for Kaya, 57 for Lolth, **with no cause line anywhere**. The 47
masked library lines the owner spotted are a symptom of a missing CHANNEL, not the defect itself.
**The structural number to fix against is 165/0, not 47** — and it is the ledger's #W41-3, one
lane, with the owner's fix shape binding.

### (d) TWO SEATS PROVED THE SAME THING ABOUT CRUTCHES, FROM OPPOSITE DIRECTIONS

`seat162-validation.md` §2c **refused to execute a removal condition that fired.** The Rule-4
stopgap's registered condition (`grep -c 'STOPS all'` non-empty) is TRUE — 33 emissions — and the
seat correctly declined to retire the crutch, because the crutch is **belief-correcting, not
information-supplying**: the baseline failure was never a missing fact (the fact was on the line),
it was a wrong prior reasoned past the fact. Belief-correcting crutches need failure to stay 0
across **two** corpora with the belief-triggering shape present. This is corpus 1. **The removal
condition itself was under-specified for its crutch class**, and the seat rewrote it.

`seat152-146-validation.md` §4a proved the converse at deck146: **the banned WORDING held at 0
occurrences while the BEHAVIOUR it was banned for came back in different words.** The wave-38 edit
banned `"I take the damage either way"`; that string is still 0. The chump anchor regressed
2/2 → 3/5 anyway, and both violations carry one authored sentence in the reply —
*"preserve … for future blocks"*, *"let attackers through"*. **Banning a string bans the string.**

### (e) THE CROSS-SEAT CONTRADICTION NO SINGLE SEAT COULD SEE

deck152 and deck146 now teach **opposite rules about the same rendered parenthesis**. 152 has a
high-N chump bound (`N ≥ 10` → a `(your blocker dies, attacker lives)` block is a chump); 146 has
an unbounded *"'My blocker dies anyway' is never a reason to take the damage — a 2/1 for 12 life
is the best trade this deck is ever offered"*. Each produced a violation in the OPPOSITE
direction in the same corpus: 152 declined a **free kill** at N=11 (`(you kill it, your blocker
lives)`), 146 double-chumped two Spiders at forecast **17**. **Only the seat that reviewed both
found it**, and only because it reviewed both. Resolution in §3.4; the rung in §5 M3.

### (f) CONSTRUCTION MOVED A SEAT, AND THE SEAT SAID SO BEFORE READING ITS NUMBERS

deck126 rode the owner's `Forest*16 → *1` fix (75/39 → 60/24) **and** its first guide in the same
corpus. `seat126-validation.md` opened with the attribution frame and an **independent in-game
discriminator** (Idyllic Tutor reveal-menu size 61/58 → 50/49/45/37/37) before adjudicating
anything, then split every finding: combo FREQUENCY (3/6 → 4/6 BOTH-at-once) is **CONSTRUCTION**;
the Battlement upkeep leak (22/22 → **0/25**) is **GUIDE**, and provably so — the engine change
ran the *other* way (the option that promised 1 mana now promises up to 7, i.e. became strictly
more attractive, and the pilot declined it anyway). That is the wave-35 attribute-deltas rule
executed correctly under two simultaneous changes.

---

## 2. PER-SEAT VERDICTS

### 2.1 deck125 "Revelation Fracture" — Step-0 guide, first validation corpus

| item | verdict |
|---|---|
| #1 Emrakul belief replaced | **VALIDATED, large effect** — Emrakul in reasoning **144/169 (85%) → 45/267 (16.9%)**; PLAN lines naming Staff of Nin **0 → 92/99 (93%)**, 50 verbatim quotations of the guide's own plan line. Every one of the 45 survivors hand-read: all are either the model quoting the guide's prohibition, or a genuinely resolved Emrakul. `"ramp to 15 mana"` appears only inside quotations; the wave-39 exemplar shape has **0** occurrences. |
| escape clause ("if it is ever printed, take it") | **VALIDATED 2/2, both ARRIVED** (123 s49 t28 at 15+ mana; 126 s66 t48 at 16) — resolution-verified from narration, not the pick. The never-hard-suppress escape did its job. |
| Staff-of-Nin plan at the SEAM | **VALIDATED** — pings **25/25**, Staff casts **6/6** (each on the first window it appeared), Elixir activations **8/8**. vs123 the opponent went 20 → 7 on Staff pings alone. |
| Counter discipline | **VALIDATED, large improvement** — **9/25 (36%) → 2/21 (9.5%)**, all 21 enumerated with the stack line they answered. Violations: `139` s25 (Cancel on Arboreal Grazer 0/3 — on the guide's OWN let-it-resolve list, with `(creature 0/3)` printed on screen), `123` s7 (Cancel on Vision Skeins — a **guide-rule gap**, licensed by the unknown-name catch-all). |
| Sphinx X sizing at the menu | **VALIDATED 10/10 largest X**; the 6-mana cast threshold held (12 declines at 3-4 mana). |
| Path timing | **VIOLATED 6/13** at high life — target SELECTION was **4/4 correct** when a choice existed, so the failure is precisely at the windows where the render skips the facts (→ #W41-8). |
| Sweepers | **8/10.** The 0-creature class is **GONE**. New: `152` s47 cast Final Judgment at `1 is a creature` (Intrepid Adversary 3/1) **while holding two Path to Exile with Path offered as option 2** — VIOLATED. Two windows (`139` s14/s18) are a **guide GAP**, not defiance: "exactly 1 creature → Path it" has no branch for "you hold no Path" or "the lone creature is a 6/6". |
| Lightmine Field gate | **VIOLATED 2/3** — `126` s33 at `0 are creatures`, `126` s38 at `1 is a creature`. 8 mana and 2 cards burned, **with the header number correct on screen both times**. Pure adherence: the rule exists, in two places. |
| Ledger #2 | **metrics VALIDATED, X=0 cast is an HONEST NULL** — the guide's own 6-mana threshold structurally prevents the zero-slack window. Fixture still owed (#W41-2). |
| Ledgers #3 / #4 / #8 | **VALIDATED — CLOSED** at this seat (19 `was COUNTERED` lines with attribution vs 0 baseline; 24/24 stack spell lines carrying all three fields vs 0; 0 defect-shape duplicate pairs). |
| Record 0/6 | **VERIFIED** against six `gameend` records + the deck123 mirror. Wave-39's bogus crash row is gone. |
| **THE HEADLINE LOSS** | **vs123: deck125 DECKED ITSELF while winning 70 to 7.** Resolution-verified chain: opponent at 7 with **0 creatures**, deck125 at 67 with two Staffs; the s71 prompt printed **`Your library: 1 cards`** and `Mana available: 18`; the pilot cast Revelation for X=3 and drew from an empty library. The two turns before were the same mistake cheaper (X=3 at t32, X=9 at t34 out of a sub-15 library, **10 cards discarded at cleanup**). **Routing: GUIDE.** The deciding fact is rendered on every prompt and **no rule in 155 lines reads it.** |
| Loss audit, the other five | vs139 **CONSTRUCTION + matchup** (Final Judgment never appeared in hand or any option list — honest null); vs146 **CONSTRUCTION-ceilinged** (killed by a creature-land a sweeper cannot answer; decisions audited clean); vs152 **PLAY (1 error) + matchup**; vs126 **matchup/combo** with two identified wastes attached; vs162 **PLAY — and the guide's own two rules collide** (see 125-E6). |
| Stopgaps | **BOTH RETIRE** — see 125-S1/S2. |

### 2.2 deck126 "Sanguine Blood" — Step-0 guide + construction change, first validation corpus

| item | verdict | attribution |
|---|---|---|
| §8(a) Battlement upkeep leak | **VALIDATED, exactly 0/N** — 25 upkeep windows offering the tap, **0 taken** (baseline 22/22). Blocker windows opening with a self-tapped Battlement **5/8 → 1/25**. | **GUIDE**, against the grain of the engine change |
| ledger #9 label scales | **VALIDATED** 57/57; guide anchor (the PHASE) unbroken | ENGINE |
| Main-phase manual tap | **VIOLATED (seq)** — `126v139` s14 tapped for 4, then answered "Cast nothing right now" at s15 with `{g}{g}{g}{g}` floating, and s16 shows the Battlement `[tapped - cannot attack or block]`. Entire residual: **1 wasted activation, 1 lost blocker, in 347 decisions** (baseline 21 and 5 in 202). | GUIDE gap — rule #1 is upkeep-only |
| combo assembly | **VALIDATED** — tutor targets **5/5**, tutors cast the turn they were drawable 5/5, **0 reverse errors**, pilot names the loop unprompted in all six games. BOTH-at-once **3/6 → 4/6**. | GUIDE preserves; **CONSTRUCTION** drives frequency |
| "BOTH at once → win" | **REVISED to 3W/1L** — and the loss is an **HONEST NULL, not a win-button violation**: `126v152` had both pieces from t14, cast Perimeter Captain at s28 as the correct setup, and the Captain was **exiled at the opponent's upkeep before the block resolved**. No window existed where a listed win-button option was declined, in any game. Rule #2: **0 violations in 4 BOTH-at-once games**. | opponent interaction |
| ledger #12 may-asks + #22's owed arrival trace | **VALIDATED / DISCHARGED** — 22 asks, **22/22 accepted**, gains traced per attempt (`126v139` t24: 8 asks → 8 echoes → 8×+2 life, **61 → 77**, all before combat damage). | ENGINE |
| §8(c) Tribute victim-naming | **VALIDATED** — fabricated namings **3 → 0** across 116 windows; the one creature-naming PLAN is the guide's sanctioned single-creature case and **won the game** (20/50 → 80/-10). 12k-char blowups 3 → 0. | GUIDE + ENGINE, **not separable** — stopgap removal condition stays **NONE** |
| §8(b) blockers assigned | **VALIDATED** **78/81 (96.3%)** from 14/20 (70%). Best window: `126v139` t24, 5/5 assigned, **+17 life**. | GUIDE |
| rule #5 idle blockers | **VIOLATED (seq) ×2 minor** — `126v146` s13 declined a line offering **A2 (both die)** (a favourable trade + 1 lifelink life); `126v139` s98 declined with Exquisite Blood down (letter violation, ~zero material value). | GUIDE — exception wording too narrow |
| rule #7.7 Chromatic Lantern | **VIOLATED (seq) 4/5 casts.** Rate improved 24% → 4.2%, but the **shape survived intact**: both violations are **double-casts in a single turn**, and `126v146` s37/s38 spent six mana at **2 life on turn 21** of a game lost the next turn. Rule #7.7 states its `{b}` condition correctly; what it does not cover is *"you already control one."* | GUIDE — rule lacks a zero-or-one clause |
| rule #6 lifelink attacks | **VALIDATED 3/3** (baseline 1/3) — old F4 closed | GUIDE |
| §8(d) mulligans | **VALIDATED 0/6**; 6/6 openers exactly `3 lands, 4 spells`; lands 51/52. The mulligan FLOOR was **never stressed** — every hand cleared it by a full land. Untested, not validated. | **CONSTRUCTION** |
| protocol health | 0 fallbacks / 347, 0 `answer_replaced`, 0 `commit_retracted`, `reasoning_degenerate` max 0.009 — clean for the second consecutive wave. | |

### 2.3 deck162 "Draw and Die!" — Step-0 guide, first validation corpus

| item | verdict |
|---|---|
| 1a punisher-before-engine (pred. 1) | **VALIDATED 3 → 0.** All **7** windows where a punisher appeared in the cast list with an empty punisher line were answered with the punisher, same ask, same reply. The wave-39 signature reasoning (*"it doesn't strictly matter which order"*) appears **nowhere** in the corpus. |
| 1b named exception (life ≤ 8 + opponent creature) | **VALIDATED** — fired once, verbatim, with the deferral stated (`146v162` s16). |
| 1c countable brake (one Mine max at zero punishers) | **VIOLATED 1×** (`146v162` s9) against **11 windows of the brake holding under temptation** — Mines sitting in the list, answered "Cast nothing right now". |
| 1d Rule 1 with the brake OFF | **VIOLATED 1× (NEW SHAPE)** — `139v162` s15: one punisher out, 5 mana, Fate Unraveler payable at 4; took a {2} Mine, and by s16 the same turn the punisher was **out of the list**. It never resolved; game lost T12. **Rule 2's "stack without limit" licenses the displacement and Rule 1's "ALWAYS" does not visibly win the collision.** |
| 1e engine→punisher gap | Baseline 2,4,4,4,6,8 → **flat 4,4,4,4,4,4**. Honest reading: **tail eliminated, median unchanged**, and 4 is near this deck's mana floor. The guide moved the seat TO the curve, not past it. |
| 2a ledger #5 `STOPS all N` | **VALIDATED** — 33 emissions, two rendered variants, old buried-tail wording **0**. |
| 2b `no blockers` with `(neither dies)` offered (pred. 3) | **VALIDATED 2 → 0.** All 5 block windows answered with a block, all under NOT-lethal headers. `139v162` s5 is the clean specimen: the reply quotes the render AND the guide in one breath and overrides the core hint by name. |
| 2c Rule-4 stopgap | **DO NOT RETIRE** — belief-correcting crutch, corpus 1 of 2. Removal condition AMENDED; L74-75 **demoted** (see 162-E1). |
| 2d Rule 4 item 3 vs the render | Guide **correct as written**, render **FALSE** → #W41-5. **No guide edit** (the trust doctrine forbids hedging against a render). |
| 3 ledger #10 `draws 0` | **VALIDATED** — 0 rendered occurrences; on this seat's own cards the clause is suppressed entirely. |
| 3 ledger #13 Peer branches | **VALIDATED** — labelled 3/3, menu answered `target opponent`, won the game 19–0 that turn. |
| 4a Fog Bank deploy floor (pred. 2) | **VALIDATED** — the raw ratio (4/17) is the wrong metric; scored against the gate, **every window in which the gate was live was taken, 4/4**, and all 11 declines were creature-free boards. Shield Sphere same lane: 8 offered / 6 taken, both declines took a **punisher** instead (Rule 1 correctly outranking Rule 3). |
| 4b Fate Unraveler ordering | **PARTIALLY VIOLATED 2/4** — held in **both** solo-attacker windows, lost in **both** mixed-attacker windows, with the lethal check never appearing in the reply. **A list-shape trigger, not a comprehension gap.** Cost repeated the wave-39 outcome exactly: both attacks absorbed, zero damage through, punisher tapped out of blocking. |
| 4c lethal exception | **NULL, uncontradicted** — the failure mode is over-attacking, never under. The wave-39 concern (a blanket "never attack" costing a win) is **not realized**. |
| pred. 4 regression watch | **NO REGRESSION** — Mine arrivals **2.17/game**, above the 2.0 floor; the brake released cleanly the turn a punisher landed. |
| protocol health | **0 fallbacks / 134 decisions.** |

### 2.4 deck152 / deck146 — hold seats

**deck152 (5-1).** ⚠ **Structural scope that governs everything at this seat: THREE blocker
windows in six games.** It is an attack-side seat in this pool; the block-lane predictions are
adjudicated on n=3, and the seat said so out loud instead of reporting a percentage.

- **Edit 1 (RETURN-SWING arming test), prediction (a): NULL ON ITS PRECONDITION.** 34 attackers
  windows, **1 ARMED**, and that one offered **four** attackers — the class the edit was written
  against (armed **+** exactly one offered attacker) **did not occur once**. The corpus cannot
  distinguish "the edit fixed it" from "the situation never came up". The measurable sub-clause
  (`"free damage"` 0 times in 357 records) is nearly uninformative at n=1 armed.
  - **What the one armed window DID show (new, unpredicted):** the ≥2-attacker branch applies and
    was **VIOLATED** — `ATTACK: A1, A2, A3, A4`, nothing home. The lethal exception does not
    rescue it (13 power into life 10 with two untapped 0/3 blockers). **Counterfactual stated
    because it changes priority, not verdict:** holding the Wolf home would not have saved the
    seat (the return swing was a 6/6 trampler + a 4/4 flier into life 1, and the seat had zero
    fliers and zero reach). **Conformance violation, no measurable outcome cost — record, do not
    promote.**
  - **The paralysis cost DID land, on the attack lane the edit touched:** `ATTACK: none` answers
    went **0/23 → 2/34**, both at **UNARMED** windows. One is correct (rule #2's named case), one
    is a **MISS** — `152v139` seq19, life 16 vs biggest opposing power 3, a Wolf 4/4 declined
    against a Migratory Greathorn 3/4 **it kills while surviving**. Step (2) now sits inside the
    arming block where the model does its counting and **does not restate its dependence on step
    (1)**.
- **Edit 2 (THE CHUMP TEST): (b) CONFIRMED on n=2 opportunities declined; (c) first half
  CONFIRMED 0/3 trade-drift; (c) second half UNMEASURABLE** — **zero** blocker windows at forecast
  ≤ 9 (the three were 16, 11, 14). The brake's low-N side was never exercised.
  - **The paralysis signal landed one window ABOVE the range (c) was watching:** `152v139` seq20,
    forecast 11, `B1. Wolf (4/4) — may block A1 (you kill it, your blocker lives)`, a free kill of
    a 1/3 with the other attacker explicitly unblockable and the Wolf holding no competing job.
    Answer: `BLOCKS: none`, no PLAN line. **The section has no positive form** — nothing tells the
    pilot that `(you kill it, your blocker lives)` is always taken.
- **L201 frequency claim.** Subject occurs and the teach is corroborated: 152 suffered **9** and
  **8** permanent-loss events in its two grindiest games, kept deploying into both, and **won
  both**. **The defect is the ADJECTIVE**, exactly as wave-32 HL4 rewrote HL6 — `"far more often
  than"` invites the pilot to compare two frequencies it cannot observe instead of executing the
  deploy rule, and the mandatory pre-ship grep hits it. The grep's only other hit (L213,
  *"taking damage while ahead is often correct"*) is a **verbatim quotation of the engine's own
  render string being overridden** — not a defect; leave it byte-identical.

**deck146 (4-2).**

- **The hold-back gate: named requirement DISCHARGED, standing retire recommendation REFUTED —
  KEEP the line.** Wave-39's condition was to report the arming count from `146v139` and
  `146v152` specifically, or retire. Max opposing total power per game: 139 → **10**, 152 → **6**,
  123 → 5, 126 → 1, 125 → 0, 162 → 0 — the two named games are exactly the two that supplied beefy
  boards, as predicted. The gate **armed 3 times** (after two corpora at one total) and **bound the
  decision in all three**, with two replies reasoning from the gate's own arithmetic by name.
  Both retirement conditions fail. Two honest qualifiers kept: neither armed game was won (it
  arms at life 4 and 2, where it can no longer change the result — an argument about WHERE it
  arms, not about retiring it), and `seq34` is a partial **with a false premise** (held a ground
  3/2 *"to block Elite Spellbinder next turn"*; Spellbinder **flies**) → #W41-13, a perception
  item, not a gate item.
- **Chump anchor: REGRESSION, 2/2 → 3/5.** `146v139` seq35 (forecast 7, a blockable 4/4 flier
  with two **reach** Spiders available, `BLOCKS: none`, took 5) and `146v152` seq32 (forecast 2, an
  even **(both die)** trade declined at life 4 to *"preserve Silencer for future blocks"* — the
  future blocks never came; the seat died two turns later). **The banned string is still 0; the
  belief came back in new words.** This is the strongest single reason 146 cannot exit.
- **Opposite sign, same parenthesis:** `146v139` seq23 — both Spiders (2/1) gang-blocked a 3/4 at
  **life 20, forecast 17**, two creatures dead to prevent three damage. **The 146 guide sanctions
  this** with no upper bound. → the cross-seat contradiction, §3.4.
- **MAIN-1 pumps: BINDING, no regression** — Silverquill Command 4 casts, **4/4 in Main phase 1**,
  0 in Main 2, phase read off each record's own `--- CURRENT SITUATION ---` header. Companion
  teach (*"pick YOUR OWN creature"*): the one observable target ask chose **its own** creature.
- Wave-39 #6 (`all_assignments_illegal`): **0 recurrences** across nine blocker windows — with the
  confound stated (deck36 rotated out, so the class's original context is absent; **a clean
  corpus, not a discharged defect**).

### 2.5 deck123 "Intruders Of Thraben" — Step-0 GUIDELESS debut

Record **1/6**, verified against `gameend` — **and the one win is not a kill**: deck125 decked
itself at opponent life 70. **This deck killed nobody in six games.** 278 decisions, 3 fallbacks
(all `empty_reply` in one 3-record endpoint hiccup at 2.5 s each — not pilot behaviour).

- **Preserve ledger enumerated BEFORE any rule was drafted** (Step 0-bis rule 0), and it correctly
  froze the best decision in the corpus: `vs125` seq58, 12 eligible attackers, opponent at 31 with
  0 creatures — the pilot sent **3**, naming Lightmine Field and its N-damage-to-each-of-N
  arithmetic. **A naive "3 of 12 = under-declaration" read would have shipped a loud attack floor
  at a seam that is already right.**
- **#1 leak, and the belief fingerprint is textbook:** the combo is assembled, offered, free, and
  **abandoned mid-loop** — `[cost: Tap]` lines appeared in **43 windows, taken 29, passed 14
  (33%)**. `vs162` seq30→31 is a same-window control: the pilot takes the activation writing
  *"Intruder Alarm will untap Thraben Doomsayer"*, then one decision later passes the identical
  re-armed line reasoning *"I have Intruder Alarm in play, so my creatures won't untap"* and
  *"I have 0 mana"* — **a mana objection applied to a `{T}`-only ability.** The corpus's **#1 and
  #2 largest deliberations** are both 2-option `[cost: Tap]` windows (17,635 and 14,949 chars
  against a 7,203 median). **A 17.6k-character deliberation over "tap this creature for a free
  token, or pass" cannot be about the choice.**
- Four more leaks, all GUIDE-layer, all board-readable: Marsh Flats fetched a **basic** in 4 of 6
  activations with both blue duals on the same list (stranding 2 Intruder Alarms in hand t9→t26);
  Damnation cast onto the pilot's **own larger board** with a same-game control proving the
  discriminator is the printed `of which N are creatures` comparison; Tragic Slip fired where it
  kills nothing 5/9 with a same-window control (a 4/4 targeted while two 2/1s sat on the same
  list); the two edicts treated as one card (**Devour Flesh handed the opponent 16 life in one
  game** it lost at −1).
- **Every loss routes to PLAY**; no loss routes to CONSTRUCTION or ENGINE/REPRESENTATION.
- Five new ledger items (L-123a…e), a complete stopgap register, a 33-row per-claim card-fact
  ledger, and a quoted-render-string sweep with counts. **Draft guide vetting: §4 below.**

---

## 3. THE EXACT GUIDE EDITS

All line numbers are against the **deployed** files in `projects/mtg/bin/Res/ai/baka/`.
Every BEFORE block is byte-verbatim from the deployed file.

### 3.1 deck125_strategy.txt — 7 edits + 2 stopgap retirements

**125-S1 — RETIRE L55-56 (registered stopgap; removal condition FIRES).**
Measured: `/usr/bin/grep -c "on the stack: .*{"` = **5, 2, 5, 7, 1, 5 — non-zero in all six seat
files**; 24/24 spell stack lines now carry type + cost + P/T. **The sentence is now FALSE about
the render, and under the trust doctrine a false statement about the render must not ship.**

BEFORE (L55-56):
```
The stack prints only a name - "1 (top): opponent's Silver Myr [spell]". The card's rules text is in
the OPPONENT DECK list at the top of this prompt; that is where you read what it does.
```
AFTER: **delete both lines.** (Replaced by the numeric rule in 125-E7, which must land in the same
edit — do not delete the name table without it.)

---

**125-S2 — RETIRE L91-92 (registered stopgap; both halves of the removal condition fire).**
`validation_reject_reask_exhausted` = **0** corpus-wide (1,989 decisions); ledger-#8-shape
duplicate pairs = **0** in 697 cast asks.

BEFORE (L91-92):
```
If you choose Sphinx's Revelation and your very next prompt still shows Sphinx's Revelation in your
hand with your mana unspent, cast something else this turn and take it again next turn.
```
AFTER: **delete both lines.** Neither leaves a hole — the 6-mana threshold and the largest-X menu
line cover what they were bracing.

---

**125-E1 — THE LIBRARY CLAUSE (the highest-value edit in this wave, at any seat).**
Routing is GUIDE: the deciding fact `Your library: N cards` is rendered on **every** prompt and
**no rule in 155 lines reads it**. Insert after L85 (end of the Revelation threshold paragraph).

AFTER (insert):
```
BEFORE you take Sphinx's Revelation, or any other optional draw, read the line "Your library: N
cards". If N is 5 or less, answer "Cast nothing right now" and keep the card: drawing your last
card loses the game outright, and Sphinx's Revelation draws X on top of your normal draw step.
At N of 6 or more, cast it as above and pick the largest X the menu offers, EXCEPT that you never
announce an X larger than N minus 2.
Once the opponent's printed life is 10 or less and you control a Staff of Nin, stop drawing
entirely: the Staff clock finishes the game on its own and every extra card is a turn of library
you are spending for nothing.
```
Why this shape: it is keyed to a printed countable, it names the two numbers to compare, and its
second paragraph is the one that would have saved `125v123` (70 life, opponent at 7, two Staffs).

---

**125-E2 — PATH DECLINE FLOOR, rekeyed to the surface that actually carries the facts.**
6/13 casts violated the floor, 4 of them on printed 0/4 walls at 25-50 life; target SELECTION was
4/4 correct. Per #W41-8 the **cast option line names the target with no P/T and no tags**, and at
exactly one legal target the sub-menu that would carry them never opens — so the rule must send
the pilot to the BATTLEFIELD line by name.

BEFORE (L63-66):
```
So: cast Path to Exile at the creature with the HIGHEST printed power on their battlefield line. When
every target it offers you is tagged [defender] or has printed power 1 or 0, answer "Cast nothing
right now" and keep the Path for the real threat - unless your life is 12 or less, and then exile the
biggest creature they have right now.
```
AFTER:
```
The "legal targets right now:" clause on the Path option gives you NAMES ONLY. Before you answer,
find each of those names on the opponent battlefield line and read its (P/T) and its tags there.
Then: cast Path to Exile at the creature with the HIGHEST printed power. If EVERY name on that
clause reads printed power 0 or 1 on the battlefield line, or is tagged [defender], answer "Cast
nothing right now" and keep the Path - at any life total above 12, whatever else is on the board
and however many turns you have been holding it. A 0/4 wall is never a Path target.
Your life is 12 or less: that floor is off - exile the biggest creature they have right now.
```
(The added "at any life total above 12, whatever else is on the board and however many turns you
have been holding it" is the falsity-fact interception the sanctioned negative form permits — all
six violations were at 18-50 life with the wall visible.)

---

**125-E3 — THE SWEEPER "EXACTLY 1 CREATURE" BRANCH NEEDS ITS TWO MISSING FALLBACKS.**
`139` s14/s18 were **good Magic against a guide-rule mismatch** (lone 6/6, no Path in hand);
`152` s47 was a real violation (Final Judgment on a 3/1 with two Paths in hand and Path offered).

BEFORE (L76):
```
- exactly 1 creature: Path to Exile it and keep the sweeper.
```
AFTER:
```
- exactly 1 creature, and Path to Exile is in your list: take the Path and keep the sweeper. A
  {W} instant is the right price for one creature; a sweeper is not.
- exactly 1 creature, and Path to Exile is NOT in your list: cast the sweeper when that creature's
  printed power is 4 or more, otherwise answer "Cast nothing right now" and keep it.
```

---

**125-E4 — RELOCATE THE LIGHTMINE FIELD GATE INTO THE HEADER-COUNT TEST (HL9: salience failure is
repaired by relocation, not by a second sentence).** The gate exists at L106-107 and again at
L148; both violations happened with the correct header number on screen, in the section where the
pilot was already counting. Do not add text — move the branch into the read.

Insert into the SWEEPERS list, after the amended L76-77 block:
```
- 3 or more creatures, and Lightmine Field is in your list: cast Lightmine Field. At 0, 1 or 2
  creatures on that header it kills nothing at any size, so keep it and spend the mana on a Path
  or a sweeper instead.
```
And in the LIGHTMINE FIELD section, replace L106-107's standalone gate with a pointer so the rule
lives in exactly one place:

BEFORE (L106-107):
```
Cast Lightmine Field when the opponent battlefield header counts 3 or more creatures. Against one or
two big creatures, spend that mana on Path to Exile or a sweeper instead.
```
AFTER:
```
Its gate is the same header count you already read for your sweepers - see that list.
```

---

**125-E5 — RESOLVE THE ESCAPE-CLAUSE CONTRADICTION.** The escape clause fired twice and both
Emrakuls ARRIVED; the guide's flat statements *"You never have a creature on the battlefield"*
(L3) and *"you have no creature to attack with"* (L17) become FALSE at that moment, and the pilot
noticed (`126` s66/s67: *"we don't attack with Emrakul according to the plan" … "Emrakul can't
attack."*).

BEFORE (L19-21):
```
If "Cast Emrakul, the Aeons Torn" is ever actually printed in your numbered list, take it - the game
only prints a cast you can already pay for. Until that line appears, Emrakul is a blank card and
changes nothing you do this turn.
```
AFTER:
```
If "Cast Emrakul, the Aeons Torn" is ever actually printed in your numbered list, take it - the game
only prints a cast you can already pay for. Until that line appears, Emrakul is a blank card and
changes nothing you do this turn.
Once an Emrakul of yours IS on the battlefield, the two sentences above about having no creature
stop applying to it: declare it as an attacker in every attackers window that offers it, and
assign it as a blocker whenever the block line does not read "your blocker dies". Nothing else in
this guide changes.
```

---

**125-E6 — ORDER THE STAFF RULE AGAINST THE HOLD-MANA RULE (they collided and cost the vs162
game).** At t12/t13 the pilot tapped out for Staff of Nin per L27-28 (*"ahead of anything except an
answer you need this same turn"*); at t14 the opponent resolved **Underworld Dreams + Shield
Sphere + two Howling Mines in one turn** unanswered — Underworld Dreams and Howling Mine are
**both on the guide's own COUNTER-ON-SIGHT list** — and the pilot answered `Cast nothing right
now` at `Mana available: 1` in both windows it got. **The two rules have no precedence between
them.** This is the same shape as deck162's W41-RULE12; fix it in both guides this wave.

BEFORE (L27-28):
```
- Cast it the first turn "Cast Staff of Nin" appears in your list, ahead of anything except an answer
  you need this same turn.
```
AFTER:
```
- Cast it the first turn "Cast Staff of Nin" appears in your list - but only when doing so leaves
  {1}{U} still untapped. If casting the Staff would take your "Mana available:" line below {1}{U},
  pass this turn and cast it next turn with a land to spare: a Staff you cast one turn later still
  wins, and a turn spent tapped out is the turn their engine lands uncountered. This rule outranks
  "cast it the first turn it appears"; nothing else does.
```

---

**125-E7 — REPLACE THE TWO NAME LISTS WITH A NUMERIC RULE KEYED TO THE NOW-PRINTED FACTS, KEEPING
ONLY THE NON-DERIVABLE NAMES.** Ledger #4 shipped at 99.1%/100%; the facts are on screen (both
residual misspends had them printed — `Arboreal Grazer {g} (creature 0/3)`). This is the edit
125-S1's deletion depends on, and the seat's #1 authoring recommendation. **Must land in the same
edit as 125-S1.**

BEFORE (L39-54, the two bulleted name lists and the unknown-name catch-all — from
`COUNTER ON SIGHT, whatever else is going on:` through
`your life is 12 or less; otherwise, let it resolve and answer the permanent afterwards.`)

AFTER:
```
The stack line and the counter option both print the target's TYPE, MANA COST and (P/T):
"1 (top): opponent's Overgrown Battlement {1}{g} (creature 0/4) [spell]". Read them and decide
from the numbers, not from the name:
  - (creature P/T) with printed POWER 2 or more: COUNTER IT.
  - (creature P/T) with printed power 0 or 1: LET IT RESOLVE. It cannot kill you, and Path to
    Exile or a sweeper answers it later for less mana. This holds however expensive it was and
    however much the reasoning wants the counter spent.
  - anything printed (land), (artifact) with no P/T, or (instant)/(sorcery) that only draws or
    searches: LET IT RESOLVE.
COUNTER ON SIGHT regardless of the numbers - these are the names whose danger is not visible in a
P/T, and this list is short on purpose:
  - every planeswalker;
  - every permanent that hits you or grows them EVERY turn: Underworld Dreams, Fate Unraveler,
    Sanguine Bond, Exquisite Blood, Howling Mine, Staff of Nin, Nuisance Engine;
  - Idyllic Tutor: it is not a permanent, but it fetches the enchantment half of their combo.
```
Also update the two DECIDING SITUATIONS lines that restate the old table (L141-144) so both copies
agree — a half-updated restatement is worse than none:
```
- Their spell on the stack prints (creature P/T) with printed power 0 or 1, or prints no P/T at
  all: let it resolve and keep the counter.
- Their spell on the stack prints (creature P/T) with printed power 2 or more, or is a
  planeswalker, or is one of the named permanents above: counter it now.
```

---

**125 — CARRIED WITH A NAMED DEADLINE (no edit this wave).** The mulligan line L131-133
(*"having already taken"*) is **UNEXERCISED for the second consecutive corpus** (0 mulligans in 12
games). Per the unexercised-teach rung this is its **last free wave**: wave-41 either arms it with
a probe deck or the reviser **cuts the line**. Recorded here so the next seat does not re-derive
the clock.

### 3.2 deck162_strategy.txt — 3 guide edits + 1 register amendment

**162-E1 — DEMOTE L74-75 (the information-supplying half of the Rule-4 crutch; the render now
duplicates it verbatim, and the multi-attacker variant makes the shipped wording partly stale).**

BEFORE (L74-75):
```
   Shield Sphere is tagged "[deals 0 - this block kills nothing, but it STOPS all N damage from
   reaching you]": that STOPS clause is the whole job, and N is the damage the block erases.
```
AFTER:
```
   The wall line's "STOPS all ..." clause is the whole job - it names the damage the block erases.
```
**Keep unchanged**: L67-70, L73, **L81-82** (the falsity fact — that is the belief-correcting half
and it is under the 2-corpora bar), and the Rule-4 override header at L65-66.

---

**162-R1 — AMEND THE REMOVAL CONDITION in `wave39/deck162/findings.md` §10, row 1 (replace the
removal-condition cell verbatim).** The condition as written FIRES NOW and executing it would be
wrong: the crutch is belief-correcting, not information-supplying, and this is the corpus that
first validated the engine fix it guards.
```
Delete only when BOTH hold: (a) `/usr/bin/grep -c 'STOPS all' <corpus>/*.jsonl` is non-empty,
and (b) `no blockers` chosen with a `(neither dies)` line offered under a NOT-lethal header
stays at 0 across TWO corpora that both contain a wall-block window (belief-correcting crutch,
2-corpora bar; corpus 1 = matchups-20260823-110407, PASSED 0/5 windows).
```

---

**162-E2 — THE RULE 1 / RULE 2 COLLISION (W41-RULE12).** Rule 1 says "ALWAYS", Rule 2 says "stack
engines without limit" once a punisher is out, and neither outranks the other. `139v162` s15:
5 mana, one punisher out, Fate Unraveler payable at 4 — took a {2} Mine and the punisher fell out
of the list the same turn. **Rule 1's "ALWAYS" needs a mana-order clause that survives the brake
being off.**

BEFORE (L31-33):
```
- ONE OR MORE of them on your battlefield: cast every draw engine you are offered, every turn, as
  fast as you can - each one multiplies the damage. This is where the deck wins. Stack Mines,
  Dictates and Puzzle Boxes without limit.
```
AFTER:
```
- ONE OR MORE of them on your battlefield: cast every draw engine you are offered, every turn, as
  fast as you can - each one multiplies the damage. This is where the deck wins. Stack Mines,
  Dictates and Puzzle Boxes without limit, with ONE ordering rule that never lapses: if a second
  punisher (Underworld Dreams, Fate Unraveler, Ob Nixilis) is ALSO in this ask's list, cast the
  punisher FIRST and the engines with whatever mana is left. Rule 1 outranks this rule at every
  life total and on every board, whether or not a punisher is already out. A punisher you priced
  out of your own turn is a punisher that never resolves.
```

---

**162-E3 — THE FATE UNRAVELER HOLD IS LIST-SHAPE SENSITIVE (W41-ATTACKLIST).** Held in **both**
solo-attacker windows, lost in **both** mixed-attacker windows, and **the lethal check never
appears in the reply**. The repair is a countable one-liner AT THE DECLARATION, not more
lethal-check prose (HL9: the failure is placement/salience, not coverage).

BEFORE (L129-132, the sentence beginning `FATE UNRAVELER stays home` through `...end it.`):
```
FATE UNRAVELER stays home unless the lethal check below clears. Its damage comes from the opponent's draw step, not from
combat, and attacking taps it out of blocking on their turn. Send Fate Unraveler ONLY when its power
plus your other unblocked attackers is at or above the opponent's printed life total - then swing
with everything and end it.
```
AFTER:
```
FATE UNRAVELER stays home. Before you write "ATTACK:", read the names your attacker list offers:
if it holds ANY name other than Fate Unraveler, declare those names and LEAVE FATE UNRAVELER OUT.
Its damage comes from the opponent's draw step, not from combat, and attacking taps it out of
blocking on their turn. The one time it goes: the printed opponent life is at or below the total
power of every attacker you are sending including it - then run the lethal check below, swing with
everything and end it. A list with two names in it is where this rule gets forgotten; it applies
there most of all.
```

### 3.3 deck126_strategy.txt — 3 edits

**126-E1 — EXTEND RULE #1's WINDOW FROM A PHASE TO A CONDITION.** The leak moved one phase later
(both surviving activations are Main phase 1, which rule #1 does not name; one was productive, one
burned 4 mana and cost a blocker). **The minimal edit is to widen the ALWAYS-FALSE clause, not to
add a second phase-keyed rule** — rule #1's phase anchor is what survived the #9 label fix and it
must not be re-keyed to the label.

BEFORE (L17-20):
```
The thought "I should ramp first so I can afford Sanguine Bond" is ALWAYS FALSE in the upkeep
window: mana made there does not survive to the main phase where the enchantment is cast.
The ONE time you take it: you are casting Tribute to Hunger in that same window and need the mana
to pay for it right now.
```
AFTER:
```
The thought "I should ramp first so I can afford Sanguine Bond" is ALWAYS FALSE in the upkeep
window: mana made there does not survive to the main phase where the enchantment is cast.
The same is true in EVERY window, your main phase included: tapping the Battlement by hand is
right only when the very next thing you do in that same decision is cast a spell with that mana.
If your answer to the following ask would be "Cast nothing right now", the tap was wasted and the
wall is now a 0/4 that cannot block on their turn.
The ONE time you take it: you are casting Tribute to Hunger in that same window and need the mana
to pay for it right now.
```

---

**126-E2 — RULE #7.7 BECOMES A ZERO-OR-ONE RULE.** 4 of 5 casts violated it, and **both violation
pairs are double-casts in a single turn** — `126v146` s37/s38 spent six mana at 2 life on turn 21.
The `{b}` condition is correctly stated; what is missing is "you already control one".

BEFORE (L96-98):
```
 7. Chromatic Lantern - only when the "colours you can make:" line does NOT include {b}. Your
    Bayous, Scrublands, Savannahs and Urborg already make black; when {b} is on that line the
    Lantern does nothing for 3 mana, so take a wall or a combo piece instead.
```
AFTER:
```
 7. Chromatic Lantern - only when BOTH are true: the "colours you can make:" line does NOT include
    {b}, AND no Chromatic Lantern is already on your battlefield line. Your Bayous, Scrublands,
    Savannahs and Urborg already make black; when {b} is on that line the Lantern does nothing for
    3 mana. A SECOND Lantern does nothing at any time - once one is on your battlefield this entry
    is finished for the rest of the game, so take a wall or a combo piece instead.
```

---

**126-E3 — WIDEN RULE #5's EXCEPTION SO IT STOPS EXCLUDING FAVOURABLE TRADES.** Two minor
violations, both a Vampire declining a line that was not the excepted wording (`126v146` s13
offered **A2 (both die)** — a favourable trade plus 1 lifelink life; `126v139` s98 declined with
Exquisite Blood down). The exception's wording is too narrow in one direction and its release is
too quiet in the other.

BEFORE (L76-78):
```
The one blocker you leave out: neither enchantment is on your battlefield AND that blocker's line
reads "your blocker dies, attacker lives". Every other line - "neither dies", or any line at all
once an enchantment is down - blocks.
```
AFTER:
```
The one blocker you leave out: neither enchantment is on your battlefield AND that blocker's line
reads "your blocker dies, attacker lives" on EVERY attacker it is offered. A line reading "both
die" is not that line - take it; a body of theirs for a body of yours is a trade you want, and a
lifelink blocker gains you life on the way. Once either enchantment is on your battlefield, every
blocker blocks, on any line at all.
```

### 3.4 THE CROSS-SEAT CHUMP CONTRADICTION — ONE RULE, TWO CALIBRATIONS

**The contradiction, stated precisely.** deck152 teaches *"N ≥ 10 → a `(your blocker dies,
attacker lives)` block is a chump; leave it unblocked"*. deck146 teaches *"'My blocker dies
anyway' is never a reason to take the damage — a 2/1 for 12 life is the best trade this deck is
ever offered"*, with **no upper bound**. Both fired this corpus and both misfired, in **opposite**
directions:

| seat | window | what happened | what the rule did |
|---|---|---|---|
| 152 | `152v139` seq20, forecast 11 | declined `(you kill it, your blocker lives)` — a FREE kill, blocker survives, no competing job | the N-threshold's salience swallowed a line it was never about |
| 146 | `146v139` seq23, forecast 17, life 20 | double-chumped two 2/1 Spiders into a 3/4 to prevent 3 damage | the unbounded rule has no high-N cut |
| 146 | `146v152` seq32, forecast 2, life 4 | declined an even `(both die)` trade to *"preserve Silencer for future blocks"* | the rule's own low-N case lost to a preservation instinct it does not name |

**THE RESOLUTION I CHOSE: the PARENTHESIS LADDER — three rungs keyed to the three strings the
blocker window actually prints, installed VERBATIM-IDENTICAL in both guides, with exactly ONE
number differing.**

The diagnosis that makes this work: **the two guides were never really in conflict at the line
they argue about.** Both agree that a blocker-dies block is a chump at high N and a bargain at low
N; they differ only on where the line sits (152: 9, 146: 10) — a one-point calibration difference
that is a legitimate deck property (152's creatures ARE its clock; 146's are cheap chaff and its
life total is the resource). **The entire contradiction lives at the OTHER two parentheses**,
which each guide has collapsed into the chump rung from opposite ends: 152 collapsed rungs 1 and
2 UP into rung 3 by threshold salience, and 146 collapsed rungs 1 and 2 DOWN into rung 3 by its
unbounded wording. So the fix is not to pick a winner — it is to **separate the rungs the render
already separates for us**, and let only the floor differ.

**THE LADDER (identical text in both guides; `<FLOOR>` = 9 in deck152, 10 in deck146):**
```
BLOCK LADDER - the block window prints one parenthesis per offered block. Read it and answer from
this ladder, top down. The parenthesis is the answer; do not re-derive the combat.
 1. "(you kill it, your blocker lives)" - TAKE THAT BLOCK, always, at every life total and every
    forecast number. It costs you nothing: their creature dies, yours does not, and the damage is
    gone. No number on this screen can make this block wrong.
 2. "(both die)" - TAKE THAT BLOCK unless the attacker it kills is worth less than your blocker.
    A body for a body is a trade you want, and the damage is gone as well.
 3. "(your blocker dies, attacker lives)" - THIS is the chump line, and it is the ONLY line the
    forecast number decides. Read "you would be at N". Take this block when N is <FLOOR> or less,
    or when the header says the attack IS lethal. When N is above <FLOOR>, leave that attacker
    unblocked and keep your creature.
ONE BLOCKER STOPS ALL OF A NON-TRAMPLING ATTACKER'S DAMAGE, however big the attacker is. Blocking
CHANGES the forecast number: each non-trampler you block deals you ZERO. "I take the damage either
way" is FALSE against a non-trampler, and so is "I will preserve this creature for future blocks"
and "let the attackers through" - a creature you preserved at N of <FLOOR> or less is a creature
you kept for a turn you will not get.
"BLOCKS: none" is correct only when every offered block is rung 3 above your floor.
```

**Application per deck.**

- **152-E3** — replace the CHUMP TEST at L93-98 with the ladder, `<FLOOR>` = **9**. This ships
  W41-2's missing positive form (rung 1) and keeps the confirmed prediction (b) result intact:
  its two declines this corpus were at N = 16 and N = 11 on rung-3 lines and stay correct.
  Leave L90-92 (Triumphant Adventurer), L99-101 (Katilda) and **L213 byte-identical**.
- **146-E1** — replace L124-133 with the ladder, `<FLOOR>` = **10**. This simultaneously (i)
  re-teaches the chump anchor that regressed, (ii) installs the high-N bound 146 lacked (fixing
  seq23's double-chump at 17), (iii) fixes seq32's declined even trade via rung 2, and (iv) keeps
  seq35's case (forecast 7 ≤ 10 → block the biggest non-trampler) with the ZERO-damage arithmetic
  that makes it work restated inline.
- **146-E2** — the belief, named in the pilot's OWN new words, is built into the ladder's closing
  paragraph above (*"I will preserve this creature for future blocks"* / *"let the attackers
  through"*). This is the lesson of §1(d): the wave-38 edit banned a string and the string stayed
  at 0 while the behaviour returned in different words. **Name the belief, not the phrase** — and
  place it at the decision, immediately after its positive rule (the sanctioned negative form).
- **146-E3** — update the DECIDING SITUATIONS restatement so both copies agree.
  BEFORE (L231-233):
  ```
  - The forecast's "you would be at" number is 10 or less -> chump-block the biggest non-trample
    attacker with your cheapest creature; a blocked non-trampler deals you ZERO - you never "take
    it either way".
  ```
  AFTER:
  ```
  - A block line reads "(you kill it, your blocker lives)" -> take it, whatever the forecast says.
  - A block line reads "(both die)" -> take it unless their creature is worth less than yours.
  - Only "(your blocker dies, attacker lives)" is decided by the forecast: take it at 10 or less,
    leave it above 10. A blocked non-trampler deals you ZERO - you never "take it either way", and
    "preserve it for future blocks" is not a reason at 10 or less.
  ```

**Why one shared ladder rather than two tuned rules:** it is greppable across seats (a future
reviewer can diff the two guides' ladders and see the floor is the only difference), it is
render-keyed at every rung so it survives pool rotation, and it makes the next cross-seat
divergence detectable instead of invisible. The rung this produced for the skill is §5 M3.

### 3.5 deck152_strategy.txt — remaining 2 edits

**152-E1 — L201-202 REWRITE (HL4/HL6 frequency claim; ready to apply verbatim).** The subject
occurs (9 and 8 permanent-loss events in the two grindiest games) and the teach is corroborated
(won both) — **the defect is the adjective**, exactly the documented way to break HL6 while
fixing it: a reviewer holding two supporting games reaches for the supporting adjective.
`"far more often than"` invites the pilot to compare two frequencies it cannot observe instead of
executing the deploy rule.

BEFORE (L201-202):
```
- vs REMOVAL/GRIND: keep deploying anyway - an empty board loses these far more often than
  walking into removal does. Keep attacking so the clock forces trades.
```
AFTER:
```
- vs REMOVAL/GRIND: keep deploying anyway. Their removal answers one creature at a time and your
  board is what wins the game; a creature you held in hand blocks nothing and deals nothing. Cast
  the best creature offered on every one of your main phases, and keep attacking so the clock
  forces trades.
```

---

**152-E2 — RE-ANCHOR STEP (2) OF THE ARMING TEST TO STEP (1), AND NAME THE UNARMED DEFAULT IN THE
SAME BREATH (W41-1).** `ATTACK: none` answers went 0/23 → 2/34, both at UNARMED windows; step (2)
now sits inside the arming block where the model does its counting and does not restate its
dependence on step (1).

BEFORE (L42-44):
```
    (2) COUNT THE ATTACKERS THE LIST OFFERS YOU. When (1) armed and the list offers exactly ONE
        attacker, your answer is "ATTACK: none" - that one creature is your blocker. The thought
        "it is free damage" is ALWAYS FALSE here: that damage costs you the block.
```
AFTER:
```
    (2) ONLY IF (1) ARMED: count the attackers the list offers you. When (1) armed and the list
        offers exactly ONE attacker, your answer is "ATTACK: none" - that one creature is your
        blocker. The thought "it is free damage" is ALWAYS FALSE here: that damage costs you the
        block.
        IF (1) DID NOT ARM, this whole check is over and you skip step (2) entirely: attack with
        every creature the list offers, including a sole attacker, per rule #2 above. A creature
        that kills its blocker and survives always attacks; holding it back at a healthy life
        total is a turn of damage thrown away.
```

**Deck152's rotation-carried condition is in §4.2 — its two edits are NOT adjudicated by this
corpus and the wave-41 discharge is a probe deck, not a third corpus.**

---

## 4. ROTATION ADJUDICATION

**Standing per-deck rule**: a seat EXITS only when **both** hold — the guide was unmodified this
wave **and** the seat produced no signal for additional work.

| seat | guide modified? | seat signal? | verdict |
|---|---|---|---|
| **139** | No (byte-frozen wave-39) | No seat review; the only open 139-scoped item is #W41-4, an ENGINE render item | **EXIT → canary** |
| **152** | **YES** (this wave's two edits are the modification under test) | YES — W41-1, W41-2, W41-6 | **HOLD**, with a mandated probe |
| **146** | **YES** (this wave: the ladder + 2 HL6 strings) | YES — the chump-anchor REGRESSION, plus W41-4/5/7/8 | **HOLD** |
| **125** | **YES** (7 edits + 2 stopgap retirements) | YES | **HOLD** |
| **126** | **YES** (3 edits) | YES | **HOLD** |
| **162** | **YES** (3 edits + a register amendment) | YES — and its Rule-4 crutch is at corpus 1 of a 2-corpus bar | **HOLD** |
| **123** | Step-0, initial guide authored this wave | — | **HOLD** (Step-0 seats never rotate) |

### 4.1 deck139 — EXIT to canary, and #W41-4 re-keyed to a PROBE DECK

All three of wave-39's named hold reasons are resolved or reassigned:

1. **Party to the Affinity crash — DISCHARGED.** `139v125` ended **naturally at t26**; 21/21
   natural corpus-wide.
2. **The owed 139v125 rerun — DISCHARGED.** The matchup completed and both `gameend` records are
   paired; the wave-39 bogus `results.tsv` row is gone (#16 closed at 21/21 natural).
3. **The mutated-pile render item (#7) — STILL OPEN**, and now quantified at **14/20 = 70% wrong**
   and reaching **both** boards.

**The judgment call is item 3, and I am NOT holding the seat for it.** Carrying an entire seat
through a wave to validate one MEDIUM-class render fix is the expensive answer, and the cheap one
is already a proven campaign pattern: **#W41-4 is discharged by a MUTATE PROBE DECK** (198/199,
stacked with Everquill Phoenix / Gemrazer / Snapdax / Brokkos plus Arboreal Grazer / Dryad of the
Ilysian Grove / Pollywog Symbiote hosts), one pinned GPT one-shot, then a pure render grep over
the option lines — deterministic, cheaper than a corpus seat, and it exercises the defect at a
density the random pool never reaches (20 pile lines in 21 games). The probe-deck rung exists for
exactly this: *"probe decks discharge what the random pool can't."*

Two supporting facts: the mutate machinery is otherwise **fully validated** (wave-39 #18: 10/10
casts landed on the intended host, over AND under honoured), and #W41-4's defect is confined to
option/target lines — battlefield-summary lines are clean — so a render grep is a complete
instrument for it. **139's guide stays deployed as a canary. Canary list grows to 27.**

### 4.2 deck152 — HOLD, and I am REFINING the validator's discharge proposal

The seat proposed: report (i) armed-**sole**-attacker count and (ii) blocker windows at forecast
≤ 9 in wave-41; if either is still 0 **after that corpus**, discharge by probe deck.

**ADOPTED with one refinement: run the probe deck IN wave-41, alongside the corpus, not
conditionally after it.** The rationale is the unexercised-teach rung read forward. We have now
MEASURED the starvation twice, not guessed at it — **1 armed window in 34 attackers windows, 3
blocker windows in six games, and zero at forecast ≤ 9** — and the pool did not change in a
direction that fixes it (139, the pool's only source of large armed boards opposite 152, is the
deck that just left). Spending a third corpus to re-confirm a structural starvation we can already
name is the "unfalsified is not validated" trap with a wave attached to it.

**The wave-41 152 probe (198/199), constructed to arm both predictions deterministically:**
- **Edit-1 arming**: opponent stacked with one large body (a 6/6-class ground creature) and no
  chaff; seat board reduced to one creature, seat life pushed to at or below that body's power.
  That is the armed-**sole**-attacker window, which the corpus has produced **zero** of in two
  waves.
- **Edit-2 low-N arming**: a wide board of small attackers against a low seat life, to force a
  blocker window at forecast ≤ 9 with a rung-3 line offered.
The corpus still reports both counts as a bonus; the probe is the instrument of record.
**If the probe discharges both edits, 152 is EXIT-eligible in wave 42.**

### 4.3 The other holds, in one line each

- **146** — the gate's blocker is cleared (named requirement discharged, retire recommendation
  **refuted**, line KEPT), but the chump anchor moved **backwards**, which is exactly the "signal
  for additional work" the exit rule is about. Its wave-41 review reports the ladder's three rungs
  separately: rung-1 lines offered/taken, rung-2 lines offered/taken, and rung-3 take-rate split
  above and below forecast 10.
- **125 / 126 / 162** — all three are Step-0 seats whose validation corpus is complete, and per
  the Step-0 pipeline they would freeze and become EXIT-eligible **if the guides needed no further
  mods**. **All three need mods** (9, 3 and 4 respectively), so all three hold for one revised
  corpus. 162 additionally cannot exit before wave 42 on its own crutch clock (corpus 1 of 2).
- **123** — Step-0 entrant, initial guide authored, owes one validation corpus. Its two
  null-carrying teaches have named arming requirements already recorded (§9 of its findings) and
  the next reviewer reports the Lightmine throttle's arming count **from the deck125 matchups
  specifically**.

### 4.4 ENTRANT — deck130 "Budde's Ponza" (mono-R land destruction), Step-0 GUIDELESS

Verified: 60 cards, **no `deck130_strategy.txt` exists**, never seated, not a canary, not on the
construction-flag list.

**Why 130 over the other two named runners-up.** All three were verified at 60 cards with no
guide (56 Enchanted Coatl, 121 Reckoner's Chain, 130 Budde's Ponza), so the choice is made on the
standing criteria — LLM value = (deck power ceiling) − (what Baka already extracts), plus
archetype diversity against **the wave-41 pool and the canary lanes**.

- **deck56 "Enchanted Coatl" — DEFERRED, same reason as wave 39, still live.** Its value is
  cast-ORDER sequencing (enchantress before enchantments), which is engine-management — the lane
  **162 still occupies** — and deck62 (Enchantresses) is a canary with a deployed guide on the
  same core mechanic. Two overlaps, one of them with a canary. Re-evaluate when 162 exits.
- **deck121 "Reckoner's Chain" — DEFERRED, and it is the wave-42 FRONT-RUNNER.** Blasphemous Act
  into your own Boros Reckoner / Stuffy Doll is the richest inversion of the three and the closest
  thing on the roster to the project's own "make card X win" thesis — Baka will read a self-sweep
  as self-harm and never fire it. It is deferred only because its **wall-stall shell mirrors
  126's seat shape** and 126 is still seated. It becomes free the moment 126 exits.
- **deck130 "Budde's Ponza" — ENTERS.** Three reasons, in order.
  1. **Zero archetype collision.** The wave-41 pool is four combo/control/prison shells deep
     (125 draw-go control, 126 tutor-combo, 162 prison-engine, 123 token-combo) plus two midrange
     (146, 152). Nothing in it, and nothing in the canary set, is proactive resource denial.
  2. **Land destruction is a wholly uncovered mechanic**, and Baka's LD targeting is naive in a
     way the corpus can measure cleanly: it cannot choose WHICH land to kill off the rendered
     battlefield line, cannot sequence LD against its own curve, and has no concept of when LD
     stops being the right spend. That is a per-decision, board-readable decision class — the
     shape this campaign extracts value from fastest.
  3. **Its "moderate ceiling" (the wave-39 caveat) is an ADVANTAGE for a Step-0 debut this wave.**
     Three of the six holding seats are running revised guides whose deltas must be attributable;
     a linear red shell with one novel decision class is a cleaner Step-0 signal than a second
     combo deck would be, and it will not compete with 126/123 for the combo-assembly lane.
  - Canary check: **109 mono-R Aggro is a canary, and Ponza is not that deck** — its core is LD,
    not burn, which is the criterion the standing rule states ("in its CORE mechanic").

**Wave-41 pool: 146, 152, 125, 126, 162, 123, 130.** Pool size held at 7.

---

## 5. RUNGS EARNED THIS WAVE (folded into `wave40/strategy-writing-skill.md`)

- **M1 — The coverage-fraction method itself is now a rung**: every denominator found
  INDEPENDENTLY of the fixed string, narration counted over the per-record `events` DELTA stream
  and never over `prompt` (raw-greps over `prompt` overcount by 5-25×: `resolved and went to`
  raw-greps 7,731 times against a true delta count a fraction of that), both POVs of every game
  counted, and **the fraction reported with what its denominator did not contain**.
- **M2 — THE OBSERVER-SCOPE BLINDNESS CLASS.** Before writing any opponent-modeling teach, check
  what the OPPONENT seat can SEE. 165 actor-side activation lines, 0 observer-side.
- **M3 — CROSS-SEAT CONTRADICTION IS A SYNTHESIS-LANE DEFECT** that no single seat can detect;
  the repair is one shared render-keyed ladder with a per-deck calibration constant.
- **M4 — FALSE BELIEF AT A COST TYPE**: a `{T}`-only ability answered with a mana objection.
- **M5 — BELIEF-VS-DECISION VIA REASONING-LENGTH SIGNATURE**, sharpened: rank ALL traces by
  `reasoning_chars` and read the LOW-BRANCHING rows first.
- **M6 — THE PROBE-DECK DISCHARGE PATTERN for structurally unmeasurable predictions**, with the
  measured-twice trigger.
- **M7 — BANNING A STRING BANS THE STRING, NOT THE BELIEF** (the crutch-wording lesson).
- **M8 — A REMOVAL CONDITION MUST MATCH ITS CRUTCH CLASS**: information-supplying crutches retire
  on one corpus; belief-correcting crutches need two with the belief-triggering shape present.
- **M9 — THE PRE-SHIP FREQUENCY GREP MUST BE THE COMPLETE LIST** (deck123's §8 reported "zero
  hits" from an incomplete pattern; `expect` was omitted and it hits).

Full text, with sources and the adopted/adapted/rejected changelog, is in the skill's wave-40
METHOD HEADLINES section.

---

## 6. deck123 DRAFT GUIDE — VETTING AGAINST THE SKILL RUNGS (violations + exact fixes; NOT edited here)

Vetted file: `wave40/deck123/deck123_strategy.txt` (157 lines). **It is NOT deployed** — there is
no `deck123_strategy.txt` in `bin/Res/ai/baka/`, verified. The Step-0 author's own §7 card-fact
ledger (33 rows, adjudicated against Scryfall AND `primitives/mtg.txt`) and §8 frequency audit are
strong work and I re-ran the checkable parts rather than trusting them. **Eleven violations, in
severity order. The seat applies these; the synthesis seat does not edit a guide it did not
author.**

**V1 — [BLOCKING] The pre-ship frequency grep was run with an INCOMPLETE pattern and reported a
false clean.** §8 claims *"Zero hits"* for `rarely|often|usually|most of the time|typically|
seldom|always happens|almost never`. The mandatory list also contains **`expect`**, and it hits:
> L22: `... That is Intruder Alarm working: the token you just made untapped the maker again.`
> **`Expect the line to come back`**`, and take it.`
This is a definition-of-done failure, not a taste call — a guide cannot ship on an audit that did
not run. **Fix**: re-run the complete pattern, and change L22's tail to
`When the line comes back, take it again.` (the mechanism sentence immediately before it already
carries the causal claim, so nothing is lost).

**V2 — [HIGH] Two cards both claim rank 1 in the cast order — the deck162 W41-RULE12 collision,
shipped before it can bite.** Intruder Alarm is *"ahead of everything else you could cast that
window"* (L88-89, restated L145) and Bloodline Keeper is *"ahead of anything else you could cast in
the same window"* (L105, restated L148). Thraben Doomsayer is *"Cast it the turn it is listed"*
(L101). **Three unconditional priorities, no precedence between them**, in a deck whose whole
plan is a two-card assembly — this is exactly the shape that cost deck162 a game at `139v162` s15
and deck125 a game at `125v162` s17. **Fix**: install one ordered cast list (the deck126 rule-#7
shape) and delete every per-card "ahead of everything" claim, each card section pointing at the
list instead:
```
=== WHAT TO CAST WHEN SEVERAL THINGS ARE LISTED ===
Take the highest entry on this list that appears in your casting menu:
 1. "Cast Intruder Alarm {2}{u}" - when no Intruder Alarm is on your battlefield.
 2. "Cast Bloodline Keeper {2}{b}{b}".
 3. "Cast Thraben Doomsayer {1}{w}{w}".
 4. "Cast Idyllic Tutor {2}{w}" - when no Intruder Alarm is on your battlefield or in your hand.
 5. A removal spell, by Rules 3, 4 and 5.
 6. "Cast Vision Skeins {1}{u}".
```

**V3 — [HIGH] Rule 5's Tribute clause contradicts Rule 5's own closing clause.** L61-62 says cast
Tribute *"at the opponent whenever it is listed and they have a creature"* — unconditional on
creature COUNT. L67-69 then says *"With two or more of their creatures on the board, take Tragic
Slip, Damnation or a token activation instead."* The guide's own reasoning (the sacrificing player
hands over their worst) makes the first clause wrong at 2+ creatures. **Fix** — replace L61-62:
```
Cast it at the opponent whenever it is listed and their battlefield line shows exactly ONE
creature. With two or more of their creatures listed, they hand over their worst one and keep the
good one - take Tragic Slip, Damnation or a token activation instead.
```

**V4 — [HIGH] PRESERVE-LEDGER VIOLATION: the guide legislates over the FROZEN blocker seam, and
does it with the exact preservation clause that just regressed deck146.** §1 records
*"Blocker declarations — a block was declared at every blockers window offered (5 windows) —
FROZEN"*, and the guide then adds L123-125: *"Keep Thraben Doomsayer and Bloodline Keeper back out
of blocks unless the block line says your blocker lives."* That is a new restriction on a working
seam (the wave-31 rung-0 failure the seat correctly avoided at the ATTACKER seam two sections
earlier), and it is the same *"preserve the good creature"* shape that produced 146's two chump
violations and 152's declined free kill this very corpus. **Fix**: delete L124-125 and keep only
the positive:
```
Blocking: block with your Human and Vampire TOKENS first - they are free and you make more every
turn.
```

**V5 — [MEDIUM] PRESERVE-LEDGER VIOLATION #2: the mulligan section is 10 lines on a seam §1 froze
and authorised THREE.** §1: *"Mulligan — 6/6 `Keep this hand` at seq1; zero mulligans, zero
spirals — FROZEN — guide carries a 3-line confirmatory section only."* Shipped: L127-136, ten
lines, including a falsity fact and a tiebreaker paragraph. It also duplicates its own terminator
in two currencies (*"Never keep fewer than 5 cards"* and *"At most two mulligans"*). **Fix**: cut
to the authorised three — the count-read (L128-129), the numeric floor (one of the two, keep
*"Never keep fewer than 5 cards"*), and the satisfiability line (L135-136 *"Your cheapest card
costs ONE mana…"*). Delete the tiebreakers (L133-135) and the falsity fact (L131-132); neither is
adjudicable on a seam with zero mulligans in six games.

**V6 — [MEDIUM] Rule 3's Damnation test has no branch at EQUAL creature counts.** L42: *"Cast
Damnation when THEIR number is bigger than yours."* L44: *"When YOUR number is the bigger one …
keep the Damnation."* **2-vs-2 is unanswered**, and both numbers are printed on screen, so the
pilot will reach for something outside the rule at exactly the window the rule exists for.
**Fix** — append to L42:
```
Equal numbers are not a reason to cast it: keep the Damnation and take another action.
```

**V7 — [MEDIUM] The COMBAT throttle asks the pilot to SOLVE an inequality over a quantity it must
invent, instead of reading a printed number.** L121-122: *"send the largest number of attackers N
such that N is still less than your attackers' toughness"* — but toughness varies across the
board it is describing (4/4 Vampire tokens, a 5/5 Lord, 3/3 fateful-hour Humans coexist), so "your
attackers' toughness" has no single value. The observed CORRECT play (§1, `vs125` seq58) was 3 of
12 with 4/4s. **Fix** — make it a board read:
```
Count it off the battlefield line: find the SMALLEST toughness among the creatures you would send,
and send that many minus one, biggest power first. Leave the rest home.
```
(4/4s → send 3, reproducing the corpus's own correct answer.)

**V8 — [MEDIUM] Rule 4 uses two different words for the same target choice.** L52 says *"take the
one with the biggest power"*; L54-55's morbid branch says *"take their BIGGEST creature"* —
undefined against the render (power? toughness?). **Fix** — L55:
`then take the target with the biggest printed power on the battlefield line.`

**V9 — [MEDIUM] HL6 CROSS-CHECK IS FALSE AS WRITTEN: the guide quotes a string its own ledger
schedules for change.** §7 states *"the guide quotes none of those three as anchors"* for
L-123b/c/d. It does: **L74-75 and L156 quote `"Equip with Lightning Greaves targeting"`**, and
**L-123d's ask is to annotate that option line with its current holder, or to suppress the
re-offer entirely.** If L-123d ships as suppression the quoted trigger stops appearing and Rule 6's
re-offer machinery becomes dead text. This is precisely the wave-39 HL6 rung (a quoted string is a
DEPENDENCY on that string; presence in the corpus is necessary and not sufficient). **Fix, both
halves**: (a) add a row to the §6 stopgap register recording the L74/L156 quoted-string dependency
with removal condition *"the option line names the current holder, or the re-offer is
suppressed"*; (b) soften the quotation so the rule survives either fix shape — L74:
`An equip line for Lightning Greaves costs {0}, so the game re-offers it at every window`.

**V10 — [LOW] Rule 1's terminator is correctly present and correctly registered — but it is
stated in two places and only one of them is registered.** L14-15 and L141-142 both carry
*"12 or more creatures"*; §6 row 1's removal condition names the Rule-1 copy only. **Fix**: name
both line locations in the register row, so the removal deletes both copies. (A half-removed
terminator is the failure mode the register exists to prevent, and #W41-6 makes this the guide
sentence currently holding invariant 00.)

**V11 — [LOW, note not a fix] Rule 2's ordered fetch list is the strongest single rule in the
draft — do not let the edits above dilute it.** It is unconditional, countable, board-readable,
and it names the exact strings (`Put in Play with Marsh Flats targeting Underground Sea` — 11
corpus occurrences, verified). Its "why this decides games" sentence is grounded in a measured
consequence (2 Intruder Alarms stranded t9→t26 with zero blue sources on the battlefield). Keep it
byte-identical.

**What the draft gets right and should be preserved verbatim through the revision**: the §1
preserve ledger enumerated BEFORE drafting; the `[cost: Tap]` falsity facts (L18-22) aimed at the
measured belief rather than at the decision; the registered stopgap for the morbid log-tail read
rather than a teach that the render is wrong; the per-claim card-fact ledger; and the refusal to
legislate over the partial attack. **Nine of the eleven violations above are placement,
precedence and audit-completeness defects, not content defects** — the guide's facts are sound and
its #1 slot is correctly chosen and auditably argued (§10).
