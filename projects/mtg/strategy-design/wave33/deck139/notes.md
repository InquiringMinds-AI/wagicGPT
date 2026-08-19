# deck139 -> development notes (engine / harness / render / instrument), wave 33

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260819-070739`, binary `bdb30d8e7`.
All seq references are this seat's translogs (`*ai_baka_deck139-*.jsonl`); wave-32 comparison
numbers are re-measured from `matchups-20260727-115847`, not quoted from prose.

---

## CLOSED THIS WAVE (validated live; remove from the open ledger)

- **N-139n -- the pregame hand-count header.** CLOSED, and it is the strongest single-fix result
  this seat has produced. Mulligans **9 -> 0** in six games; false mulligans **6/9 -> 0/0**; bottom
  asks 3 -> 0; `zero green sources` / `this hand has zero lands` **0 occurrences**. The header is
  arithmetically correct in **6/6** hands, including two hands with exactly ONE green source that
  the wave-32 belief would have thrown away. The `Playing every land in this hand would cover the
  cost of: ...` satisfiability line is the part that closes HL7 rung 3 by construction.
  Seat citations: vs36 s1 (`{G} 3`), vs158 s1 (`{G} 1`, kept), vs116 s1 (`{G} 1`, kept).
  **Untested direction:** no unkeepable hand appeared, so the FALSE-KEEP failure mode of a count
  header is unmeasured at this seat. Record it; do not claim it.
- **N-139o -- poison counters unrendered.** CLOSED. 4/4 infect blocker windows carry
  `Your life: 14. Your poison counters: 3 of 10.`; the board snapshot carries
  `Poison counters (you): 3 of 10 - you LOSE the game at 10 poison counters, whatever the life
  total is; 7 more end it.` from the first accrual on; every accrual narrates
  (`- Poison: you take 1 poison counter - now 2 of 10 ...`); every infect attacker line carries the
  damage-type clause. **`NOT lethal` on an infect window: 0/4.** The three non-infect windows
  elsewhere print the ordinary form unchanged -- negative control clean. The guide stopgap is
  retired per its own removal condition and the seat's replies show zero "still safe at N life"
  reasoning in that game.
- **N-139j (0-power attacker tag)** -- stays closed, 4th corpus: 33 attacker windows, **0**
  declarations of a `[deals 0 ...]` creature.
- **N-139s (all-illegal blocker set exits silently)** -- **no recurrence**;
  `all_assignments_illegal` fired **0** times at this seat; the i8 name-form legs all parse
  (vs36 s21, vs152 s27, vs105 s24).

---

## N-139v (NEW, HIGH, RENDER LANE) -- creature TYPE is a declared primitive field the render never emits, and the pilot filled the gap by INVENTION

**Symptom.** Five replies in three games assert that a deck139 creature is a **Human**, and each
one uses that assertion to refuse an offered `with its mutate cost` line:

- vs36 s22: *"Option 2 (Mutate) is illegal because it requires a non-Human creature on my
  battlefield to target, and **my only creature, Pollywog Symbiote, is a Human**."*
- vs36 s23: *"I have no non-Human creature on my battlefield to serve as a host (**Pollywog
  Symbiote is a Human**). Therefore, the mutate option is not legally available regardless of the
  text on the card."*
- vs158 s24: *"Your only creatures are **Arboreal Grazer (Human)** and **Pollywog Symbiote
  (Human)**, so you cannot mutate Greathorn this turn."*
- vs158 s29: *"I currently have no valid non-Human target on my battlefield (**Migratory Greathorn
  is Human, Pollywog is Human**, lands are not creatures)."*
- vs105 s22: *"you currently have no non-Human creature on the battlefield to mutate onto (**your
  only creature, Pollywog Symbiote, is a Human**)."*

**Ground truth, from `borderline.txt`.** `Pollywog Symbiote subtype=Frog`;
`Arboreal Grazer subtype=Beast`; `Migratory Greathorn subtype=Beast`;
`Dryad of the Ilysian Grove subtype=Nymph`; the four Apexes are
Nightmare/Beast/Elemental/Dinosaur/Cat combinations. **There is no Human in the deck.** And every
mutate card carries `otherrestriction=type(creature[-human]|mybattlefield)~morethan~0`, so the
option list the model is reading is ITSELF proof a legal host exists.

**Why it is a render item and not only a guide item.** `subtype=` is a DECLARED primitive field
that the corpus render emits **zero times in 190 records**. The hand and board lines read
`Pollywog Symbiote {1}{u} (1/3) [creature]` -- type-class only, no creature type. This is the
wave-32 HL9.4 ABSENT-FIELD shape exactly: a field silently absent inside an option that is present,
so there is no wrong value to catch and no counter records anything. The pilot did what the trust
doctrine predicts for a gap -- it confabulated, four different ways, and the confabulation
overrode the ENGINE'S OWN legality decision.

**Cost.** Mutate take-rate **6/7 (86%) in wave 32 -> 1/6 (17%) here**; four of the five declines
rest on this belief. The deck's #1 rule was off in five of six games. The belief has **zero**
instances in the wave-32 corpus.

**PROPOSAL, in priority order.**

1. **Cheapest and sufficient for this seat's class (guide layer, SHIPPED this wave):** restore the
   explicit falsity-fact plus a decision rule that does not DEPEND on the missing field --
   *"if a `with its mutate cost` line is on the list, a legal non-Human host is already on your
   battlefield; the engine does not print that line otherwise."* This routes around the absent
   surface instead of teaching doubt about it. Validate at wave 34; if it fails, escalate to (2).
2. **Render (durable):** print the creature's TYPE on creature lines the same way the land tag is
   printed -- `Pollywog Symbiote {1}{u} (1/3) [creature: Frog]` -- or, cheaper and scoped to the
   decision that turns on it, a boolean token on any creature that is a legal mutate host:
   `[non-Human - legal mutate host]`. Prefer the general TYPE field: this seat's failure is one
   instance of "the model needs a card property that exists in the data and not on the surface",
   and a mutate-specific token buys only this deck.
3. **If (2) lands, ALSO consider the aggregate**, because the N-139n result is that the aggregate
   is what actually kills membership denial: a count on the board line
   (`Your battlefield (creatures: 4, non-Human: 4)`). Recorded as the escalation ladder's next
   rung, not proposed now -- (1) has not been tried yet at this seat.

**Falsifiable prediction for wave 34 (guide layer only):** zero replies asserting a deck139
creature is Human; mutate take-rate >= 5/6 of offered windows.

---

## N-139y (NEW, MED, GUIDE LANE this wave; render lane if it recurs) -- ONE BLOCKER ASSIGNED TO FOUR ATTACKERS; the parser is correct and the game was lost

**Repro:** vs105 s24, T11, `kind=blockers`, life 14, poison 3 of 10, **no fallback**, `choice=1`.

The prompt says, two lines above the answer:
```
Assign each blocker to AT MOST ONE attacker (a creature cannot block two attackers), but several
DIFFERENT blockers may gang-block the same attacker.
```
The reply's first line:
```
BLOCKS: Illuna blocks Cystbearer, Illuna blocks Hand of the Praetors, Illuna blocks Ichorclaw Myr #1, Illuna blocks Ichorclaw Myr #2
```
reasoned as *"Illuna is the only creature capable of blocking all four attackers ... Since she can
legally block all four (assigning damage from each to her), she stops all poison damage."*

The parser took the first assignment (first-wins, as designed), `chosen_text = "Illuna, Apex of
Wishes blocks Cystbearer"`. The other three attackers connected for **7 poison; 3 + 7 = 10**;
game over at 14 life.

