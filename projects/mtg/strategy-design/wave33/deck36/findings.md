# deck36 (Master of Ether — UB artifacts) — wave-33 seat review

Corpus `matchups-20260819-070739`, binary `bdb30d8e7`, -T 3000, -j 3.
Seat: 6 games, **201 asks** (32/25/36/36/42/30), 6 stderr files.
**Fallbacks 0/201. Defers 0. Truncations 0. `validation_reject_reask` 0.
`unparsed_reply` 0. `all_assignments_illegal` 0. `pruned_pairs` never written.
Crash signatures 0/6.** Record **3/6** (was 4/6).
Card facts read from `bin/Res/sets/primitives/mtg.txt` with `/usr/bin/grep` this session.

---

## 0. HEADLINE — the i2 rematch verdict: **PASS, and the fix flipped the DECISION, not just the text**

The wave-32 reference repro was THIS seat's loss: deck36 s25 t12, `no blocks` at 20 life
against four `[infect]` attackers, on a header that said *"you would be at 15 - NOT lethal"*.

**Wave-33, same matchup, deck36 WON at 20 life on turn 11 (opponent -5).** Every infect
blocker window, with its forecast verbatim:

| # | seq / turn | forecast text (verbatim, the whole projection line) | decision |
|---|---|---|---|
| 1 | s14 t4 | `Your life: 20. Your poison counters: 0 of 10.` / `Unblocked, these attackers put up to 1 POISON COUNTER on you - you would be at 1 of 10 poison - that is not yet 10, but poison counters never reset and nothing here removes them, so every counter you take is permanent progress toward losing. Life total is not the resource under attack; price the block against the poison count.` | **Cathodion blocks Blight Mamba** |
| 2 | s20 t6 | `Your life: 20. Your poison counters: 1 of 10.` / `Unblocked, these attackers put up to 1 POISON COUNTER on you - you would be at 2 of 10 poison - that is not yet 10, but poison counters never reset and nothing here removes them, so every counter you take is permanent progress toward losing. Life total is not the resource under attack; price the block against the poison count.` | **Ornithopter blocks Necropede** |
| 3 | s26 t8 | identical string to #2 (`you would be at 2 of 10 poison`) | **Master of Etherium blocks Necropede** |

Each attacker line also carried the per-attacker partition:
`A1. Blight Mamba (1/1) deals 1 [infect] [its combat damage to YOU is dealt as POISON COUNTERS, not
life loss - your life total will not move and blocking is the only thing that stops it] [infect also
damages CREATURES as -1/-1 counters, which do not wear off at end of turn - a blocker that survives
stays shrunk]`

**Coverage: 3/3 infect blocker windows partitioned by damage type and priced against 10.
`you would be at N - NOT lethal` emissions on infect windows: 0/3.** The prediction's forbidden
string does not occur on any infect window at this seat.

**Negative control (same-seam non-infect windows) UNCHANGED, 7/7.** Verbatim:
`Your life: 20. Unblocked, these attackers deal up to 3 - you would be at 17 - NOT lethal: block
only where the trade favors you; taking damage while ahead is often correct.` (vs158 s13, vs146
s9/s12/s16/s20, vs139 s9) and the lethal branch `Your life: 9. Unblocked, these attackers deal up
to 11 - you would be at -2 - LETHAL if it all connects: block enough to survive.` (vs152 s30).
The header machinery is untouched on the ordinary branch.

**Did the honest forecast change the BLOCKS, or only the prose? It changed the blocks, and the
replies name the new fact as the reason.** Wave-32: 5/5 no-blocks. Wave-33: **3/3 blocks**:

- s14: *"Blight Mamba has infect, meaning its damage gives you a permanent poison counter. While
  1 poison counter isn't lethal immediately, accumulating them is a guaranteed loss condition
  that cannot be removed."*
- s20: *"You currently have 1 poison counter; taking another brings you to 2, which is safe for
  now, but blocking is free and prevents the counter entirely."*
- s26: *"You currently have 1 poison counter; taking 1 more puts you at 2/10, which is safe for
  now, but every counter is permanent and brings you closer to losing."* — then blocks with the 9/9
  Master to KILL the 1/1 rather than chump with the Ornithopter.

The wave-32 game-losing shape (no-blocks at a healthy life total on a "NOT lethal" life claim) is
**DEAD at this seat**. The seat ended the game at 20 life and 1 poison.

**Alternative explanation, stated and rejected:** the win is not only a blocking artifact — the seat
also drew Master of Etherium here and killed on turn 11. But the DECISION-level claim stands
independent of the record: at 3/3 windows the pilot took a block it declined 5/5 times last wave,
on the same board shape, quoting the new render as its reason.

---

## 1. i1 — poison is RENDERED (fractions)

