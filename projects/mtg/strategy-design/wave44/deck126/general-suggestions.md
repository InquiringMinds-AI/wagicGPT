# Core-prompt / render proposals — deck126 seat (wave 44)

(Identical to deck125/general-suggestions.md — the same four items were observed on both
seats of this reviewer; deck126 counts are given inline where they differ.)

Layer-routed. Engine defects belong in the seat file's engine list; nothing below is an
engine bug, so all of it is prompt or render text.

## G1 (MED, core prompt) — suppress the plan-mismatch note on land-drop windows

`#W44-7` predicted the resurrected plan-note gate would fire ~0% on attacker windows. On my
seats it did: **0/20** attacker windows and **0/1** blocker window carried the note. That
part PASSED. The residual is land drops.

| window kind (deck125, 545 decisions) | note fired |
|---|---|
| land drop | 137 / 140 |
| casting decision | 180 / 349 |
| priority | 15 / 28 |
| X announce | 0 / 14 |
| mulligan / bottom | 0 / 8 |

A land-drop menu contains nothing but `Play <land>` and `Play no land right now`. It can
never contain the actions a plan names, so the note is true by construction and carries no
information — it fires on 98% of them. Proposal: gate the note off entirely for land-drop
windows (and for the X-announce sub-window, though that already reads 0). deck126 shows the
same shape at 43/52.

The note's text is good where it fires meaningfully; this is a predicate scope fix, not a
wording fix.

## G2 (LOW, render) — the X-announce window still shows the spell in hand, with no stack line

In **14/14** Sphinx's Revelation X-announce windows the card being cast is still listed on
`Your hand:`, the hand count is identical to the preceding casting window, and there is no
`ON THE STACK` section. Repro:
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260826-111937/1787761192-ai_baka_deck125-0x555929beb550-vs-ai_baka_deck146.jsonl`
seq 86 (hand 8, casting decision) and seq 87 (hand 8, X menu).

This is arguably correct Wagic state — X is announced during casting — but any guide that
asks the model to size X against its hand is then off by one, and the model cannot tell from
the frame that the card is on its way out. Two cheap options, either is fine:
(a) annotate the row, e.g. `Sphinx's Revelation {u}{u}{w}{x} [instant] [being cast now]`;
(b) print a one-line `ON THE STACK: your Sphinx's Revelation (X not yet chosen)` header in
the announce window.
I worked around it in the guide (`SUBTRACT 1`), so this is a convenience, not a blocker.

## G3 (LOW, render) — duplicate copies in a target clause carry no {target text:}

Target clauses print `{target text: ...}` for the first card of a name and omit it for later
copies. Repro: `1787761221-ai_baka_deck125-0x558033705f60-vs-ai_baka_deck152.jsonl` seq 21 —
`Brutal Cathar #1 ... {target text: "When this creature enters or transforms..."}, Elite
Spellbinder #1 ... {target text: "Flying -- When Elite Spellbinder enters..."}, Brutal Cathar
#2 {2}{w} (creature 2/2) [daybound], Elite Spellbinder #2 {2}{w} (creature 3/1) [flying]`.
Same shape at `...vs-ai_baka_deck126` seq 76 (Perimeter Captain #1 with text, #2 and #3
without).

The dedup is sensible for length. It only bites a guide that tells the model to read the
text of the candidate it is choosing — if the model picks #2 there is no text on its row.
Cheapest fix if it is ever worth doing: on the later copies print `{target text: as #1}`.
I found **no decision this corpus where a duplicate copy was the chosen target**, so this
is LOW and speculative.

## G4 (keep — a render line that is doing its job)

The variable-mana annotation is exactly the right shape and should be preserved verbatim as
a pattern for other variable sources:
```
Overgrown Battlement {g} (VARIABLE output: this ONE source adds more than one mana - its amount
is a count, read its own rules text; the total above counts it as one source, not as its mana)
```
It states the render's own limitation inside the render, which is what let me correct
deck126's guide rather than file a bug. If anything is added later, the computed number
(`x2, 2 defenders`) would remove the last step of arithmetic — but the caveat alone is
already honest and sufficient.
