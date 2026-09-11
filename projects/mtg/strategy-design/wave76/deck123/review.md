# Wave 76 — deck 123 (Intruders of Thraben, WBU token combo) — per-deck review

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260910-141526-final`, SINGLE binary
`archives/wagic-d372a1153-w76step1` (source b4784ed05), `--thinking on`. All six deck123 games are
RERUNS (start epochs 1789074078–1789074163); none is a first-run game. Live guide = the wave-75
edition, installed at 21d2203cf.

**2 wins (vs146 t18 at 9/−20, vs152 t18 at 15/−4), 4 losses (vs162 t12 at 0/21, vs126 t17 at 0/40,
vs130 t18 at −1/20, vs125 t67 at 0/66)** — wave 75 was 1/6.

Seat census: **536 decisions** (ask 415, priority 96, discard 11, attackers 7, blockers 4, reveal 3;
by game 125: 294, 152: 75, 126: 50, 130: 46, 162: 46, 146: 43), 100% native reasoning
(`reasoning_chars` median 5,555, max 19,452), **0 fallbacks, 0 `off_protocol_bytes`, 0
`plan_line_missing`, 0 `protocol_deviation_replies`, 0 `reply_truncated`, 0 `action_before_plan`, 0
`[RE-ASK]` renders, 0 `answer_replaced`**. Gameend sums for this seat: `ask_replays_reserved` 478,
`identical_ask_answers_reserved` 397, `hold_released_turn` 26, `hold_windows_skipped` 283 (cast 272
/ priority 11), `stop_reached_windows_skipped` 18, `crossphase_identical_reputs` 83,
`plan_names_uncastable_zone_card` 41, **`plan_names_stranded_card` 0**, `forced_close_unrecorded` 2
(125, 146), `phase2_answer_recovered` 2 / missing 0, `async_drops` 12,
`reserve_decline_windows_noted` 14 / skipped 2, `main_phase_windows_skipped` 0,
`own_turn_windows_skipped` 6, `repeat_annotated_takes` 0, `force_close_arms_refused/deferred` 0/0.

**Hands are real.** Three of the four losses (162, 126, 130) are games where the seat's own
battlefield line read `of which 0 are creatures` for most or all of the game after its one maker was
answered (Ob Nixilis −2, Starstorm, or never drawn). I do not score the 1/6 → 2/6 swing as a guide
effect either way. What I score is the four decisions below where the seat had a live choice.

## 1. Game by game — the deciding decisions

**vs deck146 — WIN, turn 18 (9 / −20).** Clean. Keeper t14, Alarm t16, one repeat-row take at
**seq 36** (`stop=26; M=8; x18`), lone-maker attack declined at **seq 37** under CHECK -1 while 24
Vampires sat summoning sick, a 6-blocker assignment at **seq 42** that took 0 damage, then 21
attackers for 42 at **seq 58** on an `ATTACK TOTAL` that called itself lethal. CHECK -1, the stop,
the blocking assignment rule and the lethal read all fired correctly in one game.

**vs deck152 — WIN, turn 18 (15 / −4).** The best-played game of the corpus. **seq 82** is a verbatim
CHECK -1 citation (`CHECK -1 applies: Bloodline Keeper is the sole attacker and a maker`) declining
a 3-power attack; **seq 95** rebuilds a stale stop from the current board rather than defending the
old number; **seq 104** sends 11 fliers into a `NOT lethal` total for correct reasons; **seq 118**
closes. One breach inside the win: **seq 52/53**, below.

**vs deck162 — LOSS, turn 12 (0 / 21). Variance, correctly played until the last card.** Doomsayer
resolved t6, Ob Nixilis −2 destroyed it t9, and Underworld Dreams + Ob Nixilis + Dictate of Kruphix
charged 4 life per compulsory draw step from t8. The seat declined **six** reserve-VERDICT rows on
its own turns (**seq 25, 28, 35, 36, 38, 39, 40**, each printing
`VERDICT: taking this row STRANDS Damnation this turn`) — the exact class it broke four times last
wave. Its last card, **seq 64**, is a Tragic Slip cast on `{kills: Shield Sphere}` — a 0/5 defender
that was neither attacking nor blocking, with 5 damage already on the stack. The guide's
`"{kills: <name>}": cast it` has no relevance test; the game was already lost, so I do not pay a
guide edit with it (§4).

**vs deck126 — LOSS, turn 17 (0 / 40). The deciding decision is seq 70/71 and it is a STRATEGY error
against a printed gate.** Board at t14: their Sanguine Bond in play, `LIFE-TO-DAMAGE CONVERTER on the
battlefield: theirs` printed, Exquisite Blood already revealed in their hand, and exactly two of
their creatures — Perimeter Captain (0/4) [defender] and a lifelink Vampire (1/1). Row 2 printed
`{right now: they control 2 creatures - they choose which one; YOU control 0 creatures - targeting
yourself does nothing}`. The reply was `PLAN: Remove the 1/1 Vampire token with Devour Flesh ... /
CHOICE: 2`, then at **seq 71** `PLAN: Target the opponent to remove a creature without triggering
Sanguine Bond against myself. / CHOICE: 1 (The opponent)`. The opponent handed over the 0/4 wall,
**gained 4**, and Sanguine Bond took **4 off the seat** (21 → 17). Three separate live guide rules
forbade it: the printed-strings entry (`they choose which one` → no edict at the opponent), the plan
naming a victim, and `Never at the opponent ... under "LIFE-TO-DAMAGE CONVERTER ... theirs"`. The
seq-71 reasoning shows the misconception underneath all three: the seat believes naming the opponent
is the way to *avoid* giving life, when Devour Flesh pays the life to the player it names. Edit 1
and HIGH-1 both exist for this seq.

**vs deck130 — LOSS, turn 18 (−1 / 20). Variance.** Keeper t7, one Vampire t9, Starstorm killed both
at t12, and the last five turns of hand are five lands. The seat's only discretionary spend is
**seq 70–72 (t10, at 20 life)**: Devour Flesh naming Yourself with the Pyrite Spellbomb ability
already on the stack targeting the Vampire, sacrificing that Vampire for 2 and fizzling their
Spellbomb. It kept the maker when asked which body (**seq 72**), as the guide requires. The guide's
own condition for the self-edict — `Take it when the LIFE, not their board, is killing you` — was
not met at 20 life; the improvisation was reasonable and the game was lost to the draw, so no edit.

**vs deck125 — LOSS, turn 67 (0 / 66). The corpus's marathon and the seat's largest misplay is
seq 418.** Deck125 countered or exiled the seat's maker **five** times (Cancel ×2, Essence Scatter
×2, Path to Exile) and the seat spent turns 26–51 on a board reading `of which 0 are creatures`.
Two decisions matter:
- **seq 228 (t36) and seq 442 (t56) — the bait rule never fired.** At seq 228 the seat had 10
  untapped sources, a menu with `Cast Lightning Greaves {2} {right now: you control 0 creatures -
  this equips nothing}`, a dead `Cast Idyllic Tutor` bracket and `Cast Bloodline Keeper`; their
  untapped sources read **10 ({u}{w})** and their graveyard, enumerated in the seat's own reasoning,
  held Essence Scatter and Cancel. It cast the Keeper first; Essence Scatter took it. At seq 442,
  with 16 sources, the same two dead rows, Dream Fracture and Path to Exile in their graveyard and
  **12 untapped sources**, it cast Thraben Doomsayer first; Path to Exile took it. The reasoning at
  both seqs walks the P-list mechanically (`P1 ... P2 ... P3 dead ... P4 dead ...`) and never reaches
  the bait clause, which the wave-74 guide filed as a subordinate sentence *inside* the P1/P2 entry.
  Edit 2 promotes it to a gate above the list.
- **seq 418 (t54) — the stop rule overrode the model's own correct read.** With Doomsayer, Intruder
  Alarm, 15 sources and their board showing **Lightmine Field**, 2 Elixirs of Immortality and 2 Staff
  of Nin at 61 life, the reasoning states the position exactly: *"If I attack with 63, Lightmine
  Field deals 63 damage to each. My tokens are 1/1, they die. Doomsayer is 2/2, dies. This is a
  problem! ... How do I deal with Lightmine Field? I can't remove it."* — and then: *"Maybe I should
  just follow the plan: build the army. The prompt says 'Take the REPEAT ROW ... with N = (L + C + 3)
  - M'."* It took `Create human with Thraben Doomsayer x62`, reaching 64 bodies. **seq 424** then
  correctly answered `ATTACK: none` (tokens summoning sick, CHECK -1 on the lone Doomsayer). On t55
  the opponent Path-to-Exiled the Doomsayer at upkeep and cast **Supreme Verdict** in main 1; 63
  tokens died having dealt 0 damage, on a board where the opponent had discarded Supreme Verdict at
  cleanup on **t21, t33 and twice on t51** plus a Final Judgment. The CHECK 0 attack-trigger rule
  (wave-75 edit 2) worked — it is why the seat did not repeat wave 75's 100-attacker suicide — but
  nothing in the guide told it not to *build* the army CHECK 0 had just forbidden it to send. Edit 3
  exists for this seq. I am not claiming it wins the game; at 13 life against 61, two Elixirs and two
  Staffs it does not. It is the play.

## 2. Engine / interface / card items

**HIGH-1 — the Devour Flesh row never states that naming the opponent GIVES THEM the life.** Repro:
`1789074103-ai_baka_deck123-0x56537c4403a0-vs-ai_baka_deck126.jsonl` **seq 70**, row 2:
`Cast Devour Flesh {1}{b} {right now: they control 2 creatures - they choose which one; YOU control 0
creatures - targeting yourself does nothing}`. The annotation prices the *self*-target branch fully
(`targeting yourself sacrifices one of them, your choice, and you gain its toughness` in the 130
seq 70 render) and prices the *opponent* branch not at all — no life figure, no converter clause,
though `LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond` is printed 40 lines
above. The seq-71 reply reasons from exactly that asymmetry (`Target the opponent to remove a
creature without triggering Sanguine Bond against myself`) and hands the loop deck 4 life, which
Sanguine Bond converts into 4 damage to the seat. Same gap, no converter, at
`...vs-ai_baka_deck152.jsonl` **seq 52** (`PLAN: Cast Devour Flesh to gain life before combat`,
target: the opponent). **Suggested fix**: mirror the self-branch — for the opponent branch print
`targeting the opponent: THEY choose which of their N, and THEY gain its toughness (you gain
nothing)`, and under a converter of theirs append the damage that life becomes. The engine already
computes both halves; only the self-branch is rendered. This is the single highest-value render
change for this deck: the edict misread is now four waves old and has survived two guide restatements.

**MED-2 — the ATTACK TOTAL blocker count and the `At least N damage lands` floor are computed from
their UNTAPPED creature count, not from the count that can LEGALLY block the attackers.** Repro 1:
`...vs-ai_baka_deck152.jsonl` **seq 104** — every one of the 11 attackers is `[flying]`, their three
untapped creatures are Intrepid Adversary (4/2, no evasion), Briarbridge Tracker (2/3, vigilance) and
Elite Spellbinder (3/1, **flying**), and every per-attacker tag correctly lists **only** Elite
Spellbinder. The aggregate lines nonetheless read `They have 3 untapped creatures able to block;
declaring more than 3 attackers leaves at least (your attackers - 3) of them unblocked` and
`At least 16 damage lands whatever they block` (23 − 3 − 2 − 2). The true floor is 23 − 3 = **20**.
Repro 2: **seq 118**, 9 flying attackers, their two untapped creatures are Briarbridge Tracker (3/4,
no evasion) and Sigarda (4/4, flying); the line says `2` and the floor says 14 where it is 16. The
error direction is conservative on the floor (it understates what lands) but the "N untapped
creatures able to block" sentence is simply false, and the same code underwrites the
`Their N untapped blockers can cover every attacker` cover sentence, which would be false rather
than conservative on a board with fewer legal blockers than untapped bodies
(`...vs-ai_baka_deck146.jsonl` **seq 37** prints `Their 4 untapped blockers can cover every attacker
you could send` against a lone flier that only the 2 reach Spiders could block — true here, one
blocker short of false). **Suggested fix**: count blockers through the same legality filter the
per-attacker tag already uses.

**MED-3 — Q13's cross-phase re-put bracket almost never prints: 2 prints against 83 counted
re-puts in this seat.** Repro: `crossphase_identical_reputs` sums to 83 for the six games (125: 51,
130: 10, 126: 8, 162: 6, 146: 4, 152: 4); the string `nothing on the board has changed` appears
**twice** in 536 prompts — `...vs-ai_baka_deck130.jsonl` **seq 15** and **seq 69**, both reading
`[this exact list was put to you 1 window ago at Main phase 2 and you declined; nothing on the board
has changed]`. CQ F4's warning is confirmed from the deck that had the most re-puts in wave 75: the
BOARD-key conjunct is strict enough that 97.6% of the counted re-puts render no bracket at all. The
older prompt-only note (`you declined this exact list N times already this turn`) still prints
normally, 242 times, reaching 7 in the 130 game. Recommend loosening the board key (ignore life
totals, counter counts and tapped state) or printing the bracket on the re-put count alone.

**LOW-4 — the repeated-activation completion line lands on the wrong turn.** Repro:
`...vs-ai_baka_deck125.jsonl` **seq 442** game log: the 62-repeat activation taken at
`Turn 54 - Phase: Upkeep` is reported as `Your repeated activation ran 62 of the 62 times you named`
under `=== Turn 55 - opponent's turn === - Phase: Upkeep`, after the tokens themselves are already
logged on turn 54. Harmless here, but a model reading the log at t55 sees its own action attributed
to the opponent's turn.

