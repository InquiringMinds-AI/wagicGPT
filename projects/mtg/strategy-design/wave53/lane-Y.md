# Wave-53 lane Y — owner Vita report: Branching Evolution scripted as a trigger, not a replacement

Owner's words (2026-09-01): "one of my enchantments working wrong, letting me add counters to a
target instead of doubling as a replacement effect."

## Oracle vs old script vs new script

**Oracle** (Scryfall `cards/named?exact=Branching Evolution`, fetched 2026-09-02 — `{2}{G}`,
Enchantment): *"If one or more +1/+1 counters would be put on a creature you control, twice that
many +1/+1 counters are put on that creature instead."*
The card's own `text=` line already carried this verbatim; the script did not implement it.

**Old script** (`bin/Res/sets/primitives/borderline.txt`):
```
auto=@counteradded(1/1) from(creature|mybattlefield):target(creature[counter{1/1}]|Battlefield) counter(1/1) notrg
```
Three defects, all of them the owner's symptom:
1. **Wrong shape** — `@counteradded` is a *triggered* ability (`TrCounter`), not a replacement.
   It resolves through the stack after the counter has landed.
2. **Opens a target ask** — the payload is `target(creature[counter{1/1}]|Battlefield)`, i.e. a
   chooser over **either** battlefield. That is the prompt the owner saw on the Vita; the Oracle
   text has no target and no choice at all. In the RED run the stray ask consumed the next script
   command (`RULES: Can't find card:assertpt 4/4 grizzly bears` → `TESTSUITE menu default`).
3. **Wrong amount** — it adds exactly ONE counter per event, so a 2-counter event became 3, not 4.

**New script** (functionally identical twin: Corpsejack Menace, same Oracle):
```
auto=@totalcounteradded(1/1) from(creature|myBattlefield) duplicate(all):ability$!name(Double the counters) donothing!$ controller
```
Grammar confirmed in source, not assumed:
- `MTGAbility.cpp:1856` parses `totalcounteradded(` → `TrTotalCounter`, reading `duplicate(all)`,
  `plus(N)`, `half(all)`, `nocost`, `from(...)`, `except(...)`, `bycontroller`.
- `AllAbilities.h` `TrTotalCounter::triggerOnEventImpl`: the `duplicate` branch adds
  `e->totalamount` further counters — i.e. **doubles the whole event**, which is what
  "twice that many" means; the `plus` branch is the Hardened Scales / Conclave Mentor "that many
  plus one" form. `Counters::addCounter(..., duplicated=true, ...)` suppresses the follow-up
  event, so the doubling cannot re-trigger itself (no loop).
- `from(creature|myBattlefield)` is tested against `e->targetCard` (the creature that RECEIVED the
  counters), which is exactly the Oracle's "a creature you control" scope.
- The payload is the corpus's standard inert rider (`donothing`) — the doubling happens inside the
  trigger, so no target chooser is ever built.

Deliberately NOT copied from Doubling Season: its `nocost` (suppresses counters placed as a cost).
Branching Evolution's Oracle has no cost carve-out, and Corpsejack Menace — the card with the
byte-identical Oracle — does not carry it either. `except(...)` was likewise not added: it tests
the counter's SOURCE and is loop-avoidance for the `(any)`-counter doublers; a `(1/1)`-scoped
doubler with `duplicated=true` suppression does not need it.

## Audit of the same defect class

Swept all five `primitives/*.txt` (515 `@counteradded` lines; 499 in borderline.txt) with two
passes: (a) every card whose `text=` says "instead" together with replacement wording
("would be put/placed", "twice that many", "that many plus"); (b) every `@counteradded` line whose
payload is `target(...) ... counter(...)` — the exact broken shape.

**Changed: Branching Evolution only.** It is the sole card in the corpus whose Oracle is a
counter *replacement* and whose script is a `@counteradded`-plus-target *trigger*.

**Not changed, with reason:**
- **The Great Work** — hit pass (a), but its "instead" is about a *spell* going to a graveyard.
  Its `@counteradded(0/0,1,Lore)` lines are genuine Saga lore-counter triggers. Correct as is.
- **Twelve Sagas** (Ajani Fells the Godsire, Arni Slays the Troll, Fall of Gil-galad, Fall of the
  Impostor, Jugan Defends the Temple, Revival of the Ancestors, Showdown of the Skalds, Teachings
  of the Kirin, The First Iroan Games, The Grand Evolution, Triumph of Gerrard, Welcome to
  Sweettooth) — hit pass (b). Every one is "chapter N: put counters on **target** creature"; the
  target ask is the printed Oracle. Correct as is.
