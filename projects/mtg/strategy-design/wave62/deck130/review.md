# wave-62 deck130 review — Budde's Ponza (mono-red land destruction)

Seat corpus: 6 games, all natural, **0 fallbacks, 0 re-asks, 0 wall misses, 0 timeouts** across
250 decision records (19/61/59/34/14/63 per game). Binary 12be0b79c. Record 3-3.
Files below are in `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-133058/`, named by the
`...deck130-0x...-vs-<opp>` seat unless stated.

---

## 1. Game by game, and the decisions that decided them

### L vs deck152 (turn 13, -5 to 20) — `1788633061-...-vs-ai_baka_deck152.jsonl`
Mana screw, not a guide effect. Three Mountains from turn 6 to turn 12 with `Land drop: you have no
land you could play right now` at seq10/13/16. It still spent every window: Stone Rain seq8 + seq11,
Blastminer seq14, a correct block seq15.
Deciding window **seq17 (turn 12, life 5)**: `CRACK-BACK NEXT TURN: 3 of their creatures ... for up
to 9 ... that would KILL you`. Only 3 mana. It took Starstorm and announced X=1 (seq18/19), killing
Katilda (1/1) and leaving the 6/4 flier. Defensible — no line on that menu survives 9 damage — and
the cast-route it walked (cast row -> `Choose an option` -> X menu, each row carrying its own kill
list) is exactly the surface lanes Y/AA built. Verdict: variance.

