# Wave-54 lane E — combat / mulligan / land render

Branch `w54-lane-E`, worktree `worktrees/lanes/w54-E`, base master 42b3bf5da.
Docket: D21, D17, D16, D20, D19 (wave53/engine-ledger.md), = R181/R177/R176/R180/R179.
All five are RENDER changes in `src/AIPlayerGPT.cpp`; no rules/engine behaviour moved, so
the gate is PARSETEST (positive + must-NOT-match negative + echo shape per item) and the
suite is the no-regression check.

## Gate numbers (this worktree)

| | base (seeded bin/wagic, 42b3bf5da) | after |
|---|---|---|
| PARSETEST | 2258 passed, 0 failed | **2290 passed, 0 failed** (+32) |
| suite | 1219 tests, **2 failed** (`lifeline.txt`, `merrow_reejerey.txt`), 49 AI / 0, 0 timed out | 1219 tests, **1 failed** (`merrow_reejerey.txt`), 49 AI / 0, 0 timed out |

The single residual failure is one of the known concurrency-only pair: re-run scoped and
single-threaded (`WAGIC_TESTSUITE_THREADS=1`, registry `test/_w54e_probe.txt`, deleted after)
`merrow_reejerey.txt` + `lifeline.txt` = **2 tests, 0 failed**. Base and after were both built
and run under the memory cap; logs `~/.gatelogs/w54-E-base-*.log`, `~/.gatelogs/w54-E-*.log`.
Build: incremental `make -f Makefile.sdl -j4`, clean link. `git diff | grep -c U+FFFD` = 0.

## What shipped

### D21 — the corpus's only render FALSEHOOD (`combatDamageForecast`, tag `#W54-E (D21)`)
`combatDamageForecast` gained a REQUIRED `oppLife` argument (deliberately not defaulted — a
forecast that cannot see the opponent's life must not be able to fall back into printing the
claim). The non-lethal branch now prints
`- NOT lethal: block only where the trade favors you; taking damage while ahead on LIFE is
often correct (your strategy guide's blocking rules override this general hint).`
only when `life > oppLife`, and otherwise
`- NOT lethal: block only where the trade favors you.`
The lethal branch, the poison branches and the ahead-branch wording are byte-identical to the
shipped strings. The caller passes `opponent()->life`.
**Prediction (falsifiable):** in the wave-54 corpus, blockers records whose `my_life <= opp_life`
carrying `ahead on LIFE`: **0** (was 1 of 1 such window). Records with `my_life > opp_life`
still carry it verbatim, so the string does not simply vanish from the corpus.

### D17 — the opposite number (`attackerBlockerCountLine`, emitted in `chooseAttackers`)
One header line after the A-rows, from `canBlock()` over the opponent's battlefield creatures —
the same solo gate the per-attacker `their untapped blockers` lists are filtered by, so header
and rows cannot disagree:
`They have N untapped creatures able to block; declaring more than N attackers leaves at least
(your attackers - N) of them unblocked.`
N == 0 takes a positive branch instead of printing a 0-subtraction:
`They have 0 untapped creatures able to block: every attacker you declare this turn is
unblocked unless something changes first.`
The subtraction is left as an expression because the attacker count is the model's to choose.
**Prediction:** attackers prompts with no blocker-count line: **0**.

### D16 — the mulligan coverage cause (`mulliganNoCoverCause`, in `pregameHandHeaderText`)
`Playing every land in this hand would not cover any spell in it` now continues with which of
the two hands it is, from the tallies the function already takes (still strictly hand-derived —
no board state joins the pregame surface):
- `lands < cheapestCmc` → ` (1 land is not enough for your cheapest spell at mana value 2)`
- otherwise → ` (you have 2 lands; no spell in it is castable off {G} alone)` — the colours in
  the same uppercase symbols the `Mana sources among those lands` line two rows up uses;
  `colourless mana` when the lands make no colour.
- `cheapestCmc < 0` (an all-land hand) → ` (this hand holds no spells at all)`.
The colour branch is a sound inference, not a guess: `reachable` is empty only when no spell
passes both the count test and the per-pip test, so a hand whose count reaches the cheapest
spell can have failed only on a coloured pip.
**Prediction:** `would not cover any spell` rendering with no cause clause: **0**.

