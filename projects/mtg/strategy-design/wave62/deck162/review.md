# deck162 ("Draw and Die!", UB forced-draw punish) — wave-62 review

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-133058/`, binary `12be0b79c`.
Six games, all natural, 0 fallbacks, 0 wall misses, 0 stale drops, 0 transport errors,
0 timeouts. Record 3-3. Seat files below are named by opponent (`vsNNN`); all citations are
`seq` in `1788633*-ai_baka_deck162-*-vs-ai_baka_deckNNN.jsonl`.

## 1. Game by game, and the decisions that decided them

| game | result | turn | deciding decision |
|---|---|---|---|
| vs123 | WIN 14-0 | 14 | none — clean line |
| vs126 | WIN 20-0 | 12 | seq14/16 (cast a 2nd punisher + Ob Nixilis -2 with their Sanguine Bond + Exquisite Blood out) |
| vs125 | WIN 18-0 | 25 | seq67-73 (Ob Nixilis, then the engine stack) |
| vs130 | LOSS -1/17 | 20 | seq6, seq9 (first Howling Mine declined twice); mana-denied by Stone Rain afterwards |
| vs146 | LOSS -5/1 | 17 | **seq16** — Fate Unraveler attacked into a printed GANG BLOCK clause and died |
| vs152 | LOSS -8/29 | 13 | **seq9** — Fate Unraveler blocked onto "(both die) + they gain 5 lifelink" |

**vs123 (WIN).** Textbook: first Howling Mine t3 (seq4), Caress t5 (seq6), Underworld Dreams t7
(seq8), Fate Unraveler t9 (seq10), engines t11/t13. Nothing to fix.

**vs126 (WIN).** Punisher curve t6/t8/t10, Ob Nixilis -2 twice (seq11, seq16). The opponent
completed Sanguine Bond + Exquisite Blood on their turn 11. The seat **disobeyed the guide's
DECIDING-SITUATIONS bullet** (which ordered ATTACK: none, no further punisher, no further engine,
"every rule SUSPENDED") and cast a second Fate Unraveler (seq14) and fired the -2 (seq16),
finishing 20-0 with its own life untouched at 20. The guide's rule, obeyed, throws this game away.

**vs125 (WIN, 25 turns).** Won, but idle from turn 6 to turn 18: 39 windows carried
`[you declined this exact list N times already this turn]` (seq 9-17, 24-31, 36-39, 43-51, 57-66)
and the HOLD row was taken twice. The brake itself was correct throughout (K=0, second engines).

**vs130 (LOSS).** seq6 (t3) and seq9 (t5): the FIRST Howling Mine, untagged, row printing
`converters on your battlefield: 0 ...; in your hand: 2 - Underworld Dreams, Ob Nixilis, the
Hate-Twisted`, declined both times with the plan line "Do not cast Howling Mine until a converter
is on the battlefield". The guide names this failure verbatim in three places and it recurred.
After turn 9 the opponent cast two Stone Rains and the seat was at 0 lands on turn 11 (seq12:
"Your battlefield (2 permanents listed ... 0 are lands)", `Mana available: 0`); from there it was
mana denial, not a guide effect.

**vs146 (LOSS at opponent 1 life).** Correct curve (Underworld Dreams t6, Fate Unraveler t8,
Puzzle Box t10, Caress t12, Ob Nixilis + -2 t14). Lost at **seq16**: the only attacker and only
converter was sent into a tag whose lone-blocker results were three friendly parentheses followed
by `- GANG BLOCK: any 2 of them together deal 4, enough to kill this attacker; each result above
is a LONE blocker only`. The reply reasoned the gang block out loud ("If they gang block, Fate
dies ... We die") and sent anyway. Both Silencers blocked; Fate Unraveler died. From there Kaya
and two Vanishing Verses stripped every replacement and the seat died at -5 with the opponent
on 1. `ATTACK: none` wins it on draw steps alone (Puzzle Box was drawing them 3/turn at 1 damage).

**vs152 (LOSS).** Lost at **seq9**, turn 9, life 12, header "NOT lethal", one blocker — Fate
Unraveler, the ONLY name on the `DRAW PUNISHERS on the battlefield: yours` line. Offered two
`(you kill it, your blocker lives)` lines (A1 2/2, A3 2/3) and `A2 (5/3) (both die) {their
attacker's lifelink, this block: they gain 5 from this block only}`; it took A2. Punisher dead,
opponent 21 → 26, zero converters for the remaining four turns. Rule 4's rung 0 ("if what walks
past leaves you at 8 or less, block the BIGGEST deals N") *points at that line* — the guide is
complicit, not merely disobeyed. Downstream: seq11 (Puzzle Box at K=0, reply: "casting Puzzle Box
is risky ... But I have no other good plays"), seq14 (Rule 0c body turn at life 3, M=5>N=0: took
Fog Bank only and left an affordable Master of the Feast on the menu), seq16 (2nd Dictate at K=0).
Those were already-lost positions.

## 2. Engine / interface / card items

**HIGH — a `- GANG BLOCK:` clause is rendered as the tail of a list of friendly results.**
`vs146` seq16, the A1 line: `[their untapped blockers: Silverquill Silencer #1 (3/2) (you kill
it, your attacker lives); Goblin (1/1) (you kill it, your attacker lives); Silverquill Silencer #2
(3/2) (you kill it, your attacker lives) - GANG BLOCK: any 2 of them together deal 4, enough to
kill this attacker; each result above is a LONE blocker only]`. Three "your attacker lives"
parentheses precede the one clause that negates all of them. The engine already knows the verdict;
it should lead with it (e.g. `[GANG-BLOCKABLE: any 2 of Silverquill Silencer #1, Silverquill
Silencer #2 deal 4 and KILL this attacker - the per-blocker results below are LONE blockers only:
...]`) rather than append it. Same clause shape appears in the collapsed-tag form, where the guide
had already scoped its rule; the *uncollapsed* form is the one that bit.

**MED — 43 windows carried `[you declined this exact list N times already this turn]`, HOLD
taken 2.** vs125 39, vs130 2, vs146 1, vs152 1. Worst run: vs125 seq43-51, turn 15, nine
consecutive identical 3-row menus (`Cast Dictate of Kruphix ... converters on your battlefield: 0`
/ `Cast nothing right now` / `Hold priority`), each answered `Cast nothing right now`. The
`gameend` record for that game reports `hold_windows_skipped: 18` and
`identical_ask_answers_reserved: 15`, so the reservation machinery exists but does not cover this
seam. Two waves of guide text have now failed to move this (2/43 this corpus, 0/42 last); the
remaining lever is engine-side — auto-apply the hold once the declined-count reaches N with an
unchanged row set. Cost is real: 175 model calls in six games, 7,049 s of latency total, and
individual calls up to 264 s (`vs152` seq11, `vs146` seq20 244 s, `vs146` seq16 209 s).

**MED — the `{feeds:}` clause's two counters are easy to conflate, and the guide had conflated
them.** Every feeds row prints `converters on your battlefield: K` and then, separately,
`discard punishers (a different class ...): on your battlefield: N - Liliana's Caress`. Across the
whole corpus, 0 of 63 K-lists ever name Liliana's Caress (`vs152` seq11/12/14/15/16 all read
`converters on your battlefield: 0 ...; discard punishers ... on your battlefield: 1 - Liliana's
Caress`). The render is correct and consistent; the *guide* asserted K counts Caress. Fixed below.
Suggestion only: naming the first counter `draw converters` would make the two classes
self-distinguishing at a glance.

