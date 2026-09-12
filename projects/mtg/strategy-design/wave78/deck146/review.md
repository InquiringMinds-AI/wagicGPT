# Wave 78 — deck 146 (Orzhov dungeon midrange) per-deck review

Seat files: `matchups-20260911-125420-final/*ai_baka_deck146-0x*-vs-*.jsonl` (6 games, single binary
`wagic-47ef378c8-w78step1`). Live guide reviewed = the wave-77 edition installed at 031db297b
(19,998 B). Opponent seats read for board/log context only.

Seat census: **296 model decisions** (ask 227, priority 31, attackers 29, blockers 6, reveal 2,
bottom 1) plus 2 `forced_close` records, **0 fallbacks** — 0 `unparsed_reply`, 0 `empty_reply`,
0 `reply_truncated`, 0 `action_before_plan`, 0 `defer`, 0 `recovery`; none of the corpus's 8
fallbacks is mine, so there is nothing to quote verbatim here. `thinking` on and native reasoning
on 296/296 (median 5,100 chars, max 12,268); the only two records with no `thinking` field are the
two `forced_close` rows, which carry no reply. Latency p50 86.0 s, p95 169.5 s, max 284.6 s;
`deadline_pct` p50 9.5, max 31.6. `off_protocol_bytes>0` 36/296 (12.2 %), `plan_line_missing`
42/296 (14.2 %), `protocol_deviation` compliant 254 / `unlabelled_plan` 36 / `plan_absent` 6 /
`answer_label_absent` **0**. Gameend sums for this seat: `ask_replays_reserved` 14,
`identical_ask_answers_reserved` 11, `hold_windows_skipped` 138 (cast 127 / priority 11),
`hold_released_turn` 9, `crossphase_identical_reputs` 5 / `crossphase_board_unchanged` **0**,
`main_phase_windows_skipped` 27, `own_turn_windows_skipped` 89, `async_drops` 2,
`forced_close_events` 2 / `forced_close_unrecorded` 1, `blocker_forecast_rows` 38 (multi 14, gang 8,
collapsed 4), `plan_names_uncastable_zone_card` 0, `plan_names_stranded_card` 1,
`phase2_answer_recovered` 0 / missing 0, `declined_face_latches` 0, `wall_miss_events` 0,
`menu_pass_no_progress` 0, `identical_option_asks_resolved` 0. **Zero renders on this seat of the
wave-78 additions S3 (`stop_reached_windows_skipped` 0), S4 (`stack_drain_windows_asked` 0), S6
(`own_loop_windows_asked` 0) and `mana_only_windows_skipped` 0** — all UNTESTED here, not passed.
Max prompt 26,923 B (`146v125` seq 141); 52 of 296 prompts over 20 KB (17.6 %; wave 77: 31,701 max
and 36 %).

| # | Opponent | Result | Turn | Final life | What decided it |
|---|---|---|---|---|---|
| 1 | 123 | **LOSS** | 10 | -8 / 16 | Intruder Alarm + Thraben Doomsayer made 24 tokens on T8 with no instant in hand; unanswerable |
| 2 | 125 | **WIN** | 34 | 14 / 0 | both boards creatureless for 12 turns; the seat held Soul Shatter/Command correctly and won the long game |
| 3 | 126 | **WIN** | 15 | 17 / 0 | Silencer on Exquisite Blood, Kaya -3 on Chromatic Lantern, Verse on Exquisite Blood; won despite a missed attack at seq 40 |
| 4 | 130 | **WIN** | 22 | 15 / -1 | Kaya +1 then -3 on Siege-Gang Commander, dungeon drains, Nadaar + Goblins |
| 5 | 152 | **LOSS** | 16 | -1 / 19 | **seq 26** — a 3-Spider gang block on a 6/6 TRAMPLER that the render priced as stopping all 6; the seat went 16 -> 1 instead of 10 |
| 6 | 162 | **WIN** | 10 | 20 / -2 | Silencer on Master of the Feast, Verse, Hive animated for the alpha strike |

**4-2** (wave 77: 5-1). Hands are real; only the 152 loss carries a decision item, and its root cause
is a false render clause, not the guide.

## 0. Wave-77 items: what closed, what paid, what is still untested here

