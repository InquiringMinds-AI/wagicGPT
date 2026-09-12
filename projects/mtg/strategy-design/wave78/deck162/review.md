# Wave 78 — deck 162 ("Draw and Die!", UB forced-draw punish) per-deck review

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260911-125420-final`, SINGLE binary
`wagic-47ef378c8-w78step1` (source `7c0f15835`), `--thinking on`. Six deck162 seat translogs plus the six
opponent seats for outcomes.

**124 model decisions on this seat, 1 heuristic fallback, 0 `reply_truncated`, 0 `action_before_plan`,
native reasoning on 124/124** (median 6,272 chars, max 15,760). `protocol_deviation`: compliant 118 /
`unlabelled_plan` 6 (4.8%; wave 77: 7.5%), `plan_absent` 0, `answer_label_absent` 0 — see MED-1, that
last zero is a measurement artefact. `transport` `http=200, empty=0` on all 124; `deadline_pct` max 34.1.
Latency p50 **126.2 s**, p95 216.0, max 307.8. Prompt bytes: max **19,112** (126 seq20), median 9,124,
**0 of 124 windows over 20 KB** — against wave 77's max 30,537 and 51 of 187. S13 paid on this seat.
Gameend sums over the six games: `hold_windows_skipped` 26 (all cast), `own_turn_windows_skipped` 146,
`forced_close_events` 2 / `_unrecorded` 1, `async_drops` 0, `crossphase_identical_reputs` 0,
`stop_reached_windows_skipped` 0, `stack_drain_windows_asked` 0, `own_loop_windows_asked` 0,
`plan_names_uncastable_zone_card` 0 (wave 77: 1 false positive), `plan_names_stranded_card` 0,
`phase2_answer_recovered`/`_missing` 0, `declined_face_latches` 0, `repeat_annotated_takes` 0,
`identical_option_asks_resolved` 0, `mana_only_windows_skipped` 0, `main_phase_windows_skipped` 0.

**Record 2–4** (wave 77: 3–3; different pairings, and deck162 drew the two hardest decks twice over).
Three of the four losses are not guide-addressable: 123 is the same Thraben Doomsayer token burst that
beat the deck in wave 77 (21 tokens in one activation chain, 22 attackers at t10), 126 is a
Sanguine Bond + Exquisite Blood loop closing on a 1/1 lifelink token while the seat was AHEAD on the race,
152 is a race the seat was 8 points behind from t11. **The fourth (146) is guide-attributable and the
mechanism is in the model's own words: rung 0's "and no noncreature row" refused Ob Nixilis three times in
one trace, and Ob Nixilis's -2 was the only line that survived that turn.** One further guide-attributable
misplay, 126 seq14, handed the opponent 9 life off an §ATTACKING rule that is written as a price rather
than as a stop.

## 1. Game by game

| Opp | Result | Deciding decisions |
|---|---|---|
| 130 | **WIN** t14, 12–0 | 15 decisions, no deviation, no fallback. Underworld Dreams t5 and a second t9 (rung 1b, both first in their window), Ob Nixilis t11 (seq11), **-2 on Dwarven Blastminer #1 in the SAME main phase (seq12)** and again at t13 (seq16). With P = 3 each -2 is 6 damage plus the body; seq16's plan states it ("deal 6 damage. Opponent will draw 3 next turn for 9 damage, losing the game") and that is what happened. §LETHAL CHECK's burst rung, working. seq11→seq12 is also the load-bearing fact for guide edit A: **the -2 window is offered on the turn Ob Nixilis lands.** |
| 125 | **WIN** t23, 20–0 | 38 decisions, the longest game on this seat. **The K-of-0 brake held nine consecutive windows from seq16 (t12) to seq27 (t16)** with Dictate and Puzzle Box on the menu and up to 9 idle sources; seq17's reply names the rule ("Dictate of Kruphix is braked at K=0 and would feed them cards without punishment"). Fate Unraveler seq28 (t16) released it; Dictate seq34, Underworld Dreams seq35, Mine seq36, Puzzle Box seq37 in two turns, and the opponent died on their t22 draw step. A success verdict for §THE BRAKE and its RELEASE clause, the same one as waves 76 and 77. |
| 146 | **LOSS** t11, −2–20 | **The one guide-attributable loss — see guide edit A and HIGH-1.** t10 seq15 at 8 life, five sources, crack-back header reading `the total to subtract from your life is 10 and you would be at -2; that would KILL you`. Row 3 was `Cast Ob Nixilis, the Hate-Twisted {3}{b}{b}`, payable, with three creatures of theirs on the -2 list. Row 4 was Master of the Feast, carrying `[NAMED BY THEIR Silverquill Silencer: casting this costs you 3 life ... you would be at 5]`. The trace asks **three separate times** whether to cast Ob Nixilis and each time answers itself with rung 0: *"Wait, should I also cast Ob Nixilis? It's not a creature. The rule says 'no noncreature row'. So just MotF."* Master cost 3 life, handed them a card, and covered 3 of the 10; Ob Nixilis + a same-turn -2 on Barrowin (3 power) leaves 7 incoming at 8 life — **survives at 1, with K going 0 -> 1 and two cards forced into their draw**. seq19's blockers window is then arithmetically decided before it is asked (`no block saves you`, `-2 AT BEST`) and the seat's block IS one of the −2 assignments. Earlier: seq7 (t6) paid the same 3-life named price for Master #1 under rung 4b's first case; the two casts together are 6 of the 8 life the seat died by. |
| 126 | **LOSS** t13, 0–32 | The seat was **winning this race** — opponent 13 at t13 against Fate Unraveler + four Howling Mines — and lost to a two-card loop. **seq14 (t10 Attackers) is the guide violation and it cost 9 life:** A1 Fate Unraveler's tag read `(3 certain if all block, and up to 9 if they also take every "may") - Perimeter Captain (0/4) (neither dies) (blocking trigger: they may gain 2); Overgrown Battlement (0/4) (neither dies) (blocking trigger: they may gain 2); Pride Guardian (0/3) (you kill it, your attacker lives) (blocking trigger: they gain 3 and may gain 2 more)`, and the window also carried `LOOP HALF PENDING: Exquisite Blood is on THEIR battlefield and the other half ... Sanguine Bond, is in their hand ... every life payment you are pricing now becomes fatal rather than expensive`. The trace checks the three §ATTACKING rules that are WRITTEN AS STOPS (low life, one fatal name, GANG BLOCK), finds none of them fires, and concludes *"So Fate Unraveler can attack safely"*; it then prices the loop and dismisses it — *"Sanguine Bond isn't out, so I don't lose life. It's fine."* Three blocks, 9 life gained (log: `Opponent gained 3 life` ×3 plus the Exquisite Blood echoes). **See guide edit C.** Everything after is clean: seq20/21 refused both creature rows under §SANGUINE BOND ("both creature options are fatal due to their life loop"), seq22 sent only Master, whose A-line printed `[no creature they control can block this attacker]` — exactly the converter rule — and seq23's BLOCKS: none was as fatal as every other answer (the render says so: a 1/1 lifelink with both halves out, `NO best-case life figure is given`). |
| 152 | **LOSS** t13, −6–17 | Race loss, not guide-addressable. The seat curved Shield Sphere t2, Mine t4, Fog Bank t6, Fate Unraveler t8, Underworld Dreams t10, Shield Sphere + Caress t10, Forced Fruition t12, and still faced 22 power by t12. **seq19 (t11 Blockers) is the best decision on this seat all corpus and the engine's own header under-rates it — see HIGH-2**: three blocks, actual damage taken 4, life 18 -> 14, against a header reading `you would be at 12 AT BEST (no assignment of your blockers does better)`. seq22 (t13) is 22 damage at 14 life with one Fog Bank left; `-8` either way. seq21's Forced Fruition is guide-legal (K = 1 off Underworld Dreams) and was the only non-pass row in hand. |
| 123 | **LOSS** t10, −4–16 | Opponent combo, not guide-addressable and a repeat of wave 77's. Their Thraben Doomsayer chain made 17 tokens in one named repeat at t9 and 3 more at combat; they then killed the seat's only blocker with Tribute to Hunger at t10 and attacked with 22 bodies. deck162 held no sweeper and no removal (Ob Nixilis never drawn). Its 11 decisions are all on rung: Mine t3, Underworld Dreams t7 and t9, Fog Bank t9 — punishers ahead of the wall, per rung 1b. The seat's punishers were in fact working (4 damage on their t10 draw). Two `forced_close` records here, LOW-2. |

## 2. Engine / interface / card items

**HIGH-1 — a creature row's `{crack-back cover:}` subtracts from the CREATURE-ONLY FLOOR while the same
prompt's header has already added the animation and printed the real total. The model took the row's
figure and called it safe.**
`1789149279-ai_baka_deck162-0x55999eaa1c40-vs-ai_baka_deck146.jsonl` **seq 15** (t10, life 8). The header
in that prompt reads, in full: `CRACK-BACK NEXT TURN: 3 of their creatures will be able to attack ... for
up to 7 from combat as their board stands - you would be at 1 or lower ... noncreature permanents of
theirs that can animate and attack are not in that count - Hive of the Eye Tyrant ... ADD THOSE UP: the
numbers named there come to 3 more damage, so the total to subtract from your life is 10 and you would be
at -2; that would KILL you, which is every source on this screen counted once`. Row 4's own clause,
120 lines later in the same prompt: `{crack-back cover: the CRACK-BACK NEXT TURN line above is 7 from 3
of their creatures ... Counting only the checked bodies you cover 3 of 7, leaving 4 -> you would be at 4.
THIS IS NOT A SURVIVAL VERDICT ...}`. The hedge is honest and the arithmetic is internally consistent,
but the clause recomputes from the floor the header has already superseded, so one window prints `4` and
`-2` for the same combat. The cost is quotable: *"So it covers 3 damage. I go to 5. **Safe enough** to
draw and set up."* Truth at seq19: `you would be at -2 AT BEST ... no block saves you`, and the game
ended at −2. **Fix**: compute the cover against the header's ADD-THOSE-UP total whenever the header
printed one, and carry the header's KILL verdict into the row.

**HIGH-2 — the `INCOMING THIS COMBAT` header's "at least N lands" / "AT BEST" figure uses a different
trample rule from the blocker rows underneath it, and was FALSE by 2 life.**
`1789149292-ai_baka_deck162-0x56095b55dbb0-vs-ai_baka_deck152.jsonl` **seq 19** (t11, life 18). Header:
`INCOMING THIS COMBAT: 3 attackers, 16 unblocked damage - you would be at 2 ... at least 6 of that lands
whatever you block (trample/menace counted as unblocked): you would be at 12 AT BEST (no assignment of
your blockers does better)`. Row B1 in the same window: `Fog Bank (0/2) ... may block ... A2 (neither dies
(no combat damage is dealt either way - prevented), **4 tramples to your face**)`. The seat blocked
A1/A2/A3, took exactly 4 (`Opponent's Sigarda, Champion of Light dealt 4 damage to you (now 14)`) and
finished at **14**, two better than the "no assignment does better" claim. The header appears to charge
the whole 6 of a trampler against any block (power minus blocker POWER 0), while the row charges power
minus blocker TOUGHNESS. A seat that trusts the header — the render tells it to, `Trust it over your own
arithmetic; never re-derive it` — is being told a survivable combat is not. Same family as HIGH-1: the
aggregate line and the per-row line are computed by different code from the same board.

