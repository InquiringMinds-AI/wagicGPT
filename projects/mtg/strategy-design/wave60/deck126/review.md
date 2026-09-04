# deck126 ("Sanguine Blood") — wave-60 per-deck review

Seat files (all in `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-171140/`), short names used below:
`162` = 1788559904-ai_baka_deck126-0x56068c38a0d0-vs-ai_baka_deck162.jsonl ·
`125` = 1788559917-...-0x55ad4b6e15d0-vs-ai_baka_deck125.jsonl ·
`130` = 1788559926-...-0x564462829dc0-vs-ai_baka_deck130.jsonl ·
`123` = 1788559930-...-0x55de45084f10-vs-ai_baka_deck123.jsonl ·
`152` = 1788559945-...-0x55b02e618660-vs-ai_baka_deck152.jsonl ·
`146` = 1788559953-...-0x559eba6abe30-vs-ai_baka_deck146.jsonl.
Record 2-4. 254 records, 6/6 natural, 0 crash/hang. Fallbacks: 2, both in `125`
(s15 `named_row_reask`, s17 `named_row_not_offered`), both recovered (s16, s18). `dropped_assignments` 0 on all
10 blocker replies. No `empty_reply`, no `transport_error`, no `bad_reply`, no timeouts at this seat.

## 1. Game by game — outcome and the deciding decisions

**`130` — WIN 38 / 0, turn 15.** Textbook Rule #2. Sanguine Bond s10 (t9), Exquisite Blood s17 (t13), Sorin
+1 for a lifelink Vampire s18/s20, attack with it s21 → loop → opponent 20 to 0 in that combat. No misplay.

**`152` — WIN 112 / 0, turn 22.** Perimeter Captain wall-and-gain plan executed exactly: batched
"Gain 2 life - accept ALL 6/10 triggers" asks at s9/s15/s19/s23/s28 carried the seat to 95 life; Exquisite
Blood s17 (t14), Idyllic Tutor → Sanguine Bond s32/s33, cast s34 (t20), Staff of Nin ping s35 → loop → 112/0.
No misplay.

