# Wave-73 lane BZ — the stop grammar + list truth (N3, N4, N7, N10, N11, N15)

Base `90f8d1d68`. One file touched: `projects/mtg/src/AIPlayerGPT.cpp`.
Gates: PARSETEST **5519 passed / 0 failed** (base 5487 → +32 cases); suite THREADS=1
**1281 tests, 0 failed; 76 AI tests, 0 failed** (none of the three known flakes fired).
`git diff | grep -c U+FFFD` = 0. No corpus, no VPK, no protocol-text change.

RED-on-base log: `~/.gatelogs/w73-BZ-red-on-base.log` — a temporary probe block written with
BASE symbols only, compiled and run on `90f8d1d68`: **5 FAIL** (N3, N4, N10, N11, N15), then
removed. N7 has no base symbol to fail: its RED is documentary (see below).

---

## N3 HIGH — the stated stop is never parsed — **CONFIRMED, fixed on both sides**

Repro (engine-seat HIGH-2 / deck123 HIGH-1): `123v162` seq 11,
`PLAN: Tap Thraben Doomsayer 22 times to reach 24 creatures (stop count 24, currently at 2,
making 22 this window), ...`. `repeatPlanScanNumberAt` skipped only ` = : ( ` and the word
`is`, so `stop count 24` yielded −1; `repeatRowStopClause` therefore rendered 0 times over 379
Doomsayer prompts and M10's collapse was unreachable. The row's own bracket asked for the two
numbers *in words* ("stating your stop count, the count you are at now") — the surface taught
exactly the wording that broke the parser.

Fix, both halves:
* **RENDER** (`repeatRowLine`, and the identically-worded `plan_missing` re-ask): the bracket
  now states the literal shape — `a count on this row REQUIRES a PLAN line carrying the two
  numbers in this exact shape - "stop=<N>; M=<N>", where stop= is the count you mean to finish
  at and M= is the count you are at now - plus how many you perform this window`. This is row
  text, not the golden protocol (`reply-protocol.txt` / `kReplyProtocol` untouched).
* **PARSER** (`repeatPlanScanNumberAt`): between a LABEL and its number, a bounded list of
  filler words is skipped — `is`, `count`, `at`, at most two of them, whole words only. Nothing
  else is skipped and no number without a label in front of it is ever taken.
* **SPAN** (new `planLineOnly`): the stop/count store at the ask seam was fed
  `protocolLinesOnly` — the PLAN line **and the action line** — so a count echoed in a CHOICE
  parenthetical could supply half a "stated stop". It now reads the PLAN line and only that.

Accepted stop forms (each pinned, positive): `stop=N`, `stop N`, `stop count N`, `stop at N`;
counts `M=N`, `M N`, `M is N`, `M at N`. MUST-NOT-MATCH pinned: a bare number with no label
(`make 24 creatures, then 2 more`); a plan stating no numbers; `stopped` (label inside a word);
a `stop=24; M=2` written on the **CHOICE line** instead of the PLAN line; prose with no PLAN
label at all. BX F2's ownership test (`their stop=29`) is re-pinned and still refuses.

