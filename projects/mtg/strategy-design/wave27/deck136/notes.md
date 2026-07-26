# deck136 (Yarok of Pranks) - notes.md (engine/harness ledger items)
# Wave-27, STEP-0 GUIDELESS DEBUT. Binary ffec79fd8. Corpus matchups-20260725-193608.
# Layer-routing: these are engine/card-script/harness items. NONE become guide prose.

## TOP ITEM -- NEW LEDGER ENTRY: R-RESOLVED-SPELL-PHANTOM-CHOOSER (Legion's End)
Name: R-RESOLVED-SPELL-PHANTOM-CHOOSER
Severity: HIGH-VISIBILITY, LOW-HARM (deterministic, harmless outcome, but the single
largest reasoning-tax sink in the corpus and a double invariant violation).
Verdict: ENGINE BUG (card-script + legality-oracle), NOT model, NOT render-cosmetic,
NOT harness-parser. The model's refusal is CORRECT reasoning about an illegal ask.

### The 4 records (all deck136 seat, kind=ask, fb=unparsed_reply, QUESTION field empty)
- vs22  seq7  turn3  Main1  (9 options)  -> refused, heuristic answered
- vs137 seq7  turn3  Main1  (9 options)  -> refused, heuristic answered
- vs18  seq7  turn3  Main1  (8 options)  -> refused, heuristic answered
- vs18  seq17 turn7  Main1  (4 options)  -> refused, heuristic answered
Distribution explained: deck runs Legion's End *2; the phantom fires once PER Legion's
End that resolves. vs18 resolved two copies (seq7 + seq17); the others one each.

### What actually happens (vs22 seq7 fully traced)
1. seq6 (kind=ask): model correctly casts "Legion's End - legal targets right now:
   Mogg Sentry" (601.2c cast filter + legal-target preview both WORK). choice=2.
2. Engine resolves the PRIMARY effect correctly: narration shows
   "Opponent's Mogg Sentry: battlefield -> exile" then
   "Your Legion's End: stack -> graveyard". The creature is gone, spell is in GY.
3. seq7 (the bug): engine issues a NEW ask rendered by the harness as
   "TARGET CHOICE for Legion's End (this spell/ability is already on the stack and
   needs a target ... Pick the ONE target it will affect)". The offered option set is
   contextless: cards in BOTH players' HANDS + lands (opponent's Mountain/Stinkdrinker/
   Inferno Titan AND your own Witch's Cottage/Mausoleum Secrets/Legion's End/Dread
   Presence/Castle Locthwain). NONE is a "creature an opponent controls MV<=2"; the
   opponent battlefield has 0 creatures. Option 7 is Legion's End itself.
4. Model refuses (correctly): "Legion's End is in your graveyard, not on the stack ...
   you cannot be asked to choose a target for a spell that has already resolved ...
   Opponent's Battlefield: 0 creatures." ~15k chars -> unparsed_reply -> heuristic.
5. seq8 (attackers) continues normally; NO extra card left any hand. Fallback HARMLESS.

### Mechanism (card-script)
borderline.txt Legion's End block:
  target=creature[manacost<=2]|opponentbattlefield      <- primary target, CORRECT
  auto=moveto(exile)                                      <- exiles the creature
  auto=all(*[share!name!]|targetcontrollerbattlefield) moveto(exile)
  auto=donothing notaTarget(*|targetcontrollerhand)      <- THE OFFENDER
  auto=all(*[share!name!]|targetcontrollerhand) moveto(exile)
  auto=all(*[share!name!]|targetcontrollergraveyard) moveto(exile)
