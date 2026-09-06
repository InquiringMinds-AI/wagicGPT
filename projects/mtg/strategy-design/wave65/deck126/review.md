# wave-65 per-deck review — deck 126 (Sanguine Blood, WGB walls + Sanguine Bond / Exquisite Blood)

Corpus `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-033121/`, seat files
`*-ai_baka_deck126-0x*-vs-*`. 194 decision windows this seat (150 ask, 19 priority, 12 attackers,
9 blockers, 4 reveal, 3 recovery) + 6 gameend. **3 fallbacks, all `named_row_reask` — every
`named_row_reask` in the whole corpus is this seat.** Other parse notes: `later_answer_ignored` 3,
`stale_echo_in_range` 2, `latched_row_mismatch` 1, `plan_contradicts_noop_row` 1,
`attack_walker_target` 1. 115 windows rendered a hold row, 25 hold rows taken. No timeouts, no
transport errors, no hangs, no crashes.

Record **2-4** (wins vs 130, 162). The same guide file went 4-2 in wave 64 — the guide is byte
identical (`deck126_strategy.txt`, mtime Sep 5 16:52, untouched by the wave-64 pass) and the swing
is matchup and hand variance, not guide effect: this wave the seat drew an Intruder Alarm /
Thraben Doomsayer token combo (38 attackers on turn 10), a 29-damage white curve-out, a 10-land
flood with an empty hand for six turns, and a 13-counterspell UW deck that ended on Emrakul.

## 1. Game by game

| opp | result | turns | end life | how it ended |
|---|---|---|---|---|
| 123 | LOSS | 10 | -2 / 20 | Intruder Alarm + Thraben Doomsayer: **s14** offered **A1-A38** attackers against 4 blockers |
| 130 | **WIN** | 19 | 35 / 0 | Bond s8 T5, Blood s15 T11, lifelink Vampire attack s23 T19 closed the loop |
| 152 | LOSS | 15 | -6 / 34 | Bond s14 T10; opponent's board reached 29 damage by T15 (s31) with 3 blockers available |
| 146 | LOSS | 27 | -9 / 17 | hand empty from T19 (s26, s28 "Your hand (0 cards)"); flooded to 10 lands, no combo half drawn |
| 125 | LOSS | 36 | -14 / 20 | Blood **countered** T18, Bond resolved T24; died to 2x Staff of Nin + Emrakul T36. See HIGH-1 |
| 162 | **WIN** | 17 | 18 / -2 | Sorin Vampires + Bond s25 T13, Staff of Nin s30 T15, Blood s38 T17 |

Deciding decisions, with the guide checked against each:

- **v130 (win)** — pair complete T11. The seat then held T13 (**s16**) and T17 (**s19**) against
  menus whose only cast rows were `[second copy: you already control ...]`; that is exactly the
  guide's Rule #2 "three rows are not a cast row" stop, taken correctly twice. The T15→T17 gap to
  the win is NOT a misplay: the T15 Vampire was killed by Pyrite Spellbomb on T16 (log in s23) and
  the T17 replacement was summoning sick, so no attackers window existed on T17. Guide-correct win.
- **v162 (win)** — Sorin every turn from T5, four Vampires attacking by T13, Bond T13, Blood T17.
  No misplay decided it.
- **v125 (loss)** — the only loss with a decision chain in it, and it is an interface chain, not a
  guide one. See HIGH-1.
- **v152 / v146 / v123 (losses)** — no misplay found that changed the result. v146 is pure flood
  (0-card hand for 8 windows); v123 is a combo the deck cannot block; v152's blocks at s15, s23,
  s31 all took the maximum available cover.

## 2. Engine / interface / card items

### HIGH-1 — the format line's worked example is an answer attractor on a RE-ASK, and it landed on a self-declared no-op row twice
`1788683510-ai_baka_deck126-0x559886fb3d20-vs-ai_baka_deck125.jsonl` **s32 → s34** (turn 22) and
**s45 → s47** (turn 26).

Every ask ends with `... e.g. "CHOICE: 1 (Cast <row 1's name>)" (a worked example of the format
using the first option ...)` (AIPlayerGPT.cpp:30714, `askExemplar`). On a re-ask the tail becomes:

> `[RE-ASK] "Cast Idyllic Tutor" is not on this list. Answer with a number from 1 to 5.`

placed directly after that example. Both times, the recovery answer was the example, verbatim:

- s32 reply `CHOICE: 1 (Cast Idyllic Tutor)` (not on the menu, and not in hand) →
  s34 reply `CHOICE: 1 (Cast Tribute to Hunger)` — the exemplar's own words — with
  `PLAN: The opponent has no creatures, so Tribute to Hunger does nothing.`
