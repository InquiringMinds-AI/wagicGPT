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
