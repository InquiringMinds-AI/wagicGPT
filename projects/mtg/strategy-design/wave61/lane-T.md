# Wave-61 lane T — C7, C8, C9, C11 (facts the hand/land/menu rows withhold)

Branch `w61-lane-T`, base master `718e99d2a`. Four files changed:
`projects/mtg/src/AIPlayerGPT.cpp`, `projects/mtg/src/AllAbilities.cpp`,
`projects/mtg/include/Player.h`, `projects/mtg/include/AIPlayerGPT.h`.

Every change is APPEND-ONLY onto an existing row, header or hand entry, plus one arity gate at the
reveal seam. No option is added, removed, reordered or capped; no answer index moves; no window is
closed; no cache is introduced. Every new clause is computed from facts the engine already holds
(the cast oracle, the primitive's own script, the printed card text) and quotes rather than asserts
wherever the fact belongs to the card.

---

## What shipped

### C7a — the hand listing states castability, with the REASON printed
`handCastabilityTag` (pure) + a per-name verdict pass in `serializeGameStateImpl` + an optional
`castTags` map on `describeZoneCards`.

The `Your hand (N cards):` line carried cost, type and P/T and never why a card had no cast row.
Five of deck123's six fallbacks and both of deck162's were one shape — the model named a hand card
that was not on the menu (`CHOICE: 3 (Cast Fate Unraveler)`, where row 3 was HOLD) — and where no
re-ask fired the omission corrupted the PLAN silently (`162v130` seq 18 built a whole lethal line on
an Ob Nixilis `{3}{b}{b}` at three sources, and then took the HOLD row it had not planned). Guide
prose has been tried for two waves and the count went 2 -> 5.

Every non-land hand entry now ends in one of six verdicts, taken from the CAST MENU's own oracle
(`LegalActionsOracle::legalCasts` under `GptManaPolicy`, the same willingness policy
`FindCardToPlay` uses) evaluated in the oracle's own gate ORDER, so the reason named is the FIRST
one that actually stops the cast and the hand line cannot contradict the cast rows in the same
prompt:

- `[castable now]`
- `[cannot pay now: needs 5 mana, you have 3 untapped sources]` (count short — the source count is
  the same `potentialColorReach` integer the `Mana available:` line three lines above is built from)
- `[cannot pay now: needs {1}{w}{w}, your 3 untapped sources cannot pay it]` (count sufficient,
  payment does not assemble)
- `[no cast row now: sorcery speed - only in your own main phase with an empty stack]`
- `[no cast row now: it must have a target and there is no legal target on the board]` (CR 601.2c)
- `[no cast row now: a play restriction forbids casting it]`

Lands get nothing here: the land drop is its own decision with its own row (C7b). The tag is LAST on
the entry, so the identifying facts are unchanged and two copies of one card still carry
byte-identical tails — the `#W55-D` copy collapse is unaffected. `GptManaPolicy` moved up the file
unchanged so both surfaces share one policy object definition.

### C7b — the land-drop row says a land enters tapped
`landEntersTappedTag` (pure) applied to the `Play <land>` rows in `FindCardToPlay`.

`123v152` seq 18 offered `1. Play Arcane Sanctum` under a parenthetical that reads as a promise the
land is available now, the seat wrote `Cast Thraben Doomsayer turn 6` into its plan off that window,
and turn 6 opened on two sources with a three-mana creature in hand — H1's four wasted windows.
The branch is read off the SCRIPT (`tap(noevent)`, and whether the same auto line is gated by
`aslongas` / `if(` / `restriction` before it) and the condition is QUOTED from the card's own printed
sentence, never asserted here:

- `Play Arcane Sanctum [enters TAPPED - it makes no mana this turn: "Arcane Sanctum enters tapped."]`
- `Play Isolated Chapel [enters tapped UNLESS its own condition holds, so it may make no mana this
  turn: "Isolated Chapel enters tapped unless you control a Plains or Swamp."]`

A land whose script has no `tap(noevent)` gets nothing, so every untapped land's row is byte-
identical to wave 60. Verified against `mtg.txt:4934` / `mtg.txt:59410` and against Scryfall
(Arcane Sanctum "This land enters tapped."; Isolated Chapel "This land enters tapped unless you
control a Plains or a Swamp.").

### C8 — a choose-ONE reveal is asked as one, and the narration reports the one card
`AllAbilities.cpp` arity gate + `singlePickOptional` through `Player::decideReveal` ->
`AIPlayerGPT::decideReveal` -> `buildRevealAskText`.

**Why the multi-select PUT shape was emitted.** The seam already had a choose-ONE branch; its gate
was `eligTc->maxtargets == 1 && eligTc->targetMin`. `targetMin` is set ONLY by a `<N>` arity prefix
that carries no `upto:` (`src/TargetChooser.cpp:522`). A BARE `target(...)` — the shape of Pelakka
Predation's option one, `optionone name(Choose a card) target(*[manacost>=3]|reveal) reject`
(`borderline.txt:82652`) — parses to `maxtargets = 1, targetMin = false`, so it fell to the SUBSET
branch: the ask instructed `PUT: 1, 3`, the model answered `PUT: 1, 3`, the engine's chooser
auto-fired on the first click and took ONE card, and `decideReveal`'s trim-to-the-first-pick — gated
on that same flag — did not run, so `chosen` listed two names and the narration asserted a two-card
discard that never happened, carried in every later prompt (`146v125` s18/s19).

The fix reads the chooser's ARITY: `maxtargets == 1` means at most one card can reach option one
whatever `targetMin` says. Widened only for a chooser built from abilityOne's OWN `target()`; the
defensive `tc` fallback keeps the prior test, so no reveal whose option one has no `target()` spec
changes shape. The mandatory/optional distinction is kept as its own flag so the wording stays
truthful: the `<1>` (Thoughtseize-class) wording is byte-identical to wave 20, and the new branch
reads `Choose ONE card to send to "Choose a card" ... This is a ONE-card choice: you can never take
two, and "PUT: none" declines.` / `On the FIRST line write PUT: followed by the ONE card number you
choose (e.g. "PUT: 2") - ONE number, never a list, or exactly "PUT: none" to choose no card`.

That wording describes what the ENGINE's chooser accepts and claims nothing about the card's own
"may": Scryfall's current Oracle for Pelakka Predation reads "You choose a card from it with mana
value 3 or greater" (the primitive's `text=` still carries the pre-errata "You may choose"), while
the engine's chooser carries no minimum and the decline path exists. Naming the decline is a fact
about the answer surface; asserting the card permits it would not be.

Because the flag also gates the existing trim, the narration follows for free: `chosen` and the
translog now carry exactly the one card the engine took.

### C9 — dungeon rows price VENTURES to completion, and B12's clause is made symmetric
`dungeonVenturesToCompletion` + `dungeonRowTag` (both pure) in `describeTarget`; the venture ask's
header; `ventureSourceSilentTag` on the venture rows.

The row priced the printed ROOM LIST length and the header then told the model to "weigh how many
rooms to completion" off it. A dungeon's rooms BRANCH, so the two are different quantities. The
engine's own explore ladder holds the true one: the `autocommandzone` line carrying
`completedungeon` under `restriction{compare(hascntexplore)~equalto~N}`. Read off THAT:
Tomb of Annihilation 4 (a branch through the Oubliette at 3), Dungeon of the Mad Mage 7, Lost Mine
of Phandelver 4 — against printed room lists of 5 / 9 / 7. `146v126` seq 26's own narration confirms
Lost Mine finishing on "venture step 4". A dungeon with two completion lines reports the longest as
the count and names the shorter branch; both are true and the difference is what a value pick weighs.
A script this cannot parse returns 0 and the row keeps the wave-59 shape byte for byte. The header
now names the same quantity and states plainly that printed rooms are not ventures.

**Why Acererak still picked Lost Mine 14/14 after B12.** The clause is not absent and it is not on
the wrong row: the wave-60 engine seat recorded it rendering on the Tomb of Annihilation row of the
Acererak ask, 1 of 1, on `opts[i]` — the same strings the model answers from. It is ASYMMETRIC. One
row of three carried a quote and the other two carried nothing, and an annotation that appears on one
row and not its siblings reads as decoration rather than as a fact about the menu — the same shape as
the `#W57-C` discard finding ("while two dead cards render differently no guide can teach which row
is the keep") and the deck146 MED banner flicker. So when the venturing source's text names ANY
offered dungeon, every offered dungeon now says which side of that line it is on:
`[Acererak the Archlich's own text does NOT name this dungeon]`. A venture whose source names no
offered dungeon is byte-identical to wave 60. The false room counts were NOT the driver here (Lost
Mine read as the LONGER list and was taken anyway); the counts are fixed because the prompt names
them as the decision input, not because they explain this pick.

### C11 — `{spare: ...}` is a verdict and is now conditioned
`discardSpareLandClause(myLands, highestHandCost, haveHandCost)`.

The clause printed on every land row whenever `myLands >= 1`. `130v123` seq 9 (turn 4, TWO lands
down, Siege-Gang Commander `{3}{r}{r}` in hand) marked all three land rows `{spare: you control 2
lands already}` and the seat answered `PUT: 4, 6` — the emitter marked exactly the cards the deck
guide protects and marked nothing else, and the bait was taken 1/1. `myLands` is a FACT; "spare" is
a VERDICT. The word is now gated on the comparison it implies — lands down must EXCEED the most
expensive card in hand plus one — and the same two numbers print as a bare fact otherwise:

- `{spare: you control 9 lands already; the most expensive card in your hand costs 3}`
- `{you control 2 lands already; the most expensive card in your hand costs 5}` (the seq-9 board)
- `{you control 1 land already}` (no non-land card in hand: no verdict is available either way)

No row loses its count, and every land row still carries a clause — only the verdict word is
conditioned, so the two land rows do not render asymmetrically.

---

## Gate

Detached units `w61-T-gate2` / `w61-T-gate3`, memory-capped 4G / no swap, from `bin/`:

- Suite, `WAGIC_TESTSUITE_THREADS=1`: **1248 tests, 0 failed**, `==Test Failed !==` **0**,
  `==Test timed out` **0**, **65 AI tests, 0 failed** — matching the brief's baseline exactly.
- PARSETEST: **3740 passed, 0 failed** (base 3699 -> **+41** cases: a positive, a must-NOT-match
  negative and the echo shape for each of C7a, C7b, C8, C9, C11).
- `git diff | /usr/bin/grep -c $'\357\277\275'` = **0**.

**One intermittent, reported rather than smoothed over.** An earlier full run of this same binary
returned 1 failure: `intrepid_adversary_repeated_payment.txt`, a narration assertion that pins the
auto-tap SOURCE ORDER (`expected "paid {1}{w} with plains #5; katilda"`, got a different but legal
six-payment order). It passes in isolation (`WAGIC_TESTSUITE_FILE` probe, 1 test 0 failed) and did
not recur in the two other full runs of this binary; a full base run (`718e99d2a` binary, same
command) was also 1248/0/65-AI-0. The diff cannot reach it: the GPT seat's render is never invoked
by the suite, and the only non-GPT file changed is the reveal arity gate, which this fixture has no
reveal for. Recorded as an observed order-sensitive flake in `#W55-OPT`'s option-preserving auto-tap,
NOT as a lane result — I did not prove it pre-exists (the base run happened to be clean).

## RED-on-base evidence

No engine-behaviour change ships — nothing here changes which cards move, which options exist, or
what the engine does with an answer — so no suite fixture is added. RED-on-base lives in the
PARSETEST corpus, on the pure functions, against the real primitive/Oracle strings:

- **C8** calls `buildRevealAskText` with the wave-60 flags for Pelakka Predation's shape and asserts
  it emits `comma-separated (e.g. "PUT: 1, 3")` — the exact instruction the model obeyed at
  `146v125` s18 — and then asserts the wave-61 flags emit a ONE-number protocol with no
  multi-select instruction anywhere in it.
- **C9** and **C11** carry byte-identity NEGATIVES from the other side: `dungeonRowTag(5,0,0,...)`
  is asserted equal to the wave-59 room-count string, and the wave-55 `{spare: ...}` cases are
  UPDATED (not deleted) to show the count still prints while the word is now conditioned.
- **C7b** asserts a basic Mountain's row is byte-identical to wave 60.

Note on C8 specifically: a suite fixture CANNOT be RED here. The engine already took exactly one
card on base — the divergence between the ask, the narration and the engine was the whole bug — so a
fixture asserting "one card is discarded" is GREEN on base and proves nothing. The falsifiable
artifact is the ask text and the narrated take, which is where the pins are.

## Predictions (falsifiable, for the wave-58/next corpus)

1. **C7a** — every `Your hand (N cards):` render carries a castability bracket on EVERY non-land
   entry; grep the corpus for `Your hand (` and check the co-occurrence with
   `[castable now]`/`[cannot pay now:`/`[no cast row now:` is 1.0 on prompts with a non-land hand
   card. And the `named_row_reask` cluster whose named row is a HAND CARD (8 across deck123 +
   deck162 in wave 60) falls to 0.
2. **C7a (outcome)** — 0 `PLAN:` lines name a cast whose hand entry in the SAME prompt carries a
   `cannot pay now` / `no cast row now` bracket. `162v130` seq 18's shape (a 900-word winning line
   built on an unpayable Ob Nixilis) does not recur.
3. **C7b** — 0 `Play <land>` rows for a land whose primitive carries `tap(noevent)` render without
   an `enters` bracket; every Arcane Sanctum / Isolated Chapel row carries one, and 0 plans written
   at such a window spend that land's mana on the same turn.
4. **C8** — 0 reveal asks whose option one is a single-target chooser render
   `comma-separated (e.g. "PUT: 1, 3")`; every Pelakka-class reveal narration line
   (`You revealed N cards and took ...`) names exactly ONE card, never two; 0 `PUT:` replies to such
   an ask carry more than one number.
5. **C9** — every dungeon row prints `completes after N ventures` with N = 4 / 7 / 4 for Tomb /
   Mad Mage / Lost Mine, and 0 rows print a bare `[dungeon: N rooms;`. On an Acererak-sourced
   venture ALL THREE rows carry a source clause (one naming, two `does NOT name`), and the Lost Mine
   share on Acererak-sourced ventures falls below 14/14.
6. **C11** — 0 `{spare:` renders occur at `myLands <= highest hand cost + 1`; the `130v123` seq-9
   board shape renders the bare count; and no cleanup discard sends a land while a card costing
   more than `myLands - 1` is in the same hand.

## What I did NOT verify

- **No live probe was run**, and the suite does not exercise the GPT seat (it needs an endpoint), so
  C7a's runtime block, C7b's row assembly, C9's dungeon-row assembly and the venture-row symmetry
  pass were verified by READING plus PARSETEST over the real primitive strings — none of the four
  was observed in a rendered prompt on this binary. The corpus checks in the predictions are what
  settle them.
- **C7a's residue reason.** `kHandNoLegalTarget` is assigned to everything that passes timing,
  restriction and payability and is still absent from `legalCasts` — I established that by reading
  the oracle's gate order (`src/LegalActions.cpp:41-100`), not by constructing a live case. If a
  gate is ever added to `legalCasts` ahead of the 601.2c check, that verdict becomes wrong and this
  is where it will show.
- **C7a cost/performance.** `legalCasts` + `potentialMana` now run on every `serializeGameState`
  call, including the livelock breaker's board hash. I did not measure it, and the extra work is not
  memoised.
- **C7b does not EVALUATE the condition** on a conditional tapped land — it says the land may enter
  tapped and quotes the card's own sentence. Whether Isolated Chapel enters tapped RIGHT NOW is
  still the model's to read off the board.
- **C8 reach beyond Pelakka Predation.** I fixed the gate, not a survey: I did not enumerate every
  `optionone ... target(` script in the primitives to count how many reveals change shape, nor
  confirm that no reveal relies on the old subset framing while carrying a `maxtargets == 1`
  chooser. The `oneTc`-only widening bounds the blast radius to choosers built from abilityOne's own
  `target()`.
- **C9 dungeon Oracle text.** The ladders were verified against `borderline.txt` and against the
  corpus's own narration (`venture step 4`); I did not cross-check the three dungeons' room lists on
  Scryfall. I DID verify Arcane Sanctum, Isolated Chapel, Pelakka Predation and Acererak the
  Archlich on Scryfall.
- **C11's "or the deck's curve top".** The verdict compares against the HAND's cost top only. A hand
  with no non-land card yields no verdict at all rather than falling back to the deck's curve.
- I did not adjudicate any other lane's items, and I did not touch the wave-60 MED items that sit
  next to mine (the banner flicker at `seq 93-100`, the malformed dungeon worked example, the MDFC
  "Cast Card Normally" face name, the repeated card-text blob) — they are outside C7/C8/C9/C11.
