# Wave-50 reviewer boundary pass (deck reviewer lane)

Scope: the seven boundary findings B1-B7 in `wave50/synthesis-notes.md`, adjudicated by the
reviewer of each deck (reviewers own guides; synthesis only reports) and applied to the LIVE
guides in `projects/mtg/bin/Res/ai/baka/` (the deployed wave-50 reviewer revisions). Governing
amendments: **143** (`PUT:` list-answer asks are an emit surface: content rule in emit form + the
excluded class at the current N + read-back against the PLAN), **136** (a new decision kind gets a
per-guide order copied from the nearest existing one), **145** (class words checked against the
primitive's filter; the guide states the delivered card fact), **131/119** (mulligan sections
unchanged this pass - the discard orders are derived from them, not edits to them).

Facts that landed AFTER synthesis and were used: lane A merged (`42f2eff2b`) - `borderline.txt:
106093` now reads `sacrifice notaTarget(*[creature;planeswalker;manacost=convertedcost:highest:
creature^planeswalker:myBattlefield]|myBattlefield)`, i.e. the Oracle fact (highest MV among
creatures AND planeswalkers), so B2 is keyed straight to the Oracle class and the "until lane A
ships" interim wording was never written. Lane B merged (`df8ff7641`) - no guide sentence
describes the dual-vs-mono tap order (#128 grep: zero hits on "dual", "Sea", "Tundra" in a
payment sense at any seat); nothing to retire.

Every cited line was re-read in the live file before the verdict; the discard ask's shape was
re-read from the corpus (`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260827-115759/`, 28 `discard`
records): the ask body reads `Cleanup step (CR 514.1): your hand has N cards and your maximum hand
size is 7, so you must discard exactly K card(s) now ... On the FIRST line write PUT:` over a
numbered hand list; the log line reads `Cleanup discard (hand N, limit 7): you chose ...`. Both
literals are quoted in the new sections. ASCII only; `git diff -- projects/mtg/bin/Res/ai/baka/ |
/usr/bin/grep -c $'\357\277\275'` = **0**; a `[^ -~\t]` sweep over all seven guides returns no
file. No `src/` change, no commit, no wagic run.

## Verdicts

