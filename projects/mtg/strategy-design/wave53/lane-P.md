# Wave-53 lane P — render/state (D4, D7, D8, D11, D14)

Base: master `3dfd77d65`. Baseline re-confirmed on this worktree before any edit:
PARSETEST **2121/0**; suite **1210 (2 failed — lifeline.txt + merrow_reejerey.txt, the known
concurrency-only solo-green pair) + 44 AI (0 failed), 0 timeouts**.

Final gate on this branch: PARSETEST **2159/0** (+38 cases); suite **1211 (2 failed — the same
pair, by name) + 45 AI (0 failed), 0 timeouts**. Memory cap held on every run (no kills).

## What changed

### D4 (HIGH) — the stack line names the granted ability's source and victim; the edict row says a sacrifice is already aimed
`src/AIPlayerGPT.cpp`

Two mechanism findings this lane pinned on disk, neither of which the docket could have known:

1. `StackAbility` never sets `Interruptible::source` (only `MTGAbility::source` is set), so
   `stackAbilityName(srcName, menu)` fell to its anonymous menu-text branch on **every** ability
   line in the stack block — the "ability from X" branch was dead in live play.
2. An ability GRANTED to a player (`ability$!...!$ opponent`, the Soul Shatter shape) is parsed
   onto a **nameless dummy card** (`ATargetedAbilityCreator::resolve`), whose real granter is
   reachable only through `MTGCardInstance::storedSourceCard` — the same indirection
   `MTGAbility.cpp` already applies to token creation and mana production inside that keyword.
   Without it the grantor still reads empty and the line still falls back.
3. A third trap on the victim: `MTGAbility::target` is initialised to the ability's own SOURCE
   and is written only at RESOLVE time (`TargetAbility::resolve`), so on an unresolved object it
   names the dummy. The real pick sits in the ability's own target CHOOSER — exactly where
   `ActivatedAbility::activateAbility` reads it to announce the activation.

New: `stackAbilityLine()` (pure, four strings) and `stackAbilityPick()` (the chooser-first pick
resolver), shared by the prompt emitter and the suite register through `stackAbilityBody()`.
Line now reads
`ability: Soul Shatter's sacrifice a creature or planeswalker (aimed at Serra Angel) [from your Soul Shatter]`.
With no grantor it renders **byte-identical** to the pre-fix forms.

(b) `edictAlreadyOnStackClause()` + `edictVictimAlreadyOnStack()`: the highest-MV edict's
`{right now: ...}` clause gains
`- a sacrifice is already on the stack aimed at this permanent; this one would find their next-highest`
when an unresolved stack object's pick is one of the permanents tied at the top mana value.
Pointer identity against the pick, never a name match. No window removed, no cast capped.

### D7 (MED) — `#a-#b`/`xN` collapse reaches ABILITY/target menus
`src/AIPlayerGPT.cpp` (`AIPlayerGPT::askModel`)

`askModel` reached `joinNumberedRows` but never `groupNumberedRows`, so its collapse was
ADJACENCY-ONLY — and the engine orders these lists lexicographically on the target name
(`#1, #10, #11, ... #2, #3`), the order in which consecutive ranks almost never sit together.
The priority menu has gathered repeated rows since wave 48; this is the same gather one seam
later. The permutation is applied to a LOCAL copy of the rows and to the caller-supplied
per-option narration; the chosen number is mapped straight back to the caller's index before it
is returned **or cached**, so every caller's index -> action mapping is unchanged. The parser
still accepts any handle in the printed range (PARSETEST pins the first, an interior and the
last handle, plus the map-back).

### D8 (MED) — the `{castable from exile}` cause is stamped on the card
`include/MTGCardInstance.h`, `src/MTGCardInstance.cpp`, `src/AllAbilities.cpp`, `src/AIPlayerGPT.cpp`

New per-instance `exileCastGrantName` / `exileCastGrantControllerId`, written in
`ATransformer::addToGame` when the transform grants `canplayfromexile` (both spellings: the
basic-ability list and the `newability[...]` payload Elite Spellbinder actually uses), and
carried across zone-move instance rebuilds (`MTGCardInstance::clone`, `::copy`).
`exileCastNote` reads the stamp FIRST and keeps the old battlefield scan as the fallback.

### D11 (MED) — `[second copy:` appends the usefulness verdict, decided by the SCRIPT
`src/AIPlayerGPT.cpp`

`autoLineStacks()` / `splitAutoLines()` / `secondCopyVerdict()` classify each `auto=` line of the
card's own script. A line does NOT stack when it is a `lord(...)` continuous effect (tested
FIRST — a lord payload can carry a nested `newability[{t}:add{G}]` whose cost is the GRANTEE's)
or when it carries no activation cost and no countable payload (Intruder Alarm's two idempotent
`untap all(creature)` triggers). Three verdicts:
* no stacking line -> `..., but its effect is already on the battlefield and a second copy changes nothing`
* a `lord(...)` line plus stacking lines -> `..., but the effect it gives your OTHER permanents is already on - this copy adds only its own abilities`
* otherwise -> the wave-52 wording, byte-identical.
Verified against the primitives with `/usr/bin/grep`: Intruder Alarm (mtg.txt:58850) dead,
Chromatic Lantern (19630) partial, Talisman of Impulse (117529) / Staff of Nin (112425) /
Howling Mine (55935) / Exquisite Blood (37966) / Ranger Class (borderline.txt:89046) unchanged.

