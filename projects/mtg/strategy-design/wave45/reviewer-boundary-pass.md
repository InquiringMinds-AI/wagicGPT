# Wave-45 reviewer boundary pass (2026-08-26)

The deck REVIEWER lane's adjudication of the fifteen boundary findings synthesis reported in
`synthesis-notes.md` ("Guide boundary findings for the deck reviewers"), plus the two
evidence-decided recommendations routed here from `engine-ledger.md`.

Scope discipline: every edit landed in the LIVE guide
`projects/mtg/bin/Res/ai/baka/deckN_strategy.txt` only (B14 and B15 are records, and landed in
`wave45/deckN/edit-texts.md`). No `src/` file touched, no commit, no wagic run. Every card cost
and P/T asserted in a new sentence was read off `projects/mtg/bin/Res/sets/primitives/mtg.txt`
with `/usr/bin/grep`: Bloodline Keeper `{2}{B}{B}`, Damnation `{2}{B}{B}`, Sanguine Bond
`{3}{B}{B}`, Exquisite Blood `{4}{B}`, Siege-Gang Commander `{3}{R}{R}`, Rorix Bladewing
`{3}{R}{R}{R}`, Pyrite Spellbomb `{1}`, Spark Spray `{R}`. Encoding: `git diff |
/usr/bin/grep -c $'\357\277\275'` = 0, and all seven guides are pure ASCII.

---

## Verdicts, B1-B15

| # | deck | verdict | reason |
|---|---|---|---|
| B1 | 125 | **ACCEPT** | The section really does run to the end with no card-count stop, and the seat took 2 of the corpus's 8 mulligans. Applied as STEP 1, keyed to the ask's own `(keeping N)` price line, at the very top of the section. |
| B2 | 125 | **ACCEPT** | One-line bottoming with no floor and no land carve-out is the #37 gap. Extended to a priority order with both floors. |
| B3 | 152 | **ACCEPT** | Line 308's "keeping five cards" is rhetorical, inside a warning - the executor cannot stop on it. Same STEP 1 imperative added above the land branches. |
| B4 | 152 | **ACCEPT** | The bottoming line had no land rule at all; deck146's carve-out is the tested form and is what was appended. |
| B5 | 126 | **ACCEPT** | Stop present, sequence silent after the keep - the exact defect that cost deck162 a Shield Sphere. Bottoming rule added, naming the three cards that win the game. |
| B6 | 130 | **ACCEPT** | Same gap. Bottoming rule added, with Starstorm named as the card never to bottom. |
| B7 | 130 | **ACCEPT** | The coverage line is the fact that made this seat's one-land keep right, and the guide never pointed at it. Added as the branch that decides the close hands. |
| B8 | 123 | **ACCEPT** | The deck attacks wide into 12-14 untapped creatures and the collapsed form renders on its A-lines. Added as a new CHECK 3, explicitly a FILTER rather than a stopping check, and the old CHECK 3/4 renumbered to 4/5. |
| B9 | 126 | **ACCEPT** | 1/1 lifelink Vampires are exactly what a gang eats for free. Added in the deck's own vocabulary (the Vampire is worth more as a lifelink blocker), with the both-enchantments win button as the stated override. |
| B10 | 130 | **ACCEPT** | Live window in the corpus. Added inside the ATTACK FLOOR: Goblin tokens go anyway, Rorix goes only when every named blocker is grounded, Siege-Gang never. |
| B11 | 146 | **ACCEPT** | Its Nadaar died to a collapsed tag. Placed ABOVE the Shield Sphere exception, with the precedence stated outright: a collapsed tag never proves every blocker is a Shield Sphere, so the exception cannot reach it. |
| B12 | 146 | **ACCEPT (in part - the list already shipped)** | The finding is right about the defect but stale about the file: the wave-45 revision already deployed `THE WHOLE CAST ORDER, ONE LIST` as a first-match list of seven entries. The RESIDUAL is real and was applied - DEPLOY, PLANESWALKERS, CLOSE THE GAME and SILVERQUILL COMMAND still read as independent imperatives, so each is now stamped with the numbered entry it hangs off. |
| B13 | 130 | **ACCEPT** | Correct for this guide: `HOW THE DECK WINS` is a land-count curve table, not a first-match cast list. One added, with the card sections declared detail beneath it. |
| B13 | 126 | **REJECT** | Misreads the corpus evidence. deck126 HAS an ordered cast list - `RULE #7 - WHAT TO CAST WHEN SEVERAL THINGS ARE LISTED`, seven numbered entries taken highest-first, with 7a and 7b as named re-orderings. The game the finding cites (a combo half cast over the removal that answered the board) is a violation of 7b, which already outranks entries 1 and 2 for exactly that case - a second list would not have changed it and would contradict the first. |
| B14 | 123 | **ACCEPT** | Agreed on the evidence and agreed that it is not a wording change. The absolute stands verbatim; the falsifier is recorded in `deck123/edit-texts.md` as UNREACHABLE-IN-POOL with a wave-46 re-check. |
| B15 | all 7 | **ACCEPT** | Amendment 61 applies to every threshold, and none of the seven files carried the WHY line. A `THRESHOLD WHYS` section appended to each, one line per numeric threshold that guide ships. |

## The two evidence-decided recommendations