**Render clauses I checked and found TRUE.**
- **Q4b (the wave-75 MED-1 fix): PASS.** `...vs-ai_baka_deck125.jsonl` **seq 424** —
  `At least 2 damage lands whatever they BLOCK, but that floor is over their BLOCKS ONLY: Lightmine
  Field fires on your declaration, before any combat damage, and can remove attackers from that
  total - so it is NOT a floor on what lands`. The unqualified floor sentence that licensed wave 75's
  seq-1208 board wipe is gone on exactly the board that produced it. 7 `ATTACK TOTAL` renders in the
  seat, all arithmetically exact.
- **Q9: PASS, and the wave-75 under-count question is moot.** 14 prompts printed
  `VERDICT: taking this row STRANDS <card> this turn`; the stranding row was declined **14 of 14**
  (126 seq 38/55/58, 146 seq 22, 152 seq 36/58/63, 162 seq 25/28/35/36/38/39/40).
  `plan_names_stranded_card` is 0 and there were 0 real events — consistent, not blind.
  `plan_names_uncastable_zone_card` 41: I sampled 11 and all 11 are real (plans naming a Bloodline
  Keeper that is in the graveyard after a counter, or an Intruder Alarm already on the battlefield);
  0 were self-recursion or cast-from-exile exemptions.
