# Wave 71 — deck 146 (Orzhov dungeon midrange) per-deck review

Seat files: `matchups-20260907-085638/*ai_baka_deck146-0x*-vs-*.jsonl` (6 games). Census for this
seat: 431 records / **413 model decisions**, **0 fallbacks**, 0 `reply_truncated`, 0
`action_before_plan`, 0 `menu_pass_no_progress`, 0 `ask_replays_refused`, 2 `async_drops`.
Reasoning present on every decision (`reasoning_chars` min 1,229 / median 5,472 / max 18,640; zero
records at 0). Latency p50 73.6 s, p95 165.0 s, max 402.0 s; 9.8 h of model wall time for the six
games. `off_protocol_bytes>0` on 34 records, `plan_line_missing` on **39 (9.4 %)** — this one seat
holds 43 % of the corpus's 90.

| # | Opponent | Result | Turn | Final life | Wave-70 result vs same deck |
|---|---|---|---|---|---|
| 1 | 126 | WIN | 27 | 17 / -2 | WIN |
| 2 | 123 | WIN | 21 | 21 / 0 | WIN |
| 3 | 125 | **WIN** | 35 | 13 / -3 | **LOSS** |
| 4 | 162 | WIN | 15 | 16 / -4 | WIN |
| 5 | 152 | **LOSS** | 16 | 0 / 19 | WIN |
| 6 | 130 | WIN | 19 | 12 / -4 | WIN |

5-1 again, the corpus's best record.

## 0. Did the wave-70 edits pay off? YES, on the game they were written for

Wave-70 edit 1 added "A Hive you already controlled as the turn began IS NOT SUMMONING SICK ... it
ATTACKS THIS TURN", paid for by the vs-125 loss where the model declined a payable animation on a
self-invented haste rule and attacked with nothing. In wave 71 the same matchup is a **WIN at turn
35**, and the Hive animates and attacks four times: `...vs-ai_baka_deck125.jsonl` seq **65** (T27),
**98** (T29), **150** (T33), **171** (T35, the lethal swing "Attack with Hive and Goblin to win").
No reasoning trace in this seat repeats the haste error. Edit 2 (affordability) is now also carried
by the engine itself (see §2, fixed items), so I have folded the guide's paragraph down to one line.

The two other wave-70 MED items are **FIXED in the engine**:
- the modal-spell "already resolved" narration contradiction now ships an explicit disclaimer on
  the ask header (`Choose an option for Silverquill Command: the GAME LOG above may already say ...
  it did: the card leaves the stack as it resolves, and this choice is part of that resolution`) —
  5 occurrences this corpus, none burned reasoning;
- the manland rung ceiling is now printed on the **battlefield line** and on the option row
  (`[creature-land: ... its OWN mana is not counted toward that activation]`), i.e. at the window
  where it can change the plan.

## 1. Game by game — the deciding decisions

**vs 126 (WIN T27).** The opponent assembled Exquisite Blood + Sorin. seq **76** (T23) is the best
decision of the corpus: 8 attackers, all sent at Sorin (`ATTACK: A1>W1 ... A8>W1`) with the
reasoning naming the reason — "Exquisite Blood makes it suicide ... damage to a planeswalker does
not reduce their life total". Correct, and only possible because the render carried both the
planeswalker row and the enchantment's effect text. The one bad decision is seq **17** (§2 HIGH 2).

**vs 123 (WIN T21).** Straight curve-out. seq 30 Pelakka Predation stripped Damnation (their only
sweeper); seq 34 Vanishing Verse on Intruder Alarm (an enchantment, off the creature reflex);
Kaya +1 twice on the lone Silencer (seq 38, 41) kept the clock alive; lethal seq 55.

**vs 125 (WIN T35).** Creatureless permission-control, the wave-70 loss. Three Soul Shatters again
sat dead in hand correctly ("at 0 this does nothing"). The Hive line (above) is what changed the
result. Cost: 159 records / 35 turns — the most expensive game of the corpus (§2 MED 5).

**vs 162 (WIN T15).** Clean, but contains the legend-rule defect: seq **19** (§2 MED 3).

