# General-strategy / core-prompt proposals (deck110, wave 5)

Baseline reviewed: `bin/Res/ai/gpt/system_prompt.txt` (current live core). Layer-routing per the
skill: burden of proof is ON core; single-seat evidence cannot CUT a core line.

## Headline: NO new core-prompt text proposed. The one live defect routes to ENGINE/REPRESENTATION.

The deck's only material residual this wave is the re-equip waste loop. The core prompt ALREADY
carries the defense (line 27: "before any Equip or attach line, read the target's stats: TWO
power numbers, like '(6/2) (printed 2/2)', mean a bonus is already on it and the equipment is
almost certainly already attached") AND the stale-plan grounding (line 35: "if an action is still
offered, it has NOT been done"). deck110 seq84 proves the pilot can READ the line-27 cue,
verbalize the correct conclusion, and STILL take the re-offered equip. Per the skill's own rule
("a rule already stated and still bypassed routes to representation, never to a second paragraph
of the same instruction"), the fix is the attachment marker / re-offer suppression (engine task
#26 — see notes.md), NOT more core wording. Adding core text here would tax every deck for a
defect that instruction cannot reach.

## Flag for the synthesis agent (not an edit — a caution about an existing core stopgap)

Core line 27's "two power numbers = equipment almost certainly attached" heuristic is a STOPGAP
with two weaknesses this deck exposes, both resolved by #26's attachment marker:
1. **Proven ceiling** (above): the pilot reads it correctly and disobeys anyway.
2. **False positives it cannot see**: two power numbers are produced by ANY stat modifier, not
   just equipment — Master of Etherium's +1/+1 lord, Steel Overseer's +1/+1 counters, and Signal
   Pest's battle cry all render "(N/M) (printed x/y)" with NO equipment attached. In this deck a
   creature can show two numbers from a lord buff while the Plating sits unattached; the heuristic
   would then wrongly say "done" and could suppress a NEEDED equip. I did not OBSERVE this
   false-negative firing this corpus, so this is a caution, not a cut — but it argues the line
   should be TAGGED as a stopgap whose removal condition is "#26 ships an attachment marker,"
   rather than reinforced or generalized. Recommend the synthesis agent record it in the core
   stopgap triple-list (line, defect+citation `1783980343` seq83/84, removal = attachment marker).

There is also a latent internal tension the marker resolves: line 35 says "still offered = not
done," but a re-offered already-attached equip is precisely a DONE action still offered. The
marker gives the pilot a fact that outranks both heuristics.

## Deck-specific asks that must NOT go to core (recorded so they route correctly)

- The Lightmine-Field toughness interaction (attack only with toughness > Lightmine damage; else
  Galvanic Blast to the face) is a NAMED-CARD, deck-shape rule — it belongs in deck110's guide
  (and any other low-toughness go-wide deck's), never in core. A deck-agnostic core line about
  "damage-to-attackers punishers" already exists (line 29: "An effect that punishes attackers...
  recompute per attacker"); it is correct as written. The gap is that it does not mention that a
  punisher can make attacking impossible when your toughness is too low — but that is a per-deck
  instantiation, not a core edit.

## Confirmations (no action, for the record)

- The head-first reply parser and the mana-representation change remain SOLID for this deck
  (0/247 desync, 0 fallbacks, no mana delusion) — do not revert either.
