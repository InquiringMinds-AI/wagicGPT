# Wave-54 lane B — reply-parser stamps (D13, D14, D15, D9)

Base: master `42b3bf5da`. Branch `w54-lane-B`, worktree `worktrees/lanes/w54-B`.
Baseline re-confirmed on this worktree BEFORE any edit (incremental build of the
seeded objs, then both gates under the 4G cap, `WAGIC_TESTSUITE_THREADS=1`):
**PARSETEST 2258 / 0 failed**; **suite 1219 tests, 0 failed, 49 AI tests / 0 failed,
0 timeouts**. (Single-threaded is ground truth, so the brief's two known
concurrency-only failures — `lifeline.txt`, `merrow_reejerey.txt` — did not appear;
no third failure and no timeout, so nothing on this worktree was mine to inherit.)

Everything in this lane is ADDITIVE observability plus ONE narrowing of the echo
matcher (D15, which is the docket's own ask). No deadline moved, no window closed,
no legal choice was capped, no cache was added, and nothing was deleted from any
render. All four items are pure functions, so the whole surface is PARSETEST-pinned.

## D13 — a note when the latched coded line's index AND parenthetical both differ from the executed row

`AIPlayerGPT::latchedRowMismatch(reply, choice, optionCount, optionTexts)` (pure,
static, header-declared). It reads the **LAST** line-leading `CHOICE:` line — the
line the engine's own last-wins rule honours — and returns true only when
* the head index is in the menu and is **not** the row that executed, **and**
* the parenthetical is a real name (>= 4 chars, not decline filler) that appears
  neither in the executed row's **annotation-stripped core** nor, word by word, in
  its label.

`writeTransLog` stamps `parse_note: latched_row_mismatch` on it, every seam.

Deliberately silent on the three shapes that look identical and are correct: a
name-over-index remap (the NAME won, so the parenthetical IS on the executed row),
a cosmetic prefix of a longer label ("Cast Vampire" for "Cast Vampire Nighthawk"),
and row 0 (pass), which owns no option text. Those are the 6 cosmetic + 15
prose-reversal disagreements of the corpus's 25; only the 2 genuine mis-executions
have both halves foreign.

**A note, not a re-ask.** The docket says "better, a re-ask" — a re-ask needs a
seam-side channel per decision kind, and the shape has 2 instances in 3,253
parentheticals; measuring it first is the cheaper order, and the note is what makes
the re-ask decidable next wave. Stated so it can be overridden.

## D14 — the ADDITIVE stamp: a no-op row taken against the reply's own PLAN

Two pure halves, both header-declared:
* `rowSaysNoOp(row)` — the row's OWN annotation, engine-computed from the live
  board, says the action does nothing: `does nothing`, `deals 0`, `destroys 0`,
  `kills 0`, `removes 0`, `drains 0`, `does not apply`, `gains 0`, `draws 0`.
* `planArguesAgainstRow(reply, row)` — the reply's PLAN (last `PLAN:` marker, the
  same anchor every other consumer uses) contains a **clause** that names this
  row's card AND carries a word arguing against doing it. Clause, not whole plan:
  the split is on `. ! ? ; \n`, so "Avoid casting Damnation this turn; Tribute to
  Hunger is the play." cannot be read as an argument against Tribute.

`writeTransLog` stamps `parse_note: plan_contradicts_noop_row` when BOTH fire on
the row that executed. **Stamp only** — no re-ask, no suppression, no narrowing, as
the docket directs; whether it should ever do more is a decision for after the
count exists. Lane J's detector reads the prose BEFORE the `CHOICE:` line and
structurally cannot see this; the two seq-73/74 replies put the contradiction
after it.

## D15 — an echoed name inside an annotation can never bind a row

Two changes:
1. `stripRenderAnnotationsLc` now drops **curly annotation groups** as well as
   bracketed ones. A mana symbol is told from an annotation by SHAPE — at most
   three characters, no space, no colon (`isManaSymbolBody`) — so `{2}{b}`,
   `{w/u}`, `{2/w}`, `{t}`, `{x}` stay on the core and `{right now: ...}`,
   `{kills: ...}`, `{spends 3 of your 5 ...}` do not.
2. The two remaining match tiers that read the RAW row now read the stripped core:
   the significant-words **tier 2** fallback and **tier 1** of the relaxed
   numeric/alpha pass. (Tier 1 of the words pass, the numeric disambiguator and the
   option-subset fallback already ran on `optionLabel`, which cuts at the first
   annotation.) Tier 2 keeps its purpose — it still reaches row text that sits
   AFTER an annotation, which `optionLabel` truncates away — and loses only the
   ability to match text INSIDE one.