**MED-1 — `protocol_deviation` is single-valued, so a reply missing BOTH labels never reaches the
`answer_label_absent` measure the owner's S1 ruling is being taken on.**
Same file, **seq 16**. Raw reply, verbatim: `"\n\nCast nothing right now.\n3 (Cast nothing right now)"`.
`off_protocol_bytes` 49 = 23 (`Cast nothing right now.`) + 26 (`3 (Cast nothing right now)`), i.e. the
parser found neither label and charged both lines. The record is classed `unlabelled_plan`; the gameend
reads `answer_label_absent: 0` and `answer_label_absent_heuristic_played: 0`, so this window — an
in-range, byte-exact, label-less answer line that the heuristic then played — is invisible in the corpus
census of exactly that shape (6, all deck125). **The corpus's answer-label-absent heuristic plays are 7,
not 6, when the plan deviation does not mask them.** Fix: make the class a set, or count
`answer_label_absent` off the answer line independently of the plan line. (I am not proposing parser
tolerance; the parser is unchanged by ruling.)

**The mandated fallback, in full.** Window: 146 seq16, t10 Main phase 1, life 5, `options: 3`, latency
160.7 s, `deadline_pct` 17.8, `coded_answers: 0`, `fallback: unparsed_reply`, `chosen_text:
<refused: unparsed_reply>`. The three legal rows were `1. Cast Howling Mine {2} [second copy: ...]`,
`2. Hold priority - ...`, `3. Cast nothing right now (combat comes next this turn) {closes ONLY this
window ...}`. The model asked for row 3. **What the heuristic played** (record seq 17,
`kind: recovery`): `executed_by: heuristic, executed_seam: cast, executed_choice: 0, executed_text:
"cast nothing", recovers_seq: 16`. **Did it matter: no.** The heuristic's "cast nothing" is the same
game action as row 3, the second Mine was braked at K = 0 anyway (the guide holds a `[second copy:` Mine),
and the seat's life, board and turn are identical either way; the loss at t11 was already arithmetically
fixed at seq15 (HIGH-1). The cost is 160.7 s of decode and one lost PLAN line, not a play. **Which pin**:
this is the deck162 `unlabelled_plan` fallback named in the brief, and the only unparsed reply on this
seat in the corpus.