- **Wave-77 EDIT 2 (the Kaya -3 ladder) — FIRED AND PASSED 3/3, and it is the edit that had the most
  to prove.** `146v126` seq 30 (T13, 19-6, Kaya at 5): the -3 list offered `Chromatic Lantern
  [opponent's battlefield]` and `Overgrown Battlement [opponent's battlefield]` — a 0/4 `[defender]`,
  which is exactly the body the wave-76 guide let the seat exile twice. The seat took the Lantern
  (ladder rung "a noncreature permanent of theirs making cards, damage or life every turn").
  `146v130` seq 29 (T13, Kaya at 5): the ONLY -3 row was `targeting Kaya the Inexorable [your
  battlefield]` and the seat took the +1 — the ladder's `EVERY OTHER BOARD IS THE +1` and the
  `[your battlefield]` line, both as written. `146v130` seq 31 (T15, Kaya now at 6): -3 on
  `Siege-Gang Commander`, the one body the crack-back line prices as a damage source, over three
  1/1 Goblin tokens the ladder excludes by name. Three live tests, three correct answers, no token
  and no defender exiled.
- **Wave-77 EDIT 3 (Tomb of Annihilation at +5 life) — PASSED.** `146v126` seq 24 (T11): the
  three-dungeon list appeared at **20 life vs 7** and the seat answered Tomb; 13 above is well over
  the printed gate. No other Tomb selection window this corpus.
- **Wave-77 EDIT 4 (the 3-life condition (a)) — held; condition (b) FAILED and is Edit 3 below.**
  Three `pay 3 life` windows. `146v125` seq 88 (T27, 20-19) answered `tap`, correct. `146v123`
  seq 17 (T9, 21 life) **paid**, and (b) as written could not refuse it: (b) asks for "the power
  total on their line", a number the render never prints — their board was `Human #1-#26 (1/1) x26`
  plus a 2/2, and the only printed total is the CRACK-BACK line's `for up to 27`. The reasoning
  performs no test at all. Life 21 -> 18 against a 28-damage crack-back.
- **Wave-77 EDIT 1 (the counter test at the cast seam) — UNTESTED.** `146v125` is the only
  counterspell matchup and this game had no window where a Lolth/Kaya row was offered with their
  counter visible and 2+ untapped sources: both boards were creatureless from T13 to T25 and the
  seat's walkers never came up. Not passed, not failed.
- **Wave-77 MED 2 (`hold_check_ref_seq` unjoinable) — CLOSED.** 189 `[hold check:` brackets on this
  seat; every bracketed record carries the field. 18 read **-1**, and all 18 are the "this is the
  first window I have asked you at this seam" face. The other 171 name a real record `seq` in the
  same file, and 169 of them land on a record with the SAME `log_window_kind`; the 2 that do not
  (`146v152` seq 30, `146v126` seq 43) both point at a record my key buckets as `unclassified`, so
  they are my key's coarseness, not a miss. The field is now joinable from a seat log as instructed.
- **Wave-77 MED 3 (loyalty rows priced nowhere) — CLOSED (S14).** All 12 loyalty rows across the six
  games now print `{counter cost: spends 3 loyalty counters - <walker> has N now, leaving M}` (or
  `adds 1 ... going to N+1`), and all 12 N values match the `[counters: Nx loyalty]` figure on the
  same screen. 12/12 TRUE.
- **Wave-77 MED 1 (`crossphase_board_unchanged` stuck at 0) — RECURS for a third wave.** 5
  `crossphase_identical_reputs` on this seat, `crossphase_board_unchanged` **0**, and the string
  `nothing on the board has changed` renders 0 times in 296 prompts. Same shape as last wave; not
  decidable from a seat log.
- **S12 (`Opponent life trend` gained/lost split) — PASSES.** 288 trend lines, 48 carry the split
  (`over those turns life-gain EVENTS put +2 on them and life-loss EVENTS took -1 off - the figure
  before this is the two netted`); the other 240 are spans with no gain event, which is the
  net-only case CY F7 specifies. Two spot-checks against the narration: `146v152` seq 24
  (T11 22 -> now 23, +2 gain from Teferi on T12, -1 loss from dark pool on T13 — correct) and seq 32
  (T14 23 -> now 19, +2 Teferi T15, -6 combat T16 — correct).
- **S8/S13 (`{effect:}` gloss and hoisted shared text) — near-pass.** 71 `(...more)` cuts, 20
  distinct; 18 of the 20 cut at a sentence or clause end. **Two cut mid-clause** — see LOW 4.
  5 prompts carry a `Card text shared by options N-M` header; in all 5 it appears exactly ONCE and
  the shared text appears in no numbered row. S13's hoist is clean on this seat.
