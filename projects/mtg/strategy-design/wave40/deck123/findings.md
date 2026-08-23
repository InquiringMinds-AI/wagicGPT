# deck123 "Intruders Of Thraben" (WBU token combo) — Step-0 GUIDELESS review, wave 40

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260823-110407`, binary `465422fcf`, pilot
`qwen36-35b-a3b` (FP8 35B). 6 games, all NATURAL completions. 278 decisions at this seat,
3 fallbacks (1.1%, all `empty_reply` in one 3-record endpoint hiccup, vs146 seq61-63,
latency 2.5s each — not pilot behaviour).

This is the deck's FIRST corpus and it played with **no guide** (core prompt only), so
everything below is the qwen×core interaction with no guide confound.

---

## §0. RECORD CORRECTION AND VERIFICATION (wave-39 HL1)

The wave-40 status file reports **123: 1/6**. Verified against this seat's own `gameend`
records — the adjudicating source — and it is **CORRECT**:

| opponent | turns | my_life | opp_life | won |
|---|---|---|---|---|
| 152 | 8 | 0 | 24 | no |
| 126 | 12 | 0 | 42 | no |
| 162 | 16 | -1 | 31 | no |
| 139 | 14 | -6 | 30 | no |
| 146 | 26 | -2 | 13 | no |
| 125 | 35 | **7** | **70** | **yes** |

**The one win is not a kill.** The vs125 `gameend` reads opponent life **70** and the last
four prompts show `Opponent library: 16 -> 13 -> 4 -> 1 cards`. deck125 (UW draw-go) **decked
itself**. Per owner ruling the record is context only, but the decomposition matters for the
absence table below: **this deck killed nobody in six games.**

---

## §1. PRESERVE LEDGER — FROZEN TERRITORY (Step 0-bis rule 0, unconditional)

Enumerated before any rule was drafted. The guide does not legislate over any of these.

| seam | measured | verdict |
|---|---|---|
| Mulligan | 6/6 `Keep this hand` at seq1; zero mulligans, zero spirals, no 5-or-fewer keeps | FROZEN — guide carries a 3-line confirmatory section only |
| Land drops | a land was played at every `Play <land>` window offered across all 6 games | FROZEN — not taught |
| Attacker declaration | 7 real windows (the 8th, vs146 seq62, is an `empty_reply` fallback, NOT a decision): 6 full declarations of every offered creature, 1 deliberate partial that is **correct** (below) | FROZEN — short confirmatory line |
| Idyllic Tutor reveal pick | 2/2 chose **Intruder Alarm** off 47- and 44-option reveal lists (vs146 seq21, seq49) | FROZEN — one confirming line only |
| Bloodline Keeper transform | offered once (vs125 seq29, at exactly 5 Vampires) and TAKEN | FROZEN |
| Blocker declarations | a block was declared at every blockers window offered (5 windows) | FROZEN |
| Reply protocol | 0 unparsed, 0 stale echo, 0 retracted, 0 answer-replaced | FROZEN |

**The partial attack is the best decision in the corpus and must not be "fixed".**
vs125 seq58, t17: 12 eligible attackers (Lord of Lineage 5/5 + eleven 4/4 fliers), opponent
at 31 life with **0 creatures**. The pilot sent **3**. Its reasoning names the reason:
opponent controls **Lightmine Field** — engine `auto=@each blockers:foreach(creature[attacking]|Battlefield) damage:1 all(creature[attacking]|Battlefield)`, i.e. N damage to each of
N attackers, resolving at the blockers step **before** combat damage. All 12 attacking -> each
takes 12 -> the whole board dies dealing zero. 3 attacking -> each takes 3 -> a 4/4 lives.
13 damage dealt, board intact. **This is correct play, arrived at from the card text.**
A naive "3 of 12 = under-declaration" read would have shipped a loud attack floor aimed at a
seam that is already right (the wave-31 rung-0 failure).

---

## §2. ABSENCE TABLE — what the deck's plan required that NEVER OCCURRED (rule 0b)

Computed per game from the `Your battlefield` line of every prompt.

| the plan requires | games it happened | routing |
|---|---|---|
| Thraben Doomsayer OR Bloodline Keeper on the battlefield | **6/6** | fine |
| **Intruder Alarm on the battlefield** | **3/6** (162, 139, 125) | see below — part DRAW, part **PLAY** |
| combo LIVE (Alarm + a token-maker both in play at once) | **3/6** | — |
| the combo actually RUN to an army (>=6 tokens) | **1/6** (vs125, 14 Vampires) | **PLAY** |
| **a win by combat damage** | **0/6** | **PLAY** (the one win was an opponent deck-out) |

**The Intruder Alarm zero does NOT route to draw.** The deck runs 4 Alarms **and** 4 Idyllic
Tutors — 8 outs — and the Alarm was in hand in 5 of 6 games. In vs146 it was tutored into
hand **twice** (seq21 t9, seq49 t13) and held as two copies until the game ended at t26
without ever being castable: prompts seq67/68 show hand `Intruder Alarm (copy 1 of 2)`,
`Intruder Alarm (copy 2 of 2)` and battlefield `Scrubland x2, Swamp, Isolated Chapel x3`
— **zero blue sources**. The cause is a decision, not a draw: §3.2.

**Routing summary per loss:** 152 -> PLAY (removal held, then run over by t8). 126 -> PLAY
(Tragic Slip wasted twice on 0/3-0/4 walls; no blue). 162 -> PLAY (combo live and abandoned).
139 -> PLAY (combo live and abandoned). 146 -> PLAY (fetched a basic; self-Damnation).
125 -> won. No loss routes to CONSTRUCTION or to ENGINE/REPRESENTATION.

---

## §3. THE DECISION AUDIT

### 3.1 #1 LEAK — the combo is assembled, OFFERED, free, and ABANDONED mid-loop

**Tally, mechanical (numbered option lines only):** across 6 games a
`Create human with Thraben Doomsayer [cost: Tap]` / `Create vampire with Bloodline Keeper
[cost: Tap]` / `Create vampire with Lord of Lineage [cost: Tap]` line appeared in
**43 decision windows**. Taken **29**, **passed 14 (33%)**.

The ability's whole cost is `[cost: Tap]` — no mana, no land drop, usable on either player's
turn. With Intruder Alarm out (`auto=@movedTo(creature|myBattlefield):untap all(creature)`)
each token entering untaps the maker, so the line returns — which is exactly why the engine
keeps printing it. The core prompt guarantees a printed option is takeable now.

**Repro A — vs162, seq30 -> seq31 -> seq33, turn 15, life 1 vs 36.** Same-window control:
at **seq30** the pilot TAKES the activation and writes, verbatim, *"Intruder Alarm will untap
Thraben Doomsayer."* At **seq31** the option list contains **exactly one line**:
`1. Create human with Thraben Doomsayer [cost: Tap]` — the combo re-armed precisely as
predicted, two Intruder Alarms on the battlefield, and Fateful Hour live so every Human
renders `(3/3) (printed 1/1)` against an opponent with **0 creatures**. The pilot answers
**`CHOICE: 0 (pass)`**, reasoning *"I have Intruder Alarm in play, so my creatures won't
untap"* (the exact inversion of what it wrote one decision earlier) and *"I have 0 mana. I
can't ca[st]"* — a mana objection applied to a **{T}-only** ability. At **seq33** the same
line is offered again and it passes again. Two free 3/3s declined in one turn.
*Honest scoping:* this particular game was probably lost regardless (at 1 life the
opponent's Ob Nixilis pings on every draw), so this window is the cleanest REPRO, not the
most decisive instance.

**Repro B — vs139, seq26 -> seq27 -> seq28, turn 12 (opponent's turn), life 12.** The pilot
controls Intruder Alarm, TWO Thraben Doomsayers and a Bloodline Keeper. seq26: takes
`Create human`. seq27: both lines still offered (Doomsayer #2 and the Keeper untapped) ->
**pass**. seq28: everything untapped again (the Alarm trigger has now resolved) -> **pass**.
Facing Brokkos and Nissa at 12 life, on the opponent's turn, it declined two free blockers
that would also have been attackers the following turn. Its own next-turn blocker window
(seq29) is the one it goes 12 -> 4 on. **This is the decisive instance of the class.**

**Repro C — vs125, the WIN, seq25-34 and seq47-61.** This is the positive control: the pilot
DID run the loop — 8 consecutive activations (seq25-28 Keeper, seq29 transform, seq30-33
Lord of Lineage) building the 14-Vampire board that produced the corpus's only real
offense — and then **passed at seq34 with the line still offered**, and passed again at
seq47, seq53, seq60, seq61. The one game where it kept tapping is the one game it built a
board. 9 further activations were left on the table in that game alone.

**The tax table proves this is a BELIEF, not a decision** (wave-39 HL5 / wave-31 HL9).
Ranking all 282 reasoning traces by `reasoning_chars` and reading the LOW-BRANCHING rows
first: the corpus's **#1 and #2 largest deliberations** are both 2-option
`Create vampire with Lord of Lineage [cost: Tap]` windows —
**17,635 chars** (vs125 seq33) and **14,949 chars** (vs125 seq51) — and the #4 low-branching
row is a 2-option `pass` on the same line (13,250 chars, vs125 seq34). Median trace is
7,203 chars. A 17.6k-character deliberation over "tap this creature for a free token, or
pass" cannot be about the choice.

**Layer:** GUIDE (deck fact + procedure over the visible list). The surface is TRUE — the
option is printed with its exact cost — so this is a STRATEGY/deck-fact leak, not perception.

### 3.2 #2 LEAK — Marsh Flats fetches a basic, stranding the blue half of the deck

**Tally:** 6 fetch activations taken. Every one of the 6 option lists offered
`Put in Play with Marsh Flats targeting Underground Sea` **and**
`... targeting Tundra` alongside the basics.

| game | seq | chose | consequence |
|---|---|---|---|
| 152 | 14 t7 | **Plains** | — (died t8) |
| 139 | 5 t3 | **Plains** | Alarm cast only at t11 |
| 139 | 38 t14 | Underground Sea | ok |
| **146** | **7 t5** | **Swamp** | **2 Intruder Alarms + 2 Vision Skeins stranded in hand t9->t26** |
| 125 | 6 t5 | Underground Sea | ok — this is the game the combo ran |
| 125 | 9 t7 | **Swamp** | — |

**4 of 6 fetches took a basic that cannot cast the deck's combo piece.** Verified against
the primitives, the duals are STRICTLY better here, with no trade-off to weigh:
`Underground Sea` is `subtype=Island Swamp` (everything a Swamp does, plus {U}),
`Tundra` is `subtype=Plains Island` (everything a Plains does, plus {U}), both are legal
fetch targets for `Marsh Flats` (`auto={T}{L:1}{S}:moveTo(myBattlefield) target(*[swamp;plains]|myLibrary)` matches by SUBTYPE), and neither carries `tap(noevent)` so both enter
untapped. Only 10 of the 24 lands make {U} (Tundra x4, Underground Sea x4, Arcane Sanctum
x2 — and Arcane Sanctum enters tapped).

**Layer:** GUIDE. Unconditional, countable, board-readable — the strongest possible shape.

### 3.3 #3 LEAK — Damnation cast onto the pilot's own larger board

**vs146 seq67, t17.** Battlefield lines in the prompt read
`Your battlefield (9 permanents listed, of which 2 are creatures)` — Vampire #1 (2/2 flying),
Vampire #2 (2/2 flying) — against
`Opponent battlefield (7 permanents listed, of which 1 is a creature)` — Nadaar 3/3.
The pilot's own PLAN one decision earlier reads *"Untap next turn and attack with Bloodline
Keeper and both Vampires for lethal."* It then chose **`CHOICE: 1 (Cast Damnation)`**,
destroying 2 of its own creatures to kill 1 of theirs, and wrote *"Next turn, cast Intruder
Alarm and build an army"* — with no blue source in play (§3.2). Opponent finished the game
at 13 life.

**Same-game control (the same card, played correctly):** seq68, t19 — `Your battlefield
(7 permanents, of which 0 are creatures)` vs `Opponent battlefield (...of which 2 are
creatures)`. Damnation there is right, and it took it. So the discriminator is proven to be
the creature-count comparison, not anything about the card or the pilot.

The engine already **surfaces the number the rule needs**: both battlefield lines print
`of which N are creatures`. That is the anchor the guide keys to (HL7).

### 3.4 #4 LEAK — Tragic Slip fired where it kills nothing

Tragic Slip is `{B}` instant, engine `auto=ifnot morbid then -1/-1` / `auto=if morbid then
-13/-13` (matches Oracle exactly). Without morbid it kills only toughness 1.

**9 casts. 4 killed the target. 5 did nothing.**

| game/seq | turn | target (as rendered) | morbid | result |
|---|---|---|---|---|
| 152 s6 | 4 | Luminarch Aspirant (2/2) | no | **whiff** -> 1/1 |
| 126 s3 | 1 | Pride Guardian (0/3) | no (turn 1) | **whiff** -> 0/2 |
| 126 s10 | 7 | Overgrown Battlement (0/4) | YES (`Opponent's Pride Guardian died`) | kill |
| 162 s12 | 7 | Fate Unraveler (3/4) | no | **whiff** -> 2/3; the Unraveler then took the pilot 18 -> 1 |
| 139 s3 | 1 | Arboreal Grazer (0/3) | no (turn 1) | **whiff** |
| 139 s17 | 9 | Dryad of the Ilysian Grove (2/4) | YES (`Your Human died`) | kill |
| 146 s4 | 3 | Triumphant Adventurer (1/1) | no | kill |
| 146 s74 | 21 | Spider #1 (2/1) | (irrelevant, T=1) | kill |
| **146 s77** | **25** | **Angel #1 (4/4)** | **no** | **whiff — and Spider #1 (2/1) and Spider #2 (2/1) were on the SAME target list** |

