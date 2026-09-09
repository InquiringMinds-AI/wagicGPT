# Wave-73 deck 152 review — BANT HUMAN MIDRANGE

Corpus `matchups-20260907-225637-final`, seat files `...ai_baka_deck152-<ptr>-vs-...` (6 games, all natural
ends; the vs-deck125 game is a rerun and is read as an ordinary corpus game).
**222 model decisions, 222 with native reasoning** (`reasoning_chars` min 1,179 / p50 5,444 / max 13,618),
**0 fallbacks of any kind**, 0 `reply_truncated`, 0 `action_before_plan`. `plan_line_missing` 16 (7.2%),
`off_protocol_bytes>0` 16 (4.7% of bytes; the SAME 16 records — see MED-1). `async_drops` 4,
`ask_replays_reserved` 2, `identical_option_asks_resolved` 1.
Live guide read = the wave-72 edit (19,995 B, byte-identical to `wave72/deck152/strategy.txt`). Seat **5-1**.

**Wave-73 census for this seat, all six gameends: `hold_windows_skipped` 0, `hold_released_turn` 0,
`sibling_window_asks_skipped` 0, `stop_reached_windows_skipped` 0, `chain_windows_collapsed` 0,
`reserve_decline_windows_noted` 0 / skipped 0, `repeat_annotated_takes` 0.** The seat took **no HOLD row at
all** in 222 decisions, so this seat contributes nothing to the wave-73 hold-release trace; the release
machinery is untested here rather than passing. The same machinery *is* alive in these games on the
OPPONENT's seat (`game-152v123-1788839827.stderr` lines 1613-1625: deck123's repeat-stop collapses at its own
COUNT of 27, and its reserve-decline latch at line 1204), so the zeros are this seat's behaviour, not a dead
instrument. `main_phase_windows_skipped` 10/4/2/1/18/12 = 47; `own_turn_windows_skipped` 32/29/0/51/52/55.

## 1 — Game by game

| # | Opp | Result | Turn | Life | Deciding decisions |
|---|-----|--------|------|------|--------------------|
| 1 | 162 | **win** | 11 | 16 / -7 | clean curve; seq 26 four-attacker alpha. **T5 attack window never fired — HIGH-1** |
| 2 | 126 | **win** | 15 | 20 / -10 | seq 30/31 Fateful Absence on Sorin; seq 46 lethal alpha; seq 48 Absence on the last blocker mid-combat |
| 3 | 146 | **win** | 18 | 11 / -1 | seq 11/19/26 three correct legend-rule declines (guide deploy-floor case (c)); seq 28/30 Katilda pump then seq 29/31 lethal |
| 4 | 123 | **loss** | 18 | -26 / 11 | **seq 21 → the T15 attack that was never asked for — HIGH-1.** Opponent then comboed (see below) |
| 5 | 130 | **win** | 23 | 14 / -4 | **seq 20 — see MED-2**, the turn-11 attack sold for a Brutal Cathar; seq 37/38 Absence on Rorix; seq 58 lethal |
| 6 | 125 | **win** | 22 | 49 / -4 | Intrepid Adversary lifelink race under Lightmine Field; seq 27/28 counters, seq 31/34/38 attacks |

**Game 4 in full.** The seat was at 20 life with the opponent at 9 on turn 15, holding Briarbridge Tracker
(5/4, vigilance, untapped) and a 2/2 Wolf against a lone Bloodline Keeper. It cast Sigarda off four LAND
sources (`Paid {1}{g}{w}{w} ... (4 sources)`, no creature tapped) and **was never asked to attack** — 7 power
into one blocker went unspent. On turn 16 the opponent's Bloodline Keeper + Intruder Alarm made 26 Vampire
tokens in a single upkeep; turn 17 the seat again got no attackers window; turn 18 it took a 25-attacker,
50-damage swing (`INCOMING THIS COMBAT: 25 attackers, 50 unblocked damage`). Bloodline Keeper
(`mtg.txt:12582`, `auto={T}:token(Vampire,...)`) and Intruder Alarm (`mtg.txt:58850`,
`auto=@movedTo(creature|myBattlefield):untap all(creature)`) are both faithful to their Oracle text — the
combo is real MTG and nothing in a deck guide answers it once it lands. What the guide could have banked is
the turn-15 attack, and the guide never got the chance.

