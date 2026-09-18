# Wave 81 — deck123 (Intruders of Thraben) seat review

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260917-191141-final`, binary `wagic-6e0c51238-w81step1`.
Record: **1 win / 6** (wave 80: 1/6). Seat protocol was clean in all six games: 300 windows,
0 fallbacks, 0 `plan_line_missing`, 0 `protocol_deviation`, 0 `reask_reason`, 12 `async_drops`
(every one logged `[outcome: re-asked]` after "the question and board moved" — each re-ask correct).

Seat logs are `<epoch>-ai_baka_deck123-*-vs-ai_baka_deck<M>.jsonl`; seq numbers below are that
file's `seq`.

---

## 1. Game by game, and the decisions that decided them

### vs deck125 — LOSS, 0 vs 34, turn 29 (file `1789690308-…-vs-ai_baka_deck125`)
Blue-white control with Staff of Nin ×2, Sphinx's Revelation, Path to Exile, Elixir. Deck123 got
three makers onto the battlefield and lost all three to spot removal: Bloodline Keeper T8 (Path to
Exile), Bloodline Keeper T16 (Path to Exile), Thraben Doomsayer T18→T19 (Path to Exile). **No Create
row was offered in this game at all** — a search of every window's `options_text` finds zero rows
containing "Create" or "Transform". Every maker died before it lost summoning sickness.
- seq 56/57 (T16, Main 1, 8 untapped sources): cast Bloodline Keeper, then Intruder Alarm, tapping
  down to 1. The Keeper was Pathed in response inside the same main phase. This is the one window
  where the guide's cast-gate (bait a dead row first) would have mattered — but the gate is written
  for a *counterspell* on the graveyard line, and Path to Exile is not one, so the guide did not
  fire. Not a guide defect; noting it as the only place where a cheap first cast was available and
  the seat was tapping out into an opponent with 5 untapped sources.
- Turns 20–29 the seat sat at 0 creatures with Damnation ×2, Devour Flesh and a dead Tutor in hand
  and correctly answered "Cast nothing right now" ~25 times while Staff of Nin ×2 pinged it out.
  Deck123 has no artifact answer; this is a deck-vs-deck loss, not a decision loss.

### vs deck50 — **WIN**, 20 vs −24, turn 16 (file `1789690310-…-vs-ai_baka_deck50`)
The guide's chain, executed exactly as written. Doomsayer T8, Keeper T10, Greaves on the Keeper,
Intruder Alarm T14 (seq 60), then the repeat row **on the opponent's turn** (seq 75,
`Create vampire with Bloodline Keeper, repeated then stop`) built the army, and seq 133 (T16) sent
all 20 for −24. Three earlier "no attackers" answers on a lone hasted Keeper (seq 55, 92) are CHECK
−1 applied correctly. This game is the guide's positive control.

### vs deck162 — LOSS, 0 vs 16, turn 14 (file `1789690333-…-vs-ai_baka_deck162`)
Opponent board: Underworld Dreams ×2 + Fate Unraveler + Howling Mine + Dictate of Kruphix — three
draw punishers and two forced-draw engines. The seat was drawn to death.
- **seq 101 (T14, Draw step) — the decisive misplay, and a guide violation twice over.** The header
  read `ON THE STACK: … = 12 in total - you would be at -2; that would KILL you`, and row 1 read
  `[DRAW PRICE: this draws 2 cards, and the opponent's Underworld Dreams #1, Underworld Dreams #2,
  Fate Unraveler punish every draw, so taking it costs you 6 life right now - you would be at 4]`.
  The seat cast Vision Skeins with the PLAN "casting Vision Skeins to stay alive at 4 life".
  The guide says "Vision Skeins … Never against a card that draws them extra or punishes drawing"
  AND "ANY ROW PRINTING '[DRAW PRICE: … you would be at X]' HAS ANSWERED ITSELF … Decline at X of 5
  or less." X was 4. Both rules present, both correctly written, both ignored. (The game was already
  lost on that stack; the misplay is the reading, not the outcome.)
- seq 52 (T10 attackers): sent Thraben Doomsayer — the only maker — alongside a Human, both
  unblockable. CHECK 3's "KEEP ONE UNTAPPED MAKER HOME, every attack" says the maker stays. LOW: the
  Alarm was not out, so the Doomsayer untapped normally.
- Cleanup discards seq 41 / 54 / 83 were all correct picks (a duplicate Keeper, a second Damnation,
  a duplicate Doomsayer) — made from a menu that prints **no** copy counts or verdicts (see §2, MED-2).

### vs deck130 — LOSS, 0 vs 11, turn 22 (file `1789690335-…-vs-ai_baka_deck130`) — the game this seat should have won
This is the game to read. By T19 the seat had Intruder Alarm, Lord of Lineage and the loop running,
and built to **26 creatures**. It lost anyway, through two decisions:
- **seq 62–67 (T19, the seat's OWN upkeep): built the whole army on its own turn with a sweeper in
  plain sight.** Their graveyard on that very prompt reads `Starstorm {r}{r}{x}`. The guide's rule
  is verbatim: "SWEEPER SEEN (Damnation, Supreme Verdict, Final Judgment, Wrath, **Starstorm**, Fall
  of the Gavel - in their log, graveyard or hand): an army built in YOUR OWN main phase sits
  summoning sick through their whole turn and is swept before it attacks. Fire at THEIR END STEP."
  On T20 (seq 75's prompt) the opponent cast Starstorm and killed 25 Vampires.
- **seq 78 (T20 Draw, opponent's turn) — the decision that lost the game.** Board: 3 creatures,
  a live `Create vampire with Lord of Lineage` row, stop = L(20)+C(0)+3 = 23, M = 3. The seat took
  the **Hold priority** row, whose PLAN reads *"Hold priority to block Rorix with Vampire #1 and
  Vampire #2 in combat."* The guide forbids exactly this: "THE HOLD ROW IS A P8 ANSWER AND NOTHING
  ELSE: take it only when EVERY row on THIS window is dead … One live row — a maker … a Create row
  below the stop — and the hold is wrong." The hold stood for the rest of the opponent's turn, so
  the rebuild happened on the seat's **own** T21 (seq 80/81, x20) where the tokens were summoning
  sick: at seq 93 it could send only 2 of 23 creatures, dealt 9 (opp 20→11) and died on T22 to burn.
  Had the T20 run been taken at the opponent's end step, 22 six-power flyers attack on T21 for a
  kill several times over.
- seq 105 (T21 Main 2) is the same misuse a third time: "Hold priority" chosen with the PLAN
  "this window pass, then hold until untap". See §2, HIGH-1 — the row's short name is being read in
  its Magic sense.
- No blockers window fired on T22; correctly so — their battlefield at seq 107 lists 0 creatures.
  The seat died to burn at 3 life with 24 untapped flyers. Not an engine miss.

### vs deck146 — LOSS, 0 vs 21, turn 17 (file `1789690340-…-vs-ai_baka_deck146`)
Fast white-black aggro with Silverquill Silencer, Vanishing Verse, Barrowin, Lolth. Only 23 windows.
Every maker answered on sight; the seat was at 5 life by T12.
- seq 22 (T14 Main 1, life **2**): Damnation row read `destroys 3 of their creatures (3 without a
  restriction against attacking), 2 of yours - … YOURS: Bloodline Keeper (3/3) [flying], Vampire
  (2/2)`. The seat cast it. **The guide told it not to**: "A TOKEN-MAKER OF YOURS IN THE 'M of
  yours' LIST IS A VETO OF ITS OWN - Doomsayer, Keeper or Lord of Lineage there and the number is
  not written, whatever N and K say." Obeying the veto loses on the spot to three attackers at 2
  life. The seat overrode the guide and bought a turn. Recorded as evidence, not as an edit (§4).
- seq 14/15 (T10): cast Bloodline Keeper (leaves 0 of 4) and took the land row in the next window —
  land second, against the guide's "1. Land, first, every turn". Cost nothing here (the land was
  Marsh Flats, which makes no mana until cracked).

### vs deck152 — LOSS, −1 vs 21, turn 13 (file `1789690346-…-vs-ai_baka_deck152`)
Werewolf/human aggro. Brutal Cathar exiled the Keeper, Elite Spellbinder exiled the Intruder Alarm
out of hand. The seat tutored a second Alarm at seq 32 (T12) but at 1 untapped source could not cast
it, and died at T13 to a 17-power attack it could block only one of. Nothing here turns on a
decision the guide governs; seq 34/37 is a P9 wobble (see §3).

---

## 2. Engine / interface / card items

### HIGH-1 — the "Hold priority" row's short name is read in its Magic sense, and it decided a game
**Repro:** `1789690335-ai_baka_deck123-…-vs-ai_baka_deck130.jsonl` seq 78 (T20, Draw, opponent's
turn). Rendered row: `Hold priority - pass now, and do not ask me again - YOU CANNOT COME BACK AND
TAKE ONE OF THE ROWS ABOVE LATER THIS TURN - it stands until your next turn begins…`. The seat's
reply: `PLAN: Hold priority to block Rorix with Vampire #1 and Vampire #2 in combat. | CHOICE: 3
(Hold priority)`.

The seat took a hold row **25 times** across the six games. In substantially all of them the PLAN
written on the same line states an intention to act *later this turn* — i.e. the exact thing the row
forfeits. A sample, all verbatim:
- deck125 seq 108 "Hold priority to cover the Staff of Nin triggers." · seq 121 "Hold priority until
  main phase."
- deck162 seq 33 "Hold priority now. In Main 1, play Underground Sea, cast Intruder Alarm." ·
  seq 62 "hold priority through draw step, play land in main phase, cast Intruder Alarm, cast
  Bloodline Keeper, tap Keeper to make token…"
- deck50 seq 78 "Hold priority, then attack with all 19 Vampires." · seq 19 "Pass priority. Next
  turn: untap, draw, play land, cast Thraben Doomsayer."
- deck146 seq 19 "Hold priority to preserve mana for Idyllic Tutor in Main 1."
- deck130 seq 78 (above) · seq 105 "this window pass, then hold until untap."

In Magic, "hold priority" means *retain priority and keep acting*. The row does the opposite: it
closes the seat's remaining windows for the turn. The body text says so at length; the model's
attention lands on the two-word name, and every one of these PLANs contradicts the body it just
read. The row survived here only because a row-set change re-opened the window most times
(`hold_windows_skipped` 16 / 54 / 5 / 11 / 3 / 19 across the six games); on deck130 T20 nothing
re-opened it and the loop never ran. Recommendation: the row's SHORT NAME should say what it does
(it is a *stop asking me this turn* row, not a priority-retention row); the body text is already
correct and does not need more words.

### HIGH-2 — P9 shipped the folded land row but not the header text that describes it
**Repro:** `1789690308-…-vs-ai_baka_deck125.jsonl` seq 10 (T8). Menu row 3 is a live
`Play Underground Sea`, while the header above it reads:
`Land drop: a land play IS available to you right now. The land drop is its OWN decision (a "Land
drop:" question with its own Play options), so the absence of a land from the choices below does not
mean the drop is gone.`
**27 windows** across the six deck123 games carry a folded `Play <land>` row on the casting menu
*and* this header sentence telling the seat the drop is a separate question. Both descriptions
cannot be true of the same menu. The correct new text is already written elsewhere in the same
binary: the folded decline row reads `Cast nothing right now … - and play no land in this window`.

### HIGH-3 — the separate land ask still fires; the corpus runs two land regimes at once
**Repro:** `1789690346-…-vs-ai_baka_deck152.jsonl` seq 34 and seq 37 (both T12), a standalone `ask`
window whose entire option list is `['Play Marsh Flats', 'Play no land right now']`.
Counts for this seat: **20 separate land asks** vs **27 folded land rows**. Per game (separate /
folded): deck125 4/10, deck50 2/5, deck162 3/2, deck130 3/5, deck146 4/2, deck152 4/3. The fold
appears to apply only to some windows; the old ask is still reachable in every game. Until this is
one regime, the P9 revert criterion cannot be evaluated cleanly (see §3).

### MED-1 — P5: the compacted GAME LOG drops the actor from every "cast" line
**Repro:** `1789690308-…-vs-ai_baka_deck125.jsonl` seq 62, GAME LOG, T16 block:
`T16 (you): drew Bloodline Keeper.
  Main 1: cast Bloodline Keeper ({2}{b}{b}, 4 sources) -> resolved; cast Intruder Alarm ({2}{u}, 3
  sources); cast Path to Exile; your Bloodline Keeper was exiled from the battlefield; their Path to
  Exile resolved and went to the opponent's graveyard; played Marsh Flats.`
Inside a `(you)` block the opponent's removal is rendered as an unattributed `cast Path to Exile`,
between two of the seat's own casts; only the later `their Path to Exile resolved` clause discloses
the owner. Same shape at T8 of the same log. **The old register did attribute it** — the `events`
field of seq 56 in the same file reads `- Opponent cast Staff of Nin`. 30+ lines in this seat's six
logs are of the unattributed form. No decision in these six games is provably traceable to it, but
"who has removal up" is exactly the fact the cast-gate section of the guide is built on.

### MED-2 — discard / bottom / reveal seams print bare card names with no verdicts at all
**Repro:** `1789690333-…-vs-ai_baka_deck162.jsonl` seq 41 (Cleanup, discard). Full option list:
`['Bloodline Keeper', 'Bloodline Keeper', 'Intruder Alarm', 'Thraben Doomsayer', 'Marsh Flats',
'Damnation', 'Tribute to Hunger', 'Devour Flesh']`. No cost, no type, no `copy N of N`, no
`dead right now: 0 legal targets`, no `spare: you control N lands`. Every discard, bottom and reveal
window in all six games is the same bare list (deck125 seq 6 and 9; deck162 seq 41, 54, 83;
deck130 seq 45; deck146 seq 4; deck152 seq 33). The guide's DISCARD AND SACRIFICE ASKS section is
written to read verdicts that this seam never prints, and the two identical `Bloodline Keeper` rows
at seq 41 are not even marked as copies of one another. The seat's picks happened to be right all
three times.

### MED-3 — the Idyllic Tutor search window lists the whole library, unfiltered and uncollapsed
**Repro:** `1789690335-…-vs-ai_baka_deck130.jsonl` seq 45 (seam `reveal`): **45 rows**, the entire
library — `Underground Sea` ×4, `Tragic Slip` ×3, `Bloodline Keeper` ×3 … — for a card whose own
casting row said `finds only an enchantment card`. Nothing marks which rows are legal, and the
identical entries are not collapsed to a range the way P3 collapses hand entries. Same at deck125
seq 9 (48 rows). Both picks were correct, but the surface is large and offers illegal answers.

### MED-4 — P5: damage that fails to kill a creature prints no outcome line
**Repro:** `1789690335-…-vs-ai_baka_deck130.jsonl` seq 75, GAME LOG, T20 block:
`Starstorm -> 4 damage to Lord of Lineage; Starstorm -> 4 damage to Rorix Bladewing; your Vampire
died …`. Both survived; neither says so, while every body that died carries a `died` clause. The P5
register's own rule is "damage carries the new life total"; creatures have no equivalent, so a
reader counting the board from the log undercounts survivors.

### LOW-1 — 10 consecutive identical windows in one turn
`1789690333-…-vs-ai_baka_deck162.jsonl` seq 16–25, all T5, phases Upkeep→Cleanup, all 3 rows, all
answered `Cast nothing right now`, every one a live round trip (latencies 63 s–160 s). ~19 minutes of
wall clock for one turn in which nothing was castable. `crossphase_identical_reputs` = 9 for this
game; the latch reserved 18 of 52 ask answers. Worth a look at whether a cross-phase identical
re-put with an unchanged board can be latched at the first window rather than the second.

### Checked and clean
- `WAGIC addToGame REFUSED a garbaged element` (bug-list #12): **absent from all 21 stderr files** in
  the corpus dir, not just this seat's six.
- The single corpus-wide heuristic fallback is in a deck50 seat log, not this one; this seat had 0.
- All 12 `async_drops` in this seat's games logged `[outcome: re-asked]` with reason "the question
  (or turn/phase) moved" / "the question and board moved". Each re-ask was correct: the board had
  moved. No answer was applied to a stale question.
- P10 order seam: this seat got 0 `order` windows. Nothing to report on the whole-order answer.
- `multi_answer_first_taken` 0, `menu_pass_no_progress` 0, `x_cast_row_refusal_markers` 0,
  `plan_names_stranded_card` 1 (deck162), `plan_names_uncastable_zone_card` 2 (deck125) in six games.

---

## 3. P9 and P5 counts

**P9 — the land drop as a casting-menu row.**
- Land-drop decisions offered: **47 windows** over the six games — 27 as a folded row on the casting
  menu, 20 as the old standalone land ask (per-game split in HIGH-3). The two regimes coexist.
- **Turns where a land drop was available and NOT taken: 1.** deck50, T16, seqs 118 and 119 — the
  seat answered `Cast nothing right now (combat comes next this turn) - and play no land in this
  window` twice. This was correct and the guide required it: lethal was on the board and the seat
  won that turn on the attack (seq 133, 20 attackers, opponent to −24). The guide's rule is "LETHAL
  IS ALREADY ON THE BOARD … the ONLY job left is to REACH the attack step: every window before it is
  a pass - cast nothing, draw nothing, tap nothing."
- **Turns where the land row was taken instead of a spell that should have gone first: 0.**
- Turns where a spell was taken *before* an available land row on the same menu (the inverse of the
  guide's "land first"): **3** — deck125 T16 (seq 56 Keeper, 57 Alarm, then 58 land), deck146 T10
  (seq 14 Keeper, then 15 land), deck152 T12 (seq 32 Tutor, then 37 land). None cost a castable
  spell: deck125 had 8 sources for a 4+3 turn, deck146's land was Marsh Flats (no mana until
  cracked), deck152's likewise.
- One wobble inside the *old* ask, not the fold: deck152 seq 34 (T12 Main 1) answered `Play no land
  right now` with the PLAN "Hold mana and pass this turn", then took `Play Marsh Flats` at seq 37 in
  Main 2 of the same turn. The drop was not lost.

No evidence of degradation from P9 in this seat's six games. The one criticism of it is HIGH-2/HIGH-3
— the row shipped, the header text and the old ask did not go away.

**P5 — the compacted GAME LOG.**
- Decisions where the compacted log omitted or mis-stated a fact the seat needed, and the misread
  is traceable to a choice: **0**.
- Distinct register defects found: **2**, both with repros above — MED-1 (the actor dropped from
  every `cast <Name>` line; old register printed `Opponent cast <Name>`, visible in the `events`
  field of the same records) and MED-4 (non-lethal damage to a creature prints no outcome, so
  survivors are invisible where deaths are explicit).
- Everything else in the register held up under checking: the sweep batch at deck130 T20 collapsed
  25 damage lines and 23 death lines with explicit `[x25 …]` / `[x23 …]` counts and dropped no fact;
  mana payments carry cost and source count; draws, land drops and life totals are all present on
  the turn line as designed.

---

## 4. Guide verdict — **KEEP as is**

`projects/mtg/bin/Res/ai/baka/deck123_strategy.txt`, 19,952 B, unchanged.

The reason is what the six games actually show. Four decisions decided the five losses, and **three
of them were violations of rules the guide already states, correctly, by name**:

| decision | the rule already in the guide |
|---|---|
| deck130 seq 62–67: built 26 creatures on its own turn with Starstorm in their graveyard | "SWEEPER SEEN (… **Starstorm** …) … Fire at THEIR END STEP." |
| deck130 seq 78: took the hold row over a live Create row at M=3, stop=23 | "THE HOLD ROW IS A P8 ANSWER AND NOTHING ELSE … a Create row below the stop - and the hold is wrong" |
| deck162 seq 101: cast Vision Skeins into three draw punishers at DRAW PRICE X=4 | "Never against a card that draws them extra or punishes drawing" + "Decline at X of 5 or less" |

Nothing is added by writing any of those a second time; the guide is not the thing that failed. The
deck130 loss in particular is explained by HIGH-1 — the hold row's name, not the guide's text — and
the fix for it belongs in the interface, where a rewrite is free, rather than in a guide sitting 48
bytes under a hard 20,000-byte ceiling where every addition must evict a rule that is currently
being obeyed.

The guide's positive control is in the corpus too: the one win (vs deck50) is the ORDER OF
OPERATIONS, TIMING THE CHAIN and ATTACKING sections run end to end without deviation — maker, Alarm,
the repeat row **on the opponent's turn** (seq 75), then all 20 attackers at seq 133 for −24. The
instruction set produces the intended line when it is read.

**One defect recorded but deliberately not edited**, for the synthesis seat's attention: the
DAMNATION section's maker-veto is absolute — "A TOKEN-MAKER OF YOURS IN THE 'M of yours' LIST IS A
VETO OF ITS OWN … whatever N and K say" — with no life boundary, while the very next bullet grants
one to the M=0 case ("at your life 10 or less, 'hold' is not an option"). At deck146 seq 22 the seat
was at **2 life** facing three attackers with the Keeper inside the M list; obeying the veto loses
immediately, and the seat correctly ignored it. That is a real asymmetry in the rule. It is not
edited here for three reasons: the model already plays the hard case right unprompted, so the edit
buys no behaviour; the natural repair is a concessive clause on a rule's hard case, which A334
forbids, and the non-concessive repair (restating the veto with its boundary built in) costs more
bytes than the 48 available and would have to evict an obeyed rule; and one window is a thin
evidence base for spending a rule's worth of the ceiling.

No `strategy.txt` is written, so the `check-reply-instructions.py` gate does not apply.

---

## 5. Proposals

None. No `general-proposals.md` and no `skill-proposals.md` from this seat. The general guide is not
loaded at runtime and nothing in this seat's evidence is general rather than deck-specific; and the
one candidate for a strategy-writing-skill amendment — the absolute-veto asymmetry in §4 — is
already governed by A334, so a proposal would restate an existing amendment.

---

## What I did NOT check

- The opponents' seat logs beyond the board state they put in front of deck123 — I read the six
  deck123 logs in full and used the opponents' only for context lines quoted above.
- Card-script correctness against Oracle text for any card in either deck. Nothing in these games
  looked mechanically wrong (Intruder Alarm untapped on every creature ETB, Fateful hour printed
  +2/+2 results, Lord of Lineage's Vampire lord and the Greaves' haste/shroud all rendered), but I
  did not open a single `primitives/*.txt` or retrieve any Oracle text.
- The `askreplay/` cache files — I read the seat logs' `ask_replays_*` counters but did not open the
  three deck123 replay files to verify what was served from cache.
- Any engine source. No file under `src/` or `bin/Res` was read or edited; wagic was not run and git
  was not touched.
- Latency/throughput beyond noting LOW-1; I did not profile the seat's 97 s median round trip.
- The corpus-wide census — I recomputed only this seat's numbers and took the brief's corpus totals
  as given.