**LOW — `Opponent life trend` samples turn-boundaries, so a mid-turn swing reads oddly.**
`vs146` seq19: `Opponent life trend: turn 12: 8, turn 14: 9, turn 16: 1, now 1 (-7 since turn 12)`
while the log shows 9 → 7 inside turn 14 and 6 → 1 inside turn 15. Not wrong, but the "-7 since
turn 12" understates a 3-punisher board. No decision turned on it.

**No card-text defects found.** Verified against
`bin/Res/sets/primitives/{mtg,planeswalkers}.txt`: Liliana's Caress
`auto=@discarded(*|opponenthand):life:-2 opponent`; Underworld Dreams / Fate Unraveler / Ob
Nixilis all `auto=@drawfoeof(player):damage:1 opponent`; Howling Mine `sourcenottap` on each
player's draw; Teferi's Puzzle Box bottom-then-redraw on each player's draw; Sanguine Bond
`@lifeof(player)... :life:-thatmuch opponent`; Exquisite Blood `@lifelostfoeof(player):life:
thatmuch controller`. All match the rendered text and Oracle. The Sanguine Bond / Exquisite Blood
pair on THEIR side is entered by the pilot's life going down or their life going up — never by
punisher damage — which is what the wave-62 `LOOP SCOPE` sentence now prints (`vs126` seq13-16,
correct and PASSing there).

## 3. Guide verdict: **EDIT** — `wave62/deck162/strategy.txt` written (58,932 B, band 41-71 KB)

Seven edits, each paid for by a seq above. Everything else is unchanged.

1. **RULE 4, new pre-rung block (a)+(b)** — before → after: rung 0 was the first thing run at a
   block window and it selects the biggest `deals N` whenever ≤8 life would get through; now two
   prices are charged first: (a) when the `DRAW PUNISHERS on the battlefield: yours` line names
   exactly ONE card and that card is the blocker offered, `(both die)` and `your blocker dies`
   lines are off the menu for it below the lethal check; (b) a `(lifelink: ...)` /
   `{their attacker's lifelink, this block: they gain N}` clause is added to that line's cost
   before comparing lines. Rung 0 gained an explicit "bounded by (a) and (b)" sentence.
   Paid for by **`vs152` seq9**.
