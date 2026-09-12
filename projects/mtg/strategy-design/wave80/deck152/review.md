# Wave-80 deck 152 review — BANT HUMAN MIDRANGE

Corpus `matchups-20260912-074153-final`. My six seat files are
`<epoch>-ai_baka_deck152-<ptr>-vs-ai_baka_deck<N>.jsonl`; none of my six games is the rerun `162v123`,
so **all six ran on the 20-game binary `wagic-a7a6b8942-w80step1` (source cf72a002f)** — lanes DI and DJ
are UNTESTED from this seat by construction. I read the six opponent seat files for results/context only.
Live guide = the wave-79 edition installed at 36ae77199, **19,963 B**. Short names below: `152v130` etc.

**280 model decisions** (125: 145 · 130: 38 · 126: 35 · 123: 26 · 146: 21 · 162: 14), `thinking` `on` on all
286 records that carry the field, **100% with native reasoning** (median 5,471 / max 12,015 chars).
Latency p50 76.8 s, max 259 s. `protocol_deviation`: compliant 274 / `unlabelled_plan` 4 / `plan_absent` 1 →
**5 `plan_line_missing` (1.8%; this seat was 8.2% in wave 79)**, `off_protocol_bytes>0` on 4.
0 `reply_truncated`, 0 `answer_replaced`, 0 `post_answer_overrun`, 0 `dropped_assignments`,
0 `answer_label_absent_read`, 0 `action_before_plan_replies`.

**This seat carries four of the corpus's singular events**: the ONLY `unparsed_reply`
(`152v125` seq 101, §2 HIGH-1), the ONLY `reveal`/`engine_answered` (`152v130` seq 43, zero-eligible —
correct), **all 3 `menu_single_outcome` records and all 60 `single_outcome_rows_spared`** (U13, §2 PASS-1),
and 2 of the corpus's 4 `crossphase_replayed` (§2 PASS-3).

Seat **4-2** (`results.tsv`: wins over 130/123/162/126, losses to 125 and 146; wave 79 was 5-1). Hands are
real; I do not read the swing as guide effect. The 146 loss turns on one attack, §3 S-1, and is what EDIT 1
is paid by. The 125 loss is a 49-turn grind against a UW control deck holding 3 Essence Scatters,
Supreme Verdict, Final Judgment, Lightmine Field and two Elixirs of Immortality; no single decision decides
it (§3 S-3).

