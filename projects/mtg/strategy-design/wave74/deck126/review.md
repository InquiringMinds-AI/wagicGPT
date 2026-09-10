# Wave-74 deck126 review — SANGUINE BLOOD (WGB wall prison, Exquisite Blood + Sanguine Bond)

Corpus `matchups-20260909-104713-final`, **MIXED BINARY**: five of this seat's games ran on
`wagic-60e9fe2ad-w74step1b` (through CG); the **125v126 game is the CH-binary rerun**
(`wagic-76c5fab24-w74step1c`, start_epoch 1788998153, hold key = action identity). Every claim below
about hold behaviour is split by binary and says which.

Record **2/6** (wave 73: 2/6). **308 records / 288 model prompts** at this seat: 2 fallbacks
(1 `http_error`, 1 `unparsed_reply` — both quoted in §2), 1 `plan_line_missing` (the http_error's
empty reply — the counter is inflated by fallbacks), 0 `action_before_plan`, 0 `reply_truncated`,
0 `off_protocol_bytes`, 0 `ask_replays_refused`, 0 `menu_pass_no_progress`, 0 `wall_miss_events`,
0 `declined_face_latches`, 0 `plan_names_stranded_card`, 0 `repeat_annotated_takes`,
0 `stop_reached_windows_skipped`, 0 `sibling_window_asks_skipped`, 0 `chain_windows_collapsed`,
0 `main_phase_windows_skipped` (all six games). `thinking: on` and non-empty `reasoning` on every
record (invariant 000 satisfied). Per game (162/130/146/152/123/125): decisions 31/23/23/81/21/129;
`hold_windows_skipped` 0/18/0/28/0/683 (cast 0/18/0/0/0/683, priority 0/0/0/28/0/0);
`hold_released_turn` 0/1/0/1/0/21; `ask_replays_reserved` 0/1/0/29/0/30, refused 0;
`identical_ask_answers_reserved` **0 in all six**; `async_drops` 0/0/0/1/1/0;
`phase2_answer_recovered` 1/1/1/1/0/0, `missing` 0/0/0/1/0/0; `own_turn_windows_skipped` 0/0/13/0/0/0;
`mana_only_windows_skipped` 34/80/0/22/0/0; `chain_acting_rows` 410 / `chain_selfharm_rows` 354 (152
only, the rest 0); `chain_windows_only_selfharm` 0 everywhere; `blocker_forecast_rows` 24 / `multi`
12 / `collapsed` 4 (152 only).

**Max asks in any one (turn, phase) at this seat: 6.** No window loop, on either binary. The
`[you declined this exact list N times]` clause reached **N=4** once (wave 73: ≤3); the tagged
decline row was taken in **13 of 64** tagged windows (20%) — down from the 40% the brief cites for
wave 73, so O11's tag is moving the number in the intended direction at this seat.

## 1. Game by game