| Surface | Fraction at this seat |
|---|---|
| Situation-block poison lines (`Poison counters (you): N of 10 - you LOSE the game at 10 poison counters, whatever the life total is; 9 more end it. Poison is not life: it does not reset between turns, and gaining life or preventing damage does not remove a poison counter.`) | **17/17 prompts from the first poison counter (s20) through game end (s36)** = 100% |
| Blockers-header poison forecast on infect windows | **3/3** (fires even at 0 poison: s14 reads `Your poison counters: 0 of 10`) |
| Non-combat poison NARRATED | **YES** — Ichor Rats' ETB narrates symmetrically and distinctly: `- Poison: the opponent takes 1 poison counter - now 1 of 10 (the opponent LOSES at 10 poison counters; 9 more end it)` / `- Poison: you take 1 poison counter - now 1 of 10 (...)`. The wave-32 four-silent-events class is dead. |
| Infect COMBAT damage narrated distinctly | **YES** — `- Infect damage: 1 from Blight Mamba to Cathodion - dealt as 1 -1/-1 counter, a permanent shrink that does NOT wear off at end of turn`, paired with `- Counter added to Cathodion: -1/-1 (now 2/2) [from Blight Mamba]` |
| Non-infect games | 0/158 prompts carry a poison line — correct suppression, no leakage |

**One zero-suppression, recorded not escalated (N-36i-b):** the situation-block poison pair is absent
for s1–s19, i.e. while both totals are 0. The blockers header does print `0 of 10`, so the one window
where it is decision-relevant is covered. Flagged only because it is the same
`if (value) print(value)` family that N-36b's fix just closed, and because deck36 has no infect —
the OPPONENT's `0 of 10` is the only view it ever gets of an alt-win it could theoretically race.
Cost this corpus: zero.

---

## 2. i5 / N-36b — the {0} confabulation class, at the natural witness seat

**PREDICTION: zero `{0}`-confabulations. VERDICT: PASS, with a clean render fraction behind it.**

Coverage as FRACTIONS, scoped per emitter (regex over every prompt line of the class; `#N` instance
handles tolerated between name and cost):

| Emitter | renders of Mox Jet / Mox Sapphire / Ornithopter | carrying `{0}` |
|---|---|---|
| Hand line (`Your hand: ...`) | 55 | **55/55** |
| Cast-option line (`N. Cast ...`) | 26 | **26/26** |
| Battlefield line (`Your battlefield (creatures: N): ...`) | 427 | **427/427** |
| **Total engine cost emitters** | **508** | **508/508 = 100%** |

Plus a FOURTH emitter that did not exist in wave-32, also correct — the pregame affordability
header: `Cheapest spell in this hand: Mox Jet {0} (mana value 0).` and `Playing every land in this
hand would cover the cost of: Salvage Slasher {1}{b}, Mox Jet {0}, Mox Sapphire {0}, Scarecrone {3}.`

Wave-32 baseline: **347 renders, 0 carrying `{0}`.**

**Confabulation sweep over all 201 replies** (regex for the four wave-32 shapes and their
neighbours: *not spells you can cast* / *do not "cast" artifacts* / *land drop slot* / *cost mana to
cast* / *no cost shown|listed|printed* / *does not show a mana cost* / *unknown cost*): **0 hits.**

**Behavioural consequence, the number that matters:** `Cast nothing right now` was offered in
**45 windows and taken 0 times.** Wave-32 baseline: 7 taken, 4 of them declining a free artifact.
Guide RULE #1's falsifiable prediction #1 — **FIRED, 45/45.**

---

## 3. Phase-header contradiction — **SECOND WITNESS, CONFIRMED**

The seat verbalized it again, verbatim (vs152 s21, kind=priority, 177 s / 11,969 chars, chose `pass`):

> *"However, looking closely at the 'Current Situation' block, it explicitly states **"Phase: Main
> phase 1 | It is the opponent's turn."** This is a contradiction in the provided text (Main Phase 1
> is typically a player's own turn)."*

The header it quotes is exact: `Phase: Main phase 1 | It is the opponent's turn.`

**Fraction: 1 verbalized contradiction / 42 opponent-turn main-phase priority windows at this seat.**
Low rate, but the verdict is not a rate question — the wave-32 convergence check asked for a second
witness and this is it. Promotion recommendation and the proposed representation change (name the
turn OWNER before the phase) are in `general-suggestions.md`.

**A distinct, adjacent contradiction, separated deliberately** (vs158 s16, the seat's single worst
reply: 227 s / 13,930 chars on a `pass`): the pilot found the GAME LOG and the phase header mutually
inconsistent — *"Then 'CURRENT SITUATION' says 'Phase: Main phase 1'. This is contradictory. If I
already attacked and dealt damage, I am in the End Phase or Second Main Phase, not Main Phase 1."*
That is the log's turn-scoped combat entries read as THIS-turn entries. Different mechanism, same
symptom class; routed as N-36k, NOT counted toward the phase-header witness.

---

## 4. i10 attachments (probe lane N-36h) — **NO LONGER A NULL. EXERCISED and PASSING**

Wave-32 reported UNEXERCISED corpus-wide and wrote a probe-deck spec. **The probe is unnecessary:
deck105 runs `Relic Putrescence`, an Aura, and it attached to this seat's Mox Jet.**

Both render directions fire, in the same 11 prompts (vs105 s26–s36), verbatim:

- FORWARD, on the host permanent's battlefield entry:
  `Mox Jet {0} [artifact] {attached: Relic Putrescence}`
- REVERSE, on the aura's entry:
  `Relic Putrescence {2}{b} [enchantment] [attached to: Mox Jet]`