`126v130` seq 21's shape now parses to `-1` + `stale_echo_in_range` (the verdict the
seam turns into the one named-row re-ask, which is what `126v125` seq 9 already got)
instead of executing row 1.

One EXISTING PARSETEST case changed meaning and was rewritten rather than deleted:
`W39-13 the labels stay on the OPTION line the model chooses from` asserted that
`stripRenderAnnotationsLc` KEEPS a `{right now: ...}` label. Its intent — the labels
stay on the option line the model reads — is now asserted against the option string
itself, and a second case asserts the new truth about the matching core. That
flip is also this lane's counterfactual receipt for change (1).

## D9 — `long_reply` and the elapsed fraction on an ANSWERED reply at the wall

`noAnswerClassFor` only ever classifies a reply that never came, and the wave-53
corpus had ZERO empty replies and six that reached 600 s and answered. Two pure
functions (`deadlineTenthsPct`, `isLongReply`) and one stamp in `writeTransLog`:
on `latency_ms >= 95%` of `mTimeoutMs` with a non-empty reply the record gains
`long_reply: 1`, `deadline_pct: <e.g. 96.5>` and `parse_note: long_reply`.

The 95% mark is deliberately the SAME mark the wave-53 worker's timeout test uses,
so the two arms partition the wall cleanly: empty at >= 95% is `fallback: timeout`,
non-empty at >= 95% is `long_reply`. `WAGIC_GPT_TIMEOUT` and every default are
untouched — the deadline is the owner's dial and this item is observability, exactly
as wave 52 ruled for D10.

## Gate (this worktree, after the change; memory-capped, single-threaded)

| | base (before) | after |
|---|---|---|
| PARSETEST | 2258 passed / **0 failed** | **2297 passed / 0 failed** (+39) |
| suite | 1219 tests / **0 failed** | **1219 tests / 0 failed** |
| AI tests | 49 / 0 failed | **49 / 0 failed** |
| timeouts | 0 | **0** |

Build: clean link, `make -f Makefile.sdl -j4`. `git diff | grep -c U+FFFD` = **0**.

## Falsifiable predictions for the wave-54 corpus

* **D13** — records whose latched coded line's index and parenthetical both differ
  from the executed row: every one carries `parse_note: latched_row_mismatch`
  (N/N). Expected N is small (2 / 3,253 in wave 53); N = 0 is also a pass.
  FALSIFIED by a reply-vs-executed audit finding a both-halves-foreign record with
  no stamp, or by a stamp on a record whose parenthetical IS on the executed row
  (a name-over-index remap wrongly flagged).
* **D14** — records whose chosen row carries a no-op annotation and whose PLAN
  argues against that row, carrying no stamp: **0** (was 4, at two seats). And the
  stamp must not appear on a record whose row states a live magnitude.
* **D15** — records whose reply names a card occurring on the menu only inside an
  annotation and which EXECUTE a row rather than re-asking: **0** (was 1).
  Watch for the cost of the narrowing: a rise in `stale_echo_in_range` /
  `named_row_not_offered` re-asks whose parenthetical was in fact a legitimate
  row name would falsify the "widen-only in effect" claim.
* **D9** — replies reaching >= 95% of `WAGIC_GPT_TIMEOUT` carrying no stamp of any
  kind: **0** (was 1 stamped of 6 above 600 s). Every such record carries either
  `fallback: timeout` (empty) or `long_reply` + `deadline_pct` (answered).

## What this lane did NOT verify

* **No live model run.** Nothing here was exercised against a real endpoint; every
  claim is PARSETEST-level over strings taken from the wave-53 ledger's quoted
  records. The corpus is the real gate.
* **The pre-fix RED for D15's parseChoice outcome was not re-run on the base
  binary** (`archives/wagic-ef133ea2a-w54base` exists but its PARSETEST corpus
  predates these cases). The pre-fix verdict — `126v130` seq 21 returned row 1 with
  `echo_index_conflict` — is the ledger's recorded observation, not something this
  lane reproduced. The stripper half DOES have a live counterfactual: the existing
  W39-13 case went RED on this build before it was rewritten.
* **D13's re-ask half is not built** — only the note. If the wave-54 corpus shows
  the shape recurring, the re-ask is the next step, and it belongs at the seams
  that already own one.
* **D14's phrase list is a whitelist**, drawn from the five annotation phrasings and
  the negative vocabulary the ledger quotes. A no-op annotation worded some other
  way, or a plan that argues against a row without any of the listed words, is a
  false negative this lane cannot bound. Nothing rides on it, so a miss costs a
  record field, never a decision.
* **`isManaSymbolBody` is a shape rule**, not a mana parser. A future annotation
  shorter than four characters with no space or colon would survive as if it were a
  cost symbol; none exists today.
* **No render string changed**, so no board/option surface was re-measured for
  prompt size.