### D20 — fetch rows name their colours (`fetchLandColorsClause` + `fetchMakesNoManaClause`)
The braced clause continues inside the same braces:
`{this land makes no mana - crack it for a land: your untapped mana sources go from 2 to 3,
and it adds {W} or {U} (you cannot make {U} right now)}`
Colours are read off the row's OWN `action.target` with `landColorFlags` (the same reader the
hand line's `(land: taps for {W}{U})` uses); the cannot-make half is measured against
`ManaEngine::potentialColorReach`, the same potential the `colours you can make` mana line is
built from — so no third opinion about either fact can enter. Two missing colours share one
parenthetical. A land whose colours the reader returns empty claims NOTHING (deliberate
abstention: an unread script and a genuinely colourless land are indistinguishable here, and a
wrong colour claim on this menu is the defect being fixed).
**Prediction:** fetch target rows naming a land and no colours: **0** (was 90 at that seat) —
except rows whose target land yields no colour flags at all, which stay silent by design.

### D19 — never the source name twice (`stackAbilityLine` / `stackAbilityBody`)
When the rendered effect label is (case-insensitively) the grantor's own name — the
`ability: Ob Nixilis, the Hate-Twisted's Ob Nixilis, the Hate-Twisted` shape, 53 wave-53 lines —
the label is replaced by a kind the CALLER read off the live object: `triggered ability`
(`dynamic_cast<TriggeredAbility*>`), `activated ability` (`ActivatedAbility*`), else the neutral
`ability`. The kind is passed in so `stackAbilityLine` stays pure and never asserts a trigger it
did not observe. Lane P's shipped form for abilities with a real label is byte-identical.
**Prediction:** stack lines whose effect label equals the source name: **0**; the 706-line,
0-bare shape of lane P's D4 is otherwise unchanged.

## PARSETEST additions (+32, one block `[#W54-E]` at the corpus end)
Positives, must-NOT-match negatives and echo shapes for all five, including: the exact D21
behind/level/one-ahead strings and the untouched lethal branch; D17 singular/zero branches and
the zero branch carrying no subtraction; D16 count-vs-colour cross negatives, the covered-hand
negative and a no-board-state negative; D20 the byte-identical no-clause form, the
already-makeable negative, the unread-colours negative and the two-missing-colours list; D19
the case-insensitive match, the unknown-kind degrade, an exactly-twice name count and the
real-label negative. Echo shapes assert the new clauses open no `[`, that an echoed fetch row
still strips to its land name, and that the D17 header leads with prose.
Seven pre-existing call sites were updated for the two changed signatures
(`combatDamageForecast` +oppLife: the CONTROL check now names an opponent at 15 so its
byte-identical assertion still holds; `fetchMakesNoManaClause` +colorsClause: `""`).

## What I did NOT verify
- **No live game.** Nothing here was observed in a corpus or a probe run — every claim above is
  from the pure functions plus a code reading of the emitters. All five predictions are for the
  wave-54 corpus to settle; a lane gate is a hypothesis, not a closure.
- **D17's N is `canBlock()`-only**, i.e. it counts creatures that can block *something*. A
  creature that can block but is legally unable to block *any* declared attacker (e.g. a
  no-flying blocker against an all-flying attack) is still counted, so N is an upper bound on
  blockers relevant to a given attack. The per-attacker rows carry the pairwise truth; I did not
  add a second, attack-specific count and did not measure how often the two diverge.
- **D20's `entersTapped` detector is unchanged** (the pre-existing `tap(noevent)` / `tapped`
  substring read) — I did not audit its accuracy, only appended to its clause. I also did not
  check the added `potentialColorReach` call's cost on wide boards; it is one extra call per
  rendered fetch row.
- **D19's kind cast** was not exercised against a live `StackAbility` — the PARSETEST pins the
  string function, and the `dynamic_cast` chain is a code reading of `MTGAbility.h`'s two base
  classes. If a granted ability is neither, the line degrades to `ability`, which is safe but
  uninformative; I did not measure how many of the 53 lines land in each of the three branches.
- **D16's all-land branch** (`cheapestCmc < 0`) is reachable only via a hand of pure lands; not
  observed in any corpus, pinned in PARSETEST only.
- **D21**: I did not check whether any deck guide's blocking prose depends on the removed clause
  being present; the ledger's ask says the guide owns the behind-on-life case.
