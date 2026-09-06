# Deck 152 (Bant Human midrange) — wave-68 review

Corpus: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-134120/`, six seat files
`*-ai_baka_deck152-*-vs-*`. Record 5-1 (only loss: vs deck146). 265 records on this seat, 246 with a
rendered `prompt`. Guide read: the live `projects/mtg/bin/Res/ai/baka/deck152_strategy.txt`
(19,974 B, rewritten this wave; `wave68/rewrite/deck152/changes.md` (b) lists the drops).

## 1. Game by game

| opp | result | turn | deciding decisions |
|---|---|---|---|
| 125 | WIN 12-0 | 31 | Sigarda on curve (seq 8) then an unblockable Wolf clock; Katilda counters grew the Wolves 3/3 -> 7/7 (seq 49/57/70/80). No misplay changed the result. |
| 123 | WIN 27/-3 | 19 | Curve-out, all attacks unblocked; lethal all-in at seq 32 (`ATTACK TOTAL ... that KILLS them whatever they block`) taken correctly. |
| 126 | WIN 20/-7 | 11 | Aspirant + two Spellbinders in the air; every A-line read `[no creature they control can block this attacker]`; seq 27 lethal taken. |
| 162 | WIN 12/-8 | 15 | Correct block at seq 34 (Sigarda `(both die)` on Master of the Feast, preserving Spellbinder); lethal all-8 swing at seq 60. One `reply_truncated_reask` (seq 46) cost a round, not a decision. |
| 130 | WIN 28/-1 | 13 | Tracker + Adversary, no blockers offered to them; lethal at seq 24. |
| 146 | **LOSS** -1/21 | 13 | seq 14 -> seq 15 (below). |

### The loss (file `1788720116-ai_baka_deck152-0x55c6ee04e600-vs-ai_baka_deck146.jsonl`)

Hand was real (mulligan to 6: 5 lands + 2 Sigarda; both mulligan answers follow the guide). Two
STRATEGY decisions, both against a rule the guide states or should state, decided it:

- **seq 14 (T8 attackers).** `A1. Brutal Cathar (3/3) [daybound] [their untapped blockers: Nadaar,
  Selfless Paladin (3/3) (both die); Goblin (1/1) (you kill it, your attacker lives)] {if this leaves
  the battlefield: their Silverquill Silencer (3/2) comes back from exile}`. The seat sent it.
  Nadaar blocked, both died, and the exiled Silverquill Silencer came back — a two-for-one against
  the seat. The live guide only said "never throw the Cathar into a bad block"; the tag rules read
  `(both die)` as sendable, and nothing priced the refund. PERCEPTION was fine: the refund clause was
  printed on the row it answered. STRATEGY item.
- **seq 15 (T9 blockers), the deciding one.** `B1. Sigarda, Champion of Light (4/4) [trample, flying]
  - may block A1 (your blocker dies, attacker lives), A2 (you kill it, your blocker lives), A3 (both
  die)`. Header: `you would be at 9 ... best case with every blocker assigned: you would be at 13 ...
  chosen for the LIFE ONLY - it does not preserve your material`. The seat answered `BLOCKS: B1:A3`
  ("remove their best threat"), spending Sigarda — its only anthem and its only flier — to kill the
  returned 4/3, and took 4 instead of 2. Board was empty at seq 16; dead at T13. The guide's ladder
  ranks the rungs but never says the ladder also chooses WHICH attacker one blocker blocks: rung 1
  (`you kill it, your blocker lives`) was on the same row and was passed over.
- Same shape recurs at **seq 19**: `B1. Elite Spellbinder (3/1)` offered A2 `(both die)` and A3
  `(your blocker dies, attacker lives)`; the seat again took the biggest attacker (A3, rung 3) over
  the better rung. 2 of 2 windows where one B-row listed several A-numbers were answered on the
  attacker's size, not the rung.
- seq 11 is the control: with only `(your blocker dies, attacker lives)` on the Cathar's row and
  `you would be at 17`, the seat correctly answered `BLOCKS: none` — the N>=10 branch works.

## 2. Engine / interface / card items

**MED — the blockers header names a material-losing assignment and the pilot takes it.**
File `...deck152-...-vs-ai_baka_deck146.jsonl`, seq 15 (again seq 19). Rendered:
`best case with every blocker assigned: you would be at 13 - one legal assignment that reaches it,
chosen for the LIFE ONLY - it does not preserve your material and no better material reaches that
life figure among LONE blocks: Sigarda, Champion of Light blocks Silverquill Silencer`. Both times
the seat answered exactly the named assignment and both times a better rung sat on the same B-row.
The disclaimer is printed, so this is not a false surface — but the only *named* assignment in the
prompt is the one that spends material. Naming the best-material assignment beside it (or printing
the rung word for the named one) would remove the anchor. 2 of 2 windows.

**MED — a cap can eat a whole reply when the model reasons before answering.**
`...deck152-...-vs-ai_baka_deck162.jsonl` seq 46: 2,391 B of prose, no CHOICE line, `fallback:
reply_truncated_reask`, latency 121,798 ms, followed by a `recovery` record (seq 47) and a clean
answer at seq 48 (14,844 ms, `CHOICE: 2 (Hold priority)`). No decision was lost; cost was one round
and ~2 min. The seam's emit instruction ("On a line of its own write CHOICE:") does not say the
CHOICE line comes FIRST; saying so at the seam would make the cap unreachable for this failure mode.

**LOW/MED — `plan_choice_conflict` fired on think-aloud, not on a real contradiction.**
`...vs-ai_baka_deck125.jsonl` seq 47: reply `CHOICE: 1 (Cast Katilda, Dawnhart Prime)` with a PLAN
containing "Wait, we only have 4 mana total ... Actually, looking at the board ...". Refused; the
re-ask (seq 48) returned the identical choice. Detector cost a round and changed nothing. Worth a
count across the corpus at the engine seat (25 `plan_choice_conflict` corpus-wide) before tightening.

**LOW — two literals for one fact on tap-cost rows.** Katilda's activation row prints
`{tapping Katilda, Dawnhart Prime now: it cannot attack this turn}` (17 offers on this seat), while
the payment-side clause prints `{paying this taps: <names> - they cannot attack this turn}`
(`...vs-ai_baka_deck162.jsonl` seq 25 prints BOTH on one row). One guide rule cannot key to both
without quoting two literals; a single literal would halve the guide text this needs.

**LOW — X/repeat menu renders 18 unusable rows.** `...vs-ai_baka_deck146.jsonl` seq 24 (Intrepid
Adversary valor counters): rows 4-21 (`add 3 counters` ... `add 20 counters`) each read
`you have 4 spendable now, which pays for 2 of them and stops`. Only rows 1-3 are distinct answers.
Stopping the list one rung past the affordable count would cut ~1.4 KB from that prompt.

**Card facts checked this wave against `bin/Res/sets/primitives/borderline.txt`:** Brutal Cathar
(l.14662, `(blink)forsrc`, `until this creature leaves the battlefield`) — the refund the new rule
prices is real and applies to death in combat on either side. Sigarda (l.102243, `{1}{G}{W}{W}`,
Legendary, flying/trample, `lord(human|mybattlefield) 1/1`) — guide entry correct. No card-script
defect found on this deck.

## 3. Guide verdict: EDIT (small, two rules, both paid by the only loss)

Live 19,974 B -> revised **19,997 B** (`stat -c %s wave68/deck152/strategy.txt`), ceiling 20,000.
A332 grep (`corpus|wave[- ]?[0-9]|\bseq\b|previous game|last wave|this wave|census|paid (for )?by|
[0-9]{3}v[0-9]{3}|you (declined|took|cast|passed) [0-9]`) = **0**. Frequency-adjective grep = 2, both
inside the pre-existing mulligan ODDS sentence ("about as often as anything else", "rarely fixes
lands"), which the mulligan convention requires to be odds-shaped; the mechanic was exercised (8
mulligan asks this corpus). The guide grew by 23 B net; the two additions are paid for by cuts below.

### Edits

**E1 — blocking: the ladder also selects WHICH attacker one blocker blocks.** Paid by seq 15 (the
deciding decision of the only loss) and seq 19.
Before: (nothing; the ladder was a per-row take/decline test)
After, inserted between rung 3 and the Katilda B-row:
`A B-ROW LISTING SEVERAL A-NUMBERS: the ladder also picks WHICH attacker it blocks - read its own list
and take its BEST rung, 0 before 1 before 2 before 3, never the biggest attacker and never the one the
header's "best case" assignment names.`
Plus one SITUATION: `One blocker, its row listing "(you kill it, your blocker lives)" on a 2/2 and
"(both die)" on a bigger attacker -> block the 2/2; the bigger name is not the better rung.`
Prediction: window class = a blockers ask where one B-row lists two or more A-numbers with different
parentheses (2 of 5 blocker windows this corpus). Literal = the rung strings already audited.
Refuting number: the seat again answers a lower rung than one available on the same B-row in any such
window. A hit is a break only if a strictly better rung was printed on that same B-row.
Removal condition (information-supplying): delete when the render itself orders the B-row's options
by rung, or names the best-material assignment in the header.

