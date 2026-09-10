# Wave-74 deck 152 review — BANT HUMAN MIDRANGE

Corpus `matchups-20260909-104713-final`, seat files `...ai_baka_deck152-<ptr>-vs-...`. **MIXED BINARY**: five
of my six games are on `wagic-60e9fe2ad-w74step1b` (CG); the **vs-deck125 game is a wave-74 step-1c rerun on
`wagic-76c5fab24-w74step1c` (CH, hold key = action identity)** — `start_epoch` 1788998133. Every hold row this
seat took is in that one game, so **all hold evidence below is CH-binary only and says nothing about CG**.

264 model decisions (deck123 32 / deck162 26 / deck146 32 / deck126 60 / deck130 44 / deck125 70), **264 with
native reasoning** (`reasoning_chars` min 1,582 / p50 4,766 / max 20,121), `thinking` on in all 264.
0 fallbacks, 0 `answer_replaced`, 0 `reply_truncated`, 0 `action_before_plan`, 0 `wall_miss`, 0
`ask_replays_refused`. `transport` `curl=0,http=200,empty=0` on 264/264. Latency p50 77.5 s / p95 166.7 s /
max 381.5 s; `deadline_pct` p50 8.6, max 42.0. `off_protocol_bytes>0` 11 (4.2%), `plan_line_missing` 12
(4.5%). `async_drops` 7, all re-asked (below). Seat **5-1**. Live guide read = the wave-73 edit
(19,993 B, byte-identical to `wave73/deck152/strategy.txt`).