Gameend counters, by game (130/125/123/162/146/126): `protocol_replies` 38/145/26/14/21/35,
`ask_replays_reserved` 2/80/1/0/0/3 (= `_cache` 2/78/1/0/0/3 + `crossphase_replayed` 0/2/0/0/0/0,
`_repeat_latch` 0 everywhere — U9's identity holds on all six),
`ask_key_continuation_differs` 3/10/1/1/0/3, `plan_cast_steps_closed` 7/21/9/3/5/6,
`hold_events` 0/8/0/0/0/0 (all 8 `reopen_rows_moved`), `hold_windows_skipped` 0/58/0/0/0/0 (all `_cast`),
`hold_released_turn` 0/6/0/0/0/0, `crackback_verdict_lines_rendered` 1/0/4/0/3/0,
`stack_death_verdict_lines_rendered` 0/5/0/0/0/0, `single_outcome_menus_answered` 1/0/0/0/2/0 /
`single_outcome_rows_spared` 20/0/0/0/40/0, `forced_close_events` 0/2/0/2/0/0 (`_unrecorded` 1 each),
`async_drops` 0/3/0/0/0/0, `blocker_forecast_rows` 4/0/6/0/12/20 (`_gang` 2/0/0/0/0/0,
`_multi` 4/0/0/0/10/12, `_collapsed` 0/0/0/0/2/0), `chain_acting_rows` 0/28/0/0/0/0 (`_cast` 0/132/0/0/0/0),
`crossphase_identical_reputs` 0/17/0/0/0/0, `main_phase_windows_skipped` 14/28/6/0/4/6,
`own_turn_windows_skipped` 38/80/52/43/59/35, `plan_names_uncastable_zone_card` 0/3/1/0/0/0,
`stack_drain_windows_asked` 0/2/0/0/0/0, `identical_option_asks_resolved` 3/0/0/0/0/0.
**Zero on all six**: `crossphase_board_unchanged`, `hold_verdict_safer_ignored`, `hold_reopened_new_lethal`,
`hold_reopened_new_threat`, `hold_windows_skipped_priority`, `own_loop_verdict_lines_rendered`,
`own_loop_windows_asked`, `cached_replay_reasked`, `stop_reached_windows_skipped`,
`stop_reached_reputs_collapsed`, `mana_only_windows_skipped`, `crackback_lethal_blocked_away`,
`menu_pass_no_progress(_suppressed)`, `phase2_answer_recovered/_missing`, `wall_miss_*`,
`force_close_arms_*`, `put_gloss_stripped`, `sibling_window_asks_skipped`, `repeat_annotated_takes`,
`engine_reveal_floor_picks`, `ask_replays_refused`, `identical_ask_answers_reserved`,
`reserve_decline_windows_*`, `chain_selfharm_rows`, `chain_windows_only_selfharm`.

## 1 — Game by game

| # | Opp | Result | Turn | Life | Deciding decisions |
|---|-----|--------|------|------|--------------------|
| 1 | 146 | **loss** | 17 | -10 / 12 | **seq 19 (t12, Attackers) is the game (§3 S-1).** The seat sent its only body — Intrepid Adversary, 6/4 lifelink, the anthem for its whole board — alone into two untapped 4/4s whose tag read `(both die ...)` on BOTH. It was blocked by Barrowin, both died, the seat gained 6, and from t13 it held one Luminarch Aspirant (Ranger Class was exiled by Vanishing Verse the same turn). Every surface was TRUE. Precursor of the same shape at seq 11 (t8, `PLAN: Trade Intrepid Adversary for Nadaar`), where the opponent declined the block. seq 21 (t14) correctly answered `ATTACK: none` on a per-creature STOP; by seq 23 (t16) the crack-back line already read `up to 20 - you would be at -10`, and the seat was dead whatever it did. |
| 2 | 125 | **loss** | 49 | -12 / 66 | UW control. The seat resolved and lost Luminarch Aspirant x3, Katilda x2, Elite Spellbinder x2, Intrepid Adversary x2, Sigarda x2, Brutal Cathar x2 and Briarbridge Tracker to Supreme Verdict / Final Judgment / three Essence Scatters; Lightmine Field priced every attack, two Elixirs of Immortality took the opponent 18 → 66. Last attack was seq 23 (t15); 26 turns of casting into open blue mana followed. The one adherence miss that is priceable is §3 S-2 (MATCHUPS counterspell rule: best-creature-first at U 14 with their hand at 5). The window that produced the corpus's only `unparsed_reply` is seq 101 (§2 HIGH-1). |
| 3 | 130 | win | 15 | 22 / -10 | Teferi t7 (seq 13) into Tovolar's Huntmaster t9, Sigarda t11; Lair of the Hydra animated at t13 (seq 34, **in UPKEEP** — against the guide's MAIN-1-ONLY rule, §3 S-4) and t15, and both animations attacked. Closed at seq 42 on `ATTACK TOTAL ... that KILLS them whatever they block` with five attackers for 23. The `rung ceiling` and `death price` clauses on the Lair rows were read correctly at both animations. |
| 4 | 123 | win | 17 | 20 / -2 | A clean curve — Ranger Class t3, Aspirant t5, Sigarda t7, Katilda t9, Augur + second Ranger Class t11, Brutal Cathar t13, Intrepid Adversary + 1 valor counter t15 (seq 20→21, the 21-row menu ASKED because 4 mana were spendable — the right half of U13). Closed at seq 28 on `that KILLS them whatever they block`. |
| 5 | 162 | win | 11 | 20 / -1 | Fourteen decisions, no combat losses. seq 14 exiled `Peer into the Abyss` with Elite Spellbinder; seq 17 closed on `that KILLS them whatever they block` with three attackers for 11. Two `forced_close` events, both benign. |
| 6 | 126 | win | 13 | 20 / -3 | The Perimeter-Captain lifegain game, and **the guide's ATTACK TOTAL branch paid exactly**: seq 18 (t9) printed `blocking can leave them as high as 38` against their life of 32, and the seat answered `no attackers` — the only window in six games where that branch fired. seq 26 (t11) then swung five for 16 under `At least 16 damage lands whatever they block`, and seq 37 closed on `that KILLS them whatever they block`. |

**Guide machinery that fired and was right**: the `ATTACK TOTAL` last-clause rule on all four wins
(4/4 closing combats took the `KILLS them whatever they block` branch, and `126` seq 18 took the
`as high as N` branch); the LETHAL COUNT tap-bill PASS (14 `{paying this taps}` clauses read, no row taken
with two names billed in Upkeep or Main 1 except `126` seq 25/36, both in a second main phase);
the deploy floor (0 quiet-board declines with an untaken creature row); the MULLIGAN floor and bottom order
(`152v125` seqs 2/3/4 — one mulligan, then a keep, then a correct bottom); the PATHWAY face rule (every face
taken was the one the hand needed); the Fateful Absence STOP (`the only legal targets are YOUR OWN`
appeared in 68 prompts and the seat never cast it into its own Teferi); and the Teferi "activate every turn"
rule (13 loyalty activations across the 130 and 125 games).

## 2 — Engine / interface / card items