### L vs deck146 (turn 15, -5 to 22) — `1788633071-...-vs-ai_baka_deck146.jsonl`
**The deciding decision is seq1, the opening keep.** Header: `counted by the engine: 6 lands, 1
spell`, `Playing every land in this hand would cover the cost of: Pyrite Spellbomb {1}`. It kept.
Fourteen turns later it had cast exactly one non-cantrip spell (Stone Rain, seq9) and died. This was
**guide-compliant** — see §3; the guide told it to keep.
Downstream, both Pyrite Spellbombs went at 1/1 Goblin tokens (seq11, seq28) while Nadaar hit for
3-4 a turn. That is not clearly wrong (Spellbomb's 2 cannot kill a 3/3 alone) but the two together
could have. The last two windows (seq33 Rorix, seq34 attack with it) are not the loss: at life 2
into Nadaar + two 2/1 Spiders, holding Rorix back blocks one attacker and still dies.
See the HIGH card item in §2 — Nadaar was hitting one point harder than its Oracle text allows.

### L vs deck126 (turn 13, 0 to 40) — `1788633081-...-vs-ai_baka_deck126.jsonl`
Unanswerable. Mulliganed a 0-land seven (seq1, correct), kept at six (seq2), bottomed a Mountain
(seq3). By **seq14 (turn 12 upkeep)** the prompt already rendered the full loop:
`Both halves of a life LOOP are on THEIR battlefield (Sanguine Bond + Exquisite Blood)`. Ponza holds
no enchantment removal at any point in its 60. The loop fired on the opponent's turn 13 off a
Tribute to Hunger lifegain (opponent seat seq18) and ran 20 to 0. Nothing in the seat's window
changes that. One small note: the reply at seq14 wrote *"Avoid drawing cards while Sanguine
Bond/Exquisite Blood loop is active"* — the loop keys off life change, not draws. §3 covers why.

### W vs deck125 (turn 30, 20 to 0) — `1788633065-...-vs-ai_baka_deck125.jsonl`
Clean execution of the guide's late-game plan. Hammer of Bogardan's upkeep recursion
(`Put a card into hand with Hammer of Bogardan`) taken at seq42/49/54/55/58 and cast for 3 to the
face each turn — 15 -> 12 -> 9 -> 6 -> 3 -> 0 with the seat never dropping below 20 life. Every
Starstorm/Lay Waste cycle (seq24, seq6, seq14) happened with `of which 0 are creatures` on the
opponent line, per rule #1. seq61 took the row annotated `THIS WINS THE GAME`.

### W vs deck162 (turn 20, 17 to -1) — `1788633067-...-vs-ai_baka_deck162.jsonl`
Land destruction to a real kill: Molten Rain seq6/seq16/seq51, Stone Rain seq9/seq14, then
Hammer + Spellbomb + three Siege-Gang Goblin sacrifices (seq54/57/59) for the last 6. Starstorm
X=6 at seq21-23 was priced by the cast row (`kills THEIRS: Shield Sphere; YOURS: none`) and taken.
Underworld Dreams was on their board from turn 20 and the `DRAW PUNISHERS` line rendered in all 17
windows of that turn (seq43-59) — the guide's #0 rule had the surface it promises.

### W vs deck123 (turn 36, 20 to 0) — `1788633083-...-vs-ai_baka_deck123.jsonl`
Total land lock: from turn 16 onward the opponent line reads `1 permanent listed, ... and 0 are
lands`. The seat then correctly **declined its own land destruction eight times** when the only
legal targets were its own Mountains (seq23/24/25/26/33/34/40/41 — `the only legal targets are YOUR
OWN right now` + `(warning: this would harm your own side)`). Win again on Hammer recursion
(seq54/56/58/61/62).

---

## 2. Engine / interface / card items

### HIGH — Nadaar, Selfless Paladin pumps ITSELF; Oracle says "Other creatures"
Repro: `1788633071-ai_baka_deck130-...-vs-ai_baka_deck146.jsonl` **seq21, seq29, seq33**, rendered
board line:
`Nadaar, Selfless Paladin {2}{w} (4/4) (printed 3/3) [vigilance]`
Primitive (`bin/Res/sets/primitives/borderline.txt:75387`):
`auto=this(variable{pdungeoncompleted}>0) lord(creature|myBattlefield) 1/1`
The same card's own `text=` line (75388) reads *"**Other** creatures you control get +1/+1 as long
as you've completed a dungeon."* Scryfall agrees verbatim (3/3, "Other creatures you control...").
`lord(creature|myBattlefield)` includes the source; the correct idiom `lord(other creature|...)` is
used ~190 times elsewhere in the same primitives. Effect in this game: the dungeon completed on
turn 9 and Nadaar's damage log went `dealt 3 damage` (turn 7) -> `dealt 4 damage` (turns 9, 11, 13),
three points the seat should not have taken in a game it lost at -5 from life 2. Data-only fix.

### HIGH — the CRACK-BACK animator caveat names ONE permanent once per animate RUNG
Repro: `1788633061-ai_baka_deck130-...-vs-ai_baka_deck152.jsonl` **seq17, seq18, seq19**:
`... that number is a FLOOR, not a ceiling: noncreature permanents of theirs that can animate and
attack are not in that count - Lair of the Hydra, Lair of the Hydra, Lair of the Hydra, Lair of
the Hydra, Lair of the Hydra`
The same prompt's `Opponent battlefield` line lists **one** `Lair of the Hydra [tapped]`.
Mechanism: `src/AIPlayerGPT.cpp:18317-18330` walks the card's script LINE BY LINE and appends
`c->getDisplayName()` on every line matching `becomes(creature` that passes `crackBackAbilityUsable`.
Lair of the Hydra defines 8+ animate rungs (`borderline.txt:64293-64300`), so an affordable-rung
count is printed as a permanent count. Fix: emit at most one entry per card (keep the largest
affordable rung). Corpus-wide this is the only card that triggers it (3 of 3 windows where a
multi-rung manland was in play), all of them in this seat.
**Second defect on the same line**: the `(N power once animated)` suffix never appeared, because
line 18384-ish parses the power from a `^` after `becomes(`, while Lair's script writes
`becomes(Creature Hydra,1/1,green)` — the comma form. So the seat is told a manland can attack but
never how hard, on exactly the cards that most need the number. This is a *different* surface from
lane-Y **D4** (which fixed the RankingContainer collapse in the OPTION list); the crack-back
narration was not covered by it.

### MED — HOLD taken in main phase 1 does not cover main phase 2, because of one parenthetical
Repro pairs (same turn, identical numbered rows, seat re-asked): deck123 seat **seq23->24 (t16),
25->26 (t18), 30->31 (t20), 33->34 (t22), 40->41 (t24), 43->44 (t26)**; deck125 seat **25->26 (t14),
33->34 (t18)**. 8 of this seat's 250 decisions.
The only textual difference between the two windows is the suffix on the pass row:
main 1 `2. Cast nothing right now (combat comes next this turn)` vs main 2 `2. Cast nothing right
now`. The hold row's own promise — *"do not ask me again ... until one of the rows above changes"* —
is therefore false by a phrase that carries no decision content: at seq23 the board, hand, mana and
every castable row were identical, and the seat had just declined them. Either exclude that
parenthetical from the hold's identity key, or move it out of the row text into the header.

### LOW — a land-destruction row is offered whose only targets are the caster's own lands
`Cast Molten Rain ... - the only legal targets are YOUR OWN right now: Mountain #1-#7 (land) x7
(warning: this would harm your own side ...)` (deck123 seat seq23-44, ~10 windows once the lock was
complete). The warning **worked** — the seat declined every one — so this is a cost item, not a
correctness item: once the opponent controls zero lands, these rows could be suppressed rather than
warned, and the seat would stop being asked a question with no right answer.

### LOW — `mana_only_windows_skipped` reads 0 in all six deck130 seats
The opponent seat in the same deck126 game reports `mana_only_windows_skipped: 171`. deck130
controlled 2-3 Talismans of Impulse in four of its games and still logged 0. I did not read the
counter's definition, so this may be correct by construction; flagging it as a thing to check, not
as a claimed defect.

---

## 3. Guide verdict: **EDIT** (one rule, one loss)

Everything else in `deck130_strategy.txt` held under trace, and I want that on the record before the
edit: rule #1's Starstorm gate (never cycled with a creature on the opponent line — seq24/51 both
read `of which 0 are creatures`), the NEVER-ANNOUNCE-X=0 gate (X=1 and X=6 taken, X=0 never), the
upkeep pass rule, the land-drop-before-cycle gate, the Hammer EMPTY-HAND clock (the win condition in
**all three** wins), and the #0 draw-punisher rule (its promised `DRAW PUNISHERS on the battlefield:`
line rendered on every window of deck162 turn 20). Those are KEEP-quality and are untouched.

