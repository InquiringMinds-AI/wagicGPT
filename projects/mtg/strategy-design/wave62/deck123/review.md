# wave-62 deck123 review — Intruders Of Thraben (WBU Intruder Alarm combo)

Corpus `matchups-20260905-133058`, binary `12be0b79c`. Seat files are the six
`*-ai_baka_deck123-0x*-vs-*.jsonl`; seq numbers below are that seat's unless a file is named.
**Record 0-6.** No fallbacks except one `wall_miss_unrecorded_wall` (162 game, seq 16, recovered).

## 1. Game by game, and the decision that decided it

**vs deck146 — LOSS, turn 11, 0 vs 21** (`1788633098-…-vs-ai_baka_deck146`). Curve was clean:
Tutor for Intruder Alarm (seq 10-11), Bloodline Keeper T8 (seq 13, Soul Shattered), Doomsayer T10
(seq 14). **Deciding decision: seq 18.** At 10 life against 2+4+2+4 the header printed
`best case with every blocker assigned: you would be at 2 - one legal assignment that reaches it:
Thraben Doomsayer blocks Nadaar, Selfless Paladin`. The reply's first line was `BLOCKS: B1:A3` —
the 2-power Goblin, the smallest attacker — and its own PLAN line, written *after* the answer,
says *"Thraben Doomsayer blocks Nadaar."* The seat then believed it had blocked Nadaar: at seq 19
its PLAN again reads *"Thraben Doomsayer blocks Nadaar"* while the battlefield line says
`[blocking Goblin #1]`. 10 damage connected, 10 → 0. PERCEPTION/protocol, not strategy: the model
reached the right assignment inside the same reply and the protocol had already spent it.

**vs deck126 — LOSS, turn 15, 0 vs 40** (`1788633085-…-vs-ai_baka_deck126`). The seat *executed*:
two Intruder Alarms (seq 8, 10), two Thraben Doomsayers, 81 Human tokens by turn 15, and it
correctly obeyed the guide's attack rule under the Sanguine Bond + Exquisite Blood board (seq 55
`no attackers`; plan *"Sanguine Bond + Exquisite Blood loop prevents attacking"*).
**Deciding decision: seq 76.** One 1/1 lifelink Vampire attacking, seat at 16, 83 blockers. Header:
`INCOMING THIS COMBAT: 1 attacker, 1 unblocked damage - you would be at 15 … best case with every
blocker assigned: you would be at 16`. Rows B1-B83 each carry
`{their attacker's lifelink, this block: they gain 1 from this block only, …}` beside
`(you kill it, your blocker lives)`. It blocked. seq 77-78 events: the 1 lifelink gain chained
16 → 5 → 0 while they went 24 → 40. **Declining the block ends at 15; the engine's own "best case"
named the fatal line.** See HIGH-1.

**vs deck162 — LOSS, turn 14, 0 vs 14** (`1788633069-…-vs-ai_baka_deck162`). Best game of the six:
Keeper T8, second Keeper T10, Intruder Alarm T12 (seq 38), and on turn 13 the loop fired properly
on *their* turn — seq 43-52 built 54 creatures under Lord of Lineage against an opponent on 14.
It never got to attack. **The deciding fact is a render that told the truth and had no answer:**
seq 53/54 `DRAW FORECAST: your next draw step draws 11 cards (1 + Howling Mine #1 1 + Dictate of
Kruphix 1 + Howling Mine #2 1 + Teferi's Puzzle Box: your hand size 7) … you would be at -5; that
KILLS you`, and at seq 55 the same line in the tense `your draw step, resolving NOW, draws 12`.
Underworld Dreams took it 6 → 0 in its own draw step. I found no legal out at seq 53-63 (tapped
out, hand was lands + a draw spell); the seat's hand size was 8 of those 12 draws via Teferi's
Puzzle Box, but no window let it shed the hand. **No misplay at the deciding window; D8's tense
and KILLS-you verdict both rendered correctly and were read correctly.**

