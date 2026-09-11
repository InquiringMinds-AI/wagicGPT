# Wave-76 deck 152 review — BANT HUMAN MIDRANGE

Corpus `matchups-20260910-141526-final` (consolidated: 2 first-run games + 19 reruns after the pilot wedge),
SINGLE binary `wagic-d372a1153-w76step1`. My seat files `...ai_baka_deck152-<ptr>-vs-...`; opponent seats read
for context. Live guide = the wave-75 edition, 19,945 B.

**217 model decisions** (162: 22 · 146: 17 · 123: 30 · 125: 38 · 126: 69 · 130: 41 — the two FIRST-RUN games
are 152v162 and 146v152), 100% with native reasoning, `thinking` `on` on all 217, 0 fallbacks, 0
`answer_replaced`, 0 `reply_truncated`, 0 `action_before_plan`, 0 `ask_replays_refused`, 0
`phase2_answer_missing`, `transport` `curl=0,http=200,empty=0` throughout. Seat **5-1** (loss vs deck123).
Max prompt 23,969 B; 8 prompts over 20 KB.

Gameend counters by game (162/146/123/125/126/130): `main_phase_windows_skipped` 6/2/4/2/12/0,
`own_turn_windows_skipped` 33/48/23/62/53/0, `hold_windows_skipped` 0/0/0/0/0/**8** (all cast),
`ask_replays_reserved` 0/0/1/0/1/4, `identical_ask_answers_reserved` 0/0/1/0/0/3,
`crossphase_identical_reputs` 0/0/0/0/0/**1**, `chain_acting_rows` 0/0/0/32/10/18,
`blocker_forecast_multi` 4/0/8/0/30/8, `_gang` 0/0/2/0/0/2, `plan_line_missing` 0/1/0/3/11/1,
`async_drops` 0/0/0/0/1/0. **Zero** everywhere: `plan_names_uncastable_zone_card`,
`plan_names_stranded_card`, `forced_close_unrecorded`, `force_close_arms_refused/_deferred/_defer_bound_hits`,
`chain_selfharm_rows`, `stop_reached_windows_skipped`, `mana_only_windows_skipped`, `repeat_annotated_takes`,
`reserve_decline_windows_noted`, `declined_face_latches`, `hold_released_turn`.

## 1 — Game by game

| # | Opp | Result | Turn | Life | Deciding decisions |
|---|-----|--------|------|------|--------------------|
| 1 | 162 | win | 11 | 36 / -13 | seq 21 correct `Cast nothing` under a cleared lethal count with FIVE creature rows offered; seq 23 four-attacker alpha, `KILLS them whatever they block` |
| 2 | 146 | win | 12 | 17 / 0 | seq 12 `BLOCKS: none` at "you would be at 19" vs a lone deathtouch first-striker (rung-3, gate satisfied); seq 17 correct decline under the cleared count; seq 18 lethal |
| 3 | 123 | **loss** | 18 | -4 / 15 | seq 11 T9 Adversary + valor counter; **seq 26 — `ATTACK: none` with both bodies tagged `held back, could not block ANY of their 10 creatures` (HIGH-1)**; seq 29/31 correct declines (a 1/1 cannot block fliers — see MED-3); seq 32 one legal blocker against 9 fliers |
| 4 | 125 | win | 23 | 17 / -18 | seq 27 Tovolar's Huntmaster; seq 29-33 Teferi -2 → Katilda recast; seqs 34/36 tap-bill Katilda pump correctly PASSED in Upkeep/Main 1; seq 37 `ATTACK: all`, -18 |
| 5 | 126 | win | 15 | 24 / -4 | seq 51 + seq 71 the `as high as N` compare run correctly, unblockable-only attacks (wave-74 Edit 1 pays twice); **seq 65 — eight free rung-1 blocks declined (HIGH-2)**; seq 59 tap-bill row TAKEN with three names (MED-2) |
| 6 | 130 | win | 15 | 17 / 0 | **seq 7 — a `{reserve: ... STRANDS}` row PASSED in Upkeep (wave-75 Edit 1 pays)**; seqs 23/27 two holds taken on menus whose only live row was a dead Fateful Absence; seq 45 lethal |

**Game 3 in full (the loss).** Esper-ish deck123: Tragic Slip ×3, Devour Flesh, Bloodline Keeper, Intruder
Alarm. Three of the seat's first four creatures were killed on the spot (Katilda T5, Adversary + Sigarda T9).
Fateful Absence answered Keeper #1 in Upkeep on T11 (correct — it is the engine), Keeper #2 landed T12, and on
T14 Intruder Alarm + Keeper made **nine 2/2 fliers in one turn**. From there the board was unanswerable: at
the T18 blockers window the engine offered exactly ONE B-row (Sigarda) against nine fliers. Hands and draws,
not the guide, decide this game; the one guide-attributable decision is seq 26.

## 2 — Engine / interface / card items

### HIGH-1 (STRATEGY, guide) — two bodies held home "to preserve board" that could block nothing
`1789074143-...-vs-ai_baka_deck123.jsonl` **seq 26** (T15, 27/15). Both A-rows carry the tag:
`A1. Intrepid Adversary (4/2) [lifelink, ...] [held back, THIS creature could not block ANY of their 10
creatures: Bloodline Keeper (flying), Vampire #1 (flying), Vampire #2 (flying), +7 more] [their untapped
blockers: ... biggest Bloodline Keeper (3/3) (both die (lifelink: you gain 4 from this block only ...))]` and
the same "held back" clause on A2. Answer **`ATTACK: none`**, with the plan line
`Attack with none to preserve board against their 10 blockers` — a defensive premise the two tags on the very
rows refute: neither body can legally block any creature they control, so staying home guards nothing. The
reasoning never quotes the tag (it does at seq 21, two windows earlier, and attacked). A2 was a correct hold
under the guide's rung-3 STOP; A1 was a lifelink trade for a Keeper or a token plus 2-4 life, thrown away.
**Surface TRUE, complete and on the row → STRATEGY.** Guide edit 2.

### HIGH-2 (STRATEGY, guide) — eight free rung-1 blocks declined on a false rules premise
`1789074153-...-vs-ai_baka_deck126.jsonl` **seq 65** (T14, 26 life). Two 1/1 lifelink Vampires attacking;
**eight** B-rows, seven of them `may block A1, A2 (all: you kill it, your blocker lives)`. Answer:
**`BLOCKS: none`** (bare, no PLAN label). The reasoning names the cost it invented:
`If I block with non-vigilance creatures, they will be tapped and can't attack next turn. That's bad for my
clock.` **Blocking never taps a blocker** — the guide does not say so and nothing on the screen does either,
so the model supplied a rule and it beat rung 1 ("TAKE IT, at every life total, always"). The header's own
general hint (`block only where the trade favors you; taking damage while ahead on LIFE is often correct`) is
quoted in the reasoning as support; its parenthetical `(your strategy guide's blocking rules override this
general hint)` did not hold. Cost: two lifelink Vampires survived and gained 2. Guide edit 1.

### MED-1 (RENDER, `{crack-back cover:}`) — the body COUNT is not block-legality-filtered, the damage figure is
`...vs-ai_baka_deck123.jsonl` **seq 31** (T17). The clause reads: `... CHECKED: 1 of them is a body whose block
legality against these attackers this row computed. 8 of their 9 could not legally be blocked by it at all ...
You ALSO already control 2 untapped creatures that can block on their turn without casting anything, and this
clause counts them FIRST: on their own they cover 2 of 18 ... leaving 16 -> you would be at -4, which still
KILLS you.` The arithmetic is exactly right — the seat died at **-4** the next turn — but "2 untapped
creatures that can block" is false in the same sentence that prices them at one body's worth: the blockers
window two records later (seq 32) offers **one** B-row, Sigarda, because Briarbridge Tracker cannot block a
flier. The new-body half applies the legality filter and says so; the already-controlled half prints an
unfiltered count next to a filtered number. Repro: that file, seq 31, then seq 32's option list.

### MED-2 (guide friction, good outcome) — the tap-bill count rule was broken and the play was right
`...vs-ai_baka_deck126.jsonl` **seq 59** (T13, Main phase 1). Row taken:
`Cast Tovolar's Huntmaster {4}{g}{g} (6/6) {leaves 1 of your 7 untapped mana sources untapped; paying this
taps: Briarbridge Tracker, Luminarch Aspirant, Luminarch Aspirant - they cannot attack this turn}`. The guide:
`two or more names in Upkeep or Main 1 is a PASS, whatever the row is`. Three names, taken. It cost nothing:
those three are exactly the blockable bodies that this deck's own `as high as N` rule keeps home against
deck126's six lifegain blockers (seq 51 and seq 71 both send the unblockable flier ALONE), and the Huntmaster
plus two Wolves is what closed the game. **No edit** — the tap-bill rule is a proxy for "a lost attack", and
under A334 the hard case does not earn a concessive clause; recorded so a later wave that sees this cost a
game knows the two rules meet here. The other three tap-bill rows this wave (125 seqs 34/36, 126 seq 66) were
correctly PASSED.

### MED-3 (guide friction, good outcome) — the deploy floor declined, correctly, against an all-flier board
`...vs-ai_baka_deck123.jsonl` **seqs 29 and 31** (T17, 7 then 12 life). Luminarch Aspirant offered on the own
main phase; answered `Cast nothing right now` with the reason `casting Luminarch Aspirant provides no
defensive value against their flying creatures`. True — seq 32 proves it — and the `{crack-back cover:}` clause
on the very row said the same (`8 of their 9 could not legally be blocked by it at all`). The deploy floor
admits three exceptions and none of them is this. **No edit** (A334); the screen carried the whole argument.

### MED-4 (INTERFACE, decode cost) — one Teferi +1 costs about ten model calls
`...vs-ai_baka_deck126.jsonl` **T11, seqs 36-47**: `+1` (36-37) → `choose a land` (38) → `choose your land`
(39) → the land (40) → pass (41) → `choose a creature` (42) → `choose your creature` (43) → the creature (44)
→ pass (45) → `Decline - do nothing` at the artifact stage (46) → pass (47). **13 asks in that one (turn,
phase)**, 10 in T9, 9 in T7 — three of the seat's four largest per-(turn, phase) counts, all one card's
three-stage +1, at p50 ≈ 92 s per call. This is a chain, not a loop or a re-put (each window offers a
different list), so it does not trip the loop family — but it is where this seat's decisions go. A pre-declined
empty stage, or one window carrying stage + side + target, would buy back most of it.

### LOW-1 (protocol) — wave-75 MED-2 RECURS, same shape
16 records with `plan_line_missing` (7.4%): **10 carry a correct, single-sentence plan with the `PLAN:` label
dropped** (`Target Elite Spellbinder to deal unblocked lethal damage.` 126 seq 71 · `Remove Sorin to prevent
further threats and secure the win.` 126 seq 63 · `Attack with all creatures to deal lethal damage.` 125 seq
37) and **6 are genuine bare action lines** (125 seq 38, 126 seqs 39/53/55/60/65). `off_protocol_bytes` is
measuring the missing label, not stray prose: no record carries prose outside the two lines. Unchanged from
wave 75 and wave 74.

### LOW-2 (reasoning shape) — tail degeneracy on the reasoning channel
Every seat file shows the same closing loop in `reasoning`: `Output matches. Proceeds. [Output Generation] ->
*Proceeds*` repeated for hundreds of characters (130 seq 7, 123 seq 26/30, 126 seq 65 all quoted above end
this way); `reasoning_ngram_repeat` reaches 0.24 (162 seq 9). It did not corrupt an answer this wave and
`reasoning_degenerate` stays low (≈0.002), but it is where the tail of the 5,600-char median goes.

### Verified NOT defects this wave
- **Q13 / wave-75 HIGH-3 — the ten-call re-put run does NOT recur.** The wave-75 run was 152v125 seqs 25-34;
  this wave that game has `crossphase_identical_reputs` **0** and no seam repeats. The seat's ONE re-put is
  130 seq 27, and the new bracket printed: `[this exact list was put to you 1 window ago at Blockers and you
  declined; the board HAS moved since then]` — TRUE (combat damage between the two windows took them 12 → 5),
  and the re-put was answered with a HOLD, not the same decline. Corpus-wide the bracket printed 109 times and
  only **5** carry `nothing on the board has changed` (4.6%) — the CQ F4 worry, for the engine seat.
- **Hold rows.** 82 prompts render one; **2 holds taken**, both in the deck130 game (seqs 23 and 27;
  `hold_windows_skipped` 8 is what those two then suppressed), and **neither was taken while an activated or
  loyalty ability sat above it**:
  at 130 seq 23 and seq 27 the only row above was a Fateful Absence whose `only legal targets are YOUR OWN`.
  That is wave-75 Edit 3 firing exactly as written (hold at the first dead-list note). 0 `hold_released_turn`
  — every hold was taken on the seat's own turn and expired with it.
- **Wave-75 Edit 1 (the reserve VERDICT is a PASS) — TESTED and PASSES.** The seat's one reserve row,
  `...vs-ai_baka_deck130.jsonl` seq 7 (T7 Upkeep): `Cast Fateful Absence ... {reserve: TAKE THIS ROW AND YOU
  CANNOT CAST Luminarch Aspirant AT ALL THIS TURN ...}` → answered `Cast nothing right now`. The wave-75 HIGH-1
  failure does not recur; `plan_names_stranded_card` is 0 across the seat.
- **Wave-75 Edit 2 (the M-of-7 floor is terminal) — PASSES 6/6.** All six openers were untouched sevens that
  clear the floor (4/3, 2/5, 3/4, 3/4, 3/4, 3/4 lands/spells, every coverage line naming a card) and all six
  were KEPT. No mulligan, no bottom ask, no invented quotation.
- **Wave-75 HIGH-2 (`Play no land right now`) — does NOT recur.** 36 land-drop windows, **36 lands played**,
  including tapped ones (123 seqs 3/4, 130 seq 30 Lair of the Hydra) and 14 Pathway rows, every one taken off a
  `TWO FACES, ONE CARD` row.
- **Wave-74 Edit 1 (the `as high as N` compare) — PASSES twice, both in deck126.** seq 51: `up to 31 life back
  across their 6 blockers, so blocking can leave them as high as 42` at their life 17 → sent A4 (the
  unblockable flier) alone. seq 71: `as high as 30` at their life 6 → sent A3 alone for exact lethal. Both are
  the rule's intended answer and the second one won the game.
- **Deploy floor / lethal count.** 10 own-main-phase declines with a creature row offered; 8 are the cleared
  lethal count or a deferral to Main 2 that was then taken (126 seqs 54/55 → Katilda at seq 58; 130 seq 31 →
  Spellbinder at seq 40), 2 are MED-3. No quiet-board hold, no dodge-removal hold anywhere.
- **Blocking.** 5 block windows. 162 seq 19 rung 1 taken; 146 seq 12 `BLOCKS: none` legal under the gate
  (N=19, the only row rung 3); 123 seq 27 traded Elite Spellbinder for Bloodline Keeper; 123 seq 32 lethal
  incoming, one legal blocker assigned. The one failure is HIGH-2.
- **`{crack-back cover:}` arithmetic — TRUE** on both instances (123 seqs 29/31): 23 from 11 / 18 from 9 both
  audit against the board, and the `-4` the second one forecast is exactly the life the seat died at. See
  MED-1 for the one false clause inside a true calculation.
- **Aspirant counters — 17 target picks, no summoning-sick pick** (wave-75 Edit 4 PASSES; the one wave-75
  failure shape does not recur). The guide's `if your pick reads 6/6 or larger, choose another` clause is
  contradicted 4 times (126 seqs 60/61/69/70 put counters on Elite Spellbinder 6/3 → 9/6, the only body with
  `[no creature they control can block this attacker]`, and that flier won the game). The model ignored the
  clause and played correctly, so **no edit** — recorded because the literal test is refuted by the board it
  was written for.

### UNTESTED on this seat
No `LIFE-TO-DAMAGE CONVERTER` line (wave-74 Edit 3 untested a second wave), no `- THEIRS:` roster, no folded
`(copy N of M in your hand)` row, no ghostform `{removes:}`, no up-to-N loyalty row, no 3-life land row, no
`forced_close_prefill_echo`, no ANNOUNCE_X/`{X}` window, no Moonrage ward ask, no Sigarda COVEN reveal, no
cleanup discard, no bottom ask, no `PUT: none`, 0 `force_close_*` arms, 0 `forced_close_unrecorded`.

## 3 — Guide verdict: **EDIT**

Revised guide at `wave76/deck152/strategy.txt`, **19,972 B** (live 19,945; ceiling 20,000), net **+27 B**.
Amendment-332 clean (no wave, seq, game, corpus, count or history string in the file); headings unchanged
(A333); neither edit adds a concessive clause to a rule's hard case (A334); the guide carries no PLAN template
and quotes no reply label.

**Edit 1 — declaring a blocker does not tap it.** Paid by `...vs-deck126` seq 65 (HIGH-2).
before (BLOCKING, after the `BLOCKS: none` gate): `The test is N, not the size of the attack: at "you would be
at 6" you block even if only 2 damage is coming.`
after: same, plus `DECLARING A BLOCKER DOES NOT TAP IT: the blocker untaps and attacks on your next turn
exactly as if it had stayed home, so a rung-0 or rung-1 block costs you nothing and keeping a body back "for
the attack" is never a reason to decline one.`
Why this shape and why here: the failure was not perception (eight rows read `you kill it, your blocker
lives`) and not the ladder (rung 1 already says TAKE IT always) — it was a false rule the model supplied to
price the block, and a false price beats a true rung. The sentence states the fact and names the excuse it
kills, in the paragraph where the model reached for it. **This is the rule that earned the guide's growth**:
it is the only edit here whose failure gave away free material at a window the guide already covered twice.

**Edit 2 — the "held back" tag ends the defensive argument.** Paid by `...vs-deck123` seq 26 (HIGH-1).
before: `"ATTACK: none" only when every attacker would die for nothing or do nothing.`
after: same, plus `A creature tagged "[held back, THIS creature could not block ANY of their N creatures]"
guards nothing by staying home - keeping it back buys you no blocker at all - so send or hold it on its own
per-creature tag alone.`
Why this shape: the guide names every other tag on that menu and never this one, so "preserve the board" ran
unopposed against a tag that refutes it. It composes with the existing rules rather than competing with them —
the per-creature STOP still held A2 home in the same window, correctly.

**Bytes.** 19,945 → **19,972**, net **+27 B**. The two edits add ~590 B, reclaimed from two places, neither a
rule: Sigarda's COVEN paragraph (the `get a human` reveal and the `PUT: none` answer — neither string has
appeared in a prompt on this seat in two waves) and the Moonrage Brute ward-ask sentence (the
`[opponent's stack]` row, also zero in two waves; the night side's stat line is kept).

## 4 — Proposals

**None.** HIGH-2's second half — the blockers header's general hint being quoted as licence against the
guide's own ladder — is an engine string question with a repro written up above, not an instruction-surface
proposal. MED-4 is an engine window-shape item. The one transferable observation, that the model fills a gap
in the guide with an invented rule rather than falling back to the nearest one (last wave a fabricated
mulligan quotation, this wave a fabricated tapping rule), is already the substance of
`wave68/strategy-writing-skill-v2.md`'s scope material; restating it would be noise under this wave's standard.

## 5 — What I did NOT check

- Any seat but 152's, except the six opponent seat files read for context; no stderr at all, so the engine-side
  reconciliations (forced closes, replay lines, the harness's harvest bug) are untouched here.
- Corpus-wide Q-lane adjudication: I verified Q13's bracket only on my own seat's one re-put plus a
  corpus-wide count of the two clause forms (109 prints / 5 board-unchanged); Q1's hold-check truth, Q7's
  main-phase meter, Q4's knapsack cover, Q6's sacrifice clause and Q15's prefill echo I did not audit — none
  of the surfaces they name fired in a decision that mattered on this seat.
- `blocker_forecast_multi` 50 / `_collapsed` 28 / `_gang` 4 across the seat: I read the five block windows and
  the three gang-tagged attack rows and did not audit every collapsed forecast against the board.
- The `async_drops: 1` in the deck126 game: not traced to its re-ask.
- No card was verified against Oracle text this wave; nothing in my six games turned on a suspect card
  behaviour. Deck 152's own primitives were last verified in waves 72-73 and I did not re-check them.
