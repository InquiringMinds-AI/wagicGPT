# Wave-79 lane DD — the reply parser reads the label-less answer line

Base: `b1e1db7be` (worktree `worktrees/lanes/w79-DD`, branch `w79-lane-DD`) — the wave-79
corpus binary's source.

## The ruling (verbatim, owner, 2026-09-12)

> "no such rule exists. Explicitly the rule is to 'make the parser robust'. And that 'any plan,
> must precede action.' Mechanically, for an llm, a plan that follows a choice is post hoc
> justification instead of load bearing reasoning."

It corrects a rule the loop had invented for itself — wave-78 lane CX pinned, in code, that "the
parser is UNCHANGED … owner ruling pending", and wave-79 lane DB repeated it ("the parser is
byte-identical"). Both pins are RETIRED HERE and INVERTED IN PLACE (not deleted: the CHECK lines
still stand at the same sites, stating the read instead of the refusal, so the history of the
claim survives in the corpus that enforces it).

The two constraints that survive:

* **(i) the protocol TEXT is untouched.** `bin/Res/ai/gpt/reply-protocol.txt` and `kReplyProtocol`
  are byte-identical on this branch (`git show --stat` names two files, neither of them a Res
  asset). Nothing licenses prose in the reply. Invariant 000(b) stands as written.
* **(ii) PLAN BEFORE ACTION.** An answer that PRECEDES the plan is refused and re-asked exactly
  as it is today — that is the ruling's own clause and it is pinned as a MUST-NOT-MATCH.

Everything else is the parser's job: read the answer wherever it unambiguously is.

## RED evidence — the 12 corpus replies, and what base did with them

Every reply below is verbatim off a record's `reply` field; every option list is that record's own
`options_text`, verbatim. Both are compiled into PARSETEST as the `kW79DD[]` fixture table and run
through the LIVE chain (`answerSegmentStatic` → `gptAnswerSegment`, the selector `consumePlan`
itself calls → `parseChoice` with the real rows).

Corpora: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260911-125420-final` (wave 78) and
`matchups-20260911-010157-final` (wave 77).

| log / seq | wave | reply (verbatim) | row | base |
|---|---|---|---|---|
| 125v123 s83 | 78 | `\n\nPLAN: Cast Sphinx's Revelation for X=4.\n2 (Cast Sphinx's Revelation)` | 2 | refused |
| 125v123 s140 | 78 | `\n\nPLAN: Play a land and hold for Emrakul next turn.\n3 (Cast nothing right now)` | 3 | refused |
| 125v123 s215 | 78 | `\n\nPLAN: I will hold priority to skip further windows this turn, as the opponent has no threats and I have no plays to make right now.\n2 (Hold priority)` | 2 | refused |
| 125v123 s305 | 78 | `\n\nPLAN: I will hold priority to end my turn and proceed to the next turn to attack with the two Staffs of Nin, which will win the game in 4 turns.\n4 (Cast nothing right now)` | 4 | refused |
| 125v126 s107 | 78 | `\n\nPLAN: Pass main phase and hold priority.\n4 (Cast nothing right now)` | 4 | refused |
| **162v146 s16** | 78 | `\n\nCast nothing right now.\n3 (Cast nothing right now)` — **no `PLAN:` label either** | 3 | refused |
| 125v130 s303 | 78 | `\n\nPLAN: Pass upkeep, cast Staff of Nin in main phase 1.\n2 (Hold priority)` | 2 | refused |
| 125v123 s315 | 77 | `\n\nPLAN: Opponent is at 5 life, my two Staffs will deal 2 damage per turn, securing the win in 3 turns. Stop 3 prohibits Sphinx's Revelation, and there are no threats to answer. I will hold priority to close the turn.\n2 (Hold priority)` | 2 | refused |
| 125v126 s140 | 77 | `\n\nPLAN: Let Staff pings resolve and hold priority to stop the menu loop.\n2 (Hold priority)` | 2 | refused |
| 125v126 s201 | 77 | `\n\nPLAN: Wait for Staff of Nin to ping the opponent down to 0.\n3 (Cast nothing right now)` | 3 | refused |
| 125v126 s219 | 77 | `\n\nPLAN: Exquisite Blood converts my damage to their life gain, so I will avoid dealing damage and hold priority to survive.\n5 (Cast nothing right now)` | 5 | refused |
| 125v162 s49 | 77 | `\n\nPLAN: Cast nothing right now.\n4 (Cast nothing right now)` | 4 | refused |

All twelve carry `fallback: unparsed_reply` and `choice: -1`; eleven are the deck125 seat, one is
deck162. Two were costly: `125v126` s140 and s201 asked to hold Path to Exile and the heuristic
spent it (the trailing `recovery` record on s140 reads `executed_text: "cast Path to Exile"`).

