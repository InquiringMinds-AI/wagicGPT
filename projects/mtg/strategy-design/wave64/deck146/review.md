# Wave-64 deck 146 (Orzhov dungeon midrange) — per-deck review

Corpus: `matchups-20260906-001533`. Seat files `*-ai_baka_deck146-0x*-vs-ai_baka_deck<opp>.jsonl`.
Record 3-3. 6 games, 311 seat decisions, **0 fallbacks**, 0 hangs, 0 crashes, 0 wall misses,
`transport` clean, max `deadline_pct` observed 9.6. Seq numbers below are the deck-146 SEAT's.

## 1. Game by game

| opp | result | turn | end life | deciding decisions |
|---|---|---|---|---|
| 152 | LOSS | 12 | -5 / 32 | s13 (t9, 14 life) cast Silencer over Barrowin as the blocker vs Sigarda 4/4 flier; s21 (t11) attacked with the 1/1 first-strike deathtoucher into a stated 15-damage crack-back, then at s23 the survival header already read `no block saves you` — the game was lost to a faster curve, not to that attack. |
| 123 | WIN | 11 | 22 / -1 | Clean: s12 Vanishing Verse on Thraben Doomsayer, s18/s20 Pelakka Predation stripped Idyllic Tutor, s26 lethal 3-attacker swing. Nothing to fix. |
| 126 | LOSS | 14 | 0 / 30 | **s29 (t13, 21 vs 9, opponent creatureless, 7 power on board): declined a completely free attack** because every attacker row carried a false loop tag (item HIGH-1). Game was already lost on board — their Sanguine Bond + Exquisite Blood pair turned t14 Tribute to Hunger (s31, forced sacrifice, they gain toughness) into an unbounded drain, 21 -> 0. Vanishing Verse (the deck's only enchantment answer) was never drawn in 7 draws; Soul Shatter and Command cannot touch an enchantment. Variance + a hard matchup, plus one free swing thrown away. |
| 130 | LOSS | 40 | 0 / 4 | Grind, lost by 4 life. **s93 (t39, 1 life): the model answered `CHOICE: 2 (Cast nothing)` and the ENGINE TOOK ROW 1 (Cast Acererak)** — `answer_replaced:true`, latched a second `CHOICE:` line at byte 6544 (item HIGH-2). Self-inflicted life: s57 (5 life) and s74 (3 life) both took Silverquill Command's `you draw and sacrifice creature`, whose hidden `life:-1 controller` cost 1 life each — 2 of the 4 life it lost by (item HIGH-3). |
| 125 | WIN | 21 | 20 / -3 | s41 Verse on Lightmine Field, s25/s26 Lolth -3 for two Spiders, then a wide Spider board. One wart: s32/s33 planned pump+return, then chose `return creature and sacrifice creature` whose sacrifice half was DEAD (opponent had 0 creatures) — the modal ask had dropped the cast row's `dead (no legal object right now)` annotation (item HIGH-3). |
| 162 | WIN | 17 | 13 / -2 | s34 Soul Shatter on Fate Unraveler, s40 six-attacker lethal. Note s39 was also an `answer_replaced` flip (model wrote `CHOICE: 0 (pass)`, engine took row 1 `becomes beholder`) — the flip happened to enable the lethal swing (item HIGH-2 cuts both ways). |

## 2. Engine / interface / card items

### HIGH-1 — the life-LOOP tag is printed unconditionally on attackers that cannot possibly trigger it
File `...deck146-...-vs-ai_baka_deck126.jsonl` **seq 29** (t13, 21 vs 9). Opponent battlefield:
`(7 permanents listed, of which 0 are creatures ...)`. Rendered rows:

```
A1. Barrowin of Clan Undurr (3/3) [no creature they control can block this attacker] (their life LOOP is in play: any life they gain or you lose in this combat chains without limit - fatal to you, not a trade)
A2. Goblin (1/1) [no creature they control can block this attacker] (their life LOOP is in play: ...)
A3. Silverquill Silencer (3/2) [no creature they control can block this attacker] (their life LOOP is in play: ...)
They have 0 untapped creatures able to block: every attacker you declare this turn is unblocked unless something changes first.
ATTACK TOTAL: 3 attackers listed, 7 total combat damage to a player ...
```

3 of 3 attacker rows tagged; all 3 unblockable; opponent has no creatures, no lifelink and no
blocking trigger, so nothing in this combat can gain them life or cost the seat life. The seat
answered `ATTACK: none` and its PLAN quotes the tag back verbatim ("Attacking would cause the
opponent to lose life (triggering Exquisite Blood), which causes them to gain life ..."). That
reading is also wrong on the primitives — `Exquisite Blood` is `auto=@lifelostfoeof(player):life:thatmuch controller`
(mtg.txt:37967) and `Sanguine Bond` is `auto=@lifeof(player) from(*[-lifefaker]|*):life:-thatmuch opponent`
(mtg.txt:99004): the loop starts when THEY gain or the SEAT loses, never when they lose. The
tag's own wording says exactly that; the defect is that it is attached to rows where neither
branch is reachable. Repro N=1 of 1 window in this deck's corpus, and it cost the only free EV
available in a lost game. Fix: emit the per-attacker loop clause only when that attacker's own
listed outcomes contain a life gain for them or a life loss for the seat; otherwise print the
negative fact ("no listed outcome of this attack gains them life or costs you life").

### HIGH-2 — a second column-0 `CHOICE:` line thousands of bytes past the plan block silently replaces the answer
`...vs-ai_baka_deck130.jsonl` **seq 93**, t39, 1 life vs 6. Record fields:
`answer_replaced:true`, `choice:1`, `latched_line:"CHOICE: 1 (Cast Acererak the Archlich)"`,
`latched_line_offset:6544`, `latched_line_in_plan:false`, `plan_block_end:735`,
`latched_coded_line:2`, `post_answer_overrun:6678`, `reply_trimmed_bytes:5512`.
`reply` line 1 is `CHOICE: 2 (Cast nothing right now)`. The engine executed row 1.
The F13 byte-offset record (lane AJ) works and correctly reports `in_plan:false` — but the
answer is still taken from a coded line 5.8 KB past the plan paragraph, inside the model's
visible self-argument ("Wait, I need to check if I can kill them this turn ...").
Corpus for this deck: **3 of 3 `answer_replaced` records changed the executed row** (130 s93, 130 s101, 162 s39).
Two of the three were on the final turn of games decided
by 4 life or less. s101 and 162 s39 the flip helped; s93 it hurt. Fix candidate: adopt a
coded line past `plan_block_end` only when it is the ONLY coded line, or gate adoption on the
overrun being below some byte budget; either way an executed row that contradicts line 1 should
be reported at the seam, not merely recorded.

### HIGH-3 — F6 is NOT fixed on the `Choose an option for <card>` ask: modal rows are still bare names
Lane AG claims "F6 modal rows priced like cast rows". The follow-up modal ask still renders
nothing but the mode label. `...vs-ai_baka_deck130.jsonl` **seq 74** (t35, `Your life: 3`):

```
Choose an option for Silverquill Command:
1. return creature and you draw
2. return creature and opponent draws
3. return creature and sacrifice creature
4. you draw and sacrifice creature
5. opponent draws and sacrifice creature
```
Answer: `CHOICE: 4`. Life went 3 -> 2. `borderline.txt:102747` is
`auto=choice name(You draw and sacrifice creature) draw:1 controller && life:-1 controller && ...`
— the row never says it costs 1 life. Same at **seq 57** (5 life -> 4). The seat lost this game
at 0 vs 4. Same class at `...vs-ai_baka_deck125.jsonl` **seq 33**: chose
`return creature and sacrifice creature` while the opponent's board was
`(6 permanents listed, of which 0 are creatures ...)` — the sacrifice half was dead and the row
did not say so, although the CAST row one window earlier (seq 32) carried
`{modes live right now: ...; dead (no legal object right now): ...}`. Fix: carry the cast row's
mode annotations (live/dead, life delta, draw grant) onto the modal follow-up rows.

### MED-1 — the "best case with every blocker assigned" life figure ignores the same block's lifelink gift
`...vs-ai_baka_deck152.jsonl` **seq 12** (t8, 20 life, NOT lethal):
`best case with every blocker assigned: you would be at 14 - one legal assignment that reaches it, chosen for the LIFE ONLY - it does not preserve your material ...: Silverquill Silencer blocks Intrepid Adversary #1; 1 of those blockers die - taking it SPENDS Silverquill Silencer.`
The truthful-ranking half of the F8 fix works. But the B1 line for the same block reads
`{their attacker's lifelink, this block: they gain 4 from this block only}`, so the block is
+4 for the seat and +4 for them — a zero-swing trade that spends the seat's only creature.
The seat copied it (`BLOCKS: B1:A1`) and lost its Silencer at 20 life against an aggro board.
Suggest netting opponent lifelink/blocking-trigger gain into the "best case" figure, or printing
the race delta rather than only the seat's own life.

### MED-2 — Kaya the Inexorable's `-3` still offers Kaya as her own target
`...vs-ai_baka_deck130.jsonl` seqs **65, 77, 82, 99** — 4 of 4 windows carry
`N. -3: exile non-land permanent with Kaya the Inexorable targeting Kaya the Inexorable`.
Wave-63 MED-5 (row-count bloat) is largely fixed (12 rows -> 2-3), but the self-exile row is
never legal-to-want. LOW cost, trivially collapsible.

### MED-3 — `Choose a card NAME` still ranks TOKEN names first by public-zone visibility
`...vs-ai_baka_deck130.jsonl` **seq 27** (t17): row 1 is
`Goblin {visible now: 2 on their battlefield}`. "Goblin" is a token; the opponent can never CAST
a spell named Goblin, so the row can never fire Silverquill Silencer's
`Whenever an opponent casts a spell with the chosen name` trigger — and the visibility sort puts
it at the top of the list. Wave-63 deck146 MED-3, unchanged. Related and larger: the annotation
is public-zone visibility, but the bet is on a card they have NOT cast yet; a row for a
permanent already resolved on their battlefield is a much worse bet than an unmarked row, and
nothing on the surface says so. Suggest suppressing token names and annotating remaining copies
(library + hand) rather than visible ones.

### LOW — `; converters` literal still rendered
13 windows in this deck's corpus, e.g. `...vs-ai_baka_deck152.jsonl` seq 13:
`they draw 1 card now; converters on your battlefield: 0 (nothing of yours punishes their draws ...)`.
Wave-63 E13's rename was declared out of scope for lane AC; recording only that it recurs.

### Confirmed working (no action)
- **F2 PASS**: 69 windows print `this ask has no pass row`; 0 of them also render a
  `Cast nothing` or `Hold priority` row.
- **F7 PASS**: `...vs-ai_baka_deck152.jsonl` seq 21 —
  `GANG BLOCK: any 2 of them together deal 2 past everything its own first strike can kill, enough to kill this attacker`
  on a 1/1 first-strike deathtoucher, with the LONE-blocker results printed alongside. The
  wave-63 HIGH-2 suppression is gone.
- **F9 PASS**: `...vs-ai_baka_deck152.jsonl` seq 23 leads with survival —
  `you would be at 0 AT BEST (no assignment of your blockers does better); no block saves you`.
- **F11 PASS**: `...vs-ai_baka_deck152.jsonl` seq 21 renders the full
  `CRACK-BACK COST OF ATTACKING:` paragraph with `Of the 1 creature offered above, 0 have vigilance`.
- **F8 labelling PASS**: both suggestion renders (152 s12, 130 s31) name their ranking basis
  truthfully and say what they spend; see MED-1 for the residual arithmetic gap.

## 3. Guide verdict: **KEEP** as is

`bin/Res/ai/baka/deck146_strategy.txt`, 70,986 bytes (top of the 41-71 KB band), unchanged.

Reasons this is a success verdict:
- The seat went 3-3, took 0 fallbacks in 311 decisions, and every WIN was executed on the guide's
  own plan: name a card the opponent will recast (123 s8/s17, 162), venture on curve, spend
  removal on curve (123 s12, 162 s34, 125 s41), close with a wide swing.
- Each of the three losses traces to an engine surface, not to a rule the guide is missing:
  126 to HIGH-1's false loop tag, 130 to HIGH-2's answer replacement and HIGH-3's unpriced modal
  rows, 152 to a genuinely faster aggro draw against which the seat's own removal never arrived.
- The one place the model played AGAINST the guide is already covered by text on the page:
  lines 299-302 say the self-aimed draw mode is "a card for 1 life - a good deal at healthy life,
  a bad one at 5 or less", and 130 s57 (5 life) and s74 (3 life) both broke it. The reason it
  broke is that the modal ROW shows only `you draw and sacrifice creature` (HIGH-3) — restating
  a rule the guide already states in stronger language buys nothing; pricing the row does.
- Line 532's "Take the room that ... DAMAGES the opponent (Dark Pool)" was examined against the
  126 loss and **exonerated**: Dark Pool (opponent loses 1, seat gains 1) does not start their
  Sanguine Bond/Exquisite Blood loop — the log at t13 shows `Opponent lost 1 life (now 9)` with
  no chain. The loop fired on t14's forced sacrifice, which no room choice touched.
- The file is at the band ceiling; no addition here is paid for by a decision that went wrong.

No `general-proposals.md` and no `skill-proposals.md`: every item above is an engine/render fix,
and nothing in this deck's corpus shows a general-guide or skill rule failing.

## 4. What I did NOT check
- The opponents' seat translogs beyond the deck-146 board state visible in its own prompts; I did
  not trace any opponent's decisions.
- The other six decks' corpora, and therefore whether HIGH-1/HIGH-2/HIGH-3 recur outside deck 146
  (cross-deck counts are the engine seat's job).
- Scryfall cross-checks: I verified Silverquill Command, Sanguine Bond, Exquisite Blood and Lost
  Mine of Phandelver's Dark Pool against the primitives only (`borderline.txt:102739-102749`,
  `mtg.txt:99004`, `mtg.txt:37966`, `borderline.txt:67545-67549`); I did not query the Scryfall API.
- I did not read `AIPlayerGPT.cpp` to locate the emitters for HIGH-1 or HIGH-3; the repro is
  stated from the rendered prompt only.
- HOLD take census: I did not prefix-match holds against rows; `hold_windows_skipped` was 60 in
  the 130 game and I report only that number, not its correctness.
- Mulligan quality: all six openers were kept at 7 and every hand was playable, so no mulligan
  branch of the guide was exercised.