- **Q1: 0 false NEW in this seat.** 433 `[hold check: ...]` brackets, 358 claiming "every row above
  was also on the menu", 75 claiming N new. Comparing each claim against the previous record at the
  same turn+seam (26 comparable pairs) every mismatch is an UNDER-claim, i.e. the engine treats an
  annotation-only change as the same row exactly as the bracket says; no bracket claimed a row was
  new that was not.
- **Q3: PASS.** 3 `LOOP COMPLETE` renders (126 seq 74/75/80, all cast windows) and 4
  `NOT lethal: block only where the trade favors you` blockers headers (146 seq 21/42, 152 seq
  62/105); **0 co-occurrences**, and no `reaches 0 in N more turns` render in the seat.
- **Q5: 24 `{crack-back cover:}` renders, arithmetic true on the 4 I recomputed** (126 seq 70/73,
  146 seq 25, 152 seq 36). The 126 seq 70 cover correctly reports a FLOOR of 0 where one of their
  two bodies is not in the crack-back total. No regenerating or returning body appeared in the seat,
  so the "not subtracted" half is untested here.
- **Q2: max prompt 23,051 bytes (125 seq 423), 69 of 536 prompts over 20 KB.** 12 `- THEIRS:`/YOURS
  rosters, all carrying count + shapes; the 64-body one at 125 seq 423 collapses correctly
  (`64 bodies: Thraben Doomsayer (2/2), Human #1-#63 (1/1) x63`). No roster needed `and N more`.