**vs 152 (the LOSS, T16).** Bant midrange. Deck 146 was 20/20 at T7 and 7/20 by T13; the swing was
turn 12, when Brutal Cathar exiled its only Nadaar and Elite Spellbinder exiled Lolth **from hand**
while both its creatures were tapped from the T11 attack (seq 20). No blockers ask was issued that
turn because nothing was untapped — the engine was right. Mostly matchup and tempo, not guide.
The one decision I would call a misplay is seq **27** (T13, `ATTACK: none`): the render offered a
2/2 first-strike deathtouch Adventurer whose two possible blockers BOTH read `(you kill it, your
attacker lives)`, i.e. the attack was free upside — either 2 damage or a blocker removed from an
11-point crack-back. The reasoning ran only the guide's lethal test ("They are at 20 ... Not lethal.
So ATTACK: none") and never read the two kill-tags. This is arguably covered already by the guide's
CHECK A (printed life 8 or less — not in force at 20) and by "LETHAL OVERRIDES"; I am **not**
proposing a rule for it. One decision in one lost game, and a blanket "attack whenever your tags
say the blocker dies" would fight the converter/lifelink stops that this guide has needed. Flagged
here for the record, not edited.

**vs 130 (WIN T19).** Barrowin carried it (seq 21 Silverquill Command pump for a 6-power swing,
seq 25 Kaya -3 on Siege-Gang Commander). Contains the HIGH 1 defect at seq 30.

## 2. Engine / interface / card items

### HIGH 1 — a cast-trigger NAME list offers TOKEN names, and the model took one
`1788789434-ai_baka_deck146-0x55df18eac130-vs-ai_baka_deck130.jsonl`, **seq 30**, T17. Rendered
list head:
```
Choose a card NAME for Silverquill Silencer - ... Whenever an opponent casts a spell with the
chosen name, they lose 3 life and you draw a card.
1. Goblin {visible now: 2 on their battlefield}
2. Talisman of Impulse {visible now: 1 on their battlefield}
...
e.g. "CHOICE: 1 (Goblin)"
```
It chose row 1. **Deck 130 contains no card named "Goblin"** (verified against
`bin/Res/ai/baka/deck130.txt`: Dwarven Blastminer, Rorix Bladewing, Siege-Gang Commander, Hammer of
Bogardan, Lay Waste, Molten Rain, Pyrite Spellbomb, Spark Spray, Starstorm, Stone Rain, Talisman of
Impulse, lands) — "Goblin" is a **token** made by Siege-Gang Commander. A token is never cast, so
the Silencer's trigger (`auto=chooseanameopp ... @movedto(*[chosenname]|opponentstack)`,
`borderline.txt:102765`) can never fire again. The decision seam was made worthless by a row that
cannot be right. Fix: exclude token names from the `chooseanameopp` candidate list, or tag them
`[token - never cast; this name can never trigger]`. Repro: any game where a Silencer enters while
the opponent controls tokens.