### HIGH-1 (ENGINE, a real response window is suppressed and the render says the card is castable) — an INSTANT in hand is not offered while an opposing spell is on the stack, because "you already answered this phase's Casting decision"
`152v125` **seq 101** (t37, Main phase 1). The stack reads:
```
ON THE STACK, waiting to resolve (top resolves FIRST - you can respond now):
  1 (top): opponent's Essence Scatter {1}{u} (instant) [spell] targeting Briarbridge Tracker
  2: your Briarbridge Tracker {2}{g} (creature 2/3) [vigilance] [spell]
```
`Mana available: 6 total`, and the hand line reads:
```
Fateful Absence {1}{w} [instant] [no cast row now: you already answered this phase's Casting decision, so
this window does not offer it - it is otherwise castable and its own casting window this turn has passed]
```
The legal list is two rows: Teferi's +1, and Hold priority. The model answered
`CHOICE: Cast Fateful Absence targeting Essence Scatter` — **the corpus's only `unparsed_reply`** — and the
heuristic played Teferi's +1 (`recovery` record seq 104, `recovers_seq: 101`, `executed_by: "heuristic"`).
Two things are wrong here and only one of them is the model's:
1. **The gate.** The phase's casting dedupe was answered BEFORE the opponent cast Essence Scatter. A spell
   the opponent casts afterwards is a new opportunity, and the seat cannot reach an instant in response to
   it for the rest of that phase. The `HOW A HOLD ENDS` block in the same prompt even names the shape it
   ignores: *"a note that it cannot reach a spell on the stack - is the SAME row"*. **15 windows in this
   game** carry the `already answered this phase's Casting decision` tag on an instant while `ON THE STACK`
   is non-empty (`152v125` seqs 38, 39, 42, 44, 60, 91, 101, 105, 107, 108, 111, 113, 135, 137, 139); in
   most of them the stack holds the seat's own Teferi ability, but seq 101 is a live opposing counterspell.
2. **The render.** Under the trust doctrine `it is otherwise castable` is a true statement in a scope that
   makes it a lie: it tells the model the card is available in a window where no row exists for it. The
   parse refusal was CORRECT (the answer names no row on the list, so it is not a robust-parser miss and
   not an answer-before-plan — `protocol_deviation` on the record is `compliant`, `off_protocol_bytes` 0);
   the render is what produced it.
Repro: `152v125` seq 101 (prompt + reply), seq 104 (`recovery`), and the 15-window list above.
Fix shape: re-open the casting seam for INSTANT-speed cards when a new object reaches the stack after the
phase's casting decision was answered; until then, the hand tag must not say the card is castable.

### HIGH-2 (RENDER, the mechanism is only explained once it is already lethal) — the `CRACK-BACK COST OF ATTACKING` clause renders only when the crack-back kills
`CRACK-BACK NEXT TURN:` appears on **8 of my 25 attackers prompts**, but the `CRACK-BACK COST OF ATTACKING`
block — the one that says *"Declaring an attacker taps it, and a tapped creature cannot block, so every
attacker you declare without vigilance is one fewer blocker against that line"* plus the
`{crack-back cover, STAY HOME: ...}` arithmetic — renders on exactly **one**: `152v146` seq 23, where the
line already reads `up to 20 - you would be at -10; that would KILL you`.
At `152v146` **seq 19** (t12), the decision that lost the game, the same prompt printed
`CRACK-BACK NEXT TURN: 3 of their creatures will be able to attack (tapped ones untap first), for up to 10 -
you would be at 10` and **no cost-of-attacking clause at all**, while the seat's only untapped body was the
attacker it declared. The clause that explains the trade is withheld from every window where the trade is
still avoidable and is printed once at the window where it no longer matters. Same at seq 21 (t14, same
`up to 10` line, no cost clause).
Repro: `152v146` seqs 19 and 21 (line printed, no cover clause) against seq 23 (cover clause printed,
crack-back already lethal); `152v130` seq 35, `152v123` seqs 13/16, `152v146` seqs 11/15 are five more
attackers prompts with the line and no cover clause.
(I did NOT write a guide rule against this surface — there is nothing at that seam for a rule to read.)

### MED-1 (RENDER, the plan echo) — the cast step is dropped at a COMPLETING menu when the plan wrote it as its own sentence
`plan_cast_steps_closed` is 51 across my six games and it holds on two of the three cast→completing-menu
pairs I traced, but not all three:
- `152v123` seq 20 → **21** (cast Intrepid Adversary → its valor-counter menu, which completes the cast):
  reply at 20 was `PLAN: Cast Intrepid Adversary, add 1 counter, attack with Moonrage Brute, level Ranger
  Class in main 2.` and the echo at 21 reads the whole line back. **PASS.**
- `152v125` seq 21 → **22** (same shape, `PLAN: Cast Intrepid Adversary, add 2 valor counters, attack with
  Moonrage Brute.`): echo intact. **PASS.**
