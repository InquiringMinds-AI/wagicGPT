# wave-61 deck146 (Orzhov dungeon midrange) — per-deck review

Seat files: `matchups-20260905-092408/1788618{262,268,278,282,291,293}-ai_baka_deck146-0x...-vs-ai_baka_deck<N>.jsonl`.
Record: **3-3** (W vs 123, 125, 126; L vs 130, 152, 162). 207 decisions, **0 fallbacks, 0 re-asks, 0 stale drops** at this seat, 2.32 MB of prompt (mean 11,230 B).

## 1. Game by game, and the decisions that decided them

**W vs deck123 (t15, 22-0).** Textbook guide line: T3 Triumphant Adventurer, T5 Acererak (venture, bounce), T7 Nadaar, removal on curve (s14 Vanishing Verse on Intruder Alarm, s18 Soul Shatter), Emeria's Call played as Emeria, Shattered Skyclave (s24), Hive activated (s26), alpha at s27 for 14. No misplay found.

**W vs deck125 (t19, 23-0).** Slow start behind a lifegain deck (opp 20→27), then the dungeon engine took over: s31 Lolth (countered), s32 seven attackers whose ventures completed Lost Mine mid-combat, s35 Barrowin reanimated Nadaar, opp 18→0 in one combat. No misplay found.

**W vs deck126 (t15, 24 to -5).** Vanishing Verse spent correctly on the opponent's engine pieces (s24 Exquisite Blood, s44 Sanguine Bond) rather than bodies — this is the guide's #1 RULE working. s31 the forced sacrifice was paid with a Goblin token, not a real creature.

**L vs deck130 (t18, -3 to 19).** Not a guide loss: kept a legal 2-land seven (s1; the ship list correctly did not fire), then a red LD deck removed the 3rd and 4th land (Stone Rain t6, Molten Rain t10) and held the seat to 2-3 lands for the whole game. The seat cast 12 spells' worth of {b}{w} cards on {W}{W} for ten turns. **Deciding decision s11 (t17, 6 life, 4 attackers for 4):** it cast Vanishing Verse on a 1/1 Goblin (row 3, four 1/1 legal targets) with three Triumphant Adventurers castable; a deathtouch blocker was worth more than exiling one of four 1/1s. STRATEGY, but marginal and inside a lost position.

**L vs deck162 (t15, 0-20).** Died without dealing a point. Opponent's engine was Howling Mine + Underworld Dreams + Liliana's Caress + recurring Ob Nixilis. Vanishing Verse **cannot** touch Howling Mine (colourless artifact; "exile target *monocolored* permanent" — primitive/Scryfall agree), so the doubled draw tick was unanswerable with this deck's removal. Every body it deployed (Nadaar s15, Silverquill Silencer s19) was answered by Ob Nixilis -2 the same turn. Guide-conformant, lost to card matchup + inevitability.

**L vs deck152 (t24, -2 to 9).** Longest game, and the loss is a single decision — see HIGH-1. At s54 the seat was at 8 with 2 blockers vs 3 attackers for 13; the header printed the survivable number (`best case with every blocker assigned: you would be at 6`), the guide's SURVIVAL COMES BEFORE THE LADDER rule prescribed the line, and the model's own PLAN text derived the line — and the answer line it had already committed to sent it to -2.

## 2. Engine / interface / card items

### HIGH-1 — the reply's answer line is committed BEFORE the model reasons, and the model's own reasoning then contradicts it. This lost a game.
Repro: `...deck146-0x561c51b3e400-vs-ai_baka_deck152.jsonl` **seq 54, turn 24**, seat at 8 life.
Rendered: `INCOMING THIS COMBAT: 3 attackers, 13 unblocked damage - you would be at -5; this KILLS you (... at least 1 of them go unblocked) - best case with every blocker assigned: you would be at 6`; `B1. Goblin (1/1) - may block A1-A3`; `B2. Spider (2/1) ... A1 (your blocker dies, attacker lives)`.
Reply line 1: `BLOCKS: B2:A2, B1:none`. Reply PLAN, verbatim tail: *"...If I block A1 with B2, B2 dies. Then A2 and A3 get through for 5 damage. I survive at 3 life. This is better than dying. **So BLOCKS: B2:A1.**"* The engine took line 1 (`--CHOSEN-- Spider blocks Wolf` = A2, the 3/3), the 8/8 and the 2/2 connected for 10, and seq 55 is the gameend at -2.
This is not isolated: **8 of 207 replies at this seat (3.9%) continue deliberating past the answer line**, and 2 were truncated mid-sentence by the recorder (`[+807 bytes written past the PLAN line trimmed from this record]` at 162 s21; `[+1131 bytes ...]` at 126 s45; also 162 s17, 126 s19, 123 s21, 123 s23, 152 s25). The protocol says "Write nothing else", the model does not obey it, and at combat asks the deliberation it does anyway is where the right answer lives.
Ask: at minimum, when a reply contains a LATER well-formed directive of the same kind (`BLOCKS:` / `ATTACK:` / `CHOICE:` / `PUT:`) that differs from line 1, do not silently take line 1 — take the last one, or `[RE-ASK]`. The recovery arm `plan_choice_conflict_recovered` already exists in the ledger and has never executed; this is the window it was built for. Alternatively, put the answer line LAST for `blockers`/`attackers` asks so the commitment follows the reasoning.

