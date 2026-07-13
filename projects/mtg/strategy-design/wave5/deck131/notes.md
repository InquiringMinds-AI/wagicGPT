# deck131 wave-5 — working notes (catch-all)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260713-145947/`, 6 deck131 games, 0W/5L/1TO.

## For the deck builder (deck131 rebuild levers) — the record is deck-bound, not guide-bound

- **[REFRESHED, now the #1 flag] The manabase actively lost games this wave.** 14 Island /
  8 Mountain, but every win condition is red (Guttersnipe {2}{R}, Young Pyromancer {1}{R}).
  **2 of 6 losses (vs110 file 1783974685; vs140 file 1783976995) had NO red source when it
  mattered** - vs110 kept a 3-Island opener with Young Pyromancer and never cast a creature (dead
  T7); vs140 drew its first Mountain on T19 and its threats sat stranded ~18 turns. This is no
  longer a theoretical skew - it is the direct cause of two losses. Cheapest fix: move ~3-4
  Islands to Mountains (11M/11I or 12M/10I). The blue spells are forgiving: only Counterspell
  {U}{U}, Dissipate {1}{U}{U}, and Opportunity {4}{U}{U} are blue-hungry, and all three are
  support; the workhorses are single-{U} (Downsize {U}, Cyclonic Rift {1}{U}, Essence Scatter
  {1}{U}, Artificer's Epiphany {2}{U}, Aetherize {3}{U}).
- **Threat density 6/60 is the structural ceiling** (3 Guttersnipe + 3 Young Pyromancer). Even
  perfect piloting durdles when neither is drawn. If a rebuild is on the table: **Spellheart
  Chimera** (already in the #SB, on-theme - power scales with instants/sorceries in graveyard) for
  2 of the 4 Aetherize; and **4 Prism Ring is pure lifegain filler** that showed up in every
  durdle loss as a mana sink - the obvious flex slot to cut for more threats or more red sources.
- **Against lifegain (deck140) this deck cannot win at any pilot quality** - a 2-damage clock vs
  +10/turn (opp climbed to 56). Fine if deck tiers are intended, but every deck131-vs-140 game
  will loss/timeout and burn ~35 turns of wall clock.

## For the engine / representation owner

- **Mana-ability mislabel persists (deck110-class).** vs110 file 1783974685 seq 8, a priority
  window offered `Cast Card Normally with Mountain {card text: "R"}` - a land's mana ability
  presented as a spell cast. Wastes a decision floating mana. Representation defect; a guide/prompt
  line cannot fix a mislabeled option. Repro pointer: that record's `prompt` option list.

## For the harness owner

- **TIMEOUT games still write no `gameend` record** (wave-4 ask, unaddressed). vs135 file
  1783977943 timed out with no closing record; end life/turn had to be read off the last decision
  (self 23 / opp 8 at the cap). A timeout-adjudication record (final life totals + turn + who was
  ahead) would let reviewers score timeouts without reconstruction.
- **Result pairing is still manual epoch arithmetic.** Seat file -> game -> outcome needed
  `seat_epoch = results start_epoch + 3..4` plus a confirm-by-opponent-card-names pass. Every
  reviewer re-derives this identically; emitting a game-id/opponent/result header in each seat
  jsonl (or a pairing table in the run dir) would remove ~7x duplicated toil per wave.
- **`options` is still an int count, not the list.** Offered actions (cast options AND the `A#.`
  attacker list) must be regexed out of `prompt`. A structured `options` array would make
  offered-vs-taken and attacker-participation scripts trivial - and would have prevented this
  wave's phantom "subset attack" read (see next section).

## For the next deck131 reviewer (method gotchas that cost me time)

- **Attacker participation: count declared vs the engine's `A#.` OFFERED list, NOT vs creatures on
  the battlefield.** Summoning-sick / tapped / just-made creatures are legitimately absent from the
  legal set; counting against the battlefield manufactures a fake under-commitment leak. This
  corpus was 100% participation once counted correctly. (Full write-up in skill.md #1.)
- **Guttersnipe/payoff on-board checks: isolate the `Your battlefield:` line.** Splitting on
  `--- CURRENT SITUATION ---` and searching the remainder also matches `Your hand:` and the carried
  PLAN, giving false "on board" reads.
- **n=6, single run: win-rate is noise.** The comparable behavior metrics at this n: attack
  participation, draw-casts/game, cast-nothing-while-Guttersnipe-out (the new lever), Mountain-first
  adherence, block-quality. Don't read 0/6-vs-2/12 as a guide regression - the opponent slate was
  harder and the two wave-4 wins (vs109, vs44) recurred and lost to mana/removal.

## For the model-experiments track (qwen35 as pilot)

- **Concrete named rules keep near-perfect compliance** (reconfirmed): Mountain-first (violated
  only vs109 T0), draw-on-curve, cast-payoff-first, count-and-match attackers all executed.
- **The one durable weakness this wave is a suppression, not a capability gap:** the pilot declines
  a castable spell while Guttersnipe is out because the "reactions need a target" prior wins. The
  wave-5 guide overrides it by name; the countable recheck for next wave is
  `(spells cast while Guttersnipe on board) / (castable-spell windows while Guttersnipe on board)` -
  this wave it missed at least vs44 seq 10, vs44 seq 24, vs140 seq 22.
- **No parser desyncs, no fallbacks** across all 6 games - transport layer is clean; reply-format
  experiments can assume it.
