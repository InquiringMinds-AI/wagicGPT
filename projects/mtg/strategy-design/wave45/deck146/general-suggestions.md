# deck146 — core-prompt / render proposals (layer-routed)

Engine bugs and render gaps are in the seat file's ENGINE / RENDER ITEMS list (E-1..E-5); this file
is only for proposals to the CORE PROMPT text and to render conventions.

## G-1 (MED, render convention) — put activated-ability state on the option row
The `becomes beholder with Hive of the Eye Tyrant [cost: {3}{b}]` row is identical whether the Hive
is a land or already a 3/3 Beholder. The engine already annotates other option rows with the state
that changes their value: `{target text: ...}` on removal, `{right now: returns NOTHING - your
graveyard has no creature cards}` on Barrowin, `{X pricing: max affordable X=6 (9 mana total)}` on
Agadeem's Awakening. Extend the same convention to activated abilities whose effect is already in
place: `{already a 3/3 Beholder until end of turn - paying again adds nothing}`. Repro and counts in
seat-file E-2. This is a convention proposal; the code item is docketed there.

## G-2 (LOW, core prompt) — the plan-note's phase-mismatch class
The land-drop false-fire class is GONE (0 fires / 238 land-drop asks corpus-wide, was 69%) and my
audit found the note TRUE on 49 of 49 fires. About 14 of those 49 are still low-value: the plan
names a COMBAT action ("Attack with all available creatures") and the menu is a cast or priority
menu, so the note is literally true and tells the model nothing it can act on. If the gate is
tightened further, the cheapest next cut is to suppress the note when the plan's named actions are
all of a different decision KIND than the current ask. Not urgent — precision is already high and
the note is short.

## G-3 (LOW, core prompt) — the reply-protocol example and multi-part sequences
No fallback occurred on this seat this corpus (was 6), so nothing here is failing. Recording one
observation for the docket: deck146's dungeon/room asks and its `Choose an option for <spell>:`
menus are multi-ask sequences (cast -> mode -> target) and the model answered every one of them
correctly, 8/8 dungeon and 14/14 room. Whatever the E-3 header fix did, it did not disturb the
sequence handling; the amendment-#37 "a multi-ask sequence is one governed window" concern is not
visible on this seat.

## G-4 (LOW, core prompt) — nothing to report on the general combat hint
The blockers prompt's general hint ("taking damage while ahead on LIFE is often correct") appeared
in every one of my 22 blocker windows and I found no reply that quoted it against a guide rule.
The wave-41 concern (amendment #1) stays discharged on this seat.
