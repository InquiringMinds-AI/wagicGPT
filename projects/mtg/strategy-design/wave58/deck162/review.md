# wave-58 deck162 review — "Draw and Die!" (UB forced-draw punisher)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-015059/`. My seat's six logs are
`1788504{671,674,679,681,685,699}-ai_baka_deck162-*-vs-ai_baka_deck{146,130,152,126,125,123}.jsonl`;
opponent seats read for context. Record **4-2** (beat 126, 125, 130, 123; lost to 146, 152).
105 model decisions over the six games, median latency 36.0 s, 5128 s of inference total.
Fallbacks: 3 `empty_reply`, 1 `named_row_reask`, 0 stale drops in my seat's stderr; every one
was followed by a `recovery` record naming what the heuristic did instead.

## 1. Game by game, and the decisions that decided them

**vs126 — WIN, turn 11, 20-0** (`...681-...vs-ai_baka_deck126`). Textbook: Shield Sphere t2,
Howling Mine t4, Underworld Dreams t6, Puzzle Box t8, Fate Unraveler t10. Eleven decisions, no
combat ask, no fallback. Guide's cast order executed line for line.

**vs125 — WIN, turn 15, 20-0** (`...685-...vs-ai_baka_deck125`). Opponent countered Ob Nixilis
(s14, Cancel), Howling Mine (s18→s19, Dream Fracture) and Master of the Feast (s21, Essence
Scatter). The seat simply re-cast: second Ob Nixilis resolved s16 and the Underworld Dreams +
Ob Nixilis pair converted the opponent's 2-card draw steps at 4/turn. Narration correctly
carried every counter ("was COUNTERED by Cancel and went to your graveyard") — I checked the
battlefield line against the log and the missing first Ob Nixilis is a counterspell, not a
lost arrival.

