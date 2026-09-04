# deck130 (Budde's Ponza, mono-R land destruction) — wave-60 per-deck review

Record **1/6**. Corpus `matchups-20260904-171140`. Seat files
`1788559{910,912,915,919,927,942}-ai_baka_deck130-0x…-vs-…`. 367 replies, 1 fallback
(`wall_miss_unrecorded`), 0 crashes, 0 hangs.

**The one-line finding:** the render was right and the seat did not use it. Deck130 ran
4 Starstorms, was offered `Cast Starstorm` in **51 windows**, and cast it **0 times**;
in the 3 of those windows where the row's own `{X pricing:}` named two or more of THEIRS,
it declined all 3, and two of those declines are the deciding decision of two losses.
The guide's mandated `PLAN: Starstorm: THEIRS <n>; YOURS names Rorix/Commander <y/n>` line
was written **0 of 51** times.

---

## 1. Game by game

| opp | result | turn | life | deciding decision |
|---|---|---|---|---|
| 123 | LOSS | 11 | −10 / 20 | `…vs-ai_baka_deck123` **seq 20** (t10) |
| 152 | LOSS | 15 | −7 / 35 | `…vs-ai_baka_deck152` **seq 19** (t12) |
| 146 | LOSS | 21 | −1 / 9 | `…vs-ai_baka_deck146` **seq 41** (t12) |
| 162 | LOSS | 14 | 0 / 9 | `…vs-ai_baka_deck162` **seq 60–63** (t13) |
| 126 | LOSS | 15 | 0 / 38 | no seat decision decided it (see below) |
| 125 | **WIN** | 44 | 8 / 0 | Hammer-of-Bogardan recursion to the face, t42–44 |

**vs deck123 (seq 20, t10, 16 life, 5 mana).** Opponent board: Thraben Doomsayer 2/2
`[tapped]`, Bloodline Keeper 3/3, **Intruder Alarm** (its full effect text rendered on the
line). Row 1 read verbatim:

> `1. Cast Starstorm {r}{r}{x} {X pricing: max affordable X=3 (5 mana total); … At X=3 - kills THEIRS: Thraben Doomsayer, Bloodline Keeper; YOURS: none} [<- best X for this cast: X=3 - most kills at any affordable X that costs you nothing]`