**MED-2 — the `{effect:}` gloss still cuts mid-clause with a bare `...` on two of the three over-length
glosses in this seat; the `(...more)` clause-boundary form fired on one.** PASS case:
`152` seq19, `{effect ... "When Briarbridge Tracker enters, investigate. (...more) -- As long as you
control a token, Briarbridge Tracker gets +2/+0."}` — cut after a whole sentence, marker present, the
following clause preserved. FAIL cases: `146` seq13 `"... -- Whenever Barrowin of Clan Undurr attacks,
return up to one creature card with mana value 3 or less from your graveyard..."` (cut before
"to the battlefield tapped", no marker) and `152` seq11 Sigarda `"... Coven - Whenever Sigarda attacks,
if you control three or more creatures with different powers, look at the top five..."` (cut mid-clause,
no marker). Neither is false, and both are better than wave 77's Howling Mine cut (which is GONE — the
Mine renders in full in all 6 games). Read: the fix covers the parenthetical-reminder case and not the
trailing-clause-over-cap case.

**MED-3 — `Opponent life trend:` prints the GAINED/LOST split only when BOTH directions have events, not
only "when no event source" exists.** 116 trend lines on this seat: 68 `unchanged at N`, **9 split**,
**39 net-only**. The 39 net-only lines are not sourceless — e.g. `1789149287-...-vs-ai_baka_deck123.jsonl`
seq 13 prints `turn 5: 20, turn 7: 20, turn 9: 18, now 18 (-2 since turn 5)` over a span whose only life
events are two Underworld Dreams pings, i.e. losses of 2 and gains of 0. The 9 split lines are true where
I could reconstruct them: `126` seq23 reads `over those turns life-gain EVENTS put +9 on them and
life-loss EVENTS took -14 off`, and +9 is exactly the three Perimeter Captain / Pride Guardian gains
logged at t10. Not a false statement — a reporting note for the S12 adjudication, since a reader told
"net-only means no source" will misread 39 lines.

