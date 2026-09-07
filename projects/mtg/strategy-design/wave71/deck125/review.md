# Wave 71 — deck 125 (Revelation Fracture, UW draw-go control)

Seat files: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260907-085638/*ai_baka_deck125-*`.
461 model decisions across 6 games (3 W / 3 L), reasoning on for all of them. 3 of the corpus's 4
`noop_row_zero_reask` fallbacks are in this seat and **all three are genuine, not false fires**
(§2 A-1). 25 records (5.4%) carry `plan_line_missing` and/or `off_protocol_bytes>0`; 0
`action_before_plan`, 0 `reply_truncated`, 0 `ask_replays_refused`, `menu_pass_no_progress` 0,
`async_drops` 3 (2 in vs130, 1 in vs123), `identical_ask_answers_reserved` 9. No game hung.

## 1. Game-by-game

| opp | result | turns | end life | the decision that decided it |
|---|---|---|---|---|
| 152 | **WIN** | 34 | 7–0 | **seq 15** (t12, 17 life) declined a free kill on a 3/1 flier — see G-1; won anyway from 2 life |
| 130 | **WIN** | 71 | 67–0 | uncontested Staff grind; Elixir activated at library 12 (seq 152), no deck-out |
| 123 | **WIN** | 49 | 65–0 | same; **seq 126** declined Emrakul citing the wave-70 rule verbatim (that edit PAID) |
| 162 | **LOSS** | 13 | 0–20 | Howling Mine t4 + Underworld Dreams t6/t12 + Dictate of Kruphix; the seat held **no counterspell at all** on t4, t6 or t12 and was never offered a window on those turns. Variance, not guide. |
| 126 | **LOSS** | 18 | 0–40 | Sanguine Bond + Exquisite Blood loop; hand at t17 was 4× Revelation + Staff + Final Judgment + Elixir, **zero counterspells all game**. Variance. |
| 146 | **LOSS** | 35 | −3–13 | hand shrank to 2 then 1 card by t30 with 12–13 lands out; never drew a Staff or a Revelation in 35 turns. Every cast it made was on-guide. Variance. |

Wave-70 edits, checked: **#5 (Emrakul)** paid — vs123 seq 126 reasoning quotes it and declines.
**#3 (animating lands)** is now largely carried by the render itself (wave-71 lane BR): the opponent
header prints `1 of those noncreature permanents can animate into a creature and is in no creature
count above` and CRACK-BACK now *adds the rungs up* (`the total to subtract from your life is 5 and
you would be at 6`) — wave 70's E-2/E-3 are FIXED. **#4 (ping)** backfired, see G-1. **#2 (shape (d)
crack-back exception)** UNTESTED: no Staff cast this corpus met its trigger (life ≤15 with a
CRACK-BACK line). **#1 (plan paragraph)** half-paid, see E-3.

## 2. Engine / interface / card items

**HIGH — A-1. The noop re-ask does not change the answer; it just spends a round-trip.**
All 3 fallbacks were the model choosing a row whose own verdict reads zero.
- `…deck125-…-vs-ai_baka_deck123.jsonl` seq 7 → `<refused: noop_row_zero_reask>`, row
  `1. Cast Lightmine Field {2}{w}{w} {right now: they control 0 creatures able to attack - deals 0
  until they have an attacker} {leaves 0 of your 4 untapped mana sources untapped - casting this taps
  you out}`. The re-ask at **seq 8 was answered identically and ACCEPTED** — the tap-out went through.
- `…vs-ai_baka_deck152.jsonl` seq 32 (same Lightmine row at N=0) → re-ask seq 33 took a *different*
  wrong row (second Staff of Nin for {8}, tapping out at 11 life with Essence Scatter in hand).
- `…vs-ai_baka_deck152.jsonl` seq 64 (`Cast Supreme Verdict … {right now: destroys 0 of their
  creatures}` at 2 life) → re-ask seq 65 corrected to `Cast nothing right now`.
1 of 3 re-asks recovered. The re-ask re-serves the identical prompt with no statement of *why* the
answer was refused, so the model re-derives the same choice. Cheapest fix: the re-ask prompt should
name the refused row and its zero verdict (`row 1 was refused: its own verdict reads 0`), or the
engine should treat a second identical noop answer as the model's considered choice and log it
separately from a first-pass slip.

**MED — A-2. `[LOOP RUNNING]` prints before the loop can hurt, and the converter paragraph tells the
reader everything except what to do.** `…vs-ai_baka_deck126.jsonl` seq 26 (t17, 20–20) rendered the
full Sanguine Bond + Exquisite Blood paragraph ending `ANY nonzero payment on a tag above is fatal,
not merely expensive` plus `[LOOP RUNNING: both halves of the opponent's life LOOP are on the
battlefield and the chain is live … Nothing on this list stops the chain.]`. Deck 125 had no
counterspell and no enchantment removal; the render is accurate and there was no answer on the menu.
The gap is upstream: the paragraph never says *the loop starts on any life YOU lose*, which is why a
Staff of Nin ping from the opponent on t18 (deck126 seat, seq 35) took this seat from 20 to 0 and
the opponent from 20 to 40 in one chain. A single added clause — `their Staff/burn/combat damage
starts it too` — would let a reader price its remaining turns.

**MED — A-3. The `YOUR PLAN` echo class doubled after the wave-70 guide edit that quoted the label.**
10 of 25 flagged replies open with the literal `YOUR PLAN:` (`…vs-ai_baka_deck123.jsonl` seq 99,
112, 147, 149, 150, 152, 163, 177; `…vs-ai_baka_deck152.jsonl` seq 58; `…vs-ai_baka_deck146.jsonl`
seq 75). Wave 70: 7 of 673 (1.0%); wave 71: 10 of 461 (2.2%). Wave 70's edit #1 removed the canned
plan sentence (that class fell 7.9% → 2.8%: 13 bare-sentence replies) but introduced the literal
string `"YOUR PLAN"` into the guide. Small numbers, so this is a hypothesis, not a proof — but it
costs nothing to test: this wave's guide drops the literal (edit 1 below). Independently, the parser
should accept `YOUR PLAN:` as the plan label; every one of the 10 carried a correct `CHOICE:` line.

**MED — A-4. The declined-count clause still loses a quarter of its windows, two waves running.**
65 windows printed `[you declined this exact list N times already this turn]`; the model took the
hold row in 35, `Cast nothing right now` in 17 (26%), something else in 13. Wave 70 measured ~20% on
the same rule. The guide states it in capitals in both waves and the number has not moved — this is
a render problem, not a guide problem. Wave 70's suggestion stands: when the declined-count clause
prints, drop the plain decline row or tag it `(this same question will be asked again this turn)`.

**LOW — A-5. `askreplay/` sidecar is empty for this seat and `ask_replays_reserved` is not.**
gameend counters read `ask_replays_reserved` 8/17/37/38/54/60 across the six games with
`ask_replays_refused` 0 and no files under `askreplay/`. Either reserved re-serves are not the event
that writes the sidecar, or the path is wrong. (Engine seat's call; flagging the inconsistency.)

**LOW — A-6. Cards verified.** `Elite Spellbinder` (borderline.txt:33380) is 3/1 flying with an
enters-trigger only, matching Scryfall; `Dwarven Blastminer` (mtg.txt:33251) has `{2}{R}, {T}:
Destroy target nonbasic land`, matching Scryfall. Both render truthfully. No card defect found.

## 3. Guide verdict: **EDIT** (`wave71/deck125/strategy.txt`, 19,981 bytes — 40 bytes *smaller* than
the live guide, so no rule needs to buy bytes; six trims listed at the end pay for the four additions)

| # | before → after | paid by |
|---|---|---|
| 1 | `… written fresh every window - never the sentence the prompt echoes back to you under "YOUR PLAN".` → `… written fresh every window; the plan the prompt quotes back at you is history, not an instruction.` | A-3 — the guide is the only place the literal string `YOUR PLAN` appears outside the prompt header, and the echo class doubled after it was added |
| 2 | TAP-OUT GATE: `With no counterspell in hand at all, the gate is open at any N.` → `… the gate is open at any leaves-count. THE GATE ONLY EVER DECIDES WHETHER YOU MAY AFFORD A ROW, NEVER WHETHER THE ROW DOES ANYTHING: a row whose own verdict reads zero - a sweeper at "destroys 0", a Lightmine Field at "0 creatures able to attack" - is still not a play, with an empty hand and an open gate.` | vs123 seq 7 **and** the accepted repeat at seq 8. The model's `reasoning` is explicit: *"Since I have 0 counterspells, I can spend mana. Lightmine Field is fine."* It reached ORDER OF OPERATIONS step 3 and the gate's `at any N` and never reached the LIGHTMINE FIELD section — and `N` names two different numbers in those two places |
| 3 | PING: one exception → **two**, the new one first: `A ping row that reads "DIES" AND carries "{removes N from the CRACK-BACK total": TAKE THAT ROW, at any life and whatever the body's own text says: the row did the arithmetic, N damage off you EVERY turn beats 1 off them ONCE …` (and the dead `Your plan line's "ping their face" is a default` sentence is dropped) | vs152 **seq 15**: row 1 read `Deal 1 damage … targeting Elite Spellbinder {right now: takes 1 damage - DIES} {removes 3 from the CRACK-BACK total above: 3 -> 0 - you would be at 17}`. The model checked the guide, found Spellbinder's text is an enters-trigger and so outside the `{T}:` / attacks / beginning-of list, and concluded *"Pinging face is correct per guide."* It went 17 → 13 → 11 → 4 → 2 and won at 7 with the opponent on 0. Wave 70's edit #4 wrote this rule; it is the rule that produced the miss |
| 4 | SWEEPERS, `K IS 0` bullet gains: `NO SURVIVAL CLAUSE IN THIS GUIDE REACHES K OF 0, whatever your life reads: a sweeper destroys what is a creature as it resolves, and a land that animates is not one until THEIR turn, after your sorcery window shut.` | vs152 seq 64, at 2 life against Lair of the Hydra. `reasoning`: *"The guide says for Lightmine: 'N of 0: Cast nothing right now, always.' So definitely not Lightmine. Supreme Verdict has a similar rule for K=0, but survival overrides. I'm confident."* The guide's own K=1 bullet (`unless your life is 10 or less`) and Lightmine's `SURVIVAL OUTRANKS THAT KILL TEST` are where that override came from |
| 5 | ELIXIR: `in that window, ahead of a Staff ping and ahead of any spell.` → `in THAT window, on a priority menu with a row 0 as much as on a casting menu, ahead of a Staff ping, ahead of any spell and ahead of the hold row.` | vs152 seq 117 — **2 life**, 14 untapped sources, row 1 = `Life with Elixir of Immortality {right now: shuffles your 16-card graveyard and this card back into your library …} [cost: {2}, Tap]`, and the model took `Hold priority`. It activated one turn later (seq 119). This was a priority menu (row 0 present); the guide's "that window" did not visibly reach it |
| 6 | Byte payers, no rule lost: the animating-land paragraph cut 5 lines → 3 (the wave-71 render now prints the count caveat *and* the crack-back total itself); the Forced Fruition and discard-punisher bullets compressed; the X-MENU, Staff-library, Path, converter and cleanup-tag paragraphs re-worded shorter; `Being far ahead on life is no exception.`, `and never cast something because a window opened.`, `and never open the target ask`, `- a spell they cast later opens its own window.` deleted as restatements | bytes only |

KEEP, explicitly: the tap-out gate arithmetic (no `{leaves N …}` misread in 461 decisions); the
counterspell type/power table (every counter spent this corpus was on-list — Fate Unraveler, Barrowin
3/3, Triumphant Adventurer, Kaya, Teferi, Elite Spellbinder, Sigarda); PATH's ordered list (vs146 seq
78/79 took Triumphant Adventurer on rule 2 and seq 82/83 took the 3/2 over a 1/1 on rule 3, both
correct); the Elixir **library** rule (activated at library 12 in vs130 and 13 in vs123, no deck-out
in two games past turn 45); LAND DROPS (85 land-drop asks, 85 lands played, tapped ones included);
CLEANUP DISCARD (33 discard windows, no keep this reader would fault); MULLIGAN.

Two guide non-compliances I am **not** editing for, because the rule is already stated plainly and
adding words is unlikely to move it: the Revelation SIZE FLOOR (vs126 seq 26/27 cast X=1 at 20 life;
`reasoning` shows the floor was never consulted) and Revelation stop (1) (vs162 seq 43/44 cast under
three Underworld Dreams at 7 life — the model was dead on the next draw step either way, and its
reasoning had already computed that).

## 4. Not checked
Opponent seats were read only where deck 125's own log was ambiguous (deck126 t18, deck152 t25).
I did not audit the `reveal`, `attackers` or `blocks` seams (this deck fielded no creature in any of
the six games), did not measure latency against wave 70, did not verify any card besides Elite
Spellbinder and Dwarven Blastminer against Scryfall, and adjudicated none of the lane-{BO,BP,BQ,BR,BS}
predictions — that is the engine seat's job. No general-guide or skill proposals: nothing here
generalises past this guide.
