# Deck 44 (UB Faerie Tempo) — Wave 5 review

Corpus: `matchups-20260713-145947/`, deck44 seat (deck44 was **deck0 in all 6 games**).
**Record 2-4** (down from wave-4's 7-5 across 12 games; this run is 6 games, one per matchup).
- WON vs135 (snow/simic), vs131 (izzet spells)
- LOST vs133 (mono-black), vs110 (affinity), vs140 (WU control), vs109 (mono-red aggro)

Citations use `eEEEE sN` where `eEEEE` = last 4 digits of the jsonl start-epoch, `sN` = seq:
`e2790`=vs135 W, `e2792`=vs133 L, `e5169`=vs110 L, `e8528`=vs140 L, `e0437`=vs109 L, `e1248`=vs131 W.

Severity: **P0** = game-losing, observed; **P1** = large play-quality cost; **P2** = smaller.

---

## Harness-defect sweep (run first, per skill)

- **DESYNC (parser mis-grab): 0 across all 6 games.** The head-int of every `reply` equals the
  recorded `choice`. Head-first parser is clean, per engine-context. No misplay here is a desync.
- **ENGINE NO-OP: none observed.** Every chosen cast that mattered shows `hand -> stack` and spends
  mana; no chosen-many/happened-never loops.
- **NUMERIC-OFFSET / X-menu trap: N/A.** Deck44 has no X-spells; no numeric menus in the corpus.
- **REPRESENTATION check (the deciding one):** the pending spell IS derivable - see F1. The counter
  option is offered and the log tail shows the pending spell. So F1 is **not** a pure invisibility
  defect the way wave 4 framed it; it is now a comprehension + intent-binding failure on top of a
  still-suboptimal representation (stack absent from CURRENT SITUATION; option unlabeled).

---

## F1 (P0) - Counterspell seam is ALIVE now, but the pilot fires it 2 of 27 times

The deciding lever for every loss, and the diagnosis has **moved** since wave 4.

**Tell audit (scripted over all 6 games, per skill):** the counter option (`Cast Counterspell` /
`Cast Arcane Denial`) appeared in **27 windows**. In **27/27** a live pending spell existed - the
last `hand -> stack` game-log line with **no** `stack ->` after it. The tell holds **absolutely**
(27/27), so it is safe to state as an absolute. The pilot **fired a counter in only 2 of the 27**
(7%). Wave 4 reported counters firing 0/12 games - the seam is no longer dead, but obedience is
still near-zero.

Two failure mechanisms, both proven verbatim in the replies:

**(A) Hallucinated resolution - "no spell on the stack / it already resolved" while a spell is
pending and the counter is offered.** The core prompt AND the wave-4 guide both carry the tell
("the thought 'there is no spell on the stack' is always false while the option is listed"); it is
**losing**.
- `e1248 s3` (vs131, T4 opp turn): pending = `Opponent's Young Pyromancer: hand -> stack` (last log
  line, nothing after). Options `1. Cast Arcane Denial / 2. Cast Counterspell / 3. Cast nothing`.
  Reply: *"no spell is currently on the stack for me to counter (the game log shows the spell moved
  to the battlefield)."* It moved nowhere. Young Pyromancer is on the guide's must-counter list.
- `e5169 s30` (vs110, T15): pending = `Cranial Plating: hand -> stack`. Reply: *"no spells on the
  stack to counter."* Cranial Plating is on the must-counter list; it resolved and killed deck44.
- Same misread: `e1248 s34` (Guttersnipe pending), `e5169 s10`.

**(B) Plan-says-counter, picks-Cast-nothing (intent betrayal - NOT a desync; head-int == choice).**
The core prompt's "never describe an action and then choose Cast nothing" is losing here.
- `e8528 s26` (vs140, T20): phase line *"Main phase 1 | It is the opponent's turn"*, mana
  `{u}{u}{u}`, two Arcane Denials in hand, two 4/4 Bladecrafters on board, pending = `Wrath of God:
  hand -> stack`. PLAN: *"I must counter it immediately... I will cast Arcane Denial to counter
  Wrath of God."* Options `1. Cast Arcane Denial / 2. Cast nothing`. Chose **2 (Cast nothing)**.
- `e8528 s49` (vs140, T33): identical - PLAN *"Countering this spell is the absolute priority... I
  will cast Arcane Denial to counter Wrath of God"*, chose `3. Cast nothing`.

**It CAN fire when neither mechanism strikes:** `e5169 s16` countered Galvanic Blast; `e8528 s19`
countered a Wrath of God ("board wipe that will destroy my Archmage and Obyra"). The capability is
intact; the seam fails on comprehension, not ability.

