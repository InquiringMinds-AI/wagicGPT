# Wave 66 — deck 146 (Orzhov dungeon midrange) per-deck review

Corpus `matchups-20260906-064038`, binary 9b797fb81. Seat files (all six games natural):

| game | file (epoch prefix) | result | turn | life |
|---|---|---|---|---|
| 146 v 162 | `1788694857-…deck146-0x557f1bc54090-vs-…deck162` | **WIN** | 13 | 15 / -2 |
| 146 v 152 | `1788694847-…deck146-0x555bfeca02e0-vs-…deck152` | **WIN** | 15 | 22 / 0 |
| 146 v 126 | `1788694840-…deck146-0x55e5f1b97220-vs-…deck126` | **WIN** | 17 | 21 / 0 |
| 146 v 125 | `1788694855-…deck146-0x558da8ba0140-vs-…deck125` | **WIN** | 19 | 20 / 0 |
| 146 v 130 | `1788694845-…deck146-0x55d9b68c3270-vs-…deck130` | LOSS | 14 | 0 / 7 |
| 146 v 123 | `1788694867-…deck146-0x55940b5ed1e0-vs-…deck123` | LOSS | 12 | -80 / 14 |

**4/6.** 206 decisions, **0 `unparsed_reply`**, 0 `repeat_past_stop`, 0 `named_row_reask`,
0 `reveal_stall_forced`, 0 `wall_miss_unrecorded`, 0 `answer_replaced`, 5 `later_answer_ignored`.
Hold row rendered 77 times, **taken 20**, plain `Cast nothing` taken 4 — deck 146 inverts wave 65's
hold-vs-decline ratio (123 was 4:182, 125 was 29:213). `DRAW FORECAST` appeared in **1** window all
corpus, so AQ H1's re-charge run class did not arise on this seat.

## 1. Game by game — the deciding decisions

**146 v 162 (win, T13).** Clean. Silencer named Howling Mine (s7) off the correct engine census;
Soul Shatter took Ob Nixilis at its only live window (s19, `{right now: they sacrifice Ob Nixilis,
the Hate-Twisted (MV 5, their highest)}`); Vanishing Verse took Liliana's Caress over Master of the
Feast (s26) because Soul Shatter was already on the stack for the Master — the seat sequenced two
removal spells correctly across one stack. s23 discard offered 8 rows and the seat discarded a dead
Soul Shatter (`{right now: they control 0 creatures or planeswalkers}`), the disposability-correct
answer. s8's reply carried its reasoning **before** the coded line (AR H2a's target shape) and parsed.

**146 v 152 (win, T15).** Correct removal-on-curve pattern (Soul Shatter on Katilda s13, on Sigarda
s30). s26 attackers: reasoning-first reply, no coded line until after the working — parsed clean.

**146 v 126 (win, T17).** Won it at s25/s26 by exiling Exquisite Blood with Vanishing Verse, having
already exiled Sanguine Bond at s15/s16 — the seat correctly identified the two-card loop and spent
both Verses on it. The cost of that game is entirely in the LOOP HALF PENDING narration (HIGH-2).

**146 v 125 (win, T19).** Silencer named Sphinx's Revelation; Kaya exiled Lightmine Field's
predecessors. Nothing decided against the seat; the game's cost is deliberation (s26 325 s,
s41 324 s, s38 274 s on windows whose answer did not change the result).

**146 v 130 (LOSS, T14) — the decided game.**
- s20 (T9 attackers) the seat attacked with all four. The engine printed the true verdict:
  `ATTACK TOTAL: 4 attackers listed, 11 total combat damage to a player - declaring all of them with
  none blocked puts them at 3. At least 7 damage lands whatever they block - they would be at 7.`
  The reply wrote `The total power is 11, which reduces their life to 3 … which is lethal` against
  an opponent at 14. STRATEGY error (arithmetic), not a render error — the attack was still the
  right play (every row carried `[held back, it CANNOT block: Rorix Bladewing (flying)]`, so
  holding back bought nothing). Opponent to 7; Starstorm then swept the seat's whole board on T10.