| # | seat file (epoch) | opp | result | deciding decisions |
|---|---|---|---|---|
| 1 | 1788968848 | 162 | **LOST** 0–18, t19 | Tutor t5 → Sanguine Bond (rung 4, correct with neither half held). Bond landed t15 (seq22). Blood tutored t17 (seq25) and **never cast: the engine told the seat it could not pay for it, falsely, in four separate windows** (§2 HIGH-1; the decisive pair is seq22 and seq26). Death was the seat's own compulsory draw under Dictate of Kruphix + Teferi's Puzzle Box + 2 punishers: at seq23 the DRAW FORECAST read `draws 10 cards (1 + Dictate of Kruphix 1 + Teferi's Puzzle Box: your hand size 8) = 10 x 2 = 20 life LOST BY YOU ... that KILLS you` at 9 life. This is the corpus's only hand-size-driven forecast and it disproves a sentence the wave-73 guide edit put in (§3-A). One guide violation on the way: **seq17** cast a Tribute on a `[defender]` victim (Shield Sphere) at 20 life, which the guide's HOLD IT rule forbids; it gained 6 and did not decide the game. |
| 2 | 1788968859 | 130 | **LOST** 0–20, t20 | Curve-out board (Captain t3, Battlement t5) into Siege-Gang Commander + Rorix Bladewing. Two textbook Perimeter-Captain blocks (seq15 t14, seq19 t18, both `Gain 2 life - accept ALL 2 triggers`). Never drew a second black source: at seq21 (t19, life 2) the seat's own reasoning reads *"I have no {B} mana and cannot cast Sanguine Bond or Tribute"* — the hand-line "cannot pay" tags in that game are colour-correct. Mana screw, not a decision loss. The one unblocked attack (stderr t15, `declared attack (1 of 4)`) was a flier the seat's 0/4 walls could not legally block. |
| 3 | 1788968861 | 146 | **LOST** −6 to 14, t15 | Bond t10 (seq15), Blood never found. Every wall the seat cast was removed; from seq16 (t12) on its battlefield had **0 creatures** for the rest of the game, which is why no blockers window ever opened (checked: not a defect). seq20 (t14, life 6) took the Tribute at `they control 4 creatures - they choose which one` under a lethal crack-back — rung 0, correct. Removal game. |
| 4 | 1788968865 | 152 | **WON** 25–0, t26 | The wave-73 ATTACKING rewrite converted. Sorin t12, Staff t14, second Staff t18, four Perimeter-Captain/Pride-Guardian gang blocks (seq31 `accept ALL 8 triggers` = +16 life, seq41 the same). Bond cast t24 (seq61). **seq106 (t26, them at 1) is the win**: the only Vampire's A-line listed 5 untapped blockers, EVERY verdict `your attacker dies, their blocker lives`, and the ATTACK TOTAL read `1 of that damage is LIFELINK, and your LIFE-TO-DAMAGE CONVERTER ... so with none of them blocked they are at -1, not 0. That IS lethal`. The seat sent it, the Vampire was blocked, lifelink gained 1, the Bond drained 1, opponent 1 → 0. That is exactly the guide's `SEND IT ANYWAY` + `THE WIN BUTTON OUTRANKS EVERY STOP` pair, on a board where the pre-wave-73 text would have benched the attacker. Two engine defects live in this game: §2 HIGH-2 (Sorin) and the seq48 `http_error` that cost a 3-Vampire lifelink swing at opponent life 9. |
| 5 | 1788968875 | 123 | **LOST** −86 to 18, t12 | Mulligan correct (1 land / 6 spells shipped at seven, kept a 4-land six, bottomed the duplicate Scrubland — guide letter on all three). Bond t9 (seq15). Killed on t11 by Intruder Alarm + Bloodline Keeper: `declared attack (26 of 26)`, all 26 tokens **flying**, and the seat's three 0/4 defenders could not legally block any of them — I checked for a missing BLOCKERS window and there is none to report (zero `Defenser Toggle` lines in the game is consistent with zero legal blocks, not with a suppressed seam). Nothing in the seat's play is at fault; 21 decisions, 0 fallbacks. |
| 6 | 1788998153 | 125 | **WON** 48–0, t52 (CH binary) | 129 decisions, 683 held windows. Blood t20 (seq29), Bond t24 (seq61) off the Tutor — **LOOP COMPLETE from t24**, then **28 turns with no starter**: the opponent (Supreme Verdict / Path to Exile / Dream Fracture control) held **0 creatures** for the whole stretch, so every Tribute row printed `at 0 this does nothing` and the seat declined it at **every one of ~60 windows** while its life fell 20 → 7 under the opponent's Staff of Nin. THE ZERO STOP held perfectly. The seat did not win this game: at their seq267 the **opponent pinged ITSELF** (`Deal 1 damage with Staff of Nin targeting you {right now: takes 1 damage - you would be at 41}`), which fired the seat's Exquisite Blood and ran the pair to 0. Credit is the opponent's blunder, not this seat's play. |

**2/6 read.** Two of the four losses (146, 123) are decks doing their thing to an empty board;
130 is a colour screw the seat correctly recognised; **162 is the one loss where a false engine
surface, not a decision, removed the win condition from the menu** (HIGH-1). The two wins are one
correct conversion (152) and one gift (125). Play quality is not down: land drop 34/34, reveal picks
4/4 correct, ZERO STOP ~60/60, the win button converted in the first live window in the game that
had one, and one guide violation (seq17) plus one hold-over-a-live-loyalty-row (seq63) in 288 windows.

## 2. Engine / interface / card items

**HIGH-1 — the hand-line `cannot pay now` gate and the cast menu FALSELY suppress Exquisite Blood,
the deck's win-condition half, while Overgrown Battlement is untapped.** Repro
`1788968848-ai_baka_deck126-0x55f8f4a04460-vs-ai_baka_deck162.jsonl` **seq 22** (t15, life 27). One
prompt, both statements:
- menu row 1: `Cast Sanguine Bond {3}{b}{b} {leaves 0 of your 4 untapped mana sources untapped - casting this taps you out; paying this taps: Overgrown Battlement, Overgrown Battlement - they cannot block on thei...}`
- hand line: `Exquisite Blood {4}{b} [enchantment] [cannot pay now: needs 5 mana, you have 4 untapped sources]`

