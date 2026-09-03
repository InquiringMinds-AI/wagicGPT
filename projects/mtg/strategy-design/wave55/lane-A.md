# Wave-55 lane A — the priority seam and the HOLD row: D2, D4, D11, D19, D21

Base: `master 8a1768d1c` (worktree `w55-A`, branch `w55-lane-A`; lane R's stall-floor hotfix is in
the base and is not touched here). Baseline re-confirmed on this worktree's seeded binary BEFORE
any edit, single-threaded (ground truth): **PARSETEST 2489 / 0 failed**; suite **1228 tests, 0
failed**, **51 AI tests, 0 failed**, **0 timeouts**, no memory kill under the 4 G cap. The brief's
two known concurrency-only failures (`lifeline.txt`, `merrow_reejerey.txt`) did not appear
single-threaded, and there was no third failure. Logs `~/.gatelogs/w55-A-base-{parsetest,suite}.log`.

One file touched: `projects/mtg/src/AIPlayerGPT.cpp`, spliced as bytes (python `rb`/`wb`).
`git diff | /usr/bin/grep -c $'\357\277\275'` = **0**. Every comment tagged `#W55-A (Dn)`.

---

## D2 (HIGH) — the mandatory life-loss loop: the key, in two halves, neither a cap

**(a) the repeat/de-dup key is the OPTION SET.** New pure helpers `optionSetKeyLine` /
`optionSetKeyOf`: a row reduced to its name, its mana cost and its ordinals, via the existing
`stripRenderAnnotationsLc` (which already drops `[...]` groups and every `{...}` group that is not a
mana symbol) plus one further canonicalisation, `castDeclineRow`'s Main-1 clause (that half is D19).
The 130v126 seq 67-106 repro is 40 consecutive decisions on ONE two-row menu whose row 1 priced
itself against a life total ticking one point per iteration — 21 distinct rendered tuples, so every
key built from the rendered rows read them as 21 different questions. Both seams' declined-list key
(`listKey`, priority + cast) and the hold latch's row sets are built from it now.
**The annotations are rendered in full and unchanged — only the key moved.**

**(b) the HOLD row's own key.** `holdKeyDroppedLine` drops the life line as well as the hidden-zone
counters, so a life-only tick no longer retires a hold the model itself took (`123v126` took the row
six times inside the loop and it stopped nothing). Scope, stated: this is the HOLD key only; the
life line is on every prompt, and every other re-opener stands — the stack top-first, either
battlefield, poison, a newly available row, the turn ending. Any event that could change the answer
still re-opens the window; a number moving while nothing else does is the loop the model opted out of.
The 130v126 loop's stack line is unchanged across all 40 iterations, so the latch survives it.

## D4 (HIGH) — the HOLD row is offered on the seat's OWN turn

Both seams (`chooseOrderedAction`'s priority menu, `FindCardToPlay`'s casting menu) now offer the row
on EVERY window, not only the opponent's turn — the docket's "or simply always". The wave-53
exclusion's rationale ("on the seat's own turn the board changes with every land drop and cast") is
refuted by `126v125` seq 72-121: 50 byte-identical own-turn Blockers windows, every reply a decline,
no board change and no row to close them with. `holdBoardKeyOf` already retires the hold on any real
change, so nothing about the guarantee is weakened; the model still opts in, one row at a time.

## D11 (MED) — the two rows no longer collide, in the parser and in the prompt

**(a) an EXACT row label beats a prefix.** The wave-54 prefix-head test (`passEchoAmbiguousWithHold`)
is retired: every reserved pass echo IS row 0's own name, which is what makes it reserved, so it can
no longer be made ambiguous merely by heading another row's text — that shape was 13 of wave 54's 16
`pass_hold_ambiguous` stamps and every one of them executed row 0 correctly. ONE shape stays
genuinely ambiguous and keeps the stamp: the coded INDEX names the hold row while the echo names row
0 (`146v126` seq 54/56/57, all executed on the hold row) — the reply's two halves disagree and the
index is the only half that tells the rows apart. The answer is still never a third row.

**(b) the leading words differ.** `kHoldPriorityRowText` is now
`Hold priority for the rest of this turn: pass now, and do not ask me again unless the board changes
(any change re-opens this window; you give up no cast)`. It leads with its own act instead of row 0's
name, keeps the pilot's own verb one clause later, and keeps the guarantee. `isReservedHoldEcho`
accepts the new head (through its colon), the row echoed back with its benefit annotation, and both
earlier spellings.

