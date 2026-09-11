# Wave 77 — deck 162 ("Draw and Die!", UB forced-draw punish) per-deck review

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260911-010157-final`, SINGLE binary
`wagic-42c41fc66-w77step1` (source `8e394a077`), `--thinking on`. Six deck162 seat translogs + the six
opponent seats for outcomes. **162v126 is the FIRST-RUN game** (epoch 1789106561, the one game that
finished before the Spark pilot wedge); the other five are reruns from the 1789114xxx pools.

**187 model decisions, 0 fallbacks on this seat, 0 `reply_truncated`, 0 `action_before_plan`, native
reasoning on 187/187.** `plan_line_missing` 14 (7.5%) = `protocol_deviation_replies` 14: 13
`unlabelled_plan` + 1 `plan_absent` (125 seq92, `CHOICE: 2 (Cast Forced Fruition)` alone,
`off_protocol_bytes` 0). **The wave-75/76 identity recurs 13/13** — `off_protocol_bytes` equals the byte
length of the unlabelled sentence exactly (16/18/32/34/44/56/56/58/68/72/88/93/104).
`plan_names_uncastable_zone_card` 1 (162v125 seq36, a FALSE positive — see HIGH-2),
`plan_names_stranded_card` 0, `async_drops` 1 (125), `crossphase_identical_reputs` 7 (125),
`own_loop_windows_asked` 0, `forced_close_unrecorded` 0, `phase2_answer_recovered` 2 / `_missing` 0,
`hold_windows_skipped` 75 (125 alone; 49 cast / 26 priority), `main_phase_windows_skipped` 11 (125).
Prompt bytes: max **30,537** (125 seq122), median 14,486, **51 of 187 windows over 20 KB** (wave 76: max
20,195, one window over 20 KB). Latency p50 92.4 s, p95 207.9 s, max 270.2 s.

**Record 3–3** (wave 76 also 3–3, different pairing). No fallback, no re-ask and no forced close touched
this seat. **Two of the three losses are arithmetically unavoidable at the deciding combat** (146 and 152
both had "no assignment of your blockers survives this" printed and true); **the third (123) is an
opponent two-card combo the deck has no answer to.** The guide-attributable material this wave is not in
the losses: it is **three K-of-0 Master of the Feast casts the guide's own LAST CHECK calls WRONG**, and
one forced-sacrifice row where the model computed the better line and the guide's fixed ORDER overrode it.

## 1. Game by game

| Opp | Result | Deciding decisions |
|---|---|---|
| 126 | **WIN** t12, 14–0 (first-run) | Clean curve: Shield Sphere t2, Mine t4, Master t6, Underworld Dreams t8, Ob Nixilis t10 with `-2` on **Overgrown Battlement** (seq14 — §CARD FACTS "aim it FIRST at a creature whose text makes more creatures"; Battlement ramps, and the two cards it drew them fired both punishers). **seq16 (t11 Upkeep) is the wave-75 edit-C branch finally OPENING** — Tribute to Hunger with Sanguine Bond live and Exquisite Blood in hand (`LOOP HALF PENDING` printed and correct). See guide edit B. **seq18 (t12) is R3's test and it PASSES**: the `{their converter:}` clause on `Cast Fate Unraveler` now reads `this body has toughness 4 and they control 1 life-to-damage converter (Sanguine Bond) - any effect of theirs that gains them life equal to its toughness (a sacrifice-for-toughness edict, a drain of that size) takes 4 off YOU: life 14 -> 10`, and the reasoning prices it as a life cost on an event, verbatim: *"This is a warning, not a hard stop unless they sacrifice it or it dies to damage"*, *"This row says 'takes 4 off YOU', not 'fatal'. So it's fine."* The wave-76 loss read the same clause as a MANA statement twice. seq21 `-2` on Perimeter Captain for the kill. |
| 125 | **WIN** t35, 10–0 | 108 records, the longest game in the corpus. Opponent (UW control, 2 Staff of Nin, Supreme Verdict, Lightmine Field) killed every punisher. **The K-of-0 brake held 20 consecutive windows from seq37 (t22) to seq79 (t28)** with 13 idle mana sources and Dictate/2nd Mine/Forced Fruition on the menu every time — **a success verdict, the same one as wave 76 and the rule that won this game**: the seat rebuilt at t30 (Ob Nixilis seq89, then Dictate seq90, Mine + Forced Fruition seq91/92) and killed them on the t34 draw step. seq35/36 Peer into the Abyss answered `CHOICE: 1 (the opponent)` per §CARD FACTS. **seq80 (t28) is a guide violation** — Master of the Feast at K = 0 with their line reading `of which 0 are creatures` — see guide edit A. |
| 146 | **LOSS** t11, 0–11 | Not guide-addressable. Race against Triumphant Adventurers + Nadaar + dungeon tokens. **seq13 (t11 Blockers) at 6 life: `INCOMING THIS COMBAT: 5 attackers, 12 unblocked damage ... best case with every blocker assigned: you would be at 0; no block saves you`.** Two 0-power walls against 5 attackers stop at most 6 of 12; the seat's blocks (Shield Sphere on Nadaar 4, Fog Bank on an Adventurer 2) are exactly the engine's own least-damage assignment. seq10 (t9) likewise took the engine's named best case (6 life). The earlier windows are clean: t8 (seq8/9, 3 mana, 11 life, M = 4 over N = 0 with a killing crack-back) took Shield Sphere then Fog Bank off rung 0 — **Fate Unraveler was NOT castable there** (`{3}{b}` against 3 sources), and it landed seq12 the first turn it was affordable. Kaya exiled it t11. |
| 152 | **LOSS** t15, −4–17 | Race loss, and the wave-76 guide item HELD. **seq24 (t14 Attackers) is R6c's test and it PASSES**: at 2 life the prompt printed `{crack-back cover, STAY HOME: keeping all 4 of them back covers 16 of that 22, leaving 6. THIS IS NOT A SURVIVAL VERDICT ...}` — arithmetic verified true against their eight bodies (6+5+3+2 = 16 of 22) — and the seat answered `ATTACK: none` with *"hold all creatures to block next turn's lethal crack-back"*. Wave 76's 152 seq14 is the window that attacked into the same shape off the rung-0 leak; **the wave-76 edit C closed it.** **seq25 (t15) is the loss and is unavoidable**: 8 attackers, 23 damage, 2 life, `best case with every blocker assigned: you would be at -4`, and the seat's four blocks ARE the engine's named least-damage assignment, body for body. The seat was behind from t10 (5 life at seq16). |
| 123 | **LOSS** t12, −3–11 | Opponent combo, not guide-addressable. Intruder Alarm + Thraben Doomsayer with Lightning Greaves went off on **their own t10 upkeep** — 10 tokens at upkeep, 14 more in main 2, 25 attackers by t11 — with no cast of the seat's triggering it. deck162 has no removal but Ob Nixilis's `-2` and drew none. **seq12 (t9 Attackers) sending Fate Unraveler is guide-legal and was the only pressure on Doomsayer**: the tag read `Thraben Doomsayer (2/2) (you kill it, your attacker lives)`, the §ATTACKING send rung fires on exactly that, and Intruder Alarm's own untap-on-ETB means attacking does not strand the body. They declined the block and Tragic Slipped it to 2/3. seq15 answered `ATTACK: none` off a gang-block clause. seq16 took the engine's least-damage blocks with `no block saves you` printed. |
| 130 | **WIN** t11, 18–0 | Mine t3, Master t5, Shield Sphere + Fate Unraveler t7, attacks t7 and t9 off `no creature they control can block this attacker` / `They have 0 untapped creatures able to block` (no tag = the §ATTACKING send rung). **seq7 (t5) is the same K-of-0 Master violation as 125 seq8/seq80** — and here the 5/5 flier was the clock that won the game. |

### Wave-76 items: what recurs, and the wave-77 lanes

- **R3 (CS) — PASS, and it is the strongest result in this seat.** The converter clause now names the
  event and gives a figure; 162v126 seq18's reasoning prices it as a sacrifice cost, not a mana cost, and
  distinguishes the metered case from the `ANY nonzero payment here is fatal` case by quoting both. The
  wave-76 HIGH is closed. 4 clause instances over 3 windows (seq13, 18 ×2, 19), all arithmetically true
  (toughness 2 → `life 20 -> 18`, `14 -> 12`; toughness 4 → `14 -> 10`).
- **R6c (CS) — PASS.** 162v152 seq24, above. The stay-home figure is present, correct, and was used.
- **R4/R7 (CS) — PASS in eight blocker windows.** Every `INCOMING THIS COMBAT:` header carried the
  legality-filtered count (`your creatures can legally block at most 2 of those 4 attackers at once`) and
  a named best-case assignment; **all eight of the seat's answers match the engine's own assignment**, and
  the two "no block saves you" claims (146 seq13, 152 seq25) are arithmetically true.
- **R2/CR (hold check) — PASS, 0 false NEW.** 146 same-seam `[hold check:]` comparisons across the six
  games: **0 cases where the engine claimed MORE new rows than a group-stripped diff finds**, 30 where it
  claimed fewer (it folds pass-row re-wordings such as `Cast nothing right now` vs `Cast nothing right now
  (combat comes next this turn)`). The land seam is separately keyed and the cast/land brackets no longer
  interleave. Caveat as last wave: coarse comparator, "no counterexample in 146", not a proof.
- **Wave-76 edit A (STRANDS as a claim about the ANSWER) — UNTESTED.** 0 STRANDS rows offered in six games.
- **Wave-76 edits B1/B2/B3 (the converter refusal) — UNTESTED.** Both halves never stood together; no
  creature row printed the `ANY nonzero payment here is fatal` form, so the refusal branch never opened.
  B1's literal-string exception was quoted correctly as *not* firing at four Shield Sphere windows.
- **Wave-75 edit C (converter reorders the walls) — TESTED FOR THE FIRST TIME, and it UNDER-FIRES.**
  126 seq16; see guide edit B.
- **The `main_phase_windows_skipped` figure is benign**: 11, all in the 35-turn 125 game against 17
  own-turn main phases offered.
- **`own_loop_windows_asked` 0, `forced_close_unrecorded` 0, `stop_reached_windows_skipped` 0,
  `repeat_count_reask` 0, `declined_face_latches` 0 in all six gameends** — R1, the force-close bound and
  the land latch are all UNTESTED in this seat.

## 2. Engine / interface / card items

**HIGH-1 — `hold_check_ref_seq` does not resolve in the translog `seq` space, so the field a reviewer is
told to "use" points at the wrong record.** 161 records in this seat carry the field. Repro:
`1789106561-ai_baka_deck162-0x55e3c8746a40-vs-ai_baka_deck126.jsonl` **seq 5** (t4, the land-drop window)
carries `"hold_check_ref_seq": 1` and the bracket `[hold check: 1 row above is new since the last window I
asked you at this seam ...]`; **record seq 1 is the `system` record**, and the previous asked window at the
land seam is seq 3. No constant offset reconciles it either — the deltas `seq - ref` across the six games
run 3, 4, 5, 6, 7, 10, 15, 16, 17, 18 — and **12 of the 161 values name a seq no record in the file
carries at all** (e.g. `1789114834-...-vs-ai_baka_deck125.jsonl` seq 48 → ref 43, seq 60 → ref 42, seq 81
→ ref 66). The bracket's own CLAIM is true in every case I could test (see R2 above), so this is a
reporting defect, not a render defect: the ref counter appears to index windows (including hold-skipped
ones) while `seq` indexes records. Fix: emit the ref in the same space as `seq`, or rename it so no
consumer joins it against the translog. This matters now because the wave-77 brief instructs every
downstream seat to resolve it.

**HIGH-2 — `plan_names_uncastable_zone_card` fires on a spell's OWN resolution sub-menu.**
`1789114834-ai_baka_deck162-0x5625252eb900-vs-ai_baka_deck125.jsonl` **seq 36**, t22. The window is Peer
into the Abyss's mode choice; its header says so in the render's own words: `Choose an option for Peer into
the Abyss: the GAME LOG above may already say Peer into the Abyss resolved and went to a graveyard - it
did: the card leaves the stack as it resolves, and this choice is part of that resolution`. The plan echo
repeats the cast just made (`PLAN: Cast Peer into the Abyss targeting the opponent ...`) and the counter
flags it because Peer is now in the graveyard. **R9's cast-intent restriction does not exclude a
resolution sub-menu.** The seat answered correctly. This is 1 of the corpus's 52 — the false-positive rate
matters for the engine seat's sample of 20.

**MED-1 — the prompt grew 50% at the top end and half the deck's windows are now over 20 KB.**
Max 30,537 B (125 seq122) against wave 76's 20,195 B on the same seat, and **51 of 187 windows over
20 KB** against 1 of 131. The growth is the R5 `{effect:}` glosses plus opponent boards of 16–32
permanents each carrying one. Two consequences visible here: (a) latency p50 rose to 92.4 s with a 270 s
max, and (b) the snippet ceiling starts cutting glosses — see MED-2.

**MED-2 — the `{effect:}` snippet ceiling cuts a gloss at the verb, removing the effect and leaving the
trigger.** Two of the 16 distinct glosses in this seat are truncated, and one of them is the deck's own
key engine card: `1789114834-...-vs-ai_baka_deck125.jsonl` **seq 95**, `{effect: "At the beginning of each
player's draw step, if Howling Mine is untapped, that player..."}` — cut immediately before *draws an
additional card*. The same card renders in full in smaller prompts in the same game, so the reader is
shown a complete rule at one window and a conditional with no consequent at the next. The other is
`1789114900-...-vs-ai_baka_deck152.jsonl` seq 16, `"When this creature enters or transforms into Brutal
Cathar, exile target creature an..."` (cut before the object and the duration). Neither is a FALSE
statement — the ellipsis is honest — but a ceiling that cuts at the verb is the worst place to cut. Fix:
cut at the last clause boundary that fits, or drop the gloss entirely rather than emit a headless trigger.

**MED-3 — the ATTACKERS menu still prices attacking better than it prices staying home, but only because
STAY HOME is aggregate.** `1789114900-...-vs-ai_baka_deck152.jsonl` **seq 24**: `{crack-back cover, STAY
HOME: keeping all 4 of them back covers 16 of that 22, leaving 6}` — correct, and it worked (wave 76's
MED-2 is materially answered). The residue is that the figure is ALL-or-NOTHING: with one attacker offered
and four bodies on the board, the number the seat needs is what keeping *that one* home covers, not what
keeping all four home covers. Here the two happened to agree (only Master could attack). On a board where
two of four can attack, the menu gives no per-body figure and the seat is back to inference. Low priority
— this is a refinement of a clause that now exists and is true.

**LOW-1 — `unparsed_reply` in one of my games, on the opponent's seat, on a reply the parser should read.**
`1789114834-ai_baka_deck125-0x562523fd1ba0-vs-ai_baka_deck162.jsonl` **seq 49**, t15 Main phase 1. Reply
verbatim: `"\n\nPLAN: Cast nothing right now.\n4 (Cast nothing right now)"`. The row number and its short
name are on their own line and unambiguous against the four options; the only thing missing is the literal
`CHOICE:` label — the mirror image of the 13 `unlabelled_plan` deviations on MY seat, where the label is
dropped from the PLAN line instead. `chosen_text` reads `<refused: unparsed_reply>`. The seat wanted the
decline row and the heuristic's substitute also declined, so the play did not change that game (deck162
won it at t35). **Classification: a reply shape the parser SHOULD read
under the ruling.** One label-tolerant rule (`^\s*(\d+)\s*\(` on the last line) covers both deviation
families. Flagged for the engine seat's fallback census — this is 1 of the corpus's 6.

