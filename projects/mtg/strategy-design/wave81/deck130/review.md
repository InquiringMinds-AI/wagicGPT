# wave-81 step-3 review — deck130 (Budde's Ponza), 3/6

Corpus `matchups-20260917-191141-final`, binary `wagic-6e0c51238-w81step1`. Six seat logs, all
`gameend`, no fallback, no re-ask, no truncated reply, no `action_before_plan`. Seat totals across
the six: 253 windows (ask 179, priority 61, attackers 11, blockers 1, order 1), 26 hold_events,
13 records flagged `plan_line_missing` (the `gameend` counters report 8 + 3; the per-record count is 9 in W50 and 4 in W123), 7 `async_drops`, 0 discard/bottom/reveal windows.

File short-hand below: **L152** = `1789690312-ai_baka_deck130-0x56515a193370-vs-ai_baka_deck152.jsonl`,
**W50** = `1789690320-…-vs-ai_baka_deck50.jsonl`, **L162** = `1789690327-…-vs-ai_baka_deck162.jsonl`,
**W125** = `1789690331-…-vs-ai_baka_deck125.jsonl`, **W123** = `1789690335-…-vs-ai_baka_deck123.jsonl`,
**L146** = `1789690342-…-vs-ai_baka_deck146.jsonl`. `seq` is the record `seq`.

## 1. Game by game, and the decisions that decided them

### L162 — LOSS, turn 11, deck130 at -1 / deck162 at 17 (shortest game of the six)
Their board by T7 was Master of the Feast (5/5 flier), Fate Unraveler (3/4) and two Liliana's
Caress. The seat's only creature answer was Starstorm, whose affordable X never reached 5.
- **seq 10 / seq 11 (T6, opp life 20) — the deciding misplay.** The cast row printed
  `{kills 0 of the 1 CREATURE target at 3 damage - and 3 to the opponent at life 20 leaves them at
  17}`. The seat wrote `PLAN: Cast Hammer of Bogardan targeting Master of the Feast` and took the
  cast anyway; the target ask that followed (seq 11) offered only `1. The opponent (player, life
  20)`, `2. Master of the Feast … - SURVIVES (toughness 5)`, `3. Yourself`, and states
  *"this ask has no pass row"*. It took the face. STRATEGY — but see HIGH-2: the guide's refusal
  was written for a window that cannot refuse.
- seq 19 (T8, life 14) Lay Waste at Drowned Catacomb, seq 30 (T10, life 4) Starstorm X=4 killing
  Fate Unraveler. Both correct-by-guide and both too late; the game was already decided by the
  unanswerable 5/5 flier. No further seat error.

### L152 — LOSS, turn 17, deck130 at -9 / deck152 at 11
Land destruction landed (Hengegate, Tidechannel, Lair of the Hydra all destroyed) but 152 deployed
Katilda, Sigarda and Luminarch Aspirant; Sigarda grew 4/4 → 6/6 on counters and flew over.
- **seq 40 (T16, attackers) — the losing decision.** Life 7. `CRACK-BACK COST OF ATTACKING: … (15
  damage, which puts you at -8)`; `{crack-back cover, STAY HOME: keeping all 2 of them back covers
  11 of that 15, leaving 4.` The seat wrote `PLAN: Attack with Rorix to pressure; keep Blastminer
  to block next turn` and sent Rorix (`ATTACK: A2`), with `ATTACK TOTAL … NOT lethal … they would
  be at 11`. The guide's CRACK-BACK EXCEPTION is explicit — at "for up to N" ≥ "Your life:" send
  only vigilance or a Rorix whose 6 is lethal THIS turn — and Rorix's 6 was not lethal at 17.
  STRATEGY, against a rule the guide already states. Keeping Rorix home covered 5 of the 15 (Sigarda
  tramples, so the cover is partial) — probably still lethal, but attacking made it certain.
- seq 23/24 (T8, opp life 20, their line 0 creatures / 3 lands) Spark Spray cast and pointed at
  the face. The 6-LIFE FLOOR's release needs 0 creatures AND ≤2 lands; at 3 lands it had not
  released, and seq 24 row 2 offered `cycling` — the guide's CYCLING rule sends Spark Spray to the
  cycle exactly here. STRATEGY, same mechanism as L162 seq 10 (a no-kill cast row taken anyway).

