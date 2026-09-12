# Wave 80 — deck 162 ("Draw and Die!", UB forced-draw punish) per-deck review

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260912-074153-final`, **MIXED binary**: five games on
`wagic-a7a6b8942-w80step1` (source `cf72a002f`); **`162v123` is the RERUN** on `wagic-ba5cdc374-w80step1b`
(`337becbf2`, + lanes DI/DJ) after attempt 0 hung in the Doomsayer + Intruder Alarm cached-replay loop.
Six deck162 seat translogs plus the six opponent seats for outcomes and log narration.

**168 model decisions on this seat, 0 heuristic fallbacks, 0 `reply_truncated`, 0 `action_before_plan`,
`thinking: on` and native reasoning on 168/168** (median `reasoning_chars` 6,405, max 18,420).
`transport` `curl=0,http=200,empty=0` on all 168; `deadline_pct` max 35.0 (ceiling 900 s).
`protocol_deviation`: compliant 162 / `unlabelled_plan` 6 / `plan_absent` 0 / `answer_label_absent` 0 —
`plan_line_missing` 6 of 168 = **3.6%** (wave 79 on this seat: 7.4%); `off_protocol_bytes` > 0 on the same
6 windows, max 210 B. In all 6 the plan sentence still PRECEDES the action line; only the `PLAN:` label is
missing. Latency p50 **114.7 s**, p95 222.6, max 315.7 (corpus p50 87.0). Prompt bytes: max **25,717**,
median 9,212, **10 of 168 windows over 20 KB** (wave 79: max 20,199, 1 over) — see MED-3.
Gameend sums over the six games: `hold_events` 6 (`reopen_rows_moved` 5 + `reopen_new_lethal` 1),
`hold_released_turn` 5, `hold_windows_skipped` 47 (cast 45 / priority 2), `own_turn_windows_skipped` 162,
`main_phase_windows_skipped` 8, `crackback_verdict_lines_rendered` 76, `crackback_lethal_blocked_away` 6,
`stack_death_verdict_lines_rendered` 0, `plan_cast_steps_closed` 45, `ask_replays_reserved` 8 =
`ask_replays_cache` 8 + `_repeat_latch` 0 + `crossphase_replayed` 0 (U9 identity holds on this seat),
`ask_key_continuation_differs` 13, `crossphase_identical_reputs` 10, `crossphase_board_unchanged` 0,
`identical_option_asks_resolved` 3, `forced_close_events` 4 / `_unrecorded` 2, `async_drops` 0,
`blocker_forecast_rows` 2 / `multi` 2 / `gang` 2 / `collapsed` 0, `chain_acting_rows` 14 /
`chain_selfharm_rows` 6, `plan_names_uncastable_zone_card` 1, `cached_replay_reasked` 0 (rerun game only),
`menu_single_outcome` 0, `own_loop_*` 0, `stop_reached_*` 0, `stack_drain_windows_asked` 0,
`hold_verdict_safer_ignored` 0, `hold_reopened_new_threat` 0, `mana_only_windows_skipped` 0.

**Record 3–3** (wave 79: 4–2). **Both wave-79 render items are FIXED and confirmed** (Puzzle Box priced off
the live hand; `[legendary:` carries loyalty, in both the Ob Nixilis and the Teferi render). **U5's neutral
exemplar shipped** and the anchoring trace is gone — but the row-1 base rate did not move (below).
The two losses that are not variance are a colour screw (`162v130`) and a two-card loop (`162v126`); the
third (`162v152`) is a flood with both threats removed from play. **Two guide violations in 168 decisions,
both in games the seat WON, both with a mechanism visible in the reasoning** — they are what the EDIT pays
for.

## 1. Game by game

| Opp | Result | Deciding decisions |
|---|---|---|
| 123 | **WIN** t12, 26–0 | **The rerun.** 19 decisions, 1 deviation, 0 fallback, and the hang state never recurred (§4). Curve: Shield Sphere t1 (seq4), Howling Mine t3 (seq6), Underworld Dreams t5 (seq8), Dictate t7 (seq10 — K=1, brake released correctly), second Dreams t9 (seq12), **Fog Bank t9 (seq13)** on `Thraben Doomsayer` (rung 4), Fate Unraveler t11 (seq15). At **seq16 (t12 Blockers)** it blocked Doomsayer with Fog Bank (`neither dies`, prevention) and named the real reason: *"Opponent will draw 3 cards next turn and lose to draw damage."* True: they were at 1, three punishers × 3 cards. At **seq19** it correctly answered `Cast nothing right now` with the win already locked, then at **seq21** took the Ob Nixilis -2 on Doomsayer for 4 × 2 = 8 and ended it. Nothing on this seat needed the DI fix. |
| 125 | **WIN** t16, 21–0 | 28 decisions, 0 deviation, 0 fallback — and **the one guide violation that cost material: `seq 13`, HIGH-1.** Fate Unraveler t8 (seq7), Ob Nixilis t10 (seq9), four clean Fate Unraveler attacks on empty boards (seq11/15/21/28, every A-line printing no blocker tag), Howling Mine + Master t14, **Forced Fruition t16 (seq25)** then Liliana's Caress (seq26) with the kill already counted. At seq13 (t12) it cast a SECOND Ob Nixilis over a live 5-loyalty copy, tapping out and forgoing Master of the Feast and Howling Mine on the same menu; at seq14 it gave up the new copy. Card + 5 mana for nothing, exactly as in wave 79 — but this time the render carried the loyalty. |
| 146 | **WIN** t16, 1–0 | 29 decisions, 0 deviation, 0 fallback, won at **1 life** — and **the second violation, `seq 23`, HIGH-2**, is why it was 1 and not 7. Shield Sphere t2, Howling Mine t4, Master t6, Fate Unraveler + second Sphere t8, Underworld Dreams + Fog Bank t10, Ob Nixilis t14 with the -2 on Nadaar the same main phase (seq21/22 — the same-turn -2 window confirmed a fourth wave). Three blocker windows, all on rung (seq12 `B1:A1`, seq16 `B1:A2, B2:A1`, seq19 `B1:A1, B2:A3`). At seq23 it sent Fate Unraveler into a tag whose FIRST clause was `GANG BLOCK: any 2 of them together deal 6, enough to kill this attacker`; two Silverquill Silencers ganged it, **Fate Unraveler died** for 3 damage and one Silencer, and the deck lost its only punisher body two turns from the end. |
| 152 | **LOSS** t10, −1–20 | 29 decisions, 0 deviation, 0 fallback, **every one of them on rung and none of them available.** Shield Sphere t2 (seq4) was **exiled by Brutal Cathar on t3** (`162v152` seq6 log); Peer into the Abyss was **exiled from hand by Elite Spellbinder at t9** (seq24 log). From t8 the hand is Puzzle Box + Dictate + four lands, K = 0, `of which 0 are creatures` on the seat's own line: the brake holds both engines and there is no wall, no punisher and no Caress to take instead. The seat answered hold/decline through t8–t11 and died to an 11-point crack-back at 10 life. **I checked the counterfactual**: casting either engine at seq32 (t10) adds no blocker and no damage at K = 0, so the 11 still kills at 10 life. The brake is exonerated on this board, not merely unfalsified. |
| 126 | **LOSS** t12, 0–23 | 17 decisions, 0 deviation, 0 fallback, all on rung. Underworld Dreams t6 and t8, Ob Nixilis t10 with the -2 on Wall of Omens (seq13/14), Master + Dictate t12 (seq17/18), then **seq19: the -2 on Pride Guardian, which drew them Idyllic Tutor and SANGUINE BOND with Exquisite Blood already on their battlefield since t9.** The loop closed on their t13 and took the seat from 17 to 0 in one turn. §SANGUINE BOND governs both-halves-out and the seat obeyed it everywhere it applied; **no `LOOP HALF PENDING:` line was rendered on this seat at any point**, correctly — that clause fires only when the missing half is a card the seat has been SHOWN (verified against `123v126`, where it does fire off a revealed hand), and here it was in their library. I am not calling this guide-addressable: the deck's only clock is making them draw, the alternative is conceding the race, and one occurrence is not a rule. It is recorded because it is the one shape where this deck's engine is also their tutor. |
| 130 | **LOSS** t23, 0–5 | 46 decisions, 4 deviations, 0 fallback. A ten-turn colour screw: from t11 to t19 the seat's line reads `colours you can make: {u}` while the hand holds Ob Nixilis {3}{b}{b}, two Master of the Feast {1}{b}{b}, Underworld Dreams {b}{b}{b} and Liliana's Caress {1}{b}. Every land drop offered was taken (10/10) and black taken first wherever offered (seq26 Swamp over Island, seq42 Swamp over a tapped dual). The K = 0 brake then held the only two castable rows (Puzzle Box, a SECOND Howling Mine) at seq39/40 — correct by the rule, and there was nothing else. **MED-1 sits on `seq 47`**: the cleanup menu's new `{spare:}` default told it to discard a **Swamp** while the board made two black sources and the hand held three `{B}{B}{B}` Underworld Dreams. The seat obeyed. Deck130 then swept with Starstorm X=6 at t22 and burned the seat out with two Pyrite Spellbombs — damage no wall stops, and the crack-back line at seq42 had named exactly that (`ADD THOSE UP ... 4 more damage`). |

## 2. Engine / interface / card items

**HIGH-1 — `162v125` seq 13. The CAST ORDER ladder licenses the legendary row that another section
forbids, and the reasoning shows the model asking for the prohibition and not finding it.**
This is a GUIDE defect surfaced by a now-correct render, not a render defect: the row printed
`[legendary: you already control Ob Nixilis, the Hate-Twisted at 5 loyalty - legend rule: casting this
sends one copy to your graveyard (you choose which)] {leaves 0 of your 5 untapped mana sources untapped -
casting this taps you out}` — wave-79 MED-2's fix, shipped and TRUE. The reasoning trace walks the ladder
literally: *"Skip rung 0 ... Skip [1a] ... '1b. UNDERWORLD DREAMS or OB NIXILIS otherwise.' I have Ob
Nixilis in hand ... 'Any punisher in the list -> cast it THIS ask, ahead of every other card, at every life
total' So CHOICE: 1 ... is correct."* It then asks the question outright — ***"Is there any reason NOT to
cast it?"*** — and answers by re-quoting the MENU ROW, not the guide. It also runs the LAST CHECK
(*"K of 0 on the row you picked AND your hand naming none of the three punishers -> your answer is WRONG.
Ob Nixilis is a punisher. So it's fine."*) and passes. The rule that forbids the row lives in
`=== THE {reserve:} CLAUSE AND FLASHED ENGINES ===` — a heading that declares a different scope — and is
reached by neither the ladder nor the check, which are the two places the decision is actually made.
Mechanism, not diligence: an absolute imperative ("ahead of every other card, at every life total; a
printed row is payable") outvoting a stop stated once elsewhere. Cost: one card, 5 mana, and Master of
the Feast + Howling Mine forgone on the same menu. Paid the EDIT (below).

**HIGH-2 — `162v146` seq 23. The seat read the GANG BLOCK clause correctly and attacked into it, because
§LETHAL CHECK's "EVERYTHING GOES" was applied without its condition and the crack-back sentence was read as
a BLOCKING rule.** The A-line:
`A1. Fate Unraveler (3/4) [their untapped blockers: GANG BLOCK: any 2 of them together deal 6, enough to
kill this attacker; each result below is a LONE blocker only - Silverquill Silencer #2 (3/2) (you kill it,
your attacker lives); Silverquill Silencer #3 (3/2) (you kill it, your attacker lives)]`, with
`ATTACK TOTAL: ... That is NOT lethal: they survive at 6` on the same screen. §ATTACKING's stop is exact:
a GANG BLOCK clause anywhere in a tag is a STOP *below the lethal check*. The reasoning never cites it. It
cites two other things instead: (a) the crack-back sentence — *"'A CRACK-BACK NEXT TURN line reading "that
would KILL you" is the REASON that body stays home: it blocks one of those attackers.' **This refers to
blocking, not attacking.**"* (said twice), and (b) §LETHAL CHECK — *"For attacking, it says: 'EVERYTHING
GOES once this clears: swing with everything, Fate Unraveler included.'"* — with the check visibly NOT
cleared. It then prices the risk and takes it anyway: *"If they block with 1, I win the trade. If they gang
block, I die. **Either way, attacking is correct due to crack-back.**"* They ganged; Fate Unraveler died
(`162v146` seq27 log). The seat won at **1 life**. Two clauses each of which, read alone, reverses a stop
— both fixed in the EDIT, both cheaply. No render item here: every figure on that screen was true.

**MED-1 — the cleanup discard's new `{spare:}` land verdict is computed from land COUNT and GENERIC cost
and is blind to COLOUR; it marked a Swamp "spare" on a board with two black sources and a hand of three
`{B}{B}{B}` spells, and the menu instructs the seat to take it BY DEFAULT.**
`1789216937-ai_baka_deck162-0x562b6751c5a0-vs-ai_baka_deck130.jsonl` **seq 47** (t21 cleanup). Header:
`BY DEFAULT, discard the spare land. 2 rows below are marked {spare: ...} - a surplus land this hand no
longer needs, and the rows are ordered most disposable FIRST`. Row 2 is
`Swamp (land) {card text: B} {spare: you control 5 lands already; the most expensive card in your hand you
could still reach costs 3}`. On the same screen the hand is `Liliana's Caress {1}{b}`, two
`Master of the Feast {1}{b}{b}`, **three `Underworld Dreams {b}{b}{b}`**, and the battlefield's five lands
are Drowned Catacomb + Island ×3 + Swamp — **two black sources**. "The most expensive card you could still
reach costs 3" is false of every one of those cards: `{B}{B}{B}` is not reachable on two black sources, and
nothing in the clause looks at colour. The seat obeyed (`PUT: 1, 2`) and did not cast Underworld Dreams
until **t23** (seq49), one turn before it died at 0. The same window prints
`colours you can make: {u}{b} (sources that can make each: {U} 1, {B} 1)` — the inputs the verdict needs are
already rendered two lines away. **Fix**: price the spare test against the hand's COLOUR requirements, not
its converted cost; a land that is one of N sources of a colour the hand needs N-or-more of is not spare.
**NOT established**: that keeping the Swamp wins the game — deck130 killed with Starstorm + two Pyrite
Spellbombs, which the seat's board never answers. What I establish is that the clause states a false fact
and that the seat acted on it.

**MED-2 — `{crack-back cover:}` is TRUE, trample-aware and honest, and it is also the biggest single
string in the prompt — and it reprints its whole standing explanation on EVERY creature row of the menu.**
`162v146` **seq 30** (t16, 25,717 B, the largest prompt on this seat in two waves): rows 2 and 4 each carry
a ~1.9–2.3 KB `{crack-back cover:}` clause, and the 416-byte paragraph beginning `HOW A BODY REMOVES POWER
FROM THAT TOTAL: an attacker that is BLOCKED deals its combat damage to the blocker, not to you - all of
it, whatever the blocker's size, and a 1/1 in front of a 5/5 stops the whole 5. ...` is printed **twice on
that one window** (33 prints over 24 windows corpus-wide). The arithmetic in it checks out exactly —
`covers 8 of 17, leaving 9 -> you would be at -2` at 7 life — and DH F8's mechanism sentence is present and
correct (`None of these attackers has trample.`), as is `THIS IS NOT A SURVIVAL VERDICT`. **Fix is the
U16-shaped one**: hoist the standing paragraph out of the row and print it once per window, leaving the
per-row clause to carry only that row's numbers. Ten of this seat's 168 windows are over 20 KB against one
of 149 last wave, and the cover clause is where the growth is.

**MED-3 — U5 ADJUDICATED: the exemplar is neutral, the anchoring TRACE is gone, and the row-1 rate did not
move — which is the result, not a failure.** Every ask/priority window on this seat now ends
`e.g. "CHOICE: <row number> (<that row's short name>)" (a worked example of the FORMAT only - the number and
the name in it are placeholders, not a recommendation: ...)`. **Row-1 pick rate on the ask seam this wave:
60 of 136 multi-option windows = 44.1%, against wave 79's 62 of 139 = 44.6%** (split: casting menus 37/102 =
36.3%, land-drop menus 24/45 = 53.3%). Across all 168 records **zero** reasoning traces use menu position or
the exemplar as a tie-break (searched: `first in the menu`, `is first in`, `pick/choose the first`,
`example suggests/recommends/names`, `as recommended`) — wave 79's `162v152` seq 18, which quoted the live-row
example three times and settled on *"I will pick Dictate as it's first in the menu"*, has no analogue. Read
together: the fix removed the mechanism wave-79 HIGH-1 demonstrated, and the flat 44% confirms that review's
own caveat that the number was a base rate (row 1 is often genuinely correct in this deck). **Not checked**:
the corpus-wide rate on the other six decks.