**LOW-2 — a plan line that contradicts its own CHOICE, unannotated.**
`1789114874-...-vs-ai_baka_deck146.jsonl` **seq 12**: reply `Play Swamp to enable casting Fate
Unraveler.\nCHOICE: 1 (Cast Fate Unraveler)`. The land drop was answered at seq11; the plan text describes
a window that has passed, and the echo at the next window repeats it. Harmless here (the CHOICE is right)
but it is the plan-echo staleness family, and the render does not contradict it.

**Passes worth recording.** No card behaved contrary to its rendered text in six games. Card facts
re-checked against the prompts: Fate Unraveler 3/4 `{3}{B}`, Ob Nixilis 5 loyalty with the `-2`, Shield
Sphere 0/6 taking `-0/-1` counters per block (rendered as `(0/4) (printed 0/6) [counters: 2x +0/-1]` at
152 seq24 — correct), Master of the Feast 5/5 flier, Peer into the Abyss's two modes, Howling Mine's
untapped condition. `LOOP HALF PENDING` at 126 seq16 is true and correctly scoped. The THEIR-card
perspective wrapper (`"you"/"your" in this text means THEM`) is on every opposing gloss. Max asks in any
one (turn, phase): **17** (162v125 t32, an Ob Nixilis trigger chain) — every one a distinct stack state,
no loop family.

