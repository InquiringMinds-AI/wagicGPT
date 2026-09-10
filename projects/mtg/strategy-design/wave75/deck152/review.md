# Wave-75 deck 152 review — BANT HUMAN MIDRANGE

Corpus `matchups-20260910-003556-final`, SINGLE binary `wagic-7a04cd6da-w75step1`. Seat files
`...ai_baka_deck152-<ptr>-vs-...`; opponent seats read for context only.

242 model decisions (146 35 / 125 57 / 126 60 / 130 18 / 123 38 / 162 34), **242 with native reasoning**,
`thinking` true on all 242. 0 fallbacks, 0 `answer_replaced`, 0 `reply_truncated`, 0 `action_before_plan`,
0 `wall_miss`, 0 `ask_replays_refused`, 0 `phase2_answer_missing`. `transport` `curl=0,http=200,empty=0` on
242/242. `protocol_deviation` non-compliant 23 (9.5%): 18 `unlabelled_plan`, 5 `plan_absent`.
Seat **5-1** (loss vs deck146). Live guide read = the wave-74 edit, 19,992 B.

Gameend counters by game (146/125/126/130/123/162): `hold_released_turn` 0/2/0/0/0/1,
`hold_windows_skipped` 0/19(cast)/0/0/0/9(priority), `main_phase_windows_skipped` 0/6/8/12/10/3,
`own_turn_windows_skipped` 0/28/55/38/34/42, `ask_replays_reserved` 1/5/1/1/0/1,
`identical_ask_answers_reserved` 1/4/1/0/0/0, `async_drops` 0/0/1/0/2/1,
`plan_names_stranded_card` **1**/0/0/0/0/0, `plan_names_uncastable_zone_card` 0 everywhere,
`forced_close_unrecorded` 0/0/0/0/**1**/0, `blocker_forecast_gang` 0 everywhere,
`chain_selfharm_rows` 0 everywhere, `stop_reached_windows_skipped` 0, `mana_only_windows_skipped` 0,
`repeat_annotated_takes` 0, `reserve_decline_windows_noted` 0, `sibling_window_asks_skipped` 0,
`chain_windows_collapsed` 0, `declined_face_latches` 0.

## 1 — Game by game

| # | Opp | Result | Turn | Life | Deciding decisions |
|---|-----|--------|------|------|--------------------|
| 1 | 146 | **loss** | 17 | -7 / 17 | **seq 13 — Fateful Absence taken in Upkeep off a row that printed `STRANDS Brutal Cathar this turn` (HIGH-1)**; seq 11 correct `ATTACK: none`; seq 23 Wolf redirected onto Kaya, killing her; seq 30 `BLOCKS: none` at N=6 (see LOW-2); seq 32/34 defeatism ("accept the loss") but still deployed |
| 2 | 125 | win | 19 | 20 / 0 | **seqs 25-34 — ten identical declined windows in one turn, hold row present at every one (HIGH-3 / MED-1)**; seq 48 Tovolar's Huntmaster; seq 60 Katilda pump on a ONE-name tap bill in Main 2 (correct); seq 62 correct pass, then lethal |
| 3 | 126 | win | 15 | 28 / -1 | seq 10 Brutal Cathar exiles **Perimeter Captain** (the lifegain defender) — wave-74 Edit 2 paying; seq 22 the "as high as N" compare run correctly (below); Teferi +1 taken every own turn (seqs 14-21, 27-35, 41-48, 53-60); seq 49/62 four-attacker alphas |
| 4 | 130 | win | 13 | 13 / 4→dead | Aspirant counter onto the flier every combat (seqs 10/12/15); **seq 17 `Play no land right now` (HIGH-2)**; seq 20 lethal |
| 5 | 123 | win | 11 | 23 / 10→dead | **seq 2 — untouched seven with 2 lands + 5 spells shipped (HIGH-4)**; seq 12 Teferi; seq 13-18 / 23-29 the +1 cascade navigated correctly; seq 29 Aspirant counter onto a summoning-sick body (LOW-1); seq 38 lethal |
| 6 | 162 | win | 13 | 19 / 6→dead | seq 22 Lair of the Hydra animated in Main 1 as an unblockable rung; seq 13/20 correct HOLDs; seq 23/34 Wolf + Tracker beats |

