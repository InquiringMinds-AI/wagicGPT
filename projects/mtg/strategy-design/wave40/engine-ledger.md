# Wave-40 consolidated engine ledger — the wave-41 step-1 docket

Sources, all seven: `wave40/fix-validation.md` (cross-cutting completeness critic),
`wave40/seat125-validation.md`, `wave40/seat126-validation.md`, `wave40/seat162-validation.md`,
`wave40/seat152-146-validation.md`, `wave40/deck123/findings.md` (Step-0), and the carried-open
items of `wave39/engine-ledger.md` (#7, #19/#19-AMENDED, #20, #21, #22, #23).

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260823-110407`, binary **465422fcf**, pilot
`qwen36-35b-a3b` (FP8 35B, standing serve :8084 — **NEVER cycle, owner ruling**), 21 games,
**2,073 decisions, 7 fallbacks (0.34%)**, **21/21 NATURAL — the first fully complete corpus on
this pool.** Suite baseline 1057/0 + 30/0, PARSETEST 671/0.

Ranked by (game cost observed) × (mechanism located) × (repro quality). **Severity and repro
quality stay SEPARATE fields** (wave-31 HL4).

---

## Cross-cutting directives for the wave-41 fix agents

- **THE OWNER RULING ON ZERO-SLACK X IS BINDING AND IS NOT ADJUDICABLE** (2026-08-23, verbatim):
  *"casting a spell, with x as zero is legal. not only that, there obviously times when a player
  should do so."* No lane may filter an `{X}` spell out of a cast menu, price it out at menu-build
  time, or otherwise remove the offer. **Item #W41-1 is the same ruling read forward**: the pilot
  must be ASKED for X on every `{X}` cast it commits. Removing the question is the same wrong as
  removing the option.
- **THE OWNER'S NARRATION FIX SHAPE IS BINDING** (2026-08-23, verbatim, on
  `- Opponent put a card into their library`): *"this should indicate where from, and also, I
  believe it's a revealed card(from the graveyard) so it should name the card."* Every zone-change
  narration names the ORIGIN zone; when the origin is a PUBLIC zone (battlefield, graveyard,
  stack, exile) the card is NAMED. Masking is legitimate only for hidden origins (hand/library),
  and even then the origin is stated.
- **THE OWNER'S `may` RULING IS BINDING** (2026-08-23, verbatim): *"may should be rules faithful.
  7 blockers are quite rare anyways."* #12 shipped Oracle-faithful and stays that way. **#W41-7
  (mass accept/deny) is a BATCHING affordance, never a re-introduction of the mandatory form** —
  no lane may remove the `may`.
- **A pointer is not a diagnosis.** Items carrying a seat's code pointer carry it flagged as
  unverified; the lane starts from the core/stderr, not from the pointer.
- **Every render fix reports a COVERAGE FRACTION** on the wave-41 corpus (emissions-with-the-new-
  string / occasions-total, denominator found INDEPENDENTLY of the fixed string), never "it
  renders". `wave40/fix-validation.md`'s method section is the template — narration is counted
  over the per-record `events` DELTA stream, never over `prompt` (the prompt carries the
  cumulative GAME LOG and raw-greps overcount by 5-25x).
- **Verify every card fact before acting on a reviewer's claim** (campaign standing rule).
- **Two items are discharged by PROBE DECK this wave, not by seat** (#W41-4 mutate, and seat 152's
  two un-adjudicated edits). Probe decks go in the numbering gap at **198/199** — the real roster
  ends at deck164, verified — and are DELETED from `bin/Res/ai/baka/` after the run, with
  decklists + translogs + report preserved under `strategy-design/wave41/probe-<topic>/`. Probe
  one-shots run SEQUENTIALLY, never concurrent.

---

# CLOSED THIS WAVE (with evidence cites — no wave-41 lane; do not re-open)

| # | id | verdict | evidence |
|---|---|---|---|
| **#3** | W39-COUNTERED | **CLOSED** | `fix-validation.md` #3a **40/40 = 100%** of counterspell resolutions carry `was COUNTERED by <spell>`; #3b/#3c KILL metrics **0 leaks** (0 `resolved and went to` on a countered spell; 0 of 40 pool permanents narrated as resolving to graveyard). Attribution shipped on BOTH sides (seat125 §6, seat126 §6, seat162 §3-bonus, deck123 §5: `was COUNTERED` in 90/285 prompts, `resolved and went to the graveyard` in **0**). Pilot CONSUMED it: `126v125` s72 PLAN reads *"Staff of Nin was countered by Dream Fracture, removing the intended win trigger."* **Caveat carried to #W41-9**: the register is unvalidated against a MODAL counter (Silverquill Command, 8 resolutions, 0 counter modes chosen). |
| **#4** | W39-STACKFACTS | **CLOSED** | `fix-validation.md` #4a **108/109 = 99.1%** stack SPELL entries carry cost + type + (P/T); #4b **39/39 = 100%** of `can target on the stack:` clauses. Payoff metric the item asked for: counters-spent-on-non-threats **9/25 → 2/21** (seat125 §1.3, all 21 enumerated). The single #4a miss is **#W41-11** (a daybound Day marker, not a card). |
| **#8** | W39-DOUBLEASK | **CLOSED** | `fix-validation.md` #8: 697 cast-decision asks, 12 consecutive-seq identical-menu pairs examined, **0 fit the defect** (baseline 4 pairs/game on X-decks). Mechanically re-derived, not string-matched (seat125 §3): in all 11 substantive pairs mana DROPPED, the cast spell was GONE from B's list, option sets strictly differed. Residual: 1 harmless duplicate cast-nothing ask (`162` s17/s18), logged as **#W41-14**. |
| **#9** | W39-BATTLEMENT | **CLOSED** | `fix-validation.md` #9 **57/57 = 100%** of `Add N green mana with Overgrown Battlement` labels have N == the defender count on the controller's battlefield at emission (forms 1-7 observed; ramp tracked monotonically across three games). Guide anchor unbroken — seat126 §1 confirms rule #1 keys on the PHASE, not the label, so the sequencing hazard (V1) discharged clean. |
| **#10** | W39-DRAWS0 | **CLOSED-BY-SUPPRESSION** | `fix-validation.md` #10: **0** occurrences corpus-wide in prompt AND narration. seat162 §3 confirms every Dictate / Puzzle Box / Howling Mine / Forced Fruition option line now renders with **no `{right now: ...}` clause at all**. ⚠ The fix took the *suppress* branch; the ask's second half (`{each draw step: draws 1 each}`, name-the-branch) is **unimplemented** — recorded, not re-docketed, but see **#W41-12** where the same suppression is now owed on two other cards. |
| **#11** | W39-TRIBUTE | **CLOSED** | seat126 §4: option line now renders `"Target opponent sacrifices a creature of their choice…"`; PLAN lines naming a fabricated victim **3 → 0** across 116 offered windows; the one creature-naming PLAN (`126v125` s97/s98) is the guide's sanctioned single-creature case and **won the game** (20/50 → 80/-10). 12k-char reasoning blowups on the card **3 → 0**. Guide stopgap keeps **removal condition: none** (the belief survives the `text=` change). |
| **#12** | W39-CAPTAIN | **CLOSED behaviourally** | seat126 §3: `primitives/mtg.txt:85440` now Oracle-faithful `…:may life:2 controller`. **22 may-asks, 22/22 accepted, 0 declines, 0 fallbacks**, gains ARRIVAL-TRACED per attempt — `126v139` t24 renders 8 asks → 8 `You chose Life` echoes → 8 × `+2 life`, **61 → 77**, all before combat damage. No prompt without a matching gain; no gain without a prompt. |
| **#13** | W39-PEER | **CLOSED** | seat162 §3: `{right now: if you choose "target opponent": life -12, draws 23; if you choose "target controller": life -10, draws 21}` in 3/3 payable windows; the flattened form has **0** occurrences. End-to-end verified: s22 cast → s23 menu answered `target opponent` → game ended 19–0 that turn. |
| **#16** | results.tsv bogus winner | **CLOSED** | `fix-validation.md` #16: 21 rows, **21 natural**, every loser at ≤0 life, no `adj`/crash row. seat125 §4.1 cross-checks all six deck125 rows against paired `gameend` records — all agree. |
| **#1** | W39-CRASH-AFFINITY | **CLOSED (carried from the wave-40 gate)** | The completeness invariant is DISCHARGED on the exact matchup that broke: **139v125 ended naturally t26**, 139 won 18-0, paired `gameend` records present. 21/21 natural corpus-wide. The cardsAbilities weak-registry fix held. |
| **#18** | wave-38 lanes | already closed wave-39 | no action. |
| **#22** | suite may-menu wedge — **DEBT HALF ONLY** | seat126 §3: the arrival trace #22 owed (a rendered may-ask for the GPT seat + accepted gains landing) is **DISCHARGED**. The suite-DRIVER wedge itself is untouched and remains open as **#W41-15**. |
| **#2** | W39-D1 desirability veto | **METRICS VALIDATED — STAYS OPEN AS FIXTURE-OWED** | see #W41-2 below. |

---

# Tier 1 — the wave-41 HIGH docket

## #W41-1 — [HIGH] An `{X}` cast commits with NO X-announcement window; X is set below the affordable maximum by the payment path

- **id**: W41-XCOMMIT (`seat125-validation.md` §5.1). **This item decided a game.**
- **Mechanism (half-located, do not guess the rest)**: 14 Sphinx's Revelation casts across six
  games; **only 10 raised `Announce the value of X`**. The four that did not are all at high mana
  and all resolved far below what was affordable. The translog carries **no ANNOUNCE_X record**
  for any of the four — the pilot was never asked.

  | game | seq | `Mana available:` | X actually announced | X affordable |
  |---|---|---|---|---|
  | deck123 | s56 (t31) | 16 | **3** | 13 |
  | deck123 | s66 (t33) | 18 | **9** | 15 |
  | deck123 | **s71 (t35)** | 18 | **3** | 15 |
  | deck126 | s59 (t40) | 14 | (no menu at all; mana 14 → 0) | 11 |

- **Repro quality: EXCELLENT — deterministic, one binary, with a WITH-MENU CONTROL 100 lines
  earlier in the same stderr file** (`game-125v123-1787501085.stderr`):
  ```
  (WITH menu, line 2540)                 (WITHOUT menu, line 2664)
  AIPlayerGPT: casting Sphinx's ...      AIPlayerGPT: casting Sphinx's ...
  AIPlayerBaka: ... using potential        AIPlayerBaka: ... using potential
    Mana now.                                Mana now.
  in GetCost Seems ManaCost was not      ACTIONSTACK Add spell
    properly initialized                 Action added to stack
  AIPlayerGPT: Announce the value of
    X ... -> chose 1 of 14
  ACTIONSTACK Add spell
  ```
  The correlation is with high mana counts and with the `could be Paid with potential mana`
  payment branch. **Not root-caused — the lane starts at the stderr divergence, not here.**
- **Game cost, traced**: `125v123` **s71** is the deck-out cast of seat125 §4.2 — deck125 was at
  **70 life vs 7** with two Staff of Nin pinging for a 4-turn unopposed kill, its own prompt read
  **`Your library: 1 cards`**, and the un-asked Revelation drew 3 from a one-card library. Had the
  pilot been asked for X it would have had the library count and an X menu on the same screen.
- **Why it is HIGH, and why it is the docket's #1**: this is the **silent-discard class of #2 on a
  different seam** — a decision taken away from the model without telling it. The owner's X ruling
  says X=0 is a legal choice a player should sometimes make; a path that CHOOSES X for the pilot
  violates that ruling as surely as filtering the menu would.
- **Ask**: the X window must arm on every `{X}` cast the GPT seat commits. If a payment path is
  structurally unable to raise it, the engine must LOG which X it chose and why, and the narration
  must state it — silence is the defect.
- **Validated next by**: seat 125 (the pool's only `{X}` deck) + the #W41-2 fixture.
  **Metric**: `ANNOUNCE_X` records / `{X}` casts committed by the GPT seat == 1.00; zero casts
  resolving below `maxAnnounceableX` without a record.

## #W41-2 — [HIGH, FIXTURE-OWED] The desirability veto — metrics clean, positive instrument still missing

- **id**: W39-D1, carried. **Do not close on this corpus.**
- **VALIDATED on the two falsifiable metrics** (`fix-validation.md`; seat125 §2):
  `validation_reject_reask_exhausted` **5 → 0** corpus-wide (all 64 seat files, 1,989 decisions);
  `defer` records of any kind **0**; `latency_ms = -1` records on any seam at deck125 **0** across
  267 decisions. The replay loop and the veto are both absent from the evidence.
- **The X=0 null is an HONEST NULL, and it is the guide's doing, not the engine's.** Zero-slack
  windows DID arise — Sphinx's Revelation offered at exactly `Mana available: 3` twelve times
  (`152` s11/s12/s15/s16/s38/s39/s40, `126` s19, + four 4-mana near-misses) — and the pilot
  declined every one, because the shipped guide casts Revelation only at 6+ mana. **The seat can
  no longer reach the old repro by play.**
- **What is still owed** (owner-directed shape, unchanged): a regression FIXTURE — an `{X}` spell
  with exactly its coloured pips available and **no slack**; the GPT seat must announce and commit
  **X = 0**, and the translog must show **ONE** record **with a model call**. This is the only
  instrument that can close #2 positively.
- **Same lane as #W41-1** — both live on the cast-commit/X-announce path; the fixture that proves
  #2 will exercise #W41-1's missing window.

## #W41-3 — [HIGH, TRUST DOCTRINE, OWNER-RULED FIX SHAPE] THE NARRATION CHANNEL — one lane

- **id**: W41-NARRCHANNEL. **Merges #19, #19-AMENDED, #23 and F2/F3 into ONE lane** — they are the
  same emitter family and splitting them guarantees a seam defect.
- **The reframing that makes this bigger than wave-39 thought it was** (`fix-validation.md` F2):
  **the mask is OBSERVER-scoped, not origin-scoped.** The origin logic is already right on the
  ACTOR's own seat and wrong only on the OBSERVER's. Same game, same events, two seat files:

  | seat `…deck125-…-vs-deck123` (actor) | seat `…deck123-…-vs-deck125` (observer) |
  |---|---|
  | `- You used: Life with Elixir of Immortality` | *(no line)* |
  | `- You gained 5 life (now 25)` | `- Opponent gained 5 life (now 25)` |
  | `- Your Elixir of Immortality was put into your library` | `- Opponent put a card into their library` |
  | `- Your Dream Fracture moved from your graveyard to your library` (×N, each named) | `- Opponent put a card into their library` (×N) |

- **THE STRUCTURAL NUMBER TO FIX AGAINST IS 165/0, NOT 47.** `- You used: …` appears **165 times**
  corpus-wide; `- Opponent used: …` appears **0 times**. Every activated ability and every
  planeswalker loyalty ability is INVISIBLE on the opposing seat — the observer sees only the
  effect (`- Opponent's Kaya the Inexorable got a loyalty counter` ×50;
  `- Opponent's Lolth, Spider Queen …` ×57) with **no cause line**. Verified side-by-side:
  `…deck146-…` seq 30 `- You used: +1: don't target any creature with Kaya the Inexorable` vs its
  pair `…deck126-…` seq 25 `- Opponent's Kaya the Inexorable got a loyalty counter`.
