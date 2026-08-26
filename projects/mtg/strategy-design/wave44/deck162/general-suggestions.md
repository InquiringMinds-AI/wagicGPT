# deck162 — core-prompt / render proposals (layer-routed)

Engine BUGS are in the seat file's engine list, not here.

## G-9 (HIGH, render text) — the collapsed blocker summary hides gang-block lethality
Repro: deck162 vs deck123, `attackers` seq 13. The tag read
```
[their untapped blockers: they have 6 untapped creatures that could block this one,
 biggest Bloodline Keeper #1 (3/3) (you kill it, your attacker lives)]
```
The model swung, three 2/2 Vampires gang-blocked, and the 5/5 Master of the Feast died.
Every word is true and the footnote does say "before gang-blocks", but the only OUTCOME the line
prints is a favourable one, on a board where the unfavourable outcome is trivially available.
PROPOSAL: when the render collapses the blocker list, price the collapse instead of the biggest
single blocker - e.g. "they have 6 untapped creatures that could block this one; any 2 of them
together deal 4, enough to kill it" or, minimally, "biggest X (1-on-1: you kill it, your attacker
lives) - but 6 blockers can gang this attacker". The threshold at which the enumerated list
collapses should also be recorded somewhere the reviewers can see, since guide rules are written
against the enumerated form.

## G-10 (LOW, render text) — singular loyalty loss carries no "(now N)"
The plural branch is fixed: "got 3 loyalty counters (now 4)" appears 437 times corpus-wide with
"(now N)". The SINGULAR branch is not: "- Your Ob Nixilis, the Hate-Twisted lost a loyalty counter"
(deck162 vs deck146 seq 16; also vs deck126 seq 13, vs deck130 seq 63) and "- Your Teferi, Who Slows
the Sunset lost a loyalty counter" (deck152 vs deck125 seq 78). Corpus-wide, 8 of 14 unique
loyalty-LOSS log lines carry no "(now N)" while 20 of 25 GOT lines do.
PROPOSAL: route the singular loss through the same formatter as the plural one.

## G-11 (INFO) — lane #W44-5, half PASS half FAIL, evidence attached
The `TeferiEffect` counter is now rendered in the LOG as "a bookkeeping (Teferi's +1) counter" -
readable. But the STACK section still leaks the raw lexicon token:
`3: ability: teferieffect Counter Removed [triggered/activated ability]`
(deck152 vs deck125, seq 52, 55, 62 - three instances, all in the same game). Those same seqs are
where two of the three `stale_echo` fallbacks fired. Dungeon rows carried no zone tag anywhere in
my seats' 16 dungeon-choice windows.
