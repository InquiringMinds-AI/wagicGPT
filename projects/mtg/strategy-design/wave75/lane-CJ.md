# Wave-75 lane CJ — transport and reply plumbing (P2, P21, P9, P14, P20)

Base: master `207a7f410`. Commit: `a80f9d0ff`. Suite (THREADS=1): **1285 tests, 0 failed;
76 AI tests, 0 failed** — none of the three known flakes fired. PARSETEST: **5788 → 5826,
0 failed** (+38 = exactly this lane's additions; base count measured on the archived
`wagic-76c5fab24-w74step1c`). `git diff | grep -c U+FFFD` = 0.

---

## P2 — the phase-2 forced-close prefill (HIGH)

**Repro.** Wave-74 corpus, 2 of 12 forced closes: `1788968842-…deck123…` seq 64 (ask, t14) and
`1788968865-…deck126…` seq 48 (**attackers**, t20 — three lifelink Vampires + Sanguine Bond
against 9 life). Both `http=400`, empty reply, `<refused: http_error>`, reasoning 20,039 /
19,605 chars. The 400 body was logged nowhere.

**RED on base, live.** The mechanism is visible in the pilot's own chat template
(`spark:~/models/Qwen3.6-35B-A3B-FP8/tokenizer_config.json`): an assistant turn renders as
`'<think>\n' + reasoning|trim + '\n</think>\n\n' + content`, and `reasoning` is derived from
OUR content by `split('</think>')[0].rstrip('\n').split('<think>')[-1].lstrip('\n')` then
`|trim`. vLLM's `continue_final_message` then truncates the rendered prompt at
`rindex(final message content)` and raises if it is not there. The engine sent
`"<think>\n" + prefill + "\n</think>\n\n"` **raw**, so any prefill with leading or trailing
whitespace is rewritten by the template and cannot be found.

Both failing prefills end in whitespace (`…\n   ` and `…**One detail:**\n`); the ten that
succeeded do not. Confirmed with the two probe requests this item is allowed (tiny prompt,
`max_tokens` 60, `--thinking on` shape, one per shape, 2026-09-09):

