# Dev notes -- deck109 wave-8

## Record: 4/6 (unchanged count, reshuffled split) -- MATCHUP + VARIANCE, spine frozen
Beat 140, 44, 135, 131. Lost to 133 (devotion+Obliterator+Gray Merchant) and 110 (affinity). This wave
it BEAT 44 (faeries) and LOST to 133 -- the exact inverse of wave-7's loss pair, confirming the record is
draw/opponent variance at n=6, not a guide signal. See findings.md.

## Harness sweep -- CLEAN
- Desyncs (head-int != choice): 0.  Cast no-ops (same pick, mana unchanged): 0.  Cast-repeat loops: 0.
- Phantom "missed cast" reasoning: 0.  Attacker commitment: 65/65 (0 "no attackers").
- Hellrider-fallback DELETION regression check: CLEAN again (0 recurrences). Keep it deleted.

## Engine defects / gaps (repro pointers)
- **#5 punisher rider MISSING on Phyrexian Obliterator.** vs133 file `1784027795-...deck109...jsonl`
  seq22 blocker prompt: A2 line = "Phyrexian Obliterator (5/5) [trample]" with no sacrifice-punisher
  text, while A1 Bloodghast got a full `{text:...}` rider. Obliterator's `auto=@damaged(this)... sacrifice`
  is exactly the damage-punisher #5's rider is meant to surface. Route to engine ledger. (Handled per-deck
  this wave with a named "never block Obliterator" line.)
- **#5 blocker-seam lethal arithmetic itself FIRED correctly** in all 3 blocker prompts (the "Unblocked...
  you would be at M - NOT lethal" line) -- no engine bug there; the issue is the pilot disobeying it
  (representation present-and-losing; see general-suggestions #1).
- No new cast/no-op/desync defects.

## The two losses (repro pointers)
- **133 LOSS** -- file `1784027795-...deck109...jsonl`. DECIDING LEVER = seq22: blocked Phyrexian
  Obliterator with 2 Goblins at 10 life (surfaced "at 3 - NOT lethal"); each Goblin dealt 1 damage ->
  sacrificed 2 Mountains (seq23/24) -> manaless; lost T13 at -4. Guide-fixable (blocking rule + Obliterator
  note). Matchup backdrop: Gray Merchant drained opp 2->10 / me 18->10 the turn before (devotion lifegain).
- **110 LOSS** -- file `1784029007-...deck109...jsonl`. seq13: gang-blocked Master of Etherium (6/6) at 19
  life ("at 12 - NOT lethal") -- high-life floor violation; also 6 windows declining its one Pillar of
  Flame (false "{1}{R}" cost belief / hold-mana). Non-outcome-changing -- affinity alpha-struck for 21 at
  T8. Matchup + variance.

## strategy.txt freeze-check (turnaround / obeyed-but-losing mode)
Drafted as a constrained diff off the LIVE deployed (wave-7) guide. Changes, all aimed at the ONE new leak
(blocker seam) plus a one-line corrective:
1. **BLOCKING section rewritten** -- re-anchored the chump branch from "would drop you to 8 or less" to the
   engine-SURFACED lethal line ("obey 'you would be at M - NOT lethal / LETHAL'"; chump ONLY on LETHAL).
   Threshold-reuse of the shipped #5 number, killing the too-eager chump-at-8 that FOUGHT the math (vs133).
   Kept exception (a) verbatim in substance (gang-block token-maker/lifegain -- fired correctly vs131 s7).
   Added a NEVER-block-damage-punisher absolute (Obliterator/Boros Reckoner) as the explicit exception --
   the wave-7 deck44 hoist-the-absolute method, now needed at this seat.
2. **Phyrexian Obliterator note** -- added "NEVER block it" (previously it only covered ATTACKING past it).
3. **BURN / Pillar line** -- one-clause corrective: "Pillar of Flame costs {R}, not {1}{R}; fire it at the
   face when it's your only castable option" (targets the vs140/vs110 false-cost declines).
FROZEN VERBATIM and confirmed present: identity block, DEPLOY-AND-ATTACK #1 rule + combat clause, MP2
checklist, BURN face-default + exception, EVASIVE/UNTOUCHABLE-BOARDS note, all KEY CARDS, MULLIGAN, and the
rest of SITUATIONS (the block/Obliterator SITUATIONS lines were re-anchored to match the rewrite).

## Stopgap ledger
| stopgap line | defect it patched | status this wave | action |
|---|---|---|---|
| deck109 FALLBACK RULE | Hellrider combo-hold hint veto | DELETED wave 6; 0 regression 2 waves running | stays DELETED |
| core `hand -> stack` anchor | cast no-op class | 0 no-ops corpus-wide | wave-8 A/B already adopted (brief) |
| deck109 BLOCKING chump-at-8 branch | pre-#5 blocker-seam blindness | #5 now surfaces lethal math; old <=8 threshold FIGHTS it | RE-ANCHORED to surfaced line this wave |
| deck109 Obliterator "never block" | punisher rider missing on Obliterator (engine gap) | NEW this wave | KEEP until #5 riders cover Obliterator |

## Deck-construction observations (upstream, low priority -- unchanged)
- No answer to evasion (flyers/shroud/pro-red) -- restated, not relitigated; it beat 44 this wave on a
  good draw, structural weakness unchanged.
- Devotion + Obliterator + Gray Merchant (133) is a sibling hard matchup (can't attack into Obliterator;
  drain undoes the race). No card advantage / mana sink -- inherent. Do not change at 4/6.
