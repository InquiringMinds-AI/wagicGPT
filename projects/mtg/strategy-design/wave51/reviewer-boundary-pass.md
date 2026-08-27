# Wave-51 reviewer boundary pass (deck reviewer lane)

Scope: the eight boundary findings B1-B8 in `wave51/synthesis-notes.md`, adjudicated by the
reviewer of each deck (reviewers own guides; synthesis only reports) and applied to the LIVE
guides in `projects/mtg/bin/Res/ai/baka/` (the deployed wave-51 reviewer revisions, commit
`5d0c24d28`). Governing amendments: **153** (one shippable property per mulligan section - the
engine's land count and coverage line; every other property is a bottom-ask question in odds
form; (b) carry step on any kept second property; (c) the thirteenth hand, 2 lands / no
coverage), **131** (twelve-hand self-consistency check), **152(a)** (list-answer scoring:
reply numbers vs `chosen_text` before any PLAN is read), **154** (same-wave re-key when a lane
adds a tag), **155/156/157/161** as cited per finding.

Facts that landed AFTER synthesis and were used: lane I merged (`4bdca64bf`) - the AI seat now
gets a Cast row on the opponent's turn with an empty stack (D5 shipped). No guide sentence in
B1-B8 keys to "instants only on your own turn"; deck125's counter rules key to the stack line,
which is unchanged. Nothing to retire in this pass (the core-prompt sentence the synthesis
called false is the core prompt's, not a guide's). Lane H merged (`d58aeba6b`) - one
Battlement taps for a cost one covers (D4 shipped); deck126's mulligan section says "two black
pips that the Battlement cannot pay", a card fact independent of the tap order - still true.

Every cited line was re-read in the live file before the verdict; corpus literals were re-read
from `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260827-155545/` (36 game files). ASCII only;
`git diff -- projects/mtg/bin/Res/ai/baka/ | /usr/bin/grep -c $'\357\277\275'` = **0**; a
`[^ -~\t\n]` sweep over all seven guides returns no file. No `src/` change, no commit, no wagic
run.

## Verdicts