- **UNTESTED here, not passed:** S3 (0 stop-reached windows), S4 (0 drain annotations), S6 (0 own-loop
  windows), S5 (0 `already targets this on the stack` clauses), S10 (0 `defer` records), S14's
  `[X pricing]` and `[second copy:` (0 renders each), S2's repeat-row carve-out (0 repeat rows),
  `mana_only_windows_skipped` (0). BLOCK LADDER finally had **6** live blockers windows after three
  waves at zero; CLEANUP DISCARD had **0** for the fifth wave running and stays uncut.

## 1. Game by game — the deciding decisions

**vs 152 (LOSS, T16) — decided at `...vs-ai_baka_deck152.jsonl` seq 26 (T14 Blockers), a 3-blocker
gang block on a TRAMPLER the render priced as fully stopped.** Board 16-23; attackers
`A1. Sigarda, Champion of Light (6/6) deals 6 [trample, flying]`, `A2. Luminarch Aspirant (2/2)`,
`A3. Tovolar's Huntmaster (8/8) deals 8`, `A4/A5. Wolf (2/2)`. Blockers: three Spiders (2/1 reach)
and Barrowin (3/3). A1's row carries
`[GANG BLOCK: any 3 of yours together deal 6, enough to kill this attacker; each B-line result below
is a LONE blocker only]` — and says nothing about trample. Each Spider's own B-row does say it:
`may block A1 (your blocker dies, attacker lives, 5 tramples to your face)`. The seat took
`BLOCKS: B1-B3:A1, B4:A2` and reasoned from the GANG BLOCK clause explicitly:
> *"Block Sigarda (6) with 3 Spiders (2x3=6). Sigarda dies. Spiders die. **0 trample.** ... Life becomes 4. Survives. Best outcome."*

The narration: `Opponent's Sigarda, Champion of Light dealt 3 damage to you (now 1)`. Three
tramples over the three 1-toughness blockers. The model's predicted 4 was 3 too high because the
clause it was told to trust ("Use that number; do not re-derive it") omitted the spill. At 1 life
the T15 attack with Barrowin and the T16 block both became irrelevant; the seat died at T16.
**PERCEPTION -> ENGINE, HIGH 1.** The alternative the header itself priced (Barrowin on the 8/8,
Spiders spread on the 2/2s, Sigarda unblocked) leaves the seat at 10 with a 3/3 and three Spiders —
the line the guide's new one-blocker-per-attacker rule produces (Edit 2).

**vs 123 (LOSS, T10) — not a decision, a combo.** T8 the opponent resolved `Intruder Alarm` and
tapped `Thraben Doomsayer` 25 times for 25 Human tokens (each token entering untaps every creature).
The seat's hand through T9 was Lolth, two Acereraks, Silverquill Command, Agadeem's Awakening — no
instant, and the seat was never given a priority window during T8 (1 priority record in the whole
game, at T9). At seq 15 (T9, 21 life, 4 mana, `CRACK-BACK NEXT TURN: 26 ... for up to 27`) every
castable row was a body or a one-creature edict against 26 creatures; the seat cast Barrowin.
seq 18 correctly declared no attackers; seq 19 took the material block. Unanswerable from that hand.
Two items fell out of it anyway: the 3-life payment at seq 17 (Edit 3) and **MED 2** below, where
the model spent roughly a third of its seq-15 reasoning fighting the CRACK-BACK line with an
invented board state.

**vs 126 (WIN, T15) — won, with a clear missed attack at seq 40 (T15 Attackers, 17 life vs 2).**
The render could not have been plainer:
`ATTACK TOTAL: 2 attackers listed, 4 total combat damage to a player - declaring all of them with
none blocked puts them at -2. That IS lethal - but only if none of them is blocked. At least 1
damage lands whatever they block ... so blocking can leave them as high as 4.` Their only untapped
body was `Pride Guardian (0/3)` and BOTH attacker rows read `(you kill it, your attacker lives)
(blocking trigger: they gain 3)`. The swing risks nothing and cannot be worse than passing. The
seat answered **ATTACK: none**, reasoning:
> *"'blocking trigger: they gain 3' -> swing pays them. Send only what gets through and win with removal, Kaya and the drains."*

