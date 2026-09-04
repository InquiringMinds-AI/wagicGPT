# wave-59 per-deck review — deck162 "Draw and Die!" (draw-punisher control)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-130044/`, model `qwen36-35b-a3b`.
Six games, all natural, **3-3**. My seat's decision counts are small (18-34 records/game);
median latency 40-65 s, max 210 s. Fallbacks across all six seats: **1** (`named_row_reask`,
146-game seq12) — no `empty_reply`, no `unparsed_reply`, no timeouts, no `async_drops`,
no wall misses. HOLD row rendered in 65 prompts, taken 3.

## (1) Game by game

| opp | result | turn | life | file (my seat) |
|---|---|---|---|---|
| 152 | LOSS | 13 | -6 / 27 | `1788544851-ai_baka_deck162-0x563617690c10-vs-ai_baka_deck152.jsonl` |
| 125 | WIN | 24 | 12 / -31 | `1788544862-...-0x56143eb2a380-vs-ai_baka_deck125.jsonl` |
| 146 | WIN | 17 | 20 / 0 | `1788544879-...-0x55ae9e49ea50-vs-ai_baka_deck146.jsonl` |
| 123 | LOSS | 12 | -71 / 6 | `1788544880-...-0x55eb3c708060-vs-ai_baka_deck123.jsonl` |
| 130 | WIN | 12 | 20 / 0 | `1788544895-...-0x5630b63ce150-vs-ai_baka_deck130.jsonl` |
| 126 | LOSS | 15 | 0 / 40 | `1788544901-...-0x55741600ed80-vs-ai_baka_deck126.jsonl` |

**vs 126 — the deciding game, and the wave's HIGH.** The opponent tutored **both** halves of the
Sanguine Bond + Exquisite Blood loop on turn 9 (both tutor lines are in my GAME LOG). My seat cast
Underworld Dreams on turn 10 (seq13), a second Howling Mine the same ask (seq14), two Dictates
(seq15/18), and **Fate Unraveler on turn 14 (seq20) with the loop already complete on their
board**. Their turn-15 draw step drew 5 cards off my own Mines/Dictates; every point my punishers
dealt them was returned by Exquisite Blood, converted by Sanguine Bond, and chained until I was at
0. Final: **me 16 -> 0, them 24 -> 40, in one draw step.** My own engine killed me. PERCEPTION
half: HIGH-1 below. STRATEGY half: the guide's own rule told the seat the punishers were still
safe — see (3).

**vs 152 — LOSS, one clean surface-caused misplay.** seq13 (turn 10, my life 13, their board 3
attackers, CRACK-BACK line already reading "you would be at 4"): the seat cast a **second Howling
Mine** on a row reading `converters on your battlefield: 1 - Liliana's Caress` — and echoed the
surface straight into its plan: *"Stack draw engines now that we have a converter (Liliana's
Caress) on board."* Liliana's Caress is `@discarded(...)`, a **discard** punisher; it converts no
draw. That ask handed an aggro deck an extra card a turn at 13 life with no draw punisher out and
no board. The seat self-corrected two windows later (seq16 plan: *"only Liliana's Caress (which
only triggers on discard, not draw)"*) and cast Fate Unraveler at 8 life into a CRACK-BACK line
reading "-10; that would KILL you" — correct but a turn late. HIGH-2 below.

**vs 123 — LOSS, not a play failure.** deck123 assembled Bloodline Keeper + Intruder Alarm and
declared 24 flying 4/4 Vampires on turn 12. My punishers had them at 6 and would have won on their
next draw step. The blockers render was exemplary — `INCOMING THIS COMBAT: 24 attackers, 96
unblocked damage - you would be at -75; this KILLS you ... best case with every blocker assigned:
you would be at -71; no block saves you` — and the seat's `BLOCKS: B1:A1` was the best legal
answer. Fate Unraveler was correctly withheld from the blocker list (no flying/reach vs. 24
fliers). No item.