**Fraction: 11/11 windows carry BOTH directions; 0 windows carry one without the other.**

Card-fact check: `name=Relic Putrescence / abilities=infect / target=artifact /
auto=@tapped(mytgt):damage:1 targetcontroller / text=Enchant artifact -- Whenever enchanted artifact
becomes tapped, its controller gets a poison counter. / type=Enchantment` — the render's host is an
artifact, legal for `target=artifact`, so the attachment is CORRECT, not merely present.

**Honest residual:** the sub-question N-36h actually raised — *does the reverse render carry the
host's instance handle when the host has duplicate names* — is **still untested**; the host here is
a singleton Mox Jet, correctly rendered with no `#N`. And `[equipment]` / `Equip ` remain **0/201**:
the EQUIPMENT half of i10 is still unexercised. Updated probe spec in `notes.md`.

**One read error, non-costly (N-36l):** vs105 s32 the pilot attributed the aura to the wrong
permanent — *"A1 (Ornithopter) deals 0 damage due to the attached Relic Putrescence"* — when the
render says it is on Mox Jet. It self-corrected mid-reply and the decision (exclude the `[deals 0]`
body) was right for the right reason anyway. 1/11.

---

## 5. THE TWO CAP-ADJUDICATED LOSSES — both latency-spiral driven, one measurable to the second

Neither loss is a decision defect. Both games were still running at turn 13/14 when the 3000 s
wall-clock cap adjudicated them.

### 5.1 vs116 (LOST, adjudicated 5-27, turn 13) — **74% of the wall clock was this seat's replies**

| | asks | seat latency |
|---|---|---|
| deck36 (this seat) | 42 | **1,910 s** |
| deck116 (opponent) | 26 | 660 s |
| total | 68 | **2,570 s** of a 3,000 s cap |

The spike is ONE seam and the guide has nothing to say about it: **Emrakul's Annihilator.** The
engine asks the sacrifice ONE CARD AT A TIME, six times per trigger:

`SACRIFICE ONE OF YOUR OWN CARDS: your own effect (Sacrifice) makes you sacrifice a card from the
list below - each option is a card YOU will LOSE, NOT a target you attack or affect. Pick the card
you can best AFFORD TO LOSE (usually your LEAST valuable: a spare land or a redundant/dead card;
KEEP your best spells, answers, and threats). Choose card 4 of exactly 6 from the list below, and
answer with the chosen card's name.`

**16 sacrifice windows across the seat (s27–s32 and s38–s42 in this game).** The six worst decisions
of the game, by cost:

| seq | latency | reply chars | `post_plan_overrun` | decision |
|---|---|---|---|---|
| s35 | **214 s** | 12,555 | 0 | Cast Glaze Fiend |
| s39 | **163 s** | 11,190 | **7,777** | sacrifice Nuisance Engine #1 |
| s30 | **137 s** | 9,127 | 59 | sacrifice Swamp |
| s38 | **129 s** | 8,793 | **7,312** | sacrifice Mox Jet |
| s37 | 95 s | 5,825 | 0 | attack with Salvage Slasher |
| s31 | 93 s | 6,112 | 1,267 | sacrifice Island #1 |

**831 s — 44% of this seat's entire budget — on six decisions, four of which are "which of my tapped
lands do I throw away". Every one of them was answered correctly.**

**A contributing render defect, verbatim (N-36j):** *"The prompt says 'Choose card 1 of exactly 6'.
This is confusing. Maybe the list is truncated or I should just pick from the 9."* (s38; same
confusion at s30). The header says "of exactly 6" without ever saying **six WHAT** — six sequential
sacrifices from one trigger. The pilot spends its first paragraph every time reconciling "6" against
a 9- or 11-item list.

### 5.2 vs152 (LOST, adjudicated 6-12, turn 14) — spiral plus a genuine board deficit

Seat 1,095 s / 36 asks; opponent 1,023 s / 37. Worst specimens: **s21, 177 s / 11,969 chars on a
`pass`** (the phase-header contradiction reply of §3) and **s23, 126 s / 7,502 chars on `Play Swamp`**.
Unlike vs116 this one was also losing on board — Fateful Absence exiled the Glaze Fiend, two Ranger
Class Wolves grew to 6/6, the seat sat at 6 life with one Master. The clock decided a game it was
probably losing; the vs116 clock decided a game that was merely slow.

### 5.3 The third loss, vs158, is neither a clock nor a decision loss

Death at -14 on turn 10, 32 asks / 1,109 s. Decomposed: no Master drawn, no Akroma, zero answers to
a 7/7 Orc army. **Both of its low-life attack decisions are correct and explicitly reasoned** — the
guide's untapped-swing-back rule fired verbatim at s23 (life 11): *"The opponent's Orc Army (6/6) is
tapped and cannot attack this turn. The opponent's only untapped creature is Orcish Bowmasters
(1/1), which is summoning sick... there is zero immediate danger of damage returning to me this
turn."* At s31 (life 3) it correctly proved no block survives: *"Blocking with Scarecrone against
the 7/7 results in my death (1 vs 7)."* Construction loss. It carries one spiral (s16, 227 s /
13,930 chars on a `pass`) that did not change the outcome.