## D19 (MED) — the declined-count annotation reaches the own-turn Main 1 / Main 2 pair

ANNOTATION only; no suppression is added and none is proposed. The cause was mechanical and is
D2a's: `castDeclineRow` renders `Cast nothing right now (combat comes next this turn)` at Main 1 and
`Cast nothing right now` at Main 2, so the identical own-turn list keyed as two lists and the count
never reached the pair (`162v126` s7-s12). The clause states a PHASE fact, so the option-set key
drops it; the rendered row is untouched.

## D21 (MED) — the HOLD row states its BENEFIT

The row now carries ` {taking this row skips the rest of this turn's identical windows}`, a `{...}`
annotation like every other priced fact, so the option-set key does not see it and the row's identity
is one string on every window.

**One deliberate deviation from the docket's literal clause, and the reason.** The docket's wording
also put the declined COUNT on the row (`you have declined this same list N times this turn; ...`).
The count is kept OFF the row: the row's text is inside the ask key at both seams and inside the
priority seam's `unchanged` deadlock key, so a number that rises with every answer would mint a fresh
question and re-mint the deadlock key — the exact trap `declinedListNote` was built to avoid and
which its own comment names. The count is stated by that prompt-only note one line below the list,
and D19/D2a are what finally deliver it to these windows. Two rows of the same prompt, both true.
Also `this turn's`, not the docket's `their turn's`: after D4 the row renders on the seat's own turn,
where "their turn" would be false.

---

## Tests

**PARSETEST 2489 -> 2511 (+22), 0 failed.** Two new sections (`#W55-A D2a`, `#W55-A D21`) plus
amendments to the wave-53/54 pins this lane supersedes.

* **#W55-A D2a** — the repro's two menus (life 26 / life 25) render differently and key IDENTICALLY;
  the key literal (`cast spark spray {r}`); four must-NOT-match NEGATIVEs (a different mana cost, a
  different name, a different target ordinal, a menu that gained a row are all different keys);
  **D19** the two `castDeclineRow` spellings key alike and the Main 1 / Main 2 menu pair is one list;
  and the latch itself — a hold taken inside the loop STANDS at the next iteration, while a newly
  available row and a permanent arriving each still re-open it with their own `why`.
* **#W55-A D21** — the clause literal; the rendered row's shape; the clause is out of the option-set
  key; a NEGATIVE that it advises nothing, counts nothing and claims no turn it is not on;
  `holdRowIndexOf` still finds the rendered row; ECHO SHAPE — the row echoed back WITH its annotation
  answers as its own row; must-NOT-match — the annotation echoed alone never steals the cast row.
* **#W55-A D11a/D11b amendments** — the reworded literal, its lead words, a NEGATIVE that neither row
  heads the other any more; the new disambiguating prefix and the pre-reword literal both still name
  the hold row; `CHOICE: 0 (Pass priority)` and `CHOICE: 0 (pass)` bind row 0 with NO stamp (the 13
  false positives); `CHOICE: 2 (Pass priority)` on a menu whose row 2 is the hold row still takes the
  hold row and IS stamped (the 3 genuine ones).
