# Wave-53 lane T — Vita human-play report: heuristic-AI blocking

Owner report (Vita vpk11, engine == master dddb6f62b), his words: *"one was a game
losing block by the heuristic ai, where it could have blocked differently, and likely
won."* Transcript: `psp-work/logs/20260902-vpk11/transcript-1788329701-player_deck7-vs-ai_baka_deck72.txt`,
`#result winner=p1 turn=14 life=2/-2`, `#classification=bad blocking`.

## What the transcript records at the block (in plain words)

Turn 14, human p1 (deck7 "Relentless Growth") attacks with **three** creatures —
Predator Ooze (indestructible, +1/+1 counters), Hungering Hydra (X counters,
`oneblocker`), and Greensleeves, Maro-Sorcerer (a CDA: power and toughness each equal
to the lands p1 controls). The AI seat (Baka, deck72 B/G) holds **five untapped
bodies** — Creakwood Liege 2/2, two Worm tokens (1/1 B/G, so 3/3 each under the
Liege's two lords), Odious Trow (1/1 BG -> 3/3), Stalker Hag (3/2 -> 5/4). Its
declaration clicks are `creakwood liege` x1, `worm` x1, `odious trow` x4, `worm` x4,
`stalker hag` x5 — each click cycles that blocker one step through its legal
attackers, so with three attackers a 4-click blocker ends **unassigned**. The AI took
the swing and died at -2; the human finished on 2 life, i.e. any surviving board with
two unblocked power would have won the AI the game on the crack-back.

## The hole

`AIPlayerBaka::chooseBlockers` reasons **one attacker at a time**, in all three
passes. Its only "I am about to die" branch (src/AIPlayerBaka.cpp, pass 3) is

```
else if (!survives && attacker->power >= life)
```

a **per-attacker** lethality test. A set that is lethal only in **aggregate** — three
4-power attackers into a defender at 5 — fails that test on every single member, so
every candidate chump is declined and the seat takes the whole swing on the face.
Nothing anywhere in `chooseBlockers` computed the number that actually decides the
game: **the damage that reaches me**, i.e. the summed power of the attackers nobody is
blocking (a trampler additionally getting through whatever its blockers cannot absorb).

Pass 1 has no per-attacker blocker cap and pass 2 unassigns any blocker whose attacker
will not die; between them the seat can also end up with several blockers stacked on a
single high-toughness attacker (Greensleeves was a 12/12-class body here) and nothing
on the two that kill it — the shape the owner's click record shows.

## The fix

A fourth, **survival sweep**, added after pass 3 and before the W43-1 CR 509.1c repair
sweep (so every value-motivated block the earlier passes wanted is already taken, and
the legality repair still runs last):

- new file-static `unblockedDamageTo(Player*)` — the real incoming total, trample-aware;
- `while (unblockedDamageTo(this) >= life)`: chump the **biggest** unblocked attacker,
  spending the free untapped creature that **survives** it if one exists and otherwise
  the lowest-power / lowest-toughness body; recompute and **stop the moment the total is
  survivable**, so the crack-back keeps every creature survival did not cost;
- menace / "three or more blockers" attackers are skipped (pass 3 owns set declarations;
  a partial one here would only be deleted by the rules layer), and legality goes through
  `canBlock()` + `canBlockPairwise(target)` + `blockRequirementSatisfiable()`;
- guarded at 32 iterations and it breaks if `toggleDefenser` is refused.

Blast radius is deliberately small: the sweep only ever **adds** blocks, and only when
the seat is otherwise dead on board. It touches no existing pass.

## Fixture

`bin/Res/test/ai_block_lethal_set_wave53.txt` (registered in `_tests.txt`) — three
Plated Wurms (4/5) attack an AI defender at 5 holding three Hill Giants (3/3) and two
Balduvian Bears (2/2). No Wurm kills alone, no defender kills or survives a Wurm.

- **RED on base** (`archives/wagic-dddb6f62b-w52corpus`): zero blockers declared, phase
  reached Combat damage, defender at **-7**, five creatures still on the battlefield.
- **GREEN after**: the two Bears (the cheapest bodies) chump the two biggest Wurms, the
  third Wurm is let through, defender ends at **1** with all three Hill Giants up.

## Gate

- Suite: **1210 tests (2 failed) + 45 AI tests (0 failed)**, 0 timeouts. The two
  failures are exactly the known concurrency-only pair `lifeline.txt` +
  `merrow_reejerey.txt`. AI count 44 -> 45 is this lane's fixture.
- Every existing blocking/`ai_block*` fixture green, incl. the deck62 N8 lone-chump pin
  `argothian_enchantress.txt`, `menace_defender_*`, `plague_wight_blockers_shrink`,
  `trample_vs_*`, `nontrample_vs_protection_blocker`, `combat_block_after_attack_trigger`.
- PARSETEST: **2121 passed, 0 failed** (baseline, unchanged — no parse-relevant strings).

## Replay: why the ground truth is a fixture and not the transcript

The brief asked for replay. `WAGIC_REPLAY` on this dump **diverges at turn 6** and
cannot be driven to turn 14. Two separate defects, both found here:

1. **Index drift (fixed in this lane).** A recorded click is `zone[INDEX] result name`.
   Once any zone reorders, every later index is wrong and the replay dies on a card
   that is merely in a different slot. The **name** is the stable identity, so under
   `WAGIC_REPLAY` `GameObserver::processAction` now resolves the recorded name inside
   the recorded zone (nearest slot to the recorded one wins) and acceptance compares
   zone+result+name instead of the slot number. Verified: it carried this dump from
   action 112 to 116. Everything is `getenv("WAGIC_REPLAY")`-gated, so it is inert in
   the suite, in `undo()`, and in normal play — which is also **why it carries no suite
   pin**: the behaviour is unreachable from a fixture by construction.
2. **RNG-stream drift (NOT fixed — docket item).** `rvalues:` is one shared stream:
   `GameObserver::randomGenerator` serves both game randomness (`MTGGameZone::shuffle`)
   and the AI seats' own `random_shuffle` calls in `AIPlayer.cpp`. AI seats are passive
   during a replay (their clicks come from the record), so they do **not** consume their
   values, every later draw from the stream is offset, and the library shuffle triggered
   by the human's Evolving Wilds produced a **different library order** — replay drew
   Ramunap Excavator on turn 6 where the device drew Predator Ooze. From there the board
   is a different game. The fix is to give the AI seats their own `RandomGenerator`
   instance (the class comment already says that was the intent) or to record their draws
   separately; that is an engine change well outside this lane and it is what stands
   between wagicGPT and replayable human-vs-Baka transcripts.

Note also that even a perfect replay could not have *observed* this decision: the AI's
blocker clicks are IN the record, so `chooseBlockers` does not run during a replay. The
transcript locates and describes the board; a fixture is the only thing that can
re-decide it.

## What I did NOT verify

- The exact turn-14 board was **read from the transcript, not replayed** (see above).
  The fixture reproduces the decision **shape** the report describes (a set lethal only
  in aggregate, blockers to spare) with vanilla stats; it is not a card-for-card
  reconstruction of Greensleeves/Predator Ooze/Hungering Hydra. In particular I did not
  verify whether Baka's `power`/`toughness` reads are correct for a CDA body
  (Greensleeves' `power=*` from land count) or for counter-carrying bodies — the
  instrumentation showed live values on ordinary creatures only.
- No live corpus / selfplay shakedown of the new sweep; suite only.
- Trample top-ups: the sweep never adds a **second** blocker to an already-blocked
  trampler, so a trampler whose lone blocker cannot absorb it can still push lethal
  through. Left for the docket.
- "Spend the cheapest body" is by power then toughness — it has no notion of card value
  (a 1-power engine piece outranks a 4-power vanilla under this rule). Also docketable.
