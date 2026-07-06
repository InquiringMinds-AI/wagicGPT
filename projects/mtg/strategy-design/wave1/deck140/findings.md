# Additional findings from the deck140 work

Catch-all for observations beyond the strategy guide, the skill, and the
general-guide suggestions. All cite specific translog records from
`selfplay-runs/pool7-20260705-232254/*ai_baka_deck140*.jsonl` (records
referenced by my [index] from the game-by-game extraction).

## 1. (Most useful) Cast/action options carry NO effect text at the decision point

Each casting option is just the name + mana, e.g. `2. Cast Pyroclasm
{{1}{r}}`. What the card DOES is only in the `{MY_DECK}` block far above the
action list. A weak pilot has to recall each card's effect from memory to
choose. This is a direct contributor to the wrong-sweeper misplay I saw:
game 1783311796 [8], board = Faerie Miscreant (1/1) + Archmage of Echoes
(4/4), the pilot cast Pyroclasm (2 damage -- kills only the 1/1) and then had
to spend Damnation too. The board state DID show the toughness (`(4/4)`), so
the information to evaluate was present -- the gap is that the option line
didn't remind the model that Pyroclasm deals only 2.

Concrete suggestion: annotate each cast option with a short effect drawn from
the card's `text=`/`auto=` line, e.g. `Cast Pyroclasm {1}{R} (2 damage to all
creatures)`, `Cast Wrath of God {2}{W}{W} (destroy all creatures)`. This puts
the decision-relevant fact at the decision point and would help removal/
sweeper selection across every deck, cheaply. This likely helps more than any
prompt-wording change because it fixes an information-locality problem, not a
reasoning one.

## 2. X-spell mana renders as a meaningless `{l}`, and X-spells aren't flagged as scalable

Rakdos's Return ({X}{B}{R}) displays as `Cast Rakdos's Return {{r}{b}{l}}` and
Black Sun's Zenith ({X}{B}{B}) as `{{b}{b}{l}}` (e.g. game 1783311796 [8]-[9],
game 1783313459 throughout). The X pip is being rendered as `{l}` -- an
apparent symbol-mapping bug (X -> `l`). To the model this reads as a
nonsense mana symbol and gives NO signal that these are scalable X-cost spells
whose power depends on available mana. The X value is then chosen in a
SEPARATE later menu (`1. X = 0  2. X = 1  3. X = 2  4. X = 3`, game 1783311796
[10]). Two fixes: (a) render the X pip as `{X}`; (b) consider annotating X
spells so the pilot understands the cost scales (ties into finding #1). Note
also the X menu offers `X = 0`, a legal-but-useless choice (BSZ for X=0 puts
zero counters and shuffles back doing nothing) that a weak model could pick --
worth suppressing X=0 for effects where it is a no-op.

## 3. Position bias: the passive option is always listed FIRST

Casting decisions always list `1. Cast nothing right now` first; land drops
always list `1. Hold <land> - do not play it now` first. Given the measured
82% "cast nothing" rate and LLMs' known first-option bias, listing the passive
choice at position 1 plausibly AMPLIFIES the passivity, not just reflects it.
Cheap, testable experiment: order the passive/"do nothing" option LAST in the
action list (or randomize position) and re-measure the pass rate. This is a
representation lever independent of the prompt wording, and it costs nothing
to try. At minimum, be aware of it when reading pass-rate stats.

## 4. Heavy re-polling inflates the translog and multiplies "pass" opportunities

The same casting decision is presented many times within a single turn -- the
pilot is re-asked at Main 1 (twice), during combat steps, and at Main 2 with
an identical board and identical options (e.g. game 1783313459 [15]-[17], all
the same `Cast nothing / Pyroclasm / BSZ / Staff / Lightmine` at 22-21). This
is priority being offered at each step, not a bug, but it (a) inflates raw
decision counts, so DEDUPE near-identical consecutive records before computing
behavior stats, and (b) gives the passive model many independent chances to
pass the same turn away. If per-step priority prompts for pure sorcery-speed
plays could be collapsed to one prompt per main phase when nothing changed, it
would cut redundant inference calls and reduce passivity surface.

## 5. Verified NON-issues (so nobody re-chases them)

- I initially suspected sorcery casts were being offered during the opponent's
  turn. Verified false: casting decisions only appear on the pilot's own main
  phases (checked the `| It is your turn.` line directly; game 1783311787 [6]
  is `Main phase 2 | It is your turn`, not opponent combat). My first compact
  extraction's phase label was pulled from the events recap and was unreliable
  -- a caution for anyone parsing these logs: read the `Phase: ... | It is ...`
  line, not the events list, for the true current phase.
- Board state DOES include current power/toughness, tapped status, counters,
  and attachments (e.g. `Signal Pest (3/1) (printed 0/1) [tapped]`, game
  1783314195 [12]; `Etched Champion (7/2) (printed 2/2)`, [23]). So combat/
  removal misplays are reasoning failures the guides can address, not
  missing-information failures -- the "(current) (printed X/Y)" format is
  actually good and worth keeping.

## 6. Minor: mulligan is prompted twice at game start

Records [0] and [1] of most games are identical `Keep / Mulligan` prompts
(e.g. game 1783311783 [0] Untap, [1] Upkeep). Harmless, but redundant inference
-- worth confirming the mulligan seam isn't firing an extra time.