**LOW-1 — latency rose 37% while prompts shrank 40%.** p50 126.2 s (wave 77: 92.4 s), p95 216.0, max
307.8, on prompts whose max fell from 30,537 B to 19,112 B and whose over-20-KB count fell from 51/187 to
0/124. Whatever the S13 hoist bought, it did not buy wall-clock here; 21 concurrent games is the more
likely cause. `deadline_pct` max 34.1 — no window came near the 900 s ceiling.

**LOW-2 — the 123 game's two `forced_close` records, for the S11 reconciliation.** Both on window_seq 8,
turn 7, arm `cast`: seq10 `{outcome: armed, unrecorded_so_far: 0, defer_ticks: 0, park_armed: 0}`, seq11
`{outcome: dropped_decision_moved, unrecorded_so_far: 1}`. Gameend: `forced_close_events` 2,
`forced_close_unrecorded` 1, every arm-bound counter 0. The decision that moved (seq12, Underworld Dreams)
was answered normally and nothing was lost. I did not read the stderr `unclosed <think>` count.

**Passes worth recording.**
- **S7 is FIXED, and it closes my own wave-77 HIGH-1.** All **103** `[hold check:` brackets on this seat
  carry a `hold_check_ref_seq` that resolves to a real record, and **103/103 name the previous asked
  window at the same seam** under a land/cast/activated classifier — 0 missing, 0 mis-seamed (wave 77:
  12 values named no record at all and no offset reconciled the rest). `hold_check_ref_window` rides
  beside it as the ordinal.