---

## 6. THE COMMIT-FAILURE COUNTER — counted fields, plus an instrument gap that matters

**Replies whose text continues past their own `PLAN:` line: 25/201 (12.4%).**
**Of those, retracted: 0. Fatal: 0. `commit_retracted` is FALSE on all 201 records.**
Mean overrun among the 25 nonzero: **2,275 chars.** Max **7,777** (vs116 s39). Full distribution:
7,777 / 7,417 / 7,312 / 6,716 / 5,787 / 3,997 / 3,653 / 3,554 / 1,401 / 1,338 / 1,308 / 1,267 /
993 / 890 / 787 / 637 / 428 / 401 / 327 / 299 / 262 / 178 / 66 / 59 / 17.
**`pruned_pairs` was never written at this seat.**

**Replies with NO `PLAN:` line: 6/201** — all protocol-compliant under interim `f46dd58ee`, and none
scored `post_plan_overrun` (correct: the field returns 0 when there is no plan line).

### 6.1 ⚠ The counter cannot see this seat's most decision-relevant retraction

**8/201 replies (4.0%) emitted a coded answer and then emitted a DIFFERENT coded answer later in the
same reply** — the last-label-differs class; the parser correctly took the last. **Every one of those
8 scored `post_plan_overrun` 0 and `commit_retracted` false**, because the protocol puts `PLAN:` LAST:
a reply that re-answers and then plans has nothing after its plan line, and nothing for the
retraction machinery to refuse.

`postPlanOverrun` measures the tail past the FIRST `PLAN:`; `commitRetracted` fires only on the three
existing fallback exits (`retracted_choice` / `truncated_abandoned` / `truncated_abandoned_heuristic`).
**Neither is a post-ANSWER boundary** — which is the boundary the owner's directive legislates. The
two shipped fields therefore under-count the phenomenon at this seat by construction. One-line fix in
`general-suggestions.md`.

### 6.2 Owner-directive item 1(b): does the ~4.2% post-answer self-correction carry decision VALUE?

**At this seat, yes, in 8 of 8 — and one of them is the deciding play of a WIN.**

| game / seq | first coded answer | final coded answer (executed) | value |
|---|---|---|---|
| **146 s18** | `CHOICE: 2 (Cast Yotian Soldier {3} (1/4))` | **`CHOICE: 1 (Cast Akroma's Memorial {7})`** | **GAME-DECIDING.** Akroma's resolved turn 9, gave the whole team flying / first strike / vigilance / trample / haste / pro-red / pro-black (visible on every attacker line from s19), and the seat won on turn 13. The 4,600-char post-answer block is where it found the reason: *"With Akroma's Memorial, Cathodion has First Strike. It kills the 1/1 Adventurer before it deals damage... This is a huge difference."* |
| 116 s31 | `8 (Tolarian Academy)` | `4 (Island #1)` | keeps the 8-mana land instead of throwing it away |
| 116 s38 | `9 (Glaze Fiend)` | `1 (Mox Jet)` | keeps the only creature |
| 116 s39 | `8 (Glaze Fiend)` | `4 (Nuisance Engine #1)` | same, again |
| 158 s15 | `1 (Play Swamp)` | `2 (Play Seat of the Synod)` | artifact land = +1 count (the guide's own rule, recovered late) |
| 152 s11 | `1 (Play Swamp)` | `2 (Play Seat of the Synod)` | same |
| 116 s14 | `2 (tap Tolarian Academy)` | `1 (Create pest with Nuisance Engine)` | correct order: make the Pest first, THEN the Academy counts it |
| 158 s30 | `1 (Put in Play with Scarecrone targeting Salvage Slasher)` | `2 (Draw 1 with Scarecrone)` | defensible either way; deliberate |

**8/8 improved or held the decision; 0/8 degraded it.** This is evidence FOR the owner's fix and
against any variant that removes the reasoning channel WITHOUT replacing it: at deck36 an answer-only
protocol with no reasoning anywhere would have cast Yotian Soldier and probably lost the vs146 game.
Native pre-answer reasoning does replace it — but it must actually be TURNED ON, not merely have the
post-choice block deleted. Evidence contribution only, per the directive.

### 6.3 Owner-directive item 1(c): was discarded scratch text load-bearing for REVIEW?

Yes, twice, and both times it was the only record of a live defect:
- vs105 s35's abandoned first plan is what exposed the plan-splitter bug of §7 — the diagnosis is
  impossible from the committed plan alone.
- vs116 s38/s30's *"Choose card 1 of exactly 6. This is confusing"* is the only evidence for N-36j.

Both survive under the target protocol if native reasoning is captured to the translog as
`reasoning_content`, exactly as the directive specifies. No objection.

### 6.4 Owner-directive item 3 (narration decoration) — measured at this seat

Decoration inside the GAME LOG block only (`{card text: "..."}` and `[cost: ...]` on consumed
decisions, e.g. `- You: add blue mana for each artifact with Tolarian Academy [cost: Tap] {card
text: "{T}: Add {U} for each artifact you control."}`): **72/201 prompts carry any; p50 0, mean 81
chars, max 501, against a p50 prompt of 5,754 and a max of 20,153.** Consistent with the owner's
~320-in-12k measure. **Small, and not a latency driver at this seat** — supports "wrong by
architecture", not "expensive".

---

## 7. N-36e — the plan-splitter takes the FIRST `PLAN:`, not the last. ROOT-CAUSED; WORSE THAN WAVE-32

**107 of 201 prompts (53%) carry a `YOUR PLAN (as you last stated it):` field longer than 400
characters** (wave-32: 51/132 = 39%). p50 428, max **1,541**.

Repro, `vs105 s35 -> s36`, exact:

- s35's reply contains **two** `PLAN:` occurrences: one at char **501** (a first-pass plan it then
  talked itself out of) and the real committed one at char **5,359**.
