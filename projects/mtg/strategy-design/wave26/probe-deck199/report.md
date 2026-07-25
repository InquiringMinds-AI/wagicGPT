# deck199 probe report — steal-window + R-DFC-BACKFACE validation (2026-07-25)

Probe run on binary 62d8783a5 (HEAD e23dba705), Spark qwen35 @ :8081, headless GPT-vs-GPT
oneshot, DECK0=199 (stacked Tergrid) vs DECK1=198 (tokenless nontoken-permanent opponent),
REPPENALTY=1.05. Two games (game 1 = deck-load dud, see below; game 2 = both legs). Probe
decks deleted from bin/Res/ai/baka/ after harvest (verified highest deckN=164); decklists
preserved here.

## VERDICTS

### Backface leg — R-DFC-BACKFACE-RESOLVES-FRONT: REPRODUCED / BUG CONFIRMED
The model cast **Tergrid's Lantern {3}{B}** (back face) and the engine deployed **Tergrid,
God of Fright {3}{B}{B} (4/5 menace)** — the FRONT face. Paid the cheap back-face cost,
received the strictly-better front permanent. Rules-wrong, benefit-side.
- g2 translog seq24 (t8): chose "Cast Tergrid's Lantern {3}{b}" (PLAN intended the Lantern).
- seq25: cast-mode menu -> "Cast Card Normally".
- seq26 (t9) narration: `Your Tergrid's Lantern: hand -> stack` then
  `Your Tergrid, God of Fright: stack -> battlefield`; board shows
  `Tergrid, God of Fright {3}{b}{b} (4/5) [menace]`.
- g2.stderr:549 `casting Tergrid's Lantern (model's pick)` -> :557 `Action added to stack:
  Tergrid, God of Fright` -> :560 `Resolving Action on stack: Tergrid, God of Fright`.
The card's `autostack=if paid(alternative) then flip(...)` does not fire on a normal cast.
(Post-probe engine diagnosis, wave-26 agent C: AllAbilities.cpp:5422 resets isFlipped for AI
controllers, and the autostack flip keys on paid(alternative) which a normal cast never sets.
Fix deferred — shared DFC/MDFC cast path with no suite coverage; ledger carries the fix map.)

### Steal leg — modal FIRED: NO. Root cause: THE ABILITY NEVER PARSES.
A genuine steal window occurred (opponent forced to sacrifice nontoken **Scathe Zombies** to
Plaguecrafter with the God live since t9; g2 translog seq33-34, g2.stderr:757-782) and NO
"Put on battlefield / Don't put on battlefield" modal appeared. Discovered cause:
`ABILITYFACTORY ERROR: Parser returned NULL` on BOTH the `@sacrificed(...)` and
`@discarded(...)` lines (g2.stderr:581-582, firing right after the God resolves) — the steal
ability was never installed on the card, in ANY wave.

**This OVERTURNS wave-25 deck102 findings.md §1** ("the line is correct and un-fired, not
broken"). The 3-wave validation debt was misdiagnosed as random-pool inability; the true
cause is that Tergrid's steal has never functioned. (Wave-26 agent C follow-up: the parse
NULL is a general parser state-leak — an `anyzone={0}:doubleside(...)` line, NULL-by-design
on the battlefield, poisons the parse of the NEXT ability line. Fixed card-script-side by
faithful-Oracle rewrite + reordering the trigger lines above the doubleside line; fixtures
tergrid_steals_sacrificed_creature / tergrid_steals_discarded_permanent /
tergrid_ignores_sacrificed_token now guard it.)

Caveat recorded: the g2 God arrived via the buggy Lantern-flip path so its isflipped state
is unclear, but the ABILITYFACTORY error is parse-time on the static string — operative
regardless; the isflipped question is moot for the parse verdict.

## Game-1 dud — deck-load bug found
deck199 v1 used `Tergrid, God of Fright (KHM) *6`; Tergrid never appeared in 17 turns.
Cause: MTGAllCards::getCardByName (MTGDeck.cpp:1043) drops a card whose setId != the set
hint; Tergrid exists ONLY in primitives/borderline.txt (INTERNAL_SET) — the 6 copies were
silently dropped. Fixed with `(*)` (setId=-1, name-only match) + bumped to 8x.
**Ledger [NEW, low]: primitives-only card + real set hint = silent drop from a main deck**
(loud-rejection candidate at MTGDeck.cpp:1043; deck102's `(KHM)` works only in the #CMD:
commander slot, a different load path).

## Minor
Flip-thrash recurred at the probe seat under mana pressure (g2 seq18/19, two "Flip Side ...
DISPLAY TOGGLE only" pseudo-actions despite the deck102 guide demotion) — and the leftover
Lantern-face display steered the t8 cast menu into the backface-bug path.

## Follow-up
Re-run this probe ONCE on the Tergrid-fixed binary to observe the live `may` steal ask and
the model's take-the-steal decision (the deck102 guide-line validation); then deck102
rotates per the wave-25 synthesis. The backface bug will still reproduce until its engine
fix ships — expected, not a probe failure.

## Artifacts
deck199.txt / deck198.txt / deck199_strategy.txt (copy of deck102 guide), g1/g2 translogs +
stderr. All probe decks removed from the live opponent pool post-harvest.
