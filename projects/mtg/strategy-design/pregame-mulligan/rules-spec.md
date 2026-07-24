# Pre-game & mulligan procedure — current CR spec (research record, 2026-07-24)

Source: official WotC Comprehensive Rules text file, effective 2026-02-27
(https://media.wizards.com/2026/downloads/MagicCompRules%2020260227.txt), section 103 read
verbatim by the research agent. Gemstone Caverns Oracle/rulings cross-checked on Scryfall.
NOTE: mulligans are **CR 103.5** in current numbering (not the pre-2019 103.4); pre-game
actions are **103.6**; section 103 ends at 103.8c.

## Pre-game sequence (strict order)
1. 103.1 determine starting player (game 1: random; later games: loser of previous chooses).
2. 103.2 setup steps (companion reveal — stays OUTSIDE the game, NOT a 103.6 action; commander;
   out of scope for 1v1 constructed except noting companions are not opening-hand actions).
3. 103.3 shuffle; decks become libraries.
4. 103.4 starting life (20 in normal 1v1).
5. 103.5 draw opening hands + mulligan procedure (below).
6. 103.6 opening-hand "begin the game" actions (below) — strictly AFTER all mulligans.
7. 103.8 starting player takes the first turn; 103.8a: in a two-player game the player who
   plays first SKIPS the draw step of their first turn.

## 103.5 — the London mulligan (verbatim decomposition, 2-player)
Verbatim core: "Each player draws a number of cards equal to their starting hand size, which
is normally seven. ... First, the starting player declares whether they will take a mulligan.
Then each other player in turn order does the same. Once each player has made a declaration,
all players who decided to take mulligans do so at the same time. To take a mulligan, a player
shuffles the cards in their hand back into their library, draws a new hand of cards equal to
their starting hand size, then puts a number of those cards equal to the number of times that
player has taken a mulligan on the bottom of their library in any order. Once a player chooses
not to take a mulligan, the remaining cards become that player's opening hand, and that player
may not take any further mulligans. This process is then repeated until no player takes a
mulligan. A player can take mulligans until their opening hand would be zero cards, after
which they may not take further mulligans."

Engine decomposition:
- Declarations are TURN-ORDERED (starting player first), per round. Actions are SIMULTANEOUS
  after both declarations. Loop: declare (SP first, then opp) -> execute all yes-mulligans
  together -> repeat until nobody mulligans.
- Every mulligan draws the FULL starting hand size (7). The shrink is the BOTTOMING, not the
  draw: at KEEP time the player puts N cards (N = mulligans they took) on the BOTTOM of the
  library in any order they choose. Keep after 0 mulls -> bottom 0.
- KEEP is terminal: a player who keeps is permanently out of the loop; the loop continues for
  the other player alone.
- Mulligan to zero is legal (max 7 mulligans at hand size 7; the 8th is prohibited).
- "Opening hand" = the kept, post-bottoming hand — this is the reference for 103.6 triggers.
- 103.5b: "any time you could mulligan" actions (Serum Powder) happen at that player's
  declaration moment, any round; the player then declares.
- Multiplayer/Brawl free first mulligan (103.5c) is OUT OF SCOPE for 1v1.

## 103.6 — pre-game actions
Verbatim: "Once the mulligan process (see rule 103.5) is complete, the starting player may
take any such actions in any order. Then each other player in turn order may do the same."
- 103.6a Leylines: "begin the game with that card on the battlefield" — put directly onto the
  battlefield (not cast).
- 103.6b Chancellors: reveal from opening hand; stays revealed until the first turn begins;
  each card revealable only once; sets up an as-the-turn-begins delayed trigger.
- Gemstone Caverns: only for the NON-starting player, has a cost (exile a card from hand),
  enters with a luck counter.
- The legend rule / SBAs do NOT apply during this window — they first apply just before the
  starting player receives priority in the first upkeep.

## Engine-relevant invariants
- Starting hand size is a VARIABLE (default 7), keyed everywhere (draw count, redraw count,
  zero floor).
- All of 103.5 and 103.6 resolve BEFORE turn 1 begins; no interleaving.
- Play/draw: starting player skips their first draw step (103.8a).
