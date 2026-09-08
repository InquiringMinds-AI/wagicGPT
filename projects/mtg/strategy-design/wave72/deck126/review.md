# Wave-72 deck126 review — SANGUINE BLOOD (WGB wall prison, Exquisite Blood + Sanguine Bond)

Corpus `matchups-20260907-163626`, all six seat files `...ai_baka_deck126-<ptr>-vs-...`.
Record **5/6** (wave 71: 3/6; wave 70: 2/6). 219 model decisions at this seat: **0 fallbacks, 0
`off_protocol_bytes`, 0 `plan_line_missing`, 0 `action_before_plan`, 0 `reply_truncated`, 0
`reasoning_chars == 0`, 0 `ask_replays_refused`, 0 `menu_pass_no_progress`, 0 `wall_miss_events`, 0
`declined_face_latches`.** Every reply was exactly PLAN + action. `reasoning_chars` p50 5,655 / p95
10,142 / max 19,924; latency p50 86.5 s / p95 172.8 s / max 465.1 s. One `async_drops` (vs 125), one
`phase2_answer_recovered` (vs 162). `ask_replays_reserved` 11/5/54/1/0/0 across the six, refused 0 —
the sidecar absorbed every repeat-ask run in this seat and cost no model call.

## 1. Game by game

| # | seat file (epoch) | opp | result | deciding decisions |
|---|---|---|---|---|
| 1 | 1788816997 | 162 | **WON** 28–0, t17 | Textbook rung list: Tutor→Bond (seq 8/9), Tutor→Blood (seq 12/13), Bond seq 22 (t13), Blood seq 25 (t15), Staff seq 28 (t17) and the ping at seq 30 closed it from 21 — the rung-8 rule (both names out → Staff/Sorin to the top) paying for the third wave running. Two Tributes cast correctly (seq 16 at "2 creatures - they choose", seq 18 naming Fate Unraveler 3/4); the [defender]-hold rule respected twice at t7 (Fog Bank named, life 20, held). |
| 2 | 1788816999 | 130 | **WON** 31–0, t21 | Sorin t7 (seq 10) then +1 on **every** subsequent turn (seq 11, 14, 19, 21, 25, 27) — the wave-71 hold-row edit visibly working. -2 emblem taken at seq 16 (t11) against a Goblin board that kills 1/1s, exactly the guide's trigger. Bond t17 (seq 24), Blood t19 (seq 26), 2/1 lifelink Vampire attacks t21 (seq 41) and the loop ends it. |
| 3 | 1788817001 | 146 | **WON** 29 to −2, t17 | Pair completed under pressure: Bond seq 15 (t12), Blood seq 16 (t14, life 11 under a crack-back), Sorin seq 17 (t16, life 8), then the **block** at seq 20 (t17, life 8, Pride Guardian on an indestructible Silverquill Silencer) fired the loop for the win — the guide's "ONCE AN ENCHANTMENT OF YOURS IS OUT, STEP 1 IS OFF AND EVERY BLOCKER BLOCKS" line, cashed. seq 15 is the guide contradiction in §3-B: the seat cast the **Silencer-tagged** Bond for 3 life over the untagged Blood, against the letter of the guide and **rightly so**. |
| 4 | 1788817015 | 123 | **WON** 39–0, t15 | Bottomed a duplicate Overgrown Battlement (seq 4, by the book), Tutor→Bond seq 9/10, Bond seq 24 (t11), Blood seq 27 (t13), and at seq 28 (t15) a Tribute into a **24-creature** board with both names out — the win button, taken in the first window that offered it. Five dead-Tribute windows at t7-t8 (`at 0 this does nothing`) all declined; two took `Cast nothing right now` over the hold row (wave-71 LOW-4 recurring, but the replay sidecar reserved all 5 re-serves, so the cost was zero calls this time). |
| 5 | 1788817017 | 125 | **WON** 50–0, t32 | Long grind against a lifegain/wrath deck (opp peaked at 31). Bond t16 (seq 47), Blood only t30 (seq 128 — the Tutor was not drawn until t30; variance, not guide). **seq 141/142 (t30) is the misplay and it cost two turns** (§3-A): both names out, Lightmine Field on their line, the seat sent its lone 1/1 Vampire, it died on the declaration, no lifelink, no loop. The Staff ping at t32 won instead. |
| 6 | 1788817028 | 152 | **LOST** −8 to 20, t15 | Two Elite Spellbinders exiled the Tutor (t5) and Sorin (t7) out of hand, then attacked as 5/3 **fliers** pumped by two Intrepid Adversaries. Sixteen defenders and no flier answer: **zero** blockers asks in the whole game, life 21→18→10→2→dead. The one Tribute the seat drew was spent correctly at t6 killing a Spellbinder. Not a decision game — with one exception, seq 16 (§2 MED-2): the land drop was **declined** at 2 life on a hallucinated hand. |