## 2 — Engine / interface / card items

### HIGH-1 — the declare-attackers window is silently skipped on turns the seat has legal attackers (the N-146s class, still live)
Three instances, all on the seat's own turn, all with the engine's OWN board line saying attackers exist:

1. `1788839806-ai_baka_deck152-0x558b855f0f40-vs-ai_baka_deck162.jsonl` seq 9 (T5, Combat begins). The
   Luminarch Aspirant trigger's target list prints `1. Luminarch Aspirant (2/2) [your battlefield]` — no
   `[tapped]`, no `[summoning sick]`, so it was a legal attacker. The next record, seq 10, is **turn 7**
   Upkeep. In `game-152v162-1788839803.stderr` the whole turn segment (line 455 `Untap Phase ... Turn 4` to
   line 559 `Turn 5`) contains **no `[combattrace]` line for seat `0x558b855f0f40` and no
   `declared attack`** — the gate in `GameObserver::pendingCombatDecision` was never reached, so not even
   `no-legal-attacker` was printed.
2. `1788839831-ai_baka_deck152-0x55cb1a502f10-vs-ai_baka_deck123.jsonl` seq 21 (T15, Main 1). Board line:
   `Your battlefield (7 permanents listed, of which 2 are creatures, 2 of them able to attack right now ...)`
   — Briarbridge Tracker (5/4, vigilance) and Wolf (2/2). Opponent at 9 with one creature. The rendered game
   log for turn 15 (visible in seq 24's prompt) shows Draw → Main phase 1 → cast Sigarda → **Turn 16**: no
   `Phase: Combat begins`, no attack. `game-152v123-1788839827.stderr` has no `[combattrace]` for
   `0x55cb1a502f10` between line 1157 (`Turn 14`) and line 1200 (`Turn 15`).
3. Same file, T17. seq 23's board line again reads `2 of them able to attack right now` (Briarbridge Tracker
   4/5 vigilance, Sigarda 4/4 flying, both untapped and unsick). The log prints `Phase: Combat begins` and
   then goes straight to Turn 18. No attackers record, no combattrace.