- The s36 prompt's plan field is **1,424 chars** beginning with the char-501 text and running on
  through the abandoned deliberation: *"Proceed to the Declare Attackers step. Attack with Master of
  Etherium, Glaze Fiend, Cathodion #1, Cathodion #2, and Silver Myr (once it is cast, though it is
  currently in hand and needs to be cast first... Wait, looking at the options provided in the
  prompt... Or maybe the prompt is truncated? ... No, that's a contradiction."*
- s35's ACTUAL final plan — *"PLAN: Pass priority in Main Phase 1 to proceed to the Declare Attackers
  step. Attack with Master of Etherium, Glaze Fiend, Cathodion #1, Cathodion #2, and Silver Myr to
  deal lethal damage (approx 10+ damage) and win the game."* — **never reached s36.**

The pilot is fed its own RETRACTED reasoning as authoritative plan text while its committed plan is
discarded. Same first-vs-last anchoring the choice parser already gets RIGHT (§6.2's Akroma case
proves `parseChoice` takes the last line-leading label). Fix in `notes.md` N-36e.

---

## 8. STEP-1 / OPEN-LEDGER STEADY STATE

| Item | Verdict | Evidence |
|---|---|---|
| **i5 / N-36b** | **PASS** | 508/508; 0 confabulations; 0/45 `Cast nothing right now` |
| **i2 / N-105b** | **PASS** | §0; 3/3 infect, 7/7 non-infect control unchanged |
| **i1 / N-105a** | **PASS** | §1; 17/17 + non-combat + distinct infect narration |
| **i9 pregame hand-only** | **PASS, 6/6** | all six `s1` prompts contain no battlefield facts; the engine header (`Your hand (7 cards), counted by the engine: 3 lands, 4 spells` / `Mana sources among those lands, counted by the engine: {U} 1, {B} 2` / `Cheapest spell in this hand:` / `Playing every land in this hand would cover the cost of:`) present 6/6. **0 mulligans, 0 bottoms, 0 false mulligans, 0 `Mana available: 0 total` echoes.** All six hands were 3-land keeps, so this is a clean pass but a WEAK test. |
| **i8 combat tags** | **PASS** | `[summoning sick - cannot attack this turn, but CAN block]`, `[tapped - cannot attack or block this turn]`, `[tapped - attacking]` throughout; zero contradiction verbalizations about a combat tag |
| **i10 attachments** | **EXERCISED, PASS 11/11** | §4 (was an honest null; no longer) |
| **i3 `[-multicolor]`** | **UNEXERCISED at this seat** | 0 prompts contain `multicolor`; 0 contain `colourless`; the 48 `colorless` hits are all Nuisance Engine's Pest card text. deck36 casts no targeted spells — the natural-witness role belongs to deck146. |
| **i6 amass** | N/A | no amass card at this seat |
| **i4 / i7 / i8-parser** | N/A | no all-flexible manabase, no transform card, and all 4 block replies used INDEX form (`BLOCKS: B2:A1`) — no name-form reply emitted |
| **N-36c Master cast line one short** | **NOT FIXED — stopgap STAYS** | 3 casts, 3 offsets: vs105 s23 `Cast Master of Etherium {2}{u} (8/8)` at `you 8` → blocks as `(9/9)` at s26; vs152 s24 `(6/6)` → plan and board `(7/7)`; vs139 s11 `(7/7)` → attacker line `(10/10)` then `(11/11)`. Guide's `+1` teach **FIRED 4/4**: s24 *"enter as an 8/8 (becoming 9/9 immediately after)"*; vs139 s12 *"a massive 7/7 (effectively 8/8 due to its own artifact status)"*; vs146 s11 *"will be 8 on resolution"*; vs152 s25 *"Attack with Master of Etherium (7/7)"*. Wave-32 baseline: 0/2 correct. |
| **N-36d Academy in upkeep** | **FIXED BY THE GUIDE** | 25 Academy offer windows. **Upkeep takes 0/7** (baseline 6/8). Opponent's-turn takes 0/3. Main-phase-1 takes 4/10, all on turns the seat cast something. Prediction #4 FIRED. |
| **N-36f generic-payability belief** | **ABSENT, 0/201** | mana line wording unchanged (`Mana available: 4 total (four untapped sources, tapped automatically when you cast; colours you can make: {u}{b})`); the wave-32 rewording did not ship; the belief did not recur. Guide RULE #2 held. Recorded, no escalation. |
| **N-36g Clue `()` card text** | **RECURS, and spreads** | vs152 s20 `Draw 1 with Clue [cost: {2}, Sacrifice] {card text: "() source: Glaze Fiend"}`; the token family shows the same shape at vs116 s27–s29: `Pest #1 (0/1) [your battlefield] - "( artifact) source: Nuisance Engine"`. Cosmetic; correct action every time. |
| **N-152g legend suppression** | **NOT EXERCISED** | deck36 runs 1 Tolarian Academy and 1 Academy Ruins — no same-name legend can ever be in hand with its copy in play. 0 suppressed windows. |
| **N-158k Feed the Swarm** | **NOT EXERCISED** | 2 mentions, both in this seat's own reply prose speculating about its own deck. No target menu. |
| **L-18-1 planning tax** | **WORSE** | §7: plan field >400 chars at 53% (was 39%) |
| **L2 cross-zone dup-name** | **NOT WITNESSED** | 0 occurrences |