## 2. Engine / interface / card items

**MED-1 — the priority hold row's new activation sentence works, but "second main phase" is the hole
it leaves.** Repro `...vs-ai_baka_deck125.jsonl` **seq 36** (t14) and **seq 101** (t24). Row 4 now
reads `... (any change re-opens this window; the rows above include ACTIVATED abilities that are
usable RIGHT NOW, and taking this row gives every one of them up for as long as these rows stand)`
and, inside the brace, `A pass row that differs only by naming which step comes next is the same
row, so a hold taken in your first main phase also covers your second main phase while these rows
do not change`. At seq 36 the reply is `PLAN: Let Essence Scatter resolve, then activate Sorin +1 to
create a Vampire token, then attack with it in combat. / CHOICE: 4 (Hold priority)`; at seq 101,
`PLAN: ... where I will cast Idyllic Tutor ... / CHOICE: 0 (Hold priority)`. Both plans defer the
activation to a later window this turn; the sentence that forbids that is buried ~600 characters
into the brace, after the row's own headline cost clause. **The headline clause names the cost but
not the deferral**; the model reads the headline. Suggested fix: hoist "and you cannot take it later
this turn" into the parenthesis beside "gives every one of them up".

**MED-2 — nothing in the prompt contradicts a hallucinated hand, and at seq 16 that cost the land
drop.** Repro `...vs-ai_baka_deck152.jsonl` **seq 16** (t14, life 2). The prompt states
`Your hand (2 cards): Savannah (land ...); Scrubland (land ...)`. The `reasoning` field reads
*"I can cast Exquisite Blood right now! {4}{B} is available"*, then *"PLAN: Play no land. Cast
Exquisite Blood {4}{B}. Activate Staff of Nin targeting opponent ..."* — neither card is in hand, on
either battlefield or anywhere else; the seat has never controlled a Staff in this game. The reply
took `3 (Play no land right now)` — the **only** land-drop decline in 49 land windows across the six
games. The very next window (seq 18) carries the plan-staleness note machinery for named cards, and
the attackers seam prints a `(note: your plan names "X", and no "X" is in your hand ...)` block
(seen at deck130 seq 22 and deck125 seq 32) — **the land-drop seam does not**, so the false premise
was never challenged before it was acted on. Cheap fix: run the same plan-vs-board name check on the
land-drop ask. Material cost here was one land (the seat was dead on board either way), but the
failure mode is general.

**LOW-3 — `hold_windows_skipped` is still all-or-nothing per game: 0 / 0 / 0 / 25 / 441 / 0.** Same
asymmetry the wave-71 review flagged: the two long games did all the skipping, and the four short
ones took `Cast nothing right now` where the hold row was the closing answer. With the `askreplay/`
sidecar now reserving repeats (11/5/54/1 in this seat, 0 refusals) the *cost* of the wrong decline
row has collapsed to near zero, which is why I am not paying a guide edit for it this wave.

**LOW-4 — `CHOICE: 0` is legal at the priority seam and is NOT the hold row.** `...vs-deck125.jsonl`
seq 141: the instruction line reads `CHOICE: followed by the number (0 = pass priority)`, the
printed rows are 1-4, and the reply `CHOICE: 0 (pass)` was executed as a bare pass — so the same
menu was re-served at seq 143/144/153. Not a defect (the casting seam even prints
`This menu has no row 0`), but worth recording as the reason a priority window can repeat after an
apparent "hold". At seq 101 the model also wrote `CHOICE: 0` while naming `Hold priority`, and the
engine correctly resolved by **name** to row 4 — the name-over-number rule doing exactly its job.

