# Wave 65 - deck146 (Orzhov dungeon midrange) per-deck review

Corpus: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-033121/`
Seat files: `*-ai_baka_deck146-0x*-vs-*.jsonl` (6 games, 2-4). All 6 ended naturally; 0 fallbacks,
0 re-asks, 0 transport errors, max `deadline_pct` 66.2 on this seat.

## 1. Game by game

**vs152 - LOSS 0-20, turn 18** (`...494-...deck146-0x55a3f53451d0-vs-...152`)
Even game through turn 15 (Soul Shatter took Teferi at seq 34; Kaya -3 exiled Briarbridge Tracker
at seq 42). Decided at **seq 40**, turn 17 UPKEEP, 7 life vs their 20: the seat answered
"becomes beholder" with Hive of the Eye Tyrant for {3}{B}. The guide's CHECK 1 says Upkeep +
CHECK 0 not fired -> PASS, and the reply wrote no CHECK 0 sum. Then at **seq 43** it cast
Triumphant Adventurer off a row that printed
`{paying this taps: Hive of the Eye Tyrant (animated this turn) - it cannot attack this turn}`
and still wrote "PLAN: ... Attack with Hive (menace) and Triumphant Adventurer". The Hive was
auto-tapped for the {b}, no attackers ask fired that turn, and the 4 mana bought nothing. STRATEGY
both times - the surface was truthful and explicit.

**vs130 - LOSS -1/16, turn 16** (`...498-...deck146-0x5576139bb1e0-vs-...130`)
Not a decision loss. Kept a 3-Plains seven, cast Nadaar (seq 6), attacked, and then took two Molten
Rains (turns 6 and 10) plus Starstorm. From turn 9 on, every card in hand was {B} or {B}{W} and the
seat never saw a black source: 8 asks in 16 turns, both later ones cleanup discards (seq 11, 12).
Land destruction + colour screw, not guide effect.

**vs125 - LOSS 0-52, turn 39** (`...502-...deck146-0x559a2af59330-vs-...125`)
Ahead 20-11 at turn 16. Lost to Supreme Verdict (turn 20), two Staff of Nin, and Sphinx's
Revelation (turn 24, +8 and 8 cards). Every threat after the wipe was countered one at a time -
Acererak seq 45/81, Nadaar seq 51, Silverquill Command seq 82, Vanishing Verse seq 83 - but the
hand was one card deep at each of those windows (seq 46 and 52 both print "Your hand (1 card)"),
so the guide's existing "ONE SPELL A TURN INTO OPEN BLUE MANA LOSES" rule had no second spell to
offer. One clear misplay: **seq 84 and 85**, turn 38 at 2 life, the land drop offered
`1. Play Hive of the Eye Tyrant [enters TAPPED - it makes no mana this turn ...]` - its only
remaining threat, free, no cost - and the seat answered "Play no land right now" twice.

**vs126 - WIN 17/-9, turn 27** (`...508-...deck146-0x561e625af1e0-vs-...126`)
Clean guide game: removal on their best permanent on time (Soul Shatter seq 7, 30; Kaya -3 on
Overgrown Battlement/Perimeter Captain seq 11, 25, 48), two walkers deployed, two dungeons
completed, five-creature alpha strikes seq 51/65. One defect-driven slip at **seq 61/62** (below).

**vs162 - LOSS 0-14, turn 17** (`...520-...deck146-0x560142fdb270-vs-...162`)
Underworld Dreams + Ob Nixilis + Dictate of Kruphix drained the seat 2-4 a turn. The Soul Shatter
on Ob Nixilis in response to the draw triggers (**seq 25**) was correct and well-read. The turn
that lost it is **seq 23/24**, turn 11 main 2 at 18-17: the cast row named the live modes
(`creature gains 3/3 and you draw, creature gains 3/3 and opponent draws`) and listed
`opponent draws and sacrifice creature` on its DEAD side; the mode ask then offered that dead pair
with a live-looking price and the seat took it - 4 mana, tapping out, for 1 damage and a free card
handed to a draw-punisher deck, with Soul Shatter left uncastable. Ob Nixilis landed next turn and
killed Barrowin.

**vs123 - WIN 22-0, turn 11** (`...522-...deck146-0x55da9511e210-vs-...123`)
Textbook: turn-3 Adventurer, ventures every combat, Soul Shatter on Thraben Doomsayer the turn it
appeared (seq 14), three-creature attacks from turn 9. No misplays found.

## 2. Engine / interface / card items

**HIGH-1 - the Silverquill Command mode ask contradicts the cast row's own mode census, and prices
half-dead pairs as whole ones.** Repro (two games, both times the seat then took a dead pair):
- `1788683520-ai_baka_deck146-0x560142fdb270-vs-ai_baka_deck162.jsonl` seq 23, cast row:
  `{modes live right now: creature gains 3/3 and you draw, creature gains 3/3 and opponent draws;
  dead (no legal object right now): ... you draw and sacrifice creature, opponent draws and
  sacrifice creature}` (true: their board had 0 creatures, the seat's graveyard 0 creature cards).
  seq 24, the very next ask, prints all six pairs with no dead marking, and row 6 reads
  `6. opponent draws and sacrifice creature {this mode right now: they LOSE 1 life - they would be
  at 16; they draw 1 ...}`. Rows 1 and 4 - also dead - carry no annotation at all, so on this menu
  "no tag" means both "dead" and "unpriced". Taken; the sacrifice half did nothing.
- Same pair of windows in `...508-...deck146-...-vs-...126` seq 61 -> seq 62 (there with SEVEN of
  the seat's creatures on the board, so the live `creature gains 3/3` pump was thrown away on an
  alpha-strike turn).
Fix shape: the mode ask should carry the same live/dead verdict the casting row already computed
(one census, both seams - the AL lane's `modeRowAnnotations` is where it lives), and mark the dead
half of a pair it prices.

**HIGH-2 - the mode ask's life forecast ignores the opponent's DRAW PUNISHERS printed in the same
prompt.** `...520-...vs-...162` seq 24: the prompt's own block says "Every card YOU draw costs you
1 life to theirs. They fire on EVERY draw - ... a draw spell, any extra draw", yet rows 2 and 5
(`you draw`) forecast `you LOSE 1 life - you would be at 17` from 18. The true result is 16
(1 from the Command, 1 from their Underworld Dreams). The machinery exists and is applied in the
other direction on the casting row - `[DRAW GRANT (one-shot, on resolution): ... converters on
your battlefield: 0 ...]` - but not to your own draws. No window in this seat's corpus executed the
row, so the number was never falsified in play; it is a false number on the screen either way.

**MED-1 - land-drop rows price mana only, never that the land is a threat.**
`...502-...vs-...125` seq 84 (and identically seq 85), turn 38, 2 life:
`1. Play Hive of the Eye Tyrant [enters TAPPED - it makes no mana this turn (you control 12 other
lands): ... - it taps for mana from your next turn on]` / `2. Play no land right now`. Everything
the row says is about mana, and at 12 lands mana is worthless; nothing says this land is the only
attacker the deck has left. Declined twice. A manland row that named its animation cost and body
(as the "becomes beholder" row does) would have carried the whole decision.

**MED-2 - answers keep arriving with the decision re-argued past the coded line.** 9 records on
this seat carry `later_answer_ignored`; 4 also trim the reply (`reply_trimmed_bytes` up to 4039 at
`...vs-...152` seq 40, with `post_answer_overrun` 5763). Adjudicating the AO lane's question for
this deck: **0 of 9 windows would have been better served by the later coded line.** Six are byte-
identical repeats of the first answer (`...vs-...125` seq 21, 59; `...vs-...126` seq 31, 37, 58;
`...vs-...126` seq 60), two have only one coded line inside a long prose dump (`...vs-...152` seq
40, 41), and the one real conflict went the right way: `...vs-...152` seq 35, first line
`CHOICE: 1 (+1: don't target any creature with Kaya the Inexorable)`, later line
`CHOICE: 3 (Hold priority...)`. First-wins took the +1, which is what put Kaya at 3 loyalty and
paid for the -3 that exiled Briarbridge Tracker at seq 42. First-wins was strictly better there.

**LOW-1 - `{modes live right now: ...}` is printed on the cast row but the mode ask arrives as a
separate `ask` record with no link back.** A reader (model or reviewer) has to hold the previous
window's census in memory across a record boundary; the mode ask's `prompt` re-renders the whole
board but drops the one clause that priced the decision.

**LOW-2 - card facts checked, all correct.** Silverquill Command's nine `auto=choice` pairs in
`bin/Res/sets/primitives/borderline.txt:102739-102749` match the printed `text=` and the rendered
row labels; `opponent draws and sacrifice creature` really is `draw:1 opponent && life:-1 opponent
&& ... sacrifice`, so the engine's execution at vs162 seq 24 (opponent drew, lost 1, sacrificed
nothing) was correct - only the *pricing* was misleading. Vanishing Verse's target list at
`...vs-...125` seq 83 correctly offered only the monocolored Lightmine Field and excluded the
colorless Staffs of Nin.

