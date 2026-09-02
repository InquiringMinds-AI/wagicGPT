# Wave-53 lane N — the priority-window economy, the pass row's own name, the plan's age

Base: `master 3dfd77d65` (worktree `w53-lane-N`). Baseline re-confirmed on this worktree
before any edit: **PARSETEST 2121/0**; suite **1210, exactly 2 failed** (`lifeline.txt`,
`merrow_reejerey.txt` — the known concurrency-only pair) **+ 44 AI/0, 0 timeouts**.

Files touched: `projects/mtg/src/AIPlayerGPT.cpp`, `projects/mtg/include/AIPlayerGPT.h`. Nothing
else. Both spliced as bytes (`rb`/`wb`); `git diff | grep -c $'\357\277\275'` = **0**.

---

## D2 (HIGH) — the model-takeable HOLD row, plus the declined-list annotation

**What the row is.** `Hold priority - do not ask me again this turn unless the board changes`,
rendered as the LAST numbered row (with the other declines) on **both** dead-window seams:
the priority menu (`chooseOrderedAction`) and the casting menu (`FindCardToPlay` →
`askModel`, the seam that owns 884 of the corpus's 966 opponent-turn windows). No window is
removed, no row is withheld, nothing is cached blind: the engine replays **an answer the model
itself gave**, and only while the board it gave it on still stands.

**Scope decision, stated because it is mine and not the docket's.** The row is offered on the
**opponent's turn only**. The defect is measured there (966 windows / 85 acts; 286 windows at
six phases with zero casts), and on the seat's own turn the board changes with every land drop
and cast, so a hold taken there would be taken and retired in the same breath. This adds an
affordance in one scope rather than removing anything in either.

**The re-openers, and why the control survives.** The latch is `(turn, phase-stripped
situation, per-seam row set)`. It re-opens on:
* **any board change** — the key is `serializeGameState()` **minus its leading phase line**, so
  life, poison, both battlefields, both hands and the **stack (top-first)** all retire it; a new
  stack object is a board change by construction;
* **a newly affordable row** — any row on today's menu that was not on the menu when the hold
  was taken re-opens it, even on a byte-identical board;
* **the turn ending.**
A row *disappearing* is **not** a re-opener (the model already declined it).
The phase line is deliberately excluded: phase progression is not a board change, and it is
exactly the information `130v152` seq 12→13 acted on. **That control is untouched for a
different and stronger reason: it is not suppressed by anything here, because that pilot never
took a hold row.** It declined with `Cast nothing right now`, which latches nothing. The 30
real actions are protected by the row being a CHOICE, not by any key's sensitivity.

**The declined-list annotation ships alongside the row, not instead of it.**
`[you declined this exact list N times already this turn]`, keyed on the joined option list,
per turn, incremented once per **real** answer (never on a cached replay). It is a
**PROMPT-ONLY** annotation, spliced in after the option list and deliberately kept **out of
every ask key** — a number that rises with each answer inside the key would mint a fresh
question at every AI tick and turn the state-plus-question cache (and the deadlock breaker
riding it) into an HTTP call per tick, which is D2 inverted. `askKey`/`mLastAskKey`/`mAskCache`
are byte-identical to base.

**Observability.** `hold_windows_skipped` on the gameend record (beside
`mana_only_windows_skipped`); stderr on every path — the take
(`the model took the hold row at the <seam> seam on turn N`), each honoured window
(`holding priority at the <seam> seam ... M windows held this game`), and each re-open
(`hold re-opened at the <seam> seam - the board changed` / `- a row is newly available`).
The take itself writes an ordinary `priority` translog record whose `chosen_text` is the row.

**Falsifiable prediction (D2).** On the next corpus, with the same decks: opponent-turn casting
windows per game **down ≥ 30%**, opponent-turn casts held at **≥ 90% of 68**;
End / Cleanup / Attackers / Combat-ends contribute **0** casts again; runs of 3+ consecutive
byte-identical declined menus **< 10%** of decisions (was 23.0%). Falsifier: if
`hold_windows_skipped` is ~0 across the corpus the row is not being taken and the item failed;
if opponent-turn casts fall below 90% of 68 while `hold_windows_skipped` is large, the hold is
eating live windows and the re-opener set is too narrow.

## D9 (MED) — `CHOICE: n (Pass)` resolves to the pass row

`parseChoice` gains `passRowOffered` (defaulted **false**; passed **true** only from the
priority seam, which is the only seam carrying `0. Pass priority`). Where it is true, a
parenthetical that is **exactly** one of the reserved pass echoes — `pass`, `pass priority`,
`0`, `pass (0)`, `0 (pass)`, `pass this window`, `no action`, or the pass row's own full text,
trimmed and case-folded — **names row 0** and binds **ahead of every index and staleness
verdict**, stamped `pass_row_named` (never stamped on a coded `0`, which already is that row).
It had to bind that early: `pass priority`'s only significant word is "priority", which matches
no option, so the word pass was failing the whole reply to the heuristic.
Where `passRowOffered` is false — every `askModel` menu, which `kNoPassRowFact` tells the model
is mandatory — **nothing changes**.

**Falsifiable prediction (D9).** `CHOICE: n (Pass)` on a menu carrying `0. Pass priority`
executes the pass row **N/N**; total name-vs-executed mismatches **≤ 1 / 3,000**. Falsifier: any
record whose reply's parenthetical is a reserved pass echo and whose `choice` is not 0.

## D12 (MED) — the plan's age, the latched coded line, the protocol's intent rule