| # | verdict | reason |
|---|---|---|
| B1 | **ACCEPT - six of seven guides edited; deck130 PASS** | The #153(a) audit was run on every section (property list per guide below). The synthesis named deck125/deck130 as the unchecked holdouts; deck130 is clean (land count + coverage line only; "zero lands or all lands" and "acts by turn three" are the same property). The second shippable property the audit actually found lives in the ONE-LAND branch of FOUR guides (deck123, deck146, deck152, deck162) - the branch #153's own source case (146-M) never touched: a keep/ship decided by what the spells COST or which card is covered ("nearly all two-mana cards AND that one land makes the colour", "judgment call on that card: keep when that card wins the early turns", "judgment call on those exact cards", "nothing castable before turn four"). deck152's header (:11) already says "not a two-drop, not a curve: those are bottom-ask questions" and its own branch contradicted it. All four converted to the coverage line in odds form (the deck's real land count -> the second land inside two draws; the fresh seven that is a one-lander or worse), card identity and cost named as "not a reason". deck125: the no-coverage ship condition "and they have a creature on turn two" is a property not on the header at look time - converted to the pool fact W27 already checked. deck126: "six lands and one spell is a keep if the spell is a wall or the Tutor" implied a ship on spell identity - made a keep with the odds. #153(b) carry step: no second property is KEPT anywhere after this pass, so no carry check is owed; #131/#153(c) thirteen-hand check run on the six edited sections (below) - no two sentences disagree. |
| B2 | **PASS (no edit), as instructed** | deck162:412-420 re-read: "A row whose K names only Liliana's Caress counts with their hand at 7 or more" and "(a lone Caress only with their hand at 7 or more)" - keyed to the `{feeds:}` K sentence (prints at all six deck162 seats this corpus: 5/8/10/8/3/7 rows). Cut when D26's qualifier is on the row (#154). |
| B3 | **PASS (no edit), as instructed** | deck130: the "ability rows print NO result" paragraph is GONE from the live file (grep "NO result" = 0; :196 now reads "EVERY damage row prints its result") - D51-2 landed. deck123:67-71 names `[from exile]` ("means castable; the number in front of it is the price"); the literal prints 5 times at `123 vs152`. Both wave-52 re-keys (D11 -> `[from exile - your card, exiled by ...]`; D10 -> `[finds: <half>]` at deck123:369/:664 and deck126:317/:516) go on the wave-52 reviewer brief's #136 checklist - not this pass. |
| B4 | **PASS (no edit); caveat CONFIRMED** | deck152:330-343 and :558-559 re-read - all three Lair forms quoted. Corpus: `[Upkeep offer:` appears in the `prompt` field 8 times across the deck152 seats and 0 times in `options_text` / `chosen_text` - the reviewer's grep must read `prompt`, exactly as the synthesis says. Re-key the "then tapped ... in Main 1" sentence (:342-343) to D13's `- it was animated this turn` when it lands. |
| B5 | **PASS (no edit), as instructed** | deck125:91 re-read: the Scatter bullet is the wave-50 B6 wording (keyed to `(creature P/T)` on the stack line) with no per-opponent scope; W27's pool fact holds by decklist (every pool deck runs creatures). Corpus note, not a finding: deck125 saw an opponent creature spell on its stack line at 4 of 6 seats (146: 6, 123/152/162: 1 each, 126/130: 0) - the 126/130 zeros are windows not offered, not a decklist fact. The old `[creature]` literal is not re-filed. |
| B6 | **PASS (no edit)** | deck126:307-314 re-read: "Sanguine Bond, Exquisite Blood or Idyllic Tutor is on your hand line AND your 'Mana available:' line reads 4 or less. Then the Lantern is the fifth mana the enchantment needs" - the next sentence already states the fifth mana is for the ENCHANTMENT, and the two cited wins (Blood at 4 sources, Bond at 3) are the shape. The optional parenthetical was not taken: the reviewer judges the "Tutor in hand -> Lantern at 2 sources" misread unlikely against "reads 4 or less" plus "fifth mana the enchantment needs"; re-open if a corpus shows a Lantern over a wall at <= 2 sources with only a Tutor in hand. |
| B7 | **PASS (no edit); obligation carried** | deck123:672-675 (annihilator: lands first, tapped first; Alarm and creatures last) and deck146:80-84 (a land only once the battlefield prints four or more lands) re-read - list-answer orders in emit form. Corpus: `Sacrifice with Emrakul` asks exist only at `123 vs125` (53 rows) - the wave-52 seat runs #152(a) (reply numbers vs `chosen_text`) on those BEFORE scoring the order; D3's parser can make a correct PUT look like a broken one. |
| B8 | **PASS (no edit), as instructed** | deck130:312-316 ("Talisman pays: life <L> -> <L-1>" in the PLAN at 3 life or less) and :572-574 (keyed to `{leaves 0 of your 1 untapped mana source}` + the sources line) re-read; the literal prints at four deck130 seats this corpus (4/2/2/4). Re-key to D7's `{pays {r} with Talisman of Impulse: ...}` when it ships and cut the derivation (#154). |

## B1 - the #153(a) property list, per guide

| guide | shippable properties found (beyond land count / coverage) | action |
|---|---|---|
| deck123 | ONE LAND branch: spell cost ("nearly all two-mana cards") AND colour ("that one land makes the colour they need") | converted |
| deck125 | no-coverage ship gated on "they have a creature on turn two" (opponent property, not on the header) | converted |
| deck126 | six-land keep gated on spell identity ("if the spell is a wall or the Tutor" - the implied else is a ship) | converted |
| deck130 | none | PASS |
| deck146 | ONE LAND + coverage: card identity ("keep when that card wins the early turns by itself, ship when it does not") | converted |
| deck152 | ONE LAND branch: spell cost / card identity ("judgment call on those exact cards ... ship it when they do not"); contradicted its own :11 | converted |
| deck162 | (keeping 6) ONE LAND ship on spell cost ("nothing castable before turn four"); the punisher-in-hand KEEP exception is keep-widening, not shippable - left | converted (ship clause only) |