2. **DECIDING SITUATIONS, the Sanguine Bond + Exquisite Blood bullet** — before: "THE CHAIN ENDS
   ON YOUR TOTAL ... the DRAW FORECAST line is counting the wrong direction ... ATTACK: none ...
   every rule SUSPENDED." After: the pair on their side fires on YOUR life going down and THEIR
   life going up; punisher damage enters it nowhere; the prompt's `LOOP SCOPE` sentence says so
   and is correct; keep casting punishers and engines, keep walls down, price ATTACK by the tags.
   Paid for by **`vs126` seq13-16** (the seat won 20-0 by disobeying the old bullet), corroborated
   by the primitives above and by wave62/lane-Y.md's live probes.
3. **DECIDING SITUATIONS, the "one half tutored → stop committing" bullet** — before: do not cast
   a punisher you have not already cast. After: one half changes nothing about your punishers; it
   only raises the price of life THEY gain, so it governs the ATTACK rule, not the cast rule.
   Same evidence as (2) — the old bullet is the same false premise one draw earlier.
4. **RULE 2, the K definition** — before: "It counts Underworld Dreams, Fate Unraveler, Ob Nixilis
   AND Liliana's Caress for you", plus a home-made "K counts Caress only at opponent hand ≥7"
   qualifier. After: K counts the three punishers and nothing else; the row prints Caress in a
   separate `discard punishers` clause and the engine never moves it into K; "discard punishers: 1"
   is not the release. Paid for by **`vs152` seq11/12/14/15/16** and 0/63 K-lists corpus-wide.
5. **RULE 2, cast-order rung 6** — before: "A Caress already on the battlefield is what lets a
   later Mine's row read 'converters on your battlefield: 1' at all." After: it does not; cast the
   Caress anyway, for the reasons the rung already gives. Same evidence as (4).
6. **RULE 2, the brake release** — before: "READ OFF YOUR HAND LINE ... scan 'Your hand (N cards):'
   for the three names". After: the row itself prints `in your hand: N - <names>`; read the
   counter, not the hand line. Plus a new "a bad board is not a release" clause naming the four
   sentences that produced takes ("I have no other good plays", "I am in a bad spot", "to set up
   for when I get a punisher", "I have to do something"). Paid for by **`vs130` seq6/seq9** (the
   row printed `in your hand: 2 - Underworld Dreams, Ob Nixilis` and the first Mine was declined
   twice) and **`vs152` seq11**.
7. **ATTACKING, the GANG BLOCK clause hoisted** — before: the clause lived inside "MASTER OF THE
   FEAST AND THE COLLAPSED TAG", scoped to collapsed tags. After: a standalone paragraph stating
   that a `- GANG BLOCK:` clause anywhere in any tag, including one that has just listed every
   blocker with a friendly parenthesis, is `ATTACK: none` for that creature below the lethal
   check. Paid for by **`vs146` seq16**.

Not edited, deliberately: the first-Howling-Mine rule (`vs130` seq6/seq9). It is already stated
three times with a WHAT-THIS-COST attached and was still broken; a fourth restatement is text, not
a fix. Edit 6 attaches it to a printed counter instead, which is the only new lever the corpus
offers. Also not edited: Rule 0c (`vs152` seq14 broke it, but the position was already lost —
Fog Bank + Master of the Feast still leaves 6 damage against 3 life).

## 4. Proposals

None. No general-guide or skill amendment is proposed: every defect above is deck-local (a
deck162 guide falsehood, or a deck162 rule mis-scoped) or engine-side, and the two general
patterns visible here — "believe the engine's computed clause" and "a restated brake is not an
obeyed brake" — are already amendments in the wave-59 skill edition.

## 5. What I did NOT check

- The opponents' seat translogs, except where the deck162 prompt's GAME LOG quoted their actions;
  I did not read deck125/130/146/152/123/126's own decisions or their guides.
- The other 15 games in the corpus, the 152v125 hang, and the `matchups-20260905-161801` rerun
  (deck162 is not in either pairing).
- Any lane prediction adjudication — that is the engine seat's deliverable; I only note that the
  `LOOP SCOPE`/`LOOP CAUTION` split (lane Y, D3) and the `KILLS you` crack-back verdict (lane X,
  D19) both rendered as specified in this deck's games.
- Engine source. I read no `src/` file and ran no build, no game, and no git command.
- Scryfall: the API was not queried; card facts were verified against the primitives only, and
  none of them looked doubtful enough to need a second source.
- Mulligan quality: all six keeps were 2+ lands with spells, i.e. the guide's not-close-call
  branch; no bottoming or second-look decision arose in this deck's games.