- **The four sub-fixes, all in this lane**:
  1. **(a) OPPONENT-SIDE ACTIVATION/LOYALTY NARRATION.** `- Opponent used: <ability> with <Card>`
     — the whole missing channel. Baseline **165 / 0**.
  2. **(b) PUBLIC-ORIGIN NAMING + ORIGIN ZONES** (owner ruling, binding). Every zone-change line
     names the ORIGIN zone; a PUBLIC origin (battlefield, graveyard, stack, exile) NAMES the card.
     Hidden origins (hand/library) stay masked but STILL state the origin ("from their hand").
     Baseline: **47** masked `put a card into their library` lines + 1 `…into their hand`
     (a public graveyard→hand move), 6 seat files / 6 games.
  3. **(c) BULK-SHUFFLE COLLAPSE.** One line with count + source:
     `Opponent shuffled their graveyard (N cards) into their library with Elixir of Immortality`,
     replacing N identical anonymous lines.
  4. **(d) #23 — EFFECT-AS-SOURCE ATTRIBUTION** (owner-spotted in the wave-40 specimen, 125v126
     t48). Symptom: `You targeted Swamp - "B" with Put in Play`. **Owner ruling, verbatim:**
     *"'put in play' is not good. this should be the effect source, not the effect."* Mechanism
     pointer (flagged unverified): the emitter at `AIPlayerGPT.cpp:2418-2425` renders
     `You targeted <X> with <src>` / `with <src>'s <ability> ability`, and on this path the `src`
     slot received the ABILITY's display name ("Put in Play", `AllAbilities.cpp:6958/7015`,
     the AAMover/put-in-play effect) instead of the owning card's name. Target shape:
     `You targeted <X> with <Card>'s Put in Play ability`. **SWEEP THE CLASS** — audit every
     caller of the 2418 narration for what it passes as `src`; any other
     effect-name-as-source instance dies in the same fix.
  5. **(e) F3 — 25 no-op `moved from the opponent's zone to the opponent's zone` lines.** All 25
     are `Lost Mine of Phandelver` dungeon venturing (`…deck125-…` seqs 7/15/18/26). A zone change
     that begins and ends in the same zone is narrated as a move, next to the correct
     `- Opponent ventured into Lost Mine of Phandelver: venture step 1 of that run` line that
     already says the real thing. Cheap suppression: skip narration when origin == destination.