Land-odds numbers written (hypergeometric, 60-card deck, one land in a seven, next two draws):
21 lands -> 0.62 ("three times in five"), 24 -> 0.68 ("two times in three"), 25 -> 0.71 ("seven
times in ten"); fresh seven with <= 1 land: 21 -> 0.22 ("one in five"), 24 -> 0.14 ("one in
seven"), 25 -> 0.12 ("one in eight"). deck126's "36 of the 53" = 60 - 24 lands, minus the seven.

#131 + #153(c) check on the six edited sections (0 lands / 1 no-cov / 1 cov / all lands /
2 no-cov at keeping 7, 6, 4): deck123 - STEP 1 owns keeping <= 5 (any land = keep), STEP 2
owns 7/6 (1-cov keep, 1-nocov ship, 0/all ship, 2+ keep); no overlap. deck146 - same split
(STEP 1 <= 5; STEP 2 7/6), 2-nocov falls under "KEEP a 7 or a 6 with 2-5 lands". deck152 -
RULE -1's M-is-6 line and the new ONE LAND bullet return the same verdicts at 6 and 7; 2-nocov
= "TWO OR MORE LANDS AND AT LEAST ONE SPELL: KEEP". deck162 - untouched seven ships a one-lander
on land count alone, (keeping 6) on coverage, (keeping 5) keeps: three N, three rules, no hand
gets two verdicts at one N (the untouched-seven land-count ship is the section's standing
rule, outside this pass). deck125/deck126 - wording-only changes inside one branch each.

## Edits applied, per guide

| guide | before -> after | byte delta |
|---|---|---|
| `deck123_strategy.txt` | :611-614 *"ONE LAND: compare the two sides out loud. A one-land seven is a keep only if it is nearly all two-mana cards AND that one land makes the colour they need. A one-land seven whose spells cost three and four - two Intruder Alarms, Idyllic Tutor, Bloodline Keeper - does nothing for four turns and ships."* -> *"ONE LAND: read the coverage line. "would cover the cost of:" naming a card is a keep - the second land is in your next two draws about two times in three (23 lands in 53), and the ship buys a fresh seven that is a one-lander or worse about one time in seven. "would not cover any spell in it" ships on the untouched seven: nothing happens until a draw. Colour and what the spells cost are not part of this call - a one-land seven of two Intruder Alarms, Idyllic Tutor and Bloodline Keeper prints "would not cover", and that line is the reason."* | 60,924 -> 61,173 (**+249**) |
| `deck125_strategy.txt` | :467 *"ship it when the answer is "four or more turns and they have a creature on turn two"."* -> *"ship it when the answer is "four or more turns" - every deck in this pool casts creatures, and the cheap ones come down on the first turns, so that wait is a threat you cannot answer; their turn two is not on the header and is not a reason either way."* | 47,835 -> 48,009 (**+174**) |
| `deck126_strategy.txt` | :430-431 *"six lands and one spell is a keep if the spell is a wall or the Tutor."* -> *"six lands and one spell is a keep - which spell it is is not a reason to ship (36 of the 53 cards left are spells, and a bottom ask, when there is one, sheds the extra lands)."* | 43,331 -> 43,440 (**+109**) |
| `deck146_strategy.txt` | :28-30 *"ONE land under "would cover the cost of:" naming a card is a judgment call on that card: keep when that card wins the early turns by itself, ship when it does not."* -> *"ONE land under "would cover the cost of:" naming a card is a keep: the second land is in your next two draws about three times in five (20 lands in 53), and the ship buys a fresh seven that is a one-lander or worse about one time in five. Which card it covers is not a reason either way."* | 52,658 -> 52,784 (**+126**) |
| `deck152_strategy.txt` | :446-452 *"ONE LAND: weigh it on what the spells cost. One land with a hand of THREES and up ... is a ship ... One land with three or four TWO-drops ... is a judgment call on those exact cards ... Take that keep when the cheap cards themselves win the early turns; ship it when they do not. Colour is not part of this call - see THE FORBIDDEN THOUGHT below."* -> *"ONE LAND: read the coverage line. "would cover the cost of:" naming a card is a keep - the second land is in your next two draws about seven times in ten (24 lands in 53), and the ship buys a fresh seven that is a one-lander or worse about one time in eight. "would not cover any spell in it" ships on the untouched seven: nothing happens until a draw, and that is as true of a hand of two-drops (Katilda, Luminarch Aspirant, Ranger Class) as of a hand of threes and up (Sigarda, Tovolar's Huntmaster, Teferi) - the ship costs one card for the same land odds either way. What the spells cost is not part of this call, and neither is colour - see THE FORBIDDEN THOUGHT below."* | 47,813 -> 47,913 (**+100**) |
| `deck162_strategy.txt` | :344-345 *"ship it when the hand is one land and nothing castable before turn four."* -> *"ship it when the coverage line reads "would not cover any spell in it" and no punisher is in the hand."* | 34,979 -> 35,009 (**+30**) |
| `deck130_strategy.txt` | untouched (B1 PASS; B3/B8 PASS as instructed) | 0 |