**LOW-1 — the one `hold_reopened_new_lethal` on this seat is TRUE and the seat had nothing to do with it.**
`162v152` seq 23, t9, seam `cast`, face `[crack-back verdict: LETHAL]`, reason
`new_lethal_stack: a NEW crack-back threat over the same [crack-back verdict: LETHAL] face at the cast seam
- the objects behind it are not the ones this hold was taken over`. Sigarda had landed and the seat had gone
12 → 10 → 3 in that combat, so the re-open is real and correctly keyed on threat identity (DH F3). The
re-opened window (seq24) offered only braked engines; the seat answered `Cast nothing right now`. The other
5 `hold_event` records are all `reopen_rows_moved`. `hold_verdict_safer_ignored` and
`hold_reopened_new_threat` are both **0 on this seat and UNTESTED, not passed**.

**LOW-2 — all 8 cached ask-replays on this seat are DECLINES on unchanged End-step menus, `replay_run` ≤ 2,
and none is an activation or an object-creating cast.** `askreplay/1789216931-...-vs-ai_baka_deck152.jsonl`
(7 records, `why: cache_replay`, `choice: 3`, `replayed_from` seqs 9/19/27/34 — record seqs, DH F12) and the
rerun's single record (`replayed_from: 19`, seq 20, t13). DI's cap therefore had no case to bind on this
seat: `cached_replay_reasked` 0 and the absence of any `reask_reason` field are correct non-events.

