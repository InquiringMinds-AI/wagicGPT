# Wave 72 — deck 162 ("Draw and Die!", UB forced-draw punish) per-deck review

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260907-163626`, binary source `ceb9b8de0`, `--thinking on`.
Seat files `*-ai_baka_deck162-0x*-vs-*.jsonl` (6). **103 model decisions, 0 fallbacks, 0 truncations,
0 `action_before_plan`, reasoning on 103/103** (`reasoning_chars` 2,079 / 5,434 / 14,178 min-med-max).
1 record with `off_protocol_bytes`>0 and `plan_line_missing` (**1.0%**, wave 71 was 3.0%).
Record **3-3**: beat 125 (20–0 t13), 152 (14–0 t19), 130 (16–0 t10); lost to 123 (−4–16 t10),
126 (0–28 t17), 146 (−2–10 t15). Wave 71 was 4-2 with the parent guide.

## Did wave 71's edits pay off, and did consequence-first STRANDS change the decline?

**YES on both, cleanly.** Only **3 STRANDS rows** appeared in this seat this wave (all in the 126 game,
seq 11 / 16 / 21), all carrying the new M9(b) head
`{reserve: TAKE THIS ROW AND YOU CANNOT CAST <X> AT ALL THIS TURN. ...}`.
**3 of 3 declined, and 0 reversed at a later window of the same turn** — seq 11 (t10 Upkeep) → seq 12/13
played Island + cast Ob Nixilis; seq 16 (t12) → seq 17/18 Island + Fate Unraveler; seq 21 (t14) → seq 22/23
Underground Sea + Dictate. Wave 71 was **2 obeyed of 6, both reversed one window later** (152 seq16→17 lost
that game; 126 seq14→15). seq 11's reasoning quotes wave-71 edit B verbatim as the deciding rule:
*"the guide says: 'A row you picked whose VERDICT begins "taking this row STRANDS" -> your answer is
WRONG'. So 3 is definitely correct."* Wave-71 edit A (survive-the-re-ask) had no reversal left to prevent.
**M9(a)** (land drop folded in): **0 false STRANDS verdicts** in 103 decisions — seq 11 and 16 both printed
the land drop *inside* the remainder (`leaves 2 sources (your 1 untapped now plus the 1 your unused land
drop can still add)`), the exact shape that was wrong at wave-71 146 seq10. **M9(c)**
(`reserve_decline_windows_noted`) reads **0 in all six gameends** — no declined reserve row was re-served
this wave, so the re-serve note is UNTESTED here, not failed.
Wave-71 edit C (K is a battlefield count) held — no hand/battlefield conflation in any reasoning trace.
Wave-70 edit A (Peer into the Abyss ungated) is finally **TESTED and correct**: 146 seq15/16, life 5, the
only damage on the board, cast and modal-answered `the opponent`.

## 1. Game by game

| Opp | Result | Deciding decisions |
|---|---|---|
| 125 | **WIN** t13, 20–0 | Caress t4, Underworld Dreams t6, Puzzle Box t8, Mine t10; seq13/14 at opp 1 with a printed `DRAW FORECAST ... they would be at -8; that KILLS them` took Shield Sphere then Fate Unraveler — free bodies with lethal already locked. Clean. |
| 152 | **WIN** t19, 14–0 | Caress t4, Fog Bank t6, three Fate Unravelers t10/12/14, Dreams t16, Ob Nixilis t18 + −2, then seq24 swings three Unravelers for the kill. Blocks seq17/21 correct (below). |
| 130 | **WIN** t10, 16–0 | Mulligan a 6 then keep, bottom a Swamp (seq2/3/4); first Howling Mine at K=0 t3 (guide-sanctioned), Dreams t5, Unraveler t7, Puzzle Box t9. seq16 attacked into a 1-for-1 blocker with `DRAW FORECAST ... at -1; that KILLS them` — correct. |
| 123 | LOSS t10, −4–16 | Thraben Doomsayer + Intruder Alarm went from 1 creature (t7) to 26 (t9) in one turn. seq9's `CRACK-BACK ... for up to 27 - you would be at -7; that would KILL you` fired rule 0; the only creature row was Master of the Feast and it took it. Fog Bank was uncastable (3 Swamps, no blue, and no land drop t7/t9). **Variance + matchup, not guide.** |
| 126 | LOSS t17, 0–28 | The three STRANDS declines were all right; **seq 29 threw the game** — see below. |
| 146 | LOSS t15, −2–10 | Double removal took both punishers off the board by t8. **seq 10 cast a Dictate at K=0** — see below. |

### 126 seq 29 — the deciding decision, and it is a render contradiction

t16 Main phase 2, life 21, opponent 12, both halves of their Sanguine Bond + Exquisite Blood loop out.
Rows 1-4 are Ob Nixilis `-2: destroy target creature and draw two cards` (2 punishers out, so those 2
draws are **4 off them**, plus the removal); row 5 is Hold priority. The model took **row 5** and the game
ended 0–28 with the −2 never used again. Its `reasoning` quotes the render:
*"'Any life YOU lose, and any life THEY gain, chains until you are at 0' ... So using −2 is suicide"*,
then enumerates all four targets and rejects each. The **same prompt** carries the correct scoping 40 lines
above: `LOOP SCOPE: ... THIS number does not enter it - ... this number is life THEY lose. Their chain
starts only from life YOU lose or life THEY gain.` The model believed the loud header over the scoped
clause. PERCEPTION (the surface contradicts itself), with a guide component (§SANGUINE BOND already says
punisher damage enters the chain nowhere, but never names the −2 or a draw grant).

### 146 seq 10 — the K-of-0 engine, and the guide gap is quotable

t10 Main phase 1, life 15, both punishers already exiled, board `draw converters ... on your battlefield: 0`,
`draw converters in your hand: 0`, their board 2 creatures. The model cast **Dictate of Kruphix**, handing
the aggro deck an extra card a turn for nothing. Its reasoning names the gap exactly:
*"'K (BATTLEFIELD) OF 0: your FIRST Howling Mine is still cast ... With a Mine already out, any further
K-of-0 row is BRAKED'. It doesn't explicitly forbid casting the first engine at K=0. In fact, I have no
other plays. I must cast it."* The LAST-DITCH RELEASE did not fire (`CRACK-BACK ... you would be at 13`,
above the 10 gate). Note the guide's Forced-Fruition clause **did** hold two turns later: seq 12 and 13
declined a castable Forced Fruition at K=0 twice. STRATEGY / guide gap → edit A below.
This is the second wave running that this seat casts a K-of-0 engine into deck 146 and loses.

## 2. Engine / interface / card items

**HIGH — the `LOOP COMPLETE` header states as fatal a number the `LOOP SCOPE` clause in the same prompt
states is excluded, and the model obeyed the header.**
`1788816997-ai_baka_deck162-0x556dbce73c80-vs-ai_baka_deck126.jsonl` **seq 29** (also present seq 25-28).
Rendered: `LOOP COMPLETE: BOTH halves of a life LOOP ... are on THEIR battlefield right now. Any life YOU
lose, and any life THEY gain, chains until you are at 0 - so every life payment on this screen is fatal
rather than expensive, and a number that reads as life to them is lethal to you.` The trailing clause is
false for the dominant number on this deck's screen: punisher damage is life they **lose**, and the
`LOOP SCOPE:` clause on the DRAW PUNISHERS / DRAW FORECAST lines says so in the same prompt. Repro: read
seq 29's `LOOP COMPLETE` line, then its `DRAW PUNISHERS` line, then the reply
(`CHOICE: 5 (Hold priority)`) and its reasoning. Fix: give `LOOP COMPLETE` the same exclusion the scope
clause carries — *life THEY lose does not enter the chain* — and drop or qualify
"a number that reads as life to them is lethal to you". Cost here: the seat's only removal and 4 damage
a click, in a game it lost 0–28.

**HIGH — `[LOOP RUNNING: ... Nothing on this list stops the chain. If your answer would be the same for
every link, the HOLD row answers all of them at once]` was printed on a menu whose rows 1-4 were activated
abilities that do not enter the chain.** Same file, **seq 29** (and seq 26). The bracket's only actionable
sentence points at the HOLD row, and M6's own hold text then reads *"the rows above include ACTIVATED
abilities that are usable RIGHT NOW, and taking this row gives every one of them up"*. The model took the
hold. Suggested gate: suppress the "HOLD row answers all of them at once" sentence when any row above is an
activated ability whose effect does not feed the chain, or say plainly that rows above may still be worth
taking.

**MED — the `{reserve:}` STRANDS verdict reserves mana for a card the legend rule makes dead.**
Same file **seq 21**: `TAKE THIS ROW AND YOU CANNOT CAST Ob Nixilis, the Hate-Twisted AT ALL THIS TURN ...
3 sources cannot pay 5` while the seat already controlled an Ob Nixilis (the same prompt's `{feeds:}` reads
`draw converters ... on your battlefield: 2 - Ob Nixilis, the Hate-Twisted, Fate Unraveler`). The hand copy
is a legend-rule duplicate; the casting menu tags it (`[legendary: you already control ...]`) but the
reserve clause does not, so the decline was priced against a card the tag elsewhere calls near-dead. Not
harmful here (the recast at seq 26 was actually good — loyalty 1 → 5), but the two annotations should agree:
carry the legendary tag into the reserve clause's naming of the reserved card.

**LOW — `plan_line_missing` fires on exactly the lethal-swing attack reply, again.**
`...deck162-0x55a9eb02cb90-vs-...deck152.jsonl` **seq 24**, 72 off-protocol bytes:
`'\n\nAttack with all three Fate Unravelers to deal 8 damage and win the game.\nATTACK: A1, A2, A3'`
— the plan sentence written without the `PLAN:` label. Identical shape to wave 71's 130 seq17
(`'\n\nAttack with Master of the Feast for lethal damage; ...'`). 1 of 103 here vs 3 of 100 last wave; the
one survivor is the lethal-attack seam both waves. Answer unharmed.

**LOW — `INCOMING THIS COMBAT` prints its whole paragraph twice.** `...vs-...deck152.jsonl` **seq 21**: the
`INCOMING THIS COMBAT: 2 attackers, 9 unblocked damage ...` block appears verbatim on two lines of the same
prompt (grep the record's `prompt` for `INCOMING THIS COMBAT` — 2 hits). Costs tokens on the largest
combat prompts; no wrong fact.

**Not defects, recorded as passes**: the `[castable now]` answered-tag appeared on 57 prompts across the
seat with no case of a card tagged castable being unpayable; `CRACK-BACK NEXT TURN` rendered on 37 prompts and drove rule 0
correctly at 123 seq9; the conditional crack-back "that would KILL you" clause fired only where an
animator/attacker set actually existed; `own_turn_windows_skipped` (0/12/39/30/67/23) collapsed the seat to
103 decisions from wave 71's 100 over the same six games without any observed lost window in this seat.

## 3. Guide verdict: **EDIT**

`wave72/deck162/strategy.txt` — **19,997 bytes, from 19,911 (net +86)**, under the 20,000 ceiling. Three
adds, all paid by a decision that went wrong; the bytes come from a cut section and two prose trims.

| # | before → after | paid for by |
|---|---|---|
| A | `K (BATTLEFIELD) OF 0: your FIRST Howling Mine is still cast ... With a Mine already out, any further K-of-0 row is BRAKED` → `the ONE engine you cast here is your FIRST HOWLING MINE. A Dictate, Puzzle Box, Forced Fruition or Master row at K of 0 is BRAKED whether or not a Mine is out - "the guide does not explicitly forbid the first engine" is wrong, this line forbids it, and an engine at K of 0 arms THEIR deck and not yours. ... Every other K-of-0 row: take a wall, a punisher, a Caress, or "Cast nothing right now"` | 146 seq 10, reasoning verbatim: *"It doesn't explicitly forbid casting the first engine at K=0 ... I must cast it."* The branch named Howling Mine and only ever braked "with a Mine already out"; a Dictate at K=0 with no Mine fell through the hole. |
| B | `A row tagged "[legendary: you already control one ...]" is likewise not a play.` → same, plus: ONE exception — your Ob Nixilis row while the Ob Nixilis on your battlefield shows loyalty 2 or less; cast it, and on the LEGEND RULE ask name the LOW-loyalty copy as the one you GIVE UP | 126 seq 26 (cast the tagged second Ob Nixilis over a loyalty-1 copy) + seq 27 (correctly gave up copy #1, keeping loyalty 5). The model played it right **against** the guide; the guide's absolute was the error. |
| C | `§SANGUINE BOND: ... your punisher damage enters it NOWHERE.` → adds that Ob Nixilis's −2 and every card you hand them is life THEY LOSE, which starts no chain — keep activating the −2 — and names `LOOP SCOPE:` as the clause that scopes this, against a headline about payments "lethal to you" | 126 seq 29, the game-losing hold. The section covered *casting punishers* and *racing*, never an activation that makes them draw. |

Bytes returned: the whole `=== SITUATIONS ===` section (two worked examples — bullet 1 restates the reserve
rule, which now fires 3/3 unaided; bullet 2 restates the K-of-0 brake and the Caress rule, both rewritten
above); the `GENERIC IS NOT A COLOUR` worked example (the hand line prints `[cannot pay now: needs {1}{u},
your 3 untapped sources cannot pay it]` itself — no colour misread in 103 decisions); and the sentence
describing the `[no cast row now: sorcery speed ...]` hand tag.

**Not changed, deliberately:** the `{reserve:}` section and its survive-the-re-ask rule (3/3 obeyed, 0
reversals — a success verdict, it is doing exactly the job wave 71 bought it for), BLOCKING (152 seq17 took
the Shield Sphere `neither dies` line over two Fate Unraveler `blocker dies` lines and over a printed GANG
BLOCK, per rungs 1-2; seq21 split two walls across two attackers rather than gang-piling), ATTACKING
(126 seq28 `no attackers` under a LIFE-TO-DAMAGE CONVERTER of theirs; 130 seq16 sent into a friendly tag;
152 seq24 swung all three on a cleared lethal check), MULLIGAN/BOTTOMING (130 seq2/3/4, all three per rule),
FORCED SACRIFICE (no prompt), CAST ORDER, LAND DROP (9 of 9 land drops taken), the LETHAL CHECK
(no off-by-one this wave — wave 71's `9 - 8 = 1. They die.` shape did not recur).

## 4. Not checked / limits

- Only this seat's six translogs plus the six opponent seats for outcomes. No lane adjudication, no other
  deck's seats, no engine-seat counters beyond this seat's gameends.
- **`reserve_decline_windows_noted` is 0 in all six games** — I report M9(c) as UNTESTED in this seat, not
  as a failed fix; I did not read the engine code to confirm the re-serve path is reachable.
- `own_turn_windows_skipped` totals 171 across the six games. I did not audit the skipped windows (no
  record exists for them), so "no lost window" is an absence of evidence, not evidence of absence — the
  engine seat owns that.
- The 123 loss is called variance on the strength of the t7→t9 board jump and an all-Swamp mana base with
  no land drop available on either turn; I did not model whether another t7 line survives Intruder Alarm.
- The 126 counterfactual (that using the −2 at seq 29 wins or extends) is not re-run: it is bounded by the
  removal plus 4 damage per activation the seat forfeited, not measured.
- Card facts read off the engine's own rendered rows and `{card text:}`; no primitives grep and no Scryfall
  call was needed — no card behaved contrary to its rendered text in these six games.
- No general-guide or skill proposals. Both new defects are deck-162-local or engine-render items (the
  LOOP header contradiction is a render fix, and proposing "obey the scoped clause over the headline" as
  general guidance would be teaching doubt about a surface that should simply be made consistent).