**vs146 seq77 is the sharpest repro** — a same-window control: a target the spell kills
outright was offered next to the one it cannot touch, and the pilot took the big one.

**Representation note:** the option line carries only the static rules text; there is no
live magnitude annotation (`{right now: -1/-1}` vs `{right now: -13/-13}`) the way
Gray Merchant renders `{right now: drains N}`. Adjudicating morbid currently requires reading
the GAME LOG tail — a weak surface by this skill's own rung. See ledger item L-123c; the
guide's primary rule is therefore keyed to **printed toughness on the battlefield line**
(board-readable, no tail reading), with the morbid upgrade as a secondary branch registered
as a stopgap.

### 3.5 #5 LEAK — the two edicts treated as one card

Oracle- and engine-verified, and they differ in **who gains the life**:
- `Tribute to Hunger {2}{B}` — `targetopponent`, `text=Target opponent sacrifices a creature
  of their choice. **You** gain life equal to that creature's toughness.`
- `Devour Flesh {1}{B}` — `targetedplayer`, `dynamicability<!mytgt toughnesslifegain
  targetcontroller!>` — **the player who sacrifices** gains the life.

**Devour Flesh named "The opponent" 5 times.** Measured life swings from the following
records: vs162 s4 opp 20 -> **26**; vs162 s7 opp 26 -> **32**; vs162 s19 opp 32 -> **36**;
vs146 s72 opp 13 -> 14. In vs162 alone the pilot handed the opponent **16 life** with its own
removal, in a race it lost at -1 life. (vs125 s82's 27 -> 55 is not attributable — other
effects resolve in the same window; excluded.)

