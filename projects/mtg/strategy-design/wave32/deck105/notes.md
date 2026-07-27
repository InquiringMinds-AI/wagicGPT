# deck105 — development notes (engine / render / harness), wave 32

Corpus `matchups-20260727-115847`, binary `7fabd9bd0`. All repros are seq+turn cited from the
seat translogs; source line numbers are from the tree at that binary.

---

## N-105a — POISON COUNTERS ARE NOT RENDERED TO THE AI, ANYWHERE. (RENDER, HIGH)

**Severity: HIGH. Repro quality: PERFECT (present in 100% of windows of 6/6 games).**
(Recorded as separate fields per wave-31 HL4.3.)

The engine tracks the number (`Player::poisonCount`, `Player::isPoisoned()`, `Player::poisoned()`
in `include/Player.h:94-95`) and RENDERS IT FOR THE HUMAN (`src/GuiStatic.cpp:138-140`,
`if (poisonCount > 0) { sprintf(poison, "%i", poisonCount); ... }`). `AIPlayerGPT.cpp` contains
zero occurrences of "poison" outside a code comment. This is an ishuman-lens gap of the exact
shape the skill's lens section describes: information built for the human frontend, absent from
the AI's.

Emitter enumeration (wave-31 HL5 sweep — every surface that could carry it):

| surface | source | current output | needed |
|---|---|---|---|
| life line | `AIPlayerGPT.cpp:2251` | `Your life: 20 \| Opponent life: 20` | + poison, both players |
| board/battlefield | player lines | no player-counter field at all | — |
| narration, combat | damage emitter | `- Damage: 1 dealt by Plague Stinger to the opponent` | needs a poison line |
| narration, non-combat | `alterpoison` (`AllAbilities.cpp:1464`) | **silent** | needs an event line |
| attacker seam | attacker list | `A1. Plague Stinger (1/1) [flying, infect]` | poison-to-lethal forecast |
| blocker seam | `AIPlayerGPT.cpp:6768` | life forecast — **false**, see N-105b | — |

Two shapes make it worse than a plain omission:
1. **Infect damage narrates in the SAME SHAPE as real damage.** Real: `- Opponent's life -4
   (now 8)` + `- Damage: 4 dealt by Orc army to the opponent`. Infect: `- Damage: 2 dealt by
   Contagious Nim to the opponent` with the life line ABSENT and life unchanged. The only signal
   is a MISSING line.
2. **`alterpoison` narrates nothing at all.** Ichor Rats (`auto=alterpoison:1 all(player)`)
   entered 4× across the corpus; each time both players silently took a counter. Verbatim log
   (vs36): `- Your Ichor Rats: hand -> stack` / `- Your Ichor Rats: stack -> battlefield` /
   `- Phase: Attackers`.

Observed cost — the pilot must hand-count and gets it WRONG:
- `vs36 s17 t12` (ask): *"I am at 20 life and need 10 poison counters to win; I currently have
  0."* — the opponent was on **6**. Same game `s19 t12`: *"bringing the opponent to 5/10"* on the
  swing that landed on **11** and won.
- `vs146 s23 t15` (ask, LAND DROP, 5,262 chars / 83 s): *"has 11 poison counters (... 2+1+2+1+2+1+2
  = 11)"* — actual **8** — then *"I need 20 poison. I have 11."* (threshold conflated with life).