**The parser behaved correctly and this is NOT a proposal to change it.** The observation is
threefold:
1. It is the **inverse** of the gang-block trap the bundling was built to fix. The prompt teaches
   "several DIFFERENT blockers may gang-block the same attacker" and the model generalised the
   symmetry: one blocker across several attackers. The prohibition and the permission are in the
   same sentence, and the permission is the more vivid half.
2. The **silent drop is the expensive part**: three assignments were discarded with no signal in
   the record. `unparsed_reply` did not fire (one assignment survived), `all_assignments_illegal`
   did not fire (not all were illegal), and nothing counted the three drops. **Suggested
   instrument, cheap: a `dropped_assignments` int on blocker records** (count of `B:A` pairs the
   reconciler discarded as illegal or duplicate). It costs one counter and it makes this class
   visible without changing behaviour. `pruned_pairs` exists as a string field but was not emitted
   on any record at this seat -- check whether it is wired for this path.
3. Guide-layer interim shipped this wave: the prohibition quoted verbatim from the render, plus
   the forbidden sentence named in the pilot's own phrasing.

**Severity vs repro quality (wave-31 HL4.3):** severity **HIGH** for this game (it decided it),
**MED** as a class (n=1 at this seat this corpus); repro quality **HIGH** -- fully determined by
the printed option set, no timing dependence.

---

## N-139x (NEW, MED, INSTRUMENT LANE) -- `commit_retracted` cannot see a post-answer re-commit the parser ACCEPTS

`commitRetracted()` (`AIPlayerGPT.cpp:2259`) returns false unless `fallback` is one of
`retracted_choice` / `truncated_abandoned` / `truncated_abandoned_heuristic`. It is therefore a
counter of retractions that reached the HEURISTIC, not a counter of retractions.

