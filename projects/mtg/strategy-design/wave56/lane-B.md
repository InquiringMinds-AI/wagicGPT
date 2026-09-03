# Wave-56 lane B — render prices (D2, D6, D14, D15, D9, D13, D10)

Branch `w56-lane-B`, worktree `worktrees/lanes/w56-B`, base master 104668cfa (src = b228b8648).

## Gate

| | base (this worktree, first run) | after |
|---|---|---|
| PARSETEST | 2636 passed / 0 failed | **2685 / 0** (+49, all `#W56-B`) |
| suite | 1229 tests, 2 failed (`lifeline.txt`, `merrow_reejerey.txt` — the known concurrency-only pair), 51 AI / 0, 0 timeouts | **1230 tests, 2 failed (the SAME two), 51 AI / 0, 0 timeouts** |

The +1 suite test is this lane's new fixture. Final gate ran on a hermetic `make -B`
(`~/.gatelogs/w56-B-build.log`, rc 0), logs `~/.gatelogs/w56-B-parsetest.log`,
`~/.gatelogs/w56-B-suite.log`. `git diff | grep -c $'\357\277\275'` = 0.

## D2 (HIGH) — the ability row's life total

**What `action.player` actually pointed at: nothing. It was never written.**
`AIAction`'s player-ability constructor
(`AIAction(AIPlayer*, MTGAbility*, Player*, MTGCardInstance*)`, `include/AIPlayer.h`) is the only
ctor that takes a player target for an ABILITY, and its initializer list set
`owner, ability, click, target, playerAbilityTarget` — never `player`. The field was therefore
INDETERMINATE on every ability action, and `describeAction`'s row branch read it FIRST
(`Player * pt = action.player; if (!pt) pt = dynamic_cast<Player*>(action.playerAbilityTarget);`),
so the `dynamic_cast` fallback that would have found the real seat never ran. `pt->life` then read
whatever the reused stack slot held — a stale non-Player value whose `life` offset reads a constant,
which is the corpus's base of **166** (`2 damage -> at 164` on 176 rows, `1 -> at 165` on 61). The
seat LABEL ("targeting the opponent") was right only by luck: a garbage pointer is != `this`.
Sibling found in the same audit: `AIAction(AIPlayer*, MTGCardInstance*, MTGCardInstance*)`
(`src/AIPlayer.cpp`) left `playerAbilityTarget` indeterminate; a plain card action could answer a
`playerAbilityTarget ?` test with garbage. Both fields are now initialized.

Shipped: (a) both ctors initialize every pointer; (b) ONE resolver, `AIAction::targetedSeat()`
(ability target first, then the spell-target field), used by the render AND by the new suite
command so the two can never disagree; (c) `describeAction` resolves through it and prices only a
LIVE seat (`pt == this || pt == opponent()`, else no life is printed at all).