Both cost **five mana**. Sanguine Bond needs **two** black pips, Exquisite Blood **one** — Blood is
strictly the easier of the two to pay, and the engine itself proves the mana exists by offering and
resolving the Bond off the same four sources (Chromatic Lantern, Isolated Chapel, Overgrown
Battlement #1, Overgrown Battlement #2; two defenders on the board, so each Battlement adds {G}{G}
= 6 mana available). **Exquisite Blood has no Cast row on that menu at all.** The same seat, same
game: **seq 26** (t17, life 9, the last window that could have completed the pair) prints
`Exquisite Blood {4}{b} [cannot pay now: needs {4}{b}, your 5 untapped sources cannot pay it]` with
Lantern + Chapel + Sunpetal Grove (all any-colour) + two Battlements (2 green each) untapped = 7
mana; and **seq 17** (t13) and **seq 21** (t15) repeat it. Sixteen of this seat's `cannot pay` tags
sit on a board where the Battlement multiplier covers the cost. **Mechanism (hypothesis, for the
engine seat, not established):** the gate compares the cost to a SOURCE COUNT (its own wording is
"needs 5 mana, you have 4 untapped sources") rather than to the planner, so a variable-output source
counts as one mana — while the row that DID appear (the Bond) has two coloured pips that force the
planner down the variable path. Two ways to falsify cheaply: (a) a fixture with one Battlement, two
defenders, and both halves in hand — `assertcastable 1 Exquisite Blood`; (b) whether the hand flag
and `legalCasts` come from the same predicate. **Why it is the worst class of defect for this deck:**
the model is instructed to believe the surface, the guide tells it a card with no Cast row is not
castable, and the surface deleted half the win condition at the window that decided a game.

**HIGH-2 — Sorin, Lord of Innistrad: the −6 charges 3 loyalty, does nothing, and does not stop a
second loyalty ability the same turn.** Repro
`1788968865-ai_baka_deck126-0x558242cd6390-vs-ai_baka_deck152.jsonl`, turn 18, three consecutive
records. At **seq 38** the board reads `Sorin, Lord of Innistrad ... [counters: 6x loyalty]` and the
seat takes row 3, `-6: destroy up to three and reanimate with Sorin, Lord of Innistrad targeting
Sigarda, Champion of Light [opponent's battlefield] [cost: Counters]`. At **seq 39** (same turn):
- Sorin reads `[counters: 3x loyalty]` — a −6 removed **three** counters, not six;
- the opponent's board is byte-identical to seq38's, `Sigarda, Champion of Light (4/4) [trample, flying]` still on **their** battlefield — nothing was destroyed and nothing was reanimated onto ours;
- the menu offers `-2: emblem` **again**, the seat takes it, and at **seq 40** Sorin reads `[counters: 1x loyalty]` (3−2=1, so the −2 is charged correctly).

So one planeswalker activated **two** loyalty abilities in one turn (CR 606.3 allows one), one of
them for the wrong cost and with no effect. The card script is faithful to Oracle
(`planeswalkers.txt:3059`: `auto={C(0/0,-6,Loyalty)}:name(-6: Destroy up to three and reanimate)
target(<upto:3>*[creature;planeswalker]|battlefield) destroy and!(moveto(mybattlefield))!`;
`counter(0/0,3,loyalty)` starting loyalty 3 is correct), so this is engine-side — the `<upto:3>`
targeting and the loyalty cost payment are the two places to look. Also confirmed against the
primitives: `Sanguine Bond` (`mtg.txt:99004`) and `Exquisite Blood` (`mtg.txt:37966`) are faithful.

**MED-3 — the reply parser rejects a well-formed answer whose `CHOICE:` shares the PLAN's line.**
Repro `1788998153-ai_baka_deck126-...-vs-ai_baka_deck125.jsonl` **seq 154** (t52), `fallback:
unparsed_reply`, the whole reply verbatim:
`PLAN: Play a land to expand mana base and prepare to cast Chromatic Lantern. CHOICE: 1 (Play Sunpetal Grove)`
Nothing here is prose and nothing is ambiguous: one PLAN, one `CHOICE: N (name)`, no second answer.
This is 1 of the seat's 2 fallbacks and the only model-side one. I am NOT proposing parser tolerance
for prose (invariant 000); the narrow question for the engine seat is whether an anchored
`CHOICE: <int> (<name>)` occurring after a PLAN on the same physical line should be accepted, or
whether the protocol text should say "on a line of its own" more loudly. I took the guide half of
that pair this wave (§3-C) so the engine seat can decide the parser half independently.

