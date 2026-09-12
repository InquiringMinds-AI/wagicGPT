# wave 81 lane DJ - the harness reads the SHARED log directory by time

**Defect.** `$HOME/.Wagic/ai/gpt/logs` is shared by every pool, probe, suite
fixture and concurrently-running lane. Wave 76 learned that for the HARVEST and
wave 77 CU F10 fixed it there (each game announces its two seat-log basenames on
its own stderr, `WAGIC_GPT_TRANSLOG_FILE <base>`, recorded in
`$OUTDIR/.seatlogs`). Every OTHER check kept selecting seat logs out of the
shared directory by recency, so on 2026-09-12 a valid single-game rerun
(`matchups-20260912-074153-rerun-01-162v123`) PASSED the regime gate on its own
two seats - "10 records, all carry reasoning" - and was then killed by the next
sweep, which read 20 records of ANOTHER lane's suite fixture
(`1789239545-ai_baka_deck198-...-vs-ai_baka_deck199.jsonl`, GPT stub endpoint,
thinking off; 214 such deck198/deck199 logs are in that directory now). Nothing
was wrong with the corpus. A foreign log is not evidence about this run in
either direction: it can FAIL a valid run, and it can equally MASK this run's
own wedge.

## The scan sites (all on base commit cf72a002f)

| site | file:function | what it keyed on |
|---|---|---|
| regime gate (first + every later sweep) | `tools/regime-gate.py:run_records` (L~55) | `glob("$LOGDIR/*.jsonl")`, keep `epoch >= start-2` |
| pilot-stall tripwire | `tools/selfplay-harness.sh:pilot_stall_verdict` (L~325) | same glob, `epoch >= start-5` |
| window-loop tripwire | `selfplay-harness.sh:window_loop_verdict` (L~500) | same glob, `epoch >= start-5` |
| hung-game / no-progress watchdog | `selfplay-harness.sh:no_progress_sweep` (NPY heredoc, L~1190) | `glob("*-ai_baka_deck<d>-*.jsonl")`, `gstart-2 <= epoch <= gstart+300` |
| cap adjudicator (after each game) | `selfplay-harness.sh:run_one_game` (PYEOF heredoc, L~1090) | same deck+300 s window |
| feasibility latency projection | `selfplay-harness.sh:supervisor` (WPY heredoc, L~1320) | same glob, `epoch >= start-5` |
| fallback census in the results banner | the trailing `PY` block | ALREADY manifest-keyed: it reads `$OUTDIR/*.jsonl`, i.e. the harvest's output. Left alone. |

## RED (on base)

1. Fixture (the incident's shape): own seat log thinking=on + a foreign
   deck198v199 thinking=off log in the same directory; the run announced only
   its own on `game-162v123-1789238538.stderr`.
   `run_records` scanned **2** files and `gate` returned
   `FAIL the binary played 5 of 10 checked records with thinking=off ... (first:
   1789239545-ai_baka_deck198-...)` - character-for-character the kill in
   `~/.gatelogs/selfplay-harness-REGIME-FAIL`.
2. Against the REAL incident data (`--logdir ~/.Wagic/ai/gpt/logs --start
   1789238538 --regime on`), base printed
   `FAIL the binary played 40 of 50 checked records with thinking=off ...`.
3. Pilot stall, the MASKING direction: our announced seat timing out on every
   decision while a foreign seat is answered normally -> base `OK 2` (the wedge
   is invisible); window loop: a foreign seat at 295 declines -> base
   `LOOP 295 1789239545-ai_baka_deck198-...` (our healthy run is stopped).

## The fix

New `projects/mtg/tools/runmanifest.py` - the single answer to "is this seat log
mine": `own_seatlogs(outdir[, game])` unions `$OUTDIR/.seatlogs` (written as each
game is reaped) with the live `WAGIC_GPT_TRANSLOG_FILE` announcements on the
run's own `game-*.stderr` (present from a game's first decision, so in-flight
games are covered), and `own_logs(logdir, own)` selects by exact basename.
Announcement scanning is capped at 8 MB of stderr per game (the announcements
sit 5-7 KB in; a spinning game's stderr can reach 1.23 GB).

Every site above now takes the run's `$OUTDIR` (per-GAME stderr for the two
per-game sites, so concurrent games of the same pair are separable too) and
reads only announced names. A check that runs before any announcement sees an
EMPTY set and therefore no records, which is the pre-existing WAIT / "no seat
log yet" branch - it waits for the announcement instead of scanning the
directory. The regime gate's existing 30-minute no-records deadline is the loud
backstop.

**The one degrade, deliberate and inherited from CU F10:** an outdir with
neither `game-*.stderr` nor `.seatlogs` has no announcement channel at all (a
probe dir, a selftest fixture, an old corpus layout); `own_seatlogs` returns
`None` = "do not filter" and the old epoch rule stands. A live run always has
its per-game stderr, created before the game starts, so it cannot take that
branch.

## GREEN

- `tools/selfplay-harness.sh --selftest`: harvest **16** (was 14),
  pilot-stall **21** (was 19), window-loop **14** (was 12),
  regime-gate **34** (was 30) - 0 failed in all four.
- New pins, each RED on base: foreign thinking-off log present during the run ->
  gate PASSes on our own logs and never names the foreign one; foreign
  all-timeout / foreign dead-window -> tripwires stay OK; foreign healthy seat ->
  our own wedge still STALLs; foreign log newer than `$BEFORE_LIST` -> not
  harvested. MUST-NOT-MATCH pins in every one: our OWN thinking-off seat still
  FAILs the gate, our OWN wedged seat still STALLs, our OWN dead window still
  LOOPs, and a run that has announced nothing WAITs rather than reading the dir.
- `bash -n tools/selfplay-harness.sh` OK; `python3 -m py_compile` OK on
  `regime-gate.py` and `runmanifest.py`.
- End to end on the real incident data:
  `regime-gate.py --logdir ~/.Wagic/ai/gpt/logs --start 1789238538 --regime on
  --outdir <the rerun outdir>` -> `PASS thinking=on: 10 records, all carry
  reasoning. seats: 2 shapes: two_line=10`, the same verdict the run's own first
  sweep reached before the foreign log killed it.

## Weakest evidence

1. **No live corpus was run** (lane is tools-only, no corpus by standing rule).
   Everything above is selftests plus a replay of the incident's own on-disk
   data. The path that is exercised only by a real run is the LIVE announcement
   read - `.seatlogs` is empty until a game is reaped, so mid-run identity rests
   entirely on grepping each `game-*.stderr`. That read is verified against the
   real `game-162v123-1789238538.stderr` (announcements at bytes 5,076 and
   6,710) but never against a game that is still writing.
2. **The 8 MB stderr cap is a judgment, not a measurement.** If some future
   engine path printed megabytes before its first decision, a seat could go
   unannounced and its checks would silently see fewer seats. Nothing detects
   that today except the 30-minute gate deadline.
3. **The `own is None` degrade is a silent revert by construction.** It is
   pinned as a deliberate behaviour, so a bug that made a live outdir look
   manifest-less (e.g. a future change that stops creating the per-game stderr
   before launch) would restore the wave-76 defect with no error. The degrade
   warns on stderr in the harvest but not in the checks.
4. The census/latency/adjudicator sites are argued, not observed: no fixture
   demonstrates a foreign log moving the latency median or the adjudicated life
   totals; they were fixed because they read the same directory the same way.