| shape | result |
|---|---|
| A — prefill `"I think option 1 is right.\n"` (the engine's shape) | **400** `continue_final_message is set but the final message does not appear in the chat after applying the chat template!` |
| B — the same prefill trimmed | **200**, content `"\n\nCHOICE: 1 (test)"` |

**Fix.** `gptForceClosePrefillBody()` (pure) builds the assistant content: embedded
`<think>`/`</think>` markers removed (they would move the template's own split), the trace
trimmed with Jinja's whitespace set, wrapped in the three-part shape. A whitespace-only trace
returns the empty think block rather than an empty string — an empty final message makes
`rindex("")` the END of the render, which would continue the prompt after `<|im_end|>` and open
a fresh turn.

Also shipped, both from the docket: (a) any non-200 with a body is logged to stderr/`gptLogLine`
and stamped on the record as `http_error_body` (600 B cap); (c) `forced_close_unrecorded` on the
gameend record — incremented when a close is armed while a previous one is still outstanding,
when a pending close is dropped because the decision changed, and for one still armed at game
end; cleared on the record that stamps `reasoning_forced_close`.

**GREEN.** PARSETEST models the template's transform exactly and asserts the built body is a
**fixed point** for six prefills including the two that 400'd, plus a RED assertion that the
pre-fix shape is NOT a fixed point.

**Prediction (falsifiable).** Next corpus: `http_status: 400` appears on **0** records; the
`transport` field carries no `http=400` on any forced-close record; `phase2_answer_missing`
falls to 0 from 2; `forced_close_unrecorded` is present on every gameend record and
`phase2_answer_recovered + phase2_answer_missing + forced_close_unrecorded` equals the game's
stderr count of `unclosed <think>` closes. If a 400 does recur, `http_error_body` names it.

---

## P21 — the reply cut at exactly 64 characters (MED)

**Repro.** `130v146` deck130 seq 16: reply `"\n\nPLAN: Hold priority, then activate Talisman
for {R}, cast Siege"` (65 chars incl. the leading `\n\n`), `reply_truncated: 1`,
`reasoning_chars` 23,318, `max_tokens` 6288 (`max_tokens_reasoning` 6000 +
`max_tokens_answer` 288), `transport curl=0,http=200`.

**The 64 is not a transport read and not a buffer.** 23,318 chars ÷ ~3.72 B/token ≈ 6,271
tokens of reasoning against a 6,288-token whole-decode cap: **the answer reserve is not
reserved**. `max_tokens` bounds the entire decode, the server spends it on reasoning first, and
the answer gets the remainder — here ~17 tokens. The think block CLOSED, so the forced-close
arm (which required `mLastReasoningOnly && content.empty()`) never fired, and a decision with a
complete 23 KB trace behind it fell to the heuristic, which cast Starstorm against a plan that
said hold (seq 17 recovery record).

**Fix.** `gptForceCloseEarned()` (pure): the wave-34 arm unchanged, plus `finishLength &&
codedAnswers == 0` — a reply that stopped at the cap without writing its answer line is the
same failure the forced close exists for, and the same rescue works (its own trace back, answer
only). Thinking-off is untouched (no trace to prefill, the predicate returns false).

**GREEN.** Five PARSETEST pins including three MUST-NOT-MATCH (a truncated reply that DID write
its answer; a natural stop with no answer; no trace to prefill).

**Prediction.** Next corpus: every record carrying `reply_truncated: 1` also carries `retry: 1`
and `reasoning_forced_close`, and none of them carries `fallback: unparsed_reply`.

---

## P9 — the `CHOICE:` that shares the PLAN's line (MED)

**Repro / RED on base.** `125v126` deck126 seq 154, `fallback: unparsed_reply`, verbatim
`"\n\nPLAN: Play a land to expand mana base and prepare to cast Chromatic Lantern. CHOICE: 1
(Play Sunpetal Grove)"`. `findAnswerLabelLine` only matched a label at a LINE start, so a
present, unique, legal answer was thrown to the heuristic.

**Fix.** `gptInlineChoiceOnPlanLine()`, tried only when the line scan found no head and no
rejection line, and only for `CHOICE:`. Every clause is a gate: the label occurs **exactly
once** in the whole reply; it is whitespace-anchored; its physical line **starts with the
line-leading PLAN marker**; the tail is not a rejection and passes `choiceLineIsClean`. It
returns the LABEL'S offset (not the line's), so `findPlanMarker` still picks the plan that
belongs to the answer and the carried plan is bounded before the answer — a line start would
have carried `CHOICE: 1 (…)` into the next window's plan.

This is invariant-000-clean: nothing here reads an unlabelled sentence, weighs a verdict out of
the reply's words, or licenses a correction paragraph. The golden protocol text is untouched
and still asks for the CHOICE on its own line.

**GREEN.** 11 pins: the repro parses, the segment and the plan bound are right, the two-line
shape still wins, and seven MUST-NOT-MATCH (two labels, rejection tail, prose after the answer,
no PLAN marker on the line, glued `MYCHOICE:`, a foreign `ATTACK:` label).

**Prediction.** Next corpus: 0 `unparsed_reply` fallbacks whose reply contains exactly one
`CHOICE:` on a `PLAN:`-leading line; `post_answer_overrun` and `coded_answers >= 2` counts
unchanged (the gate cannot fire on either shape).

---

## P14 — the repeat row's answerable short name (MED)

**Repro / RED on base.** `123v125` deck123 seq 58: `CHOICE: 3 (Create vampire with Bloodline
Keeper, repeated then stop)` over a PLAN reading `this window 21` → `repeat_count_missing`, one
token made, and **no re-ask** — while the row's own bracket says in as many words that copying
the name alone "names no count and is refused and re-asked". #W71-BO (R6) deleted the
repeat-count arm because it read 0 in the wave-70 corpus; O17 then removed the `N` placeholder
from the short name, which made the bare copy the natural thing to write, and the deleted arm
was exactly what the row still promised.

**Fix.** `repeatCountMissing` restored as a fourth trigger on the existing one-re-ask-per-board
priority ladder (`repeat_count_reask`), quoting the model's own latched line and the exact short
name the footer tells it to copy, via the pure `repeatRowShortName()`. It guesses nothing: no
count is invented, no row is withheld, the ceiling is unchanged, and a bare copy that comes back
a second time runs once exactly as it does today. Under the trust doctrine the alternative was
to delete the row's claim; keeping the claim and making it true is the better half.

**GREEN.** 3 pins on `repeatRowShortName` (round-trip off a rendered row, bracket not cut,
MUST-NOT-MATCH a non-repeat row).

**Prediction.** Next corpus: every `repeat_count_missing` record is followed by a
`repeat_count_reask` record on the same board, and `repeat_count_missing` takes that execute one
activation drop to at most the re-asked residue.

---

## P20 — near-duplicate degeneracy (MED, INSTRUMENT ONLY)

`reasoning_ngram_repeat` added beside `reasoning_degenerate`: whitespace tokens lowercased and
stripped of surrounding punctuation, 8-grams, reported as the share that are not first
occurrences. Floor of 64 tokens. Nothing gates on it; both numbers ride every record so a later
wave can retire whichever is blind.

**GREEN.** A synthetic 14-rerun loop with one edited token per rerun reads **< 0.10** on the
exact 40-char meter (the wave-74 blindness, reproduced) and **> 0.60** on the 8-gram meter; a
40-sentence varied trace reads < 0.35; below the floor it reports 0.

**Prediction.** Next corpus: on deck146-class seats `reasoning_degenerate` stays under 0.02
while `reasoning_ngram_repeat` has a materially higher median (the 319-of-322 protocol-requoting
traces become visible); the two are not rank-correlated.

---

## Weakest evidence

1. **P2's fix is proven against ONE template.** The fixed-point pin models the pilot's Qwen
   template; a llama.cpp or other-vLLM template with a different assistant render could still
   reject the prefill. The mitigation is now that the 400 body is logged, so the next such
   failure is diagnosable from the corpus rather than requiring a live probe. The two live
   probes used one request each and are not a sample.
2. **P2(c) counts arms, not stderr closes.** `forced_close_unrecorded` catches an arm dropped by
   the retry path or superseded by a later arm; a close lost inside the async layer without
   either happening would still be invisible. The identity `recovered + missing + unrecorded ==
   closes` is the check that will say so, and it is untested until a corpus runs.
3. **P21's ~3.72 B/token is inferred, not measured on that record.** The record carries
   `reasoning_chars`, not reasoning tokens. The arithmetic is consistent with the corpus's own
   measured 3.15–3.97 B/token band, and the causal claim (cap ate the answer) rests on
   `finish_reason: length` with a closed think block — but no probe re-ran that exact request.
4. **P21 changes latency.** A truncated-no-answer reply now buys a second round trip where it
   previously fell straight to the heuristic. The wave-74 rate was 1 in 2,396, so the cost
   should be negligible, but it is a cost and it is unmeasured.
5. **P9 and P14 are pinned on synthetic strings and one corpus record each.** Neither has been
   exercised in a live game on this binary; the corpus is the gate.
6. **P14's re-ask spends the board's single re-ask budget.** It shares
   `mPriorityReaskBoard` with `named_row` and `plan_missing` (the no-op arm has its own latch),
   so a window that would have earned a named-row re-ask and also names no count now gets only
   the first of them. No corpus case has both.
7. **I did not touch the golden protocol text, the thinking regime, or the harness flags**, and
   I ran no corpus. Every "next corpus" prediction above is unverified by me.
