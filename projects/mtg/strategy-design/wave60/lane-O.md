# Wave-60 lane O — B7, B8, B10, B12 (rows that hide what they do)

Branch `w60-lane-O`. One file changed: `projects/mtg/src/AIPlayerGPT.cpp`. All four items are
RENDER changes at the GPT seat — no engine behaviour, no option added or removed, no reorder, no
cap. Every new string is append-only onto an existing row or header; `req.optionTexts` (the
staleness key), the option ORDER, the answer index and every apply path are untouched.

## What shipped

### B7 — sweeper rows name the creatures they destroy
`sweeperVictimName` / `sweeperRosterTail` / `sweeperClause` (+ `boardCreatureCounts` now
optionally collects the two rosters, `boardTurnOnClause` passes them only on the sweeper branch).

The `{right now: destroys N of their creature ...}` clause priced a COUNT and named nobody in all
180 wave-59 renders, while every Path to Exile row already carried `removalVictimTag`'s
`{removes: A, B}`. The clause now appends `- THEIRS: <name> (P/T) (printed X/Y) [keywords]...;
YOURS: ...` INSIDE the existing `{right now: ...}` brace, built from the same facts the board
snapshot prints (display name + `instanceHandle` copy handle + live P/T + `printedPTTag` +
`keywordList`). With both rosters empty the string is byte-identical to wave-59 (PARSETEST pins
that). This puts deck125's own branch conditions — the creature's identity and its PRINTED power
— on the row where the decision commits instead of forty lines up the prompt.

### B8 — the activated clause survives the budget; tap/untap branches say which is which
(a) `boardEffectSnippetFocus` + `loyaltyClausePrefix` + a `focusPrefix` pass-through on
`optionCardTextCore` / `optionCardText`, consumed at `describeAction`'s `{card text: "..."}`.
`boardEffectSnippet` protects the LAST clause and shares the rest evenly, so on a planeswalker the
clause being ACTIVATED is truncated like any other: Teferi, Who Slows the Sunset's +1 cut one word
before "Untap the chosen permanents you control. Tap the chosen permanents you don't control." in
all 60 corpus renders. When the row knows which clause it activates — a loyalty ability's menu text
names itself with the same token the printed clause starts with ("+1:", "-2:", "-7:") — that
clause's allowance is now spent FIRST and at the whole-tag budget, exactly as `last` already is.
Printed clause ORDER is preserved and an omission stays COUNTED. An empty prefix, or one no clause
carries, returns `boardEffectSnippet` byte for byte, so no other row in the corpus moves.

(b) `tapUntapBranchTag`, applied to the CHOOSE_MODE rows. The sub-menus the +1 fans into reached
the model as bare `Choose your land` / `Choose opponent land`. The branch is read off the SCRIPT
the menu was built from (`choice name(<label>) target(<type>|mybattlefield) untap _ choice
name(<label>) target(<type>|opponentbattlefield) tap`), never guessed from the label's words; a
label the script does not carry gets nothing. The opponent-side row now states the timing the seat
got wrong four consecutive plans running: a tapped creature cannot block, so tapping takes it out
of blocking for the REST OF THIS TURN, but it UNTAPS in THEIR untap step at the start of their next
turn, so it does not stop it attacking.

