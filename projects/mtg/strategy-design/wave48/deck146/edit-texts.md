# deck146 - wave-48 edits (5), before -> after, each with its finding

Baseline: the LIVE guide `bin/Res/ai/baka/deck146_strategy.txt` (41,918 bytes = wave-47 reviewer
revision + boundary-pass B7). After: `wave48/deck146/strategy.txt`, 44,111 bytes (**+2,193**).
0 whole-rule deletions; two paragraphs collapsed (146-A, 146-D). ASCII only.
Corpus `matchups-20260827-072004`. Card facts re-verified against `bin/Res/sets/primitives/*.txt`
with `/usr/bin/grep`: Sanguine Bond `auto=@lifeof(player) from(*[-lifefaker]|*):life:-thatmuch
opponent`; Pride Guardian `auto=@combat(blocking) source(this):life:3 controller`; Hive of the Eye
Tyrant `auto={3}{B}:name(Becomes beholder) transforms(...) ueot`.

---

## 146-A (HIGH) - the converter rule at its THIRD identical failure -> amendment #102 routing
**Finding:** `146 vs126` attackers s36, s48, s60, s72: **7 priced A-rows offered with
`LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine Bond` printed, 7 sent.** Life
between the attackers record and the next: 19->18, 16->12, 8->7 and at s72 **4 -> -2** (dead on
its own attack: Pride Guardian + Vampire blocked, 3 + 1 off it, then the drain). The lane-P clause
`most expensive to attack into: Pride Guardian (0/3) (... their converter takes 3 off you)` was
on the s72 screen and the reply named it and sent anyway. Replies, verbatim: s36 *"they gain 1
life which Sanguine Bond makes you lose 1 life, net 0 change but you remove their lifelink
source"*; s48 *"they gain 3 life which is drained from them via Sanguine Bond, netting us 0 life
change"*. Wave 46: 25 life to two walls. Wave 47: 11 + 5 (s50/s59). Wave 48: 7/7 and a death.
The rule was correct, restated in DECIDING SITUATIONS, keyed to the printed strings, and the
render was complete. CLASSIFICATION: **STRATEGY, third failure** -> per #102: shrink, and move
the check onto the pilot's own plan sentence, quoting the corpus's wrong sentences; refute the
false belief (the pilot thinks the converter drains THEM).
- **before:** two paragraphs, `A CONVERTER OF THEIRS TURNS EVERY ONE OF THOSE PAYMENTS INTO
  DAMAGE ON YOU ...` and `AND THE PRICE YOU CAN SEE IS NOT THE PRICE YOU PAY WHEN THE TAG IS
  COLLAPSED ... price the swing as paying you out for EVERY defender on that line`.
- **after:** one paragraph stating the direction in the fold's own words (`"their converter takes
  N off you". Their life goes UP by N and yours goes DOWN by N. That is not a wash, it is not "net
  0", and nothing is "drained from them"`), plus **CHECK YOUR OWN PLAN SENTENCE BEFORE YOU WRITE
  THE ATTACK LINE** naming "net 0", "netting us 0", "net zero", "they gain N which Sanguine Bond
  drains from them", "the game ends before that damage matters" as sentences that make the plan
  wrong, plus the collapsed-tag paragraph re-keyed to the lane-P string `most expensive to attack
  into:` (which now renders) instead of "read their battlefield line for the defenders it did not
  print". DECIDING SITUATIONS converter row rewritten to match (both branches, #98).
- **Prediction (plan-text metric first, per #102):** replies at a converter window containing
  "net 0" / "netting" / "drained from them" / "net zero": 0 (was 3 of 4 windows). Outcome half
  separately: priced rows sent with the converter up <= 1 of N (was 7/7). If the plan-text half
  fails too, the lane is exhausted at this seat and the remedy is a row annotation.

## 146-B (MED) - no last-body rule and a lone-attacker rule that went 0/3
**Finding:** `146 vs126` s14, s18, s60: one Silverquill Silencer sent into one `Wall of Omens
(0/4) (neither dies)` three times; the guide's own sentence "one attacker never outnumbers one
untapped blocker ... the answer is ATTACK: none" is in the section and did not fire. At s60 the
seat was at **8 life**, the Silencer was its ONLY creature, and the reply reads *"It deals 3
damage to Wall of Omens #1. The Wall survives (1/4 remaining)"* - a false rules belief (damage
does not persist). At s72 (life 4) both bodies were sent and none was home for their turn. deck162
carries a count-keyed last-body rule (amendment 83); deck146 does not (amendment 81 coverage
diff). CLASSIFICATION: **STRATEGY.**
- **before:** the crack-back sentence, then `- NEVER plan to block with a creature you are
  attacking with ...`.