## 3. Guide verdict: EDIT (three changes, net +11 bytes, 70,986 -> 70,997)

Written to `wave65/deck146/strategy.txt`. The guide is at the top of the pool band, so every
addition is paid for by a deletion of something this corpus proves stale.

1. **line 762-763.** BEFORE: "The Command row carries no mode annotation at all, so the graveyard
   branch is a name search in the log, never a clause you wait for on the row."
   AFTER: "The Command row prints its own mode census (\"{modes live right now: ...; dead ...}\") -
   read it, not the log, for which pairs are live."
   PAID BY: `...vs-...162` seq 23 and `...vs-...126` seq 61 - both cast rows print the census, so
   the sentence is false today, and it contradicts entry 5 fifteen lines earlier, which tells the
   model to read exactly that clause.

2. **entry 5, new paragraph** (and the removal of "This deck owns exactly TWO creatures at mana
   value 2 ... printed the return pairs on the live side", which the two sentences around it
   already say). AFTER, added: "THEN THE MODE ASK RE-LISTS THE DEAD PAIRS, UNMARKED. \"Choose an
   option for Silverquill Command\" prints every pair again, dead included, and prices only the
   half it CAN price (\"{this mode right now: they LOSE 1 life...}\"), so a half-dead pair reads
   whole. CARRY THE CAST ROW'S DEAD LIST INTO THAT ASK - `vs162` seq 23/24 and `vs126` seq 61/62
   both took a dead pair off its priced half. Never take an \"opponent draws\" pair while another
   pair is live: it buys 1 damage with a card for them."
   PAID BY: `...vs-...162` seq 23/24 (the turn that lost that game) and `...vs-...126` seq 61/62.
   This is the guide half of HIGH-1; it should be deleted again once the mode ask carries the
   census itself.

3. **lines 149-153.** BEFORE: "The pay row does not warn you about that; \"Your life:\" is printed
   two lines above the menu and reading it is your job. / REMOVE THIS FLOOR WHEN the \"pay 3
   life\" row prints its own life verdict - re-verify by counting ... it was 0 of 12 such rows
   ... in wave 57, and the docket asks for that verdict (D1)."
   AFTER: "THE ROW NOW PRINTS THE RESULT - \"{this payment puts you at N}\" (`vs125` seq 22,
   `vs152` seq 31). Read that N; answer \"tap\" unless (a) and (b) above both hold."
   PAID BY: `...vs-...125` seq 22 `1. pay 3 life - Agadeem, the Undercrypt enters UNTAPPED ...
   {this payment puts you at 17}` and `...vs-...152` seq 31 (`{this payment puts you at 16}`) -
   2 of 2 such rows in this seat's corpus now carry the verdict the guide was waiting on, so the
   standing re-verification instruction is answered and the "the row does not warn you" clause is
   false.

**Not edited, on purpose.** The two other real misplays this corpus contains are rules the guide
already states and the seat did not follow: the upkeep Hive animation (`...vs-...152` seq 40)
violates CHECK 1 verbatim, and the declined land drop (`...vs-...125` seq 84/85) violates "Play a
land every turn you can." Restating either would add bytes to a guide at its ceiling without
adding an instruction.

## What I did NOT check
The opponent seat files were read only for board context, not audited. I did not adjudicate the
lane predictions (engine seat's job), did not measure G7 re-offer/decline churn, did not open the
`.stderr` files, and did not look at the 162v123 -1089 loop. Card facts were checked against the
primitives only for Silverquill Command, Vanishing Verse, Underworld Dreams and Hive of the Eye
Tyrant; no Scryfall lookups were made, so no engine-vs-Oracle divergence is claimed here.