- **The hold-check CLAIMS: 0 cases where the engine claimed MORE new rows than a group-stripped diff
  finds**, 16 where it claimed fewer (all folds it declares: annotation-only changes, and pass-row
  re-wordings such as `Cast nothing right now` vs `... (combat comes next this turn)`). Same comparator
  caveat as last wave — "no counterexample", not a proof.
- **S13 on this seat**: max prompt 19,112 B, 0 windows over 20 KB, median 9,124. No hoisted shared card
  text appeared inside a row in the 4 shared-source menus I opened.
- **S14 loyalty**: the 3 `[cost: Counters]` rows all print the counter cost in its own group with the
  figure — `{counter cost: spends 2 loyalty counters - Ob Nixilis, the Hate-Twisted has 5 now, leaving 3}`
  (130 seq12) and `... has 3 now, leaving 1` (seq16). Both true against the loyalty the card then had.
- **`{reserve:}` + the hold row**: 146 seq8 offered `Cast Dictate of Kruphix ... VERDICT: taking this row
  STRANDS Liliana's Caress this turn`. The seat refused it, answered the HOLD row per §THE {reserve:}
  CLAUSE's last paragraph, and cast the stranded Caress in main phase 1 at seq10. The one STRANDS row in
  the corpus for this seat, answered correctly.
- No card behaved contrary to its rendered text in six games. Card facts re-checked against the prompts:
  Fate Unraveler 3/4 `{3}{B}`, Fog Bank's prevention (and its 4-trample-through line, HIGH-2), Shield
  Sphere's `+0/-1` per block (`now 0/5` at 152 t11), Master of the Feast 5/5 flier with the upkeep gift,
  Ob Nixilis 5 loyalty and the -2 in the same main phase, Howling Mine's untapped condition, Teferi's
  Puzzle Box, Underworld Dreams stacking as two copies. 37 of 37 land-drop asks answered with a land.

## 3. Guide verdict: **EDIT**

`wave78/deck162/strategy.txt` — **19,968 bytes, from 19,947 (net +21)**, under the 20,000 ceiling. Three
edits in four places plus two byte returns. **The guide grows by 21 bytes; the bytes are earned by
edit A**, which is the only rule in the corpus that turned a survivable turn into a lost one.

