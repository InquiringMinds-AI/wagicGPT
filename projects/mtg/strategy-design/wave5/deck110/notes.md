# Deck110 wave-5 notes — engine/harness/deck-build items

Evidence base: 6 games / 281 decisions, run `matchups-20260713-145947`. Organized by audience.

---

## For the ENGINE owner

### E1 (task #26). Attachment-state invisibility — RE-EQUIP QUANTIFIED, primary witness
This is the deck's #1 residual and the requested quantification for engine task #26.

**Count this corpus:** 35 equip/attach selections; classified by target-power delta across
consecutive prompts:
- 18 LEGIT (power rose — a real attach, including legit SECOND-plating attaches)
- 15 WASTE (power unchanged — re-equipping an already-attached Cranial Plating, {1} for nothing)
- 2 unknown (creature left play)
=> ~43% wasted. (Wave-4 was 47 equips / ~27 waste = ~57%; ratio improved but the class persists.)

**Concentration:** 21 of 35 equips (and ~11 of the 15 wastes) are in ONE game — the 140v110
timeout, 115 decisions / 68 priority windows. The other 5 games took 14 equips, ~4 waste. The
re-offer scales with priority-window count, so long/grindy games amplify it badly and inflate
wall-clock (contributing to that game's timeout).

**The decisive repro — instruction has failed at full strength:** `1783980343` seq83 and seq84
(T21). Board reads `Steel Overseer (14/2) (printed 1/1)` throughout; both platings already
attached (power unchanged 14->14 both picks). At seq84 the model's REPLY says verbatim: "The
Cranial Plating is already attached to Steel Overseer (indicated by the '14/2 (printed 1/1)'
stats)... Equipping it again is a waste of mana. I will attack immediately." — then chose option
2 = Equip. It READ the state, REASONED correctly, and still took the re-offered no-op. This is
the clean proof that no amount of guide/core wording closes this; the fix must be representation:
  - annotate the OPTION line: `Equip with Cranial Plating targeting Steel Overseer [cost {1}] [ALREADY ATTACHED]`, or
  - suppress the already-attached equip line entirely, or
  - add board attachment markers: `Steel Overseer (14/2) [equipped: Cranial Plating x2]` +
    `Cranial Plating [attached to Steel Overseer]`.
The board/option marker also fixes the two-power-number heuristic's FALSE POSITIVES (lords,
+1/+1 counters, battle cry all produce two numbers with no equipment attached — see
general-suggestions.md). Wave-4 notes E1/E2 raised this; wave 5 confirms it at full strength with
a read-it-then-disobey-it repro. **Highest-value single change for this deck.**

### E2. `{B}{B}` attach line still offered on a deck with no black source (menu noise)
Cranial Plating's `auto={B}{B}:name(attach)` line is offered even when the deck has no black mana
available; it is pure clutter next to the {1} equip and costs a decision to skip. Same class as
wave-4 E2 / the Mox Opal label noise. Low severity; bundle with the #26 menu cleanup.

### E3. Mana-ability labels still read as spell casts (unchanged from wave 4)
`Cast Card Normally with Mox Opal` / `Play Land with Mox Opal` still surface Mox Opal's tap-for-
mana as spell-shaped options. deck110's guide carries the defusal line (stopgap; remove when the
label says what it does, e.g. "Tap Mox Opal for one mana of any color"). No new repro needed.

## For the HARNESS owner

- Clean bill: 0/247 head-vs-choice desync, 0 fallbacks in 281 records — head-first parser and
  the -j pacing are healthy for this seat. Do not revert.
- Corpus size note for the next reviewer: this wave is ONE run (6 games) vs wave-4's two runs (12
  games). The "16 -> 35 equip" watch metric is not comparable across those without normalizing by
  game count AND game length; per-game and per-priority-window are the honest denominators.

## For the NEXT deck110 REVIEWER

- Re-equip target for wave 6: with #26 shipped, waste should approach 0. If #26 is NOT shipped,
  expect the waste to persist regardless of guide wording — do NOT spend the wave making rule #1
  louder (proven ceiling). Check the per-priority-window re-equip rate, not the raw count.
- Frozen lines under the constrained-diff discipline (see findings.md freeze-check): mana
  falsity-fact, THE RACE, ATTACK EVERY TURN, LETHAL CHECK, Blast-to-face-when-<=8, carrier order,
  Memnite/Ornithopter {0} on sight, mulligan. All obeyed this corpus.
- Watch the aggro mirror (deck109): wave-4 lost both, wave-5 won the one game (my6/op0). If a
  larger sample shows it holding, THE RACE section has earned demotion/compression.

## For the DECK BUILDER / difficulty-roster track

- **Toughness fragility is a hard structural weakness.** Every creature is toughness 1-2
  (Memnite 1/1, Ornithopter 0/2, Signal Pest 0/1, Vault Skirge 1/1, Etched Champion 2/2, Steel
  Overseer 1/1) and Cranial Plating pumps POWER only. Any "damage to each attacker" effect or
  repeatable pinger neutralizes the whole combat plan. deck140's TWO Lightmine Field
  (`damage = number of attackers, to each attacker`) reduced this deck to a 33-turn timeout: a
  14/2 attacker died to 2 Lightmine damage dealing zero (`1783980343` seq86). The deck's only out
  vs Lightmine is Galvanic Blast to the face, and it runs just 4 — so those matchups are variance
  on drawing burn. This is a real RPS shape (deck110 crushes most; is hosed by Lightmine/pingers)
  worth keeping for roster tiering, but flag it: deck110 is NOT a safe "always strong" slot.
- Glimmervoid is a LAND, not an artifact (does not count for metalcraft; sacrifices itself with
  no artifacts out). Mox Opal + Darksteel Citadel DO count. Keep in the generator's card-notes so
  a generated artifact deck doesn't silently miss metalcraft (re-flag from wave-4 notes).

## For the MODEL-EXPERIMENTS track

- Reconfirms wave-4's sharpest finding: rules requiring the pilot to DERIVE state from the board
  fail even when the pilot verbalizes the correct derivation (seq84: reads "already attached,"
  says "waste of mana," equips anyway). Vocabulary compliance != action compliance. The clean A/B
  is still available: ship #26's attachment marker, hold guides constant, measure whether the
  re-equip waste dies. If it does where two waves of instruction text did not, that is the second
  representation-beats-instruction confirmation (after the mana line) and should re-rank the
  backlog toward representation work.
