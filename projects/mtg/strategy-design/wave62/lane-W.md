# Wave-62 lane W — D1, D15, D17 (the land-drop and hand rows)

Branch `w62-lane-W`, worktree `worktrees/lanes/w62-W`, base master `0d9b2d52e`.
One file touched: `projects/mtg/src/AIPlayerGPT.cpp`. Every change is a RENDER change — no
engine behaviour, no legality, no window, no cache — so the gate is PARSETEST plus a
no-regression suite; no suite fixture is owed and none was added.

---

## What shipped

### D1 — the enters-tapped classifier reads all three script idioms, and RESOLVES the gate

`landEntersTappedTag` decided "unconditional" from the `tap(noevent)` line alone and tested only
`aslongas` / `if(` / `restriction` / `choice` in front of it. 131 of 172 `Play <land>` rows
printed `enters TAPPED - it makes no mana this turn` while quoting the card's own "unless you
control …" sentence (engine-seat HIGH-1; deck125 HIGH-1; deck152 HIGH-1).

Replaced by a scan → gate → resolve → render pipeline, all of it pure except the battlefield read:

* `landTapGateScan(script)` walks every line and returns a `LandTapGate` (found / conditional /
  kind / subtype list / comparator+threshold). Three idioms are now classified:
  1. **same line** — `aslongas(<sel>|myBattlefield) tap(noevent) <1|>N oneshot`
     (Isolated Chapel mtg.txt:59411, Seachrome Coast 101153). Already conditional in wave 61;
     now also *parsed* into a countable gate.
  2. **next line** — a bare `tap(noevent)` whose condition is the script's *other* line,
     `aslongas(plains,island|myBattlefield) untap` (Glacial Fortress mtg.txt:46617-8, Drowned
     Catacomb 32505-6, Sunpetal Grove 115716-7). **70 of the false rows.** A bare tap is no
     longer read alone when that untap line is present.
  3. **`if <expr> then tap(noevent)`** with no paren after the `if` — the whole family wave 61
     could not see (Deserted Beach borderline.txt:27515, Overgrown Farmland 81143, Lair of the
     Hydra 64291, Hive of the Eye Tyrant 53392). **56 of the false rows.** `type(other
     land|mybattlefield)~lessthan~N` and `compare(type:land:myBattlefield)~morethan~N` are both
     parsed; the `compare` form counts the ENTERING land too, so its threshold is lowered by one
     against the count the row can see (the land is still in hand when the row renders).
* `landTapResolve(gate, myLands, witness, &evidence)` (pure) decides the gate against the
  pilot's own battlefield — the same battlefield the prompt printed a few lines above the row —
  and returns untapped / tapped / undecidable plus the evidence phrase.
* `landEntersTappedTagResolved(land, me)` is the only impure part: it counts my lands and finds
  a witness permanent per subtype using the engine's own type ids
  (`MTGAllCards::findType(name, /*forceAdd*/false)` + `hasType`, so an unrecognised selector can
  never grow the global type list). This is what deck123 MED-1 asked for: `Underground Sea` is
  `subtype=Island Swamp`, so Isolated Chapel resolves TRUE on a board with no card *named*
  Plains or Swamp.
* Rendered shapes (nothing deleted — the card's own sentence is still quoted in every branch
  that has one, so verdict and sentence can be checked against each other on the row):
  * resolved untapped — `[enters UNTAPPED - it makes mana this turn (you control Underground Sea, a Swamp): "Isolated Chapel enters tapped unless you control a Plains or Swamp."]`
  * resolved tapped — `[enters TAPPED - it makes no mana this turn (you control no Plains and no Swamp): "…"]`
  * unresolved conditional — the wave-61 hedge, byte for byte
  * unconditional — the wave-61 verdict, byte for byte (Arcane Sanctum is pinned to its exact string)

**Fails closed.** Any gate this code cannot count EXACTLY stays hedged, never resolved:
hand-gated reveals (`|myhand` — Shineshadow Snarl and the 15 other Snarl-shaped cards),
bracketed selectors (`land[basic]`, `other land[forest]`, `creature[legendary]`,
`*[plains;swamp]`), and `choice`-branch shocklands (Blood Crypt). A primitive census of the 472
land cards carrying `tap(noevent)` puts the newly-resolvable population at 27 same-line
aslongas + 15 `if…then` battlefield forms + 10 conditional-untap checklands; the remaining
conditional shapes hedge exactly as they did in wave 61.

### D15 — MDFC land faces stop contradicting the header, and stay visible in the hand line