| # | before → after | paid for by |
|---|---|---|
| A | §CAST ORDER rung 0: `... line reads "that would KILL you": take every creature row you can afford in ONE window, cheapest per body first (Shield Sphere {0}, Fog Bank, FATE UNRAVELER, Master of the Feast), and no noncreature row.` → `... line reads "that would KILL you": OB NIXILIS FIRST while his row is payable and a creature of theirs is on his -2 list - that -2 has a window the turn he lands and DESTROYS one of the bodies the count was computed from. THEN every creature row the window still pays for, cheapest per body first (Shield Sphere {0}, Fog Bank, FATE UNRAVELER, Master of the Feast), and no other noncreature row.` | **146 seq15**, above and HIGH-1. The trace refuses Ob Nixilis three times by quoting `no noncreature row`, takes the body the rung names, and dies at −2 on a board where Ob Nixilis + his same-turn -2 removes 3 of the 10 and survives at 1. The rung's blind spot is structural, not incidental: rung 0 is written entirely in the vocabulary of BLOCKING (bodies, cheapest per body), and the deck's only removal is a noncreature permanent — so on the exact board the rung exists for, it strikes the best line off the menu by name. **The mechanism is verified in this corpus, not assumed: 130 seq11 casts Ob Nixilis and 130 seq12 offers the `-2` in the same main phase.** Targeting is deliberately NOT restated here — §CARD FACTS already owns it, and wave 77's Master defect was one rule in three wordings. |
| B | §THE BRAKE, RELEASE: `... AND your life 15 or less -> no release at all. Take a wall row.` → `... -> no release at all. Take a wall or a punisher row.` | The same window. This clause was live at 146 seq15 (life 8, M = 3 over N = 0) and, read alone, points at the wall as the only alternative to the braked engine — a second sentence that would fire backwards against edit A on the identical board. A rule gaining an exception gives it to every line that summarises it. |
| C | §ATTACKING: `LIFE THEY GAIN IS A PRICE: "(neither dies (blocking trigger: they may gain 2))" and "(you kill it, your attacker lives (lifelink: they gain 1))" are not all-free tags and the send rungs do not fire on them. With "LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs" printed those numbers double and point at you: send ONLY attackers whose A-line prints neither clause, else ATTACK: none.` → `LIFE THEY GAIN IS A STOP, like a fatal name: "(blocking trigger:" or "(lifelink:" anywhere in a tag -> ATTACK: none for that creature, however friendly every fight result beside it reads. THEY pick the blocker, so the tag's own summed figure ("up to N if they also take every "may"") is the price, and this deck kills at 1 damage a card they draw: life they gain is turns handed back. "LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs" points that price at your life as well, and a "LOOP HALF PENDING:" line says the pair closes on their next cast, with the life you hand them now already on their side of it.` | **126 seq14.** The old paragraph states the rule as a property of tags ("are not all-free tags") and puts its only `ATTACK: none` behind a header that was NOT printed in that window. The trace checks the three neighbouring rules that ARE written as stops, finds none fires, and sends. The rewrite states the same rule in the operative form the others use — a quoted literal in the tag, then `-> ATTACK: none` — and names `LOOP HALF PENDING:` directly, since that is the line the render actually printed while `LIFE-TO-DAMAGE CONVERTER` was absent. No new restriction: the old text already said the send rungs do not fire on these tags. |
| D | §SANGUINE BOND / EXQUISITE BLOOD: delete `ONE half out only makes life they gain worth more, so the LIFE-TO-DAMAGE CONVERTER rule still governs attacking.` | Same window. This sentence is what should have covered 126 seq14 and could not: it forwards to a rule keyed on a header the one-half board does not print. Edit C now carries the one-half case in the section that answers an attack question, so the forwarding sentence is a second wording of a rule that lives elsewhere — the shape that beat rung 4b last wave. |