Pins: PARSETEST walks every opponent life 34..1 at both corpus magnitudes and requires
`K == life - damage` exactly (skill #259 — the number checked against the record's field, not the
clause checked for presence), plus a must-NOT-match on `at 164` / `at 165`, plus the lethal branch
and the echo shape. Suite fixture `w56b_ability_row_seat.txt` + new scripted command
`assertabilityseat <life> <card name>`: it builds the SAME action the seam builds (Staff of Nin,
the corpus's own source, opponent at 17) and requires the resolved seat to be the live opponent
with life 17 and the spell-target field to be NULL.
**Counterfactual run:** with the ctor initializer reverted and everything else identical, the
fixture is RED — `resolved seat OK, spell-target field NOT NULL (indeterminate), life expected 17
got 17`. Restoring it is GREEN. Note honestly what that RED does and does not prove: it proves the
field was indeterminate-and-non-NULL (the exact mechanism the old render read), not that the render
printed 166 in that fixture — the suite has no GPT seat rendering rows.

**Prediction.** Rows printing `they would be at K` where `K != opp_life - damage`: **0 of N**
(wave-55: 238 of 248). A lethal face row is reachable from an ABILITY menu at least once.

## D6 — `INCOMING THIS COMBAT:`
New board line in `serializeGameStateImpl`, emitted on EVERY window while the opponent is the
active player and has attackers declared (so the creatureless seat that is never handed a
`blockers` ask gets the number too): `INCOMING THIS COMBAT: 3 attackers, 11 unblocked damage - you
would be at -1; this KILLS you`. `unblocked` counts only attackers with no blocker assigned. When
the seat HAS bodies the line splits with the engine's own `potentialBlockerCount()`:
`(of that, 4 from 1 attacker none of your creatures can block)`, or, when nothing is evasive,
`(your creatures may legally block every attacker in that total)`.
**Prediction.** Windows during the opponent's combat with attackers declared and no printed
incoming total: **0 of N** (wave-55: 50 of 50).

## D10 — `Their untapped sources: N (colours they could make: {u}{w})`
Emitted under the opponent battlefield block, from the SAME engine call our own
`Mana available:` line uses (`ManaEngine::potentialColorReach` on the opponent, one entry per
source card, colours deduped). `0` is printed as `0` — a tapped-out opponent is the fact a
counterspell window turns on, so its absence never has to be inferred.
**Prediction.** Prompts whose opponent battlefield block carries no untapped-source count: **0**.

## D9 — the edict tie's quantifier
`highestMvEdictClause` now leads with the actor and the ONE:
`they sacrifice ONE of these 24, their choice - all tied at MV 0 (their highest): Human #1; ...`.
Nothing is deleted (the tie list and "their choice" both survive; the trailing
"- they choose which one" moved to the front as "their choice").
**Prediction.** Replies whose PLAN asserts an enumerating clause removes more than one object:
**0 of N** (wave-55: 2 of 2 consecutive plans).

## D13 — the blocking trigger, un-nested, plus the standing cost
`combatTradePreviewStats` gained two OPTIONAL out-params (`outBlockTrigger`, `outBlockerDies`);
with them NULL every existing caller renders byte-identically (pinned in PARSETEST). The B-line
emitter passes them, so the row now reads
`- may block A1 (your blocker dies, attacker lives) {blocking trigger, this combat: you gain 3 and
may gain 2 more} {after this combat: you control 1 fewer blocker - 2 available now, 1 if this one
dies here}` — three annotations in the order the decision uses them, and the third number (the body
the seat stops owning) exists for the first time. Both new annotations are decision-time and are
added to `stripNarrationDecoration`, so neither enters history.
**Prediction.** B-lines whose gain is printed INSIDE the survival verdict: **0**. Blocks taken at a
losing trade while the seat is one body from an unblocked lethal swing: fewer than wave-55's
`126v146` shape (not a 0-claim — this is a teach the render supports, not a legality gate).

## D14 — the equipment cast row
`{right now: you control 0 creatures - this equips nothing}` on an equipment cast row at zero own
creatures, counted with the same creature predicate the board header uses. A living-weapon-style
equipment (its text names a token/germ/living weapon) brings its own body, so the claim would be
false and the tag is omitted rather than guessed.
The item's second half — `<name> already wears <equipment>` on a re-offered equip line — is
ALREADY shipped and was left alone: `equipHolderNote`'s same-host branch renders
`(ALREADY attached to it - this would change NOTHING; no equip step is left to take with X)`.
Re-rendering the same fact in a second vocabulary would only add prompt weight.
**Prediction.** Equipment casts taken at `of which 0 are creatures`: **0** (wave-55: 1 of 3).

## D15 — two rows, one card, two prices
New menu pass `applyDuplicateEffectTags`, run next to `applyMenuFitTags` on the same menu copy
(a row cannot know its number until the suppression filter and any re-ask removal have settled).
Two rows match when they name ONE card AND carry the same non-empty `{right now: ...}` verdict
(brace-balanced extraction, so a mana symbol inside a verdict cannot truncate it); the DEARER row
gains ` {same effect as row N, for K more mana}` pointing at the cheapest match. No row is removed
or reordered. Deliberately conservative: rows with no verdict, rows at equal cost, and `{X}` rows
(no converted cost at this seam) are never compared — "same effect" stays a fact, never an
inference.
**Prediction.** Takes of a row while the same card with the same `{right now:}` verdict is on the
same menu at a lower cost: **0** (wave-55: 1 of 1).

## What I did NOT verify
- **No live game was run.** Every claim here rests on the unit corpus (PARSETEST), the suite, the
  counterfactual on D2's fixture, and code reading. No corpus, no probe deck, no GUI, no model call.
  Six of the seven items (all but D2) have NO suite pin at all — they are pure builders proven in
  PARSETEST and wired at one call site each; the wiring itself (which window each line reaches, and
  what it looks like in a real prompt) is unverified until the wave-56 corpus.
- **D6**: `potentialBlockerCount()` is PERMISSIVE by design (a creature already assigned elsewhere
  still counts), so the "none of your creatures can block" half can only UNDER-claim, never
  over-claim. Not exercised against a real evasive board. The `unblocked` total also ignores
  trample carry-over past an assigned blocker — a blocked trampler contributes 0 to the printed
  total, which under-states the incoming damage; I chose the under-claim over an unprovable number.
- **D10**: `potentialColorReach` on the opponent is called once per situation render. I did not
  measure its cost on a large opponent board, and I did not check it against a hidden-information
  boundary beyond the battlefield (it reads their untapped permanents only — nothing from hand).
- **D15**: cost comparison uses the row's own `ManaCost` (alternative cost when the row is an
  alternative cast). I did NOT verify that an exile-cast surcharge ("costs {2} more than printed")
  is inside that object for every such row; if it is not, that row's K would be the printed
  difference rather than the paid one. The wave-55 repro (Damnation {2}{b}{b} vs {4}{b}{b}) prints
  two different cost tokens, which is consistent with the surcharge being in the instance's cost,
  but I did not prove it on a live board.
- **D13**: the ATTACKERS window still nests its own lifelink/blocking-trigger clauses; only the
  blockers window was de-nested (that is where the docket's repro and the decision live).
- **D2**: I did not audit the other consumers of `AIAction::player` / `playerAbilityTarget` for
  behaviour changes now that both fields are deterministic. `AIAction::Act`'s
  `if (player && !playerAbilityTarget)` was already guarded against the indeterminate case, and the
  suite is unchanged, but nothing else was traced.