The 4th auto line is a botched implementation of the Oracle clause "that player reveals
their hand". `donothing notaTarget(*|zone)` is a legitimate wagic "reveal/look" idiom
elsewhere (see borderline.txt:29396, 64987; mtg.txt:25690 -- all guarded by name(look)
inside an if/then or activated-ability frame). In Legion's End it is a BARE top-level
auto= line with NO name(look) frame, and it resolves AFTER the primary target creature
has already left the battlefield (moved to exile by auto line 1). With the target gone,
`targetcontroller` no longer resolves, so the zone selector degrades to "any hand" ->
the chooser offers ALL cards in BOTH hands (confirmed across all 4 records: options are
tagged [opponent's hand] and [your hand]; vs18 seq17 even offers Cavalier in YOUR hand).
The engine surfaces this `notaTarget(...)` selector as an interactive target ask.

### Two invariant violations (both named in the wave-27 brief)
(a) no-ask-without-legal-options: the legality oracle surfaces an ask whose options
    cannot satisfy the header's stated constraint ("creature an opponent controls MV<=2").
(b) ask freshness: the ask is issued for a spell already in the graveyard (resolved).

### Recommended fix (durable, engine-side; two candidate layers)
- PREFERRED (broad): a `notaTarget(...)` selector attached to a donothing/name(look)
  effect carries ZERO decision content (reveal-all / look) and must NEVER generate a
  model-facing ask -- auto-resolve it silently. This also covers the other 3 cards using
  the same idiom (Distress/discard/look effects) and any future one.
- OR (card-specific): rewrite Legion's End's reveal-hand line so it does not spawn a
  chooser, and/or capture targetcontroller before the primary target is exiled so the
  same-name hand/graveyard exile still scopes to the opponent only.
### Explicit NON-fix: do NOT add a guide crutch for this. The fallback is harmless
(heuristic answers, no game-state damage), and the LEGITIMATE Legion's End cast-target
seam (seq6/seq16 "legal targets right now: <creature>") works PERFECTLY. A guide line
telling the model "Legion's End asks are spurious, answer anything" risks poisoning the
working cast-target choice. Fix at the engine, teach nothing.

## Secondary card-script / Oracle-verification items (routed per wagicgpt-verify-oracle)
- Command the Dreadhorde: engine text= says "deals damage to you equal to the total mana
  value" -- SCRYFALL-CONFIRMED this IS the real Oracle wording (damage, not life loss).
  NO divergence. (Recorded because it looks like the more-common "you lose life" template.)
- Ritual of Soot: "Destroy all creatures with mana value 3 or less." SCRYFALL-CONFIRMED.
  Consequence for the guide (matchup fact, not a bug): MISSES Relentless Rats (MV4) and
  any MV4+ body; anthem/counter pumps do not change MV so it still hits pumped Kithkin.
- Legion's End text= matches Scryfall Oracle exactly (verified).
- Murderous Rider // Swift End: engine 2/3 lifelink // "Destroy target creature or
  planeswalker. You lose 2 life." SCRYFALL-CONFIRMED. (Flexible removal that also hits PWs.)
- Foulmire Knight // Profane Insight: engine {B} 1/1 deathtouch // adventure other={2}{B}
  "draw a card, lose 1 life." Consistent with Oracle.

## Unexercised decision classes (carry-forward, per skill "surface-unexercised" rung)
- Rankle "choose any number" combat-damage MAY-ASK: NEVER fired in 6 games (Rankle never
  connected / rarely resolved). Rendering of the multi-mode may-ask (wave-26 may-ask object
  naming) is UNVERIFIED at this seat. Carry forward; do not read as clean.
- Liliana Dreadhorde General LOYALTY activations (+1/-4/-9): cast ONCE (vs18 seq39, t15,
  game ended t18) with NO subsequent loyalty ask observed. Whether the harness surfaces
  planeswalker loyalty as an ask at this seat is UNVERIFIED. Carry forward.
- Ugin, the Ineffable: never cast in any game. Fully unexercised.
- Cavalier of Night ETB "may sacrifice -> destroy" DID fire cleanly (vs22 seq36-39:
  cast Cavalier, Sacrifice, chose Knight to sac, destroyed Inferno Titan). WORKS.
- Command the Dreadhorde multi-target reanimation WORKS (vs18 seq44-46: cast, chose
  Knight from GY, then "Done - no further targets"). Batch-target choreography clean.
- reveal kind (library search / Mausoleum Secrets tutor) WORKS: 1/game, 0 fallbacks,
  model selects a real card; the "does NOT qualify - goes to shuffle" MV annotation
  (deck131 seq12) renders correctly.

## Corpus integrity (deck136 seat)
6 games, 4 fallbacks total, ALL = R-RESOLVED-SPELL-PHANTOM-CHOOSER (unparsed_reply).
Zero other fallback classes. Reply-length p50 1200 / p90 6103 / p95 12246 / max 15903;
the top 3 spikes (15.9k/15.5k/15.4k) are ALL the phantom-chooser refusal -- i.e. the
single largest tax sink in the corpus is engine-caused and is NOT guide-addressable.
