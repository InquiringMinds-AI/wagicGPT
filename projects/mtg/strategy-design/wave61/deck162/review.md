# deck162 ("Draw and Die!", UB forced-draw punish) — wave-61 per-deck review

Seat logs (all six, `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-092408/`):

| file (abbrev) | opp | result | turn | final life | decisions | fallbacks |
|---|---|---|---|---|---|---|
| `1788618254-…deck162-0x55693c4e7f90-vs-…deck152` | 152 | **WIN** | 14 | 8 / -4 | 24 | 0 |
| `1788618258-…deck162-0x563185043ec0-vs-…deck126` | 126 | **WIN** | 21 | 4 / 0 | 54 | 0 |
| `1788618262-…deck162-0x555a5e730fe0-vs-…deck146` | 146 | **WIN** | 15 | 20 / 0 | 18 | 0 |
| `1788618266-…deck162-0x5583f4d7c500-vs-…deck125` | 125 | **WIN** | 15 | 18 / 0 | 20 | 0 |
| `1788618280-…deck162-0x5557bfad2330-vs-…deck130` | 130 | **WIN** | 14 | 20 / 0 | 20 | 0 |
| `1788618284-…deck162-0x55908f93c230-vs-…deck123` | 123 | **WIN** | 15 | 20 / 0 | 22 | 0 |

**6-0.** 158 model decisions, 0 fallbacks, 0 `transport`/`transport_error`, 0 timeouts, max
`deadline_pct` 68.3, 0 wall-miss events, 0 `deadline_pct > 100`. Five of six wins were
untouched-life or near-it; the deck killed on the opponent's own draw step every time.

## 1. Game-by-game: the deciding decisions

**vs152 (win, t14, 8/-4).** Kept a 7 (seq 1). Land every offered drop (seq 2,3,4,7,11,21 — no
skips all corpus). seq 8 Shield Sphere {0} free, seq 9 Master of the Feast at
`converters on your battlefield: 0` — legal under Rule 2's Master exception, opponent line read
"of which 3 are creatures" (verified in the seq-9 prompt). seq 10 blocked Sigarda (4/4 trample
flying) with Master over Elite Spellbinder (4/2): both rows read "you kill it, your blocker
lives", both stop 4, Sigarda is the better kill. seq 12 Ob Nixilis; seq 13/16 -2 on Sigarda then
Intrepid Adversary — the two removals that stopped the clock. seq 22-24 punisher + second
Dictate, seq 24 Master swings for lethal. Clean line, no misplay found.

**vs126 (win, t21, 4/0) — the only close game and the source of every HIGH item below.**
Opponent tutored Sanguine Bond and Exquisite Blood. On turn 7 (seq 8-15, eight consecutive
windows) the seat answered "Cast nothing right now" to a one-spell menu whose only row was
Dictate of Kruphix at `converters on your battlefield: 0` — the decline is **correct** under
Rule 2's brake independent of the loop reasoning (no punisher was out), but seven of those eight
windows should have been the HOLD row (item MED-2). From seq 17 it *ignored* the guide's
Bond/Blood suspension rule and cast Fate Unraveler, two Dictates, Master, Ob Nixilis, Howling
Mine, Underworld Dreams, Teferi's Puzzle Box and Forced Fruition — and that is what won:
seq 54's forecast (12 draws × 2 punishers = 24) resolved into the opponent going 20 → 0 on turn
21 with the seat's own life untouched at 4. Its two forced sacrifices (seq 22, seq 40, Tribute to
Hunger) both gave up a Fate Unraveler; seq 22's pick (the tapped copy of two) was right, seq 40's
was the cheapest-life row on the menu (4 vs 6 vs 5) with Sanguine Bond live. seq 46 is a real
misplay: 5 mana at 4 life on a second Ob Nixilis carrying
`[legendary: you already control Ob Nixilis, the Hate-Twisted - legend rule: casting this sends
one copy to your graveyard]`, then seq 47 chose which copy to bin. The guide's own HOLD-exemption
paragraph names exactly that row shape as "not a play in that window".