**MED-4 — the other fallback was infrastructure and it cost a real swing.** Same 152 game, **seq 48**
(t20, life 53 / them 9), kind `attackers`, `fallback: http_error`, empty reply, `choice -1`. Three
1/1 lifelink Vampires were listed with Sanguine Bond on the board; the heuristic answered and the
opponent's life is unchanged at seq 50, so the seat lost a swing worth 3 combat + 3 converted = 6
against a total of 9. Recording it because `plan_line_missing` is credited to this record too: at
this seat 1 of 1 `plan_line_missing` is a transport failure, not a protocol failure, and the
corpus-wide 93 should be split the same way before it is read as a protocol regression.

**LOW-5 — `main_phase_windows_skipped` is 0 in all six games at this seat, including the 683-hold
game.** Wave 73 reported 58 in the equivalent long game and I flagged it as double-counting the hold
latch. O10's rewiring (exclude hold-suppressed phases) has taken it to zero here — which is the
honest figure for a seat that holds rather than skips, and is evidence FOR the rewiring rather than
against it. Same shape: `stop_reached_windows_skipped` is 0 in all six because no PLAN at this seat
ever wrote a countable stop — this seat cannot distinguish "the stop no longer parses" from "there
was no stop to parse", and the engine seat should not read this seat's zero either way.

**LOW-6 — `identical_ask_answers_reserved` is 0 in all six games while `ask_replays_reserved` is 60.**
Both counters are live in the gameend record and the replay path is clearly working (60 reserves,
0 refusals, and the 125 game held 683 windows at zero model cost). Whatever the second counter
names, it never fired once at this seat — the engine seat has the code and should say whether 0 is
its correct value or a dead increment.

