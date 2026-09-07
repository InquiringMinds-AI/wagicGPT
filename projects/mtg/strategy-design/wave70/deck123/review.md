# Wave 70 — deck 123 (Intruders of Thraben, WBU token combo) — per-deck review

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-224849`, binary master 14c82df37, `--thinking on`.
My seat's six files are `...ai_baka_deck123-<ptr>-vs-<opp>.jsonl`. Record 1–6 = deck123 1 win, 5 losses.
Seat census: 216 decisions, 216 with native reasoning, **0 off_protocol_bytes, 0 action_before_plan,
0 truncations, 0 wall misses, 0 async drops, 0 transport errors**. 3 fallbacks, all one kind
(`noop_row_zero_reask`) and **all three false** — see HIGH-1. Latency median 107.8 s, p95 220.6 s,
max 421.8 s; `deadline_pct` max 46.5 (no seat decision came near the 900 s wall).
`reasoning_chars` median 6042, max 23488, zero records at 0; `reasoning_degenerate` max 0.0094
(no record above 0.01 — the trailing "Done. / Ready. / Proceeds." tails are short and did not
displace an answer).

## 1. Game by game

**vs deck130 — WIN, turn 13 (20 / −17).** The only game where the combo assembled. Doomsayer t5,
Alarm t7, Keeper t9, second Doomsayer t11. At **seq 23** the model read the stop correctly off the
window (L 16 + C 1 + 3 = 20, M 8) and took `CHOICE: 4 (Create vampire with Bloodline Keeper x6)`,
then **seq 24** `x6` again to land exactly on 20. Arithmetic right, no re-ask, no overshoot.
Cost noted, not decisive: **seq 18–22** were five windows of single taps and one hold (**seq 20**,
a hold taken with a live `Create human with Thraben Doomsayer #1 [cost: Tap]` row on the menu)
before it found the repeat row; the guide says take the repeat row once, and it took two — harmless
here because both landed on the stop.

**vs deck126 — LOSS, turn 16 (0 / 39). THE DECIDING DECISION IS seq 12.** Turn 10, life 20–20,
4 mana, board: Thraben Doomsayer + Human token + **Intruder Alarm already resolved** — the engine
was one untap trigger from running. Row 4 read
`Cast Damnation {right now: destroys 3 of their creatures (all of them carry a restriction against
attacking), 2 of yours - THEIRS: Pride Guardian #1 (0/3) [defender], Wall of Omens (0/4) [defender],
Pride Guardian #2 (0/3) [defender]; YOURS: Thraben Doomsayer (2/2), Human (1/1)}`.
The model took it, and its own PLAN said so out loud: *"clear their board of defenders **and my
tapped Doomsayer**"*. It swept its own maker to kill three creatures that could not attack, never
drew another maker, and lost 39–0. **Classification: STRATEGY, and the guide licensed it** — the
live guide's Damnation section reads "K at 0 on Perimeter Captain or Pride Guardian: cast it", and
two of the three were Pride Guardians. The M-gate only vetoes at M ≥ 4; M was 2 here. Guide edit 1.

**vs deck162 — LOSS, turn 12 (0 / 20). Deciding decision seq 12.** Turn 8, 4 mana, empty
battlefield, Keeper (row 1) and Alarm (row 4) both castable, only one affordable. It cast the
**Alarm on an empty board**. Its reasoning quotes the guide's P3 line as permission ("no Alarm on
your battlefield") and then convinces itself it can cast the Keeper in the same main phase —
"If I cast Alarm first, it resolves, then I cast Keeper" — although the row printed
`{leaves 1 source - rows 1, 2 and 3 need more mana sources than the 1 this leaves}`. **Classification:
STRATEGY** (the surface was true and explicit; the model misread its own mana), **plus a guide item**:
P3's first clause reads as the whole gate. Keeper first would have had a maker down two turns
earlier against a board (Underworld Dreams + Fate Unraveler + Dictate of Kruphix) charging it
4 life per draw step. Guide edit 3.

