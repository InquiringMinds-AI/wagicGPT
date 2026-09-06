# Wave-63 corpus (matchups-20260905-191148): 152v162 and 152v130 HUNG — the daybound marker count reads zero, wave-62 fix never engaged

**What happened.** 21 games launched 19:11 on `98d13050f`; 19 ended naturally; `deck152 vs deck162` (5.5k Day/Night
stack adds, 1,144 repeated Brutal Cathar exile targetings) and `deck152 vs deck130` (40k adds, 1.1 M `ManaCost`
objects) wrote no seat record for 3,622 s and the harness killed each (Invariant 00: engine defect, reruns owed).
Both loops began at an UNTAP right after a turn in which the marker's end-step check fired: the Day marker flipped to
Night in place, Night's `lord(*[daybound]) flip(backside)` transformed the 152 seat's Brutal Cathar into Moonrage
Brute, and the Brute's enter script `if type(*[day;night]|battlefield)~equalto~0 then ... castcard(Night)` cast a
SECOND Night. From there the wave-62 shape (two markers flipping each other's permanents) ran forever.

**Mechanism (verified by probe, `WAGIC_DAYNIGHTPROBE=1`, dev-only in `MTGAbility.cpp`).** `*[day;night]` is `*` plus
bracketed attributes, which `TargetChooserFactory` builds as a **DescriptorTargetChooser** — not a TypeTargetChooser.
The wave-62 fix `9230a94c3` put `acceptsDesignationMarkers()` on TypeTargetChooser only; the descriptor chooser kept
the base-class `false`, so every marker count read 0 beside a live marker on EVERY binary since D16, including
`9230a94c3` itself. The wave-62 fixture `w62fix_daybound_marker_seen_by_count.txt` loops 73 times on that binary and
still passed, because markers are invisible to zone asserts — the "pre-fix 71 adds, fix 2" measurement recorded in
`wave62/corpus-hang.md` was wrong (re-measured today on the archived `wagic-9230a94c3-w62hangfix`: 37 Day + 36
Night adds). Why the corpora between did not hang: the loop needs a marker to FLIP beside a werewolf (a spell-less
turn under Day, or ≥2 spells under Night) — wave-62's rerun and wave-61's corpus had 1–2 marker adds per 152 game,
i.e. the Day cast and no flip.

**Fix (this commit).** `DescriptorTargetChooser::acceptsDesignationMarkers()` returns true when the descriptor's type
list (positive or negated) or its name names a marker (same `typeNamesDesignationMarker` table as the type chooser).
D16 stands for every other descriptor. Probe on the fixed tree: the count reads 1 beside the marker; the Brute casts
nothing; zero marker stack adds in either fixture.

**Fixtures.** Markers are invisible to zone asserts, so the RED is read through a test-only creature (Lexicon Synthetic
Night Counter, id 2100000604: loses its controller 5 life at each upkeep while more than one Night marker is out — a
plain type chooser, which sees markers). `w63fix_daybound_flip_casts_no_second_marker.txt` (Day + Cathar, spell-less
turn, next upkeep): RED on `wagic-98d13050f-w63step1` (life 15), GREEN on the fix (life 20, Cathar transformed).
`w62fix_daybound_marker_seen_by_count.txt` rewritten the same way (Night in play, cast Cathar, next upkeep): RED on the
archived binary (phase overshoot + Cathar not transformed after the loop), GREEN on the fix. Placement order in a
fixture matters: list the marker before the werewolf, or the werewolf's own enter script casts a legitimate marker
beside the placed one (that is the wave-62 two-marker shape, not this one).

**Rerun.** `-p 152,162 -j 1` then `-p 152,130 -j 1` on the fixed binary (unit `w63-rerun`), owed before step three
reads the corpus; the two hung games' partial translogs are evidence of the hang only.