**Pool total: +788 bytes across six guides.** Every edit keeps the section's structure and the
rule's action; only the second property was replaced by the coverage line in odds form.

## Cross-guide KEY diff (#144)

- One-land branch keyed to the coverage line `"would cover the cost of:"` / `"would not cover any
  spell in it"`: now deck123, deck125, deck126, deck130, deck146, deck152; deck162 keys it at
  (keeping 6) only and ships a one-land UNTOUCHED seven on land count alone (its own standing
  rule, "Your cheap plays ... none of them advances the plan"). Holdout by key: **deck162**'s
  untouched-seven line - not a #153 defect (land count is the one property), flagged for its
  reviewer as the pool's only coverage-blind one-land ship.
- Spell-cost / card-identity words in a mulligan branch as a reason: zero guides after this pass
  (deck152's "no one-drop is never a reason", deck162's "no cheap play is never a reason" remain
  as negations - correct form).
- `[Upkeep offer:` literal: deck152 only (prompt field). `{feeds:}`: deck162 only. `{leaves 0 of
  your 1 untapped mana source}`: deck130 only. `[from exile]`: deck123 only. No holdouts.

## Not verified / notes forward

- B1: the six conversions are derived, not corpus-tested; the wave-52 seats score the first
  one-land looks under the new wording (metric: keep/ship agrees with the coverage line, N/N).
- B1: the odds numbers are the reviewer's arithmetic (hypergeometric on the printed land
  counts), rounded to the nearest common fraction - marked inference.
- B1: deck125's "every deck in this pool casts creatures" is W27's pool check restated; "the
  cheap ones come down on the first turns" is the reviewer's reading of the decklists, not a
  corpus count.
- B3/B4/B8: three re-keys owed the wave D10/D11/D13/D7 ship (#154) - the wave-52 reviewer brief
  carries them; nothing here is keyed to a string that does not print today.
- B5: 126/130 zeros are "no counter window offered at that seat", not a decklist fact.
- Lane I (D5): no guide sentence describes opponent-turn casting as impossible; the first
  corpus with opponent-turn Cast rows is the one to grep every counter/instant rule against
  (deck125 especially: its rules assume the stack line is non-empty when a counter row is
  offered - an empty-stack Cast row on their turn is a NEW row family for #139's table).
