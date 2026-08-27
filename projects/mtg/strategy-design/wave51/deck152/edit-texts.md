# deck152 - wave-51 edits (corpus `matchups-20260827-155545`, deck152 3-3)

Baseline = live `bin/Res/ai/baka/deck152_strategy.txt` (46,084 bytes, wave-50 revision + boundary
B1). After = `wave51/deck152/strategy.txt` (47,813 bytes, +1,729). Three edits plus three DECIDING
lines. Card facts read off the primitives: Elite Spellbinder `moveto(opponentexile) ... newability
[canplayfromexile forever], newability[changecost(colorless:2)]` = Oracle "its owner may play it ...
costs 2 more" (the exile is a tax, not removal - the live guide's "the ONLY answer this deck has to
an enchantment" was a card-fact overstatement, #145); Idyllic Tutor `target(<1>enchantment|reveal)
... moveto(hand)`; Katilda `lord(creature[human&<colour>]) {T}:add{<colour>}`; Lair of the Hydra
`{1}{G}: becomes(Creature Hydra,1/1,green) ueot`.

| # | section | before -> after (abridged) | finding (file, seq) |
|---|---|---|---|
| 152-G | #4 RULE, KATILDA'S B-ROW paragraph + DECIDING | appends *"HER A-ROW IS THE SAME RULE: a Katilda whose tag names a blocker that kills her - '(your attacker dies, their blocker lives)' - does not attack; one damage is never worth every Human's mana. WHAT THIS COST: on turn 6 this seat sent a 1/1 Katilda into a Silverquill Silencer printed exactly that way; she died, the second Katilda was edicted away, and a game at 20 life was at 3 by turn 10."* DECIDING: one line. | `152 vs146` s12 (T6, 19/20): `A1. Katilda, Dawnhart Prime (1/1) [their untapped blockers: Silverquill Silencer (3/2) (your attacker dies, their blocker lives)]` -> `ATTACK: A1`; s13 events: Katilda died. The generic stop rule ("a tag that contains 'your attacker dies' ANYWHERE is a per-creature STOP") exists and was broken on the deck's mana engine; the reply's plan was the s9 carried plan ("Attack with Katilda next turn"). Re-keyed as the A-row twin of 152-F (#148: per-card exception keyed to the printed parenthesis). Second Katilda cast s13, Soul-Shattered at `146 vs152` s19; 20 -> 3 by s17 (T10); lost -3/19. |
| 152-H | REMOVAL, Elite Spellbinder bullet + DECIDING | *"its hand look is the ONLY answer this deck has to an enchantment, and it works before the card is cast"* -> *"the only way this deck touches an enchantment before it lands - and it is a TAX, not an answer: the card text says its owner may still cast the exiled card, for {2} more"*; adds *"When NEITHER name is on the list, IDYLLIC TUTOR is the name - ... above a Staff, a Lantern or a wall"* + the cost line. | Card fact: primitive `canplayfromexile` + `changecost(colorless:2)`; `152 vs126` s49 events: *"Opponent cast Sanguine Bond from the opponent's exile"* - the exiled card came back. `152 vs126` s10 (T7): list = Overgrown Battlement x2, Staff of Nin, Chromatic Lantern, Idyllic Tutor x2; took Staff of Nin; s20 (T11) took a Tutor over Tribute; both names on their line by s47 (T17), `ATTACK: none` correctly with 11 creatures at 37 life, dead at 0/51 on T18 (Tribute + the loop). The Tutor is the fetch; the guide named only the two enchantments. |
| 152-E (re-key) | YOUR LORDS AND GROWTH, Lair paragraph + DECIDING | *"Two forms of that row are a PASS on sight"* -> *"Three forms ... one printed '[Upkeep offer: this animation lasts only until end of turn, and the same row is offered again in your main phase ...]' (the engine's own note that the main-phase offer is coming - pass it here)"*; cost line extended. | `152 vs125` s36 (T17 Upkeep, 18/13): row carried the lane-E tag and was taken (`CHOICE: 1 (animate Lair of the Hydra)`, plan "attack with Sigarda, Adversary and the Hydra"); s39 (Main 1): `Cast Elite Spellbinder ... {paying this taps: Lair of the Hydra - it cannot attack this turn}` taken - the animated Lair was tapped for mana and never attacked (s41 attackers: Sigarda, Adversary). The "MAIN 1 only" sentence existed; the new tag is the literal to key it to (#128). 1 take in 35 tagged Upkeep offers on my three seats. |

## Read but NOT edited
- **Katilda `(both die)` block at N = 8** (`152 vs130` s16: `B2. Katilda (1/1) - may block A2 (both
  die)`, header "you would be at 8") - inside 152-F's N <= 9 branch; compliant, Rorix's 6 in the air
  was the real damage. Game lost -1/8 T18 to Rorix + Hammer + Starstorm; no guide window.
- **`152 vs126` loss from 37 life**: `ATTACK: none` at s47 with both names on their line (the
  reply explains the loop correctly); 152-A's both-halves prediction PASSES on its first real
  window (0 non-none attacks). The loss is the loop itself, reached via the Tutor (152-H).
- **Deploy floor**: every own-main "Cast nothing" this corpus had no castable creature on the row
  list (`152 vs125` s17/s21/s23: hand Fateful Absence + Huntmaster at five lands; `152 vs162` s22
  after the Tracker; `152 vs126` s21 after Aspirant + Spellbinder). 0 breaks.
- **Katilda counter (152-C)**: 1 take, `152 vs126` s49 in Main 2 - compliant; Upkeep/Main-1 takes
  0 (s11 `152 vs123` passed). Fateful-Absence-own: 0 casts. Land drops: offered-taken every window.
- **Mulligans**: `152 vs125` s1 0 lands ship, s2 (keeping 6) 2 lands / not-cover ship, s3 (keeping
  5) 3 lands keep, bottomed Teferi + Sigarda (order: the lands and the cheap spells stay) - won T21;
  `152 vs146` s1 0 lands ship, s2 (keeping 6) 2 lands covers three cards keep, bottomed Huntmaster.
  Both by the book; no floor, no OWNER QUESTION.
- **Idyllic Tutor at s20** was taken over Tribute to Hunger - right under the new 152-H line.