**Game 1 in full (the loss).** Mardu-ish deck146: two deathtouch Triumphant Adventurers, Lost Mine of
Phandelver venturing, Silverquill Silencer recurring from exile, Kaya, then Lolth making 2/1 menace Spiders.
The seat never got ahead on board: it spent turn 8's Upkeep on Fateful Absence (HIGH-1), which cost it the
turn-8 Brutal Cathar; Kaya then exiled the turn-10 Cathar, Vanishing Verse exiled Intrepid Adversary (T7) and
Elite Spellbinder (T15), and the T14 double-block trade emptied the board. The crack-back render was exactly
right on the way out: seq 32/35 print `CRACK-BACK NEXT TURN ... 13 from 6 of their creatures ... you would be
at -7 or lower` and the seat died at **-7** the next turn.

## 2 — Engine / interface / card items

### HIGH-1 (STRATEGY, guide) — a `{reserve: ...}` row with an explicit STRANDS verdict was taken anyway
`1789018570-...-vs-ai_baka_deck146.jsonl` **seq 13** (T8 Upkeep, 16 life). Row 1 rendered:
`Cast Fateful Absence {1}{w} {leaves 1 of your 3 untapped mana sources untapped} {reserve: TAKE THIS ROW AND
YOU CANNOT CAST Brutal Cathar AT ALL THIS TURN. This row is INSTANT SPEED - it still has a window at the end
of THEIR turn. Taking it HERE, before your main phase, leaves 1 source, and Brutal Cathar {2}{w} in your hand
needs 3 - it is SORCERY SPEED, so your main phase this turn is its last window. VERDICT: taking this row
STRANDS Brutal Cathar this turn - 1 source cannot pay 3, and it has no window after your main phase}`.
The reasoning never mentions the reserve clause; it prices only `next turn they attack with 2 creatures
instead of 3, I take 2 damage next turn instead of 3`. The reply then plans the very card the row had
refuted — `PLAN: ... In Main 1, play Plains, cast Brutal Cathar` — and the engine tagged the record
`plan_names_stranded_card: Brutal Cathar` (this is one of the corpus's five; quoted here in full).
The surface is TRUE, complete and unusually explicit, so this is STRATEGY. Two guide rules were already
against it (`with a Brutal Cathar in hand, cast the CATHAR instead`; the deploy floor) and neither reached an
Upkeep window. **Cost: a 2/2 + exile trade for one point of damage, in the only game the seat lost.**
Guide edit 1.

### HIGH-2 (STRATEGY, guide) — `Play no land right now` taken, on the exact excuse the guide pre-empts
`1789018590-...-vs-ai_baka_deck130.jsonl` **seq 17** (T13 Main 1). Menu:
`1. Play Lair of the Hydra [enters TAPPED - it makes no mana this turn ... it taps for mana from your next
turn on] {also a THREAT: this land ANIMATES ...}` / `2. Play no land right now`. Answer: **2**.
The reasoning quotes a rule and then breaks it: `Should I play the land? The strategy guide says "Play a land
every turn you can". But it enters tapped. It's fine. I'll stick with 2. ... It's safer to just attack.`
The live guide already forbids exactly this, by name (`never "Play no land right now" - ... not because the
row says the land enters TAPPED ... Lair of the Hydra is a land like any other: play it`). **No edit**: the
rule is already maximal and enumerating the excuse again did not hold. Recorded as evidence that on this deck
the recurring failure mode is *recite-then-override*, not perception. Zero cost (won that turn).

### HIGH-3 / MED-1 (ENGINE, window family) — ten model calls on one unchanged three-row menu
`1789018578-...-vs-ai_baka_deck125.jsonl` **seqs 25, 26, 27, 28, 29, 30, 31, 32, 33, 34**, all T12, all
`Casting decision (<phase>, opponent's turn)` across Upkeep → Draw → ... → Cleanup, with the identical three
rows every time (`1. Cast Fateful Absence ... - the only legal targets are YOUR OWN right now: Luminarch
Aspirant`, `2. Hold priority ...`, `3. Cast nothing right now`). The engine's own brackets say so:
seq 34 prints `[you declined this exact list 9 times already this turn]` and `[hold check: every row above was
also on the menu at the last window at this seam (11 windows in a row now)]`.
`identical_ask_answers_reserved` for that whole game is **4**, and the stderr replay line
(`game-152v125-1789018575.stderr:1119,1312,1318,1751`) fires only at *Combat begins*, *Blockers* ×2 and
*Upkeep* — never once inside this run. So the replay path did not collapse the very run the hold-check
bracket was counting. Ten calls at p50 latency ≈ 13 minutes of wall clock on a dead menu.
**Repro:** that file, seqs 25-34; the row set is byte-identical apart from the two bracket notes.
For the engine seat: whether the `[you declined this exact list N times]` note, which increments every
window, is itself what makes each prompt non-identical to the replay comparator. Note also that the model's
protocol compliance decays inside the run — seqs 31/33/34 are three of this seat's five `plan_absent` /
`unlabelled_plan` records.
Guide edit 3 closes the seat's half (take the hold at the FIRST note).

### HIGH-4 (STRATEGY, guide) — an untouched seven that cleared the floor was shipped, on a fabricated quote
`1789018594-...-vs-ai_baka_deck123.jsonl` **seq 2**. Header: `Your hand (7 cards), counted by the engine:
2 lands, 5 spells. ... Playing every land in this hand would cover the cost of: Fateful Absence {1}{w}.`
That is the live guide's M-of-7 floor met twice over (two lands, five spells, coverage names a card). The
reasoning invents an authority for the opposite: **`The guide says: "Mulligan hands with 0-1 or 6-7 lands."`**
— that sentence appears nowhere in `deck152_strategy.txt` — and adds a curve argument (`T2 play Pathway,
untapped lands 0. Stuck.`) that the guide bans as a ship reason only under the M-of-5 heading. The seat went
to six (seq 3, a correct ship: 1 land, `would not cover any spell in it`) and kept at five (seq 4, correct).
Two cards thrown away; the seat won anyway. **Perception was fine; the instruction set had a scope hole and
the model filled it with an invented rule.** Guide edit 2.

### MED-2 (ENGINE, meter) — `plan_line_missing` / `protocol_deviation` are still one shape, and still charging correct plans
23 non-compliant of 242. **18 are `unlabelled_plan`**: a correct, single-sentence plan with the `PLAN:` label
dropped — `Attack with nobody to preserve the Wolf for blocking next turn ...` (146 seq 11, 122 B) ·
`Tap opponent's Perimeter Captain to prevent it from blocking my attackers.` (126 seq 29, 74 B) ·
`Attack with Tovolar's Huntmaster alone to deal 6 damage, dropping opponent to 1 life.` (125 seq 62, 85 B).
**5 are genuine `plan_absent`** (bare action line: 146 seq 22, 125 seqs 31/42/53, 123 seq 25). **Zero records
carry real off-protocol prose this wave** (wave 74 had one 621 B paragraph) — `off_protocol_bytes` is
measuring the missing label, not stray text. Wave-74 MED-2 **RECURS**, same shape, unchanged.

### MED-3 (guide friction, no cost) — the `(neither dies)` outnumber clause does not price the blocking trigger
`1789018592-...-vs-ai_baka_deck126.jsonl` **seq 22** (T9, 22/17). The only `as high as N` window in this seat
all wave: `ATTACK TOTAL: 2 attackers listed, 6 total combat damage ... up to 2 life back across their 1
blocker, so blocking can leave them as high as 16`, with `A1. Elite Spellbinder [no creature they control can
block this attacker]` and `A2. Moonrage Brute [their untapped blockers: Overgrown Battlement (0/4) (neither
dies) (blocking trigger: they may gain 2)]`. The model ran wave-74 Edit 1's compare explicitly
(`if N=16, and their life is 17, 16 is not higher`) and then sent **A1 only** — better than the literal
guide, which at N below their life routes to the per-creature tags, and whose `(neither dies)` rung says send
when `your declared attackers OUTNUMBER their untapped blockers` (2 > 1 here). Sending A2 was worth -2 life
of ground: 14 versus a best case of 11 and a likely 16. **No edit** — the outcome was right and adding an
arithmetic sub-case to a per-creature tag is the kind of concession Amendment 334 forbids; recorded so a
later wave that sees this cost a game knows where it lives.

### LOW-1 (STRATEGY, guide) — Aspirant counter onto a body that could not attack
`...vs-ai_baka_deck123.jsonl` **seq 29** (T9). Target list: `1. Luminarch Aspirant (4/4) [your battlefield]`
and `2. Brutal Cathar (2/2) [daybound] [your battlefield] [summoning sick - cannot attack this turn, but CAN
block]`. Answer: **2**. The guide's own head clause (`PUT IT ON A CREATURE THAT CAN CONNECT`) says 1; its
tail clause (`never the Aspirant while another creature can attack`) reads as an absolute and won. One point
of damage in a won game. Guide edit 4 scopes the tail to *this turn*.

### LOW-2 (judgement call, no defect) — `BLOCKS: none` at `you would be at 6`
`...vs-ai_baka_deck146.jsonl` **seq 30** (T15, 11 life, sole blocker Katilda 1/1 against A1 1/1
first-strike-deathtouch, A2 1/1 Goblin `(both die)`, A3 3/2). The guide's `BLOCKS: none` gate does not admit
this answer (N is 6, and one offered rung is 2, not 3), and the ladder's rung-2 exception (`unless the
attacker it kills is worth less than your blocker`) is what the model actually applied — a 1/1 token against
its only creature and mana engine. The board's own header sanctioned it: `declining every block leaves you at
6 and costs you no permanent`. Checked against the outcome: the seat took 13 the following turn, so both 9
and 6 were lethal. **Not a defect and no edit** — but the interaction between the `BLOCKS: none` gate (which
tests N and rung shape) and the ladder's rung-2 exception is a place where two rules of this guide give
opposite answers, and rung-2's exception won without the model noticing the gate.

### Verified NOT defects this wave (render truth checks the brief asked for)
- **P18 MDFC land drops — PASS.** 21 land-drop menus with a Pathway in hand; **all 21** list FRONT and BACK as
  separate numbered rows, each tagged `TWO FACES, ONE CARD`. **0** rows anywhere use the old
  `[PLAY THIS AS A LAND: ...]` form, and **0** of the 31 `Choose an option for <card>:` prompts carry an
  `OTHER FACE of` row. (Side effect: the live guide's PATHWAYS bullet still described the *retired* render —
  guide edit 5.)
- **P7/F8 crack-back cover — PASS, and TRUE.** 3 instances (146 seq 32/35, 130 seq 19). 146 seq 35:
  `{crack-back cover: the CRACK-BACK NEXT TURN line above is 13 from 6 of their creatures and puts you at -7
  OR LOWER ... Counting only the checked bodies you cover 4 of 13, leaving 9 -> you would be at -3.}` Board
  audit: TA 1 + Goblin 1 + Silencer#1 3 + Silencer#2 4 + Spider 2 + Spider 2 = **13**, correct; the seat died
  at exactly **-7**. The legend-tagged Katilda row on the same two menus carries **no** cover clause — a
  legend-rule duplicate is correctly not counted as cover.
- **P6 `CRACK-BACK NEXT TURN` — present in every own-turn window I sampled** (all 6 games), and every figure
  I audited against the board matched, including the FLOOR extension naming `Hive of the Eye Tyrant` (146
  seq 35) and `Siege-Gang Commander (2 per activation)` (130 seq 17).
- **Holds — no hold outlived its holder's untap.** 3 takes (125 seq 40, 46; 162 seq 13, 20 — 4 takes, 3
  latches). `game-152v125-1789018575.stderr:1386` `taken on turn 12 (own turn), now turn 14`; `:1746`
  `taken on turn 16 ... now turn 18`; `game-152v162-1789018581.stderr` `taken on turn 7 (their turn), now
  turn 8`. All released at this seat's own untap. Every hold was taken on a menu whose only non-pass row was a
  dead cast row — no activated or loyalty ability was ever above a taken hold (wave-72 rule respected).
- **P19 — PASS.** 96 prompts render a hold row; **every one** carries the `[HOW A HOLD ENDS:` paragraph
  exactly once (0 prompts with two). Hold-row length: min 492 B, median **596 B**, max 596 B — all under the
  650 B bar, against wave 74's 1,074-1,287 B row.
- **P5 loop-scope sentence, P10 own-clock gate, P11 `another copy of`, P15 X ladders, P22 pathway parse
  noise — UNTESTED here**: `chain_selfharm_rows` is 0 in all six games, no `reaches 0 in N more turns` and no
  `another copy of` string appears in any of the 242 prompts, no ANNOUNCE_X or X-ladder window fired, and no
  `Parser returned NULL {t}:add{` line appears in any of my six stderrs.
- **P3 `[cannot pay now]`** — 208 occurrences across the 242 prompts, all of the hand-card form `[cannot pay now: needs N mana, you have
  M untapped sources]`; every one I audited against the `Mana available` line was arithmetically TRUE (e.g.
  146 seq 11, Fateful Absence needs 2, one untapped Plains). No `[cannot pay now]` on any *activated* row.
- **P16 short names** — every `CHOICE:` answer this seat wrote was accepted; `answer_replaced` 0, and no
  ambiguous or uncopyable short name appeared.
- **Teferi's +1 cascade — wave-73 Edit 2 is now TESTED and PASSES.** Teferi resolved in three games and the
  cascade ran **12 times** (123 seqs 13-18 / 23-29; 126 seqs 14-21 / 27-35 / 41-48 / 53-60). Every stage the
  seat had nothing for was declined (126 seq 21 `Decline - do nothing` at the artifact stage), every entered
  stage had a nameable target, and the side pick was right every time: on its own turns it tapped a would-be
  blocker (`choose opponent creature` → Perimeter Captain / the lifelink Vampire), and it untapped its own
  tapped land when that was the live use. **Zero wrong-side picks, zero pass-less dead ends.**
- **Wave-74 Edit 2 (life engine off the list) PAID.** Last wave's losing matchup was deck126; this wave the
  same matchup is a 28-to-(-1) win, and the pivot is `...vs-deck126` **seq 10**, where Brutal Cathar's target
  list offered the vanilla wall and the seat took `Perimeter Captain #1 (0/4) [defender] - "Defender --
  Whenever a creature you control with defender blocks, you may gain 2 life."` — the exact row wave-74's
  HIGH-2 said it must take. Teferi then tapped the same class of body every turn thereafter.
- **Wave-74 Edit 4 (tap-bill count) PAID.** 5 tap-bill rows. The two two-name-in-Main-1 style rows were
  passed (146 seq 31/34, 36); the one-name Main-2 Katilda pump was correctly **taken** (125 seq 60,
  `{paying this taps: Katilda, Dawnhart Prime - it cannot attack this turn}`, after attackers, three creatures
  out). No lost attack from a tap bill this wave.

## 3 — Did wave 74's edits pay off?

- **Edit 1 (the "as high as N" COMPARE) — PASS, once.** One instance (126 seq 22); the model ran the compare
  in its own words and answered correctly. See MED-3 for the one seam the rule leaves open.
- **Edit 2 (life engine / target off the list) — PASS**, and it flipped the matchup that paid for it.
- **Edit 3 (blocking under THEIR converter) — UNTESTED.** No `LIFE-TO-DAMAGE CONVERTER` line in any of the 242
  prompts.
- **Edit 4 (obey the tap-bill count) — PASS**, 5/5.
- **Wave-74 MED-2 (off-protocol is one shape) — RECURS**, 18/23.
- **Wave-74 MED-3 (`main_phase_windows_skipped` over-count) — NOT reproducible on this seat this wave.**
  39 counted across six games; I cross-referenced the deck130 game (12 counted) and deck123 game (10) and
  found no own Main-phase-1 turn that both carries a `Casting decision (Main phase 1, YOUR turn)` record and
  is counted. P13's fix looks to hold here.
- **Wave-74 HIGH-1/HIGH-2/HIGH-3 (all from the deck126 game) — none recur.**
- **Wave-74 LOW-1 (`(both die)` has no rung in the ATTACK ladder) — RECURS as a near-miss**; see LOW-2, where
  the *blocking* ladder's rung-2 exception silently overrode the `BLOCKS: none` gate.

## 4 — Guide verdict: **EDIT**

Revised guide at `wave75/deck152/strategy.txt`, **19,945 B** (live 19,992; ceiling 20,000), net **-47 B**.
Amendment-332 clean (no wave, seq, game, corpus, count or history string in the file); heading scopes
unchanged (A333); no concessive clause added for any rule's hard case (A334); the guide carries no PLAN
template and quotes no reply label.

**Edit 1 — the reserve clause is a PASS.** Paid by `...vs-deck146` seq 13 (HIGH-1).
before (end of the FATEFUL ABSENCE bullet): `STOP: when the option says "the only legal targets are YOUR OWN
right now", DO NOT CAST IT.`
after: same, plus `A row printing "{reserve: ... VERDICT: taking this row STRANDS <name> this turn}" is a
PASS: the body you would strand outweighs a small creature killed early, and this instant still has its
window at the end of THEIR turn.`
Why this shape and why here: Fateful Absence is this deck's only instant, so it is the only card that can
generate a reserve clause — the rule belongs in its bullet rather than in a section whose heading scopes it
to a phase. The failure was not perception (the clause is the most explicit line on the screen) and not
arithmetic; it was that nothing in the guide named the string, so the model priced only the damage. Naming
the string and giving the verdict in one sentence is cheaper than re-arguing the tempo.

**Edit 2 — the M-of-7 floor is terminal.** Paid by `...vs-deck123` seq 2 (HIGH-4).
before: `- M of 7, the untouched seven: TWO OR MORE LANDS AND AT LEAST ONE SPELL is the floor - KEEP. ALL
LANDS or ZERO LANDS: ship. ONE LAND: keep if the coverage line names a card, ship if it covers nothing.`
after: same, plus `The floor is the whole test at seven: a hand that clears it is a KEEP, and no count of
colours, of expensive spells, or of lands you would rather have reopens it. What a hand cannot cast on turn
two it casts on turn four.`
Why this shape: the bans on colour and curve as ship reasons live under the *M of 5 or less* bullet, and
under A333 that heading scopes them — so at M of 7 the model met a bare floor with no closure and supplied
its own rule. The floor already said KEEP; what it lacked was the statement that nothing else is consulted.
This is the rule that earned the guide's growth: it is the only edit here whose failure threw away real
cards before a single decision was made.

**Edit 3 — the hold trigger is made concrete.** Paid by `...vs-deck125` seqs 25-34 (HIGH-3).
before (end of the HOLD-PRIORITY paragraph): `That N counts identical OPTION LISTS, not boards, so it is
never itself a reason to pass.`
after: same, plus `The hold row's warning prices rows you still WANT; when every row above it is one you have
already ruled out - a Fateful Absence whose only legal targets are your own, a legend-tagged duplicate of a
healthy creature - it prices nothing, so hold at the FIRST such note rather than decline the same list
again.`
Why this shape: the existing rule already prescribed HOLD at the first `[you declined ...]` note and the
model declined nine more times. Its reasoning shows what beat the rule — the hold row's own capitalised
`YOU CANNOT COME BACK AND TAKE ONE OF THE ROWS ABOVE LATER THIS TURN`, read as a live cost. Pricing that
warning at zero for a menu of rows the guide has *already* told it not to take is the only thing that
removes the tension, and both examples named are rows this guide independently bans.

**Edit 4 — the Aspirant tail clause is scoped to this turn.** Paid by `...vs-deck123` seq 29 (LOW-1).
before: `and never the Aspirant while another creature can attack.`
after: `and never the Aspirant while another creature on the list can attack THIS turn - a row tagged
"[summoning sick" cannot.`
Why this shape: the head of the rule (`A CREATURE THAT CAN CONNECT`) and its tail disagreed on exactly one
board — the one that occurred. Pointing the tail at the printed tag rather than at a general capability
makes the two clauses one rule.

**Edit 5 — the PATHWAYS bullet describes the render that exists.** Paid by the P18 audit above (21/21 menus,
0 rows using the retired string).
before: `- PATHWAYS have two faces, both with a row on the same menu: "Play Land" plays the shown face and
"<other face name> [PLAY THIS AS A LAND: ...]" plays the other. ...`
after: `- PATHWAYS have two faces and BOTH get their own Play row on the same menu, each tagged "TWO FACES,
ONE CARD" as the FRONT or the BACK. Either row is one land drop, costs nothing, and is the only route to that
face. Take the face that turns an uncastable card in hand castable.`
Why: the guide was telling the model to look for a string the engine no longer prints. No cost was observed
(the seat played 20 of 21 Pathway drops correctly) but a guide that names a dead surface is a guide that will
eventually be trusted over the live one.

**Bytes.** 19,992 → **19,945**, net **-47 B**. The five edits add ~1,010 B, reclaimed from seven places, none
of them a rule: the tap-bill paragraph's explanation of *why* the source count misleads (the SEARCH and COUNT
instructions both survive, and the rule went 5/5 this wave); `Believe that clause over any other`, which
`OBEY THE COUNT` in the next sentence already says; the attack section's `"That IS lethal - but only if none
of them is blocked"` aside (that string has not appeared in two waves); the Teferi bullet's restatement of
the two side-pick row labels, which the rows themselves print verbatim (the actionable inversion — tap an
attacker on *their* turn — survives); the FORBIDDEN THOUGHT bullet's two worked mono-colour examples, now
covered by edit 2's `no count of colours ... reopens it`; `Vigilance is no licence and lifelink is not
payment` and `NEVER hold everything; the deploy floor still applies`, both restatements of rules stated in
full elsewhere; and the cleanup-discard sentence compressed (that ask has fired **zero** times in four
waves; the ordering rule is kept, the `never your only two-drop` gloss is not).

## 5 — Proposals

**None.** HIGH-3's engine half is an engine string/comparator question, written up as a repro for the engine
seat rather than as an instruction-surface proposal. HIGH-2 is a failure of a rule that is already as
explicit as it can be made, so proposing more instruction there would be noise. The one transferable
observation — *a rule stated under a scoping heading does not reach the sibling case, and the model will
invent an authority to fill the gap rather than fall back to the nearest rule* — is close enough to
`wave68/strategy-writing-skill-v2.md`'s existing scope-and-observable-criteria material that a proposal
restating it would be noise under this wave's own standard.

## 6 — What I did NOT check

- Any seat but 152's, except the six opponent seat files read for context. No stderr beyond my six games.
- The `forced_close_unrecorded: 1` in my vs-deck123 game: I did not reconcile it against the stderr
  forced-close count or determine whether a decision was lost — that reconciliation is item (c) of the
  engine seat and needs the corpus-wide arm accounting.
- `async_drops` 4 (126 ×1, 123 ×2, 162 ×1): I did not trace each to a re-ask, so I cannot say none fell to
  the heuristic on this seat.
- `blocker_forecast_multi` 14 / `_rows` 30 across the seat: I read the six block windows that mattered
  (146 seq 6/12/24/30, 126 seq 50, 130 seq 8) and did not verify every collapsed forecast against the board.
  `blocker_forecast_gang` is 0 here, so BS F5 and the gang-clause checks are UNTESTED on this seat.
- I did not re-verify this deck's own primitives (verified waves 72-73) and I verified no opponent card
  against Oracle text this wave — nothing in my six games turned on a suspect card behaviour.
- No ANNOUNCE_X window, no `{X}`-announcement payment loss, no converter board, no Moonrage ward ask, no
  cleanup discard, no reveal-with-`[cost:]` row and no ghostform gloss fired in this seat, so all of those
  remain UNTESTED here.
- `main_phase_windows_skipped` 39: I cross-referenced two of the six games, not all six.