- **P19 hold-row size: 444 hold rows, median 597 B, max 727 B, exactly one over 650.**

**UNTESTED from this seat (0 renders in 536 decisions):** Q6 (`off YOUR life` on a forced-sacrifice
row — 0), Q10 (`{removes:}` / ghostform — 0), Q11 (up-to-N loyalty rows — 0), Q14 (3-life land usable
tag — 0), Q15 (`forced_close_prefill_echo` — 0 off-protocol bytes at all), P13
(`main_phase_windows_skipped` 0, fourth wave running), P14 (0 `[RE-ASK]`, 0 `repeat_count_reask`),
P18 MDFC faces (sixth wave with no MDFC in a deck123 hand), ANNOUNCE_X (0 windows),
`sibling_window_asks_skipped` / `chain_windows_collapsed` (0 again, same structural reason as waves
74–75: every chain row here costs Tap, which the mana policy always affords).

## 3. Which wave-75 items recur

| wave-75 item | wave-76 status |
|---|---|
| the reserve-VERDICT override (4× in wave 75, MED-2 + edit 4) | **CLOSED. 14 of 14 declined**, 0 overrides. The `YOUR OWN UPKEEP AND DRAW STEP` section is the wave's clearest success; 162 alone declined six of them while dying. |
| MED-1 the `At least N damage lands whatever they block` floor under a declaration-time trigger | **CLOSED by the CO lane** — verified on the same Lightmine Field board (125 seq 424). |
| edit 2, CHECK 0's `AN ATTACK TRIGGER ON THEIR SIDE` | **FIRED, and visibly.** 125 seq 418's reasoning derives the whole Lightmine Field consequence from it and refuses the all-in. It did not reach the *build* decision — edit 3 below. |
| edit 3, the `(your attacker dies, their blocker lives)` tag | **UNTESTED** — the tag rendered on no A-row in this seat's 7 attackers windows. |
| the edict gate at N ≠ 1 | **RECURS, three times** — 126 seq 70/71 (N = 2, decisive under Sanguine Bond), 152 seq 52/53 (N = 2), 146 seq 25 (N = 2, Tribute, benign). Four waves running, and the wave-75 first-match list did not stop it. The mechanism is now diagnosed (HIGH-1): the seat does not know who gains the life. |
| the Tragic Slip `kills 0 of the N` / `- SURVIVES` pair | **Does not recur.** Every Slip cast in the seat named a row printing `{kills: <name>}` or `so Morbid applies`. |
| the Alarm-on-an-empty-board cast (4× in wave 75) | **Does not recur.** 0 casts of Intruder Alarm on an `of which 0 are creatures` line; the 125 and 126 games decline it by name, repeatedly, citing the string. |
| CHECK -1 (the lone maker) | **Held, 3 of 3** (125 seq 424, 146 seq 37, 152 seq 82), one of them a verbatim citation. |
| MED-3 the hold-check volume | Volume unchanged (433 brackets), **0 false NEW**; the new cross-phase bracket replaces it as the item — see MED-3 above, which is the opposite defect (under-print). |
| LOW-4 Lightmine Field's N×1-damage narration | Not re-observed (no attack into a Field this corpus); docketed for the card-script lane per the brief. |
| — new — | the bait-cast gate never firing (2×, both decisive for that game), the stop rule building into a declaration-scaling punisher (1×), the Devour Flesh who-gains-the-life misread (2×). |