**RED, executed.** With `w79LabellessAnswerLine` forced to `return false` (which IS base
behaviour — base's selector was `findAnswerLabelLine` alone and the function does not exist
there), PARSETEST is **6893 passed, 9 failed**, and the per-case line prints
`read=0 row=-1` for all twelve. Log: `~/.gatelogs/w79-DD-parsetest-RED.log`. Every MUST-NOT-MATCH
below stayed GREEN under RED, which is the direction that proves they are not testing the new code
into existence.

Base is also pinned inside each case rather than only asserted here: `codedAnswerCount(reply) == 0`
(no coded answer line anywhere) and `!gptInlineChoiceOnPlanLine(reply, …)` (P9's inline reader does
not rescue it) hold for all twelve, which is exactly why base found nothing.

## The fix

Three pure pieces plus one selector, all in `src/AIPlayerGPT.cpp`.

1. **`w79BareAnswerLineSpan(text, start, end)`** — is ONE already-trimmed line the protocol's own
   action line with the label elided: `<digits> [ws] ( <non-empty name> )`, the closing paren
   matched **by balance** from the opener and being the LAST byte of the line. So `1 (Goblin (1/1))`
   is read whole (the #W50-Y D8 shape), while `2 (Hold priority) is best` is prose. The row number
   must LEAD the line, so `not 2 (Hold priority)` (wave-75 P9) is refused by SHAPE rather than by a
   blacklist of negation words.

2. **`w79LabellessAnswerLine(text, planLineStart, hadCodedAnswerLine, &segStart, &segEnd)`** — five
   unambiguity clauses, each a direction the read must not go:
   * no coded answer line anywhere in the reply, of any label (a labelled line always wins);
   * EXACTLY ONE line in the reply has the bare-answer shape (two is two answers);
   * it is the LAST non-blank line (nothing may be said after the answer);
   * at least one non-blank line PRECEDES it (an action with no plan stays refused — the deleted
     #W70-BM E2 bare-head tolerance does not come back through this door);
   * when a line-leading `PLAN:` marker exists, the answer starts AFTER it — **the ruling's clause**.

3. **`gptAnswerSegment(...)`** — THE selector: the labelled walk (`findAnswerLabelLine`, unchanged),
   then piece 2 when the labelled walk found no answer *and* refused no rejection line. `consumePlan`
   calls it; so does PARSETEST's `answerSegmentStatic`. That is deliberate: the corpus cases exercise
   the code the ask seam runs, not a mirror of it (the wave-64 lesson — two scanners that "agree"
   about the same bytes eventually do not). `labelLineStart` is set to the answer line's own start,
   so the plan carry (`planEnd`) still excludes the action line and it is never folded back into the
   plan the next window is served.

`parseChoice` is UNCHANGED and is still the only resolver: it already reads a bare
`<n> (<name>)` segment (it has since wave 22), so the reader's job ends at LOCATING the line.

**The 7th wave-78 fallback (`162v146` s16, no `PLAN:` label) IS read**, and the decision is the
ruling's: the answer line is the sole bare-answer line, it is the reply's last word, and a line
precedes it — so plan-before-action holds and there is no ambiguity about which line is the answer.
Its deviation class is unchanged (`unlabelled_plan`): naming a record's shape and locating its
answer are two jobs and stay two functions.

**Phase-2 recovery** consumes into `content`, which goes to `consumePlan` like any other reply, so
it reads the same shape for free. Its CENSUS did not: `hasCodedAnswerLine(content)` alone would have
stamped `phase2_answer_missing` on a consume `consumePlan` was about to answer from. That predicate
now also accepts the label-less shape (pinned). **`answerTailFromReasoning` is deliberately left
label-anchored** — it selects a tail out of a REASONING trace, which is full of numbered
parentheticals, and that is the genuine ambiguity the ruling still excludes. **The forced-close
prefill path** is untouched and needs nothing: `w76OffProtocolIsPrefillEcho` and the
`forced_close_prefill_echo` class arm still run ahead of everything, and an echoed trace is never a
lone action line.

## Re-derived wave-78 lane-CX pins (not deleted)

* `w78AnswerLabelAbsentShape` **keeps its job**: it NAMES the record's shape for the census. Its
  "MEASURE ONLY / the parser is untouched" paragraph is replaced with the ruling and a pointer to
  the reader. The class is deliberately the NARROWER of the two — the census wants the clean
  two-line population; the reader wants every answer it can locate unambiguously.
* `w78LabelAbsentCounted` → **`w79LabelAbsentRead`**, and the counter
  `answer_label_absent_heuristic_played` → **`answer_label_absent_read`** (member
  `mAnswerLabelAbsentHeuristicPlayed` → `mAnswerLabelAbsentRead`; header + ctor initializer list
  both moved). **Renamed, not retired.** Same denominator, same class, opposite arm: it counted the
  class's COST (played by the heuristic), it now counts the class's READS. The wave-78 reading of any
  corpus is recoverable from it (cost = the `answer_label_absent` population − this count) and it is
  a live regression check — a record classed `answer_label_absent` that is NOT counted here is one
  the reader refused.
* `w79RecoveryLabelAbsent` / `recovers_seq` (wave-78 S10, wave-79 DB T1): untouched.

## GREEN — PARSETEST 6883 → 6902, 0 failed (+19)

* **+1** the twelve-case corpus table (one CHECK, per-case diagnostics printed on failure).
* **+16** the refusal block: the ruling's clause both ways (answer-before-plan refused / same two
  lines in the ruled order read), two bare answers, prose after the answer, nothing before the
  answer, a bare row beside a real `CHOICE:` line, `not 2 (…)`, the inline `CHOICE:` on the PLAN's
  physical line (still accepted, still by P9's reader), bytes after the paren, an empty short name,
  a rejection-only reply, out-of-range + unoffered-name through `parseChoice`, the balanced
  `1 (Goblin (1/1))`, prose-between-the-parts (read AND still charged off-protocol), and the
  phase-2 census.
* **+2** the two retired pins, inverted in place (#W78-CX S1, #W79-DB T1).

## What is still REJECTED, and why

| shape | verdict | why |
|---|---|---|
| an answer line BEFORE the plan | refused, re-asked | the ruling: a plan after a choice is post hoc justification |
| two bare answer lines | refused | two answers is ambiguity, not a missing label |
| anything said after the action line | refused | the reader takes the reply's last word or nothing |
| an action line with nothing before it | refused | an action with no plan; the bare-head tolerance stays deleted |
| a bare row number beside a real `CHOICE:` line | the labelled line wins | that reply is prose and stays prose |
| `not 2 (Hold priority)` | refused | the row number must lead the line (wave-75 P9) |
| bytes after the closing paren / empty short name | refused | not the protocol's action line |
| a rejection-only reply | refused | #W50-Y D7 — a refusal is not an answer with its label missing |
| out-of-range row, or a name no option carries | `parseChoice` fails it | reading the line adds no tolerance to resolution |
| prose beyond the two parts | the numbered line is read; the prose is still charged `off_protocol_bytes` and still classes `prose_outside_two_lines` | reading an answer does not launder a rambling reply compliant |

No legal option is removed, capped or auto-answered anywhere in this lane. The heuristic fallback
path is intact and still answers every window the parser refuses.

## Prediction (falsifiable)

1. In the wave-80 corpus, **zero** `ask` records carry `fallback: unparsed_reply` whose `reply` is a
   plan line (labelled or not) followed by a single in-range `<n> (<short name>)` line — the
   population that was 12 across waves 77-78.
2. The deck125 seat's gameend `answer_label_absent_read` is **> 0**, unless its guide edit removed
   the habit entirely (in which case the class's population is 0 too, and the two must move
   together).
3. For every gameend, `answer_label_absent_read` equals the number of `ask` records in that seat
   classed `answer_label_absent` — a shortfall is a record the reader refused, and its reply is the
   counter-example to read.

Falsifiers: a record classed `answer_label_absent` that still carries `fallback: unparsed_reply`
(the reader missed a shape it should have taken); or a `choice` executed from a reply whose numbered
line PRECEDES its plan (the ruling's clause leaked).

## Weakest evidence

* **No live game ran on this branch.** The twelve cases are replayed corpus bytes through the live
  selector and `parseChoice`, not a seat consuming a real window: the chain from
  `consumePlan`'s return to the click is exercised by every other record in every corpus, but not by
  these twelve on this binary. The wave-80 corpus is the real gate.
* **The phase-2 census fix is pinned on the predicate, not on a live forced close.** No forced close
  in either corpus returned a label-less action line, so that arm has never actually fired in the
  wild; it is a shape agreement, and `phase2_answer_recovered` moving is the observable that would
  confirm it.
* **`answerTailFromReasoning` was judged, not measured.** Leaving it label-anchored is a judgement
  that a bare numbered parenthetical inside a reasoning trace is genuinely ambiguous. No census
  counts how often a trace ENDS on such a line, so the judgement is unquantified.
* **The `162v146` s16 read is one instance.** A plan-shaped sentence with no `PLAN:` label is a
  weaker anchor than a labelled one; one corpus record is thin evidence that the shape is stable
  rather than a one-off.
