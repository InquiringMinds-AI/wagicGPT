# Wave-35 35B corpus: 146v36 stale-answer livelock (2026-08-21, LIVE capture)

20/21 games finished naturally; this game livelocked at ~turn 10 for 4.6h+ until the -T cap.
Trigger (stderr): a Land drop ask for Hive of the Eye Tyrant answered "play now" ->
FindCardToPlay picks it, cost "free to play" -> the land-play COMMIT NEVER ADVANCES STATE ->
engine re-offers, model re-answers, every answer drops as stale ("dropping stale async
answer" x341, one per model-latency interval). The ~2% self-healing stale-drop path becomes
a permanent loop when the underlying commit is a no-op.
OPEN QUESTIONS for the fix: why does the Hive land-play click choreography no-op in this
state (already-played land drop? the c5 plan validation rejecting silently?); and the GPT
seat needs a livelock breaker - N consecutive stale drops of the SAME ask => fall back to
Baka for that decision (bounded), never loop for hours.

## SECOND instability (2026-08-22, 122B wave 146v139, INTERMITTENT — not fixed yet)

gdb catch (ll5, drop at AIPlayerGPT.cpp:3081): both keys untagged, identical through the
PLAN line; the REBUILT prompt inserts the stale-plan caveat ("this decision's list does not
contain the actions your plan names") while the SPAWNED prompt goes straight to a cast menu
whose option 1 IS the plan's named card (Agadeem's Awakening {X}). The caveat is a pure
function of (plan, options) => the CAST MENU OPTIONS themselves flicker between rebuilds of
the same state — suspicion: X-cost castability / potential-mana evaluation instability for
the X spell. Effect: occasional stale drop + a wasted phase-1 round trip (20 drops across a
28-turn game; game still progresses — cost, not livelock; each phase-1 consume resets the
6-drop breaker so it correctly does not fire). FIX DOCKET: (a) find the nondeterminism in
legalCasts/X evaluation with a fixture at the potential-mana boundary; (b) consider keying
staleness on the ASK content minus volatile annotations. Evidence: task outputs ll5 (inline
diff, diverge at 13769) in session; game 146v139 epoch 1787301017.