### HIGH-2 — the blockers header proves a survivable assignment exists, and then refuses to name it.
Same file/seq. `AIPlayerGPT.cpp:17351-17357` computes `bestCaseDamage` from an actual matching (`bestCaseOptimal`) and prints only `you would be at 6`. The seat's PLAN then says, in full: *"I have no other blockers"* — with `B1. Goblin (1/1) - may block A1-A3` printed one line above it. The engine held `B1:A1, B2:A2` in hand and printed a number instead.
Ask: on any blockers window whose header says LETHAL and whose best case is survivable, print the assignment itself — `one legal assignment gets you to 6: B1:A1, B2:A2`. Wave-59 already ships a `one legal assignment gets you to` string for a neighbouring case; it did not render at the one window this corpus where it decided a game.

### HIGH-3 — MDFC land faces contradict the standing land-drop header, and vanish from the hand listing.
Repro: `...vs-ai_baka_deck130.jsonl` **seq 7, turn 13**. The prompt carries, verbatim, `Land drop: NOT yet used this turn ... The land drop is its OWN decision (a "Land drop:" question with its own Play options); it is **never listed in a casting, ability or target menu**, so its absence from the choices below does not mean it is gone` — and then row 1 of that same priority menu is `1. Pelakka Caverns with Pelakka Predation -> PLAY THIS AS A LAND ... it USES YOUR LAND DROP for this turn`. Same shape at `vs-deck123` s24 (Emeria's Call) and `vs-deck162` s21 (Agadeem's Awakening).
Second half: the hand line in that same prompt reads `Pelakka Predation {2}{b} [sorcery] [cannot pay now: needs 3 mana, you have 2 untapped sources]` — no land-face marker at all, on a seat stuck on 2 lands. The engine knows the fact (primitive `borderline.txt:82649` `autohand={0}:restriction{can play land} ... forcetype(land)`) and prints it in the PRE-GAME header (`vs-deck162` s1: *"2 cards are counted TWICE above, on purpose: Emeria's Call is a spell you may instead play as the land Emeria, Shattered Skyclave; Pelakka Predation ... So 4 lands + 5 spells = 9 for a 7-card hand"* — an excellent render) and then stops printing it the moment the game starts.
Ask: carry the pre-game clause into the in-game hand listing (`Pelakka Predation {2}{b} [sorcery] [or play as the land Pelakka Caverns, taps for {B}, enters tapped]`), and either exempt MDFC land faces from the "never listed in a casting menu" sentence or route them to the land-drop ask.

### MED-1 — `ATTACK TOTAL` is pre-trigger for the PILOT'S OWN attack triggers, and undercounted by 8 on a lethal turn.
Repro: `...vs-ai_baka_deck125.jsonl` **seq 32, turn 19**: `ATTACK TOTAL: 7 attackers listed, 9 total combat damage to a player - declaring all of them with none blocked puts them at 9. At least 9 damage lands whatever they block - they would be at 9.` Opponent was at 18 and finished the combat at 0 — 17 damage, not 9. The delta is the seat's own printed board: four venture-on-attack triggers completed Lost Mine mid-combat (turning on Nadaar's "other creatures get +1/+1"), plus `dark pool` (drain 1) and `goblin lair` (a token). Known-bugs #1 prices the OPPONENT's blocking/attack triggers; this is the mirror case and the same clause. It errs in the safe direction here, but it is the number the seat plans lethal from.
Ask: fold the pilot's own venture/anthem chain, or say "before your own attack triggers".

### MED-2 — the all-unblocked resulting-life clause never carries a lethality flag, so exactly-0 reads as survivable.
Repro: `...vs-ai_baka_deck123.jsonl` **seq 23, turn 13**, opponent at 6: `ATTACK TOTAL: 3 attackers listed, 6 total combat damage to a player - declaring all of them with none blocked puts them at 0. At least 4 damage lands whatever they block - they would be at 2.` The `that KILLS them` badge only ever attaches to the guaranteed-floor clause (`vs-deck126` s49, `vs-deck123` s27). At 0 the opponent is dead — the seat's own blockers header says so in the other direction (*"at 0 life you LOSE - 0 is not survival"*). A player-facing number that means "you win if they don't block" is printed with no marker at all.

### MED-3 — the Pathway land drop is asked twice, and the colour is invisible in the first ask.
Repro: `...vs-ai_baka_deck130.jsonl` **seq 8 then seq 9, both turn 15**. s8: `Land drop: play Brightclimb Pathway now? 1. Play Brightclimb Pathway / 2. Hold` — no face annotation. s9 then offers `1. Brightclimb Pathway ... (taps for {W})` vs `2. Grimclimb Pathway ... (taps for {B})`. The seat's entire hand was `{b}{w}` costs off two Plains, so {B} was the whole decision, and it was not on screen until after the seat had already answered "yes". (The seat did pick Grimclimb — but it had to spend a window to find out the choice existed.) Fold the faces into the land-drop ask.