**vs146 (win, t15, 20/0).** First Howling Mine at K=0 (seq 4, sanctioned), Liliana's Caress
(seq 6), Underworld Dreams ×2 (seq 8, 10), Shield Sphere (seq 11), Ob Nixilis (seq 13) and the
-2 on Nadaar, Selfless Paladin (seq 14) — a token/anthem maker, exactly the guide's targeting
rule. Opponent 20 → 6 by t14 off draw damage alone; the seat never took a point.

**vs125 (win, t15, 18/0).** First Mine t4, Fate Unraveler t8, Sphere, a library-search pick
(seq 9, took Swamp to reach {B}×3), Underworld Dreams t10, second Mine t10 at K=1, Ob Nixilis +
Sphere + Caress t12, Dictate t14, second Underworld Dreams t14. Textbook Rule 1 → Rule 2 order.

**vs130 (win, t14, 20/0).** Mine t3, Master t5 (opponent line "of which 1 is a creature" —
exception satisfied), free block killing Dwarven Blastminer (seq 7), Caress t7, Fate Unraveler
t9, Underworld Dreams t11, Dictate t13, second Mine t13 at K≥1. Opponent 20 → 4 by t13.

**vs123 (win, t15, 20/0).** Caress t3, Underworld Dreams t9, Sphere, Fate Unraveler + Fog Bank
t11, Master + second Fate Unraveler t13, Master blocks and kills Bloodline Keeper (seq 19 — the
token maker, correct target under the guide's Ob-Nixilis targeting principle applied to blocks),
then seq 21 Peer into the Abyss and seq 22 "the opponent": 21 draws × 3 punishers, game over.

## 2. Engine / interface / card items

**HIGH-1 — the LOOP CAUTION narration asserts a loop the engine does not run. (PERCEPTION:
false surface.)**
File `1788618258-ai_baka_deck162-0x563185043ec0-vs-ai_baka_deck126.jsonl`, seq 52 and seq 54
(turn 20, my_life 4, opp_life 20). Rendered:

> `DRAW FORECAST (theirs): their next draw step draws 12 cards (1 + Dictate of Kruphix #1 1 +
> Dictate of Kruphix #2 1 + Howling Mine 1 + Teferi's Puzzle Box: their hand size 8) = 12 x 2 =
> 24 life to you from your punishers above. LOOP CAUTION: they control BOTH halves of a life LOOP
> (Sanguine Bond + Exquisite Blood) - Exquisite Blood turns life YOU lose into life THEY gain,
> and Sanguine Bond turns life they gain into life YOU lose, so once any…`

What actually happened at the very next draw step (gameend record, seq 55, turn 21):
`opp_life 0`, `my_life 4` — **unchanged**. 20+ points of punisher damage went through with zero
life gained by the opponent and zero life lost by this seat. The loop never fired in the
punisher direction. Corroborating: seq 18→20 the opponent fell 39 → 35 → 30 → 20 with my_life
pinned at 4 the whole way.

Card facts, verified against the primitives:
- `Exquisite Blood` — `mtg.txt:37966`, `auto=@lifelostfoeof(player):life:thatmuch controller`,
  `text=Whenever an opponent loses life, you gain that much life.`
- `Sanguine Bond` — `mtg.txt:99004`, `auto=@lifeof(player) from(*[-lifefaker]|*):life:-thatmuch
  opponent`, `text=Whenever you gain life, target opponent loses that much life.`
- `Underworld Dreams` — `mtg.txt:125822`, `auto=@drawfoeof(player):damage:1 opponent`
- `Ob Nixilis, the Hate-Twisted` — `planeswalkers.txt:2686`, `auto=@drawfoeof(player):damage:1
  opponent`
Scryfall (`/cards/named?exact=`) returns the identical Oracle text for both enchantments, so the
printed text is right and the divergence is behavioural: **`@lifelostfoeof` is not raised by
`damage:` life loss**, so Exquisite Blood never converts damage-based life loss. Sanguine Bond's
half *does* work — seq 37→38 in the same file, my_life 20 → 8 as opp_life went 37 → 49 off their
blocking-trigger lifegain, exactly as the `LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs`
paragraph promises. So the pair is half-implemented and the narration describes the whole
real-rules loop. A seat that believes the narration correctly concludes its own win condition is
suicide and passes forever; this seat only won by disbelieving it.
Fix belongs in the engine (raise the life-lost event from damage), not in the guide — see §3.

**HIGH-2 — `latched_row_mismatch` silently downgraded a correct, unambiguous answer to the
no-op row.**
Same file, **seq 49**, turn 20, main phase 1. Reply first line, verbatim:
`CHOICE: 4 (Cast Teferi's Puzzle Box)`. Option 4, verbatim:
`Cast Teferi's Puzzle Box {4} {leaves 6 of your 10 untapped mana sources untapped} {card text:
"At the beginning of each player's draw step, that player puts the…`. Index and short name
agree; `coded_answers` is 1, so there was no second coded answer to race it. The record carries
`parse_note = latched_row_mismatch`, `choice = 5`, `chosen_text = "Cast nothing right now
(combat comes next this turn)"`. The seat's actual answer was discarded and replaced with the
pass row. It recovered by casting the Box at seq 52, so it cost only a window here, but the
failure mode — a well-formed CHOICE line judged a name/row mismatch when it is not one, and
resolved to the noop — is unsafe on a lethal turn. Corpus-wide this note fires twice
(here and `1788618274-…deck123` seq 72, where it is paired with `echo_index_conflict`).
Suspects worth checking in the parser: the trailing `{4}` mana cost being read as a row index,
or the name matcher running over the trimmed post-answer overrun (`post_answer_overrun 3989`,
`reply_trimmed_bytes 2433`) rather than the CHOICE line.

**MED-1 — the reply contract says FIRST line, the parser honours the LAST coded answer, and
nothing in the prompt says so.**
The reply-rules line reads `On the FIRST line write CHOICE: followed by the number of your
choice … Write nothing else.` Two records in this deck's seat resolve a different row than that
first line:
- `1788618262-…vs-deck146` seq 17: reply line 1 `CHOICE: 2 (Cast nothing right now)`;
  `answer_replaced true`, `coded_answers 2`, `latched_coded_line 2`, `choice 1`,
  `chosen_text = Cast Fog Bank {1}{u} …`. (Here the override happened to match the guide.)
- `1788618258-…vs-deck126` seq 50: reply line 1
  `CHOICE: 2 (-2: destroy target creature … targeting Overgrown Battlement #1)`; the model
  reversed itself in prose and closed with `CHOICE: 0 (pass)`; resolved `choice 0`, `pass`.
12 records corpus-wide carry `answer_replaced true`. Last-wins is defensible for a genuine
prose reversal, but it is undocumented in the prompt the model reads, and it makes the
translog's `reply` first line unreliable as the seat's decision for anyone reviewing by eye.
Either state the rule in the reply contract or record both answers explicitly.

**MED-2 — HOLD is rendered constantly and taken almost never; the wasted windows are model
calls, and one of them ran 10 minutes.**
This seat: HOLD row rendered in **80** windows, taken **6**, with **15** windows answered
"Cast nothing right now" while HOLD sat on the same menu. Eleven of those 15 carried the
guide's own literal trigger `[you declined this exact list N times already this turn]` and only
3 of the 11 took HOLD. Worst run: `…vs-deck126` seq 8-15 — eight identical three-row menus
(`Cast Dictate of Kruphix {1}{u}{u}` / `Cast nothing right now` / `Hold priority: …`) across
one opponent turn, seven of them answered "Cast nothing" before the eighth took HOLD. The
decline itself is correct; the repetition is pure cost. Related: reply overrun is where the
latency goes — median 38.8 s but seq 17 of `…vs-deck146` took **614,878 ms** with
`post_answer_overrun 6706` and `reply_trimmed_bytes 3727` (the model wrote ~10 KB of
self-argument past its answer), and seq 19 of `…vs-deck130` took 402,284 ms.

**MED-3 — the forced-sacrifice row prices the opponent's life gain but not the Sanguine Bond
conversion of it onto your own total.**
`1788618258-…vs-deck126`, seq 40, turn 17, my_life 8, Sanguine Bond on their battlefield.
Rendered row 2: `Fate Unraveler (3/4) [your battlefield] [tapped] - "…" [you SACRIFICE this;
they gain 4 life (its toughness)]`. Row 1 offers Shield Sphere at "they gain 6", row 3 Master of
the Feast at "they gain 5". With their converter live, every one of those numbers is also that
much off **your** life — my_life went 8 → 4 on the resolution. The converter paragraph is
elsewhere in the prompt and the arithmetic is left to the model. This is the same
resulting-life-honesty shape lane R/C1 applied to ATTACK TOTAL; the sacrifice chooser has not
had it applied. Suggested render: `[you SACRIFICE this; they gain 4 life (its toughness) — their
Sanguine Bond turns that into 4 off you: you would be at 4]`.

**LOW-1 — duplicate-legend row offered with no price on its own face.**
Same file, seq 46: `Cast Ob Nixilis, the Hate-Twisted {3}{b}{b} [legendary: you already control
Ob Nixilis, the Hate-Twisted - legend rule: casting this sends one copy to your graveyard (you
choose which)]`. The tag states the rule but prints no cost line, and the seat spent 5 of 6
sources on it at 4 life. A `{spends 5 … and nets you nothing: the copy you keep is the one you
already have}` clause would make the row self-refuting the way the `{feeds: … converters: 0}`
clause does.

**LOW-2 — Peer into the Abyss prices its own two branches but not the punisher conversion.**
`1788618284-…vs-deck123` seq 21: `{right now: if you choose "the opponent": life -8, draws 21;
if you choose "you": life -10, draws 21}`. With `DRAW PUNISHERS … yours - Underworld Dreams,
Fate Unraveler ×2` on the same screen the true number is 21 × 3 = 63 more. The seat did the
multiplication itself and got it right, so this is an enhancement, not a defect.

## 3. Guide verdict: **KEEP as is**

`projects/mtg/bin/Res/ai/baka/deck162_strategy.txt` (52,982 bytes, inside the 41-71 KB band) is
kept unchanged, and no `wave61/deck162/strategy.txt` is written.

Why this is a success verdict, not a shrug:
- **6-0, and the guide's spine is what produced it.** Rule 0: the land drop was offered 21 times
  across six games and taken 21 times — zero skips. Rule 1's order held: every window that
  offered a punisher took one, except three where the *same turn's* next window took it
  (`…vs-deck123` seq 16→17) or the game ended that turn. Rule 1a/1b was applied correctly at
  every fork I checked: `…vs-deck123` seq 10, `…vs-deck130` seq 16 and `…vs-deck125` seq 14 all
  took the bodyless punisher over Fate Unraveler, and all three prompts read
  "0 of them without a restriction against attacking" on the opponent line — 1a's condition
  genuinely did not hold.
- **Rule 2's brake held under pressure.** Only five takes at `converters on your battlefield: 0`
  in six games; three are the sanctioned FIRST Howling Mine (`…vs-deck146` seq 4, `…vs-deck125`
  seq 4, `…vs-deck130` seq 4) and two are Master of the Feast inside its named exception — I
  read both prompts and the opponent line read "3 are creatures" and "1 is a creature"
  respectively. Zero second-engine-at-K-0 breaks, which is the failure this guide's largest
  section exists to prevent.
- **The one rule the corpus contradicts is contradicted by an engine defect, not by strategy.**
  The Sanguine Bond + Exquisite Blood suspension in DECIDING SITUATIONS is correct MTG. The seat
  won by breaking it only because Exquisite Blood does not fire on damage in this build
  (HIGH-1). Rewriting the guide to exploit that would encode a bug as doctrine and would be
  wrong the day the trigger is fixed. The other half of the pair *is* live and *did* take this
  seat from 20 to 8 in one combat (seq 37→38), so the rule's attack/lifegain half is paying for
  itself already.
- **The deviations that did occur cost nothing measurable.** The 8-of-11 HOLD misses (MED-2)
  wasted model calls, not plays; the duplicate Ob Nixilis (seq 46) wasted 5 mana on a turn the
  seat had 6 sources and no better row but Forced Fruition, which its own plan had ruled out.
  Neither is a decision the corpus shows losing a game, so neither buys a guide edit.

Two literals in the guide are **stale** and should be corrected the next time the file is opened
for a reason that is paid for — flagged here, not acted on, because neither cost a decision:
- The HOLD section quotes `Hold priority for the rest of this turn: … {taking this row skips the
  rest of this turn's identical windows}`. The live row in 80 of 80 of this seat's windows reads
  `Hold priority: pass now, and do not ask me again - this turn or later - until one of the rows
  above changes … {taking this row skips every later window whose rows are identical to these}`
  (the old string survives 6 times corpus-wide, 0 in this seat). Lane U/C14 changed it; the
  guide was not updated. The seat took HOLD correctly 6 times anyway.
- HOW THE KILL ACTUALLY HAPPENS item 5 says the Peer chooser offers `"target opponent"` and
  `"target controller"`. The live options are `the opponent` and `you`
  (`…vs-deck123` seq 22). The seat answered correctly anyway.

## 4. Proposals

None. No general-guide (`general-proposals.md`) or skill (`skill-proposals.md`) change is
written: nothing in this deck's six games is a decision that went wrong *because* the general
guide or the strategy-writing skill said something, and the two guide-accuracy defects above are
file-local staleness rather than a rule the skill would have prevented.

## 5. What I did NOT check

- I read only the six deck162-seat translogs in full; the opponents' seat files were consulted
  only for the `results.tsv` outcomes and not traced, so opponent misplays that handed this deck
  a win are not separated from this deck's merit. The lopsided finals (five wins at 18-20 life)
  make that a real possibility this review cannot rule out.
- I did not adjudicate any lane-R/S/T/U/V prediction — that is the engine seat's deliverable —
  and did not read `lane-*.md`, `codex-review.md` or `known-bugs.md`, so HIGH-1/HIGH-2 may
  duplicate a known item.
- I did not read the `.stderr` files, so engine-side SEGV/abort/assert signatures for these six
  games are unexamined; my "0 crashes, 0 hangs" claim rests on the `gameend` records only.
- I verified card facts for the 15 cards that decided windows (deck162's own list plus Sanguine
  Bond, Exquisite Blood, Perimeter Captain) against the primitives, and Scryfall for the two
  enchantments only. Opponent cards outside those — Sigarda, Katilda, Elite Spellbinder,
  Bloodline Keeper, Nadaar, Tribute to Hunger, Intruder Alarm — were taken as rendered.
- HIGH-1's mechanism (`@lifelostfoeof` not raised by `damage:`) is inferred from the primitives
  plus the observed life totals; I did not read `src/` to confirm it, and the brief forbids
  running the engine, so no repro run was made.
- The HOLD census is a prefix match on `chosen_text` / `options_text` (`startswith("Hold
  priority")`), not on the rendered `prompt` body, so a window whose HOLD row was worded
  differently would be missed.