### HIGH 2 — the same list sorts by `{visible now: ...}`, which is anti-correlated with castability
`...vs-ai_baka_deck126.jsonl`, **seq 17**, T11. Rows 1-4 are the visible ones, unmarked rows follow.
Row 1 was `Sanguine Bond {visible now: 1 on their battlefield}` and it was chosen. For a
*cast* trigger, "already on their battlefield" is the strongest evidence a name is spent, not the
strongest evidence it matters. The reasoning is explicit that the visible tag drove it ("Opponent
has Sanguine Bond on the battlefield ... I should choose Sanguine Bond"), and the prompt's own
convention — usually-correct option first — endorses the ordering. The deciding number is not
rendered anywhere: **copies of that name not yet in a public zone** (deck 126 runs 4x Tribute to
Hunger, the card that actually threatens this deck, and it sat unmarked at row 8). Fix: print the
remaining-copy count and sort by it, or at minimum tag battlefield-visible rows
`[already on the battlefield - only another copy could ever be cast]`.

### MED 3 — the legend-rule choice wears a false header
`...vs-ai_baka_deck162.jsonl`, **seq 19** (T11) and **seq 26** (T13). Rendered:
```
TARGET CHOICE for Nadaar, Selfless Paladin - its "put a card into the graveyard" ability
(this spell/ability is already on the stack and needs a target - it is NOT a cast or phase step).
Pick the ONE target it will affect from the list below ... LEGEND RULE (CR 704.5j): ... the one you
pick on this list is the one that GOES TO ITS OWNER'S GRAVEYARD now
```
Nadaar has no "put a card into the graveyard" ability (`borderline.txt`, Nadaar = vigilance +
venture on enter/attack), and the legend rule is a state-based action, not a targeted ability on
the stack. The header and the clause under it say opposite things about what the pick means. The
model spent roughly a third of its reasoning on it ("venturing doesn't target Nadaar ... maybe it's
a bug in the prompt's description") and then treated the list as *which copy to use*: at seq 19 it
picked row 1, the copy with **no** restriction tag, and kept the row-2 copy tagged
`[summoning sick]` — it binned the Nadaar that could attack that turn. Fix: give the legend-rule
chooser its own header instead of borrowing the generic ability-target one.

### MED 4 — `[castable now]` is stamped on hand cards at windows where those cards are not offered
`...vs-ai_baka_deck125.jsonl`, **seq 150**, T33 main 1. The hand line reads
`Lolth, Spider Queen {3}{b}{b} [planeswalker] [castable now]`, and the legal-action list contains
only the Lolth loyalty ability, the Hive animation and Hold. The truthful explanation is present
further down (`You have already answered this phase's Casting decision (a card you did not cast
there is not re-offered below)`), but the model reasoned to "this is a contradiction or a specific
UI quirk" before finding it. **27 of the 47 casting-already-answered windows in this seat still
carry `[castable now]`.** Fix: at those windows tag the card
`[castable this turn, but not offered here - you already answered this phase's casting decision]`.

### MED 5 — the inference budget of the new own-turn windows
413 model decisions for six games (wave 70: 329 for six), 9.8 h of model wall time, and **135 of
them (32.7 %) resolved to a no-op** (`Cast nothing right now` / `Hold priority` / `pass` /
`Play no land right now`) — 97 at `ask`, 38 at `priority`. The single most expensive turn was
`...vs-ai_baka_deck125.jsonl` turn 27 at **34 decisions**. The suppression machinery is working
(gameend `hold_windows_skipped` 130 / 29 / 27 in the three long games, and 0 consecutive windows in
the whole seat offered an identical option set), so this is the residual cost of lane BQ's own-turn
instant windows, not a dedupe bug. Reporting it as a budget fact for the engine seat: a third of
this deck's inference bought nothing.

### MED 6 — the format example is written from the LIVE row 1 and reads as an endorsement
Both HIGH cases above end their reasoning by quoting the example back: `e.g. "CHOICE: 1 (Sanguine
Bond)"`, `e.g. "CHOICE: 1 (Goblin)"`. Across the 284 asks in this seat that carry
`written out from row 1 of this list`, row 1 was chosen 120 times (42 %) — that number alone proves
nothing, since row 1 is deliberately the usually-correct option, and I am **not** claiming a
measured effect. What is specific is that the NAME-choice ask is the one list where row 1 is
systematically wrong (§HIGH 2), and there the example names the wrong card in the model's own
words. Cheapest fix: on name-choice asks use the placeholder form the cast menu already uses
(`"CHOICE: 3 (Cast Example Card)" (a placeholder - copy a real number and short name from the
list)`).

### LOW 7 — opponent spells narrate no target
`...vs-ai_baka_deck123.jsonl`, **seq 18**, T7. The log reads:
```
- Opponent cast Tragic Slip
- Opponent's Tragic Slip resolved and went to the opponent's graveyard
- Your Goblin died
```
Opponent *abilities* narrate targets (`Opponent used: exile a creature with Brutal Cathar targeting
Nadaar, Selfless Paladin`), but only 3 of 48 opponent spell casts in this seat's logs carry a
`targeting` line. Here the model wrote a long block asking whether Nadaar had been hit and whether
the log was wrong, because the resolve line sits between the cast and the effect. Fix: emit the
same `targeting <name>` clause for opponent spells.

### Corpus-question resolved: the `askreplay/` sidecar is NOT missing, the HARVEST is
The brief asks whether zero re-serves happened or the path is wrong. **Neither**: the sidecar is
written to `~/.Wagic/ai/gpt/logs/askreplay/` (29 files, one per seat that had one) and the
harness's end-of-run harvest simply does not copy that directory into the run dir. This seat's 39
records match its gameend counters exactly (`ask_replays_reserved` 3 + 1 + 32 + 0 + 2 + 1 = 39),
every one `why: cache_replay`, 0 refused, each carrying `replayed_from`/`seq`. Engine seat should
adjudicate lane BP/BS from `logs/askreplay/`, not from the run dir.

### Reply-shape census (39 `plan_line_missing`, 34 `off_protocol_bytes>0`)
Three shapes, none of which cost a decision (every action line parsed, no fallback):
1. **30 records** — the plan sentence is present but the literal `PLAN:` label is dropped
   (`Attack with Barrowin.` / `ATTACK: Barrowin of Clan Undurr`). Same shape as wave 70.
2. **4 records** — the model uses the prompt's OTHER label: `YOUR PLAN: ...`
   (deck125 seq 78, 107, 151) or `Write your PLAN: ...` (deck123 seq 23, 294 off-protocol bytes).
   The prompt names the plan twice with different wording — the recap header
   `YOUR PLAN (as you last stated it ...)` and the instruction `Write your PLAN: line first` — and
   the model copies whichever it saw last. A header rename (e.g. `LAST PLAN (as you stated it ...)`)
   removes the collision without touching the protocol or the parser.
3. **5 records** — no plan line at all, only the action line (deck126 seq 53, 68, 69, 76; deck125
   seq 53). seq 76 is the Sorin alpha-strike: the reasoning shows the model reading
   "Write nothing else" and concluding "I will just write the ATTACK line as requested". The
   "Write nothing else" clause and the "PLAN line first" clause read as competing instructions to
   this model.

## 3. Guide verdict: EDIT

Live guide 19,927 bytes -> revised **19,992 bytes** (under the 20,000 ceiling).
`wave71/deck146/strategy.txt`. Four edits; two add bytes, two pay for them.

**Edit 1 — REWRITE the SILVERQUILL SILENCER rule (the bytes this rule earned).**
- before: `- SILVERQUILL SILENCER ({W}{B}): the name is a bet on a card they have NOT cast: (1) one Pelakka Predation showed you, or the missing half of a pair on their line (Sanguine Bond -> name Exquisite Blood); (2) a card their deck replays - a counterspell the log prints as "was COUNTERED by <name>", a punisher enchantment, a token maker, a lord; (3) never a card on their line or a defender.`
- after: a mechanical read of the rendered rows — the trigger fires only on a CAST, so
  `{visible now: ...}` is a MINUS not a plus and the list prints those rows FIRST; take (1) an
  UNMARKED row for the card their decklist holds most copies of, counterspells and sweepers ahead
  of bodies, or (2) a graveyard-visible row whose decklist count is above N; **never a name their
  decklist does not hold — that is a TOKEN ("Goblin"), a token is never cast, and the Silencer is
  dead for the rest of the game**; never a name whose only copies are on their battlefield.
- paid for by: **deck130 seq 30** (named the token "Goblin"; Silencer dead for the game) and
  **deck126 seq 17** (named the battlefield copy of Sanguine Bond over the unmarked half the old
  rule already prescribed). The old rule's clause (3) "never a card on their line" was already
  correct and was still violated, because it was a prohibition with no positive test and it
  collided with the SANGUINE BOND block (Edit 3). The new form is a row-reading procedure, and it
  is the only place in the guide that can catch a token name.

**Edit 2 — SHRINK the HIVE affordability paragraph (frees the bytes).**
- before: `AFFORDABILITY: its own mana cannot pay for it, so read "Mana available: N" as N MINUS ONE here (minus one more for a second Hive you also mean to attack with) - FOUR OTHER untapped sources or the row is not offered at all, and a plan naming an animation you cannot pay for is a turn spent doing nothing.`
- after: `AFFORDABILITY: FOUR OTHER untapped sources - its own mana never pays for it.`
- paid for by: the engine now prints the whole rule at the two windows that need it — the
  battlefield line (`[creature-land: ... its OWN mana is not counted toward that activation -
  tapping it for mana leaves it tapped, and a tapped permanent can neither attack nor block]`) and
  the option row's `{rung ceiling: ...}` clause. Four windows this corpus wrote a payable animation
  plan and executed it (deck125 seq 65, 98, 150, 171); none of the wave-70 unpayable-plan shape
  recurred. Keeping the long form would be the guide restating a true surface.

**Edit 3 — SCOPE the SANGUINE BOND block to removal lists.**
- before: `Either name on their line, on Verse's target list, or on Kaya's -3 list -> take it, above any creature.`
- after: `Either name on Verse's target list or on Kaya's -3 list -> take it, above any creature.
  That is a REMOVAL rule and never a Silencer name.`
- paid for by: **deck126 seq 17** — the reasoning quotes this block verbatim ("The guide says:
  'SANGUINE BOND AND EXQUISITE BLOOD. Either name on their line ... -> take it, above any
  creature'") and applies it to the NAME list. "on their line" made a removal rule read as a
  universal one, and it outranked the Silencer rule on the same screen.

**Edit 4 — REPLACE the WORKED SITUATIONS bullet with a legend-rule rule (the bytes this rule earned).**
- cut: the Shield-Sphere / Pride-Guardian worked situation — it composes two combat rules already
  stated verbatim ("your attacker dies, their blocker lives" -> stays home; "(blocking trigger:
  they gain N)" -> that swing pays them) and adds no discrimination either lacks.
- add: `TWO COPIES OF ONE LEGEND. A list offering two permanents with the SAME name is the legend
  rule, not a target choice, whatever header it wears: the row you PICK is the one that DIES. Pick
  the copy tagged "[summoning sick" or carrying fewer counters - an untagged copy can attack this
  turn.`
- paid for by: **deck162 seq 19** (kept the summoning-sick Nadaar, binned the one that could
  attack) and seq 26. Deck 146 runs 4x Nadaar, 3x Acererak, 3x Lolth, 2x Kaya, 2x Barrowin — this
  ask is a recurring seam for this deck, and MED 3 says the header will keep pointing the wrong way
  until the engine is fixed. This is the route-around form the trust doctrine allows: a decision
  rule that does not depend on the broken header, and it retires the moment the header is fixed.

Also folded into Edit 1: the redundant `- Silencer names the counterspell the log prints as "was
COUNTERED by <name>", not a creature.` line in the WHEN-THEY-COUNTER block (now covered by
"counterspells and sweepers ahead of bodies"), and the `[repeat: ...]` sentence in the HIVE block
compressed to one clause (CHECK 2 already carries the same test; the tag rendered 0 times this
corpus, so I compressed rather than cut it).

No `general-proposals.md` and no `skill-proposals.md`. HIGH 1/2, MED 3/4/6 and LOW 7 are engine and
render items for the core loop, not general-guide prose; MED 5 is a budget observation for the
engine seat. Nothing I found is a gap in wave68/strategy-writing-skill-v2.md's method — Edit 1 is
exactly the "read the rendered row, don't re-derive" discipline the skill already teaches, applied
to a list whose ordering was misleading.

## 4. What I did NOT check
- The opponents' seat files beyond board context; I did not audit any opponent deck's play.
- The 2 `async_drops` (deck123, deck125) — not traced to a seam.
- Mulligan and bottoming: all six games were untouched-7 keeps again, so the MULLIGAN and BOTTOM
  blocks got zero exercise and I make no claim about them.
- Blocking from this seat: only 5 `blockers` decisions across six games (deck126 1, deck152 3,
  deck130 1); the BLOCK LADDER is effectively untested for a second wave running.
- The DRAW-PRICE BRACKET block and CHECK 0b: no window this corpus rendered either trigger.
- Whether the 42 % row-1 rate in MED 6 is causal — it is not powered to say so, and I did not
  design a test for it.
- Corpus-wide census, lane BO/BP/BQ/BR/BS prediction adjudication, and the 4 corpus fallbacks
  (none in this seat) — engine seat.