- **PARSETEST**: existing W35 cases pin the correct shapes; add a regression per sub-fix (a
  positive, a negative that must NOT match, and the echo shape of every new bracketed annotation).
- **Validated next by**: every seat — this is corpus-wide. Owning seats: 125 (Elixir loop),
  126 (Sorin/tokens/Kaya), 146 (dungeon + Kaya). **Metrics**: zero cause-less effect lines on the
  observing seat (`Opponent used:` count ≥ the actor-side `You used:` count for the paired game);
  zero anonymous public-origin moves; shuffle collapsed with count + source named; zero narration
  lines whose `src` slot names an effect; zero same-zone move lines.

## #W41-4 — [HIGH — PROMOTED from MEDIUM] A mutated pile renders the WRONG card's rules text — 70% wrong, and now on BOTH boards

- **id**: W39-MUTATEPILE, carried and **quantified**. Promoted because the defect crossed from
  one seat's own board to the opposing seat's targeting surface.
- **Mechanism**: board and target option lines render a merged pile as
  `<top card name> (P/T) [combined keywords] - "<some OTHER card in the pile's rules text>"`.
  The `[mutated pile]` annotation truthfully promises *"the combined abilities of every card in
  the pile"*, then the quoted text shows ONE card's — and not the named one. A true statement in
  the wrong scope.
