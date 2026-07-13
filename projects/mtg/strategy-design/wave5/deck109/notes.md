# Dev notes — deck109 wave-5

## Engine defects (repro pointers)

### [PERSISTS] Hellrider cast no-op (known ~#26 region / "Hellrider-class cast no-ops")
"Cast Hellrider" is selected, parsed correctly, and does nothing: mana is not spent, Hellrider stays
in hand, the phase advances. Confirmed again this corpus with a clean A/B repro in ONE game:

- File: `matchups-20260713-145947/1783977821-ai_baka_deck109-0x55b2adce2f60.jsonl`
- **NO-OP:** seq54 (T27 MP1) and seq56 (T27 MP2) — `Mana available:` reads `7 untapped sources`
  BEFORE and `7 untapped sources` AFTER; Hellrider ({2}{R}{R}) stays in hand. A real cast drops the
  count by ~4.
- **RESOLVE (same game, same board region):** seq57 (T29 MP1) — `Mana available:` drops 7 -> 4.

Wave-4 hypothesized correlation with board width (no-op when the board is creature-light; resolves
when other creatures are out). This corpus is consistent: the no-op happened at T27 and resolution
followed. Wave-4 lost two games to this; this wave the guide's fallback rule contained it into a slow
win (T29). Still worth an engine fix — candidate: the cast path for a haste creature whose only
trigger is an on-attack ping when there is nothing else to attack with. Prior A/B (wave-4): no-op
L131 s27 vs resolve W110 r1 s17.

## Stopgap ledger (for the platform-fix sweep)

| stopgap line | defect it patched | status this wave | action |
|---|---|---|---|
| deck109 output-format block (old lines 6-14: "bare number only, don't echo option text") | reply-parser desync (wave-4 §1) | defect FIXED — 0 desyncs / 6 games (head-first parser live) | **REMOVED** from strategy.txt this wave |
| deck109 FALLBACK RULE (Hellrider "hand -> stack" check) | Hellrider cast no-op (above) | defect PERSISTS (140 seq54/seq56) | **KEPT**, re-cited |
| core `hand -> stack` cast-happened anchor | same no-op class | defect PERSISTS | keep in core until engine fix |
| deck109 heavy MANA/interface restatement (old lines 16-19) | "attacking taps my lands" delusion | delusion FIXED via representation (§3) + core interface contract carries it | **COMPRESSED** to the "Mana available:" reference inside the MP2 checklist |

## Deck-construction observations (for the user — low priority)

deck109 is a well-formed RDW: 28 creatures / 12 spells (8 burn + 4 Krenko's Command) / 20 Mountains,
mono-red so no color problems. Two soft notes, neither urgent given the 5/6 record:

- **No card advantage and no mana sink.** The deck's only reach is 8 burn spells; once they are
  spent it draws off the top with no way to convert excess lands into pressure. In the 110 LOSS it
  ran mana-light (~4-5 lands) and lost a one-turn race; a flooded draw would have the opposite dead
  end (excess Mountains, nothing to spend them on). This is inherent to the archetype, not a defect —
  flag only if the user wants to tune the curve (a Goblin Grenade / Reforge the Soul-style top-end,
  or Mutavault-style manland, would give flood insurance). Not recommended to change for now.
- **20 lands is on the high side for a 12-one-drop curve topping at 4 mana.** Could shave to 18-19
  for more gas, but the mana-light 110 loss argues against cutting lands. Leave as-is.

## Harness notes

- This corpus was a single round-robin run (6 games, one per matchup) — small sample. deck109's 5/6
  matches last corpus's run2 (also 5/6), so the record is stable, but per-game conclusions rest on
  1-2 instances each; the reflexive-blocking finding (4 instances, all one direction) is the only
  play pattern with enough repetition to act on confidently.
- No timeouts, no empty-reply fallbacks, no `choice==-1` in any deck109 game this run.