**vs deck152 — LOSS, turn 17, -5 vs 20** (`1788633075-…-vs-ai_baka_deck152`). Damnation at T10 was
right (`destroys 3 of their creatures`). **Deciding decision: seq 36.** At 4 life, with
`CRACK-BACK NEXT TURN: … up to 10 … you would be at -6 or lower; that would KILL you - of that, 8
from 2 attackers nothing you control can legally block`, its only untappable body was the lone
Greaves-wearing Thraben Doomsayer (22 tokens summoning sick), whose own row read
`Elite Spellbinder #1 (5/3) (your attacker dies, their blocker lives)`. It answered `ATTACK: A1`.
STRATEGY: this violates two rules already written in the guide (CHECK 0's crack-back total, and
CHECK 5's "do not send a LONE Thraben Doomsayer … its own row says when"). A compliance miss on
an existing rule, not a gap — and the game was already lost by then.

**vs deck125 — LOSS, turn 38, -7 vs 45** (`1788633095-…-vs-ai_baka_deck125`). Variance did most of
it: Bloodline Keeper was Cancelled on cast (T20, seq 38) and Thraben Doomsayer exiled the turn
after it resolved (T26, seq 55), so the assembled Alarms never had a maker. **Three dead casts
against the row's own verdict tag**, all in this game: seq 26 and seq 30 `Cast Devour Flesh
{right now: they control 0 creatures - at 0 this does nothing; YOU control 0 creatures - targeting
yourself does nothing}`, and seq 53 `Cast Damnation {right now: destroys 0 of their creatures …,
0 of yours}`. STRATEGY, and again against rules the guide already states verbatim (RULE 5 step (2)
"If N is not exactly 1, do not cast an edict - not at 0"; RULE 3 "At N of 0 the answer is hold").

**vs deck130 — LOSS, turn 36, 0 vs 20** (`1788633083-…-vs-ai_baka_deck130`). Budde's Ponza stripped
the board: at seq 26 (turn 33) `Your battlefield (1 permanent listed, … and 0 are lands)`. Turns
13-35 contain no casting window at all because the seat had no mana — that is land destruction,
not a hold latch. **Deciding decision: seq 27**, and it is an interface item, not a play: see
HIGH-3. The reply spent 7,680 bytes convincing itself that generic mana cannot be paid with
coloured mana ("*{2} is generic. It must be paid with colorless mana … So I CANNOT pay {2} with
{U}*"), concluding the deck was uncastable. The prompt sentence that refutes this was not on that
screen.

## 2. Engine / interface / card items

**HIGH-1 — the blockers header and the per-B-row block-gain clause are not priced against an
opposing life LOOP. D2's fix is attack-side only.**
Repro: `1788633085-ai_baka_deck123-0x557b7b88b130-vs-ai_baka_deck126.jsonl` seq 76, turn 15,
`kind: blockers`, seat at 16 life. Rendered:
`INCOMING THIS COMBAT: 1 attacker, 1 unblocked damage - you would be at 15 (your creatures may
legally block every attacker in that total) - best case with every blocker assigned: you would be
at 16 - one legal assignment that reaches it: Thraben Doomsayer #1 blocks Vampire #1`
and on every B row `{their attacker's lifelink, this block: they gain 1 from this block only, and
this attacker deals nothing to your life}`.
The same prompt's own banner says *"Any life they gain, or any life you lose, chains until YOU are
at 0 - so ANY nonzero payment on a tag above is fatal, not merely expensive."* Sanguine Bond
(`mtg.txt:99005 @lifeof(player)…life:-thatmuch opponent`) and Exquisite Blood
(`mtg.txt:37967 @lifelostfoeof(player):life:thatmuch controller`) are both on their battlefield, so
the gain is unbounded — seq 78's own event list is eleven `You lost 1 life` / `Opponent gained 1
life` pairs, 16 → 0. `blockGain` is exactly the bounded ceiling lane X removed from
`attackTotalLine`; the blockers emitter still treats it as one. Both halves are wrong at once: the
row's word **"only"**, and a `best case` of 16 when the true best case is 15 with `BLOCKS: none`.
Corpus scope: 1 of 21 blockers windows, and 1 of 1 where a life loop was on their side — rare,
and it cost the whole game from a position the seat had won on board.

**HIGH-2 — D9 recurs and is game-losing here: the coded answer is committed before the reasoning
that contradicts it.**
Repro: `1788633098-ai_baka_deck123-0x560325d49fb0-vs-ai_baka_deck146.jsonl` seq 18. Reply, verbatim
and complete: `BLOCKS: B1:A3` / `PLAN: Survive this combat with Thraben Doomsayer alive. …`. The
header on that screen names `Thraben Doomsayer blocks Nadaar, Selfless Paladin` as the assignment
that reaches 2 life; seq 19's PLAN then states the seat blocked Nadaar. Result: 10 → 0, seq 20
gameend. Note the asymmetry the corpus shows in the same seat: at
`…-vs-ai_baka_deck130` seq 27 the engine DID honour the later, prose-embedded answer
(`parse_note: decision_reversed_in_prose`, `answer_replaced: true`, coded line `CHOICE: 2
(… Tundra)` resolved to row 3, Underground Sea). BLOCKS/ATTACKS do not get that treatment. Either
extension named in D9 would have saved this game.

**HIGH-3 — the generic-cost clarifier is suppressed on exactly the screens that provoke the
misconception.** `Mana available: N total (… mana of ANY colour pays a generic cost like {2})`
renders that clause on **1922 of 1922** nonzero-mana lines corpus-wide and on **0 of 177**
`Mana available: 0 total (no untapped sources)` lines. Repro:
`1788633083-ai_baka_deck123-0x5647178d3220-vs-ai_baka_deck130.jsonl` seq 26 (turn 33) prints
`Mana available: 0 total (no untapped sources)` with no clause; the very next window, seq 27, is
where the model wrote *"Colored mana cannot be used for generic costs … This deck is uncastable"*
and burned 10,799 bytes of overrun on it. seq 28, back at 1 source, carries the clause again.
Four windows corpus-wide across three decks reason from this false rule
(126v125 s73, 123v130 s27, 126v146 s58, 152v162 s7). The clause costs ~50 bytes; it is missing
only where a seat is most likely to conclude its own deck is uncastable.

**MED-1 — the blockers `best case` line is reprinted after blockers are declared, naming an
assignment that is no longer legal, and its own attacker count contradicts itself.**
Repro: same 146 file, seq 19 (`kind: ask`, phase Blockers, after `- You declared blockers: Thraben
Doomsayer blocks Goblin`): `INCOMING THIS COMBAT: 4 attackers, 10 unblocked damage - you would be
at 0; this KILLS you (your creatures can legally block at most 1 of those 3 attackers at once, so
at least 2 of them go unblocked) - best case with every blocker assigned: you would be at 4 - one
legal assignment that reaches it: Thraben Doomsayer blocks Nadaar, Selfless Paladin`. Blockers were
locked one window earlier and the seat's only creature is already blocking Goblin #1, so "at 4" is
unreachable; `4 attackers` and `those 3 attackers` in one sentence are the same emitter disagreeing
with itself. 1 of 23 windows carrying the line; it was on a lethal screen the seat then lost.

**MED-2 — D16 recurs on a wide margin.** Same 130 file, seq 27: `post_answer_overrun 10799`,
`post_plan_overrun 3663`, `reply_trimmed_bytes 3197`, and the record's own tail
`[+3263 bytes written past the PLAN line trimmed from this record]`. At seq 28 the plan came back
as `[…the rest of your plan was not carried: 6818 further characters, of 7129 you wrote - restate
it in a sentence or two if you still mean it]`, i.e. 96% of the plan discarded and the surviving
4% is a mid-sentence self-correction (`"…Swamp provides {B} - wait, Tundra provides {U} and {W}"`)
quoted back as the seat's standing plan. The truncation keeps the PREFIX; the seat's conclusion is
always at the END.

**LOW-1 — `decision_reversed_in_prose` is undocumented in the reply protocol.** Three windows in
these six games (`…deck130` s27, `…deck126` s30/34/62/64) had the coded CHOICE line overridden by a
later prose answer. In every case here it improved the play, but the protocol block still says
*"On the FIRST line write CHOICE: … Write nothing else"*, so the behaviour is unreachable to a
model that follows the instructions — which is precisely why HIGH-2 was fatal on the BLOCKS seam.

**Card facts** (checked against `bin/Res/sets/primitives/mtg.txt`, not Scryfall — no disagreement
found needing a Scryfall call): Bloodline Keeper 12582-12592, Intruder Alarm 58850-58856, Thraben
Doomsayer 120583-120592 (`this(controllerlife < 6) lord(other creature|mybattlefield) 2/2` matches
Oracle's "5 or less life", other creatures only), Devour Flesh 29133-29138, Sanguine Bond 99004,
Exquisite Blood 37966. All rendered rows I checked quoted these faithfully.

## 3. Guide verdict: **EDIT**

Only one of the six losses is paid for by a gap in the guide rather than by variance or by a rule
the guide already states. The guide's loop coverage (CHECK 1, BRANCH B2) is complete and it WORKED
— the deck126 seat refused to attack for the whole game on the strength of it. But BRANCH B2 is
scoped to attacking, and the blocking section's own words then instructed the fatal block. Five
edits, one of them the rule, four of them the byte budget that paid for it. New size **70,995 B**
(was 70,982; band 41-71 KB).

**Edit 1 — new `STEP -1` at the head of the blocking order (the rule).**
before: blocking began at `STEP 0 - A "B" ROW IS A BLOCKER`, and STEP 0 ended
`A row on a token-maker reading "(you kill it, your blocker lives)" is a free kill: take it.`
after: a `STEP -1 - ON THE LOOP BOARD, A BLOCK THAT GAINS THEM LIFE IS LETHAL, NOT A TRADE`
bullet — both halves of the loop named, `they gain N from this block only` declared fatal, the
`best case with every blocker assigned` line declared unpriced and wrong, `BLOCKS: none` when every
attacker carries a life-gain clause, and the 16 → 0 cost.
paid for by: `…vs-ai_baka_deck126` seq 76 → seq 78 events → seq 79 gameend.

**Edit 2 — STEP 0's free-kill sentence gated.**
before: `… is a free kill: take it.`
after: `… is a free kill: take it, unless STEP -1 above says it is not free.`
paid for by: the same seq 76 — that sentence is what the row `(you kill it, your blocker lives)`
matched.

**Edit 3 — the DECIDING SITUATIONS "NOT lethal" bullet gets the same exception.**
before: `… a TOKEN still goes in front of the biggest attacker. A 2/2 you can remake is not a cost,
and the "you would be at" number is the life your combo has to live on next turn.`
after: same, plus `- UNLESS the row gains them life under a Sanguine Bond + Exquisite Blood loop,
which is STEP -1 and blocks nothing.`
paid for by: seq 76's header, which read exactly `- NOT lethal: block only where the trade favors
you.` — this bullet was the quick-reference the seat could have landed on.

**Edit 4 — the two near-lethal blocking rules merged into one.**
before: `WHEN THE PROMPT SAYS LETHAL, ASSIGN TO THE BIGGEST ATTACKERS FIRST …` and, three bullets
later, `Add up the damage of every attacker with no blocker on it. If that total is your life total
minus 4 or more, put a blocker on EVERY attacker …` — two triggers, one action, stated twice.
after: one bullet carrying both triggers, the same ordering rule, the same spend-your-last-blocker
clause, and a new cost line: the 146 game's `BLOCKS: B1:A3` against a header naming Nadaar.
paid for by: `…vs-ai_baka_deck146` seq 18. Duplication was not buying compliance; the seat broke
the rule while both copies were on the page.

**Edit 5 — four compressions, no rule removed**, to stay inside the band: RULE 2's untagged-fetch
paragraph, CHECK 0's crack-back anecdote, the `A TOKEN IS NOT A MAKER` anecdote, the Damnation
anecdote and the Check-1 arithmetic restatement each lose words, not clauses; and the DECIDING
SITUATIONS bullet that repeated STEP 1 verbatim (`your first blocker is a Human or Vampire TOKEN`)
is dropped, since Edit 3 now carries `tokens first, makers last` in its place.

**Not changed, deliberately.** The three dead casts (125 game seq 26/30/53) and the lone-maker
attack (152 game seq 36) each break a rule the guide already states in the words the row uses.
Restating them would be the wave-58 pattern.

## 4. What I did NOT check
- The opponents' seat files, except to read deck130's and deck126's decklists and the shared game
  log rendered in my seat's prompts. No opponent-side misplay is assessed.
- The rerun `matchups-20260905-161801` (152v125 only — no deck123 pairing).
- Any lane prediction adjudication: that is the engine seat's job. I note in passing that D1's
  enters-tapped classifier resolved correctly on every Isolated Chapel row I read, and that D8's
  tense and KILLS-you verdict both rendered correctly at 162 seq 53-55, but I did not count them.
- `async_drops`, `transport`, timeouts, `deadline_pct`, HOLD prefix-match census — engine seat.
- Whether the 130 game's turn 13-35 silence has any cause beyond zero lands (I confirmed zero
  lands on the battlefield at seq 26 and 0 mana at seq 26/27, and did not audit the hold latch).
- I did not run wagic, build, or touch anything outside `wave62/`.