* **(a)** `YOUR PLAN (as you last stated it, N windows ago on turn T): …`. `N` is
  `mTransSeq - mPlanSetSeq` — the same seq arithmetic a corpus reader does (`146v125` seq 163 →
  177 renders "14 windows ago on turn 32"); the stamp is omitted only when the model stated the
  plan at the window being rendered. The age resets whenever the model **writes** a PLAN line,
  whatever its content, and is cleared with the plan on expiry.
* **(b)** New translog field **`latched_coded_line`**: the 1-based ordinal, among the reply's
  line-leading `CHOICE:` lines, of the first that resolves to the record's `choice`. Written on
  every record with `coded_answers >= 2` and an option list — which is exactly the five
  `plan_choice_conflict` records that held the first answer in `reply` and the latched row in
  `choice` with nothing tying them together.
* **(c)** `kReplyProtocol` (code-appended, so a stale user template cannot drop it) gains:
  *"Write the PLAN as INTENT … The line you write is stored and re-served to you VERBATIM at
  later decisions, unchanged and unchecked, so any board fact you put in it … will still read as
  true after that has stopped being true. The board you act on is the CURRENT SITUATION block,
  never your own plan."*

**Falsifiable prediction (D12).** PLAN blocks rendered with no age stamp: **0/N** (excluding the
window the plan was written on); decisions taken on an echoed plan whose board claim is false:
**0** (was 3 windows in `126v146`); every `plan_choice_conflict` record with
`coded_answers >= 2` carries `latched_coded_line`.

---

## Tests

**PARSETEST: +32 cases, five new sections** (`#W53-N D2` ×3, `D9`, `D12`) —
2121 → **2153, 0 failed**. They pin:
* the hold row's exact literal, and NEGATIVEs that it advises nothing and hides nothing;
* its reply shapes: the full row echoed back, the short name `Hold priority`, and the NEGATIVE
  that a real cast on a hold-bearing menu still executes;
* **the re-opening rules** (`holdBoardKeyOf` / `holdStillStands`): a phase step alone does not
  re-open; a life change does; **a new stack object does**; **a newly affordable row does, on an
  unchanged board**; a row disappearing does NOT;
* the annotation's echo shape, singular and plural, its absence at N=0, and the NEGATIVE that
  the annotation echoed back as a name resolves to no row;
* the D9 repro verbatim (`CHOICE: 1 (Pass)` on the seq-91 Kaya menu → row 0, `pass_row_named`),
  every reserved spelling, a numberless `(Pass)`, and three NEGATIVEs: no pass row offered → the
  index stands; a card name containing "pass" (`Passwall Adept`) is not the pass row; a coded
  `0` earns no note;
* the D12a clause shapes and the whole header line at the repro's own age; D12b's ordinal on a
  two-coded-line reply plus its NEGATIVE; D12c's two protocol sentences.

**Suite: no fixture added, and here is why.** Every behaviour changed by this lane lives behind
`AIPlayerGPT::chooseOrderedAction`'s `if (!ranking.size() || mEndpoint.empty()) return
AIPlayerBaka::…` guard and behind `askModel`, i.e. behind a **live model endpoint**. The suite's
AI fixtures run the Baka heuristic with no endpoint, so a fixture pinning the hold row's
re-opening would be **GREEN on base** — it could not be RED-on-base, which is the point of the
pin. The re-opening is therefore pinned in PARSETEST against the pure helpers the seams call
(`holdBoardKeyOf`, `holdStillStands`), which is the only place it is testable without an
endpoint. Full suite re-run as the regression gate: **1210, exactly 2 failed** (the same
`lifeline.txt` + `merrow_reejerey.txt` pair as base), **44 AI/0, 0 timeouts** — no third
failure, no timeout, no memory kill under the 4G cap.

## Gate

| | base (3dfd77d65 binary) | after |
|---|---|---|
| PARSETEST | 2121 passed / 0 failed | **2153 passed / 0 failed** |
| suite | 1210 (2 failed) | **1210 (2 failed)** |
| AI suite | 44 (0 failed) | **44 (0 failed)** |
| timeouts | 0 | **0** |

Both runs under `systemd-run --user -p MemoryMax=4G -p MemorySwapMax=0`, waited on with a
foreground until-loop on `~/.gatelogs/W53_N_DONE`. Logs: `~/.gatelogs/w53-N-base.log`,
`~/.gatelogs/w53-N-post.log`.

## What I did NOT verify

* **No live-model probe was run.** Nothing here was exercised against
  `qwen36-35b-a3b` — every claim about what the model will DO with the hold row (whether it
  takes it at all, and how often) is an untested prediction, and `hold_windows_skipped` on the
  next corpus is the measurement that settles it.
* **The hold's cross-seam behaviour is untested end-to-end.** A hold taken at the casting seam
  does not suppress the priority seam (each seam is keyed separately, so the first window at the
  second seam is always asked). By design; unmeasured. Worst case is two asks per turn instead
  of one, never a suppressed window.
* **`firstCodedChoice` was NOT given `passRowOffered`.** On a priority record whose answer
  resolved to row 0 by name, `answer_replaced` may still read `true` where the pass-named line
  was the first coded line. A reporting-field edge, not an execution one; left alone to keep the
  blast radius at the parse seam.
* `salvageLoopedChoice` likewise does not know about the pass row; it only runs when the primary
  parse already failed, and the D9 path returns 0 rather than failing, so it is unreachable for
  this class — argued, not measured.
* **`hold_windows_skipped` and `latched_coded_line` were not read back off a real translog** —
  they are written by code paths that need an endpoint. The JSON shapes were not validated by a
  corpus reader.
* The declined-list annotation's *effect* on the model is unmeasured; only its rendering,
  keying and cache-neutrality were verified.
* I did not re-derive any of the wave-52 corpus numbers; they are quoted from the ledger.