## 3. Guide verdict: **EDIT**

`wave77/deck162/strategy.txt` — **19,947 bytes, from 19,973 (net −26)**, under the 20,000 ceiling. Two
edits in five places. **The guide does not grow**; both edits are paid for by decisions in this corpus and
both return bytes by removing a duplicate.

| # | before → after | paid for by |
|---|---|---|
| A | §CAST ORDER rung 4b: `MASTER OF THE FEAST {1}{b}{b} whenever their line reads 1 or more creatures. A 5/5 flier is a BODY and K does not brake a body; the card it hands them is the price of the only real blocker this deck casts.` → `MASTER OF THE FEAST {1}{b}{b} - the one body that also feeds them, so read THEIR line first. 1 or more creatures on it: cast at any K, the card it hands them being the price of the only real blocker this deck casts. "Of which 0 are creatures" on it: a clock, not a blocker, feeding them for nothing at K of 0 - cast it then only with no punisher in hand and every other row braked, stranded or unwanted.` **Plus**: §THE BRAKE's duplicate `MASTER OF THE FEAST IS NOT ON THIS BRAKE AT ALL - it is a body, and their line reading 1 or more creatures casts it at any K, this ask; only "of which 0 are creatures" on their line holds it.` → `MASTER OF THE FEAST IS NOT ON THIS BRAKE - rung 4b governs it, both of its cases.` **Plus**: §LAST CHECK drops the conjunct `AND (for Master) their line reading "of which 0 are creatures"` and the example `or "cast Master of the Feast for board presence"`, gaining `Master of the Feast is not a row this check reaches - rung 4b decides it.` | **125 seq8 (t6), 125 seq80 (t28), 130 seq7 (t5)** — three casts of Master at K = 0 with `of which 0 are creatures` on their line, which the LAST CHECK calls WRONG by name. The mechanism is in seq80's reasoning, which quotes rung 4b's CONDITION five separate times and each time lets the adjacent free-standing sentence override it: *"the guide says '4b ... whenever their line reads 1 or more creatures.' But it also says 'A 5/5 flier is a BODY and K does not brake a body'. So it's fine to cast it even with 0 creatures, to develop board."* The LAST CHECK's Master conjunct never entered the trace at all, and the reply is verbatim the sentence it names: `Cast Master of the Feast to develop board presence`. Amendments 333/334 both apply: a rationale clause stated as a universal is wider than its rule and wins; and the rule was stated in three places with three wordings, so the model could pick. The fix puts THEIR line first as the scope declaration, gives both cases in one place, and deletes the universal. **The permission direction follows the corpus, not my preference**: all three violations occurred in games the seat WON, in 130 the 5/5 was the clock that won it, and wave 76 recorded two more K-of-0 Master casts that "cost nothing in either" — five instances over two waves with no measured harm. So the second case is written as a narrow permission (dead menu, no punisher in hand) rather than as a re-tightened prohibition I have no evidence for. |
| B | §FORCED SACRIFICE: `A CONVERTER OF THEIRS REORDERS THE WALLS. Then "[they gain N life]" is also N off YOUR total in the same event, so among the WALLS take the LOWEST N on the board - Fog Bank over a fresh Shield Sphere - and read the number as damage to you. A punisher still goes last at every N.` → `A CONVERTER OF THEIRS REORDERS EVERY NON-PUNISHER ROW, and the ORDER above then decides only a tie. Then "[they gain N life]" is also N off YOUR total in the same event, so among those rows take the LOWEST N on the board - Fog Bank, or Master of the Feast over a fresh Shield Sphere - and read it as damage to you. A punisher still goes last at every N.` | **126 seq16.** Tribute to Hunger with Sanguine Bond live. Two rows: `Shield Sphere (0/6) ... they gain 6 life (its toughness) - and with their Sanguine Bond that is 6 off YOUR life` and `Master of the Feast (5/5) ... they gain 5 life ... 5 off YOUR life`. **The model worked out the better row unaided** — *"If I sacrifice Shield Sphere, they gain 6 life. With Sanguine Bond, I lose 6 life... If I sacrifice Master of the Feast, they gain 5... sacrificing Master of the Feast is better because it leaves them at 8 instead of 9"* — and was then overruled five times by the fixed ORDER: *"the guide says: 'ORDER: SHIELD SPHERE first ...'. So I will sacrifice Shield Sphere."* The reorder clause said `among the WALLS`, and with one wall and one non-wall body on the menu it reordered nothing, so the guide picked the row that pays them MORE and costs the seat MORE, against its own stated principle (`read the number as damage to you`). This is wave-75 edit C's branch opening for the first time and under-firing; the scope word was the defect, not the rule. Cost 1 life each way in a game already won — recorded as a rule that fires backwards on a converter board, not as a loss. |