- **after:** `TWO NUMBERED CHECKS BEFORE ANY OTHER ATTACK RULE - STOP AT THE FIRST ONE THAT
  ANSWERS. CHECK A - YOUR LAST BODY` (life <= 8, count your creatures with no `[tapped` / no
  `[summoning sick`; if ONE, it stays home; Nadaar excepted by vigilance) and `CHECK B - ONE
  ATTACKER, EVERY NAME "(neither dies)"` -> ATTACK: none, with the "leaving it at 1 toughness"
  belief refuted. Placed ABOVE the existing prose (amendment 70).
- **Prediction:** only untapped creature sent at life <= 8: 0 (was 1). Lone all-`(neither dies)`
  attacker sent: 0 of N (was 3/3).

## 146-C (MED) - eight mana on two Hives, zero attackers
**Finding:** `146 vs125` s63-s65 (T26, life 13 vs 35). s63 animated Hive #1 in Main 1 (fine).
s64: battlefield line `Hive of the Eye Tyrant #1 (3/3) [menace ...]; Hive of the Eye Tyrant #2
[tapped]`; sources line `... Hive of the Eye Tyrant #1 {b} ...` as the only black source; the
reply first wrote `CHOICE: 0 (pass)` and then `CHOICE: 2 (becomes beholder with Hive #2)`, which
E1 took. s65: `Hive #1 (3/3) [tapped - cannot attack or block this turn]; Hive #2 (3/3) [tapped -
cannot attack or block this turn]`, `0 of them able to attack`, `Mana available: 1`. The engine
paid Hive #2's {B} with Hive #1. No attackers record that turn. CHECK 2 asks "is it already a
creature"; nothing asks "is it tapped" or "what pays". CLASSIFICATION: **STRATEGY** (the tapped
tag and the sources list were printed) with a render half filed as E-2.
- **before:** CHECK 2 ends `... an anthem makes that number bigger than the printed 3/3.`
- **after:** + `CHECK 3 - IS THAT HIVE TAPPED, AND WHAT PAYS FOR IT?` - entry reads `[tapped]` ->
  PASS; only `{b}` source on the `Those sources` list is a Hive already `(3/3)` -> PASS; a second
  Hive only when a Swamp or Snarl is still on the sources list.
- **Prediction:** animation of a Hive whose entry reads `[tapped]`: 0 (was 1); two Hives animated
  in one turn: 0 (was 1).

## 146-D (MED) - Acererak over Command seven times on dead boards; "there is always a playable pair of modes" was false and the pilot said so
**Finding:** `146 vs125` s8, s11, s33, s38, s40 (Acererak over Command), s21 (over Nadaar), s60
(over Pelakka) - all with their line at `0 creatures`, own line at `0 creatures` and no MV<=2
creature in the graveyard. s47's reply (an E1 window) writes the argument out: *"Command's only
useful mode is drawing a card. Is drawing a card better than venturing? ... Acererak is PURELY a
venture engine"*. Wave 47: 13 declines of the same card. Amendment #84: a threshold disobeyed
underneath and unpunished is inert -> replace with a condition with evidence; #16: author the
release condition with the rule; #96: the card section points, it never restates.
CLASSIFICATION: **STRATEGY.**
- **before:** entry 5 `Silverquill Command (see its own rule for the modes).`; card section
  `THE TEST IS THE OPTION LIST, NOT THE BOARD ... There is always a playable pair of modes ...`;
  DECIDING row `... cast it. There is always a playable pair of modes.`
