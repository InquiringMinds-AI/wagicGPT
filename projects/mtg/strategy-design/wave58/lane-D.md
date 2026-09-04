# Wave-58 lane D — D45, the lower-cased script token in narration

Base: master `6976b839a`. Worktree `worktrees/lanes/w58-D`, branch `w58-lane-D`.

## What the corpus actually proves

The item is D45's narration half (wave-57 ledger; the wave-56 D28 row it discharges reads
"PASS on `chosen_text` (0), FAIL on narration (698 occurrences / 422 prompts)").

I characterised the emitters from the wave-57 corpus
(`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260903-174505/`, read-only, 64 translogs).

**Every occurrence of the seven named tokens sits in ONE emitter shape.** A census of the
context around each of `boulderloft` / `tidechannel` / `grimclimb` / `agadeem` / `pelakka` /
`emeria` / `skyclave`:

| context | occurrences |
|---|---|
| `- You used: <token> with <Card>` / `- Opponent used: <token> with <Card>` | 708 |
| `Stack: N (top): ability: <Card>'s <token> [from your/their <Card>]` | 3 |
| anything else (ARRIVAL, `resolved and entered`, `created ->`, cast lines, `You played`, `You chose`, the board block, the decklist, `chosen_text`) | **0** |

So the brief's other narration emitters are already clean: they take their name from a live
`MTGCardInstance::getDisplayName()`, not from a script token. The one seam that does not is the
ABILITY LABEL, and it has exactly two consumers.

**The discriminator, measured rather than assumed.** I took every distinct `used:` label in the
corpus (171 distinct labels) and intersected the lower-case-initial ones with the set of `name=`
entries in `bin/Res/sets/primitives/*.txt`:

- **6 labels / 708 occurrences ARE card names** — `boulderloft pathway` 323, `tidechannel
  pathway` 185, `grimclimb pathway` 85, `agadeem, the undercrypt` 77, `pelakka caverns` 23,
  `emeria, shattered skyclave` 15. These are the defect: MDFC / Pathway back faces.
- **30 labels / 9,261 occurrences are NOT card names** — `cycling`, `choose a land`,
  `sacrifice a creature or planeswalker`, `goblin lair`, `dungeon completed`, … These are
  genuine lower-case English written by the script author and must not move.

That census is what the fix's rule is built on, and it is why the rule is a card-name lookup
rather than a capitalisation.

