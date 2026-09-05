# Wave-61 corpus (matchups-20260904-220227): stale-drop livelock — killed at 11 h, rerun owed

**What happened.** Launched 22:02 on master `5faa9e970`. At 09:15 the next morning 17/21 games
had ended and four were still open (123v130 turn 32, 125v123 turn 61, 125v126 turn 41, 126v123
turn 21), every one cycling `dropping stale async answer (casting arm; the question (or turn/phase)
moved) [outcome: re-asked]` six times, then `stale_livelock` -> heuristic, per decision.
Stopped the unit at 09:20 (owner: "this timespan is egregious. check whats happening.").

| | wave-60 corpus (171140) | wave-61 corpus (220227, partial) |
|---|---|---|
| stale-drop stderr lines | 64 | 15,900 |
| decision records (ex gamestart/end/recovery) | 2,013 | 3,586 |
| `stale_livelock` refusals | 0 | 2,644 (73.7% of decisions) |

The corpus therefore measured the heuristic, not the model: it is INVALID for step three
(Invariant 00) and a rerun is owed.

**Mechanism (verified in code + a prompt diff).** Lane U's C14 `[hold check: ... (N windows in a
row now) ...]` note is spliced into the priority/cast tail AFTER `askKey` is built (so it is out
of the dedupe key, as the lane's comment and PARSETEST claim) but BEFORE `assemblePrompt(userTail)`,
which sets `mPromptTail` = the seam half of the ASYNC SLOT KEY. `holdReopenNote` ran on every
per-tick prompt rebuild while an answer was in flight and did `run++` each time, so the in-flight
slot key and the rebuilt key always differed by one — every answer stale, six round trips burned,
then the livelock breaker handed the decision to Baka. Prompt diff 125v123 seat-123 seq 856 vs 858
(both Cleanup, turn 61): the only per-rebuild delta was `(125 windows in a row now)` ->
`(132 windows in a row now)`. The lane's PARSETEST checked the note's bracket shape, not the slot
key — a green test on the component next to the defect.

**Fix (this commit).** (1) `holdReopenNote` measures once per WINDOW: same translog seq + no new
row = the same window rebuilt, returns the cached note (`holdNoteSameWindow`, pure, 4 PARSETEST
cases). (2) Both seams pass the note-free `tailStr` as `keyTail` to a new 3-arg `assemblePrompt`,
so prompt-only notes (declined count, hold check) can never move the slot key. Gate: make -B, suite
1249 (2 known: lifeline, merrow_reejerey), 65 AI/0, PARSETEST 3915/0.

**Not verified.** No suite fixture can exercise the async slot (no network seat); the rerun corpus
is the instrument. Watch the stale-drop count in the first 15 minutes of the rerun.

**Process note.** An 11-hour silent failure: the watch only fired on the done-file or unit death.
A corpus watch should also emit the stale-drop count periodically so a drop storm is visible in
minutes. Applied to the rerun's Monitor.