**Wave-73 items, re-checked on this corpus.**
- **HIGH-1 (ATTACK TOTAL ignoring the seat's own Sanguine Bond) — FIXED, verified.** Every attackers window with the Bond out now prints the converter arithmetic, and it is arithmetically right both ways: seq 64 `declaring all of them with none blocked puts them at 4. 1 of that damage is LIFELINK, and your LIFE-TO-DAMAGE CONVERTER turns the life it gains you into that much life off them as well, so with none of them blocked they are at 3, not 4. That is NOT lethal`; seq 106 the same clause resolving to `they are at -1, not 0. That IS lethal`. The seq-106 verdict is what the seat acted on to win the 152 game. 2 of 2 converter windows correct, 0 false.
- **MED-2 (loyalty rows without consequence clauses) — FIXED, verified.** All 11 Sorin loyalty menus at this seat carry them: `+1 ... {right now: the body this makes arrives summoning sick - it cannot attack until your next turn}` and `-2 ... {right now: you already control 1 emblem - a new emblem is ADDED to them, it does not replace them, and no emblem can be removed}` (and, before the first emblem, `{right now: an emblem is permanent - nothing in the game removes...}`). **One gap:** the `-6` row (seq 37, seq 38) carries **no** `{right now: ...}` clause at all while its two siblings do — and it is the row that misbehaves (HIGH-2).
- **MED-3 (main_phase_windows_skipped double-counting) — see LOW-5, resolved at this seat.**
- **LOW-4 (hold taken on a menu printing a live activated row) — RECURS, third wave.** `1788968865-...-vs-ai_baka_deck152.jsonl` **seq 63** (t24, Sanguine Bond just resolved, them at 5): rows 1 `+1: create a 1/1 vampire`, 2 `-2: emblem`, 3 `Add 7 green mana with Overgrown Battlement`, 4 `Hold priority`. Reply: `PLAN: Attack with Vampire token in combat ... Pass priority now to proceed to combat. / CHOICE: 4 (Hold priority)` — a Sorin activation thrown away to a plan that deferred. Rate at this seat: 1 of 3 hold-windows that printed any activated row. The other two (seq 65, seq 66) printed **only** `Add 7 green mana with Overgrown Battlement`, where holding is right and the guide's blanket wording was wrong — §3-B fixes that half.
- **O16 `(keeping 7)` — PRESENT and correct**, in the pre-game prose rather than on a row: `Pre-game mulligan decision (London mulligan). You have a fresh 7-card opening hand (keeping 7).` (162/130/146/152/125 seq2, 123 seq3 carries the mulligan-taken form). 7/7 windows.
- **O9 own-clock tag — 38 exposures at this seat, 38 arithmetically correct** (ceil(their life / rate), and the singular `in 1 more turn` at ≤2). E.g. seq 43 `2 damage a turn ... reaches 0 in 6 more turns` at them-11, seq 69 `... in 1 more turn` at them-1.
- **O1 ghostform gloss, O19 ANNOUNCE_X, O4 MDFC land menu, O13 chain instruments at the CAST seam — UNTESTED here:** no ghostform, no X spell, no MDFC and no priority-seam chain occurred at this seat.

**THE CH-BINARY CHAIN — the hold HELD, completely.** In the 125v126 rerun the pair went off on the
seat's turn 52 and ran to lethal. The stderr shows the whole chain as
`Action added to stack: StackAbility. (Source: Sanguine Bond)` / `Resolving ... (Source: Exquisite
Blood)` alternating for **84 consecutive links**, each one followed by
`AIPlayerGPT[ai_baka_deck126]: holding priority at the cast seam (the model's own hold row, turn 52;
N windows held this game)` with N running 600 → 683. **Asks at this seat during turn 52: 5 total**
(seq 153–157), all of them BEFORE the chain started, and **zero during the chain** — the hold key on
action identity survived every life-total change across every link. Compare the CG binary: the 152
game's 28 priority holds and the 130 game's 18 cast holds are all short-lived, and neither binary
produced a phase with more than 6 asks at this seat. `hold_released_turn` = 21 in the CH game and 1
each in the 152/130 games; I traced the release lines and each names its own turn and the next
(`the hold is RELEASED at this seat's untap (taken on turn 50 (their turn), now turn 51; 21 released
this game)`) — **no hold outlived the holder's untap** in any of the six.

## 3. Guide verdict: **EDIT** (`strategy.txt`, 19,996 B — down 1 B from the live guide, ceiling 20,000)

Three changes; four cuts pay for them and the guide does not grow. HIGH-1 gets **no** guide text: the
trust doctrine forbids teaching doubt about a rendered value, and the guide's existing "A hand card
with NO 'Cast' row is not castable this window - never name one" is the correct behaviour given a
false surface. That defect is the engine's to fix, not the guide's to hedge.

**A. DRAW PUNISHERS states a mechanic this corpus disproves — the wave-73 correction over-corrected.
Paid by `...vs-ai_baka_deck162.jsonl` seq 23, seq 26 and seq 29 (t17–t19).** Wave 73 deleted "hand
size is the multiplier" as false, on a corpus whose forecasts read `1 + Howling Mine 1) = 2 x 3`.
This corpus's forecast reads `your next draw step draws 10 cards (1 + Dictate of Kruphix 1 +
Teferi's Puzzle Box: your hand size 8) = 10 x 2 = 20 life LOST BY YOU` — the hand size **is** a term
in the multiplier when a Puzzle-Box-shaped punisher is out, and each card cast off the hand removes
one draw and its punisher hits. The live guide's flat "not hand size ... nothing you cast or decline
slows it" is therefore a false teach in front of one of the pool's decks. (It would not have saved
this game: at seq 26 the seat needed hand ≤ 3 from 7 with one land drop. The rule is still wrong.)
before: `"DRAW FORECAST: ... = N life" meters CARDS DRAWN times punishers - not hand size - and your draw step is COMPULSORY, so nothing you cast or decline slows it.`
after: `"DRAW FORECAST: ... = N life" meters CARDS DRAWN times punishers and your draw step is COMPULSORY. READ ITS ARITHMETIC FOR A HAND-SIZE TERM: while it names one ("your hand size N"), each card you cast off your hand cuts the next forecast by that many hits and emptying your hand is your only lever; while it names none, nothing you cast or decline slows it.`

**B. THE HOLD ROW's "never hold on a menu that prints an activated row" mis-fires on mana rows and
under-fires on loyalty rows. Paid by `...vs-ai_baka_deck152.jsonl` seq 63 (violation) and seq 65 /
seq 66 (the rule pointing the wrong way).** At seq 63 the menu printed Sorin's `+1` and the seat
held; at seq 65/66 the only activated row was `Add 7 green mana with Overgrown Battlement`, which is
never an answer, and the guide's blanket wording forbade the correct hold. Naming the distinction
makes the loyalty case sharper, not just narrower.
before: `A LOYALTY ROW OF SORIN'S, A STAFF PING ROW, OR ANY OTHER ACTIVATED ROW IS A LIVE ROW, and the hold row's "you give up no cast" is not about them`
after: `A LOYALTY ROW OF SORIN'S, A STAFF PING ROW, OR ANY OTHER ACTIVATED ROW THAT CHANGES A NUMBER ON THE BOARD IS A LIVE ROW - a row that only ADDS MANA is not one, and you may hold over it - and the hold row's "you give up no cast" is not about them`