The one rule the corpus refuted is the mulligan floor. `deck130_strategy.txt:637` reads *"A hand with
two or more lands and at least one spell is a keep at every N"*, and the quick-reference bullet at
:808 reads *"The mulligan header's coverage line names even one card -> keep, whatever the land
count"*. deck146 seq1 was `6 lands, 1 spell` with a coverage line naming Pyrite Spellbomb — the guide
mandated that keep, and the keep is the loss. The guide already ships flood-shape floods (*"a hand of
nothing but lands ships at any N above 4"*) but the 6-lands-and-a-one-drop shape falls through it.

Revised guide: `wave62/deck130/strategy.txt` (70999 bytes, in the 41-71 KB band; was 70927).

**Edit 1 — MULLIGAN section, :637.** Paid for by deck146 seq1.
before: `one spell is a keep at every N. A one-lander is a keep when the coverage line names a card,`
after: `one spell is a keep at every N - UNLESS THE HEADER READS 5 OR MORE LANDS AND EXACTLY 1
SPELL. / "6 lands, 1 spell" is not a keep: one spell is one action, and a deck whose turn has a job
every turn cannot do it off a hand with one card to cast. Ship it at (keeping 6) and (keeping 5);
keep only at (keeping 4) or lower. / A one-lander is a keep when the coverage line names a card,`

**Edit 2 — quick-reference bullet, :808.** Same seq; the bullet is the surface the seat actually
reads at the mulligan ask, so the gate has to exist in both places or the earlier one does not bind.
before: `- The mulligan header's coverage line names even one card -> keep, whatever the land count;`
after: `- The mulligan header reads 5 or more lands and exactly 1 spell -> SHIP at (keeping 6) or
(keeping 5); keep only at (keeping 4) or lower. / - Else the coverage line names a card -> keep,
whatever the land count;`

**Edits 3-5 — byte budget only, no behavioural change.** The two additions above pushed the file
past the band ceiling, so three sentences of pure corpus scorekeeping were cut to pay for them, each
of which stated a past tally rather than a rule: the "Both of last corpus's SURVIVES takes were
loyalty rows - Teferi 3 to 1, Sorin 4 to 1 ... for the first time in three corpora" sentence in the
DIES/SURVIVES block; the "and this sentence has already told this deck to pass it fourteen upkeeps
running while the opponent sat at 4" clause in the upkeep rule; and the quoted-plan tail of the X=0
anecdote (`"X=0 is fine ... we are just clearing the way"` -> `"X=0 is fine"`). No instruction, gate,
number or threshold was removed anywhere in the file.

No general-guide proposals and no strategy-writing-skill proposals: nothing this seat did wrong is
general (the mulligan floor is this deck's own land count and curve), and both items in §2 are
engine/card work, not guide work.

---

## What I did NOT check
- I did not run wagic, rebuild, or run PARSETEST; the Nadaar fix and the crack-back dedupe are
  **unverified by execution** — both are read off the primitive/source plus the rendered line.
- I did not adjudicate the wave-62 lane predictions (engine seat's job) and did not read the 20
  games outside deck130's six, or the 152v125 rerun.
- I traced the opponent seat only for the deck126 loop and the deck146 board; I did not read the
  other four opponent translogs end to end.
- I did not verify Nadaar's damage arithmetic against the combat-damage code — only against the
  rendered P/T, the game log's `dealt 3` -> `dealt 4`, the primitive, and Scryfall.
- I did not measure whether the MP1/MP2 re-ask costs a wall-clock deadline anywhere; latency was
  never near the deadline in this seat (`deadline_pct` peaked at 8.9).
- I did not check `mana_only_windows_skipped`'s definition in source before flagging it.