**vs123 — WIN, turn 13, 12/-7** (`...699-...vs-ai_baka_deck123`). Master of the Feast cast t7
against a creature board (Rule 2's named exception, correctly applied), attacked t9/t11/t13.
Deciding decision s19 (t13, opp at 1): attacked with both bodies rather than waiting for a
draw step. Correct.

**vs130 — WIN, turn 16, 18-0** (`...674-...vs-ai_baka_deck130`). Deciding decision s20 (t14
blockers): four blockers spread across four attackers rather than gang-piling — `BLOCKS: B1:A2,
B2:A3, B3:A4, B4:A1`, one unblocked Goblin for 2, exactly as its plan predicted. s23 attacked
with Fate Unraveler while holding Dictate, and the next draw step (3 cards x 2 punishers) was
lethal.

**vs152 — LOSS, turn 13, 0-20** (`...679-...vs-ai_baka_deck152`). Decided at **s19**, the t13
blockers ask: life 18, one Fog Bank (0/2 flying defender), five attackers dealing
4 + 4(trample) + 4 + 3 + 5 = 20. Blocking the 5-power Briarbridge Tracker erases 5 → life 3,
survives. The seat blocked **Sigarda (4/4 trample)** instead; Fog Bank prevents damage but
2 trampled through, so 18 landed and it died at exactly 0. Its reply computed
"4+4+3+5 = 16 damage… I survive with 2 life" — the trample 2 was never in the sum, though its
own B-row printed "2 tramples to your face". **STRATEGY** against the guide (Rule 4 STEP 3 says
block the biggest "deals N"), **plus** a false render bound (HIGH-1 below) that positively
told it the floor was survivable.

**vs146 — LOSS, turn 11, -4/18** (`...671-...vs-ai_baka_deck146`). Decided at **s11**, t8,
life 14, one 0/5 Shield Sphere against three untapped creatures. The only cast row was Dictate
of Kruphix printing `converters on your battlefield: 0 (nothing of yours punishes their draws
or discards yet…)`. Rule 2's brake fired, and Rule 2's *release* ("a punisher is in your hand")
also fired — Underworld Dreams and Ob Nixilis were both in hand — so the seat handed the
opponent an extra card per turn. The mana line read `{B} 2`; Underworld Dreams is `{B}{B}{B}`
(primitive `mtg.txt:125825`), so the punisher was **not** one turn away and never landed. Soul
Shatter ate both Fog Banks and Shield Sphere (s10/s14/s19 of the 146 seat log) and the seat
died with 0 converters ever on the battlefield. **STRATEGY / guide gap** — the release had no
colour test and no board test. Hands were real (mulligan to 6 at s1-s3); the keep was fine, the
release was not.

## 2. Engine / interface / card items

**HIGH-1 — the D7 "or better" bound states a damage FLOOR as a LIFE floor; it read as a
survival guarantee on a lethal board.**
Repro: `1788504679-ai_baka_deck162-0x55dea21cfe10-vs-ai_baka_deck152.jsonl` seq 19, turn 13.
Rendered line, verbatim:
`INCOMING THIS COMBAT: 5 attackers, 20 unblocked damage - you would be at -2; this KILLS you (your creatures may legally block every attacker in that total) - at least 15 of that lands whatever you block (trample/menace counted as unblocked): you would be at 3 or better`
Site: `src/AIPlayerGPT.cpp:15122-15124` (`incomingCombatLine`, `#W58-B (D7)`). `bestCaseDamage`
is a floor on DAMAGE, so `myLife - bestCaseDamage` is a **ceiling on life**; printing it with
"or better" asserts the opposite. The seat's actual legal block landed it at 0 while the line
promised "3 or better". The lane's own PARSETEST cases carry the inversion in plain sight:
`AIPlayerGPT.cpp:47035` expects `you would be at -5 or better` and `:48540` `at -1 or better` —
a negative life "or better" is not a sentence the doctrine allows.
Two fixes, both in the pessimistic direction:
 (a) word it as the ceiling it is — `you would be at 3 AT BEST (no assignment of your blockers
     does better)`;
 (b) the non-optimal branch currently claims no death at all. When `myLife - bestCaseDamage <= 0`
     the damage floor *proves* death and the line must say so (`no block saves you`), exactly as
     the `bestCaseOptimal` branch already does.
Contrast repro, same file seq 14: `at least 4 of that lands… you would be at 16 or better`,
and the seat actually took 2 and ended at 18 — the phrasing looks validated in the safe case,
which is why it survived the lane gate.

**HIGH-2 — the trample amount is promised on the A-line and printed only on the B-line.**
Same file, seq 19. B2's bracket reads
`[deals 0 - this block kills nothing; it stops the blocked attacker's damage except what a trampler pushes through - each A-line above says how much]`
but A2 reads only `A2. Sigarda, Champion of Light (4/4) deals 4 [trample, flying]` — no amount.
The number ("2 tramples to your face") lives in B2's own may-block clause. A pointer to a fact
that is not where it says it is, at the seam that decided the game: either put the
trample-through on the A-line (where the seat does its `deals N` sum) or delete the promise.
This is the D6/D7 under-claim family and is cheap.

**HIGH-3 — hand cards carry cost and type but no castability verdict, and the seat pays for it
in whole decisions.** `src/AIPlayerGPT.cpp:15669` prints `Your hand (N cards): <name> <cost>
[type]` and nothing about whether it is payable. The engine already knows (`legalCasts` builds
the cast menu from it). Two repros in six games:
 - `...671-…vs-ai_baka_deck146` seq 11: **210 348 ms**, the seat's single most expensive
   decision of the corpus (median 36 s), spent re-deriving affordability in prose — and it
   quoted Underworld Dreams as `{3}{B}{B}` (it is `{B}{B}{B}`, `mtg.txt:125825`), reaching the
   right conclusion off a wrong cost.
 - `...679-…vs-ai_baka_deck152` seq 8: answered `CHOICE: 3 (Cast Fate Unraveler)` for a card
   not on the menu — a whole extra round trip, recovered by the re-ask.
A per-card verdict on the hand line — `Fate Unraveler {3}{b} [creature] [not castable now:
needs 4 sources, you have 3]` — adds a true token, removes no choice, and is the cheapest
inference saving visible in my seat.

**MED-1 — no cast-menu row is ever annotated as a GIFT-while-behind.** The `{feeds: …
converters on your battlefield: 0 …}` clause is excellent and the seat reads it correctly every
time (`...671` seq 11 quotes it back). What it does not carry is the board: at 14 life with
three untapped attackers facing it, the same clause reads identically to the safe turn-4 case.
The engine has both numbers on screen already (`M of them without a restriction against
attacking`, `of which N are creatures`). This is the deck-guide's job first (edited below), but
a `{… and you are behind on board}` rider would route it at the render where it cannot be
argued with.

**MED-2 — the blockers example invites a self-contradicting assignment.**
`...699-…vs-ai_baka_deck123` seq 14, reply `BLOCKS: B1:A1, B2:A2, B2:none` — B2 named twice.
The parser's first-wins rule handled it correctly (`Fog Bank blocks Thraben Doomsayer; Fate
Unraveler blocks Thraben Doomsayer`), so no damage was done, but the instruction's own example
`"BLOCKS: B1:A2, B3:A1, B2:none"` teaches that a trailing `:none` is a thing you append, and
the model appended it for a blocker it had just assigned. Dropping `B2:none` from the example
(the next sentence already says "Blockers you do not mention stay out of combat") removes the
shape.

**LOW-1 — same-turn duplicate casting window with a stale plan.** `...671-…vs-ai_baka_deck146`
seq 15 (Main 1) and seq 16 (Main 2): identical two-row menus, the seat answered "Cast nothing
right now" twice and re-sent the same PLAN verbatim both times (102 s of inference for a
no-op). The Hold-priority row exists and works (`...679` seq 6 took it) but was not offered on
seq 16. Worth a look at why the hold row is absent from the Main-2 form of the same window.

**Positive, worth keeping:** the `named_row_reask` → `recovery` path is the cleanest thing in
my corpus. `...679` seq 8 refused a hallucinated row with `[RE-ASK] "Cast Fate Unraveler" is
not on this list. Answer with a number from 1 to 3.`, and seq 10 came back correct. The
`recovery` records (new this wave) name exactly what the heuristic did instead
(`recovered_by: "- You cast Shield Sphere\n- Your Shield Sphere resolved…"`), which turned three
`empty_reply` fallbacks from blind spots into readable history.

**Not checked:** I did not adjudicate any lane prediction (engine seat's job), did not read the
hung 152v126 game or its rerun, did not open the 126/125/130/123 seats beyond the context
needed for my six games, and did not verify HIGH-1's fix shape against the wave-57 ledger D24
history beyond the comment block at `AIPlayerGPT.cpp:15064-15113`. Card facts cited above were
checked against `bin/Res/sets/primitives/` (Underworld Dreams, Dictate of Kruphix, Fog Bank,
Ob Nixilis, Sigarda); I did not cross-check them against Scryfall.

## 3. Guide verdict: **EDIT**

The guide is doing its job — four wins are it executing its own cast order with no deviation,
and both losses are decisions where the guide's text had a hole rather than where the seat
ignored it. Revised guide: `wave58/deck162/strategy.txt` (51 219 B, band 41-71 KB), started
from the live `bin/Res/ai/baka/deck162_strategy.txt` (47 921 B). Three edits, each paid for by
a seq:

**Edit 1 — Rule 4, the "what a block does" preamble. Paid by `vs152` seq 19.**
before: `Every blocker line tells you what its block does: a "STOPS all ..." clause naming the
damage that block erases, and a parenthesis naming who dies. Both are true. Read both and act
on them.` (nothing about trample anywhere in Rule 4)
after: same, plus `AND A TRAMPLER IS NOT STOPPED BY BLOCKING IT.` — the B-row prints the
leftover in its own parenthesis, that number still hits you, blocking a trampler erases only
`deals N` minus that number, and for Fog Bank the trample-through is every point past printed
toughness; never spend your last blocker on a trampler while an equal-or-larger non-trampler is
in the list.

**Edit 2 — Rule 4 STEP 3. Paid by `vs152` seq 19.**
before: `Assign each blocker to the biggest "deals N" it is allowed to stop, whatever its
parenthesis says, and then check step 2 again.`
after: adds that "the biggest it can stop" means the biggest amount ACTUALLY ERASED — for a
`[trample]` attacker that is `deals N` minus the "K tramples to your face" on your own B-row, so
a 5-power non-trampler outranks a 4-power trampler leaving 2 behind, and a trampler is the LAST
attacker a lone blocker goes on — with the seq-19 cost written out (block Briarbridge → life 3
alive; blocked Sigarda → 0).

**Edit 3 — Rule 2's brake RELEASE. Paid by `vs146` seq 11.**
before: the release fires on the hand line alone — `If one of those three names is IN YOUR HAND,
the converter is one turn away … take the engine row.`
after: the release gets two conditions of its own. (i) COLOUR — read `sources that can make
each: {B} n` off the Mana available line; Underworld Dreams needs `{B}` of 3, Ob Nixilis `{B}`
of 2 and 5 total, Fate Unraveler `{B}` of 1 and 4 total; short of that and with no land in hand
to fix it, the punisher is not one turn away and the brake STANDS. (ii) THE BOARD — if their
`M of them without a restriction against attacking` exceeds your `of which N are creatures` AND
your life is 15 or less, the release does not fire at all; take a wall row. Both are written
off numbers already printed in every window, and the seq-11 cost (life 14, `{B} 2`, dead at
turn 11 with zero converters ever cast) is quoted in the guide.

No general-strategy or skill proposals: my two engine items route to the core loop, and the
trample-render teach is already carried by the current skill edition (amendments at
`wave57/strategy-writing-skill.md:9250-9252`) — restating it would be noise.