- `vs146 s22 t14` (blockers, **8,827 chars / 143,614 ms** — the seat's 5th-largest reply):
  *"If they have 11 poison counters, they have already lost. The game should be over. ... However,
  the game log continues to Turn 15. ... If the game is still going, then my count is wrong."*

**Proposed fix (smallest sufficient):**
(a) `AIPlayerGPT.cpp:2251` — append poison to the life line whenever either player has any, e.g.
    `Your life: 20 (poison 2/10) | Opponent life: 20 (poison 6/10)`. Suppress entirely at 0/0 so
    non-infect duels pay nothing.
(b) narration — emit a poison line for BOTH sources: after infect combat damage, and on
    `alterpoison`, e.g. `- Opponent gets 2 poison counters (now 6 of 10)`. This is the line that
    makes the append-only log self-sufficient, which is what the pilot actually reads.
(c) optional, cheap: on the attacker seam tail, where the life forecast is built, add the poison
    equivalent for infect attackers (`these attackers add up to 5 poison - the opponent would be
    at 11 of 10 - LETHAL`).

**Validation predicate for the wave that ships it (falsifiable, write it into the brief):** at
deck105's next corpus, (i) grep the seat's replies for the hand-count family
(`poison counters (based on|Let's recount|my count is wrong|X of 10`) — expect ZERO
re-derivations; (ii) the seat's top-5 tax rows must no longer contain a LAND DROP; (iii) at least
one reply must quote the rendered poison number back (`they are on N poison`). Arrival-trace, do
not read a counter.

**Guide stopgap in force until (a)+(b) ship:** `strategy.txt` section "THE CLOCK — COUNT THE
POISON ONCE, THEN STOP" (the count-from-the-log procedure and the "their life staying at 20 is
what infect looks like working" falsity fact). **REMOVAL CONDITION: delete the whole counting
procedure and replace it with "read the poison number on the life line" the wave (a)+(b) ship.**

---

## N-105b — THE BLOCKER-SEAM LIFE FORECAST IS FALSE AGAINST INFECT ATTACKERS, AND IT COST A GAME.
(RENDER, HIGH — CROSS-SEAT)

**Severity: HIGH (game-losing, 6 seats exposed). Repro quality: PERFECT (12/12 windows, one
verbatim decision quote).**

`AIPlayerGPT.cpp:6768-6770` builds the defender's forecast from raw attacker power with no infect
check:
```
tail << "Your life: " << life << ". Unblocked, these attackers deal up to "
     << incoming << " - you would be at " << (life - incoming) ...
```
Emitted string, every infect blocker window in this corpus:
> `Your life: 20. Unblocked, these attackers deal up to 5 - you would be at 15 - NOT lethal:
> block only where the trade favors you; taking damage while ahead is often correct.`

Three independent falsehoods for an infect swing: (1) the defender's life will not change;
(2) "NOT lethal" is a claim about a resource the attack does not touch, and is *inverted* on any
swing that crosses 10 poison; (3) "taking damage while ahead is often correct" is the exact
opposite of correct play against a counter that never resets and that no card in the pool removes.

**Occurrence: 12 of 12** infect blocker windows across the six games (deck36 s9/s13/s17/s20/s25,
deck146 s28, deck158 s9, deck116 s8/s11, deck152 s11, deck139 s20/s34). 4 of the 12 ended in
`no blockers`.

**Game-losing repro — deck36 s25 t12, kind=blockers, the killing swing.** Prompt line:
`Your life: 20. Unblocked, these attackers deal up to 5 - you would be at 15 - NOT lethal: ...`
Reply, verbatim: *"Since the total damage (5) is not lethal (I would drop to 15), and blocking
with the Pest achieves nothing but removing my only potential future blocker, I declare no blocks
to preserve the Pest"* -> `BLOCKS: none` -> poison 6 -> 11 -> `WAGIC_SELFPLAY_RESULT winner=0
life0=20 life1=20 turn=12`. deck36 lost the game at **20 life** having been told the lethal
attack was not lethal.

**Same-seam control (wave-31 HL4.1):** the identical emitter is CORRECT in every non-infect
window in the same corpus — e.g. deck105's own blocker windows against deck146's ordinary
attackers (`s18 t12`: `you would be at 7 - NOT lethal`, and the life did go 15 -> 11). So the
defect is branch-scoped to infect (and, by construction, to any damage-replacement keyword:
**check `toxic` and `wither` on the same pass** — `Damage.cpp:237` reads `source->getToxicity()`,
so toxic exists in this engine and would have the same bug).

**Proposed fix:** when summing `incoming`, partition attackers by whether their damage is
replaced. For the infect/toxic partition emit a poison forecast against the 10-counter threshold
and DROP the "taking damage while ahead is often correct" clause, e.g.
`Your life: 20 (poison 6 of 10). Unblocked, these attackers add 5 poison - you would be at 11 of
10 - LETHAL: you must block or you lose.` For a mixed swing emit both halves. Note the advice
clause is itself part of the payload — an accurate number with the stale advice attached is still
a misteach (wave-31 HL5 / the annotation-wording rung: restriction-first, no affirmative
substring the model can latch).

**Validation predicate:** after the fix, zero occurrences of `- you would be at <life-N>` in any
window whose attacker list contains `[infect]`; and at least one opponent block declared against
an infect swing that the old line called "NOT lethal".

**Consequence for the record (routed to synthesis, not to a guide):** deck105's 6/6 was set while
its six opponents were being told, at the decisive seam, that its attacks were harmless. The
record is CONFOUNDED and should be re-measured against the same pool after this fix before
deck105 is priced for the difficulty ladder.

---

## N-105c — (routed to general-suggestions.md, not an engine item)

Core system prompt `AIPlayerGPT.cpp:63` asserts "You win by reducing the opponent's life from 20
to 0", which is false for any alt-win deck and is the only win-condition statement the pilot gets.
See `general-suggestions.md` G1 — filed as a PROPOSAL with a HOLD recommendation, because N-105a
is the likelier carrier and adopting both in one wave makes them uncorrelatable.

---

## N-105d — the `(creatures: N)` battlefield header is read as a type claim about the list.
(RENDER, LOW — clean repro)

Board lines render as
`Your battlefield (creatures: 1): Swamp #1; Swamp #2; Necropede {2} (1/1) [artifact] [infect]` —
the parenthetical count is immediately followed by land names. deck152 s5 t5 (a LAND DROP,
5,346 chars) spent a paragraph on it, verbatim:
> *"This is weird. It lists lands under 'creatures'? No, it lists 'Necropede' as the creature.
> The lands are listed separately? ... This looks like a formatting error in the prompt's
> representation of the board."*
It recovered and chose correctly, so severity is LOW; but it is pure tax on the exact
decision kind wave-31 HL9 says to read first, and the fix is cosmetic — either move the count to
the end (`Your battlefield: Swamp #1; ...; Necropede ... (1 creature)`) or label it
(`Your battlefield (1 creature among the permanents below):`). Only worth doing if that emitter
is being touched for another reason.

---

## N-105e — mulligan-bottoming card text truncates mid-word. (RENDER, COSMETIC)

`deck146 s4` bottom options:
`1. Blackcleave Goblin (cost 4) {text: Haste -- Infect (This creature deals damage to creatures in
form of -1/-1 counters and to players in form of poison...}` — the truncation lands one word
before the payload ("...poison counters"), so the reminder text stops exactly where it becomes
informative. If the bottom-line text budget is a fixed char cap, prefer truncating at a word
boundary after the first sentence, or drop parenthetical reminder text from bottom lines
entirely (the cards are the pilot's own deck).

---

## Construction / roster observations (NOT engine items — for the synthesis's roster lane)

- **deck105 runs 35 lands in 65 cards (53.8%)** — 21 Forest, 14 Swamp, 30 spells. It flooded
  visibly (the vs146 five-card keep of FIVE LANDS; four of the seat's top-eight tax spikes are
  land drops in board states with 6-7 lands out and nothing castable) and won 6/6 regardless.
  There is a lot of slack here for a deck the current pool cannot answer.
- **The deck has no 1-mana cards.** Cheapest MV is 2 (Blight Mamba {1}{G}, Plague Stinger {1}{B},
  Necropede {2}, Ichorclaw Myr {2}). This drove a false mulligan criterion (findings NM3); the
  guide states it flatly.
- **Two build-arounds went UNEXERCISED in 6 games** (wave-30 seeded-but-unexercised verdict, not
  "clean" and not "buggy"): **Hand of the Praetors** (`lord(other creature[infect]|myBattlefield)
  1/1` + `@movedTo(*[infect]|mystack):alterpoison:1 target(player)`) was drawn **0 times**;
  **Relic Putrescence** (`@tapped(mytgt):damage:1 targetcontroller`, card carries
  `abilities=infect`) was drawn 4 times and cast **0 times**. Both are 2-ofs in 65 cards.
  Relic Putrescence in particular has an unverified mechanism — it is implemented as
  `damage:1 targetcontroller` on an ENCHANTMENT that carries `abilities=infect`, and whether that
  1 damage is actually converted to a poison counter (rather than 1 life loss) has never been
  observed live. **PROBE SPEC:** a temporary `deck199` stacking 4× Hand of the Praetors, 4× Relic
  Putrescence and 4× Ichor Rats against an artifact-heavy opponent (deck36's list is a ready-made
  target — it plays Seat of the Synod, Ancient Den, Vault of Whispers, Silver Myr, Ornithopter,
  Nuisance Engine), one pinned GPT selfplay game, then DELETE the probe deck. Check three things:
  the Hand lord bonus appearing in the rendered `(N/M)`; the Hand cast-trigger firing per infect
  creature spell; and whether Relic Putrescence's trigger produces a POISON counter or life loss.
  Do this after N-105a ships so the poison side is observable at all.

---

## Harness / corpus hygiene

- 0 crash, FATAL, assert or sanitizer signatures across all 6 deck105 stderr files — i16's
  effect confirmed at a brand-new deck.
- 0 fallbacks in 148 decisions (0 empty_reply, 0 unparsed_reply, 0 retracted_choice,
  0 truncated_abandoned, 0 validation_reject_reask/_exhausted). i5/i6 UNEXERCISED here.
- Latency median 17.1 s, p90 61.8 s, max 219.2 s. The three largest replies (13,772 / 13,275 /
  12,604 chars) are all poison-count or mana re-derivation spirals, not truncation risk — reply
  p95 is 6,266 chars and nothing truncated. **If N-105a lands, expect the p90 latency at this
  seat to fall materially; that is a second, free validation signal for the fix.**