- `152v125` seq 15 → **16** (same shape): reply at 15 was `PLAN: Cast Intrepid Adversary and add 1 valor
  counter. Attack with Elite Spellbinder only to survive Lightmine Field. Tap Katilda for mana in main 2.`
  and the echo at 16 reads only `"Attack with Elite Spellbinder only to survive Lightmine Field. Tap Katilda
  for mana in main 2."` — **the cast clause AND the window's own step ("add 1 valor counter") are both
  gone**, at the very menu that executes them. **FAIL.**
The difference between the pass and the fail is punctuation: the two passes wrote the cast and the counter
as comma-joined clauses of one sentence; the fail wrote them as a full sentence that the trimmer consumed
whole. A fourth pair shows the same sentence-granularity: `152v126` seq 19 → 20, `PLAN: Cast Brutal Cathar
to exile a Perimeter Captain, then level Ranger Class to 2 with the remaining {G}.` echoes at the ETB TARGET
window as `"then level Ranger Class to 2 with the remaining {G}."` — the clause naming the target is dropped
at the window that picks the target. (The seat picked Perimeter Captain anyway.)
Fix shape: the trimmer's unit is a plan STEP, not a sentence or a `, then` fragment; a step that names the
choice the CURRENT window is making must never be trimmed from that window's echo.
Repro: the four pairs above.

### MED-2 (PROMPT COST) — the narration budget is not board-aware, and a long game spends 30% of this seat's windows over 20 KB
85 of my 280 prompts exceed 20 KB (max **23,241 B**); **83 of the 85 are the 49-turn `152v125` game**
(mean 18,219 B there, vs 6,130-11,033 B mean in the other five). The T10 trim is clearly working and is
byte-anchored, not turn-anchored — at `152v125` seq 215 it kept 16 turn headers and folded everything
earlier into one `(...earlier events trimmed - graveyards at trim: ...)` line — but it stabilises the
NARRATION at ~15 KB independently of everything else, and in that game the `--- CURRENT SITUATION ---`
block alone is 6.8 KB (20 opponent permanents, 13-card graveyard, a 3-artifact line). 15.3 + 6.8 = 22.1 KB.
Fix shape: budget the narration against the ASSEMBLED prompt size, not against itself — the board section's
size is known before the narration is trimmed.
Repro: `152v125` seq 215 (narration 15,298 B of 22,143 B total) and seqs 20 / 53 / 80 / 101 / 160 / 180
(narration 4,377 → 13,523 → 14,930 → 13,089 → 13,375 → 15,048 B, turn headers 13 → 32 → 34 → 25 → 19 → 20).

### PASS-1 (U13 / DF) — the 21-row Adversary menu is engine-answered when the mana pays for zero repeats, and is still ASKED when it pays for one or more
All three corpus `menu_single_outcome` records are mine, and all three are the Intrepid Adversary valor
menu: `152v130` seq 6 (t3), `152v146` seq 6 (t4) and seq 8 (t6), each `rows: 21`,
`repeat_payments_paid: 0`, `answered: 0`, `answered_text: "don't add any counter"`,
`subject: "Intrepid Adversary"` — 20 rows spared each, 60 total, matching
`single_outcome_rows_spared` 20 + 40.
**Every row on those three menus did resolve to one outcome**: each fired one window after the cast row
whose own annotation read `{leaves 0 of your 2 untapped mana sources untapped - casting this taps you out}`
(`152v130` seq 5, `152v146` seqs 5 and 7), so 0 mana was spendable and every rung bought 0 counters.
The discriminator is sound in the other direction too — the three windows this wave where mana WAS
spendable were **asked** with the full 21 rows and the seat answered a real rung:
`152v125` seq 16 (`{repeat cost: 1 x {1}}`/`2 x`/`20 x` clauses present, answered `add 2 counters`),
`152v125` seq 22 (`add 2 counters`), `152v123` seq 21 (`add 1 counter`). Wave-79 HIGH-2 is CLOSED.

### PASS-2 (U4 / DF) — one tap bill per row, and the bill now states its own count
**14 `{paying this taps: ...}` clauses across my six games, and 0 rows carrying more than one clause**
(wave 79: three rows with 2-3 clauses each, none stating more than 2 names, on a row that cost 3 attackers).
The fix also added the count and its comma warning where a card name contains a comma, e.g. `152v130`
seq 22: `{paying this taps: Katilda, Dawnhart Prime, Luminarch Aspirant (that is 2 permanents of yours -
one of the names in this list contains a comma, so count them from this number, not from the commas) -
they cannot attack this turn}`, and `152v126` seq 25 with `(that is 3 permanents of yours ...)`.
The ability's own `{T}` is folded into the same clause now — `152v126` seqs 21/29/33 print
`Katilda, Dawnhart Prime (this card itself)` inside the one bill. Wave-79 HIGH-1 is CLOSED, and I have
rewritten the guide's LETHAL COUNT rule to read the printed number instead of counting names (EDIT 2).