— quoting the guide's lifegain bullet almost verbatim, and never reaching `LETHAL OVERRIDES EVERY
STOP ABOVE`, which sat at the BOTTOM of the COMBAT section, eight bullets below the rule it
overrides. **STRATEGY, and an ordering defect in my own guide: Edit 1.** The game was won at T15
anyway (Acererak + the +1 next turn), so this cost tempo, not the game.

**vs 125 (WIN, T34) — the seat's discipline, not its aggression.** From T13 to T25 both boards were
creatureless; `Soul Shatter` and up to three `Silverquill Command` printed `[castable now]` every
window and did nothing, and the seat answered `Cast nothing right now` or `Hold priority` for ~40
consecutive windows without once burning one. It animated the Hive at T17 upkeep only after CHECK 0b
(`of which 0 are creatures` on their line) was true. Correct play throughout; the cost is wall-clock
(LOW 5).

**vs 130 (WIN, T22) and vs 162 (WIN, T10) — clean.** 130: mulligan to 6 and a bottom, then Nadaar
on curve, the legend-rule reset at seq 13-14 (picked the summoning-sick copy, as the guide says),
Kaya +1 at seq 29 and -3 on Siege-Gang Commander at seq 31, Acererak recast for venture three times
at T19-T21. 162: Silencer named `Master of the Feast` off the `{copies not yet in a public zone: N}`
sort, Verse at T7 upkeep, and the Hive animated at T11 upkeep under CHECK 0 with the alpha strike
for exact lethal.

## 2. Engine / interface / card items

### HIGH 1 — a `GANG BLOCK:` clause on a TRAMPLE attacker omits the trample spill, and the prompt forbids re-deriving it
Repro `146v152` seq 26, attacker row A1:
`A1. Sigarda, Champion of Light (6/6) deals 6 [trample, flying] [only 3 of your 4 available blockers
can block this attacker - it has flying, so only blockers with flying or reach can block it - the
may-block lists below say which] [GANG BLOCK: any 3 of yours together deal 6, enough to kill this
attacker; each B-line result below is a LONE blocker only]`
and the boilerplate under it: *"A 'GANG BLOCK:' clause on an attacker line is the cheapest lethal
group YOUR listed blockers can field ... Use that number; do not re-derive it."* The engine prices
trample correctly for a LONE blocker on the very same screen (`may block A1 (your blocker dies,
attacker lives, 5 tramples to your face)`) and drops it for the group. Outcome:
`Opponent's Sigarda, Champion of Light dealt 3 damage to you (now 1)` — 3 tramples through the
group's 3 total toughness while the model had been told the block was free. This is the trust
doctrine's exact failure shape: the model followed the clause over its own arithmetic, as
instructed, and the clause was incomplete. Fix direction: a GANG BLOCK clause on an attacker with
trample states the spill (`... enough to kill this attacker; it has TRAMPLE, so N still goes to your
face over the group's toughness`), computed the same way the lone-blocker parenthetical already is.
Pin: a PARSETEST case with a trampler and a two-blocker group.

