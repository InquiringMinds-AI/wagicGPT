# Audit 2026-09-02 — slice 3 (gpt-c): AIPlayerGPT async worker / transport / translog / narration cap / PARSETEST, plus GptConfig.cpp and OptionGpt.cpp

Master 859186536. Read-only on master; measurements in worktree `audit-gpt-c` (removed at the end).

## Scope note (line-range vs topic)
The brief assigned `AIPlayerGPT.cpp` 24000-37143 and named the async worker, transport, request
builder, translog writers, narration cap and PLAN handling as living there. They do not: lines
24000-24916 are the reveal / mulligan / bottom / cleanup-discard seams, and 24917-37134 is
`runParseSelfTest` (12,218 lines, 2,245 CHECKs). The named machinery lives at 8146-9130
(AsyncState, WorkerMain, pollCompletion, pollCompletionRetry), 9132-9254 (ctor, config, curl
init), 9286-9713 (translog record writers), 9715-9758 (endpoint), 10081-10247 (narration log +
trim), 10690-10718 (PLAN carry), 14480-14640 (request body). I audited the TOPICS wherever they
live, plus GptConfig.cpp and OptionGpt.cpp in full. Slices gpt-a/gpt-b may overlap the 8-15k
region; the findings below are anchored to exact lines so duplicates are easy to fold.

## What I read / did not read
Read in full: AIPlayerGPT.cpp 8100-9130, 9132-9770, 10040-10125, 10200-10260, 10690-10730,
14480-14600, 18830-18875, 22540-22600, 24000-25120, 37040-37143; GptConfig.cpp (all 1854 lines);
OptionGpt.cpp (all 485); GptConfig.h; the Vita CMakeLists (read-only, for build defines).
Sampled, not traced: runParseSelfTest 25120-37040 (grep-scanned for heap/static/env/file
state - it allocates nothing on the heap, touches no globals, opens no files; it is pure
CHECK-on-static-parsers). Not read: the prompt renderers outside the located functions
(1-8100, 9770-24000), GameStateOptions.cpp's model-fetch worker, JGE SDLmain/Vitamain beyond
the exit paths, the Android Java side of the JNI transport.

## Measurements
* `nm -S objs-sdl/AIPlayerGPT.o`: `AIPlayerGPT::runParseSelfTest()` = 0x102535 = **1,058,101 bytes
  of .text** (x86-64), plus **339,004 bytes of string literals** inside the function (awk over the
  source range). AIPlayerGPT.o .text total = 2,840,904 bytes -> the self-test is ~37% of the TU's
  code and ~49% with its rodata.