**Not this bug** (checked and excluded): the vs-deck146 T8/T12 and vs-deck130 T11 turns also show
`able to attack right now` > 0 at Main 1, but in each the seat then tapped Katilda / Intrepid Adversary FOR
MANA, so `hasLegalAttacker` was correctly false and the tracer duly printed
`[combattrace] ... suppressed by no-legal-attacker` (e.g. `game-152v130-1788839829.stderr:1158`, t10). The
three above are the ones where the tracer printed **nothing at all**, which is the diagnostic difference: the
suppression is upstream of `pendingCombatDecision`, not inside it. That is exactly the failure the tracer was
installed for (`GameObserver.cpp:409-425`, the N-146s note: "received NO attacker asks for nine consecutive
turns while holding legal attackers"). The tracer as written cannot see it, because it only fires from inside
the gate; a counterpart trace at the *entry* to `MTG_PHASE_COMBATATTACKERS` (turn, seat, `hasLegalAttacker`,
whether `chooseAttackers` was called) would name the missing caller on the next corpus.
Cost: instance 2 is the deciding decision of this seat's only loss.

### MED-1 — every off-protocol byte in this seat is a correct plan sentence with its label dropped, and the wave-73 heading change did not move the rate
All 16 `plan_line_missing` records are exactly the 16 `off_protocol_bytes>0` records; the correlation is 1.00.
Line 1 of each is a correct, single-sentence plan with the `PLAN:` label missing:
`Attack with all four creatures.` (deck162 seq 23, 31 B) · `Cast Brutal Cathar to add a body to the board and
apply pressure.` (deck146 seq 23, 65 B) · `Attack with all creatures to maximize damage, accepting Luminarch
Aspirant's death to put the opponent at 4 life.` (deck125 seq 34, 113 B). None is prose after the action line,
none reverses the action, none exceeds one line. The meter is therefore charging the seat off-protocol bytes
for a plan that is present and right. Wave 72 blamed the carried-plan heading's caps phrase; wave 73 changed
that heading and the rate went **5.2% → 7.2%**, so the collision was not the cause. Two candidate fixes for
the engine seat: accept an unlabelled line 1 as the plan when a labelled action line follows (the shape is
unambiguous), or stop counting the plan's own bytes as off-protocol.

### MED-2 — the mana bill is rendered immediately after a decoy that reads almost identically, and the model read the decoy
`1788839833-ai_baka_deck152-0x558d2cb2edb0-vs-ai_baka_deck130.jsonl` seq 20 (T11, Main 1, 20 vs 20). Row 1:
`Cast Brutal Cathar {2}{w} (2/2) {leaves 0 of your 3 untapped mana sources untapped - casting this taps you
out} {paying this taps: Intrepid Adversary, Katilda, Dawnhart Prime - they cannot attack this turn} {card
text: ...}`. The seat's other three sources were lands already spent, so the two creatures WERE the payment.
The `reasoning` quotes the first bracket twice (`"leaves 0 of your 3 untapped mana sources untapped"`,
`"casting this taps you out"`) and never once the second; the reply's plan is
`Cast Brutal Cathar to exile Dwarven Blastminer, **attack with Intrepid Adversary and Katilda for 6 damage**`
— a plan the row had already ruled out on its own face. No attack happened that turn (correctly: no legal
attacker) and the game ran 12 more turns. Surface true and complete → classified STRATEGY, guide edit 1; but
the render invites it: the two brackets are adjacent, both begin with the same currency, and only the second
names bodies. Folding them (`... taps you out, and 2 of those 3 sources are creatures: Intrepid Adversary,
Katilda, Dawnhart Prime - they cannot attack this turn`) puts one fact in one place.

### MED-3 — the live guide's model of Teferi's +1 cascade is FALSE, and the render is the correct half
The guide says `STAGE 1 ... is THE ONLY DECLINE WINDOW; STAGES 2 and 3 have no pass row`. The render actually
gives **each mode stage** its own decline: `...vs-deck130` seq 26 (`1. choose a land / 2. Decline - do
nothing`), seq 29 (`1. choose an artifact / 2. Decline - do nothing`), seq 46 (same, on the T19 activation).
It is the SIDE pick (seq 27, `this ask has no pass row`) and the TARGET list (seq 28, same) that cannot be
declined. The seat declined at seq 29 and seq 46 and got the 2 life both times, so the false guide sentence
cost nothing here — but a guide stating a false fact about a window is a defect on its own, and this one
would suppress a correct decline the moment the model believed it. Guide edit 2. Engine half: none needed,
the render is right; the stage menus also carry a genuinely useful reachability gloss
(`"choose opponent artifact" - NONE. A branch at NONE has no legal object at all this window`).

### LOW-1 — `main_phase_windows_skipped` (CB F6) reads as a per-PHASE meter, not a "phase with nothing to do" meter
47 skips across this seat's ~50 own main phases, concentrated in the long games (18 in the 23-turn vs-deck130
game, 12 in the 22-turn vs-deck125 game). The one instance I traced,
`game-152v123-1788839827.stderr:1778` — `own main phase (turn 17, phase 4) ENDED with NO casting window - the
instant-speed response arm answered it` — sits in the SAME turn as a real translog casting record (seq 23,
T17 Main phase 1, `Cast Briarbridge Tracker`), so the counter is scoped to a phase id, not to the turn, and
"no casting window" means "not at this phase id". On that reading 375 corpus-wide is plausible arithmetic
(~1 per own main phase past the early turns). As a meter of *phases the seat could not act in* it over-counts,
because the phase it names is one the seat did act in by another arm. The engine seat owns the fix; the
concrete instance is above.

### LOW-2 — Teferi's +1 land pick drifted from its own plan with no render aid
`...vs-deck130` seq 28: all four target rows read `[tapped]` (correct — untapping your own tapped land is the
point, and seq 27 row 1 glosses it exactly right). The PLAN said `Untap Branchloft Pathway`; the answer was
row 1, `Boulderloft Pathway`. Both are the same physical card's two faces, so the pick was harmless, but the
target list does not say so — `Boulderloft Pathway ... // Branchloft Pathway` is in the row's card text and
nowhere in its label. A `[this is the other face of "Branchloft Pathway"]` tag on Pathway target rows would
close it. No cost this wave.