## 4. Guide verdict: **EDIT**

Revised guide at `wave76/deck123/strategy.txt` — **19,988 bytes** (live guide 19,995; ceiling
20,000). A332 grep (`seq`, `wave`, `corpus`, `game N`, turn citations): 0 hits. `YOUR PLAN`: 0.
`even when` / `even if` / `regardless of` / `no matter` / `however` (A334 audit): 0. Both PLAN
templates are one physical line each. Headings declare scope (A333); the one new heading-level line,
`BEFORE THE LIST, ONE GATE`, states its own scope in its first four words and sits inside
`WHAT TO CAST WHEN SEVERAL ARE LISTED`.

**Which rules earned bytes.** Three rules are added, ~830 bytes of new rule; the guide *shrinks* by
7 bytes net. Edit 2 is a relocation that is close to byte-neutral on its own. The rest is paid by
deleting text the new rules supersede and by prose compression that removes no rule (listed last).

| # | before → after | seq that paid for it |
|---|---|---|
| 1 | **`THE PRINTED STRINGS` edict entry gains its consequence.** `... on an edict row: no edict at the opponent. Only "<name> is sacrificed" (N of 1) is a cast.` → `... no edict at the opponent - they hand over their worst body and keep the one you meant, and Devour Flesh pays the life to the player you NAME. Only "<name> is sacrificed" (N of 1) is a cast.` The verdict was already there and was already read past; what was missing at the seam was *why*, and specifically who the life goes to. Paid for by deleting the same verdict where it was restated in `THE TWO EDICTS`' opening paragraph (`N NOT EXACTLY 1 means no edict at the opponent - at 2 or more they bin their worst and take the life, and the only use left is naming Yourself, below`), which the list now carries. | **126 seq 70/71** (N = 2 at the opponent under a converter: they gained 4, Sanguine Bond took 4), **152 seq 52/53**, **146 seq 25**. **+~150 / −~200.** |
| 2 | **The bait-cast rule promoted from a subordinate clause to a gate above the priority list.** New first line of `WHAT TO CAST WHEN SEVERAL ARE LISTED`: `BEFORE THE LIST, ONE GATE: a counterspell in their log or graveyard, their untapped sources at 2 or more, and mana on this menu for TWO rows - then the FIRST row you take is a DEAD one (a Greaves that equips nothing, a dead Tutor, a spare Alarm) and the maker is the SECOND cast of the same main phase. The dead row is the bait; skipping it is how the maker dies.` The P1/P2 sentence that pointed at another section is DELETED, and `THE ONE TIME A DEAD ROW IS A PLAY` in the Tutor section shrinks to one line pointing back at the gate. It keys on two strings the window prints (`Their untapped sources:` and the dead-row text) instead of on a memory of their deck; "or sweeper" is dropped from the trigger because a sweeper does not answer the first spell you cast. | **125 seq 228** (Keeper cast first into 10 untapped {u}{w} with Essence Scatter and Cancel in their graveyard → countered) and **125 seq 442** (Doomsayer cast first into 12 untapped with Path to Exile in their graveyard → exiled). **+~330 / −~470.** |
| 3 | **NEW bullet at the head of `THE TOKEN LINES AND THE STOP`**, above `THE STOP IS L + C + 3`: `WHAT THE ARMY IS FOR IS THE ATTACK STEP. Where CHECK 0 names an attack trigger of theirs, you will never declare more than ONE attacker, so the stop is 1: at M of 1 or more every Create line is "CHOICE: 0 (pass)". Bodies built past it are swept or exiled before they attack.` It keys on CHECK 0's own finding, so the two rules cannot disagree, and it reuses the existing `N of 0 or less is a pass` arithmetic rather than inventing a second formula. | **125 seq 418 → seq 424 → t55**: 62 tokens built, `ATTACK: none` the same turn, all 63 swept by Supreme Verdict having dealt 0 damage, with the model's own reasoning having already concluded the army could not attack. **+~350.** |
| — | **Compression paying for the rest (−~640), no rule removed:** THE DECK / ORDER OF OPERATIONS / LETHAL IS ALREADY ON THE BOARD reflowed; the two HOLDS bullets about a repeated dead list folded into one; `"[second copy: ... changes nothing]" is LEGAL, not USEFUL` dropped (the printed-strings list carries `a second copy changes nothing`); MULLIGAN's opening and the BOTTOMING order compressed to prose; the DAMNATION, LIGHTNING GREAVES, BLOCKING, CHECK -1, CHECK 0, TRAGIC SLIP, Vision Skeins and DRAW PRICE paragraphs trimmed word-for-word. | — |