### L146 — LOSS, turn 13, deck130 at -2 / deck146 at 20
- **seq 9 (T4, priority).** Their line read 0 creatures only because Spark Spray had just killed
  Triumphant Adventurer that same upkeep; their graveyard named it. The seat took
  `3. Draw 1 with Pyrite Spellbomb`, cashing the charge. Silverquill Silencer (3/2) landed T7 and
  a second Triumphant Adventurer T5 — two bodies the loaded Spellbomb kills. The guide licensed
  this ("draw mode only when their line reads 0 creatures"); see the EDIT.
- **seq 19 (T10, own Upkeep).** Took `1. cycling with Forgotten Cave`. The guide says, in capitals,
  NEVER TAKE A CYCLING ROW AT YOUR OWN UPKEEP. STRATEGY, against an existing rule.
- seq 16 (T8) cast Lay Waste while the row read `[NAMED BY THEIR Silverquill Silencer: casting this
  costs you 3 life and draws them a card]` at life 19 — 19→16, and the Silencer then beat for 3/turn.
  The guide has no clause for a named-card tax; a land kill for 3 life and a card against a
  creature-based aggro deck is the LAND PLAN spending life it needed. Recorded, not paid for by a
  second occurrence.
- seq 23 (T12) Rorix cast at life 6 with the board already lethal next turn; it never attacked.

### W50 — WIN, turn 18, 20 / -3
Clean. Blastminer attacked into a creatureless mill deck every turn (seq 18/24/32, correct per the
ATTACK FLOOR's Blastminer case), Lay Waste and Stone Rain kept 50 off Islands, and two Rorix casts
(seq 36 T14, seq 41 T16) closed it 17 → 9 → 3 → -3. One counter (`opp_countered_spells` 1). Nine
`unlabelled_plan` replies here (see LOW-1).

### W125 — WIN, turn 24, 20 / -1
Also clean. Three Stone Rains plus Molten Rain stripped 125 to one Island, Siege-Gang (seq 29 T12)
and Rorix (seq 72 T22) did the work, and the seat never dropped below 20 life. Two notes:
- seq 34 (T14) Hammer at the face at opp life 18 and seq 66 (T20) Siege-Gang sacrifice at the face
  at opp life 13 are **legal** — their line read 0 creatures and 2 lands, then 0 and 1, so the
  6-LIFE FLOOR's release clause had fired. The release mechanism works.
- seq 44 (T16 Main 1), seq 59 and seq 61 (both T18 Main 2) cycled Starstorm. The guide says second
  main phase only and never twice in one turn; seq 44 broke the first, seq 59+61 the second. Cost
  nothing here (it cycled into more Starstorms) but it is two more rule violations.

### W123 — WIN, turn 22, 11 / 0
Won on seq 77, Hammer returned at upkeep (seq 74) and pointed at a face row reading
`{… they would be at 0; THIS WINS THE GAME}`. The interesting turn is T20.
- **seq 68/70 (T20 upkeep) Starstorm X=4** against 25 Vampires: the X row read
  `{X pricing: kills THEIRS: Vampire x25; YOURS: Dwarven Blastminer}`. Rorix (6/5) was correctly
  NOT on the YOURS list, so the guide's rule (a) did not fire and the cast was right.
- **seq 72 (T20 attackers) — HIGH-1 below.** Rorix attacked carrying 4 damage from that same
  Starstorm and died to two 4/4 blockers the render had priced as safe.
- seq 73 — the **P10 order window, this seat's only one**: the seat wrote `ORDER: B1, B2` and it
  PARSED (`chosen_text` `'Vampire, Vampire'`). Flagged `prose_outside_two_lines`,
  `off_protocol_bytes` 13 — the PLAN line asserts "Rorix deals 6 damage to opponent, winning the
  game" while Rorix is blocked by two creatures, but the ORDER line itself was clean.

## 2. Engine / interface / card items

**HIGH-1 — an attacker's "their untapped blockers" verdict ignores damage already marked on it.**
`W123` seq 72 (window_seq 60, seam attackers, T20). The same prompt's GAME LOG carries
`Starstorm -> 4 damage to Rorix Bladewing`, and the battlefield line is
`Rorix Bladewing {3}{r}{r}{r} (6/5) [flying, haste, doesn't untap during its controller's untap
step] [untapped]` — **no damage-marked tag anywhere on the screen outside the log**. The attacker
row then reads:

    A1. Rorix Bladewing (6/5) [flying, haste, …] [their untapped blockers: Lord of Lineage (5/5)
    (both die); Vampire #1 (4/4) (you kill it, your attacker lives); Vampire #2 (4/4) (you kill
    it, your attacker lives)]