Prediction (next corpus): `stop=`/`M=` appear in deck123's PLAN lines, `repeatRowStopClause`
renders > 0 in Doomsayer windows, and `stop_reached_windows_skipped` can be non-zero for the
first time. If the clause still renders 0 while Doomsayer is on the board, the defect is
upstream of the parse (the store's date guard, `mStatedStopTurn`), not the grammar.

## N4 HIGH — rung menus sorted lexicographically — **CONFIRMED, fixed**

`152v125` seq 62: 15 rows ordered `1/1, 10/10, 11/11 … 2/2 … 9/9`; the reply
`CHOICE: 14 (becomes a 14/14 hydra)` bound index 14 = the **9/9** and produced
`echo_index_conflict` and one of the corpus's two fallbacks. Cause: the priority menu's
byte-stable render order is a plain `a.first < b.first`. Fix: `naturalTextLess` — digit RUNS
compare as numbers, everything else byte for byte; equal magnitudes fall back to the raw run so
the order stays total and the prompt stays byte-stable for an unchanged board (the wave-41
livelock rule). Pinned: the 14-rung menu renders 1/1…14/14; the byte comparator provably
mis-orders it; non-numeric rows keep byte order; no row precedes itself; `x 07` vs `x 7` keeps
a total order. Prediction: 0 `echo_index_conflict` notes on rung menus next corpus.

## N7 HIGH — no mana cost on reveal/search rows — **CONFIRMED, fixed**

`146v125` seq 14: the ELIGIBILITY header gates on mana VALUE (`only a manacost>=3 card may go
to "choose a card"`) and no row printed a cost; the biggest body read as the biggest threat and
the seat took Emrakul over the Supreme Verdict that swept it at seq 39. New `revealCostTag`
prints both facts the header decides on — `[cost: {4}{W}{W} (mana value 6)]` — off
`card->data->getManaCost()`, and says `[no mana cost (mana value 0)]` rather than leaving a gap
(the {0}-cost lesson). Pinned: both faces, and the echo shape (a `[...]` tail, which answer
matching strips off an already-anchored candidate). **RED is documentary**: the base reveal-row
builder contains no `getManaCost` call at all (grep on `90f8d1d68`), and the review quotes the
three rendered rows with no cost on any of them.

## N10 MED — worked example truncated mid-token — **CONFIRMED, fixed**

`substr(0, 48)` emitted the halves of quotes and parens it had opened — 79 of 279 exemplified
asks malformed, 20 reasonings calling the prompt a typo. New `exampleCoreTrim`: a row that
already FITS is returned byte for byte (so the wave-50 `Cast Fire // Ice (fused)` pin still
holds); an over-long row is cut at its own first separator (` - ` / ` (`) — where the name ends
and the gloss begins — then at a whitespace boundary, then trailing tokens are dropped until
quotes/parens/braces/brackets are balanced. All three observed shapes pinned as a class
(`Nadaar, Selfless Paladin #1`, `goblin lair`, `Hold priority`), each balanced and ≤ 48; an
over-long single token still yields a printable core, and a core that trims to nothing routes
to the FORMAT template with `*usedRow = 0` rather than printing an empty name.

## N11 MED — loyalty clause truncated in the board frame — **CONFIRMED, fixed**

BV M16 gave every loyalty clause the focus allowance on a walker's ROWS; the board frame has no
row and therefore no focus prefix, so it kept the flat per-clause budget and cut the ultimate
mid-sentence in 92 prompts. New `firstLoyaltyClausePrefix` names the walker's first loyalty
clause and the frame calls `boardEffectSnippetFocus` with it — M16's loop then covers every
loyalty clause. Pinned on Sorin, Lord of Innistrad: the −6's `under your control` survives now
and did not on base; `+1:` and `−2:` still print, in printed order; a non-walker text returns
"" and routes to `boardEffectSnippet` byte for byte.

## N15 MED — the noop re-ask named a row the reply had not written — **CONFIRMED, fixed**

`152v125` seq 62 wrote `CHOICE: 14 (…14/14 hydra)`, the short-name match rescued it to row 6,
and the re-ask opened "You chose row 6". `noopReaskLine` now takes the parser's own
`name_over_index` verdict and the written line, and leads with *why*: `Your line ("CHOICE: 14
(becomes a 14/14 hydra)") names a row by its short name, and that short name is row 6 on this
list - not the number in the line - so this is about row 6.` The matched row is still named by
number and by its own text. Wired at all four `noopReaskLine` call sites (priority latch,
priority re-ask ladder, ask latch, ask re-ask ladder). NEGATIVE pinned: an index answered as
written gets no name-match clause.

---

## Not done / weakest evidence

* **N7 has no RED test**, only the base-source grep plus the review's quoted rows — the only
  item here whose "before" is not a failing assertion.
* **N3's parser change cannot make `123v162` seq 11 a complete stop**: that reply states its
  stop (`stop count 24`, now parsed) but writes its count as *"currently at 2"* — no `M` label
  — so `repeatPlanStopAndCurrent` still returns false on it. The brief's accepted-form list is
  labelled forms only, and inventing an unlabelled "currently at N" read would be a prose read.
  The half that closes this case is the RENDER, and its payoff is a **prediction about a future
  corpus**, not something this lane proved.
* **Every fix here is pure-function pinned; none was observed in a live game.** N4's numeric
  order and N11's frame allowance change bytes in the composed prompt for boards this lane
  never built — the composed-prompt check for those is the next corpus's specimen.
* `naturalTextLess` changes the render order of ANY menu containing digit runs, not only rung
  menus (e.g. copy ordinals `#1 … #10`). That is the intended generalisation, but its blast
  radius is wider than the one repro, and byte-stability is argued from the comparator's
  totality rather than measured over a corpus.
