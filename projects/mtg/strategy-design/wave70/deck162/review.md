# Wave 70 — deck 162 ("Draw and Die!", UB forced-draw punish) per-deck review

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-224849`, binary master 14c82df37, `--thinking on`.
Seat files: `*-ai_baka_deck162-0x*-vs-*.jsonl` (6). 134 model decisions in this seat, 0 fallbacks,
0 truncations, 0 `action_before_plan`, 8 records with `off_protocol_bytes>0` (6.0%), reasoning present on
134/134, `reasoning_degenerate` max 0.006 (no collapsed traces), `reasoning_chars` p50 6470 / max 16425.
Latency this seat p50 100.1 s, p95 207.3 s, max 360 s (corpus p50 80.7 / p95 185.6).
Record: **2-4** (beat 123 and 130; lost to 126, 146, 152, 125).

## 1. Game by game

| Opp | Result | End | Deciding decisions |
|---|---|---|---|
| 123 | **WIN** t11, 20–0 | Ob −2 into Bloodline Keeper, then Fate Unraveler swing | seq16, seq17 — both clean |
| 130 | **WIN** t11, 16–(−2) | Underworld Dreams t5, Master t7, lethal swing t11 | seq7, seq12, seq14 — clean |
| 126 | LOSS t11, 0–33 | Sanguine Bond + Exquisite Blood assembled t11 | seq9, seq13 — **not a misplay**, see below |
| 146 | LOSS t13, 0–14 | flashed Dictate on a K-of-0 board at seq14 | seq10 (hold), **seq14** |
| 152 | LOSS t15, −37–7 | opponent at 7 and one punisher away; **seq21** stranded it | **seq21**, seq22 |
| 125 | LOSS t39, 0–43 | 8 windows declining a payable Peer into the Abyss | **seq16–seq22**, seq25 |

**126 (loss) is clean play, not a guide failure.** At seq9 the reasoning computes the block correctly
(blocking a 1/1 lifelinker under their Sanguine Bond is *me −1 / them +1* vs *me −1 / them +0* unblocked),
and at seq13 it correctly identifies that with both halves out any lifegain loops. The prompt itself says
declining does not escape it either. The game was over when Exquisite Blood resolved. No deck-162 line
interacts with that; do not read this loss as guide effect.

**152 (loss).** seq21, own Draw step, 1 life, opponent at **7** — one Underworld Dreams from winning.
The Dictate row printed `VERDICT: taking this row STRANDS Underworld Dreams this turn - 1 source cannot
pay 3`. The model took it anyway; reasoning: `K=1 (Underworld Dreams in hand) -> Cast Dictate`. seq22
confirms the strand (1 untapped source, and the punisher gone from hand). STRATEGY, and a straight
violation of the guide's `{reserve:}` section — which outranks both K branches and was not consulted.

**146 (loss).** seq14, own Draw step, life 12, K=0 with a Mine already out, board 0 creatures vs 2 of
theirs — the guide brakes this row on *both* release tests (colour: Ob Nixilis needs 5 total, 4 available;
board: their creatures over your 0 at life ≤15). Reasoning shows exactly what beat the brake: it quotes
the CARD FACTS line *"its flash is why its row can appear in your upkeep or draw step where no punisher
can be beside it"* and concludes **"This strongly suggests casting it now."** An explanatory clause was
read as permission. Same trace also rejects the engine's `{leaves 1 of your 4}` count ("Maybe it counts
differently or I misread") after mis-parsing `{1}{U}{U}` as two sources.

**125 (loss).** By t16 every punisher was in the graveyard. Peer into the Abyss was payable and printed
`if you choose "the opponent": life -13/-15` in **11 windows** (seq14–seq27); it was cast in **one**
(seq25, t22). Reasoning at seq16–22 is uniform: *"Wait for a punisher to hit the battlefield"* — i.e. the
guide's rung 2 gate, `PEER INTO THE ABYSS, with a punisher already out`. Six turns of no action at 8–9
untapped sources with the opponent gaining life. STRATEGY, guide-caused.

## 2. Engine / interface / card items

**HIGH — converter and {reserve:} counts ignore a pending own-Puzzle-Box trigger on the stack.**
`1788752962-...deck162-...-vs-...deck152.jsonl` seq21 (t14 Draw). The same prompt renders
`ON THE STACK ... 2: ability: Teferi's Puzzle Box's Ability [from your Teferi's Puzzle Box]` **and**
`draw converters in your hand: 1 - Underworld Dreams` **and** a `{reserve: ... STRANDS Underworld Dreams
this turn}` verdict. That trigger bottoms the seat's own hand; at seq22 the prompt says
`no "Underworld Dreams" is in your hand ... a copy is still in your LIBRARY`. Both the K-release input
and the reserve verdict were computed against a hand a listed stack object was about to destroy. Repro:
open seq21, compare its `Your hand (6 cards)` line with seq22's.

**MED — modal rows for the Tomb of Annihilation room render with no annotation at all.**
`...deck162-...-vs-...deck146.jsonl` seq21 (t13, life 4). Rendered verbatim:
`1. lose 2 life` / `2. sacrifice cards` — no `{this mode right now: ...}` clause on either row, while the
*same seat's* seq16 (Annihilation Rooms) rendered `1. lose 2 life {this mode right now: you LOSE 2 life -
you would be at 9 ...}`. The reasoning then invents the missing row's content: *"Sacrificing cards costs
life. I'd die."* The primitive (`borderline.txt:118767`, Sandfall Cell) sacrifices an artifact, a creature
and a land — no life. Also the header names the dungeon, not the room.

**MED — a reply with no PLAN line counts as protocol-conforming.**
`...vs-...deck146.jsonl` seq22 reply is exactly `\n\nBLOCKS: B1:A1` — no PLAN line — yet
`off_protocol_bytes` is 0 for that record and `protocol_replies` = 21 of 21 at gameend. The shape check
detects extra bytes, not a missing first line.

**LOW — `off_protocol_bytes` census for this seat (8 of 134, one shape class).** Every one is the same
defect: the plan line written *without* the `PLAN:` label, immediately followed by a well-formed CHOICE
line. Quoted:
`'\n\nCast Master of the Feast.\nCHOICE: 1 (Play Island)'` (146 seq19, 25 B);
`'\n\nApply direct damage to win the game.\nCHOICE: 2 (Cast Peer into the Abyss)'` (125 seq25, 36 B);
`'\n\nBrake on draw engines and hold Master of the Feast until opponent has creatures, as I lack draw
converters to punish their extra draws.\nCHOICE: 4 (Cast nothing right now)'` (125 seq43, 135 B).
Remaining five: 125 seq30/33/41/46/47. No answer was harmed; the label, not the shape, is what slipped.