- **after:** entry 5 carries three screen-readable conditions (their line shows a creature / your
  line shows a creature / the row's `{right now:` names a card it returns) and a new **entry 8b**
  (Command with none true) below Acererak and above "nothing"; the card section says WHEN is
  entry 5's and keeps only the HOW (modes, timing, target, draw); the DECIDING row restates the
  three conditions. The "always a playable pair" sentence is deleted.
- **Prediction:** Acererak over a Command with a live condition: 0. Command cast when a condition
  is live: >= 80% of offers (was 5 of 15 overall).

## 146-E (LOW) - the accountability example is being copied verbatim onto menus that have no Command
**Finding:** `146 vs125` s83 and s87 answer `Cast nothing right now` with *"The Command is entry
5, their board is empty and I control no creature, so both of its useful mode pairs do nothing"*
- the guide's own worked example - on menus of Soul Shatter / Acererak / Agadeem's Awakening.
Amendment 1 in a new shape: a quoted sentence becomes the reply. CLASSIFICATION: **STRATEGY.**
- **before:** the sentence in quotes.
- **after:** a fill-in shape with three blanks (`entry <number> <card name> was on this menu; it
  did not match because <a fact printed on this screen>`) and the line "a sentence about a card
  that is not on the menu is not the work".
- **Prediction:** replies quoting "the Command is entry 5" on a menu with no Command row: 0 (was 2).

---

## NOT edited, and why
- **MULLIGAN STEP 1 / bottoming floor:** `146 vs152` seq 3 broke STEP 1 at `(keeping 5)` with a
  Swamp in hand ("no white source") and seq 6 bottomed all four spells from a `(keeping 3)` keep.
  Per the wave-48 brief this is an OWNER QUESTION (Q1 in the seat file); the floor and the
  bottoming sentence are byte-identical to the live guide.
- **`146 vs123` seq 1** shipped a 2-land seven outside STEP 2's four reasons and won; n=1 (#66).
- **`becomes beholder` in Upkeep: 0 of 58 taken** - wave-47 edits 146-C/E hold; not re-touched.
- **Lolth 0 at life <= 6: 0 of N** - floor holds.
- **Land drops 168/168, Verse own-only 6/6, Agadeem's 0/14, Lost Mine 10/10** - nothing to fix.
- **`146 vs123` s24** Soul Shatter cast into 0 creatures with a plan saying it is useless
  (answer/plan contradiction, one window) - the DECIDING row already forbids it; not restated.

## 146-F (HIGH, decided after the owner ruling) - the STEP 1 hard floor and the bottoming floor become odds reasoning
**Finding:** `146 vs152` seq 3 (`(keeping 5)`, one Swamp, "no white source" -> mulligan), seq 4-5
(chain to `(keeping 3)`), seq 6 (bottomed Pelakka, Kaya, Soul Shatter x2 from a 3-land keep,
plan said "cast Kaya"); lost T13 with no cast before T10. The floor was an imperative and the
model argued past it with the one clause it excluded. Owner constraint: no code-like floors.
- **before:** `STEP 1 ... If it says (keeping 5) or ANY lower number: KEEP, if the hand holds at
  least one land ... you are DONE`; bottoming: `NEVER bottom a land ... unless ... at least 4
  lands - AND NEVER bottom your way to a hand with zero spells`; two DECIDING rows.
- **after:** `STEP 1 - WHAT DOES THE NEXT LOOK COST?` - each look is a fresh seven shaved by one
  more card at the same land odds (21 lands, nine dual-colour), so at (keeping 5) or lower a
  one-land hand plus the bottom ask beats the ship, colour is not a reason, zero lands is the
  one case to weigh; the bottom ask rewritten as building the best N (`You will be left with an
  N-card hand`: lands that cover the cheapest spells, those spells, expensive cards under; never
  all lands; at N=3 two lands and the cheapest spell) with the seq-6 cost quoted; DECIDING rows
  restated as reasons, not thresholds.
- **Prediction:** mulligan answers at (keeping 5) or lower with a land in hand: 0 (was 1);
  bottom answers leaving zero spells: 0 (was 1).