**Wins.** vs 130 (seq10/12/16): Master of the Feast attacked three consecutive turns while
Underworld Dreams ticked — the deck's real dual clock, played correctly. vs 146: Fate Unraveler ×2
+ Underworld Dreams + Mines took 18 to 0 in four turns; the two `priority` HOLD takes (seq27,
seq32) both fired cleanly. vs 125 (23 turns, the deck's showcase): every creature was answered by
removal (2× Path to Exile, Supreme Verdict, Cancel, Fall of the Gavel), so the win came purely off
Teferi's Puzzle Box + 2 Mines + 2 Dictates feeding Underworld Dreams — 10 opponent draws a turn,
25 -> -31. The narration ("Opponent put a card from their hand into their library (x9)"; "Opponent
drew 10 cards") made that engine legible at every step.

## (2) Engine / interface / card items

**HIGH-1 — the punisher-vs-their-loop surface counts the damage in the wrong direction, and it
killed a game.** With Sanguine Bond + Exquisite Blood on the OPPONENT's battlefield, my draw
punishers deal damage that loops back onto MY life total. Three renders in the same prompt say the
opposite.
Repro: `1788544901-ai_baka_deck162-0x55741600ed80-vs-ai_baka_deck126.jsonl`, **seq 20**, turn 14,
my life 16. Rendered lines, verbatim:
- `DRAW PUNISHERS on the battlefield: yours - Underworld Dreams. Every card the OPPONENT draws costs them 1 life to yours.`
- `DRAW FORECAST (theirs): their next draw step draws 5 cards (...) = 5 x 1 = 5 life to you from your punishers above.`
- option 2: `Cast Fate Unraveler {3}{b} (3/4) ... {their converter: this body has toughness 4 and they control 1 life-to-damage converter (Sanguine Bond) - any effect of theirs that gains them life equal to its toughness ... takes 4 off YOU: life 16 -> 12}`
The converter block DOES print the loop warning ("Both halves of a life LOOP are on THEIR
battlefield ... chains until YOU are at 0"), but the two punisher lines and the cast row's own
converter annotation contradict it: the forecast promises 5 life *gained* off a draw step that in
fact took 16 off me, and the Fate Unraveler annotation prices the *edict* risk (4) while missing
that the card's printed ability is now self-lethal. The seat cast it. One turn later: me 0, them
40. **Fix**: when the opponent controls a completed life-to-damage loop (or Exquisite Blood alone
with a Sanguine Bond effect), invert the DRAW PUNISHERS / DRAW FORECAST framing to "every card
they draw takes N off YOUR total", and make any punisher cast row carry that instead of the
toughness-edict clause. Under the trust doctrine this is a render that lies at the exact decision
that lost the game.

**HIGH-2 — `{feeds: ... converters on your battlefield: K}` merges draw punishers and discard
punishers into one count, so a discard punisher is credited to a draw-feeder.**
Repro: `1788544851-ai_baka_deck162-0x563617690c10-vs-ai_baka_deck152.jsonl`, **seq 13**, turn 10,
my life 13, `Opponent hand size: 6`. Rendered row 1, verbatim:
`Cast Howling Mine {2} [second copy: ...] {leaves 1 of your 3 untapped mana sources untapped} {card text: "At the beginning of each player's draw step, if Howling Mine is untapped, that player draws an additional card."} {feeds: the opponent draws 1 extra card per turn; converters on your battlefield: 1 - Liliana's Caress; in your hand: 4 - Fate Unraveler, Ob Nixilis, the Hate-Twisted, Underworld Dreams, Ob Nixilis, the Hate-Twisted}`
Liliana's Caress: `auto=@discarded(*|opponenthand):life:-2 opponent` / `text=Whenever an opponent
discards a card, that player loses 2 life.` (mtg.txt:67351-67353); Scryfall agrees. Howling Mine
adds a **draw**, which Caress never sees. Site: `src/AIPlayerGPT.cpp:8566` `converterScanZone()`
runs `drawPunisherClause` (pass 0) and `discardPunisherClause` (pass 1) into one undifferentiated
`names` vector; `feedsRowTag()` (~8611) then prints it under a "the opponent draws N" sentence.
The PARSETEST case at `src/AIPlayerGPT.cpp:43631` bakes the wrong pairing in
(`"...draws 1 extra card per turn; converters on your battlefield: 2 - Underworld Dreams #1, Liliana's Caress..."`),
so this ships as designed. **18 rows across my six seats credited Caress on a draw-feeder row.**
The deck162 guide already carries a hand-written qualifier for exactly this ("when the only name
after K is Liliana's Caress, K counts only while ... 7 or more"; `Opponent hand size: 6` here, so
the guide says read K as 0) — and the surface beat the prose, which is the documented routing
outcome. **Fix at the render, not the guide**: match the converter class to what the feeder hands
them, or split the list ("draw punishers: K1 - ...; discard punishers: K2 - ...") and count only
the class the row actually feeds. The `Teferi's Puzzle Box` row has the same exposure: its
bottom-then-draw is not a discard either (confirmed in the 125 game's narration, "Opponent put a
card from their hand into their library (x9)" — no Caress trigger).

**MED-1 — `askExemplar` truncates the worked answer example at a blind 48 bytes, producing an
unbalanced-quote, mid-word "short name" in the line that defines the required reply shape.**
Repro: `1788544879-ai_baka_deck162-0x55ae9e49ea50-vs-ai_baka_deck146.jsonl`, **seq 10**. Rendered:
`e.g. "CHOICE: 1 (Shield Sphere #1 (0/6) - "Defender -- Whenever S)"`
Site: `src/AIPlayerGPT.cpp:23331` — `if (core.size() > 48) core = core.substr(0, 48);`, with no
word boundary and no quote/paren balancing, then wrapped in `(...)` inside a `"..."`. Two windows
later the same seat produced the corpus's only fallback, `named_row_reask` at seq12
(`CHOICE: 4 (Cast Fate Unraveler)` — number 4 was the HOLD row and that name was on no row; the
guard refused, `recovery` at seq13, clean answer at seq14 — the guard itself worked correctly).
**Fix**: truncate at the last space/`-`/`"` boundary inside 48 and drop any unclosed quote, or on
a named-row ask exemplify with just the card name.

**MED-2 — the carried `YOUR PLAN` line has no length bound, so a reasoning ramble re-enters every
later prompt verbatim.** Repro: `...vs-ai_baka_deck123.jsonl`, **seq 18**, whose carried plan is
1603 characters and includes the model's own live self-correction *"Wait, looking at the board:
Opponent has Intruder Alarm."* and a paragraph of dead arithmetic. Across my six seats the carried
plan is median 387 chars, max 1603. It is paid for on every prompt until replaced, and a plan
containing "Wait," is a reasoning artifact, not a plan. **Fix**: cap the carried plan (a sentence
or ~300 chars), or drop anything after a self-correction marker.

**LOW-1 — no line says why a creature is absent from the blocker list.** Same seq 18: Fate
Unraveler is on my battlefield line untapped and creature-legal, and is not offered as B2 (correct
— it has no flying against 24 fliers), but the render never says so. The lists are authoritative
so no confabulation occurred here; a one-clause reason would still be cheaper than the model
re-deriving evasion from the battlefield block.

**LOW-2 — HOLD economics.** 65 prompts rendered the HOLD row, 3 took it, and
`identical_option_asks_resolved` is 0 in all six seats (`identical_ask_answers_reserved` = 1, in
the 125 game). Not a defect at this deck's decision volume; noted only as a data point for the
engine seat's cross-deck census.

## (3) Guide verdict: **EDIT** (two edits; full revised guide in `strategy.txt`, 52,982 bytes,
within the 41-71 KB band)

The live guide's `=== DECIDING SITUATIONS ===` bullet on the Sanguine Bond + Exquisite Blood pair
ends with a clause that is **factually false** and is half of what lost the 126 game.

**Edit 1** — the loop bullet.
BEFORE: `- Their battlefield line shows BOTH "Sanguine Bond" and "Exquisite Blood" -> any damage you deal them and any life they gain repeats without limit. ATTACK: none, whatever the tags say, until one is gone; the punishers still fire on their draw step.`
AFTER: the same trigger, but it now states that the chain ends on MY life total, spells out the
Underworld-Dreams -> Exquisite Blood -> Sanguine Bond -> me sequence, says one opponent draw with a
punisher of mine out is lethal at any life total, flags the `DRAW FORECAST ... = N life to you`
line as counting the wrong direction while the pair is out, and **suspends** the guide's own "cast
the punisher this ask" and "cast every draw engine" rules until one enchantment leaves.
PAID FOR BY: `...vs-ai_baka_deck126.jsonl` **seq 20** (cast Fate Unraveler into the completed
loop), with seq13/14/15/18 as the feeding chain and the `gameend` record (turn 15, my life 0,
theirs 40). The prior clause is the one the seat was following.

**Edit 2** — a new bullet immediately after it: ONE half of the pair named in the GAME LOG or on
their battlefield, while I hold or control a punisher, is a STOP-COMMITTING signal — no new
punisher, no new draw engine, take the wall / the Caress / "Cast nothing right now" until it
resolves.
PAID FOR BY: the same file's turn-9 narration, verbatim in my own prompt at seq20:
`- Opponent put the revealed Sanguine Bond into their hand` and `- Opponent put the revealed Exquisite Blood into their hand`.
Both were named **four turns** before they killed me, and before the seat's Underworld Dreams cast
at seq13 (turn 10). This is a rule the seat could have executed off information it was shown.

**Not edited, deliberately.** The Caress K-qualifier (guide lines ~89-96) is already correct and
already carries the `Opponent hand size:` discriminator that would have stopped the 152-game
seq13 Mine. Restating it would be noise; the ask is HIGH-2 at the render, per the perception/
strategy routing rule. The blocking, attacking and body-turn rules all executed correctly in this
corpus (123 seq18, 130 seq10/12/16, 146 seq5/7) and are untouched.

## (4) Proposals

None. No general-guide or skill change is paid for by this deck's evidence: the two guide edits
are deck-specific card interactions, and the two HIGH items are render defects for the core loop,
not instruction-surface problems.

## What I did NOT check

- I did not run the game, the suite, PARSETEST, or any build; every claim above is read off the
  translogs, the primitives, and a read-only look at `src/AIPlayerGPT.cpp`.
- I did not verify that the engine's Exquisite Blood / Sanguine Bond loop is *bounded* by design
  rather than by the victim hitting 0 — I inferred the mechanism from the primitives
  (`@lifelostfoeof(player):life:thatmuch controller`, mtg.txt:37967;
  `@lifeof(player) from(*[-lifefaker]|*):life:-thatmuch opponent`, mtg.txt:99005) and from the
  exact -16/+16 match in the 126 game. Whether the `[-lifefaker]` filter is doing anything here is
  unexamined.
- I did not adjudicate any lane-{I,J,H,K} or wave-58 lane-{F,G} prediction — that is the engine
  seat's job; I only report that my six seats show 0 `empty_reply`, 0 translog gaps, 0 aborts.
- I read the opponent seat's log only for the 126 and 123 games; the other four opponents' logs
  were not read.
- I did not check whether HIGH-2's fix would disturb decks whose feeders genuinely cause discards
  (none of deck162's do), nor whether any other deck's guide depends on the merged K count.
