# wave-64 deck125 review — Revelation Fracture (Modern UW Control)

Seat logs: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-001533/*deck125-0x*.jsonl` (binary `dab6a6cba`).
Six games, all natural, 0 crashes, 0 hangs. Record **2-4** (wave 63: 4-2). **708 model decisions
(wave 63: 1033), 0 fallbacks, 0 re-asks.** Median latency 3.0 s (125v126) to 20.3 s (125v146).

The record fell and the decision count fell by a third. Neither is a regression signal: three of the
four losses are matchup and variance (below), and the decision count fell because the wave-64
render fixes worked — the hold row is now taken, and two of the three losses were games in which
the seat had no mana and nothing to answer.

**The wave-63 guide edition is the live one** (`cmp` against `wave63/deck125/strategy.txt`:
identical, 70985 bytes). Every one of its five edits paid, measurably — see §3.

## 1. Game by game

| game | result | turn | end life | the decision that decided it |
|---|---|---|---|---|
| 125v162 | **LOSS** | 19 | 0 / 20 | **seq 72 + 73** — at 12 life holding 10 cards with 7 open mana and both Final Judgment and Path to Exile castable, answered `Cast nothing right now` in main 1 and again in main 2. The cleanup at **seq 74** billed 3 discards x Liliana's Caress = **6 of its 12 life**. Dead at seq 97 two turns later. See HIGH-1. |
| 125v130 | **LOSS** | 30 | 0 / 20 | Not a decision. Dedicated land destruction (2 Dwarven Blastminer, Molten Rain, Lay Waste) removed six lands; the seat held Fall of the Gavel x3, Cancel, Dream Fracture x2 and **never once had 3 untapped sources**. 11 asks in 30 turns, all land drops and cleanup discards. Nothing to classify. |
| 125v152 | **LOSS** | 17 | -3 / 20 | Not a decision. Correct sequencing throughout (Essence Scatter on Intrepid Adversary seq 5, Fall of the Gavel on Elite Spellbinder seq 9, Final Judgment exiling 4 at seq 11), then flooded: at **seq 13**, turn 16, life 3, its five-card hand was **four lands and a Cancel with no legal target**. Variance. |
| 125v146 | **LOSS** | 21 | -3 / 20 | Sequenced well (Path seq 6, Cancel seq 10/16, Supreme Verdict seq 12, Lightmine Field seq 23, Final Judgment seq 24) but drew no Staff of Nin all game and so had **zero clock**; 146 rebuilt through three sweepers. Deck-vs-deck, not a misplay I can cite. |
| 125v123 | WIN | 45 | 61 / 0 | Clean. Elixir recursion five times (seq 18/28/68/94/106), Staff pings closed it. |
| 125v126 | WIN | 83 | 44 / 0 | **The wave-63 HIGH-1 failure did not recur.** This is the game that decked itself from 70 life last wave; this wave the library never fell below 26 and the seat won. Emrakul attacked a planeswalker at seq 259 (see §2 PASS notes). |

Corpus-wide the library **never dropped below 23 cards** in any of the six games (max X ever
offered: 19). The wave-63 deck-out class had no window to recur in and did not.

## 2. Engine / interface / card items

### HIGH-1 — the pass row is the only row in the window that is not priced, and under a discard punisher its price is the game
`1788671742-ai_baka_deck125-0x55b97d0742e0-vs-ai_baka_deck162.jsonl`, **seq 72** (main 1) and
**seq 73** (main 2), turn 17, life 12, hand 10, 7 untapped sources.

The window prints, correctly and in full:
- the `DISCARD PUNISHERS on the battlefield: theirs - Liliana's Caress. Every card YOU discard
  costs you 2 life to theirs ... and on the CLEANUP step, where a hand over its maximum size
  discards down to it with no choice to decline` paragraph;
- `Your hand (10 cards)`;
- and on row 2, on the Sphinx's Revelation cast row, a fully computed
  `CLEANUP PRICE at X=4: that leaves 13 cards in hand against a maximum hand size of 7, so unless
  you spend cards before then the cleanup step forces up to 6 discards you cannot decline, and the
  opponent's Liliana's Caress punishes every discard for 2 life each`.

So the engine **already computes this exact figure**, on the row that would make it worse. It does
not compute it for the hand the seat is holding *right now*, and it prints nothing at all on:

```
4. Cast nothing right now (combat comes next this turn)
5. Hold priority: pass now, and do not ask me again ...
```

One window later the same engine prints the number to the byte, at the step where no action
remains — `1788671742-...-vs-ai_baka_deck162.jsonl` **seq 74**:

```
Cleanup step (CR 514.1): your hand has 10 cards and your maximum hand size is 7, so you must
discard exactly 3 cards now. The opponent's Liliana's Caress punishes every discard for 2 life
each, so these 3 discards cost you 6 life - you would be at 6. You cannot decline the discard;
you choose only WHICH cards go.
```

Counts over the seat's six games: **18 windows** carried a `DISCARD PUNISHERS` paragraph, a hand
above 7, and a pass row. **0 of 18** priced the pass row. **18 of 18 were answered with a pass**
(`162` seqs 60-70, 72, 73, 92-96). This is a strict subset of a wider shape: 138 windows had a
hand over 7 and a pass row; the cleanup cost is unpriced on all of them (it is only free of life
cost when no discard punisher is out, which is why the punisher subset is the one that matters).

Fix shape, and it is a move of an existing string rather than a new computation: the cleanup
sentence at seq 74 is already generated from `hand size`, `max hand size` and the punisher rate.
Emit its forecast form on any window on the controller's own turn whose hand exceeds the maximum —
beside the pass row, where declining is the act that incurs it. Something of the shape
`{passing costs you nothing now, but your hand is 10 against a maximum of 7: the cleanup step this
turn forces 3 discards you cannot decline, and their Liliana's Caress bills 2 life each = 6 - you
would be at 6}`. The `CLEANUP PRICE` emitter on the Revelation row is the precedent and probably
the code to reuse.

Note the interaction that makes this HIGH rather than MED for this deck: the deck125 guide's
DRAW-IS-DAMAGE and DISCARD-IS-DAMAGE stops are *correct* and were obeyed perfectly this corpus
(the seat declined Revelation at seqs 89/94/96 under `no listed X leaves you alive` and rewrote its
PLAN to say so). Obeying them is what drives the hand to 10-11 cards. The render prices the
forbidden action and not the mandated one's side effect.

### MED-1 — `(this ask has no pass row)` is now truthful; the residual 22 are all correct
Wave-63 HIGH-2 counted 680 false occurrences in 843 windows. This wave, counting the **rendered
option rows** (not the log text): **0 contradictions** in 708 windows. 684 windows offered a real
pass row and 556 carried the true `the LAST row of this menu declines: it is a real answer, not a
fallback`. The 22 remaining `this ask has no pass row` strings are on windows that genuinely have
none — target-choice asks (`162`-side seqs 146/157/165), damage-order and multi-pick asks
(`126` seqs 272-277, 294-299), and the X menu (`126` seq 307). **F2 is fixed for this seat.**
Filed as MED only so the count is on the record; there is nothing to repair.

### MED-2 — 58% of this seat's decisions are still no-ops, and the inference spend is the cost
408 of 708 decisions were `Cast nothing right now` / `Hold priority` / `pass` (wave 63: 53%).
Per game: `162` 73%, `126` 64%, `123` 40%, `146` 28%. 125v126 alone spent **288 no-op decisions
across 83 turns** on a board it never lost. The hold row is working far better than it was
(below), but a creatureless control seat is still asked ~7 times a turn on the opponent's turn
with an unchanged board. The wave-63 suggestion stands: when the
`[you declined this exact list N times already this turn]` tag fires, stop offering the plain
`Cast nothing right now` row — at that point the hold **is** the decline, and offering the weaker
pass first is what splits the answer.

### LOW-1 — Glacial Fortress's tapped bracket names the condition, not the fix
`...vs-ai_baka_deck130.jsonl` seq 21, turn 21:
`[enters TAPPED - it makes no mana this turn (you control no Plains and no Island)]`. True, and
verified against the primitive. The seat was holding no Plains or Island, so the bracket is a
statement about a board it cannot change; noted only because a reader could take "you control no
Plains" as advice to hold the land for one. It correctly took the drop both times.

### PASS notes on the wave-64 lanes, from this seat's windows (adjudication is the engine seat's; these are my counts)
- **AI F4 (planeswalker attack targets)** — the seat's single `attackers` window,
  `...vs-ai_baka_deck126.jsonl` **seq 259**, turn 51, rendered
  `A1. Emrakul, the Aeons Torn (16/15) ... W1. Sorin, Lord of Innistrad [planeswalker] [15 loyalty
  left: combat damage removes that many counters, and it dies at 0]` and the seat answered with the
  `A#>W#` grammar (`Emrakul, the Aeons Torn -> Sorin, Lord of Innistrad`). Works, first try, N=1 of 1.
- **AH F10 (compulsory draw step)** — 71 `DRAW FORECAST` lines, **71** carrying
  `This draw step is COMPULSORY`; 71 `DRAW PUNISHERS` paragraphs, **71** carrying
  `Your DRAW STEP is COMPULSORY`. No reply in the seat states a plan of holding to avoid a draw
  step. 71 of 71.
- **AJ prediction 8 (enters-TAPPED next-turn value)** — 26 `[enters TAPPED ...]` brackets, **26**
  ending with `it taps for mana from your next turn on`. Outcome half: the seat declined
  **0 of 84** land drops this corpus, against **44 of 44 declines being tapped rows** in wave 63.
  Decisive.
- **AH F1 (library ceiling)** — **UNTESTED at this seat.** 278 `best X for this cast` rows; the
  smallest library under one was 23 and the largest X offered was 19, so the ceiling condition
  never arose and `LIBRARY CEILING:` rendered 0 times. What *did* render is the price extension:
  18 rows carried a `NET` life verdict, and at `162` seq 89 the marker read
  `[<- best X for this cast: X=6 - largest affordable X ... but NET -14 life for this cast puts you
  at -8; this KILLS you. X=3 is the largest listed X whose NET (-5) leaves you alive, at 1]`. The
  seat declined. This is the wave-63 HIGH-1 marker made honest.
- **AH F2 second order (hold-take rate)** — 245 windows carried the
  `[you declined this exact list N times already this turn]` tag; the hold was taken on **41**
  (16.7%), against wave-63's 18 of 390 (4.6%). Predicted rise confirmed. 61 holds overall.
- **AG F6/F7/F8/F9** — **UNTESTED at this seat, structurally.** deck125 is creatureless apart from
  Emrakul; it received **0 `blockers` decisions and 0 `CHOOSE_MODE` menus** in six games. No window
  of any of those four shapes arose. Do not read this as evidence either way.

### Card facts verified against the primitives (all agree; Scryfall queried where the render mattered)
`Liliana's Caress` (`mtg.txt:67351`, `auto=@discarded(*|opponenthand):life:-2 opponent`),
`Underworld Dreams` (`:125822`, `@drawfoeof(player):damage:1 opponent`),
`Howling Mine` (`:55935`), `Sphinx's Revelation` (`:110851`, `auto=life:X && draw:X`),
`Elixir of Immortality` (`:34775`), `Staff of Nin` (`:112425`),
`Ob Nixilis, the Hate-Twisted` (`planeswalkers.txt:2684`, `counter(0/0,5,loyalty)`, static
`@drawfoeof`, `-2` destroy). Scryfall returns loyalty 5 and `−2: Destroy target creature. Its
controller draws two cards.` — **the primitive and Scryfall agree exactly**; the render's
`2 life per card drawn` (Underworld Dreams 1 + Ob Nixilis 1) and `(loyalty 4, 3 left)` are both
correct. Nothing to reconcile.

## 3. Guide verdict: **EDIT** — `wave64/deck125/strategy.txt` written (70901 bytes, 829 lines, 0 U+FFFD, `#HINT` count unchanged; live guide 70985, pool band 41-71 KB)

First, the part that is a **KEEP** result and should be read as one. Every wave-63 edit paid, and
the numbers are not close:

| wave-63 edit | wave-63 | wave-64 |
|---|---|---|
| E1/E2 library ceiling above the largest-X absolute | decked itself from 70 life (125v126) | library never below 23; 125v126 **won** |
| E3 Elixir outranks the Staff ping | 2 activations, 3 collisions lost to the ping | **15 activations**, no collision arose (library never ≤16) |
| E4 take the tapped land | 44 of 44 land drops declined were tapped rows | **0 of 84 declined** |
| E5 take the hold row | 18 of 390 (4.6%) | **41 of 245 (16.7%)** |

The three edits below are the minimum the corpus forces. Two of them **remove claims this corpus
falsified**, which is the only reason the third fits inside the band.

**E1 — the largest-X marker's description was falsified; corrected, and it shortens.**
before:
> `THE MARKER ON OPTION 1 DOES NOT READ YOUR LIBRARY. "[<- largest affordable X ...]" ... are computed from your MANA alone ... The ceiling above is yours to apply; nothing on the menu applies it for you.` (plus the `THE OTHER THING THAT CAN LOWER IT` bullet, which repeats that the marker `does not read the NET tail printed on its own row`)

after:
> `THE MARKER ON OPTION 1 NOW PRICES THE CAST, AND IT NAMES YOUR ANSWER ... "[<- best X for this cast: X=6 ... but NET -14 life for this cast puts you at -8; this KILLS you. X=3 is the largest listed X whose NET (-5) leaves you alive, at 1]" (125v162 seq 89). READ IT TO THE END. When it names a surviving X, announce THAT X and no larger. When it says no listed X leaves you alive, answer "Cast nothing right now" and do not open the menu at all. The library ceiling above still binds first and is still yours to check.`

Paid by **125v162 seq 89, 94, 96** (18 NET-carrying rows). Both sentences were true of the wave-63
binary and are false of this one: the marker now reads the library, the draw price and the cleanup
price, and names the surviving X. A guide that tells the pilot a live verdict is absent will get
the verdict ignored. The library-ceiling rule itself is untouched — it was never exercised this
corpus and its receipt is not disturbed.

**E2 — the tapped-land teach's receipt was falsified; compressed to a settled rule.**
before: `THE ONE ROW YOU KEEP DECLINING IS THE TAPPED ONE. Every land you passed this corpus - 44 of them, 29 in 125v146 alone (seq 148 ...) ...` (7 lines of argument)
after: `TAKE EVERY LAND DROP, TAPPED OR NOT - this is settled and needs no re-deriving. The tapped row now prints its own residual value ("it taps for mana from your next turn on") and you took all 84 land drops offered this corpus, 26 of them tapped.`

Paid by **0 of 84 declines, 26 of 26 tapped rows taken**. The behaviour is fixed and the render now
supplies what the guide was supplying (lane AJ shipped the next-turn sentence). Keeping seven lines
of argument against a decline rate of zero is the guide arguing with itself. The `WHICH LAND`
paragraph below it, which is about *choosing between* rows and was not falsified, is untouched.

**E3 — the new teach: the hand you are ALREADY holding.** Inserted at the end of the existing
`THE DISCARD-IS-DAMAGE STOP` section:
> `YOUR HAND IS ALREADY OVER SEVEN: THE STOP ABOVE DOES NOT COVER IT, AND SPENDING IS THE ANSWER. The stop above only stops you ADDING cards; it says nothing about the hand you are already holding, and holding is exactly what it makes you do. So read "Your hand (N cards)" on every window while a DISCARD PUNISHER is printed. If N is above seven on YOUR turn, the cleanup step at the end of this turn bills you 2 life for every card above seven, you cannot decline it, and no row on the menu prints that figure - only the cleanup ask does, one window too late to act on. While N is above seven and a discard punisher is on their line, "Cast nothing right now" is the one row you may not take: in your own main phase cast one castable card for each card you are over - a sweeper, a Path, anything with a legal target - even at a target you would otherwise hold for. At 125v162 seq 72 and seq 73 you were at 12 life holding 10 cards with 7 open mana and both Final Judgment and Path to Exile on the row list; you answered "Cast nothing right now" twice, the cleanup at seq 74 billed 6 of your 12 life, and you were dead at seq 97 two turns later. Every card you spend before cleanup is 2 life you keep.`

Paid by **125v162 seqs 72, 73, 74** and the 18-window count in HIGH-1. This is the one edit that
adds a teach, and I want to be explicit about why it is not the thing the brief warns against
(prose against a contradicting surface): the surface here does not contradict the teach, it is
**silent** — the punisher paragraph and the hand count are both true and both printed, and only
the arithmetic joining them is missing. The guide can carry that join until HIGH-1 lands, and
should then be re-cut. It is also not a general rule dressed as a deck rule: this deck is the one
whose two standing stops *cause* the over-size hand.

**Budget:** E3 adds ~1.1 kB; E1 and E2 return ~1.0 kB, and the hold-receipt paragraph's stale
numbers (`390 windows ... you held on EIGHTEEN`, `125v126 ... 68 turns and 511 decisions`) were
refreshed to this corpus (`245 ... 41 ... 150 of them in 125v126`) and tightened for the rest.
Net **-84 bytes** against the live guide.

## 4. Optional proposals

**None.** No `general-proposals.md`, no `skill-proposals.md`.
- The general guide is not loaded at runtime (wave-63 synthesis), and HIGH-1 is an engine item in
  any case — the fix is to move a string the engine already emits, not to add prose one layer up.
- The skill lesson my E1/E2 embody — *a receipt that the engine has since fixed is a claim the
  guide must retract, not carry* — is the substance of amendment 14 (`UNCONDITIONAL NUMERIC
  TRIGGERS AGE BADLY`) and of the retraction discipline already in the skill; E3's lesson (*a stop
  that forbids an action creates a state, and the guide owes a rule for the state it creates*) is
  the closest thing here to genuinely new, but I have one game's evidence for it and the brief's
  bar is a decision the corpus shows going wrong across more than one seat. Left for a seat that
  can pay for it.

## 5. What I did NOT check

- **The opponent seats' translogs.** Read only the six `deck125` seat files. The 162/130/152/146/
  123/126 sides of these games, and the opponents' decision quality, were not traced. Where I say
  "the opponent cast X" I am reading it out of my own seat's GAME LOG.
- **The lane predictions as an adjudication.** The brief assigns that to the engine seat. My §2
  PASS notes are counts over *my seat's* windows only, and three of the five lanes (AG entirely,
  AI's F3/F12/F14, AJ's F13/E9/E14a) had either no window at this seat or no surface I can see
  from a translog. AG's F6/F7/F8/F9 are UNTESTED here for a structural reason worth the engine
  seat's attention: **this seat had 0 `blockers` decisions and 0 `CHOOSE_MODE` menus in six games**,
  so it can contribute no evidence at all to the four blockers/modal items. Do not read my silence
  as a pass.
- **`wave64/lane-AI.md` and `wave64/lane-AK.md` in full** — I read the brief's summary of them and
  lane-AI's F4 claim, which I could test; I did not read AK's Codex review or lane-AI's F3/F12/F14
  sections closely, and I adjudicated nothing from either.
- **F3 (the pathway/MDFC main-phase-consuming bug).** 155 windows in this seat printed the
  `PLAY THIS AS A LAND and USES YOUR LAND DROP` sentence, but deck125 runs no MDFCs and I saw no
  such row actually offered; I did not verify the fix. The one shape that *looked* like F3 —
  125v130's 11 asks in 30 turns — I chased and **refuted**: the seat had 0-1 untapped sources for
  most of the game under four land-destruction effects, so the missing windows are missing
  legally. Recording the refutation so nobody re-chases it.
- **`src/`, the binary, and any game run.** Nothing under `bin/Res` or `src/` was modified, no git
  command was run, and wagic was not launched. I did not open `AIPlayerGPT.cpp` to locate HIGH-1's
  emitter; the `CLEANUP PRICE` and `Cleanup step (CR 514.1)` strings are cited from the rendered
  prompts, and the engine seat should find the emitter rather than trust my fix shape.
- **The deck125 guide's card facts wholesale.** I verified the seven cards §2 lists, which are the
  ones my edits and my HIGH item depend on. The rest of the guide's card claims were not re-checked
  this wave.
- **The 138-window wider shape under HIGH-1** (hand over 7, pass row, *no* discard punisher). I
  counted it but did not judge whether pricing the cleanup there too is worth the bytes, since
  without a punisher the cost is cards rather than life.
