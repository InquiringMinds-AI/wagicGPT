# Wave-58 engine findings (orchestrator, as they surface)

## F1 — HIGH — SIGABRT in `DecisionManager::buildMenuChoice`: a menu item id indexes past `ActionLayer::mObjects`
Observed 2026-09-04 00:28 in the log-window A/B arm `4`, game deck146 vs deck152
(`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-235619/game-146v152-1788499448.stderr`, binary =
`archives/wagic-027db3b09-w57V`, i.e. wave-57 lane V; the code is unchanged on master a3f0700d3).
Core: `coredumpctl` PID 2321273 (SIGABRT), extracted to the session scratchpad as `ab4-core`.
Backtrace: `std::vector<JGuiObject*>::operator[]` (n=175) assertion <- `DecisionManager::buildMenuChoice`
(src/DecisionContract.cpp:367) <- `AIPlayerGPT::computeActions` (AIPlayerGPT.cpp:24317) <- `AIPlayerBaka::Act`.
Site: `MTGAbility * ab = (MTGAbility *) object->mObjects[object->abilitiesMenu->mObjects[k]->GetId()];`
— `GetId()` is an index into `ActionLayer::mObjects`, read with no bounds check; here 175 >= size().
Context in stderr: Lost Mine of Phandelver venture (DUNGEON ROOM CHOICE) resolved twice, then a
StackAbility from Lost Mine of Phandelver was added, then a TARGET CHOICE for Nadaar, Selfless Paladin's
"put a card into the graveyard" ability, then the abort. Hypothesis (orchestrator, unverified): the
`abilitiesMenu` was built while the action layer held more abilities (the venture/room abilities) and
those were removed before the AI's next Act consumed the menu, so the menu's ids are stale; the human
path (`ActionLayer::ButtonPressed`) may guard the same index or may have the same hole.
Wave-56/57: 0 SEGV in 42 games; this is a NEW abort class, 1 of 63 A/B games. Rare path (dungeons).
Fix shape: bounds-check the id against `object->mObjects.size()` (and null) at EVERY site that maps a
menu id to an ability — a stale menu must make `buildMenuChoice` return false (the engine re-asks
next tick), never index; plus a fixture that reproduces the stale-menu shape (Nadaar + Lost Mine, or a
synthetic test card whose activated ability removes a sibling ability while its menu is open).

## F2 — HIGH — engine HANG (no tick for 3640 s) after an AI menu answer of -1 on Sigarda, Champion of Light
Wave-58 corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-015059/`, game deck152 vs deck126
(`game-152v126-1788504663.stderr`; seat translogs `1788504666-ai_baka_deck152-*.jsonl` (31 records) and
`1788504666-ai_baka_deck126-*.jsonl` (13 records)); binary `archives/wagic-a3f0700d3-w58step1b` (= master).
The harness's hang watchdog stopped the game (no seat translog record for 3640 s, limit 3600); the
process then shut down normally (full resource teardown in stderr), so it was alive and not ticking
the game — a dead loop or a parked decision, not a crash. Last activity in stderr (line ~957 after
filtering resource noise):
  Action added to stack: StackAbility. (Source: Sigarda, Champion of Light)
  AIPlayerGPT: Choose an option for Sigarda, Champion of Light: -> chose -1 of 2
  ActionLayer::doReactTo 178
  Action added to stack: StackAbility. (Source: Sigarda, Champion of Light)
  Resolving Action on stack: StackAbility. (Source: Sigarda, Champion of Light)
  Resolving Action on stack: StackAbility. (Source: Sigarda, Champion of Light)
  [nothing further until "Ending GameStateDuel"]
Last translog record of the deck152 seat (Sigarda's controller is deck152 — verify): seq 30, turn 11,
phase Blockers, `fallback: empty_reply`, chosen_text `<refused: empty_reply>` (heuristic answered).
The Sigarda menu itself has NO translog record — the -1 came from a path that did not ask the model
(or the record was never written). 58 `only display-toggle` lines in the game (all by line 819, well
before the hang — not the wave-56 livelock signature). Hypothesis (orchestrator, unverified): the
"Choose an option" may-menu (Sigarda's Coven attack trigger: look at the top five, may reveal a Human)
got -1 (= cancel/no choice) from `chooseMenuAction` after the empty-reply fallback state, the
trigger's follow-up chooser was left owned by the AI seat with no consumer, and the phase could not
advance — the same class as the owner's vpk16 softlock ("screen frozen, turn indicator opponent")
that lane F (wave 57) added the AI-owned orphan-chooser breaker for in `userRequestNextGamePhase`;
either the breaker does not cover this shape or the loop is elsewhere (e.g. inside the double
resolution of the same StackAbility). Tools: lane S's `aiseat` fixture DSL + `WAGIC_STALLPROBE`,
lane T's `WAGIC_HANG_GUARD` (default OFF — it was off in this corpus, so no dump exists).
F2 addendum (deck152 seat review, 2026-09-04): the three decisions before the hang (seq 26/28/30 of
the deck152 seat) were ALL `empty_reply` heuristic fallbacks, so the `-1 of 2` on the Sigarda menu
almost certainly came from the BAKA FALLBACK path (AIPlayerBaka's menu choice), not the model —
which also explains the missing translog record. Repro on the forced-fallback path
(WAGIC_GPT_NOTHREAD / a dead endpoint), and stamp fallback answers at every seam.

## F3 — HIGH — `empty_reply` is a 2.5 s CONNECT timeout: 89 of 2,270 decisions (3.9%) silently heuristic-played, in runs up to 10
Every seat review and the engine seat converged on it: all 89 wave-58 `empty_reply` fallbacks have
`reply == ""` and latency 2502-2558 ms (answered median ~12.7 s), in runs (deck152 vs deck162 seq
32-50: ten consecutive; deck126 seq 36-50: eight), with no HTTP status / curl error in stderr. The
hang F2's precondition is such a run (seq 26/28/30 then the Sigarda menu's -1 at seq 30).
SOURCE (orchestrator, verified by grep): `src/GptConfig.cpp:822`
`curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 2500L);` (the sibling at :926 uses 5000). Under
21 concurrent games a TCP connect to Spark's vLLM can exceed 2.5 s; curl returns a connect timeout
with an empty body; `noAnswerClassFor` files a fast empty body as `empty_reply` (transport) and the
seat falls back to the heuristic — no retry, no receipt (the curl result code is never logged).
Fix shape: (a) the connect timeout is not a decision deadline — raise it to the request deadline
or at least tens of seconds (owner doctrine: generous defaults, tighten on evidence); (b) log the
curl result code + HTTP status on EVERY non-200/empty outcome (translog field, not only stderr);
(c) a transport failure inside the decision deadline is RETRIED (bounded by the deadline), never
handed to the heuristic on the first miss; (d) PARSETEST for the classification. Prediction:
`empty_reply` at ~2.5 s = 0 of N; fallback rate falls from 108/2270 toward the parse-failure floor.