**Devour Flesh named "Yourself" twice**, both times sacrificing a real body, not a spare
token: vs146 s45-47 (life 16, gave up `Vampire #1 (2/2) [flying]` for 2 life) and
vs125 s62-64 (life 15, gave up `Vampire #3 (4/4) [flying]` for 4 life, off a 17-option list
that included Human tokens).

Note both edicts let the **sacrificing player choose**, so against 2+ creatures they give up
their worst. This is a deck-fact gap, not a perception gap — the option line prints
`{card text: "Target player sacrifices a creature, then gains life equal to that creature's
toughness."}` in full. **Layer: GUIDE**, as a scoped preference + fire-gate, never a
suppression.

### 3.6 #6 LEAK — the Lightning Greaves equip churn (the re-offered-action trap)

`Lightning Greaves` is `auto={0}:equip`, so `Equip with Lightning Greaves targeting X` costs
nothing and the engine re-offers it at **every** priority window.

| game | equip windows offered | equips TAKEN | per turn | reasoning chars spent on equips |
|---|---|---|---|---|
| 126 | 3 | 1 | t11: 1 | 4,849 |
| 162 | 2 | 2 | t15: 2 | 13,733 |
| 139 | 1 | 1 | t13: 1 | 5,858 |
| **146** | **30** | **22** | **t7: 5, t11: 11, t13: 2, t15: 4** | **190,348** |

