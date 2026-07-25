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

---

## RE-PROBE (fixed binary 79e39511f)

One live GPT-vs-GPT game, deck199 (Tergrid) seat vs deck198, `WAGIC_AI=gpt`,
`WAGIC_GPT_REPPENALTY=1.05`. Result: **deck199 LOST** (winner=1/deck198, life 8 vs -3, turn
23). Artifacts: `reprobe-g1-deck199.jsonl` (steal seat, 32 recs), `reprobe-g1-deck198.jsonl`,
`reprobe-g1.stderr`.

### Verdicts

**Tergrid parse error GONE: YES.** The two steal-trigger lines
(`@sacrificed(nontoken)`/`@discarded(permanent)` → `may name(Put onto battlefield under your
control) all(trigger[to]) moveto(mybattlefield)`) produced ZERO `ABILITYFACTORY ERROR`. Tergrid
cast normally (stderr: "casting Tergrid, God of Fright (model's pick)", turn 15) and its trigger
FIRED live (see below). The one `ABILITYFACTORY ERROR` in stderr is the pre-existing
**R-DFC-BACKFACE** bug — `ABILITYFACTORY Error parsing: {0}:doubleside(tergrid's lantern)` —
exactly as documented (unfixed, expected). No steal-line regression.

**Steal window occurred: YES.** deck199 seq 21 (turn 16/17, Main phase 1). The seat cast Soul
Shatter forcing the opponent to sacrifice Bog Wraith (`Opponent's Bog Wraith: battlefield ->
graveyard`), which armed Tergrid's `@sacrificed(nontoken)` steal trigger. Exactly one steal ask
in the whole game (opponent's other discards were Swamps before Tergrid hit the board / hand
empty afterward).

**The may-ask verbatim render (from deck199 seq 21 prompt):**
```
Choose an option for Tergrid, God of Fright:
1. put onto battlefield under your control
2. Decline - do nothing
```

**Model's decision: DECLINED (choice 2, "Decline - do nothing") — a MISPLAY.** The
take-the-steal guide line was **NOT cited/applied at the decision**. The model's earlier PLAN
line did carry guide-flavored intent ("steal permanents if the opponent recovers"), but at the
actual ask it abandoned it.

**Card landed under the steal controller: NO.** Bog Wraith stayed in the graveyard (never
`-> your battlefield`); deck199's board after the decision held only Tergrid. The free 3/3 was
refused, and deck199 went on to lose.

### NEW render/ledger issue (SIGNIFICANT — the fix parses but the ask is unusable)

The `may` steal ask does not communicate what is being offered. TWO compounding defects, both
visible verbatim in seq 21:

1. **The stolen card is UNNAMED.** The option label is the literal `name(...)` string "put onto
   battlefield under your control" — it never interpolates "Bog Wraith". The model is asked to
   put *something* onto the battlefield with no indication of what card, from where, or that it
   is the just-sacrificed permanent.

2. **The ask is attributed to Tergrid itself** ("Choose an option for **Tergrid, God of
   Fright**"), identical framing to Tergrid's own modal/DFC cast choice. With no named object,
   the model read "put onto battlefield under your control" as referring to **Tergrid** and
   concluded the prompt was a glitch: its reply reasons at length "Tergrid is *already* on the
   battlefield according to 'Your battlefield'... choosing 'put onto battlefield' is impossible
   ... I will select Decline." The confusing render directly caused the model to throw away free
   value.

Root cause is in the card script / trigger-render seam, not the parser: the trigger's `may`
prompt surfaces a static option label and Tergrid's card name, but never the moved object's
identity. A faithful render would name the stolen card and its source (e.g. "Tergrid's ability:
put the sacrificed Bog Wraith onto the battlefield under your control?"). Until that lands, the
steal will keep being declined-by-confusion even though the mechanic now works — mechanically
FIXED, decision-legibly BROKEN.

### Unobserved
- Multi-object / @discarded(permanent) steal path: UNOBSERVED (only one @sacrificed window
  arose; opponent discards were lands before Tergrid or with an empty hand after).
- Model taking the steal: UNOBSERVED (the single window was declined; cannot confirm the
  landed-under-controller board path live, only that the decline path leaves the card in yard).

---

## FINAL PROBE (may-ask render fix, pre-commit validation)

R-MAY-ASK-UNNAMED-OBJECT fixed (DecisionContract resolveTriggerObject + AIPlayerGPT
buildMayObjectAsk — the ask now names the moved object, its origin zone, and identifies the
trigger source as SOURCE, not the object acted on). Live one-shot validation on the fixed
build: Soul Shatter forced the opponent to sacrifice Wall of Bone; the steal ask rendered:

```
Triggered ability of Tergrid, God of Fright - you MAY act on Wall of Bone (from opponent's
graveyard). Tergrid, God of Fright is the trigger SOURCE, not the object being moved. Choose:
1. put onto battlefield under your control [Wall of Bone (from opponent's graveyard) is the
   card this acts on, NOT Tergrid, God of Fright]
2. Decline - do nothing
```

**The model chose option 1 — TOOK THE STEAL** (translog seq 32: `Your Wall of Bone:
graveyard -> battlefield`). Exact inverse of the pre-fix decline. The steal chain is CLOSED
end-to-end: parse (agent C) -> fire (re-probe) -> legible ask (agent D) -> correct model
play (this probe). deck102's take-the-steal guide line is discharged; deck102 rotation per
wave-25 synthesis may proceed after its wave-26 seat review.

Scope guard confirmed live: the Tergrid DFC cast-mode menu still renders plain (no
double-annotation); ETB pay-or-tap menus untouched. Remaining open at this card: the
benign battlefield doubleside ABILITYFACTORY line (NULL-by-design, no longer poisoning),
R-DFC-BACKFACE (fix map on ledger), primitives-only set-hint silent drop (ledger, low).