**LOW-3 — the 6 `unlabelled_plan` records and the 4 `forced_close` events.** Deviations: `162v130`
seq 37/44/46/47 and `162v125` seq 28, `162v123` seq 18 — each a plan SENTENCE without the `PLAN:` label,
each with the plan still ahead of the action line (`action_before_plan` 0, `action_before_plan_rejected` 0).
`forced_close`: `162v146` window_seq 3 (seqs 25/26) and `162v130` window_seq 4 (seqs 32/33); gameends 2
events / 1 unrecorded each. Both moved decisions were answered normally in the next record. I did not read
the stderr `unclosed <think>` counts.

**Passes worth recording.**
- **Wave-79 MED-1 FIXED.** Teferi's Puzzle Box now prices off the live hand on every row:
  `{feeds: the opponent draws their whole hand: 4 cards right now (the amount is their hand size at the
  moment it fires, so it moves with their hand); ...}` (`162v152` seq 17 = 4, seq 32 = 3; `162v130` seq 23
  = 7). **The FORECAST half is still UNTESTED** — no Puzzle Box resolved in these six games either.
- **Wave-79 MED-2 FIXED, in both renders.** `[legendary: you already control Ob Nixilis, the Hate-Twisted
  at 5 loyalty - ...]` (`162v125` seq 13) and, corpus-wide, `[legendary: you already control Teferi, Who
  Slows the Sunset at 1/2/3/5 loyalty - ...]`. The clause is now sufficient; HIGH-1 is what remained.
