# wave-58 deck146 review (Orzhov Dungeons)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-015059/`, six deck146 seats
(`1788504664/670/673/677/689/703-ai_baka_deck146-0x*`). Guide read: live
`projects/mtg/bin/Res/ai/baka/deck146_strategy.txt` (70,101 B = 68.5 KB, inside the 41-71 KB band)
+ `wave57/general-strategy.md` + the reply protocol rendered in each prompt.

## 1. Game by game

deck146 went **6-0** (results.tsv rows: 146v162 18/-4 t10; 146v152 21/-5 t16; 146v123 22/-5 t16;
146v130 19/-2 t18; 146v125 17/-4 t30; 146v126 21/0 t30). All six seats **kept the untouched
opening seven** (seq 1 in every file) — no mulligan, no bottoming ask fired in this deck all wave.
367 recorded decisions, 3h23m of model wall clock.

- **vs deck162 (win t11, 18/-4).** Curve-out: Adventurer t3 (seq 4), Nadaar t5 (seq 6), Lost Mine
  chosen (seq 7), two Soul Shatters clearing Fog Bank and Shield Sphere (seq 10, 14). Deciding
  decision: seq 14 Soul Shatter with `{right now: they sacrifice Shield Sphere (MV 0, their
  highest)}` — the only blocker went, and seq 15's attack was unblockable for 8. **The last six
  decisions of this game were NOT the model's**: seq 22, 24, 26, 28, 30, 32 are all
  `fallback=empty_reply`, latency 2504-2534 ms, `reply` empty — including seq 28, the **attackers
  declaration that dealt lethal**. The heuristic won the game. See HIGH-1.
- **vs deck152 (win t17, 21/-5).** Adventurer t3, two Soul Shatters, Lolth -3 for Spiders
  (seq 21), Kaya -3 exiling Brutal Cathar (seq 27). Deciding decision: seq 27 — Brutal Cathar was
  the only card that could stop the Spider/Goblin swarm; from there the seat attacked every turn
  unopposed. One `empty_reply` (seq 23) cost a priority window with no consequence.
- **vs deck123 (win t17, 22/-5).** Two Silverquill Silencers naming Intruder Alarm (seq 6, 9)
  pre-empted the opponent's engine; Pelakka Predation (seq 13) stripped a card; Lolth -3 (seq 25)
  produced the clock. Deciding decision: seq 25. Clean game, zero fallbacks.
- **vs deck130 (win t19, 19/-2).** Opponent Stone Rain'd/Lay Waste'd three of the seat's lands
  (turns 6, 8, 10). Deciding decision: seq 27/35/39/41/42 — Acererak recast five times as a
  repeatable venture engine off a wrecked mana base, exactly the guide's line 499 reading of him.
  **Misplay: seq 36** (see MED-1 / §3).
- **vs deck125 (win t31, 17/-4).** Longest game; the seat fell to 20-27 down by t19 with an empty
  board and three Vanishing Verses facing no creatures. Deciding decisions: seq 64 Emeria's Call
  cast as the SPELL (two 4/4 fliers) rather than as a land, seq 67 Lolth, and seq 102 Vanishing
  Verse exiling **Lightmine Field** (seq 103) — that enchantment was the only thing punishing the
  alpha strike, and seq 107/115 then killed from 20 to -4 in two swings. Costs: one wall_miss
  (seq 36, 900,018 ms) and one `empty_reply` (seq 30).
- **vs deck126 (win t31, 21/0).** Attrition. Three Vanishing Verses on Wall of Omens x2 and
  Lightmine-class permanents, Kaya -3 three times (seq 29, 39, 51). Deciding decision: seq 51 —
  Kaya exiled **Exquisite Blood** while the opponent already held Sanguine Bond in hand; the seat
  then named Sanguine Bond with two Silencers (seq 57, 67), and the Bond/Blood infinite loop never
  assembled. That is the best single read in deck146's whole wave. Costs: `empty_reply` seq 24 and
  a wall_miss (seq 26, 900,022 ms).

## 2. Engine / interface / card items