**E2 — Brutal Cathar: its death refunds their creature, so `(both die)` is not an even trade.**
Paid by seq 14 (which produced the board that lost at seq 15); consistent with seq 11, where the seat
already declined a losing block for this reason.
Before: `ETB exiles an opponent creature until the Cathar leaves, and it comes BACK if the Cathar
dies, so never throw the Cathar into a bad block.`
After: `ETB exiles an opponent creature until the Cathar leaves. Every row it appears on then prints
"{if this leaves the battlefield: their <name> comes back from exile}": while that clause is there its
death hands them a creature back, so it is never an even trade. Send it ONLY on "(you kill it, your
attacker lives)" or with no blocker listed - "(both die)" keeps it home - and block with it only on
rung 0 or rung 1.`
Prediction: window class = any A-line or B-row carrying `{if this leaves the battlefield:`. Refuting
number: the Cathar is sent on a `(both die)` tag again, or held on `(you kill it, your attacker
lives)`. A hit is a break only if the refund clause was on the row answered.
Removal condition (belief-correcting): both the render pricing the refund inside the parenthesis AND
zero breaks across two corpora with the clause present.

### Cuts that paid for them (rungs: all "restatement" or "worked example on a rule that held")
- SITUATIONS bullet restating the LETHAL COUNT rule verbatim — the rule held at every lethal window
  this corpus (3 all-in swings, all taken). ~155 B.