- **The crack-back header folds non-combat damage and names it.** `162v130` seq 42:
  `... for up to 6 from combat as their board stands - you would be at 1 or lower - and that number is a
  FLOOR ... they can also aim ability damage at you that no block stops - Pyrite Spellbomb (2 per
  activation), Siege-Gang Commander (2 per activation) - ADD THOSE UP: the numbers named there come to 4
  more damage`. Deck130 killed with exactly those two Spellbombs at t22/t23. Also `162v146` seq 20/23,
  folding Hive of the Eye Tyrant's animation at its own rung and stating that its own mana is not counted.
- **`crackback_lethal_blocked_away` = 6, all in `162v146`, and the "best block" floor is right.**
  `with your best block: 3 damage still gets through, leaving you at 1.` / `6 ... leaving you at 1.` /
  `8 ... leaving you at 2.` The seat finished the game at **1**. The accompanying sentence — *"So passing
  this window does NOT hand them the game: you still get the blockers decision on their turn, and that
  assignment is what this figure is computed from"* — is the right framing and is not a survival claim.
- **The K = 0 brake, working in the one place it mattered.** `162v152` seqs 17/32/39 and `162v130` seqs
  39/40: every hold is on a menu whose only rows are Dictate / Puzzle Box / a second Howling Mine, and I
  verified in both games that taking one changes neither the blocker count nor the damage clock.
