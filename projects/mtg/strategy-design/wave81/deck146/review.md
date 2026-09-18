# wave-81 review — deck146 (Orzhov Dungeons)

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260917-191141-final`, binary
`archives/wagic-6e0c51238-w81step1`. Seat record: **4 wins / 6**. Seat logs are the six
`*-ai_baka_deck146-*` files; opponent logs read for context. 342 window records (ask 272, priority 32, attackers 30, blockers 3, discard 3, order 2), 0 fallbacks,
0 `action_before_plan`, 0 `reply_truncated`, 0 re-asks (`reask_reason` absent on every record).

Guide verdict: **EDIT** (one rule added, one removed) — see section 4. `strategy.txt` = 19,999 B.

---

## 1. Game by game

| file (epoch) | opp | result | turns | end life |
|---|---|---|---|---|
| 1789690329 | deck50 | WIN | 11 | 21 / -3 |
| 1789690342 | deck130 | WIN | 13 | 20 / -2 |
| 1789690340 | deck123 | WIN | 17 | 21 / 0 |
| 1789690318 | deck162 | WIN | 17 | 9 / -7 |
| 1789690314 | deck152 | LOSS | 15 | -2 / 5 |
| 1789690316 | deck125 | LOSS | 47 | -11 / 63 |

**vs deck50 (win, 11 turns).** Curve-out: Adventurer T3, second Adventurer T5, Silencer T7 naming
Dream Fracture (highest `copies not yet in a public zone`, guide-correct), Command pump T9, Hive
animated at Upkeep T11 (seq27) under CHECK 0 with lethal on the table. No deciding error.

**vs deck130 (win, 13 turns).** Deciding decision seq25/26: Vanishing Verse on Rorix Bladewing
(6/5 flier, haste) the turn it landed. Land drop taken on every available turn including two
land-backed rows at the priority seam (seq10 Pelakka Caverns, seq13 Emeria). Silencer named
Lay Waste (N=4) over the plan's Stone Rain (`visible now: 1 in their graveyard`) — the guide's
"highest N, never a row whose copies are spent" rule beat the carried plan, correctly.

**vs deck123 (win, 17 turns).** Barrowin + Command edict (seq19-21) cleared Bloodline Keeper and
put them to 5. Lolth landed seq25 and -3'd the same turn (seq26) and again seq29 — the
"USE THEM THE TURN THEY LAND" rule fired exactly as written, and the Spiders carried the last
two turns. Only blemish: seq31 cast Acererak with the plan "generate a 2/2 Zombie token" — the
guide's ACERERAK IS NOT A BODY paragraph says he bounces himself; the game was already won.

**vs deck162 (win, 17 turns).** Ventured four times off two Adventurers; Dark Pool drains
(seq19, seq32) plus Hive animation closed it. seq22 declined the real land (Shineshadow Snarl)
and spent the drop on Pelakka Caverns at seq24 instead — the guide says "Play a real land first
and hold the land-backed card". Cost nothing here; the same habit cost the deck125 game (below).

**vs deck152 (LOSS, 15 turns) — the deciding sequence is turn 13.**
- seq34 (Upkeep, life 5/8): took `becomes beholder ... with Hive of the Eye Tyrant [cost: {3}{b}]
  {spends 4 of your 6 untapped mana sources this turn}` at the **Upkeep** offer. The guide's HIVE
  CHECK 0 did not fire (2 untapped Spiders = 4 power, +3 = 7, under their printed 8) and CHECK 0b
  did not fire (their line showed 7 creatures), so CHECK 1 — "Phase: says Upkeep and CHECK 0 did
  not fire -> PASS" — was the answer. Spending 4 of 6 at Upkeep left 2, and seq36's own hand line
  then read `Soul Shatter {2}{b} [instant] [cannot pay now: needs 3 mana, you have 2 untapped
  sources]`. Tovolar's Huntmaster (6/6) survived the turn; the crack-back line that turn read
  `up to 21 ... that would KILL you`. STRATEGY (guide rule present and violated), not perception.
- seq36 (Main 1, same turn): the menu was `1. +0: draw card ... Lolth`, `2. -3: create spiders
  with Lolth {spends 3 loyalty counters - Lolth has 3 now, leaving 0}`, `3. Hold priority`,
  `0. Pass`. The seat took **3 (Hold priority)** — the guide's absolute is "NEVER hold a list
  offering a Kaya or Lolth loyalty ability or the Hive animation". Engine lane DK rescued it: the
  hold_event at seq40 (`reopen_new_lethal`, face `[crack-back verdict: LETHAL]`) re-opened the
  window and the -3 fired at seq41 — but in **Main 2**, so the two Spiders were summoning-sick and
  could only block (seq42), never attack. Same reply was prose-only (no `PLAN:` label, 522
  off-protocol bytes); it parsed.
- seq45 (T15 Upkeep): animated Hive at Upkeep again, plan "Animate Hive to block Brute" — an
  animation taken in one's own upkeep ends at end of turn and cannot block on their turn. The row
  printed `[Upkeep offer: this animation lasts only until end of turn, and the same row is offered
  again in your main phase]`, so the surface was TRUE and the plan was false against it.

**vs deck125 (LOSS, 47 turns) — countered out, and the guide binned the win condition.**
deck125 held Cancel / Essence Scatter / Dream Fracture / Fall of the Gavel / Supreme Verdict with
15 untapped sources late; `opp_countered_spells` = 12, `counter_intel_rendered` = 1319.
- seq23 (T15): declined Lolth citing the guide's COUNTER TEST — "Hold off on casting Lolth due to
  their known counterspells and untapped mana." That is entry 2 of THE WHOLE CAST ORDER working
  as written.
- seq48 (T19 cleanup, 1 discard): rows 5 and 6 were both `Lolth, Spider Queen {3}{b}{b}`.
  Reply: `PLAN: Discard highest mana value card to comply with hand size limit. / PUT: 5`.
- seq60 (T23 cleanup): row 6 `Lolth, Spider Queen`. Reply: `PLAN: Discard highest mana value card
  per cleanup rules. / PUT: 6`. **Both Lolths gone by turn 23.** The seat quoted the guide's
  CLEANUP ladder rule (1) "Highest printed mana value among cards NEITHER LAND NOR LAND-BACKED"
  and the rule fired exactly as written on the two cards that were the only way to beat a deck
  answering every body. This is the guide defect the EDIT fixes.
- seq128 (T35): with Emeria's Call the only land-capable card in hand and the drop unspent, the
  seat took `Cast Card Normally [cost: {4}{w}{w}{w}]` over
  `Emeria, Shattered Skyclave [PLAY THIS AS A LAND...]` into 15 untapped {u}{w} sources. Cancel
  countered it; the card AND the land drop were lost in one window. The guide's rule ("take its
  land row when the alternative is no drop") was violated, not defective.
- From T29 to T45 the seat walked Nadaar, Acererak x2, Emeria's Call, Silverquill Command,
  Triumphant Adventurer, Silverquill Silencer and Soul Shatter into counters one per turn. The
  guide's counter test is scoped to entry 2 (Lolth/Kaya) only; nothing in it reaches a 7-mana
  sorcery or a lone creature. Evidence only — no edit made (see section 5).

---

## 2. Engine / interface / card items

**HIGH-1 — the `Land drop:` header contradicts the P9 folded menu (all six games).**
Repro: `1789690316-ai_baka_deck146-0x555bfc3f8960-vs-ai_baka_deck125.jsonl` seq63. Header:
`Land drop: a land play IS available to you right now. The land drop is its OWN decision (a "Land
drop:" question with its own Play options), so the absence of a land from the choices below does
not mean the drop is gone. ONE exception: a modal double-faced card whose back face is a land is
offered in the casting menu on its own row, marked PLAY THIS AS A LAND and USES YOUR LAND DROP`.
Option 4 on that very menu is `Play Plains` — a plain basic land, not an MDFC back face. P9 folded
the land row into the casting menu; the header still tells the model that menu cannot carry one
and names a single exception that does not cover it. 17 folded-menu windows in this seat alone
(vs125 seq63/65/98/101/104/116/171/203; vs152 seq17; vs162 seq8/14; vs50 seq6/10/19; vs123 seq10;
vs130 seq6/19). No misplay traced to it here, but it is a false statement on the screen and the
`Land drop:` line is the only place the drop's availability is asserted.

**MED-2 — the HOLD row is offered where every other row is a free loyalty ability.**
Repro: `1789690314-...-vs-ai_baka_deck152.jsonl` seq36 (quoted in section 1). Rows were Lolth +0,
Lolth -3, Hold, Pass — nothing on the menu costs mana, so taking the hold can only give value away,
and the same screen printed `[crack-back verdict: LETHAL if it is UNBLOCKED]`. Suggest suppressing
the hold row (or printing the X-style refusal marker on it) when every non-pass row is a zero-mana
activated ability, at least while the crack-back verdict reads LETHAL.

**MED-3 — `verdict_clamped` storm: 44 of the corpus's 108 `hold_verdict_safer_ignored` are one
game.** Repro: `1789690316-...-vs-ai_baka_deck125.jsonl`, hold_event seq140 onward,
`reason: held [stack death verdict: you survive the stack] over live [stack death verdict: nothing
lethal on the stack]`, in bursts of 9 / 10 / 10 / 15 on turns 38 / 40 / 42 / 46 — one per Staff of
Nin ping. The two verdict faces are operationally the same statement ("the stack does not kill
you"), so the hold is re-evaluated and clamped on every trivial stack object. Folding those two
faces into one would remove the burst without changing any decision.

**MED-4 — P5: opponent spell casts render with no actor prefix.** Repro: same file, seq230's GAME
LOG, T35 line: `Main 1: paid ({4}{w}{w}{w}, 7 sources) for Emeria's Call; you chose Cast Card
Normally for Emeria's Call; cast Emeria's Call; cast Cancel; your Emeria's Call was COUNTERED by
Cancel and went to your graveyard; their Cancel resolved`. Their Cancel appears as a bare
`cast Cancel`, identical in shape to the seat's own `cast <X>`, while every other opponent action
in the compacted register carries `their`/`opp`. 12 such lines in that game. Adding the actor
("opp cast Cancel") costs 4 bytes per line and removes the only ambiguity found in the new
register.

**MED-5 — the Upkeep Hive row does not say the animation cannot block on their turn.** Repro:
vs152 seq45. The row's clause is `[Upkeep offer: this animation lasts only until end of turn, and
the same row is offered again in your main phase]`, which is true but is about the SAME turn; the
reply planned to "Animate Hive to block Brute" on the opponent's turn. Extending the clause to
name the block case would close a plan the current wording does not refute.

**LOW-6 — `async_drops` has no per-window record.** This seat carried 5 of the corpus's 52
(vs152 3, vs123 1, vs130 1). The counter exists only on the `gameend` record; no window record
carries a drop field (grep for `async` in a seat log returns only `async_drops`). The brief asks
each seat to say what dropped and whether the re-ask was right — the corpus cannot answer that.

**LOW-7 — `garbageReaddRefused` check is CLEAN.** `/usr/bin/grep -l 'REFUSED a garbaged element'`
over all 21 `*.stderr` in the corpus dir returns nothing.

**LOW-8 — P10 order seam, first live firing (2 windows, both this seat).** vs152 seq38 and seq39,
both answered `ORDER: B2, B1`, both parsed as whole-order answers (`chosen_text` "Wolf,
Briarbridge Tracker" and "Wolf, Tovolar's Huntmaster"); no fallback to per-pick asks. seq39's
reply was the bare line `ORDER: B2, B1` with no plan at all and still parsed.

**LOW-9 — parser robustness confirmed on two more shapes.** Attackers answered by card NAME
instead of A-number parsed correctly (vs130 seq21 `ATTACK: Triumphant Adventurer, Silverquill
Silencer, Goblin`; vs130 seq32; vs123 seq40 `ATTACK: Spider`). 19 of 342 windows dropped the
literal `PLAN:` label (`protocol_deviation: unlabelled_plan`); in 18 the plan sentence was present
and the answer line followed it, and the answer was read in every one.

---

## 3. P9 and P5 counts (this seat)

**P9 — land drop folded into the casting menu.**
- Turns where the prompt's `Land drop:` line read "a land play IS available": **49**.
- Turns where the drop was available and **no land was played**: **2**.
  - vs125 T35, seq128 — declined `Emeria, Shattered Skyclave [PLAY THIS AS A LAND]` to hard-cast
    Emeria's Call into 15 open sources; countered.
  - vs123 T17, seq39 — declined `Emeria, Shattered Skyclave with Emeria's Call -> PLAY THIS AS A
    LAND` to pass to combat; that combat won the game.
- Turns where a **land row was taken instead of a spell that should have gone first**: **0**.
  17 windows carried both a land row and a live cast row (listed under HIGH-1); the land was taken
  in 9 of them, and in every one either the spell was cast later the same turn (vs152 seq17 ->
  seq18 Lolth; vs125 seq65, seq104, seq116, seq171, seq203 — all with only `{dead right now}` cast
  rows beside them; vs130 seq6 -> seq7 Adventurer; vs123 seq10 -> seq11 Acererak) or the alternative
  row was priced dead on its own tag (vs50 seq19).
- No revert signal from this seat: the folding cost nothing measurable. The one thing folding did
  NOT fix is the stale header (HIGH-1).

**P5 — compacted GAME LOG.**
- Facts the log **omitted** that a decision needed: **0** found.
- Facts the log **mis-stated**: **1 class**, MED-4 above (opponent spell casts without an actor
  prefix; 12 lines in the deck125 game). Everything else checked clean against the per-record
  `events` field: counters, day/night, venture steps, room names, room effects, damage with the new
  life total, draws folded onto the turn line, and counter events (`your X was COUNTERED by Y`)
  are all present and correct.

---

## 4. Guide verdict: EDIT

`strategy.txt` in this directory, 19,999 B (live guide 19,995 B). `wc -c` 19999 <= 20000;
`check-reply-instructions.py src <guide>` reports OK (5 source files, 1 guide); A332 grep, the
frequency-adjective grep and the A334 concessive grep each print 0.

**Edit 1 — CLEANUP DISCARD rule (1) gains a closed exception.**

before:
```
 (1) Highest printed mana value among cards NEITHER LAND NOR LAND-BACKED. Emeria's Call, Agadeem's
     Awakening and Pelakka Predation are LAND-BACKED and not eligible; the row prints them as plain
     sorceries and says nothing of it.
```
after:
```
 (1) Highest printed mana value among cards NEITHER LAND NOR LAND-BACKED - never Kaya or Lolth if you
     control neither. Emeria's Call, Agadeem's Awakening and Pelakka Predation are LAND-BACKED
     and not eligible; the row prints them as plain sorceries and says nothing of it.
```
Paid for by: seq48 and seq60 of `1789690316-ai_baka_deck146-0x555bfc3f8960-vs-ai_baka_deck125.jsonl`
— the seat binned both copies of Lolth at cleanup, quoting this rule, against a deck that answered
every creature it cast; it then spent 24 more turns with no win condition and lost at 63 life.
Lolth and Kaya are the deck's highest mana values, so rule (1) selects them by construction, and
entry 2's counter test defers them exactly in the matchups where they are the only way to win. The
exception is flat (not concessive, A334), observable at the ask (the battlefield line is on the
screen), and scoped to the deck's two planeswalkers so it cannot swallow the rule.

**Edit 2 — removal that pays for it: the MAD MAGE clause in WHICH DUNGEON.**

before: `theirs; MAD MAGE only in a slow grind for cards.`
after: `theirs.`

"a slow grind for cards" is not a fact any window prints, so the clause is a release the pilot
cannot act on (anti-pattern: unreachable condition). It never fired: all six games answered LOST
MINE OF PHANDELVER by name at every three-dungeon ask (vs50 seq9, vs130 seq12, vs123 seq12,
vs162 seq11/seq26/seq38, vs125 seq75, vs152 seq10). With it gone the section still names Lost Mine
as the standing answer and keeps Tomb's one printed-life condition.

No other change. Byte-reclaim and reordering edits were not made.

---

## 5. What I did NOT check / did not change

- **The counter test (cast order entry 2) is untouched.** The deck125 evidence shows it deferring
  Lolth from T15 to a discard at T19, and I could construct a release ("a cleanup discard ask with
  her in hand ends the test"), but I cannot show from the corpus that casting her into four live
  counters would have won that game — that would be recording a hypothesis of mine as a finding.
  Flagged here for synthesis instead; if the loop wants it, the byte payer is available.
- Opponent seat logs were read only for board context, not audited.
- I did not compute per-seat latency or reasoning-length statistics; I did not read
  `askreplay/`, `lane-DI/DJ/DK/DL/DM` or the audit lanes beyond what the brief summarised.
- I did not verify any card's Oracle text against Gatherer/Scryfall this wave — no card behaved
  wrongly in these six games, so no card-script item is raised.
- `async_drops` (5 in this seat) could not be attributed to windows (LOW-6); the seq gaps in the
  logs do not line up with the counters, so I make no claim about what dropped.
- I did not read `wave68/strategy-writing-skill-v2.md` in full — I ran its section-2.2/5/6
  verification greps and checked my edit against A332, A333 and A334 specifically. No
  skill-proposal is filed for that reason.