With 4 already marked, one 4/4 blocker kills Rorix; the prompt says *"Do not re-derive these
outcomes; use them."* The seat used them, attacked, and lost its only win condition (`your Rorix
Bladewing died`, T20 log of the same file's last record). Two fixes, both needed: put the marked
damage on the battlefield/attacker line, and subtract it inside the 1-on-1 result.
Repro: `1789690335-ai_baka_deck130-0x5586b2e3ab70-vs-ai_baka_deck123.jsonl`, seq 72.

**HIGH-2 — a damage spell's target ask has no declining row, so a wrong cast is irreversible.**
`L162` seq 11 (window_seq 8) prints `Write your PLAN: … CHOICE: followed by the number of your
choice (this ask has no pass row)` over `1. The opponent … 2. Master of the Feast … - SURVIVES
(toughness 5) 3. Yourself`. Same shape at `L152` seq 24 (which at least offered `3. Decline - do
nothing`, with the mana already spent). Once the cast row is taken, every guide rule that says
"decline" is unreachable and the face is the only sane row left. Either give the target ask a real
decline that unwinds the cast, or make the cast row carry the refusal (this review's guide EDIT
does the latter from the guide side, but the engine side is the durable fix).
Repro: `1789690327-ai_baka_deck130-0x55bc1e82dda0-vs-ai_baka_deck162.jsonl`, seq 11.

**MED-1 — P9 land row contradicts the "Land drop:" header paragraph, in 44 of 44 windows.**
Every ask window in this seat that carries a `Play <land>` row also carries the legacy sentence:

    Land drop: a land play IS available to you right now. The land drop is its OWN decision (a
    "Land drop:" question with its own Play options), so the absence of a land from the choices
    below does not mean the drop is gone.

…while the same menu's header says `The "Play <land>" rows play that land instead of casting this
window; you have one land drop this turn, and this casting decision is asked again after the land
enters.` The header paragraph promises a separate question that P9 removed. Behaviour did not
suffer this corpus (see §3), but the two sentences cannot both be true.
Repro: `1789690327-…-vs-ai_baka_deck162.jsonl`, seq 29; 44/44 such windows across the six files.

**MED-2 — the compacted Attack line renders an activated ability's damage as the attacker's
combat damage.** `W123` T18. Siege-Gang was BLOCKED by Bloodline Keeper, and the seat activated
`{1}{R}, Sacrifice a Goblin: 2 damage` at the blockers step (seq 57). The log renders:

    Attack: Siege-Gang Commander, Goblin, Goblin -> Siege-Gang Commander: 2 damage, opp 4;
    Goblin: 1 damage, opp 3; used Deal 2 damage with Siege-Gang Commander -> the opponent;
    paid ({1}{r}, 2 sources) for Siege-Gang Commander; …

The record life totals say the truth (opp 6 → 4 on the ability at seq 60, → 3 on the one unblocked
Goblin at seq 64): the `Siege-Gang Commander: 2 damage, opp 4` entry IS the sacrifice ability, and
the `used Deal 2 damage …` line that names it carries no result at all. A seat reading that log
concludes a blocked attacker still hit the player for 2. Fix: name the ability in the
damage-and-life line, and keep the result on the line that names the activation.
Repro: `1789690335-…-vs-ai_baka_deck123.jsonl`, seq 67, GAME LOG block `T18 (you): … Attack:`.

**MED-3 — `async_drops` has no per-record trace.** 7 across this seat's six games (L152 1, L162 1,
W125 2, W123 3) and 52 corpus-wide (wave 80: 9), but the count lives only in `gameend`; no
`window`, `hold_event` or `recovery` record in any of the six files carries a drop field, so the
brief's question — what dropped and whether the re-ask was right — is unanswerable from the
translog. Stamp the dropped window's `window_seq`/seam on a record.
Repro: `1789690335-…-vs-ai_baka_deck123.jsonl`, `gameend` `async_drops: 3`; no other record in the
file mentions it.

**LOW-1 — `plan_line_missing` counts the LABEL, not the plan.** 13 of this seat's records are
flagged (`protocol_deviation: unlabelled_plan`); in every one the plan is present, is a plan, and
PRECEDES the action line — e.g. `W50` seq 14 `"\n\nCast Stone Rain at Island #1.\nCHOICE: 3 (Cast
Stone Rain)"`. The parser read all 13 correctly. The counter's name reads as a protocol failure
rate; it is a label-absence rate.