### PASS-3 (U8/DE + DH F12) — both of my `crossphase_replayed` traced, and the replayed answer was right for the new phase
`askreplay/1789216920-ai_baka_deck152-...-vs-ai_baka_deck125.jsonl` carries 80 entries: 78
`why: "cache_replay"` and the two crossphase ones, both `replayed_from: 203` (a record seq, as DH F12
specifies), `why: "crossphase_replay from Main phase 1 to Combat begins"` (seq 204) and
`"... to Attackers"` (seq 205), t44, `replays_this_game` 64 and 65.
Record seq 203's three rows were: (1) `Cast Fateful Absence ... {kills whichever you target: THEIRS - none;
YOURS - Teferi, Who Slows the Sunset} - the only legal targets are YOUR OWN right now`, (2) Hold priority,
(3) `Cast nothing right now`. The seat answered row 3, and row 3 remains correct in both replayed phases —
the only castable row would have destroyed the seat's own planeswalker. The other four games' replays are
all `cache_replay` (2/1/0/0/3).
`ask_replays_reserved` = `_cache` + `_repeat_latch`(0) + `crossphase_replayed` holds exactly on each of my
six gameends (U9).

### PASS-4 (U2/DE + DH F3/F4) — every crack-back and stack-death verdict line I have is TRUE
8 `crackback_verdict_lines_rendered` (`152v130` seq 34; `152v123` seqs 11, 12, 14, 15; `152v146` seqs 9,
13, 17) and 5 `stack_death_verdict_lines_rendered` (`152v125` seqs 53, 122, 148, 174, 198) — counts match
the gameends exactly, and all 13 read "you survive". I checked the three on the game I LOST against the
same prompt's own board line: `152v146` seq 9 (`CRACK-BACK NEXT TURN ... up to 3 - you would be at 17`,
board: 1 creature, Nadaar 3/3), seq 13 (`up to 5`, 3 creatures, → 16), seq 17 (`up to 10`, 3 creatures,
→ 10). All three survive figures are correct and none was on a suppressed window. `crackback_lethal_blocked_away`
is 0 on all six of my games, so the `with your best block` floor is UNTESTED here.

### LOW-1 — all 8 of my `hold_event` records are a class the brief's reconciliation does not name
`152v125` seqs 50, 52, 121, 145, 147, 173, 188, 212: every one is `what: "reopen_rows_moved"`, `seam: "cast"`,
`face: ""`, reason either `a printed row it was held over is gone` or `a printed row changed or is newly
available`. My `hold_verdict_safer_ignored`, `hold_reopened_new_lethal` and `hold_reopened_new_threat` are
all 0, so the corpus-wide `hold_events` 180 = 71 + 7 + 0 + **?** residual is at least partly this
`reopen_rows_moved` class. Each of the 8 has a `window_seq` that resolves to an earlier record in the same
file, so the kind is traceable — it just needs naming in the census.

### LOW-2 — the one `reveal`/`engine_answered` is genuinely zero-eligible
`152v130` seq 43 (t15, Blockers), `options: 5`, `chosen_text: "none (no legal target)"`. Compare seq 36 in
the same game, `options: 5`, answered by the model as `none` with no fallback — so the seam does ask when
there is something to pick. Engine answering a no-legal-target reveal is right; it is not a model failure
and should not be counted against the ask seam.

### LOW-3 — `async_drops` 3 and `forced_close_unrecorded` 1 per affected game
`152v125` carries `async_drops: 3` with no per-drop record to join, and `152v125`/`152v162` each carry
`forced_close_events: 2` with `forced_close_unrecorded: 1` — i.e. the two `forced_close` records I can read
(`152v125` seqs 87/89, `152v162` seqs 9/10) do not account for the counter. Same shape as wave-78 MED-2,
smaller. Not worth a HIGH; noting it so the census is not read as reconciled from this seat.

## 3 — Strategy (guide-adherence) observations

**S-1 — THE DECIDING MISPLAY: the anthem traded itself for a vanilla 4/4.**
`1789216945-ai_baka_deck152-0x55f869719e20-vs-ai_baka_deck146.jsonl` **seq 19** (t12, Attackers). Life 20,
opponent 12. The whole attackers list:
```
A1. Intrepid Adversary (6/4) [lifelink] [their untapped blockers: Nadaar, Selfless Paladin (4/4) (both die
(lifelink: you gain 6 from this block only, and this attacker deals nothing to their life)); Barrowin of
Clan Undurr (4/4) (both die (lifelink: you gain 6 from this block only, and this attacker deals nothing to
their life))]
They have 2 untapped creatures able to block; declaring more than 2 attackers leaves at least (your
attackers - 1) of them unblocked.
NOT offered above and NOT able to attack this turn (summoning sick) - Luminarch Aspirant.
ATTACK TOTAL: 1 attacker listed, 6 total combat damage to a player - declaring all of them with none blocked
puts them at 6. That is NOT lethal: they survive at 6 even with none of them blocked. Their 2 untapped
blockers that can legally block at least one of these attackers can cover every attacker you could send, so
none of that damage is guaranteed.
CRACK-BACK NEXT TURN: 3 of their creatures will be able to attack (tapped ones untap first), for up to 10 -
you would be at 10
```
Answer: `PLAN: Attack with Intrepid Adversary to push damage.` / `ATTACK: Intrepid Adversary`. The next
prompt's narration (seq 20) records the result: `Opponent's Barrowin of Clan Undurr dealt 4 damage to
Intrepid Adversary / Your Intrepid Adversary dealt 6 damage to Barrowin / You gained 6 life (now 26) /
Your Intrepid Adversary died / Opponent's Barrowin of Clan Undurr died`, and one line later
`Opponent cast Vanishing Verse / Your Ranger Class was exiled`. From t13 the seat's whole board was one
Luminarch Aspirant; it lost at t17 on -10 with the opponent still at 12. **Every surface was TRUE and
complete** — this is STRATEGY, not perception.
What the guide said: nothing. The ATTACKING per-creature ladder has a rule for
`"(you kill it, your attacker lives)"`, for `"(neither dies)"`, for
`"your attacker dies, their blocker lives"`, for `GANG BLOCK:` and for blocking triggers — and **no rung at
all for `"(both die)"`**, which is the tag on both entries here. The BLOCKING ladder has had a worth-test
for exactly that tag since wave 79 (rung 2: a token or an ability-less body is worth less than any ENGINE
of yours). The attacking side never got the mirror. EDIT 1 installs it.
I measured the rule's blast radius mechanically over all 25 of my attackers prompts: **an engine facing a
tag whose every outcome reads `(both die)` occurs in exactly 2 windows, both in this game** — seq 11 (t8,
Adversary 4/2 vs Nadaar 3/3; the opponent declined the block and took 4) and seq 19. It fires in **none** of
the four wins, and the lethal carve-out covers all four closing combats. The honest cost of the rule is the
4 damage at seq 11; the opponent in that game never went below 12 either way.