### D14 (MED) — the battlefield header counts lands
`src/AIPlayerGPT.cpp`

`battlefieldHeaderText` takes `lands` (default -1 = omit, so the pre-fix render is byte-identical)
and closes the sentence with `and L are lands` — after the creature-scoped attack clauses, so
their "of them" cannot be re-scoped. Both call sites count `TYPE_LAND` in the same loop that
counts creatures.

## Suite pins (RED on base, GREEN after — both verified by rebuilding with the two fixes reverted)

Both fixtures need a render surface the suite could not otherwise see (it asserts ZONES, and both
defects live entirely in the words over an unchanged zone layout). `TestSuiteAI::Act` now records
two PRODUCTION-emitter registers into the existing `mNarrationLog`, deduped against their last
value and skipped when empty:
* `stackAbilityRegister()` — one line per unresolved ability on the stack, in the stack block's
  own wording (`stackAbilityBody`, the same function the prompt calls).
* `exileCastRegister()` — the `exileCastNote` clause for each stamped card in the seat's exile,
  prefixed with whether ANY granter is still in play (the pre-fix derivation itself), so the
  assertion is "the cause is named while that scan reads no".

`bin/Res/test/w53_stack_ability_names_source_and_victim.txt` (AI test) — pre-fix log reads
`on the stack: ability: sacrifice a creature or planeswalker`; post-fix
`ability: Soul Shatter's sacrifice a creature or planeswalker (aimed at Serra Angel) [from their Soul Shatter]`.
`bin/Res/test/w53_exile_cast_cause_survives_granter.txt` — pre-fix
`in exile [granter in play: no]: shock {castable from exile - a legal cast, not a dead row...}`;
post-fix the same line names `exiled by their Elite Spellbinder`.
Both appended to `bin/Res/test/_tests.txt`.

## Falsifiable predictions

* **D4** — In the next corpus, stack-block lines reading a bare `ability: <effect>` with no source:
  **0/N**. Edict cast rows whose `{right now:}` names a permanent already picked by an unresolved
  stack object, with no such-clause: **0/N** (was 2/3 in one window).
* **D7** — No `ask`-kind menu with > 12 printed rows whose bodies mask (over `#N`) to <= 2 shapes;
  the count of `ask` records whose printed row count exceeds their distinct-shape count by more
  than 12 falls to **0**. Max non-log prompt segment stays **< 12 KB**.
* **D8** — `{castable from exile}` naming the exiling source: **17/17** on the same shape (was 4/17);
  no record in which the same exiled card names its cause in one window and not in a later one.
* **D11** — `[second copy:` takes on a card whose script has no stacking term: **0** (was 4/14 at
  Intruder Alarm). Staff of Nin / Talisman of Impulse / Ranger Class take counts unchanged, and
  Chromatic Lantern is never called dead.
* **D14** — Every battlefield header carries `and L are lands`; deck130's land-destruction casts
  past its own four-land gate fall from 8/19.

## What I did NOT verify

* **No live model probe was run.** Every prediction above is about the NEXT corpus; nothing here
  was measured against a model this lane. The engine-side behaviour is pinned by PARSETEST + suite
  only.
* **D4's caster-chair wording (`[from your Soul Shatter]`) is pinned only in PARSETEST.** In the
  suite fixture the seat whose `Act` observes the unresolved ability is the one holding priority
  (P2), so the fixture pins the `their` form. I did not find a fixture shape that puts the CASTER's
  `Act` inside that window.
* **D4b's such-clause has no suite fixture.** Its trigger (`edictVictimAlreadyOnStack`) shares the
  `stackAbilityPick` resolver that the D4a fixture does exercise, but the two-copies-in-one-window
  board was not reproduced in a fixture; the clause itself is PARSETEST-pure.
* **D7's permutation is proven index-safe by construction and by PARSETEST**, and the full suite is
  green, but the suite drives few `askModel` menus wide enough to permute — I did not observe a
  live wide ability menu being answered through the mapped-back index.
* **D8's stamp is applied in `ATransformer::addToGame` only.** Any other engine path that sets
  `CANPLAYFROMEXILE` (a direct `basicAbilities.set`, a foretell/adventure route) still falls back
  to the battlefield scan. I did not audit every such path.
* **D11's `autoLineStacks` was calibrated against the seven cards the docket names** plus the
  scripts I read. Cards whose redundancy is semantic rather than structural (two different lords
  that happen to overlap; a trigger whose payload is countable but capped) are not classified.
* The `[granter in play: yes|no]` prefix exists only in the test-suite register, never in a prompt.