**What I deliberately did NOT change.** `SWEEPER SEEN` stands untouched although 125 seq 418 breached
it too (four Supreme Verdict sightings in the log, the army built in the seat's own upkeep): edit 3
governs the same window off a string the render prints, and on that board waiting for their end step
would have lost the maker to the t55 Path to Exile instead — I will not rewrite a timing rule on a
board where both branches lose. I did not touch CHECK -1 (3 for 3), the reserve-VERDICT section
(14 for 14), or the printed-strings entries for the Slip and the Alarm, all of which held this wave.
I did not add a relevance test to `"{kills: <name>}": cast it` for 162 seq 64 (a Slip spent on an
idle 0/5 wall): the game was already lost on the stack, the rule's cost is one {B} in a deck that
draws three Slips, and Amendment 332's posture is that a rule must be paid for by a decision that
went wrong. And I did not bend the guide around HIGH-1 — edit 1 is written to stand whether or not
the Devour Flesh render is fixed, because it keys on `they choose which one`, which is true.

## 5. Optional proposals

**None.** All three edits are this deck's own cards and this file's own wording. HIGH-1 and MED-2 are
render defects for the engine seat, MED-3 is a threshold-loosening request, and a skill proposal
restating A332/A333/A334 would be noise. Edit 2 is an *application* of the existing skill rule that a
gate belongs where the decision is made, not in the section that owns the card.

## 6. What I did NOT check

- **Opponent seat files were opened only for board context** (deck125's Lightmine Field, Supreme
  Verdicts, Elixirs and Staffs of Nin; deck126's Sanguine Bond / Exquisite Blood; deck162's draw
  punishers; deck130's Starstorm). I audited no opponent decision.
- **I adjudicated no lane prediction corpus-wide.** §2 answers Q1, Q2, Q3, Q4b, Q5, Q9 and Q13 **for
  this seat's six games only**; the UNTESTED list names what this seat could not reach. Q4's menace
  knapsack proper is untested here (no menace on any attacker of mine); what I found is a legality
  gap in the same sentence.
- **The 2 `forced_close_unrecorded` (125, 146) were not reconciled against stderr**, and the 12
  `async_drops` were not traced to re-asks. `phase2_answer_recovered` is 2 / missing 0 and no
  decision in the seat lacks a record, so I saw no loss; the reconciliation is the engine seat's.
- **The 83 cross-phase re-puts were not individually paired to their answers.** I established the
  bracket print rate (2) and read the two prints; I did not verify that all 83 were answered with the
  same decline.
- **`askreplay/` was not opened**, and the 26 `hold_released_turn` events were not paired to their
  holding windows.
- **I did not open any card script**, run a build, run wagic, or run git; nothing under `bin/Res`,
  `src/` or outside `wave76/deck123/` was touched. LOW-4 is diagnosed from the rendered event log.
- I read every attackers, blockers, discard and reveal window, every window carrying a Cast, Create,
  Equip or Transform row that was taken, and the `reasoning` of the deciding windows; I did not read
  the `reasoning` of all 283 held windows individually.