**Cause.** `MTGDeck.cpp` lower-cases every value it reads out of a primitive, so a card script's
`name(Boulderloft Pathway)` reaches the engine as `boulderloft pathway`. Wave 57 fixed the ROW
head (`mdfcRowHead`, and the `flipStats` lookup at the row's `PLAY THIS AS A LAND` clause) and
`chosen_text` with it; the narration write seam was never given the same lookup.

## What shipped (`projects/mtg/src/AIPlayerGPT.cpp`, one file)

1. `restoreScriptTokenCase(label, printed)` — **pure**, so PARSETEST (which runs before any card
   database exists) proves the whole rule. It substitutes `printed` for `label` only when
   `label` is ALREADY all lower case AND `printed` differs from it by CASE ALONE. Anything else
   — an author-cased label, an empty side, a printed form that is a different string — comes
   back byte-identical.
2. `scriptTokenDisplayCase(label, ctx)` — the impure half: `MTGCollection()->getCardByName(label,
   ctx->setId)`, the SAME lookup the wave-57 row head already uses on the same token, so the
   narration resolves to the printing the row resolved. A label naming no card never survives
   guard 1.
3. Applied at the two seams the corpus proves: the `WEventAbilityActivated` narration
   (`AIPlayerGPT::narrate…`, the `abilityActivationNarration` call — the 708) and
   `stackAbilityBody`'s menu text (the 3).

Nothing is removed: the label still renders, the `with <Card>` clause is untouched, and the
existing W43-LOW guard (a label that IS its own card's name collapses to `an ability`) still
fires after the re-casing, because it compares case-insensitively on both sides.

**Not shipped, deliberately:** the third label consumer, the `TARGET CHOICE` header's
`renderAbilityLabel(waiting->getMenuText())` (~line 26490). It is the same class, but it renders
**0** lower-cased card names in the wave-57 corpus, and the brief forbids widening past what the
finding proves. Recorded here so a later wave does not have to re-find it.

## PARSETEST cases (12 new, all in the `#W58-D (D45)` block)

Positives: the token replaced (`boulderloft pathway` -> `Boulderloft Pathway`), a multi-word
back face with internal lower-case words (`agadeem, the undercrypt` -> `Agadeem, the
Undercrypt`), and the **echo shape at both chairs** — `You used: Boulderloft Pathway with
Branchloft Pathway` / `Opponent used: …`, the same sentence with the subject swapped.
Negatives: a label that names no card (`cycling`; `sacrifice a creature or planeswalker`) is
untouched and still renders verbatim; an author-cased label (`Flip Side`) is never re-cased; a
printed form differing by more than case (`Agadeem's Awakening`) is refused; an empty side of
the pair changes nothing. Plus the two composed invariants: the self-named label still collapses
to `an ability`, and the stack row reads `ability: Branchloft Pathway's Boulderloft Pathway
[from your Branchloft Pathway]`. No new bracketed/braced annotation was added, so there is no
new echo shape beyond these.

## RED / GREEN — same instrument, both arms

`strategy-design/wave58/d45-narration-case-probe.sh` (new, shipped): one stub-server
(`tools/gpt-stub-server.py`, `--prefer Pathway`) self-play game, deck152 vs deck162, no model and
no inference call, under the 4G memory cap. It counts `used:` labels that ARE card names, split
by casing.

- **RED on base.** `src/AIPlayerGPT.cpp` reverted to `6976b839a`, full rebuild, same probe:
  **312 lower-cased / 0 printed — FAIL** (`mistgate pathway` 199, `boulderloft pathway` 82,
  `tidechannel pathway` 31).
- **GREEN after.** **0 lower-cased / 107 printed — PASS** (`Mistgate Pathway` 60,
  `Tidechannel Pathway` 30, `Boulderloft Pathway` 17).

The binary was then rebuilt from the restored source and is **byte-identical** to the one the
gate below ran on.

## Gate (detached, unit `w58-D-gate`, MemoryMax=4G, MemorySwapMax=0)

- **PARSETEST**: `3362 passed, 0 failed` (base 3350/0; +12 = exactly the new cases).
- **Suite**: `1245 tests (2 failed)`, `61 AI tests (0 failed)`, **0 timed out**. The two failures
  are the brief's two known concurrency-only ones, by name: `lifeline.txt`,
  `merrow_reejerey.txt`. **0 new failures.**
- `git diff | grep -c $'\357\277\275'` prints **0** (no non-UTF-8 mangling).

## Falsifiable prediction for the wave-58 corpus

**In the wave-58 corpus, the number of `- You/Opponent used: <label> with <Card>` lines whose
`<label>` is (case-insensitively) a `name=` entry in `bin/Res/sets/primitives/*.txt` AND begins
with a lower-case letter is ZERO** — against 708 in the wave-57 corpus — while the count of the
same lines with the printed casing is > 0 on any matchup containing a Pathway or MDFC deck
(152, 162, 146, 123, 125), and the count of lower-case `used:` labels that are NOT card names is
unchanged in character (wave 57: 30 distinct labels / 9,261 occurrences).

## What I did NOT verify

- **No live-model run.** Both probe arms used the stub server; the model never saw the new
  strings, so nothing is claimed about whether the corrected casing changes a decision. It is a
  render fix, and only the render is measured.
- **No fixture in the suite.** Suite fixtures do not assert translog text, so the RED/GREEN
  above is the probe, not a `.txt` fixture; the probe is not wired into the suite and will not
  run in a future gate unless someone runs it.
- **One game per arm, one matchup (152v162).** The probe's absolute counts are not a
  distribution — 312 vs 0 is a categorical result (zero-vs-nonzero), not a rate.
- **The `setId`-scoped lookup's fallback path is untested in isolation.** Every token in both
  probe arms resolved; a back face whose printing lives only in another set would take
  `getCardByName`'s global fall-through, which I read but did not exercise.
- **The other four items in D45's LOW cluster** (`NO LIVE CAST ROW` verb disagreement,
  multi-select `chosen_text` instance handle, the pregame Pathway mana-source count, the one-row
  menu round trip) are **not touched** — the brief scopes lane D to the narration-case item, and
  the first of those belongs to lanes A and C.
- **Whether any other lower-cased script token exists outside the seven the ledger named.** My
  census enumerated every distinct `used:` label in the corpus and intersected it with the card
  names, which covers that emitter completely; I did not run the same intersection over every
  other narration line in the corpus.