### LOW-3 — a guide rule violated at no cost: Teferi tapped THEIR creature on the seat's OWN turn
`...vs-deck130` seq 44/45 (T19): `choose opponent creature` → `Siege-Gang Commander`, reasoning "to prevent
its ability and block". The guide already says plainly to tap an ATTACKER on THEIR turn. The Commander
attacked at T20 anyway (seq 47 blocked it). Rule present, not followed, no consequence — recorded, not
edited.

### Verified NOT defects this wave
- **The wave-73 hold row is correct and complete**: it leads with its cost (`pass now, and do not ask me
  again - YOU CANNOT COME BACK AND TAKE ONE OF THE ROWS ABOVE LATER THIS TURN`) and carries the new release
  clause (`it is released at the start of your next turn whatever the rows do`). Priority menus print
  `0. Pass priority` explicitly (taken at deck126 seq 22/seq 28, deck130 seq 21/23).
- **The legend-rule tag reaches cast rows** and the guide's deploy-floor exception (c) fired correctly three
  times (deck146 seq 11, 19, 26 — all `Cast Katilda ... [legendary: you already control Katilda ...]` with a
  healthy copy out, all declined).
- **43 windows offered `Play no land right now`; it was answered zero times.**
- Bloodline Keeper, Intruder Alarm (both `mtg.txt`, cited in §1) match Scryfall's Oracle text.

## 3 — Did wave 72's edits pay off?

- **Edit 1 (HOLD barred while an unused activated ability stands) — PASS, and no over-correction.** The seat
  took the hold row **zero** times in 222 decisions (`hold_windows_skipped` 0 in all six gameends), and the
  once-per-turn abilities it silenced last wave were all used: Teferi activated on T15, T17, T19 and T21 of
  the vs-deck130 game, Ranger Class levelled at deck123 seq 18 and deck125 seq 22/23, Katilda's pump at
  deck146 seq 24/28/30. Wave-72 HIGH-1 **does not recur**.
- **Edit 4 (Teferi used every turn) — PASS.** The only game in which Teferi resolved is vs-deck130 and he was
  activated on every one of the seat's turns from T15 to T21.
- **Edit 3 (land drop never declined) — PASS, 43/43** (see §2).
- **Edit 5 (deploy floor covers a lost game) — UNTESTED**; no "accept loss" plan appeared. The seat's one
  loss ended before a concession window could form.