**`162` — LOSS 0 / 20, turn 14. This is the game the corpus paid for.** Opponent board from t10:
Forced Fruition (`@movedTo(*[-land]|opponentstack):draw:7 opponent`, "whenever an opponent casts a spell that
player draws seven cards") + Underworld Dreams + Howling Mine. Every cast the seat makes costs it exactly 7
life; it controls zero creatures, so the seat's walls block nothing all game.
**Deciding decision: s16, turn 13, life 23, 7 untapped sources.** The menu printed BOTH combo halves as live
rows, each priced survivable — row 1 `Cast Exquisite Blood {4}{b} ... [DRAW PRICE: casting this draws YOU 7
cards (their Forced Fruition), and their Underworld Dreams deals you 7 - you would be at 16]` and row 4 the
same for `Cast Sanguine Bond {3}{b}{b}`. The seat took **row 5, Wall of Omens**, then Tribute to Hunger on a
Shield Sphere (s17) and a Pride Guardian (s18): three casts, 22 life in one turn (23 → 7 in the s19 log),
neither half cast. It then correctly declined at s19/s20 once the only row read "this KILLS you", discarded
19 cards at cleanup (s21), and died in the t14 draw step. This is **STRATEGY**, not perception — the price was
rendered on every row and the model's own PLAN at s19 quotes the guide's advice back ("Survive the Forced
Fruition/Underworld Dreams storm by casting cheap defenders"). See §3.

**`123` — LOSS -76 / 22, turn 18.** Opponent assembled Intruder Alarm (t10) + Bloodline Keeper + Lightning
Greaves (t16): 19 Vampire tokens in one turn, 47 by t18. All flying; the seat's only creatures were ground
defenders, so no blockers window was legal and none was offered (opponent `123` s63 confirms
`[no creature they control can block this attacker]` on all 48 attackers). The seat's line (lands every turn,
two Tributes, Idyllic Tutor → Sanguine Bond s12/s13, Sanguine Bond s15) was on-guide throughout; it never saw
Exquisite Blood. No misplay I can identify — this was lost to an opposing two-card engine, not to a decision.
The prompt's failure here is §2 MED-1: the s20 `CRACK-BACK NEXT TURN: 20 of their creatures will be able to
attack ... for up to 41 - you would be at -20; that would KILL you` never says the seat can block none of it.

**`146` — LOSS -1 / 23, turn 15.** Seat held Sanguine Bond from t8 (s12) and drew into Exquisite Blood at t10
(s16); cast Exquisite Blood s18 (t12). From then on Sanguine Bond was uncastable: at s20 (t14) six untapped
sources but only ONE ({B}-capable Scrubland), and `{3}{b}{b}` needs two. The prompt printed
`LOOP HALF PENDING: Exquisite Blood is on YOUR battlefield and the other half of the pair, Sanguine Bond, is in
your hand ... the pair is one resolution from closing` while never saying the card was unaffordable or why
(§2 MED-2). Seat cast Sorin instead (s20/s21), blocked with the token at s22 and died. Mana variance, not a
guide break — Rule #1's pip count already states the two-black requirement.

**`125` — LOSS 0 / 33, turn 39.** Blue-white control. The seat assembled the pair on schedule (Sanguine Bond
s20 t12, Exquisite Blood s21 t14) but the halves were countered on every subsequent attempt (`Dream Fracture`,
`Fall of the Gavel`, `Cancel` — s118-s119 log). It read that correctly (s27 PLAN: "The combo is broken (both
pieces countered)") and fell back to Staff of Nin pings, 12 of them, 1 damage each, against a deck with two
Staff of Nin, Elixir of Immortality and 16 lands. Losable game. One real misplay: **s121 (t38)** — attacked
with 2 lifelink Vampires into a board carrying Lightmine Field; both died before damage and the seat gained 0
(§2 HIGH-2).

## 2. Engine / interface / card items

**HIGH-1 — A row carrying two DRAW PRICE clauses prints two contradictory "you would be at" numbers, and the
row's true cost is understated.** `162` seq 16, turn 13, life 23, row 5, verbatim:

    5. Cast Wall of Omens {1}{w} (0/4) {leaves 5 of your 7 untapped mana sources untapped} {card text: "Defender
    (This creature can't attack.) -- When Wall of Omens enters, draw a card."} [DRAW PRICE: this draws 1 card,
    and the opponent's Underworld Dreams punishes every draw, so taking it costs you 1 life right now - you
    would be at 22] [DRAW PRICE: casting this draws YOU 7 cards (their Forced Fruition), and their Underworld
    Dreams deals you 7 - you would be at 16]

Both clauses are folded off the SAME base life (23), so the row shows "22" and "16" and neither is the answer;
the actual cost was 8 (the `162` s19 log shows 8 Underworld Dreams triggers between the cast and the Tribute).
Every other row on that menu carried one clause and was correct. The two clauses must be summed into one
verdict before the "you would be at" fold. This is the row the seat took.

**HIGH-2 — `ATTACK TOTAL` ignores attack-triggered punishers on the opponent's battlefield.** `125` seq 121,
turn 38, opponent battlefield line carries
`Lightmine Field {2}{w}{w} [enchantment] {effect: "Whenever one or more creatures attack, Lightmine Field deals
damage to each of those creatures equal to the number of attacking creatures."}`. The ask rendered:

    ATTACK TOTAL: 2 attackers listed, 2 total combat damage to a player - declaring all of them with none
    blocked puts them at 31. At least 2 damage lands whatever they block - they would be at 31.

Result (log at s123): `Lightmine Field dealt 1 damage to Vampire` ×2, both 1/1 tokens died, zero damage dealt,
zero lifelink gain. The lane-L B11 ATTACK TOTAL fold prices blockers and evasion but not attack triggers; it
should either subtract attackers that die to a declaration trigger or refuse to assert a damage total. Note the
blockers ask has the corresponding lens and the attackers ask does not.

**MED-1 — `CRACK-BACK NEXT TURN` never nets out blocks or flags evasion; 51 of 51 renders at this seat.**
`123` seq 20, turn 17, verbatim:
`CRACK-BACK NEXT TURN: 20 of their creatures will be able to attack (tapped ones untap first), for up to 41 -
you would be at -20; that would KILL you`. Nineteen of those 20 were `Vampire (2/2) [flying]`, and the seat's
whole board was ground defenders, so the true figure was "you can block none of this". The blockers-ask
`INCOMING THIS COMBAT` line already prints `best case with every blocker assigned: you would be at N`;
CRACK-BACK has no counterpart. (The 41 also went stale — the actual attack was 48 creatures / 97 damage after
the opponent's Intruder Alarm loop made 28 more tokens — but that is a forecast horizon, not a defect.)

**MED-2 — wave-59 deck126 MED-1 RECURS: `your creatures may legally block every attacker in that total` is
printed when they cannot.** Three false renders at this seat, all in `146`: seq 9 (2 attackers, 1 blocker),
seq 17 (3 attackers, 1 blocker), seq 22 (6 attackers, 1 blocker). Verbatim, `146` seq 22, turn 15:
`INCOMING THIS COMBAT: 6 attackers, 12 unblocked damage - you would be at -3; this KILLS you (your creatures
may legally block every attacker in that total) - best case with every blocker assigned: you would be at -1;
no block saves you` — the parenthetical and the "no block saves you" tail contradict each other in one line.

**MED-3 — `LOOP HALF PENDING` urges closing a pair whose other half is silently unaffordable.** `146` seq 20,
turn 14: banner says `the pair is one resolution from closing`, Sanguine Bond `{3}{b}{b}` is on the hand line,
6 untapped sources are listed, exactly one can make `{b}`, and no Cast row for it appears with no reason
given. This is the wave-59 "unaffordable hand cards omitted with no reason" MED, made load-bearing by the new
banner: the banner should carry the blocking reason ("needs 2 black sources, you have 1") or not fire.

**LOW-1 — the `[second copy: ...]` guard did not stop two Chromatic Lantern casts.** `125` seq 61 and seq 62,
turn 26: both rows read `Cast Chromatic Lantern {3} [second copy: you already control Chromatic Lantern; both
stay on the battlefield - no legend rule]` and both were taken in the same window. The tag is rendered; the
model treats "legal" as "worth it". Guide-side this is covered (Rule #2's three-dead-rows list); flagging it
only because two identical dead casts in one window is cheap for the engine to collapse.

**LOW-2 — protocol overrun is routine, not a defect.** 175 of 240 answered records carry
`post_answer_overrun` (prose after the CHOICE/PLAN lines), 7 carry `post_plan_overrun`; no reply was lost to
it. Plan echoes stayed short (max `plan_echo_count` 5, in `125`). The wave-59 deck126 HIGH-1 unbounded plan
echo did not recur at this seat.

## 3. Guide verdict: EDIT

The wins (`130`, `152`) and the `123`/`146` losses show the guide is working — Rule #0/#1/#2/#3 were followed
in every game and both wins are the guide's Rule #2 line executed literally. One passage is actively wrong and
cost a game, and it is wrong because it contradicts Rule #7 entry 1.

`wave60/deck126/strategy.txt` is the full revised guide (started from
`bin/Res/ai/baka/deck126_strategy.txt`), **70,974 bytes** (was 70,997 — inside the 41-71 KB pool band).

**Edit 1 (the one the corpus paid for): SITUATIONS, the DRAW-PUNISHER entry.**
Before:
> THE PROMPT PRINTS "DRAW PUNISHERS on the battlefield: theirs - <name>" AND A "DRAW FORECAST: your next draw
> step draws N cards ... = N life": ... the only half of it you control is your hand size ... **So empty your
> hand - cast the cheapest permanents you hold, every turn** - and take TWO cheap walls over one card-neutral
> Idyllic Tutor.

After (abridged; full text in strategy.txt): the entry now splits into **(a)** the draw-step punisher it was
written for (a bare `DRAW FORECAST` with no per-row price — hand size is the multiplier, empty your hand,
unchanged) and **(b)** the cast-triggered punisher, where the ROWS carry
`[DRAW PRICE: casting this draws YOU N cards (their Forced Fruition) ... - you would be at X]`: "the damage is
metered by your CASTS ... EVERY row costs the SAME fixed life. Emptying your hand here is suicide - three casts
at 7 apiece is 21 life. Rule #7 entry 1 is the ONLY row worth that price; a wall, a Lantern or a Tutor bought
at it is life paid for nothing, and a wall is worth ZERO on a board whose opponent line reads '0 are
creatures'." The `162` s16-s18 sequence is written in as the paying evidence, and HIGH-1's double-clause
arithmetic is added as a reading instruction ("computes both 'you would be at' numbers off your CURRENT life -
add both costs and subtract once"). **Paid for by `162` seq 16 (rows 1, 4 and 5 quoted above), seq 17, seq 18.**

Why this and not a Rule #7 patch: Rule #7 entry 1 already says "NOTHING BELOW ENTRY 1 IS AN ANSWER WHILE AN
ENTRY-1 ROW IS PRINTED" and "BOTH NAMES MISSING AND BOTH ROWS PRINTED ON THE SAME MENU: TAKE SANGUINE BOND".
The rule was right; the SITUATIONS entry licensed the exception, and the model's s19 PLAN shows it took that
licence. Fixing the contradiction at its source is the whole edit.

**Edits 2-6 (byte budget only, no behaviour change).** To stay inside the band, six superseded corpus-history
clauses were trimmed, each one the guide itself already reports as fixed by a later corpus:
Rule #2 "Two corpora ago you got that wrong six times in one game ... not one changed a number on the board."
→ dropped (the following sentence "Last corpus: none of them, off 160 Tribute-at-0 rows" is the live
calibration); Rule #2 "; the corpus before, you cast a Wall of Omens and a Battlement over it three times
first." → dropped; Rule #3 "Ten of ten right the corpus after that, and five of five the next - and then this
corpus," → "Then this corpus,"; Rule #7 "Two corpora ago you read that clause twice and took a wall or a Tutor
instead; last corpus you took every one of them" → "Last corpus you took every one of them"; Rule #7 "it broke
4 of 11 two corpora ago and went 10 of 10 last corpus" → "it went 10 of 10 last corpus"; Rule #7 "No such row
printed at all last corpus." → dropped. Net -23 bytes against the live guide.

No `general-proposals.md`: the failure was this deck's guide contradicting itself, and the per-row DRAW PRICE
lens the general layer supplies was correct and sufficient at every window. No `skill-proposals.md`: the
edit is an ordinary "split a rule whose two cases want opposite play" move that the amendment set already
covers; a proposal restating it would be noise.

## 4. What I did NOT check

- I did not adjudicate any lane-L/M/N/O/P/Q prediction — that is the engine seat's job; I read only
  `known-bugs.md` and this brief, not `lane-*.md` or `codex-review.md`, so nothing above is a PASS/FAIL claim.
- I did not read `wave59/general-strategy.md` or `wave59/strategy-writing-skill.md` in full (I am proposing
  no change to either).
- I traced the opponent's translog only for `123` (to confirm the 48-attacker declaration and that no blockers
  window was legal); the other five opponents' seats I did not open.
- I verified Forced Fruition, Underworld Dreams, Howling Mine, Lightmine Field, Wall of Omens, Exquisite Blood
  and Sanguine Bond against `bin/Res/sets/primitives/*.txt`; all seven match their Oracle text, and I did not
  need Scryfall. I did not verify the rest of the deck's cards.
- I did not measure latency, `deadline_pct` or transport fields, and I did not open the `.stderr` files.