**S-2 — the MATCHUPS counterspell rule pointed at the cheap creature and the seat cast the best one.**
`152v125` **seq 84** (t35): their battlefield line showed **14 untapped sources** and
`Opponent hand size: 5`. The guide's rule is `U of 3 or more with their hand at 5 or more -> cheapest
creature first`. The seat cast Sigarda ({1}{G}{W}{W}, the best card in the deck) and it was countered by
Essence Scatter; it then cast Brutal Cathar ({2}{W}) at seq 92 and that was countered too. The same order
inversion happened at seq 57 (t33, Brutal Cathar first — correct) and seq 98 (t37, Briarbridge Tracker into
the third Essence Scatter). **No edit**: the rule is correctly worded, it was simply not followed once, and
with two Essence Scatters up that turn the order changed nothing observable. Recorded because it is the
only priceable adherence miss in that game.

**S-3 — the 125 game is not decided by a decision I can name.** Board wipes (Supreme Verdict at t~21,
Final Judgment), three Essence Scatters, Lightmine Field taxing every attack, and two Elixirs of
Immortality that took them 18 → 66 while shuffling their graveyard back. The seat's last attack is seq 23
(t15). Nothing in the deck answers an artifact or an enchantment; Fateful Absence spent the whole late game
printing `the only legal targets are YOUR OWN right now` (68 prompts corpus-wide on this seat) and the
guide's STOP rule correctly held it every time. I do NOT propose a guide rule for this: the deck has no
line here, and a rule teaching it to do something it cannot do is worse than silence.

**S-4 — Lair of the Hydra animated in UPKEEP, against the MAIN-1-ONLY rule, and the game was won.**
`152v130` seq 34 (t13 Upkeep): the seat took `becomes a 3/3 hydra`, read the `{rung ceiling: Lair of the
Hydra's OWN mana is not counted toward this activation ...}` clause correctly, attacked with it at seq 35 in
a three-attacker swing, and repeated at t15 (seq 41, 4/4) in the lethal combat. The cost is visible: its
stated plan at seq 34 was `Animate Lair of the Hydra to 3/3 in upkeep, cast Luminarch Aspirant in main 1,
attack with ...` and **no main-phase casting window was put to the seat at t13 at all** (the record sequence
runs seq 34 Upkeep → seq 35 Attackers; `main_phase_windows_skipped` is 14 in that game) — the animation
consumed the mana the creature needed, which is exactly what the MAIN-1-ONLY rule protects. **No edit**
(A334 — the rule is right and the hard case earns no concessive clause); recorded so the next seat does not
write a qualifier for it. None of the three tags the rule lists as a PASS (`[this land is TAPPED:`,
`{right now: does nothing this turn}`, `[repeat: activated this turn`) rendered anywhere in my six games;
`[Upkeep offer:` rendered in 5 prompts.

**S-5 — two protocol shapes, both harmless.** The 5 `plan_line_missing` are 4 `unlabelled_plan` (a correct
one-line plan with the label dropped) and 1 `plan_absent`. No record carries prose outside the two lines;
the one refusal (§2 HIGH-1) is an off-list ANSWER, not a malformed reply.

## 4 — Guide verdict: **EDIT** (19,963 B → **19,918 B**, net −45)

One decision in six games was decided against the seat by a tag the guide's attacking ladder does not
mention at all (S-1), and it decided the 146 loss. That is the only new rule. The second edit is a
REWRITE, not an addition: wave-80's U4 fix means the tap bill now prints its own count, so the guide stops
teaching the model to count names and points at the number instead. The guide shrinks.

**EDIT 1 — ATTACKING. A new `"(both die)"` rung, mirroring BLOCKING's rung 2.**
*Paid for by `152v146` seq 19 (and its precursor at seq 11).*
before: (no rung for that tag; the ladder jumped from `"(you kill it, your attacker lives)"` to
`"(neither dies)"`)
after, inserted between them:
```
- "(both die)" is a TRADE THEY CHOOSE. Send a token or a body whose row prints no ability. An ENGINE of
  yours - anything whose text makes mana, counters or bodies: Intrepid Adversary, Katilda, Luminarch
  Aspirant, Sigarda, Ranger Class, Tovolar's Huntmaster - stays home when EVERY outcome on its tag reads
  "(both die)", unless the lethal check has cleared or your declared attackers OUTNUMBER their untapped
  blockers. A lifelink number there is what they pay for the trade, not a reason to make it, and the
  +1/+1 that body gives your board dies with it.
```
**+564 B.** Earns its bytes because the tag that lost the game had no rule, because the "EVERY outcome on
its tag reads X" test and the OUTNUMBER carve-out are copied verbatim from the `"(neither dies)"` rung
directly below it (so the model is applying a shape it already applies), and because the lethal carve-out
keeps all four of this seat's winning closes legal. Heading-free bullet inside an existing scope (A333
satisfied by the section heading); no concessive clause for the case where the trade is right (A334) — the
two carve-outs are conditions, not concessions.