- **s29 / s31 / s33 (T13, at 3 life) — the game.** Silverquill Command's census read
  `{modes live right now: return creature and you draw, return creature and opponent draws; …}`.
  It was **correct**: Triumphant Adventurer and Silverquill Silencer (both MV 2) had died to
  Starstorm on T10. The seat could not see that, spent 164.8 s and ~4.6 KB at s31 trying to
  reconstruct its graveyard from the log, concluded "There are NO creature cards in my graveyard …
  This is a contradiction … I cannot cast Silverquill Command", and took `Cast nothing right now`.
  Casting it returns a 2/2 first-strike deathtouch body (completed-dungeon anthem was ACTIVE, the
  prompt says so) plus a card, at 3 life with an empty board against a 6/5 flier. It died the next
  turn. PERCEPTION — the surface the seat needed (its own graveyard) is not rendered anywhere.

**146 v 123 (LOSS, T12).** Mulligan to 6 (s2/s3 — the s2 ship was a real 1-land Snarl hand, defensible),
then a black-source drought to T9. s12: Vanishing Verse's target list offered only `Vampire (creature
2/2) [flying]` — **correct**, Bloodline Keeper carried Lightning Greaves and therefore shroud, so the
engine that made 22 tokens by T11 was untargetable by every removal spell in this deck. Not a guide
or engine failure; an unanswerable board. s19 is worth recording for AR: the reply is
`ATTACK: A1` followed by a PLAN reading *"attacking into 22 flyers with a 3/2 is suicide … We must
hold Silencer to block"* — the model's reasoning landed **after** the coded line and contradicted it,
and first-wins ran the answer. Here first-wins was *right* and the reasoning was wrong (the row said
`[held back, it CANNOT block ANY of their 22 creatures]`). Nothing changed the result; -80 was the
Intruder Alarm / Bloodline Keeper loop.

## 2. Engine / interface / card items

### HIGH-1 — the prompt never renders your graveyard, but rows are priced from it
File `1788694845-ai_baka_deck146-0x55d9b68c3270-vs-ai_baka_deck130.jsonl`, seqs 29, 31, 33.
Rendered (s31, row 1): `Cast Silverquill Command {2}{b}{w} … {modes live right now: return creature
and you draw, return creature and opponent draws; HALF DEAD (one half of the pair has no legal
object …): creature gains 3/3 and return creature, return creature and sacrifice creature, …}`.
The CURRENT SITUATION block lists life, mana, land drop, hand, **your battlefield**, opponent
battlefield, artifacts, hand size, both library counts — and no graveyard, in any window of any of
the six games. Every graveyard-dependent surface this deck runs (Silverquill Command's return mode,
Barrowin's attack trigger, Agadeem's Awakening's X, Kaya's -7) is therefore a claim the seat cannot
check. The deck guide already forbids reconstructing it (`deck146_strategy.txt:203`: *"Do not
reconstruct your graveyard from the log lines above the menu; the row has already done it"*), and
the seat broke that rule precisely because the census had no corroborating surface: it re-derived
from a 15-turn log, got the wrong answer, and called the engine buggy.
Cost on this seat: 3 windows / 199 s and the game. **Repro:** add a `Your graveyard (N cards): …`
line to the situation block beside `Your battlefield`, at minimum naming the creature cards with
their mana values — the census already computes exactly this set to decide the row.
Primitive verified: `borderline.txt:102739-102749`, `text=` matches Scryfall's Silverquill Command
Oracle exactly; the `auto=choice … target(creature[manacost<=2]|mygraveyard)` lines confirm the
census's live half was right.

### HIGH-2 — `LOOP HALF PENDING` calls an EXILED half "one resolution from closing"
File `1788694840-ai_baka_deck146-0x55e5f1b97220-vs-ai_baka_deck126.jsonl`, seqs 21, 22, 23, 30, 31,
32, 33. Rendered (s30): `LOOP HALF PENDING: Sanguine Bond is on THEIR battlefield and the other half
of the pair, Exquisite Blood, is in their exile. Nothing has chained yet - the pair is one
resolution from closing, and when it closes any life YOU lose, and any life they gain, chains until
you are at 0, so every life payment you are pricing now becomes fatal rather than expensive.`
The note names the zone correctly and then contradicts it. Exile is not one resolution from the
battlefield — and the seat had *just spent its own removal* putting it there (s15/s16 and s25/s26),
so the reward for solving the problem was a prompt insisting the problem was imminent. 6 of the 10
renders on this seat are the exile form; the 4 hand-zone renders (s24-26) are correct and useful.
Price: the three windows carrying the exile wording are the three slowest of the game — s21 256.5 s
(3 749-byte overrun), s30 197.8 s, s32 220.1 s = **674 s**, all re-deriving whether the loop closes.
At s32 the model talked itself into `Sanguine Bond triggers, causing them to gain 5 life`, which
Sanguine Bond does not do (`mtg.txt:99004-99006`, `text=Whenever you gain life, target opponent
loses that much life` — matches Scryfall). **Repro:** gate the "one resolution from closing" clause
on the missing half being in a zone it can actually return from (hand, library, graveyard with
recursion); when it is in exile, say so and say the pair is broken.

### MED-1 — the attackers seam prices lethality but never states it
`…deck130.jsonl` s20 (quoted above). `ATTACK TOTAL:` gives both numbers and leaves the comparison to
the model, which got it backwards and wrote "lethal" of a line that leaves the opponent at 3. The
row already knows both figures. One clause — `— this is NOT lethal (they survive at 3)` / `— this
IS lethal` — closes a class of error the model demonstrably makes. Same seam, `…deck162.jsonl` s21
("Attack for lethal. 7 damage to 11 life leaves them at 4") and `…deck126.jsonl` s23 ("Deal 5 damage
to opponent (11 -> 6)" written under the heading "lethal") — 3 windows on this seat where the model
wrote "lethal" about a non-lethal total. None flipped a result here; all three were the right attack.

### MED-2 — `choice` disagrees with the rendered row number on a reveal/PUT ask
`…deck125.jsonl` s11 (`kind: reveal`). The prompt renders `5. Final Judgment (sorcery) … [eligible
for "choose a card"]` and asks for `PUT: <number>`. The reply was `CHOICE: 5 (Final Judgment)` —
wrong label, right number and name. The engine resolved it by NAME and took the right card
(`chosen_text: "Final Judgment"`), but recorded `choice: 1`. Two notes: (a) the name-resolution
recovery is what kept this off the corpus's 83 `unparsed_reply` — worth knowing which recovery path
saved it; (b) any census counting reveal decisions from `choice` will be wrong. `reveal_wait_secs`
6, `reveal_wait_unexplained_secs` 6, no stall.

### MED-3 — dead rows re-offered without the no-live banner, at 300 s a window
`…deck125.jsonl` s26 (325.4 s, 4 164-byte overrun) and s41 (324.3 s). At both the seat re-derives
row-by-row that nothing does anything; `NO LIVE CAST ROW ON THIS MENU` fired at s44 (71 s) but not
at s26/s41 — correctly, since one row was half-live each time. The observation is that half-live
menus cost as much as fully dead ones. On this seat the four windows over 250 s (s2 301 s, s26 325 s,
s38 274 s, s41 324 s) are all menus whose answer did not change the game.

### LOW-1 — first-wins ran the answer written before the reasoning, 1 of 206
`…deck123.jsonl` s19: `ATTACK: A1` then a PLAN arguing the opposite. AR H2a's reorder did not take
on this reply; the outcome was still correct. All **5** `later_answer_ignored` on this seat are
byte-identical **restatements** of the executed line — `…deck126` s21 `CHOICE: 1 (Cast Silverquill
Command)`, s30 `CHOICE: 6 (Cast nothing right now)`; `…deck130` s27 `CHOICE: 3 (Hold priority)`;
`…deck125` s24 `ATTACK: A1, A2`, s38 `CHOICE: 7 (Hold priority)`. Measured cost of first-wins on
deck 146 this wave: **zero**. `ignored_line` is recorded verbatim in all 5 (AR H2c works).

### LOW-2 — `Cast nothing right now (combat comes next this turn)` in the Blockers step
`…deck126.jsonl` s32, phase Blockers, row 3 reads `Cast nothing right now` while the same game's
main-phase menus append `(combat comes next this turn)`; the wording is right here but s33 (also
Blockers) and s29 show the seat re-reading the phase off the row rather than the header. Cosmetic.

### Not an item — verified false alarms
`…deck123.jsonl` s12: Vanishing Verse's target list omitted Bloodline Keeper. Correct — Lightning
Greaves granted shroud, and the battlefield line printed `[flying, haste, shroud] {attached:
Lightning Greaves}`. `…deck130.jsonl` s18: the legend-rule target ask named both Nadaars with
`[summoning sick - cannot attack this turn, but CAN block]` on #2 (AT H6's tag, working on a target
row — the wave-65 HIGH-6 seam). 47 windows carried a summoning-sickness tag on this seat, none wrong.

## 3. Guide verdict: **KEEP AS IS**

`bin/Res/ai/baka/deck146_strategy.txt`, 70 997 bytes (in band, at its ceiling). No edit; no
`strategy.txt` written.

Why this is a success verdict, not a shrug:
- **4/6, and both losses are paid for elsewhere.** 146 v 123 is a mulligan to 6 into a shroud-
  protected token engine no card in this deck can target — the guide cannot answer that. 146 v 130's
  deciding decision (s31) is a missing render, HIGH-1.
- **The one decision that lost a game was the guide being DISOBEYED, not being wrong.** Lines
  196-205 already say, in the seat's own words at the right seam: *"Read the live half and look for
  the words 'return creature' in it - that, and nothing in the log, is how you know the return mode
  is live … Do not reconstruct your graveyard from the log lines above the menu; the row has already
  done it."* At s31 the seat did exactly the forbidden thing and lost. Restating an instruction the
  seat already had, in stronger words, is the busywork pattern; the fix is HIGH-1's render.
- **The guide's live instructions were followed and paid.** Removal-on-curve rather than hoarding
  (`vs162` s19/s24, `vs152` s13/s30, `vs126` s25/s27); the dungeon line (`dark pool` over `storeroom`
  at `vs162` s22, `vs152` s28, `vs126` s20) drained 3 life across the wins; "DO NOT CAST AGADEEM'S
  AWAKENING - EVER" held in all 4 windows where the row was offered (`vs123` s13, `vs130` s8,
  `vs152` s22/s24 — each played as a land instead); the wave-65 mulligan rewrite held at
  `vs162` s2, `vs126` s2, `vs152` s2 and `vs125` s2 (four keeps, four wins).
- The two wave-65 deck-146 HIGH items are answered: the modal mode-ask census (H9) is now correct
  and internally consistent everywhere it printed on this seat (`vs126` s21/s22 took a genuinely
  live pair off the census, `vs130` s29/s31/s33 was right about a graveyard the seat could not see);
  the land-drop threat tag (wave-65 MED) now prints — `vs123` s16 `{also a THREAT: this land
  ANIMATES … cheapest rung {3}{b} for 3 power}` — 10 windows, and the Hive was played every time.

## 4. Proposals

None. No `general-proposals.md`, no `skill-proposals.md`. Both HIGH items are engine-side surfaces,
not general-guide or skill-writing matters, and MED-1 is a one-clause render change at a seam the
engine seat owns.

## What I did NOT check
- The opponents' seat files, except for `results.tsv` outcomes and the six games' own logs — I did
  not read deck 123's, 130's, 125's, 126's, 152's or 162's translogs for their side of these games.
- The 83 corpus-wide `unparsed_reply` records: **none are on this seat** (0 of 206 decisions across
  all six files), so I could not characterise either reply shape from deck 146's transcripts.
- `AIPlayerGPT.cpp` — I did not read source for any item; every claim above is from the rendered
  `prompt`, the record fields, and the primitives files.
- Whether `Final Judgment` at `vs125` s11 was the best of the seven revealed cards (I checked the
  execution, not the choice).
- Blocker-declaration quality: no `blockers` decision arose on this seat in any of the six games.