**Bytes returned** (both duplicates of rules stated elsewhere, and unexercised in two waves):
(i) §THE BRAKE, K-of-1: `"EVERY engine" never reaches a row whose VERDICT begins "taking this row
STRANDS" - that row is refused at every K, on every board, and with the game already won; "K is 1, so
cast every engine" is not an answer to a strand.` — the §CAST ORDER preamble already says a STRANDS row
is NEVER YOUR ANSWER and that no rung reaches it, and §LAST CHECK says it a third time. The corpus's one
STRANDS row (146 seq8) was at K = 0 and was refused.
(ii) §THE BRAKE: `First Mine AND a punisher on one unaffordable ask: take the punisher.` — rung 1b
already casts a punisher ahead of every other card at every life total; no window in two waves put a
first Mine and a punisher on one unaffordable ask.

**Not changed, deliberately.** §THE BRAKE's K-of-0 branch — **a success verdict**: 162v125 seq16–seq27,
nine consecutive windows held with Dictate and Puzzle Box listed, the model naming the rule, and the
rebuild after release won the game. §BLOCKING in full — **a success verdict**: five blocker windows, every
answer at least as good as the engine's own named best case and one (152 seq19) two life BETTER than it.
§LAND DROP — 37 of 37 land-drop asks answered with a land. §CARD FACTS Ob Nixilis targeting and the Peer modes. Rung 4b's second case
(wave 77's K-of-0 Master permission) — **still untested in both directions**: both Master casts this wave
(146 seq7, seq15) were first-case casts, their line reading 1 or more creatures.

## 4. Not checked / limits

- Only this seat's six translogs and the six opponent translogs for outcomes and game log. **No stderr
  files, no `askreplay/` sidecar, no corpus counters beyond my six gameends, no other deck's seats, no
  engine source, no lane adjudication.** I did not reconcile the two `forced_close` records against the
  stderr `unclosed <think>` count (LOW-2).
- **HIGH-1 and HIGH-2 are inferences from the rendered text and the game log, not from the code.** For
  HIGH-2 I show the header's figure is wrong by 2 against what the seat actually took and that the row
  underneath prints a different trample number for the same block; the "power minus blocker power vs
  power minus blocker toughness" reading is my hypothesis for the mechanism and I did not verify it.
- **Edit A's counterfactual is arithmetic on the rendered board, not a replayed game.** I claim Ob Nixilis
  + a -2 on Barrowin leaves 7 incoming at 8 life because the header itemises 3 + 3 + 3 + 1 and the -2 row
  was offered against all three creatures; I have not replayed the turn, and their untapped source could
  have paid for something I cannot see. The same-turn -2 window is verified (130 seq11/12), the target
  legality is verified (the -2 rows at 146 seq15 name their creatures), the survival is computed.
- **Edit C tightens nothing that the old text permitted**, but it does make a STOP out of a sentence the
  pilot has so far read as advice. If a future corpus shows this seat declining an attack that was free,
  that clause is where to look — the literal `(lifelink:` fires on a tag where the lifelinker is a body
  they would never block with.
- The 123, 152 and 126 losses are called not-guide-addressable on the engine's own printed totals and the
  game logs, not on replayed games. The 126 game in particular was WINNING on the race count at the
  moment it died; I did not attempt to establish whether a different t10–t12 line beats the loop.
- **UNTESTED, not passed, in this seat**: S3 (`stop_reached_windows_skipped` 0), S4 (`stack_drain` 0),
  S6 (`own_loop_windows_asked` 0), S5 (no counterspell in the deck, 0 `{your <counter> already targets`
  clauses), S10 (0 `defer` records), S2 (0 repeat rows), the `[X pricing]` keep-X lane (0 X windows),
  `[legendary:` Ob Nixilis second copy (0 rows), `{removes:}` ghostform (0), MDFC land menus (0),
  `async_drops` (0), the S13 shared-source hoist at scale (this seat's menus were small),
  `declined_face_latches` (0, 6th wave).