Gameend counters, six games: `hold_released_turn` 0/0/0/0/0/**2**, `hold_windows_skipped` 0/0/0/0/0/**26**
(cast 26, priority 0), `main_phase_windows_skipped` 10/0/0/24/6/2 = 42, `stop_reached_windows_skipped` 0
everywhere, `sibling_window_asks_skipped` 0, `chain_windows_collapsed` 0, `chain_windows_only_selfharm` 0,
`chain_selfharm_rows` 0, `chain_acting_rows` 0/0/0/0/0/72, `identical_ask_answers_reserved` 0,
`ask_replays_reserved` 0/0/0/6/0/8, `plan_names_stranded_card` 0, `repeat_annotated_takes` 0,
`reserve_decline_windows_noted` 0/2/0/0/0/0, `phase2_answer_recovered` 0/1/0/0/0/0, `phase2_answer_missing` 0.
`own_turn_windows_skipped` 52/0/59/109/22/51.

## 1 — Game by game

| # | Opp | Result | Turn | Life | Deciding decisions |
|---|-----|--------|------|------|--------------------|
| 1 | 123 | **win** | 15 | 20 / 0 | clean curve; seq 26 Moonrage+Sigarda for 8; seq 31 four-attacker lethal; seq 32 Absence on Bloodline Keeper mid-combat |
| 2 | 162 | **win** | 9 | 21 / -1 | seq 17/18 Absence on Fog Bank (unblocks the ground); seq 22 two-attacker 7; seq 27 lethal alpha |
| 3 | 146 | **win** | 16 | 13 / -2 | seq 14 Tovolar's Huntmaster; seq 16 correct **ATTACK: none** (3/3 Katilda vs a 1/1 first-strike deathtoucher, "(both die)"); seq 26/33 Wolf beats |
| 4 | 126 | **loss** | 26 | 0 / 25 | **seq 22 / 27 / 38 — the three attacks that fed their lifegain wall (HIGH-1)**; **seq 37 — Brutal Cathar exiled the ONE defender with no lifegain trigger (HIGH-2)**; **seq 59 — declined a free rung-1 block under their Sanguine Bond (HIGH-3)** |
| 5 | 130 | **win** | 21 | 20 / 0 | **seq 36 — cast Elite Spellbinder off a two-name tap bill and lost the whole attack (MED-1)**; seq 45 three-attacker lethal |
| 6 | 125 | **win** | 27 | 20 / 0 | seq 21 Intrepid Adversary for 2 counters; seq 23 Aspirant(6/6)+Wolf for 11; seq 45/56/69 Moonrage beats; three correct HOLDs (seq 47/51/62) |

**Game 4 in full (the loss).** Bant board vs a mono-defender lifegain deck: Perimeter Captain (`mtg.txt:85398`,
`@combat(blocking) source(creature[defender]|mybattlefield):may life:2 controller`), Pride Guardian
(`mtg.txt:88356`, `@combat(blocking) source(this):life:3 controller`), lifelink Vampires, later Sanguine Bond
(`mtg.txt:99004`) — all three faithful to Scryfall's Oracle text. The seat's only unblockable damage was
Sigarda (4-5/turn). It sent Augur of Autumn alongside her on turns 11, 13, 15, 17 and 19. Turn 17 is the
decider: opponent at 15, `ATTACK TOTAL ... up to 23 life back across their 5 blockers, so blocking can leave
them as high as 34`; the seat sent A1+A2, they gang-blocked Augur with four defenders, and the log in seq 34's
prompt reads `Opponent gained 3 life (now 18) [x11 ...]` then `Sigarda dealt 4 damage to the opponent
(now 33)`. **Opponent 15 -> 33 in one combat the seat chose to make.** From there the guide's own converter
line was unreachable arithmetic. The seat *did* self-correct from turn 21 on (seq 50/56/63 send unblockables
only) — too late.

## 2 — Engine / interface / card items

### HIGH-1 (STRATEGY, guide) — the "as high as N" clause was read aloud and reasoned past, four times
`1788968865-...-vs-ai_baka_deck126.jsonl` seq 16 (T11), 18 (T13), **22 (T15)**, **27 (T17)**, **38 (T19)**.
Rendered line at seq 27: `ATTACK TOTAL: 2 attackers listed, 7 total combat damage to a player - ... At least
4 damage lands whatever they block ... - up to 23 life back across their 5 blockers, so blocking can leave
them as high as 34.` The seat's `reasoning` quotes it verbatim and then writes
`"blocking can leave them as high as 34" - this is a warning about blocking triggers. But attacking is still
the best play ... They won't block with all of them unless they want to.` Surface TRUE and complete (actual
result 33, inside the stated bound), so this is STRATEGY, not render. **The render is doing its job; the guide
rule was not applied.** Note the live guide's test — `N above 0 -> send ONLY the unblockable A-lines` — is
*vacuous*: N is a life total, so "above 0" is true on every non-lethal attack, and applied literally it would
also have vetoed the correct seq 16 attack (N 16 vs their 18). Guide edit 1 replaces it with the compare the
render actually supports. Cost: the game.

### HIGH-2 (STRATEGY, guide + MED render half) — the carried plan chose the removal target over the list in front of it
Same file, **seq 37** (T19, Brutal Cathar's exile target). The eight rows include four life engines
(`Perimeter Captain #1 (0/4) [defender] - "Defender -- Whenever a creature you control with defender blocks,
you may gain 2 life."`, `Pride Guardian #1 (0/3) ... "you gain 3 life."`, +1 of each) and one vanilla
`Wall of Omens (0/4) [defender] - "Defender ... When Wall of Omens enters, draw a card."`. The reasoning gets
it right — `Perimeter Captains and Pride Guardians are defenders. Exiling them stops their blocking triggers.`
— and then discards its own read: **`Given the plan explicitly says "exiling Wall of Omens", I will follow
that.`** The plan was written one window earlier, before this list existed. Two turns later that wall of
untouched lifegain defenders took them from 36 to 58.
**Render half (MED):** the REVEAL windows in this same game carry the de-binding sentence — seq 51/57/65 all
print `The PLAN carried above was stated at an EARLIER window; this window's answer is a card number off the
list above, and no action that plan names can be taken here` — and the **TARGET-CHOICE window does not**.
Repro: seq 51 prompt (has it) vs seq 37 prompt (same shape of ask, no such line). Extending that clause to
target-choice asks is one string and closes a demonstrated plan-lock.

### HIGH-3 (STRATEGY, guide) — a free rung-1 block declined under their converter
Same file, **seq 59** (T24, seat at 5 life, opponent 43 with Sanguine Bond out). One attacker,
`A1. Vampire (1/1) deals 1 [lifelink]`; six blockers all read
`may block A1 (you kill it, your blocker lives) {their attacker's lifelink, this block: they gain 1 from this
block only, and this attacker deals nothing to your life}`. Answer: `BLOCKS: none`. The reasoning is
arithmetically correct and strategically wrong: it computes that the seat loses 1 either way (damage, or the
converter echoing their lifelink gain) and concludes `blocking is strictly worse or equal ... Definitely
BLOCKS: none` — never pricing the **removal**, which was the whole point: the block kills the Vampire and
ends a recurring 2-life-per-turn drain. The seat died at 1 life two turns later. The guide's block ladder
already says rung 1 is `TAKE IT, at every life total, always` and its `BLOCKS: none` gate already forbids
this answer; what the guide did not have is the converter case that generated the false tie. Guide edit 3.

### MED-1 (STRATEGY, guide) — the tap bill was FOUND this wave, quoted, and disobeyed
`1788968874-...-vs-ai_baka_deck130.jsonl` **seq 36** (T19, Main 1). Row 2:
`Cast Elite Spellbinder {2}{w} (3/1) {leaves 0 of your 3 untapped mana sources untapped - casting this taps
you out} ... {paying this taps: Brutal Cathar, Luminarch Aspirant - they cannot attack this turn}`. The
reasoning **does** quote it (`The prompt says "paying this taps: Brutal Cathar, Luminarch Aspirant"`) — so
wave-73's SEARCH step worked — and the reply is
`PLAN: Cast Elite Spellbinder to disrupt their hand, then attack with Brutal Cathar and Luminarch Aspirant.`
i.e. a plan the row had already ruled out on its face. Two names in Main 1 is a PASS by the live guide. The
board line goes `3 are creatures, 2 of them able to attack right now` (seq 36) -> `4 are creatures, 0 of them
able to attack right now` (seq 37); no attack that turn, and the Aspirant's counter then went onto a
summoning-sick body for want of anything that could connect (seq 38). Opponent was at 9; the seat won two
turns later anyway. **This is the wave-73 MED-2 failure one step further along**: last wave the model never
found the clause, this wave it found it and did not obey the count. Guide edit 4 makes the count an
instruction rather than a step.

### MED-2 (ENGINE, meter) — `plan_line_missing` / `off_protocol_bytes` are still charging a present, correct plan
12 `plan_line_missing`, 11 `off_protocol_bytes>0`, 10 of them the same records. Ten of the twelve are one
correct single-sentence plan with the `PLAN:` label dropped: `Attack with all creatures to win.` (deck162
seq 26, 33 B) · `Play Branchloft Pathway, cast Sigarda, Champion of Light, attack with Sigarda and Augur of
Autumn.` (deck126 seq 20, 98 B) · `Cast Katilda, Dawnhart Prime to establish the mana engine, then attack
with Brutal Cathar and Luminarch Aspirant.` (deck130 seq 35, 113 B). Two are a genuine missing plan (bare
`CHOICE: 2 (Katilda, Dawnhart Prime #2)`, deck146 seq 28; deck126 seq 33). **One** record is real off-protocol
prose: deck126 **seq 50**, 621 B, a paragraph replacing the PLAN line — and it is the seat's single best
decision of that game (`Sigarda ... is unblockable ... This is the only way to deal damage safely`), i.e. the
meter's largest reading this wave is on its most correct answer. Also worth the engine seat: a lowercase
`Plan:` line parses (deck126 seq 27, not counted missing) while a label-less line does not.

### MED-3 (ENGINE, meter) — `main_phase_windows_skipped` still over-counts, and the shape is unchanged
42 across six games, 24 of them in the vs-deck126 game whose every own main phase contains a real Casting
record. Example: deck126 T19 is counted while seq 34/35/36/37 are all Main-phase-1 casting decisions in that
same turn. Same per-phase-id scoping wave 73 reported at 47; the rewire has moved the corpus total but not
this seat's semantics. Engine seat owns it.

### LOW-1 (guide gap) — the attack tag ladder has no rung for `(both die)`
deck146 seq 16: `A1. Katilda, Dawnhart Prime (3/3) [their untapped blockers: Triumphant Adventurer (1/1)
(both die)]`. The guide's per-creature list covers `(you kill it...)`, `(neither dies)` and
`your attacker dies...` but not `(both die)`; the model reasoned it out unaided and correctly held a 3/3 lord
back from a 1/1 first-strike deathtoucher (`ATTACK: none`). No cost, no edit — recorded because the next
`(both die)` may not be so lopsided.

### Verified NOT defects this wave
- **The O2 attackers-window repair HOLDS on this seat.** Across all six games there are 34 own turns whose
  board line reads `N of them able to attack right now` with N > 0; **33 have an attackers record**. The one
  that does not — deck130 T19 — is legitimate and traced: the seat tapped Brutal Cathar and Luminarch
  Aspirant for mana (MED-1), the board line duly falls to `0 of them able to attack right now` at seq 37, and
  `game-152v130-1788968870.stderr:1593` prints `[combattrace] t18 ph6 seat=0x5580c48ad630 combat decision
  suppressed by no-legal-attacker`. **Zero silent skips — wave-73 HIGH-1 does not recur on this seat.**
- **Holds (CH binary only, vs-deck125).** Three taken (seq 47 T23 Blockers, seq 51 T23 Main 2, seq 62 T25
  Main 2), all on menus whose only non-pass row was `Cast Fateful Absence ... - the only legal targets are
  YOURS` — no activated or loyalty ability above the hold, so the wave-72 rule is respected. Both latches
  released at the holder's own untap: `game-152v125-1788998131.stderr:1631` (`taken on turn 22 (own turn),
  now turn 24`) and `:1767` (`taken on turn 24 ... now turn 26`). **No hold outlived its holder's untap.**
  26 windows saved, all of them the same dead menu.
- **No loop.** The maximum `[you declined this exact list N times already this turn]` in 264 prompts is **5**
  (deck125 seq 74, T27 Blockers), and the tagged decline row was never *taken* (`repeat_annotated_takes` 0).
- **`async_drops` 7, no decision lost**: `casting/board/re-asked` x4 (deck123 seq 32, deck126 seq 21/36/47),
  `casting/question and board/re-asked` x3 (deck125 seq 22/66/71). Every one re-asked, all seven answered by
  the model, none fell to the heuristic.
- **O1 ghostform gloss TRUE here** (the one instance in my seat): `...vs-deck146` seq 22 log,
  `Triumphant Adventurer got a ghostform (when this creature dies or is exiled its owner returns it to hand
  and creates a 1/1 white flying Spirit - killing it is not removal) counter` — matches Kaya the Inexorable's
  +1 exactly, and the grant was live.
- **O16 `(keeping 7)`** printed on all six first mulligan windows; all six hands were untouched sevens, all
  kept, all with two-plus lands.
- **O4 MDFC land drops**: every Pathway land drop offered both faces on the same menu (23 such menus across
  the six games), and no drop was double-decided.
- **Land drop never declined**: 0 takes of `Play no land right now`.
- Perimeter Captain, Pride Guardian, Sanguine Bond, Kaya the Inexorable checked against Oracle text — all
  faithful.

## 3 — Did wave 73's edits pay off?

- **Edit 1 (SEARCH for "paying this taps") — HALF PASS.** The clause is now found and quoted (deck130 seq 36)
  where last wave it was invisible; the *count* rule was then ignored. Edit 4 below takes the next step.
- **Edit 2 (Teferi's +1 cascade described as rendered) — UNTESTED.** Teferi was cast once in six games
  (deck125 seq 15) and `was COUNTERED by Dream Fracture`; he never reached the battlefield, so no stage menu
  fired.
- **Wave-73 HIGH-1 (silent attackers window) — FIXED** (see §2 Verified).
- **Wave-73 MED-1 (off-protocol is one shape) — RECURS**, 10/12.
- **Wave-73 LOW-1 (`main_phase_windows_skipped` scoping) — RECURS.**
- **Mulligan / bottoming / cleanup discard / Sigarda coven / Moonrage ward** — coven fired three times this
  wave (deck126 seq 51/57/65; each pick is a tied-cheapest castable Human — Luminarch Aspirant over Katilda,
  Elite Spellbinder over Brutal Cathar, Elite Spellbinder again — and Sigarda's own legend-tagged row, marked
  `[does NOT qualify - goes to "put on bottom"]`, was correctly never named); bottoming, cleanup discard and the Moonrage ward ask all fired **zero** times, third wave running.

## 4 — Guide verdict: **EDIT**

Revised guide at `wave74/deck152/strategy.txt`, **19,992 B** (live 19,993; ceiling 20,000). Amendment-332
clean (no wave/seq/game/corpus citation, no counts, no history); the guide carries no PLAN template and
quotes no reply label.

**Edit 1 — the "as high as N" test becomes a COMPARE, not a tautology.** Paid by deck126 seq 22, 27, 38
(HIGH-1), and checked against seq 16/18 where the old literal rule would have been *wrong*.
before: `- "...blocking can leave them as high as N", N above 0 -> send ONLY the A-lines tagged "[no creature
they control can block this attacker]", nobody else. ... and N is what they end at.`
after: `- "...blocking can leave them as high as N": COMPARE N TO THE LIFE ON THEIR LINE RIGHT NOW. N AT OR
ABOVE IT means the attack can leave them HEALTHIER than they are now - send ONLY the A-lines tagged "[no
creature they control can block this attacker]", nobody else. ... N BELOW their current life is an attack that
still gains ground: send the blockable bodies too, priced by the per-creature tags below. Re-run that compare
EVERY combat, since N climbs as their board grows - a swing that was fine two turns ago is how an opponent at
15 reaches 50.`
Why this shape: the old test could not discriminate (every non-lethal attack passes "N above 0"), which is
almost certainly why the model reasoned around it rather than with it — and it made the "Otherwise send each
listed creature..." branch below unreachable. The new test is two numbers both printed on the same screen,
and it reproduces exactly the seat's own later, correct behaviour while forbidding the three attacks that lost
the game. The trailing sentence exists because the seat passed this test at T11/T13 and failed it at T15/T17
on the *same* two attackers: the number moves, so the check has to be re-run.

**Edit 2 — name the life engine, and take the target off the list rather than off the plan.** Paid by deck126
seq 37 (HIGH-2).
before: `- LIFEGAIN / FLIERS: kill or exile a lifelinker on sight ... could have blocked.`
after: same, plus `THEIR LIFE ENGINE IS WHATEVER THE ROWS SAY GAINS THEM LIFE, not just lifelink: a
"[defender]" whose text reads "whenever ... blocks, you gain N" IS the engine, and a defender with no such
line is the WORST exile on the list. When Brutal Cathar or Fateful Absence opens a target list, take the row
with the biggest gain-on-block number - and take it FROM THE LIST IN FRONT OF YOU, since a plan that named a
target was written before this list existed and does not choose here.`
Why this shape: the model's own reasoning had already identified the right targets; what beat it was the
carried plan. So the rule has to carry both halves — which row, and that the plan does not get a vote at a
target window.

**Edit 3 — blocking under THEIR converter.** Paid by deck126 seq 59 (HIGH-3).
new bullet in the converter section: `* BLOCKING UNDER THEIR CONVERTER: a lifelink attacker costs you the
same life whether you block or not - unblocked it deals it, blocked it gains them that much and the converter
takes it off you - so a block is free and is your only way to REMOVE that attacker. Take every "(you kill it,
your blocker lives)" row, at every life total.`
Why this shape: the whole converter section was written about ATTACKING; nothing in it reached a blocking
window, and the converter is exactly what manufactured the false life-neutral tie the model then broke the
wrong way. Stating the tie *and* its resolution in one sentence is cheaper than trying to out-argue the
arithmetic, which was correct.

**Edit 4 — the tap-bill count becomes an instruction with the disobedience named.** Paid by deck130 seq 36
(MED-1).
before: `... is how a whole attack goes missing. COUNT THE NAMES IN THE BILL: two or more in Upkeep or Main 1
is a pass, whatever the row is; ...`
after: `... is how a whole attack goes missing. THEN COUNT THE NAMES IN THE BILL AND OBEY THE COUNT - quoting
the bill and taking the row anyway is the same lost attack: two or more names in Upkeep or Main 1 is a PASS,
whatever the row is and whatever your plan says you will attack with afterwards; ...`
Why this shape: the observed failure was not perception (the clause was quoted) and not the counting (two
names, plainly). It was the step between the count and the answer, and the plan in the same reply named the
two creatures the bill had just taken away — so the rule now closes on the plan.

**Bytes.** 19,993 -> **19,992**, net **-1 B**. The four edits add ~1,470 B, reclaimed from fifteen places,
none of them a rule: the ONE-LAND-PAYS-ONE-PIP bullet's Teferi example (he has not resolved in two waves);
the land-drop bullet's enumerated non-reasons and the Pathway face table (both derivable from the menu, and
the rule is 43/43 for two waves); the mulligan carried-plan and one-land-test restatements; the counterspell
bullet's closing restatement; the deploy-floor and order-of-operations closing restatements; the Teferi
bullet's "nothing else you run gains life" gloss (untested two waves); the Moonrage ward mislabel gloss (no
ward ask in three waves); the SHIELD SPHERE and FOG BANK named-card asides (neither card has appeared; the
general "(neither dies)" test survives); the two-or-more-blockers worked example; the vigilance/lifelink
aside in the attack STOP tag; the bottoming walk-through; the Lair bullet's explanation of *why* the
animation expires (the PASS rule and the "only when its A-line reads unblockable" rule both survive intact);
and the converter's BOTH-names branch compressed to its one rule (no both-names board in four waves).
**The rule that earned the growth is edit 1** — it is the only rule in this guide whose failure this wave cost
the game, and it costs bytes because a compare needs both of its numbers named.

## 5 — Proposals

**None.** HIGH-2's render half (the plan-de-binding clause missing from TARGET-CHOICE windows) is an engine
string, not an instruction-surface change, and I have written it up as a repro rather than a proposal.
`wave68/strategy-writing-skill-v2.md` already carries the decision-time-only, primitive-verification and
observable-criteria rules this review leaned on; a proposal restating any of them would be noise. The one
transferable lesson I can name — *a rule whose test cannot discriminate (every board passes it) will be
reasoned around rather than applied* — is close enough to the skill's existing observable-criteria material
that I am not going to spend a proposal on it.

## 6 — What I did NOT check

- Any seat but 152's, except the opponent seats read for context; no `.stderr` beyond the six games of this
  seat; I did not count the `{X}`-announcement payment loss, and **no ANNOUNCE_X window fired in this seat**
  (0 occurrences in 264 prompts), so O19 is UNTESTED here.
- The CG-binary hold behaviour: this seat took **zero** hold rows in the five CG games, so my hold evidence
  is CH-binary only. Whether a hold re-opens per trigger-chain link on the CG binary is not testable here.
- I did not audit the F1 pending-deadlock hazard (turns with ticks but no decision and no pass) — that needs
  the stderr tick trace, which the engine seat owns.
- `blocker_forecast_multi` 28 / `_gang` 4 / `_collapsed` 24, all in the vs-deck126 game: I read the four block
  windows that mattered (seq 53/59/67 plus the T17 damage assignment) and did not verify every collapsed
  forecast against the board.
- I did not re-verify the deck's own primitives (Sigarda, Katilda, Brutal Cathar, Lair of the Hydra, Teferi —
  verified in waves 72-73); I verified only the four opponent cards that decided game 4.
- MULLIGAN, BOTTOMING and CLEANUP DISCARD carry no evidence this wave (six untouched-seven keeps, zero bottom
  asks, zero cleanup discards); the edits I made there are byte reclaims of restatement, not rule changes.
