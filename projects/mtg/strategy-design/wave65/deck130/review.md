# Wave-65 per-deck review — deck130 (Budde's Ponza, mono-red land destruction)

Record this wave 1/6 (3/6 last wave). Corpus:
`/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-033121/`
Seat files (all `1788683NNN-ai_baka_deck130-<ptr>-vs-ai_baka_deckM.jsonl`); 458 records,
0 heuristic fallbacks, 1 `stale_echo` (recovered), 0 transport errors, max `deadline_pct` 94.2
(no >100), 5 `async_drops`, 13 `later_answer_ignored`/`echo_index_conflict` parse notes.

Bottom line: the wave's drop is **not** guide-driven. Two of the five losses are matchup-lock
(126's Sanguine Bond + Exquisite Blood loop against a 60 with zero enchantment removal; 125's
Cancel / Fall of the Gavel wall with deck130 hellbent and flooded). Of the three genuinely
loseable games, **two model-side misplays each contradicted a rule the live guide already
states, and in both cases an engine annotation argued for the choice the model made** — that is
the finding worth the loop's attention, and it is a render item, not a guide item.

---

## 1. Game by game

### 130 vs 146 — WON, 16 to -1, turn 16 (`1788683498-...deck130-0x557614cf13c0-vs-...deck146`)
The guide executed literally. seq 16-18 Starstorm cast normally at X=3 ("kills THEIRS: Nadaar,
Selfless Paladin; YOURS: none"), seq 10/20 two Molten Rains, seq 23 Rorix Bladewing deployed the
turn it was payable, seq 24/27/29 three straight alpha strikes with Rorix + Blastminer. 30
decisions, no re-asks. This is the deck's script and it works.

### 130 vs 123 — LOST, -78 to 10, turn 19 (`1788683485-...deck130-0x55e166da30d0-vs-...deck123`)
Deciding decision: **seq 44 -> 45, turn 16.** The cast menu offered Starstorm and priced it:
`{X pricing: max affordable X=3 ... kills THEIRS: Bloodline Keeper; YOURS: Siege-Gang Commander,
Goblin}`. The model took the Starstorm row at seq 44, then at the mode window seq 45 chose row 2
`cycling` and wrote "Starstorm would kill my own Siege-Gang Commander and Goblin for no gain."
Bloodline Keeper lived. Deck123 equipped Lightning Greaves, transformed it into Lord of Lineage
and, three turns later, attacked with 40+ Vampire tokens (opponent seat seq 45); deck130 went
15 -> -78 in one combat.
The guide forbids exactly this (#1 RULE: "do not cycle Starstorm while the 'Opponent battlefield'
line names ANY creature"). What argued the other way was the engine's own bracket on seq 44:
`[<- board sweep: THEIRS 1 / YOURS 2 - the only row on this menu that prices a board sweep (it
takes more of YOURS than of THEIRS)]`. See HIGH-3.
Secondary: seq 47, the model's PLAN prose re-derived "I MUST kill BK ... cast Spark Spray on BK
NOW ... then sacrifice Goblin" but its coded first line was `CHOICE: 4 (... targeting the
opponent)`; +9929 bytes of that reasoning were trimmed from the record (LOW-1).

### 130 vs 126 — LOST, 0 to 35, turn 19 (`1788683489-...deck130-0x55f9eb1af480-vs-...deck126`)
Unwinnable, and played correctly. Sanguine Bond resolved turn 5, Exquisite Blood turn 11
(verified against primitives mtg.txt:99004 and mtg.txt:37966 — both match Oracle). With both
halves live, any life deck130 loses re-enters the chain; deck130's 60 has no enchantment
removal, so the game was decided when the second enchantment resolved, exactly as the guide's
LIFE LOOP section says. The model recognised it at seq 20 ("Opponent has Sanguine Bond +
Exquisite Blood (life loop)") and held.
Not variance, but not guide either: deck130 was mana-locked out of a body. At seq 20 (t12) it had
3 sources and needed 5; at seq 24 (t14) it had 4 and ramped with Talisman of Impulse; by seq 32
(t18) it had 6 sources but 2 of them were Talismans, so every payable creature row auto-tapped a
source that deals it 1 damage — lethal under the loop. The ramp card that got it to six is the
card that made six unusable. Recorded as a watch item, not an edit (below).
The last 19 windows (seqs 37-55) are the loop resolving one point at a time, one full LLM
decision billed per iteration. See HIGH-2.

### 130 vs 152 — LOST, -2 to 25, turn 17 (`1788683504-...deck130-0x55f9f9567bf0-vs-...deck152`)
Mulligan to 6 (seq 2-4). Best decision of the game: seq 72-74, Starstorm X=3 for a clean 3-for-0
(Brutal Cathar, Luminarch Aspirant, Intrepid Adversary).
Deciding misplay: **seq 77-79, turn 12.** The cast row read
`Cast Spark Spray {r} ... {kills 0 of the 1 CREATURE target at 1 damage - and 1 to the opponent
at life 25 leaves them at 24}` and the only creature target was Brutal Cathar (2/2). The model
answered `CHOICE: 1 (Cast Spark Spray)` with `PLAN: Kill Brutal Cathar with Spark Spray.` — a
flat contradiction of the number on the row it took. Cathar lived, transformed into Moonrage
Brute (3/3 first strike) at turn 15 and finished the game. Guide #1 RULE already says to cycle
Spark Spray unless a target row reads "DIES". PERCEPTION failure by the model; but note the mode
window at seq 78 re-offered "Cast Card Normally" with **no kill arithmetic at all** (MED-2).
Also 11 consecutive no-op `Cast nothing right now` asks in one phase, seqs 14-24 (MED-3).

### 130 vs 162 — LOST, 0 to 6, turn 15 (`1788683524-...deck130-0x5586489001f0-vs-...deck162`)
The closest game of the wave — deck130 alpha-struck for 12 at seq 115 (t14) and died two points
short. Deciding decision: **seq 61, turn 10.** See HIGH-1: the reply's first coded line was
`CHOICE: 3 (Cast nothing right now)`; the model then wrote "Re-evaluating:" and a second coded
line `CHOICE: 1 (Cast Spark Spray)` to kill **Ob Nixilis, the Hate-Twisted at 1 loyalty**. The
first-wins rule discarded it. Ob Nixilis then pinged deck130 for every draw across turns 10-14
(life ticks visible at seqs 52, 57, 71, 86, 94, 107, 108, 111, 113) in a game lost at 0 against 6.
Also: `stale_echo` at seq 62 with a clean `recovery` at seq 63 (LOW-3); land-light (no land drop
turn 8), which is real variance.

### 130 vs 125 — LOST, -12 to 38, turn 46 (`1788683487-...deck130-0x5620e0dc4cc0-vs-...deck125`)
Matchup lock. Rorix connected once (seq 28, 19 -> 13); deck130 got the opponent to 7 by turn 18
and then lost every spell to a counterspell: Siege-Gang -> Cancel (t18), Lay Waste -> Fall of the
Gavel +5 life (t20), Molten Rain -> Fall of the Gavel +5 life (t22), Pyrite Spellbomb -> Cancel
(t30), plus Elixir of Immortality (+5, t29) and Staff of Nin pinging 1/turn for 25 turns. The
guide's counter-bait rule (line 478: "cast the CHEAP one FIRST") could not apply — hand size was
**1 card** at both seq 49 and seq 52. No `attackers` window was offered after seq 28 because
deck130 never again had a live creature. Nothing here is a guide defect.

---

## 2. Engine / interface / card items

### HIGH-1 — first-wins discards an explicitly self-corrected better answer
Repro: `1788683524-ai_baka_deck130-0x5586489001f0-vs-ai_baka_deck162.jsonl` seq 61, turn 10,
`parse_note: later_answer_ignored;latched_row_mismatch`.
Rendered row 1: `Cast Spark Spray {r} ... legal targets right now: ... Ob Nixilis, the
Hate-Twisted {3}{b}{b} (planeswalker) ...`. Reply, verbatim shape:
```
CHOICE: 3 (Cast nothing right now)
PLAN: ... I should cast Spark Spray.

Re-evaluating:
Ob Nixilis has 1 loyalty.  Spark Spray deals 1 damage. ... Ob Nixilis dies (0 loyalty).
This removes the draw punisher.  This is a huge advantage.

CHOICE: 1 (Cast Spark Spray)
```
Chosen: row 3. **This directly answers the lane-flagged AO question.** Census over deck130's 458
records: 13 records carry `later_answer_ignored` or `echo_index_conflict`; only **1 of 13**
actually contained a second coded CHOICE line, and in that one the later line was strictly
better and plausibly decided a game lost 0-to-6. So the first-wins rule cost deck130 1 decision
in 458 (0.2%) — but that one was game-deciding, and its marker is the plain-English header
"Re-evaluating:", which AP's marked-CORRECTION detector does not accept.
Suggested seam (engine seat's call, not mine): either widen the correction marker to a
self-labelled re-derivation header immediately preceding a second coded line, or make
kReplyProtocol's first-wins statement loud enough at the *front* of the reply that the model
stops emitting exploratory coded lines at all. The current text states the rule; this model
does not obey it.

### HIGH-2 — a life LOOP bills one full LLM decision per iteration
Repro: `1788683489-ai_baka_deck130-0x55f9eb1af480-vs-ai_baka_deck126.jsonl` seqs 37-55, turn 19.
19 consecutive `ask` records, life stepping 19,18,17,...,1 one per record, every one answered
`Cast nothing right now`. The rendered log at seq 55 shows 19 repeats of
`- Opponent gained 1 life (now NN)` / `- You lost 1 life (now NN)`, and the stack shows
`1 (top): ability: Sanguine Bond's Life Loss ... 2: ability: Exquisite Blood's Life`.
The verdict narration is correct and even useful (`ON THE STACK: 1 damage to you - you would be
at 0; that would KILL you`), so this is a **cost** defect, not a truth defect: nothing detects
that the chain is unbounded and the responder has no legal answer, so each point of a 19-point
(and, in 162v123, a 1089-point) loop pays for a model round-trip. Same signature the brief flags
for 162v123. A "both halves in play + responder holds no instant that changes the chain" gate
that auto-passes the remainder would remove the entire class.

### HIGH-3 — the board-sweep bracket prices bodies, not threats, and overrode the guide
Repro: `1788683485-ai_baka_deck130-0x55e166da30d0-vs-ai_baka_deck123.jsonl` seq 44, turn 16.
Rendered, on the Starstorm row:
```
[<- best X for this cast: X=3 - the only X that kills anything of THEIRS costs you more of
YOURS than it takes - THEIRS 1 / YOURS 2]
... [<- board sweep: THEIRS 1 / YOURS 2 - the only row on this menu that prices a board sweep
(it takes more of YOURS than of THEIRS)]
```
Both brackets are head-counts. The one body on THEIRS was Bloodline Keeper, whose own
`{target text: ...}` printed two lines above on the same row reads `{T}: Put a 2/2 black Vampire
creature token with flying onto the battlefield`, and the opponent also controlled Intruder
Alarm (`Whenever a creature enters, untap all creatures`) — i.e. the THEIRS-1 body was an
unbounded token engine and the YOURS-2 bodies were a 2/2 and a 1/1. The model quoted the ratio
back ("would kill my own Siege-Gang Commander and Goblin for no gain") and cycled at seq 45; the
engine it declined to kill produced the 40+ attacker swing that ended the game at -78.
A 1-for-2 head-count verdict rendered as an arrow-marked recommendation, with no weighting for a
target that generates bodies, is narration that argues against the guide's explicit rule. Either
drop the recommendation arrow and leave the raw counts, or let a target whose own printed text
creates permanents suppress the "takes more of YOURS" framing.

### MED-1 — the life-LOOP converter clause gives no per-row payment verdict
Repro: same file as HIGH-2, seq 32, turn 18. Every creature row carried the identical clause:
`{their converter: this body has toughness N and they control 1 life-to-damage converter
(Sanguine Bond) - and BOTH halves of their life LOOP are in play, so NO figure is given for what
this body costs you ... ANY nonzero payment here is fatal, whatever your life total is}`.
The rows already compute their own tap set (`{leaves 4 of your 6 untapped mana sources
untapped}`) and the frame already names the damaging sources (`CAUTION - ... Talisman of
Impulse: 1 damage` x2), so the engine can answer per row whether *that* row's auto-tap touches a
Talisman — row 3 (Dwarven Blastminer, 2 of 6 sources) and row 4 (Spark Spray, 1 of 6) could be
paid off Mountains alone; rows 1 and 2 could not. Instead all four got the same undifferentiated
"ANY nonzero payment is fatal" and the model concluded, verbatim, "No safe play exists" and
passed with six castable cards. The clause is not false; it is unresolved where the row already
holds the facts to resolve it.

### MED-2 — the mode menu drops the cast row's kill arithmetic
Repro: `1788683504-...deck130-0x55f9f9567bf0-vs-...deck152.jsonl` seq 77 then seq 78.
seq 77 row 1: `... {kills 0 of the 1 CREATURE target at 1 damage ...}`.
seq 78 row 1: `Cast Card Normally [cost: {r}] {card text: "Spark Spray deals 1 damage to any
target."} {the alternative this replaces: cycling - draws 1 card}` — no kill count, no target
arithmetic. The model's PLAN said "Kill Brutal Cathar with Spark Spray" at both windows; the
mode window was the last screen that could have contradicted it and carried nothing to
contradict it with. Since lane AL now routes modal pricing through the subject's script
snapshot, carrying the parent cast row's `{kills ...}` clause onto the "Cast Card Normally" row
looks cheap and would have caught this.

### MED-3 — no-op window churn
Repro: `...vs-deck152` seqs 14-24 (11 consecutive `ask`, all `Cast nothing right now`, turn 5,
life unchanged 20/20) and seqs 38-69 (~30 alternating ask/priority at turn 9 with no board
change); `...vs-deck162` seqs 22-32 (11 consecutive, turn 7, 20/20). `hold_windows_skipped`
across the six seats: 0, 1, 110, 14, 27, 81 — so the hold row is doing real work where it is
taken, and these runs are windows where it was **not** taken and the seam re-opened anyway.
This is the measurable side of the lane-flagged G7 re-offer churn from deck130's slice: the
churn is concentrated in the model declining to take the hold row, not in the re-opener firing
wrongly. Reported as data for the engine seat rather than as a defect claim.

### LOW-1 — unbounded post-PLAN generation
`...vs-deck123` seq 47 records `[+9929 bytes written past the PLAN line trimmed from this
record]`. The content is a full re-derivation that reached the opposite (better) conclusion and
was discarded. Cost is latency and tokens on every such window; it also means the record cannot
be audited for what the model actually decided last.

### LOW-2 — `echo_index_conflict;name_over_index` on same-shape rows
`...vs-deck125` seqs 97 and 98, turn 46, a mana-tap menu whose rows are `Mountain #1 [land]
[your battlefield] - "R"`, `Mountain #2 ...`, `Mountain #3 ...`. Resolved correctly by name; no
harm, noted only because the conflict is structural to identical-shape rows.

### LOW-3 — `stale_echo` + clean recovery
`...vs-deck162` seq 62 (`fallback: stale_echo`, `parse_note: stale_echo_in_range`,
`deadline_pct 0.3`, `choice -1`) followed by `recovery` seq 63 (`recovers_seq: 62`). The
recovery seam worked. 1 event in 458 records.

---

## 3. Guide verdict — **KEEP as is**

`projects/mtg/bin/Res/ai/baka/deck130_strategy.txt` (71,227 bytes, inside the 41-71 KB band).
No edit is paid for by this corpus:

- The win vs 146 is the guide's own script run top to bottom (sweep -> land kill -> Rorix ->
  attack every turn), with 0 re-asks.
- Both model-side misplays this wave were **violations of rules the guide already states**, not
  gaps in it. seq 45 (123) broke "#1 RULE ... do not cycle Starstorm while the 'Opponent
  battlefield' line names ANY creature"; seq 77 (152) broke "#1 RULE ... Spark Spray -> cycle it
  when its cast option's 'legal targets right now:' list shows no opponent creature whose row
  reads 'takes 1 damage - DIES'". Restating a rule the model read and overrode adds bytes and
  changes nothing; in both cases what actually pushed the other way was an engine bracket
  (HIGH-3, MED-2), which is where the fix belongs.
- The 126 loss was played *in compliance with* the guide's LIFE LOOP section and was unwinnable
  by that section's own admission ("you were dead when the second enchantment resolved. Nothing
  in this sixty removes an enchantment"). Verified: deck130.txt contains no enchantment removal.
- 125 and 152 were a counterspell wall against a 1-card hand and a mulligan to 6. Real hands.
- 1/6 after 3/6 over six games against these six matchups is inside variance; no seq in this
  slice shows the guide instructing an action that lost a game.

**Watch item recorded, deliberately NOT edited.** The guide gates Talisman self-damage on a life
floor ("at 3 life or less -> nothing that needs a Talisman to pay for it", lines 743 and 765),
but under a live Sanguine Bond + Exquisite Blood loop the floor is *any* life total — deck130
sat at 20 life at seq 32 (126) with Talisman-dependent rows that were lethal. It cost nothing
this wave because the rendered row said so and the model obeyed the row, so no seq pays for the
edit. If a future corpus shows the model reaching for its own numeric floor over the row's
sentence, that is the window that pays for it.

No `general-proposals.md` and no `skill-proposals.md`: every finding above lands on the engine's
render, not on the general guide or the strategy-writing skill.

---

## 4. What I did NOT check

- I did not read the opponent seat files in full for 125, 152, 162 or 146 — only 123's tail (to
  confirm the -78 swing was the Bloodline Keeper / Intruder Alarm army, not an engine loop) and
  126's decklist and log excerpt.
- I did not verify Bloodline Keeper, Intruder Alarm, Starstorm, Brutal Cathar, Ob Nixilis or
  Staff of Nin against their primitives or against Scryfall; I verified only Sanguine Bond,
  Exquisite Blood and Rorix Bladewing (all three match Oracle). Card text quoted elsewhere in
  this file is quoted from the rendered prompt, not confirmed against `primitives/`.
- I did not read the `.stderr` files, the wave-65 lane files, `known-bugs.md` or
  `codex-review.md` — my classification of items as new vs already-lane-covered may therefore be
  wrong at the margins, and HIGH-1/HIGH-2 in particular are offered to the engine seat, which
  holds the lane predictions.
- I did not adjudicate any lane prediction (that is the engine seat's deliverable).
- `hold_windows_skipped` and the churn counts in MED-3 are read straight from the record fields
  and from consecutive-seq runs; I did not prefix-match HOLD takes against the rendered rows, so
  MED-3 is a magnitude, not a census.