- SITUATIONS bullet restating the `{paying this taps: ...}` rule — the rule held (17 offers, 13
  passes; the 4 takes were the Katilda upkeep windows discussed below, all in a won game). ~160 B.
- SITUATIONS bullet restating the Elite Spellbinder exile order verbatim from the card entry —
  no Blood/Bond/Tutor window arose (0 renders of `LIFE-TO-DAMAGE CONVERTER`), and the card entry
  already carries the order. ~155 B.
- Hold-priority paragraph: dropped "It is never worse, and any board change re-opens the window";
  the imperative and the "identical OPTION LISTS, not boards" guard stay. ~80 B.
- Mulligan M=7 line: dropped "What the spells cost is not part of this call, and neither is colour" —
  the same exclusion is already stated on the M<=5 line. ~72 B.
Trim prediction: if a lethal count, a paying-taps pass, a Spellbinder exile order or an M=7 keep goes
wrong next corpus at a window whose facts were printed, the cut situation is restored.

### Rules I checked and left alone (KEEP evidence)
- **Deploy floor: 50 casts in 51 windows that offered a creature row** (the 51st was the
  `plan_choice_conflict` refusal, re-asked and cast). No "cast nothing" with a creature on the menu.
- **Mulligan: 8 asks, 8 guide-conformant** (both 0-land sevens shipped; every 2+ land hand kept).
- **Attack tags**: no attacker was sent into a printed `your attacker dies, their blocker lives`;
  every `[no creature they control can block this attacker]` A-line was declared (23 renders).
- **`BLOCKS: none` at N>=10** (seq 11) and the N<10 chump branch both behaved.
- Zero-hit literals kept as UNTESTED (window never arose, branch reachable): `[opponent's stack]`
  ward row (0 renders — the ward ask never fired although Moonrage Brute attacked),
  `LIFE-TO-DAMAGE CONVERTER` (0), `Poison counters` (0), `tramples to your face` (0),
  `only a block by TWO OR MORE` (0), `[loyalty` (0), `repeat: activated this turn` (0).

### Two live findings I did NOT turn into edits
- **Katilda's counter ability in UPKEEP, 4 takes of 17 offers** (`...vs-ai_baka_deck125.jsonl` seq
  49/57/70/80, all T25-T31, all `{tapping Katilda, Dawnhart Prime now: it cannot attack this turn}`),
  against the guide's "MAIN 2 ONLY". Violated-unpunished — in that game the pre-combat pump added
  more damage than Katilda's own 1 power, and the seat won 12-0. Per the writing skill,
  violated-unpunished never loosens a rule, and the rule held 13 of 17 firings; but the WHY behind
  "MAIN 2 ONLY" (never lose an attacker) does not imply it when the pump is worth more than the
  attacker. Condition to revisit: a game where the upkeep take costs a lethal or a race.
- **The hold-priority row: 4 takes of 18 windows carrying `[you declined this exact list N times
  already this turn]`** (misses at 125 seq 52/54/63/65/68/75/83/85, 123 seq 31, 162 seq 26/37/49/50/51
  — all answered "Cast nothing right now" / pass instead). 22% compliance. It is a routing rule: no
  decision changed, only extra windows. The rule is already keyed to the exact literal, so a fourth
  paraphrase is barred; this belongs in the engine seat's ledger (an engine-side "if you decline this
  list again, the hold row is the same answer" nudge, or auto-collapsing the repeat).
- **Bottoming order gap (one window, no edit):** at `...vs-ai_baka_deck146.jsonl` seq 4 the seat kept
  5 lands + 1 Sigarda and bottomed the second Sigarda, while the guide's order caps kept lands at
  four; the guide's cleanup-discard order (which ranks a duplicate above a fifth land) points the
  other way. One window, and the duplicate is a legend, so the choice is defensible — no bytes spent.
  Adoption condition: a second window with the same shape, especially with a flood loss attached.

## 4. What I did NOT check
- Only my six seat files plus the deck146 opponent seat for context; I did not read the other decks'
  seats, `results.tsv` beyond the outcome column, or any stderr file.
- I did not adjudicate the lane predictions (engine seat's job) and did not measure per-decision
  prompt bytes or latency against wave 67.
- No Scryfall call this pass: Brutal Cathar and Sigarda were verified against the primitive only
  (the primitive's `text=` matches the printed Oracle wording in both cases).
- I did not verify the guide's Teferi three-stage cascade, the ward row, the infect/converter
  matchup paragraphs or the trample block math against live windows — none arose on this seat.