vs146 seq11-18 and seq24-34 are a ping-pong: `targeting Bloodline Keeper` -> `targeting
Vampire` -> `targeting Bloodline Keeper` -> ... eleven times in turn 11 alone, each a no-op
re-attachment that strips haste and shroud from the previous holder. Three of the corpus's
top-10 low-branching tax spikes are these 1-option equip windows (14,437 / 13,180 / 12,622
chars). At a median 130s per decision this class alone cost roughly 45 minutes of the vs146
game's wall clock. This is the deck110 wave-5 shape exactly, and it gets the three-part
guard: DONE-SIGNAL (`{attached: Lightning Greaves #1}` prints on the holder's battlefield
line), a COUNTABLE brake keyed to unequipped copies, and a NAMED release.

**Layer:** GUIDE (three-part guard) **plus** an engine ledger item — a free re-offered
attach of an already-attached Equipment to the same creature is a genuine no-op the engine
could annotate. See L-123d.

---

## §4. LATENCY / TAX

Median decision latency **130.2 s**, p90 **194.7 s**, max **344.2 s** (n=285). Median trace
**7,203 chars**. The two biggest cost centres are both named above (§3.1, §3.6) and both are
addressed by pre-computing the answer in the guide rather than by any budget change.

---

## §5. ENGINE / REPRESENTATION LEDGER ITEMS