### B10 — a forced sacrifice is headed as one, and each row is priced
`buildForcedSacrificeAsk` + `forcedSacrificeRowTag`, plus detection at the target seam. Tribute to
Hunger arrived as a generic `TARGET CHOICE for Tribute to Hunger - its "gain life equal to its
toughness" ability ... Pick the ONE target it will affect` over seven rows carrying a P/T and
nothing else. The existing `forcedSelfLoss` detector reads the effect NAME plus the waiting
ability's menu verb and never fired here (the waiting label is the life-gain rider). The new
detector reads the acting SCRIPT instead — a `sacrifice` over
`notatarget(creature|mybattlefield)`, walked through up to four `storedSourceCard` payload dummies
on both the chooser's source and the waiting ability's — and additionally requires that EVERY
candidate is one of this seat's own battlefield creatures, so nothing else can be re-framed. The
header names the forced sacrifice and, when a `toughnesslifegain` rider is present, who is paid;
each row gains `[you SACRIFICE this; they gain N life (its toughness)]`. The beneficiary is taken
from the script's own target word (`targetopponent` -> the other player, `targetcontroller` -> this
seat) and left UNNAMED when it says neither — a silent omission beats a wrong claim about who is
paid. Oracle-verified against Scryfall and the primitive (`mtg.txt` Tribute to Hunger: "Target
opponent sacrifices a creature of their choice. You gain life equal to that creature's toughness").

### B12 — the venture ask names its source, and the source's own dungeon clause
Header: `VENTURE with <source> - CHOOSE A DUNGEON to enter (...)`, using the same `effectName` the
generic target header already resolves (falls back to the wave-59 string byte-for-byte when the
source is unnameable). Rows: `sentenceNaming` + `ventureSourceDungeonTag` pull the SENTENCE of the
venturing card's own printed `text=` that names an offered dungeon and hang it on that dungeon's
row: `[Acererak the Archlich's own text names THIS dungeon: "When Acererak the Archlich enters, if
you have not completed the dungeon Tomb of Annihilation, return Acererak the Archlich to its
owner's hand and venture into the dungeon."]`. The clause is quoted from the card, never asserted
by this code; a dungeon the source does not name gets nothing. Verified against
`borderline.txt:441-445` and Scryfall.

## Gate

Detached unit `w60-O-gate`, memory-capped 4G/no-swap, from `bin/`:
- Suite, `WAGIC_TESTSUITE_THREADS=1`: **1247 tests, 0 failed**, `==Test Failed !==` 0,
  `==Test timed out` 0, **62 AI tests, 0 failed**.
- PARSETEST: **3555 passed, 0 failed** (base 3529 -> +26 cases: a positive, a must-NOT-match
  negative and the echo shape for each of B7, B8a, B8b, B10, B12).
- `git diff | grep -c $'\357\277\275'` = 0.

Note: the lane brief quotes 61 AI tests as the baseline (measured on 5039427da). This worktree's
base is master 8b922abf0 and the diff touches no test registry or fixture, so 62 is this commit's
base figure, not a lane addition.

## RED-on-base evidence

No engine-behaviour change ships, so no suite fixture is added. The RED-on-base evidence for the
one item whose old behaviour is reproducible as a pure function is IN the PARSETEST corpus: the B8
block calls `boardEffectSnippet(teferi, 140)` — the wave-59 code path, unchanged — and asserts it
does NOT contain "Untap the chosen permanents you control", then asserts `boardEffectSnippetFocus`
does. B7's byte-identity CHECK plays the same role from the other side.

## Predictions (falsifiable, for the wave-58/next corpus)

1. **B7** — every `{right now: destroys/exiles N of their creature ...}` render with N+M > 0
   carries a `- THEIRS:` roster; 0 sweeper rows print a count with no names. Grep the corpus for
   `right now: destroys` and check the co-occurrence rate is 1.0 on non-empty boards.
2. **B7 (outcome)** — a sweeper cast into exactly one opposing creature whose row shows
   `(printed 0/N)` or `(printed 1/N)` or `[defender]` drops below deck125's wave-59 rate of 5
   one-for-one sweeps in six games.
3. **B8a** — 0 renders of Teferi, Who Slows the Sunset's +1 row end in "and..."; every one contains
   "Untap the chosen permanents you control." Any planeswalker loyalty row whose menu text starts
   `<+|->N:` shows that clause untruncated or shows a visible "..." AFTER its verbs.
4. **B8b** — every `Choose opponent <type>` row on a tap/untap sub-menu carries
   `TAPS their <type>`; 0 plans claim a tap "stops an attacker" on the OPPONENT's following turn.
5. **B10** — 0 forced-sacrifice asks render the header `TARGET CHOICE for <X> - its "gain life
   equal to its toughness" ability`; every row of such an ask carries `[you SACRIFICE this`.
   Where the rows differ in toughness, the seat's pick is no longer the highest-toughness body at
   the wave-59 rate.
6. **B12** — 11/11 -> 0/N venture asks render without a source name; on an Acererak venture the
   Tomb of Annihilation row carries the `own text names THIS dungeon` clause, and the Lost Mine
   default share on Acererak-sourced ventures falls below 2/2.

## What I did NOT verify

- **No live probe was run.** Every claim above rests on PARSETEST over the real primitive/Oracle
  strings plus the suite; none of the four surfaces was observed in a live translog on this binary.
- **B10 detection reach.** I verified the SHAPE test against Tribute to Hunger's primitive by
  reading, not by driving the seam: I did not confirm at runtime that the chooser's
  `storedSourceCard` chain actually reaches a card whose `magicText` carries
  `notatarget(creature|mybattlefield)` in the granted-`ability$!` case. If it does not, the ask
  falls through to the wave-59 generic header unchanged (fail-silent, no regression) and the item
  is NOT fixed — the corpus check in prediction 5 is what settles it.
- **B10 beneficiary direction.** `targetopponent` -> "they gain" is my reading of the script token
  relative to the SACRIFICING seat (which is the seat this ask is rendered for), cross-checked
  against Tribute's Oracle. I did not test a card using `toughnesslifegain targetcontroller`; no
  such card was located in the corpus.
- **B12 source resolution.** The row clause only fires when a card in the source chain has
  `getDisplayName() == effectName` AND a non-empty `text`. On Acererak the venture rides a
  `transforms((,newability[... target(*[dungeon]|mysideboard) ...]))` dummy; I did not confirm at
  runtime that the walk reaches the Acererak instance. The HEADER half (naming the source) uses the
  already-proven `effectName` ladder and is therefore the more robust half.
- **B8b context card.** `tapUntapBranchTag` reads `ctx->magicText`; I did not confirm at runtime
  that `req.contextCard` for these sub-menus resolves to Teferi rather than to a nameless carrier.
  If it does not, no tag is emitted (fail-silent).
- **B8a interaction with the `//` face split.** The focus prefix is passed into every face of a
  double-faced card's text; I did not construct a case where a loyalty prefix appears on both
  faces.
- No performance measurement of the extra per-row script scans (they are not memoised through
  `scriptMemo`).