**deck123's 12-token cap -> 20, WHY written in (ledger DECISION 1): APPLIED.** As the reviewer of
this deck I agree with the evidence and with the bounded move rather than the deletion: ten
violations, every one odds-favourable at 16 life with no lethal on the board, is a
VIOLATED-UNPUNISHED record (skill 64) and not grounds to delete the section's only stop (skill
#35). The WHY is now IN THE GUIDE, not only in the artefacts - the ranged-collapse render held a
22-permanent board to ~19k characters, so board width costs no decision quality below 20 - so no
later wave has to re-derive it. The sunset test is recorded in `deck123/edit-texts.md`.

**The mulligan-chain teach (amendments 66/67) at the two mull-to-four seats: 123 APPLIED, 146
NOT MOVED.**
- deck123 had the substance ("AT FIVE CARDS OR FEWER: keep anything that has a land and a spell")
  but at the BOTTOM of its section and keyed to nothing the ask prints. It is now STEP 1, first
  line, keyed to the `(keeping N)` price line, with a STOP ("do not read another rule in this
  section") and the 42-0 loss named - amendment 60's placement rule as much as 67's.
- deck146's STEP 1 is already this text, verbatim, including the land carve-out and the
  castable-spell floor. Skill 66 forbids moving a floor on one window in either direction, and
  the one window here was a violation that lost at -41 - evidence FOR the floor. NOT MOVED, and
  no wording touched. The teach was PROPAGATED instead to the two guides the ledger names as
  missing the stop, deck125 and deck152 (B1, B3).

---

## Edits made, by guide

### `deck123_strategy.txt`
1. RULE 1 token stop raised 12 -> 20 (both branches, the "Twenty creatures already standing"
   line, and the past-the-cap exception), with a new `WHY THE NUMBER IS 20` paragraph.
2. DECIDING SITUATIONS' two token-line entries re-stated at 19/20 (skill 51: the summary
   inherits the rule's new number in the same edit).
3. New `CHECK 3 - THE COLLAPSED TAG AND THE "GANG BLOCK:" CLAUSE` in ATTACK, declared a filter
   rather than a stopping check; old CHECK 3/4 renumbered 4/5 with their internal
   cross-references updated, and the ATTACK header changed to "five checks".
4. CHECK 4 (the count) gained the defender-chooses clause - outnumbering tells you damage gets
   through, never that a card is safe (amendment 54).
5. MULLIGAN gained STEP 1 (the `(keeping 5)` chain stop) and STEP 2; the old five-cards branch
   now defers to STEP 1.
6. BOTTOMING gained the three-land floor and the last-castable-spell floor.
7. DECIDING SITUATIONS gained a `(keeping 5)` line, a collapsed-tag line, and the two bottoming
   floors on the existing bottoming line.

### `deck125_strategy.txt`
1. MULLIGAN gained STEP 1 (the `(keeping 5)` chain stop) and STEP 2, above the odds comparison.
2. The one-line bottoming rule became a priority order (Emrakul, duplicates, lands beyond the
   fifth) with the three-land floor and a never-bottom-your-last-answer floor.

### `deck126_strategy.txt`
1. MULLIGAN gained a `WHEN A KEEP MAKES YOU BOTTOM CARDS` rule: expensive spell first, then a
   duplicate wall; never Sanguine Bond, Exquisite Blood or Idyllic Tutor; three-land floor.
2. RULE #6 gained `THE COLLAPSED TAG`, scoped to the Vampires, with the both-enchantments win
   button as the override.
3. SITUATIONS gained three lines (collapsed tag, the `(keeping 5)` keep, the bottoming order).

### `deck130_strategy.txt`
1. MULLIGAN gained `THE COVERAGE LINE DECIDES THE CLOSE ONES` and a bottoming rule with the
   three-land floor.
2. ATTACK FLOOR gained `THE COLLAPSED TAG, INSIDE THE FLOOR`, with per-body verdicts.
3. New `THE WHOLE CAST ORDER, ONE LIST` section (seven entries, first-match), placed above
   `YOUR CARDS - ONE LINE EACH`, which is now declared detail beneath it.
4. DECIDING SITUATIONS gained four lines; the mulligan one defers to the guide's own
   never-below-6 stop rather than restating a looser floor beside it.

### `deck146_strategy.txt`
1. ATTACKING gained `THE COLLAPSED TAG AND THE "GANG BLOCK:" CLAUSE`, above the Shield Sphere
   exception, stating the precedence between them.
2. DEPLOY, PLANESWALKERS, CLOSE THE GAME and SILVERQUILL COMMAND each stamped with their entry
   in THE WHOLE CAST ORDER (B12's residual).
3. DECIDING SITUATIONS gained the collapsed-tag line.

### `deck152_strategy.txt`
1. MULLIGAN gained STEP 1 (the `(keeping 5)` chain stop) and STEP 2, above the land branches.
2. The bottoming line gained the land carve-out ("at least FOUR").

### `deck162_strategy.txt`
No guide edit. Synthesis records it as satisfying 52, 66 and 67 in form and content, and the
reviewer pass found nothing to add; its collapsed-tag wording is what B8-B11 copied elsewhere.
Its `edit-texts.md` gained the amendment-61 THRESHOLD WHYS block.

### `wave45/deckN/edit-texts.md` (all seven)
- `deck123`: the B14 falsifier record (UNREACHABLE-IN-POOL, wave-46 re-check) plus the WHYs,
  including the 12 -> 20 WHY and its wave-47 sunset test.
- all seven: a `THRESHOLD WHYS (skill amendment 61)` section, one line per numeric threshold in
  the form "T = <number> because <the observable that goes wrong above it>".
