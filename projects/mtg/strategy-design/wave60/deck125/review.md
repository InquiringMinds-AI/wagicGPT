# deck125 (Revelation Fracture — Modern UW Control) — wave-60 per-deck review

Corpus `matchups-20260904-171140`, binary master 614f8452e. Six games, all natural, **0
fallbacks** across 626 decisions at this seat (wave 59: 12 corpus-wide). Record **2-4**.
Seat files are `<epoch>-ai_baka_deck125-0x...-vs-ai_baka_deck<N>.jsonl`; every seq below is that
file unless marked otherwise.

---

## 1. Game by game

| opp | result | end | decisions | the decision that decided it |
|---|---|---|---|---|
| 126 | **WIN** t39 33-0 | Staff of Nin x2 clock | 187 | s168 Lightmine Field into 2 attackers, then s185 Staff ping for exact lethal (badge correct) |
| 123 | **WIN** t66 83-0 | Staff x2 + Elixir recursion | 195 | 8 counters landed t6-t59; s193 Staff ping for exact lethal (badge correct) |
| 162 | LOSS t15 0-20 | draw punishers | 44 | s41/s42 — see HIGH-2 |
| 130 | LOSS t44 0-8 | Hammer of Bogardan recursion | 100 | s49/s75 — see MED-1 (did NOT decide the game) |
| 146 | LOSS t27 0-24 | Nadaar + venture beats | 50 | s48 — see HIGH-3 |
| 152 | LOSS t25 -5-25 | one growing Luminarch Aspirant | 50 | s45 — see HIGH-1 |

