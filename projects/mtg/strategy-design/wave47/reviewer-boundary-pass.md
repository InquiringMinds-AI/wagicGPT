# Wave-47 reviewer boundary pass (deck reviewer lane)

Scope: the seven boundary findings B1-B7 raised in `synthesis-notes.md`, adjudicated by the
reviewer of each deck and applied to the LIVE guides in `projects/mtg/bin/Res/ai/baka/`.
Card facts re-verified with `/usr/bin/grep` against `projects/mtg/bin/Res/sets/primitives/*.txt`;
render literals re-counted with `/usr/bin/grep -Fo` over the wave-47 corpus
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260826-182155` (125 MB, 42 logs). ASCII only;
`git diff | /usr/bin/grep -c $'\357\277\275'` = **0**.

## Verdicts

| # | verdict | reason |
|---|---|---|
| B1 | **ACCEPT** | Confirmed: cast-list entry 1 named all three punishers undivided, so RULE 1's new tiebreak was unreachable on the executable surface (#92/#95). Applied at BOTH executable surfaces, not just the list - `DECIDING SITUATIONS` restated the set undivided too. |
| B2 | **ACCEPT** | Card-fact defect confirmed in the primitives: `Master of the Feast` = `auto=@each my upkeep:draw:1 opponent`, `type=Enchantment Creature`, 5/5 flying, `abilities=flying` (mtg.txt:71933). It makes the OPPONENT draw - a draw ENGINE with a body, not a punisher. The guide's by-name property list was false (#75). |
| B3 | **ACCEPT** | Negative-existence claim about the render (#99), same shape as the "does NOT print a count of your own hand" sentence this seat deleted this wave, and D7/R8 is docketed to land the keyword set on those exact rows. Cut to the positive test plus a one-line sunset. |
| B4 | **ACCEPT, re-keyed (not merely caveated)** | Reset verified independently in `1787786558-...deck123-...-vs-deck126.jsonl`: the `[repeat: activated this turn N times already]` tag reads **650** at seq 1099 and **14** at seq 1565, same loop, 1,539 creatures out. A stop on a number that starts over each turn is not a stop (#88). Re-keyed the tripwire to `of which M are creatures`, which the line moves and never gives back, at an absolute ceiling of 50 (above any reachable L+C+3, far below the 1,539-creature board). The tag is described as accurate-but-scoped, never as wrong - trust doctrine intact. |
| B5 | **ACCEPT (closing entry only; roll-call REJECTED)** | The list is not total: entry 5's own text instructs the answer `"Cast nothing right now"`, which the list did not rank (#76/#92). Appended it as entry 8. The optional by-name roll-call is **rejected** - deck123 is the pool's largest guide at 40 KB, no cast-order misplay was adjudicated this corpus, and #45 forbids unpaid bytes on no evidence. |
| B6 | **ACCEPT (all three)** | Three mood-word rule inputs (#72c), each with a printed number one line away. deck126:32 re-keyed to its own preceding formula; deck123:408 and deck130:295 re-keyed to the blockers header's `you would be at N`, verified present on **every** blockers screen in the corpus (40 hits, 1:1 with `Unblocked, these attackers deal up to`). |
| B7 | **ACCEPT, and deck146's half upgraded from example to rule key** | Both quoted composites confirmed at **0** occurrences. deck146: bare `[menace]` = 0; the real battlefield form is `(4/4) (printed 3/3) [doesn't untap during its controller's untap step, menace (can't be blocked except by two or more creatures)]` (105 hits). This is more than an example defect - CHECK 2 branched on `If it already reads (3/3)`, and an anthem-pumped animated Hive never reads 3/3, so the branch was unsatisfiable on exactly the boards it governs. Re-keyed to "ANY (P/T)". deck152: the quoted composite = 0; the printed clause `from this block only, and this attacker deals nothing to their life` = 88, and it now states the mutual exclusivity itself, so the paragraph collapsed to a pointer. |

No finding was rejected outright; one sub-item (B5's roll-call) was rejected on byte cost with no
evidence behind it.

## Edits applied, per guide

| guide | finding(s) | edit | byte delta |
|---|---|---|---|
| `deck123_strategy.txt` | B4, B5, B6 | RULE 1 tripwire re-keyed from `[repeat:]` to a monotone `of which M are creatures` ceiling of 50 (seq 1099 = 650, seq 1565 = 14, same loop); cast list closed with entry 8 `"Cast nothing right now"`; `while your life is comfortable` -> `while the combat header's "you would be at N" is above 5`. | 40,084 -> 40,627 (**+543**) |
| `deck125_strategy.txt` | B3 | Negative clause about `[defender]` deleted; rule left on the positive `{target text:}` / printed-power test (1,149 `{target text:` hits) plus a one-line sunset for when the tag lands on the stack row. | 34,394 -> 34,338 (**-56**) |
| `deck126_strategy.txt` | B6 | `looks one or two out of reach ... probably affordable` -> the guide's own printed formula: plain-number part of the cost at most `"Mana available:"` plus one per defender past the first. | 30,969 -> 31,033 (**+64**) |
| `deck130_strategy.txt` | B6 | `take the damage while your life is comfortable` -> `while the combat header's "you would be at N" is above 5`, matching the numeric line directly beneath it. | 32,386 -> 32,412 (**+26**) |
| `deck146_strategy.txt` | B7 | CHECK 2 re-keyed from `reads (3/3)` to "carries ANY (P/T)", quoting the real battlefield form (105 hits) and naming the anthem case. | 41,735 -> 41,918 (**+183**) |
| `deck152_strategy.txt` | B7 | Fabricated composite tag replaced with the printed clause (88 hits); paragraph cut to a pointer, keeping the "never write both halves" stop. | 37,060 -> 36,855 (**-205**) |
| `deck162_strategy.txt` | B1, B2 | Cast-list entry 1 split into 1a (Fate Unraveler, on the two printed creature counts) / 1b (Underworld Dreams or Ob Nixilis); the same tiebreak added to the `DECIDING SITUATIONS` restatement; Master of the Feast renamed as a draw ENGINE with a body, governed by RULE 2's brake, entry 5. | 28,033 -> 28,465 (**+582** incl. the DECIDING SITUATIONS clause) |

**Pool total: +1,137 bytes across seven guides** (two guides shrank). Every added line is paid for
by a cited seq or a corpus literal count above. No `src/` change, no commit, no wagic run.

## Notes forward

- B4's re-key does not close **D13**: the `[repeat:]` tag is still the natural key for a
  single-ability loop, and if the engine makes it cumulative (or adds a per-loop total) the
  deck123 ceiling should move back onto it and the M ceiling revert to a backstop.
- deck146's CHECK 2 was filed as low-severity (an example) and was in fact a dead rule branch.
  Worth carrying to the next wave's sweep: **every quoted (P/T) inside a rule branch is a
  literal check, not an example**, because anthems and counters move printed sizes.