---

## 9. PRESERVE LEDGER (HL8 — enumerated BEFORE any rule was revised)

| Seam | Windows | Result |
|---|---|---|
| **Attacker declaration** | **30** | **30/30 PERFECT — improved on wave-32's 20/21.** Every body without `[deals 0 - this attack deals no damage to the opponent]` declared, in every window; every body carrying it excluded, in every window; and the one all-zero window (vs152 s15, a lone `Glaze Fiend (0/1) [flying] [deals 0 ...]`) correctly answered `no attackers`. |
| **Blocking** | 10 | **10/10 defensible.** 3/3 infect blocks (§0); vs152 s30 blocked the 6/6 Wolf with Scarecrone on a `LETHAL if it all connects` header; the 6 `no blockers` calls were each at 15+ life against 1–3 non-infect damage with only a chump available. |
| **Casting / deploy** | 45 `Cast nothing` offers | **0/45 taken.** Every free artifact taken on sight. |
| **Mulligan** | 6 | 6/6 keeps, all 3-land hands. 0 false mulligans, 0 bottoms. |
| **Reply integrity** | 201 | 0 fallbacks / 0 defers / 0 truncations / 0 rejects / 0 retractions. Mean reply 2,008 chars; latency p50 18.1 s, mean 31.1 s. |

**Consequence (HL8):** attackers, blockers and the {0} seam are floor-clean, so their guide text is
CONFIRMATORY, not corrective. The revision budget moves to the seam that traced to a loss.

---

## 10. GUIDE VERDICT — **REVISE.** `strategy.txt` is a complete replacement.

### 10.1 Firing table (all four verdicts used)

| Teach | Verdict | Basis |
|---|---|---|
| RULE #1 "your free artifacts are free, cast them the turn they appear" | **FIRED** | 0/45 cast-nothing; 0 confabulations |
| RULE #1's second half — *"the game does NOT print a cost on a free card"*, *"A card with no cost printed on its line costs {0}"* | **FALSIFIED / SUPERSEDED** | i5 shipped: 508/508 renders now carry `{0}`. Superseding strings quoted: `Cast Ornithopter {0} (0/2)` (4 occurrences), `Mox Jet {0} [artifact]`. **DELETED this revision.** |
| RULE #2 "any colour pays a generic cost" | **FIRED** | 0/201 recurrence (baseline 2 replies) |
| "Master lands one bigger — add 1" | **FIRED 4/4** | §8; N-36c unfixed, stopgap RETAINED with its removal condition restated |
| Academy answer-by-window | **FIRED 7/7 upkeep, 3/3 opponent-turn** | §8 |
| "send everything without `[deals 0]`" | **FIRED 30/30** | §9 |
| "at 12 or less, count their untapped swing-back" | **FIRED** | vs158 s23 verbatim, §5.3 |
| "Akroma's Memorial ends games — cast it the first turn it is listed" | **FIRED, and it WON the vs146 game** | §6.2 |
| Steel Wall / Yotian Soldier as the designated blockers | **MISSED — no window arose** | Steel Wall was on board in the vs116 game but never had a blocker window before the cap; Yotian appeared once (vs146 s21). Retained, shortened. |

### 10.2 Stopgap retirement — BOTH questions answered (HL3)

**RULE #1.** (a) *Is the original class dead?* **Yes** — 0 confabulations, 0/45 cast-nothing,
508/508 renders correct. (b) *What does the residual tax rehearse NOW?* **Nothing — there is no
residual.** The only remaining risk is DEPRIORITIZATION (wave-32's fourth invented rule,
*"Ornithopter is low priority"*), which is a value judgement, not a belief about cost. So RULE #1
does not become a stopgap aimed at a successor belief; it **demotes to one line of ordinary
sequencing priority**, and its render-lie clauses are deleted outright. That frees the #1 slot.

**Master +1.** NOT retired: N-36c did not ship (3/3 casts still one short). Retained verbatim with
its removal condition restated in the guide's own words.

### 10.3 The new #1 slot: the SACRIFICE seam

It is the leak that traces to a loss (§5.1: 831 s = 44% of the budget that ran out); it is entirely
unaddressed by the current guide; its render string is stable and greppable
(`SACRIFICE ONE OF YOUR OWN CARDS` / `each option is a card YOU will LOSE`, 16 windows); and the
correct answer is a fixed ranking this deck can state as a flat list. Per HL9's low-branching rule a
sacrifice-a-tapped-land decision cannot be hard, and this seat spent up to 137 s on one.