- **Land drops 45/45** (wave 79: 42/42), black taken first at every window that offered a choice of
  colours (`162v130` seq 26, 42, 48). **Mulligans**: one, `162v130` seq 2→3→4, shipped a black-less seven,
  kept at six, bottomed the most expensive spell per §MULLIGAN.
- **Cleanup discards followed the cost rule exactly** where the rule reaches: `162v130` seq 41 sent Peer
  into the Abyss (7), Puzzle Box (4) and a SECOND Howling Mine from a 10-card hand; seq 44 sent Forced
  Fruition (6) and Ob Nixilis (5) with a Fate Unraveler already on the battlefield. MED-1 is the third one.
- **No card behaved contrary to its rendered text in six games.** Re-checked against prompts and logs:
  Fog Bank's prevention against Thraben Doomsayer (`162v123` seq16) and against an indestructible 3/2
  (`162v146` seq24), Shield Sphere's `-0/-1` per block, Ob Nixilis's -2 firing every punisher on two forced
  draws (three games), Underworld Dreams stacking as two and three copies, Howling Mine's untapped
  condition, Master of the Feast's upkeep gift, Forced Fruition's seven-on-a-cast, the legend rule leaving
  both copies at their own printed loyalty with nothing merged.

## 3. Guide verdict: **EDIT**

