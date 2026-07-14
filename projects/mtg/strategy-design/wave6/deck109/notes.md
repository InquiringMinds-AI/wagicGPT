# Dev notes — deck109 wave-6

## Engine defects (repro pointers)

### [FIXED THIS WAVE] Hellrider "cast no-op" — was a combo-hold HINT veto, not an engine no-op
Root cause identified: deck109.txt carries
`#HINT:combo hold(Hellrider|myhand)^until(creature|mybattlefield)^cast(Hellrider|myhand)^restriction{type(creature|mybattlefield)~morethan~2}^totalmananeeded({2}{R}{R})`.
The Baka combo-hold hint held Hellrider until `2+ creatures on battlefield` and, pre-fix, VETOED the
GPT model's explicit Hellrider pick when that restriction was unmet — presenting as a "cast no-op"
(mana unspent, card stays in hand) on a light board, resolving once the board went wide. This exactly
matches the wave-4/5 "board-width correlation" observation; the mechanism was the hint restriction.

Fix: commit **353f1a150** "Combo-hold hints no longer veto the GPT model's explicit pick."

Verification this corpus (`matchups-20260713-190011`):
- 140 `1783993523` seq38 (T25 MP1): Cast Hellrider, mana 8 -> 4, left hand. RESOLVED.
- 131 `1783994820` seq11 (T7 MP1): Cast Hellrider on a MODEST board (4 mana), left hand. RESOLVED —
  the formerly-vetoing condition, now clean.
- Corpus-wide no-op sweep (same-pick, mana-unchanged next record): 0 cast no-ops. 0 desyncs, 0 fallbacks.

Action taken: deck109 strategy.txt FALLBACK rule DELETED + Hellrider KEY-CARD parenthetical removed.
Core `hand -> stack` anchor: flagged for cross-deck retirement check (see general-suggestions.md #1).

## Stopgap ledger (for the platform-fix sweep)

| stopgap line | defect it patched | status this wave | action |
|---|---|---|---|
| deck109 FALLBACK RULE ("IF A CAST DID NOT HAPPEN…" + Hellrider parenthetical) | Hellrider combo-hold hint veto | defect FIXED (353f1a150); 2/2 casts resolved; rule was MISFIRING into a Cast-nothing durdle (131 seq6) | **REMOVED** from strategy.txt |
| core `hand -> stack` cast-happened anchor | same hint-veto / no-op class | defect FIXED from this seat; removal condition MET | flagged for CROSS-DECK confirmation then core retirement (A/B) |
| deck109 output-format block | reply-parser desync | already removed wave-5; still 0 desyncs | none |

## Harness asks / observations
- **options_text now present on ask/priority** and was used for the burn-hoard and cast-option sweeps —
  useful, keep it.
- Single round-robin run (6 games) — small sample; per-game conclusions rest on 1-2 instances. The two
  ACTED-ON conclusions are corpus-wide aggregates (Hellrider 2/2 resolves; blocker floor 0 high-life
  blocks / 2 low-life blocks), not single instances.
- No timeouts, no `choice==-1`, no empty replies in any deck109 game this run.

## Deck-construction observations (for the user — low priority, unchanged from wave-5, now CONFIRMED)
deck109 = 28 creatures / 12 spells (8 burn + 4 Krenko's Command) / 20 Mountains; mono-red, no color
issues. The one standing flag, CONFIRMED biting in the 133 loss:
- **No card advantage, no mana sink.** Reach is only the 8 burn spells; once they're spent (or not
  drawn) the deck top-decks with no way to convert excess lands into pressure. In the 133 LOSS the deck
  raced the opponent to 7 but emptied its hand (top-decking a Mountain at T12) with no more burn drawn,
  and lost the race by ~1 turn to Geralf's Messenger's undying ETB drain. Inherent to the archetype,
  not a defect. If the user wants flood/gas insurance: a small manland (Mutavault-style) or a top-end
  refuel (Reforge the Soul-style) would help. Not recommended to change at 5/6 — flag only.