At this seat **14 replies emitted more than one line-leading coded answer, and in 6 the LAST label
differs from the first**. Every one of the 6 was accepted by the parser, which executed the LATER
answer, with `fallback = null` and `commit_retracted = false`:

| game/seq | kind | first | executed | overrun |
|---|---|---|---|---|
| vs146 s13 | ask | `CHOICE: 2 (Cast nothing right now)` | `CHOICE: 1 (Cast Snapdax)` | 0 |
| vs152 s21 | priority | `CHOICE: 0 (pass)` | `CHOICE: 1 (Draw 1 with Clue)` | 0 |
| vs152 s25 | blockers | `B1:A3, B2:A3, B3:A3` | `B1:A3, B2:A3, B3:A4` | 4,943 |
| vs152 s27 | blockers | `Arboreal Grazer blocks Elite Spellbinder` | `Arboreal Grazer blocks Tovolar's Packleader #1` | 2,107 |
| vs116 s30 | ask | `CHOICE: 7 (Pollywog Symbiote)` | `CHOICE: 2 (Sacred Foundry)` | 0 |
| vs158 s28 | blockers | `B1:A1, B2:A2, B3:A2, B4:A3` | `B1:A1, B2:A3, B3:A3` | 0 |

Note that **4 of the 6 have `post_plan_overrun = 0`** -- the second answer arrived BEFORE any
PLAN line, so the overrun counter does not see them either. The corpus-level "4.2% last-label-
differs" number the owner cites is measured the right way; the two new translog FIELDS are not the
instrument that produces it. **Suggested minimal fix, no behaviour change:** add
`coded_answers` (int, count of line-leading coded answer lines) and `answer_changed` (bool, last
label differs from first) to every decision record. Two string scans, and they measure the class
the owner's directive is actually about.

---

## N-139w (NEW, LOW, RENDER) -- i5's zero-cost emitter prints `{0}` where the true statement is "has NO mana cost"

The falsy-zero guard removal (i5 / N-36b) correctly restores `{0}` on genuine zero-cost cards --
`Mox Sapphire {0}`, `Mox Jet {0}`, `Ornithopter {0}` (45 renders at this seat, zero confabulation
about any of them). It also now prints `{0}` on permanents that have no mana cost at all:

- `Tovolar's Packleader {0} (7/7) [nightbound]` and `Tovolar's Packleader #1 / #2` -- transformed
  DFC back faces (3 renders). A back face has no mana cost; `{0}` states it costs nothing.
- `Day {0}` / `Night {0}` -- the day/night DESIGNATIONS, which are not cards (5 renders).

Decision-neutral at this seat: all eight are opponent-side permanents this seat can never cast,
and no reply reasons about their cost. Recorded because the general shape is the mirror of the bug
i5 fixed -- the guard now emits a value where the field is undefined rather than zero, and a deck
that can flicker or copy a back face would meet it on a castable line. Suggested discrimination:
emit the token only when the card HAS a cost object; render nothing (or the existing token-style
line) when it does not.

---

## N-139m (HARNESS LANE, KNOWN-OPEN -- CITED, NOT RE-DIAGNOSED)

Per the wave-33 brief, N-139m is re-scoped to the harness lane (script-pump menu-flow divergence;
five reproducer fixtures preserved unregistered in `bin/Res/test/ai/`). Live count this corpus for
the record only:

**Trigger predicate** (a creature spell WITH MUTATE cast while `Pollywog Symbiote` appears in this
seat's `Your battlefield:` line): **1 firing, 0 vanishes, 1 arrival.**

vs116 s15 -> s21: `Cast Gemrazer with its mutate cost {g}{g}` against `other={1}{G}{G}` (one
Pollywog's `altercost(colorless,-1)`), `Choose an option` -> `mutate`, **two** `DISCARD ONE OF YOUR
OWN CARDS` asks (two Pollywogs, s17 + s18, both answered `Stomping Ground`), `mutate over` (s19),
`TARGET CHOICE` -> `Pollywog Symbiote #1` (s20), arrival at s21:
`Gemrazer {1}{u} (4/4) [trample, reach, mutate] [mutated pile - ONE merged creature ...]`.
**N-139g (under-card statics) confirmed a third time** in the same pile.

The low firing count is a consequence of N-139v, not of the engine: five of six mutate windows were
declined on a false belief, so the predicate had five fewer chances to fire.

---

## N-139l (RECURS, LOW, unchanged) -- merged-pile cost pip drift

Fourth consecutive corpus. The pile renders the cost of whatever card ended up carrying it rather
than the top card's own: `Gemrazer {1}{u} (4/4)` (vs116 s21-s26; `{1}{u}` is Pollywog's cost, the
under card) where the pre-merge cast line read `Cast Gemrazer {1}{g}`. No decision affected. LOW.

---

## CARRIED UNCHANGED, UNEXERCISED THIS CORPUS

- **N-139q** (Nethroi's `auto=` returns ANY NUMBER of creatures each with power<=10; Oracle is
  *total* power 10 or less) -- card-data lane, `borderline.txt`. Nethroi's mutate line was never
  offered. The guide's description stays generic pending the data fix.
- **N-139r** (`@mutated(mytgt)` present on five mutants, absent on Snapdax and Illuna) -- data
  question, unexercised.
- **N-139p** (the bottom ask drops its hand-size sentence at N=7) -- **no bottom asks in this
  corpus at all**; unexercised, not closed.
- **N-139t** (the loot's DISCARD header attributes its source inconsistently) -- vs116 s17/s18 both
  read `your own effect (Pollywog Symbiote)`, which is the CORRECT attribution; the wave-32
  `(Gemrazer)` form did not appear. One-sided evidence from a single cast; not closed.

---

## STOPGAP REGISTER (guide lines, their defect, their removal condition)

| guide line | defect it patches | status / removal condition |
|---|---|---|
| *"Your poison count is not printed anywhere, so ... block the infect attackers FIRST"* | N-139o | **RETIRED this wave.** The removal condition fired: `Poison counters (you): M of 10` renders. Replaced by a read-the-number rule. Re-grep the wave-34 corpus for "still safe at N life" reasoning to confirm it stays at zero. |
| *"The thought 'Stomping Ground / Temple Garden / Overgrown Tomb / Breeding Pool cannot make green because I must pay 2 life' is ALWAYS FALSE ..."* | N-139n | **RETIRED this wave.** Removal condition ("the hand-count header ships and one corpus shows zero false-colour-shortage mulligans") fired exactly. The two general falsity-facts above it are KEPT -- see the next row for why. |
| *"The thought 'this hand has zero lands' / 'this hand has zero green sources' is ALWAYS FALSE when ..."* | N-139n residue | **KEPT despite a 0-occurrence count.** This is the section-3 lesson: a zero count on a class the RENDER now covers is safe to delete; a zero count on a class only the GUIDE covers means the guide is working. The count header covers this one, so it is arguably deletable -- it is kept anyway at one line each, because deleting exactly this kind of sentence is what caused the wave-33 collapse and the cost of keeping it is two lines. |
| *"EVERY creature in this deck is NON-HUMAN ... 'I have no non-Human creature' is ALWAYS FALSE"* + *"the engine prints 'with its mutate cost' ONLY when a legal host exists"* | **N-139v** | NEW this wave. Not a stopgap for a broken surface -- it is a falsity-fact plus a route around an ABSENT surface. DELETE only if the render starts emitting creature type AND one corpus shows zero Human claims. |
| *"A creature blocks AT MOST ONE attacker"* + the named forbidden sentence | **N-139y** | NEW this wave. Permanent guide content unless the render/parser starts rejecting the shape loudly. |
| *"answer `tap` unless the untapped land pays for something you will cast THIS turn"* | the 20/20 shockland tax | NEW this wave. Not an engine defect -- a decision rule for a seam the guide never covered. Permanent. |
| *"THE FLOOR IS THE NUMBER PRINTED IN FRONT OF YOU ... 'having already taken N mulligans'"* | wording fix for a deferred internal counter | KEPT, **UNEXERCISED** (0 mulligans this corpus, so the quoted string has 0 hits). The emitter is alive at `AIPlayerGPT.cpp:8476`. Unexercised is not superseded; do not delete on a zero count. |

---

## HARNESS / CORPUS HYGIENE (no action needed)

- **Crash gate: 0** signatures (`segmentation|SIGSEGV|assert|abort|FATAL|core dumped|terminate
  called`) across all six deck139-seat `.stderr` files. **Fourth consecutive clean corpus.**
- **Fallbacks: 0 / 178 decisions.** No `unparsed_reply`, no `empty_reply`, no `retracted_choice`,
  no `all_assignments_illegal`, no `validation_reject_reask`, no `deferred_to_heuristic`,
  no `truncated_abandoned`. Cleanest seat log the deck has produced.
- **`fails validation; deferring to heuristic`: 0** occurrences in all six seat stderr files.
- Latency improved on every statistic (median 14,653 ms, mean 24,885, p90 53,204, max 188,084,
  **>195 s count 0** vs wave-32's 1). The three slowest records are wide-board combinatorial
  decisions (11 attackers x 3 blockers; the 4th and 5th of six annihilator sacrifices), all
  answered correctly, none retracted -- a latency profile, not a belief.
- **`pruned_pairs` was not emitted on any record at this seat**, including the vs105 s24 record
  where three assignments were pruned. Worth a one-line check that the field is wired on the
  blocker path.