### HIGH-1 — `empty_reply` arrives in unretried RUNS and silently hands whole turns to the heuristic
Repro: `1788504670-ai_baka_deck146-0x5654d30b6220-vs-ai_baka_deck162.jsonl`, seq **22, 24, 26, 28,
30, 32** — six consecutive records, every one `"fallback":"empty_reply"`, `"reply":""`,
`latency_ms` 2504/2509/2504/2504/2534/2529. Rendered line at seq 28:
`A1. Triumphant Adventurer (2/2) [first strike, deathtouch] [no creature they control can block this attacker]`
— answered `-1 | <refused: empty_reply>`.
Facts: (a) 2.50-2.56 s is not an inference; it is a transport failure with a fixed cost, and it
recurs in **89 records corpus-wide**, always in that same 2.50-2.56 s band, usually in runs
(deck130's seat vs126 has nine in a row at t15; deck152's seat vs162 has ten at t13). (b)
`noAnswerClassFor(..., long httpStatus)` (AIPlayerGPT.cpp ~10328) already distinguishes
`http_error`, but **no empty_reply record in the corpus carries an http status, a transport error
string, or a retry count** — I enumerated the union of keys over all 89: it is
`{answer_replaced, choice, chosen_text, coded_answers, commit_retracted, fallback, kind,
latency_ms, log_window, log_window_kind, model, my_life, opp_life, options, options_text, phase,
post_answer_overrun, post_plan_overrun, prompt, reply, seq, turn}` and nothing else. So a seat
reviewer cannot tell "endpoint refused the connection" from "server returned 4xx" from "model sent
nothing", and neither can the owner. (c) There is **no re-ask**: one empty reply = one decision
given to the built-in AI, and a run of six = a whole turn.
Ask: stamp the transport outcome on the record (http status, curl/errno, whether a retry was
attempted) and make a fast empty reply (< ~5 s, zero bytes) re-ask at least once before the
heuristic takes the seat. This deck's cleanest win was not the model's.
*Not checked:* I did not instrument the endpoint; the 2.5 s constant is inferred from the latency
band, not read out of the code.

### HIGH-2 — the MDFC land menu annotates the BACK face and leaves the FRONT face as bare "Play Land"
Repro: `1788504703-ai_baka_deck146-0x55a66a56e390-vs-ai_baka_deck123.jsonl` seq **3**, turn 1.
Rendered menu, verbatim:
```
Choose an option for Brightclimb Pathway:
1. Grimclimb Pathway [PLAY THIS AS A LAND: puts "Grimclimb Pathway" onto the battlefield as a land (taps for {B}). It costs no mana and uses no stack, and it USES YOUR LAND DROP for this turn.]
2. Play Land
3. Decline - do nothing
```
Row 2 IS the front face (Brightclimb Pathway, taps for `{W}`) and it carries no name, no colour, no
"uses your land drop" note — while its sibling row carries the full wave-57 treatment. The seat
answered `CHOICE: 2 (Play Land)` with a hand of `{b}{w}` spells; it needed `{W}` and got it, but
nothing on the menu told it which face row 2 was. Compare the Emeria's Call row in
`...vs-ai_baka_deck162` seq 20, which is fully annotated on both halves.
Scale: **32 menus corpus-wide contain a bare `Play Land` row, and it was taken 13 times** (deck146:
7 renders, 2 taken — also `...vs-ai_baka_deck125` seq 12). Rows 1 and 2 of the same menu should be
rendered by the same emitter.

