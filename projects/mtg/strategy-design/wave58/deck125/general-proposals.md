# deck125 — proposals for the general guide (wave-58)

Numbered in wave57/general-strategy.md's style. Counts are from
`matchups-20260904-015059` unless a file is named.

## CORE-PROMPT / TRANSPORT, HIGH

**P1. `empty_reply` is a ~2.5 s deadline being hit, not a model refusal — classify it as a
timeout and retry once before falling back.** All **89 of 89** `empty_reply` records corpus-wide
carry `latency_ms` in **2502-2558 ms** (56 ms of spread across 89 events). Successful replies on
the same seats have per-game medians of 2.7 s to 30 s and reach 142.9 s. A model that produced
nothing would not do it 89 times inside a 56 ms window. Repro: `125v126` s27, s30, s32, s34, s36
(five in a row, turn 12, each `reply: ""`, `coded_answers: 0`). Ask: find the 2.5 s deadline in the
client path, raise or remove it, and stamp the fallback `deadline_2500` rather than `empty_reply` —
the current name has been read as "the model refused" in guide-side reviews, which is the wrong
diagnosis of a lost decision.

**P2. The `PUT:` parser must accept the trailing name gloss that `CHOICE:` requires.** Every
`CHOICE:` ask instructs `e.g. "CHOICE: 1 (Cast Supreme Verdict)"`; the cleanup/bottom asks
instruct `e.g. "PUT: 2, 5"` with no gloss and refuse one. `125v130` s83 replied
`PUT: 9, 1 (Supreme Verdict, Fall of the Gavel)` — the required numbers, in order, with the
protocol's own naming convention appended — and was refused `unparsed_reply`
(`coded_answers: 1`, `post_answer_overrun: 171`). 71 `PUT:` asks corpus-wide, 2 unparsed, 1 of
them this shape. Ask: strip one trailing parenthetical from a `PUT:` line before parsing, and ship
it with its negatives (a `PUT:` line with no numbers still refuses; a parenthetical containing
digits does not become a number).

## RENDER, MED

**P3. A damage-to-attackers row should print its KILL COUNT, the way the sweeper rows do.**
`125v146` s38 printed
`Cast Lightmine Field {2}{w}{w} {right now: they control 2 creatures able to attack - deals 2 to
each if all 2 attack}` two rows from
`Cast Final Judgment {4}{w}{w} {right now: exiles 2 of their creatures ...}`. The pilot can only
reach the kill count by joining the row's N to the toughnesses on the battlefield line. The
evaluator already enumerates the able-to-attack set. Ask: append
`- kills N of those M (toughness <= N)`. 43 such rows in deck125's six games, 1 take, and four of
the declines are one lost game.

**P4. The `[<- best X for this cast]` / `[<- largest affordable X]` marker must read the NET tail
printed on its own row.** `125v162` s41 at 3 life:
`1. X = 2 {... NET -2 life for this cast} ... [<- largest affordable X - X=2 gains 2 life and draws
2 cards; no listed X does more]`. The marker is computed from mana alone, so `no listed X does
more` is asserted over a row the render has just priced as lethal; the pilot took option 1 and the
game ended at 0 with no further ask. 11 X menus corpus-wide, 1 carried a NET tail, 1 of 1 taken.
Ask: suppress the marker when the row's NET would take printed life to 0 or below, or move it to
the largest X that survives. This is skill #319's shape at a second marker (#248: the marker is the
most-followed annotation the render emits).

**P5. The LIFE-TO-DAMAGE CONVERTER paragraph should close by naming the direction that is SAFE.**
57 windows corpus-wide print it. The body is right; the close — "Any life they gain, or any life
you lose, chains until YOU are at 0 - so ANY nonzero payment on a tag above is fatal" — reads as
a statement about life events in general. `125v126` s44 and s48: the pilot wrote "Sanguine Bond
converts any gain to damage" into its plan and declined a `max affordable X=5` Revelation with
eight untapped lands. Ask: add `(life YOU gain is not converted by a converter of theirs - only
life you PAY chains)`. One clause, on a paragraph that already carries the true rule.