**Wave-40 fixes VERIFIED LIVE at this seat** (coverage fractions over 285 prompts carrying a
board):
- `was COUNTERED` present in **90 / 285** prompts; `resolved and went to the graveyard`
  present in **0**. Wave-39 ledger #3 / Method-headline-10 is **DISCHARGED at this seat** —
  the countered-vs-resolved conflation does not recur here.
- `STOPS all` emissions at this seat: **0** (this seat faced no such window; not evidence
  either way).
- `unreachable`-reason attacker rendering: `all_assignments_illegal` **0**; the 7 real
  attacker windows all produced legal declarations.

**NEW items owed:**

- **L-123a — the token-maker loop has no terminator and no cap.** With Intruder Alarm plus a
  `{T}`-token-maker on the battlefield the engine re-offers the activation indefinitely
  (vs125 seq25-33: 8 consecutive activations, option still present at seq34). Nothing in the
  engine bounds the loop; a pilot that obeys "take it every time" would never pass priority
  and the game could not complete — which collides directly with invariant 00 (a corpus game
  must finish). This is not a request to suppress a legal play: the ask is a *bound*, e.g.
  a soft cap annotation on the option line once the controller's creature count passes a
  threshold, or an engine-side "you have activated this N times this window" annotation so
  the pilot has a countable to stop on. **Until it exists the GUIDE carries the terminator**
  (registered as a stopgap, §6). Repro: vs125 seq25-34.
- **L-123b — `Flip Side with Bloodline Keeper -> DISPLAY TOGGLE only` is dead weight in
  38 / 285 prompts.** It is offered at nearly every priority window from the moment a
  Bloodline Keeper is in play, it changes nothing about the game state, and it sits directly
  adjacent to the real `Transform:backside with Bloodline Keeper [cost: {b}]` line (which
  appears in only 1 prompt). Two near-identical lines, one inert, at the seam that decides
  this deck's best card. Suggest suppressing the display toggle for AI consumers, or at
  minimum ordering it below the declines. Repro: vs125 seq25-45 (present in ~20 consecutive
  windows), vs139 seq33/37/38.
- **L-123c — Tragic Slip's option line carries no live magnitude.** The line prints the
  static text (`-1/-1 ... Morbid - ... -13/-13 instead if a creature died this turn`) but never
  the value that applies *right now*. The engine already computes and renders live
  magnitudes for other cards (`{right now: drains N}`). Asking a `{B}` spell's whole value to
  be recovered from the GAME LOG tail is the weak-tell shape this skill's own rung warns
  about. Ask: annotate as `{right now: -1/-1}` / `{right now: -13/-13 (a creature died this
  turn)}`. Repro of the harm: vs146 seq77 (targeted a 4/4 with a 2/1 on the same list);
  vs162 seq12; vs139 seq3.
- **L-123d — `{0}` Equip is re-offered without a done-signal on the OPTION line.** The
  battlefield line does carry `{attached: Lightning Greaves #1}`, but the option line does
  not say the Equipment is already attached to that creature, and re-attaching to the SAME
  creature is a pure no-op. 22 equips in one game, 11 in one turn (vs146 t11, seq24-34),
  190k reasoning characters. Ask: annotate the option with the current holder, or suppress
  the re-offer targeting the creature the Equipment is already attached to. This is the same
  class as the deck110 wave-5 Cranial Plating finding, recurring on a `{0}` cost where the
  mana brake that usually limits it does not exist.
- **L-123e (LOW, observation only) — Intruder Alarm's untap trigger resolves one priority
  window later than the activation.** vs139 seq27 renders `Thraben Doomsayer #1 ... [tapped -
  cannot attack or block this turn]` and seq28 renders the same permanent untapped, with no
  intervening action. Rules-correct (the trigger uses the stack), but it means a pilot that
  checks "is it untapped yet?" immediately after activating sees the wrong answer. Only worth
  a note if a future seat shows a decision turning on it.

---

## §6. STOPGAP REGISTER

Terminal pass, per line, asking *"would I have written this sentence if the engine were
correct?"*