### LOW-1 — a multi-target cast row prints ONE `{target text: ...}` as if it described the row.
`...vs-ai_baka_deck130.jsonl` seq 11, turn 17, row 3: `Cast Vanishing Verse {b}{w} ... {removes: Goblin #1, Goblin #2, Goblin #3, Dwarven Blastminer} - legal targets right now: Goblin #1-#3 (creature 1/1) x3, Dwarven Blastminer (creature 1/1) {target text: "{2}{R}, {T}: Destroy target nonbasic land. -- Morph {R} ..."}`. The `{target text}` is Dwarven Blastminer's alone; three of the four legal targets have no text and the reader cannot tell which one it belongs to.

### LOW-2 — dungeon room-count mismatch recurs (known-bugs #9).
`DUNGEON ROOM CHOICE` prints `Lost Mine of Phandelver's rooms, in printed order: 1. Cave Entrance; ... 7. Temple of Dumathoin` (`vs-deck152` s53), while the run completes at explore step 4 (`vs-deck125` t19 log: `venture step 4 of that run` → `dungeon completed`). Dungeon selection: **14 of 14 Lost Mine** across all six games — guide-directed, so this is the engine's B12 source clause failing again, not a seat item.

### RESOLVED since wave 60
Pure declines (`Cast nothing` / `Hold priority` / `Pass` / `Decline`) fell from **22.7% to 6.3%** (13 of 207), and the 10-consecutive-identical-window pattern does not recur at this seat. 0 fallbacks (wave-60 seat had several). The pre-game MDFC double-count clause is a genuinely good render (see HIGH-3).

### UNTESTED at this seat (no window arose — not a pass)
The choose-ONE reveal arity (known-bugs #8, last wave's deck146 HIGH-1): **Pelakka Predation was never cast in any of the six games** — 0 reveal asks, so C8 is untested here. Also untested at this seat: loop pair (C5), sweeper CAST-row rank (C10), `{spare: ...}` conditioning (C11), one-shot draw-grant pricing (C12), transport/deadline fields (C13).

## 3. Guide verdict: **KEEP as is**

Not a default — the guide's two most expensive sections were exercised and both held.

- **Mulligan: 6 of 6 kept the opening seven, 0 mulligans, 0 bottoming asks.** Two of those hands printed the exact sentence prior corpora shipped on — `vs-deck123` s1 `2 lands ... would not cover any spell in it (2 lands are not enough for your cheapest spell at mana value 3)` and `vs-deck130` s1 `2 lands ... no spell in it is castable off {W} alone even with every land in this hand in play` — and STEP 2's "that coverage line is a ONE-LAND test and nothing else" plus "colour is never a reason to ship" correctly kept both. The seat has stopped paying cards for land odds.
- **Cast order rung 1 (removal on a live target) fired every time it was offered**, and the #1 RULE's "Verse takes their engine, not their body" produced the two wins vs 126 (Exquisite Blood s24, Sanguine Bond s44).
- **The Acererak-bounce rule held**: cast 4× as a repeatable venture, never counted as a body, never planned into combat.
- **The three losses are not guide gaps.** 130 = land destruction against a legally-kept hand. 162 = Howling Mine is colourless and Vanishing Verse cannot target it; the guide cannot fix a card the deck does not have. 152 = the guide's own `SURVIVAL COMES BEFORE THE LADDER` rule was the right answer, and the model *wrote that answer out in prose* one line after committing to the wrong one — a protocol defect (HIGH-1), not a missing instruction. Adding guide text for a rule the model already applied would be noise.
- Size: the live guide is **70,343 B**, i.e. 99% of the 41-71 KB pool band. There is no room for an edit that is not paid for, and nothing here is paid for.

## 4. Proposals

None. No general-strategy change and no strategy-writing-skill amendment is supported by this seat's evidence: the guide-level behaviour was conformant in all six games, and the one game-losing defect is an engine/protocol item (HIGH-1), not a guidance one.

## 5. What I did NOT check

- Only the deck146 seat's six translogs were read in full; the opponents' seat files were consulted only through this seat's rendered GAME LOG, so opponent-side render defects are outside this review.
- I did not adjudicate any lane R/S/T/U/V prediction (engine-seat's job) and did not read `lane-*.md` beyond `known-bugs.md`/`codex-review.md` for overlap.
- I did not verify the `matchedAttackers`/`bestCaseDamage` matching logic beyond reading `AIPlayerGPT.cpp:17320-17370`; no build, no run, no test.
- Card facts checked against the primitives for Pelakka Predation, Triumphant Adventurer, Vanishing Verse, Acererak, Nadaar, Soul Shatter only; the opponents' cards were taken from the rendered `{card text:}` blobs.
- MED-1's 8-point delta was reconstructed from the turn-19 GAME LOG, not from a per-trigger damage trace.
- No claim about whether HIGH-1's 3.9% deliberation rate is model-version-stable; one corpus, one seat.
