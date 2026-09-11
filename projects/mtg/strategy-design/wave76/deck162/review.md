# Wave 76 — deck 162 ("Draw and Die!", UB forced-draw punish) per-deck review

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260910-141526-final`, SINGLE binary
`wagic-d372a1153-w76step1` (source `b4784ed05`), `--thinking on`. Six deck162 seat translogs, six
opponent seats for outcomes, three stderr files opened. **162v152 is a FIRST-RUN game** (start epoch
1789067732); the other five are reruns.

**131 model decisions, 0 fallbacks, 0 `reply_truncated`, 0 `action_before_plan`, native reasoning on
131/131.** `plan_line_missing` 6 (4.6%) = `protocol_deviation_replies` 6, all `unlabelled_plan`, each
with `off_protocol_bytes` equal to the length of the unlabelled sentence (18/33/38/49/62/73) — **the
wave-75 one-event identity recurs 6/6**. `plan_names_uncastable_zone_card` 4 (all 162v125),
`plan_names_stranded_card` 1 (162v123 seq10), `forced_close_unrecorded` 1 (162v126), `async_drops` 1
(162v126 seq20), `crossphase_identical_reputs` 0, `identical_ask_answers_reserved` 0,
`ask_replays_reserved` 0. Prompt bytes: max **20,195**, median 8,762, 1 window over 20 KB.

Record **3-3** (wave 75 also 3-3, different pairing): beat 125 (21–0, t20), 146 (3–0, t14), 123 (21–0,
t11); lost to 152 (−13–36), 126 (−4–33), 130 (−3–20). **One loss (126) is a guide failure at a single
casting window; one loss (130) is mana screw; 152 is a race the seat was behind from t6, with one
guide-violating attack at the end that did not change it.**

## 1. Game by game

| Opp | Result | Deciding decisions |
|---|---|---|
| 126 | **LOSS** | **seq20 (t14 MP1) is the loss.** Board: opponent at 9 life with BOTH halves of their life loop live (Sanguine Bond + Exquisite Blood), seat at 20 with **zero creatures**, P = 2 (Underworld Dreams + Ob Nixilis, cast that same window at seq19). The `Cast Shield Sphere {0}` row printed, verbatim: `{their converter: this body has toughness 6 and they control 1 life-to-damage converter (Sanguine Bond) - and BOTH halves of their life LOOP are in play, so NO figure is given for what this body costs you: the first life any effect of theirs gains off it re-enters the chain, which does not stop until you are at 0. ANY nonzero payment here is fatal, whatever your life total is}`. The seat cast it. Reasoning, twice, verbatim: *"'LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond... ANY nonzero payment here is fatal'. Shield Sphere costs 0, so it's safe."* and *"Shield Sphere is free. The guide says take it every time listed."* On the opponent's turn 15 they drew Tribute to Hunger, edicted that Shield Sphere in response to the lethal draw triggers, gained 6 (its toughness), and the Bond/Blood chain took the seat from 20 to −4 while they went 9 → 33. **With no creature on the seat's board that edict gains them nothing and the forecast lethal (`5 x 2 = 10 ... they would be at -1; that KILLS them`, seq22) resolves.** Two guide rules put that body on the board: §CAST ORDER rung 3 and §WALLS, both unconditional absolutes. **Paid edits B1/B2/B3.** Everything else in this game was right: seq13 (t11 Tribute to Hunger) sacrificed Master of the Feast over Fate Unraveler per §FORCED SACRIFICE ORDER with the loop still PENDING (Sanguine Bond in hand); seq19/21 cast Ob Nixilis then the third Howling Mine at K = 2. |
| 152 | **LOSS** (first-run) | Race loss: deck152 was at 3 attackers by t9 and the seat's first body landed t8. seq11 (t9 Blockers, 10 life, lone Fate Unraveler + Shield Sphere) is **correct on wave-75 edit A** — it blocked the 5/5 lifelink with Shield Sphere on `(neither dies)` and left the one punisher home; rule (a) held where it failed last wave. **seq14 (t10 Attackers) is a guide violation**, at 5 life with `CRACK-BACK NEXT TURN ... 17 ... that would KILL you`: the Fate Unraveler tag read `Luminarch Aspirant (you kill it, your attacker lives); Sigarda, Champion of Light (your attacker dies, their blocker lives)` and the ATTACK TOTAL read `That is NOT lethal ... Their 2 untapped blockers can cover every attacker`. Three separate §ATTACKING rules said stay home (one fatal name; the cover line; a count of ONE at 8 or less). The seat attacked. Reasoning verbatim: *"I am dead next turn regardless... the guide says: 'If M > N and the CRACK-BACK NEXT TURN line reads that would KILL you: take every creature row you can afford...'. So I should attack."* — **§CAST ORDER rung 0, a CASTING rung, read as an ATTACK licence**, plus the dead-anyway override, plus a false premise (*"Opponent's creatures have flying. So Fate Unraveler cannot block them"* — 2 of their 4 attackers are ground creatures). **Paid edit C**, plus engine item MED-2. |
| 130 | **LOSS** | Not guide-addressable. Mulliganed to five (seq2/3/4), bottomed 2, and never had more than **2 untapped sources in 20 turns** — Underworld Dreams {b}{b}{b}, Fate Unraveler {3}{b}, Master of the Feast and two Dictates sat uncastable in hand from t7 to the end (`cannot pay now: needs 3 mana, you have 0/1/2 untapped sources` on every record from seq8 to seq17). The five blocker windows (seq9, 12, 14, 16, 17) were all `(neither dies)` Shield-Sphere/Fog-Bank blocks, rung 2, correct each time; seq11 took Fog Bank at rung 4 the first ask it was offered with 2 mana. Nothing the guide says would have produced a punisher. |
| 123 | **WIN** t11 | Shield Sphere t1, Underworld Dreams t5, Dictate t7, Ob Nixilis t9 with `-2` on **Thraben Doomsayer** (seq14 — §CARD FACTS "aim it FIRST at a creature whose text makes more creatures" applied by name), Fate Unraveler t11, second `-2` at seq18 for the kill. **seq10 (t7 Upkeep) is a rule violation that did not cost the game**: the `Cast Dictate of Kruphix` row carried `VERDICT: taking this row STRANDS Fog Bank this turn` and the seat took it (`plan_names_stranded_card: Fog Bank`). It quoted the strike-out rule and then refuted it: *"the guide says: 'A row whose VERDICT begins taking this row STRANDS is NOT ON YOUR MENU'. This is for CASTING decisions. But here, the menu explicitly gives me the choice. The strand warning is just information."* It also weighed `K OF 1 OR MORE: cast every engine offered` against it four times. **Paid edit A.** Wave 75 reported 2 STRANDS rows offered / 0 taken; this wave 1 offered / **1 taken** — a regression of the wave-74 class in the one place the rule's wording could be argued with. |
| 146 | **WIN** t14, at 3 life | seq14 (t9) blocked Nadaar on `(you kill it, your blocker lives)`, rung 1. **seq18 (t10 Attackers) correctly answered `ATTACK: none` off a GANG BLOCK clause** — the §ATTACKING gang rule doing its job on the same seat that broke the low-life rule in 152. seq23/24 (t12) cast Ob Nixilis and `-2`'d Barrowin; seq26 cast Dictate at K ≥ 1 at 3 life; seq29 Teferi's Puzzle Box for the kill. |
| 125 | **WIN** t20 | Two Shield Spheres t2/t4, Underworld Dreams t6, Fate Unraveler t8, Caress t10. Opponent killed both punishers; from seq17 (t10) to seq24 (t14) the seat correctly **held Dictate at K = 0** through five windows (`draw converters in your hand: 0` — brake stands), then rebuilt: Underworld Dreams + Dictate t14, Mine t16, Underworld Dreams + Master + Mine t18, Peer into the Abyss t20 answered `CHOICE: 1 (the opponent)`. The K-of-0 brake is a **success verdict** here — it is the rule that kept the deck from feeding a board with no punishers on it. |

### Wave-75 items: what recurs

- **Wave-75 edit A (rule (a) hoisted above the blocking rungs) HOLDS** — 152 seq11 is the same shape as
  the window that lost wave 75's 152 game, answered correctly, with the lone punisher kept home. Success.
- **Wave-75 edit B (first Howling Mine at K = 0) HOLDS** — no K-of-0 window in six games answered
  "Cast nothing" with a Mine row and idle mana; 126 seq5 took a first Mine at K = 0 on t4.
- **Wave-75 edit C (converter reorders the walls) UNTESTED** — the only forced-sacrifice ask (126 seq13)
  came while Sanguine Bond was still in hand (`LOOP HALF PENDING`), so no converter was on the
  battlefield and the Q6 `off YOUR life` clause was correctly **absent**. Edit C's branch never opened.
- **Wave-75 edit D (discard/bottom order) UNTESTED** — 0 cleanup discards, 2 bottom asks (125 seq4 put
  Forced Fruition, the most expensive spell; 130 seq5 put Peer into the Abyss and a land at a 5-card
  keep). Both consistent with the rule.
- **The `off_protocol_bytes` = `plan_line_missing` identity recurs**, 6/6.
- **The wave-74/75 STRANDS class RECURS** (123 seq10) after a wave of 0/2 — see edit A.
- **Wave-75's `main_phase_windows_skipped` MED is GONE in this seat**: 5 of 6 games read 0, the sixth
  (123) reads 2 against 6 own-turn main phases offered. Consistent with the CN turn-index fix (Q7).
- **Wave-75's `[hold check:]` false-NEW MED does not recur in this seat** — see engine section.

## 2. Engine / interface / card items

**HIGH — the `{their converter:}` clause on a creature row reads as a statement about the row's COST,
and the model resolves it that way.** `1789074113-ai_baka_deck162-0x558dc361e320-vs-ai_baka_deck126.jsonl`
**seq 20**, t14 MP1. Rendered on row 3: `Cast Shield Sphere {0} (0/6) ... {their converter: this body has
toughness 6 and they control 1 life-to-damage converter (Sanguine Bond) - and BOTH halves of their life
LOOP are in play, so NO figure is given for what this body costs you: the first life any effect of theirs
gains off it re-enters the chain ... ANY nonzero payment here is fatal, whatever your life total is}`.
The reasoning resolves it twice as a mana statement: *"Shield Sphere costs 0, so it's safe."* The clause
never names the EVENT that makes the payment — it says "the first life any effect of theirs gains off
it", which the model read as an effect that has to exist already, and no effect of theirs on the board
gained life off a body. The decision lost the game one turn later to a Tribute to Hunger they drew.
Fix: the clause should name the trigger in the row's own words — *"if this body ever leaves the
battlefield to an effect of theirs that gains life (a forced sacrifice gains them its TOUGHNESS, 6), the
chain starts and does not stop; the {0} in this row's cost is not what this row costs you"* — and, where
the engine can see it, cite the outlet it has already seen (`Tribute to Hunger x2 in their graveyard`).
Same family as the wave-75 HIGH (the sacrifice row stating half the price) one seam earlier: the cost is
stated at the wrong window.

**MED-2 — the ATTACKERS menu prices attacking but never prices staying home.**
`1789067735-ai_baka_deck162-0x55b5408eb9d0-vs-ai_baka_deck152.jsonl` **seq 14**, t10, 5 life. The prompt
carries `CRACK-BACK NEXT TURN: ... up to 17 ... you would be at -12 or lower; that would KILL you - of
that, 9 from 2 attackers nothing you control can legally block` and a generic `CRACK-BACK COST OF
ATTACKING` paragraph (`every attacker you declare without vigilance is one fewer blocker against that
line, and each blocker you keep back stops at most ONE attacker`) — with **no number**. Casting rows in
the same corpus DO get the number: `{crack-back cover: ... Counting those bodies AND the checked new ones
you cover 3 of 6, leaving 3 -> you would be at 15}` (162v130 seq11). The model filled the gap by
inventing one, wrongly: *"Next turn they will attack with 4 creatures, including 2 flyers ... I cannot
block flyers ... Fate Unraveler cannot block them"* — false, 2 of the 4 are ground creatures the 3/4
could have eaten. Fix: give each offered attacker the mirror of `{crack-back cover:}` — `keeping this
one home covers N of the crack-back (it can legally block X, Y)` — so the trade is arithmetic on both
sides of the menu instead of arithmetic on one side and inference on the other.

**MED-3 — `forced_close_unrecorded: 1` reconciles to a re-ask, not a lost decision.**
`game-126v162-1789074110.stderr` line 1795 `AIPlayerGPT: unclosed <think> (budget/truncation); forcing
the answer`, immediately followed at line 1797 by `dropping stale async answer (casting arm; the question
and board moved) [outcome: re-asked]`; the record that follows is **seq20** carrying `async_drops: 1`.
So the forced close was superseded by the re-ask, no decision fell to the heuristic (0 fallbacks agrees),
and the counter is naming an arm whose answer was legitimately discarded. Flagging for the engine seat's
reconciliation of the five corpus-wide closes: at least this one is benign and the counter reads as a
defect when it is not. `phase2_answer_recovered` 0 / `_missing` 0 in all six of my gameends.

**LOW — the plan echo names cards in the seat's own GRAVEYARD, four times in one game, and the render
never contradicts it.** `1789074098-...-vs-ai_baka_deck125.jsonl` seq **17, 21, 28, 32**. At seq21 the
prompt prints `Your graveyard (2 cards): Underworld Dreams {b}{b}{b}; Fate Unraveler {3}{b}` and the
reply reads `PLAN: End turn. Cast Fate Unraveler next turn after land drop.` The `THE PLAN YOU LAST
STATED` echo then repeats that plan back verbatim at the next window with no annotation. All four hits
are **real, not exempt** (no self-recursion, no cast-from-exile): the card is in the seat's graveyard,
destroyed. The measure is doing its job; the render could close the loop by extending the existing
"`X` is no longer on your menu" parenthesis on the plan echo to name the zone (`Fate Unraveler is in
your graveyard`). No decision was lost — the seat still answered each window correctly and won the game.

**Q1 (hold-check bracket) — 0 false-NEW in this seat, with a caveat.** 57 `[hold check:]` brackets across
the six games; 39 claim "every row above was also on the menu", 18 claim N rows new. I diffed each NEW
claim against the previous window at the same seam with all `{...}` and `[...]` groups stripped: **in 0
of 18 did the engine claim more new rows than a group-stripped diff finds**, and in 9 of 18 it claimed
FEWER (it folds pass-row wording and re-worded rows my crude diff separates). The wave-75 residual —
`{leaves N of your M ...}` / `{spends ...}` / `{reserve: ...}` movement counted as NEW — **does not
appear here**. Caveat: my comparator is coarser than `holdActionKeyRow` and I did not read the render
code; this is "no counterexample found in 18", not a proof.

**Passes worth recording.** No card behaved contrary to its rendered text in six games; card facts
re-checked against the prompts (Fate Unraveler 3/4 `{3}{B}`, Ob Nixilis 5 loyalty with the `-2`
destroy-and-draw-two, Shield Sphere 0/6, Peer into the Abyss's two modes, Master of the Feast 5/5 flier).
`LOOP SCOPE:` **does exist** in the renders (162v126 seq21/22/23, on both the DRAW PUNISHERS and DRAW
FORECAST lines) — wave 75's LOW item said the label appears 0 times in its corpus and cut the guide
sentence naming it; the label is real, the cut still stands since the guide should not name render
labels. The loop narration in 162v126 is TRUE at every window I checked, including the correct
`LOOP HALF PENDING` at seq13 while Sanguine Bond was in hand and the correct "life THEY lose does not
enter it" scoping of the draw forecast. `[HOW A HOLD ENDS:` appears exactly once in each prompt that
renders a hold row. Max asks in any one (turn, phase): **4** (162v126 t14, 162v125 t18) — no loop family.

## 3. Guide verdict: **EDIT**

`wave76/deck162/strategy.txt` — **19,973 bytes, from 19,958 (net +15)**, under the 20,000 ceiling. Three
edits in five places, each paid by a decision in this corpus. The +15 is bought by the four cuts named
after the table; **the rule that earns new bytes is B3 (the loop-body refusal), the one that lost the
one loss this corpus can attribute to the guide.**

| # | before → after | paid for by |
|---|---|---|
| A | §CAST ORDER: `a row whose VERDICT begins "taking this row STRANDS" is NOT ON YOUR MENU - strike it out and read on past it. That is a CASTING rule and nothing in ATTACKING scopes it, no rung reaches a struck row, and no count revives it.` → `a row whose VERDICT begins "taking this row STRANDS" is NEVER YOUR ANSWER. It is printed because it is LEGAL, not because it is a play; "the engine put it on the list" is not a licence to take it, and neither is a count. Read on past it. No rung reaches it, and nothing in ATTACKING scopes this - it is a CASTING rule.` The §THE BRAKE LAST CHECK restatement is shortened to match. | **123 seq10.** The old rule made a claim about the MENU'S CONTENTS, and the model refuted it from the screen: *"here, the menu explicitly gives me the choice. The strand warning is just information."* A claim the render falsifies cannot carry a prohibition; the rule is now about the ANSWER, which nothing on the screen contradicts, and it pre-empts the exact sentence the model used. |
| B1 | §CAST ORDER rung 3: `SHIELD SPHERE {0} - free, take it every time it is listed.` → `... take it every time it is listed UNLESS its row prints "{their converter:".` | **126 seq20.** The rung is what the model quoted as its authority. The exception is keyed to a literal string on the row so it cannot be argued about. |
| B2 | §WALLS: `Shield Sphere {0} costs ZERO: take it every single time it is listed, with a full board, with no other play, on any turn.` → `Shield Sphere {0} costs ZERO: with no life-to-damage LOOP of theirs on the battlefield, take it every single time it is listed, with a full board, with no other play, on any turn.` | **126 seq20.** Amendment 333: the rule now declares its scope in its first clause instead of stating a universal it does not have. The list of hard cases it does cover (full board, no other play, any turn) is untouched. |
| B3 | §SANGUINE BOND gains: `WHILE BOTH HALVES STAND, DO NOT PUT A BODY ON THE BOARD FOR THEM TO CONVERT. A creature row of yours printing "{their converter: ... ANY nonzero payment here is fatal}" is refused at EVERY price, {0} included: that clause is not about what the row costs YOU to cast, it is about what THEY collect when that body dies or is sacrificed - a forced-sacrifice effect gains them its toughness, and that gain enters the chain and does not stop. With no creature of yours on the battlefield their edict gains them nothing. Take a noncreature row, or decline.` The sentence `A headline about payments "lethal to you" is about life YOU pay.` is cut. | **126 seq20**, the loss. The model read the fatal-payment clause as a cost statement twice and resolved it with *"Shield Sphere costs 0, so it's safe"*; the cut sentence pointed in exactly that direction. The rule is stated where the loop is discussed, names the price in the units the render uses (toughness), and gives the counterfactual that decided the game — an empty board makes their edict blank. |
| C | §ATTACKING: `...does not attack, whatever its tag says - usually Master of the Feast, which also does not untap under Intruder Alarm.` → `...does not attack, whatever its tag says. A CRACK-BACK NEXT TURN line reading "that would KILL you" is the REASON that body stays home: it blocks one of those attackers. "I am dead next turn anyway" is not an argument, and rung 0 of CAST ORDER answers a CASTING ask about creature ROWS - it never answers an ATTACK question. Master of the Feast also does not untap under Intruder Alarm.` | **152 seq14.** Two failures in one trace: the `- usually Master of the Feast` aside narrowed the rule to one card and the seat was attacking with Fate Unraveler, and the model then took rung 0 (a CASTING rung) as a licence to attack and "dead anyway" as a release. Amendment 334: the old aside was a concessive naming the rule's typical case and it shrank the rule; the replacement names the hard case (a lethal crack-back) as the rule's REASON rather than its exception, and closes the rung-0 leak at the seam where it leaked. |

**Bytes returned** (each unexercised across this wave and the last):
(i) RELEASE condition (i) COLOUR, the per-card black-source test — **no decision in three waves turned on
it**, and the surviving THE BOARD condition is the one that fires; (ii) the whole **LAST-DITCH RELEASE**
paragraph (four printed conditions at once) — never satisfied in 247 decisions over two waves, and the
wave-75 first-Howling-Mine carve-out already covers the dead-menu case it was written for; (iii) the
§LAND DROP sentence `You cannot hold mana on your own main for your own sorcery-speed card...` — a
restatement of what every row's own `{reserve:}` clause says; (iv) the §LETHAL CHECK tail `the next
punisher adds D + 1 a turn where the next engine adds only P`, a verbatim duplicate of the headline
ARITHMETIC paragraph.

**Not changed, deliberately.** §BLOCKING in full (**a success verdict**: five blocker windows in 130 and
one each in 152/146/123, every one on the rung the guide names, and wave-75 edit A's rule-(a) precedence
held at 152 seq11 where its predecessor lost the game). The K-of-0 brake (**a success verdict**: 125
seq17-24, five windows held with idle mana, and the seat rebuilt and won). §FORCED SACRIFICE (126 seq13,
correct). §CAST ORDER rungs 1a/1b/4b (Master of the Feast cast at K = 0 in 126 seq9 and 123 seq13 off
rung 4b, cost nothing in either). §CARD FACTS Ob Nixilis targeting (decided 123). §LAND DROP's rule
itself — every land-drop ask with a playable land was answered with a land, 21 of 21.

## 4. Not checked / limits

- Only this seat's six translogs, the six opponent seats for outcomes, and three stderr files
  (126v162, and spot checks). No lane adjudication, no corpus counters beyond my six gameends, no other
  deck's seats, no engine source, no `askreplay/` sidecar, no latency/`deadline_pct`/`transport` audit.
- **The 126 counterfactual** — that declining Shield Sphere at seq20 wins — rests on the engine's own
  `DRAW FORECAST` at seq22 (`5 x 2 = 10 ... they would be at -1; that KILLS them`) and on Tribute to
  Hunger being their only outlet in hand at t15. They held 8 cards; another life-gain effect would have
  found the same chain. It is not a replayed game.
- **The B-family edits are untested against the case where a body is needed to survive.** In 126 the seat
  was at 20 life against a board of two defenders, so refusing the body cost nothing. A board where
  rung 0 (BODY TURN) and B3 both fire — a lethal crack-back AND a live loop — is not in this corpus and
  the guide does not say which wins. I did not add a tie-break I have no evidence for.
- Cutting LAST-DITCH RELEASE removes the only path that casts a Dictate at K = 0. If a future corpus
  shows a seat stuck behind the brake on a dead menu with no Mine in hand, that is the cut to reverse.
- The Q1 hold-check result is "no counterexample in 18 NEW claims" under a comparator I wrote, not a
  verification against `holdActionKeyRow`.
- **UNTESTED, not passed, in this seat**: Q6's `off YOUR life` sacrifice clause (the one sacrifice ask
  had the loop still pending), Q11 up-to-N loyalty rows (both Ob Nixilis `-2` asks were single-target),
  Q2's `- THEIRS:` victim roster (0 renders), Q15 `forced_close_prefill_echo` (0), Q10 ghostform
  `{removes:}` (0), P15 X ladders (0 `ANNOUNCE_X`), P18 MDFC land menus (0), the Ob Nixilis legend-rule
  loyalty-2 exception (0 `[legendary:]` rows this wave), `repeat_count_reask` (0 in this seat).
- The 130 loss is called variance on the mana record above, not on a replay.
