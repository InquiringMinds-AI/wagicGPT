# Wave 73 — deck 125 (Revelation Fracture, UW draw-go control)

Seat files: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260907-225637-final/*ai_baka_deck125-*`
(all six are RERUNS except vs162, which is from the original run). **1 W / 5 L** (wave 72: 2/4).
547 model decisions, reasoning on for all, `plan_line_missing` 13, 0 fallbacks, 0 `reply_truncated`,
0 `action_before_plan`, 0 `ask_replays_refused`, 0 `sibling_window_asks_skipped`,
0 `stop_reached_windows_skipped`, 0 `chain_windows_collapsed`, `async_drops` 8,
`phase2_answer_recovered` 1, `hold_released_turn` 32. No hang, crash or timeout.

**The loop's question — can this guide close a game it has stabilised?** The clock can: in vs123 and
vs126 the Staff of Nin took the opponent from 20 to 0 and from 20 to 2 with no help. What the guide
cannot do is *keep* the closer. In both stall games the seat repeatedly spent its whole mana on a
max-X Sphinx's Revelation on its own turn, which (a) left nothing for the second Staff of Nin sitting
in its hand and (b) overfilled the hand so that the Staff was pitched at cleanup. It then abandoned
the clock outright: vs126 was lost at **131 life against an opponent on 2** because the seat cast
Emrakul into a printed converter + loop, the one line its guide forbids in capitals.

## 1. Game-by-game

| opp | result | turns | end life | the decision that decided it |
|---|---|---|---|---|
| 162 | LOSS | 13 | 0–19 | **seq 49**, t13, 17 life, `CRACK-BACK NEXT TURN … you would be at 12`. Row 1 was Final Judgment exiling Master of the Feast (5/5 flying); the seat took row 3, Staff of Nin, `casting this taps you out`. Gate shape (d)'s crack-back exception is written at "life 15 or less" and the seat was at 17, so the guide **permitted** it. Dead on t14. §C-1 |
| 130 | LOSS | 15 | −1–18 | Variance. 10 decisions in 16 turns; at t13 the battlefield line reads **1 land** after four land drops (land destruction). No window the guide could have answered differently |
| 123 | **WIN** | 76 | 29–0 | Stabilised by t13 (opp on 16) and then did **nothing for 44 turns**. `Cast Staff of Nin` was printed exactly three times all game (seq 279 t57, 364/376 t69). At **seq 261** (t51 cleanup) it discarded a Staff of Nin with **no Staff on its battlefield**; at **seq 276** (t55) it discarded Emrakul with **15 lands** on its battlefield. Both are the *guarded* cleanup steps, taken as if unguarded. §B-1 |
| 126 | LOSS | 62 | −14–152 | **seq 453**, t63, **131 life vs 2**. Both the `LIFE-TO-DAMAGE CONVERTER … theirs - Sanguine Bond` paragraph and the `LOOP COMPLETE` paragraph were printed; the seat cast Emrakul. Opponent answered with Tribute to Hunger ({2}{B} instant, sacrifice), gained 15, and the loop ran 9 links: 2→137 and 131→11→dead. §B-2. The 36 turns before it are §B-3 |
| 146 | LOSS | 17 | −3–19 | **seq 90**, t15, 16 life, `CRACK-BACK … you would be at 9 or lower`; Staff of Nin `casting this taps you out` taken over the Revelation on row 1. 16 → 5 on t17. Same shape as vs162, same one-point-too-tight threshold. §C-1 |
| 152 | LOSS | 21 | −4–49 | Hand at 1 card from t17; the last window (seq 20, t20) is a correct Fall of the Gavel under `INCOMING … that would KILL you`. Variance |

**Wave-72 edits, checked.**
- **#1 (own-turn hold row) PAID, biggest single move of the wave.** 194 own-turn pass windows: 58 hold
  / 136 "Cast nothing". Of the 58 holds, **42 carried the `[you declined this exact list N times …]`
  override the guide allows**, leaving **16 real violations (8%)** against wave 72's 80/121 (66%).
  KEEP verbatim.
- **#2 (Revelation stops are not heuristics) HALF-PAID.** Stop (1) held perfectly: 11 Revelation cast
  rows under a printed DRAW PUNISHERS paragraph in vs162, **0 taken** (wave 72's seq-16 miss does not
  recur). Stop (3) did **not**: five casts with a Staff on the battlefield and the opponent on 10 or
  less — vs126 seq 294 (opp 10), 298 (10), 308 (9), 429 (4); vs123 seq 428 (opp 4). §B-3.
- **#3 (X menu decline is not free) UNTESTED** — no X menu was declined this corpus.
- **#4 (Lightmine second copy / N of 0) HOLDS** — the only Lightmine cast (vs123 seq 399) was at
  N=103 under the survival clause, and it is why the seat lived past t71 to win.
- **#5 / wave-70 #5 (Emrakul with a Staff out) FAILED** — see §B-2; the same "nothing else to do"
  override wave 72 patched inside the REVELATION section reappeared verbatim inside EMRAKUL.

## 2. Engine / interface / card items

**HIGH — A-1. A Revelation cast row cannot tell the reader what it forecloses, and the seat planned
around mana the cast had already spent.** `…deck125-…-vs-ai_baka_deck126.jsonl` **seq 308**, t49,
91–9, 17 untapped sources, Staff of Nin in hand. The row prints
`{no {leaves ...} count on this row: what it spends depends on the X you announce at the next window}`.
The model's own PLAN is *"Cast Sphinx's Revelation for X=14 now. In main phase, cast Staff of Nin,
then ping opponent 9 times to win."* X=14 spends all 17; the Staff was never castable; at seq 317
the same Staff was discarded at cleanup. Every other cast row in this engine already prints
`{spends K of your M untapped mana sources this turn; <card> in your hand needs N}` — the Revelation
row is the one row that does not, and it is the row where the arithmetic is hardest. **Repro:** print
on the Revelation cast row the largest X that still leaves the cheapest castable card in hand payable,
in the same `<card> needs N` grammar. The same shape recurs at seq 211/215, 250/253, 294/297, 429/432.

**HIGH — A-2. The `LOOP COMPLETE` / converter paragraphs are correct, complete, prominent — and the
model overrode them because no row on the menu was an alternative to losing.** vs126 **seq 453**. The
prompt carries both paragraphs and the model's `reasoning` quotes them back four separate times
(*"This explicitly says attacking (which causes life loss) is fatal. But I have no other win
condition."*) while a Staff of its own had the opponent on 2 and every prior upkeep had pinged. Nothing
on that menu said **"your Staff of Nin wins in two more upkeeps"**. The board frame prints
`Opponent life trend`, and the Staff ping row prints `they would be at N`, but the *pass* rows print
nothing about the clock the seat already owns. Cheap engine fix with a large blast radius for every
inevitability deck: when the seat controls a repeatable damage source and no lethal is incoming, tag
the decline/hold row `{your Staff of Nin #1-#N reach 0 in K of your upkeeps at this rate}`. Note this
is a PERCEPTION failure at the *option-set* level, not a false fact: the guide half is §3 edit 1.

**MED — A-3. The declined-count override lost 40% of its windows — its worst wave yet.** 136 windows
carried `[you declined this exact list N times already this turn]`; **55 (40%) were answered
"Cast nothing right now"**, the row the guide names as the wrong one. Wave 70 ≈20%, wave 71 26%,
wave 72 24%, wave 73 **40%**. Three waves of guide capitals have not moved it and it is now moving the
wrong way; the wave-70/71/72 engine suggestion stands and should be taken: when that clause prints,
**do not print the plain decline row at all**, or tag it `(this same question will be asked again this
turn)`. Repro: `…vs-ai_baka_deck123.jsonl` **seq 377** (t69 Main phase 1, clause reads
`[you declined this exact list 3 times already this turn - counted over every window this turn, not
only the ones asked at this same seam]`, answer "Cast nothing right now"); also
`…vs-ai_baka_deck146.jsonl` seq 44 (t9 End, 3 times) and seq 82 (t13 End, 2 times).

**MED — A-4. `K of them able to attack right now` counts creatures the same screen says can never
attack.** `…vs-ai_baka_deck123.jsonl` **seq 399**, t71: header
`Opponent battlefield (125 permanents listed, of which 103 are creatures, 103 of them without a
restriction against attacking …)` while every one of the 103 is listed
`Human #1-#102 (1/1) [doesn't untap during its controller's untap step] [tapped - cannot attack or
block this turn]` under two Intruder Alarms. Two bracketed tags on the same line say the creature is
tapped and will never untap; K counts it anyway. The guide has had to carry a compensating sentence
(`a [tapped] creature is still counted`) since wave 70, and this corpus falsified the other half of
it (`the row counted them and they untap`) — I deleted that clause in §3. Correct fix is engine-side:
subtract from K any creature that is tapped **and** carries a no-untap effect.

**MED — A-5. Row-per-instance enumeration puts 20-35 KB prompts in front of a 1-damage decision.**
vs123 seq 418 is **35,047 bytes**; the Supreme Verdict row alone names all 103 Humans with their full
tag string, and seq 399's Lightmine row names all 103 again under `DIES at 103 (before prevention)`.
vs126 averages **20,433 bytes per decision** across 226 decisions. The `1-102.` range collapse already
exists for the ping rows and is exactly the right shape; the verdict tails on sweeper and Lightmine
rows should use it too (`DIES at 103: Thraben Doomsayer (2/2); Human #1-#102 (3/3) x102`).

**LOW — A-6. The wave-73 HOLD expiry works on this seat.** All 32 `hold_released_turn` releases land
at the holder's next untap; the largest observed gap between a hold and the seat's next window is
**2 turns** (vs123 seq 292 t59 → seq 293 t61; vs126 seq 425 t59 → 426 t61), i.e. the rest of the
seat's own turn plus the opponent's turn — never more. Wave 72's HIGH A-2 (a hold that survived four
turns while life went 18 → 6) does **not** recur. The residual cost is real but bounded and the guide
already forbids the own-turn hold.

**LOW — A-7. Cards verified against the primitives, all faithful to Oracle, no defect found:**
`Emrakul, the Aeons Torn` (mtg.txt:35731 — `autostack=if casted(this) then turns:+1 controller`,
`autograveyard=… moveTo(ownerlibrary) all(*|ownergraveyard) && shuffle`, annihilator 6 on
`_ATTACKING_`), `Tribute to Hunger` (124073 — `dynamicability<!mytgt toughnesslifegain targetopponent!>`,
i.e. the 15 life the seat handed away), `Sanguine Bond` (99004), `Staff of Nin` (112425),
`Sphinx's Revelation` (110851). The vs126 t63 sequence (Emrakul dies to Tribute → opponent gains 15
→ `[x9]` chain → 2→137 / 131→11) is the rules-correct behaviour of that board.

## 3. Guide verdict: **EDIT** (`wave73/deck125/strategy.txt`, **19,979 bytes** against the live guide's
19,999 — it **SHRINKS by 20 bytes**; every added rule is paid for out of rationale trims listed as
edit 5, so no rule has to earn new bytes.)

| # | before → after | paid by |
|---|---|---|
| 1 | EMRAKUL, appended: `HAVING NOTHING ELSE TO DO IS NOT AN EXCEPTION, AND NEITHER IS THEIR LIFE BEING LOW. A Staff already pinging IS the game ending; a {2} instant that makes you sacrifice a creature hands its controller 15 life, and under a converter that 15 comes straight back off you and chains. Take the Staff ping and the hold row instead, however many turns that reads.` | **vs126 seq 453** (§B-2/A-2). The section already said "at any mana and any life" and named the sacrifice instant and the converter; what it lacked was the sentence wave 72 had to add to REVELATION for the identical override. The model's `reasoning`: *"I have no other play"*, *"It's the only card that advances the game"* — at 131 life with the opponent on 2 and its own Staff untapped. This is the one edit paid for by a decision that lost a won game |
| 2 | CLEANUP DISCARD, after "Keep counterspells you can pay for, and a Path": + `A STEP WHOSE CONDITION IS FALSE IS NOT A STEP: read each step's clause against the board before you count it, and where the clause does not hold, skip that step entirely and read on.` | **vs123 seq 261 and 276.** At seq 261 the model's `reasoning` enumerates *"Step 6: Staff of Nin (#13)"* with no Staff on its battlefield line — step 6 reads "ONLY when one is already on your battlefield line". At seq 276 it discarded Emrakul with `15 are lands` printed on its own battlefield line — step 4 reads "unless your battlefield line prints 13 or more lands". Both guards were skipped the same way, and both cards are this deck's only two ways to end a game |
| 3 | STAFF OF NIN, after the second-copy tag sentence: + `A SECOND STAFF DOUBLES THE CLOCK AND IT IS HOW A STABILISED GAME ENDS: while fewer than two are on your battlefield line and one is in your hand, its cast row outranks a Sphinx's Revelation for the same mana in the same window, and you never send it at cleanup.` and CLEANUP step 6 `ONLY when one is already on your battlefield line` → `ONLY while TWO are already on your battlefield line` | **vs126, the whole 36-turn tail.** The seat held exactly ONE Staff from t27 to t63 (verified on the battlefield line at seq 215, 253, 317, 453) while discarding a Staff from hand at seq 215 (t37), 253 (t41), 317 (t49) and twice more in the t63 log, with 14→21 untapped lands. `Cast Staff of Nin` was never printed again after t27 because every one of those turns had already been spent on a max-X Revelation (seq 211, 250, 294, 298, 308, 429). One Staff needed 36 turns for 18 damage; two would have needed 18. Same shape in vs123 seq 257→261 |
| 4 | TAP-OUT GATE shape (d): `neither is a first one while "CRACK-BACK NEXT TURN" is printed and your life is 15 or less` → `neither is a first one while a "CRACK-BACK NEXT TURN" line reads "you would be at" 12 or less` | **vs162 seq 49** (17 life, line reads `you would be at 12`, Final Judgment on row 1 exiling a 5/5 flier, dead t14) and **vs146 seq 90** (16 life, line reads `you would be at 9 or lower`, dead t18). Both were *guide-compliant* at a flat 15-life threshold and both are the whole game. The forecast the engine already computes is the right variable: it prices the tap-out in the same units as the exception. Wave 72 recorded this exception as UNTESTED for a second wave; it is tested now, and it was one point short in both trials |
| 5 | Byte payers, no decision rule lost: the Elixir "only life this deck gains without spending a card" clause, PATH's "the basic land it hands them is worth paying", the K-IS-0 tail after "whatever your life reads", MULLIGAN's "Your job is REACTIVE …" and the ONE LAND rationale, PUNISHER BOARDS' parenthetical heading and the DISCARD-PUNISHER "UNDERSTATES the bill" tail, the X-menu "prints largest-first and may collapse its middle", and STAFF's "each Staff draws you one more card each upkeep" (edit 3 now states the same fact as a rule). **One deletion is a correction, not a trim**: LIGHTMINE's `"[tapped]" is no reason to wait - the row counted them and they untap` is FALSE on the vs123 board (§A-4) and is gone. Also rephrased the one line quoting the literal plan-line label | bytes + §A-4 |

KEEP, explicitly: the own-turn hold rule (edit #1 of wave 72 — 66% → 8% violation, the best-performing
rule in this guide); Revelation stop (1) (11 offers under a printed DRAW PUNISHERS paragraph, 0 taken);
the counterspell type/power table (every counter spent this corpus was on-list, including the vs126
Dream Fracture on the opponent's own Staff of Nin); LAND DROPS (66 land-drop asks, 66 lands played,
tapped ones included); the Lightmine survival clause (vs123 seq 399, cast at N=103 and 6 life, the
reason the seat lived to win); PATH's ordered list; the `K IS 0` sweeper bullet; MULLIGAN.

One non-compliance I am **not** editing for: 38 of 42 Staff ping windows were taken; the one real miss
is vs126 **seq 422** (t59 upkeep, opponent on 5, a face-ping row printed, the HOLD row taken instead).
That is already two existing rules — ping every window, and never hold on your own turn — failing
together in one record, and a third sentence would not add a rule.

## 4. Not checked
Opponent seats were read only for the vs126 t63 chain. I did not audit the `reveal`, `attackers` or
`blocks` seams (this deck fielded no creature until the vs126 Emrakul, which died before combat), did
not verify any card against Scryfall (the five above were checked against the primitive text only),
did not trace the 32 `hold_released_turn` releases individually past their turn gaps, did not explain
the corpus-wide `sibling_window_asks_skipped 0` / `chain_windows_collapsed 0` /
`stop_reached_windows_skipped 0` zeros (all zero on this seat too), did not measure
`main_phase_windows_skipped` (79 across these six games) against the code, and adjudicated none of the
lane-{BY,BZ,CA,CB} predictions — those are the engine seat's job. No general-guide or skill proposals:
edits 1-3 are all the same failure (a guarded rule read as unguarded) but the guards are this deck's
own card names, and the general guide has no cleanup or win-condition section to carry them.
