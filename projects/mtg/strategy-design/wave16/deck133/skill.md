# deck133 wave-16 — strategy-writing-skill proposals

Corpus: `matchups-20260716-084532` (binary 69793930a; wave-15 engine batch — zero-mana free casts,
the stale_echo option-subset-of-echo matcher, hybrid-pip annotation). This is the post-ship
VALIDATION wave for the two items deck133 owns (item A own-target-removal recurrence, item B
stale_echo superstring false-positive). Single-seat, so nothing here adds or cuts a core method line.

## Confirmation (no change) — both owned fixes validated at this seat.

- **The stale_echo option-subset-of-echo matcher (item B) did its job by absence.** Wave-15 flagged
  the superstring false-positive (echo names an offered option only as a superstring -> wrongful
  downgrade). This corpus: ZERO wrongful superstring downgrades at this seat. The Mechanism-A/B
  scoring split the skill adopted, plus wave-15's added false-positive-direction check, held.
- **The own-target-only removal annotation (item A) held 100% (0/20 misfires).** The wave-15
  self-Fatal-Push was a one-off; the "check both error directions" scoring rung correctly counted it.

## Proposal 1 (WATCH — a THIRD stale_echo cause the scoring rung should name: the DEGENERATE-OPTION-LABEL false-positive)

**A shipped absent-echo/stale_echo route has (at least) THREE downgrade causes a reviewer scoring it
must separate, not two: (a) the LEAK direction — echo names no option, executes a raw index (closed
wave-13); (b) the SUPERSTRING false-positive — echo names an option only as a superstring (closed
wave-15); and now (c) the DEGENERATE-LABEL false-positive — the echo cannot match because the OPTION
ITSELF is serialized to a contentless stub (e.g. a target sub-menu whose entries render as a bare
`Becomes ` with no creature name and no resulting stats). Cause (c) is invisible to a superstring
check (there is no name to superstring-match against) and is a HARNESS/SERIALIZATION defect, not a
matcher defect.**

Witness: deck133 vs62 seq29 — Liliana, the Last Hope `+1` target menu serialized as
`['Becomes ', 'Decline - do nothing']`; the model's echo `CHOICE: 1 (Activate Liliana's +1 ability
on Canopy Spider)` was CORRECT and present-in-intent but could not match the stub label -> stale_echo
-> heuristic (which fired the +1 anyway; non-deciding). A reviewer counting "stale_echo fired N
times, all correct" would miscredit this: the route DID correctly refuse to guess, but the underlying
cause is a serialization bug that should be fixed upstream of the matcher (emit the full target-menu
label), and it will keep tripping the route until then.

**Authoring consequence (sharpen the stale_echo scoring rung):** when scoring stale_echo events,
bucket them into (a) leaks, (b) superstring false-positives, AND (c) degenerate-label
false-positives (echo names a coherent, present intent but the OPTION LABEL is a contentless stub).
Bucket (c) points at the serialization layer, not the matcher — do not "fix" it by loosening the
matcher (there is nothing to match) and do not credit it as a clean catch.

Convergence status: **single seat (deck133), 1 firing, non-deciding, one corpus.** Per the epistemics
rule it does not add or cut a core method line; offered as a SHARPENING of the stale_echo scoring
rung. Adopt into the method only if a second seat shows a degenerate-label downgrade this wave;
otherwise WATCH.

## No other skill changes.
The freeze/demotion doctrine, the bare-N/shown==resolved GM terminus, the own-targets-removal
representation item (0 recurrence this wave — downgraded to parked WATCH, notes #A), and reply-protocol
layer-routing all held and required no revision from deck133's evidence.