- s45 reply `CHOICE: 1 (Cast Idyllic Tutor)` → s47 `CHOICE: 1 (Cast Tribute to Hunger)` with
  `PLAN: Cast Idyllic Tutor to find Exquisite Blood.`

Row 1 both times: `Cast Tribute to Hunger {2}{b} {right now: they control 0 creatures - at 0 this
does nothing}`. The engine saw it — s34 carries
`parse_note = later_answer_ignored;named_row_reask_recovered;latched_row_mismatch;plan_contradicts_noop_row`
— and executed anyway, by design (AIPlayerGPT.cpp:15019, "a stamp, never a suppression and never a
re-ask"). Third instance, benign, same shape: the 152 seat **s19 → s21**, `CHOICE: 4 (Activate
Sorin ... +1)` (a legal action, but on a *later* menu) → `CHOICE: 1 (Cast Wall of Omens)`, again
the exemplar verbatim. 3 of 3 re-asks in this corpus resolved to the exemplified row 1.

**Cost, seq-cited.** Both Tribute to Hunger copies in hand were spent on the dead row (game log in
s57: "You cast Tribute to Hunger / resolved and went to your graveyard", T22 and T26). At **s54**
(turn 35) the opponent's battlefield line reads `Emrakul, the Aeons Torn {15} (15/15)`, exactly one
creature, and the seat's line has `Sanguine Bond`. A Tribute there is `gain 15` → Bond drains 15:
the seat goes 1 → 16 and the opponent 20 → 5, and Tribute is an instant so s55/s56/s57 were all
live windows for it. Its hand at s54-s57 is `Chromatic Lantern` and a land. The re-ask burned the
card that answers Emrakul, twice, on a row whose own annotation says it does nothing.

Two cheap directions (both use predicates the file already has): on a re-ask, draw `askExemplar`
from a row that is not `rowSaysNoOp` (or exemplify the decline/hold row), and let
`plan_contradicts_noop_row` gate ONE further re-ask when it fires on a re-ask *recovery* rather
than on a first answer. Note the exemplar's row-1 rule was itself a wave-46 fix (comment at
:30703) — the ask is to make it re-ask-aware, not to revert it.

### HIGH-2 — `later_answer_ignored` is unadjudicable: the record deletes the line it refers to
`1788683489-ai_baka_deck126-0x55f9e9e94f40-vs-ai_baka_deck130.jsonl` **s21**, turn 19. First coded
line `CHOICE: 4 (Cast nothing right now)`; the PLAN then self-corrects in the model's own words —
"Wait, let's re-read battlefield. Battlefield lists: Sanguine Bond, Exquisite Blood. Both halves
are ALREADY on the battlefield. ... Rule #2: With both halves on battlefield, take the first win
button available. Options: - Cast Tribute to Hunger?" — and the record ends with
`[+2069 bytes written past the PLAN line trimmed from this record]`. `later_answer_ignored` is
stamped, so a later coded line existed; its text is gone.

The brief asks this seat's corpus to "count how often a later unmarked coded line would have been
the better answer". For this seat the answer is **3 stamps, 0 answerable** (130 s21, 125 s34,
162 s23 — all three trimmed: 2878, 1949 and 2288 bytes past the answer). Recording the later coded
line itself (e.g. `later_answer_line`, the coded line only, not the prose) makes the first-wins
rule measurable at the next wave's cost of one short string per stamped record.

### MED-1 — plan carry truncates the correction and keeps the false premise
29 of 183 plan-carry windows in this seat (16%) rendered `[...the rest of your plan was not
carried: N further characters ...]`; the seat's plans average 350 chars and reach 2710. Worked
case: 130 **s21** (above) ends with the correct reading, but what is carried into **s22/s23** is
the head — `"I have Sanguine Bond on battlefield. I need to cast Exquisite Blood"` — while the same
prompt's own battlefield line prints both names and its LIFE-TO-DAMAGE CONVERTER block says "Both
halves of a life LOOP are on YOUR battlefield". The carry keeps the premise and drops the retraction.

### MED-2 — the plan-carry annotation flags only names that left the MENU, never names that are in no zone
`...-vs-deck125` **s45 → s57**: from turn 21 to turn 36 every plan this seat states names
`Idyllic Tutor`, which is in neither its hand line nor its battlefield line at any point in that
window (s57's hand is one Chromatic Lantern). The prompt's plan header does flag stale names — it
printed `; "Sanguine Bond" is no longer on your menu` at s32 — but only for names that were on a
prior menu. A plan name in NONE of {menu, hand, battlefield} goes unflagged for 15 turns, and it is
what produced the two off-menu replies in HIGH-1. Same shape in the 146 seat (s26, s28, s29, s30,
s31 all plan "cast Idyllic Tutor next turn" with `Your hand (0 cards)`).

### LOW-1 — annihilator 6 costs six full-prompt round trips
`...-vs-deck125` **s57-s62**, turn 36. The seam itself is correct and the seat answered it
correctly (six lands, no permanent given up): the header reads "FORCED sacrifice OF YOUR OWN
CARDS ... Choose card 1 of exactly 6 from the list below". But it re-sends the entire prompt six
times for one Emrakul trigger. One window listing "choose 6" (as the blockers seam already does for
multiple assignments) would cost one round trip instead of six.

### No defect found (checked, reporting as clean)
- **G2 walker attack** — 146 seat **s25**, `ATTACK: A1>W1`, `chosen_text = Vampire -> Kaya the
  Inexorable`, `attack_walker_target`. Attacked the walker and stayed on it; no toggle-out.
- **Battlement tap rows** — 10 windows offered `Add N green mana with Overgrown Battlement`,
  **0 taken** (Rule #1 held 10/10).
- **Land drops** — 42 windows offered a `Play <land>` row, **42 taken** (Rule #0 held 42/42).
- **Second-copy brackets** — rendered on every relevant row; obeyed as a stop at 130 s16/s19
  (held rather than cast a redundant half). At 125 s36/s50 the seat did cast second-copy Chromatic
  Lanterns, which is a live tension between Rule #2's "cast something every turn" and Rule #7's
  entry-7 stop, not a render fault — both times the rest of the menu was the dead Tribute row.
  `Idyllic Tutor` rows named their live target correctly (152 s29:
  `[finds only an enchantment card - still in your library ...: Exquisite Blood]`).
- **Card facts** — `Tribute to Hunger`, `Perimeter Captain` and `Pride Guardian` verified against
  `bin/Res/sets/primitives/mtg.txt:124073, 85398, 88356`; all three primitives match Oracle text,
  and the rendered rows match the primitives.

## 3. Guide verdict: KEEP as is

The live guide is not implicated in any of the six results, and the two decisions that most look
like guide failures are decisions where the guide's teaching was present, correctly recited by the
model in its own reply, and then overridden by the parser:

- The dead-Tribute casts (125 s34, s47) violate Rule #2's "THREE ROWS ARE NOT 'A CAST ROW'" stop
  and Rule #4's zero stop — and at s34 the model *quotes the guide back* while doing it: "The
  opponent has no creatures, so Tribute to Hunger does nothing ... looking at Rule #7, Entry 1 is
  for Sanguine Bond/Exquisite Blood. I have neither on the battlefield. Entry 2 is Idyllic Tutor,
  but I don't have it in hand." The teach is in the model's head at the moment of the error. No
  wording buys this back; HIGH-1 does.
- The "both halves are on my battlefield" misread (130 s21) is exactly what Rule #3's "Read both
  names off your battlefield line" targets, and the model *did* re-read and correct itself inside
  the same reply. First-wins executed the pre-correction line (HIGH-2).

What the guide positively bought this wave: 42/42 land drops (Rule #0), 0/10 Battlement taps
(Rule #1), both second-copy holds at 130 s16/s19 (Rule #2's three-row stop), the Tutor-first line
in both wins, and Tribute taken on 5- and 7-creature rows rather than named-victim theory (146 s29,
s31). A 2-4 wave on an unchanged guide that went 4-2 last wave, with the losses traced to a
38-attacker combo, a curve-out, a flood and a counterspell deck, is not evidence for an edit. The
guide is 70,992 bytes, inside the 41-71 KB band.

No `general-proposals.md` and no `skill-proposals.md`: every item above is engine/interface, and
nothing here generalizes past what the current general guide and skill already say.

## 4. Not checked
- The opponent seats were read only for game logs and outcomes, not audited for their own misplays.
- Latency, cost and deadline distributions (no `deadline_pct > 100` seen in this seat, but I did
  not census the field).
- The `hold_windows_skipped` / `mana_only_windows_skipped` counters in the gameend records (52,
  183, 23, 58, 249, 214 across the six games) — I did not verify what was skipped was skippable.
- I did not verify the *contents* of the 2,069 / 1,949 / 2,288 trimmed byte-runs (they are not in
  the corpus — that is HIGH-2).
- Mulligan quality: all six games kept the opening hand at s2; I did not evaluate the keeps.
