# Wave-81 review — deck152 (Bant Human Midrange)

Seat: 6 games, 285 decision windows (ask 192, priority 59, attackers 23, reveal 6, blockers 3,
bottom 2). Record 5-1. Native reasoning on all 285 (median 5,598 chars, max 20,991; zero records
with empty `reasoning`). 0 fallbacks, 0 `reply_truncated`, 0 `action_before_plan`, 0
`multi_answer_first_taken`, 0 `menu_pass_no_progress`, 0 `plan_names_uncastable_zone_card`, 0
`wall_miss_events`. `forced_close_events` 2 / `_unrecorded` 0. No `WAGIC addToGame REFUSED a
garbaged element` line in any stderr in the corpus dir (bug-list #12 clear for these six games).

## 1. Game by game, and the decisions that decided them

**vs deck50 (Erosion) — WIN, 26 to -7, T15 (log turns).** Curve Katilda T3, Teferi T5, Briarbridge
Tracker T7, Sigarda + Brutal Cathar + Aspirant T13, lethal T15. The deciding run is Teferi: the
seat took a loyalty ability on T5, T7, T9, T11 and T13 (seq8, 12, 27, 42, 59) — five of the six own
turns Teferi spent on the battlefield in that game; the miss is T15, the turn the seat won. Opponent countered 1 spell; no line was lost
to it. Two land drops were declined on T15 (below, §3).

**vs deck162 — LOSS, 0 to 4, T11.** The only loss, and it is not a guide break. Opponent assembled
Howling Mine + Teferi's Puzzle Box + Underworld Dreams x2; the seat's own COMPULSORY draw step on
T11 put 14 damage-triggers on the stack (seq39 prompt: "ON THE STACK: 13 damage ... plus 2 more
that the 1 queued draw on this same stack costs you = 15 in total - you would be at -3; that would
KILL you"). Seventeen `stack_drain_windows_asked`; every menu offered only Katilda's activation,
hold and pass. deck152 has no enchantment answer, so no row on any of those 17 windows changes the
outcome. The race was also played out: T7 Sigarda alone for 5 (Adversary cast that turn), T9
Sigarda + Adversary for 11 taking them 15 -> 4 with a Coven hit (seq27), and the seat's own Elite
Spellbinder correctly exiled Ob Nixilis, the Hate-Twisted from their hand (seq23). One turn short
against a lock the deck cannot break. Route to the pool/rotation call, not to the guide.

**vs deck130 — WIN, 11 to -9, T17.** deck130 is land destruction; the seat sat on 4 lands from T7
to T13 (Stone Rain / Molten Rain in their graveyard) and correctly did nothing with an uncastable
hand (seq8-13). Lair of the Hydra's animate rows were offered 14 times over 7 windows (seq8-15),
every one tagged "[this land is TAPPED: animated, it still cannot attack this turn]"; the seat
passed all 14 — the guide's Lair PASS rule held at 14/14 offers, 0 takes. Fateful Absence spent on
Dwarven Blastminer T4 (seq5), Sigarda T11, lethal alpha T17 (seq30).

**vs deck146 — WIN, 5 to -2, T16.** The close one. At 8 life on T12 with a crack-back floor of 10
printed, the seat cast Tovolar's Huntmaster off the crack-back cover clause (seq26) and then
blocked two Spiders with gang blocks on T13 (seq31) to survive at 5. Fateful Absence was held to
T14 and spent on Lolth, Spider Queen (seq34-35) — correct per the guide's "your ONLY answer to a
PLANESWALKER". Edict at seq12 answered with the Wolf token (the cheapest body) — the edict rule
held.

**vs deck125 — WIN, 22 to 0, T17.** Slow grind against counterspells (2 of the seat's spells
countered). The seat correctly read the counterspell matchup by deploying cheap first. Teferi cast
T17 and his +1 used the same turn (seq63, 66). Katilda's pump activated in upkeep twice (§4).

**vs deck123 — WIN, 21 to -1, T13.** Textbook. "ATTACK: none" at T7 (seq14) with the only A-line
tagged "(your attacker dies, their blocker lives)" — the per-creature STOP rung held. Brutal Cathar
exiled Bloodline Keeper (seq20); alpha T13 for 17 (seq31).

## 2. Engine / interface / card items

**HIGH — the standing "Land drop:" paragraph asserts a separate question that P9 has removed.**
Every prompt carries, verbatim: `Land drop: a land play IS available to you right now. The land
drop is its OWN decision (a "Land drop:" question with its own Play options), so the absence of a
land from the choices below does not mean the drop is gone.` Under P9 that is FALSE on a folded
casting menu — the `Play <land>` rows on that menu are the whole drop, and no separate land
question follows. Both surfaces are live in the same game: `1789690312-ai_baka_deck152-...-vs-ai_baka_deck130.jsonl`
seq20 (T15) is a real standalone `Land drop: which land do you play now, if any?` ask with a `Play
no land right now` row, and seq26 (T17) is the folded casting menu whose decline row reads `Cast
nothing right now (combat comes next this turn) - and play no land in this window`. Repro of the
cost, same file seq26: the reply is `PLAN: Play Plains. Cast nothing. Attack with Sigarda, Champion
of Light and Briarbridge Tracker.` / `CHOICE: 5 (Cast nothing right now)` — the plan says play the
land, the action declines it, because the header promised a separate land question that never came.
The drop was lost that turn. Fix is on the render: print that paragraph only when a separate land
ask will actually be raised, and say on a folded menu that the `Play` rows ARE the drop.

**MED — the `ATTACK TOTAL:` line's blocked-damage clause changed shape and no guide was re-keyed.**
23 `ATTACK TOTAL:` renders on this seat; the clause now takes four forms — `All of it lands
whatever they block` (15), `At least N damage lands whatever they block - they would be at M` (5),
`Their N untapped blockers ... can cover every attacker you could send, so none of that damage is
guaranteed` (3), `that KILLS them whatever they block` (5). The live deck152 guide keyed its second
branch to `...blocking can leave them as high as N`, which renders 0 times in those 23. A seat-side
guide fix is staged (§5, E1) but the same stale literal should be checked in every other deck guide
this wave, since the string is engine-wide, not deck-specific. Repro: `1789690346-ai_baka_deck152-...-vs-ai_baka_deck123.jsonl`
seq14, line beginning `ATTACK TOTAL: 1 attacker listed, 1 total combat damage to a player`.

**MED — `HOW A HOLD ENDS` and the hold row's own text disagree with what the guide pool quotes.**
The rendered row now reads `Hold priority - pass now, and do not ask me again - YOU CANNOT COME
BACK AND TAKE ONE OF THE ROWS ABOVE LATER THIS TURN - it stands until your next turn begins ...`
(174 renders). The string `unless the board changes`, which the deck152 guide quoted as the row's
own words, renders 0 times. Not a behaviour fault this corpus — the seat took 13 holds and all were
released as the render describes — but it is audit debt across the guide pool. Repro: any priority
menu, e.g. `1789690306-ai_baka_deck152-...-vs-ai_baka_deck162.jsonl` seq33.

**LOW — 10 `async_drops` on this seat (corpus 52), all re-asked, none lost a decision.** Every one
is `AIPlayerGPT: dropping stale async answer (casting arm; the question (or turn/phase) moved)
[outcome: re-asked]` (7) or `(casting arm; the question and board moved)` (3). Checked against the
translogs: in each case the window that followed was the correct current question and the seat
answered it. Per game: 152v50 3, 146v152 4, 152v125 2, 152v123 1, 152v130 0 (the 152v130 one is on
the opponent seat's stderr), 152v162 0. Repro line: `game-152v50-1789690301.stderr:1034`.

**LOW — the P5 batch collapse prints the batch's FIRST resulting life total, not its last.**
`1789690306-ai_baka_deck152-...-vs-ai_baka_deck162.jsonl` seq21 GAME LOG: `Underworld Dreams -> 1
damage to you (now 17) [x9 - 9 lines of this shape in this batch; only the numbers in them differ]`.
A reader reconstructing life from the log alone lands on 17; the true figure after the batch was 9.
The `CURRENT SITUATION` header carried the correct life on the same prompt, so no decision was
harmed. Suggested fix: have the collapse print the LAST value ("...to you (now 9)") or both ends.

**LOW — 13 replies (4.6% of 285, vs 2.9% corpus-wide) wrote the plan without the `PLAN:` label.**
All 13 put the plan text BEFORE the action line and all 13 parsed to a valid `choice`
(`protocol_deviation: unlabelled_plan`, `plan_line_missing: true`, `off_protocol_bytes` 14-371). 0
`action_before_plan`. Under the standing ruling — the rule is a robust parser, and any plan must
precede action — these are substantively compliant; recorded so the rate is visible, not as a
defect. Repro: `1789690304-ai_baka_deck152-...-vs-ai_baka_deck50.jsonl` seq37, reply
`Attack with all available creatures to apply pressure.\nATTACK: A1, A2`.

## 3. P9 — the land row on the casting menu

Field: every `ask` window on this seat whose `options_text` contained a row starting `Play `.
Unit: window. N = 42 windows over 6 games (20 on a standalone `Land drop:` question, 22 on a folded
casting menu).

- **Land drop available and NOT taken while a land row was on the menu: 3 windows, 2 distinct
  (game, turn).** All 3 are folded menus; 0 of the 20 standalone land asks were declined.
  - `...vs-ai_baka_deck50.jsonl` seq73 and seq74, both T15, 5 lands in hand, 7 lands out. Plans name
    only the attack; both take row 11 `Cast nothing right now (combat comes next this turn) - and
    play no land in this window`. The T15 log carries no `played <land>` — the drop was lost.
  - `...vs-ai_baka_deck130.jsonl` seq26, T17, Plains in hand, 1 land out. Plan says `Play Plains`,
    choice declines. Drop lost.
- **Land row taken instead of a spell that should have gone first: 0.** Of the 22 folded menus the
  seat took the land row 14 times, a cast row 5 times and declined 3 times; in all 14 land takes the
  casting menu was re-put after the land entered (the menu says so in its own header) and the seat
  cast from it, so no cast was displaced.
- **Measured cost of the 3 declines: 0 life, 0 games.** Both lost drops fell on the turn the seat
  won. The mechanism is nonetheless live and is filed HIGH in §2 — the render tells the pilot a
  separate land question is coming.

## 4. P5 — the compacted GAME LOG

Decisions where the compacted log omitted or mis-stated a fact the seat needed: **0**.
Collapse markers of the `[xN - ...]` form across all 6 games: **1** (quoted in §2, LOW). Every other
turn line carried its draw, its land, its casts with cost in parentheses, and damage with the new
life total; the seat's plans referred to facts on those lines without error. Two side observations,
neither costing a decision: (a) an opponent turn with nothing but a draw renders as `T2 (opp):
drew; played Swamp.` with no phase heading, as designed; (b) the seat's own Coven and Sigarda
reveal chain on T9 of the deck162 game renders as one long line of `you revealed ...` clauses —
readable, but it is the longest single log line in this seat's corpus.

## 5. Guide verdict: **EDIT**

Five edits, all paid for. Byte count against the live baseline `bin/Res/ai/baka/deck152_strategy.txt`
(19,918 B): staged guide **19,951 B** (<= 20,000). A332 grep (incl. `you (declined|took|cast|passed)
[0-9]`) prints **0**. `check-reply-instructions.py src <guide>`: **OK (5 source files, 1 guide)**.
No byte-reclaim or reordering edits were made; every cut below is named as the payer for a specific
addition.

**E1 — the `ATTACK TOTAL:` branch, re-keyed to the strings the engine prints.**
BEFORE: `- "...blocking can leave them as high as N": COMPARE N TO THE LIFE ON THEIR LINE RIGHT NOW.
N AT OR ABOVE IT means the attack can leave them HEALTHIER than they are now ...` (through `...
bringing one more attacker turns a kill into their survival.`)
AFTER: three branches keyed to `All of it lands whatever they block`, `At least N damage lands
whatever they block - they would be at M`, and `can cover every attacker you could send, so none of
that damage is guaranteed`, each ending in a send instruction; the `that KILLS them whatever they
block` branch keeps its imperative and loses its leading ellipsis (the engine prints no ellipsis).
PAID BY: literal audit — 0 renders of `blocking can leave them as high as` in 23 `ATTACK TOTAL:`
lines, against 15 / 5 / 3 / 5 renders of the four live forms. Window arose, string did not print.
PREDICTION: over the next corpus, every attackers window on this seat whose `ATTACK TOTAL:` line
ends in one of the four live clauses is answered with a send-list consistent with that clause's
branch. Denominator: attackers windows with an `ATTACK TOTAL:` line (23 here). A hit is a break only
if the declared attackers include a body whose own `their untapped blockers` tag is a stop, or omit
an A-line tagged `[no creature they control can block this attacker]` while the clause is `All of it
lands whatever they block`. REMOVAL CONDITION (information-supplying): none — this is the rule's own
key, retired only if the engine changes the clause again.

**E2 — the land-drop rule fires on a `Play` row wherever it sits.**
BEFORE: `- A "Land drop:" question offering a land -> PLAY IT. It costs no mana and does not reduce
what you cast this turn, so never "Play no land right now" - ...`
AFTER: `- ANY row beginning "Play " -> TAKE IT, whether it stands on a "Land drop:" question of its
own or among the cast rows of a casting menu; either way that row IS the drop and no second land
question follows it. It costs no mana and does not reduce what you cast this turn - the casting
decision is asked again once the land enters - so never "Play no land right now", never "and play no
land in this window" - ...`
PAID BY: the 3 declines in §3 (`...vs-ai_baka_deck50.jsonl` seq73, seq74; `...vs-ai_baka_deck130.jsonl`
seq26), and specifically the plan/choice contradiction at deck130 seq26. This is the guide half of
the HIGH render item in §2 and does NOT replace it: the prompt still asserts the separate question.
PREDICTION: 0 windows in which a `Play <land>` row is on the menu, a land drop is available, and the
seat answers a row containing `play no land in this window`. Denominator: folded casting menus
carrying a `Play ` row (22 here). A hit is a break only if a land drop was still available on that
window (the `Land drop: a land play IS available` line present).
REMOVAL CONDITION (information-supplying): one corpus in which the render no longer prints the
false separate-question paragraph on folded menus AND the P9 decline count is 0.

**E3 — Katilda's pump is a BEFORE-COMBAT action, not a MAIN 2 action.**
BEFORE: `Her "{4}{G}{W}, {T}: +1/+1 counter on each creature you control" is MAIN 2 ONLY: after
attackers are declared, three or more creatures out, spare mana.`
AFTER: `Her "{4}{G}{W}, {T}: +1/+1 counter on each creature you control" TAPS HER, so she cannot
attack the turn you take it: take it BEFORE combat, in your upkeep or main 1, and the counters are
on the team when damage is dealt. Three or more creatures out, and only while the row's "spends N of
your untapped mana sources this turn" clause leaves enough for the creature you mean to cast.`
PAID BY: the rule is refuted by the primitive and was disobeyed at 4 of 4 firings. Primitive
(`bin/Res/sets/primitives/borderline.txt`): `auto={4}{G}{W}{T}:name(Put 1/1 counters)
all(creature|mybattlefield) counter(1/1)` — the tap is part of the cost, so a Katilda that has been
declared as an attacker is tapped and CANNOT pay it; "after attackers are declared" is reachable
only with Katilda held home, in which case the counters land after combat damage and buy nothing
this turn. Takes, all in Upkeep, none in Main 2: `...vs-ai_baka_deck50.jsonl` seq70 (T15),
`...vs-ai_baka_deck146.jsonl` seq32 (T14), `...vs-ai_baka_deck125.jsonl` seq27 (T11) and seq49
(T15). The mana clause in the new text is the real cost the old rule was groping at: at
`...vs-ai_baka_deck146.jsonl` seq32 the row printed `{spends 6 of your 9 untapped mana sources this
turn; Sigarda, Champion of Light {1}{g}{w}{w} in your hand needs 4}` and the activation left 3 — the
Sigarda cast was denied that turn.
PREDICTION: every Katilda pump take in the next corpus is at Upkeep or Main phase 1, and none is
taken on a window whose row prints a `spends N ...; <card> in your hand needs M` clause the take
would break. Denominator: windows offering `put 1/1 counters with Katilda, Dawnhart Prime` (4 here).
A hit is a break only if the take is at Main phase 2, or the denied card is a creature the deploy
floor would otherwise have cast that turn.
REMOVAL CONDITION (belief-correcting): the render carrying a `this taps the source - it cannot
attack this turn` clause on the Katilda row AND zero Main-2 takes across two corpora.

**E4 — Elite Spellbinder's fallback, and a card fact that is wrong for this pool.**
BEFORE: `Exile "Exquisite Blood" or "Sanguine Bond" above everything; with neither listed take
IDYLLIC TUTOR, which fetches them.`
AFTER: `Exile "Exquisite Blood" or "Sanguine Bond" above everything; with neither listed take the
row whose text makes BODIES or COUNTERS every turn - a "{T}: Put a ... token" line, a planeswalker,
a lord - and never an instant or sorcery whose whole text is a draw or one point of damage.`
PAID BY: (a) the card fact is wrong here — the only pool deck running Idyllic Tutor is deck123
(`bin/Res/ai/baka/deck123.txt:24`, `Idyllic Tutor * 4`), and its own `#HINT:combo` line fetches
Intruder Alarm; neither Exquisite Blood nor Sanguine Bond appears in any of the six pool
decklists, so "which fetches them" is false as written. (b) The fallback is unowned and the seat
misplayed it: `...vs-ai_baka_deck123.jsonl` seq12 (T7) offered Lightning Greaves, Vision Skeins,
Bloodline Keeper (3/3 flier, `{T}: Put a 2/2 black Vampire creature token onto the battlefield`) and
Thraben Doomsayer (`{T}: Put a 1/1 white Human creature token`), and the seat exiled Vision Skeins
(`Each player draws two cards.`). The Bloodline Keeper then resolved and had to be answered by
Brutal Cathar's ETB at seq20 (T11) — spending the exile the guide reserves as removal. Correct picks
elsewhere: seq23 in the deck162 game (Ob Nixilis over two Master of the Feast) and seq28 in the
deck123 game (Intruder Alarm).
PREDICTION: on every Spellbinder hand-look whose list contains a row printing `{T}: Put a` or
`[planeswalker]`, the chosen row is one of those. Denominator: Spellbinder hand-look windows (3
here). A hit is a break only if a `{T}: Put a` / `[planeswalker]` row was on the list and an instant
or sorcery was taken instead.
REMOVAL CONDITION: none (a card-fact correction).

**E5 — the edict section's trigger, re-keyed to the header the engine prints.**
BEFORE: heading `A "you SACRIFICE this" MENU IS AN EDICT - ITS LIFE NUMBER IS A PRICE, NOT THE
CHOICE` / `You pay one of these rows whatever you answer, so the smallest "they gain N life" is not
the answer.`
AFTER: heading `A "FORCED sacrifice OF YOUR OWN CARD" MENU IS AN EDICT - EVERY ROW IS A CARD YOU
LOSE` / `You pay one of these rows whatever you answer and no row declines.`
PAID BY: literal audit. The string `you SACRIFICE this` renders 0 times; the two forced-sacrifice
windows on this seat both open with `FORCED sacrifice OF YOUR OWN CARD: the opponent's effect
(<card>) forces YOU to sacrifice one of your OWN cards from the list below` —
`...vs-ai_baka_deck146.jsonl` seq12 (Silverquill Command) and seq38 (Soul Shatter). Neither menu
prints a life number at all, so the old sentence's `"they gain N life"` comparison describes a screen
that is not there. The window arose and the string did not print, which makes it a defect whether or
not the rule held — and the rule did hold, 2 of 2. This edit is byte-NEGATIVE (-33) and is one of the
payers below.
PREDICTION: on every forced-sacrifice window, the row taken is the cheapest body on the list (a token
before a creature printing no ability, before any lord or mana source). Denominator: windows whose
prompt contains `FORCED sacrifice OF YOUR OWN CARD` (2 here). A hit is a break only if a token or a
no-ability body was on the list and a lord, a mana engine or a larger body was taken instead.
REMOVAL CONDITION: none (this is the rule's key).

### Which rules paid the bytes

The five edits add 370 bytes gross. The payers, each a cut this corpus's evidence licenses:

1. `GENERIC IS NOT A COLOUR: Ranger Class {1}{G} is one green pip plus one mana of any kind, so
   "2 total ... colours: {g}" pays it.` -> the rule keeps its second sentence only. A worked example
   on a rule that held: 0 mana misreads on 285 windows, and its quoted literal renders 0 times.
2. `- Between hands that clear the floor, prefer more lands and the cheap threats.` -> deleted. An
   unreachable condition: the mulligan ask is keep-or-ship on ONE hand and never offers a choice
   between two hands (8 `Pre-game mulligan decision` renders, all two-row).
3. `What a hand cannot cast on turn two it casts on turn four.` -> deleted. A rationale restating the
   floor stated in the same bullet; the M-of-7 floor held at 3 of 3 mulligan windows.
4. `A COLOUR reason ("no spell in it is castable off {W}{U} alone")` -> `A COLOUR reason`. The quoted
   literal renders 0 times against 10 renders of the coverage line it sits beside; the rule stands.
5. `Never hold a name back on memory or on your plan.` -> deleted. Restates the plan-is-not-evidence
   rule the MULLIGAN and matchup sections both own.
6. `and the +1/+1 that body gives your board dies with it.` -> deleted. A restatement inside the
   `(both die)` rung, whose imperative is unchanged.
7. E5 itself, -33 bytes (above).
8. `with no exception for a turn you mean to win` -> deleted from the Lair PASS rule. The rule held
   at 14 of 14 offers with the concession never exercised; it is also an `even when`-class concessive
   (A334) attached to an absolute.

### Rules that HELD — kept unchanged, with their evidence

- LAND DROP on a standalone `Land drop:` ask: 20 of 20 taken.
- Lair of the Hydra PASS on `[this land is TAPPED: ...]`: 14 offers, 0 takes.
- DEPLOY FLOOR. Field: main-phase `Casting decision` windows whose `options_text` carried a `Cast `
  row printing a (P/T). N = 60 windows. 36 took a creature row, 12 took a `Play <land>` row (the menu
  is re-put after the land enters), 3 took a body-making non-creature row or the only affordable row
  (two Ranger Class, which makes a 2/2 Wolf; one Teferi where the creature rows printed `needs more
  mana sources than that`), and 9 declined. All 9 declines sit on a turn where the attack that
  followed was lethal or a creature had already been cast that turn: `...vs-ai_baka_deck50.jsonl`
  seq73/74/76 (T15, and the only creature row was tagged `[legendary: you already control Sigarda,
  Champion of Light ...]` with a healthy copy out — carve-out (c)), `...vs-ai_baka_deck130.jsonl`
  seq26/28 (T17), `...vs-ai_baka_deck146.jsonl` seq28 (T12, Huntmaster cast at seq26 the same turn)
  and seq40/41 (T16), `...vs-ai_baka_deck123.jsonl` seq26 (T13). 0 unsanctioned declines.
- FATEFUL ABSENCE: the `the only legal targets are YOUR OWN right now` clause rendered on 65 windows
  (30 in the deck50 game, 35 in the deck125 game) and the card was cast on NONE of them. It was cast
  twice in the whole seat, both with legal opposing targets: `...vs-ai_baka_deck130.jsonl` seq5 (T4,
  Dwarven Blastminer) and `...vs-ai_baka_deck146.jsonl` seq34 (T14, Lolth, Spider Queen — the
  planeswalker rung).
- ATTACKING per-creature STOP `(your attacker dies, their blocker lives)`: 2 renders, 2 obeyed
  (`...vs-ai_baka_deck123.jsonl` seq14, `ATTACK: none`).
- EDICT rule (cheapest body, token first): 2 firings, 2 correct — `...vs-ai_baka_deck146.jsonl` seq12
  gave up the Wolf token over Katilda, seq38 the Briarbridge Tracker over the Moonrage Brute. The rule
  held; its TRIGGER did not render, which is why E5 below re-keys it.
- TEFERI "take a loyalty ability EVERY turn": 6 loyalty takes over the 7 own turns Teferi spent on
  a battlefield in this seat's games (5 in the deck50 game, 1 in the deck125 game at seq66). The one
  miss is the deck50 T15, the turn the attack was lethal.
- INTREPID ADVERSARY "don't add any counter only with no spare mana": 2 firings, 2 correct
  (`...vs-ai_baka_deck123.jsonl` seq8 tapped out, declined; `...vs-ai_baka_deck162.jsonl` seq9 with
  2 spendable, added 1).
- MULLIGAN floor: 8 `Pre-game mulligan decision` windows, 8 correct. The two ships are the two hands
  the floor ships — `...vs-ai_baka_deck146.jsonl` seq2 and `...vs-ai_baka_deck123.jsonl` seq2, both
  `counted by the engine: 1 land, 6 spells` with `Playing every land in this hand would not cover any
  spell in it` — and both following keeps clear the floor with a coverage line naming cards.

### Violated-unpunished, recorded and NOT acted on

The LUMINARCH ASPIRANT size test (`if your pick reads 6/6 or larger, choose another`) was broken 4
times of 19 Aspirant trigger target picks (field: windows whose every option row is one of the seat's
own battlefield creatures, at the `Combat begins` or `Blockers` seam): `...vs-ai_baka_deck130.jsonl` seq29 (Sigarda 6/6),
`...vs-ai_baka_deck146.jsonl` seq36 (Tovolar's Packleader 8/8), `...vs-ai_baka_deck125.jsonl` seq79
and seq81 (Moonrage Brute 6/6 then 7/7). In every one the oversized body was the evasive or
first-striking attacker that actually connected, and all four turns were won. The summoning-sick
clause was broken once (`...vs-ai_baka_deck146.jsonl` seq17, Brutal Cathar tagged `[summoning sick -
cannot attack this turn, but CAN block]`), wasting that combat's counter. Violated-unpunished is its
own verdict and does not loosen the rule; no edit, re-measure next wave. If the 6/6 test breaks
again on a turn that is NOT lethal, the rule is one the pilot does not have and the test should be
replaced by the row's own evasion tags rather than a size number.

### Untested at this seat

`(neither dies)` rendered 0 times across 23 `ATTACK TOTAL:` windows and 3 blockers windows; `(blocking trigger: they
gain N)` 0; `LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs` 0; `...N tramples to your face` 0;
`[repeat: activated this turn ...]` 0. All five branches are reachable in the pool and their rules
STAY. The discard, order and X seams raised 0 windows on this seat, so BOTTOMING/CLEANUP DISCARD
(1 `bottom` pick per game in 2 games, no cleanup discard), the P10 blockers ORDER answer and the X
rules are UNTESTED here — no window, not a failure.

## 6. What I did NOT check

- The opponents' own seat logs beyond using them for board context; no opponent-side misplay is
  claimed or priced here.
- Any engine or card-script source. No primitive was read except `Katilda, Dawnhart Prime` and
  `Luminarch Aspirant` (for E3 and the Aspirant note) and the six pool decklists (for E4).
- The other six decks' guides, although the §2 MED items (`ATTACK TOTAL:` clause, hold-row literal)
  are engine-wide strings that every guide should be audited against this wave.
- Whether the deck162 draw-punisher lock is answerable by a sideboard or a pool change — that is a
  rotation question, not a guide question, and I did not attempt it.
- Latency, token budgets and cost: not measured.