| # | verdict | reason |
|---|---|---|
| B1 | **ACCEPT (all four)** | deck130, deck146, deck152, deck162 confirmed to carry NO order for the `discard` ask (grep "Cleanup"/"PUT" - only bottoming hits). Each gained one section placed directly after its own bottoming order and derived from it read from the end (#136: copied from the nearest existing order; #143: the PUT line's content rule in emit form - "the numbers of the cards you SEND, most expensive first" - plus the class that never enters it, plus the read-back-against-PLAN sentence). The excluded classes are each guide's own: deck130 the only Starstorm / the last covering land; deck146 any land while the battlefield prints fewer than four lands (the `146 vs125` six-discards-on-one-land case, cited as the fact it was), and the only removal spell while their line shows a creature or planeswalker; deck152 the only two-drop and the four named engines while a cheaper card remains; deck162 Shield Sphere and the only punisher (its KEEP order's protected pair). The synthesis's "cannot cast in the next two turns" clause was taken at deck130/deck152 (their orders are cost-and-coverage); deck146/deck162 use their named most-expensive-first lists instead, because those orders already name the cards. No floor and no hard cap: every sentence is an order, satisfiable at any K. |
| B2 | **ACCEPT, keyed to the Oracle class (lane A shipped)** | deck146:88-91 confirmed ("Soul Shatter needs a CREATURE on their line and does nothing on a board of only a planeswalker"), :161-164 ("their biggest creature ... does NOTHING if they control no creature ... only lands, artifacts or planeswalkers"), :533 ("controls no creature -> do not cast it"). All three were the pre-lane delivered fact and are now FALSE against the merged script. Re-keyed to the card fact the engine delivers today: highest-MV creature OR planeswalker among both, with the ranking made concrete (Ob Nixilis at 5 before a 4-drop; a 6-MV creature before Ob Nixilis) so the pilot can predict the victim from the printed costs. The "never" line ("the engine ignores planeswalkers") was not written; no divergence is described (#99/#103). #145's grep on the class words: `creature`, `planeswalker` both present in the primitive's filter - agrees. |
| B3 | **PASS (no edit), as instructed** | deck130:184-192 re-read: "An ABILITY'S rows ... print NO result: there you do the read yourself" - a row-family fact, the #139 shape; the Hammer rule at :253-258 and deck123's Tutor line are one-line rules. Nobody "fixes" them; D50-5 is cut the wave D6 ships. |
| B4 | **PASS (no edit), as instructed** | deck162:388-391 re-read: "A Caress already on your line with their hand at 7 or more lifts that brake: the extra cards are discards, and discards are damage." Correct with D11 unshipped; the executor reads the hand size off the opponent-hand line. Re-check the count words against the `{feeds:}` string when D11 lands (#128). |
| B5 | **PASS (no edit), as instructed** | deck152:325-334 (Lair) re-read: the two PASS forms are keyed to printed tags (`[this land is TAPPED ...]`, `[repeat: ...]`) and the Main-1 sentence is a reason ("spending it before you know what you drew" is deck146's; deck152's is "only with mana left after the creature ... only when its A-line would read [no creature they control can block]"). deck146:420-425 (Hive) re-read: CHECK 0 outranks the Upkeep pass, so an Upkeep animation stays takeable - an odds sentence, not a floor. Re-key both to D7's annotation string when it lands. |
| B6 | **ACCEPT, with one correction to the finding** | deck125:82-86 confirmed: the gate is "the cheapest counter in your hand THAT CAN HIT WHAT THIS OPPONENT CASTS" and the Scatter bullet says "CREATURE SPELLS ONLY". Primitive `mtg.txt:37073` `target=creature|stack` - agrees with Oracle; a creature spell of any other card type also carries the creature type, so the only edge is a spell that is not a creature at all. The synthesis's proposed literal `[creature]` does NOT exist on the stack line: the corpus prints `1 (top): opponent's Overgrown Battlement {1}{g} (creature 0/4) [spell]` (102 stack headers at this seat, all this shape), and the guide's own :116-117 and :485-490 already key to `(creature P/T)`. Keyed the one added clause to that print instead. Preventative. |
| B7 | **PASS (no edit), as instructed** | deck123:101-112 re-read: the subtraction-before-line-one text and the `x0` sentence ("never 'x0', which the engine carried out ONE time") are present and true; when D4 ships the x0 sentence goes redundant, not false - left. |

## Edits applied, per guide

| guide | finding(s) | before -> after | byte delta |
|---|---|---|---|
| `deck130_strategy.txt` | B1 | after :469 (*"...is the one that goes, even when it is a land or the Starstorm."*) a new paragraph: *"CLEANUP DISCARD - THE SAME ORDER, READ FROM THE END. ... you answer "PUT: n, m" - the numbers of the cards you SEND, most expensive first. Send ...: the most expensive spell your lands cannot cast in the next two turns (Rorix or a second Siege-Gang Commander from four lands), then a duplicate ..., then a land beyond the third in hand. Never the only Starstorm ... and never the last land that covers the spell your plan names. Read the PUT line back against your own plan sentence ..."* | 48,682 -> 49,602 (**+920**) |
| `deck146_strategy.txt` | B1, B2 | new bullet after :64 (*"...lost 0 to 35."*): *"- CLEANUP DISCARD IS THE SAME ORDER, READ FROM THE END. ... most expensive first: Emeria's Call, Agadeem's Awakening, a second planeswalker, then the priciest spell your battlefield's lands do not cover, then a duplicate ... A land goes ONLY when your battlefield line already prints four or more lands; from fewer, the PUT line holds spell numbers only (six discards on one land at `vs125` ...). Never your only removal spell ... while their line shows a creature or a planeswalker. Read the PUT line back ..."* · :90-91 *"Soul Shatter needs a CREATURE on their line and does nothing on a board of only a planeswalker"* -> *"Soul Shatter takes the highest-mana-value CREATURE OR PLANESWALKER on their line - a planeswalker that outranks their creatures is the card it takes - and does nothing on a board of neither"* · :161-164 *"SACRIFICES their biggest creature (highest mana value) ... does NOTHING if they control no creature - do not cast it into a board of only lands, artifacts or planeswalkers."* -> *"SACRIFICES their creature or planeswalker with the highest mana value among the creatures and planeswalkers they control - so Ob Nixilis at 5 goes before a 4-drop creature, and a 5/5 at 6 goes before Ob Nixilis. ... does NOTHING if they control neither a creature nor a planeswalker - do not cast it into a board of only lands, noncreature artifacts and enchantments."* · :533 *"the opponent controls no creature -> do not cast it."* -> *"the opponent controls no creature and no planeswalker -> do not cast it."* | 48,186 -> 49,553 (**+1,367**) |
| `deck152_strategy.txt` | B1 | after :463 (*"...a four-card hand of four lands is not a hand at all."*) a new paragraph: *"CLEANUP DISCARD IS THE SAME ORDER, READ FROM THE END. ... most expensive first: the most expensive spell your lands cannot reach in the next two turns, then a duplicate ..., then a land beyond the fourth in hand when your battlefield line already prints four or more lands. Never your only two-drop, and never Luminarch Aspirant, Intrepid Adversary, Ranger Class or Briarbridge Tracker while a cheaper card is left to send. Read the PUT line back ..."* | 45,182 -> 46,084 (**+902**) |
| `deck162_strategy.txt` | B1 | after :355 (*"...below that a land is never the right card to lose."*) a new paragraph: *"CLEANUP DISCARD IS THE SAME ORDER, READ FROM THE END. ... most expensive first: Forced Fruition, Peer into the Abyss, Teferi's Puzzle Box, a second copy of a draw engine already on your battlefield line, then a land when that line already prints six or more lands. Never Shield Sphere and never your only punisher - they go only when nothing else is left to send. Read the PUT line back ..."* | 32,669 -> 33,483 (**+814**) |
| `deck125_strategy.txt` | B6 | :86 *"- Essence Scatter counters CREATURE SPELLS ONLY. It counts - N must cover its 2 - when the"* -> *"- Essence Scatter counters CREATURE SPELLS ONLY - a spell whose stack line prints "(creature P/T)"; a spell printing any other type is out of its reach. It counts - N must cover its 2 - when the"* (rest of the bullet unchanged) | 44,619 -> 44,727 (**+108**) |
| `deck123_strategy.txt`, `deck126_strategy.txt` | - | untouched (both already carry a cleanup-discard order: 123:625, 126:361) | 0 |

**Pool total: +4,111 bytes across five guides**, of which +4,003 is the four new discard sections
(#136's per-guide order for the wave-50 ask kind). Every edit keeps the rule's action and its
printed key; only the missing order, the pre-lane-A card fact, and the stack-print key changed.

## Cross-guide KEY diff (#144)

- Cleanup discard: all seven guides now carry a section; six quote `"PUT: n, m"`; deck123's
  (:625) quotes the ask sentence only. Holdout by key: **deck123** does not quote the `PUT:` shape -
  not false, flagged for its reviewer.
- Discard-ask header literal: the four new sections quote the ask body (`your hand has N cards and
  your maximum hand size is 7`) AND the log line (`Cleanup discard (hand N, limit 7)`); deck125/
  deck126 (wave-49 W25/F18) quote neither - they key on "above seven at end of turn". Holdouts:
  **deck125, deck126** (true today; #128's grep will not find them if the ask wording changes).
- Soul Shatter's class: only deck146 plays it; no other guide names it. No holdout.
- Essence Scatter's stack print `(creature P/T)`: deck125 only. No holdout.

## Not verified / notes forward

- B1: the four orders are derived, not corpus-tested - zero `discard` records at deck130/152/162
  this corpus (all 28 were at 123/125/126/146). #143's metric (PUT lines containing a card the
  same reply's PLAN names as a play, 0/N) is the wave-51 seat's to run at all seven.
- B1: deck146's "four or more lands" land-discard gate is the reviewer's own number (the guide's
  bottoming rule is "a land goes under only from a keep of five or more lands"; at cleanup the
  battlefield count, not the hand count, is the right key). Marked inference.
- B2: the primitive's `text=` line already carried the Oracle sentence before lane A; the
  render's `{card text: ...}` in the ask printed it all corpus, so the guide and the row now agree
  for the first time. Re-check the first wave-51 Soul Shatter cast against a walker-outranks board
  (`w51a_soul_shatter_corpus_board.txt` is lane A's probe; a corpus specimen is still owed).
- B5: deck152's Lair sentence "animate it in MAIN 1 only" reads as an instruction with its reason
  beside it; left as the seat quoted it. When D7's annotation ships, that is the sentence to key.
- B6: the synthesis's `[creature]` literal was a guess at a render string that does not exist;
  recorded so the wave-51 seat greps for `(creature P/T)` on the stack line, not `[creature]`.
- Lane B: no guide sentence to retire; the payment order is not a guide subject. Not re-verified
  against a post-lane-B corpus (none exists yet).
- Card facts: Soul Shatter re-verified against the merged script's `text=` (Oracle); Essence
  Scatter against `mtg.txt:37073-37075`. Nothing else in B1-B7 turns on a card fact.