**Missing from the wave-4 must-counter list - the SWEEPERS that lost the games.** The wave-4 list
named creatures/equipment (Master of Etherium, Cranial Plating, Guttersnipe, Young Pyromancer,
Steel Overseer) but **no board wipes**. The biggest loss (vs140) was 3-4 resolved **Wrath of God**
plus Pyroclasm and Black Sun's Zenith erasing the board repeatedly. `Cyclonic Rift` (vs131, bounces
the whole board) was also passed twice (`e1248 s13, s18`). Sweepers belong at the TOP of the counter
list - one resolved sweeper undoes every creature deck44 deployed.

**Routing.** Fact is visible -> the real fix is REPRESENTATION, upstream (this is wave-4 F1
recommendation #1, still unimplemented): surface the stack in CURRENT SITUATION (`On the stack (you
may respond): Opponent's Wrath of God {2}{w}{w}`) and label the counter option with its target
(`Cast Counterspell - counter Wrath of God`). Labeling also attacks mechanism (B): "Cast nothing"
is far less attractive than "Cast Counterspell - counter Wrath of God." Guide-layer stopgaps this
wave: sweepers to the top of the counter list; a falsity-fact quoting the two exact wrong thoughts;
tie the counter to plan/number binding.

---

## F2 (deck construction, persists from wave-4 F2) - 6 reactive cards clog a beatdown deck

Both WINS came from **racing with bodies** (Miscreants, Archmage, Obyra), firing ~zero useful
counters. Both control/affinity losses are where counters would have mattered and the seam failed.
The two draw-dependent losses show the clog directly:
- `e2792` (vs133): kept Island x3 + Faerie Miscreant + Counterspell x2 + Arcane Denial, drew more
  Islands, **never found black mana**; cast only Miscreant + 2x Scion of Oona while Phyrexian
  Obliterator ground it out. Counters sat uncastable ({u}{u}) in hand.
- `e0437` (vs109): fast mono-red race; deck44 dealt ~4 damage in 13 turns while dying 20 -> -5.

The deck runs **6 reactive cards** (2 Counterspell + 2 Arcane Denial + 2 Go for the Throat) in a
deck whose plan is "get them dead before turn 10." Given F1, the four counters are near-dead weight
that produces the trap hands in the color-screwed losses. **Decklist flag for the user:** the
counters are the weakest cards as integrated; trimming toward more cheap flyers (or fixing the
manabase - 16 Islands / 10 Swamps against a black-heavy spell suite) helps more than any guide
line. Residual, not the guide's to fix with louder prose.

---

## F3 (P1) - Mulligan seam now active, and it shipped a keepable hand miscounting a defender

Wave 4 saw 0 mulligans; this corpus has them, so the seam matters now.
- `e0437 s0` (vs109, LOSS): opening = Oona's Gatewarden {u/b} (2/1) + Swamp x3 + Arcane Denial +
  Counterspell + Go for the Throat. Reply: *"This hand has 3 lands but no creatures to cast."* -
  **Oona's Gatewarden IS a creature.** The pilot discounts it because it is a DEFENDER (can't
  attack) and the guide's identity hammers *attacking* flyers. It mulliganed to 6 into fast aggro
  and had to play the Gatewarden anyway (`s2`). Guide fix: name Oona's Gatewarden (and Sleep-Cursed
  Faerie) as "counts as your creature" in the mulligan line.

---

## Overshoot audit (mandatory)

**Sleep-Cursed Faerie untap rule - OVERSHOT to never-use.** Wave-4 guide (fixing wave-3 double-pay):
*"Pay the {1}{U} untap AT MOST ONCE per turn, and ONLY in your Main Phase 1 on a turn it will
attack... always pass in Upkeep/opponent turn."* Result: the untap was offered **29 times in vs135
alone and PAID 0 times** - the pilot now **never** pays it, in any window. In vs135 that was correct
(Sleep-Cursed woke naturally in 3 turns and attacked at `s47`/`s51`), and no game was lost to
under-untapping. But the fix swung from "double-pay waste" to "never use," and the engine re-offers
the untap every priority window (even Blockers/Combat-damage phases, `e2790 s52/s53`), so the rule
costs ~29 windows of attention per game for a decision that is always "pass." **Action: DEMOTE to
one positive line** (Step-0 compression - obedience is total, no waste remains). The
re-offer-every-window clutter is an engine/representation issue, flagged in notes.