### HIGH 2 — the blockers header's `AT BEST` figure is a FALSE floor: the seat beat it by 1 with a legal block
Repro `146v152` seq 21 (T12, 20 life): `INCOMING THIS COMBAT: 2 attackers, 8 unblocked damage - you
would be at 12 ... at least 6 of that lands whatever you block (trample/menace counted as
unblocked): you would be at 14 AT BEST (no assignment of your blockers does better)`. The seat
played `Spider blocks Sigarda, Champion of Light; Goblin blocks Luminarch Aspirant` and the
narration ends `Opponent's Sigarda, Champion of Light dealt 5 damage to you (now 15)` — one life
better than the "no assignment does better" figure. The cause is the parenthetical's own rule:
`trample/menace counted as unblocked` charges the trampler's FULL power (6) to the floor when a
blocker absorbs its toughness first (the same 2/1 the lone-blocker row prices at "5 tramples"). So
the floor is pessimistic by the blockers' toughness on every trample attacker, and it is asserted
as unbeatable. Fix: compute the floor with the same trample arithmetic the per-blocker rows use,
or soften the claim to a bound ("no assignment does better than N" -> "N is a floor this search
cannot prove you beat"). Pin a case.

### HIGH 3 — a lethal blockers header prints "this line LOSES THE GAME and the line above does not" when BOTH lines lose
Repro `146v123` seq 19 (T10, 18 life), one header, verbatim and self-contradictory:
`... this KILLS you ... best case with every blocker assigned: you would be at -6; no block saves
you - no assignment of your blockers survives this; the assignment that lets in the LEAST damage
is: ... ALTERNATIVE, chosen for MATERIAL instead of life: Barrowin of Clan Undurr blocks Thraben
Doomsayer #1 ... 26 KILLS you at 18 life: this line LOSES THE GAME and the line above does not.`
At 18 life the "line above" lets in 24 and ends at -6, which the same sentence has already stated.
The comparison clause is emitted unconditionally and is FALSE whenever the header has already
declared no assignment survives. The model caught it and burned reasoning on it
(*"Wait, if I take 24 damage, I go to -6. I lose."*). Fix: suppress the LOSES-THE-GAME comparison
when the best-case figure is itself at or below 0, or restate it as a damage comparison.

### MED 1 — the `AT BEST` header names no assignment, while every other header face does
Six blockers windows on this seat, four header faces. `146v123` seq 19 and `146v152` seq 32 (lethal)
both name `the assignment that lets in the LEAST damage is: ...`; `146v152` seq 10 and `146v130`
seq 41 (survivable) both name `one legal assignment that reaches it ...`. The two `AT BEST` windows
(`146v152` seqs 21, 26) give a life number and **no assignment at all** — and they are the only two
windows where a trample or menace attacker is present, i.e. exactly the hardest assignment to
derive by hand. Both were the seat's worst blocks. Fix: name the assignment that reaches the AT BEST
figure, with the same wording the other faces use.

### MED 2 — an untap-restriction keyword reads as a tap STATE, and the crack-back's "(tapped ones untap first)" fights it
Repro `146v123` seq 15 (T9). Their 25 Human tokens each print
`Human #1-#25 (1/1) [doesn't untap during its controller's untap step] x25` with no tap tag (they
were untapped), while the header reads
`CRACK-BACK NEXT TURN: 26 of their creatures will be able to attack (tapped ones untap first), for
up to 27`. The model built a complete false board from the keyword alone and argued with the header
for ~1,300 characters:
> *"The opponent has 25 **tapped** Human tokens ... Next turn, if I do nothing, they can't untap (Intruder Alarm). They stay tapped. ... But the prompt explicitly says 'CRACK-BACK NEXT TURN: 26 of their creatures will be able to attack'. This implies they WILL untap. How?"*

Two render properties combine into this: (a) tap state is signalled only by the PRESENCE of
`[tapped]`, so on a 25-wide collapsed row the reader has to infer untapped-ness from an absence,
and the restriction keyword sitting in the same bracket group reads like the state; (b) the
parenthetical `(tapped ones untap first)` is a blanket claim that is false for exactly the
creatures carrying that keyword. Fix direction: print the affirmative state on a creature row that
carries an untap-restriction keyword, and scope the crack-back parenthetical (or drop it when no
attacker in the count is currently tapped). Cheap, and it removes a whole class of confabulation on
wide boards. No decision changed here — the crack-back figure was TRUE — but the seat paid a third
of one decision's reasoning for it.

### LOW 3 — `[castable now]` on a spell the engine has already marked as doing nothing
Repro `146v125` seqs 29-66: `Soul Shatter {2}{b} [instant] [castable now]` in the hand header for
twelve consecutive turns while `Opponent battlefield (N permanents listed, of which 0 are
creatures ...)`. The CAST ROW does carry `{right now: they control 0 creatures ... at 0 this does
nothing}` when the menu offers it, so the truth exists; the HAND HEADER's bracket says only that
the mana is payable. The seat read it right every time. A `[castable now, but dead: ...]` form on
the hand line would save the model from re-deriving deadness at each of ~40 windows.

### LOW 4 — two `(...more)` gloss cuts land mid-clause
18 of 20 distinct cuts end a sentence or a parenthetical. The two that do not:
`146v125` seq 38 — `... Hive of the Eye Tyrant becomes a 3/3 black Beholder creature with menace and
(...more)` (cut after a conjunction), and `146v152` seq 18 — `... -- -8: You get an emblem with
(...more)` (cut after a preposition). Neither produces a FALSE statement, but both stop mid-phrase,
which is what CY F6's clause-boundary rule was meant to prevent; the quote/paren atomicity half is
holding.

### LOW 5 — the seat's wall-clock is one static board
`146v125` is 134 records to 34 turns; the other five average 36 records. T13-T25 alone is ~40
windows on a board where neither side controlled a permanent that could attack and the seat's only
castable cards were dead. At p50 86 s that is ~1 h of decode for a board that could not change.
`hold_windows_skipped` 131 for that game shows the engine is already collapsing most of it; the
residue is the land drops and the Hive animation re-opening the seam. Same item as wave 77.

### LOW 6 — `146v130` `forced_close_unrecorded` 1 with every arm-bound counter at 0
The two records: seq 8 `{arm: cast, event: 1, outcome: armed, window_seq: 6}` and seq 9
`{arm: cast, event: 2, outcome: dropped_decision_moved, unrecorded_so_far: 1, window_seq: 6}` —
both on the T5 land-drop window, `force_close_arms_refused/_deferred/_same_arm_deferred/
_defer_bound_hits` all 0. Reconciles locally (2 = 1 armed + 1 dropped). The corpus-wide 32/16 split
is the engine seat's call.

## 3. Guide verdict: EDIT

Live 19,998 B -> revised **19,997 B** (`wave78/deck146/strategy.txt`). Three rules changed, and the
guide did NOT grow — every added byte is paid by the compression listed under Edit 4, so no rule
needed to earn bytes from another.

**Edit 1 — LETHAL becomes the FIRST check in COMBAT instead of the last, and the lifegain bullet
names the precedence at its own site (A333/A334).**
- before: the LETHAL rule was the penultimate COMBAT bullet — `- LETHAL OVERRIDES EVERY STOP ABOVE:
  their printed life at or below the total power of what you send -> send it all.` — eight bullets
  BELOW the lifegain rule it overrides, and the lifegain bullet ended flatly at `No such clause ->
  their walls are free to attack.`
- after: the bullet is deleted from the bottom and the section opens with
  `- LETHAL IS THE FIRST CHECK, ABOVE EVERY RULE BELOW: the ATTACK TOTAL line reads "That IS
  lethal", or their printed life is at or below the power you can send -> SEND IT ALL. A blocking
  trigger that gains them life, a wall, a converter line and a carried plan are all under it.`
  The lifegain bullet now ends `... their walls are free to attack. LETHAL outranks all of it.`
  (a flat precedence statement, not a concessive clause).
- paid for by **`146v126` seq 40** — opponent on 2, `ATTACK TOTAL ... That IS lethal`, both
  attackers tagged `(you kill it, your attacker lives)`, and the reasoning quotes the lifegain
  bullet as the reason for `ATTACK: none`. A heading that declares its scope at the top is what the
  model reads first; a precedence rule placed after the rule it governs is not read at all.

**Edit 2 — the BLOCK LADDER gets ONE BLOCKER PER ATTACKER stated flat, and a header-figure rule;
rung 3 loses the concessive tail that carried it (A334).**
- before, inside rung 3: `"...N tramples to your face" does not stop that N - chump a trampler only
  at lethal, never two blockers on one.` The multi-block prohibition was a trailing clause of a rung
  the seat only reaches for chump blocks, and the header has no instruction of its own.
- after, as its own rule above the rungs:
  `ONE BLOCKER PER ATTACKER: a second body on one attacker is one more attacker unblocked; a "GANG
  BLOCK:" clause prices the KILL, never the life that block costs you.`
  `The header NAMES an assignment -> take it; a life number alone ("at N AT BEST") -> one blocker on
  each attacker, biggest "deals N" first, and stop.`
  Rung 3 keeps `chump a trampler only at lethal` and drops `never two blockers on one`.
- paid for by **`146v152` seq 26** (three Spiders on one 6/6 trampler; 16 -> 1 where the header's own
  figure was 10) and **`146v152` seq 21** (the AT BEST face with no named assignment). Every term is
  printed on the same screen: `deals N` on each A-row, the header's `at N AT BEST`, the named
  assignment when there is one. The rule does not depend on the broken trample value — it routes
  around it, which is why HIGH 1 still needs the engine fix and the guide says nothing about it.

**Edit 3 — the 3-life menu's condition (b) is restated in a number the render prints.**
- before: `(b) "{this payment puts you at N}" is above the power total on their line.`
- after: `(b) the CRACK-BACK line's damage number is below the N in "{this payment puts you at N}",
  or no CRACK-BACK line is printed.`
- paid for by **`146v123` seq 17**: their line was 26 one-power tokens plus a 2/2 and the only
  printed total anywhere on that screen was `CRACK-BACK NEXT TURN: ... for up to 27`. Under the new
  wording 27 is not below 18 and the answer is `tap`. Checked against the other two windows:
  `146v125` seq 88 (no crack-back line printed, condition (a) failed, seat tapped — unchanged).

**Edit 4 — the bytes.** No rule and no condition deleted. Compressed: the cast-order entry-2 tail,
Command's live/dead and TIMING bullets and its TARGET line, the Silencer sort sentence, Pelakka's
creature-count sentence, the land-drop sentence, Lolth's Spiders sentence, the second-copy bullet,
the hold paragraph's opening and its `CHOICE: 0` sentence, the mulligan header, two CLEANUP DISCARD
lines, Verse's target sentence, Soul Shatter's row sentence, Hive's CHECK 0 and AFFORDABILITY, the
Acererak bullet, the Emeria land-row example, the combat-math bullet, the dungeon line, and the
deck header's parenthetical colour gloss (the colours are on every cost in the guide). One
condition was REMOVED as redundant rather than compressed: COMBAT CHECK 0's tail `Otherwise send
only when the power you send is at or above their printed life THIS turn; else ATTACK: none` became
`Otherwise ATTACK: none`, because Edit 1 now states that lethal case above it and stating it twice
was the wave's own bug in miniature. `WHEN THEY COUNTER` lost the sentence `and she is the card this
deck cannot afford to lose to a counter`, which is rationale for a test that lives in cast-order
entry 2.

## 4. What I did NOT check
- **`146v125`'s T13-T25 tail decision by decision** (~40 windows). I read the hand and both
  battlefield lines at seqs 29/36/45/55/61/66/70 and established that both boards were creatureless
  and the seat's castable cards were dead; I did not open each window.
- **Whether a different line beats deck123.** I established the seat had no instant and no priority
  window during the T8 combo turn. I did not search for a T5-T7 line that pre-empts Intruder Alarm.
- **Whether the seq-26 alternative wins the 152 game.** I checked only that the header's own figure
  was 10 and the seat reached 1; I did not simulate the rest.
- **Opponent seats** beyond board and log context; no audit of any opponent deck's play.
- **`hold_windows_skipped` 138, `own_turn_windows_skipped` 89, `main_phase_windows_skipped` 27,
  `ask_replays_reserved` 14** — the skipped windows are not in the file, so they are not confirmable
  from a seat log.
- **`forced_close_unrecorded` 1** — reconciles locally; the corpus split is the engine seat's.
- **My hold-check CLAIM audit is weak and I am reporting it as such.** I diffed each bracketed
  window's numbered rows against the record named by `hold_check_ref_seq`, stripping `{...}`/`[...]`
  groups and folding the pass row's "(<step> comes next this turn)" parenthetical: **153 of 171
  numeric claims agree with my diff**, and 15 of the 18 disagreements are off by exactly one in BOTH
  the new and the gone count — the signature of a folded row my normalizer does not fold the way the
  engine does. I am NOT claiming any hold-check statement is false this wave. The 18 first-window
  sentences and the 169 same-seam joins were checked directly and do not depend on that key.
- **A hold-check verdict face the guide has no instruction for, reported not edited:** `N row(s)
  that were on the menu at the last window I asked you at this seam is/are gone and no row above is
  new` renders **38** times on this seat, and the guide's hold paragraph instructs only on the
  all-same face and the rows-are-new face. No decision went wrong on one of the 38, so under this
  wave's REJECT-by-default rule I did not buy it bytes; if a later corpus shows a seat declining a
  strictly-shrinking list it is a one-clause edit.
- **S3, S4, S6, S5, S10, S2, `[X pricing]`, `[second copy:`, ANNOUNCE_X, `mana_only_windows_skipped`,
  CLEANUP DISCARD** — 0 renders/records each on this seat. UNTESTED, not passed.
- I read no engine source. Every claim above is from the corpus bytes.

No `general-proposals.md` and no `skill-proposals.md`: Edit 1 is A333 and A334 applied to a section
ordering, Edit 2 is A334 applied to a rung tail, Edit 3 is the installed skill's printed-number
discipline. Nothing here needs a new amendment.