`wave80/deck162/strategy.txt`, **19,971 bytes** (live guide 19,968; ceiling 20,000, headroom 29). Started
from the live `projects/mtg/bin/Res/ai/baka/deck162_strategy.txt`. No citations, counts, history, corpus,
wave or seq references are in the guide; no "YOUR PLAN" is quoted; there is no PLAN template in this guide
to break. Two rules gained bytes; three retirements paid for them.

**Which rule earned its bytes**: E1 (the legendary exclusion, moved to where the ladder reads) and E2/E3
(the two clauses that each reversed the GANG BLOCK stop). Both are paid for by a decision the corpus shows
going wrong, with the reasoning naming the clause it used.

| # | before -> after | paid by |
|---|---|---|
| **E1a** | rung 1b: `1b. UNDERWORLD DREAMS or OB NIXILIS otherwise. Any punisher in the list -> cast it THIS ask, ahead of / every other card` -> `1b. UNDERWORLD DREAMS or OB NIXILIS otherwise. A row tagged "[legendary: you already control" at / 3 loyalty or more is not a punisher row and not a play: read past it. Any other punisher in the list / -> cast it THIS ask, ahead of / every other card` | `162v125` **seq 13** — the exclusion now sits ABOVE the imperative it excepts, in the section the trace demonstrably reads, keyed on the rendered literal and on the number the render now prints. (+150 B) |
| **E1b** | §{reserve:}: `A row tagged "[legendary: you already control one ...]" is not a play - ONE exception: your Ob Nixilis / row while the Ob Nixilis on your battlefield shows loyalty 2 or less. Cast it, and on the LEGEND RULE ask / name the LOW-loyalty copy as the one you GIVE UP; you keep a fresh 5 and two more -2 activations.` -> `A "[legendary:" clause printing 2 loyalty or less is rung 1b's one exception: cast that Ob Nixilis, and / on the LEGEND RULE ask name the LOW-loyalty copy as the one you GIVE UP - you keep a fresh 5.` | `162v125` **seq 13/14** — the prohibition moved to E1a, so this paragraph keeps only the exception and the legend-rule answer, and now names the rung it excepts instead of repeating the ban under a heading about mana. (−90 B) |
| **E2** | §LETHAL CHECK: `EVERYTHING GOES once this clears: swing with everything, Fate Unraveler included.` -> same, + `An ATTACK TOTAL line / reading "That is NOT lethal" is this check NOT clearing: ATTACKING decides, every stop in it standing.` | `162v146` **seq 23** — the trace quoted this sentence as its licence to attack while the same screen read `That is NOT lethal: they survive at 6`. The condition is now a printed literal on the window, and the sentence names what governs when it fails. (+185 B) |
| **E3** | §ATTACKING: `"that would KILL you" is the REASON that body stays home: it blocks one of those attackers.` -> `"that would KILL you" is the REASON that body DOES NOT ATTACK: it stays home to block one of those / attackers, and it is never a reason TO attack.` | `162v146` **seq 23** — the trace read this sentence twice and ruled *"This refers to blocking, not attacking"*, then used the crack-back as a reason TO attack. The seam is now named in the imperative and the inversion is refuted. (+112 B) |
| **E4** | §LETHAL CHECK: `a Puzzle Box is their hand size off the "Opponent / hand size:" line.` -> `a Puzzle Box the count its own row prints.` | wave-79 MED-1 **shipped**: the Box row now prints `the opponent draws their whole hand: N cards right now` on every render I found (`162v152` seq 17/32, `162v130` seq 23). A hand-count derivation is retired where the render marks the number. (−60 B) |
| **E5** | §LAND DROP AND MANA: `It is its own decision and spends nothing: play the land AND cast the spell, same turn, / that order.` -> deleted (the imperative `PLAY THE LAND, every turn, no exceptions` and the `Never answer "Play no land right now"` stop both stay) | Retired derivation under the skill's two-corpus N/N threshold: the render carries the fact verbatim on **332 of 332** land-drop menus corpus-wide (`playing a land costs no mana and uses up no cast: it does not reduce what you can cast this turn`), and this seat is **45/45** this wave after **42/42** last wave. (−115 B) |
| **E6** | §{reserve:}: `... so it can never cost you a turn, and any row that moves re-opens it.` -> `... so it can never cost you a turn.` | Same threshold: every hold row in the corpus (**1,383 of 1,383**) prints `any change re-opens this window`. The refutation the render does NOT carry (`it can never cost you a turn`) is kept. (−36 B) |
| **E7** | §WALLS: `Shield Sphere and Fog Bank are your entire defence - no removal except Ob Nixilis's -2 - and a wall in / hand blocks nothing.` -> `Shield Sphere and Fog Bank are your entire defence, and a wall in hand blocks nothing.` | Byte recovery only; the deleted clause is stated in §CARD FACTS (`That -2 is removal AND a draw step you control`). No rule changed. (−37 B) |