The seat took **row 4, Siege-Gang Commander**, and wrote *"Next turn, use Stone Rain or
Molten Rain to remove Bloodline Keeper or Intruder Alarm."* Siege-Gang's three Goblins
entering triggered Intruder Alarm, untapping the Doomsayer — which under Intruder Alarm
never untaps on its own — and started the free-tap token loop. Next window (seq 21) the
opponent had **21 Humans** and `INCOMING THIS COMBAT: 23 attackers, 26 unblocked damage…
no block saves you`. The guide already carries this exact board as a worked example
(deck130_strategy.txt:376–388, "the Doomsayer made nineteen Humans, and the game ended at
−34"); the seat reproduced the losing line nearly word for word.

**vs deck152 (seq 19, t12, 2 life, 6 mana).** Header read `CRACK-BACK NEXT TURN: 4 of
their creatures will be able to attack … for up to 16 - you would be at -14; that would
KILL you`. Row 1:

> `{X pricing: … At X=4 - kills THEIRS: Briarbridge Tracker, Intrepid Adversary x2, Wolf; YOURS: Dwarven Blastminer} [<- best X for this cast: X=4 - best trade … THEIRS 4 / YOURS 1]`

X=4 clears their entire offence for one 1/1. The seat took **row 3, Siege-Gang Commander**.
Guide rule #2 step 0b ("SURVIVAL FIRST … take the X whose THEIRS list is longest, WHATEVER
the YOURS list names") fired on every clause and was not executed. Two windows later
(seq 23, still 2 life) the same row offered THEIRS 2 / YOURS 1 and the seat cast **Rorix**.

**vs deck162 (seq 60 → 63, t13, 3 life, opponent 9).** Fate Unraveler is a **3/4**. The
seat spent Spark Spray on it off a row reading `{kills whichever you target: THEIRS -
none; …}`, then activated Pyrite Spellbomb at it off a row reading
`{right now: takes 2 damage - SURVIVES (toughness 4, 3 more kills it)} … {paying this
costs you 1 life (Talisman of Impulse: 1 damage) - you would be at 2}`. Three damage on a
4-toughness body: two cards and one life for nothing. Its own seq-60 reply computes the
answer and then ignores it — *"1 + 2 = 3, still not enough. Wait, I need to kill it?"* —
before choosing the row. That single Talisman life was the game: it died in its own t14
draw step at 0 with **lethal on board** (Rorix 6 unblockable by two 0/6 defenders + three
Goblin sacrifices at {1}{R} = 12 vs their 9, with 7 mana untapped, seq 64). At 3 life it
would have reached Main 1.

**vs deck146 (seq 41, t12, life 8).** Silverquill Silencer (3/2) attacked every turn from
t7 and dealt ~24 of the 21 damage that killed it. At **life 8** — the guide's own floor
("at 8 life or less, stop destroying lands", deck130_strategy.txt:~430) — the seat cast
Molten Rain at a land instead of holding mana for the creature. Endgame is clean: at t20
seq 62/63 it correctly declined Hammer (the `[NAMED BY THEIR Silverquill Silencer: … you
would be at -1; this KILLS you]` clause was read and obeyed). Not a render defect.

**vs deck126.** Not lost at a decision seam. The opponent assembled Sanguine Bond (t9) +
Exquisite Blood (t13); deck130 has zero enchantment answers in 60 cards. Seat play was
reasonable throughout. One `wall_miss` here (below).

**vs deck125 (WIN).** 44 turns, 158 records. Won by Hammer of Bogardan graveyard
recursion aimed at the face through a counterspell deck — the guide's Hammer and
face-priority rungs working exactly as written.

---

## 2. Engine / interface / card items

### HIGH — the cleanup-discard `{spare: …}` clause is an unconditioned verdict and it is false at low land counts
`src/AIPlayerGPT.cpp:34018-34026` (`discardSpareLandClause`), applied at `:34212` to
**every** land row whenever `myLands >= 1`, with no comparison against the hand's costs
or the turn.

Repro — `1788559912-ai_baka_deck130-…-vs-ai_baka_deck123.jsonl` **seq 9**, turn 4, seat on
**two** lands, holding Siege-Gang Commander {3}{r}{r}:
```
4. Mountain (land) {card text: R} {spare: you control 2 lands already}
5. Forgotten Cave (copy 1 of 2 …) {spare: you control 2 lands already}
6. Forgotten Cave (copy 2 of 2 …) {spare: you control 2 lands already}
```
The seat answered `PUT: 4, 6` — it sent two lands. The deck130 guide's cleanup order
(deck130_strategy.txt:~652-658) sends "a land beyond the **third** in hand", i.e. none
here; a duplicate Stone Rain and the uncastable Siege-Gang were the guide's answers. The
clause is printed *only on land rows*, so the emitter marked exactly the two cards the
guide protects and marked nothing else. `myLands` is a count (a fact); **"spare" is a
verdict** and at 2 lands with a 5-drop in hand it is a wrong-scope lie of the kind the
same file's own comment block at `:34028-34033` says the emitter must not tell.
Corpus-wide this clause printed at `myLands <= 4` exactly once — this window — and the
seat took the bait 1/1.
Minimal fix shape: gate the word (print the bare count, or withhold the clause) until
`myLands` reaches the largest mana value in hand. *Impact in this game: low (it recovered
to 5 lands by t10); the falsehood is the reason for the rank.*

### HIGH — WHETHER-to-cast has no marker; WHICH-X does, and only the marked rung is obeyed
The `Announce the value of X` menu carries `[<- best X for this cast: …]`, and the guide
records that rung as answered 6/6 last corpus ("this rung is DONE"). The cast row one
screen earlier carries the same kill lists **and the same marker string**, but the marker
answers *which X*, never *whether* — and the guide says so in as many words
(deck130_strategy.txt:~170, "A MARKER IS NOT A REASON TO CAST"). Result this wave, same
model, same guide: **WHICH-X 0 windows reached; WHETHER 0 of 3 taken.**
Repro rows: `deck123` seq 20 (`THEIRS: Thraben Doomsayer, Bloodline Keeper; YOURS: none`),
`deck152` seq 19 (`THEIRS 4 / YOURS 1`, with `CRACK-BACK … that would KILL you` five lines
above it), `deck152` seq 23. In all three the seat chose a body over the sweep.
The interface fact is that the cast-row menu ranks nothing: rows 1..N are in an
engine-internal order and a row that clears the opponent's whole board reads the same
weight as a row that plays a 2/2. This is the one place where a printed ranking has
already been shown (on the X menu) to move this model.

### MED — a removal row can say "kills nothing / SURVIVES" and be taken anyway, twice in one turn
`…vs-ai_baka_deck162` **seq 60** (`{kills whichever you target: THEIRS - none; YOURS -
Goblin #1, Goblin #2, Goblin #3 - and 1 to the opponent at life 9 leaves them at 8}`) and
**seq 63** row 1 (`{right now: takes 2 damage - SURVIVES (toughness 4, 3 more kills it)}
… {paying this costs you 1 life … you would be at 2}`). Both annotations are correct,
complete and ignored, and seq 63's cost the game. There is nothing to fix in the *text*;
what is missing is any consequence of a THEIRS-none / SURVIVES row — it is offered with
the same weight as a killing one. Flagging for the engine seat because it is the same
shape as the HIGH above (correct facts, no ranking).

### MED — one `wall_miss`: 900 s wall, `curl=28`, empty body
`…vs-ai_baka_deck126.jsonl` **seq 27**, t14 Main 1, `latency_ms: 900024`,
`transport: "curl=28,http=0,empty=1"`, `fallback: "wall_miss_unrecorded"`,
`chosen_text: "<refused: wall_miss_unrecorded>"`. Recovered cleanly at seq 28
(`recovery`, `recovers_seq: 27`). 1 of 367 replies. curl 28 is a client-side operation
timeout, not an HTTP error — the seat waited the full 15-minute wall and got an empty
body. Cross-deck; for the engine seat to census.

### LOW — runaway post-PLAN prose clusters on the misplayed windows
20 of 367 replies had `post_plan_overrun > 0` (text past the PLAN line, against "Write
nothing else"), the largest **11,302 bytes** at `deck162` seq 63. Three of the four
deciding misplays sit inside that set (`deck162` seq 60/61/63, `deck152` seq 17/21,
`deck126` seq 22/31). The spirals read as the model arguing itself out of the annotation
it just quoted. Correlation, not a mechanism — offered as a signal for the loop, not a
fix.

---

## 3. Guide verdict: **KEEP as is**

`projects/mtg/bin/Res/ai/baka/deck130_strategy.txt`, 70,927 B — the top of the 41–71 KB
pool band. No edit; no `strategy.txt` written.

Why this is a success verdict and not a shrug:

1. **Every deciding misplay this wave violates a rule the guide already states, in the
   strongest form it has.** Priority entry #1 is *"STARSTORM … It answers a board;
   nothing else here does. AND IT GOES FIRST IN THE TURN"* (:372-380). Entry #4 is
   *"RORIX … or SIEGE-GANG COMMANDER … but never into a board where Starstorm is still
   the card you want to cast; sweep FIRST, deploy after"* (:398-399). Rule #2 step 0b
   carries the TAKE-IT test, the SURVIVAL-FIRST override, and the two carve-outs
   (:114-145). The 123 loss is the guide's **own worked example** (:376-388, Thraben
   Doomsayer beside Intruder Alarm) replayed almost verbatim, down to the seat writing
   *"use Stone Rain or Molten Rain to remove … if needed"* against the guide's recorded
   *"use Hammer to kill Thraben Doomsayer if needed"*.
2. **The most recent addition to that rule fired 0 of 51.** The mandated
   `PLAN: Starstorm: THEIRS <n>; YOURS names Rorix/Commander <y/n>; <cast / not this
   window>` line (:145-150) appears in zero replies across all six games. More prose at
   the same seam has now been shown not to bind.
3. **The gates that ARE simple counts did hold.** Land destruction respected both gates
   in 12 of 16 casts (violations: `deck146` seq 41 at life 8; five late casts vs 6–7
   opponent lands in the game it won). The Talisman life-pricing clause, the
   `[NAMED BY THEIR …]` clause and the Hammer recursion were all read and obeyed
   (`deck146` seq 62/63, `deck125` t42–44). The guide's obeyed rungs are the ones the
   render *ranks*; the ignored rung is the one it does not.
4. There is no room. Adding to a 70.9 KB guide at the band ceiling would force cuts
   elsewhere to pay for text at the exact seam where the previous payment bought nothing.

The lever this wave points at is section 2's second HIGH item — an engine-side ranking on
the cast row — not deck130's prose.

## 4. Proposals

None. No `general-proposals.md`, no `skill-proposals.md`. The general guide was not
implicated in any deciding decision, and the skill already carries the marker /
shipped-but-inert / verify-FIRED and harness-blocked-compliance material this wave's
evidence would restate (wave59/strategy-writing-skill.md:2750, 4116-4131, 4423-4431).

## What I did NOT check

- The opponents' own seat translogs, except deck123's board as rendered into deck130's
  prompt — I did not confirm from the deck123 seat file how the 21 Doomsayer activations
  were sequenced engine-side, or whether the loop hit a cap.
- Whether Siege-Gang's Goblins were the only untap source that started that loop (the
  combo is self-sustaining once a creature enters; I did not test the counterfactual).
- The lane-{L,M,N,O,P,Q} predictions — that is the engine seat's adjudication, not mine.
- The full 158-record deck125 win beyond its opening, its land-destruction casts and its
  t40–44 finish; and the `bottom` (mulligan-to-6) decision there beyond confirming it
  kept a two-land six.
- `hold_windows_skipped` accounting (19/23/29/36/57/205 across the six games) — I did no
  prefix-match HOLD census.
- Any Scryfall cross-check beyond Thraben Doomsayer (primitive **matches** Scryfall:
  `{1}{W}{W}`, `{T}: Create a 1/1 white Human creature token` — the free tap ability is
  correct, so the Intruder Alarm loop is a real card interaction, **not** a card-script
  bug).