**LOW — hold row taken without its gate.** `...vs-...deck146.jsonl` seq10 (t7 Upkeep). The prompt printed
`[hold check: every row above was also on the menu at the last window at this seam (1 window in a row
now)]`, not the `[you declined this exact list N times already this turn]` line the guide gates on. The
game's gameend records `hold_windows_skipped_cast: 16`. 1 of 1 hold takes in this seat was ungated;
16 skipped cast windows for one ungated answer is a lot of leverage on a line the model reads loosely.

## 3. Guide verdict: **EDIT**

New file: `wave70/deck162/strategy.txt` (19,963 bytes, from 19,971 — net −8, under the 20,000 ceiling;
every addition is paid for by a cut, listed last).

| # | before → after | paid for by |
|---|---|---|
| A | rung 2 `PEER INTO THE ABYSS, with a punisher already out.` → cast it whenever its row is payable; the `life -N` clause needs no punisher; only a punisher row on the SAME menu goes first; never hold it waiting for a punisher | 125 seq16–22 (8 declines of a payable Peer, reasoning naming the gate); 125 seq25 |
| B | CARD FACTS Dictate: `its flash is why its row can appear in your upkeep or draw step where no punisher can be beside it` → flash explains *why the row appears* and is NOT a licence; a Dictate row is an ENGINE row and the brake and `{reserve:}` govern it | 146 seq14 reasoning quotes this clause and concludes "This strongly suggests casting it now" |
| C | `{reserve:}` rule: `When it names a punisher, DO NOT TAKE THAT ROW` → **any** row printing a verdict beginning `taking this row STRANDS`, whatever card it names | 152 seq21 (named a punisher, taken anyway); 146 seq14 (named Master of the Feast, so the old wording never fired) |
| C2 | mana section: "do not re-derive" extended from `Mana available` to a row's `{leaves N ...}` count and any VERDICT in it | 146 seq14 reasoning mis-parses `{1}{U}{U}` as 2 sources and overrides the printed count |
| D | CARD FACTS Puzzle Box: adds that it bottoms **your** hand too — a card in hand does not survive to your next turn, and a "draw converters in your hand" count read in your own draw step with its trigger on the stack is already false | 152 seq21 → seq22 |
| E | hold gate: adds that a `[hold check: ... N window(s) in a row]` line is **not** the `[you declined this exact list N times]` line | 146 seq10 |

Bytes returned: SANGUINE BOND section compressed (its content was correct and the model reached it on its
own in 126 — it does not need five sentences); two SITUATIONS worked examples dropped (the body-turn one
restates rung 0, the four-punisher −2 one restates the lethal check); `THE RESERVATION MUST SURVIVE THE
RE-ASK` and the HOLD-PRIORITY row quotation shortened; three minor quotations trimmed.

**Not changed, deliberately:** BLOCKING, ATTACKING, FORCED SACRIFICE, MULLIGAN and the SANGUINE BOND
substance — no decision in these six games went wrong under them. The blocking rungs in particular were
never the loss: 126's two `BLOCKS: none` answers were correct, and 146 seq22 was on a board the engine
already declared unsurvivable.

## 4. Not checked / limits

- I read only this seat's six translogs plus the opponent seats' outcomes; I did not read other decks'
  seats, and I did not adjudicate any wave70 lane prediction (engine-seat's job).
- I did not verify Sandfall Cell against Scryfall (network); the claim rests on the primitive at
  `projects/mtg/bin/Res/sets/primitives/borderline.txt:118767`. Teferi's Puzzle Box symmetry is verified
  against `mtg.txt:118332` (`@each my draw: ... bottomoflibrary all(*|myhand)`).
- Whether the 16 skipped cast windows in the 146 game contained a play that mattered is **unproven** —
  the skipped windows are not in the translog, so the cost of seq10 is bounded above, not measured.
- No general-guide or skill proposals: every defect here is deck-162-local (Peer gating, a Dictate card
  fact, a Puzzle-Box omission) or an engine item, and the `{reserve:}` broadening is a wording fix inside
  this guide, not a new general rule.