**Card checks.** `Lightmine Field` (`mtg.txt:67058`): `auto=@each blockers:foreach(creature[attacking]|Battlefield) damage:1 all(creature[attacking]|Battlefield)`, `text=Whenever one or more creatures attack, Lightmine Field deals damage to each of those creatures equal to the number of attacking creatures.` — faithful to Oracle, and the deck125 t30 log confirms the timing the render claims (`Phase: Blockers / Opponent's Lightmine Field dealt 1 damage to Vampire / Your Vampire died`, before any combat damage). `Silverquill Silencer` (`borderline.txt:102764`): text and `life:-3 opponent && draw:1 controller` match Oracle. No render clause in this seat stated a false fact.

**Adjudication of lane BV's M6, from this seat only.** 42 windows in these six games printed a
numbered `+N:` / `-N:` loyalty row or a `Deal 1 damage with Staff` row **beside** the hold row. The
hold (or row-0 pass) was taken in **4 of 42 — 9.5%, against 4 of 4 in wave 71** at this same seat.
**PASS** on the rate half of the prediction. **FAIL** on the second half ("no gameend shows a hold
taken on a menu whose reply PLAN names one of its own activated rows"): deck125 **seq 36** and
**seq 101** are exactly that shape, and deck130 seq 40's plan named an attack rather than a row, so
the failure is 2 of 42, not 0. See MED-1 for why.

## 3. Guide verdict: **EDIT** (`strategy.txt`, 19,995 B — 148 B above the live 19,847 B, ceiling 20,000)

Three additions; two sections cut to pay for them.

**A. The win button does not outrank Lightmine Field. Paid by `...vs-deck125.jsonl` seq 141 → seq
142 (t30).** At seq 141 both names were on the seat's line, the header read *"so any one point of
either, from any source, ends the game in your favour"*, row 1 was Sorin `+1`, **row 2 was the `-2`
emblem**, and the seat held. At seq 142 the ATTACK TOTAL header printed *"Lightmine Field fires on
the declaration, before any combat damage, and can kill your attackers first"*; the seat sent its
one 1/1 anyway. Log: `Opponent's Lightmine Field dealt 1 damage to Vampire / Your Vampire died` —
no damage, no lifelink, no life gain, no loop, and the Vampire gone. The `-2` emblem (row 2, one
window earlier) makes that Vampire a 2/1 that **survives** a one-attacker Lightmine trigger, so the
guide's own Lightmine paragraph already had the winning line; the WIN BUTTON override told the model
to ignore it. The win came two turns later off a Staff ping.
before: `THE WIN BUTTON OUTRANKS EVERY STOP: with BOTH names on your battlefield send EVERY Vampire whatever the tag says ...`
after: `THE WIN BUTTON OUTRANKS EVERY STOP BUT THE LIGHTMINE ONE: with BOTH names on your battlefield send EVERY Vampire whatever the tag says ...`
before (end of the Lightmine paragraph): `... ATTACK: none until that permanent leaves their line.`
after: `... ATTACK: none until that permanent leaves their line. THIS ONE STOP HOLDS WITH BOTH NAMES OUT: a Vampire that dies on the declaration deals nothing and gains nothing, so no loop starts - take the emblem first, or win off a Staff ping or a Tribute instead.`

**B. The Silencer tag and which half to cast. Paid by `...vs-deck146.jsonl` seq 15 (t12, life 20).**
Row 1 was `Cast Exquisite Blood {4}{b}` (untagged, taps out), row 3 `Cast Sanguine Bond {3}{b}{b}
[NAMED BY THEIR Silverquill Silencer: casting this costs you 3 life and draws them a card - you
would be at 17 ...]`. The live guide's Silencer section says flatly *"BOTH HALVES ON ONE MENU, ONE
ROW CARRYING THAT PRICE: cast the UNTAGGED half"*, which here means casting Blood — a card its own
`=== ASSEMBLING THE PAIR ===` section calls out as **"EXQUISITE BLOOD ALONE DOES NOTHING YOU CAN
START"**, while the Bond alone is a clock. The two rules contradict each other whenever the Silencer
happens to have named the Bond. The seat cast the Bond, paid 3, and **won the game at t17**; the
guide's letter would have taken the dead half at the same price in tempo. Narrowing the Silencer
rule so it cannot beach the deck on its inert half.
before: `BOTH HALVES ON ONE MENU, ONE ROW CARRYING THAT PRICE: cast the UNTAGGED half.`
after: `BOTH HALVES ON ONE MENU, ONE ROW CARRYING THAT PRICE: cast the UNTAGGED half - unless the untagged one is EXQUISITE BLOOD and the tagged one is SANGUINE BOND, and then above 12 life you pay the 3 and cast the BOND: Blood alone starts nothing.`

**C. The hold row cannot be deferred to the second main. Paid by `...vs-deck125.jsonl` seq 36 (t14)
and seq 101 (t24).** The wave-71 edit ("NEVER HOLD ON A MENU THAT PRINTS ONE") took the seat from
4/4 wrong to 4/42, and the two remaining PLAN-names-the-activation failures share one sentence:
*"then activate Sorin +1"* / *"where I will activate Sorin's +1 ... in second main phase"*. The model
is not overriding the rule, it believes the window comes back. The row's own brace says it does not.
before: `... a plan you answer with a pass never happens.`
after: `... a plan you answer with a pass never happens. "I will activate it in my second main" is not open to you: the row itself says a hold taken in your first main covers your second while these rows stand.`

**What paid for the bytes.** (1) `=== TAPPING THE BATTLEMENT BY HAND ===` (224 B) **deleted**: the
string `green mana with Overgrown Battlement` appears in **0** of the 219 prompts this wave, as it
did in 0 of wave 71's 157 — twelve games with zero exposure, and the Battlement's mana accounting
that *is* exposed lives in `=== MANA ===`, untouched. (2) `=== WORKED SITUATIONS ===` (191 B)
**deleted**: its single surviving line restated `=== THE WIN BUTTON ===`'s *"A PRICE ON THE ROW IS
NOT A REASON TO DECLINE IT ... while its own 'you would be at N' is 1 or more"* two screens above it.

**What KEPT and earned it.** LAND DROP: 48 of 49 windows took a land (the one decline is MED-2's
hallucination, not a rule failure). THE ZERO STOP: 30+ windows printed `at 0 this does nothing` and
the row was never cast. The wave-71 hold-row edit: §2's 4/42. The rung-8 rule (both names out →
Staff/Sorin to the top) closed the deck162 win outright. The [defender]-hold rule held twice at
deck162 t7 (Fog Bank named, life 20) and the seat won that game. Rung 1 ("the turn either half
appears, CAST IT") was obeyed in all six games — no half sat in hand behind a wall. BLOCKING's
"enchantment out → every blocker blocks" line won the deck146 game at 8 life. Reveal picks: 7 of 7
correct (Bond when holding neither, Blood when the Bond was already there). MULLIGAN/BOTTOMING: 2
decisions, both by the book.

No general-guide or skill proposal. A-C are deck-local; MED-1 and MED-2 are render/prompt fixes that
belong to the engine seat, and neither restates an existing amendment.

## 4. Not checked
I did not audit the `.stderr` files (so I contribute nothing on the docketed {X}-announcement
payment loss). I read opponent seats only for context on deck152's flier clock and deck125's
Lightmine Field / Supreme Verdict timing; I did not trace deck125's or deck146's own decisions. I
verified only Lightmine Field and Silverquill Silencer against primitives — Elite Spellbinder,
Intrepid Adversary, Fate Unraveler, Fog Bank, Bloodline Keeper, Siege-Gang Commander, Rorix
Bladewing and Nadaar were **not** verified (no decision here turned on their printed text beyond
what the render stated). On the corpus-level zeros the brief asks about: `sibling_window_asks_skipped`
and `stop_reached_windows_skipped` are **0 in all six of my games**, and I did **not** read the
engine code to say which of the three causes explains it — I can only report that the shape a stop
collapse would cover (the same priority menu re-served five windows running, deck125 t30 seq
141/143/144/153, `hold check: every row above was also on the menu at the last window at this seam
(5 windows in a row now)`) did occur and was not collapsed. `own_turn_windows_skipped` was
0/26/9/0/37/0 here; I did not verify what those skips suppressed.