**EDIT 2 — LETHAL COUNT. Read the bill's printed number instead of counting names.**
*Paid for by the U4 fix landing: 14 bills read this wave, 0 split rows, and the multi-name bills now print
`(that is N permanents of yours - one of the names in this list contains a comma, so count them from this
number, not from the commas)`.*
before: `SEARCH EVERY CAST ROW FOR\n"paying this taps" BEFORE YOU TAKE IT - the source count beside it
counts lands and creatures alike and never names a body. COUNT THE NAMES IN THE BILL AND OBEY THE COUNT:
two or more names in Upkeep or Main 1 is a PASS, ... with exactly ONE name take the row only if`
after: `SEARCH EVERY CAST ROW FOR "paying this taps" BEFORE YOU TAKE IT AND OBEY THE BILL: two or more of
your permanents billed in Upkeep or Main 1 is a PASS, ... with exactly ONE billed take the row only if`
**−95 B** (plus +2 B of re-wrapping across the two paragraphs I touched). The deleted half explained a source count that is no longer the thing being read, and
"count the names" is now the wrong instruction on a row like `152v130` seq 22, where three comma-separated
names are two permanents and the clause says so.

**Reclaims** (no rule is dropped):
- R1, ATTACKING: removed `A creature tagged "[held back, THIS creature could not block ANY of their N
  creatures]" guards nothing by staying home - keeping it back buys you no blocker at all - so send or hold
  it on its own per-creature tag alone.` and shortened `Never hold a name back on memory or on your plan:
  the A-lines are the whole list of who can go, and the prompt names who cannot and why.` to its first
  sentence. **−307 B.** The tag rendered in **1** of my 280 prompts and its own bracket states its meaning;
  the deleted trailing half is restated by the attackers prompt's own
  `NOT offered above and NOT able to attack this turn (summoning sick) - <names>. A plan that names one of
  them as an attacker THIS turn cannot be executed.` block (rendered in 8 of my attackers prompts).
- R2, BLOCKING rung 2: `A TOKEN, or a body whose row prints no ability, IS worth less than any ENGINE of
  yours - anything whose text makes mana, counters or bodies, which is Katilda, Luminarch Aspirant, Sigarda
  and Ranger Class.` → `... IS worth less than any ENGINE of yours, named under ATTACKING.` **−94 B.**
  The class is now defined once, in EDIT 1, and the definition there is the SUPERSET (it adds Intrepid
  Adversary and Tovolar's Huntmaster, both of which the wave-79 list omitted — Adversary is the anthem that
  S-1 threw away).