- **Edit 2 (Lair: biggest rung, and the attack rules bind the animated land) — UNTESTED.** No animation
  window fired at all this wave (`[Upkeep offer:` appears 0 times in this seat's 222 prompts). The one Lair
  decision was deck126 seq 41, `Play Lair of the Hydra` as the land drop — the half edit 3 asked for.
- **Wave-72 MED-4 (carried-plan label collision) — RECURS in effect** (MED-1): the heading changed, the rate
  did not.
- **Wave-72 LOW-1 (off-protocol is one shape) — RECURS exactly**, 16/16.
- **Mulligan / bottoming / cleanup discard — UNTESTED.** All six openers were untouched sevens kept at the
  first ask; no bottom ask, no cleanup discard, no Sigarda coven reveal, no Moonrage ward ask fired.

## 4 — Guide verdict: **EDIT**

Revised guide at `wave73/deck152/strategy.txt`, **19,993 B** (live 19,995; ceiling 20,000). Amendment-332
grep clean (no wave/seq/corpus/game citation, no counts, no history); no line quotes the reply's own plan
label.

**Edit 1 — the tap bill must be SEARCHED FOR, and the bracket that gets read instead is named.** Paid by
`...vs-deck130` seq 20 (MED-2).
before: `... Believe that clause over any other. COUNT THE NAMES: two or more in Upkeep or Main 1 is a pass,
whatever the row is; ...`
after: `... Believe that clause over any other. SEARCH EVERY CAST ROW FOR THE WORDS "paying this taps" BEFORE
YOU TAKE IT: the source count beside them ("leaves N of your M untapped mana sources untapped") counts lands
and creatures alike and never names a body, so reading it instead of the bill is how a whole attack goes
missing. COUNT THE NAMES IN THE BILL: two or more in Upkeep or Main 1 is a pass, whatever the row is; ...`
Why this shape: the rule was already there and already correct — what failed was *finding the clause*. The
reasoning trace quotes the adjacent bracket twice, so the fix has to name the decoy and say what distinguishes
it (it never names a body). "COUNT THE NAMES" became "COUNT THE NAMES IN THE BILL" so the counting step
cannot be aimed at the wrong bracket.

**Edit 2 — Teferi's +1 cascade is described as the render actually renders it.** Paid by `...vs-deck130`
seq 26-29 and seq 46 (MED-3).
before: `His +1 is a three-stage cascade you cannot back out of: STAGE 1 ("choose a land" / "Decline - do
nothing") is THE ONLY DECLINE WINDOW, so enter it only when you can already name an untapped permanent of
THEIRS or a tapped one of YOURS to pick; STAGES 2 and 3 have no pass row and a "[tapped]" target is wasted.`
after: `His +1 walks three stages - land, then creature, then artifact - and EACH opens with its own
"Decline - do nothing" row, so decline the stage you have nothing to pick for. Once you enter a stage the side
pick ("choose your ..." UNTAPS yours, "choose opponent ..." TAPS theirs) and the target list have NO pass row:
enter it only when you can already name a tapped permanent of YOURS or an untapped one of THEIRS.`
Why this shape: the old sentence stated a false fact about where the pass rows are and, worse, ended with
`a "[tapped]" target is wasted` one clause after telling the model to pick a tapped permanent of its own —
which is the correct pick. The new text puts the decline where it is (every mode stage), puts the no-pass
boundary where it is (side pick and target list), and states which way each side goes so the choice is made
before the irreversible step.

**Bytes.** The guide did not grow: 19,995 -> 19,993, net -2 B. Edit 1 adds ~171 and edit 2 ~55, fully
reclaimed from six places, all of them untested-for-three-waves detail or restatement, none a rule: the attack section's closing restatement of the unblockable-only
rule (the two sentences before it already say it); the Lair PASS clause's explanation of *why* the animation
expires (the rule survives, the derivation goes); the cleanup-discard walk-through (no cleanup discard window
has fired in three waves — the ordering rule stays); the mulligan Pathway line's trailing reassurance; the
counterspell bullet's "and a second the same turn if the rows allow"; and the converter bullet's both-names
branch compressed to its two load-bearing facts (no both-names board has appeared in three waves). **The rule
that earned the growth is edit 1's search step** — it is the only rule in this guide whose failure this wave
cost a whole turn's attack, and it costs bytes precisely because it has to name the decoy to work.

## 5 — Proposals

**None.** HIGH-1 is an engine defect with no instruction-surface half — a guide cannot answer a window it is
never shown, and no wording change reaches it. MED-1 and LOW-1 are meter semantics, which the engine seat
owns. MED-2's transferable lesson (when two adjacent brackets price the same currency and only one names the
consequence, the model reads the first) is a *render* rule, not a general-guide or skill rule;
`wave68/strategy-writing-skill-v2.md` already carries the decision-time-only and primitive-verification rules
this review leaned on, and a proposal restating either would be noise.

## 6 — What I did NOT check

- Any deck's seat but 152's, except the opponent seats read for context, and no `.stderr` beyond the six
  games of this seat; I did not count the `{X}`-announcement payment loss.
- `own_turn_windows_skipped` (219 across the six games) beyond confirming from `AIPlayerBaka.cpp:4037` that it
  counts own-turn priority windows with nothing castable — **it does not count the missing attackers windows
  in HIGH-1**, which are unmetered by any counter I could find. The engine seat owns that instrument.
- I did NOT read the code path between `MTG_PHASE_COMBATATTACKERS` and `AIPlayerGPT::chooseAttackers` far
  enough to name the caller that goes missing in HIGH-1; I established only that
  `GameObserver::pendingCombatDecision` was never reached on those three turns (no `[combattrace]` of any
  reason), which excludes all four of its own gates and `hasLegalAttacker`.
- `blocker_forecast_multi` / `_gang` / `_collapsed` (14/4/0 in vs-deck126, 4/4/4 in vs-deck123): I read the
  two block windows that decided games and did not audit every collapsed forecast against the board.
- Card primitives: I verified Bloodline Keeper and Intruder Alarm this wave. Teferi, Kaya's ghostform and
  Lair of the Hydra were verified in wave 72 and I did not re-verify them or the rest of the deck.
- The MULLIGAN, BOTTOMING, CLEANUP DISCARD, Sigarda-coven and Moonrage-ward sections carry no evidence this
  wave (zero windows each) and were left unchanged apart from the byte reclaims named in §4.
