# Wave-62 corpus (matchups-20260905-133058): 152v125 HUNG — designation-marker guard hid markers from the daybound count

**What happened.** 21 games launched 13:31 on `12be0b79c`; 20 ended naturally; `deck152 vs deck125` wrote no seat
record for 3,609 s and the harness killed it (Invariant 00: engine defect, rerun owed). Its stderr: 88,232
`Action added to stack: Day|Night` lines, 265,020 live `MTGCardInstance` (702 MB), 1.18 M `ManaCost`. The loop
began the moment the 152 seat cast Brutal Cathar at turn 27 with a Night marker already in play.

**Mechanism (verified in the scripts and by probe).** Lane Z's D16 guard (`TargetChooser::canTarget`: an
Emblem-typed card is refused unless the chooser `acceptsDesignationMarkers()`, which only a chooser naming the
Emblem TYPE did) also hid the markers from the choosers the daybound machinery itself uses — Brutal Cathar's
`if type(*[day;night]|battlefield)~equalto~0 then ... castcard(noevent named!:Day:!)` and the markers' own
`type(*[nonight]|battlefield)`. `TypeTargetChooser` matches a type entry against the card's NAME
(`findType(card->name) == types[i]`), which is how `[day;night]` finds a card named Day or Night. With the
guard the count read zero beside a live Night marker, a second, opposite marker was cast, and the two
`lord(*[daybound|nightbound]) flip(backside)` effects flipped each other's permanents forever. The other
three 152 games in the corpus show 1–2 marker adds (normal); wave-61's rerun corpus likewise. The suite's
existing `brutal_cathar_cast_from_hand.txt` has no prior marker, so it never met the shape.

**Fix (this commit).** `acceptsDesignationMarkers()` is true when any listed type names a marker: the Emblem
type, a marker card's name (Day, Night, The Monarch, The Initiative, The Ring, City's Blessing, plus bare
forms), or its negation (`nonight`). Every other chooser still never sees a marker, so D16's annihilator fix
stands. Fixture `w62fix_daybound_marker_seen_by_count.txt` (Night in play, cast Brutal Cathar): GREEN on the
fixed tree. **RED half, stated plainly:** the markers are invisible to zone asserts and the harness gives the
Cathar's ETB no target, so the assert alone does not fail on the pre-fix body — measured on the probe, the
pre-fix body produced 71 Day/Night stack adds inside the test, the fix 2 (the harness's own placement of
the marker). The corpus hang is the RED.

**Rerun.** `-p 152,125 -j 1` on the fixed binary (unit `w62-rerun`), owed before step three reads the corpus.