* **#W55-A D2b amendments** — wave 54's `a life total change IS a board change` and `a life change
  still re-opens the window` are FLIPPED, which is the item; the life line is out of the hold key
  while the stack line is in it; and a NEW negative pins that poison, which is also a resource line,
  is NOT dropped.

**No suite fixture, same reason as wave-54 lanes A and N** — every behaviour here lives behind
`AIPlayerGPT`'s `mEndpoint.empty()` guard and behind `askModel`, i.e. behind a LIVE MODEL ENDPOINT.
The suite's AI fixtures run the Baka heuristic with no endpoint, so a fixture pinning the option-set
key, the hold key, the row's wording or the parse tie-break would be GREEN ON BASE — the opposite of
a pin. No non-GPT engine path is in the diff (one file, `AIPlayerGPT.cpp`). The pins are therefore
the PARSETEST cases against the pure helpers the seams call (each RED on base: the helpers do not
exist there, and the four amended cases assert the opposite of what base asserts), and the full suite
is the regression gate.

## Gate (this worktree, incremental build, single-threaded suite)

| | base (8a1768d1c seeded binary) | after |
|---|---|---|
| PARSETEST | 2489 passed / **0 failed** | **2511 passed / 0 failed** (+22) |
| suite | 1228 tests / **0 failed** | **1228 tests / 0 failed** |
| AI suite | 51 / **0 failed** | **51 / 0 failed** |
| timeouts | 0 | **0** |
| memory kills under the 4 G cap | 0 | **0** |

Logs: `~/.gatelogs/w55-A-base-{parsetest,suite}.log`, `~/.gatelogs/w55-A-post-{parsetest,suite}.log`.

## Falsifiable predictions (per docket item)

* **D2** — decisions inside a single-phase mandatory life-loss loop: **< 5 per loop** (was 40 and 29);
  `identical_option_asks_resolved > 0` OR `hold_windows_skipped > 0` on any gameend carrying such a
  loop; identical-declined runs **< 12%** of decisions and max run length **< 20** (this corpus:
  16.1% / 17.9% by predicate, max 50). Falsifier: a loop still spends >= 20 windows — then the key
  was never the binding constraint and the pilot's willingness to press the row is.
* **D4** — no run of >= 20 byte-identical declined OWN-TURN windows exists (was one of 50); every
  own-turn decline window renders the HOLD row (**N/N**); `hold_windows_skipped` non-zero on more
  gameends than wave 54's 7 of 42. Falsifier: the row is offered on own-turn windows and the 50-run
  class does not shrink — then the row is not the remedy and D2's key is the whole answer.
* **D11** — `pass_hold_ambiguous` **<= 4**, every firing of the `CHOICE: n (Pass priority)`-with-the-
  hold-index shape; `CHOICE: 0 (pass)` and `CHOICE: 0 (Pass priority)` never stamp again; no record
  whose reply names a pass row executes a cast. Falsifier: any `pass_hold_ambiguous` whose reply
  coded 0.
* **D19** — `[you declined this exact list N times already this turn]` appears on the SECOND ask of a
  repeated own-turn Main 1 / Main 2 pair: **N/N** (was 0 at the 162 seat); corpus-wide renders of the
  note rise above wave 54's 327. Falsifier: an own-turn Main 2 ask repeating Main 1's list with no note.
* **D21** — HOLD takes in windows carrying the declined-note at N >= 3: **>= 20%** (was 10.4%);
  `hold_windows_skipped` sum **> 72**. Falsifier: takes stay at or under 12% — then the benefit
  clause is not the lever either and the remaining barrier is the pilot's, not the render's.

## What I did NOT verify

* **No live-model probe was run.** Nothing here was exercised against `qwen36-35b-a3b`. Every claim
  about what the pilot DOES with the reworded row, the benefit clause, or the own-turn offer is an
  untested prediction; the wave-55 corpus is the measurement.
* **No live game was played at all** — the option-set key, the hold key's life drop, the own-turn
  offer and the new row text were verified against the PURE helpers and the parser with situation
  blocks I composed to match `serializeGameState`'s emitters (`Your life: N | Opponent life: M`, the
  stack line, `Opponent hand size:`, `Your library:`). I did not diff a captured prompt, and I did
  not observe a hold being taken, surviving or retiring in a running game.
* **D4's own-turn offer has no test of any kind.** It is a two-line change at each seam inside
  endpoint-gated code; only the build and the code path were checked. Its risk, stated: on its own
  turn a seat that presses the row closes the rest of that turn's identical windows, and the
  protection against that being wrong is entirely `holdBoardKeyOf` — which now ignores life. A seat
  holding at its own Main 1 and then being burned to a lethal total by a triggered ability whose
  stack object has already resolved would not be re-asked at THAT seam until something else moves.
  I judged the combination safe because a resolving object changes the stack line and a damage source
  arriving changes a battlefield line, but I did not enumerate the engine's paths to prove it.
* **The 13-vs-3 split of `pass_hold_ambiguous` is quoted from the ledger, not re-derived** — I did not
  re-read the corpus records. Likewise every wave-53/54 number in this file.
* **`optionSetKeyLine` collapses two rows that differ ONLY in a space-bearing `{...}` annotation.**
  That is the intent, but it means a genuinely different option that differs from another only inside
  an annotation would be one key to the declined count and to the hold's "newly available row" test.
  I did not audit the corpus's menus for such a pair; the canonicalisation of parentheticals is
  restricted to the single `castDeclineRow` literal precisely to keep this bounded.
* **The declined-count is now shared across Main 1 and Main 2 of the same turn**, which is what D19
  asks, and it also means the count reaches its thresholds sooner on own-turn windows than it did.
  Nothing keys a behaviour off the count except the prompt-only note, so I expect no other effect —
  but that is an argument, not a measurement.