- **Measured** (`fix-validation.md` #7 / F5): **14 of 20** mutate-pile battlefield option lines
  quote the WRONG card's text = **70% wrong**.
- **Fresh repros beyond the wave-39 two**:
  - `Everquill Phoenix (4/4) [flying, mutate] [your battlefield] - "You may play an additional
    land…"` (that is **Dryad of the Ilysian Grove's** text)
  - `Gemrazer #1 (4/4) [trample, reach, mutate] - "Each creature spell you cast costs {1} less…"`
    (**Pollywog Symbiote's**)
  - `Snapdax, Apex of the Hunt (3/5) [double strike, reach, mutate] - "Reach -- When Arboreal
    Grazer enters…"` (**Arboreal Grazer's**)
  - **BOTH-BOARDS proof**: `…deck152-0x557fe9f525a0…` **seq 35** renders two of deck139's piles
    with swapped texts — it now mis-informs the OPPONENT's targeting decisions too.
- **Scope note**: battlefield-SUMMARY lines are clean (they quote no text); the defect is confined
  to option/target lines.
- **Ask**: render the pile's text as the concatenation the annotation already promises, or
  attribute it per card (`"<name>: …"`).
- **⚠ VALIDATION RE-KEYED — deck139 EXITS the pool this wave.** This item is discharged by a
  **MUTATE PROBE DECK** (198/199) stacked with the mutate bodies + hosts above, one pinned GPT
  one-shot, then a pure render grep over the option lines. **Metric**: coverage fraction of
  `[mutated pile]` option lines whose quoted text names or concatenates every card in the pile —
  target 20/20; baseline 6/20.

## #W41-5 — [HIGH, TRUST DOCTRINE / ANNOTATION TRUTH] The block-outcome predictor ignores damage-prevention replacement effects

- **id**: W41-PREVENT-BLOCKOUTCOME (`seat162-validation.md` §2d + item 5).
- **Mechanism**: the block-outcome parenthetical compares attacker power to blocker toughness (and
  applies deathtouch) **without checking prevention**. Fog Bank
  (`mtg.txt`: `auto=preventAllCombatDamage to(this)` + `from(this)`) is rendered
  `(your blocker dies, attacker lives)` against attackers that cannot damage it at all.
- **Repro 1 — the engine SELF-REFUTES in the same game**: `146v162` **s5** —
  `B1. Fog Bank (0/2) … may block A1 (your blocker dies, attacker lives)`, A1 = Triumphant
  Adventurer 1/1 first strike + deathtouch. The pilot blocked anyway; **the narration shows Fog
  Bank SURVIVED** and was lost two events later to Vanishing Verse, an unrelated exile. The
  engine and its own annotation disagree inside one game. (702.2b: deathtouch needs damage dealt;
  prevented damage is never dealt.)
- **Repro 2 — cost measured**: `139v162` **s14** — Fog Bank's line read
  `may block A1 (your blocker dies, attacker lives), A2 (neither dies), A3 (neither dies)`; A1 is
  **Migratory Greathorn (3/4, reach, mutate, NO deathtouch)**, which cannot damage Fog Bank. The
  pilot blocked A2 (a 1-damage Pollywog) instead of the 3-damage Greathorn and **took 4 instead
  of 2**.
- **Why HIGH**: (1) it is the same LINE and the same class as #5/W39-WALLBLOCK, one wave after
  that shipped at 89/89; (2) it **inverts a shipped guide rule** — deck162's Rule 4 item 3 is
  correct against a truthful render and harmful against this one; (3) per the trust doctrine the
  guide MUST NOT hedge against a render, so there is no guide-side mitigation available and **no
  guide edit is proposed** — the surface is the only lane.
- **Ask**: apply prevention/protection replacement effects before computing blocker death. Fog
  Bank vs any non-prevention-piercing attacker must read `(neither dies)`.
- **Validated next by**: seat 162 (its entire defence is two prevention walls); any Fog Bank /
  Guard Gomazoa / prevention-wall seat. **Metric**: zero `(your blocker dies` on a blocker whose
  `auto=` contains `preventAllCombatDamage to(this)`.

## #W41-6 — [HIGH, COMPLETENESS HAZARD vs INVARIANT 00] The token-maker loop has no terminator and no cap

- **id**: L-123a (`wave40/deck123/findings.md` §5).
- **Mechanism**: with Intruder Alarm (`auto=@movedTo(creature|myBattlefield):untap all(creature)`)
  plus any `{T}`-token-maker on the battlefield, each token entering untaps the maker, so the
  engine re-offers `Create <token> with <maker> [cost: Tap]` **indefinitely**. Nothing in the
  engine bounds the loop. Repro: `vs125` **seq25-33** — 8 consecutive activations, option still
  present at seq34.
- **Why it is HIGH despite costing no game yet**: it **collides directly with invariant 00** (a
  corpus game must finish). A pilot that obeys the correct instruction — "a printed option is
  takeable now, take it" — never passes priority, and the game cannot complete. The only thing
  standing between this corpus and a non-completing game is a **guide sentence**, currently
  carried as a registered stopgap (deck123 §6 row 1: *"until your own battlefield line shows 12 or
  more creatures, then pass"*). An invariant that depends on prose is not held.
- **⚠ THIS IS NOT A REQUEST TO SUPPRESS A LEGAL PLAY** (never-suppress doctrine, owner ruling,
  extended to the offer side in wave-39 HL3). The ask is a **BOUND THE PILOT CAN COUNT**, not a
  removal: an activation-count annotation on the option line
  (`[activated 5 times this window]`), or a soft cap annotation once the controller's creature
  count passes a threshold. The option stays offered; the pilot gets a countable to stop on.
- **Validated next by**: seat 123. **Metric**:
  `/usr/bin/grep -c 'activated this' <corpus>/*deck123*.jsonl` non-zero, or the option line
  carries a cap annotation — which is also the registered removal condition for the guide's
  terminator clause.

---

# Tier 2 — representation and correctness, mechanism located

## #W41-7 — [MEDIUM-HIGH, DESIGN — owner-floated, now with a MEASURED cost] Batch answer for repeated identical `may`-prompts in one window

- **id**: #20, carried, **with the cost the wave-39 entry could only guess at**.
- **Owner's tentative ask (his words)**: *"maybe some sort of mass accept / deny option?"* — when
  the SAME `may` ability from the SAME source class triggers N>1 times in one resolution window
  with identical effect text, offer a bundled ask: **accept all / decline all / decide
  individually**.
- **THE MEASUREMENT (`seat126-validation.md` §3 + N2)** — one seat, six games:

  | metric | value |
  |---|---|
  | Perimeter Captain may-asks | **22** |
  | accepted (`Life`) | **22 / 22** (0 declines, 0 fallbacks) |
  | **max asks in ONE combat** | **8** (`126v139` t24 — two Captains × four blocking defenders) |
  | mean asks per combat | 2.75 (8 combats) |
  | share of the seat's decisions | **22 / 347 = 6.3 %** |
  | inference spent | **1,377 s** (median latency 52.1 s/ask) |
  | reasoning spent | **132,364 chars** (median 5,702/ask) |

  **22 identical answers, 23 minutes of inference, 132k reasoning characters.** Note the
  interaction the owner should see: **#12's fix is correct AND it created this tax** — the
  batching is what's missing, not the `may`.
- **Fits the campaign's bundle-a-whole-decision-into-one-ask doctrine** (blockers/attackers
  precedent). Benefits the GPT seat (one call instead of N) **and** human UX. Generalizes past
  the Captain (Soul Warden-class storms of identical triggers).
- **Lane scoping questions the wave-41 agent must answer before writing code**: where identical
  pending may-triggers can be detected (ActionStack / MTGAbility trigger queue); whether
  "identical" is safely decidable (same ability id + same controller + no per-instance targets);
  the contract shape (CHOOSE_MENU with 3 options). **Fixture**: 3+ defenders block in one combat
  — ⚠ see **#W41-15**, the suite driver currently wedges on exactly this fixture shape, so the
  driver fix may be a prerequisite.

## #W41-8 — [MEDIUM-HIGH, REPRESENTATION] The cast option's `legal targets right now:` clause names targets with no P/T and no tags, and the sub-menu that carries them never opens at one target

- **id**: W41-PATHFACTS (`seat125-validation.md` §5.2). **This is #4's fix applied to the adjacent
  emitter, and #4's payoff is the argument for it.**
- **Mechanism**: `Cast Path to Exile {w} - legal targets right now: Perimeter Captain`
  (`126` s18) vs the battlefield line for the same card,
  `Perimeter Captain {w} (0/4) [defender]`. When exactly ONE target is legal the engine correctly
  makes no model call for the target — so the only surface that carries the deciding facts is
  skipped entirely.
- **Cost measured**: **6 of 13 Path casts spent below the guide's decline floor** (seat125 §1.4),
  4 of them on printed **0/4 walls at 25-50 life**:

  | seq | life | target(s) offered | |
  |---|---|---|---|
  | `139` s5 (t3) | 20 | Arboreal Grazer 0/3 (on the guide's own let-it-resolve list) | VIOLATED |
  | `152` s14 (t9) | 18 | Luminarch Aspirant (printed 1/1) | VIOLATED (defensible Magic; guide floor says decline) |
  | `126` s7 (t4) | 25 | Wall of Omens 0/4 | VIOLATED |
  | `126` s18 (t12) | 25 | Perimeter Captain {w} (0/4) [defender] | VIOLATED |
  | `126` s44 (t28) | 37 | three 0/4 walls | VIOLATED |
  | `126` s62 (t42) | 50 | Perimeter Captain 0/4 | VIOLATED |

  Target SELECTION when a choice existed was **4/4 correct** — so the failure is exactly at the
  windows where the render skips the facts.
- **Ask**: annotate the `legal targets right now:` clause the way #4 annotated the stack clause —
  `Perimeter Captain {w} (creature 0/4) [defender]`.
- **Validated next by**: seat 125. **Metric**: coverage fraction of `legal targets right now:`
  clauses carrying cost + type + (P/T) + live keyword tags; Path casts below the guide floor
  **6/13 → 0** (paired with the guide's floor rework this wave, so report BOTH the render fraction
  and the adherence rate — attribute deltas across both, wave-35 rule).

## #W41-9 — [MEDIUM] Remaining alias-hardcode counter cards unreachable as responses — **and the whole `#3` register is untested against a MODAL counter**

- **id**: #21, carried, **merged with `fix-validation.md` F7**.
- **Carried mechanism**: Lane F fixed BEB/REB by full script replacement (modal choice + stack
  target; alias blocks bypassed). STILL BROKEN on the same mechanism: **Hydroblast/Pyroblast**
  (alias 1191/1312), **Spell Blast (1224)**, and **the five Lace cards** —
  SpellTargetChooser / SpellOrPermanentTargetChooser are invisible to
  `TargetChooser::validTargetsExist()` (`TargetChooser.cpp:1658` walks zone-gated
  `MTGCardInstance` entries only; spells on the stack match neither), so
  `GameObserver::targetListIsSet()` refuses the cast and LegalActions drops the offer whenever no
  matching PERMANENT exists.
- **⚠ Hydroblast/Pyroblast are NOT a straight BEB port**: Oracle is *"Counter target spell IF it's
  red"* — targets ANY spell, colour-checked at RESOLUTION. The current colour-restricted chooser
  **over-restricts targeting legality**. Needs a conditional resolution, not a colour-restricted
  target.
- **Two fix shapes**: (a) script the remaining cards where the DSL allows (Spell Blast: X-cost
  counter; Laces: colour-change on stack); (b) fix `validTargetsExist()` to see stack spells for
  spell-choosers (engine-level, benefits all).
- **NEW, from F7 — the validation gap that must be closed in the same lane**: **Silverquill
  Command is a latent counter and was never used as one.** 8 resolutions this corpus, every one
  choosing a non-counter mode (`return creature and you draw`, `creature gains 3/3 and you draw`).
  It is deck146's only counter-capable card, so **#3's register is unvalidated against a MODAL
  counter** — if the counter mode is even reachable, it may not raise `WEventSpellCountered`.
  **One targeted check in this lane before #3 is declared closed pool-wide**: does the
  Silverquill counter mode appear in the cast/mode menu at all, and does taking it raise the
  event and the `was COUNTERED by` line?

## #W41-10 — [MEDIUM, MAGNITUDE TRUTH — ledger-#10 class, on the two cards that WIN a deck] `{right now: life 0}` on Sanguine Bond and Exquisite Blood

- **id**: N1 (`seat126-validation.md` §6).
- **Mechanism**: `dynamicMagnitudes` evaluates a *triggered, future-event* life change at cast
  time and emits 0. **27 emissions this corpus**: `Cast Sanguine Bond {3}{b}{b} {right now: life
  0}` ×19 and `Cast Exquisite Blood {4}{b} {right now: life 0}` ×8.
- **Why it outranks its LOW-looking measurement**: the annotation asserts that **the only two
  cards that win this deck the game are worth zero life**. Three waves of magnitude work have
  trained the pilot to prefer annotations to card text — which is precisely the argument that
  closed #10 — so a WRONG magnitude is strictly worse than an absent one.
- **Status: LATENT, not yet a measured loss** — the pilot cast both every time they were offered
  (guide rule #3.1 is louder than the annotation). Docket it before that stops being true.
- **Ask**: suppress the clause for triggered-on-a-future-event amounts (the branch #10 already
  took), or name the branch (`{on each life gain: opponent loses that much}`).
- **Validated next by**: seat 126. **Metric**: zero `{right now: life 0}` on a card whose `auto=`
  fires on a future trigger.

## #W41-11 — [MEDIUM] Tragic Slip's option line carries no live magnitude, so a `{B}` spell's whole value must be recovered from the GAME LOG tail

- **id**: L-123c (`wave40/deck123/findings.md` §5).
- **Mechanism**: the line prints the static text (`-1/-1 … Morbid — … -13/-13 instead if a
  creature died this turn`) and never the value that applies RIGHT NOW. The engine already
  computes and renders live magnitudes for other cards (`{right now: drains N}`).
- **Harm measured**: 9 casts, **4 killed the target, 5 did nothing**. Sharpest repro is a
  same-window control — `vs146` **seq77** (t25): the pilot targeted **Angel #1 (4/4)** with
  **Spider #1 (2/1)** and **Spider #2 (2/1)** *on the same target list*, no morbid, whiff. Also
  `vs162` s12 (Fate Unraveler 3/4 shrunk to 2/3, then took the pilot 18 → 1), `vs139` s3.
- **Why it is docketed rather than left to the guide**: adjudicating morbid currently requires
  reading the GAME LOG tail, the weak-tell shape this campaign's own rung warns about. The
  deck123 guide's primary rule is therefore keyed to printed toughness (board-readable) with the
  morbid upgrade as a **registered stopgap** whose removal condition is this item.
- **Ask**: `{right now: -1/-1}` / `{right now: -13/-13 (a creature died this turn)}`.
- **Validated next by**: seat 123. **Metric**:
  `/usr/bin/grep -c 'right now: -13/-13' <corpus>/*deck123*.jsonl` non-zero, and 0 whiff-casts
  where a toughness-1 target was on the same list.

## #W41-12 — [MEDIUM] `{0}` Equip is re-offered with no done-signal on the OPTION line — 190k reasoning chars in one game

- **id**: L-123d (`wave40/deck123/findings.md` §3.6 + §5).
- **Mechanism**: `Lightning Greaves` is `auto={0}:equip`, so
  `Equip with Lightning Greaves targeting X` costs nothing and the engine re-offers it at **every**
  priority window. The BATTLEFIELD line does carry `{attached: Lightning Greaves #1}`; the OPTION
  line does not say the Equipment is already attached to that creature, and re-attaching to the
  SAME creature is a pure no-op that also strips haste and shroud from the previous holder.
- **Measured**, one seat, six games:

  | game | equip windows offered | equips TAKEN | reasoning chars spent |
  |---|---|---|---|
  | 126 | 3 | 1 | 4,849 |
  | 162 | 2 | 2 | 13,733 |
  | 139 | 1 | 1 | 5,858 |
  | **146** | **30** | **22** (t7: 5, t11: **11**, t13: 2, t15: 4) | **190,348** |

  `vs146` seq11-18 and seq24-34 are a ping-pong: `targeting Bloodline Keeper` → `targeting
  Vampire` → `targeting Bloodline Keeper` → … **eleven times in turn 11 alone**. Three of the
  corpus's top-10 low-branching tax spikes are these 1-option equip windows (14,437 / 13,180 /
  12,622 chars). At a median 130 s/decision this class alone cost roughly **45 minutes** of one
  game's wall clock.
- **Class**: the deck110 wave-5 Cranial Plating finding, recurring on a `{0}` cost where the mana
  brake that usually limits it does not exist.
- **Ask**: annotate the option with the current holder, or suppress the re-offer that targets the
  creature the Equipment is already attached to (a re-attach to the SAME creature is a genuine
  no-op — suppressing it removes no legal outcome, so the never-suppress doctrine is not engaged;
  **annotation is still the preferred shape**).
- **Validated next by**: seat 123. **Metric**: equip windows offered per game with an already-
  attached holder → annotated 100%; re-equips to the same creature → 0.

## #W41-13 — [MEDIUM, PERCEPTION] The attackers window carries no evasion-vs-YOUR-blockers statement, so a hold-back decision must predict a block legality the blockers window later states outright

- **id**: W41-5 (`seat152-146-validation.md` §3, §6).
- **Mechanism**: the blockers window renders `[NONE of your available blockers can block this
  attacker — it has flying and none of your available blockers has flying or reach]` (shipped as
  #6, 12/12 this corpus). The ATTACKERS window carries nothing equivalent — so a seat deciding
  whether to hold a body back has to derive next turn's block legality itself.
- **Repro with a false premise proved false**: `146v152` **seq34** — the seat kept Silverquill
  Silencer (3/2, ground) home explicitly *"to block Elite Spellbinder next turn"*. **Spellbinder
  flies.** The hold was correct by the guide's gate and worthless in fact.
- **Ask**: carry the evasion-vs-my-available-blockers statement onto the attackers window's
  own-creature lines (what each of your creatures could block if held back), or at minimum tag
  opposing creatures with the evasion that will matter.
- **⚠ Scope check before writing**: #6's register is **single-branch** — all 12 occasions this
  corpus were flying-vs-no-reach. Menace, protection, shroud/hexproof-from-blockers and "can't be
  blocked by walls" **never occurred**. The register is an untested branch, not a passing one; a
  lane extending it to the attackers window should exercise a second evasion class or say plainly
  that it did not.
- **Validated next by**: seats 146, 152 (both run ground boards into fliers).

## #W41-14 — [LOW, LABEL TRUTH] The `may`-menu option reads `1. Life` — no magnitude, no source

- **id**: N3 (`seat126-validation.md` §6). Renders as
  `Choose an option for Perimeter Captain: / 1. Life / 2. Decline - do nothing`. The label carries
  neither the amount (2) nor which trigger raised it, and the same bare `Life` label would render
  for ANY life-granting `may`. **The deciding fact is not on the option** (P1/P2/P4 doctrine).
- **Ask**: `1. Gain 2 life (Perimeter Captain — a defender you control blocked)`.
- **No loss measured** (22/22 correct) — but the pilot spent a median **5,702 reasoning chars**
  re-deriving the magnitude from the battlefield line each time, which is #W41-7's whole bill.
  **Fix this in the same lane as #W41-7** — a batched ask whose label still says nothing is a
  worse ask, not a better one.

## #W41-15 — [LOW, SUITE HARNESS] The suite driver wedges on a `may`-menu raised at the blockers step

- **id**: #22, carried (the arrival-trace half is DISCHARGED — see the closed table).
- **Mechanism**: a scoped suite test (attacker declared, Captain blocks, may-prompt raised) hangs
  the suite driver right after duel entry — no DO step executes; the `yes` keyword and the
  unanswered-menu default (`TestSuiteAI.cpp:313-363`) never engage. **LIVE games are unaffected**
  (a merged-binary Baka selfplay 126v36 ended naturally turn 19 with the may-Captain in deck).
- **Consequence**: #12's behaviour change ships with live-game validation + primitives-load gates
  but **no suite fixture**. **Prerequisite check for #W41-7** — its fixture is the same shape.

## #W41-16 — [LOW, NEW] A daybound "Day" marker renders as a `[spell]` on the stack with no card facts

- **id**: F1 (`fix-validation.md`). `…deck152-…-vs-deck126` **seq 16**:
  `ON THE STACK … 1 (top): your Day [spell]` — emitted right after `- You cast Brutal Cathar`
  (daybound). This is the **only** one of 109 stack spell entries missing cost/type/P/T, and it is
  missing them because it is not a card: the day/night STATE MARKER is being pushed onto the stack
  and typed `[spell]`.
- **Why it is on the docket at all**: a small trust-doctrine violation (the pilot is told a spell
  it can respond to is on the stack), and it is the entire reason #4 reads 99.1% rather than 100%.
- **n=1 this corpus**; deck152 is the only daybound deck in the pool. Ask: exclude designation/
  state markers from the stack render (the wave-25 rules-fix batch already excluded designation
  markers from suite accounting — the same exclusion list is the likely home).

## #W41-17 — [LOW, CARRIED, = #15] The blockers echo still has no disambiguator, and the `may`-echo made it worse

- **id**: N4 / #15. `126v139` **s87** narrates `Wall of Omens blocks Dryad of the Ilysian Grove;
  Wall of Omens blocks Pollywog Symbiote; Perimeter Captain blocks Pollywog Symbiote; …` —
  identical names, no `#1/#2`, while the B-lines and battlefield lines both carry them.
- **Now cheaper to fix than it was**, because the Captain may-echo
  (`- You chose Life for Perimeter Captain`, **×8 in a row**) makes the ambiguity worse: eight
  identical lines with no way to tell which defender each belongs to. Legibility only — every
  reply was legal and executed correctly.

## #W41-18 — [LOW] `Flip Side with Bloodline Keeper -> DISPLAY TOGGLE only` is dead weight in 38 / 285 prompts

- **id**: L-123b (`wave40/deck123/findings.md` §5). Offered at nearly every priority window from
  the moment a Bloodline Keeper is in play; it changes nothing about the game state; and it sits
  **directly adjacent** to the real `Transform:backside with Bloodline Keeper [cost: {b}]` line,
  which appears in only **1** prompt. Two near-identical lines, one inert, at the seam that
  decides this deck's best card.
- **Ask**: suppress the display toggle for AI consumers, or order it below the declines. Repro:
  `vs125` seq25-45 (~20 consecutive windows), `vs139` seq33/37/38.

---

# Tier 3 — watch items and observations (no fix scheduled)

- **#W41-W1 — [WATCH, n=1] Cross-kind protocol bleed.** `146v126` **seq49**: a **priority**
  (`CHOICE:`) window answered with `ATTACK: Barrowin, Spirit Cai, Goblin, Spider #1, Spider #2` —
  the model pre-answered the attackers question one decision early. Latency 180 s (the seat's
  slowest record), `unparsed_reply`, 1/177 for that seat. This is wave-39 **#14** recurring on a
  different seat (that was n=2 at `146v152`). **Cumulative n=3 across two corpora — promote at
  n ≥ 4 in one corpus.** Self-healing; the heuristic answered.
- **#W41-W2 — [WATCH] `unclosed <think> (budget/truncation); forcing the answer`.** 1 occurrence
  in each of the three games where deck125 was player 0, 0 in the other three. All three still
  produced a parsed answer. n=3 watch line.
- **#W41-W3 — [OBSERVATION] wave-39 #17 (PLAN-protocol churn) did NOT reproduce.**
  `seat126-validation.md` N5: the two largest replies this corpus (`126v162` s47 at 20,056 chars,
  `126v125` s56 at 23,872) are ordinary priority deliberations, **not** PLAN-protocol litigation;
  median reasoning flat at 7,062 chars. **#17 stays a watch item and did not gain a second corpus
  of evidence.** Do not promote it.
- **#W41-W4 — [OBSERVATION] The reasoning tax barely moved at the seat whose biggest belief was
  fixed.** deck125 median `reasoning_chars` 6,030 → 5,558 (−8%) despite the Emrakul rehearsal
  dropping from 85% to 17% of traces; max 22,533 (was 12,733). **The guide reclaimed the wasted
  deliberation and spent it again.** Not a defect — recorded so nobody claims a token win from a
  belief fix.
- **#W41-W5 — [INFO, checked-clean] `Mana available: 0` at the X-announce ask is NOT a lie.** All
  10 X-announce asks render `Mana available: 0 total`, and the same line carries
  `| Already in pool: {u}{u}{w}{w}{w}{w} (6 mana ALREADY produced and floating right now - this is
  SEPARATE from the 0 untapped sources counted above…)`. The zero is literally true and the
  reconciliation clause is present. **No defect** — recorded because it is the shape a future seat
  will mis-flag, and because any guide rule keyed to the bare string `Mana available:` still reads
  0 at that window.
- **#W41-W6 — [LOW, OBSERVATION] Intruder Alarm's untap trigger resolves one priority window
  later than the activation.** `vs139` seq27 renders `Thraben Doomsayer #1 … [tapped - cannot
  attack or block this turn]`; seq28 renders the same permanent untapped with no intervening
  action. Rules-correct (the trigger uses the stack), but a pilot that checks "is it untapped
  yet?" immediately after activating sees the wrong answer. Note only.

---

# GUIDE-LANE ITEMS ROUTED OUT OF THIS LEDGER

These are recorded here **only** so no fix agent picks them up. Every one is executed as a guide
edit in `wave40/synthesis-notes.md` §3; none is an engine change.

- **W41-RULE12** (seat 162) — Rule 1 and Rule 2 collide once the brake is OFF. → guide edit 162-E3.
- **W41-ATTACKLIST** (seat 162) — the Fate Unraveler hold is list-shape sensitive. → 162-E4.
- **W41-1/W41-2** (seat 152, the validator's numbering) — Edit-1 side-effect (unforced
  `ATTACK: none` at an UNARMED sole-attacker window) and Edit-2 over-reach (a trade-up block
  declined at N=11). → 152-E2 and the parenthesis ladder (152-E3).
- **W41-3/W41-4** (seat 146) — chump-anchor regression and the **cross-seat contradiction** with
  152 over the same parenthesis. → the parenthesis ladder (146-E1/E2/E3 + 152-E3). See
  synthesis §3.4 for the single resolution.
- **W41-6/W41-7** (seats 152/146) — HL6 frequency strings. → 152-E1, 146-E4/E5.
- **deck125**: Revelation library clause, Path decline-floor rework, sweeper exactly-1 fallback,
  Emrakul-resolved contradiction, Staff-vs-hold-mana precedence, counter-table → numeric rule, and
  **two stopgap RETIREMENTS**. → 125-E1..E7 + 125-S1/S2.
- **deck126**: rule #1 Main-1 window, rule #7.7 zero-or-one, rule #5 `(both die)` clause. → 126-E1..E3.

---

# ROSTER-OWNER FLAGS — not fix lanes, no agent acts on these

- **deck126 construction — RESOLVED AND VALIDATED.** The owner's `Forest (SHM) * 16 → * 1` fix
  (75/39 → **60 cards / 24 lands, 40%**) rode this corpus. Confirmed IN-GAME, not just on disk, by
  an independent discriminator: Idyllic Tutor reveal-menu size **61/58 → 50, 49, 45, 37, 37**.
  Observable consequences: 6/6 openers `3 lands, 4 spells`; the old `Play Forest`/`Cast nothing`
  flood stretches are gone; first-combo-piece turns moved t9/t9/t11/t17/t25/never →
  t6/t7/t9/t12/t13/t9 and BOTH-at-once went **3/6 → 4/6**. **The frequency gain is CONSTRUCTION**;
  the guide's contribution is negative-space (zero declined-castable-piece windows).
- **deck125 is CONSTRUCTION-CEILINGED (note, not a request).** Unchanged and re-confirmed: Emrakul
  costs `{15}`; it was offered **2** times in six games (both taken, both arrived) at 15+ and 16
  mana on turns 28 and 48. Three of six losses ended with the opponent at 18-21 life on a
  1-damage-per-turn Staff clock. The shipped guide plays the deck it actually is and **is
  validated independent of the win column** per the ceilinged-seat rule. **No fix agent touches
  deck125.txt.**
- **deck162 `#HINT` lines are not enforced for the GPT seat** — carried, unchanged, expected. The
  guide carries the instruction as an ordering rule with a lethal exception; this wave proves the
  failure mode is **over-attacking, not under-attacking** (seat162 §4c), so a blanket "never
  attack" remains wrong.

---

# Recommended lane batching for wave-41 step 1

Disjoint files, merge order **engine → parser/seam → render** (campaign worktree rule; the
merged-tree gate on master is the only gate that counts). Lanes B, C and D all touch
`AIPlayerGPT.cpp` in different regions — that merges fine (proven), but apply **B → C → D** and
expect `patch -p1 --fuzz=3` on the third. **PARSETEST count on the merged tree must equal the
exact SUM of the lanes' added cases.**

| lane | items | primary files | why grouped |
|---|---|---|---|
| **A — cast-commit / X seam** | **#W41-1**, **#W41-2** (fixture) | `src/AIPlayerGPT.cpp` (cast-planning + ANNOUNCE_X region), `src/AIPlayerBaka.cpp`, a new `[AI]` fixture | one causal chain on the same path; the #2 fixture exercises #W41-1's missing window. Highest severity, single owner. |
| **B — narration channel (OWNER-RULED)** | **#W41-3** (a-e, incl. #23 and F3) | `src/AIPlayerGPT.cpp` (narration region ≈:2036 and :2418-2425), `src/ActionStack.cpp`, zone-change emitter | ONE lane by construction — five sub-fixes in one emitter family; splitting them guarantees a seam defect. Owner's fix shape is binding. |
| **C — combat / block-outcome** | **#W41-5**, **#W41-13** | `src/AIPlayerGPT.cpp` (blocker + attacker emitters), combat-outcome predictor | both are the block/attack perception surface; #5's register is the code #W41-13 extends. |
| **D — option/annotation render** | **#W41-8**, **#W41-10**, **#W41-11**, **#W41-12**, **#W41-14**, **#W41-16**, **#W41-18** | `src/AIPlayerGPT.cpp` (option-line emitters + `dynamicMagnitudes`) | all option-line emitters, all coverage-fraction-validated, all cheap; one agent avoids merge churn in one region. |
| **E — may-batching design + suite driver** | **#W41-7**, **#W41-15**, **#W41-14** (label) | `src/ActionStack.cpp` / `src/MTGAbility.cpp` trigger queue, `DecisionContract.h/.cpp`, `src/TestSuiteAI.cpp` | design-first lane: scope before code. #W41-15 is its fixture prerequisite. |
| **F — card data / target choosers** | **#W41-9** | `Res/sets/primitives/*.txt` (byte-splice), `src/TargetChooser.cpp` | no overlap with the C++ render lanes; carries the Silverquill modal-counter check that gates #3's pool-wide closure. |
| **G — bound, not suppression** | **#W41-6** | option-label emitter + activation counter | must be scoped by someone who has read the never-suppress ruling; the deliverable is a COUNTABLE, not a filter. |

**Probe runs owed in wave 41** (sequential, never concurrent, deleted after):
1. **mutate probe (198/199)** — discharges **#W41-4**, whose seat leaves the pool.
2. **deck152 edit probe (198/199)** — one large opposing body + a seat board of one creature to
   force an ARMED SOLE-ATTACKER window; and a small-attacker board at low life to force a blocker
   window at forecast ≤ 9. Discharges seat 152's two un-adjudicated edits without a third
   corpus null.

## #W41-19 — [HIGH, OWNER LIVE-PLAY REPORT 2026-08-23, Vita session] Upkeep triggers: Delver no first-upkeep trigger; Black Vise damage not applying
- Owner verbatim: "delver of secrets doesn't trigger on the first upkeep after playing
  it. there's a problem with black vise, doesn't seem to be applying damage when its
  supposed to. possibly same upkeep bug."
- Owner observation = ground truth; the same-bug hypothesis is HIS, unverified — test it.
- Investigation lane running (Oracle-first, repro fixtures, class sweep over other
  @each-upkeep cards, fix at the right layer). If it's an engine ETB-registration/
  phase-ring ordering defect, the class is far wider than two cards.

## #W41-20 — [HIGH, OWNER LIVE-PLAY REPORT 2026-08-23, Vita session] Lord-granted keyword persists after the lord dies (sliver first strike)
- Owner verbatim: "playing against a sliver deck, a 2/2 spinnaret sliver and a 2/2
  horned sliver blocked my 2/2 frost lynx, and lived while killing the lynx, no combat
  trick. my hypothesis is they are somehow recieving first strike from one of the two
  striking slivers that died in previous turns."
- Combat math corroborates: 2x 2/2 blockers vs a 2/2 — without first strike one blocker
  must die; with it, none do. Hypothesis (HIS, unverified): Striking Sliver's granted
  first strike not removed when the grantor left the battlefield.
- If confirmed, the CLASS is every "creatures you control have X" lord (ListMaintainer/
  lord machinery grant-removal on leave) — sweep, don't fix one card.

## #W41-19-REVISED (OWNER RULING 2026-08-23, binding) — Black Vise rescript REJECTED as a workaround
- Owner verbatim: "the solution to this problem isn't to script the card wrong. that's
  a workaround. we shouldn't be doing workarounds. we should be fixing problems."
- The Iron-Maiden-form rescript is REVERTED. Black Vise keeps its Oracle-faithful script
  (as-enters choose an opponent -> @each targetedplayer upkeep). The ENGINE mechanism is
  the fix target: (a) an unanswered as-enters chooser must never wedge the game; (b) a
  cancelled/mis-answered mandatory ETB choice must never leave a permanently inert
  permanent — mandatory choices re-ask or auto-resolve; (c) with exactly ONE legal choice
  (the only opponent in a 2-seat game) the choice may auto-resolve (no-decision-when-one-
  outcome is an existing engine principle); (d) every seat must be able to answer it
  (human click, Baka, GPT, suite driver).
- Sweep the class: every card using the as-enters player-chooser / targetedplayer pattern
  shares the fragility — fix the mechanism, validate on Black Vise + at least one other.
- CS-022a in docs/card-script-potential-issues.md is OBSOLETE once reverted — revise it.

## #W42-1 — [LOW, SUITE HARNESS] Unanswered multi-candidate chooser: fail loudly, never hang or guess
- Residual from the chooser-mechanism lane (CS-022a): the suite driver has defaults for
  unanswered MENUS but none for an unanswered CHOOSER with >=2 legal candidates — such a
  fixture hangs its test. Single-candidate is covered by the auto-resolve.
- Resolution chosen (2026-08-23, discussed with owner): a LOUD RED — the driver fails the
  test with "unanswered chooser: <shape>" instead of hanging, and NEVER answers with a
  guessed default (a guessed pick is the false-green shape the suite disciplines exist to
  avoid). Wave-42 step-1 candidate; TestSuiteAI.cpp, small.

## #W42-2 — guide-rule false arm on attack-restricted bodies (probe-2 finding, 2026-08-24)
deck152 probe G1 s32: the creature arming the "biggest opposing power" return-swing check
was Goblin Goon, rendered `[can't attack, can't block, cantpwattack]` — a swing that cannot
happen. Guide step (1) reads POWER only. Fix candidate is a GUIDE-side rewording ("biggest
opposing power among creatures that can attack"), possibly generalized across all guides
using the power-floor pattern; render already carries the restriction tags, so no engine
work owed. Full context: wave41/probe-results.md.