**The two wins are the guide working.** Both were won by the guide's stated plan verbatim
(#1: "Staff of Nin is your win condition, Emrakul is not your plan"): counter everything, land
two Staffs, ping the face. In the 123 game the seat took 8 counterspells, recycled with Elixir of
Immortality four times, went to 83 life and killed from 20 with Staff pings alone over 30 turns.
Neither win contained a misplay I could name.

**Three of the four losses are the same shape**: an opponent permanent whose damage output is NOT
in the deck's only life-planning instrument (the CRACK-BACK line). The 130 loss is variance
(Hammer of Bogardan recurs every upkeep; the seat countered it once and had no answer to a
graveyard). The 162 loss is a 5-lands-in-13-turns land drought against Underworld Dreams + Ob
Nixilis + Dictate + Howling Mine — no counter was ever drawn in time; I do not read it as a guide
effect.

Verified against the primitives: Supreme Verdict, Final Judgment, Path to Exile, Lightmine Field,
Staff of Nin, Sphinx's Revelation, Siege-Gang Commander, Hive of the Eye Tyrant, Luminarch
Aspirant, Ranger Class (`/usr/bin/grep -n "name=..." -A8 bin/Res/sets/primitives/*.txt`). No
disagreement with Scryfall found on any of them.

### Wave-59 items at this seat, re-measured

| wave-59 item | wave-60 result |
|---|---|
| HIGH-2 sweeper rows price a count, never names (180 rows, 0 named) | **FIXED.** 81 magnitude rows; **49/49 with N+M>0 carry `- THEIRS: <name> (P/T)`, 0 without**. Lane-O prediction 1 PASS. |
| HIGH-2 outcome: 5 one-for-one sweeps in six games | **FIXED.** 3 sweepers cast, at 3 / 3 / 2 creatures (146 s24 Final Judgment; 152 s28, s38 Supreme Verdict). **0 one-for-ones.** Lane-O prediction 2 PASS. |
| HIGH-3 duplicate `PUT:` index rejected with no re-ask | **FIXED.** 35 discard asks, 18 carrying the new "the numbers must all be DIFFERENT" clause, **0 duplicate-index replies, 0 re-asks**. Lane-M B3 PASS. |
| HIGH-1 no DISCARD-punisher lens / cleanup price | **UNTESTED** — no discard punisher stood on any board in these six games (0/35 discard asks). Lane-N B5-2/B5-3 had no window here. |
| MED-1 `Cast nothing` dominated HOLD 282/282 | **IMPROVED.** Both rows offered 358x: Cast-nothing 227, HOLD 49, other 82; 363 windows actually skipped by HOLD. No longer a defect — see LOW-1. |
| MED-2 CRACK-BACK "for up to N" exceeded 3/34 | **REGRESSED — now 6/26 (23%). See HIGH-1.** |
| B1 win fold (lane L) | 2 `THIS WINS THE GAME` renders, **both true** (opp at 1, Staff pings 1, no life cost, empty stack). |
| B11 ATTACK TOTAL | **UNTESTED** at this seat — deck125 ran 0 creatures in all six games and never got an attackers ask. |

---

## 2. Engine / interface items

### HIGH-1 — `CRACK-BACK NEXT TURN: ... for up to N - you would be at X` is a FALSE CEILING (6 of 26)

For a deck with zero creatures this line is the *only* instrument the seat has for deciding
whether to tap out. Measured over every distinct CRACK-BACK render at this seat against the
combat damage the narration records on the very next turn: **6 of 26 turns took MORE than the
printed "up to N"**, and the two worst are the deciding turns of two losses. Three separate
mechanisms, all pre-computable from the board as printed:

**(a) Activated direct-damage on their board is excluded.** `...-vs-deck130` seq 21, t13, life 20:
```
CRACK-BACK NEXT TURN: 4 of their creatures will be able to attack (tapped ones untap first), for up to 5 - you would be at 15
Opponent battlefield (9 permanents listed, ...): Forgotten Cave [tapped]; Goblin #1-#3 (1/1) x3; Mountain #1-#4 [tapped] x4; Siege-Gang Commander {3}{r}{r} (2/2)
```
Turn 14 dealt **9**, not 5: Siege-Gang Commander sacrificed two of the attacking Goblins for 2
each (`{1}{R}, Sacrifice a Goblin: deals 2 damage to any target` — verified in the primitive).
Life 20 -> 11. The seat is told it would be at 15.

**(b) Creature-lands are excluded from the creature census.** `...-vs-deck146` seq 40, t20:
```
CRACK-BACK NEXT TURN: 1 of their creatures will be able to attack (tapped ones untap first), for up to 1 - you would be at 12
Opponent battlefield (... 1 is a creature ...): ... Hive of the Eye Tyrant; ... Triumphant Adventurer {b}{w} (1/1) [deathtouch] [tapped ...]
```
Turn 21: "Opponent used: becomes beholder with Hive of the Eye Tyrant" then "Hive of the Eye
Tyrant dealt 3 damage to you". Actual 4, printed 1. The land is on the same rendered line.

**(c) Pump triggers that fire between now and the attack are excluded.** `...-vs-deck152` seq 45,
t22, life 9: `for up to 4 - you would be at 5`, one attacker, Luminarch Aspirant (4/4) with
Ranger Class at level 3 on their board. Turn 24 it attacked as a 6/6 (its own beginning-of-combat
+1/+1, plus Ranger Class's attack trigger) for **6**. Life 3, not 5; the seat died the next turn.
Same shape at seq 32 (t14, up to 4, actual 5) and 146 seq 44 (t22, up to 1, actual 2).

Under the trust doctrine this is the worst kind of surface: the seat is instructed to believe it,
it is phrased as a bound ("for up to", "you would be at"), and it is not one. Either fold (a)/(b)/(c)
in, or stop phrasing it as a ceiling. (a) and (b) are cheap — both sources are already enumerated
on the opponent battlefield line the render walks. I did NOT determine whether the same
understatement affects the `INCOMING THIS COMBAT` line (fewer windows; deck125 never blocks).

### HIGH-2 — the "best X" badge on an X announcement ignores the NET it printed one clause earlier

`...-vs-deck162` seq 41 (cast menu) and seq 42 (ANNOUNCE_X), t15, **my_life 2**:
```
1. X = 2 {X pricing: X=2 - you gain 2 life and draw 2 cards; the opponent's Underworld Dreams, Ob Nixilis, the Hate-Twisted punish every draw, so those draws cost you 4 life - NET -2 life for this cast} {leaves 0 of your 5 floating mana unspent - this X spends your whole pool} [<- largest affordable X - X=2 gains 2 life and draws 2 cards; no listed X does more]
2. X = 1 {... NET -1 life for this cast} ...
```
The badge `[<- largest affordable X ... no listed X does more]` is computed from affordability
alone. On the same row the engine has already priced the choice at **NET -2 at 2 life = death**.
`X = 1` is NET -1 (survives at 1); the badge steers away from it. The seat took row 1 and died.
This is exactly the wave-59 HIGH-1 shape ("THIS WINS THE GAME" badged while dead) surviving on the
ANNOUNCE_X path: lane L folded the *win* verdict against life cost, but the *best-X* recommender
was not folded. The fix is the mirror of B1 — a badge that would put `my_life + NET <= 0` must not
be issued, and the row should carry the self-kill statement the win fold already carries.

**Honest scope:** it did not decide this game. Seq 41 also printed `ON THE STACK: 2 damage to you
- you would be at 0; that would KILL you` from four already-stacked triggers, and nothing in the
seat's hand counters a triggered ability, so it was dead either way. N=1 window in this corpus
(only one `NET -` render at this seat).

### HIGH-3 — a damage-based board wipe prices a count but never says who dies or what still gets through

`...-vs-deck146` seq 48, t26, **life 3**, the last real decision of the game:
```
CRACK-BACK NEXT TURN: 3 of their creatures will be able to attack (tapped ones untap first), for up to 8 - you would be at -5; that would KILL you
1. Cast Lightmine Field {2}{w}{w} {right now: they control 3 creatures able to attack - deals 3 to each if all 3 attack} {leaves 7 of your 11 untapped mana sources untapped} ...
2. Cast nothing right now (combat comes next this turn)
```
The only question at 3 life is *does this stop lethal*. It does not: 3 damage kills the Goblin
(2/2) and Triumphant Adventurer (2/2) but Nadaar, Selfless Paladin is 4/4 and still connects for
4. The row states neither fact. Lane O's B7 gave destroy/exile sweepers a `- THEIRS:` roster; the
damage-based wipes (Lightmine Field here; the same clause shape appears at 152 s44, 126 s168) got
a bare count. Extending B7's roster to this clause — name each creature, mark which survive the
damage, and total the survivors' power against the CRACK-BACK — is the same code walking the same
board, and it converts this row from a count into the answer. This class is deck125-shaped: two of
its four losses ended with an unanswered "will this stop the attack" at single-digit life.

### MED-1 — `{dead right now:}` at a CLEANUP ask answers a different question than the ask asks

`...-vs-deck130` seq 49, t25, hand of 8: the ONLY row carrying a tag is
```
5. Supreme Verdict {1}{u}{w}{w} (sorcery) {card text: Supreme Verdict can't be countered. -- Destroy all creatures.} {dead right now: destroys 0 of their creatures (0 without a restriction against attacking), 0 of yours}
```
against a hand of three Sphinx's Revelation, two Staff of Nin and two Essence Scatter. It sent the
Supreme Verdict. Ten turns later, seq 75, t35, same shape:
```
7. Final Judgment {4}{w}{w} (sorcery) ... {dead right now: exiles 0 of their creatures (0 without a restriction against attacking), 0 of yours}
```
— sent, from a hand of three Essence Scatter + Cancel + Fall of the Gavel. Both against a deck that
had already cast Siege-Gang Commander and would later cast Rorix Bladewing. The tag is TRUE and
the read is correct — this is not a perception failure at the render's own terms — but at a
cleanup ask the question is "which card do I want later", and a tag priced against *this instant*
of an empty board is the answer to a different question. On a CASTING menu the tag is exactly
right; on a cleanup list it is systematically biased against sweepers and removal, which are
precisely the cards whose board is empty on your own turn. Worth considering a cleanup-scoped
phrasing ("nothing to hit right now" rather than "dead right now"), or suppressing the verdict tag
on the cleanup ask entirely. **Did not decide this game** — deck130 never rebuilt a creature and
killed with Hammer of Bogardan. I have handled the seat's half of it in the guide (§3).

### LOW-1 — the HOLD / `Cast nothing` pair still spends a window on a distinction the model mostly ignores

Both rows offered 358 times; the model took `Cast nothing right now` 227, HOLD 49. Wave 59 was
282/282 for Cast-nothing, so the wording changes worked, but the pair still means the seat re-reads
an identical ~4 KB prompt for the same non-action: the 152 game alone skipped 59 windows via HOLD
while 17 more Cast-nothing windows were rendered in full. Not a correctness item; a prompt-economy
one, noted because deck125 (a pure draw-go deck) generates more of these windows than any other
deck in the pool.

### LOW-2 — the plan echo is now bounded and stable, and it went stale for six turns

The carried `YOUR PLAN` was 175-215 chars in every render (lane M B13 holding). But the 152 game
carried "…Staff of Nin only from a row whose leaves N covers that counter's cost; ping their face
every turn" from turn 18 to turn 24 while the seat controlled **zero** Staff of Nin and never would
— `(as you last stated it, 3 windows ago on turn 23)` correctly dates it, and the seat is not
re-asked for a plan when the plan's subject leaves the board. Low value, low cost; recording it
because the same dead plan rode all six games.

**Not checked:** the opponent seats' own decision quality (read only for context); anything at a
blockers/attackers seam (deck125 ran 0 creatures — no window in six games); latency/transport
records; the `venture` renders (43 prompts, but all are the opponent's ask seen from this seat's
narration, not this seat's decision); `async_drops` and wall-miss accounting (engine seat's).

---

## 3. Guide verdict: **EDIT** (one paragraph; the guide's cleanup section contradicted itself)

The guide is otherwise doing its job — both wins ran its stated plan, the step-2 land rule held,
and no loss traced to guide prose. One repair, paid for by two seqs.

**Edit 1 — the verdict-sort rule overrode the tier list it sits under.** The CLEANUP section's
step 3 says, correctly: *"Against a deck that has cast a real creature, a lone sweeper stays."*
Fifteen lines later the verdict-sort paragraph says *"SEND IN THAT ORDER: every `{dead right now:}`
row … first … While a dead or spare row is still on the numbered list, a live one is never part of
your answer."* At `deck130` **seq 49** the lone Supreme Verdict was the ONLY tagged row on the list,
so the second rule mandated exactly the send the first rule forbade; at **seq 75** the same for the
lone Final Judgment. The model followed the later, more absolute rule both times. Added, after
"…sat two rows above it.":

> before: *(nothing — the paragraph ended there)*
> after: `THE TAG IS A SNAPSHOT, NOT A VERDICT, AND DOES NOT OVERRIDE STEPS 3 AND 6: their board is empty at your own cleanup more often than not. A LONE sweeper or a LONE Path, against a deck that has already cast a real creature, is never sent for carrying "{dead right now:}" while an untagged spare - a third Revelation, a second Staff, a third Scatter - sits on the list. You sent your only Supreme Verdict and, ten turns later, your only Final Judgment that way.`
> paid for by: `...-vs-deck130` seq 49 (t25) and seq 75 (t35).

**Edit 2 — trimmed a now-false victory claim** so the file stays in band. Step 3's
*"you sent one ahead of your counters four times this corpus and it cost you nothing"* and the
tier note's *"THIS ASK USED TO BE WHERE YOU LOST GAMES; it is now at its ceiling - the step-2 land
rule went twenty-seven of twenty-seven last corpus and no loss was decided here."* were both
wave-59 self-congratulation; the first is superseded by Edit 1 and the second is falsified by
seq 49/75. Cut to *"dead as step 1's Lightmine Field."* and *"Hold it."*

`strategy.txt` = 71,000 bytes (live guide was 70,801; pool band 41-71 KB, pool max was 70,997).
No other line changed; 0 U+FFFD.

## 4. Proposals

**None.** Every item above is an engine/render item routed to the core loop in §2. Nothing here
argues for a change to the general guide (wave59/general-strategy.md) or to the
strategy-writing skill — the guide-writing method worked (two wins on the stated plan, one
internal contradiction which is a drafting slip, not a doctrine gap), and the cleanup-tag and
CRACK-BACK problems must be fixed at the surface, not taught around (a guide teach that hedges
against a render is forbidden).