**What is NOT edited, and why.** The K = 0 brake keeps its exact shape: `162v152` is the board it was
written for and I verified that releasing it there does not change the result. §SANGUINE BOND is untouched
— the `162v126` loss turned on forcing draws into a one-half-visible loop, which is one occurrence and
whose only alternative is conceding the deck's clock. MED-1 is left to the render: the seat followed a
correct-looking DEFAULT the engine printed, and a guide rule fighting a marker is the wrong layer while the
marker's own inputs are two lines away on the same screen. Wave-79's MED-3 (rung 0's `no other noncreature
row`) did not fire on any board this wave and stays untouched.

## 4. The rerun, and the hang state

`162v123` is the rerun on `ba5cdc374` (lanes DI + DJ). **The Doomsayer + Intruder Alarm state never arose.**
Intruder Alarm was drawn and appears 58 times in the opponent seat's log — always in HAND, never resolved:
they were short of `{U}` all game (`[cannot pay now: needs {1}{u}, your 4 untapped sources cannot pay it]`
at their seq 35), and the last prompt of the game lists their battlefield as `Scrubland #1-#3; Swamp;
Human (3/3); Underground Sea; Marsh Flats; Thraben Doomsayer` — **no Intruder Alarm**. Thraben Doomsayer
WAS on the battlefield and did make Humans, so the loop's first half stood and its second never did.
Consequently: **no `reask_reason` record appears anywhere in this corpus** (grep over all 42 seat logs
returns no file), `cached_replay_reasked` is 0 in the rerun's gameend, and the seat's single cached replay
(`askreplay/1789241587-ai_baka_deck162-...`, seq 20, `replayed_from: 19`, `replay_run: 1`, `choice: 3`) is a
DECLINE on an unchanged casting menu — not an activation, not object-creating, and one run short of DI's
cap. **Lane DI is UNTESTED on this game, not passed**, and 0 is the right number for a state that never
occurred. The rerun's own run completed and produced a gameend with no HUNG marker; I did not read the
harness's regime-gate logs to confirm DJ from the run log — that is the engine seat's check.

## 5. Not checked / limits

- Only this seat's six translogs, the six opponent translogs (outcomes and log narration), `results.tsv`,
  and the seven `askreplay/` files naming deck162. **I read no stderr file** (including the HUNG attempt-0
  stderr beyond confirming it exists in the corpus dir), ran no `corpus-stats.py` of my own, and read no
  engine source. No other deck's seats, no lane-{DE,DF,DG,DH,DI,DJ} adjudication, no `w80-stats.out`
  reconciliation.
- **HIGH-1 and HIGH-2 are single-window findings each.** The mechanism in both is quoted verbatim from the
  reasoning, which is stronger than a correlation, but neither is a repeated pattern this wave; wave 79's
  `162v125` seq 41 is HIGH-1's only prior instance and HIGH-2 has none. If either edit is wrong, it is wrong
  by over-fitting to one trace.
- **MED-1's cost is not established as a game.** I show the `{spare:}` clause states a false fact and that
  the seat obeyed it; I did not replay the game with the Swamp kept, and deck130's kill (Starstorm + two
  Pyrite Spellbombs) is one the seat's board never answers.
- **MED-3's row-1 rate is this seat only.** I did not compute the corpus-wide rate for either wave, and my
  "casting vs land-drop" split uses a crude classifier on the menu text.
- **UNTESTED on this seat, not passed**: `menu_single_outcome` / `single_outcome_rows_spared` (0 — no menu
  whose rows all resolved to one outcome arose), `own_loop_*` (0 — no `[own loop verdict:` line rendered),
  `crossphase_replayed` / `crossphase_board_unchanged` (0 each), `stop_reached_*` (0), `stack_drain_*` (0),
  `stack_death_verdict_lines_rendered` (0), `hold_verdict_safer_ignored` (0), `hold_reopened_new_threat`
  (0), `mana_only_windows_skipped` (0), the `{reserve:}` / STRANDS rules (the literals appear on 9 and 17
  windows but the seat never took a STRANDS row and never needed the reservation to survive a re-ask), the
  blockers DP's trample / menace / first-strike arms (no trampler and no menace attacked this seat), the
  forced-sacrifice section (no sacrifice row arose), `DRAW FORECAST` with a live Puzzle Box, and the
  `(blocking trigger:` / `(lifelink:` attack stops (the literals appear in zero attack tags on this seat for
  a second wave — wave 78's edits C and D remain UNTESTED in both directions).
- The `162v152` and `162v126` losses are called not-guide-addressable on the engine's own printed totals and
  the game logs, not on replayed games. For `162v126` I did not establish that no t10–t12 line beats the
  loop; I establish that the deck holds no enchantment removal and that the -2 that dug them into Sanguine
  Bond was also the only 6 damage available that turn.
- No `general-proposals.md` and no `skill-proposals.md`. HIGH-1 and HIGH-2 are both instances of rules the
  current skill edition already states (§4.1: an exception lives ABOVE the imperative it excepts; restate
  the stop wherever the imperative lives; every absolute carries its falsifier inline) — a proposal
  restating them would be noise. MED-1 and MED-2 are engine seams. No lategame specimen is written
  (owner ruling 2026-09-12).