**vs deck152 — LOSS, turn 15 (−20 / 24). Two false re-asks decided the early game.** At **seq 11**
(t6) and **seq 13** (t7) the model chose row 1 `Cast Tragic Slip {kills: Elite Spellbinder}` /
`{kills: Elite Spellbinder, Intrepid Adversary}` and the engine refused both with
`noop_row_zero_reask` (HIGH-1). On the re-asks it backed down — **seq 12** hold, **seq 14**
`Cast nothing right now`. Both creatures lived, grew to 4/2s, and **seq 20** (t8) had to spend
Damnation on them; by t14 the opponent had six attackers for 22 and there was no sweeper left.
**Classification: PERCEPTION / ENGINE.** Secondary, not decisive: **seq 32** (t14, life 2, header
reading "up to 22 - you would be at −20; that would KILL you") cast Idyllic Tutor from exile for
{4}{w}, leaving 2 sources and no way to cast the Alarm the PLAN named.

**vs deck146 — LOSS, turn 13 (0 / 20).** Mana-light (3 lands through t12, Keeper x2 stranded at
4 mana) — that part is variance, not guide effect. The guide-attributable decision is **seq 17**
(t12, main 1): the menu carried a LIVE `Cast Idyllic Tutor {2}{w} [finds only an enchantment card -
still in your library...: Intruder Alarm]` (P4's live bracket) and a Devour Flesh row whose own
crack-back cover computed survival either way; the model took **row 5, the hold**, and did nothing
for the rest of the game. Its reasoning names the guide line it used: *"Take 'Hold priority...' at
the FIRST dead window of the turn"*. `hold_windows_skipped_cast` for this game: **76**.
**Classification: STRATEGY + guide.** Guide edit 2.

**vs deck125 — LOSS, turn 41 (0 / 40, opponent ended at 175 in results.tsv).** Prison/control:
Elixir of Immortality, two Staff of Nin, Supreme Verdict and Essence Scatter. 88 decisions over 41
turns and **343 cast windows skipped by holds**. The whole midgame is `Hold priority` / `Cast
nothing right now` alternating. Two items: (a) the hold pattern again — **seq 20, 25, 27, 31, 40,
43, 46, 51, 53, 56, 60, 70, 72, 77, 80, 84** are holds taken on Casting decision menus; (b)
**seq 59** (t32, 9 untapped sources, Supreme Verdict AND Essence Scatter already in their graveyard,
dead Damnation and Lightning Greaves on the SAME menu) cast the lone Thraben Doomsayer with no
bait, and it was gone by t34. The guide's "THE ONE TIME A DEAD ROW IS A PLAY" rule covers exactly
this and was not applied. Guide edit 4. Discards were all correct (checked seq 10, 21, 44, 47, 57,
66 — the t34 Alarm discard was a genuine second copy, `[you already control one]`).

## 2. Engine / interface / card items

**HIGH-1 — `noop_row_zero_reask` fires on every non-Morbid Tragic Slip cast row, including rows the
same prompt says are lethal.** 3 of 3 fires in this seat are false; the guard is deterministic, so
it will fire on every such take corpus-wide.
Repro: `1788752940-ai_baka_deck123-0x55875fed49a0-vs-ai_baka_deck152.jsonl`, **seq 13**, rendered row:
`1. Cast Tragic Slip {b} {right now: -1/-1 (no creature has died this turn, so Morbid does NOT apply)} {leaves 2 of your 3 untapped mana sources untapped} {kills: Elite Spellbinder, Intrepid Adversary} ...`
Reply `CHOICE: 1 (Cast Tragic Slip)` → `"fallback": "noop_row_zero_reask"`, `chosen_text` `<refused>`.
Root cause (read-only): `src/AIPlayerGPT.cpp:27306 noopRowEarnsReask` → `AIPlayerGPT::rowSaysNoOp`,
which returns true on `low.find("does not apply")`. That substring is the **Morbid** clause, a
statement about which of two magnitudes applies, not about the row doing nothing — and it is
printed on every Tragic Slip row where no creature has died. The row's `{kills: <name>}` clause,
the engine's own live verdict, is right next to it and is not consulted. Same two other fires:
same file **seq 11**; `...vs-ai_baka_deck146.jsonl` **seq 4**. Cost: at 146 seq 4→5 one window and
~104 s (same answer re-sent and executed); at 152 seq 11→12 and 13→14 the model believed the
re-ask's wording — *"whose own note on this list says it does nothing right now"* — and abandoned
two printed kills. Suggested fix shape: exempt a row that also carries `{kills: ...}`, and scope
the `does not apply` cue so it cannot match a conditional-magnitude clause.

**MED-2 — the hold row is offered on Casting decision menus and silences the whole seam.** The
guide can be tightened (edit 2 below), but the interface is the amplifier: on a cast menu the hold
row sits beside `Cast nothing right now`, and the two read as the same decline while one costs the
turn's remaining cast windows. Counters, this seat: `hold_windows_skipped_cast` 343 (vs125), 76
(vs146), 60 (vs126), 44 (vs152) — versus `hold_windows_skipped_priority` 30 / 0 / 21 / 19. The
skipped windows are overwhelmingly CAST windows. Repro: `...vs-ai_baka_deck146.jsonl` **seq 17**,
row 5 taken over a live Idyllic Tutor row 4. Cheapest change that would price it: say on the hold
row of a CAST menu what it costs — "this also skips this turn's remaining casting decisions" —
rather than the current generic "you give up no cast", which is false in effect if not in letter.

**MED-3 — `[repeat: ... activated this turn N times already]` appears on a plain Create row before
the `repeated N times, then stop` row appears at all.** `...vs-ai_baka_deck130.jsonl` **seq 18**
prints `[repeat: you control 5 creatures; activated this turn 1 times already. This turn will not
advance while you keep taking this option.]` on the single-tap row, and the repeat-count row only
shows up at **seq 23/24**. The model spent seq 18–22 (five windows, ~9 min of wall clock) tapping
one at a time and holding once before the batch row existed. If the batch row can be offered as
soon as the plain Create row repeats, the loop costs 2 decisions instead of 7.

**LOW-4 — `YOUR PLAN (as you last stated it, 1 window ago on turn N)` is rendered as a truncated
fragment.** `...vs-ai_baka_deck146.jsonl` **seq 19** renders it as
`YOUR PLAN (...): then cast Bloodline Keeper to begin building an army and attack next turn.` —
the plan opens on "then". Same shape at 152 seq 32 and 146 seq 18. The 400-char plan cut is landing
mid-sentence and the carried plan reads as a continuation of nothing.

**LOW-5 — Idyllic Tutor cast from exile prices at {4}{w} with no cross-reference in the guide.**
`...vs-ai_baka_deck152.jsonl` **seq 32**:
`2. Cast Idyllic Tutor {4}{w} [from exile] {castable from exile - your card, exiled by their Elite
Spellbinder, which lets you cast it from there; it costs {2} more than printed, already counted}`.
The render is correct and clear (verified against Elite Spellbinder's Oracle text); noting it only
because every guide line names the card at its printed {2}{w}.

**Card facts verified against primitives** (`bin/Res/sets/primitives/mtg.txt`): Tragic Slip
(l.123128, `auto=ifnot morbid then -1/-1` / `if morbid then -13/-13`) and Pride Guardian (l.88356,
`abilities=defender`, `auto=@combat(blocking) source(this):life:3 controller`) both match what the
prompts render and what the guide says about them. No card-script defect found in this seat.

## 3. Guide verdict: **EDIT**

Revised guide at `wave70/deck123/strategy.txt` (**19,936 bytes**, under the 20,000 ceiling; the live
guide is 19,772, so the four edits are paid for with two cuts — see the last row). No citations,
counts or history in the guide itself.

| # | before → after | seq that paid for it |
|---|---|---|
| 1 | Damnation M-gate: `M at 4 or more, or N at 0 ... answer another row.` → same, **plus** `A TOKEN-MAKER OF YOURS INSIDE THE "M of yours" LIST IS A VETO OF ITS OWN: if that list names Thraben Doomsayer, Bloodline Keeper or Lord of Lineage, do not write Damnation's number, whatever N and K say - that is your win condition, and you are the one sweeping it away.` And `K at 0 on Perimeter Captain or Pride Guardian: cast it.` → `... cast it ONLY at M = 0 - those are walls, and walls standing cost you nothing.` | 126 **seq 12** (M=2 including its own Doomsayer, K=0, three defenders; game lost 39–0). The bytes are earned by the single largest decision loss in the six games: the old carve-out named Pride Guardian with no M bound, and the board it was written for was M=0. |
| 2 | `Take "Hold priority..." at the FIRST dead window of the turn, and answer a reopened run the same way every time.` → `THE HOLD ROW IS A P8 ANSWER AND NOTHING ELSE: take it only when EVERY row on THIS window is dead by the order above. One live row - a maker, a live Tutor bracket, a "{kills: <name>}" removal row, a Create row - and the hold is wrong however poor the window looks.` + `On a Casting decision (the menu that says it has no row 0) decline with "Cast nothing right now", never with the hold: the hold there silences every later CAST window while the rows stand.` + `Once it is right, answer a reopened run the same way every time.` | 146 **seq 17** (held over a live P4 Tutor row and never acted again; 76 cast windows skipped), 125 **seq 20/25/27/31/40/43/46/51/53/56/60/70/72/77/80/84** (343 skipped), 152 **seq 12**, 130 **seq 20** (held with a live Create row mid-loop). The old line's "first dead window" was the exact sentence the model quoted while holding a live menu; it needed a bound, not a deletion. |
| 3 | `P3 Intruder Alarm {2}{u} - no Alarm on your battlefield; once ONE maker is down this beats a second maker and the Greaves.` → `P3 Intruder Alarm {2}{u} - ONLY with a maker ALREADY on your battlefield and no Alarm there; then it beats a second maker and the Greaves. With "of which 0 are creatures" on your own line the Alarm is not a play at any price - a maker row on the same menu is.` | 162 **seq 12** (Alarm cast on an empty board over an affordable Keeper; the reasoning quotes the old first clause as its permission). Rewrite, ~+120 bytes: the gate moves from the second clause, where the model stopped reading, to the first. |
| 4 | `against counterspells in their log (Cancel, Essence Scatter, Dream Fracture, Fall of the Gavel) they counter the FIRST spell you cast` → `against counterspells or a sweeper in their log OR their graveyard (Cancel, Essence Scatter, Dream Fracture, Fall of the Gavel, Supreme Verdict) they answer the FIRST spell you cast` | 125 **seq 59** (9 mana, Supreme Verdict and Essence Scatter both already in their graveyard, dead Damnation on the same menu, lone maker cast unbaited). ~+60 bytes: the rule existed and the trigger surface did not name the zone the evidence was actually in. |
| — | **CUTS paying for the above (−745 bytes):** the two SITUATIONS bullets that restate the Damnation section and the P3/Alarm-timing rules verbatim, and the two that restate the "NAMING Yourself" and edict-vs-defender rules verbatim. The stop-arithmetic worked example is kept — it is the only one in the file carrying a number the model cannot derive from a rule. | Amendment 332: decision-time instruction only; a worked example that repeats a rule stated 100 lines above it is bytes with no decision behind them. |

## 4. Optional proposals
None. The general-guide file is not loaded at runtime, and nothing this seat found is a
strategy-writing-skill gap rather than a deck-guide or engine gap — the four edits above are all
scoped to this deck's cards, and HIGH-1/MED-2/MED-3 are engine items, not writing-method items.

## 5. What I did NOT check
- The opponent seat files were read only for board context on the six games; I did not audit the
  opponents' decisions, and the deck125 game's turns 1–8 and 20–30 I sampled rather than traced
  window by window (88 records).
- I did not verify any prediction from `wave70/lane-{BK,BL,BM,BN}.md` — that is the engine seat's
  adjudication, and I deliberately did not pre-empt it.
- I did not check whether HIGH-1's guard misfires on cards outside this deck; `rowSaysNoOp`'s
  `"does not apply"` cue is generic, so other conditional-magnitude cards are likely affected, but
  I have no evidence from my six games.
- No build, no run, no git; nothing under `bin/Res` or `src/` was edited.