| guide line | defect it patches | removal condition |
|---|---|---|
| "keep taking it ... until your own battlefield line shows 12 or more creatures, then pass" (the numeric terminator inside Rule 1) | **L-123a** — the engine puts no bound on the re-offered activation, and an unterminated pursuit is both a guide-authoring violation and a game-completion hazard | Delete the "12 or more creatures" clause **in BOTH of its line locations — Rule 1 (L14-15) and the DECIDING SITUATIONS restatement (L141-142); a half-removed terminator is the failure this register exists to prevent** — when the engine ships any countable bound or activation-count annotation on the token-maker option line. Check: `/usr/bin/grep -c 'activated this' <corpus>/*deck123*.jsonl` is non-zero, or the option line carries a cap annotation. |
| "Under the CURRENT turn header in the GAME LOG, a line ending in 'died' is your proof" (the morbid branch of Rule 4) | **L-123c** — the option line carries no live magnitude, so morbid state is only recoverable from the log tail | Delete and replace with "read the `{right now: ...}` value on the Tragic Slip line" when L-123c ships. Check: `/usr/bin/grep -c 'right now: -13/-13' <corpus>/*deck123*.jsonl` is non-zero. |
| "the answer at every re-offer is 0 (pass)" + the unequipped-copy brake (Rule 6) | **L-123d** — the option line has no already-attached done-signal | **Partial only.** When L-123d ships, delete the brake counting; **KEEP** the one-line "equip your best token-maker once" preference — that half is strategy (which creature), not a defect patch. Removal condition for the brake half: the option line names the current holder. |
| the quoted equip-option string in Rule 6 (L74) and in DECIDING SITUATIONS (L156) | **L-123d quoted-string DEPENDENCY** — the rule's re-offer machinery is keyed to a string the ledger schedules for change; if L-123d ships as suppression the trigger stops appearing and the text goes dead | Re-word or delete both quotations when the option line names the current holder, or the re-offer is suppressed. |

No other guide line is explained by an open ledger item.

---

## §7. PER-CLAIM CARD-FACT LEDGER (HL2) — terminal pass over the SHIPPED guide

Every checkable claim in `deck123_strategy.txt`, one row each. Adjudicated against Scryfall
`/cards/named?exact=` **and** `projects/mtg/bin/Res/sets/primitives/mtg.txt`, and every
quoted render string grepped against the corpus `prompt` fields (285 prompts).