* `landDropStatusLine`'s available branch no longer says the land drop `is never listed in a
  casting, ability or target menu` — that sentence was false in the very prompt that carried it
  (146v130 seq 7 had `Pelakka Caverns with Pelakka Predation -> PLAY THIS AS A LAND … it USES
  YOUR LAND DROP` as row 1 of the same menu; same shape at 146v123 s24, 146v162 s21). The true
  half is kept verbatim in meaning ("the absence of a land from the choices below does not mean
  the drop is gone") and the one real exception is now NAMED, with the row's own marker words
  so the pilot can recognise it: `PLAY THIS AS A LAND and USES YOUR LAND DROP - taking that row
  spends this same drop`.
* `mdfcHandLandFaceTag` (pure) puts the land face back on the in-game hand line, which had been
  dropping a fact the PRE-GAME header states well:
  `Pelakka Predation {2}{b} [sorcery] [cannot pay now: …] [also a LAND: you may play it as
  Pelakka Caverns, which taps for {B} - that uses your land drop and this face can no longer be
  cast]`. Built from the same `mdfcHandBackLandName` + `landTapMana` pair the pregame note uses,
  so the two cannot disagree. It is a bracket, so it strips out of narration history like every
  other decision-time annotation (pinned).

### D17 — one land-drop wording, and the repeat row states the rule that refuses it

* `landDropAskText(n)` and `kLandDropDeclineRow` replace the arity branch. Both arities now ask
  `Land drop: which land do you play now, if any?` over a decline row `Play no land right now`.
  The one-land variant used to ask `Land drop: play <land> now?` over `Hold <land> - do not play
  it now` — and five deployed guides (deck123, 125, 126, 152, 162) quote the *many-land* decline
  verbatim as the answer never to give, so the variant they cannot name is the one the seat took
  (123v162 seq 43; both of deck123's land declines are on that shape). Nothing is removed: the
  land is still named on its own Play row directly above the decline.
* `repeatRowLine` now states, inside its own bracket, the rule the engine enforces:
  `a count on this row REQUIRES a PLAN line stating your stop count, the count you are at now,
  and how many you perform this window - a count with no PLAN line is refused and re-asked`.
  The `plan_missing` refusal is real and well-defined in code (`repeatRowTaken && namedCount >= 1
  && !replyHasPlanLine`), which is why 123v126 seq 18 (a pass) was accepted and seq 21 (a counted
  repeat take) was refused three windows later — but the rule was, in the reviewer's words,
  "nowhere on the page". It is now on the row that enforces it. Wording matches the re-ask text
  the pilot gets when it trips, so the two cannot drift. No behaviour changed.

---

## Gate

Detached unit `w62-W-gate`, memory-capped (`MemoryMax=4G`, `MemorySwapMax=0`), foreground
until-loop wait; build `rm -f bin/wagic && make -f Makefile.sdl -j4`, clean link.

| leg | result |
|---|---|
| suite, `WAGIC_TESTSUITE_THREADS=1 WAGIC_FASTCLOCK=0.1` | **1249 tests (0 failed), 65 AI tests (0 failed)** |
| `==Test Failed !==` / `==Test timed out` | **0 / 0** |
| PARSETEST `WAGIC_GPT_PARSETEST=1` | **3951 passed, 0 failed** |
| PARSETEST delta | base 3915 → 3951 = **+36**, exactly the 36 `CHECK(` lines this diff adds |
| encoding | `git diff \| grep -c $'\357\277\275'` = **0** |
| memory | no run killed under the 4G cap |

Baseline for the lane's base commit is the brief's own: PARSETEST 3915/0, suite 1249 with 2
concurrency-only failures at THREADS=16 and 0 at THREADS=1. This gate ran THREADS=1 (ground
truth) and is 0 failed there, so no new failure and no regression.

PARSETEST cases added (all tagged `#W62-W`): one positive per D1 idiom plus a resolved-TRUE and
a resolved-FALSE for each of the two resolvable gate kinds; must-NOT-match negatives for Bant
Panorama (with a board in hand, which is the new failure mode), a basic Mountain, an
unconditional Arcane Sanctum pinned to its exact wave-61 string, a hand-gated Snarl and a
bracketed selector (both must stay hedged); the echo shape of the resolved bracket (it binds
`CHOICE: 1 (Play Isolated Chapel)` and strips to `Play Isolated Chapel`); D15's tag positive,
its two negatives and its echo, plus the header's must-NOT-match on `never listed in a casting`;
D17's two-arity equality, the decline-row string, the Hold must-NOT-match, and the repeat row's
new clause with its echo.

---

## Predictions for the wave-62 corpus (falsifiable, per item)

**D1.** Of the `Play <land>` rows whose card carries a battlefield-decidable gate (the checkland,
`other land` count and `compare(type:land)` families), **0** will read
`enters TAPPED - it makes no mana this turn` on a prompt whose own `Your battlefield` line
satisfies the quoted condition, and **0** will read `enters UNTAPPED` on a prompt whose
battlefield line does not. Falsifier: any single row of either shape. Secondary: the wave-61
count of 131-of-172 rows asserting an unconditional verdict on a conditional land drops to
**0**; every remaining `enters TAPPED - it makes no mana` row belongs to a card whose primitive
has an ungated own-ETB `tap(noevent)` (the Arcane Sanctum / Dismal Backwater / Forgotten Cave
class, 41 rows in wave 61). Third: every `enters UNTAPPED` row carries a parenthetical naming
either a witness permanent that appears on the same prompt's own battlefield line, or an
other-land count equal to the number of land entries on that line.

**D15.** **0** prompts will carry the string `never listed in a casting`, and every prompt whose
priority menu contains a `PLAY THIS AS A LAND` row and whose land-drop status line is the
available branch will carry the exception clause. Every hand line naming a card with an MDFC
land back face will carry `[also a LAND: you may play it as …]`; falsifier is one such hand
entry without it (wave 61: all of them).

**D17.** **0** prompts will carry `Land drop: play ` or a land-drop decline row beginning
`Hold `; every land-drop ask, at any arity, reads `Land drop: which land do you play now, if
any?` with `Play no land right now` last. And: no `plan_missing` fallback will be recorded on a
window whose repeat row was rendered by this build — the rule is now printed on the row. That
last one is the weakest of the three (the pilot may still ignore a printed rule); the honest
falsifier is a `plan_missing` rate that does not fall from wave 61's 1-per-deck-123-corpus.

---

## What I did NOT verify

* **No live probe was run.** Nothing here was observed in a real prompt: the whole lane is
  proved by PARSETEST over pure functions plus the suite's no-regression. `landEntersTappedTagResolved`
  — the only impure function added — has NO test coverage of its battlefield walk (the suite
  never opens a GPT land-drop ask). Specifically unverified at runtime: that
  `MTGAllCards::findType(name, false)` + `hasType` matches a dual land's subtype the way
  `hasSubtype("plains")` does in `landColorFlags`, and that `game->inPlay` land counting matches
  what the prompt's own battlefield line prints. Both are read off the same structures the
  render already uses, but "same structure" is not "same number" — the first corpus prompt with
  a checkland is the real gate, and the third D1 prediction above is written so a reviewer can
  cross-tab it without hand-tracing.
* **RED-on-base was established by source reading and the wave-61 corpus census, not by running
  the new cases against the base binary** (the base binary does not contain them). The base
  classifier's `if(`-only test and its bare-tap-wins rule are visible in the wave-61 source; the
  corpus figure is engine-seat HIGH-1's 131/172 with per-idiom counts 70 / 56 / 5.
* **Oracle text was verified against the primitives' own `text=` lines in this worktree, not
  against Scryfall.** Glacial Fortress, Drowned Catacomb, Sunpetal Grove, Isolated Chapel,
  Seachrome Coast, Deserted Beach, Overgrown Farmland, Lair of the Hydra, Hive of the Eye
  Tyrant, Shineshadow Snarl, Bant Panorama and Arcane Sanctum all read as modern Oracle wording
  and each matches the `auto=` semantics as parsed here, but no external lookup was made.
* **The `compare(type:land:myBattlefield)` off-by-one is reasoned, not observed.** It rests on
  the land being in HAND when the row renders and on the battlefield already containing itself
  when the ETB gate evaluates. The deck152 G1 seq 4 repro (one other land out, Lair entered
  untapped) is consistent with it and is the only live data point; the boundary at exactly two
  other lands is untested against the engine.
* **Not touched, and still open from the cited reviews:** deck146 MED-3 (the Pathway land drop
  is asked twice and the colour is invisible in the first ask) — it is a land-drop item and it
  is not in this lane's D-list; deck146 MED-1/MED-2 (ATTACK TOTAL) belong to lane X; the
  reviewer's second half of deck123 MED-3 ("why is the plan block dropped inside a repeat chain
  at all") is a prompt-assembly question this lane did not open.
* **No guide, no Res asset and no `_cards.dat` was edited**, and no primitive was changed — the
  deck reviewer owns the guides, and five of them quote the decline row this lane standardised
  ON, not away from.