### 10.4 HL3 QUOTED-RENDER-STRING SWEEP — **5 zero-hit strings found in the SHIPPED guide, all repaired**

Every substring the guide presents as something the pilot SEES, grepped verbatim against all 201
`prompt` fields:

| Guide quotation | hits | action |
|---|---|---|
| `Cast Ornithopter (0/2)` | **0** | → `Cast Ornithopter {0} (0/2)` (4 hits) |
| `{2}, Tap: create a 0/1 Pest artifact creature token` | **0** | → `{2}, {T}: Put a 0/1 colorless Pest artifact creature token onto the battlefield.` (48 hits) |
| `{4}, Tap: return target artifact creature from your graveyard to the battlefield` | **0** | → `{4}, {T}: Return target artifact creature card from your graveyard to the battlefield.` (28 hits) |
| `{1}{U}, Tap: put target artifact card from your graveyard on top of your library` | **0** | → `{1}{U}, {T}: Put target artifact card in your graveyard on top of your library.` (11 hits) |
| `Already in pool: {c}{c}{c}` | **0** | Cathodion never died at this seat this corpus — UNEXERCISED, not falsified. Quotation DELETED; the mechanism kept as a plain fact. |
| `Artifacts in play: you N \| opponent N` | 185 | keep |
| `Mana available: N total` | 191 | keep |
| `(printed 0/1)` | 37 | keep |
| `[deals 0 - this attack deals no damage to the opponent]` | 14 | keep |
| `add blue mana for each artifact with Tolarian Academy [cost: Tap]` | 54 | keep |
| `Cast Master of Etherium {2}{u} (N/N)` | 3 | keep |
| `Cast Silver Myr {2}` | 9 | keep |

New strings the revision keys to, all verified present: `SACRIFICE ONE OF YOUR OWN CARDS` (16),
`each option is a card YOU will LOSE` (16), `Draw 1 with Scarecrone [cost: {1}, Sacrifice]` (24),
`Your hand (7 cards), counted by the engine` (7), `Playing every land in this hand would cover the
cost of` (10), `Poison counters (you)` (17), `POISON COUNTER on you` (3), `land: taps for` (139).

### 10.5 CARD-FACT AUDIT — one row per checkable claim, terminal pass over the SHIPPED guide

Adjudicated against `bin/Res/sets/primitives/mtg.txt`, read this session with `/usr/bin/grep`.

| Guide claim (as written) | Adjudicating primitive line | Verdict |
|---|---|---|
| Mox Jet / Mox Sapphire / Ornithopter cost {0} | `mana={0}` on all three | TRUE |
| Ornithopter is 0/2 flying | `power=0 toughness=2 abilities=flying` | TRUE |
| Master of Etherium {2}{U}; P/T = your artifact count; +1/+1 to OTHER artifact creatures | `mana={2}{U} power=* toughness=*`; `auto=lord(other creature[artifact]\|mybattlefield) 1/1`; `anyzone=type:artifact:mybattlefield/... cdaactive` | TRUE |
| "You run four copies" of Master | decklist `Master of Etherium (ALA) * 4` | TRUE |
| Glaze Fiend {1}{B}, printed 0/1 flying, +2/+2 per OTHER artifact entering, until end of turn | `mana={1}{B} power=0 toughness=1 abilities=flying auto=@movedTo(other artifact\|myBattlefield):2/2 ueot` | TRUE |
| Salvage Slasher {1}{B} 1/1, +1/+0 per artifact card in YOUR graveyard | `mana={1}{B} power=1 toughness=1 auto=foreach(artifact\|mygraveyard) 1/0` | TRUE |
| Cathodion {3} 3/3, adds {3} when it dies | `mana={3} power=3 toughness=3 auto=_DIES_Add{3}` | TRUE |
| Silver Myr {2} 1/1, taps for {U} | `mana={2} power=1 toughness=1 auto={T}:Add{U}` | TRUE |
| Steel Wall {1} 0/4 defender, never on an attacker list | `mana={1} power=0 toughness=4 abilities=defender` | TRUE |
| Yotian Soldier {3} 1/4 vigilance | `mana={3} power=1 toughness=4 abilities=vigilance` | TRUE |
| Coiled Tinviper {3} 2/1 first strike | `mana={3} power=2 toughness=1 abilities=first strike` | TRUE |
| Nuisance Engine {3}; `{2},{T}` makes a 0/1 Pest artifact creature token | `mana={3} type=Artifact auto={2}{T}:token(Pest,artifact creature pest, 0/1, artifact)` | TRUE (wording repaired, §10.4) |
| Scarecrone {3} 1/2; `{4},{T}` reanimates an artifact creature from your graveyard | `mana={3} power=1 toughness=2 auto={4}{T}:moveTo(myBattlefield) target(artifact[creature]\|mygraveyard)` | TRUE (wording repaired) |
| **Scarecrone ALSO has `{1}, Sacrifice a Scarecrow: Draw a card`** | `auto={1}{S(scarecrow\|myBattlefield)}:draw:1` | **TRUE and MISSING from the shipped guide** — the pilot used it 3× unprompted (vs158 s22/s27/s30, offer string `Draw 1 with Scarecrone [cost: {1}, Sacrifice]`, 24 renders). ADDED this revision. |
| Akroma's Memorial {7} grants flying, first strike, vigilance, trample, haste, pro-black, pro-red to ALL your creatures | seven `auto=lord(creature\|myBattlefield) ...` lines | TRUE |
| Vedalken Archmage {2}{U}{U} is NOT an artifact; draws on each artifact SPELL cast | `type=Creature power=0 toughness=2 auto=@movedTo(artifact\|mystack):draw:1 controller` | TRUE. Note `mystack`: a LAND DROP is not a spell and does not draw. The guide's *"your free Moxes and Ornithopters replace themselves"* is TRUE (those are spells) but must NOT extend to artifact LANDS. Clarified this revision. |
| Ancient Den {W} / Great Furnace {R} / Seat of the Synod {U} / Tree of Tales {G} / Vault of Whispers {B} are Artifact Lands | `type=Artifact Land` on all five | TRUE |
| Island, Swamp, Academy Ruins, Tolarian Academy are NOT artifacts | Academy Ruins and Tolarian Academy are `type=Legendary Land` | TRUE — independently confirmed by the engine's own count, §11 |
| Tolarian Academy makes one {U} per artifact | `auto={T}:name(add blue mana for each artifact) foreach(artifact\|myBattlefield) add{U}` | TRUE |
| Academy Ruins puts an artifact from your graveyard on top of your library for `{1}{U},{T}` | `auto={1}{U}{T}:moveTo(mylibrary) target(artifact\|mygraveyard)` | TRUE. **The guide OMITTED that Academy Ruins also taps for {1}** (`auto={T}:Add{1}`) — an omission, not a false fact; added. |
| The deck is 48 artifacts (35 nonland + 13 artifact lands) | decklist sums to 60 with 13 artifact lands | TRUE |