| # | claim as written in the guide | adjudicating source | verdict |
|---|---|---|---|
| 1 | Intruder Alarm costs `{2}{u}` | `mana={2}{U}`; Scryfall `{2}{U}` | TRUE |
| 2 | Intruder Alarm: creatures don't untap in the untap step, and every creature entering untaps all creatures | `auto=lord(creature) doesnotuntap`; `auto=@movedTo(creature|myBattlefield):untap all(creature)`; `auto=@movedTo(creature|opponentBattlefield):untap all(creature)` | TRUE |
| 3 | Intruder Alarm is symmetric (applies to their creatures too, and their creature entering also untaps yours) | both `movedTo` lines above, one per battlefield; `lord(creature)` is unscoped | TRUE |
| 4 | Thraben Doomsayer `{1}{w}{w}` 2/2 | `mana={1}{W}{W}`, `power=2`, `toughness=2` | TRUE |
| 5 | Thraben Doomsayer `{T}` makes a 1/1 white Human token | `auto={T}:token(Human,Creature Human,1/1,white)` | TRUE |
| 6 | Fateful hour: at YOUR life 5 or less, every OTHER creature you control gets +2/+2 | `auto=this(controllerlife < 6) lord(other creature|mybattlefield) 2/2`; Scryfall "5 or less life" | TRUE |
| 7 | Bloodline Keeper `{2}{b}{b}` 3/3 flying | `mana={2}{B}{B}`, `power=3`, `toughness=3`, `abilities=flying` | TRUE |
| 8 | Bloodline Keeper `{T}` makes a 2/2 black flying Vampire token | `auto={T}:token(Vampire,Creature Vampire,2/2,black,flying)` | TRUE |
| 9 | Transform costs `{b}` and needs 5 or more Vampires | `auto={B}:flip(backside) restriction{type(vampire|mybattlefield)~morethan~4}` (>4 = 5+) | TRUE |
| 10 | Lord of Lineage is 5/5 and gives OTHER Vampires +2/+2 | `power=5`, `toughness=5`, `auto=lord(other vampire|mybattlefield) 2/2` | TRUE |
| 11 | Lord of Lineage still makes a token each turn | `auto={T}:token(Vampire,...)` present on the back face | TRUE |
| 12 | 2/2 Vampire tokens become 4/4 under Lord of Lineage | 8 + 10; corpus render `Vampire #1 (4/4) (printed 2/2)` (vs125 seq58) | TRUE |
| 13 | Idyllic Tutor `{2}{w}`, searches for an enchantment and puts it in HAND | `mana={2}{W}`; `auto=... target(<1>enchantment|reveal) ... moveto(hand)` | TRUE |
| 14 | Lightning Greaves `{2}`, Equip `{0}`, grants haste and shroud | `mana={2}`; `auto={0}:equip`; `auto=teach(creature) shroud`; `auto=teach(creature) haste` | TRUE |
| 15 | Shroud means that creature cannot be targeted by YOUR spells either | Scryfall reminder text "It can't be the target of spells or abilities" (no controller clause) | TRUE |
| 16 | Tragic Slip `{b}`, -1/-1, or -13/-13 if a creature died this turn | `mana={B}`; `auto=ifnot morbid then -1/-1`; `auto=if morbid then -13/-13` | TRUE |
| 17 | -1/-1 kills a creature whose printed toughness is 1 | arithmetic on 16 | TRUE |
| 18 | Devour Flesh `{1}{b}`; the player you name sacrifices AND that player gains the life | `mana={1}{B}`, `target=player`, `dynamicability<!mytgt toughnesslifegain targetcontroller!>`; Scryfall "Target player sacrifices a creature of their choice, then gains life equal to that creature's toughness" | TRUE |
| 19 | Tribute to Hunger `{2}{b}`; the opponent sacrifices and **YOU** gain the life | `mana={2}{B}`, `target=opponent`, `dynamicability<!mytgt toughnesslifegain targetopponent!>`; Scryfall "You gain life..." | TRUE |
| 20 | The sacrificing player chooses which creature | `notaTarget(creature|mybattlefield)` (not a targeted choice by the caster); Scryfall "of their choice" | TRUE |
| 21 | Damnation `{2}{b}{b}` destroys ALL creatures including yours | `mana={2}{B}{B}`; `auto=bury all(creature)` (unscoped) | TRUE |
| 22 | Vision Skeins `{1}{u}`, EACH player draws two | `mana={1}{U}`; `auto=all(player) draw:2` | TRUE |
| 23 | Underground Sea makes {U} and {B} | `subtype=Island Swamp` (basic land types supply the mana abilities) | TRUE |
| 24 | Tundra makes {W} and {U} | `subtype=Plains Island` | TRUE |
| 25 | Scrubland makes {W} and {B}, no blue | `subtype=Plains Swamp` | TRUE |
| 26 | Arcane Sanctum makes {W}, {U} or {B} and enters TAPPED | `auto=tap(noevent)`, `auto={T}:Add{W}/{U}/{B}` | TRUE |
| 27 | Marsh Flats can fetch Underground Sea and Tundra | `auto={T}{L:1}{S}:moveTo(myBattlefield) target(*[swamp;plains]|myLibrary)` — matches by SUBTYPE, and 23/24 carry those subtypes; corpus offers both lines in 11 prompts each | TRUE |
| 28 | The fetched dual enters untapped | neither Underground Sea nor Tundra carries `tap(noevent)` (contrast 26) | TRUE |
| 29 | Marsh Flats costs 1 life | `{L:1}` in the cost; `text=...Pay 1 life...` | TRUE |
| 30 | "only 10 of your 24 lands make blue" | decklist: Tundra x4 + Underground Sea x4 + Arcane Sanctum x2 = 10; total lands 4+4+4+2+4+4+1+1 = 24 | TRUE |
| 31 | "your cheapest card costs ONE mana" (mulligan satisfiability, HL7c) | Tragic Slip `{B}` MV 1; corpus mulligan prompt prints `Cheapest spell in this hand: Tragic Slip {b} (mana value 1)` | TRUE |
| 32 | Isolated Chapel makes {W} or {B} and can enter tapped | `auto=aslongas(plains,swamp|myBattlefield) tap(noevent) <1 oneshot`; `{T}:Add{W}`/`{B}` | TRUE |
| 33 | Lightmine Field damages each attacker by the number of attackers, before combat damage | `auto=@each blockers:foreach(creature[attacking]|Battlefield) damage:1 all(creature[attacking]|Battlefield)` — fires at the blockers step | TRUE |

**Class sweep:** the two false-fact classes this deck invites — (a) "an edict's lifegain goes
to the caster" and (b) "a fetchland can only get a basic" — were each grepped across the
whole guide, not just fixed at the card that produced them. No other line asserts either.

