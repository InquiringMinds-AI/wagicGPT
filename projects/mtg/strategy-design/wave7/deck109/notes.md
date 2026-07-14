# Dev notes -- deck109 wave-7

## Record: 4/6 (down from 5/6) -- MATCHUP + VARIANCE, spine frozen
Beat 140, 135, 131, 133. Lost to 44 (faeries) and 110 (affinity). Both losses attribute to matchup +
variance, not a guide regression. See findings.md for the full audit.

## Fallback-DELETION regression check (wave-7's #1 mandated task) -- CLEAN
The wave-6 deletion of the deck's cast-fallback rule caused ZERO regression:
- Phantom "missed cast" / cast-didn't-happen reasoning: **0** across all 6 games.
- Cast-repeat / re-pick loops: **0**.
- Cast no-ops (same pick, mana unchanged): **0**.
- Desyncs (head-int != choice): **0**.
- Attacker commitment: **25/25** decisions fully committed, 0 "no attackers."
Keep the fallback deleted.

## Engine defects (repro pointers)
None new. The Hellrider combo-hold-hint veto (fixed by 353f1a150 in wave 6) stayed fixed -- Hellrider
cast and resolved where mana allowed (110 seq11).

## The two losses (repro pointers)
- **44 (faeries) LOSS** -- file `1783999716-...deck109-...jsonl`. deck44 = "Faerie Archmage" (UB
  faeries: Faerie Miscreant, Scion of Oona, Obyra, Archmage of Echoes + 6 counters/removal). Beat us
  with an all-FLYING clock; Scion of Oona grants the other faeries SHROUD (`auto=lord(other faerie)
  shroud`) so burn can't target them. We can neither block nor burn their board. Compounded by a 3-land
  screw (stuck on 3 Mountains T7-T11, both Hellriders + Boros Reckoner stranded). Counters never fired
  against us -- every cast resolved. Marginal misplay at seq19 (see general-suggestions #2).
- **110 (affinity) LOSS** -- file `1784009179-...deck109-...jsonl`. Out-raced (my 7 vs opp 17 at T7);
  Etched Champion has protection from red (`auto=aslongas(artifact) protection from red`), blanking our
  attackers AND burn. All-out attack at 7 was the only shot; swing-back for 12 killed us T8.

## Stopgap ledger
| stopgap line | defect it patched | status this wave | action |
|---|---|---|---|
| deck109 FALLBACK RULE | Hellrider combo-hold hint veto | DELETED wave 6; 0 regression this wave | stays DELETED |
| core `hand -> stack` anchor | no-op class | 0 no-ops/loops corpus-wide even post-deletion | cross-deck retire A/B (see general-suggestions #1) |
| deck109 output-format block | reply desync | removed wave 5; still 0 desyncs | none |

## strategy.txt freeze-check (turnaround / obeyed-but-losing mode)
Drafted as a constrained diff off the LIVE deployed guide. `diff` shows EXACTLY two changes; everything
else is byte-for-byte frozen:
1. Sharpened the "Cast nothing" clause in rule #1 (+ its echo in the SITUATIONS list): never decline a
   listed creature while fixated on an unaffordable bomb/lethal -- deploy the biggest listed body.
   (Targets deck44 seq19; reinforces an existing obeyed rule, does not move it.)
2. Added an "EVASIVE / UNTOUCHABLE BOARDS" matchup note (parallel to the Phyrexian Obliterator block):
   vs faerie flyers (fly+shroud) and pro-red artifact creatures you cannot block or burn -- race the face.
Frozen verbatim and confirmed present: identity block, DEPLOY-AND-ATTACK combat clause, MP2 checklist,
BURN face-default + exception, BLOCKING floor (the wave-5 fix -- still holding, 0 high-life blocks),
Phyrexian Obliterator note, all KEY CARDS, MULLIGAN, the rest of SITUATIONS.

## Deck-construction observations (upstream, low priority -- unchanged)
- **No answer to evasion** (NEW framing this wave): mono-red ground aggro cannot interact with flyers or
  with a shroud/pro-red board. deck44 (faeries) and deck110 (affinity+Etched Champion) are the two pool
  decks that exploit this -- the exact two that beat us. A reach/flying body or a pro-red-agnostic answer
  would help; flag only, do not change at 4/6.
- **No card advantage / mana sink** (standing since wave-5): compounded the deck44 3-land screw. Inherent.