### MED-1 — the venture-dungeon ask never names the venture SOURCE or its dungeon-keyed clause
Repro: `1788504689-ai_baka_deck146-0x5595223952f0-vs-ai_baka_deck130.jsonl` seq **36**, turn 11.
The header reads `VENTURE - CHOOSE A DUNGEON to enter (you are picking WHICH dungeon to venture
into, NOT targeting a permanent).` The venture was triggered by **Acererak the Archlich**, whose
own bounce clause is keyed to one specific dungeon by name. Nothing in that prompt says so: the
hand line prints `Acererak the Archlich {2}{b} (5/5) [creature]` with **no card text**, the log
prints `Your Acererak the Archlich was returned to your hand from the battlefield` with no cause,
and the ask never names what is venturing. The seat picked **Dungeon of the Mad Mage (9 rooms)**,
which guarantees Acererak bounces on every future cast.
Card fact verified both ways — primitive `bin/Res/sets/primitives/borderline.txt:441-445`
(`auto=if type(Tomb of Annihilation[dungeoncompleted]|myzones)~equalto~0 then ... name(Return to hand) moveTo(hand)`,
`text=When Acererak the Archlich enters, if you have not completed the dungeon Tomb of
Annihilation, return Acererak the Archlich to its owner's hand and venture into the dungeon.`) and
Scryfall (`{2}{B}`, 5/5, same clause). **They agree.**
Ask: the dungeon-choice ask should name the source permanent, and where a completion-gated clause
on that source names a dungeon (Acererak), print the gate on the matching row.

### MED-2 — `wall_miss` records carry FALSE census fields and a post-hoc timestamp
Repro: `1788504673-ai_baka_deck146-0x558a81c2d360-vs-ai_baka_deck125.jsonl` seq **36**
(and `...vs-ai_baka_deck126` seq 26). The record says `"options": 0` and omits `options_text`
entirely, while its own `prompt` renders three:
```
1. Cast Soul Shatter {2}{b} {right now: they control 0 creatures or planeswalkers - at 0 this does nothing} ...
2. Cast nothing right now
3. Hold priority for the rest of this turn: ...
```
Anything that censuses options per decision reads 0 for these. Separately the record stamps
`turn: 16 / phase: "Main phase 2"` while the prompt it holds says `Phase: Main phase 2 | It is your
turn.` on turn 15 and its `events` reads `=== Turn 16 - opponent's turn ===` — the turn/phase/life
triple describes the moment the wall was declared, not the moment the decision was posed.
Cost: 900,018 and 900,022 ms — **30 minutes of the deck's 3h23m wall clock, on two menus whose only
live rows were "nothing" and "hold"**. 6 wall_miss events corpus-wide.