**CLASS SWEEP.** The corrected false-fact class this wave is *"an activated ability's cost written
as `Tap` rather than `{T}`, and its effect paraphrased rather than quoted"* — grepped across the
WHOLE guide, **three instances** found (Nuisance Engine, Scarecrone, Academy Ruins), **all three
repaired**, not just the one that produced the finding. No frequency or rarity claim appears in the
revised guide (HL6).

---

## 11. NEW DEFECT — a NON-artifact land renders with NO type tag, and the pilot counts it as an artifact

Twice in the vs105 game the pilot stopped, recounted the artifact total against the battlefield
line, disagreed with the engine, and burned the reply reconciling it.

Render, vs105 s11, against `Artifacts in play: you 5`:

`Your battlefield (creatures: 2): Tolarian Academy; Ornithopter {0} (0/2) [artifact] [flying]; Mox
Jet {0} [artifact]; Mox Sapphire {0} [artifact]; Glaze Fiend {1}{b} (2/3) (printed 0/1) [artifact]
[flying]; Vault of Whispers [artifact]`

Six items, five tagged `[artifact]`, one — `Tolarian Academy` — carrying **no tag at all**. The
engine's 5 is CORRECT (`type=Legendary Land`). The pilot's is not:

> *"Total = 6 artifacts. The prompt says 'Artifacts in play: you 5'... Perhaps Tolarian Academy is
> not counted as an artifact in the summary? Or maybe the prompt's summary count is slightly off?"*

Repeated at s23 (`you 8`, nine items listed): *"That is definitely 9. However, the prompt explicitly
states 'Artifacts in play: you 8'."*

**Mechanism.** On the battlefield line, artifact lands get `[artifact]` and plain lands get NOTHING
— confirmed on a board holding a basic, vs158 s23: `... Mox Sapphire {0} [artifact]; Island;
Scarecrone {3} (1/2) [artifact]; ...`. The SACRIFICE menu emitter for the SAME permanents DOES tag
them (`Swamp [land] [your battlefield] [tapped]`, `Tolarian Academy [land] [your battlefield]`), so
the two emitters disagree about the same card.

**Cost: 2 windows, no wrong decision** — the guide's read-don't-count rule held both times (*"I must
trust the game state provided in the summary line over my manual count"*). This is wave-32's N-36d
("coherent as artifacts, opaque as lands") with the failure mode INVERTED: the seat no longer
under-reads artifact lands, it now OVER-reads non-artifact lands. Routed to `notes.md` **N-36i**;
the guide gets one NAMING line, never a counting procedure (HL1 rung 4).

---

## 12. ROTATION READ

3/6 with **zero decision-lane regressions**: attackers 20/21 → **30/30**, blocks 14/14 → **10/10**
including the infect windows that used to be this seat's game-losing shape, the {0} class extinct,
and both new losses are CLOCK losses on games whose decisions were correct. Guide-lane work is
nearly exhausted — after this revision the only open guide surface is the sacrifice seam it
introduces. **Recommend HOLD one corpus** to validate the sacrifice section and the retired RULE #1,
then exit-eligible. The card-fact audit HARD GATE is SATISFIED and recorded above (§10.5); the seat
is not blocked on it.
