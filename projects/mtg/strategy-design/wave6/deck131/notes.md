# deck131 wave-6 — working notes (catch-all)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260713-190011/`, 6 deck131 games, 1W/4L/1TO.
Seat files: vs133-TO 1783987216 | vs140-L 1783989086 | vs44-L 1783994322 | vs109-L 1783994820 |
vs135-W 1783995005 | vs110-L 1783996823.

## For the deck builder (deck131 rebuild levers — the record is deck-bound, not guide-bound)

- **[#1 FLAG, CONFIRMED 3rd wave running] The manabase actively loses games.** 14 Island / 8 Mountain,
  but every win condition is red (Guttersnipe {2}{R}, Young Pyromancer {1}{R}). This wave red screw
  shaped 3 of 6 games: vs110 drew ZERO Mountains in 13 turns (Young Pyromancer uncastable all game,
  dead); vs109 first Mountain T8 (mono-red raced it dead T9); vs133 first Mountain T10 (threats
  stranded -> 31-turn stall). Cheapest fix: move 3-4 Islands to Mountains (11M/11I or 12M/10I). Blue
  is forgiving - only Counterspell {U}{U}, Dissipate {1}{U}{U}, Opportunity {4}{U}{U} are blue-hungry
  and all are support; workhorses are single-{U} (Downsize, Cyclonic Rift, Essence Scatter, Artificer's
  Epiphany, Aetherize).
- **Threat density 6/60 is the structural ceiling** (3 Guttersnipe + 3 Young Pyromancer). Even perfect
  piloting durdles when neither is drawn, and once they ARE drawn a removal-heavy opponent answers them
  on sight (vs44: both removed the turn after casting; vs133: Guttersnipe removed, deck had no closer
  left -> 17-17 timeout). The deck has NO reach once its 6 threats are gone - every remaining card is a
  bounce/counter that does nothing without Guttersnipe. **4 Prism Ring is pure lifegain filler** (cast
  8x across the corpus, always as a "nothing better" mana sink, never advancing the clock) - the
  obvious flex to cut for threats/reach or more red. Spellheart Chimera (already #SB, scales with
  instants/sorceries in yard) is on-theme reach worth maindecking.
- **Against lifegain (deck140) this deck cannot win at any pilot quality** - a 2-damage clock vs
  +10/turn (opp climbed to 143). Fine if deck tiers are intended, but every deck131-vs-140 game
  loses/times out and burns ~35 turns of wall clock.

## For the engine / representation owner
- **Mana-ability mislabel (deck110-class) still present** - a land's mana ability can be offered as
  "Cast Card Normally with Mountain". Representation defect; a guide/prompt line cannot fix a mislabeled
  option. (Carried from wave 5; not re-audited exhaustively this wave.)

## For the harness owner
- **`options_text` is now a STRUCTURED ARRAY** (wave-5 ask ADDRESSED - thank you). Offered-vs-taken and
  attacker-participation scripts are now trivial; no more regexing option lines out of `prompt`. The
  `A#.` attacker list still lives only in the prompt text, but cast-option analysis is clean now.
- **TIMEOUT games still write no `gameend` record** (wave-4/5 ask, still open). vs133 timed out with no
  closing record; final 17/17 at turn 31 had to be read off the last decision. A timeout-adjudication
  record (final life + turn + who was ahead) would let reviewers score timeouts without reconstruction.
- **Result pairing is still manual epoch arithmetic** (`seat_epoch = results start_epoch + 3..4`, then
  confirm by opponent card names). A game-id/opponent/result header in each seat jsonl (or a pairing
  table in the run dir) would remove ~7x duplicated toil per wave.

## For the next deck131 reviewer (method gotchas)
- **n=6, single run: win-rate is noise.** The stable behavior metrics at this n: attack participation,
  cast-nothing-while-Guttersnipe-out (2 fired/3 windows/0 declined this wave - RESOLVED but
  under-powered, precondition rare), Mountain-first adherence, MULLIGAN adherence (the new lever), and
  block quality. Don't read 1/6 as a regression or a recovery - the opponent slate and draws dominate.
- **Guttersnipe/payoff on-board checks: isolate the `Your battlefield:` line** (splitting on `--- CURRENT
  SITUATION ---` also matches `Your hand:` and the carried PLAN). Guttersnipe reached the board with a
  cast window in only ONE of six games this wave; verify presence per-record before scoring engine use.
- **Attacker participation: count declared vs the engine's `A#.` OFFERED list, not the battlefield**
  (summoning-sick/tapped/just-made creatures are legitimately absent). 100% participation this wave.

## For the model-experiments track (qwen35 as pilot)
- **Concrete named rules keep near-perfect compliance** (reconfirmed): Guttersnipe cast-every-turn,
  Mountain-first, cast-payoff-first, count-and-match attackers, no-blocks-while-racing all executed.
- **The durable weakness this wave is a PRECEDENCE failure at the mulligan seam, not a capability gap:**
  the pilot kept 2 of 3 no-red openers because the general "keep 2-5 lands" default beat the deck
  guide's red-source override - yet vs109 applied the guide rule correctly and verbalized exactly why
  ("lacks a Mountain, which is required"). Same model, opposite calls on the same hand shape =
  inconsistency the sharpened by-name override targets. Recheck metric wave 7:
  `(no-Mountain openers mulliganed)/(no-Mountain openers)`; this wave 1/3.
- **No parser desyncs, no fallbacks, no engine no-ops** across all 6 games - transport clean.