**LOW-2 — not a bug, recorded so the next seat does not re-file it.** `W123` T19's
`used Create vampire with Bloodline Keeper (x4) … used Create vampire with Lord of Lineage (x21)`
looks like a tap-ability firing many times per turn, but Intruder Alarm ("Whenever a creature
enters, untap all creatures") was on their battlefield from T17 — that is the real Bloodline
Keeper + Intruder Alarm loop, correctly engine-capped.

**Checked and clean:** no `WAGIC addToGame REFUSED a garbaged element` in any of the 6 stderr files
for deck130's games (nor anywhere else in the corpus dir); 0 heuristic fallbacks, 0 re-asks, 0
`reply_truncated` in this seat.

## 3. P9 and P5 counts

**P9 (land drop folded into the casting menu).**
- Ask windows carrying a `Play <land>` row: **44**.
- Turns on which such a row was offered: **38** (L152 6, W50 6, L162 5, W125 7, W123 8, L146 6).
- Turns where a land drop was available and the land was **NOT** played while a land row was on the
  menu: **0 of 38**.
- Turns where the land row was taken instead of a spell that should have gone first: **0**. The
  casting window re-opened after the land entered in every case and the spell followed on the same
  turn — e.g. `L146` seq 20 (cast Talisman) → seq 21 (Play Mountain) → seq 22 (cast second
  Talisman); `W123` seq 41 (Play Mountain) → seq 42 (cast Rorix); `W125` seq 29 (cast Siege-Gang)
  → seq 30 (Play Mountain). Both orders occur and neither lost a spell or a drop.
- No verdict offered; the numbers are 38/38 with zero displacement, plus the header contradiction
  filed as MED-1.

**P5 (compacted GAME LOG).**
- Omissions a deck130 decision needed: **0 found**. Every damage event in the six logs carries its
  new life total; the batch folds (`[x25 - this exact line 25 times in this batch]`,
  `[x23 - 23 lines of this shape in this batch; only the numbers in them differ]`) are accurate.
- Mis-statements: **1**, filed as MED-2 — `W123` T18 `Attack: … -> Siege-Gang Commander: 2 damage,
  opp 4` for damage that a BLOCKED Siege-Gang did not deal in combat; the old register would have
  attributed those 2 to the sacrifice activation and left the blocked attacker with no
  damage-to-player line at all.
- Worth noting against HIGH-1: the log did NOT drop the fact that mattered
  (`Starstorm -> 4 damage to Rorix Bladewing` is right there in `W123` seq 72's log). The false
  surface was the computed blocker tag, not the narration.

## 4. Guide verdict: **EDIT**

Three changes. Live guide 19,832 B → revised **19,988 B** (ceiling 20,000);
`python3 projects/mtg/tools/check-reply-instructions.py projects/mtg/src <dir>` reports OK, 1
guide, no reply-format instruction. Full revised guide: `wave81/deck130/strategy.txt`.

**Edit 1 — ORDER OF OPERATIONS entry 2 gains a cast-window refusal.** (+215 B)

before:
```
 2. A single-target damage spell (Pyrite Spellbomb, Spark Spray, Hammer of Bogardan) aimed by
    TARGETING below. A token-MAKER on their line (Thraben Doomsayer 2/2, Bloodline Keeper 3/3)
    outranks entry 4.
```
after:
```
 2. A single-target damage spell (Pyrite Spellbomb, Spark Spray, Hammer of Bogardan) aimed by
    TARGETING below. A CAST ROW READING "no creature target" or "kills 0 of the N CREATURE
    target" SKIPS THIS ENTRY while the 6-LIFE FLOOR holds: the target ask after it has NO
    declining row and will offer nothing but the face. A token-MAKER on their line (Thraben
    Doomsayer 2/2, Bloodline Keeper 3/3) outranks entry 4.
```
Paid for by `L162` seq 10 (`{kills 0 of the 1 CREATURE target at 3 damage}` cast at opp life 20,
floor not released, face taken at seq 11 where the ask has no pass row) and `L152` seq 23
(`{no creature target - and 1 to the opponent at life 20}` cast, face taken at seq 24 with a
`cycling` row on the same menu). Both errors were committed one window BEFORE the window the
guide's TARGETING section governs; this is the earliest binding point. It deliberately does not
fire once the floor releases — `W125` seq 34 and seq 66 were correct face takes under the release
clause and must stay legal.

**Edit 2 — TARGETING step 2's declining-row enumeration goes; it is the rule that pays Edit 1's
bytes.** (-97 B)

before:
```
    EVERY row says SURVIVES the answer is the declining row of the menu in front of you -
    "Cast nothing right now" on a casting list, the hold row or "0 (pass)" on a priority list -
    and the charge stays loaded for their next 2- or 3-toughness creature.
```
after:
```
    EVERY row says SURVIVES the answer is the declining row of the menu in front of you, and
    the charge stays loaded for their next 2- or 3-toughness creature.
```
It is removed rather than reclaimed: on a target ask — the one menu where "every row says
SURVIVES" is most likely to be true (`L162` seq 11) — none of the three rows it enumerates exists,
so the enumeration told the seat to look for a row that was not there. What survives ("the
declining row of the menu in front of you") is still correct on the menus that have one, and those
menus print that row themselves; entry 7 of ORDER OF OPERATIONS and PLAN LINES AND THE HOLD ROW
already name `"Cast nothing right now"` and the hold row by name.

**Edit 3 — PYRITE SPELLBOMB draw mode gains a second gate.** (+38 B)

before:
```
PYRITE SPELLBOMB: cast for {1} on a spare-mana turn, leave it loaded for their first
2-toughness creature; draw mode only when their line reads 0 creatures.
```
after:
```
PYRITE SPELLBOMB: cast for {1} on a spare-mana turn, leave it loaded for their first
2-toughness creature; draw mode only when their line reads 0 creatures AND their graveyard names
no creature.
```
Paid for by `L146` seq 9: their line read 0 creatures only because Spark Spray had killed
Triumphant Adventurer that same upkeep, and their graveyard named it on the same screen
(`Their graveyard (1 card): Triumphant Adventurer {b}{w} (creature 1/1)`). A second Adventurer
landed T5 and Silverquill Silencer (3/2) T7 — both dead to the charge that had just been spent on
a card. The new clause is readable in the window and would have declined that row.

**What is NOT edited, deliberately.** The three other rule violations this corpus — `L152` seq 40
(attacking into a lethal crack-back), `L146` seq 19 (cycling at own upkeep), `W125` seq 44/59/61
(cycling outside main 2, twice in a turn) — all ran against rules the guide already states in
capitals. They are compliance failures, not guide gaps; adding emphasis would cost bytes a rule
pays for and buy nothing observable. The `L146` seq 16 Lay Waste into a named-card 3-life tax is a
single occurrence with no second instance to pay for a rule.

## 5. Proposals

None. No `general-proposals.md` and no `skill-proposals.md`: nothing in this seat's evidence shows
the general guide or the wave-68 strategy-writing skill producing a bad rule. The one guide defect
found (Edit 3) is a mis-drawn gate in this deck's own card facts, and Edits 1–2 are a hard case of
the engine's own window shape (HIGH-2), not a skill-edition problem.

## What I did NOT check

- The three losses' OPPONENT seat logs beyond the `W123` T18 damage reconciliation — deciding
  decisions are traced from deck130's own translog only.
- Mulligan, bottoming, discard, blocker and reveal rules: the seat kept its opening hand in all six
  games and drew **0** discard, bottom or reveal windows and exactly **1** blockers window
  (`L152` seq 41, `no blockers` at life 7 facing lethal). Those guide sections got no exercise.
- Whether `L162` and `L152` were winnable at all. I did not model alternative lines; I only
  measured the seat's decisions against the guide and against the printed surfaces.
- Latency, reasoning-channel content and `reasoning_chars` distribution for this seat.
- Card-script/Oracle verification for any card named here. HIGH-1 is an arithmetic gap in the
  blocker-forecast render, not a claim about a card's text; LOW-2's Bloodline Keeper loop I
  reasoned about from the printed Intruder Alarm text in the prompt, not from Gatherer.
- The `hold_verdict_safer_ignored` (2, `L162`) and `plan_names_uncastable_zone_card` (8, `W125`)
  counters beyond confirming they are the designed DK/DL behaviour and cost no decision.