* Compile of AIPlayerGPT.cpp (worktree, `make -f Makefile.sdl objs-sdl/AIPlayerGPT.o`, other
  agents' builds running concurrently): **baseline 152.2 s wall / 44.5 s user**; with
  runParseSelfTest stubbed: see the line at the end of this section (filled after the run).
* Translog corpus at ~/.Wagic/ai/gpt/logs (1,887 seat files, 1.3 GB), 40 largest seat logs:
  records/game median 313, max 2,622; **sum of `events` bytes per seat-game median 28,956, max
  191,588** (this is exactly what `mNarrationPending` grows to when the translog is OFF - finding
  1); sum of `prompt` bytes written per seat-game median 6.5 MB, max 74 MB; largest single
  prompt 233,662 bytes (seq 1570 of 1787786558-deck123-vs-deck126, a 1,525-attacker roster,
  178 s latency). Recent files (epoch >= 1788300000, i.e. post-#W48 A-row collapse) contain
  **no prompt over 60 KB** - the roster blow-up is already fixed by the W48 collapse; not a finding.

* Stubbed compile result: **58.9 s wall / 22.3 s user** (vs 152.2 / 44.5 baseline) - the self-test
  is half the TU's compile time; `size` .text drops from 2,840,904 to **1,392,364 bytes**
  (-1,448,540, 51% of the TU's text+rodata), the .o from 11.7 MB to 8.2 MB.

## Findings (ranked)

### 1. MED — MEM-EFF/LEAK-per-game: `mNarrationPending` is never consumed when the translog is off  (VERIFIED by trace + corpus measurement)
`src/AIPlayerGPT.cpp:9483-9487` (`writeTransLog` early return), fed at `:10246` (`writeNarration`
-> `narrationAppend(..., &mNarrationPending)`), `:9882`, `:10845`; member `include/AIPlayerGPT.h:665`.
Mechanism: the per-record narration delta is appended on EVERY narration line, unconditionally,
but the only two consumers (`writeTransLog` `:9579-9583`, `flushRecoveryRecord` `:9463`) sit behind
`if (mTransLogPath.empty()) return;`. With `translog`/`telemetry` unset (the shipped default and
the Vita's normal state) the string grows for the whole game and is freed only with the player.
The 24 KB trim at `:10103` deliberately does not touch it (W42-D8), so nothing bounds it.
Evidence: sum(`events`) per seat-game in the corpus = median 29 KB, max 192 KB (above) - that is
the resident size the buffer reaches, and `+=` growth means transient 2x during reallocation.
Small on desktop; on the Vita's ~40 MB in-game heap it is an unbounded per-game buffer holding
data nobody will read. Fix: in the early return at `:9483` add `mNarrationPending.clear();`
(mirrors the `mLastParseNote.clear()` already there), and guard the three feed sites on
`!mTransLogPath.empty()` so the copy is never made (`writeNarration` passes `NULL` as the delta
pointer when the log is off). Blast radius: none when the translog is on (records byte-identical);
`flushRecoveryRecord` is already path-gated. Verify: suite + PARSETEST unchanged; a headless
`WAGIC_GPT_TRANSLOG=0` game with a temporary size print (or ASAN heap profile) shows the member
flat at 0.

### 2. MED — CRUD/MEM-EFF: `runParseSelfTest` (12.2k lines, ~1.06 MB code + 339 KB strings) compiles into every target, including the three where it can never run  (VERIFIED by nm/size; compile-time measured)
`src/AIPlayerGPT.cpp:24917-37134`, entry `:37138`; the only caller is `JGE/src/SDLmain.cpp:1027`
(env `WAGIC_GPT_PARSETEST`). Vitamain.cpp, the PSP main and Android never call it, yet the Vita
CMakeLists compiles AIPlayerGPT.cpp with no gate, so the Vita/PSP/Android binaries carry ~1.4 MB
(x86-64 numbers; ARM Thumb code is a bit denser, the 339 KB of literals is identical) of dead
text+rodata, resident for the whole session on the memory-constrained targets. It also costs
every desktop rebuild of this TU (numbers above). Owner rule: diagnostics compile out of release;
this is the largest single diagnostic in the tree. Fix: wrap `:24905-37134` (the static helper
`answerSegmentStatic` included) in `#if defined(WAGIC_GPT_PARSETEST_BUILD)` with an `#else` stub
`void AIPlayerGPT::runParseSelfTest() { std::cout << "PARSETEST not built into this binary\n"; }`,
and add `unix:DEFINES += WAGIC_GPT_PARSETEST_BUILD` next to `WITH_GPT_AI` in `wagic-SDL.pro:20`
(so Makefile.sdl keeps the gate). Simplest zero-config alternative: `#if !defined(VITA) &&
!defined(PSP) && !defined(ANDROID)`. Blast radius: the desktop PARSETEST gate (must still report
2249/0), the psp worktree's CMake (no change needed for the exclusion variant). Verify:
`WAGIC_GPT_PARSETEST=1 ./wagic` -> 2249/0 on desktop; `nm -S` on the Vita ELF shows the symbol
gone. Longer-term the 339 KB of expected-output literals argue for the test moving to its own TU
(`AIPlayerGPTSelfTest.cpp`) so the game TU rebuilds fast; the static parsers it reaches would
need a friend/header seam - a bigger change, note only.

### 3. MED — "does it do the job": the generic HTTP path swallows the status code, so a 4xx/5xx is indistinguishable from an unreachable server and logs nothing  (VERIFIED by reading)
`src/GptConfig.cpp:644-654` (`httpRequestImpl`: `if (res != CURLE_OK || httpCode != 200) return "";`),
consumed at `src/AIPlayerGPT.cpp:8245` -> `:8286` (`timedOut` = empty + at the wall) -> `:8510`
(`empty_reply`). Mechanism: a wrong API key (401), a model id the server rejects (404), a prompt
over the server's context (400/413 - vLLM's "maximum context length" error), a 429 or a 5xx all
return the same empty body the code treats as TRANSPORT failure; nothing writes the code anywhere
(the JNI path logs `gptLastError`; the Codex path logs `HTTP <code>: <body>` once per distinct
cause at `:8232-8240`; the libcurl path has no log line at all). #W53-Q (D10) just spent a wave
separating "timeout" from "empty_reply" because a seat review could not tell them apart - this is
the same blindness one layer down, and a whole match can play on the heuristic with
`fallback: empty_reply` on every record. Fix: return the code out of `httpRequestImpl` (an
optional `long * codeOut`, as `httpRequestFull` already does) and in `WorkerMain` log once per
distinct (url, code, first 160 body bytes) under the same `logMtx` pattern the Codex branch uses;
stamp the code onto AsyncState so the record can carry `http_status` and `noAnswerClassFor` can
name `http_error` as its own class. Blast radius: `gptHttpGet/Post` callers (probe, model list,
worker), translog schema (additive field), PARSETEST's `noAnswerClassFor` table (add rows).
Verify: point `WAGIC_GPT_URL` at a server with a bad key -> gpt-log.txt names the 401 and the
record says `http_error`, not `empty_reply`.

### 4. LOW/MED — MEM-SAFE (threads): detached workers vs process teardown; `curl_global_init` per player with no cleanup  (SUSPECTED)
`src/GptConfig.cpp:1383-1398` (`std::thread(fn, ctx).detach()`), `src/AIPlayerGPT.cpp:9161-9163`
(`curl_global_init` in every AIPlayerGPT ctor; no `curl_global_cleanup` anywhere - grep count 1),
worker at `:8209-8261`. The lifetime design is sound for the PLAYER (AsyncState is a shared_ptr
held by the ctx; the worker touches only ctx/state; `:18848` and `:8170` say so and I found no
`this` capture). Two residual hazards: (a) at process exit a worker still inside
`curl_easy_perform` runs against OpenSSL's `atexit` cleanup and this TU's function-local statics
(`codexAuth()` json doc `:899`, `codexUsedPercentRef()` `:1078`, `logMtx`/`lastErr` `:8232-8233`) as
they are destroyed - a corpus game that ends with the losing seat's ask in flight (blockers on the
lethal swing is the common shape) can die with SIGSEGV after `main` returns, which a harness
keyed on exit status records as a crash; (b) `curl_global_init` is documented not thread-safe
and runs on the game thread at every duel start, i.e. possibly while the previous duel's detached
worker is still inside libcurl; the probes in OptionGpt.cpp/GameStateOptions.cpp call
`curl_easy_init` on worker threads with no global init at all if no AIPlayerGPT was ever
constructed (two concurrent probes -> concurrent implicit global init). Fix: one `gptCurlInit()`
with a function-local `static bool once` called from the game thread (ctor + both probe sites);
an atomic in-flight counter incremented before spawn / decremented at the end of `WorkerMain`,
and a bounded wait (`<= 2 s`, poll 50 ms) at the game-exit path before `main` returns, then
`curl_global_cleanup`. Blast radius: startup/exit only. Verify: run a headless game with
`WAGIC_GPT_TIMEOUT=600` against a deliberately slow endpoint and kill the game at a decision -
exit status 0 with the guard, intermittently 139 without (needs repetition: rare event).

### 5. LOW — Vita-specific: a function-local static is first-touched ON THE WORKER THREAD, and the Vita's `__cxa_guard` has no lock  (SUSPECTED)
`src/AIPlayerGPT.cpp:8232-8233` (`static GptMutex logMtx; static string lastErr;` inside
`WorkerMain`'s Codex-failure branch). The Vita CMakeLists (`worktrees/psp/CMakeLists.txt:308-322`)
records that gthreads is inactive there and that a live `__cxa_guard_acquire` crashed on
hardware; with gthreads inactive the guard is a plain flag, so two workers (AI-vs-AI on the
subscription preset, both failing at once - exactly the case the comment above the lines
describes) can double-construct the mutex. Everything else that is static in the GPT TUs
(`codexMutex()`, `codexAuth()`, `codexUsedPercentRef()`) is first-touched on the game thread via
`resolveEndpoint` -> `gptProbeEndpoint`, which is why they are safe. Fix: hoist the two to
namespace scope (constant-initialised at load), no other change. Verify: PARSETEST/suite unchanged.

### 6. LOW — PERF/IO on the handhelds: every translog record and every gpt-log line opens and closes the file  (VERIFIED by reading; the code's own comment names the cost)
`src/AIPlayerGPT.cpp:9314, :9465, :9658, :9710` (four `std::ofstream(mTransLogPath, app)` per
game, one per record kind) and `src/GptConfig.cpp:256` (`gptLogLine`). `:8788-8792` explains
that a per-event open/write/close on the game thread is "a known cause of lag" on the Vita and
made the refusal log fire once for that reason - but every decision record still pays an open
(and `rec.dump()` of a prompt that is median 29 KB, max 234 KB, so the write is not small).
Fix: a lazily-opened `std::ofstream mTransLog` member, opened in `ensureGameStartRecord` after
the `-vs-` rename (`:9296-9301` guarantees the rename precedes the first append), flushed per
record, closed in `logGameEnd`. Blast radius: the three writers; harvest tooling reads the same
JSONL. Verify: identical records, one `open` in `strace -e openat` per game instead of N.

### 7. LOW — narration cap arithmetic: the trim re-fires on every line once the trim marker passes ~3.9 KB, and the marker can be blank  (SUSPECTED)
`src/AIPlayerGPT.cpp:10103-10108` (trim when `size > 24000`, keeps the last ~20,000 bytes behind
`trimMarker`), `:10241-10245` (marker built only when `mNarration.size() + line.size() > 24000`),
`:5730-5747` (`trimMarkerLine`), `:10044-10070` (`zoneNameDigest`, distinct names only). The cap
DOES bound the log: after a trim the buffer is `marker + <= 20,000`, and every appended line ends
in `\n` so `find` never fails. Two edges: (a) when the four-zone digest exceeds ~3,900 bytes
(200+ distinct card names across both graveyards and exiles - mill/dredge games reach it) every
subsequent `writeNarration` exceeds 24,000 again and re-trims: four zone walks (each O(cards x
distinct)) plus a ~24 KB rebuild per event line, and the retained history shrinks to
`20,000 - marker`; (b) the caller's precondition omits the pending-phase line and the `- `/`\n`
decoration, so an append that crosses the cap only because of them trims with an EMPTY marker -
the log gets a bare blank line and the zone digest that trim was meant to preserve is lost.
Fix: make `narrationAppend` cut to `20000 - trimMarker.size()` bytes of tail (so the post-trim size
is always <= ~20 KB and the next trim is ~4 KB of lines away), and have `writeNarration` build the
marker when `mNarration.size() + line.size() + pendingPhase.size() + 6 > 24000`. Blast radius: the
PARSETEST cases that pin `narrationAppend`/`trimMarkerLine` (search "trimMarkerLine" at
`:28293-28588`). Verify: PARSETEST; a synthetic 300-distinct-name graveyard game shows one trim
per ~4 KB instead of one per line.

### 8. LOW — secrets at rest: the key-obfuscation salt is `rand()` on any platform without /dev/urandom, and three secret files are created 0644 before the chmod  (SUSPECTED)
`src/GptConfig.cpp:163-176` (`/dev/urandom`, else `rand() & 0xFF` x32 - seeded only by
`GameApp.cpp:103`'s `srand(time(0))`), `:185-190` (keysalt written by `ofstream` then `chmod 0600`),
`:1019-1024` (`codexSave`), `:1653-1661` (sign-in writes `oai-auth.json` the same way). The
comment at `:43-50` promises "the config file alone is not decodable"; on the Vita (no
/dev/urandom in vitasdk newlib) and native Windows the salt is a second-resolution-time-seeded
`rand()` stream, so a copied `endpoints.txt` plus the install epoch is a small brute force. The
chmod-after-create window is one process-scheduling gap on a single-user console; cheap to
close. Fix: on VITA use `sceKernelGetRandomNumber`, on `_WIN32` `rand_s`/`BCryptGenRandom`; write
secret files through `open(path, O_WRONLY|O_CREAT|O_TRUNC, 0600)` + `fdopen`, or set `umask(077)`
around the three writes. Blast radius: first-run salt creation only (existing salts unchanged).
Verify: `stat -c %a` on a fresh keysalt is 600 from creation; a Vita salt differs between two
fresh installs.

### 9. LOW — transport details on the libcurl path  (SUSPECTED)
`src/GptConfig.cpp:625-642` and `:701-716`: `CURLOPT_FOLLOWLOCATION 1` on a POST with no
`CURLOPT_POSTREDIR` turns a 301/302/303 into a GET with no body (so any redirecting endpoint
silently yields the empty-body class of finding 3), and no `CURLOPT_MAXREDIRS` (libcurl default
is unlimited) leaves a redirect loop bounded only by the 600 s timeout. The bearer rides
`CURLOPT_HTTPHEADER`; libcurl >= 7.58 strips a custom Authorization header on a cross-host
redirect (CVE-2018-1000007) and >= 7.83 on a protocol/port change, so on Arch's 8.x and
vitasdk's current curl the key does not leak - but it is a version-dependent guarantee. A fresh
`curl_easy_init`/TLS handshake per request is also a full handshake per decision (no keep-alive)
- a few hundred ms on a 30 s decision, noted not ranked. Fix: `CURLOPT_MAXREDIRS 3`,
`CURLOPT_POSTREDIR CURL_REDIR_POST_ALL` (or drop FOLLOWLOCATION on the POST), and prefer
`CURLOPT_XOAUTH2_BEARER` + `CURLOPT_HTTPAUTH CURLAUTH_BEARER`, which libcurl scopes to the
origin host by construction. Verify: a local redirecting stub (302 to the real path) now reaches
the model; `curl --version` no longer matters.

### 10. LOW — OptionGpt: a row binds `&cfg.urls[0]` while `cfg` is copy-assigned on Reload  (SUSPECTED)
`src/OptionGpt.cpp:49` (`OptionGptText(&cfg.urls[0], ...)`), `:102` (`cfg = fresh;`). `std::vector`
copy-assignment reuses storage only when the new size fits the old capacity; a config that gained
`url=` lines between construction and `Reload()` (hand edit while the game runs, or a save from a
second seat) reallocates and the Endpoint row's `mBind` dangles into freed memory (`Render` reads
it every frame, `updateValue` hands it to the keypad to write). Narrow trigger, real UAF when it
hits. Fix: bind the row to `(GptSettings*, index)` and resolve `urls[i]` at use, or
`cfg.urls.reserve(8)` after both `load()`s. Verify: ASAN build, edit endpoints.txt to add a second
`url=` while on the options screen, back out and re-enter.

## Checked and found sound (so nobody re-derives them)
* Worker/game-thread sharing: `WorkerMain` touches only `ctx` (owned, deleted on the worker) and
  `ctx->state` under `GptMutex`; the game thread reads/writes AsyncState only under the same
  mutex (`:8470, :8521, :8735, :8778`); prompt/response/`started`/`timedOut` are all covered.
  Player destruction mid-flight is safe (shared_ptr). The spawn-refusal path deletes the ctx and
  resets the slot (`:8757-8802`). The `WAGIC_GPT_NOTHREAD` seam makes that path testable.
* Timeout/retry (#W53-Q D10): `kTimeoutRetryTag` is stripped in `buildRequestBody` (`:14490-14493`)
  and excluded from the tight retry cap (`:14570`); the retry keys its own slot so a late first
  answer cannot be consumed as the retry's; `mRetryDoneBase` bounds it to one per decision. A
  timeout retry that comes back reasoning-only never gets a forced close (early return at
  `:9032`) - by design, one retry per decision.
* Reply JSON handling (`:8552-8598`): all `operator[]` on a non-const json (no throw on missing
  keys), and the only throwing shapes (array/scalar root) are caught.
* Secrets in logs: no code path prints `mApiKey`/`ctx->key`/the bearer; GPTASYNCLOG prints url
  and byte counts only and is `_DEBUG || WAGIC_DEVLOGS`-gated (`:8175-8195`, correct gate);
  stderr prints endpoint+model only (`:9253`); translog records carry prompt/reply, never the key;
  OptionGpt masks the key to its last 4 chars (`OptionGpt.cpp:191-200`).
* `GptSettings::load()` (disk read + salt read) is called once per AI-player construction
  (`AIPlayer.cpp:270`), not per tick; `gptPeekOpponentHand` caches.
* The 24 KB narration cap bounds the LOG; the prompt is not otherwise capped, but the recent
  corpus shows no prompt over 60 KB since the #W48 A-row collapse (largest pre-collapse prompt:
  233 KB / 1,525 attacker rows). PLAN carry is bounded at 1,600 chars (`:10694-10700`).
* runParseSelfTest itself: no heap allocation, no globals mutated, no files; safe to leave
  runtime-gated on desktop - its cost is the binary/compile weight of finding 2 only.
* JNI transport: attach/detach balanced, every local ref released, pending exceptions cleared
  (`GptConfig.cpp:508-596, 743-828`).
* Codex auth: every token mutation is under `codexMutex()`, refresh-token rotation is persisted
  before use, a concurrent seat's refresh is detected by comparing the access token (`:1257-1270`).

## Worktree
`worktrees/lanes/audit-gpt-c` (branch `audit-gpt-c`) was used only for the two compile timings;
removed after the report was written. No edits to master; scratch under ~/.gatelogs/audit-gpt-c-*.