### MED-3 — the same provably-dead menu is re-rendered dozens of times per game
Repro: `...vs-ai_baka_deck125` seq **35, 36, 38, 40, 41, 44, 45, 46, 47, 50, 52, 54, 55, 57, 58 …**
— **43 renders in one game** of a header + row pair that never changes:
```
NO LIVE CAST ROW ON THIS MENU: all 1 cast row below carry a verdict computed from the board that reads zero ...
1. Cast Soul Shatter {2}{b} {right now: they control 0 creatures or planeswalkers - at 0 this does nothing} ...
```
That seat spent **61 of its 115 decisions (53%)** on `Cast nothing` / `Hold priority` / `pass`
(vs130 41%, vs123 40%). The HOLD row works (`hold_windows_skipped: 62` in that game's `gameend`),
but hold is turn-scoped and any board change re-opens the window even when the *only* cast row's
verdict is unchanged and still zero. A hold keyed to "re-ask when THIS row's verdict changes"
rather than "when the board changes" would have removed most of them.
*Doctrine note:* this is not a request to remove a window — the row must stay legal and castable;
it is a request to make the re-ask trigger match the fact the header is computed from.

### LOW-1 — number agreement in the dead-menu header
Same renders as MED-3: `all 1 cast row below carry a verdict ... not one of them changes a number`.
At N=1 it should read `the 1 cast row below carries`. 43 of deck146's 43 NO-LIVE-CAST-ROW renders
were the N=1 form.

### Verified-good (things I looked for and did NOT find in this deck)
- **D2 false header:** all 43 `NO LIVE CAST ROW` renders in deck146 seats were **true** — the sole
  row was always Soul Shatter with the opponent at 0 creatures/planeswalkers. 0 false headers.
- **D45 narration case:** I scanned every `- You …/- Your …/- Opponent …` narration line in all six
  seats for lowercased script tokens (grimclimb, brightclimb, emeria, agadeem, pelakka, hive,
  acererak, nadaar, barrowin, silverquill, shineshadow, lolth, kaya). **Zero occurrences.**
- **D1 life-payment verdict:** present and correct — `...vs-ai_baka_deck162` seq 21 rendered
  `pay 3 life - Emeria, Shattered Skyclave enters UNTAPPED [usable (tap for mana / attack) this
  turn] {this payment puts you at 18}` at life 21. Non-lethal, so the lethal branch is untested here.
- **Re-asks:** zero `named_row_reask` in any deck146 seat.
- **D9 INCOMING at Main 1:** only 3 INCOMING renders in 367 decisions, all in combat phases
  (`vs125` seq 61 Combat ends, `vs152` seq 37 Blockers, `vs130` seq 22 Combat ends). deck146 never
  sat at low life facing an untapped board, so no Main-1 window arose — **UNTESTED here**, not FAIL.
  Defer to the engine seat.

## 3. Guide verdict: **KEEP as is**

Six wins in six games with the guide's own lines doing the work, and the two behaviours the guide
spends the most words on both held:

- **Dungeon selection.** 11 dungeon-choice asks across the six games; **10 chose Lost Mine of
  Phandelver by name**, which is exactly guide lines 504-511 (`**Pick LOST MINE OF PHANDELVER by
  NAME** … whenever the THREE-DUNGEON LIST APPEARS, including after you have already completed
  one`). The single deviation is `...vs-ai_baka_deck130` seq 36 (Dungeon of the Mad Mage while
  racing at opponent-life 10) — a violation of an **already-explicit** rule that also names Mad
  Mage by name at line 516. The rule is present, correct and obeyed 10/11; restating it would be
  noise, and the missing fact at that ask is an interface defect (MED-1), not a guide gap.
- **Acererak.** Lines 492-503 tell the seat Acererak is `a {2}{B} REPEATABLE VENTURE`, not a body,
  and to cast him only with spare mana. vs130 seq 27/35/39/41/42 and vs125 seq 74/87/99 are that
  line executed, including off a mana base the opponent had Stone Rained twice.
- **Removal discipline.** `Do NOT sit on removal` held: three Vanishing Verses were spent on
  Lightmine Field (`vs125` seq 102-103), Wall of Omens x2 (`vs126` seq 16-19), and Kaya's -3 fired
  every time it was up. The `vs126` seq 51 Exquisite Blood exile plus the two Sanguine Bond namings
  is the guide's `read the opponent` clause paying off.

Guide is 68.5 KB — in band. **No `strategy.txt` written.**

One internal contradiction is worth recording without an edit: the cleanup-discard rungs put
`a second planeswalker` on rung 1, above `a land in hand once your battlefield line already prints
four or more lands` on rung 2 — but rung 2's own parenthetical argues the opposite
(`a ninth land is worth less than a castable walker`). At `...vs-ai_baka_deck162` seq 18 (four
lands out, hand held both Lolth and Kaya plus a Brightclimb Pathway) the seat sent **the land**,
contradicting rung 1 and matching rung 2's parenthetical — and won. One correct resolution in one
window is not enough evidence to reorder a rung list this expensive, so it is logged here for the
next corpus rather than edited now. If a future seat sends a castable walker under while holding a
spare land, that is the seq that pays for the fix.

## 4. Proposals

None. Nothing in deck146's evidence supports a general-guide change or a
strategy-writing-skill amendment that `wave57/strategy-writing-skill.md` (amendments 1-329) does
not already carry; a proposal restating an existing amendment is noise.

## What I did NOT check
- I did not read the opponent seats' translogs in full — only the deck146 seat files plus
  `results.tsv`. Opponent misplays that handed deck146 a game are not separated out here, so
  "6-0" should not be read as "the guide beat six good pilots".
- I did not open the game stderr files beyond confirming the endpoint banner
  (`AIPlayerGPT: using http://100.116.136.74:8084 (model qwen36-35b-a3b)`); `WAGIC_BLOCKLOG=1`
  output and `async_drops` census belong to the engine seat and I left them there.
- I did not adjudicate any lane-A..E prediction (that is the engine seat's job); the
  "verified-good" list above is only what deck146's own prompts show.
- No live probe, no build, no run of the engine.
- Single corpus, six games, one model. Every count above is from these six files.