- R3, MANA: `- "Mana available: N total (... colours you can make: {g}{w})": N is what you spend, the colour
  list is which COLOURED pips you pay.` → `- "Mana available: N total": N is what you spend; the colour list
  is which COLOURED pips you can pay.` **−31 B**, wording only; the prompt prints the parenthetical
  in full on every window.
- R4, MULLIGAN: removed `A one-colour opener is never dead: the other colour arrives within a turn or two.`
  **−84 B.** The bullet two lines above already rules that a colour reason is never a ship above the floor
  (`A COLOUR reason ("no spell in it is castable off {W}{U} alone") is never a ship above the floor`).

Full revised guide at `wave80/deck152/strategy.txt`, **19,918 B**. Scanned: no citation, count, history,
corpus, wave or seq reference; no `YOUR PLAN` quotation; no PLAN template; no line over 112 columns.

## 5 — Proposals

**None.** HIGH-1, HIGH-2, MED-1 and MED-2 are engine/render items with repros, not instruction-surface
questions. S-4 is an A334 case already settled by amendment. Nothing here generalises past this deck in a
way the general guide or the strategy-writing skill does not already carry.

## 6 — What I did NOT check

- **Any seat but 152's.** I opened the six opponent files only to confirm results and never traced their
  decisions; nothing here adjudicates a corpus-wide counter.
- **Lanes DI and DJ are structurally UNTESTED from this seat**: none of my six games is the rerun, so all
  six ran on `a7a6b8942`. `cached_replay_reasked` is 0 on all six and cannot be otherwise. No Intruder
  Alarm, no Thraben Doomsayer, and no object-creating repeat row appeared in any of my games.
- **`hold_verdict_safer_ignored` and `hold_reopened_new_lethal`: 0 on all six.** The corpus's 71 and 7 are
  elsewhere; I verified nothing about them. Likewise `crackback_lethal_blocked_away` (0 here — the
  `with your best block` floor is unverified from this seat), `own_loop_verdict_lines_rendered` /
  `own_loop_windows_asked` (0), `crossphase_board_unchanged` (0), `mana_only_windows_skipped` (0),
  `stop_reached_*` (0), `menu_pass_no_progress` (0), `phase2_*` (0), `wall_miss_*` (0).
- **`ask_key_continuation_differs` (18 across my six games) has no per-record join key.** Wave-79 MED-1
  stands unchanged for this counter: the replayed window writes no decision record, so I can report the
  number and not one verdict about whether a continuation genuinely moved. `askreplay/` gave me the
  `replayed_from` seq for the 4 replays with entries, which is why PASS-3 was checkable and this is not.
- **The `(...more)` gloss cuts (U12).** I counted them mechanically but opened none this wave; the six
  gloss/render fixes (Sigarda, Puzzle Box, `[legendary:` loyalty, Sorin, mode objects) are unverified from
  here — `[legendary:` rendered in 15 of my prompts and I read only the two the seat answered
  (`152v123` seq 15, `152v125` seq 183), both of which named the duplicate and were handled by the guide's
  deploy-floor case (c).
- **U3's cover clauses and U7's `board sweep:` markers: 0 renders in my six games.** No ETB self-leave
  candidate, no Acererak-class row, no sweep marker, and 0 cycling rows (U6). All UNTESTED here, not
  confirmed.
- **U14's blockers header folding compulsory draw damage.** My 4 blockers windows (all in `152v146`) had no
  draw punisher on either side; `blocker_forecast_collapsed` 2 / `_gang` 2 and I verified neither figure
  against an independent DP.
- **No card was verified against Oracle text this wave.** Intrepid Adversary, Katilda, Luminarch Aspirant,
  Sigarda, Brutal Cathar, Ranger Class, Tovolar's Huntmaster, Lair of the Hydra, Fateful Absence, Elite
  Spellbinder, Teferi, Nadaar, Barrowin, Triumphant Adventurer, Silverquill Silencer, Essence Scatter,
  Elixir of Immortality, Lightmine Field and Staff of Nin all behaved as their rendered text says, but
  **I opened no primitive and no Scryfall page** — including for the EDIT 1 engine list, whose membership I
  took from the rendered `card text:` strings in my own prompts.
- **stderr and `corpus-stats.py`.** I read neither; every count above comes from the six jsonl files and
  the four `askreplay/` files for my seat. The census reconciliation, `dropped after its X was announced`,
  `deadline_pct`, `transport` and the per-seam cost table are the engine seat's.
- **Teferi's -7 emblem** was never offered or considered in the 125 game and I did not work out whether it
  was reachable; the guide's Teferi bullet says take a loyalty ability every turn and the seat did.