**Bytes returned** (two, each unexercised as a decision in this wave and the last):
(i) §ATTACKING's closing aside `Master of the Feast also does not untap under Intruder Alarm.` — Intruder
Alarm WAS on the board this wave (162v123) and no attack decision turned on the sentence; it is also a
half-truth that invites the wrong inference, since Intruder Alarm untaps ALL creatures whenever any
creature enters, so the body is not stranded by attacking (162v123 seq12 relied on exactly that).
(ii) §MULLIGAN's rationale `You run 26 lands and no one-mana card, so a shipped six is most often a
two-lander again and` — the operative sentence (`"No cheap play" never justifies a ship.`) stays; 12
mulligan/bottom asks over two waves and none of them argued with the land count.

**Not changed, deliberately.** §BLOCKING in full — **a success verdict**: eight blocker windows across the
six games, every answer identical to the engine's own named best-case/least-damage assignment, including
both hopeless ones. The K-of-0 brake — **a success verdict**: 162v125 seq37–seq79, twenty consecutive
windows held with 13 idle sources, and that discipline is what let the seat rebuild and win a 35-turn
game. §ATTACKING's wave-76 crack-back rule (held at 152 seq24). §CAST ORDER rungs 0, 1a, 1b, 2, 3
(Shield Sphere taken at every free listing; the `{their converter:` exception quoted and correctly not
fired). §CARD FACTS Ob Nixilis targeting (decided 126). §LAND DROP — 44 of 44 land-drop asks
answered with a land.

## 4. Not checked / limits

- Only this seat's six translogs, the six opponent seats for outcomes, and no stderr files at all this
  wave (0 forced closes and 0 fallbacks on this seat left nothing to reconcile). No lane adjudication, no
  corpus counters beyond my six gameends, no other deck's seats, no engine source, no `askreplay/`
  sidecar, no `deadline_pct`/`transport` audit beyond confirming `http=200, empty=0` on the windows I
  opened.
- **HIGH-1 is an inference from outside the engine.** I show that `hold_check_ref_seq` does not resolve
  against record `seq` under offset 0 or 2 and that 12 values name no record; I did not read the code that
  emits it and cannot say which counter it indexes.
- The R2 hold-check result is "0 false NEW in 146 same-seam comparisons under a comparator I wrote", not a
  verification against `holdActionKeyRow`. My seam classifier is land/activated/cast by row prefix and
  mis-files the mulligan ask.
- **Edit A's second case is a permission written off three wins.** No window in this corpus shows a K-of-0
  Master cast COSTING anything, and none shows the seat declining one either — so the clause's restrictive
  half (`only with no punisher in hand and every other row braked, stranded or unwanted`) is untested in
  both directions. If a future corpus shows the seat casting Master over a live punisher row, that clause
  is where to look.
- **Edit B is paid by one window worth 1 life in a game already won.** The general case it now covers — a
  converter board where the cheapest body is not a wall — appeared once.
- The 146 and 152 losses are called unavoidable on the engine's own printed best-case lines and my own
  re-addition of the attacker totals, not on a replayed game. The 123 loss is called an opponent combo on
  the game log (the token burst is at their t10 upkeep, before any cast of the seat's that turn).
- **UNTESTED, not passed, in this seat**: R1 own-loop (`own_loop_windows_asked` 0), the force-close defer
  bound (0 closes), the land latch (`declined_face_latches` 0 for the 5th wave), wave-76 edits A/B1/B2/B3,
  ANNOUNCE_X and R10 (0 X windows), `[legendary:]` Ob Nixilis second copy (0 rows), ghostform `{removes:}`
  (0), MDFC land menus (0), cleanup discard (1 bottom ask at 125 seq64, consistent with the rule).