**C. WRITING THE ANSWER does not say the CHOICE needs its own physical line. Paid by
`...vs-ai_baka_deck125.jsonl` seq 154** (the seat's only model-side fallback in 288 windows; §2 MED-3
quotes it). One clause, no new concept.
before: `PLAN line, then one CHOICE line, stop.`
after: `PLAN line, then the CHOICE on a LINE OF ITS OWN, stop - a CHOICE sharing the PLAN's line is not read and spends the window.`

**What paid for the bytes.** (1) The verbatim double-Tribute row quotation `"but an effect already
on the stack is aimed at that same creature; if it is gone when this resolves they control 0
creatures and this does nothing"` — that string appears in **0 of 288** prompts this wave; the rule
survives in short form (−213 B). (2) TRIBUTE's "Sharpest case: a victim reading `{T}: Put a ...
token`..." sentence — no such victim was named on any of the 15 `is sacrificed, you gain` rows this
wave (−151 B). (3) Rung 5's "if one is countered or exiled, tutor the next copy" restates the
sentence four lines above it (−52 B). (4) CLEANUP DISCARD reworded (−63 B) — one discard ask in six
games (130 seq18, correct: Chromatic Lantern). No rule was removed.

**What KEPT and earned it.** THE WIN BUTTON + ATTACKING's `SEND IT ANYWAY` / `THE WIN BUTTON
OUTRANKS EVERY STOP` pair: **the 152 game was won on them** at seq 106, on a board where all five
listed verdicts read `your attacker dies, their blocker lives`. THE ZERO STOP: ~60 windows printed
`at 0 this does nothing` in the 125 game and the row was cast **zero** times across the whole corpus
(113 exposures). LAND DROP: 34 of 34 windows took a land. Rung 4's reveal rule: 4 of 4 correct.
MULLIGAN + BOTTOMING: 7 windows, all by the letter, including the 123 ship-at-one-land and its
surplus-land bottom. THE SILENCER TAG: 6 exposures, no both-halves-on-one-menu case — kept, untested.
The Lightmine paragraph: 57 exposures, no attack ever offered under it — kept, untested. The wave-73
`THE ATTACK TOTAL UNDERSTATES YOU` clause is now **redundant with a correct render** (HIGH-1 fixed)
but I kept it: it costs 337 B, it agrees with the render rather than contradicting it, and one
corpus of two converter windows is not enough to retire a rule that pays for the deck's kill.

No general-guide proposal and no skill proposal. A–C are deck-local; HIGH-1, HIGH-2, MED-3, MED-4
and LOW-5/LOW-6 belong to the engine seat, and none of them restates an existing amendment.

## 4. Not checked
I did not read the engine source, so every mechanism sentence in §2 is labelled hypothesis and the
`identical_ask_answers_reserved` / `sibling_window_asks_skipped` / `chain_windows_collapsed` zeros
are reported, not explained. I read opponent seats only where a fact of mine depended on them: the
125 seat's seq 267 self-ping, and 123's Bloodline Keeper flying tokens; I traced none of their other
decisions and contribute nothing on the other decks. I audited `.stderr` only for the 125v126,
152v126, 126v123, 126v130 and 146v126 games and only for hold/chain/combat lines — so I contribute
nothing on the docketed {X}-announcement payment loss, `deadline_pct`, `transport`, or `askreplay/`.
I verified Sorin, Sanguine Bond and Exquisite Blood against primitives; Dictate of Kruphix, Teferi's
Puzzle Box, Fate Unraveler, Siege-Gang Commander, Rorix Bladewing, Intruder Alarm, Bloodline Keeper,
Sigarda, Brutal Cathar, Augur of Autumn, Fateful Absence, Kaya and Supreme Verdict were **not**
verified beyond what the render stated (the Bloodline Keeper flying claim rests on the render's
`Bloodline Keeper (3/3) [flying]` plus the seat's walls having no reach, not on the primitive). I did
not construct the fixture that would settle HIGH-1 or HIGH-2; both are stated as falsifiable repros
for the engine seat. `own_turn_windows_skipped` was 13 in the 146 game and I did not verify what
those skips suppressed.
