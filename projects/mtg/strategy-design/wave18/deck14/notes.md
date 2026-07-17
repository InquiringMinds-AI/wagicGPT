# deck14 (Deep Blue) — notes.md — engine/harness/model items + rotation verdict

## ENGINE / HARNESS LEDGER (layer-routed here, never core prose)

### E1 (HIGH value; 3-repro, deciding at least once) — the blockers/attackers parser should accept
### CARD-NAME assignments, not only `B#:A#` codes.
Three fallbacks this seat lost a REASONABLE combat decision purely to format drift — the model
finished its (sound) reasoning and wrote the assignment line with card names/prose instead of the
index codes:
- vs62 s34 (blockers, 8,654 chars): `BLOCKS: Gravelgill Axeshark: Saproling (1/3), Lord of Atlantis:
  Saproling (1/1)` — a legal, sensible block; unparsed → Baka answered.
- vs62 s37 (blockers, 10,206 chars): `BLOCKS: Air Elemental:Canopy Spider, Lord of Atlantis:Canopy
  Spider` — legal gang-block; unparsed.
- vs135 s26 (attackers, 13,116 chars): decisive turn (LOSS turn 14), spiraled and never emitted a
  clean `ATTACK:` line.
The block/attacker labels already carry the card NAME next to each B#/A# code, so a tolerant parser
can map an unambiguous name back to its index (fall back to code-only when a name is ambiguous —
duplicate names, the known gang-block disambiguation concern). This is the combat-side analog of the
target name→index reconcile already shipped for attackers (`d0f88326b`). Guide rule #2 is the interim
mitigation. Highest-leverage engine item from this seat.

### E2 (MEDIUM; representation) — combat reasoning-tax on math-heavy blue combat.
The 8-13k-char combat replies are model behavior, but the CURRENT SITUATION already renders each
creature's live [X/Y] and keywords; a combat-decision prompt could add a one-line "you rarely block
while ahead; N attackers deal up to K, you would be at L" cue (the blockers prompt already shows the
"NOT lethal" life line — it fired correctly at vs62 s34 and the model still enumerated everything).
Consider surfacing the trade outcome per candidate blocker (mirror of "the deciding fact rides the
option") to pre-empt the enumeration. Representation lever, not core.

### E3 (LOW; representation) — invented option number (vs110 s8).
Model answered `CHOICE: 3` when only options 1-2 existed, sourcing a card from hand. Out-of-range
choice numbers already fall back cleanly; a soft "you named an option that is not on the list" nudge
in the re-ask could help, but this is the known present-and-disobeyed list-authority wall — low
priority, guide off-ramp covers it.

## CARD-SCRIPT DIVERGENCES TO ORACLE-VERIFY (per the verify-oracle-text rule; guide uses engine reality)
- **Gravelgill Axeshark**: engine = {4}{u/b} 3/3 [islandwalk, persist]. Paper Gravelgill Axeshark is
  {4}{U} 4/3 Merfolk Soldier with "As long as you control a red permanent, it can't attack or block"
  — NO islandwalk, NO persist. The engine card is a substantially different card (likely a mis-mapped
  primitive). Flag for the owner: verify the primitives entry against Scryfall; the guide teaches what
  the engine renders (3/3 islandwalk+persist), but the script may be wrong.
- **Counsel of the Soratami**: engine cost {2}{u} (3 mana); paper 10E printing is {3}{U} (4 mana).
  Minor; guide uses engine {2}{u}.
- **Coral Merfolk** engine 2/1 (paper 2/1 — OK). **Lord of Atlantis** engine 2/2 lord +1/+1/islandwalk
  to Merfolk (matches paper — OK). **Air Elemental** 4/4 flying, **Mahamoti Djinn** 5/6 flying (OK).
- **Islandwalk relevance**: Lord of Atlantis grants islandwalk to Merfolk, and Axeshark has it, but
  it is unblockable only vs a defender controlling an Island — near-dead in this pool. Not a bug;
  noted so the guide does not over-teach it.

## MODEL-BEHAVIOR notes
- The blue toolkit (bounce/draw/steal) reliably pulls the pilot into a passive control posture on a
  tempo deck (vs35 0-attacker loss). The guide rule-#1 countermand targets exactly this.
- Bounce and steal target-selection are weak by default (re-bounces a 1/1; steals a 1/1 with Control
  Magic vs49 s27). Guide addresses via "clear a blocker / steal their BEST creature."

## ROTATION VERDICT
**NEWCOMER — does NOT rotate.** deck14 entered guideless this wave and receives its first guide.
Strong new-work signal: FC1 (durdle-into-defense, 0-attacker loss) and FC2 (combat format-drift tax,
3 fallbacks) are load-bearing and both addressed in the new guide + E1 engine item. Re-evaluate after
its first GUIDED corpus (wave-19) per Step 0-ter (died/persisted/mutated per taught class).