**"You are the BEATDOWN in every matchup" identity - mild misfit vs control.** Not a classic
overshoot (the deck should be the aggressor), but vs140 shows pure beatdown without protecting the
board loses to repeated Wraths. The real defect was F1 (counters not firing), so I keep the beatdown
identity and instead tie the sweeper-counters INTO the race ("a resolved Wrath erases your clock -
countering it IS attacking").

**"Cast a creature every turn - NO EXCEPTIONS" - no harmful overshoot.** Cast-nothing on own-turn
windows (vs135 8/14, vs110 8/12, vs131 10/20) is mostly legitimate (only reactive instants in hand
with no target). Creature-cast execution is sound where bodies exist.

**Counter must-counter list - under-firing, not over-firing.** No overshoot; the fix is F1.

---

## Wave-4 status ledger

**FIXED / holding:**
- Archmage of Echoes deploys reliably (offered 1 / taken 1 in every game it was drawn: vs135 `s46`,
  vs140, vs131 `s46`). Wave-3 "never cast in six games" stays fixed.
- Timeouts: 0/6. Games close.
- Creature-cast execution sound: vs131 Faerie Miscreant taken 4/5 offered; Scion, Bladecrafter,
  Obyra all cast when drawn and mana existed.
- Removal half works: Go for the Throat fires on the right target when one exists (vs135 `s43`
  kills the 7/7 Abominable Treefolk); low take-rate elsewhere (vs110 0/14, vs131 1/20) is **correct
  target discipline** - the opponents' boards were all-artifact (vs110) or empty of nonartifact
  creatures at those windows, not a leak.
- Hybrid-mana refusal: no residual (Oona's Gatewarden cast off a single land, vs135 `s2`, vs109 `s2`).

**PERSISTING:**
- Counter cards die in hand (now F1 - comprehension + intent + missing sweepers + the wave-4
  representation ask still unimplemented).
- 6-reactive-card clog / manabase skew (F2).
- Sleep-Cursed untap rule now over-corrected (overshoot audit).

**NOT EXERCISED:** Faerie Bladecrafter `{right now: life -2}` annotation-as-cost - appeared without
a clear misread this corpus; keep the wave-4 clarifying line (it is working).

---

## Win / loss narratives

- **vs135 WIN (snow/simic), opp -11.** Drew mostly instants early (Go for the Throat with no target
  -> legitimate cast-nothing `s5-s6`, `s12`, `s14`), let Sleep-Cursed wake naturally, GFTT'd the
  7/7 Abominable Treefolk (`s43`), cast Archmage (`s46`), attacked in the air for the win (`s47`,
  `s51`, `s61`). Passed 3 counter windows but won by racing. Slow (~turn 16) because it drew answers
  not threats (variance), not a leak.
- **vs131 WIN (izzet spells), opp -6.** Raced with Miscreant x4 + Archmage + Obyra to lethal by
  turn 19-21. Passed **every** counter window (Young Pyromancer, Guttersnipe, Cyclonic Rift x2 -
  all should have been countered) yet won anyway. Confirms deck44's real win route is the RACE.
- **vs140 LOSS (WU control), opp 26.** The clearest guide/representation-fixable loss. Deployed
  correctly; opponent Wrath'd repeatedly. Countered one Wrath (`s19`), then mechanism (B) struck
  twice (`s26`, `s49`: planned to counter Wrath, picked Cast nothing). Board wiped 3-4x; opp gained
  to 26 via Venser's Journal + Elixir + Staff of Nin over a 35-turn durdle.
- **vs110 LOSS (affinity), opp 21.** All-artifact board neutralised Go for the Throat (correctly
  0/14). Countered Galvanic Blast (`s16`) but missed Cranial Plating (`s30`, mechanism A). Master
  of Etherium grew to 9/5 and killed it. Bad matchup + one decisive counter misfire.
- **vs133 LOSS (mono-black), opp 13.** Color-screwed on Islands (F2); cast only Miscreant + 2
  Scions; Phyrexian Obliterator ground it out. Attribution: draw/deck, not piloting.
- **vs109 LOSS (mono-red aggro), opp 16.** Mulliganed a keepable 7 miscounting the defender as "no
  creatures" (F3), kept a 6, lost the race dealing ~4 damage. Attribution: aggro race + mulligan.

## One-line handoff
The guide's core job (deploy, cast Archmage, race) works - it wins games where it draws bodies.
Every loss traces to the counterspell seam firing 2/27 (F1: comprehension + intent + missing
sweepers + the still-unimplemented representation ask) and to 6 reactive cards clogging a beatdown
deck (F2, a decklist issue). Guide levers this wave: sweepers to the top of the counter list, a
falsity-fact on the two exact wrong thoughts, a plan/number tie, a compressed untap line, and a
mulligan line that counts the defenders.
