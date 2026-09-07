# Wave-71 deck126 review — SANGUINE BLOOD (WGB wall prison, Exquisite Blood + Sanguine Bond)

Corpus `matchups-20260907-085638`, all six seat files
`...ai_baka_deck126-<ptr>-vs-...`. Record **3/6** (wave 70: 2/6). 157 model decisions in this seat:
**0 fallbacks, 0 `off_protocol_bytes`, 0 `plan_line_missing`, 0 `action_before_plan`, 0
`reply_truncated`, 0 `reasoning_chars==0`, 0 `async_drops`, 0 `menu_pass_no_progress`, 0
`ask_replays_refused`, 0 `declined_face_latches`, 0 `wall_miss_events`.** Every reply was exactly
PLAN + action. `reasoning_chars` p50 5,522 / p95 12,587 / max 20,925; latency p50 103.9 s / p95
229.8 s / max 479.2 s (slower than wave 70's p50 61.3 s at the same seat). No `askreplay/`
directory exists in the corpus dir at all (not empty — absent), so this seat can say nothing about
whether the F4 sidecar path is right.

## 1. Game by game

| # | seat file (epoch) | opp | result | deciding decisions |
|---|---|---|---|---|
| 1 | 1788789405 | 146 | LOST (-2)–17, t27 | Pair COMPLETE t12 (seq 17) and live through t14; hand held no Tribute/Staff, so no starter existed (seq 18-20 menus are walls + Sorin only). Sorin cast seq 19, +1 taken seq 32 — both by the book. Bond **exiled** by the opponent at t15 (seq 35 line reads "Sanguine Bond is in your exile ... the pair is BROKEN"); the seat then ground 12 more turns behind walls. **seq 50** is the misplay: Sorin at 7 loyalty, their line at 9 creatures, rows 1/2/3 = +1 / -2 / **-6 destroy-up-to-three-and-reanimate**, and the answer was row 5 Hold priority (§3-A, §3-B). |
| 2 | 1788789417 | 152 | **WON** 41–0, t16 | Bond re-cast **from exile** at seq 23 ({5}{b}{b}, +{2} tax, render correct and the model read it) completing the pair at 12 life under a lethal crack-back; Vampire attack seq 27 closed it. One wasted window at seq 26 (Hold priority over Sorin +1 at 4 life, §3-A). |
| 3 | 1788789419 | 130 | LOST (-2)–19, t14 | Not a decision game. Draws were lands: hands at seq 11 (t9) and seq 12 (t11) are 4 lands and 0 spells, so no cast menu was offered at all. Rorix Bladewing (6/5 flying haste) meant **zero** blockers asks in the whole game — legitimate, no defender can block a flier. Only Tribute arrived t13 at 5 life against 5 creatures. Variance + evasion. |
| 4 | 1788789429 | 123 | **WON** 42–0, t13 | Bond t9 (seq 26), Blood t11 (seq 29), Staff of Nin t13 (seq 32) and the ping at seq 33 closed the loop from 19 — the wave-70 rung-8 edit ("with BOTH names out, Staff and Sorin move to the TOP") paying directly. Four dead-Tribute windows at t7 handled correctly ("at 0 this does nothing"), though two of them took "Cast nothing" over the hold row (§2 LOW-4). |
| 5 | 1788789431 | 162 | LOST 0–20, t13 | **The one loss with real decisions in it.** Draw-punisher board (Underworld Dreams ×2, Liliana's Caress, Howling Mine, Ob Nixilis, Dictate of Kruphix); no combat at all. seq 14 (t5, life 20) spent Tribute on Shield Sphere (0/6) **[defender]** — a HOLD case by the live guide — for 6 life. seq 21 (t9) and seq 27 (t11): Sorin cast t9, and the +1 row was on the menu both times with the PLAN line naming it; both answered **Hold priority**, so Sorin made **zero** Vampires in the game (§3-A). seq 26 (t11, life 15): cast Wall of Omens off a row printing `[DRAW PRICE: ... you would be at 13]` with the pair incomplete — the exact cast the guide's DRAW PUNISHERS paragraph forbids, and the reasoning never mentions the price (§3-C). |
| 6 | 1788789435 | 125 | **WON** 40–0, t18 | Mulligan seq 2 (1 land / 6 spells → ship, by the book), bottomed the duplicate Pride Guardian at seq 4 (by the book). Blood t8 (seq 14), Tutor→Bond t12/t14 (seq 18/19/22), Staff t18 (seq 34) + ping (seq 35) closed it. No misplay found. |

**On seq 14 (vs 162).** The guide's [defender]-hold rule was violated and I am **not** paying for
an edit with it: deck162's only creature all game was that Shield Sphere, so a held Tribute would
have rotted, and 6 life bought a turn against a clock the seat had no other answer to. The rule is
right in general; this was the case it does not cover, and one instance is not evidence.

## 2. Engine / interface / card items

**HIGH-1 — the priority hold row's "you give up no cast" is false about the activated rows printed
beside it, and the model reads it literally.** Repro `...vs-ai_baka_deck162.jsonl` **seq 21** (t9,
life 24). Rendered rows:
`1. +1: create a 1/1 vampire with Sorin, Lord of Innistrad [cost: Counters]`
`2. -2: emblem: "creatures get +1/+0" ...`
`3. Hold priority: pass now, and do not ask me again - this turn or later - until one of the rows above changes (any change re-opens this window; you give up no cast)`
The reply's PLAN line is *"Let Underworld Dreams trigger resolve. Activate Sorin +1 to create a
Vampire token."* and the CHOICE is 3. Same shape again at **seq 27** (t11), at
`...vs-ai_baka_deck146.jsonl` **seq 50** (t22, rows 1/2/3 all Sorin, row 4 a Battlement mana row),
and at `...vs-ai_baka_deck152.jsonl` **seq 26** (t16, life 4, lethal crack-back printed). **4 of 4
priority-seam holds in this seat were taken on menus whose top rows were loyalty activations, and
in every one the PLAN line named the activation the CHOICE then discarded.** Lane BR's L17(a) gave
the CASTING-seam hold row an honest cost sentence; the PRIORITY seam kept `you give up no cast`,
which on these menus is the only cost sentence there is and it points at the wrong thing. Fix
symmetrically: name what the row actually suppresses (these rows, this turn), not what it does not.

**MED-2 — no surface tells the seat what Sorin's -6 does for a wall deck, and it was never taken.**
Repro `...vs-deck146.jsonl` seq 50, row 3 rendered as
`3. -6: destroy up to three and reanimate with Sorin, Lord of Innistrad targeting Triumphant Adventurer #1 [opponent's battlefield] [cost: Counters]`
The row label says "destroy up to three and reanimate"; the reanimation's *controller* — the part
that makes it a swing rather than a sweeper — is only in the trailing `{card text: ...}`, which at
that seq is truncated mid-sentence in the row (`...Return each card put into `). The primitive is
faithful (`planeswalkers.txt:3059`: `target(<upto:3>*[creature;planeswalker]|battlefield) destroy
and!(moveto(mybattlefield))!`, `text=` byte-matching Scryfall), so this is a render item, not a
card item: a loyalty row whose label is a summary should not truncate the clause that changes whose
board it is. Printed at deck146 seq 50 (their line: 9 creatures) and deck152 seq 26 (6 attackers,
seat at 4 life); taken 0 times in the corpus.

**MED-3 — `hold_windows_skipped_cast` 58 / 21 / 0 / 0 / 0 / 0 across the six games, entirely from
the two won games.** The 58 (vs125) and 21 (vs123) are windows the engine correctly suppressed
after a hold; the four games with 0 are the four where the seat answered `Cast nothing right now`
instead. That asymmetry is the L17(b)/L18(b) question restated from the deck side: the two decline
rows are still being chosen interchangeably (LOW-4 below), and only one of them buys the skip.

**LOW-4 — `NO LIVE CAST ROW ON THIS MENU` printed and the decline row was still taken.**
`...vs-deck123.jsonl` t7: the identical menu (one dead Tribute row, `they control 0 creatures - at 0
this does nothing`) was served four times — seq 9 → Hold (correct), **seq 12 → "Cast nothing right
now"**, **seq 18 → "Cast nothing right now"**, seq 24 → Hold. Two of the four re-serves were bought
by the wrong decline row. Cost is small (two windows, ~3 min) and it is a guide-compliance miss more
than an engine one, but it is the cheapest possible confirmation that the two rows still read alike.

**LOW-5 — the L15 dungeon-mode negative fired and was read correctly.** `...vs-deck146.jsonl` seq
57: `2. sacrifice cards {this mode right now: it changes no life total and draws no cards - what it
does is in the row label and the card text}` — the model took it over `1. lose 2 life` and its plan
cites the life saving. Recording as a PASS, not a defect.

**Adjudication of the wave-71 Tribute engine work, from this seat only.** Four Tributes were cast
(deck162 seq 14 → life 20→26 for a 0/6, exact; deck146 seq 37 → 13→14 and seq 49 → 15→16, both
against multi-creature boards whose row read `you gain at least 1 ... plan on the floor`; deck130
seq 14 at 5 life). **Every one paid exactly once and exactly the printed floor. No double payout
anywhere, and no `noop_row_zero_reask` fired at this seat (0 fallbacks of any kind).** But this seat
never produced the shape lane BR/BS F1/F2 fix: no window offered a second Tribute while one was on
the stack, and no `{... an effect already on the stack is aimed at that same creature ...}` clause
was rendered in any of the six games. So lane BR's L6 prediction and lane BS's F2 prediction are
**UNTESTED here** — not confirmed. What I *can* say is that the fix caused no regression in the
single-victim case: 4 of 4 correct.

## 3. Guide verdict: **EDIT** (`strategy.txt`, 19,847 B — 124 B **below** the live 19,971 B)

Three additions, each paid below; all three were funded by trimming, so the guide shrank.

**A. The hold row never covers an activated row. Paid by deck162 seq 21 + seq 27, deck146 seq 50,
deck152 seq 26 (4 of 4 priority holds in the seat, in three different games).**
after (added to `=== THE HOLD ROW ===`): `A LOYALTY ROW OF SORIN'S, A STAFF PING ROW, OR ANY OTHER ACTIVATED ROW IS A LIVE ROW, and the hold row's "you give up no cast" is not about them: taking it here throws that activation away for the rest of the turn. NEVER HOLD ON A MENU THAT PRINTS ONE. If your PLAN line names an activation, the CHOICE number on the next line is that row - a plan you answer with a pass never happens.`
The live guide's hold rule keys only on "NO LIVE CAST ROW" and on the *casting* seam; nothing in it
said an activation is a live row, and the render says the opposite (HIGH-1). The last sentence is
the cheap general form of the failure the corpus actually shows: in all four the plan and the choice
disagree, and it is always the plan that was right. Cost of the miss at deck162: Sorin resolved on
t9 and produced no Vampire before the seat died on t13, in a game whose only clock was life gain.

**B. Sorin's -6. Paid by deck146 seq 50 (row 3 printed, 9 creatures on their line, seat losing) and
deck152 seq 26 (row 3 printed at 4 life under a lethal crack-back).**
before: `Take the +1 every turn after.`
after: `Take the +1 every turn after. THE -6 ROW ("destroy up to three and reanimate") IS YOUR ONLY SWEEPER AND IT KEEPS WHAT IT KILLS - the three come back under YOUR control, so it removes their board and hands you the attackers your walls never were. The turn it is printed and three or more creatures are on their line, TAKE IT over the +1.`
The live guide names Sorin's +1 and -2 and is silent on the -6, so a deck with no sweeper and no
attacker had a printed board-swing it was never told to look for, twice, in the two games where the
opponent's board was the thing killing it. Verified against `planeswalkers.txt:3059` (MED-2).

**C. The `[DRAW PRICE]` stop, moved to where the model reads. Paid by deck162 seq 26.**
before (head of `=== WHAT TO CAST WHEN SEVERAL THINGS ARE LISTED ===`): `Take the highest entry that appears in your menu.`
after: `Take the highest entry that appears in your menu. A row tagged "[DRAW PRICE: ... you would be at X]" is barred here unless it is entry 1: that price is paid per CAST, and a wall bought with life against a punisher board is life spent on nothing.`
The rule already existed, at the bottom of the guide under `=== DRAW PUNISHERS ===`, and the model
did not apply it: at seq 26 (life 15, pair incomplete) it took a Wall of Omens row printing
`[DRAW PRICE: ... you would be at 13]` and its whole reasoning trace argues mana and defenders
without once naming the price. The rung list is the surface it demonstrably reads at every casting
window, so the stop now lives at its head; the DRAW PUNISHERS paragraph was shortened by the same
amount it gained.

**What paid for the bytes.** (1) `=== TAPPING THE BATTLEMENT BY HAND ===` collapsed from four lines
to two: `mana_only_windows_skipped` was 199/38/33/0/0/0 and **not one mana-only window reached the
model in any of the six games**, so the section's premise ("a window whose only action row is Add N
green mana") never occurred; the shape that did occur is a mana row on a mixed priority menu
(deck146 seq 50 row 4), which the shorter absolute form covers. (2) Three of the four WORKED
SITUATIONS restated rules stated in full above them; one kept. (3) `=== THE DECK ===`'s numbered
turn order restated the rung list two screens below it; replaced by a pointer. (4) The Lightmine
Field paragraph tightened, rule intact.

**What KEPT and earned it:** the rung-8 rule (both names out → Staff/Sorin to the top) closed both
the deck123 and deck125 wins (seq 32/33 and seq 34/35); LAND DROP taken 6/6 games, every land row,
including three consecutive land-only turns while flooding vs deck130; the ZERO STOP (four dead
Tributes at deck123 t7); the [defender]-hold rule (see §1's note — violated once, and I decline to
weaken it on one instance); MULLIGAN and BOTTOMING (deck125 seq 2/4, both by the book); the wave-70
"NO HEADER ANNOUNCES A COMPLETE PAIR" line (the seat correctly identified the complete pair at
deck146 seq 18-34 with no header, and correctly read the BROKEN-pair line at seq 35); the wave-70
Lantern narrowing (a Lantern was cast once, deck146 seq 42, at 8 untapped sources with nothing else
castable — rung 7 as written, not the wave-70 failure repeating).

No general-guide or skill proposal: items A-C are all deck-local, and HIGH-1/MED-2 are render fixes
for the engine seat, not guide amendments.

## 4. Not checked
I read opponent seats only where this seat's window needed context (deck130's attackers, deck162's
punisher sequencing, deck146's exile effect — I did **not** identify which deck146 card exiled
Sanguine Bond at t15, nor verify it against a primitive). I did not audit the `.stderr` files, did
not verify Rorix Bladewing, Siege-Gang Commander, Underworld Dreams, Howling Mine, Dictate of
Kruphix, Ob Nixilis or Elite Spellbinder against their primitives (no decision here turned on their
printed text), and did not census `hold_windows_skipped` window-by-window for a wrong *skip* (only
for wrong takes). The Tribute engine fix is adjudicated above as UNTESTED for its double-pay case,
not as passing.