**Quoted-render-string sweep (HL6):** every substring the guide presents as something the
pilot SEES was grepped verbatim against the 285 corpus prompts. All present, with counts:
`Create human with Thraben Doomsayer [cost: Tap]` 17 - `Create vampire with Bloodline Keeper
[cost: Tap]` 18 - `Create vampire with Lord of Lineage [cost: Tap]` 17 -
`Put in Play with Marsh Flats targeting Underground Sea` 11 -
`Put in Play with Marsh Flats targeting Tundra` 11 - `Equip with Lightning Greaves targeting`
36 - `Transform:backside with Bloodline Keeper [cost: {b}]` 1 - `Cast Intruder Alarm {2}{u}`
23 - `Cast Idyllic Tutor {2}{w}` 34 - `Cast Tragic Slip {b}` 37 - `Cast Devour Flesh {1}{b}`
53 - `Cast Tribute to Hunger {2}{b}` 30 - `Cast Damnation {2}{b}{b}` 31 -
`Cast Vision Skeins {1}{u}` 11 - `Cast nothing right now` 115 - `The opponent (player, life`
58 - `Yourself (player, life` 70 - `are creatures)` 277 - `Your creatures that can attack:` 8
- `Fateful hour` 31. **Zero-hit strings: none.**

Cross-checked against this seat's OWN ledger (wave-39 HL6): no quoted string above is
scheduled for change by L-123a-e. L-123b would remove `Flip Side with Bloodline Keeper`,
L-123c would ADD a `{right now: ...}` tail to the Tragic Slip line, and L-123d would ADD a
holder name to the Equip line — the guide quotes none of those three as anchors (Rule 6
anchors on the phase/seam and on `{attached: Lightning Greaves` from the BATTLEFIELD line,
which no ledger item touches).

---

## §8. FREQUENCY-CLAIM AUDIT (HL4, both directions)

Grepped the shipped guide for `rarely`, `often`, `usually`, `most of the time`, `typically`,
`seldom`, `always happens`, `almost never`. **Zero hits.** Two mechanics are UNEXERCISED or
thinly exercised in this corpus and are written as rule + both outcomes + the board field
that answers the question, with no rarity claim in either direction:
- **Bloodline Keeper's transform** — offered exactly ONCE (vs125 seq29) across 6 games. The
  guide states the trigger (`Transform:backside ... [cost: {b}]` appears) and what to do, and
  makes no claim about how often 5 Vampires assemble.
- **Vision Skeins against a draw-punisher** — the pilot never cast it into one, so nothing is
  known about the frequency. The guide names the trigger (a permanent on their battlefield
  that damages or drains you when a player draws) and gives both branches.

## §9. NULL / UNEXERCISED TEACHES CARRIED INTO WAVE 41 (HL8 clock)

Two lines in the shipped guide are written on a mechanic this corpus did not arm, and each
carries a named arming requirement for the next reviewer:
1. **The Lightmine-Field-style attack throttle** (Combat section). Armed exactly once, and
   correctly (§1). Arming requirement: any game against **deck125**. Next reviewer reports
   the arming count **from the 125 matchups specifically**, not corpus-wide.
2. **The transform line.** Arming requirement: a board with 5+ Vampires — reachable only if
   Rule 1 lands, so it is expected to arm far more often next corpus. If it is still 1-of-6
   after wave 41, the line compresses to a single clause.

---

## §10. WHAT THE #1 SLOT IS, AND WHY (auditable)

Every loss routes to PLAY (§2), so the #1 slot does not need the wave-32 "take it from the
wins" rule. Among play leaks the combo abandonment (§3.1) wins on all three axes the skill
ranks by: it is the deck's ENGINE (the zero column of the absence table), it is the largest
measured leak (14 passes / 43 offered windows), and it is the corpus's #1 and #2
reasoning-tax spikes on 2-option decisions — the belief fingerprint. Its corrective is a
FACT ("`[cost: Tap]` is the whole cost") plus a procedure over the visible list, which is the
shape this executor actually executes.

## §11. THE DECK'S PLAN AND THE ONE PROBLEM THE GUIDE IS BUILT TO FIX

**The plan:** land, token-maker, Intruder Alarm; then tap the token-maker over and over —
every token that enters untaps it — until the board is an army, while five kinds of cheap
removal buy the turns.

**The problem:** the pilot assembles the engine and then stops running it. It reads
"my mana available is 0" as "I can do nothing" at a `[cost: Tap]` ability, so the free half
of the deck goes unspent; and it strands the blue half of the deck by fetching basics with
Marsh Flats when both blue duals are on the same option list. Rule 1 and Rule 2 are the whole
fix; everything after them is card-selection cleanup.
