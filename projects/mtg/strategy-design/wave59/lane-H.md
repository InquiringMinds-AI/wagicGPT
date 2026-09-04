# Wave-59 Lane H — K1 (F3): the 2.5 s connect timeout that filed transport failures as `empty_reply`

Base: `35c9b85bf`. Branch `w59-lane-H`.

## The defect

`GptConfig.cpp:822` set `CURLOPT_CONNECTTIMEOUT_MS` to a hardcoded `2500L` while the request's
own deadline (`CURLOPT_TIMEOUT_MS`) was the decision deadline (tens of seconds to minutes).
Under 21 concurrent wave-58 games a TCP connect to Spark's vLLM regularly exceeded 2.5 s, curl
returned a connect-timeout CURLcode with an empty body, and the seat could not tell that from a
model that answered nothing: `noAnswerClassFor` filed it `empty_reply`, the heuristic played the
decision, and no curl code or HTTP status was recorded anywhere. 89 of 2,270 decisions (3.9%),
in runs up to ten consecutive, all with latency 2502-2558 ms. The sibling full-control transport
(`httpRequestFull`, :926) carried the same shape with a 5000 ms cap.

## What shipped

**(a) the connect timeout is the request deadline.** `GptConfig.cpp`: both curl transports now
call `gptConnectTimeoutMs(timeoutMs)` — the request's own deadline, with a named
`kGptConnectTimeoutFloorMs = 30000` floor covering a caller that passes no deadline (libcurl
reads 0 as "the built-in default", so the generous value is stated rather than inherited).
Connection setup is not a decision deadline and has no reason to be the smaller of the two.

**(b) both layers of the outcome are recorded.** A new `gptHttpPost` overload carries a
`long * curlCode` out-parameter down to `httpRequestImpl` (-1 = this platform is not libcurl;
0 = `CURLE_OK`); the JNI and no-transport implementations report -1 truthfully. The async slot
carries `curlResult` beside `httpStatus`, and `pollCompletion` stamps every empty/non-200
attempt into `mLastTransportOutcomes` as `curl=<n>,http=<n>,empty=<0|1>`. The translog record
gains a **`transport`** field: the accumulated stamps joined with `;`, so a decision whose first
attempt failed and whose retry succeeded still carries the failed attempt's cause. Consumed and
cleared with the record (both the logging-on and logging-off paths). The matching stderr line
lives in `WorkerMain` under `#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`.

**(c) a transport failure inside the deadline is retried with what is left of it.**
`pollCompletionRetry`'s single-retry gate gains a second arm beside the existing #W53-Q (D10)
wall-miss arm:
- `retryableTransportFailure(curl, http, empty)` = a nonzero CURLcode, an HTTP 5xx, or an empty
  body with no status at all. A 4xx is a completed client error and an empty HTTP 200 is an
  answered empty body; neither is made legal by buying the same request twice.
- `remainingTransportRetryMs(deadline, firstLatency)` bounds the second attempt to the
  decision deadline's REMAINDER, so first attempt + retry still fit inside one deadline. The
  arm requires a positive remainder, so this is one retry, never a loop.
- Classification: `noAnswerClassFor(..., httpStatus, curlCode)` adds **`transport_error`**.

**(d)** PARSETEST cases for the classification table, the retry predicate, the budget
arithmetic (including the corpus's own 2502/2558 ms numbers under a 120 s deadline), and the
`transport` field's shape plus its JSON round trip.

Files: `src/GptConfig.cpp`, `include/GptConfig.h`, `src/AIPlayerGPT.cpp`,
`include/AIPlayerGPT.h`.

## Gate

Detached unit `w59-H-gate`, `MemoryMax=4G`, `MemorySwapMax=0`.

- PARSETEST: **3469 passed, 0 failed** (baseline 3455 + the 14 cases this lane adds).
- Suite, `WAGIC_TESTSUITE_THREADS=1`: **1245 tests (0 failed), 61 AI tests (0 failed)**;
  `==Test Failed !==` 0, `==Test timed out` 0. Baseline exactly.

## Predictions (falsifiable, for the wave-59 corpus)

1. `empty_reply` records with `latency_ms` in 2400-2700 ms: **0 of N**. The 2.5 s cluster is
   the fingerprint of the removed cap; if it survives, the cap was not the mechanism.
2. Every remaining non-answer record carries either `http_status` or `transport` — no decision
   is filed as a transport fault with no cause written down.
3. Total heuristic handoffs fall from wave-58's 108/2270 toward the parse-failure floor
   (~19/2270 = the non-transport residue), and the surviving transport failures appear under
   `transport_error`, not `empty_reply`.
4. `timeout` records are unchanged in count and meaning — a wall miss still reads `timeout`,
   never `transport_error` (pinned in PARSETEST; see below).

## What I did NOT verify

- **No live probe was run.** Nothing here was exercised against a real endpoint: the raised
  connect timeout, the `transport` field's appearance in a real translog, and the retry
  actually recovering a decision are all unobserved. The corpus is the gate.
- **No suite fixture.** The change is entirely in the HTTP transport and the async retry
  bookkeeping; the suite has no network seat and cannot make a connect timeout happen, so
  there is no fixture that is RED on base. The pure functions (`noAnswerClassFor`,
  `retryableTransportFailure`, `remainingTransportRetryMs`, `transportOutcomeStamp`) are
  pinned in PARSETEST instead, which is where this item's (d) put them.
- **The non-curl transports** (Android JNI, the no-transport PSP stub) compile with the new
  signature and report -1, but neither was built or run here.
- **Long-tail retry interaction.** The transport retry shares `mRetryDoneBase` with the
  force-close and answer-lock retries, so a decision still spends at most one retry of any
  kind; that this is the right allocation when a transport miss is followed by a decode-garbage
  reply is reasoned, not observed.
- The build was incremental on the seeded worktree's `objs-sdl`, not `make -B`.
