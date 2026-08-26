# deck162 — core-prompt / render proposals (layer-routed)

Engine bugs are in the seat file's ENGINE / RENDER ITEMS list; this file is CORE PROMPT and render
conventions only.

## G-9 (informational, render) — the menace lane rendered correctly on its only window
The corpus contains exactly one `blockers` record where a menace creature attacks (162 vs146 seq
19). Everything the lane shipped is present and correct there: the A-row's
`[menace - only a block by TWO OR MORE of your creatures counts; one creature alone does not block
it at all]`, every B-row's `(no 1-on-1 result exists - only a block by TWO OR MORE of your creatures
counts; this one alone does not block it at all)` instead of a bare verdict, and the zero-power
blocker's `stops NOTHING there` branch on both Shield Spheres. Behaviour was correct too (walls
assigned to the two non-menace attackers, no solo menace block). One window is not a rate — the
probe deck is still owed — but nothing here needs changing.

## G-10 (LOW, metric) — dropped_assignments still counts `Bn:none`
The corpus's only non-zero `dropped_assignments` is this seat's vs152 seq 14, reading **7**, on a
window that offered ONE blocker; the reply was `B1:A2, B2:none, B3:none, ... B8:none`. Nothing was
dropped. Until the metric excludes explicit `none` assignments it reads 7 where the true corpus
count is 0, which will mask a real menace-caused drop the first time one occurs. Docketed as
seat-file E-4.

## G-11 (LOW, core prompt) — the lethal-check vocabulary is doing real work
deck162's ATTACKING and LETHAL CHECK sections both refer to "the lethal check has cleared", and the
model used that phrase correctly in the windows where it applied. No change proposed; recording it
because it is the one piece of cross-section vocabulary on this seat that survived a wave without
being quoted against itself, and it is worth copying rather than reinventing when other guides need
a named gate to point at.

## G-12 (informational) — X pricing on a non-damage spell
21 X-spell windows on this seat's sibling deck146, all `Cast Agadeem's Awakening {b}{b}{b}{x}
{X pricing: max affordable X=N (M mana total)}` with no `kills THEIRS:/YOURS:` list. That is the
correct branch for a spell with no damage clause, not a coverage hole — recording the
classification so the lane's "kills present in only 2 records" pre-count is not read as a miss.