- **Aragorn, Company Leader** — `@counteradded` with a target, but its Oracle really is "Whenever
  you put one or more counters on Aragorn, put one of each of those kinds..." — a trigger. Left.
- **Token doublers** (Doubling Season's token half, Adrix and Nev, Mondrak, Elspeth Storm Slayer,
  Kaya Geist Hunter) — same "twice that many ... instead" wording, but the *token* mechanism
  (`@tokencreated ... clone`), not counters. Out of class, left alone.
- **Devour cards** (Devouring Hellion, Feaster of Fools, Marrow Chomper, Mycoloth, Predator
  Dragon, Preyseizer Dragon, Tar Fiend, Thorn-Thrash Viashino, Dragon Broodmother) and
  **Galloping Lizrog** — "twice that many" is a one-shot ETB computation, not a replacement of an
  incoming counter event. Correct shape already.
- **unsupported.txt** (Lae'zel Vlaakith's Champion, Pir Imaginative Rascal, Selesnya Loft Gardens)
  — these are in the unimplemented file, out of scope for this lane; Lae'zel and Pir would each
  need a `plus(1)`-family script over `(any)` counters if ever promoted. Noted, not touched.
- Existing correct doublers/adders left untouched as the reference idiom: Corpsejack Menace and
  Vorinclex (`duplicate(all)`), Hardened Scales and Conclave Mentor (`plus(1)`), Doubling Season.

## Suite pins (registered in `bin/Res/test/_tests.txt`, `git add -f`)

Two test-only witnesses were appended to `bin/Res/test/lexicon/test_primitives.txt` (loaded by
default for the whole suite): `Lexicon Any Counter Witness` (id 2100000335,
`{0}:counter(1/1) target(creature|battlefield)`) and `Lexicon Two Counter Witness`
(id 2100000551, `{0}:counter(1/1,2) target(creature|battlefield)`) — the existing
`Lexicon Counter Witness` can only target its controller's side, which cannot express the
negative case.

| fixture | asserts | base | after |
|---|---|---|---|
| `branching_evolution_doubles_one_counter.txt` | your 2/2 gets 1 counter → **4/4**; opponent's Runeclaw Bear stays 2/2 | **RED** (got 3/3) | GREEN |
| `branching_evolution_doubles_two_counters.txt` | your 2/2 gets a single 2-counter event → **6/6** | **RED** (got 4/4) | GREEN |
| `branching_evolution_opponent_creature_not_doubled.txt` | counter on the OPPONENT's 2/2 → 3/3, not 4/4; your Bears untouched | GREEN (see note) | GREEN |

Note on the negative: it is green on base as well, because the old script's `from(...)` filter also
rejected an opponent's creature. It is a *scope* pin against an over-broad fix (a naive
`from(creature|battlefield)`), not a RED-on-base reproduction. The RED evidence for the owner's
report is the pair above, plus the base run's stray target prompt swallowing a script command.

## Gate (worktree w53-Y, no rebuild — card script and fixtures are data)

- Suite: **1219 tests (2 failed), 47 AI tests (0 failed), 0 timeouts.** The 2 are the known
  concurrency-only pair `lifeline.txt` + `merrow_reejerey.txt` (identified by name in the log);
  1216 baseline + 3 new fixtures = 1219.
- PARSETEST: **2258 passed, 0 failed** (baseline).
- `WAGIC_VALIDATE=1`: **0 warnings**.
- Byte-splice discipline: all three files edited via python rb/wb;
  `git diff | grep -c $'\357\277\275'` = **0**.

## Not verified

- **No Vita/PSP rebuild and no on-device run.** The fix is data only (`bin/Res`), so no engine
  rebuild was needed, but the owner's actual handheld has not been re-tested.
- **No engine change was made or needed** — `duplicate(all)` already existed and is exercised by
  Corpsejack Menace and Vorinclex.
- **Interaction stacking not pinned**: two counter-doublers out at once (Branching Evolution +
  Corpsejack Menace = 4x), or a doubler plus a `plus(1)` card (Hardened Scales), is not covered by
  a fixture. The engine applies each trigger to its own event, which is the rules-correct
  behaviour, but that is reasoning, not a measured result.
- **Counters placed as a cost** are doubled by this script (no `nocost`). That matches the Oracle
  and matches Corpsejack Menace, but no fixture exercises a counter-paying cost with the
  enchantment out.
- **`-1/-1` and other counter types** are out of scope by construction (`(1/1)` filter); not pinned.
- `unsupported.txt` cards in the same class were catalogued, not implemented or tested.
